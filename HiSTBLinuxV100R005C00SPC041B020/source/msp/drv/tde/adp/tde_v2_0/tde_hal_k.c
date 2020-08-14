/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_hal_k.c
Version             : Initial Draft
Author              :
Created             : 2015/07/11
Description         :
Function List       :
History             :
Date                            Author                  Modification
2015/07/11                                              Created file
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <asm/barrier.h>
#else
#include "higo_list.h"
#endif
#include "tde_hal.h"
#include "tde_define.h"
#include "wmalloc.h"

#include "hi_common.h"
#include "hi_reg_common.h"
#include "tde_filterPara.h"
#include "tde_adp.h"
#include "hi_gfx_sys_k.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */
/****************************************************************************/
/*                   TDE hal inner macro definition                         */
/****************************************************************************/

/* compute struct member's offset */
#define TDE_OFFSET_OF(type, member) ((HI_U32) (&((type *)0)->member))

#define UPDATE_SIZE 64

#define NO_HSCALE_STEP 0x100000
#define NO_VSCALE_STEP 0x1000

/* Judge if Update flag is 1 or not on current configure */
#define TDE_MASK_UPDATE(member) (1 << ((TDE_OFFSET_OF(TDE_HWNode_S, member) >> 2) % (UPDATE_SIZE)))

extern TDE_DRV_COLOR_FMT_E g_enTdeCommonDrvColorFmt[TDE2_COLOR_FMT_BUTT + 1];
#define TDE_FILL_DATA_BY_FMT(fill, data, fmt)\
do{\
    HI_U32 u32Bpp;\
    u32Bpp = TdeHalGetbppByFmt(fmt);\
    if (0xffffffff != u32Bpp)\
    {\
        HI_U32 i;\
        HI_U32 u32Cell;\
        u32Cell = data & (0xffffffff >> (32 - u32Bpp));\
        for (i = 0; i < (32 / u32Bpp); i++)\
        {\
            fill |= (u32Cell << (i * u32Bpp));\
        }\
    }\
    else\
    {\
        fill = data;\
    }\
}while(0)

/* Get member pointer in node buffer according to current member and update flag
   Return value pRet is null is to say current buffer has not needed member */
#define TDE_GET_MEMBER_IN_BUFF(pRet, member, pstVirBuf, update)\
do{\
    HI_S32 s32Offset;\
    \
    s32Offset = TdeHalGetOffsetInNode(TDE_MASK_UPDATE(member), update);\
    if (-1 != s32Offset )\
    {\
        pRet = pstVirBuf + s32Offset;\
    }\
    else\
    {\
        pRet = HI_NULL;\
    }\
}while(0)



#define TDE_GET_MEMBER_IN_BUFNODE(pRet, member, pstNdBuf) \
    TDE_GET_MEMBER_IN_BUFF(pRet, member, (pstNdBuf)->pu32VirAddr, (pstNdBuf)->u32CurUpdate)

/* R/W register's encapsulation */
#define TDE_READ_REG(base, offset)  (*(volatile unsigned int *)((void *)(base) + (offset)))
#define TDE_WRITE_REG(base, offset, val)  (*(volatile unsigned int *)((void *)(base) + (offset)) = (val))

/* TDE register's Address range */
#define TDE_REG_SIZE 0x40a4

/* Step range's type by algorithm team */
#define TDE_RESIZE_PARA_AREA_0  4096     /*1.0*/
#define TDE_RESIZE_PARA_AREA_1  5461      /*0.75*/
#define TDE_RESIZE_PARA_AREA_2  8192     /*0.5*/
#define TDE_RESIZE_PARA_AREA_3  12412     /*0.33*/
#define TDE_RESIZE_PARA_AREA_4  16384     /*0.25*/

/* coefficient table rangle of 8*32 area */
#define TDE_RESIZE_8X32_AREA_0  1048576     /*1.0*/
#define TDE_RESIZE_8X32_AREA_1  1398101    /*0.75*/
#define TDE_RESIZE_8X32_AREA_2  2097152     /*0.5*/
#define TDE_RESIZE_8X32_AREA_3  3177503    /*0.33*/
#define TDE_RESIZE_8X32_AREA_4  4194304   /*0.25*/

/*Colorkey mode of CMOS chip */
#define TDE_COLORKEY_IGNORE     2
#define TDE_COLORKEY_AREA_OUT   1
#define TDE_COLORKEY_AREA_IN    0

/* Aq control mode */
#define TDE_AQ_CTRL_COMP_LIST   0x0 /* start next AQ list, after complete current list's operations */
#define TDE_AQ_CTRL_COMP_LINE   0x4 /* start next AQ list, after complete current node and line */

#define TDE_MAX_READ_STATUS_TIME 10


#define TDE_HAL_TWO_BIT_SHIFT    2
#define TDE_HAL_ONE_BIT_SHIFT    1
#define TDE_HAL_ZERO_BIT_SHIFT   0

#define TDE_TWENTYFOUR_BITS_SHIFT  24
#define TDE_EIGHT_BITS_SHIFT       8
#define TDE_SIXTEEN_BITS_SHIFT     16

#define TDE_HAL_CHECK_BLEND_CMD(SetBlendCmd)\
    if ( (SetBlendCmd < TDE2_BLENDCMD_NONE) || (SetBlendCmd > TDE2_BLENDCMD_BUTT) )\
    {\
        return HI_ERR_TDE_INVALID_PARA;\
    }

/****************************************************************************/
/*                        TDE hal inner type definition                     */
/****************************************************************************/
/* source bitmap running mode */
typedef enum hiTDE_SRC_MODE_E
{
    TDE_SRC_MODE_DISA  = 0,
    TDE_SRC_MODE_BMP   = 1,
    TDE_SRC_MODE_PATTERN = 2,
    TDE_SRC_MODE_FILL  = 3,
    TDE_SRC_QUICK_COPY = 5,
    TDE_SRC_QUICK_FILL = 7
}TDE_SRC_MODE_E;


/* Aq control register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_AQ_CTRL_U
{
    struct
    {
        HI_U32 u32Reserve1     :20; /* Reserve 1 */
        HI_U32 u32AqOperMode   :4;  /* Resume open mode in AQ list */
        HI_U32 u32AqEn         :1;  /* Enable AQ */
        HI_U32 u32Reserve2     :7;  /* Reserve 2 */
    }stBits;
    HI_U32 u32All;
}TDE_AQ_CTRL_U;

/* Operate manner register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_INS_U
{
    struct
    {
        HI_U32 u32Src1Mod       :3;     /* source1 runnning mode */
        HI_U32 u32Src2Mod       :2;     /* source2 runnning mode */
        HI_U32 u32IcsConv       :1;     /* Enable to conversion of importing color zone */
        HI_U32 u32Resize        :1;     /* Enable to reszie on 2D*/
        HI_U32 u32DfeEn         :1;     /* Enable to Deflicker flicter */
		HI_U32 u32Y1En		:1;		    /* Enable Y1 channel */
		HI_U32 u32Y2En		:1;		    /* Enable Y2 channel */
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
		HI_U32 u32CasProt       :1;
#else
        HI_U32 u32Reserve1      :1;     /* Reserve 1 */
#endif
        HI_U32 u32Clut          :1;     /* Enable CLUT */
        HI_U32 u32ColorKey      :1;     /* Enable Colorkey */
        HI_U32 u32IcscSel      :1;      /* Select in importing color zone conversion */
        HI_U32 u32Clip          :1;     /* Enable Clip */
        HI_U32 u32MbMode        :3;     /* MB operate mode */
        HI_U32 u32MbEn          :1;     /* Enable to operate MB */
        HI_U32 u32AqIrqMask     :4;     /* Fake code of interrupt in AQ lsit */
        HI_U32 u32SqIrqMask     :5;     /* Fake code of interrupt in SQ lsit */
        HI_U32 u32OcsConv      :1;      /* Enable to conversion of exporting color zone */
        HI_U32 u32BlockFlag     :1;     /* Flag if block */
        HI_U32 u32FirstBlockFlag    :1; /* Flag that is block at first */
        HI_U32 u32LastBlockFlag     :1; /* Flag that is block last */
    }stBits;
    HI_U32 u32All;
}TDE_INS_U;

/* Source bitmap register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_SRC_TYPE_U
{
    struct
    {
        HI_U32 u32Pitch        :16; /* line pitch */
        HI_U32 u32SrcColorFmt  :5;  /* pixel format */
        HI_U32 u32AlphaRange   :1;  /* 8bit alpha range */
        HI_U32 u32ColorizeEnable:1; /* Enable Colorize,only effective for Src2, Src1 reserved */
        HI_U32 u32Reserve1     :1;  /* Reserve */
        HI_U32 u32HScanOrd     :1;  /* Horizontal scanning direction */
        HI_U32 u32VScanOrd     :1;  /* Vertical scanning direction */
        HI_U32 u32Reserve2     :3;  /* Reserve */
        HI_U32 u32RgbExp       :1;  /* RGB expand mode */
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
		HI_U32 u32Reserve3     :1;  /* Reserve */
		HI_U32 u32MmuBypass	   :1;
#else
        HI_U32 u32Reserve3     :2;  /* Reserve */
#endif
    }stBits;
    HI_U32 u32All;
}TDE_SRC_TYPE_U;

/* Register's Config format at section start */
typedef union hiTDE_SUR_XY_U
{
    struct
    {
        HI_U32 u32X            :12; /* First X coordinate */
        HI_U32 u32Reserve1     :4;  /* Reserve */
        HI_U32 u32Y            :12; /* First Y coordinate */
        HI_U32 u32Reserve2     :4;  /* Reserve */
    }stBits;
    HI_U32 u32All;
}TDE_SUR_XY_U;

/* Bitmap size register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_SUR_SIZE_U
{
    struct
    {
        HI_U32 u32Width        :12;             /* Image width */
        HI_U32 u32AlphaThresholdLow     :4;     /* The lowest 4 bit of alpha threshold, effective in size register of target, reserved in src2 size register */
        HI_U32 u32Height       :12;             /* Image height */
        HI_U32 u32AlphaThresholdHigh     :4;    /* The highest 4 bit of alpha threshold, effective in size register of target, reserved in src2 size register */
    }stBits;
    HI_U32 u32All;
}TDE_SUR_SIZE_U;

/* Target bitmap type register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_TAR_TYPE_U
{
    struct
    {
        HI_U32 u32Pitch        :16;     /* line pitch */
        HI_U32 u32TarColorFmt  :5;      /* pixel format */
        HI_U32 u32AlphaRange   :1;      /* 8bit alpha range */
        HI_U32 u32AlphaFrom    :2;      /* Emporting alpha from */
        HI_U32 u32HScanOrd     :1;      /* Horizontal scanning direction */
        HI_U32 u32VScanOrd     :1;      /* Vertical scaning direction */
        HI_U32 u32RgbRound     :1;      /* RGB truncate bit mode */
        HI_U32 u32DfeLastlineOutEn  :1; /* Emporting enable to deflicker at last line */
        HI_U32 u32DfeFirstlineOutEn :1; /* Emporting enable to deflicker at first line */
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
        HI_U32 u32Reserve2     :2;      /* Reserve */
		HI_U32 u32MmuBypass	   :1;
#else
		HI_U32 u32Reserve2     :3;      /* Reserve */
#endif
    }stBits;
    HI_U32 u32All;
}TDE_TAR_TYPE_U;

/* Color zone conversion register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_COLOR_CONV_U
{
    struct
    {
        HI_U32 u32InColorImetry       :1;   /* Import color conversion standard  */
        HI_U32 u32InRgb2Yc       :1;        /* Import color conversion oreitation */
        HI_U32 u32InColorSpace  :1;         /* Import matrix of color conversion used */
        HI_U32 u32InChromaFmt   :1;         /* Import chroma format */
        HI_U32 u32OutColorImetry    :1;     /* Export color conversion standard */
        HI_U32 u32OutColorSpace     :1;     /* Export matrix of color conversion */
        HI_U32 u32OutChromaFmt      :1;     /* Export chroma format */
        HI_U32 u32Reserve1       :2;        /* Reserve */
        HI_U32 u32ClutMod        :1;        /* clut mode */
        HI_U32 u32ClutReload     :1;        /* Enable to update clut table */
        HI_U32 u32Reserve2       :1;        /* Reserve */
        HI_U32 u32OcscReload	:1;	        /* Enable to reload exported CSC self-defined arguments */
        HI_U32 u32OcscCustomEn	:1;	        /* Enable to self-defined export CSC */
        HI_U32 u32IcscReload	:1;	        /* Enable to reload inported CSC self-defined arguments */
        HI_U32 u32IcscCustomEn	:1;	        /* Enable to self-defined inported CSC */
        HI_U32 u32Alpha0         :8;        /* alpha1555 format, 0:alpha value */
        HI_U32 u32Alpha1         :8;        /* alpha1555 format, 1:alpha value */
    }stBits;
    HI_U32 u32All;
}TDE_COLOR_CONV_U;

/* Zoom register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_2D_RSZ_U
{
    struct
    {
        HI_U32 u32HfMod        :2;      /* Horizontal filter mode */
        HI_U32 u32Reserve1     :2;      /* Reserve */
        HI_U32 u32VfMod        :2;      /* Vertical filter mode */
        HI_U32 u32Reserve2     :2;      /* Reserve */
        HI_U32 u32DfeMod       :2;      /* Select in deflicker coefficient */
        HI_U32 u32Reserve3     :2;      /* Reserve */
        HI_U32 u32AlpBorder    :2;      /* alpha border handling mode */
        HI_U32 u32Reserve4     :2;      /* Reserve */
        HI_U32 u32VfRingEn     :1;      /* Enable to median filter,when vertical filter enable */
        HI_U32 u32HfRingEn     :1;      /* Enable to median filter,when horizontal filter enable */
        HI_U32 u32CoefSym      :1;      /* Property of filter coefficient */
        HI_U32 u32Reserve5     :5;      /* Reserve */
        HI_U32 u32VfCoefReload :1;      /* Enable to update vertical filter coefficient */
        HI_U32 u32HfCoefReload :1;      /* Enable to update horizontal filter coefficient */
        HI_U32 u32DfeAlphaDisable   :1; /* Unable to alpha deflicker */
        HI_U32 u32Reserve6     :5;      /* Reserve */
    }stBits;
    HI_U32 u32All;
}TDE_2D_RSZ_U;

/* Bitmap size register's configured format, idiographic meanings can refer to register's handbook */
typedef union hiTDE_ALU_U
{
    struct
    {
        HI_U32 u32AluMod        :4;     /* ALU running mode */
        HI_U32 u32AlphaThreshodEn  :1;  /* Enable to threshold alpha */
        HI_U32 u32AlphaRopMod   :4;     /* Rop operate mode of alpha */
        HI_U32 u32RgbRopMod     :4;     /* Rop operate mode of RGB */
        HI_U32 u32GlobalAlpha   :8;     /* Global alpha value */
        HI_U32 u32CkBMod        :2;     /* Colorkey blue component operating mode */
        HI_U32 u32CkGMod        :2;     /* Colorkey green component operating mode */
        HI_U32 u32CkRMod        :2;     /* Colorkey red component operating mode */
        HI_U32 u32CkAMod        :2;     /* Colorkey alpha component operating mode */
        HI_U32 u32CkSel         :2;     /* Colorkey inporting selection */
        HI_U32 u32BlendReverse      :1; /* Enable to reserve blending fore and background */
    }stBits;
    HI_U32 u32All;
}TDE_ALU_U;

/* ARGB/AYUV component order register format */
typedef union hiTDE_ARGB_ORDER_U
{
    struct
    {
        HI_U32 u32Src1ArgbOrder :   5;  /* Src1's ARGB order */
        HI_U32 u32Reserved1 :   3;      /* Reserve */
        HI_U32 u32Src2ArgbOrder :   5;  /* Src2's ARGB order */
        HI_U32 u32Reserved2 :   3;      /* Reserve */
        HI_U32 u32TarArgbOrder  :   5;  /* Target's ARGB order */
        HI_U32 u32Reserved3 :   3;      /* Reserve */
    }stBits;
    HI_U32 u32All;
}TDE_ARGB_ORDER_U;

