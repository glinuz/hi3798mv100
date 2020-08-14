/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : optm_define.h
Version             : Initial Draft
Author              :
Created             : 2014/09/09
Description         : ALG这一层代码是由算法提供的
Function List       :
History             :
Date                       Author                   Modification
2014/09/09                 y00181162                Created file
******************************************************************************/

#ifndef __OPTM_DEFINE_H__
#define __OPTM_DEFINE_H__


/*********************************add include here******************************/
#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/
// test switch

#define OPTM_FPGA_TEST 1

#define OPTM_VID_MAX 4
#define OPTM_GFX_MAX 6
#define OPTM_WBC_MAX 1
#define OPTM_CHN_MAX 2
#define OPTM_GP_MAX  2
#define OPTM_VP_MAX  2
#define OPTM_CBM_MAX  (OPTM_VP_MAX + OPTM_GP_MAX)

#define OPTM_MAX_LAYER OPTM_VID_MAX + OPTM_GFX_MAX
#define OPTM_MIX_PARA  8


#define OPTM_WBC_GP0_SEL 1

#define OPTM_MAX_VALUE 255

#define HI_TYPE   1


#if !(OPTM_FPGA_TEST)
#define MyAssert(x) { if (!(x)) { printf("\nErr @ (%s, %d)\n", __FILE__, __LINE__); \
                                                         exit(-__LINE__); } }
#else
#define MyAssert(x)
#endif



/*************************** Structure Definition ****************************/

typedef enum tagVDP_LAYER_GFX_E
{
    OPTM_VDP_LAYER_GFX0  = 0,
    OPTM_VDP_LAYER_GFX1  = 1,
    OPTM_VDP_LAYER_GFX2  = 2,
    OPTM_VDP_LAYER_GFX3  = 3,
    OPTM_VDP_LAYER_GFX4  = 4,
    OPTM_VDP_LAYER_GFX5  = 5,

    OPTM_VDP_LAYER_GFX_BUTT

} OPTM_VDP_LAYER_GFX_E;

typedef enum tagVDP_LAYER_GP_E
{
    OPTM_VDP_LAYER_GP0   = 0,
    OPTM_VDP_LAYER_GP1   = 1,

    OPTM_VDP_LAYER_GP_BUTT

} OPTM_VDP_LAYER_GP_E;

typedef enum tagVDP_LAYER_WBC_E
{
    OPTM_VDP_LAYER_WBC_GP0  = 0,   /** 同源回写 **/
    OPTM_VDP_LAYER_WBC_HD0  = 1,
    OPTM_VDP_LAYER_WBC_G0   = 2,   /** 压缩回写 **/
    OPTM_VDP_LAYER_WBC_G4   = 3,   /** 压缩回写 **/
    OPTM_VDP_LAYER_WBC_BUTT

} OPTM_VDP_LAYER_WBC_E;

typedef enum tagOPTM_VDP_INTMSK_E
{
    OPTM_VDP_INTMSK_NONE      = 0,
    OPTM_VDP_INTMSK_WBC_GP0_INT = 0x100,
    OPTM_VDP_INTMSK_WBC_G0_INT  = 0x400,
    OPTM_VDP_INTMSK_WBC_G4_INT  = 0x800,
    OPTM_VDP_INTMSK_BUTT,
}OPTM_VDP_INTMSK_E;


typedef enum tagVDP_CHN_E
{
    OPTM_VDP_CHN_DHD0    = 0,
    OPTM_VDP_CHN_DHD1    = 1,
    OPTM_VDP_CHN_WBC0    = 2,
    OPTM_VDP_CHN_WBC1    = 3,
    OPTM_VDP_CHN_WBC2    = 4,
    OPTM_VDP_CHN_WBC3    = 5,
    OPTM_VDP_CHN_NONE    = 6,
    OPTM_VDP_CHN_BUTT

} OPTM_VDP_CHN_E;


