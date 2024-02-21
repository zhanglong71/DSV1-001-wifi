#ifndef __PTYPE_H__
#define __PTYPE_H__

/*******************************************************************************
 *
 * All data type define here
 *
 *******************************************************************************/
 
/*******************************************************************************/
typedef enum {
    FAILED = 0, 
    PASSED = !FAILED
} RetStatus;

typedef	int	 (*pfunc_t)(unsigned *arg);
typedef	void (*paction_t_0)(void);

typedef struct func_s {
    pfunc_t func;       /** function **/
    unsigned int  *arg;    /** argument **/
} func_t;

typedef struct func_stack{
	u8 top;
	func_t func[STACKSIZE]; 
}fstack_t;

typedef enum {
    CMSG_NONE = 0, 
/*************
 * 重新拟定消息命名规则
 * 1. 普通以CMSG_xxx形式命名
 * 2. 以CSYS_xxx的形式命令在sys中处理的消息
 *************/    
    CMSG_PWON,  	//power on(需要初始化必须的数据)
    CMSG_RESU,  	//resume (从挂起状态恢复。需要初始化必须的数据及外设)
    CMSG_PEND,  	//pending (挂起，外设停止工作)
    
    CSYS_INIT,  	//初始化动作(进入选择)
    CMSG_INIT,  	//初始化动作(进入选择)
    CRESP_INFO,
    
    CGETDEVINFO_REQ,
    CGETDEVINFO_RSPOK,
    CGETDEVINFO_RSPERROR,
    
    CREPORT_RSPOK,
    CREPORT_RSPERROR,
    CGET_CHAR,
    CPUT_CHAR,
    CHEART_BEAT,
    CWIFI_STATUS,
    CSCAN_WIFI,
    
    CCONN_WIFI,
    CCONNWIFI_RSPOK,
    CCONNWIFI_RSPFAIL,
    
    CRESET_NET,
    CRESETNET_RSPOK,
    CRESETNET_RSPFAIL,
    //CWIFI_TEST,
    
    CMSG_TMR,
    CMSG_DLAY, 	/** 时钟 **/
    CMSG_NEXT, 	/** 进行入下一阶段 **/
    CMSG_DKEY,
    CMSG_UKEY,
    CDOUBLE_CLICK,
 
    CMSG_BLELINK,		/** 蓝牙配对连接完成(可以发数据了) **/    
    //CMSG_FGCCEL,		/** 退出操作成功(0x0130回答) **/
    //CMSG_FGCCELING,	/** 退出操作成功(0x0130回答) **/
    
    CMSG_FGDOING,		/** 指纹处理继续 **/
    
	CNOKEY_ENTER,		/** 无钥匙进入 **/
	
    CMSG_COMTX,
    CMSG_COMRX,
    
    CMSG_UART3RX,
    CMSG_UART3TX,
    CUART3_TMROUT,
    
    CMSG_RMT,			/** remote **/
    CMSG_PRY,			/** anti-pry **/
    
    //CMSG_BLTOOTH,		/** 蓝牙数据信息 **/
	CMSG_BTMATCH,		/** 蓝牙数据查询匹配 **/
    CMSG_BTENROLL,		/** 蓝牙数据注册 **/
    
	/**  **/
	//CVOP_STRT,		/** voice prompt start **/
	//CVOP_OVER,

	CADC_TOUT,		/** 动作超时 **/
	CADC_OVER,		/** 动作完成 **/
	
	CACT_TOUT,		/** 动作超时 **/
	CACT_OVER,		/** 动作完成 **/
	
	CPMT_TOUT,		/** 提示动作超时 **/
	CPMT_OVER,		/** 提示动作完成 **/
	
	CBLK_TOUT,		/** blink提示动作超时 **/
	CBLK_OVER,		/** blink提示动作完成 **/

    //CMSG_ADC,		/** AD转换 **/
    //CMSG_CALC,		/** 重新校验低电参照点 **/
    
    CUART1_RCV,
    CUART2_RCV,
    
    CIIC_INIT,		/** IIC接口初始化 **/
    
    CMSG_COMTIMEOUT,
} msgType_t;

typedef struct msg_s {
	msgType_t msgType;
	u8	msgValue;
}msg_t;

typedef struct msg_queue{
	u8	head;
	u8	tail;
	msg_t msg[QUEUESIZE];
} msgq_t;

typedef struct Timer_s {
    u16 tick_bak;
    u16 tick;
    msgType_t msgType;                      /** the type of msg when timeout **/
    //unsigned int count;                   /** Is it necessary ? **/
}Timer_t; 

typedef struct jsonTL_s {
	u8 *jHead;
    u16 jLen;    /** ָ������! �����0�����ǲ�ȷ�� **/
    u8 *jBody;
    void* arg; /** �յ���Ӧ�������Ӧ���� �� ���� **/
}jsonTL_t;

