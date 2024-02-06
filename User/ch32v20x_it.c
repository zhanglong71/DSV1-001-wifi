/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v20x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main Interrupt Service Routines.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include "main.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void){
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void){
  while (1){
  }
}

void EXTI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
  	EXTI_ClearFlag(IDs_INT_EXT_Line);     /* Clear Flag */
  	IDs_.fang |= ID_F_INT;
  }
}


void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void){
//	EXTI_ClearFlag(KEY1_EXT_Line);

}
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(KEY1_EXT_Line)!=RESET){// 计中断
		EXTI_ClearFlag(KEY1_EXT_Line);
	}
	else if(EXTI_GetITStatus(IN_CH_EXT_Line)!=RESET){// 中断
		EXTI_ClearFlag(IN_CH_EXT_Line);

	}
}

void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(BAT_INT_EXT_Line)!=RESET){// 计中断
		sysvar.batfang |= BAT_INTfang;
		EXTI_ClearFlag(BAT_INT_EXT_Line);
	}

}


void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void){
    SysTick->SR=0;
	TimingDelay_Decrement();
    key_scan4setwifi();
}





