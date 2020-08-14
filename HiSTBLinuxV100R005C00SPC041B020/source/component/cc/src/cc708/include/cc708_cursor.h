
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
#ifndef __CC_CURSOR_H__
#define __CC_CURSOR_H__

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
void CC708_Cursor_ResetToOrigin(CC708_Window_S *pstWindow, CC708_Cursor_S * pstCursor, 
                               CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorForwards(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 columnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorBackwards(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 columnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorToPosition(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32Column, 
                                      HI_U32 u32Y, HI_U32 u32Row,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorForwardsOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorForwardsOffScreen(CC708_Cursor_S * pstCursor, HI_U32 columnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorBackwardsOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorBackwardsOffScreen(CC708_Cursor_S * pstCursor, HI_U32 columnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32Y,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

void CC708_Cursor_MoveCursorOffScreen(CC708_Cursor_S * pstCursor, HI_U32 u32Row, HI_U32 u32Column,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);

int CC708_Cursor_GetCurrentColumn(CC708_Cursor_S * pstCursor);

int CC708_Cursor_GetCurrentRow(CC708_Cursor_S * pstCursor);

int CC708_Cursor_GetCurrentXPosition(CC708_Cursor_S * pstCursor, 
                                    CC708_PrintDirection_E enPrintDirection, 
                                    CC708_ScrollDirection_E enScrollDirection);
int CC708_Cursor_GetCurrentYPosition(CC708_Cursor_S * pstCursor,
                                    CC708_PrintDirection_E enPrintDirection, 
                                    CC708_ScrollDirection_E enScrollDirection);

#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/
