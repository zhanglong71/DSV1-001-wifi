#include "main.h"

#define _RGB_C_

RGB_TypeDef RGBvar={RGB_DelayOFF_Handler,0,{0},{0},1000};

//UART1
 void DMA_TX1_Config(){
	DMA_InitTypeDef DMA_InitStructure = {0};
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&RGBvar.Show;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 11;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}





void display_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;//9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);

    RGBvar.Show.EndMark1=0XDD;
    RGBvar.Show.EndMark2=0XAA;

    DMA_TX1_Config();

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}



void Disp_Send_Group(){
	RGBvar.Show.S_Sum=RGBvar.Show.S_SOC+RGBvar.Show.S_Modo+RGBvar.Show.S_WiFi+RGBvar.Show.S_RGB+RGBvar.Show.S_Brig+RGBvar.Show.S_R_Brig+RGBvar.Show.S_G_Brig+RGBvar.Show.S_B_Brig;
	while(!(DMA1->INTFR&DMA1_FLAG_TC4));
	DMA1->INTFCR = DMA1_FLAG_TC4;
	DMA1_Channel4->CNTR = 11;
	DMA1_Channel4->CFGR |= DMA_CFGR1_EN;
}



void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		sysvar.data=USART_ReceiveData(USART1);
		if(RGBvar.Show.S_Sum!=sysvar.data){
			sysvar.sysfang |= SysDormancy;
		}else{
//			if(keybuf == 0xff)sysvar.sysfang |= KeyDormancy_quit;//按键松开状态下无需按键激活
			sysvar.sysfang &= ~SysDormancy;
		}
	}
}



//void Disp_Send_Group_D2(const u8 Data){
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//    USART1->DATAR = (Data);
//}
//
//void UART2_Send_Group(u8* buf, u8 len){
//  while(len--)
//  	Disp_Send_Group_D2(*buf++);
//}









void RGB_DelayOpen_Handler(){
	if(++RGBvar.RGBtim>RGBvar.frequency){
		UpdataRGBmoodo(RGBvar.state);
	}
}


void RGB_DelayClosing_Handler(){
	if(++RGBvar.RGBtim>RGBvar.frequency){
		if(sysvar.sysfang & (MOTO_ERR_1))// 报警
			IDs_Set_Data(0,0,0,0);
		sysvar.sysfang &= ~(MOTO_ERR_1);
		sysvar.keyfang &= ~(KeyInstallState|MOTO_ERR_2);
		sysvar.batfang &= ~(BATpowers_ERR);
		RGBvar.Show.S_WiFi &= 0xfc;
		IDs_.fang &= ~ID_F_ERR_;
		UpdataRGBmoodo(Rgb_Await);
	}
}



void RGB_DelayOFF_Handler(){
	if(++RGBvar.RGBtim>RGBvar.frequency){
		sysvar.sysfang &= ~(MOTO_ERR_1);
		sysvar.keyfang &= ~(KeyInstallState);
		IDs_.fang &= ~ID_F_ERR_;
		UpdataRGBmoodo(Rgb_Off);
	}
}

/* */
void RGB_SingleFlashingLEDG_Handler(){
	if(++RGBvar.RGBtim>RGBvar.frequency){
		RGBvar.RGBtim=0;
		if(RGBvar.direction){
			RGBvar.direction=0;

			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=RGBvar.ShowBRK.S_Modo;
			RGBvar.Show.S_RGB=SH_Rgb_Style1|SH_Rgb_EN;		//RGB
			Disp_Send_Group();

		}
		else{
			RGBvar.direction=1;
			RGBvar.FlickerNum--;
			if(RGBvar.FlickerNum){

				RGBvar.Show.S_SOC=sysvar.BAT_soc;
//				if(RGBvar.state==Rgb_ERRSewageProtect)RGBvar.Show.S_Modo=0;
				RGBvar.Show.S_RGB=0;		//灭
				Disp_Send_Group();
			}else{

				if(sysvar.BAT_soc<100 && !(sysvar.sysfang & OFF_ON)){
					UpdataRGBmoodo(Rgb_Await);//不在 和运行
				}
				sysvar.sysfang &= ~(MOTO_ERR_1);
				sysvar.keyfang &= ~(KeyInstallState);
				sysvar.batfang &= ~(BATpowers_ERR);
				IDs_.fang &= ~ID_F_ERR_;
			}
		}
	}
}


