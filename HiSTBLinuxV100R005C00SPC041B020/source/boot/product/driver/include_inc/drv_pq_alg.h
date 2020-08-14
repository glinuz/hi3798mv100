/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_pq_alg.h
  Version       : Initial Draft
  Author        : Hisilicon PQ software group
  Created       : 2013/07/30
  Description   : pq alg define

******************************************************************************/

#ifndef __DRV_PQ_ALG_H__
#define __DRV_PQ_ALG_H__

#include "hi_type.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif/*cplusplus*/

#define STR_LEN_8                8
#define STR_LEN_32              32
#define STR_LEN_80              80
#define STR_LEN_256            256
#define STR_LEN_1024          1024
#define GAMMA_NUM             256
#define ACM_Y_NUM            5
#define ACM_H_NUM            29
#define ACM_S_NUM            7
#define ACC_CRV_NUM            5
#define ACC_CRV_RESOLUTION    9
#define CSC_CUSTOM_ROW      3
#define CSC_CUSTOM_COL       5


typedef enum hi_PQ_CHAN_E
{
    PQ_CHAN0 = 0,
    PQ_CHAN1,
    PQ_CHAN2,
    PQ_CHAN_BUTT,
} PQ_CHAN_E;

typedef enum hi_GAMMA_CS_E
{
    GAMMA_MODE_RGB = 0,
    GAMMA_MODE_YUV,
    GAMMA_MODE_BUTT,
} GAMMA_CS_E;

typedef enum hi_GAMMA_CHAN_E
{
    GAMMA_CHAN_DISPLAY0 = 0,
    GAMMA_CHAN_DISPLAY1,
    GAMMA_CHAN_BUTT,

} GAMMA_CHAN_E;

typedef enum hi_GAMMA_MODE_E
{
    GAMMA_PQ_MODE_0          = 0,
    GAMMA_PQ_MODE_1            ,
    GAMMA_PQ_MODE_2            ,
    GAMMA_PQ_MODE_3            ,  
    GAMMA_PQ_MODE_CLOSE        ,
    GAMMA_PQ_MODE_BUTT

} GAMMA_MODE_E;

typedef enum hi_ACC_MODE_E
{
    ACC_PQ_MODE_GENTLE          = 0,
    ACC_PQ_MODE_MIDDLE            ,
    ACC_PQ_MODE_STRONG            , 
    ACC_PQ_MODE_CLOSE        ,
    ACC_PQ_MODE_BUTT
} ACC_MODE_E;

typedef enum hi_ACM_MODE_E
{
    ACM_PQ_MODE_BLUE          = 0,
    ACM_PQ_MODE_GREEN            ,
    ACM_PQ_MODE_BG            ,
    ACM_PQ_MODE_SKIN            ,
    ACM_PQ_MODE_VIVID            ,  
    ACM_PQ_MODE_CLOSE        ,
    ACM_PQ_MODE_BUTT
} ACM_MODE_E;

typedef enum hi_DNR_MODE_E
{
    DNR_MODE_DISABLED          = 0,
    DNR_MODE_AUTO            ,
    DNR_MODE_MODE_BUTT
} DNR_MODE_E;

typedef enum hi_FILM_MODE_E
{
    FILM_MODE_DISABLED          = 0,
    FILM_MODE_AUTO            ,
    FILM_MODE_MODE_BUTT
} FILM_MODE_E;


typedef enum hi_SHARP_CHAN_E
{
    SHARP_CHAN_HD = 0,
    SHARP_CHAN_SD,
    
    SHARP_CHAN_GP0,
    SHARP_CHAN_GP1,
    SHARP_CHAN_BUTT,

} SHARP_CHAN_E;

typedef enum hi_CSC_CHAN_E
{
    CSC_CHAN_VDP_V0 = 0,
    CSC_CHAN_VDP_V1,
    CSC_CHAN_VDP_V3,
    CSC_CHAN_VDP_V4,
    CSC_CHAN_VDP_G0,
    CSC_CHAN_VDP_G1,
    CSC_CHAN_VDP_WBC_DHD0,
    CSC_CHAN_VDP_INTER_DHD0,
    CSC_CHAN_VDP_INTER_DHD1,
    CSC_CHAN_VDP_BUTT,
} CSC_CHAN_E;

typedef enum hi_OPTION_MODE_E
{
    OPTION_MODE0 = 0,
    OPTION_MODE1,
    OPTION_MODE2,
    OPTION_MODE3,    
    OPTION_MODE_BUTT,
} OPTION_MODE_E;

typedef enum hi_TEMP_MODE_E
{
    TEMP_MODE_COLD = 0,
    TEMP_MODE_MIDDLE,
    TEMP_MODE_WARM,  
    TEMP_MODE_BUTT,
} TEMP_MODE_E;

typedef enum hi_OPT_CHOICE_MODE_E
{
    CHOICE_MODE_WORKING = 0,
    CHOICE_MODE_DEFAULT, 
    CHOICE_MODE_BUTT,
} OPT_CHOICE_MODE_E;

