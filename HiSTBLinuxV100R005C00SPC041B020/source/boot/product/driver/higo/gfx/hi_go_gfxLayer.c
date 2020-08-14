/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_gfxlayer.c
Version             : Initial Draft
Author              :
Created             : 2015/07/02
Description         :
                       CNcomment: 图层操作适配层 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2015/07/02                  y00181162               Created file
******************************************************************************/


/*********************************add include here******************************/

#include "hi_go_gfxlayer.h"
#include "hi_go_logo.h"
#include "drv_hifb_adp.h"
#include "drv_hifb_wbc.h"
#include "drv_disp_ext.h"

/***************************** Macro Definition ******************************/

#define WBC_TC_WIDTH 720

/*************************** Structure Definition ****************************/


typedef enum hiFB_COLOR_FMT_E
{
    HI_FB_FMT_RGB565 = 0,
    HI_FB_FMT_RGB888,        /**<  RGB888 24bpp */

    HI_FB_FMT_KRGB444,       /**<  RGB444 16bpp */
    HI_FB_FMT_KRGB555,       /**<  RGB555 16bpp */
    HI_FB_FMT_KRGB888,       /**<  RGB888 32bpp */

    HI_FB_FMT_ARGB4444,      /**< ARGB4444      */
    HI_FB_FMT_ARGB1555,      /**< ARGB1555      */
    HI_FB_FMT_ARGB8888,      /**< ARGB8888      */
    HI_FB_FMT_ARGB8565,      /**< ARGB8565      */

    HI_FB_FMT_RGBA4444,      /**< ARGB4444      */
    HI_FB_FMT_RGBA5551,      /**< RGBA5551      */
    HI_FB_FMT_RGBA5658,      /**< RGBA5658      */
    HI_FB_FMT_RGBA8888,      /**< RGBA8888      */

    HI_FB_FMT_BGR565,        /**< BGR565        */
    HI_FB_FMT_BGR888,        /**< BGR888        */
    HI_FB_FMT_ABGR4444,      /**< ABGR4444      */
    HI_FB_FMT_ABGR1555,      /**< ABGR1555      */
    HI_FB_FMT_ABGR8888,      /**< ABGR8888      */
    HI_FB_FMT_ABGR8565,      /**< ABGR8565      */
    HI_FB_FMT_KBGR444,       /**< BGR444 16bpp  */
    HI_FB_FMT_KBGR555,       /**< BGR555 16bpp  */
    HI_FB_FMT_KBGR888,       /**< BGR888 32bpp  */

    HI_FB_FMT_1BPP,          /**<  clut1        */
    HI_FB_FMT_2BPP,          /**<  clut2        */
    HI_FB_FMT_4BPP,          /**<  clut4        */
    HI_FB_FMT_8BPP,          /**< clut8         */
    HI_FB_FMT_ACLUT44,       /**< AClUT44       */
    HI_FB_FMT_ACLUT88,       /**< ACLUT88       */
    HI_FB_FMT_PUYVY,         /**< UYVY          */
    HI_FB_FMT_PYUYV,         /**< YUYV          */
    HI_FB_FMT_PYVYU,         /**< YVYU          */
    HI_FB_FMT_YUV888,        /**< YUV888        */
    HI_FB_FMT_AYUV8888,      /**< AYUV8888      */
    HI_FB_FMT_YUVA8888,      /**< YUVA8888      */
    HI_FB_FMT_BUTT
}HI_FB_COLOR_FMT_E;

/********************** Global Variable declaration **************************/

static HI_GFX_DISPLAY_INFO_S gs_stDisplayInfo[HI_GFX_LAYER_MAXCOUNT];

/**********************       API realization       **************************/


