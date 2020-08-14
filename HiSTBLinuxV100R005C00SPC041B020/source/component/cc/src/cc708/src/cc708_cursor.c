
#include "hi_cc708_def.h"
#include "cc708_cursor.h"
#include "hi_type.h"

void CC708_Cursor_ResetToOrigin(CC708_Window_S *pstWindow, CC708_Cursor_S * pstCursor, 
                                           CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32CurRow = 0;
    pstCursor->s32CurColumn = 0;
    pstCursor->s32X = pstWindow->u16X;
    pstCursor->s32Y = pstWindow->u16Y+1;
    return;

}

void CC708_Cursor_MoveCursorForwards(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32ColumnCount,
                                    CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32X += u32X;
    pstCursor->s32CurColumn += u32ColumnCount;
    return;
}

void CC708_Cursor_MoveCursorBackwards(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32ColumnCount,
                                     CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{

    pstCursor->s32X -= u32X;
    pstCursor->s32CurColumn -= u32ColumnCount;
    return;

}

void CC708_Cursor_MoveCursorToPosition(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32Column, HI_U32 u32Y, HI_U32 u32Row,
                                      CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32X = u32X;
    pstCursor->s32CurColumn = u32Column;
    pstCursor->s32Y = u32Y;
    pstCursor->s32CurRow = u32Row;
    return;

}

void CC708_Cursor_MoveCursorForwardsOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X,
                                            CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32X += u32X;
    return ;
}

void CC708_Cursor_MoveCursorForwardsOffScreen(CC708_Cursor_S * pstCursor, HI_U32 u32ColumnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32CurColumn += u32ColumnCount;
    return;
}

void CC708_Cursor_MoveCursorBackwardsOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X,
                                             CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32X -= u32X;
    return;
}

void CC708_Cursor_MoveCursorBackwardsOffScreen(CC708_Cursor_S * pstCursor, HI_U32 u32ColumnCount,
                                CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32CurColumn -= u32ColumnCount;
    return;
}

void CC708_Cursor_MoveCursorOnScreen(CC708_Cursor_S * pstCursor, HI_U32 u32X, HI_U32 u32Y,
                                    CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
     pstCursor->s32X = u32X;
     pstCursor->s32Y = u32Y;
     return;
}

void CC708_Cursor_MoveCursorOffScreen(CC708_Cursor_S * pstCursor, HI_U32 u32Row, HI_U32 u32Column,
                                     CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    pstCursor->s32CurColumn = u32Column;
    pstCursor->s32CurRow = u32Row;
    return;
}

HI_S32 CC708_Cursor_GetCurrentColumn(CC708_Cursor_S * pstCursor)
{
        return pstCursor->s32CurColumn;
}


HI_S32 CC708_Cursor_GetCurrentRow(CC708_Cursor_S * pstCursor)
{
        return pstCursor->s32CurRow;
}

HI_S32 CC708_Cursor_GetCurrentXPosition(CC708_Cursor_S * pstCursor,
                                    CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
        return pstCursor->s32X;
}

HI_S32 CC708_Cursor_GetCurrentYPosition(CC708_Cursor_S * pstCursor,
                                    CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
        return pstCursor->s32Y;
}
/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/