/////////////////////////////////////////////////define option///////////////////////////////////////////////////////////////
typedef struct hi_PQ_OPT_CHAN0_S
{
    HI_U32       u32Brightness;
    HI_U32       u32Contrast;
    HI_U32       u32Hue;
    HI_U32       u32Saturation;
    HI_U32       u32colortemperature;
    HI_U32       u32GammaMode;
} PQ_OPT_CHAN0_S;
typedef struct hi_PQ_OPT_CHAN1_S
{
    HI_U32       u32Brightness;
    HI_U32       u32Contrast;
    HI_U32       u32Hue;
    HI_U32       u32Saturation;
    HI_U32       u32colortemperature;
    HI_U32       u32DynamicContrast;	
    HI_U32       u32IntelligentColor;
    HI_U32       u32GammaMode;    
} PQ_OPT_CHAN1_S;

typedef struct hi_PQ_OPT_COMMOM_S
{
    HI_U32       u32Sharpeness;      /* Sharpeness  锐化强度 */                
    HI_U32       u32Denoise;         /* Denoise  降噪使能*/       
    HI_U32       u32FilmMode;        /* FilmMode 电影模式使能选择*/  
} PQ_OPT_COMMON_S;

typedef struct hi_PQ_OPT_IMAGE_S
{
    HI_U32       u32OptionChan0Mode;
    HI_U32       u32OptionChan1Mode;	
} PQ_OPT_IMAGE_S;
typedef struct hi_PQ_OPT_COEF_S
{
    PQ_OPT_IMAGE_S	 stOptImage;
    PQ_OPT_COMMON_S	 stOptCommon;
    PQ_OPT_CHAN0_S  stOptChan0[OPTION_MODE_BUTT];
    PQ_OPT_CHAN1_S  stOptChan1[OPTION_MODE_BUTT];	
} PQ_OPT_COEF_S;

/////////////////////////////////////////////////define coef///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////define dei///////////////////////////////////////////////////////////////
typedef struct hi_PQ_DEI_CTRL
{
    //HI_U32 u32Dei_en;                  //the enable of deinterlace module
    HI_U32 u32Die_out_sel_l;        //the output selection mode of luma DEI:0-interpolation; 1-interleave
    HI_U32 u32Die_out_sel_c;        //the output selection mode of chroma DEI:0-interpolation; 2-interleave
    HI_U32 u32Die_lmmode;         //the mode of luma deinterlace: 0-5 field mode;1-4 field mode;2-3 field mode;3-reserved
    HI_U32 u32Die_cmmode;         //the mode of chroma deinterlace: 0-5 field mode;1-4 field mode;2-3 field mode;4-reserved
    //HI_U32 u32Die_rst;                //the reset signal: 0-reset invalid;1-reset valid
    //HI_U32 u32stinfo_stop;          //the reset signal: 0-reset invalid;1-reset valid

} PQ_DEI_CTRL;

typedef struct hi_PQ_DEI_GLOBAL_MOTION_CTRL
{
    HI_U32 u32Die_sad_thd;                  //the threshold of SAD
    HI_U32 u32Adjust_gain;                  //the adjust factor of motion
    HI_U32 u32Luma_mf_max;              //motion detect selection of luma: 0-computing value;2-fixed to maximum value
    HI_U32 u32Chroma_mf_max;          //motion detect selection of chroma: 0-computing value;1-fixed to maximum value
    HI_U32 u32Luma_scesdf_max;        //the selection mode of luma SAD:0-minimum;1-maximum

} PQ_DEI_GLOBAL_MOTION_CTRL;


typedef struct hi_PQ_DEI_DIR_CHECK
{
    HI_U32 u32Ck1_gain;                            //the gain of check 1
    HI_U32 u32Ck1_range_gain;                  //the range gain of check 1
    HI_U32 u32Ck1_max_range;                 //the maximum range of check 1
    HI_U32 u32Ck2_gain;                           //the gain of check 2
    HI_U32 u32Ck2_range_gain;                //the range gain of check 2
    HI_U32 u32Ck2_max_range;               //the maximum range of check 2
} PQ_DEI_DIR_CHECK;

typedef struct hi_PQ_DEI_DIR_MULTI
{
    HI_U32 u32Dir0_mult;                            //direction 0 multiplier
    HI_U32 u32Dir1_mult;                            //direction 1 multiplier
    HI_U32 u32Dir2_mult;                            //direction 2 multiplier
    HI_U32 u32Dir3_mult;                            //direction 3 multiplier
    HI_U32 u32Dir4_mult;                            //direction 4 multiplier
    HI_U32 u32Dir5_mult;                            //direction 5 multiplier
    HI_U32 u32Dir6_mult;                            //direction 6 multiplier
    HI_U32 u32Dir7_mult;                            //direction 7 multiplier
    HI_U32 u32Dir8_mult;                            //direction 8 multiplier
    HI_U32 u32Dir9_mult;                            //direction 9 multiplier
    HI_U32 u32Dir10_mult;                            //direction 10 multiplier
    HI_U32 u32Dir11_mult;                            //direction 11 multiplier
    HI_U32 u32Dir12_mult;                            //direction 12 multiplier
    HI_U32 u32Dir13_mult;                            //direction 13 multiplier
    HI_U32 u32Dir14_mult;                            //direction 14 multiplier
} PQ_DEI_DIR_MULTI;

