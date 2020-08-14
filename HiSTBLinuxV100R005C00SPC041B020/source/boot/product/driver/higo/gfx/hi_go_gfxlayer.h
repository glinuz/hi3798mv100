/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_gfxlayer.h
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         :
                       CNcomment: 开机logo图层适配层 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2016/01/12                  y00181162               Created file
******************************************************************************/

#ifndef  __HI_GO_GFXLAYER_H__
#define  __HI_GO_GFXLAYER_H__


/*********************************add include here******************************/

#include "hi_go_common.h"

/***************************** Macro Definition ******************************/
/** 这个要和驱动保持一致，否则无法释放 **/
#define DISPLAY_BUFFER_HD           "HIFB_DispBuf_HD"
#define DISPLAY_BUFFER_SD           "HIFB_DispBuf_SD"
#define HI_GFX_LAYER_MAXCOUNT       6

/*************************** Structure Definition ****************************/

typedef struct hiFbAlpha_S
{
    HI_BOOL bAlphaEnable;
    HI_BOOL bAlphaChannel;
    HI_U8 u8Alpha0;
    HI_U8 u8Alpha1;
    HI_U8 u8GlobalAlpha;
    HI_U8 u8Reserved;
}HI_FB_ALPHA_S;


typedef struct hiGfxDisplayInfo_S
{
    HI_U32 u32Stride;
    HI_U32 u32ScreenAddr;
    HI_GO_RECT_S stInRect;
    HI_GO_RECT_S stOutRect;
    HI_FB_ALPHA_S stAlpha;
}HI_GFX_DISPLAY_INFO_S;


/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
/*****************************************************************************
* func            : HI_GFX_ADP_CreateLayer
* description     : 创建图层
* in              : 图层参数
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GFX_ADP_CreateLayer(HI_GO_SURFACE_S *pstGfxSurface, HI_BOOL Support4KLogo);

/*****************************************************************************
* func            : HI_GFX_ADP_GetLayerInfo
* description     : 获取图层信息
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_ADP_GetLayerInfo(HI_GO_SURFACE_S *pstGfxSurface);


/*****************************************************************************
* func            : HI_GFX_ADP_RefreshLayer
* description     : 刷新图层
* in              : 图层参数
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GFX_ADP_RefreshLayer(HI_GO_SURFACE_S *pstGfxSurface, HI_BOOL Support4KLogo);


/*****************************************************************************
* func            : HI_GFX_ADP_DestoryLayer
* description     : 销毁图层
* in              : 图层参数
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_GFX_ADP_DestoryLayer(HI_GO_SURFACE_S *pstGfxSurface);


#endif /*__HI_GO_GFXLAYER_H__ */
