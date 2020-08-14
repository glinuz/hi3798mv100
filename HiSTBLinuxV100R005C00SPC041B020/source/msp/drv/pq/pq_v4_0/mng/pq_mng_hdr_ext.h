/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name    : pq_mng_hdr_ext.h
  Version       : Initial Draft
  Author        : p00203646
  Created      : 2016/06/13
  Description  :

******************************************************************************/
#ifndef __PQ_MNG_HDR_EXT_H__
#define __PQ_MNG_HDR_EXT_H__

#include "hi_drv_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(CHIP_TYPE_hi3798mv200)

#define PQ_DEGMM_SEG_SIZE		4
#define PQ_TMAP_SEG_SIZE		8
#define PQ_SMAP_SEG_SIZE		8
#define PQ_GMM_SEG_SIZE		    8
#define PQ_DEGMM_LUT_SIZE	   64
#define PQ_TMAP_LUT_SIZE	   64
#define PQ_SMAP_LUT_SIZE	   64
#define PQ_GMM_LUT_SIZE		   64

#elif  defined(CHIP_TYPE_hi3798mv200_a)

#define PQ_DEGMM_SEG_SIZE		4
#define PQ_TMAP_SEG_SIZE		4
#define PQ_SMAP_SEG_SIZE		4
#define PQ_GMM_SEG_SIZE		    8
#define PQ_DEGMM_LUT_SIZE	   64
#define PQ_TMAP_LUT_SIZE	   32
#define PQ_SMAP_LUT_SIZE	   32
#define PQ_GMM_LUT_SIZE		   64

#elif  defined(CHIP_TYPE_hi3796mv200)

#define PQ_DEGMM_SEG_SIZE		8
#define PQ_TMAP_SEG_SIZE		8
#define PQ_SMAP_SEG_SIZE		8
#define PQ_GMM_SEG_SIZE		    8
#define PQ_DEGMM_LUT_SIZE	   64
#define PQ_TMAP_LUT_SIZE	   64
#define PQ_SMAP_LUT_SIZE	   64
#define PQ_GMM_LUT_SIZE		   64

#else

#define PQ_DEGMM_SEG_SIZE		8
#define PQ_TMAP_SEG_SIZE		8
#define PQ_SMAP_SEG_SIZE		8
#define PQ_GMM_SEG_SIZE		    8
#define PQ_DEGMM_LUT_SIZE	   64
#define PQ_TMAP_LUT_SIZE	   64
#define PQ_SMAP_LUT_SIZE	   64
#define PQ_GMM_LUT_SIZE		   64
#endif



typedef enum hiPQ_DISP_OUT_TYPE_E
{
    HI_PQ_DISP_TYPE_NORMAL = 0,    /* 普通信号显示 BT601 BT709*/
    HI_PQ_DISP_TYPE_HDR10,         /* 标准HDR信号显示 */

    HI_PQ_DISP_TYPE_BUTT
} HI_PQ_DISP_OUT_TYPE_E;


typedef struct hiPQ_WIN_HDR_INFO_S
{
    HI_DRV_VIDEO_FRAME_TYPE_E enSrcFrameType;
    HI_DRV_VIDEO_STD_E enCodecType;
    union
    {
        HI_DRV_VIDEO_DOLBY_INFO_S       stDolbyInfo;       /*Dolby frame info.*/
        HI_DRV_VIDEO_HDR10_INFO_S       stHDR10Info;       /*HDR10 frame info.*/
        HI_DRV_VIDEO_HLG_INFO_S         stHLGInfo;         /*HLG frame info.*/
        HI_DRV_VIDEO_SLF_INFO_S         stSLFInfo;         /*SLF frame info.*/
        HI_DRV_VIDEO_TECHNICOLOR_INFO_S stTechnicolorInfo; /*Technicolor frame info.*/
    } unHDRInfo;
    HI_PQ_DISP_OUT_TYPE_E enDispType; /* SDR/HDR10/... */
} HI_PQ_WIN_HDR_INFO;

typedef enum hiPQ_HDR_COLOR_PRIMARIES_E
{
    HI_PQ_HDR_COLOR_PRIMARIES_BT709_L = 0x0,
    HI_PQ_HDR_COLOR_PRIMARIES_BT709_F,
    HI_PQ_HDR_COLOR_PRIMARIES_BT2020_L,
    HI_PQ_HDR_COLOR_PRIMARIES_BT2020_F,
    HI_PQ_HDR_COLOR_PRIMARIES_UNKNOW,

    HI_PQ_HDR_COLOR_PRIMARIES_BUTT
} HI_PQ_HDR_COLOR_PRIMARIES_E;

typedef enum hiPQ_HDR_EOTF_E
{
    HI_PQ_HDR_EOTF_1886 = 0x0,
    HI_PQ_HDR_EOTF_2084 ,
    HI_PQ_HDR_EOTF_HLG ,
    HI_PQ_HDR_EOTF_SLF ,

    HI_PQ_HDR_EOTF_BUTT
} HI_PQ_HDR_EOTF_E;

