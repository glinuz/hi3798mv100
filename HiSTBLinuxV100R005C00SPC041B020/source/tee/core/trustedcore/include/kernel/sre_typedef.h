/**
 * @file sre_typedef.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义基本数据类型和数据结构。 \n
 */

#ifndef _SRE_TYPEDEF_H
#define _SRE_TYPEDEF_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#ifdef SRE_WIN
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>

#define INLINE                                              static __inline
#else
#define INLINE                                              static inline
#endif

#ifndef SRE_TYPE_DEF
#define SRE_TYPE_DEF

/* type definitions */
#ifdef SRE_WIN
typedef unsigned __int64                                    UINT64;
typedef signed   __int64                                    INT64;
typedef unsigned int                                        UINTPTR;
typedef signed int                                          INTPTR;
typedef unsigned short                                      UINT16;
typedef unsigned int                                        UINT32;
typedef unsigned char                                       UINT8;
typedef signed char                                         INT8;
typedef signed short                                        INT16;
typedef signed int                                          INT32;
typedef float                                               FLOAT;
typedef double                                              DOUBLE;
typedef char                                                CHAR;
#else
typedef unsigned char                                       UINT8;
typedef unsigned short                                      UINT16;
typedef unsigned int                                        UINT32;
typedef signed char                                         INT8;
typedef signed short                                        INT16;
typedef signed int                                          INT32;
typedef float                                               FLOAT;
typedef double                                              DOUBLE;
typedef char                                                CHAR;

typedef unsigned int                                        BOOL;
typedef unsigned long long                                  UINT64;
typedef signed long long                                    INT64;
typedef unsigned int                                        UINTPTR;
typedef signed int                                          INTPTR;
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;
#endif

#ifndef VOID
#define VOID                                                void
#endif

#endif  /*end of #ifndef SRE_TYPE_DEF*/

#ifndef FALSE
#define FALSE                                               ((BOOL)0)
#endif

#ifndef TRUE
#define TRUE                                                ((BOOL)1)
#endif

#ifndef NULL
#define NULL                                                ((VOID *)0)
#endif

#ifdef  YES
#undef  YES
#endif
#define YES                                                 (1)

#ifdef  NO
#undef  NO
#endif
#define NO                                                  (0)

#define OS_NULL_BYTE                                        ((UINT8)0xff)
#define OS_NULL_SHORT                                       ((UINT16)0xffff)
#define OS_NULL_INT                                         ((UINT32)0xffffffff)
#define OS_MAX_UINT_VALUE                                   ((UINT32)0xffffffff)

#ifndef SRE_OK
#define SRE_OK                                          (0)
#endif

#define OS_FAIL                                             (1)
#define OS_ERROR                                            (UINT32)(-1)
#define OS_INVALID                                          (-1)

#ifndef __ASSEMBLY__

typedef VOID (* OS_VOID_FUNC)();

/* DTS: add a func piont type, l0020256 add begin */
typedef UINT32(* OS_UINT32_FUNC)();
/* DTS: add a func piont type, l0020256 add end */

/**
 * @ingroup  SRE_sys
 * 系统时间定义结构体
 */
typedef struct tagSysTime {
	UINT16  uwYear;    /**< 年，系统时间取值为1970 ~ 2038或1970 ~ 2100 */
	UINT8   ucMonth;   /**< 月，取值为 1 - 12 */
	UINT8   ucDay;     /**< 日，取值为 1 - 31 */
	UINT8   ucHour;    /**< 时，取值为 0 - 23 */
	UINT8   ucMinute;  /**< 分，取值为 0 - 59 */
	UINT8   ucSecond;  /**< 秒，取值为 0 - 59 */
	UINT8   ucWeek;    /**< 星期，取值为 0 - 6  */
} SYS_TIME_S;

#endif

#define SET_BIT(map, bit) (map |= (0x1<<(bit)))
#define CLR_BIT(map, bit) (map &= (~(unsigned)(0x1<<(bit))))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_TYPEDEF_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

