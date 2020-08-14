/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: Hi_unf_wdg.c
 * Description:
 *
 * History:
 * Version   Date               Author     DefectNum    Description
 * main\1    2006-10-10   w00142069  NULL         Create this file.
 ******************************************************************************/
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/watchdog.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "hi_drv_wdg.h"
#include "hi_type.h"
#include "hi_error_mpi.h"
#include "hi_drv_struct.h"

static HI_S32 g_s32WDGDevFd = 0;

#define WATCHDOG_TIMEOUT_MAX 356000
#define WATCHDOG_TIMEOUT_MIN 1000

static pthread_mutex_t g_WdgMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_WDG_LOCK() (void)pthread_mutex_lock(&g_WdgMutex);
#define HI_WDG_UNLOCK() (void)pthread_mutex_unlock(&g_WdgMutex);
/*---- wdg ----*/

/*******************************************
Function:              HI_UNF_WDG_Init
Description:   Init WDG devide
Calls:        HI_WDG_Open
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_Init(HI_VOID)
{
    HI_S32 s32DevFd = 0;
    HI_S32 iRet = 0;
    HI_S32 Option = 0;

    HI_WDG_LOCK();
    if (g_s32WDGDevFd > 0) {
        iRet = HI_SUCCESS;
        goto exit;
    }

    s32DevFd = open("/dev/watchdog", O_RDWR, 0);

    if (s32DevFd < 0) {
        HI_ERR_WDG("open /dev/watchdog faild:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_INIT;
        goto exit;
    }

    g_s32WDGDevFd = s32DevFd;

    Option = WDIOS_DISABLECARD;
    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETOPTIONS, &Option);

    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("wdg disable failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_DISABLE;
        close(s32DevFd);
        g_s32WDGDevFd = 0;
        goto exit;
    }

    iRet = HI_SUCCESS;

exit:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:              HI_UNF_WDG_DeInit
Description:  Deinit WDG device
Calls:        HI_WDG_Close
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_DeInit(HI_VOID)
{
    HI_S32 iRet;

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        iRet = HI_SUCCESS;
        goto exit;
    }

    iRet = close(g_s32WDGDevFd);

     if (HI_SUCCESS != iRet) {
        HI_FATAL_WDG("DeInit WDG err:%d\n", iRet);
        iRet =  HI_ERR_WDG_FAILED_DEINIT;
        goto exit;
     }

    g_s32WDGDevFd = 0;
    iRet = HI_SUCCESS;

exit:
    HI_WDG_UNLOCK();
    return iRet;
}

HI_S32 HI_UNF_WDG_GetCapability(HI_U32 *pu32WdgNum)
{
    if (HI_NULL != pu32WdgNum) {
        *pu32WdgNum = HI_WDG_NUM;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

/*******************************************
Function:              HI_UNF_WDG_Enable
Description:  enable WDG device
Calls:        HI_WDG_Enable
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_Enable(HI_U32 u32WdgNum)
{
    HI_S32 iRet = 0;
	HI_S32 Option = 0;

    if (u32WdgNum >= HI_WDG_NUM) {
        HI_ERR_WDG("Input parameter(u32WdgNum) invalid: %d\n", u32WdgNum);
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet =  HI_ERR_WDG_NOT_INIT;
        goto err;
    }


	Option = WDIOS_ENABLECARD;
    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETOPTIONS, &Option);
    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("wdg enable failed:%s, ret:%d\n", strerror(errno), iRet);
        iRet = HI_ERR_WDG_FAILED_ENABLE;
        goto err;
    }

    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:              HI_UNF_WDG_Disable
Description:  disable WDG device
Calls:        HI_WDG_Disable
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_Disable(HI_U32 u32WdgNum)
{
    HI_S32 iRet = 0;
	HI_S32 Option = 0;

    if (u32WdgNum >= HI_WDG_NUM) {
        HI_ERR_WDG("Input parameter(u32WdgNum) invalid: %d\n", u32WdgNum);
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet =  HI_ERR_WDG_NOT_INIT;
        goto err;
    }

	Option = WDIOS_DISABLECARD;
    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETOPTIONS, &Option);

    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("wdg disable failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_DISABLE;
        goto err;
    }
    
    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:              HI_UNF_WDG_SetTimeout
Description:  set the time interval of feeding the WDG
Calls:        HI_WDG_SetTimeout
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_SetTimeout(HI_U32 u32WdgNum, HI_U32 u32Value)
{
    HI_S32 iRet = 0;
    HI_U32 u32ValueInSec;

    if ((u32WdgNum >= HI_WDG_NUM)
        || (u32Value > WATCHDOG_TIMEOUT_MAX)
        || (u32Value < WATCHDOG_TIMEOUT_MIN)) {
        HI_ERR_WDG("Input parameter(u32Value) invalid: %d\n", u32Value);
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet = HI_ERR_WDG_NOT_INIT;
        goto err;
    }

    /* convert ms to s */
    u32ValueInSec = (u32Value + 999) / 1000;

    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETTIMEOUT, &u32ValueInSec);
    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("wdg set timeout failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_SETTIMEOUT;
        goto err;
    }

    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:     HI_UNF_WDG_GetTimeout