void RGB_Uunwanted_Handler(void){
}

void RGB_OFF_Handler(void){
}


void UpdataRGBmoodo(const RGBstate_TypeDef temp){
	sysvar.stop_tim=0;
	switch (temp){
		case Rgb_RUNingStandard:{// 
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_M2|(IdSensor_fang(IdSensor_CLEAR|IdSensor_SEWAGE)?SH_Modo_ERR:0);
//			RGBvar.Show.S_WiFi=SH_Wifiing|SH_VOICE;
			RGBvar.Show.S_RGB=((sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2))?SH_Rgb_Style3:SH_Rgb_Style1)|SH_Rgb_EN;//
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			break;
		}
		case Rgb_RUNingStrong:{// 
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_M3|(IdSensor_fang(IdSensor_CLEAR|IdSensor_SEWAGE)?SH_Modo_ERR:0);
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=((sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2))?SH_Rgb_Style3:SH_Rgb_Style1)|SH_Rgb_EN;//RGB-蓝色旋转
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			break;
		}
		case Rgb_RUNingStrong2:{
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_M1|(IdSensor_fang(IdSensor_CLEAR|IdSensor_SEWAGE)?SH_Modo_ERR:0);
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=((sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2))?SH_Rgb_Style3:SH_Rgb_Style1)|SH_Rgb_EN;
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			break;
		}
		case Rgb_RUNingCleaning:{// 
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_M1|(IdSensor_fang(IdSensor_CLEAR|IdSensor_SEWAGE)?SH_Modo_ERR:0);
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=((sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2))?SH_Rgb_Style3:SH_Rgb_Style1)|SH_Rgb_EN;
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 

			break;
		}
		case Rgb_CHing:{// 中
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style2|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			Voice.vfang &= ~VOICE_ching_f;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			if(IDs_.Equipment==f_DragLala){//拖拉啦
				IDs_Set_Data(0,0,0,sysvar.CH_state?ID_Fang_OZ:0);// 
			}
			break;
		}
		case Rgb_CHerr:{// 错误-- 
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_ERR;
			RGBvar.Show.S_RGB=SH_Rgb_Style3|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;//RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			RGBvar.RGBtim=0;
			VOIC_Control(VTx_CHErr,0);
			break;
		}
		case Rgb_CHover:{//充满电
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style1|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			break;
		}

		case Rgb_ERRBACH_lowining:{// 预警
			if(RGBvar.state==temp)return;

			break;
		}

		case Rgb_ERRBACH_lowin:{//   ----  、天眼红色
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style3|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			RGBvar.direction=0;
			RGBvar.RGBtim=0;
			break;
		}
		case Rgb_ERRProtect:{// 保护
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_ERR2;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style3|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			RGBvar.direction=0;
			RGBvar.RGBtim=0;
			if(IDs_.Equipment==f_DragLala)
				VOIC_Control(VTx_RollerErr,0);//滚筒故障
			else
				 VOIC_Control(VTx_DeviceErr,0);//设备故障
			break;
		}
		case Rgb_ERRSewageProtect://污水满保护
			if(RGBvar.state==temp)return;
			sysvar.sysfang &= ~(OFF_ON);
			sysvar.t1=1000;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_ERR;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style1|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;// 
			VOIC_Control(VTx_sewageErr,0);
			break;
		case Rgb_ERRNoPumpProtect:{//水泵不在位--红色常亮，故障灯 
			if(RGBvar.state==temp)return;
			sysvar.sysfang &= ~(OFF_ON);
			sysvar.t1=1000;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_ERR2;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style3|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			RGBvar.direction=0;
			RGBvar.RGBtim=0;
			IDs_.State_Hall &= ~(IdSensor_PUMP|IdSensor_PUMPERR);// 
			IDs_.State_HallBRK &= ~(IdSensor_PUMP|IdSensor_PUMPERR);// 
			VOIC_Control(VTx_PumpErr,0);
			break;
		}
		case Rgb_ERRPumpProtect:{//水泵保护--红色常亮，故障灯 
			if(RGBvar.state==temp)return;
			sysvar.sysfang &= ~(OFF_ON);
			sysvar.t1=1000;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=SH_Modo_ERR2;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style3|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			RGBvar.direction=0;
			RGBvar.RGBtim=0;
			IDs_.State_Hall &= ~(IdSensor_PUMP|IdSensor_PUMPERR);// 
			IDs_.State_HallBRK &= ~(IdSensor_PUMP|IdSensor_PUMPERR);// 
			VOIC_Control(VTx_PumpErr,0);
			break;
		}
		case Rgb_ERRNoEquipment:{//无设备-红灯亮一下
//			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=0;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_Uunwanted_Handler;//RGB_DelayClosing_Handler;// 
			RGBvar.frequency=100*(20)-1;//  *10ms
			IDs_.fang &= ~ID_F_EquipmentReady;//设备断开
			sysvar.t6=0;
			RGBvar.frequency=80-1;
			RGBvar.RGBtim=0;
			break;
		}
		case Rgb_ERREquipment:{// 
//			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style1|SH_Rgb_EN;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;// 
			IDs_.fang |= ID_F_EquipmentReady;//设备已连接
			sysvar.keyfang &= ~(KeyInstallState|KeyLongFang|keyRGBDelayOpen);//关闭设置模式
			sysvar.t6=0;
			RGBvar.frequency=80-1;//  *10ms
			RGBvar.RGBtim=0;

			break;
		}
		case Rgb_Install:{
			if(RGBvar.state==temp && !(sysvar.keyfang & keyRGBDelayOpen))return;
			if(RGBvar.ShowBRK.S_WiFi != RGBvar.Show.S_WiFi &&
					RGBvar.RGBhandler==RGB_DelayClosing_Handler){
				sysvar.keyfang |= keyRGBDelayOpen;
				RGBvar.Show.S_SOC=sysvar.BAT_soc;
				RGBvar.Show.S_Modo=0;
				RGBvar.Show.S_RGB=0;
				RGBvar.Show.S_Brig=0XFF;
				Disp_Send_Group();
				RGBvar.ShowBRK=RGBvar.Show;
				RGBvar.RGBhandler=RGB_DelayOpen_Handler;
				RGBvar.frequency=20-1;
				RGBvar.RGBtim=0;
				break;
			}
			sysvar.keyfang &= ~keyRGBDelayOpen;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=SH_Rgb_Style1|SH_Rgb_EN;
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayClosing_Handler;
			RGBvar.frequency=100*(20)-1;
			RGBvar.RGBtim=0;
			break;
		}
		case Rgb_Await:{//熄屏
			if(RGBvar.state==temp)return;
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			RGBvar.Show.S_Modo&=~(SH_Modo_M1|SH_Modo_M2|SH_Modo_M3);
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=0;		//RGB
			RGBvar.Show.S_Brig=0XFF;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_DelayOFF_Handler;
			sysvar.keyfang &= ~(KeyInstallState|KeyLongFang|keyRGBDelayOpen);//关闭设置模式
			sysvar.t6=0;
			Voice.vfang |= VOICE_ching_f;
			RGBvar.frequency=100*(10)-1;
			RGBvar.RGBtim=0;
			break;
		}
		case Rgb_Off:
			sysvar.Show_D2 =0;
			RGBvar.Show.S_SOC=201;
			RGBvar.Show.S_Modo=0;
			RGBvar.Show.S_Modo=0;
//			RGBvar.Show.S_WiFi=0;
			RGBvar.Show.S_RGB=0;
			RGBvar.Show.S_R_Brig=0;
			RGBvar.Show.S_G_Brig=0;
			RGBvar.Show.S_B_Brig=0;
			RGBvar.Show.S_Brig=0X0;
			Disp_Send_Group();
			RGBvar.ShowBRK=RGBvar.Show;
			RGBvar.RGBhandler=RGB_OFF_Handler;
			Voice.vfang |= VOICE_ching_f;
			RGBvar.state=0;
			RGBvar.RGBtim=0;
			RGBvar.Duty=0;
			RGBvar.frequency=0;
			RGBvar.direction=0;
			RGBvar.RGB_Prepare_=0;
			RGBvar.RGB_Prepare_brk=0;
			break;

		default:break;
	}
	RGBvar.state=temp;
}













