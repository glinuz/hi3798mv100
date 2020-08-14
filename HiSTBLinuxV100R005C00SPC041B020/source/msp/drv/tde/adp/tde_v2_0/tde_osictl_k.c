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

#define TDE_MIN(a, b) \
    (((a) > (b)) ? (b) : (a))
        
#define TDE_CALC_MAX_SLICE_NUM(out_w) \
     ((out_w) /256 +((out_w%256)!=0)) /*AI7D02711*/

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
                                    + ((pstOpt)->stClipRect.u32Height - 1); \
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
        if (enFmt >= TDE2_COLOR_FMT_byte)\
        {\
            TDE_TRACE(TDE_KERN_INFO, "This operation doesn't support Semi-plannar!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
        }\
    }while(0)
#define TDE_CHECK_DST_FMT(enFmt) do\
    {\
        if ((enFmt >= TDE2_COLOR_FMT_CLUT1)&&(enFmt <= TDE2_COLOR_FMT_A8))\
        {\
            TDE_TRACE(TDE_KERN_INFO, "This operation doesn't Clut&&A1!\n");\
            return HI_ERR_TDE_INVALID_PARA;\
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


#define TDE_CHECK_ROTATERECTAREA(pstForeGroundRect, pstDstRect, enRotateAngle)\
    do{\
        HI_BOOL bWrongRotateRectArea = ((pstForeGroundRect->u32Height != pstDstRect->u32Width) || (pstForeGroundRect->u32Width != pstDstRect->u32Height));\
        if ((TDE_ROTATE_CLOCKWISE_180 != enRotateAngle) && bWrongRotateRectArea)\
        {\
            TDE_TRACE(TDE_KERN_ERR, "Rotate operation rect is wrong!\n");\
            return -1;\
        }\
        if ((TDE_ROTATE_CLOCKWISE_180 == enRotateAngle) && bWrongRotateRectArea)\
        {\
            TDE_TRACE(TDE_KERN_ERR, "Rotate 180 operation rect is wrong!\n");\
            return -1;\
        }\
    }while(0)
            
#define TDE_OSI_FOUR_BITS_SHIFT    4
#define TDE_OSI_THREE_BITS_SHIFT   3
#define TDE_OSI_TWO_BITS_SHIFT     2
#define TDE_OSI_ONE_BIT_SHIFT     1
#define TDE_OSI_ZERO_BIT_SHIFT    0
     
/****************************************************************************/
/*                         TDE osi ctl struct definition					*/
/****************************************************************************/

typedef struct hiTDE_SLICE_DATA_S
{
    HI_U32 des_width;
    HI_U32 des_hoffset_pix;
    HI_U32 des_h_scan_ord;  
    HI_U32 des_crop_en;
    HI_U32 des_crop_start_x;
    HI_U32 des_crop_end_x;

    HI_U32 xdpos;
    HI_U32 h_scan_ord;
    HI_U32 width;

    //change
    //HI_U32 hoffset_pix ;//??????????¨°???¡ä??¡Â

    
    HI_U32 hlmsc_en;//????¨¢¨¢?¨¨??¡¤?¨º1?¨¹?¡ê
    HI_U32 hchmsc_en;//????¨¦??¨¨??¡¤?¨º1?¨¹?¡ê
    HI_U32 hratio;//(u,4,20)
    HI_S32 hor_loffset;//????¨¢¨¢?¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê3?¨º??¨¤??
    HI_S32 hor_coffset;//????¨¦??¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê
    HI_U32 zme_ow ;
    HI_U32 zme_iw;
    HI_U32 hpzme_en;//?????¡è??¡¤?¨º1?¨¹
    HI_U32 hpzme_mode;//?????¡è??¡¤?3¨¦¦Ì??¡ê¨º?
    HI_U32 hpzme_width;//?¡è??¡¤?¨º?3?¦Ì?¨ª????¨ª?¨¨

    HI_U32 des_xst_pos_blk;                                                // 0   256  512  768
    HI_U32 des_xed_pos_blk;                                               //255  511  767  1023
    HI_U32 xed_pos_blk;
    HI_U32 xst_pos_cord;
    HI_U32 xed_pos_cord;
    HI_U32 xst_pos_cord_in;                                           //5
    HI_U32 xed_pos_cord_in;
    HI_U32 hor_loffset_cfg_int_comp;                                  //5
    HI_S32 hor_loffset_cfg_int;                                        //5
    HI_S32 xst_pos_cord_in_offset;
    HI_S32 xed_pos_cord_in_offset;
    HI_U32 xst_pos_cord_in_tap_rgb;                                    //0
    HI_U32 xed_pos_cord_in_tap_rgb;
    HI_U32 node_cfg_zme_iw_rgb;
    HI_U32 hor_loffset_cfg_fraction;
    HI_U32 hor_loffset_pix_fraction;                                   //
    HI_U32 hor_loffset_fraction;                                      //[19:0]
    HI_S32 hor_loffset_int;
    HI_S32 node_cfg_hor_loffset_rgb;
    HI_U32 xst_pos_cord_in_tap_luma;
    HI_U32 xed_pos_cord_in_tap_luma;
    HI_U32 xst_pos_cord_in_chroma;
    HI_U32 xed_pos_cord_in_chroma;
    HI_U32 hor_coffset_cfg_int_comp;                                   //5
    HI_S32 hor_coffset_cfg_int;                                        //5
    HI_S32 xst_pos_cord_in_offset_chroma;
    HI_S32 xed_pos_cord_in_offset_chroma;
    HI_U32 xst_pos_cord_in_tap_chroma;                                //0
    HI_U32 xed_pos_cord_in_tap_chroma;
    HI_U32 xst_pos_cord_in_tap_chroma_x2;
    HI_U32 xed_pos_cord_in_tap_chroma_x2;
    HI_U32 xst_pos_cord_in_tap_sp;
    HI_U32 xed_pos_cord_in_tap_sp;
    HI_U32 node_cfg_zme_iw_sp;
    HI_U32 hor_coffset_cfg_fraction;
    HI_U32 hor_coffset_pix_fraction;
    HI_U32 hor_coffset_fraction;
    HI_S32 hor_loffset_int_sp;
    HI_S32 hor_coffset_int_sp;
    HI_U32 hor_loffset_int_sp_complent;
    HI_U32 hor_coffset_int_sp_complent;
    HI_S32 node_cfg_hor_loffset_sp;
    HI_S32 node_cfg_hor_coffset_sp;
    HI_U32 xst_pos_cord_in_tap;
    HI_U32 xed_pos_cord_in_tap;
    //change
    //HI_U32 xst_pos_cord_in_tap_hso;
    
    HI_U32 hor_loffset_int_complement;
    HI_U32 node_num;

//change
    HI_U32 xst_pos_cord_in_tap_hpzme;
    HI_U32 xed_pos_cord_in_tap_hpzme;
    HI_U32 xst_pos_cord_in_tap_hpzme_hso;
    HI_U32 xed_pos_cord_in_tap_hpzme_hso;
    HI_U32 u32422v_pro;
    HI_U32 hor_loffset_int_beyond;
    HI_U32 hor_loffset_int_beyond_complent;


    HI_U32 u32SliceWidth;
    HI_U32 u32SliceWi;
    HI_S32 s32SliceCOfst;
    HI_S32 s32SliceLOfst;
    HI_U32 u32SliceHoffset;
    HI_U32 u32SliceWo;
    HI_U32 u32SliceWHpzme;
    HI_U32 u32SliceDstWidth;
    HI_U32 u32SliceDstHoffset;
    HI_U32 fmt;
}TDE_SLICE_DATA_S;

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
    HI_U32 u32SliceWidth;
    HI_U32 u32SliceWi;
    HI_S32 s32SliceCOfst;
    HI_S32 s32SliceLOfst;
    HI_U32 u32SliceHoffset;
    HI_U32 u32SliceWo;
    HI_U32 u32SliceWHpzme;
    HI_U32 u32SliceDstWidth;
    HI_U32 u32SliceDstHeight;
    HI_U32 u32SliceDstHoffset;
    TDE_SLICE_TYPE_E enSliceType;
    HI_U32 des_crop_en;
    HI_U32 des_crop_start_x;
    HI_U32 des_crop_end_x;
}TDE_SLICE_INFO_S;



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

typedef struct tagTDE_TDE_UPDATECONFIG_S
{
    HI_S32 ori_in_width; //original image width
    HI_S32 ori_in_height; //original image height 
    HI_S32 zme_out_width; //output full image width
    HI_S32 zme_out_height; //output full image height

    HI_S32 update_instart_w; //the start_x of update area in original image 
    HI_S32 update_instart_h; //the start_y of update area in original image
    HI_S32 update_in_width;  //the width of update area in original image
    HI_S32 update_in_height;  //the height of update area in original image 
}TDE_UPDATECONFIG_S;

typedef struct tagTDE_TDE_UPDATEINFO_S
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
}TDE_UPDATEINFO_S;

/****************************************************************************/
/*                     TDE osi ctl inner variables definition   			*/
/****************************************************************************/
TDE_DRV_COLOR_FMT_E g_enTdeCommonDrvColorFmt[TDE2_COLOR_FMT_BUTT + 1] = {
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

STATIC HI_S32                    TdeOsiSetFilterNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode,TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstDst,
                                  TDE2_RECT_S  *pstDstRect, TDE2_DEFLICKER_MODE_E enDeflickerMode,TDE2_FILTER_MODE_E enFliterMode);

STATIC HI_S32                    TdeOsi1SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstDst,
                                                   TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                                                   TDE2_OPT_S *pstOpt);
                                                   
STATIC HI_S32 TdeOsiSingleSrc2Blit(TDE_HANDLE s32Handle,TDE2_SURFACE_S* pstForeGround,TDE2_RECT_S  *pstForeGroundRect,TDE2_SURFACE_S* pstDst,
                                                                TDE2_RECT_S  *pstDstRect,TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst);                                                   
                                                   
#ifndef HI_BUILD_IN_BOOT 
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

