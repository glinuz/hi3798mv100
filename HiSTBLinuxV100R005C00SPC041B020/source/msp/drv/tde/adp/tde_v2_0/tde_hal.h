/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_hal.h
Version             : Initial Draft
Author              :
Created             : 2014/06/20
Description         :
Function List       :
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file
******************************************************************************/
#ifndef  __TDE_HAL_H__
#define  __TDE_HAL_H__

#include "hi_tde_type.h"
#include "tde_adp.h"
#include "tde_define.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */


/****************************************************************************/
/*                             TDE hal types define                         */
/****************************************************************************/

/* TDE pixel format, accord with register, value cannot modified freely */
typedef enum hiTDE_DRV_COLOR_FMT_E
{
    TDE_DRV_COLOR_FMT_ARGB8888 = 0,
    TDE_DRV_COLOR_FMT_KRGB8888 = 1,
    TDE_DRV_COLOR_FMT_ARGB4444 = 2,
    TDE_DRV_COLOR_FMT_ARGB1555 = 3,
    TDE_DRV_COLOR_FMT_ARGB8565 = 4,
    TDE_DRV_COLOR_FMT_RGB888 = 5,
    TDE_DRV_COLOR_FMT_RGB444 = 6,
    TDE_DRV_COLOR_FMT_RGB555 = 7,
    TDE_DRV_COLOR_FMT_RGB565 = 8,
    TDE_DRV_COLOR_FMT_A1 = 9,
    TDE_DRV_COLOR_FMT_A8 = 10,
    TDE_DRV_COLOR_FMT_YCbCr888 = 11,
    TDE_DRV_COLOR_FMT_AYCbCr8888 = 12,
    TDE_DRV_COLOR_FMT_CLUT1 = 16,
    TDE_DRV_COLOR_FMT_CLUT2 = 17,
    TDE_DRV_COLOR_FMT_CLUT4 = 18,
    TDE_DRV_COLOR_FMT_CLUT8 = 19,
    TDE_DRV_COLOR_FMT_ACLUT44 = 20,
    TDE_DRV_COLOR_FMT_ACLUT88 = 21,
    TDE_DRV_COLOR_FMT_YCbCr422 = 33,
    TDE_DRV_COLOR_FMT_byte = 300,
    TDE_DRV_COLOR_FMT_halfword = 34,
    TDE_DRV_COLOR_FMT_YCbCr400MBP = 48,
    TDE_DRV_COLOR_FMT_YCbCr422MBH = 49,
    TDE_DRV_COLOR_FMT_YCbCr422MBV = 50,
    TDE_DRV_COLOR_FMT_YCbCr420MB = 51,
    TDE_DRV_COLOR_FMT_YCbCr444MB = 52,
    TDE_DRV_COLOR_FMT_RABG8888,
    TDE_DRV_COLOR_FMT_BUTT
} TDE_DRV_COLOR_FMT_E;

/* branch order*/
/* support 24 orders in ARGB, TDE driver can only expose 4 usual orders; if needed, can be added */
typedef enum hiTDE_DRV_ARGB_ORDER_E
{
    TDE_DRV_ORDER_ARGB = 0x0,
    TDE_DRV_ORDER_ABGR = 0x5,
	TDE_DRV_ORDER_RABG = 0x7,
    TDE_DRV_ORDER_RGBA = 0x9,
    TDE_DRV_ORDER_BGRA = 0x14,
    TDE_DRV_ORDER_BUTT
}TDE_DRV_ARGB_ORDER_E;

/* TDE basic operate mode */
typedef enum hiTDE_DRV_BASEOPT_MODE_E
{
    /* Quick fill */
    TDE_QUIKE_FILL,

    /* Quick copy */
    TDE_QUIKE_COPY,

    /* Normal fill in single source */
    TDE_NORM_FILL_1OPT,

    /* Normal bilit in single source */
    TDE_NORM_BLIT_1OPT,

    /* Fill and Rop */
    TDE_NORM_FILL_2OPT,

    /* Normal bilit in double source */
    TDE_NORM_BLIT_2OPT,

    /* MB operation */
    TDE_MB_C_OPT,    /* MB chroma zoom */
    TDE_MB_Y_OPT,    /* MB brightness zoom */
    TDE_MB_2OPT,     /* MB combinate operation */

    /* Fill operate in single source mode  */
    TDE_SINGLE_SRC_PATTERN_FILL_OPT,

    /* Fill operate in double source mode */
    TDE_DOUBLE_SRC_PATTERN_FILL_OPT
} TDE_DRV_BASEOPT_MODE_E;

/* Type definition in interrupted state */
typedef enum hiTDE_DRV_INT_STATS_E
{
    TDE_DRV_LINK_COMPLD_STATS = 0x1,
    TDE_DRV_NODE_COMPLD_STATS = 0x2,
    TDE_DRV_LINE_SUSP_STATS = 0x4,
    TDE_DRV_RDY_START_STATS = 0x8,
    TDE_DRV_SQ_UPDATE_STATS = 0x10,
    TDE_DRV_INT_ALL_STATS = 0x800F001F
} TDE_DRV_INT_STATS_E;

/* ColorKey mode is needed by hardware */
typedef enum hiTDE_DRV_COLORKEY_MODE_E
{
    TDE_DRV_COLORKEY_BACKGROUND = 0,          		/* color key in bkground bitmap */
    TDE_DRV_COLORKEY_FOREGROUND_BEFORE_CLUT = 2,  	/* color key in foreground bitmap,before CLUT */
    TDE_DRV_COLORKEY_FOREGROUND_AFTER_CLUT = 3    	/* color key in bkground bitmap, after CLUT */
} TDE_DRV_COLORKEY_MODE_E;

/* color key setting arguments*/
typedef struct hiTDE_DRV_COLORKEY_CMD_S
{
    TDE_DRV_COLORKEY_MODE_E enColorKeyMode;        	/* color key mode */
    TDE2_COLORKEY_U        unColorKeyValue;       	/* color key value */
} TDE_DRV_COLORKEY_CMD_S;

/* Deficker filting mode */
typedef enum hiTDE_DRV_FLICKER_MODE
{
    TDE_DRV_FIXED_COEF0 = 0,   /* Deficker by fixed coefficient: 0 */
    TDE_DRV_AUTO_FILTER,       /* Deficker by auto filter */
    TDE_DRV_TEST_FILTER        /* Deficker by test filter */
} TDE_DRV_FLICKER_MODE;

/* Block type, equipped register note in numerical value reference */
typedef enum hiTDE_SLICE_TYPE_E
{
    TDE_NO_BLOCK_SLICE_TYPE = 0,         /* No block */
    TDE_FIRST_BLOCK_SLICE_TYPE = 0x3,    /* First block */
    TDE_LAST_BLOCK_SLICE_TYPE = 0x5,     /* Last block */
    TDE_MID_BLOCK_SLICE_TYPE = 0x1       /* Middle block */
} TDE_SLICE_TYPE_E;

/* vertical/horizontal filt mode: available for zoom */
typedef enum hiTDE_DRV_FILTER_MODE_E
{
    TDE_DRV_FILTER_NONE = 0,    /* none filt*/
    TDE_DRV_FILTER_ALL          /* filt on Alpha and color value */
} TDE_DRV_FILTER_MODE_E;


typedef enum hiTDE_ZME_FMT_MODE_E
{
    TDE_ZME_FMT_YUV422 = 0,    /* none filt*/
    TDE_ZME_FMT_YUV420,       /* filt on color parameter */
    TDE_ZME_FMT_YUV444,       /* filt on Alpha value */
    TDE_ZME_FMT_ARGB          /* filt on Alpha and color value */

} TDE_ZME_FMT_MODE_E;

/* Deflicker operate setting */
typedef struct hiTDE_DRV_FLICKER_CMD_S
{
    TDE_DRV_FLICKER_MODE enDfeMode;
    TDE_DRV_FILTER_MODE_E enFilterV;
    HI_U8            u8Coef0LastLine;
    HI_U8            u8Coef0CurLine;
    HI_U8            u8Coef0NextLine;
    HI_U8            u8Coef1LastLine;
    HI_U8            u8Coef1CurLine;
    HI_U8            u8Coef1NextLine;
    HI_U8            u8Coef2LastLine;
    HI_U8            u8Coef2CurLine;
    HI_U8            u8Coef2NextLine;
    HI_U8            u8Coef3LastLine;
    HI_U8            u8Coef3CurLine;
    HI_U8            u8Coef3NextLine;
    HI_U8            u8Threshold0;
    HI_U8            u8Threshold1;
    HI_U8            u8Threshold2;
    TDE2_DEFLICKER_MODE_E enDeflickerMode;
} TDE_DRV_FLICKER_CMD_S;


typedef struct hiTDE_DRV_HSP_S
{
    HI_U32    hratio                : 24  ; /* [23..0]  */
    HI_U32    hfir_order            : 1   ; /* [24]  */
    HI_U32    hchfir_en             : 1   ; /* [25]  */
    HI_U32    hlfir_en              : 1   ; /* [26]  */
    HI_U32    hafir_en              : 1   ; /* [27]  */
    HI_U32    hchmid_en             : 1   ; /* [28]  */
    HI_U32    hlmid_en              : 1   ; /* [29]  */
    HI_U32    hchmsc_en             : 1   ; /* [30]  */
    HI_U32    hlmsc_en              : 1   ; /* [31]  */
}TDE_DRV_HSP_S;


/* Zoom operate settings */
typedef struct hiTDE_DRV_RESIZE_CMD_S
{
    HI_U32            u32OffsetX;
    HI_U32            u32OffsetY;
    HI_U32            u32StepH;
    HI_U32            u32StepV;
    HI_BOOL           bCoefSym;
    HI_BOOL           bVfRing;
    HI_BOOL           bHfRing;
    TDE_DRV_FILTER_MODE_E enFilterV;
    TDE_DRV_FILTER_MODE_E enFilterH;
    HI_BOOL           bFirstLineOut;
    HI_BOOL           bLastLineOut;
} TDE_DRV_RESIZE_CMD_S;

/* Clip Setting */
typedef struct hiTDE_DRV_CLIP_CMD_S
{
    HI_U16  u16ClipStartX;
    HI_U16  u16ClipStartY;
    HI_U16  u16ClipEndX;
    HI_U16  u16ClipEndY;
    HI_BOOL bInsideClip;
} TDE_DRV_CLIP_CMD_S;

/* clut mode */
typedef enum hiTDE_DRV_CLUT_MODE_E
{
    /* color expand */
    TDE_COLOR_EXP_CLUT_MODE = 0,

    /* color correct */
    TDE_COLOR_CORRCT_CLUT_MODE
} TDE_DRV_CLUT_MODE_E;

/* clut setting */
typedef struct hiTDE_DRV_CLUT_CMD_S
{
    TDE_DRV_CLUT_MODE_E enClutMode;
    HI_U32          u32PhyClutAddr;
} TDE_DRV_CLUT_CMD_S;

/* MB Setting */
typedef enum hiTDE_DRV_MB_OPT_MODE_E
{
    TDE_MB_Y_FILTER = 0, 		/* brightness filt */
    TDE_MB_CbCr_FILTER = 2, 	/* chroma filt*/
    TDE_MB_UPSAMP_CONCA = 4,	/* first upsample then contact in chroma and brightness */
    TDE_MB_CONCA_FILTER = 6, 	/* first contact in chroma and brightness and then filt */
} TDE_DRV_MB_OPT_MODE_E;

/* MB Command Setting */
typedef struct hiTDE_DRV_MB_CMD_S
{
    TDE_DRV_MB_OPT_MODE_E enMbMode;        /* MB Operate Mode */
} TDE_DRV_MB_CMD_S;

/* plane mask command setting */
typedef struct hiTDE_DRV_PLMASK_CMD_S
{
    HI_U32 u32Mask;
} TDE_DRV_PLMASK_CMD_S;

/* Color zone convert setting */
typedef struct hiTDE_DRV_CONV_MODE_CMD_S
{
    /* Import Metrix used by color converted:graphic:0/video:1 */
    HI_U8 bInMetrixVid;

    /* Import standard in color convertion:IT-U601:0/ITU-709:1 */
    HI_U8 bInMetrix709;

    /* Export Metrix used by color converted:graphic:0/video:1 */
    HI_U8 bOutMetrixVid;

    /* Import standard in color conversion:IT-U601:0/ITU-709:1 */
    HI_U8 bOutMetrix709;

    /* Enable or unable conversion on importing color zone */
    HI_U8 bInConv;

    /* Enable or unable conversion on exporting color zone */
    HI_U8 bOutConv;
    HI_U8 bInSrc1Conv;

    /* import color conversion direction */
    HI_U8 bInRGB2YC;
} TDE_DRV_CONV_MODE_CMD_S;

/* vertical scanning direction */
typedef enum hiTDE_DRV_VSCAN_E
{
    TDE_SCAN_UP_DOWN = 0,	/* form up to down */
    TDE_SCAN_DOWN_UP = 1 	/* form down to up */
} TDE_DRV_VSCAN_E;

/* horizontal scanning direction */
typedef enum hiTDE_DRV_HSCAN_E
{
    TDE_SCAN_LEFT_RIGHT = 0,	/* form left to right */
    TDE_SCAN_RIGHT_LEFT = 1 	/* form right to left */
} TDE_DRV_HSCAN_E;

/* Definition on scanning direction */
typedef struct hiTDE_SCANDIRECTION_S
{
    /* vertical scanning direction */
    TDE_DRV_VSCAN_E enVScan;

    /* horizontal scanning direction */
    TDE_DRV_HSCAN_E enHScan;
} TDE_SCANDIRECTION_S;

/*  Between bitmap info struct setted by driver , by user and hardware info is not all
	the same. eg, bitmap info can be divided into two bitmap info: src1 and src2,
	which is hardware needs, when user set for MB.

	In MB mode(refer to TDE_INS register), pu8PhyCbCr is not used in driver, but divided into
	head addr of src1 and src2.
*/

/* TDEV240 version:
1. In nonMB mod, you can support MB. Because adding two membet variables:u32CbCrPhyAddr¡¢u32CbCrPitch,
which for Src1 and Src2 add one assistant channel by each.

2.Support component order in ARGB/RGB format(24 kinds in toal)
,add component order register and member variables:enRgbOrder.
*/

typedef struct hiTDE_DRV_SURFACE_S
{
    /* Bitmap head addr */
    HI_U32 u32PhyAddr;

    /* color format */
    TDE_DRV_COLOR_FMT_E enColorFmt;

    /* ARGB component order */
    TDE_DRV_ARGB_ORDER_E enRgbOrder;

    /* Position X at first */
    HI_U32 u32Xpos;

    /* Position Y at first */
    HI_U32 u32Ypos;

    /* Bitmap Height */
    HI_U32 u32Height;

    /* Bitmap Width */
    HI_U32 u32Width;

    /* Bitmap Pitch */
    HI_U32 u32Pitch;

    /* CbCr component addr */
    HI_U32 u32CbCrPhyAddr;

    /* CbCr pitch*/
    HI_U32 u32CbCrPitch;

    /* alpha max value is 255?or 128? */
    HI_BOOL bAlphaMax255;

    /* Vertical scanning direction */
    TDE_DRV_VSCAN_E enVScan;

    /* Horizontal scanning direction */
    TDE_DRV_HSCAN_E enHScan;
	HI_BOOL bCma;
} TDE_DRV_SURFACE_S;

/* MB bitmap info */
typedef struct hiTDE_DRV_MB_S
{
    TDE_DRV_COLOR_FMT_E enMbFmt;
    HI_U32              u32YPhyAddr;
    HI_U32              u32YWidth;
    HI_U32              u32YHeight;
    HI_U32              u32YStride;
    HI_U32              u32CbCrPhyAddr;
    HI_U32              u32CbCrStride;
} TDE_DRV_MB_S;


/* ALU mode*/
typedef enum hiTDE_DRV_ALU_MODE_E
{
    TDE_SRC1_BYPASS = 0,
    TDE_ALU_NONE,	/* register has no setting, used in flag */
    TDE_ALU_ROP,
    TDE_SRC2_BYPASS,
    TDE_ALU_MASK_ROP1,
    TDE_ALU_MASK_ROP2,
    TDE_ALU_MASK_BLEND,
    TDE_ALU_BLEND,
    TDE_ALU_BLEND_SRC2,
    TDE_ALU_CONCA,
    TDE_CLIP,
    TDE_BUTT
} TDE_DRV_ALU_MODE_E;



typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_fmt              : 6   ; /* [5..0]  */
        HI_U32    src1_argb_order       : 5   ; /* [10..6]  */
        HI_U32    src1_cbcr_order       : 1   ; /* [11]  */
        HI_U32    src1_rgb_exp          : 2   ; /* [13..12]  */
        HI_U32    reserved_0            : 4   ; /* [17..14]  */
        HI_U32    src1_alpha_range      : 1   ; /* [18]  */
        HI_U32    src1_v_scan_ord       : 1   ; /* [19]  */
        HI_U32    src1_h_scan_ord       : 1   ; /* [20]  */
        HI_U32    src1_422v_pro       : 1   ; /* [21]  */
        HI_U32    reserved_1            : 7   ; /* [28..22]  */
        HI_U32    src1_dma              : 1   ; /* [29]  */
        HI_U32    src1_mode             : 1   ; /* [30]  */
        HI_U32    src1_en               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CTRL;

/* Define the union U_SRC1_CH0_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_ch0_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC1_CH0_ADDR;
/* Define the union U_SRC1_CH1_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_ch1_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC1_CH1_ADDR;
/* Define the union U_SRC1_CH0_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_ch0_stride        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC1_CH0_STRIDE;
/* Define the union U_SRC1_CH1_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_ch1_stride        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC1_CH1_STRIDE;
/* Define the union U_SRC1_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_width            : 16  ; /* [15..0]  */
        HI_U32    src1_height           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_IMGSIZE;