/* Colorkey Mask Register */
typedef union hiTDE_COLORKEY_MASK_U
{
    struct
    {
        HI_U32 u32BlueMask  :   8;  /* Blue component /clut/ V component mask */
        HI_U32 u32GreenMask :   8;  /* Green component / U component mask */
        HI_U32 u32RedMask   :   8;  /* Red component /Y component mask */
        HI_U32 u32AlphaMask :   8;  /* Alpha component mask */
    }stBits;
    HI_U32 u32All;
}TDE_COLORKEY_MASK_U;

/* alpha blend mode register */
typedef union hiTDE2_ALPHA_BLEND_U
{
    struct
    {
        HI_U32 u32Src1BlendMode :       4;      /* Src1's blending mode */
        HI_U32 u32Src1PremultiEn :      1;      /* Enable to pre-multiply alpha */
        HI_U32 u32Src1MultiGlobalEn :   1;      /* Enable to Src1 pre-multi global alpha */
        HI_U32 u32Src1PixelAlphaEn    :   1;    /* Enable to Src1 pixel alpha */
        HI_U32 u32Src1GlobalAlphaEn   :   1;    /* Enable to Src1 global alpha */
        HI_U32 u32Src2BlendMode :       4;      /* Blend mode in Src2 */
        HI_U32 u32Src2PremultiEn :      1;      /* Enable to Src2 pre-multi alpha */
        HI_U32 u32Src2MultiGlobalEn :   1;      /* Enable to Src2 pre-multi global alpha */
        HI_U32 u32Src2PixelAlphaEn    :   1;    /* Enable to Src2 pixel alpha */
        HI_U32 u32Src2GlobalAlphaEn   :   1;    /* Enable to Src2 global alpha*/
        HI_U32 u32AlphaRopEn    :   1;          /* Enable to alpha,when alpha blending */
        HI_U32 u32Reserved  :   15;             /* Reserve */
    }stBits;
    HI_U32 u32All;
}TDE_ALPHA_BLEND_U;

typedef union hiTDE_Y_PITCH
{
    struct
    {
        HI_U32 u32Pitch     :   16; /* line pitch */
        HI_U32 u32Reserved	:16;	/* Reserve */
    }stBits;
    HI_U32 u32All;
}TDE_Y_PITCH_U;

/* Filter argument table */
typedef union hiTDE_FILTER_PARA_U
{
    struct
    {
        HI_U32 u32Para0        :5;
        HI_U32 u32Para1        :6;
        HI_U32 u32Para2        :8;
        HI_U32 u32Para3        :7;
        HI_U32 u32Para4        :6;
    }stBits;
    HI_U32 u32All;
}TDE_FILTER_PARA_U;

typedef union hiTDE_BUSLIMITER_U
{
    struct
    {
        HI_U32 u32BwCnt :   10;
        HI_U32 u32CasRidSel :   1;
        HI_U32 u32Src1RidSel    :   1;
        HI_U32 u32Src2RidSel    :   1;
        HI_U32 u32Y1RidSel  :   1;
        HI_U32 u32Y2RidSel  :   1;
        HI_U32 u32Reserved1  :   1;
        HI_U32 u32WOutstdMax    :   4;
        HI_U32 u32ROutstdMax    :   4;
        HI_U32 u32Reserved2 :   7;
        HI_U32 u32ClkCtlEn  :   1;
    }stBits;
    HI_U32 u32All;
}TDE_BUSLIMITER_U;

/* List of each hardware node, using to maitain hardware node */
typedef struct hiTDE_BUF_NODE_S
{
    struct list_head stList;
    HI_U32 u32PhyAddr;              /* Point to physical address of hardware buffer */
    HI_U32* pu32VirAddr;            /* Virtual address responding to hardware buffer */
    HI_S32 s32Handle;               /* Current node's job handle */
    HI_VOID* pSwNode;               /* Software node pointer of current node */
    HI_U32 u32WordSize;             /* Size of current node config is needed, united by word */
    HI_U32 u32CurUpdate;            /* Update flag of current node */
    struct hiTDE_BUF_NODE_S* pstParentAddr; /* Parent node addr of configed information inherited */
}TDE_BUF_NODE_S;


/* State information when TDE operate is suspend */
typedef struct hiTDE_SUSP_STAT_S
{
    HI_S32 s32AqSuspLine;       /* Line executed while AQ is suspend ,if -1 , present no effection */
    TDE_BUF_NODE_S* pstSwBuf;
    HI_VOID *pSwNode;       	/* Pointer point to software node be suspend */
}TDE_SUSP_STAT_S;

/* Head address of argument table used as config */
typedef struct hiTDE_PARA_TABLE_S
{
    HI_U32 u32HfCoefAddr;
    HI_U32 u32VfCoefAddr;
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32DeflickerVfCoefAddr;
#endif
}TDE_PARA_TABLE_S;

/****************************************************************************/
/*                         TDE hal inner variable definition                */
/****************************************************************************/
/* Base addr of register after mapping */
#ifdef TDE_COREDUMP_DEBUG
 volatile HI_U32* s_pu32BaseVirAddr = HI_NULL;
#else
STATIC volatile HI_U32* s_pu32BaseVirAddr = HI_NULL;
#endif
#ifndef CONFIG_TDE_USE_SDK_CRG_ENABLE
/* Pointer of TDE clock register after mapping */
STATIC volatile HI_U32 *s_pu32TdeClockVir = HI_NULL;
#endif
/* Head address of config argument table */
STATIC TDE_PARA_TABLE_S s_stParaTable = {0};

/* Deflicker level, default is auto */
#ifndef HI_BUILD_IN_BOOT
STATIC TDE_DEFLICKER_LEVEL_E s_eDeflickerLevel = TDE_DEFLICKER_AUTO;
#endif
/* alpha threshold switch */
STATIC HI_BOOL s_bEnAlphaThreshold = HI_FALSE;

/*alpha threshold value */
STATIC HI_U8 s_u8AlphaThresholdValue = 0xff;

STATIC TDE_DRV_ALU_MODE_E s_u32CbmctrlAlu_mode[ TDE_BUTT + 1 ] = { 3 ,TDE_ALU_NONE ,TDE_ALU_ROP ,3 ,TDE_ALU_MASK_ROP1,\
                                                                  TDE_ALU_MASK_ROP2 ,TDE_ALU_MASK_BLEND ,3 ,3 ,3 ,3 ,TDE_BUTT};
#ifndef HI_BUILD_IN_BOOT

STATIC TDE2_BLEND_MODE_E  s_u32Src1BlendCmdMode[TDE2_BLENDCMD_BUTT + 1] = {\
    TDE2_BLEND_INVSRC2ALPHA ,   TDE2_BLEND_ZERO,         TDE2_BLEND_ZERO,\
    TDE2_BLEND_INVSRC2ALPHA,    TDE2_BLEND_ONE,          TDE2_BLEND_ZERO,\
    TDE2_BLEND_SRC2ALPHA,       TDE2_BLEND_ZERO,         TDE2_BLEND_INVSRC2ALPHA,\
    TDE2_BLEND_INVSRC2ALPHA,    TDE2_BLEND_SRC2ALPHA,    TDE2_BLEND_ONE,\
    TDE2_BLEND_INVSRC2ALPHA,    TDE2_BLEND_ONE,           0 ,\
    0 };

STATIC TDE2_BLEND_MODE_E  s_u32Src2BlendCmdMode[TDE2_BLENDCMD_BUTT + 1] = {\
    TDE2_BLEND_SRC2ALPHA ,      TDE2_BLEND_ZERO,         TDE2_BLEND_ONE,\
    TDE2_BLEND_ONE,             TDE2_BLEND_INVSRC1ALPHA, TDE2_BLEND_SRC1ALPHA,\
    TDE2_BLEND_ZERO,            TDE2_BLEND_INVSRC1ALPHA, TDE2_BLEND_ZERO,\
    TDE2_BLEND_SRC1ALPHA,       TDE2_BLEND_INVSRC1ALPHA, TDE2_BLEND_ONE,\
    TDE2_BLEND_INVSRC1ALPHA,    TDE2_BLEND_ZERO,         0 ,\
    0 };
#endif
/****************************************************************************/
/*                             TDE hal inner function definition            */
/****************************************************************************/
STATIC HI_S32 TdeHalInitParaTable(HI_VOID);

#ifndef HI_BUILD_IN_BOOT
STATIC INLINE HI_S32 TdeHalGetOffsetInNode(HI_U64 u64MaskUpdt, HI_U64 u64Update);

STATIC HI_VOID TdeHalNodeSetCLUTColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey);
STATIC HI_VOID TdeHalNodeSetYcbcrColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey);
STATIC HI_VOID TdeHalNodeSetARGBColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey);

STATIC HI_U32 TdeHalGetColorKeyMode(TDE2_COLORKEY_COMP_S *stColorKey);
STATIC HI_U32 TdeHalGetYCbCrKeyMask(HI_U8 u8Cr, HI_U8 u8Cb, HI_U8 u8Cy, HI_U8 u8Alpha);
STATIC HI_U32 TdeHalGetClutKeyMask(HI_U8 u8Clut, HI_U8 u8Alpha);
STATIC HI_U32 TdeHalGetArgbKeyMask(HI_U8 u8Blue, HI_U8 u8Green, HI_U8 u8Red, HI_U8 u8Alpha);

#endif

STATIC HI_S32  TdeHalGetbppByFmt(TDE_DRV_COLOR_FMT_E enFmt);
STATIC INLINE HI_U32 TdeHalGetResizeParaHTable(HI_U32 u32Step);
STATIC INLINE HI_U32 TdeHalGetResizeParaVTable(HI_U32 u32Step);


STATIC INLINE HI_VOID TdeHalInitQueue(HI_VOID);

HI_VOID TdeHalSetClock(HI_BOOL bEnable);
#ifndef HI_BUILD_IN_BOOT
#define REGISTDER_FLUSH()      mb()
#else
#define REGISTDER_FLUSH()      {do{}while(0);}
#endif

/****************************************************************************/
/*                TDE hal ctl interface realization                         */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeHalInit
* Description:   map the base address for tde
* Input:         u32BaseAddr: the base address of tde
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeHalInit(HI_U32 u32BaseAddr)
{
    /*init the pool memery of tde*//*CNcomment:初始化TDE内存池*/
    if (HI_SUCCESS != wmeminit())
    {
        goto TDE_INIT_ERR;
    }

    /*config start address for the parameter  *//*CNcomment: 配置参数表首地址 */
    if (HI_SUCCESS != TdeHalInitParaTable())
    {
        goto TDE_INIT_ERR;
    }

#ifndef HI_BUILD_IN_BOOT
     /*map address for the register *//*CNcomment:寄存器映射*/
    s_pu32BaseVirAddr = (volatile HI_U32*)HI_GFX_REG_MAP(u32BaseAddr, TDE_REG_SIZE);
    if(HI_NULL == s_pu32BaseVirAddr)
    {
        goto TDE_INIT_ERR;
    }
	#ifndef CONFIG_TDE_USE_SDK_CRG_ENABLE
     /* set limit of clock and div *//*CNcomment:  设置时钟门限、时钟分频 */
    s_pu32TdeClockVir = (volatile HI_U32 *)HI_GFX_REG_MAP(TDE_REG_CLOCK, 4);
    if (HI_NULL == s_pu32TdeClockVir)
    {
        HI_GFX_REG_UNMAP(s_pu32BaseVirAddr);
        goto TDE_INIT_ERR;
    }
	#endif
#else
	s_pu32BaseVirAddr = (HI_U32*)u32BaseAddr;
#endif

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_GFX_MapSmmuReg((HI_U32)(TDE_REG_BASEADDR + 0xf000));
#endif

    TdeHalSetClock(HI_FALSE);
    return HI_SUCCESS;

TDE_INIT_ERR:
    return HI_FAILURE;
}


#ifndef HI_BUILD_IN_BOOT
HI_VOID TdeHalResumeInit(HI_VOID)
{
    TdeHalSetClock(HI_TRUE);

    TdeHalCtlReset();

    TdeHalInitQueue();

    return;
}

HI_VOID TdeHalSuspend(HI_VOID)
{
	TdeHalSetClock(HI_FALSE);
	return;
}

/*****************************************************************************
* Function:      TdeHalClose
* Description:   close the tde
* Input:         none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalClose(HI_VOID)
{
    /* do sth ... */
    TdeHalSetClock(HI_FALSE);
    return HI_SUCCESS;
}
/*****************************************************************************
* Function:      TdeHalOpen
* Description:   open the tde
* Input:         none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalOpen(HI_VOID)
{
    /* do sth ... */
    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalRelease
* Description:   release the address that had map
* Input:         none
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_VOID TdeHalRelease(HI_VOID)
{
    HI_VOID *pBuf;
    if(0 != s_stParaTable.u32HfCoefAddr)
    {
        pBuf = (HI_VOID *)wgetvrt(s_stParaTable.u32HfCoefAddr);
        if (HI_NULL != pBuf)
        {
            TDE_FREE(pBuf);
            s_stParaTable.u32HfCoefAddr = 0;
        }
    }
    if (0 != s_stParaTable.u32VfCoefAddr)
    {
        pBuf = (HI_VOID *)wgetvrt(s_stParaTable.u32VfCoefAddr);
        if (HI_NULL != pBuf)
        {
            TDE_FREE(pBuf);
            s_stParaTable.u32VfCoefAddr = 0;
        }
    }
#ifndef HI_BUILD_IN_BOOT
    if (0 != s_stParaTable.u32DeflickerVfCoefAddr)
    {
        pBuf = (HI_VOID *)wgetvrt(s_stParaTable.u32DeflickerVfCoefAddr);
        if (HI_NULL != pBuf)
        {
            TDE_FREE(pBuf);
            s_stParaTable.u32DeflickerVfCoefAddr = 0;
        }
    }
#endif
#ifndef CONFIG_TDE_USE_SDK_CRG_ENABLE
    /* unmap*//*CNcomment: 解除映射*/
   	HI_GFX_REG_UNMAP(s_pu32TdeClockVir);
    s_pu32TdeClockVir = HI_NULL;
#endif
    /* unmap the base address*//*CNcomment:  反映射基地址 */
    HI_GFX_REG_UNMAP(s_pu32BaseVirAddr);
    s_pu32BaseVirAddr = HI_NULL;

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_GFX_UnMapSmmuReg();
#endif

     /*free the pool of memery*//*CNcomment:TDE内存池去初始化*/
    wmemterm();

    return;
}
#endif/**HI_BUILD_IN_BOOT**/


/*****************************************************************************
* Function:      TdeHalCtlIsIdle
* Description:   get the state of tde
* Input:         none
* Output:        none
* Return:        True: Idle/False: Busy
* Others:        none
*****************************************************************************/
HI_BOOL TdeHalCtlIsIdle(HI_VOID)
{
    return (HI_BOOL)(!(TDE_READ_REG(s_pu32BaseVirAddr, TDE_STA) & 0x1));
}

