#include "main.h"


#define CH_NONE					0
#define CH_DETECTION		1
#define CH_ING					2
#define CH_over					3
#define CH_STANDBY			4
#define CH_ERR					5




u8 CH_task2(void){//1s
	static u8 tim;
#if 0
	if(IDs_.State_Charge){
/*���϶�ȡ�豸�� ��Ϣ*/
// ״̬-1����У�2���� ��3  /ʹ�ܹرգ�4��Ч ��������δ��⵽
		if((sysvar.sysfang & (OFF_ON)) && sysvar.Modes!=MODE_CLEANING){
			sysvar.Modes=MODE_CLEANING;
			mode_determine();
		}
		if(!(IDs_.fang & ID_F_RJust)){//���յ�����Ϣ����һ���ٶ�ȡ
			IDs_.fang |= ID_F_Read|ID_F_TJust;//��ָ���ʹ�ܣ��շ��ͱ�־
		}else IDs_.fang &= ~ID_F_RJust;

/*�����*/
		if(IDs_.State_Charge==1){//
			sysvar.sysfang |= _stop_CH;
			EN_WATER_ON();
			tim=0;
			return CH_DETECTION;
		}
/* ��*/
		else if(IDs_.State_Charge==2){
			sysvar.sysfang |= _stop_CH;
			if(sysvar.BAT_soc>199){// 100%
				if(tim>180-1){
					if(ADC_.VbatAvg>(VBAT_gat(16.8))){// ��ѹ����16.8V���ر��ֱ���
						EN_WATER_OFF();
//						if(ADC_.VbatAvg>(VBAT_gat(17))){//�����ʱ��ѹ��Ȼ����17v����ر��豸�� 
//							if(sysvar.sysfang & (OFF_ON))
//								IDs_Set_Data(IDs_.Tdata.D4,IDs_.Tdata.D3,IDs_.Tdata.D2,(IDs_.Tdata.D1|ID_Fang_Charge));
//							else IDs_Set_Data(0,0,0,ID_Fang_Charge);
//						}
					}
					return CH_STANDBY;//����Ϩ��
				}else tim++;
				return CH_over;//��������
			}else{// <100%
				EN_WATER_ON();
				return CH_ING;
			}
		}
/*�豸 ʹ�ܱ��ر�*/
		else if(IDs_.State_Charge==3){
//			if(sysvar.BAT_soc<195){
//				if(sysvar.sysfang & (OFF_ON))
//					IDs_Set_Data(IDs_.Tdata.D4,IDs_.Tdata.D3,IDs_.Tdata.D2,(IDs_.Tdata.D1&(~ID_Fang_Charge)));
//				else IDs_Set_Data(0,0,0,0);
//			}
			return CH_STANDBY;
		}
/*��Ч ��*/
		else if(IDs_.State_Charge==4){//��Ч ��
			sysvar.sysfang &= ~(_stop_CH);//����˯��
			EN_WATER_OFF();
			return CH_NONE;//�ر�
		}
		return IDs_.State_Charge;
	}
	else
#endif
		if(!IN_CH_IN()){
		if(sysvar.chingtim1>30*60 || sysvar.chingtim2>3*60*60){//21600
			/*if(sysvar.sysfang & CH_LightsOut_OFF){
				sysvar.sysfang &= ~(_stop_CH);//����˯��
				return 4;//Ϩ��
			}*/
			if(!(sysvar.batfang &	BACHready)){
				sysvar.batfang |=	BACHready;//  
//				VOIC_Control(VTx_CHover,NoWaitingVoic);
			}
			if(tim>180){
				/*if(ADC_.VbatAvg>VBAT_gat(16.8))EN_CH_OFF();
				else if(ADC_.VbatAvg<VBAT_gat(16.7))*/
//				EN_CH_ON();
				sysvar.batfang &= ~(BATCHining);
				sysvar.sysfang &= ~(_stop_CH);//����˯��
				return CH_STANDBY;//�ر�
			}
			else{
				tim++;
				return CH_over;//  
			}
		}
		else{
			if(!EN_CH_IN()){//��û�п�ʼ 
				if(ADC_.VinAvg>VBAT_gat(Vin_CHdown) && ADC_.VinAvg<VBAT_gat(Vin_CHup)){//��������ѹ
					sysvar.chingtim2=ADC_.SOC*((3*60*60)/100);//�жϵ�ѹ�������ó� ��Ҫ�� 
					sysvar.sysfang |= _stop_CH;// �н�ֹ����
					sysvar.batfang &=	~BACHready;
					tim=0;
					if(++sysvar.chingtim1>2)
						EN_CH_ON();
					return CH_DETECTION;//�����������
				}else {
					if(sysvar.chingtim1<2){
						sysvar.chingtim1++;
						return CH_DETECTION;
					}
					else return CH_ERR;
				}
			}
			else {//��ʼ 
				if(ADC_.VinAvg < ADC_.VbatAvg){//�������ѹС�ڵ�ص�ѹ������������
//					sysvar.batfang &= ~(BATCHining);
//					EN_CH_OFF();
//					return CH_NONE;
				}
				sysvar.chingtim2++;// �ۼ�ʱ
				sysvar.batfang |= BATCHining;
				if(sysvar.batfang & BACH_over) 	sysvar.chingtim1++;// �� 
				if(ADC_.VbatAvg>(VBAT_gat(16.8)))//��⵽��ص�ѹ����
					sysvar.chingtim1=0xffff;
				return CH_ING;
			}
		}
	}
	else {
		sysvar.sysfang &= ~(_stop_CH);//����˯��
		sysvar.batfang &= ~(BACHready|BATCHining);
		EN_CH_OFF();
		sysvar.chingtim2=0;
		sysvar.chingtim1=0;
		sysvar.chingtim3=0;
		return 0;
	}
}


