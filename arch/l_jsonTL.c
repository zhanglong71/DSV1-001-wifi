#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include <string.h>
#include <stdio.h>
#include "l_jsonTL.h"
#include "main.h"

/**********************************************************************************************/
int doNothing(unsigned *arg)
{
    (void)arg;
    return 0;
}

int constructGetCharCmd(u8 status)
{
    //jsonTL_t* p = getDevInfo(0);
    //sm_sendData(p);
    return 0;
}

int reportDevInfo(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getDevInfo(0);
    sm_sendData(p);
    return (TRUE);
}

int reportHeartbeat(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getHeartbeat();
    sm_sendData(p);
    return (TRUE);
}

int reportService(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getService(0);
    sm_sendData(p);
    return (TRUE);
}

int reportResetNet(u8 idx)
{
    static jsonTL_t resetNetArr[] = {
        { "resetNet", 2,"AP"},   /** reset net and config net with AP **/
        { "resetNet", 3,"BLE"},  /** reset net and config net with BLE **/
    };
    if (idx >= MTABSIZE(resetNetArr)) {
        return (FALSE);
    }
    sm_sendData(&(resetNetArr[idx]));

    return (TRUE);
}

int reportScanWifi(void *arg)
{
    (void)arg;
    u8Data_t u8Data;
    u8 buf[] = "scanWifi,0\n";
    for (int i = 0; i < strlen(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
    }

    return (TRUE);
}

int reportConnectWifi(void *arg)
{
    (void)arg;
    jsonTL_t* p = getConnectWifi(0);
    sm_sendData(p);
    return (TRUE);
}

int reportBatteryLevel(u8 arg)
{
    jsonTL_t jsonTypeTx;
    u8 buf[64]; 
    u8 len = 0;
    u8Data_t u8Data;
    
    jsonTypeTx.jHead = "getChar";
    jsonTypeTx.jBody =buf;
    jsonTypeTx.jLen = strlen(buf);
    
    /** hhhhhhhhh head **/
    len = strlen(jsonTypeTx.jHead);
    for (int i = 0; i < len; i++) {
        u8Data.u8Val = jsonTypeTx.jHead[i];
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart3TxQue, &u8Data); 
    
    /** lllllllll length **/
    sprintf(buf, "{\"battery\":{\"level\":\"%u\"}}",  arg);
    len = strlen(buf);
    if (sprintf(buf, "%d", len)) {
        for (int i = 0; i < strlen(buf); i++) {
            u8Data.u8Val = buf[i];
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        }
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart3TxQue, &u8Data);
    
    /** bbbbbbbbb body **/
    sprintf(buf, "{\"battery\":{\"level\":\"%u\"}}",  arg);
    for (int i = 0; i < strlen(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
    }
    return (TRUE);
}

const static reportStatusBody_t reportStatusBodyArr[] = {
    #if 0
    { CINDEX_CHARGING,           "{\"mop\":{\"charge\":\"charging\"}}"},         // 正在充电
    { CINDEX_CHARGECOMPLETE,     "{\"mop\":{\"charge\":\"chargeComplete\"}}"},   // 充电完成
    { CINDEX_CHARGEFAULT,        "{\"mop\":{\"charge\":\"chargeFault\"}}"},      // 充电故障
    { CINDEX_LOWBATTERY,         "{\"mop\":{\"battery\":\"lowBattery\"}}"},      // 电池电压低
    { CINDEX_DORMANCY,           "{\"mop\":{\"status\":\"dormancy\"}}"},         // 休眠状态
    { CINDEX_SCREENSHUTDOWN,     "{\"mop\":{\"screen\":\"ScreenShutdown\"}}"},   // 熄屏
    { CINDEX_STANDBY,            "{\"mop\":{\"status\":\"standby\"}}"},                 // 待机
    { CINDEX_NEUTRAL,            "{\"mop\":{\"status\":\"neutral\"}}"},                 // 空档模式
    { CINDEX_SETUP,              "{\"mop\":{\"status\":\"setup\"}}"},                   // 设置模式 setup mode
    { CINDEX_SETUP2,             "{\"mop\":{\"status\":\"setup\"}}"},                   // 设置语音 setup voice prompt
    { CINDEX_SETUP3,             "{\"mop\":{\"status\":\"setup\"}}"},                   // wifi复位 wifiReset
            
    { CINDEX_CONNECTION,         "{\"mop\":{\"status\":\"connection\"}}"},              // 主机接驳
    { CINDEX_STANDARD,           "{\"mop\":{\"status\":\"standard\"}}"},                // 标准模式
    { CINDEX_HIGHPOWER,          "{\"mop\":{\"status\":\"highpower\"}}"},               // 强力模式
    { CINDEX_CLEANING,           "{\"mop\":{\"status\":\"cleaning\"}}"},                // 自清洗模式
    { CINDEX_RINSE,              "{\"mop\":{\"status\":\"rinse\"}}"},                   // 大水冲洗模式
    { CINDEX_TANKINPLACE,        "{\"mop\":{\"tank\":\"tankInPlace\"}}"},               // 水箱在位识别
    { CINDEX_TANKNOTINPLACE,     "{\"mop\":{\"tank\":\"tankNotInPlace\"}}"},            // 水箱不在位识别
    { CINDEX_FULLSEWAGE,         "{\"mop\":{\"sewage\":\"fullSewage\"}}"},              // 污水满
    { CINDEX_INSUFFICIENTWATER,  "{\"mop\":{\"clear\":\"InsufficientWater\"}}"},        // 没有清水
    { CINDEX_PUMPOVERLOAD,       "{\"mop\":{\"pump\":\"pumpOverLoad\"}}"},              // 水泵过载     pumpOverload
    { CINDEX_PUMPCURRENTSMALL,   "{\"mop\":{\"pump\":\"pumpCurrentSmall\"}}"},          // 水泵电流过小 pumpCurrentToosmall
    { CINDEX_MOTOROVERLOAD,      "{\"mop\":{\"roller\":\"rollerError\"}}"},                // 电机故障
    #else
    { CINDEX_UNKNOW,              "{\"mop\":{\"status\":0}}"},             // unknow 未知状态(主机断开及其它)
    { CINDEX_STANDBY,             "{\"mop\":{\"status\":1}}"},             // standby 待机
    { CINDEX_STANDARD,            "{\"mop\":{\"status\":2}}"},                   // standard 标准模式
    { CINDEX_HIGHPOWER,           "{\"mop\":{\"status\":3}}"},                   // highPower强力模式
    { CINDEX_RINSE,               "{\"mop\":{\"status\":4}}"},                   // rinse 大水冲洗模式
    { CINDEX_CLEANING,            "{\"mop\":{\"status\":5}}"},                   // cleaning 自清洗模式
    
    { CINDEX_ROLLERNORMAL,        "{\"roller\":{\"status\":1}}"},                // normal 滚筒电机正常
    { CINDEX_ROLLEROVERLOAD,      "{\"roller\":{\"status\":2}}"},                // error 滚筒电机故障
    
    { CINDEX_CLEARWATERNORMAL,    "{\"clearWater\":{\"status\":1}}"},             // clear water normal 清水正常
    { CINDEX_CLEARWATERSHORTAGE,  "{\"clearWater\":{\"status\":2}}"},             // clear water shortage 清水不足
    
    { CINDEX_PUMPNORMAL,          "{\"pump\":{\"status\":1}}"},                  // 水泵正常     pumpNormal
    { CINDEX_PUMPOVERLOAD,        "{\"pump\":{\"status\":2}}"},                  // 水泵过载     pumpOverload
    { CINDEX_PUMPCURRENTSMALL,    "{\"pump\":{\"status\":3}}"},                  // 水泵电流过小 pumpCurrentTooSmall
    
    { CINDEX_BATTERYNORMAL,       "{\"battery\":{\"status\":1}}"},      // 电池电压在正常范围
    { CINDEX_BATTERYLOW,          "{\"battery\":{\"status\":2}}"},      // 电池电压过低
    
    { CINDEX_UNCHARGED,           "{\"charge\":{\"status\":1}}"},         // 没充电
    { CINDEX_CHARGING,            "{\"charge\":{\"status\":2}}"},         // 正在充电
    { CINDEX_CHARGECOMPLETE,      "{\"charge\":{\"status\":3}}"},         // 充电完成(区别不充电场景)
    { CINDEX_CHARGEFAULT,         "{\"charge\":{\"status\":4}}"},         // 充电故障
    #endif
};

static u8 getIdxbyMode(u8 mode)
{
    const static pair_u8u8_t statusNum2IdxArr[] = {
        {0, CINDEX_UNKNOW},
        {MODE_1, CINDEX_STANDARD},
        {MODE_2, CINDEX_HIGHPOWER},
        {MODE_3, CINDEX_HIGHPOWER},
        {MODE_RINSE, CINDEX_RINSE},
        {MODE_CLEANING, CINDEX_CLEANING},
    };

    for (int i = 0; i < MTABSIZE(statusNum2IdxArr); i++) {
        if ((statusNum2IdxArr[i].first) == mode) {
            return statusNum2IdxArr[i].second;
        }
    }
    return CINDEX_INVALID;
}

/**
 * response getChar
 * mode ==> index ==> json info(jhead/jBody/jLen)
 **/
jsonTL_t* getGetCharCmdbyMode(u8 mode)
{
    static jsonTL_t jsonTypeTx;
    
    u8 idx = getIdxbyMode(mode); 
    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (NULL);
    }

    jsonTypeTx.jHead = "getChar";
    jsonTypeTx.jBody = reportStatusBodyArr[idx].body;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
    
	return (&jsonTypeTx);
}

/** the current work status **/
int reportGetCharCmd(void)
{
    jsonTL_t* p = getGetCharCmdbyMode(sysvar.Modes);
    sm_sendData(p);
    return 0;
}

/**
 * ?ϱ????? 
 * ?????? getChar/reportChar
 **/
#if 1
jsonTL_t* getReportCmdbyMode(u8 mode)
{
    static jsonTL_t jsonTypeTx;
    u8 statusIndex = getIdxbyMode(mode); 
    u8 idx = 0;
    
    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == statusIndex) {
            break;
        }
    }
    
    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (NULL);
    }

    jsonTypeTx.jHead = "reportChar";
    jsonTypeTx.jBody = reportStatusBodyArr[idx].body;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
    
	return (&jsonTypeTx);
}

