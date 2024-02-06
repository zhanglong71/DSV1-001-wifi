#ifndef __GPIO_H_
#define __GPIO_H_
#include "ch32v20x_gpio.h"
#include "ch32v20x_rcc.h"

#define KEY1_GPIO 			GPIOA
#define KEY1_PIN 				GPIO_Pin_8
#define KEY1_GPIOSRC 		GPIO_PortSourceGPIOA
#define KEY1_PINSRC 		GPIO_PinSource8
#define KEY1_EXT_Line 	EXTI_Line8
#define KEY1_NVIC_Ch  	EXTI9_5_IRQn
#define KEY1   					GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_PIN)		

#define IN_CH_IN_GPIO 			GPIOB
#define IN_CH_IN_PIN 				GPIO_Pin_9
#define IN_CH_IN_GPIOSRC 		GPIO_PortSourceGPIOB
#define IN_CH_PINSRC 				GPIO_PinSource9
#define IN_CH_EXT_Line 			EXTI_Line9
#define IN_CH_IN_NVIC_Ch  	EXTI9_5_IRQn
#define IN_CH_IN()  				GPIO_ReadInputDataBit(IN_CH_IN_GPIO,IN_CH_IN_PIN)

#define BAT_INT_IN_GPIO 		GPIOB
#define BAT_INT_IN_PIN 			GPIO_Pin_15
#define BAT_INT_IN_GPIOSRC 	GPIO_PortSourceGPIOB
#define BAT_INT_PINSRC 			GPIO_PinSource15
#define BAT_INT_EXT_Line 		EXTI_Line15
#define BAT_INT_IN_NVIC_Ch  EXTI15_10_IRQn
#define BAT_INT_IN()  			GPIO_ReadInputDataBit(BAT_INT_IN_GPIO,BAT_INT_IN_PIN)

#define IDs_INT_GPIO 					GPIOA
#define IDs_INT_PIN 					GPIO_Pin_2
#define IDs_INT_GPIOSRC 			GPIO_PortSourceGPIOA
#define IDs_INT_PINSRC 				GPIO_PinSource2
#define IDs_INT_EXT_Line 			EXTI_Line2
#define IDs_INT_NVIC_Ch  			EXTI2_IRQn
#define IDs_INT_IN()  				GPIO_ReadInputDataBit(IDs_INT_GPIO,IDs_INT_PIN)

#define IN_WATER_PIN 			GPIO_Pin_7
#define IN_WATER_OFF()  	GPIO_ResetBits(GPIOA,IN_WATER_PIN)
#define IN_WATER_ON()  		GPIO_SetBits(GPIOA,IN_WATER_PIN)
#define IN_WATER_IN()  		GPIO_ReadInputDataBit(GPIOA,IN_WATER_PIN)

#define EN_5V_PIN 			GPIO_Pin_15
#define EN_5V_OFF()  		GPIO_ResetBits(GPIOA,EN_5V_PIN)
#define EN_5V_ON()  		GPIO_SetBits(GPIOA,EN_5V_PIN)
#define EN_5V_IN()  		GPIO_ReadOutputDataBit(GPIOA,EN_5V_PIN)

#define EN_CH_PIN 			GPIO_Pin_4
#define EN_CH_ON()  		GPIO_SetBits(GPIOA,EN_CH_PIN)
#define EN_CH_OFF()  		GPIO_ResetBits(GPIOA,EN_CH_PIN)
#define EN_CH_IN()  		GPIO_ReadOutputDataBit(GPIOA,EN_CH_PIN)

#define EN_WATERV_PIN 	GPIO_Pin_6
#define EN_WATER_ON()  	GPIO_SetBits(GPIOA,EN_WATERV_PIN)
#define EN_WATER_OFF() GPIO_ResetBits(GPIOA,EN_WATERV_PIN)
#define EN_WATER_IN()  GPIO_ReadOutputDataBit(GPIOA,EN_WATERV_PIN)



#define GPIOL_install(gpx,pinx,modex)\
	gpx->CFGHR &= ~(GPIO_CNF_MODE_MASK << pinx),\
	gpx->CFGHR |= (modex << pinx)


#define GPIO_install(gpx,pinx,modex)\



