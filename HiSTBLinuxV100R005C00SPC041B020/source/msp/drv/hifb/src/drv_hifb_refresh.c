/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_refresh.c
Version         : Initial Draft
Author          :
Created         : 2016/06/16
Description     :
Function List   :


History         :
Date                Author                Modification
2016/06/16         y00181162              Created file
******************************************************************************/


/*********************************add include here******************************/
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/file.h>
#include <linux/fb.h>

#include "hifb.h"
#include "drv_hifb_osr.h"
#include "drv_hifb_config.h"

#ifdef CONFIG_HIFB_TRANSIT_LOGO_BY_MASK
#include "drv_hifb_logo.h"
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
#include "drv_hifb_fence.h"
#endif


/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
#ifdef CFG_HIFB_FENCE_SUPPORT
typedef struct tagHifbRefreshWorkQueue_S
{
    HIFB_PAR_S *pstPar;
    HIFB_HWC_LAYERINFO_S stLayerInfo;
    struct sync_fence *pSyncfence;
    struct work_struct HwcRefreshWork;
}HIFB_REFRESH_WORKQUEUE_S;
#endif


/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

static HI_S32  HIFB_PanDisplayForStero(struct fb_var_screeninfo *var, struct fb_info *info);
static HI_VOID HIFB_PanDisplayForNoStero(struct fb_var_screeninfo *var, struct fb_info *info);
static HI_VOID HIFB_UpdateDisplayADDR(struct fb_var_screeninfo *var, struct fb_info *info);
static HI_U32  HIFB_GetDispADDR(struct fb_var_screeninfo *var, struct fb_info *info);\


#ifdef CFG_HIFB_PROC_SUPPORT
extern HI_VOID HI_UNF_HIFB_CaptureImgFromLayer(HI_U32 u32LayerID, HI_BOOL bAlphaEnable);
#endif


#ifdef CFG_HIFB_FENCE_SUPPORT
static HI_VOID DRV_HIFB_HwcRefreshWork(struct work_struct *work);
static HI_VOID HIFB_HwcRefresh(HIFB_REFRESH_WORKQUEUE_S *pstWork);
#endif

/******************************* API realization *****************************/


HI_S32 DRV_HIFB_PanDisplay(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (HIFB_REFRESH_MODE_WITH_PANDISPLAY != par->stExtendInfo.enBufMode)
    {
        return HI_SUCCESS;
    }

#ifdef CFG_HIFB_PROC_SUPPORT
    HI_UNF_HIFB_CaptureImgFromLayer(par->stBaseInfo.u32LayerID, HI_FALSE);
#endif

    (HI_TRUE == par->st3DInfo.IsStereo) ? (HIFB_PanDisplayForStero(var,info)) : (HIFB_PanDisplayForNoStero(var,info));

    par->bHwcRefresh = HI_FALSE;

#ifdef CONFIG_HIFB_TRANSIT_LOGO_BY_MASK
    if (HI_FALSE == par->bSetPar)
    {
       DRV_HIFB_CloseLogoByMask(par->stBaseInfo.u32LayerID);
    }
    par->bSetPar = HI_FALSE;
#endif

    return HI_SUCCESS;
}