int reportReportCharCmd(void)
{
    jsonTL_t* p = getReportCmdbyMode(sysvar.Modes);
    sm_sendData(p);
    return 0;
}

#endif
/***********************************************************************
 * moto/pump/battery/clear/charge
 ***********************************************************************/
int reportComponentStatus(u8 statusIndex)
{
    static jsonTL_t jsonTypeTx;
    u8 idx = 0;

    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == statusIndex) {
            break;
        }
    }

    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return 0;
    }

    jsonTypeTx.jHead = "reportChar";
    jsonTypeTx.jBody = reportStatusBodyArr[statusIndex].body;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
    
    sm_sendData(&jsonTypeTx);
    return 0;
}

/*********************************************************************/
jsonTL_t* getDevInfo(u8 idx)
{
    static jsonTL_t jsonTypeDevInfo[] = {
        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"AP\","
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            // "\"marketName\": \"多功能水尘洗地机\","
            "\"marketName\": \"DIISEA-DM6\","
            "\"brand\": \"c-chip\"}"
        },

        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"BLE\","
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\": \"多功能水尘洗地机\","
            "\"brand\": \"c-chip\"}"
        },

        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"BLE\","
            "\"near\": {"
                "\"type\": 1,"
                "\"trp\": -8"
            "},"
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\": \"多功能水尘洗地机\","
            "\"brand\": \"c-chip\"}"
        }
    };

    if (idx >= MTABSIZE(jsonTypeDevInfo)) {
        return (NULL);
    }
	return (&jsonTypeDevInfo[idx]);
}