Description:  get the time interval of feeding the WDG
Calls:        HI_WDG_GetTimeout
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:       ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_GetTimeout(HI_U32 u32WdgNum, HI_U32 *pu32Value)
{
    HI_S32 iRet = 0;

    if ((u32WdgNum >= HI_WDG_NUM) || (HI_NULL == pu32Value)) {
        HI_ERR_WDG("para pu32Value is null.\n");
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet = HI_ERR_WDG_NOT_INIT;
        goto err;
    }


    iRet = ioctl(g_s32WDGDevFd, WDIOC_GETTIMEOUT, pu32Value);
    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("wdg get timeout failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_GETTIMEOUT;
        goto err;
    }

    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:              HI_UNF_WDG_Clear
Description:  clear the WDG
Calls:        HI_WDG_ClearWatchDog
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_Clear(HI_U32 u32WdgNum)
{
    HI_S32 iRet = 0;

    if (u32WdgNum >= HI_WDG_NUM) {
        HI_ERR_WDG("Input parameter(u32WdgNum) invalid: %d\n", u32WdgNum);
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet = HI_ERR_WDG_NOT_INIT;
        goto err;
    }


    iRet = ioctl(g_s32WDGDevFd, WDIOC_KEEPALIVE, NULL);

    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("clear wdg failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_CLEARWDG;
        goto err;
    }

    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}

/*******************************************
Function:              HI_UNF_WDG_Reset
Description:  reset WDG
Calls:        HI_WDG_Reset
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_WDG_Reset(HI_U32 u32WdgNum)
{
    HI_S32 iRet = 0;
	HI_S32 Option = 0;
	HI_S32 timeout = 0;

    if (u32WdgNum >= HI_WDG_NUM) {
        HI_ERR_WDG("Input parameter(u32WdgNum) invalid: %d\n", u32WdgNum);
        return HI_ERR_WDG_INVALID_PARA;
    }

    HI_WDG_LOCK();
    if (g_s32WDGDevFd <= 0) {
        HI_ERR_WDG("file descriptor is illegal\n");
        iRet = HI_ERR_WDG_NOT_INIT;
        goto err;
    }

	Option = WDIOS_ENABLECARD;
    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETOPTIONS, &Option);
    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("enable failed:%s\n", strerror(errno));
        iRet = HI_ERR_WDG_FAILED_RESET;
        goto err;
    }

	timeout = 1;
    iRet = ioctl(g_s32WDGDevFd, WDIOC_SETTIMEOUT, &timeout);
    if (HI_SUCCESS != iRet) {
        HI_ERR_WDG("reset failed\n");
        iRet = HI_ERR_WDG_FAILED_RESET;
        goto err;
    }

    iRet = HI_SUCCESS;

err:
    HI_WDG_UNLOCK();
    return iRet;
}