STATIC HI_S32                   TdeOsiSetClipPara(TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S *pstBGRect,
                                                   TDE2_SURFACE_S * pstForeGround,
                                                   TDE2_RECT_S *pstFGRect, TDE2_SURFACE_S * pstDst,
                                                   TDE2_RECT_S *pstDstRect,
                                                   TDE2_OPT_S * pstOpt,
                                                   TDE_HWNode_S *pstHwNode);

STATIC HI_S32                   TdeOsiSetBaseOptParaForBlit(TDE2_OPT_S * pstOpt, TDE2_SURFACE_S* pstSrc1, TDE2_SURFACE_S* pstSrc2,
                                                             TDE_OPERATION_CATEGORY_E enOptCategory, TDE_HWNode_S *pstHwNode);

STATIC        HI_S32             TdeOsiAdjClipPara(TDE_HWNode_S* pHWNode);

STATIC HI_S32                    TdeOsiSetNodeFinish(TDE_HANDLE s32Handle, TDE_HWNode_S* pHWNode,
                                                     HI_U32 u32WorkBufNum, TDE_NODE_SUBM_TYPE_E enSubmType);

STATIC INLINE HI_S32             TdeOsiCheckResizePara(HI_U32 u32InWidth, HI_U32 u32InHeight,
                                                       HI_U32 u32OutWidth, HI_U32 u32OutHeight);
STATIC INLINE HI_BOOL            TdeOsiWhetherContainAlpha(TDE2_COLOR_FMT_E enColorFmt);
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


TDE2_COLOR_FMT_E TdeOsiCovertMbFmt(TDE2_MB_COLOR_FMT_E enFmt)
{
    switch (enFmt)
    {
        case TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP:
            return TDE2_COLOR_FMT_JPG_YCbCr400MBP;
        case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP:
            return TDE2_COLOR_FMT_JPG_YCbCr422MBHP;
        case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP:
            return TDE2_COLOR_FMT_JPG_YCbCr422MBVP;
       case TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP:
            return TDE2_COLOR_FMT_MP1_YCbCr420MBP;     
       case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP:
            return TDE2_COLOR_FMT_MP2_YCbCr420MBP;  
       case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI:
            return TDE2_COLOR_FMT_MP2_YCbCr420MBI; 
       case TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP:
            return TDE2_COLOR_FMT_JPG_YCbCr420MBP; 
       case TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP:
            return TDE2_COLOR_FMT_JPG_YCbCr444MBP; 
       default:
	        return TDE2_COLOR_FMT_BUTT;
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


            stClutCmd.u32PhyClutAddr = (HI_U32)(unsigned long)pClutSur->pu8ClutPhyAddr;
            if(TdeHalNodeSetClutOpt(pstHWNode, &stClutCmd, bClutReload)<0)
            {
                return HI_ERR_TDE_UNSUPPORTED_OPERATION;
            }
        }
    }
    return HI_SUCCESS;
}



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
    TDE_CHECK_DST_FMT(enDstFmt);
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

    bUnknownFmt = ((TDE_COLORFMT_CATEGORY_BYTE <= enSrc1Category)\
                           || (TDE_COLORFMT_CATEGORY_BYTE <= enSrc2Category)\
                           || (TDE_COLORFMT_CATEGORY_BYTE <= enDstCategory));

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
        enMirror = pstOpt->enMirror;
    }
#if 0
    if (enMirror && bFilter)
    {
        TDE_TRACE(TDE_KERN_INFO, "Could not support VF/FF/HF and Mirror\n!");
        return -1;
    }

    if (bFilter) /* if use filter, use default scanning direction */
    {
        return 0;
    }
#endif
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
STATIC HI_S32 TdeOsiGetInterRect(TDE2_RECT_S *pDstRect, TDE2_RECT_S *pClipRect, TDE2_RECT_S *pInterRect)
{
    HI_S32 s32Left, s32Top, s32Right, s32Bottom;
    HI_S32 s32Right1, s32Bottom1, s32Right2, s32Bottom2;
    HI_S32 s32Xpos,s32Ypos;
    s32Xpos = pDstRect->s32Xpos;
    s32Ypos = pDstRect->s32Ypos;
    s32Left = (s32Xpos > pClipRect->s32Xpos) ? s32Xpos : pClipRect->s32Xpos;
    s32Top = (s32Ypos > pClipRect->s32Ypos) ? s32Ypos : pClipRect->s32Ypos;

    s32Right1 = s32Xpos + pDstRect->u32Width - 1;  /* AI7D02635 */
    s32Right2 = pClipRect->s32Xpos + pClipRect->u32Width - 1;  /* AI7D02635 */
    s32Right = (s32Right1 > s32Right2) ? s32Right2 : s32Right1;

    s32Bottom1 = s32Ypos + pDstRect->u32Height - 1; /* AI7D02635 */
    s32Bottom2 = pClipRect->s32Ypos + pClipRect->u32Height - 1; /* AI7D02635 */
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
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_RECT_S stForeGroundRect = {0};
    TDE2_COLOR_FMT_E enColorFmt;
    TDE2_OPT_S stOpt = {0};
    stForeGround.u32PhyAddr = pstMB->u32YPhyAddr;
    stForeGround.u32Stride = pstMB->u32YStride;
    stForeGround.u32Width = pstMB->u32YWidth;
    enColorFmt = TdeOsiCovertMbFmt(pstMB->enMbFmt);
    stForeGround.enColorFmt = enColorFmt;
    stForeGround.u32Height = pstMB->u32YHeight;
    stForeGround.u32CbCrPhyAddr = pstMB->u32CbCrPhyAddr;
    stForeGround.u32CbCrStride = pstMB->u32CbCrStride;

   

    HI_GFX_Memcpy(&stForeGroundRect,pstMbRect,sizeof(TDE2_RECT_S));

    stOpt.enDeflickerMode = (pMbOpt->bDeflicker)?TDE2_DEFLICKER_MODE_BOTH:TDE2_DEFLICKER_MODE_NONE;  
    stOpt.enOutAlphaFrom = (pMbOpt->bSetOutAlpha)?TDE2_OUTALPHA_FROM_GLOBALALPHA:TDE2_OUTALPHA_FROM_NORM;
    stOpt.bResize = (TDE2_MBRESIZE_NONE == pMbOpt->enResize)?HI_FALSE:HI_TRUE;
    stOpt.stClipRect = pMbOpt->stClipRect;
    stOpt.u8GlobalAlpha = pMbOpt->u8OutAlpha;
    stOpt.enClipMode = pMbOpt->enClipMode;
    TdeOsiSingleSrc2Blit(s32Handle,&stForeGround,&stForeGroundRect,pstDst, pstDstRect, &stOpt,HI_FALSE,HI_FALSE);
    return HI_SUCCESS;
}

#define MAX2(a, b)        (((a) < (b)) ?  (b) : (a))
#define MIN2(a, b)        (((a) < (b)) ?  (a) : (b))

STATIC HI_U32 TdeTrueValueToComplement(HI_S32 data_in, HI_U32 bit)//data_in :positive or negetive  ;  bit:complement width
{
    HI_U32 data_out ;
    HI_U32 data_in_tmp ;
    if(data_in >> (bit-1))
    {
        data_in_tmp = 0 - data_in ;
        data_out = ((1<<(bit-1)) | (((~data_in_tmp)&((1<<(bit-1))-1))+1)) & ((1<<bit)-1) ;
    }
    else
    {
        data_out = data_in ;
    }
     
    return data_out ;
}

STATIC HI_S32 TdeComplementToTrueValue(HI_U32 data_in, HI_U32 bit)//data_in :positive or negetive  ;  bit:complement width
{
    HI_S32 data_out ;
    HI_U32 data_in_tmp ;
    if(data_in >> (bit-1))
    {
        data_in_tmp = data_in&((1<<(bit-1))-1);
        data_out = (((~data_in_tmp) & ((1<<(bit-1))-1))+1)*(-1) ;
    }
    else
    {
        data_out = data_in ;
    }
     
    return data_out ;
}


