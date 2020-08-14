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
#ifndef __CC708_OSD_H__
#define __CC708_OSD_H__

#include "hi_type.h"
#include "cc708.h"

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
HI_S32 CC708_OSD_Start(HI_VOID);
HI_S32 CC708_OSD_Stop(HI_VOID);
HI_S32 CC708_OSD_Reset(HI_VOID);


HI_S32 CC708_OSD_DefineWindow (HI_U8 u8WindowID,HI_U8 u8Priority,HI_U8 u8AnchorPoint,
                                            HI_U8 u8RelativePositioning,HI_U8 u8AnchorVertical,
                                            HI_U8 u8AnchorHorizontal,HI_U8 u8RowCount,HI_U8 u8ColumnCount,
                                            HI_U8 u8RowLock,HI_U8 u8ColumnLock,HI_U8 u8IsVisible,
                                            HI_U8 u8WindowStyleID, HI_U8 u8PenStyleID);
HI_S32 CC708_OSD_SetWindowAttributes (HI_U8 u8Justify,HI_U8 u8PrintDirection,HI_U8 u8ScrollDirection,
                                                      HI_U8 u8WordWrap,HI_U8 u8DisplayEffect,HI_U8 u8EffectDirection,
                                                      HI_U8 u8EffectSpeed,HI_U32 u32FillColor,HI_U8 u8FillOpacity,
                                                      HI_U8 u8BorderType,HI_U32 u32BorderColor);

HI_S32 CC708_OSD_SetCurrentWindow(HI_U8 u8WindowID);
HI_S32 CC708_OSD_ClearWindows(HI_U8 u8WindowMap);
HI_S32 CC708_OSD_DeleteWindows(HI_U8 u8WindowMap);
HI_S32 CC708_OSD_DisplayWindows(HI_U8 u8WindowMap);
HI_S32 CC708_OSD_HideWindows(HI_U8 u8WindowMap);
HI_S32 CC708_OSD_ToggleWindows(HI_U8 u8WindowMap);
HI_S32 CC708_OSD_CharFlash(HI_VOID);


HI_S32 CC708_OSD_SetPenAttributes (HI_U8 u8FontSize,HI_U8 u8FontName,HI_U8 u8TextTag,HI_U8 u8TextOffset,
                                               HI_U8 u8IsItalic,HI_U8 u8IsUnderline,HI_U8 u8EdgeType);
HI_S32 CC708_OSD_SetPenColor (HI_U32 u32FGColor,HI_U8 u8FGOpacity,HI_U32 u32BGColor,
                                          HI_U8 u8BGOpacity,HI_U32 u32EdgeColor);
HI_S32 CC708_OSD_SetPenLocation (HI_U8 u8Row,HI_U8 u8Column);

HI_S32 CC708_OSD_ETX(HI_VOID);
HI_S32 CC708_OSD_BSText(HI_VOID);
HI_S32 CC708_OSD_FFText(HI_VOID);
HI_S32 CC708_OSD_CRText(HI_VOID);
HI_S32 CC708_OSD_HCRText(HI_VOID);

HI_S32 CC708_OSD_ClrCCScreen(HI_VOID);
HI_VOID CC708_OSD_DrawCaptionToWindow(HI_U16 *pu16Text,HI_S32 s32Len);

#ifdef __cplusplus
}
#endif

#endif //#ifndef _DTVCCOSD_H_

/*****************************************************************************
*                    End Of File
*****************************************************************************/
