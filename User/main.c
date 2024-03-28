#include "main.h"
/*
SYSCLK	144000000
HCLK		144000000
PCLK1		144000000
PCLK2		72000000
ADCCLK	9000000

 */

//u8 *p=_IQMathVerStr();

 _signal	sysvar={RGB_Uunwanted_Handler,SysDormancy};
 fstack_t g_fstack;
 msgq_t g_msgq;
 msg_t msg;
 func_t func;
 u16 g_tick;			/** 定时器计数 **/
 u16 g_flag;
 /**
  * g_flag:
  * bit 0: 
  * bit 1: wait for double click 
  * bit 2..16 reserved
  **/
 Timer_t g_timer[TIMER_NUM];		/** g_timer[0] for global, g_timer[1] for uart3 receive, g_timer[2] for key...  **/
 /**
  * g_timer[0] for system tick
  * g_timer[1] for uart3 timer out
  * g_timer[2] for key
  **/
u8FIFO_t g_uart3TxQue;
u8FIFO_t g_uart3RxQue;

u8 g_sm_wifiInitStatus;
/**
 * ��ʼ��״̬��
 * 0 --- sm_none (wait for REQ getDevInfo message)
 * 1 --- sm_init (ACK the getDevInfo message, then wait for reportService)
 * 2 --- sm_normal (initial completed)
 * 3 --- 
 **/
 ComponentField_t g_componentStatus = {0};
/** 
 * the latest status changed and record the current status
 * the status used only once for status upload report
 **/
NetInfo_t g_netInfo;
/**
 * record wifi net information
 **/
/*********************************************************************************/
 //SysTick初始化配置
 uint32_t SysTick_Config(uint64_t ticks){
     SysTick->CTLR = 0x0000;
     SysTick->CNT = 0;
     //设置计数比较寄存器的值，主要用于触发STK中断服务函数。当CNT[63:0]与CMP[63:0]值相等时将触发STK中断服务。
     SysTick->CMP = (u64)(ticks);
     NVIC_SetPriority(SysTicK_IRQn, 15);      //设置SysTick中断优先级
     NVIC_EnableIRQ(SysTicK_IRQn);            //使能开启Systick中断
     SysTick->CTLR |=  (1 << 5) | (1 << 3)| (1 << 1) | (1 << 0);                  //启动系统计数器STK（HCLK/8时基） 72000000/8=9000000
     return (0);
 }

RCC_ClocksTypeDef RCC_Clocks;

int main(void){
//	_IQMathVerStr();
    NVIC_PriorityGroupConfig(0x02);
    //SysTick_Config(SystemCoreClock / 8000);//            72M/8000/9000000 = 1/1000 = 1ms
    SysTick_Config(SystemCoreClock / 800);//                 10ms
//    USART_Printf_Init(115200);
    TIM1_PWMOut_Init(1000-1, 9-1,0);
    IO_Init();
    ADC_AnyChanInit();
    Voice_Init();
//    TIM_Init(1000,144);
    ID_Init();
    display_Init();

//    ID_Send_Group("ID OK",5);

//    wifi_Init();
    RCC_GetClocksFreq(&RCC_Clocks);

//    IDs_.Equipment=f_DragLala;
//		VOIC_Control(VTx_ConnectDragLala,0);
//		UpdataRGBmoodo(Rgb_ERREquipment); // 

    cw_bat.ErrF=cw221x_bat_init();

	MGPIO_Toggle_INIT();
    //wifi_Init();
    USART3_CFG();
    g_flag = 0;
    g_sm_wifiInitStatus = sm_none;
    netInfoData_init();
    memset(&g_componentStatus, CINDEX_UNKNOW, sizeof(g_componentStatus));
    fstack_init(&g_fstack);
    func.func = f_idle;
    fstack_push(&g_fstack, &func);
    for(int i = 0; i < MTABSIZE(g_timer); i++) {
        ClrTimer(&g_timer[i]);
    }
	SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
	SetTimer_irq(&g_timer[2], TIMER_1SEC, CCMPNT_STATUS);
		
	u8FIFOinit(&g_uart3TxQue);
	u8FIFOinit(&g_uart3RxQue);
    
    msgq_init(&g_msgq);
    msg.msgType = CSYS_INIT;
    msgq_in_irq(&g_msgq, &msg);

    while(1){
#if 1
  		if(DMA_GetFlagStatus(DMA1_IT_TC1)){
  			DMA_ClearFlag(DMA1_IT_TC1);
  			sysvar.LoadHandler();
				Gat_ADC_Avg();

				if(++sysvar.t2>9){//10mS
					sysvar.t2=0;
					KEY_scanning();
					RGBvar.RGBhandler();
					CHruning();
					IDs_Judge();
				}

				if(++sysvar.t1>99){
					sysvar.t1=0;

					Voice_exe();
					moto_run();
					perform();
					Power_off_Mode();//低功耗

				}

//
//				if(++sysvar.t9>100){
//					sysvar.t9=0;
//
//					GPIO_WriteBit(GPIOA, GPIO_Pin_12, GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)?0:1);
//    		}
    	}
//  		wifi_uart_service();
// #else
    DAEMON_USART3_Send(&g_uart3TxQue);
    if(msgq_out_irq(&g_msgq, &msg) == FALSE) {     	/** ����Ϣ��? **/
        continue;
    }
    if(sysProcess((unsigned *)&msg) == TRUE) {   			/** ��ϵͳ��Ϣ��? **/
        continue;
    }

    if(fstack_top(&g_fstack, &func) == FALSE) {    /** ��ǰ����״̬ **/
	    /** something wrong happend, Power Down or recover it **/
        fstack_init(&g_fstack);
        func.func = f_idle;
        fstack_push(&g_fstack, &func);
	
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        continue;
    }
    func.func((unsigned *)&msg);
#endif
    }
}