typedef struct hi_PQ_DEI_INTP_SCALE_RATIO
{
    HI_U32 u32Intp_scale_ratio_1;                            //the strength fator of direction 1
    HI_U32 u32Intp_scale_ratio_2;                            //the strength fator of direction 2
    HI_U32 u32Intp_scale_ratio_3;                            //the strength fator of direction 3
    HI_U32 u32Intp_scale_ratio_4;                            //the strength fator of direction 4
    HI_U32 u32Intp_scale_ratio_5;                            //the strength fator of direction 5
    HI_U32 u32Intp_scale_ratio_6;                            //the strength fator of direction 6
    HI_U32 u32Intp_scale_ratio_7;                            //the strength fator of direction 7
    HI_U32 u32Intp_scale_ratio_8;                            //the strength fator of direction 8
    HI_U32 u32Intp_scale_ratio_9;                            //the strength fator of direction 9
    HI_U32 u32Intp_scale_ratio_10;                            //the strength fator of direction 10
    HI_U32 u32Intp_scale_ratio_11;                            //the strength fator of direction 11
    HI_U32 u32Intp_scale_ratio_12;                            //the strength fator of direction 12
    HI_U32 u32Intp_scale_ratio_13;                            //the strength fator of direction 13
    HI_U32 u32Intp_scale_ratio_14;                            //the strength fator of direction 14
    HI_U32 u32Intp_scale_ratio_15;                            //the strength fator of direction 15

} PQ_DEI_INTP_SCALE_RATIO;

typedef struct hi_PQ_DEI_INTP_CTRL
{
    HI_U32 u32Strength_thd;                            //the strength threshold
    HI_U32 u32Dir_thd;                            //the direction select threshold
    HI_U32 u32Bc_gain;                            //the weight factor of check module
    HI_U32 u32Ver_min_inten;                            //the minimum interpolation strength of vertical direction
    HI_U32 u32Dir_inten_ver;                            //the strength multiplier of vertical interpolation
    HI_U32 u32Range_scale;                            //the strength multiplier of the best direction interpolation

} PQ_DEI_INTP_CTRL;

typedef struct hi_PQ_DEI_JITTER_MOTION
{
    HI_U32 u32Jitter_mode;                            //the compute mode of jitter:0-horizontal texture;1-the minimum of horizontal texture and edge
    HI_U32 u32Jitter_factor;                            //the normalization weight of horizontal HPF
    HI_U32 u32Jitter_coring;                            //the coring value of jitter
    HI_U32 u32Jitter_gain;                            //the gain value of jitter
    HI_S32 s32Jitter_filter_0;                            //coefficient 0 of horizontal HPF
    HI_S32 s32Jitter_filter_1;                            //coefficient 1 of horizontal HPF
    HI_S32 s32Jitter_filter_2;                            //coefficient 2 of horizontal HPF
} PQ_DEI_JITTER_MOTION;

typedef struct hi_PQ_DEI_FIELD_MOTION
{
    HI_U32 u32Fld_motion_coring;                            //the coring value of field motion
    HI_U32 u32Fld_motion_gain;                               //the gain value of field motion
    HI_U32 u32Fld_motion_curve_slope;                   //the slope of field motion adjust curve
    HI_U32 u32Fld_motion_thd_low;                         //the low threshold of field motion adjust curve
    HI_U32 u32Fld_motion_thd_high;                         //the high threshold of field motion adjust curve
} PQ_DEI_FIELD_MOTION;

typedef struct hi_PQ_DEI_MOTION_RATIO_CURVE
{
    HI_U32 u32Mtion_ratio_en;                            //the enable of motion adjust curve
    HI_U32 u32Lm_avg_thd_0;                               //the threshold 0 of average luminance
    HI_U32 u32Lm_avg_thd_1;                              //the threshold 1 of average luminance
    HI_U32 u32Lm_avg_thd_2;                         //threshold 2 of average luminance
    HI_U32 u32Lm_avg_thd_3;                         //the threshold 3 of average luminance
    HI_S32 s32Mtion_curve_slope_0;                            //the slope 0 of motion adjust curve
    HI_S32 s32Mtion_curve_slope_1;                           //the slope 1of motion adjust curve
    HI_S32 s32Mtion_curve_slope_2;                           //the slope 2of motion adjust curve
    HI_S32 s32Mtion_curve_slope_3;                           //the slope 3of motion adjust curve
    HI_U32 u32Sart_motion_ratio;                            //the start value of motion adjust curve
    HI_U32 u32Mn_motion_ratio;                            //the minimum value of motion adjust curve
    HI_U32 u32Mx_motion_ratio;                            //the maximum value of motion adjust curve
    HI_U32 u32Mtion_curve_ratio_0;                            //the RATIO 0 of motion adjust curve,must be consistent with luminance average and slope.
    HI_U32 u32Mtion_curve_ratio_1;                           //the RATIO 1 f motion adjust curve,must be consistent with luminance average and slope.
    HI_U32 u32Mtion_curve_ratio_2;                           //the RATIO 2 f motion adjust curve,must be consistent with luminance average and slope.
    HI_U32 u32Mtion_curve_ratio_3;                           //the RATIO 3 f motion adjust curve,must be consistent with luminance average and slope.

} PQ_DEI_MOTION_RATIO_CURVE;


