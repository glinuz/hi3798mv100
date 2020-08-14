/******************************************************************************
Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_osal.h
Version       : V1.0 Initial Draft
Author        : l00185424
Created       : 2013/7/25
Last Modified :
Description   : OS Abstract Layer.
Function List : None.
History       :
******************************************************************************/
#ifndef __HI_OSAL_H__
#define __HI_OSAL_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#else
#include <linux/kernel.h>
#endif
#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/** @addtogroup   COMMON  */
/** @{ */  /** <!-- [COMMON] */

typedef va_list HI_VA_LIST_S;

HI_CHAR* HI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
HI_S32 HI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);
HI_S32 HI_OSAL_Strncasecmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);
HI_CHAR* HI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
HI_S32 HI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...);
HI_S32 HI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, HI_VA_LIST_S stVAList);


/** @} */  /** <!-- ==== group Definition end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_OSAL_H__ */

