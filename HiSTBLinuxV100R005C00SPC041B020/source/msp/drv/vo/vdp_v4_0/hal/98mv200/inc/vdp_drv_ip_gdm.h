#ifndef __VDP_IP_DRV_GDM_H__
#define __VDP_IP_DRV_GDM_H__

#include "vdp_hal_comm.h"
#include "vdp_hal_ip_gdm.h"
#include "vdp_hal_ip_gdm.h"

typedef enum
{
	VDP_GP_GDM_TYP =0,
	VDP_GP_GDM_TYP1 ,
	VDP_GP_GDM_RAND ,
	VDP_GP_GDM_MAX ,
	VDP_GP_GDM_MIN ,
	VDP_GP_GDM_ZERO ,
	VDP_GP_GDM_BUTT
}VDP_GP_GDM_MODE_E;
typedef enum 
{
	GDM_SDR_IN_HDR10_OUT = 0,
    GDM_SDR_IN_BBC_OUT	,
	GDM_BYPASS_MODE 	,



	
    GDM_709_IN_BT2020_OUT,
    GDM_SDR_PREMULT,
	GDM_BUTT
}GDM_SCENE_MODE_E;
typedef struct
{
    //module enable
	HI_U32 gdm_en ;
    HI_U32 gdm_degmm_en;
    HI_U32 gdm_csc_en      ;
    HI_U32 gdm_tmap_en ;
    HI_U32 gdm_gmm_en  ;
    HI_U32 gdm_dither_en ;

    HI_U32 gdm_rgb2yuv_en ;

    //function ctrl
//     HI_U32 gdm_dither_mode ;
    HI_U32 gdm_r2y_premult_en;

    // dither ctrl
    HI_U32 u32_dither_round         ; 
    HI_U32 u32_dither_domain_mode   ; 
    HI_U32 u32_dither_tap_mode      ; 
    HI_U32 u32_dither_round_unlim   ;     

    //HDR Scene
    GDM_SCENE_MODE_E GdmSceneMode;

}VDP_GDM_CFG_S;

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

}
GDM_TF_PARA_S;

typedef struct _GDM_DITHER_REG_S{
	
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

}GDM_DITHER_REG_S;

typedef enum
{
	GDM_DITHER_FLAG_A= 0,
	GDM_DITHER_FLAG_Y= 1,
	GDM_DITHER_FLAG_U= 2,
	GDM_DITHER_FLAG_V= 3,
	GDM_DITHER_FLAG_BUTT
}GDM_DITHER_FLAG_E;



typedef struct GdmCfg_t_
{

//     HDR_Param_S   HDR_Param;

    GDM_TF_PARA_S DePQPara;
    GDM_TF_PARA_S TMPara;
    GDM_TF_PARA_S GammaPara;

    // YUV2RGB
    HI_S16 s16M33YUV2RGB0[3][3];
    HI_S16 s16M33YUV2RGB1[3][3];
    HI_U16 u16ScaleYUV2RGB; 
    HI_S32 s32InOffsetYUV2RGB0[3];
    HI_S32 s32InOffsetYUV2RGB1[3];
    HI_S32 s32OutOffsetYUV2RGB0[3];
    HI_S32 s32OutOffsetYUV2RGB1[3];
    HI_U16 u16ClipMinYUV2RGB ;
    HI_U16 u16ClipMaxYUV2RGB ;

    // CSC
    HI_S16 s16M33Src2Disp[3][3];
    HI_U16 u16ScaleSrc2Disp ;
    HI_U32 u32ClipMinCSC;
    HI_U32 u32ClipMaxCSC;

    // Tonemapping
    HI_U16 u16M3LumaCal[3];
    HI_U16 u16ScaleLumaCal;
    HI_U16 u16ScaleCoefTM;
    HI_U16 u16ClipMinTM;
    HI_U16 u16ClipMaxTM;

    // Round or Dither
    HI_U16 u16ClipMinGmm;
    HI_U16 u16ClipMaxGmm;

    // RGB2YUV
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
    HI_U32 vdm_csc_en      ;
    HI_U32 vdm_v1_gamma_en  ;
    HI_U32 vdm_v0_gamma_en  ;
    HI_U32 vdm_v1_csc_en    ;
    HI_U32 vdm_v0_csc_en    ;
    HI_U32 vdm_yuv2rgb_en ;
    HI_U32 vdm_rgb2yuv_en ;
    HI_U32 vdm_dither_en ;
    HI_U32 vdm_dither_mode ;
    HI_U32 gdm_r2y_premult_en;

	GDM_DITHER_REG_S stDitherCfg;

    //HDR Scene
    GDM_SCENE_MODE_E GdmSceneMode;

} GdmCfg_t;



//function declare

HI_VOID VDP_FUNC_SetGdmMode(HI_U32 enLayer, VDP_GP_GDM_MODE_E GdmMode, VDP_GDM_CFG_S * pstCfg);

#endif