STATIC HI_VOID TdeOsiInitSliceData(TDE_SLICE_DATA_S* stSliceData , TDE_HWNode_S* pChildNode, HI_U32 i)
{
    if (0 == i)
    {
        stSliceData->fmt = pChildNode->SRC1_CTRL.bits.src1_fmt;
        stSliceData->h_scan_ord = pChildNode->SRC1_CTRL.bits.src1_h_scan_ord;//pstFilterOpt->stSrcDire.enHScan;
        stSliceData->width = pChildNode->SRC1_IMGSIZE.bits.src1_width + 1;
        
        stSliceData->u32422v_pro = pChildNode->SRC1_CTRL.bits.src1_422v_pro;
        
        stSliceData->hlmsc_en = pChildNode->SRC1_HSP.bits.hlmsc_en;//????¨¢¨¢?¨¨??¡¤?¨º1?¨¹?¡ê
        stSliceData->hchmsc_en = pChildNode->SRC1_HSP.bits.hchmsc_en;//????¨¦??¨¨??¡¤?¨º1?¨¹?¡ê
        stSliceData->hratio = pChildNode->SRC1_HSP.bits.hratio;//(u,4,20)
        stSliceData->hor_loffset = pChildNode->SRC1_HLOFFSET;//????¨¢¨¢?¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê3?¨º??¨¤??
        stSliceData->hor_coffset = pChildNode->SRC1_HCOFFSET;//????¨¦??¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê
        stSliceData->zme_ow = pChildNode->SRC1_ZMEORESO.bits.ow + 1;
        stSliceData->zme_iw = pChildNode->SRC1_ZMEIRESO.bits.iw + 1;
        stSliceData->hpzme_en = pChildNode->SRC1_HPZME.bits.src1_hpzme_en;//?????¡è??¡¤?¨º1?¨¹
        stSliceData->hpzme_mode = pChildNode->SRC1_HPZME.bits.src1_hpzme_mode;//?????¡è??¡¤?3¨¦¦Ì??¡ê¨º?
        stSliceData->hpzme_width = pChildNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width + 1; //?¡è??¡¤?¨º?3?¦Ì?¨ª????¨ª?¨¨
    }
    else
    {
        stSliceData->fmt = pChildNode->SRC2_CTRL.bits.src2_fmt;
        stSliceData->h_scan_ord = pChildNode->SRC2_CTRL.bits.src2_h_scan_ord;//pstFilterOpt->stSrcDire.enHScan;
        stSliceData->width = pChildNode->SRC2_IMGSIZE.bits.src2_width + 1;
        
        stSliceData->u32422v_pro = pChildNode->SRC2_CTRL.bits.src2_422v_pro;
        
        stSliceData->hlmsc_en = pChildNode->SRC2_HSP.bits.hlmsc_en;//????¨¢¨¢?¨¨??¡¤?¨º1?¨¹?¡ê
        stSliceData->hchmsc_en = pChildNode->SRC2_HSP.bits.hchmsc_en;//????¨¦??¨¨??¡¤?¨º1?¨¹?¡ê
        stSliceData->hratio = pChildNode->SRC2_HSP.bits.hratio;//(u,4,20)
        stSliceData->hor_loffset = pChildNode->SRC2_HLOFFSET;//????¨¢¨¢?¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê3?¨º??¨¤??
        stSliceData->hor_coffset = pChildNode->SRC2_HCOFFSET;//????¨¦??¨¨??????¨°?¡ê?(s,8,20)??¨º??¡ê21??¡À¨ª¨º??¡ê
        stSliceData->zme_ow = pChildNode->SRC2_ZMEORESO.bits.ow + 1;
        stSliceData->zme_iw = pChildNode->SRC2_ZMEIRESO.bits.iw + 1;
        stSliceData->hpzme_en = pChildNode->SRC2_HPZME.bits.src2_hpzme_en;//?????¡è??¡¤?¨º1?¨¹
        stSliceData->hpzme_mode = pChildNode->SRC2_HPZME.bits.src2_hpzme_mode;//?????¡è??¡¤?3¨¦¦Ì??¡ê¨º?
        stSliceData->hpzme_width = pChildNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width + 1;//?¡è??¡¤?¨º?3?¦Ì?¨ª????¨ª?¨¨
    }

    return;
}
STATIC HI_VOID TdeOsiCalcSliceS1(TDE_SLICE_DATA_S *stSliceData)
{
    //...................................s1........................................................................................

    stSliceData->xdpos = (stSliceData->hlmsc_en | stSliceData->hchmsc_en) ? (stSliceData->zme_ow - 1) : (stSliceData->hpzme_en ? (stSliceData->hpzme_width - 1) : (stSliceData->width - 1));
    stSliceData->xed_pos_blk = MIN2(stSliceData->des_xed_pos_blk, stSliceData->xdpos);
    /*step2 : out pos relative to s1 disp start */
    stSliceData->xst_pos_cord = stSliceData->des_xst_pos_blk ;
    stSliceData->xed_pos_cord = stSliceData->xed_pos_blk;
    /*setp2 : calc s1 zme parameter */
    stSliceData->xst_pos_cord_in = (stSliceData->xst_pos_cord * stSliceData->hratio) >> 20;
    stSliceData->xed_pos_cord_in = (stSliceData->xed_pos_cord * stSliceData->hratio) >> 20;
    stSliceData->hor_loffset_cfg_int_comp  = stSliceData->hor_loffset >> 20;
    stSliceData->hor_loffset_cfg_int = TdeComplementToTrueValue(stSliceData->hor_loffset_cfg_int_comp, 8);
    stSliceData->hor_loffset_cfg_fraction = stSliceData->hor_loffset & 0xfffff;
    stSliceData->hor_loffset_pix_fraction = (stSliceData->xst_pos_cord * stSliceData->hratio) & 0xfffff;
    stSliceData->hor_loffset_fraction = (stSliceData->hor_loffset_cfg_fraction +  stSliceData->hor_loffset_pix_fraction) & 0xfffff;
    stSliceData->xst_pos_cord_in_offset = stSliceData->xst_pos_cord_in +  stSliceData->hor_loffset_cfg_int + (((stSliceData->hor_loffset_cfg_fraction + stSliceData->hor_loffset_pix_fraction) & 0xfff00000) != 0);
    stSliceData->xed_pos_cord_in_offset = stSliceData->xed_pos_cord_in +  stSliceData->hor_loffset_cfg_int + (((stSliceData->hor_loffset_cfg_fraction + stSliceData->hor_loffset_pix_fraction) & 0xfff00000) != 0);
    stSliceData->xst_pos_cord_in_tap_rgb = stSliceData->xst_pos_cord_in_offset < 0 ? 0 : ((stSliceData->xst_pos_cord_in_offset >= (TDE_MAX_ZOOM_OUT_STEP / 2 - 1))  ? (stSliceData->xst_pos_cord_in_offset - (TDE_MAX_ZOOM_OUT_STEP / 2 - 1))  : 0);
    stSliceData->xed_pos_cord_in_tap_rgb = (stSliceData->xed_pos_cord_in_offset + TDE_MAX_ZOOM_OUT_STEP / 2) < 0 ? 0 : (((stSliceData->xed_pos_cord_in_offset + TDE_MAX_ZOOM_OUT_STEP / 2) >= stSliceData->zme_iw - 1) ? stSliceData->zme_iw - 1  : (stSliceData->xed_pos_cord_in_offset + TDE_MAX_ZOOM_OUT_STEP / 2));
    stSliceData->node_cfg_zme_iw_rgb =  stSliceData->xed_pos_cord_in_tap_rgb - stSliceData->xst_pos_cord_in_tap_rgb + 1;
    stSliceData->hor_loffset_int = (stSliceData->xst_pos_cord_in_offset - stSliceData->xst_pos_cord_in_tap_rgb);
    stSliceData->hor_loffset_int_complement = TdeTrueValueToComplement(stSliceData->hor_loffset_int, 8);
    stSliceData->node_cfg_hor_loffset_rgb = (stSliceData->hor_loffset_int_complement << 20) + stSliceData->hor_loffset_fraction;

    return;
}

