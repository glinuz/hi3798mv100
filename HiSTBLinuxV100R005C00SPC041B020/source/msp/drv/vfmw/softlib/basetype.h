
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: basetype.h
Author & ID: 宋小刚+00133955 张丽萍+00139017 曹俊茂+00103648
Version: 1.00
Date:  2010-03-01
Description: 本文件包括了常用数据类型重定义的宏（windows、linux平台）
Function List:
History:
* Revision 1.00  2010/03/01 10:00:00  songxg+00133955
* 初始版本编码完成
*
* Revision 1.01  2010/03/02 14:15:00  songxg+00133955
* 根据曹俊茂和张丽萍意见添加INT40、UINT40、BOOL类型及将防重定义宏去除
*
*******************************************************************************/
#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

// 整数数据类型重定义
typedef signed char        INT8;
typedef signed short       INT16;
typedef signed int         INT32;
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;

#if defined(__GNUC__)
typedef          long long INT64;
typedef unsigned long long UINT64;

typedef double             DOUBLE;
#else
typedef          __int64   INT64;
typedef unsigned __int64   UINT64;
#endif

// DSP可能会用到的数据类型
typedef long               INT40;
typedef unsigned long      UINT40;

// 浮点数据类型重定义
typedef float              FLOAT32;
typedef double             FLOAT64;


// 布尔数据类型重定义
typedef char               BOOL8;
typedef short              BOOL16;
typedef int                BOOL;

#ifndef TRUE
#define TRUE               1
#endif

#ifndef FALSE
#define FALSE              0
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __BASE_TYPE_H__ */

/************************************************************************
* $Log$
************************************************************************/
