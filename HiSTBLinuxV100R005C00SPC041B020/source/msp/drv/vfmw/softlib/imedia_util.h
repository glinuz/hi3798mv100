
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: imedia_util.h
Author & ID: 宋小刚+00133955
Version: 1.00
Date:  2009-12-18
Description: 本文件包括了中间处理模块的接口函数及相关结构定义
Function List:
1.
History:
* Revision 1.00  2009/12/18 10:00:00  songxg+00133955
* 初始版本编码完成
*
* Revision 1.01  2010/01/21 14:15:00  songxg+00133955
* 根据评审意见修定代码规范、接口修改等
*
* Revision 1.02  2010/03/23 14:15:00  songxg+00133955
* 增加内存对齐分配和对齐释放等函数
*
* Revision 1.03  2010/04/29 18:25:00  songxg+00101841
* 修改汇编优化传入接口，并将CPU指令相关信息从imedia_common.h移到本文件中定义
*
*******************************************************************************/
#ifndef __IMEDIA_UTIL_H__
#define __IMEDIA_UTIL_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#include "imedia_common.h"

#pragma pack(push, 1)

/**< 防错码，用于防范外部模块传递句柄出错 */
#define CRC_MASK 0x55AA55AA

#define IMEDIA_FOURCC(a, b, c, d) (((UINT32)a) | ( ((UINT32)b) << 8 ) | ( ((UINT32)c) << 16 ) | ( ((UINT32)d) << 24 ))
#define IMEDIA_TWOCC(a, b)        ((UINT16)(a) | ( (UINT16)(b) << 8 ))

#ifndef IMEDIA_MAX
#define IMEDIA_MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef IMEDIA_MIN
#define IMEDIA_MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef IMEDIA_ABS
#define IMEDIA_ABS(x)      ((x) < (0) ? (-(x)) : (x))
#endif

#ifndef SAFE_FREE_CUSTOM
#define SAFE_FREE_CUSTOM(pfn, x) {if (x){pfn(x);x = NULL;}}
#endif

/*!CPU flags*/
#define IMEDIA_CPU_CACHELINE_32    0x000001  /* avoid memory loads that span the border between two cachelines */
#define IMEDIA_CPU_CACHELINE_64    0x000002  /* 32/64 is the size of a cacheline in bytes */
#define IMEDIA_CPU_ALTIVEC         0x000004
#define IMEDIA_CPU_MMX             0x000008
#define IMEDIA_CPU_MMXEXT          0x000010  /* MMX2 aka MMXEXT aka ISSE */
#define IMEDIA_CPU_SSE             0x000020
#define IMEDIA_CPU_SSE2            0x000040
#define IMEDIA_CPU_SSE2_IS_SLOW    0x000080  /* avoid most SSE2 functions on Athlon64 */
#define IMEDIA_CPU_SSE2_IS_FAST    0x000100  /* a few functions are only faster on Core2 and Phenom */
#define IMEDIA_CPU_SSE3            0x000200
#define IMEDIA_CPU_SSSE3           0x000400
#define IMEDIA_CPU_SHUFFLE_IS_FAST 0x000800  /* Penryn, Nehalem, and Phenom have fast shuffle units */
#define IMEDIA_CPU_STACK_MOD4      0x001000  /* if stack is only mod4 and not mod16 */
#define IMEDIA_CPU_SSE4            0x002000  /* SSE4.1 */
#define IMEDIA_CPU_SSE42           0x004000  /* SSE4.2 */
#define IMEDIA_CPU_SSE_MISALIGN    0x008000  /* Phenom support for misaligned SSE instruction arguments */
#define IMEDIA_CPU_LZCNT           0x010000  /* Phenom support for "leading zero count" instruction. */

typedef struct STRU_IMEDIA_CPU_CAPS
{
    UINT32 uiCpuFlag;               /**< CPU能力MASK标记，如是否支持MMX、SSE等，当且仅当CPU能力也支持该功能时才有效，可通过IMedia_GetCpuCaps获取 */
} STRU_IMEDIA_CPU_CAPS;

typedef struct STRU_IMEDIA_GLOBAL_INFO
{
    BOOL  bIsExternalSet;           /**< 信息是否为外部设置的 */
    BOOL  bMallocFxnUsed;           /**< Malloc回调函数是否被使用过 */
    STRU_IMEDIA_SYSINFO stSysInfo;  /**< 系统信息结构 */
} STRU_IMEDIA_GLOBAL_INFO;

extern STRU_IMEDIA_GLOBAL_INFO g_stGlobalInfo;

