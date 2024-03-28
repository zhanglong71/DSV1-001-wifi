#ifndef __CONST_H__
#define __CONST_H__

/*******************************************************************************/

/*******************************************************************************
 *
 * constant define here
 *
 *******************************************************************************/
#define	STACKSIZE	8
#define	QUEUESIZE	8

#define U8FIFOSIZE   128
//#define	OK	0
//#define	ERROR	-1

#define	TRUE	1
#define	FALSE	0

#define CSEND_BUF_NUM	8	//16	 //缓冲区个数
#define CSEND_BUF_SIZE	16	//16	 //缓冲区大小

#define TIMER_NUM	3       /** 定时器个数 **/

#define	CMAX1_COUPLE		6    // the max number of kv couple in one object between master and slave (uart1 communicate)

/** 
 * 0 - 计时计数
 * 1 - 开/闭锁
 * 2 - VOP语音
 * 3 - led blink
 * 4 - ADC采集周期
 */
#define TIMER_0 	0		/** 100Hz定时, 0等于关闭时钟 **/
#define TIMER_10MS (1)		/** 100Hz定时, 10ms计数次数 **/

#define TIMER_50MS	(TIMER_10MS * 5)
#define TIMER_100MS (TIMER_10MS * 10)

#define TIMER_200MS (TIMER_10MS * 20)
#define TIMER_500MS (TIMER_10MS * 50)
#define TIMER_800MS (TIMER_10MS * 80)
#define TIMER_1SEC  (TIMER_10MS * 100)

#define TIMER_2SEC  (TIMER_10MS * 200)
#define TIMER_5SEC  (TIMER_10MS * 500)
#define TIMER_10SEC (TIMER_10MS * 1000)
#define TIMER_20SEC (TIMER_10MS * 2000)
#define TIMER_30SEC (TIMER_10MS * 3000)
#define TIMER_1MIN (TIMER_10MS * 6000)

/*******************************************************************************/
/** ֻҪ����ͬ���� **/
#define CINDEX_UNKNOW                 (0)

#define CINDEX_STANDBY                (1)
#define CINDEX_STANDARD               (2)  
#define CINDEX_HIGHPOWER              (3)
#define CINDEX_CLEANING               (4)
#define CINDEX_RINSE                  (5)

#define CINDEX_ROLLEROVERLOAD         (6)
#define CINDEX_ROLLERNORMAL           (7)

#define CINDEX_CLEARWATERNORMAL       (8)
#define CINDEX_CLEARWATERSHORTAGE     (9)

#define CINDEX_PUMPNORMAL             (10)
#define CINDEX_PUMPOVERLOAD           (11)
#define CINDEX_PUMPCURRENTSMALL       (12)

#define CINDEX_BATTERYNORMAL          (13)
#define CINDEX_BATTERYLOW             (14)
#define CINDEX_BATTERYLEVEL           (15)

#define CINDEX_UNCHARGED              (16)
#define CINDEX_CHARGING               (17)
#define CINDEX_CHARGECOMPLETE         (18)
#define CINDEX_CHARGEFAULT            (19)

#define CINDEX_CONNECTED              (20)
#define CINDEX_DISCONNECTED           (21)
#define CINDEX_NETINFO                (22)
#define CINDEX_UPDATE                (23)

#define CINDEX_TANKINPLACE            (31)
#define CINDEX_TANKNOTINPLACE         (32)
#define CINDEX_DORMANCY               (33)
#define CINDEX_SCREENSHUTDOWN         (34)
#define CINDEX_NEUTRAL                (35)
#define CINDEX_CONNECTION             (36)
#define CINDEX_FLUSHING               (37) 
#define CINDEX_FULLSEWAGE             (38)
#define CINDEX_SETUP                  (39)

#define CINDEX_INVALID (0xff)

/*******************************************************************************/
#endif
