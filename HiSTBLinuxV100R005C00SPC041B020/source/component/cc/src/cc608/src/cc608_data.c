/*******************************************************************************
*                       Include files
*******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include "hi_cc608_def.h"
#include "cc608_data.h"
#include "cc608_obj.h"
#include "cc608_dec.h"
#include "cc_debug.h"
#include "cc_timer.h"

#include "ccdisp_api.h"

/*******************************************************************************
*                       Static Function Prototype
*******************************************************************************/
static void _CC608_DATA_ClrCell(HI_U8 module_id, HI_U8 row,HI_U8 column, HI_U8 len);
static inline void _CC608_DATA_CalcStaPosition(HI_U8 module_id);
static inline void _CC608_DATA_ResetBuffer(HI_U8 module_id,CC608_BUFFER_TYPE_E buf_type);
static HI_U16 _CC608_DATA_CalcCurrentCursor(HI_U8 module_id,HI_U8 row,HI_U8 column,CC608_BUFFER_TYPE_E buf_type);
static HI_S32 _CC608_DATA_PaintCaption(HI_U8 module_id, HI_U16 *buf, HI_U8 len,
                         HI_U8 is_overflow, HI_BOOL bClear, HI_BOOL bHasLeadingSpace, HI_BOOL bTailingSpace);
static HI_S32 _CC608_DATA_PopCaption(HI_U8 module_id);
static HI_S32 _CC608_DATA_ClearRows(HI_U8 module_id, HI_U8 top, HI_U8 bottom);
static void _CC608_DATA_ResetText(HI_U8 module_id);
static void _CC608_DATA_EnterTextMode(HI_U8 module_id);
static void _CC608_DATA_EnterCaptionMode(HI_U8 module_id);
static inline HI_S32 _CC608_DATA_StoreCaptionToBuffer(HI_U8 module_id,HI_U16 *caption,HI_U16 len,CC608_BUFFER_TYPE_E buf_type);
static inline void _CC608_DATA_MoveToNewBaserow(HI_U8 module_id,HI_U8 old_baserow,HI_U8 old_rows);
static void _CC608_DATA_RedrawRowFromColumn(HI_U8 module_id,HI_U8 row,HI_U8 fromcolumn);
static HI_BOOL _CC608_DATA_IsCharDisplayable(HI_U16 caption);
static void _CC608_DATA_PaintLeadingTailingSpace(HI_U8 module_id, HI_U16 x, HI_U16 y,HI_U16 width, HI_U16 height);
static void _CC608_DATA_SetUnderline(HI_U8 module_id, HI_BOOL bUnderline);
/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/
/*****************************************************************************
* function name: CC608_DATA_ClearSTA(HI_U8 module_id);                   *
* descripton   : clear safe title area with window-color(transparent)       *
* arguments    :                                                            *
*                                                                           *
****************************************************************************/
HI_S32 CC608_DATA_ClearSTA(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_UNF_CC_RECT_S rect;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    rect.x = pstObject->stScreen.stStaScreen.x-1;
    rect.y = pstObject->stScreen.stStaScreen.y-1;
    rect.width = pstObject->stScreen.stStaScreen.width+2;
    rect.height = pstObject->stScreen.stStaScreen.height+2;

    if( pstObject->enVbiMode == CC608_MODE_TEXT)
    {
       (HI_VOID)CCDISP_Screen_SetColor(HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);
    }

    (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT,rect,1);

    pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);

    return HI_SUCCESS;
}

/*****************************************************************************
 * description  : Initialize the Data Layer, called by upper layer           *
 *                                                                           *
 *****************************************************************************/
void CC608_DATA_Create(HI_U8 module_id)
{
    CC608_OBJ_Create(module_id);
}

/*****************************************************************************
* function name: CC608_DATA_Reset(HI_U8 module_id);                      *
* description  : Reset the 608 caption, when change channel or timeout Ex   *
*                                                                           *
*****************************************************************************/
void CC608_DATA_Reset(HI_U8 module_id)
{
     CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

     if( HI_NULL == pstObject )
     {
         HI_ERR_CC("Invalid module id:%d\n",module_id);
         return;
     }

     OSD_TRACE("CC608_DATA_Reset!\n");
     pstObject->enVbiMode = CC608_MODE_NOCAPTION;
     pstObject->u8IsModeModified = 1;
     pstObject->enDispStyle = CC608_DISP_ROLLUP;
     pstObject->u8IsXDSStarted = 0;
     pstObject->u8IsTextShown = 1;
     pstObject->stScreen.u8CurColumn = 0;
     pstObject->stScreen.u8CurRow    = 14;
     pstObject->stScreen.u8BaseRow   = 14;
     pstObject->stScreen.u8TopRow    = 13;
     pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);

     pstObject->u8IsCaptureText = HI_FALSE;
     pstObject->stPenAttr.bIsFlash = HI_FALSE;
     pstObject->stPenAttr.bIsItalic = HI_FALSE;
     pstObject->stPenAttr.bIsUnderline = HI_FALSE;
     pstObject->stPenAttr.u32FgColor        = HI_UNF_CC_COLOR_WHITE;
     pstObject->stPenAttr.enFgOpacity      = HI_UNF_CC_OPACITY_SOLID;
     pstObject->stPenAttr.u32BgColor        = HI_UNF_CC_COLOR_BLACK;
     pstObject->stPenAttr.enBgOpacity      = HI_UNF_CC_OPACITY_SOLID;

     (HI_VOID)CCDISP_Text_SetBGColor(pstObject->enWindowId,pstObject->stPenAttr.u32BgColor,pstObject->stPenAttr.enBgOpacity);
     (HI_VOID)CCDISP_Text_SetFGColor(pstObject->enWindowId,pstObject->stPenAttr.u32FgColor,pstObject->stPenAttr.enFgOpacity);

     (HI_VOID)CCDISP_Screen_SetColor(HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);
     (HI_VOID)CCDISP_Window_SetColor(pstObject->enWindowId,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID);

     (HI_VOID)CCDISP_Text_SetFontName(pstObject->enWindowId,HI_UNF_CC_FN_MONOSPACED);
     (HI_VOID)CCDISP_Text_SetFontSize(pstObject->enWindowId,HI_UNF_CC_FONTSIZE_STANDARD);

     (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);
     (HI_VOID)CCDISP_Text_SetFlash(pstObject->enWindowId,pstObject->stPenAttr.bIsFlash);

     _CC608_DATA_ResetBuffer(module_id,CC608_BUFFER_ON_SCREEN);
     _CC608_DATA_ResetBuffer(module_id,CC608_BUFFER_OFF_SCREEN);

     OSD_TRACE("CC608_DATA_ClearSTA to call clear sta!\n\n");
     (HI_VOID)CC608_DATA_ClearSTA(module_id);
     CC608_DATA_SetDefaultRowColumn(module_id,CC608_MODE_NOCAPTION,CC608_DISP_PAINTON);
     return;
}


/*****************************************************************************
* function name: CC608_DATA_SetBgColor(HI_U8 module_id, HI_U8 bg_color, HI_UNF_CC_OPACITY_E bg_opct)*
* descripton   : Set caption background color and opacity                   *
* arguments    :                                                            *
*               bg_color: background color, bg_opacity:opacity              *
****************************************************************************/
void CC608_DATA_SetBgColor(HI_U8 module_id, HI_U32 u32BgColor, HI_UNF_CC_OPACITY_E enBgOpct)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;
    HI_U8 u8Row;
    HI_U32 u32Color;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if( pstObject->stUserPenAttr.u32BgColor == HI_UNF_CC_COLOR_DEFAULT )
    {
        pstObject->stPenAttr.u32BgColor = u32BgColor;
    }
    else
    {
        pstObject->stPenAttr.u32BgColor = pstObject->stUserPenAttr.u32BgColor;
    }

    if( pstObject->stUserPenAttr.enBgOpacity == HI_UNF_CC_OPACITY_DEFAULT )
    {
        pstObject->stPenAttr.enBgOpacity = enBgOpct;
    }
    else
    {
        pstObject->stPenAttr.enBgOpacity = pstObject->stUserPenAttr.enBgOpacity;
    }

    u32Color = pstObject->stPenAttr.u32BgColor;
    enOpacity = pstObject->stPenAttr.enBgOpacity;

    (HI_VOID)CCDISP_Text_SetBGColor(pstObject->enWindowId,u32Color,enOpacity);

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            pstBuffer = &(pstObject->stOffBuffer);
        }
        else
        {
            pstBuffer = &(pstObject->stOnBuffer);
        }
        u8Row = (HI_U8)pstBuffer->u8CurRow;

        pstBuffer->stLastPenAttr[u8Row].u32BgColor = u32Color;
        pstBuffer->stLastPenAttr[u8Row].enBgOpacity = enOpacity;
    }

    return;
}


/*****************************************************************************
* function name: void CC608_DATA_SetTextAttr(HI_U8 module_id,  *
*                    HI_U8 txt_color, HI_U8 is_under) *
* descripton   : Set caption attributes                                     *
* arguments    :                                                            *
*                txt_color: caption color, is_italic:opacity             *
****************************************************************************/
void CC608_DATA_SetTextAttr(HI_U8 module_id, HI_U32 u32TxtColor, HI_U8 u8IsUnderline)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;
    HI_U8 u8Row;
    HI_U32 u32Color;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if( pstObject->stUserPenAttr.u32FgColor == HI_UNF_CC_COLOR_DEFAULT )
    {
        if(u32TxtColor != 0)
        {
            pstObject->stPenAttr.u32FgColor = u32TxtColor;
        }
        else
        {
            pstObject->stPenAttr.u32FgColor = HI_UNF_CC_COLOR_WHITE;
        }
    }
    else
    {
        pstObject->stPenAttr.u32FgColor = pstObject->stUserPenAttr.u32FgColor;
    }

    if( pstObject->stUserPenAttr.bIsUnderline == HI_FALSE )
    {
        pstObject->stPenAttr.bIsUnderline = u8IsUnderline ? HI_TRUE : HI_FALSE;
    }
    else if( pstObject->stUserPenAttr.bIsUnderline == HI_TRUE )
    {
        pstObject->stPenAttr.bIsUnderline = HI_TRUE;
    }
    else
    {
        pstObject->stPenAttr.bIsUnderline = HI_FALSE;
    }

    u32Color = pstObject->stPenAttr.u32FgColor;

    if( pstObject->stUserPenAttr.enFgOpacity == HI_UNF_CC_OPACITY_DEFAULT)
    {
        pstObject->stPenAttr.enFgOpacity = HI_UNF_CC_OPACITY_SOLID;
    }
    else
    {
        pstObject->stPenAttr.enFgOpacity = pstObject->stUserPenAttr.enFgOpacity;
    }

    enOpacity = pstObject->stPenAttr.enFgOpacity;

    OSD_TRACE("CC608_DATA_SetTextAttr:u32Color = 0x%x,enOpacity= %s,underline = %d\n",u32Color,DBG_OPA_OUT[enOpacity],pstObject->stPenAttr.bIsUnderline);

    (HI_VOID)CCDISP_Text_SetFGColor(pstObject->enWindowId,u32Color,enOpacity);
    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            pstBuffer = &(pstObject->stOffBuffer);
        }
        else
        {
            pstBuffer = &(pstObject->stOnBuffer);
        }
        u8Row = (HI_U8)pstBuffer->u8CurRow;
        if( u8Row >= CC608ROWS )
        {
            HI_ERR_CC("u8Row overflow!\n");
            return;
        }
        pstBuffer->stLastPenAttr[u8Row].u32FgColor = pstObject->stPenAttr.u32FgColor;
        pstBuffer->stLastPenAttr[u8Row].enFgOpacity = pstObject->stPenAttr.enFgOpacity;

        pstBuffer->stLastPenAttr[u8Row].bIsUnderline = pstObject->stPenAttr.bIsUnderline;
    }

    return;
}

void CC608_DATA_GetTextAttr(HI_U8 module_id, HI_U32 *pu32TxtColor, HI_U8 *pu8IsUnderline)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    if( pu32TxtColor != HI_NULL )
    {
        *pu32TxtColor = pstObject->stPenAttr.u32FgColor;
    }
    if( pu8IsUnderline != HI_NULL )
    {
        *pu8IsUnderline = pstObject->stPenAttr.bIsUnderline;
    }
    return;
}

void CC608_DATA_SetTextItalic(HI_U8 module_id, HI_U8 u8IsItalic)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;
    HI_U8 u8Row;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if( pstObject->stUserPenAttr.bIsItalic == HI_FALSE )
    {
        pstObject->stPenAttr.bIsItalic = u8IsItalic ? HI_TRUE : HI_FALSE;
    }
    else if( pstObject->stUserPenAttr.bIsItalic == HI_TRUE )
    {
        pstObject->stPenAttr.bIsItalic = HI_TRUE;
    }
    else
    {
        pstObject->stPenAttr.bIsItalic = HI_FALSE;
    }

    (HI_VOID)CCDISP_Text_SetItalic(pstObject->enWindowId,pstObject->stPenAttr.bIsItalic);

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            pstBuffer = &(pstObject->stOffBuffer);
        }
        else
        {
            pstBuffer = &(pstObject->stOnBuffer);
        }
        u8Row = (HI_U8)pstBuffer->u8CurRow;

        pstBuffer->stLastPenAttr[u8Row].bIsItalic = pstObject->stPenAttr.bIsItalic;
    }
    return;
}


