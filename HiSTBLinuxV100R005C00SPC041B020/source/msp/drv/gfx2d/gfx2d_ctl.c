/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_ctl.c
* Description: Graphic 2D engine ctl code.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include <linux/mutex.h>

#include "hi_gfx2d_type.h"
#include "gfx2d_hal.h"
#include "gfx2d_list.h"
#include "gfx2d_mem.h"
#include "gfx2d_ctl.h"
#include "gfx2d_fence.h"
#include "hi_gfx_comm_k.h"
#include "hi_gfx2d_errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

static HI_U32 gs_u32DevRef = 0;
static DEFINE_MUTEX(gs_stDevRefMutex);

HI_S32 GFX2D_CTL_Init(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32MemSize;

    u32MemSize = GFX2D_CONFIG_GetMemSize();

    s32Ret = GFX2D_MEM_Init(u32MemSize);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    s32Ret = GFX2D_HAL_Init();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR;
    }

    s32Ret = GFX2D_LIST_Init();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }

    return HI_SUCCESS;

ERR1:
    (HI_VOID)GFX2D_HAL_Deinit();
ERR:
    (HI_VOID)GFX2D_MEM_Deinit();

    return s32Ret;
}

HI_S32 GFX2D_CTL_Deinit(HI_VOID)
{
    (HI_VOID)GFX2D_LIST_Deinit();

    (HI_VOID)GFX2D_HAL_Deinit();

    (HI_VOID)GFX2D_MEM_Deinit();

    return HI_SUCCESS;
}

HI_S32 GFX2D_CTL_Open(HI_VOID)
{
    HI_S32 s32Ret;

    mutex_lock(&gs_stDevRefMutex);

    /*dev has opened!*/
    if (gs_u32DevRef > 0)
    {
        gs_u32DevRef++;
        mutex_unlock(&gs_stDevRefMutex);
        return HI_SUCCESS;
    }

    s32Ret = GFX2D_MEM_Open();
    if (HI_SUCCESS != s32Ret)
    {
        mutex_unlock(&gs_stDevRefMutex);
        return s32Ret;
    }

    s32Ret = GFX2D_HAL_Open();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR;
    }

    s32Ret = GFX2D_LIST_Open();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }

#ifdef GFX2D_FENCE_SUPPORT
    GFX2D_FENCE_Open();
#endif

    gs_u32DevRef++;

    mutex_unlock(&gs_stDevRefMutex);

    return HI_SUCCESS;

ERR1:
    (HI_VOID)GFX2D_HAL_Close();
ERR:
    (HI_VOID)GFX2D_MEM_Close();

    mutex_unlock(&gs_stDevRefMutex);

    return s32Ret;
}

HI_S32 GFX2D_CTL_Close(HI_VOID)
{
    mutex_lock(&gs_stDevRefMutex);

    /*dev not opened!*/
    if (0 == gs_u32DevRef)
    {
        mutex_unlock(&gs_stDevRefMutex);
        return HI_SUCCESS;
    }

    gs_u32DevRef--;

    /*still used by some module!*/
    if (gs_u32DevRef > 0)
    {
        mutex_unlock(&gs_stDevRefMutex);
        (HI_VOID)GFX2D_CTL_WaitAllDone(HI_GFX2D_DEV_ID_0, 0);
        return HI_SUCCESS;
    }

    (HI_VOID)GFX2D_CTL_WaitAllDone(HI_GFX2D_DEV_ID_0, 0);
#ifdef GFX2D_FENCE_SUPPORT
    GFX2D_FENCE_Close();
#endif
    (HI_VOID)GFX2D_LIST_Close();
    (HI_VOID)GFX2D_HAL_Close();
    (HI_VOID)GFX2D_MEM_Close();

    mutex_unlock(&gs_stDevRefMutex);

    return HI_SUCCESS;
}

HI_S32 GFX2D_CTL_Compose(const HI_GFX2D_DEV_ID_E enDevId,
                         HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                         HI_GFX2D_SURFACE_S *pstDstSurface,
                         HI_BOOL bSync,
                         const HI_U32 u32Timeout)
{
    HI_S32 s32Ret;
    HI_VOID *pNode = NULL;
    GFX2D_HAL_DEV_TYPE_E enNodeType = GFX2D_HAL_DEV_TYPE_HWC;
    HI_S32 s32ReleaseFenceFd = -1;
    HI_U32 i;

    if ((HI_NULL == pstComposeList)
        || (HI_NULL == pstComposeList->pstCompose)
        || (HI_NULL == pstDstSurface))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Null ptr\n");
        return HI_ERR_GFX2D_NULL_PTR;
    }

