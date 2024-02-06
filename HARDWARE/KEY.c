#include "main.h"


void mode_determine(){
	sysvar.t1=0;
	sysvar.t5=0;
	sysvar.t7=0;
	sysvar.stop_tim=0;
	sysvar.t6=85;
	sysvar.IERR_tim=0;
	switch (IDs_.Equipment){
		case f_Empty:
			UpdataRGBmoodo(Rgb_ERRNoEquipment);
			VOIC_Control(VTx_ConnectNo,0);
			break;
		case f_DragLala:
			if(sysvar.Modes==MODE_2){			/*模式2	*/
				ADC_.PWMcompare=VPWM_(13000);
				IDs_Set_Data(ID_UI_M1,BASE_10MS(0.4),BASE_10MS(7.5),ID_Fang_Electrolysis|ID_Fang_Pump|ID_Fang_Air|ID_Fang_LED|ID_Fang_OZ);
				VOIC_Control(VTx_RUNm2,0);
				if(IdSensor_fang(IdSensor_CLEAR))VOIC_Control(VTx_ClearErr,1);
			}
			else if(sysvar.Modes==MODE_RINSE){			/*冲洗模式	*/
				ADC_.PWMcompare=VPWM_(12000);
				IDs_Set_Data(ID_UI_M4,BASE_10MS(0),BASE_10MS(10),ID_Fang_Electrolysis|ID_Fang_Pump|ID_Fang_Air|ID_Fang_LED);
				VOIC_Control(VTx_CisternTake,0);
			}
			else if(sysvar.Modes==MODE_CLEANING){	/*自清洗*/
				if(IdSensor_fang(IdSensor_BOX)){
					VOIC_Control(VTx_CisternNo,0);
					return;
				}
				ADC_.PWMcompare=VPWM_(12000);
				IDs_Set_Data(ID_UI_M3,BASE_10MS(2),BASE_10MS(5),ID_Fang_Electrolysis|ID_Fang_Pump|ID_Fang_Air|ID_Fang_LED|ID_Fang_OZ);
				VOIC_Control(VTx_RUNCL,0);
			}
			else {										/*标准1*/
				sysvar.Modes=MODE_1;
				ADC_.PWMcompare=VPWM_(11500);
				IDs_Set_Data(ID_UI_M1,BASE_10MS(0.4),BASE_10MS(7.5),ID_Fang_Electrolysis|ID_Fang_Pump|ID_Fang_Air|ID_Fang_LED|ID_Fang_OZ);
				VOIC_Control(VTx_RUNm1,0);
				if(IdSensor_fang(IdSensor_CLEAR))VOIC_Control(VTx_ClearErr,1);
			}
			EN_WATER_ON();
			IDs_.fang &= ~ID_F_ERR_;
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_DragLala;
			RGBvar.state=0;
			break;
		case f_Spray:
			if(sysvar.Modes==MODE_2){
				ADC_.PWMcompare=VPWM_(13000);
				IDs_Set_Data(0,BASE_10MS(0.5),BASE_10MS(18),IdSpray_2);
				VOIC_Control(VTx_RUN3,0);
			}
			else {
				sysvar.Modes=MODE_1;
				ADC_.PWMcompare=VPWM_(11000);
				IDs_Set_Data(0,BASE_10MS(0.5),BASE_10MS(18),IdSpray_1);
				VOIC_Control(VTx_RUNm1,0);
			}
			EN_WATER_ON();
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_Spray;
			break;
	case f_Aspiration_FOC:
			if(sysvar.Modes==MODE_2){
				IDs_Set_Data(0,0,0,101);
				VOIC_Control(VTx_RUNm2,0);
			}
			else {
				sysvar.Modes=MODE_1;
				IDs_Set_Data(0,0,0,50);
				VOIC_Control(VTx_RUNm1,0);
			}
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_Aspiration_FOC;
			break;
		case f_Aspiration_Brush:
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_Aspiration_Brush;
			break;
		case f_Feather:
			if(sysvar.Modes==MODE_2){
				ADC_.PWMcompare=VPWM_(12000);
				IDs_Set_Data(0,0,0,IdSpray_2);
				VOIC_Control(VTx_RUNm2,0);
			}
			else {
				sysvar.Modes=MODE_1;
				ADC_.PWMcompare=VPWM_(10000);
				IDs_Set_Data(0,0,0,IdSpray_1);
				VOIC_Control(VTx_RUNm1,0);
			}
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_Feather;
			
			break;
		case f_WipeWin:
			if(sysvar.Modes==MODE_2){
				sysvar.pump_ON_tim=BASE_10MS(3);
				sysvar.pump_OFF_tim=BASE_10MS(1);
				ADC_.PWMcompare=VPWM_(13000);
				IDs_Set_Data(0,0,0,IdSpray_2);
				VOIC_Control(VTx_RUNm2,0);
			}
			else {
				sysvar.Modes=MODE_1;
				sysvar.pump_ON_tim=BASE_10MS(1);
				sysvar.pump_OFF_tim=BASE_10MS(3);
				ADC_.PWMcompare=VPWM_(11000);
				IDs_Set_Data(0,0,0,IdSpray_1);
				VOIC_Control(VTx_RUNm1,0);
			}
			sysvar.sysfang |=(OFF_ON);
			sysvar.LoadHandler=Run_WipeWin;
			break;
		default:break;
	}
}



