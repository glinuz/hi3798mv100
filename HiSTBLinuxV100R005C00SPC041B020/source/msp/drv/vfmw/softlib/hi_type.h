
/******************************************************************************
*             Copyright 2009 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: hi_type.h
* Description: STB Step-2 模块公共定义
*
* History:
* Version  Date        Author                               DefectNum  Description
* 1.0      2009/03/19  Hisilicon STB Step-2 software group  NULL       Create this file.
******************************************************************************/

#ifndef __HI_TYPE_H__
#define __HI_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * 操作系统定义，默认是Linux
 *----------------------------------------------*/

#if !defined(HI_OS_LINUX)
#define HI_OS_LINUX         1
#endif
#if !defined(HI_OS_WIN32)
#define HI_OS_WIN32         2
#endif
#if !defined(HI_OS_TYPE)
#define HI_OS_TYPE          HI_OS_LINUX
#endif
#ifndef __FUNCTION__
//#define __FUNCTION__        ""
#endif

/*----------------------------------------------*
 * 数据类型定义，应用层和内核代码均使用         *
 *----------------------------------------------*/

typedef unsigned char       HI_U8;
typedef unsigned char       HI_UCHAR;
typedef unsigned short      HI_U16;
typedef unsigned int        HI_U32;
#if HI_OS_TYPE == HI_OS_LINUX
typedef unsigned long long  HI_U64;
#elif HI_OS_TYPE == HI_OS_WIN32
typedef unsigned __int64    HI_U64;
#endif

typedef char                HI_S8;
typedef short               HI_S16;
typedef int                 HI_S32;

typedef long                HI_LONG;
typedef unsigned long       HI_UL;

#if HI_OS_TYPE == HI_OS_LINUX
typedef  long long          HI_S64;
#elif HI_OS_TYPE == HI_OS_WIN32
typedef  __int64            HI_S64;
#endif

typedef char                HI_CHAR;
typedef char               *HI_PCHAR;

typedef float               HI_FLOAT;
typedef double              HI_DOUBLE;
typedef void                HI_VOID;

typedef HI_U64              HI_PTS_TIME;
typedef unsigned long       HI_SIZE_T;
typedef unsigned long       HI_LENGTH_T;

typedef int                 STATUS;

typedef enum
{
    HI_FALSE    = 0,
    HI_TRUE     = 1
} HI_BOOL;

#ifndef __HI_HANDLE__
#define __HI_HANDLE__
typedef HI_U32 HI_HANDLE;
#endif

#ifndef NULL
#define NULL                (0U)
#endif

#define HI_NULL             (0U)
#define HI_NULL_PTR         (0U)

#define HI_SUCCESS          (0)
#define HI_FAILURE          (-1)

#define HI_LITTLE_ENDIAN    (1234)
#define HI_BIG_ENDIAN       (4321)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TYPE_H__ */