typedef enum hiPQ_HDR_MATRIX_COEFF_E
{
    HI_PQ_HDR_MATRIX_COEFF_BT709 = 0x0,
    HI_PQ_HDR_MATRIX_COEFF_BT2020_NCL,
    HI_PQ_HDR_MATRIX_COEFF_BT2020_CL,
    HI_PQ_HDR_MATRIX_COEFF_UNKNOW,

    HI_PQ_HDR_MATRIX_COEFF_BT2020_BUTT

} HI_PQ_HDR_MATRIX_COEFF_E;

typedef struct hiPQ_HDR_UNIFIED_METADATA_IN_S
{
    HI_DRV_VIDEO_FRAME_TYPE_E enSrcFrameType;
    HI_DRV_COLOUR_DESCRIPTION_INFO_S stColorDescriptionInfo;
    HI_U8 u8TransferCharacteristics;

    HI_BOOL  bMasteringAvailable;
    HI_BOOL  bContentAvailable;
    HI_DRV_MASTERING_DISPLAY_COLOUR_VOLUME_S stMasteringInfo;
    HI_DRV_CONTENT_LIGHT_LEVEL_INFO_S stContentInfo;
} HI_PQ_HDR_UNIFIED_METADATA_IN_S;

typedef struct hiPQ_HDR_UNIFIED_METADATA_OUT_S
{
    HI_DRV_VIDEO_FRAME_TYPE_E enSrcFrameType;
    HI_PQ_HDR_COLOR_PRIMARIES_E enColorSpace;
    HI_PQ_HDR_EOTF_E enEotfType;
    HI_PQ_HDR_MATRIX_COEFF_E enMatrixType;

    HI_BOOL  bMasteringAvailable;
    HI_BOOL  bContentAvailable;
    HI_DRV_MASTERING_DISPLAY_COLOUR_VOLUME_S stMasteringInfo;
    HI_DRV_CONTENT_LIGHT_LEVEL_INFO_S stContentInfo;
} HI_PQ_HDR_UNIFIED_METADATA_OUT_S;

#if defined(CHIP_TYPE_hi3798cv200)

#define HI_PQ_HDR_TM_LUT_SIZE 512

typedef struct HiPQ_HDR_TM
{
    /*ToneMapping*/
    HI_S16 s16aTMLutI[HI_PQ_HDR_TM_LUT_SIZE];
    HI_S16 s16aTMLutS[HI_PQ_HDR_TM_LUT_SIZE];
    HI_S16 s16aSMLutI[HI_PQ_HDR_TM_LUT_SIZE];
    HI_S16 s16aSMLutS[HI_PQ_HDR_TM_LUT_SIZE];
} HI_PQ_HDR_TM;

typedef struct HiPQ_HDR_CFG
{
    HI_PQ_HDR_TM stPQHdrTm;
} HI_PQ_HDR_CFG;

#else

typedef struct hiPQ_HDR_Y2R
{
    HI_U16 u16ScaleY2R;				        /* U4.0  [0,15] */
    HI_S16 as16M33Y2R[3][3];		        /* S2.13 [-2^15, 2^15-1] = [-32768, 32767] */
    HI_S16 as16DcInY2R[3];			        /* S10.0  [-1024, 1023] */
    HI_S16 as16DcOutY2R[3];			        /* S12.0  [-4096, 4095] */
    HI_U16 u16ClipMinY2R ;			        /* U12.0  [0,4095]  */
    HI_U16 u16ClipMaxY2R ;			        /* U12.0  [0,4095]  */
} PQ_HDR_Y2R;

typedef struct hiPQ_HDR_DEGMM
{
    HI_U32	au32Step[PQ_DEGMM_SEG_SIZE];	/* U4.0  [0,15] */
    HI_U32	au32Pos[PQ_DEGMM_SEG_SIZE];		/* U12.0  [0,4095] */
    HI_U32	au32Num[PQ_DEGMM_SEG_SIZE];		/* U6.0  [0,63] */
    HI_U32*	pu32LUT	;						/* U21.0 [0,2097151]   Size:PQ_DEGMM_LUT_SIZE */
} PQ_HDR_DEGMM;

