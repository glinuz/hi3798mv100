/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : drv_hifb_blit.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/


/***************************** add include here*********************************/

#include "hi_module.h"
#include "hi_drv_module.h"
#include "drv_tde_ext.h"
#include "hifb_drv.h"
#include "drv_hifb_mem.h"
#include "drv_hifb_blit.h"
#include "drv_hifb_config.h"

/***************************** Macro Definition ********************************/


/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

static IntCallBack s_pTdeCallBack;


/**
 ** tde support which color format
 **/
static HI_BOOL s_bTdeColFmt[HIFB_FMT_BUTT] =
                {
                    HI_TRUE,    /* HIFB_FMT_RGB565 */
                    HI_TRUE,    /* HIFB_FMT_RGB888 */
                    HI_TRUE,    /* HIFB_FMT_KRGB444 */
                    HI_TRUE,    /* HIFB_FMT_KRGB555 */

                    HI_FALSE,   /* HIFB_FMT_KRGB888 */
                    HI_TRUE,    /* HIFB_FMT_ARGB4444 */
                    HI_TRUE,    /* HIFB_FMT_ARGB1555 */
                    HI_TRUE,    /* HIFB_FMT_ARGB8888 */

                    HI_TRUE,    /* HIFB_FMT_ARGB8565 */
                    HI_TRUE,   /* HIFB_FMT_RGBA4444 */
                    HI_TRUE,   /* HIFB_FMT_RGBA5551 */
                    HI_TRUE,   /* HIFB_FMT_RGBA5658 */

                    HI_TRUE,   /* HIFB_FMT_RGBA8888 */
                    HI_TRUE,   /**< BGR565 */
                    HI_TRUE,   /**< BGR888 */
                    HI_TRUE,   /**< ABGR4444 */

                    HI_TRUE,   /**< ABGR1555 */
                    HI_TRUE,   /**< ABGR8888 */
                    HI_TRUE,   /**< ABGR8565 */
                    HI_TRUE,   /**< BGR444 16bpp */

                    HI_TRUE,   /**< BGR555 16bpp */
                    HI_TRUE,   /**< BGR888 32bpp */
                    HI_TRUE,   /* HIFB_FMT_1BPP */
                    HI_TRUE,   /* HIFB_FMT_2BPP */

                    HI_TRUE,   /* HIFB_FMT_4BPP */
                    HI_TRUE,   /* HIFB_FMT_8BPP */
                    HI_TRUE,   /* HIFB_FMT_ACLUT44 */
                    HI_TRUE,   /* HIFB_FMT_ACLUT88 */

                };


/***************************** API forward declarations ************************/
static HI_S32  HIFB_DRV_QuickCopy(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg);
static HI_S32  HIFB_DRV_QuickCopyEx(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg);
static HI_S32  HIFB_DRV_Blit(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt, HI_BOOL bRefreshScreen);
static HI_S32  HIFB_DRV_MbBlitEx (HIFB_BLIT_BUFFER_S *pSrcImg, HIFB_BLIT_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt);
static HI_S32  HIFB_DRV_CalScaleRect(const TDE2_RECT_S* pstSrcRect, const TDE2_RECT_S* pstDstRect, TDE2_RECT_S* pstRectInSrc, TDE2_RECT_S* pstRectInDst);
static HI_S32  HIFB_DRV_ClearRect(HIFB_SURFACE_S* pDstImg, HIFB_BLIT_OPT_S* pstOpt);
static HI_S32  HIFB_DRV_SetTdeCallBack(IntCallBack pTdeCallBack);
static HI_S32  HIFB_DRV_TdeSupportFmt(HIFB_COLOR_FMT_E fmt);
static HI_S32  HIFB_DRV_WaitAllTdeDone(HI_BOOL bSync);
static HI_S32  HIFB_DRV_WaitForDone(TDE_HANDLE s32Handle, HI_U32 u32TimeOut);


static HI_S32  DRV_HIFB_Gfx2dOpen(HI_VOID);
static HI_S32  DRV_HIFB_Gfx2dClose(HI_VOID);

static inline HI_VOID DRV_HIFB_PrintBlitInfo(TDE2_SURFACE_S *pSrcSurface, TDE2_RECT_S *pstSrcRect, TDE2_SURFACE_S *pDstSurface, TDE2_RECT_S *pstDstRect, TDE2_OPT_S *pstOpt);


#define DRV_HIFB_GET_GFX2D_EXPORTFUNCS(GetGfx2dExportFuncs)     \
             {\
                 HI_S32 Ret = HI_SUCCESS;\
                 Ret = HI_DRV_MODULE_GetFunction(HI_ID_TDE, (HI_VOID**)&GetGfx2dExportFuncs);\
                 if ( (HI_SUCCESS != Ret) || (NULL == GetGfx2dExportFuncs))\
                 {\
                     HIFB_ERROR("gfx2d export funcs get failure\n");\
                     return HI_FAILURE;\
                 }\
             }


