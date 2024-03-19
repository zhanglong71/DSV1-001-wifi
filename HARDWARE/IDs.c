#include "main.h"
#include "stdio.h"
#include "string.h"

IDs_TypeDef IDs_;

//UART2
 void DMA_TX2_Config(){
	DMA_InitTypeDef DMA_InitStructure = {0};
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DATAR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&IDs_.Tdata.CMD;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel7, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
}

void ID_Init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure  = { 0 };
	USART_InitTypeDef USART_InitStructure = { 0 };
	NVIC_InitTypeDef  NVIC_InitStructure  = { 0 };

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_HalfDuplexCmd(USART2, ENABLE);//使能半双工模式

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);


	IDs_.IDs_CONTROL_Data=-1;
	IDs_.Tdata.CMD=IdTxCmdRead_ID;
	IDs_.Tdata.EndMark_1=0x0D;
	IDs_.Tdata.EndMark_2=0x0A;
	DMA_TX2_Config();

}

void ID_Send_Byte(const unsigned char dat) {
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART2->DATAR = (dat);
}

void ID_Send_Group(u8* buf, u8 len) {
	while(len--)
	ID_Send_Byte(*buf++);
}



void UART_Send_Str(char *s){
	u8 Len=strlen((const char*)s);
	while(Len--)
		ID_Send_Byte(*s++);		//
}


unsigned char Long_Str(long dat,unsigned char *str){
	signed char i=0;
	unsigned char len = 0;
	unsigned char buf[11];
	if (dat < 0){
		dat = -dat;
		*str++ = '-';
		len++;
	}
	do	{
		buf[i++] = dat % 10+0x30;
		dat /= 10;
	} while (dat > 0);
    len += i;
    while (i-- > 0){
        *str++ = buf[i] ;
    }
	*str = 0;
	return len;
}


void UART_Send_Num(unsigned long dat){
	unsigned char temp[11];
	Long_Str(dat,temp);
	UART_Send_Str(temp);
}

u8 len,data[256];
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void){
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		data[++len]=USART_ReceiveData(USART2);
		if(data[len]==0XA0 && data[len-1]==0XD0){
			switch (data[len-2]){//
				case 0x01://
					IDs_.Equipment = data[len-3];
					if(!(IDs_.fang & ID_F_EquipmentReady)){
//						sysvar.sysfang |= KeyDormancy_quit;//
						RGBvar.Show.S_WiFi &= 0xfc;
						switch (IDs_.Equipment){
							case f_Empty://无
								break;
							case f_DragLala:
								VOIC_Control(VTx_ConnectDragLala,0);
								UpdataRGBmoodo(Rgb_ERREquipment); // 
								IDs_.fang |= ID_F_Read;//
								EN_WATER_ON();
								break;
							case f_Spray:
								VOIC_Control(VTx_ConnectSpray,0);
								UpdataRGBmoodo(Rgb_ERREquipment); // 
								break;
							case f_Aspiration_FOC:
								VOIC_Control(VTx_ConnectAspiration,0);
								UpdataRGBmoodo(Rgb_ERREquipment); // 
								break;
							case f_Aspiration_Brush:
								break;
							case f_Feather:
								VOIC_Control(VTx_ConnectFeather,0);
								UpdataRGBmoodo(Rgb_ERREquipment); // 
								break;
							case f_WipeWin:
								break;
								default:
									IDs_.Equipment=0;
								break;
						}
					}
					break;
				case 0x02:{
					u8 sbuf[30];
					UART_Send_Str(" \r\n");
					sprintf((char*)sbuf,"%s%d%s \r\n","CW-ADC:",(u16)(cw_bat.voltage),"mV");
					UART_Send_Str(sbuf);
					sprintf((char*)sbuf,"%s%d%s \r\n","CW-SOC:",cw_bat.SOC,"%");
					UART_Send_Str(sbuf);
					sprintf((char*)sbuf,"%s%d%s \r\n","SYS-ADC:",(u16)(ADC_.VbatAvg/0.17),"mV");
					UART_Send_Str(sbuf);
					sprintf((char*)sbuf,"%s%d%s \r\n","SYS-SOC:",ADC_.SOC,"%");
					UART_Send_Str(sbuf);
					UART_Send_Str("Firmware:20230321_REV0.1 \r\n");
					UART_Send_Str("Host :CH32V203C8T6 \r\n");
					//UART_Send_Str(_IQMathVerStr());

				}
					break;
				case 0x03://
					if(IDs_.Effect==data[len-3])
						IDs_.fang &= ~ID_F_TX;
					break;
				case 0x04:{//
					IDs_.State_Charge = data[len-5];
					IDs_.State_Hall 	= data[len-4];
					IDs_.State_Other	= data[len-3];
					IDs_.fang |= (ID_F_Effect|ID_F_Control);//

					IDs_.fang &= ~ID_F_Read;

					if(!(IDs_.fang & ID_F_TJust)){
						sysvar.t6=0;
						IDs_.fang |= ID_F_RJust;
					}IDs_.fang &= ~ID_F_TJust;


					ID_TxRapid();
					}
					break;
				case 0x05:{

				}
				break;
				default:break;
			}
		}
	}
}