/**************************************************************************
* function name: CC608_DATA_SetDefaultAttr(HI_U8 module_id)           *
* description:   Set cc608 default attributes                            *
**************************************************************************/
void CC608_DATA_SetDefaultAttr(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    OSD_TRACE("CC608_DATA_SetDefaultAttr\n");

    pstObject->stPenAttr.u32BgColor = HI_UNF_CC_COLOR_BLACK;
    pstObject->stPenAttr.enBgOpacity = HI_UNF_CC_OPACITY_SOLID;
    pstObject->stPenAttr.u32FgColor = HI_UNF_CC_COLOR_WHITE;
    pstObject->stPenAttr.enFgOpacity = HI_UNF_CC_OPACITY_SOLID;
    (HI_VOID)CCDISP_Text_SetBGColor(pstObject->enWindowId, pstObject->stPenAttr.u32BgColor, pstObject->stPenAttr.enBgOpacity);
    (HI_VOID)CCDISP_Text_SetFGColor(pstObject->enWindowId, pstObject->stPenAttr.u32FgColor, pstObject->stPenAttr.enFgOpacity);
    (HI_VOID)CCDISP_Text_SetFontName(pstObject->enWindowId, HI_UNF_CC_FN_DEFAULT);

    pstObject->stPenAttr.bIsItalic = HI_FALSE;
    pstObject->stPenAttr.bIsUnderline = HI_FALSE;
    pstObject->stPenAttr.bIsFlash = HI_FALSE;
    (HI_VOID)CCDISP_Text_SetItalic(pstObject->enWindowId,pstObject->stPenAttr.bIsItalic);
    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);
    (HI_VOID)CCDISP_Text_SetFlash(pstObject->enWindowId, pstObject->stPenAttr.bIsFlash);

    return;
}


/**************************************************************************
* function name: CC608_DATA_Caption(...)                                      *
* description  : Receipt of normal ascii characters                      *
*                if now is pop-on style store the caption to off-buffer  *
*                otherwise paint the caption instantly                   *
**************************************************************************/
void CC608_DATA_Caption(HI_U8 module_id, HI_U16 *pu16CaptionData, HI_U8 u8Len)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row,column,overflow_num;
    HI_U8 ret_val = 0;
    HI_BOOL bLeadingSpace = HI_FALSE;
    HI_BOOL bClear = HI_TRUE;
    HI_BOOL bTailingSpace = HI_TRUE;

    if( HI_NULL == pstObject || pu16CaptionData == HI_NULL || u8Len == 0 )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if( pstObject->enCurMode == CC608_MODE_CAPTION )
    {
        /*pop-on style,store the caption in buffer*/
        if (pstObject->enDispStyle == CC608_DISP_POPON)
        {
            (HI_VOID)CC608_TimeoutErase_Start(module_id,CC608_AUTOERASE_TIMEOUT);
            (HI_VOID)_CC608_DATA_StoreCaptionToBuffer(module_id,pu16CaptionData,u8Len,CC608_BUFFER_OFF_SCREEN);
        }
        else
        {
            (HI_VOID)CC608_TimeoutErase_Start(module_id,CC608_AUTOERASE_TIMEOUT);
            row = (HI_U8)pstObject->stOnBuffer.u8CurRow;
            column = (HI_U8)pstObject->stOnBuffer.u8CurColumn;

            /*draw leading space*/
            if(_CC608_DATA_IsCharDisplayable(*pu16CaptionData))
            {
                if(column == 0)
                {
                    bLeadingSpace = HI_TRUE;
                }
                else if(pstObject->stOnBuffer.stCaption[row][column - 1].u16Text == 0)
                {
                    bLeadingSpace = HI_TRUE;
                }
                else if(!_CC608_DATA_IsCharDisplayable(pstObject->stOnBuffer.stCaption[row][column - 1].u16Text))
                {
                    bLeadingSpace = HI_TRUE;
                }
            }

            if(bLeadingSpace)
            {
                HI_U16 x,y,width,height;

                x = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.u16CurTextWidth - pstObject->au8CharWidth[0];
                y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
                width = pstObject->au8CharWidth[0];
                height = pstObject->stScreen.u8RowHeight;

                _CC608_DATA_PaintLeadingTailingSpace(module_id,x,y,width,height);
            }

            if((column > 0) &&
               (pstObject->stOnBuffer.stCaption[row][column - 1].u8IsChar != 0) &&
               (pstObject->stOnBuffer.stCaption[row][column - 1].u16Text != 0x1111))
            {
                bClear = HI_FALSE;
            }

            if((column < CC608COLUMNS) &&
               (pstObject->stOnBuffer.stCaption[row][column + 1].u8IsChar != 0) &&
               (pstObject->stOnBuffer.stCaption[row][column + 1].u16Text != 0x1111))
            {
                bTailingSpace = HI_FALSE;
            }

            ret_val = _CC608_DATA_StoreCaptionToBuffer(module_id,pu16CaptionData,u8Len,CC608_BUFFER_ON_SCREEN);

            /*draw caption immediatelly*/
            if( pstObject->stScreen.u8CurColumn >= CC608COLUMNS )
            {
                /*if the caption overflow columns*/
                OSD_TRACE("OverFlow: screen.cur_column = %d\n",pstObject->stScreen.u8CurColumn);

                overflow_num = pstObject->stScreen.u8CurColumn - CC608COLUMNS + 1;
                pstObject->stScreen.u8CurColumn = 32;
                pstObject->stOnBuffer.u8CurColumn = 32;
                row = pstObject->stOnBuffer.u8CurRow;
                if(row >= CC608ROWS)
                {
                    row = CC608ROWS-1;
                }
                pstObject->stOnBuffer.u8CharNum[row] -= overflow_num;
            }
            if( ret_val == 0 )
            {
                pstObject->stScreen.u16CurTextWidth -=pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
            }

            (HI_VOID)_CC608_DATA_PaintCaption(module_id, pu16CaptionData, u8Len,!ret_val,bClear,bLeadingSpace,bTailingSpace);
            if( ret_val == 2 && pstObject->enDispStyle == CC608_DISP_PAINTON)
            {
                HI_S32 ccdata;
                OSD_TRACE("PAC or MID-ROW code overwrite!\n");

                /*overwrite pac or mid-row code*/
                _CC608_DATA_RedrawRowFromColumn(module_id,pstObject->stOnBuffer.u8CurRow,pstObject->stOnBuffer.u8CurColumn);

                // recover pen attributes after redraw
                ccdata = pstObject->stOnBuffer.stCaption[row][pstObject->stOnBuffer.u8CurColumn - 1].u16Text;

                if((ccdata >= 0x1120) &&(ccdata <= 0x112f))
                {
                    CC608_DEC_MidRow(module_id,0x11,(HI_U8)((HI_U32)ccdata&0xff));
                }
            }

            if(((ret_val == 4)||(ret_val == 5)) && pstObject->enDispStyle == CC608_DISP_PAINTON)
            {
                HI_S32 pac;
                HI_U32 rowchar;
                HI_U8 is_underline = 0,is_italic = 0;
                HI_U8 txt_color = 0;

                /*overwrite pac or mid-row code*/
                _CC608_DATA_RedrawRowFromColumn(module_id,pstObject->stOnBuffer.u8CurRow,pstObject->stOnBuffer.u8CurColumn);

                // recover pen attributes after redraw
                rowchar = pstObject->stOnBuffer.u8CurColumn - u8Len + (ret_val - 4);
                if (CC608ROWCHARS <= rowchar)
                {
                    rowchar = CC608ROWCHARS - 1;
                }
                pac = pstObject->stOnBuffer.stCaption[row][rowchar].u16PacCode;

                // get underline flag
                is_underline = pac&0x1;

                if (pac&0x10)
                {
                    //indent
                }
                else
                {
                    if(((pac&0x0E) >> 1) == 0x07)
                    {
                        is_italic = 1;
                    }
                    else
                    {
                        txt_color = ((pac&0x0E) >> 1);
                    }
                }
                CC608_DATA_SetTextItalic(module_id,is_italic);
                /*any color italics mid-row code will turn off flash*/
                CC608_DATA_SetTextAttr(module_id,txt_color,is_underline);
            }

        }
    }
    else if ( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        OSD_TRACE("Draw %d text to row = %d, column = %d\n",u8Len,pstObject->stScreen.u8CurRow,pstObject->stScreen.u8CurColumn);
        /*here we begin timer auto erase time-out text*/
        (HI_VOID)CC608_TimeoutErase_Start(module_id,CC608_AUTOERASE_TIMEOUT);

        row = pstObject->stTextBuffer.u8CurRow;
        column = pstObject->stTextBuffer.u8CurColumn;

        /*draw leading space*/
        if(_CC608_DATA_IsCharDisplayable(*pu16CaptionData))
        {
            if(column == 0)
            {
                bLeadingSpace = HI_TRUE;
            }
            else if(pstObject->stTextBuffer.stCaption[row][column - 1].u16Text == 0)
            {
                bLeadingSpace = HI_TRUE;
            }
            else if(!_CC608_DATA_IsCharDisplayable(pstObject->stTextBuffer.stCaption[row][column - 1].u16Text))
            {
                bLeadingSpace = HI_TRUE;
            }
        }

        if(bLeadingSpace)
        {
            HI_U16 x,y,width,height;

            x = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.u16CurTextWidth - pstObject->au8CharWidth[0];
            y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
            width = pstObject->au8CharWidth[0];
            height = pstObject->stScreen.u8RowHeight;
            _CC608_DATA_PaintLeadingTailingSpace(module_id,x,y,width,height);
        }

        if((column > 0) &&
           (pstObject->stTextBuffer.stCaption[row][column - 1].u8IsChar != 0) &&
           (pstObject->stTextBuffer.stCaption[row][column - 1].u16Text != 0x1111))
        {
                bClear = HI_FALSE;
        }

        /*draw text immediatelly*/
        ret_val = _CC608_DATA_StoreCaptionToBuffer(module_id,pu16CaptionData,u8Len,CC608_BUFFER_TEXT_SCREEN);
        if( pstObject->stScreen.u8CurColumn >= CC608COLUMNS )
        {
            OSD_TRACE("CC608_MODE_TEXT OverFlow: screen.cur_column = %d,char_num[row]:%d\n",
                         pstObject->stScreen.u8CurColumn,pstObject->stOnBuffer.u8CharNum[row]);

            /*if the caption overflow columns*/
            overflow_num = pstObject->stScreen.u8CurColumn - CC608COLUMNS + 1;
            pstObject->stScreen.u8CurColumn = 32;
            pstObject->stTextBuffer.u8CurColumn = 32;
            row = pstObject->stTextBuffer.u8CurRow;
            pstObject->stTextBuffer.u8CharNum[row]-= overflow_num;
        }
        if( ret_val == 0 )
        {
            pstObject->stScreen.u16CurTextWidth -=pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
        }

        HI_BOOL bTailing=HI_TRUE;
        HI_S32 tailing_pos=column+u8Len;
        /**
        *   check if tailing pos has HI_U8
        *   the tailing of the text would make the line flash .
        */
        if (tailing_pos<CC608COLUMNS)
        {
            if (pstObject->stTextBuffer.stCaption[row][tailing_pos].u8IsDisplay)
            {
                bTailing=HI_FALSE;
            }
        }
        (HI_VOID)_CC608_DATA_PaintCaption(module_id, pu16CaptionData, u8Len,!ret_val,bClear,HI_FALSE,bTailing);

    }
    return;
}


void CC608_DATA_SetRow(HI_U8 module_id, HI_U8 u8Row)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_S32 rollup_rows,valid_rows;
    HI_U8 u8OldBaseRow;

    if( HI_NULL == pstObject || u8Row >= CC608ROWS )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
       return;
    }

    pstObject->stScreen.u8CurRow  = u8Row;

    if( pstObject->enDispStyle == CC608_DISP_ROLLUP )
    {
        u8OldBaseRow = pstObject->stScreen.u8BaseRow;
        rollup_rows = pstObject->stScreen.u8BaseRow - pstObject->stScreen.u8TopRow + 1;
        if( (u8Row + 1 < rollup_rows) && (rollup_rows <= CC608ROWS))
        {
            /*invalid base_row*/
            valid_rows = rollup_rows - 1;
        }
        else
        {
            valid_rows = u8Row;
        }

        pstObject->stScreen.u8BaseRow = valid_rows;
        pstObject->stOnBuffer.u8CurRow   = valid_rows;
        pstObject->stOnBuffer.u32SeqNum[valid_rows] = pstObject->stOnBuffer.u32CurSeq;
        pstObject->stOnBuffer.u32CurSeq++;
        pstObject->stScreen.u8CurRow  = valid_rows;
        pstObject->stScreen.u8TopRow  = valid_rows + 1 - rollup_rows;

        OSD_TRACE("CC608_DATA_SetRow set rollup mode base_row = %d,top_row = %d\n",pstObject->stScreen.u8BaseRow,pstObject->stScreen.u8TopRow);
        if( u8OldBaseRow != pstObject->stScreen.u8BaseRow )
        {
            /*according to the 608 spec,when base row changed of rollup style
                    the displayed caption should move to the new base row*/
            _CC608_DATA_MoveToNewBaserow(module_id, u8OldBaseRow,rollup_rows);
        }
    }
    else if( pstObject->enDispStyle == CC608_DISP_PAINTON )
    {
        pstObject->stOnBuffer.u8CurRow = u8Row;
        pstObject->stOnBuffer.u32SeqNum[u8Row] = pstObject->stOnBuffer.u32CurSeq;
        pstObject->stOnBuffer.u32CurSeq++;
    }
    else if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        pstObject->stOffBuffer.u8CurRow = pstObject->stScreen.u8CurRow;
    }

    return;
}


