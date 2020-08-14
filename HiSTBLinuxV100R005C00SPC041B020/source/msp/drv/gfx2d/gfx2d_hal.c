/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_hal.c
* Description: Graphic 2D engine hal code.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include <linux/fs.h>

#include "hi_gfx2d_type.h"
#include "gfx2d_hal.h"
#include "gfx2d_hal_hwc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

HI_S32 GFX2D_HAL_Init(HI_VOID)
{
    return GFX2D_HAL_HWC_Init();
}

HI_S32 GFX2D_HAL_Deinit(HI_VOID)
{
    return GFX2D_HAL_HWC_Deinit();
}

HI_S32 GFX2D_HAL_Open(HI_VOID)
{
    return GFX2D_HAL_HWC_Open();
}

HI_S32 GFX2D_HAL_Close(HI_VOID)
{
    return GFX2D_HAL_HWC_Close();
}

HI_S32 GFX2D_HAL_Compose(const HI_GFX2D_DEV_ID_E enDevId,
                         const HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                         const HI_GFX2D_SURFACE_S *pstDstSurface,
                         HI_VOID **ppNode,
                         GFX2D_HAL_DEV_TYPE_E *penNodeType)
{
    return GFX2D_HAL_HWC_Compose(enDevId, pstComposeList, pstDstSurface, ppNode, penNodeType);
}

HI_VOID GFX2D_HAL_GetNodeOps(const GFX2D_HAL_DEV_TYPE_E enDevType,
                             GFX2D_HAL_NODE_OPS_S **ppstNodeOps)
{
    if (enDevType == GFX2D_HAL_DEV_TYPE_HWC)
    {
        GFX2D_HAL_HWC_GetNodeOps(ppstNodeOps);
    }

    return;
}

HI_S32 GFX2D_HAL_GetIntStatus(const GFX2D_HAL_DEV_TYPE_E enDevType)
{
    return GFX2D_HAL_HWC_GetIntStatus();
}

HI_U32 GFX2D_HAL_GetIsrNum(const HI_GFX2D_DEV_ID_E enDevId,
                           const GFX2D_HAL_DEV_TYPE_E enDevType)
{
    return GFX2D_HAL_HWC_GetIsrNum();
}

HI_U32 GFX2D_HAL_GetBaseAddr(const HI_GFX2D_DEV_ID_E enDevId,
                             const GFX2D_HAL_DEV_TYPE_E enDevType)
{
    return GFX2D_HAL_HWC_GetBaseAddr();
}


#ifndef GFX2D_PROC_UNSUPPORT
HI_VOID GFX2D_HAL_ReadProc(struct seq_file *p, HI_VOID *v)
{
    GFX2D_HAL_HWC_ReadProc(p, v);

    return;
}

HI_S32 GFX2D_HAL_WriteProc(struct file *file, const char __user *buf, \
                           size_t count, loff_t *ppos)
{
    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
