#ifndef __MAIN_H
#define __MAIN_H

//#include "types.h"

#include <stdbool.h>
#include <string.h>

//#include "debug.h"
#include "ch32v20x.h"



#include "ADC.h"
#include "GPIO.h"
#include "MOTOR.h"
#include "voice.h"
#include "iwdg.h"
#include "display.h"
#include "IDs.h"
#include "Cellwise_CW2217_Driver_V1.5.h"
#include "wifi.h"

#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include "l_timer.h"
#include "l_driver.h"
#include "l_u8FIFO.h"
#include "f_idle.h"

//#define	 CW2217
//#define TEST


#define  OFF_ON								SET32_BIT(0)
#define  OFF_ONsewage					SET32_BIT(1)
#define	 moto_stop						SET32_BIT(2)
#define	 MOTO_ERR_1						SET32_BIT(3)
#define	 MOTO_ERR_2						SET32_BIT(22)
#define	 TEST_fang						SET32_BIT(4)
#define	 PUMP_Start						SET32_BIT(5)
#define	 _stop_CH							SET32_BIT(6)
#define	 moto_stop2						SET32_BIT(7)
#define  SysDormancy					SET32_BIT(8)

#define  BATpowers_ERR					SET16_BIT(0)
#define	 BACH_over							SET16_BIT(1)
#define	 BACH_low								SET16_BIT(2)
#define	 BACH_lowin2 						SET16_BIT(3)
#define	 BACH_lowin 						SET16_BIT(4)
#define	 BACHready							SET16_BIT(5)
#define	 BATCHining							SET16_BIT(6)
#define  BATCH_SPOT             SET16_BIT(7)
#define	 BAT_INTfang						SET16_BIT(8)
#define	 BAT_CHOpenOZ						SET16_BIT(9)


#define	 KeyLongFang					SET16_BIT(0)
#define	 KeyPressReleaseFang	SET16_BIT(1)
#define	 KeyInstallState			SET16_BIT(2)
#define	 KeyConnecting				SET16_BIT(3)
#define  KeyLongFang2					SET16_BIT(4)
#define	 keyRGBDelayOpen			SET16_BIT(5)



/*Ä£Ê½ºê*/
#define	 MODE_1								1//0
#define	 MODE_2								2//1
#define	 MODE_3								3//1
#define	 MODE_RINSE						4//1
#define	 MODE_CLEANING				5//1


#define BASE_10MS(i)			(10*i)



typedef struct{
  void (*LoadHandler)(void);
  u32 sysfang;
  u16 batfang;
  u16 keyfang;

	u16 PWM_Duty;
	u16 stop_tim;
	u16 IERR_tim;
	u16 clear_ERR_tim;
	u8 CH_state;
	u8 CH_state_IDs;

	u16 pump_ON_tim;
	u16 pump_OFF_tim;
	u16 chingtim1;
	u16 chingtim2;
	u16 chingtim3;
	u16 t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11;
	u8 Show_D1;
	u8 Show_D2;
	u8 BAT_soc;
	u8 EN_motor_;
	u8 PWM_Step;
	u8 Modes;
	u16 data;
	u8 SOC;
	u8 Load;//
	u8 LoadTxNum;//
}_signal;

//extern 

#ifdef _MAIN_C_
#define GLOBAL

#else
#define GLOBAL extern

#endif
GLOBAL u8 keybuf;
GLOBAL _signal  sysvar;
GLOBAL IDs_TypeDef  IDs_;
#undef GLOBAL


void TIM_Init(u16 arr, u16 psc);
void IDs_Judge(void);
void test_run(void);
void CHruning(void);//1s
void mode_determine(void);
void moto_run(void);
void KEY_scanning(void);
void ADC_AnyChanTest(void);
void Motor_Init(const u16 prescaler,const u16 period,const u16 dutyCycle);
void wifi_Send_Group(const u8 Data);
void wifi_Init(void);
void USART3_CFG(void);

void ID_Init(void);
void ID_Send_Byte(unsigned char  dat);
void ID_Send_Group(u8* buf, u8 len);


//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT FMD *****END OF FILE****/