/*****************************************************************************
* Function:      TdeHalCtlIsIdleSafely
* Description:   get the state of tde one more time ,make sure it's idle
* Input:         none
* Output:        none
* Return:        True: Idle/False: Busy
* Others:        none
*****************************************************************************/
HI_BOOL TdeHalCtlIsIdleSafely(HI_VOID)
{
    HI_U32 i = 0;

    /*get the state of tde one more time ,make sure it's idle *//*CNcomment: 连续读取多次硬件状态,确保TDE完成 */
    for (i = 0; i < TDE_MAX_READ_STATUS_TIME; i++)
    {
        if (!TdeHalCtlIsIdle())
        {
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

/*****************************************************************************
* Function:      TdeHalCtlIntMask
* Description:   get the state of interrupt for Sq/Aq
* Input:         none
* Output:        none
* Return:        the interrupt state of Sq/Aq
* Others:        none
*****************************************************************************/
HI_U32 TdeHalCtlIntStats(HI_VOID)
{
    HI_U32 Value;
    Value = TDE_READ_REG(s_pu32BaseVirAddr, TDE_INT);
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_INTCLR, 0x7);

#ifndef HI_BUILD_IN_BOOT
    (HI_VOID)HI_GFX_SmmuIsr("HI_MOD_TDE");
#endif

    return Value;
}


STATIC INLINE HI_VOID TdeHalReset(HI_VOID)
{
#ifdef CONFIG_TDE_USE_SDK_CRG_ENABLE
    HI_S32 s32Cnt = 0;

    U_PERI_CRG37 unTempValue;
  #ifdef CONFIG_GFX_MMU_CLOCK
    U_PERI_CRG204 unTempSmmuValue;
  #endif

  #ifdef CONFIG_GFX_MMU_SUPPORT
    HI_GFX_DeinitSmmu();
    udelay(1);
  #endif

  #ifdef CONFIG_GFX_MMU_CLOCK
    unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
    unTempSmmuValue.bits.tde_smmu_cken = 0x1;
    g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
    REGISTDER_FLUSH();
  #endif

    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
    unTempValue.bits.tde_cken = 0x1;
    g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
    REGISTDER_FLUSH();


    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
    unTempValue.bits.tde_srst_req = 0x1;
    g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
    REGISTDER_FLUSH();

  #ifdef CONFIG_GFX_MMU_CLOCK
    unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
    unTempSmmuValue.bits.tde_smmu_srst_req = 0x1;
    g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
    REGISTDER_FLUSH();
  #endif

    do
    {
        s32Cnt++;
        unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
        if (0x1 == unTempValue.bits.tde_cken)
        {
            break;
        }
    }while(s32Cnt < 10);

    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
    unTempValue.bits.tde_srst_req = 0x0;
    g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
    REGISTDER_FLUSH();

 #ifdef CONFIG_GFX_MMU_CLOCK
    unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
    unTempSmmuValue.bits.tde_smmu_srst_req = 0x0;
    g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
    REGISTDER_FLUSH();
 #endif

 #ifdef CONFIG_GFX_MMU_SUPPORT
    udelay(1);
    HI_GFX_InitSmmu((HI_U32)(TDE_REG_BASEADDR + 0xf000));
 #endif
#endif
    return;
}


/*****************************************************************************
* Function:      TdeHalCtlReset
* Description:   reset by software, this operation will clear the interrupt state
* Input:         none
* Output:       none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalCtlReset(HI_VOID)
{
    TdeHalReset();
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_INTCLR,0x7);
    return;
}
/*****************************************************************************
* Function:      TdeHalSetClock
* Description:   enable or disable the clock of TDE
* Input:        HI_BOOL bEnable:enable/disable
* Output:       none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalSetClock(HI_BOOL bEnable)
{
#ifdef CONFIG_TDE_USE_SDK_CRG_ENABLE
    HI_S32 s32Cnt = 0;

#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32StartTimeMs = 0;
    HI_U32 u32EndTimeMs   = 0;
    HI_U32 u32TotalTime   = 0;
#endif

    U_PERI_CRG37 unTempValue;
#ifdef CONFIG_GFX_MMU_CLOCK
    U_PERI_CRG204 unTempSmmuValue;
#endif

    if (bEnable)
    {
     #ifdef CONFIG_GFX_MMU_CLOCK
        unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
        unTempSmmuValue.bits.tde_smmu_cken = 0x1;
        g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
        REGISTDER_FLUSH();
     #endif

        unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
        unTempValue.bits.tde_cken = 0x1;
        g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
        REGISTDER_FLUSH();

        do
        {
            s32Cnt++;
            unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
            if (0x1 == unTempValue.bits.tde_cken)
            {
                break;
            }
        }while(s32Cnt < 10);

        unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
        unTempValue.bits.tde_srst_req = 0x0;
        g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
        REGISTDER_FLUSH();

      #ifdef CONFIG_GFX_MMU_CLOCK
        unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
        unTempSmmuValue.bits.tde_smmu_srst_req = 0x0;
        g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
        REGISTDER_FLUSH();
      #endif

      #ifdef CONFIG_GFX_MMU_SUPPORT
       udelay(1);
       HI_GFX_InitSmmu((HI_U32)(TDE_REG_BASEADDR + 0xf000));
      #endif
    }
    else
    {
       #ifdef CONFIG_GFX_MMU_SUPPORT
        HI_GFX_DeinitSmmu();
        udelay(1);
       #endif

        unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
        unTempValue.bits.tde_srst_req = 0x1;
        g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
        REGISTDER_FLUSH();

      #ifdef CONFIG_GFX_MMU_CLOCK
        unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
        unTempSmmuValue.bits.tde_smmu_srst_req = 0x1;
        g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
        REGISTDER_FLUSH();

        unTempSmmuValue.u32 = g_pstRegCrg->PERI_CRG204.u32;
        unTempSmmuValue.bits.tde_smmu_cken = 0x0;
        g_pstRegCrg->PERI_CRG204.u32 = unTempSmmuValue.u32;
        REGISTDER_FLUSH();
     #endif

     #ifndef HI_BUILD_IN_BOOT
        HI_GFX_GetTimeStamp(&u32StartTimeMs,NULL);
        while (u32TotalTime <= 1)
        {
            HI_GFX_GetTimeStamp(&u32EndTimeMs,NULL);
            u32TotalTime = u32EndTimeMs - u32StartTimeMs;
        }
     #endif

        unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
        unTempValue.bits.tde_cken = 0x0;
        g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
        REGISTDER_FLUSH();

    }

#else
    if (bEnable)
    {
        *s_pu32TdeClockVir &= ~0x1;
        *s_pu32TdeClockVir |= 0x2;
    }
    else
    {
        *s_pu32TdeClockVir &= ~0x2;
        *s_pu32TdeClockVir |= 0x1;
    }
#endif
    return;
}

#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeHalCtlIntClear
* Description:   clear the state of interrupt
* Input:
*                u32Stats: state that need clear
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalCtlIntClear(HI_U32 u32Stats)
{
    HI_U32 u32ReadStats = 0;

    u32ReadStats = TDE_READ_REG(s_pu32BaseVirAddr, TDE_INT);
    u32ReadStats = (u32ReadStats & 0x0000ffff) | ((u32Stats << 16) & 0xffff0000);

    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_INT, u32ReadStats);
}
#endif

/****************************************************************************/
/*                             TDE hal node interface                    */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeHalNodeInitNd
* Description:   init the software node struct for tde
* Input:         pHWNode: the pointer of software node struct
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeInitNd(TDE_HWNode_S** pstHWNode)
{
    HI_VOID * pBuf= NULL;
    TDE_HWNode_S *pHWNode = NULL;

    pBuf = (HI_VOID *)TDE_MALLOC(sizeof(TDE_HWNode_S)+ TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
    if (HI_NULL == pBuf)
    {
        TDE_TRACE(TDE_KERN_INFO, "malloc (%d) failed, wgetfreenum(%d)!\n", (sizeof(TDE_HWNode_S) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE), wgetfreenum());
        return HI_ERR_TDE_NO_MEM;
    }
    *pstHWNode = (TDE_HWNode_S*)(pBuf +TDE_NODE_HEAD_BYTE);

    pHWNode = *pstHWNode;
    pHWNode->MMU_PROT_CTRL.u32All = 0xffff0000;

    /*open interrupt*//*CNcomment:开启链表完成中断,
    同步链表可更新中断, 关闭节点完成中断 */
    return HI_SUCCESS;
}

#ifdef CONFIG_GFX_MMU_SUPPORT
HI_VOID TdeHalFreeTmpBuf(TDE_HWNode_S* pstHWNode)
{
#if 0
	if (pstHWNode->u32TDE_CLUT_ADDR != 0)
	{
		TDE_FREE(wgetvrt(pstHWNode->u32TDE_CLUT_ADDR));
	}

	if ((pstHWNode->u32TDE_ICSC_ADDR != 0) && (pstHWNode->u32TDE_ICSC_ADDR != s_u32Rgb2YuvCoefAddr)
		&& (pstHWNode->u32TDE_ICSC_ADDR != s_u32Yuv2RgbCoefAddr))
	{
		TDE_FREE(wgetvrt(pstHWNode->u32TDE_ICSC_ADDR));
	}

	if ((pstHWNode->u32TDE_OCSC_ADDR != 0) && (pstHWNode->u32TDE_OCSC_ADDR != s_u32Rgb2YuvCoefAddr)
		&& (pstHWNode->u32TDE_OCSC_ADDR != s_u32Yuv2RgbCoefAddr))
	{
		TDE_FREE(wgetvrt(pstHWNode->u32TDE_OCSC_ADDR));
	}
#endif
}
#endif

/*****************************************************************************
* Function:      TdeHalFreeNodeBuf
* Description:   Free TDE operate node buffer
* Input:         pstHWNode:Node struct pointer.
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID TdeHalFreeNodeBuf(TDE_HWNode_S* pstHWNode)
{
    HI_VOID * pBuf= NULL;
    pBuf = (HI_VOID*)pstHWNode - TDE_NODE_HEAD_BYTE;

#ifdef CONFIG_GFX_MMU_SUPPORT
	TdeHalFreeTmpBuf(pstHWNode);
#endif

    TDE_FREE(pBuf);
}

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
HI_VOID TdeHalNodeInitChildNd(TDE_HWNode_S* pHWNode, HI_U32 u32TDE_CLIP_START, HI_U32 u32TDE_CLIP_STOP)
{
#if 0
#ifndef HI_BUILD_IN_BOOT
    TDE_INS_U unIns;
    TDE_2D_RSZ_U unRsz;
    HI_BOOL bClipOut;

    TDE_ASSERT(HI_NULL != pHWNode);


    bClipOut = pHWNode->u32TDE_CLIP_START >> 31;

     /*it need set father's info and clear Update info when node is child*/
     /*CNcomment:如果是子节点, 还需要父节点的设置信息,只把Update清空 */

    unRsz.u32All = pHWNode->u32TDE_2D_RSZ;

    /*it's not need set parameter info when node is child*/
    /*CNcomment: 如果是子节点则不需要更新参数表 */
    unRsz.stBits.u32VfCoefReload = 0;
    unRsz.stBits.u32HfCoefReload = 0;
    pHWNode->u32TDE_2D_RSZ = unRsz.u32All;
    if(bClipOut)
    {
        pHWNode->u32TDE_INS = pHWNode->u32TDE_INS | 0x4000; /*open clip function*//* CNcomment: 开启clip 功能*/
        pHWNode->u32TDE_CLIP_START = u32TDE_CLIP_START;
        pHWNode->u32TDE_CLIP_STOP = u32TDE_CLIP_STOP;
    }

    /*open interrupt*//* CNcomment:  开启链表完成中断,同步链表可更新中断, 关闭节点完成中断 */
    unIns.u32All = pHWNode->u32TDE_INS;
    //unIns.stBits.u32AqIrqMask = TDE_AQ_COMP_LIST_MASK_EN;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
	unIns.stBits.u32CasProt = 0x1;
#endif
    pHWNode->u32TDE_INS = unIns.u32All;
#endif
#endif
}



/*****************************************************************************
* Function:      TdeHalNodeExecute
* Description:  start list of tde
* Input:
*                u32NodePhyAddr: the start address of head node address
*                u64Update:the head node update set
*                bAqUseBuff: whether use temp buffer
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeExecute(HI_U32 u32NodePhyAddr, HI_U64 u64Update, HI_BOOL bAqUseBuff)
{
    /*tde is idle*//* CNcomment:TDE空闲*/
    if(TdeHalCtlIsIdleSafely())
    {
       TdeHalSetClock(HI_TRUE);
       TdeHalCtlReset();

       TdeHalInitQueue();
       /*write the first node address*//* CNcomment:写入首节点地址*/
       TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_NADDR, u32NodePhyAddr);
       /*write the first node update area*//* CNcomment:写入首节点更新标识*/

       /*start Aq list*//* CNcomment:启动Aq*/
       TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_CTRL, 0x1);
       #ifndef HI_BUILD_IN_BOOT
         mb();
       #endif
    }
    else
    {
       return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeEnableCompleteInt
* Description:   enable the finish interrupt of node
* Input:         pBuf: buffer of node
*                enType: type of list
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeEnableCompleteInt(HI_VOID* pBuf)
{
    TDE_HWNode_S* pHWNode = (TDE_HWNode_S*)pBuf;
    TDE_ASSERT(HI_NULL != pBuf);
    pHWNode->TDE_INTMASK.bits.eof_mask = 0x1;
    pHWNode->TDE_INTMASK.bits.timeout_mask = 0x1;
    pHWNode->TDE_INTMASK.bits.bus_err_mask = 0x1;
    pHWNode->TDE_INTMASK.bits.eof_end_mask = 0x0;
}

/*****************************************************************************
* Function:      TdeHalNodeSetSrc1
* Description:   set the info for source of bitmap 1
* Input:         pHWNode: pointer of node
*                pDrvSurface: bitmap info
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetSrc1(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface)
{
    HI_U32 u32Bpp;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvSurface);

    /*set the source bitmap attribute info*//*CNcomment:配置源位图属性信息*/
    pHWNode->SRC1_CTRL.bits.src1_fmt= (HI_U32)pDrvSurface->enColorFmt;
    pHWNode->SRC1_CTRL.bits.src1_alpha_range = 1-(HI_U32)pDrvSurface->bAlphaMax255;
    pHWNode->SRC1_CTRL.bits.src1_h_scan_ord = (HI_U32)pDrvSurface->enHScan;
    pHWNode->SRC1_CTRL.bits.src1_v_scan_ord = (HI_U32)pDrvSurface->enVScan;
   /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    pHWNode->SRC1_CTRL.bits.src1_rgb_exp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    #ifdef CONFIG_GFX_MMU_SUPPORT
	pHWNode->MMU_PROT_CTRL.bits.src1_ch0_mmu_bypass = pDrvSurface->bCma;
    #else
	pHWNode->MMU_PROT_CTRL.bits.src1_ch0_mmu_bypass = 0x1;
    #endif
#endif


    pHWNode->SRC1_IMGSIZE.bits.src1_width = pDrvSurface->u32Width-1;
    pHWNode->SRC1_IMGSIZE.bits.src1_height = pDrvSurface->u32Height-1;


    u32Bpp = TdeHalGetbppByFmt(pDrvSurface->enColorFmt);


    pHWNode->SRC1_CH0_ADDR.bits.src1_ch0_addr = pDrvSurface->u32PhyAddr + pDrvSurface->u32Ypos*pDrvSurface->u32Pitch\
                                                                                       +((pDrvSurface->u32Xpos*u32Bpp)>>3);

    if (pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_RGB565)
    {
        pHWNode->SRC1_CTRL.bits.src1_argb_order = pDrvSurface->enRgbOrder;
    }

    /*target bitmapis same with source bitmap 1,so not need set*/
    /*CNcomment:源1位图宽高总是与Target位图一致,因此不设置源1的大小*/

    /*config the node*//*CNcomment:配置缓存节点*/
    pHWNode->SRC1_CH0_STRIDE.bits.src1_ch0_stride = pDrvSurface->u32Pitch;
    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetSrc2
* Description:   set the source bitmap 2
* Input:         pHWNode: pointer of node
*                pDrvSurface:  bitmap info
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetSrc2(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface)
{
    HI_U32 u32Bpp;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvSurface);

    /*set attribute info for source bitmap*//*CNcomment:配置源位图属性信息*/
    pHWNode->SRC2_CTRL.bits.src2_fmt= (HI_U32)pDrvSurface->enColorFmt;
    pHWNode->SRC2_CTRL.bits.src2_alpha_range = 0;
    pHWNode->SRC2_CTRL.bits.src2_h_scan_ord = (HI_U32)pDrvSurface->enHScan;
    pHWNode->SRC2_CTRL.bits.src2_v_scan_ord = (HI_U32)pDrvSurface->enVScan;
   /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    pHWNode->SRC2_CTRL.bits.src2_rgb_exp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    #ifdef CONFIG_GFX_MMU_SUPPORT
	      pHWNode->MMU_PROT_CTRL.bits.src2_ch0_mmu_bypass = pDrvSurface->bCma;
       pHWNode->MMU_PROT_CTRL.bits.src2_ch1_mmu_bypass = pDrvSurface->bCma;
    #else
	      pHWNode->MMU_PROT_CTRL.bits.src2_ch0_mmu_bypass = 0x1;
       pHWNode->MMU_PROT_CTRL.bits.src2_ch1_mmu_bypass = 0x1;
    #endif
#endif

    pHWNode->SRC2_IMGSIZE.bits.src2_width = pDrvSurface->u32Width-1;
    pHWNode->SRC2_IMGSIZE.bits.src2_height = pDrvSurface->u32Height-1;



    u32Bpp = TdeHalGetbppByFmt(pDrvSurface->enColorFmt);
    if (pDrvSurface->enColorFmt >= TDE_DRV_COLOR_FMT_YCbCr400MBP)
    {
       pHWNode->SRC2_CH0_ADDR.bits.src2_ch0_addr = pDrvSurface->u32PhyAddr;
       pHWNode->SRC2_CH1_ADDR.bits.src2_ch1_addr = pDrvSurface->u32CbCrPhyAddr;
       if(pDrvSurface->enColorFmt==TDE_DRV_COLOR_FMT_YCbCr422MBV)
        {
            pHWNode->SRC2_CTRL.bits.src2_422v_pro = 1;
        }

    }
    else
    {
        pHWNode->SRC2_CH0_ADDR.bits.src2_ch0_addr= pDrvSurface->u32PhyAddr + pDrvSurface->u32Ypos*pDrvSurface->u32Pitch\
                                                                                           +((pDrvSurface->u32Xpos*u32Bpp)>>3);

        if (pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_RGB565)
        {
            pHWNode->SRC2_CTRL.bits.src2_argb_order = pDrvSurface->enRgbOrder;
        }
    }

    if(pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_A1)
    {
        pHWNode->SRC2_ALPHA.bits.src2_alpha0 = 0x00;
        pHWNode->SRC2_ALPHA.bits.src2_alpha1 = 0xff;
    }
    pHWNode->SRC2_CH0_STRIDE.bits.src2_ch0_stride = pDrvSurface->u32Pitch;
    pHWNode->SRC2_CH1_STRIDE.bits.src2_ch1_stride = pDrvSurface->u32CbCrPitch;

    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetSrcMbY
* Description:   set the brightness info for source bitmap that is mb format
* Input:         pHWNode: pointer of node
*                pDrvMbY: brightness info
*                enMbOpt: set info
* Output:       none
* Return:       none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetSrcMbY(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvMbY, TDE_DRV_MB_OPT_MODE_E enMbOpt)
{
#if 0
    U_SRC1_CTRL unSrc1Ctrl;
    U_SRC1_IMGSIZE unSrc1Size;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvMbY);

    /*set attribute for source bitmap *//*CNcomment:配置源位图属性信息*/
    unSrc1Ctrl.u32All = pHWNode->SRC1_CTRL;
    unSrc1Ctrl.bits.src1_fmt= (HI_U32)pDrvMbY->enColorFmt;
    unSrc1Ctrl.bits.src1_alpha_range = 1-(HI_U32)pDrvMbY->bAlphaMax255;
    unSrc1Ctrl.bits.src1_h_scan_ord = (HI_U32)pDrvMbY->enHScan;
    unSrc1Ctrl.bits.src1_v_scan_ord = (HI_U32)pDrvMbY->enVScan;
   /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    unSrc1Ctrl.bits.src1_rgb_exp = 0;
    unSrc1Size.bits.src1_width = pDrvMbY->u32Width - 1;
    unSrc1Size.bits.src1_height = pDrvMbY->u32Height - 1;

    pHWNode->SRC1_CTRL = unSrc1Ctrl.u32All;
    pHWNode->SRC1_IMGSIZE = unSrc1Size.u32All;
    pHWNode->SRC1_CH0_ADDR = pDrvMbY->u32PhyAddr;
    pHWNode->SRC1_CH0_STRIDE = pDrvMbY->u32Pitch;

#endif
    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetSrcMbCbCr
* Description:   set the chroma info for source bitmap that is mb format
* Input:         pHWNode: pointer of node
*                pDrvMbCbCr: cbcr info
*                enMbOpt: set info
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetSrcMbCbCr(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvMbCbCr, TDE_DRV_MB_OPT_MODE_E enMbOpt)
{
#if 0
    U_SRC2_CTRL unSrc2Ctrl;
    U_SRC2_IMGSIZE unSrc2Size;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvMbCbCr);

    /*set attribute info for source bitmap*//*CNcomment:配置源位图属性信息*/
    unSrc2Ctrl.u32All = pHWNode->SRC2_CTRL;
    unSrc2Size.u32All = pHWNode->SRC2_IMGSIZE;
    unSrc2Ctrl.bits.src2_fmt= (HI_U32)pDrvMbCbCr->enColorFmt;
    unSrc2Ctrl.bits.src2_alpha_range = 1 - (HI_U32)pDrvMbCbCr->bAlphaMax255;
    unSrc2Ctrl.bits.src2_h_scan_ord = (HI_U32)pDrvMbCbCr->enHScan;
    unSrc2Ctrl.bits.src2_v_scan_ord = (HI_U32)pDrvMbCbCr->enVScan;
   /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    unSrc2Ctrl.bits.src2_rgb_exp = 0;
    unSrc2Size.bits.src2_width = pDrvMbCbCr->u32Width-1;
    unSrc2Size.bits.src2_height = pDrvMbCbCr->u32Height-1;

    if(pDrvMbCbCr->enColorFmt==TDE_DRV_COLOR_FMT_YCbCr422MBV)
    {
        unSrc2Ctrl.bits.src2_422v_pro = 1;
    }

    pHWNode->SRC2_CTRL = unSrc2Ctrl.u32All;
    pHWNode->SRC2_CH0_STRIDE = pDrvMbCbCr->u32Pitch;
    pHWNode->SRC2_CH1_STRIDE = pDrvMbCbCr->u32CbCrPitch;
    pHWNode->SRC2_CH0_ADDR = pDrvMbCbCr->u32PhyAddr;
    pHWNode->SRC2_CH1_ADDR = pDrvMbCbCr->u32CbCrPhyAddr;

    pHWNode->SRC2_IMGSIZE = unSrc2Size.u32All;
#endif
    return;
}
/*****************************************************************************
* Function:      TdeHalNodeSetTgt
* Description:   set target bitmap info
* Input:         pHWNode: pointer of node
*                pDrvSurface: bitmap info
*                enAlphaFrom: alpha from
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetTgt(TDE_HWNode_S* pHWNode, TDE_DRV_SURFACE_S* pDrvSurface, TDE_DRV_OUTALPHA_FROM_E enAlphaFrom)
{
    HI_U32 u32Bpp;
    TDE_ASSERT(HI_NULL != pHWNode);

    /*set bitmap attribute info*//*CNcomment:配置源位图属性信息*/
    pHWNode->DES_CTRL.bits.des_en = 1;
    pHWNode->DES_CTRL.bits.des_fmt = pDrvSurface->enColorFmt;
    pHWNode->DES_CTRL.bits.des_alpha_range= 1-(HI_U32)pDrvSurface->bAlphaMax255;
    pHWNode->DES_CTRL.bits.des_h_scan_ord = (HI_U32)pDrvSurface->enHScan;
    pHWNode->DES_CTRL.bits.des_v_scan_ord = (HI_U32)pDrvSurface->enVScan;
    pHWNode->DES_CTRL.bits.des_alpha_range= 1-(HI_U32)pDrvSurface->bAlphaMax255;
    pHWNode->DES_CTRL.bits.des_rgb_round = 0; /*CNcomment:一直使用使用四舍无入的截位方式*/

    pHWNode->CBMALUPARA.bits.alpha_from = (HI_U32)enAlphaFrom;
    pHWNode->DES_ALPHA.bits.des_alpha_thd = s_u8AlphaThresholdValue;

    /*set bitmap size info*//*CNcomment:配置位图大小信息*/
    pHWNode->DES_IMGSIZE.bits.des_width = (HI_U32)pDrvSurface->u32Width - 1;
    pHWNode->DES_IMGSIZE.bits.des_height = (HI_U32)pDrvSurface->u32Height - 1;

    pHWNode->DES_SUR_XY.bits.des_start_x = pDrvSurface->u32Xpos;
    pHWNode->DES_SUR_XY.bits.des_start_y = pDrvSurface->u32Ypos;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
   #ifdef CONFIG_GFX_MMU_SUPPORT
   	 pHWNode->DES_CTRL.bits.des_ch0_mmu_bypass = pDrvSurface->bCma;
   #else
   	 pHWNode->DES_CTRL.bits.des_ch0_mmu_bypass = 1;
   #endif
