#include "main.h"



void wifi_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;//9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(USART3, ENABLE);
    wifi_protocol_init();
}



/*********************************************************************
 * @fn      USART3_CFG
 *
 * @brief   Initializes the USART3 peripheral.
 *
 * @return  none
 */
void USART3_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
    #if 0
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif
    /* USART3 TX-->B.10  RX-->B.11 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    // USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //USART_Init(USART2, &USART_InitStructure);
    //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    //NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //USART_Cmd(USART2, ENABLE);
    USART_Cmd(USART3, ENABLE);
}


void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        u8Data_t u8data;
        u8data.u8Val = USART_ReceiveData(USART3);
        u8FIFOin(&g_uart3RxQue, &u8data);

        msg_t msg;
        msg.msgType = CMSG_UART3RX;		/** message CMSG_UART3RX **/
        msgq_in(&g_msgq, &msg);

        SetTimer(&g_timer[1], TIMER_1SEC, CUART3_TMROUT);
	}
}

void wifi_Send_Group(const u8 Data){
	//while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	//USART3->DATAR = (Data);
	
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, Data);
}


/*开关机上报*/
void wifi_Upload_GO(void){
	mcu_dp_bool_update(DPID_SWITCH_GO,(sysvar.sysfang & OFF_ON?1:0)); //BOOL型数据上报;
}

/*自清洁上报*/
void wifi_Upload_CLEANO(void){
	mcu_dp_bool_update(DPID_SELF_CLEAN,(sysvar.Modes==MODE_CLEANING?1:0)); //BOOL型数据上报;
}

/*当前模式上报*///smart=0,water_absorb=1,strong=2,sterilization=3,steam=4,wooden_floor=5,ceramic_tile=6,carpet=7,none=8
void wifi_Upload_MODE(void){
	u8 mode=8;
	if(sysvar.Modes==MODE_1)mode=1;
	else if(sysvar.Modes==MODE_2)mode=2;
	else if(sysvar.Modes==MODE_RINSE)mode=3;
	else if(sysvar.Modes==MODE_CLEANING)mode=4;
	mcu_dp_enum_update(DPID_MODE,mode); //枚举型数据上报;
}


/*当前当前状态*/// standby=0,清洁中cleaning=1,  charge_done=2, 中charging=3,智能清洁smart_clean=4,睡眠sleep=5,自清洁中self_clean=6
void wifi_Upload_STATUS(void){
	u8 STATUS=0;
	if(sysvar.Modes==MODE_1)STATUS=1;
	else if(sysvar.Modes==MODE_2)STATUS=1;
	else if(sysvar.Modes==MODE_RINSE)STATUS=4;
	else if(sysvar.Modes==MODE_CLEANING)STATUS=6;
	else if(sysvar.batfang & (BATCHining))STATUS=3;
	else if(sysvar.batfang &	BACHready)STATUS=2;//  
	mcu_dp_enum_update(DPID_STATUS,STATUS); //枚举型数据上报;
}



/*
	mcu_dp_enum_update(DPID_SUCTION,当前吸力); //枚举型数据上报;
	mcu_dp_enum_update(DPID_CISTERN,当前水量); //枚举型数据上报;
*/

/* 上报*/
void wifi_Upload_SOC(void){
	mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,(unsigned long )sysvar.BAT_soc); //VALUE型数据上报;
}

/*当前故障告警
滚刷卡住01
风道堵塞02
风机异常04
 耗尽08
电池过温10
脏污传感器20
滤网异常40
适配器错误80
*/
void wifi_Upload_FAULT(void){
	u8 FAULT=(
	((sysvar.CH_state==CH_ERR)?0x80:0)|
	((sysvar.sysfang & MOTO_ERR_1)?0x01:0)|
	((sysvar.sysfang & MOTO_ERR_2)?0x02:0)|
	((IdSensor_fang(IdSensor_SEWAGE))?0x20:0)|//污水
	((IdSensor_fang(IdSensor_PUMP))?0x40:0)|//水泵不再位
	((IdSensor_fang(IdSensor_PUMPERR))?0x10:0)|//水泵
	((sysvar.batfang & BATpowers_ERR)?0x08:0));
	mcu_dp_fault_update(DPID_FAULT,FAULT); //故障型数据上报;
}


unsigned char wifi_download(const unsigned char DP,const unsigned char var){
  switch(DP) {
      case DPID_SELF_CLEAN://自清洁开关
				if(var == 0) {
						//bool off
				}else {
						//bool on
				}
      break;

      case DPID_MODE: 
        switch(var) {
            case 0:
            break;

            case 1:
            break;

            case 2:
            break;

            case 3:
            break;

            case 4:
            break;

            case 5:
            break;

            case 6:
            break;

            case 7:
            break;

            case 8:
            break;

            default:

            break;
        }
      break;

      case DPID_SUCTION://吸力
        switch(var) {
            case 0:
            break;

            case 1:
            break;

            case 2:
            break;

            case 3:
            break;

            default:

            break;
        }
      break;

      case DPID_CISTERN:
        switch(var) {//水量
            case 0:
            break;

            case 1:
            break;

            case 2:
            break;

            case 3:
            break;

            default:

            break;
        }
      break;

      default:

      break;
  }
  return 0;

}