typedef struct hi_PQ_DEI_IIR_MOTION_CURVE
{
    HI_U32 u32Mtion_iir_en;                                  //the enable of history motion IIR filter
    HI_U32 u32Mtion_diff_thd_0;                            //the threshold 0 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_1;                           //the threshold 1 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_2;                           //the threshold 2 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_3;                           //the threshold 3 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_4;                           //the threshold 4 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_5;                           //the threshold 5 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_6;                           //the threshold 6 of IIR filter curve
    HI_U32 u32Mtion_diff_thd_7;                           //the threshold 7 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_0;                            //the slope 0 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_1;                           //the slope 1 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_2;                             //the slope 2 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_3;                           //the slope 3 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_4;                           //the slope 4 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_5;                           //the slope 5 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_6;                           //the slope 6 of IIR filter curve
    HI_S32 s32Mtion_iir_curve_slope_7 ;                          //the slope 7 of IIR filter curve
    HI_U32 u32Start_motion_iir_ratio;                              //the start value of IIR filter curve
    HI_U32 u32Min_motion_iir_ratio;                             //the minimum value of IIR filter curve
    HI_U32 u32Max_motion_iir_ratio;                            //the maximum value of IIR filter curve
    HI_U32 u32Motion_iir_curve_ratio_0;                       //the RATIO 0 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_1;                      //the RATIO 1 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_2;                      //the RATIO 2 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_3;                      //the RATIO 3 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_4;                      //the RATIO 4 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_5;                      //the RATIO 5 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_6;                      //the RATIO 6 of IIR filter curve,must be consistent with threshold and slope
    HI_U32 u32Motion_iir_curve_ratio_7;                      //the RATIO 7 of IIR filter curve,must be consistent with threshold and slope
} PQ_DEI_IIR_MOTION_CURVE;

typedef struct hi_PQ_DEI_REC_MODE
{
    HI_U32 u32Rec_mode_en;                                         //the enable of recursive mode
    HI_U32 u32Rec_mode_write_mode;                            //write mode of recursive mode
    HI_U32 u32Rec_mode_mix_mode;                              //mix mode of recursive mode
    HI_U32 u32Rec_mode_scale;                                      //the scale factor of recursive mode
    HI_U32 u32Rec_mode_fld_motion_gain;                             //the gain value of recursive field motion
    HI_U32 u32Rec_mode_fld_motion_coring;                           //the coring value of recursive field motion
    HI_U32 u32Rec_mode_motion_thd;                                     //the motion threshold of recursive mode
    HI_U32 u32Rec_mode_fld_motion_step_0;                           //the step 0 of recursive field motion
    HI_U32 u32Rec_mode_fld_motion_step_1;                           //rec_mode_fld_motion_step_1
    HI_U32 u32Rec_mode_frm_motion_step_0;                            //rec_mode_frm_motion_step_0
    HI_U32 u32Rec_mode_frm_motion_step_1;                            //rec_mode_frm_motion_step_1
} PQ_DEI_REC_MODE;

typedef struct hi_PQ_DEI_HIST_MOTION
{
    HI_U32 u32His_motion_en;                                          //the enable of recursive mode
    HI_U32 u32His_motion_write_mode;                            //the write mode of history motion
    HI_U32 u32His_motion_using_mode;                           //the using mode of history motion
    HI_U32 u32Pre_info_en;                                             //the enable of previous history motion
    HI_U32 u32Ppre_info_en;                                           //the enable of before previous history motion

} PQ_DEI_HIST_MOTION;

typedef struct hi_PQ_DEI_MOR_FLT
{
    HI_U32 u32Mor_flt_en;                                          //the enable of morphological filter
    HI_U32 u32Mor_flt_size;                            //the window size of morphological filter：0-1x9；1-1x7；2-1x5
    HI_U32 u32Mor_flt_thd;                           //the threshold of morphological filter

} PQ_DEI_MOR_FLT;


typedef struct hi_PQ_DEI_OPTM_MODULE
{
    HI_U32 u32med_blend_en;                                          //the median filter enable of temporal interpolation
    HI_U32 u32deflicker_en;                                             //the deflicker enable

} PQ_DEI_OPTM_MODULE;

typedef struct hi_PQ_DEI_COMB_CHK
{
    HI_U32 u32Comb_chk_en;                                                        //the enable of comb feature detection
    HI_U32 u32Comb_chk_md_thd;                                                //the motion threshold of comb feature detection
    HI_U32 u32Comb_chk_edge_thd;                                             //the edge strength threshold of comb feature detection
    HI_U32 u32Comb_chk_upper_limit;                                             //the upper threshold of comb feature detection
    HI_U32 u32Comb_chk_lower_limit;                                             //the lower threshold of comb feature detection
    HI_U32 u32Comb_chk_min_vthd;                                             //the minimum vertical different of comb feature detection
    HI_U32 u32Comb_chk_min_hthd;                                             //the minimum horizontal different of comb feature detection

} PQ_DEI_COMB_CHK;


