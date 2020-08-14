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

#define NO_SCALE_STEP 0x1000

/* Judge if Update flag is 1 or not on current configure */
#define TDE_MASK_UPDATE(member) (1 << ((TDE_OFFSET_OF(TDE_HWNode_S, member) >> 2) % (UPDATE_SIZE)))


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

#define TDE_QUICKCOPY_CHECK(u32Capability)\
    do{\
        if(!(u32Capability&QUICKCOPY))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It deos not support QuickCopy\n");\
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        }\
    }while(0)

#define TDE_QUICKFill_CHECK(u32Capability)\
    do{\
        if(!(u32Capability&QUICKFILL))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It deos not support QuickFill\n");\
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        }\
    }while(0)

#define TDE_BITMASKROP_CHECK(enAlu, u32Capability)\
    do{\
        if (((TDE_ALU_MASK_ROP1==enAlu)||(TDE_ALU_MASK_ROP2==enAlu))&&(!(u32Capability&MASKROP)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It deos not support MaskRop\n");\
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        }\
    }while(0)

#define TDE_BITMASKBLEND_CHECK(enAlu, u32Capability)\
    do{\
        if((TDE_ALU_MASK_BLEND==enAlu)&&(!(u32Capability&MASKBLEND)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It deos not support MaskBlend\n");\
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        }\
    }while(0)

#define TDE_PATTERNFILL_CHECK(u32Capability)\
    do{\
        if(!(u32Capability&PATTERFILL))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It deos not support PatternFill\n");\
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        }\
    }while(0)

/* TDE register's Address range */
#define TDE_REG_SIZE 0x1000

/* Step range's type by algorithm team */
#define TDE_RESIZE_PARA_AREA_0  3724     /*1.1*/
#define TDE_RESIZE_PARA_AREA_1  4096     /*1.0*/
#define TDE_RESIZE_PARA_AREA_2  4501     /*0.91*/
#define TDE_RESIZE_PARA_AREA_3  4935     /*0.83*/
#define TDE_RESIZE_PARA_AREA_4  5326     /*0.769*/
#define TDE_RESIZE_PARA_AREA_5  5729     /*0.715*/
#define TDE_RESIZE_PARA_AREA_6  6141     /*0.667*/
#define TDE_RESIZE_PARA_AREA_7  8192     /*0.5*/
#define TDE_RESIZE_PARA_AREA_8  12300    /*0.333*/
#define TDE_RESIZE_PARA_AREA_9  16384    /*0.25*/
#define TDE_RESIZE_PARA_AREA_10 20480    /*0.2*/
/* coefficient table rangle of 8*32 area */
#define TDE_RESIZE_8X32_AREA_0  4096     /*1.0*/
#define TDE_RESIZE_8X32_AREA_1  5936     /*0.69*/
#define TDE_RESIZE_8X32_AREA_2  8192     /*0.5*/
#define TDE_RESIZE_8X32_AREA_3  12412    /*0.33*/
#define TDE_RESIZE_8X32_AREA_4  204800   /*0.02*/

/*Colorkey mode of CMOS chip */
#define TDE_COLORKEY_IGNORE     2
#define TDE_COLORKEY_AREA_OUT   1
#define TDE_COLORKEY_AREA_IN    0

/* Aq control mode */
#define TDE_AQ_CTRL_COMP_LIST   0x0 /* start next AQ list, after complete current list's operations */
#define TDE_AQ_CTRL_COMP_LINE   0x4 /* start next AQ list, after complete current node and line */

#define TDE_MAX_READ_STATUS_TIME 10

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
STATIC TDE_DEFLICKER_LEVEL_E s_eDeflickerLevel = TDE_DEFLICKER_AUTO;

/* alpha threshold switch */
STATIC HI_BOOL s_bEnAlphaThreshold = HI_FALSE;

/*alpha threshold value */
STATIC HI_U8 s_u8AlphaThresholdValue = 0xff;

#define TDE_CSCTABLE_SIZE 7
#define TDE_CSC_SIZE (TDE_CSCTABLE_SIZE * 4)


STATIC HI_U32 s_u32Rgb2YuvCsc[TDE_CSCTABLE_SIZE] =
    {0x0096004d, 0xffd5001d, 0x0080ffab, 0xff950080, 0x0000ffeb, 0x00000000, 0x02010000};
STATIC HI_U32 s_u32Yuv2RgbCsc[TDE_CSCTABLE_SIZE] =
    {0x00000100, 0x01000167, 0xff49ffa8, 0x01c60100, 0x00000000, 0x06030000, 0x00000000};

STATIC HI_U32 s_u32Rgb2YuvCoefAddr = 0;
STATIC HI_U32 s_u32Yuv2RgbCoefAddr = 0;


/****************************************************************************/
/*                             TDE hal inner function definition            */
/****************************************************************************/
STATIC HI_S32 TdeHalInitParaTable(HI_VOID);

#ifndef HI_BUILD_IN_BOOT
STATIC INLINE HI_S32 TdeHalGetOffsetInNode(HI_U64 u64MaskUpdt, HI_U64 u64Update);
#endif

STATIC HI_S32  TdeHalGetbppByFmt(TDE_DRV_COLOR_FMT_E enFmt);
STATIC INLINE HI_U32 TdeHalGetResizeParaHTable(HI_U32 u32Step);
STATIC INLINE HI_U32 TdeHalGetResizeParaVTable(HI_U32 u32Step);
STATIC INLINE HI_VOID TdeHalSetXyByAdjInfo(TDE_HWNode_S* pHWNode, TDE_CHILD_INFO* pChildInfo);
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

    if (0 != s_u32Rgb2YuvCoefAddr)
    {
        pBuf = (HI_VOID *)wgetvrt(s_u32Rgb2YuvCoefAddr);
        if (HI_NULL != pBuf)
        {
            TDE_FREE(pBuf);
            s_u32Rgb2YuvCoefAddr = 0;
        }
    }

    if (0 != s_u32Yuv2RgbCoefAddr)
    {
        pBuf = (HI_VOID *)wgetvrt(s_u32Yuv2RgbCoefAddr);
        if (HI_NULL != pBuf)
        {
            TDE_FREE(pBuf);
            s_u32Yuv2RgbCoefAddr = 0;
        }
    }
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
    /* clear all status */
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_INT, (Value)/*0x800f001f*/);

#ifndef HI_BUILD_IN_BOOT
    (HI_VOID)HI_GFX_SmmuIsr("HI_MOD_TDE");
#endif

    return Value;
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
    /*reset *//*CNcomment: 复位 */
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_RST, 0xffffffff);

    /* clear the state of interrupt*//*CNcomment: 请中断状态 */
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_INT, 0x800f001f);

    /* write TDE_REQ_TH*//*CNcomment: 写申请门限控制寄存器*/
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_REQ_TH, 0x01fef7cf);

    /* write TDE_BUS_LIMITER*//*CNcomment: 写总线流量控制寄存器*/
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_BUS_LIMITER, 0x80770000);
    //TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_BUS_LIMITER, 0x80110000);

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

    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;

    if (bEnable)
    {
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

    #ifndef HI_BUILD_IN_BOOT
        HI_GFX_GetTimeStamp(&u32StartTimeMs,NULL);
        while(u32TotalTime <= 1)
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
        /*cancel reset*/
        *s_pu32TdeClockVir &= ~0x1;

        /*enable clock*/
        *s_pu32TdeClockVir |= 0x2;
    }
    else
    {
        /*disable clock*/
        *s_pu32TdeClockVir &= ~0x2;

        /* reset*/
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
    TDE_INS_U unIns;
    HI_VOID * pBuf= NULL;
    pBuf = (HI_VOID *)TDE_MALLOC(sizeof(TDE_HWNode_S) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
    if (HI_NULL == pBuf)
    {
        TDE_TRACE(TDE_KERN_INFO, "malloc (%d) failed, wgetfreenum(%d)!\n", (sizeof(TDE_HWNode_S) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE), wgetfreenum());
        return HI_ERR_TDE_NO_MEM;
    }
    *pstHWNode = (TDE_HWNode_S*)(pBuf +TDE_NODE_HEAD_BYTE);
    /*open interrupt*//*CNcomment:开启链表完成中断,
    同步链表可更新中断, 关闭节点完成中断 */
    unIns.u32All = (*pstHWNode)->u32TDE_INS;
    unIns.stBits.u32AqIrqMask = TDE_AQ_COMP_LIST_MASK_EN;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
	unIns.stBits.u32CasProt = 0x1;
#endif
    (*pstHWNode)->u32TDE_INS = unIns.u32All;
    return HI_SUCCESS;
}

#ifdef CONFIG_GFX_MMU_SUPPORT
HI_VOID TdeHalFreeTmpBuf(TDE_HWNode_S* pstHWNode)
{
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
    unIns.stBits.u32AqIrqMask = TDE_AQ_COMP_LIST_MASK_EN;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
	unIns.stBits.u32CasProt = 0x1;
#endif
    pHWNode->u32TDE_INS = unIns.u32All;
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
    TDE_AQ_CTRL_U unAqCtrl;
    /*tde is idle*//* CNcomment:TDE空闲*/
    if(TdeHalCtlIsIdleSafely())
    {
    	TdeHalSetClock(HI_TRUE);
    	TdeHalCtlReset();
    	TdeHalInitQueue();

        /*write the first node address*//* CNcomment:写入首节点地址*/
        TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_NADDR, u32NodePhyAddr);
        /*write the first node update area*//* CNcomment:写入首节点更新标识*/
        TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_UPDATE, (HI_U32)(u64Update & 0xffffffff));
        TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_UPDATE2, (HI_U32)((u64Update >> 32) & 0xffffffff));
        unAqCtrl.u32All = TDE_READ_REG(s_pu32BaseVirAddr, TDE_AQ_CTRL);
        /*modiyf the break off to finish current node when use the temp buffer */
        /* CNcomment:若需要使用临时buffer,
        则打断模式为当前节点完成打断,否则为当前节点
           当前行完成打断*/
        if (HI_TRUE ==  bAqUseBuff)
        {
            unAqCtrl.stBits.u32AqOperMode = TDE_AQ_CTRL_COMP_LIST;
            TDE_TRACE(TDE_KERN_DEBUG, "Aq Ctrl use comp list mode\n");
        }
        else
        {
            unAqCtrl.stBits.u32AqOperMode = TDE_AQ_CTRL_COMP_LINE;
            TDE_TRACE(TDE_KERN_DEBUG, "Aq Ctrl use comp node line mode\n");
        }

        TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_CTRL, unAqCtrl.u32All);

        #ifndef HI_BUILD_IN_BOOT
        mb();
        #endif
        /*start Aq list*//* CNcomment:启动Aq*/
        TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_CTRL, 0x1);
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
    HI_U32 *pu32Ins = (HI_U32*)pBuf;
    TDE_INS_U unIns;
    HI_U32 u32Mask;

    TDE_ASSERT(HI_NULL != pBuf);
    unIns.u32All = *pu32Ins;
    u32Mask = unIns.stBits.u32AqIrqMask;
    unIns.stBits.u32AqIrqMask = TDE_AQ_COMP_NODE_MASK_EN | u32Mask;

    *pu32Ins = unIns.u32All;

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
    TDE_SRC_TYPE_U unSrcType;
    TDE_SUR_XY_U unXy;
    TDE_Y_PITCH_U unYPitch;
    TDE_ARGB_ORDER_U unArgbOrder;
    TDE_INS_U unIns;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvSurface);

    /*set the source bitmap attribute info*//*CNcomment:配置源位图属性信息*/
    unSrcType.u32All = pHWNode->u32TDE_S1_TYPE;
    unSrcType.stBits.u32SrcColorFmt = (HI_U32)pDrvSurface->enColorFmt;
    unSrcType.stBits.u32AlphaRange = (HI_U32)pDrvSurface->bAlphaMax255;
    unSrcType.stBits.u32HScanOrd = (HI_U32)pDrvSurface->enHScan;
    unSrcType.stBits.u32VScanOrd = (HI_U32)pDrvSurface->enVScan;
    /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    unSrcType.stBits.u32RgbExp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
	unSrcType.stBits.u32MmuBypass = pDrvSurface->bCma;
#else
	unSrcType.stBits.u32MmuBypass = 0x1;
#endif
#endif

    unXy.u32All = pHWNode->u32TDE_S1_XY;
    unXy.stBits.u32X = pDrvSurface->u32Xpos;
    unXy.stBits.u32Y = pDrvSurface->u32Ypos;

    /*MB format*//*CNcomment:如果是宏块格式*/
    if (pDrvSurface->enColorFmt >= TDE_DRV_COLOR_FMT_YCbCr400MBP)
    {
        unSrcType.stBits.u32Pitch = pDrvSurface->u32CbCrPitch;
        unYPitch.stBits.u32Pitch = pDrvSurface->u32Pitch;

		unIns.u32All = pHWNode->u32TDE_INS;
		unIns.stBits.u32Y1En = HI_TRUE;

		if (TDE_DRV_COLOR_FMT_YCbCr400MBP == pDrvSurface->enColorFmt)
			pHWNode->u32TDE_S1_ADDR = pDrvSurface->u32PhyAddr;
		else
        	pHWNode->u32TDE_S1_ADDR = pDrvSurface->u32CbCrPhyAddr;
        pHWNode->u32TDE_Y1_ADDR = pDrvSurface->u32PhyAddr;
        pHWNode->u32TDE_Y1_PITCH = unYPitch.u32All;
		pHWNode->u32TDE_INS = unIns.u32All;

    }
    else
    {
        unSrcType.stBits.u32Pitch = pDrvSurface->u32Pitch;
        pHWNode->u32TDE_S1_ADDR = pDrvSurface->u32PhyAddr;

        if (pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_ARGB8888)
        {
            unArgbOrder.u32All = pHWNode->u32TDE_ARGB_ORDER;
            unArgbOrder.stBits.u32Src1ArgbOrder = pDrvSurface->enRgbOrder;
            pHWNode->u32TDE_ARGB_ORDER = unArgbOrder.u32All;
        }
    }

    /*target bitmapis same with source bitmap 1,so not need set*/
    /*CNcomment:源1位图宽高总是与Target位图一致,因此不设置源1的大小*/

    /*config the node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_S1_TYPE = unSrcType.u32All;
    pHWNode->u32TDE_S1_XY = unXy.u32All;


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
    TDE_SRC_TYPE_U unSrcType;
    TDE_SUR_XY_U unXy;
    TDE_Y_PITCH_U unYPitch;
    TDE_ARGB_ORDER_U unArgbOrder;
    TDE_INS_U unIns;
    TDE_SUR_SIZE_U unSurSize;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvSurface);

    /*set attribute info for source bitmap*//*CNcomment:配置源位图属性信息*/
    unSrcType.u32All = 0;
    unSrcType.stBits.u32SrcColorFmt = (HI_U32)pDrvSurface->enColorFmt;
    unSrcType.stBits.u32AlphaRange = (HI_U32)pDrvSurface->bAlphaMax255;
    unSrcType.stBits.u32HScanOrd = (HI_U32)pDrvSurface->enHScan;
    unSrcType.stBits.u32VScanOrd = (HI_U32)pDrvSurface->enVScan;
    unSrcType.stBits.u32RgbExp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
	unSrcType.stBits.u32MmuBypass = pDrvSurface->bCma;
#else
	unSrcType.stBits.u32MmuBypass = 1;
#endif
#endif

    unXy.u32All = pHWNode->u32TDE_S2_XY;
    unXy.stBits.u32X = pDrvSurface->u32Xpos;
    unXy.stBits.u32Y = pDrvSurface->u32Ypos;

    if (pDrvSurface->enColorFmt >= TDE_DRV_COLOR_FMT_YCbCr400MBP)
    {
        unSrcType.stBits.u32Pitch = pDrvSurface->u32CbCrPitch;
        unYPitch.stBits.u32Pitch = pDrvSurface->u32Pitch;

		unIns.u32All = pHWNode->u32TDE_INS;
		unIns.stBits.u32Y2En = HI_TRUE;

		if (TDE_DRV_COLOR_FMT_YCbCr400MBP == pDrvSurface->enColorFmt)
			pHWNode->u32TDE_S2_ADDR = pDrvSurface->u32PhyAddr;
		else
        	pHWNode->u32TDE_S2_ADDR = pDrvSurface->u32CbCrPhyAddr;
        pHWNode->u32TDE_Y2_ADDR = pDrvSurface->u32PhyAddr;
        pHWNode->u32TDE_Y2_PITCH = unYPitch.u32All;
	 	pHWNode->u32TDE_INS = unIns.u32All;
    }
    else
    {
        unSrcType.stBits.u32Pitch = pDrvSurface->u32Pitch;
        pHWNode->u32TDE_S2_ADDR = pDrvSurface->u32PhyAddr;

        if (pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_ARGB8888)
        {
            unArgbOrder.u32All = pHWNode->u32TDE_ARGB_ORDER;
            unArgbOrder.stBits.u32Src2ArgbOrder = pDrvSurface->enRgbOrder;
            pHWNode->u32TDE_ARGB_ORDER = unArgbOrder.u32All;
        }
    }

    /*set node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_S2_TYPE = unSrcType.u32All;
    pHWNode->u32TDE_S2_XY = unXy.u32All;

    unSurSize.u32All = 0;
    /*set size info of bitmap*//*CNcomment:配置位图大小信息*/
    unSurSize.stBits.u32Width = (HI_U32)pDrvSurface->u32Width;
    unSurSize.stBits.u32Height = (HI_U32)pDrvSurface->u32Height;
    pHWNode->u32TDE_S2_SIZE = unSurSize.u32All;

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
    TDE_SRC_TYPE_U unSrcType;
    TDE_SUR_XY_U unXy;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvMbY);

    /*set attribute for source bitmap *//*CNcomment:配置源位图属性信息*/
    unSrcType.u32All = pHWNode->u32TDE_S1_TYPE;
    unSrcType.stBits.u32Pitch = (HI_U32)pDrvMbY->u32Pitch;
    unSrcType.stBits.u32SrcColorFmt = (HI_U32)pDrvMbY->enColorFmt;
    unSrcType.stBits.u32AlphaRange = (HI_U32)pDrvMbY->bAlphaMax255;
    unSrcType.stBits.u32HScanOrd = (HI_U32)pDrvMbY->enHScan;
    unSrcType.stBits.u32VScanOrd = (HI_U32)pDrvMbY->enVScan;
    /*file zero of low area and top area use low area extend*//*CNcomment:一直使用低位填充为0,高位使用低位的扩展方式*/
    unSrcType.stBits.u32RgbExp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
	unSrcType.stBits.u32MmuBypass = 0;
