/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  vi_utils.c
* Description:
*
***********************************************************************************/
#include <linux/version.h>

#include "drv_vi_utils.h"
#include "hi_drv_mem.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
static spinlock_t time_lock = SPIN_LOCK_UNLOCKED;
#else
static spinlock_t time_lock = __SPIN_LOCK_UNLOCKED(time_lock);
#endif

HI_U32 g_u32JifFirst = 0;

HI_U32 VI_UtilsGetPTS(HI_VOID)
{
    static HI_U32 u32JifLast = 0;
    static HI_U32 u32Ret = 0;
    HI_U32 u32Add;
    HI_U32 u32JifNow;
    static HI_U32 u32UsecComp = 0;
    unsigned long flags;

    spin_lock_irqsave(&time_lock, flags);

    u32JifNow = jiffies;

    if (g_u32JifFirst == 0)
    {
        g_u32JifFirst = 1;
        u32JifLast = u32JifNow;
    }

    if (u32JifNow < u32JifLast)
    {
        u32Add = 0xFFFFFFFFU - u32JifLast + u32JifNow + 1;
    }
    else
    {
        u32Add = u32JifNow - u32JifLast;
    }

    if (u32Add == 0)
    {
        u32UsecComp += 10;
    }
    else
    {
        u32UsecComp = 0;
    }

    u32Ret = u32Ret + jiffies_to_msecs(u32Add);

    u32JifLast = u32JifNow;

    spin_unlock_irqrestore(&time_lock, flags);

    return (HI_U32) (u32Ret + u32UsecComp);
}

HI_S32 VI_UtilsLockCreate(HI_VOID**phLock)
{
    spinlock_t *pLock;

    pLock = (spinlock_t *)HI_KMALLOC(HI_ID_VI, sizeof(spinlock_t), GFP_KERNEL);
    if (pLock == NULL)
    {
        return HI_FAILURE;
    }

    spin_lock_init(pLock);

    *phLock = pLock;

    return HI_SUCCESS;
}

HI_VOID VI_UtilsLockDestroy(HI_VOID* hLock)
{
    HI_KFREE(HI_ID_VI, hLock);
}

HI_VOID VI_UtilsLock(HI_VOID* hLock, unsigned long *pFlag)
{
    spin_lock_irqsave((spinlock_t *)hLock, *pFlag);
}

HI_VOID VI_UtilsUnlock(HI_VOID* hLock, unsigned long *pFlag)
{
    spin_unlock_irqrestore((spinlock_t *)hLock, *pFlag);
}

HI_VOID VI_UtilsInitEvent(VI_EVENT_S *pEvent, HI_S32 InitVal)
{
    pEvent->flag = InitVal;
    init_waitqueue_head(&(pEvent->queue_head));
}

HI_VOID VI_UtilsGiveEvent(VI_EVENT_S *pEvent)
{
    pEvent->flag = 1;

    //wake_up_interruptible (&(pEvent->queue_head));
    wake_up(&(pEvent->queue_head));
}

HI_S32 VI_UtilsWaitEvent(VI_EVENT_S *pEvent, HI_U32 msWaitTime)
{
    HI_S32 l_ret = 0;

    if (msWaitTime != 0xffffffff)
    {
        l_ret = wait_event_interruptible_timeout(pEvent->queue_head, (pEvent->flag != 0), msecs_to_jiffies(msWaitTime));

        pEvent->flag = 0;

        return (l_ret != 0) ? HI_SUCCESS : HI_FAILURE;
    }
    else
    {
        l_ret = wait_event_interruptible(pEvent->queue_head, (pEvent->flag != 0));
        pEvent->flag = 0;
        return (l_ret == 0) ? HI_SUCCESS : HI_FAILURE;
    }
}

HI_U32 VI_UtilsMathPow(HI_U32 a, HI_U32 b)
{
    HI_U32 i;
    HI_U32 result;

    for (i = 0, result = a; i < b; i++)
    {
        result = a * result;
    }

    return result;
}