/************************************************************************************
* function name:  CC608_DATA_SetColumn(HI_U8 module_id, HI_U8 column                     *
* description  :  Set Indent columns                                                *
*                                                                                   *
************************************************************************************/
void CC608_DATA_SetColumn(HI_U8 module_id, HI_U8 u8Column)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 u8Row = 0;

    if( HI_NULL==pstObject || u8Column >= CC608COLUMNS - 1)
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        pstObject->stTextBuffer.u8CurColumn = u8Column;
        pstObject->stScreen.u8CurColumn  = u8Column;
        pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stTextBuffer.u8CurRow,u8Column,CC608_BUFFER_TEXT_SCREEN);
        return;
    }

    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        pstObject->stOffBuffer.u8CurColumn = u8Column;
        u8Row = pstObject->stOffBuffer.u8CurRow;
        if( pstObject->stOffBuffer.u8CharNum[u8Row] > u8Column )
        {
            pstObject->stOffBuffer.u8CharNum[u8Row] = u8Column;
        }
    }
    else
    {
        pstObject->stOnBuffer.u8CurColumn  = u8Column;
        pstObject->stScreen.u8CurColumn = u8Column;
        u8Row = pstObject->stOnBuffer.u8CurRow;
        if( pstObject->stOnBuffer.u8CharNum[u8Row] > u8Column )
        {
            pstObject->stOnBuffer.u8CharNum[u8Row] = u8Column;
        }
        pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8Column,CC608_BUFFER_ON_SCREEN);
        OSD_TRACE("SetPenlocation to rollup u8Column %d, textwidth = %d \n",u8Column, pstObject->stScreen.u16CurTextWidth);
    }

    /*An character Attributes will remain in effect until changed by
      another control code or until the end of the row is reached*/
    if( u8Column == 0 )
    {
        CC608_DATA_SetBgColor(module_id,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID);
    }

    return;
}

/************************************************************************************
* function name:  void CC608_DATA_SetDefaultRowColumn(HI_U8 module_id,          *
*                                       CC608_MODE_E mode, CC608_DISP_STYLE_E style)          *
* description  :  Set default row and column value accorind to current vbi mode,   *
*                 next vbi mode and next style.                                    *
*                                                                                  *
************************************************************************************/
void CC608_DATA_SetDefaultRowColumn(HI_U8 module_id, CC608_MODE_E enMode, CC608_DISP_STYLE_E enStyle)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    // FCC 47cfr15.119
    // if the reception of data ... is interrupted by data for ...
    // TEXT Mode, the display of caption text will resume from the same cursor position ...
    if((pstObject->enVbiMode == CC608_MODE_TEXT) &&(enMode == CC608_MODE_CAPTION))
    {
        if((enStyle == CC608_DISP_PAINTON) ||(enStyle == CC608_DISP_ROLLUP))
        {
            pstObject->stScreen.u8CurRow = pstObject->stOnBuffer.u8CurRow;
            pstObject->stScreen.u8CurColumn = pstObject->stOnBuffer.u8CurColumn;
        }
        else
        {
            pstObject->stScreen.u8CurRow = pstObject->stOffBuffer.u8CurRow;
            pstObject->stScreen.u8CurColumn = pstObject->stOffBuffer.u8CurColumn;
        }
    }
    else if((pstObject->enVbiMode == CC608_MODE_CAPTION) &&(enMode == CC608_MODE_TEXT))
    {
        pstObject->stScreen.u8CurRow = pstObject->stTextBuffer.u8CurRow;
        pstObject->stScreen.u8CurColumn = pstObject->stTextBuffer.u8CurColumn;
    }
    else if(enMode == CC608_MODE_NOCAPTION)
    {
        pstObject->stScreen.u8CurRow = 0xff;
        pstObject->stScreen.u8CurColumn  = 0xff;

        pstObject->stOnBuffer.u8CurRow = 0xff;
        pstObject->stOnBuffer.u8CurColumn = 0xff;

        pstObject->stOffBuffer.u8CurRow = 0xff;
        pstObject->stOffBuffer.u8CurColumn = 0xff;

        pstObject->stTextBuffer.u8CurRow = 0xff;
        pstObject->stTextBuffer.u8CurColumn = 0xff;
    }

    return;
}

/**
* check if current row and column values are valid
* @param module_id
*
* @return HI_S32
* 0: valid -1: invalid
*/
HI_S32 CC608_DATA_CheckRowColumnValidation(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row,column;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    // in pop-on style, captions are write to off screen buffer
    // so we don't need to check if row and column are initialized
    if(pstObject->enDispStyle == CC608_DISP_POPON)
    {
        return HI_SUCCESS;
    }

    row = pstObject->stScreen.u8CurRow;
    column = pstObject->stScreen.u8CurColumn;

    if((row == 0xff) || (column == 0xff))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/**********************************************************************************
* function name:  CC608_DATA_SetPacCode(HI_U8 module_id, HI_U8 pac    *
* description  :  Set pac code of every row                                      *
*                                                                                *
*********************************************************************************/
void CC608_DATA_SetPacCode(HI_U8 module_id, HI_U8 u8Pac)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row,column;
    CC608_BUFFER_S *pstBuffer;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            pstBuffer = &pstObject->stOffBuffer;
        }
        else
        {
            pstBuffer = &pstObject->stOnBuffer;
        }
        row = pstBuffer->u8CurRow;
        column = pstBuffer->u8CurColumn;

        OSD_TRACE("CC608_DATA_SetPacCode:row %d, column: %d\n",row,column);
        /*only when the new pac column less than the old one,and the old one
          column above zero, it's just can be regarded as valid pac overwrite*/
        if( (((pstBuffer->u8PacCode[row] > 0x51) && (pstBuffer->u8PacCode[row] < 0x60)) ||
             ((pstBuffer->u8PacCode[row] > 0x71) && (pstBuffer->u8PacCode[row] < 0x80))) &&
            pstObject->enDispStyle == CC608_DISP_PAINTON )
        {
            if( (u8Pac >= 0x40 && u8Pac <= 0x51)||(u8Pac >= 0x60 && u8Pac <= 0x71)||
                ((u8Pac&0x0E) < (pstBuffer->u8PacCode[row]&0x0E)) )
            {
                pstBuffer->u8IsPacOverwrite = 1;
                OSD_TRACE(" u8Pac over write = 1!\n");
            }
            else
            {
                pstBuffer->u8IsPacOverwrite = 0;
            }
        }
        else
        {
            pstBuffer->u8IsPacOverwrite = 0;
        }

        if((pstObject->enDispStyle == CC608_DISP_PAINTON) &&
           (pstBuffer->stCaption[row][column].u16PacCode != u8Pac))
        {
            pstObject->bPACOverwrite = HI_TRUE;
        }

        pstBuffer->u8PacCode[row] = u8Pac;
        pstBuffer->stCaption[row][column].u16PacCode = u8Pac;
    }

    return;
}

/*control code command*/
/*****************************************************************************
* function name: CC608_DATA_ResumeCL(void);                                      *
* descripton   : Resume caption loading, enter "pop on" style               *
* arguments    : none                                                       *
****************************************************************************/
HI_S32 CC608_DATA_ResumeCL(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    if( pstObject->enVbiMode != CC608_MODE_CAPTION )
    {
        CC608_DATA_SetDefaultRowColumn(module_id,CC608_MODE_CAPTION,CC608_DISP_POPON);
        _CC608_DATA_EnterCaptionMode(module_id);
    }
    if( pstObject->enDispStyle != CC608_DISP_POPON )
    {
        _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_OFF_SCREEN);
    }
    pstObject->enDispStyle = CC608_DISP_POPON;

    /* Does it need to clear the screen and on-buffer? */
    _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_ON_SCREEN);

    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_BackSpace(void);                                     *