jsonTL_t* getService(u8 idx)
{
    static jsonTL_t ServiceArr[] = {
        {
            "reportService", 0,
            "{"
                "\"sId\": ["
                    "\"mop\","
                    "\"roller\","
                    "\"clearWater\","
                    "\"pump\","
                    "\"battery\","
                    "\"charge\""
                    "],"
                "\"sType\": ["
                    "\"mop\","
                    "\"roller\","
                    "\"clearWater\","
                    "\"pump\","
                    "\"battery\","
                    "\"charge\""
                "]"
            "}"
        }   
       // {"reportService", 0, ""}
    };

    if (idx >= MTABSIZE(ServiceArr)) {
        return (NULL);
    }
    return (&ServiceArr[idx]);
}

jsonTL_t* getHeartbeat(void)
{
    static jsonTL_t heartbeat[] = {
        {"\"heartbeat\"",0,""}
    };
    return (&heartbeat[0]);
}

jsonTL_t* getConnectWifi(u8 idx)
{
    static jsonTL_t ConnectWifiArr[] = {
        {
            "connectWifi", 0,
            "{"
            "\"ssid\":\"hilink_production_test\","
            "\"password\":\"12345678\""
            "}"
        }
    };
    if (idx >= MTABSIZE(ConnectWifiArr)) {
        return (NULL);
    }

    return (&ConnectWifiArr[idx]);
}


