/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_blit.c
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         :
                      CNcomment: 图层显示接口 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2016/01/12                  y00181162               Created file
******************************************************************************/


/*********************************add include here******************************/
#include "hi_go_common.h"
#include "hi_go_logo.h"

#include "hi_tde_type.h"
#include "hi_tde_api.h"
/***************************** Macro Definition ******************************/

#define RGB32TO1555(c) \
    (((c & 0x80000000) >> 16) | ((c & 0xf80000) >> 9) | ((c & 0xf800) >> 6) | ((c & 0xf8) >> 3))

#define RGB32TO565(c) \
    (((c & 0xf80000) >> 8) | ((c & 0xfc00) >> 5) | ((c & 0xf8) >> 3))


/*************************** Structure Definition ****************************/
struct
{
    HIGO_PF_E                HiGfxFmt;
    TDE2_COLOR_FMT_E         enTdeFmt;
}gs_HiGfx_To_Tde_MapTable[] = {
        {HIGO_PF_565,   TDE2_COLOR_FMT_RGB565},
        {HIGO_PF_1555,  TDE2_COLOR_FMT_ARGB1555},
        {HIGO_PF_8888,  TDE2_COLOR_FMT_ARGB8888},
        {HIGO_PF_AYCbCr8888,  TDE2_COLOR_FMT_ARGB8888},
    };


struct
{
    HIGO_PF_E             HiGoPF;
    TDE2_MB_COLOR_FMT_E   enTdeFmt;
}gs_HiGfx_To_MBTde_MapTable[] = {
        {HIGO_PF_YUV400,      TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP},
        {HIGO_PF_YUV420,      TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP},
        {HIGO_PF_YUV422_12,   TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP},
        {HIGO_PF_YUV422_21,   TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP},
        {HIGO_PF_YUV444,      TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP}
    };

/********************** Global Variable declaration **************************/

/**********************       API realization       **************************/

