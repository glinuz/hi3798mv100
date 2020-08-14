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