#else
	unSrcType.stBits.u32MmuBypass = 1;
#endif
#endif
    unXy.u32All = pHWNode->u32TDE_S1_XY;
    unXy.stBits.u32X = pDrvMbY->u32Xpos;
    unXy.stBits.u32Y = pDrvMbY->u32Ypos;

    /*target bitmapis same with source bitmap 1,so not need set*/
    /*CNcomment:源1位图宽高总是与Target位图一致,因此不设置源1的大小*/
     /*config the node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_S1_ADDR = pDrvMbY->u32PhyAddr;
    pHWNode->u32TDE_S1_TYPE = unSrcType.u32All;
    pHWNode->u32TDE_S1_XY = unXy.u32All;

    if (TDE_MB_Y_FILTER == enMbOpt || TDE_MB_CONCA_FILTER == enMbOpt)
    {
        TDE_SUR_SIZE_U unSurSize;
        unSurSize.u32All = pHWNode->u32TDE_S2_SIZE;
        /*CNcomment:当为如上类型时,需要配置源2的大小,作为位图大小信息*/
        unSurSize.stBits.u32Width = (HI_U32)pDrvMbY->u32Width;
        unSurSize.stBits.u32Height = (HI_U32)pDrvMbY->u32Height;
        pHWNode->u32TDE_S2_SIZE = unSurSize.u32All;
    }

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
    TDE_SRC_TYPE_U unSrcType;
    TDE_SUR_XY_U unXy;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pDrvMbCbCr);

    /*set attribute info for source bitmap *//*CNcomment:配置源位图属性信息*/
    unSrcType.u32All = 0;
    unSrcType.stBits.u32Pitch = pDrvMbCbCr->u32Pitch;
    unSrcType.stBits.u32SrcColorFmt = (HI_U32)pDrvMbCbCr->enColorFmt;
    unSrcType.stBits.u32AlphaRange = (HI_U32)pDrvMbCbCr->bAlphaMax255;
    unSrcType.stBits.u32HScanOrd = (HI_U32)pDrvMbCbCr->enHScan;
    unSrcType.stBits.u32VScanOrd = (HI_U32)pDrvMbCbCr->enVScan;
    unSrcType.stBits.u32RgbExp = 0;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
	unSrcType.stBits.u32MmuBypass = 0;
