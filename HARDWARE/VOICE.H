#ifndef __voice_H
#define __voice_H
#include "stdbool.h"
#include "types.h"


#define CH_ERR						5//
#define VOIC_PWMvar(n)			TIM2->CH2CVR=n


typedef enum {
	VTx_ConnectNo=1,
	VTx_Disconnect=2,
	VTx_Install=3,//
	VTx_VoiceOpen=4,
	VTx_VoiceClose=5,//
	VTx_WifiReset=6,//
	VTx_WifiConnecting=7,//
	VTx_WifiOk=8,//
	VTx_CHing=9,//
	nop1=10,//
	VTx_Choff=11,//充电中断
	VTx_RUNm1=12,//进入标准模式
	VTx_RUNm2=13,//进入强力模式
	nop2=14,//
	VTx_RUNCL=15,//
	VTx_RunclOver=16,//
	VTx_RUNover=17,//
	VTx_RUNOFF=18,//
	VTx_Chlowing=19,//
	nop3=20,//
	nop4=21,//
	VTx_CHErr=22,//
	VTx_ConnectDragLala=23,//
	VTx_CisternOk=24,//
	VTx_CisternNo=25,//
	VTx_CisternTake=26,//
	VTx_sewageErr=27,//
	VTx_ClearErr=28,//
	VTx_PumpErr=29,//
	nop5=30,//
	VTx_RollerErr=31,//
	VTx_ConnectAspiration=32,//
	VTx_DeviceErr=33,//
	VTx_ConnectFeather=34,//
	nop6=35,//
	nop7=36,//
	nop8=37,//
	VTx_ConnectSpray=38,//
	VTx_RUN3=39,//


  VTx_prompt,			  //

	VTx_LCD_RunclOver,//
	VTx_LCD_CHlowinOver,//

	VTx_set,
	VTx_STOP,
}VOIC_TypeDef;



#define WaitingVoic		 true	
#define NoWaitingVoic  false
	


#define VOICE_Waiting  		SET8_BIT(0)
#define VOICE_InitFang  	SET8_BIT(1)
#define VOICE_ching_f  		SET8_BIT(3)
#define VOICE_runOff_f  	SET8_BIT(4)

typedef	union{
	struct{
		u8 ContA;//
		u8 ContB;
	}Cont8;
	u16 Cont16;
}VoiceCont_TypeDef;

typedef struct{
	VoiceCont_TypeDef VoiceController;
	u16 vfang;
	VOIC_TypeDef stat;
	signed char SendSteps;

	u8 SendDATA;
	u8 ReadData;
	u8 TxData;
	u16 tim;
	u16 tim2;
}Voice_TypeDef;

extern Voice_TypeDef Voice;










extern u16 USART_RX_STA;
extern u8 USART_RX_BUF;
extern u8 VOIC_who;

void Change_List_Sound(u8 list_id);
void VOIC_Control(const u8 stack,const u8 Waiting);
void Voice_exe(void);
void Voice_Init(void);
#endif