static HI_S32 HIFB_PanDisplayForStero(struct fb_var_screeninfo *var, struct fb_info *info)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
     HI_S32 Ret = HI_SUCCESS;
     HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
     HIFB_BUFFER_S stDispBuf;
     HIFB_BLIT_OPT_S stBlitOpt;

     if (HIFB_STEREO_SIDEBYSIDE_HALF == par->st3DInfo.StereoMode)
     {
         par->st3DInfo.DataStereoMode = HIFB_STEREO_SIDEBYSIDE_HALF;
     }
     else if(HIFB_STEREO_TOPANDBOTTOM == par->st3DInfo.StereoMode)
     {
         par->st3DInfo.DataStereoMode = HIFB_STEREO_TOPANDBOTTOM;
     }
     else
     {
         par->st3DInfo.DataStereoMode = HIFB_STEREO_FRMPACKING;
     }

     if ( (HIFB_STEREO_FRMPACKING == par->st3DInfo.StereoMode) || (0 == par->st3DInfo.st3DMemInfo.u32StereoMemStart))
     {
         HIFB_UpdateDisplayADDR(var,info);
         return HI_SUCCESS;
     }

     HI_GFX_Memset(&stDispBuf, 0x0, sizeof(stDispBuf));
     stDispBuf.stCanvas.enFmt      = par->stExtendInfo.enColFmt;
     stDispBuf.stCanvas.u32Pitch   = info->fix.line_length;
     stDispBuf.stCanvas.u32PhyAddr = HIFB_GetDispADDR(var,info);
     stDispBuf.stCanvas.u32Width   = info->var.xres;
     stDispBuf.stCanvas.u32Height  = info->var.yres;

     stDispBuf.UpdateRect.x = 0;
     stDispBuf.UpdateRect.y = 0;
     stDispBuf.UpdateRect.w = info->var.xres;
     stDispBuf.UpdateRect.h = info->var.yres;

     par->st3DInfo.st3DSurface.enFmt      = stDispBuf.stCanvas.enFmt;
     par->st3DInfo.st3DSurface.u32Width   = stDispBuf.stCanvas.u32Width;
     par->st3DInfo.st3DSurface.u32Height  = stDispBuf.stCanvas.u32Height;
     par->st3DInfo.st3DSurface.u32PhyAddr = par->st3DInfo.u32DisplayAddr[par->stRunInfo.u32IndexForInt];

     HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
     stBlitOpt.bScale = HI_TRUE;
     stBlitOpt.bBlock = HI_TRUE;
     stBlitOpt.bRegionDeflicker = HI_TRUE;
     if (HIFB_ANTIFLICKER_TDE == par->stBaseInfo.enAntiflickerMode)
     {
         stBlitOpt.enAntiflickerLevel = par->stBaseInfo.enAntiflickerLevel;
     }

     stBlitOpt.bCallBack   = HI_TRUE;
     stBlitOpt.pfnCallBack = (IntCallBack)DRV_HIFB_CallBack;
     stBlitOpt.pParam      = &(par->stBaseInfo.u32LayerID);

     Ret = DRV_HIFB_SteroDataUpdate(par->stBaseInfo.u32LayerID, &stDispBuf, &stBlitOpt);
     if (HI_SUCCESS != Ret)
     {
         HIFB_ERROR("pandisplay config stereo data failure!");
         return HI_FAILURE;
     }

#else
     UNUSED(var);
     UNUSED(info);
#endif
     return HI_SUCCESS;
}



static HI_VOID HIFB_PanDisplayForNoStero(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    HIFB_UpdateDisplayADDR(var,info);

    if ( (var->activate & FB_ACTIVATE_VBL) && (HI_TRUE == par->bVblank))
    {
        DRV_HIFB_WaitVBlank(par->stBaseInfo.u32LayerID);
    }

    return;
}


static HI_VOID HIFB_UpdateDisplayADDR(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    par->stRunInfo.bModifying          = HI_TRUE;
    par->stRunInfo.u32ScreenAddr       = HIFB_GetDispADDR(var,info);
    par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    par->stRunInfo.bModifying          = HI_FALSE;

    return;
}


static HI_U32 HIFB_GetDispADDR(struct fb_var_screeninfo *var, struct fb_info *info)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
     HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
#endif

     HI_U32 u32Align         = 0xf;
     HI_U32 StartDisplayAddr = 0;
     HI_U32 PerPixDepth      = 0;
     HI_U32 PixXOffset       = 0;
     HI_U32 PixYOffset       = 0;

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (par->st3DInfo.IsStereo)
    {
        u32Align = 0x0;
    }
#endif

    PerPixDepth = DRV_HIFB_GetPixDepth(var->bits_per_pixel);
    PixXOffset  = var->xoffset * PerPixDepth;
    PixYOffset  = info->fix.line_length * var->yoffset;

    if ( (info->var.bits_per_pixel == 24) && ( (info->var.xoffset != 0) || (info->var.yoffset != 0) ) )
    {
        StartDisplayAddr = (info->fix.smem_start + PixYOffset + PixXOffset)/16/3;
        StartDisplayAddr = StartDisplayAddr * 16 * 3;
    }
    else
    {
        StartDisplayAddr = (info->fix.smem_start + PixYOffset + PixXOffset) & (~u32Align);
    }

    return StartDisplayAddr;
}