* descripton   : backspace, delete a character at the last position         *
* arguments    : none                                                       *
* return value : HI_SUCCESS,HI_FAILURE                                   *
****************************************************************************/
HI_S32 CC608_DATA_BackSpace(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row,column,cap_len;
    HI_UNF_CC_RECT_S rect;

    OSD_TRACE("Enter BackSpace Command implementation!\n");

    if( HI_NULL == pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    /*here need to add Textmode Implemention
      The text mode should excecute the same with pain-on*/
    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        row  = pstObject->stTextBuffer.u8CurRow;
        cap_len = pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
        if( pstObject->stScreen.u8CurColumn > 0 )
        {
            pstObject->stScreen.u8CurColumn --;
        }
        if( pstObject->stTextBuffer.u8CurColumn > 31 )
        {
            pstObject->stTextBuffer.u8CurColumn = 31;
            pstObject->stScreen.u8CurColumn = 31;
            pstObject->stScreen.u16CurTextWidth -= cap_len;
            if( pstObject->stTextBuffer.u8CharNum[row] > 31 )
            {
                pstObject->stTextBuffer.u8CharNum[row] = 31;
            }
        }
        if( pstObject->stTextBuffer.u8CurColumn > 0 )
        {
            pstObject->stTextBuffer.u8CurColumn--;
        }

        column = pstObject->stTextBuffer.u8CurColumn;
        OSD_TRACE("Backspace from row = %d,column = %d caption = %c\n",row,column,pstObject->stTextBuffer.stCaption[row][column].u16Text);
        if( pstObject->stTextBuffer.u8CharNum[row] > 0 )
        {
            pstObject->stTextBuffer.u8CharNum[row]--;
        }
        pstObject->stTextBuffer.stCaption[row][column].u8IsChar = 0;
        pstObject->stTextBuffer.stCaption[row][column].u8IsDisplay = 0;
        pstObject->stTextBuffer.stCaption[row][column].u16Text = 0;
        /*clear the backspace*/

        pstObject->stScreen.u16CurTextWidth -= cap_len;
        rect.x = pstObject->stScreen.stStaScreen.x + column*cap_len;

        /*fcc tap require the backspace delete column 1-31, not 32*/
        if( pstObject->stTextBuffer.u8CurColumn == 30 )
        {
            rect.width = cap_len * 2 + 2;
        }
        else
        {
            rect.width = cap_len + 2;
        }
        rect.y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
        rect.height = pstObject->stScreen.u8RowHeight;

        HI_U32 u32Color = 0;
        HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
        (HI_VOID)CCDISP_Window_GetColor(pstObject->enWindowId, &u32Color, &enOpacity);
        (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);

        return HI_SUCCESS;
    }

    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        row = pstObject->stOffBuffer.u8CurRow;
        if( pstObject->stOffBuffer.u8CurColumn > 0 )
        {
            /*the backspace command erase column 1-31 not 32*/
            if( pstObject->stOffBuffer.u8CurColumn > 31 )
            {
                pstObject->stOffBuffer.u8CurColumn = 31;
                pstObject->stOffBuffer.stCaption[row][31].u8IsChar = 0;
            }
            pstObject->stOffBuffer.u8CurColumn--;
            row    = pstObject->stOffBuffer.u8CurRow;
            column = pstObject->stOffBuffer.u8CurColumn;
            pstObject->stOffBuffer.stCaption[row][column].u8IsChar = 0;
            if( pstObject->stOffBuffer.u8CharNum[row] > 0 )
            {
                pstObject->stOffBuffer.u8CharNum[row]--;
            }
        }
        else
        {
            /*current column = 0, no backspace*/
            HI_INFO_CC("current column = 0, no backspace \n");
        }
        return HI_SUCCESS;
    }

    if(pstObject->stScreen.u8CurColumn > 0 )
    {
        HI_U8 i;
        HI_BOOL bFirstChar = HI_TRUE;
        row   = pstObject->stOnBuffer.u8CurRow;
        cap_len = pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
        pstObject->stScreen.u8CurColumn --;
        if( pstObject->stOnBuffer.u8CurColumn > 31 )
        {
            pstObject->stOnBuffer.u8CurColumn = 31;
            pstObject->stScreen.u8CurColumn = 31;
            pstObject->stScreen.u16CurTextWidth -= cap_len;
            if( pstObject->stOnBuffer.u8CharNum[row] > 31 )
            {
                pstObject->stOnBuffer.u8CharNum[row] = 31;
            }
        }
        else if( pstObject->stOnBuffer.u8CurColumn > 0 )
        {
            pstObject->stOnBuffer.u8CurColumn --;
        }

        column = pstObject->stOnBuffer.u8CurColumn;
        OSD_TRACE("Backspace from row = %d,column = %d caption = %c\n",row,column,pstObject->stOnBuffer.stCaption[row][column].u16Text);
        if( pstObject->stOnBuffer.u8CharNum[row] > 0 )
        {
            pstObject->stOnBuffer.u8CharNum[row]--;
        }
        pstObject->stOnBuffer.stCaption[row][column].u8IsChar = 0;
        pstObject->stOnBuffer.stCaption[row][column].u8IsDisplay = 0;
        pstObject->stOnBuffer.stCaption[row][column].u16Text = 0;
        pstObject->stScreen.u16CurTextWidth -= cap_len;

        rect.x = pstObject->stScreen.stStaScreen.x + column*cap_len;

        /*add leading space width*/
        rect.x += pstObject->au8CharWidth[0];

        /*fcc tap require the backspace delete column 1-31, not 32*/
        if( pstObject->stOnBuffer.u8CurColumn == 30 )
        {
            rect.width = cap_len * 2 + 2;
        }
        else
        {
            rect.width = cap_len + 4;
        }

        /*add tailing space width*/
        rect.width += pstObject->au8CharWidth[0];
        rect.y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
        rect.height = pstObject->stScreen.u8RowHeight;

        for(i=0; i<column; i++)
        {
            if((pstObject->stOnBuffer.stCaption[row][i].u8IsChar != 0) &&
               (pstObject->stOnBuffer.stCaption[row][i].u16Text != 0x1111))
            {
                bFirstChar = HI_FALSE;
            }
        }

        /*add leading space*/
        if(bFirstChar)
        {
            rect.x -= pstObject->au8CharWidth[0];
            rect.width += pstObject->au8CharWidth[0];
        }

        HI_U32 u32Color = 0;
        HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
        (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
        (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);

        /*draw tailing space again*/
        if(!bFirstChar)
        {
            _CC608_DATA_PaintLeadingTailingSpace(module_id,rect.x,rect.y,pstObject->au8CharWidth[0],pstObject->stScreen.u8RowHeight);
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

/*****************************************************************************
* function name: CC608_DATA_Substitute(void);                                    *
* descripton   : delete the substitute character at the last position       *
*                when received extend chars call this function to substitute*
*                the substitute character of the received extend HI_U8    *
* arguments    : none                                                       *
* return value : 0 on success, -1 at fail                                   *
****************************************************************************/
HI_S32 CC608_DATA_Substitute(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row,column,cap_len;
    HI_UNF_CC_RECT_S rect;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            if( pstObject->stOffBuffer.u8CurColumn >= 1 )
            {
                pstObject->stOffBuffer.u8CurColumn--;
                row    = pstObject->stOffBuffer.u8CurRow;
                column = pstObject->stOffBuffer.u8CurColumn;
                pstObject->stOffBuffer.stCaption[row][column].u8IsChar = 0;
                if( pstObject->stOffBuffer.u8CharNum[row] > 0 )
                {
                    pstObject->stOffBuffer.u8CharNum[row]--;
                }
            }
            else
            {
                /*current column = 0, no backspace*/
                return HI_FAILURE;
            }
            return HI_SUCCESS;
        }

        if(pstObject->stScreen.u8CurColumn > 0 )
        {
            row    = pstObject->stOnBuffer.u8CurRow;
            pstObject->stScreen.u8CurColumn --;

            if( pstObject->stOnBuffer.u8CurColumn > 0 )
            {
                pstObject->stOnBuffer.u8CurColumn--;
            }
            column = pstObject->stOnBuffer.u8CurColumn;

            OSD_TRACE("Backspace from row = %d,column = %d caption = %c\n",row,column,pstObject->stOnBuffer.stCaption[row][column].u16Text);
            if( pstObject->stOnBuffer.u8CharNum[row] > 0 )
            {
                pstObject->stOnBuffer.u8CharNum[row]--;
            }
            pstObject->stOnBuffer.stCaption[row][column].u8IsChar = 0;
            pstObject->stOnBuffer.stCaption[row][column].u8IsDisplay = 0;
            pstObject->stOnBuffer.stCaption[row][column].u16Text = 0;

            /*clear the backspace*/
            cap_len = pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
            pstObject->stScreen.u16CurTextWidth -= cap_len;

            rect.x =  pstObject->stScreen.stStaScreen.x + _CC608_DATA_CalcCurrentCursor(module_id,row,column,CC608_BUFFER_ON_SCREEN);
            rect.width = 2*cap_len + 2;
            rect.y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
            rect.height = pstObject->stScreen.u8RowHeight;

            HI_U32 u32Color = 0;
            HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
            (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
            (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);
        }
        else
        {
            return HI_SUCCESS;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_AlarmOff(void);                                      *
 * descripton   : reserved(formerly Alarm off)  for future use               *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_AlarmOff(HI_U8 module_id)
{
    //TODU
    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_AlarmOn(void);                                       *
 * descripton   : reserved(formerly alarm on) for future use                 *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_AlarmOn(HI_U8 module_id)
{
    //TODU
    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_DeleteToER(void);                                    *
* descripton   : delete to end of rows                                      *
* arguments    : none                                                       *
****************************************************************************/
HI_S32 CC608_DATA_DeleteToER(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_S32 i,row;
    CC608_BUFFER_S *pstBuffer;
    HI_UNF_CC_RECT_S rect;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_SUCCESS;
    }
    if ( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        return HI_SUCCESS;
    }

    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }

    row = pstBuffer->u8CurRow;
    for( i = pstBuffer->u8CurColumn; i < CC608ROWCHARS; i++)
    {
        if( pstBuffer->stCaption[row][i].u8IsChar == 1)
        {
            pstBuffer->stCaption[row][i].u8IsChar = 0;
            pstBuffer->stCaption[row][i].u8IsDisplay = 0;
        }
        pstBuffer->stCaption[row][i].u16Text = 0;
    }
    if( pstBuffer->u8CurColumn == 0 )
    {
        pstBuffer->u32SeqNum[row] = 0;
    }
    /*if it's not pop on style, clear to end of row*/
    if( pstObject->enDispStyle != CC608_DISP_POPON )
    {
        HI_BOOL bFirstChar = HI_TRUE;

        rect.x = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.u16CurTextWidth;
        rect.y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight*pstObject->stScreen.u8CurRow;
        rect.height = pstObject->stScreen.u8RowHeight;
        rect.width = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.stStaScreen.width - pstObject->stScreen.u16CurTextWidth;

        HI_U32 u32Color = 0;
        HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
        (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
        (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);

        for(i=0; i<pstBuffer->u8CurColumn; i++)
        {
            if((pstBuffer->stCaption[row][i].u8IsChar != 0) &&
               (pstBuffer->stCaption[row][i].u16Text != 0x1111))
            {
                bFirstChar = HI_FALSE;
            }
        }

        /*delete leading space*/
        if(bFirstChar)
        {
            OSD_TRACE("Delete leading space!\n");
            rect.x -= pstObject->au8CharWidth[0];
            rect.height = pstObject->stScreen.u8RowHeight;
            rect.width = pstObject->au8CharWidth[0];
            (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_Rollup(HI_U8 rows);                                  *
* descripton   : rollup mode(enter rollup mode)                             *
*                Receipt of a roll-up caption command will cause any pop-on *
*                or paint-on caption to be erased from displayed memory and *
*                non-displayed memory                                       *
* arguments    : rows: number of rows within rollup window                  *
****************************************************************************/
HI_S32 CC608_DATA_Rollup(HI_U8 module_id, HI_U8 u8RollupRows)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 u8OldRollupRows,u8OldTopRow;

    /*does it need to delete all the caption. when receipt of different rollup rows with the old one */
    if( HI_NULL == pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FALSE;
    }
    if(pstObject->enVbiMode != CC608_MODE_CAPTION)
    {
        CC608_DATA_SetDefaultRowColumn(module_id,CC608_MODE_CAPTION,CC608_DISP_ROLLUP);
        _CC608_DATA_EnterCaptionMode(module_id);
    }

    if( pstObject->enDispStyle == CC608_DISP_ROLLUP )
    {
        u8OldRollupRows = pstObject->stScreen.u8BaseRow - pstObject->stScreen.u8TopRow + 1;
        if ( u8OldRollupRows != u8RollupRows )
        {
            u8OldTopRow = pstObject->stScreen.u8TopRow;

            if( pstObject->stScreen.u8BaseRow - u8RollupRows + 1 < 0 )
            {
                pstObject->stScreen.u8TopRow = 0;
                pstObject->stScreen.u8BaseRow = pstObject->stScreen.u8TopRow + u8RollupRows - 1;
                _CC608_DATA_MoveToNewBaserow(module_id,u8OldTopRow + u8OldRollupRows - 1,u8OldRollupRows);
            }
            else
            {
                pstObject->stScreen.u8TopRow = pstObject->stScreen.u8BaseRow - u8RollupRows + 1;
            }

            /*need to update the displayed caption*/
            if( u8RollupRows < u8OldRollupRows )
            {
                /*the new rollup rows less than old one, we must clear the top 1 or two rows*/
                (HI_VOID)_CC608_DATA_ClearRows(module_id,u8OldTopRow, pstObject->stScreen.u8TopRow - 1);
            }
        }
        return HI_SUCCESS;
    }


    /*first time enter rollup mode*/
    (HI_VOID)CC608_DATA_ClearSTA(module_id);
    CC608_DATA_SetDefaultAttr(module_id);
    _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_ON_SCREEN);
    _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_OFF_SCREEN);

    pstObject->enDispStyle = CC608_DISP_ROLLUP;
    pstObject->stScreen.u8BaseRow = 14;
    pstObject->stScreen.u8TopRow  = 14 - u8RollupRows + 1;
    pstObject->stScreen.u8CurRow  = pstObject->stScreen.u8BaseRow;
    pstObject->stScreen.u8CurColumn = 0;
    pstObject->stOnBuffer.u8CurRow = pstObject->stScreen.u8CurRow;
    pstObject->stOnBuffer.u8CurColumn = pstObject->stScreen.u8CurColumn;
    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_FlashOn(HI_U8 is_flash);                     *
* descripton   : caption flash on(off)                                      *
* arguments    :                                                            *
*                is_flash   1:on  0:off                                     *
****************************************************************************/
HI_S32 CC608_DATA_FlashOn(HI_U8 module_id, HI_U8 u8IsFlash)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;
    HI_U8 row;
    HI_BOOL bIsFlash = u8IsFlash ? HI_TRUE : HI_FALSE;
    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    (HI_VOID)CCDISP_Text_SetFlash(pstObject->enWindowId,bIsFlash);
    pstObject->stPenAttr.bIsFlash = bIsFlash;

    if(pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if( pstObject->enDispStyle == CC608_DISP_POPON )
        {
            pstBuffer = &(pstObject->stOffBuffer);
        }
        else
        {
            pstBuffer = &(pstObject->stOnBuffer);
        }
        row = pstBuffer->u8CurRow;

        pstBuffer->stLastPenAttr[row].bIsFlash = bIsFlash;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_ResumeDC(void);                                      *
 * descripton   : Resume Direct Captioning, enter "paint-on" style           *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_ResumeDC(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    if( pstObject->enVbiMode != CC608_MODE_CAPTION )
    {
        CC608_DATA_SetDefaultRowColumn(module_id,CC608_MODE_CAPTION,CC608_DISP_PAINTON);
        _CC608_DATA_EnterCaptionMode(module_id);
    }
    pstObject->enDispStyle = CC608_DISP_PAINTON;
    //for recvied RDC command, decode may reset the on screen cursor position if it is invalid now.
    if (pstObject->stScreen.u8CurRow >= 0xff)
    {
        OSD_TRACE("reset cur_row attribute\n");
        pstObject->stScreen.u8CurRow = 0;
    }

    return HI_SUCCESS;
}


/*text mode functions*/

/*****************************************************************************
 * function name: CC608_DATA_TextRestart(void);                                   *
 * descripton   : text mode restart                                          *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_TextRestart(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    if( pstObject->enVbiMode != CC608_MODE_TEXT)
    {
        _CC608_DATA_EnterTextMode(module_id);
    }
    _CC608_DATA_ResetText(module_id);
    CC608_DATA_SetDefaultAttr(module_id);
    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_TextDisplay(void);                                   *
 * descripton   : text mode, Resume text display                             *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_TextDisplay(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    if(pstObject->enVbiMode != CC608_MODE_TEXT)
    {
        _CC608_DATA_EnterTextMode(module_id);
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_EraseDM(void);                                       *
* descripton   : Erase displayed memory(on screen)                          *
*        In the case of received EDM or ENM command, that command shall be  *
*        acted upon as appropriate for caption processing without terminate *
*        the TextMode data stream.   608spec P26 7.7 Other Interruptions    *
* arguments    : none                                                       *
****************************************************************************/
HI_S32 CC608_DATA_EraseDM(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    /*Clear the caption on screen, clear the on-screen buffer, reset coordinate*/

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    /*clear caption on screen*/

    if( pstObject->enCurMode == CC608_MODE_CAPTION && pstObject->u8IsTextShown == 1)
    {
        (HI_VOID)CC608_DATA_ClearSTA(module_id);
        _CC608_DATA_ResetBuffer( 0, CC608_BUFFER_ON_SCREEN );
        pstObject->u8IsTextShown = 0;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* function name: CC608_DATA_CarriageReturn(void);                                *
* descripton   : change line                                                *
*                carriage returns have no effect on cursor location during  *
*                caption loading & direct captioning.                       *
*                so it just affect the roll-up caption                      *
* arguments    : none                                                       *
****************************************************************************/
HI_S32 CC608_DATA_CarriageReturn(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_S32 x,top_y, btm_y;
    CC608_BUFFER_S *pstBuffer;
    HI_U8 row;

    if( HI_NULL == pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    pstObject->u8CmdCount = 0;
    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        if( pstObject->stTextBuffer.u8CurRow < pstObject->stScreen.u8BaseRow )
        {
            pstObject->stTextBuffer.u8CurRow ++;
            pstObject->stTextBuffer.u8CurColumn = 0;
            pstObject->stScreen.u8CurRow ++;
            pstObject->stScreen.u8CurColumn = 0;
            pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);
            OSD_TRACE("TEXT MODE CR\n");

            CC608_DATA_SetDefaultAttr(module_id);
            return HI_SUCCESS;
        }
    }

    if( pstObject->enDispStyle != CC608_DISP_ROLLUP )
    {
        return HI_SUCCESS;
    }

    /*excecute the rollup action*/
    x = pstObject->stScreen.stStaScreen.x;
    /*row number from 1-15, so the row top position is row - 1*/
    top_y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * (pstObject->stScreen.u8TopRow);
    btm_y = pstObject->stScreen.stStaScreen.y +pstObject->stScreen.u8RowHeight * (pstObject->stScreen.u8BaseRow + 1);

    OSD_TRACE("CC608_DATA_CarriageReturn: top_row = %d, base_row = %d\n",pstObject->stScreen.u8TopRow, pstObject->stScreen.u8BaseRow);
    OSD_TRACE("Rollup Text:x = %d, width = %d,top_y = %d, bottom_y = %d,height = %d\n",x, pstObject->stScreen.stStaScreen.width,top_y, btm_y, pstObject->stScreen.u8RowHeight);

    (HI_VOID)CCDISP_Window_Rollup(pstObject->enWindowId,x,pstObject->stScreen.stStaScreen.width,top_y,
                                     btm_y,pstObject->stScreen.u8RowHeight,CCDISP_SPEED6);

    pstObject->stScreen.u8CurColumn    = 0;
    pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);;

    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        pstBuffer = &(pstObject->stTextBuffer);
        OSD_TRACE("TEXT MODE last row clear!\n");
    }
    else
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }

    /*ROLL UP,move the row text to the contigues higher row*/
    for( row = pstObject->stScreen.u8TopRow; row < pstObject->stScreen.u8BaseRow; row++)
    {
        pstBuffer->u8CharNum[row] = pstBuffer->u8CharNum[row+1];
        pstBuffer->u8PacCode[row] = pstBuffer->u8PacCode[row+1];
        pstBuffer->stLastPenAttr[row] = pstBuffer->stLastPenAttr[row+1];
        memcpy(pstBuffer->stCaption[row],pstBuffer->stCaption[row+1],sizeof(CC608_CAPTION_S)*CC608ROWCHARS);
#if 0
        /*print the row data */
        {
            for( idd = 0;idd < pstBuffer->u8CharNum[row];idd++)
            {
                OSD_TRACE("%c",pstBuffer->stCaption[row][idd].u16Text);
            }
            OSD_TRACE("\n");
        }
#endif
    }

    /*set the base row attribute*/
    memset(pstBuffer->stCaption[pstObject->stScreen.u8BaseRow], 0, sizeof(CC608_CAPTION_S)*CC608ROWCHARS);
    pstBuffer->u8CharNum[pstObject->stScreen.u8BaseRow] = 0;
    pstBuffer->u8PacCode[pstObject->stScreen.u8BaseRow] = 0;
    memset(&pstBuffer->stLastPenAttr[pstObject->stScreen.u8BaseRow],0,sizeof(CC608_PENATTR_S));
    CC608_DATA_SetDefaultAttr(module_id);
    pstBuffer->u8CurColumn = 0;

    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_EraseNM(void);                                       *
 * descripton   : Erase non-display memory(reset offset screen mem)          *
 *                                                                           *
 *        In the case of received EDM or ENM command, that command shall be  *
 *        acted upon as appropriate for caption processing without terminate *
 *        the TextMode data stream.   608spec P26 7.7 Other Interruptions    *
 *                                                                           *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_EraseNM(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    pstObject->u8CmdCount = 0;
    _CC608_DATA_ResetBuffer(0, CC608_BUFFER_OFF_SCREEN);
    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_FlipMemory(void);                                    *
 * descripton   : End of Caption                                             *
 *                make a displayed caption to become non-displayed(and vice  *
 *                versa) without being erased from memory.                   *
 *                if type == pop-on, display(show) the caption on off-buffer *
 *                if type != pop-on, hide the caption on screen              *
 * arguments    : none                                                       *
 ****************************************************************************/
HI_S32 CC608_DATA_FlipMemory(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S tmp_buf;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    pstObject->u8CmdCount = 0;

    if(pstObject->enVbiMode != CC608_MODE_CAPTION)
    {
        _CC608_DATA_EnterCaptionMode(module_id);
    }

    if( pstObject->enDispStyle != CC608_DISP_POPON )
    {
        pstObject->enDispStyle = CC608_DISP_POPON;
        if(pstObject->stOffBuffer.u8CurRow >= 0xff || pstObject->stOffBuffer.u8CurColumn >= 0xff)
        {
            pstObject->stOffBuffer.u8CurRow = 0;
            pstObject->stOffBuffer.u8CurColumn = 0;
        }
    }

    memcpy(&tmp_buf, &(pstObject->stOnBuffer), sizeof(CC608_BUFFER_S));
    (HI_VOID)CC608_DATA_ClearSTA(module_id);

    /*here we begin timer auto erase time-out text*/
    (HI_VOID)CC608_TimeoutErase_Start(module_id,CC608_AUTOERASE_TIMEOUT);

    (HI_VOID)_CC608_DATA_PopCaption(module_id);

    memcpy(&(pstObject->stOnBuffer), &(pstObject->stOffBuffer), sizeof(CC608_BUFFER_S));
    memcpy(&(pstObject->stOffBuffer), &tmp_buf, sizeof(CC608_BUFFER_S));

    pstObject->stScreen.u8CurRow = pstObject->stOffBuffer.u8CurRow;
    pstObject->stScreen.u8CurColumn = pstObject->stOffBuffer.u8CurColumn;

    return HI_SUCCESS;
}

/*****************************************************************************
 * function name: CC608_DATA_Tab(HI_U8 columns);                          *
 * descripton   : tab offset columns                                         *
 * arguments    : columns                                                    *
 *                the number of columns that will tab(1,2,3)                 *
 ****************************************************************************/
HI_S32 CC608_DATA_Tab(HI_U8 module_id, HI_U8 u8Columns)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        if( pstObject->stTextBuffer.u8CurColumn + u8Columns >= CC608COLUMNS )
        {
            return HI_FAILURE;
        }
        if( pstObject->stTextBuffer.u8CurColumn + u8Columns < CC608COLUMNS - 1 )
        {
            pstObject->stTextBuffer.u8CurColumn += u8Columns;
            pstObject->stScreen.u8CurColumn += u8Columns;
        }
        else
        {
            pstObject->stTextBuffer.u8CurColumn = 31;
            pstObject->stScreen.u8CurColumn = 31;
        }
        pstObject->stScreen.u16CurTextWidth =
            _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stTextBuffer.u8CurRow,pstObject->stTextBuffer.u8CurColumn,CC608_BUFFER_TEXT_SCREEN);

        return HI_SUCCESS;
    }
    /*need to add Textmode implementation*/

    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        if( pstObject->stOffBuffer.u8CurColumn + u8Columns < CC608COLUMNS - 1 )
        {
            pstObject->stOffBuffer.u8CurColumn += u8Columns;
        }
        else
        {
            pstObject->stOffBuffer.u8CurColumn = 31;
        }
    }
    else
    {
        if( pstObject->stOnBuffer.u8CurColumn + u8Columns < CC608COLUMNS - 1)
        {
            /*overflow*/
            pstObject->stOnBuffer.u8CurColumn += u8Columns;
            pstObject->stScreen.u8CurColumn += u8Columns;
        }
        else
        {
            pstObject->stOnBuffer.u8CurColumn = 31;
            pstObject->stScreen.u8CurColumn = 31;
        }

        pstObject->stScreen.u16CurTextWidth =
            _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,pstObject->stOnBuffer.u8CurColumn,CC608_BUFFER_ON_SCREEN);
    }

    return HI_SUCCESS;
}

/******************************************************************
 * function name: CC608_DATA_ResumeLastAttr(...)                       *
 * description:   When received the PAC code after displayable    *
 *                characters. it should assumes the attributes of *
 *                that character.  Spec P79                       *
 ******************************************************************/
void CC608_DATA_ResumeLastAttr(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 row;
    HI_U32 color;
    HI_UNF_CC_OPACITY_E opac;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    row = pstObject->stOnBuffer.u8CurRow;

    (HI_VOID)CCDISP_Text_SetItalic(pstObject->enWindowId,pstObject->stOnBuffer.stLastPenAttr[row].bIsItalic);
    pstObject->stPenAttr.bIsItalic = pstObject->stOnBuffer.stLastPenAttr[row].bIsItalic;

    _CC608_DATA_SetUnderline(module_id,(HI_BOOL)pstObject->stOnBuffer.stLastPenAttr[row].bIsUnderline);

    //color = cc608_to_ccdisp_color(pstObject->on_buffer.last_attr[row].fg_color);
    color = pstObject->stOnBuffer.stLastPenAttr[row].u32FgColor;
    opac = pstObject->stOnBuffer.stLastPenAttr[row].enFgOpacity;

    (HI_VOID)CCDISP_Text_SetFGColor(pstObject->enWindowId,color,opac);
    (HI_VOID)CCDISP_Text_SetFlash(pstObject->enWindowId,pstObject->stOnBuffer.stLastPenAttr[row].bIsFlash);
}

static void CC608_TimeoutErase_Callback(HI_U32 u32Param)
{
    HI_U32 ucModuleID = u32Param;
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(ucModuleID);

    if(pstObject == HI_NULL)
    {
        return;
    }

    /*to disable the timeout erase of TEXT mode
      according to the spec. The TEXT mode shouldn't have timeout erase */
    // see spec 608B p26
    if(pstObject->enCurMode != CC608_MODE_TEXT)
    {
        CC608_DATA_ClearSTA(pstObject->module_id);
        pstObject->enVbiMode = CC608_MODE_NOCAPTION;
        pstObject->u8IsModeModified = 1;
        OSD_TRACE("[%s] Timeout\n", __FUNCTION__);
    }
}

HI_S32 CC608_TimeoutErase_TimerInit(HI_U8 module_id)
{
    HI_U32 u32ModuleID = module_id;

    (HI_VOID)CCTimer_Open(CCTIMER_ID_608ERASE, CC608_TimeoutErase_Callback, u32ModuleID);

    return (HI_S32)CCTIMER_ID_608ERASE;
}

void CC608_TimeoutErase_TimerStop(HI_U8 module_id)
{
    (HI_VOID)CCTimer_Stop(CCTIMER_ID_608ERASE);
}

HI_S32 CC608_TimeoutErase_Start(HI_U8 module_id, HI_U32 u32Seconds)
{
    HI_U32 u32Msec = u32Seconds * 1000;

    (HI_VOID)CCTimer_Start(CCTIMER_ID_608ERASE, u32Msec, TIMER_MODE_ONE_SHOOT);

    return HI_SUCCESS;
}

/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/
static HI_BOOL _CC608_DATA_IsCharDisplayable(HI_U16 caption)
{
    HI_BOOL bDisplayable = HI_FALSE;

    if(caption != 0x1111)
    {
        bDisplayable = HI_TRUE;
    }
    return bDisplayable;
}

static inline void _CC608_DATA_CalcStaPosition(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
            HI_ERR_CC("Invalid module id:%d\n",module_id);
            return;
    }
    pstObject->stScreen.stStaScreen.x      = pstObject->stScreen.stPhyScreen.x + pstObject->stScreen.stPhyScreen.width/10;
    pstObject->stScreen.stStaScreen.y      = pstObject->stScreen.stPhyScreen.y + pstObject->stScreen.stPhyScreen.height/10;
    pstObject->stScreen.stStaScreen.width  = pstObject->stScreen.stPhyScreen.width*8/10;
    pstObject->stScreen.stStaScreen.height = pstObject->stScreen.stPhyScreen.height*8/10;

    pstObject->stScreen.u8RowHeight        = pstObject->stScreen.stStaScreen.height/CC608ROWS;
    pstObject->stScreen.u8CharWidth        = pstObject->stScreen.stStaScreen.width/(CC608COLUMNS+2-1);
    return;
}

static inline void _CC608_DATA_ResetBuffer(HI_U8 module_id,CC608_BUFFER_TYPE_E buf_type)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    if(buf_type == CC608_BUFFER_ON_SCREEN)
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }
    else if(buf_type == CC608_BUFFER_OFF_SCREEN)
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else if(buf_type == CC608_BUFFER_TEXT_SCREEN)
    {
        pstBuffer = &(pstObject->stTextBuffer);
    }
    else
    {
        HI_ERR_CC("Invalid buf_type:0x%x\n",buf_type);
        return;
    }

    memset(pstBuffer, 0, sizeof(CC608_BUFFER_S));
    pstBuffer->u32CurSeq = 1;

    if( buf_type == CC608_BUFFER_TEXT_SCREEN)
    {
        pstBuffer->u8CurRow = 7;
    }
    else
    {
        pstBuffer->u8CurRow = 0;
    }
    pstBuffer->u8CurColumn = 0;

    return;
}

/**************************************************************************
* function name: _CC608_DATA_CalcCurrentCursor(HI_U8 module_id,                    *
*                                        HI_U8 row,              *
*                                        HI_U8 column            *
*                                        CC608_BUFFER_TYPE_E buf_type)       *
* description  : calculate the cursor according to current coordinate    *
*                (row,column). It usually called by CC608_DATA_SetColumn      *
* modify:        We must calculate the cursor consider the condition that*
*                two fonts mixed together, if there is a mid-row code we *
*                calculate it.                                           *
*************************************************************************/
static HI_U16 _CC608_DATA_CalcCurrentCursor(HI_U8 module_id,HI_U8 row,HI_U8 column,CC608_BUFFER_TYPE_E buf_type)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 i,is_italic = 0;
    HI_U16 u16CurCursor = 0;
    CC608_BUFFER_S *pstBuffer = HI_NULL;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_SUCCESS;
    }
    if( buf_type == CC608_BUFFER_ON_SCREEN )
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }
    else if( buf_type == CC608_BUFFER_OFF_SCREEN )
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else
    {
        pstBuffer = &(pstObject->stTextBuffer);
    }

    /*we calculate the cursor separately use the mid-row code
     as segmentation,just the mid-row code and pac can set the
     font to italic*/
    if( pstBuffer->u8PacCode[row] == 0x4e || pstBuffer->u8PacCode[row] == 0x4f ||
        pstBuffer->u8PacCode[row] == 0x6e || pstBuffer->u8PacCode[row] == 0x6f)
    {
        is_italic = 1;
    }
    else
    {
        is_italic = 0;
    }


    for( i = 0; i < column; i++)
    {
        u16CurCursor += pstObject->au8CharWidth[is_italic];

        if( pstBuffer->stCaption[row][i].u16Text >= 0x1120 &&
            pstBuffer->stCaption[row][i].u16Text <= 0x112f )
        {
            /*mid-row code*/
            if( pstBuffer->stCaption[row][i].u16Text == 0x112e ||
                pstBuffer->stCaption[row][i].u16Text == 0x112f )
            {
                is_italic = 1;
            }
            else
            {
                is_italic = 0;
            }
        }
    }

    u16CurCursor += pstObject->au8CharWidth[0]; // add the leading space width,

    return u16CurCursor;
}

