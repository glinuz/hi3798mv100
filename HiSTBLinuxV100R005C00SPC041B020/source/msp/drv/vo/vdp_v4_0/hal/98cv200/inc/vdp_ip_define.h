//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_define.h
// Author      : s173141
// Data        : 2012/10/17
// Version     : v1.0
// Abstract    : header of vdp define
//                                                                         
// Modification history                                                    
//----------------------------------------------------------------------  
// Version       Data(yyyy/mm/dd)      name                                  
// Description                                                              
//                                                                          
//
//
//
//
//**********************************************************************

#ifndef _VDP_IP_DEFINE_H_
#define _VDP_IP_DEFINE_H_

#include "hi_type.h"


#define  ZME_HPREC        (1<<20)
#define  ZME_VPREC        (1<<12)

#define  GZME_HPREC       (1<<20)
#define  GZME_VPREC       (1<<12)

//acm lut size : 5x7x29 or 9x13x29
#define ACM_INDEX1     9
#define ACM_INDEX2     13 


//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
typedef enum tagVDP_DCI_POS_E
{
    VDP_DCI_POS_VID  = 0,
    VDP_DCI_POS_VP   = 1,
    
    VDP_DCI_POS_BUTT

} VDP_DCI_POS_E;

//add VID_NXG LayerID 
typedef enum
{
    VDP_LAYER_NXG_VID0      = 0x0,
    VDP_LAYER_NXG_DHD0      = 0x0,
    VDP_LAYER_NXG_VID1      = 0x1,
    VDP_LAYER_NXG_DHD1      = 0x1,
    VDP_LAYER_NXG_VP0       = 0x2,
    VDP_LAYER_NXG_BUTT
} VDP_LAYER_NXG_E;
#if 0
typedef struct tagVDP_NXG_CFG_S
{
	HI_U32 dwm_en;	
	HI_U32 debug_en;	
	HI_U32 frame_rate;	
	HI_U32 keyin;	
	HI_U32 payload_0;	
	HI_U32 payload_1;	
	HI_U32 * setting;	 
	HI_U32 video_yfpos;	
	HI_U32 video_xfpos;	
	HI_U32 video_ylpos;	
	HI_U32 video_xlpos;	
} VDP_NXG_CFG_S;
#endif

typedef struct tagVDP_DISP_CLIP_S
{
    HI_U32   bClipEn;
    HI_U32   u32ClipLow_y;  
    HI_U32   u32ClipLow_cb;  
    HI_U32   u32ClipLow_cr;  
    
    HI_U32   u32ClipHigh_y;  
    HI_U32   u32ClipHigh_cb;  
    HI_U32   u32ClipHigh_cr;  

} VDP_DISP_CLIP_S;

typedef enum 
{
    VDP_ZME_MODE_HOR = 0,
    VDP_ZME_MODE_VER,

    VDP_ZME_MODE_HORL,  
    VDP_ZME_MODE_HORC,  
    VDP_ZME_MODE_VERL,
    VDP_ZME_MODE_VERC,

    VDP_ZME_MODE_ALPHA,
    VDP_ZME_MODE_ALPHAV,
    VDP_ZME_MODE_VERT,
	VDP_ZME_MODE_VERB,
    VDP_ZME_MODE_VERBL,
	VDP_ZME_MODE_VERBC,
    VDP_ZME_MODE_ALL,
    VDP_ZME_MODE_NONL,
    VDP_ZME_MODE_BUTT
      
}VDP_ZME_MODE_E;

typedef enum 
{
    VDP_TI_MODE_LUM = 0,  
    VDP_TI_MODE_CHM,  

    VDP_TI_MODE_ALL,
    VDP_TI_MODE_NON,
    VDP_TI_MODE_BUTT
      
}VDP_TI_MODE_E;

typedef enum tagVDP_ZME_ORDER_E
{
    VDP_ZME_ORDER_HV = 0x0,
    VDP_ZME_ORDER_VH = 0x1,

    VDP_ZME_ORDER_BUTT
} VDP_ZME_ORDER_E;

typedef enum tagVDP_DITHER_E
{
    VDP_DITHER_DROP_10   = 0,
    VDP_DITHER_TMP_10    = 1,
    VDP_DITHER_SPA_10    = 2,
    VDP_DITHER_TMP_SPA_8 = 3,
    VDP_DITHER_ROUND_10  = 4,
    VDP_DITHER_ROUND_8   = 5,
    VDP_DITHER_DISEN     = 6,
    VDP_DITHER_BUTT
} VDP_DITHER_E;

typedef struct
{
    HI_U32 dither_coef0;
    HI_U32 dither_coef1;
    HI_U32 dither_coef2;
    HI_U32 dither_coef3;

    HI_U32 dither_coef4;
    HI_U32 dither_coef5;
    HI_U32 dither_coef6;
    HI_U32 dither_coef7;
} VDP_DITHER_COEF_S;

typedef enum tagVDP_IFIRMODE_E
{
    VDP_IFIRMODE_DISEN = 0,
    VDP_IFIRMODE_COPY,
    VDP_IFIRMODE_DOUBLE,
    VDP_IFIRMODE_6TAPFIR,

    VDP_IFIRMODE_BUTT
}VDP_IFIRMODE_E;

typedef struct
{
    HI_U32 acc_multi    ;
    HI_U32 thd_med_high ;
    HI_U32 thd_med_low  ;
    HI_U32 thd_high     ;
    HI_U32 thd_low      ;
} ACCTHD_S;