/* Define the union U_SRC1_FILL */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_color_fill        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC1_FILL;
/* Define the union U_SRC1_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_alpha0           : 8   ; /* [7..0]  */
        HI_U32    src1_alpha1           : 8   ; /* [15..8]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_ALPHA;

/* Define the union U_SRC1_PIX_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_hoffset_pix      : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_PIX_OFFSET;

/* Define the union U_SRC2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_fmt              : 6   ; /* [5..0]  */
        HI_U32    src2_argb_order       : 5   ; /* [10..6]  */
        HI_U32    src2_cbcr_order       : 1   ; /* [11]  */
        HI_U32    src2_rgb_exp          : 2   ; /* [13..12]  */
        HI_U32    src2_clut_mode            : 1  ;
        HI_U32    reserved_0            : 3   ; /* [17..14]  */
        HI_U32    src2_alpha_range      : 1   ; /* [18]  */
        HI_U32    src2_v_scan_ord       : 1   ; /* [19]  */
        HI_U32    src2_h_scan_ord       : 1   ; /* [20]  */
        HI_U32    src2_422v_pro         : 1   ; /* [21]  */
        HI_U32    reserved_1            : 7   ; /* [28..21]  */
        HI_U32    src2_mode             : 2   ; /* [30..29]  */
        HI_U32    src2_en               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CTRL;


typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_start_x      : 16  ; /* [15..0]  */
        HI_U32    des_start_y      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_SUR_XY;

/* Define the union U_SRC2_CH0_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_ch0_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC2_CH0_ADDR;
/* Define the union U_SRC2_CH1_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_ch1_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC2_CH1_ADDR;
/* Define the union U_SRC2_CH0_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_ch0_stride        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC2_CH0_STRIDE;
/* Define the union U_SRC2_CH1_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_ch1_stride        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC2_CH1_STRIDE;
/* Define the union U_SRC2_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_width            : 16  ; /* [15..0]  */
        HI_U32    src2_height           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_IMGSIZE;

/* Define the union U_SRC2_FILL */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_color_fill        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_SRC2_FILL;
/* Define the union U_SRC2_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_alpha0           : 8   ; /* [7..0]  */
        HI_U32    src2_alpha1           : 8   ; /* [15..8]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_ALPHA;

/* Define the union U_SRC2_PIX_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_hoffset_pix      : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_PIX_OFFSET;

typedef union
{
    struct
    {
        HI_U32 u32X            :12; /* First X coordinate */
        HI_U32 u32Reserve1     :4;  /* Reserve */
        HI_U32 u32Y            :12; /* First Y coordinate */
        HI_U32 u32Reserve2     :4;  /* Reserve */
    }bits;
    HI_U32 u32All;
}U_SUR_XY;








/* Define the union U_DES_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_fmt               : 6   ; /* [5..0]  */
        HI_U32    des_argb_order        : 5   ; /* [10..6]  */
        HI_U32    des_cbcr_order        : 1   ; /* [11]  */
        HI_U32    reserved_0            : 5   ; /* [16..12]  */
        HI_U32    des_rgb_round         : 1   ; /* [17]  */
        HI_U32    des_alpha_range       : 1   ; /* [18]  */
        HI_U32    des_v_scan_ord        : 1   ; /* [19]  */
        HI_U32    des_h_scan_ord        : 1   ; /* [20]  */
        HI_U32    reserved_1            : 2   ; /* [22..21]  */
        HI_U32    cmp_en                : 1   ; /* [23]  */
        HI_U32    reserved_2            : 3   ; /* [26..24]  */
        HI_U32    des_ch1_mmu_bypass    : 1   ; /* [27]  */
        HI_U32    des_ch0_mmu_bypass    : 1   ; /* [28]  */
        HI_U32    reserved_3            : 2   ; /* [30..29]  */
        HI_U32    des_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CTRL;

/* Define the union U_DES_CH0_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 des_ch0_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_DES_CH0_ADDR;
/* Define the union U_DES_CH1_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 des_ch1_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_DES_CH1_ADDR;
/* Define the union U_DES_CH0_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 des_ch0_stride         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_DES_CH0_STRIDE;
/* Define the union U_DES_CH1_STRIDE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 des_ch1_stride         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_DES_CH1_STRIDE;
/* Define the union U_DES_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_width             : 16  ; /* [15..0]  */
        HI_U32    des_height            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_IMGSIZE;

/* Define the union U_DES_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_alpha_thd         : 8   ; /* [7..0]  */
        HI_U32    des_crop_mode         : 1   ; /* [8]  */
        HI_U32    des_crop_en           : 1   ; /* [9]  */
        HI_U32    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_ALPHA;

/* Define the union U_DES_CROP_POS_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_crop_start_x      : 16  ; /* [15..0]  */
        HI_U32    des_crop_start_y      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CROP_POS_ST;

/* Define the union U_DES_CROP_POS_ED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_crop_end_x        : 16  ; /* [15..0]  */
        HI_U32    des_crop_end_y        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CROP_POS_ED;

/* Define the union U_MMU_PROT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_ch0_mmu_bypass   : 1   ; /* [0]  */
        HI_U32    src1_ch1_mmu_bypass   : 1   ; /* [1]  */
        HI_U32    src2_ch0_mmu_bypass   : 1   ; /* [2]  */
        HI_U32    src2_ch1_mmu_bypass   : 1   ; /* [3]  */
        HI_U32    gdc3_mmu_bypass       : 1   ; /* [4]  */
        HI_U32    gdc4_mmu_bypass       : 1   ; /* [5]  */
        HI_U32    gdc5_mmu_bypass       : 1   ; /* [6]  */
        HI_U32    gdc6_mmu_bypass       : 1   ; /* [7]  */
        HI_U32    gdc7_mmu_bypass       : 1   ; /* [8]  */
        HI_U32    clut_mmu_bypass       : 1   ; /* [9]  */
        HI_U32    reserved_0            : 6   ; /* [15..10]  */
        HI_U32    src1_ch0_prot         : 1   ; /* [16]  */
        HI_U32    src1_ch1_prot         : 1   ; /* [17]  */
        HI_U32    src2_ch0_prot         : 1   ; /* [18]  */
        HI_U32    src2_ch1_prot         : 1   ; /* [19]  */
        HI_U32    gdc3_prot             : 1   ; /* [20]  */
        HI_U32    gdc4_prot             : 1   ; /* [21]  */
        HI_U32    gdc5_prot             : 1   ; /* [22]  */
        HI_U32    gdc6_prot             : 1   ; /* [23]  */
        HI_U32    gdc7_prot             : 1   ; /* [24]  */
        HI_U32    des_hdgb_prot             : 1   ; /* [25]  */
        HI_U32    des_hdar_prot             : 1   ; /* [26]  */
        HI_U32    desc_prot             : 1   ; /* [27]  */
        HI_U32    des_prot             : 1   ; /* [28]  */
        HI_U32    reserved_1            : 3  ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_MMU_PROT_CTRL;

/* Define the union U_DES_PIX_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_hoffset_pix       : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_PIX_OFFSET;

/* Define the union U_SRC1_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    hratio                : 24  ; /* [23..0]  */
        HI_U32    hfir_order            : 1   ; /* [24]  */
        HI_U32    hchfir_en             : 1   ; /* [25]  */
        HI_U32    hlfir_en              : 1   ; /* [26]  */
        HI_U32    hafir_en              : 1   ; /* [27]  */
        HI_U32    hchmid_en             : 1   ; /* [28]  */
        HI_U32    hlmid_en              : 1   ; /* [29]  */
        HI_U32    hchmsc_en             : 1   ; /* [30]  */
        HI_U32    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_HSP;

/* Define the union U_SRC1_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    hor_loffset           : 28  ; /* [27..0]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_HLOFFSET;

/* Define the union U_SRC1_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    hor_coffset           : 28  ; /* [27..0]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_HCOFFSET;

/* Define the union U_SRC1_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 19  ; /* [18..0]  */
        HI_U32    zme_in_fmt            : 2   ; /* [20..19]  */
        HI_U32    zme_out_fmt           : 2   ; /* [22..21]  */
        HI_U32    vchfir_en             : 1   ; /* [23]  */
        HI_U32    vlfir_en              : 1   ; /* [24]  */
        HI_U32    vafir_en              : 1   ; /* [25]  */
        HI_U32    vsc_chroma_tap        : 1   ; /* [26]  */
        HI_U32    reserved_1            : 1   ; /* [27]  */
        HI_U32    vchmid_en             : 1   ; /* [28]  */
        HI_U32    vlmid_en              : 1   ; /* [29]  */
        HI_U32    vchmsc_en             : 1   ; /* [30]  */
        HI_U32    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_VSP;

/* Define the union U_SRC1_VSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    vratio                : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_VSR;

/* Define the union U_SRC1_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    vchroma_offset        : 16  ; /* [15..0]  */
        HI_U32    vluma_offset          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_VOFFSET;

/* Define the union U_SRC1_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ow                    : 16  ; /* [15..0]  */
        HI_U32    oh                    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_ZMEORESO;

/* Define the union U_SRC1_ZMEIRESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    iw                    : 16  ; /* [15..0]  */
        HI_U32    ih                    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_ZMEIRESO;

/* Define the union U_SRC1_CROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_crop_x           : 13  ; /* [12..0]  */
        HI_U32    reserved_0            : 3   ; /* [15..13]  */
        HI_U32    src1_crop_y           : 13  ; /* [28..16]  */
        HI_U32    reserved_1            : 2   ; /* [30..29]  */
        HI_U32    src1_crop_en          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CROP_POS;

/* Define the union U_SRC1_CROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_crop_width       : 13  ; /* [12..0]  */
        HI_U32    reserved_0            : 3   ; /* [15..13]  */
        HI_U32    src1_crop_height      : 13  ; /* [28..16]  */
        HI_U32    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CROP_SIZE;

/* Define the union U_SRC1_HPZME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_hpzme_en         : 1   ; /* [0]  */
        HI_U32    reserved_0            : 3   ; /* [3..1]  */
        HI_U32    src1_hpzme_mode       : 4   ; /* [7..4]  */
        HI_U32    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_HPZME;

/* Define the union U_SRC1_HPZME_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src1_hpzme_width      : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_HPZME_SIZE;

/* Define the union U_SRC1_CSC_IDC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscidc0               : 9   ; /* [8..0]  */
        HI_U32    cscidc1               : 9   ; /* [17..9]  */
        HI_U32    cscidc2               : 9   ; /* [26..18]  */
        HI_U32    csc_en                : 1   ; /* [27]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_IDC;

/* Define the union U_SRC1_CSC_ODC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscodc0               : 9   ; /* [8..0]  */
        HI_U32    cscodc1               : 9   ; /* [17..9]  */
        HI_U32    cscodc2               : 9   ; /* [26..18]  */
        HI_U32    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_ODC;

/* Define the union U_SRC1_CSC_P0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp00                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp01                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_P0;

/* Define the union U_SRC1_CSC_P1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp02                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp10                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_P1;

/* Define the union U_SRC1_CSC_P2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp11                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp12                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_P2;

/* Define the union U_SRC1_CSC_P3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp20                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp21                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_P3;

/* Define the union U_SRC1_CSC_P4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp22                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CSC_P4;

/* Define the union U_SRC2_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    hratio                : 24  ; /* [23..0]  */
        HI_U32    hfir_order            : 1   ; /* [24]  */
        HI_U32    hchfir_en             : 1   ; /* [25]  */
        HI_U32    hlfir_en              : 1   ; /* [26]  */
        HI_U32    hafir_en              : 1   ; /* [27]  */
        HI_U32    hchmid_en             : 1   ; /* [28]  */
        HI_U32    hlmid_en              : 1   ; /* [29]  */
        HI_U32    hchmsc_en             : 1   ; /* [30]  */
        HI_U32    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_HSP;




/* Define the union U_SRC2_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_S32    hor_loffset           : 28  ; /* [27..0]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_HLOFFSET;

/* Define the union U_SRC2_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_S32    hor_coffset           : 28  ; /* [27..0]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_HCOFFSET;

/* Define the union U_SRC2_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 19  ; /* [18..0]  */
        HI_U32    zme_in_fmt            : 2   ; /* [20..19]  */
        HI_U32    zme_out_fmt           : 2   ; /* [22..21]  */
        HI_U32    vchfir_en             : 1   ; /* [23]  */
        HI_U32    vlfir_en              : 1   ; /* [24]  */
        HI_U32    vafir_en              : 1   ; /* [25]  */
        HI_U32    vsc_chroma_tap        : 1   ; /* [26]  */
        HI_U32    reserved_1            : 1   ; /* [27]  */
        HI_U32    vchmid_en             : 1   ; /* [28]  */
        HI_U32    vlmid_en              : 1   ; /* [29]  */
        HI_U32    vchmsc_en             : 1   ; /* [30]  */
        HI_U32    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_VSP;

/* Define the union U_SRC2_VSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    vratio                : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_VSR;

/* Define the union U_SRC2_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_S32    vchroma_offset        : 16  ; /* [15..0]  */
        HI_S32    vluma_offset          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_VOFFSET;

/* Define the union U_SRC2_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ow                    : 16  ; /* [15..0]  */
        HI_U32    oh                    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_ZMEORESO;

/* Define the union U_SRC2_ZMEIRESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    iw                    : 16  ; /* [15..0]  */
        HI_U32    ih                    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_ZMEIRESO;

/* Define the union U_SRC2_CROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_crop_x           : 13  ; /* [12..0]  */
        HI_U32    reserved_0            : 3   ; /* [15..13]  */
        HI_U32    src2_crop_y           : 13  ; /* [28..16]  */
        HI_U32    reserved_1            : 2   ; /* [30..29]  */
        HI_U32    src2_crop_en          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CROP_POS;

/* Define the union U_SRC2_CROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_crop_width       : 13  ; /* [12..0]  */
        HI_U32    reserved_0            : 3   ; /* [15..13]  */
        HI_U32    src2_crop_height      : 13  ; /* [28..16]  */
        HI_U32    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CROP_SIZE;

/* Define the union U_SRC2_HPZME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_hpzme_en         : 1   ; /* [0]  */
        HI_U32    reserved_0            : 3   ; /* [3..1]  */
        HI_U32    src2_hpzme_mode       : 4   ; /* [7..4]  */
        HI_U32    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_HPZME;

/* Define the union U_SRC2_HPZME_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_hpzme_width      : 16  ; /* [15..0]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_HPZME_SIZE;

/* Define the union U_SRC2_CSC_MUX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    src2_csc_mode         : 1   ; /* [0]  */
        HI_U32    des_premulten         : 1   ; /* [1]  */
        HI_U32    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CSC_MUX;

/* Define the union U_DES_CSC_IDC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscidc0               : 9   ; /* [8..0]  */
        HI_U32    cscidc1               : 9   ; /* [17..9]  */
        HI_U32    cscidc2               : 9   ; /* [26..18]  */
        HI_U32    csc_en                : 1   ; /* [27]  */
        HI_U32    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_IDC;

/* Define the union U_DES_CSC_ODC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscodc0               : 9   ; /* [8..0]  */
        HI_U32    cscodc1               : 9   ; /* [17..9]  */
        HI_U32    cscodc2               : 9   ; /* [26..18]  */
        HI_U32    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_ODC;

/* Define the union U_DES_CSC_P0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp00                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp01                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_P0;

/* Define the union U_DES_CSC_P1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp02                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp10                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_P1;

/* Define the union U_DES_CSC_P2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp11                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp12                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_P2;

/* Define the union U_DES_CSC_P3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp20                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 1   ; /* [15]  */
        HI_U32    cscp21                : 15  ; /* [30..16]  */
        HI_U32    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_P3;

/* Define the union U_DES_CSC_P4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    cscp22                : 15  ; /* [14..0]  */
        HI_U32    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_CSC_P4;

/* Define the union U_DES_DSWM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    des_h_dswm_mode       : 2   ; /* [1..0]  */
        HI_U32    reserved_0            : 2   ; /* [3..2]  */
        HI_U32    des_v_dswm_mode       : 1   ; /* [4]  */
        HI_U32    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_DES_DSWM;

/* Define the union U_SRC1_BEFOR_ZME_COPY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 8   ; /* [7..0]  */
        HI_U32    src1_rcopy_pixel_num  : 8   ; /* [15..8]  */
        HI_U32    reserved_1            : 14  ; /* [29..16]  */
        HI_U32    src1_rcopy_en         : 1   ; /* [30]  */
        HI_U32    src1_copy_cfg_from    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_BEFOR_ZME_COPY;

/* Define the union U_SRC1_AFTER_ZME_CROP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 8   ; /* [7..0]  */
        HI_U32    src1_rcrop_pixel_num  : 8   ; /* [15..8]  */
        HI_U32    reserved_1            : 14  ; /* [29..16]  */
        HI_U32    src1_rcrop_en         : 1   ; /* [30]  */
        HI_U32    src1_crop_cfg_from    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_AFTER_ZME_CROP;

/* Define the union U_SRC2_BEFOR_ZME_COPY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 8   ; /* [7..0]  */
        HI_U32    src2_rcopy_pixel_num  : 8   ; /* [15..8]  */
        HI_U32    reserved_1            : 14  ; /* [29..16]  */
        HI_U32    src2_rcopy_en         : 1   ; /* [30]  */
        HI_U32    src2_copy_cfg_from    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_BEFOR_ZME_COPY;

/* Define the union U_SRC2_AFTER_ZME_CROP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 8   ; /* [7..0]  */
        HI_U32    src2_rcrop_pixel_num  : 8   ; /* [15..8]  */
        HI_U32    reserved_1            : 14  ; /* [29..16]  */
        HI_U32    src2_rcrop_en         : 1   ; /* [30]  */
        HI_U32    src2_crop_cfg_from    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_AFTER_ZME_CROP;