typedef struct hiPQ_HDR_TMAP
{
    HI_U16 u16ScaleLumaCal ;                /* U4.0  [0,15]*/
    HI_U16 au16M3LumaCal[3] ;               /* U1.15 [0, 65535]*/
    HI_U32 u32ClipMinTM ;                   /* U21.0 [0,2097151]*/
    HI_U32 u32ClipMaxTM ;                   /* U21.0 [0,2097151]*/
    HI_S32 as32TMOff[3] ;		            /* S21.0 [-2097151,2097151]*/

    HI_U16  u16ScaleCoefTM ;		   		/* U4.0  [0,15]*/
    HI_U32	au32StepTM[PQ_TMAP_SEG_SIZE];	/* U5.0  [0,21]*/
    HI_U32	au32PosTM[PQ_TMAP_SEG_SIZE];	/* U21.0 [0,2097151]*/
    HI_U32	au32NumTM[PQ_TMAP_SEG_SIZE];	/* U6.0  [0,63]*/
    HI_U32*	pu32LUTTM	;					/* U16.0 [0,65535]   Size:PQ_TMAP_LUT_SIZE*/

    HI_U16  u16ScaleCoefSM ;		   		/* U4.0  [0,15]*/
    HI_U32	u32StepSM[PQ_SMAP_SEG_SIZE];	/* U5.0  [0,21]*/
    HI_U32	u32PosSM[PQ_SMAP_SEG_SIZE];		/* U21.0 [0,2097151]*/
    HI_U32	u32NumSM[PQ_SMAP_SEG_SIZE];		/* U6.0  [0,63]*/
    HI_U32*	pu32LUTSM	;					/* U16.0 [0,65535]   Size:PQ_SMAP_LUT_SIZE*/
} PQ_HDR_TMAP;

typedef struct hiPQ_HDR_GMAP
{
    HI_U16 u16ScaleGMAP;	                /* U4.0  [0,15] */
    HI_S16 as16M33GMAP[3][3];               /* S1.14 [-2^15, 2^15-1] = [-32768, 32767]  */
    HI_U32 u32ClipMinGMAP;                  /* U21.0 [0,2097151] */
    HI_U32 u32ClipMaxGMAP;                  /*  U21.0 [0,2097151] */
    HI_S32 as32DcInGMAP[3];	                /*  S21.0 [-2097152,2097151] */
    HI_S32 as32DcOutGMAP[3];                /*  S21.0 [-2097152,2097151] */
} PQ_HDR_GMAP;

typedef struct hiPQ_HDR_GMM
{
    HI_U32	au32Step[PQ_GMM_SEG_SIZE];		/* U5.0  [0,21] */
    HI_U32	au32Pos[PQ_GMM_SEG_SIZE];		/* U21.0 [0,2097151] */
    HI_U32	au32Num[PQ_GMM_SEG_SIZE];		/* U6.0  [0,63] */
    HI_U32*	pu32LUT	;						/* U12.0 [0,4095]   Size:PQ_GMM_LUT_SIZE */
} PQ_HDR_GMM;

typedef struct hiPQ_HDR_R2Y
{
    HI_U16 u16ScaleR2Y ;       		         /*U4.0  [0,15] */
    HI_S16 as16M33R2Y[3][3];    		     /* S1.14 [-32768, 32767] */
    HI_S16 as16DcInR2Y[3] ;     		     /* S10.0  [-1024, 1023] */
    HI_S16 as16DcOutR2Y[3] ;    		     /* S10.0  [-1024,1023] */
    HI_U16 u16ClipMinR2Y_Y ;   		         /* U10.0  [0,1023] */
    HI_U16 u16ClipMaxR2Y_Y ;   		         /* U10.0  [0,1023] */
    HI_U16 u16ClipMinR2Y_UV ;  		         /* U10.0  [0,1023] */
    HI_U16 u16ClipMaxR2Y_UV ;  		         /* U10.0  [0,1023] */
} PQ_HDR_R2Y;

typedef struct hiPQ_HDR_CFG
{
    /**** Enable  ****/
    HI_BOOL bY2REn			;	      /*YUV2RGB enable*/
    HI_BOOL bDegammaEn		;	      /*DeGamma enable*/
    HI_BOOL bTMapEn		    ;	      /*ToneMapping enable*/
    HI_BOOL bGMapEn		    ;	      /*GamutMapping enable*/
    HI_BOOL bGammaEn		;	      /*Gamma enable*/
    HI_BOOL bDitherEn		;	      /*Dither enable*/
    HI_BOOL bR2YEn			;	      /*RGB2YUV enable*/
    HI_BOOL bChromaAdjEn	;	      /*ChromaAdjust enable*/
    HI_BOOL bBT2020CL		;	      /* 1：BT2020CL； 0：BT2020NCL */

    /**** Mode Sel ****/
    HI_BOOL bGMapPosSel		;	      /* 1: GMAP after TMAP;   0: GMAP before TMAP */
    HI_BOOL bDitherMode	    ;	      /* 1: Dither; 0: Round */

    /***** Coef *****/
    PQ_HDR_Y2R   stY2R      ;         /* YUV2RGB */
    PQ_HDR_DEGMM stDeGmm    ;         /* DeGamma */
    PQ_HDR_TMAP  stTMAP     ;         /* ToneMapping */
    PQ_HDR_GMAP	 stGMAP     ;         /* GamutMapping */
    PQ_HDR_GMM   stGmm      ;         /* Gamma */
    PQ_HDR_R2Y   stR2Y      ;         /* RGB2YUV */
} HI_PQ_HDR_CFG;

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


