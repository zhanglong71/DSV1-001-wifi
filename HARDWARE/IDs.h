#ifndef __IDS_H
#define __IDS_H

#include "stdbool.h"
#include "types.h"

#define ID_TxRapid() 	temp_ids[1]=200


#define IdTxCmdRead_ID						(u8)0x01//
#define IdTxCmdRead_Rev						(u8)0x02//
#define IdTxCmdRead_State					(u8)0x03//
#define IdTxCmdEffect							(u8)0x10//
#define IdTxCmdWrite_Rev					(u8)0x12//
#define IdTxCmdHost_REV						(u8)0x13//
#define IdTxCmdControl_1					(u8)0x21//
#define IdTxCmdControl_2					(u8)0x22
#define IdTxCmdControl_3					(u8)0x23


#define ID_F_TX								SET16_BIT(0)//
#define ID_F_Effect	    			SET16_BIT(1)//
#define ID_F_INT  						SET16_BIT(2)//
#define ID_F_TJust  					SET16_BIT(3)//
#define ID_F_RJust  					SET16_BIT(4)//
#define ID_F_Read							SET16_BIT(5)//
#define ID_F_Control    			SET16_BIT(6)//
#define ID_F_Sensor_BOX 			SET16_BIT(7)
#define	ID_F_EquipmentReady		SET16_BIT(8)//
#define	ID_F_ERR_							SET16_BIT(9)//
#define	ID_F_TX_1							SET16_BIT(10)//Éè
#define	ID_F_Rx								SET16_BIT(11)//¶Á



#define ID_Fang_Electrolysis	(u8)0x01//
#define ID_Fang_Pump	  			(u8)0x02//
#define ID_Fang_Air						(u8)0x04//
#define ID_Fang_Charge				(u8)0x08//
#define ID_Fang_LED						(u8)0x10//LED
#define ID_Fang_OZ						(u8)0x20//LED


#define IdSensor_BOX		 			(u8)0x01//bit0
#define IdSensor_CLEAR 				(u8)0x02//bit1
#define IdSensor_SEWAGE 			(u8)0x04//bit2
#define IdSensor_PUMP 				(u8)0x08//bit3
#define IdSensor_PUMPERR 			(u8)0x10//bit4
#define IdSensor_fang(i) 			(IDs_.State_Hall&(i))
#define IdSensor_fCmp(i) 			((IDs_.State_Hall&(i))!=(IDs_.State_HallBRK&(i)))

#define ID_UI_off					(u8)0x01//bit0
#define ID_UI_M1					(u8)0x02//bit1
#define ID_UI_M2					(u8)0x04//bit2
#define ID_UI_M3					(u8)0x08//bit3
#define ID_UI_M4					(u8)0x10//bit4




#define IdSpray_3     (u8)0x01//
#define IdSpray_1     (u8)0x02//
#define IdSpray_2     (u8)0x03//

#define IdSpray_RGB_mask	(u8)0x18


typedef enum {
	f_Wait							=-1,	//
	f_Empty							=0,		//
	f_DragLala					=1,		//
	f_Spray							=2,		//
	f_Aspiration_FOC		=3,		//Îü³¾Æ÷
	f_Aspiration_Brush	=4,		//Îü³¾Æ÷
	f_Feather						=5,		//
	f_WipeWin						=6,		//

}IDsEquipment_TypeDef;


typedef struct{
	u8 D4;	//
	u8 D3;	//
	u8 D2;	//
	u8 D1;
	u8 CMD;	//
	u8 EndMark_1;
	u8 EndMark_2;
}IDs_DATA_TypeDef;


typedef struct{
	IDsEquipment_TypeDef Equipment;
	IDs_DATA_TypeDef TdataTemp;
	IDs_DATA_TypeDef Tdata;

	u8 Effect;//

	u8 State_Charge;//
	u8 State_Hall;	//
	u8 State_Other;	//
	u8 State_HallBRK;

	signed char IDs_CONTROL_Data;//
	u16 fang;
}IDs_TypeDef;


extern u8 temp_ids[];


void IDs_Sens_4b(const u8,const u8);
void IDs_Set_Data(const u8,const u8,const u8,const u8);
void UART_Send_Str(char *s);
#endif


