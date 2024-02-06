#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include <string.h>

#include "main.h"
#include "l_driver.h"

int sysProcess(unsigned *pMsg)
{
    int iRet = TRUE;
    u8Data_t u8Data;
    u8 buf[U8FIFOSIZE];

    //int i = 0;
    u8 len = 0;
    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART3RX:
        if(u8FIFOisEmpty(&g_uart3RxQue) != TRUE) {
            /** do something to uart3 data **/
            //???????????????????????????????????????
            #if 1
            u8 len = 0;
            u8Data_t u8Data;
            objType_t objtype = sm_receiveData(buf);
            if ((objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body)) {
                #if 0
            	len = strlen(buf);
                for(int i = 0; i < len; i++) {
                    u8Data.u8Val = buf[i];
                    u8FIFOin_irq(&g_uart3TxQue, &u8Data);
                }
                #endif
            }
            #endif
            //???????????????????????????????????????
        }

        break;
    case CMSG_UART3TX:
        sm_sendData(NULL);
        break;

    case CGET_CHAR:
        (void)reportGetCharCmd(NULL);
        break;
		
    case CPUT_CHAR:
			(void)JsonParse(NULL, NULL);
         #if 1  //?????????????????? for test only
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
			break;
        
    case CGETDEVINFO_REQ:
    case CGETDEVINFO_RSPERROR:
        (void)reportDevInfo(NULL);
        break;
        
    case CREPORT_RSPERROR:   /** report Services error, try again **/
    case CGETDEVINFO_RSPOK:  /** according protocal report Services after devInfo reported ok **/
        (void)reportService(0);
        break;
        
    case CREPORT_RSPOK:
        /** do nothing **/
        #if 1  //?????????????????? for test only
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'e';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 's';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        break;

    case CWIFI_STATUS:
        /** do what ? **/
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        break;    
        
    case CHEART_BEAT:
        (void)reportHeartbeat(NULL);
        break;
    #if 0
    case CWIFI_TEST:
        /** do test only! 2-byte data !!! **/
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }        
        break;
    #endif
    case CSCAN_WIFI:
         #if 1  //?????????????????? for test only
        u8Data.u8Val = 's';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'c';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'a';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'n';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = ':';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
         /** do test only! 2-byte data !!! **/
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }         
        break;
        
     case CCONN_WIFI:
         #if 1  //?????????????????? for test only
        u8Data.u8Val = 'c';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'o';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'n';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'n';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = ':';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
         /** do test only! 2-byte data !!! **/
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }         
        break;
        
    case CRESET_NET:
    case CRESETNET_RSPFAIL:
        #if 0  //?????????????????? for test only
        u8Data.u8Val = 'r';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'e';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 's';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'e';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = ':';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        (void)reportResetNet(0);
        break;
    case CUART3_TMROUT:
        if(u8FIFOisFull(&g_uart3RxQue) == TRUE) {
            u8FIFOinit_irq(&g_uart3RxQue);
        }
        ClrTimer_irq(&g_timer[1]);
        break;
        
    default:
        iRet = FALSE;
        break;	
	}
    return  iRet;   
}

void DAEMON_USART3_Recive(u8FIFO_t* comRxQue)
{
/**
 * 数据在中断时收到并存入队列! 不需要此函数
 **/
#if 0
    u8Data_t   u8Data;
    if(UART3_GetFlagStatus(UART3_FLAG_RXNE) != RESET) {
    	
    	g_T_UART3Busy = 0;
        u8Data.u8Val = UART3_ReceiveData8();
        charQueueIn_irq(comRxQue, &comData);
    }
#endif
}

void DAEMON_USART3_Send(u8FIFO_t* queue)
{
    u8Data_t u8Data;
    msg_t msg;
    if (u8FIFOisEmpty(queue) == TRUE) {
        return;
    }
    if(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET) {
        return;
    }
    
        //MGPIO_Blink(); //???????????????????????????????
    if(u8FIFOout_irq(queue, &u8Data) == TRUE) {
        USART_SendData(USART3, u8Data.u8Val);
        /** 如果队列已空，数据发送完成 **/
        if(u8FIFOisEmpty(queue) == TRUE) {
            msg.msgType = CMSG_UART3TX;		/** message CMSG_UART3TX **/
            msgq_in_irq(&g_msgq, &msg);
        }
    }  
}

#if 0
void wifi_Send_data(const u8 Data){
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	USART3->DATAR = (Data);
}
#endif

/** nothing to do with old key_scan **/
void key_scan4setwifi(void)
{
	msg_t msg;
    static u8 tmr_key = 0;
    static u8 key_last = 0xff;
#if 1
    if (KEY1 == Bit_SET) {
        if (Mget_bit(key_last, 1) == 0) {
            tmr_key++;
            if (tmr_key == 2) { // 去抖动时间
                msg.msgType = CMSG_DKEY;
                msgq_in(&g_msgq, &msg);
                
                tmr_key = 0;
                Mset_bit(key_last,1);
            }
        } else {
            tmr_key = 0;
        }
    } else { /**KEY1 == Bit_RESET **/
        if (Mget_bit(key_last, 1) == 1) {
            tmr_key++;
            if (tmr_key == 2) { // 去抖动时间
                msg.msgType = CMSG_UKEY;
                msgq_in(&g_msgq, &msg);
                
                tmr_key = 0;
                Mreset_bit(key_last, 1);
            }
        } else {
            tmr_key = 0;
        }
    }

#else
#if 0
    if (Mget_bit(key_last, 1) == Bit_SET) {
	    if (KEY1 != Bit_SET) {
            tmr_key++;
	        if (tmr_key == 2) { // 去抖动时间
			    msg.msgType = CMSG_DKEY;
                msgq_in(&g_msgq, &msg);
                
                tmr_key = 0;
                Mreset_bit(key_last, 1);
            }
	    } else {
	        tmr_key = 0;
		    //Mreset_bit(key_last, 1);
	    }
    } else { /** Mget_bit(key_last, 1) == Bit_RESET **/
        if (KEY1 == Bit_SET) {
            tmr_key++;
            if (tmr_key == 2) { // 去抖动时间
			    msg.msgType = CMSG_UKEY;
                msgq_in(&g_msgq, &msg);
                
                tmr_key = 0;
                Mset_bit(key_last, 1);
            }
        } else {
             tmr_key = 0;
        }
    }
#endif
#endif
}