static inline TDE2_COLOR_FMT_E HIFB_DRV_GetXRGBFmt(HIFB_COLOR_FMT_E Fmt);
static inline TDE2_COLOR_FMT_E HIFB_DRV_GetRGBFmt(HIFB_COLOR_FMT_E Fmt);
static inline TDE2_COLOR_FMT_E HIFB_DRV_GetClutFmt(HIFB_COLOR_FMT_E Fmt);
static inline TDE_DEFLICKER_LEVEL_E HIFB_BLIT_GetAntiflickerLevel(HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel,TDE2_DEFLICKER_MODE_E *penDeflickerMode);



/***************************** API realization *********************************/

/***************************************************************************************
* func          : HIFB_DRV_TdeSupportFmt
* description   : CNcomment: 判断TDE是否支持该像素格式操作 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_TdeSupportFmt(HIFB_COLOR_FMT_E fmt)
{
    if(HIFB_FMT_BUTT <= fmt)
    {
        return HI_FALSE;
    }
    return s_bTdeColFmt[fmt];
}

/***************************************************************************************
* func          : HIFB_DRV_ConvFmt
* description   : CNcomment:FB像素格式转成TDE像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
TDE2_COLOR_FMT_E HIFB_DRV_ConvFmt(HIFB_COLOR_FMT_E Fmt)
{
    TDE2_COLOR_FMT_E eTdeColorFmt = TDE2_COLOR_FMT_BUTT;

    eTdeColorFmt = HIFB_DRV_GetRGBFmt(Fmt);
    if (TDE2_COLOR_FMT_BUTT != eTdeColorFmt)
    {
       return eTdeColorFmt;
    }

    eTdeColorFmt = HIFB_DRV_GetXRGBFmt(Fmt);
    if (TDE2_COLOR_FMT_BUTT != eTdeColorFmt)
    {
       return eTdeColorFmt;
    }

    eTdeColorFmt = HIFB_DRV_GetClutFmt(Fmt);
    if (TDE2_COLOR_FMT_BUTT != eTdeColorFmt)
    {
       return eTdeColorFmt;
    }

    return TDE2_COLOR_FMT_BUTT;
}

static inline TDE2_COLOR_FMT_E HIFB_DRV_GetXRGBFmt(HIFB_COLOR_FMT_E Fmt)
{
    switch (Fmt)
    {
        case HIFB_FMT_KRGB888:
            return TDE2_COLOR_FMT_ARGB8888;
        case HIFB_FMT_ARGB4444:
            return TDE2_COLOR_FMT_ARGB4444;
        case HIFB_FMT_ARGB1555:
            return TDE2_COLOR_FMT_ARGB1555;
        case HIFB_FMT_ARGB8888:
            return TDE2_COLOR_FMT_ARGB8888;
        case HIFB_FMT_ARGB8565:
            return TDE2_COLOR_FMT_ARGB8565;
        case HIFB_FMT_ABGR1555:
            return TDE2_COLOR_FMT_ABGR1555;
        case HIFB_FMT_ABGR4444:
            return TDE2_COLOR_FMT_ABGR4444;
        case HIFB_FMT_ABGR8888:
            return TDE2_COLOR_FMT_ABGR8888;
        case HIFB_FMT_ABGR8565:
            return TDE2_COLOR_FMT_ABGR8565;
        case HIFB_FMT_KBGR888:
            return TDE2_COLOR_FMT_ABGR8888;
        default:
            return TDE2_COLOR_FMT_BUTT;
    }
}


static inline TDE2_COLOR_FMT_E HIFB_DRV_GetRGBFmt(HIFB_COLOR_FMT_E Fmt)
{
     switch (Fmt)
     {
         case HIFB_FMT_RGB565:
             return TDE2_COLOR_FMT_RGB565;
         case HIFB_FMT_RGB888:
             return TDE2_COLOR_FMT_RGB888;
         case HIFB_FMT_KRGB444:
             return TDE2_COLOR_FMT_RGB444;
         case HIFB_FMT_KRGB555:
             return TDE2_COLOR_FMT_RGB555;
         case HIFB_FMT_BGR565:
             return TDE2_COLOR_FMT_BGR565;
         case HIFB_FMT_KBGR555:
             return TDE2_COLOR_FMT_BGR555;
         case HIFB_FMT_KBGR444:
             return TDE2_COLOR_FMT_BGR444;
         case HIFB_FMT_BGR888:
             return TDE2_COLOR_FMT_BGR888;
         default:
            return TDE2_COLOR_FMT_BUTT;
    }
}

static inline TDE2_COLOR_FMT_E HIFB_DRV_GetClutFmt(HIFB_COLOR_FMT_E Fmt)
{
     switch (Fmt)
     {
         case HIFB_FMT_1BPP:
             return TDE2_COLOR_FMT_CLUT1;
         case HIFB_FMT_2BPP:
             return TDE2_COLOR_FMT_CLUT2;
         case HIFB_FMT_4BPP:
             return TDE2_COLOR_FMT_CLUT4;
         case HIFB_FMT_8BPP:
             return TDE2_COLOR_FMT_CLUT8;
         case HIFB_FMT_ACLUT44:
             return TDE2_COLOR_FMT_ACLUT44;
         case HIFB_FMT_ACLUT88:
             return TDE2_COLOR_FMT_ACLUT88;
         default:
             return TDE2_COLOR_FMT_BUTT;
     }
}

HI_BOOL HIFB_IsTdeColorFmtClut(TDE2_COLOR_FMT_E enColorFmt)
{
    if (enColorFmt >= TDE2_COLOR_FMT_CLUT1
        && enColorFmt <= TDE2_COLOR_FMT_ACLUT88)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


/***************************************************************************************
* func            : HIFB_DRV_QuickCopy
* description    : CNcomment: tde quickcopy CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_QuickCopy(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TDE2_SURFACE_S stSrcSur = {0};
    TDE2_SURFACE_S stDstSur = {0};
    TDE2_RECT_S stSrcRect   = {0};
    TDE2_RECT_S stDstRect   = {0};
    TDE_HANDLE handle = 0;

    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    stSrcSur.u32PhyAddr   = pSrcImg->stCanvas.u32PhyAddr;
    stSrcSur.u32Width     = pSrcImg->stCanvas.u32Width;
    stSrcSur.u32Height    = pSrcImg->stCanvas.u32Height;
    stSrcSur.u32Stride    = pSrcImg->stCanvas.u32Pitch;
    stSrcSur.bAlphaMax255 = HI_TRUE;
    stSrcSur.bYCbCrClut   = HI_FALSE;
    stSrcSur.enColorFmt   = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);
    stSrcSur.u8Alpha0     = 0;
    stSrcSur.u8Alpha1     = 0;
    stSrcRect.s32Xpos     = pSrcImg->UpdateRect.x;
    stSrcRect.s32Ypos     = pSrcImg->UpdateRect.y;
    stSrcRect.u32Width    = pSrcImg->UpdateRect.w;
    stSrcRect.u32Height   = pSrcImg->UpdateRect.h;

    stDstSur.u32PhyAddr   = pDstImg->stCanvas.u32PhyAddr;
    stDstSur.u32Width     = pDstImg->stCanvas.u32Width;
    stDstSur.u32Height    = pDstImg->stCanvas.u32Height;
    stDstSur.u32Stride    = pDstImg->stCanvas.u32Pitch;
    stDstSur.bAlphaMax255 = HI_TRUE;
    stDstSur.bYCbCrClut   = HI_FALSE;
    stDstSur.enColorFmt   = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);
    stDstSur.u8Alpha0     = 0;
    stDstSur.u8Alpha1     = 0;
    stDstRect.s32Xpos     = pDstImg->UpdateRect.x;
    stDstRect.s32Ypos     = pDstImg->UpdateRect.y;
    stDstRect.u32Width    = pDstImg->UpdateRect.w;
    stDstRect.u32Height   = pDstImg->UpdateRect.h;

    DRV_HIFB_PrintBlitInfo(&stSrcSur, &stSrcRect, &stDstSur, &stDstRect, NULL);

    s32Ret = Gfx2dExportFuncs->pfnTdeBeginJob(&handle);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("begin job failed\n");
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeQuickCopy(handle,&stSrcSur,&stSrcRect,&stDstSur,&stDstRect);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("tde quick copy failed s32Ret = 0x%x\n",s32Ret);
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(handle,HI_TRUE,100,HI_FALSE,HI_NULL,HI_NULL);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("end job failed\n");
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    return HI_SUCCESS;
}

/***************************************************************************************
* func            : HIFB_DRV_QuickCopyEx
* description    : CNcomment: tde quickcopy boot mem to smmu memory CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_QuickCopyEx(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TDE2_SURFACE_S stSrcSur = {0};
    TDE2_SURFACE_S stDstSur = {0};
    TDE2_RECT_S stSrcRect   = {0};
    TDE2_RECT_S stDstRect   = {0};
    TDE_HANDLE handle = 0;

    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    stSrcSur.u32PhyAddr   = pSrcImg->stCanvas.u32PhyAddr;
    stSrcSur.u32Width     = pSrcImg->stCanvas.u32Width;
    stSrcSur.u32Height    = pSrcImg->stCanvas.u32Height;
    stSrcSur.u32Stride    = pSrcImg->stCanvas.u32Pitch;
    stSrcSur.bAlphaMax255 = HI_TRUE;
    stSrcSur.bYCbCrClut   = HI_FALSE;
    stSrcSur.enColorFmt   = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);
    stSrcSur.u8Alpha0     = 0;
    stSrcSur.u8Alpha1     = 0;
    stSrcRect.s32Xpos     = pSrcImg->UpdateRect.x;
    stSrcRect.s32Ypos     = pSrcImg->UpdateRect.y;
    stSrcRect.u32Width    = pSrcImg->UpdateRect.w;
    stSrcRect.u32Height   = pSrcImg->UpdateRect.h;

    stDstSur.u32PhyAddr   = pDstImg->stCanvas.u32PhyAddr;
    stDstSur.u32Width     = pDstImg->stCanvas.u32Width;
    stDstSur.u32Height    = pDstImg->stCanvas.u32Height;
    stDstSur.u32Stride    = pDstImg->stCanvas.u32Pitch;
    stDstSur.bAlphaMax255 = HI_TRUE;
    stDstSur.bYCbCrClut   = HI_FALSE;
    stDstSur.enColorFmt   = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);
    stDstSur.u8Alpha0     = 0;
    stDstSur.u8Alpha1     = 0;
    stDstRect.s32Xpos     = pDstImg->UpdateRect.x;
    stDstRect.s32Ypos     = pDstImg->UpdateRect.y;
    stDstRect.u32Width    = pDstImg->UpdateRect.w;
    stDstRect.u32Height   = pDstImg->UpdateRect.h;

    DRV_HIFB_PrintBlitInfo(&stSrcSur, &stSrcRect, &stDstSur, &stDstRect, NULL);

    s32Ret = Gfx2dExportFuncs->pfnTdeBeginJob(&handle);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("begin job failed\n");
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeQuickCopyEx(handle,&stSrcSur,&stSrcRect,&stDstSur,&stDstRect,HI_TRUE,HI_FALSE);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("tde quick copy failed\n");
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(handle,HI_TRUE,100,HI_FALSE,HI_NULL,HI_NULL);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("end job failed\n");
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    return HI_SUCCESS;
}


/***************************************************************************************
* func            : HIFB_DRV_Blit
* description    : CNcomment: tde blit CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_Blit(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt, HI_BOOL bRefreshScreen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TDE2_SURFACE_S stSrcSur = {0};
    TDE2_SURFACE_S stDstSur = {0};
    TDE2_RECT_S stSrcRect   = {0};
    TDE2_RECT_S stDstRect   = {0};
    TDE_HANDLE handle = 0;
    TDE2_OPT_S stOpt  = {0};
    TDE_DEFLICKER_LEVEL_E enTdeDflkLevel = TDE_DEFLICKER_BUTT;

    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    /** confing src*/
    stSrcSur.u32PhyAddr   = pSrcImg->stCanvas.u32PhyAddr;
    stSrcSur.u32Width     = pSrcImg->stCanvas.u32Width;
    stSrcSur.u32Height    = pSrcImg->stCanvas.u32Height;
    stSrcSur.u32Stride    = pSrcImg->stCanvas.u32Pitch;
    stSrcSur.bAlphaMax255 = HI_TRUE;
    stSrcSur.bYCbCrClut   = HI_FALSE;
    stSrcSur.enColorFmt   = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);
    stSrcSur.u8Alpha0     = pstOpt->stAlpha.u8Alpha0;
    stSrcSur.u8Alpha1     = pstOpt->stAlpha.u8Alpha1;

    if (!((stSrcSur.u8Alpha0 == 0) && (stSrcSur.u8Alpha1 == 0))){
        stSrcSur.bAlphaExt1555 = HI_TRUE;
    }

    stSrcRect.s32Xpos    = pSrcImg->UpdateRect.x;
    stSrcRect.s32Ypos    = pSrcImg->UpdateRect.y;
    stSrcRect.u32Width   = pSrcImg->UpdateRect.w;
    stSrcRect.u32Height  = pSrcImg->UpdateRect.h;

    /** confing dst*/
    stDstSur.u32PhyAddr   = pDstImg->stCanvas.u32PhyAddr;
    stDstSur.u32Width     = pDstImg->stCanvas.u32Width;
    stDstSur.u32Height    = pDstImg->stCanvas.u32Height;
    stDstSur.u32Stride    = pDstImg->stCanvas.u32Pitch;
    stDstSur.bAlphaMax255 = HI_TRUE;
    stDstSur.bYCbCrClut   = HI_FALSE;
    stDstSur.enColorFmt   = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);
    stDstSur.u8Alpha0     = pstOpt->stAlpha.u8Alpha0;
    stDstSur.u8Alpha1     = pstOpt->stAlpha.u8Alpha1;

    stDstRect.s32Xpos   = pDstImg->UpdateRect.x;
    stDstRect.s32Ypos   = pDstImg->UpdateRect.y;
    stDstRect.u32Width  = pDstImg->UpdateRect.w;
    stDstRect.u32Height = pDstImg->UpdateRect.h;


    stOpt.bResize = pstOpt->bScale;

    if (HIFB_IsTdeColorFmtClut(stSrcSur.enColorFmt)){
        stOpt.bClutReload       = HI_TRUE;
        stSrcSur.pu8ClutPhyAddr = pstOpt->pu8CmapAddr;
        stDstSur.pu8ClutPhyAddr = pstOpt->pu8CmapAddr;
    }

    enTdeDflkLevel = HIFB_BLIT_GetAntiflickerLevel(pstOpt->enAntiflickerLevel, &stOpt.enDeflickerMode);
    if(TDE_DEFLICKER_BUTT != enTdeDflkLevel){
        Gfx2dExportFuncs->pfnTdeSetDeflickerLevel(enTdeDflkLevel);
    }

    if (pstOpt->stCKey.bKeyEnable){
        if (HIFB_IsTdeColorFmtClut(stSrcSur.enColorFmt)){
            stOpt.enColorKeyMode = TDE2_COLORKEY_MODE_FOREGROUND;
            stOpt.unColorKeyValue.struCkClut.stAlpha.bCompIgnore = HI_TRUE;
            stOpt.unColorKeyValue.struCkClut.stClut.bCompOut     = pstOpt->stCKey.u32KeyMode;
            stOpt.unColorKeyValue.struCkClut.stClut.u8CompMax    = pstOpt->stCKey.u8BlueMax;
            stOpt.unColorKeyValue.struCkClut.stClut.u8CompMin    = pstOpt->stCKey.u8BlueMin;
            stOpt.unColorKeyValue.struCkClut.stClut.u8CompMask   = 0xff;
        }else{
            stOpt.enColorKeyMode = TDE2_COLORKEY_MODE_FOREGROUND;
            stOpt.unColorKeyValue.struCkARGB.stAlpha.bCompIgnore = HI_TRUE;
            stOpt.unColorKeyValue.struCkARGB.stRed.u8CompMax     = pstOpt->stCKey.u8RedMax;
            stOpt.unColorKeyValue.struCkARGB.stRed.u8CompMin     = pstOpt->stCKey.u8RedMin;
            stOpt.unColorKeyValue.struCkARGB.stRed.bCompOut      = pstOpt->stCKey.u32KeyMode;
            stOpt.unColorKeyValue.struCkARGB.stRed.u8CompMask    = 0xff;

            stOpt.unColorKeyValue.struCkARGB.stGreen.u8CompMax   = pstOpt->stCKey.u8GreenMax;
            stOpt.unColorKeyValue.struCkARGB.stGreen.u8CompMin   = pstOpt->stCKey.u8GreenMin;
            stOpt.unColorKeyValue.struCkARGB.stGreen.bCompOut    = pstOpt->stCKey.u32KeyMode;
            stOpt.unColorKeyValue.struCkARGB.stGreen.u8CompMask  = 0xff;

            stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMax    = pstOpt->stCKey.u8BlueMax;
            stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMin    = pstOpt->stCKey.u8BlueMin;
            stOpt.unColorKeyValue.struCkARGB.stBlue.bCompOut     = pstOpt->stCKey.u32KeyMode;
            stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMask   = 0xff;
        }
    }

    stOpt.u8GlobalAlpha = 255;

    if (pstOpt->stAlpha.bAlphaEnable){
        stOpt.enAluCmd       = TDE2_ALUCMD_BLEND;
        stOpt.u8GlobalAlpha  = pstOpt->stAlpha.u8GlobalAlpha;
        stOpt.enOutAlphaFrom = TDE2_OUTALPHA_FROM_NORM;
        stOpt.stBlendOpt.bGlobalAlphaEnable = HI_TRUE;
        stOpt.stBlendOpt.bPixelAlphaEnable  = HI_TRUE;
        stOpt.stBlendOpt.bSrc1AlphaPremulti = HI_TRUE;
        stOpt.stBlendOpt.bSrc2AlphaPremulti = HI_TRUE;
        stOpt.stBlendOpt.eBlendCmd          = TDE2_BLENDCMD_SRC;

    }else{
        stOpt.enOutAlphaFrom = TDE2_OUTALPHA_FROM_FOREGROUND;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeEnableRegionDeflicker(pstOpt->bRegionDeflicker);
    if (s32Ret != HI_SUCCESS){
        HIFB_ERROR("enable region deflicker failed!\n");
        return s32Ret;
    }

    if (pstOpt->stClip.bClip){
        stOpt.enClipMode = pstOpt->stClip.bInRegionClip ? TDE2_CLIPMODE_INSIDE : TDE2_CLIPMODE_OUTSIDE;
        stOpt.stClipRect.s32Xpos    = pstOpt->stClip.stClipRect.x;
        stOpt.stClipRect.s32Ypos    = pstOpt->stClip.stClipRect.y;
        stOpt.stClipRect.u32Width   = pstOpt->stClip.stClipRect.w;
        stOpt.stClipRect.u32Height  = pstOpt->stClip.stClipRect.h;
    }

    DRV_HIFB_PrintBlitInfo(&stSrcSur, &stSrcRect, &stDstSur, &stDstRect, &stOpt);

    s32Ret = Gfx2dExportFuncs->pfnTdeBeginJob(&handle);
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("begin job failed\n");
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeBlit(handle, &stDstSur, &stDstRect, &stSrcSur, &stSrcRect, &stDstSur, &stDstRect, &stOpt);
    if (s32Ret != HI_SUCCESS)
    {
        HIFB_ERROR("tde blit failed s32Ret= 0x%x\n",s32Ret);
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    if (pstOpt->bCallBack){
     /**
      **要是TDE任务完成，tde内部会调用pstOpt->pfnCallBack这个函数，这个在fb注册过了
      **/
        s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(handle,                            \
                                                   pstOpt->bBlock,                  \
                                                   100,                             \
                                                   HI_FALSE,                        \
                                                   (TDE_FUNC_CB)pstOpt->pfnCallBack,\
                                                   pstOpt->pParam);
    }else{
        s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(handle,                     \
                                                   pstOpt->bBlock,           \
                                                   100,                      \
                                                   HI_FALSE,                 \
                                                   HI_NULL,                  \
                                                   HI_NULL);
    }
    if(s32Ret != HI_SUCCESS){
        HIFB_ERROR("end job failed\n");
        Gfx2dExportFuncs->pfnTdeCancelJob(handle);
        return s32Ret;
    }

    if (pstOpt->bRegionDeflicker){
        s32Ret = Gfx2dExportFuncs->pfnTdeEnableRegionDeflicker(HI_FALSE);
        if (s32Ret != HI_SUCCESS){
            HIFB_ERROR("disable region deflicker failed!\n");
            return s32Ret;
        }
    }
    return handle;
}

static inline TDE_DEFLICKER_LEVEL_E HIFB_BLIT_GetAntiflickerLevel(HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel,TDE2_DEFLICKER_MODE_E *penDeflickerMode)
{
   switch (enAntiflickerLevel)
   {
       case HIFB_LAYER_ANTIFLICKER_NONE:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_NONE;
           return TDE_DEFLICKER_BUTT;
       case HIFB_LAYER_ANTIFLICKER_LOW:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_BOTH;
           return TDE_DEFLICKER_LOW;
       case HIFB_LAYER_ANTIFLICKER_MIDDLE:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_BOTH;
           return TDE_DEFLICKER_MIDDLE;
       case HIFB_LAYER_ANTIFLICKER_HIGH:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_BOTH;
           return TDE_DEFLICKER_HIGH;
       case HIFB_LAYER_ANTIFLICKER_AUTO:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_BOTH;
           return TDE_DEFLICKER_AUTO;
       default:
           *penDeflickerMode = TDE2_DEFLICKER_MODE_NONE;
           return TDE_DEFLICKER_BUTT;
   }
}

static HI_S32  HIFB_DRV_MbBlitEx(HIFB_BLIT_BUFFER_S *pSrcImg, HIFB_BLIT_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt)
{
    TDE2_SURFACE_S stSrcSur;
    TDE2_SURFACE_S stDstSur;
    TDE2_RECT_S    stSrcRect;
    TDE2_RECT_S    stDstRect;
    TDE2_OPT_S stOpt;

    TDE_HANDLE BlitHandle = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;
    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    HI_GFX_Memset(&stSrcSur, 0x0, sizeof(TDE2_SURFACE_S));
    stSrcSur.u32PhyAddr     = pSrcImg->stCanvas.YAddr;
    stSrcSur.u32CbCrPhyAddr = pSrcImg->stCanvas.CbCrAddr;
    stSrcSur.u32Width       = pSrcImg->stCanvas.Width;
    stSrcSur.u32Height      = pSrcImg->stCanvas.Height;
    stSrcSur.u32Stride      = pSrcImg->stCanvas.YStride;
    stSrcSur.u32CbCrStride  = pSrcImg->stCanvas.CbCrStride;
    stSrcSur.enColorFmt     = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);
    stSrcSur.bAlphaMax255   = HI_TRUE;

    stSrcRect.s32Xpos    = pSrcImg->UpdateRect.x;
    stSrcRect.s32Ypos    = pSrcImg->UpdateRect.y;
    stSrcRect.u32Width   = pSrcImg->UpdateRect.w;
    stSrcRect.u32Height  = pSrcImg->UpdateRect.h;


    HI_GFX_Memset(&stDstSur, 0x0, sizeof(TDE2_SURFACE_S));
    stDstSur.u32PhyAddr = pDstImg->stCanvas.YAddr;
    stDstSur.u32Width   = pDstImg->stCanvas.Width;
    stDstSur.u32Height  = pDstImg->stCanvas.Height;
    stDstSur.u32Stride  = pDstImg->stCanvas.YStride;
    stDstSur.enColorFmt = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);
    stDstSur.bAlphaMax255 = HI_TRUE;

    stDstRect.s32Xpos   = pDstImg->UpdateRect.x;
    stDstRect.s32Ypos   = pDstImg->UpdateRect.y;
    stDstRect.u32Width  = pDstImg->UpdateRect.w;
    stDstRect.u32Height = pDstImg->UpdateRect.h;

    HI_GFX_Memset(&stOpt, 0x0, sizeof(TDE2_OPT_S));
    stOpt.bResize = pstOpt->bScale;

    DRV_HIFB_PrintBlitInfo(&stSrcSur, &stSrcRect, &stDstSur, &stDstRect, &stOpt);

    s32Ret = Gfx2dExportFuncs->pfnTdeBeginJob(&BlitHandle);
    if (HI_SUCCESS != s32Ret){
        HIFB_ERROR("begin job failed\n");
        return s32Ret;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    s32Ret = Gfx2dExportFuncs->pfnTdeSingleBlitEx(BlitHandle, &stSrcSur, &stSrcRect, &stDstSur, &stDstRect, &stOpt, HI_TRUE, HI_FALSE);
#else
    s32Ret = Gfx2dExportFuncs->pfnTdeSingleBlitEx(BlitHandle, &stSrcSur, &stSrcRect, &stDstSur, &stDstRect, &stOpt, HI_TRUE, HI_TRUE);
#endif
    if (s32Ret != HI_SUCCESS){
        HIFB_ERROR("tde blit failed s32Ret = 0x%x\n",s32Ret);
        Gfx2dExportFuncs->pfnTdeCancelJob(BlitHandle);
        return s32Ret;
    }

    s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(BlitHandle,HI_TRUE,100,HI_FALSE,HI_NULL,HI_NULL);
    if (s32Ret != HI_SUCCESS){
        HIFB_ERROR("end job failed\n");
        Gfx2dExportFuncs->pfnTdeCancelJob(BlitHandle);
        return s32Ret;
    }

    return BlitHandle;
}

