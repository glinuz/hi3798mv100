#include<string.h>

#include"hi_type.h"
#include"hi_debug.h"

#include "hi_cc708_def.h"
#include "cc708_osd.h"
#include "cc708_pen.h"
#include "cc708_cursor.h"
#include "cc708_obj.h"
#include "ccdisp_api.h"
#include "cc_debug.h"
#include "cc_timer.h"


/********************************************************************************
*                      STATIC FUNTION DEFINITION
*********************************************************************************/
#define ABS(a) ((a)>=0?(a):-(a))

#define MAX_CC_WINDOWS 8
#define LEADINGSPACEWIDTH 0
#define TAILINGSPACEWIDTH 0
#define INTERLINE_HEIGTH 5

#define DTVCC_ERASE_TEXT_TIME 16 //16s

typedef struct tagCC708CharInfoInFont
{
    HI_U16 u16MaxChar;
    HI_S32 s32MaxCharWidth;
    HI_BOOL bScanned;
}CC708CharInfoInFont_t;


static CC708_Window_S CCWindow[MAX_CC_WINDOWS];
static HI_S32 CurrentWindowID = -1;
static const CC708_WindowAttr_S astPreDefWinAttrTable[7] =
{

    {HI_UNF_CC_JUSTIFY_LEFT, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_DISABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_LEFT, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_DISABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_TRANSPARENT,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_CENTER, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_DISABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_LEFT, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_ENABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_LEFT, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_ENABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_TRANSPARENT, HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_CENTER, CC_PD_LEFT_RIGHT, CC_SD_BOTTOM_TOP, HI_UNF_CC_WW_ENABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_JUSTIFY_LEFT, CC_PD_TOP_BOTTOM, CC_SD_RIGHT_LEFT, HI_UNF_CC_WW_DISABLE, CC_EFFECT_SNAP,
     CC_DED_LEFT_RIGHT,1,HI_UNF_CC_COLOR_BLACK, HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_EDGETYPE_NONE,HI_UNF_CC_COLOR_BLACK}
};

static const CC708_PenAttr_S astPreDefPenAttrTable[7] =
{

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_DEFAULT,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_NONE,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_MONOSPACED,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_NONE,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_PROPORT,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_NONE,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_MONOSPACED_NO_SERIAFS,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_NONE,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_PROPORT_NO_SERIAFS,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_NONE,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_MONOSPACED_NO_SERIAFS,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_UNIFORM,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT,HI_UNF_CC_COLOR_BLACK},

    {HI_UNF_CC_FONTSIZE_STANDARD,HI_UNF_CC_FN_PROPORT_NO_SERIAFS,CC_OS_NORMAL,0,0,HI_UNF_CC_EDGETYPE_UNIFORM,
    HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT,HI_UNF_CC_COLOR_BLACK},
};


static HI_U16 au16G2UnicodeTable[] =
{
    0x2026,0x0160,0x0152,0x2588,0x2018,0x2019,0x201c,0x201d,
    0x2022,0x2122,0x0161,0x0153,0x2120,0x0178,0x215b,0x215c,
    0x215d,0x215e,0x2502,0x2510,0x2514,0x2500,0x2518,0x250c,
};


/********************************************************************************
*                      STATIC FUNTION STATEMENT
*********************************************************************************/
static inline HI_S32 _DtvCC_CheckWindowOverlap(HI_U8 u8WindowID, HI_U8 flag);
static inline HI_VOID _DtvCC_ClearWindowOnScreen(HI_U8 WindowID,HI_U8 isClear);
static inline HI_VOID _DtvCC_ClearWindowOffScreen(HI_U8 WindowID);
static inline HI_VOID _DtvCC_ClearRowOnScreen(HI_U8 u8WindowID, HI_U8 u8Row);
static inline HI_VOID _DtvCC_ClearRowOffScreen(HI_U8 u8WindowID, HI_U8 u8Row);
static inline HI_VOID _DtvCC_ClearRow(HI_U8 u8WindowID, HI_U8 u8Row);
static inline HI_VOID _DtvCC_CalcDrawStartPositionByRow(HI_U8 u8WindowID, HI_S32 s32Row);
static inline HI_VOID _DtvCC_GetTextSize(HI_S32 u8WindowID,HI_U16 *buf,
                                                 HI_U8 size, HI_S32 *w, HI_S32 *h);
static inline HI_VOID _DtvCC_BlockMove(HI_U8 u8WindowID, HI_U16 u16DstX, HI_U16 u16DstY, HI_U16 u16Width,
                                              HI_U16 u16Height,HI_U16 u16SrcX,HI_U16 u16SrcY);
static inline HI_VOID _DtvCC_DispText(HI_U8 u8WindowID, HI_S32 x, HI_S32 y,HI_S32 s32Width, HI_S32 s32Height,
                                          HI_U16 *pu16TextBuf, HI_S32 s32TextNum,CC708_PrintDirection_E enPrintDirection,
                                          CC708_ScrollDirection_E enScrollDirection,HI_BOOL bOffScreen,
                                           HI_BOOL bIgnoreLeftBG,HI_BOOL bTailingSpace);
static inline HI_BOOL _DtvCC_CalcWinPosByAnchorPoint(HI_U8 u8WindowID);

static inline HI_VOID _DtvCC_InitWindow(HI_U8 u8WindowID);
static inline HI_VOID _DtvCC_DeleteWindow(HI_U8 u8WindowID);
static inline HI_VOID _DtvCC_DrawCaptionToWindowOffScreen(HI_U16 *buf,HI_S32 len);
static inline HI_VOID _DtvCC_DrawCaptionToWindowOnScreen(HI_U16 *buf, HI_S32 len, HI_S32 row, HI_S32 column);
static inline HI_VOID _DtvCC_DrawPendingRow(HI_VOID);


static inline HI_VOID _DtvCC_FillRect(HI_U8 u8WindowID, HI_S32 s32X, HI_S32 s32Y, HI_S32 s32Width, HI_S32 s32Height,
                                        HI_S32 isClear, CC708_PrintDirection_E enPrintDirection,
                                        CC708_ScrollDirection_E enScrollDirection, HI_BOOL bOffScreen);
static inline HI_S32 _DtvCC_GetYPosByRow(HI_U8 u8WindowID, HI_U8 curRow);
static inline HI_VOID  _DtvCC_HideWindow(HI_U8 u8WindowID);
static inline HI_VOID _DtvCC_MoveCursorByRowAndColumn(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column);
static inline HI_VOID _DtvCC_MoveCursorByRowAndColumnOnScreen(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column);
static inline HI_VOID _DtvCC_MoveCursorByRowAndColumnOffScreen(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column);
static inline HI_VOID _DtvCC_ReDrawWindow(HI_U8 u8WindowID, HI_BOOL bOffScreen);

static inline HI_U8 _DtvCC_SetWindowColor(HI_U8 u8WindowID,HI_U32 u32FillColor,HI_U8 u8FillOpacity);
static inline HI_U8 _DtvCC_SetCurrentPenAttributes(CC708_PenDef_S * pstPen,HI_U8 u8FontName,HI_U8 u8FontSize,
                                                              HI_U8 u8IsItalic,HI_U8 u8Underline,HI_U8 u8EdgeType,
                                                              HI_U8 u8TextOffset,HI_U8 u8TextTag,HI_U8 u8IsVisible);
static inline HI_VOID _DtvCC_SetPenAttrByPenStyleID(HI_U8 u8WindowID);
static inline HI_VOID _DtvCC_SetWinAttrByWinStyleID(HI_U8 u8WindowID);
static inline HI_U8 _DtvCC_SetCurrentPenColor(CC708_PenDef_S *pstPen,HI_U32 u32FGColor,HI_U8 u8FGOpacity,
                                     HI_U32 u32BGColor,HI_U8 u8BGOpacity,HI_U32 u32EdgeColor, HI_U8 u8IsVisible);
static inline HI_VOID _DtvCC_ScrollRows(HI_U8 u8WindowID, HI_S32 s32ScrollRows,
                              CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection);
static inline HI_VOID _DtvCC_SetLastDisplayOrder(HI_U8 u8WindowID);
static inline HI_VOID _DtvCC_SetDefaultDisplayOrder(HI_U8 u8WindowID);

static HI_VOID _DtvCC_ClearLeadingSpace(HI_U8 u8WindowID);
static HI_VOID _DtvCC_DrawLeadingSpace(HI_U8 u8WindowID, HI_S32 x, HI_S32 y,
                                                   HI_S32 isClear, HI_BOOL bFisrtColumn, HI_BOOL bOffScreen);
static HI_VOID _DtvCC_DrawRowText(HI_U8 u8WindowID,HI_U16 u16StartX, HI_U16 u16StartY, HI_U8 u8Row, HI_BOOL bOffScreen);
static HI_VOID _DtvCC_EraseTimerStart(HI_U8 seconds);
static HI_S32 _DtvCC_EraseTimerInit(HI_S32 s32ModuleID);


/********************************************************************************
*                      STATIC FUNTION DEFINITION
*********************************************************************************/

static HI_U16 _DtvCC_GetMaxCharWidth(HI_UNF_CC_FONTNAME_E CurFontName, HI_UNF_CC_FONTSIZE_E CurFontSize, HI_S32 *pMaxWidth)
{
    //scan result
    HI_S32 s32MaxWidth=0;
    HI_U16 u16MaxChar=0;

    //local variables
    HI_U16 u16CurrentChar = 0;
    HI_S32 s32CharWidth = 0,s32CharHeight = 0;

    //static char info table
    static CC708CharInfoInFont_t stSmallCharInfo[8]= {{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},};
    static CC708CharInfoInFont_t stNormalCharInfo[8]={{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},};
    static CC708CharInfoInFont_t stLargeCharInfo[8]= {{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},{0,0,HI_FALSE,},};

    //current table id
    CC708CharInfoInFont_t *stCharInfo;

    switch (CurFontSize)
    {
        case HI_UNF_CC_FONTSIZE_SMALL:
            stCharInfo = stSmallCharInfo;
            break;
        case HI_UNF_CC_FONTSIZE_STANDARD:
            stCharInfo = stNormalCharInfo;
            break;
        case HI_UNF_CC_FONTSIZE_LARGE:
            stCharInfo = stLargeCharInfo;
            break;
        default:
            stCharInfo = stNormalCharInfo;
            *pMaxWidth=0;
            return 0;
    }

    if (/*CurFontName < HI_UNF_CC_FN_DEFAULT||*/ CurFontName >= HI_UNF_CC_FN_BUTT)
    {
        CurFontName = HI_UNF_CC_FN_DEFAULT;
    }

    //if not scaned, we start scan
    if (!stCharInfo[CurFontName].bScanned)
    {
        //scan G0 form        '!'     to     'Music Note'
        for (u16CurrentChar = 0x21; u16CurrentChar <= 0x7f; u16CurrentChar++)
        {
            if(u16CurrentChar == 0x7f)
            {
                u16CurrentChar = 0x266a;
            }

            _DtvCC_GetTextSize(0,&u16CurrentChar,1,&s32CharWidth,&s32CharHeight);

            //save max_width
            if (s32CharWidth > s32MaxWidth)
            {
                u16MaxChar = u16CurrentChar;
                s32MaxWidth = s32CharWidth;
            }
        }

        //Scan G1 from  latin 'i'     to          'y'
        for (u16CurrentChar = 0xa1; u16CurrentChar <= 0xff; u16CurrentChar++)
        {
            //get current character size
            _DtvCC_GetTextSize(0,&u16CurrentChar,1,&s32CharWidth,&s32CharHeight);

            //save max_width
            if (s32CharWidth > s32MaxWidth)
            {
                u16MaxChar = u16CurrentChar;
                s32MaxWidth = s32CharWidth;
            }
        }

        //scan G2
        {
            HI_U8 i=0;
            for (i = 0; i < 24; i++)
            {
                //map char to G3, if eq 0x005f, jump
                u16CurrentChar = au16G2UnicodeTable[i];
                _DtvCC_GetTextSize(0,&u16CurrentChar,1,&s32CharWidth,&s32CharHeight);

                //save max_width
                if (s32CharWidth > s32MaxWidth)
                {
                    u16MaxChar = u16CurrentChar;
                    s32MaxWidth = s32CharWidth;
                }
            }
        }

        //scan G3
        {
            //get current character size
            u16CurrentChar = 0xf101;
            _DtvCC_GetTextSize(0,&u16CurrentChar,1,&s32CharWidth,&s32CharHeight);

            //save max_width
            if (s32CharWidth > s32MaxWidth)
            {
                u16MaxChar = u16CurrentChar;
                s32MaxWidth = s32CharWidth;
            }
        }

        //save to charinfo structer
        stCharInfo[CurFontName].bScanned = HI_TRUE;
        stCharInfo[CurFontName].u16MaxChar = u16MaxChar;
        stCharInfo[CurFontName].s32MaxCharWidth = s32MaxWidth;
    }

    //set max_width & height parameter
    if (pMaxWidth != NULL)
    {
        *pMaxWidth = stCharInfo[CurFontName].s32MaxCharWidth;
    }

    return (stCharInfo[CurFontName].u16MaxChar);

}

static inline HI_VOID _DtvCC_DrawTSP(HI_S32 s32X, HI_S32 s32Y, HI_BOOL bOffScreen)
{

    HI_S32 WinID = CurrentWindowID;
    HI_S32 s32Width = 0, s32Heigth = 0;

    /* draw space on screen */
    if(CCWindow[WinID].u8IsVisible)
    {
        HI_U16 au16Space[1] = {0x20};
        _DtvCC_GetTextSize(WinID,au16Space,1,&s32Width, &s32Heigth);
        _DtvCC_FillRect(WinID,s32X,s32Y,s32Width,s32Heigth,1,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen);
    }
}

static inline HI_VOID _DtvCC_ReDrawWindow(HI_U8 u8WindowID, HI_BOOL bOffScreen)
{
    HI_S32 s32Row,s32CurRow;
    CC708_Cursor_S *pstCursor;

    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    /*here we begin timer auto erase time-out text*/
    _DtvCC_EraseTimerStart(DTVCC_ERASE_TEXT_TIME);

    /*recover window color*/
    (HI_VOID)_DtvCC_SetWindowColor(u8WindowID,CCWindow[u8WindowID].u32FillColor,CCWindow[u8WindowID].u32FillOpacity);

    /*recover pen attribute*/
    (HI_VOID)_DtvCC_SetCurrentPenAttributes(&CCWindow[u8WindowID].stCurPen,
                                            CCWindow[u8WindowID].stCurPen.enFontName,
                                            CCWindow[u8WindowID].stCurPen.enFontSize,
                                            CCWindow[u8WindowID].stCurPen.u8IsItalic,
                                            CCWindow[u8WindowID].stCurPen.u8IsUnderline,
                                            CCWindow[u8WindowID].stCurPen.enEdgeType,
                                            CCWindow[u8WindowID].stCurPen.enTextOffset,
                                            CCWindow[u8WindowID].stCurPen.enTextTag,
                                            1);
    /*recover pen color*/
    (HI_VOID)_DtvCC_SetCurrentPenColor(&CCWindow[u8WindowID].stCurPen,
                                           CCWindow[u8WindowID].stCurPen.u32FGColor,
                                           CCWindow[u8WindowID].stCurPen.u8FGOpacity,
                                           CCWindow[u8WindowID].stCurPen.u32BGColor,
                                           CCWindow[u8WindowID].stCurPen.u8BGOpacity,
                                           CCWindow[u8WindowID].stCurPen.u32EdgeColor,
                                           1);
    /* clear window on the screen */
    _DtvCC_ClearWindowOnScreen(u8WindowID,0);

#if 0    
    if(bOffScreen)
    {
        HI_S32 x,y,width,height;
        x = 0;
        y = 0;
        width = CCWindow[u8WindowID].u16Width;
        height = CCWindow[u8WindowID].u16Height;
        _DtvCC_FillRect(u8WindowID,x,y,width,height,0,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen);

    }
#endif

    pstCursor = &CCWindow[u8WindowID].stCursor;
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);

    for(s32Row = 0; s32Row <= s32CurRow; s32Row++)
    {
        HI_S32 s32Y;
        /* calculate draw start position of current s32Row */
        _DtvCC_CalcDrawStartPositionByRow(u8WindowID,s32Row);

        s32Y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);

        if(bOffScreen)
        {
            s32Y -= CCWindow[u8WindowID].u16Y;
        }

        /* draw text */
        _DtvCC_DrawRowText(u8WindowID,0,s32Y,s32Row,bOffScreen);
    }

}


static inline HI_VOID _DtvCC_FillRect(HI_U8 u8WindowID, HI_S32 s32X, HI_S32 s32Y, HI_S32 s32Width, HI_S32 s32Height,
                            HI_S32 isClear, CC708_PrintDirection_E enPrintDirection,
                            CC708_ScrollDirection_E enScrollDirection, HI_BOOL bOffScreen)
{
    HI_UNF_CC_RECT_S stRect;
    HI_U32 u32Color = 0;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
    CCDISP_WINDOW_ID_E enWindowId = (CCDISP_WINDOW_ID_E)u8WindowID;

    stRect.x= s32X;
    stRect.y = s32Y;
    stRect.width = s32Width;
    stRect.height = s32Height;
    if(isClear == 1)
    {
        (HI_VOID)CCDISP_Screen_GetColor(&u32Color, &enOpacity);
    }
    else if(isClear == 2)
    {
        (HI_VOID)CCDISP_Text_GetBGColor(enWindowId, &u32Color, &enOpacity);
    }
    else
    {
        (HI_VOID)CCDISP_Window_GetColor(enWindowId, &u32Color, &enOpacity);
    }
    (HI_VOID)CCDISP_Window_FillRect(enWindowId, u32Color, enOpacity, stRect ,2);
}