typedef enum tagVDP_VID_IFMT_E
{
    OPTM_VDP_VID_IFMT_SP_400      = 0x1,
    OPTM_VDP_VID_IFMT_SP_420      = 0x3,
    OPTM_VDP_VID_IFMT_SP_422      = 0x4,
    OPTM_VDP_VID_IFMT_SP_444      = 0x5,
    OPTM_VDP_VID_IFMT_PKG_UYVY    = 0x9,
    OPTM_VDP_VID_IFMT_PKG_YUYV    = 0xa,
    OPTM_VDP_VID_IFMT_PKG_YVYU    = 0xb,

    OPTM_VDP_VID_IFMT_BUTT

}OPTM_VDP_VID_IFMT_E;

typedef enum tagVDP_GFX_IFMT_E
{
    VDP_GFX_IFMT_CLUT_1BPP   = 0x00,
    VDP_GFX_IFMT_CLUT_2BPP   = 0x10,
    VDP_GFX_IFMT_CLUT_4BPP   = 0x20,
    VDP_GFX_IFMT_CLUT_8BPP   = 0x30,

    VDP_GFX_IFMT_ACLUT_44    = 0x38,

    VDP_GFX_IFMT_RGB_444     = 0x40,
    VDP_GFX_IFMT_RGB_555     = 0x41,
    VDP_GFX_IFMT_RGB_565     = 0x42,

    VDP_GFX_IFMT_PKG_UYVY    = 0x43,
    VDP_GFX_IFMT_PKG_YUYV    = 0x44,
    VDP_GFX_IFMT_PKG_YVYU    = 0x45,

    VDP_GFX_IFMT_ACLUT_88    = 0x46,
    VDP_GFX_IFMT_ARGB_4444   = 0x48,
    VDP_GFX_IFMT_ARGB_1555   = 0x49,

    VDP_GFX_IFMT_RGB_888     = 0x50,//24bpp
    VDP_GFX_IFMT_YCBCR_888   = 0x51,//24bpp
    VDP_GFX_IFMT_ARGB_8565   = 0x5a,//24bpp

    VDP_GFX_IFMT_KRGB_888    = 0x60,
    VDP_GFX_IFMT_ARGB_8888   = 0x68,
    VDP_GFX_IFMT_AYCBCR_8888 = 0x69,

    VDP_GFX_IFMT_RGBA_4444   = 0xc8,
    VDP_GFX_IFMT_RGBA_5551   = 0xc9,
    VDP_GFX_IFMT_RGBA_5658   = 0xda,//24bpp
    VDP_GFX_IFMT_RGBA_8888   = 0xe8,
    VDP_GFX_IFMT_YCBCRA_8888 = 0xe9,
    VDP_GFX_IFMT_ABGR_8888   = 0xef,

    VDP_GFX_IFMT_BUTT

}OPTM_VDP_GFX_IFMT_E;

typedef enum tagVDP_PROC_FMT_E
{
    VDP_PROC_FMT_SP_422      = 0x0,
    VDP_PROC_FMT_SP_420      = 0x1,
    VDP_PROC_FMT_SP_444      = 0x2,

    VDP_PROC_FMT_BUTT

}OPTM_VDP_PROC_FMT_E;

typedef enum tagVDP_WBC_FMT_E
{
    VDP_WBC_OFMT_PKG_UYVY = 0,
    VDP_WBC_OFMT_PKG_YUYV = 1,
    VDP_WBC_OFMT_PKG_YVYU = 2,
    VDP_WBC_OFMT_ARGB8888 = 3,
    VDP_WBC_OFMT_SP420   = 4,
    VDP_WBC_OFMT_SP422   = 5,

    VDP_WBC_OFMT_BUUT

}OPTM_VDP_WBC_OFMT_E;



typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_SELF_ADAPTION = 0,  /** 自适应，隔行输出隔行读，逐行输出逐行读 **/
    VDP_RMODE_PROGRESSIVE,        /** 逐行读取                               **/
    VDP_RMODE_TOP,
    VDP_RMODE_BOTTOM,
    VDP_RMODE_BUTT

} OPTM_VDP_DATA_RMODE_E;

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

    VDP_ZME_MODE_ALL,
    VDP_ZME_MODE_NONL,
    VDP_ZME_MODE_BUTT

}OPTM_VDP_ZME_MODE_E;

