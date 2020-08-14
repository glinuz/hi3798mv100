
/*******************************************************************************
*                       Include files
*******************************************************************************/
#include "hi_cc708_def.h"
#include "cc708_pen.h"

#include "hi_type.h"
#include "cc_debug.h"


HI_U8 CC708_Pen_SetPenSize(CC708_PenDef_S *pstPen, HI_U8 u8PenSize, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->enFontSize != u8PenSize )
    {
        pstPen->enFontSize = (HI_UNF_CC_FONTSIZE_E)u8PenSize;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetFontStyle(CC708_PenDef_S *pstPen, HI_U8 u8FontStyle, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->enFontName != u8FontStyle )
    {
        pstPen->enFontName = (HI_UNF_CC_FONTNAME_E)u8FontStyle;
        u8PenChanged = 1;
    }
    
    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextTag(CC708_PenDef_S *pstPen, HI_U8 u8TextTag, HI_U8 u8IsVisible)
{
    pstPen->enTextTag = (CC708_TextTag_E)u8TextTag;

    return 1;
}

HI_U8 CC708_Pen_SetTextOffset(CC708_PenDef_S *pstPen, HI_U8 u8TextOffset, HI_U8 u8IsVisible)
{
    pstPen->enTextOffset = (CC708_TextOffset_E)u8TextOffset;

    return 0;
}

HI_U8 CC708_Pen_SetTextItalics(CC708_PenDef_S *pstPen, HI_U8 u8IsItalic, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->u8IsItalic != u8IsItalic )
    {
        pstPen->u8IsItalic = u8IsItalic;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextUnderline(CC708_PenDef_S *pstPen, HI_U8 u8IsUnderline, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->u8IsUnderline != u8IsUnderline )
    {
        pstPen->u8IsUnderline = u8IsUnderline;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextEdgeType(CC708_PenDef_S *pstPen, HI_U8 u8EdgeType, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->enEdgeType != u8EdgeType )
    {
        pstPen->enEdgeType = (HI_UNF_CC_EdgeType_E)u8EdgeType;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextEdgeColor(CC708_PenDef_S *pstPen, HI_U32 u32EdgeColor, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->u32EdgeColor != u32EdgeColor )
    {
        pstPen->u32EdgeColor = u32EdgeColor;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextFgColor(CC708_PenDef_S *pstPen, HI_S32 s32FGColor, HI_S32 s32FGOpacity, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    if ( pstPen->u32FGColor != s32FGColor )
    {
        pstPen->u32FGColor = s32FGColor;
        u8PenChanged = 1;
    }

    /* text opacity */
    if ( pstPen->u8FGOpacity != s32FGOpacity )
    {
        pstPen->u8FGOpacity = s32FGOpacity;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}

HI_U8 CC708_Pen_SetTextBgColor(CC708_PenDef_S *pstPen, HI_S32 s32BGColor, HI_S32 s32BGOpacity, HI_U8 u8IsVisible)
{
    HI_U8 u8PenChanged = 0;

    /* background color */
    if ( pstPen->u32BGColor != s32BGColor )
    {
        pstPen->u32BGColor = s32BGColor;
        u8PenChanged = 1;
    }

    /* background opacity */

    if ( pstPen->u8BGOpacity != s32BGOpacity )
    {
        pstPen->u8BGOpacity = s32BGOpacity;
        u8PenChanged = 1;
    }

    return u8PenChanged;
}
/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/