#endif


    if (pDrvSurface->enColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888)
    {
        pDrvSurface->enColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;
        pHWNode->DES_CTRL.bits.des_argb_order= 0x17;
    }

    if ((pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_RGB565) || (pDrvSurface->enColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888))
    {
        pHWNode->DES_CTRL.bits.des_argb_order = pDrvSurface->enRgbOrder;
#if 1
        if (pDrvSurface->enColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888)
        {
            pDrvSurface->enColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;
            pHWNode->DES_CTRL.bits.des_argb_order= 0x17;
        }
  #endif
    }
    if(TDE_DRV_COLOR_FMT_RABG8888 == pDrvSurface->enColorFmt)
    {
        pHWNode->DES_CTRL.bits.des_argb_order = TDE_DRV_ORDER_RABG; //RABG
        pHWNode->DES_CTRL.bits.des_fmt = TDE_DRV_COLOR_FMT_ARGB8888;
    }

    u32Bpp = TdeHalGetbppByFmt(pDrvSurface->enColorFmt);
    pHWNode->DES_CH0_ADDR.bits.des_ch0_addr = pDrvSurface->u32PhyAddr + pDrvSurface->u32Ypos*pDrvSurface->u32Pitch\
                                                                                           +(pDrvSurface->u32Xpos*u32Bpp>>3);

    if(pDrvSurface->enColorFmt==TDE_DRV_COLOR_FMT_YCbCr422)
    {
        pHWNode->DES_DSWM.bits.des_h_dswm_mode = 1;
    }
    pHWNode->DES_CH0_STRIDE.bits.des_ch0_stride = (HI_U32)pDrvSurface->u32Pitch;
    return;
}

STATIC HI_BOOL TdeHalIsOperationSupport(TDE_DRV_ALU_MODE_E enAlu, HI_U32 u32Capability, HI_U32 u32OperMode)
{
    if ( !(MASKBLEND & u32OperMode) && !(MASKROP & u32OperMode) )
    {
        return ( (u32Capability & u32OperMode) ? HI_TRUE : HI_FALSE );
    }

    if ( MASKBLEND & u32OperMode )
    {
        return  !((TDE_ALU_MASK_BLEND == enAlu) && (!(u32Capability & u32OperMode)));
    }

    if (MASKROP & u32OperMode)
    {
        return !(((TDE_ALU_MASK_ROP1 == enAlu) || (TDE_ALU_MASK_ROP2 == enAlu)) && (!(u32Capability & u32OperMode)));
    }

    return HI_TRUE;
}
/*****************************************************************************
* Function:      TdeHalNodeSetBaseOperate
* Description:   set base operation
* Input:         pHWNode: pointer of node
*                enMode: base operation mode
*                enAlu: ALU mode
*                pstColorFill:fill color value
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetBaseOperate(TDE_HWNode_S* pHWNode, TDE_DRV_BASEOPT_MODE_E enMode,
                                TDE_DRV_ALU_MODE_E enAlu, TDE_DRV_COLORFILL_S* pstColorFill)
{
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);

    TdeHalGetCapability(&u32Capability);

    switch (enMode)
    {
        case TDE_QUIKE_FILL:
        {
            TDE_ASSERT(HI_NULL != pstColorFill);

            if (!TdeHalIsOperationSupport(enAlu, u32Capability , QUICKFILL))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support QuickFill\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }
            TDE_FILL_DATA_BY_FMT(pHWNode->SRC1_FILL.bits.src1_color_fill,pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);
            pHWNode->SRC1_CTRL.bits.src1_en = 1;
            pHWNode->SRC1_CTRL.bits.src1_mode = 1;
            pHWNode->SRC1_CTRL.bits.src1_dma = 1;
        }
        break;

       #ifndef HI_BUILD_IN_BOOT
	    case TDE_QUIKE_COPY:/*quick copy*//*CNcomment:快速拷贝*/
        {
            if (!TdeHalIsOperationSupport(enAlu, u32Capability , QUICKCOPY))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support QuickCopy\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }
            pHWNode->SRC1_CTRL.bits.src1_en = 1;
            pHWNode->SRC1_CTRL.bits.src1_dma = 1;
        }
        break;
	    case TDE_NORM_FILL_1OPT:/*signal fill*//*CNcomment:普通单源填充*/
        {
            TDE_ASSERT(HI_NULL != pstColorFill);
            TDE_FILL_DATA_BY_FMT(pHWNode->SRC2_FILL.bits.src2_color_fill, pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);
            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 1;
            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;
            pHWNode->CBMCTRL.bits.alu_mode = 1;
        }
        break;
	    case TDE_NORM_FILL_2OPT:/*signal color with bitmap operation and blit*//*CNcomment:单色和位图运算操作搬移*/
        {
            TDE_ASSERT(HI_NULL != pstColorFill);
            TDE_FILL_DATA_BY_FMT(pHWNode->SRC2_FILL.bits.src2_color_fill, pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);
            pHWNode->SRC1_CTRL.bits.src1_en = 1;
            pHWNode->SRC1_CTRL.bits.src1_mode = 0;
            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 1;
            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;

            pHWNode->CBMCTRL.bits.alu_mode = s_u32CbmctrlAlu_mode[enAlu];

        }
        break;
      #endif
            case TDE_NORM_BLIT_1OPT:/*signal blit*//*CNcomment:普通单源操作搬移*/
        {
            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 0;
            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;
            pHWNode->CBMCTRL.bits.alu_mode = 1;
        }
        break;

        case TDE_NORM_BLIT_2OPT:
        {
            if (!TdeHalIsOperationSupport(enAlu, u32Capability , MASKROP))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support MaskRop\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }

            if (!TdeHalIsOperationSupport(enAlu, u32Capability , MASKBLEND))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support MaskBlend\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }

            pHWNode->SRC1_CTRL.bits.src1_en = 1;
            pHWNode->SRC1_CTRL.bits.src1_mode = 0;

            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 0;

            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;


            pHWNode->CBMCTRL.bits.alu_mode = s_u32CbmctrlAlu_mode[enAlu];

        }
        break;

        case TDE_MB_2OPT:
        {

            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 0;

            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;
            pHWNode->CBMCTRL.bits.alu_mode =  0x1;
        }
        break;
#ifndef HI_BUILD_IN_BOOT

        case TDE_SINGLE_SRC_PATTERN_FILL_OPT:
        {
            if (!TdeHalIsOperationSupport(enAlu, u32Capability , PATTERFILL))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support PatternFill\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }

               pHWNode->SRC2_CTRL.bits.src2_en = 1;
               pHWNode->SRC2_CTRL.bits.src2_mode = 2;
               pHWNode->CBMCTRL.bits.cbm_en = 1;
               pHWNode->CBMCTRL.bits.cbm_mode = 1;

            pHWNode->CBMCTRL.bits.alu_mode = s_u32CbmctrlAlu_mode[enAlu];

            break;
        }

        case TDE_DOUBLE_SRC_PATTERN_FILL_OPT:
        {
            if (!TdeHalIsOperationSupport(enAlu, u32Capability , PATTERFILL))
            {
                TDE_TRACE(TDE_KERN_INFO, "It deos not support PatternFill\n");
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }

            pHWNode->SRC1_CTRL.bits.src1_en = 1;
            pHWNode->SRC1_CTRL.bits.src1_mode = 0;

            pHWNode->SRC2_CTRL.bits.src2_en = 1;
            pHWNode->SRC2_CTRL.bits.src2_mode = 2;
            pHWNode->CBMCTRL.bits.cbm_en = 1;
            pHWNode->CBMCTRL.bits.cbm_mode = 1;

            pHWNode->CBMCTRL.bits.alu_mode = s_u32CbmctrlAlu_mode[enAlu];

            break;
        }

#endif

        default:
            break;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeSetGlobalAlpha
* Description:   set alpha blend
* Input:         pHWNode: pointer of node
*                u8Alpha: Alpha value
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetGlobalAlpha(TDE_HWNode_S* pHWNode, HI_U8 u8Alpha, HI_BOOL bEnable)
{
#ifndef HI_BUILD_IN_BOOT
    TDE_ASSERT(HI_NULL != pHWNode);

    /*set node*//*CNcomment:配置缓存节点*/
   // pHWNode->SRC1_CBMPARA.bits.s1_galpha = u8Alpha;
    pHWNode->SRC2_CBMPARA.bits.s2_galpha = u8Alpha;

#endif
    return;
}