static inline HI_U8 _DtvCC_SetCurrentPenColor(CC708_PenDef_S *pstPen,HI_U32 u32FGColor,HI_U8 u8FGOpacity,
                                     HI_U32 u32BGColor,HI_U8 u8BGOpacity,HI_U32 u32EdgeColor, HI_U8 u8IsVisible)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_U32 u32TextFGColor = 0,u32TextFGOpacity = 0, u32TextBGColor = 0,u32TextBGOpacity = 0, u32TextEdgeColor = 0;
    HI_U8 bPenChanged = 0;
    CCDISP_WINDOW_ID_E enWindowId = (CCDISP_WINDOW_ID_E)CurrentWindowID;

    OSD_TRACE("_DtvCC_SetCurrentPenColor,fgclr:0x%x,fgopa:%s,bgclr:0x%x,bgopa:%s\n",
             u32FGColor,DBG_OPA_OUT[u8FGOpacity],u32BGColor,DBG_OPA_OUT[u8BGOpacity]);

    u32TextFGColor = (pCCObj->u32UserTextFGColor == HI_UNF_CC_COLOR_DEFAULT ?
                       u32FGColor : pCCObj->u32UserTextFGColor);
    u32TextFGOpacity = (pCCObj->u32UserTextFGOpacity == HI_UNF_CC_OPACITY_DEFAULT ?
                         u8FGOpacity : pCCObj->u32UserTextFGOpacity);
    u32TextBGColor = (pCCObj->u32UserTextBgColor == HI_UNF_CC_COLOR_DEFAULT ?
                        u32BGColor : pCCObj->u32UserTextBgColor);
    u32TextBGOpacity = (pCCObj->u32UserTextBGOpacity == HI_UNF_CC_OPACITY_DEFAULT ?
                         u8BGOpacity : pCCObj->u32UserTextBGOpacity);
    u32TextEdgeColor = (pCCObj->u32UserTextEdgecolor == HI_UNF_CC_COLOR_DEFAULT ?
                      u32EdgeColor : pCCObj->u32UserTextEdgecolor);

    bPenChanged |= CC708_Pen_SetTextFgColor(pstPen,u32TextFGColor,u32TextFGOpacity,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetFGColor(enWindowId,u32TextFGColor,(HI_UNF_CC_OPACITY_E)u32TextFGOpacity);

    bPenChanged |= CC708_Pen_SetTextBgColor(pstPen,u32TextBGColor,u32TextBGOpacity,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetBGColor(enWindowId,u32TextBGColor,(HI_UNF_CC_OPACITY_E)u32TextBGOpacity);

    bPenChanged |= CC708_Pen_SetTextEdgeColor(pstPen,u32TextEdgeColor,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetEdgeColor(enWindowId,u32TextEdgeColor,HI_UNF_CC_OPACITY_SOLID);

    return bPenChanged;
}

static inline HI_U8 _DtvCC_SetCurrentPenAttributes(CC708_PenDef_S * pstPen,HI_U8 u8FontName,HI_U8 u8FontSize,
                                          HI_U8 u8IsItalic,HI_U8 u8Underline,HI_U8 u8EdgeType,
                                          HI_U8 u8TextOffset,HI_U8 u8TextTag,HI_U8 u8IsVisible)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_U32 u32FontName = u8FontName, u32FontSize = u8FontSize;
    HI_U32 u32EdgeType = u8EdgeType;
    HI_U8 IsItalic = u8IsItalic, IsUnderline = u8Underline;
    HI_U8 bPenChanged = 0;
    CCDISP_WINDOW_ID_E enWindowId = (CCDISP_WINDOW_ID_E)CurrentWindowID;

    if(pCCObj->u32UserFontName != HI_UNF_CC_FN_DEFAULT)
    {
        u32FontName = pCCObj->u32UserFontName;
    }
    if(pCCObj->u32UserFontSize != HI_UNF_CC_FONTSIZE_DEFAULT)
    {
        u32FontSize = pCCObj->u32UserFontSize;
    }
    if(pCCObj->u32UserFontStyle != HI_UNF_CC_FONTSTYLE_DEFAULT)
    {
        if(pCCObj->u32UserFontStyle == HI_UNF_CC_FONTSTYLE_ITALIC)
        {
            IsItalic = 1;
        }
        else if(pCCObj->u32UserFontStyle == HI_UNF_CC_FONTSTYLE_UNDERLINE)
        {
            IsUnderline = 1;
        }
        else if(pCCObj->u32UserFontStyle == HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE)
        {
            IsItalic = 1;
            IsUnderline = 1;
        }
    }
    if(pCCObj->u32UserTextEdgetype != HI_UNF_CC_EDGETYPE_DEFAULT)
    {
        u32EdgeType = pCCObj->u32UserTextEdgetype;
    }

    bPenChanged |= CC708_Pen_SetFontStyle(pstPen,u32FontName,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetFontName(enWindowId, (HI_UNF_CC_FONTNAME_E)u32FontName);

    bPenChanged |= CC708_Pen_SetPenSize(pstPen,u32FontSize,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetFontSize(enWindowId, (HI_UNF_CC_FONTSIZE_E)u32FontSize);
    if(bPenChanged)
    {
        (HI_VOID)_DtvCC_GetMaxCharWidth((HI_UNF_CC_FONTNAME_E)u32FontName,(HI_UNF_CC_FONTSIZE_E)u32FontSize,&pCCObj->s32MaxCharWidth);
    }

    bPenChanged |= CC708_Pen_SetTextItalics(pstPen,IsItalic,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetItalic(enWindowId,(HI_BOOL)IsItalic);

    bPenChanged |= CC708_Pen_SetTextUnderline(pstPen,IsUnderline,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetUnderLine(enWindowId,(HI_BOOL)IsUnderline);

    bPenChanged |= CC708_Pen_SetTextEdgeType(pstPen,u32EdgeType,u8IsVisible);
    (HI_VOID)CCDISP_Text_SetEdgeType(enWindowId,(HI_UNF_CC_EdgeType_E)u32EdgeType);

    bPenChanged |= CC708_Pen_SetTextOffset(pstPen,u8TextOffset,u8IsVisible);
    bPenChanged |= CC708_Pen_SetTextTag(pstPen,u8TextTag,u8IsVisible);

    return bPenChanged;
}


static inline HI_VOID _DtvCC_SetWinAttrByWinStyleID(HI_U8 u8WindowID)
{
    HI_U8 u8WinStyleID = CCWindow[u8WindowID].u8WindowStyleID - 1;

    /* set predefined window attributes */
    CCWindow[u8WindowID].enTextJustify = astPreDefWinAttrTable[u8WinStyleID].enJustify;
    CCWindow[u8WindowID].enTextPrintDirection = astPreDefWinAttrTable[u8WinStyleID].enPrintDirection;
    CCWindow[u8WindowID].enTextScrollDirection = astPreDefWinAttrTable[u8WinStyleID].enScrollDirection;
    CCWindow[u8WindowID].enWordWrap = astPreDefWinAttrTable[u8WinStyleID].enWordWrap;
    CCWindow[u8WindowID].enDisplayEffect = astPreDefWinAttrTable[u8WinStyleID].enDisplayEffect;
    CCWindow[u8WindowID].enEffectDirection = astPreDefWinAttrTable[u8WinStyleID].enEffectDirection;
    CCWindow[u8WindowID].u8EffectSpeed = astPreDefWinAttrTable[u8WinStyleID].u8EffectSpeed;
    CCWindow[u8WindowID].enBorderType = astPreDefWinAttrTable[u8WinStyleID].enBorderType;
    CCWindow[u8WindowID].u32BorderColor = astPreDefWinAttrTable[u8WinStyleID].u32BorderColor;

    CCWindow[u8WindowID].u32FillColor = astPreDefWinAttrTable[u8WinStyleID].u32FillColor;
    CCWindow[u8WindowID].u32FillOpacity = astPreDefWinAttrTable[u8WinStyleID].enFillOpacity;

    (HI_VOID)_DtvCC_SetWindowColor(u8WindowID,CCWindow[u8WindowID].u32FillColor,
                                   CCWindow[u8WindowID].u32FillOpacity);

    if((u8WinStyleID == 1)||(u8WinStyleID == 4))
    {
        CCWindow[u8WindowID].u8IsEmpty = 1;
    }


}

/***********************************************************************************
 * function name: _DtvCC_SetPenAttrByPenStyleID(HI_U8 u8WindowID)
 * description:   Initialize the pen attributes according to the pen StyleID
 * arguments:     u8WindowID
 * creation:
 * Modify history:
 ***********************************************************************************/
static inline HI_VOID  _DtvCC_SetPenAttrByPenStyleID(HI_U8 u8WindowID)
{
    CC708_Cursor_S *pstCursor;
    CC708_PenDef_S *pstPen;
    HI_S32 s32Row,s32Column;
    HI_U8 u8PenStyleID = CCWindow[u8WindowID].u8PenStyleID - 1;
    HI_U32 u32FGColor,u32BGColor,u32EdgeColor;

    /* set font attributes of the window */
    _DtvCC_SetCurrentPenAttributes(&CCWindow[u8WindowID].stCurPen,
                                 astPreDefPenAttrTable[u8PenStyleID].enFontName,
                                 astPreDefPenAttrTable[u8PenStyleID].enFontSize,
                                 astPreDefPenAttrTable[u8PenStyleID].u8IsItalic,
                                 astPreDefPenAttrTable[u8PenStyleID].u8IsUnderline,
                                 astPreDefPenAttrTable[u8PenStyleID].enEdgeType,
                                 astPreDefPenAttrTable[u8PenStyleID].enTextOffset,
                                 0,1);

    u32FGColor = astPreDefPenAttrTable[u8PenStyleID].u32FGColor;
    u32BGColor = astPreDefPenAttrTable[u8PenStyleID].u32BGColor;
    u32EdgeColor = astPreDefPenAttrTable[u8PenStyleID].u32EdgeColor;

    /* set pen color of the window */
     _DtvCC_SetCurrentPenColor(&CCWindow[u8WindowID].stCurPen,u32FGColor,
                             astPreDefPenAttrTable[u8PenStyleID].enFGOpacity,
                             u32BGColor,
                             astPreDefPenAttrTable[u8PenStyleID].enBGOpacity,
                             u32EdgeColor,
                             1);

    /* set pen attributes and color of current text */
    pstCursor = &CCWindow[u8WindowID].stCursor;
    s32Row = CC708_Cursor_GetCurrentRow(pstCursor);
    s32Column = CC708_Cursor_GetCurrentColumn(pstCursor);
    pstPen = &CCWindow[u8WindowID].astText[s32Row][s32Column].stPen;
    memcpy(pstPen,&CCWindow[u8WindowID].stCurPen,sizeof(CC708_PenDef_S));

    /* set bPenChanged flag,here will always be 1 */
    CCWindow[u8WindowID].astText[s32Row][s32Column].u8IsPenChanged = 1;
}

static inline HI_U8 _DtvCC_SetWindowColor(HI_U8 u8WindowID,HI_U32 u32FillColor,HI_U8 u8FillOpacity)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_U32 u32WindowColor = u32FillColor, u32WindowOpacity = u8FillOpacity;
    HI_U8 u8IsChanged = 0;

    /* set window fill color */
    if( CCWindow[u8WindowID].u32FillColor != u32FillColor )
    {
        CCWindow[u8WindowID].u32FillColor = u32FillColor;
        u8IsChanged = 1;
    }

    if(pCCObj->u32UserWinColor != HI_UNF_CC_OPACITY_DEFAULT)
    {
       u32WindowColor = pCCObj->u32UserWinColor;
    }

    if( CCWindow[u8WindowID].u32FillOpacity != u8FillOpacity )
    {
        CCWindow[u8WindowID].u32FillOpacity = u8FillOpacity;
        u8IsChanged = 1;
    }

    if(pCCObj->u32UserWinOpac != HI_UNF_CC_OPACITY_DEFAULT)
    {
        u32WindowOpacity = pCCObj->u32UserWinOpac;
    }

    (HI_VOID)CCDISP_Window_SetColor((CCDISP_WINDOW_ID_E)u8WindowID,u32WindowColor,
                                    (HI_UNF_CC_OPACITY_E)u32WindowOpacity);
    return u8IsChanged;
}

static inline HI_VOID _DtvCC_ScrollRows(HI_U8 u8WindowID, HI_S32 s32ScrollRows,
                              CC708_PrintDirection_E enPrintDirection, CC708_ScrollDirection_E enScrollDirection)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_S32 i;
    HI_S32 s32Row;
    HI_S32 s32SrcX,s32SrcY,s32DstX,s32DstY;
    HI_S32 s32ScrollOffset,s32ScrollWidth;
    static HI_S32 s32RollupStep = 2;
    HI_S32 s32OverlapHigherWindowMap = CCWindow[u8WindowID].s32OverlapHigherWindowMap;

    if(s32ScrollRows == 0)
    {
        return;
    }

    s32RollupStep = (CCWindow[u8WindowID].as32RowHeight[0]==0)?pCCObj->s32RowHeight:CCWindow[u8WindowID].as32RowHeight[0];
    s32RollupStep = s32RollupStep/pCCObj->u8ScrollTimes;
    OSD_TRACE("s32RollupStep:%d\n",s32RollupStep);

    /* scroll the window on a line basis, and scroll the line on a rollup step basis */

    /* calculate the block move destination position, always the top-left corner of the window */
    s32DstX = CCWindow[u8WindowID].u16X;
    s32SrcY = CCWindow[u8WindowID].u16Y;
    s32SrcX = s32DstX;
    s32DstY = s32SrcY + CCWindow[u8WindowID].u16Height;

    if((s32OverlapHigherWindowMap != 0) && (CCWindow[u8WindowID].u16Width*CCWindow[u8WindowID].u16Height <= 1920*60))
    {
        #if 0
            HI_UNF_CC_RECT_S rect;

            bUseDoubleBuffer = HI_TRUE;
            CCDispSetBackupSurface(pCCObj->sid,CCWindow[u8WindowID].u16Width,CCWindow[u8WindowID].u16Height);

            rect.x = 0;
            rect.y = 0;
            rect.width = CCWindow[u8WindowID].u16Width;
            rect.height = CCWindow[u8WindowID].u16Height;

            CCDispSetClipArea(pCCObj->sid,&rect,0);
            _DtvCC_ReDrawWindow(u8WindowID,1);
            CCDISP_Screen_DisableClip(pCCObj->sid);
        #endif
    }

    for(i = 0; i < s32ScrollRows; i++)
    {
        /* if the window is visible, block move the Row on screen */
        if(CCWindow[u8WindowID].u8IsVisible)
        {
            #if 0
            if(CCWindow[u8WindowID].as32RowHeight[0] == 0)
            {
                s32ScrollOffset = pCCObj->s32RowHeight;
                CCWindow[u8WindowID].as32RowHeight[0] = pCCObj->s32RowHeight;
            }
            else
            {
                s32ScrollOffset = CCWindow[u8WindowID].as32RowHeight[0];
            }
            #endif
            s32ScrollWidth = CCWindow[u8WindowID].u16Width;
            s32ScrollOffset = CCWindow[u8WindowID].u16Height / (CCWindow[u8WindowID].u8RowCount + 1);

            _DtvCC_BlockMove(u8WindowID,s32DstX,s32DstY,s32ScrollWidth,s32ScrollOffset,s32SrcX,s32SrcY);

        }

        /* update window attributes */
        for( s32Row = 0; s32Row < (CCWindow[u8WindowID].u8RowCount - i); s32Row++ )
        {
            /* update text in the Row */
            memcpy(CCWindow[u8WindowID].astText[s32Row],CCWindow[u8WindowID].astText[s32Row+1],MAX_CC_COLUMNS*sizeof(CC708_TextDef_S));

            /* update Row width and height */
            CCWindow[u8WindowID].as32RowWidth[s32Row] = CCWindow[u8WindowID].as32RowWidth[s32Row+1];
            CCWindow[u8WindowID].as32RowHeight[s32Row] = CCWindow[u8WindowID].as32RowHeight[s32Row+1];
        }

        /* clear the last Row */
        memset(CCWindow[u8WindowID].astText[s32Row],0,MAX_CC_COLUMNS*sizeof(CC708_TextDef_S));
        CCWindow[u8WindowID].as32RowWidth[s32Row] = 0;
        CCWindow[u8WindowID].as32RowHeight[s32Row] = 0;
    }

}

static inline HI_VOID _DtvCC_DisplayWindow(HI_U8 u8WindowID)
{
    HI_S32 s32Row,s32CurRow;
    CC708_Cursor_S *pstCursor;

    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    if(CCWindow[u8WindowID].u8IsVisible)
    {
        return;
    }

    CCWindow[u8WindowID].u8IsVisible = 1;
    (HI_VOID)CCDISP_Window_Show((CCDISP_WINDOW_ID_E)u8WindowID);

    /*here we begin timer auto erase time-out text*/
    _DtvCC_EraseTimerStart(DTVCC_ERASE_TEXT_TIME);

    /* clear window on the screen */
    _DtvCC_ClearWindowOnScreen(u8WindowID,0);

    pstCursor = &CCWindow[u8WindowID].stCursor;
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);

    OSD_TRACE("_DtvCC_DisplayWindow %d:s32CurRow= %d\n",u8WindowID,s32CurRow);

    for(s32Row = 0; s32Row <= s32CurRow; s32Row++)
    {
        /* calculate draw start position of current s32Row */
        _DtvCC_CalcDrawStartPositionByRow(u8WindowID,s32Row);

        /* draw text */
        _DtvCC_DrawRowText(u8WindowID,0,0,s32Row,HI_FALSE);
    }

    /* set the display order of this window to the last displayed window*/
    _DtvCC_SetLastDisplayOrder(u8WindowID);

}

static inline HI_VOID _DtvCC_InitWindow(HI_U8 u8WindowID)
{
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    memset(&CCWindow[u8WindowID],0,sizeof(CC708_Window_S));
    CCWindow[u8WindowID].s32WindowID = -1;
    CCWindow[u8WindowID].s32DisplayOrder = -1;
}

static inline HI_VOID _DtvCC_DrawCaptionToWindowOffScreen(HI_U16 *pu16Text,HI_S32 s32Len)
{
    HI_S32 i = CurrentWindowID;
    HI_S32 s32CurRow,s32CurColumn,s32MaxColumn;
    HI_S32 j;
    CC708_Cursor_S * pstCursor = &CCWindow[i].stCursor;

    /* firstly, put caption into window offscreen caption buffer */
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);
    s32CurColumn = CC708_Cursor_GetCurrentColumn(pstCursor);

    if( HI_UNF_CC_FONTSIZE_SMALL == CCWindow[i].stCurPen.enFontSize)
    {
        s32MaxColumn=(CCWindow[i].u8ColumnCount+1) * MAX_CC_COLUMNS_16B9 /MAX_CC_COLUMNS_4B3-1;
    }
    else if(HI_UNF_CC_FONTSIZE_LARGE == CCWindow[i].stCurPen.enFontSize)
    {
        s32MaxColumn=( CCWindow[i].u8ColumnCount+1) * MAX_CC_COLUMNS_4B3 / MAX_CC_COLUMNS_16B9-1;
    }
    else
    {
        s32MaxColumn=CCWindow[i].u8ColumnCount;
    }

    if(s32CurColumn > s32MaxColumn)
    {
        memset(pu16Text,0,s32Len*sizeof(HI_U16));
        return;
    }

    for(j = 0; j < s32Len; j++)
    {
        /* if characters beyond the last column,put them in the last column one by one to replace the former one*/
        if ((s32CurColumn + j) <= s32MaxColumn)
        {
            /* copy character into window caption buffer */
            CCWindow[i].astText[s32CurRow][s32CurColumn+j].u16Caption = pu16Text[j];

            /* copy pen attributes */
            memcpy(&CCWindow[i].astText[s32CurRow][s32CurColumn+j].stPen,&CCWindow[i].stCurPen,sizeof(CC708_PenDef_S));

            /* move cursor forwards */
            CC708_Cursor_MoveCursorForwardsOffScreen(pstCursor,1,CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);
        }
        else
        {
            /* copy character into window caption buffer */
            //CCWindow[i].astText[s32CurRow][CCWindow[i].u8ColumnCount].u16Caption = pu16Text[j];

            /* copy pen attributes */
            //memcpy(&CCWindow[i].astText[s32CurRow][CCWindow[i].u8ColumnCount].stPen,&CCWindow[i].stCurPen,sizeof(CC708_PenDef_S));

            /* remove the unused characters */
            if (j > ( s32MaxColumn - s32CurColumn))
            {
                pu16Text[j] = 0;
            }
        }
    }

}


static inline HI_VOID _DtvCC_DrawCaptionToWindowOnScreen(HI_U16 *pu16Buf, HI_S32 s32Len, HI_S32 s32Row, HI_S32 s32Column)
{
    HI_S32 WinID = CurrentWindowID;
    HI_S32 x, y;
    HI_S32 i, j;
    CC708_PenDef_S *pstPen;
    CC708_Cursor_S *pstCursor;
    HI_S32 s32TextWidth = 0, s32TextHeight = 0, s32CaptionCount = 0;
    HI_U16 u16SubBuf[MAX_CC_COLUMNS];
    HI_BOOL bIgnoreLeftBG = HI_FALSE;
    HI_S32 s32PreTextHeight =0, s32CurTextHeight = 0;

    CC708OSDPARAM_CHECK_RETURN_VOID(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID");

    _DtvCC_EraseTimerStart(DTVCC_ERASE_TEXT_TIME);

    OSD_TRACE("Draw caption OnScreen at Row %d Column %d\n",s32Row,s32Column);

    // find the last character height
    if(s32Column > 0)
    {
        for(i = s32Column-1; i >= 0; i--)
        {
            if(CCWindow[WinID].astText[s32Row][i].u8IsPenChanged)
            {
                CC708_PenDef_S curPen;
                HI_U16 space[1] = {0x20};

                memcpy(&curPen,&CCWindow[WinID].stCurPen,sizeof(CC708_PenDef_S));
                pstPen = &CCWindow[WinID].astText[s32Row][i].stPen;
                (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                        pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[WinID].u8IsVisible);

                _DtvCC_GetTextSize(WinID,space,1,&s32TextWidth, &s32CurTextHeight);

                pstPen = &curPen;
                (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                        pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[WinID].u8IsVisible);
                break;
            }
        }
    }

    pstCursor = &CCWindow[WinID].stCursor;
    pstPen = &CCWindow[WinID].astText[s32Row][s32Column].stPen;

    /* set font attrbites if pen has been changed */
    if(CCWindow[WinID].astText[s32Row][s32Column].u8IsPenChanged)
    {
        (HI_VOID)_DtvCC_SetCurrentPenColor(pstPen,pstPen->u32FGColor,pstPen->u8FGOpacity,pstPen->u32BGColor,pstPen->u8BGOpacity,
                                             pstPen->u32EdgeColor,CCWindow[WinID].u8IsVisible);
        (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[WinID].u8IsVisible);
    }

    /* if not left justified, calculate draw start position first */
    if((CCWindow[WinID].enTextJustify != HI_UNF_CC_JUSTIFY_LEFT)&&(s32Column == 0))
    {
        _DtvCC_CalcDrawStartPositionByRow(WinID,s32Row);
        //x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
        //y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
    }

    //x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
    //y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

    if(pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT)
    {
        if((s32Column == 0)||(CCWindow[WinID].astText[s32Row][s32Column-1].u16Caption == 0) ||
           (CCWindow[WinID].astText[s32Row][s32Column-1].u16Caption == 0x1020))
        {
            HI_BOOL bFirstColumn;

            if(s32Column == 0)
            {
                bFirstColumn = HI_TRUE;
                CCWindow[WinID].as32RowWidth[s32Row] += LEADINGSPACEWIDTH;
            }
            else
            {
                bFirstColumn = HI_FALSE;
            }

            x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            if((pu16Buf[0] == 0x1020) || (pu16Buf[0] == 0x1021))
            {
                _DtvCC_DrawLeadingSpace(WinID,x,y,1,bFirstColumn,HI_FALSE);
            }
            else
            {
                _DtvCC_DrawLeadingSpace(WinID,x,y,2,bFirstColumn,HI_FALSE);
            }

            if(bFirstColumn)
            {
                CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,LEADINGSPACEWIDTH,
                         CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
                //bFirstColumn = HI_FALSE;
            }
            bIgnoreLeftBG = HI_FALSE;
        }
        else
        {
            bIgnoreLeftBG = HI_FALSE;
        }
    }
    for(j = 0; j < s32Len; j++)
    {
        if((j == 0) && (pu16Buf[j] == 0x20) && pstPen->u8IsUnderline &&
           (pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT))
        {
            OSD_TRACE("The first underline character is space.");
            (HI_VOID)CC708_Pen_SetTextUnderline(pstPen,0,1);

            /* draw text */
            x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            OSD_TRACE("_DtvCC_DispText:x:%d,y:%d,s32CaptionCount:%d\n",x,y,s32CaptionCount);

            /* calculate text width and height */
            _DtvCC_GetTextSize(WinID,pu16Buf,1,&s32TextWidth, &s32TextHeight);
            s32PreTextHeight = s32CurTextHeight;
            s32CurTextHeight = s32TextHeight;

            // draw left bottom background when font size changes
            if((s32CurTextHeight > s32PreTextHeight) && (s32PreTextHeight > 0))
            {
                _DtvCC_FillRect(WinID,x,y+s32PreTextHeight,TAILINGSPACEWIDTH,s32CurTextHeight-s32PreTextHeight,2,
                                       CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_FALSE);
            }

            // if pen color changed, we can't ignore left background
            if(CCWindow[WinID].bPenColorChanged)
            {
                bIgnoreLeftBG = HI_FALSE;
                CCWindow[WinID].bPenColorChanged = HI_FALSE;
            }
            OSD_TRACE("_DtvCC_DispText:x:%d,y:%d,s32CaptionCount:%d\n",x,y,s32CaptionCount);

            _DtvCC_DispText(WinID,x,y,s32TextWidth,s32TextHeight,pu16Buf,1,CC_PD_LEFT_RIGHT,
                                 CC_SD_LEFT_RIGHT,HI_FALSE,bIgnoreLeftBG,HI_TRUE);

            bIgnoreLeftBG = HI_FALSE;

            /* move cursor */
            CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,s32TextWidth,
                 CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            /* calculate total width and max height until current Column */
            CCWindow[WinID].as32RowWidth[s32Row] += s32TextWidth;
            CCWindow[WinID].as32RowHeight[s32Row] = (s32TextHeight>CCWindow[WinID].as32RowHeight[s32Row])?s32TextHeight:CCWindow[WinID].as32RowHeight[s32Row];

            (HI_VOID)CC708_Pen_SetTextUnderline(pstPen,1,1);

            continue;
        }

        if(pu16Buf[j] == 0x1020)
        {
            HI_U16 space[1] = {0x20};

            if(s32CaptionCount>0)
            {
                if(pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT)
                {
                    /* draw text */
                    x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
                    y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

                    /* calculate text width and height */
                    _DtvCC_GetTextSize(WinID,u16SubBuf,s32CaptionCount,&s32TextWidth, &s32TextHeight);
                    s32PreTextHeight = s32CurTextHeight;
                    s32CurTextHeight = s32TextHeight;

                    // draw left bottom background when font size changes
                    if((s32CurTextHeight > s32PreTextHeight) && (s32PreTextHeight > 0))
                    {
                        _DtvCC_FillRect(WinID,x,y+s32PreTextHeight,TAILINGSPACEWIDTH,s32CurTextHeight-s32PreTextHeight,2,
                                         CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_FALSE);
                    }

                    // if pen color changed, we can't ignore left background
                    if(CCWindow[WinID].bPenColorChanged)
                    {
                        bIgnoreLeftBG = HI_FALSE;
                        CCWindow[WinID].bPenColorChanged = HI_FALSE;
                    }

                    OSD_TRACE("_DtvCC_DispText:x:%d,y:%d,s32CaptionCount:%d\n",x,y,s32CaptionCount);
                    _DtvCC_DispText(WinID,x,y,s32TextWidth,s32TextHeight,u16SubBuf,s32CaptionCount,
                        CC_PD_LEFT_RIGHT,CC_SD_LEFT_RIGHT,HI_FALSE,bIgnoreLeftBG,HI_TRUE);

                    bIgnoreLeftBG = HI_FALSE;

                    /* move cursor */
                    CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,s32TextWidth,
                       CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

                    /* calculate total width and max height until current s32Column */
                    CCWindow[WinID].as32RowWidth[s32Row] += s32TextWidth;
                    CCWindow[WinID].as32RowHeight[s32Row] = (s32TextHeight>CCWindow[WinID].as32RowHeight[s32Row])?s32TextHeight:CCWindow[WinID].as32RowHeight[s32Row];
                }

                s32CaptionCount = 0;
            }

            /* draw text */
            x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            _DtvCC_GetTextSize(WinID,space,1,&s32TextWidth, &s32TextHeight);
            _DtvCC_DrawTSP(x,y,HI_FALSE);

            /* move cursor */
            CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,s32TextWidth,
               CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            CCWindow[WinID].as32RowWidth[s32Row] += s32TextWidth;

        }
        else
        {
            if(((j > 1) && (pu16Buf[j-1] == 0)) ||
               ((j > 1) && (pu16Buf[j-1] == 0x1020)))
            {
                x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
                y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
                _DtvCC_DrawLeadingSpace(WinID,x,y,2,HI_FALSE,HI_FALSE);
            }
            else
            {
            }
            u16SubBuf[s32CaptionCount++] = pu16Buf[j];
        }
    }

    // draw the remain caption
    if(s32CaptionCount>0)
    {
        if(pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT)
        {
            /* draw text */
            x = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);


            /* calculate text width and height */
            _DtvCC_GetTextSize(WinID,u16SubBuf,s32CaptionCount,&s32TextWidth, &s32TextHeight);
            s32PreTextHeight = s32CurTextHeight;
            s32CurTextHeight = s32TextHeight;

            // draw left bottom background when font size changes
            if((s32CurTextHeight > s32PreTextHeight) && (s32PreTextHeight > 0))
            {
                _DtvCC_FillRect(WinID,x,y+s32PreTextHeight,TAILINGSPACEWIDTH,s32CurTextHeight-s32PreTextHeight,2,
                                       CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_FALSE);
            }

            // if pen color changed, we can't ignore left background
            if(CCWindow[WinID].bPenColorChanged)
            {
                bIgnoreLeftBG = HI_FALSE;
                CCWindow[WinID].bPenColorChanged = HI_FALSE;
            }

            OSD_TRACE("_DtvCC_DispText:x:%d,y:%d,s32CaptionCount:%d,char:%c\n",x,y,s32CaptionCount,u16SubBuf[0]);
            _DtvCC_DispText(WinID,x,y,s32TextWidth,s32TextHeight,u16SubBuf,s32CaptionCount,
                          CC_PD_LEFT_RIGHT,CC_SD_LEFT_RIGHT,HI_FALSE,bIgnoreLeftBG,HI_TRUE);

            /* move cursor */
            CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,s32TextWidth,
                  CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            /* calculate total width and max height until current s32Column */
            CCWindow[WinID].as32RowWidth[s32Row] += s32TextWidth;
            CCWindow[WinID].as32RowHeight[s32Row] = (s32TextHeight>CCWindow[WinID].as32RowHeight[s32Row])?s32TextHeight:CCWindow[WinID].as32RowHeight[s32Row];
        }

        //s32CaptionCount = 0;
    }

    if(CCWindow[WinID].bAddedTailingSpaceWidth[s32Row] == HI_FALSE)
    {
        CCWindow[WinID].as32RowWidth[s32Row] += TAILINGSPACEWIDTH;
        CCWindow[WinID].bAddedTailingSpaceWidth[s32Row] = HI_TRUE;
    }
}

/*hide window means ,if visible ,clear the text in the window on screen,and fill the window
with screen color.Do not clear the text buffer in the window*/
static inline HI_VOID  _DtvCC_HideWindow(HI_U8 WindowID)
{
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(WindowID,MAX_CC_WINDOWS,"u8WindowID");
    if((!CCWindow[WindowID].u8IsVisible) || (CCWindow[WindowID].u8IsEmpty == 1))
    {
        /* set window visibility */
        (HI_VOID)CCDISP_Window_Hide((CCDISP_WINDOW_ID_E)WindowID);
        CCWindow[WindowID].u8IsVisible = 0;
        return;
    }
    /* clear window on the screen */
    _DtvCC_ClearWindowOnScreen(WindowID,1);
    _DtvCC_SetDefaultDisplayOrder(WindowID);
    (HI_VOID)CCDISP_Window_Hide((CCDISP_WINDOW_ID_E)WindowID);
    CCWindow[WindowID].u8IsVisible = 0;
}

static inline HI_VOID _DtvCC_DispText(HI_U8 u8WindowID, HI_S32 x, HI_S32 y,HI_S32 s32Width, HI_S32 s32Height,
                                          HI_U16 *pu16TextBuf, HI_S32 s32TextNum,CC708_PrintDirection_E enPrintDirection,
                                          CC708_ScrollDirection_E enScrollDirection,HI_BOOL bOffScreen,
                                           HI_BOOL bIgnoreLeftBG,HI_BOOL bTailingSpace)
{
    (HI_VOID)CCDISP_Text_Draw((CCDISP_WINDOW_ID_E)u8WindowID, x, y, pu16TextBuf, s32TextNum, HI_NULL,2);
}


/*isClear: 0---the window still visible,fill with window color
               1---make the window invisible,fill with screen color*/
static inline HI_VOID  _DtvCC_ClearWindowOnScreen(HI_U8 u8WindowID,HI_U8 isClear)
{
    HI_S32 overlapWindowMap1,overlapWindowMap2;
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    OSD_TRACE("u8WindowID:%d,window color:0x%x,opacity:%d\n",u8WindowID,CCWindow[u8WindowID].u32FillColor,CCWindow[u8WindowID].u32FillOpacity);

    // update window overlap map
    CCWindow[u8WindowID].s32OverlapLowerWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,0);
    CCWindow[u8WindowID].s32OverlapHigherWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,1);

    if(CCWindow[u8WindowID].s32OverlapHigherWindowMap)
    {
        #if 0
        CC708SearchWindowClipArea(u8WindowID);
        #endif
    }

    overlapWindowMap1 = CCWindow[u8WindowID].s32OverlapHigherWindowMap;
    overlapWindowMap2 = CCWindow[u8WindowID].s32OverlapLowerWindowMap;

    // if there are windows under current window
    if(isClear && (overlapWindowMap2 != 0))
    {
        HI_S32 i;
        HI_BOOL bReDraw = HI_FALSE;

        for(i = 0; i < 8; i++)
        {
            if((overlapWindowMap2 & (1<<i)) &&(CCWindow[i].u8IsVisible))
            {
                // update window overlap map
                CCWindow[i].s32OverlapLowerWindowMap = _DtvCC_CheckWindowOverlap(i,0);
                CCWindow[i].s32OverlapHigherWindowMap = _DtvCC_CheckWindowOverlap(i,1);

                if(CCWindow[i].s32OverlapHigherWindowMap)
                {
                    #if 0
                    CC708SearchWindowClipArea(i);
                    #endif
                }

                _DtvCC_ReDrawWindow(i,HI_FALSE);
                bReDraw = HI_TRUE;
            }
        }

        // recover window status after redraw other windows
        if(bReDraw)
        {
            /*recover window color*/
            (HI_VOID)_DtvCC_SetWindowColor(u8WindowID,CCWindow[u8WindowID].u32FillColor,CCWindow[u8WindowID].u32FillOpacity);

            /*recover pen attribute*/
            (HI_VOID)_DtvCC_SetCurrentPenAttributes(&CCWindow[u8WindowID].stCurPen,
                                    CCWindow[u8WindowID].stCurPen.enFontName,
                                    CCWindow[u8WindowID].stCurPen.enFontSize,
                                    CCWindow[u8WindowID].stCurPen.u8IsItalic,
                                    CCWindow[u8WindowID].stCurPen.u8IsUnderline,
                                    CCWindow[u8WindowID].stCurPen.enEdgeType,
                                    CCWindow[u8WindowID].stCurPen.enTextOffset,
                                    CCWindow[u8WindowID].stCurPen.enTextTag,
                                    1);
            /*recover pen color*/
            (HI_VOID)_DtvCC_SetCurrentPenColor(&CCWindow[u8WindowID].stCurPen,
                               CCWindow[u8WindowID].stCurPen.u32FGColor,
                               CCWindow[u8WindowID].stCurPen.u8FGOpacity,
                               CCWindow[u8WindowID].stCurPen.u32BGColor,
                               CCWindow[u8WindowID].stCurPen.u8BGOpacity,
                               CCWindow[u8WindowID].stCurPen.u32EdgeColor,1);
        }
    }

    if((overlapWindowMap1 == 0) && (overlapWindowMap2 == 0))
    {
        //we need to find the widest row width to fillrect, otherwise we cannot erase completely
        HI_S32 i, width, height;
        width = CCWindow[u8WindowID].u16Width;
        height = CCWindow[u8WindowID].u16Height;
        for(i = 0; i <=  CCWindow[u8WindowID].u8RowCount; i++)
        {
            width = (width > CCWindow[u8WindowID].as32RowWidth[i]) ? width : CCWindow[u8WindowID].as32RowWidth[i];
        }


        _DtvCC_FillRect(u8WindowID,CCWindow[u8WindowID].u16X,CCWindow[u8WindowID].u16Y,width,
                        height,isClear,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_FALSE);

    }

}

