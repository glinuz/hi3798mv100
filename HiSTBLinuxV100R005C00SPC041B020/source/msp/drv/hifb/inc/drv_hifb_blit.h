/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_blit.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_BLIT_H__
#define __DRV_HIFB_BLIT_H__


/*********************************add include here******************************/
#include <linux/fb.h>

#include "hi_tde_type.h"
#include "hifb.h"
#include "drv_hifb_common.h"


/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */





/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
typedef struct
{
    HI_BOOL   bInRegionClip;
    HI_BOOL   bClip;
    HIFB_RECT stClipRect;
}HIFB_CLIP_S;

typedef struct
{
    HI_BOOL bScale;
    HI_BOOL bBlock;
    HI_BOOL bCallBack;
    HI_BOOL bRegionDeflicker;
    HI_VOID *pParam;
    HI_U8*  pu8CmapAddr;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel;
    IntCallBack pfnCallBack;
    HIFB_ALPHA_S stAlpha;
    HIFB_COLORKEYEX_S stCKey;
    HIFB_CLIP_S stClip;
}HIFB_BLIT_OPT_S;


typedef struct
{
    HI_U32  YAddr;
    HI_U32  CbCrAddr;
    HI_U32  Width;
    HI_U32  Height;
    HI_U32  YStride;
    HI_U32  CbCrStride;
    HIFB_COLOR_FMT_E enFmt;
}HIFB_BLIT_SURFACE_S;

typedef struct
{
    HIFB_BLIT_SURFACE_S stCanvas;
    HIFB_RECT UpdateRect;
}HIFB_BLIT_BUFFER_S;


typedef struct
{
    HI_S32  (*HIFB_DRV_QuickCopy)      (HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg);
    HI_S32  (*HIFB_DRV_QuickCopyEx)    (HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg);
    HI_S32  (*HIFB_DRV_Blit)           (HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt,HI_BOOL bScreenRefresh);
    HI_S32  (*HIFB_DRV_MbBlitEx)       (HIFB_BLIT_BUFFER_S *pSrcImg, HIFB_BLIT_BUFFER_S *pDstImg,  HIFB_BLIT_OPT_S *pstOpt);
    HI_S32  (*HIFB_DRV_ClearRect)      (HIFB_SURFACE_S* pDstImg, HIFB_BLIT_OPT_S* pstOpt);
    HI_S32  (*HIFB_DRV_SetTdeCallBack) (IntCallBack pTdeCallBack);
    HI_S32  (*HIFB_DRV_WaitAllTdeDone) (HI_BOOL bSync);
    HI_S32  (*HIFB_DRV_TdeSupportFmt)  (HIFB_COLOR_FMT_E fmt);
    HI_S32  (*HIFB_DRV_CalScaleRect)   (const TDE2_RECT_S* pstSrcRect, const TDE2_RECT_S* pstDstRect,TDE2_RECT_S* pstRectInSrc, TDE2_RECT_S* pstRectInDst);
    HI_S32  (*HIFB_DRV_WaitForDone)    (TDE_HANDLE s32Handle, HI_U32 u32TimeOut);
    HI_S32  (*HIFB_DRV_TdeOpen)        (HI_VOID);
    HI_S32  (*HIFB_DRV_TdeClose)       (HI_VOID);
}HIFB_DRV_TDEOPS_S;


/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_VOID DRV_HIFB_GetGfx2dCallBackFunction(HIFB_DRV_TDEOPS_S *Gfx2dCallBackFunction);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