/*****************************************************************************
* function name: _CC608_DATA_PaintCaption();                                           *
* descripton   : paint on the caption                                       *
* arguments    :                                                            *
*                len:the number of chars that will paint                    *
*                buf:the string to paint                                    *
****************************************************************************/
static HI_S32 _CC608_DATA_PaintCaption(HI_U8 module_id, HI_U16 *buf, HI_U8 len,
                     HI_U8 is_overflow, HI_BOOL bClear, HI_BOOL bHasLeadingSpace, HI_BOOL bTailingSpace)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U16 x,y;
    HI_U8 u8MidrowCode;
    HI_S32 s32DrawWidth = 0;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    x = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.u16CurTextWidth;
    y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;

    if( buf[0] == 0x1111 ) /*the transparent space*/
    {
        s32DrawWidth = pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];

        /*avoid not to clear the tailing space*/
        if(bClear)
        {
            _CC608_DATA_ClrCell(module_id,pstObject->stScreen.u8CurRow, pstObject->stScreen.u8CurColumn+1,1);
        }
        else
        {
            /*redraw tailing space in special case*/
            _CC608_DATA_PaintLeadingTailingSpace(module_id,x,y,pstObject->au8CharWidth[0],pstObject->stScreen.u8RowHeight);
        }
    }
    else if( buf[0] >= 0x1120 && buf[0] <= 0x112f )
    {
        /*Mid_Row Code*/
        OSD_TRACE(">>>>paint_on mid_row code\n");
        u8MidrowCode = buf[0] & 0xff;
        buf[0] = 0x20;
        //(HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf, 1, &s32DrawWidth);
        if( pstObject->stScreen.u8CurColumn != 32 )
        {
            CC608_DEC_MidRow(module_id,0x11,u8MidrowCode);
        }
    }
    else if( buf[0] == 0xbbbb )
    {
        /*flash on command*/
        OSD_TRACE(">>>>paint_on flash_on code\n");
        buf[0] = 0x20;
        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf, 1, &s32DrawWidth,1);

        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);

        (HI_VOID)CC608_DATA_FlashOn(module_id,1);
    }
    else if( ((buf[0]&0xf7ff) >= 0x1020 && (buf[0]&0xf7ff) <= 0x102f) ||
             ((buf[0]&0xf7ff) >= 0x172d && (buf[0]&0xf7ff) <= 0x172f))
    {
        /*background*/
        OSD_TRACE(">>>>paint_on BAC code\n");
        CC608_DEC_Color(module_id, (buf[0]>>8)&0xff, buf[0]&0xff);

        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,HI_FALSE);

        buf[0] = 0x20;

        /************************************************************************
            * For some BAC code are following are Space in a new row, when setting the BAC,
            * the leading of the Space in front should be redrawed in the new attribute.
            ************************************************************************/
        if (bHasLeadingSpace)
        {
            HI_S32 x1,y1,width,height;
            x1 = pstObject->stScreen.stStaScreen.x + pstObject->stScreen.u16CurTextWidth - pstObject->au8CharWidth[0];
            y1 = pstObject->stScreen.stStaScreen.y +
                pstObject->stScreen.u8RowHeight * pstObject->stScreen.u8CurRow;
            width = pstObject->au8CharWidth[0];
            height = pstObject->stScreen.u8RowHeight;
            _CC608_DATA_PaintLeadingTailingSpace(module_id,x1,y1,width,height);
        }
        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf, 1, &s32DrawWidth,1);

        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);

    }
    else   /*regular character*/
    {
        if( pstObject->stScreen.u8CurColumn == 32 && len == 2 &&is_overflow )
        {
            OSD_TRACE("paint_on1:%c%c,cur_width:%d\n",buf[0],buf[1],pstObject->stScreen.u16CurTextWidth);
            (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);

            (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf, 1, &s32DrawWidth,1);
            (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf+1, 1, &s32DrawWidth,1);
        }
        else
        {
            OSD_TRACE("paint_on2:%c%c,cur_width:%d\n",buf[0],buf[1],pstObject->stScreen.u16CurTextWidth);
            (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
            (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId, x, y, buf, len, &s32DrawWidth,1);
        }
    }

    if(s32DrawWidth > 0)
    {
        pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
    }

    return s32DrawWidth;
}