#else
	unSrcType.stBits.u32MmuBypass = 1;
#endif
#endif
    unXy.u32All = pHWNode->u32TDE_S2_XY;
    unXy.stBits.u32X = pDrvMbCbCr->u32Xpos;
    unXy.stBits.u32Y = pDrvMbCbCr->u32Ypos;

    /*set node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_S2_ADDR = pDrvMbCbCr->u32PhyAddr;
    pHWNode->u32TDE_S2_TYPE = unSrcType.u32All;
    pHWNode->u32TDE_S2_XY = unXy.u32All;

    #ifndef HI_BUILD_IN_BOOT
    if (TDE_MB_CbCr_FILTER== enMbOpt || TDE_MB_UPSAMP_CONCA== enMbOpt)
    {
        TDE_SUR_SIZE_U unSurSize;
        unSurSize.u32All = 0;
        /*set size info of bitmap*//*CNcomment:配置位图大小信息*/
        unSurSize.stBits.u32Width = (HI_U32)pDrvMbCbCr->u32Width;
        unSurSize.stBits.u32Height = (HI_U32)pDrvMbCbCr->u32Height;
        pHWNode->u32TDE_S2_SIZE = unSurSize.u32All;
    }
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
    TDE_TAR_TYPE_U unTarType;
    TDE_SUR_SIZE_U unSurSize;
    TDE_SUR_XY_U unXy;
    TDE_ARGB_ORDER_U unArgbOrder;

    TDE_ASSERT(HI_NULL != pHWNode);

    /*set bitmap attribute info*//*CNcomment:配置源位图属性信息*/
    unTarType.u32All = pHWNode->u32TDE_TAR_TYPE;
    unTarType.stBits.u32Pitch = (HI_U32)pDrvSurface->u32Pitch;
    unTarType.stBits.u32TarColorFmt = (HI_U32)pDrvSurface->enColorFmt;
    unTarType.stBits.u32AlphaRange = (HI_U32)pDrvSurface->bAlphaMax255;
    unTarType.stBits.u32HScanOrd = (HI_U32)pDrvSurface->enHScan;
    unTarType.stBits.u32VScanOrd = (HI_U32)pDrvSurface->enVScan;
    unTarType.stBits.u32AlphaFrom = (HI_U32)enAlphaFrom;
    unTarType.stBits.u32RgbRound = 0; /*CNcomment:一直使用使用四舍无入的截位方式*/
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
	unTarType.stBits.u32MmuBypass = pDrvSurface->bCma;
#else
	unTarType.stBits.u32MmuBypass = 1;
#endif
#endif

    if (unTarType.stBits.u32TarColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888)
    {
        unTarType.stBits.u32TarColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;
    }

    /*set bitmap size info*//*CNcomment:配置位图大小信息*/
    unSurSize.u32All = pHWNode->u32TDE_TS_SIZE;
    unSurSize.stBits.u32Width = (HI_U32)pDrvSurface->u32Width;
    unSurSize.stBits.u32Height = (HI_U32)pDrvSurface->u32Height;

    /*set alpha info*//*CNcomment:设置alpha判决阈值 */
    unSurSize.stBits.u32AlphaThresholdHigh = (s_u8AlphaThresholdValue >> 4); //change the range 0-255
    unSurSize.stBits.u32AlphaThresholdLow = (s_u8AlphaThresholdValue & 0x0f);
    unXy.u32All = pHWNode->u32TDE_TAR_XY;
    unXy.stBits.u32X = pDrvSurface->u32Xpos;
    unXy.stBits.u32Y = pDrvSurface->u32Ypos;

    if ((pDrvSurface->enColorFmt <= TDE_DRV_COLOR_FMT_ARGB8888) || (pDrvSurface->enColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888))
    {
        unArgbOrder.u32All = pHWNode->u32TDE_ARGB_ORDER;
        unArgbOrder.stBits.u32TarArgbOrder = pDrvSurface->enRgbOrder;

        if (pDrvSurface->enColorFmt == TDE_DRV_COLOR_FMT_AYCbCr8888)
        {
            unTarType.stBits.u32TarColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;
    	    unArgbOrder.stBits.u32TarArgbOrder = 0x17;
        }

        pHWNode->u32TDE_ARGB_ORDER = unArgbOrder.u32All;
    }
    if(TDE_DRV_COLOR_FMT_RABG8888 == pDrvSurface->enColorFmt)
    {
        unArgbOrder.u32All = pHWNode->u32TDE_ARGB_ORDER;
        unArgbOrder.stBits.u32TarArgbOrder  = TDE_DRV_ORDER_RABG; //RABG
        pHWNode->u32TDE_ARGB_ORDER = unArgbOrder.u32All;
        unTarType.stBits.u32TarColorFmt     = TDE_DRV_COLOR_FMT_ARGB8888;
    }

    /*set node info*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_TAR_ADDR = pDrvSurface->u32PhyAddr;
    pHWNode->u32TDE_TAR_TYPE = unTarType.u32All;
    pHWNode->u32TDE_TAR_XY = unXy.u32All;
    pHWNode->u32TDE_TS_SIZE = unSurSize.u32All;

    return;
}
static HI_U32 TdeHalNodeGetAluMod(TDE_DRV_BASEOPT_MODE_E enMode, TDE_DRV_ALU_MODE_E enAlu)
{
    if (TDE_MB_2OPT == enMode)
    {
#ifndef HI_BUILD_IN_BOOT

        if (TDE_ALU_CONCA == enAlu)
        {
            return TDE_ALU_CONCA;
        }
        else if (TDE_ALU_NONE == enAlu)
#else
        if (TDE_ALU_NONE == enAlu)
#endif
        {
            return TDE_SRC2_BYPASS;
        }
        else
        {
            return (HI_U32)enAlu;
        }
    }

    if (TDE_ALU_NONE == enAlu)
    {
        return TDE_SRC2_BYPASS;
    }

    return (HI_U32)enAlu;

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
                                 TDE_DRV_ALU_MODE_E enAlu, TDE_DRV_COLORFILL_S *pstColorFill)
{
    TDE_ALU_U unAluMode;
    TDE_INS_U unIns;
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);

    unAluMode.u32All = pHWNode->u32TDE_ALU;
    unIns.u32All = pHWNode->u32TDE_INS;
    unAluMode.stBits.u32AlphaThreshodEn = (s_bEnAlphaThreshold)?1:0;
    switch (enMode)
    {
	    case TDE_QUIKE_FILL:/*quick file*//*CNcomment:快速填充*/
        {
            TDE_ASSERT(HI_NULL != pstColorFill);
            TDE_QUICKFill_CHECK(u32Capability);
            TDE_FILL_DATA_BY_FMT(pHWNode->u32TDE_S1_FILL,
                pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);
            unAluMode.stBits.u32AluMod = TDE_SRC1_BYPASS;
            unIns.stBits.u32Src1Mod = TDE_SRC_QUICK_FILL;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_DISA;
        }
        break;

       #ifndef HI_BUILD_IN_BOOT
	    case TDE_QUIKE_COPY:/*quick copy*//*CNcomment:快速拷贝*/
        {
            TDE_QUICKCOPY_CHECK(u32Capability);
            unAluMode.stBits.u32AluMod = TDE_SRC1_BYPASS;
            unIns.stBits.u32Src1Mod = TDE_SRC_QUICK_COPY;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_DISA;
        }
        break;
	    case TDE_NORM_FILL_1OPT:/*signal fill*//*CNcomment:普通单源填充*/
        {
            TDE_ASSERT(HI_NULL != pstColorFill);
            TDE_FILL_DATA_BY_FMT(pHWNode->u32TDE_S2_FILL,
                pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);

            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_DISA;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_FILL;

            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

        }
        break;
	    case TDE_NORM_BLIT_1OPT:/*signal blit*//*CNcomment:普通单源操作搬移*/
        {
            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_DISA;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_BMP;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

        }
        break;
	    case TDE_NORM_FILL_2OPT:/*signal color with bitmap operation and blit*//*CNcomment:单色和位图运算操作搬移*/
        {
            TDE_ASSERT(HI_NULL != pstColorFill);
            TDE_FILL_DATA_BY_FMT(pHWNode->u32TDE_S2_FILL,
                pstColorFill->u32FillData, pstColorFill->enDrvColorFmt);
            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_FILL;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

        }
        break;
      #endif
	    case TDE_NORM_BLIT_2OPT:/*double blit*//*CNcomment:普通双源操作搬移 */
        {
            TDE_BITMASKROP_CHECK(enAlu,u32Capability);
            TDE_BITMASKBLEND_CHECK(enAlu,u32Capability);

            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_BMP;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

        }
        break;
	    case TDE_MB_2OPT:/*mb combination operation*//*CNcomment:宏块合并操作*/
        {
            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32MbEn = 1;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);


        }
        break;
        #ifndef HI_BUILD_IN_BOOT
	    case TDE_MB_C_OPT:/*mb cbcr sampling operation*//*CNcomment:宏块色度上采样操作*/
        {
            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_DISA;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32MbEn = 1;
            unAluMode.stBits.u32AluMod = TDE_SRC2_BYPASS;
        }
        break;
	    case TDE_MB_Y_OPT:/*mb ligthness resize*//*CNcomment:宏块亮度缩放*/
        {
            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_DISA;
            unIns.stBits.u32MbEn = 1;
            unAluMode.stBits.u32AluMod = TDE_SRC2_BYPASS;
        }
        break;
	    case TDE_SINGLE_SRC_PATTERN_FILL_OPT:
	    {
            TDE_PATTERNFILL_CHECK(u32Capability);

	        unIns.stBits.u32Src1Mod = TDE_SRC_MODE_DISA;
	        unIns.stBits.u32Src2Mod = TDE_SRC_MODE_PATTERN;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

	        break;
	    }
	    case TDE_DOUBLE_SRC_PATTERN_FILL_OPT:
        {
            TDE_PATTERNFILL_CHECK(u32Capability);

            unIns.stBits.u32Src1Mod = TDE_SRC_MODE_BMP;
            unIns.stBits.u32Src2Mod = TDE_SRC_MODE_PATTERN;
            unAluMode.stBits.u32AluMod = TdeHalNodeGetAluMod( enMode, enAlu);

            break;
        }
	   #endif
	    default:
        	break;
    }

    /*set node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_ALU = unAluMode.u32All;
    pHWNode->u32TDE_INS = unIns.u32All;
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
    TDE_ALU_U unAluMode;

    TDE_ASSERT(HI_NULL != pHWNode);

    /*set node*//*CNcomment:配置缓存节点*/
    unAluMode.u32All = pHWNode->u32TDE_ALU;
    unAluMode.stBits.u32GlobalAlpha = u8Alpha;
    pHWNode->u32TDE_ALU = unAluMode.u32All;
