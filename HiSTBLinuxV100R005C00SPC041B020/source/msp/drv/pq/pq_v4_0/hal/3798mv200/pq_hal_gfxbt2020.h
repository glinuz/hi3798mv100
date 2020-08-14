/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************


  File Name     : pq_hal_gfxbt2020.h
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2016/03/22
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_GFXBT2020_H__
#define __PQ_HAL_GFXBT2020_H__


#include "pq_hal_gfxcsc.h"

#define GDM_GMM_LUT_SIZE      64

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



typedef struct
{
    
    HI_U32 gmm_x1_step;
    HI_U32 gmm_x2_step;
    HI_U32 gmm_x3_step;
    HI_U32 gmm_x4_step;
    HI_U32 gmm_x5_step;
    HI_U32 gmm_x6_step;
    HI_U32 gmm_x7_step;
    HI_U32 gmm_x8_step;

    HI_U32 gmm_x1_pos;
    HI_U32 gmm_x2_pos;
    HI_U32 gmm_x3_pos;
    HI_U32 gmm_x4_pos;
    HI_U32 gmm_x5_pos;
    HI_U32 gmm_x6_pos;
    HI_U32 gmm_x7_pos;
    HI_U32 gmm_x8_pos;

    HI_U32 gmm_x1_num;
    HI_U32 gmm_x2_num;
    HI_U32 gmm_x3_num;
    HI_U32 gmm_x4_num;
    HI_U32 gmm_x5_num;
    HI_U32 gmm_x6_num;
    HI_U32 gmm_x7_num;
    HI_U32 gmm_x8_num;


    HI_U32 *gmm_lut;

}PQ_HAL_GDM_TF_PARA_S;

typedef struct
{
	HI_U32 u32_dither_round      ;
	HI_U32 u32_dither_domain_mode;
	HI_U32 u32_dither_tap_mode   ;
	HI_U32 u32_dither_round_unlim;
	HI_U32 u32_dither_thr_max    ;
	HI_U32 u32_dither_thr_min    ;
	HI_U32 u32_dither_sed_y0     ;
	HI_U32 u32_dither_sed_u0     ;
    HI_U32 u32_dither_sed_v0     ;
	HI_U32 u32_dither_sed_w0     ;
	HI_U32 u32_dither_sed_y1     ;
	HI_U32 u32_dither_sed_u1     ;
	HI_U32 u32_dither_sed_v1     ;
	HI_U32 u32_dither_sed_w1     ;

}PQ_HAL_GDM_DITHER_REG_S;

typedef struct
{
    //HDR_Param
    PQ_HAL_GDM_TF_PARA_S DePQPara;
    PQ_HAL_GDM_TF_PARA_S TMPara;
    PQ_HAL_GDM_TF_PARA_S GammaPara;

    //YUV2RGB
    HI_S16 s16M33YUV2RGB0[3][3];
    HI_S16 s16M33YUV2RGB1[3][3];
    HI_U16 u16ScaleYUV2RGB;
    HI_S32 s32InOffsetYUV2RGB0[3];
    HI_S32 s32InOffsetYUV2RGB1[3];
    HI_S32 s32OutOffsetYUV2RGB0[3];
    HI_S32 s32OutOffsetYUV2RGB1[3];
    HI_U16 u16ClipMinYUV2RGB ;
    HI_U16 u16ClipMaxYUV2RGB ;

    //CSC
    HI_S16 s16M33Src2Disp[3][3];
    HI_U16 u16ScaleSrc2Disp ;
    HI_U32 u32ClipMinCSC;
    HI_U32 u32ClipMaxCSC;

    //Tonemapping
    HI_U16 u16M3LumaCal[3];
    HI_U16 u16ScaleLumaCal;
    HI_U16 u16ScaleCoefTM;
    HI_U16 u16ClipMinTM;
    HI_U16 u16ClipMaxTM;

    //Round or Dither
    HI_U16 u16ClipMinGmm;
    HI_U16 u16ClipMaxGmm;

    //RGB2YUV
    HI_U16 u16ScaleRGB2YUV;
    HI_S16 s16M33RGB2YUV[3][3];
    HI_S16 s16InOffsetRGB2YUV[3];
    HI_S16 s16OutOffsetRGB2YUV[3];
    HI_U16 u16ClipMinRGB2YUV;
    HI_U16 u16ClipMaxRGB2YUV;

    //ctrl signal
    HI_U32 vdm_tmap_in_bdp;
    HI_U32 vdm_csc_bind;
    HI_U32 vdm_gmm_en  ;
    HI_U32 vdm_tmap_en ;
    HI_U32 vdm_degmm_en;
    HI_U32 vdm_en      ;
    HI_U32 vdm_csc_en  ;
    HI_U32 vdm_v1_gamma_en;
    HI_U32 vdm_v0_gamma_en;
    HI_U32 vdm_v1_csc_en  ;
    HI_U32 vdm_v0_csc_en  ;
    HI_U32 vdm_yuv2rgb_en ;
    HI_U32 vdm_rgb2yuv_en ;
    HI_U32 vdm_dither_en  ;
    HI_U32 vdm_dither_mode;
    HI_U32 gdm_r2y_premult_en;

    PQ_HAL_GDM_DITHER_REG_S stDitherCfg;

} PQ_HAL_GDMCFG_S;



HI_S32  PQ_HAL_GfxBT2020MemInit(HI_VOID);
HI_VOID PQ_HAL_GfxGamLutInit(HI_VOID);
HI_S32  PQ_HAL_GfxBT2020DeInit(HI_VOID);
HI_VOID PQ_HAL_SetGfxBt2020Mode(PQ_HAL_GFX_CSC_MODE_S* pstPqCscMode);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


