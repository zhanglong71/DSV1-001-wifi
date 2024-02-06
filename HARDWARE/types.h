////////////////////////////////////////////////////////////////////////////////
/// @file     types.h
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE TYPE FIRMWARE FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#ifndef __TYPES_H
#define __TYPES_H

// Files includes
#include <stdint.h>
#include <stdbool.h>



#ifdef __cplusplus
#define     __I     volatile                        ///< Defines 'read only' permissions
#else
#define     __I     volatile const                  ///< Defines 'read only' permissions
#endif
#define     __O     volatile                        ///< Defines 'write only' permissions
#define     __IO    volatile                        ///< Defines 'read / write' permissions

typedef long long    s64;                           ///< used for signed 64bit

//typedef signed int   s32;
typedef signed short s16;
typedef signed char  s8;

//typedef signed int  const  sc32;                    ///< Read Only
typedef signed short const sc16;                    ///< Read Only
typedef signed char  const sc8;                     ///< Read Only

//typedef volatile signed int   vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

//typedef volatile signed int  const  vsc32;          ///< Read Only
//typedef volatile signed short const vsc16;          ///< Read Only
//typedef volatile signed char  const vsc8;           ///< Read Only

//typedef long long  u64;
//typedef unsigned int   u32;
//typedef unsigned short u16;
//typedef unsigned char  u8;

//typedef unsigned int  const  uc32;                  ///< Read Only
//typedef unsigned short const uc16;                  ///< Read Only
//typedef unsigned char  const uc8;                   ///< Read Only

//typedef volatile unsigned int   vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

//typedef volatile unsigned int  const  vuc32;        ///< Read Only
typedef volatile unsigned short const vuc16;        ///< Read Only
typedef volatile unsigned char  const vuc8;         ///< Read Only
typedef bool    BOOL;
#ifndef NULL
#define NULL ((void *)0)
#endif


#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648uL)


#define SET8_BIT(i)     (u8)(0x01U << i)
#define SET_BIT0        (u8)0x01
#define SET_BIT1        (u8)0x02
#define SET_BIT2        (u8)0x04
#define SET_BIT3        (u8)0x08
#define SET_BIT4        (u8)0x10
#define SET_BIT5        (u8)0x20
#define SET_BIT6        (u8)0x40
#define SET_BIT7        (u8)0x80

#define SET16_BIT(i)    (u16)(0x0001U << i)
#define SET_BIT00       (u16)0x0001
#define SET_BIT01       (u16)0x0002
#define SET_BIT02       (u16)0x0004
#define SET_BIT03       (u16)0x0008
#define SET_BIT04       (u16)0x0010
#define SET_BIT05       (u16)0x0020
#define SET_BIT06       (u16)0x0040
#define SET_BIT07       (u16)0x0080
#define SET_BIT08       (u16)0x0100
#define SET_BIT09       (u16)0x0200
#define SET_BIT10       (u16)0x0400
#define SET_BIT11       (u16)0x0800
#define SET_BIT12       (u16)0x1000
#define SET_BIT13       (u16)0x2000
#define SET_BIT14       (u16)0x4000
#define SET_BIT15       (u16)0x8000


#define SET32_BIT(i)      (u32)(0x00000001U << i)

#define SET32_BIT00       (u32)0x00000001
#define SET32_BIT01       (u32)0x00000002
#define SET32_BIT02       (u32)0x00000004
#define SET32_BIT03       (u32)0x00000008
#define SET32_BIT04       (u32)0x00000010
#define SET32_BIT05       (u32)0x00000020
#define SET32_BIT06       (u32)0x00000040
#define SET32_BIT07       (u32)0x00000080
#define SET32_BIT08       (u32)0x00000100
#define SET32_BIT09       (u32)0x00000200
#define SET32_BIT10       (u32)0x00000400
#define SET32_BIT11       (u32)0x00000800
#define SET32_BIT12       (u32)0x00001000
#define SET32_BIT13       (u32)0x00002000
#define SET32_BIT14       (u32)0x00004000
#define SET32_BIT15       (u32)0x00008000
#define SET32_BIT16       (u32)0x00010000
#define SET32_BIT17       (u32)0x00020000
#define SET32_BIT18       (u32)0x00040000
#define SET32_BIT19       (u32)0x00080000
#define SET32_BIT20       (u32)0x00100000
#define SET32_BIT21       (u32)0x00200000
#define SET32_BIT22       (u32)0x00400000
#define SET32_BIT23       (u32)0x00800000
#define SET32_BIT24       (u32)0x01000000
#define SET32_BIT25       (u32)0x02000000
#define SET32_BIT26       (u32)0x04000000
#define SET32_BIT27       (u32)0x08000000
#define SET32_BIT28       (u32)0x10000000
#define SET32_BIT29       (u32)0x20000000
#define SET32_BIT30       (u32)0x40000000
#define SET32_BIT31       (u32)0x80000000

#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))



#define LEFT_SHIFT_BIT(x)   (1 << x)


#endif