static inline HI_VOID _DtvCC_DeleteWindow(HI_U8 WindowID)
{
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(WindowID,MAX_CC_WINDOWS,"u8WindowID");

    OSD_TRACE("_DtvCC_DeleteWindow ======   %d\n",WindowID);

    CCDISP_Text_DeleteCharFlash(WindowID);
    if(CCWindow[WindowID].u8IsVisible)
    {
        _DtvCC_ClearWindowOnScreen(WindowID,1);
        _DtvCC_SetDefaultDisplayOrder(WindowID);
        CCWindow[WindowID].u8IsVisible = 0;
    }

    /* set window to default state */
    _DtvCC_InitWindow(WindowID);
    CCDISP_Window_Destroy((CCDISP_WINDOW_ID_E)WindowID);
}


static inline HI_VOID _DtvCC_ClearWindowOffScreen(HI_U8 WindowID)
{
    /* clear the text of the window */
    memset(CCWindow[WindowID].astText,0,sizeof(CC708_TextDef_S)*MAX_CC_ROWS*MAX_CC_COLUMNS);

    /* Move the cursor to the left-top corner of the window*/
    /* Spec does not mention this, however, we should add this for following text drawing */
    CC708_Cursor_ResetToOrigin(&CCWindow[WindowID],&CCWindow[WindowID].stCursor,
         CCWindow[WindowID].enTextPrintDirection,CCWindow[WindowID].enTextScrollDirection);

    /* clear the text width and height attributes */
    memset(CCWindow[WindowID].as32RowWidth, 0, MAX_CC_ROWS*sizeof(HI_S32));
    memset(CCWindow[WindowID].as32RowHeight, 0, MAX_CC_ROWS*sizeof(HI_S32));
}