typedef enum tagVDP_CSC_MODE_E
{
    /*
    VDP_CSC_YUV2RGB_601 = 0,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_YUV2YUV,
    VDP_CSC_YUV2YUV_MAX,
    VDP_CSC_YUV2YUV_MIN,
    VDP_CSC_YUV2YUV_RAND,
    */

	/*
    VDP_CSC_YUV2YUV     = 1,
    VDP_CSC_YUV2RGB_601    ,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
	*/


    VDP_CSC_YUV2RGB_601 = 0      , /* YCbCr_601 L  -> RGB */
    VDP_CSC_YUV2RGB_709          , /* YCbCr_709 L  -> RGB */
    VDP_CSC_RGB2YUV_601          , /* RGB          -> YCbCr_601 L */
    VDP_CSC_RGB2YUV_709          , /* RGB          -> YCbCr_709 L */
    VDP_CSC_YUV2YUV_709_601      , /* YCbCr_709 L  -> YCbCr_601 L */
    VDP_CSC_YUV2YUV_601_709      , /* YCbCr_601 L  -> YCbCr_709 L */
    VDP_CSC_YUV2YUV              , /* YCbCr L      -> YCbCr L */

    VDP_CSC_RGB2RGB_709_2020     , /* RGB_709      -> RGB_2020 */
    VDP_CSC_RGB2YUV_2020_2020_L  , /* RGB_2020     -> YCbCr_2020 L */
    VDP_CSC_YUV2RGB_2020_2020_L  , /* YCbCr_2020 L -> RGB_2020 */
    VDP_CSC_RGB2RGB_2020_709     , /* RGB_2020     -> RGB_709 */
    VDP_CSC_RGB2RGB_601_2020     , /* RGB_601      -> RGB_2020 */
    VDP_CSC_RGB2RGB_2020_601     , /* RGB_2020     -> RGB_601 */

    VDP_CSC_YUV2YUV_2020_601_L_L , /* YCbCr_2020 L -> YCbCr_601 L */
    VDP_CSC_RGB2YUV_2020_601_L   , /* RGB_2020     -> YCbCr_601 L */

    VDP_CSC_YUV2RGB_601_FULL     , /* YCbCr_601 F  -> RGB */
    VDP_CSC_YUV2RGB_709_FULL     , /* YCbCr_709 F  -> RGB */
    VDP_CSC_RGB2YUV_601_FULL     , /* RGB          -> YCbCr_601 F */
    VDP_CSC_RGB2YUV_709_FULL     , /* RGB          -> YCbCr_709 F */
    VDP_CSC_RGB2RGB              , /* RGB          -> RGB */


	//for EDA test
    VDP_CSC_YUV2YUV_MAX    ,
    VDP_CSC_YUV2YUV_MIN    ,
    VDP_CSC_YUV2YUV_RAND   ,
   
    VDP_CSC_BUTT
} VDP_CSC_MODE_E;

typedef struct 
{
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;

} VDP_CSC_COEF_S;