/*****************************************************************************
* func            : HI_GO_BLIT_Init
* description     : tde初始化
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GO_BLIT_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_TDE2_Open();
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = HI_TDE2_SetAlphaThresholdState(HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = HI_TDE2_SetAlphaThresholdValue(0x80);
    if (s32Ret != HI_SUCCESS)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* func            : GFX_ADP_ConvertFormat
* description     : surface像素格式转成tde需要的像素格式
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 GFX_ADP_ConvertFormat(HIGO_PF_E enHiGfxFmt, TDE2_COLOR_FMT_E *pTdeFmt)
{
    HI_S32 s32Cnt  = 0;
    HI_S32 s32Size = 0;

    s32Size = (HI_S32)(sizeof(gs_HiGfx_To_Tde_MapTable) / sizeof(gs_HiGfx_To_Tde_MapTable[0]));

    for (s32Cnt = 0; s32Cnt < s32Size; s32Cnt++)
    {
        if (gs_HiGfx_To_Tde_MapTable[s32Cnt].HiGfxFmt == enHiGfxFmt)
        {
            *pTdeFmt = gs_HiGfx_To_Tde_MapTable[s32Cnt].enTdeFmt;
            return HI_SUCCESS;
        }
    }
    return HI_FAILURE;
}

/*****************************************************************************
* func            : GFX_ADP_ConvertMBFormat
* description     : surface像素格式转成tde需要的像素格式
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_S32 GFX_ADP_ConvertMBFormat(HIGO_PF_E enHiGfxFmt, TDE2_MB_COLOR_FMT_E *pTdeFmt)
{
    HI_S32 s32Cnt  = 0;
    HI_S32 s32Size = 0;

    s32Size = (HI_S32)(sizeof(gs_HiGfx_To_MBTde_MapTable) / sizeof(gs_HiGfx_To_MBTde_MapTable[0]));

    for (s32Cnt = 0; s32Cnt < s32Size; s32Cnt++)
    {
        if (gs_HiGfx_To_MBTde_MapTable[s32Cnt].HiGoPF == enHiGfxFmt)
        {
            *pTdeFmt = gs_HiGfx_To_MBTde_MapTable[s32Cnt].enTdeFmt;
            return HI_SUCCESS;
        }
    }
    return HI_FAILURE;
}

/*****************************************************************************
* func            : GFX_ADP_ColorExpand
* description     : 将要填充的color值转换成tde识别的值
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_S32 GFX_ADP_ColorExpand(HIGO_PF_E enPixelFormat, HI_U32 u32Color, HI_U32 *pFillData)
{
    switch(enPixelFormat)
    {
        case HIGO_PF_1555:
            *pFillData = (HI_U32)RGB32TO1555(u32Color);
            break;
        case HIGO_PF_8888:
        case HIGO_PF_AYCbCr8888:
            *pFillData = (HI_U32)u32Color;
            break;
        case HIGO_PF_565:
            *pFillData = RGB32TO565(u32Color);
            break;
        default:
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* func            : GFX_ADP_YCbCrToRgb
* description     : yuv转rgb
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_S32 GFX_ADP_YCbCrToRgb(HI_GO_SURFACE_S* pSrcSurface, TDE2_RECT_S *pSrcRect, HI_GO_SURFACE_S* pDstSurface, TDE2_RECT_S *pDstRect, HI_BOOL bScale)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TDE_HANDLE s32Handle;
    TDE2_MB_S stSrcSurface;
    TDE2_SURFACE_S stDstSurface;
    TDE2_MBOPT_S stMbOpt;

    memset(&stSrcSurface,0,sizeof(TDE2_MB_S));
    memset(&stDstSurface,0,sizeof(TDE2_SURFACE_S));
    memset(&stMbOpt,0,sizeof(TDE2_MBOPT_S));

    s32Ret = GFX_ADP_ConvertMBFormat(pSrcSurface->enPixelFormat, &(stSrcSurface.enMbFmt));
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (HI_SUCCESS != GFX_ADP_ConvertFormat(pDstSurface->enPixelFormat, &(stDstSurface.enColorFmt)))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    stSrcSurface.u32YPhyAddr    = (HI_U32)pSrcSurface->pPhyAddr[0];
    stSrcSurface.u32CbCrPhyAddr = (HI_U32)pSrcSurface->pPhyAddr[1];
    stSrcSurface.u32YStride     = pSrcSurface->u32Stride[0];
    stSrcSurface.u32CbCrStride  = pSrcSurface->u32Stride[1];
    stSrcSurface.u32YWidth      = pSrcSurface->u32Width[0];
    stSrcSurface.u32YHeight     = pSrcSurface->u32Height[0];

#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("stSrcSurface.u32YPhyAddr    = 0x%x\n",stSrcSurface.u32YPhyAddr);
    HI_GFX_Print("stSrcSurface.u32CbCrPhyAddr = 0x%x\n",stSrcSurface.u32CbCrPhyAddr);
    HI_GFX_Print("stSrcSurface.u32YStride     = %d\n",stSrcSurface.u32YStride);
    HI_GFX_Print("stSrcSurface.u32YWidth      = %d\n",stSrcSurface.u32YWidth);
    HI_GFX_Print("stSrcSurface.u32YHeight     = %d\n",stSrcSurface.u32YHeight);
    HI_GFX_Print("stSrcSurface.enMbFmt        = %d\n",stSrcSurface.enMbFmt);
    HI_GFX_Print("=============================================\n");
#endif

    stDstSurface.u32PhyAddr     = (HI_U32)pDstSurface->pPhyAddr[0];
    stDstSurface.u32Height      = pDstSurface->u32Height[0];
    stDstSurface.u32Width       = pDstSurface->u32Width[0];
    stDstSurface.u32Stride      = pDstSurface->u32Stride[0];
    stDstSurface.pu8ClutPhyAddr = NULL;
    stDstSurface.bYCbCrClut     = HI_FALSE;
    stDstSurface.bAlphaMax255   = HI_TRUE;
    stDstSurface.bAlphaExt1555  = HI_TRUE;
    stDstSurface.u8Alpha0       = 0;
    stDstSurface.u8Alpha1       = 255;

#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("stDstSurface.u32PhyAddr = 0x%x\n",stDstSurface.u32PhyAddr);
    HI_GFX_Print("stDstSurface.u32Height  = %d\n",stDstSurface.u32Height);
    HI_GFX_Print("stDstSurface.u32Width   = %d\n",stDstSurface.u32Width);
    HI_GFX_Print("stDstSurface.u32Stride  = %d\n",stDstSurface.u32Stride);
    HI_GFX_Print("stDstSurface.enColorFmt = %d\n",stDstSurface.enColorFmt);
    HI_GFX_Print("=============================================\n");
#endif

    if (HI_TRUE == bScale)
    {
        stMbOpt.enResize = TDE2_MBRESIZE_QUALITY_LOW;
    }

    s32Handle = HI_TDE2_BeginJob();
    if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = HI_TDE2_MbBlit(s32Handle, &stSrcSurface,pSrcRect, &stDstSurface, pDstRect, &stMbOpt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        (HI_VOID)HI_TDE2_EndJob(s32Handle, HI_FALSE, HI_TRUE, 100);
        return HI_FAILURE;
    }

    s32Ret |= HI_TDE2_EndJob(s32Handle, HI_FALSE, HI_TRUE, 100);
    if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* func            : GFX_ADP_RgbToRgb
* description     : rgb转rgb
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
static HI_S32 GFX_ADP_RgbToRgb(HI_GO_SURFACE_S* pSrcSurface, TDE2_RECT_S *pSrcRect, HI_GO_SURFACE_S* pDstSurface, TDE2_RECT_S *pDstRect,HI_BOOL bScale)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TDE_HANDLE s32Handle;
    TDE2_SURFACE_S stSrcSurface;
    TDE2_SURFACE_S stDstSurface;
    TDE2_OPT_S stOpt;

    memset(&stSrcSurface,0,sizeof(TDE2_SURFACE_S));
    memset(&stDstSurface,0,sizeof(TDE2_SURFACE_S));
    memset(&stOpt,0,sizeof(TDE2_OPT_S));

    if (HI_SUCCESS != GFX_ADP_ConvertFormat(pSrcSurface->enPixelFormat, &(stSrcSurface.enColorFmt)))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (HI_SUCCESS != GFX_ADP_ConvertFormat(pDstSurface->enPixelFormat, &(stDstSurface.enColorFmt)))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    stSrcSurface.u32PhyAddr     = (HI_U32)pSrcSurface->pPhyAddr[0];
    stSrcSurface.u32Height      = pSrcSurface->u32Height[0];
    stSrcSurface.u32Width       = pSrcSurface->u32Width[0];
    stSrcSurface.u32Stride      = pSrcSurface->u32Stride[0];
    stSrcSurface.pu8ClutPhyAddr = NULL;
    stSrcSurface.bYCbCrClut     = HI_FALSE;
    stSrcSurface.bAlphaMax255   = HI_TRUE;
    stSrcSurface.bAlphaExt1555  = HI_TRUE;
    stSrcSurface.u8Alpha0       = 0;
    stSrcSurface.u8Alpha1       = 255;

#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("stSrcSurface.u32PhyAddr    = 0x%x\n",stSrcSurface.u32PhyAddr);
    HI_GFX_Print("stSrcSurface.u32Stride     = %d\n",  stSrcSurface.u32Stride);
    HI_GFX_Print("stSrcSurface.u32Width      = %d\n",  stSrcSurface.u32Width);
    HI_GFX_Print("stSrcSurface.u32Height     = %d\n",  stSrcSurface.u32Height);
    HI_GFX_Print("stSrcSurface.enColorFmt    = %d\n",  stSrcSurface.enColorFmt);
    HI_GFX_Print("=============================================\n");
#endif

    stDstSurface.u32PhyAddr     = (HI_U32)pDstSurface->pPhyAddr[0];
    stDstSurface.u32Height      = pDstSurface->u32Height[0];
    stDstSurface.u32Width       = pDstSurface->u32Width[0];
    stDstSurface.u32Stride      = pDstSurface->u32Stride[0];
    stDstSurface.pu8ClutPhyAddr = NULL;
    stDstSurface.bYCbCrClut     = HI_FALSE;
    stDstSurface.bAlphaMax255   = HI_TRUE;
    stDstSurface.bAlphaExt1555  = HI_TRUE;
    stDstSurface.u8Alpha0       = 0;
    stDstSurface.u8Alpha1       = 255;

#if 0
    memset(pDstSurface->pPhyAddr[0],0xff,stDstSurface.u32Stride * stDstSurface.u32Height);
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("stDstSurface.u32PhyAddr    = 0x%x\n",stDstSurface.u32PhyAddr);
    HI_GFX_Print("stDstSurface.u32Stride     = %d\n",  stDstSurface.u32Stride);
    HI_GFX_Print("stDstSurface.u32Width      = %d\n",  stDstSurface.u32Width);
    HI_GFX_Print("stDstSurface.u32Height     = %d\n",  stDstSurface.u32Height);
    HI_GFX_Print("stDstSurface.enColorFmt    = %d\n",  stDstSurface.enColorFmt);
    HI_GFX_Print("=============================================\n");
#endif

    stOpt.enColorKeyMode = TDE2_COLORKEY_MODE_NONE;
    stOpt.enOutAlphaFrom = TDE2_OUTALPHA_FROM_NORM;
    stOpt.enAluCmd       = TDE2_ALUCMD_NONE;
    stOpt.u8GlobalAlpha  = 0xff;
    if (HI_TRUE == bScale)
    {
        stOpt.bResize      = HI_TRUE;
        stOpt.enFilterMode = TDE2_FILTER_MODE_COLOR;
    }

    s32Handle = HI_TDE2_BeginJob();
    if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
    {
        return HI_FAILURE;
    }
    /**
     **这个地方可以使用单源操作，不需要背景参与
     **/
    s32Ret = HI_TDE2_Bitblit(s32Handle,&stDstSurface,pDstRect,&stSrcSurface,pSrcRect,&stDstSurface,pDstRect,&stOpt);
    if (s32Ret != HI_SUCCESS)
    {
        HI_GFX_Log();
        (HI_VOID)HI_TDE2_EndJob(s32Handle,HI_FALSE,HI_TRUE,100);
        return HI_FAILURE;
    }

    s32Ret |= HI_TDE2_EndJob(s32Handle,HI_FALSE,HI_TRUE,100);
    if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* func            : HI_GO_Blit
