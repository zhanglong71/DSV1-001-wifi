#define _ADC_C_

#include "main.h"
#include <string.h>
#include <stdlib.h>

adc_TypeDef  ADC_={0};



void Tim3_init(u16 arr, u16 psc){
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );

		TIM_TimeBaseInitStructure.TIM_Period = arr;
		TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

		TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);

		TIM_Cmd(TIM3, ENABLE );
}




void ADC1_ChannelInit(void){

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);


  GPIOA->CFGLR &= ~((GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_0_Pos)|
//  									(GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_1_Pos)|
										(GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_3_Pos));

  GPIOB->CFGLR &= ~((GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_0_Pos)|
  									(GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_1_Pos));


  ADC1->CTLR1 = ADC_SCAN;
  ADC1->CTLR2 = ADC_EXTTRIG|ADC_TSVREFE|ADC_EXTSEL_2|ADC_DMA|ADC_ADON;
  ADC1->RSQR1 = (6-1)<<20;

  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5, ADC_SampleTime_239Cycles5);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 6, ADC_SampleTime_239Cycles5);

  ADC_BufferCmd(ADC1, ENABLE); //enable buffer
}





 void DMA_Config(){
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_.ADCread;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 6;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnyChanInit(void){
	ADC1_ChannelInit();
	DMA_Config();
	Tim3_init(1000,144);
}

/*比较数组中最大和最小值差并排序*/
u16 Compare_differences(u16* a,const u8 n){
	u8 i,j;
	u16 temp;
	for(i=0;i<n-1;i++) {
		for(j=i+1;j<n;j++){	
			if(a[i] > a[j]){
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
			}
		}
	}
	return(a[n-1]-a[0]);
}

/*/比较数组中最大和最小值差*/
u16 Compare_Size(const u16* const a,const u8 n){
	u8 i;
	u16 min=a[0],max=a[0];
	for(i=1;i<n;i++){
		if(a[i] > max)
			max=a[i];
		if(a[i] < min)
			min=a[i];
	}
	return max-min;
}

/*/比较n个长度的数组最大和最小值,若小于var则返回平均值,n<17*/
u16 Compare_(const u16* const a,const u8 n,const u16 var){
	u8 i;
	u16 min=a[0],max=a[0];
	u16 vartemp=0;
	for(i=1;i<n;i++){
		if(a[i] > max)
			max=a[i];
		if(a[i] < min)
			min=a[i];
	}
	if(max-min<=var){
		for(i=0;i<n;i++){
			vartemp+=a[i];
		}
		return (vartemp/n);
	}else 
	return 0;
}

void Gat_ADC_Avg(){
	static u32 IoutSum = 0,VinSum = 0,VbatSum=0;
	//对采样值滑动求平均
	
	VbatSum = VbatSum + (ADC_.ADCread.DMA_Vbat<<3) -(VbatSum>>3);
	ADC_.VbatAvg = VbatSum>>6;

	VinSum = VinSum + (ADC_.ADCread.DMA_Vin<<3) -(VinSum>>3);
	ADC_.VinAvg = VinSum>>6;

	IoutSum = IoutSum + (ADC_.ADCread.DMA_sens<<3) -(IoutSum>>3);
	ADC_.IoutAvg = IoutSum>>6;
}


/*读取校准电压*/
void Get_ADC_RefValue(void){
}


u8 ADC_ElectricityCompare_CHing(void){
	u8 i=0;
	while(ADC_.ADCread.DMA_Vbat>BATcompare_CHing[i]){
		if(++i>99)
			break;
	}
	return i;
}

u8 ADC_ElectricityCompare_Load(void){
	u8 i=0;
	while(ADC_.ADCread.DMA_Vbat<BATcompare_Load[i]){
		if(++i>99)
			break;
	}
	return 100-i;
}




void perform(){		//0.1S BATDischarge
	static u8 temp_soc=100,tim;
#ifdef  CW2217
	u8 reg_val;
#endif
	if(++tim>10-1){
		tim=0;
		if(sysvar.CH_state==2 || sysvar.CH_state==3 || sysvar.CH_state==4){
			ADC_.SOC=ADC_ElectricityCompare_CHing()+100;
			if(ADC_.SOC==100)ADC_.SOC+=1;
			if(temp_soc>ADC_.SOC)ADC_.SOC=temp_soc;
		}
		else{
			ADC_.SOC=ADC_ElectricityCompare_Load();
			if(temp_soc<ADC_.SOC)ADC_.SOC=temp_soc;
		}

#ifdef  CW2217
//cw_update_data();
		if(sysvar.batfang & BAT_INTfang){
			sysvar.batfang &= ~BAT_INTfang;
			if(cw_read(REG_SOC_INT,&reg_val)==0){		//
				if(reg_val&0x01)//TMN_INT
				;
				else if(reg_val&0x02)//TMx_INT
				;
				else if(reg_val&0x04){//SOC_INT
					if(cw_read(REG_SOC_INT, &cw_bat.SOC)==0)//
						cw_bat.ErrNum=0;
					else cw_bat.ErrNum++;
				}
				reg_val = CW_CONFIG_INT;
				cw_write(REG_GPIO_CONFIG,&reg_val);//清除中断
			}else cw_bat.ErrNum++;
		}else{
			cw221x_get_vol(&cw_bat.voltage);	//获得电压
			if(cw_read(REG_SOC_INT, &cw_bat.SOC)==0){//读soc寄存器
				cw_bat.ErrNum=0;
				if(cw_bat.SOC==0){
					if(!cw_read(REG_MODE_CONFIG, &reg_val)){
						if (reg_val == 0xf0){//检查是否睡眠
							cw221x_active();
						}
					}
				}
			}
			else {
				cw_bat.ErrNum++;
				if(cw_bat.ErrF !=0 ){
					cw_bat.ErrF=cw221x_bat_init();
				}
			}
		}
		if(cw_bat.ErrNum==0)
			sysvar.BAT_soc=cw_bat.SOC;
		else if(cw_bat.ErrNum>10)
#endif
			sysvar.BAT_soc=ADC_.SOC;

		if(temp_soc!=sysvar.BAT_soc){
			temp_soc=sysvar.BAT_soc;
			sysvar.sysfang |= SysDormancy;
		}
		else if(sysvar.sysfang & SysDormancy){
			RGBvar.Show.S_SOC=sysvar.BAT_soc;
			if(RGBvar.RGBhandler!=RGB_OFF_Handler)Disp_Send_Group(); 
			RGBvar.ShowBRK=RGBvar.Show;
		}




		if(sysvar.BAT_soc<1){
			sysvar.batfang |= BACH_low;// 不足   
			if(sysvar.sysfang & OFF_ON){
				sysvar.sysfang &= ~(OFF_ON);
				sysvar.batfang |= (BATpowers_ERR);
				VOIC_Control(VTx_Chlowing,1);
				sysvar.t6=1000;
			}
		}
		else if(sysvar.BAT_soc<3){
			sysvar.batfang &= ~(BACH_low);
			if(!(sysvar.batfang & BACH_lowin2)){//
				VOIC_Control(VTx_Chlowing,1);
				sysvar.batfang |= (BACH_lowin2);
				sysvar.t6=1000;
			}
		}
		else if(sysvar.BAT_soc<5){
			sysvar.batfang &= ~(BACH_low);
			if(!(sysvar.batfang & BACH_lowin)){// 不足
				VOIC_Control(VTx_Chlowing,1);
				sysvar.batfang |= (BACH_lowin);
				sysvar.t6=1000;
			}
		}
		else{
			sysvar.batfang &= ~(BACH_low|BACH_lowin|BACH_lowin2);
		}
	}
}




