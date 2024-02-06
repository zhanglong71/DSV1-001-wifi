#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stdbool.h"
#include "types.h"

#define	SH_Wifiing 			(0X01)//
#define	SH_WifiON 			(0X02)//
#define	SH_WifiOFF 			(0X00)//
#define	SH_VOICE 				(0X04)//

#define	SH_Modo 			(0x0F)//
#define	SH_Modo_M1 		(0x01)
#define	SH_Modo_M2 		(0x02)//
#define	SH_Modo_M3 		(0x04)//
#define	SH_Modo_ERR 	(0x08)//
#define	SH_Modo_ERR2 	(0x10)//


#define	SH_Rgb_EN 	  		(0X01)//
#define	SH_Rgb_Style 	  	(0Xf0)//
#define	SH_Rgb_Style1   	(0X10)//
#define	SH_Rgb_Style2	 		(0X20)//
#define	SH_Rgb_Style3   	(0X30)//


typedef enum {
	Rgb_RUNingStandard=1,
	Rgb_RUNingStrong,
	Rgb_RUNingStrong2,
	Rgb_RUNingRinse,
	Rgb_RUNingCleaning,

	Rgb_CHing,
	Rgb_CHover,
	Rgb_CHerr,

	Rgb_ERR_BAT,
	Rgb_ERRBACH_lowining,

	Rgb_ERRBACH_lowin,
	Rgb_ERRProtect,
	Rgb_ERRPumpProtect,
	Rgb_ERRNoPumpProtect,
	Rgb_ERRSewageProtect,

	Rgb_ERRNoEquipment,
	Rgb_ERREquipment,
	Rgb_Install,

	Rgb_UPelectricity,
	Rgb_Await,
	Rgb_Off,
}RGBstate_TypeDef;

typedef struct{
	u8 S_SOC;
	u8 S_Modo;//
	u8 S_WiFi;//
	u8 S_RGB;//
	u8 S_Brig;//
	u8 S_R_Brig;//
	u8 S_G_Brig;//
	u8 S_B_Brig;//
	u8 S_Sum;
	u8 EndMark1;//
	u8 EndMark2;//
}SHOW_TypeDef;


typedef struct{
	void (*RGBhandler)(void);
	RGBstate_TypeDef state;
	SHOW_TypeDef Show;
	SHOW_TypeDef ShowBRK;

	u16 frequency;//
	u16 RGBtim;//
	u16 Duty;
	u8 direction;//
	u8 FlickerNum;
	u8 RGB_FlickerNum;
	u8 RGB_Prepare_;//
	u8 RGB_Prepare_brk;//
//	unsigned char RGBnumber;
//	bool PWMover;
}RGB_TypeDef;

#ifdef _RGB_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL RGB_TypeDef RGBvar;

#undef GLOBAL


void Disp_Send_Group(void);
void Disp_Send_Group_D1(const u8 Data);
void Disp_Send_Group_D2(const u8 Data);
void RGB_DelayClosing_Handler(void);
void RGB_DelayOFF_Handler(void);
void RGB_SingleFlashingLED4_Handler(void);
void RGB_SingleFlashingLED6_Handler(void);
void RGB_SingleFlashingERR_Handler(void);
void RGB_Uunwanted_Handler(void);
void RGB_OFF_Handler(void);
void RGB_SingleBreathingFlashing_Handler(void);
void Line_scan(void);
void UpdataRGBmoodo(RGBstate_TypeDef);
void RGB_oneSingleBreathing_Handler(void);
void Display_GPIO_Shut(void);
void Display_GPIO_Init(void);
void display_Init(void);
void UART2_Send_Group(unsigned char * buf, unsigned char len);
#endif