/***************************************************************************************
* func          : HIFB_DRV_SetTdeCallBack
* description   : CNcomment: 设置TDE回调函数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_SetTdeCallBack(IntCallBack pTdeCallBack)
{/** 这个全局变量没有人用 **/
   s_pTdeCallBack = pTdeCallBack;
   return HI_SUCCESS;
}


/***************************************************************************************
* func          : HIFB_DRV_ClearRect
* description   : CNcomment:清surface CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_ClearRect(HIFB_SURFACE_S* pDstImg, HIFB_BLIT_OPT_S* pstOpt)
{
    HI_S32 s32Ret;
    TDE2_SURFACE_S TDESurface = {0};
    TDE2_RECT_S Rect;
    TDE_HANDLE s32Handle;

    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    /** confing dst*/
    TDESurface.u32PhyAddr    = pDstImg->u32PhyAddr;
    TDESurface.u32Width      = pDstImg->u32Width;
    TDESurface.u32Height     = pDstImg->u32Height;
    TDESurface.u32Stride     = pDstImg->u32Pitch;
    TDESurface.bAlphaMax255  = HI_TRUE;
    TDESurface.bYCbCrClut    = HI_FALSE;
    TDESurface.enColorFmt    = HIFB_DRV_ConvFmt(pDstImg->enFmt);
    TDESurface.u8Alpha0      = pstOpt->stAlpha.u8Alpha0;
    TDESurface.u8Alpha1      = pstOpt->stAlpha.u8Alpha1;

    Rect.s32Xpos  = 0;
    Rect.s32Ypos  = 0;
    Rect.u32Width = pDstImg->u32Width;
    Rect.u32Height = pDstImg->u32Height;

    s32Ret = Gfx2dExportFuncs->pfnTdeBeginJob(&s32Handle);
    if(s32Ret != HI_SUCCESS)
    {
        HIFB_ERROR("begin job failed\n");
        return s32Ret;
    }
    s32Ret = Gfx2dExportFuncs->pfnTdeQuickFill(s32Handle, &TDESurface, &Rect, 0x0);
    if(s32Ret != HI_SUCCESS)
    {
        HIFB_ERROR("tde quick fill failed s32Ret:0x%x\n", s32Ret);
        Gfx2dExportFuncs->pfnTdeCancelJob(s32Handle);
        return s32Ret;
    }
    s32Ret = Gfx2dExportFuncs->pfnTdeEndJob(s32Handle, pstOpt->bBlock, 100, HI_FALSE, HI_NULL, HI_NULL);
    if(s32Ret != HI_SUCCESS)
    {
        HIFB_ERROR("end job failed ret:%d\n", s32Ret);
        Gfx2dExportFuncs->pfnTdeCancelJob(s32Handle);
        return s32Ret;
    }
    return s32Handle;

}