typedef struct hi_PQ_DEI_CRS_CLR
{
    HI_U32 u32Chroma_ccr_en;                                                        //the enable of cross-color reduction
    HI_U32 u32Chroma_ma_offset;                                                   //the offset between chroma motion and luma motion
    HI_U32 u32No_ccr_detect_max;                                                 //the maximum value of cross-color detection
    HI_U32 u32No_ccr_detect_thd;                                                  //the threshold of cross-color detection
    HI_U32 u32Similar_max;                                                           //the maximum value of similarity
    HI_U32 u32Similar_thd;                                                             //the threshold value of similarity
    HI_U32 u32No_ccr_detect_blend;                                              //the mix ratio of cross-colr detection
    HI_U32 u32Max_xchroma;                                                       //the maximum change value of chroma
} PQ_DEI_CRS_CLR;
typedef struct hi_PQ_DEI_COEF_S
{
    PQ_DEI_CTRL                               stPqDeiCtrl;
    PQ_DEI_GLOBAL_MOTION_CTRL       stPqDeiGlobalMotionCtrl;
    PQ_DEI_DIR_CHECK                       stPqDeiDirCheck;
    PQ_DEI_DIR_MULTI                        stPqDeiDirMulti;
    PQ_DEI_INTP_SCALE_RATIO            stPqDeiIntpScaleRatio;
    PQ_DEI_INTP_CTRL                        stPqDeiIntpCtrl;
    PQ_DEI_JITTER_MOTION                 stPqDeiJitterMotion;
    PQ_DEI_FIELD_MOTION                   stPqDeiFieldMotion;
    PQ_DEI_MOTION_RATIO_CURVE        stPqDeiMotionRatioCurve;
    PQ_DEI_IIR_MOTION_CURVE             stPqDeiIirMotionCurve;
    PQ_DEI_REC_MODE                         stPqDeiRecMode;
    PQ_DEI_HIST_MOTION                     stPqDeiHistMotion;
    PQ_DEI_MOR_FLT	                       stPqDeiMorFlt;
    PQ_DEI_OPTM_MODULE                    stPqDeiOptmModule;
    PQ_DEI_COMB_CHK                          stPqDeiCombChk;
    PQ_DEI_CRS_CLR                             stPqDeiCrsClr;

} PQ_DEI_COEF_S;

///////////////////////////////////////////////define fmd ////////////////////////////////////////////////////

typedef struct hi_PQ_FMD_CTRL_S
{
    HI_U32 u32Pd32_en;                       //pulldown 3:2 detection enable
    HI_U32 u32Pd22_en;                       //pulldown 2:2 detection enable
    HI_U32 u32Fod_en_mode;                       //field order detection enable:
    HI_U32 u32Pd22_mode;                       //pulldown 2:2 detection mode:
    HI_U32 u32Edge_smooth_en;                       //edgesmooth enable
    HI_U32 u32Edge_smooth_ratio;                       //edgesmooth ratio
    HI_U32 u32Bitsmov2r;                       //iCHD calculation para-right shift bits: 0：right shift 6bit;

} PQ_FMD_CTRL_S;


typedef struct hi_PQ_FMD_HISTBIN_THD_S
{
    HI_U32 u32Hist_thd0;                       //histgram threshold 0
    HI_U32 u32Hist_thd1;                       //histgram threshold 1
    HI_U32 u32Hist_thd2;                       //histgram threshold 2
    HI_U32 u32Hist_thd3;                       //histgram threshold 3
    HI_U32 u32Mon_start_idx;                       //start bin number mark for calculating bin-monotonic:1-15
    HI_U32 u32Mon_tkr_thr;                       //bin-monotonic threshold
    HI_U32 u32Diff_movblk_thd;                       //difference threshold of moving block
    HI_U32 u32Stillblk_thd;                       //still block threshold

} PQ_FMD_HISTBIN_THD_S;

typedef struct hi_PQ_FMD_PCC_THD_S
{
    HI_U32 u32Mov_coring_norm;                       //moving coring value for normal pixel
    HI_U32 u32Mov_coring_tkr;                       //moving coring value for texture
    HI_U32 u32Coring_norm;                       //pilority change coring for normal pixel
    HI_U32 u32Coring_tkr;                       //pilority change coring for texture pixel
    HI_U32 u32Pcc_hthd;                       //horizontal threshold
    HI_U32 u32Pcc_vthd0;                       //vertical threshold 0
    HI_U32 u32Pcc_vthd1;                       //vertical threshold 1
    HI_U32 u32Pcc_vthd2;                       //vertical threshold 2
    HI_U32 u32Pcc_vthd3;                       //vertical threshold 3
} PQ_FMD_PCC_THD_S;

typedef struct hi_PQ_FMD_PCC_BLK_S
{
    HI_U32 u32Blk_x0;                       //x value of the top left pixel
    HI_U32 u32Blk_y0;                       //y value of the top left pixel
    HI_U32 u32Blk_x1;                       //x value of the bottom right pixel
    HI_U32 u32Blk_y1;                       //y value of the bottom right pixel
    HI_U32 u32Mov_coring_blk;                       //moving coring value of block computation
    HI_U32 u32Coring_blk;                       //pilority change coring value of block computation

} PQ_FMD_PCC_BLK_S;

typedef struct hi_PQ_FMD_UM_THD_S
{
    HI_U32 u32Um_thd0;                       //match threshold 0
    HI_U32 u32Um_thd1;                       //match threshold 1
    HI_U32 u32Um_thd2;                       //match threshold 2

} PQ_FMD_UM_THD_S;