* description     : 2d叠加操作
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GO_Blit(HI_HANDLE u32SrcSurface, const HI_GO_RECT_S* pSrcRect,HI_HANDLE u32DstSurface, const HI_GO_RECT_S* pDstRect, const HIGO_BLTOPT_S* pBlitOpt)
{

    HI_S32 s32Ret        = HI_SUCCESS;
    HI_BOOL bEnableScale = HI_FALSE;
    HI_GO_SURFACE_S *pstSrcSurface = (HI_GO_SURFACE_S *)u32SrcSurface;
    HI_GO_SURFACE_S *pstDstSurface = (HI_GO_SURFACE_S *)u32DstSurface;
    TDE2_RECT_S stSrcRect = {0};
    TDE2_RECT_S stDstRect = {0};

    if (NULL == pstSrcSurface || NULL == pstDstSurface || NULL == pBlitOpt)
    {
        return HI_FAILURE;
    }

    if (NULL == pSrcRect)
    {
        stSrcRect.u32Width  = pstSrcSurface->u32Width[0];
        stSrcRect.u32Height = pstSrcSurface->u32Height[0];
    }
#ifdef HI_LOADER_BOOTLOADER
    else
    {
        stSrcRect.s32Xpos   = pSrcRect->x;
        stSrcRect.s32Ypos   = pSrcRect->y;
        stSrcRect.u32Width  = pSrcRect->w;
        stSrcRect.u32Height = pSrcRect->h;
    }
#endif

    /** should not need check pDstRect, because is not equal to NULL **/
    stDstRect.s32Xpos   = pDstRect->x;
    stDstRect.s32Ypos   = pDstRect->y;
    stDstRect.u32Width  = pDstRect->w;
    stDstRect.u32Height = pDstRect->h;

    bEnableScale = pBlitOpt->EnableScale;
    if (pstSrcSurface->enPixelFormat >= HIGO_PF_YUV400 && pstSrcSurface->enPixelFormat <= HIGO_PF_YUV422_21)
    {
        s32Ret = GFX_ADP_YCbCrToRgb(pstSrcSurface, &stSrcRect, pstDstSurface, &stDstRect,bEnableScale);
    }
    else
    {
        s32Ret = GFX_ADP_RgbToRgb(pstSrcSurface, &stSrcRect, pstDstSurface, &stDstRect,bEnableScale);
    }
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        goto ERROR_EXIT;
    }

    return HI_SUCCESS;

