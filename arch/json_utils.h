#ifndef __JSON_UTILS_H
#define __JSON_UTILS_H
#ifdef  JSON_GLOBALS
#define JSON_EXT
#else
#define JSON_EXT  extern
#endif
#include <stdio.h>
#include <string.h>

//#define STRING char *
//#define uint unsigned int

#define JSON_SIZE_STRUCTURE 6
#define JSON_SIZE_JSONARRAY 128
#define MBATCH "batch"
typedef struct _JsonArr
{
    char jName[10];
    char jValue[10];
    unsigned char jIndexN;
} JsonArr;

JSON_EXT JsonArr jsonstcarr[JSON_SIZE_STRUCTURE];
JSON_EXT char jsonstrbuf[JSON_SIZE_JSONARRAY];

//void testGenerateJson(void);
//void testPrintJson(void);
//unsigned char JsonGenerateUtil(void);
//unsigned char JsonParseUtil(void);
//unsigned char JsonParseL0(unsigned char* str, JsonArr* jsonstcarr);

#endif