/*发送数据*/
void IDs_Sens_4b(const u8 len,const u8 CMD){
//	while(!(DMA1->INTFR&DMA1_FLAG_TC7));
//	DMA1->INTFCR = DMA1_FLAG_TC7;
	DMA1_Channel7->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
	IDs_.Tdata.CMD=CMD;
	IDs_.TdataTemp=IDs_.Tdata;
	if(len==3)DMA1_Channel7->MADDR =(u32)&IDs_.TdataTemp.CMD;
	else if(len==4)DMA1_Channel7->MADDR =(u32)&IDs_.TdataTemp.D1;
	else if(len==6)DMA1_Channel7->MADDR =(u32)&IDs_.TdataTemp.D3;
	else if(len==7)DMA1_Channel7->MADDR =(u32)&IDs_.TdataTemp.D4;
	DMA1_Channel7->CNTR = len;
	DMA1_Channel7->CFGR |= DMA_CFGR1_EN;
}


void IDs_Set_Data(const u8 D4,const u8 D3,const u8 D2,const u8 D1){
	IDs_.fang |= ID_F_TX;
	ID_TxRapid();
	IDs_.Effect = D1+D2+D3+D4;
	IDs_.Tdata.D4=D4;
	IDs_.Tdata.D3=D3;
	IDs_.Tdata.D2=D2;
	IDs_.Tdata.D1=D1;
}


