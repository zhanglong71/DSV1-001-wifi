#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include "json_utils.h"

#if 0
void testGenerateJson(void)
{
    u8 i_u8;
    for(i_u8=0;i_u8<5;i_u8++)
    {
        jsonstcarr[i_u8].jIndexN = 5-i_u8;
        strcpy(jsonstcarr[i_u8].jName,"moto1");
        sprintf(jsonstcarr[i_u8].jValue,"%u",2000+i_u8);
        //stpcpy(jsonstcarr[i_u8].jValue,"2000");
    }
}

void testPrintJson(void)
{
    u8 j_u8;
    for(j_u8=0;j_u8<jsonstcarr[0].jIndexN;j_u8++)
    {
        printf(jsonstcarr[j_u8].jName);
        printf("\n");
        printf(jsonstcarr[j_u8].jValue);
        printf("\n");
        printf("length:%d\n",jsonstcarr[j_u8].jIndexN);
    }
}

unsigned char JsonGenerateUtil(unsigned char* jsonstrbuf)
{
    u8 i_u8;

    if(jsonstcarr[0].jIndexN <= 0)
    {
        return(0);
    }
    strcpy(jsonstrbuf, "{");
    for(i_u8=0; i_u8 < jsonstcarr[0].jIndexN; i_u8++)
    {
       strcat(jsonstrbuf, "\"");
       strcat(jsonstrbuf, jsonstcarr[i_u8].jName);
       strcat(jsonstrbuf, "\":");
       strcat(jsonstrbuf, jsonstcarr[i_u8].jValue);
       if(i_u8 < jsonstcarr[0].jIndexN - 1)strcat(jsonstrbuf, ",");
    }
    strcat(jsonstrbuf, "}");
    return (1);
}
#endif

/**
 * remove the first and last blank character
 **/
unsigned char* strim(char* str) 
{
    char *end, *sp, *ep;
    int len;
    
    sp = str;
    end = str + strlen(str) - 1;
    ep = end;
    
    while((sp <= end) && isspace(*sp)) {   /** head blank **/
        sp++;
    }
    while((ep >= sp) && isspace(*ep)) {    /** tail blank **/
        ep--;
    }
    len = ((ep < sp) ? 0:((ep - sp) + 1));
    sp[len] = '\0';
    return sp;
}

/**
 * check if the str include 'first' and 'last'; and &first < &last
 * example:
 * {k1:v1, ..., kn:vn}
 **/
unsigned char isCoupledL0(char* str, unsigned char fch, unsigned char lch) 
{
    unsigned char* fp = NULL;
    unsigned char* lp = NULL;
    
    if((fp = strchr(str, fch)) == NULL) {
        return (0);
    }

    if((lp = strchr(str, lch)) == NULL) {
        return (0);
    }
    if(lp < fp) {
        return (0);
    }

    return (1);
}

/**
 * 找第一个'first ch'配对的'last ch', 返回'last ch'的地址
 * 
 **/
unsigned char* isCoupledLn(char* str, unsigned char fch, unsigned char lch) 
{
    #define CTABSIZE_TMP (16)
    unsigned char fCnt = 0;
    unsigned char lCnt = 0;
    unsigned char count = 0;
    unsigned char* p = NULL;
    unsigned char* fp[CTABSIZE_TMP];
    unsigned char* lp[CTABSIZE_TMP];
    
    /** 取出'{'和'}'的地址! 作为序号的先后 **/
    memset(fp, 0, (sizeof(unsigned char*) * CTABSIZE_TMP));
    memset(lp, 0, (sizeof(unsigned char*) * CTABSIZE_TMP));
    p = str;
    while((fCnt < CTABSIZE_TMP) && ((fp[fCnt] = strchr(p, fch)) != NULL)) {
        p = fp[fCnt] + 1;
        fCnt++;
    }
    p = str;
    while((lCnt < CTABSIZE_TMP) && ((lp[lCnt] = strchr(p, lch)) != NULL)) {
        p = lp[lCnt] + 1;
        lCnt++;
    }
    
    if(lCnt != fCnt) {   /** not coupled**/
        return (0);
    }
    count = lCnt;
    
    for(lCnt = 0; lCnt < count; lCnt++) {
        for(fCnt = count; fCnt < count; fCnt--) {
            if((fp[fCnt] == 0) || (lp[lCnt] == 0)) { /** 跳过无效或已经成功配对过的 **/
                continue;
            }
            
            if(lp[lCnt] > fp[fCnt]) /** 配对成功 **/
            {
                if(fCnt == 0) {    /** 第一个 **/
                    return lp[lCnt];
                }
                fp[fCnt] = 0; /** 配对成功 **/
                lp[lCnt] = 0;
                break;
            }
        }
        
    }
    #undef CTABSIZE_TMP
    return (0);
}