#endif
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
    TDE_COLOR_CONV_U unConv;

    TDE_ASSERT(HI_NULL != pHWNode);


    /*set alpha0 alpha1*//*CNcomment:配置alpha0, alpha1*/
    unConv.u32All = pHWNode->u32TDE_COLOR_CONV;
    unConv.stBits.u32Alpha0 = u8Alpha0;
    unConv.stBits.u32Alpha1 = u8Alpha1;
    pHWNode->u32TDE_COLOR_CONV = unConv.u32All;

    if(TDE_DRV_SRC_S1 & enSrc)
    {
        pHWNode->u32TDE_S1_TYPE |= 0x20000000;
    }

    if(TDE_DRV_SRC_S2 & enSrc)
    {
        pHWNode->u32TDE_S2_TYPE |= 0x20000000;
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
    TDE_2D_RSZ_U unRsz;
    TDE_ASSERT(HI_NULL != pHWNode);
    unRsz.u32All = pHWNode->u32TDE_2D_RSZ;
    unRsz.stBits.u32AlpBorder = (((HI_U32)bVEnable << 1) | (HI_U32)bHEnable);
    return;
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
    TDE_ALU_U unAluMode;
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&ROP))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Rop\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    /*set node*//*CNcomment:配置缓存节点*/
    unAluMode.u32All = pHWNode->u32TDE_ALU;
    unAluMode.stBits.u32RgbRopMod = (HI_U32)enRgbRop;
    unAluMode.stBits.u32AlphaRopMod = (HI_U32)enAlphaRop;
    pHWNode->u32TDE_ALU = unAluMode.u32All;
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
    TDE_ALPHA_BLEND_U unAlphaBlend;
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&ALPHABLEND))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Blend\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    unAlphaBlend.u32All = pHWNode->u32TDE_ALPHA_BLEND;
    /*those paramter is fix ,app cann't view*//*CNcomment: 这四个参数驱动固定，对外不可见 */
    unAlphaBlend.stBits.u32Src1PixelAlphaEn = HI_TRUE;
    unAlphaBlend.stBits.u32Src1GlobalAlphaEn = HI_FALSE;
    unAlphaBlend.stBits.u32Src1MultiGlobalEn = HI_FALSE;
    unAlphaBlend.stBits.u32Src2MultiGlobalEn = HI_FALSE;

    unAlphaBlend.stBits.u32Src1PremultiEn = pstBlendOpt->bSrc1AlphaPremulti;
    unAlphaBlend.stBits.u32Src2PremultiEn = pstBlendOpt->bSrc2AlphaPremulti;
    unAlphaBlend.stBits.u32Src2PixelAlphaEn = pstBlendOpt->bPixelAlphaEnable;
    unAlphaBlend.stBits.u32Src2GlobalAlphaEn = pstBlendOpt->bGlobalAlphaEnable;

    /*set mode for src1 and src2*//*CNcomment:  配置Src1、Src2模式 */
    switch(pstBlendOpt->eBlendCmd)
    {
        /**< fs: sa      fd: 1.0-sa */
        case TDE2_BLENDCMD_NONE:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_INVSRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_SRC2ALPHA;
            break;
        }
        /**< fs: 0.0     fd: 0.0 */
        case TDE2_BLENDCMD_CLEAR:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ZERO;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ZERO;
            break;
        }
        /**< fs: 1.0     fd: 0.0 */
        case TDE2_BLENDCMD_SRC:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ZERO;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ONE;
            break;
        }
        /**< fs: 1.0     fd: 1.0-sa */
        case TDE2_BLENDCMD_SRCOVER:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_INVSRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ONE;
            break;
        }
        /**< fs: 1.0-da  fd: 1.0 */
        case TDE2_BLENDCMD_DSTOVER:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ONE;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_INVSRC1ALPHA;
            break;
        }
        /**< fs: da      fd: 0.0 */
        case TDE2_BLENDCMD_SRCIN:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ZERO;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_SRC1ALPHA;
            break;
        }
        /**< fs: 0.0     fd: sa */
        case TDE2_BLENDCMD_DSTIN:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_SRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ZERO;
            break;
        }
        /**< fs: 1.0-da  fd: 0.0 */
        case TDE2_BLENDCMD_SRCOUT:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ZERO;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_INVSRC1ALPHA;
            break;
        }
        /**< fs: 0.0     fd: 1.0-sa */
        case TDE2_BLENDCMD_DSTOUT:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_INVSRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ZERO;
            break;
        }
        /**< fs: da      fd: 1.0-sa */
        case TDE2_BLENDCMD_SRCATOP:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_INVSRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_SRC1ALPHA;
            break;
        }
        /**< fs: 1.0-da  fd: sa */
        case TDE2_BLENDCMD_DSTATOP:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_SRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_INVSRC1ALPHA;
            break;
        }
        /**< fs: 1.0     fd: 1.0 */
        case TDE2_BLENDCMD_ADD:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ONE;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ONE;
            break;
        }
        /**< fs: 1.0-da  fd: 1.0-sa */
        case TDE2_BLENDCMD_XOR:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_INVSRC2ALPHA;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_INVSRC1ALPHA;
            break;
        }
        /**< fs: 0.0  fd: 1.0*/
        case TDE2_BLENDCMD_DST:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = TDE2_BLEND_ONE;
            unAlphaBlend.stBits.u32Src2BlendMode = TDE2_BLEND_ZERO;
            break;
        }
        /*user parameter*//*CNcomment:  用户自己配置参数 */
        case TDE2_BLENDCMD_CONFIG:
        default:
        {
            unAlphaBlend.stBits.u32Src1BlendMode = pstBlendOpt->eSrc1BlendMode;
            unAlphaBlend.stBits.u32Src2BlendMode = pstBlendOpt->eSrc2BlendMode;
            break;
        }
    }
    pHWNode->u32TDE_ALPHA_BLEND = unAlphaBlend.u32All;
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
    TDE_SRC_TYPE_U unSrcType;
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&COLORIZE))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Colorize\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unSrcType.u32All = pHWNode->u32TDE_S2_TYPE;
    unSrcType.stBits.u32ColorizeEnable = HI_TRUE;
    pHWNode->u32TDE_S2_TYPE = unSrcType.u32All;

    pHWNode->u32TDE_COLORIZE = u32Colorize;
    return HI_SUCCESS;
}