#ifdef CFG_HIFB_FENCE_SUPPORT
HI_S32 DRV_HIFB_HwcRefresh(HIFB_PAR_S* pstPar, HI_VOID *pArgs)
{
    HI_S32 Ret = 1;
    HI_S32 s32FenceFd = -1;
    HIFB_REFRESH_WORKQUEUE_S *pstWork = NULL;

    if ((NULL == pstPar) || (NULL == pArgs))
    {
        HIFB_ERROR("hwc refresh input par is null\n");
        return -EFAULT;
    }

    pstWork = (HIFB_REFRESH_WORKQUEUE_S*)HI_GFX_KMALLOC(HIGFX_FB_ID,sizeof(HIFB_REFRESH_WORKQUEUE_S), GFP_KERNEL);
    if (NULL == pstWork)
    {
        HIFB_ERROR("HI_GFX_KMALLOC pstWork size %d is failed!\n", sizeof(HIFB_REFRESH_WORKQUEUE_S));
        return -EFAULT;
    }

    s32FenceFd = DRV_HIFB_FENCE_Create();
    if (s32FenceFd < 0)
    {
        HIFB_ERROR("DRV_HIFB_FENCE_Create failed! s32Ret = 0x%x\n", s32FenceFd);
        HI_GFX_KFREE(HIGFX_FB_ID,pstWork);
        return -EFAULT;
    }

    if (copy_from_user(&(pstWork->stLayerInfo), pArgs, sizeof(HIFB_HWC_LAYERINFO_S)))
    {
       HIFB_ERROR("copy hwc layerinfo from user failure\n");
       HI_GFX_KFREE(HIGFX_FB_ID,pstWork);
       return -EFAULT;
    }

#ifdef CFG_HIFB_PROC_SUPPORT
    pstPar->stProcInfo.HwcRefreshUnSyncCnt++;
    pstPar->stProcInfo.b3DStatus = pstPar->st3DInfo.IsStereo;
    pstPar->stProcInfo.bHwcRefreshInDeCmpStatus = pstWork->stLayerInfo.bDeCompress;
    pstPar->stProcInfo.HwcRefreshInDispFmt      = (HI_U32)pstWork->stLayerInfo.eFmt;
    pstPar->stProcInfo.HwcRefreshInDispStride   = pstWork->stLayerInfo.u32Stride;
    pstPar->stProcInfo.HwcRefreshInDispAdress   = pstWork->stLayerInfo.u32LayerAddr;
#endif

    pstWork->pstPar = pstPar;
    pstWork->stLayerInfo.s32ReleaseFenceFd = s32FenceFd;

    if (pstWork->stLayerInfo.s32AcquireFenceFd >= 0)
    {
        pstWork->pSyncfence = sync_fence_fdget(pstWork->stLayerInfo.s32AcquireFenceFd);
    }
    else
    {
        pstWork->pSyncfence = NULL;
    }

    if (NULL == pstPar->pstHwcRefreshWorkqueue)
    {
        HIFB_ERROR("the hwc work queue has not been create for layer%d\n", pstPar->stBaseInfo.u32LayerID);
        put_unused_fd(pstWork->stLayerInfo.s32ReleaseFenceFd);
        HI_GFX_KFREE(HIGFX_FB_ID,pstWork);
        return -EFAULT;
    }

    pstWork->stLayerInfo.bStereo = pstPar->st3DInfo.IsStereo;
    if (copy_to_user(pArgs,&(pstWork->stLayerInfo),sizeof(HIFB_HWC_LAYERINFO_S)))
    {
        HIFB_ERROR("copy layerinfo to user failure\n");
        put_unused_fd(pstWork->stLayerInfo.s32ReleaseFenceFd);
        HI_GFX_KFREE(HIGFX_FB_ID,pstWork);
        return -EFAULT;
    }

    pstPar->bEndHwcRefresh = HI_FALSE;
    INIT_WORK(&(pstWork->HwcRefreshWork), DRV_HIFB_HwcRefreshWork);
    queue_work(pstPar->pstHwcRefreshWorkqueue, &(pstWork->HwcRefreshWork));
    if (HI_FALSE == pstPar->bHwcRefreshWithWorkQueue)
    {
       Ret = wait_event_interruptible_timeout(pstPar->WaiteEndHwcRefresh, HI_TRUE == pstPar->bEndHwcRefresh,50 * HZ/1000);
    }
    if (Ret <= 0)
    {
        HIFB_INFO("+++++++warnint:refresh time out\n");
    }

    pstPar->bEndHwcRefresh = HI_FALSE;

    return HI_SUCCESS;
}


