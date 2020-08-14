/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_hwc_capability.h
* Description: Graphic 2D engine hwc adp interface declaration
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include <linux/fs.h>

#include "hi_gfx2d_type.h"

#ifndef _GFX2D_HAL_HWC_ADP_H_
#define _GFX2D_HWC_HWC_ADP_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#define HWC_MAX_LAYERS 7

typedef struct tagHWC_CAPABILITY_S
{
    HI_U8 u8MaxLayerNum;  /*最大支持输入层个数*/
    HI_U8 u8MaxGLayerNum;   /*最大图形层个数*/
    HI_U8 u8MaxVLayerNum;   /*最大视频层个数*/
    HI_U32 au32InputFmtBitmap[2];
    HI_U32 au32OutputFmtBitmap[2];
    HI_U32 u32MinStride;
    HI_U32 u32MaxStride;
    HI_U8 u8StrideAlign;
    HI_U32 u32MinWidth;
    HI_U32 u32MaxWidth;
    HI_U32 u32MinHeight;
    HI_U32 u32MaxHeight;
    HI_U32 u32MinVWidth;    /*视频层最小宽度*/
    HI_U32 u32MinVHeight;   /*视频层最小高度*/
    HI_U32 u32MaxHorizonZmeRatio;   /*水平最大缩小倍数*/
    HI_U32 u32MaxVerticalZmeRatio;  /*垂直最大缩小倍数*/
    HI_U8 u32CmpWidthAlign;       /*压缩宽度对齐要求*/
    HI_U32 au32CmpFmtBitmap[2];     /*压缩支持的格式*/
    HI_BOOL bGZmeSupport;   /*图形层是否支持缩放*/
    HI_BOOL bClipSupport;   /*是否支持clip*/
    HI_U8 u8ZmeLayerNum;
    HI_U8 u8ZmeAlign;
} HWC_CAPABILITY_S;

/*******************************************************************************
* Function:      HWC_ADP_GetCapability
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 HWC_ADP_GetCapability(HWC_CAPABILITY_S *pstCapability);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif  /*_GFX2D_HWC_HWC_ADP_H_*/