/***************************************************************************************
* func          : GFX_ADP_LAYERID_TO_GfxId
* description   : CNcomment: 获取VDP图形层ID CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 GFX_ADP_LAYERID_TO_GfxId(HI_GO_LAYER_ID_E eLayerID)
{
    if (HI_GO_LAYER_HD_0 == eLayerID)
    {
        return 0;
    }
    else if(HI_GO_LAYER_HD_1 == eLayerID)
    {
        return 1;
    }
    else if(HI_GO_LAYER_HD_2 == eLayerID)
    {
        return 2;
    }
    else if(HI_GO_LAYER_HD_3 == eLayerID)
    {
        return 3;
    }
    else if(HI_GO_LAYER_SD_0 == eLayerID)
    {
        return 4;
    }
    else if(HI_GO_LAYER_SD_1 == eLayerID)
    {
        return 5;
    }
    else
    {
        return HI_FAILURE;
    }
}

/***************************************************************************************
* func          : GFX_ADP_LAYERID_TO_GpId
* description   : CNcomment: 获取GP的ID CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 GFX_ADP_LAYERID_TO_GpId(HI_GO_LAYER_ID_E eLayerID)
{
    if (eLayerID <= HI_GO_LAYER_HD_3)
    {
        return 0;
    }
    else if (eLayerID <= HI_GO_LAYER_SD_1)
    {
        return 1;
    }
    else
    {
        return HI_FAILURE;
    }
}


/*****************************************************************************
* func            : GFX_ADP_ParaAlign
* description     : 参数对齐
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_U32 GFX_ADP_ParaAlign(HI_U32 x, HI_U32 a)
{
    if (!a)
    {
        return x;
    }
    else
    {
        return ( (( x + (a-1) ) / a ) * a);
    }
}

/*****************************************************************************
* func            : Win_ReviseOutRect
* description     : 改变输出大小，该接口在optm_hifb.c中也有调用，使用extern
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID Win_ReviseOutRect(const HI_RECT_S *tmp_virtscreen,          \
                          const HI_DRV_DISP_OFFSET_S *stOffsetInfo, \
                          const HI_RECT_S *stFmtResolution,         \
                          const HI_RECT_S *stPixelFmtResolution,    \
                          HI_RECT_S *stToBeRevisedRect,             \
                          HI_RECT_S *stRevisedRect)
{
    HI_U32 width_ratio   = 0;
    HI_U32 height_ratio  = 0;
    HI_U32 zmeDestWidth  = 0;
    HI_U32 zmeDestHeight = 0;
    HI_DRV_DISP_OFFSET_S tmp_offsetInfo;

    tmp_offsetInfo = *stOffsetInfo;

    if ( (stFmtResolution->s32Width * 2) == stPixelFmtResolution->s32Width)
    {
        tmp_offsetInfo.u32Left  *= 2;
        tmp_offsetInfo.u32Right *= 2;
    }

    zmeDestWidth = (stPixelFmtResolution->s32Width - tmp_offsetInfo.u32Left - tmp_offsetInfo.u32Right);
    zmeDestHeight = (stPixelFmtResolution->s32Height - tmp_offsetInfo.u32Top - tmp_offsetInfo.u32Bottom);

    width_ratio  = zmeDestWidth  * 100 /(tmp_virtscreen->s32Width);
    height_ratio = zmeDestHeight * 100 /(tmp_virtscreen->s32Height);

    if (tmp_virtscreen->s32Width != stToBeRevisedRect->s32Width)
    {
        stRevisedRect->s32Width = (stToBeRevisedRect->s32Width * width_ratio) / 100;
    }
    else
    {
        stRevisedRect->s32Width = zmeDestWidth;
    }

    if (tmp_virtscreen->s32Height != stToBeRevisedRect->s32Height)
    {
        stRevisedRect->s32Height = (stToBeRevisedRect->s32Height * height_ratio) / 100;
    }
    else
    {
        stRevisedRect->s32Height = zmeDestHeight;
    }


    stRevisedRect->s32X = (stToBeRevisedRect->s32X * width_ratio) /100 + tmp_offsetInfo.u32Left;
    stRevisedRect->s32Y = (stToBeRevisedRect->s32Y  * height_ratio) /100 + tmp_offsetInfo.u32Top;

    stRevisedRect->s32X      = GFX_ADP_ParaAlign(stRevisedRect->s32X, 2);
    stRevisedRect->s32Y      = GFX_ADP_ParaAlign(stRevisedRect->s32Y , 2);
    stRevisedRect->s32Width  = GFX_ADP_ParaAlign(stRevisedRect->s32Width, 2);
    stRevisedRect->s32Height = GFX_ADP_ParaAlign(stRevisedRect->s32Height, 2);

    return;
}


static HI_S32 GFX_ADP_GetDisplayInfo(HI_GO_SURFACE_S *pstSurface,HI_GFX_DISPLAY_INFO_S *pDisplayInfo, HI_BOOL Support4KLogo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BufferSize = 0;
    HI_DISP_DISPLAY_INFO_S pstInfo = {0};
    MMZ_BUFFER_S stDisplayBuf;
    HI_GO_RECT_S stOutRect = {0};

    if (NULL == pstSurface)
    {
        return HI_FAILURE;
    }

    if (pstSurface->enLayerID <= HI_GO_LAYER_HD_3)
    {
        HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_1, &pstInfo);
    }
    else
    {
        HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &pstInfo);
    }

    Win_ReviseOutRect(&pstInfo.stVirtaulScreen,      \
                      &pstInfo.stOffsetInfo,         \
                      &pstInfo.stFmtResolution,      \
                      &pstInfo.stPixelFmtResolution, \
                      &pstInfo.stVirtaulScreen,      \
                      (HI_RECT_S*)(&stOutRect));

    memset(pDisplayInfo, 0, sizeof(HI_GFX_DISPLAY_INFO_S));

    pDisplayInfo->stAlpha.bAlphaEnable  = HI_TRUE;
    pDisplayInfo->stAlpha.bAlphaChannel = HI_FALSE;
    pDisplayInfo->stAlpha.u8Alpha0      = 0x00;
    pDisplayInfo->stAlpha.u8Alpha1      = 0xff;
    pDisplayInfo->stAlpha.u8GlobalAlpha = 0xff;

    pDisplayInfo->stInRect.x  = pstInfo.stVirtaulScreen.s32X;
    pDisplayInfo->stInRect.y  = pstInfo.stVirtaulScreen.s32Y;
    pDisplayInfo->stInRect.w  = pstInfo.stVirtaulScreen.s32Width;
    pDisplayInfo->stInRect.h  = pstInfo.stVirtaulScreen.s32Height;
    pDisplayInfo->stOutRect.x  = stOutRect.x;
    pDisplayInfo->stOutRect.y  = stOutRect.y;
    pDisplayInfo->stOutRect.w  = stOutRect.w;
    pDisplayInfo->stOutRect.h  = stOutRect.h;
    pstSurface->stRect.x  = pDisplayInfo->stInRect.x;
    pstSurface->stRect.y  = pDisplayInfo->stInRect.y;
    pstSurface->stRect.w  = pDisplayInfo->stInRect.w;
    pstSurface->stRect.h  = pDisplayInfo->stInRect.h;

#if 0
    HI_GFX_Print("\n===========================================================\n");
    HI_GFX_Print("========%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    HI_GFX_Print("disp in rect  [%d %d %d %d]\n",pDisplayInfo->stInRect.x,pDisplayInfo->stInRect.y,pDisplayInfo->stInRect.w,pDisplayInfo->stInRect.h);
    HI_GFX_Print("disp out rect [%d %d %d %d]\n",pDisplayInfo->stOutRect.x,pDisplayInfo->stOutRect.y,pDisplayInfo->stOutRect.w,pDisplayInfo->stOutRect.h);
    HI_GFX_Print("this outrect should equal to logo to app's src rect\n");
    HI_GFX_Print("logo virtual screen and offset should equal to app's\n");
    HI_GFX_Print("===========================================================\n");
#endif
    if (HI_TRUE == Support4KLogo)
    {
        pDisplayInfo->stInRect.x  = 0;
        pDisplayInfo->stInRect.y  = 0;
        pDisplayInfo->stInRect.w  = pstInfo.stFmtResolution.s32Width;
        pDisplayInfo->stInRect.h  = pstInfo.stFmtResolution.s32Height;
        pDisplayInfo->stOutRect.x = pDisplayInfo->stInRect.x;
        pDisplayInfo->stOutRect.y = pDisplayInfo->stInRect.y;
        pDisplayInfo->stOutRect.w = pDisplayInfo->stInRect.w;
        pDisplayInfo->stOutRect.h = pDisplayInfo->stInRect.h;
        pstSurface->stRect.x  = stOutRect.x;
        pstSurface->stRect.y  = stOutRect.y;
        pstSurface->stRect.w  = stOutRect.w;
        pstSurface->stRect.h  = stOutRect.h;
#if 0
        HI_GFX_Print("\n===========================================================\n");
        HI_GFX_Print("========%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
        HI_GFX_Print("=======let input rect equal to out rect, gp0 and gp1 not need zme\n");
        HI_GFX_Print("disp in rect  [%d %d %d %d]\n",pDisplayInfo->stInRect.x,pDisplayInfo->stInRect.y,pDisplayInfo->stInRect.w,pDisplayInfo->stInRect.h);
        HI_GFX_Print("disp out rect [%d %d %d %d]\n",pDisplayInfo->stOutRect.x,pDisplayInfo->stOutRect.y,pDisplayInfo->stOutRect.w,pDisplayInfo->stOutRect.h);
        HI_GFX_Print("===========================================================\n");
#endif
    }

    pstSurface->u32Width[0]     = pDisplayInfo->stInRect.w;
    pstSurface->u32Height[0]    = pDisplayInfo->stInRect.h;
    switch (pstSurface->enPixelFormat)
    {
        case HIGO_PF_8888:
             pstSurface->u32Stride[0] = (pDisplayInfo->stInRect.w * 4 + 0xf) & 0xfffffff0;
             break;
        case HIGO_PF_0888:
             pstSurface->u32Stride[0] = (pDisplayInfo->stInRect.w * 3 + 0xf) & 0xfffffff0;
             break;
        case HIGO_PF_565:
        case HIGO_PF_1555:
             pstSurface->u32Stride[0] = (pDisplayInfo->stInRect.w * 2 + 0xf) & 0xfffffff0;
             break;
        default:
             break;
    }
    u32BufferSize = pstSurface->u32Stride[0] * pDisplayInfo->stInRect.h;

    if (pstSurface->enLayerID <= HI_GO_LAYER_HD_3)
    {
        s32Ret = HI_DRV_PDM_AllocReserveMem(DISPLAY_BUFFER_HD, u32BufferSize, &stDisplayBuf.u32StartPhyAddr);
    }
    else
    {
        s32Ret = HI_DRV_PDM_AllocReserveMem(DISPLAY_BUFFER_SD, u32BufferSize, &stDisplayBuf.u32StartPhyAddr);
    }

    pstSurface->pPhyAddr[0] = (HI_CHAR*)stDisplayBuf.u32StartPhyAddr;
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pDisplayInfo->u32Stride     = pstSurface->u32Stride[0];
    pDisplayInfo->u32ScreenAddr = stDisplayBuf.u32StartPhyAddr;

    return HI_SUCCESS;
}

/*****************************************************************************
* func            : GFX_ADP_LayerToHifbFmt
* description     : 将layer像素格式转层图层像素格式
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_VOID GFX_ADP_LayerToHifbFmt(HIGO_PF_E SrcFmt,HI_FB_COLOR_FMT_E *DstFmt)
{
    switch(SrcFmt)
    {
        case HIGO_PF_1555:
            *DstFmt = HI_FB_FMT_ARGB1555;
            break;
        case HIGO_PF_565:
            *DstFmt = HI_FB_FMT_RGB565;
            break;
        case HIGO_PF_8888:
            *DstFmt = HI_FB_FMT_ARGB8888;
            break;
        case HIGO_PF_0888:
            *DstFmt = HI_FB_FMT_RGB888;
            break;
        default:
            *DstFmt = HI_FB_FMT_ARGB8888;
            break;
    }
    return;
}


HI_S32 HI_GFX_ADP_CreateLayer(HI_GO_SURFACE_S *pstGfxSurface, HI_BOOL Support4KLogo)
{

    HI_S32 s32Ret    = HI_SUCCESS;
    HI_S32 s32DataId = 0;
    HI_S32 s32GPId   = 0;
    HI_FB_COLOR_FMT_E enLayerFmt;
    HI_DISP_DISPLAY_INFO_S stInfo = {0};
    HI_GFX_DISPLAY_INFO_S *pDisplayInfo = NULL;

    if (NULL == pstGfxSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32DataId = GFX_ADP_LAYERID_TO_GfxId(pstGfxSurface->enLayerID);
    if (HI_FAILURE == s32DataId)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32GPId  = GFX_ADP_LAYERID_TO_GpId(pstGfxSurface->enLayerID);
    if (HI_FAILURE == s32GPId)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pDisplayInfo = gs_stDisplayInfo + s32DataId;

    s32Ret = GFX_ADP_GetDisplayInfo(pstGfxSurface,pDisplayInfo, Support4KLogo);
    if (s32Ret != HI_SUCCESS)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    GFX_ADP_LayerToHifbFmt(pstGfxSurface->enPixelFormat, &enLayerFmt);

    OPTM_GfxInit            ();
    OPTM_GfxOpenLayer       (s32DataId);
    OPTM_GfxSetLayerDataFmt (s32DataId,  (enLayerFmt));
    OPTM_GfxSetLayerStride  (s32DataId,  (pDisplayInfo->u32Stride));
    OPTM_GfxSetLayerAddr    (s32DataId,  (pDisplayInfo->u32ScreenAddr));
    OPTM_GfxSetLayerAlpha   (s32DataId,  (HIFB_ALPHA_S*)(&(pDisplayInfo->stAlpha)));
    OPTM_GfxSetLayerRect    (s32DataId,  (const HIFB_RECT*)(&(pDisplayInfo->stInRect)));
    OPTM_GfxSetGpRect       (s32GPId,    (const HIFB_RECT*)(&(pDisplayInfo->stInRect)));
    OPTM_GfxSetDispFMTSize  (s32GPId,    (const HI_RECT_S*)(&(pDisplayInfo->stOutRect)));
    OPTM_GpInitFromDisp     (s32GPId, Support4KLogo);

    if (HI_FALSE == HIFB_ADP_GetIsGfxWorkAtWbc())
    {
       return HI_SUCCESS;
    }


    HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &stInfo);
    if (stInfo.stFmtResolution.s32Width > WBC_TC_WIDTH)
    {
        OPTM_GFX_SetTCFlag(HI_TRUE);
    }
    HIFB_WBC_InitLayer     (HI_GO_LAYER_SD_0);
    OPTM_GpInitFromDisp    (OPTM_GFX_GP_1, Support4KLogo);

    return HI_SUCCESS;
}

/*****************************************************************************
* func            : HI_GFX_ADP_GetLayerInfo
* description     : 获取图层信息
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_ADP_GetLayerInfo(HI_GO_SURFACE_S *pstGfxSurface)
{
    HIFB_WBC_DATA_S stWbcInfo;
    HI_DISP_DISPLAY_INFO_S pstInfo;
    HI_GO_RECT_S stOutRect = {0};


    memset(&stWbcInfo, 0x0, sizeof(HIFB_WBC_DATA_S));
    memset(&pstInfo, 0x0, sizeof(HI_DISP_DISPLAY_INFO_S));

    HIFB_WBC_BOOT_GetWbcInfo  (&stWbcInfo);

    HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &pstInfo);
    Win_ReviseOutRect(&pstInfo.stVirtaulScreen,      \
                      &pstInfo.stOffsetInfo,         \
                      &pstInfo.stFmtResolution,      \
                      &pstInfo.stPixelFmtResolution, \
                      &pstInfo.stVirtaulScreen,      \
                      (HI_RECT_S*)(&stOutRect));

    pstGfxSurface->stRect.x     = 0;
    pstGfxSurface->stRect.y     = 0;
    pstGfxSurface->stRect.w     = stOutRect.w;
    pstGfxSurface->stRect.h     = stOutRect.h;
    pstGfxSurface->u32Width[0]  = stOutRect.w;
    pstGfxSurface->u32Height[0] = stOutRect.h;

    pstGfxSurface->u32Stride[0] = stWbcInfo.u32Stride;
    pstGfxSurface->pPhyAddr[0]  = (HI_CHAR*)stWbcInfo.u32ReadBufAddr;
#if 0
    HI_GFX_Print("\n===========================================================\n");
    HI_GFX_Print("========%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    HI_GFX_Print("sd layer rect [%d %d %d %d]\n",pstGfxSurface->stRect.x,pstGfxSurface->stRect.y,pstGfxSurface->stRect.w,pstGfxSurface->stRect.h);
    HI_GFX_Print("sd layer surface [%d %d]\n",   pstGfxSurface->u32Width[0],pstGfxSurface->u32Height[0]);
    HI_GFX_Print("sd layer surface stride %d\n", pstGfxSurface->u32Stride[0]);
    HI_GFX_Print("sd layer surface ddr 0x%x\n",  stWbcInfo.u32ReadBufAddr);
    HI_GFX_Print("this outrect should equal to logo to app's src rect\n");
    HI_GFX_Print("logo virtual screen and offset should equal to app's\n");
    HI_GFX_Print("===========================================================\n");
#endif
    return;
}


HI_S32 HI_GFX_ADP_RefreshLayer(HI_GO_SURFACE_S *pstGfxSurface, HI_BOOL Support4KLogo)
{
    HI_S32 s32DataId = 0;

    if (NULL == pstGfxSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32DataId = GFX_ADP_LAYERID_TO_GfxId(pstGfxSurface->enLayerID);
    if (HI_FAILURE == s32DataId)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    OPTM_GfxSetEnable  (s32DataId, HI_TRUE);
    OPTM_GfxUpLayerReg (s32DataId);

    if (HI_FALSE == HIFB_ADP_GetIsGfxWorkAtWbc())
    {
       return HI_SUCCESS;
    }

    HIFB_WBC_BOOT_ReFresh   (Support4KLogo);
    OPTM_GfxUpLayerReg      (HI_GO_LAYER_SD_0);

    return HI_SUCCESS;
}
/*****************************************************************************
* func            : HI_GFX_ADP_DestoryLayer
* description     : 销毁图层
* in              : 图层参数
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GFX_ADP_DestoryLayer(HI_GO_SURFACE_S *pstGfxSurface)
{
    HI_S32 s32DataId = 0;

    if (NULL == pstGfxSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32DataId = GFX_ADP_LAYERID_TO_GfxId(pstGfxSurface->enLayerID);
    if (HI_FAILURE == s32DataId)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    OPTM_GfxCloseLayer(s32DataId);

    return HI_SUCCESS;
}
/*****************************************************************************
* func            : HI_GO_Init
* description     : logo初始化
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GO_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = HI_GO_BLIT_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
