/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  drv_vi_utils.h
* Description:
*
***********************************************************************************/
#ifndef __DRV_VI_UTILS_H__
#define __DRV_VI_UTILS_H__

#include <linux/wait.h>
#include <linux/sched.h>
#include "hi_type.h"

typedef struct hiVI_EVENT_S
{
    wait_queue_head_t queue_head;
    HI_S32            flag;
} VI_EVENT_S;

HI_U32	VI_UtilsGetPTS(HI_VOID);
HI_S32	VI_UtilsLockCreate(HI_VOID**phLock);
HI_VOID VI_UtilsLockDestroy(HI_VOID* hLock);
HI_VOID VI_UtilsLock(HI_VOID* hLock, unsigned long *pFlag);
HI_VOID VI_UtilsUnlock(HI_VOID* hLock, unsigned long *pFlag);
HI_VOID VI_UtilsInitEvent(VI_EVENT_S *pEvent, HI_S32 InitVal);
HI_VOID VI_UtilsGiveEvent(VI_EVENT_S *pEvent);
HI_S32	VI_UtilsWaitEvent(VI_EVENT_S *pEvent, HI_U32 msWaitTime);
HI_U32	VI_UtilsMathPow(HI_U32 a, HI_U32 b);

#endif // __DRV_VI_UTILS_H__