STATIC HI_VOID TdeOsiCalcSliceSp(TDE_SLICE_DATA_S* stSliceData)
{

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  SP  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    stSliceData->xst_pos_cord_in_tap_luma = ((stSliceData->xst_pos_cord_in_tap_rgb % 2) == 1) ?  stSliceData->xst_pos_cord_in_tap_rgb - 1 : stSliceData->xst_pos_cord_in_tap_rgb;

    stSliceData->xed_pos_cord_in_tap_luma =  (((stSliceData->xed_pos_cord_in_tap_rgb%2) == 0) ?  stSliceData->xed_pos_cord_in_tap_rgb + 1 : stSliceData->xed_pos_cord_in_tap_rgb) > (stSliceData->zme_iw -1) ?  (stSliceData->zme_iw -1) :  (((stSliceData->xed_pos_cord_in_tap_rgb%2) == 0) ?  stSliceData->xed_pos_cord_in_tap_rgb + 1 : stSliceData->xed_pos_cord_in_tap_rgb);


    stSliceData->xst_pos_cord_in_chroma = (stSliceData->fmt ==50) ? (stSliceData->xst_pos_cord * (stSliceData->hratio))  >> 20  : (stSliceData->xst_pos_cord * (stSliceData->hratio/2))  >> 20;

    
    stSliceData->xed_pos_cord_in_chroma = (stSliceData->fmt ==50) ? (stSliceData->xed_pos_cord * (stSliceData->hratio))  >> 20  : (stSliceData->xed_pos_cord * (stSliceData->hratio/2))  >> 20;

    stSliceData->hor_coffset_cfg_int_comp = stSliceData->hor_coffset >> 20;
    stSliceData->hor_coffset_cfg_int = TdeComplementToTrueValue(stSliceData->hor_coffset_cfg_int_comp, 8);
    stSliceData->hor_coffset_cfg_fraction = stSliceData->hor_coffset & 0xfffff;

    stSliceData->hor_coffset_pix_fraction = ((stSliceData->fmt ==50) ? (stSliceData->xst_pos_cord * (stSliceData->hratio)) : (stSliceData->xst_pos_cord * (stSliceData->hratio/2))) & 0xfffff;


    stSliceData->hor_coffset_fraction = (stSliceData->hor_coffset_cfg_fraction +  stSliceData->hor_coffset_pix_fraction) & 0xfffff;
    stSliceData->xst_pos_cord_in_offset_chroma   = stSliceData->xst_pos_cord_in_chroma +  stSliceData->hor_coffset_cfg_int + (((stSliceData->hor_coffset_cfg_fraction +  stSliceData->hor_coffset_pix_fraction) & 0xfff00000) != 0);
    stSliceData->xed_pos_cord_in_offset_chroma = stSliceData->xed_pos_cord_in_chroma +  stSliceData->hor_coffset_cfg_int + (((stSliceData->hor_coffset_cfg_fraction +  stSliceData->hor_coffset_pix_fraction) & 0xfff00000) != 0);
    stSliceData->xst_pos_cord_in_tap_chroma = stSliceData->xst_pos_cord_in_offset_chroma < 0 ? 0 : ((stSliceData->xst_pos_cord_in_offset_chroma >= (TDE_MAX_ZOOM_OUT_STEP / 2 - 1))  ? (stSliceData->xst_pos_cord_in_offset_chroma - (TDE_MAX_ZOOM_OUT_STEP / 2 - 1))  : 0); //0

    stSliceData->xed_pos_cord_in_tap_chroma = (stSliceData->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP/2) <0 ? 0 : (((stSliceData->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP/2) >= ((stSliceData->fmt ==50) ? ((stSliceData->zme_iw -1)) : ((stSliceData->zme_iw -1)/2))) ? ((stSliceData->fmt ==50) ? ((stSliceData->zme_iw -1)) : ((stSliceData->zme_iw -1)/2))  : (stSliceData->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP/2)) ;


    stSliceData->xst_pos_cord_in_tap_chroma_x2 = (stSliceData->fmt ==50) ? stSliceData->xst_pos_cord_in_tap_chroma : stSliceData->xst_pos_cord_in_tap_chroma *2;


    stSliceData->xed_pos_cord_in_tap_chroma_x2 = (stSliceData->fmt ==50) ? stSliceData->xed_pos_cord_in_tap_chroma : stSliceData->xed_pos_cord_in_tap_chroma *2 +1;


    stSliceData->xst_pos_cord_in_tap_sp = MIN2(stSliceData->xst_pos_cord_in_tap_luma, stSliceData->xst_pos_cord_in_tap_chroma_x2);
    stSliceData->xed_pos_cord_in_tap_sp = MAX2(stSliceData->xed_pos_cord_in_tap_luma, stSliceData->xed_pos_cord_in_tap_chroma_x2);
    stSliceData->node_cfg_zme_iw_sp = stSliceData->xed_pos_cord_in_tap_sp - stSliceData->xst_pos_cord_in_tap_sp + 1;
    stSliceData->hor_loffset_int_sp = (stSliceData->xst_pos_cord_in_offset - stSliceData->xst_pos_cord_in_tap_sp);

    stSliceData->hor_coffset_int_sp = (stSliceData->xst_pos_cord_in_offset_chroma - ((stSliceData->fmt ==50) ? stSliceData->xst_pos_cord_in_tap_sp : stSliceData->xst_pos_cord_in_tap_sp/2));


    stSliceData->hor_loffset_int_sp_complent = TdeTrueValueToComplement(stSliceData->hor_loffset_int_sp, 8);
    stSliceData->hor_coffset_int_sp_complent = TdeTrueValueToComplement(stSliceData->hor_coffset_int_sp, 8);

    stSliceData->node_cfg_hor_loffset_sp = (stSliceData->hor_loffset_int_sp_complent << 20) + stSliceData->hor_loffset_fraction;
    stSliceData->node_cfg_hor_coffset_sp = (stSliceData->hor_coffset_int_sp_complent << 20) + stSliceData->hor_coffset_fraction;


    return;
}
STATIC HI_VOID TdeOsiCalcSliceMux(TDE_SLICE_DATA_S* stSliceData , TDE_HWNode_S* pChildNode, HI_U32 i)
{
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MUX  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    HI_BOOL bCfgZmeIwSp;
    HI_BOOL bCfgHorLoffsetSp;
    HI_BOOL bCfgHorCoffsetSp;
    HI_BOOL bSetSliceSize;
    
    stSliceData->u32SliceWo = stSliceData->xed_pos_cord - stSliceData->xst_pos_cord + 1;

    bCfgZmeIwSp = ((stSliceData->fmt >= 32) && (stSliceData->fmt !=48) && (!((stSliceData->fmt ==50) && !stSliceData->u32422v_pro)) && (stSliceData->fmt !=52));
    stSliceData->u32SliceWi= (bCfgZmeIwSp ) ? stSliceData->node_cfg_zme_iw_sp:((stSliceData->hlmsc_en || stSliceData->hchmsc_en) ? stSliceData->node_cfg_zme_iw_rgb : stSliceData->u32SliceWo);




    bCfgHorLoffsetSp =((stSliceData->fmt >= 32) && (stSliceData->fmt !=48) && (!((stSliceData->fmt ==50) && !stSliceData->u32422v_pro)) && (stSliceData->fmt !=52));
    stSliceData->s32SliceLOfst = (bCfgHorLoffsetSp) ? stSliceData->node_cfg_hor_loffset_sp :   stSliceData->node_cfg_hor_loffset_rgb;                                                 


    bCfgHorCoffsetSp = bCfgHorLoffsetSp;
    stSliceData->s32SliceCOfst = (bCfgHorCoffsetSp )? stSliceData->node_cfg_hor_coffset_sp :  stSliceData->node_cfg_hor_loffset_rgb;                                     


    stSliceData->u32SliceWHpzme = stSliceData->u32SliceWi;
    stSliceData->u32SliceWo = stSliceData->u32SliceWo - 1;
    stSliceData->u32SliceWi = stSliceData->u32SliceWi - 1;

    stSliceData->xst_pos_cord_in_tap = (stSliceData->hlmsc_en || stSliceData->hchmsc_en) ? (((stSliceData->fmt >= 32) && (stSliceData->fmt !=48) && (!((stSliceData->fmt ==50) && !stSliceData->u32422v_pro)) && (stSliceData->fmt !=52)) ? stSliceData->xst_pos_cord_in_tap_sp:stSliceData->xst_pos_cord_in_tap_rgb): stSliceData->xst_pos_cord;


    stSliceData->xed_pos_cord_in_tap = (stSliceData->hlmsc_en || stSliceData->hchmsc_en) ? (((stSliceData->fmt >= 32) && (stSliceData->fmt !=48) && (!((stSliceData->fmt ==50) && !stSliceData->u32422v_pro)) && (stSliceData->fmt !=52)) ? stSliceData->xed_pos_cord_in_tap_sp:stSliceData->xed_pos_cord_in_tap_rgb): stSliceData->xed_pos_cord;
    stSliceData->xst_pos_cord_in_tap_hpzme = stSliceData->hpzme_en ? (stSliceData->xst_pos_cord_in_tap * (stSliceData->hpzme_mode +1)) : stSliceData->xst_pos_cord_in_tap;
    stSliceData->xed_pos_cord_in_tap_hpzme = stSliceData->hpzme_en ? ((stSliceData->xed_pos_cord_in_tap+1) * (stSliceData->hpzme_mode +1)-1) : stSliceData->xed_pos_cord_in_tap;


    if (stSliceData->h_scan_ord)
    {
        
        stSliceData->xst_pos_cord_in_tap_hpzme_hso = stSliceData->width - 1 - stSliceData->xed_pos_cord_in_tap_hpzme;
        stSliceData->xed_pos_cord_in_tap_hpzme_hso = stSliceData->width - 1 - stSliceData->xst_pos_cord_in_tap_hpzme;              
    }
    else
    {
    
        stSliceData->xst_pos_cord_in_tap_hpzme_hso = stSliceData->xst_pos_cord_in_tap_hpzme;
        stSliceData->xed_pos_cord_in_tap_hpzme_hso = stSliceData->xed_pos_cord_in_tap_hpzme;     
    }

    stSliceData->u32SliceWidth = (stSliceData->xed_pos_cord_in_tap_hpzme_hso - stSliceData->xst_pos_cord_in_tap_hpzme_hso + 1);
    stSliceData->u32SliceHoffset = stSliceData->xst_pos_cord_in_tap_hpzme_hso;
    
    stSliceData->hor_loffset_int_beyond              = 1;
    stSliceData->hor_loffset_int_beyond_complent     = TdeTrueValueToComplement(stSliceData->hor_loffset_int_beyond,8);

    bSetSliceSize = ((stSliceData->hpzme_en == 0) && (stSliceData->u32SliceWidth % 2 == 1) && ((stSliceData->fmt == 48) || ((stSliceData->fmt == 50) && !stSliceData->u32422v_pro) || (stSliceData->fmt == 52)) && (stSliceData->xed_pos_cord_in_tap_hpzme_hso == stSliceData->width - 1) && (stSliceData->width % 2 == 0));
    if (bSetSliceSize)
    {

        if (stSliceData->h_scan_ord)
        {
            stSliceData->u32SliceWi         = stSliceData->u32SliceWi + 1     ;
            stSliceData->u32SliceWidth      = stSliceData->u32SliceWidth  + 1     ;
            stSliceData->u32SliceHoffset    = stSliceData->u32SliceHoffset -1 ;

        }
        else
        {
            stSliceData->u32SliceWi        = stSliceData->u32SliceWi + 1     ;
            stSliceData->u32SliceWidth     = stSliceData->u32SliceWidth  + 1     ;
            stSliceData->u32SliceHoffset   = stSliceData->u32SliceHoffset -1 ;
            stSliceData->s32SliceLOfst     = stSliceData->s32SliceLOfst + (stSliceData->hor_loffset_int_beyond_complent << 20) ;                                  
            stSliceData->s32SliceCOfst     = stSliceData->s32SliceCOfst + (stSliceData->hor_loffset_int_beyond_complent << 20) ;                                  

        }
    }

    if (0 == i)
    {
        pChildNode->SRC1_ZMEORESO.bits.ow = stSliceData->u32SliceWo;
        pChildNode->SRC1_HLOFFSET = stSliceData->s32SliceLOfst;
        pChildNode->SRC1_HCOFFSET = stSliceData->s32SliceCOfst;
        pChildNode->SRC1_PIX_OFFSET.bits.src1_hoffset_pix = stSliceData->u32SliceHoffset;
        pChildNode->SRC1_IMGSIZE.bits.src1_width = stSliceData->u32SliceWidth - 1;
        pChildNode->SRC1_ZMEIRESO.bits.iw = stSliceData->u32SliceWi;
        pChildNode->SRC1_HPZME_SIZE.bits.src1_hpzme_width = stSliceData->u32SliceWHpzme - 1;

        return;
    }

    pChildNode->SRC2_ZMEORESO.bits.ow = stSliceData->u32SliceWo;
    pChildNode->SRC2_HLOFFSET = stSliceData->s32SliceLOfst;
    pChildNode->SRC2_HCOFFSET = stSliceData->s32SliceCOfst;
    pChildNode->SRC2_PIX_OFFSET.bits.src2_hoffset_pix = stSliceData->u32SliceHoffset;
    pChildNode->SRC2_IMGSIZE.bits.src2_width = stSliceData->u32SliceWidth - 1;
    pChildNode->SRC2_ZMEIRESO.bits.iw = stSliceData->u32SliceWi;
    pChildNode->SRC2_HPZME_SIZE.bits.src2_hpzme_width = stSliceData->u32SliceWHpzme - 1;

return;
}
STATIC HI_VOID TdeOsiFinalizeSliceCalculation(TDE_SLICE_DATA_S* stSliceData , TDE_HWNode_S* pChildNode)
{
    stSliceData->u32SliceDstWidth = stSliceData->des_xed_pos_blk - stSliceData->des_xst_pos_blk + 1;
    stSliceData->u32SliceDstHoffset = (stSliceData->des_h_scan_ord ? (stSliceData->des_width - 1 - stSliceData->des_xed_pos_blk ) : stSliceData->des_xst_pos_blk) + stSliceData->des_hoffset_pix;
    stSliceData->des_crop_en = (!((stSliceData->des_crop_start_x >  stSliceData->des_xed_pos_blk) ||  (stSliceData->des_crop_end_x < stSliceData->des_xst_pos_blk))) && stSliceData->des_crop_en;
    stSliceData->des_crop_start_x = stSliceData->des_crop_en ?  (MAX2(stSliceData->des_xst_pos_blk, stSliceData->des_crop_start_x) - stSliceData->des_xst_pos_blk) : 0;
    stSliceData->des_crop_end_x = stSliceData->des_crop_en ?  (MIN2(stSliceData->des_xed_pos_blk, stSliceData->des_crop_end_x) -  stSliceData->des_xst_pos_blk)  : 0;

    pChildNode->DES_IMGSIZE.bits.des_width = stSliceData->u32SliceDstWidth - 1;
    pChildNode->DES_PIX_OFFSET.bits.des_hoffset_pix = stSliceData->u32SliceDstHoffset;
    pChildNode->DES_ALPHA.bits.des_crop_en = stSliceData->des_crop_en;
    pChildNode->DES_CROP_POS_ST.bits.des_crop_start_x = stSliceData->des_crop_start_x;
    pChildNode->DES_CROP_POS_ED.bits.des_crop_end_x =  stSliceData->des_crop_end_x;

    return;
}


