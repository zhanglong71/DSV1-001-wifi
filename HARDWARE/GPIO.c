#include "main.h"


void ExitSleepInit(){
  EXTI->INTENR &= ~(IDs_INT_EXT_Line|KEY1_EXT_Line);//关闭中断，节省mpu资源


  GPIOA->CFGHR |= (GPIO_CNF_MODE_INPUPD << GPIO_CRH_CNF_MODE_10_Pos);//带上下拉输入
  GPIOB->CFGLR &= ~(0x04 << GPIO_CRL_CNF_MODE_3_Pos);//复用推挽

	switch (IDs_.Equipment){
		case f_DragLala:
			EN_WATER_ON();
			break;
			default:break;
	}
}



void EntrySleepInit(){




  GPIOA->CFGHR &= ~(0x08 << GPIO_CRH_CNF_MODE_10_Pos);//
  GPIOB->CFGLR |= (0x04 << GPIO_CRL_CNF_MODE_3_Pos);//

  EXTI_ClearFlag(IDs_INT_EXT_Line|KEY1_EXT_Line);
  EXTI->INTENR |= IDs_INT_EXT_Line|KEY1_EXT_Line;


}



void IO_Init(void){
	EXTI_InitTypeDef EXTI_InitStructure={IN_CH_EXT_Line,EXTI_Mode_Interrupt,EXTI_Trigger_Rising_Falling,ENABLE};
  NVIC_InitTypeDef NVIC_InitStructure={IN_CH_IN_NVIC_Ch,3,3,ENABLE};
  GPIO_InitTypeDef GPIO_InitStructure={0,GPIO_Speed_2MHz,GPIO_Mode_IPU};


  RCC_APB2PeriphClockCmd(\
      RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
			RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);



	GPIO_InitStructure.GPIO_Pin  =  (KEY1_PIN); //
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  (BAT_INT_IN_PIN|IN_CH_IN_PIN);
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  =  (IN_WATER_PIN);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  =  (EN_CH_PIN|EN_WATERV_PIN|EN_5V_PIN);
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	EN_5V_ON();
	if(!IN_CH_IN())EN_CH_ON();



	GPIO_EXTILineConfig(IN_CH_IN_GPIOSRC, IN_CH_PINSRC);
	GPIO_EXTILineConfig(IDs_INT_GPIOSRC, IDs_INT_PINSRC);
	GPIO_EXTILineConfig(KEY1_GPIOSRC, KEY1_PINSRC);
	GPIO_EXTILineConfig(BAT_INT_IN_GPIOSRC, BAT_INT_PINSRC);

	EXTI_InitStructure.EXTI_Line = IN_CH_EXT_Line;// 
  EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = IDs_INT_EXT_Line;//ID
  EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//
	EXTI_InitStructure.EXTI_Line = KEY1_EXT_Line;//
  EXTI_Init(&EXTI_InitStructure);

//	EXTI_InitStructure.EXTI_Line = BAT_INT_EXT_Line;//
//  EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = IN_CH_IN_NVIC_Ch;// 
  NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = IDs_INT_NVIC_Ch;//
//  NVIC_Init(&NVIC_InitStructure)
//	NVIC_InitStructure.NVIC_IRQChannel = KEY1_NVIC_Ch;//
//  NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = BAT_INT_IN_NVIC_Ch;//
//  NVIC_Init(&NVIC_InitStructure);

}







void STOPdown(){
	EN_5V_OFF();
	EN_WATER_OFF();
	Voice.vfang &= ~VOICE_InitFang;
	RGBvar.state=0;
	RGBvar.RGBhandler=RGB_OFF_Handler;//
	IDs_.State_Hall 	= 0;
	IDs_.State_HallBRK= 0;
	sysvar.t6=1000;
	sysvar.stop_tim=0;
//	sysvar.sysfang &= ~KeyDormancy_quit;
	sysvar.sysfang |= SysDormancy;
	EntrySleepInit();
  ADC_DeInit(ADC1);
	RCC_LSICmd(DISABLE);
//  RCC->CSR  &= ~RCC_CSR_LSION;
//	RCC->CFGR &= ~RCC_CSR_LSION;

//	RCC_HSICmd(DISABLE);
	
	PWR_PVDCmd(DISABLE);
					//PWREN=1

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);


  PWR->CTLR = (SET32_BIT02|SET32_BIT20|SET32_BIT16|PWR_Regulator_LowPower);
  NVIC->SCTLR |= SET32_BIT02;
  __WFI();
  NVIC->SCTLR &= ~SET32_BIT02;
//  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //
//	Write_Iwdg_RL();
  SystemInit();

//  RCC->CSR  |= RCC_CSR_LSION;//打开LSI
//	RCC_LSICmd(ENABLE);

	ADC1_ChannelInit();
	ExitSleepInit();
	EN_5V_ON();
//	Energy_Display();
}



u8 resetTIM;
/*******************************************************************************/
void Power_off_Mode (void){//
	if(sysvar.stop_tim<600-1){
		if(!(sysvar.sysfang & (OFF_ON))){
			if(!(sysvar.sysfang & (_stop_CH)) && IN_CH_IN())
				sysvar.stop_tim++;
		}
	}else {
		sysvar.sysfang &= ~PUMP_Start;
		STOPdown();
	}
}
