/*****************************************************************************
* function name: _CC608_DATA_PaintLeadingTailingSpace(HI_U8 module_id, HI_U16 x, HI_U16 y) *
* descripton   : paint on the leading space                                 *
* arguments    :                                                            *
*                                                                           *
****************************************************************************/
static void _CC608_DATA_PaintLeadingTailingSpace(HI_U8 module_id, HI_U16 x, HI_U16 y,HI_U16 width, HI_U16 height)
{

    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_UNF_CC_RECT_S rect;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    if(HI_TRUE == pstObject->bLeadingTailingSpace)
    {
        rect.x = x;
        rect.y = y;
        rect.width = width;
        rect.height = height;

        OSD_TRACE("Draw leading and tailing space at %d!\n",rect.x);

        HI_U32 u32Color = 0;
        HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
        (HI_VOID)CCDISP_Text_GetBGColor(pstObject->enWindowId, &u32Color, &enOpacity);
        (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);
    }

    return;
}

/*****************************************************************************
* function name: _CC608_DATA_PopCaption(HI_U8 module_id);                      *
* descripton   : draw all the caption in off-screen buffer                  *
*                before EOC we copy the displayed memory to a temprary buf  *
*                after _CC608_DATA_PopCaption, copy the non_displayed memory to display*
*                memory. and copy the pre displayed memory(temprary buf) to *
*                non_displayed memory                                       *
* arguments    :                                                            *
*                                                                           *
****************************************************************************/
static HI_S32 _CC608_DATA_PopCaption(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_S32 s32Row,s32Column;
    HI_U16 u16CaptionData;
    HI_U16 au16Buf[64];
    HI_U8 u8CharCount,u8RightCharNumInRow = 0;
    HI_S32 s32FirstCaptionColumn = -1;
    HI_S32 s32DrawX = 0, s32DrawY = 0, s32StaX = 0, s32DrawWidth = 0;

    OSD_TRACE("Enter _CC608_DATA_PopCaption!\n");
    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }

    for( s32Row = 0; s32Row < CC608ROWS; s32Row++ )
    {
        if( pstObject->stOffBuffer.u8CharNum[s32Row] == 0 )
        {
            continue;
        }

        u8RightCharNumInRow = pstObject->stOffBuffer.u8CharNum[s32Row];
        s32FirstCaptionColumn = -1;
        u8CharCount = 0;
        s32StaX = pstObject->stScreen.stStaScreen.x;
        s32DrawY = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * s32Row;

        if( pstObject->stOffBuffer.u8PacCode[s32Row] > 0x3f &&pstObject->stOffBuffer.u8PacCode[s32Row] < 0x80 )
        {
            //CC608_DEC_PACDCD(module_id,0x11,pstObject->off_buffer.pac_code[s32Row],1);
        }

        for( s32Column = 0; s32Column < CC608COLUMNS; s32Column++)
        {
            s32DrawWidth = 0;
            if( pstObject->stOffBuffer.stCaption[s32Row][s32Column].u16PacCode != 0 )
            {
                CC608_DEC_PAC(module_id,0x11,pstObject->stOffBuffer.stCaption[s32Row][s32Column].u16PacCode,1);
            }
            if( pstObject->stOffBuffer.stCaption[s32Row][s32Column].u8IsChar == 1)
            {
                u16CaptionData = pstObject->stOffBuffer.stCaption[s32Row][s32Column].u16Text;

                if( -1 == s32FirstCaptionColumn)
                {
                    s32FirstCaptionColumn = s32Column;
                    pstObject->stScreen.u16CurTextWidth =
                        _CC608_DATA_CalcCurrentCursor(module_id,s32Row,s32FirstCaptionColumn,CC608_BUFFER_OFF_SCREEN);

                    if(_CC608_DATA_IsCharDisplayable(u16CaptionData))
                    {
                        HI_S32 x, y, w, h;
                        x = s32StaX + pstObject->stScreen.u16CurTextWidth - pstObject->au8CharWidth[0];
                        y = s32DrawY;
                        w = pstObject->au8CharWidth[0];
                        h = pstObject->stScreen.u8RowHeight;
                        _CC608_DATA_PaintLeadingTailingSpace(module_id, x, y, w, h);
                    }
                }

                s32DrawX = s32StaX + pstObject->stScreen.u16CurTextWidth;
                if( u16CaptionData == 0x1111 )/*transparent space*/
                {
                    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                    (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32DrawX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);
                    u8CharCount = 0;
                    s32FirstCaptionColumn = -1;
                }
                else if( u16CaptionData >= 0x1120 && u16CaptionData <= 0x112f )
                {
                    /*mid row code*/
                    OSD_TRACE(">>>mid row code command\n");
                    if( u8CharCount > 0 )
                    {
                        OSD_TRACE("Draw text at x=%d,y=%d,u8CharCount=%d\n",s32DrawX,s32DrawY,u8CharCount);

                        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32DrawX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);
                        u8RightCharNumInRow -= u8CharCount;
                        u8CharCount = 0;
                    }
                    //au16Buf[u8CharCount++] = 0x20;
                    s32FirstCaptionColumn = s32Column;

                    CC608_DEC_MidRow(module_id,0x11,u16CaptionData&0xff);
                }
                else if( u16CaptionData == 0xbbbb )
                {
                    /*flash on command*/
                    OSD_TRACE("flash on command\n");
                    if( u8CharCount > 0 )
                    {
                        OSD_TRACE("Draw text at x=%d,y=%d\n",s32DrawX,s32DrawY);
                        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32DrawX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);
                        u8RightCharNumInRow -= u8CharCount;
                        u8CharCount = 0;

                    }
                    au16Buf[u8CharCount++] = 0x20;
                    s32FirstCaptionColumn = s32Column;

                    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,pstObject->stPenAttr.bIsUnderline);
                    (HI_VOID)CC608_DATA_FlashOn(module_id,1);
                }
                else if( ((u16CaptionData&0xf7ff) >= 0x1020 && (u16CaptionData&0xf7ff) <= 0x102f) ||
                     ((u16CaptionData&0xf7ff) >= 0x172d && (u16CaptionData&0xf7ff) <= 0x172f))
                {
                    /*background*/
                    OSD_TRACE("backgroud attribution code!\n");
                    if(u8CharCount > 0)
                    {
                        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32DrawX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);
                        u8RightCharNumInRow -= u8CharCount;
                        u8CharCount = 0;
                    }
                    au16Buf[u8CharCount++] = 0x20;
                    s32FirstCaptionColumn = s32Column;
                    CC608_DEC_Color(module_id, (u16CaptionData>>8)&0xff, u16CaptionData&0xff);
                }
                else
                {
                    if(64 > u8CharCount)
                    {
                        au16Buf[u8CharCount++] = u16CaptionData;
                    }
                }

                if(s32DrawWidth > 0)
                {
                    pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
                }
                pstObject->stOffBuffer.stCaption[s32Row][s32Column].u8IsDisplay = 1;
            }
            else
            {
                if( u8CharCount > 0 && u8CharCount == u8RightCharNumInRow)
                {
                    s32DrawX = s32StaX + pstObject->stScreen.u16CurTextWidth;
                    OSD_TRACE("POP_ON,Position:\t%d,%d\n",s32Row,s32FirstCaptionColumn);
                    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                    (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32DrawX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);

                    if(s32DrawWidth > 0)
                    {
                        pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
                    }

                    {
                        HI_S32 x, y, w, h;
                        x = s32StaX + pstObject->stScreen.u16CurTextWidth;
                        y = s32DrawY;
                        w = pstObject->au8CharWidth[0];
                        h = pstObject->stScreen.u8RowHeight;
                        _CC608_DATA_PaintLeadingTailingSpace(module_id, x, y, w, h);
                    }

                    u8CharCount = 0;
                    s32FirstCaptionColumn = -1;
                }
            }

        }
        if( u8CharCount == 0 )
        {
            continue;
        }

        /*draw the caption when a row has  chars to the last column*/
        OSD_TRACE("_CC608_DATA_PopCaption to draw s32Row %d text at x=%d,y=%d count = %d\n",s32Row,s32StaX,s32DrawY,u8CharCount);
        s32StaX += _CC608_DATA_CalcCurrentCursor(module_id,s32Row,s32FirstCaptionColumn,CC608_BUFFER_OFF_SCREEN);
        (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
        (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,s32StaX,s32DrawY,au16Buf,u8CharCount,&s32DrawWidth,1);
        if(s32DrawWidth > 0)
        {
            pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
        }

    }
    OSD_TRACE("_CC608_DATA_PopCaption end!\n");
    return HI_SUCCESS;
}