/*clear window means just clear the text in the window,
and if visible ,the window will filled with window color and keep visible*/
static inline HI_VOID _DtvCC_ClearWindow(HI_U8 WindowID)
{
    _DtvCC_ClearWindowOffScreen(WindowID);/* clear the off screen buffer of the window */
    CCDISP_Window_Clear((CCDISP_WINDOW_ID_E)WindowID);

    /* if the window is visible, fill the window with window fill color */
    if( CCWindow[WindowID].u8IsVisible )
    {
       _DtvCC_ClearWindowOnScreen(WindowID,0);
    }
}


/*************************************************************************
 * function name: _DtvCC_GetYPosByRow
 * description  : we maintain a variable of text height for each row,in the
 *                before we regard each row as pCCObj->s32RowHeight, so now we
 *                add this new function to calculat the y pos with current row
 * arguments    : u8WindowID 0-7
 *                       curRow   0-3
 * return value : y coordinate of the screen
 ************************************************************************/
static inline HI_S32 _DtvCC_GetYPosByRow(HI_U8 u8WindowID, HI_U8 curRow)
{
    CC708_OBJECT_S *pCCObj= CC708_OBJ_GetHandle(0);
    HI_U8 i = 0;
    HI_S32 y = 0;

    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(u8WindowID,MAX_CC_WINDOWS,"u8WindowID",-1);
    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(curRow,CCWindow[u8WindowID].u8RowCount+2,"row",-1);

    if(curRow <= CCWindow[u8WindowID].u8RowCount)
    {
        /* calculate from window top position */
        y = CCWindow[u8WindowID].u16Y + INTERLINE_HEIGTH;

        /* calculate current row draw start y position */
        for( i = 0; i < curRow; i++)
        {
            y += INTERLINE_HEIGTH;
            if(CCWindow[u8WindowID].as32RowHeight[i] == 0)
            {
                CCWindow[u8WindowID].as32RowHeight[i] = pCCObj->s32RowHeight;
                y += pCCObj->s32RowHeight;
            }
            else
            {
                y += CCWindow[u8WindowID].as32RowHeight[i];
            }
        }

    }
    else
    {
        /* the window bottom position */
        y = CCWindow[u8WindowID].u16Y + CCWindow[u8WindowID].u16Height;
    }

    /* sanity check */
    if( y > CCWindow[u8WindowID].u16Y + CCWindow[u8WindowID].u16Height )
    {
        y = CCWindow[u8WindowID].u16Y + CCWindow[u8WindowID].u16Height;
    }

    return y;
}

static inline HI_VOID _DtvCC_GetTextSize(HI_S32 u8WindowID,HI_U16 *buf,
                                 HI_U8 size, HI_S32 *w, HI_S32 *h)
{
    (HI_VOID)CCDISP_Text_GetSize((CCDISP_WINDOW_ID_E)u8WindowID, buf, size, w, h);
}

static inline HI_VOID _DtvCC_ClearRowOnScreen(HI_U8 u8WindowID, HI_U8 u8Row)
{
    CC708_OBJECT_S *pCCObj= CC708_OBJ_GetHandle(0);
    HI_S32 y,height,width;

    if(CCWindow[u8WindowID].as32RowWidth[u8Row] == 0)
    {
        CCWindow[u8WindowID].as32RowWidth[u8Row] = CCWindow[u8WindowID].u16Width;
    }

    if(CCWindow[u8WindowID].as32RowHeight[u8Row] == 0)
    {
        CCWindow[u8WindowID].as32RowHeight[u8Row] = pCCObj->s32RowHeight;
    }

    height = CCWindow[u8WindowID].as32RowHeight[u8Row];

    /*get the maximun of TextWidth and window Width*/
    width = CCWindow[u8WindowID].u16Width;
    if( width < CCWindow[u8WindowID].as32RowWidth[u8Row] )
    {
        width = CCWindow[u8WindowID].as32RowWidth[u8Row];
    }

    y = _DtvCC_GetYPosByRow(u8WindowID,u8Row);

    /*adapt the rect position to make sure the row can be cleared completely*/
    y -= (INTERLINE_HEIGTH/2);
    height += INTERLINE_HEIGTH;

    /* clear the row on the screen */
    _DtvCC_FillRect(u8WindowID,CCWindow[u8WindowID].u16X,y,width,height,0,
             CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection,HI_FALSE);

}


static inline HI_VOID _DtvCC_ClearRowOffScreen(HI_U8 u8WindowID, HI_U8 u8Row)
{
    /* clear row attributes */
    CCWindow[u8WindowID].as32RowHeight[u8Row] = 0;
    CCWindow[u8WindowID].as32RowWidth[u8Row]  = 0;

    /* clear texts in current row */
    memset(CCWindow[u8WindowID].astText[u8Row],0,MAX_CC_COLUMNS * sizeof(CC708_TextDef_S));
}

/**********************************************************************************
 * function name: ClearRow
 * description  : clear the specified rows's text of the specified u8WindowID
 * arguments    : u8WindowID,
 *                     u8Row:  the row number to clear
 * return       :
 * creation     :
 **********************************************************************************/
static inline HI_VOID _DtvCC_ClearRow(HI_U8 u8WindowID, HI_U8 u8Row)
{
    if(CCWindow[u8WindowID].u8IsVisible)
    {
        _DtvCC_ClearRowOnScreen(u8WindowID,u8Row);
    }

    _DtvCC_ClearRowOffScreen(u8WindowID,u8Row);
}


