/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_mem_android.cpp
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2014年7月4日
  最近修改   :
  功能描述   : android/linux平台内存操作函数适配(目的:不同操作系统可能不同实现，
               尤其涉及到安全函数时)
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "osal_mem.h"

#include "securec.h"

//lint -e1784
//lint -e713 //pclint can not see the declaration of V_SIZE

VOS_API V_VOID*
VOS_Memset(INOUT V_VOID* pvSrcMem, IN V_INT32 i32Val, IN V_SIZE szLen)
{
    return memset(pvSrcMem, i32Val, szLen);
}

VOS_API V_VOID*
VOS_Memcpy(INOUT V_VOID* pvDstMem, IN const V_VOID* pvSrcMem, IN V_SIZE szLen)
{
    return memcpy(pvDstMem, pvSrcMem, szLen);
}

VOS_API V_INT32
VOS_Memcmp(IN const V_VOID* pvSrcMem1, IN const V_VOID* pvSrcMem2, IN V_SIZE szLen)
{
    return memcmp(pvSrcMem1, pvSrcMem2, szLen);
}

VOS_API V_VOID*
VOS_Memmove(INOUT V_VOID* pvDstMem, IN const V_VOID* pvSrcMem, IN V_SIZE szLen)
{
    return memmove(pvDstMem, pvSrcMem, szLen);
}

//////////////////////////////////////////////////////////////////////////////
VOS_API E_SME_MEM_SECURE_RET
VOS_Memset_S(INOUT V_VOID* pvDestMem, IN V_SIZE /*szDestMax*/, IN V_INT32 i32Val, IN V_SIZE szCount)
{
    errno_t ret = 0;

    if(pvDestMem != memset(pvDestMem, i32Val, szCount))
    {
        ret = E_SME_MEM_ERANGE;
        printf("memset failed !\n");
    }

    return (E_SME_MEM_SECURE_RET)ret;
}

VOS_API E_SME_MEM_SECURE_RET
VOS_Memcpy_S(INOUT V_VOID* pvDstMem, IN V_SIZE /*szDestMax*/, IN const V_VOID* pvSrcMem, IN V_SIZE szCount)
{
    errno_t ret = 0;

    if(pvDstMem != memcpy(pvDstMem, pvSrcMem, szCount))
    {
        ret = E_SME_MEM_ERANGE;
        printf("memcpy failed !\n");
    }

    return (E_SME_MEM_SECURE_RET)ret;
}

VOS_API E_SME_MEM_SECURE_RET
VOS_Memmove_S(INOUT V_VOID* pvDstMem, IN V_SIZE /*szDestMax*/, IN const V_VOID* pvSrcMem, IN V_SIZE szCount)
{
    errno_t ret = 0;

    if(pvDstMem != memmove(pvDstMem, pvSrcMem, szCount))
    {
        ret = E_SME_MEM_ERANGE;
        printf("memmove failed !\n");
    }

    return (E_SME_MEM_SECURE_RET)ret;
}