/**
 * check if the number of first equal to the number of last
 **/
unsigned char isCoupled(char* str, unsigned char first, unsigned char last) 
{
    unsigned char fCnt = 0;
    unsigned char lCnt = 0;
    unsigned char* p = NULL;
    unsigned char* pnext = NULL;

    p = str;
    while((pnext = strchr(p, first)) != NULL) {
        fCnt++;
        p = pnext + 1;
    }
    p = str;
    while((pnext = strchr(p, last)) != NULL) {
        lCnt++;
        p = pnext + 1;
    }
    
    return (lCnt == fCnt);
}

/**
 * find the paddr of first and last
 * if lCnt = fCnt, then body coupled [!!!]
 * if fp[i + 1] > lp[i], side by side
 * if fp[i + 1] < lp[i], nested
 * if fp[i + 1] = lp[i], error
 **/
unsigned char* findCoupled(char* str, unsigned char first, unsigned char last) 
{
    #define CTABSIZE_TMP (16)
    unsigned char fCnt = 0;
    unsigned char lCnt = 0;
    unsigned char count = 0;
    unsigned char* p = NULL;
    unsigned char* pnext = NULL;
    unsigned char* fp[CTABSIZE_TMP];
    unsigned char* lp[CTABSIZE_TMP];
    
    p = str;
    while((fp[fCnt] = strchr(p, first)) != NULL) {
        p = fp[fCnt] + 1;
        fCnt++;
    }
    
    p = str;
    while((lp[lCnt] = strchr(p, last)) != NULL) {
        p = fp[lCnt] + 1;
        lCnt++;
    }
    for(int i = 0; i <= CTABSIZE_TMP - 1; i++) {
        fp[i + 1] > lp[i];
    }
    #undef CTABSIZE_TMP
    return (lCnt == fCnt);
}

/**可能收到的命令 **/
jsonTL_t commandHeadArr[] = {
    {"putChar", 0},      /** 鍛戒护涓嬪彂锛侀暱搴︿笉瀹?**/
    {"getDevInfo", 0},   /** WIFI 鏌ラ厤缃?len = 0)锛涘強鍥炲(len = 2銆?)銆佹纭洖澶?**/
    {"getChar", 0},      /** 鏌ヨ鍗曚釜鏈嶅姟鐘舵�侊紒 闀垮害涓嶅畾  **/
    {"heartbeat", 0},    /** 涓嬪彂蹇冭烦锛侀暱搴︿负0 **/
    {NULL, 0}
};

/**
 * prase json body(JsonParseL0)
 * 
 * example:
 * {key1:value1, key2:value2, ... ,keyn:valuen}
 **/
unsigned char JsonParse_L0(unsigned char* jsonstrbuf, JsonArr* jsonstcarr)
{
    u8 j_u8,k_u8,i_u8 = 0;
    char *p[12];
    char *pChar = jsonstrbuf;
    jsonstcarr[0].jIndexN = 0;
    if((jsonstrbuf[0] != '{')||(jsonstrbuf[strlen(jsonstrbuf) - 1] != '}')) return(0);
    
    jsonstrbuf[strlen(jsonstrbuf) - 1] = '\0';
    for(j_u8 = 0; j_u8 < strlen(jsonstrbuf); j_u8++) {
        jsonstrbuf[j_u8] = jsonstrbuf[j_u8 + 1];
    }
    
    while((p[i_u8]  = strtok(pChar, ":,")) != NULL)
    {
        i_u8++;
        pChar = NULL;
    }
    for(j_u8 = 0; j_u8 < i_u8/2; j_u8++)
    {
        jsonstcarr[j_u8].jIndexN = i_u8/2 - j_u8;
        strcpy(jsonstcarr[j_u8].jName, p[j_u8 * 2]);
        strcpy(jsonstcarr[j_u8].jValue, p[j_u8 * 2 + 1]);

        jsonstcarr[j_u8].jName[strlen(jsonstcarr[j_u8].jName) - 1] = '\0';
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].jName); k_u8++)
        {
            jsonstcarr[j_u8].jName[k_u8] = jsonstcarr[j_u8].jName[k_u8 + 1];
        }
    }
    return (1);
}

/**
 * 单条命令json报文处理入口
 * 
 * prase json body(JsonParseL1E0)
 * 
 * example:
 * { "switch": {key1:value1, key2:value2, ... ,keyn:valuen} }
 * example:
 * {
 *   "switch": {
 *       "on": 40,
 *       "target": 65
 *   }
 * }
 *
 * 1.command first
 * 2.then ":"
 * 3.next json body L0
 **/
 
 #if 1
