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
2014/06/20                                 Created file
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
    TDE_DRV_COLOR_FMT_RGB444   = 0,
    TDE_DRV_COLOR_FMT_RGB555   = 1,
    TDE_DRV_COLOR_FMT_RGB565   = 2,
    TDE_DRV_COLOR_FMT_RGB888   = 3,
    TDE_DRV_COLOR_FMT_ARGB4444 = 4,
    TDE_DRV_COLOR_FMT_ARGB1555 = 5,
    TDE_DRV_COLOR_FMT_ARGB8565 = 6,
    TDE_DRV_COLOR_FMT_ARGB8888 = 7,
    TDE_DRV_COLOR_FMT_CLUT1   = 8,
    TDE_DRV_COLOR_FMT_CLUT2   = 9,
    TDE_DRV_COLOR_FMT_CLUT4   = 10,
    TDE_DRV_COLOR_FMT_CLUT8   = 11,
    TDE_DRV_COLOR_FMT_ACLUT44 = 12,
    TDE_DRV_COLOR_FMT_ACLUT88 = 13,
    TDE_DRV_COLOR_FMT_A1 = 16,
    TDE_DRV_COLOR_FMT_A8 = 17,
    TDE_DRV_COLOR_FMT_YCbCr888   = 18,
    TDE_DRV_COLOR_FMT_AYCbCr8888 = 19,
    TDE_DRV_COLOR_FMT_YCbCr422 = 20,
    TDE_DRV_COLOR_FMT_byte = 22,
    TDE_DRV_COLOR_FMT_halfword = 23,
    TDE_DRV_COLOR_FMT_YCbCr400MBP = 24,
    TDE_DRV_COLOR_FMT_YCbCr422MBH = 25,
    TDE_DRV_COLOR_FMT_YCbCr422MBV = 26,
    TDE_DRV_COLOR_FMT_YCbCr420MB = 27,
    TDE_DRV_COLOR_FMT_YCbCr444MB = 28,
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
    TDE_DRV_FILTER_COLOR,       /* filt on color parameter */
    TDE_DRV_FILTER_ALPHA,       /* filt on Alpha value */
    TDE_DRV_FILTER_ALL          /* filt on Alpha and color value */
} TDE_DRV_FILTER_MODE_E;

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

    /* Vertical scanning direction */
    TDE_DRV_VSCAN_E enVScan;

    /* Horizontal scanning direction */
    TDE_DRV_HSCAN_E enHScan;

    /* alpha max value is 255?or 128? */
    HI_BOOL bAlphaMax255;

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
    TDE_ALU_ROP,
    TDE_ALU_BLEND,
    TDE_ALU_BLEND_SRC2,
    TDE_ALU_MASK_ROP1,
    TDE_ALU_MASK_BLEND,
    TDE_ALU_CONCA,
    TDE_SRC2_BYPASS,
    TDE_ALU_MASK_ROP2,
    TDE_ALU_NONE 		/* register has no setting, used in flag */
} TDE_DRV_ALU_MODE_E;


/* cofigure info of node,by TDE_UPDATE order */
typedef struct hiTDE_HWNode_S
{
    HI_U32 u32TDE_INS;
    HI_U32 u32TDE_S1_ADDR;
    HI_U32 u32TDE_S1_TYPE;
    HI_U32 u32TDE_S1_XY;
    HI_U32 u32TDE_S1_FILL;
    HI_U32 u32TDE_S2_ADDR;
    HI_U32 u32TDE_S2_TYPE;
    HI_U32 u32TDE_S2_XY;
    HI_U32 u32TDE_S2_SIZE;
    HI_U32 u32TDE_S2_FILL;
    HI_U32 u32TDE_TAR_ADDR;
    HI_U32 u32TDE_TAR_TYPE;
    HI_U32 u32TDE_TAR_XY;
    HI_U32 u32TDE_TS_SIZE;
    HI_U32 u32TDE_COLOR_CONV;
    HI_U32 u32TDE_CLUT_ADDR;
    HI_U32 u32TDE_2D_RSZ;
    HI_U32 u32TDE_HF_COEF_ADDR;
    HI_U32 u32TDE_VF_COEF_ADDR;
    HI_U32 u32TDE_RSZ_STEP;
    HI_U32 u32TDE_RSZ_Y_OFST;
    HI_U32 u32TDE_RSZ_X_OFST;
    HI_U32 u32TDE_DFE_COEF0;
    HI_U32 u32TDE_DFE_COEF1;
    HI_U32 u32TDE_DFE_COEF2;
    HI_U32 u32TDE_DFE_COEF3;
    HI_U32 u32TDE_ALU;
    HI_U32 u32TDE_CK_MIN;
    HI_U32 u32TDE_CK_MAX;
    HI_U32 u32TDE_CLIP_START;
    HI_U32 u32TDE_CLIP_STOP;
    HI_U32 u32TDE_Y1_ADDR;
    HI_U32 u32TDE_Y1_PITCH;
    HI_U32 u32TDE_Y2_ADDR;
    HI_U32 u32TDE_Y2_PITCH;
    HI_U32 u32TDE_RSZ_VSTEP;
    HI_U32 u32TDE_ARGB_ORDER;
    HI_U32 u32TDE_CK_MASK;
    HI_U32 u32TDE_COLORIZE;
    HI_U32 u32TDE_ALPHA_BLEND;
    HI_U32 u32TDE_ICSC_ADDR;
    HI_U32 u32TDE_OCSC_ADDR;
} TDE_HWNode_S;

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
    HI_U64 u64Update;
    TDE_MBSTART_ADJ_INFO_S stAdjInfo;
    TDE_DOUBLESRC_ADJ_INFO_S stDSAdjInfo;
    TDE_SLICE_TYPE_E enSliceType;
} TDE_CHILD_INFO;

