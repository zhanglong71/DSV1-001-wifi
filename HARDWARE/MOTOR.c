#include "main.h"



void TIM1_PWMOut_Init(const u16 arr,const u16 psc,const u16 ccp){
	GPIO_InitTypeDef GPIO_InitStructure={GPIO_Pin_13,GPIO_Speed_50MHz,GPIO_Mode_AF_PP};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={psc,TIM_CounterMode_Up,arr,TIM_CKD_DIV1,0};
	TIM_OCInitTypeDef TIM_OCInitStructure={0};


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_TIM1, ENABLE );

	GPIO_Init( GPIOB, &GPIO_InitStructure );
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//
	TIM_OCInitStructure.TIM_Pulse = ccp;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//
	TIM_OC1Init( TIM1, &TIM_OCInitStructure );

	TIM_CtrlPWMOutputs(TIM1, ENABLE );
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable );//
	TIM_ARRPreloadConfig(TIM1, ENABLE );
	TIM_Cmd( TIM1, ENABLE );
}




void LOAD_Idle_Handler(void){
}




void Run_DragLala(){
	if(ADC_.ADCread.DMA_Vout > (ADC_.PWMcompare)){//
		if(MOTOSPWM_REG>2){
			MOTOSPWM_REG-=1;
		}else MOTOSPWM_REG=0;
	}
	else if(MOTOSPWM_REG<1000){
		MOTOSPWM_REG+=1;
	}
	if(sysvar.IERR_tim>2000){
		sysvar.IERR_tim=0;
		sysvar.sysfang |= (MOTO_ERR_1);
		MOTOSPWM_OFF;
		IDs_Set_Data(0,0,0,sysvar.CH_state?ID_Fang_OZ:0);// 
	}
	else {
		if(ADC_.IoutAvg > SENSVAR_A(7))//
			sysvar.IERR_tim+=500;
		else if(ADC_.IoutAvg>SENSVAR_A(5))	//
			sysvar.IERR_tim+=10;
		else if(ADC_.IoutAvg>SENSVAR_A(4))//
			sysvar.IERR_tim+=1;
		else if(sysvar.IERR_tim>2)sysvar.IERR_tim-=2;
	}
	sysvar.PWM_Duty=MOTOSPWM_REG;
}


void Run_Spray(){
	if(ADC_.ADCread.DMA_Vout > (ADC_.PWMcompare)){
		if(MOTOSPWM_REG>2){
			MOTOSPWM_REG-=1;
		}else MOTOSPWM_REG=0;
	}
	else if(MOTOSPWM_REG<1000){
		MOTOSPWM_REG+=1;
	}
	if(sysvar.IERR_tim>2000){
		sysvar.IERR_tim=0;
		sysvar.sysfang |= (MOTO_ERR_1);
		MOTOSPWM_OFF;
	}
	else {
		if(ADC_.IoutAvg > SENSVAR_A(3.5))//
			sysvar.IERR_tim+=500;
		else if(ADC_.IoutAvg>SENSVAR_A(2.6))	//
			sysvar.IERR_tim+=10;
		else if(ADC_.IoutAvg>SENSVAR_A(2.2))//
			sysvar.IERR_tim+=1;
		else if(sysvar.IERR_tim>2)sysvar.IERR_tim-=2;
	}

}


void Run_Aspiration_FOC(){
	MOTOSPWM_REG=1600;
	if(sysvar.IERR_tim>2000){
		sysvar.IERR_tim=0;
		sysvar.sysfang |= (MOTO_ERR_1);
		MOTOSPWM_OFF;
		IDs_Set_Data(0,0,0,0);
	}
	else {
		if(ADC_.IoutAvg > SENSVAR_A(16))//
			sysvar.IERR_tim+=500;
		else if(ADC_.IoutAvg>SENSVAR_A(14))	//
			sysvar.IERR_tim+=10;
		else if(ADC_.IoutAvg>SENSVAR_A(12))//
			sysvar.IERR_tim+=1;
		else if(sysvar.IERR_tim>2)sysvar.IERR_tim-=2;
	}
	sysvar.PWM_Duty=MOTOSPWM_REG;
}

/* */
void Run_Aspiration_Brush(){

}

/* */
void Run_Feather(){
	if(ADC_.ADCread.DMA_Vout > (ADC_.PWMcompare)){//大于预设//VPWM_(12500)2400
		if(MOTOSPWM_REG>2){
			MOTOSPWM_REG-=1;
		}else MOTOSPWM_REG=0;
	}
	else if(MOTOSPWM_REG<1000){
		MOTOSPWM_REG+=1;
	}
	if(sysvar.IERR_tim>2000){
		sysvar.IERR_tim=0;
		sysvar.sysfang |= (MOTO_ERR_1);
		MOTOSPWM_OFF;
	}
	else {
		if(ADC_.IoutAvg > SENSVAR_A(3))//
			sysvar.IERR_tim+=500;
		else if(ADC_.IoutAvg>SENSVAR_A(2.5))	//
			sysvar.IERR_tim+=10;
		else if(ADC_.IoutAvg>SENSVAR_A(1.8))//
			sysvar.IERR_tim+=1;
		else if(sysvar.IERR_tim>2)sysvar.IERR_tim-=2;
	}
}
/* */
void Run_WipeWin(){
	if(ADC_.ADCread.DMA_Vout > (ADC_.PWMcompare)){
		if(MOTOSPWM_REG>2){
			MOTOSPWM_REG-=1;
		}else MOTOSPWM_REG=0;
	}
	else if(MOTOSPWM_REG<1000){
		MOTOSPWM_REG+=1;
	}
	if(sysvar.IERR_tim>2000){
		sysvar.IERR_tim=0;
		sysvar.sysfang |= (MOTO_ERR_1);
		MOTOSPWM_OFF;
	}
	else {
		if(ADC_.IoutAvg > SENSVAR_A(7))//
			sysvar.IERR_tim+=500;
		else if(ADC_.IoutAvg>SENSVAR_A(5))	//
			sysvar.IERR_tim+=10;
		else if(ADC_.IoutAvg>SENSVAR_A(4))//
			sysvar.IERR_tim+=1;
		else if(sysvar.IERR_tim>2)sysvar.IERR_tim-=2;
	}
	sysvar.PWM_Duty=MOTOSPWM_REG;
}


