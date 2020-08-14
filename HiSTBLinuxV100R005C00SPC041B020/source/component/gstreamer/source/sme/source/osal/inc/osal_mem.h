/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_mem.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2014年7月4日
  最近修改   :
  功能描述   : 内存操作函数适配(目的:不同操作系统可能不同实现，尤其涉及到安全函数时)
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OSAL_MEM__
#define __OSAL_MEM__

#include "osal_type.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#ifdef VOSDLL_EXPORTS
#define VOS_API /*__declspec(dllexport)*/
#else
#define VOS_API /*__declspec(dllimport)*/
#endif
#else
#define VOS_API
#endif


/*****************************************************************************
 函 数 名  : VOS_Memset
 功能描述  : 给内存块赋值。
 输入参数  : V_VOID* pvSrcMem :要赋值的内存指针,不能为NULL，否则不可预知后果，
                               可能crash。
             V_INT32 i32Val   :内存块每个字节要赋的值。
             V_SIZE szLen     :赋值的长度，字节数。
 输出参数  : V_VOID* pvSrcMem :要赋值的内存指针。
 返 回 值  : 返回指向pvSrcMem内存块首的指针。
 修改历史      :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID*
VOS_Memset(INOUT V_VOID* pvSrcMem, IN V_INT32 i32Val, IN V_SIZE szLen);

/*****************************************************************************
 函 数 名  : VOS_Memcpy
 功能描述  : 内存拷贝,pvDstMem与pvSrcMem不能有地址重叠。
 输入参数  : V_VOID* pvDstMem       :目标内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
             const V_VOID* pvSrcMem :源内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
             V_SIZE szLen           :拷贝的长度，字节数。
 输出参数  : V_VOID* pvDstMem       :目标内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
 返 回 值  : 返回指向DstMem内存块的指针。
 修改历史      :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID*
VOS_Memcpy(INOUT V_VOID* pvDstMem, IN const V_VOID* pvSrcMem, IN V_SIZE szLen);

/*****************************************************************************
 函 数 名  : VOS_Memcmp
 功能描述  : 内存比较,pvSrcMem1与pvSrcMem2,各个字符想减，pvSrcMem1-pvSrcMem2。
 输入参数  : const V_VOID* pvSrcMem1:要比较的内存1，不能为NULL，否则不可预知后果，
                                     可能crash。
             const V_VOID* pvSrcMem2:要比较的内存2，不能为NULL，否则不可预知后果，
                                     可能crash。
             V_SIZE szLen           :要比较的长度，字节数。
 输出参数  : NULL.
 返 回 值  : 0,比较的内存一致;其他,pvSrcMem1-pvSrcMem2,各个字符想减的结果。
 修改历史      :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_INT32
VOS_Memcmp(IN const V_VOID* pvSrcMem1, IN const V_VOID* pvSrcMem2, IN V_SIZE szLen);

/*****************************************************************************
 函 数 名  : VOS_Memmove
 功能描述  : 内存拷贝,pvDstMem与pvSrcMem可以有地址重叠。
 输入参数  : V_VOID* pvDstMem       :目标内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
             const V_VOID* pvSrcMem :源内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
             V_SIZE szLen           :拷贝的长度，字节数。
 输出参数  : V_VOID* pvDstMem       :目标内存指针，不能为NULL，否则不可预知后果，
                                     可能crash。
 返 回 值  : 返回指向DstMem内存块的指针。
 修改历史      :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID*
VOS_Memmove(INOUT V_VOID* pvDstMem, IN const V_VOID* pvSrcMem, IN V_SIZE szLen);

/////////////////////////////////////////////////////////////////////////////////////////////////////
// For security function start.

typedef enum _MemSecureRet
{
    E_SME_MEM_OK = 0,
    E_SME_MEM_ERANGE = 34,
    E_SME_MEM_EINVAL = 22,
    E_SME_MEM_EINVAL_AND_RESET = 150,
    E_SME_MEM_ERANGE_AND_RESET = 162,
    E_SME_MEM_EOVERLAP_AND_RESET = 182,
} E_SME_MEM_SECURE_RET;

// return value:
//   =0:      succeed.
//   >0:      failed; detailed information, refer to "secure function library manual".

VOS_API E_SME_MEM_SECURE_RET
VOS_Memset_S(INOUT V_VOID* pvDestMem, IN V_SIZE szDestMax, IN V_INT32 i32Val, IN V_SIZE szCount);

VOS_API E_SME_MEM_SECURE_RET
VOS_Memcpy_S(INOUT V_VOID* pvDstMem, IN V_SIZE szDestMax, IN const V_VOID* pvSrcMem, IN V_SIZE szCount);

VOS_API E_SME_MEM_SECURE_RET
VOS_Memmove_S(INOUT V_VOID* pvDstMem, IN V_SIZE szDestMax, IN const V_VOID* pvSrcMem, IN V_SIZE szCount);

// For security function end.
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif// __OSAL_MEM__