HI_VOID TdeHalNodeEnableAlphaRop(TDE_HWNode_S *pHWNode)
{
    TDE_ALPHA_BLEND_U unAlphaBlend;

    unAlphaBlend.u32All = pHWNode->u32TDE_ALPHA_BLEND;

    unAlphaBlend.stBits.u32AlphaRopEn = HI_TRUE;/*enable alpha rop operation*//*CNcomment: 使能Alpha Rop操作*/

    pHWNode->u32TDE_ALPHA_BLEND = unAlphaBlend.u32All;
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
    TDE_INS_U unIns;
    TDE_COLOR_CONV_U unConv;
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pClutCmd);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CLUT))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Clut\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32Clut = 1; /*enable clut operation*//*CNcomment: 使能Clut操作*/

    unConv.u32All = pHWNode->u32TDE_COLOR_CONV;
    unConv.stBits.u32ClutMod = (HI_U32)pClutCmd->enClutMode;
    unConv.stBits.u32ClutReload = (HI_U32)bReload; /*reload clut to register*//*CNcomment: reload clut 到寄存器 */

     /*set node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_CLUT_ADDR = pClutCmd->u32PhyClutAddr;
    pHWNode->u32TDE_INS = unIns.u32All;
    pHWNode->u32TDE_COLOR_CONV = unConv.u32All;
#endif
    return HI_SUCCESS;
}
#ifndef HI_BUILD_IN_BOOT
static HI_VOID TdeHalNodeSetYcbcrColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey, TDE_ALU_U* unAluMode)
{
    pHWNode->u32TDE_CK_MIN = pColorKey->unColorKeyValue.struCkYCbCr.stCr.u8CompMin
        | (pColorKey->unColorKeyValue.struCkYCbCr.stCb.u8CompMin << 8)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stY.u8CompMin << 16)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stAlpha.u8CompMin << 24);
    pHWNode->u32TDE_CK_MAX = pColorKey->unColorKeyValue.struCkYCbCr.stCr.u8CompMax
        | (pColorKey->unColorKeyValue.struCkYCbCr.stCb.u8CompMax << 8)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stY.u8CompMax << 16)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stAlpha.u8CompMax << 24);
    pHWNode->u32TDE_CK_MASK = pColorKey->unColorKeyValue.struCkYCbCr.stCr.u8CompMask
        | (pColorKey->unColorKeyValue.struCkYCbCr.stCb.u8CompMask << 8)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stY.u8CompMask << 16)
        | (pColorKey->unColorKeyValue.struCkYCbCr.stAlpha.u8CompMask << 24);

    if (pColorKey->unColorKeyValue.struCkYCbCr.stCr.bCompIgnore)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkYCbCr.stCr.bCompOut)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkYCbCr.stCb.bCompIgnore)
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkYCbCr.stCb.bCompOut)
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkYCbCr.stY.bCompIgnore)
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkYCbCr.stY.bCompOut)
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkYCbCr.stAlpha.bCompIgnore)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkYCbCr.stAlpha.bCompOut)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_IN;
    }


    return;
}

static HI_VOID TdeHalNodeSetCLUTColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey, TDE_ALU_U* unAluMode)
{
    pHWNode->u32TDE_CK_MIN = pColorKey->unColorKeyValue.struCkClut.stClut.u8CompMin
        | (pColorKey->unColorKeyValue.struCkClut.stAlpha.u8CompMin << 24);
    pHWNode->u32TDE_CK_MAX = pColorKey->unColorKeyValue.struCkClut.stClut.u8CompMax
        | (pColorKey->unColorKeyValue.struCkClut.stAlpha.u8CompMax << 24);
    pHWNode->u32TDE_CK_MASK = pColorKey->unColorKeyValue.struCkClut.stClut.u8CompMask
        | (pColorKey->unColorKeyValue.struCkClut.stAlpha.u8CompMask << 24);

    if (pColorKey->unColorKeyValue.struCkClut.stClut.bCompIgnore)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkClut.stClut.bCompOut)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkClut.stAlpha.bCompIgnore)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkClut.stAlpha.bCompOut)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_IN;
    }

    return;
}

static HI_VOID TdeHalNodeSetARGBColorKeyPara(TDE_HWNode_S* pHWNode, TDE_DRV_COLORKEY_CMD_S* pColorKey, TDE_ALU_U*unAluMode)
{
    pHWNode->u32TDE_CK_MIN = pColorKey->unColorKeyValue.struCkARGB.stBlue.u8CompMin
        | (pColorKey->unColorKeyValue.struCkARGB.stGreen.u8CompMin << 8)
        | (pColorKey->unColorKeyValue.struCkARGB.stRed.u8CompMin << 16)
        | (pColorKey->unColorKeyValue.struCkARGB.stAlpha.u8CompMin << 24);
    pHWNode->u32TDE_CK_MAX = pColorKey->unColorKeyValue.struCkARGB.stBlue.u8CompMax
        | (pColorKey->unColorKeyValue.struCkARGB.stGreen.u8CompMax << 8)
        | (pColorKey->unColorKeyValue.struCkARGB.stRed.u8CompMax << 16)
        | (pColorKey->unColorKeyValue.struCkARGB.stAlpha.u8CompMax << 24);
    pHWNode->u32TDE_CK_MASK = pColorKey->unColorKeyValue.struCkARGB.stBlue.u8CompMask
        | (pColorKey->unColorKeyValue.struCkARGB.stGreen.u8CompMask << 8)
        | (pColorKey->unColorKeyValue.struCkARGB.stRed.u8CompMask << 16)
        | (pColorKey->unColorKeyValue.struCkARGB.stAlpha.u8CompMask << 24);

    if (pColorKey->unColorKeyValue.struCkARGB.stBlue.bCompIgnore)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkARGB.stBlue.bCompOut)
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkBMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkARGB.stGreen.bCompIgnore)
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkARGB.stGreen.bCompOut)
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkGMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkARGB.stRed.bCompIgnore)
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkARGB.stRed.bCompOut)
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkRMod = TDE_COLORKEY_AREA_IN;
    }

    if (pColorKey->unColorKeyValue.struCkARGB.stAlpha.bCompIgnore)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_IGNORE;
    }
    else if (pColorKey->unColorKeyValue.struCkARGB.stAlpha.bCompOut)
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_OUT;
    }
    else
    {
        unAluMode->stBits.u32CkAMod = TDE_COLORKEY_AREA_IN;
    }

    return;
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
    TDE_INS_U unIns;
    TDE_ALU_U unAluMode;
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pColorKey);

    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&COLORKEY))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support ColorKey\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32ColorKey = 1; /*enable color key operation*//*CNcomment:使能Color Key操作*/
    unAluMode.u32All = pHWNode->u32TDE_ALU;
    unAluMode.stBits.u32CkSel = (HI_U32)pColorKey->enColorKeyMode;


    if (TDE_COLORFMT_CATEGORY_ARGB == enFmtCat)
    {
        TdeHalNodeSetARGBColorKeyPara(pHWNode, pColorKey, &unAluMode);
    }
    else if (TDE_COLORFMT_CATEGORY_CLUT == enFmtCat)
    {
        TdeHalNodeSetCLUTColorKeyPara(pHWNode, pColorKey, &unAluMode);
    }
    else if (TDE_COLORFMT_CATEGORY_YCbCr == enFmtCat)
    {
        TdeHalNodeSetYcbcrColorKeyPara(pHWNode, pColorKey, &unAluMode);
    }
    else
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support ColorKey\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    pHWNode->u32TDE_ALU = unAluMode.u32All;
    pHWNode->u32TDE_INS = unIns.u32All;

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
    TDE_INS_U unIns;
    TDE_SUR_SIZE_U unClipPos;
    HI_U32 u32Capability;
    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pClip);

    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CLIP))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Clip\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32Clip = 1; /*enable clip operation*//*CNcomment:启用clip操作*/

	unClipPos.u32All = 0;
    if(!pClip->bInsideClip)
    {
        /*over clip*//*CNcomment: 区域外clip指示 */
        unClipPos.u32All = 0x80000000;
    }
    unClipPos.stBits.u32Width = pClip->u16ClipStartX;
    unClipPos.stBits.u32Height = pClip->u16ClipStartY;
    pHWNode->u32TDE_CLIP_START = unClipPos.u32All;

    unClipPos.u32All = 0; /*reset for next set*//*CNcomment:  重新复位,准备下一设置 AI7D02798 */
    unClipPos.stBits.u32Width = pClip->u16ClipEndX;
    unClipPos.stBits.u32Height = pClip->u16ClipEndY;
    pHWNode->u32TDE_CLIP_STOP = unClipPos.u32All;

    pHWNode->u32TDE_INS = unIns.u32All;
#endif
    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeSetFlicker
* Description:  set flicker operation parameter
* Input:         pHWNode: pointer of node
*                pFlicker: flicker coefficient
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetFlicker(TDE_HWNode_S* pHWNode, TDE_DRV_FLICKER_CMD_S* pFlicker)
{
    TDE_INS_U unIns;
    TDE_2D_RSZ_U unRsz;
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pFlicker);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&DEFLICKER))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Deflicker\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32DfeEn = 1; /*enbale Flicker*//*CNcomment:使能抗闪烁*/
    unRsz.u32All = pHWNode->u32TDE_2D_RSZ;
    if (pFlicker->enDeflickerMode == TDE2_DEFLICKER_MODE_RGB)
    {
        unRsz.stBits.u32DfeAlphaDisable = 1;
    }
    else
    {
        unRsz.stBits.u32DfeAlphaDisable = 0;
    }
    unRsz.stBits.u32DfeMod = (HI_U32)pFlicker->enDfeMode;

    /*set node*//*CNcomment:配置缓存节点*/
    switch(s_eDeflickerLevel)
    {
    	#ifndef HI_BUILD_IN_BOOT/**codecc**/
        case TDE_DEFLICKER_LOW:
        {
            unRsz.stBits.u32DfeMod = 0;
            pHWNode->u32TDE_DFE_COEF0 = 0x0f003307;
            break;
        }
        case TDE_DEFLICKER_MIDDLE:
        {
            unRsz.stBits.u32DfeMod = 0;
            pHWNode->u32TDE_DFE_COEF0 = 0x0f002010;
            break;
        }
        case TDE_DEFLICKER_HIGH:
        {
            unRsz.stBits.u32DfeMod = 0;
            pHWNode->u32TDE_DFE_COEF0 = 0x0f001a13;
            break;
        }
        #endif
        default:
        {
            pHWNode->u32TDE_DFE_COEF0 = (HI_U32)pFlicker->u8Coef0NextLine
                | ((HI_U32)pFlicker->u8Coef0CurLine << 8)
                | ((HI_U32)pFlicker->u8Coef0LastLine << 16)
                | ((HI_U32)pFlicker->u8Threshold0 << 24);
            pHWNode->u32TDE_DFE_COEF1 = (HI_U32)pFlicker->u8Coef1NextLine
                | ((HI_U32)pFlicker->u8Coef1CurLine << 8)
                | ((HI_U32)pFlicker->u8Coef1LastLine << 16)
                | ((HI_U32)pFlicker->u8Threshold1 << 24);
            pHWNode->u32TDE_DFE_COEF2 = (HI_U32)pFlicker->u8Coef2NextLine
                | ((HI_U32)pFlicker->u8Coef2CurLine << 8)
                | ((HI_U32)pFlicker->u8Coef2LastLine << 16)
                | ((HI_U32)pFlicker->u8Threshold2 << 24);
            pHWNode->u32TDE_DFE_COEF3 = (HI_U32)pFlicker->u8Coef3NextLine
                | ((HI_U32)pFlicker->u8Coef3CurLine << 8)
                | ((HI_U32)pFlicker->u8Coef3LastLine << 16);
                    break;
        }
    }

    pHWNode->u32TDE_INS = unIns.u32All;
    pHWNode->u32TDE_2D_RSZ = unRsz.u32All;
    return HI_SUCCESS;
}
/*****************************************************************************
* Function:      TdeHalNodeSetResize
* Description:   set resize operation parameter
* Input:         pHWNode: pointer of node
*                pResize: resize coefficient
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32 TdeHalNodeSetResize(TDE_HWNode_S* pHWNode, TDE_FILTER_OPT* pstFilterOpt,
TDE_NODE_SUBM_TYPE_E enNodeType)
{
    TDE_INS_U unIns;
    TDE_2D_RSZ_U unRsz;
    HI_U32 u32Sign = 0;
    TDE_TAR_TYPE_U unTarType;
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pstFilterOpt);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&RESIZE))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support Resize\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unIns.u32All = pHWNode->u32TDE_INS;

    unRsz.u32All = pHWNode->u32TDE_2D_RSZ;
    if(NO_SCALE_STEP==pstFilterOpt->u32VStep)
    {
        unRsz.stBits.u32VfMod = TDE_DRV_FILTER_NONE;
    }
    else
    {
        unRsz.stBits.u32VfMod = pstFilterOpt->enFilterMode;
        /*plumb Flicker*//*CNcomment: 垂直滤波 */
        if (TDE_DRV_FILTER_NONE != pstFilterOpt->enFilterMode)
        {
            unRsz.stBits.u32VfRingEn = pstFilterOpt->bVRing;  /*enable middle Flicker*//*CNcomment: 使能中值滤波*/
            unRsz.stBits.u32VfCoefReload = 0x1; /*load parameter*//*CNcomment:载入垂直参数*/
            /*set v resize parameter of start address*//*CNcomment:根据步长配置垂直缩放参数表首地址 */
            pHWNode->u32TDE_VF_COEF_ADDR = s_stParaTable.u32VfCoefAddr
                          + TdeHalGetResizeParaVTable(pstFilterOpt->u32VStep) * TDE_PARA_VTABLE_SIZE;
            if (TDE_NODE_SUBM_PARENT == enNodeType)
            {
           /*set plumb offset*//*CNcomment:  设置垂直偏移 */
            u32Sign = ((HI_U32)pstFilterOpt->s32VOffset)>> 31; /*get symbol*//*CNcomment: 提取符号*/
            pHWNode->u32TDE_RSZ_Y_OFST = (u32Sign << 16) | ((HI_U32)pstFilterOpt->s32VOffset & 0xffff);
            unIns.stBits.u32Resize = 0x1; /*enable resize*//*CNcomment: 使能缩放*/
            }
        }
        else
        {
            unRsz.stBits.u32VfMod = 0x0;
             if (TDE_NODE_SUBM_PARENT == enNodeType)
            {
           /*set plumb offset*//*CNcomment:  设置垂直偏移 */
            u32Sign = ((HI_U32)pstFilterOpt->s32VOffset) >> 31; /*get symbol*//*CNcomment: 提取符号*/
            pHWNode->u32TDE_RSZ_Y_OFST = (u32Sign << 16) | ((HI_U32)pstFilterOpt->s32VOffset & 0xffff);
            unIns.stBits.u32Resize = 0x1; /*enable resize*//*CNcomment: 使能缩放*/
            }
        }
    }

    if(NO_SCALE_STEP==pstFilterOpt->u32HStep)
    {
        unRsz.stBits.u32HfMod = TDE_DRV_FILTER_NONE;
    }
    else
    {
        unRsz.stBits.u32HfMod = pstFilterOpt->enFilterMode;
        /*h Flicker*//*CNcomment: 水平滤波 */
        if (TDE_DRV_FILTER_NONE != pstFilterOpt->enFilterMode)
        {
            unRsz.stBits.u32HfRingEn = pstFilterOpt->bHRing; /*enable middle Flicker*//*CNcomment: 使能中值滤波*/
            unRsz.stBits.u32HfCoefReload = 0x1;/*load parameter*//*CNcomment:载入垂直参数*/
            /*set h resize parameter of start address*//*CNcomment:根据步长配置垂直缩放参数表首地址 */
            pHWNode->u32TDE_HF_COEF_ADDR = s_stParaTable.u32HfCoefAddr
                        + TdeHalGetResizeParaHTable(pstFilterOpt->u32HStep) * TDE_PARA_HTABLE_SIZE;
            if (TDE_NODE_SUBM_PARENT == enNodeType)
            {
            /*set h offset*//*CNcomment:  设置水平偏移 */
            u32Sign = ((HI_U32)pstFilterOpt->s32HOffset) >> 31; /*get symbol*//*CNcomment: 提取符号*/
            pHWNode->u32TDE_RSZ_X_OFST = (u32Sign << 16) | ((HI_U32)pstFilterOpt->s32HOffset & 0xffff);
            unIns.stBits.u32Resize = 0x1; /*enable resize*//*CNcomment:使能缩放*/
            }
        }
        else
        {
            unRsz.stBits.u32HfMod = 0x0;
            if (TDE_NODE_SUBM_PARENT == enNodeType)
            {
            /*set h offset*//*CNcomment:  设置水平偏移 */
            u32Sign = ((HI_U32)pstFilterOpt->s32HOffset)>> 31; /*get symbol*//*CNcomment: 提取符号*/
            pHWNode->u32TDE_RSZ_X_OFST = (u32Sign << 16) | ((HI_U32)pstFilterOpt->s32HOffset & 0xffff);
            unIns.stBits.u32Resize = 0x1; /*enable resize*//*CNcomment:使能缩放*/
            }
        }
    }

    unRsz.stBits.u32CoefSym = pstFilterOpt->bCoefSym;/*Flicker parameter*//*CNcomment:对称滤波系数*/
    pHWNode->u32TDE_2D_RSZ = unRsz.u32All;
    if (TDE_NODE_SUBM_PARENT == enNodeType)
    {
        pHWNode->u32TDE_INS = unIns.u32All;

        unTarType.u32All = pHWNode->u32TDE_TAR_TYPE;
        unTarType.stBits.u32DfeFirstlineOutEn = pstFilterOpt->bFirstLineOut;
        unTarType.stBits.u32DfeLastlineOutEn = pstFilterOpt->bLastLineOut;
        pHWNode->u32TDE_TAR_TYPE = unTarType.u32All;

        /*set flicker step*//*CNcomment:   设置滤波步长 */
        pHWNode->u32TDE_RSZ_STEP = pstFilterOpt->u32HStep;
        pHWNode->u32TDE_RSZ_VSTEP = pstFilterOpt->u32VStep;

    }

    return HI_SUCCESS;
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
    TDE_INS_U unIns;
    TDE_COLOR_CONV_U unConv;
    HI_U32 u32Capability;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pConv);
    TdeHalGetCapability(&u32Capability);
    if(!(u32Capability&CSCCOVERT))
    {
        TDE_TRACE(TDE_KERN_INFO, "It deos not support CSCCovert\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    unIns.u32All = pHWNode->u32TDE_INS;
    unConv.u32All = pHWNode->u32TDE_COLOR_CONV;

    unIns.stBits.u32IcsConv = (HI_U32)pConv->bInConv;
    unIns.stBits.u32OcsConv = (HI_U32)pConv->bOutConv;
    unIns.stBits.u32IcscSel = (HI_U32)pConv->bInSrc1Conv;

    unConv.stBits.u32InRgb2Yc = (HI_U32)pConv->bInRGB2YC;
    unConv.stBits.u32InColorSpace = (HI_U32)pConv->bInMetrixVid;
    unConv.stBits.u32InColorImetry = (HI_U32)pConv->bInMetrix709;
    unConv.stBits.u32OutColorSpace = (HI_U32)pConv->bOutMetrixVid;
    unConv.stBits.u32OutColorImetry = (HI_U32)pConv->bOutMetrix709;

    /*set node*//*CNcomment:配置缓存节点*/
    pHWNode->u32TDE_COLOR_CONV = unConv.u32All;
    pHWNode->u32TDE_INS = unIns.u32All;

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeHalNodeAddChild
* Description:   add child node on Flicker
* Input:         pHWNode: pointer of node
*                pChildInfo: child info
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeAddChild(TDE_HWNode_S* pHWNode, TDE_CHILD_INFO* pChildInfo)
{
    TDE_SUR_XY_U unXy;
    TDE_INS_U unIns;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pChildInfo);

    /* config register according update info
       u32TDE_RSZ_OFST
       u32TDE_2D_RSZ
       TDE_S2_XY
       TDE_TAR_XY
       TDE_S2_SIZE
       TDE_TS_SIZE
       */

    /*CNcomment: 根据Update信息配置寄存器:
       u32TDE_RSZ_OFST
       u32TDE_2D_RSZ
       TDE_S2_XY
       TDE_TAR_XY
       TDE_S2_SIZE
       TDE_TS_SIZE
       */
    if (pChildInfo->u64Update & 0x1)
    {
        TdeHalSetXyByAdjInfo(pHWNode, pChildInfo);
    }

    if (pChildInfo->u64Update & 0x2)
    {
        TDE_SUR_SIZE_U unSz;
        unSz.u32All = pHWNode->u32TDE_S2_SIZE;
        unSz.stBits.u32Width = pChildInfo->u32Wi;
        unSz.stBits.u32Height = pChildInfo->u32Hi;
        pHWNode->u32TDE_S2_SIZE = unSz.u32All;
    }

    if (pChildInfo->u64Update & 0x4)
    {
        HI_U32 u32Sign;

        /*set h offset*//* CNcomment: 设置水平偏移 */
        u32Sign = pChildInfo->u32HOfst >> 31;/*get symbol*/ /*CNcomment:提取符号*/
        pHWNode->u32TDE_RSZ_X_OFST = (u32Sign << 16) | (pChildInfo->u32HOfst & 0xffff);
        /*set v offset*//* CNcomment: 设置垂直偏移 */
        u32Sign = pChildInfo->u32VOfst >> 31; /*get symbol*/ /*CNcomment:提取符号*/
        pHWNode->u32TDE_RSZ_Y_OFST = (u32Sign << 16) | (pChildInfo->u32VOfst & 0xffff);

    }

    if (pChildInfo->u64Update & 0x8)
    {
        unXy.u32All = pHWNode->u32TDE_TAR_XY;
        unXy.stBits.u32X = pChildInfo->u32Xo;
        unXy.stBits.u32Y = pChildInfo->u32Yo;
        pHWNode->u32TDE_TAR_XY = unXy.u32All;
        if(pChildInfo->stDSAdjInfo.bDoubleSource)
        {
            unXy.u32All = pHWNode->u32TDE_S1_XY;
            unXy.stBits.u32X = (HI_U32)(pChildInfo->u32Xo + pChildInfo->stDSAdjInfo.s32DiffX);
            unXy.stBits.u32Y = (HI_U32)(pChildInfo->u32Yo + pChildInfo->stDSAdjInfo.s32DiffY);
            pHWNode->u32TDE_S1_XY = unXy.u32All;
        }
    }

    if (pChildInfo->u64Update & 0x10)
    {
        TDE_SUR_SIZE_U unSz;
        unSz.u32All = pHWNode->u32TDE_TS_SIZE;
        unSz.stBits.u32Width = pChildInfo->u32Wo;
        unSz.stBits.u32Height = pChildInfo->u32Ho;
        pHWNode->u32TDE_TS_SIZE = unSz.u32All;
    }
    /*fill block info*/ /*CNcomment: 填充块类型信息 */
    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32BlockFlag = (HI_U32)pChildInfo->enSliceType;
    if (pChildInfo->enSliceType == TDE_FIRST_BLOCK_SLICE_TYPE)
    {
        unIns.stBits.u32FirstBlockFlag = 1;
        unIns.stBits.u32LastBlockFlag = 0;
    }
    else if (pChildInfo->enSliceType == TDE_LAST_BLOCK_SLICE_TYPE)
    {
        unIns.stBits.u32FirstBlockFlag = 0;
        unIns.stBits.u32LastBlockFlag = 1;
    }
    else
    {
        unIns.stBits.u32FirstBlockFlag = 0;
        unIns.stBits.u32LastBlockFlag = 0;
    }
    pHWNode->u32TDE_INS = unIns.u32All;
    return;
}

/*****************************************************************************
* Function:      TdeHalNodeSetMbMode
* Description:   set parameter for deal with mb
* Input:         pHWNode: pointer of node
*                pMbCmd: parameter
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeHalNodeSetMbMode(TDE_HWNode_S* pHWNode, TDE_DRV_MB_CMD_S* pMbCmd)
{
    TDE_INS_U unIns;

    TDE_ASSERT(HI_NULL != pHWNode);
    TDE_ASSERT(HI_NULL != pMbCmd);

    unIns.u32All = pHWNode->u32TDE_INS;
    unIns.stBits.u32MbMode = (HI_U32)pMbCmd->enMbMode;
    pHWNode->u32TDE_INS = unIns.u32All;
    return;
}

HI_S32 TdeHalNodeSetCsc(TDE_HWNode_S* pHWNode, TDE2_CSC_OPT_S stCscOpt)
{
	TDE_COLOR_CONV_U unColorConv;
	TDE_INS_U unIns;

	TDE_ASSERT(HI_NULL != pHWNode);

	unColorConv.u32All = pHWNode->u32TDE_COLOR_CONV;
	unIns.u32All = pHWNode->u32TDE_INS;

	unColorConv.stBits.u32IcscCustomEn = stCscOpt.bICSCUserEnable;
	unColorConv.stBits.u32IcscReload = stCscOpt.bICSCParamReload;
	unColorConv.stBits.u32OcscCustomEn = stCscOpt.bOCSCUserEnable;
	unColorConv.stBits.u32OcscReload = stCscOpt.bOCSCParamReload;

	/*use correct parameter when user not use user parameter and enable ICSC function*/
    /*CNcomment:若用户未开启自定义系数且ICSC功能使能,
    则强制使用TDE?男拚凳?(TDE默认系数存在问题)*/
    if (unIns.stBits.u32IcsConv)
    {
		if (!stCscOpt.bICSCUserEnable)
		{
	        unColorConv.stBits.u32IcscCustomEn = 1;
	    	unColorConv.stBits.u32IcscReload = 1;

		    if (unColorConv.stBits.u32InRgb2Yc)
	        {
	            pHWNode->u32TDE_ICSC_ADDR = s_u32Rgb2YuvCoefAddr;
	        }
		    else
	        {
	            pHWNode->u32TDE_ICSC_ADDR = s_u32Yuv2RgbCoefAddr;
	        }
		}
		else
		{
#ifdef CONFIG_GFX_MMU_SUPPORT
			HI_U8 *pMmzCsc = HI_NULL;
			HI_U8 *pMmuCsc = HI_NULL;
#ifdef TDE_CACH_STRATEGY
			GFX_MMZ_BUFFER_S stFlushBuf = {0};
#endif

			pMmuCsc = HI_GFX_Map(stCscOpt.u32ICSCParamAddr,TDE_CSC_SIZE, HI_TRUE);
			if (HI_NULL == pMmuCsc)
			{
				TDE_TRACE(TDE_KERN_INFO, "Map csc failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			pMmzCsc = TDE_MALLOC(TDE_CSC_SIZE);
			if (HI_NULL == pMmzCsc)
			{
				TDE_TRACE(TDE_KERN_INFO, "Alloc csc failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			HI_GFX_Memcpy(pMmzCsc, pMmuCsc, TDE_CSC_SIZE);

			HI_GFX_Unmap(pMmuCsc, HI_TRUE);

			pHWNode->u32TDE_ICSC_ADDR = wgetphy(pMmzCsc);

#ifdef TDE_CACH_STRATEGY
			stFlushBuf.u32Size = TDE_CSC_SIZE;
			stFlushBuf.u32StartPhyAddr = pHWNode->u32TDE_ICSC_ADDR;
			stFlushBuf.pu8StartVirAddr = pMmzCsc;

			HI_GFX_Flush(&stFlushBuf,HI_FALSE);
#endif
#else
			pHWNode->u32TDE_ICSC_ADDR = stCscOpt.u32ICSCParamAddr;
#endif
		}
    }

    if (unIns.stBits.u32OcsConv)
    {
		if (!stCscOpt.bOCSCUserEnable)
		{
	        unColorConv.stBits.u32OcscCustomEn = 1;
	        unColorConv.stBits.u32OcscReload = 1;
	    	if (unColorConv.stBits.u32InRgb2Yc)
	        {
	            pHWNode->u32TDE_OCSC_ADDR = s_u32Yuv2RgbCoefAddr;
	        }
	    	else
	        {
	            pHWNode->u32TDE_OCSC_ADDR = s_u32Rgb2YuvCoefAddr;
	        }
		}
		else
		{
#ifdef CONFIG_GFX_MMU_SUPPORT
			HI_U8 *pMmzCsc = HI_NULL;
			HI_U8 *pMmuCsc = HI_NULL;
#ifdef TDE_CACH_STRATEGY
			GFX_MMZ_BUFFER_S stFlushBuf = {0};
#endif

			pMmuCsc = HI_GFX_Map(stCscOpt.u32OCSCParamAddr,TDE_CSC_SIZE, HI_TRUE);
			if (HI_NULL == pMmuCsc)
			{
				TDE_TRACE(TDE_KERN_INFO, "Map csc failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			pMmzCsc = TDE_MALLOC(TDE_CSC_SIZE);
			if (HI_NULL == pMmzCsc)
			{
				TDE_TRACE(TDE_KERN_INFO, "Alloc csc failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			HI_GFX_Memcpy(pMmzCsc, pMmuCsc, TDE_CSC_SIZE);

			HI_GFX_Unmap(pMmuCsc, HI_TRUE);

			pHWNode->u32TDE_OCSC_ADDR = wgetphy(pMmzCsc);

#ifdef TDE_CACH_STRATEGY
			stFlushBuf.u32Size = TDE_CSC_SIZE;
			stFlushBuf.u32StartPhyAddr = pHWNode->u32TDE_OCSC_ADDR;
			stFlushBuf.pu8StartVirAddr = pMmzCsc;

			HI_GFX_Flush(&stFlushBuf,HI_FALSE);
#endif
#else
			pHWNode->u32TDE_OCSC_ADDR = stCscOpt.u32OCSCParamAddr;
#endif
		}
    }

    pHWNode->u32TDE_COLOR_CONV = unColorConv.u32All;

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
    #define OrgHfCoef Tde8x32Coef
    HI_U32 i = 0;
    HI_U32 j = 0;
    TDE_FILTER_PARA_U unPara;
    HI_U32 *pHfCoef = HI_NULL;
    HI_U32 *pVfCoef = HI_NULL;
    HI_U32 *pRgb2YuvCsc = HI_NULL;
    HI_U32 *pYuv2RgbCsc = HI_NULL;

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

    /*copy parameter according other offer way*//* CNcomment:按照算法组提供的结构拷贝参数表 */
    for (i = 0; i < TDE_PARA_HTABLE_NUM; i++)
    {
        HI_GFX_Memcpy(pHfCoef + i*(TDE_PARA_HTABLE_SIZE/4), OrgHfCoef + i*(TDE_PARA_HTABLE_ORG_SIZE/4), (TDE_PARA_HTABLE_ORG_SIZE));
    }
    for (i = 0, j = 0; i < TDE_PARA_VTABLE_ORG_SIZE * TDE_PARA_VTABLE_NUM - 4; j++)
    {
        unPara.stBits.u32Para0 = OrgVfCoef[i++];
        unPara.stBits.u32Para1 = OrgVfCoef[i++];
        unPara.stBits.u32Para2 = OrgVfCoef[i++];
        unPara.stBits.u32Para3 = OrgVfCoef[i++];
        unPara.stBits.u32Para4 = OrgVfCoef[i++];
        *(pVfCoef + j) = unPara.u32All;
    }

    HI_GFX_Memset(&s_stParaTable, 0, sizeof(s_stParaTable));

    s_stParaTable.u32HfCoefAddr = wgetphy((HI_VOID *)pHfCoef);
    s_stParaTable.u32VfCoefAddr = wgetphy((HI_VOID *)pVfCoef);

    pRgb2YuvCsc = (HI_U32 *)TDE_MALLOC(TDE_CSCTABLE_SIZE * sizeof(HI_U32));
    if (HI_NULL == pRgb2YuvCsc)
    {
        TDE_FREE(pHfCoef);
        TDE_FREE(pVfCoef);
        TDE_TRACE(TDE_KERN_INFO, "Alloc csc coef failed!\n");
        return HI_FAILURE;
    }

    pYuv2RgbCsc = (HI_U32 *)TDE_MALLOC(TDE_CSCTABLE_SIZE * sizeof(HI_U32));
    if (HI_NULL == pYuv2RgbCsc)
    {
        TDE_TRACE(TDE_KERN_INFO, "Alloc csc coef failed\n");
        TDE_FREE(pHfCoef);
        TDE_FREE(pVfCoef);
        TDE_FREE(pRgb2YuvCsc);
        return HI_FAILURE;
    }

    HI_GFX_Memcpy(pRgb2YuvCsc, s_u32Rgb2YuvCsc, TDE_CSCTABLE_SIZE * sizeof(HI_U32));
    HI_GFX_Memcpy(pYuv2RgbCsc, s_u32Yuv2RgbCsc, TDE_CSCTABLE_SIZE * sizeof(HI_U32));

    s_u32Rgb2YuvCoefAddr = wgetphy((HI_VOID *)pRgb2YuvCsc);
    s_u32Yuv2RgbCoefAddr = wgetphy((HI_VOID *)pYuv2RgbCsc);

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
    else if (TDE_RESIZE_8X32_AREA_0 <= u32Step && TDE_RESIZE_8X32_AREA_1 > u32Step)
    {
        u32Index = 1;
    }
    else if (TDE_RESIZE_8X32_AREA_1 <= u32Step && TDE_RESIZE_8X32_AREA_2 > u32Step)
    {
        u32Index = 2;
    }
    else if (TDE_RESIZE_8X32_AREA_2 <= u32Step && TDE_RESIZE_8X32_AREA_3 > u32Step)
    {
        u32Index = 3;
    }
    else if (TDE_RESIZE_8X32_AREA_3 <= u32Step && TDE_RESIZE_8X32_AREA_4 > u32Step)
    {
        u32Index = 4;
    }
    else /* TDE_RESIZE_8X32_AREA_4 <= u32Step */
    {
        u32Index = 5;
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
    HI_BOOL bResizeParaVTableEqualToOne = (u32Step >= TDE_RESIZE_PARA_AREA_0 && u32Step < TDE_RESIZE_PARA_AREA_1);
    HI_BOOL bResizeParaVTableEqualToTWO = (u32Step >= TDE_RESIZE_PARA_AREA_1 && u32Step < TDE_RESIZE_PARA_AREA_2);
    HI_BOOL bResizeParaVTableEqualToThree = (u32Step >= TDE_RESIZE_PARA_AREA_2 && u32Step < TDE_RESIZE_PARA_AREA_3);
    HI_BOOL bResizeParaVTableEqualToFour = (u32Step >= TDE_RESIZE_PARA_AREA_3 && u32Step < TDE_RESIZE_PARA_AREA_4);
    HI_BOOL bResizeParaVTableEqualToFive = (u32Step >= TDE_RESIZE_PARA_AREA_4 && u32Step < TDE_RESIZE_PARA_AREA_5);
    HI_BOOL bResizeParaVTableEqualToSix = (u32Step >= TDE_RESIZE_PARA_AREA_5 && u32Step < TDE_RESIZE_PARA_AREA_6);
    HI_BOOL bResizeParaVTableEqualToSeven = (u32Step >= TDE_RESIZE_PARA_AREA_6 && u32Step < TDE_RESIZE_PARA_AREA_7);
    HI_BOOL bResizeParaVTableEqualToEight = (u32Step >= TDE_RESIZE_PARA_AREA_7 && u32Step < TDE_RESIZE_PARA_AREA_8);
    HI_BOOL bResizeParaVTableEqualToNine = (u32Step >= TDE_RESIZE_PARA_AREA_8 && u32Step < TDE_RESIZE_PARA_AREA_9);
    HI_BOOL bResizeParaVTableEqualToTen = (u32Step >= TDE_RESIZE_PARA_AREA_9 && u32Step < TDE_RESIZE_PARA_AREA_10);

    /*get index table according step*//*CNcomment: 根据步长找到表的索引 */
    if (u32Step < TDE_RESIZE_PARA_AREA_0)
    {
        u32Index = 0;
    }
    else if (bResizeParaVTableEqualToOne)
    {
        u32Index = 1;
    }
    else if (bResizeParaVTableEqualToTWO)
    {
        u32Index = 2;
    }
    else if (bResizeParaVTableEqualToThree)
    {
        u32Index = 3;
    }
    else if (bResizeParaVTableEqualToFour)
    {
        u32Index = 4;
    }
    else if (bResizeParaVTableEqualToFive)
    {
        u32Index = 5;
    }
    else if (bResizeParaVTableEqualToSix)
    {
        u32Index = 6;
    }
    else if (bResizeParaVTableEqualToSeven)
    {
        u32Index = 7;
    }
    else if (bResizeParaVTableEqualToEight)
    {
        u32Index = 8;
    }
    else if (bResizeParaVTableEqualToNine)
    {
        u32Index = 9;
    }
    else if (bResizeParaVTableEqualToTen)
    {
        u32Index = 10;
    }
    else /* u32Step >= TDE_RESIZE_PARA_AREA_10 */
    {
        u32Index = 11;
    }

    return u32Index;
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
#endif

/*****************************************************************************
* Function:      TdeHalSetXyByAdjInfo
* Description:   pHWNode:   hardware node info
*                pChildInfo: child info
* Input:         none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_VOID TdeHalSetXyByAdjInfo(TDE_HWNode_S* pHWNode, TDE_CHILD_INFO* pChildInfo)
{
    TDE_SUR_XY_U unXy;

    switch (pChildInfo->stAdjInfo.enScaleMode)
    {
	    case TDE_CHILD_SCALE_NORM:
	    case TDE_CHILD_SCALE_MBC:
        {
            /*CNcomment: 直接按照S2分块 */
            unXy.u32All = pHWNode->u32TDE_S2_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;
            pHWNode->u32TDE_S2_XY = unXy.u32All;
            break;
        }
	    case TDE_CHILD_SCALE_MBY:
        {
            /*CNcomment: 直接按照S1分块 */
            unXy.u32All = pHWNode->u32TDE_S1_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;
            pHWNode->u32TDE_S1_XY = unXy.u32All;
            break;
        }
	    case TDE_CHILD_SCALE_MB_CONCA_H:
	    case TDE_CHILD_SCALE_MB_CONCA_M:
        {
            /*CNcomment: 按照亮度S1分块, 调整色度S2起始位置 */
            unXy.u32All = pHWNode->u32TDE_S1_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;
            pHWNode->u32TDE_S1_XY = unXy.u32All;
            unXy.u32All = pHWNode->u32TDE_S2_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi - pChildInfo->stAdjInfo.u32StartInX;
            unXy.stBits.u32Y = 0;
            pHWNode->u32TDE_S2_XY = unXy.u32All;
            break;
        }
	    case TDE_CHILD_SCALE_MB_CONCA_L:
        {
            /*CNcomment: 按照亮度S1分块, 调整色度S2起始位置 */
            unXy.u32All = pHWNode->u32TDE_S1_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;
            pHWNode->u32TDE_S1_XY = unXy.u32All;

            unXy.u32All = pHWNode->u32TDE_S2_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;

            if (TDE_DRV_COLOR_FMT_YCbCr420MB == pChildInfo->stAdjInfo.enFmt
                || TDE_DRV_COLOR_FMT_YCbCr422MBH == pChildInfo->stAdjInfo.enFmt)
            {
                unXy.stBits.u32X = (pChildInfo->u32Xi >> 1);
            }
            if (TDE_DRV_COLOR_FMT_YCbCr420MB == pChildInfo->stAdjInfo.enFmt
                || TDE_DRV_COLOR_FMT_YCbCr422MBV == pChildInfo->stAdjInfo.enFmt)
            {
                unXy.stBits.u32Y = (pChildInfo->u32Yi >> 1);
            }

            pHWNode->u32TDE_S2_XY = unXy.u32All;
            break;
        }
	    case TDE_CHILD_SCALE_MB_CONCA_CUS:
        {
            /*CNcomment: 按照色度S2分块, 调整亮度S1起始位置 */
            unXy.u32All = pHWNode->u32TDE_S2_XY;
            unXy.stBits.u32X = pChildInfo->u32Xi;
            unXy.stBits.u32Y = pChildInfo->u32Yi;
            pHWNode->u32TDE_S2_XY = unXy.u32All;

            unXy.u32All = pHWNode->u32TDE_S1_XY;
            unXy.stBits.u32X = pChildInfo->u32Xo - pChildInfo->stAdjInfo.u32StartOutX
                               + pChildInfo->stAdjInfo.u32StartInX;
            unXy.stBits.u32Y = pChildInfo->stAdjInfo.u32StartInY;
            pHWNode->u32TDE_S1_XY = unXy.u32All;
			break;
	 	}
	    default:
	        break;
    }
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
    TDE_AQ_CTRL_U unAqCtrl;

    /*write 0 to Aq list start address register*/
    /*CNcomment: 将Aq链表首地址寄存器写0 */
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_NADDR, 0);

    unAqCtrl.u32All = TDE_READ_REG(s_pu32BaseVirAddr, TDE_AQ_CTRL);


    /*enable Aq list*//*CNcomment: 使能Aq链表 */
    unAqCtrl.stBits.u32AqEn = 1;/*hardware disable*//*CNcomment: 硬件暂时屏蔽*/

    /*set Aq operation mode*//*CNcomment:  配置Aq操作模式 */
    unAqCtrl.stBits.u32AqOperMode = TDE_AQ_CTRL_COMP_LINE;
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AQ_CTRL, unAqCtrl.u32All);
    TDE_WRITE_REG(s_pu32BaseVirAddr, TDE_AXI_ID, 0x3010);
}

#ifndef HI_BUILD_IN_BOOT
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
HI_VOID TdeHalNodePrintInfo(struct seq_file *p,HI_U32 *pu32CurNode)
{
    HI_S32 i;
     /* see define of TDE_HWNode_S */
    HI_U8*  chUpdate[] =
    {
        "INS         ",
        "S1_ADDR     ",
        "S1_TYPE     ",
        "S1_XY       ",
        "S1_FILL     ",
        "S2_ADDR     ",
        "S2_TYPE     ",
        "S2_XY       ",
        "S2_SIZE     ",
        "S2_FILL     ",
        "TAR_ADDR    ",
        "TAR_TYPE    ",
        "TAR_XY      ",
        "TS_SIZE     ",
        "COLOR_CONV  ",
        "CLUT_ADDR   ",
        "2D_RSZ      ",
        "HF_COEF_ADDR",
        "VF_COEF_ADDR",
        "RSZ_STEP    ",
        "RSZ_Y_OFST  ",
        "RSZ_X_OFST  ",
        "DFE_COEF0   ",
        "DFE_COEF1   ",
        "DFE_COEF2   ",
        "DFE_COEF3   ",
        "ALU         ",
        "CK_MIN      ",
        "CK_MAX      ",
        "CLIP_START  ",
        "CLIP_STOP   ",
        "Y1_ADDR     ",
        "Y1_PITCH    ",
        "Y2_ADDR     ",
        "Y2_PITCH    ",
        "RSZ_VSTEP   ",
        "ARGB_ORDER  ",
        "CK_MASK     ",
        "COLORIZE    ",
        "ALPHA_BLEND ",
        "ICSC_ADDR   ",
        "OCSC_ADDR   "
    };

    /* print node information */
    PROC_PRINT(p,"\n--------- Hisilicon TDE Node params Info ---------\n");

    for (i = 0; i < sizeof(TDE_HWNode_S) / 4; i++)
    {
        PROC_PRINT(p, "(%s):\t0x%08x\n", chUpdate[i], *(pu32CurNode + i));
    }

    return;
}
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