#define GPIO_CNF_MODE_AIN             0x00UL //0b0000,                                ///< 模拟输入
#define GPIO_CNF_MODE_FLOATING        0x04UL //0b0100,                                ///< 浮空输入
#define GPIO_CNF_MODE_INPUPD          0x08UL //0b1000,                                ///< 上下拉输入
#define GPIO_CNF_MODE_INRESEVED       0x0CUL //0b1100,                                ///<
#define GPIO_CNF_MODE_10MHZ_OUT_PP    0x01UL //0b0001,                                ///< 通用推拉   10MHZ
#define GPIO_CNF_MODE_10MHZ_OUT_OD    0x05UL //0b0101,                                ///< 通用开漏   10MHZ
#define GPIO_CNF_MODE_10MHZ_AF_PP     0x09UL //0b1001,                                ///< 复用推完   10MHZ
#define GPIO_CNF_MODE_10MHZ_AF_OD     0x0DUL //0b1101                                 ///< 复用开漏   10MHZ
#define GPIO_CNF_MODE_2MHZ_OUT_PP     0x02UL //0b0010,                                ///< Universal push-pull  output 2MHZ
#define GPIO_CNF_MODE_2MHZ_OUT_OD     0x06UL //0b0110,                                ///< Universal open drain output 2MHZ
#define GPIO_CNF_MODE_2MHZ_AF_PP      0x0AUL //0b1010,                                ///< Multiplex push-pull  output 2MHZ
#define GPIO_CNF_MODE_2MHZ_AF_OD      0x0EUL //0b1110                                 ///< Multiplex open drain output 2MHZ
#define GPIO_CNF_MODE_50MHZ_OUT_PP    0x03UL //0b0011,                                ///< Universal push-pull  output 50MHZ
#define GPIO_CNF_MODE_50MHZ_OUT_OD    0x07UL //0b0111,                                ///< Universal open drain output 50MHZ
#define GPIO_CNF_MODE_50MHZ_AF_PP     0x0BUL //0b1011,                                ///< Multiplex push-pull  output 50MHZ
#define GPIO_CNF_MODE_50MHZ_AF_OD     0x0FUL //0b1111                                 ///< Multiplex open drain output 50MHZ
#define GPIO_CNF_MODE_MASK            0x0FUL //0b1111	掩码

#define GPIO_CRL_CNF_MODE_0_Pos       (0)  //                                       ///< Analog input
#define GPIO_CRL_CNF_MODE_1_Pos       (4)  //                                       ///< Floating input
#define GPIO_CRL_CNF_MODE_2_Pos       (8)  //                                       ///< Pull up and down input
#define GPIO_CRL_CNF_MODE_3_Pos       (12) //                                       ///< Reseved input mode
#define GPIO_CRL_CNF_MODE_4_Pos       (16) //                                       ///< Universal push-pull  output default
#define GPIO_CRL_CNF_MODE_5_Pos       (20) //                                       ///< Universal open drain output default
#define GPIO_CRL_CNF_MODE_6_Pos       (24) //                                       ///< Multiplex push-pull  output default
#define GPIO_CRL_CNF_MODE_7_Pos       (28) //                                       ///< Multiplex open drain output default
#define GPIO_CRH_CNF_MODE_8_Pos       (0)  //                                       ///< Universal push-pull  output 50MHZ
#define GPIO_CRH_CNF_MODE_9_Pos       (4)  //                                       ///< Universal open drain output 50MHZ
#define GPIO_CRH_CNF_MODE_10_Pos      (8)  //                                       ///< Multiplex push-pull  output 50MHZ
#define GPIO_CRH_CNF_MODE_11_Pos      (12) //                                       ///< Multiplex open drain output 50MHZ
#define GPIO_CRH_CNF_MODE_12_Pos      (16) //                                       ///< Universal push-pull  output 20MHZ
#define GPIO_CRH_CNF_MODE_13_Pos      (20) //                                       ///< Universal open drain output 20MHZ
#define GPIO_CRH_CNF_MODE_14_Pos      (24) //                                       ///< Multiplex push-pull  output 20MHZ
#define GPIO_CRH_CNF_MODE_15_Pos      (28) //                                       ///< Multiplex open drain output 20MHZ





void IO_Init(void);
void Power_off_Mode (void);
void STOPdown(void);
void moto_run(void);
void AF_GPIO_PinConfigPushPull(GPIO_TypeDef* GPIOn,const u16 gpio_pin);
void ADC_GPIO_PinConfigAnalog(GPIO_TypeDef* GPIOn,const u16 gpio_pin);
#endif




