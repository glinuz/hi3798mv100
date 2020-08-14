/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : gfx2d_fence.c
Version             : Initial Draft
Author              :
Created             : 2015/08/11
Description         :
Function List       :
History             :
Date                       Author                   Modification
2015/08/11                 z00141204                Created file
******************************************************************************/

/*********************************add include here******************************/
#ifdef GFX2D_FENCE_SUPPORT

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

#include "gfx2d_fence.h"
#include "sw_sync.h"
#include "hi_gfx_comm_k.h"

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
static HI_U32 gs_u32FenceValue = 0;
static HI_U32 gs_u32TimelineValue  = 0;
static struct sw_sync_timeline *gs_pstTimeline = NULL;

/******************************* API declaration *****************************/

HI_VOID GFX2D_FENCE_Open(HI_VOID)
{
    if (NULL == gs_pstTimeline)
    {
        gs_pstTimeline = sw_sync_timeline_create("gfx2d");
    }

    return;
}

HI_VOID GFX2D_FENCE_Close(HI_VOID)
{
    if (gs_pstTimeline)
    {
        sync_timeline_destroy(&gs_pstTimeline->obj);
        gs_pstTimeline = NULL;
        gs_u32FenceValue = 0;
        gs_u32TimelineValue = 0;
    }

    return;
}

HI_S32 GFX2D_FENCE_Create(const char *name)
{
    HI_S32 fd;
    struct sync_fence *fence = NULL;
    struct sync_pt *pt = NULL;

    if (NULL == gs_pstTimeline)
    {
        return HI_FAILURE;
    }

    /**
     **获取可用的文件节点
     **/
    fd = get_unused_fd();
    if (fd < 0)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "get_unused_fd failed!\n");
        return fd;
    }

    /**
     **创建同步节点
     **/
    pt = sw_sync_pt_create(gs_pstTimeline, ++gs_u32FenceValue);
    if (NULL == pt)
    {
        gs_u32FenceValue--;
        put_unused_fd(fd);
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "sw_sync_pt_create failed!\n");
        return -ENOMEM;
    }

    fence = sync_fence_create(name, pt);
    if (fence == NULL)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "sync_fence_create failed!\n");
        sync_pt_free(pt);
        put_unused_fd(fd);
        return -ENOMEM;
    }

    sync_fence_install(fence, fd);

    return fd;

}

HI_VOID GFX2D_FENCE_Destroy(HI_S32 fd)
{
    put_unused_fd(fd);

    return;
}

#define GFX2D_FENCE_TIMEOUT 3000

HI_S32 GFX2D_FENCE_Wait(HI_S32 fd)
{
    HI_S32 s32Ret;
    struct sync_fence *fence = NULL;

    fence = sync_fence_fdget(fd);
    if (fence == NULL)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "sync_fence_fdget failed!\n");
        return HI_FAILURE;
    }

    s32Ret = sync_fence_wait(fence, GFX2D_FENCE_TIMEOUT);
    if (s32Ret < 0)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "error waiting on fence: 0x%x\n", s32Ret);
    }

    sync_fence_put(fence);

    return 0;
}

HI_S32 GFX2D_FENCE_WakeUp(HI_VOID)
{
    sw_sync_timeline_inc(gs_pstTimeline, 1);
    gs_u32TimelineValue++;

    return HI_SUCCESS;
}

HI_VOID GFX2D_FENCE_ReadProc(struct seq_file *p, HI_VOID *v)
{
    SEQ_Printf(p, "---------------------GFX2D Fence Info--------------------\n");
    SEQ_Printf(p, "FenceValue\t:%u\n", gs_u32FenceValue);
    SEQ_Printf(p, "TimeLineValue\t:%u\n", gs_u32TimelineValue);

    return;
}

#endif