typedef struct
{
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_DC_COEF_S;

typedef struct 
{
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;

    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_CFG_S;

#if VDP_WATERMARK
typedef struct tagVDPWATERMARK_SOFT_S
{
}VDPWATERMARK_SOFT_S
#endif

typedef struct 
{
    HI_U32 hue_adj_6;
    HI_U32 hue_adj_5;
    HI_U32 hue_adj_4;
    HI_U32 hue_adj_3;
    HI_U32 hue_adj_2;
    HI_U32 hue_adj_1;
    HI_U32 hue_adj_0;

}VDP_ACM_HUE_ADJ_S;

typedef struct 
{
    HI_U32 sat_adj_6;
    HI_U32 sat_adj_5;
    HI_U32 sat_adj_4;
    HI_U32 sat_adj_3;
    HI_U32 sat_adj_2;
    HI_U32 sat_adj_1;
    HI_U32 sat_adj_0;

}VDP_ACM_SAT_ADJ_S;

typedef enum tagVDP_DCI_SHIFT_E
{
    SHIFT_6BIT     = 0,//right shift 6bit 
    SHIFT_7BIT     = 1,//right shift 7bit 
    SHIFT_8BIT     = 2,//right shift 8bit 
    SHIFT_9BIT     = 3,//right shift 9bit 
    SHIFT_BUTT
}VDP_DCI_SHIFT_E;

typedef enum tagVDP_DCI_RANGE_E
{
    IN_RANGE,
    OUT_RANGE,
    RANGE_BUTT
}VDP_DCI_RANGE_E;

typedef struct tagVDP_DCI_POS_S
{
    HI_U32 hstart;
    HI_U32 hend;
    HI_U32 vstart;
    HI_U32 vend;
	HI_U32 outhstart;	  //12 bit, DCI作用区域
	HI_U32 outhend;		  //12 bit
	HI_U32 outvstart;	  //12 bit
	HI_U32 outvend;		  //12 bit

}VDP_DCI_POS_S;

typedef struct tagVDP_DCI_PARA_S
{
	HI_U32  u32_dci_sel;

	HI_S32  s32_dci_en;
    HI_S32  s32_dci_demoen;
    HI_S32  s32_dci_demomode;
    HI_S32  s32_dci_demopos;

	HI_S32 s32HIST_WGT_EN;

	HI_S32  s32BS_limit_THR;
	HI_S32  s32BS_limit_ABlend;

    HI_S32  s32EnSceneChange;
    HI_S32  s32EnManAdj[3];
	HI_S32  s32CbCrComp_en;
	HI_S32  s32CbCrSta_en;
	HI_S32  s32InputFRange;   //  input data range: 1/0 = full range/limited range
	HI_S32  s32OutputFRange;  // output data range: 1/0 = full range/limited range 
	HI_S32  s32HistRShftBit;
    HI_S32  s32HistLPFEn;
    HI_S32  s32EnSWcfg;
    
    HI_S32  s32HStart;
	HI_S32  s32HEnd;
	HI_S32  s32VStart;
	HI_S32  s32VEnd;
		
	HI_S32  s32OutHStart;	  //12 bit, DCI作用区域
	HI_S32  s32OutHEnd;		  //12 bit
	HI_S32  s32OutVStart;	  //12 bit
	HI_S32  s32OutVEnd;		  //12 bit

    HI_S32  s32CbCrSta_Y,s32CbCrSta_Cb, s32CbCrSta_Cr,  s32CbCrSta_Offset;

    HI_S32  s32HistCoring[3];

    HI_S32  s32MetricAlpha[3];
    HI_S32  s32HistABlend;
    HI_S32  s32OrgABlend;

    HI_S32  s32ManAdjWgt[3];

    HI_S32 s32AdjGain_ClipL[3];
    HI_S32 s32AdjGain_ClipH[3];

    HI_S32 s32GlbGain0,s32GlbGain1,s32GlbGain2;

    HI_S32  s32CbCrGainPos_Ythrsh[7];
    HI_S32  s32CbCrGainPos[9];

    HI_S32  s32CbCrGainPos_Slp[8];

    HI_S32  s32CbCrGainNeg_Ythrsh[7];

    HI_S32  s32CbCrGainNeg[9];

    HI_S32  s32CbCrGainNeg_Slp[8];

    HI_S32  ps32AdjLUT_SW[33];
//	HI_S32  s32Histogram[2][BINNUM];
//	HI_S32  s32HistRShft[2][BINNUM];
	
 //	HI_S32  s32CurFrmNum;
//  HI_S32  s32Format;
//
//	HI_S32 *ps32BinWeight[METRICNUM];
//	HI_U8   u8WgtBitNum;

//	HI_S32  ps32AdjLUT[33], ps32AdjLUT_0[33], ps32AdjLUT_1[33], ps32AdjLUT_2[33];
//
//	HI_S32  s32divLUT[64];
    HI_S32  s32YLinear;
	HI_U8   u8EnManAlgBlend;	//1 bit
	HI_U8   u8ManAlgBlend;		//7 bit
	HI_U8   u8SplitMode; 		//2 bit, 0: manully set split point; 1: use default split type; 2: auto change between peak, mean and valley according to weights
	HI_U8   u8SplitType;		//2 bit, 0: Peak; 1: Mean; 2: Valley
	HI_U8   u8SplitPoint;		//8 bit
	HI_U16	u16ClipLimitL;		//10 bit
	HI_U16	u16ClipLimitR;		//10 bit
	HI_U8   u8HistMapBlend;		//5 bit
	HI_U8   u8FlatMode;			//2 bit, 0: dci with dynamic split point (same as other videos); 1: flat-> dci with fixed split point; 2: flat -> don't apply dci
	HI_U32  u32FlatThrsh;		// 1080p:200


	HI_U8   u8EnBlackStretch;	//1 bit
	HI_S32  s32BendingLevel;	//320 (10bit proc), 80 (8bit proc)
	HI_S32  s32MaxBSLevel;		//160 (10bit proc), 40 (8bit proc)
	HI_U32	u32MinBlackThrsh;	//8 bit
	HI_U8   u8MinBlackBlend;    //5 bit
	HI_S32  s32BSLOffset;		//6 bit (signed)
	HI_U8   u8BSLimitThrsh;		//7 bit
	HI_U8	u8BlackWgtThrsh;	//6 bit
	HI_S32  s32BSDelta[320]; 	//10 bit x 320,RW
	HI_S32  s32BSDeltaInv;    //102 for (1<<14)/160 || (1<<16)/640

}VDP_DCI_PARA_S;

typedef enum tagVDP_VP_DCI_ADJ_NUM_E
{
    VDP_VP_DCI_ADJ0    = 0x0,
    VDP_VP_DCI_ADJ1    = 0x1,
    VDP_VP_DCI_ADJ2    = 0x2,

    VDP_VP_DCI_ADJ_BUTT        
    
}VDP_VP_DCI_ADJ_NUM_E;

typedef enum tagVDP_VP_DCI_CTRL_MODE_E
{
    VDP_VP_DCI_CTRL_AUTO    = 0x0,
    VDP_VP_DCI_CTRL_MAN     = 0x1,

    VDP_VP_DCI_CTRL_MODE_BUTT        
    
}VDP_VP_DCI_CTRL_MODE_E;

typedef struct tagVDP_DHD_WCG_COEF_S
{
	HI_U32 u32WcgEn;
    HI_U32 csc2_scale_sel;
    HI_U32 csc2_r_r_coef;
    //HI_S32 csc2_r_g_coef;
    //HI_S32 csc2_r_b_coef;
    //HI_S32 csc2_g_r_coef;
    HI_U32 csc2_r_g_coef;
    HI_U32 csc2_r_b_coef;
    HI_U32 csc2_g_r_coef;
    HI_U32 csc2_g_g_coef;
    //HI_S32 csc2_g_b_coef;
    //HI_S32 csc2_b_r_coef;
    //HI_S32 csc2_b_g_coef;
    HI_U32 csc2_g_b_coef;
    HI_U32 csc2_b_r_coef;
    HI_U32 csc2_b_g_coef;
    HI_U32 csc2_b_b_coef;

    HI_U32 gma_lin_neg_scl ; 
    HI_U32 gma_lin_init_slp; 
    HI_U32 gma_lin_thre_0  ; 
    HI_U32 gma_lin_thre_1  ; 
    HI_U32 gma_lin_thre_2  ; 

    //HI_U32 gma_lin_coef_a_0;
    //HI_U32 gma_lin_coef_a_1;
    //HI_U32 gma_lin_coef_a_2;
    //HI_S32 gma_lin_coef_b_0;
    //HI_S32 gma_lin_coef_b_1;
    //HI_S32 gma_lin_coef_b_2;
    HI_S32 gma_lin_coef_a_0;
    HI_S32 gma_lin_coef_a_1;
    HI_S32 gma_lin_coef_a_2;
    HI_U32 gma_lin_coef_b_0;
    HI_U32 gma_lin_coef_b_1;
    HI_U32 gma_lin_coef_b_2;
    HI_U32 gma_lin_coef_c_0;
    HI_U32 gma_lin_coef_c_1;
    HI_U32 gma_lin_coef_c_2;

    HI_U32 lin_gma_thre_0  ;
    HI_U32 lin_gma_thre_1  ;
    HI_U32 lin_gma_thre_2  ;

    HI_U32 lin_gma_coef_a_0;
    HI_U32 lin_gma_coef_a_1;
    HI_U32 lin_gma_coef_a_2;
    HI_U32 lin_gma_coef_b_0;
    HI_U32 lin_gma_coef_b_1;
    HI_U32 lin_gma_coef_b_2;
    HI_S32 lin_gma_coef_c_0;
    HI_S32 lin_gma_coef_c_1;
    HI_S32 lin_gma_coef_c_2;
}VDP_DHD_WCG_COEF_S;

typedef struct tagVDP_WCG_COEF_S
{
	HI_U32 u32WcgEn;
    HI_U32 csc2_scale_sel;
    HI_U32 csc2_r_r_coef;
    HI_U32 csc2_r_g_coef;
    HI_U32 csc2_r_b_coef;
    HI_U32 csc2_g_r_coef;
    HI_U32 csc2_g_g_coef;
    HI_U32 csc2_g_b_coef;
    HI_U32 csc2_b_r_coef;
    HI_U32 csc2_b_g_coef;
    HI_U32 csc2_b_b_coef;

    HI_U32 gma_lin_neg_scl ; 
    HI_U32 gma_lin_init_slp; 
    HI_U32 gma_lin_thre_0  ; 
    HI_U32 gma_lin_thre_1  ; 
    HI_U32 gma_lin_thre_2  ; 

    HI_S32 gma_lin_coef_a_0;
    HI_S32 gma_lin_coef_a_1;
    HI_S32 gma_lin_coef_a_2;
    HI_U32 gma_lin_coef_b_0;
    HI_U32 gma_lin_coef_b_1;
    HI_U32 gma_lin_coef_b_2;
    HI_U32 gma_lin_coef_c_0;
    HI_U32 gma_lin_coef_c_1;
    HI_U32 gma_lin_coef_c_2;

    HI_U32 lin_gma_thre_0  ;
    HI_U32 lin_gma_thre_1  ;
    HI_U32 lin_gma_thre_2  ;

    HI_U32 lin_gma_coef_a_0;
    HI_U32 lin_gma_coef_a_1;
    HI_U32 lin_gma_coef_a_2;
    HI_U32 lin_gma_coef_b_0;
    HI_U32 lin_gma_coef_b_1;
    HI_U32 lin_gma_coef_b_2;
    HI_S32 lin_gma_coef_c_0;
    HI_S32 lin_gma_coef_c_1;
    HI_S32 lin_gma_coef_c_2;

}VDP_WCG_COEF_S;

typedef enum tagVDP_SR_CONN_E
{
    VDP_SR_CONN_NULL = 0x0,
    VDP_SR_CONN_DHD0 ,
    VDP_SR_CONN_VP0 ,
    VDP_SR_CONN_GP0 ,
    VDP_SR_CONN_V0  ,
    VDP_SR_CONN_BUTT
} VDP_SR_CONN_E;

//---------------------------------
//Sharpen BEGIN
//---------------------------------
typedef struct tagVDP_SHARP_HIGH_FREQ_S
{
    HI_S16   s16GainPos;/*(u,9,7):Max 3.0 */
    HI_S16   s16GainNeg;
    //-------------------------------------------- 
    //change
    HI_U8    u8Coringthr;	
    HI_U8    u8Coringzero;	
    HI_U8    u8Coringratio;	
    //-------------------------------------------- 
    HI_U8    u8OverThrsh;
    HI_U8    u8UnderThrsh;
    HI_U8    u8MixRatio;
    HI_U8    u8WinSize;
    /***limit***/    
    HI_U16    u16LimitRatio;  
    HI_U16    u16LimitPos0 ;   
    HI_U16    u16LimitPos1 ;   
    HI_S16    s16LimitNeg0 ;   
    HI_S16    s16LimitNeg1 ;   
    HI_U16    u16BoundPos  ;    
    HI_S16    s16BoundNeg  ;

    HI_BOOL  bEnable;
} VDP_SHARP_HIGH_FREQ_S;

typedef struct tagVDP_SHARP_GROUP_S
{
    VDP_SHARP_HIGH_FREQ_S stSharpU1;//unit 1

} VDP_SHARP_GROUP_S;

typedef enum tagVDP_SHARP_DIR_E
{
    VDP_SHARP_DIR_H   = 0x0,//horizontal
    VDP_SHARP_DIR_V   = 0x1,//vertical
    VDP_SHARP_DIR_D   = 0x2,//diagonal
    VDP_SHARP_DIR_BUTT          
} VDP_SHARP_DIR_E;

typedef struct tagVDP_SHARP_CFG_S
{
    HI_U32 bSharpenEn;
} VDP_SHARP_CFG_S;

//---------------------------------
//sharpen END
//---------------------------------

//---------------------------------
//SR BEGIN
//---------------------------------
#if 0
typedef enum tagVDP_SR_MUX_E
{
    VDP_SR_AND_ZME_EN = 0x0,
    VDP_SR_AND_ZME_EN_L ,
    VDP_SR_AND_ZME_EN_R ,
    VDP_SR_ONLY_ZME_EN ,
    VDP_SR_ZME_EN_BUTT
} VDP_SR_MUX_E;

typedef enum tagVDP_SR_DEMO_E
{
    VDP_SR_DEMO_EN = 0x0,
    VDP_SR_DEMO_EN_L ,
    VDP_SR_DEMO_EN_R ,
    VDP_SR_DEMO_EN_BUTT
} VDP_SR_DEMO_E;
#endif

typedef enum tagVDP_WBC_LOCATE_E
{
    VDP_WBC_LOCATE_CBM1 = 0x0,
    VDP_WBC_LOCATE_CBM2 = 0x1,
    VDP_WBC_LOCATE_E_BUTT
} VDP_WBC_LOCATE_E;

#if 0
typedef enum tagVDP_SR_SERVE_MODE_E
{
    VDP_SR_2IN_2OUT = 0x0,
    VDP_SR_1IN_2OUT ,
    VDP_SR_1IN_1OUT ,
    VDP_SR_SERVE_MODE_BUTT
} VDP_SR_SERVE_MODE_E;
#endif

typedef enum tagVDP_SR_SHARP_MODE_E
{
    VDP_SR_SHARP_MODE_DETAIL = 0x0,
    VDP_SR_SHARP_MODE_TRAD ,
    VDP_SR_SHARP_MODE_BUTT
} VDP_SR_SHARP_MODE_E;

typedef enum tagVDP_SR_E
{
    VDP_SR_UENABLE = 0x0,
    VDP_SR_ENABLE ,
    VDP_SR_BUTT
} VDP_SR_E;

typedef enum tagVDP_SR_SHARP_E
{
    VDP_SR_SHARP_UENABLE = 0x0,
    VDP_SR_SHARP_R_ENABLE ,
    VDP_SR_SHARP_L_ENABLE ,
    VDP_SR_SHARP_ENABLE ,
    VDP_SR_SHARP_BUTT
} VDP_SR_SHARP_E;

typedef enum tagVDP_SR_SMOOTH_E
{
    VDP_SR_SMOOTH_UENABLE = 0x0,
    VDP_SR_SMOOTH_ENABLE ,
    VDP_SR_SMOOTH_BUTT
} VDP_SR_SMOOTH_E;

typedef enum tagVDP_SR_PLUS_E
{
    VDP_SR_PLUS_UENABLE = 0x0,
    VDP_SR_PLUS_ENABLE ,
    VDP_SR_PLUS_BUTT
} VDP_SR_PLUS_E;

typedef enum tagVDP_SR_MODE_E
{
    VDP_SR_MODE_ALL = 0x0  , 
    VDP_SR_MODE_ONLY_ZME   , 
    VDP_SR_MODE_DEMO_LEFT  , 
    VDP_SR_MODE_DEMO_RIGHT , 
    VDP_SR_MODE_BUTT
} VDP_SR_MODE_E;

typedef struct tagVDP_SR_PARA_INFO
{
    VDP_SR_SHARP_MODE_E hsh_nodir   ; 
    VDP_PROC_FMT_E      ifmt        ; 
    HI_U32              sr_en       ; 
    VDP_SR_SHARP_E      sharp_en    ; 
    HI_U32              smooth_en   ; 
    HI_U32              ringdt_en   ;
    HI_U32              skindt_en   ;
    HI_U32              plus_en     ; 
    HI_U32              mid_en      ; 
    HI_U32              DispMode    ; 
    VDP_SR_MODE_E       sr_mode     ;
}VDP_SR_PARA_INFO;

typedef struct tagVDP_SR_IORESO
{
    HI_U32 iw  ;
    HI_U32 ih  ;
    HI_U32 ow  ;
    HI_U32 oh  ;
}VDP_SR_IORESO;

typedef struct tagVDP_SR_PARA_S
{
    VDP_SR_PARA_INFO  SrParaInfo   ;

    HI_U32 hfir_order              ; 
    HI_U32 hratio                  ; 
    HI_U32 vratio                  ; 
    HI_U32 grd_sub_ratio           ; 
    HI_S32 grd_thrl                ; 
    HI_S32 grd_thrh                ; 
    HI_U32 grd_thrh_subl           ; 
    HI_U32 grd_add_ctrl            ; 
    HI_U32 disc_detail_gain        ; 
    HI_U32 sharp_coring            ; 
    HI_U32 sharp_gain              ; 
    HI_U32 over_thrsh              ; 
    HI_U32 under_thrsh             ; 
    HI_U32 plus_ctrl               ; 
    HI_U32 grd_hvnear              ;
    HI_U32 clip_data               ;
    HI_U32 y_exp_val               ;
    HI_U32 u_exp_val               ;
    HI_U32 v_exp_val               ;
    HI_U32 y_thres_l               ;
    HI_U32 y_thres_h               ;
    HI_S32 y_slop                  ;
    HI_U32 u_thres_l               ;
    HI_U32 u_thres_h               ;
    HI_S32 u_slop                  ;
    HI_U32 v_thres_l               ;
    HI_U32 v_thres_h               ;
    HI_S32 v_slop                  ;
    HI_U32 grad_flat_thr           ;
    HI_U32 grad_flat_thr2          ;
    HI_U32 grad_thres_l            ;
    HI_U32 grad_thres_h            ;
    HI_U32 grad_slop               ;
    HI_U32 sharp_thres_l           ;
    HI_U32 sharp_thres_h           ;
    HI_U32 sharp_low_ctrl_l        ;
    HI_U32 sharp_low_ctrl_h        ;
    HI_U32 sharp_low_start         ;
    HI_U32 sharp_low_slop          ;
    HI_U32 sharp_high_ctrl_l       ;
    HI_U32 sharp_high_ctrl_h       ;
    HI_U32 sharp_high_start        ;
    HI_S32 sharp_high_slop         ;
    HI_U32 ring_thres_l            ;
    HI_U32 ring_thres_h            ;
    HI_U32 skin_thres_l            ;
    HI_U32 skin_thres_h            ;
    HI_U32 sharp_high_ctrl         ;
    HI_U32 strengh                 ;
    HI_U32 nolinear_ctrl           ;
    HI_U32 edge_factor_ctrl        ;
    HI_U32 shoot_thrsh             ;
    HI_U32 dir_hv_ctrl             ;
    HI_U32 dir_dg_ctrl             ;
    
    HI_U32 zme_en                  ; 
    HI_U32 hoffset                  ; 
    HI_U32 voffset                  ; 
    HI_U32 fir_en                  ; 
    HI_U32 srzme_hladdr            ; 
    HI_U32 srzme_hcaddr            ; 
    HI_U32 srzme_vladdr            ; 
    HI_U32 srzme_vcaddr            ; 

}VDP_SR_PARA_S;


//---------------------------------
//SR END
//---------------------------------
//---------------------------------
//DIMMING  BEGIN
//---------------------------------

typedef enum{
    SCL1_TAP12 = 12,
    SCL1_TAP8 = 8,
	SCL1_TAP7 = 7,
    SCL1_TAP6 = 6,
    SCL1_TAP4 = 4,
    SCL1_TAP2 = 2
}PIC_SCL_TAP_E1;
typedef enum{
	SCL1_PHASE512 = 9,
	SCL1_PHASE128 = 7,
	SCL1_PHASE64 = 6,
    SCL1_PHASE32 = 5,
    SCL1_PHASE16 = 4,
    SCL1_PHASE8 = 3,
}PIC_SCL_PHASE_E1;

typedef struct tagVDP_DISP_DIMMING_ZMEV
{
    HI_S32  s32WidthIn;/*（horz/vert,lum/chrom）input width*/
    HI_S32  s32HeightIn;/*（horz/vert,lum/chrom）input height*/
    HI_S32  s32WidthOut;/*（horz/vert,lum/chrom）output width*/
    HI_S32  s32HeightOut;/*（horz/vert,lum/chrom）output height*/
    HI_U32  u32StrideIn;/*（horz/vert,lum/chrom）input stride*/
    HI_U32  u32StrideOut;/*（horz/vert,lum/chrom）output stride*/
    HI_S32  *ps32Coeff;/*（horz/vert,lum/chrom）coefficients*/
    PIC_SCL_TAP_E1    enTap;/*（horz/vert,lum/chrom）tap*/
    PIC_SCL_PHASE_E1  enPhaseShft;
    HI_U32  u32Step;/*（horz/vert,lum/chrom) step*/
    HI_DOUBLE  dPhase;/*（horz/vert,lum/chrom）initial phase*/
    HI_BOOL bEnMedian;/*（horz/vert,lum/chrom）Median enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnScale;/*(horz/vert,lum/chrom) scale enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnFilter;

} VDP_DISP_DIMMING_ZMEV;

typedef struct tagVDP_DISP_DIMMING_ZMEH
{
    HI_S32  s32WidthIn;/*（horz/vert,lum/chrom）input width*/
    HI_S32  s32HeightIn;/*（horz/vert,lum/chrom）input height*/
    HI_S32  s32WidthOut;/*（horz/vert,lum/chrom）output width*/
    HI_S32  s32HeightOut;/*（horz/vert,lum/chrom）output height*/
    HI_U32  u32StrideIn;/*（horz/vert,lum/chrom）input stride*/
    HI_U32  u32StrideOut;/*（horz/vert,lum/chrom）output stride*/
    HI_S32  *ps32Coeff;/*（horz/vert,lum/chrom）coefficients*/
    PIC_SCL_TAP_E1    enTap;/*（horz/vert,lum/chrom）tap*/
    PIC_SCL_PHASE_E1  enPhaseShft;
    HI_U32  u32Step;/*（horz/vert,lum/chrom) step*/
    HI_DOUBLE  dPhase;/*（horz/vert,lum/chrom）initial phase*/
    HI_BOOL bEnMedian;/*（horz/vert,lum/chrom）Median enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnScale;/*(horz/vert,lum/chrom) scale enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnFilter;
} VDP_DISP_DIMMING_ZMEH;
typedef struct tagVDP_DISP_DIMMING_THD_S
{
    HI_U32 u32CoreLow;
    HI_U32 u32CoreMid;
    HI_U32 u32CoreHigh;

    HI_U32 u32X0Low;
    HI_U32 u32X1Low;
    HI_U32 u32X2Low;
    HI_U32 u32X0Mid;
    HI_U32 u32X1Mid;
    HI_U32 u32X2Mid;
    HI_U32 u32X0High;
    HI_U32 u32X1High;
    HI_U32 u32X2High;

    HI_U32 u32G0Low;
    HI_U32 u32G1Low;
    HI_U32 u32G2Low;
    HI_U32 u32G3Low;
    HI_U32 u32G0Mid;
    HI_U32 u32G1Mid;
    HI_U32 u32G2Mid;
    HI_U32 u32G3Mid;
    HI_U32 u32G0High;
    HI_U32 u32G1High;
    HI_U32 u32G2High;
    HI_U32 u32G3High;

    HI_U32 u32K0Low;
    HI_U32 u32K1Low;
    HI_U32 u32K2Low;
    HI_U32 u32K3Low;
    HI_U32 u32K0Mid;
    HI_U32 u32K1Mid;
    HI_U32 u32K2Mid;
    HI_U32 u32K3Mid;
    HI_U32 u32K0High;
    HI_U32 u32K1High;
    HI_U32 u32K2High;
    HI_U32 u32K3High;

} VDP_DISP_DIMMING_THD_S;
typedef struct tagVDP_DISP_DIMMING_FIR_COEF_S
{
    HI_U32 u32HFirTap0;
    HI_U32 u32HFirTap1;
    HI_U32 u32HFirTap2;
    HI_U32 u32HFirTap3;
    HI_U32 u32HFirTap4;

    HI_U32 u32VFirTap0;
    HI_U32 u32VFirTap1;
    HI_U32 u32VFirTap2;
    HI_U32 u32VFirTap3;
    HI_U32 u32VFirTap4;

} VDP_DISP_DIMMING_FIR_COEF_S;

#define  FILE_NAME_SIZE 255
typedef struct tagVDP_DISP_DIMMING_CFG_S
{
	char	BLCInputFileBMP[FILE_NAME_SIZE];
	char	BLCOutputFileBMP[FILE_NAME_SIZE];
	HI_S32 s32ColorSpaceIn;
	HI_S32 s32ColorSpaceOut;

	int m_bOutputEN;
	int m_bAutoSettingEN;			//自动配, 1/zonesize,  1/height;
	int m_LEDNUMH;					//horz zone number
	int m_LEDNUMV;					//vert zone number
	int m_BYPASS_LCD;				//如果 = 1, 输出图像值 .
	int m_BYPASS_LED;				//如果=1 输出最大值.
	int m_DISABLE_2D;				//如果=1 输出最大值.
	int m_SEL_UHD;				    //如果=1 输出最大值.
	int m_MODE_INIT_LED;			//0D时初始化与计算的进行blengding
	int m_LEDDRIVE_MAX;
	int m_DIV_HEIGHT;				//1/height
	int m_DIV_WIDTH;				//1/width
	int m_DIV_SEGHEIGHT;			//1/zonesize
	int m_DIV_SEGWIDTH;				//1/zonesize
	int m_LEDSEGH;					//每个zonesize
	int m_LEDSEGV;					//每个zonesize
	int m_COL_STARTLED;				//第一个zone的中心位置.
	int m_ROW_STARTLED;				//第一个zone的中心位置.
	int m_LEDDSTH;					//两个zone的中心点距离.  overlap   zoneSize 允许有overlap
	int m_LEDDSTV;					//v两个zone的中心点距离.
	int m_LPF_MODE;
	int m_GAIN_LED2D;				//4095 与算出来每个zone LED 值得blend              固定
	int m_GAIN_BLDMAX2D;			//算出来每个zone LED值 与每个zone的最大值进行blending  固定
	int m_BITS_B_HIST;
	int m_BITS_W_HIST;
	int m_THR_MD0;
	int m_THR_MD2;
	int m_COR_DSTATLW;
	int m_COR_DSTATMD;
	int m_COR_DSTATHG;
	int m_K_DSTAT0D;				//scene change
	int m_GAIN_LED0D;				//与4095 blending的增益
	int m_GAIN_LED0D_TF;			//时间域滤波
	int m_SC0D_SW;					// scene change传出去给软件。
	int m_LED0D_SW;					//scene change传出去给软件。
	int m_LED0D_CORE_LW;
	int m_LED0D_CORE_MD;
	int m_LED0D_CORE_HG;
	int m_LED0D_X0_LW;
	int m_LED0D_X1_LW;
	int m_LED0D_X2_LW;
	int m_LED0D_K0_LW;
	int m_LED0D_K1_LW;
	int m_LED0D_K2_LW;
	int m_LED0D_K3_LW;
	int m_LED0D_G0_LW;
	int m_LED0D_G1_LW;
	int m_LED0D_G2_LW;
	int m_LED0D_G3_LW;
	int m_LED0D_X0_MD;
	int m_LED0D_X1_MD;
	int m_LED0D_X2_MD;
	int m_LED0D_K0_MD;
	int m_LED0D_K1_MD;
	int m_LED0D_K2_MD;
	int m_LED0D_K3_MD;
	int m_LED0D_G0_MD;
	int m_LED0D_G1_MD;
	int m_LED0D_G2_MD;
	int m_LED0D_G3_MD;
	int m_LED0D_X0_HG;
	int m_LED0D_X1_HG;
	int m_LED0D_X2_HG;
	int m_LED0D_K0_HG;
	int m_LED0D_K1_HG;
	int m_LED0D_K2_HG;
	int m_LED0D_K3_HG;
	int m_LED0D_G0_HG;
	int m_LED0D_G1_HG;
	int m_LED0D_G2_HG;
	int m_LED0D_G3_HG;
	int m_LED2D_CORE_LW;
	int m_LED2D_CORE_MD;
	int m_LED2D_CORE_HG;
	int m_LED2D_X0_LW;
	int m_LED2D_X1_LW;
	int m_LED2D_X2_LW;
	int m_LED2D_K0_LW;
	int m_LED2D_K1_LW;
	int m_LED2D_K2_LW;
	int m_LED2D_K3_LW;
	int m_LED2D_G0_LW;
	int m_LED2D_G1_LW;
	int m_LED2D_G2_LW;
	int m_LED2D_G3_LW;
	int m_LED2D_X0_MD;
	int m_LED2D_X1_MD;
	int m_LED2D_X2_MD;
	int m_LED2D_K0_MD;
	int m_LED2D_K1_MD;
	int m_LED2D_K2_MD;
	int m_LED2D_K3_MD;
	int m_LED2D_G0_MD;
	int m_LED2D_G1_MD;
	int m_LED2D_G2_MD;
	int m_LED2D_G3_MD;
	int m_LED2D_X0_HG;
	int m_LED2D_X1_HG;
	int m_LED2D_X2_HG;
	int m_LED2D_K0_HG;
	int m_LED2D_K1_HG;
	int m_LED2D_K2_HG;
	int m_LED2D_K3_HG;
	int m_LED2D_G0_HG;
	int m_LED2D_G1_HG;
	int m_LED2D_G2_HG;
	int m_LED2D_G3_HG;
	int m_CORE_LED0D;
	int m_K_LED0D;
	int m_GAIN_BLD0D2D;	






	int m_GAIN_LPFH;	
	int m_GAIN_LPFV;
	int m_CORE_LED2D;
	int m_K_LED2D;
	int m_GAIN_TF;


	int m_Gain_Final_2D;
	int m_Offset_Final_2D;
	int m_Clip_btm_Final_2D;
	int m_Clip_up_Final_2D;
	
	int m_Gain_Final_0D;
	int m_Offset_Final_0D;
	int m_Clip_btm_Final_0D;
	int m_Clip_up_Final_0D;

	
	int m_COEF0_LPFH;
	int m_COEF1_LPFH;
	int m_COEF2_LPFH;
	int m_COEF3_LPFH;
	int m_COEF4_LPFH;
	int m_COEF0_LPFV;
	int m_COEF1_LPFV;
	int m_COEF2_LPFV;
	int m_COEF3_LPFV;
	int m_COEF4_LPFV;	
	int m_LCDCOMPGAIN;
	int m_LCDCOMPGAIN_2D;
	int m_LCDG_CORE_LED0D;
	int m_LCDG_TH_LED0D;
	int m_LCDG_K_LED0D;
	int m_LCDG_CORE_LED2D;
	int m_LCDG_K_LED2D;	
	int m_K_LEDPANEL;
	int m_MAX_LEDPANEL;
	int m_K_LEDOUT;
    HI_DOUBLE  dPhaseh;
    HI_BOOL bEnMedianh;
    HI_BOOL bEnScaleh;
    HI_BOOL bEnFilterh;
    HI_DOUBLE  dPhasev;
    HI_BOOL bEnMedianv;
    HI_BOOL bEnScalev;
    HI_BOOL bEnFilterv;

	
}VDP_DISP_DIMMING_CFG_S;
//---------------------------------
//DIMMING END
//---------------------------------


//---------------------------------
//ES BEGIN
//---------------------------------

typedef struct tagVDP_ES_PARA_INFO
{
    HI_U32            es_en                ;
    HI_U32            mk_en                ;
    HI_U32            mk_mode              ;
    HI_U32            blend_mode           ;
    HI_U32            edge_strength_mode   ;
    HI_U32            alter_dir_en         ;
    HI_U32            check2_en            ;
    HI_U32            check1_en            ;
    HI_U32            dir_mode             ;
    HI_U32            interlaced_proc      ;
}VDP_ES_PARA_INFO;

typedef struct tagVDP_ES_PARA_S
{
    VDP_ES_PARA_INFO  EsParaInfo           ;
    HI_U32            check1_deadzone      ;
    HI_U32            check1_autodz_gain   ;
    HI_U32            check1_gain          ;
    HI_U32            check1_dir_diff_gain ;
    HI_U32            check2_deadzone      ;
    HI_U32            check2_autodz_gain   ;
    HI_U32            check2_gain          ;
    HI_U32            check_gain           ;
    HI_U32            ratio_limit_scale    ;
    HI_S32            ver_min_strength     ;
    HI_U32            edge_smooth_gain     ;
}VDP_ES_PARA_S;

//---------------------------------
//ES END
//---------------------------------

//---------------------------------
//for EC of S5V100
//---------------------------------
typedef enum tagVDP_GP_OUTMODE_E
{
    VDP_GP_OUTMODE_NORMAL   = 0x0,// sr 1in_2out or FHD intf: single channel; else : dual channel; 
    VDP_GP_OUTMODE_2IN_1OUT = 0x1,// online/offline GP 4k in 2k*2k out
    VDP_GP_OUTMODE_BUTT
} VDP_GP_OUTMODE_E;

//---------------------------------
//TNR BEGIN
//---------------------------------
typedef struct tagVDP_TNR_PARA_INFO
{
    HI_U32             tnr_en      ;
    HI_U32             tnr_pgbm_en ;
    HI_U32             tnr_y_bypass;
    HI_U32             tnr_c_bypass;
    HI_U32             market_en   ;
    HI_U32             market_mode ;
    HI_U32             market_coordinate ;
    HI_U32             tnr_c_en    ;
}VDP_TNR_PARA_INFO;

//---------------------------------
//TNR END
//---------------------------------

//---------------------------------
//SNR BEGIN
//---------------------------------
typedef struct tagVDP_SNR_PARA_INFO
{
    HI_U32             snr_en           ;
    HI_U32             lumasnren        ;
    HI_U32             chromasnren      ;
    HI_U32             debugmodeen      ;
    HI_U32             debugmode        ;
    HI_U32             demomodeen       ;
    HI_U32             demomodelr       ;
    HI_U32             demomodecoor       ;
    HI_U32             diredgesmoothen  ;
    HI_U32             motionedgeen     ;
}VDP_SNR_PARA_INFO;

//---------------------------------
//SNR END
//---------------------------------

//---------------------------------
//DBM BEGIN
//---------------------------------
typedef struct tagVDP_DBM_PARA_INFO
{
    HI_U32             in_width      ;
    HI_U32             in_height     ;
    HI_U32             ifmt          ;
    HI_U32             dbm_en        ;
    HI_U32             db_en         ;
    HI_U32             dm_en         ;
    HI_U32             ds_en         ;
    HI_U32             dr_en         ;
    HI_U32             dbm_out_mode  ;
    HI_U32             db_lum_ver_en ;
    HI_U32             db_lum_hor_en ;
    HI_U32             db_chr_ver_en ;
    HI_U32             db_chr_hor_en ;
    HI_U32             market_en     ;
    HI_U32             market_mode   ;
    HI_U32             l_h_bs        ;  // just for the stream mode
    HI_U32             l_v_bs        ;  // just for the stream mode
    HI_U32             c_h_bs        ;  // just for the stream mode

    HI_U32             dbm_protocol         ;  
    HI_U32             db_vdh_trans_info_en ;  
    HI_U32             db_det_en            ;  
    HI_U32             db_blk_mode          ;  
    HI_U32             vdh_db_width         ;  
    HI_U32             vdh_db_height        ;  
    HI_U32             db_stt_offset_x      ;  
    HI_U32             db_stt_offset_y      ;  
    HI_U32             db_end_offset_x      ;  
    HI_U32             db_end_offset_y      ;  
}VDP_DBM_PARA_INFO;

//---------------------------------
//DBM END
//---------------------------------


//---------------------------------
//DB DETECT BEGIN
//---------------------------------
typedef struct tagVDP_DB_DETECT_PARA_INFO
{
    HI_U32             in_width         ;
    HI_U32             in_height        ;
    HI_U32             dbm_en           ;
    HI_U32             det_size_en      ;
    HI_U32             det_hy_en        ;
    HI_U32             det_hc_en        ;
    HI_U32             det_vy_en        ;
    HI_U32             hy_tst_blk_num   ; // just for the stream mode
    HI_U32             hc_tst_blk_num   ; // just for the stream mode
    HI_U32             vy_tst_blk_num   ; // just for the stream mode
    HI_U32             min_blk_size     ; // just for the stream mode
    HI_U32             hy_max_blk_size  ; // just for the stream mode
    HI_U32             hc_max_blk_size  ; // just for the stream mode
    HI_U32             vy_max_blk_size  ; // just for the stream mode
}VDP_DB_DETECT_PARA_INFO;

//---------------------------------
//DB DETECT END
//---------------------------------


//ZME2 coef
typedef struct tagVDP_VID_ZME2_HCOEF_S
{
    HI_S32  zme2_hlcoef00; 
    HI_S32  zme2_hlcoef01; 
    HI_S32  zme2_hlcoef02; 
    HI_S32  zme2_hlcoef03; 
    HI_S32  zme2_hlcoef04; 
    HI_S32  zme2_hlcoef05; 
    HI_S32  zme2_hlcoef06; 
    HI_S32  zme2_hlcoef07; 
    HI_S32  zme2_hlcoef10; 
    HI_S32  zme2_hlcoef11; 
    HI_S32  zme2_hlcoef12; 
    HI_S32  zme2_hlcoef13; 
    HI_S32  zme2_hlcoef14; 
    HI_S32  zme2_hlcoef15; 
    HI_S32  zme2_hlcoef16; 
    HI_S32  zme2_hlcoef17; 

    HI_S32  zme2_hccoef00; 
    HI_S32  zme2_hccoef01; 
    HI_S32  zme2_hccoef02; 
    HI_S32  zme2_hccoef03; 
    HI_S32  zme2_hccoef10; 
    HI_S32  zme2_hccoef11; 
    HI_S32  zme2_hccoef12; 
    HI_S32  zme2_hccoef13;

}VDP_VID_ZME2_HCOEF_S;

typedef struct tagVDP_VID_ZME2_VCOEF_S
{
    HI_S32  zme2_vlcoef00; 
    HI_S32  zme2_vlcoef01; 
    HI_S32  zme2_vlcoef02; 
    HI_S32  zme2_vlcoef03; 
    HI_S32  zme2_vlcoef04; 
    HI_S32  zme2_vlcoef05; 
    HI_S32  zme2_vlcoef10; 
    HI_S32  zme2_vlcoef11; 
    HI_S32  zme2_vlcoef12; 
    HI_S32  zme2_vlcoef13; 
    HI_S32  zme2_vlcoef14; 
    HI_S32  zme2_vlcoef15; 

    HI_S32  zme2_vccoef00; 
    HI_S32  zme2_vccoef01; 
    HI_S32  zme2_vccoef02; 
    HI_S32  zme2_vccoef03; 
    HI_S32  zme2_vccoef10; 
    HI_S32  zme2_vccoef11; 
    HI_S32  zme2_vccoef12; 
    HI_S32  zme2_vccoef13;

}VDP_VID_ZME2_VCOEF_S;

#if 0
// used for communication between modules
typedef enum tagVDP_VMX_MODE_E
{
    VDP_VMX_TYP = 0,
    VDP_VMX_4K_SMALL_SYMBOLS ,
    VDP_VMX_MAXIMUM_COVERAGE ,
    VDP_VMX_NORM ,
    VDP_VMX_RAND_MULTIPLY_THRESHOLD , //all settings rand but :watermark is on, multiply is 100, threshold value use default value
    VDP_VMX_RAND_THRESHOLD ,    //all seetings rand but :watermark is on, threshold value use default value
    VDP_VMX_RAND ,              //all seetings rand include watermark on or not
    VDP_VMX_MAX  ,
    VDP_VMX_MIN  ,
    VDP_VMX_BUTT
}VDP_VMX_MODE_E;

typedef enum tagVDP_NXG_NXG_MODE_E
{
    VDP_NXG_NXG_TYP  = 0 , 
    VDP_NXG_NXG_NORM = 1 , 

    VDP_NXG_NXG_BUTT
}VDP_NXG_NXG_MODE_E;

#endif

#endif//_VDP_IP_DEFINE_H_