HI_VOID TdeHalNodeSetSrc1Alpha(TDE_HWNode_S* pHWNode)
{
    TDE_ASSERT(HI_NULL != pHWNode);

    /*set alpha0 alpha1*//*CNcomment:配置alpha0, alpha1*/

    pHWNode->SRC1_ALPHA.bits.src1_alpha0 = 0;
    pHWNode->SRC1_ALPHA.bits.src1_alpha1 = 0xff;

    return;
}

HI_VOID TdeHalNodeSetSrc2Alpha(TDE_HWNode_S* pHWNode)
{
    TDE_ASSERT(HI_NULL != pHWNode);

    /*set alpha0 alpha1*//*CNcomment:配置alpha0, alpha1*/

    pHWNode->SRC2_ALPHA.bits.src2_alpha0 = 0;
    pHWNode->SRC2_ALPHA.bits.src2_alpha1 = 0xff;

    return;
}
/*****************************************************************************
* Function:      TdeHalNodeSetExpAlpha
* Description:   extend to alpha0 and alpha1 operation when extend alpha for RGB5551
* Input:         pHWNode:pointer of node
*                u8Alpha0: Alpha0 value
*                u8Alpha1: Alpha1 value
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetExpAlpha(TDE_HWNode_S* pHWNode, TDE_DRV_SRC_E enSrc, HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    TDE_ASSERT(HI_NULL != pHWNode);

    /*set alpha0 alpha1*//*CNcomment:配置alpha0, alpha1*/
    if(TDE_DRV_SRC_S1 & enSrc)
    {
        pHWNode->SRC1_ALPHA.bits.src1_alpha0 = u8Alpha0;
        pHWNode->SRC1_ALPHA.bits.src1_alpha1 = u8Alpha1;
        pHWNode->SRC1_CTRL.bits.src1_rgb_exp = 3;
    }

    if(TDE_DRV_SRC_S2 & enSrc)
    {
        pHWNode->SRC2_ALPHA.bits.src2_alpha0 = u8Alpha0;
        pHWNode->SRC2_ALPHA.bits.src2_alpha1 = u8Alpha1;
        pHWNode->SRC2_CTRL.bits.src2_rgb_exp = 3;
    }

    return;
}

#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeHalNodeSetAlphaBorder
* Description:   set deal with the Alpha side to enable or disable
* Input:         pHWNode: pointer of node
*                bEnable: enable true or false
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetAlphaBorder(TDE_HWNode_S* pHWNode, HI_BOOL bVEnable, HI_BOOL bHEnable)
{
#if 0
    TDE_2D_RSZ_U unRsz;
    TDE_ASSERT(HI_NULL != pHWNode);
    unRsz.u32All = pHWNode->u32TDE_2D_RSZ;
    unRsz.stBits.u32AlpBorder = (((HI_U32)bVEnable << 1) | (HI_U32)bHEnable);
    return;
#endif
}
#endif

/*****************************************************************************
* Function:      TdeHalNodeSetRop
* Description:   set rop operation parameter
* Input:         pHWNode: pointer of node
*                enRopCode: ROP code
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetRop(TDE_HWNode_S* pHWNode, TDE2_ROP_CODE_E enRgbRop, TDE2_ROP_CODE_E enAlphaRop)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&ROP))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Rop\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    /*set node*//*CNcomment:配置缓存节点*/

    pHWNode->CBMCTRL.bits.cbm_en = 1;
    pHWNode->CBMCTRL.bits.cbm_mode = 1;

    pHWNode->CBMALUPARA.bits.rgb_rop = (HI_U32)enRgbRop;
    pHWNode->CBMALUPARA.bits.a_rop = (HI_U32)enAlphaRop;
#endif
    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeSetBlend
* Description:   set blend operation parameter
* Input:         pHWNode: pointer of node
*                stBlendOpt:blend operation
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetBlend(TDE_HWNode_S *pHWNode, TDE2_BLEND_OPT_S *pstBlendOpt)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);


    TDE_HAL_CHECK_BLEND_CMD(pstBlendOpt->eBlendCmd);

    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&ALPHABLEND))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Blend\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    pHWNode->SRC1_CBMPARA.bits.s1_palphaen = HI_TRUE;
    pHWNode->SRC1_CBMPARA.bits.s1_galphaen = HI_FALSE;
    pHWNode->SRC1_CBMPARA.bits.s1_multiglobalen = HI_FALSE;
    pHWNode->SRC2_CBMPARA.bits.s2_multiglobalen = HI_FALSE;

    pHWNode->SRC1_CBMPARA.bits.s1_premulten = pstBlendOpt->bSrc1AlphaPremulti;
    pHWNode->SRC2_CBMPARA.bits.s2_premulten = pstBlendOpt->bSrc2AlphaPremulti;
    pHWNode->SRC2_CBMPARA.bits.s2_palphaen = pstBlendOpt->bPixelAlphaEnable;
    pHWNode->SRC2_CBMPARA.bits.s2_galphaen = pstBlendOpt->bGlobalAlphaEnable;

    /*set mode for src1 and src2*//*CNcomment:  配置Src1、Src2模式 */
    if ( pstBlendOpt->eBlendCmd >= TDE2_BLENDCMD_CONFIG )
    {
        pHWNode->SRC1_CBMPARA.bits.s1_blendmode = pstBlendOpt->eSrc1BlendMode;
        pHWNode->SRC2_CBMPARA.bits.s2_blendmode = pstBlendOpt->eSrc2BlendMode;
    }
    else
    {
        pHWNode->SRC1_CBMPARA.bits.s1_blendmode = s_u32Src1BlendCmdMode[pstBlendOpt->eBlendCmd];
        pHWNode->SRC2_CBMPARA.bits.s2_blendmode = s_u32Src2BlendCmdMode[pstBlendOpt->eBlendCmd];

    }
    pHWNode->CBMCTRL.bits.cbm_en = 1;
    pHWNode->CBMCTRL.bits.cbm_mode = 1;
 #endif
    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeSetColorize
* Description:  set blend operation parmeter
* Input:         pHWNode: point of node
*                u32Colorize:Co
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetColorize(TDE_HWNode_S *pHWNode, HI_U32 u32Colorize)
{
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&COLORIZE))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Colorize\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    pHWNode->CBMCOLORIZE.bits.colorizeen = 1;
    pHWNode->CBMCOLORIZE.bits.colorizeb = u32Colorize & 0xff;
    pHWNode->CBMCOLORIZE.bits.colorizeg = (u32Colorize >> 8)&0xff;
    pHWNode->CBMCOLORIZE.bits.colorizer = (u32Colorize >> 16)&0xff;
    return HI_SUCCESS;
}

HI_VOID TdeHalNodeEnableAlphaRop(TDE_HWNode_S *pHWNode)
{
    pHWNode->CBMALUPARA.bits.blendropen = 1;
    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetColorExp
* Description:   set color extend or color revise parameter
* Input:         pHWNode: pointer of node
*                pClutCmd: Clut operation parameter
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetClutOpt(TDE_HWNode_S* pHWNode, TDE_DRV_CLUT_CMD_S* pClutCmd, HI_BOOL bReload)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pClutCmd);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CLUT))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Clut\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    pHWNode->SRC2_CTRL.bits.src2_clut_mode = (HI_U32)pClutCmd->enClutMode;
     /*set node*//*CNcomment:配置缓存节点*/
    if(pClutCmd->u32PhyClutAddr)
    {
        pHWNode->TDE_CLUT_ADDR = pClutCmd->u32PhyClutAddr;
    }


#endif
    return HI_SUCCESS;
}
#ifndef HI_BUILD_IN_BOOT

STATIC HI_VOID TdeHalNodeSetYcbcrColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey)
{
    TDE2_COLORKEY_COMP_S stCr    = pColorKey->unColorKeyValue.struCkYCbCr.stCr;
    TDE2_COLORKEY_COMP_S stCb    = pColorKey->unColorKeyValue.struCkYCbCr.stCb;
    TDE2_COLORKEY_COMP_S stY     = pColorKey->unColorKeyValue.struCkYCbCr.stY;
    TDE2_COLORKEY_COMP_S stAlpha = pColorKey->unColorKeyValue.struCkYCbCr.stAlpha;

    pHWNode->CBMKEYMIN.u32All  = TdeHalGetYCbCrKeyMask(stCr.u8CompMin, stCb.u8CompMin, stY.u8CompMin, stAlpha.u8CompMin);
    pHWNode->CBMKEYMAX.u32All  = TdeHalGetYCbCrKeyMask(stCr.u8CompMax, stCb.u8CompMax, stY.u8CompMax, stAlpha.u8CompMax);
    pHWNode->CBMKEYMASK.u32All = TdeHalGetYCbCrKeyMask(stCr.u8CompMask, stCb.u8CompMask, stY.u8CompMask, stAlpha.u8CompMask);

    pHWNode->CBMKEYPARA.bits.keybmode = TdeHalGetColorKeyMode(&stCr);
    pHWNode->CBMKEYPARA.bits.keygmode = TdeHalGetColorKeyMode(&stCb);
    pHWNode->CBMKEYPARA.bits.keyrmode = TdeHalGetColorKeyMode(&stY) ;
    pHWNode->CBMKEYPARA.bits.keyamode = TdeHalGetColorKeyMode(&stAlpha) ;

    return;
}

STATIC HI_VOID TdeHalNodeSetCLUTColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey)
{
    TDE2_COLORKEY_COMP_S stClut    = pColorKey->unColorKeyValue.struCkClut.stClut;
    TDE2_COLORKEY_COMP_S stAlpha = pColorKey->unColorKeyValue.struCkClut.stAlpha;

    pHWNode->CBMKEYMIN.u32All  = TdeHalGetClutKeyMask(stClut.u8CompMin, stAlpha.u8CompMin);
    pHWNode->CBMKEYMAX.u32All  = TdeHalGetClutKeyMask(stClut.u8CompMax, stAlpha.u8CompMax);
    pHWNode->CBMKEYMASK.u32All = TdeHalGetClutKeyMask(stClut.u8CompMask, stAlpha.u8CompMask);

    pHWNode->CBMKEYPARA.bits.keybmode = TdeHalGetColorKeyMode(&stClut) ;
    pHWNode->CBMKEYPARA.bits.keyamode = TdeHalGetColorKeyMode(&stAlpha) ;

    return;
}

STATIC HI_VOID TdeHalNodeSetARGBColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey)
{
    TDE2_COLORKEY_COMP_S stBlue    = pColorKey->unColorKeyValue.struCkARGB.stBlue;
    TDE2_COLORKEY_COMP_S stGreen   = pColorKey->unColorKeyValue.struCkARGB.stGreen;
    TDE2_COLORKEY_COMP_S stRed     = pColorKey->unColorKeyValue.struCkARGB.stRed;
    TDE2_COLORKEY_COMP_S stAlpha   = pColorKey->unColorKeyValue.struCkARGB.stAlpha;

    pHWNode->CBMKEYMIN.u32All  = TdeHalGetArgbKeyMask(stBlue.u8CompMin, stGreen.u8CompMin,stRed.u8CompMin,stAlpha.u8CompMin);
    pHWNode->CBMKEYMAX.u32All  = TdeHalGetArgbKeyMask(stBlue.u8CompMax, stGreen.u8CompMax,stRed.u8CompMax,stAlpha.u8CompMax);
    pHWNode->CBMKEYMASK.u32All = TdeHalGetArgbKeyMask(stBlue.u8CompMask, stGreen.u8CompMask,stRed.u8CompMask,stAlpha.u8CompMask);

    pHWNode->CBMKEYPARA.bits.keybmode = TdeHalGetColorKeyMode(&stBlue);
    pHWNode->CBMKEYPARA.bits.keygmode = TdeHalGetColorKeyMode(&stGreen);
    pHWNode->CBMKEYPARA.bits.keyrmode = TdeHalGetColorKeyMode(&stRed) ;
    pHWNode->CBMKEYPARA.bits.keyamode = TdeHalGetColorKeyMode(&stAlpha) ;

    return;
}
STATIC HI_U32 TdeHalGetColorKeyMode(TDE2_COLORKEY_COMP_S *stColorKey)
{
    return (stColorKey->bCompIgnore) ? TDE_COLORKEY_IGNORE   : (stColorKey->bCompOut)
                                     ? TDE_COLORKEY_AREA_OUT : TDE_COLORKEY_AREA_IN;
}