void pump_run(){
	if(!(sysvar.sysfang & PUMP_Start)){//
		if(sysvar.t5>=10*5){
			sysvar.sysfang |= PUMP_Start;
			EN_WATER_OFF();
		}else
		EN_WATER_ON();
	}
	else if(EN_WATER_IN() && sysvar.t5>=sysvar.pump_ON_tim){
		EN_WATER_OFF();
		sysvar.t5 = 0;
	}
	else if(!EN_WATER_IN() && sysvar.t5>=sysvar.pump_OFF_tim){
		EN_WATER_ON();
		sysvar.t5 = 0;
	}
	sysvar.t5++;
}


void moto_run(){
	if((sysvar.sysfang & OFF_ON) && !(sysvar.batfang & (BACH_low)) && !(sysvar.sysfang & (MOTO_ERR_1))){
		sysvar.sysfang |= (moto_stop);
		sysvar.EN_motor_=1;
		switch (IDs_.Equipment){

			case f_DragLala:{
				if(IN_WATER_IN()){
					sysvar.sysfang &= ~(OFF_ON);
					sysvar.sysfang |= MOTO_ERR_2;
				}
				if(sysvar.Modes == MODE_CLEANING){
					if(++sysvar.t3>200-1){
						sysvar.sysfang &= ~(OFF_ON);
						Change_List_Sound(VTx_RunclOver);
					}
				}
			}
				break;

			case f_Spray:{
				if(IN_WATER_IN()){//
					sysvar.sysfang &= ~(OFF_ON);
					sysvar.sysfang |= MOTO_ERR_2;
				}
			}
				break;

			case f_Aspiration_FOC:
				EN_WATER_OFF();
				break;
/*********************** * ***************************************/
			case f_Aspiration_Brush:
				break;
/*********************** * ***************************************/
			case f_Feather:
				break;
/*********************** * ***************************************/
			case f_WipeWin:
				pump_run();
				break;
				default:break;
		}


	}
	else if(sysvar.sysfang & (moto_stop)){
		if(sysvar.EN_motor_){
		switch (IDs_.Equipment){
			case f_Empty://无设备
				EN_WATER_OFF();
				break;

			case f_DragLala:
				if(!(sysvar.sysfang & MOTO_ERR_1)){
			    sysvar.batfang &= ~BATCH_SPOT;
				}
				IDs_Set_Data(ID_UI_off,0,0,sysvar.CH_state?ID_Fang_OZ:0);// 
				sysvar.chingtim3=0;
				if(Voice.vfang & VOICE_runOff_f)
					VOIC_Control(VTx_RUNover,0);
				Voice.vfang &= ~VOICE_runOff_f;
				sysvar.EN_motor_=0;
				break;
/************************ * **************************************/
			case f_Spray:
				VOIC_Control(VTx_RUNOFF,0);
				IDs_Set_Data(0,0,0,0);// 
				EN_WATER_OFF();
				sysvar.EN_motor_=0;
				break;
/********************** * ***************************************/
			case f_Aspiration_FOC:
				VOIC_Control(VTx_RUNOFF,0);
				IDs_Set_Data(0,0,0,0);
				EN_WATER_OFF();
				sysvar.EN_motor_=0;
				break;
/*********************** * ***************************************/
			case f_Aspiration_Brush:
				VOIC_Control(VTx_RUNOFF,0);
				IDs_Set_Data(0,0,0,0);
				EN_WATER_OFF();
				sysvar.EN_motor_=0;

				break;
/*********************** * ***************************************/
			case f_Feather:
				VOIC_Control(VTx_RUNOFF,0);
				IDs_Set_Data(0,0,0,0);
				EN_WATER_OFF();
				sysvar.EN_motor_=0;
				break;
/*********************** * ***************************************/
			case f_WipeWin:
				VOIC_Control(VTx_RUNOFF,0);
				IDs_Set_Data(0,0,0,0);
				EN_WATER_OFF();
				sysvar.EN_motor_=0;
				break;
			default:
				EN_WATER_OFF();
				sysvar.EN_motor_=0;
				break;
		}}
		MOTOSPWM_OFF;
		sysvar.t2=0;
		sysvar.t3=0;
		sysvar.t5=0;
		sysvar.stop_tim=0;
		sysvar.t6=1000;
		sysvar.Modes=0;
		if(!(sysvar.sysfang & moto_stop2)){
			sysvar.t7=0;
			sysvar.sysfang |= (moto_stop2);
		}else if(++sysvar.t7>4){
			sysvar.t7=0;

			sysvar.sysfang &= ~(OFF_ON|moto_stop|moto_stop2);
			sysvar.LoadHandler=LOAD_Idle_Handler;//
		}
	}
	else {
		sysvar.sysfang &= ~(OFF_ON);
	}
}








	