static inline HI_VOID _DtvCC_CalcDrawStartPositionByRow(HI_U8 u8WindowID, HI_S32 s32Row)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_S32 s32TextWidth = 0,s32TotalWidth,s32SpaceWidth,s32TextHeight,s32CharCount = 0;
    HI_U16 buf[MAX_CC_COLUMNS];
    CC708_PenDef_S * pstPen;
    HI_S32 i, j;

    OSD_TRACE("function:%s,Window justify:%d\n",__FUNCTION__,CCWindow[u8WindowID].enTextJustify);

    CC708_Cursor_MoveCursorOnScreen(&CCWindow[u8WindowID].stCursor,CCWindow[u8WindowID].u16X,_DtvCC_GetYPosByRow(u8WindowID,s32Row),
                                    CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);

    if(CCWindow[u8WindowID].enTextJustify != HI_UNF_CC_JUSTIFY_LEFT)
    {
        /* set initial value */
        s32CharCount = 0;
        s32TotalWidth = 0;
        s32SpaceWidth = 0;

        /* set initial font attributes at the beginning */
        if(CCWindow[u8WindowID].astText[s32Row][0].u8IsPenChanged)
        {
            pstPen = &CCWindow[u8WindowID].astText[s32Row][0].stPen;
            (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                    pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
        }

        for(j = 0;j < MAX_CC_COLUMNS;j++)
        {
            if(CCWindow[u8WindowID].astText[s32Row][j].u8IsPenChanged ||
               (CCWindow[u8WindowID].astText[s32Row][j].u16Caption == 0) ||
               (CCWindow[u8WindowID].astText[s32Row][j].u16Caption == 0x1020))
            {
                if( s32CharCount > 0 )
                {
                    /* calculate text width and height */
                    _DtvCC_GetTextSize(u8WindowID,buf,s32CharCount,&s32TextWidth, &s32TextHeight);

                    /* calculate total width until current j */
                    s32TotalWidth += s32SpaceWidth;
                    s32TotalWidth += s32TextWidth;
                    s32SpaceWidth = 0;
                }

                 /* if there is TSP in this grid, take it as a space */
                if(CCWindow[u8WindowID].astText[s32Row][j].u16Caption == 0x1020)
                {
                    HI_U16 space[1] = {0x20};

                    _DtvCC_GetTextSize(u8WindowID,space,1,&s32TextWidth, &s32TextHeight);

                    s32SpaceWidth += s32TextWidth;
                }

                /* if there is no caption in this grid, skip a j */
                if(CCWindow[u8WindowID].astText[s32Row][j].u16Caption == 0)
                {
                    s32SpaceWidth += pCCObj->s32MaxCharWidth;
                }

                /* initial char count of the next text to zero */
                s32CharCount = 0;

                /* if pen changed, set font attributes for next text */
                if(CCWindow[u8WindowID].astText[s32Row][j].u8IsPenChanged)
                {
                    pstPen = &CCWindow[u8WindowID].astText[s32Row][j].stPen;
                    (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                                pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
                }
            }

            /* group characters into text buffer */
            if((CCWindow[u8WindowID].astText[s32Row][j].u16Caption != 0) &&
               (CCWindow[u8WindowID].astText[s32Row][j].u16Caption != 0x1020))
            {
                buf[s32CharCount++] = CCWindow[u8WindowID].astText[s32Row][j].u16Caption;
            }
        }

        /* get last text width and height */
        if(s32CharCount>0)
        {
            /* calculate text width and height */
            _DtvCC_GetTextSize(u8WindowID,buf,s32CharCount,&s32TextWidth,&s32TextHeight);

            /* calculate total width until current j */
            s32TotalWidth += s32SpaceWidth;
            s32TotalWidth += s32TextWidth;
        }

        s32TotalWidth += LEADINGSPACEWIDTH;
        s32TotalWidth += TAILINGSPACEWIDTH;
        OSD_TRACE("Text width:%d\n",s32TotalWidth);

        if(CCWindow[u8WindowID].enTextJustify == HI_UNF_CC_JUSTIFY_CENTER)
        {
            CC708_Cursor_MoveCursorForwardsOnScreen(&CCWindow[u8WindowID].stCursor,(CCWindow[u8WindowID].u16Width - s32TotalWidth)>>1,
                                                   CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
            for(i=0; i<(CCWindow[u8WindowID].u8RowCount+1); i++)
            {
                CCWindow[u8WindowID].as32RowWidth[i] = 0;//CCWindow[u8WindowID].leftMargin;
            }
        }
        else if(CCWindow[u8WindowID].enTextJustify == HI_UNF_CC_JUSTIFY_RIGHT)
        {
            CC708_Cursor_MoveCursorForwardsOnScreen(&CCWindow[u8WindowID].stCursor,(CCWindow[u8WindowID].u16Width - s32TotalWidth),
                                                   CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
            for(i=0; i<(CCWindow[u8WindowID].u8RowCount+1); i++)
            {
                CCWindow[u8WindowID].as32RowWidth[i] = 0;//CCWindow[u8WindowID].leftMargin;
            }
        }
        else
        {
            //CCWindow[u8WindowID].leftMargin = 0;
        }
    }
}


static HI_VOID _DtvCC_DrawLeadingSpace(HI_U8 u8WindowID, HI_S32 x, HI_S32 y, HI_S32 isClear, HI_BOOL bFisrtColumn, HI_BOOL bOffScreen)

{
    HI_S32 s32Width = 0, s32SpaceWidth = 0, s32Height = 0;
    HI_U16 space[1] = {0x20};

    _DtvCC_GetTextSize(u8WindowID,space,1,&s32SpaceWidth,&s32Height);

    if(s32SpaceWidth > LEADINGSPACEWIDTH)
    {
        s32SpaceWidth = LEADINGSPACEWIDTH;
    }

    /**
    *  note: In case of 0,0x1020,1021, just use space s32Width as the leading space with
    *  This can avoid leading space erase characters before 0,0x1020 and 0x1021
    */
    s32Width = LEADINGSPACEWIDTH;

    if(!bFisrtColumn)
    {
        x -= s32SpaceWidth;
    }

    _DtvCC_FillRect(u8WindowID,x,y,s32Width,s32Height,isClear,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen);
}

static HI_VOID _DtvCC_ClearLeadingSpace(HI_U8 u8WindowID)
{
    HI_S32 s32X,s32Y,s32TextWidth,s32TextHeight = 0;
    CC708_Cursor_S *pstCursor = &CCWindow[u8WindowID].stCursor;
    unsigned short space[1] = {0x20};

    s32X = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
    s32Y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);

    _DtvCC_GetTextSize(u8WindowID,space,1,&s32TextWidth,&s32TextHeight);
    s32TextWidth = LEADINGSPACEWIDTH;

    _DtvCC_FillRect(u8WindowID,s32X-s32TextWidth,s32Y,s32TextWidth,s32TextHeight,1,
             CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection,HI_FALSE);

    CC708_Cursor_MoveCursorBackwardsOnScreen(pstCursor,s32TextWidth,
                                           CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
}

static inline HI_VOID _DtvCC_MoveCursorByRowAndColumn(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column)
{
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    OSD_TRACE("function:%s,line:%d,row:%d,column:%d\n",__FUNCTION__,__LINE__,s32Row,s32Column);

    if(CCWindow[u8WindowID].u8IsVisible)
    {
        _DtvCC_MoveCursorByRowAndColumnOnScreen(u8WindowID,s32Row,s32Column);
    }

    _DtvCC_MoveCursorByRowAndColumnOffScreen(u8WindowID,s32Row,s32Column);
}


static inline HI_VOID _DtvCC_MoveCursorByRowAndColumnOnScreen(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_S32 i;
    HI_S32 s32CharCount = 0, s32TextWidth = 0,s32TextHeight = 0;
    HI_S32 s32TotalWidth,x,y;
    HI_U16 buf[MAX_CC_COLUMNS];
    CC708_PenDef_S *pstPen;
    CC708_Cursor_S *pstCursor;

    /* calculate draw start position of current s32Row */
    _DtvCC_CalcDrawStartPositionByRow(u8WindowID,s32Row);

    /* calculete draw position of current s32Column */

    /* Set initial value */
    s32CharCount = 0;
    s32TotalWidth = 0;

    /* set initial font attributes at the beginning */
    if(CCWindow[u8WindowID].astText[s32Row][0].u8IsPenChanged)
    {
        pstPen = &CCWindow[u8WindowID].astText[s32Row][0].stPen;

        (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                              pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
    }

    for(i = 0; i < s32Column; i++)
    {
        if( CCWindow[u8WindowID].astText[s32Row][i].u8IsPenChanged ||
            (CCWindow[u8WindowID].astText[s32Row][i].u16Caption == 0) ||
            (CCWindow[u8WindowID].astText[s32Row][i].u16Caption == 0x1020))
        {

            if( s32CharCount > 0 )
            {
                /* calculate text width and height */
                _DtvCC_GetTextSize(u8WindowID,buf,s32CharCount,&s32TextWidth, &s32TextHeight);

                /* calculate total width */
                s32TotalWidth += s32TextWidth;
            }

            /* initial char count of the next text to zero */
            s32CharCount = 0;

            /* if there is TSP in this grid, take it as a space */
            if(CCWindow[u8WindowID].astText[s32Row][i].u16Caption == 0x1020)
            {
                HI_U16 space[1] = {0x20};

                _DtvCC_GetTextSize(u8WindowID,space,1,&s32TextWidth, &s32TextHeight);

                s32TotalWidth += s32TextWidth;
            }

            /* if there is no caption in this grid, skip a s32Column */
            if(CCWindow[u8WindowID].astText[s32Row][i].u16Caption == 0)
            {
                s32TotalWidth += pCCObj->s32MaxCharWidth;
            }

            /* if pen changed, set font attributes for next text */
            if(CCWindow[u8WindowID].astText[s32Row][i].u8IsPenChanged)
            {
                OSD_TRACE("Pen changed at s32Row %d, s32Column %d\n",s32Row,i);
                pstPen = &CCWindow[u8WindowID].astText[s32Row][i].stPen;
                (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,(HI_U8)pstPen->enFontName,(HI_U8)pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                        pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
            }
        }

        /* group characters into text buffer, increase character count */
        if((CCWindow[u8WindowID].astText[s32Row][i].u16Caption != 0) &&
           (CCWindow[u8WindowID].astText[s32Row][i].u16Caption != 0x1020))
        {
            buf[s32CharCount++] = CCWindow[u8WindowID].astText[s32Row][i].u16Caption;
        }
    }

    /* get last text width */
    if(s32CharCount!=0)
    {
        /* calculate text width and height */
        _DtvCC_GetTextSize(u8WindowID,buf,s32CharCount,&s32TextWidth, &s32TextHeight);

        /* calculate total width and max height until current s32Column */
        s32TotalWidth += s32TextWidth;
    }
    if(s32Column != 0)
    {
        s32TotalWidth += LEADINGSPACEWIDTH;
    }

    /* move cursor to current s32Column */
    OSD_TRACE("s32TotalWidth:%d\n",s32TotalWidth);
    x = CCWindow[u8WindowID].u16X + s32TotalWidth;
    y = _DtvCC_GetYPosByRow(u8WindowID,s32Row);

    pstCursor = &CCWindow[u8WindowID].stCursor;

    CC708_Cursor_MoveCursorOnScreen(pstCursor,x,y,
                   CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);

    /* set s32Row width and height */
    CCWindow[u8WindowID].as32RowWidth[s32Row] = s32TotalWidth;
}


static inline HI_VOID _DtvCC_MoveCursorByRowAndColumnOffScreen(HI_U8 u8WindowID, HI_S32 s32Row, HI_S32 s32Column)
{
    CC708_Cursor_S *pstCursor = &CCWindow[u8WindowID].stCursor;

    CC708_Cursor_MoveCursorOffScreen(pstCursor,s32Row,s32Column,
                              CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
}

static HI_VOID _DtvCC_EraseTimerCallback(HI_U32 U32Param)
{
    (HI_VOID)CCDISP_Screen_Clear();

}

HI_S32 _DtvCC_EraseTimerInit(HI_S32 s32ModuleID)
{
   (HI_VOID)CCTimer_Open(CCTIMER_ID_708ERASE, _DtvCC_EraseTimerCallback, (HI_U32)s32ModuleID);
    return HI_SUCCESS;
}

static HI_VOID _DtvCC_EraseTimerStart(HI_U8 seconds)
{
    HI_U32 u32Msec = seconds * 1000;
   (HI_VOID)CCTimer_Start(CCTIMER_ID_708ERASE, u32Msec, TIMER_MODE_ONE_SHOOT);
}

HI_VOID _DtvCC_EraseTimerStop(HI_VOID)
{
    (HI_VOID)CCTimer_Stop(CCTIMER_ID_708ERASE);
}


/**********************************************************************************************************************
*                                   Interpretation Layer Decoder Function Definitions
**********************************************************************************************************************/

/********************************************internal APIs***********************************************/
static inline HI_VOID _DtvCC_SetLastDisplayOrder(HI_U8 u8WindowID)
{
    HI_S32 i,s32MaxOrder = -1;
    CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(u8WindowID,MAX_CC_WINDOWS,"u8WindowID");

    for(i = 0;i < 8;i++)
    {
        if((CCWindow[i].s32WindowID != -1) &&
           (CCWindow[i].s32DisplayOrder != -1) &&
           (CCWindow[i].s32DisplayOrder > CCWindow[u8WindowID].s32DisplayOrder))
        {
            if(CCWindow[i].s32DisplayOrder > s32MaxOrder)
            {
                 s32MaxOrder = CCWindow[i].s32DisplayOrder;
            }

            if(CCWindow[u8WindowID].s32DisplayOrder != -1)
            {
                CCWindow[i].s32DisplayOrder -= 1;
            }
        }
    }

    if(s32MaxOrder == -1)
    {
        CCWindow[u8WindowID].s32DisplayOrder = 0;
    }
    else if(CCWindow[u8WindowID].s32DisplayOrder == -1)
    {
        CCWindow[u8WindowID].s32DisplayOrder = s32MaxOrder + 1;
    }
    else
    {
        CCWindow[u8WindowID].s32DisplayOrder = s32MaxOrder;
    }

}


/**********************************************************************************
 * function name: _DtvCC_CalcWinPosByAnchorPoint(HI_U8 u8WindowID)
 * description  : calculate the window position(translate the anchor point to the
 *                     window coordinate)
 * arguments    : u8WindowID
 * creation     :
 *********************************************************************************/
static inline HI_BOOL _DtvCC_CalcWinPosByAnchorPoint(HI_U8 u8WindowID)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);
    HI_S32 MaxHorizonalAnchor = 210;
    HI_S32 MaxVerticalAnchor = 75;

    HI_U16 StaX,StaY,StaWidth,StaHeight;
    HI_S32 OriginalX,OriginalY,OriginalWidth,OriginalHeight;
    HI_S32 WinX,WinY,WinWidth,WinHeight;

    HI_S32 s32CurRow,s32CurColumn;
    CC708_Cursor_S * pstCursor;
    HI_S32 i, s32MaxStaWidth = 0;
    HI_BOOL bWindowChanged = HI_FALSE;
    HI_S32 s32SpaceWidth = 0, s32SpaceHeight = 0;
    HI_U16 space[1] = {0x20};

    pstCursor = &CCWindow[u8WindowID].stCursor;
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);
    s32CurColumn = CC708_Cursor_GetCurrentColumn(pstCursor);

    /* save original size and position */
    OriginalX = CCWindow[u8WindowID].u16X;
    OriginalY = CCWindow[u8WindowID].u16Y;
    OriginalWidth = CCWindow[u8WindowID].u16Width;
    OriginalHeight = CCWindow[u8WindowID].u16Height;

    if(CCWindow[u8WindowID].u8RelativePositioning)
    {
        /*relative position percentage*/
        MaxHorizonalAnchor = 100;
        MaxVerticalAnchor = 100;
    }
    else
    {
        MaxVerticalAnchor = 75;
        MaxHorizonalAnchor = pCCObj->stScreen.u16MaxHorizonalCells;
    }

    // adjust MaxHorizonalAnchor and MaxVerticalAnchor
    if( CCWindow[u8WindowID].u8AnchorHorizontal > MaxHorizonalAnchor )
    {
        CCWindow[u8WindowID].u8AnchorHorizontal = MaxHorizonalAnchor;
    }

    if( CCWindow[u8WindowID].u8AnchorVertical > MaxVerticalAnchor )
    {
        CCWindow[u8WindowID].u8AnchorVertical = MaxVerticalAnchor;
    }

    _DtvCC_GetTextSize(u8WindowID,space,1,&s32SpaceWidth,&s32SpaceHeight);
    pCCObj->s32RowHeight = s32SpaceHeight;

    if((s32CurRow == 0) && (s32CurColumn == 0))
    {
        /* clear the text Width and Height attributes */
        memset(CCWindow[u8WindowID].as32RowWidth, 0, MAX_CC_ROWS*sizeof(int));
        memset(CCWindow[u8WindowID].as32RowHeight, 0, MAX_CC_ROWS*sizeof(int));
    }

    /*caption screen(safe title area) size*/
    StaX = pCCObj->stScreen.CaptionRect.x;
    StaY = pCCObj->stScreen.CaptionRect.y;
    StaWidth = pCCObj->stScreen.CaptionRect.width;
    StaHeight = pCCObj->stScreen.CaptionRect.height;

    OSD_TRACE("StaX = %d, StaY = %d, StaWidth = %d, StaHeight = %d\n",StaX,StaY,StaWidth,StaHeight);
    OSD_TRACE("MaxHorizonalAnchor=%d,MaxVerticalAnchor=%d\n",MaxHorizonalAnchor,MaxVerticalAnchor);

    WinX = CCWindow[u8WindowID].u8AnchorHorizontal * StaWidth/MaxHorizonalAnchor + StaX;
    WinY = CCWindow[u8WindowID].u8AnchorVertical * StaHeight/MaxVerticalAnchor + StaY;
    WinWidth = (CCWindow[u8WindowID].u8ColumnCount + 1) * pCCObj->s32MaxCharWidth + LEADINGSPACEWIDTH + TAILINGSPACEWIDTH;
    WinHeight = 0;

    /*If the resulting size of any window is larger than the safe title area for the corresponding display's aspect ratio,
         then this window will be completely disregarded,Spec Page 59*/
    if(StaWidth < WinWidth)
    {
        HI_INFO_CC("Window width is larger than safe title area width,disregard this Window!\n");
        _DtvCC_InitWindow(u8WindowID);
        CC708_OSD_SetCurrentWindow(u8WindowID);
        return bWindowChanged;
    }

    /*we add a INTERLINE_HEIGTH to enlarge the window heigth to make sure clear the window completely */
    WinHeight += INTERLINE_HEIGTH;
    for(i=0; i<(CCWindow[u8WindowID].u8RowCount + 1); i++)
    {
        WinHeight += INTERLINE_HEIGTH;
        if(CCWindow[u8WindowID].as32RowHeight[i] > s32SpaceHeight)
        {
            WinHeight += CCWindow[u8WindowID].as32RowHeight[i];
        }
        else
        {
            WinHeight += s32SpaceHeight;
        }

        /*if row width > window width but is smaller than Max Sta Width(when 1920 *1080) i.e:1536*/
        if((CCWindow[u8WindowID].as32RowWidth[i] > WinWidth) && (CCWindow[u8WindowID].as32RowWidth[i] < 1536))
        {
            WinWidth = CCWindow[u8WindowID].as32RowWidth[i];
        }
    }

    /*get the real position by anchor point,see spec[cea_708b],page 27*/
    switch( CCWindow[u8WindowID].u8AnchorPoint )
    {
        case 0:
            break;
        case 1:
            WinX = WinX - WinWidth / 2 ;
            break;
        case 2:
            WinX = WinX - WinWidth;
            break;
        case 3:
            WinY = WinY - WinHeight / 2;
            break;
        case 4:
            WinY = WinY - WinHeight / 2;
            WinX = WinX - WinWidth / 2 ;
            break;
        case 5:
            WinY = WinY - WinHeight / 2;
            WinX = WinX - WinWidth;
            break;
        case 6:
            WinY  = WinY - WinHeight;
            break;
        case 7:
            WinY  = WinY - WinHeight;
            WinX  = WinX - WinWidth / 2;
            break;
        case 8:
            WinY  = WinY - WinHeight;
            WinX  = WinX - WinWidth;
            break;
        default:
            OSD_TRACE("the anchor point is out of reach\n");
            return bWindowChanged;
    }

    /*check StaX or StaY over the window position*/
    if( WinX < StaX || WinX > StaX + StaWidth )
    {
        WinX = StaX;
    }

    if( WinY < StaY || WinY > StaY + StaHeight)
    {
        WinY = StaY;
    }

    /*--start ??*/
    if(StaWidth == pCCObj->stScreen.CaptionRect.width)
    {
        s32MaxStaWidth = (StaWidth * 9) / 8 - 50;
    }
    else
    {
        s32MaxStaWidth = 1920 - StaX - 50;
    }

    /*limit the cc window position into the cc screen*/
    if( (((WinX + WinWidth) > (StaWidth + StaX)) && (WinWidth < 1000)) ||
        (((WinX + WinWidth) > (s32MaxStaWidth + StaX)) && (WinWidth >= 1000)))
    {
        if(WinWidth >= 1000)
        {
            WinX = (s32MaxStaWidth + StaX) - WinWidth;
        }
        else
        {
            WinX = (StaWidth + StaX) - WinWidth;
        }

        if(WinX < 0)
        {
            WinX = 100;
            WinWidth = (s32MaxStaWidth + StaX) - WinX;
        }
    }
    /*--end ??*/

    /*if window position has changed , clear the orignal window*/
    if( (WinY + WinHeight) > (StaHeight + StaY))
    {
        OSD_TRACE("Window vertical position overflow!\n");
        OSD_TRACE("y:%d,height:%d,safe title area:y:%d,height:%d\n",WinY,WinHeight,StaY,StaHeight);

        WinY = (StaHeight + StaY) - WinHeight;
        if(CCWindow[u8WindowID].u8IsVisible &&((WinX != OriginalX) || (WinY != OriginalY) ||(WinWidth != OriginalWidth) ||(WinHeight != OriginalHeight)))
        {
            _DtvCC_FillRect(u8WindowID,OriginalX,OriginalY,OriginalWidth,OriginalHeight,1,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_FALSE);
        }
    }

    OSD_TRACE("\n[window %d] position: X=%d,Y=%d,Width=%d,Height=%d\n",u8WindowID,WinX,WinY,WinWidth,WinHeight);
    if((WinX != OriginalX) || (WinY != OriginalY) || (WinWidth != OriginalWidth) || (WinHeight != OriginalHeight))
    {
        HI_UNF_CC_RECT_S stWinRect;
        stWinRect.x = WinX;
        stWinRect.y = WinY;
        stWinRect.width = WinWidth;
        stWinRect.height = WinHeight;
        (HI_VOID)CCDISP_Window_Create((CCDISP_WINDOW_ID_E)u8WindowID,stWinRect);
        if(CCWindow[u8WindowID].u8IsVisible == 0)
        {
            CCDISP_Window_Hide((CCDISP_WINDOW_ID_E)u8WindowID);
        }
    }

    // update window overlap map
    CCWindow[u8WindowID].s32OverlapLowerWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,0);
    CCWindow[u8WindowID].s32OverlapHigherWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,1);

    OSD_TRACE("u8WindowID:%d,overlapLowerWindowMap:%d,overlapHigherWindowMap:%d\n",
             u8WindowID,CCWindow[u8WindowID].s32OverlapLowerWindowMap,CCWindow[u8WindowID].s32OverlapHigherWindowMap);

    CCWindow[u8WindowID].u16X = WinX;
    CCWindow[u8WindowID].u16Y = WinY;
    CCWindow[u8WindowID].u16Width = WinWidth;
    CCWindow[u8WindowID].u16Height = WinHeight;

    /*when the window is visible,and window position has changed,*/
    if(CCWindow[u8WindowID].u8IsVisible)
    {
       if((WinX != OriginalX) || (WinY != OriginalY) || (WinWidth != OriginalWidth) || (WinHeight != OriginalHeight))
       {
           if((OriginalWidth != 0) && (OriginalHeight != 0))
           {
                _DtvCC_ClearWindowOnScreen(u8WindowID,1);
                _DtvCC_SetDefaultDisplayOrder(u8WindowID);
                CCWindow[u8WindowID].u8IsVisible = 0;
           }

           (HI_VOID)_DtvCC_SetWindowColor(u8WindowID,CCWindow[u8WindowID].u32FillColor,CCWindow[u8WindowID].u32FillOpacity);

           _DtvCC_ClearWindowOnScreen(u8WindowID,0);

           bWindowChanged = HI_TRUE;
       }
    }

    if(CCWindow[u8WindowID].s32OverlapHigherWindowMap)
    {
        //CC708SearchWindowClipArea(u8WindowID);
    }

    if((s32CurRow == 0) && (s32CurColumn == 0))
    {
        CC708_Cursor_ResetToOrigin(&CCWindow[u8WindowID],&CCWindow[u8WindowID].stCursor,
              CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
    }

    return bWindowChanged;
}


/*************************************************************************************************
* Function name: HI_S32 _DtvCC_CheckWindowOverlap(HI_U8 u8WindowID, HI_U8 flag)             *
* Description: Decide if the whole window is visible.                                           *
* Arguments:  u8WindowID: ID of current window.                                                   *
*             flag: 0 check if there are windows with lower priority overlap with current window*
*                   1 check if there are windows with higher priority overlap with current window*
* Return value:  return the u8WindowMap of the window which overlap with current window,         *
*                or 0 if no overlap.                                                           *                                                                          *
*************************************************************************************************/
static inline HI_S32 _DtvCC_CheckWindowOverlap(HI_U8 u8WindowID, HI_U8 flag)
{
    HI_S32 i,s32CurWinStartX,s32CurWinStartY,s32CurWinEndX,s32CurWinEndY;
    HI_S32 x0,x1,y0,y1;
    HI_U8 u8WindowMap = 0;
    HI_U8 u8OverlapWindowMap = 0;

    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(u8WindowID,MAX_CC_WINDOWS,"u8WindowID",0);
    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(flag,2,"flag",0);

    if(CCWindow[u8WindowID].u8IsVisible == 0)
    {
        return 0;
    }

    if(flag == 0)
    {
        for(i = 0; i < MAX_CC_WINDOWS; i++)
        {
            if((i != u8WindowID) &&(CCWindow[i].s32WindowID != -1) &&(CCWindow[i].u8IsVisible == 1))
            {
                if(CCWindow[i].u8Priority > CCWindow[u8WindowID].u8Priority)
                {
                    u8WindowMap |= (1<<i);
                }
                else if((CCWindow[i].u8Priority == CCWindow[u8WindowID].u8Priority) &&
                        (CCWindow[i].s32WindowID > CCWindow[u8WindowID].s32WindowID))
                {
                    u8WindowMap |= (1<<i);
                }
            }
        }
    }
    else
    {
        for(i = 0; i < MAX_CC_WINDOWS; i++)
        {
            if((i != u8WindowID) &&(CCWindow[i].s32WindowID != -1) &&(CCWindow[i].u8IsVisible == 1))
            {
                if(CCWindow[i].u8Priority < CCWindow[u8WindowID].u8Priority)
                {
                    u8WindowMap |= (1<<i);
                }
                else if((CCWindow[i].u8Priority == CCWindow[u8WindowID].u8Priority) &&
                             (CCWindow[i].s32WindowID < CCWindow[u8WindowID].s32WindowID))
                {
                    u8WindowMap |= (1<<i);
                }
            }
        }
    }

    if(!u8WindowMap)
    {
        return 0;
    }

    s32CurWinStartX = CCWindow[u8WindowID].u16X;
    s32CurWinEndX = CCWindow[u8WindowID].u16X + CCWindow[u8WindowID].u16Width - 1;
    s32CurWinStartY = CCWindow[u8WindowID].u16Y;
    s32CurWinEndY = CCWindow[u8WindowID].u16Y + CCWindow[u8WindowID].u16Height - 1;

    for(i = 0; i < MAX_CC_WINDOWS; i++)
    {
        if(u8WindowMap & (1<<i))
        {
            x0 = CCWindow[i].u16X;
            x1 = CCWindow[i].u16X + CCWindow[i].u16Width - 1;
            y0 = CCWindow[i].u16Y;
            y1 = CCWindow[i].u16Y + CCWindow[i].u16Height - 1;

            // if one vertex of a window is inside current window,
            // two windows overlap.
            if(((x0 > s32CurWinStartX) && (x0 < s32CurWinEndX)) ||((x1 > s32CurWinStartX) && (x1 < s32CurWinEndX)))
            {
                if(((y0 > s32CurWinStartY) && (y0 < s32CurWinEndY)) ||((y1 > s32CurWinStartY ) && (y1 < s32CurWinEndY)))
                {
                    u8OverlapWindowMap |= (1<<i);
                }
            }

            // if one vertex of the current window is inside a window
            // two windows overlap.
            if(((s32CurWinStartX > x0) && (s32CurWinStartX < x1)) ||((s32CurWinEndX > x0) && (s32CurWinEndX < x1)))
            {
                if(((s32CurWinStartY > y0) && (s32CurWinStartY < y1)) ||((s32CurWinEndY > y0) && (s32CurWinEndY < y1)))
                {
                    u8OverlapWindowMap |= (1<<i);
                }
            }
            // if two edges of two window cross,
            // two windows overlap.
            if((( s32CurWinStartX >= x0 )&&( s32CurWinEndX <= x1 )&&( s32CurWinStartY <= y0 )&&( s32CurWinEndY >= y1 )) ||
               (( x0 >= s32CurWinStartX )&&( x1 <= s32CurWinEndX )&&( y0 <= s32CurWinStartY )&&( y1 >= s32CurWinEndY )))
            {
                u8OverlapWindowMap |= (1<<i);
            }

            // if two edges of the two windows overlap, two windows overlap
            if(((s32CurWinStartX==x0)&&(s32CurWinEndX==x1)&&( ABS(y1-s32CurWinStartY)<(ABS(s32CurWinEndY-s32CurWinStartY)+ABS(y1-y0)))&&( ABS(s32CurWinEndY-y0)<(ABS(s32CurWinEndY-s32CurWinStartY)+ABS(y1-y0)))) ||
               ((s32CurWinStartY==y0)&&(s32CurWinEndY==y1)&&( ABS(x1-s32CurWinStartX)<(ABS(s32CurWinEndX-s32CurWinStartX)+ABS(x1-x0)))&&( ABS(s32CurWinEndX-x0)<(ABS(s32CurWinEndX-s32CurWinStartX)+ABS(x1-x0)))))
            {
                u8OverlapWindowMap |= (1<<i);
            }
        }
    }

    OSD_TRACE("u8OverlapWindowMap:0x%x\n",u8OverlapWindowMap);
    return u8OverlapWindowMap;
}


static inline HI_VOID _DtvCC_SetDefaultDisplayOrder(HI_U8 u8WindowID)
{
    HI_U8 i;
    if(CCWindow[u8WindowID].s32DisplayOrder == -1)
    {
        OSD_TRACE("This window isn't on screen.\n");
        return;
    }

    for(i = 0;i < 8;i++)
    {
        if((CCWindow[i].s32WindowID != -1) &&
           (CCWindow[i].s32DisplayOrder != -1) &&
           (CCWindow[i].s32DisplayOrder > CCWindow[u8WindowID].s32DisplayOrder))
        {
            CCWindow[i].s32DisplayOrder -= 1;
        }
    }

    CCWindow[u8WindowID].s32DisplayOrder = -1;

}

static inline HI_VOID _DtvCC_DrawPendingRow(HI_VOID)
{
    HI_S32 WinID = CurrentWindowID;

    CC708OSDPARAM_CHECK_RETURN_VOID(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID");

    if(CCWindow[WinID].bPendingRow)
    {
        /* calculate draw start position of current row */
        _DtvCC_CalcDrawStartPositionByRow(WinID,CCWindow[WinID].s32PendingRow);
        /* draw text */
        _DtvCC_DrawRowText(WinID,0,0,CCWindow[WinID].s32PendingRow,HI_FALSE);

        CCWindow[WinID].s32PendingRow = 0;
        CCWindow[WinID].bPendingRow = HI_FALSE;
    }
}

static HI_VOID _DtvCC_DrawRowText(HI_U8 u8WindowID,HI_U16 u16StartX, HI_U16 u16StartY, HI_U8 u8Row, HI_BOOL bOffScreen)
{
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);

    HI_S32 column,s32DrawX,s32DrawY;
    HI_S32 s32CharCount = 0, s32TextWidth = 0, s32SpaceWidth = 0, s32SpaceCount = 0,s32TextHeight = 0;
    HI_S32 s32TotalWidth = 0, s32MaxHeight = 0, s32LastHeight = 0, s32CurHeight = 0;
    HI_U16 au16Buf[MAX_CC_COLUMNS] = {0};
    CC708_PenDef_S *pstPen,*pstLastPen;
    CC708_Cursor_S * pstCursor;
    HI_BOOL bLastUnderlineSpace = HI_FALSE;
    HI_BOOL bLeadingSpace = HI_FALSE;
    HI_BOOL bFirstColumn = HI_FALSE;
    HI_BOOL bAddLeadingSpaceWidth = HI_FALSE;
    HI_U8 bClear = 2, u8ColumnBegin = 0, u8ColumnEnd = MAX_CC_COLUMNS;
    HI_BOOL bIgnoreLeftBG = HI_FALSE;
    HI_BOOL bPenColorChanged = HI_FALSE;

    if(HI_NULL == pCCObj)
    {
        return;
    }

    /* Set initial value */
    pstCursor = &CCWindow[u8WindowID].stCursor;

    if(bOffScreen)   /*off screen ,invisible*/
    {
        s32DrawX = u16StartX;
        s32DrawY = u16StartY;
    }
    else
    {
        s32DrawX = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
        s32DrawY = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
    }

    pstPen = &CCWindow[u8WindowID].astText[u8Row][u8ColumnBegin].stPen;
    pstLastPen = pstPen;

    /* set initial font attributes at the beginning */
    if(CCWindow[u8WindowID].astText[u8Row][u8ColumnBegin].u8IsPenChanged)
    {
        (HI_VOID)_DtvCC_SetCurrentPenColor(pstPen,pstPen->u32FGColor,pstPen->u8FGOpacity,pstPen->u32BGColor,
                                          pstPen->u8BGOpacity,pstPen->u32EdgeColor,CCWindow[u8WindowID].u8IsVisible);
        (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                               pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
    }

    if(CCWindow[u8WindowID].astText[u8Row][0].u16Caption == 0)
    {
        bAddLeadingSpaceWidth = HI_TRUE;
    }

    for(column = u8ColumnBegin; column < u8ColumnEnd; column++)
    {
        if((CCWindow[u8WindowID].astText[u8Row][column].u16Caption == 0x20) &&
           (column < u8ColumnEnd-1) &&
           (CCWindow[u8WindowID].astText[u8Row][column+1].u8IsPenChanged) &&
           (CCWindow[u8WindowID].astText[u8Row][column+1].stPen.u8IsUnderline == 0))
        {
            bLastUnderlineSpace = HI_TRUE;
        }

        if( CCWindow[u8WindowID].astText[u8Row][column].u8IsPenChanged ||
            (CCWindow[u8WindowID].astText[u8Row][column].u16Caption == 0) ||
            (CCWindow[u8WindowID].astText[u8Row][column].u16Caption == 0x1020) ||
            (bLastUnderlineSpace == HI_TRUE))
        {
            if( (s32CharCount > 0) && (pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT))
            {
                if(bAddLeadingSpaceWidth)
                {
                    s32TotalWidth += LEADINGSPACEWIDTH;
                    bAddLeadingSpaceWidth = HI_FALSE;
                }

                /* move cursor skip last spaces */
                if(s32SpaceCount)
                {
                    s32TotalWidth += s32SpaceWidth;
                    s32SpaceWidth = 0;
                    s32SpaceCount = 0;
                }

                if(bLeadingSpace)
                {
                    _DtvCC_DrawLeadingSpace(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY,bClear,bFirstColumn,bOffScreen);
                    bLeadingSpace = HI_FALSE;

                    if(bFirstColumn)
                    {
                        s32TotalWidth += LEADINGSPACEWIDTH;
                        bFirstColumn = HI_FALSE;
                    }
                    bIgnoreLeftBG = HI_FALSE;
                }
                else
                {
                    bIgnoreLeftBG = HI_TRUE;
                }

                /* draw text */
                OSD_TRACE("CCDispText:x:%d,y:%d,s32CharCount:%d,CCWindow[u8WindowID].astText[%d][%d].u8IsPenChanged:%d,char: \n",
                        s32DrawX+s32TotalWidth,s32DrawY,s32CharCount,u8Row,column,CCWindow[u8WindowID].astText[u8Row][column].u8IsPenChanged);


                /* calculate text width and height */
                _DtvCC_GetTextSize(u8WindowID,au16Buf,s32CharCount,&s32TextWidth, &s32TextHeight);

                s32LastHeight = s32CurHeight;
                s32CurHeight = s32TextHeight;

                OSD_TRACE("s32LastHeight:%d,s32CurHeight:%d\n",s32LastHeight,s32CurHeight);

                // draw left bottom background when font size changes
                if((s32CurHeight > s32LastHeight) && (s32LastHeight > 0))
                {
                    _DtvCC_FillRect(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY+s32LastHeight,TAILINGSPACEWIDTH,s32CurHeight-s32LastHeight,2,
                                 CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen);
                }

                if(bPenColorChanged)
                {
                    bIgnoreLeftBG = HI_FALSE;
                    bPenColorChanged = HI_FALSE;
                }

                _DtvCC_DispText(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY,s32TextWidth,s32TextHeight,au16Buf,s32CharCount,
                         CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen,bIgnoreLeftBG,HI_TRUE);

                /* calculate total width and max height until current column */
                s32TotalWidth += s32TextWidth;
                s32MaxHeight = (s32TextHeight > s32MaxHeight) ? s32TextHeight : s32MaxHeight;
            }

            if(CCWindow[u8WindowID].astText[u8Row][column].u16Caption == 0x1020)
            {
                unsigned short space[1] = {0x20};

                _DtvCC_GetTextSize(u8WindowID,space,1,&s32TextWidth, &s32TextHeight);

                if(column == 0)
                {
                    _DtvCC_DrawLeadingSpace(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY,1,HI_TRUE,bOffScreen);
                    s32TotalWidth += LEADINGSPACEWIDTH;
                    bFirstColumn = HI_FALSE;
                }
                _DtvCC_DrawTSP(s32DrawX+s32TotalWidth,s32DrawY,bOffScreen);
                s32TotalWidth += s32TextWidth;
                s32MaxHeight = (s32TextHeight > s32MaxHeight)? s32TextHeight : s32MaxHeight;
            }

            /* if there is no caption in this grid, skip a column */
            if(CCWindow[u8WindowID].astText[u8Row][column].u16Caption == 0)
            {
                s32SpaceWidth += pCCObj->s32MaxCharWidth;
                s32SpaceCount ++;
            }

            /* initial char count of the next text to zero */
            s32CharCount = 0;

            /* set font attributes for next text */
            if(CCWindow[u8WindowID].astText[u8Row][column].u8IsPenChanged)
            {
                OSD_TRACE("Pen changed at u8Row %d, column %d,pstPen->u32FGColor:0x%x\n",u8Row,column,pstPen->u32FGColor);
                pstPen = &CCWindow[u8WindowID].astText[u8Row][column].stPen;

                if((pstLastPen->u32BGColor != pstPen->u32BGColor) ||
                   (pstLastPen->u8BGOpacity != pstPen->u8BGOpacity))
                {
                    bPenColorChanged = HI_TRUE;
                }

                pstLastPen = pstPen;

                (HI_VOID)_DtvCC_SetCurrentPenColor(pstPen,pstPen->u32FGColor,pstPen->u8FGOpacity,pstPen->u32BGColor,pstPen->u8BGOpacity,pstPen->u32EdgeColor,
                                            CCWindow[u8WindowID].u8IsVisible);
                (HI_VOID)_DtvCC_SetCurrentPenAttributes(pstPen,pstPen->enFontName,pstPen->enFontSize,pstPen->u8IsItalic,pstPen->u8IsUnderline,
                                            pstPen->enEdgeType,pstPen->enTextOffset,pstPen->enTextTag,CCWindow[u8WindowID].u8IsVisible);
            }

            if(bLastUnderlineSpace)
            {
                pstPen = &CCWindow[u8WindowID].astText[u8Row][column].stPen;
                (HI_VOID)CC708_Pen_SetTextUnderline(pstPen,0,1);
                bLastUnderlineSpace = HI_FALSE;
            }
        }

        /* group characters into text buffer, increase character count */
        if((CCWindow[u8WindowID].astText[u8Row][column].u16Caption != 0) &&
           (CCWindow[u8WindowID].astText[u8Row][column].u16Caption != 0x1020))
        {
            if((column == 0) ||(CCWindow[u8WindowID].astText[u8Row][column-1].u16Caption == 0) ||
              (CCWindow[u8WindowID].astText[u8Row][column-1].u16Caption == 0x1020) ||
              (CCWindow[u8WindowID].astText[u8Row][column-1].u16Caption == 0x1021))
            {
                bLeadingSpace = HI_TRUE;

                if(column == 0)
                {
                    bFirstColumn = HI_TRUE;
                }
                else
                {
                    bFirstColumn = HI_FALSE;
                }

                if((column != 0) &&
                   ((CCWindow[u8WindowID].astText[u8Row][column-1].u16Caption == 0x1020) ||
                   (CCWindow[u8WindowID].astText[u8Row][column-1].u16Caption == 0x1021)))
                {
                    bClear = 1; //DrawLeadingSpace(u8WindowID,1,bFirstColumn);
                }
                else
                {
                    bClear = 2; //DrawLeadingSpace(u8WindowID,2,bFirstColumn);
                }
            }

            au16Buf[s32CharCount++] = CCWindow[u8WindowID].astText[u8Row][column].u16Caption;
        }
    }

    /* draw last text */
    if((s32CharCount!=0) &&(pstPen->enTextTag != CC_TT_NON_DISPLAYED_TEXT))
    {
        if(bAddLeadingSpaceWidth)
        {
            s32TotalWidth += LEADINGSPACEWIDTH;
            //bAddLeadingSpaceWidth = HI_FALSE;
        }

        /* move cursor skip last spaces */
        if(s32SpaceCount)
        {
            s32TotalWidth += s32SpaceWidth;
//            s32SpaceWidth = 0;
//            s32SpaceCount = 0;
        }

        if(bLeadingSpace)
        {
            //DrawLeadingSpace(u8WindowID,x+s32TotalWidth,y,bClear,bFirstColumn,bOffScreen);20130426
            //bLeadingSpace = HI_FALSE;

            if(bFirstColumn)
            {
                s32TotalWidth += LEADINGSPACEWIDTH;
                //bFirstColumn = HI_FALSE;
            }
            bIgnoreLeftBG = HI_FALSE;
        }
        else
        {
            bIgnoreLeftBG = HI_TRUE;
        }

        /* draw text */
        OSD_TRACE("CCDispText:x:%d,y:%d,s32CharCount:%d,char:%c\n",s32DrawX+s32TotalWidth,s32DrawY,s32CharCount,au16Buf[0]);

        /* calculate text width and height */
        _DtvCC_GetTextSize(u8WindowID,au16Buf,s32CharCount,&s32TextWidth, &s32TextHeight);

        s32LastHeight = s32CurHeight;
        s32CurHeight = s32TextHeight;

        OSD_TRACE("s32LastHeight:%d,s32CurHeight:%d\n",s32LastHeight,s32CurHeight);

        // draw left bottom background when font size changes
        if((s32CurHeight > s32LastHeight) && (s32LastHeight > 0))
        {
            _DtvCC_FillRect(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY+s32LastHeight,TAILINGSPACEWIDTH,s32CurHeight-s32LastHeight,2,
                                            CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,bOffScreen);
        }

        if(bPenColorChanged)
        {
            bIgnoreLeftBG = HI_FALSE;
            //bPenColorChanged = HI_FALSE;
        }

        _DtvCC_DispText(u8WindowID,s32DrawX+s32TotalWidth,s32DrawY,s32TextWidth,s32TextHeight,au16Buf,s32CharCount,
                      CC_PD_LEFT_RIGHT,CC_SD_LEFT_RIGHT,bOffScreen,bIgnoreLeftBG,HI_TRUE);

        /* calculate total width and max height until current column */
        s32TotalWidth += s32TextWidth;
        s32MaxHeight = (s32TextHeight > s32MaxHeight) ? s32TextHeight : s32MaxHeight;
    }

    if(s32TotalWidth > 0)
    {
        s32TotalWidth += TAILINGSPACEWIDTH;
    }

    if(!bOffScreen && s32TotalWidth>0)
    {
        CC708_Cursor_MoveCursorForwardsOnScreen(pstCursor,s32TotalWidth - TAILINGSPACEWIDTH,
                   CCWindow[u8WindowID].enTextPrintDirection,CCWindow[u8WindowID].enTextScrollDirection);
    }
    /* set Row width and height */
    if(CCWindow[u8WindowID].as32RowWidth[u8Row] < s32TotalWidth)
    {
        CCWindow[u8WindowID].as32RowWidth[u8Row] = s32TotalWidth;
    }

    OSD_TRACE("s32MaxHeight:%d\n",s32MaxHeight);

    if(s32MaxHeight > 0)
    {
        CCWindow[u8WindowID].as32RowHeight[u8Row] = s32MaxHeight;
    }
}

static inline HI_VOID _DtvCC_BlockMove(HI_U8 u8WindowID, HI_U16 u16DstX, HI_U16 u16DstY, HI_U16 u16Width, HI_U16 u16Height,
                                  HI_U16 u16SrcX,HI_U16 u16SrcY)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CCDISP_WINDOW_ID_E enWinId = (CCDISP_WINDOW_ID_E)u8WindowID;
    s32Ret = CCDISP_Window_Rollup(enWinId,u16SrcX,u16Width,u16SrcY,u16DstY,u16Height,CCDISP_SPEED6);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("Block Move error\n");
    }

}


/********************************************************************************
*                      EXTERN FUNTION DEFINITION
*********************************************************************************/
HI_S32 CC708_OSD_SetCurrentWindow(HI_U8 u8WindowID)
{

  if( CCWindow[u8WindowID].s32WindowID != -1 )     /*whether the window has been defined*/
    {
        CurrentWindowID = u8WindowID;
        #if 0/* modify by ghl*/
        /*recover window color*/
        (HI_VOID)_DtvCC_SetWindowColor(u8WindowID,CCWindow[u8WindowID].u32FillColor,CCWindow[u8WindowID].u32FillOpacity);

        /*recover pen attribute*/
        (HI_VOID)_DtvCC_SetCurrentPenAttributes(&CCWindow[u8WindowID].stCurPen,
                                CCWindow[u8WindowID].stCurPen.enFontName,
                                CCWindow[u8WindowID].stCurPen.enFontSize,
                                CCWindow[u8WindowID].stCurPen.u8IsItalic,
                                CCWindow[u8WindowID].stCurPen.u8IsUnderline,
                                CCWindow[u8WindowID].stCurPen.enEdgeType,
                                CCWindow[u8WindowID].stCurPen.enTextOffset,
                                CCWindow[u8WindowID].stCurPen.enTextTag,1);
        /*recover pen color*/
        (HI_VOID)_DtvCC_SetCurrentPenColor(&CCWindow[u8WindowID].stCurPen,
                           CCWindow[u8WindowID].stCurPen.u8FGColor,
                           CCWindow[u8WindowID].stCurPen.u8FGOpacity,
                           CCWindow[u8WindowID].stCurPen.u8BGColor,
                           CCWindow[u8WindowID].stCurPen.u8BGOpacity,
                           CCWindow[u8WindowID].stCurPen.u8EdgeColor,1);

        // update window overlap map
        CCWindow[u8WindowID].s32OverlapLowerWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,0);
        CCWindow[u8WindowID].s32OverlapHigherWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,1);

        if(CCWindow[u8WindowID].s32OverlapHigherWindowMap)
        {
            #if 0
            CC708SearchWindowClipArea(u8WindowID);
            #endif
        }
        #endif
    }
    else
    {
        /*the specified u8WindowID is invalid*/
        CurrentWindowID = -1;
    }

    return HI_SUCCESS;

}