typedef enum
{
    VDP_TI_MODE_LUM = 0,
    VDP_TI_MODE_CHM,

    VDP_TI_MODE_ALL,
    VDP_TI_MODE_NON,
    VDP_TI_MODE_BUTT

}OPTM_VDP_TI_MODE_E;



typedef enum tagVDP_ZME_ORDER_E
{
    VDP_ZME_ORDER_HV = 0x0,
    VDP_ZME_ORDER_VH = 0x1,

    VDP_ZME_ORDER_BUTT
} OPTM_VDP_ZME_ORDER_E;

typedef enum tagVDP_GP_ORDER_E
{
    VDP_GP_ORDER_NULL     = 0x0,
    VDP_GP_ORDER_CSC      = 0x1,
    VDP_GP_ORDER_ZME      = 0x2,
    VDP_GP_ORDER_CSC_ZME  = 0x3,
    VDP_GP_ORDER_ZME_CSC  = 0x4,

    VDP_GP_ORDER_BUTT
} OPTM_VDP_GP_ORDER_E;


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
} OPTM_VDP_DITHER_E;

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
} OPTM_VDP_DITHER_COEF_S;

typedef struct tagVDP_DISP_RECT_S
{
    HI_U32 u32SX;   /** source horizontal start position   **/
    HI_U32 u32SY;   /** source vertical start position     **/

    HI_U32 u32DXS;  /** dispaly horizontal start position  **/
    HI_U32 u32DYS;  /** display vertical start position    **/

    HI_U32 u32DXL;  /** dispaly horizontal end position    **/
    HI_U32 u32DYL;  /** display vertical end position      **/

    HI_U32 u32VX;   /** video horizontal start position    **/
    HI_U32 u32VY;   /** video vertical start position      **/

    HI_U32 u32VXL;  /** video horizontal start position    **/
    HI_U32 u32VYL;  /** video vertical start position      **/

    HI_U32 u32IWth; /** input width   **/
    HI_U32 u32IHgt; /** input height  **/
    HI_U32 u32OWth; /** output width  **/
    HI_U32 u32OHgt; /** output height **/

} OPTM_VDP_DISP_RECT_S;


typedef struct
{
    HI_U32 u32X;
    HI_U32 u32Y;

    HI_U32 u32Wth;
    HI_U32 u32Hgt;

} OPTM_VDP_RECT_S;

typedef enum tagVDP_GP_PARA_E
{
    VDP_GP_PARA_ZME_HOR = 0,
    VDP_GP_PARA_ZME_VER   ,

    VDP_GP_PARA_ZME_HORL  ,
    VDP_GP_PARA_ZME_HORC  ,
    VDP_GP_PARA_ZME_VERL  ,
    VDP_GP_PARA_ZME_VERC  ,

    VDP_GP_GTI_PARA_ZME_HORL  ,
    VDP_GP_GTI_PARA_ZME_HORC  ,
    VDP_GP_GTI_PARA_ZME_VERL  ,
    VDP_GP_GTI_PARA_ZME_VERC  ,

    VDP_GP_PARA_BUTT
} OPTM_VDP_GP_PARA_E;

typedef enum tagVDP_WBC_PARA_E
{
    VDP_WBC_PARA_ZME_HOR = 0,
    VDP_WBC_PARA_ZME_VER   ,

    VDP_WBC_PARA_ZME_HORL  ,
    VDP_WBC_PARA_ZME_HORC  ,
    VDP_WBC_PARA_ZME_VERL  ,
    VDP_WBC_PARA_ZME_VERC  ,

    VDP_WBC_GTI_PARA_ZME_HORL  ,
    VDP_WBC_GTI_PARA_ZME_HORC  ,
    VDP_WBC_GTI_PARA_ZME_VERL  ,
    VDP_WBC_GTI_PARA_ZME_VERC  ,

    VDP_WBC_PARA_BUTT
} OPTM_VDP_WBC_PARA_E;

//---------------------------------------
// Modules
//---------------------------------------

// bkg color patern fill
typedef struct tagVDP_BKG_S
{
    HI_U32 u32BkgY;
    HI_U32 u32BkgU;
    HI_U32 u32BkgV;

    HI_U32 u32BkgA;

    HI_BOOL bBkType;

} OPTM_VDP_BKG_S;