static HI_S32 HIFB_DRV_WaitAllTdeDone(HI_BOOL bSync)
{
    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    Gfx2dExportFuncs->pfnTdeWaitAllDone(bSync);

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : HIFB_DRV_CalScaleRect
* description   : CNcomment: 计算缩放更新区域 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 HIFB_DRV_CalScaleRect(const TDE2_RECT_S* pstSrcRect, const TDE2_RECT_S* pstDstRect,
                                TDE2_RECT_S* pstRectInSrc, TDE2_RECT_S* pstRectInDst)
{
    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    return Gfx2dExportFuncs->pfnTdeCalScaleRect(pstSrcRect, pstDstRect, pstRectInSrc, pstRectInDst);
}

static HI_S32 HIFB_DRV_WaitForDone(TDE_HANDLE s32Handle, HI_U32 u32TimeOut)
{
    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    return Gfx2dExportFuncs->pfnTdeWaitForDone(s32Handle, u32TimeOut);
}


static HI_S32 DRV_HIFB_Gfx2dOpen(HI_VOID)
{
    HI_S32 Ret = HI_SUCCESS;
    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    Ret = Gfx2dExportFuncs->pfnTdeOpen();
    if (Ret < 0){
        HIFB_ERROR("Tde open failed!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


static HI_S32 DRV_HIFB_Gfx2dClose(HI_VOID)
{
    TDE_EXPORT_FUNC_S *Gfx2dExportFuncs = NULL;

    DRV_HIFB_GET_GFX2D_EXPORTFUNCS(Gfx2dExportFuncs);

    Gfx2dExportFuncs->pfnTdeClose();

    return HI_SUCCESS;
}


HI_VOID DRV_HIFB_GetGfx2dCallBackFunction(HIFB_DRV_TDEOPS_S *Gfx2dCallBackFunction)
{
    Gfx2dCallBackFunction->HIFB_DRV_TdeOpen        = DRV_HIFB_Gfx2dOpen;
    Gfx2dCallBackFunction->HIFB_DRV_TdeClose       = DRV_HIFB_Gfx2dClose;
    Gfx2dCallBackFunction->HIFB_DRV_QuickCopy      = HIFB_DRV_QuickCopy;
    Gfx2dCallBackFunction->HIFB_DRV_QuickCopyEx    = HIFB_DRV_QuickCopyEx;
    Gfx2dCallBackFunction->HIFB_DRV_Blit           = HIFB_DRV_Blit;
    Gfx2dCallBackFunction->HIFB_DRV_MbBlitEx       = HIFB_DRV_MbBlitEx;
    Gfx2dCallBackFunction->HIFB_DRV_CalScaleRect   = HIFB_DRV_CalScaleRect;
    Gfx2dCallBackFunction->HIFB_DRV_ClearRect      = HIFB_DRV_ClearRect;
    Gfx2dCallBackFunction->HIFB_DRV_SetTdeCallBack = HIFB_DRV_SetTdeCallBack;
    Gfx2dCallBackFunction->HIFB_DRV_TdeSupportFmt  = HIFB_DRV_TdeSupportFmt;
    Gfx2dCallBackFunction->HIFB_DRV_WaitAllTdeDone = HIFB_DRV_WaitAllTdeDone;
    Gfx2dCallBackFunction->HIFB_DRV_WaitForDone    = HIFB_DRV_WaitForDone;
    return;
}

static inline HI_VOID DRV_HIFB_PrintBlitInfo(TDE2_SURFACE_S *pSrcSurface, TDE2_RECT_S *pstSrcRect, TDE2_SURFACE_S *pDstSurface, TDE2_RECT_S *pstDstRect, TDE2_OPT_S *pstOpt)
{
#if 0
    if (NULL == pSrcSurface){
       return;
    }
    if (NULL == pstSrcRect){
       return;
    }
    if (NULL == pDstSurface){
       return;
    }
    if (NULL == pstDstRect){
       return;
    }
    HI_PRINT("\n=============================================================================\n");
    HI_PRINT("SrcRect [%d %d %d %d]\n", pstSrcRect->s32Xpos, pstSrcRect->s32Ypos, pstSrcRect->u32Width, pstSrcRect->u32Height);
    HI_PRINT("SrcSurface yphyadd 0x%x, width %d, height %d, ystride %d\n",    \
                  pSrcSurface->u32PhyAddr,pSrcSurface->u32Width,pSrcSurface->u32Height,pSrcSurface->u32Stride);
    HI_PRINT("SrcSurface cbcrphyadd 0x%x, cbcrstride %d fmt %d\n",    \
                  pSrcSurface->u32CbCrPhyAddr,pSrcSurface->u32CbCrStride,pSrcSurface->enColorFmt);

    HI_PRINT("DstRect [%d %d %d %d]\n", pstDstRect->s32Xpos, pstDstRect->s32Ypos, pstDstRect->u32Width, pstDstRect->u32Height);
    HI_PRINT("DstSurface yphyadd 0x%x, width %d, height %d, ystride %d\n",    \
                  pDstSurface->u32PhyAddr,pDstSurface->u32Width,pDstSurface->u32Height,pDstSurface->u32Stride);
    HI_PRINT("DstSurface cbcrphyadd 0x%x, cbcrstride %d fmt %d\n",    \
                  pDstSurface->u32CbCrPhyAddr,pDstSurface->u32CbCrStride,pDstSurface->enColorFmt);
    HI_PRINT("=============================================================================\n");
#endif
}