typedef struct hiTDE_DRV_COLORFILL_S
{
    TDE_DRV_COLOR_FMT_E enDrvColorFmt;
    HI_U32              u32FillData;
} TDE_DRV_COLORFILL_S;

typedef enum hiTDE_DRV_INT_E
{
	TDE_DRV_INT_lIST = 0x10000,
	TDE_DRV_INT_NODE = 0x20000,
 	TDE_DRV_INT_ERROR = 0x80000000,
}TDE_DRV_INT_E;

typedef TDE2_OUTALPHA_FROM_E TDE_DRV_OUTALPHA_FROM_E;

typedef enum hiTDE_DRV_SRC_E
{
    TDE_DRV_SRC_NONE = 0,
    TDE_DRV_SRC_S1 = 0x1,
    TDE_DRV_SRC_S2 = 0x2,
    TDE_DRV_SRC_T = 0x4,
}TDE_DRV_SRC_E;

typedef struct hiTDE_FILTER_OPT
{
    HI_U32  u32HStep;
    HI_U32  u32VStep;
    HI_S32  s32HOffset;
    HI_S32  s32VOffset;
    HI_U32  u32Bppi;
    HI_U32  u32WorkBufNum;
    HI_U32  bBadLastPix;        /* while blocking, last point of each block is if effective*/
    HI_BOOL bVRing;
    HI_BOOL bHRing;
    HI_BOOL bEvenStartInX;      /* when input need up_sample, bEvenStartInX is set to HI_TRUE */
    HI_BOOL bEvenStartOutX;     /* when input need drop_sample, bEvenStartInX is set to HI_TRUE */
    HI_BOOL bCoefSym;           /* coefficient is if symmetrical, use filed ground */
    HI_BOOL b2OptCbCr;
    TDE_SCANDIRECTION_S stSrcDire;
    TDE_SCANDIRECTION_S stDstDire;
    TDE_MBSTART_ADJ_INFO_S stAdjInfo;
    TDE_DOUBLESRC_ADJ_INFO_S stDSAdjInfo;
    TDE_DRV_FILTER_MODE_E enFilterMode;
    HI_BOOL bFirstLineOut;
    HI_BOOL bLastLineOut;
} TDE_FILTER_OPT;


/****************************************************************************/
/*                             TDE register macro definition                            */
/****************************************************************************/
#define TDE_RST 0x0804
#define TDE_CTRL 0x0808
#define TDE_AQ_ADDR 0x0810
#define TDE_STA 0x0814
#define TDE_INT 0x0818
#define TDE_AQ_CTRL 0x082C
#define TDE_AQ_NADDR 0x0830
#define TDE_AQ_UPDATE 0x0834
#define TDE_BUS_LIMITER 0x0844

#define TDE_AQ_UPDATE2 0x085c
#define TDE_REQ_TH 0x0868
#define TDE_AXI_ID 0x086c

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
* Function:      TdeHalNodeSetFlicker
* Description:   set deflicker fliter operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pFlicker: Deflicker coefficient
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetFlicker(TDE_HWNode_S* pHWNode, TDE_DRV_FLICKER_CMD_S* pFlicker);

/*****************************************************************************
* Function:      TdeHalNodeSetResize
* Description:   set zoom fliter operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pResize: zoom coefficient
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetResize(TDE_HWNode_S* pHWNode,TDE_FILTER_OPT* pstFilterOpt, TDE_NODE_SUBM_TYPE_E enNodeType);
/*****************************************************************************
* Function:      TdeHalNodeSetColorConvert
* Description:   set color zone conversion arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pConv: color zone conversion argument
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_S32 TdeHalNodeSetColorConvert(TDE_HWNode_S* pHWNode, TDE_DRV_CONV_MODE_CMD_S* pConv);

/*****************************************************************************
* Function:      TdeHalNodeAddChild
* Description:   Add child node when filter operate
* Input:         pHWNode: Node struct pointer used in cache by software
*                pChildInfo: add child node's configue information
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeAddChild(TDE_HWNode_S* pHWNode,TDE_CHILD_INFO* pChildInfo);

/*****************************************************************************
* Function:      TdeHalNodeSetMbMode
* Description:   set MB operate arguments
* Input:         pHWNode: Node struct pointer used in cache by software
*                pMbCmd: MB operate arguments
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalNodeSetMbMode(TDE_HWNode_S* pHWNode, TDE_DRV_MB_CMD_S* pMbCmd);


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
#ifndef HI_BUILD_IN_BOOT
HI_VOID TdeHalNodePrintInfo(struct seq_file *p,HI_U32 *pu32CurNode);
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _TDE_HAL_H_ */