//-------------------
// CSC
//-------------------
typedef enum tagVDP_CSC_MODE_E
{
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

    VDP_CSC_BUTT
} OPTM_VDP_CSC_MODE_E;

//-------------------
// CSC
//-------------------
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


    HI_S32 rgb2yuv_coef00;
    HI_S32 rgb2yuv_coef01;
    HI_S32 rgb2yuv_coef02;

    HI_S32 rgb2yuv_coef10;
    HI_S32 rgb2yuv_coef11;
    HI_S32 rgb2yuv_coef12;

    HI_S32 rgb2yuv_coef20;
    HI_S32 rgb2yuv_coef21;
    HI_S32 rgb2yuv_coef22;

} OPTM_VDP_CSC_COEF_S;

typedef struct
{
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;

    HI_S32 rgb2yuv_out_dc0;
    HI_S32 rgb2yuv_out_dc1;
    HI_S32 rgb2yuv_out_dc2;

} OPTM_VDP_CSC_DC_COEF_S;

//for rm
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
} OPTM_VDP_CSC_CFG_S;

typedef enum tagOPTM_VDP_DISP_MODE_E
{
    VDP_DISP_MODE_2D  = 0,
    VDP_DISP_MODE_SBS = 1,
    VDP_DISP_MODE_TAB = 4,
    VDP_DISP_MODE_FP  = 5,

    VDP_DISP_MODE_BUTT
}OPTM_VDP_DISP_MODE_E;
//-------------------
// vou graphic layer data extend mode
//-------------------
typedef enum tagVDP_GFX_BITEXTEND_E
{
    VDP_GFX_BITEXTEND_1ST =   0,
    VDP_GFX_BITEXTEND_2ND = 0x2,
    VDP_GFX_BITEXTEND_3RD = 0x3,

    VDP_GFX_BITEXTEND_BUTT
}OPTM_VDP_GFX_BITEXTEND_E;

typedef enum tagVDP_CBM_MIX_E
{
    VDP_CBM_MIXV0 = 0,
    VDP_CBM_MIXV1 = 1,
    VDP_CBM_MIXG0 = 2,
    VDP_CBM_MIXG1 = 3,
    VDP_CBM_MIX0  = 4,
    VDP_CBM_MIX1  = 5,

    VDP_CBM_MIX_BUTT
}OPTM_VDP_CBM_MIX_E;

typedef enum
{
    VDP_DISP_COEFMODE_HOR  = 0,
    VDP_DISP_COEFMODE_VER,
    VDP_DISP_COEFMODE_LUT,
    VDP_DISP_COEFMODE_GAM,
    VDP_DISP_COEFMODE_ACC,
    VDP_DISP_COEFMODE_ABC,
    VDP_DISP_COEFMODE_ACM,
    VDP_DISP_COEFMODE_NR,
    VDP_DISP_COEFMODE_SHARP,
    VDP_DISP_COEFMODE_DIM,
    VDP_DISP_COEFMODE_DIMZMEH,
    VDP_DISP_COEFMODE_DIMZMEV,

    VDP_DISP_COEFMODE_ALL
}OPTM_VDP_DISP_COEFMODE_E;

typedef struct tagVDP_GFX_CKEY_S
{
    HI_U32 u32Key_r_min;
    HI_U32 u32Key_g_min;
    HI_U32 u32Key_b_min;

    HI_U32 u32Key_r_max;
    HI_U32 u32Key_g_max;
    HI_U32 u32Key_b_max;

    HI_U32 u32Key_r_msk;
    HI_U32 u32Key_g_msk;
    HI_U32 u32Key_b_msk;

    HI_U32 bKeyMode;

    HI_U32 u32KeyAlpha;

} OPTM_VDP_GFX_CKEY_S;

typedef struct tagVDP_GFX_MASK_S
{
    HI_U32 u32Mask_r;
    HI_U32 u32Mask_g;
    HI_U32 u32Mask_b;

} OPTM_VDP_GFX_MASK_S;



/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __OPTM_DEFINE_H__ */
