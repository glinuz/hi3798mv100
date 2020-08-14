/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_str_android.cpp
  版 本 号   : 初稿
  作    者   : l00180035
  生成日期   : 2014年7月4日
  最近修改   :
  功能描述   : 字符串操作函数适配(目的:不同操作系统可能不同实现，尤其涉及到安全函数时)
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月4日
    作    者   : l00180035
    修改内容   : 创建文件

******************************************************************************/
#include <string.h>
#include "securec.h"
#include "osal_str.h"

#define VOS_STR_PRINT printf

//lint -e1784
//lint -esym(40,va_list, stArgList, stVar, strlen)
//lint -esym(530, stArgList)
//lint -esym(601, stVar)
//lint -esym(1055,va_start, va_end, strlen)
//lint -e522
//lint -e10
//lint -e838
//lint -e713 //pclint can not see the declaration of V_SIZE

#define FAKE_MAX_SIZE (2*1024*1024)

VOS_API V_CHAR*
VOS_Strcpy(INOUT V_CHAR* pszDst, IN const V_CHAR* pstSrc)
{
    if(0 != strcpy_s(pszDst, strlen(pstSrc) + 1, pstSrc))
    {
        VOS_STR_PRINT("VOS_Strcpy failed \n");
        return NULL;
    }

    return pszDst;
}

VOS_API V_CHAR*
VOS_Strncpy(INOUT V_CHAR* pszDst, IN const V_CHAR* pstSrc, IN V_SIZE szLen)
{
    if(0 != strncpy_s(pszDst, szLen + 1, pstSrc, szLen))
    {
        VOS_STR_PRINT("VOS_Strcpy failed \n");
        return NULL;
    }

    return pszDst;
}

VOS_API V_INT32
VOS_Sprintf(INOUT V_CHAR* pszDst, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;
    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsprintf_s(pszDst, (V_SIZE)FAKE_MAX_SIZE, pszFmt, stArgList);
    va_end(stArgList);

    return i32Ret;
}

VOS_API V_INT32
VOS_Snprintf(INOUT V_CHAR* pszDst, IN V_SIZE szLen, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    if(szLen < 1)
    {
        return 0;
    }

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsnprintf_s(pszDst, szLen, szLen - 1, pszFmt, stArgList);
    if(szLen > 0)
    {
        pszDst[szLen - 1] = '\0';
    }
    va_end(stArgList);

    return i32Ret;
}

VOS_API V_INT32
VOS_Vsnprintf(INOUT V_CHAR* pszDst, IN V_SIZE szLen, IN const V_CHAR* pszFmt,
    IN va_list stVar)
{
    if(szLen < 1)
    {
        return 0;
    }

    return vsnprintf_s(pszDst, szLen, szLen - 1, pszFmt, stVar);
}

VOS_API V_INT32 VOS_Sscanf(IN const V_CHAR* pszDst, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsscanf_s(pszDst, pszFmt, stArgList);
    va_end(stArgList);

    return i32Ret;
}

//安全函数
VOS_API E_VOS_STR_ERR
VOS_Strcpy_S(INOUT V_CHAR* pszDst, IN V_SIZE szDstMax, IN const V_CHAR* pstSrc)
{
    errno_t i32Ret = 0;

    i32Ret = strcpy_s(pszDst, szDstMax, pstSrc);
    if(i32Ret != 0)
    {
        VOS_STR_PRINT("strcpy_s failed, i32Ret=%d(%#x)\n", i32Ret, i32Ret);
    }

    return (E_VOS_STR_ERR)i32Ret;
}

VOS_API E_VOS_STR_ERR
VOS_Strncpy_S(INOUT V_CHAR* pszDst, IN V_SIZE szDstMax, IN const V_CHAR* pstSrc,
  IN V_SIZE szCount)
{
    errno_t i32Ret = 0;
    i32Ret = strncpy_s(pszDst, szDstMax, pstSrc, szCount);
    if(i32Ret != 0)
    {
        VOS_STR_PRINT("strncpy_s failed, i32Ret=%d(%#x)\n", i32Ret, i32Ret);
    }

    return (E_VOS_STR_ERR)i32Ret;

}

VOS_API V_INT32
VOS_Sprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE szDstMax, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;
    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsprintf_s(pszDst, szDstMax, pszFmt, stArgList);
    va_end(stArgList);

    if(-1 == i32Ret)
    {
        VOS_STR_PRINT("vsprintf_s failed\n");
    }

    return i32Ret;
}

VOS_API V_INT32
VOS_Snprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE szDstMax, IN V_SIZE szCount,
  IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsnprintf_s(pszDst, szDstMax, szCount, pszFmt, stArgList);
    va_end(stArgList);

    if(-1 == i32Ret)
    {
        VOS_STR_PRINT("vsnprintf_s failed\n");
    }

    return i32Ret;

}

VOS_API V_INT32
VOS_Vsnprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE szDstMax, IN V_SIZE szCount,
  IN const V_CHAR* pszFmt, IN va_list stVarLst)
{
    V_INT32 i32Ret = 0;

    i32Ret = vsnprintf_s(pszDst, szDstMax, szCount, pszFmt, stVarLst);

    if(-1 == i32Ret)
    {
        VOS_STR_PRINT("vsnprintf_s failed\n");
    }

    return i32Ret;
}

VOS_API V_INT32
VOS_Sscanf_S(INOUT const V_CHAR* pszSrc, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsscanf_s(pszSrc, pszFmt, stArgList);
    va_end(stArgList);
    if(-1 == i32Ret)
    {
        VOS_STR_PRINT("vsscanf_s failed\n");
    }

    return i32Ret;
}
