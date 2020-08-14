#ifndef __VDP_IP_DRV_VDM_H__
#define __VDP_IP_DRV_VDM_H__

#include "vdp_hal_comm.h"
#include "vdp_hal_ip_vdm.h"
//#include <stdio.h>

typedef enum
{
	VDP_VID_VDM_TYP =0,
	VDP_VID_VDM_TYP1 ,
	VDP_VID_VDM_RAND ,
	VDP_VID_VDM_MAX ,
	VDP_VID_VDM_MIN ,
	VDP_VID_VDM_ZERO ,
	VDP_VID_VDM_BUTT
}VDP_VID_VDM_MODE_E;

typedef enum 
{
  VDM_HDR10_IN_SDR_OUT = 0,
  VDM_BBC_IN_SDR_OUT   ,
  VDM_AVS_IN_SDR_OUT   , //new add
  VDM_HDR10_IN_BBC_OUT , 
  VDM_BBC_IN_HDR10_OUT ,
  VDM_AVS_IN_HDR10_OUT , //new add

  VDM_BT2020_IN_709_OUT,

  VDM_SDR_IN_HDR10_OUT ,//not use
  VDM_SDR_IN_BBC_OUT   ,//not use
  VDM_709_IN_BT2020_OUT,//not use
  VDM_BYPASS_MODE      ,
  VDM_BUTT
}VDM_SCENE_MODE_E;

typedef struct
{
    HI_U32 vdm_yuv2rgb_en    ; 
    HI_U32 vdm_en            ; 
    HI_U32 vdm_degmm_en      ; 
    HI_U32 vdm_csc_en        ; 
    HI_U32 vdm_tmap_en       ; 
    HI_U32 vdm_gmm_en        ; 
    HI_U32 vdm_dither_en     ; 
    HI_U32 vdm_rgb2yuv_en    ; 

    // Y2R PIP Ctrl
    HI_U32 vdm_v0_csc_en     ; 
    HI_U32 vdm_v1_csc_en     ; 
    HI_U32 vdm_v0_gamma_en   ; 
    HI_U32 vdm_v1_gamma_en   ; 

    HI_U32 vdm_csc_bind      ; 

    // Dither Ctrl
    HI_U32 u32_dither_round         ; 
    HI_U32 u32_dither_domain_mode   ; 
    HI_U32 u32_dither_tap_mode      ; 
    HI_U32 u32_dither_round_unlim   ; 

    //HDR Scene
    VDM_SCENE_MODE_E VdmSceneMode;
}VDP_VDM_CFG_S;

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


    HI_U32 *gmm_lut;		//???

}HDR_TF_PARA_S;

typedef struct _DITHER_REG_S{
	
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

}DITHER_REG_S;


typedef struct DmCfg_t_
{

//     HDR_Param_S   HDR_Param        ; 

    HDR_TF_PARA_S DePQPara         ; 
    HDR_TF_PARA_S TMPara           ; 
    HDR_TF_PARA_S GammaPara        ; 

    // YUV2RGB
    HI_S16 s16M33YUV2RGB0[3][3]    ; 
    HI_S16 s16M33YUV2RGB1[3][3]    ; 
    HI_U16 u16ScaleYUV2RGB         ; 
    HI_S32 s32InOffsetYUV2RGB0[3]  ; 
    HI_S32 s32InOffsetYUV2RGB1[3]  ; 
    HI_S32 s32OutOffsetYUV2RGB0[3] ; 
    HI_S32 s32OutOffsetYUV2RGB1[3] ; 
    HI_U16 u16ClipMinYUV2RGB       ; 
    HI_U16 u16ClipMaxYUV2RGB       ; 

    // CSC
    HI_S16 s16M33Src2Disp[3][3]    ; 
    HI_U16 u16ScaleSrc2Disp        ; 
    HI_U32 u32ClipMinCSC           ; 
    HI_U32 u32ClipMaxCSC           ; 

    // Tonemapping
    HI_U16 u16M3LumaCal[3]         ; 
    HI_U16 u16ScaleLumaCal         ; 
    HI_U16 u16ScaleCoefTM          ; 
    HI_U32 u32ClipMinTM            ; 
    HI_U32 u32ClipMaxTM            ; 
    HI_S32 s32TMOff0               ; 
    HI_S32 s32TMOff1               ; 
    HI_S32 s32TMOff2               ; 

    // Round or Dither
    HI_U16 u16ClipMinGmm           ; 
    HI_U16 u16ClipMaxGmm           ; 

    // RGB2YUV
    HI_U16 u16ScaleRGB2YUV         ; 
    HI_S16 s16M33RGB2YUV[3][3]     ; 
    HI_S16 s16InOffsetRGB2YUV[3]   ; 
    HI_S16 s16OutOffsetRGB2YUV[3]  ; 
    HI_U16 u16ClipMinRGB2YUV       ; 
    HI_U16 u16ClipMaxRGB2YUV       ; 

    // Module enable
    HI_U32 vdm_yuv2rgb_en          ; 
    HI_U32 vdm_en                  ; 
    HI_U32 vdm_degmm_en            ; 
    HI_U32 vdm_csc_en              ; 
    HI_U32 vdm_tmap_en             ; 
    HI_U32 vdm_gmm_en              ; 
    HI_U32 vdm_dither_en           ; 
    HI_U32 vdm_rgb2yuv_en          ; 

    // Y2R PIP Ctrl
    HI_U32 vdm_v0_csc_en           ; 
    HI_U32 vdm_v1_csc_en           ; 
    HI_U32 vdm_v0_gamma_en         ; 
    HI_U32 vdm_v1_gamma_en         ; 

    HI_U32 vdm_csc_bind            ; 
    HI_U32 vdm_dither_mode         ; // 0:12bit->10bit

    HI_U32 vdm_tmap_in_bdp         ; 

    DITHER_REG_S stDitherCfg      ;

    //HDR Scene
    VDM_SCENE_MODE_E VdmSceneMode;
} DmCfg_t; 


