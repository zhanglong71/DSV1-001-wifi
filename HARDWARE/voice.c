#include "main.h"


#define _VOICE_C_

#define UART1baudrate 9600
u8 USART_RX_BUF;

Voice_TypeDef Voice;

void Voice_Init(){
	GPIO_InitTypeDef GPIO_InitStructure={GPIO_Pin_3,GPIO_Speed_50MHz,GPIO_Mode_AF_PP};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={144,TIM_CounterMode_Up,1000,TIM_CKD_DIV1,0};
	TIM_OCInitTypeDef 			TIM_OCInitStructure=	{0};
	NVIC_InitTypeDef				NVIC_InitStructure = 	{0};

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE );

	GPIO_Init( GPIOB, &GPIO_InitStructure );
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//
	TIM_OCInitStructure.TIM_Pulse = 500;	//
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//
	TIM_OC2Init(TIM2,&TIM_OCInitStructure );

	AFIO->PCFR1 |= 1<<8;

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	TIM_CtrlPWMOutputs(TIM2,ENABLE);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Disable);//
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);

	Voice.vfang |= VOICE_ching_f;

}






void VOIC_Control(const u8 stack,const u8 Waiting){
	if(Waiting==0){//最高优先级
		Voice.tim=(Voice.vfang & VOICE_InitFang)?1:7;//
		Voice.VoiceController.Cont16=(u16)stack;
	}else if(Waiting==1){
		if(Voice.VoiceController.Cont16)//低优先级
			Voice.VoiceController.Cont8.ContB=stack;
		else Voice.VoiceController.Cont16=(u16)stack;
	}
}


u8 Voice_ReadStack(){
	u8 stack=Voice.VoiceController.Cont8.ContA;
	Voice.VoiceController.Cont16>>=8;
	return stack;
}



void Voice_exe(){
	if(Voice.tim)Voice.tim--;
	else if(!(Voice.vfang & VOICE_Waiting)){//
		switch (Voice_ReadStack()){//
			case VTx_ConnectNo://
				Change_List_Sound(VTx_ConnectNo);
				Voice.tim=30;//3
				break;
			case VTx_Disconnect://
				Change_List_Sound(VTx_Disconnect);
				Voice.tim=30;
				break;
			case VTx_Install://
				Change_List_Sound(VTx_Install);
				Voice.tim=30;
				break;
			case VTx_VoiceOpen://
				Change_List_Sound(VTx_VoiceOpen);
				break;
			case VTx_VoiceClose://
				Change_List_Sound(VTx_VoiceClose);
				break;
			case VTx_WifiReset://
				Change_List_Sound(VTx_WifiReset);
				Voice.tim=30;
				break;
			case VTx_WifiConnecting://
				Change_List_Sound(VTx_WifiConnecting);
				Voice.tim=30;
				break;
			case VTx_WifiOk://
				Change_List_Sound(VTx_WifiOk);
				Voice.tim=30;
				break;
			case VTx_CHing://
				Change_List_Sound(VTx_CHing);
				Voice.tim=30;
				break;
			case VTx_Choff://
				Change_List_Sound(VTx_Choff);
				Voice.tim=30;
				break;
			case VTx_RUNm1:// 
				Change_List_Sound(VTx_RUNm1);
				Voice.tim=30;
				break;
			case VTx_RUNm2:// 
				Change_List_Sound(VTx_RUNm2);
				Voice.tim=30;
				break;
			case VTx_RUNCL://
				Change_List_Sound(VTx_RUNCL);
				Voice.tim=30;
				break;
			case VTx_RunclOver://
				Change_List_Sound(VTx_RunclOver);
				if(sysvar.CH_state == CH_ERR)sysvar.CH_state=0;

				Voice.tim=60;
				break;
			case VTx_RUNover://
				Change_List_Sound(VTx_RUNover);
				Voice.tim=30;
				break;
			case VTx_RUNOFF://
				Change_List_Sound(VTx_RUNOFF);
				Voice.tim=30;
				break;
			case VTx_Chlowing://
				Change_List_Sound(VTx_Chlowing);
				if(sysvar.CH_state == CH_ERR)sysvar.CH_state=0;
				Voice.tim=60;
				break;
			case VTx_CHErr://
				Change_List_Sound(VTx_CHErr);
				Voice.tim=60;
				break;
			case VTx_ConnectDragLala://
				Change_List_Sound(VTx_ConnectDragLala);
				Voice.tim=60;
				break;
			case VTx_CisternOk://
				Change_List_Sound(VTx_CisternOk);
				Voice.tim=60;
				break;
			case VTx_CisternNo://
				Change_List_Sound(VTx_CisternNo);
				Voice.tim=60;
				break;
			case VTx_CisternTake://
				Change_List_Sound(VTx_CisternTake);
				Voice.tim=60;
				break;
			case VTx_sewageErr://
				Change_List_Sound(VTx_sewageErr);
				Voice.tim=60;
				break;
			case VTx_ClearErr://
				Change_List_Sound(VTx_ClearErr);
				Voice.tim=60;
				break;
			case VTx_PumpErr://
				Change_List_Sound(VTx_PumpErr);
				Voice.tim=60;
				break;
			case VTx_RollerErr://
				Change_List_Sound(VTx_RollerErr);
				Voice.tim=60;
				break;
			case VTx_ConnectAspiration://
				Change_List_Sound(VTx_ConnectAspiration);
				Voice.tim=60;
				break;
			case VTx_DeviceErr://
				Change_List_Sound(VTx_DeviceErr);
				Voice.tim=60;
				break;
			case VTx_ConnectFeather://
				Change_List_Sound(VTx_ConnectFeather);
				Voice.tim=60;
				break;
			case VTx_ConnectSpray://
				Change_List_Sound(VTx_ConnectSpray);
				Voice.tim=60;
				break;
			case VTx_RUN3://
				Change_List_Sound(VTx_RUN3);
				Voice.tim=60;
				break;




			case VTx_prompt://
				Change_List_Sound(VTx_prompt);
				Voice.tim=60;
				break;



			case VTx_set://
				Voice.vfang &= ~VOICE_InitFang;
				Change_List_Sound(0xEF);
				Voice.tim=0;
				break;
			case VTx_STOP://
				Change_List_Sound(0xFE);//
				break;
			default:
				Voice.tim=10;//1S
				break;
		}
	}
}




void Change_List_Sound(u8 list_id){
	if(!(RGBvar.Show.S_WiFi & SH_VOICE)){
		Voice.vfang |= VOICE_InitFang;
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//
		Voice.vfang |= VOICE_Waiting;//
		Voice.SendSteps=-6;
		Voice.SendDATA=list_id/*+36*/;
	}
}




void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void){
	if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET ){
		if(++Voice.SendSteps>7){
			VOIC_PWMvar(1000);
			if(Voice.SendSteps>17){
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
				Voice.vfang &= ~VOICE_Waiting;//
			}
		}
		else if(Voice.SendSteps<0)
			VOIC_PWMvar(0);
		else if(Voice.SendDATA & SET8_BIT(Voice.SendSteps))
			VOIC_PWMvar(750);
		else
			VOIC_PWMvar(250);
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
}




