void CHruning(){//1s
	if(++sysvar.t6>100-1){
		sysvar.t6=0;
		sysvar.CH_state=CH_task2();
		if(sysvar.sysfang & MOTO_ERR_1){
			UpdataRGBmoodo(Rgb_ERRProtect);
		}
		else if(IDs_.fang & ID_F_ERR_){
			if(IdSensor_fang(IdSensor_PUMP))
				UpdataRGBmoodo(Rgb_ERRNoPumpProtect);
			else if(IdSensor_fang(IdSensor_PUMPERR))
				UpdataRGBmoodo(Rgb_ERRPumpProtect);
		}
		else if(sysvar.batfang & BATpowers_ERR){
			UpdataRGBmoodo(Rgb_ERRBACH_lowin);
		}
		else if(sysvar.sysfang & MOTO_ERR_2){
			UpdataRGBmoodo(Rgb_ERRPumpProtect);
		}
//		else if(sysvar.batfang & (BAT_ERR)){/*��ض���*/  /*��ع���*/
//			sysvar.batfang &= ~(BAT_ERR);
//			UpdataRGBmoodo(Rgb_ERR_BAT);
//		}
		else if(sysvar.sysfang & OFF_ON){/*����*/
			if(sysvar.Modes==MODE_1)UpdataRGBmoodo(Rgb_RUNingStandard);
			else if(sysvar.Modes==MODE_2)UpdataRGBmoodo(Rgb_RUNingStrong);	
			else if(sysvar.Modes==MODE_3)UpdataRGBmoodo(Rgb_RUNingStrong2);
			else if(sysvar.Modes==MODE_CLEANING)UpdataRGBmoodo(Rgb_RUNingCleaning);
		}
		else if(sysvar.keyfang & KeyInstallState){//������
			UpdataRGBmoodo(Rgb_Install);
		}
		else if(sysvar.CH_state){
			if(IDs_.Equipment==f_DragLala){
				if(!(sysvar.sysfang & OFF_ON)){
					if(sysvar.chingtim3==0){
						IDs_Set_Data(0,0,0,ID_Fang_OZ);// 
						sysvar.chingtim3++;
						sysvar.batfang |= BAT_CHOpenOZ;
					}
					else if(++sysvar.chingtim3>60*10){
						if(sysvar.batfang & BAT_CHOpenOZ){
							IDs_Set_Data(0,0,0,0);// 
							sysvar.batfang &= ~BAT_CHOpenOZ;
						}
					}
				}
			}
			if(RGBvar.RGBhandler == RGB_DelayClosing_Handler)
				sysvar.t6=95;
			else if(sysvar.CH_state==CH_DETECTION || sysvar.CH_state==CH_over){
				UpdataRGBmoodo(Rgb_CHover);				//����
				if(sysvar.CH_state==CH_over){
					sysvar.batfang &= ~BATCH_SPOT;
				}
			}
			else if(sysvar.CH_state==CH_ING){
				UpdataRGBmoodo(Rgb_CHing);				//
				sysvar.batfang |= BATCH_SPOT;
			}
//			else if(sysvar.CH_state==CH_OFF){
//				if(RGBvar.Duty>400){
//					RGBvar.Duty-=10;
//					sysvar.t6=900;			//
//				}
//			}
			else if(sysvar.CH_state==CH_STANDBY){//����
				UpdataRGBmoodo(Rgb_Await);
				RGBvar.RGBtim=0;
				sysvar.batfang &= ~BATCH_SPOT;
			}
			else if(sysvar.CH_state==CH_ERR){
				UpdataRGBmoodo(Rgb_CHerr);
				sysvar.batfang &= ~BATCH_SPOT;
			}
			else{
				RGBvar.RGBtim=0;
				UpdataRGBmoodo(Rgb_Await);				// ��Ч
				sysvar.batfang &= ~BATCH_SPOT;
			}
		}
		else{	/* */
			Voice.vfang |= VOICE_ching_f;
			if(RGBvar.RGBhandler!=RGB_OFF_Handler ){//����ΪϨ��
				UpdataRGBmoodo(Rgb_Await);//���� ����
				if(sysvar.batfang & BATCH_SPOT){// �ж�
					VOIC_Control(VTx_Choff,0);
					sysvar.batfang &= ~BATCH_SPOT;
				}else if(sysvar.batfang & BAT_CHOpenOZ){
					if(IDs_.Equipment==f_DragLala)//������
						IDs_Set_Data(0,0,0,0);
					sysvar.batfang &= ~BAT_CHOpenOZ;
				}
			}
		}
	}
}