typedef struct hi_PQ_FMD_ITDIFF_THD_S
{
    HI_U32 u32Itdiff_vthd0;                       //match threshold 0
    HI_U32 u32Itdiff_vthd1;                       //IT different threshold 1
    HI_U32 u32Itdiff_vthd2;                       //IT different threshold 2
    HI_U32 u32Itdiff_vthd3;                       //IT different threshold 3

} PQ_FMD_ITDIFF_THD_S;

typedef struct hi_PQ_FMD_LASI_THD_S
{
    HI_U32 u32Lasi_mode;                       //lasi computation mode:0-new mode;1-old
    HI_U32 u32Lasi_thd;                       //lasi threshold
    HI_U32 u32Edge_thd;                       //edge threshold
    HI_U32 u32Lasi_mov_thr;                       //moving threshold of lasi

} PQ_FMD_LASI_THD_S;


typedef struct hi_PQ_FMD_COEF_S
{
    PQ_FMD_CTRL_S  stPqFmdCtrl;
    PQ_FMD_HISTBIN_THD_S stPqFmdHistbinThd;
    PQ_FMD_PCC_THD_S stPqFmdPccThd;
    PQ_FMD_PCC_BLK_S stPqFmdPccBlk;
    PQ_FMD_UM_THD_S	stPqFmdUmThd;
    PQ_FMD_ITDIFF_THD_S stPqFmdItdiffThd;
    PQ_FMD_LASI_THD_S stPqFmdLashThd;

} PQ_FMD_COEF_S;
/////////////////////////////////////////////////define dnr///////////////////////////////////////////////////////////////
typedef struct hi_PQ_DNR_CTRL_S
{
    HI_U32 u32Dr_en;                                 //DR filter enbale
    HI_U32 u32Db_en;                                //DB filter enbale
    HI_U32 u32Db_en_vert;                        //DB vertical filter enable:0-off;1-on
    HI_U32 u32Db_en_hor;                         //DB horizontal filter enable:0-off;1-on
} PQ_DNR_CTRL_S;

typedef struct hi_PQ_DNR_DR_FILTER_S
{
    HI_U32 u32Dr_thr_flat3x3zone;                //contrast threshold for DR 3*3 block
    HI_U32 u32Dr_thr_maxsimilarpixdiff;        //pixel difference similarity threshold for DR
    HI_U32 u32Dr_betascale;                        //beta factor for DR
    HI_U32 u32Dr_alphascale;                      //alpha factor for DR
} PQ_DNR_DR_FILTER_S;

typedef struct hi_PQ_DNR_DB_FILTER_S
{
    HI_U32 u32db_thr_maxgrad;                //maximum gratitude threshold for DB filter
    HI_U32 u32db_thr_edge;                     //edge threshold for DB filter
    HI_U32 u32picest_qp;                         //Qp for DB filter
    HI_U32 u32db_text_en;                      //texture filter enable for DB
    HI_U32 u32db_useweakflt;                  //strong filter mode select for DB:0- 9 tap strong filter;1- 7 tap strong filter
    HI_U32 u32db_thr_maxdiffvert;           //maximum block different of vertical direction  for DB
    HI_U32 u32db_thr_leastblkdiffvert;      //maximum block different of vertical direction  for DB
    HI_U32 u32db_thr_maxdiffhor;             //maximum block different of horizontal direction  for DB
    HI_U32 u32db_thr_leastblkdiffhor;       //maximum block different of horizontal direction  for DB
    HI_U32 u32db_betascale;                    //beta factor for DB
    HI_U32 u32db_alphascale;                   //alpha factor for DB
    HI_U32 u32db_thr_largesmooth;          //smooth region threshold for DB
    HI_U32 u32detail_img_qpthr;               //Qp threshold for DB
} PQ_DNR_DB_FILTER_S;

typedef struct hi_PQ_DNR_INFO_S
{
    HI_U32 u32thd_cntrst8;                                 //contrast threshold of 8*8 block for selecting DR block
    HI_U32 u32thd_maxgrad;                              //maximum gratitude threshold of 8*8 block for selecting DR
    HI_U32 u32thr_intl_cnt;                                //counter threshold of one column for IP detection
    HI_U32 u32thr_intl_colcnt;                            //counter threshold of one block for IP detection
} PQ_DNR_INFO_S;

typedef struct hi_PQ_DNR_COEF_S
{
    PQ_DNR_CTRL_S stPqDnrCtrl;
    PQ_DNR_DB_FILTER_S stPqDnrDbFilter;
    PQ_DNR_DR_FILTER_S stPqDnrDrFilter;
    PQ_DNR_INFO_S         stPqDnrInfo;
} PQ_DNR_COEF_S;