/* Define the union U_CBMCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    mix_prio0             : 3   ; /* [2..0]  */
        HI_U32    mix_prio1             : 3   ; /* [5..3]  */
        HI_U32    mix_prio2             : 3   ; /* [8..6]  */
        HI_U32    mix_prio3             : 3   ; /* [11..9]  */
        HI_U32    mix_prio4             : 3   ; /* [14..12]  */
        HI_U32    mix_prio5             : 3   ; /* [17..15]  */
        HI_U32    mix_prio6             : 3   ; /* [20..18]  */
        HI_U32    alu_mode              : 4   ; /* [24..21]  */
        HI_U32    cbm_mode              : 1   ; /* [25]  */
        HI_U32    reserved_0            : 5   ; /* [30..26]  */
        HI_U32    cbm_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMCTRL;

/* Define the union U_CBMBKG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    bkgb                  : 8   ; /* [7..0]  */
        HI_U32    bkgg                  : 8   ; /* [15..8]  */
        HI_U32    bkgr                  : 8   ; /* [23..16]  */
        HI_U32    bkga                  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMBKG;

/* Define the union U_CBMCOLORIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    colorizeb             : 8   ; /* [7..0]  */
        HI_U32    colorizeg             : 8   ; /* [15..8]  */
        HI_U32    colorizer             : 8   ; /* [23..16]  */
        HI_U32    reserved_0            : 7   ; /* [30..24]  */
        HI_U32    colorizeen            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMCOLORIZE;

/* Define the union U_CBMALUPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    rgb_rop               : 4   ; /* [3..0]  */
        HI_U32    a_rop                 : 4   ; /* [7..4]  */
        HI_U32    reserved_0            : 8   ; /* [15..8]  */
        HI_U32    alpha_from            : 2   ; /* [17..16]  */
        HI_U32    alpha_border_en       : 2   ; /* [19..18]  */
        HI_U32    reserved_1            : 11  ; /* [30..20]  */
        HI_U32    blendropen            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMALUPARA;

/* Define the union U_CBMKEYPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    keybmode              : 2   ; /* [1..0]  */
        HI_U32    keygmode              : 2   ; /* [3..2]  */
        HI_U32    keyrmode              : 2   ; /* [5..4]  */
        HI_U32    keyamode              : 2   ; /* [7..6]  */
        HI_U32    keysel                : 2   ; /* [9..8]  */
        HI_U32    reserved_0            : 21  ; /* [30..10]  */
        HI_U32    keyen                 : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMKEYPARA;

/* Define the union U_CBMKEYMIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U8    keybmin               : 8   ; /* [7..0]  */
        HI_U8    keygmin               : 8   ; /* [15..8]  */
        HI_U8    keyrmin               : 8   ; /* [23..16]  */
        HI_U8    keyamin               : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMKEYMIN;

/* Define the union U_CBMKEYMAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U8    keybmax               : 8   ; /* [7..0]  */
        HI_U8    keygmax               : 8   ; /* [15..8]  */
        HI_U8    keyrmax               : 8   ; /* [23..16]  */
        HI_U8    keyamax               : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMKEYMAX;

/* Define the union U_CBMKEYMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U8   keybmask              : 8   ; /* [7..0]  */
        HI_U8    keygmask              : 8   ; /* [15..8]  */
        HI_U8    keyrmask              : 8   ; /* [23..16]  */
        HI_U8    keyamask              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_CBMKEYMASK;

/* Define the union U_SRC1_CBMPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    s1_galphaen           : 1   ; /* [0]  */
        HI_U32    s1_palphaen           : 1   ; /* [1]  */
        HI_U32    s1_premulten          : 1   ; /* [2]  */
        HI_U32    s1_multiglobalen      : 1   ; /* [3]  */
        HI_U32    s1_blendmode          : 4   ; /* [7..4]  */
        HI_U32    s1_galpha             : 8   ; /* [15..8]  */
        HI_U32    reserved_0            : 15  ; /* [30..16]  */
        HI_U32    s1_coverblenden       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CBMPARA;

/* Define the union U_SRC1_CBMSTPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    s1_xfpos              : 16  ; /* [15..0]  */
        HI_U32    s1_yfpos              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC1_CBMSTPOS;

/* Define the union U_SRC2_CBMPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    s2_galphaen           : 1   ; /* [0]  */
        HI_U32    s2_palphaen           : 1   ; /* [1]  */
        HI_U32    s2_premulten          : 1   ; /* [2]  */
        HI_U32    s2_multiglobalen      : 1   ; /* [3]  */
        HI_U32    s2_blendmode          : 4   ; /* [7..4]  */
        HI_U32    s2_galpha             : 8   ; /* [15..8]  */
        HI_U32    reserved_0            : 15  ; /* [30..16]  */
        HI_U32    s2_coverblenden       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CBMPARA;

/* Define the union U_SRC2_CBMSTPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    s2_xfpos              : 16  ; /* [15..0]  */
        HI_U32    s2_yfpos              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_SRC2_CBMSTPOS;



/* Define the union U_TDE_SRC1_ZME_LHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_scl_lh            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC1_ZME_LHADDR;
/* Define the union U_TDE_SRC1_ZME_LVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_scl_lv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC1_ZME_LVADDR;
/* Define the union U_TDE_SRC1_ZME_CHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_scl_ch            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC1_ZME_CHADDR;
/* Define the union U_TDE_SRC1_ZME_CVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src1_scl_cv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC1_ZME_CVADDR;
/* Define the union U_TDE_SRC2_ZME_LHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_scl_lh            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC2_ZME_LHADDR;
/* Define the union U_TDE_SRC2_ZME_LVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_scl_lv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC2_ZME_LVADDR;
/* Define the union U_TDE_SRC2_ZME_CHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_scl_ch            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC2_ZME_CHADDR;
/* Define the union U_TDE_SRC2_ZME_CVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 src2_scl_cv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_SRC2_ZME_CVADDR;
/* Define the union U_TDE_CLUT_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 clut_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_CLUT_ADDR;
/* Define the union U_TDE_AXIID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    reserved_0            : 20  ; /* [19..0]  */
        HI_U32    awid_cfg0             : 4   ; /* [23..20]  */
        HI_U32    reserved_1            : 4   ; /* [27..24]  */
        HI_U32    arid_cfg0             : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_AXIID;

/* Define the union U_TDE_NODEID */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 nodeid                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_NODEID;
/* Define the union U_TDE_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    eof_mask              : 1   ; /* [0]  */
        HI_U32    timeout_mask          : 1   ; /* [1]  */
        HI_U32    bus_err_mask          : 1   ; /* [2]  */
        HI_U32    eof_end_mask          : 1   ; /* [3]  */
        HI_U32    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_INTMASK;

/* Define the union U_TDE_PNEXT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 p_next                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32All;

} U_TDE_PNEXT;
/* Define the union U_TDE_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    start                 : 1   ; /* [0]  */
        HI_U32    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_START;

/* Define the union U_TDE_INTSTATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    eof_state             : 1   ; /* [0]  */
        HI_U32    timeout_state         : 1   ; /* [1]  */
        HI_U32    bus_err               : 1   ; /* [2]  */
        HI_U32    eof_end_state         : 1   ; /* [3]  */
        HI_U32    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_INTSTATE;

/* Define the union U_TDE_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    eof_clr               : 1   ; /* [0]  */
        HI_U32    timeout_clr           : 1   ; /* [1]  */
        HI_U32    bus_err_clr           : 1   ; /* [2]  */
        HI_U32    eof_end_clr           : 1   ; /* [3]  */
        HI_U32    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_INTCLR;

/* Define the union U_TDE_RAWINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    raw_eof               : 1   ; /* [0]  */
        HI_U32    raw_timeout           : 1   ; /* [1]  */
        HI_U32    raw_bus_err           : 1   ; /* [2]  */
        HI_U32    raw_eof_end           : 1   ; /* [3]  */
        HI_U32    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32All;

} U_TDE_RAWINT;

/* Define the union U_TDE_PFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 pfcnt                  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_PFCNT;
/* Define the union U_TDE_MISCELLANEOUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    routstanding          : 4   ; /* [3..0]  */
        HI_U32    woutstanding          : 4   ; /* [7..4]  */
        HI_U32    init_timer            : 16  ; /* [23..8]  */
        HI_U32    ck_gt_en              : 1   ; /* [24]  */
        HI_U32    ck_gt_en_calc         : 1   ; /* [25]  */
        HI_U32    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_TDE_MISCELLANEOUS;

/* Define the union U_TDE_MACCFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 mac_ch_prio            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_MACCFG;
/* Define the union U_TDE_TIMEOUT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 timeout                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_TIMEOUT;
/* Define the union U_TDE_EOFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 eof_cnt                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_EOFCNT;
/* Define the union U_TDE_MEMCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    emab                  : 3   ; /* [2..0]  */
        HI_U32    reserved_0            : 1   ; /* [3]  */
        HI_U32    emaa                  : 3   ; /* [6..4]  */
        HI_U32    reserved_1            : 1   ; /* [7]  */
        HI_U32    emaw                  : 2   ; /* [9..8]  */
        HI_U32    reserved_2            : 2   ; /* [11..10]  */
        HI_U32    ema                   : 3   ; /* [14..12]  */
        HI_U32    reserved_3            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_TDE_MEMCTRL;

/* Define the union U_TDE_DEBUG0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug0                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG0;
/* Define the union U_TDE_DEBUG1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug1                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG1;
/* Define the union U_TDE_DEBUG2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug2                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG2;
/* Define the union U_TDE_DEBUG3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug3                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG3;
/* Define the union U_TDE_DEBUG4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug4                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG4;
/* Define the union U_TDE_DEBUG5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug5                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG5;
/* Define the union U_TDE_DEBUG6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug6                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG6;
/* Define the union U_TDE_DEBUG7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug7                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG7;
/* Define the union U_TDE_DEBUG8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug8                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG8;
/* Define the union U_TDE_DEBUG9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug9                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG9;
/* Define the union U_TDE_DEBUG10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug10                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG10;
/* Define the union U_TDE_DEBUG11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 debug11                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_TDE_DEBUG11;
#if 0
/* Define the union U_SMMU_SCR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    glb_bypass            : 1   ; /* [0]  */
        HI_U32    reserved_0            : 2   ; /* [2..1]  */
        HI_U32    int_en                : 1   ; /* [3]  */
        HI_U32    reserved_1            : 12  ; /* [15..4]  */
        HI_U32    ptw_pf                : 4   ; /* [19..16]  */
        HI_U32    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_SCR;

/* Define the union U_SMMU_LP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    auto_clk_gt_en        : 1   ; /* [0]  */
        HI_U32    reserved_0            : 1   ; /* [1]  */
        HI_U32    smmu_idle             : 1   ; /* [2]  */
        HI_U32    reserved_1            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_LP_CTRL;

/* Define the union U_SMMU_INTMASK_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ints_tlbmiss_msk      : 1   ; /* [0]  */
        HI_U32    ints_ptw_trans_msk    : 1   ; /* [1]  */
        HI_U32    ints_tlbinvalid_msk   : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTMASK_S;

/* Define the union U_SMMU_INTRAW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ints_tlbmiss_raw      : 1   ; /* [0]  */
        HI_U32    ints_ptw_trans_raw    : 1   ; /* [1]  */
        HI_U32    ints_tlbinvalid_raw   : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTRAW_S;

/* Define the union U_SMMU_INTSTAT_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ints_tlbmiss_stat     : 1   ; /* [0]  */
        HI_U32    ints_ptw_trans_stat   : 1   ; /* [1]  */
        HI_U32    ints_tlbinvalid_stat  : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTSTAT_S;

/* Define the union U_SMMU_INTCLR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ints_tlbmiss_clr      : 1   ; /* [0]  */
        HI_U32    ints_ptw_trans_clr    : 1   ; /* [1]  */
        HI_U32    ints_tlbinvalid_clr   : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTCLR_S;

/* Define the union U_SMMU_INTMASK_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    intns_tlbmiss_msk     : 1   ; /* [0]  */
        HI_U32    intns_ptw_trans_msk   : 1   ; /* [1]  */
        HI_U32    intns_tlbinvalid_msk  : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTMASK_NS;

/* Define the union U_SMMU_INTRAW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    intns_tlbmiss_raw     : 1   ; /* [0]  */
        HI_U32    intns_ptw_trans_raw   : 1   ; /* [1]  */
        HI_U32    intns_tlbinvalid_raw  : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTRAW_NS;

/* Define the union U_SMMU_INTSTAT_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    intns_tlbmiss_stat    : 1   ; /* [0]  */
        HI_U32    intns_ptw_trans_stat  : 1   ; /* [1]  */
        HI_U32    intns_tlbinvalid_stat : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTSTAT_NS;

/* Define the union U_SMMU_INTCLR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    intns_tlbmiss_clr     : 1   ; /* [0]  */
        HI_U32    intns_ptw_trans_clr   : 1   ; /* [1]  */
        HI_U32    intns_tlbinvalid_clr  : 1   ; /* [2]  */
        HI_U32    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_INTCLR_NS;

/* Define the union U_SMMU_SCB_TTBR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 scb_ttbr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_SCB_TTBR;
/* Define the union U_SMMU_CB_TTBR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 cb_ttbr                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_CB_TTBR;
/* Define the union U_SMMU_ERR_RDADDR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 err_s_rd_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_ERR_RDADDR_S;
/* Define the union U_SMMU_ERR_WRADDR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 err_s_wr_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_ERR_WRADDR_S;
/* Define the union U_SMMU_ERR_RDADDR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 err_ns_rd_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_ERR_RDADDR_NS;
/* Define the union U_SMMU_ERR_WRADDR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 err_ns_wr_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_ERR_WRADDR_NS;
/* Define the union U_SMMU_FAULT_ADDR_PTW_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_ptw_s       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_PTW_S;
/* Define the union U_SMMU_FAULT_ID_PTW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_s      : 16  ; /* [15..0]  */
        HI_U32    fault_strm_id_s       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_PTW_S;

/* Define the union U_SMMU_FAULT_ADDR_PTW_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_ptw_ns      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_PTW_NS;
/* Define the union U_SMMU_FAULT_ID_PTW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_ns     : 16  ; /* [15..0]  */
        HI_U32    fault_strm_id_ns      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_PTW_NS;

/* Define the union U_SMMU_FAULT_PTW_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_s_ptw_num       : 4   ; /* [3..0]  */
        HI_U32    fault_ns_ptw_num      : 4   ; /* [7..4]  */
        HI_U32    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_PTW_NUM;

/* Define the union U_SMMU_FAULT_ADDR_WR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_wr_s        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_WR_S;
/* Define the union U_SMMU_FAULT_TLB_WR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_tlb_wr_s         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_TLB_WR_S;
/* Define the union U_SMMU_FAULT_ID_WR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_wr_s   : 16  ; /* [15..0]  */
        HI_U32    fault_str_id_wr_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_WR_S;

/* Define the union U_SMMU_FAULT_ADDR_WR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_wr_ns       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_WR_NS;
/* Define the union U_SMMU_FAULT_TLB_WR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_tlb_wr_ns        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_TLB_WR_NS;
/* Define the union U_SMMU_FAULT_ID_WR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_wr_ns  : 16  ; /* [15..0]  */
        HI_U32    fault_str_id_wr_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_WR_NS;

/* Define the union U_SMMU_FAULT_ADDR_RD_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_rd_s        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_RD_S;
/* Define the union U_SMMU_FAULT_TLB_RD_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_tlb_rd_s         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_TLB_RD_S;
/* Define the union U_SMMU_FAULT_ID_RD_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_rd_s   : 16  ; /* [15..0]  */
        HI_U32    fault_str_id_rd_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_RD_S;

/* Define the union U_SMMU_FAULT_ADDR_RD_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_addr_rd_ns       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_ADDR_RD_NS;
/* Define the union U_SMMU_FAULT_TLB_RD_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        HI_U32 fault_tlb_rd_ns        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        HI_U32    u32;

} U_SMMU_FAULT_TLB_RD_NS;
/* Define the union U_SMMU_FAULT_ID_RD_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_index_id_rd_ns  : 16  ; /* [15..0]  */
        HI_U32    fault_str_id_rd_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_ID_RD_NS;

/* Define the union U_SMMU_FAULT_TBU_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    fault_tlbmiss_err_s   : 2   ; /* [1..0]  */
        HI_U32    fault_tlbinvalid_err_s : 2   ; /* [3..2]  */
        HI_U32    fault_tlbmiss_err_ns  : 2   ; /* [5..4]  */
        HI_U32    fault_tlbinvalid_err_ns : 2   ; /* [7..6]  */
        HI_U32    fault_tbu_num         : 4   ; /* [11..8]  */
        HI_U32    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_TBU_INFO;

/* Define the union U_SMMU_FAULT_TBU_DBG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    aw_valids             : 1   ; /* [0]  */
        HI_U32    aw_readys             : 1   ; /* [1]  */
        HI_U32    tbu_awreadym          : 1   ; /* [2]  */
        HI_U32    awch_stall_n          : 1   ; /* [3]  */
        HI_U32    write_command_counter : 2   ; /* [5..4]  */
        HI_U32    ar_valids             : 1   ; /* [6]  */
        HI_U32    ar_readys             : 1   ; /* [7]  */
        HI_U32    tbu_arreadym          : 1   ; /* [8]  */
        HI_U32    arch_stall_n          : 1   ; /* [9]  */
        HI_U32    read_command_counter  : 2   ; /* [11..10]  */
        HI_U32    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_FAULT_TBU_DBG;

/* Define the union U_SMMU_PREF_BUFFER_EMPTY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    pref_buffer_empty     : 1   ; /* [0]  */
        HI_U32    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_PREF_BUFFER_EMPTY;