u8 temp_ids[5];
void IDs_Judge() {
	if(!IDs_INT_IN()){
		if (temp_ids[0] < 10-1) {
			temp_ids[0]++;
		} else {
			if(IDs_.fang & ID_F_EquipmentReady){
				UpdataRGBmoodo(Rgb_ERRNoEquipment); //
				VOIC_Control(VTx_Disconnect,0);
				IDs_.State_Charge = 0;
				IDs_.State_Hall 	= 0;
				IDs_.State_HallBRK= 0;
				IDs_.State_Other	= 0;
			}
			EN_WATER_OFF();
			temp_ids[1] = 0;
			IDs_.State_Charge = 0;
			IDs_.Equipment = f_Empty;
			sysvar.sysfang &= ~(OFF_ON|MOTO_ERR_1);
		}
	}else{
		if(++temp_ids[1]>50){//0.5秒
			temp_ids[0]=0;
			temp_ids[1]=0;
			if(IDs_.Equipment == f_Empty){//
				IDs_Sens_4b(3,IdTxCmdRead_ID);//读ID
			}else if(IDs_.fang & ID_F_Effect){//
				DMA1_Channel7->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
				IDs_.fang &= ~ID_F_Effect;
				IDs_.TdataTemp.D1=IDs_.State_Charge+IDs_.State_Hall+IDs_.State_Other;
				IDs_.TdataTemp.CMD=IdTxCmdEffect;
				DMA1_Channel7->MADDR =(u32)&IDs_.TdataTemp.D1;
				DMA1_Channel7->CNTR = 4;
				DMA1_Channel7->CFGR |= DMA_CFGR1_EN;
			}
			else{
				if(IDs_.fang&ID_F_TX){//
					switch (IDs_.Equipment){
			/****************************拖拉啦*设备 处理**********************************/
						case f_DragLala:{
							IDs_Sens_4b(7,IdTxCmdControl_1);
							break;
						}
			/************************喷雾器*设备 处理**************************************/
						case f_Spray:
							IDs_Sens_4b(6,IdTxCmdControl_1);
							break;
			/********************** *设备 处理***************************************/
						case f_Aspiration_FOC:
							IDs_Sens_4b(4,IdTxCmdControl_1);
							break;
			/*********************** ****************************************/
						case f_Aspiration_Brush:
							IDs_Sens_4b(4,IdTxCmdControl_1);
							break;
			/*********************** *设备 处理***************************************/
						case f_Feather:
							IDs_Sens_4b(4,IdTxCmdControl_1);
							break;
			/*********************** *设备 处理***************************************/
						case f_WipeWin:
							IDs_Sens_4b(4,IdTxCmdControl_1);
							break;
						default:break;
					}
				}
				else if(IDs_.fang&ID_F_Read){//读 信息
					IDs_Sens_4b(3,IdTxCmdRead_State);
				}
				if(IDs_.fang&ID_F_Control){//
					IDs_.fang &= ~ID_F_Control;
					switch (IDs_.Equipment){
			/****************************拖拉啦*设备状态处理**********************************/
						case f_DragLala:{
							if(IDs_.State_HallBRK!=IDs_.State_Hall){
								if(IdSensor_fCmp(IdSensor_CLEAR)){//
									temp_ids[2]=0;
									if(IdSensor_fang(IdSensor_CLEAR)){//
										if((sysvar.sysfang & (OFF_ON)) && (sysvar.Modes!=MODE_RINSE))
											VOIC_Control(VTx_ClearErr,1);
										RGBvar.Show.S_Modo|=SH_Modo_ERR;
									}else	RGBvar.Show.S_Modo&=~SH_Modo_ERR;
									if(RGBvar.RGBhandler!=RGB_OFF_Handler){//
										sysvar.t6=1000;
										RGBvar.Show.S_SOC=sysvar.BAT_soc;
										Disp_Send_Group();
									}
								}
								if(IdSensor_fCmp(IdSensor_SEWAGE)){
									temp_ids[2]=0;
									if(IdSensor_fang(IdSensor_SEWAGE)){
										if((sysvar.sysfang & (OFF_ON)) && (sysvar.Modes!=MODE_RINSE))
											VOIC_Control(VTx_sewageErr,0);
										RGBvar.Show.S_Modo|=SH_Modo_ERR;
									}else	RGBvar.Show.S_Modo&=~SH_Modo_ERR;
									if(RGBvar.RGBhandler!=RGB_OFF_Handler){//
										sysvar.t6=1000;
										RGBvar.Show.S_SOC=sysvar.BAT_soc;
										Disp_Send_Group();//
									}
								}
								if(IdSensor_fang(IdSensor_PUMP|IdSensor_PUMPERR)){
									//
									if(sysvar.sysfang & (OFF_ON)){
										IDs_.fang |= ID_F_ERR_;
										sysvar.t6=1000;
                                        g_componentStatus.pump = CINDEX_PUMPCURRENTSMALL;
									}else{
										IDs_.State_Hall &= ~(IdSensor_PUMP|IdSensor_PUMPERR);//
										IDs_.State_HallBRK &= ~(IdSensor_PUMP|IdSensor_PUMPERR);//
//										IDs_.fang &= ~ID_F_ERR_;
										IDs_Set_Data(0,0,0,0);// 
									}
								}
								IDs_.State_HallBRK=IDs_.State_Hall;
							}
							break;
						}
			/************************ **************************************/
						case f_Spray:

							break;
			/********************** *设备状态处理***************************************/
						case f_Aspiration_FOC:
							break;
			/*********************** *设备状态处理***************************************/
						case f_Aspiration_Brush:
							break;
			/*********************** *设备状态处理***************************************/
						case f_Feather:
							break;
			/*********************** *设备状态处理***************************************/
						case f_WipeWin:
							break;
						default:break;
					}
				}else if(IdSensor_fang(IdSensor_CLEAR)){
					if((sysvar.sysfang & OFF_ON) && (sysvar.Modes!=MODE_RINSE)){
						if(++temp_ids[2]>30*2){
							temp_ids[2]=0;
							RGBvar.Show.S_SOC=sysvar.BAT_soc;
							RGBvar.Show.S_Modo|=SH_Modo_ERR;
							Disp_Send_Group();//
							VOIC_Control(VTx_ClearErr,1);
						}
					}else{
						IDs_.State_Hall &= ~(IdSensor_CLEAR);//
						IDs_.State_HallBRK &= ~(IdSensor_CLEAR);//
					}
				}
			}
		}
	}
}





















