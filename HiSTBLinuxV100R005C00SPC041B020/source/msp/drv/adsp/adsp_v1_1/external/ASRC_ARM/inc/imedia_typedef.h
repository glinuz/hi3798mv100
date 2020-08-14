//==============================================================================
//           Copyright (C), 2008-2013, Huawei Tech. Co., Ltd.
//==============================================================================
//  文件名称: imedia_typedef.h
//  文件描述: 该文件包含iMedia发布的算法库对数据类型的重定义信息，
//            为了跟其他项目组、产品线的定义不冲突和统一，提供宏控制的方式，对
//            重定义内容进行加载，定义仅供TI、LSI、ARM、HiFi平台使用
//  修改记录: 见文件尾
//==============================================================================

#ifndef _IMEDIA_TYPEDEF_
#define _IMEDIA_TYPEDEF_

#ifdef __cplusplus
extern "C" {
#endif

// 使用uniDSP宏定义防止重复定义引起的编译报错
#ifndef UNI_TYPE_DEF
#define UNI_TYPE_DEF

// 8位数据类型重定义
typedef unsigned char         IMEDIA_UINT8;
typedef signed char           IMEDIA_INT8;
typedef char                  IMEDIA_CHAR;   // 8位数据类型重定义,与uniDSP头文件兼容

// 16位数据类型重定义
typedef unsigned short        IMEDIA_UINT16;
typedef signed short          IMEDIA_INT16;

// 32位数据类型重定义
typedef unsigned int          IMEDIA_UINT32;
typedef signed int            IMEDIA_INT32;
typedef unsigned int          IMEDIA_BOOL;  // 32位数据类型重定义,与uniDSP头文件兼容

// 32位数据类型重定义, 与uniDSP平台头文件兼容
typedef unsigned int          IMEDIA_UINTPTR;
typedef signed int            IMEDIA_INTPTR;

// 浮点数据类型重定义: 与uniDSP平台头文件兼容
typedef float                 IMEDIA_FLOAT;
typedef double                IMEDIA_DOUBLE;

// 浮点数据类型重定义: 与VTC、VQE算法库内定义一致
typedef float                 IMEDIA_FLOAT32;
typedef double                IMEDIA_FLOAT64;

// VOID重定义，与uniDSP平台头文件兼容
#ifndef     IMEDIA_VOID
#define     IMEDIA_VOID              void
#endif

//==============================================================================
// ARM平台64位数据类型重定义
//==============================================================================
#ifdef __arm__
typedef unsigned long long    IMEDIA_UINT64;
typedef long long             IMEDIA_INT64;
#endif

//==============================================================================
// MSVC平台64位数据类型重定义
//==============================================================================
#if defined(_MSC_VER)
typedef __int64               IMEDIA_INT64;
#endif

#if defined(_MSC_VER)
typedef unsigned __int64      IMEDIA_UINT64;
#endif

//==============================================================================
// TI平台40位、64位数据类型重定义
//==============================================================================
#ifdef __COMPILER_VERSION__
// 40位数据类型重定义
#ifdef __TI_40BIT_LONG__               // COFF 格式下long 型位宽是40位, ELF 格式下
                                       // long 型位宽是32位 
typedef unsigned long         IMEDIA_UINT40;
typedef long                  IMEDIA_INT40;
#else                                  // ELF 格式的40位定义
typedef unsigned __int40_t    IMEDIA_UINT40;
typedef  __int40_t            IMEDIA_INT40;
#endif

#if (__COMPILER_VERSION__ < 5000000)
// CBSC TI平台低版本编译器(4.32,低于5.0版本)不支持long long类型
#else
typedef unsigned long long    IMEDIA_UINT64;
typedef long long             IMEDIA_INT64;
#endif
#endif

//==============================================================================
// HiFi平台64位数据类型重定义
//==============================================================================
#ifdef __XTENSA__
typedef unsigned long long    IMEDIA_UINT64;
typedef long long             IMEDIA_INT64;
#endif

#endif    // end of #ifndef UNI_TYPE_DEF

#ifndef     IMEDIA_NULL
#define     IMEDIA_NULL              ((IMEDIA_VOID *)0)
#endif

#ifdef __cplusplus
}
#endif

#endif    // end of #ifndef _IMEDIA_TYPEDEF_

//==============================================================================
// 修改记录:
// 2011-5-13 11:45:34   创建文件
//                      修改人: 刘有文，王青
// 2011-5-30 14:58:18   修改宏__TI_COMPILER_VERSION__为__COMPILER_VERSION__,解决
//                      4.32编译器上INT40无定义的错误
//                      修改人: 李杰，王青
// 2012-4-20 16:50:06   C6000 下 ELF格式下long型为32位变量类型，现在在ELF格式下
//                      修改为 __int40_t
//                      修改人: 顾雷雷
// 2013-6-24 13:54:41   添加vc和XTENSA版本long long类型  
//                      修改人: 施雯
// 2013-7-18 16:05:02   按照TI平COFF 格式下long 型位宽是40位, ELF 格式下long 型
//                      位宽是32位同步修改
//                      修改人: 施雯
// 2013-7-19 10:20:40   统一ARM和HiFi平台该文件格式
//                      修改人: 黄涛
// 2014-4-29 10:20:40   添加IMEDIA_前缀，解决在各平台集成时编译不通过问题
//                      修改人: 周洪伟
//==============================================================================