static HI_VOID DRV_HIFB_HwcRefreshWork(struct work_struct *work)
{
    HIFB_PAR_S *pstPar  = NULL;
    HIFB_REFRESH_WORKQUEUE_S *pstWork = NULL;

    pstWork = (HIFB_REFRESH_WORKQUEUE_S*)container_of(work, HIFB_REFRESH_WORKQUEUE_S, HwcRefreshWork);
    if (NULL != pstWork->pSyncfence)
    {
        DRV_HIFB_FENCE_Waite(pstWork->pSyncfence, -1);
        sync_fence_put(pstWork->pSyncfence);
        pstWork->pSyncfence = NULL;
    }

    HIFB_HwcRefresh(pstWork);

    pstPar = pstWork->pstPar;
    pstPar->stFrameInfo.u32RefreshFrame++;
    pstPar->bHwcRefresh = HI_TRUE;

    HI_GFX_KFREE(HIGFX_FB_ID,pstWork);

    pstPar->bEndHwcRefresh = HI_TRUE;
    wake_up_interruptible(&pstPar->WaiteEndHwcRefresh);

#ifdef CFG_HIFB_PROC_SUPPORT
    pstPar->stProcInfo.HwcRefreshUnSyncCnt--;
#endif
    return;
}


static HI_VOID HIFB_HwcRefresh(HIFB_REFRESH_WORKQUEUE_S *pstWork)
{
    HI_BOOL bDispEnable = HI_FALSE;
    HI_ULONG LockParFlag = 0;
    HIFB_PAR_S *pstPar  = pstWork->pstPar;
    struct fb_info *info = NULL;

    if (NULL == pstPar)
    {
        HIFB_ERROR("pstPar is null\n");
        return;
    }

    if ((HI_TRUE == pstWork->stLayerInfo.bDeCompress) && (HI_TRUE == pstPar->st3DInfo.IsStereo))
    {
        HIFB_ERROR("3d and decompress state is wrong\n");
        return;
    }

    //DRV_HIFB_WaiteRefreshEnd();

    pstPar->stExtendInfo.enColFmt = pstWork->stLayerInfo.eFmt;
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerDataFmt(pstPar->stBaseInfo.u32LayerID, pstWork->stLayerInfo.eFmt);



    if (HI_TRUE == pstPar->st3DInfo.IsStereo)
    {
        pstPar->bDeCompress = HI_FALSE;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetDeCmpSwitch(pstPar->stBaseInfo.u32LayerID, HI_FALSE);
        info = s_stLayer[pstPar->stBaseInfo.u32LayerID].pstInfo;
        info->var.yoffset = (pstWork->stLayerInfo.u32LayerAddr - info->fix.smem_start)/pstWork->stLayerInfo.u32Stride;
        DRV_HIFB_PanDisplay(&info->var, info);
        g_stDrvAdpCallBackFunction.HIFB_DRV_GetHaltDispStatus(pstPar->stBaseInfo.u32LayerID, &bDispEnable);
        DRV_HIFB_IncRefreshTime(bDispEnable);
        return;
    }

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&LockParFlag);

    pstPar->stRunInfo.bModifying          = HI_TRUE;
    pstPar->bDeCompress                   = pstWork->stLayerInfo.bDeCompress;
    pstPar->stRunInfo.u32ScreenAddr       = pstWork->stLayerInfo.u32LayerAddr;
    pstPar->stExtendInfo.stPos.s32XPos    = pstWork->stLayerInfo.stInRect.x;
    pstPar->stExtendInfo.stPos.s32YPos    = pstWork->stLayerInfo.stInRect.y;
    pstPar->stExtendInfo.u32DisplayWidth  = pstWork->stLayerInfo.stInRect.w;
    pstPar->stExtendInfo.u32DisplayHeight = pstWork->stLayerInfo.stInRect.h;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FENCE;
    pstPar->stRunInfo.bModifying          = HI_FALSE;
    pstPar->u8RefreshCount++;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockParFlag);

    return;
}
#endif