STATIC HI_U32 TdeHalGetYCbCrKeyMask(HI_U8 u8Cr, HI_U8 u8Cb, HI_U8 u8Cy, HI_U8 u8Alpha)
{
    return (HI_U32)(u8Cr | (u8Cb << TDE_EIGHT_BITS_SHIFT) | (u8Cy << TDE_SIXTEEN_BITS_SHIFT) | (u8Alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}
STATIC HI_U32 TdeHalGetClutKeyMask(HI_U8 u8Clut, HI_U8 u8Alpha)
{
    return (HI_U32)(u8Clut | (u8Alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}
STATIC HI_U32 TdeHalGetArgbKeyMask(HI_U8 u8Blue, HI_U8 u8Green, HI_U8 u8Red, HI_U8 u8Alpha)
{
    return (HI_U32)(u8Blue | (u8Green << TDE_EIGHT_BITS_SHIFT) | (u8Red << TDE_SIXTEEN_BITS_SHIFT) | (u8Alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}

#endif
/*****************************************************************************
* Function:      TdeHalNodeSetColorKey
* Description:  set parameter for color key operation  according color format
* Input:         pHWNode:pointer of node
*                enFmt: color format
*                pColorKey: pointer of color key value
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetColorKey(TDE_HWNode_S* pHWNode, TDE_COLORFMT_CATEGORY_E enFmtCat,
                             TDE_DRV_COLORKEY_CMD_S* pColorKey)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pColorKey);

    TdeHalGetCapability(&u32Capability);

    if (!(u32Capability & COLORKEY))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support ColorKey\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    pHWNode->CBMKEYPARA.bits.keysel = (HI_U32)pColorKey->enColorKeyMode;
    pHWNode->CBMKEYPARA.bits.keyen = 1;

    if (TDE_COLORFMT_CATEGORY_ARGB == enFmtCat)
    {
        TdeHalNodeSetARGBColorKeyPara(pHWNode, pColorKey);
    }
    else if (TDE_COLORFMT_CATEGORY_CLUT == enFmtCat)
    {
        TdeHalNodeSetCLUTColorKeyPara(pHWNode, pColorKey);
    }
    else if (TDE_COLORFMT_CATEGORY_YCbCr == enFmtCat)
    {
        TdeHalNodeSetYcbcrColorKeyPara(pHWNode, pColorKey);
    }
    else
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support ColorKey\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    pHWNode->CBMCTRL.bits.cbm_en = 1;
    pHWNode->CBMCTRL.bits.cbm_mode = 1;
#endif
    return HI_SUCCESS;
}
/*****************************************************************************
* Function:      TdeHalNodeSetClipping
* Description:  set clip operation parameter
* Input:         pHWNode: pointer of node
*                pClip: clip range
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetClipping(TDE_HWNode_S* pHWNode, TDE_DRV_CLIP_CMD_S* pClip)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pClip);

    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CLIP))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Clip\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    pHWNode->DES_ALPHA.bits.des_crop_mode = 0;
    if(!pClip->bInsideClip)
    {
        /*over clip*//*CNcomment: 区域外clip指示 */
        pHWNode->DES_ALPHA.bits.des_crop_mode = 1;
    }
    pHWNode->DES_ALPHA.bits.des_crop_en = 1;
    pHWNode->DES_CROP_POS_ST.bits.des_crop_start_x = pClip->u16ClipStartX;
    pHWNode->DES_CROP_POS_ST.bits.des_crop_start_y = pClip->u16ClipStartY;
    pHWNode->DES_CROP_POS_ED.bits.des_crop_end_x = pClip->u16ClipEndX;
    pHWNode->DES_CROP_POS_ED.bits.des_crop_end_y = pClip->u16ClipEndY;

#endif
    return HI_SUCCESS;
}


#if 0
HI_S32 TdeHalNodeSetSrc1Resize(TDE_HWNode_S* pHWNode, TDE_FILTER_OPT* pstFilterOpt, TDE_NODE_SUBM_TYPE_E enNodeType)
{
    pHWNode->SRC1_HSP.bits.hchmsc_en= 1;
    pHWNode->SRC1_HSP.bits.hlmsc_en = 1;

    if(pstFilterOpt->u32HStep!=NO_HSCALE_STEP)
   {

        pHWNode->SRC1_HSP.bits.hfir_order = 0;
        pHWNode->SRC1_HSP.bits.hafir_en = 1;
        pHWNode->SRC1_HSP.bits.hlfir_en = 1;
        pHWNode->SRC1_HSP.bits.hchfir_en =1;

        pHWNode->TDE_SRC1_ZME_CHADDR= s_stParaTable.u32HfCoefAddr + TdeHalGetResizeParaHTable(pstFilterOpt->u32HStep) * TDE_PARA_HTABLE_SIZE;
        pHWNode->TDE_SRC1_ZME_LHADDR = pHWNode->TDE_SRC1_ZME_CHADDR;

   }

    pHWNode->SRC1_VSP.bits.vchmsc_en = 1;
    pHWNode->SRC1_VSP.bits.vlmsc_en = 1;

   if(pstFilterOpt->u32VStep!=NO_VSCALE_STEP)
   {

        pHWNode->SRC1_VSP.bits.vafir_en = 1;
        pHWNode->SRC1_VSP.bits.vlfir_en = 1;
        pHWNode->SRC1_VSP.bits.vchfir_en = 1;

        pHWNode->TDE_SRC1_ZME_CVADDR = s_stParaTable.u32VfCoefAddr + TdeHalGetResizeParaVTable(pstFilterOpt->u32VStep)* TDE_PARA_VTABLE_SIZE;
        pHWNode->TDE_SRC1_ZME_LVADDR = pHWNode->TDE_SRC1_ZME_CVADDR;
   }
   pHWNode->SRC1_VSP.bits.zme_in_fmt = pstFilterOpt->u32ZmeInFmt;
   pHWNode->SRC1_VSP.bits.zme_out_fmt = pstFilterOpt->u32ZmeOutFmt;
   pHWNode->SRC1_HSP.bits.hratio = pstFilterOpt->u32HStep;
   pHWNode->SRC1_VSR.bits.vratio = pstFilterOpt->u32VStep;

   pHWNode->SRC1_ZMEIRESO.bits.iw = pstFilterOpt->u32HpzmeWidth;
   pHWNode->SRC1_HPZME.bits.src1_hpzme_en = pstFilterOpt->u32HpzmEn;
   pHWNode->SRC1_HPZME.bits.src1_hpzme_mode = pstFilterOpt->u32HpzmeMod;


   pHWNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width = pstFilterOpt->u32HpzmeWidth - 1;
   pHWNode->SRC1_HLOFFSET = pstFilterOpt->s32HOffset;


    return HI_SUCCESS;
}
#endif
/*****************************************************************************
* Function:      TdeHalNodeSetResize
* Description:   set resize operation parameter
* Input:         pHWNode: pointer of node
*                pResize: resize coefficient
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
#if 0
HI_S32 TdeHalNodeSetResize(TDE_HWNode_S* pHWNode, TDE_FILTER_OPT* pstFilterOpt, TDE_NODE_SUBM_TYPE_E enNodeType)
{

    pHWNode->SRC2_HSP.bits.hchmsc_en= 1;
    pHWNode->SRC2_HSP.bits.hlmsc_en = 1;

    if(pstFilterOpt->u32HStep!=NO_HSCALE_STEP)
   {

        pHWNode->SRC2_HSP.bits.hfir_order = 0;
        pHWNode->SRC2_HSP.bits.hafir_en = 1;
        pHWNode->SRC2_HSP.bits.hlfir_en = 1;
        pHWNode->SRC2_HSP.bits.hchfir_en =1;

        pHWNode->TDE_SRC2_ZME_CHADDR= s_stParaTable.u32HfCoefAddr + TdeHalGetResizeParaHTable(pstFilterOpt->u32HStep) * TDE_PARA_HTABLE_SIZE;
        pHWNode->TDE_SRC2_ZME_LHADDR = pHWNode->TDE_SRC2_ZME_CHADDR;

   }

    pHWNode->SRC2_VSP.bits.vchmsc_en = 1;
    pHWNode->SRC2_VSP.bits.vlmsc_en = 1;

   if((pstFilterOpt->u32VStep!=NO_VSCALE_STEP)&&(!pstFilterOpt->bDeflicker))
   {

        pHWNode->SRC2_VSP.bits.vafir_en = 1;
        pHWNode->SRC2_VSP.bits.vlfir_en = 1;
        pHWNode->SRC2_VSP.bits.vchfir_en = 1;
        pHWNode->TDE_SRC2_ZME_CVADDR = s_stParaTable.u32VfCoefAddr + TdeHalGetResizeParaVTable(pstFilterOpt->u32VStep)* TDE_PARA_VTABLE_SIZE;
        pHWNode->TDE_SRC2_ZME_LVADDR = pHWNode->TDE_SRC2_ZME_CVADDR;
   }

    if(pstFilterOpt->bDeflicker)
    {
        pHWNode->SRC2_VSP.bits.vafir_en = 1;
        pHWNode->SRC2_VSP.bits.vlfir_en = 1;
        pHWNode->SRC2_VSP.bits.vchfir_en = 1;
        pHWNode->TDE_SRC2_ZME_CVADDR = s_stParaTable.u32DeflickerVfCoefAddr + TdeHalGetResizeParaVTable(pstFilterOpt->u32VStep)* TDE_PARA_VTABLE_SIZE;
        pHWNode->TDE_SRC2_ZME_LVADDR = pHWNode->TDE_SRC2_ZME_CVADDR;
    }

   pHWNode->SRC2_VSP.bits.zme_in_fmt = pstFilterOpt->u32ZmeInFmt;
   pHWNode->SRC2_VSP.bits.zme_out_fmt = pstFilterOpt->u32ZmeOutFmt;
   pHWNode->SRC2_HSP.bits.hratio = pstFilterOpt->u32HStep;
   pHWNode->SRC2_VSR.bits.vratio = pstFilterOpt->u32VStep;
   pHWNode->SRC2_ZMEIRESO.bits.iw = pstFilterOpt->u32HpzmeWidth - 1;
   pHWNode->SRC2_HPZME.bits.src2_hpzme_en = pstFilterOpt->u32HpzmEn;
   pHWNode->SRC2_HPZME.bits.src2_hpzme_mode = pstFilterOpt->u32HpzmeMod;

   pHWNode->SRC2_VOFFSET.bits.vluma_offset = pstFilterOpt->s32VOffset;


   pHWNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width = pstFilterOpt->u32HpzmeWidth - 1;
   pHWNode->SRC2_HLOFFSET = pstFilterOpt->s32HOffset;
    return HI_SUCCESS;
}
#endif

 STATIC HI_U32 TdeHalGetZmeInFmt( TDE_DRV_COLOR_FMT_E enInDrvFmt )
 {
     HI_U32 u32ZmeInFmt;

     HI_BOOL bFmtArgb = (enInDrvFmt <= TDE_DRV_COLOR_FMT_ACLUT88) || (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr444MB)\
                        || (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr400MBP);
     HI_BOOL bFmtYuv422 = (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr422MBH) || (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr422);
     HI_BOOL bFmtYuv420 = (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr420MB) || (enInDrvFmt == TDE_DRV_COLOR_FMT_YCbCr422MBV);

     if ( bFmtArgb )     { u32ZmeInFmt =  TDE_ZME_FMT_ARGB; }

     if ( bFmtYuv422 )   { u32ZmeInFmt = TDE_ZME_FMT_YUV422; }

     if ( bFmtYuv420 )   { u32ZmeInFmt = TDE_ZME_FMT_YUV420; }

     return u32ZmeInFmt;
 }

 STATIC HI_U32 TdeHalGetHpzmeMode( HI_U32 u32OutRectWidth , HI_U32 u32InRectWidth)
 {
     HI_U32 u32HpzmeMode;
     if ((u32OutRectWidth * 32) >= u32InRectWidth)
     {
         u32HpzmeMode = 1;
     }
     else if ((u32OutRectWidth * 64) >= u32InRectWidth)
     {
         u32HpzmeMode = 3;
     }
     else if ((u32OutRectWidth * 128) >= u32InRectWidth)
     {
         u32HpzmeMode = 7;
     }
     else
     {
         u32HpzmeMode = 15;
     }

     return u32HpzmeMode;
 }
 STATIC HI_U32 TdeHalGetZoomMode( TDE2_FILTER_MODE_E enFilterMode)
 {
     HI_BOOL bFilterModeColor = ( TDE2_FILTER_MODE_COLOR == enFilterMode );
     HI_BOOL bFilterModeAlpha = ( TDE2_FILTER_MODE_ALPHA == enFilterMode );
     HI_BOOL bFilterModeBoth  = ( TDE2_FILTER_MODE_BOTH  == enFilterMode );
     HI_U32 u32FirstBit, u32SecondBit, u32ThirdBit;

     if (bFilterModeColor)
     {
         u32FirstBit  = 1;
         u32SecondBit = 1;
         u32ThirdBit  = 0;
     }
     else if (bFilterModeAlpha)
     {
         u32FirstBit  = 0;
         u32SecondBit = 0;
         u32ThirdBit  = 1;
     }
     else if (bFilterModeBoth)
     {
         u32FirstBit  = 1;
         u32SecondBit = 1;
         u32ThirdBit  = 1;
     }
     else
     {
         u32FirstBit  = 0;
         u32SecondBit = 0;
         u32ThirdBit  = 0;
     }

     return (u32ThirdBit << TDE_HAL_TWO_BIT_SHIFT) + ( u32SecondBit << TDE_HAL_ONE_BIT_SHIFT ) + (u32FirstBit << TDE_HAL_ZERO_BIT_SHIFT);

 }
 STATIC HI_U32 TdeHalGetHeightScaleCoff(HI_U32 u32OutRectHeight , HI_U32 u32InRectHeight )
 {
     HI_U32 u32HeightScaleCoff = 1;

     if ((u32OutRectHeight * 32) >= u32InRectHeight)
     {
         u32HeightScaleCoff = 2;
     }
     else if ((u32OutRectHeight * 64) >= u32InRectHeight)
     {
         u32HeightScaleCoff = 4;
     }
     else if ((u32OutRectHeight * 128) >= u32InRectHeight)
     {
         u32HeightScaleCoff = 8;
     }

     return u32HeightScaleCoff;
 }

HI_VOID TdeOsiCalcSrc1FilterOpt(TDE_HWNode_S* pNode, TDE2_COLOR_FMT_E enInFmt, TDE2_COLOR_FMT_E enOutFmt,
                                TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect, TDE2_FILTER_MODE_E enFilterMode)
{
    TDE_DRV_COLOR_FMT_E enInDrvFmt;
    HI_U32 u32TempZoomMode;

    enInDrvFmt = g_enTdeCommonDrvColorFmt[enInFmt];


    pNode->SRC1_VSP.bits.zme_in_fmt =  TdeHalGetZmeInFmt( enInDrvFmt );

    pNode->SRC1_VSP.bits.zme_out_fmt = (pNode->SRC1_VSP.bits.zme_in_fmt == TDE_ZME_FMT_ARGB) ? TDE_ZME_FMT_ARGB : TDE_ZME_FMT_YUV444;


    pNode->SRC1_IMGSIZE.bits.src1_width = pInRect->u32Width - 1;
    pNode->SRC1_ZMEIRESO.bits.iw = pInRect->u32Width - 1;
    pNode->SRC1_ZMEORESO.bits.ow = pOutRect->u32Width - 1;

    pNode->SRC1_IMGSIZE.bits.src1_height = pInRect->u32Height - 1;
    pNode->SRC1_ZMEIRESO.bits.ih = pInRect->u32Height - 1;
    pNode->SRC1_ZMEORESO.bits.oh = pOutRect->u32Height - 1;

    pNode->SRC1_HSP.bits.hratio = (1 >= pOutRect->u32Width) ? 0 : ((pInRect->u32Width) << TDE_HAL_HSTEP_FLOATLEN) / (pOutRect->u32Width);

    pNode->SRC1_VSR.bits.vratio = (1 >= pOutRect->u32Height) ? 0 : ((pInRect->u32Height) << TDE_FLOAT_BITLEN) / (pOutRect->u32Height);

    pNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width = pInRect->u32Width;

    if ((pOutRect->u32Width * 16) <= pInRect->u32Width)
    {
        pNode->SRC1_HPZME.bits.src1_hpzme_en = 1;

        pNode->SRC1_HPZME.bits.src1_hpzme_mode = TdeHalGetHpzmeMode( pOutRect->u32Width , pInRect->u32Width );

        pNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width = pInRect->u32Width / (pNode->SRC1_HPZME.bits.src1_hpzme_mode + 1)\
                + ((pInRect->u32Width % (pNode->SRC1_HPZME.bits.src1_hpzme_mode + 1)) ? 1 : 0);
    }

    pNode->SRC1_HSP.bits.hchmsc_en = 1;
    pNode->SRC1_HSP.bits.hlmsc_en = 1;

    u32TempZoomMode = TdeHalGetZoomMode( enFilterMode );

    if (pNode->SRC1_HSP.bits.hratio != NO_HSCALE_STEP)
    {
        pNode->SRC1_HSP.bits.hfir_order = 0;

        pNode->SRC1_HSP.bits.hafir_en  = (u32TempZoomMode >> TDE_HAL_TWO_BIT_SHIFT)   & 0x1 ;
        pNode->SRC1_HSP.bits.hlfir_en  = (u32TempZoomMode >> TDE_HAL_ONE_BIT_SHIFT)   & 0x1 ;
        pNode->SRC1_HSP.bits.hchfir_en = (u32TempZoomMode >> TDE_HAL_ZERO_BIT_SHIFT)  & 0x1 ;

        pNode->TDE_SRC1_ZME_CHADDR = s_stParaTable.u32HfCoefAddr + TdeHalGetResizeParaHTable(pNode->SRC1_HSP.bits.hratio) * TDE_PARA_HTABLE_SIZE;
        pNode->TDE_SRC1_ZME_LHADDR = pNode->TDE_SRC1_ZME_CHADDR;

   }

    pNode->SRC1_VSP.bits.vchmsc_en = 1;
    pNode->SRC1_VSP.bits.vlmsc_en = 1;

    if (pNode->SRC1_VSR.bits.vratio != NO_VSCALE_STEP)
    {
        pNode->SRC1_VSP.bits.vafir_en  = (u32TempZoomMode >> TDE_HAL_TWO_BIT_SHIFT)   & 0x1 ;
        pNode->SRC1_VSP.bits.vlfir_en  = (u32TempZoomMode >> TDE_HAL_ONE_BIT_SHIFT)   & 0x1 ;
        pNode->SRC1_VSP.bits.vchfir_en = (u32TempZoomMode >> TDE_HAL_ZERO_BIT_SHIFT)  & 0x1 ;

        pNode->TDE_SRC1_ZME_CVADDR = s_stParaTable.u32VfCoefAddr + TdeHalGetResizeParaVTable(pNode->SRC1_VSR.bits.vratio) * TDE_PARA_VTABLE_SIZE;
        pNode->TDE_SRC1_ZME_LVADDR = pNode->TDE_SRC1_ZME_CVADDR;
   }


    pNode->SRC1_ZMEIRESO.bits.iw = pNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width - 1;


    pNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width = pNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width - 1;


    return;
}


HI_VOID TdeOsiCalcSrc2FilterOpt(TDE_HWNode_S* pNode, TDE2_COLOR_FMT_E enInFmt, TDE2_COLOR_FMT_E enOutFmt,
                                TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect, HI_BOOL bDefilicker, TDE2_FILTER_MODE_E enFilterMode)
{
    TDE_DRV_COLOR_FMT_E enInDrvFmt;
    HI_U32 u32TempZoomMode;
    HI_U32 u32TempHeightScaleCoff;

    enInDrvFmt = g_enTdeCommonDrvColorFmt[enInFmt];

    pNode->SRC2_VSP.bits.zme_in_fmt = TdeHalGetZmeInFmt( enInDrvFmt );

    pNode->SRC2_VSP.bits.zme_out_fmt = (pNode->SRC2_VSP.bits.zme_in_fmt == TDE_ZME_FMT_ARGB) ? TDE_ZME_FMT_ARGB : TDE_ZME_FMT_YUV444;

    pNode->SRC2_IMGSIZE.bits.src2_width = pInRect->u32Width - 1;
    pNode->SRC2_ZMEIRESO.bits.iw = pInRect->u32Width - 1;
    pNode->SRC2_ZMEORESO.bits.ow = pOutRect->u32Width - 1;
    pNode->SRC2_ZMEORESO.bits.oh = pOutRect->u32Height - 1;
    pNode->SRC2_IMGSIZE.bits.src2_height = pInRect->u32Height - 1;
    pNode->SRC2_ZMEIRESO.bits.ih = pInRect->u32Height - 1;

    pNode->DES_IMGSIZE.bits.des_width = pOutRect->u32Width - 1;
    pNode->DES_IMGSIZE.bits.des_height = pOutRect->u32Height - 1;


    pNode->SRC2_HSP.bits.hratio = (1 >= pOutRect->u32Width) ? 0 : ((pInRect->u32Width) << TDE_HAL_HSTEP_FLOATLEN) / (pOutRect->u32Width);

    pNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width = pInRect->u32Width;

    if ((pOutRect->u32Width * 16) <= pInRect->u32Width)
    {
        pNode->SRC2_HPZME.bits.src2_hpzme_en = 1;

        pNode->SRC2_HPZME.bits.src2_hpzme_mode = TdeHalGetHpzmeMode(pOutRect->u32Width , pInRect->u32Width);

        pNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width = pInRect->u32Width / (pNode->SRC2_HPZME.bits.src2_hpzme_mode + 1)\
                + ((pInRect->u32Width % (pNode->SRC2_HPZME.bits.src2_hpzme_mode + 1)) ? 1 : 0);
    }


    if ((pOutRect->u32Height * 16) <= pInRect->u32Height)
    {
        u32TempHeightScaleCoff = TdeHalGetHeightScaleCoff(pOutRect->u32Height , pInRect->u32Height);
        pNode->SRC2_ZMEIRESO.bits.ih = pInRect->u32Height / u32TempHeightScaleCoff - 1;
        pNode->SRC2_IMGSIZE.bits.src2_height = pInRect->u32Height / u32TempHeightScaleCoff - 1;
        pNode->SRC2_CH0_STRIDE.bits.src2_ch0_stride = pNode->SRC2_CH0_STRIDE.bits.src2_ch0_stride * u32TempHeightScaleCoff;

		pNode->SRC2_VSR.bits.vratio = ((pNode->SRC2_IMGSIZE.bits.src2_height) << TDE_FLOAT_BITLEN) / (pOutRect->u32Height);
	}
	else
	{
		pNode->SRC2_VSR.bits.vratio = (1 >= pOutRect->u32Height) ? 0 : ((pInRect->u32Height) << TDE_FLOAT_BITLEN) / (pOutRect->u32Height);
	}


    pNode->SRC2_HSP.bits.hchmsc_en = 1;
    pNode->SRC2_HSP.bits.hlmsc_en = 1;

    u32TempZoomMode = TdeHalGetZoomMode( enFilterMode );

    if (pNode->SRC2_HSP.bits.hratio != NO_HSCALE_STEP)
    {
        pNode->SRC2_HSP.bits.hfir_order = 0;

        pNode->SRC2_HSP.bits.hafir_en  = (u32TempZoomMode >> TDE_HAL_TWO_BIT_SHIFT)   & 0x1 ;
        pNode->SRC2_HSP.bits.hlfir_en  = (u32TempZoomMode >> TDE_HAL_ONE_BIT_SHIFT)   & 0x1 ;
        pNode->SRC2_HSP.bits.hchfir_en = (u32TempZoomMode >> TDE_HAL_ZERO_BIT_SHIFT)  & 0x1 ;

        pNode->TDE_SRC2_ZME_CHADDR = s_stParaTable.u32HfCoefAddr + TdeHalGetResizeParaHTable(pNode->SRC2_HSP.bits.hratio) * TDE_PARA_HTABLE_SIZE;
        pNode->TDE_SRC2_ZME_LHADDR = pNode->TDE_SRC2_ZME_CHADDR;

   }

    pNode->SRC2_VSP.bits.vchmsc_en = 1;
    pNode->SRC2_VSP.bits.vlmsc_en = 1;

    if ((pNode->SRC2_VSR.bits.vratio != NO_VSCALE_STEP) && (!bDefilicker))
    {
        pNode->SRC2_VSP.bits.vafir_en  = (u32TempZoomMode >> TDE_HAL_TWO_BIT_SHIFT)   & 0x1 ;
        pNode->SRC2_VSP.bits.vlfir_en  = (u32TempZoomMode >> TDE_HAL_ONE_BIT_SHIFT)   & 0x1 ;
        pNode->SRC2_VSP.bits.vchfir_en = (u32TempZoomMode >> TDE_HAL_ZERO_BIT_SHIFT)  & 0x1 ;

        pNode->TDE_SRC2_ZME_CVADDR = s_stParaTable.u32VfCoefAddr + TdeHalGetResizeParaVTable(pNode->SRC2_VSR.bits.vratio) * TDE_PARA_VTABLE_SIZE;
        pNode->TDE_SRC2_ZME_LVADDR = pNode->TDE_SRC2_ZME_CVADDR;
   }
#ifndef HI_BUILD_IN_BOOT
    if(bDefilicker)
    {
        pNode->SRC2_VSP.bits.vchmsc_en = 1;
        pNode->SRC2_VSP.bits.vlmsc_en = 1;
        pNode->SRC2_VSP.bits.vafir_en = 1;
        pNode->SRC2_VSP.bits.vlfir_en = 1;
        pNode->SRC2_VSP.bits.vchfir_en = 1;
        pNode->TDE_SRC2_ZME_CVADDR = s_stParaTable.u32DeflickerVfCoefAddr + TdeHalGetResizeParaVTable(pNode->SRC2_VSR.bits.vratio)* TDE_PARA_VTABLE_SIZE;
        pNode->TDE_SRC2_ZME_LVADDR = pNode->TDE_SRC2_ZME_CVADDR;
    }
#endif

    pNode->SRC2_ZMEIRESO.bits.iw = pNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width - 1;


    pNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width = pNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width - 1;

    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetColorConvert
* Description:  set parameter for color space change
* Input:         pHWNode:pointer of node
*                pConv: parameter of color space change
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetColorConvert(TDE_HWNode_S* pHWNode, TDE_DRV_CONV_MODE_CMD_S* pConv)
{
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pConv);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CSCCOVERT))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support CSCCovert\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

     if (pConv->bInConv)
    {
           pHWNode->SRC2_CSC_MUX.bits.src2_csc_mode = 1-(HI_U32)pConv->bInSrc1Conv;
    	    pHWNode->SRC1_CSC_IDC.bits.csc_en = 1;
	    if (pConv->bInRGB2YC)
        {
            pHWNode->SRC1_CSC_IDC.bits.cscidc0 = 0;
            pHWNode->SRC1_CSC_IDC.bits.cscidc1 = 0;
            pHWNode->SRC1_CSC_IDC.bits.cscidc2 = 0;

            pHWNode->SRC1_CSC_ODC.bits.cscodc0 = 128;
            pHWNode->SRC1_CSC_ODC.bits.cscodc1 = 128;
            pHWNode->SRC1_CSC_ODC.bits.cscodc2 = 16;

            pHWNode->SRC1_CSC_P0.bits.cscp00 =  66*4;
            pHWNode->SRC1_CSC_P0.bits.cscp01 =  129*4;

            pHWNode->SRC1_CSC_P1.bits.cscp02 =  25*4;
            pHWNode->SRC1_CSC_P1.bits.cscp10 =  -38*4;

            pHWNode->SRC1_CSC_P2.bits.cscp11 = -74*4;
            pHWNode->SRC1_CSC_P2.bits.cscp12 =  112*4;

            pHWNode->SRC1_CSC_P3.bits.cscp20 =  112*4;
            pHWNode->SRC1_CSC_P3.bits.cscp21 =  -94*4;

            pHWNode->SRC1_CSC_P4.bits.cscp22 = -18*4;
        }
	    else
        {

            pHWNode->SRC1_CSC_IDC.bits.cscidc0 = -128;
            pHWNode->SRC1_CSC_IDC.bits.cscidc1 = -128;
            pHWNode->SRC1_CSC_IDC.bits.cscidc2 = -16;

            pHWNode->SRC1_CSC_ODC.bits.cscodc0 = 0;
            pHWNode->SRC1_CSC_ODC.bits.cscodc1 = 0;
            pHWNode->SRC1_CSC_ODC.bits.cscodc2 = 0;

            pHWNode->SRC1_CSC_P0.bits.cscp00 =  297*4;
            pHWNode->SRC1_CSC_P0.bits.cscp01 =  0;

            pHWNode->SRC1_CSC_P1.bits.cscp02 =  408*4;
            pHWNode->SRC1_CSC_P1.bits.cscp10 =  297*4;

            pHWNode->SRC1_CSC_P2.bits.cscp11 = -100*4;
            pHWNode->SRC1_CSC_P2.bits.cscp12 =  -208*4;

            pHWNode->SRC1_CSC_P3.bits.cscp20 =  297*4;
            pHWNode->SRC1_CSC_P3.bits.cscp21 =  516*4;

            pHWNode->SRC1_CSC_P4.bits.cscp22 = 0;
	}
    }

    if (pConv->bOutConv)
    {

	 pHWNode->SRC2_CSC_MUX.bits.src2_csc_mode = 1-(HI_U32)pConv->bInSrc1Conv;
        pHWNode->DES_CSC_IDC.bits.csc_en = 1;
    	 if (pConv->bInRGB2YC)
        {
            pHWNode->DES_CSC_IDC.bits.cscidc0 = -128;
            pHWNode->DES_CSC_IDC.bits.cscidc1 = -128;
            pHWNode->DES_CSC_IDC.bits.cscidc2=  -16;
            pHWNode->DES_CSC_ODC.bits.cscodc0 =  0;
            pHWNode->DES_CSC_ODC.bits.cscodc1 =  0;
            pHWNode->DES_CSC_ODC.bits.cscodc2 =  0;

            pHWNode->DES_CSC_P0.bits.cscp00 = 297*4;
            pHWNode->DES_CSC_P0.bits.cscp01 = 0;

            pHWNode->DES_CSC_P1.bits.cscp02 = 408*4;
            pHWNode->DES_CSC_P1.bits.cscp10 =  297*4;

            pHWNode->DES_CSC_P2.bits.cscp11=  -100*4;
            pHWNode->DES_CSC_P2.bits.cscp12 =  -208*4;

            pHWNode->DES_CSC_P3.bits.cscp20=  297*4;
            pHWNode->DES_CSC_P3.bits.cscp21=  516*4;
            pHWNode->DES_CSC_P4.bits.cscp22= 0;
	 }
        else
        {
            pHWNode->DES_CSC_IDC.bits.cscidc0 = 0;
            pHWNode->DES_CSC_IDC.bits.cscidc1 = 0;
            pHWNode->DES_CSC_IDC.bits.cscidc2=  0;
            pHWNode->DES_CSC_ODC.bits.cscodc0 =  128;
            pHWNode->DES_CSC_ODC.bits.cscodc1 =  128;
            pHWNode->DES_CSC_ODC.bits.cscodc2 =  16;

            pHWNode->DES_CSC_P0.bits.cscp00 = 66*4;
            pHWNode->DES_CSC_P0.bits.cscp01 = 129*4;

            pHWNode->DES_CSC_P1.bits.cscp02 = 25*4;
            pHWNode->DES_CSC_P1.bits.cscp10 =  -38*4;

            pHWNode->DES_CSC_P2.bits.cscp11=  -74*4;

            pHWNode->DES_CSC_P2.bits.cscp12 =  112*4;

            pHWNode->DES_CSC_P3.bits.cscp20=  112*4;

            pHWNode->DES_CSC_P3.bits.cscp21=  -94*4;
            pHWNode->DES_CSC_P4.bits.cscp22= -18*4;
        }
    }
return HI_SUCCESS;
}


HI_S32 TdeHalNodeSetCsc(TDE_HWNode_S* pHWNode, TDE2_CSC_OPT_S stCscOpt)
{
#if 0
	TDE_COLOR_CONV_U unColorConv;
	TDE_INS_U unIns;

	TDE_ASSERT(HI_NULL != pHWNode);

	unColorConv.u32All = 0;
	unIns.u32All = 0;
	unColorConv.stBits.u32IcscCustomEn = stCscOpt.bICSCUserEnable;
	unColorConv.stBits.u32IcscReload = stCscOpt.bICSCParamReload;
	unColorConv.stBits.u32OcscCustomEn = stCscOpt.bOCSCUserEnable;
	unColorConv.stBits.u32OcscReload = stCscOpt.bOCSCParamReload;
	/*use correct parameter when user not use user parameter and enable ICSC function*/
    /*CNcomment:若用户未开启自定义系数且ICSC功能使能,
    则强制使用TDE的修正系数(TDE默认系数存在问题)*/
#endif
	return HI_SUCCESS;
}


/****************************************************************************/
/*                             TDE hal function inner                         */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeHalInitParaTable
* Description:  init config parameter
* Input:         none
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeHalInitParaTable(HI_VOID)
{
    #define OrgHfCoef Tde6x32Coef
    HI_U32 i = 0;
    HI_U32 *pHfCoef = HI_NULL;
    HI_U32 *pVfCoef = HI_NULL;
#ifndef HI_BUILD_IN_BOOT
    HI_U32 *pDeflickerVfCoef = HI_NULL;
#endif
    pHfCoef = (HI_U32 *)TDE_MALLOC(TDE_PARA_HTABLE_SIZE * TDE_PARA_HTABLE_NUM);
    if (HI_NULL == pHfCoef)
    {
        TDE_TRACE(TDE_KERN_INFO, "Alloc horizontal coef failed!\n");
        return HI_FAILURE;
    }
    pVfCoef = (HI_U32 *)TDE_MALLOC(TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
    if (HI_NULL == pVfCoef)
    {
        TDE_TRACE(TDE_KERN_INFO, "Alloc vertical coef failed\n");
        TDE_FREE(pHfCoef);
        return HI_FAILURE;
    }
#ifndef HI_BUILD_IN_BOOT
    pDeflickerVfCoef = (HI_U32 *)TDE_MALLOC(TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
    if (HI_NULL == pDeflickerVfCoef)
    {
        TDE_TRACE(TDE_KERN_INFO, "Alloc Deflicker vertical coef failed\n");
        TDE_FREE(pHfCoef);
        TDE_FREE(pVfCoef);
        return HI_FAILURE;
    }
#endif
    /*copy parameter according other offer way*//* CNcomment:按照算法组提供的结构拷贝参数表 */
    for (i = 0; i < TDE_PARA_HTABLE_NUM; i++)
    {
        HI_GFX_Memcpy(pHfCoef + i*(TDE_PARA_HTABLE_SIZE/4), OrgHfCoef + i*(TDE_PARA_HTABLE_ORG_SIZE), (TDE_PARA_HTABLE_ORG_SIZE)*4);
    }
    for (i = 0; i < TDE_PARA_VTABLE_NUM; i++)
    {
        HI_GFX_Memcpy(pVfCoef + i*(TDE_PARA_VTABLE_SIZE/4), OrgVfCoef + i*(TDE_PARA_VTABLE_ORG_SIZE), (TDE_PARA_VTABLE_ORG_SIZE)*4);
    }
#ifndef HI_BUILD_IN_BOOT
    for (i = 0; i < TDE_PARA_VTABLE_NUM; i++)
    {
        HI_GFX_Memcpy(pDeflickerVfCoef + i*(TDE_PARA_VTABLE_SIZE/4), DeflickerVfCoef + i*(TDE_PARA_VTABLE_ORG_SIZE), (TDE_PARA_VTABLE_ORG_SIZE)*4);
    }
 #endif

    HI_GFX_Memset(&s_stParaTable, 0, sizeof(s_stParaTable));
#ifndef HI_BUILD_IN_BOOT
    s_stParaTable.u32DeflickerVfCoefAddr = wgetphy((HI_VOID *)pDeflickerVfCoef);
#endif
    s_stParaTable.u32HfCoefAddr = wgetphy((HI_VOID *)pHfCoef);
    s_stParaTable.u32VfCoefAddr = wgetphy((HI_VOID *)pVfCoef);


    return HI_SUCCESS;
}


/*****************************************************************************
* Function:      TdeHalCurNode
* Description:   get the node physics address that is suspended
* Input:         none
* Output:        none
* Return:       the address of current running node
* Others:        none
*****************************************************************************/
HI_U32 TdeHalCurNode()
{
    HI_U32 u32Addr = 0;

    u32Addr = TDE_READ_REG(s_pu32BaseVirAddr, TDE_AQ_ADDR);

    return u32Addr;
}
STATIC HI_S32  TdeHalGetbppByFmt16(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_RGB444:
	    case TDE_DRV_COLOR_FMT_RGB555:
	    case TDE_DRV_COLOR_FMT_RGB565:
	    case TDE_DRV_COLOR_FMT_ARGB4444:
	    case TDE_DRV_COLOR_FMT_ARGB1555:
	    case TDE_DRV_COLOR_FMT_ACLUT88:
	    case TDE_DRV_COLOR_FMT_YCbCr422:
	    case TDE_DRV_COLOR_FMT_halfword:
	        return 16;

        default:
            return -1;
    }
}
STATIC HI_S32  TdeHalGetbppByFmt24(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_RGB888:
	    case TDE_DRV_COLOR_FMT_ARGB8565:
	    case TDE_DRV_COLOR_FMT_YCbCr888:

            return 24;

        default:
            return -1;
    }
}
STATIC HI_S32  TdeHalGetbppByFmt32(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_ARGB8888:
	    case TDE_DRV_COLOR_FMT_AYCbCr8888:
	    case TDE_DRV_COLOR_FMT_RABG8888:
            return 32;

        default:
            return -1;
    }
}
STATIC HI_S32  TdeHalGetbppByFmt1(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_CLUT1:
	    case TDE_DRV_COLOR_FMT_A1:
            return 1;

        default:
            return -1;
    }
}

STATIC HI_S32  TdeHalGetbppByFmt2(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_CLUT2:
            return 2;

        default:
            return -1;
    }
}
STATIC HI_S32  TdeHalGetbppByFmt4(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_CLUT4:
            return 4;

        default:
            return -1;
    }
}
STATIC HI_S32  TdeHalGetbppByFmt8(TDE_DRV_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
	    case TDE_DRV_COLOR_FMT_CLUT8:
	    case TDE_DRV_COLOR_FMT_ACLUT44:
	    case TDE_DRV_COLOR_FMT_A8:
	    case TDE_DRV_COLOR_FMT_byte:
	        return 8;

        default:
            return -1;
    }
}
/*****************************************************************************
* Function:      TdeHalGetbppByFmt
* Description:   get bpp according color of driver
* Input:         enFmt: color type
* Output:        pitch width
* Return:        -1: wrong format
* Others:        none
*****************************************************************************/