////////////////////////////////////////////define sharp////////////////////////////////////////////////////////////
typedef struct hi_PQ_SHARP_LUMA_S
{
    HI_U32 u32Lti_en;                                 //luma sharpen enable
    HI_U32 u32SharpIntensity;                               //Sharp Intensity
    HI_U32 u32Lgain_ratio;                                 //gain ratio for luma
    HI_U32 u32Lmixing_ratio;                                 //mixing weight for luma (u,1,7）
    HI_S32 s32Lhpass_coef0;                                 //HPL coefficient 0 for luma
    HI_S32 s32Lhpass_coef1;                                 //HPL coefficient 1 for luma
    HI_S32 s32Lhpass_coef2;                                 //HPL coefficient 2 for luma
    HI_S32 s32Lhpass_coef3;                                 //HPL coefficient 3 for luma
    HI_S32 s32Lhpass_coef4;                                 //HPL coefficient 4 for luma
    HI_U32 u32Lcoring_thd;                                 //coring threshold for luma
    HI_U32 u32Lunder_swing;                                 //minimum swing value for luma
    HI_U32 u32Lover_swing;                                 //maximum swing value for luma
    HI_U32 u32Lhfreq_thd0;                                 //HF threshold 0 of HF gain curve for luma
    HI_U32 u32Lhfreq_thd1;                                 //HF threshold 1 of HF gain curve for luma
    HI_U32 u32Lgain_coef0;                                 //gain ratio 0 of HF gain curve for luma
    HI_U32 u32Lgain_coef1;                                 //gain ratio 1 of HF gain curve for luma
    HI_U32 u32Lgain_coef2;                                 //gain ratio 2 of HF gain curve for luma

} PQ_SHARP_LUMA_S;

typedef struct hi_PQ_SHARP_CHROMA_S
{
    HI_U32 u32Cti_en;                                 //chroma sharpen enable
    HI_U32 u32Cgain_ratio;                                 //gain ratio for chroma
    HI_U32 u32Cmixing_ratio;                                 //mixing weight for chroma (u,1,7）
    HI_S32 s32Chpass_coef0;                                 //HPL coefficient 0 for chroma
    HI_S32 s32Chpass_coef1;                                 //HPL coefficient 1 for chroma
    HI_S32 s32Chpass_coef2;                                 //HPL coefficient 2 for chroma
    HI_U32 u32Ccoring_thd;                                 //coring threshold for chroma
    HI_U32 u32Cunder_swing;                                 //minimum swing value for chroma
    HI_U32 u32Cover_swing;                                 //maximum swing value for chroma

} PQ_SHARP_CHROMA_S;

typedef struct hi_PQ_SHARP_DATA_S
{
    PQ_SHARP_LUMA_S  stPqSharpLuma;
    PQ_SHARP_CHROMA_S stPqSharpChroma;

} PQ_SHARP_DATA_S;
typedef struct hi_PQ_SHARP_COEF_S
{
//    PQ_SHARP_DATA_S  stSharpVhdData;
//    PQ_SHARP_DATA_S  stSharpVsdData;
//    PQ_SHARP_DATA_S  stSharpStrData;

    PQ_SHARP_DATA_S  stSharpHdData;
    PQ_SHARP_DATA_S  stSharpSdData;
    PQ_SHARP_DATA_S  stSharpGp0Data;
    PQ_SHARP_DATA_S  stSharpGp1Data;
} PQ_SHARP_COEF_S;

///////////////////////////////////////////define csc//////////////////////////////////////////////////////////////

