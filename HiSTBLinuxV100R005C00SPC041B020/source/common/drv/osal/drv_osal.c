/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_osal.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/25
  Description   : OS Abstract Layer.
  History       :
  1.Date        : 2013/07/25
    Author      : l00185424
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* Unf headers */

/* Drv headers */

/* Local headers */
#include "hi_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/


/*********************************** Code ************************************/

HI_CHAR* HI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen)
{
    return strncpy(pszDest, pszSrc, ulLen);
}

HI_S32 HI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen)
{
    return strncmp(pszStr1, pszStr2, ulLen);
}

HI_S32 HI_OSAL_Strncasecmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen)
{
    return strncasecmp(pszStr1, pszStr2, ulLen);
}

HI_CHAR* HI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen)
{
    return strncat(pszDest, pszSrc, ulLen);
}

HI_S32 HI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...)
{
    HI_S32 s32Len;
    va_list stArgs = {0};

    va_start(stArgs, pszFormat);
    s32Len = vsnprintf(pszStr, ulLen, pszFormat, stArgs);
    va_end(stArgs);

    return s32Len;
}

HI_S32 HI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, HI_VA_LIST_S stVAList)
{
    return vsnprintf(pszStr, ulLen, pszFormat, stVAList);
}

EXPORT_SYMBOL(HI_OSAL_Strncpy);
EXPORT_SYMBOL(HI_OSAL_Strncmp);
EXPORT_SYMBOL(HI_OSAL_Strncasecmp);
EXPORT_SYMBOL(HI_OSAL_Strncat);
EXPORT_SYMBOL(HI_OSAL_Snprintf);
EXPORT_SYMBOL(HI_OSAL_Vsnprintf);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