HI_S32 TdeOsiCalcSlice(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode)
{
    TDE_SLICE_DATA_S stSliceData;
    HI_U32 n = 0;
    HI_U32 i = 0;
    HI_S32 s32Ret;

    stSliceData.des_width = pNode->DES_IMGSIZE.bits.des_width + 1;
    stSliceData.des_hoffset_pix  = 0;
    stSliceData.des_h_scan_ord = pNode->DES_CTRL.bits.des_h_scan_ord;

    stSliceData.des_crop_en = pNode->DES_ALPHA.bits.des_crop_en;
    stSliceData.des_crop_start_x = pNode->DES_CROP_POS_ST.bits.des_crop_start_x;
    stSliceData.des_crop_end_x  = pNode->DES_CROP_POS_ED.bits.des_crop_end_x;
  
    stSliceData.node_num = stSliceData.des_width / 256 + ((stSliceData.des_width % 256) != 0);

    for (n = 0; n < stSliceData.node_num; n++) //node_num
    {
        TDE_HWNode_S* pChildNode;
        HI_VOID* pBuf = NULL;
        pBuf = (HI_VOID*)TDE_MALLOC(sizeof(TDE_HWNode_S) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);

        if (HI_NULL == pBuf)
        {
            TDE_TRACE(TDE_KERN_INFO, "malloc (%d) failed, wgetfreenum(%d)!\n", (sizeof(TDE_HWNode_S)), wgetfreenum());
            return HI_ERR_TDE_NO_MEM;
        }

        pChildNode = (TDE_HWNode_S*)(pBuf + TDE_NODE_HEAD_BYTE);

        HI_GFX_Memcpy(pChildNode, pNode, sizeof(TDE_HWNode_S)); 
        
        stSliceData.des_xst_pos_blk = n * TDE_MAX_SLICE_WIDTH; // 0   256  512  768
        stSliceData.des_xed_pos_blk = (((n + 1) * TDE_MAX_SLICE_WIDTH) <= stSliceData.des_width) ? (((n + 1) * TDE_MAX_SLICE_WIDTH) - 1) :  stSliceData.des_width - 1; //255  511  767  1023

        for (i = 0; i < 2; i++)
        {
            TdeOsiInitSliceData(&stSliceData, pChildNode, i);

            TdeOsiCalcSliceS1(&stSliceData);

            TdeOsiCalcSliceSp(&stSliceData);
            
            TdeOsiCalcSliceMux(&stSliceData, pChildNode, i);           
        }
        
        TdeOsiFinalizeSliceCalculation(&stSliceData, pChildNode);
        
        if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pChildNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
        {
            TdeHalFreeNodeBuf(pChildNode);
            return s32Ret;
        }
     
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
#ifndef HI_BUILD_IN_BOOT/**codecc**/
STATIC  HI_VOID TdeOsiGetHUpdateInfo(TDE_UPDATECONFIG_S *pstReg, TDE_UPDATEINFO_S *pstInfo, HI_BOOL bScaler)
{
    HI_S32 s32ZmeHinstart=0, s32ZmeHinstop=0;
    HI_S32 s32ZmeHoutstart=0, s32ZmeHoutstop=0;
    HI_S32 s32UpdateHstart=pstReg->update_instart_w;
    HI_S32 s32UpdateHstop=s32UpdateHstart+pstReg->update_in_width-1;
    HI_S32 s32ZmeHphase=0;
    HI_S32 s32Ratio=(HI_S32)(4096*(pstReg->ori_in_width-1)/(pstReg->zme_out_width-1)+1/2);
    HI_S32 s32Dratio=4096*(pstReg->zme_out_width-1)/(pstReg->ori_in_width-1); 
    TDE_TRACE(TDE_KERN_DEBUG, "update_start:%d, update_hstop:%d, ori_in_width:%d\n", \
     s32UpdateHstart, s32UpdateHstop, pstReg->ori_in_width);

    if(HI_TRUE != bScaler)  /*hor_scaler not enable*/
    {
        pstInfo->zme_instart_w = pstReg->update_instart_w;
        pstInfo->zme_outstart_w = pstReg->update_instart_w;
        pstInfo->zme_in_width = pstReg->update_in_width;
        pstInfo->zme_out_width = pstReg->update_in_width;
        pstInfo->zme_hphase = 0;

        return;
    }

    //hor_scaler enable
    if(s32UpdateHstart>=0 && s32UpdateHstart<3) 
    {
        /*update outstretched area exceed left limit bordline*/
    	s32ZmeHinstart = 0;
    	s32ZmeHoutstart = 0;
    	s32ZmeHphase = 0;
    }
    else  
    {
        /*update outstretched area didn't exceed the left limit bordline*/
    	s32ZmeHinstart = (s32UpdateHstart-3)*s32Dratio;
    	s32ZmeHoutstart = (s32ZmeHinstart%4096)==0 ? (s32ZmeHinstart>>12): ((s32ZmeHinstart>>12)+1);
    	s32ZmeHinstart = s32ZmeHoutstart*s32Ratio>>12;	
    	if(s32ZmeHinstart-2<0)  
    	{
            /*the left few point need mirror pixels when scale*/
            s32ZmeHphase = (s32ZmeHoutstart*s32Ratio)%4096+s32ZmeHinstart*4096;
    	    s32ZmeHinstart = 0;
    	}
    	else 
    	{
            /*the left few point not need mirror pixels when scale*/
            s32ZmeHphase = (s32ZmeHoutstart*s32Ratio)%4096+3*4096;
    		s32ZmeHinstart = s32ZmeHinstart - 2;
    	}	
    }

    if( s32UpdateHstop>(pstReg->ori_in_width-3) && s32UpdateHstop<pstReg->ori_in_width)
    {
        /*update outstretched area exceed the right limit bordline*/
    	s32ZmeHinstop = pstReg->ori_in_width - 1;
    	s32ZmeHoutstop = pstReg->zme_out_width - 1;
    }
    else 
    {
        /*update outstretched area didn't exceed the right limit bordline*/
    	s32ZmeHinstop = (s32UpdateHstop+2+1)*s32Dratio;
    	s32ZmeHoutstop = (s32ZmeHinstop%4096)==0 ? ((s32ZmeHinstop>>12)-1): (s32ZmeHinstop>>12);
    	s32ZmeHinstop = s32ZmeHoutstop*s32Ratio>>12;
    	if(s32ZmeHinstop+3>(pstReg->ori_in_width-1)) 
    	{
            /*the right few point need mirror pixels when scale*/
    	    s32ZmeHinstop = pstReg->ori_in_width - 1;
    	}
    	else
    	{
            /*the right few point need mirror pixels when scale*/
    	    s32ZmeHinstop = s32ZmeHinstop + 3;
    	}
    }
    			
    pstInfo->zme_instart_w = s32ZmeHinstart;
    pstInfo->zme_outstart_w = s32ZmeHoutstart;
    pstInfo->zme_in_width = s32ZmeHinstop - s32ZmeHinstart +1;
    pstInfo->zme_out_width = s32ZmeHoutstop - s32ZmeHoutstart +1;
    pstInfo->zme_hphase = s32ZmeHphase;

    return ;
}

STATIC  HI_VOID TdeOsiGetVUpdateInfo(TDE_UPDATECONFIG_S *pstReg, TDE_UPDATEINFO_S *pstInfo, int bScaler, int bDeflicker)
{
    HI_S32 s32ZmeVinstart = 0, s32ZmeVinstop = 0;
    HI_S32 s32ZmeVoutstart = 0, s32ZmeVoutstop = 0;
    HI_S32 s32UpdateVstart = pstReg->update_instart_h;
    HI_S32 s32UpdateVstop = s32UpdateVstart + pstReg->update_in_height - 1;
    HI_S32 s32ZmeVphase = 0;
    HI_S32 s32Ratio = (HI_S32)(4096 * (pstReg->ori_in_height) / (pstReg->zme_out_height));
    HI_S32 s32Dratio = 4096 * (pstReg->zme_out_height) / (pstReg->ori_in_height);

    if (HI_TRUE != bScaler)
    {
        pstInfo->zme_instart_h = pstReg->update_instart_h;
        pstInfo->zme_in_height = pstReg->update_in_height;
        pstInfo->zme_outstart_h = pstReg->update_instart_h;
        pstInfo->zme_out_height = pstReg->update_in_height;
        pstInfo->zme_vphase = 0;
        pstInfo->def_offsetup = 0;
        pstInfo->def_offsetdown = 0;

        return;
    }

    //ver_scale enable & deflicker not enable
    if (s32UpdateVstart >= 0 && s32UpdateVstart < 2)
    {
        //the update outstreatched area exceed the up limit bordline
        s32ZmeVinstart = 0;
        s32ZmeVoutstart = 0;
        s32ZmeVphase = 0;
    }
    else
    {
        //the update outstreatched area didn't exceed the up limit bordline
        s32ZmeVinstart = (s32UpdateVstart - 2) * s32Dratio;
        s32ZmeVoutstart = (s32ZmeVinstart % 4096) == 0 ? (s32ZmeVinstart >> 12) : ((s32ZmeVinstart >> 12) + 1);
        s32ZmeVinstart = s32ZmeVoutstart * s32Ratio >> 12;

        if (s32ZmeVinstart - 1 < 0)
        {
            //the up few point need mirror pixels when scale
            s32ZmeVphase = (s32ZmeVoutstart * s32Ratio) % 4096 + s32ZmeVinstart * 4096;
            s32ZmeVinstart = 0;
        }
        else
        {
            //the up few point not need mirror pixels when scale
            s32ZmeVphase = (s32ZmeVoutstart * s32Ratio) % 4096 + 1 * 4096;
            s32ZmeVinstart = s32ZmeVinstart - 1;
        }
    }

    if (s32UpdateVstop > (pstReg->ori_in_height - 2) && s32UpdateVstop < pstReg->ori_in_height)
    {
        //the update outstreatched area exceed the down limit bordline
        s32ZmeVinstop = pstReg->ori_in_height - 1;
        s32ZmeVoutstop = pstReg->zme_out_height - 1;
    }
    else
    {
        //the update outstreatched area didn't exceed the down limit bordline
        s32ZmeVinstop = (s32UpdateVstop + 1 + 1) * s32Dratio;
        s32ZmeVoutstop = (s32ZmeVinstop % 4096) == 0 ? ((s32ZmeVinstop >> 12) - 1) : (s32ZmeVinstop >> 12);
        s32ZmeVinstop = s32ZmeVoutstop * s32Ratio >> 12;

        if (s32ZmeVinstop + 2 > (pstReg->ori_in_height - 1))
        {
            //the down few point need mirror pixels when scale
            s32ZmeVinstop = pstReg->ori_in_height - 1;
        }
        else
        {
            //the down few point not need mirror pixels when scale
            s32ZmeVinstop = s32ZmeVinstop + 2;
        }
    }

    pstInfo->zme_in_height = s32ZmeVinstop - s32ZmeVinstart + 1;
    pstInfo->zme_instart_h = s32ZmeVinstart;
    pstInfo->zme_outstart_h = s32ZmeVoutstart;
    pstInfo->zme_out_height = s32ZmeVoutstop - s32ZmeVoutstart + 1;
    pstInfo->zme_vphase = s32ZmeVphase;
    pstInfo->def_offsetup = 0;
    pstInfo->def_offsetdown = 0;


    return;

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
STATIC HI_S32 TdeOsiSetFilterNode(TDE_HANDLE s32Handle, TDE_HWNode_S* pNode,TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstDst,
                                  TDE2_RECT_S  *pstDstRect, TDE2_DEFLICKER_MODE_E enDeflickerMode,TDE2_FILTER_MODE_E enFilterMode)
{
    TDE2_COLOR_FMT_E enSrc1Fmt;
    TDE2_COLOR_FMT_E enSrc2Fmt;
    TDE2_COLOR_FMT_E enOutFmt;
    HI_BOOL bDeflicker;
    #ifndef HI_BUILD_IN_BOOT
    TDE_UPDATECONFIG_S reg;
    TDE_UPDATEINFO_S info;
    TDE2_RECT_S stUpdateInRect;
    TDE2_RECT_S stUpdateOutRect;
    HI_BOOL bScale = HI_FALSE;
    #endif
    enOutFmt = pstDst->enColorFmt;
    bDeflicker = (TDE2_DEFLICKER_MODE_NONE == enDeflickerMode)?HI_FALSE:HI_TRUE;
    if ((HI_NULL != pstBackGround)&&(HI_NULL != pstBackGroundRect))
    {
        enSrc1Fmt = pstBackGround->enColorFmt;
        TdeOsiCalcSrc1FilterOpt(pNode,enSrc1Fmt,enOutFmt,pstBackGroundRect,pstDstRect,enFilterMode);
    }
    if ((HI_NULL != pstForeGround)&&(HI_NULL != pstForeGroundRect))
    {
        enSrc2Fmt = pstForeGround->enColorFmt;
        TdeOsiCalcSrc2FilterOpt(pNode,enSrc2Fmt,enOutFmt,pstForeGroundRect,pstDstRect,bDeflicker,enFilterMode);
    #ifndef HI_BUILD_IN_BOOT
        if (((pstForeGround->u32Width != pstForeGroundRect->u32Width) || (pstForeGround->u32Height != pstForeGroundRect->u32Height))
        && s_bRegionDeflicker)
        {
            reg.ori_in_height = pstForeGround->u32Height;
            reg.ori_in_width = pstForeGround->u32Width;
            reg.zme_out_height = pstDst->u32Height;
            reg.zme_out_width = pstDst->u32Width;
            reg.update_instart_w = pstForeGroundRect->s32Xpos;
            reg.update_instart_h = pstForeGroundRect->s32Ypos;
            reg.update_in_width = pstForeGroundRect->u32Width;
            reg.update_in_height = pstForeGroundRect->u32Height;
            
            if (TDE_NO_SCALE_HSTEP != pNode->SRC2_HSP.bits.hratio)
            {
                bScale = HI_TRUE;
            }
            
        
            TDE_TRACE(TDE_KERN_DEBUG, "bHScale:%x\n", bScale);
            TdeOsiGetHUpdateInfo(&reg, &info, bScale);

            bScale = HI_FALSE;
            if (TDE_NO_SCALE_VSTEP != pNode->SRC2_VSR.bits.vratio)
            {
                bScale = HI_TRUE;
            }
            TDE_TRACE(TDE_KERN_DEBUG, "bVScale:%x\n", bScale);
            TdeOsiGetVUpdateInfo(&reg, &info, bScale, bDeflicker);

            pNode->SRC2_IMGSIZE.bits.src2_width = info.zme_in_width - 1;
            pNode->SRC2_ZMEIRESO.bits.iw= info.zme_in_width - 1;

            pNode->SRC2_IMGSIZE.bits.src2_height = info.zme_in_height - 1;
            pNode->SRC2_ZMEIRESO.bits.ih = info.zme_in_height - 1;

            pstDstRect->s32Xpos = info.zme_outstart_w;
            pstDstRect->s32Ypos = info.zme_outstart_h;
            pstDstRect->u32Width = info.zme_out_width;
            pstDstRect->u32Height = info.zme_out_height;

            
            pNode->SRC2_HCOFFSET = info.zme_hphase;
            pNode->SRC2_VOFFSET.bits.vluma_offset = info.zme_vphase;

            stUpdateInRect.s32Xpos = info.zme_instart_w;
            stUpdateInRect.s32Ypos = info.zme_instart_h;
            stUpdateInRect.u32Width = info.zme_in_width;
            stUpdateInRect.u32Height = info.zme_in_height;

            stUpdateOutRect.s32Xpos = info.zme_outstart_w;
            stUpdateOutRect.s32Ypos = info.zme_outstart_h;
            stUpdateOutRect.u32Width = info.zme_out_width;
            stUpdateOutRect.u32Height = info.zme_out_height;

            TdeOsiCalcSrc2FilterOpt(pNode,enSrc2Fmt,enOutFmt,&stUpdateInRect,&stUpdateOutRect,bDeflicker,enFilterMode);
            if (1 >= pstDst->u32Width)
            {
                pNode->SRC2_HSP.bits.hratio = 0;
            }
            else
            {
                pNode->SRC2_HSP.bits.hratio = ((pstForeGround->u32Width) << TDE_HAL_HSTEP_FLOATLEN) / (pstDst->u32Width);
            }

            if (1 >= pstDst->u32Height)
            {
                pNode->SRC2_VSR.bits.vratio = 0;
            }
            else
            {
                pNode->SRC2_VSR.bits.vratio = ((pstForeGround->u32Height) << TDE_FLOAT_BITLEN) / (pstDst->u32Height);
            }
            ;
            pNode->SRC2_CH0_ADDR.bits.src2_ch0_addr = pNode->SRC2_CH0_ADDR.bits.src2_ch0_addr + stUpdateInRect.s32Ypos*pNode->SRC2_CH0_STRIDE.bits.src2_ch0_stride\
                                                                                           +((stUpdateInRect.s32Xpos*TdeOsiGetbppByFmt(enSrc2Fmt))>>3);
             pNode->DES_CH0_ADDR.bits.des_ch0_addr = pNode->DES_CH0_ADDR.bits.des_ch0_addr + stUpdateOutRect.s32Ypos*pNode->DES_CH0_STRIDE.bits.des_ch0_stride\
                                                                                           +((stUpdateOutRect.s32Xpos*TdeOsiGetbppByFmt(enOutFmt))>>3);                                                                               
        }
         #endif
    }
       
        return TdeOsiCalcSlice(s32Handle,pNode);
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
     
    if(TdeHalNodeInitNd(&pstHWNode)<0)
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
		HI_BOOL bOutAlphaFromError;
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
        
        bOutAlphaFromError = HI_FALSE;
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

#endif

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

    if (TdeHalNodeSetBaseOperate(pstHWNode, enBaseMode, enAluMode, &stDrvColorFill) < 0)
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
#ifndef HI_BUILD_IN_BOOT

STATIC HI_S32 TdeOsiSetForegroundColorKey(TDE_HWNode_S* pstHWNode , TDE2_SURFACE_S* pstSrc, TDE2_OPT_S* pstOpt , TDE_CLUT_USAGE_E enClutUsage)
{
    TDE_DRV_COLORKEY_CMD_S stColorkey;
    TDE_COLORFMT_CATEGORY_E enFmtCategory;
    HI_BOOL bForegroundColorkeyAfterClut;
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

STATIC HI_S32 TdeOsiSetBlend(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , TDE2_BLEND_OPT_S stBlendOpt , TDE_DRV_ALU_MODE_E* enAluMode, HI_BOOL bCheckBlend)
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


STATIC HI_S32 TdeOsiSetColorize(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , HI_S32 u32Colorize)
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



STATIC HI_S32 TdeOsiSetRop(TDE_HWNode_S* pstHWNode , TDE2_ALUCMD_E enAluCmd , TDE2_ROP_CODE_E enRopCode_Color ,
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
#ifndef HI_BUILD_IN_BOOT
STATIC HI_S32 TdeOsi2SourceFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S*  pstSrcRect,
                                TDE2_SURFACE_S* pstDst,
                                TDE2_RECT_S*  pstDstRect, TDE2_FILLCOLOR_S* pstFillColor,
                                TDE2_OPT_S* pstOpt)
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
    TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;
    TDE2_RECT_S stSrcRect = {0};
    TDE2_RECT_S stDstRect = {0};
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL bSetFileNode;
    HI_BOOL bCheckSingleSrc2Rop = HI_FALSE;
    HI_BOOL bCheckBlend = HI_TRUE;
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



    bSetFileNode = ((pstOpt->bResize) || (pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE) || (pstSrc->enColorFmt >= TDE2_COLOR_FMT_YCbCr422));

    if (bSetFileNode)
    {

        if ((s32Ret =  TdeOsiSetFilterNode(s32Handle, pstHWNode, pstSrc, &stSrcRect, NULL, NULL, \
                                           pstDst, &stDstRect, pstOpt->enDeflickerMode, pstOpt->enFilterMode)) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
       }
       TdeHalFreeNodeBuf(pstHWNode);
       return HI_SUCCESS;
    }

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;

}

#endif

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


STATIC HI_S32 TdeOsiPreCheckSurface(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S*  pstRect)
{
    HI_S32 s32Ret ;
    HI_BOOL bUnknownColorFmt, bRasterFmt;
    TDE_CHECK_COLORFMT(pstSurface->enColorFmt);
    
    TDE_CHECK_SURFACEPARA(pstSurface,pstRect);
    
    bUnknownColorFmt = TDE2_COLOR_FMT_BUTT <= pstSurface->enColorFmt;   
    if (bUnknownColorFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "Unkown color format!\n");
        return -1;
    }
    
    TDE_YCBCR422_FMT_CHECK_ODD(pstSurface->enColorFmt, pstRect->s32Xpos, pstRect->u32Width);
    
    bRasterFmt = (pstSurface->enColorFmt <= TDE2_COLOR_FMT_halfword) ;
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
        /* AI7D02547 */
        pstRect->u32Height = pstSurface->u32Height - pstRect->s32Ypos;
    }
    return HI_SUCCESS;
}


#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeOsiCheckSurfaceEX
* Description:   adjust right operate zone, according by the size of bitmap is more than 4095*4095,and less than 8190*8190
* Input:         pstSurface: bitmap info
*                pstRect: bitmap operate zone
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
STATIC INLINE HI_S32 TdeOsiCheckSurfaceEX(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S*  pstRect)
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
STATIC INLINE HI_S32 TdeOsiCheckSurface(TDE2_SURFACE_S* pstSurface, TDE2_RECT_S*  pstRect)
{
    HI_S32 s32Ret ;
    HI_BOOL bInvalidOperationArea;

    if ( ( s32Ret = TdeOsiPreCheckSurface( pstSurface,  pstRect) ) != HI_SUCCESS)
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
    TDE2_SURFACE_S *pTmpSrc2 = HI_NULL;
    TDE2_RECT_S *pTmpSrc2Rect = HI_NULL;

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

    TDE_CHECK_NOT_MB(pstBackGround->enColorFmt);

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
     //   pstDrvSur->u32Xpos = (HI_U32)pstRect->s32Xpos + pstRect->u32Width - 1;
    }
    else
    {
       // pstDrvSur->u32Xpos   = (HI_U32)pstRect->s32Xpos;
    }
    pstDrvSur->u32Xpos   = (HI_U32)pstRect->s32Xpos;
    if(TDE_SCAN_DOWN_UP == pstScanInfo->enVScan)
    {
      //  pstDrvSur->u32Ypos = (HI_U32)pstRect->s32Ypos + pstRect->u32Height - 1;
    }
    else
    {
      //  pstDrvSur->u32Ypos   = (HI_U32)pstRect->s32Ypos;
    }
    pstDrvSur->u32Ypos   = (HI_U32)pstRect->s32Ypos;
    if(NULL != pstOperationArea)
    {
        
        pstOperationArea->s32Xpos = pstRect->s32Xpos;
        pstOperationArea->s32Ypos = (HI_S32)pstDrvSur->u32Ypos;
        pstOperationArea->u32Width = pstDrvSur->u32Width;
        pstOperationArea->u32Height = pstDrvSur->u32Height;
    }

    return;
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

        stClip.bInsideClip = HI_TRUE;
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos;
        stClip.u16ClipEndX = (pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos) + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = (pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos) + pstOpt->stClipRect.u32Height - 1;
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
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos;
        stClip.u16ClipEndX = (pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos) + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = (pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos) + pstOpt->stClipRect.u32Height - 1;

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
            stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos;
            stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos;
            stClip.u16ClipEndX = (pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos) + pstOpt->stClipRect.u32Width - 1;
            stClip.u16ClipEndY = (pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos) + pstOpt->stClipRect.u32Height - 1;

    
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
        
        stClip.u16ClipStartX = pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos;
        stClip.u16ClipStartY = pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos;
        stClip.u16ClipEndX = (pstOpt->stClipRect.s32Xpos - pstDstRect->s32Xpos) + pstOpt->stClipRect.u32Width - 1;
        stClip.u16ClipEndY = (pstOpt->stClipRect.s32Ypos - pstDstRect->s32Ypos) + pstOpt->stClipRect.u32Height - 1;

        
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

    if((NULL != pstForeGround)&&(TDE2_COLOR_FMT_ARGB1555 <= pstForeGround->enColorFmt)
        && (pstForeGround->enColorFmt <= TDE2_COLOR_FMT_BGRA1555))
    {
        TdeHalNodeSetSrc2Alpha(pstHwNode);
    }

    if((NULL != pstBackGround)
        && (TDE2_COLOR_FMT_ARGB1555 <= pstBackGround->enColorFmt)
        && (pstBackGround->enColorFmt <= TDE2_COLOR_FMT_BGRA1555))
    {
        TdeHalNodeSetSrc1Alpha(pstHwNode);
    }
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
	HI_S32 s32Ret;

    if ((HI_NULL == pstHwNode) || (HI_NULL == pstOpt))
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
    //#ifndef HI_BUILD_IN_BOOT/**codecc**/
    else
    {
        enBaseOpt = TDE_NORM_BLIT_1OPT;
    }
	//#endif
    
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
    if(pHWNode->DES_ALPHA.bits.des_crop_en)
    {
    
        HI_U32 u32ClipStartX, u32ClipStartY, u32ClipEndX, u32ClipEndY;
        HI_U32 u32OutStartX, u32OutStartY, u32OutEndX, u32OutEndY;
        HI_U32 u32StartX, u32StartY, u32EndX, u32EndY;

        u32ClipStartX = pHWNode->DES_CROP_POS_ST.bits.des_crop_start_x & 0xfff;
        u32ClipStartY = pHWNode->DES_CROP_POS_ST.bits.des_crop_start_y & 0xfff;
        u32ClipEndX = pHWNode->DES_CROP_POS_ED.bits.des_crop_end_x & 0xfff;
        u32ClipEndY = pHWNode->DES_CROP_POS_ED.bits.des_crop_end_y & 0xfff;

        u32OutStartX = 0;//unDesSurXY.bits.des_start_x & 0xfff;
        u32OutStartY = 0;//unDesSurXY.bits.des_start_y & 0xfff;
        u32OutEndX = u32OutStartX + ((pHWNode->DES_IMGSIZE.bits.des_width+ 1) & 0xfff) - 1;
        u32OutEndY = u32OutStartY + ((pHWNode->DES_IMGSIZE.bits.des_height +1) & 0xfff) - 1;

        u32StartX = (u32ClipStartX > u32OutStartX) ? u32ClipStartX : u32OutStartX;
        u32StartY = (u32ClipStartY > u32OutStartY) ? u32ClipStartY : u32OutStartY;
        u32EndX = (u32ClipEndX < u32OutEndX) ? u32ClipEndX : u32OutEndX;
        u32EndY = (u32ClipEndY < u32OutEndY) ? u32ClipEndY : u32OutEndY;
        if((u32StartX > u32EndX) || (u32StartY > u32EndY))
        {
            pHWNode->DES_ALPHA.bits.des_crop_en = 0;
        }
        else
        {
            pHWNode->DES_CROP_POS_ST.bits.des_crop_start_x = u32StartX;
            pHWNode->DES_CROP_POS_ST.bits.des_crop_start_y = u32StartY;
            pHWNode->DES_CROP_POS_ED.bits.des_crop_end_x = u32EndX;
            pHWNode->DES_CROP_POS_ED.bits.des_crop_end_y = u32EndY;
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
#endif
STATIC HI_S32 TdeOsiCheckSingleSr2Para(TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S*  pstForeGroundRect,
                                       TDE2_SURFACE_S* pstDst, TDE2_RECT_S*  pstDstRect, TDE2_OPT_S* pstOpt)
{
    HI_BOOL bContainNullPtr;
    TDE_ASSERT(NULL != pstOpt);
    
    bContainNullPtr =  ((NULL == pstDst) || (NULL == pstDstRect) || (NULL == pstOpt)\
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
 HI_S32 TdeOsiSingleSrc2Blit(TDE_HANDLE s32Handle,
                                                                TDE2_SURFACE_S* pstForeGround,
                                                                TDE2_RECT_S  *pstForeGroundRect,
                                                                TDE2_SURFACE_S* pstDst,
                                                                TDE2_RECT_S  *pstDstRect,
                                                                TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst)
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
    TDE2_RECT_S stForeRect = {0};
    TDE2_RECT_S stDstRect = {0};
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL bSetFilterNode ;

    TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;

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

    bSetFilterNode = ((pstOpt->bResize) || (pstOpt->enDeflickerMode != TDE2_DEFLICKER_MODE_NONE) || (pstForeGround->enColorFmt >= TDE2_COLOR_FMT_YCbCr422));

    if (bSetFilterNode)
    {
        if((pstForeGround->enColorFmt>=TDE2_COLOR_FMT_YCbCr422)&&(!(pstOpt->bResize)))
        {
            pstForeGroundRect->u32Height = stDstRect.u32Height;
            pstForeGroundRect->u32Width = stDstRect.u32Width;
        }
        if ((s32Ret =  TdeOsiSetFilterNode(s32Handle,pstHWNode, NULL,NULL,pstForeGround,pstForeGroundRect,\
                                    pstDst,&stDstRect,pstOpt->enDeflickerMode,pstOpt->enFilterMode)) < 0)
       {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
       }
       TdeHalFreeNodeBuf(pstHWNode);
       return HI_SUCCESS;
    }

    if ((s32Ret = TdeOsiSetNodeFinish(s32Handle, pstHWNode, 0, TDE_NODE_SUBM_ALONE)) < 0)
    {
        TdeHalFreeNodeBuf(pstHWNode);
        return s32Ret;
    }

    return HI_SUCCESS;
}
#ifndef HI_BUILD_IN_BOOT
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
STATIC HI_S32 TdeOsiDoubleSrcBlit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S*  pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S*  pstForeGroundRect,
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
    HI_S32 s32Ret = HI_FAILURE;
    TDE_DRV_CONV_MODE_CMD_S stConv = {0};
    TDE2_SURFACE_S stTempSur = {0};
    TDE2_RECT_S stForeRect = {0};
    TDE2_RECT_S stDstRect = {0};
    TDE2_RECT_S stBackRect = {0};
    HI_BOOL bSetFilterNode;

    TDE_CLUT_USAGE_E enClutUsage = TDE_CLUT_USAGE_BUTT;

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
    TdeOsiSetExtAlpha(&stTempSur, pstForeGround, pstHWNode);

    bSetFilterNode = ((pstOpt->bResize) || (TDE2_DEFLICKER_MODE_NONE != pstOpt->enDeflickerMode)\
                      || (pstForeGround->enColorFmt >= TDE2_COLOR_FMT_YCbCr422) || (pstBackGround->enColorFmt >= TDE2_COLOR_FMT_YCbCr422));

    if (bSetFilterNode)
    {
        //stFilterOpt.enFilterMode = g_enTdeFilterMode[pstOpt->enFilterMode];   
        
        pstBackGroundRect->u32Height = stDstRect.u32Height;
        pstBackGroundRect->u32Width = stDstRect.u32Width;
        if((pstForeGround->enColorFmt>=TDE2_COLOR_FMT_YCbCr422)&&(!(pstOpt->bResize)))
        {
            pstForeGroundRect->u32Height = stDstRect.u32Height;
            pstForeGroundRect->u32Width = stDstRect.u32Width;
        }
        if ((s32Ret = TdeOsiSetFilterNode(s32Handle,pstHWNode, pstBackGround,pstBackGroundRect,pstForeGround,&stForeRect,\
                                    pstDst,&stDstRect,pstOpt->enDeflickerMode,pstOpt->enFilterMode)) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return s32Ret;
        }
        TdeHalFreeNodeBuf(pstHWNode);
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
		                                        pstDst, pstDstRect,HI_FALSE, HI_FALSE);
		        }
		        else
		        {         
		            return TdeOsiSingleSrc1Blit(s32Handle, pstBackGround, pstBackGroundRect,
		                                        pstDst, pstDstRect,HI_FALSE, HI_FALSE);
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
		            if ((s32Ret = TdeOsiSingleSrc2Blit(s32Handle, pstBackGround, pstBackGroundRect,
		                                        pstDst, pstDstRect, pstOpt,HI_FALSE,HI_FALSE)) < 0 ) /*AI7D02940*/
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

STATIC HI_S32 TdeOsiCheckMbBlitPara(TDE2_MB_S* pstMB, TDE2_RECT_S*  pstMbRect,
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

STATIC HI_S32 TdeOsiGetDoubleCycleData(TDE2_RECT_S*  pstMbRect)
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

    if ((pstDstRect->u32Height == pstMbRect->u32Height)
        && (pstDstRect->u32Width == pstMbRect->u32Width))
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
        
        for(m=0;m<i;m++)
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

    if ((NULL == pstBackGround) || (NULL == pstBackGroundRect) || (NULL == pstForeGround)
        || (NULL == pstForeGroundRect)
        || (NULL == pstMask) || (NULL == pstMaskRect) || (NULL == pstDst) || (NULL == pstDstRect))
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
    if(0==stMidSurface.u32PhyAddr)
    {
        TDE_TRACE(TDE_KERN_INFO, "There is no temp buffer in TdeOsiBitmapMaskRop!\n");
        return HI_ERR_TDE_NO_MEM;
    }
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
    if(0==stMidSurface.u32PhyAddr)
    {
        TDE_TRACE(TDE_KERN_INFO, "There is no temp buffer in TdeOsiBitmapMaskBlend!\n");
        return HI_ERR_TDE_NO_MEM;
    }
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

    HI_BOOL bUnknownColorFormat;

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
STATIC TDE_OPERATION_CATEGORY_E TdeOsiSingleSrcPatternOperation(TDE2_SURFACE_S* pstBackGround,
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

        if (TDE2_COLOR_FMT_YCbCr422 == pstForeGround->enColorFmt)
        {
            TDE_TRACE(TDE_KERN_INFO, "It doesn't support YCbCr422 in pattern fill!\n");
            return -1;
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

STATIC TDE_OPERATION_CATEGORY_E TdeOsiDoubleSrcPatternOperation(TDE2_SURFACE_S* pstBackGround,
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

    if (TDE2_COLOR_FMT_YCbCr422 == pstForeGround->enColorFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "It doesn't support YCbCr422 in pattern fill!\n");
        return -1;
    }

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
        TDE2_RECT_S*  pstBackGroundRect, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S*  pstForeGroundRect,
        TDE2_SURFACE_S* pstDst, TDE2_RECT_S*  pstDstRect, TDE2_PATTERN_FILL_OPT_S* pstOpt)
{
    if ((NULL == pstDst) || (NULL == pstDstRect))
    {
        TDE_TRACE(TDE_KERN_INFO, "Dst/Dst Rect should not be null!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }
    if(TDE2_COLOR_FMT_YCbCr422==pstDst->enColorFmt)
    {
        TDE_TRACE(TDE_KERN_INFO, "It doesn't support YCbCr422 in pattern fill!\n");
        return -1;
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
                                               pstForeGroundRect , pstDst, pstDstRect , pstOpt);
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


    {
        
        if (pstRect2->u32Height < pstRect1->u32Height)
        {
            pstRect1->u32Height = pstRect2->u32Height;
        }

       
        if (pstRect2->u32Width < pstRect1->u32Width)
        {
            pstRect1->u32Width = pstRect2->u32Width;
        }
    }
    

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

STATIC HI_S32 TdeOsiCheckSingleSrcPatternFillPara(TDE2_SURFACE_S* pstSrc,  TDE2_RECT_S* pstSrcRect,
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
    
    if(TdeHalNodeInitNd(&pstHWNode)<0)
    {
        return HI_ERR_TDE_NO_MEM;
    }
    enBaseMode = TDE_SINGLE_SRC_PATTERN_FILL_OPT;
    enAluMode = TDE_ALU_NONE;

    if(pstOpt != HI_NULL)
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

        s32Ret = TdeOsiSetRop( pstHWNode , pstOpt->enAluCmd, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha , &enAluMode , bCheckSingleSrc2Rop);

        if (s32Ret)
        {
            return s32Ret;
        }
        
        s32Ret = TdeOsiSetBlend( pstHWNode ,  pstOpt->enAluCmd, pstOpt->stBlendOpt , &enAluMode , bCheckBlend);

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

       
        if(TdeOsiSetPatternClipPara(HI_NULL, HI_NULL, pstSrc, pstSrcRect, pstDst, pstDstRect, pstOpt, pstHWNode) < 0)
        {
            TdeHalFreeNodeBuf(pstHWNode);
            return HI_ERR_TDE_CLIP_AREA;
        }
    }
    
    if(TdeHalNodeSetBaseOperate(pstHWNode, enBaseMode, enAluMode, HI_NULL)<0)
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

    (pstOpt != HI_NULL) ? TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, pstOpt->enOutAlphaFrom) : \
    TdeHalNodeSetTgt(pstHWNode, &stDstDrvSurface, TDE2_OUTALPHA_FROM_FOREGROUND);

   
    u16Code = TdeOsiSingleSrc2GetOptCode(pstSrc->enColorFmt, pstDst->enColorFmt);

    TdeOsiGetConvbyCode(u16Code, &stConv);

    if(TdeHalNodeSetColorConvert(pstHWNode, &stConv)<0)
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

STATIC HI_S32 TdeOsiCheckDoubleSrcPatternFillPara( TDE2_SURFACE_S* pstBackGround,
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

    s32Ret = TdeOsiSetRop( pstHWNode , pstOpt->enAluCmd, pstOpt->enRopCode_Color, pstOpt->enRopCode_Alpha , &enAluMode , bCheckSingleSrc2Rop);

    if (s32Ret)
    {
        return s32Ret;
    }

    s32Ret = TdeOsiSetBlend( pstHWNode ,  pstOpt->enAluCmd, pstOpt->stBlendOpt , &enAluMode , bCheckBlend);

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

    s32Ret = TdeOsiSetColorKey( pstForeGround , pstBackGround , pstHWNode, pstOpt->unColorKeyValue , pstOpt->enColorKeyMode , enClutUsage);

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
    TDE_UPDATECONFIG_S reg;
    TDE_UPDATEINFO_S info;


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

HI_S32 TdeOsiSingleBlitEx(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst)
{
    return TdeOsiSingleSrc2Blit(s32Handle, pstSrc, pstSrcRect,pstDst, pstDstRect,pstOpt,bMMZForSrc,bMMZForDst);
}
EXPORT_SYMBOL(TdeOsiPatternFill);
EXPORT_SYMBOL(TdeOsiEnableRegionDeflicker);
EXPORT_SYMBOL(TdeCalScaleRect);

#endif 


#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