typedef struct hi_PQ_CSC_CUSTOM_DATA_S
{
    HI_S32 as32[CSC_CUSTOM_ROW][CSC_CUSTOM_COL];
} PQ_CSC_CUSTOM_DATA_S;
typedef struct hi_PQ_CSC_DATA_S
{
    HI_U32 u32Csc_en;                                       //CSC enable
    HI_U32 u32Csc_type;                                     //CSC type
    HI_U32 u32Csc_coef_list;                                //CSC coefficient list
    PQ_CSC_CUSTOM_DATA_S stCscCustomData;                   //CSC coefficient matrix
    HI_U32 u32Bright;                                       //bright adjust value
    HI_U32 u32Contrast;                                     //contrast adjust value
    HI_U32 u32Hue;                                          //hue adjust value
    HI_U32 u32Saturation;                                   //saturation adjust value
    HI_U32 u32Kr;                                           //kr adjust value
    HI_U32 u32Kg;                                           //kg adjust value
    HI_U32 u32Kb;                                           //kb adjust value
} PQ_CSC_DATA_S;
typedef struct hi_PQ_CSC_COEF_S
{
    PQ_CSC_DATA_S  stPqCscVdpV0Data;
    PQ_CSC_DATA_S  stPqCscVdpV1Data;
    PQ_CSC_DATA_S  stPqCscVdpV3Data;
    PQ_CSC_DATA_S  stPqCscVdpV4Data;
    PQ_CSC_DATA_S  stPqCscVdpG0Data;
    PQ_CSC_DATA_S  stPqCscVdpG1Data;
    PQ_CSC_DATA_S  stPqCscVdpWbcDh0Data;
    PQ_CSC_DATA_S  stPqCscVdpInterDh0Data;
    PQ_CSC_DATA_S  stPqCscVdpInterDh1Data;
} PQ_CSC_COEF_S;
///////////////////////////////////////////define acc//////////////////////////////////////////////////////////////
typedef struct hi_PQ_ACC_CRV_S
{
    HI_U16 aU16AccCrv[ACC_CRV_NUM][ACC_CRV_RESOLUTION];   /**<  */
} PQ_ACC_CRV_S;
typedef struct hi_PQ_ACC_MODE_S
{
    PQ_ACC_CRV_S stPqAccModeGentle;
    PQ_ACC_CRV_S stPqAccModeMiddle;
    PQ_ACC_CRV_S stPqAccModeStrong;
} PQ_ACC_MODE_S;
typedef struct hi_PQ_ACC_CTRL_S
{
    HI_U32 u32Acc_en;
    HI_U32 u32Acc_mode;
    HI_U32 u32Acc_thd_low;
    HI_U32 u32Acc_thd_med_low;
    HI_U32 u32Acc_thd_med_high;
    HI_U32 u32Acc_thd_high;
    HI_U32 u32Acc_multiple;
} PQ_ACC_CTRL_S;
typedef struct hi_PQ_ACC_COEF_S
{
    PQ_ACC_CTRL_S stPqAccCtrl;
    PQ_ACC_MODE_S stPqAccModeCrv;

} PQ_ACC_COEF_S;
/////////////////////////////////////////define acm////////////////////////////////////////////////////////////////
typedef struct hi_PQ_ACM_LUT_S
{
    HI_S16 as16Y[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
    HI_S16 as16H[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
    HI_S16 as16S[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
} PQ_ACM_LUT_S;
typedef struct hi_PQ_ACM_MODE_S
{
    PQ_ACM_LUT_S stPqAcmModeBlue;
    PQ_ACM_LUT_S stPqAcmModeGreen;
    PQ_ACM_LUT_S stPqAcmModeBG;
    PQ_ACM_LUT_S stPqAcmModeSkin;
    PQ_ACM_LUT_S stPqAcmModeVivid;
} PQ_ACM_MODE_S;
typedef struct hi_PQ_ACM_CTRL_S
{
    HI_U32 u32Acm_en;
    HI_U32 u32Acm_mode;
    HI_U32 u32Acm_dbg_en;
    HI_U32 u32Acm_stretch;
    HI_U32 u32Acm_cliprange;
    HI_U32 u32Acm_cliporwrap;
    HI_U32 u32Acm_cbcrthr;
    HI_U32 u32Acm_gain_luma;
    HI_U32 u32Acm_gain_hue;
    HI_U32 u32Acm_gain_sat;
} PQ_ACM_CTRL_S;
typedef struct hi_PQ_ACM_COEF_S
{

    PQ_ACM_CTRL_S stPqAcmCtrl;
    PQ_ACM_MODE_S stPqAcmModeLut;
} PQ_ACM_COEF_S;

/////////////////////////////////////////define gamma////////////////////////////////////////////////////////////////

typedef struct hi_PQ_GAMMA_RGB_S
{
    HI_U16 au16R[GAMMA_NUM + 1];   /**<  R Y*/
    HI_U16 au16G[GAMMA_NUM + 1];   /**<  G U*/
    HI_U16 au16B[GAMMA_NUM + 1];   /**<  B V*/
} PQ_GAMMA_RGB_S;
typedef struct hi_PQ_GAMMA_RGB_MODE_S
{
    PQ_GAMMA_RGB_S stPqGammaRgbMode1;
    PQ_GAMMA_RGB_S stPqGammaRgbMode2;
    PQ_GAMMA_RGB_S stPqGammaRgbMode3;
    PQ_GAMMA_RGB_S stPqGammaRgbMode4;
} PQ_GAMMA_RGB_MODE_S;

typedef struct hi_PQ_GAMMA_CTRL_S
{
    HI_U32 u32GammaEnable;
    HI_U32 u32GammaMode;
} PQ_GAMMA_CTRL_S;
typedef struct hi_PQ_GAMMA_COEF_S
{
    PQ_GAMMA_CTRL_S        stPqGammaDisp0Ctrl;
    PQ_GAMMA_CTRL_S        stPqGammaDisp1Ctrl;
    PQ_GAMMA_RGB_MODE_S stPqGammaRgbMode;
    PQ_GAMMA_RGB_MODE_S stPqGammaYuvMode;
} PQ_GAMMA_COEF_S;

////////////////////////////////////////define dither////////////////////////////////////////////////////////////////

typedef struct hi_PQ_DITHER_COEF_S
{
    HI_U32 u32data1;
    HI_U32 u32data2;

} PQ_DITHER_COEF_S;

/////////////////////////////////////////////////define ColorTemperCtrl///////////////////////////////////////////////////////////////
typedef struct hi_PQ_TEMP_MODE_S
{
    HI_U32       u32Kr;
    HI_U32       u32Kg;
    HI_U32       u32Kb;
} PQ_TEMP_MODE_S;

typedef struct hi_PQ_COLOR_TEMP_COEF_S
{
    PQ_TEMP_MODE_S stPqTempModeCold;
    PQ_TEMP_MODE_S stPqTempModeMiddle;
    PQ_TEMP_MODE_S stPqTempModeWarm;
} PQ_COLOR_TEMP_COEF_S;

typedef struct hi_PQ_RESTORE_DEFAULTS_S
{
    HI_BOOL   bDefaultEnable;    
} PQ_RESTORE_DEFAULTS_S;

typedef struct hi_PQ_OPTION_CHOICE_S
{
    HI_U32   u32ChoiceMode;   /**< 0: Working;  1: Default*/ 
} PQ_OPTION_CHOICE_S;

typedef struct hi_PQ_GRAFX_DEFLICKER_S
{
    HI_U32 u32HdCtrlEn;                               
    HI_U32 u32SdCtrlEn;  
} PQ_GFX_DEFLICKER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif/*__cplusplus*/

#endif/*__DRV_PQ_ALG_H__*/