STATIC HI_S32 TdeHalGetbppByFmt(TDE_DRV_COLOR_FMT_E enFmt)
{
    HI_S32 s32Ret = 0;

    if (16 == (s32Ret = TdeHalGetbppByFmt16(enFmt)))
    {
        return 16;
    }
    else if (24 == (s32Ret = TdeHalGetbppByFmt24(enFmt)))
    {
        return 24;
    }
    else if (32 == (s32Ret = TdeHalGetbppByFmt32(enFmt)))
    {
        return 32;
    }
    else if (1 == (s32Ret = TdeHalGetbppByFmt1(enFmt)))
    {
        return 1;
    }
    else if (2 == (s32Ret = TdeHalGetbppByFmt2(enFmt)))
    {
        return 2;
    }
    else if (4 == (s32Ret = TdeHalGetbppByFmt4(enFmt)))
    {
        return 4;
    }
    else if (8 == (s32Ret = TdeHalGetbppByFmt8(enFmt)))
    {
        return 8;
    }
    else
    {
        return -1;
    }

}

/*****************************************************************************
* Function:      TdeHalGetResizeParaTable
* Description:   get index table according the step
* Input:         u32Step: input step
* Output:        none
* Return:        index table address
* Others:        none
*****************************************************************************/
STATIC INLINE HI_U32 TdeHalGetResizeParaHTable(HI_U32 u32Step)
{
    HI_U32 u32Index;

    if (TDE_RESIZE_8X32_AREA_0 > u32Step)
    {
        u32Index = 0;
    }
    else if (TDE_RESIZE_8X32_AREA_0 == u32Step)
    {
        u32Index = 1;
    }
    else if (TDE_RESIZE_8X32_AREA_0 < u32Step && TDE_RESIZE_8X32_AREA_1 >= u32Step)
    {
        u32Index = 2;
    }
    else if (TDE_RESIZE_8X32_AREA_1 < u32Step && TDE_RESIZE_8X32_AREA_2 >= u32Step)
    {
        u32Index = 3;
    }
    else if (TDE_RESIZE_8X32_AREA_2 < u32Step && TDE_RESIZE_8X32_AREA_3 >= u32Step)
    {
        u32Index = 4;
    }
    else if (TDE_RESIZE_8X32_AREA_3 < u32Step && TDE_RESIZE_8X32_AREA_4 >= u32Step)
    {
        u32Index = 5;
    }
    else /* TDE_RESIZE_8X32_AREA_4 <= u32Step */
    {
        u32Index = 6;
    }

    return u32Index;
}