/**
 * state machine
 *
 * sm_init =(jp!=NULL)=> sm_step =(p!=NULL)=> sm_end ==>sm_init
 * 
 * sm_init: send data include head+len+head of body(< 40 byte)
 * sm_step: send data body(< 60 byte) again and again(CMSG_UART3TX)
 * sm_end: send over
 *
 * note: send data the length of g_uart3TxQue buf
 **/
void sm_sendData(jsonTL_t* jp)
{
    static smStatus_t s_smStatus = sm_init;
    static jsonTL_t* p = NULL;
    static u16 total = 0;
    u8Data_t u8Data;
    u8 buf[8];
    int len = 0;

    if (s_smStatus == sm_init) {   /** first enter the send process **/
        if (jp == NULL) {
            return;
        }
        s_smStatus = sm_step;
        p = jp;
        total = 0;
        
        /** hhhhhhhhh head **/
        len = strlen(p->jHead);
        for (int i = 0; i < len; i++) {
            u8Data.u8Val = p->jHead[i];
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        }
        u8Data.u8Val = ',';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data); 
        
        /** lllllllll length **/
        len = strlen(p->jBody);
        if (sprintf(buf, "%d", len)) {
            for (int i = 0; i < strlen(buf); i++) {
                u8Data.u8Val = buf[i];
                u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            }
        }
        
        if (len > 0) {
            u8Data.u8Val = ',';
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        } else { /** len == 0, nobody need transmit **/
            u8Data.u8Val = '\n';
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            s_smStatus = sm_init;
            return;
        }
        /** bbbbbbbbb body **/
        for (int i = 0; ((i < len) && (i < (Mu8FIFO_bufLen(&g_uart3TxQue) - 32))); i++, total++) {
            u8Data.u8Val = p->jBody[i];
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        }
    } else if (s_smStatus == sm_step) {   /**  enter the send process again **/
        /** body **/
        if (p == NULL) {
            s_smStatus = sm_end;
            return; 
        }
        len = strlen(p->jBody);
        for (int i = 0; ((total < len) && (i < Mu8FIFO_bufLen(&g_uart3TxQue))); i++, total++) {
            u8Data.u8Val = p->jBody[total];
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        }

        if (total >= len) {               /** the last transmit part **/
            s_smStatus = sm_end;
        }
    } else if (s_smStatus == sm_end) {   /** transmit over **/
        s_smStatus = sm_init;
        p = NULL;
    } else { /** unrecognize **/
        s_smStatus = sm_init;
        p = NULL;
    }
}