/* Define the union U_SMMU_PTWQ_IDLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    ptwq0_idle_state      : 1   ; /* [0]  */
        HI_U32    ptwq1_idle_state      : 1   ; /* [1]  */
        HI_U32    ptwq2_idle_state      : 1   ; /* [2]  */
        HI_U32    ptwq3_idle_state      : 1   ; /* [3]  */
        HI_U32    ptwq4_idle_state      : 1   ; /* [4]  */
        HI_U32    ptwq5_idle_state      : 1   ; /* [5]  */
        HI_U32    ptwq6_idle_state      : 1   ; /* [6]  */
        HI_U32    ptwq7_idle_state      : 1   ; /* [7]  */
        HI_U32    ptwq8_idle_state      : 1   ; /* [8]  */
        HI_U32    ptwq9_idle_state      : 1   ; /* [9]  */
        HI_U32    ptwq10_idle_state     : 1   ; /* [10]  */
        HI_U32    ptwq11_idle_state     : 1   ; /* [11]  */
        HI_U32    ptwq12_idle_state     : 1   ; /* [12]  */
        HI_U32    ptwq13_idle_state     : 1   ; /* [13]  */
        HI_U32    ptwq14_idle_state     : 1   ; /* [14]  */
        HI_U32    ptwq15_idle_state     : 1   ; /* [15]  */
        HI_U32    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_PTWQ_IDLE;

/* Define the union U_SMMU_RESET_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    smmu_rst_state        : 1   ; /* [0]  */
        HI_U32    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_RESET_STATE;

/* Define the union U_SMMU_MASTER_DBG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_miss_cnt_rd      : 10  ; /* [9..0]  */
        HI_U32    cur_miss_cnt_rd       : 10  ; /* [19..10]  */
        HI_U32    vld_debug_rd          : 4   ; /* [23..20]  */
        HI_U32    rdy_debug_rd          : 4   ; /* [27..24]  */
        HI_U32    in_out_cmd_cnt_rd     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG0;

/* Define the union U_SMMU_MASTER_DBG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_miss_cnt_wr      : 10  ; /* [9..0]  */
        HI_U32    cur_miss_cnt_wr       : 10  ; /* [19..10]  */
        HI_U32    vld_debug_wr          : 4   ; /* [23..20]  */
        HI_U32    rdy_debug_wr          : 4   ; /* [27..24]  */
        HI_U32    in_out_cmd_cnt_wr     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG1;

/* Define the union U_SMMU_MASTER_DBG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_double_miss_cnt_rd : 8   ; /* [7..0]  */
        HI_U32    cur_double_miss_cnt_rd : 8   ; /* [15..8]  */
        HI_U32    last_double_upd_cnt_rd : 8   ; /* [23..16]  */
        HI_U32    cur_double_upd_cnt_rd : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG2;

/* Define the union U_SMMU_MASTER_DBG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_double_miss_cnt_wr : 8   ; /* [7..0]  */
        HI_U32    cur_double_miss_cnt_wr : 8   ; /* [15..8]  */
        HI_U32    last_double_upd_cnt_wr : 7   ; /* [22..16]  */
        HI_U32    cur_double_upd_cnt_wr : 7   ; /* [29..23]  */
        HI_U32    mst_fsm_cur           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG3;

/* Define the union U_SMMU_MASTER_DBG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_sel1_chn_miss_cnt_rd : 16  ; /* [15..0]  */
        HI_U32    cur_sel1_chn_miss_cnt_rd : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG4;

/* Define the union U_SMMU_MASTER_DBG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_sel2_chn_miss_cnt_rd : 16  ; /* [15..0]  */
        HI_U32    cur_sel2_chn_miss_cnt_rd : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG5;

/* Define the union U_SMMU_MASTER_DBG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_sel1_chn_miss_cnt_wr : 16  ; /* [15..0]  */
        HI_U32    cur_sel1_chn_miss_cnt_wr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG6;

/* Define the union U_SMMU_MASTER_DBG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    last_sel2_chn_miss_cnt_wr : 16  ; /* [15..0]  */
        HI_U32    cur_sel2_chn_miss_cnt_wr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG7;

/* Define the union U_SMMU_MASTER_DBG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        HI_U32    sel2_chn_wr           : 8   ; /* [7..0]  */
        HI_U32    sel1_chn_wr           : 8   ; /* [15..8]  */
        HI_U32    sel2_chn_rd           : 8   ; /* [23..16]  */
        HI_U32    sel1_chn_rd           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    HI_U32    u32;

} U_SMMU_MASTER_DBG8;
#endif


typedef struct hiTDE_HWNode_S
{
    U_SRC1_CTRL            SRC1_CTRL                         ; /* 0x0 */
    U_SRC1_CH0_ADDR        SRC1_CH0_ADDR                     ; /* 0x4 */
    U_SRC1_CH1_ADDR        SRC1_CH1_ADDR                     ; /* 0x8 */
    U_SRC1_CH0_STRIDE      SRC1_CH0_STRIDE                   ; /* 0xc */
    U_SRC1_CH1_STRIDE      SRC1_CH1_STRIDE                   ; /* 0x10 */
    U_SRC1_IMGSIZE         SRC1_IMGSIZE                      ; /* 0x14 */
    U_SRC1_FILL            SRC1_FILL                         ; /* 0x18 */
    U_SRC1_ALPHA           SRC1_ALPHA                        ; /* 0x1c */
    U_SRC1_PIX_OFFSET      SRC1_PIX_OFFSET                   ; /* 0x20 */
    HI_U32           reserved_0[7]                     ; /* 0x24~0x3c */
    U_SRC2_CTRL            SRC2_CTRL                         ; /* 0x40 */
    U_SRC2_CH0_ADDR        SRC2_CH0_ADDR                     ; /* 0x44 */
    U_SRC2_CH1_ADDR        SRC2_CH1_ADDR                     ; /* 0x48 */
    U_SRC2_CH0_STRIDE      SRC2_CH0_STRIDE                   ; /* 0x4c */
    U_SRC2_CH1_STRIDE      SRC2_CH1_STRIDE                   ; /* 0x50 */
    U_SRC2_IMGSIZE         SRC2_IMGSIZE                      ; /* 0x54 */
    U_SRC2_FILL            SRC2_FILL                         ; /* 0x58 */
    U_SRC2_ALPHA           SRC2_ALPHA                        ; /* 0x5c */
    U_SRC2_PIX_OFFSET      SRC2_PIX_OFFSET                   ; /* 0x60 */
    HI_U32           reserved_1[7]                     ; /* 0x64~0x7c */
    HI_U32            GDC3_CTRL                         ; /* 0x80 */
    HI_U32            GDC3_ADDR                         ; /* 0x84 */
    U_DES_SUR_XY    DES_SUR_XY;
    HI_U32           reserved_2[1]                     ; /* 0x88~0x8c */
    HI_U32          GDC3_STRIDE                       ; /* 0x90 */
    HI_U32         GDC3_IMGSIZE                      ; /* 0x94 */
    HI_U32            GDC3_FILL                         ; /* 0x98 */
    HI_U32           GDC3_ALPHA                        ; /* 0x9c */
    HI_U32      GDC3_PIX_OFFSET                   ; /* 0xa0 */
    HI_U32           reserved_3[7]                     ; /* 0xa4~0xbc */
    HI_U32            GDC4_CTRL                         ; /* 0xc0 */
    HI_U32            GDC4_ADDR                         ; /* 0xc4 */
    HI_U32           reserved_4[2]                     ; /* 0xc8~0xcc */
    HI_U32          GDC4_STRIDE                       ; /* 0xd0 */
    HI_U32         GDC4_IMGSIZE                      ; /* 0xd4 */
    HI_U32            GDC4_FILL                         ; /* 0xd8 */
    HI_U32           GDC4_ALPHA                        ; /* 0xdc */
    HI_U32      GDC4_PIX_OFFSET                   ; /* 0xe0 */
    HI_U32           reserved_5[7]                     ; /* 0xe4~0xfc */
    HI_U32            GDC5_CTRL                         ; /* 0x100 */
    HI_U32            GDC5_ADDR                         ; /* 0x104 */
    HI_U32           reserved_6[2]                     ; /* 0x108~0x10c */
    HI_U32          GDC5_STRIDE                       ; /* 0x110 */
    HI_U32         GDC5_IMGSIZE                      ; /* 0x114 */
    HI_U32            GDC5_FILL                         ; /* 0x118 */
    HI_U32           GDC5_ALPHA                        ; /* 0x11c */
    HI_U32      GDC5_PIX_OFFSET                   ; /* 0x120 */
    HI_U32           reserved_7[7]                     ; /* 0x124~0x13c */
    HI_U32            GDC6_CTRL                         ; /* 0x140 */
    HI_U32            GDC6_ADDR                         ; /* 0x144 */
    HI_U32           reserved_8[2]                     ; /* 0x148~0x14c */
    HI_U32          GDC6_STRIDE                       ; /* 0x150 */
    HI_U32         GDC6_IMGSIZE                      ; /* 0x154 */
    HI_U32            GDC6_FILL                         ; /* 0x158 */
    HI_U32           GDC6_ALPHA                        ; /* 0x15c */
    HI_U32      GDC6_PIX_OFFSET                   ; /* 0x160 */
    HI_U32           reserved_9[7]                     ; /* 0x164~0x17c */
    HI_U32            GDC7_CTRL                         ; /* 0x180 */
    HI_U32            GDC7_ADDR                         ; /* 0x184 */
    HI_U32           reserved_10[2]                     ; /* 0x188~0x18c */
    HI_U32          GDC7_STRIDE                       ; /* 0x190 */
    HI_U32         GDC7_IMGSIZE                      ; /* 0x194 */
    HI_U32            GDC7_FILL                         ; /* 0x198 */
    HI_U32           GDC7_ALPHA                        ; /* 0x19c */
    HI_U32      GDC7_PIX_OFFSET                   ; /* 0x1a0 */
    HI_U32           reserved_11[7]                     ; /* 0x1a4~0x1bc */
    U_DES_CTRL             DES_CTRL                          ; /* 0x1c0 */
    U_DES_CH0_ADDR         DES_CH0_ADDR                      ; /* 0x1c4 */
    U_DES_CH1_ADDR         DES_CH1_ADDR                      ; /* 0x1c8 */
    U_DES_CH0_STRIDE       DES_CH0_STRIDE                    ; /* 0x1cc */
    U_DES_CH1_STRIDE       DES_CH1_STRIDE                    ; /* 0x1d0 */
    U_DES_IMGSIZE          DES_IMGSIZE                       ; /* 0x1d4 */
    U_DES_ALPHA            DES_ALPHA                         ; /* 0x1d8 */
    U_DES_CROP_POS_ST      DES_CROP_POS_ST                   ; /* 0x1dc */
    U_DES_CROP_POS_ED      DES_CROP_POS_ED                   ; /* 0x1e0 */
    U_MMU_PROT_CTRL        MMU_PROT_CTRL                     ; /* 0x1e4 */
    U_DES_PIX_OFFSET       DES_PIX_OFFSET                    ; /* 0x1e8 */
    HI_U32           reserved_12[5]                     ; /* 0x1ec~0x1fc */
    U_SRC1_HSP             SRC1_HSP                          ; /* 0x200 */
    HI_S32        SRC1_HLOFFSET                     ; /* 0x204 */
    HI_S32        SRC1_HCOFFSET                     ; /* 0x208 */
    U_SRC1_VSP             SRC1_VSP                          ; /* 0x20c */
    U_SRC1_VSR             SRC1_VSR                          ; /* 0x210 */
    U_SRC1_VOFFSET         SRC1_VOFFSET                      ; /* 0x214 */
    U_SRC1_ZMEORESO        SRC1_ZMEORESO                     ; /* 0x218 */
    U_SRC1_ZMEIRESO        SRC1_ZMEIRESO                     ; /* 0x21c */
    U_SRC1_CROP_POS        SRC1_CROP_POS                     ; /* 0x220 */
    U_SRC1_CROP_SIZE       SRC1_CROP_SIZE                    ; /* 0x224 */
    U_SRC1_HPZME           SRC1_HPZME                        ; /* 0x228 */
    U_SRC1_HPZME_SIZE      SRC1_HPZME_SIZE                   ; /* 0x22c */
    U_SRC1_CSC_IDC         SRC1_CSC_IDC                      ; /* 0x230 */
    U_SRC1_CSC_ODC         SRC1_CSC_ODC                      ; /* 0x234 */
    U_SRC1_CSC_P0          SRC1_CSC_P0                       ; /* 0x238 */
    U_SRC1_CSC_P1          SRC1_CSC_P1                       ; /* 0x23c */
    U_SRC1_CSC_P2          SRC1_CSC_P2                       ; /* 0x240 */
    U_SRC1_CSC_P3          SRC1_CSC_P3                       ; /* 0x244 */
    U_SRC1_CSC_P4          SRC1_CSC_P4                       ; /* 0x248 */
    HI_U32           reserved_13[13]                     ; /* 0x24c~0x27c */
    U_SRC2_HSP             SRC2_HSP                          ; /* 0x280 */
    HI_S32        SRC2_HLOFFSET                     ; /* 0x284 */
    HI_S32        SRC2_HCOFFSET                     ; /* 0x288 */
    U_SRC2_VSP             SRC2_VSP                          ; /* 0x28c */
    U_SRC2_VSR             SRC2_VSR                          ; /* 0x290 */
    U_SRC2_VOFFSET         SRC2_VOFFSET                      ; /* 0x294 */
    U_SRC2_ZMEORESO        SRC2_ZMEORESO                     ; /* 0x298 */
    U_SRC2_ZMEIRESO        SRC2_ZMEIRESO                     ; /* 0x29c */
    U_SRC2_CROP_POS        SRC2_CROP_POS                     ; /* 0x2a0 */
    U_SRC2_CROP_SIZE       SRC2_CROP_SIZE                    ; /* 0x2a4 */
    U_SRC2_HPZME           SRC2_HPZME                        ; /* 0x2a8 */
    U_SRC2_HPZME_SIZE      SRC2_HPZME_SIZE                   ; /* 0x2ac */
    U_SRC2_CSC_MUX         SRC2_CSC_MUX                      ; /* 0x2b0 */
    HI_U32           reserved_14[3]                     ; /* 0x2b4~0x2bc */
    U_DES_CSC_IDC          DES_CSC_IDC                       ; /* 0x2c0 */
    U_DES_CSC_ODC          DES_CSC_ODC                       ; /* 0x2c4 */
    U_DES_CSC_P0           DES_CSC_P0                        ; /* 0x2c8 */
    U_DES_CSC_P1           DES_CSC_P1                        ; /* 0x2cc */
    U_DES_CSC_P2           DES_CSC_P2                        ; /* 0x2d0 */
    U_DES_CSC_P3           DES_CSC_P3                        ; /* 0x2d4 */
    U_DES_CSC_P4           DES_CSC_P4                        ; /* 0x2d8 */
    U_DES_DSWM             DES_DSWM                          ; /* 0x2dc */
    HI_U32   SRC1_BEFOR_ZME_COPY              ; /* 0x2e0 */
    HI_U32   SRC1_AFTER_ZME_CROP              ; /* 0x2e4 */
    HI_U32   SRC2_BEFOR_ZME_COPY              ; /* 0x2e8 */
    HI_U32   SRC2_AFTER_ZME_CROP              ; /* 0x2ec */
    HI_U32           reserved_15[4]                     ; /* 0x2f0~0x2fc */
    U_CBMCTRL              CBMCTRL                           ; /* 0x300 */
    U_CBMBKG               CBMBKG                            ; /* 0x304 */
    U_CBMCOLORIZE          CBMCOLORIZE                       ; /* 0x308 */
    U_CBMALUPARA           CBMALUPARA                        ; /* 0x30c */
    U_CBMKEYPARA           CBMKEYPARA                        ; /* 0x310 */
    U_CBMKEYMIN            CBMKEYMIN                         ; /* 0x314 */
    U_CBMKEYMAX            CBMKEYMAX                         ; /* 0x318 */
    U_CBMKEYMASK           CBMKEYMASK                        ; /* 0x31c */
    U_SRC1_CBMPARA         SRC1_CBMPARA                      ; /* 0x320 */
    HI_U32        SRC1_CBMSTPOS                     ; /* 0x324 */
    U_SRC2_CBMPARA         SRC2_CBMPARA                      ; /* 0x328 */
    HI_U32        SRC2_CBMSTPOS                     ; /* 0x32c */
    HI_U32         GDC3_CBMPARA                      ; /* 0x330 */
    HI_U32        GDC3_CBMSTPOS                     ; /* 0x334 */
    HI_U32         GDC4_CBMPARA                      ; /* 0x338 */
    HI_U32        GDC4_CBMSTPOS                     ; /* 0x33c */
    HI_U32         GDC5_CBMPARA                      ; /* 0x340 */
    HI_U32        GDC5_CBMSTPOS                     ; /* 0x344 */
    HI_U32         GDC6_CBMPARA                      ; /* 0x348 */
    HI_U32        GDC6_CBMSTPOS                     ; /* 0x34c */
    HI_U32         GDC7_CBMPARA                      ; /* 0x350 */
    HI_U32        GDC7_CBMSTPOS                     ; /* 0x354 */
    HI_U32           reserved_16[26]                     ; /* 0x358~0x3bc */
    HI_U32      OUTCMP_GLB_INFO                   ; /* 0x3c0 */
    HI_U32    OUTCMP_FRAME_SIZE                 ; /* 0x3c4 */
    HI_U32       OUTCMP_RC_CFG0                    ; /* 0x3c8 */
    HI_U32       OUTCMP_RC_CFG1                    ; /* 0x3cc */
    HI_U32       OUTCMP_RC_CFG2                    ; /* 0x3d0 */
    HI_U32       OUTCMP_RC_CFG3                    ; /* 0x3d4 */
    HI_U32       OUTCMP_RC_CFG4                    ; /* 0x3d8 */
    HI_U32       OUTCMP_RC_CFG5                    ; /* 0x3dc */
    HI_U32       OUTCMP_RC_CFG6                    ; /* 0x3e0 */
    HI_U32       OUTCMP_RC_CFG7                    ; /* 0x3e4 */
    HI_U32       OUTCMP_RC_CFG8                    ; /* 0x3e8 */
    HI_U32       OUTCMP_RC_CFG9                    ; /* 0x3ec */
    HI_U32   OUTCMP_MAX_ROW_LEN                ; /* 0x3f0 */
    HI_U32           reserved_17[35]                     ; /* 0x3f4~0x47c */
    HI_U32   TDE_SRC1_ZME_LHADDR              ; /* 0x480 */
    HI_U32   TDE_SRC1_ZME_LVADDR              ; /* 0x484 */
    HI_U32   TDE_SRC1_ZME_CHADDR              ; /* 0x488 */
    HI_U32   TDE_SRC1_ZME_CVADDR              ; /* 0x48c */
    HI_U32   TDE_SRC2_ZME_LHADDR              ; /* 0x490 */
    HI_U32   TDE_SRC2_ZME_LVADDR              ; /* 0x494 */
    HI_U32   TDE_SRC2_ZME_CHADDR              ; /* 0x498 */
    HI_U32   TDE_SRC2_ZME_CVADDR              ; /* 0x49c */
    HI_U32        TDE_CLUT_ADDR                     ; /* 0x4a0 */
    HI_U32           reserved_18[19]                     ; /* 0x4a4~0x4ec */
    HI_U32            TDE_AXIID                         ; /* 0x4f0 */
    HI_U32           TDE_NODEID                        ; /* 0x4f4 */
    U_TDE_INTMASK          TDE_INTMASK                       ; /* 0x4f8 */
    #if 0
    U_TDE_PNEXT            TDE_PNEXT                         ; /* 0x4fc */
    U_TDE_START            TDE_START                         ; /* 0x500 */
    U_TDE_INTSTATE         TDE_INTSTATE                      ; /* 0x504 */
    U_TDE_INTCLR           TDE_INTCLR                        ; /* 0x508 */
    U_TDE_RAWINT           TDE_RAWINT                        ; /* 0x50c */
    U_TDE_PFCNT            TDE_PFCNT                         ; /* 0x510 */
    U_TDE_MISCELLANEOUS    TDE_MISCELLANEOUS                 ; /* 0x514 */
    U_TDE_MACCFG           TDE_MACCFG                        ; /* 0x518 */
    U_TDE_TIMEOUT          TDE_TIMEOUT                       ; /* 0x51c */
    U_TDE_EOFCNT           TDE_EOFCNT                        ; /* 0x520 */
    U_TDE_MEMCTRL          TDE_MEMCTRL                       ; /* 0x524 */
    HI_U32           reserved_19[3766]                     ; /* 0x528~0x3ffc */
    U_TDE_DEBUG0           TDE_DEBUG0                        ; /* 0x4000 */
    U_TDE_DEBUG1           TDE_DEBUG1                        ; /* 0x4004 */
    U_TDE_DEBUG2           TDE_DEBUG2                        ; /* 0x4008 */
    U_TDE_DEBUG3           TDE_DEBUG3                        ; /* 0x400c */
    U_TDE_DEBUG4           TDE_DEBUG4                        ; /* 0x4010 */
    U_TDE_DEBUG5           TDE_DEBUG5                        ; /* 0x4014 */
    U_TDE_DEBUG6           TDE_DEBUG6                        ; /* 0x4018 */
    U_TDE_DEBUG7           TDE_DEBUG7                        ; /* 0x401c */
    U_TDE_DEBUG8           TDE_DEBUG8                        ; /* 0x4020 */
    U_TDE_DEBUG9           TDE_DEBUG9                        ; /* 0x4024 */
    U_TDE_DEBUG10          TDE_DEBUG10                       ; /* 0x4028 */
    U_TDE_DEBUG11          TDE_DEBUG11                       ; /* 0x402c */
    HI_U32           reserved_20[11252]                     ; /* 0x4030~0xeffc */

    U_SMMU_SCR             SMMU_SCR                          ; /* 0xf000 */
    HI_U32           reserved_21                     ; /* 0xf004 */
    U_SMMU_LP_CTRL         SMMU_LP_CTRL                      ; /* 0xf008 */
    HI_U32           reserved_22                     ; /* 0xf00c */
    U_SMMU_INTMASK_S       SMMU_INTMASK_S                    ; /* 0xf010 */
    U_SMMU_INTRAW_S        SMMU_INTRAW_S                     ; /* 0xf014 */
    U_SMMU_INTSTAT_S       SMMU_INTSTAT_S                    ; /* 0xf018 */
    U_SMMU_INTCLR_S        SMMU_INTCLR_S                     ; /* 0xf01c */
    U_SMMU_INTMASK_NS      SMMU_INTMASK_NS                   ; /* 0xf020 */
    U_SMMU_INTRAW_NS       SMMU_INTRAW_NS                    ; /* 0xf024 */
    U_SMMU_INTSTAT_NS      SMMU_INTSTAT_NS                   ; /* 0xf028 */
    U_SMMU_INTCLR_NS       SMMU_INTCLR_NS                    ; /* 0xf02c */
    HI_U32           reserved_23[118]                     ; /* 0xf030~0xf204 */
    U_SMMU_SCB_TTBR        SMMU_SCB_TTBR                     ; /* 0xf208 */
    U_SMMU_CB_TTBR         SMMU_CB_TTBR                      ; /* 0xf20c */
    HI_U32           reserved_24[56]                     ; /* 0xf210~0xf2ec */
    U_SMMU_ERR_RDADDR_S    SMMU_ERR_RDADDR_S                 ; /* 0xf2f0 */
    U_SMMU_ERR_WRADDR_S    SMMU_ERR_WRADDR_S                 ; /* 0xf2f4 */
    HI_U32           reserved_25[3]                     ; /* 0xf2f8~0xf300 */
    U_SMMU_ERR_RDADDR_NS   SMMU_ERR_RDADDR_NS                ; /* 0xf304 */
    U_SMMU_ERR_WRADDR_NS   SMMU_ERR_WRADDR_NS                ; /* 0xf308 */
    HI_U32           reserved_26                     ; /* 0xf30c */
    U_SMMU_FAULT_ADDR_PTW_S   SMMU_FAULT_ADDR_PTW_S          ; /* 0xf310 */
    U_SMMU_FAULT_ID_PTW_S   SMMU_FAULT_ID_PTW_S              ; /* 0xf314 */
    HI_U32           reserved_27[2]                     ; /* 0xf318~0xf31c */
    U_SMMU_FAULT_ADDR_PTW_NS   SMMU_FAULT_ADDR_PTW_NS        ; /* 0xf320 */
    U_SMMU_FAULT_ID_PTW_NS   SMMU_FAULT_ID_PTW_NS            ; /* 0xf324 */
    U_SMMU_FAULT_PTW_NUM   SMMU_FAULT_PTW_NUM                ; /* 0xf328 */
    HI_U32           reserved_28                     ; /* 0xf32c */
    U_SMMU_FAULT_ADDR_WR_S   SMMU_FAULT_ADDR_WR_S            ; /* 0xf330 */
    U_SMMU_FAULT_TLB_WR_S   SMMU_FAULT_TLB_WR_S              ; /* 0xf334 */
    U_SMMU_FAULT_ID_WR_S   SMMU_FAULT_ID_WR_S                ; /* 0xf338 */
    HI_U32           reserved_29                     ; /* 0xf33c */
    U_SMMU_FAULT_ADDR_WR_NS   SMMU_FAULT_ADDR_WR_NS          ; /* 0xf340 */
    U_SMMU_FAULT_TLB_WR_NS   SMMU_FAULT_TLB_WR_NS            ; /* 0xf344 */
    U_SMMU_FAULT_ID_WR_NS   SMMU_FAULT_ID_WR_NS              ; /* 0xf348 */
    HI_U32           reserved_30                     ; /* 0xf34c */
    U_SMMU_FAULT_ADDR_RD_S   SMMU_FAULT_ADDR_RD_S            ; /* 0xf350 */
    U_SMMU_FAULT_TLB_RD_S   SMMU_FAULT_TLB_RD_S              ; /* 0xf354 */
    U_SMMU_FAULT_ID_RD_S   SMMU_FAULT_ID_RD_S                ; /* 0xf358 */
    HI_U32           reserved_31                     ; /* 0xf35c */
    U_SMMU_FAULT_ADDR_RD_NS   SMMU_FAULT_ADDR_RD_NS          ; /* 0xf360 */
    U_SMMU_FAULT_TLB_RD_NS   SMMU_FAULT_TLB_RD_NS            ; /* 0xf364 */
    U_SMMU_FAULT_ID_RD_NS   SMMU_FAULT_ID_RD_NS              ; /* 0xf368 */
    U_SMMU_FAULT_TBU_INFO   SMMU_FAULT_TBU_INFO              ; /* 0xf36c */
    U_SMMU_FAULT_TBU_DBG   SMMU_FAULT_TBU_DBG                ; /* 0xf370 */
    U_SMMU_PREF_BUFFER_EMPTY   SMMU_PREF_BUFFER_EMPTY        ; /* 0xf374 */
    U_SMMU_PTWQ_IDLE       SMMU_PTWQ_IDLE                    ; /* 0xf378 */
    U_SMMU_RESET_STATE     SMMU_RESET_STATE                  ; /* 0xf37c */
    U_SMMU_MASTER_DBG0     SMMU_MASTER_DBG0                  ; /* 0xf380 */
    U_SMMU_MASTER_DBG1     SMMU_MASTER_DBG1                  ; /* 0xf384 */
    U_SMMU_MASTER_DBG2     SMMU_MASTER_DBG2                  ; /* 0xf388 */
    U_SMMU_MASTER_DBG3     SMMU_MASTER_DBG3                  ; /* 0xf38c */
    HI_U32           reserved_32[100]                     ; /* 0xf390~0xf51c */
    U_SMMU_MASTER_DBG4     SMMU_MASTER_DBG4                  ; /* 0xf520 */
    U_SMMU_MASTER_DBG5     SMMU_MASTER_DBG5                  ; /* 0xf524 */
    U_SMMU_MASTER_DBG6     SMMU_MASTER_DBG6                  ; /* 0xf528 */
    U_SMMU_MASTER_DBG7     SMMU_MASTER_DBG7                  ; /* 0xf52c */
    U_SMMU_MASTER_DBG8     SMMU_MASTER_DBG8                  ; /* 0xf530 */
#endif
} TDE_HWNode_S;