#ifdef GFX2D_FENCE_SUPPORT
    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        if (pstComposeList->pstCompose[i].stSurface.s32AcquireFenceFd >= 0
            && (pstComposeList->pstCompose[i].stSurface.enType != HI_GFX2D_SURFACE_TYPE_COLOR))
        {
            GFX2D_FENCE_Wait(pstComposeList->pstCompose[i].stSurface.s32AcquireFenceFd);
        }
    }

    if (pstDstSurface->s32AcquireFenceFd >= 0)
    {
        GFX2D_FENCE_Wait(pstDstSurface->s32AcquireFenceFd);
    }

    if (!bSync)
    {
        s32ReleaseFenceFd = GFX2D_FENCE_Create("gfx2d");
        if (s32ReleaseFenceFd < 0)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "GFX2D_FENCE_Create err!\n");
            return HI_ERR_GFX2D_SYS;
        }
    }
#endif

    s32Ret = GFX2D_HAL_Compose(enDevId, pstComposeList, pstDstSurface, &pNode, &enNodeType);
    if (HI_SUCCESS != s32Ret)
    {
#ifdef GFX2D_FENCE_SUPPORT
        if (s32ReleaseFenceFd >= 0)
        {
            GFX2D_FENCE_Destroy(s32ReleaseFenceFd);
        }
#endif
        return s32Ret;
    }

    s32Ret = GFX2D_LIST_SubNode(enDevId, pNode, enNodeType, bSync, u32Timeout, s32ReleaseFenceFd);
    if (HI_SUCCESS != s32Ret)
    {
#ifdef GFX2D_FENCE_SUPPORT
        if (s32ReleaseFenceFd >= 0)
        {
            GFX2D_FENCE_Destroy(s32ReleaseFenceFd);
        }
#endif
        return s32Ret;
    }

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        pstComposeList->pstCompose[i].stSurface.s32ReleaseFenceFd = s32ReleaseFenceFd;
    }

    pstDstSurface->s32ReleaseFenceFd = s32ReleaseFenceFd;

    return HI_SUCCESS;
}


#if 0
HI_S32 GFX2D_CTL_ComposeAsync(const HI_GFX2D_DEV_ID_E enDevId,
                              HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                              HI_GFX2D_SURFACE_S *pstDstSurface,
                              GFX2D_FUNC_CB pFunc,
                              HI_VOID *pParam)
)
{
    HI_S32 s32Ret;
    HI_VOID *pNode = NULL;
    GFX2D_HAL_DEV_TYPE_E enNodeType;

    s32Ret = GFX2D_HAL_Compose(enDevId, pstComposeList, pstDstSurface, &pNode, &enNodeType);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = GFX2D_LIST_SubNode(enDevId, pNode, enNodeType, HI_FALSE, 0);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 GFX2D_CTL_WaitAllDone(const HI_GFX2D_DEV_ID_E enDevId,
       const HI_U32 u32Timeout)
{
    return GFX2D_LIST_WaitAllDone(enDevId, u32Timeout);
}

HI_S32 GFX2D_CTL_Resume(HI_VOID)
{
    return GFX2D_LIST_Resume();
}

#ifndef GFX2D_PROC_UNSUPPORT
HI_S32 GFX2D_CTL_ReadProc(struct seq_file *p, HI_VOID *v)
{

    if (NULL == p)
    {
       return HI_FAILURE;
    }

    GFX2D_MEM_ReadProc(p, v);

    GFX2D_LIST_ReadProc(p, v);

    GFX2D_HAL_ReadProc(p, v);

#ifdef GFX2D_FENCE_SUPPORT
    GFX2D_FENCE_ReadProc(p, v);
#endif

    return HI_SUCCESS;
}

HI_S32 GFX2D_CTL_WriteProc(struct file *file, const char __user *buf,
                                   size_t count, loff_t *ppos)
{
    (HI_VOID)GFX2D_MEM_WriteProc(file, buf, count, ppos);

    (HI_VOID)GFX2D_LIST_WriteProc(file, buf, count, ppos);

    (HI_VOID)GFX2D_HAL_WriteProc(file, buf, count, ppos);

    return HI_SUCCESS;
}
#endif

HI_BOOL GFX2D_CTL_CheckOpen(HI_VOID)
{
    return (gs_u32DevRef > 0) ? HI_TRUE : HI_FALSE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
