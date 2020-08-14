/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_str_android.cpp
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2014年7月4日
  最近修改   :
  功能描述   : 字符串操作函数适配(目的:不同操作系统可能不同实现，尤其涉及到安全函数时)
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月4日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
#include <string.h>
#include "securec.h"
#include "osal_str.h"

//lint -e1784
//lint -esym(40,va_list, stArgList, stVar)
//lint -esym(530, stArgList)
//lint -esym(601, stVar)
//lint -esym(1055,va_start, va_end)
//lint -e522
//lint -e10
//lint -e838
//lint -e713 //pclint can not see the declaration of V_SIZE
//lint -e601 //pclint can not see the declaration of stVarLst

VOS_API V_CHAR*
VOS_Strcpy(INOUT V_CHAR* pszDst, IN const V_CHAR* pszSrc)
{
    return strcpy(pszDst, pszSrc);
}

VOS_API V_CHAR*
VOS_Strncpy(INOUT V_CHAR* pszDst, IN const V_CHAR* pszSrc, IN V_SIZE szLen)
{
    return strncpy(pszDst, pszSrc, szLen);
}

VOS_API V_INT32
VOS_Sprintf(INOUT V_CHAR* pszDst, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;
    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsprintf(pszDst, pszFmt, stArgList);
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
    i32Ret = vsnprintf(pszDst, szLen, pszFmt, stArgList);
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

    return vsnprintf(pszDst, szLen, pszFmt, stVar);
}

VOS_API V_INT32 VOS_Sscanf(IN const V_CHAR* pszDst, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsscanf(pszDst, pszFmt, stArgList);
    va_end(stArgList);

    return i32Ret;
}

//安全函数
VOS_API E_VOS_STR_ERR
VOS_Strcpy_S(INOUT V_CHAR* pszDst, IN V_SIZE /*szDstMax*/, IN const V_CHAR* pszSrc)
{
    errno_t i32Ret = 0;

    if(pszDst != strcpy(pszDst, pszSrc))
    {
        i32Ret = E_VOS_STR_ERANGE;
        printf("strcpy failed !\n");
    }

    return (E_VOS_STR_ERR)i32Ret;
}

VOS_API E_VOS_STR_ERR
VOS_Strncpy_S(INOUT V_CHAR* pszDst, IN V_SIZE /*szDstMax*/, IN const V_CHAR* pszSrc,
  IN V_SIZE szCount)
{
    errno_t i32Ret = 0;
    if(pszDst != strncpy(pszDst, pszSrc, szCount))
    {
        i32Ret = E_VOS_STR_ERANGE;
        printf("strncpy failed !\n");
    }

    return (E_VOS_STR_ERR)i32Ret;
}

VOS_API V_INT32
VOS_Sprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE /*szDstMax*/, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;
    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsprintf(pszDst, pszFmt, stArgList);
    va_end(stArgList);

    return i32Ret;
}

VOS_API V_INT32
VOS_Snprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE /*szDstMax*/, IN V_SIZE szCount,
  IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    if(szCount < 1)
    {
        return 0;
    }

    va_list stArgList;
    va_start(stArgList, pszFmt);
    i32Ret = vsnprintf(pszDst, szCount, pszFmt, stArgList);
    if(szCount > 0)
    {
        pszDst[szCount - 1] = '\0';
    }
    va_end(stArgList);

    return i32Ret;
}

VOS_API V_INT32
VOS_Vsnprintf_S(INOUT V_CHAR* pszDst, IN V_SIZE /*szDstMax*/, IN V_SIZE szCount,
  IN const V_CHAR* pszFmt, IN va_list stVarLst)
{
    V_INT32 i32Ret = 0;
    if(szCount < 1)
    {
        return 0;
    }

    i32Ret = vsnprintf(pszDst, szCount, pszFmt, stVarLst);

    return i32Ret;
}

VOS_API V_INT32
VOS_Sscanf_S(INOUT const V_CHAR* pszSrc, IN const V_CHAR* pszFmt, ...)
{
    V_INT32 i32Ret = 0;

    va_list stArgList;

    va_start(stArgList, pszFmt);
    i32Ret = vsscanf(pszSrc, pszFmt, stArgList);
    va_end(stArgList);

    return i32Ret;
}
