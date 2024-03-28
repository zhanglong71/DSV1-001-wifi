#ifndef __MACRO_H__
#define __MACRO_H__
/*******************************************************************************/
/***************************** 数组大小 ****************************************/
#define	MTABSIZE(x)	(sizeof(x)/sizeof(x[0]))
/*******************************************************************************/
#define	MIRQ_enable()	__enable_irq()		//中断使能
#define	MIRQ_disable()	__disable_irq()		//中断禁止
/********************************** 按键背光LED ********************************/
/*********************************************************************************/
#define Mset_bit(var, offset) do{	\
        ((var) | (1 << (offset))); \
    }while(0)
    
#define Mreset_bit(var, offset) do{	\
        ((var) & (~(1 << (offset)))); \
    }while(0)

#define Mget_bit(var, offset) \
    !(!((var) & (1 << (offset))))
/*********************************************************************************/


/*********************************************************************************/
#define	MFPACK_FIFO_IN(buf, x) do{	\
	buf[0] = ((buf[0] >= CMAX_PSBUF - 1)? (CMAX_PSBUF - 1):(buf[0] + 1));	\
	buf[buf[0] + 2] = x;	\
	}while(0)
//only for get data from uart1 received buffer(main context)
#define	MFPACK_FIFO_OUT(buf, x) do{	\
	buf[1] = ((buf[1] >= CMAX_PSBUF - 1)? (CMAX_PSBUF - 1):(buf[1] + 1));	\
	x = buf[buf[1] + 2];	\
	}while(0)

// for debug


/*********************************************************************************/
#define CDEBUG_PORT GPIO_Pin_4
#define	MGPIO_Toggle_INIT() do{	\
        GPIO_InitTypeDef GPIO_InitStructure = {0};             \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  \
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;              \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       \
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      \
        GPIO_Init(GPIOA, &GPIO_InitStructure);                 \
	}while(0)                                              \

#define	MGPIO_Blink() do{	\
        if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) { \
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);    \
        } else {                                          \
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);  \
        }                                                 \
	}while(0)                                             \
    
#define	MGPIO_SET() do{	\
         GPIO_SetBits(GPIOA, GPIO_Pin_4);       \
	}while(0)                                             \
     
#define	MGPIO_RESET() do{	\
         GPIO_ResetBits(GPIOA, GPIO_Pin_4);     \
    }while(0)                                             \
     
/*******************************************************************************/
#define	MisRespOk(len, body)  (((len) == 2) && (strnstr((body), "ok", (len)) != NULL)) 
#define	MisRespFail(len, body)  (((len) == 4) && (strnstr((body), "fail", (len)) != NULL))
#define	MisRespErr(len, body)  (((len) == 5) && (strnstr((body), "error", (len))!= NULL))
/********************************/

#define	MisDevinfo(idx)  ((idx) == 0)    /** the index in table commandKeyArr **/
#define	MisDevinfoRespOk(idx, len, body)  (MisDevinfo(idx) && (MisRespOk(len, body)))
#define	MisDevinfoRespErr(idx, len, body)  (MisDevinfo(idx) && (MisRespErr(len, body))) 

#define	MisReportSerice(idx)  ((idx) == 4)   /** the index in table commandKeyArr **/
#define	MisReportSericeRespOk(idx, len, body)  (MisReportSerice(idx) && (MisRespOk(len, body)))
#define	MisReportSericeRespErr(idx, len, body)  (MisReportSerice(idx) && (MisRespErr(len, body))) 

#define	MisHeartbeatCommand(idx)  ((idx) == 1)   /** the index in table commandKeyArr **/

#define	MisPutChar(idx)  ((idx) == 2)    /** the index in table commandKeyArr **/

#define	MisGetChar(idx)  ((idx) == 3)    /** the index in table commandKeyArr **/
#define	MisGetCharMopStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 3) && (strnstr((body), "mop", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharRollerStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strnstr((body), "roller", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharClearWaterStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 10) && (strnstr((body), "clearWater", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharPumpStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 4) && (strnstr((body), "pump", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharBatteryStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 13) && (strnstr((body), "batterystatus", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharChargeStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strnstr((body), "charge", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharNetInfoStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 7) && (strnstr((body), "netInfo", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharUpdateStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strnstr((body), "update", len) != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strnstr((body), "status", len) != NULL))    /** the index in table commandKeyArr **/

#define	MisScanWifi(idx)  ((idx) == 5)       /** the index in table commandKeyArr **/
#define	MisScanWifiRespFail(idx, len, body) (MisScanWifi(idx) && (MisRespFail(len, body)))

#define	MisconnectWifi(idx)  ((idx) == 6)       /** the index in table commandKeyArr **/
#define	MisconnectWifiRespOk(idx, len, body)  (MisconnectWifi(idx) && (MisRespOk(len, body))) 
#define	MisconnectWifiRespFail(idx, len, body) (MisconnectWifi(idx) && (MisRespFail(len, body)))

#define	MisputWifiStatus(idx)  ((idx) == 8)    /** the index in table commandKeyArr **/

#define	MisgetWifiStatus(idx)  ((idx) == 9)    /** the index in table commandKeyArr **/
#define	MisgetWifiStatusExpect(idx, len, value, expected)  ((MisgetWifiStatus(idx)) && ((len) == 1) && ((value) == (expected)))   /** the index in table commandKeyArr **/

#define	MisResetNet(idx)  ((idx) == 10)       /** the index in table commandKeyArr **/
#define	MisResetNetRespOk(idx, len, body)   (MisResetNet(idx) && (MisRespOk(len, body))) 
#define	MisResetNetRespFail(idx, len, body) (MisResetNet(idx) && (MisRespFail(len, body)))

#define	MisGetSsid(idx)  ((idx) == 11)       /** the index in table commandKeyArr **/
#define	MisGetIp(idx)    ((idx) == 12)       /** the index in table commandKeyArr **/
#define	MisGetMac(idx)   ((idx) == 13)       /** the index in table commandKeyArr **/

#define	MisGetRssi(idx)  ((idx) == 14)       /** the index in table commandKeyArr **/
#define	MisGetRssiFail(idx, len, body)   (MisGetRssi(idx) && (MisRespFail(len, body)))       /** the index in table commandKeyArr **/

#define	MisPutSync(idx)  ((idx) == 15)       /** the index in table commandKeyArr **/

// #define	MisTestWifiResponse(idx, len, body)  (((idx) == CTestWIFIkeyIdx) &&  ((len) == 2) && (strlen(body) == 2))

/*******************************************************************************/

#define	MCOMMENT(x)
/*******************************************************************************/

#endif