#if (!defined(ARCH_X86) || !ARCH_X86)
/****************************************************************************************
* 函数说明：获取CPU信息函数。
* 输入参数：
*     caps       CPU支持指令信息
* 输出参数：
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_GetCpuCaps(STRU_IMEDIA_CPU_CAPS *pstCaps);
#endif

/*========================================================================================
* 函数说明：设置系统信息，只能在内存分配函数尚未调用前设置有效
* 输入参数：
*     pstSysInfo 系统信息
* 输出参数：
* 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_SetSysInfo(struct STRU_IMEDIA_SYSINFO *pstSysInfo);

/*****************************************************************************************
* 函数说明：时间获取函数。
* 输入参数：
* 输出参数：
* 返 回 值：返回相应的时间。
*****************************************************************************************/
#ifndef ENV_ARMLINUX_KERNEL
EXPORT_API const char  *IMedia_OS_TimeString();
#endif
//EXPORT_API UINT64 IMedia_OS_Microseconds();
//EXPORT_API UINT64 IMedia_OS_Milliseconds();
//EXPORT_API UINT32 IMedia_OS_Milliseconds32();
EXPORT_API UINT32 IMedia_ValueOffset32(UINT32 begin, UINT32 end);

/*****************************************************************************************
* 函数说明：线程休息函数。
* 输入参数：
*     ms    休息时间
* 输出参数：
* 返 回 值：无。
*****************************************************************************************/
EXPORT_API void   IMedia_Sleep(UINT32 ms);

/*****************************************************************************************
* 函数说明：日志信息输出函数。
* 输入参数：
*     p
*      level       CPU支持指令信息
* 输出参数：
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
#if defined(__GNUC__)
EXPORT_API void IMedia_msg_format(const char *file, int line, const char *funcname, int level, const char *format, ...);
#define IMEDIA_LOG(level, ...) IMedia_msg_format(__FILE__, __LINE__, __FUNCTION__, level, ##__VA_ARGS__)
#else
EXPORT_API void IMedia_msg_format(int level, const char *format, ...);
#define IMEDIA_LOG IMedia_msg_format
#endif

/*****************************************************************************************
* 函数说明：内存分配函数。
* 输入参数：
*     size  所需内存长度
* 输出参数：
* 返 回 值：实际内存空间地址，或失败-NULL。
*****************************************************************************************/
EXPORT_API void *IMedia_Malloc(unsigned int size);
EXPORT_API void *IMedia_AlignMalloc(unsigned int size);

/*****************************************************************************************
* 函数说明：内存释放函数。
* 输入参数：
*     ptr   内存地址
* 输出参数：
* 返 回 值：无。
*****************************************************************************************/
EXPORT_API void  IMedia_Free(void *ptr);
EXPORT_API void  IMedia_AlignFree(void *ptr);

/*****************************************************************************************
* INI配置文件读写函数
*****************************************************************************************/
typedef void *IMEDIA_INI_FILE;

EXPORT_API INT32  IMedia_Ini_OpenFromFile(const char *pszFileName, IMEDIA_INI_FILE *pIni);
EXPORT_API INT32  IMedia_Ini_OpenFromMemory(const char *pszText, IMEDIA_INI_FILE *pIni);
EXPORT_API void   IMedia_Ini_Close(IMEDIA_INI_FILE ini);
EXPORT_API INT32  IMedia_Ini_GetPairCount(IMEDIA_INI_FILE ini);
EXPORT_API INT32  IMedia_Ini_GetInteger(IMEDIA_INI_FILE ini, const char *key, INT32 defaultValue);
EXPORT_API double IMedia_Ini_GetDouble(IMEDIA_INI_FILE ini, const char *key, double defaultValue);
EXPORT_API INT32  IMedia_Ini_GetBoolean(IMEDIA_INI_FILE ini, const char *key, const char *defaultValue);
EXPORT_API char  *IMedia_Ini_GetString(IMEDIA_INI_FILE ini, const char *key);
EXPORT_API const char *IMedia_Ini_GetValue(IMEDIA_INI_FILE ini, const char *key);

#if 0
/*****************************************************************************************
* 函数说明：错误字符串获取函数。
* 输入参数：
*     iErrorCode   模块内部错误码
* 输出参数：
* 返 回 值：返回相应的字符串。
*****************************************************************************************/
EXPORT_API INT32 IMedia_GetErrorString(INT32 iErrorCode, char *pszRet, INT32 iLen);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __IMEDIA_UTIL_H__ */
