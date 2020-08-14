/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_fence.c
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/


/*********************************add include here******************************/
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/time.h>
#include <linux/fb.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>

#include "drv_hifb_mem.h"

#ifdef CFG_HIFB_FENCE_SUPPORT
#include "drv_hifb_fence.h"
#include "drv_hifb_config.h"

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
HIFB_SYNC_INFO_S gs_SyncInfo;


/******************************* API declaration *****************************/
HI_S32 DRV_HIFB_FenceInit(HIFB_PAR_S *par)
{
    if (HIFB_LAYER_HD_0 != par->stBaseInfo.u32LayerID)
    {
        return HI_SUCCESS;
    }

    atomic_set(&gs_SyncInfo.UpdateCnt, 0);
    gs_SyncInfo.u32FenceValue = CONFIG_HIFB_LAYER_BUFFER_MAX_NUM - 2;
    gs_SyncInfo.u32Timeline   = 0;
    gs_SyncInfo.FrameEndFlag  = 0;
    init_waitqueue_head(&gs_SyncInfo.FrameEndEvent);
    gs_SyncInfo.pstTimeline = sw_sync_timeline_create("hifb");

    par->pstHwcRefreshWorkqueue = create_singlethread_workqueue(HIFB_HWC_REFRESH_WORK_QUEUE);
    if (NULL == par->pstHwcRefreshWorkqueue)
    {
        HIFB_ERROR("create hwc refresh work queue failed!\n");
        return HI_FAILURE;
    }

    spin_lock_init(&gs_SyncInfo.lock);

    return HI_SUCCESS;
}


HI_VOID DRV_HIFB_FenceDInit(HIFB_PAR_S *par)
{
    if ( (NULL == gs_SyncInfo.pstTimeline) || (HIFB_LAYER_HD_0 != par->stBaseInfo.u32LayerID))
    {
        return;
    }

    sync_timeline_destroy((struct sync_timeline*)gs_SyncInfo.pstTimeline);
    gs_SyncInfo.pstTimeline = NULL;

    if (NULL == par->pstHwcRefreshWorkqueue)
    {
       return;
    }

    destroy_workqueue(par->pstHwcRefreshWorkqueue);
    par->pstHwcRefreshWorkqueue = NULL;

    return;
}


HI_VOID DRV_HIFB_FENCE_FrameEndCallBack(HI_VOID)
{
    unsigned long lockflag;

    gs_SyncInfo.bFrameHit = HI_FALSE;

    if (NULL == gs_SyncInfo.pstTimeline)
    {
       return;
    }
    spin_lock_irqsave(&gs_SyncInfo.lock,lockflag);
    while(atomic_read(&gs_SyncInfo.UpdateCnt) > 0)
    {
        /** 更新一帧数据 **/
        gs_SyncInfo.bFrameHit = HI_TRUE;
        atomic_dec(&gs_SyncInfo.UpdateCnt);
        sw_sync_timeline_inc(gs_SyncInfo.pstTimeline, 1);
        gs_SyncInfo.u32Timeline++;
    }
	spin_unlock_irqrestore(&gs_SyncInfo.lock,lockflag);
    /** 帧结束中断唤醒，可以刷新了 **/
    gs_SyncInfo.FrameEndFlag = 1;

    wake_up_interruptible(&gs_SyncInfo.FrameEndEvent);

    return;
}


HI_BOOL DRV_HIFB_FENCE_IsRefresh(HI_VOID)
{
    return gs_SyncInfo.bFrameHit;
}


HI_S32 DRV_HIFB_FENCE_Create(HI_VOID)
{
    HI_S32 FenceFd = -1;
    HI_U32 u32FenceValue = 0;
    struct sync_fence *fence = NULL;
    struct sync_pt *pt = NULL;

    if (NULL == gs_SyncInfo.pstTimeline)
    {
        return -EINVAL;
    }

    u32FenceValue = ++gs_SyncInfo.u32FenceValue;

    FenceFd = get_unused_fd();
    if (FenceFd < 0)
    {
        HIFB_ERROR("get_unused_fd failed!\n");
        return FenceFd;
    }

    pt = sw_sync_pt_create(gs_SyncInfo.pstTimeline, u32FenceValue);
    if (NULL == pt)
    {
        return -ENOMEM;
    }

    fence = sync_fence_create(HIFB_FENCE_NAME, pt);
    if (NULL == fence)
    {
        sync_pt_free(pt);
        return -ENOMEM;
    }

    sync_fence_install(fence, FenceFd);

    return FenceFd;
}


HI_VOID DRV_HIFB_WaiteRefreshEnd(HI_VOID)
{
    if (atomic_read(&gs_SyncInfo.UpdateCnt) <= 1)
    {/** only one frame, not waite**/
        return;
    }
    gs_SyncInfo.FrameEndFlag = 0;
    wait_event_interruptible_timeout(gs_SyncInfo.FrameEndEvent, gs_SyncInfo.FrameEndFlag, HZ);

    return;
}


HI_VOID DRV_HIFB_IncRefreshTime(HI_BOOL bLayerEnable)
{
    unsigned long lockflag;

    spin_lock_irqsave(&gs_SyncInfo.lock,lockflag);
    atomic_inc(&gs_SyncInfo.UpdateCnt);
    spin_unlock_irqrestore(&gs_SyncInfo.lock,lockflag);

    if (HI_TRUE == bLayerEnable)
    {/** return frame to gpu at end frame refresh **/
        return;
    }

    if (NULL == gs_SyncInfo.pstTimeline)
    {
       return;
    }

    /** as shallow suspend, layer has benn closed and not has interupts.
     ** but gpu also work, so here should return frame**/
    while (atomic_read(&gs_SyncInfo.UpdateCnt) > 0)
    {
        atomic_dec(&gs_SyncInfo.UpdateCnt);
        sw_sync_timeline_inc(gs_SyncInfo.pstTimeline, 1);
        gs_SyncInfo.u32Timeline++;
    }

    return;
}


HI_S32 DRV_HIFB_FENCE_Waite(struct sync_fence *fence, long timeout)
{
    int err;
    /**
     **这里等待gs_SyncInfo.u32Timeline与gs_SyncInfo.u32FenceValue相等
     **相等之后这个fence线程会被唤醒，否则等超时。这两个值创建的时候都指定了
     **底层会有记录，这里的两个值只做为上面调试使用
     **/
    err = sync_fence_wait(fence, timeout);
    if (err == -ETIME)
    {/**#define MSEC_PER_SEC    1000L 在time.h中**/
        err = sync_fence_wait(fence, 10 * MSEC_PER_SEC);
    }
    if (err < 0)
    {
        HIFB_WARNING("error waiting on fence: 0x%x\n", err);
    }

    return HI_SUCCESS;
}
#endif