HI_S32 CC708_OSD_DefineWindow (HI_U8 u8WindowID,HI_U8 u8Priority,HI_U8 u8AnchorPoint,HI_U8 u8RelativePositioning,
                                          HI_U8 u8AnchorVertical,HI_U8 u8AnchorHorizontal,HI_U8 u8RowCount,HI_U8 u8ColumnCount,
                                          HI_U8 u8RowLock,HI_U8 u8ColumnLock,HI_U8 u8IsVisible,HI_U8 u8WindowStyleID, HI_U8 u8PenStyleID)
{

    if( CCWindow[u8WindowID].s32WindowID == -1 )
    {
        /* When zero during a window create,means style #1.See Spec p41*/
        if( u8WindowStyleID == 0 )
        {
            u8WindowStyleID = 1;
        }
        if( u8PenStyleID == 0 )
        {
            u8PenStyleID = 1;
        }

        CCWindow[u8WindowID].s32WindowID = u8WindowID;
        CCWindow[u8WindowID].u8Priority = u8Priority;
        CCWindow[u8WindowID].u8AnchorPoint = u8AnchorPoint;
        CCWindow[u8WindowID].u8RelativePositioning = u8RelativePositioning;
        CCWindow[u8WindowID].u8AnchorVertical = u8AnchorVertical;
        CCWindow[u8WindowID].u8AnchorHorizontal = u8AnchorHorizontal;
        CCWindow[u8WindowID].u8RowCount = u8RowCount;
        CCWindow[u8WindowID].u8ColumnCount = u8ColumnCount;
        CCWindow[u8WindowID].u8IsRowLock = u8RowLock;
        CCWindow[u8WindowID].u8IsColumnLock = u8ColumnLock;
        CCWindow[u8WindowID].u8IsVisible = u8IsVisible;
        CCWindow[u8WindowID].u8WindowStyleID = u8WindowStyleID;
        CCWindow[u8WindowID].u8PenStyleID = u8PenStyleID;

        (HI_VOID)CC708_OSD_SetCurrentWindow(u8WindowID);
        _DtvCC_SetWinAttrByWinStyleID(u8WindowID);
        _DtvCC_SetPenAttrByPenStyleID(u8WindowID);
        (HI_VOID)_DtvCC_CalcWinPosByAnchorPoint(u8WindowID);
        (HI_VOID)CC708_OSD_SetPenLocation(0, 0);

        /* draw window background color on screen */
        if(u8IsVisible)
        {
            _DtvCC_ClearWindowOnScreen(u8WindowID,0);
            _DtvCC_SetLastDisplayOrder(u8WindowID);
        }
    }
    else  /*the window has already been defined ,updated */
    {
        (HI_VOID)CC708_OSD_SetCurrentWindow(u8WindowID);
        /*command is to be ingnored if the command parameters are unchanged,Spec P41*/
        if(CCWindow[u8WindowID].u8Priority == u8Priority &&
            CCWindow[u8WindowID].u8AnchorPoint == u8AnchorPoint &&
            CCWindow[u8WindowID].u8RelativePositioning == u8RelativePositioning &&
            CCWindow[u8WindowID].u8AnchorVertical == u8AnchorVertical &&
            CCWindow[u8WindowID].u8AnchorHorizontal == u8AnchorHorizontal &&
            CCWindow[u8WindowID].u8RowCount == u8RowCount &&
            CCWindow[u8WindowID].u8ColumnCount == u8ColumnCount &&
            //CCWindow[u8WindowID].u8RowLock == u8RowLock &&
            //CCWindow[u8WindowID].u8ColumnLock == u8ColumnLock &&
            CCWindow[u8WindowID].u8IsVisible == u8IsVisible &&
            CCWindow[u8WindowID].u8WindowStyleID == u8WindowStyleID &&
            CCWindow[u8WindowID].u8PenStyleID == u8PenStyleID)
        {

        }
        else   /* some of the window parameters are changed,window update */
        {
            if(CCWindow[u8WindowID].u8WindowStyleID != u8WindowStyleID)
            {
                /* when zero during a window update, no window attribute parameters are changed.See Spec P41*/
                if(u8WindowStyleID != 0)
                {
                    /* set window attributes according to window style ID */
                    CCWindow[u8WindowID].u8WindowStyleID = u8WindowStyleID;
                    _DtvCC_SetWinAttrByWinStyleID(u8WindowID);
                }

            }
            /* pen style is changed */
            if(CCWindow[u8WindowID].u8PenStyleID != u8PenStyleID)
            {
                /* the same as the u8WindowStyleID */
                /* when an existing window is being updated with the CC708DefineWindow command, */
                /* the pen location and pen attributes are unaffected. See Spec p42 */
                if(u8PenStyleID != 0)
                {
                    /* set window attributes according to window style ID */
                    CCWindow[u8WindowID].u8PenStyleID = u8PenStyleID;
                    _DtvCC_SetPenAttrByPenStyleID(u8WindowID);

                }

            }
            /*when the window is not on the screen,just update window attribute*/
            if(!CCWindow[u8WindowID].u8IsVisible)
            {
                if((CCWindow[u8WindowID].u8RelativePositioning != u8RelativePositioning) ||
                   (CCWindow[u8WindowID].u8AnchorPoint != u8AnchorPoint) ||
                   (CCWindow[u8WindowID].u8AnchorVertical != u8AnchorVertical) ||
                   (CCWindow[u8WindowID].u8AnchorHorizontal != u8AnchorHorizontal) ||
                   (CCWindow[u8WindowID].u8RowCount != u8RowCount) ||
                   (CCWindow[u8WindowID].u8ColumnCount != u8ColumnCount))
                {
                    CCWindow[u8WindowID].u8RelativePositioning = u8RelativePositioning;
                    CCWindow[u8WindowID].u8AnchorPoint = u8AnchorPoint;
                    CCWindow[u8WindowID].u8AnchorVertical = u8AnchorVertical;
                    CCWindow[u8WindowID].u8AnchorHorizontal = u8AnchorHorizontal;
                    CCWindow[u8WindowID].u8RowCount = u8RowCount;
                    CCWindow[u8WindowID].u8ColumnCount = u8ColumnCount;

                    /* recalculate current window position */
                    (HI_VOID)_DtvCC_CalcWinPosByAnchorPoint(u8WindowID);

                    /* move cursor by row and column */
                    _DtvCC_MoveCursorByRowAndColumnOffScreen(u8WindowID,0,0);
                }
            }
            /*when the window is on the screen,we should redraw the screen*/
            else
            {
                 if((CCWindow[u8WindowID].u8RelativePositioning != u8RelativePositioning) ||
                   (CCWindow[u8WindowID].u8AnchorPoint != u8AnchorPoint) ||
                   (CCWindow[u8WindowID].u8AnchorVertical != u8AnchorVertical) ||
                   (CCWindow[u8WindowID].u8AnchorHorizontal != u8AnchorHorizontal) ||
                   (CCWindow[u8WindowID].u8RowCount != u8RowCount) ||
                   (CCWindow[u8WindowID].u8ColumnCount != u8ColumnCount))
                 {

                    if((CCWindow[u8WindowID].u8RowCount > u8RowCount) ||(CCWindow[u8WindowID].u8ColumnCount > u8ColumnCount))
                    {
                        if((CCWindow[u8WindowID].u8RelativePositioning != u8RelativePositioning) ||
                           (CCWindow[u8WindowID].u8AnchorPoint != u8AnchorPoint) ||
                           (CCWindow[u8WindowID].u8AnchorVertical != u8AnchorVertical) ||
                           (CCWindow[u8WindowID].u8AnchorHorizontal != u8AnchorHorizontal) ||
                           (CCWindow[u8WindowID].u8AnchorPoint != 0))
                        {
                            _DtvCC_ClearWindowOnScreen(u8WindowID,1);
                            _DtvCC_SetDefaultDisplayOrder(u8WindowID);
                            CCWindow[u8WindowID].u8IsVisible = 0;
                        }

                        /* clear the text width and height attributes */
                        memset(CCWindow[u8WindowID].as32RowWidth, 0, MAX_CC_ROWS*sizeof(int));
                        memset(CCWindow[u8WindowID].as32RowHeight, 0, MAX_CC_ROWS*sizeof(int));

                        CCWindow[u8WindowID].u8RelativePositioning = u8RelativePositioning;
                        CCWindow[u8WindowID].u8AnchorPoint = u8AnchorPoint;
                        CCWindow[u8WindowID].u8AnchorVertical = u8AnchorVertical;
                        CCWindow[u8WindowID].u8AnchorHorizontal = u8AnchorHorizontal;
                        CCWindow[u8WindowID].u8RowCount = u8RowCount;
                        CCWindow[u8WindowID].u8ColumnCount = u8ColumnCount;

                        /* recalculate current window position */
                        (HI_VOID)_DtvCC_CalcWinPosByAnchorPoint(u8WindowID);

                        _DtvCC_ClearWindowOnScreen(u8WindowID,0);
                        CCWindow[u8WindowID].u8IsVisible = 1;

                        /* move cursor by row and column */
                        _DtvCC_MoveCursorByRowAndColumn(u8WindowID,0,0);
                    }
                    else
                    {
                        HI_S32 col,row;
                        _DtvCC_ClearWindowOnScreen(u8WindowID,1);
                        _DtvCC_SetDefaultDisplayOrder(u8WindowID);
                        CCWindow[u8WindowID].u8IsVisible = 0;

                        memset(CCWindow[u8WindowID].as32RowWidth, 0, MAX_CC_ROWS*sizeof(int));
                        memset(CCWindow[u8WindowID].as32RowHeight, 0, MAX_CC_ROWS*sizeof(int));

                        CCWindow[u8WindowID].u8RelativePositioning = u8RelativePositioning;
                        CCWindow[u8WindowID].u8AnchorPoint = u8AnchorPoint;
                        CCWindow[u8WindowID].u8AnchorVertical = u8AnchorVertical;
                        CCWindow[u8WindowID].u8AnchorHorizontal = u8AnchorHorizontal;
                        CCWindow[u8WindowID].u8RowCount = u8RowCount;
                        CCWindow[u8WindowID].u8ColumnCount = u8ColumnCount;

                        (HI_VOID)_DtvCC_CalcWinPosByAnchorPoint(u8WindowID);

                        // if u8Priority changes, we redraw the window later
                        if(CCWindow[u8WindowID].u8Priority == u8Priority)
                        {
                            _DtvCC_ReDrawWindow(u8WindowID,HI_FALSE);
                        }
                        else
                        {
                            CCWindow[u8WindowID].u8IsVisible = 1;
                        }
                        row = CCWindow[u8WindowID].stCursor.s32CurRow;
                        col = CCWindow[u8WindowID].stCursor.s32CurColumn;
                        OSD_TRACE("-->window %d visable=%d\n",u8WindowID,CCWindow[u8WindowID].u8IsVisible);
                        _DtvCC_MoveCursorByRowAndColumn(u8WindowID,row,col);
                        OSD_TRACE("re-calc cursor pos,row=%d,col=%d,%d,%d\n",row,col,CCWindow[u8WindowID].stCursor.s32X,CCWindow[u8WindowID].stCursor.s32Y);
                    }
                 }
            }


            /* window visibility is changed */
            if(CCWindow[u8WindowID].u8IsVisible != u8IsVisible)
            {
                if(u8IsVisible)
                {
                    /* display the window */
                    _DtvCC_DisplayWindow(u8WindowID);
                }
                else
                {
                    /* hide the window */
                    _DtvCC_HideWindow(u8WindowID);
                }
            }

            if((CCWindow[u8WindowID].u8Priority != u8Priority) && CCWindow[u8WindowID].u8IsVisible)
            {
                HI_S32 overlapHigherWindowMap = CCWindow[u8WindowID].s32OverlapHigherWindowMap;

                CCWindow[u8WindowID].u8Priority = u8Priority;

                // update window overlap map

                CCWindow[u8WindowID].s32OverlapLowerWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,0);
                CCWindow[u8WindowID].s32OverlapHigherWindowMap = _DtvCC_CheckWindowOverlap(u8WindowID,1);

                if(CCWindow[u8WindowID].s32OverlapHigherWindowMap)
                {
                    #if 0
                    CC708SearchWindowClipArea(u8WindowID);
                    #endif
                }

                _DtvCC_ReDrawWindow(u8WindowID,HI_FALSE);

                if(overlapHigherWindowMap != CCWindow[u8WindowID].s32OverlapHigherWindowMap)
                {
                    HI_S32 i;

                    for(i=0; i<MAX_CC_WINDOWS; i++)
                    {
                        if(((overlapHigherWindowMap&(1<<i)) == 0) &&
                           ((CCWindow[u8WindowID].s32OverlapHigherWindowMap&(1<<i)) != 0))
                        {
                            _DtvCC_ReDrawWindow(i,HI_FALSE);
                        }
                    }
                }

            }

        }
    }
    return HI_SUCCESS;
}