#if 0
void reportTest(void)
{
    u8 buf[] = {0xA5, 0x5A, 0x01, 0x13, 0x00, 0x05, 0x00, 0xA, 0x00, 0x01, 0x00, 0x23}; /** C8138 module test req(12-bytes) **/

    u8Data_t u8Data;
    for (int i = 0; i < MTABSIZE(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
    }

}

int checkResponseTest(u8* str)
{
	  int i = 0;
    u8 buf[] = {0x5A, 0xA5, 0x01, 0x10, 0x00, 0x06, 0x00, 0xA, 0x00, 0x02, 0x00, 0x00}; /** C8138 module test response(12-bytes) **/
    for (i = 0; i < MTABSIZE(buf); i++) {
        if (buf[i] != str[i]) {
            break;
        }
    }

    if (i >= MTABSIZE(buf)) {
        return TRUE;
    }

    return FALSE;
}
#endif

/********************************
 * enrolled key 
 ********************************/
const jsonTL_t commandKeyArr[] = {
    {"getDevInfo", 0, NULL, NULL},
    {"heartbeat", 0, NULL, NULL},
    {"putChar", 0, NULL, NULL},      /** 命令下发！长度不定 **/
    {"getChar", 0, NULL, NULL},      /** 查询单个服务状态！ 长度不定  **/
    {"reportService,", 0, NULL, NULL},      /** 查询单个服务状态！ 长度不定  **/
    {"scanWifi", 0, NULL, NULL},        /** production test ack of scanwifi **/
    {"connectWifi", 0, NULL, NULL},     /** production test ack of connect action **/
    {"getRssi", 0, NULL, NULL},         /** production test ack of getRssi action **/
    {"putWifiStatus", 0, NULL, NULL},   /** the command description !!! **/
    {"getWifiStatus", 0, NULL, NULL},   /** the command description !!! **/
    {"resetNet", 0, NULL, NULL},        /** reset net and configure net !!! **/
    // {"\xa5\x5a\x01\x10\x00\x06\x00\x0A\x00\x02", 0, NULL, NULL},
    // {"\"getDevInfo\"", 0},   /**  **/
    // {"\"heartbeat\"", 0},    /** 下发心跳！长度为0 **/

#define CTestWIFIkeyIdx (MTABSIZE(commandKeyArr))
};

u8 getCommandKeyArrLen(void)
{
    return MTABSIZE(commandKeyArr);
}

jsonTL_t* getCommandKey(u8 idx)
{ 
    if (idx >= MTABSIZE(commandKeyArr)) {
        return (NULL);
    }
	return (&commandKeyArr[idx]);
}

/**
 * "key",length,"{body}"
 * if receive key/length/body, then return true!
 **/
objType_t sm_receiveData(u8 *data)
{
    static smStatus_t s_smStatus = sm_init;
    static u16 s_bodyLen = 0;
    static u8 s_keyIdx = 0;
    static u8 offset = 0;
    u8Data_t u8Data;
    msg_t msg;
    u8 chData;
    u8 i;

    if(u8FIFOisEmpty(&g_uart3RxQue) == TRUE) { /** no data !!! **/
        return obj_none;
    }
    (void)u8FIFO_last(&g_uart3RxQue, &chData);

    if (s_smStatus == sm_init) {   /** identifing key **/
        if (chData != ',') {
            offset = 0;
            return obj_none;
        }
        if (u8FIFO_get(&g_uart3RxQue, 0, data) != TRUE) {
            return obj_none;
        }

        for(i = 0; i < getCommandKeyArrLen(); i++) {
            if (strstr(data, getCommandKey(i)->jHead) != NULL) {   /** key **/
                break;
            }
        }

        if(i < getCommandKeyArrLen()) {
            s_keyIdx = i;
            s_bodyLen = 0 ;
            
            /** 
             * ������key����ʶ�𣬲���Ҫ sm_receiveLenStep1 ״̬
             * �� sm_receiveLenStep2 ״̬ʶ�𳤶�
             **/
            s_smStatus = sm_receiveLenStep2;
            return obj_key;
        }else {
            #if 0
            /** !!! wifi module working verify!!! **/
            if (checkResponseTest(data)) {
                s_smStatus = sm_receiveBody;
                s_bodyLen = 2;
                s_keyIdx = CTestWIFIkeyIdx;   /** ȡһ����Чֵ ���⴦�� **/
            }
            #endif
        }
    } else if (s_smStatus == sm_receiveLenStep2) {    /** identifing length  **/
        if ((chData == ' ') || (chData == '\t')) { // ignore the blank and tab
            return obj_none;
        } else if (isdigit(chData)) {   // the valid number
            s_bodyLen = (s_bodyLen * 10) + (chData - '0');
        } else if (chData == ',') {
            sprintf(data, "%d", s_bodyLen);
            s_smStatus = sm_receiveBody;
            offset = u8FIFOlength(&g_uart3RxQue);
            return obj_len;
        } else if (chData == '\n') {
            u8FIFOinit_irq(&g_uart3RxQue);
            s_smStatus = sm_init;   // over
            
            if (MisDevinfo(s_keyIdx) == TRUE) {
                /** get devinfo **/
                msg.msgType = CGETDEVINFO_REQ;
                msgq_in_irq(&g_msgq, &msg);
                
                return obj_none;
            } else if (MisHeartbeatCommand(s_keyIdx) == TRUE) {
                msg.msgType = CHEART_BEAT;
                msgq_in_irq(&g_msgq, &msg);
                
                return obj_none;
            } else {
                /** end transmit but invalid data **/
            }
        }
        return obj_none;
    } else if (s_smStatus == sm_receiveBody) {    /** identifing body **/
        // ??????????????????????????????
        #if 0
            u8Data.u8Val = '!';
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            u8Data.u8Val = chData;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        // ??????????????????????????????
        /** offset: start of head, end of len **/
        if (u8FIFO_get(&g_uart3RxQue, offset, data) != TRUE) {
            return obj_none;
        }
        if (chData == '\n') {
            ClrTimer_irq(&g_timer[1]);
            u8FIFOinit_irq(&g_uart3RxQue);
            s_smStatus = sm_init;   // over 
           
            if (MisDevinfoRespOk(s_keyIdx, s_bodyLen, data)) {
                /** response devinfo **/
                msg.msgType = CGETDEVINFO_RSPOK;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if (MisDevinfoRespErr(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETDEVINFO_RSPERROR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisReportSericeRespOk(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CREPORT_RSPOK;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisReportSericeRespErr(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CREPORT_RSPERROR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharMopStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGET_CHAR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisPutChar(s_keyIdx)) {
                msg.msgType = CPUT_CHAR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisgetWifiStatus(s_keyIdx)) {
                msg.msgType = CWIFI_STATUS;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisScanWifi(s_keyIdx)) {
                /** need one global variety store the rssi **/
                msg.msgValue = s_bodyLen;
                msg.msgType = CSCAN_WIFI;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisconnectWifi(s_keyIdx)) {
                /** need one global variety store the connection status **/
                msg.msgType = CCONN_WIFI;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisResetNet(s_keyIdx)) {
                if (MisRespOk(s_bodyLen, data)) {
                    msg.msgType = CRESETNET_RSPOK;
                    msgq_in_irq(&g_msgq, &msg);
                    return obj_none;
                } else if (MisRespFail(s_bodyLen, data)) {
                    msg.msgType = CRESETNET_RSPFAIL;
                    msgq_in_irq(&g_msgq, &msg);
                    return obj_none;
                }
            }
            return obj_body;
        }

        if (u8FIFOisFull(&g_uart3RxQue) == TRUE) {
            /** warning...  buf full **/
            u8Data.u8Val = 'F';
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        } else {
            // ??????????????????????????????
            #if 0
            u8Data.u8Val = '!';
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            u8Data.u8Val = chData;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            u8Data.u8Val = g_uart3RxQue.in;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            u8Data.u8Val = g_uart3RxQue.out;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
            #endif
           // ??????????????????????????????
        }
        if (u8FIFOlength(&g_uart3RxQue) >= (s_bodyLen + offset + 1)      /** strlen(",x,") + the "" end of body + the \n end of body **/) {
            u8FIFOinit_irq(&g_uart3RxQue);
            s_smStatus = sm_init;   // over 
            return obj_body;
        }
    } else if  (s_smStatus == sm_end) {           /** identifing end **/
        s_smStatus = sm_init;
    } else {
        /** !!! **/
    }

    return obj_none;
}

int sm_receiveDataLen(const u8FIFO_t *que, u8 *data)
{
    
}

void CmdProcess(u8* CommId)
{
    for(int i = 0; i < MTABSIZE(commandKeyArr); i++) {
        //if (strstr(jsonTypeArr[i].jsonKey, CommId)) {
			
        //}
    }
}