unsigned char JsonParseL1E0(unsigned char* str, JsonArr* jsonstcarr)
{
    unsigned char* p = NULL;
    unsigned char* sid = NULL;
    int i;

    for(i = 0; i < MTABSIZE(commandHeadArr); i++) {
        if(strstr(str, commandHeadArr[i].jHead) != NULL) {   /** command **/
            break; 
        }
    }
    if(i >= MTABSIZE(commandHeadArr)) {
        return NULL;   
    }
    sid = commandHeadArr[i].jHead;

    p = strchr(str, ':');
	if(p == NULL) {
        return NULL;
    }
    p++;
    return JsonParse_L0(p, jsonstcarr);
}

/**
 * prase json body(JsonParseL1Batch)
 * 
 * example:
 * { 
 *  "sid": "switch",
 *  "data": {"key1":value1, "key2":value2, ... , "keyn":valuen} 
 * }
 *
 * 1.sid first
 * 2.then "," as end if the sid
 * 3.data next
 * 4.then "..L0" 
 *
 * 此实现方法有待改进
 **/
unsigned char JsonParseL1BatchItem(unsigned char* str, JsonArr* jsonstcarr)
{
    #define CTABSIZE_TMPKV (128)
    #define CTABSIZE_TMPSTR (6)
    unsigned char* p[CTABSIZE_TMPSTR];
    unsigned char kv[CTABSIZE_TMPKV];
    unsigned char* sp = NULL;
    unsigned char* lp = NULL;
    unsigned char* sid = NULL;
    unsigned char i = 0;
    
    /** sid **/
    if(strstr(str, "sid") != NULL) {
        return 0;
    }
    
    if((sp = strchr(str, '"')) != NULL) { /** 这个应该是sid前的**/
        return 0;
    }
    
    /** get "sid":"xxx" **/
    strncpy(kv, sp, CTABSIZE_TMPKV);
    if((lp = strchr(kv, ',')) != NULL) {
        return 0;
    }
    *lp = '\0';
    
    sp = kv;
    while((p[i]  = strtok(sp, ":,")) != NULL) /** 正常情况下应该只循环一次 **/
    {
        i++;
        sp = NULL;
    }
    sid = p[1];  /** pid[0]="sid"不处理 **/
    
    
    if((sp = strstr(str, "data")) == NULL) {
        return 0;
    }
    if((sp = strstr(sp, ":")) == NULL) {   /** 这个应该是data后面的**/
        return 0;
    }
    /** get "data":"xxx" **/
    strncpy(kv, sp + 1, CTABSIZE_TMPKV);
    /** 剩下data的L0报文 **/
    
    #undef CTABSIZE_TMPKV
    #undef CTABSIZE_TMPSTR
    return JsonParse_L0(kv, jsonstcarr);
}

/**
 * 批量json报文处理入口
 *
 * prase json body(JsonParseBatch)
 * example
 * {
 *    "batch": [
 *       {
 *           "sid": "switch",
 *           "data": {
 *               "on": 1
 *           }
 *       },
 *       {
 *           "sid": "brightness",
 *           "data": {
 *               "value": 100
 *           }
 *       },
 *       {
 *           "sid": "colour",
 *           "data": {
 *             "white": 100
 *          }
 *      }
 *   ]
 * }
 **/
unsigned char JsonParseBatch(unsigned char* str, JsonArr* jsonstcarr)
{
    unsigned char* p = NULL;
    unsigned char* sid = NULL;
    int i;
    
    str = strim(str);    /** 头尾空格删除 **/
    if((str[0] != '{')||(str[strlen(str) - 1] != '}')) {
        return(0);
    }
    if(strstr(str, "batch") == NULL) {   /** command **/
        return(0); 
    }
    if((strchr(str, '[') == NULL)||(strchr(str, ']') == NULL)) {
        return(0);
    }
    
    return JsonParseL1BatchItem(str, jsonstcarr);
}

/**
 * json解析总入口
 **/
unsigned char JsonParse(unsigned char* str, JsonArr* jsonstcarr)
{
    if ((str == NULL) || (jsonstcarr == NULL)) {
        return 0;
    }
    str = strim(str);    /** 头尾空格删除 **/
    if(strstr(str, MBATCH)) /** 批量命令下发 **/
    {
        return JsonParseBatch(str, jsonstcarr);
    }
    
    return JsonParseL1E0(str, jsonstcarr);
}
#endif

