/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/

#ifndef _ARIB_CC_GRAPHIC_H_
#define _ARIB_CC_GRAPHIC_H_

#include "hi_type.h"
#include "aribcc_def.h"
#include "aribcc_osd.h"
#include "hi_unf_cc.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "c"
{
#endif


#define ARIBCC_TRANSPARENT_COLOR    8
#define MAX_CCSD_WINDOWS            2   /*one for cc,one for sd*/
#define ACG_TYPE_SD 0 
#define ACG_TYPE_CC 1 

//#define ARIBCC_FONT_NAME     "MS_Gothic.ttf"

typedef struct tagARIBCC_Pallette_S
{
	HI_U8 r;
	HI_U8 g;
	HI_U8 b;
	HI_U8 padding;
} ARIBCC_Pallette_S;


HI_S32 ARIBCC_GRAPHIC_Init(void);
HI_S32 ARIBCC_GRAPHIC_DeInit(void);

HI_S32 ARIBCC_GRAPHIC_CreateScreen(HI_S32 ccType, HI_S32 width, HI_S32 height);
HI_S32 ARIBCC_GRAPHIC_DestroyScreen(HI_S32 ccType);
HI_S32 ARIBCC_GRAPHIC_ClearScreen(HI_S32 ccType);
HI_S32 ARIBCC_GRAPHIC_TextOut(HI_S32 ccType,HI_S32 x, HI_S32 y, void *str, HI_S32 length, HI_S32 flag,HI_S32 space_interval);
HI_S32 ARIBCC_GRAPHIC_TextOutEUC(HI_S32 ccType,HI_S32 x, HI_S32 y, void *str, HI_S32 length, HI_S32 flag);

HI_S32 ARIBCC_GRAPHIC_FillRect(HI_S32 ccType, HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, HI_UNF_CC_COLOR_S color);
HI_S32 ARIBCC_GRAPHIC_DrawLine(HI_S32 ccType,HI_S32 x1,HI_S32 y1,HI_S32 x2,HI_S32 y2, HI_UNF_CC_COLOR_S color);
HI_S32 ARIBCC_GRAPHIC_BlockMove(HI_S32 ccType,HI_S32 srcx, HI_S32 srcy, HI_S32 width, HI_S32 height, HI_S32 dstx, HI_S32 dsty);
HI_S32 ARIBCC_GRAPHIC_DrawFontBitmap(HI_S32 ccType,HI_S32 x,HI_S32 y,HI_U16 width, HI_U16 height, HI_S32 depth, HI_U8 *pbitmap, HI_U32 bitmaplen);
HI_S32 ARIBCC_GRAPHIC_SetUnderline(HI_S32 ccType,HI_S32 is_under);
HI_S32 ARIBCC_GRAPHIC_SetOrnament(HI_S32 ccType,HI_S32 ornament);
HI_S32 ARIBCC_GRAPHIC_SetFontScale(HI_S32 ccType,ARIBCC_CHARSIZE_E scale);
HI_S32 ARIBCC_GRAPHIC_SetPrintDirection(HI_S32 ccType,HI_S32 direction);       /*0: horizontal, 1: vertical*/

HI_S32 ARIBCC_GRAPHIC_SetPalette(HI_S32 ccType, ARIBCC_Pallette_S *pLut, HI_S32 start, HI_S32 length);
HI_S32 ARIBCC_GRAPHIC_SetAribPalette(HI_S32 ccType);
HI_S32 ARIBCC_GRAPHIC_GetAribPalette(HI_U8 u8Index, HI_UNF_CC_COLOR_S *color);
HI_S32 ARIBCC_GRAPHIC_SetFlashOpaqu(HI_S32 ccType,HI_U8 is_same_with_fg, HI_U8 alpha);

HI_S32 ARIBCC_GRAPHIC_SetFlashBGColor(HI_S32 ccType, HI_UNF_CC_COLOR_S fg_color, HI_UNF_CC_COLOR_S bg_color);

HI_S32 ARIBCC_GRAPHIC_SetFGColor(HI_S32 ccType,HI_UNF_CC_COLOR_S color, HI_S32 is_flash);
HI_S32 ARIBCC_GRAPHIC_SetBGColor(HI_S32 ccType,HI_UNF_CC_COLOR_S color);
HI_S32 ARIBCC_GRAPHIC_SetMidColor(HI_S32 ccType,HI_UNF_CC_COLOR_S fg_mid_color, HI_UNF_CC_COLOR_S bg_mid_color);
HI_S32 ARIBCC_GRAPHIC_SetEdgeColor(HI_S32 ccType,HI_UNF_CC_COLOR_S edge_color);
HI_S32 ARIBCC_GRAPHIC_SetFontSize(HI_S32 ccType,HI_U8 size);
HI_S32 ARIBCC_GRAPHIC_GetTextSize(HI_S32 ccType,HI_S32 *pWidth, HI_S32 *pHeight);

HI_S32 ARIBCC_GRAPHIC_SysMem2FBCopy(HI_S32 ccType,HI_U8 *pBuf,HI_S32 width,HI_S32 height,HI_S32 bpp,HI_S32 rowbytes,HI_S32 dstx,HI_S32 dsty);

HI_S32 ARIBCC_GRAPHIC_RollLeft(HI_S32 ccType,HI_U16 x,HI_U16 y, HI_U16 width,HI_U16 height,
                HI_U16 offset,HI_UNF_CC_COLOR_S win_color,HI_U8 speed);
HI_S32 ARIBCC_GRAPHIC_Rollup(HI_S32 ccType,HI_U16 x, HI_U16 width, HI_U16 top_y,
                 HI_U16 btm_y,HI_U16 offset, HI_UNF_CC_COLOR_S win_color,HI_U8 speed);
HI_S32 ARIBCC_GRAPHIC_RollupStart(HI_S32 ccType);

HI_S32 ARIBCC_GRAPHIC_NewWindow(HI_S32 ccType,HI_S32 x,HI_S32 y,HI_S32 width,HI_S32 height);
HI_S32 ARIBCC_GRAPHIC_DelWindow(HI_S32 ccType);
HI_S32 ARIBCC_GRAPHIC_ShowWindow(HI_S32 ccType,HI_S32 dstx,HI_S32 dsty,HI_S32 width,HI_S32 height,HI_S32 startx,HI_S32 starty,HI_S32 is_colorkey_enable);
HI_S32 ARIBCC_GRAPHIC_FillWindow(HI_S32 ccType,HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, HI_UNF_CC_COLOR_S color);
HI_S32 ARIBCC_GRAPHIC_SetScaleFactor(HI_S32 ccType, HI_S32 vscale, HI_S32 hscale, HI_S32 fifo_lvl);
HI_S32 ARIBCC_GRAPHIC_SetResolution(ARIBCC_WINDOWFORMAT_E resolution);

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif

#endif