ERROR_EXIT:
    return HI_FAILURE;
}

/*****************************************************************************
* func            : HI_GO_FillRect
* description     : 填充矩形
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GO_FillRect(HI_HANDLE hSurface, const HI_GO_RECT_S* pstRect, HI_U32 u32Color, HIGO_COMPOPT_E eComPopt)
{

    HI_S32 s32Ret = HI_SUCCESS;
    TDE_HANDLE s32Handle = HI_ERR_TDE_INVALID_HANDLE;
    TDE2_RECT_S stRect   = {0};
    HI_U32 u32FillData   = 0;
    TDE2_SURFACE_S stSurface;
    HI_GO_SURFACE_S *pstSurface = (HI_GO_SURFACE_S *)hSurface;

    if (NULL == pstSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (NULL == pstRect)
    {
        stRect.u32Width  = pstSurface->u32Width[0];
        stRect.u32Height = pstSurface->u32Height[0];
    }
#ifdef HI_LOADER_BOOTLOADER
    else
    {/**deal with codecc,boot loader need, logo no need **/
        stRect.s32Xpos   = pstRect->x;
        stRect.s32Ypos   = pstRect->y;
        stRect.u32Width  = pstRect->w;
        stRect.u32Height = pstRect->h;
    }
#endif

    memset(&stSurface,0,sizeof(TDE2_SURFACE_S));
    if (HI_SUCCESS != GFX_ADP_ConvertFormat(pstSurface->enPixelFormat, &(stSurface.enColorFmt)))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    s32Ret = GFX_ADP_ColorExpand(pstSurface->enPixelFormat,u32Color,&u32FillData);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    stSurface.u32PhyAddr     = (HI_U32)pstSurface->pPhyAddr[0];
    stSurface.u32Height      = pstSurface->u32Height[0];
    stSurface.u32Width       = pstSurface->u32Width[0];
    stSurface.u32Stride      = pstSurface->u32Stride[0];
    stSurface.pu8ClutPhyAddr = NULL;
    stSurface.bYCbCrClut     = HI_FALSE;
    stSurface.bAlphaMax255   = HI_TRUE;
    stSurface.bAlphaExt1555  = HI_TRUE;
    stSurface.u8Alpha0       = 0;
    stSurface.u8Alpha1       = 255;


#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("stSurface.u32PhyAddr    = 0x%x\n",stSurface.u32PhyAddr);
    HI_GFX_Print("stSurface.u32Stride     = %d\n",  stSurface.u32Stride);
    HI_GFX_Print("stSurface.u32Width      = %d\n",  stSurface.u32Width);
    HI_GFX_Print("stSurface.u32Height     = %d\n",  stSurface.u32Height);
    HI_GFX_Print("=============================================\n");
#endif

    s32Handle = HI_TDE2_BeginJob();
    if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
    {
        HI_GFX_Log();
        goto ERROR_EXIT;
    }

    s32Ret = HI_TDE2_QuickFill(s32Handle, &stSurface, &stRect, u32FillData);
    if (s32Ret != HI_SUCCESS)
    {
        HI_GFX_Log();
        (HI_VOID)HI_TDE2_EndJob(s32Handle,HI_FALSE,HI_TRUE,100);
        goto ERROR_EXIT;
    }

    s32Ret |= HI_TDE2_EndJob(s32Handle,HI_FALSE,HI_TRUE,100);
    if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
    {
        HI_GFX_Log();
        goto ERROR_EXIT;
    }

    return HI_SUCCESS;

ERROR_EXIT:
    return HI_FAILURE;
}
