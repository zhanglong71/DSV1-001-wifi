#ifndef __MOTOR_H
#define __MOTOR_H
#include "types.h"

#define SERVICE_OFF				sysvar.LoadHandler=LOAD_Idle_Handler,sysvar.sysfang &= ~(OFF_ON),sysvar.keyfang &= ~(KeyPressReleaseFang)


#define MOTOSPWM_REG				TIM1->CH1CVR
#define MOTOSPWM_OFF				MOTOSPWM_REG=0,sysvar.LoadHandler=LOAD_Idle_Handler,sysvar.sysfang &= ~(OFF_ON),sysvar.keyfang &= ~(KeyPressReleaseFang)
#define MOTOSPWM_load(n)		MOTOSPWM_REG=n

void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp);
void moto_run(void);


void Run_DragLala(void);
void Run_Spray(void);
void Run_Aspiration_FOC(void);
void Run_Aspiration_Brush(void);
void Run_Feather(void);
void Run_WipeWin(void);


void MOTO1_ControlInformation(void);
void MOTO2_ControlInformation(void);
#endif

