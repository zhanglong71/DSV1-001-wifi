#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "l_driver.h"
#include "l_jsonTL.h"

int sysProcess(unsigned *pMsg)
{
    func_t func;
    msg_t msg;
    int iRet = TRUE;
    u8Data_t u8Data;
    u8 buf[U8FIFOSIZE];
    kv_t KVarr[3];
    int flag = 0;
    //int i = 0;
    u8 len = 0;
    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART3RX:
        if(u8FIFOisEmpty(&g_uart3RxQue) != TRUE) {
            /** do something to uart3 data **/
            u8Data_t u8Data;
            objType_t objtype = sm_receiveData(buf);
            if (objtype == obj_none) {
                /** nothing **/
            } else if ((objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body)) {
                /** do something **/
                /** what ? **/
            } else if (objtype == obj_SSID) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "ssid")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ssid, KVarr[i].value, sizeof(g_netInfo.ssid));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 0);
                }
            } else if (objtype == obj_IP) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "ip")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ip, KVarr[i].value, sizeof(g_netInfo.ip));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 1);
                }
            } else if (objtype == obj_MAC) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "mac")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.mac, KVarr[i].value, sizeof(g_netInfo.mac));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 2);
                }
            } else if (objtype == obj_RSSI) {
                g_netInfo.rssi = atoi(buf);
                g_netInfo.flag |= (1 << 3);
            }
        }

        break;
    case CMSG_UART3TX:
        sm_sendData(NULL);
        break;

    case CGET_CHAR:
        /** do nothng **/
        //(void)reportGetCharCmd();
        break;
    case CGETCHAR_STATUS:
        (void)checkAndAckGetCharConnectionStatus();
        break;
    
    case CGETCHAR_MOP:
        (void)checkAndAckGetCharWorkMode();
        break;
    case CGETCHAR_ROLLER:
        (void)checkAndAckGetCharRollerStatus();
        break;
    case CGETCHAR_CLEARWATER:
        (void)checkAndAckGetCharClearWaterStatus();
        break;
    case CGETCHAR_PUMP:
        (void)checkAndAckGetCharPumpStatus();
        break;
    case CGETCHAR_BATTERY:
        (void)checkAndAckGetCharBatteryStatus();
        break;
    case CGETCHAR_CHARGE:
        (void)checkAndAckGetCharChargeStatus();
        break;
    case CGETCHAR_NETINFO:
        checkAndAckGetCharNetInfo();
        break;
    case CGETCHAR_UPDATE:
        checkAndAckGetCharUpdate();
        break;
    case CPUT_SYNC:
        reportAckPutSync();
        break;
        
    case CPUT_CHAR:
			// (void)JsonParse(NULL, NULL);
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
        g_sm_wifiInitStatus = sm_wifiInitStep1;
        (void)reportService(0);
        break;
        
    case CREPORT_RSPOK:
        /** do nothing **/
        g_sm_wifiInitStatus = sm_wifiInitStep2;
        #if 0  //?????????????????? for test only
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
    case CDISCONN_CLOUD:
        g_sm_wifiInitStatus = sm_wifiDisconnected;
        break;
        
    // case CWIFI_STATUS:
    case CCONN_CLOUD:
        g_sm_wifiInitStatus = sm_wifiConnected;
        /** do what ? **/
        break;
        
    case CCONN_ROUTE:   /** connect route then get net info  **/ 
        fstack_init(&g_fstack);
        func.func = f_getNetInfo;
        fstack_push(&g_fstack, &func);
        
        msg.msgType = CMSG_INIT;
        msgq_in_irq(&g_msgq, &msg);
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
         #if 0  //?????????????????? for test only
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
         #if 0  //?????????????????? for test only
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
             #if 0
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }    
             #endif
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
        
    case CCMPNT_STATUS:
        // checkAndReportComponentStatus();
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