#define TDE_SPLIT_NODE_MAX 20
typedef struct
{

        HI_U32 s1_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 s1_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s1_zme_iw[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_zme_ow[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_hor_loffset[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_hor_coffset[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_hpzme_width[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s1_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_h_scan_ord[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s1_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_hlmsc_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_hchmsc_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_hratio[TDE_SPLIT_NODE_MAX]            ;//(u,4,20)
        HI_U32 s1_hpzme_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_hpzme_mode[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 s1_dma[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s1_v_scan_ord[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cbcr_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_ch0_addr[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_ch1_addr[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_ch0_stride[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_ch1_stride[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_hlmid_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_hchmid_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_hafir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_hlfir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_hchfir_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_hfir_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_vlmsc_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_vchmsc_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_vlmid_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_vchmid_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_vsc_chroma_tap[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 s1_vafir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_vlfir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_vchfir_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_zme_out_fmt[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_zme_in_fmt[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s1_vratio[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_vluma_offset[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 s1_vchroma_offset[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 s1_zme_oh[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_zme_ih[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 s2_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s2_zme_iw[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_zme_ow[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_hor_loffset[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_hor_coffset[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_hpzme_width[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s2_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_h_scan_ord[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_hlmsc_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_hchmsc_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_hratio[TDE_SPLIT_NODE_MAX]            ;//(u,4,20)
        HI_U32 s2_hpzme_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_hpzme_mode[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 s2_dma[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_v_scan_ord[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_clut_mode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s2_cbcr_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_ch0_addr[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_ch1_addr[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_ch0_stride[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_ch1_stride[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_hlmid_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_hchmid_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_hafir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_hlfir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_hchfir_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_hfir_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_vlmsc_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_vchmsc_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_vlmid_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_vchmid_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_vsc_chroma_tap[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 s2_vafir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_vlfir_en[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_vchfir_en[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_zme_out_fmt[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_zme_in_fmt[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 s2_vratio[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_vluma_offset[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 s2_vchroma_offset[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 s2_zme_oh[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_zme_ih[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_csc_en[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscidc2[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscidc1[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscidc0[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscodc2[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscodc1[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscodc0[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 s1_cscp00[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp01[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp02[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp10[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp11[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp12[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp20[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp21[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_cscp22[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_csc_mode[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 src_csc_premulten[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 g3_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 g3_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g3_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g3_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g3_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g3_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g3_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g3_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g3_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 g3_addr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g3_stride[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g3_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g3_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g3_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g3_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g4_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 g4_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g4_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g4_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g4_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g4_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g4_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g4_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g4_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 g4_addr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g4_stride[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g4_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g4_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g4_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g4_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g5_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 g5_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g5_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g5_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g5_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g5_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g5_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g5_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g5_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 g5_addr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g5_stride[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g5_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g5_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g5_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g5_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g6_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 g6_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g6_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g6_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g6_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g6_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g6_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g6_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g6_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 g6_addr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g6_stride[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g6_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g6_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g6_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g6_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g7_en[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 g7_xfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g7_width[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g7_hoffset_pix[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g7_mode[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g7_alpha_range[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g7_rgb_exp[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g7_argb_order[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g7_fmt[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 g7_addr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 g7_stride[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g7_height[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g7_color_fill[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 g7_alpha1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g7_alpha0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 cbm_en[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 cbm_mode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 alu_mode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 mix_prio6[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio5[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio4[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio3[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio2[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 mix_prio0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 cbmbkga[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 cbmbkgr[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 cbmbkgg[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 cbmbkgb[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 colorizeen[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 colorizer[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 colorizeg[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 colorizeb[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 blendropen[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 alpha_border_en[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 alpha_from[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 a_rop[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 rgb_rop[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keyen[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 keysel[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 keyamode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keyrmode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keygmode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keybmode[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keyamin[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keyrmin[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keygmin[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keybmin[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keyamax[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keyrmax[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keygmax[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keybmax[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 keyamask[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keyrmask[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keygmask[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 keybmask[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s1_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 s1_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s1_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 s1_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s1_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s2_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 s2_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 s2_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 s2_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s2_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 s2_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g3_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 g3_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g3_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g3_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 g3_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g3_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g3_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g3_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g4_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 g4_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g4_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g4_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 g4_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g4_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g4_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g4_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g5_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 g5_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g5_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g5_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 g5_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g5_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g5_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g5_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g6_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 g6_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g6_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g6_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 g6_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g6_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g6_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g6_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g7_coverblenden[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 g7_galpha[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 g7_blendmode[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g7_multiglobalen[TDE_SPLIT_NODE_MAX]     ;
        HI_U32 g7_premulten[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 g7_palphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g7_galphaen[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 g7_yfpos[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_width[TDE_SPLIT_NODE_MAX]              ;
        HI_U32 des_hoffset_pix[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 des_crop_en[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_crop_start_x[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 des_crop_end_x[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_h_scan_ord[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_fmt[TDE_SPLIT_NODE_MAX]                ;
        HI_U32 des_height[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_en[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 cmp_addr_chg[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 cmp_en[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 des_v_scan_ord[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_alpha_range[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 des_rgb_round[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 des_cbcr_order[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_argb_order[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_ch0_addr[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 des_ch1_addr[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 des_ch0_stride[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_ch1_stride[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 des_crop_mode[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 des_alpha_thd[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 des_crop_start_y[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 des_crop_end_y[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 head_ar_addr[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 head_gb_addr[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 des_premulten[TDE_SPLIT_NODE_MAX]          ;
        HI_U32 des_csc_en[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscidc2[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscidc1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscidc0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscodc2[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscodc1[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscodc0[TDE_SPLIT_NODE_MAX]            ;
        HI_U32 des_cscp00[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp01[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp02[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp10[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp11[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp12[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp20[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp21[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_cscp22[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 des_v_dswm_mode[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 des_h_dswm_mode[TDE_SPLIT_NODE_MAX]        ;
        HI_U32 des_hd_ar_mmu_bypass[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 des_hd_gb_mmu_bypass[TDE_SPLIT_NODE_MAX]    ;
        HI_U32 des_ch0_mmu_bypass[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 des_ch1_mmu_bypass[TDE_SPLIT_NODE_MAX]      ;
        HI_U32 s1_ch0_mmu_bypass[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s1_ch1_mmu_bypass[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_ch0_mmu_bypass[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 s2_ch1_mmu_bypass[TDE_SPLIT_NODE_MAX]       ;
        HI_U32 g3_mmu_bypass[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g4_mmu_bypass[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g5_mmu_bypass[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g6_mmu_bypass[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 g7_mmu_bypass[TDE_SPLIT_NODE_MAX]           ;
        HI_U32 clut_mmu_bypass[TDE_SPLIT_NODE_MAX]         ;
        HI_U32 s1_ch0_prot[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s1_ch1_prot[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s2_ch0_prot[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 s2_ch1_prot[TDE_SPLIT_NODE_MAX]             ;
        HI_U32 g3_prot[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 g4_prot[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 g5_prot[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 g6_prot[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 g7_prot[TDE_SPLIT_NODE_MAX]                 ;
        HI_U32 s1_scl_lh[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s1_scl_lv[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s1_scl_ch[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s1_scl_cv[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_scl_lh[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_scl_lv[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_scl_ch[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 s2_scl_cv[TDE_SPLIT_NODE_MAX]               ;
        HI_U32 clut_addr[TDE_SPLIT_NODE_MAX]               ;

		HI_U32 node_num_in 						           ;
} TDESplitCropOut_S;

#if 0
typedef struct
{
        HI_U32 s1_en                ;
        HI_U32 s1_xfpos             ;
        HI_U32 s1_zme_iw            ;
        HI_U32 s1_zme_ow            ;
        HI_U32 s1_hor_loffset       ;
        HI_U32 s1_hor_coffset       ;
        HI_U32 s1_hpzme_width       ;
        HI_U32 s1_width             ;
        HI_U32 s1_hoffset_pix       ;
        HI_U32 s1_h_scan_ord        ;
        HI_U32 s1_fmt               ;
        HI_U32 s1_height            ;
        HI_U32 s1_hlmsc_en          ;
        HI_U32 s1_hchmsc_en         ;
        HI_U32 s1_hratio            ;//(u,4,20)
        HI_U32 s1_hpzme_en          ;
        HI_U32 s1_hpzme_mode        ;
        HI_U32 s1_mode              ;
        HI_U32 s1_dma               ;
        HI_U32 s1_v_scan_ord        ;
        HI_U32 s1_alpha_range       ;
        HI_U32 s1_rgb_exp           ;
        HI_U32 s1_cbcr_order        ;
        HI_U32 s1_argb_order        ;
        HI_U32 s1_ch0_addr          ;
        HI_U32 s1_ch1_addr          ;
        HI_U32 s1_ch0_stride        ;
        HI_U32 s1_ch1_stride        ;
        HI_U32 s1_color_fill        ;
        HI_U32 s1_alpha1            ;
        HI_U32 s1_alpha0            ;
        HI_U32 s1_hlmid_en          ;
        HI_U32 s1_hchmid_en         ;
        HI_U32 s1_hafir_en          ;
        HI_U32 s1_hlfir_en          ;
        HI_U32 s1_hchfir_en         ;
        HI_U32 s1_hfir_order        ;
        HI_U32 s1_vlmsc_en          ;
        HI_U32 s1_vchmsc_en         ;
        HI_U32 s1_vlmid_en          ;
        HI_U32 s1_vchmid_en         ;
        HI_U32 s1_vsc_chroma_tap    ;
        HI_U32 s1_vafir_en          ;
        HI_U32 s1_vlfir_en          ;
        HI_U32 s1_vchfir_en         ;
        HI_U32 s1_zme_out_fmt       ;
        HI_U32 s1_zme_in_fmt        ;
        HI_U32 s1_vratio            ;
        HI_U32 s1_vluma_offset      ;
        HI_U32 s1_vchroma_offset    ;
        HI_U32 s1_zme_oh            ;
        HI_U32 s1_zme_ih            ;
        HI_U32 s2_en                ;
        HI_U32 s2_xfpos             ;
        HI_U32 s2_zme_iw            ;
        HI_U32 s2_zme_ow            ;
        HI_U32 s2_hor_loffset       ;
        HI_U32 s2_hor_coffset       ;
        HI_U32 s2_hpzme_width       ;
        HI_U32 s2_width             ;
        HI_U32 s2_hoffset_pix       ;
        HI_U32 s2_h_scan_ord        ;
        HI_U32 s2_fmt               ;
        HI_U32 s2_height            ;
        HI_U32 s2_hlmsc_en          ;
        HI_U32 s2_hchmsc_en         ;
        HI_U32 s2_hratio            ;//(u,4,20)
        HI_U32 s2_hpzme_en          ;
        HI_U32 s2_hpzme_mode        ;
        HI_U32 s2_mode              ;
        HI_U32 s2_dma               ;
        HI_U32 s2_v_scan_ord        ;
        HI_U32 s2_alpha_range       ;
        HI_U32 s2_clut_mode         ;
        HI_U32 s2_rgb_exp           ;
        HI_U32 s2_cbcr_order        ;
        HI_U32 s2_argb_order        ;
        HI_U32 s2_ch0_addr          ;
        HI_U32 s2_ch1_addr          ;
        HI_U32 s2_ch0_stride        ;
        HI_U32 s2_ch1_stride        ;
        HI_U32 s2_color_fill        ;
        HI_U32 s2_alpha1            ;
        HI_U32 s2_alpha0            ;
        HI_U32 s2_hlmid_en          ;
        HI_U32 s2_hchmid_en         ;
        HI_U32 s2_hafir_en          ;
        HI_U32 s2_hlfir_en          ;
        HI_U32 s2_hchfir_en         ;
        HI_U32 s2_hfir_order        ;
        HI_U32 s2_vlmsc_en          ;
        HI_U32 s2_vchmsc_en         ;
        HI_U32 s2_vlmid_en          ;
        HI_U32 s2_vchmid_en         ;
        HI_U32 s2_vsc_chroma_tap    ;
        HI_U32 s2_vafir_en          ;
        HI_U32 s2_vlfir_en          ;
        HI_U32 s2_vchfir_en         ;
        HI_U32 s2_zme_out_fmt       ;
        HI_U32 s2_zme_in_fmt        ;
        HI_U32 s2_vratio            ;
        HI_U32 s2_vluma_offset      ;
        HI_U32 s2_vchroma_offset    ;
        HI_U32 s2_zme_oh            ;
        HI_U32 s2_zme_ih            ;
        HI_U32 s1_csc_en            ;
        HI_U32 s1_cscidc2           ;
        HI_U32 s1_cscidc1           ;
        HI_U32 s1_cscidc0           ;
        HI_U32 s1_cscodc2           ;
        HI_U32 s1_cscodc1           ;
        HI_U32 s1_cscodc0           ;
        HI_U32 s1_cscp00            ;
        HI_U32 s1_cscp01            ;
        HI_U32 s1_cscp02            ;
        HI_U32 s1_cscp10            ;
        HI_U32 s1_cscp11            ;
        HI_U32 s1_cscp12            ;
        HI_U32 s1_cscp20            ;
        HI_U32 s1_cscp21            ;
        HI_U32 s1_cscp22            ;
        HI_U32 s2_csc_mode          ;
        HI_U32 src_csc_premulten    ;
        HI_U32 g3_en                ;
        HI_U32 g3_xfpos             ;
        HI_U32 g3_width             ;
        HI_U32 g3_hoffset_pix       ;
        HI_U32 g3_mode              ;
        HI_U32 g3_alpha_range       ;
        HI_U32 g3_rgb_exp           ;
        HI_U32 g3_argb_order        ;
        HI_U32 g3_fmt               ;
        HI_U32 g3_addr              ;
        HI_U32 g3_stride            ;
        HI_U32 g3_height            ;
        HI_U32 g3_color_fill        ;
        HI_U32 g3_alpha1            ;
        HI_U32 g3_alpha0            ;
        HI_U32 g4_en                ;
        HI_U32 g4_xfpos             ;
        HI_U32 g4_width             ;
        HI_U32 g4_hoffset_pix       ;
        HI_U32 g4_mode              ;
        HI_U32 g4_alpha_range       ;
        HI_U32 g4_rgb_exp           ;
        HI_U32 g4_argb_order        ;
        HI_U32 g4_fmt               ;
        HI_U32 g4_addr              ;
        HI_U32 g4_stride            ;
        HI_U32 g4_height            ;
        HI_U32 g4_color_fill        ;
        HI_U32 g4_alpha1            ;
        HI_U32 g4_alpha0            ;
        HI_U32 g5_en                ;
        HI_U32 g5_xfpos             ;
        HI_U32 g5_width             ;
        HI_U32 g5_hoffset_pix       ;
        HI_U32 g5_mode              ;
        HI_U32 g5_alpha_range       ;
        HI_U32 g5_rgb_exp           ;
        HI_U32 g5_argb_order        ;
        HI_U32 g5_fmt               ;
        HI_U32 g5_addr              ;
        HI_U32 g5_stride            ;
        HI_U32 g5_height            ;
        HI_U32 g5_color_fill        ;
        HI_U32 g5_alpha1            ;
        HI_U32 g5_alpha0            ;
        HI_U32 g6_en                ;
        HI_U32 g6_xfpos             ;
        HI_U32 g6_width             ;
        HI_U32 g6_hoffset_pix       ;
        HI_U32 g6_mode              ;
        HI_U32 g6_alpha_range       ;
        HI_U32 g6_rgb_exp           ;
        HI_U32 g6_argb_order        ;
        HI_U32 g6_fmt               ;
        HI_U32 g6_addr              ;
        HI_U32 g6_stride            ;
        HI_U32 g6_height            ;
        HI_U32 g6_color_fill        ;
        HI_U32 g6_alpha1            ;
        HI_U32 g6_alpha0            ;
        HI_U32 g7_en                ;
        HI_U32 g7_xfpos             ;
        HI_U32 g7_width             ;
        HI_U32 g7_hoffset_pix       ;
        HI_U32 g7_mode              ;
        HI_U32 g7_alpha_range       ;
        HI_U32 g7_rgb_exp           ;
        HI_U32 g7_argb_order        ;
        HI_U32 g7_fmt               ;
        HI_U32 g7_addr              ;
        HI_U32 g7_stride            ;
        HI_U32 g7_height            ;
        HI_U32 g7_color_fill        ;
        HI_U32 g7_alpha1            ;
        HI_U32 g7_alpha0            ;
        HI_U32 cbm_en               ;
        HI_U32 cbm_mode             ;
        HI_U32 alu_mode             ;
        HI_U32 mix_prio6            ;
        HI_U32 mix_prio5            ;
        HI_U32 mix_prio4            ;
        HI_U32 mix_prio3            ;
        HI_U32 mix_prio2            ;
        HI_U32 mix_prio1            ;
        HI_U32 mix_prio0            ;
        HI_U32 cbmbkga              ;
        HI_U32 cbmbkgr              ;
        HI_U32 cbmbkgg              ;
        HI_U32 cbmbkgb              ;
        HI_U32 colorizeen           ;
        HI_U32 colorizer            ;
        HI_U32 colorizeg            ;
        HI_U32 colorizeb            ;
        HI_U32 blendropen           ;
        HI_U32 alpha_border_en      ;
        HI_U32 alpha_from           ;
        HI_U32 a_rop                ;
        HI_U32 rgb_rop              ;
        HI_U32 keyen                ;
        HI_U32 keysel               ;
        HI_U32 keyamode             ;
        HI_U32 keyrmode             ;
        HI_U32 keygmode             ;
        HI_U32 keybmode             ;
        HI_U32 keyamin              ;
        HI_U32 keyrmin              ;
        HI_U32 keygmin              ;
        HI_U32 keybmin              ;
        HI_U32 keyamax              ;
        HI_U32 keyrmax              ;
        HI_U32 keygmax              ;
        HI_U32 keybmax              ;
        HI_U32 keyamask             ;
        HI_U32 keyrmask             ;
        HI_U32 keygmask             ;
        HI_U32 keybmask             ;
        HI_U32 s1_coverblenden      ;
        HI_U32 s1_galpha            ;
        HI_U32 s1_blendmode         ;
        HI_U32 s1_multiglobalen     ;
        HI_U32 s1_premulten         ;
        HI_U32 s1_palphaen          ;
        HI_U32 s1_galphaen          ;
        HI_U32 s1_yfpos             ;
        HI_U32 s2_coverblenden      ;
        HI_U32 s2_galpha            ;
        HI_U32 s2_blendmode         ;
        HI_U32 s2_multiglobalen     ;
        HI_U32 s2_premulten         ;
        HI_U32 s2_palphaen          ;
        HI_U32 s2_galphaen          ;
        HI_U32 s2_yfpos             ;
        HI_U32 g3_coverblenden      ;
        HI_U32 g3_galpha            ;
        HI_U32 g3_blendmode         ;
        HI_U32 g3_multiglobalen     ;
        HI_U32 g3_premulten         ;
        HI_U32 g3_palphaen          ;
        HI_U32 g3_galphaen          ;
        HI_U32 g3_yfpos             ;
        HI_U32 g4_coverblenden      ;
        HI_U32 g4_galpha            ;
        HI_U32 g4_blendmode         ;
        HI_U32 g4_multiglobalen     ;
        HI_U32 g4_premulten         ;
        HI_U32 g4_palphaen          ;
        HI_U32 g4_galphaen          ;
        HI_U32 g4_yfpos             ;
        HI_U32 g5_coverblenden      ;
        HI_U32 g5_galpha            ;
        HI_U32 g5_blendmode         ;
        HI_U32 g5_multiglobalen     ;
        HI_U32 g5_premulten         ;
        HI_U32 g5_palphaen          ;
        HI_U32 g5_galphaen          ;
        HI_U32 g5_yfpos             ;
        HI_U32 g6_coverblenden      ;
        HI_U32 g6_galpha            ;
        HI_U32 g6_blendmode         ;
        HI_U32 g6_multiglobalen     ;
        HI_U32 g6_premulten         ;
        HI_U32 g6_palphaen          ;
        HI_U32 g6_galphaen          ;
        HI_U32 g6_yfpos             ;
        HI_U32 g7_coverblenden      ;
        HI_U32 g7_galpha            ;
        HI_U32 g7_blendmode         ;
        HI_U32 g7_multiglobalen     ;
        HI_U32 g7_premulten         ;
        HI_U32 g7_palphaen          ;
        HI_U32 g7_galphaen          ;
        HI_U32 g7_yfpos             ;
        HI_U32 des_width              ;
        HI_U32 des_hoffset_pix        ;
        HI_U32 des_crop_en            ;
        HI_U32 des_crop_start_x       ;
        HI_U32 des_crop_end_x         ;
        HI_U32 des_h_scan_ord         ;
        HI_U32 des_fmt                ;
        HI_U32 des_height             ;
        HI_U32 des_en                 ;
        HI_U32 cmp_addr_chg           ;
        HI_U32 cmp_en                 ;
        HI_U32 des_v_scan_ord         ;
        HI_U32 des_alpha_range        ;
        HI_U32 des_rgb_round          ;
        HI_U32 des_cbcr_order         ;
        HI_U32 des_argb_order         ;
        HI_U32 des_ch0_addr           ;
        HI_U32 des_ch1_addr           ;
        HI_U32 des_ch0_stride         ;
        HI_U32 des_ch1_stride         ;
        HI_U32 des_crop_mode          ;
        HI_U32 des_alpha_thd          ;
        HI_U32 des_crop_start_y       ;
        HI_U32 des_crop_end_y         ;
        HI_U32 head_ar_addr           ;
        HI_U32 head_gb_addr           ;
        HI_U32 des_premulten          ;
        HI_U32 des_csc_en             ;
        HI_U32 des_cscidc2            ;
        HI_U32 des_cscidc1            ;
        HI_U32 des_cscidc0            ;
        HI_U32 des_cscodc2            ;
        HI_U32 des_cscodc1            ;
        HI_U32 des_cscodc0            ;
        HI_U32 des_cscp00             ;
        HI_U32 des_cscp01             ;
        HI_U32 des_cscp02             ;
        HI_U32 des_cscp10             ;
        HI_U32 des_cscp11             ;
        HI_U32 des_cscp12             ;
        HI_U32 des_cscp20             ;
        HI_U32 des_cscp21             ;
        HI_U32 des_cscp22             ;
        HI_U32 des_v_dswm_mode        ;
        HI_U32 des_h_dswm_mode        ;
        HI_U32 des_hd_ar_mmu_bypass    ;
        HI_U32 des_hd_gb_mmu_bypass    ;
        HI_U32 des_ch0_mmu_bypass      ;
        HI_U32 des_ch1_mmu_bypass      ;
        HI_U32 s1_ch0_mmu_bypass       ;
        HI_U32 s1_ch1_mmu_bypass       ;
        HI_U32 s2_ch0_mmu_bypass       ;
        HI_U32 s2_ch1_mmu_bypass       ;
        HI_U32 g3_mmu_bypass           ;
        HI_U32 g4_mmu_bypass           ;
        HI_U32 g5_mmu_bypass           ;
        HI_U32 g6_mmu_bypass           ;
        HI_U32 g7_mmu_bypass           ;
        HI_U32 clut_mmu_bypass         ;
        HI_U32 s1_ch0_prot             ;
        HI_U32 s1_ch1_prot             ;
        HI_U32 s2_ch0_prot             ;
        HI_U32 s2_ch1_prot             ;
        HI_U32 g3_prot                 ;
        HI_U32 g4_prot                 ;
        HI_U32 g5_prot                 ;
        HI_U32 g6_prot                 ;
        HI_U32 g7_prot                 ;
        HI_U32 s1_scl_lh               ;
        HI_U32 s1_scl_lv               ;
        HI_U32 s1_scl_ch               ;
        HI_U32 s1_scl_cv               ;
        HI_U32 s2_scl_lh               ;
        HI_U32 s2_scl_lv               ;
        HI_U32 s2_scl_ch               ;
        HI_U32 s2_scl_cv               ;
        HI_U32 clut_addr               ;
    ////
    	HI_U32 split_en				   ;
}TDESplitCropCfg_S;

#endif

typedef struct
{
        HI_U32 s1_en                ;
        HI_U32 s1_xfpos             ;
        HI_U32 s1_zme_iw            ;
        HI_U32 s1_zme_ow            ;
        HI_U32 s1_hor_loffset       ;
        HI_U32 s1_hor_coffset       ;
        HI_U32 s1_hpzme_width       ;
        HI_U32 s1_width             ;
        HI_U32 s1_hoffset_pix       ;
        HI_U32 s1_h_scan_ord        ;
        HI_U32 s1_fmt               ;
        HI_U32 s1_height            ;
        HI_U32 s1_hlmsc_en          ;
        HI_U32 s1_hchmsc_en         ;
        HI_U32 s1_hratio            ;//(u,4,20)
        HI_U32 s1_hpzme_en          ;
        HI_U32 s1_hpzme_mode        ;
        HI_U32 s1_mode              ;
        HI_U32 s1_dma               ;
        HI_U32 s1_422v_pro          ;
        HI_U32 s1_v_scan_ord        ;
        HI_U32 s1_alpha_range       ;
        HI_U32 s1_rgb_exp           ;
        HI_U32 s1_cbcr_order        ;
        HI_U32 s1_argb_order        ;
        HI_U32 s1_ch0_addr          ;
        HI_U32 s1_ch1_addr          ;
        HI_U32 s1_ch0_stride        ;
        HI_U32 s1_ch1_stride        ;
        HI_U32 s1_color_fill        ;
        HI_U32 s1_alpha1            ;
        HI_U32 s1_alpha0            ;
        HI_U32 s1_hlmid_en          ;
        HI_U32 s1_hchmid_en         ;
        HI_U32 s1_hafir_en          ;
        HI_U32 s1_hlfir_en          ;
        HI_U32 s1_hchfir_en         ;
        HI_U32 s1_hfir_order        ;
        HI_U32 s1_vlmsc_en          ;
        HI_U32 s1_vchmsc_en         ;
        HI_U32 s1_vlmid_en          ;
        HI_U32 s1_vchmid_en         ;
        HI_U32 s1_vsc_chroma_tap    ;
        HI_U32 s1_vafir_en          ;
        HI_U32 s1_vlfir_en          ;
        HI_U32 s1_vchfir_en         ;
        HI_U32 s1_zme_out_fmt       ;
        HI_U32 s1_zme_in_fmt        ;
        HI_U32 s1_vratio            ;
        HI_U32 s1_vluma_offset      ;
        HI_U32 s1_vchroma_offset    ;
        HI_U32 s1_zme_oh            ;
        HI_U32 s1_zme_ih            ;
        HI_U32 s2_en                ;
        HI_U32 s2_xfpos             ;
        HI_U32 s2_zme_iw            ;
        HI_U32 s2_zme_ow            ;
        HI_U32 s2_hor_loffset       ;
        HI_U32 s2_hor_coffset       ;
        HI_U32 s2_hpzme_width       ;
        HI_U32 s2_width             ;
        HI_U32 s2_hoffset_pix       ;
        HI_U32 s2_h_scan_ord        ;
        HI_U32 s2_fmt               ;
        HI_U32 s2_height            ;
        HI_U32 s2_hlmsc_en          ;
        HI_U32 s2_hchmsc_en         ;
        HI_U32 s2_hratio            ;//(u,4,20)
        HI_U32 s2_hpzme_en          ;
        HI_U32 s2_hpzme_mode        ;
        HI_U32 s2_mode              ;
        HI_U32 s2_dma               ;
        HI_U32 s2_422v_pro          ;
        HI_U32 s2_v_scan_ord        ;
        HI_U32 s2_alpha_range       ;
        HI_U32 s2_clut_mode         ;
        HI_U32 s2_rgb_exp           ;
        HI_U32 s2_cbcr_order        ;
        HI_U32 s2_argb_order        ;
        HI_U32 s2_ch0_addr          ;
        HI_U32 s2_ch1_addr          ;
        HI_U32 s2_ch0_stride        ;
        HI_U32 s2_ch1_stride        ;
        HI_U32 s2_color_fill        ;
        HI_U32 s2_alpha1            ;
        HI_U32 s2_alpha0            ;
        HI_U32 s2_hlmid_en          ;
        HI_U32 s2_hchmid_en         ;
        HI_U32 s2_hafir_en          ;
        HI_U32 s2_hlfir_en          ;
        HI_U32 s2_hchfir_en         ;
        HI_U32 s2_hfir_order        ;
        HI_U32 s2_vlmsc_en          ;
        HI_U32 s2_vchmsc_en         ;
        HI_U32 s2_vlmid_en          ;
        HI_U32 s2_vchmid_en         ;
        HI_U32 s2_vsc_chroma_tap    ;
        HI_U32 s2_vafir_en          ;
        HI_U32 s2_vlfir_en          ;
        HI_U32 s2_vchfir_en         ;
        HI_U32 s2_zme_out_fmt       ;
        HI_U32 s2_zme_in_fmt        ;
        HI_U32 s2_vratio            ;
        HI_U32 s2_vluma_offset      ;
        HI_U32 s2_vchroma_offset    ;
        HI_U32 s2_zme_oh            ;
        HI_U32 s2_zme_ih            ;
        HI_U32 s1_csc_en            ;
        HI_U32 s1_cscidc2           ;
        HI_U32 s1_cscidc1           ;
        HI_U32 s1_cscidc0           ;
        HI_U32 s1_cscodc2           ;
        HI_U32 s1_cscodc1           ;
        HI_U32 s1_cscodc0           ;
        HI_U32 s1_cscp00            ;
        HI_U32 s1_cscp01            ;
        HI_U32 s1_cscp02            ;
        HI_U32 s1_cscp10            ;
        HI_U32 s1_cscp11            ;
        HI_U32 s1_cscp12            ;
        HI_U32 s1_cscp20            ;
        HI_U32 s1_cscp21            ;
        HI_U32 s1_cscp22            ;
        HI_U32 s2_csc_mode          ;
        HI_U32 src_csc_premulten    ;
        HI_U32 g3_en                ;
        HI_U32 g3_xfpos             ;
        HI_U32 g3_width             ;
        HI_U32 g3_hoffset_pix       ;
        HI_U32 g3_mode              ;
        HI_U32 g3_alpha_range       ;
        HI_U32 g3_rgb_exp           ;
        HI_U32 g3_argb_order        ;
        HI_U32 g3_fmt               ;
        HI_U32 g3_addr              ;
        HI_U32 g3_stride            ;
        HI_U32 g3_height            ;
        HI_U32 g3_color_fill        ;
        HI_U32 g3_alpha1            ;
        HI_U32 g3_alpha0            ;
        HI_U32 g4_en                ;
        HI_U32 g4_xfpos             ;
        HI_U32 g4_width             ;
        HI_U32 g4_hoffset_pix       ;
        HI_U32 g4_mode              ;
        HI_U32 g4_alpha_range       ;
        HI_U32 g4_rgb_exp           ;
        HI_U32 g4_argb_order        ;
        HI_U32 g4_fmt               ;
        HI_U32 g4_addr              ;
        HI_U32 g4_stride            ;
        HI_U32 g4_height            ;
        HI_U32 g4_color_fill        ;
        HI_U32 g4_alpha1            ;
        HI_U32 g4_alpha0            ;
        HI_U32 g5_en                ;
        HI_U32 g5_xfpos             ;
        HI_U32 g5_width             ;
        HI_U32 g5_hoffset_pix       ;
        HI_U32 g5_mode              ;
        HI_U32 g5_alpha_range       ;
        HI_U32 g5_rgb_exp           ;
        HI_U32 g5_argb_order        ;
        HI_U32 g5_fmt               ;
        HI_U32 g5_addr              ;
        HI_U32 g5_stride            ;
        HI_U32 g5_height            ;
        HI_U32 g5_color_fill        ;
        HI_U32 g5_alpha1            ;
        HI_U32 g5_alpha0            ;
        HI_U32 g6_en                ;
        HI_U32 g6_xfpos             ;
        HI_U32 g6_width             ;
        HI_U32 g6_hoffset_pix       ;
        HI_U32 g6_mode              ;
        HI_U32 g6_alpha_range       ;
        HI_U32 g6_rgb_exp           ;
        HI_U32 g6_argb_order        ;
        HI_U32 g6_fmt               ;
        HI_U32 g6_addr              ;
        HI_U32 g6_stride            ;
        HI_U32 g6_height            ;
        HI_U32 g6_color_fill        ;
        HI_U32 g6_alpha1            ;
        HI_U32 g6_alpha0            ;
        HI_U32 g7_en                ;
        HI_U32 g7_xfpos             ;
        HI_U32 g7_width             ;
        HI_U32 g7_hoffset_pix       ;
        HI_U32 g7_mode              ;
        HI_U32 g7_alpha_range       ;
        HI_U32 g7_rgb_exp           ;
        HI_U32 g7_argb_order        ;
        HI_U32 g7_fmt               ;
        HI_U32 g7_addr              ;
        HI_U32 g7_stride            ;
        HI_U32 g7_height            ;
        HI_U32 g7_color_fill        ;
        HI_U32 g7_alpha1            ;
        HI_U32 g7_alpha0            ;
        HI_U32 cbm_en               ;
        HI_U32 cbm_mode             ;
        HI_U32 alu_mode             ;
        HI_U32 mix_prio6            ;
        HI_U32 mix_prio5            ;
        HI_U32 mix_prio4            ;
        HI_U32 mix_prio3            ;
        HI_U32 mix_prio2            ;
        HI_U32 mix_prio1            ;
        HI_U32 mix_prio0            ;
        HI_U32 cbmbkga              ;
        HI_U32 cbmbkgr              ;
        HI_U32 cbmbkgg              ;
        HI_U32 cbmbkgb              ;
        HI_U32 colorizeen           ;
        HI_U32 colorizer            ;
        HI_U32 colorizeg            ;
        HI_U32 colorizeb            ;
        HI_U32 blendropen           ;
        HI_U32 alpha_border_en      ;
        HI_U32 alpha_from           ;
        HI_U32 a_rop                ;
        HI_U32 rgb_rop              ;
        HI_U32 keyen                ;
        HI_U32 keysel               ;
        HI_U32 keyamode             ;
        HI_U32 keyrmode             ;
        HI_U32 keygmode             ;
        HI_U32 keybmode             ;
        HI_U32 keyamin              ;
        HI_U32 keyrmin              ;
        HI_U32 keygmin              ;
        HI_U32 keybmin              ;
        HI_U32 keyamax              ;
        HI_U32 keyrmax              ;
        HI_U32 keygmax              ;
        HI_U32 keybmax              ;
        HI_U32 keyamask             ;
        HI_U32 keyrmask             ;
        HI_U32 keygmask             ;
        HI_U32 keybmask             ;
        HI_U32 s1_coverblenden      ;
        HI_U32 s1_galpha            ;
        HI_U32 s1_blendmode         ;
        HI_U32 s1_multiglobalen     ;
        HI_U32 s1_premulten         ;
        HI_U32 s1_palphaen          ;
        HI_U32 s1_galphaen          ;
        HI_U32 s1_yfpos             ;
        HI_U32 s2_coverblenden      ;
        HI_U32 s2_galpha            ;
        HI_U32 s2_blendmode         ;
        HI_U32 s2_multiglobalen     ;
        HI_U32 s2_premulten         ;
        HI_U32 s2_palphaen          ;
        HI_U32 s2_galphaen          ;
        HI_U32 s2_yfpos             ;
        HI_U32 g3_coverblenden      ;
        HI_U32 g3_galpha            ;
        HI_U32 g3_blendmode         ;
        HI_U32 g3_multiglobalen     ;
        HI_U32 g3_premulten         ;
        HI_U32 g3_palphaen          ;
        HI_U32 g3_galphaen          ;
        HI_U32 g3_yfpos             ;
        HI_U32 g4_coverblenden      ;
        HI_U32 g4_galpha            ;
        HI_U32 g4_blendmode         ;
        HI_U32 g4_multiglobalen     ;
        HI_U32 g4_premulten         ;
        HI_U32 g4_palphaen          ;
        HI_U32 g4_galphaen          ;
        HI_U32 g4_yfpos             ;
        HI_U32 g5_coverblenden      ;
        HI_U32 g5_galpha            ;
        HI_U32 g5_blendmode         ;
        HI_U32 g5_multiglobalen     ;
        HI_U32 g5_premulten         ;
        HI_U32 g5_palphaen          ;
        HI_U32 g5_galphaen          ;
        HI_U32 g5_yfpos             ;
        HI_U32 g6_coverblenden      ;
        HI_U32 g6_galpha            ;
        HI_U32 g6_blendmode         ;
        HI_U32 g6_multiglobalen     ;
        HI_U32 g6_premulten         ;
        HI_U32 g6_palphaen          ;
        HI_U32 g6_galphaen          ;
        HI_U32 g6_yfpos             ;
        HI_U32 g7_coverblenden      ;
        HI_U32 g7_galpha            ;
        HI_U32 g7_blendmode         ;
        HI_U32 g7_multiglobalen     ;
        HI_U32 g7_premulten         ;
        HI_U32 g7_palphaen          ;
        HI_U32 g7_galphaen          ;
        HI_U32 g7_yfpos             ;
        HI_U32 des_width              ;
        HI_U32 des_hoffset_pix        ;
        HI_U32 des_crop_en            ;
        HI_U32 des_crop_start_x       ;
        HI_U32 des_crop_end_x         ;
        HI_U32 des_h_scan_ord         ;
        HI_U32 des_fmt                ;
        HI_U32 des_height             ;
        HI_U32 des_en                 ;
        HI_U32 cmp_addr_chg           ;
        HI_U32 cmp_en                 ;
        HI_U32 des_v_scan_ord         ;
        HI_U32 des_alpha_range        ;
        HI_U32 des_rgb_round          ;
        HI_U32 des_cbcr_order         ;
        HI_U32 des_argb_order         ;
        HI_U32 des_ch0_addr           ;
        HI_U32 des_ch1_addr           ;
        HI_U32 des_ch0_stride         ;
        HI_U32 des_ch1_stride         ;
        HI_U32 des_crop_mode          ;
        HI_U32 des_alpha_thd          ;
        HI_U32 des_crop_start_y       ;
        HI_U32 des_crop_end_y         ;
        HI_U32 head_ar_addr           ;
        HI_U32 head_gb_addr           ;
        HI_U32 des_premulten          ;
        HI_U32 des_csc_en             ;
        HI_U32 des_cscidc2            ;
        HI_U32 des_cscidc1            ;
        HI_U32 des_cscidc0            ;
        HI_U32 des_cscodc2            ;
        HI_U32 des_cscodc1            ;
        HI_U32 des_cscodc0            ;
        HI_U32 des_cscp00             ;
        HI_U32 des_cscp01             ;
        HI_U32 des_cscp02             ;
        HI_U32 des_cscp10             ;
        HI_U32 des_cscp11             ;
        HI_U32 des_cscp12             ;
        HI_U32 des_cscp20             ;
        HI_U32 des_cscp21             ;
        HI_U32 des_cscp22             ;
        HI_U32 des_v_dswm_mode        ;
        HI_U32 des_h_dswm_mode        ;
        HI_U32 des_alpha_detect_clear ;
        HI_U32 des_hd_ar_mmu_bypass    ;
        HI_U32 des_hd_gb_mmu_bypass    ;
        HI_U32 des_ch0_mmu_bypass      ;
        HI_U32 des_ch1_mmu_bypass      ;
        HI_U32 s1_ch0_mmu_bypass       ;
        HI_U32 s1_ch1_mmu_bypass       ;
        HI_U32 s2_ch0_mmu_bypass       ;
        HI_U32 s2_ch1_mmu_bypass       ;
        HI_U32 g3_mmu_bypass           ;
        HI_U32 g4_mmu_bypass           ;
        HI_U32 g5_mmu_bypass           ;
        HI_U32 g6_mmu_bypass           ;
        HI_U32 g7_mmu_bypass           ;
        HI_U32 clut_mmu_bypass         ;
        HI_U32 s1_ch0_prot             ;
        HI_U32 s1_ch1_prot             ;
        HI_U32 s2_ch0_prot             ;
        HI_U32 s2_ch1_prot             ;
        HI_U32 g3_prot                 ;
        HI_U32 g4_prot                 ;
        HI_U32 g5_prot                 ;
        HI_U32 g6_prot                 ;
        HI_U32 g7_prot                 ;
        HI_U32 s1_scl_lh               ;
        HI_U32 s1_scl_lv               ;
        HI_U32 s1_scl_ch               ;
        HI_U32 s1_scl_cv               ;
        HI_U32 s2_scl_lh               ;
        HI_U32 s2_scl_lv               ;
        HI_U32 s2_scl_ch               ;
        HI_U32 s2_scl_cv               ;
        HI_U32 clut_addr               ;
    ////
    HI_U32 split_en;
}TDESplitCropCfg_S;



/* Zoom mode in subnode*/
typedef enum hiTDE_CHILD_SCALE_MODE_E
{
    TDE_CHILD_SCALE_NORM = 0,
    TDE_CHILD_SCALE_MBY,
    TDE_CHILD_SCALE_MBC,
    TDE_CHILD_SCALE_MB_CONCA_H,
    TDE_CHILD_SCALE_MB_CONCA_M,
    TDE_CHILD_SCALE_MB_CONCA_L,
    TDE_CHILD_SCALE_MB_CONCA_CUS,
} TDE_CHILD_SCALE_MODE_E;

/*  Info needed in MB format when Y/CbCr change */
typedef struct hiTDE_MBSTART_ADJ_INFO_S
{
    HI_U32 u32StartInX;     /* Start X,Y imported after MB adjust */
    HI_U32 u32StartInY;
    HI_U32 u32StartOutX;    /* Start X,Y exported after MB adjust */
    HI_U32 u32StartOutY;
    TDE_DRV_COLOR_FMT_E enFmt; /* color format, MB use it to renew position of Y and CbCr */
    TDE_CHILD_SCALE_MODE_E enScaleMode;
} TDE_MBSTART_ADJ_INFO_S;

/* Adjusting info when double source dispart */
typedef struct hiTDE_DOUBLESRC_ADJ_INFO_S
{
    HI_BOOL bDoubleSource;
    HI_S32 s32DiffX;    /*  s32DiffX = S1x - Tx         */
    HI_S32 s32DiffY;    /*  s32DiffY = S1y - Ty         */
}TDE_DOUBLESRC_ADJ_INFO_S;
/*
 * Configure info when set child node
 * u64Update :
 * _________________________________________
 * |    |    |    |    |    |    |    |    |
 * | ...| 0  | 0  | 1  | 1  | 1  | 1  |  1 |
 * |____|____|____|____|____|____|____|____|
 *                   |    |    |    |    |
 *                  \/   \/   \/   \/   \/
 *                u32Wo u32Xo HOfst u32Wi u32Xi
 *                u32Ho u32Yo VOfst u32Hi u32Yi
 */
typedef struct hiTDE_CHILD_INFO
{
    HI_U32 u32SliceWidth;
    HI_U32 u32SliceWi;
    HI_S32 s32SliceCOfst;
    HI_S32 s32SliceLOfst;
    HI_U32 u32SliceHoffset;
    HI_U32 u32SliceWo;
    HI_U32 u32SliceWHpzme;
    HI_U32 u32Xi;
    HI_U32 u32Yi;
    HI_U32 u32Wi;
    HI_U32 u32Hi;
    HI_U32 u32HOfst;
    HI_U32 u32VOfst;
    HI_U32 u32Xo;
    HI_U32 u32Yo;
    HI_U32 u32Wo;
    HI_U32 u32Ho;
    HI_U32 u32SliceDstWidth;
    HI_U32 u32SliceDstHeight;
    HI_U32 u32SliceDstHoffset;
    HI_U64 u64Update;
    TDE_MBSTART_ADJ_INFO_S stAdjInfo;
    TDE_DOUBLESRC_ADJ_INFO_S stDSAdjInfo;
    TDE_SLICE_TYPE_E enSliceType;
    HI_U32 des_crop_en;
    HI_U32 des_crop_start_x;
    HI_U32 des_crop_end_x;
} TDE_CHILD_INFO;

typedef struct hiTDE_DRV_COLORFILL_S
{
    TDE_DRV_COLOR_FMT_E enDrvColorFmt;
    HI_U32              u32FillData;
} TDE_DRV_COLORFILL_S;

typedef enum hiTDE_DRV_INT_E
{
	TDE_DRV_INT_lIST = 0x8,
	TDE_DRV_INT_NODE = 0x1,
 	TDE_DRV_INT_ERROR = 0x4,
 	TDE_DRV_INT_TIMEOUT = 0x2,
}TDE_DRV_INT_E;

typedef TDE2_OUTALPHA_FROM_E TDE_DRV_OUTALPHA_FROM_E;

typedef enum hiTDE_DRV_SRC_E
{
    TDE_DRV_SRC_NONE = 0,
    TDE_DRV_SRC_S1 = 0x1,
    TDE_DRV_SRC_S2 = 0x2,
    TDE_DRV_SRC_T = 0x4,
}TDE_DRV_SRC_E;



/****************************************************************************/
/*                             TDE register macro definition                            */
/****************************************************************************/
//#define TDE_RST 0x0804
#define TDE_CTRL 0x0500
#define TDE_INT 0x0504
#define TDE_INTCLR 0x0508
#define TDE_AQ_NADDR 0x04fc
#define TDE_STA 0x4000
#define TDE_AQ_ADDR 0x4004
#define TDE_MISCELLANEOUS 0x0514

//#define TDE_REQ_TH 0x0868
//#define TDE_AXI_ID 0x086c

#define TDE_AQ_COMP_NODE_MASK_EN 4 /*0100£ºEnable to interrupt when complete current node in AQ */
#define TDE_AQ_COMP_LIST_MASK_EN 8 /*1000£ºEnable to interrupt  in complete AQ */



/*Handle responsed with node */
/*
	Add 4 byte pointer in physical buffer header,to save software node;
	For need to consult current executing software node,but register can only
	give the physical addr of it.
*/
#define TDE_NODE_HEAD_BYTE 16

/* Next node addr¡¢update info¡¢occupied bytes */
#define TDE_NODE_TAIL_BYTE 12

/****************************************************************************/
/*                             TDE hal ctl functions define                 */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeHalInit
* Description:   main used in mapping TDE basic addr
* Input:         u32BaseAddr:Register basic addr
* Output:        None
* Return:        Success/Failure
* Others:        None
*****************************************************************************/
HI_S32  TdeHalInit(HI_U32 u32BaseAddr);

/*****************************************************************************
* Function:      TdeHalOpen
* Description:   main used in initialize needed register
* Input:         None
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalOpen(HI_VOID);
/*****************************************************************************
* Function:      TdeHalClose
* Description:  close the tde
* Input:         None
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalClose(HI_VOID);

/*****************************************************************************
* Function:      TdeHalRelease
* Description:   main used in release TDE basic addr by mapping
* Input:         None
* Output:        None
* Return:        Success/Failure
* Others:        None
*****************************************************************************/
HI_VOID TdeHalRelease(HI_VOID);

/*****************************************************************************
* Function:      TdeHalCtlIsIdle
* Description:   Query if TDE is in IDLE state or not
* Input:         None
* Output:        None
* Return:        True: Idle/False: Busy
* Others:        None
*****************************************************************************/
HI_BOOL TdeHalCtlIsIdle(HI_VOID);

/*****************************************************************************
* Function:      TdeHalCtlIsIdleSafely
* Description:   cycle many times, to make sure TDE is in IDLE state
* Input:         None
* Output:        None
* Return:        True: Idle/False: Busy
* Others:        None
*****************************************************************************/
HI_BOOL TdeHalCtlIsIdleSafely(HI_VOID);

/*****************************************************************************
* Function:      TdeHalCtlIntMask
* Description:   Get Sq/Aq interrupt state
* Input:         None
* Output:        None
* Return:        Sq/Aq interrupt state
* Others:        None
*****************************************************************************/
HI_U32  TdeHalCtlIntStats(HI_VOID);

/*****************************************************************************
* Function:      TdeHalCtlReset
* Description:   soft replace, reset interrupt state
* Input:         None
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalCtlReset(HI_VOID);

/*****************************************************************************
* Function:      TdeHalCtlIntClear
* Description:   Reset relevant interrupt state
* Input:         u32Stats: Reset relevant interrupt state
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalCtlIntClear(HI_U32 u32Stats);



/****************************************************************************/
/*                             TDE hal node functions define                */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeHalNodeInitNd
* Description:   Initialize struct, TDE operate node is needed
* Input:         pstHWNode:Node struct pointer.
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeInitNd(TDE_HWNode_S** pstHWNode);

/*****************************************************************************
* Function:      TdeHalFreeNodeBuf
* Description:   Free TDE operate node buffer
* Input:         pstHWNode:Node struct pointer.
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalFreeNodeBuf(TDE_HWNode_S* pstHWNode);

 HI_VOID TdeOsiCalcSrc1FilterOpt(TDE_HWNode_S* pNode, TDE2_COLOR_FMT_E enInFmt, TDE2_COLOR_FMT_E enOutFmt,
                                            TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect,TDE2_FILTER_MODE_E enFilterMode);

HI_VOID TdeOsiCalcSrc2FilterOpt(TDE_HWNode_S* pNode, TDE2_COLOR_FMT_E enInFmt, TDE2_COLOR_FMT_E enOutFmt,
                                            TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect,HI_BOOL bDefilicker,TDE2_FILTER_MODE_E enFilterMode);

/*****************************************************************************
* Function:      TdeHalNodeInitChildNd
* Description:   Initialize TDE child node
* Input:         pstHWNode:Node struct pointer.
                     u32TDE_CLIP_START:The start position of the clip rect.
                     u32TDE_CLIP_STOP:The stop position of the clip rect
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeInitChildNd(TDE_HWNode_S* pHWNode, HI_U32 u32TDE_CLIP_START, HI_U32 u32TDE_CLIP_STOP);
/*****************************************************************************
* Function:      TdeHalNodeEnableCompleteInt
* Description:   Complete interrupt by using node's operate
* Input:         pBuf: Buffer need node be operated
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeEnableCompleteInt(HI_VOID* pBuf);



/*****************************************************************************
* Function:      TdeHalNodeSetSrc1
* Description:   Set Src1 bitmap info
* Input:         pHWNode: Node struct pointer used in cache by software
*                pDrvSurface: bitmap info used in setting
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetSrc1(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface);

/*****************************************************************************
* Function:      TdeHalNodeSetSrc2
* Description:   Set Src2 bitmap info
* Input:         pHWNode: Node struct pointer used in cache by software
*                pDrvSurface: bitmap info used in setting
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetSrc2(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface);

/*****************************************************************************
* Function:      TdeHalNodeSetSrcMbY
* Description:   Set brightness information in MB source bitmap
* Input:         pHWNode: Node struct pointer used in cache by software
*                pDrvMbY: bitmap brightness information used in setting
*                enMbOpt: MB mode option
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetSrcMbY(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvMbY, TDE_DRV_MB_OPT_MODE_E enMbOpt);

/*****************************************************************************
* Function:      TdeHalNodeSetSrcMbCbCr
* Description:   set chroma info in MB source bitmap
* Input:         pHWNode: Node struct pointer used in cache by software
*                pDrvMbCbCr: CbCr info
*                enMbOpt: MB mode option
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetSrcMbCbCr(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvMbCbCr, TDE_DRV_MB_OPT_MODE_E enMbOpt);

/*****************************************************************************
* Function:      TdeHalNodeSetTgt
* Description:   Set target bitmap information
* Input:         pHWNode: Node struct pointer used in cache by software
*                pDrvSurface: bitmap information used in setting
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetTgt(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface, TDE_DRV_OUTALPHA_FROM_E enAlphaFrom);

/*****************************************************************************
* Function:      TdeHalNodeSetBaseOperate
* Description:   Set basic operate type
* Input:         pHWNode: Node struct pointer used in cache by software
*                enMode: basic operate mode
*                enAlu: ALU mode
*                u32FillData: if basic mode have fill operate ,read this value
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetBaseOperate(TDE_HWNode_S* pHWNode, TDE_DRV_BASEOPT_MODE_E enMode,
                                 TDE_DRV_ALU_MODE_E enAlu, TDE_DRV_COLORFILL_S *pstColorFill);

/*****************************************************************************
* Function:      TdeHalNodeSetGlobalAlpha
* Description:   Set Alpha mixed arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                u8Alpha: Alpha mixed setting value
*                       bEnable: Enable to use global alpha
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetGlobalAlpha(TDE_HWNode_S* pHWNode, HI_U8 u8Alpha, HI_BOOL bEnable);

HI_VOID TdeHalNodeSetSrc1Alpha(TDE_HWNode_S* pHWNode);

HI_VOID TdeHalNodeSetSrc2Alpha(TDE_HWNode_S* pHWNode);

/*****************************************************************************
* Function:      TdeHalNodeSetExpAlpha
* Description:   When expand Alpha in RGB5551. to alpha0 and alpha1
* Input:         pHWNode: Node struct pointer used in cache by software
*                u8Alpha: Alpha mixed setting value
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetExpAlpha(TDE_HWNode_S* pHWNode, TDE_DRV_SRC_E enSrc, HI_U8 u8Alpha0, HI_U8 u8Alpha1);
/*****************************************************************************
* Function:      TdeHalNodeSetAlphaBorder
* Description:   Enable to set Alpha can be bordered
* Input:         pHWNode: Node struct pointer used in cache by software
*                bEnable: Enanle to border Alpha
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetAlphaBorder(TDE_HWNode_S* pHWNode, HI_BOOL bVEnable, HI_BOOL bHEnable);

/*****************************************************************************
* Function:      TdeHalNodeSetRop
* Description:   Set ROP arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                enRopCode: ROP operator
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetRop(TDE_HWNode_S* pHWNode, TDE2_ROP_CODE_E enRgbRop, TDE2_ROP_CODE_E enAlphaRop);

/*****************************************************************************
* Function:      TdeHalNodeSetBlend
* Description:   Set blend operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                stBlendOpt:blend operate option
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetBlend(TDE_HWNode_S *pHWNode, TDE2_BLEND_OPT_S *pstBlendOpt);

/*****************************************************************************
* Function:      TdeHalNodeSetColorize
* Description:   Set blend operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                u32Colorize:Co
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetColorize(TDE_HWNode_S *pHWNode, HI_U32 u32Colorize);

/*****************************************************************************
* Function:      TdeHalNodeEnableAlphaRop
* Description:   Enable to blend Rop operate
* Input:         pHWNode: Node struct pointer used in cache by software
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeEnableAlphaRop(TDE_HWNode_S *pHWNode);

/*****************************************************************************
* Function:      TdeHalNodeSetColorExp
* Description:   Set color expand or adjust argument
* Input:         pHWNode: Node struct pointer used in cache by software
*                pClutCmd: Clut operate atguments
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetClutOpt(TDE_HWNode_S* pHWNode, TDE_DRV_CLUT_CMD_S* pClutCmd, HI_BOOL bReload);

/*****************************************************************************
* Function:      TdeHalNodeSetColorKey
* Description:   Set arguments needed by color key,according current color format
* Input:         pHWNode: Node struct pointer used in cache by software
*                enFmt: color format
*                pColorKey: color key pointer
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetColorKey(TDE_HWNode_S* pHWNode, TDE_COLORFMT_CATEGORY_E enFmtCat,
                              TDE_DRV_COLORKEY_CMD_S* pColorKey);

/*****************************************************************************
* Function:      TdeHalNodeSetClipping
* Description:   Set rectangle's clip operated arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pClip: Clip rectangle range
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetClipping(TDE_HWNode_S* pHWNode, TDE_DRV_CLIP_CMD_S* pClip);

/*****************************************************************************
* Function:      TdeHalNodeSetResize
* Description:   set zoom fliter operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pResize: zoom coefficient
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
//HI_S32 TdeHalNodeSetResize(TDE_HWNode_S* pHWNode, TDE_FILTER_OPT* pstFilterOpt, TDE_NODE_SUBM_TYPE_E enNodeType);
/*****************************************************************************
* Function:      TdeHalNodeSetColorConvert
* Description:   set color zone conversion arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pConv: color zone conversion argument
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/

//HI_S32 TdeHalNodeSetSrc1Resize(TDE_HWNode_S* pHWNode, TDE_FILTER_OPT* pstFilterOpt, TDE_NODE_SUBM_TYPE_E enNodeType);

HI_S32 TdeHalNodeSetColorConvert(TDE_HWNode_S* pHWNode, TDE_DRV_CONV_MODE_CMD_S* pConv);



/*****************************************************************************
* Function:      TdeHalNodeSetCsc
* Description:  Set CSC's first optional argument
* Input:         pHWNode:Node struct pointer used in cache by software
			stCscOpt:CSC first optional argument
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetCsc(TDE_HWNode_S* pHWNode, TDE2_CSC_OPT_S stCscOpt);


/*****************************************************************************
* Function:      TdeHalSetDeflicerLevel
* Description:   set deflicker level
* Input:         eDeflickerLevel:deflicker level
* Output:        None
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalSetDeflicerLevel(TDE_DEFLICKER_LEVEL_E eDeflickerLevel);

/*****************************************************************************
* Function:      TdeHalGetDeflicerLevel
* Description:   Get deflicker level
* Input:
* Output:        pDeflicerLevel:deflicker level
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalGetDeflicerLevel(TDE_DEFLICKER_LEVEL_E *pDeflicerLevel);

/*****************************************************************************
* Function:      TdeHalSetAlphaThreshold
* Description:   Set alpha threshold
* Input:         u8ThresholdValue:alpha threshold
* Output:
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalSetAlphaThreshold(HI_U8 u8ThresholdValue);

/*****************************************************************************
* Function:      TdeHalGetAlphaThreshold
* Description:   Get alpha threshold
* Input:
* Output:        pu8ThresholdValue:alpha threshold
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalGetAlphaThreshold(HI_U8 * pu8ThresholdValue);

/*****************************************************************************
* Function:      TdeHalGetAlphaThresholdState
* Description:   Set alpha threshold to judge if open or close
* Input:         bEnAlphaThreshold:alpha switch status
* Output:        None
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalSetAlphaThresholdState(HI_BOOL bEnAlphaThreshold);

/*****************************************************************************
* Function:      TdeHalGetAlphaThresholdState
* Description:   Get alpha threshold to judge if open or close
* Input:         None
* Output:        pbEnAlphaThreshold:alpha switch status
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalGetAlphaThresholdState(HI_BOOL *pbEnAlphaThreshold);

/*****************************************************************************
* Function:      TdeHalNodeExecute
* Description:   Start TDE list
* Input:
*                u32NodePhyAddr: list first node address
*                u64Update: first node updating flag
*                bAqUseBuff: if use temporary buffer
* Output:        None
* Return:        Success / Fail
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeExecute(HI_U32 u32NodePhyAddr, HI_U64 u64Update, HI_BOOL bAqUseBuff);


/*****************************************************************************
* Function:      TdeHalCurNode
* Description:   Get current node in register
* Input:         None
* Output:        node physical address
* Return:        None
* Others:        None
*****************************************************************************/
HI_U32 TdeHalCurNode(HI_VOID);

HI_U32 TdeHalFinishedJobID(HI_VOID);
/*****************************************************************************
* Function:      TdeHalResumeInit
* Description:   Resume the hardware by software ,initialize the TDE device
* Input:         None
* Output:        node
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalResumeInit(HI_VOID);

/*****************************************************************************
* Function:      TdeHalSuspend
* Description:   Suspend the hardware
* Input:         None
* Output:        node
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalSuspend(HI_VOID);

HI_VOID TdeHalSetClock(HI_BOOL bEnable);

#ifdef CONFIG_GFX_MMU_SUPPORT
HI_VOID TdeHalFreeTmpBuf(TDE_HWNode_S* pstHWNode);
#endif
struct seq_file * TdeHalNodePrintInfo(struct seq_file *p,HI_U32 *pu32CurNode);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _TDE_HAL_H_ */