/*****************************************************************************
* function name: _CC608_DATA_ClearRows(HI_U8 module_id);                       *
* descripton   : clear safe title area with window-color(transparent)       *
* arguments    :                                                            *
*                                                                           *
****************************************************************************/
static HI_S32 _CC608_DATA_ClearRows(HI_U8 module_id, HI_U8 top, HI_U8 bottom)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_UNF_CC_RECT_S rect;
    HI_U8 rows;
    CC608_BUFFER_S *pstBuffer;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    if( top > bottom )
    {
        HI_ERR_CC("Invalide row parameter top = %d, bottom = %d for _CC608_DATA_ClearRows!\n",top, bottom);
        return HI_FAILURE;
    }

    OSD_TRACE("clear rows top=%d, bottom=%d\n", top, bottom);
    rows = bottom - top + 1;

    rect.x = pstObject->stScreen.stStaScreen.x-1;
    rect.y = pstObject->stScreen.stStaScreen.y-1 + pstObject->stScreen.u8RowHeight * top;
    rect.width = pstObject->stScreen.stStaScreen.width+2;
    rect.height = pstObject->stScreen.u8RowHeight * rows+2;

    HI_U32 u32Color = 0;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
    (HI_VOID)CCDISP_Window_GetColor(pstObject->enWindowId, &u32Color, &enOpacity);
    (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);

    if( pstObject->enCurMode == CC608_MODE_TEXT )
    {
        pstBuffer = &(pstObject->stTextBuffer);
    }
    else if( pstObject->enCurMode == CC608_MODE_CAPTION)
    {
        if(pstObject->enDispStyle == CC608_DISP_POPON)
        {
             pstBuffer = &(pstObject->stOffBuffer);
        }
        else
        {
             pstBuffer = &(pstObject->stOnBuffer);
        }
    }
    else
    {
        return HI_FAILURE;
    }

    for ( rows = top; rows <= bottom; rows++ )
    {
        memset(pstBuffer->stCaption[rows], 0, sizeof(CC608_CAPTION_S)*CC608ROWCHARS);
        pstBuffer->u8CharNum[rows] = 0;
        pstBuffer->u8PacCode[rows] = 0;
        pstBuffer->u32SeqNum[rows] = 0;
        memset(&pstBuffer->stLastPenAttr[rows],0,sizeof(CC608_PENATTR_S));
    }
    return HI_SUCCESS;
}
/********************************************************************************
* function name: _CC608_DATA_ResetText(HI_U8 module_id)                           *
* description:   When received Text Restart Command. Call this function        *
*                clear text screen.(include clear text buffer)                 *
*                reset text cursor                                             *
********************************************************************************/
static void _CC608_DATA_ResetText(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    OSD_TRACE("reset Text Mode!\n\n");
    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_TEXT_SCREEN);
    pstObject->stScreen.u8CurRow = 7;
    pstObject->stScreen.u8CurColumn = 0;
    pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);
    pstObject->stScreen.u8BaseRow = 14;
    pstObject->stScreen.u8TopRow  = 7;
    pstObject->stTextBuffer.u8CurColumn = 0;
    pstObject->stTextBuffer.u8CurRow    = 7;

    (HI_VOID)CCDISP_Window_SetColor(pstObject->enWindowId,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);

    /*Draw text background window*/
    (HI_VOID)_CC608_DATA_ClearRows(module_id, 7, CC608ROWS - 1);
    return;
}

/**************************************************************************
* function name: _CC608_DATA_EnterTextMode(HI_U8 module_id)                *
* description  : switch to text mode                                     *
*                1.set pstObject->mode to CC608_MODE_TEXT                         *
*                2.clear caption on screen                               *
*                3.reset text buffer & draw text background window       *
*                4.Set window color to black                             *
*************************************************************************/
static void _CC608_DATA_EnterTextMode(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    OSD_TRACE("Enter Text Mode!\n\n");

    pstObject->enVbiMode = CC608_MODE_TEXT;
    if( pstObject->u8IsModeModified == 0)
    {
        return;
    }

    /*the following is the first time to enter text mode*/
    (HI_VOID)CCDISP_Window_SetColor(pstObject->enWindowId,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);
    pstObject->u8IsModeModified = 0;
    pstObject->enDispStyle = CC608_DISP_ROLLUP;
    OSD_TRACE("Enter Text Mode to call clear sta!\n\n");
    (HI_VOID)CC608_DATA_ClearSTA(module_id);
    _CC608_DATA_ResetText(module_id);
    CC608_DATA_SetDefaultAttr(module_id);
    return;
}


/**************************************************************************
* function name: _CC608_DATA_EnterCaptionMode(HI_U8 module_id)             *
* description  : switch to caption mode                                  *
*                1.set pstObject->mode to CC608_MODE_CAPTION                      *
*                2.set caption background color to transparent           *
*                3.clear text on screen                                  *
*                4.reset text buffer                                     *
*************************************************************************/
static void _CC608_DATA_EnterCaptionMode(HI_U8 module_id)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);


    if( HI_NULL == pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }

    /* if switch from text to rollup caption mode at first time,
       we need to set the sytle other than rollup*/
    if((pstObject->enVbiMode == CC608_MODE_TEXT) &&
       ((pstObject->stOnBuffer.u8CurRow >= 0xff) && (pstObject->stOnBuffer.u8CurColumn >= 0xff)))
    {
        OSD_TRACE("Switch from text to caption, style needs change!!!\n");
        pstObject->enDispStyle = CC608_DISP_BUTT;
    }

    /*switch TEXT mode to caption mode*/
    pstObject->enVbiMode = CC608_MODE_CAPTION;

    if( pstObject->u8IsModeModified == 0)
    {
        return;
    }
    pstObject->u8IsModeModified = 0;

    (HI_VOID)CCDISP_Window_SetColor(pstObject->enWindowId,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);
    /*clear text window*/
    (HI_VOID)_CC608_DATA_ClearRows(module_id, 7, CC608ROWS - 1);

    _CC608_DATA_ResetBuffer(module_id, CC608_BUFFER_TEXT_SCREEN);
    pstObject->stScreen.u8CurColumn = 0;
    pstObject->stScreen.u16CurTextWidth = _CC608_DATA_CalcCurrentCursor(module_id,0,0,CC608_BUFFER_TEXT_SCREEN);

    return;
}

/**************************************************************************
* function name: store_caption_to_buffe(HI_U8 module_id,         *
*                                       HI_U16 *caption,         *
*                                       HI_U16 len,              *
*                                       CC608_BUFFER_TYPE_E buf_type)        *
* description  : store the caption to specific buffer                    *
* return:      1 success. 0 overwrite caption, 2 overwrite pac or mid_row*
*************************************************************************/
static inline HI_S32 _CC608_DATA_StoreCaptionToBuffer(HI_U8 module_id,HI_U16 *caption,HI_U16 len,CC608_BUFFER_TYPE_E buf_type)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    CC608_BUFFER_S *pstBuffer;
    HI_U8 i,row,column;
    HI_U8 u8Rows_HaveChar = 0;
    HI_U8 u8Offset = 0;
    HI_S32 s32Ret = 1;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return HI_FAILURE;
    }
    if(buf_type == CC608_BUFFER_ON_SCREEN)
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }
    else if(buf_type == CC608_BUFFER_OFF_SCREEN)
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else if(buf_type == CC608_BUFFER_TEXT_SCREEN)
    {
        pstBuffer = &(pstObject->stTextBuffer);
    }
    else
    {
        OSD_TRACE("Invalid buf_type:0x%x\n",buf_type);
        return s32Ret;
    }

    if( buf_type != CC608_BUFFER_TEXT_SCREEN )
    {
        /*if the display rows above 4 rows*/
        for( i = 0; i < CC608ROWS; i++ )
        {
            /*don't include current row*/
            if( i == pstBuffer->u8CurRow )
            {
                continue;
            }
            if( pstBuffer->u8CharNum[i] != 0 )
            {
                u8Rows_HaveChar++;
                if( abs(pstBuffer->u8CurRow - i) > u8Offset )
                {
                    u8Offset = abs(pstBuffer->u8CurRow - i);
                }
            }
        }
        if( u8Rows_HaveChar > 3 )
        {
            OSD_TRACE("over than 4 rows delete!\n");
        }
    }
    /*if overflow the manximun column*/
    row = pstBuffer->u8CurRow;
    column = pstBuffer->u8CurColumn;
    if(row >= CC608ROWS)
    {
        row = CC608ROWS-1;
    }

    for( i = 0; i < len; i++)
    {
        if( column >= pstObject->u8CmdCount + CC608COLUMNS - 1 )
        {
            // if the last column is a Mid-Row command
            if((pstBuffer->stCaption[row][column-1].u16Text >= 0x1120) &&
               (pstBuffer->stCaption[row][column-1].u16Text <= 0x112f) &&
               (pstObject->u8CmdCount > 0))
            {
                pstObject->u8CmdCount -= 1;
            }

            {
                column = CC608COLUMNS - 2 + pstObject->u8CmdCount;
                pstBuffer->u8CurColumn = CC608COLUMNS - 2 + pstObject->u8CmdCount;
                pstObject->stScreen.u8CurColumn = CC608COLUMNS - 2;
                s32Ret = 0;
            }
        }
        else
        {
            pstBuffer->u8CharNum[row] ++;
        }
        OSD_TRACE("StoreCaptionToBuffer, row:%d,column:%d,text:%c\n",row,column,caption[i]);

        if( pstBuffer->stCaption[row][column].u8IsChar == 1 && s32Ret != 0)
        {
            /*check mid-row code overwrite*/
            if( caption[i] >=0x1120&&
                 caption[i] <=0x112f )
            {
                s32Ret = 2;
                /* We don't allow Mid-Row overwrite and PAC overwrite happen at the same time */
                pstObject->bPACOverwrite = HI_FALSE;
            }
        }
        if((pstBuffer->stCaption[row][column+1].u8IsChar == 1) && pstObject->bPACOverwrite)
        {
            s32Ret = 4+i;
            pstObject->bPACOverwrite = HI_FALSE;
        }

        pstBuffer->stCaption[row][column].u16Text = caption[i];
        pstBuffer->stCaption[row][column].u8IsChar = 1;

        if(buf_type != CC608_BUFFER_OFF_SCREEN)
        {
            pstBuffer->stCaption[row][column].u8IsDisplay = 1;
        }
        else
        {
            pstBuffer->stCaption[row][column].u8IsDisplay = 0;
        }

        pstBuffer->u8CurColumn ++;
        column ++;

        // if in pop-on mode, we don't need to move the cursor
        if(buf_type != CC608_BUFFER_OFF_SCREEN)
        {
            pstObject->stScreen.u8CurColumn++;
        }

        /*the column 0 pac can't be regarded as pac overwrite*/
        if( pstBuffer->u8IsPacOverwrite && pstBuffer->stCaption[row][column].u8IsChar && column !=  1 )
        {
            s32Ret  = 2;
            pstBuffer->u8IsPacOverwrite = 0;
        }
    }
    return s32Ret;
}