/** nothing to do with old key_scan **/
void key_scan4setwifi(void)
{
	msg_t msg;
    static u8 tmr_key = 0;
    static u8 key_last = 0xff;

    if (KEY1 == Bit_SET) {
        if (!(key_last & (1 << 1))) {
            tmr_key++;
            if (tmr_key >= 2) { // 去抖动时间
                msg.msgType = CMSG_UKEY;
                msgq_in(&g_msgq, &msg);
                
                tmr_key = 0;
                key_last |= (1 << 1);
            }
        } else {
            tmr_key = 0;
        }
    } else { /**KEY1 == Bit_RESET **/
        if ((key_last & (1 << 1))) {
            tmr_key++;
            if (tmr_key >= 2) { // 去抖动时间
                msg.msgType = CMSG_DKEY;
                msgq_in(&g_msgq, &msg);
                // MGPIO_Blink();  // ????????????????????
                tmr_key = 0;
                key_last &= (~(1 << 1));
            }
        } else {
            tmr_key = 0;
        }
    }
}
/*************************************************************************************/
int checkConnectionStatusChange(u8* status)
{
    u8 static connection_last = 0;
    /** mechine connected or not **/
    if (IDs_.Equipment == f_DragLala) {
        *status = CINDEX_CONNECTED;
    } else {
        *status = CINDEX_DISCONNECTED;
    }

    if (connection_last != *status) {
        connection_last = *status;
        return TRUE; 
    }
    return FALSE; 
}
/** report the status when status changed **/
void checkAndReportConnectionStatus(void)
{
    u8 status = 0;
    if (checkConnectionStatusChange(&status) == TRUE) {
        (void)reportComponentStatus(status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharConnectionStatus(void)
{
    u8 status = 0;
    (void)checkConnectionStatusChange(&status);
    (void)getCharAckComponentStatus(status);   
}
/********************* connection status **********************************************************/
int checkWorkModeChange(u8* status)
{
    u8 static mode_last = 0;
    u8 mode = sysvar.Modes;

    if (status == NULL) {
        return FALSE;
    }
    
    /** mechine disconnected **/
    if (IDs_.Equipment != f_DragLala) {
        if (mode_last != mode) {
            mode_last = mode;
            *status = CINDEX_UNKNOW;
            return TRUE;
        }
    }

    /** runing or stoped **/
    if(sysvar.sysfang & OFF_ON) {
        if (mode_last != mode) {
            mode_last = mode;
            *status = getIdxbyMode(mode);
            return TRUE;
        }
    } else {
        if (mode_last != mode) {
            mode_last = mode;
            *status = CINDEX_STANDBY;
            return TRUE;
        }
    }
    return FALSE;
}

/** report the status when status changed **/
void checkAndReportWorkMode(void)
{
    u8 status = 0;
    if (checkWorkModeChange(&status) == TRUE) {
        (void)reportComponentStatus(status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharWorkMode(void)
{
    u8 status = 0;
    (void)checkWorkModeChange(&status);
    (void)getCharAckComponentStatus(status);   
}
/*********************roller***********************************************************************/
int checkRollerStatusChange(u8* status)
{
    u8 static roller_status_last = 0;
    u8 roller_status = 0;
    
    if (status == NULL ) { /** something wrong**/
        return FALSE;
    }
    *status = roller_status_last;
    if (IDs_.Equipment != f_DragLala) {
        return FALSE;
    }
    /** 发生故障后，会立即停机
    if(!(sysvar.sysfang & OFF_ON)) {
        return FALSE;
    }
    **/

    if (sysvar.sysfang & MOTO_ERR_1) {
        *status = CINDEX_ROLLEROVERLOAD;
    } else {  // normal
        *status = CINDEX_ROLLERNORMAL;
    }
    
    if (roller_status_last != *status) {
        roller_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportRollerStatus(void)
{
    u8 roller_status = 0;

    if (checkRollerStatusChange(&roller_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(roller_status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharRollerStatus(void)
{
    u8 roller_status = 0;

    (void)checkRollerStatusChange(&roller_status);
    (void)getCharAckComponentStatus(roller_status);
}

/************************** pump *****************************************************************/
/**
 * pump fault occured, then the next action done and IdSensor_fang(IdSensor_PUMP|IdSensor_PUMPERR) status lost
 * so get the status in IDs_Judge() specially and trans
 **/
int checkPumpStatusChange(u8* status)
{
    u8 static pump_status_last = CINDEX_PUMPNORMAL;

    if (status == NULL) {
        return FALSE;
    }
    *status = pump_status_last;
    if (IDs_.Equipment != f_DragLala) {
        return FALSE;
    }
    /** 发生故障后，会立即停机
    if(!(sysvar.sysfang & OFF_ON)) {
        return FALSE;
    }
    **/

    /** 原双引号逻辑不保存水泵状态! 改为 IDs_Judge()中取状态; 再传入此函数进行处理
    if(IdSensor_fang(IdSensor_PUMP|IdSensor_PUMPERR)) {
        *status = CINDEX_PUMPCURRENTSMALL;
    } else {
        *status = CINDEX_PUMPNORMAL;
    }
    **/


    if (pump_status_last != *status) {
        pump_status_last = *status;
        /** report **/
        return TRUE;
    }
    
    return FALSE;

}

/**
 * the g_componentStatus.pump state get from IDs_Judge() only
 **/
void checkAndReportPumpStatus(void)
{
    if (checkPumpStatusChange(&(g_componentStatus.pump)) == TRUE) {
        /** report **/
        (void)reportComponentStatus(g_componentStatus.pump);
    }

    /** after status upload, then recoverd pump status **/
    if(sysvar.sysfang & OFF_ON) {
        g_componentStatus.pump = CINDEX_PUMPNORMAL;
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharPumpStatus(void)
{
    u8 pump_status = 0;
    (void)checkPumpStatusChange(&g_componentStatus.pump);
    (void)getCharAckComponentStatus(g_componentStatus.pump);
}
/**************************************************************************************************/
int checkBatteryChange(u8* status)
{
    u8 static battery_status_last = 0;

    if (status == NULL) {
        return FALSE;
    }
    if(sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2)) {  // 
        *status = CINDEX_BATTERYLOW;
    } else {  // normal
        *status = CINDEX_BATTERYNORMAL;
    }
    
    if (battery_status_last != *status) {
        battery_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportBatteryStatus(void)
{
    u8 battery_status = 0;
    
    if (checkBatteryChange(&battery_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(battery_status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharBatteryStatus(void)
{
    u8 battery_status = 0;
    (void)checkBatteryChange(&battery_status);
    (void)getCharAckComponentStatus(battery_status);
}

/**************************************************************************************************/
int checkChargeChange(u8* status)
{
    u8 static charge_status_last = 0;

/**
 * note: this MACRO define must same as the macro in CHARGING.c
 **/
#define CH_ING					2
#define CH_over					3
#define CH_ERR					5
    if (status == NULL) {
        return FALSE;
    }
    if (sysvar.CH_state==CH_ING) {
        *status = CINDEX_CHARGING;
    } else if (sysvar.CH_state==CH_ERR) {
        *status = CINDEX_CHARGEFAULT;
    } else if (sysvar.CH_state==CH_over) {
        *status = CINDEX_CHARGECOMPLETE;
    } else {
        *status = CINDEX_UNCHARGED;
    }

    if (charge_status_last != *status) {
        charge_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportChargeStatus(void)
{
    u8 charge_status = 0;

    if (checkChargeChange(&charge_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(charge_status);
    } else {
        #if 0
            (void)reportBatteryLevel(sysvar.BAT_soc);
        #else
        /** 充电时，电量比率会超过 100%!  **/
        if (charge_status == CINDEX_UNCHARGED) {
            (void)reportBatteryLevel(sysvar.BAT_soc);
        } else {
            reportComponentStatus(charge_status);
        }
        #endif
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharChargeStatus(void)
{
    u8 charge_status = 0;
    (void)checkChargeChange(&charge_status);
    (void)getCharAckComponentStatus(charge_status);
}
/**************************************************************************************************/
/**
 * latest clearwater status; ...
 **/
int checClearWaterChange(u8* status)
{
    u8 static clear_status_last = CINDEX_CLEARWATERNORMAL;

    if (status == NULL) {
        return FALSE;
    }
    *status = clear_status_last;
    if (IDs_.Equipment != f_DragLala) {
        return FALSE;
    }
    if(!(sysvar.sysfang & OFF_ON)) {
        return FALSE;
    }

    if(IdSensor_fang(IdSensor_CLEAR)) {    //
        *status = CINDEX_CLEARWATERSHORTAGE;
    } else {
        *status = CINDEX_CLEARWATERNORMAL;
    }

    if (clear_status_last != *status) {
        clear_status_last = *status;
        /** report **/
        return TRUE; // !!!!!!!!!!!!!!
    }
    return FALSE;
}

void checkAndReportClearWaterStatus(void)
{
    u8 clear_status = 0;

    if (checClearWaterChange(&clear_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(clear_status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharClearWaterStatus(void)
{
    u8 clear_status = 0;
    (void)checClearWaterChange(&clear_status);
    (void)getCharAckComponentStatus(clear_status);
}
/**************************************************************************************************/
/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharNetInfo(void)
{
    reportgetCharNetInfo(&g_netInfo);
}

void netInfoData_init(void)
{
    g_netInfo.flag = 0;
    g_netInfo.rssi = 0;
    strcpy(g_netInfo.ssid, "ssid");
    strcpy(g_netInfo.ip, "0.0.0.0");
    strcpy(g_netInfo.mac, "0:0:0:0:0:0");
}

void checkAndAckGetCharUpdate(void)
{
    (void)getCharAckComponentStatus(CINDEX_UPDATE);
}

/**************************************************************************************************/
#if 0
void checkAndReportComponentStatus(void)
{
    static u8 fp_index = 0;
    paction_t_0 fp_tab[] = {
        checkAndReportWorkMode,
        checkAndReportRollerStatus,
        checkAndReportPumpStatus,
        checkAndReportBatteryStatus,
        checkAndReportChargeStatus,
        checkAndReportClearWaterStatus
    };

    if (!((g_sm_wifiInitStatus == sm_wifiInitComplete) || (g_sm_wifiInitStatus == sm_wifiInitStep2))) {
        return;
    }
    fp_index++;
    if (fp_index >= MTABSIZE(fp_tab)) {
        fp_index = 0;
    }
    fp_tab[fp_index]();
}
#endif