//typedef struct map_s {
//    void *var1;
//    void *var2;
//}map_t;

typedef struct pair_s {
	void *first;
    void *second;
}pair_t;

typedef struct pair_u8u8_s {
   u8 first;
   u8 second;
}pair_u8u8_t;


#if 0
typedef struct bitmap_s {
    //unsigned int bitmap;		    /** 32 bits **/
    u8 bitmap[40];				/** 40 * 8 = 320bits **/
} bitmap_t;

typedef enum {              /** 用于控制com数据的发送 **/
    CTRL_START = 0,         /** 0 - start transmission **/
    CTRL_CONTI = 1,         /** 1 - continue transmission  **/
                            /** 2 reserved. for other control **/
    CTRL_TMR = 5,           /** 3..200 for delay **/
    CTRL_STOP = 255,        /** 255 - stop transmission**/
} ctrlFlag_t;
#endif

typedef struct u8Data_s{
	u8 u8Val;
} u8Data_t;

typedef struct u8FIFO_s {
    u8 in;
    u8 out;
    //u8 len;
	//u8 __pad1;
    u8 buf[U8FIFOSIZE];     /** 16 bytes **/
} u8FIFO_t;

#if 0
typedef struct action_s {
	/*****************
	 * MOT-ON
	 * MOT-OFF
	 * LED-ON
	 * LED-OFF
	 * VOP-ON
	 * VOP-OFF
	 * Delay
	 *****************/
#define CACT_MOTONORM	0xff
#define CACT_MOTOREVE	0xfe
#define CACT_MOTOWAIT	0xfd
  
#define CACT_LEDPMTON	0xfb
#define CACT_LEDPMTOFF	0xfa
#define CACT_LEDPMTWAT	0xf9
  
#define CACT_LEDWRNON	0xf7
#define CACT_LEDWRNOFF	0xf6
#define CACT_LEDWRNWAT	0xf5
  
#define CACT_VOPON	0xf3
//#define CACT_VOPOFF	0xf2
#define CACT_VOPWAT	0xf1
//#define CACT_DELAY	0xf0
  
#define CACT_BEEPON	0xef
#define CACT_BEEPOFF	0xee


#define CACT_MISC	0xeb


    paction_t_0		func;
    u16 actionTime;	/** 持续时间 **/
    u16 actionPara;	/** 动作需要的其它参数，如VPidx索引号 **/
    u8 actionType;	/** 动作类型. 似与前面的参数有重叠之嫌，是一个区别动作的依据 **/
    u8 __pad1;		/** 字节对齐 **/
} action_t;

typedef struct actionQueue_s {
    u8	head;
	u8	tail;
    u8 flag;
	msgType_t stepMsgType;			/** 阶段事件完成消息 **/
	msgType_t overMsgType;			/** 队列事件全部完成消息 **/
	Timer_t *timer;
        #define ACTIONQUEUESIZE 16
    action_t buf[ACTIONQUEUESIZE];     /** 16 bytes **/
} actionQueue_t;
#endif
/*******************************************************************************/
#if 0
typedef struct iicTxData_s{
	u8 addr;
	u8 data;
} iicTxData_t;

typedef struct iicData_s{
	u8 addr;
	u8 data;
	u8 stat;		//用于标识数据状态
	/************************************************** 
		stat = 	0 新加载待处理数据(手动加载时)
				1 addr正在发送中(发送动作时)
				2 data正在发送中(发送动作时)
				3 完成data发送(发送data完成，待停止)	
	 **************************************************/
	u8 _pad;		//reserved
} iicData_t;
#endif
/*******************************************************************************/
#if 0
typedef struct adcData_s{
    u16	u16count;				//计数
	u16	u16currValue;			//当前值
    u16 u16average;				//均值
    
	u16 tick;
	msgType_t stepMsgType;		/** 阶段事件完成消息 **/
	Timer_t *timer;
} adcData_t;
#endif
/*******************************************************************************/

typedef enum {
    sm_none = 0,
    sm_init, 
    sm_step,
    sm_key,
    sm_sendBody,
    sm_receiveLen,
    sm_receiveLenStep1,
    sm_receiveLenStep2,
    sm_receiveLenStep3,
    sm_receiveBody,
    sm_end
} smStatus_t;

typedef enum {
    obj_none = 0,
    obj_key,
    obj_len,
    obj_body
} objType_t;

/*******************************************************************************/
typedef struct ComponentField_s{
	u8 moto;
	u8 pump;
	u8 battery;
	u8 charge;
	u8 clear;
} ComponentField_t;

typedef struct reportStatusBody_s{
	u8 index;
	u8* body;
} reportStatusBody_t;
/*******************************************************************************/

#endif /** ifndef end **/