HI_S32 CC708_OSD_ClearWindows(HI_U8 u8WindowMap)
{

    HI_U8 WinID = 0;
    for( WinID = 0; WinID < 8; WinID++ )
    {
        if( (u8WindowMap >> WinID)&(0x01))
        {
            if( CCWindow[WinID].s32WindowID != -1 )
            {
                _DtvCC_ClearWindow(WinID);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 CC708_OSD_DeleteWindows(HI_U8 u8WindowMap)
{
    HI_U8 i,u8Priority;

    // delete windows according to u8Priority
    for(u8Priority = 0; u8Priority < 8; u8Priority++)
    {
        for( i = 0;i < 8; i++ )
        {
            if((u8WindowMap &(1<<i))&&(CCWindow[i].s32WindowID != -1)&&(CCWindow[i].u8Priority == u8Priority))
            {
                _DtvCC_DeleteWindow(i);

                /* update CurrentWindowID */
                if( CurrentWindowID == i )
                {
                    CurrentWindowID = -1;
                }
                u8WindowMap ^= (1<<i);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 CC708_OSD_DisplayWindows(HI_U8 u8WindowMap)
{
    HI_S32 priority;
    HI_U8 i;

    for(priority = 7;priority >= 0;priority --)
    {
        for( i = 0; i < 8; i++ )        /*high priority can overlap low priority*/
        {
            if((u8WindowMap&(1<<i))&&(CCWindow[i].s32WindowID != -1)&&(CCWindow[i].u8Priority == priority))
            {
                _DtvCC_DisplayWindow(i);
            }
        }
    }
    return HI_SUCCESS;
}

HI_S32 CC708_OSD_CharFlash(HI_VOID)
{
    CCDISP_Text_CharFlash();
    return HI_SUCCESS;
}

HI_S32 CC708_OSD_HideWindows(HI_U8 u8WindowMap)
{
    HI_U8 i=0;

    for( i = 0; i < 8; i++ )
    {
        if( ((u8WindowMap >> i)&(0x01))!=0 )
        {
            if ( (CCWindow[i].s32WindowID != -1) /*&& CCWindow[i].visible*/) // comment by jules
            {
                _DtvCC_HideWindow(i);
            }
        }
    }
    return HI_SUCCESS;

}

HI_S32 CC708_OSD_ToggleWindows(HI_U8 u8WindowMap)
{
    HI_U8 i = 0;
    HI_U8 u8DisplayMap = 0;
    /*hide the visible window*/
    for( i = 0; i < 8; i++ )
    {
        if( ((u8WindowMap >> i)&(0x01))!=0 )
        {
            if( CCWindow[i].s32WindowID != -1 )
            {
                if( CCWindow[i].u8IsVisible == 1 )
                {
                    _DtvCC_HideWindow(i);
                }
                else
                {
                    u8DisplayMap = (u8DisplayMap | (0x01 << i)) & (0xff);
                }
            }
        }
    }

    /*display the invisible window*/
    for( i = 0; i < 8; i++ )
    {
        if ( ((u8DisplayMap >> i) & (0x01)) != 0 )
        {

            if ( CCWindow[i].s32WindowID != -1 )
            {
                if ( CCWindow[i].u8IsVisible == 0 )
                {
                    _DtvCC_DisplayWindow(i);
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 CC708_OSD_SetWindowAttributes (HI_U8 u8Justify,HI_U8 u8PrintDirection,HI_U8 u8ScrollDirection,
                                                     HI_U8 u8WordWrap,HI_U8 u8DisplayEffect,HI_U8 u8EffectDirection,
                                                     HI_U8 u8EffectSpeed,HI_U32 u32FillColor,HI_U8 u8FillOpacity,
                                                     HI_U8 u8BorderType,HI_U32 u32BorderColor)
{
    HI_S32 WinID = CurrentWindowID;
    HI_U8 u8IsColorChange = 0;
    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    CCWindow[WinID].enTextJustify = (HI_UNF_CC_JUSTIFY_E)u8Justify;
    CCWindow[WinID].enTextPrintDirection = (CC708_PrintDirection_E)u8PrintDirection;
    CCWindow[WinID].enTextScrollDirection = (CC708_ScrollDirection_E)u8ScrollDirection;
    CCWindow[WinID].enWordWrap = (HI_UNF_CC_WORDWRAP_E)u8WordWrap;
    CCWindow[WinID].enDisplayEffect = (CC708_DisplayEffect_E)u8DisplayEffect;
    CCWindow[WinID].enEffectDirection = (CC708_EffectDirection_E)u8EffectDirection;
    CCWindow[WinID].u8EffectSpeed = u8EffectSpeed;
    CCWindow[WinID].enBorderType = (HI_UNF_CC_EdgeType_E)u8BorderType;
    CCWindow[WinID].u32BorderColor = u32BorderColor;

    /*window color and opacity has been changed ,if the window is visible,the window should be redraw*/
    u8IsColorChange = _DtvCC_SetWindowColor(WinID,u32FillColor,u8FillOpacity);
    if(u8IsColorChange)
    {
        if(CCWindow[WinID].u8IsVisible)
        {
            /* clear window */
            _DtvCC_ClearWindowOnScreen(WinID,0);
            /* redraw window */
            _DtvCC_ReDrawWindow(WinID,HI_FALSE);
        }

        if(CCWindow[WinID].u8IsEmpty == 1)
        {
            CCWindow[WinID].u8IsEmpty = 0;
        }
    }

    return HI_SUCCESS;

}

HI_S32 CC708_OSD_SetPenAttributes (HI_U8 u8FontSize,HI_U8 u8FontName,HI_U8 u8TextTag,HI_U8 u8TextOffset,
                                               HI_U8 u8IsItalic,HI_U8 u8IsUnderline,HI_U8 u8EdgeType)
{
    HI_S32 WinID = CurrentWindowID;
    HI_U8 u8CurRow,u8CurColumn;
    HI_U8 u8IsPenChanged;
    CC708_Cursor_S *pstCursor;
    CC708_PenDef_S *pstPen;
    HI_BOOL bFontSizeChanged = HI_FALSE;

    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    pstCursor = &CCWindow[WinID].stCursor;
    u8CurRow = pstCursor->s32CurRow;
    u8CurColumn = pstCursor->s32CurColumn;

    if((CCWindow[WinID].stCurPen.enFontName != u8FontName)
        ||(CCWindow[WinID].stCurPen.enFontSize != u8FontSize)
        ||(CCWindow[WinID].stCurPen.u8IsItalic != u8IsItalic))
    {
        bFontSizeChanged = HI_TRUE;
    }

    /* set current pen attributes of current window */
    u8IsPenChanged = _DtvCC_SetCurrentPenAttributes(&CCWindow[WinID].stCurPen,u8FontName,u8FontSize,u8IsItalic,
                                  u8IsUnderline,u8EdgeType,u8TextOffset,u8TextTag,CCWindow[WinID].u8IsVisible);

    /* set pen attributes of the text */
    pstPen = &CCWindow[WinID].astText[u8CurRow][u8CurColumn].stPen;
    memcpy(pstPen,&CCWindow[WinID].stCurPen,sizeof(CC708_PenDef_S));

    CCWindow[WinID].astText[u8CurRow][u8CurColumn].u8IsPenChanged |= u8IsPenChanged;

    /* CC708-B 8.4.6 */
    if(bFontSizeChanged && (CCWindow[WinID].enTextJustify == HI_UNF_CC_JUSTIFY_LEFT))
    {
        //_DtvCC_CalcWinPosByAnchorPoint(i);
        if(CCWindow[WinID].u8IsVisible)
        {
            HI_S32 x0,x1;

            x0 = CC708_Cursor_GetCurrentXPosition(pstCursor,
                             CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            (HI_VOID)CC708_Cursor_GetCurrentYPosition(pstCursor,
                             CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);

            _DtvCC_ReDrawWindow(WinID,HI_FALSE);

            x1 = CC708_Cursor_GetCurrentXPosition(pstCursor,
                             CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            (HI_VOID)CC708_Cursor_GetCurrentYPosition(pstCursor,
                             CCWindow[WinID].enTextPrintDirection,CCWindow[WinID].enTextScrollDirection);
            if(x0 != x1)
            {
                /* move cursor by row and column */
                _DtvCC_MoveCursorByRowAndColumnOnScreen(WinID,u8CurRow,u8CurColumn);
            }

            (HI_VOID)_DtvCC_SetCurrentPenAttributes(&CCWindow[WinID].stCurPen,u8FontName,u8FontSize,u8IsItalic,
                       u8IsUnderline,u8EdgeType,u8TextOffset,u8TextTag,CCWindow[WinID].u8IsVisible);
        }
    }

    return HI_SUCCESS;

}

HI_S32 CC708_OSD_SetPenColor (HI_U32 u32FGColor,HI_U8 u8FGOpacity,HI_U32 u32BGColor,HI_U8 u8BGOpacity,HI_U32 u32EdgeColor)
{
    HI_S32 i = CurrentWindowID;
    HI_U8 u8CurRow,u8CurColumn;
    HI_U8 u8IsPenChanged;
    CC708_Cursor_S *pstCursor;
    CC708_PenDef_S *pstPen;

    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    pstCursor = &CCWindow[i].stCursor;
    u8CurRow = CC708_Cursor_GetCurrentRow(pstCursor);
    u8CurColumn = CC708_Cursor_GetCurrentColumn(pstCursor);

    if((u32BGColor != CCWindow[i].stCurPen.u32BGColor) ||(u8BGOpacity != CCWindow[i].stCurPen.u8BGOpacity))
    {
        CCWindow[i].bPenColorChanged = HI_TRUE;
    }

    /* set current pen colors of current window */
    u8IsPenChanged = _DtvCC_SetCurrentPenColor(&CCWindow[i].stCurPen,u32FGColor,u8FGOpacity,u32BGColor,
                                                 u8BGOpacity,u32EdgeColor,CCWindow[i].u8IsVisible);
    /* set pen color of the text */
    pstPen = &CCWindow[i].astText[u8CurRow][u8CurColumn].stPen;
    memcpy(pstPen,&CCWindow[i].stCurPen,sizeof(CC708_PenDef_S));

    /* set bPenChanged flag */
    /* the bPenChanged flag of the text indicates either attributes or color change */
    CCWindow[i].astText[u8CurRow][u8CurColumn].u8IsPenChanged |= u8IsPenChanged;

    return HI_SUCCESS;

}

HI_S32 CC708_OSD_SetPenLocation (HI_U8 u8Row,HI_U8 u8Column)
{
    HI_S32 j,i = CurrentWindowID;
    HI_U8 u8CurRow,u8CurColumn;
    CC708_PenDef_S *pstPen;

    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);
    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(u8Row,CCWindow[i].u8RowCount+1,"u8Row",HI_FAILURE);
    CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(u8Column,CCWindow[i].u8ColumnCount+1,"u8Column",HI_FAILURE);

    u8CurRow = CCWindow[i].stCursor.s32CurRow;
    u8CurColumn = CCWindow[i].stCursor.s32CurColumn;

    /*column value ignored from SPL command for text with non left justification type*/
    if(CCWindow[i].enTextJustify != HI_UNF_CC_JUSTIFY_LEFT)
    {

        if( u8Row == u8CurRow)
        {
            u8Column = u8CurColumn;
        }
        else
        {
             u8Column = 0;
        }
    }

    if( (u8Row == u8CurRow) &&(u8Column == u8CurColumn)/* && (u8Column != 0) modify by ghl*/)
    {
        return HI_SUCCESS;
    }

    if((u8CurColumn != 0)&&(u8Row == u8CurRow)&&(u8CurColumn == (u8Column-1)))
    {
        HI_U16 space[1] = {0x20};

        if((CCWindow[i].astText[u8CurRow][u8CurColumn-1].u16Caption != 0) &&
           (CCWindow[i].astText[u8CurRow][u8CurColumn].u16Caption == 0))
        {
            CC708_OSD_DrawCaptionToWindow(space,1);
        }
    }

    /*draw pending u8Row first*/
    _DtvCC_DrawPendingRow();

    /* move cursor by u8Row and u8Column */
    _DtvCC_MoveCursorByRowAndColumn(i,u8Row,u8Column);

    // remember the former pen attributes
    for(j = u8Column; j >= 0; j--)
    {
        if(CCWindow[i].astText[u8Row][j].u8IsPenChanged)
        {
            memcpy(&CCWindow[i].stPrevPen,&CCWindow[i].astText[u8Row][j].stPen,sizeof(CC708_PenDef_S));
            break;
        }
    }

    /* set bPenChanged flag */
    CCWindow[i].astText[u8Row][u8Column].u8IsPenChanged = 1;
    pstPen = &CCWindow[i].astText[u8Row][u8Column].stPen;
    memcpy(pstPen,&CCWindow[i].stCurPen,sizeof(CC708_PenDef_S));

    return HI_SUCCESS;

}

HI_S32 CC708_OSD_ETX(HI_VOID)
{

    /*draw pending row first*/
    _DtvCC_DrawPendingRow();

    return HI_SUCCESS;
}


/******************************************************************************
 * function name: CC708_OSD_BSText(HI_VOID)                                           *
 * description:   BackSpace                                                   *
 ******************************************************************************/
HI_S32 CC708_OSD_BSText(HI_VOID)
{

    HI_S32 i = CurrentWindowID;
    HI_S32 s32X,s32Y,s32CurRow, s32CurColumn;
    HI_U16 au16Buf[2];
    HI_S32 s32Width = 0;
    HI_S32 s32Height = 0;
    CC708_Cursor_S *pstCursor;

    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    pstCursor = &CCWindow[i].stCursor;
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);
    s32CurColumn = CC708_Cursor_GetCurrentColumn(pstCursor);

     if( s32CurColumn > 0 )
     {
        if( CCWindow[i].u8IsVisible )
        {
            /* get current cursor position */
            s32X = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);
            s32Y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);

            /* calculate last character size */
            au16Buf[0] = CCWindow[i].astText[s32CurRow][s32CurColumn-1].u16Caption;
            _DtvCC_GetTextSize(i,au16Buf,1,&s32Width,&s32Height);

            /* clip the s32Width within the window */
            if((s32X - s32Width)< CCWindow[i].u16X)
            {
                s32Width = s32X - CCWindow[i].u16X;
            }

            /* erase the last character */
            _DtvCC_FillRect(i,s32X - s32Width,s32Y,s32Width,s32Height,0,CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection,HI_FALSE);

            if((CCWindow[i].astText[s32CurRow][s32CurColumn].u16Caption == 0) ||
               (CCWindow[i].astText[s32CurRow][s32CurColumn].u16Caption == 0x1020) ||
               (CCWindow[i].astText[s32CurRow][s32CurColumn].u16Caption == 0x1021))
            {
                _DtvCC_FillRect(i,s32X,s32Y,TAILINGSPACEWIDTH,s32Height,0,
                         CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection,HI_FALSE);
            }
            /* move the cursor backward */
            CC708_Cursor_MoveCursorBackwardsOnScreen(pstCursor,s32Width,
                           CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);

            if (s32CurColumn-2 >= 0 && CCWindow[i].astText[s32CurRow][s32CurColumn-2].u16Caption != 0)
            {
                //set color
                HI_U8 clear_color=0;
                if (CCWindow[i].astText[s32CurRow][s32CurColumn-2].u16Caption==0x1020||
                      CCWindow[i].astText[s32CurRow][s32CurColumn-2].u16Caption==0x1021)
                {
                    clear_color = 1;  //using screen color
                }
                else
                {
                    clear_color = 2;  //using bgcolor
                }
                //draw  tailing
                _DtvCC_DrawLeadingSpace(i,pstCursor->s32X,pstCursor->s32Y,clear_color,HI_TRUE,HI_FALSE);
            }
        }

        /* remove the caption in last column */
        CCWindow[i].astText[s32CurRow][s32CurColumn-1].u16Caption = 0;

        /* move the cursor backward */
        CC708_Cursor_MoveCursorBackwardsOffScreen(pstCursor,1,
                 CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);

        if((s32CurColumn == 1)||
           ((s32CurColumn > 1) && (CCWindow[i].astText[s32CurRow][s32CurColumn-2].u16Caption == 0)) ||
           ((s32CurColumn > 1) && (CCWindow[i].astText[s32CurRow][s32CurColumn-2].u16Caption == 0x1020)))
        {
            _DtvCC_ClearLeadingSpace(i);
        }

     }

return HI_SUCCESS;
}

/******************************************************************************
 * function name: CC708_OSD_FFText(HI_VOID)                                                *
 * description:   This is a form feed,It erase all text in the window and place
 *                the cursor at the first character position in the window(0.0)
 *                This is equivalent to specifying the window in a ClearWindow
 *                command, followed by CC708SetPenLocation(0,0).                   *
 *                Refer to Guidance p6                                        *
 ******************************************************************************/
HI_S32 CC708_OSD_FFText(HI_VOID)
{
    HI_S32 i = CurrentWindowID;
    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    _DtvCC_ClearWindow(i);/* clear window */

    return HI_SUCCESS;
}

/*Row_up mode*/
HI_S32 CC708_OSD_CRText(HI_VOID)
{
    HI_S32 i = CurrentWindowID;
    HI_U8 u8CurRow = 0;
    u8CurRow = (HI_U8)CC708_Cursor_GetCurrentRow(&CCWindow[i].stCursor);
    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    OSD_TRACE("CC708_OSD_CRText at row:%d column:%d,CCWindow[i].u8RowCount:%d\n",CC708_Cursor_GetCurrentRow(&CCWindow[i].stCursor),CC708_Cursor_GetCurrentColumn(&CCWindow[i].stCursor),CCWindow[i].u8RowCount);

    /*draw pending row first*/
    _DtvCC_DrawPendingRow();

    /* if the next line is not beyond the extent of the window, */
    /* the cursor is moved to the beginning of that line. */
    /* See Guide P6 */
    if( u8CurRow < CCWindow[i].u8RowCount )
    {
        /* move cursor */
        u8CurRow += 1;
        _DtvCC_MoveCursorByRowAndColumn(i,u8CurRow,0);

        /* set pen attributes for the first column */
        if(CCWindow[i].u8IsVisible)
        {
            memcpy(&CCWindow[i].astText[u8CurRow][0].stPen,&CCWindow[i].stCurPen,sizeof(CC708_PenDef_S));
            CCWindow[i].astText[u8CurRow][0].u8IsPenChanged = 1;
        }
    }
    /* if the next line would be beyond the extend of the window, */
    /* it "rolls up" the window. */
    /* See Guide P5 */
    else
    {
        HI_S32 overlapHigherWindowMap = CCWindow[i].s32OverlapHigherWindowMap;

        /* roll up the window */
        _DtvCC_ScrollRows(i,1,CCWindow[i].enTextPrintDirection,CCWindow[i].enTextScrollDirection);

        if((overlapHigherWindowMap != 0) &&(CCWindow[i].u16Width*CCWindow[i].u16Height > 1920*60))
        {
            _DtvCC_ReDrawWindow(i,HI_FALSE);
        }

        /* move cursor */
        _DtvCC_MoveCursorByRowAndColumn(i,u8CurRow,0);

        /*recalculate window size*/
        if(CCWindow[i].enTextJustify == HI_UNF_CC_JUSTIFY_LEFT)
        {
            HI_BOOL bChanged;

            bChanged = _DtvCC_CalcWinPosByAnchorPoint(i);

            if(bChanged)
            {
                _DtvCC_ReDrawWindow(i,HI_FALSE);
            }
        }
    }

    return HI_SUCCESS;
}


/********************************************************************************
 * function name: CC708_OSD_HCRText(HI_VOID)                                                 *
 * description:   This is a horizontal carriage return. It move the current entry*
 *                point to the beginning of the current line(no line increment/ *
 *                decrement performance). It also should erase all the text on  *
 *                that line. Guidance  P6                                       *
 ********************************************************************************/
HI_S32 CC708_OSD_HCRText(HI_VOID)
{
    HI_S32 i = CurrentWindowID;
    HI_S32 s32CurRow;
    CC708OSDPARAM_CHECK_RETURN_VAL(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID",HI_FAILURE);

    s32CurRow = CC708_Cursor_GetCurrentRow(&CCWindow[i].stCursor);

    /* erase all the text on the current line */
    _DtvCC_ClearRow(i,s32CurRow);

    /* move the cursor to the beginning of the current line */
    _DtvCC_MoveCursorByRowAndColumn(i,s32CurRow,0);

    return HI_SUCCESS;
}


/********************************************************************************
 * function name: CC708_OSD_Start(HI_VOID)                                       *
 * description:  Init windows
 ********************************************************************************/
HI_S32 CC708_OSD_Start(HI_VOID)
{
    HI_U8 i;

    for( i = 0; i < MAX_CC_WINDOWS;i++ )
    {
        _DtvCC_InitWindow(i);
    }

    CurrentWindowID = -1;
    _DtvCC_EraseTimerInit(0);

    return HI_SUCCESS;
}

HI_S32 CC708_OSD_Stop(HI_VOID)
{
    _DtvCC_EraseTimerStop();
    return HI_SUCCESS;
}


/********************************************************************************
 * function name: CC708_OSD_Reset(HI_VOID)                                       *
 * description:  Reset windows
 ********************************************************************************/
HI_S32 CC708_OSD_Reset(HI_VOID)
{
    HI_S32 i;
    CC708_OBJECT_S *pCCObj = CC708_OBJ_GetHandle(0);

    for( i=0; i<8; i++ )
    {
        /* delete all defined windows */
        if(CCWindow[i].s32WindowID != -1)
        {
            _DtvCC_DeleteWindow(i);
        }

        CurrentWindowID = -1;
    }
    /*we should clear the screen*/
    (HI_VOID)CCDISP_Screen_Clear();

    CCQueue_Flush(&pCCObj->stServiceQueue);

    return HI_SUCCESS;
}

HI_S32 CC708_OSD_ClrCCScreen(HI_VOID)
{
    return CCDISP_Screen_Clear();
}

HI_VOID CC708_OSD_DrawCaptionToWindow(HI_U16 *pu16Text,HI_S32 s32Len)
{
    HI_S32 WinID = CurrentWindowID;
    HI_S32 s32CurRow,s32CurColumn/*,s32X,s32Y*/;
    //HI_S32 j;
    //HI_U8 u8HasCaption = 0;
    //HI_BOOL bPenAttrChanged = HI_TRUE;
    //HI_BOOL bCaptionChanged = HI_FALSE;
    CC708_Cursor_S *pstCursor = &CCWindow[WinID].stCursor;
    //CC708_PenDef_S *pstPrevPen, *pstCurPen;

    CC708OSDPARAM_CHECK_RETURN_VOID(CurrentWindowID,-1,MAX_CC_WINDOWS,"CurrentWindowID");
    if(s32Len <= 0 || HI_NULL == pu16Text)
    {
        return;
    }
    /* firstly, put caption into window offscreen caption buffer */
    s32CurRow = CC708_Cursor_GetCurrentRow(pstCursor);
    s32CurColumn = CC708_Cursor_GetCurrentColumn(pstCursor);

#if 0    
    pstPrevPen = &CCWindow[WinID].stPrevPen;
    pstCurPen = &CCWindow[WinID].stCurPen;

    if((pstPrevPen->u8IsItalic == pstCurPen->u8IsItalic) &&
        (pstPrevPen->enEdgeType == pstCurPen->enEdgeType) &&
        (pstPrevPen->enFontName == pstCurPen->enFontName) &&
        (pstPrevPen->enFontSize == pstCurPen->enFontSize) &&
        (pstPrevPen->enTextTag == pstCurPen->enTextTag))
    {
        bPenAttrChanged = HI_FALSE;
    }

    for(j = 0; j < s32Len; j++)
    {
        if(pu16Text[j] != CCWindow[WinID].astText[s32CurRow][s32CurColumn+j].u16Caption)
        {
            bCaptionChanged = HI_TRUE;
            break;
        }
    }

    /* if there are captions after current Column in current Row, the whole Row need redraw */
    for(j = s32CurColumn;j <= CCWindow[WinID].u8ColumnCount;j++)
    {
        if(CCWindow[WinID].astText[s32CurRow][j].u16Caption)
        {
            u8HasCaption = 0;
            OSD_TRACE("CCWindow[%d].astText[%d][%d].u16Caption:0x%x\n",
                WinID,s32CurRow,j,CCWindow[WinID].astText[s32CurRow][j].u16Caption);
        }
    }
#endif

    _DtvCC_DrawCaptionToWindowOffScreen(pu16Text,s32Len);

    if(s32Len > (CCWindow[WinID].u8ColumnCount + 1 - s32CurColumn))
    {
        s32Len = CCWindow[WinID].u8ColumnCount + 1 - s32CurColumn;
    }
    /*first end*/

    /* secondly, draw caption on window */
    if(CCWindow[WinID].u8IsVisible)
    {
        /*if not left justified and visible, don't draw until the next CC control command ??*/
        if(CCWindow[WinID].enTextJustify != HI_UNF_CC_JUSTIFY_LEFT)
        {
            CCWindow[WinID].bPendingRow = HI_TRUE;
            CCWindow[WinID].s32PendingRow = s32CurRow;
            return;
        }
        #if 0
        if(u8HasCaption && ((bCaptionChanged == HI_TRUE) || (bPenAttrChanged == HI_TRUE)))
        {
            _DtvCC_CalcDrawStartPositionByRow(WinID,s32CurRow);
            s32X = CC708_Cursor_GetCurrentXPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,
                                                     CCWindow[WinID].enTextScrollDirection);
            s32Y = CC708_Cursor_GetCurrentYPosition(pstCursor,CCWindow[WinID].enTextPrintDirection,
                                                     CCWindow[WinID].enTextScrollDirection);
            if(CCWindow[WinID].as32RowHeight[s32CurRow] > 0)
            {
                _DtvCC_FillRect(WinID,s32X,s32Y,CCWindow[WinID].u16Width,CCWindow[WinID].as32RowHeight[s32CurRow],
                               0,CC_PD_LEFT_RIGHT,CC_SD_BOTTOM_TOP,HI_TRUE);
                _DtvCC_DrawRowText(WinID,s32X,s32Y,s32CurRow,HI_TRUE);
                _DtvCC_MoveCursorByRowAndColumn(WinID,s32CurRow,s32CurColumn + s32Len);
            }
        }
        else
        #endif
        {
            _DtvCC_DrawCaptionToWindowOnScreen(pu16Text,s32Len,s32CurRow,s32CurColumn);
        }

        _DtvCC_SetLastDisplayOrder(WinID);
    }
    CCWindow[WinID].u8IsEmpty = 0;
}

/***********************************************************************************
*                        End of file
************************************************************************************/