u8 keybuf = 0xff,keybufCH = 0xff;
bool KeySta_K1=1,backdn_K1=1,KeySta_CH=1,backdn_CH=1;
void KEY_scanning(){
	static u16 changansta,ConnectingPress;
	keybufCH  = ((keybufCH<<1) | IN_CH_IN());
	if(keybufCH==0)KeySta_CH=0;
	else if(keybufCH==0xff)KeySta_CH=1;

	if(KeySta_CH != backdn_CH){
		if(!KeySta_CH){
			if(!IN_CH_IN()){
				sysvar.t5 = 0;
				if(sysvar.sysfang & OFF_ON){
					if(!IdSensor_fang(IdSensor_BOX)){
						sysvar.Modes=MODE_CLEANING;
						mode_determine();
					}else sysvar.sysfang &= ~OFF_ON;
						Voice.vfang &= ~VOICE_ching_f;
						sysvar.batfang |= BATCH_SPOT;

				}
			}else sysvar.t6=1000;
			if(Voice.vfang&VOICE_ching_f){
				VOIC_Control(VTx_CHing,0);
				sysvar.batfang |= BATCH_SPOT;
			}
			sysvar.chingtim2=0;
			sysvar.chingtim1=0;
			sysvar.sysfang |= (_stop_CH);
			sysvar.batfang &= ~(BATpowers_ERR|BACH_low);
		}
		backdn_CH=KeySta_CH;
	}

/******************K1*********************************/
	keybuf  = ((keybuf<<1) | KEY1);
	if (keybuf == 0xff){
		KeySta_K1 = 1;
		if((sysvar.keyfang & KeyInstallState)&&(sysvar.keyfang & KeyLongFang)){//设置模式且已松开过
			if(!ConnectingPress){
				if(sysvar.keyfang & KeyConnecting){//单击
					sysvar.keyfang &= ~KeyConnecting;
					if(RGBvar.Show.S_WiFi & SH_VOICE){
						RGBvar.Show.S_WiFi &= ~SH_VOICE;
						Change_List_Sound(VTx_VoiceOpen);
						Voice.tim=20;
					}
					else {
						Change_List_Sound(VTx_VoiceClose);
						RGBvar.Show.S_WiFi |= SH_VOICE;
						Voice.tim=20;
					}
					sysvar.t6=1000;
					RGBvar.state=0;
				}
			}else ConnectingPress--;
		}
	}
	else if(!keybuf){         	//操作
		KeySta_K1 = 0;
		{
			if(IDs_.fang & ID_F_EquipmentReady){
				if(changansta<50)
					changansta++;
				else if(!(sysvar.keyfang & KeyLongFang)/* && (sysvar.keyfang&KeyDormancy_quit)*/){
					sysvar.keyfang |= KeyLongFang;
					if(sysvar.sysfang & (MOTO_ERR_1))return;
					if(sysvar.sysfang & OFF_ON){
						if(sysvar.CH_state/* || sysvar.batfang&BACHready*/){
							sysvar.Modes = MODE_CLEANING;
						}
						else {
							if(IDs_.Equipment==f_Feather){
								sysvar.Modes = (sysvar.Modes==MODE_1)?((sysvar.keyfang & KeyLongFang2)?MODE_3:MODE_2):((sysvar.Modes==MODE_2)?MODE_3:MODE_1);
							}else
								sysvar.Modes = (sysvar.Modes==MODE_1)?MODE_2:MODE_1;
						}
						mode_determine();
						if((sysvar.batfang & BACH_lowin) && !sysvar.CH_state){//切换模式
							VOIC_Control(VTx_Chlowing,1);
						}
					}
				}
			}else{
				if(changansta<150)
					changansta++;
				else {
					changansta=0;
					if(sysvar.keyfang & KeyInstallState){
						sysvar.keyfang &= ~(KeyInstallState);
						RGBvar.Show.S_WiFi &= 0xfc;
					}
					else {
						sysvar.keyfang |= (KeyInstallState);
						VOIC_Control(VTx_Install,0);
					}
					sysvar.t6=1000;
					RGBvar.state=0;
				}
			}
		}
	}
//更新
	if(backdn_K1 != KeySta_K1){
		if(!(sysvar.keyfang & KeyLongFang)){//长按
			if((sysvar.keyfang & KeyPressReleaseFang)?(KeySta_K1):(!KeySta_K1)){//有效
				if(sysvar.sysfang&OFF_ON){
					sysvar.keyfang &= ~(KeyPressReleaseFang);//关机后
					sysvar.sysfang &= ~(OFF_ON);
					sysvar.batfang &= ~(BACH_low|BACH_lowin);
					Voice.vfang |= VOICE_runOff_f;
					sysvar.t1=100;
				}
				else if(!(sysvar.keyfang & KeyInstallState)){//设置模式
					if((sysvar.batfang & BACH_low) || (ADC_.VbatAvg<BATcompare_CHing[1])){
						sysvar.batfang |= BATpowers_ERR;
//						if(sysvar.BAT_soc>100)VOIC_Control(VTx_Chlowing,0);
//						else
						VOIC_Control(VTx_Chlowing,0);// 不足，
						RGBvar.FlickerNum=10;// 次数
						RGBvar.state=0;
						sysvar.Modes=0;
						sysvar.t6=1000;
					}else if(sysvar.sysfang & (MOTO_ERR_1)){// 报警
						sysvar.sysfang &= ~(MOTO_ERR_1);
						if(sysvar.LoadHandler==Run_DragLala)//设备
							IDs_Set_Data(0,0,0,sysvar.CH_state?ID_Fang_OZ:0);//
					}
					else{//上电
						if(IDs_.Equipment==f_Spray){//喷雾器
							sysvar.Modes=MODE_3;
							mode_determine();
							if(sysvar.batfang & BACH_lowin)//
								VOIC_Control(VTx_Chlowing,1);
						}
						else if(IdSensor_fang(IdSensor_PUMP|IdSensor_PUMPERR)){
							IDs_.fang |= ID_F_ERR_;
							sysvar.t6=1000;
						}
						else{
							if(sysvar.CH_state){						// 中
								sysvar.Modes=MODE_CLEANING;
							}
							else{
								sysvar.Modes=MODE_1;
								sysvar.keyfang |= KeyLongFang2;// 启动标志
							}
							mode_determine();
							if(sysvar.batfang & BACH_lowin){//切换模式
								VOIC_Control(VTx_Chlowing,1);
							}
						}
					}
				}
			}
		}
		if(KeySta_K1){
			if(sysvar.keyfang & KeyInstallState){
				if(sysvar.keyfang & KeyLongFang){
					if(sysvar.keyfang & KeyConnecting){//第二次
						if((RGBvar.Show.S_WiFi&0x03)==1)//
							RGBvar.Show.S_WiFi=(RGBvar.Show.S_WiFi&0xfc)|0x02;
						else if((RGBvar.Show.S_WiFi&0x03)==2)
							RGBvar.Show.S_WiFi=(RGBvar.Show.S_WiFi&0xfc)|0;
						else RGBvar.Show.S_WiFi=(RGBvar.Show.S_WiFi&0xfc)|0x01;
						sysvar.t6=1000;
						RGBvar.state=0;
						sysvar.keyfang &= ~KeyConnecting;
					}
					else{
						sysvar.keyfang |= KeyConnecting;
						ConnectingPress=50;//0.5S
					}
				}
				else sysvar.keyfang |= KeyLongFang;
			}else {
			if(sysvar.sysfang&OFF_ON){
				sysvar.keyfang |= KeyPressReleaseFang;
//				else sysvar.keyfang &= ~KeyPressReleaseFang;
			}
			sysvar.keyfang &= ~(KeyLongFang|KeyLongFang2);
		}
//		sysvar.keyfang |= KeyDormancy_quit;
		changansta=0;
		}
//	sysvar.t6=95;
	backdn_K1 = KeySta_K1; 
	}
}