typedef struct Y2RCfg_t_
{
    // Y2R PIP Ctrl
    HI_U32 vdm_yuv2rgb_en          ; 
    HI_U32 vdm_v0_csc_en           ; 
    HI_U32 vdm_v1_csc_en           ; 
    HI_U32 vdm_v0_gamma_en         ; 
    HI_U32 vdm_v1_gamma_en         ; 

    HI_S16 s16M33YUV2RGB0[3][3]    ; 
    HI_S16 s16M33YUV2RGB1[3][3]    ; 
    HI_U16 u16ScaleYUV2RGB         ; 
    HI_S32 s32InOffsetYUV2RGB0[3]  ; 
    HI_S32 s32InOffsetYUV2RGB1[3]  ; 
    HI_S32 s32OutOffsetYUV2RGB0[3] ; 
    HI_S32 s32OutOffsetYUV2RGB1[3] ; 
    HI_U16 u16ClipMinYUV2RGB       ; 
    HI_U16 u16ClipMaxYUV2RGB       ; 
} Y2RCfg_t;

typedef struct R2YCfg_t_
{ 
    HI_U32 vdm_rgb2yuv_en          ; 

    HI_U16 u16ScaleRGB2YUV         ; 
    HI_S16 s16M33RGB2YUV[3][3]     ; 
    HI_S16 s16InOffsetRGB2YUV[3]   ; 
    HI_S16 s16OutOffsetRGB2YUV[3]  ; 
    HI_U16 u16ClipMinRGB2YUV       ; 
    HI_U16 u16ClipMaxRGB2YUV       ; 
}R2YCfg_t;

typedef enum 
{
    YUV2RGB_601   = 0,
    YUV2RGB_709   ,
    YUV2RGB_2020  ,
    YUV2RGB_CFG   ,
    YUV2RGB_BUTT
} YUV2RGB_MODE_E ;

typedef enum 
{
    RGB2YUV_601   = 0,
    RGB2YUV_709   ,
    RGB2YUV_2020  ,
    RGB2YUV_CFG   ,
    RGB2YUV_BUTT
} RGB2YUV_MODE_E ;

//function declare

HI_VOID VDP_FUNC_SetVdmMode(HI_U32 enLayer, VDP_VID_VDM_MODE_E VdmMode, VDP_VDM_CFG_S * pstCfg);

HI_VOID VDP_FUNC_SetYUV2RGB(HI_U32 enLayer, YUV2RGB_MODE_E Yuv2RgbMode, Y2RCfg_t* pY2RCfg);

HI_VOID VDP_FUNC_SetRGB2YUV(HI_U32 enLayer, RGB2YUV_MODE_E Rgb2YuvMode, R2YCfg_t *pR2YCfg);

#endif