/**************************************************************************
* function name: _CC608_DATA_MoveToNewBaserow(HI_U8 ,HI_U8 ,HI_U8 )               *
*                                                                        *
* description  : according to the 608 spec, when base row changed of     *
*                rollup style, the displayed caption must move to the new*
*                base row                                                *
*                this function move the display caption from old base row*
*                to new base row                                         *
*************************************************************************/
static inline void _CC608_DATA_MoveToNewBaserow(HI_U8 module_id,HI_U8 old_baserow,HI_U8 old_rollrows)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U8 u8RollupRows,u8NewBaseRow;
    //HI_U16 x,srcy,dsty,height,width;
    HI_S32 i;
    CC608_BUFFER_S stBuffer;

    if( HI_NULL==pstObject )
    {
       HI_ERR_CC("Invalid module id:%d\n",module_id);
       return;
    }

    u8NewBaseRow = pstObject->stScreen.u8BaseRow;
    u8RollupRows = pstObject->stScreen.u8BaseRow - pstObject->stScreen.u8TopRow + 1;
    if( u8RollupRows > old_rollrows )
    {
       u8RollupRows = old_rollrows;
    }

#if 0
    x = pstObject->stScreen.stStaScreen.x;
    width = pstObject->stScreen.stStaScreen.width;
    height = u8RollupRows * pstObject->stScreen.u8RowHeight;
    srcy = pstObject->stScreen.stStaScreen.y +
           (old_baserow + 1 - u8RollupRows) * pstObject->stScreen.u8RowHeight;
    dsty = pstObject->stScreen.stStaScreen.y +
           (pstObject->stScreen.u8BaseRow - u8RollupRows + 1)*pstObject->stScreen.u8RowHeight;

    OSD_TRACE("_CC608_DATA_MoveToNewBaserow, rows=%d,height=%d,srcy=%d,dsty=%d\n",u8RollupRows,height,srcy,dsty);

    //(HI_VOID)CCDISP_Window_BlockMove(pstObject->enWindowId,x,dsty,width,height,x,srcy);
#endif

    OSD_TRACE("old_baserow=%d,pstObject->stScreen.u8BaseRow=%d\n",old_baserow, pstObject->stScreen.u8BaseRow);

    /*should add memory move here. move the memory accordingly*/
    /*the following two paragraph code are ugly,but they are
        called infrequently,so to simplify.just do this*/
    for( i = 0; i < u8RollupRows; i++ )
    {
        /*temperary store source buffer*/
        memcpy(stBuffer.stCaption[u8RollupRows-i],
               pstObject->stOnBuffer.stCaption[old_baserow-i],
               sizeof(CC608_CAPTION_S)*CC608ROWCHARS);
        stBuffer.u8CharNum[u8RollupRows-i] =
                pstObject->stOnBuffer.u8CharNum[old_baserow-i];
        stBuffer.u8PacCode[u8RollupRows-i] =
                pstObject->stOnBuffer.u8PacCode[old_baserow-i];
        stBuffer.u32SeqNum[u8RollupRows-i] =
                pstObject->stOnBuffer.u32SeqNum[old_baserow-i];
        memcpy(&stBuffer.stLastPenAttr[u8RollupRows-i],
               &pstObject->stOnBuffer.stLastPenAttr[old_baserow-i],
               sizeof(CC608_PENATTR_S));
    }
    for( i = 0; i < u8RollupRows; i++ )
    {
        /*copy temperary buffer to new baserow*/
        memcpy(pstObject->stOnBuffer.stCaption[u8NewBaseRow-i],
               stBuffer.stCaption[u8RollupRows-i],
               sizeof(CC608_CAPTION_S)*CC608ROWCHARS);
        pstObject->stOnBuffer.u8CharNum[u8NewBaseRow-i] =
                stBuffer.u8CharNum[u8RollupRows-i];
        pstObject->stOnBuffer.u8PacCode[u8NewBaseRow-i] =
                stBuffer.u8PacCode[u8RollupRows-i];
        pstObject->stOnBuffer.u32SeqNum[u8NewBaseRow-i] =
                stBuffer.u32SeqNum[u8RollupRows-i];
        memcpy(&pstObject->stOnBuffer.stLastPenAttr[u8NewBaseRow-i],
               &stBuffer.stLastPenAttr[u8RollupRows-i],
               sizeof(CC608_PENATTR_S));
    }

    if( abs(old_baserow - pstObject->stScreen.u8BaseRow) >= u8RollupRows )
    {
        /*didn't clip*/
        (HI_VOID)_CC608_DATA_ClearRows(module_id,old_baserow + 1 - u8RollupRows, old_baserow);
    }
    else if( old_baserow > pstObject->stScreen.u8BaseRow )
    {
        /*move up*/
        (HI_VOID)_CC608_DATA_ClearRows( module_id,pstObject->stScreen.u8BaseRow + 1, old_baserow );
    }
    else if( old_baserow < pstObject->stScreen.u8BaseRow )
    {
        /*move down*/
        (HI_VOID)_CC608_DATA_ClearRows( module_id, old_baserow + 1 - u8RollupRows, pstObject->stScreen.u8BaseRow - u8RollupRows);
    }
}

static void _CC608_DATA_ClrCell(HI_U8 module_id, HI_U8 row,HI_U8 column, HI_U8 len)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_UNF_CC_RECT_S rect;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    rect.x = pstObject->stScreen.stStaScreen.x + column*pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic];
    rect.y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * row;
    rect.height = pstObject->stScreen.u8RowHeight;
    rect.width = pstObject->au8CharWidth[pstObject->stPenAttr.bIsItalic]*len;

    HI_U32 u32Color = 0;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
    (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
    (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);
    return;
}

/****************************************************************
* function name: redraw_currow_afterhere()                     *
* description:   redraw the captions after the column at       *
*                the specified row                             *
****************************************************************/
static void _CC608_DATA_RedrawRowFromColumn(HI_U8 module_id,HI_U8 row,HI_U8 fromcolumn)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_S32 s32Column , s32DrawWidth = 0;
    HI_U16 x,y,ccdata;
    HI_U16 buf[64];
    HI_U8 u8CharCount = 0;
    HI_U8 u8FirstCaptionColumn = 0xff;
    HI_S32 s32CaptionLen = 0;
    HI_UNF_CC_RECT_S rect;
    HI_S32 width = 0,height = 0;
    HI_U8 u8MidrowCode;

    if( HI_NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",module_id);
        return;
    }
    OSD_TRACE("Enter _CC608_DATA_RedrawRowFromColumn!row=%d,s32Column=%d\n",row,fromcolumn);

    if(row >= CC608ROWS)
    {
 	  HI_ERR_CC("Invalid row num:%d\n",row);
 	  return;
    }

    /*if there is no HI_U8 on this row, do nothing*/
    if( pstObject->stOnBuffer.u8CharNum[row] == 0 )
    {
        return;
    }
    x = pstObject->stScreen.stStaScreen.x;
    y = pstObject->stScreen.stStaScreen.y + pstObject->stScreen.u8RowHeight * row;

    /*before redraw the caption. clear the old captions*/
    s32CaptionLen = _CC608_DATA_CalcCurrentCursor(module_id,row,fromcolumn,CC608_BUFFER_ON_SCREEN);
    rect.x = x + s32CaptionLen;
    rect.y = y;
    rect.width = pstObject->stScreen.stStaScreen.width - s32CaptionLen;
    rect.height = pstObject->stScreen.u8RowHeight;

    HI_U32 u32Color = 0;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
    (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
    (HI_VOID)CCDISP_Window_FillRect(pstObject->enWindowId,u32Color,enOpacity,rect,1);


    for( s32Column = fromcolumn; s32Column < CC608ROWCHARS; s32Column++)
    {
        if( pstObject->stOnBuffer.stCaption[row][s32Column].u8IsChar &&
            pstObject->stOnBuffer.stCaption[row][s32Column].u8IsDisplay )
        {
            /*there is a caption*/
            if( u8FirstCaptionColumn == 0xff)
            {
                u8FirstCaptionColumn = s32Column;
            }
            ccdata = pstObject->stOnBuffer.stCaption[row][s32Column].u16Text;
            if( ccdata == 0x1111 )
            {
                /*transparent*/
                s32CaptionLen = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8FirstCaptionColumn,CC608_BUFFER_ON_SCREEN);
                (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x+s32CaptionLen,y,buf,u8CharCount,&s32DrawWidth,1);

                _CC608_DATA_ClrCell(module_id,row,s32Column,1);
                u8CharCount = 0;
                u8FirstCaptionColumn = 0xff;
            }
            else if( ccdata >= 0x1120 && ccdata <= 0x112f )
            {
                u8MidrowCode = ccdata & 0xff;
                /*mid row code*/

                s32CaptionLen = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8FirstCaptionColumn,CC608_BUFFER_ON_SCREEN);
                (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x+s32CaptionLen,y,buf,u8CharCount,&s32DrawWidth,1);

                if( pstObject->stScreen.u8CurColumn != 32 )
                {
                    CC608_DEC_MidRow(module_id,0x11,u8MidrowCode);
                }

                (HI_VOID)CCDISP_Text_GetSize(pstObject->enWindowId,buf,u8CharCount,&width,&height);

                buf[0] = 0x20;
                (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x+s32CaptionLen+width,y,buf,1,&s32DrawWidth,1);

                u8CharCount = 0;
                u8FirstCaptionColumn = 0xff;
            }
            else if( ccdata == 0xbbbb )
            {
                /*flash on command*/
                if(u8CharCount > 63)
                {
                    u8CharCount = 63;
                }
                buf[u8CharCount++] = 0x20;
                s32CaptionLen = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8FirstCaptionColumn,CC608_BUFFER_ON_SCREEN);
                (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x+s32CaptionLen,y,buf,u8CharCount,&s32DrawWidth,1);

                u8CharCount = 0;
                u8FirstCaptionColumn = 0xff;
                (HI_VOID)CC608_DATA_FlashOn(module_id,1);
            }
            else
            {
                if(u8CharCount > 63)
                {
                    u8CharCount = 63;
                }
                buf[u8CharCount++] = ccdata;
            }

            if(s32DrawWidth > 0)
            {
                pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
            }

            pstObject->stOnBuffer.stCaption[row][s32Column].u8IsDisplay = 1;
        }
        else
        {
            if( u8CharCount > 0 )
            {
                s32CaptionLen = _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8FirstCaptionColumn,CC608_BUFFER_ON_SCREEN);
                (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
                (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x+s32CaptionLen,y,buf,u8CharCount,&s32DrawWidth,1);

                u8CharCount = 0;
                u8FirstCaptionColumn = 0xff;
            }
        }
    }
    if( u8CharCount == 0 )
    {
        return;
    }
    /*draw the caption*/
    OSD_TRACE("_CC608_DATA_PopCaption row %d text at x=%d,y=%d count = %d\n",row,x,y,u8CharCount);
    x += _CC608_DATA_CalcCurrentCursor(module_id,pstObject->stOnBuffer.u8CurRow,u8FirstCaptionColumn,CC608_BUFFER_ON_SCREEN);
    (HI_VOID)CCDISP_Text_SetUnderLine(pstObject->enWindowId,(HI_BOOL)pstObject->stPenAttr.bIsUnderline);
    (HI_VOID)CCDISP_Text_Draw(pstObject->enWindowId,x,y,buf,u8CharCount,&s32DrawWidth,1);
    if(s32DrawWidth > 0)
    {
        pstObject->stScreen.u16CurTextWidth += s32DrawWidth;
    }

    return;
}

static void _CC608_DATA_SetUnderline(HI_U8 module_id, HI_BOOL bUnderline)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);

    if (HI_NULL == pstObject)
    {
        return;
    }

    pstObject->bUnderline = bUnderline;
}

