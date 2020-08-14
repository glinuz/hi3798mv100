/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_osictl_k.c
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
#include <linux/module.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/irq.h>
#include "tde_proc.h"
#else
#include "exports.h"
#endif
#include "tde_define.h"
#include "tde_handle.h"
#include "tde_osictl.h"
#include "tde_osilist.h"
#include "tde_hal.h"
#include "wmalloc.h"
#include "hi_gfx_sys_k.h"

#include "tde_adp.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif  /* __cplusplus */
#endif  /* __cplusplus */

/****************************************************************************/
/*                             TDE osi ctl macro definition					*/
/****************************************************************************/
#define TDE_OSI_FOUR_BITS_SHIFT    4
#define TDE_OSI_THREE_BITS_SHIFT   3
#define TDE_OSI_TWO_BITS_SHIFT     2
#define TDE_OSI_ONE_BIT_SHIFT     1
#define TDE_OSI_ZERO_BIT_SHIFT    0


#define TDE_MIN(a, b) \
    (((a) > (b)) ? (b) : (a))

#define TDE_CALC_MAX_SLICE_NUM(bFF, vstep, in_w) \
    ((HI_FALSE == (bFF) && TDE_NO_SCALE_STEP == (vstep)) ? (1) : ((in_w) / 118 + 5)) /*AI7D02711*/

#define TDE_FILLUP_RECT_BY_DRVSURFACE(rect, drvSur) \
    do {\
        (rect).s32Xpos   = (HI_S32)(drvSur).u32Xpos; \
        (rect).s32Ypos   = (HI_S32)(drvSur).u32Ypos; \
        (rect).u32Width  = (drvSur).u32Width; \
        (rect).u32Height = (drvSur).u32Height; \
    } while (0)

/*AI7D02579 beg*/
#define TDE_CHANGE_DIR_BY_MIRROR(pDirect, enMirror) \
    do{\
        switch (enMirror)\
        {\
        case TDE2_MIRROR_HORIZONTAL:\
            (pDirect)->enHScan = !((pDirect)->enHScan);\
            break;\
        case TDE2_MIRROR_VERTICAL:\
            (pDirect)->enVScan = !((pDirect)->enVScan);\
            break;\
        case TDE2_MIRROR_BOTH:\
            (pDirect)->enHScan = !((pDirect)->enHScan);\
            (pDirect)->enVScan = !((pDirect)->enVScan);\
            break;\
        default:\
            break;\
        }\
    } while(0)
/*AI7D02579 end*/

#define TDE_SET_CLIP(phwNode, pstOpt) \
    do {\
        if (TDE2_CLIPMODE_NONE != (pstOpt)->enClipMode)\
        {\
            TDE_DRV_CLIP_CMD_S stClipCmd; \
            stClipCmd.u16ClipStartX = (HI_U16)(pstOpt)->stClipRect.s32Xpos; \
            stClipCmd.u16ClipStartY = (HI_U16)(pstOpt)->stClipRect.s32Ypos; \
            stClipCmd.u16ClipEndX = ((pstOpt)->stClipRect.s32Xpos \
                                     + (pstOpt)->stClipRect.u32Width - 1); \
            stClipCmd.u16ClipEndY = (pstOpt)->stClipRect.s32Ypos \
                                    + (pstOpt)->stClipRect.u32Height - 1; \
            stClipCmd.bInsideClip = (TDE2_CLIPMODE_INSIDE == (pstOpt)->enClipMode) \
                                    ? HI_TRUE : HI_FALSE; \
            if(TdeHalNodeSetClipping(phwNode, &stClipCmd)<0) return HI_ERR_TDE_UNSUPPORTED_OPERATION;\
        } \
    } while (0)

/*
start % 4 = 0: (h-1)/4 + 1
start % 4 = 1: (h/4) + 1
start % 4 = 2: (h+1)/4 + 1
start % 4 = 3: (h+2)/4 + 1
 */
#define TDE_ADJ_SIZE_BY_START_I(start, size) \
    (((size) + (((start) % 4) - 1)) / 4 + 1)

/*
when bottom filed, adjust height, only according by 4 integal-multiple of start
address.so, start reserve is not used
*/
#define TDE_ADJ_B_SIZE_BY_START_I(start, size) \
    ((size >> 2) + ((size % 4) >> 1))
/*
start even number:  (w-1)/2 +1
start odd number: w/2 + 1
 */
#define TDE_ADJ_SIZE_BY_START_P(start, size) \
    (((size) + (((start) % 2) - 1)) / 2 + 1)


#define TDE_ADJ_FIELD_HEIGHT_BY_START(y, h) \
    ((h) / 2 + (((y) & 1) & ((h) & 1)))


/* AI7D02880
    return CbCr422R fill value
      ------------
      |Y'|Cb|Y|Cr|
      ------------
       ^_____|
      copy Y to Y'
*/
#define TDE_GET_YC422R_FILLVALUE(value) \
        ((value) & 0xffffff) | ((((value) >> 8) & 0xff) << 24)

#define TDE_UNIFY_RECT(psrcRect, pdstRect)\
    do{\
        if ((psrcRect)->u32Height != (pdstRect)->u32Height)\
        {\
            (psrcRect)->u32Height = TDE_MIN((psrcRect)->u32Height, (pdstRect)->u32Height);\
            (pdstRect)->u32Height = (psrcRect)->u32Height;\
        }\
        if((psrcRect)->u32Width != (pdstRect)->u32Width)\
        {\
            (psrcRect)->u32Width = TDE_MIN((psrcRect)->u32Width, (pdstRect)->u32Width);\
            (pdstRect)->u32Width = (psrcRect)->u32Width;\
        }\
    }while(0)

#define TDE_STRIDE_CHECK_ALIGN(u32Stride, u32AlignNum)\
    do{\
        if(u32Stride % u32AlignNum)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "stride is not aligned! %s\n");\
            return HI_ERR_TDE_NOT_ALIGNED;\
        }\
    }while(0)

#define TDE_ADDRESS_CHECK_ALIGN(u32PhyAddr, u32AlignNum)\
    do{\
        if(u32PhyAddr % u32AlignNum)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Bitmap address is not aligned!\n");\
            return HI_ERR_TDE_NOT_ALIGNED;\
        }\
    }while(0)


#define TDE_CHECK_COLORFMT(enColorFmt) do{\
        if(TDE2_COLOR_FMT_BUTT <= enColorFmt)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "color format error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_MBCOLORFMT(enMbColorFmt) \
    do{\
        if(TDE2_MB_COLOR_FMT_BUTT <= enMbColorFmt)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "mb color format error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_OUTALPHAFROM(enOutAlphaFrom) \
    do{\
        if(TDE2_OUTALPHA_FROM_BUTT <= enOutAlphaFrom)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_ROPCODE(enRopCode) \
    do{\
        if(TDE2_ROP_BUTT <= enRopCode)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enRopCode error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)


#define TDE_CHECK_ALUCMD(enAluCmd) \
    do{\
            if(TDE2_ALUCMD_BUTT <= enAluCmd)\
            {\
                TDE_TRACE(TDE_KERN_INFO, "enAluCmd error!\n");\
                return HI_ERR_TDE_INVALID_PARA;\
            }\
    }while(0)

#define TDE_CHECK_MIRROR(enMirror) \
    do{\
        if(TDE2_MIRROR_BUTT <= enMirror)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enMirror error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_CLIPMODE(enClipMode) \
    do{\
        if(TDE2_CLIPMODE_BUTT <= enClipMode)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enClipMode error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_MBRESIZE(enMbResize) \
    do{\
        if(TDE2_MBRESIZE_BUTT <= enMbResize)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enMbResize error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_MBPICMODE(enMbPicMode) \
    do{\
        if(TDE_PIC_MODE_BUTT <= enMbPicMode)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enMbPicMode error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_COLORKEYMODE(enColorKeyMode) \
    do{\
        if(TDE2_COLORKEY_MODE_BUTT <= enColorKeyMode)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enColorKeyMode error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_FILTERMODE(enFilterMode) \
    do{\
        if(TDE2_FILTER_MODE_BUTT <= enFilterMode)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enFilterMode error:%d!\n",enFilterMode);\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_ROTATE(enRotate) \
    do{\
        if(TDE_ROTATE_BUTT <= enRotate)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "enRotate error!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)

#define TDE_CHECK_SUBBYTE_STARTX(startx, w, outFmt) \
    do {\
        HI_S32 s32Bpp;\
        s32Bpp = TdeOsiGetbppByFmt(outFmt);\
        if (0 > s32Bpp)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Unkown color format!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
        \
        if (s32Bpp < 8)\
        {\
            /* when writing, subbyte format align ask start point byte align */\
            if (((startx) * s32Bpp % 8) || ((w) * s32Bpp % 8))\
            {\
                TDE_TRACE(TDE_KERN_INFO, "Subbyte's start or width for writing is not aligned!\n");\
                return HI_ERR_TDE_INVALID_PARA;\
            }\
        }\
    }while(0)

#define TDE_CHECK_BLENDCMD(enBlendCmd) \
    do{\
            if (enBlendCmd >= TDE2_BLENDCMD_BUTT)\
            {\
                TDE_TRACE(TDE_KERN_INFO, "Unknown blend cmd!\n");\
                return HI_ERR_TDE_INVALID_PARA;\
            }\
    }while(0)

#define TDE_CHECK_BLENDMODE(enBlendMode) \
	do{\
		if (enBlendMode >= TDE2_BLEND_BUTT)\
		{\
			TDE_TRACE(TDE_KERN_INFO, "Unknown blend mode!\n");\
			return HI_ERR_TDE_INVALID_PARA;\
		}\
	}while(0)


#define TDE_CHECK_NOT_MB(enFmt) do\
    {\
        if (enFmt >= TDE2_COLOR_FMT_JPG_YCbCr400MBP)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "This operation doesn't support Semi-plannar!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    } while (0)

#define TDE_PATTERNBACKGROUNDCATEGORY_CHECK_CLUT(enBackGroundCategory, enForeGroundCategory, enBackGroundFmt,enForeGroundFmt, enDstFmt, enAluCmd , enDstCategory)\
    do{\
        if (( TDE_COLORFMT_CATEGORY_CLUT == enBackGroundCategory ) &&( TDE2_ALUCMD_NONE != enAluCmd ))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support ROP!\n");\
            return -1;\
        }\
        if (( TDE_COLORFMT_CATEGORY_CLUT == enBackGroundCategory )&& ((enBackGroundFmt != enForeGroundFmt) || (enBackGroundFmt != enDstFmt)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "If background, foreground , dst are clut, they should be the same fmt!\n");\
            return -1;\
        }\
        if ((TDE_COLORFMT_CATEGORY_CLUT == enBackGroundCategory)&& ((TDE_COLORFMT_CATEGORY_CLUT != enForeGroundCategory)|| (TDE_COLORFMT_CATEGORY_CLUT != enDstCategory)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Unsupported operation!\n");\
            return -1;\
        }\
    }while(0)

#define TDE_BACKGROUNDCATEGORY_CHECK_CLUT(enBackGroundCategory, enForeGroundCategory, bResize, enDeflickerMode, enAluCmd , enDstCategory)\
    do{\
        if ((TDE_COLORFMT_CATEGORY_CLUT == enBackGroundCategory)&& ((TDE_COLORFMT_CATEGORY_CLUT != enForeGroundCategory)|| (TDE_COLORFMT_CATEGORY_CLUT != enDstCategory)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Unsupported operation!\n");\
            return -1;\
        }\
        if (((enDeflickerMode != TDE2_DEFLICKER_MODE_NONE)||(bResize) || (TDE2_ALUCMD_NONE != enAluCmd)) && (TDE_COLORFMT_CATEGORY_CLUT == enBackGroundCategory))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support deflicker or ROP or mirror!\n"); \
            return -1; \
        }\
    }while (0)

#define TDE_BACKGROUNDCATEGORY_CHECK_AN(enBackGroundCategory, enForeGroundCategory, enDstCategory ,enAluCmd )\
    do{\
        TDE_PATTERNBACKGROUNDCATEGORY_CHECK_AN(enBackGroundCategory, enForeGroundCategory, enDstCategory ,enAluCmd );\
    }while(0)


#define TDE_PATTERNBACKGROUNDCATEGORY_CHECK_AN(enBackGroundCategory, enForeGroundCategory, enDstCategory ,enAluCmd )\
    do{\
        if (( TDE_COLORFMT_CATEGORY_An == enBackGroundCategory )\
            && ( TDE_COLORFMT_CATEGORY_An == enForeGroundCategory )\
            && ( TDE_COLORFMT_CATEGORY_An == enDstCategory )\
            && ( TDE2_ALUCMD_NONE != enAluCmd ))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support ROP or mirror!\n");\
            return -1;\
        }\
    }while(0)


#define TDE_BACKGROUNDCATEGORY_CHECK_ARGB(enBackGroundCategory, enForeGroundCategory, enDstFmt)\
    do{\
        if ((TDE_COLORFMT_CATEGORY_ARGB == enBackGroundCategory || TDE_COLORFMT_CATEGORY_YCbCr == enBackGroundCategory)\
            && (TDE_COLORFMT_CATEGORY_An == enForeGroundCategory)&& (!TdeOsiWhetherContainAlpha(enDstFmt)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Target must have alpha component!\n");\
            return -1;\
        }\
    }while(0)


#define TDE_PATTERNBACKGROUNDCATEGORY_CHECK_ARGB(enBackGroundCategory, enForeGroundCategory, enDstFmt)\
    do{\
        if (( TDE_COLORFMT_CATEGORY_ARGB == enBackGroundCategory )&&( TDE_COLORFMT_CATEGORY_An == enForeGroundCategory )\
            &&( !TdeOsiWhetherContainAlpha(enDstFmt) ))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Target must have alpha component!\n");\
            return -1;\
        }\
    }while(0)



#define TDE_PATTERNFOREGROUNDCATEGORY_CHECK_AN(enForeGroundCategory, enBackGroundFmt, enForeGroundFmt)\
    do{\
        if ((TDE_COLORFMT_CATEGORY_An == enForeGroundCategory)\
            &&\
            ((TDE2_COLOR_FMT_YCbCr888 == enBackGroundFmt) || (TDE2_COLOR_FMT_AYCbCr8888 == enForeGroundFmt)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Target must have alpha component!\n");\
            return -1;\
        }\
    }while(0)

#define TDE_PATTERNDSTCATEGORY_CHECK_CLUT(enBackGroundCategory, enForeGroundCategory, enDstCategory)\
    do{\
        if ((enDstCategory == TDE_COLORFMT_CATEGORY_CLUT)\
            &&\
            ((TDE_COLORFMT_CATEGORY_CLUT != enForeGroundCategory) || (TDE_COLORFMT_CATEGORY_CLUT != enBackGroundCategory)))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Unsupported operation!\n");\
            return -1;\
        }\
    }while(0)


#define TDE_YCBCR422_FMT_CHECK_ODD(enColorFmt, s32Xpos, u32Width)\
    do{\
        if (((s32Xpos & 0x1) || (u32Width & 0x1)) && (TDE2_COLOR_FMT_YCbCr422 == enColorFmt))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "x, width of YCbCr422R couldn't be odd!\n");\
            return -1;\
        }\
    }while(0)

#define TDE_YCBCR_FMT_CHECK_STRIDE(u32CbCrStride, enColorFmt)\
    do{\
        if ( ((u32CbCrStride > TDE_MAX_SURFACE_PITCH) || (u32CbCrStride == 0))\
             && (enColorFmt != TDE2_COLOR_FMT_JPG_YCbCr400MBP) )\
        {\
            TDE_TRACE(TDE_KERN_INFO, "Invalid CbCr stride!\n");\
            return -1;\
        }\
    }while(0)

#define TDE_CHECK_SURFACEPARA(pstSurface, pstRect)\
    do{\
        if((TDE_MAX_SURFACE_PITCH < pstSurface->u32Stride)\
           || (0 == pstSurface->u32Stride)\
           || (0 == pstRect->u32Height)\
           || (0 == pstRect->u32Width)\
           || (pstRect->s32Xpos < 0)\
           || ((HI_U32)pstRect->s32Xpos >= pstSurface->u32Width)\
           || (pstRect->s32Ypos < 0)\
           || ((HI_U32)pstRect->s32Ypos >= pstSurface->u32Height)\
           || (0 == pstSurface->u32PhyAddr))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "sw:%d, sh:%d, stride:%x, sphy:%x, x:%d, y:%d, w:%d, h:%d\n", pstSurface->u32Width,\
                      pstSurface->u32Height, pstSurface->u32Stride,  pstSurface->u32PhyAddr, pstRect->s32Xpos, \
                      pstRect->s32Ypos,pstRect->u32Width, pstRect->u32Height);\
            TDE_TRACE(TDE_KERN_INFO, "invalid surface phyaddr or invalid surface size or operation area!");\
            return -1;\
        }\
    }while(0)

#define TDE_CHECK_MBCBCRPARA(pstMbSurface)\
    do{\
        if ((TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP < pstMbSurface->enMbFmt) && (TDE2_MB_COLOR_FMT_BUTT > pstMbSurface->enMbFmt))\
        {\
            if (0 == pstMbSurface->u32CbCrStride)\
            {\
                TDE_TRACE(TDE_KERN_INFO, " mb u32CbCrStride is null !\n");\
                return HI_ERR_TDE_INVALID_PARA;\
            }\
            if (0 == pstMbSurface->u32CbCrPhyAddr)\
            {\
                TDE_TRACE(TDE_KERN_INFO, "mb u32CbCrPhyAddr is 0!\n");\
                return HI_ERR_TDE_INVALID_PARA;\
            }\
        }\
    }while(0)

#define TDE_CHECK_MBSURFACEPARA(pstMbSurface, pstRect , rectWidth, rectHeight)\
    do{\
        if ((pstRect->s32Xpos < 0)\
            || (rectWidth < pstRect->u32Width)\
            || (rectHeight < pstRect->u32Height)\
            || (0 == pstRect->u32Height)\
            || (0 == pstRect->u32Width)\
            || (TDE_MAX_SURFACE_PITCH < pstMbSurface->u32YStride)\
            || (TDE_MAX_SURFACE_PITCH < pstMbSurface->u32CbCrStride)\
            || ((HI_U32)pstRect->s32Xpos >= pstMbSurface->u32YWidth)\
            || (pstRect->s32Ypos < 0)\
            || ((HI_U32)pstRect->s32Ypos >= pstMbSurface->u32YHeight)\
            || (0 == pstMbSurface->u32YPhyAddr))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "syw:%d, syh:%d, systride:%x, syphy:%x, scbcrstride:%x, scbcrphy:%x, x:%d, y:%d, w:%d, h:%d\n", pstMbSurface->u32YWidth,\
                      pstMbSurface->u32YHeight, pstMbSurface->u32YStride, pstMbSurface->u32YPhyAddr,pstMbSurface->u32CbCrStride,pstMbSurface->u32CbCrPhyAddr,\
                      pstRect->s32Xpos,pstRect->s32Ypos, pstRect->u32Width, pstRect->u32Height);\
            TDE_TRACE(TDE_KERN_INFO, "invalid mbsurface phyaddr or invalid surface size or operation area!\n");\
            return -1;\
        }\
    }while(0)

/****************************************************************************/
/*                         TDE osi ctl struct definition					*/
/****************************************************************************/

/* pixel format transform type */
typedef enum hiTDE_COLORFMT_TRANSFORM_E
{
    TDE_COLORFMT_TRANSFORM_ARGB_ARGB = 0,
    TDE_COLORFMT_TRANSFORM_ARGB_YCbCr,
    TDE_COLORFMT_TRANSFORM_CLUT_ARGB,
    TDE_COLORFMT_TRANSFORM_CLUT_YCbCr,
    TDE_COLORFMT_TRANSFORM_CLUT_CLUT,
    TDE_COLORFMT_TRANSFORM_YCbCr_ARGB,
    TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr,
    TDE_COLORFMT_TRANSFORM_An_An,
    TDE_COLORFMT_TRANSFORM_ARGB_An,
    TDE_COLORFMT_TRANSFORM_YCbCr_An,
    TDE_COLORFMT_TRANSFORM_BUTT,
} TDE_COLORFMT_TRANSFORM_E;

/* CLUT table use */
typedef enum hiTDE_CLUT_USAGE_E
{
    TDE_CLUT_COLOREXPENDING = 0,    /* color expend */
    TDE_CLUT_COLORCORRECT,          /* color correct */
    TDE_CLUT_CLUT_BYPASS,
    TDE_CLUT_USAGE_BUTT
} TDE_CLUT_USAGE_E;

/* frame/filed operate mode */
typedef enum hiTDE_PIC_MODE_E
{
    TDE_FRAME_PIC_MODE = 0,     /* frame operate mode */
    TDE_TOP_FIELD_PIC_MODE,     /* top filed operate mode */
    TDE_PIC_MODE_BUTT
} TDE_PIC_MODE_E;

typedef struct hiTDE_SLICE_INFO_S
{
    HI_U32 u32SliceXi;
    HI_U32 u32SliceWi;
    HI_U32 s32SliceHOfst;
    HI_U32 u32SliceXo;
    HI_U32 u32SliceWo;
    TDE_SLICE_TYPE_E enSliceType;
} TDE_SLICE_INFO_S;



typedef enum hiTDE_OPERATION_CATEGORY_E
{
    TDE_OPERATION_SINGLE_SRC1 = 0,
    TDE_OPERATION_SINGLE_SRC2,
    TDE_OPERATION_DOUBLE_SRC,
    TDE_OPERATION_BUTT
} TDE_OPERATION_CATEGORY_E;
typedef enum hiTDE_PATTERN_OPERATION_CATEGORY_E
{
    TDE_PATTERN_OPERATION_SINGLE_SRC = 0,
    TDE_PATTERN_OPERATION_DOUBLE_SRC,
    TDE_PATTERN_OPERATION_BUTT
}TDE_PATTERN_OPERATION_CATEGORY_E;


typedef struct tagTDE_MBPARA_S
{
    HI_U32 u32WorkBufNum;
    TDE2_OUTALPHA_FROM_E enAlphaFrom;
    TDE_DRV_SURFACE_S stDrvS1;
    TDE_DRV_SURFACE_S stDrvS2;
    TDE_DRV_SURFACE_S stDrvS1Tmp;
    TDE_DRV_SURFACE_S stDrvS2Tmp;
    TDE_DRV_CONV_MODE_CMD_S stConv;
    TDE_DRV_SURFACE_S stDrvDst;
    TDE2_RECT_S stInRect;
    TDE2_CSC_OPT_S stCscOpt ;
    TDE_SCANDIRECTION_S stScan;
    TDE_FILTER_OPT stFilterOpt;
    TDE_PIC_MODE_E enPicMode;
    TDE_DRV_MB_CMD_S stMbCmd;
    TDE2_RECT_S stAdjFieldRect;
}
TDE_MBPARA_S;

typedef struct tagTDE_OVERLAPSLICE_DATUM
{
    HI_U32 u32Xi ;
    HI_U32 u32Wi;
    HI_U32 s32HOfst;
    HI_U32 u32Xo ;
    HI_U32 u32Wo;
    HI_U32 u32OfstWord;
    HI_U32 u32LeftWi;
    HI_U32 u32LeftWo ;
    HI_U32 u32CalcWi ;
    HI_U32 u32BackPix ;
}TDE_OVERLAPSLICE_DATUM;

typedef struct tagTDE2_FMT_BITOFFSET_S
{
    HI_U8 u8AlphaBits;
    HI_U8 u8RedBits;
    HI_U8 u8GreenBits;
    HI_U8 u8BlueBits;
    HI_U8 u8AlphaOffset;
    HI_U8 u8RedOffset;
    HI_U8 u8GreenOffset;
    HI_U8 u8BlueOffset;
}TDE2_FMT_BITOFFSET_S;

/****************************************************************************/
/*                     TDE osi ctl inner variables definition   			*/
/****************************************************************************/
STATIC TDE_DRV_COLOR_FMT_E g_enTdeCommonDrvColorFmt[TDE2_COLOR_FMT_BUTT + 1] = {
    TDE_DRV_COLOR_FMT_RGB444,   TDE_DRV_COLOR_FMT_RGB444,
    TDE_DRV_COLOR_FMT_RGB555,   TDE_DRV_COLOR_FMT_RGB555,
    TDE_DRV_COLOR_FMT_RGB565,   TDE_DRV_COLOR_FMT_RGB565,
    TDE_DRV_COLOR_FMT_RGB888,   TDE_DRV_COLOR_FMT_RGB888,
    TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444,
    TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555,
    TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565,
    TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888,
    TDE_DRV_COLOR_FMT_RABG8888,
    TDE_DRV_COLOR_FMT_CLUT1,    TDE_DRV_COLOR_FMT_CLUT2,    TDE_DRV_COLOR_FMT_CLUT4,
    TDE_DRV_COLOR_FMT_CLUT8,    TDE_DRV_COLOR_FMT_ACLUT44,  TDE_DRV_COLOR_FMT_ACLUT88,
    TDE_DRV_COLOR_FMT_A1,       TDE_DRV_COLOR_FMT_A8,
    TDE_DRV_COLOR_FMT_YCbCr888, TDE_DRV_COLOR_FMT_AYCbCr8888,TDE_DRV_COLOR_FMT_YCbCr422,
    TDE_DRV_COLOR_FMT_byte,     TDE_DRV_COLOR_FMT_halfword,
    TDE_DRV_COLOR_FMT_YCbCr400MBP,
    TDE_DRV_COLOR_FMT_YCbCr422MBH, TDE_DRV_COLOR_FMT_YCbCr422MBV,
    TDE_DRV_COLOR_FMT_YCbCr420MB,  TDE_DRV_COLOR_FMT_YCbCr420MB,  TDE_DRV_COLOR_FMT_YCbCr420MB,
    TDE_DRV_COLOR_FMT_YCbCr420MB,
    TDE_DRV_COLOR_FMT_YCbCr444MB, TDE_DRV_COLOR_FMT_BUTT
};
STATIC TDE_DRV_ARGB_ORDER_E g_enTdeArgbOrder[TDE2_COLOR_FMT_BUTT + 1] = {
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_RABG,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT, TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT,
    TDE_DRV_ORDER_BUTT
};

STATIC TDE_DRV_COLOR_FMT_E g_enTdeMbDrvColorFmt[TDE2_MB_COLOR_FMT_BUTT + 1] = {
    TDE_DRV_COLOR_FMT_YCbCr400MBP,
    TDE_DRV_COLOR_FMT_YCbCr422MBH, TDE_DRV_COLOR_FMT_YCbCr422MBV,
    TDE_DRV_COLOR_FMT_YCbCr420MB,  TDE_DRV_COLOR_FMT_YCbCr420MB,  TDE_DRV_COLOR_FMT_YCbCr420MB,
    TDE_DRV_COLOR_FMT_YCbCr420MB,
    TDE_DRV_COLOR_FMT_YCbCr444MB,  TDE_DRV_COLOR_FMT_BUTT
};

STATIC TDE_DRV_FILTER_MODE_E g_enTdeFilterMode[TDE2_FILTER_MODE_BUTT + 1] = {
    TDE_DRV_FILTER_COLOR, TDE_DRV_FILTER_ALPHA, TDE_DRV_FILTER_ALL, TDE_DRV_FILTER_NONE
};

#ifndef HI_BUILD_IN_BOOT/**codecc**/
/* if local deflicker flag */
STATIC HI_BOOL s_bRegionDeflicker = HI_FALSE;
#endif
/****************************************************************************/
/*                 TDE osi ctl inner interface definition                   */
/****************************************************************************/
STATIC TDE_COLORFMT_CATEGORY_E   TdeOsiGetFmtCategory(TDE2_COLOR_FMT_E enFmt);

STATIC TDE_COLORFMT_TRANSFORM_E  TdeOsiGetFmtTransType(TDE2_COLOR_FMT_E enSrc2Fmt, TDE2_COLOR_FMT_E enDstFmt);

/* AI7D02649 beg */
STATIC INLINE HI_S32             TdeOsiSetClutOpt(TDE2_SURFACE_S * pClutSur, TDE2_SURFACE_S * pOutSur, TDE_CLUT_USAGE_E* penClutUsage,
                                                  HI_BOOL bClutReload, TDE_HWNode_S* pstHWNode);

STATIC INLINE TDE_CLUT_USAGE_E   TdeOsiGetClutUsage(TDE2_COLOR_FMT_E enSrcFmt, TDE2_COLOR_FMT_E enDstFmt);
/* AI7D02649 end */

STATIC HI_S32                    TdeOsiGetScanInfo(TDE2_SURFACE_S *pSrc, TDE2_RECT_S *pstSrcRect,
                                                   TDE2_SURFACE_S *pstDst,
                                                   TDE2_RECT_S *pstDstRect, TDE2_OPT_S* pstOpt,
                                                   TDE_SCANDIRECTION_S *pstSrcDirection,
                                                   TDE_SCANDIRECTION_S *pstDstDirection);

STATIC HI_S32                    TdeOsiGetInterRect(TDE2_RECT_S *pRect1, TDE2_RECT_S *pRect2, TDE2_RECT_S *pInterRect);

STATIC INLINE HI_S32             TdeOsiSetMbPara(TDE_HANDLE s32Handle,
                                                 TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect,
                                                 TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                                 TDE2_MBOPT_S* pMbOpt);

STATIC INLINE HI_S32             TdeOsiAdjQuality(TDE_DRV_SURFACE_S* pS1Tmp, TDE_DRV_SURFACE_S* pS2Tmp,
                                                  TDE2_MB_S* pstMB, TDE2_RECT_S* pstMbRect,
                                                  TDE2_SURFACE_S* pstDst, TDE2_RECT_S* pstDstRect,
                                                  TDE_PIC_MODE_E enPicMode, TDE2_MBOPT_S* pMbOpt);

STATIC INLINE HI_U32             TdeOsiMbCalcHStep(HI_U32 u32Wi, HI_U32 u32Wo,
                                                   TDE2_MB_COLOR_FMT_E enInFmt, TDE2_MB_COLOR_FMT_E enOutFmt, HI_BOOL bCbCr);

STATIC INLINE HI_U32             TdeOsiMbCalcVStep(HI_U32 u32Hi, HI_U32 u32Ho,
                                                   TDE2_MB_COLOR_FMT_E enInFmt, TDE2_MB_COLOR_FMT_E enOutFmt, HI_BOOL bCbCr);

#ifndef HI_BUILD_IN_BOOT/**codecc**/
STATIC INLINE HI_S32             TdeOsiMbCalcHOffset(HI_U32 u32Xi, TDE2_MB_COLOR_FMT_E enFmt, HI_BOOL bCbCr);

STATIC INLINE HI_S32              TdeOsiMbCalcVOffset(HI_U32 u32Yi, TDE2_MB_COLOR_FMT_E enFmt,
                                                    TDE_PIC_MODE_E enPicMode,HI_BOOL bCbCr);
#endif

STATIC INLINE HI_U32             TdeOsiCalcNoLapSlice(TDE_SLICE_INFO_S *pSliceInfo, HI_U32 u32MaxNum, HI_U32 u32Xi,
                                                      HI_U32 u32Xo, HI_U32 u32BmpW, /*AI7D02711*/
                                                      TDE_FILTER_OPT* pstFilterOpt);

STATIC INLINE HI_U32             TdeOsiCalcOverLapSlice(TDE_SLICE_INFO_S *pSliceInfo, HI_U32 u32MaxNum, HI_U32 u32BmpXi, HI_U32 u32BmpWi,
                                                        HI_U32 u32BmpXo, HI_U32 u32BmpWo, /*AI7D02711*/
                                                        TDE_FILTER_OPT* pstFilterOpt);

STATIC HI_S32                    TdeOsiSetFilterChildNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode,
                                                          TDE2_RECT_S* pInRect,
                                                          TDE2_RECT_S*  pOutRect,
                                                          TDE2_DEFLICKER_MODE_E enDeflickerMode,
                                                          TDE_FILTER_OPT* pstFilterOpt);

STATIC HI_S32                    TdeOsiSetFilterNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode,
                                                     TDE2_SURFACE_S *pstForeGround, TDE2_SURFACE_S *pstDst,
                                                     TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect,
                                                     TDE2_DEFLICKER_MODE_E enDeflickerMode, TDE_FILTER_OPT *pstFilterOpt);

STATIC HI_S32                    TdeOsi1SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstDst,
                                                   TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                                                   TDE2_OPT_S *pstOpt);

#ifndef HI_BUILD_IN_BOOT
STATIC HI_S32 TdeOsiSingleSrc2Blit(TDE_HANDLE s32Handle,TDE2_SURFACE_S* pstForeGround,TDE2_RECT_S  *pstForeGroundRect,TDE2_SURFACE_S* pstDst,
                                                                TDE2_RECT_S  *pstDstRect,TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst);

STATIC HI_S32                    TdeOsi2SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc,
                                                   TDE2_RECT_S  *pstSrcRect,
                                                   TDE2_SURFACE_S *pstDst,
                                                   TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                                                   TDE2_OPT_S *pstOpt);

#endif

STATIC INLINE HI_S32             TdeOsiCheckSurface(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S  *pstRect);

STATIC TDE_OPERATION_CATEGORY_E  TdeOsiGetOptCategory(TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S *pstBackGroundRect,
                                                      TDE2_SURFACE_S * pstForeGround, TDE2_RECT_S *pstForeGroundRect,
                                                      TDE2_SURFACE_S *pstDst,
                                                      TDE2_RECT_S *pstDstRect,
                                                      TDE2_OPT_S * pstOpt);

STATIC INLINE HI_VOID            TdeOsiConvertSurface(TDE2_SURFACE_S *pstSur, TDE2_RECT_S *pstRect,
                                                      TDE_SCANDIRECTION_S *pstScanInfo,
                                                      TDE_DRV_SURFACE_S *pstDrvSur,
                                                      TDE2_RECT_S *pstOperationArea);

STATIC HI_S32                    TdeOsiConvertMbSurface(TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect,
                                                        TDE_DRV_SURFACE_S *pstDrvY,
                                                        TDE_DRV_SURFACE_S *pstDrvCbCr,
                                                         TDE_PIC_MODE_E enPicMode, HI_BOOL bCbCr2Opt);

STATIC INLINE HI_VOID            TdeOsiCalcMbFilterOpt(TDE_FILTER_OPT* pstFilterOpt, TDE2_MB_COLOR_FMT_E enInFmt,
                                                       TDE2_MB_COLOR_FMT_E enOutFmt,
                                                       TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect, HI_BOOL bCbCr,
                                                       HI_BOOL bCus,TDE_PIC_MODE_E enPicMode);

STATIC HI_S32                   TdeOsiSetClipPara(TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S *pstBGRect,
                                                   TDE2_SURFACE_S * pstForeGround,
                                                   TDE2_RECT_S *pstFGRect, TDE2_SURFACE_S * pstDst,
                                                   TDE2_RECT_S *pstDstRect,
                                                   TDE2_OPT_S * pstOpt,
                                                   TDE_HWNode_S *pstHwNode);

STATIC HI_S32                    TdeOsiMbSetClipPara(TDE2_RECT_S * pstDstRect, TDE2_MBOPT_S *pstMbOpt,
                                                     TDE_HWNode_S *pstHwNode);

STATIC HI_S32                   TdeOsiSetBaseOptParaForBlit(TDE2_OPT_S * pstOpt, TDE2_SURFACE_S* pstSrc1, TDE2_SURFACE_S* pstSrc2,
                                                             TDE_OPERATION_CATEGORY_E enOptCategory, TDE_HWNode_S *pstHwNode);

STATIC        HI_S32             TdeOsiAdjClipPara(TDE_HWNode_S* pHWNode);
STATIC INLINE HI_S32            TdeOsiSetDeflickerPara(TDE_HWNode_S* pstHwNode, TDE2_DEFLICKER_MODE_E enDeflickerMode, TDE_FILTER_OPT* pstFilterOpt);

STATIC HI_S32                    TdeOsiSetNodeFinish(TDE_HANDLE s32Handle, TDE_HWNode_S* pHWNode,
                                                     HI_U32 u32WorkBufNum, TDE_NODE_SUBM_TYPE_E enSubmType);

STATIC INLINE HI_S32             TdeOsiCheckResizePara(HI_U32 u32InWidth, HI_U32 u32InHeight,
                                                       HI_U32 u32OutWidth, HI_U32 u32OutHeight);
STATIC INLINE HI_BOOL            TdeOsiWhetherContainAlpha(TDE2_COLOR_FMT_E enColorFmt);
STATIC INLINE HI_VOID            TdeOsiSetFilterOptAdjInfo(TDE_DRV_SURFACE_S *pInSur, TDE_DRV_SURFACE_S* pOutSur,
                                                           TDE_FILTER_OPT* pstFilterOpt, TDE_CHILD_SCALE_MODE_E enScaleMode);
STATIC INLINE HI_VOID            TdeOsiSetExtAlpha(TDE2_SURFACE_S *pstBackGround, TDE2_SURFACE_S *pstForeGround,
                                                   TDE_HWNode_S *pstHwNode);


/****************************************************************************/
/*                      TDE osi ctl inner interface realization				*/
/****************************************************************************/

/*  Y  =  (263 * R + 516 * G + 100 * B) >> 10 + 16 */
/*  Cb =  (-152 * R - 298 * G + 450 * B) >> 10  + 128 */
/*  Cr =  (450 * R - 377 * G - 73 * B) >> 10  + 128 */
STATIC HI_VOID rgb2ycc(HI_U8 r, HI_U8 g, HI_U8 b, HI_U8 *y, HI_U8 *cb, HI_U8 *cr)
{
    *y = ((((263 * r + 516 * g + 100 * b) >> 9) + 1) >> 1) + 16;
    *cb = ((((-152 * r - 298 * g+ 450 * b) >> 9) + 1) >> 1)  + 128;
    *cr = ((((450 * r - 377 * g - 73 * b) >> 9) + 1) >> 1)  + 128;

    return ;
}

/*  R  =  ((298 * (Y-16) + 0 * (Cb-128) + 409 * (Cr-128)) >> 8 */
/*  G =  ((298 * (Y-16) - 100 * (Cb-128) - 208 * (Cr-128)) >> 8 */
/*  B = ((298 * (Y-16) + 517 * (Cb-128) - 0 * (Cr-128)) >> 8 */
STATIC HI_VOID ycc2rgb(HI_U8 y, HI_U8 cb, HI_U8 cr, HI_U8 *r, HI_U8 *g, HI_U8 *b)
{
    *r = (((298 *(y -16) + 409 * (cr - 128)) >> 7) + 1) >> 1;
    *g = (((298 * (y - 16) - 100 * (cb - 128) - 208 * (cr - 128)) >> 7) + 1) >> 1;
    *b = (((298 *(y -16) + 517 * (cb - 128)) >> 7) + 1) >> 1;

    return ;
}



STATIC TDE2_FMT_BITOFFSET_S s_u8FmtBitAndOffsetArray[TDE2_COLOR_FMT_AYCbCr8888 + 1] =
{
    {0, 4, 4, 4, 12, 8, 4, 0},      /* TDE2_COLOR_FMT_RGB444 */
    {0, 4, 4, 4, 12, 0, 4, 8},      /* TDE2_COLOR_FMT_BGR444 */
    {0, 5, 5, 5, 15, 10, 5, 0},     /* TDE2_COLOR_FMT_RGB555 */
    {0, 5, 5, 5, 15, 0, 5, 10},     /* TDE2_COLOR_FMT_BGR555 */
    {0, 5, 6, 5, 16, 11, 5, 0},     /* TDE2_COLOR_FMT_RGB565 */
    {0, 5, 6, 6, 16, 0, 5, 11},     /* TDE2_COLOR_FMT_BGR565 */
    {0, 8, 8, 8, 24, 16, 8, 0},     /* TDE2_COLOR_FMT_RGB888 */
    {0, 8, 8, 8, 24, 0, 8, 16},     /* TDE2_COLOR_FMT_BGR888 */
    {4, 4, 4, 4, 12, 8, 4, 0},      /* TDE2_COLOR_FMT_ARGB4444 */
    {4, 4, 4, 4, 12, 0, 4, 8},      /* TDE2_COLOR_FMT_ABGR4444 */
    {4, 4, 4, 4, 0, 12, 8, 4},      /* TDE2_COLOR_FMT_RGBA4444 */
    {4, 4, 4, 4, 0, 4, 8, 12},      /* TDE2_COLOR_FMT_BGRA4444 */
    {1, 5, 5, 5, 15, 10, 5, 0},     /* TDE2_COLOR_FMT_ARGB1555 */
    {1, 5, 5, 5, 15, 0, 5, 10},     /* TDE2_COLOR_FMT_ABGR1555 */
    {1, 5, 5, 5, 0, 11, 6, 1},      /* TDE2_COLOR_FMT_RGBA1555 */
    {1, 5, 5, 5, 0, 1, 6, 11},      /* TDE2_COLOR_FMT_BGRA1555 */
    {8, 5, 6, 5, 16, 11, 5, 0},     /* TDE2_COLOR_FMT_ARGB8565 */
    {8, 5, 6, 5, 16, 0, 5, 11},     /* TDE2_COLOR_FMT_ABGR8565 */
    {8, 5, 6, 5, 0, 19, 13, 8},     /* TDE2_COLOR_FMT_RGBA8565 */
    {8, 5, 6, 6, 0, 8, 13, 19},     /* TDE2_COLOR_FMT_BGRA8565 */
    {8, 8, 8, 8, 24, 16, 8, 0},     /* TDE2_COLOR_FMT_ARGB8888 */
    {8, 8, 8, 8, 24, 0, 8, 16},     /* TDE2_COLOR_FMT_ABGR8888 */
    {8, 8, 8, 8, 0, 24, 16, 8},     /* TDE2_COLOR_FMT_RGBA8888 */
    {8, 8, 8, 8, 0, 8, 16, 24},     /* TDE2_COLOR_FMT_BGRA8888 */
    {8, 8, 8, 8, 16, 24, 0, 8},     /* TDE2_COLOR_FMT_RABG8888 */

    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 8, 8, 8, 24, 16, 8, 0},     /* TDE2_COLOR_FMT_YCbCr888 */
    {8, 8, 8, 8, 24, 16, 8, 0},     /* TDE2_COLOR_FMT_AYCbCr8888 */
};

/*****************************************************************************
 Prototype       : TdeOsiColorConvert
 Description     : translate other RGB  pixel format to ARGB8888's
 Input           : enColorFmt    **
                   u32InColor    **
                   pu32OutColor  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/21
    Author       : wming
    Modification : Created function

*****************************************************************************/
STATIC HI_S32 TdeOsiColorConvert(TDE2_FILLCOLOR_S *pstFillColor, TDE2_SURFACE_S *pstSur, HI_U32 *pu32OutColor)
{
    HI_U8 a, r, g, b, y, cb, cr;
    TDE_COLORFMT_TRANSFORM_E enColorTrans;

    TDE_ASSERT(NULL != pstFillColor);
    TDE_ASSERT(NULL != pstSur);
    TDE_ASSERT(NULL != pu32OutColor);
    enColorTrans = TdeOsiGetFmtTransType(pstFillColor->enColorFmt, pstSur->enColorFmt);

    if (((TDE2_COLOR_FMT_CLUT1 <= pstFillColor->enColorFmt)
        && (pstFillColor->enColorFmt <= TDE2_COLOR_FMT_A8))
        || (pstFillColor->enColorFmt >= TDE2_COLOR_FMT_YCbCr422))
    {
        TDE_TRACE(TDE_KERN_INFO, "Unsupported color!\n");
        return -1;
    }

    a = (pstFillColor->u32FillColor >> s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8AlphaOffset)
            & (0xff >> (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8AlphaBits));
    r = (pstFillColor->u32FillColor >> s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8RedOffset)
            & (0xff >> (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8RedBits));
    g = (pstFillColor->u32FillColor >> s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8GreenOffset)
            & (0xff >> (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8GreenBits));
    b = (pstFillColor->u32FillColor >> s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8BlueOffset)
            & (0xff >> (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8BlueBits));

    if ((TDE2_COLOR_FMT_ARGB1555 <= pstFillColor->enColorFmt)
        && (pstFillColor->enColorFmt <= TDE2_COLOR_FMT_BGRA1555))
    {
        if(a)
        {
            a = pstSur->u8Alpha1;
        }
        else
        {
            a = pstSur->u8Alpha0;
        }
    }
    else
    {
        a = a << (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8AlphaBits);
    }

    r = r << (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8RedBits);
    g = g << (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8GreenBits);
    b = b << (8 - s_u8FmtBitAndOffsetArray[pstFillColor->enColorFmt].u8BlueBits);

    switch(enColorTrans)
    {
        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
        case TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr:
        {
            *pu32OutColor = (a << 24) + (r << 16) + (g << 8) + b;
            return 0;
        }

        case TDE_COLORFMT_TRANSFORM_ARGB_YCbCr:
        {
            rgb2ycc(r, g, b, &y, &cb, &cr);
            *pu32OutColor = (a << 24) + (y << 16) + (cb << 8) + cr;
            return 0;
        }

        case TDE_COLORFMT_TRANSFORM_YCbCr_ARGB:
        {
            ycc2rgb(r, g, b, &y, &cb, &cr);
            *pu32OutColor = (a << 24) + (y << 16) + (cb << 8) + cr;
            return 0;
        }
        default:
            TDE_TRACE(TDE_KERN_INFO, "Unsupported color transport!\n");
            return -1;
    }
}

/*****************************************************************************
* Function:      TdeOsiCheckResizePara
* Description:   check zoom ratio limit
* Input:         pstInRect: rect before zoom
*                pstOutRect: rect after zoom
* Output:        none
* Return:        TDE_COLORFMT_CATEGORY_E   pixel format category
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiCheckResizePara(HI_U32 u32InWidth, HI_U32 u32InHeight,
                                           HI_U32 u32OutWidth, HI_U32 u32OutHeight)
{
    if ((TDE_MAX_RECT_WIDTH < u32InWidth)\
        || (TDE_MAX_RECT_HEIGHT < u32InHeight)||(TDE_MAX_RECT_WIDTH < u32OutWidth)\
        || (TDE_MAX_RECT_HEIGHT < u32OutHeight))
    {
        if((u32InWidth != u32OutWidth)
        ||(u32InHeight!= u32OutHeight))
        {
            TDE_TRACE(TDE_KERN_ERR, "Can not support resize!\n");
            return HI_ERR_TDE_INVALID_PARA;
        }
    }

    if (((u32OutWidth * TDE_MAX_MINIFICATION_H) < u32InWidth)
        || ((u32OutHeight * TDE_MAX_MINIFICATION_V) < u32InHeight))
    {
        TDE_TRACE(TDE_KERN_INFO, "Resize parameter error!\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************
* Function:      TdeOsiGetFmtCategory
* Description:   get pixel format category info
* Input:         enFmt: pixel format
* Output:        none
* Return:        TDE_COLORFMT_CATEGORY_E  pixel format category
* Others:        none
*****************************************************************************/
STATIC TDE_COLORFMT_CATEGORY_E TdeOsiGetFmtCategory(TDE2_COLOR_FMT_E enFmt)
{
    /* target is ARGB format */
    if (enFmt <= TDE2_COLOR_FMT_RABG8888)
    {
        return TDE_COLORFMT_CATEGORY_ARGB;
    }
    /* target is CLUT table format */
    else if (enFmt <= TDE2_COLOR_FMT_ACLUT88)
    {
        return TDE_COLORFMT_CATEGORY_CLUT;
    }
    /* target is alpha CLUT table format */
    else if (enFmt <= TDE2_COLOR_FMT_A8)
    {
        return TDE_COLORFMT_CATEGORY_An;
    }
    /* target is YCbCr format */
    else if (enFmt <= TDE2_COLOR_FMT_YCbCr422)
    {
        return TDE_COLORFMT_CATEGORY_YCbCr;
    }
    /* byte format */
    else if (enFmt == TDE2_COLOR_FMT_byte)
    {
        return TDE_COLORFMT_CATEGORY_BYTE;
    }
    /* halfword  format */
    else if (enFmt == TDE2_COLOR_FMT_halfword)
    {
        return TDE_COLORFMT_CATEGORY_HALFWORD;
    }
    else if (enFmt <= TDE2_COLOR_FMT_JPG_YCbCr444MBP)
    {
        return TDE_COLORFMT_CATEGORY_YCbCr;
    }
    /* error format */
    else
    {
        return TDE_COLORFMT_CATEGORY_BUTT;
    }
}

/*****************************************************************************
* Function:      TdeOsiGetFmtTransType
* Description:   get pixel format transform type
* Input:         enSrc2Fmt: foreground pixel format
                 enDstFmt: target pixel format
* Output:        none
* Return:        TDE_COLORFMT_TRANSFORM_E pixel format transform type
* Others:        none
*****************************************************************************/
STATIC TDE_COLORFMT_TRANSFORM_E TdeOsiGetFmtTransType(TDE2_COLOR_FMT_E enSrc2Fmt, TDE2_COLOR_FMT_E enDstFmt)
{
    TDE_COLORFMT_CATEGORY_E enSrcCategory;
    TDE_COLORFMT_CATEGORY_E enDstCategory;

    /* get foreground pixel format category */
    enSrcCategory = TdeOsiGetFmtCategory(enSrc2Fmt);

    /* get target pixel format category*/
    enDstCategory = TdeOsiGetFmtCategory(enDstFmt);

    switch (enSrcCategory)
    {
	    case TDE_COLORFMT_CATEGORY_ARGB:
	    {
	        if (TDE_COLORFMT_CATEGORY_ARGB == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_ARGB_ARGB;
	        }

	        if (TDE_COLORFMT_CATEGORY_YCbCr == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_ARGB_YCbCr;
	        }

	        if (TDE_COLORFMT_CATEGORY_An == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_ARGB_An;
	        }

	        return TDE_COLORFMT_TRANSFORM_BUTT;
	    }

	    case TDE_COLORFMT_CATEGORY_CLUT:
	    {
	        if (TDE_COLORFMT_CATEGORY_ARGB == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_CLUT_ARGB;
	        }

	        if (TDE_COLORFMT_CATEGORY_YCbCr == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_CLUT_YCbCr;
	        }

	        if (TDE_COLORFMT_CATEGORY_CLUT == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_CLUT_CLUT;
	        }

	        return TDE_COLORFMT_TRANSFORM_BUTT;
	    }

	    case TDE_COLORFMT_CATEGORY_YCbCr:
	    {
	        if (TDE_COLORFMT_CATEGORY_ARGB == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_YCbCr_ARGB;
	        }

	        if (TDE_COLORFMT_CATEGORY_YCbCr == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr;
	        }

	        if (TDE_COLORFMT_CATEGORY_An == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_ARGB_An;
	        }

	        return TDE_COLORFMT_TRANSFORM_BUTT;
	    }

	    case TDE_COLORFMT_CATEGORY_An:
	    {
	        if (TDE_COLORFMT_CATEGORY_An == enDstCategory)
	        {
	            return TDE_COLORFMT_TRANSFORM_An_An;
	        }

	        return TDE_COLORFMT_TRANSFORM_BUTT;
	    }

	    default:
	        return TDE_COLORFMT_TRANSFORM_BUTT;
    }
}
#ifndef HI_BUILD_IN_BOOT
static HI_S32 TdeOsiSetBlend(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , TDE2_BLEND_OPT_S stBlendOpt , TDE_DRV_ALU_MODE_E* enAluMode, HI_BOOL bCheckBlend)
{
    HI_BOOL bSetBlend = (enAluCmd & TDE2_ALUCMD_BLEND) ? HI_TRUE : HI_FALSE;
    HI_BOOL bUnknownBlendMode = (stBlendOpt.eBlendCmd == TDE2_BLENDCMD_CONFIG)\
                                && \
                                ((stBlendOpt.eSrc1BlendMode >= TDE2_BLEND_BUTT) || \
                                 (stBlendOpt.eSrc2BlendMode >= TDE2_BLEND_BUTT));

    HI_BOOL bEnableAlphaRop = (enAluCmd & TDE2_ALUCMD_ROP) ? HI_TRUE : HI_FALSE;

    HI_BOOL BUnknownBlendCmd = (stBlendOpt.eBlendCmd >= TDE2_BLENDCMD_BUTT);

    if (!bSetBlend)
    {
        return HI_SUCCESS;
    }

    *enAluMode = TDE_ALU_BLEND;

    if (bCheckBlend)
    {
        if (BUnknownBlendCmd)
        {
            TDE_TRACE(TDE_KERN_INFO, "Unknown blend cmd!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }


        if (bUnknownBlendMode)
        {
            TDE_TRACE(TDE_KERN_INFO, "Unknown blend mode!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }
    }

   if (TdeHalNodeSetBlend(pstHWNode, &stBlendOpt) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    if (bEnableAlphaRop)
    {
        TdeHalNodeEnableAlphaRop(pstHWNode);
    }

    return HI_SUCCESS;
}


static HI_S32 TdeOsiSetColorize(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , HI_S32 u32Colorize)
{
    HI_BOOL bSetColorize = (enAluCmd & TDE2_ALUCMD_COLORIZE) ? HI_TRUE : HI_FALSE;

    if (!bSetColorize)
    {
        return HI_SUCCESS;
    }

    if (TdeHalNodeSetColorize(pstHWNode, u32Colorize) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiIsSingleSrc2Rop
* Description:   query if ROP operate is if single source2 operate
* Input:         enRop: rop operate type
* Output:        none
* Return:        HI_TRUE: single ROP;HI_FALSE: non single ROP
* Others:        none
*****************************************************************************/
STATIC INLINE HI_BOOL TdeOsiIsSingleSrc2Rop(TDE2_ROP_CODE_E enRop)
{
    switch (enRop)
    {
	    case TDE2_ROP_BLACK:
	    case TDE2_ROP_NOTCOPYPEN:
	    case TDE2_ROP_COPYPEN:
	    case TDE2_ROP_WHITE:
	        return HI_TRUE;

	    default:
	        return HI_FALSE;
    }
}

static HI_S32 TdeOsiSetRop(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , TDE2_ROP_CODE_E enRopCode_Color ,
                                    TDE2_ROP_CODE_E enRopCode_Alpha , TDE_DRV_ALU_MODE_E* enAluMode , HI_BOOL SingleSr2Rop)
{
    HI_BOOL bSetRop = (enAluCmd & TDE2_ALUCMD_ROP) ? HI_TRUE : HI_FALSE;
    HI_BOOL bErrorRopCode = (TDE2_ROP_BUTT <= enRopCode_Color) || (TDE2_ROP_BUTT <= enRopCode_Alpha);
    HI_BOOL bOnlySupportSingleSr2Rop = ((!TdeOsiIsSingleSrc2Rop(enRopCode_Alpha))\
                                        || (!TdeOsiIsSingleSrc2Rop(enRopCode_Color)));

    if (!bSetRop)
    {
        return HI_SUCCESS;
    }

    *enAluMode = TDE_ALU_ROP;

    if (bErrorRopCode)
    {
        TDE_TRACE(TDE_KERN_INFO, "enRopCode error!\n");
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (SingleSr2Rop)
    {
        if (bOnlySupportSingleSr2Rop)
        {
            TDE_TRACE(TDE_KERN_INFO, "Only support single s2 rop!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return -1;
        }
    }

    if (TdeHalNodeSetRop(pstHWNode, enRopCode_Color, enRopCode_Alpha) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return HI_SUCCESS;
}



#endif

/*****************************************************************************
* Function:      TdeOsiGetClutUsage
* Description:   get CLUT table usage
* Input:         enSrc2Fmt foreground pixel format
                 enDstFmt  target pixel format
* Output:        none
* Return:        TDE_CLUT_USAGE_E:  clut  usage
* Others:        none
*****************************************************************************/
STATIC INLINE TDE_CLUT_USAGE_E TdeOsiGetClutUsage(TDE2_COLOR_FMT_E enSrcFmt, TDE2_COLOR_FMT_E enDstFmt)
{
    TDE_COLORFMT_TRANSFORM_E enColorTransType;

    enColorTransType = TdeOsiGetFmtTransType(enSrcFmt, enDstFmt);

    switch (enColorTransType)
    {
	    case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
	    case TDE_COLORFMT_TRANSFORM_CLUT_YCbCr:
	        return TDE_CLUT_COLOREXPENDING;         /* color expand */

	    case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
	    case TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr:
	        return TDE_CLUT_COLORCORRECT;           /* color adjust */

	    case TDE_COLORFMT_TRANSFORM_CLUT_CLUT:
	        return TDE_CLUT_CLUT_BYPASS;

	    default:
	        return TDE_CLUT_USAGE_BUTT;
    }
}

STATIC INLINE HI_BOOL TdeOsiWhetherContainAlpha(TDE2_COLOR_FMT_E enColorFmt)
{
    switch (enColorFmt)
    {
        case TDE2_COLOR_FMT_ARGB4444:
        case TDE2_COLOR_FMT_ABGR4444:
        case TDE2_COLOR_FMT_RGBA4444:
        case TDE2_COLOR_FMT_BGRA4444:
        case TDE2_COLOR_FMT_ARGB1555:
        case TDE2_COLOR_FMT_ABGR1555:
        case TDE2_COLOR_FMT_RGBA1555:
        case TDE2_COLOR_FMT_BGRA1555:
        case TDE2_COLOR_FMT_ARGB8565:
        case TDE2_COLOR_FMT_ABGR8565:
        case TDE2_COLOR_FMT_RGBA8565:
        case TDE2_COLOR_FMT_BGRA8565:
        case TDE2_COLOR_FMT_ARGB8888:
        case TDE2_COLOR_FMT_ABGR8888:
        case TDE2_COLOR_FMT_RGBA8888:
        case TDE2_COLOR_FMT_BGRA8888:
        case TDE2_COLOR_FMT_AYCbCr8888:
        case TDE2_COLOR_FMT_RABG8888:
            return HI_TRUE;
        default:
            return HI_FALSE;
    }
}

#define TDE_CLUT_SIZE (256 * 4)
/*****************************************************************************
* Function:      TdeOsiSetClutOpt
* Description:   set clut parameter
* Input:         pClutSur Clut bitmap info
*                penClutUsage  return clut usage type: expand/adjust
*                bClutReload   if reload clut
*                pstHWNode     hardware node information
* Output:        none
* Return:        TDE_CLUT_USAGE_E: clut usage
* Others:        modify AI7D02649 add
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiSetClutOpt(TDE2_SURFACE_S * pClutSur, TDE2_SURFACE_S * pOutSur, TDE_CLUT_USAGE_E* penClutUsage,
                                      HI_BOOL bClutReload, TDE_HWNode_S* pstHWNode)
{
    if (((HI_U8*)-1 != pClutSur->pu8ClutPhyAddr)&&(HI_NULL != pClutSur->pu8ClutPhyAddr))
    {
        TDE_COLORFMT_CATEGORY_E enFmtCate;
#ifdef CONFIG_GFX_MMU_SUPPORT
		HI_U8 *pMmzClut = HI_NULL;
		HI_U8 *pMmuClut = HI_NULL;
#ifdef TDE_CACH_STRATEGY
		GFX_MMZ_BUFFER_S stFlushBuf = {0};
#endif
#endif
		HI_SIZE_T clutPhyaddr = (HI_SIZE_T)pClutSur->pu8ClutPhyAddr;

        enFmtCate = TdeOsiGetFmtCategory(pOutSur->enColorFmt);

        /* when user input the type of clut is not consistent with output format,rerurn error */
        if ( (!pClutSur->bYCbCrClut && TDE_COLORFMT_CATEGORY_YCbCr == enFmtCate)
             || (pClutSur->bYCbCrClut && TDE_COLORFMT_CATEGORY_ARGB == enFmtCate) )
        {
            TDE_TRACE(TDE_KERN_INFO, "clut fmt not same\n");
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        /* Because of unsupport output CSC, input and background color zone is consistent.
			In trine sources operation,clut need use background, so output color zone instand of background's */
        *penClutUsage = TdeOsiGetClutUsage(pClutSur->enColorFmt, pOutSur->enColorFmt);/*AI7D02757*/
        if (TDE_CLUT_CLUT_BYPASS > *penClutUsage)
        {
            TDE_DRV_CLUT_CMD_S stClutCmd;
            if (TDE_CLUT_COLOREXPENDING == *penClutUsage)
            {
                stClutCmd.enClutMode = TDE_COLOR_EXP_CLUT_MODE;
            }
            else
            {
                stClutCmd.enClutMode = TDE_COLOR_CORRCT_CLUT_MODE;
            }

            if(clutPhyaddr % 4)
            {
                TDE_TRACE(TDE_KERN_INFO, "pClutSur->pu8ClutPhyAddr=%p\n", pClutSur->pu8ClutPhyAddr);
                return HI_ERR_TDE_NOT_ALIGNED;
            }

#ifdef CONFIG_GFX_MMU_SUPPORT
			pMmuClut = HI_GFX_Map((HI_U32)clutPhyaddr, TDE_CLUT_SIZE, HI_TRUE);
			if (HI_NULL == pMmuClut)
			{
				TDE_TRACE(TDE_KERN_INFO, "Map clut failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			pMmzClut = TDE_MALLOC(TDE_CLUT_SIZE);
			if (HI_NULL == pMmzClut)
			{
				TDE_TRACE(TDE_KERN_INFO, "Alloc clut failed!\n");
				return HI_ERR_TDE_NO_MEM;
			}

			HI_GFX_Memcpy(pMmzClut, pMmuClut, TDE_CLUT_SIZE);

			HI_GFX_Unmap(pMmuClut, HI_TRUE);

			stClutCmd.u32PhyClutAddr = wgetphy((HI_VOID *)pMmzClut);

#ifdef TDE_CACH_STRATEGY
			stFlushBuf.u32Size = TDE_CLUT_SIZE;
			stFlushBuf.u32StartPhyAddr = stClutCmd.u32PhyClutAddr;
			stFlushBuf.pu8StartVirAddr = pMmzClut;

			HI_GFX_Flush(&stFlushBuf,HI_FALSE);
#endif

#else
            stClutCmd.u32PhyClutAddr = (HI_U32)((HI_ULONG)pClutSur->pu8ClutPhyAddr);
#endif
            if(TdeHalNodeSetClutOpt(pstHWNode, &stClutCmd, bClutReload)<0)
            {
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }
        }
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiSetFilterOptAdjInfo
* Description:   Set adjust info when each block operate in MB bitmap
* Input:         pInSur:  input brightness info
*                pOutSur: output bitmap info
*                pstFilterOpt:    filter info
*                enScaleMode:     child node zoom node
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_VOID TdeOsiSetFilterOptAdjInfo(TDE_DRV_SURFACE_S *pInSur, TDE_DRV_SURFACE_S* pOutSur,
                                                TDE_FILTER_OPT* pstFilterOpt, TDE_CHILD_SCALE_MODE_E enScaleMode)
{
    pstFilterOpt->stAdjInfo.enScaleMode = enScaleMode;
    pstFilterOpt->stAdjInfo.u32StartInX = pInSur->u32Xpos;
    pstFilterOpt->stAdjInfo.u32StartInY = pInSur->u32Ypos;
    pstFilterOpt->stAdjInfo.u32StartOutX = pOutSur->u32Xpos;
    pstFilterOpt->stAdjInfo.u32StartOutY = pOutSur->u32Ypos;
    pstFilterOpt->stAdjInfo.enFmt = pInSur->enColorFmt;

    if ((TDE_CHILD_SCALE_MB_CONCA_L == enScaleMode)
        && (TDE_DRV_COLOR_FMT_YCbCr420MB == pInSur->enColorFmt
            || TDE_DRV_COLOR_FMT_YCbCr422MBH == pInSur->enColorFmt))
    {
        pstFilterOpt->bEvenStartInX = HI_TRUE;
        pstFilterOpt->bBadLastPix = HI_TRUE;
    }

    if (TDE_DRV_COLOR_FMT_YCbCr422 == pOutSur->enColorFmt)
    {
        pstFilterOpt->bEvenStartOutX = HI_TRUE;
    }
}

#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeOsiCheckSingleSrc2Opt
* Description:   check if valid of foreground single source operate
* Input:         enSrc2Fmt foreground pixel format
                 enDstFmt  target pixel format
                 pstOpt     operate attribute pointer
* Output:        none
* Return:        0  valid parameter;
                 -1 invalid parameter;
* Others:        none
*****************************************************************************/
STATIC HI_S32  TdeOsiCheckSingleSrc2Opt(TDE2_COLOR_FMT_E enSrc2Fmt, TDE2_COLOR_FMT_E enDstFmt,
                                        TDE2_OPT_S* pstOpt)
{
    TDE_COLORFMT_TRANSFORM_E enColorTransType;

    /* return error, if enable color key */
    if (TDE2_COLORKEY_MODE_NONE != pstOpt->enColorKeyMode)
    {
        TDE_TRACE(TDE_KERN_INFO, "It doesn't support colorkey in single source mode!\n");
        return -1;
    }

    TDE_CHECK_ALUCMD(pstOpt->enAluCmd);

    /* if operate type is ROP and it is not single operate,return error */
    if (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
    {
        TDE_CHECK_ROPCODE(pstOpt->enRopCode_Alpha);
        TDE_CHECK_ROPCODE(pstOpt->enRopCode_Color);
        if ((!TdeOsiIsSingleSrc2Rop(pstOpt->enRopCode_Alpha))
            || (!TdeOsiIsSingleSrc2Rop(pstOpt->enRopCode_Color)))
        {
            TDE_TRACE(TDE_KERN_INFO, "Only support single s2 rop!\n");
            return -1;
        }
    }

    /* single source can not do blend operate */
    if(pstOpt->enAluCmd & TDE2_ALUCMD_BLEND)
    {
        TDE_TRACE(TDE_KERN_INFO, "Alu mode error!\n");
        return -1;
    }

    enColorTransType = TdeOsiGetFmtTransType(enSrc2Fmt, enDstFmt);
    if(TDE_COLORFMT_TRANSFORM_BUTT == enColorTransType)
    {
        TDE_TRACE(TDE_KERN_INFO, "Unkown color transport type!\n");
        return -1;
    }

    if(TDE_COLORFMT_TRANSFORM_CLUT_CLUT == enColorTransType)
    {
        /* unsupport deflicker,zoom, Rop, mirror,colorize */
        if ((pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE)
            || (pstOpt->bResize)
            || (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
            || (pstOpt->enAluCmd & TDE2_ALUCMD_COLORIZE)
            /*|| (TDE2_MIRROR_NONE != pstOpt->enMirror)*/) /* AI7D02892 undo mirror */
        {
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support deflicker or ROP or mirror!\n");
            return -1;
        }
    }

    return 0;
}
#endif

/*****************************************************************************
* Function:      TdeOsiCheckDoubleSrcOpt
* Description:   check if valid of dual source operate
* Input:         enSrc1Fmt background pixel format
                 enSrc2Fmt foreground pixel format
                 enDstFmt  target pixel format
                 pstOpt    operate attribute operate
* Output:        none
* Return:        0  valid parameter;
                 -1 invalid parameter;
* Others:        none
*****************************************************************************/
STATIC HI_S32  TdeOsiCheckDoubleSrcOpt(TDE2_COLOR_FMT_E enSrc1Fmt, TDE2_COLOR_FMT_E enSrc2Fmt,
                                       TDE2_COLOR_FMT_E enDstFmt, TDE2_OPT_S* pstOpt)
{
    TDE_COLORFMT_CATEGORY_E enSrc1Category;
    TDE_COLORFMT_CATEGORY_E enSrc2Category;
    TDE_COLORFMT_CATEGORY_E enDstCategory;
    HI_BOOL bUnknownFmt;


    TDE_CHECK_ALUCMD(pstOpt->enAluCmd);

    /* get background pixel format category */
    enSrc1Category = TdeOsiGetFmtCategory(enSrc1Fmt);

    /* get foreground pixel format category  */
    enSrc2Category = TdeOsiGetFmtCategory(enSrc2Fmt);

    /* get target pixel format category  */
    enDstCategory = TdeOsiGetFmtCategory(enDstFmt);


    bUnknownFmt = ((TDE_COLORFMT_CATEGORY_BYTE <= enSrc1Category)|| (TDE_COLORFMT_CATEGORY_BYTE <= enSrc2Category)|| (TDE_COLORFMT_CATEGORY_BYTE <= enDstCategory));

    if (bUnknownFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "unkown format!\n");
        return -1;
    }


    TDE_BACKGROUNDCATEGORY_CHECK_ARGB(enSrc1Category, enSrc2Category, enDstFmt);

    TDE_BACKGROUNDCATEGORY_CHECK_CLUT(enSrc1Category, enSrc2Category, pstOpt->bResize, pstOpt->enDeflickerMode, \
                                      pstOpt->enAluCmd, enDstCategory);

    TDE_BACKGROUNDCATEGORY_CHECK_AN(enSrc1Category, enSrc2Category, enDstCategory, pstOpt->enAluCmd);


    if (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
    {
        TDE_CHECK_ROPCODE(pstOpt->enRopCode_Alpha);
        TDE_CHECK_ROPCODE(pstOpt->enRopCode_Color);
    }

    if (pstOpt->enAluCmd & TDE2_ALUCMD_BLEND)
    {
        TDE_CHECK_BLENDCMD(pstOpt->stBlendOpt.eBlendCmd);
        if (pstOpt->stBlendOpt.eBlendCmd == TDE2_BLENDCMD_CONFIG)
        {
             TDE_CHECK_BLENDMODE(pstOpt->stBlendOpt.eSrc1BlendMode);
   	      TDE_CHECK_BLENDMODE(pstOpt->stBlendOpt.eSrc2BlendMode);
        }
    }

	return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiDoubleSrcGetOptCode
* Description:   get dual source operate encode
* Input:         enSrc1Fmt background pixel format
                 enSrc2Fmt foreground pixel format
                 enDstFmt: target pixel format
* Output:        none
* Return:        code value
* Others:        none
*****************************************************************************/
STATIC HI_U16 TdeOsiDoubleSrcGetOptCode(TDE2_COLOR_FMT_E enSrc1Fmt, TDE2_COLOR_FMT_E enSrc2Fmt,
                                        TDE2_COLOR_FMT_E enDstFmt)
{
    HI_U16 u16Code1 = 0x0;
    HI_U16 u16Code2 = 0x0;
    TDE_COLORFMT_TRANSFORM_E enColorTransType;

    enColorTransType = TdeOsiGetFmtTransType(enSrc2Fmt, enSrc1Fmt);
    switch (enColorTransType)
    {
	    case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
	        u16Code1 = 0x0;
	        break;

	    case TDE_COLORFMT_TRANSFORM_ARGB_YCbCr:
	        u16Code1 = 0x5;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
	        u16Code1 = 0x8;
	        break;
	    case TDE_COLORFMT_TRANSFORM_CLUT_YCbCr:
		    u16Code1 = 0x8 | 0x10 | 0x1;
		    break;
	    case TDE_COLORFMT_TRANSFORM_YCbCr_ARGB:
	        u16Code1 = 0x1;
	        break;

	    case TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr:
	        u16Code1 = 0x0;
	        break;

	    default:
	        u16Code1 = 0x8000;
	        return u16Code1;
    }

    enColorTransType = TdeOsiGetFmtTransType(enSrc1Fmt, enDstFmt);
    switch (enColorTransType)
    {
	    case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
	        u16Code2 = 0x0;
	        break;

	    case TDE_COLORFMT_TRANSFORM_ARGB_YCbCr:
	        u16Code2 = 0x2;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
	        u16Code2 = 0x8;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_YCbCr:
	        u16Code2 = 0xa;
	        break;

	    case TDE_COLORFMT_TRANSFORM_YCbCr_ARGB:
	        u16Code2 = 0x2 | 0x4;
		 	if (enSrc2Fmt >= TDE2_COLOR_FMT_CLUT1 && enSrc2Fmt <= TDE2_COLOR_FMT_ACLUT88)
	        {
	        	u16Code2 = 0;
	        }
	        break;

	    case TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr:
	        u16Code2 = 0x0;
	        if (enSrc2Fmt >= TDE2_COLOR_FMT_CLUT1 && enSrc2Fmt <= TDE2_COLOR_FMT_ACLUT88)
	        {
	        	u16Code2 = 0x2;
	        }
	        break;

	    default:
	        u16Code2 = 0x8000;
	        return u16Code2;
    }

    return (u16Code1 | u16Code2);
}

#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeOsiSingleSrc2GetOptCode
* Description:   get dual source operate encode
* Input:         enSrc2Fmt foreground pixel format
                 enDstFmt  target pixel format
* Output:        none
* Return:        encode value, expression is fllowinf:

|------0-----|------1-----|------2-------|------3-----|
|-----ICSC---|-----OCSC---|(in)RGB->YCbCr|-----CLUT---|

* Others:       none
*****************************************************************************/
STATIC HI_U16 TdeOsiSingleSrc2GetOptCode(TDE2_COLOR_FMT_E enSrc2Fmt, TDE2_COLOR_FMT_E enDstFmt)
{
    HI_U16 u16Code = 0;
    TDE_COLORFMT_TRANSFORM_E enColorTransType;

    enColorTransType = TdeOsiGetFmtTransType(enSrc2Fmt, enDstFmt);

    switch (enColorTransType)
    {
	    case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
	        u16Code = 0x0;
	        break;

	    case TDE_COLORFMT_TRANSFORM_ARGB_YCbCr:
	        u16Code = 0x5;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
	        u16Code = 0x8;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_CLUT:
	        u16Code = 0x0;
	        break;

	    case TDE_COLORFMT_TRANSFORM_CLUT_YCbCr:
	        u16Code = 0xA;
	        break;

	    case TDE_COLORFMT_TRANSFORM_YCbCr_ARGB:
	        u16Code = 0x1;
	        break;

	    case TDE_COLORFMT_TRANSFORM_YCbCr_YCbCr:
	        u16Code = 0x0;
	        break;

	    default:
	        u16Code = 0x8000;
    }

    return u16Code;
}
#endif

/*****************************************************************************
* Function:      TdeOsiGetConvbyCode
* Description:   get format conversion manner by format conversion code
* Input:         u16Code  format conversion code
*                pstConv  format conversion struct
* Output:        none
* Return:        encode value
* Others:        none
*****************************************************************************/
STATIC HI_VOID TdeOsiGetConvbyCode(HI_U16 u16Code, TDE_DRV_CONV_MODE_CMD_S *pstConv)
{
    pstConv->bInConv   = u16Code & 0x1;
    pstConv->bOutConv  = (u16Code >> 1) & 0x1;
    pstConv->bInRGB2YC = ((u16Code >> 2) & 0x1);
    pstConv->bInSrc1Conv = ((u16Code >> 4) & 0x1);

    return;
}

STATIC HI_S32 TdeOsiGetbppByFmt16(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_COLOR_FMT_RGB444:
        case TDE2_COLOR_FMT_BGR444:
        case TDE2_COLOR_FMT_RGB555:
        case TDE2_COLOR_FMT_BGR555:
        case TDE2_COLOR_FMT_RGB565:
        case TDE2_COLOR_FMT_BGR565:
        case TDE2_COLOR_FMT_ARGB4444:
        case TDE2_COLOR_FMT_ABGR4444:
        case TDE2_COLOR_FMT_RGBA4444:
        case TDE2_COLOR_FMT_BGRA4444:
        case TDE2_COLOR_FMT_ARGB1555:
        case TDE2_COLOR_FMT_ABGR1555:
        case TDE2_COLOR_FMT_RGBA1555:
        case TDE2_COLOR_FMT_BGRA1555:
        case TDE2_COLOR_FMT_ACLUT88:
        case TDE2_COLOR_FMT_YCbCr422:
        case TDE2_COLOR_FMT_halfword:
            return 16;
        default:
            return -1;
    }
}

STATIC HI_S32 TdeOsiGetbppByFmt24(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_COLOR_FMT_RGB888:
        case TDE2_COLOR_FMT_BGR888:
        case TDE2_COLOR_FMT_ARGB8565:
        case TDE2_COLOR_FMT_ABGR8565:
        case TDE2_COLOR_FMT_RGBA8565:
        case TDE2_COLOR_FMT_BGRA8565:
        case TDE2_COLOR_FMT_YCbCr888:
            return 24;
        default:
            return -1;
    }
}
STATIC HI_S32 TdeOsiGetbppByFmt32(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_COLOR_FMT_ARGB8888:
        case TDE2_COLOR_FMT_ABGR8888:
        case TDE2_COLOR_FMT_RGBA8888:
        case TDE2_COLOR_FMT_BGRA8888:
        case TDE2_COLOR_FMT_AYCbCr8888:
        case TDE2_COLOR_FMT_RABG8888:
            return 32;
        default:
            return -1;
    }
}
STATIC HI_S32 TdeOsiGetbppByFmt8(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_COLOR_FMT_CLUT8:
        case TDE2_COLOR_FMT_ACLUT44:
        case TDE2_COLOR_FMT_A8:
        case TDE2_COLOR_FMT_byte:
            return 8;
        default:
            return -1;
    }
}
STATIC HI_S32 TdeOsiGetbppByFmt4(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_COLOR_FMT_CLUT4:
            return 4;

        default:
            return -1;
    }

}

STATIC HI_S32 TdeOsiGetbppByFmt2(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {

        case TDE2_COLOR_FMT_CLUT2:
            return 2;

        default:
            return -1;
    }
}
STATIC HI_S32 TdeOsiGetbppByFmt1(TDE2_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {

        case TDE2_COLOR_FMT_CLUT1:
        case TDE2_COLOR_FMT_A1:

            return 1;

        default:
            return -1;
    }
}
/*****************************************************************************
* Function:      TdeOsiGetbppByFmt
* Description:   get pixel bit of pixel format
* Input:         enFmt  target pixel format
* Output:        none
* Return:        -1 fail; other:pixel bit
* Others:        none
*****************************************************************************/

STATIC HI_S32 TdeOsiGetbppByFmt(TDE2_COLOR_FMT_E enFmt)
{
    HI_S32 s32Ret = 0;

    if (16 == (s32Ret = TdeOsiGetbppByFmt16(enFmt)))
    {
        return 16;
    }
    else if (24 == (s32Ret = TdeOsiGetbppByFmt24(enFmt)))
    {
        return 24;
    }
    else if (32 == (s32Ret = TdeOsiGetbppByFmt32(enFmt)))
    {
        return 32;
    }
    else if (1 == (s32Ret = TdeOsiGetbppByFmt1(enFmt)))
    {
        return 1;
    }
    else if (2 == (s32Ret = TdeOsiGetbppByFmt2(enFmt)))
    {
        return 2;
    }
    else if (4 == (s32Ret = TdeOsiGetbppByFmt4(enFmt)))
    {
        return 4;
    }
    else if (8 == (s32Ret = TdeOsiGetbppByFmt8(enFmt)))
    {
        return 8;
    }
    else
    {
        return -1;
    }

}

/*****************************************************************************
* Function:      TdeOsiGetScanInfo
* Description:   get scanning direction, avoid lap
* Input:         pSrc source bitmap
                 pstDst target bitmap
                 enMirror mirror type
* Output:        pstSrcDirection source scanning information
                 pstDstDirection target scanning information
* Return:        0  success
                 -1 fail
* Others:        AI7D02579 add  antiscan handle to YCbCr422R
*****************************************************************************/
STATIC HI_S32 TdeOsiGetScanInfo(TDE2_SURFACE_S *pSrc, TDE2_RECT_S *pstSrcRect, TDE2_SURFACE_S *pstDst,
                                TDE2_RECT_S *pstDstRect, TDE2_OPT_S* pstOpt,
                                TDE_SCANDIRECTION_S *pstSrcDirection,
                                TDE_SCANDIRECTION_S *pstDstDirection)
{
    HI_U32 u32SrcAddr;
    HI_U32 u32DstAddr;
    HI_BOOL bFilter = HI_FALSE;
    TDE2_MIRROR_E enMirror = TDE2_MIRROR_NONE;
    HI_S32 s32SrcdBpp, s32DstBpp;

    TDE_ASSERT(NULL != pSrc);
    TDE_ASSERT(NULL != pstSrcRect);
    TDE_ASSERT(NULL != pstDst);
    TDE_ASSERT(NULL != pstDstRect);
    TDE_ASSERT(NULL != pstSrcDirection);
    TDE_ASSERT(NULL != pstDstDirection);

    /* when writing, subbyte format align ask for byte align of start point */
    TDE_CHECK_SUBBYTE_STARTX(pstDstRect->s32Xpos, pstDstRect->u32Width, pstDst->enColorFmt);

    /* default scanning direction */
    pstSrcDirection->enHScan = TDE_SCAN_LEFT_RIGHT;
    pstSrcDirection->enVScan = TDE_SCAN_UP_DOWN;
    pstDstDirection->enHScan = TDE_SCAN_LEFT_RIGHT;
    pstDstDirection->enVScan = TDE_SCAN_UP_DOWN;

    if (HI_NULL != pstOpt)
    {
        if ((pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE) || pstOpt->bResize)
        {
            bFilter = HI_TRUE;
        }
        enMirror = pstOpt->enMirror;
    }

    if (enMirror && bFilter)
    {
        TDE_TRACE(TDE_KERN_INFO, "Could not support VF/FF/HF and Mirror\n!");
        return -1;
    }

    if (bFilter) /* if use filter, use default scanning direction */
    {
        return 0;
    }

    if (TDE2_MIRROR_NONE != enMirror)
    {
        TDE_CHANGE_DIR_BY_MIRROR(pstSrcDirection, enMirror);
    }
    else
    {
        /* only if stride is the same, can be do conversion */
        if (pSrc->u32Stride != pstDst->u32Stride)
        {
            return 0;
        }

        s32SrcdBpp = TdeOsiGetbppByFmt(pSrc->enColorFmt);
        s32DstBpp = TdeOsiGetbppByFmt(pstDst->enColorFmt);

        if ((s32DstBpp < 0) || (s32SrcdBpp < 0))
        {
            TDE_TRACE(TDE_KERN_INFO, "bits per pixel less than 0!\n");
            return -1;
        }

        u32SrcAddr = pSrc->u32PhyAddr + pstSrcRect->s32Ypos * pSrc->u32Stride
                     + ((pstSrcRect->s32Xpos * s32SrcdBpp) >> 3);
        u32DstAddr = pstDst->u32PhyAddr + pstDstRect->s32Ypos * pstDst->u32Stride \
                     + ((pstDstRect->s32Xpos * s32DstBpp) >> 3);

        /* source is above of target or on the left of the same direction */
        if (u32SrcAddr <= u32DstAddr)
        {
            pstSrcDirection->enVScan = TDE_SCAN_DOWN_UP;
            pstDstDirection->enVScan = TDE_SCAN_DOWN_UP;

            pstSrcDirection->enHScan = TDE_SCAN_RIGHT_LEFT;
            pstDstDirection->enHScan = TDE_SCAN_RIGHT_LEFT;
        }
    }

    return 0;
}

/*****************************************************************************
* Function:      TdeOsiGetInterRect
* Description:   get inter rect of two rectangles
* Input:         pRect1  input rectangle1
                 pRect2  input rectangle2
* Output:        pInterRect output inter rectangle
* Return:        0  have inter zone
                 -1 no inter zone
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiGetInterRect(TDE2_RECT_S *pRect1, TDE2_RECT_S *pRect2, TDE2_RECT_S *pInterRect)
{
    HI_S32 s32Left, s32Top, s32Right, s32Bottom;
    HI_S32 s32Right1, s32Bottom1, s32Right2, s32Bottom2;

    s32Left = (pRect1->s32Xpos > pRect2->s32Xpos) ? pRect1->s32Xpos : pRect2->s32Xpos;
    s32Top = (pRect1->s32Ypos > pRect2->s32Ypos) ? pRect1->s32Ypos : pRect2->s32Ypos;

    s32Right1 = pRect1->s32Xpos + pRect1->u32Width - 1;  /* AI7D02635 */
    s32Right2 = pRect2->s32Xpos + pRect2->u32Width - 1;  /* AI7D02635 */
    s32Right = (s32Right1 > s32Right2) ? s32Right2 : s32Right1;

    s32Bottom1 = pRect1->s32Ypos + pRect1->u32Height - 1; /* AI7D02635 */
    s32Bottom2 = pRect2->s32Ypos + pRect2->u32Height - 1; /* AI7D02635 */
    s32Bottom = (s32Bottom1 > s32Bottom2) ? s32Bottom2 : s32Bottom1;

    if ((s32Left > s32Right) || (s32Top > s32Bottom))
    {
        return -1;
    }

    pInterRect->s32Xpos   = s32Left;
    pInterRect->s32Ypos   = s32Top;
    pInterRect->u32Width  = s32Right - s32Left + 1;
    pInterRect->u32Height = s32Bottom - s32Top + 1;

    return 0;
}

/*****************************************************************************
* Function:      TdeOsiIsRect1InRect2
* Description:   Rect1 is if inside Rect2
* Input:         pRect1  input rectangle1
                 pRect2  input rectangle2
* Output:        none
* Return:        1  have inter zone
                 0  no inter zone
* Others:        none
*****************************************************************************/
STATIC INLINE HI_BOOL TdeOsiIsRect1InRect2(TDE2_RECT_S *pRect1, TDE2_RECT_S *pRect2)
{
    HI_S32 s32Right1, s32Bottom1, s32Right2, s32Bottom2;

    s32Right1 = pRect1->s32Xpos + pRect1->u32Width - 1; /* AI7D02635 */
    s32Right2 = pRect2->s32Xpos + pRect2->u32Width - 1; /* AI7D02635 */

    s32Bottom1 = pRect1->s32Ypos + pRect1->u32Height - 1; /* AI7D02635 */
    s32Bottom2 = pRect2->s32Ypos + pRect2->u32Height - 1; /* AI7D02635 */

    if((pRect1->s32Xpos >= pRect2->s32Xpos)
        &&(pRect1->s32Ypos >= pRect2->s32Ypos)
        &&(s32Right1 <= s32Right2)
        &&(s32Bottom1 <= s32Bottom2))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}
#ifndef HI_BUILD_IN_BOOT
STATIC INLINE HI_S32 TdeOsiSetHighQualityMbReizePara(TDE_HANDLE s32Handle,TDE2_MB_S* pstMB, TDE_HWNode_S* pstHWNode,
                                                         TDE2_RECT_S  *pstDstRect,TDE2_MBOPT_S* pMbOpt,TDE_MBPARA_S* stMbPara)
{
    TDE_HWNode_S* pstHWNodePass2=NULL;
    TDE_HWNode_S* pstHWNodePass3 = NULL;
    TDE2_RECT_S stOutRect;
    HI_S32 s32Ret;

    TDE2_DEFLICKER_MODE_E enDeflickerMode = ((pMbOpt->bDeflicker)?(TDE2_DEFLICKER_MODE_RGB):(TDE2_DEFLICKER_MODE_NONE));
     TDE_TRACE(TDE_KERN_DEBUG, "High Quality Mb Resize!\n");
     /* pass1. brightness zoom at first(deflicker) */
     if(TdeHalNodeInitNd(&pstHWNode)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return HI_ERR_TDE_NO_MEM;
     }
     TdeHalNodeSetSrcMbY(pstHWNode, &(stMbPara->stDrvS1), TDE_MB_Y_FILTER);
     TdeHalNodeSetTgt(pstHWNode, &(stMbPara->stDrvS1Tmp), stMbPara->enAlphaFrom);
     if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_MB_Y_OPT, TDE_ALU_NONE, 0)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         TdeHalFreeNodeBuf(pstHWNode);
         return HI_ERR_TDE_UNSUPPORTED_OPERATION;
     }
     stMbPara->stMbCmd.enMbMode = TDE_MB_Y_FILTER;
     TdeHalNodeSetMbMode(pstHWNode, &(stMbPara->stMbCmd));
     TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                           &(stMbPara->stAdjFieldRect), pstDstRect, HI_FALSE, HI_FALSE, stMbPara->enPicMode);
     TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS1);
     TDE_FILLUP_RECT_BY_DRVSURFACE(stOutRect, stMbPara->stDrvS1Tmp);
     TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS1), &(stMbPara->stDrvS1Tmp), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MBY);
     if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNode, &(stMbPara->stInRect), &stOutRect, enDeflickerMode,
                                       &(stMbPara->stFilterOpt))) < 0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         TDE_TRACE(TDE_KERN_INFO, "mb high quality pass1 fail\n!");
         return s32Ret;
     }

     /* pass2. chroma zoom at then */
     if(TdeHalNodeInitNd(&pstHWNodePass2)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return HI_ERR_TDE_NO_MEM;
     }
     /* foreground size restore to chroma size */
     TdeHalNodeSetSrcMbCbCr(pstHWNodePass2, &(stMbPara->stDrvS2), TDE_MB_CbCr_FILTER);
     TdeHalNodeSetTgt(pstHWNodePass2, &(stMbPara->stDrvS2Tmp), stMbPara->enAlphaFrom);
     if(TdeHalNodeSetBaseOperate(pstHWNodePass2, TDE_MB_C_OPT, TDE_ALU_NONE, 0)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return HI_ERR_TDE_UNSUPPORTED_OPERATION;
     }
     stMbPara->stMbCmd.enMbMode = TDE_MB_CbCr_FILTER;
     TdeHalNodeSetMbMode(pstHWNodePass2, &(stMbPara->stMbCmd));

     /* calculate chroma step and offset,
         note:step calculation need use chroma width and height,
             offset calculation need start address that user input at first */
     TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                           &(stMbPara->stAdjFieldRect), pstDstRect, HI_TRUE, HI_FALSE, stMbPara->enPicMode);
     TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS2);
     TDE_FILLUP_RECT_BY_DRVSURFACE(stOutRect, stMbPara->stDrvS2Tmp);
     TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS2), &(stMbPara->stDrvS2Tmp), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MBC);
     if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNodePass2, &(stMbPara->stInRect), &stOutRect, HI_FALSE /*pMbOpt->bDeflicker*/,
                                       &(stMbPara->stFilterOpt))) < 0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         TDE_TRACE(TDE_KERN_INFO, "mb high quality pass2 fail\n!");
         return s32Ret;
     }
     /* pass3. brightness and chroma combination */
     if(TdeHalNodeInitNd(&pstHWNodePass3)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return HI_ERR_TDE_NO_MEM;
     }
     if(TdeHalNodeSetColorConvert(pstHWNodePass3, &(stMbPara->stConv))<0)
     {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
     }
     s32Ret = TdeHalNodeSetCsc(pstHWNodePass3, stMbPara->stCscOpt);
     if (s32Ret < 0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return s32Ret;
     }

     TdeHalNodeSetSrcMbY(pstHWNodePass3, &(stMbPara->stDrvS1Tmp), TDE_MB_CONCA_FILTER);
     TdeHalNodeSetSrcMbCbCr(pstHWNodePass3, &(stMbPara->stDrvS2Tmp), TDE_MB_CONCA_FILTER);
     TdeHalNodeSetTgt(pstHWNodePass3, &(stMbPara->stDrvDst), stMbPara->enAlphaFrom);
     TdeHalNodeSetGlobalAlpha(pstHWNodePass3, pMbOpt->u8OutAlpha, HI_TRUE);
     if(TdeHalNodeSetBaseOperate(pstHWNodePass3, TDE_MB_2OPT, TDE_ALU_NONE, 0)<0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return HI_ERR_TDE_UNSUPPORTED_OPERATION;
     }
     stMbPara->stMbCmd.enMbMode = TDE_MB_CONCA_FILTER;
     TdeHalNodeSetMbMode(pstHWNodePass3, &(stMbPara->stMbCmd));
     TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS1Tmp), &(stMbPara->stDrvDst), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MB_CONCA_H);

     if ((s32Ret = TdeOsiMbSetClipPara(pstDstRect, pMbOpt, pstHWNodePass3)) < 0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return s32Ret;
     }
     if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNodePass3, stMbPara->u32WorkBufNum, TDE_NODE_SUBM_ALONE)) < 0)
     {
         TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
         return s32Ret;
     }

   return HI_SUCCESS;
}
STATIC INLINE HI_S32 TdeOsiSetMiddleQualityMbReizePara(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE_HWNode_S* pstHWNode,
        TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pMbOpt, TDE_MBPARA_S* stMbPara)
{
    TDE_HWNode_S* pstHWNodePass2 = NULL;

    TDE2_RECT_S stOutRect;
    HI_S32 s32Ret;
    TDE2_DEFLICKER_MODE_E enDeflickerMode = ((pMbOpt->bDeflicker) ? (TDE2_DEFLICKER_MODE_RGB) : (TDE2_DEFLICKER_MODE_NONE));

    TDE_TRACE(TDE_KERN_DEBUG, "Middle Quality Mb Resize!\n");

    /* pass1. chroma up-sample */
    if (TdeHalNodeInitNd(&pstHWNode) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_NO_MEM;
    }

    TdeHalNodeSetSrcMbCbCr(pstHWNode, &(stMbPara->stDrvS2), TDE_MB_CbCr_FILTER);
    TdeHalNodeSetTgt(pstHWNode, &(stMbPara->stDrvS1Tmp), stMbPara->enAlphaFrom);

    if (TdeHalNodeSetBaseOperate(pstHWNode, TDE_MB_C_OPT, TDE_ALU_NONE, 0) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    stMbPara->stMbCmd.enMbMode = TDE_MB_CbCr_FILTER;
    TdeHalNodeSetMbMode(pstHWNode, &(stMbPara->stMbCmd));

    /* calculate chroma step and offset.
    	notes: for chroma up-sample, zoom by brightness size, and input and output is the same size */
    TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                          &(stMbPara->stAdjFieldRect), &(stMbPara->stAdjFieldRect), HI_TRUE, HI_TRUE, stMbPara->enPicMode);
    TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS2);
    TDE_FILLUP_RECT_BY_DRVSURFACE(stOutRect, stMbPara->stDrvS1Tmp);
    stMbPara->stFilterOpt.stAdjInfo.enScaleMode = TDE_CHILD_SCALE_MBC;
    TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS2), &(stMbPara->stDrvS1Tmp), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MBC);

    if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNode, &(stMbPara->stInRect), &stOutRect, HI_FALSE,
                                           &(stMbPara->stFilterOpt))) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TDE_TRACE(TDE_KERN_INFO, "mb middle quality pass1 fail\n!");
        return s32Ret;
    }

    /* pass2. combinate brightness and chroma, then zoom */
    if (TdeHalNodeInitNd(&pstHWNodePass2) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_NO_MEM;
    }

    /* set encode by TdeHalNodeSetColorConvert */
    if (TdeHalNodeSetColorConvert(pstHWNodePass2, &(stMbPara->stConv)) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    s32Ret = TdeHalNodeSetCsc(pstHWNodePass2, stMbPara->stCscOpt);

    if (s32Ret < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return s32Ret;
    }

    stMbPara->stDrvS1.enColorFmt = TDE_DRV_COLOR_FMT_YCbCr444MB;
    TdeHalNodeSetSrcMbY(pstHWNodePass2, &(stMbPara->stDrvS1), TDE_MB_CONCA_FILTER);
    TdeHalNodeSetSrcMbCbCr(pstHWNodePass2, &(stMbPara->stDrvS1Tmp), TDE_MB_CONCA_FILTER);
    TdeHalNodeSetTgt(pstHWNodePass2, &(stMbPara->stDrvDst), (stMbPara->enAlphaFrom));
    TdeHalNodeSetGlobalAlpha(pstHWNodePass2, pMbOpt->u8OutAlpha, HI_TRUE);

    if (TdeHalNodeSetBaseOperate(pstHWNodePass2, TDE_MB_2OPT, TDE_ALU_NONE, 0) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    stMbPara->stMbCmd.enMbMode = TDE_MB_CONCA_FILTER;
    TdeHalNodeSetMbMode(pstHWNodePass2, &(stMbPara->stMbCmd));

    if ((s32Ret = TdeOsiMbSetClipPara(pstDstRect, pMbOpt, pstHWNodePass2)) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return s32Ret;
    }

    TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                          &(stMbPara->stAdjFieldRect), pstDstRect, HI_FALSE, HI_FALSE, TDE_FRAME_PIC_MODE);
    TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS1);
    stMbPara->stFilterOpt.u32WorkBufNum = stMbPara->u32WorkBufNum;
    TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS1), &(stMbPara->stDrvDst), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MB_CONCA_M);

    if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNodePass2, &(stMbPara->stInRect), pstDstRect,
                                           enDeflickerMode, &(stMbPara->stFilterOpt))) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return s32Ret;
    }

    return HI_SUCCESS;
}
#endif
STATIC INLINE HI_S32 TdeOsiSetLowQualityMbReizePara(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE_HWNode_S* pstHWNode,
        TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pMbOpt, TDE_MBPARA_S* stMbPara)
{
    HI_S32 s32Ret;

    TDE2_DEFLICKER_MODE_E enDeflickerMode = ((pMbOpt->bDeflicker)?(TDE2_DEFLICKER_MODE_RGB):(TDE2_DEFLICKER_MODE_NONE));
    TDE_TRACE(TDE_KERN_DEBUG, "Low Quality Mb Resize!\n");
    /* pass1. first connection and then zoom */
    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_NO_MEM;
    }
    /* set encode by call TdeHalNodeSetColorConvert */
    if(TdeHalNodeSetColorConvert(pstHWNode, &(stMbPara->stConv))<0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
       TdeHalFreeNodeBuf(pstHWNode);
       return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    s32Ret = TdeHalNodeSetCsc(pstHWNode, stMbPara->stCscOpt);
    if (s32Ret < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }
    TdeHalNodeSetSrcMbY(pstHWNode, &(stMbPara->stDrvS1), TDE_MB_CONCA_FILTER);

    /* foreground size need set to brightness size */
    TdeHalNodeSetSrcMbCbCr(pstHWNode, &(stMbPara->stDrvS2), TDE_MB_CONCA_FILTER);
    TdeHalNodeSetTgt(pstHWNode, &(stMbPara->stDrvDst), stMbPara->enAlphaFrom);
    TdeHalNodeSetGlobalAlpha(pstHWNode, pMbOpt->u8OutAlpha, HI_TRUE);
    if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_MB_2OPT, TDE_ALU_NONE, 0)<0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    stMbPara->stMbCmd.enMbMode = TDE_MB_CONCA_FILTER;
    TdeHalNodeSetMbMode(pstHWNode, &(stMbPara->stMbCmd));
    if ((s32Ret = TdeOsiMbSetClipPara(pstDstRect, pMbOpt, pstHWNode)) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }
    TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                          &(stMbPara->stAdjFieldRect), pstDstRect, HI_FALSE, HI_FALSE, TDE_FRAME_PIC_MODE);
    TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS1);
    TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS1), &(stMbPara->stDrvDst), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MB_CONCA_L);
    if((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNode, &(stMbPara->stInRect), pstDstRect,
                                    enDeflickerMode, &(stMbPara->stFilterOpt))) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return s32Ret;
    }

    return HI_SUCCESS;

}
STATIC INLINE HI_S32 TdeOsiSetNoneMbReizePara(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE_HWNode_S* pstHWNode,
        TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pMbOpt, TDE_MBPARA_S* stMbPara)
{
    HI_S32 s32Ret;

    TDE2_DEFLICKER_MODE_E enDeflickerMode = ((pMbOpt->bDeflicker) ? (TDE2_DEFLICKER_MODE_RGB) : (TDE2_DEFLICKER_MODE_NONE));
    TDE_TRACE(TDE_KERN_DEBUG, "Mb CUS by Filter!\n");
    TDE_TRACE(TDE_KERN_DEBUG, "Mb CUS by Filter!\n");

    /* first up-sample, then connection */
    if (TdeHalNodeInitNd(&pstHWNode) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return HI_ERR_TDE_NO_MEM;
    }

    /* set encode by call TdeHalNodeSetColorConvert */
    if (TdeHalNodeSetColorConvert(pstHWNode, &(stMbPara->stConv)) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    s32Ret = TdeHalNodeSetCsc(pstHWNode, stMbPara->stCscOpt);

    if (s32Ret < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    TdeHalNodeSetSrcMbY(pstHWNode, &(stMbPara->stDrvS1), TDE_MB_UPSAMP_CONCA);
    TdeHalNodeSetSrcMbCbCr(pstHWNode, &(stMbPara->stDrvS2), TDE_MB_UPSAMP_CONCA);
    TdeHalNodeSetTgt(pstHWNode, &(stMbPara->stDrvDst), stMbPara->enAlphaFrom);
    TdeHalNodeSetGlobalAlpha(pstHWNode, pMbOpt->u8OutAlpha, HI_TRUE);

    if (TdeHalNodeSetBaseOperate(pstHWNode, TDE_MB_2OPT, TDE_ALU_CONCA, 0) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    stMbPara->stMbCmd.enMbMode = TDE_MB_UPSAMP_CONCA;
    TdeHalNodeSetMbMode(pstHWNode, &(stMbPara->stMbCmd));

    if ((s32Ret = TdeOsiMbSetClipPara(pstDstRect, pMbOpt, pstHWNode)) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    TdeOsiCalcMbFilterOpt(&(stMbPara->stFilterOpt), pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                          &(stMbPara->stAdjFieldRect), &(stMbPara->stAdjFieldRect), HI_TRUE, HI_TRUE, stMbPara->enPicMode);
    TDE_FILLUP_RECT_BY_DRVSURFACE(stMbPara->stInRect, stMbPara->stDrvS2);
    TdeOsiSetFilterOptAdjInfo(&(stMbPara->stDrvS1), &(stMbPara->stDrvDst), &(stMbPara->stFilterOpt), TDE_CHILD_SCALE_MB_CONCA_CUS);

    if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNode, &(stMbPara->stInRect), pstDstRect,
                                           enDeflickerMode, &(stMbPara->stFilterOpt))) < 0)
    {
        TdeOsiListPutPhyBuff(stMbPara->u32WorkBufNum);
        return s32Ret;
    }

    return HI_SUCCESS;

}
STATIC INLINE HI_S32 TdeOsiSetMbReizePara(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE_HWNode_S* pstHWNode,
        TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pMbOpt, TDE_MBPARA_S* stMbPara)
{
    HI_S32 s32Ret;
    switch (pMbOpt->enResize)
    {

#ifndef HI_BUILD_IN_BOOT

        case TDE2_MBRESIZE_QUALITY_HIGH:
        {
            s32Ret = TdeOsiSetHighQualityMbReizePara( s32Handle, pstMB, pstHWNode, pstDstRect, pMbOpt, stMbPara);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }

            break;
        }

        case TDE2_MBRESIZE_QUALITY_MIDDLE:
        {
            s32Ret = TdeOsiSetMiddleQualityMbReizePara( s32Handle, pstMB, pstHWNode, pstDstRect, pMbOpt, stMbPara);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }

            break;
        }

#endif/**boot cut by y00181162 **/

        case TDE2_MBRESIZE_QUALITY_LOW:
        {
            s32Ret = TdeOsiSetLowQualityMbReizePara( s32Handle, pstMB, pstHWNode, pstDstRect, pMbOpt, stMbPara);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }

            break;
        }

        default:     /*TDE_MBRESIZE_NONE*/
        {
            s32Ret = TdeOsiSetNoneMbReizePara( s32Handle, pstMB, pstHWNode, pstDstRect, pMbOpt, stMbPara);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }

            break;
        }
    }

       return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiSetMbPara
* Description:   MB operate setting parameter interface
* Input:         s32Handle: task handle
*                pHWNode: config node
*                pstMB:    brightness block information struct
*                pstMbRect: MB bitmap operate zone
*                pstDst:  target bitmap information struct
*                pstDstRect: target bitmap operate zone
*                pMbOpt:  operate parameter setting struct
*                enPicMode: picture top filed/ bottom filed/frame
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiSetMbPara(TDE_HANDLE s32Handle,
                                     TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect,
                                     TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                     TDE2_MBOPT_S* pMbOpt)
{
    TDE_HWNode_S* pstHWNode=NULL;
  #ifndef HI_BUILD_IN_BOOT
 //   TDE_HWNode_S* pstHWNodePass2=NULL;
//    TDE_HWNode_S* pstHWNodePass3 = NULL;
  #endif
    TDE_DRV_SURFACE_S stDrvS1= {0};
    TDE_DRV_SURFACE_S stDrvS2= {0};
    TDE_DRV_SURFACE_S stDrvS1Tmp = {0};
    TDE_DRV_SURFACE_S stDrvS2Tmp = {0};
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};
    TDE_DRV_SURFACE_S stDrvDst = {0};
    TDE2_RECT_S stInRect;
  #ifndef HI_BUILD_IN_BOOT
//    TDE2_RECT_S stOutRect;
  #endif
    HI_U16 u16Code;
    TDE2_DEFLICKER_MODE_E enDeflickerMode;

    TDE2_RECT_S stAdjFieldRect;
    TDE_DRV_MB_CMD_S stMbCmd;
    HI_S32 s32Ret;
    TDE_SCANDIRECTION_S stScan = {0};    /* MB mode always use up->down,left->right */
    TDE_FILTER_OPT stFilterOpt = {0};
    TDE_PIC_MODE_E enPicMode = TDE_FRAME_PIC_MODE;
    HI_U32 u32WorkBufNum = 0;
    HI_BOOL bColorFmtMpYCbCr420 = HI_FALSE;
    TDE2_OUTALPHA_FROM_E enAlphaFrom = TDE2_OUTALPHA_FROM_NORM;
    TDE2_CSC_OPT_S stCscOpt = {0};
    TDE_MBPARA_S *stMbPara = HI_NULL;



    bColorFmtMpYCbCr420 = ((TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP == pstMB->enMbFmt)
                                   || (TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP == pstMB->enMbFmt)
                                   || (TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI == pstMB->enMbFmt));

    /* AI7D02555 */
    if (pMbOpt->bSetOutAlpha)
    {
        enAlphaFrom = TDE2_OUTALPHA_FROM_GLOBALALPHA;
    }

    /*judge if needing calculate by filed */
    if (bColorFmtMpYCbCr420)
    {
        enPicMode = TDE_TOP_FIELD_PIC_MODE;
    }

    /* set target bitmap format which driver layer need */
    TdeOsiConvertSurface(pstDst, pstDstRect, &stScan, &stDrvDst, NULL);

    /* get encode by call TdeOsiSingleSrc2GetOptCode */
    u16Code = TdeOsiDoubleSrcGetOptCode(TDE2_COLOR_FMT_YCbCr888, TDE2_COLOR_FMT_YCbCr888,
                                        pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);

    /* if format is 400MB, operate on brightness directly */
    if (TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP == pstMB->enMbFmt)
    {
        /* set brightness bitmap format which driver layer need */
        if (TdeOsiConvertMbSurface(pstMB, pstMbRect, &stDrvS1, HI_NULL, TDE_FRAME_PIC_MODE, HI_FALSE) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "TdeOsiConvertMbSurface error!\n");
            return HI_ERR_TDE_INVALID_PARA;
        }

        /* 0nly operate on brightness */
        if(TdeHalNodeInitNd(&pstHWNode)<0)
        {
            return HI_ERR_TDE_NO_MEM;
        }
        /* foreground's size need set to brightness's size */
        TdeHalNodeSetSrcMbY(pstHWNode, &stDrvS1, TDE_MB_Y_FILTER);
        TdeHalNodeSetSrcMbCbCr(pstHWNode, &stDrvS1, TDE_MB_Y_FILTER);
        TdeHalNodeSetTgt(pstHWNode, &stDrvDst, enAlphaFrom);
        if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_MB_Y_OPT, TDE_ALU_NONE, 0)<0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        stMbCmd.enMbMode = TDE_MB_CONCA_FILTER;
        TdeHalNodeSetMbMode(pstHWNode, &stMbCmd);
        TdeHalNodeSetGlobalAlpha(pstHWNode, pMbOpt->u8OutAlpha, HI_TRUE);

        if ((s32Ret = TdeOsiMbSetClipPara(pstDstRect, pMbOpt, pstHWNode)) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }

        if(TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }

		s32Ret = TdeHalNodeSetCsc(pstHWNode, stCscOpt);
        if (s32Ret < 0)
        {
        	TdeHalFreeNodeBuf(pstHWNode);
			return s32Ret;
        }

        /* if no filter operate,complete node setting directly */
        if ((TDE2_MBRESIZE_NONE == pMbOpt->enResize) && (HI_FALSE == pMbOpt->bDeflicker))
        {
            if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
            {
                TdeHalFreeNodeBuf(pstHWNode);
                return s32Ret;
            }

            return HI_SUCCESS;
        }

        enDeflickerMode = ((pMbOpt->bDeflicker) ? (TDE2_DEFLICKER_MODE_RGB) : (TDE2_DEFLICKER_MODE_NONE));
        TDE_FILLUP_RECT_BY_DRVSURFACE(stInRect, stDrvS1);
        TdeOsiCalcMbFilterOpt(&stFilterOpt, pstMB->enMbFmt, TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
                              pstMbRect, pstDstRect, HI_FALSE, HI_FALSE, TDE_FRAME_PIC_MODE);
        stFilterOpt.stAdjInfo.enScaleMode = TDE_CHILD_SCALE_MBY;

        if ((s32Ret = TdeOsiSetFilterChildNode(s32Handle, pstHWNode, &stInRect, pstDstRect, enDeflickerMode,
                                               &stFilterOpt)) < 0)
        {
            return s32Ret;
        }

        return HI_SUCCESS;

    }

    if (TdeOsiConvertMbSurface(pstMB, pstMbRect, &stDrvS1, &stDrvS2, enPicMode, HI_FALSE) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "could not transfer mb paras!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    /* adjust width and  height, by if field zoom */
    stAdjFieldRect.s32Xpos   = pstMbRect->s32Xpos;
    stAdjFieldRect.s32Ypos   = pstMbRect->s32Ypos;
    stAdjFieldRect.u32Width  = stDrvS1.u32Width;
    stAdjFieldRect.u32Height = stDrvS1.u32Height;

    /* adjust zoom quality by color format and if getting temporay buffer */
    s32Ret = TdeOsiAdjQuality(&stDrvS1Tmp, &stDrvS2Tmp, pstMB, pstMbRect,
                              pstDst, pstDstRect, enPicMode, pMbOpt);

    u32WorkBufNum = (HI_U32)s32Ret;
    TDE_TRACE(TDE_KERN_DEBUG, "u32WorkBufNum:%d\n", u32WorkBufNum);

    #ifndef HI_BUILD_IN_BOOT
    stMbPara = (TDE_MBPARA_S*)HI_GFX_KMALLOC(HIGFX_TDE_ID, sizeof(TDE_MBPARA_S), GFP_KERNEL);
    #else
    stMbPara = (TDE_MBPARA_S*)HI_GFX_Malloc(sizeof(TDE_MBPARA_S), "TDE_MBPARA_INFO");
    #endif
    if (HI_NULL == stMbPara)
    {
        TDE_TRACE(TDE_KERN_ERR, "Alloc memory failed !\n");
        return HI_ERR_TDE_NO_MEM;
    }

    stMbPara->enPicMode = enPicMode;
    stMbPara->u32WorkBufNum = u32WorkBufNum;
    stMbPara->enAlphaFrom = enAlphaFrom;

    HI_GFX_Memcpy(&(stMbPara->stConv), &stConv, sizeof(TDE_DRV_CONV_MODE_CMD_S));
    HI_GFX_Memcpy(&(stMbPara->stCscOpt), &stCscOpt, sizeof(TDE2_CSC_OPT_S));
    HI_GFX_Memcpy(&(stMbPara->stDrvDst), &stDrvDst, sizeof(TDE_DRV_SURFACE_S));
    HI_GFX_Memcpy(&(stMbPara->stDrvS1), &stDrvS1, sizeof(TDE_DRV_SURFACE_S));
    HI_GFX_Memcpy(&(stMbPara->stDrvS2), &stDrvS2, sizeof(TDE_DRV_SURFACE_S));
    HI_GFX_Memcpy(&(stMbPara->stDrvS1Tmp), &stDrvS1Tmp, sizeof(TDE_DRV_SURFACE_S));
    HI_GFX_Memcpy(&(stMbPara->stDrvS2Tmp), &stDrvS2Tmp, sizeof(TDE_DRV_SURFACE_S));
    HI_GFX_Memcpy(&(stMbPara->stFilterOpt), &stFilterOpt, sizeof(TDE_FILTER_OPT));
    HI_GFX_Memcpy(&(stMbPara->stInRect), &stInRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&(stMbPara->stScan), &stScan, sizeof(TDE_SCANDIRECTION_S));
    HI_GFX_Memcpy(&(stMbPara->stMbCmd), &stMbCmd, sizeof(TDE_DRV_MB_CMD_S));
    HI_GFX_Memcpy(&(stMbPara->stAdjFieldRect), &stAdjFieldRect, sizeof(TDE2_RECT_S));

    s32Ret = TdeOsiSetMbReizePara(s32Handle, pstMB, pstHWNode, pstDstRect, pMbOpt, stMbPara);

    #ifndef HI_BUILD_IN_BOOT
    HI_GFX_KFREE(HIGFX_TDE_ID, stMbPara);
    #else
    HI_GFX_Free((HI_CHAR*)stMbPara);
    #endif

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}


/*****************************************************************************
* Function:      TdeOsiAdjQuality
* Description:   set filter type by input argument settings
* Input:         pS1Tmp: buffer struct of foreground temporary bitmap
*                pS2Tmp: buffer struct of foreground temporary bitmap
*                pstMB:  MB bitmap information
*                pstMbRect: Mb bitmap operate zone
*                pstDstRect:  target bitmap operate zone
*                pMbOpt:  struct of operate parameter setting
* Output:        none
* Return:        number of temporary buffer/    -1: fail
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiAdjQuality(TDE_DRV_SURFACE_S* pS1Tmp, TDE_DRV_SURFACE_S* pS2Tmp,
                                      TDE2_MB_S* pstMB, TDE2_RECT_S* pstMbRect,
                                      TDE2_SURFACE_S* pstDst, TDE2_RECT_S* pstDstRect,
                                     TDE_PIC_MODE_E enPicMode, TDE2_MBOPT_S* pMbOpt)
{
    HI_U32 u32WorkBufferNum = 0;
    HI_BOOL bNeedNotZoom = ((TDE2_MBRESIZE_NONE == pMbOpt->enResize)
        ||((pstMbRect->u32Height == pstDstRect->u32Height)
            &&(pstMbRect->u32Width == pstDstRect->u32Width)));
    /* needn't zoom */
    if (bNeedNotZoom)
    {
        /* if it is YCbCr444MB, needn't up-sample, use TDE2_MBRESIZE_QUALITY_LOW directly */
        if (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == pstMB->enMbFmt)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_LOW;
            return 0;
        }
        /* if input is not YCbCr444MB and output is YCbCr422R(output:YC422R,unsupport block), it need be completed by quality_high zoom */
        else if (TDE2_COLOR_FMT_YCbCr422 == pstDst->enColorFmt)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_HIGH;
        }
        else
        {
            /* if no zoom but open deflicker, or no zoom but use field mode, use quality_high filter */
            if (!(pMbOpt->bDeflicker || TDE_FRAME_PIC_MODE != enPicMode))
            {
                /* if no zoom and no deflicker, no use fliter */
                pMbOpt->enResize = TDE2_MBRESIZE_NONE;
                return 0;
            }

            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_MIDDLE;

        }
    }
    else /* need zoom */
    {
        /* if it is YCbCr444MB and output is non YC422R, no need to up-sample and use TDE2_MBRESIZE_QUALITY_LOW  directly */
        if (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == pstMB->enMbFmt
            && TDE2_COLOR_FMT_YCbCr422 != pstDst->enColorFmt)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_LOW;
            return 0;
        }
        /* when zoom, output is YCbCr422R(output YC422R unspport block), need to use quality_high zoom */
        else if (TDE2_COLOR_FMT_YCbCr422 == pstDst->enColorFmt)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_HIGH;
        }
    }

    /* get temporary bitmap size, if fail, use quality_low zoom */
    if (TDE2_MBRESIZE_QUALITY_HIGH == pMbOpt->enResize)
    {
        /* brightness and chroma size is the same after quality_high filter */
        pS1Tmp->u32Width  = pS2Tmp->u32Width = pstDstRect->u32Width;
        pS1Tmp->u32Pitch  = (pS1Tmp->u32Width + 3) & (0xfffffffc); /*pitch need align by 4 bytes */
        pS2Tmp->u32Pitch  = (pS2Tmp->u32Width * 2 + 3) & (0xfffffffc);
        pS1Tmp->u32Height = pS2Tmp->u32Height = pstDstRect->u32Height;

        pS1Tmp->u32PhyAddr = TdeOsiListGetPhyBuff(0);
        if (0 == pS1Tmp->u32PhyAddr)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_LOW;
            return 0;
        }

        /* only if get successfully first, it must be successful at second time */
        pS2Tmp->u32PhyAddr = TdeOsiListGetPhyBuff(pS1Tmp->u32Pitch * pS1Tmp->u32Height);
        TDE_ASSERT(HI_NULL != pS2Tmp->u32PhyAddr);


        pS1Tmp->enColorFmt = TDE_DRV_COLOR_FMT_YCbCr444MB;/* the result of quality_high is always 444MB */
        pS2Tmp->enColorFmt = TDE_DRV_COLOR_FMT_YCbCr444MB;

        u32WorkBufferNum = 2;
    }
    else if (TDE2_MBRESIZE_QUALITY_MIDDLE == pMbOpt->enResize)
    {
        pS1Tmp->enColorFmt = TDE_DRV_COLOR_FMT_YCbCr444MB;/* quality_mid zoom result is always 444MB */
        pS1Tmp->u32Width  = pstMbRect->u32Width;

        pS1Tmp->u32Height = (TDE_FRAME_PIC_MODE != enPicMode) ? TDE_ADJ_FIELD_HEIGHT_BY_START((HI_U32)pstMbRect->s32Ypos, pstMbRect->u32Height):
                                                                pstMbRect->u32Height;

        pS1Tmp->u32Pitch   = (pS1Tmp->u32Width * 2 + 3) & (0xfffffffc);/*pitch need align by 4 bytes */
        pS1Tmp->u32PhyAddr = TdeOsiListGetPhyBuff(0);
        if (0 == pS1Tmp->u32PhyAddr)
        {
            pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_LOW;
            return 0;
        }

        u32WorkBufferNum = 1;
    }
    else
    {
        pMbOpt->enResize = TDE2_MBRESIZE_QUALITY_LOW;
    }

    return u32WorkBufferNum;
}

/*****************************************************************************
* Function:      TdeOsiMbCalcHStep
* Description:   calculate horizontal stride zoomed
* Input:         u32Wi: input bitmap width
*                u32Wo: output bitmap width
*                enFmt: bitmap format
*                bCbCr: calculate chroma/brightness horizontal zoomed stide
* Output:        none
* Return:        calculate back step, premultiply is 2*12
* Others:        none
*****************************************************************************/
STATIC INLINE HI_U32 TdeOsiMbCalcHStep(HI_U32 u32Wi, HI_U32 u32Wo,
                                       TDE2_MB_COLOR_FMT_E enInFmt, TDE2_MB_COLOR_FMT_E enOutFmt, HI_BOOL bCbCr)
{
    if (1 >= u32Wo)
    {
        return 0;
    }
    if (enInFmt != enOutFmt && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enOutFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "Can't calc Hstep from format:%d to format:%d\n", enInFmt, enOutFmt);
        //return 0;
    }
#ifndef HI_BUILD_IN_BOOT/**codecc**/
    /* calculate horizontal zoomed step of brightness, need round for pointing stide */
    if (bCbCr && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == enOutFmt)
    {
        if ((TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP != enInFmt) && (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enInFmt))
        {
            return (((u32Wi - 1) << TDE_FLOAT_BITLEN) / (u32Wo - 1) + 1) >> 1;
        }
    }

    if (bCbCr && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == enInFmt)
    {
        if ((TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP != enOutFmt) && (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enOutFmt))
        {
            return (((u32Wi - 1) << TDE_FLOAT_BITLEN) / (u32Wo - 1) ) << 1;
        }
    }
#endif
    return (((u32Wi - 1) << (TDE_FLOAT_BITLEN + 1)) / (u32Wo - 1) + 1) >> 1;
}

/*****************************************************************************
* Function:      TdeOsiMbCalcVStep
* Description:   calculate vertical zoomed step
* Input:         u32Hi: input bitmap width
*                u32Ho: output bitmap width
*                enFmt: bitmap format
*                bCbCr: calculate vertical zoomed step of brightness/chroma
* Output:        none
* Return:        calculate back stride, premultiply is 2^12
* Others:        none
*****************************************************************************/
STATIC INLINE HI_U32 TdeOsiMbCalcVStep(HI_U32 u32Hi, HI_U32 u32Ho,
                                       TDE2_MB_COLOR_FMT_E enInFmt, TDE2_MB_COLOR_FMT_E enOutFmt, HI_BOOL bCbCr)
{
    if (1 >= u32Ho)
    {
        return 0;
    }
    if (enInFmt != enOutFmt && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enOutFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "Can't calc Vstep from format:%d to format:%d\n", enInFmt, enOutFmt);
        //return 0;
    }

#ifndef HI_BUILD_IN_BOOT/**codecc**/
    /* calculate vertical zoomed step of brightness, need round for pointing stide */
    if (bCbCr && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == enOutFmt)
    {
        if ((TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP != enInFmt) && (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enInFmt))
        {
            return (((u32Hi - 1) << TDE_FLOAT_BITLEN) / (u32Ho - 1) + 1) >> 1;  /* 0.5 * step */
        }
    }

    if (bCbCr && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == enInFmt)
    {
        if ((TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP != enOutFmt) && (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enOutFmt))
        {
            return (((u32Hi - 1) << (TDE_FLOAT_BITLEN)) / (u32Ho - 1) ) << 1;  /* 2 * step */
        }
    }
#endif
    return (((u32Hi - 1) << (TDE_FLOAT_BITLEN + 1)) / (u32Ho - 1) + 1) >> 1;    /* step */
}

#ifndef HI_BUILD_IN_BOOT/**codecc**/
/*****************************************************************************
* Function:      TdeOsiMbCalcHOffset
* Description:   calculate vertical zoomed stride
* Input:         u32Xi: input bitmap start horizontal position
*                enFmt: bitmap format
*                bCbCr: if calculate chroma's offset
* Output:        none
* Return:        calculate back Offset,premultiply:2^12
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiMbCalcHOffset(HI_U32 u32Xi, TDE2_MB_COLOR_FMT_E enFmt, HI_BOOL bCbCr)
{
    if (!bCbCr)
    {
        return 0;
    }

    if ((TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP == enFmt)
        || (TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP == enFmt))
    {
        if ( 1 == (u32Xi & 0x1) )
        {
            return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 2));    /* 0.25 * 2^12 */
        }

        return (HI_S32)((-1) << (TDE_FLOAT_BITLEN - 2));    /* -0.25 * 2^12 */
    }

    if ((TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP == enFmt)
             || (TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI == enFmt)
             || (TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP == enFmt))
    {
        if( 1 == (u32Xi & 0x1))
        {
            return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 * 2^12 */
        }
    }

    return 0;

}
/*****************************************************************************
* Function:      TdeOsiMbCalcVOffset
* Description:   calculate vertical zoomed step
* Input:         u32Yi: input bitmap start height
*                enFmt: bitmap format
*                enPicMode: frame picture/ bottom field/ top field
*                bCbCr: if calculate chroma's offset
* Output:        none
* Return:        calculate back Offset,premultiply:2^12
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiMbCalcVOffset(HI_U32 u32Yi, TDE2_MB_COLOR_FMT_E enFmt,
                                         TDE_PIC_MODE_E enPicMode,HI_BOOL bCbCr)
{
    HI_BOOL bColorFmtMPYCbCr420 = ((TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI == enFmt)
                || (TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP == enFmt)
                || (TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP == enFmt));

    if (bCbCr)
    {
        if (TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP == enFmt)
        {
            switch (u32Yi & 0x1)
            {
	            case 1:
	                return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 2));    /* 0.25 * 2^12 */
	            default:
	                return (HI_S32)((-1) << (TDE_FLOAT_BITLEN - 2));    /* -0.25 * 2^12 */
            }
        }
        else if (bColorFmtMPYCbCr420)
        {
            switch (u32Yi & 0x3)
            {
	            case 0:
	            {
	                return (HI_S32)((-1) << (TDE_FLOAT_BITLEN - 3));    /* -0.125 * 2^12 */
	            }
	            case 1:
	            {
	                return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 3));    /* 0.125 * 2^12 */
	            }
	            case 2:
	            {
	                return (HI_S32)(3 << (TDE_FLOAT_BITLEN - 3));    /* 0.375 * 2^12 */
	            }
	            default:/**3**/
	            {
	                return (HI_S32)(5 << (TDE_FLOAT_BITLEN - 3));    /* 0.625 * 2^12 */
	            }
            }
        }
        else if (TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP == enFmt)
        {
            switch (u32Yi & 0x1)
            {
	            case 1:
	                return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 * 2^12 */
	            default:
	                return 0;
            }
        }

        return 0;

    }

    if (bColorFmtMPYCbCr420)
    {
        switch (u32Yi & 0x3)
        {
            case 1:
            case 3:
            {
                return (HI_S32)(1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 * 2^12 */
            }

            default:
                return 0;
        }
    }

    return 0;

 }
#endif

/*****************************************************************************
* Function:      TdeOsiCalcNoLapSlice
* Description:   calculate value when block,it is no lap
* Input:         pu32WFirst: return first block's size
*                pu32WLast: return last block's size
*                u32Xi: input bitmap start horizontal  coordinate
*                u32BmpW: bitmap width(no horizontal zoom, input and output is consistent)
*                u32Bppi: pixel bit width/ each pixel in input bitmap
* Output:        none
* Return:        return block number
* Others:        AI7D02711 alloc dynamicly
                 AI7D02579 add antiscaning for YC422R and operate on last point
*****************************************************************************/
STATIC INLINE HI_U32 TdeOsiCalcNoLapSlice(TDE_SLICE_INFO_S *pSliceInfo, HI_U32 u32MaxNum, HI_U32 u32Xi, HI_U32 u32Xo,
                                          HI_U32 u32BmpW, TDE_FILTER_OPT* pstFilterOpt)
{
#define TDE_GET_STARTX_BY_SCANINFO(hScan, sliceW, x) \
        ((hScan) ? (HI_U32)((x) + (sliceW) - 1) : ((HI_U32)(x)))

    /* HI_U32 u32SliceNum; */
    HI_U32 u32BmpXo = u32Xo;
    HI_U32 u32OverLapPix = (pstFilterOpt->bEvenStartInX && pstFilterOpt->bEvenStartOutX) ? 4 : 2;
    HI_U32 n = 1;

    if (TDE_MAX_SLICE_WIDTH >= u32BmpW)
    {
        pSliceInfo[0].u32SliceWi = u32BmpW;
        pSliceInfo[0].u32SliceXi = TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stSrcDire.enHScan, pSliceInfo[0].u32SliceWi, u32Xi);;
        pSliceInfo[0].s32SliceHOfst = pstFilterOpt->s32HOffset;
        pSliceInfo[0].u32SliceWo = u32BmpW;
        pSliceInfo[0].u32SliceXo = TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stDstDire.enHScan, pSliceInfo[0].u32SliceWo, u32Xo);;

        pSliceInfo[0].enSliceType = TDE_NO_BLOCK_SLICE_TYPE;
        return 1;
    }
    else
    {
        HI_U32 u32OfstWord;
        /* HI_U32 u32WFirst; */
        /* HI_U32 u32W64SliceNum;*/    /* not include first block */

        /* calculate start position and the offset of word */
        u32OfstWord = (u32Xi * pstFilterOpt->u32Bppi % 128) / pstFilterOpt->u32Bppi;
        if (pstFilterOpt->bEvenStartInX)
        {
            u32OfstWord &= (~0x1);
        }

        pSliceInfo[0].enSliceType = TDE_FIRST_BLOCK_SLICE_TYPE;

        pSliceInfo[0].u32SliceWi = TDE_MAX_SLICE_WIDTH - u32OfstWord;
        pSliceInfo[0].u32SliceXi =
            TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stSrcDire.enHScan, pSliceInfo[0].u32SliceWi, u32Xi);
        pSliceInfo[0].s32SliceHOfst = pstFilterOpt->s32HOffset;
        pSliceInfo[0].u32SliceWo = pSliceInfo[0].u32SliceWi;
        pSliceInfo[0].u32SliceXo =
            TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stDstDire.enHScan, pSliceInfo[0].u32SliceWo, u32Xo);
        u32Xo += (pstFilterOpt->bBadLastPix) ? (pSliceInfo[0].u32SliceWo - u32OverLapPix) : (pSliceInfo[0].u32SliceWo);
        u32Xi += (pstFilterOpt->bBadLastPix) ? (pSliceInfo[0].u32SliceWi - u32OverLapPix) : (pSliceInfo[0].u32SliceWi);

        for (n = 1; n <= u32MaxNum + 1; n++) /* u32MaxNum protect form memory is not outside */
        {
            if ((HI_S32)(u32BmpW - u32Xo + u32BmpXo) <= TDE_MAX_SLICE_WIDTH)
            {
                break;
            }
            pSliceInfo[n].enSliceType = TDE_MID_BLOCK_SLICE_TYPE;
            pSliceInfo[n].u32SliceWi = TDE_MAX_SLICE_WIDTH;
            pSliceInfo[n].u32SliceXi =
                TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stSrcDire.enHScan, pSliceInfo[n].u32SliceWi, u32Xi);
            pSliceInfo[n].s32SliceHOfst = pstFilterOpt->s32HOffset;
            pSliceInfo[n].u32SliceWo = pSliceInfo[n].u32SliceWi;
            pSliceInfo[n].u32SliceXo =
                TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stDstDire.enHScan, pSliceInfo[n].u32SliceWo, u32Xo);
            u32Xo += (pstFilterOpt->bBadLastPix) ? (pSliceInfo[n].u32SliceWo - u32OverLapPix) : (pSliceInfo[n].u32SliceWo);
            u32Xi += (pstFilterOpt->bBadLastPix) ? (pSliceInfo[n].u32SliceWi - u32OverLapPix) : (pSliceInfo[n].u32SliceWi);
        }

        if ((HI_S32)(u32BmpW - u32Xo + u32BmpXo) > 0)
        {
            /* recalculate to leaving width on last block */
            pSliceInfo[n].enSliceType = TDE_LAST_BLOCK_SLICE_TYPE;
            pSliceInfo[n].u32SliceWi = u32BmpW - u32Xo + u32BmpXo;
            pSliceInfo[n].u32SliceXi =
                TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stDstDire.enHScan, pSliceInfo[n].u32SliceWi, u32Xi);
            pSliceInfo[n].s32SliceHOfst = pstFilterOpt->s32HOffset;
            pSliceInfo[n].u32SliceWo = pSliceInfo[n].u32SliceWi;

            pSliceInfo[n].u32SliceXo =
                TDE_GET_STARTX_BY_SCANINFO(pstFilterOpt->stDstDire.enHScan, pSliceInfo[n].u32SliceWo, u32Xo);
        }
    }
    return n + 1;
}

STATIC INLINE HI_VOID TdeOsiCalcOverLapSliceWi(TDE_OVERLAPSLICE_DATUM* stOverSlapSliceDatum, TDE_FILTER_OPT* pstFilterOpt)
{
    if (TDE_MAX_SLICE_WIDTH >= stOverSlapSliceDatum->u32LeftWi)
    {
        stOverSlapSliceDatum->u32Wi = stOverSlapSliceDatum->u32LeftWi;

        /* last slice,update back_pix*/
        stOverSlapSliceDatum->u32BackPix = 0;
    }
    else
    {
        /* calculate start position and word offset */
        stOverSlapSliceDatum->u32OfstWord = (stOverSlapSliceDatum->u32Xi * pstFilterOpt->u32Bppi % 128) / pstFilterOpt->u32Bppi;
        stOverSlapSliceDatum->u32Wi = TDE_MAX_SLICE_WIDTH - stOverSlapSliceDatum->u32OfstWord;
    }

    return;
}
STATIC INLINE HI_VOID TdeOsiCalcOverLapSliceWo( TDE_OVERLAPSLICE_DATUM* stOverSlapSliceDatum, TDE_FILTER_OPT* pstFilterOpt)
{
    if (0 > (HI_S32)((stOverSlapSliceDatum->u32Wi - 5 - stOverSlapSliceDatum->u32BackPix) * TDE_NO_SCALE_STEP - stOverSlapSliceDatum->s32HOfst))
    {
        stOverSlapSliceDatum->u32Wo = stOverSlapSliceDatum->u32LeftWo;
    }
    else
    {
        if (TDE_MAX_SLICE_WIDTH >= stOverSlapSliceDatum->u32LeftWi)
        {
            stOverSlapSliceDatum->u32Wo = stOverSlapSliceDatum->u32LeftWo;
        }
        else
        {
            stOverSlapSliceDatum->u32Wo = ((stOverSlapSliceDatum->u32Wi - 5 - stOverSlapSliceDatum->u32BackPix) * TDE_NO_SCALE_STEP - stOverSlapSliceDatum->s32HOfst ) / pstFilterOpt->u32HStep + 1;
            /* if output need drop-sample, need ensure output is even */
            stOverSlapSliceDatum->u32Wo = (pstFilterOpt->bEvenStartOutX) ? (stOverSlapSliceDatum->u32Wo & (~0x1)) : (stOverSlapSliceDatum->u32Wo);
        }
    }

    if (stOverSlapSliceDatum->u32Wo > stOverSlapSliceDatum->u32LeftWo)
    {
        stOverSlapSliceDatum->u32Wo = stOverSlapSliceDatum->u32LeftWo;
    }

    return;
}
STATIC INLINE HI_VOID TdeOsiCalcOverLapSliceLeftWiWo( HI_U32 u32BmpXi, HI_U32 u32BmpWi, HI_U32 u32BmpXo, HI_U32 u32BmpWo,
                                                                TDE_OVERLAPSLICE_DATUM* stOverSlapSliceDatum, TDE_FILTER_OPT* pstFilterOpt)
{
    if (!pstFilterOpt->bEvenStartOutX)
    {
        stOverSlapSliceDatum->u32Xi = ((stOverSlapSliceDatum->s32HOfst + pstFilterOpt->u32HStep * stOverSlapSliceDatum->u32Wo) >> TDE_FLOAT_BITLEN) - 3 + stOverSlapSliceDatum->u32Xi;
        stOverSlapSliceDatum->s32HOfst = ((stOverSlapSliceDatum->s32HOfst + pstFilterOpt->u32HStep * stOverSlapSliceDatum->u32Wo) & 0x0fff) + 3 * TDE_NO_SCALE_STEP;

        stOverSlapSliceDatum->u32Xo = stOverSlapSliceDatum->u32Xo + stOverSlapSliceDatum->u32Wo;
    }
    else
    {
        stOverSlapSliceDatum->u32Xi = ((stOverSlapSliceDatum->s32HOfst + pstFilterOpt->u32HStep * (stOverSlapSliceDatum->u32Wo - 2)) >> TDE_FLOAT_BITLEN) - 3 + stOverSlapSliceDatum->u32Xi;
        stOverSlapSliceDatum->s32HOfst = ((stOverSlapSliceDatum->s32HOfst + pstFilterOpt->u32HStep * (stOverSlapSliceDatum->u32Wo - 2)) & 0x0fff) + 3 * TDE_NO_SCALE_STEP;

        stOverSlapSliceDatum->u32Xo = stOverSlapSliceDatum->u32Xo + stOverSlapSliceDatum->u32Wo - 2;
    }

    if (pstFilterOpt->bEvenStartInX)
    {
        stOverSlapSliceDatum->s32HOfst += (stOverSlapSliceDatum->u32Xi & 0x1) ? (TDE_NO_SCALE_STEP) : (0);
        stOverSlapSliceDatum->u32Xi = stOverSlapSliceDatum->u32Xi & ~0x1;
    }

    /* update LeftWi,u32LeftWo */
    stOverSlapSliceDatum->u32LeftWi = u32BmpXi + u32BmpWi - stOverSlapSliceDatum->u32Xi;
    stOverSlapSliceDatum->u32LeftWo = u32BmpXo + u32BmpWo - stOverSlapSliceDatum->u32Xo;

    return;

}
STATIC INLINE HI_VOID TdeOsiUpdateOverLapSliceWi(TDE_OVERLAPSLICE_DATUM* stOverSlapSliceDatum, TDE_FILTER_OPT* pstFilterOpt)
{
    stOverSlapSliceDatum->u32CalcWi = (pstFilterOpt->bEvenStartOutX) ? (((stOverSlapSliceDatum->s32HOfst + pstFilterOpt->u32HStep * (stOverSlapSliceDatum->u32Wo - 1)) >> TDE_FLOAT_BITLEN) + 5 + stOverSlapSliceDatum->u32BackPix) : stOverSlapSliceDatum->u32Wi;


    /* protect judge, output w is inside of bitmap W */
    if (stOverSlapSliceDatum->u32CalcWi > stOverSlapSliceDatum->u32LeftWi)
    {
        stOverSlapSliceDatum->u32CalcWi = stOverSlapSliceDatum->u32LeftWi;
    }

    return;
}
/*****************************************************************************
* Function:      TdeOsiCalcOverLapSlice
* Description:   calcute value when block,it overlap
* Input:         pSliceInfo: slice information
*                u32BmpXi: input start horizontal position of bitmap
*                u32BmpWi: input bitmap width
*                u32BmpHStep: horizontal stride
*                s32HOffset: horizontal offset
*                u32BmpXo: output start horizontal position of bitmap
*                u32BmpWo: output bitmap width
*                u32Bppi: input bit width each pixel
* Output:        none
* Return:        return slice number
* Others:        AI7D02711 change to alloc dynamicly
*****************************************************************************/
STATIC INLINE HI_U32 TdeOsiCalcOverLapSlice(TDE_SLICE_INFO_S *pSliceInfo, HI_U32 u32MaxNum, HI_U32 u32BmpXi, HI_U32 u32BmpWi,
                                            HI_U32 u32BmpXo, HI_U32 u32BmpWo, TDE_FILTER_OPT* pstFilterOpt)
{
    TDE_OVERLAPSLICE_DATUM stOverSlapSliceDatum = {0};
    HI_U32 n;

    stOverSlapSliceDatum.u32Xi = u32BmpXi;
    stOverSlapSliceDatum.s32HOfst = pstFilterOpt->s32HOffset;
    stOverSlapSliceDatum.u32Xo = u32BmpXo;
    stOverSlapSliceDatum.u32OfstWord = 0;
    stOverSlapSliceDatum.u32LeftWi = u32BmpWi;
    stOverSlapSliceDatum.u32LeftWo = u32BmpWo;
    stOverSlapSliceDatum.u32CalcWi = 0;
    stOverSlapSliceDatum.u32BackPix = (pstFilterOpt->bEvenStartInX) ? 2 : 0; /* u32BackPix is 2 at h2 version */

    if (0 == pstFilterOpt->u32HStep)
    {
        pSliceInfo[0].u32SliceXi = stOverSlapSliceDatum.u32Xi;
        pSliceInfo[0].u32SliceWi = u32BmpWi;
        pSliceInfo[0].s32SliceHOfst = pstFilterOpt->s32HOffset;
        pSliceInfo[0].u32SliceXo = stOverSlapSliceDatum.u32Xo;
        pSliceInfo[0].u32SliceWo = u32BmpWo;
        pSliceInfo[0].enSliceType = TDE_NO_BLOCK_SLICE_TYPE;
        return 1;
    }

    /* u32MaxNum: protect from memory is inside */
    for (n = 0; n < u32MaxNum; n++) /*AI7D02711*/
    {
        /* step1. from Xi, HOfst, Xo calculate into Wi */
        TdeOsiCalcOverLapSliceWi(&stOverSlapSliceDatum, pstFilterOpt);

        /* step2  calculate Wo by Wi */
        TdeOsiCalcOverLapSliceWo(&stOverSlapSliceDatum, pstFilterOpt);

        /* if drop-sample */

        TdeOsiUpdateOverLapSliceWi(&stOverSlapSliceDatum, pstFilterOpt);

        /* step2. get the nth array of coecifients: Xi, Wi, HOfst, Xo, Wo */
        pSliceInfo[n].s32SliceHOfst = stOverSlapSliceDatum.s32HOfst;
        pSliceInfo[n].u32SliceWi = (pstFilterOpt->bEvenStartInX) ? ((stOverSlapSliceDatum.u32CalcWi + 1) & (~0x1)) : (stOverSlapSliceDatum.u32CalcWi);
        pSliceInfo[n].u32SliceWo = stOverSlapSliceDatum.u32Wo;
        pSliceInfo[n].u32SliceXi = stOverSlapSliceDatum.u32Xi;
        pSliceInfo[n].u32SliceXo = stOverSlapSliceDatum.u32Xo;
        pSliceInfo[n].enSliceType = TDE_MID_BLOCK_SLICE_TYPE;

        /* if the leaving width is 0, break */
        if (((u32BmpXi + u32BmpWi) <= (stOverSlapSliceDatum.u32Xi + pSliceInfo[n].u32SliceWi))
            || ((u32BmpXo + u32BmpWo) <= (stOverSlapSliceDatum.u32Xo + stOverSlapSliceDatum.u32Wo)))
        {
            n++;
            break;
        }

        TdeOsiCalcOverLapSliceLeftWiWo(u32BmpXi, u32BmpWi, u32BmpXo, u32BmpWo,&stOverSlapSliceDatum, pstFilterOpt);

    }

    pSliceInfo[0].enSliceType = TDE_FIRST_BLOCK_SLICE_TYPE;
    pSliceInfo[n - 1].enSliceType = TDE_LAST_BLOCK_SLICE_TYPE;

    if (n == 1)
    {
        pSliceInfo[0].enSliceType = TDE_NO_BLOCK_SLICE_TYPE;
    }

    return n;
}

STATIC HI_S32 TdeOsiSetChildNodeInfo(TDE_HANDLE s32Handle, TDE_SLICE_INFO_S* sliceInfo,
                                              TDE_HWNode_S* pNode,  TDE_CHILD_INFO* stChildInfo, TDE_FILTER_OPT* pstFilterOpt,
                                              TDE2_RECT_S* pInRect,
                                              TDE2_RECT_S*  pOutRect, HI_U32 u32Num)
{
    TDE_HWNode_S*pChildNode = HI_NULL;
    HI_U32 u32TDE_CLIP_START = pNode->u32TDE_CLIP_START;
    HI_U32 u32TDE_CLIP_STOP = pNode->u32TDE_CLIP_STOP;

    HI_VOID* pBuf = HI_NULL;

    HI_S32 s32Ret;
    pBuf = (HI_VOID *)TDE_MALLOC(sizeof(TDE_HWNode_S)+ TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
     if (HI_NULL == pBuf)
     {
         TDE_TRACE(TDE_KERN_INFO, "malloc (%d) failed, wgetfreenum(%d)!\n", (sizeof(TDE_HWNode_S) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE), wgetfreenum());
     #ifndef HI_BUILD_IN_BOOT
         HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
     #else
         HI_GFX_Free((HI_CHAR*)sliceInfo);
     #endif
         return HI_ERR_TDE_NO_MEM;
     }
     pChildNode = (TDE_HWNode_S*)(pBuf +TDE_NODE_HEAD_BYTE);
     HI_GFX_Memcpy(pChildNode,pNode,sizeof(TDE_HWNode_S));
     TdeHalNodeInitChildNd(pChildNode,u32TDE_CLIP_START, u32TDE_CLIP_STOP);
     /* set child node fliter parameter information */
     if(TdeHalNodeSetResize(pChildNode,pstFilterOpt,TDE_NODE_SUBM_CHILD)<0)
     {
     #ifndef HI_BUILD_IN_BOOT
         HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
     #else
         HI_GFX_Free((HI_CHAR*)sliceInfo);
     #endif
         TDE_FREE(pBuf);
         return HI_ERR_TDE_UNSUPPORTED_OPERATION;
     }
     stChildInfo->u32Xi = sliceInfo[u32Num].u32SliceXi;
     stChildInfo->u32Yi = (HI_U32)pInRect->s32Ypos;
     stChildInfo->u32Wi = sliceInfo[u32Num].u32SliceWi;
     stChildInfo->u32Hi = pInRect->u32Height;
     stChildInfo->u32HOfst = sliceInfo[u32Num].s32SliceHOfst;
     stChildInfo->u32VOfst = pstFilterOpt->s32VOffset;
     stChildInfo->u32Xo = sliceInfo[u32Num].u32SliceXo;
     stChildInfo->u32Yo = (HI_U32)pOutRect->s32Ypos;
     stChildInfo->u32Wo = sliceInfo[u32Num].u32SliceWo;
     stChildInfo->u32Ho = pOutRect->u32Height;
     stChildInfo->enSliceType = sliceInfo[u32Num].enSliceType;
     TdeHalNodeAddChild(pChildNode,stChildInfo);
     s32Ret = TdeOsiSetNodeFinish(s32Handle, pChildNode, 0, TDE_NODE_SUBM_CHILD);
     if (HI_SUCCESS != s32Ret)
     {
     #ifndef HI_BUILD_IN_BOOT
         HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
     #else
         HI_GFX_Free((HI_CHAR*)sliceInfo);
     #endif
         TDE_FREE(pBuf);
         return s32Ret;
     }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiSetFilterChildNode
* Description:   calculate child nodes of slice and add into task list
* Input:         s32Handle: task list handle
*                pNode: node config parameter
*                pInRect: input bitmap zone
*                pOutRect: output bitmap zone
*                bDeflicker: if deflicker
*                pstFilterOpt: filter config parameter
* Output:        none
* Return:        return slice number
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiSetFilterChildNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode, TDE2_RECT_S* pInRect,
                                       TDE2_RECT_S*  pOutRect,
                                       TDE2_DEFLICKER_MODE_E enDeflickerMode,
                                       TDE_FILTER_OPT* pstFilterOpt)
{


    TDE_SLICE_INFO_S* sliceInfo = HI_NULL;
    TDE_CHILD_INFO stChildInfo = {0};
    HI_U32 u32SliceNum = 1;
    HI_BOOL bDeflicker = (TDE2_DEFLICKER_MODE_NONE == enDeflickerMode)?HI_FALSE:HI_TRUE;
    HI_U32 u32MaxSliceNum =
           TDE_CALC_MAX_SLICE_NUM(bDeflicker, pstFilterOpt->u32VStep, pInRect->u32Width);
    HI_U32 u32FirstNum;
    HI_S32 s32Ret;
    HI_S32 i;

    HI_BOOL bOneSliceBlockForFilter;
    HI_BOOL bNoLapSlickBlockForFilter;
    HI_BOOL bOverLapSlickBlockForFilter;

    TDE_TRACE(TDE_KERN_DEBUG, "slice calc u32MaxSliceNum: %d!\n", u32MaxSliceNum);
    #ifndef HI_BUILD_IN_BOOT
    sliceInfo =(TDE_SLICE_INFO_S*)HI_GFX_KMALLOC(HIGFX_TDE_ID,u32MaxSliceNum * sizeof(TDE_SLICE_INFO_S),GFP_KERNEL);
    #else
    sliceInfo = (TDE_SLICE_INFO_S*)HI_GFX_Malloc(u32MaxSliceNum * sizeof(TDE_SLICE_INFO_S),"TDE_SLICE_INFO");
    #endif
    if (HI_NULL == sliceInfo)
    {
        TdeHalFreeNodeBuf(pNode);
        return HI_ERR_TDE_NO_MEM;
    }

    HI_GFX_Memset(sliceInfo, 0, u32MaxSliceNum * sizeof(TDE_SLICE_INFO_S));

    /* set child node zoom mode */
    HI_GFX_Memcpy(&stChildInfo.stAdjInfo, &pstFilterOpt->stAdjInfo, sizeof(TDE_MBSTART_ADJ_INFO_S));
    HI_GFX_Memcpy(&stChildInfo.stDSAdjInfo, &pstFilterOpt->stDSAdjInfo, sizeof(TDE_DOUBLESRC_ADJ_INFO_S));
    /*  u32ChildUpdate :
     * _________________________________________
     * |    |    |    |    |    |    |    |    |
     * | ...| 0  | 0  | 1  | 1  | 1  | 1  |  1 |
     * |____|____|____|____|____|____|____|____|
     *                   |    |    |    |    |
     *                  \/   \/   \/   \/   \/
     *                u32Wo u32Xo HOfst u32Wi u32Xi
     *                u32Ho u32Yo VOfst u32Hi u32Yi
     */

    /* if block/how to block */
    /* only zoom for horizontal, not do deflicker and don't zoom when vertical zoom */
    bOneSliceBlockForFilter = ((HI_FALSE == bDeflicker)
        && (TDE_NO_SCALE_STEP == pstFilterOpt->u32VStep));

    bNoLapSlickBlockForFilter = ((TDE_NO_SCALE_STEP == pstFilterOpt->u32HStep)
             && ((bDeflicker) || (TDE_NO_SCALE_STEP != pstFilterOpt->u32VStep)));

    bOverLapSlickBlockForFilter = ((TDE_NO_SCALE_STEP != pstFilterOpt->u32HStep)
             && ((bDeflicker) || (TDE_NO_SCALE_STEP != pstFilterOpt->u32VStep)));

    if (bOneSliceBlockForFilter)
    {
        TDE_TRACE(TDE_KERN_DEBUG, "one slice block for filter!\n");
        u32SliceNum = 1;
        sliceInfo[0].s32SliceHOfst = pstFilterOpt->s32HOffset;
        sliceInfo[0].u32SliceWi = pInRect->u32Width;
        sliceInfo[0].u32SliceWo = pOutRect->u32Width;
        sliceInfo[0].u32SliceXi = pInRect->s32Xpos;
        sliceInfo[0].u32SliceXo = pOutRect->s32Xpos;
        sliceInfo[0].enSliceType = TDE_NO_BLOCK_SLICE_TYPE;
        stChildInfo.u64Update = 0x1F;
    }
    /* donot horizonal zoom, do deflicker or use nonoverlap blocj when vertical zoom */
    else if (bNoLapSlickBlockForFilter)
    {
        TDE_TRACE(TDE_KERN_DEBUG, "NoLap slice block for filter!\n");
        u32SliceNum = TdeOsiCalcNoLapSlice(sliceInfo, u32MaxSliceNum, (HI_U32)(pInRect->s32Xpos),
                                           (HI_U32)(pOutRect->s32Xpos),
                                           pInRect->u32Width, pstFilterOpt);
        stChildInfo.u64Update = 0x1B;

    }
    /* do horizontal zoom, and do vertical zoom or overlap block when  deflicker */
    else if (bOverLapSlickBlockForFilter)
    {
        TDE_TRACE(TDE_KERN_DEBUG, "OverLap slice block for filter!\n");
        u32SliceNum = TdeOsiCalcOverLapSlice(sliceInfo, u32MaxSliceNum, pInRect->s32Xpos,
                                             pInRect->u32Width, (HI_U32)pOutRect->s32Xpos,
                                             pOutRect->u32Width, pstFilterOpt);
        TDE_TRACE(TDE_KERN_DEBUG, "slice real u32SliceNum: %d!\n", u32SliceNum);
        stChildInfo.u64Update = 0x1F;
    }

    /* set filter parameter information of parent node */
    if(TdeHalNodeSetResize(pNode, pstFilterOpt,TDE_NODE_SUBM_PARENT)<0)
    {
    #ifndef HI_BUILD_IN_BOOT
        HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
    #else
        HI_GFX_Free((HI_CHAR*)sliceInfo);
    #endif
        TdeHalFreeNodeBuf(pNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if (bDeflicker)
    {
        if(TdeOsiSetDeflickerPara(pNode, enDeflickerMode, pstFilterOpt)<0)
        {
        #ifndef HI_BUILD_IN_BOOT
            HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
        #else
        	HI_GFX_Free((HI_CHAR*)sliceInfo);
        #endif
            TdeHalFreeNodeBuf(pNode);
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }

    u32FirstNum = ((TDE_SCAN_RIGHT_LEFT == pstFilterOpt->stDstDire.enHScan) || (pInRect->u32Width < pOutRect->u32Width)) ? \
                  u32SliceNum - 1 : 0;

    stChildInfo.u32Xi = sliceInfo[u32FirstNum].u32SliceXi;
    stChildInfo.u32Yi = (HI_U32)pInRect->s32Ypos;
    stChildInfo.u32Wi = sliceInfo[u32FirstNum].u32SliceWi;
    stChildInfo.u32Hi = pInRect->u32Height;
    stChildInfo.u32HOfst = sliceInfo[u32FirstNum].s32SliceHOfst;
    stChildInfo.u32VOfst = pstFilterOpt->s32VOffset;
    stChildInfo.u32Xo = sliceInfo[u32FirstNum].u32SliceXo;
    stChildInfo.u32Yo = (HI_U32)pOutRect->s32Ypos;
    stChildInfo.u32Wo = sliceInfo[u32FirstNum].u32SliceWo;
    stChildInfo.u32Ho = pOutRect->u32Height;
    stChildInfo.enSliceType = sliceInfo[u32FirstNum].enSliceType;
    TdeHalNodeAddChild(pNode, &stChildInfo);

    /* First set  parent node finished */
    s32Ret = TdeOsiSetNodeFinish(s32Handle, pNode, pstFilterOpt->u32WorkBufNum, TDE_NODE_SUBM_PARENT);
    if (HI_SUCCESS != s32Ret)
    {
     #ifndef HI_BUILD_IN_BOOT
        HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
    #else
    	HI_GFX_Free((HI_CHAR*)sliceInfo);
    #endif
        TdeHalFreeNodeBuf(pNode);
        return s32Ret;
    }
    /* accoding to each child node information, set register.
    the first is set as parent node,so begin form the second node */
    if (0 == u32FirstNum)    /* from left to right */
    {
        for (i = u32FirstNum + 1; i < (HI_S32)u32SliceNum; i++)
        {
            /* AE5D03390: outside software evade zone, clip bug */
            s32Ret = TdeOsiSetChildNodeInfo(s32Handle, sliceInfo, pNode, &stChildInfo, pstFilterOpt,
                                             pInRect,  pOutRect, i);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }
        }
    }
    else     /* from right to left */
    {
        for (i = u32FirstNum - 1; i >= 0; i--)
        {
            /* AE5D03390:outside software evade zone, clip bug */
            s32Ret = TdeOsiSetChildNodeInfo(s32Handle, sliceInfo, pNode, &stChildInfo, pstFilterOpt,
                                             pInRect,  pOutRect, i);

            if (s32Ret != HI_SUCCESS)
            {
                return s32Ret;
            }
        }
    }

    #ifndef HI_BUILD_IN_BOOT
    HI_GFX_KFREE(HIGFX_TDE_ID,sliceInfo);
    #else
    HI_GFX_Free((HI_CHAR*)sliceInfo);
    #endif

    return HI_SUCCESS;
}





/*****************************************************************************
* Function:      TdeOsiSetFilterChildNode
* Description:   calculate child nodes of slice and add into task list
* Input:         s32Handle: task list handle
*                pNode: node config parameter
*                pInRect: input bitmap zone
*                pOutRect: output bitmap zone
*                bDeflicker: if deflicker
*                pstFilterOpt: filter config parameter
* Output:        none
* Return:        return slice number
* Others:        none
*****************************************************************************/



typedef struct tagUpdateConfig
{
    HI_S32 ori_in_width; //original image width
    HI_S32 ori_in_height; //original image height
    HI_S32 zme_out_width; //output full image width
    HI_S32 zme_out_height; //output full image height

    HI_S32 update_instart_w; //the start_x of update area in original image
    HI_S32 update_instart_h; //the start_y of update area in original image
    HI_S32 update_in_width;  //the width of update area in original image
    HI_S32 update_in_height;  //the height of update area in original image
}UpdateConfig;

typedef struct tagUpdateInfo
{
    HI_S32 zme_instart_w;  //the start_x of needed readin area in original image
    HI_S32 zme_instart_h;  //the start_y of needed readin area in original image
    HI_S32 zme_in_width;   //the width of needed readin area in original image
    HI_S32 zme_in_height;  //the height of needed readin area in original image

    HI_S32 zme_outstart_w; //the start_x of needed update area in output image
    HI_S32 zme_outstart_h; //the start_y of needed update area in output image
    HI_S32 zme_out_width;  //the width of needed update area in output image
    HI_S32 zme_out_height; //the height of needed update area in output image

    HI_S32 zme_hphase; //the start phase of horizontal scale
    HI_S32 zme_vphase; //the start phase of vertical scale
    HI_S32 def_offsetup; //the up offset of deflicker
    HI_S32 def_offsetdown; //the down offset of deflicker
}UpdateInfo;

#ifndef HI_BUILD_IN_BOOT/**codecc**/
STATIC void TdeOsiGetHUpdateInfo(UpdateConfig *reg, UpdateInfo *info, int scaler_en)
{
    HI_S32 zme_hinstart=0, zme_hinstop=0;
    HI_S32 zme_houtstart=0, zme_houtstop=0;
    HI_S32 update_hstart=reg->update_instart_w;
    HI_S32 update_hstop=update_hstart+reg->update_in_width-1;
    HI_S32 zme_hphase=0;
    HI_S32 ratio=(HI_S32)(4096*(reg->ori_in_width-1)/(reg->zme_out_width-1)+1/2);
    HI_S32 dratio=4096*(reg->zme_out_width-1)/(reg->ori_in_width-1);
    TDE_TRACE(TDE_KERN_DEBUG, "update_start:%d, update_hstop:%d, ori_in_width:%d\n", \
     update_hstart, update_hstop, reg->ori_in_width);

    if(1 != scaler_en)  /*hor_scaler not enable*/
    {
        info->zme_instart_w = reg->update_instart_w;
        info->zme_outstart_w = reg->update_instart_w;
        info->zme_in_width = reg->update_in_width;
        info->zme_out_width = reg->update_in_width;
        info->zme_hphase = 0;

        return;
    }

    //hor_scaler enable
    if(update_hstart>=0 && update_hstart<4)
    {
        /*update outstretched area exceed left limit bordline*/
    	zme_hinstart = 0;
    	zme_houtstart = 0;
    	zme_hphase = 0;
    }
    else
    {
        /*update outstretched area didn't exceed the left limit bordline*/
    	zme_hinstart = (update_hstart-4)*dratio;
    	zme_houtstart = (zme_hinstart%4096)==0 ? (zme_hinstart>>12): ((zme_hinstart>>12)+1);
    	zme_hinstart = zme_houtstart*ratio>>12;
    	if(zme_hinstart-3<0)
    	{
            /*the left few point need mirror pixels when scale*/
            zme_hphase = (zme_houtstart*ratio)%4096+zme_hinstart*4096;
    	    zme_hinstart = 0;
    	}
    	else
    	{
            /*the left few point not need mirror pixels when scale*/
            zme_hphase = (zme_houtstart*ratio)%4096+3*4096;
    		zme_hinstart = zme_hinstart - 3;
    	}
    }

    if( update_hstop>(reg->ori_in_width-4) && update_hstop<reg->ori_in_width)
    {
        /*update outstretched area exceed the right limit bordline*/
    	zme_hinstop = reg->ori_in_width - 1;
    	zme_houtstop = reg->zme_out_width - 1;
    }
    else
    {
        /*update outstretched area didn't exceed the right limit bordline*/
    	zme_hinstop = (update_hstop+3+1)*dratio;
    	zme_houtstop = (zme_hinstop%4096)==0 ? ((zme_hinstop>>12)-1): (zme_hinstop>>12);
    	zme_hinstop = zme_houtstop*ratio>>12;
    	if(zme_hinstop+4>(reg->ori_in_width-1))
    	{
            /*the right few point need mirror pixels when scale*/
    	    zme_hinstop = reg->ori_in_width - 1;
    	}
    	else
    	{
            /*the right few point need mirror pixels when scale*/
    	    zme_hinstop = zme_hinstop + 4;
    	}
    }

    info->zme_instart_w = zme_hinstart;
    info->zme_outstart_w = zme_houtstart;
    info->zme_in_width = zme_hinstop - zme_hinstart +1;
    info->zme_out_width = zme_houtstop - zme_houtstart +1;
    info->zme_hphase = zme_hphase;

    return ;
}
STATIC void TdeOsiGetDeflickerEnVUpdateInfo(UpdateConfig *reg,UpdateInfo *info,HI_S32 update_vstart,HI_S32 update_vstop)
{
	HI_S32 zme_vinstart=0, zme_vinstop=0;
	HI_S32 zme_voutstart=0, zme_voutstop=0;

    /*ver_scale not enable & deflicker enable*/
    if(update_vstart<2)
    {
        /*the update outstreatched area exceed the up limit bordline*/
        zme_vinstart = 0;
        zme_voutstart = 0;
        info->def_offsetup = 0;
    }
    else
    {
        /*the update outstreatched area didn't exceed the up limit bordline*/
        zme_vinstart = update_vstart - 2;
        zme_voutstart = reg->update_instart_h -1;
        info->def_offsetup = 1;
    }

    if(update_vstop>(reg->ori_in_height-3))
    {
        /*the update outstreatched area exceed the down limit bordline*/
        zme_vinstop = reg->ori_in_height - 1;
        zme_voutstop = reg->ori_in_height - 1;
        info->def_offsetdown = 0;
    }
    else
    {
        /*the update outstreatched area didn't exceed the down limit bordline*/
        zme_vinstop = update_vstop + 2;
        zme_voutstop = zme_vinstop - 1;
        info->def_offsetdown = 1;
    }

    info->zme_in_height = zme_vinstop - zme_vinstart +1;
    info->zme_instart_h = zme_vinstart;
    info->zme_outstart_h = zme_voutstart;
    info->zme_out_height = zme_voutstop - zme_voutstart + 1;
    info->zme_vphase = 0;

    return;
}
STATIC void TdeOsiGetScalerEnVUpdateInfo(UpdateConfig *reg,UpdateInfo *info,HI_S32 update_vstart,
                                                     HI_S32 update_vstop,HI_S32 ratio,HI_S32 dratio )
{
	HI_S32 zme_vinstart=0, zme_vinstop=0;
	HI_S32 zme_voutstart=0, zme_voutstop=0;
    HI_S32 zme_vphase=0;

    /*ver_scale enable & deflicker not enable*/
    if(update_vstart>=0 && update_vstart<2)
    {
        /*the update outstreatched area exceed the up limit bordline*/
        zme_vinstart = 0;
        zme_voutstart = 0;
        zme_vphase = 0;
    }
    else
    {
        /*the update outstreatched area didn't exceed the up limit bordline*/
        zme_vinstart = (update_vstart-2)*dratio;
        zme_voutstart = (zme_vinstart%4096)==0 ? (zme_vinstart>>12): ((zme_vinstart>>12)+1);
        zme_vinstart = zme_voutstart*ratio>>12;
        if(zme_vinstart-2<0)
        {
            /*the up few point need mirror pixels when scale*/
            zme_vphase = (zme_voutstart*ratio)%4096+zme_vinstart*4096;
            zme_vinstart = 0;
        }
        else
        {
            /*the up few point not need mirror pixels when scale*/
            zme_vphase = (zme_voutstart*ratio)%4096+2*4096;
            zme_vinstart = zme_vinstart - 2;
        }
    }

    if(update_vstop>(reg->ori_in_height-3) && update_vstop<reg->ori_in_height)
    {
        /*the update outstreatched area exceed the down limit bordline*/
        zme_vinstop = reg->ori_in_height - 1;
        zme_voutstop = reg->zme_out_height - 1;
        TDE_TRACE(TDE_KERN_DEBUG, "update_vstop:%d zme_voutstop:%d\n", update_vstop, zme_voutstop);
    }
    else
    {
        /*the update outstreatched area didn't exceed the down limit bordline*/
        zme_vinstop = (update_vstop+2+1)*dratio;
        zme_voutstop = (zme_vinstop%4096)==0 ? (zme_vinstop>>12): (zme_vinstop>>12) + 1;
        TDE_TRACE(TDE_KERN_DEBUG, "update_vstop:%d zme_voutstop:%d\n", update_vstop, zme_voutstop);
        zme_vinstop = zme_voutstop*ratio>>12;
        if(zme_vinstop+2>(reg->ori_in_height-1))
        {
            /*the down few point need mirror pixels when scale*/
            zme_vinstop = reg->ori_in_height - 1;
        }
        else
        {
            /*the down few point not need mirror pixels when scale*/
            zme_vinstop = zme_vinstop + 2;
        }
    }

    info->zme_in_height = zme_vinstop - zme_vinstart +1;
    info->zme_instart_h = zme_vinstart;
    TDE_TRACE(TDE_KERN_DEBUG, "zme_voutstart:%d, zme_voutstop:%d\n", zme_voutstart, zme_voutstop);
    info->zme_outstart_h = zme_voutstart;
    info->zme_out_height = zme_voutstop - zme_voutstart +1;
    info->zme_vphase = zme_vphase;
    info->def_offsetup = 0;
    info->def_offsetdown = 0;

return;
}
STATIC void TdeOsiGetBothEnVUpdateInfo(UpdateConfig *reg,UpdateInfo *info,HI_S32 update_vstart,
                                                     HI_S32 update_vstop,HI_S32 ratio,HI_S32 dratio )
{
    HI_S32 zme_vinstart=0, zme_vinstop=0;
	HI_S32 zme_voutstart=0, zme_voutstop=0;
    HI_S32 zme_vphase=0;

    /*ver_scale enable & deflicker enable*/
     if(update_vstart>=0 && update_vstart<=2)
     {
         /*the update outstreatched area exceed the up limit bordline*/
         zme_vinstart = 0;
         zme_voutstart = 0;
         zme_vphase = 0;
         info->def_offsetup = 0;
     }
     else
     {
         /*the update outstreatched area didn't exceed the up limit bordline*/
         zme_vinstart = (update_vstart-2)*dratio;
         zme_voutstart = ((zme_vinstart%4096)==0 ? (zme_vinstart>>12): ((zme_vinstart>>12)+1)) - 1;
         if(zme_voutstart<=1)
         {
             /*the update outstreatched deflicker area exceed the up limit bordline*/
             zme_vinstart = 0;
             zme_vphase = 0;
             info->def_offsetup = (zme_voutstart==0)?0: 1;
         }
         else
         {
             /*the update outstreatched deflicker area didn't exceed the up limit bordline*/
             zme_vinstart = (zme_voutstart-1)*ratio>>12;
             if(zme_vinstart<2)
             {
                 /*the up few point need mirror pixels when scale*/
                 zme_vphase = ( (zme_voutstart-1)*ratio )%4096+zme_vinstart*4096;
                 zme_vinstart = 0;
             }
             else
             {
                 /*the up few point not need mirror pixels when scale*/
                 zme_vphase = ( (zme_voutstart-1)*ratio )%4096+2*4096;
                 zme_vinstart = zme_vinstart - 2;
             }
             info->def_offsetup = 1;
         }
     }

     if(update_vstop>(reg->ori_in_height-3) && update_vstop<reg->ori_in_height)
     {
         /*the update outstreatched area exceed the down limit bordline*/
         zme_vinstop = reg->ori_in_height - 1;
         zme_voutstop = reg->zme_out_height - 1;
         info->def_offsetdown = 0;
     }
     else
     {
         /*the update outstreatched area didn't exceed the down limit bordline*/
         zme_vinstop = (update_vstop+2+1)*dratio;
         zme_voutstop = ((zme_vinstop%4096)==0 ? (zme_vinstop>>12): (zme_vinstop>>12)) + 2;
         if(zme_voutstop>(reg->zme_out_height-3))
         {
             /*the update outstreatched deflicker area exceed the down limit bordline*/
             zme_vinstop = reg->ori_in_height - 1;
             info->def_offsetdown = (zme_voutstop>=(reg->zme_out_height-1))?0: 1;
         }
         else
         {
             zme_vinstop = (zme_voutstop+1)*ratio>>12;
             zme_vinstop = (zme_vinstop>(reg->ori_in_height-3))?(reg->ori_in_height-1): (zme_vinstop + 2);
             info->def_offsetdown = 1;
         }

         if(zme_voutstop>=reg->zme_out_height)
         {
             zme_voutstop = reg->zme_out_height -1;
         }
     }


     info->zme_instart_h = zme_vinstart;
     info->zme_outstart_h = zme_voutstart;
     info->zme_in_height = zme_vinstop - zme_vinstart +1;
     info->zme_out_height = zme_voutstop - zme_voutstart +1;
     info->zme_vphase = zme_vphase;

     return;
}
STATIC void TdeOsiGetVUpdateInfo(UpdateConfig *reg, UpdateInfo *info, int scaler_en, int deflicker_en)
{
    HI_BOOL bDeflickerEn;
    HI_BOOL bScalerEn;
    HI_BOOL bBothDeflickerAndScalerEn;

	HI_S32 update_vstart=reg->update_instart_h;
	HI_S32 update_vstop=update_vstart+reg->update_in_height-1;

	HI_S32 ratio=(HI_S32)(4096*(reg->ori_in_height-1)/(reg->zme_out_height-1)+1/2);
	HI_S32 dratio=4096*(reg->zme_out_height-1)/(reg->ori_in_height-1);
	TDE_TRACE(TDE_KERN_DEBUG, "update_vstart:%d, update_vstop:%d, ori_in_height:%d, ratio:%d£¬dratio:%d\n",\
	update_vstart, update_vstop, reg->ori_in_height, ratio, dratio);

    bDeflickerEn = (scaler_en==0 && deflicker_en==1);
    bScalerEn = (scaler_en==1 && deflicker_en==0);
    bBothDeflickerAndScalerEn = (scaler_en==1 && deflicker_en==1);

	if (bDeflickerEn)
	{
        TdeOsiGetDeflickerEnVUpdateInfo( reg, info, update_vstart, update_vstop);
	}
	else if (bScalerEn)
	{
        TdeOsiGetScalerEnVUpdateInfo( reg, info, update_vstart, update_vstop, ratio, dratio);
	}
	else if (bBothDeflickerAndScalerEn)
	{
	    TdeOsiGetBothEnVUpdateInfo( reg, info, update_vstart, update_vstop, ratio, dratio);
 	}
	else
	{
		info->zme_instart_h = reg->update_instart_h;
		info->zme_in_height = reg->update_in_height;
		info->zme_outstart_h = reg->update_instart_h;
		info->zme_out_height = reg->update_in_height;
		info->zme_vphase = 0;
		info->def_offsetup = 0;
		info->def_offsetdown = 0;
	}

}
#endif


/*****************************************************************************
* Function:      TdeOsiSetFilterNode
* Description:   do deflicker(include VF/HF/FF)
* Input:         s32Handle:Job head node pointer of needing operate
*                pNode: set parent node fliter parameter information
*                pInSurface: input bitmap information
*                pOutSurface: output bitmap information
*                bDeflicker: if deflicker
*                pstOpt: config option of filter operate needings
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiSetFilterNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode, TDE2_SURFACE_S* pstForeGround, TDE2_SURFACE_S* pstDst,
                                  TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect, TDE2_DEFLICKER_MODE_E enDeflickerMode, TDE_FILTER_OPT* pstFilterOpt)
{
    TDE2_COLOR_FMT_E enInFmt;
    TDE2_COLOR_FMT_E enOutFmt;


#ifndef HI_BUILD_IN_BOOT
    UpdateConfig reg;
    UpdateInfo info;
    TDE2_RECT_S stUpdateInRect;
    TDE2_RECT_S stUpdateOutRect;
    HI_BOOL bDeflicker;
    HI_BOOL bScale = HI_FALSE;
    HI_BOOL bLocalDeflicker;
#endif
    enInFmt = pstForeGround->enColorFmt;
    enOutFmt = pstDst->enColorFmt;

#ifndef HI_BUILD_IN_BOOT
    bDeflicker = (TDE2_DEFLICKER_MODE_NONE == enDeflickerMode) ? HI_FALSE : HI_TRUE;
#endif

    /* calculate vertical/horizontal step */

    pstFilterOpt->u32HStep = (1 >= pOutRect->u32Width) ? 0 : ((pInRect->u32Width - 1) << TDE_FLOAT_BITLEN) / (pOutRect->u32Width - 1);

    pstFilterOpt->u32VStep = (1 >= pOutRect->u32Height) ? 0 : ((pInRect->u32Height - 1) << TDE_FLOAT_BITLEN) / (pOutRect->u32Height - 1);

    pstFilterOpt->s32HOffset = 0;
    pstFilterOpt->s32VOffset = 0;
    pstFilterOpt->stAdjInfo.enScaleMode = TDE_CHILD_SCALE_NORM;
    pstFilterOpt->u32Bppi = TdeOsiGetbppByFmt(enInFmt);
    if (TDE2_COLOR_FMT_YCbCr422 == enInFmt || TDE2_COLOR_FMT_YCbCr422 == enOutFmt)
    {
        pstFilterOpt->bEvenStartInX = HI_TRUE; /* YCbCr422 ask for start width is even of each slice */
        pstFilterOpt->bBadLastPix = HI_TRUE;  /* YCbCr422 ask for conver 2 noneffective pixel points for each slice when blocking */
    }
    if (TDE2_COLOR_FMT_YCbCr422 == enOutFmt)
    {
        pstFilterOpt->bEvenStartOutX = HI_TRUE;
        pstFilterOpt->bBadLastPix = HI_TRUE;   /* YCbCr422 ask for conver 2 noneffective pixel points for each slice when blocking  */
    }


    /* judge middle value filter is enable, enable manner is judged by arithmetic team */
    if (TDE_NO_SCALE_STEP > pstFilterOpt->u32VStep)
    {
        pstFilterOpt->bVRing = HI_TRUE;
    }

    if (TDE_NO_SCALE_STEP > pstFilterOpt->u32HStep)
    {
        pstFilterOpt->bHRing = HI_TRUE;
    }

    pstFilterOpt->bCoefSym = HI_TRUE; /* arithmetic suply field, keeping open asymmetry */

	#ifndef HI_BUILD_IN_BOOT/**codecc**/
    reg.ori_in_height = pstForeGround->u32Height;
    reg.ori_in_width = pstForeGround->u32Width;
    reg.zme_out_height = pstDst->u32Height;
    reg.zme_out_width = pstDst->u32Width;
    reg.update_instart_w = pInRect->s32Xpos;
    reg.update_instart_h = pInRect->s32Ypos;
    reg.update_in_width = pInRect->u32Width;
    reg.update_in_height = pInRect->u32Height;

    TDE_TRACE(TDE_KERN_DEBUG, "InHeight:%d, InWidth:%d, OutHeight:%d, OutWidth:%d,UpdateX:%d, UpdateY:%d,\
        UpdateW:%d, UpdateH:%d\n", reg.ori_in_height, reg.ori_in_width, reg.zme_out_height, reg.zme_out_width, \
              reg.update_instart_w, reg.update_instart_h, reg.update_in_width, reg.update_in_height);

    /* local deflicker */
    bLocalDeflicker = (((pstForeGround->u32Width != pInRect->u32Width) || (pstForeGround->u32Height != pInRect->u32Height))
        && s_bRegionDeflicker);

    if (bLocalDeflicker)
    {

        pstFilterOpt->u32HStep = (1 >= pstDst->u32Width) ? 0 : ((pstForeGround->u32Width - 1) << TDE_FLOAT_BITLEN) / (pstDst->u32Width - 1);

        pstFilterOpt->u32VStep = (1 >= pstDst->u32Height) ? 0 : ((pstForeGround->u32Height - 1) << TDE_FLOAT_BITLEN) / (pstDst->u32Height - 1);


        if (TDE_NO_SCALE_STEP > pstFilterOpt->u32VStep)
        {
            pstFilterOpt->bVRing = HI_TRUE;
        }

        if (TDE_NO_SCALE_STEP > pstFilterOpt->u32HStep)
        {
            pstFilterOpt->bHRing = HI_TRUE;
        }

        if (TDE_NO_SCALE_STEP != pstFilterOpt->u32HStep)
        {
            bScale = HI_TRUE;
        }

        TDE_TRACE(TDE_KERN_DEBUG, "bHScale:%x\n", bScale);
        TdeOsiGetHUpdateInfo(&reg, &info, bScale);

        bScale = HI_FALSE;
        if (TDE_NO_SCALE_STEP != pstFilterOpt->u32VStep)
        {
            bScale = HI_TRUE;
        }
        TDE_TRACE(TDE_KERN_DEBUG, "bVScale:%x\n", bScale);
        TdeOsiGetVUpdateInfo(&reg, &info, bScale, bDeflicker);
        TDE_TRACE(TDE_KERN_DEBUG, "zme_instart_w:%d, zme_instart_h:%d, zme_in_width:%d, \
            zme_in_height:%d, zme_outstart_w:%d, zme_outstart_h:%d, zme_out_width:%d, \
            zme_out_height:%d, zme_hphase:%d, zme_vphase:%d, def_offsetup:%d, def_offsetdown:%d\n",
            info.zme_instart_w, info.zme_instart_h, info.zme_in_width, info.zme_in_height, \
            info.zme_outstart_w, info.zme_outstart_h, info.zme_out_width, info.zme_out_height, \
            info.zme_hphase, info.zme_vphase, info.def_offsetup, info.def_offsetdown);

        stUpdateInRect.s32Xpos = info.zme_instart_w;
        stUpdateInRect.s32Ypos = info.zme_instart_h;
        stUpdateInRect.u32Width = info.zme_in_width;
        stUpdateInRect.u32Height = info.zme_in_height;

        stUpdateOutRect.s32Xpos = info.zme_outstart_w;
        stUpdateOutRect.s32Ypos = info.zme_outstart_h;
        stUpdateOutRect.u32Width = info.zme_out_width;
        stUpdateOutRect.u32Height = info.zme_out_height;

        pstFilterOpt->s32HOffset = info.zme_hphase;
        pstFilterOpt->s32VOffset = info.zme_vphase;
        pstFilterOpt->bFirstLineOut = (info.def_offsetup & 0x1);
        pstFilterOpt->bLastLineOut = (info.def_offsetdown & 0x1);

        return TdeOsiSetFilterChildNode(s32Handle, pNode, &stUpdateInRect, &stUpdateOutRect, enDeflickerMode, pstFilterOpt);
    }
    else
    #endif
    {
        return TdeOsiSetFilterChildNode(s32Handle, pNode, pInRect, pOutRect, enDeflickerMode, pstFilterOpt);
    }

}

STATIC INLINE HI_VOID TdeOsiMbFilterSetStep(TDE_FILTER_OPT* pstFilterOpt, TDE2_MB_COLOR_FMT_E enInFmt,
        TDE2_RECT_S* pInRect, HI_BOOL bCbCr, HI_BOOL bCus,
        TDE2_MB_COLOR_FMT_E enOutFmt, TDE2_RECT_S*  pOutRect)
{
    HI_BOOL bCbcrAndCus = (bCbCr && bCus);

    /* calculate vertiacal/ horizontal stride and offset, brightness step calculate need its width and height */
    if (bCbcrAndCus)    /* chroma sample need calculate step by indepedant */
    {
        switch (enInFmt)
        {
            case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP:
                pstFilterOpt->u32HStep = (1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 */
                pstFilterOpt->u32VStep = TDE_NO_SCALE_STEP;    /* 1 */
                break;

            case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP:
                pstFilterOpt->u32HStep = TDE_NO_SCALE_STEP;    /* 1 */
                pstFilterOpt->u32VStep = (1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 */
                break;

            case TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP:
            case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP:
            case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI:
            case TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP:
                pstFilterOpt->u32HStep = (1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 */
                pstFilterOpt->u32VStep = (1 << (TDE_FLOAT_BITLEN - 1));    /* 0.5 */
                break;

            default:
                pstFilterOpt->u32HStep = TDE_NO_SCALE_STEP;    /* 1 */
                pstFilterOpt->u32VStep = TDE_NO_SCALE_STEP;    /* 1 */
                break;
        }
    }
    else
    {
        if (pstFilterOpt->b2OptCbCr)
        {
            pstFilterOpt->u32HStep = TdeOsiMbCalcHStep(pInRect->u32Width, pOutRect->u32Width, enInFmt, enOutFmt, HI_FALSE);
            pstFilterOpt->u32VStep = TdeOsiMbCalcVStep(pInRect->u32Height, pOutRect->u32Height, enInFmt, enOutFmt, HI_FALSE);
        }
        else
        {
            pstFilterOpt->u32HStep = TdeOsiMbCalcHStep(pInRect->u32Width, pOutRect->u32Width, enInFmt, enOutFmt, bCbCr);
            pstFilterOpt->u32VStep = TdeOsiMbCalcVStep(pInRect->u32Height, pOutRect->u32Height, enInFmt, enOutFmt, bCbCr);
        }
    }

    return;
}

/*****************************************************************************
* Function:      TdeOsiCalcMbFilterOpt
* Description:   calculate filter coefficient when mb blit
* Input:         pstFilterOpt: filter coefficient struct
*                enInFmt: input bitmap format info
*                pInRect: input bitmap operate zone
*                pOutRect: output bitmap operate zone
*                bCbCr: if chroma operate
*                bCus:  if sample in chroma
*                enPicMode: bitmap operate  mode: frame/ top filed/ bottom field
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_VOID TdeOsiCalcMbFilterOpt(TDE_FILTER_OPT* pstFilterOpt, TDE2_MB_COLOR_FMT_E enInFmt, TDE2_MB_COLOR_FMT_E enOutFmt,
                                            TDE2_RECT_S* pInRect, TDE2_RECT_S*  pOutRect, HI_BOOL bCbCr,
                                            HI_BOOL bCus, TDE_PIC_MODE_E enPicMode)
{
    pstFilterOpt->enFilterMode = TDE_DRV_FILTER_ALL;

    TdeOsiMbFilterSetStep(pstFilterOpt, enInFmt, pInRect, bCbCr, bCus, enOutFmt, pOutRect);

	#ifndef HI_BUILD_IN_BOOT/**codecc**/
    if (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP == enOutFmt && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enInFmt)
    {
	    pstFilterOpt->s32HOffset = TdeOsiMbCalcHOffset(pInRect->s32Xpos, enInFmt, bCbCr);
	    pstFilterOpt->s32VOffset = TdeOsiMbCalcVOffset(pInRect->s32Ypos, enInFmt, enPicMode, bCbCr);
    }
    else if (TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enOutFmt && TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP != enInFmt
             && enOutFmt == enInFmt)
    {
        pstFilterOpt->s32HOffset = 0;
        pstFilterOpt->s32VOffset = 0;
    }
    else
    #endif
    {
        /* other situation do not handle, calculate by 0 */
        pstFilterOpt->s32HOffset = 0;
        pstFilterOpt->s32VOffset = 0;
    }
    #ifndef HI_BUILD_IN_BOOT/**codecc**/
    /* MB format chroma Bpp is half word, brightness Bpp is the width of Byte */
    if (bCbCr)
    {
        pstFilterOpt->u32Bppi = 16;

        /* judge if median filter is enable, enable mannar is supplied by arithmetic team */
        if (TDE_NO_SCALE_STEP > pstFilterOpt->u32VStep)
        {
            pstFilterOpt->bVRing = HI_TRUE;
        }

        if (TDE_NO_SCALE_STEP > pstFilterOpt->u32HStep)
        {
            pstFilterOpt->bHRing = HI_TRUE;
        }
    }
    else
    #endif
    {
        pstFilterOpt->u32Bppi = 8;

        /*  judge if median filter is enable, enable mannar is supplied by arithmetic team  */
        if (TDE_NO_SCALE_STEP > pstFilterOpt->u32HStep)
        {
            pstFilterOpt->bHRing = HI_TRUE;
        }
    }

    pstFilterOpt->bCoefSym = HI_TRUE;
    pstFilterOpt->enFilterMode = TDE_DRV_FILTER_COLOR;
    TDE_TRACE(TDE_KERN_DEBUG, "\n");
}

/*****************************************************************************
* Function:      TdeOsiAdjPara4YCbCr422R
* Description:   when fill color is YCbCr422R, fill by word
* Input:         pstDst: target bitmap info struct
*                pstDstRect: target operate zone
*                pstFillColor: fill color
*
* Output:        none
* Return:        none
* Others:        AI7D02880 add software fill, when YCbCr422, fill function by word
*****************************************************************************/
STATIC INLINE HI_VOID TdeOsiAdjPara4YCbCr422R(TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor)
{
    if (TDE2_COLOR_FMT_YCbCr422 != pstDst->enColorFmt)
    {
        return;
    }


    pstDst->enColorFmt = TDE2_COLOR_FMT_AYCbCr8888;
    pstDst->bAlphaMax255 = HI_TRUE;
    pstFillColor->enColorFmt = TDE2_COLOR_FMT_AYCbCr8888;


    pstDst->u32Width /= 2;


    pstDstRect->u32Width /= 2;
    pstDstRect->s32Xpos /= 2;


    pstFillColor->u32FillColor = TDE_GET_YC422R_FILLVALUE(pstFillColor->u32FillColor);
}

/*****************************************************************************
* Function:      TdeOsi1SourceFill
* Description:   single source fill operate,source1 is fill color,target bitmap is pstDst,support source1 and fill color do ROP or alpha blending to target bitmap, unsupport mirror,colorkey
*                 src bitmap is not support MB color format
* Input:         s32Handle: task handle
*                pSrc: background bitmap info struct
*                pstDst: foreground bitmap info struct
*                pstFillColor:  target bitmap info struct
*                pstOpt: operate parameter setting struct
* Output:        none
* Return:        HI_SUCCESS/HI_FAILURE
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsi1SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst,
                                TDE2_RECT_S*  pstDstRect, TDE2_FILLCOLOR_S* pstFillColor, TDE2_OPT_S* pstOpt)
{
    TDE_DRV_BASEOPT_MODE_E enBaseMode = {0};
    TDE_DRV_ALU_MODE_E enAluMode = {0};
    TDE_HWNode_S* pstHWNode = NULL;
    TDE_DRV_SURFACE_S stDrvSurface = {0};
    TDE_DRV_COLORFILL_S stDrvColorFill = {0};
    TDE_SCANDIRECTION_S stScanInfo = {0};
    TDE_DRV_OUTALPHA_FROM_E enOutAlphaFrom = TDE2_OUTALPHA_FROM_NORM;
    TDE2_RECT_S stDstRect = {0};
    HI_S32 s32Ret;

#ifndef HI_BUILD_IN_BOOT
    HI_BOOL bOutAlphaFromError;
#endif
    HI_BOOL bContainNullPtr = ((HI_NULL == pstDst) || (HI_NULL == pstDstRect) || (HI_NULL == pstFillColor));

    if (bContainNullPtr)
    {
        TDE_TRACE(TDE_KERN_INFO, "NULL pointer!\n");
        return HI_ERR_TDE_NULL_PTR;
    }

    HI_GFX_Memcpy(&stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    TDE_CHECK_NOT_MB(pstDst->enColorFmt);
    TDE_CHECK_NOT_MB(pstFillColor->enColorFmt);

    if (TdeOsiCheckSurface(pstDst, &stDstRect) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }


    TDE_CHECK_SUBBYTE_STARTX(stDstRect.s32Xpos, stDstRect.u32Width, pstDst->enColorFmt);

    TdeOsiAdjPara4YCbCr422R(pstDst, &stDstRect, pstFillColor);

    if (TdeHalNodeInitNd(&pstHWNode) < 0)
    {
        return HI_ERR_TDE_NO_MEM;
    }

    enAluMode = TDE_ALU_NONE;

    if (HI_NULL == pstOpt)
    {
        enBaseMode = (pstFillColor->enColorFmt == pstDst->enColorFmt) ? TDE_QUIKE_FILL : TDE_NORM_FILL_1OPT;

        enOutAlphaFrom = TDE2_OUTALPHA_FROM_NORM;
    }

#ifndef HI_BUILD_IN_BOOT
    else
    {
        if (TDE2_ALUCMD_BUTT <= pstOpt->enAluCmd)
        {
            TDE_TRACE(TDE_KERN_INFO, "enAluCmd error!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }

        if (TDE2_ALUCMD_NONE != pstOpt->enAluCmd)
        {
            TDE_TRACE(TDE_KERN_INFO, "invalid alu command!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }

        enBaseMode = (pstFillColor->enColorFmt == pstDst->enColorFmt) ? TDE_QUIKE_FILL : TDE_NORM_FILL_1OPT;


        enOutAlphaFrom = pstOpt->enOutAlphaFrom;
        bOutAlphaFromError = (TDE2_OUTALPHA_FROM_BUTT <= enOutAlphaFrom) || (TDE2_OUTALPHA_FROM_FOREGROUND == enOutAlphaFrom);

        if (bOutAlphaFromError)
        {
            TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }

        TdeHalNodeSetGlobalAlpha(pstHWNode, pstOpt->u8GlobalAlpha, pstOpt->stBlendOpt.bGlobalAlphaEnable);

        if (TdeOsiSetClipPara(NULL, NULL, pstDst, &stDstRect, pstDst, &stDstRect, pstOpt, pstHWNode) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_CLIP_AREA;
        }
    }

#endif/**boot cut by y00181162**/

    if (TDE_NORM_FILL_1OPT == enBaseMode)
    {
        if (TdeOsiColorConvert(pstFillColor, pstDst, &stDrvColorFill.u32FillData) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }

        stDrvColorFill.enDrvColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;
    }
    else
    {
        stDrvColorFill.u32FillData = pstFillColor->u32FillColor;

        stDrvColorFill.enDrvColorFmt = g_enTdeCommonDrvColorFmt[pstFillColor->enColorFmt];
    }

    if(TdeHalNodeSetBaseOperate(pstHWNode, enBaseMode, enAluMode, &stDrvColorFill)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    stScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
    stScanInfo.enVScan = TDE_SCAN_UP_DOWN;

    TdeOsiConvertSurface(pstDst, &stDstRect, &stScanInfo, &stDrvSurface, NULL);

    if (TDE_NORM_FILL_1OPT == enBaseMode)
    {

        TdeHalNodeSetSrc1(pstHWNode, &stDrvSurface);
    }


    TdeHalNodeSetTgt(pstHWNode, &stDrvSurface, enOutAlphaFrom);

    TdeOsiSetExtAlpha(pstDst, NULL, pstHWNode); /* AI7D02681 */

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;
}

STATIC HI_S32 TdeOsiRasterFmtCheckAlign(TDE2_SURFACE_S* pstSurface)
{
    HI_S32 s32Bpp;
    HI_U32 u32BytePerPixel;
    s32Bpp = TdeOsiGetbppByFmt(pstSurface->enColorFmt);

    if ((s32Bpp >= 8) && (s32Bpp != 24))
    {
        u32BytePerPixel = (s32Bpp >> 3);

        TDE_ADDRESS_CHECK_ALIGN(pstSurface->u32PhyAddr,u32BytePerPixel);
        TDE_STRIDE_CHECK_ALIGN(pstSurface->u32Stride,u32BytePerPixel);
    }
    else if (s32Bpp == 24)
    {
        TDE_ADDRESS_CHECK_ALIGN(pstSurface->u32PhyAddr,4);
        TDE_STRIDE_CHECK_ALIGN(pstSurface->u32Stride,4);
    }

    return HI_SUCCESS;
}


STATIC INLINE HI_S32 TdeOsiPreCheckSurface(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S*  pstRect)
{
    HI_S32 s32Ret ;
    HI_BOOL bUnknownColorFmt, bRasterFmt;

    if(0==pstSurface->u32PhyAddr)
    {
        TDE_TRACE(TDE_KERN_INFO, "The surface physical address is NULL!");
        return -1;
    }

    TDE_CHECK_COLORFMT(pstSurface->enColorFmt);

    TDE_CHECK_SURFACEPARA(pstSurface,pstRect);

    bUnknownColorFmt = TDE2_COLOR_FMT_BUTT <= pstSurface->enColorFmt;
    if (bUnknownColorFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "Unkown color format!\n");
        return -1;
    }

    TDE_YCBCR422_FMT_CHECK_ODD(pstSurface->enColorFmt, (HI_U32)pstRect->s32Xpos, pstRect->u32Width);

    bRasterFmt = pstSurface->enColorFmt <= TDE2_COLOR_FMT_halfword ;
    if ( bRasterFmt )
    {
        if ( (s32Ret = TdeOsiRasterFmtCheckAlign(pstSurface)) != HI_SUCCESS )
        {
            return s32Ret;
        }
    }
    else
    {
        TDE_YCBCR_FMT_CHECK_STRIDE(pstSurface->u32CbCrStride, pstSurface->enColorFmt);
    }

    if (pstRect->s32Xpos + pstRect->u32Width > pstSurface->u32Width)
    {
        /* AI7D02547 */
        pstRect->u32Width = pstSurface->u32Width - pstRect->s32Xpos;
    }

    if (pstRect->s32Ypos + pstRect->u32Height > pstSurface->u32Height)
    {
        pstRect->u32Height = pstSurface->u32Height - pstRect->s32Ypos;
    }
    return HI_SUCCESS;
}

#ifndef HI_BUILD_IN_BOOT
STATIC HI_S32 TdeOsiSetForegroundColorKey(TDE_HWNode_S* pstHWNode , TDE2_SURFACE_S* pstSrc, TDE2_OPT_S* pstOpt , TDE_CLUT_USAGE_E enClutUsage)
{
    HI_BOOL bForegroundColorkeyAfterClut;
    TDE_DRV_COLORKEY_CMD_S stColorkey;
    TDE_COLORFMT_CATEGORY_E enFmtCategory;
    stColorkey.unColorKeyValue = pstOpt->unColorKeyValue;
    bForegroundColorkeyAfterClut = (TDE_CLUT_COLOREXPENDING != enClutUsage && TDE_CLUT_CLUT_BYPASS != enClutUsage);

    if (TDE2_COLORKEY_MODE_FOREGROUND != pstOpt->enColorKeyMode)
    {
        if (TDE2_COLORKEY_MODE_BACKGROUND == pstOpt->enColorKeyMode)
        {
            TDE_TRACE(TDE_KERN_INFO, "Unsupported solidraw colorkey in background mode!\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
        }
        else
        {
            return HI_SUCCESS;
        }
    }

    stColorkey.enColorKeyMode = ( bForegroundColorkeyAfterClut ) ?  TDE_DRV_COLORKEY_FOREGROUND_AFTER_CLUT : \
                                TDE_DRV_COLORKEY_FOREGROUND_BEFORE_CLUT;

    enFmtCategory = TdeOsiGetFmtCategory(pstSrc->enColorFmt);

    if (enFmtCategory >= TDE_COLORFMT_CATEGORY_BUTT)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if (TdeHalNodeSetColorKey(pstHWNode, enFmtCategory, &stColorkey) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return HI_SUCCESS;

}
/*****************************************************************************
* Function:      TdeOsi2SourceFill
* Description:   double sources fill operate, source1 is fill color, source2 is pstSrc,bitmap is pstDst,after source2 zoom or deflicker,
*                Rop with fill color or alpha blending to target bitmap, unsupport mirror,colorkey
*                fi src bitmap is mb format, only support single source mode, just set pstBackGround or pstForeGround
* Input:         s32Handle: task handle
*                pSrc: background bitmap info struct
*                pstDst: foreground bitmap info struct
*                pstFillColor:  target bitmap info struct
*                pstOpt:  operate parameter setting struct
* Output:        none
* Return:        HI_SUCCESS/HI_FAILURE
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsi2SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                TDE2_SURFACE_S *pstDst,
                                TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                                TDE2_OPT_S *pstOpt)
{
    TDE_DRV_BASEOPT_MODE_E enBaseMode = TDE_QUIKE_FILL;
    TDE_DRV_ALU_MODE_E enAluMode = TDE_SRC1_BYPASS;
    TDE_HWNode_S* pstHWNode = NULL;
    TDE_DRV_SURFACE_S stDrvSurface = {0};
    TDE_DRV_COLORFILL_S stDrvColorFill = {0};
    TDE_SCANDIRECTION_S stSrcScanInfo = {0};
    TDE_SCANDIRECTION_S stDstScanInfo = {0};
    TDE2_RECT_S stSrcOptArea = {0};
    TDE2_RECT_S stDstOptArea = {0};
    TDE_FILTER_OPT stFilterOpt = {0};
    TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;
    TDE2_RECT_S stSrcRect = {0};
    TDE2_RECT_S stDstRect = {0};
    HI_BOOL bCheckSingleSrc2Rop = HI_FALSE;
    HI_BOOL bCheckBlend = HI_TRUE;
    HI_BOOL bSetFileNode;
    HI_S32 s32Ret = HI_FAILURE;

    HI_BOOL bContainNullPtr = ((NULL == pstDst) || (NULL == pstDstRect) || (NULL == pstFillColor) || (NULL == pstOpt)\
                               || (NULL == pstSrc) || (NULL == pstSrcRect));

    if (bContainNullPtr)
    {
        return HI_ERR_TDE_NULL_PTR;
    }


    HI_GFX_Memcpy(&stSrcRect, pstSrcRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stDstRect, pstDstRect, sizeof(TDE2_RECT_S));


    TDE_CHECK_COLORFMT(pstFillColor->enColorFmt);


    TDE_CHECK_OUTALPHAFROM(pstOpt->enOutAlphaFrom);

    TDE_CHECK_ALUCMD(pstOpt->enAluCmd);

    if (TdeOsiCheckSurface(pstDst, &stDstRect) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (TdeOsiCheckSurface(pstSrc, &stSrcRect) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (!pstOpt->bResize)
    {
        TDE_UNIFY_RECT(&stSrcRect, &stDstRect);
    }

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }
    enBaseMode = TDE_NORM_FILL_2OPT;
    enAluMode = TDE_ALU_NONE;


    s32Ret = TdeOsiSetRop( pstHWNode , pstOpt->enAluCmd, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha , &enAluMode , bCheckSingleSrc2Rop);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }



    s32Ret = TdeOsiSetBlend( pstHWNode ,  pstOpt->enAluCmd, pstOpt->stBlendOpt , &enAluMode , bCheckBlend);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = TdeOsiSetColorize( pstHWNode , pstOpt->enAluCmd, pstOpt->u32Colorize);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    TdeHalNodeSetGlobalAlpha(pstHWNode, pstOpt->u8GlobalAlpha, pstOpt->stBlendOpt.bGlobalAlphaEnable);

    if(TdeOsiSetClipPara(NULL, NULL, pstSrc, &stSrcRect, pstDst, &stDstRect, pstOpt, pstHWNode) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_CLIP_AREA;
    }

    if(TdeOsiColorConvert(pstFillColor, pstSrc, &stDrvColorFill.u32FillData) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }

    stDrvColorFill.enDrvColorFmt = TDE_DRV_COLOR_FMT_ARGB8888;//TDE2_COLOR_FMT_ARGB8888;


    if(TdeHalNodeSetBaseOperate(pstHWNode, enBaseMode, enAluMode, &stDrvColorFill)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    TdeOsiSetExtAlpha(NULL, pstSrc, pstHWNode);


    if (TdeOsiGetScanInfo(pstSrc, &stSrcRect, pstDst, &stDstRect, pstOpt, &stSrcScanInfo,
                          &stDstScanInfo) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }


    TdeOsiConvertSurface(pstSrc, &stSrcRect, &stSrcScanInfo, &stDrvSurface, &stSrcOptArea);


    TdeHalNodeSetSrc1(pstHWNode, &stDrvSurface);


    TdeOsiConvertSurface(pstDst, &stDstRect, &stDstScanInfo, &stDrvSurface, &stDstOptArea);


    TdeHalNodeSetTgt(pstHWNode, &stDrvSurface, pstOpt->enOutAlphaFrom);


    if((s32Ret = TdeOsiSetClutOpt(pstSrc, pstDst, &enClutUsage, pstOpt->bClutReload,pstHWNode)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }


   s32Ret = TdeOsiSetForegroundColorKey( pstHWNode, pstSrc, pstOpt, enClutUsage);

   if (s32Ret != HI_SUCCESS)
   {
       return s32Ret;
    }

    bSetFileNode = ((pstOpt->bResize) || (pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE));

    if (bSetFileNode)
    {

        stFilterOpt.u32WorkBufNum = 0;


        HI_GFX_Memcpy(&stFilterOpt.stSrcDire, &stSrcScanInfo, sizeof(TDE_SCANDIRECTION_S));
        HI_GFX_Memcpy(&stFilterOpt.stDstDire, &stDstScanInfo, sizeof(TDE_SCANDIRECTION_S));

        stFilterOpt.enFilterMode = g_enTdeFilterMode[pstOpt->enFilterMode];

        if ((s32Ret = TdeOsiSetFilterNode(s32Handle, pstHWNode, pstSrc, pstDst,
                                          &stSrcOptArea, &stDstOptArea, pstOpt->enDeflickerMode, &stFilterOpt)) < 0)
        {
            return s32Ret;
        }

        return HI_SUCCESS;
    }

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;

}


/*****************************************************************************
* Function:      TdeOsiCheckSurfaceEX
* Description:   adjust right operate zone, according by the size of bitmap is more than 4095*4095,and less than 8190*8190
* Input:         pstSurface: bitmap info
*                pstRect: bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
INLINE HI_S32 TdeOsiCheckSurfaceEX(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S*  pstRect)
{
    HI_S32 s32Ret ;
    HI_BOOL bInvalidOperationArea;

    if ( ( s32Ret = TdeOsiPreCheckSurface( pstSurface,  pstRect) ) < 0)
    {
        return s32Ret;
    }

    bInvalidOperationArea = ((TDE_MAX_RECT_WIDTH_EX < pstRect->u32Width) || (TDE_MAX_RECT_HEIGHT_EX < pstRect->u32Height));

    if ( bInvalidOperationArea )
    {
        TDE_TRACE(TDE_KERN_INFO, "invalid operation area!");
        return -1;
    }

    return 0;
}

#endif




/*****************************************************************************
* Function:      TdeOsiCheckSurface
* Description:   adjust right operate zone, according by the size of bitmap and operate zone from user upload
* Input:         pstSurface: bitmap info
*                pstRect: bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiCheckSurface(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S  *pstRect)
{
    HI_S32 s32Ret ;
    HI_BOOL bInvalidOperationArea;

    if ( ( s32Ret = TdeOsiPreCheckSurface( pstSurface,  pstRect) )!= HI_SUCCESS)
    {
        return s32Ret;
    }

    bInvalidOperationArea = ((TDE_MAX_RECT_WIDTH < pstRect->u32Width) || (TDE_MAX_RECT_HEIGHT < pstRect->u32Height));

    if ( bInvalidOperationArea )
    {
        TDE_TRACE(TDE_KERN_INFO, "invalid operation area!");
        return -1;
    }

    return 0;
}


#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeOsiCheckMbSurfaceEx
* Description:   adjust right operate zone, according by the size of bitmap is more than 4095*4095 and less than 8190*8190
* Input:         pstSurface: bitmap info
*                pstRect: bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiCheckMbSurfaceEX(TDE2_MB_S* pstMbSurface, TDE2_RECT_S  *pstRect)
{
    TDE_CHECK_MBCOLORFMT(pstMbSurface->enMbFmt);


    TDE_CHECK_MBCBCRPARA(pstMbSurface);

    TDE_CHECK_MBSURFACEPARA(pstMbSurface, pstRect, TDE_MAX_RECT_WIDTH_EX, TDE_MAX_RECT_HEIGHT_EX);


    TDE_STRIDE_CHECK_ALIGN(pstMbSurface->u32YStride, 4);
    TDE_STRIDE_CHECK_ALIGN(pstMbSurface->u32CbCrStride, 4);


    if (pstRect->s32Xpos + pstRect->u32Width > pstMbSurface->u32YWidth)
    {
        pstRect->u32Width = pstMbSurface->u32YWidth - pstRect->s32Xpos;
    }

    if (pstRect->s32Ypos + pstRect->u32Height > pstMbSurface->u32YHeight)
    {
        pstRect->u32Height = pstMbSurface->u32YHeight - pstRect->s32Ypos;
    }

    return 0;
}

#endif
STATIC TDE_OPERATION_CATEGORY_E TdeOsiSingleSrcOperation(TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S*     pstBackGroundRect,
        TDE2_SURFACE_S* pstForeGround,
        TDE2_RECT_S*     pstForeGroundRect,
        TDE2_SURFACE_S* pstDst,
        TDE2_RECT_S*     pstDstRect,
        TDE2_OPT_S*      pstOpt)
{
    TDE2_SURFACE_S* pTmpSrc2 = HI_NULL;
    TDE2_RECT_S* pTmpSrc2Rect = HI_NULL;

    if ((HI_NULL != pstBackGround) && (HI_NULL == pstForeGround))
    {
        pTmpSrc2 = pstBackGround;
        pTmpSrc2Rect = pstBackGroundRect;
    }
    else
    {
        pTmpSrc2 = pstForeGround;
        pTmpSrc2Rect = pstForeGroundRect;
    }

    if (HI_NULL == pTmpSrc2Rect)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }


    if (TdeOsiCheckSurface(pTmpSrc2, pTmpSrc2Rect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

    if ((HI_NULL == pstOpt) || (!pstOpt->bResize))
    {
        TDE_UNIFY_RECT(pTmpSrc2Rect, pstDstRect);
    }

    if ((HI_NULL == pstOpt)
        && (pTmpSrc2->enColorFmt == pstDst->enColorFmt))
    {
        return TDE_OPERATION_SINGLE_SRC1;
    }


    if (HI_NULL == pstOpt)
    {
        TDE_TRACE(TDE_KERN_INFO, "pstOpt can not be NULL in single source 2 operation!\n");

        return TDE_OPERATION_BUTT;
    }

    return TDE_OPERATION_SINGLE_SRC2;

}
STATIC TDE_OPERATION_CATEGORY_E TdeOsiDoubleSrcOperation(TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S*     pstBackGroundRect,
        TDE2_SURFACE_S* pstForeGround,
        TDE2_RECT_S*     pstForeGroundRect,
        TDE2_RECT_S*     pstDstRect,
        TDE2_OPT_S*      pstOpt)
{
    if ((HI_NULL == pstBackGroundRect) || (HI_NULL == pstForeGroundRect) || (HI_NULL == pstOpt))
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

    if (TdeOsiCheckSurface(pstBackGround, pstBackGroundRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

    if (TdeOsiCheckSurface(pstForeGround, pstForeGroundRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

        if ((pstBackGroundRect->u32Height != pstDstRect->u32Height)
            || (pstBackGroundRect->u32Width != pstDstRect->u32Width))
        {
            TDE_TRACE(TDE_KERN_INFO, "\n");
            return TDE_OPERATION_BUTT;
        }

        if (!pstOpt->bResize)
        {

            if (pstBackGroundRect->u32Height != pstForeGroundRect->u32Height)
            {
                pstBackGroundRect->u32Height = TDE_MIN(pstBackGroundRect->u32Height, pstForeGroundRect->u32Height);
                pstDstRect->u32Height = pstBackGroundRect->u32Height;
                pstForeGroundRect->u32Height = pstBackGroundRect->u32Height;
            }


            if (pstBackGroundRect->u32Width != pstForeGroundRect->u32Width)
            {
                pstBackGroundRect->u32Width = TDE_MIN(pstBackGroundRect->u32Width, pstForeGroundRect->u32Width);
                pstDstRect->u32Width = pstBackGroundRect->u32Width;
                pstForeGroundRect->u32Width = pstBackGroundRect->u32Width;
            }
        }
        return TDE_OPERATION_DOUBLE_SRC;
    }

/*****************************************************************************
* Function:      TdeOsiGetOptCategory
* Description:   analyze TDE operate type
* Input:         pstBackGround: background bitmap info
*                pstBackGroundRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstForeGroundRect: foreground bitmap operate zone
*                pstDst: target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
* Output:        none
* Return:        TDE operate type
* Others:        none
*****************************************************************************/
STATIC TDE_OPERATION_CATEGORY_E  TdeOsiGetOptCategory(TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S*     pstBackGroundRect,
        TDE2_SURFACE_S* pstForeGround,
        TDE2_RECT_S*     pstForeGroundRect,
        TDE2_SURFACE_S* pstDst,
        TDE2_RECT_S*     pstDstRect,
        TDE2_OPT_S*      pstOpt)
{
    if ((HI_NULL == pstDst) || (HI_NULL == pstDstRect))
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

    TDE_CHECK_NOT_MB(pstDst->enColorFmt);

    if (TdeOsiCheckSurface(pstDst, pstDstRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_OPERATION_BUTT;
    }

    if ((HI_NULL == pstBackGround) && (HI_NULL == pstForeGround))
    {
        TDE_TRACE(TDE_KERN_INFO, "null pointer in single source 2 operation!");
        return TDE_OPERATION_BUTT;
    }
    else if ((HI_NULL != pstBackGround) && (HI_NULL != pstForeGround))
    {
        return TdeOsiDoubleSrcOperation(pstBackGround, pstBackGroundRect, pstForeGround, \
                                        pstForeGroundRect, pstDstRect, pstOpt);
    }
    else
    {
        return TdeOsiSingleSrcOperation(pstBackGround, pstBackGroundRect, pstForeGround, \
                                        pstForeGroundRect, pstDst, pstDstRect, pstOpt);
    }
}

/*****************************************************************************
* Function:      TdeOsiConvertSurface
* Description:   raster bitmap info by user upload translate to bitmap info which driver and hardware need
* Input:         pstSur: raster bitmap info by user upload
*                pstRect: raster bitmap operate zone by user upload
*                pstScanInfo: scanning direction info
*                pstDrvSur: bitmap info which driver and hardware need
* Output:        pstOperationArea: new operate zone fixed by scannning direction
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_VOID TdeOsiConvertSurface(TDE2_SURFACE_S *pstSur, TDE2_RECT_S *pstRect,
                                           TDE_SCANDIRECTION_S *pstScanInfo,
                                           TDE_DRV_SURFACE_S *pstDrvSur,
                                           TDE2_RECT_S *pstOperationArea)
{
    pstDrvSur->enColorFmt = g_enTdeCommonDrvColorFmt[pstSur->enColorFmt];
    pstDrvSur->u32Width  = pstRect->u32Width;
    pstDrvSur->u32Height = pstRect->u32Height;
    pstDrvSur->u32Pitch  = pstSur->u32Stride;
    pstDrvSur->bAlphaMax255 = pstSur->bAlphaMax255;
    pstDrvSur->enHScan = pstScanInfo->enHScan;
    pstDrvSur->enVScan = pstScanInfo->enVScan;
    pstDrvSur->u32PhyAddr = pstSur->u32PhyAddr;
    pstDrvSur->u32CbCrPhyAddr = pstSur->u32CbCrPhyAddr;
    pstDrvSur->u32CbCrPitch = pstSur->u32CbCrStride;
    pstDrvSur->enRgbOrder = g_enTdeArgbOrder[pstSur->enColorFmt];


    if(TDE_SCAN_RIGHT_LEFT == pstScanInfo->enHScan)
    {
        pstDrvSur->u32Xpos = (HI_U32)pstRect->s32Xpos + pstRect->u32Width - 1;
    }
    else
    {
        pstDrvSur->u32Xpos   = (HI_U32)pstRect->s32Xpos;
    }

    if(TDE_SCAN_DOWN_UP == pstScanInfo->enVScan)
    {
        pstDrvSur->u32Ypos = (HI_U32)pstRect->s32Ypos + pstRect->u32Height - 1;
    }
    else
    {
        pstDrvSur->u32Ypos   = (HI_U32)pstRect->s32Ypos;
    }

    if(NULL != pstOperationArea)
    {

        pstOperationArea->s32Xpos = pstRect->s32Xpos;
        pstOperationArea->s32Ypos = (HI_S32)pstDrvSur->u32Ypos;
        pstOperationArea->u32Width = pstDrvSur->u32Width;
        pstOperationArea->u32Height = pstDrvSur->u32Height;
    }

    return;
}
STATIC HI_VOID TdeOsiSetMbSurfaceDrvY(TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect, TDE_DRV_SURFACE_S* pstDrvY,
                                     TDE_PIC_MODE_E enPicMode)
{
    if (HI_NULL == pstDrvY)
    {
        return ;
    }

    pstDrvY->enColorFmt = g_enTdeMbDrvColorFmt[pstMB->enMbFmt];
    pstDrvY->u32Width = pstMbRect->u32Width;
    pstDrvY->u32Xpos = (HI_U32)pstMbRect->s32Xpos;
    pstDrvY->u32PhyAddr = pstMB->u32YPhyAddr;

    if (TDE_FRAME_PIC_MODE == enPicMode)
    {
        pstDrvY->u32Height = pstMbRect->u32Height;
        pstDrvY->u32Pitch = pstMB->u32YStride;
        pstDrvY->u32Ypos = (HI_U32)pstMbRect->s32Ypos;
    }
    else
    {
        pstDrvY->u32Height = TDE_ADJ_FIELD_HEIGHT_BY_START((HI_U32)pstMbRect->s32Ypos, (HI_U32)pstMbRect->u32Height);
        pstDrvY->u32Pitch = pstMB->u32YStride * 2;
        pstDrvY->u32Ypos = ((HI_U32)(pstMbRect->s32Ypos / 2));
    }

    return ;
}
STATIC HI_S32 TdeOsiSetMbSurfaceDrvCbCr(TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect, TDE_DRV_SURFACE_S* pstDrvCbCr,
                                        TDE_PIC_MODE_E enPicMode )
{
    TDE_DRV_COLOR_FMT_E enAdjColorFmt;

    if (HI_NULL == pstDrvCbCr)
    {
        return HI_SUCCESS;
    }

    enAdjColorFmt = g_enTdeMbDrvColorFmt[pstMB->enMbFmt];
    pstDrvCbCr->enColorFmt = g_enTdeMbDrvColorFmt[pstMB->enMbFmt];
    pstDrvCbCr->u32PhyAddr = pstMB->u32CbCrPhyAddr;

#if 0	/** deal with codecc **/
    if (bCbCr2Opt)
    {
        enAdjColorFmt = TDE_DRV_COLOR_FMT_YCbCr444MB;
    }
#endif

    if (TDE_FRAME_PIC_MODE == enPicMode)
    {
        pstDrvCbCr->u32Pitch = pstMB->u32CbCrStride;


        switch (enAdjColorFmt)
        {
	        case TDE_DRV_COLOR_FMT_YCbCr400MBP:
	            return -1;
	        case TDE_DRV_COLOR_FMT_YCbCr422MBH:
	        {
	            pstDrvCbCr->u32Xpos  = (HI_U32)pstMbRect->s32Xpos / 2;
	            pstDrvCbCr->u32Ypos  = (HI_U32)pstMbRect->s32Ypos;
	            pstDrvCbCr->u32Width =
	                TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Xpos, pstMbRect->u32Width);
	            pstDrvCbCr->u32Height = pstMbRect->u32Height;
	        }
	            break;
	        case TDE_DRV_COLOR_FMT_YCbCr422MBV:
	        {
	            pstDrvCbCr->u32Xpos   = (HI_U32)pstMbRect->s32Xpos;
	            pstDrvCbCr->u32Ypos   = (HI_U32)pstMbRect->s32Ypos / 2;
	            pstDrvCbCr->u32Width  = pstMbRect->u32Width;
	            pstDrvCbCr->u32Height =
	                TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Ypos, pstMbRect->u32Height);
	        }
	            break;
	        case TDE_DRV_COLOR_FMT_YCbCr420MB:
	        {
	            pstDrvCbCr->u32Xpos  = (HI_U32)pstMbRect->s32Xpos / 2;
	            pstDrvCbCr->u32Ypos  = (HI_U32)pstMbRect->s32Ypos / 2;
	            pstDrvCbCr->u32Width =
	                TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Xpos, pstMbRect->u32Width);
	            pstDrvCbCr->u32Height =
	                TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Ypos, pstMbRect->u32Height);
	            TDE_TRACE(TDE_KERN_DEBUG, "W:0x%x, H:0x%x\n", pstDrvCbCr->u32Width,
	            pstDrvCbCr->u32Height);
	        }
	            break;
	        case TDE_DRV_COLOR_FMT_YCbCr444MB:
	        {
	            pstDrvCbCr->u32Xpos   = (HI_U32)pstMbRect->s32Xpos;
	            pstDrvCbCr->u32Ypos   = (HI_U32)pstMbRect->s32Ypos;
	            pstDrvCbCr->u32Width  = pstMB->u32YWidth;
	            pstDrvCbCr->u32Height = pstMbRect->u32Height;
	        }
	            break;
	        default:
	            TDE_TRACE(TDE_KERN_INFO, "Unspport color format!\n");
	            return -1;
        }

        return HI_SUCCESS;
    }

    pstDrvCbCr->u32Pitch = pstMB->u32CbCrStride * 2;


    switch (enAdjColorFmt)
    {
	    case TDE_DRV_COLOR_FMT_YCbCr400MBP:
	        return -1;
	    case TDE_DRV_COLOR_FMT_YCbCr422MBH:
	    {
	        pstDrvCbCr->u32Xpos  = (HI_U32)pstMbRect->s32Xpos / 2;
	        pstDrvCbCr->u32Ypos  = ((HI_U32)pstMbRect->s32Ypos / 2);
	        pstDrvCbCr->u32Width =
	            TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Xpos, pstMbRect->u32Width);
	        pstDrvCbCr->u32Height = TDE_ADJ_FIELD_HEIGHT_BY_START((HI_U32)pstMbRect->s32Ypos, pstMbRect->u32Height);
	    }
	        break;
	    case TDE_DRV_COLOR_FMT_YCbCr422MBV:
	    {
	        pstDrvCbCr->u32Xpos   = (HI_U32)pstMbRect->s32Xpos;
	        pstDrvCbCr->u32Ypos   = ((HI_U32)pstMbRect->s32Ypos / 4);
	        pstDrvCbCr->u32Width  = pstMbRect->u32Width;
	        pstDrvCbCr->u32Height = TDE_ADJ_SIZE_BY_START_I(pstMbRect->s32Ypos, pstMbRect->u32Height);
	    }
	        break;
	    case TDE_DRV_COLOR_FMT_YCbCr420MB:
	    {
	        pstDrvCbCr->u32Xpos  = (HI_U32)pstMbRect->s32Xpos / 2;
	        pstDrvCbCr->u32Ypos  = ((HI_U32)pstMbRect->s32Ypos / 4);
	        pstDrvCbCr->u32Width = TDE_ADJ_SIZE_BY_START_P(pstMbRect->s32Xpos, pstMbRect->u32Width);
	        pstDrvCbCr->u32Height = TDE_ADJ_SIZE_BY_START_I(pstMbRect->s32Ypos, pstMbRect->u32Height);
	        TDE_TRACE(TDE_KERN_DEBUG, "W:0x%x, H:0x%x\n", pstDrvCbCr->u32Width,
	        pstDrvCbCr->u32Height);
	    }
	        break;
	    case TDE_DRV_COLOR_FMT_YCbCr444MB:
	    {
	        pstDrvCbCr->u32Xpos   = (HI_U32)pstMbRect->s32Xpos;
	        pstDrvCbCr->u32Ypos  = (HI_U32)pstMbRect->s32Ypos / 2;
	        pstDrvCbCr->u32Width = pstMB->u32YWidth;
	        pstDrvCbCr->u32Height = TDE_ADJ_FIELD_HEIGHT_BY_START((HI_U32)pstMbRect->s32Ypos, pstMbRect->u32Height);
	    }
	        break;
	    default:
	        TDE_TRACE(TDE_KERN_INFO, "Unspport color format!\n");
	        return -1;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiConvertMbSurface
* Description:   convert MB bitmap info to bitmap info which driver and hardware need
* Input:         pstMB: MB bitmap info
*                pstMbRect: MB bitmap operate zone
*                pstDrvY: brightness bitmap info
*                pstDrvCbCr: chroma bitmap info
*                enPicMode: picture mode: frame/ bottom field/ top field
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiConvertMbSurface(TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect, TDE_DRV_SURFACE_S* pstDrvY,
                                     TDE_DRV_SURFACE_S* pstDrvCbCr, TDE_PIC_MODE_E enPicMode, HI_BOOL bCbCr2Opt)
{
    HI_S32 s32Ret;
    HI_BOOL bContainNullPtr = ((HI_NULL == pstMB) || (HI_NULL == pstMbRect));

    if (bContainNullPtr)
    {
        TDE_TRACE(TDE_KERN_INFO, "null ptr!\n");
        return -1;
    }

    TdeOsiSetMbSurfaceDrvY( pstMB, pstMbRect, pstDrvY, enPicMode);

    s32Ret = TdeOsiSetMbSurfaceDrvCbCr( pstMB, pstMbRect, pstDrvCbCr, enPicMode);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}


/*****************************************************************************
* Function:      TdeOsiSetClipPara
* Description:   set clip zone parameter
* Input:         pstBackGround: background bitmap info
*                pstBGRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstFGRect: foreground bitmap operate zone
*                pstDst:  target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
*                pstHwNode: hardware operate node
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiSetClipPara(TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S *pstBGRect,
                                 TDE2_SURFACE_S * pstForeGround,
                                 TDE2_RECT_S *pstFGRect, TDE2_SURFACE_S * pstDst, TDE2_RECT_S *pstDstRect,
                                 TDE2_OPT_S * pstOpt,
                                 TDE_HWNode_S *pstHwNode)
{
    TDE_DRV_CLIP_CMD_S stClip = {0};
    TDE2_RECT_S stInterRect = {0};

    TDE_ASSERT(NULL != pstForeGround);
    TDE_ASSERT(NULL != pstDst);
    TDE_ASSERT(NULL != pstOpt);
    TDE_ASSERT(NULL != pstHwNode);


    if ((TDE2_CLIPMODE_INSIDE == pstOpt->enClipMode) && (!pstOpt->bResize))
    {
        if (TdeOsiGetInterRect(pstDstRect, &pstOpt->stClipRect, &stInterRect) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inrerrect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }

        if (NULL != pstBackGround)
        {

            pstBGRect->s32Xpos += stInterRect.s32Xpos - pstDstRect->s32Xpos;
            pstBGRect->s32Ypos  += stInterRect.s32Ypos - pstDstRect->s32Ypos;
            pstBGRect->u32Height = stInterRect.u32Height;
            pstBGRect->u32Width = stInterRect.u32Width;
        }


        pstFGRect->s32Xpos += stInterRect.s32Xpos - pstDstRect->s32Xpos;
        pstFGRect->s32Ypos  += stInterRect.s32Ypos - pstDstRect->s32Ypos;
        pstFGRect->u32Height = stInterRect.u32Height;
        pstFGRect->u32Width = stInterRect.u32Width;


        *pstDstRect = stInterRect;
    }
    else if (TDE2_CLIPMODE_INSIDE == pstOpt->enClipMode)
    {
        if (TdeOsiGetInterRect(pstDstRect, &pstOpt->stClipRect, &stInterRect) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inter-rect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }

        stClip.bInsideClip   = HI_TRUE ;
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos;
        stClip.u16ClipEndX = pstOpt->stClipRect.s32Xpos + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = pstOpt->stClipRect.s32Ypos + pstOpt->stClipRect.u32Height - 1;

        if(TdeHalNodeSetClipping(pstHwNode, &stClip)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
    else if(TDE2_CLIPMODE_OUTSIDE == pstOpt->enClipMode)
    {
        if (TdeOsiIsRect1InRect2(pstDstRect, &pstOpt->stClipRect))
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inter-rect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }


        if (TdeOsiGetInterRect(pstDstRect, &pstOpt->stClipRect, &stInterRect) < 0)
        {
            return 0;
        }

        stClip.bInsideClip   = HI_FALSE;
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos;
        stClip.u16ClipEndX = pstOpt->stClipRect.s32Xpos + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = pstOpt->stClipRect.s32Ypos + pstOpt->stClipRect.u32Height - 1;

        if(TdeHalNodeSetClipping(pstHwNode, &stClip)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
    else if(pstOpt->enClipMode >= TDE2_CLIPMODE_BUTT)
    {
        TDE_TRACE(TDE_KERN_INFO, "error clip mode!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    return 0;
}

#ifndef HI_BUILD_IN_BOOT
STATIC HI_S32 TdeOsiSetPatternClipPara(TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S *pstBGRect,
                                 TDE2_SURFACE_S * pstForeGround,
                                 TDE2_RECT_S *pstFGRect, TDE2_SURFACE_S * pstDst, TDE2_RECT_S *pstDstRect,
                                 TDE2_PATTERN_FILL_OPT_S * pstOpt,
                                 TDE_HWNode_S *pstHwNode)
{
    TDE_DRV_CLIP_CMD_S stClip = {0};
    TDE2_RECT_S stInterRect = {0};

    TDE_ASSERT(NULL != pstForeGround);
    TDE_ASSERT(NULL != pstDst);
    TDE_ASSERT(NULL != pstOpt);
    TDE_ASSERT(NULL != pstHwNode);

    TDE_CHECK_CLIPMODE(pstOpt->enClipMode);

    if (TDE2_CLIPMODE_INSIDE == pstOpt->enClipMode)
    {

        if (TdeOsiGetInterRect(pstDstRect, &pstOpt->stClipRect, &stInterRect) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inrerrect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }
            stClip.bInsideClip   = HI_TRUE;
            stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos;
            stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos;
            stClip.u16ClipEndX = pstOpt->stClipRect.s32Xpos + pstOpt->stClipRect.u32Width - 1;
            stClip.u16ClipEndY = pstOpt->stClipRect.s32Ypos + pstOpt->stClipRect.u32Height - 1;

            if(TdeHalNodeSetClipping(pstHwNode, &stClip)<0)
            {
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }
    }
    else if(TDE2_CLIPMODE_OUTSIDE == pstOpt->enClipMode)
    {
        if (TdeOsiIsRect1InRect2(pstDstRect, &pstOpt->stClipRect))
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inter-rect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }


        if (TdeOsiGetInterRect(pstDstRect, &pstOpt->stClipRect, &stInterRect) < 0)
        {
            return 0;
        }

        stClip.bInsideClip   = HI_FALSE;
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos;
        stClip.u16ClipEndX = pstOpt->stClipRect.s32Xpos + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = pstOpt->stClipRect.s32Ypos + pstOpt->stClipRect.u32Height - 1;


        if(TdeHalNodeSetClipping(pstHwNode, &stClip)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
    else if(pstOpt->enClipMode >= TDE2_CLIPMODE_BUTT)
    {
        TDE_TRACE(TDE_KERN_INFO, "error clip mode!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    return 0;
}
#endif
/*****************************************************************************
* Function:      TdeOsiMbSetClipPara
* Description:   set clip zone parameter by MB operate
* Input:         pstDstRect:background bitmap operate zone
*                pstMbOpt: set parameter
*                pstHwNode: hardware operate node
* Output:        none
* Return:        none
* Others:        note: MB operate is not refer to software optimization of clip zone
*****************************************************************************/
STATIC HI_S32 TdeOsiMbSetClipPara(TDE2_RECT_S * pstDstRect, TDE2_MBOPT_S *pstMbOpt,
                                 TDE_HWNode_S *pstHwNode)
{
    TDE2_RECT_S stInterRect = {0};

    TDE_ASSERT(NULL != pstDstRect);
    TDE_ASSERT(NULL != pstMbOpt);
    TDE_ASSERT(NULL != pstHwNode);

    if (TDE2_CLIPMODE_INSIDE == pstMbOpt->enClipMode)
    {

        if (TdeOsiGetInterRect(pstDstRect, &pstMbOpt->stClipRect, &stInterRect) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inter-rect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }
        TDE_SET_CLIP(pstHwNode, pstMbOpt);
    }
    else if(TDE2_CLIPMODE_OUTSIDE == pstMbOpt->enClipMode)
    {
        if (TdeOsiIsRect1InRect2(pstDstRect, &pstMbOpt->stClipRect))
        {
            TDE_TRACE(TDE_KERN_INFO, "clip and operation area have no inter-rect!\n");
            return HI_ERR_TDE_CLIP_AREA;
        }

        TDE_SET_CLIP(pstHwNode, pstMbOpt);
    }
    else if(pstMbOpt->enClipMode >= TDE2_CLIPMODE_BUTT)
    {
        TDE_TRACE(TDE_KERN_INFO, "error clip mode!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }
    return 0;
}
/*****************************************************************************
 Prototype       : TdeOsiSetExtAlpha
 Description     : two alpha values when set ARGB1555 format
 Input           : pstBackGround  **
                   pstForeGround  **
                   pstHwNode      **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/6/10
    Author       : executable frame by single
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_VOID TdeOsiSetExtAlpha(TDE2_SURFACE_S *pstBackGround, TDE2_SURFACE_S *pstForeGround,
                                        TDE_HWNode_S *pstHwNode)
{
    TDE_DRV_SRC_E enSrc = TDE_DRV_SRC_NONE;

    if((NULL != pstForeGround)
        && (pstForeGround->bAlphaExt1555)
        && (TDE2_COLOR_FMT_ARGB1555 <= pstForeGround->enColorFmt)
        && (pstForeGround->enColorFmt <= TDE2_COLOR_FMT_BGRA1555))
    {
        enSrc |= TDE_DRV_SRC_S2;
    }

    if((NULL != pstBackGround)
        && (pstBackGround->bAlphaExt1555)
        && (TDE2_COLOR_FMT_ARGB1555 <= pstBackGround->enColorFmt)
        && (pstBackGround->enColorFmt <= TDE2_COLOR_FMT_BGRA1555))
    {
        enSrc |= TDE_DRV_SRC_S1;
    }


    if(enSrc & TDE_DRV_SRC_S1)
    {
        TdeHalNodeSetExpAlpha(pstHwNode, enSrc, pstBackGround->u8Alpha0, pstBackGround->u8Alpha1);
    }
    else if(enSrc & TDE_DRV_SRC_S2)
    {
        TdeHalNodeSetExpAlpha(pstHwNode, enSrc, pstForeGround->u8Alpha0, pstForeGround->u8Alpha1);
    }
}

/*****************************************************************************
* Function:      TdeOsiSetBaseOptParaForBlit
* Description:   encapsulation function used to set operate type
* Input:         pstOpt: operate option
*                enOptCategory: operate category
*                pstHwNode: nareware operate node
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiSetBaseOptParaForBlit(TDE2_OPT_S * pstOpt, TDE2_SURFACE_S* pstSrc1, TDE2_SURFACE_S* pstSrc2,
                                           TDE_OPERATION_CATEGORY_E enOptCategory, TDE_HWNode_S *pstHwNode)
{
    TDE_DRV_BASEOPT_MODE_E enBaseOpt = {0};
    TDE_DRV_ALU_MODE_E enAlu = TDE_ALU_NONE;
    HI_BOOL bContainNullPtr;
	HI_S32 s32Ret;

    bContainNullPtr = ((HI_NULL == pstHwNode) || (HI_NULL == pstOpt));

    if (bContainNullPtr)
    {
        return HI_SUCCESS;
    }

    if ((HI_NULL != pstSrc1) && (HI_NULL != pstSrc2))
    {
        if ( (TDE_COLORFMT_CATEGORY_An == TdeOsiGetFmtCategory(pstSrc2->enColorFmt))
              && (TDE_COLORFMT_CATEGORY_ARGB == TdeOsiGetFmtCategory(pstSrc1->enColorFmt)
                  || TDE_COLORFMT_CATEGORY_YCbCr == TdeOsiGetFmtCategory(pstSrc1->enColorFmt)) )
        {
            enAlu = TDE_SRC1_BYPASS;
        }
    }

    if (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
    {
        enAlu = TDE_ALU_ROP;


        if(TdeHalNodeSetRop(pstHwNode, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
     #ifndef HI_BUILD_IN_BOOT
    if (pstOpt->enAluCmd & TDE2_ALUCMD_BLEND)
    {
        enAlu = TDE_ALU_BLEND;

        if(TdeHalNodeSetBlend(pstHwNode, &pstOpt->stBlendOpt)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        if (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
        {
            TdeHalNodeEnableAlphaRop(pstHwNode);
        }
    }

    if (pstOpt->enAluCmd & TDE2_ALUCMD_COLORIZE)
    {
        if(TdeHalNodeSetColorize(pstHwNode, pstOpt->u32Colorize)<0)
        {
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
    #endif
    if (TDE_OPERATION_DOUBLE_SRC == enOptCategory)
    {
        enBaseOpt = TDE_NORM_BLIT_2OPT;
    }
    #ifndef HI_BUILD_IN_BOOT/**codecc**/
    else
    {
        enBaseOpt = TDE_NORM_BLIT_1OPT;
    }
	#endif

    TdeHalNodeSetGlobalAlpha(pstHwNode, pstOpt->u8GlobalAlpha, pstOpt->stBlendOpt.bGlobalAlphaEnable);

    if(TdeHalNodeSetBaseOperate(pstHwNode, enBaseOpt, enAlu, 0)<0)
    {
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    s32Ret = TdeHalNodeSetCsc(pstHwNode, pstOpt->stCscOpt);
	if (s32Ret < 0)
	{
		return s32Ret;
	}

    return HI_SUCCESS;
}



/*****************************************************************************
* Function:      TdeOsiSetDeflickerPara
* Description:   set deflicker coefficient to hardware node
* Input:         pstHwNode: node need to set
*
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiSetDeflickerPara(TDE_HWNode_S* pstHwNode, TDE2_DEFLICKER_MODE_E enDeflickerMode, TDE_FILTER_OPT* pstFilterOpt)
{
    TDE_DRV_FLICKER_CMD_S stFlickerCmd = {0};
    HI_BOOL bDeflicker = (TDE2_DEFLICKER_MODE_NONE == enDeflickerMode)?HI_FALSE:HI_TRUE;

#ifndef __TDE_CHECK_SLICE_DATA__
    HI_U8 u8DefTable[] = {0, 64, 0, 12, 40, 12, 15, 37, 15, 17, 34, 17,
                          2, 4, 7};
#else
    HI_U8 u8DefTable[] = {0, 64, 0, 0, 64, 0, 0, 64, 0, 0, 64, 0,
                          0, 64, 0};
#endif


    if (HI_TRUE == bDeflicker)
    {
        stFlickerCmd.enDfeMode = TDE_DRV_AUTO_FILTER;
    }

	stFlickerCmd.u8Coef0LastLine = u8DefTable[0];
	stFlickerCmd.u8Coef0CurLine  = u8DefTable[1];
	stFlickerCmd.u8Coef0NextLine = u8DefTable[2];
	stFlickerCmd.u8Coef1LastLine = u8DefTable[3];
	stFlickerCmd.u8Coef1CurLine  = u8DefTable[4];
	stFlickerCmd.u8Coef1NextLine = u8DefTable[5];
	stFlickerCmd.u8Coef2LastLine = u8DefTable[6];
	stFlickerCmd.u8Coef2CurLine  = u8DefTable[7];
	stFlickerCmd.u8Coef2NextLine = u8DefTable[8];
	stFlickerCmd.u8Coef3LastLine = u8DefTable[9];
	stFlickerCmd.u8Coef3CurLine  = u8DefTable[10];
	stFlickerCmd.u8Coef3NextLine = u8DefTable[11];
	stFlickerCmd.u8Threshold0    = u8DefTable[12];
	stFlickerCmd.u8Threshold1    = u8DefTable[13];
	stFlickerCmd.u8Threshold2    = u8DefTable[14];

	stFlickerCmd.enFilterV = pstFilterOpt->enFilterMode;
    stFlickerCmd.enDeflickerMode = enDeflickerMode;
    if(TdeHalNodeSetFlicker(pstHwNode, &stFlickerCmd)<0)
    {
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return HI_SUCCESS;
}



/*****************************************************************************
* Function:      TdeOsiAdjClipPara
* Description:   set clip parameter to hardware node
* Input:         pstHwNode: node need to set
*
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiAdjClipPara(TDE_HWNode_S* pHWNode)
{

	if(pHWNode->u32TDE_CLIP_START >> 31)
	{
		HI_U32 u32ClipStartX, u32ClipStartY, u32ClipEndX, u32ClipEndY;
		HI_U32 u32OutStartX, u32OutStartY, u32OutEndX, u32OutEndY;
		HI_U32 u32StartX, u32StartY, u32EndX, u32EndY;

		u32ClipStartX = pHWNode->u32TDE_CLIP_START & 0xfff;
		u32ClipStartY = (pHWNode->u32TDE_CLIP_START >> 16) & 0xfff;
		u32ClipEndX = pHWNode->u32TDE_CLIP_STOP & 0xfff;
		u32ClipEndY = (pHWNode->u32TDE_CLIP_STOP >> 16) & 0xfff;

		u32OutStartX = pHWNode->u32TDE_TAR_XY & 0xfff;
		u32OutStartY = (pHWNode->u32TDE_TAR_XY >> 16) & 0xfff;
		u32OutEndX = u32OutStartX + (pHWNode->u32TDE_TS_SIZE & 0xfff) - 1;
		u32OutEndY = u32OutStartY + ((pHWNode->u32TDE_TS_SIZE >> 16) & 0xfff) - 1;

		u32StartX = (u32ClipStartX > u32OutStartX) ? u32ClipStartX : u32OutStartX;
		u32StartY = (u32ClipStartY > u32OutStartY) ? u32ClipStartY : u32OutStartY;
		u32EndX = (u32ClipEndX < u32OutEndX) ? u32ClipEndX : u32OutEndX;
		u32EndY = (u32ClipEndY < u32OutEndY) ? u32ClipEndY : u32OutEndY;


		if((u32StartX > u32EndX) || (u32StartY > u32EndY))
		{
			pHWNode->u32TDE_INS = pHWNode->u32TDE_INS & 0xffffbfff;
		}
		else
		{
			pHWNode->u32TDE_CLIP_START = ((0x80000000) | (u32StartX & 0xfff) | ((u32StartY & 0xfff) << 16));
			pHWNode->u32TDE_CLIP_STOP = ( (u32EndX & 0xfff) | ((u32EndY & 0xfff) << 16));
		}

	}

	return HI_SUCCESS;

}

/*****************************************************************************
* Function:      TdeOsiSetNodeFinish
* Description:   complete node config, add node to list signed by handle
* Input:         s32Handle: task handle, which is sumbit list
*                pHWNode: set node
*                u32WorkBufNum: temporary buffer number
*                enSubmType: submit node type
* Output:        none
* Return:        return slice number
* Others:        node struct is following:
*                 ----------------------
*                 |  software node pointer(4)   |
*                 ----------------------
*                 |  config parameter          |
*                 ----------------------
*                 |  physical address of next node(4) |
*                 ----------------------
*                 |  update flag of next node(4) |
*                 ----------------------
*****************************************************************************/
STATIC HI_S32 TdeOsiSetNodeFinish(TDE_HANDLE s32Handle, TDE_HWNode_S* pHWNode,
                                  HI_U32 u32WorkBufNum, TDE_NODE_SUBM_TYPE_E enSubmType)
{
    HI_HANDLE_MGR *pHandleMgr =NULL;
    TDE_SWJOB_S *pstJob=NULL;
    TDE_SWNODE_S *pstCmd=NULL;
    HI_BOOL bValid = 0;

    TdeOsiAdjClipPara(pHWNode);
    bValid = query_handle(s32Handle, &pHandleMgr);
    if (!bValid)
    {
        TDE_TRACE(TDE_KERN_INFO, "invalid handle %d!\n", s32Handle);
        return HI_ERR_TDE_INVALID_HANDLE;
    }
    pstJob = (TDE_SWJOB_S*)pHandleMgr->res;
    if(pstJob->bSubmitted)
    {
        TDE_TRACE(TDE_KERN_INFO, "job %d already submitted!\n", s32Handle);
        return HI_ERR_TDE_INVALID_HANDLE;
    }
    pstCmd = (TDE_SWNODE_S *) TDE_MALLOC(sizeof(TDE_SWNODE_S));
    if (NULL == pstCmd)
    {
        TDE_TRACE(TDE_KERN_INFO, "malloc failed!\n");
        return HI_ERR_TDE_NO_MEM;
    }
    if (enSubmType != TDE_NODE_SUBM_CHILD)
    {
        pstJob->u32CmdNum++;

        if (1 == pstJob->u32CmdNum)
        {
            pstJob->pstFirstCmd = pstCmd;

            INIT_LIST_HEAD(&pstCmd->stList);
        }

        pstJob->pstLastCmd = pstCmd;
    }

#ifndef HI_BUILD_IN_BOOT
#ifndef CONFIG_TDE_PROC_DISABLE
    TDEProcRecordNode(pHWNode);
#endif
#endif
    pstCmd->stNodeBuf.pBuf = (HI_VOID*)pHWNode-TDE_NODE_HEAD_BYTE;
    pstCmd->stNodeBuf.u32NodeSz = sizeof(TDE_HWNode_S);
    pstCmd->stNodeBuf.u64Update=(0xffffffff)|((HI_U64)0x000003ff<<32);
    pstCmd->stNodeBuf.u32PhyAddr =wgetphy(pstCmd->stNodeBuf.pBuf) + TDE_NODE_HEAD_BYTE;
    /* If the tail node of the job is not null,add the current job node to the tail of the job list of hardware.That is to say the next node address of the tail\
    node is the current node address.*/
    if (HI_NULL != pstJob->pstTailNode)
    {
       HI_U32 *pNextNodeAddr = (HI_U32 *)pstJob->pstTailNode->stNodeBuf.pBuf +(TDE_NODE_HEAD_BYTE >> 2)+((pstJob->pstTailNode->stNodeBuf.u32NodeSz) >> 2);
       HI_U64 *pNextNodeUpdate = (HI_U64 *)(pNextNodeAddr + 1);
       *pNextNodeAddr = pstCmd->stNodeBuf.u32PhyAddr;
       *pNextNodeUpdate = pstCmd->stNodeBuf.u64Update;
    }
    pstCmd->s32Handle = pstJob->s32Handle;
    pstCmd->s32Index   = pstJob->u32CmdNum;
    pstCmd->enSubmType = enSubmType;
    pstCmd->u32PhyBuffNum = u32WorkBufNum;
    *(((HI_U32 *)pstCmd->stNodeBuf.pBuf) + 1) = s32Handle;
    list_add_tail(&pstCmd->stList, &pstJob->pstFirstCmd->stList);/*Add the cmd to the job list*/
    pstJob->pstTailNode = pstCmd;
    pstJob->u32NodeNum++;
    if (pstCmd->u32PhyBuffNum != 0)
    {
        pstJob->bAqUseBuff = HI_TRUE;
    }
    return HI_SUCCESS;

}
/****************************************************************************
                             TDE osi ctl interface initialization
****************************************************************************/

/*****************************************************************************
* Function:      TdeOsiBeginJob
* Description:   get TDE task handle
* Input:         none
* Output:        none
* Return:        created task handle
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiBeginJob(TDE_HANDLE *ps32Handle)
{
    if (NULL == ps32Handle)
    {
        TDE_TRACE(TDE_KERN_INFO, "Null ptr!\n");
        return HI_ERR_TDE_NULL_PTR;
    }

    return TdeOsiListBeginJob(ps32Handle);
}
#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiBeginJob);
#endif

/*****************************************************************************
* Function:      TdeOsiEndJob
* Description:   submit TDE task
* Input:         s32Handle: task handle
*                bBlock: if block
*                u32TimeOut: timeout value(unit by 10ms)
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiEndJob(TDE_HANDLE s32Handle, HI_BOOL bBlock, HI_U32 u32TimeOut,
                        HI_BOOL bSync, TDE_FUNC_CB pFuncComplCB, HI_VOID *pFuncPara)
{
#ifndef HI_BUILD_IN_BOOT
    TDE_NOTIFY_MODE_E enNotiType;

    if (bBlock)
    {
        if(in_interrupt())
        {
            TDE_TRACE(TDE_KERN_INFO, "can not be block in interrupt!\n");
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        enNotiType = TDE_JOB_WAKE_NOTIFY;
    }
    else
    {
        enNotiType = TDE_JOB_COMPL_NOTIFY;
    }

    return TdeOsiListSubmitJob(s32Handle,u32TimeOut, pFuncComplCB, pFuncPara, enNotiType);
#else
    return TdeOsiListSubmitJob(s32Handle,u32TimeOut, pFuncComplCB, pFuncPara, TDE_JOB_WAKE_NOTIFY);
#endif
}


#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiEndJob);

/*****************************************************************************
* Function:      TdeOsiDelJob
* Description:   delete created TDE task, only effective for call before endjob
*                use to release software resource of list of task
* Input:         s32Handle: task handle
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiCancelJob(TDE_HANDLE s32Handle)
{
    return TdeOsiListCancelJob(s32Handle);
}
EXPORT_SYMBOL(TdeOsiCancelJob);

/*****************************************************************************
* Function:      TdeOsiWaitForDone
* Description:   wait for completion of submit TDE operate
* Input:         s32Handle: task handle
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiWaitForDone(TDE_HANDLE s32Handle, HI_U32 u32TimeOut)
{
    if(in_interrupt())
    {
        TDE_TRACE(TDE_KERN_INFO, "can not be block in interrupt!\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TdeOsiListWaitForDone(s32Handle, u32TimeOut);
}
EXPORT_SYMBOL(TdeOsiWaitForDone);

/*****************************************************************************
* Function:      TdeOsiWaitAllDone
* Description:   wait for all TDE operate completion
* Input:         none
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiWaitAllDone(HI_BOOL bSync)
{
    if(in_interrupt())
    {
        TDE_TRACE(TDE_KERN_INFO, "can not wait in interrupt!\n");
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TdeOsiListWaitAllDone();
}
EXPORT_SYMBOL(TdeOsiWaitAllDone);

/*****************************************************************************
* Function:      TdeOsiReset
* Description:   reset all state, release all list
* Input:         none
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_VOID TdeOsiReset(HI_VOID)
{
    return TdeOsiListReset();
}
EXPORT_SYMBOL(TdeOsiReset);



/*****************************************************************************
* Function:      TdeOsiQuickCopy
* Description:    quick blit source to target, no any functional operate, the size of source and target are the same
*                format is not MB format
* Input:         pSrc: source bitmap info
*                pstDst:  target bitmap info
*                pFuncComplCB: callback function pointer when operate is completed
				 when it is null, it say to need not notice operate is over
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickCopy(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect)
{
    return TdeOsiBlit(s32Handle,NULL,NULL, pstSrc, pstSrcRect, pstDst,pstDstRect,NULL);
}
EXPORT_SYMBOL(TdeOsiQuickCopy);

/*****************************************************************************
* Function:      TdeOsiQuickResize
* Description:   zoom the size of source bitmap to the size aasigned by target bitmap, of which source and target can be the same
* Input:         pSrc: source bitmap info struct
*                pDst: target bitmap info struct
*                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
* Output:        none
* Return:        none
* Others:        AI7D02579  add support for YCbCr422
*****************************************************************************/
HI_S32     TdeOsiQuickResize(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                             TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect)
{
    HI_S32 s32Ret = HI_FAILURE;
    TDE2_OPT_S stOpt = {0};

    stOpt.bResize = HI_TRUE;

    if ((s32Ret = TdeOsiBlit(s32Handle,NULL,NULL, pstSrc, pstSrcRect,
                                     pstDst, pstDstRect,&stOpt)) < 0)
    {
        return s32Ret;
    }
    return HI_SUCCESS;
}
EXPORT_SYMBOL(TdeOsiQuickResize);

/*****************************************************************************
* Function:      TdeOsiQuickFlicker
* Description:   deflicker source bitmap,output to target bitmap,source and target can be the same
* Input:         pSrc: source bitmap info struct
*                pDst: terget bitmap info struct
*                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickFlicker(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                              TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect)
{
    HI_S32 s32Ret = HI_FAILURE;
    TDE2_OPT_S stOpt = {0};

    stOpt.enDeflickerMode = TDE2_DEFLICKER_MODE_BOTH;

    stOpt.bResize = HI_TRUE;
    if ((s32Ret = TdeOsiBlit(s32Handle,NULL,NULL, pstSrc, pstSrcRect,
                                     pstDst, pstDstRect,&stOpt)) < 0)
    {
        return s32Ret;
    }
    return HI_SUCCESS;
}
EXPORT_SYMBOL(TdeOsiQuickFlicker);


#endif

/*****************************************************************************
* Function:      TdeOsiQuickFill
* Description:   quick fill fixed value to target bitmap, fill value is refered to target bitmap
* Input:         pstDst: target bitmap info struct
*                u32FillData: fill value
*                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
                           HI_U32 u32FillData)
{
    TDE2_FILLCOLOR_S stFillColor;

    stFillColor.enColorFmt   = pstDst->enColorFmt;
    stFillColor.u32FillColor = u32FillData;

    return TdeOsi1SourceFill(s32Handle, pstDst, pstDstRect, &stFillColor, NULL);
}
#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiQuickFill);
#endif



#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeOsiSingleSrc1Blit
* Description:   source1 operate realization
* Input:         pstBackGround: source1 bitmap info struct
*                pstBackGroundRect: source1 bitmap operate zone
*                pstDst: target bitmap info struct
*                pstDstRect: target bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
STATIC HI_S32 TdeOsiSingleSrc1Blit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                   TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect, HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst)
{
    HI_S32 s32Ret;
    TDE_HWNode_S* pstHWNode=NULL;
    TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
    TDE_DRV_SURFACE_S stDstDrvSurface = {0};
    TDE_SCANDIRECTION_S stSrcScanInfo = {0};
    TDE_SCANDIRECTION_S stDstScanInfo = {0};

    if ((NULL == pstBackGround) || (NULL == pstBackGroundRect)
        || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    TDE_UNIFY_RECT(pstBackGroundRect, pstDstRect);

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }
    if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_QUIKE_COPY, TDE_SRC1_BYPASS, 0)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    if (TdeOsiGetScanInfo(pstBackGround, pstBackGroundRect, pstDst, pstDstRect, HI_NULL,
                          &stSrcScanInfo,
                          &stDstScanInfo) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }


    TdeOsiConvertSurface(pstBackGround, pstBackGroundRect, &stSrcScanInfo, &stSrcDrvSurface, NULL);
	stSrcDrvSurface.bCma = bMMZForSrc;

    TdeHalNodeSetSrc1(pstHWNode, &stSrcDrvSurface);

    TdeOsiSetExtAlpha(pstBackGround, HI_NULL, pstHWNode); /* AI7D02681 */

    TdeOsiConvertSurface(pstDst, pstDstRect, &stDstScanInfo, &stDstDrvSurface, NULL);
    stDstDrvSurface.bCma = bMMZForDst;

    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, TDE2_OUTALPHA_FROM_NORM);

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }
    return HI_SUCCESS;
}
static HI_S32 TdeOsiCheckSingleSr2Para(TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S*  pstForeGroundRect,
                                       TDE2_SURFACE_S* pstDst, TDE2_RECT_S*  pstDstRect, TDE2_OPT_S* pstOpt)
{
    HI_BOOL bContainNullPtr;

    TDE_ASSERT(NULL != pstOpt);


    bContainNullPtr=  ((NULL == pstDst) || (NULL == pstDstRect) || (NULL == pstOpt)\
                                    || (NULL == pstForeGround) || (NULL == pstForeGroundRect));

    if (bContainNullPtr)
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    if (TdeOsiCheckSingleSrc2Opt(pstForeGround->enColorFmt, pstDst->enColorFmt, pstOpt) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (pstOpt->bResize)
    {
        if (TdeOsiCheckResizePara(pstForeGroundRect->u32Width, pstForeGroundRect->u32Height,
                                  pstDstRect->u32Width, pstDstRect->u32Height) < 0)
        {
            return HI_ERR_TDE_MINIFICATION;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiSingleSrc2Blit
* Description:   source2 operate realization
* Input:         pstBackGround: source2 bitmap info struct
*                pstBackGroundRect: source2 bitmap operate zone
*                pstDst: target bitmap info struct
*                pstDstRect: target bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/

STATIC HI_S32 TdeOsiSingleSrc2Blit(TDE_HANDLE s32Handle,
                                                                TDE2_SURFACE_S* pstForeGround,
                                                                TDE2_RECT_S  *pstForeGroundRect,
                                                                TDE2_SURFACE_S* pstDst,
                                                                TDE2_RECT_S  *pstDstRect,
                                                                TDE2_OPT_S* pstOpt, HI_BOOL bMMZForSrc, HI_BOOL bMMZForDst)
{
    TDE_HWNode_S* pstHWNode=NULL;
    TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
    TDE_DRV_SURFACE_S stDstDrvSurface = {0};
    TDE_SCANDIRECTION_S stSrcScanInfo = {0};
    TDE_SCANDIRECTION_S stDstScanInfo = {0};
    HI_U16 u16Code = 0;
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};
    TDE2_RECT_S stFGOptArea = {0};
    TDE2_RECT_S stDstOptArea = {0};
    TDE_FILTER_OPT stFilterOpt = {0};
    TDE2_RECT_S stForeRect = {0};
    TDE2_RECT_S stDstRect = {0};
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL bSetFilter;
    HI_BOOL bYCbCr422ColorFmt;
    HI_BOOL bYCbCr888ColorFmt;
    TDE_CLUT_USAGE_E enClutUsage;
    TDE2_SURFACE_S stTempForeGround;

    enClutUsage = TDE_CLUT_USAGE_BUTT;


    s32Ret = TdeOsiCheckSingleSr2Para(pstForeGround, pstForeGroundRect, pstDst, pstDstRect, pstOpt);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }


    HI_GFX_Memcpy(&stForeRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }
    if(TdeOsiSetClipPara(NULL, NULL, pstForeGround, &stForeRect, pstDst,
                      &stDstRect, pstOpt, pstHWNode) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_CLIP_AREA;
    }


    if (TdeOsiGetScanInfo(pstForeGround, &stForeRect, pstDst, &stDstRect, pstOpt,
                          &stSrcScanInfo, &stDstScanInfo) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }


    TdeOsiConvertSurface(pstForeGround, &stForeRect, &stSrcScanInfo, &stSrcDrvSurface, &stFGOptArea);

    stSrcDrvSurface.bCma = bMMZForSrc;
    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstDst, &stDstRect, &stDstScanInfo, &stDstDrvSurface, &stDstOptArea);

    if(TDE2_OUTALPHA_FROM_BUTT <= pstOpt->enOutAlphaFrom)
    {
        TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }

    if(TDE2_OUTALPHA_FROM_BACKGROUND == pstOpt->enOutAlphaFrom)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }
    stDstDrvSurface.bCma = bMMZForDst;
    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, pstOpt->enOutAlphaFrom);

    u16Code = TdeOsiSingleSrc2GetOptCode(pstForeGround->enColorFmt, pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);

    if(TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if((s32Ret = TdeOsiSetClutOpt(pstForeGround, pstDst, &enClutUsage, pstOpt->bClutReload, pstHWNode)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    TdeOsiSetBaseOptParaForBlit(pstOpt, HI_NULL, pstForeGround,TDE_OPERATION_SINGLE_SRC2, pstHWNode);

    TdeOsiSetExtAlpha(HI_NULL, pstForeGround, pstHWNode); /* AI7D02681 */

    bSetFilter =  ((pstOpt->bResize) || (pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE));
    if (bSetFilter)
    {
        HI_GFX_Memcpy(&stFilterOpt.stSrcDire, &stSrcScanInfo, sizeof(TDE_SCANDIRECTION_S));
        HI_GFX_Memcpy(&stFilterOpt.stDstDire, &stDstScanInfo, sizeof(TDE_SCANDIRECTION_S));

        stFilterOpt.enFilterMode = g_enTdeFilterMode[pstOpt->enFilterMode];

        HI_GFX_Memcpy(&stTempForeGround, pstForeGround, sizeof(TDE2_SURFACE_S));

        bYCbCr422ColorFmt = (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr422MBHP ||
            (pstForeGround->enColorFmt >= TDE2_COLOR_FMT_MP1_YCbCr420MBP && pstForeGround->enColorFmt <= TDE2_COLOR_FMT_JPG_YCbCr420MBP)
            || (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr400MBP));

        bYCbCr888ColorFmt = ((pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr422MBVP)
                 || (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr444MBP));

        if (bYCbCr422ColorFmt)
        {
            stTempForeGround.enColorFmt = TDE2_COLOR_FMT_YCbCr422;
        }
        else if (bYCbCr888ColorFmt)
        {
            stTempForeGround.enColorFmt = TDE2_COLOR_FMT_YCbCr888;
        }

        if ((s32Ret = TdeOsiSetFilterNode(s32Handle, pstHWNode, &stTempForeGround, pstDst,
                                   &stFGOptArea, &stDstOptArea, pstOpt->enDeflickerMode, &stFilterOpt)) < 0)
       {
            return s32Ret;
       }
       return HI_SUCCESS;
    }

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;


}

STATIC HI_S32 TdeOsiSetColorKey(TDE2_SURFACE_S* pstForeGround , TDE2_SURFACE_S* pstBackGround , TDE_HWNode_S* pstHWNode , TDE2_COLORKEY_U unColorKeyValue, TDE2_COLORKEY_MODE_E enColorKeyMode,  TDE_CLUT_USAGE_E enClutUsage)
{
    TDE_DRV_COLORKEY_CMD_S stColorkey;
    TDE_COLORFMT_CATEGORY_E enFmtCategory;
    HI_BOOL bUnknownFmtCategory;

    HI_BOOL bColorKeyForegroundBeforeClutMode =  (TDE_CLUT_COLOREXPENDING != enClutUsage) && (TDE_CLUT_CLUT_BYPASS != enClutUsage);
    HI_BOOL bSetColorKey = ( TDE2_COLORKEY_MODE_NONE != enColorKeyMode );
    stColorkey.unColorKeyValue = unColorKeyValue;

    if (!bSetColorKey)
    {
        return  HI_SUCCESS;
    }


    switch (enColorKeyMode)
    {
        case TDE2_COLORKEY_MODE_BACKGROUND:
            stColorkey.enColorKeyMode = TDE_DRV_COLORKEY_BACKGROUND;

            enFmtCategory = TdeOsiGetFmtCategory(pstBackGround->enColorFmt);

            break;

        case TDE2_COLORKEY_MODE_FOREGROUND:

            stColorkey.enColorKeyMode = (bColorKeyForegroundBeforeClutMode) ?
                                        TDE_DRV_COLORKEY_FOREGROUND_AFTER_CLUT : TDE_DRV_COLORKEY_FOREGROUND_BEFORE_CLUT;

            enFmtCategory = TdeOsiGetFmtCategory(pstForeGround->enColorFmt);

            break;

        default:
            TDE_TRACE(TDE_KERN_INFO, "\n");
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_INVALID_PARA;
    }

    bUnknownFmtCategory = enFmtCategory >= TDE_COLORFMT_CATEGORY_BUTT ;

    if (bUnknownFmtCategory)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TDE_TRACE(TDE_KERN_ERR, "Unknown fmt category!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (TdeHalNodeSetColorKey(pstHWNode, enFmtCategory, &stColorkey) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return HI_SUCCESS;
}
#endif
/*****************************************************************************
* Function:      TdeOsiDoubleSrcBlit
* Description:   dual source operate realization
* Input:         pstBackGround: source1 bitmap info struct
*                pstBackGroundRect: source1 bitmap operate zone
*                pstForeGround: source2 bitmap info struct
*                pstForeGroundRect: source2 bitmap operate zone
*                pstDst: target bitmap info struct
*                pstDstRect: target bitmap operate zone
*                pstOpt:config parameter
* Output:        none
* Return:        success/fail
* Others:        AI7D02579 add support for YCbCr422
*****************************************************************************/
STATIC HI_S32 TdeOsiDoubleSrcBlit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstDst,
                                  TDE2_RECT_S  *pstDstRect,
                                  TDE2_OPT_S* pstOpt)
{
    TDE_HWNode_S* pstHWNode = NULL;
    TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
    TDE_DRV_SURFACE_S stDstDrvSurface = {0};
    TDE_SCANDIRECTION_S stSrcScanInfo = {0};
    TDE_SCANDIRECTION_S stDstScanInfo = {0};
    HI_U16 u16Code = 0;
    TDE2_RECT_S stBGOptArea = {0};
    TDE2_RECT_S stFGOptArea = {0};
    TDE2_RECT_S stDstOptArea = {0};
    TDE_FILTER_OPT stFilterOpt = {0};
    HI_S32 s32Ret = HI_FAILURE;
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};
    TDE2_SURFACE_S stTempSur = {0};
    TDE2_RECT_S stForeRect = {0};
    TDE2_RECT_S stDstRect = {0};
    TDE2_RECT_S stBackRect = {0};
    HI_BOOL bYCbCr422ColorFmt;
    HI_BOOL bYCbCr888ColorFmt;
    HI_BOOL bSetFilter;
    TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;
    TDE2_SURFACE_S stTempForeGround;

   HI_BOOL bContainNullPtr = ((NULL == pstBackGround) || (NULL == pstBackGroundRect)\
                             || (NULL == pstForeGround) || (NULL == pstForeGroundRect)\
                             || (NULL == pstDst) || (NULL == pstDstRect)\
                             || (NULL == pstOpt));

    if ( bContainNullPtr )
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    HI_GFX_Memcpy(&stForeRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stBackRect, pstBackGroundRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    if (TdeOsiCheckDoubleSrcOpt(pstBackGround->enColorFmt, pstForeGround->enColorFmt, pstDst->enColorFmt,
                                pstOpt) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    if(pstOpt->bResize)
    {
        if (TdeOsiCheckResizePara(stForeRect.u32Width, stForeRect.u32Height,
                                  stDstRect.u32Width, stDstRect.u32Height) < 0)
        {
            return HI_ERR_TDE_MINIFICATION;
        }
    }

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }

    if(TdeOsiSetClipPara(pstBackGround, &stBackRect, pstForeGround, &stForeRect, pstDst,
                      &stDstRect, pstOpt, pstHWNode) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_CLIP_AREA;
    }


    if (TdeOsiGetScanInfo(pstForeGround, &stForeRect, pstDst, &stDstRect, pstOpt,
                          &stSrcScanInfo,
                          &stDstScanInfo) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }


    TdeOsiConvertSurface(pstBackGround, &stBackRect, &stSrcScanInfo, &stSrcDrvSurface, &stBGOptArea);


    HI_GFX_Memcpy(&stTempSur, pstBackGround, sizeof(TDE2_SURFACE_S));


    TdeHalNodeSetSrc1(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstForeGround, &stForeRect, &stSrcScanInfo, &stSrcDrvSurface, &stFGOptArea);


    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstDst, &stDstRect, &stDstScanInfo, &stDstDrvSurface, &stDstOptArea);

    if(TDE2_OUTALPHA_FROM_BUTT <= pstOpt->enOutAlphaFrom)
    {
        TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_INVALID_PARA;
    }

    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, pstOpt->enOutAlphaFrom);

    u16Code = TdeOsiDoubleSrcGetOptCode(stTempSur.enColorFmt, pstForeGround->enColorFmt,
                                        pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);


    if(TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if((s32Ret = TdeOsiSetClutOpt(pstForeGround, pstDst, &enClutUsage, pstOpt->bClutReload, pstHWNode)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

#ifndef HI_BUILD_IN_BOOT
    s32Ret = TdeOsiSetColorKey( pstForeGround , pstBackGround , pstHWNode , pstOpt->unColorKeyValue , pstOpt->enColorKeyMode , enClutUsage);

    if ( s32Ret )
    {
        return s32Ret;
    }

#endif

    TdeOsiSetBaseOptParaForBlit(pstOpt, &stTempSur, pstForeGround, TDE_OPERATION_DOUBLE_SRC, pstHWNode);
    TdeOsiSetExtAlpha(&stTempSur, pstForeGround,pstHWNode);

    bSetFilter = ((pstOpt->bResize) || (TDE2_DEFLICKER_MODE_NONE != pstOpt->enDeflickerMode));
    if (bSetFilter)
    {
	        TDE_DOUBLESRC_ADJ_INFO_S stDSAdjInfo;
	        stDSAdjInfo.bDoubleSource = HI_TRUE;
	        stDSAdjInfo.s32DiffX = stBGOptArea.s32Xpos - stDstOptArea.s32Xpos;
	        stDSAdjInfo.s32DiffY = stBGOptArea.s32Ypos - stDstOptArea.s32Ypos;


	        HI_GFX_Memcpy(&stFilterOpt.stSrcDire, &stSrcScanInfo, sizeof(TDE_SCANDIRECTION_S));
	        HI_GFX_Memcpy(&stFilterOpt.stDstDire, &stDstScanInfo, sizeof(TDE_SCANDIRECTION_S));


	        HI_GFX_Memcpy(&stFilterOpt.stDSAdjInfo, &stDSAdjInfo, sizeof(TDE_DOUBLESRC_ADJ_INFO_S));

	        stFilterOpt.enFilterMode = g_enTdeFilterMode[pstOpt->enFilterMode];

        HI_GFX_Memcpy(&stTempForeGround, pstForeGround, sizeof(TDE2_SURFACE_S));

        bYCbCr422ColorFmt = (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr422MBHP ||
                             (pstForeGround->enColorFmt >= TDE2_COLOR_FMT_MP1_YCbCr420MBP && pstForeGround->enColorFmt <= TDE2_COLOR_FMT_JPG_YCbCr420MBP)
                             || (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr400MBP));

        bYCbCr888ColorFmt = (pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr422MBVP
                             || pstForeGround->enColorFmt == TDE2_COLOR_FMT_JPG_YCbCr444MBP);


        if (bYCbCr422ColorFmt)
        {
            stTempForeGround.enColorFmt = TDE2_COLOR_FMT_YCbCr422;
        }
        else if (bYCbCr888ColorFmt)
        {
            stTempForeGround.enColorFmt = TDE2_COLOR_FMT_YCbCr888;
        }

        if ((s32Ret = TdeOsiSetFilterNode(s32Handle, pstHWNode, &stTempForeGround, pstDst,
                                          &stFGOptArea, &stDstOptArea, pstOpt->enDeflickerMode, &stFilterOpt)) < 0)
        {
            return s32Ret;
        }

        return HI_SUCCESS;
    }


    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;

}


/*****************************************************************************
* Function:      TdeOsiBlit
* Description:   operate pstBackGround with pstForeGround,which result output to pstDst,operate setting is in pstOpt
* Input:         enCmd: TDE operate command type
*                pstBackGround: background bitmap info struct
*                pstForeGround: foreground bitmap info struct
*                pstDst:  target bitmap info struct
*                pstOpt:  operate parameter setting struct
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBlit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                      TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect, TDE2_SURFACE_S* pstDst,
                      TDE2_RECT_S  *pstDstRect, TDE2_OPT_S* pstOpt)
{
    TDE_OPERATION_CATEGORY_E enOptCategory;
    HI_S32 s32Ret = 0;

    enOptCategory = TdeOsiGetOptCategory(pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect, pstDst,
                                         pstDstRect, pstOpt);

    switch (enOptCategory)
    {
    	#ifndef HI_BUILD_IN_BOOT
		    case TDE_OPERATION_SINGLE_SRC1:
		    {
		        if (NULL == pstBackGround)
		        {
		            return TdeOsiSingleSrc1Blit(s32Handle, pstForeGround, pstForeGroundRect,
		                                        pstDst, pstDstRect, HI_FALSE, HI_FALSE);
		        }
		        else
		        {
		            return TdeOsiSingleSrc1Blit(s32Handle, pstBackGround, pstBackGroundRect,
		                                        pstDst, pstDstRect, HI_FALSE, HI_FALSE);
		        }
		        break;
		    }
		    case TDE_OPERATION_SINGLE_SRC2:
		    {
		        if (NULL == pstBackGround)
		        {
		            if ((s32Ret = TdeOsiSingleSrc2Blit(s32Handle, pstForeGround, pstForeGroundRect,
		                                        pstDst, pstDstRect, pstOpt,HI_FALSE,HI_FALSE)) < 0)
		            {
		                return s32Ret;
		            }
		        }
		        else
		        {
		            if (((s32Ret = TdeOsiSingleSrc2Blit(s32Handle, pstBackGround, pstBackGroundRect,
		                                        pstDst, pstDstRect, pstOpt,HI_FALSE,HI_FALSE))) < 0 ) /*AI7D02940*/
		            {
		                return s32Ret;
		            }
		        }
		        break;
		    }
		#endif
	    case TDE_OPERATION_DOUBLE_SRC:
	    {
	        if ((s32Ret = TdeOsiDoubleSrcBlit(s32Handle, pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect,
	                                   pstDst, pstDstRect, pstOpt)) < 0)
	        {
	            return s32Ret;
	        }
	        break;
	    }
	    default:
	        return HI_ERR_TDE_INVALID_PARA;
    }
    return HI_SUCCESS;
}
#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiBlit);
#endif
static HI_S32 TdeOsiCheckMbBlitPara(TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect,
                                    TDE2_SURFACE_S* pstDst, TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pstMbOpt)
{
    HI_BOOL bContainNullPtr = ((HI_NULL == pstMB) || (HI_NULL == pstMbRect)\
                               || (HI_NULL == pstDst) || (HI_NULL == pstDstRect) || (HI_NULL == pstMbOpt));

    if (bContainNullPtr)
    {
        TDE_TRACE(TDE_KERN_INFO, "Contains NULL ptr!\n");
        return HI_ERR_TDE_NULL_PTR;
    }

#ifndef HI_BUILD_IN_BOOT
    TDE_CHECK_NOT_MB(pstDst->enColorFmt);
    TDE_CHECK_MBCOLORFMT(pstMB->enMbFmt);

    if (TdeOsiCheckSurfaceEX(pstDst, pstDstRect) < 0
    || TdeOsiCheckMbSurfaceEX(pstMB, pstMbRect) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }
#endif

    return HI_SUCCESS;

}
static HI_S32 TdeOsiGetDoubleCycleData(TDE2_RECT_S*  pstMbRect)
{
    HI_U32 u32FirstCycle = 1;
    HI_U32 u32SecondCycle = 16;
    HI_U32 u32FirstCycleBitsShift, u32SecondCycleBitsShift;

    if ((pstMbRect->u32Height > TDE_MAX_RECT_HEIGHT) && (pstMbRect->u32Width > TDE_MAX_RECT_WIDTH))
    {
        u32FirstCycleBitsShift  = TDE_OSI_ONE_BIT_SHIFT;
        u32SecondCycleBitsShift = TDE_OSI_ONE_BIT_SHIFT;
    }
    else if ((pstMbRect->u32Height > TDE_MAX_RECT_HEIGHT) && (pstMbRect->u32Width <= TDE_MAX_RECT_WIDTH))
    {
        u32FirstCycleBitsShift  = TDE_OSI_ZERO_BIT_SHIFT;
        u32SecondCycleBitsShift = TDE_OSI_ONE_BIT_SHIFT;
    }
    else if ((pstMbRect->u32Height <= TDE_MAX_RECT_HEIGHT) && (pstMbRect->u32Width > TDE_MAX_RECT_WIDTH))
    {
        u32FirstCycleBitsShift  = TDE_OSI_ONE_BIT_SHIFT;
        u32SecondCycleBitsShift = TDE_OSI_ZERO_BIT_SHIFT;
    }
    else
    {
        u32FirstCycleBitsShift  = TDE_OSI_ZERO_BIT_SHIFT;
        u32SecondCycleBitsShift = TDE_OSI_ZERO_BIT_SHIFT;
    }

    return ( u32FirstCycle << u32FirstCycleBitsShift ) + ( u32SecondCycle << u32SecondCycleBitsShift );
}

/*****************************************************************************
* Function:      TdeOsiMbBlit
* Description:   Mb blit
* Input:         s32Handle: task handle
*                pstMB: MB bitmap information struct
*                pstMbRect: MB bitmap operate zone
*                pstDst: target bitmap information struct
*                pstDstRect:  target bitmap operate zone
*                pstMbOpt:  operate parameter setting struct
* Output:        none
* Return:        >0: return task id of current operate; <0: fail
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiMbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect,
                        TDE2_SURFACE_S* pstDst, TDE2_RECT_S*  pstDstRect, TDE2_MBOPT_S* pstMbOpt)
{
    HI_U32 phy, u32YPhyAddr, u32CbCrPhyAddr, u32BytePerPixel, u32CbCrBytePerPixel;
    HI_U32 u32Height, u32Width, i, j, m, n;
    HI_S32 sRet, s32Bpp, s32DoubleCycleData;
    HI_U32 u32CbCrHeight = 0;
    HI_BOOL bTwoBytePerPixel, bHalfMaxRectHeight;

    sRet = TdeOsiCheckMbBlitPara(pstMB, pstMbRect, pstDst, pstDstRect, pstMbOpt);

    if (sRet != HI_SUCCESS)
    {
        return sRet;
    }

    if((pstDstRect->u32Height == pstMbRect->u32Height)
    &&(pstDstRect->u32Width == pstMbRect->u32Width))
    {
        pstMbOpt->enResize = TDE2_MBRESIZE_NONE;
    }

    if (TDE2_MBRESIZE_NONE == pstMbOpt->enResize)
    {
        TDE_UNIFY_RECT(pstMbRect, pstDstRect);
    }

    if (TdeOsiCheckResizePara(pstMbRect->u32Width, pstMbRect->u32Height,
                              pstDstRect->u32Width, pstDstRect->u32Height) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "The Scale is too large!\n");
        return HI_ERR_TDE_MINIFICATION;
    }


    u32Height = pstMbRect->u32Height;
    u32Width = pstMbRect->u32Width;
    u32YPhyAddr= pstMB->u32YPhyAddr;
    u32CbCrPhyAddr = pstMB->u32CbCrPhyAddr;
    phy = pstDst->u32PhyAddr;


    bTwoBytePerPixel = ((pstMB->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP) || \
                        (pstMB->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP));

    u32CbCrBytePerPixel = bTwoBytePerPixel ? 2 : 1;

    s32Bpp = TdeOsiGetbppByFmt(pstDst->enColorFmt);
    u32BytePerPixel = (s32Bpp >> 3);

    bHalfMaxRectHeight = ((pstMB->enMbFmt == TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP) || (pstMB->enMbFmt == TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP)\
                          || (pstMB->enMbFmt == TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI) || (pstMB->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP)\
                          || (pstMB->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP));

    u32CbCrHeight = bHalfMaxRectHeight ? TDE_MAX_RECT_HEIGHT / 2 : TDE_MAX_RECT_HEIGHT;

    s32DoubleCycleData = TdeOsiGetDoubleCycleData(pstMbRect);
    i = ( s32DoubleCycleData >> TDE_OSI_ZERO_BIT_SHIFT ) & 0xf;
    j = ( s32DoubleCycleData >> TDE_OSI_FOUR_BITS_SHIFT ) & 0xf;


    pstDstRect->u32Width =  ((1 == i) && (1 == j)) ? pstDstRect->u32Width : pstMbRect->u32Width;
    pstDstRect->u32Height =  ((1 == i) && (1 == j)) ? pstDstRect->u32Height : pstMbRect->u32Height;

    for (n = 0; n < j; n++)
    {
        for (m = 0; m < i; m++)
        {
            pstMbRect->u32Width = ((i - 1) == m) ? ( u32Width - m * TDE_MAX_RECT_WIDTH ) : TDE_MAX_RECT_WIDTH;

            if (u32Height > TDE_MAX_RECT_HEIGHT)
            {
                pstMbRect->u32Height = TDE_MAX_RECT_HEIGHT;
            }

            pstMbRect->u32Height = ((j - 1) == n) ? ( u32Height - n * TDE_MAX_RECT_HEIGHT ) : TDE_MAX_RECT_HEIGHT;

            pstMB->u32YPhyAddr = u32YPhyAddr + m * TDE_MAX_RECT_WIDTH + n * pstMB->u32YStride * TDE_MAX_RECT_HEIGHT;
            pstDst->u32PhyAddr = phy + m * u32BytePerPixel * TDE_MAX_RECT_WIDTH + n * pstDst->u32Stride * TDE_MAX_RECT_HEIGHT;
            pstMB->u32CbCrPhyAddr = u32CbCrPhyAddr + m * u32CbCrBytePerPixel * TDE_MAX_RECT_WIDTH + n * pstMB->u32CbCrStride * u32CbCrHeight;

            if ((sRet = TdeOsiSetMbPara(s32Handle, pstMB, pstMbRect, pstDst, pstDstRect, pstMbOpt)) < 0)
            {
                return sRet;
            }

        }
    }
    return HI_SUCCESS;
}


#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiMbBlit);

/*****************************************************************************
* Function:      TdeOsiBitmapMaskCheckPara
* Description:   check for trinal source operate parameter
* Input:         pstBackGround: background bitmap
*                pstBackGroundRect: background operate zone
*                pstForeGround: foreground bitmap
*                pstForeGroundRect: foreground operate zone
*                pstMask:  Mask bitmap
*                pstMaskRect:  Mask bitmap operate zone
*                pstDst:  target bitmap
*                pstDstRect: target bitmap operate zone
* Output:        none
* Return:        =0: success; <0: fail
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiBitmapMaskCheckPara(TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                               TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                               TDE2_SURFACE_S* pstMask,
                                               TDE2_RECT_S  *pstMaskRect, TDE2_SURFACE_S* pstDst,
                                               TDE2_RECT_S  *pstDstRect)
{

    HI_BOOL bContainNullPtr = ((NULL == pstBackGround) || (NULL == pstBackGroundRect) || (NULL == pstForeGround)
        || (NULL == pstForeGroundRect)
        || (NULL == pstMask) || (NULL == pstMaskRect) || (NULL == pstDst) || (NULL == pstDstRect));

    if (bContainNullPtr)
    {
        TDE_TRACE(TDE_KERN_INFO, "Contains NULL ptr!\n");
        return HI_ERR_TDE_NULL_PTR;
    }


    TDE_CHECK_NOT_MB(pstForeGround->enColorFmt);
    TDE_CHECK_NOT_MB(pstBackGround->enColorFmt);
    TDE_CHECK_NOT_MB(pstMask->enColorFmt);
    TDE_CHECK_NOT_MB(pstDst->enColorFmt);

    if (TdeOsiCheckSurface(pstBackGround, pstBackGroundRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "pstBackGroundRect does not correct!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }


    if (TdeOsiCheckSurface(pstForeGround, pstForeGroundRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "pstForeGroundRect does not correct!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }


    if (TdeOsiCheckSurface(pstMask, pstMaskRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "pstMaskRect does not correct!\n");

        return HI_ERR_TDE_INVALID_PARA;
    }


    if (TdeOsiCheckSurface(pstDst, pstDstRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "pstDstRect does not correct!\n");

        return HI_ERR_TDE_INVALID_PARA;
    }

    if ((pstBackGroundRect->u32Width != pstForeGroundRect->u32Width)
        || (pstForeGroundRect->u32Width != pstMaskRect->u32Width)
        || (pstMaskRect->u32Width != pstDstRect->u32Width)
        || (pstBackGroundRect->u32Height != pstForeGroundRect->u32Height)
        || (pstForeGroundRect->u32Height != pstMaskRect->u32Height)
        || (pstMaskRect->u32Height != pstDstRect->u32Height))
    {
        TDE_TRACE(TDE_KERN_INFO, "Don't support scale!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiBitmapMaskRop
* Description:   Ropmask foreground and mask at firs, and then ropmask background and middle bitmap
*                output result to target bitmap
* Input:         pstBackGround: background bitmap info struct
*                pstForeGround: foreground bitmap info struct
*                pMask: fask code operate bitmap info
*                pstDst:  target bitmap info
*                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBitmapMaskRop(TDE_HANDLE s32Handle,
                    TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                    TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                    TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect,
                    TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                    TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha)
{
    TDE_HWNode_S* pstHWNode = NULL;
    TDE_HWNode_S* pstHWNodePass2 = NULL;
    TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
    TDE_DRV_SURFACE_S stDstDrvSurface = {0};
    TDE_SCANDIRECTION_S stScanInfo= {0};

    TDE2_SURFACE_S stMidSurface = {0};
    TDE2_RECT_S stMidRect = {0};
    HI_U16 u16Code = 0;
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};

    HI_S32 ret;

    if ((ret = TdeOsiBitmapMaskCheckPara(pstBackGround, pstBackGroundRect,
                                         pstForeGround, pstForeGroundRect, pstMask,
                                         pstMaskRect, pstDst, pstDstRect)) < 0)
    {
        return ret;
    }

    if(!TdeOsiWhetherContainAlpha(pstForeGround->enColorFmt))
    {
        TDE_TRACE(TDE_KERN_INFO, "ForeGround bitmap must contains alpha component!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (TDE2_COLOR_FMT_A1 != pstMask->enColorFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "Maskbitmap's colorformat can only be A1 in TdeOsiBitmapMaskRop!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }

    stScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
    stScanInfo.enVScan = TDE_SCAN_UP_DOWN;


    TdeOsiConvertSurface(pstForeGround, pstForeGroundRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc1(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstMask, pstMaskRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    HI_GFX_Memcpy(&stMidSurface, pstForeGround, sizeof(TDE2_SURFACE_S));

    stMidSurface.u32PhyAddr = TdeOsiListGetPhyBuff(0);
    stMidSurface.pu8ClutPhyAddr = NULL;

    stMidRect.s32Xpos   = 0;
    stMidRect.s32Ypos   = 0;
    stMidRect.u32Height = pstForeGroundRect->u32Height;
    stMidRect.u32Width  = pstForeGroundRect->u32Width;


    TdeOsiConvertSurface(&stMidSurface, &stMidRect, &stScanInfo, &stDstDrvSurface, NULL);


    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, TDE2_OUTALPHA_FROM_NORM);

    TdeOsiSetExtAlpha(pstForeGround, &stMidSurface, pstHWNode);


    /* logical operation first passs */
    if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_ROP1, 0)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    if ((ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeOsiListPutPhyBuff(1);
        TdeHalFreeNodeBuf(pstHWNode);
        return ret;
    }

    if(TdeHalNodeInitNd(&pstHWNodePass2)<0)
    {
        TdeOsiListPutPhyBuff(1);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_NO_MEM;
    }

    TdeOsiConvertSurface(pstBackGround, pstBackGroundRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc1(pstHWNodePass2, &stSrcDrvSurface);


    TdeHalNodeSetSrc2(pstHWNodePass2, &stDstDrvSurface);


    TdeOsiConvertSurface(pstDst, pstDstRect, &stScanInfo, &stDstDrvSurface, NULL);


    TdeHalNodeSetTgt(pstHWNodePass2, &stDstDrvSurface, TDE2_OUTALPHA_FROM_NORM);


    u16Code = TdeOsiSingleSrc2GetOptCode(pstForeGround->enColorFmt, pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);


    if(TdeHalNodeSetColorConvert(pstHWNodePass2, &stConv)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if(TdeHalNodeSetRop(pstHWNodePass2, enRopCode_Color, enRopCode_Alpha)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    TdeOsiSetExtAlpha(pstBackGround, &stMidSurface,pstHWNodePass2);

    /* logical operation second passs */
    if(TdeHalNodeSetBaseOperate(pstHWNodePass2, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_ROP2, 0)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    ret = TdeOsiSetNodeFinish(s32Handle,pstHWNodePass2, 1, TDE_NODE_SUBM_ALONE);
    if (ret < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        TdeOsiListPutPhyBuff(1);
        return ret;
    }

    return HI_SUCCESS;
}

EXPORT_SYMBOL(TdeOsiBitmapMaskRop);

/*****************************************************************************
* Function:      TdeOsiBitmapMaskBlend
* Description:   blendmask foreground and mask bitmap at first,and blend background with middle bitmap
*                output reslut to target bitmap
* Input:         pstBackGround: background bitmap info struct
*                pstForeGround: foreground bitmap info struct
*                pMask: fask code operate bitmap info
*                pstDst:  target bitmap info
*                u8Alpha:  alpha value operated
*                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBitmapMaskBlend(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround,
                                 TDE2_RECT_S  *pstBackGroundRect,
                                 TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                 TDE2_SURFACE_S* pstMask,
                                 TDE2_RECT_S  *pstMaskRect, TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                 HI_U8 u8Alpha,
                                 TDE2_ALUCMD_E enBlendMode)
{
    TDE_HWNode_S* pstHWNode = NULL;
    TDE_HWNode_S* pstHWNodePass2 = NULL;
    TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
    TDE_DRV_SURFACE_S stDstDrvSurface = {0};
    TDE_SCANDIRECTION_S stScanInfo = {0};
    TDE2_SURFACE_S stMidSurface = {0};
    TDE2_RECT_S stMidRect = {0};
    TDE_DRV_ALU_MODE_E enDrvAluMode = TDE_SRC1_BYPASS;
    HI_U16 u16Code = 0;
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};
    TDE2_BLEND_OPT_S stBlendOpt = {0};

    HI_S32 ret;

    if ((ret = TdeOsiBitmapMaskCheckPara(pstBackGround, pstBackGroundRect,
                                         pstForeGround, pstForeGroundRect, pstMask,
                                         pstMaskRect, pstDst, pstDstRect)) < 0)
    {
        return ret;
    }

    if ((TDE2_COLOR_FMT_A1 != pstMask->enColorFmt)
        &&(TDE2_COLOR_FMT_A8 != pstMask->enColorFmt))
    {
        TDE_TRACE(TDE_KERN_INFO, "Maskbitmap's colorformat can only be An in TdeOsiBitmapMaskBlend!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (TDE2_ALUCMD_BLEND == enBlendMode)
    {
        enDrvAluMode = TDE_ALU_BLEND;
    }
    else
    {
        TDE_TRACE(TDE_KERN_INFO, "Alum mode can only be blending in TdeOsiBitmapMaskBlend!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }

    stScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
    stScanInfo.enVScan = TDE_SCAN_UP_DOWN;


    TdeOsiConvertSurface(pstForeGround, pstForeGroundRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc1(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstMask, pstMaskRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    HI_GFX_Memcpy(&stMidSurface, pstForeGround, sizeof(TDE2_SURFACE_S));

    stMidSurface.u32PhyAddr = TdeOsiListGetPhyBuff(0);

    stMidRect.s32Xpos   = 0;
    stMidRect.s32Ypos   = 0;
    stMidRect.u32Height = pstForeGroundRect->u32Height;
    stMidRect.u32Width  = pstForeGroundRect->u32Width;


    TdeOsiConvertSurface(&stMidSurface, &stMidRect, &stScanInfo, &stDstDrvSurface, NULL);


    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, TDE2_OUTALPHA_FROM_NORM);


    TdeOsiSetExtAlpha(pstForeGround, &stMidSurface,pstHWNode);


    if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_BLEND, 0)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }


    TdeHalNodeSetGlobalAlpha(pstHWNode, 0xff, HI_TRUE);


    if ((ret = TdeOsiSetNodeFinish(s32Handle,pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeOsiListPutPhyBuff(1);
        TdeHalFreeNodeBuf(pstHWNode);
        return ret;
    }

    if(TdeHalNodeInitNd(&pstHWNodePass2)<0)
    {
        TdeOsiListPutPhyBuff(1);
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_NO_MEM;
    }
    TdeOsiConvertSurface(pstBackGround, pstBackGroundRect, &stScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc1(pstHWNodePass2, &stSrcDrvSurface);


    TdeHalNodeSetSrc2(pstHWNodePass2, &stDstDrvSurface);


    TdeOsiConvertSurface(pstDst, pstDstRect, &stScanInfo, &stDstDrvSurface, NULL);


    TdeHalNodeSetTgt(pstHWNodePass2, &stDstDrvSurface, TDE2_OUTALPHA_FROM_NORM);


    u16Code = TdeOsiSingleSrc2GetOptCode(pstForeGround->enColorFmt, pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);


    if(TdeHalNodeSetColorConvert(pstHWNodePass2, &stConv)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    TdeHalNodeSetGlobalAlpha(pstHWNodePass2, u8Alpha, HI_TRUE);


    if(TdeHalNodeSetBaseOperate(pstHWNodePass2, TDE_NORM_BLIT_2OPT, enDrvAluMode, 0)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    stBlendOpt.bGlobalAlphaEnable = HI_TRUE;
    stBlendOpt.bPixelAlphaEnable = HI_TRUE;
    stBlendOpt.eBlendCmd = TDE2_BLENDCMD_NONE;
    if(TdeHalNodeSetBlend(pstHWNodePass2, &stBlendOpt)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    TdeOsiSetExtAlpha(pstBackGround, &stMidSurface,pstHWNodePass2);


    ret = TdeOsiSetNodeFinish(s32Handle,pstHWNodePass2, 1, TDE_NODE_SUBM_ALONE);
    if (ret < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        TdeHalFreeNodeBuf(pstHWNodePass2);
        TdeOsiListPutPhyBuff(1);
        return ret;
    }

    return HI_SUCCESS;
}
EXPORT_SYMBOL(TdeOsiBitmapMaskBlend);

/*****************************************************************************
* Function:      TdeOsiSolidDraw
* Description:   operate src1 with src2, which result to pstDst,operate setting is in pstOpt
*                if src is MB, only support single source operate, just to say to only support pstBackGround or pstForeGround
* Input:         s32Handle: task handle
*                pSrc: background bitmap info struct
*                pstDst: foreground bitmap info struct
*                pstFillColor:  fill  color
*                pstOpt: operate parameter setting struct
* Output:        none
* Return:        HI_SUCCESS/HI_FAILURE
* Others:        none
*****************************************************************************/
HI_S32  TdeOsiSolidDraw(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                        TDE2_SURFACE_S *pstDst,
                        TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                        TDE2_OPT_S *pstOpt)
{

    if (NULL == pstSrc)
    {
        return TdeOsi1SourceFill(s32Handle, pstDst, pstDstRect, pstFillColor, pstOpt);
    }
    else
    {
        if ((NULL == pstSrc) || (NULL == pstDst) || (NULL == pstFillColor))
        {
            return HI_ERR_TDE_NULL_PTR;
        }

        TDE_CHECK_NOT_MB(pstSrc->enColorFmt);
        TDE_CHECK_NOT_MB(pstDst->enColorFmt);
        TDE_CHECK_NOT_MB(pstFillColor->enColorFmt);
        return TdeOsi2SourceFill(s32Handle, pstSrc, pstSrcRect, pstDst, pstDstRect, pstFillColor, pstOpt);
    }
}
EXPORT_SYMBOL(TdeOsiSolidDraw);

HI_S32 TdeOsiSetDeflickerLevel(TDE_DEFLICKER_LEVEL_E eDeflickerLevel)
{
    return TdeHalSetDeflicerLevel(eDeflickerLevel);
}
EXPORT_SYMBOL(TdeOsiSetDeflickerLevel);

HI_S32 TdeOsiGetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel)
{
    if (HI_NULL == pDeflickerLevel)
    {
        return HI_FAILURE;
    }
    return TdeHalGetDeflicerLevel(pDeflickerLevel);
}
EXPORT_SYMBOL(TdeOsiGetDeflickerLevel);

#endif

HI_S32 TdeOsiSetAlphaThresholdValue(HI_U8 u8ThresholdValue)
{
    return TdeHalSetAlphaThreshold(u8ThresholdValue);
}
#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiSetAlphaThresholdValue);
#endif

HI_S32 TdeOsiSetAlphaThresholdState(HI_BOOL bEnAlphaThreshold)
{
    return TdeHalSetAlphaThresholdState(bEnAlphaThreshold);
}


#ifndef HI_BUILD_IN_BOOT
EXPORT_SYMBOL(TdeOsiSetAlphaThresholdState);

HI_S32 TdeOsiGetAlphaThresholdValue(HI_U8 *pu8ThresholdValue)
{
    if (HI_NULL == pu8ThresholdValue)
    {
        return HI_FAILURE;
    }
    return TdeHalGetAlphaThreshold(pu8ThresholdValue);
}
EXPORT_SYMBOL(TdeOsiGetAlphaThresholdValue);

HI_S32 TdeOsiGetAlphaThresholdState(HI_BOOL *pbEnAlphaThreshold)
{
    if (HI_NULL == pbEnAlphaThreshold)
    {
        return HI_FAILURE;
    }

    return TdeHalGetAlphaThresholdState(pbEnAlphaThreshold);
}
/*****************************************************************************
* Function:      TdeOsiCheckSingleSrcPatternOpt
* Description:   check if single source mode fill operate is valid
* Input:         enSrcFmt foreground pixel format
                 enDstFmt  target pixel format
                 pstOpt    operate attibute pointer
* Output:        none
* Return:        0  valid parameter
                 -1 invalid parameter
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiCheckSingleSrcPatternOpt(TDE2_COLOR_FMT_E enSrcFmt,
        TDE2_COLOR_FMT_E enDstFmt, TDE2_PATTERN_FILL_OPT_S *pstOpt)
{
    TDE_COLORFMT_TRANSFORM_E enColorTransType;

    enColorTransType = TdeOsiGetFmtTransType(enSrcFmt, enDstFmt);
    if(TDE_COLORFMT_TRANSFORM_BUTT == enColorTransType)
    {
        TDE_TRACE(TDE_KERN_INFO, "Unkown color transport type!\n");
        return -1;
    }

    if (pstOpt != HI_NULL)
    {
        TDE_CHECK_COLORKEYMODE(pstOpt->enColorKeyMode);

        if (TDE2_COLORKEY_MODE_NONE != pstOpt->enColorKeyMode)
        {
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support colorkey in single source pattern mode!\n");
            return -1;
        }

        TDE_CHECK_ALUCMD(pstOpt->enAluCmd);

        if (pstOpt->enAluCmd & TDE2_ALUCMD_ROP)
        {
            if ((!TdeOsiIsSingleSrc2Rop(pstOpt->enRopCode_Alpha))
                || (!TdeOsiIsSingleSrc2Rop(pstOpt->enRopCode_Color)))
            {
                TDE_TRACE(TDE_KERN_INFO, "Only support single s2 rop!\n");
                return -1;
            }
        }
        else if(TDE2_ALUCMD_NONE != pstOpt->enAluCmd)
        {
            TDE_TRACE(TDE_KERN_INFO, "Alu mode error!\n");
            return -1;
        }
        if(TDE_COLORFMT_TRANSFORM_CLUT_CLUT == enColorTransType)
        {
            if ((pstOpt->enAluCmd != TDE2_ALUCMD_NONE))
            {
                TDE_TRACE(TDE_KERN_INFO, "It doesn't ROP/Blend/Colorize!\n");
                return -1;
            }

            if (enSrcFmt != enDstFmt)
            {
                TDE_TRACE(TDE_KERN_INFO, "If src fmt and dst fmt are clut, they shoulod be the same fmt!\n");
                return -1;
            }
        }
    }
    return HI_SUCCESS;
}
/*****************************************************************************
* Function:      TdeOsiCheckDoubleSrcPatternOpt
* Description:   check if doul source mode fill operate parameter is valid
* Input:         enBackGroundFmt background pixel format
                 enForeGroundFmt foreground pixel format
                 enDstFmt  target pixel format
                 pstOpt     operate attribute pointer
* Output:        none
* Return:        0  valid parameter
                 -1 invalid parameter
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiCheckDoubleSrcPatternOpt(TDE2_COLOR_FMT_E enBackGroundFmt, TDE2_COLOR_FMT_E enForeGroundFmt,
                                        TDE2_COLOR_FMT_E enDstFmt, TDE2_PATTERN_FILL_OPT_S *pstOpt)
{
    TDE_COLORFMT_CATEGORY_E enBackGroundCategory;
    TDE_COLORFMT_CATEGORY_E enForeGroundCategory;
    TDE_COLORFMT_CATEGORY_E enDstCategory;
    HI_BOOL bUnknownColorFormat ;


    TDE_CHECK_ALUCMD(pstOpt->enAluCmd);

    enBackGroundCategory = TdeOsiGetFmtCategory(enBackGroundFmt);


    enForeGroundCategory = TdeOsiGetFmtCategory(enForeGroundFmt);


    enDstCategory = TdeOsiGetFmtCategory(enDstFmt);

    bUnknownColorFormat = (TDE_COLORFMT_CATEGORY_BYTE <= enBackGroundCategory)\
                                  || (TDE_COLORFMT_CATEGORY_BYTE <= enForeGroundCategory)\
                                  || (TDE_COLORFMT_CATEGORY_BYTE <= enDstCategory);

    if (bUnknownColorFormat)
    {
        TDE_TRACE(TDE_KERN_INFO, "Unknown color format!\n");
        return -1;
    }

    TDE_PATTERNBACKGROUNDCATEGORY_CHECK_ARGB(enBackGroundCategory, enForeGroundCategory, enDstFmt);

    TDE_PATTERNFOREGROUNDCATEGORY_CHECK_AN(enForeGroundCategory, enBackGroundFmt, enForeGroundFmt);

    TDE_PATTERNBACKGROUNDCATEGORY_CHECK_CLUT(enBackGroundCategory, enForeGroundCategory, enBackGroundFmt, \
            enForeGroundFmt, enDstFmt, pstOpt->enAluCmd , enDstCategory);

    TDE_PATTERNBACKGROUNDCATEGORY_CHECK_AN(enBackGroundCategory, enForeGroundCategory, enDstCategory ,  pstOpt->enAluCmd );

    TDE_PATTERNDSTCATEGORY_CHECK_CLUT(enBackGroundCategory, enForeGroundCategory, enDstCategory);


    return HI_SUCCESS;

}
STATIC TDE_PATTERN_OPERATION_CATEGORY_E TdeOsiSingleSrcPatternOperation(TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S*     pstBackGroundRect,
        TDE2_SURFACE_S* pstForeGround,
        TDE2_RECT_S*     pstForeGroundRect,
        TDE2_SURFACE_S* pstDst,
        TDE2_RECT_S*     pstDstRect,
        TDE2_PATTERN_FILL_OPT_S*      pstOpt)
{
    TDE2_SURFACE_S* pTmpSrc2 = HI_NULL;
    TDE2_RECT_S* pTmpSrc2Rect = HI_NULL;

    if (pstBackGround != NULL)
    {
        if (HI_NULL == pstBackGroundRect)
        {
            TDE_TRACE(TDE_KERN_INFO, "Background rect shouldn't be NULL!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }

        pTmpSrc2 = pstBackGround;
        pTmpSrc2Rect = pstBackGroundRect;
    }
    else if (pstForeGround != NULL)
    {
        if (HI_NULL == pstForeGroundRect)
        {
            TDE_TRACE(TDE_KERN_INFO, "Foreground rect shouldn't be NULL!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }

        pTmpSrc2 = pstForeGround;
        pTmpSrc2Rect = pstForeGroundRect;
    }

    if (pTmpSrc2 != HI_NULL)
    {
        TDE_CHECK_NOT_MB(pTmpSrc2->enColorFmt);


        if (TdeOsiCheckSurface(pTmpSrc2, pTmpSrc2Rect) < 0)
        {
            TDE_TRACE(TDE_KERN_INFO, "\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }

        if (pTmpSrc2Rect->u32Width > 256)
        {
            TDE_TRACE(TDE_KERN_INFO, "Max pattern width is 256!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }
    }

    return TDE_PATTERN_OPERATION_SINGLE_SRC;

}

STATIC TDE_PATTERN_OPERATION_CATEGORY_E TdeOsiDoubleSrcPatternOperation(TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S*     pstBackGroundRect,
        TDE2_SURFACE_S* pstForeGround,
        TDE2_RECT_S*     pstForeGroundRect,
        TDE2_RECT_S*     pstDstRect,
        TDE2_PATTERN_FILL_OPT_S*      pstOpt)
{
    if ((NULL == pstBackGroundRect) || (NULL == pstForeGroundRect) || (NULL == pstOpt))
    {
        TDE_TRACE(TDE_KERN_INFO, "Background Rect/Foreground Rect/Opt should not be null in two src pattern fill!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    TDE_CHECK_NOT_MB(pstBackGround->enColorFmt);
    TDE_CHECK_NOT_MB(pstForeGround->enColorFmt);

    if ((TdeOsiCheckSurface(pstBackGround, pstBackGroundRect) < 0)|| (TdeOsiCheckSurface(pstForeGround, pstForeGroundRect) < 0))
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if ((pstBackGroundRect->u32Height != pstDstRect->u32Height)
        || (pstBackGroundRect->u32Width != pstDstRect->u32Width))
    {
        TDE_TRACE(TDE_KERN_INFO, "Size of background rect and Dst rect should be the same in two src pattern fill.\
                    background x:%d, y:%d, w:%d, h:%d;dst x:%d, y:%d, w:%d, h:%d\n", pstBackGroundRect->s32Xpos, \
                  pstBackGroundRect->s32Ypos, pstBackGroundRect->u32Width, pstBackGroundRect->u32Height, \
                  pstDstRect->s32Xpos, pstDstRect->s32Ypos, pstDstRect->u32Width, pstDstRect->u32Height);
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (pstForeGroundRect->u32Width > 256)
    {
        TDE_TRACE(TDE_KERN_INFO, "Max pattern width is 256!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    return TDE_PATTERN_OPERATION_DOUBLE_SRC;
}

/*****************************************************************************
* Function:      TdeOsiGetPatternOptCategory
* Description:   analyze pattern fill operate type
* Input:         pstBackGround: background bitmap info
*                pstBackGroundRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstForeGroundRect: foreground bitmap operate zone
*                pstDst: target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
* Output:        none
* Return:        TDE operate type
* Others:        none
*****************************************************************************/
TDE_PATTERN_OPERATION_CATEGORY_E TdeOsiGetPatternOptCategory(TDE2_SURFACE_S* pstBackGround,
    TDE2_RECT_S  *pstBackGroundRect, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
    TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect, TDE2_PATTERN_FILL_OPT_S* pstOpt)
{
    if ((NULL == pstDst) || (NULL == pstDstRect))
    {
        TDE_TRACE(TDE_KERN_INFO, "Dst/Dst Rect should not be null!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    TDE_CHECK_NOT_MB(pstDst->enColorFmt);


    if (TdeOsiCheckSurface(pstDst, pstDstRect) < 0)
    {
        TDE_TRACE(TDE_KERN_INFO, "\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if ((NULL == pstBackGround) && (NULL == pstForeGround))
    {
        TDE_TRACE(TDE_KERN_INFO, "No src!");
        return TDE_PATTERN_OPERATION_BUTT;
    }
    else if ((NULL != pstBackGround) && (NULL != pstForeGround))
    {
        return TdeOsiDoubleSrcPatternOperation(pstBackGround, pstBackGroundRect , pstForeGround , \
                                               pstForeGroundRect , pstDstRect , pstOpt);
    }
    else
    {
       return TdeOsiSingleSrcPatternOperation(pstBackGround, pstBackGroundRect , pstForeGround , \
                                               pstForeGroundRect ,pstDst, pstDstRect , pstOpt);
    }
}

HI_BOOL TdeOsiCheckOverlap(TDE2_SURFACE_S *pstSur1, TDE2_RECT_S *pstRect1, TDE2_SURFACE_S *pstSur2, TDE2_RECT_S *pstRect2)
{
    HI_U32 u32Rect1StartPhy;
    HI_U32 u32Rect1EndPhy;
    HI_U32 u32Rect2StartPhy;
    HI_U32 u32Rect2EndPhy;
    HI_U32 u32Bpp1;
    HI_U32 u32Bpp2;

    u32Bpp1 = TdeOsiGetbppByFmt(pstSur1->enColorFmt) / 8;
    u32Bpp2 = TdeOsiGetbppByFmt(pstSur2->enColorFmt) / 8;

    u32Rect1StartPhy = pstSur1->u32PhyAddr + (pstRect1->s32Ypos * pstSur1->u32Stride) \
        + pstRect1->s32Xpos * u32Bpp1;
    u32Rect1EndPhy = u32Rect1StartPhy + (pstRect1->u32Height - 1) * pstSur1->u32Stride \
        + (pstRect1->u32Width - 1) * u32Bpp1;

    u32Rect2StartPhy = pstSur2->u32PhyAddr + (pstRect2->s32Ypos * pstSur2->u32Stride) \
        + pstRect2->s32Xpos * u32Bpp2;
    u32Rect2EndPhy = u32Rect2StartPhy + (pstRect2->u32Height - 1) * pstSur2->u32Stride \
        + (pstRect2->u32Width - 1) * u32Bpp2;

    TDE_TRACE(TDE_KERN_DEBUG, "u32Rect1StartPhy:%x, u32Rect1EndPhy:%x, u32Rect2StartPhy:%x, \
        u32Rect2EndPhy:%x\n", u32Rect1StartPhy, u32Rect1EndPhy, u32Rect2StartPhy, u32Rect2EndPhy);
    if (((u32Rect1StartPhy >= u32Rect2StartPhy) && (u32Rect1StartPhy <= u32Rect2EndPhy)) \
        || ((u32Rect1EndPhy >= u32Rect2StartPhy) && (u32Rect1EndPhy <= u32Rect2EndPhy)))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}
static HI_S32 TdeOsiCheckSingleSrcPatternFillPara(TDE2_SURFACE_S* pstSrc,  TDE2_RECT_S* pstSrcRect,
        TDE2_SURFACE_S* pstDst, TDE2_RECT_S* pstDstRect, TDE2_PATTERN_FILL_OPT_S* pstOpt)
{
    HI_BOOL bContainNullPtr = ((NULL == pstSrc) || (NULL == pstSrcRect)\
                               || (NULL == pstDst) || (NULL == pstDstRect));

    if (bContainNullPtr)
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    if (TdeOsiCheckSingleSrcPatternOpt(pstSrc->enColorFmt, pstDst->enColorFmt, pstOpt) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }


    if (TdeOsiCheckOverlap(pstSrc, pstSrcRect, pstDst, pstDstRect))
    {
        TDE_TRACE(TDE_KERN_INFO, "Surface overlap!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    return HI_SUCCESS;

}

/*****************************************************************************
* Function:      TdeOsiSingleSrcPatternFill
* Description:   single resource pattern fill
* Input:         s32Handle:task handle
                pstBackGround: background bitmap info
*                pstBackGroundRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstForeGroundRect: foreground bitmap operate zone
*                pstDst: target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
    HI_S32 TdeOsiSingleSrcPatternFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstSrc,
        TDE2_RECT_S *pstSrcRect, TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect,
        TDE2_PATTERN_FILL_OPT_S *pstOpt)
    {
        TDE_HWNode_S* pstHWNode = NULL;
        TDE_DRV_BASEOPT_MODE_E enBaseMode = {0};
        TDE_DRV_ALU_MODE_E enAluMode = {0};
        TDE_SCANDIRECTION_S stSrcScanInfo = {0};
        TDE_SCANDIRECTION_S stDstScanInfo = {0};
        TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
        TDE_DRV_SURFACE_S stDstDrvSurface = {0};
        HI_U16 u16Code = 0;
        TDE_DRV_CONV_MODE_CMD_S stConv = {0};
        TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;
        HI_BOOL bCheckSingleSrc2Rop = HI_TRUE;
        HI_BOOL bCheckBlend = HI_FALSE;
        HI_S32 s32Ret;

        s32Ret = TdeOsiCheckSingleSrcPatternFillPara(pstSrc, pstSrcRect, pstDst, pstDstRect, pstOpt);

        if (s32Ret != HI_SUCCESS)
        {
            return s32Ret;
        }

        if (TdeHalNodeInitNd(&pstHWNode)<0)
        {
            return HI_ERR_TDE_NO_MEM;
        }
        enBaseMode = TDE_SINGLE_SRC_PATTERN_FILL_OPT;
        enAluMode = TDE_ALU_NONE;

        if (pstOpt != HI_NULL)
        {
            if (pstOpt->enOutAlphaFrom >= TDE2_OUTALPHA_FROM_BUTT)
            {
                TDE_TRACE(TDE_KERN_INFO, "enOutAlphaFrom error!\n");
                TdeHalFreeNodeBuf(pstHWNode);
                return HI_ERR_TDE_INVALID_PARA;
            }

            if (pstOpt->enOutAlphaFrom == TDE2_OUTALPHA_FROM_BACKGROUND)
            {
                TDE_TRACE(TDE_KERN_INFO, "Single src pattern fill dosen't support out alpha form background!\n");
                TdeHalFreeNodeBuf(pstHWNode);
                return HI_ERR_TDE_INVALID_PARA;
            }

            s32Ret = TdeOsiSetRop( pstHWNode , pstOpt->enAluCmd, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha , &enAluMode ,bCheckSingleSrc2Rop);

            if (s32Ret)
            {
                return s32Ret;
            }

            s32Ret = TdeOsiSetBlend( pstHWNode ,  pstOpt->enAluCmd, pstOpt->stBlendOpt , &enAluMode ,bCheckBlend);

            if (s32Ret)
            {
                return s32Ret;
            }

            s32Ret = TdeOsiSetColorize( pstHWNode , pstOpt->enAluCmd, pstOpt->u32Colorize);

            if (s32Ret)
            {
                return s32Ret;
            }


            TdeHalNodeSetGlobalAlpha(pstHWNode, pstOpt->u8GlobalAlpha, pstOpt->stBlendOpt.bGlobalAlphaEnable);


            if (TdeOsiSetPatternClipPara(HI_NULL, HI_NULL, pstSrc, pstSrcRect, pstDst, pstDstRect, pstOpt, pstHWNode) < 0)
            {
                TdeHalFreeNodeBuf(pstHWNode);
                return HI_ERR_TDE_CLIP_AREA;
            }
        }

        if (TdeHalNodeSetBaseOperate(pstHWNode, enBaseMode, enAluMode, HI_NULL)<0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        TdeOsiSetExtAlpha(HI_NULL, pstSrc, pstHWNode);


        stSrcScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
        stSrcScanInfo.enVScan = TDE_SCAN_UP_DOWN;
        stDstScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
        stDstScanInfo.enVScan = TDE_SCAN_UP_DOWN;


        TdeOsiConvertSurface(pstSrc, pstSrcRect, &stSrcScanInfo, &stSrcDrvSurface, NULL);

    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstDst, pstDstRect, &stDstScanInfo, &stDstDrvSurface, NULL);


        (pstOpt != HI_NULL) ? TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, pstOpt->enOutAlphaFrom) :\
                              TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, TDE2_OUTALPHA_FROM_FOREGROUND);

        u16Code = TdeOsiSingleSrc2GetOptCode(pstSrc->enColorFmt, pstDst->enColorFmt);

        TdeOsiGetConvbyCode(u16Code, &stConv);

        if (TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        s32Ret = (pstOpt != HI_NULL) ? TdeOsiSetClutOpt(pstSrc, pstDst, &enClutUsage, pstOpt->bClutReload, pstHWNode) : \
                                       TdeOsiSetClutOpt(pstSrc, pstDst, &enClutUsage, HI_TRUE, pstHWNode);

        if (s32Ret != HI_SUCCESS)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }

        if (pstOpt != HI_NULL)
        {
            s32Ret = TdeHalNodeSetCsc(pstHWNode, pstOpt->stCscOpt);
            if (s32Ret < 0)
            {
                TdeHalFreeNodeBuf(pstHWNode);
                return s32Ret;
            }

        }
        if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }

        return HI_SUCCESS;
    }

static HI_S32 TdeOsiCheckDoubleSrcPatternFillPara( TDE2_SURFACE_S* pstBackGround,
        TDE2_RECT_S* pstBackGroundRect, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S* pstForeGroundRect,
        TDE2_SURFACE_S* pstDst, TDE2_RECT_S* pstDstRect, TDE2_PATTERN_FILL_OPT_S* pstOpt)
{
    HI_BOOL bContainNullPtr = ((HI_NULL == pstBackGround) || (HI_NULL == pstBackGroundRect)\
                               || (HI_NULL == pstForeGround) || (HI_NULL == pstForeGroundRect)\
                               || (HI_NULL == pstDst) || (HI_NULL == pstDstRect)\
                               || (HI_NULL == pstOpt));

    if (bContainNullPtr)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    if (TdeOsiCheckDoubleSrcPatternOpt(pstBackGround->enColorFmt, pstForeGround->enColorFmt,
                                       pstDst->enColorFmt, pstOpt) < 0)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }


    if (TdeOsiCheckOverlap(pstForeGround, pstForeGroundRect, pstBackGround, pstBackGroundRect)\
        || TdeOsiCheckOverlap(pstForeGround, pstForeGroundRect, pstDst, pstDstRect))
    {
        TDE_TRACE(TDE_KERN_INFO, "Surface overlap!\n");
        return HI_ERR_TDE_INVALID_PARA;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      TdeOsiDoubleSrcPatternFill
* Description:   dual resource pattern fill
* Input:         s32Handle: task handle
*                pstBackGround: background bitmap info
*                pstBackGroundRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstForeGroundRect: foreground bitmap operate zone
*                pstDst: target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
    HI_S32 TdeOsiDoubleSrcPatternFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstBackGround,
        TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround, TDE2_RECT_S *pstForeGroundRect,
        TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect, TDE2_PATTERN_FILL_OPT_S *pstOpt)
    {
        TDE_HWNode_S* pstHWNode = NULL;
        TDE_DRV_SURFACE_S stSrcDrvSurface = {0};
        TDE_DRV_SURFACE_S stDstDrvSurface = {0};
        TDE_SCANDIRECTION_S stSrcScanInfo = {0};
        TDE_SCANDIRECTION_S stDstScanInfo = {0};
        TDE_DRV_ALU_MODE_E enAluMode = TDE_ALU_NONE;
        TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;
        HI_U16 u16Code = 0;
        TDE_DRV_CONV_MODE_CMD_S stConv = {0};
        HI_BOOL bCheckBlend = HI_TRUE;
        HI_BOOL bCheckSingleSrc2Rop = HI_FALSE;
        HI_S32 s32Ret;


        s32Ret = TdeOsiCheckDoubleSrcPatternFillPara(pstBackGround, pstBackGroundRect, pstForeGround, \
                 pstForeGroundRect, pstDst, pstDstRect, pstOpt);


        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }

        if (TdeHalNodeInitNd(&pstHWNode) < 0)
        {
            return HI_ERR_TDE_NO_MEM;
        }



        s32Ret = TdeOsiSetRop( pstHWNode , pstOpt->enAluCmd, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha , &enAluMode ,bCheckSingleSrc2Rop);

        if (s32Ret)
        {
            return s32Ret;
        }


        s32Ret = TdeOsiSetBlend( pstHWNode ,  pstOpt->enAluCmd, pstOpt->stBlendOpt , &enAluMode ,bCheckBlend);

        if (s32Ret)
        {
            return s32Ret;
        }



        s32Ret = TdeOsiSetColorize( pstHWNode , pstOpt->enAluCmd, pstOpt->u32Colorize);

        if (s32Ret)
        {
            return s32Ret;
        }



        TdeHalNodeSetGlobalAlpha(pstHWNode, pstOpt->u8GlobalAlpha, pstOpt->stBlendOpt.bGlobalAlphaEnable);

    if(TdeOsiSetPatternClipPara(pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect, pstDst, pstDstRect, pstOpt, pstHWNode) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_CLIP_AREA;
    }

    if(TdeHalNodeSetBaseOperate(pstHWNode, TDE_DOUBLE_SRC_PATTERN_FILL_OPT, enAluMode, HI_NULL)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    TdeOsiSetExtAlpha(pstBackGround, pstForeGround, pstHWNode);


    stSrcScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
    stSrcScanInfo.enVScan = TDE_SCAN_UP_DOWN;
    stDstScanInfo.enHScan = TDE_SCAN_LEFT_RIGHT;
    stDstScanInfo.enVScan = TDE_SCAN_UP_DOWN;


    TdeOsiConvertSurface(pstBackGround, pstBackGroundRect, &stSrcScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc1(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstForeGround, pstForeGroundRect, &stSrcScanInfo, &stSrcDrvSurface, NULL);


    TdeHalNodeSetSrc2(pstHWNode, &stSrcDrvSurface);


    TdeOsiConvertSurface(pstDst, pstDstRect, &stDstScanInfo, &stDstDrvSurface, NULL);


    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, pstOpt->enOutAlphaFrom);


    u16Code = TdeOsiDoubleSrcGetOptCode(pstBackGround->enColorFmt, pstForeGround->enColorFmt,
                                        pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);


    if(TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return HI_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if((s32Ret = TdeOsiSetClutOpt(pstForeGround, pstBackGround, &enClutUsage, pstOpt->bClutReload, pstHWNode)) < 0)
    {
       TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }



        TDE_CHECK_COLORKEYMODE(pstOpt->enColorKeyMode);

        s32Ret = TdeOsiSetColorKey( pstForeGround , pstBackGround , pstHWNode, pstOpt->unColorKeyValue , pstOpt->enColorKeyMode ,enClutUsage);

        if (s32Ret)
        {
            return s32Ret;
        }

        s32Ret = TdeHalNodeSetCsc(pstHWNode, pstOpt->stCscOpt);
        if (s32Ret < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }

        if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }

        return HI_SUCCESS;
    }

/*****************************************************************************
* Function:      TdeOsiPatternFill
* Description:   pattern fill
* Input:         s32Handle:task handle
                 pstBackGround: background bitmap info
*                pstBackGroundRect: background bitmap operate zone
*                pstForeGround: foreground bitmap info
*                pstForeGroundRect: foreground bitmap operate zone
*                pstDst: target bitmap info
*                pstDstRect: target bitmap operate zone
*                pstOpt: operate option
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiPatternFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S * pstBackGround,
    TDE2_RECT_S * pstBackGroundRect, TDE2_SURFACE_S * pstForeGround,
    TDE2_RECT_S * pstForeGroundRect, TDE2_SURFACE_S * pstDst,
    TDE2_RECT_S * pstDstRect, TDE2_PATTERN_FILL_OPT_S *pstOpt)
{
    TDE_PATTERN_OPERATION_CATEGORY_E enOptCategory;

    enOptCategory = TdeOsiGetPatternOptCategory(pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect, pstDst,
                                         pstDstRect, pstOpt);
    switch(enOptCategory)
    {
        case TDE_PATTERN_OPERATION_SINGLE_SRC:
        {
            if (HI_NULL != pstBackGround)
            {
                return TdeOsiSingleSrcPatternFill(s32Handle, pstBackGround, pstBackGroundRect, pstDst, pstDstRect, pstOpt);
            }
            else
            {
                return TdeOsiSingleSrcPatternFill(s32Handle, pstForeGround, pstForeGroundRect, pstDst, pstDstRect, pstOpt);
            }
        }
        case TDE_PATTERN_OPERATION_DOUBLE_SRC:
        {
            return TdeOsiDoubleSrcPatternFill(s32Handle, pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect,
                                   pstDst, pstDstRect, pstOpt);
        }
        default:
            return HI_ERR_TDE_INVALID_PARA;
    }
}

/*****************************************************************************
* Function:      TdeCalScaleRect
* Description:   update zoom rect information
* Input:         pstSrcRect:source bitmap operate zone
                 pstDstRect: target bitmap operate zone
*                pstRectInSrc: source bitmap scale zone
*                pstRectInDst: target bitmap info atfer calculating
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeCalScaleRect(const TDE2_RECT_S* pstSrcRect, const TDE2_RECT_S* pstDstRect,
                                TDE2_RECT_S* pstRectInSrc, TDE2_RECT_S* pstRectInDst)
{
    UpdateConfig reg;
    UpdateInfo info;

    reg.ori_in_height = pstSrcRect->u32Height;
    reg.ori_in_width = pstSrcRect->u32Width;
    reg.zme_out_height = pstDstRect->u32Height;
    reg.zme_out_width = pstDstRect->u32Width;

    reg.update_instart_w = pstRectInSrc->s32Xpos;
    reg.update_instart_h = pstRectInSrc->s32Ypos;
    reg.update_in_width = pstRectInSrc->u32Width;
    reg.update_in_height = pstRectInSrc->u32Height;

    TdeOsiGetHUpdateInfo(&reg, &info, HI_TRUE);
    TdeOsiGetVUpdateInfo(&reg, &info, HI_TRUE, HI_TRUE);

    pstRectInSrc->s32Xpos = info.zme_instart_w;
    pstRectInSrc->s32Ypos = info.zme_instart_h;
    pstRectInSrc->u32Width = info.zme_in_width;
    pstRectInSrc->u32Height = info.zme_in_height;

    pstRectInDst->s32Xpos = info.zme_outstart_w;
    pstRectInDst->s32Ypos = info.zme_outstart_h;
    pstRectInDst->u32Width = info.zme_out_width;
    pstRectInDst->u32Height = info.zme_out_height;

    return HI_SUCCESS;
}


HI_S32 TdeOsiEnableRegionDeflicker(HI_BOOL bRegionDeflicker)
{
    s_bRegionDeflicker = bRegionDeflicker;
    return HI_SUCCESS;
}

HI_S32 TdeOsiQuickCopyEx(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect, HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst)
{
   	return TdeOsiSingleSrc1Blit(s32Handle, pstSrc, pstSrcRect,pstDst, pstDstRect, bMMZForSrc,bMMZForDst);
}
#ifdef CONFIG_TDE_BLIT_EX
HI_S32 TdeOsiSingleBlitEx(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst)
{
    return TdeOsiSingleSrc2Blit(s32Handle, pstSrc, pstSrcRect,pstDst, pstDstRect,pstOpt,bMMZForSrc,bMMZForDst);
}
#endif
EXPORT_SYMBOL(TdeOsiPatternFill);
EXPORT_SYMBOL(TdeOsiEnableRegionDeflicker);
EXPORT_SYMBOL(TdeCalScaleRect);

#endif


#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

