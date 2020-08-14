/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : osal_type.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2011年9月28日
  最近修改   :
  功能描述   : 终端软件平台的类型定义的公共头文件
  函数列表   : 无
  修改历史   : 无
  1.日    期   : 2011年9月28日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OSAL_TYPE_H__
#define __OSAL_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>

/*----------------------------------------------*
 * 类型重定义                                     *
 *----------------------------------------------*/

typedef char V_CHAR;
typedef unsigned char V_UCHAR;
typedef char V_INT8;
typedef unsigned char V_UINT8;
typedef short int V_INT16;
typedef unsigned short int V_UINT16;
typedef int V_INT32;
typedef unsigned int V_UINT32;
typedef long long int V_INT64;
typedef unsigned long long int V_UINT64;

typedef long V_LONG;
typedef unsigned long V_ULONG;

typedef float V_FLOAT;
typedef double V_DOUBLE;
typedef void V_VOID;

#ifdef __PC_LINT__
#define V_SIZE V_UINT32
#else
typedef size_t V_SIZE;
#endif



#ifdef __cplusplus
typedef bool V_BOOL;
#else
typedef unsigned char V_BOOL;
#endif

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ICS_FALSE
#ifdef __cplusplus
#define ICS_FALSE false
#else
#define ICS_FALSE ((V_BOOL)0)
#endif
#endif

#ifndef ICS_TRUE
#ifdef __cplusplus
#define ICS_TRUE true
#else
#define ICS_TRUE ((V_BOOL)1)
#endif
#endif

/*----------------------------------------------*
 * 公共错误码定义                               *
 *----------------------------------------------*/
#ifndef ICS_SUCCESS
#define ICS_SUCCESS 0
#endif

#ifndef ICS_FAIL
#define ICS_FAIL 1
#endif

#ifndef ICS_INVALID_PARA
#define ICS_INVALID_PARA 0x00000002
#endif

#ifdef WIN32

#ifndef ut_virtual
#define ut_virtual virtual
#endif

#ifndef ut_protected
#define ut_protected public
#endif

#ifndef ut_private
#define ut_private public
#endif

#else

#ifndef ut_virtual
#define ut_virtual
#endif

#ifndef ut_protected
#define ut_protected protected
#endif

#ifndef ut_private
#define ut_private private
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ICS_TYPE_H__ */