/*****************************************************************************
* Function:      TdeHalGetResizeParaVTable
* Description:   get table of parameter for resize
* Input:         u32Step:input step
* Output:        none
* Return:        address of table
* Others:        none
*****************************************************************************/
STATIC INLINE HI_U32 TdeHalGetResizeParaVTable(HI_U32 u32Step)
{
    HI_U32 u32Index;

    /*get index table according step*//*CNcomment: 根据步长找到表的索引 */
    if (u32Step < TDE_RESIZE_PARA_AREA_0)
    {
        u32Index = 0;
    }
    else if (u32Step == TDE_RESIZE_PARA_AREA_0)
    {
        u32Index = 1;
    }
    else if (u32Step > TDE_RESIZE_PARA_AREA_0 && u32Step <= TDE_RESIZE_PARA_AREA_1)
    {
        u32Index = 2;
    }
    else if (u32Step > TDE_RESIZE_PARA_AREA_1 && u32Step <= TDE_RESIZE_PARA_AREA_2)
    {
        u32Index = 3;
    }
    else if (u32Step > TDE_RESIZE_PARA_AREA_2 && u32Step <= TDE_RESIZE_PARA_AREA_3)
    {
        u32Index = 4;
    }
    else if (u32Step > TDE_RESIZE_PARA_AREA_3 && u32Step <= TDE_RESIZE_PARA_AREA_4)
    {
        u32Index = 5;
    }
    else
    {
        u32Index = 6;
    }
    return u32Index;
}
/*****************************************************************************
* Function:      TdeHalInitQueue
* Description:   Initialize Aq list,config the operation which is needed
* Input:         none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_VOID TdeHalInitQueue(HI_VOID)
{

    /*write 0 to Aq list start address register*/
    /*CNcomment: 将Aq链表首地址寄存器写0 */
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_NADDR, 0);

}

#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeHalGetOffsetInNode
* Description:   get offset of operation according u32MaskUpdt
* Input:         none
* Output:        none
* Return:        -1:offse
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeHalGetOffsetInNode(HI_U64 u64MaskUpdt, HI_U64 u64Update)
{
    HI_U32 i = 0;
    HI_S32 s32Ret = 0;

    if (0 == (u64MaskUpdt & u64Update))
    {
        return -1;
    }

    for (i = 0; i < 64; i++)
    {
        if (1 == ((u64MaskUpdt >> i) & 0x1))
        {
            break;
        }
        if (1 == ((u64Update >> i) & 0x1))
        {
            s32Ret++;
        }
    }
    return s32Ret;
}



/*****************************************************************************
* Function:      TdeHalSetDeflicerLevel
* Description:   SetDeflicerLevel
* Input:         eDeflickerLevel:anti-flicker levels including:auto,low,middle,high
* Output:        none
* Return:        success
* Others:        none
*****************************************************************************/
HI_S32 TdeHalSetDeflicerLevel(TDE_DEFLICKER_LEVEL_E eDeflickerLevel)
{
    s_eDeflickerLevel = eDeflickerLevel;
    return HI_SUCCESS;
}

HI_S32 TdeHalGetDeflicerLevel(TDE_DEFLICKER_LEVEL_E *pDeflicerLevel)
{
    *pDeflicerLevel = s_eDeflickerLevel;
    return HI_SUCCESS;
}
#endif

HI_S32 TdeHalSetAlphaThreshold(HI_U8 u8ThresholdValue)
{
    s_u8AlphaThresholdValue = u8ThresholdValue;

    return HI_SUCCESS;

}

#ifndef HI_BUILD_IN_BOOT
HI_S32 TdeHalGetAlphaThreshold(HI_U8 *pu8ThresholdValue)
{
    *pu8ThresholdValue = s_u8AlphaThresholdValue;

    return HI_SUCCESS;
}
#endif

HI_S32 TdeHalSetAlphaThresholdState(HI_BOOL bEnAlphaThreshold)
{
    s_bEnAlphaThreshold = bEnAlphaThreshold;

    return HI_SUCCESS;
}

#ifndef HI_BUILD_IN_BOOT
HI_S32 TdeHalGetAlphaThresholdState(HI_BOOL *pbEnAlphaThreshold)
{
    *pbEnAlphaThreshold = s_bEnAlphaThreshold;

    return HI_SUCCESS;
}

#ifndef CONFIG_TDE_PROC_DISABLE
struct seq_file * TdeHalNodePrintInfo(struct seq_file *p,HI_U32 *pu32CurNode)
{
    HI_S32 i;
    /* see define of TDE_HWNode_S */
    HI_U8*  chUpdate[] =
    {
        "SRC1_CTRL          ",
        "SRC1_CH0_ADDR      ",
        "SRC1_CH1_ADDR      ",
        "SRC1_CH0_STRIDE    ",
        "SRC1_CH1_STRIDE    ",
        "SRC1_IMGSIZE       ",
        "SRC1_FILL          ",
        "SRC1_ALPHA         ",
        "SRC1_PIX_OFFSET    ",
        "SRC2_CTRL          ",
        "SRC2_CH0_ADDR      ",
        "SRC2_CH1_ADDR      ",
        "SRC2_CH0_STRIDE    ",
        "SRC2_CH1_STRIDE    ",
        "SRC2_IMGSIZE       ",
        "SRC2_FILL          ",
        "SRC2_ALPHA         ",
        "SRC2_PIX_OFFSET    ",
        "DES_CTRL           ",                           /* 0x1c0 */
        "DES_CH0_ADDR       ",                 /* 0x1c4 */
        "DES_CH1_ADDR       ",                 /* 0x1c8 */
        "DES_CH0_STRIDE     ",                 /* 0x1cc */
        "DES_CH1_STRIDE     ",                /* 0x1d0 */
        "DES_IMGSIZE        ",                 /* 0x1d4 */
        "DES_ALPHA          ",                /* 0x1d8 */
        "DES_CROP_POS_ST    ",                 /* 0x1dc */
        "DES_CROP_POS_ED    ",               /* 0x1e0 */
        "MMU_PROT_CTRL      ",                /* 0x1e4 */
        "DES_PIX_OFFSET     ",
        "SRC1_HSP           ",                           /* 0x200 */
        "SRC1_HLOFFSET      ",                 /* 0x204 */
        "SRC1_HCOFFSET      ",                     /* 0x208 */
        "SRC1_VSP           ",                          /* 0x20c */
        "SRC1_VSR           ",                          /* 0x210 */
        "SRC1_VOFFSET       ",                      /* 0x214 */
        "SRC1_ZMEORESO      ",                     /* 0x218 */
        "SRC1_ZMEIRESO      ",                     /* 0x21c */
        "SRC1_CROP_POS      ",                     /* 0x220 */
        "SRC1_CROP_SIZE     ",                    /* 0x224 */
        "SRC1_HPZME         ",                        /* 0x228 */
        "SRC1_HPZME_SIZE    ",                   /* 0x22c */
        "SRC1_CSC_IDC       ",                      /* 0x230 */
        "SRC1_CSC_ODC       ",                      /* 0x234 */
        "SRC1_CSC_P0        ",                      /* 0x238 */
        "SRC1_CSC_P1        ",                      /* 0x23c */
        "SRC1_CSC_P2        ",                       /* 0x240 */
        "SRC1_CSC_P3        ",                       /* 0x244 */
        "SRC1_CSC_P4        ",
        "SRC2_HSP           ",                /* 0x280 */
        "SRC2_HLOFFSET      ",                 /* 0x284 */
        "SRC2_HCOFFSET      ",                 /* 0x288 */
        "SRC2_VSP           ",                 /* 0x28c */
        "SRC2_VSR           ",                /* 0x290 */
        "SRC2_VOFFSET       ",                 /* 0x294 */
        "SRC2_ZMEORESO      ",                 /* 0x298 */
        "SRC2_ZMEIRESO      ",                 /* 0x29c */
        "SRC2_CROP_POS      ",                 /* 0x2a0 */
        "SRC2_CROP_SIZE     ",                 /* 0x2a4 */
        "SRC2_HPZME         ",                 /* 0x2a8 */
        "SRC2_HPZME_SIZE    ",                 /* 0x2ac */
        "SRC2_CSC_MUX       ",
        "DES_CSC_IDC        ",                 /* 0x2c0 */
        "DES_CSC_ODC        ",                       /* 0x2c4 */
        "DES_CSC_P0         ",                 /* 0x2c8 */
        "DES_CSC_P1         ",                 /* 0x2cc */
        "DES_CSC_P2         ",                 /* 0x2d0 */
        "DES_CSC_P3         ",                 /* 0x2d4 */
        "DES_CSC_P4         ",                 /* 0x2d8 */
        "DES_DSWM           ",
        "CBMCTRL            ",                 /* 0x300 */
        "CBMBKG             ",                 /* 0x304 */
        "CBMCOLORIZE        ",                 /* 0x308 */
        "CBMALUPARA         ",                 /* 0x30c */
        "CBMKEYPARA         ",                 /* 0x310 */
        "CBMKEYMIN          ",                 /* 0x314 */
        "CBMKEYMAX          ",                 /* 0x318 */
        "CBMKEYMASK         ",                 /* 0x31c */
        "SRC1_CBMPARA       ",                 /* 0x320 */
        "SRC1_CBMSTPOS      ",                 /* 0x324 */
        "SRC2_CBMPARA       ",                 /* 0x328 */
        "SRC2_CBMSTPOS      ",
        "TDE_SRC1_ZME_LHADDR",               /* 0x480 */
        "TDE_SRC1_ZME_LVADDR",               /* 0x484 */
        "TDE_SRC1_ZME_CHADDR",               /* 0x488 */
        "TDE_SRC1_ZME_CVADDR",               /* 0x48c */
        "TDE_SRC2_ZME_LHADDR",               /* 0x490 */
        "TDE_SRC2_ZME_LVADDR",               /* 0x494 */
        "TDE_SRC2_ZME_CHADDR",               /* 0x498 */
        "TDE_SRC2_ZME_CVADDR",               /* 0x49c */
        "TDE_CLUT_ADDR      "                       /* 0x4a0 */
    };

 /* print node information */
    PROC_PRINT(p,"\n--------- Hisilicon TDE Node params Info ---------\n");
    for (i = 0; i < 9; i++)/*SRC1_CTRL 0x0*/
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i], *(pu32CurNode + i));
    }

    for (i = 0; i < 9; i++)/*SRC2_CTRL 0x040*/
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+9], *(pu32CurNode + i + 4*4));
    }

    for (i = 0; i < 11; i++)/*DES_CTRL 0x1c0 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+18], *(pu32CurNode + i + 28*4));
    }

    for (i = 0; i < 19; i++)/*SRC1_HSP 0x200 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+29], *(pu32CurNode + i + 32*4));
    }

    for (i = 0; i < 13; i++)/* SRC2_HSP 0x280 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+48], *(pu32CurNode + i + 40*4));
    }

    for (i = 0; i < 8; i++)/* DES_CSC_IDC 0x2c0 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+61], *(pu32CurNode + i + 44*4));
    }

    for (i = 0; i < 12; i++)/*CBMCTRL 0x300 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+69], *(pu32CurNode + i + 48*4));
    }

    for (i = 0; i < 9; i++)/*TDE_SRC1_ZME_LHADDR 0x480 */
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i+81], *(pu32CurNode + i + 72*4));
    }

    return p;
}
#endif

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

