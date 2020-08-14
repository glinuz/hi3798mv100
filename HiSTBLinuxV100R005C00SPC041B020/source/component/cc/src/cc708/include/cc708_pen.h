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
#ifndef __CC708_PEN_H__
#define __CC708_PEN_H__

#include "hi_type.h"
#include "hi_cc708_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
HI_U8 CC708_Pen_SetPenSize(CC708_PenDef_S *pstPen, HI_U8 u8PenSize, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetFontStyle(CC708_PenDef_S *pstPen, HI_U8 u8FontStyle, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextTag(CC708_PenDef_S *pstPen, HI_U8 u8TextTag, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextOffset(CC708_PenDef_S *pstPen, HI_U8 u8TextOffset, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextItalics(CC708_PenDef_S *pstPen, HI_U8 u8IsItalic, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextUnderline(CC708_PenDef_S *pstPen, HI_U8 u8IsUnderline, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextEdgeType(CC708_PenDef_S *pstPen, HI_U8 u8EdgeType, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextEdgeColor(CC708_PenDef_S *pstPen, HI_U32 u32EdgeColor, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextFgColor(CC708_PenDef_S *pstPen, HI_S32 s32FGColor, HI_S32 s32FGOpacity, HI_U8 u8IsVisible);

HI_U8 CC708_Pen_SetTextBgColor(CC708_PenDef_S *pstPen, HI_S32 s32BGColor, HI_S32 s32BGOpacity, HI_U8 u8IsVisible);

#ifdef __cplusplus
}
#endif
#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/
