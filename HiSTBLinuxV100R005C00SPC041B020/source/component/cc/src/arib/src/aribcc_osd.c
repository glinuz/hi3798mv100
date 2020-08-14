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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
//#include <sys/prctl.h>
#include <assert.h>

#include"hi_type.h"
#include"cc_debug.h"
#include"aribcc_osd.h"
#include"aribcc_graphic.h"

/****************************************************************
MACRO DEFINITION
*****************************************************************/

#define VERTICAL_CENTER_BASE_LINE 1
#define MAX_ARIBCC_NUM 2
#define UNDEFC 0


/****************************************************************

*****************************************************************/

extern HI_S32 Audio_PlayARIBSound ( HI_U32 ulIdx, HI_U32 ulRepeat );


/****************************************************************

*****************************************************************/
static ARIBCC_TEXTATTR_S g_astArib_text[MAX_ARIBCC_NUM];
volatile HI_U8 gIsHscrollCancel = 0;

/*variable to control whether the cc show is enabled or not. for multi program to share one plane*/
volatile HI_U8 gIsCCShowEnable = 1;
static HI_U8 gIsTextBitmapDrawn = 0;
static char gLatestCCType = -1;

static HI_U8 gUnicodeFirstByte = 0;
static HI_U16 gUnicodeDrcs0Value = 0;


/****************************************************************

*****************************************************************/

static ARIBCC_TEXTATTR_S *_AribCC_GetAribText(HI_S32 CCType)
{
    if (CCType >= MAX_ARIBCC_NUM)
    {
        return NULL;
    }
    return &g_astArib_text[CCType];
}

static HI_U8 _AribCC_IsNonspaceCode(HI_U16 u16UnicodeChar)
{
    HI_U16 au16NonspaceCode[7]={0x00b4,0xff40,0x00a8,0xff3e,0xffe3,0xff3f,0x25ef};
    HI_U16 i;
    for(i=0; i<7; i++)
    {
        if( u16UnicodeChar == au16NonspaceCode[i] )
        {
            return 1;
        }
    }
    return 0;
}

/*********************************************************************************
is_interval_include:   0: the character size returned  don't include character space and line interval
                               1: the character size returned  include character space and line interval
 **********************************************************************************/
static void _AribCC_CalcCharSize(HI_S32 CCType,HI_S32 s32IsIntervalInclude,HI_S32 *s32width, HI_S32 *s32height)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);
    /* FontScale meaning(for horizontal print):
            0-standard
            1-1/4size,
            2-1/2size,
            3-horizontal double,
            4- vertical double
            5-double in both direction
    */
    HI_U8 au8DoubleWidthRatio[6]={2,1,1,2,4,4};
    HI_U8 au8DoubleHeightRatio[6]={2,1,2,4,2,4};

    HI_U8 u8CharInterval = 0,u8LineInterval = 0;

    if( s32width == NULL || s32height == NULL || pstText == NULL)
    {
        return;
    }

    if( s32IsIntervalInclude)
    {
        u8CharInterval = pstText->u8CharInterval;
        u8LineInterval = pstText->u8LineInterval;
    }

    if(pstText->u8FontScale > 5)
    {
        return;
    }
    *s32width = ((pstText->u8FontWidth + u8CharInterval)*au8DoubleWidthRatio[pstText->u8FontScale])>>1;
    *s32height = ((pstText->u8FontHeight + u8LineInterval)*au8DoubleHeightRatio[pstText->u8FontScale])>>1;

    /*FontScale meaning(for vertical print)*/
    if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        au8DoubleWidthRatio[2] = 2;
        au8DoubleHeightRatio[2] = 1;
        *s32width = ((pstText->u8FontWidth + u8LineInterval)*au8DoubleWidthRatio[pstText->u8FontScale])>>1;
        *s32height = ((pstText->u8FontHeight + u8CharInterval)*au8DoubleHeightRatio[pstText->u8FontScale])>>1;
    }
    return;
}

static void _AribCC_CheckAndReviseWinPos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);
    if(pstText == NULL)
    {
        return;
    }

    if( pstText->stWinAttr.u16x + pstText->stWinAttr.u16width > pstText->stFormat.res_w )
    {
        pstText->stWinAttr.u16x = pstText->stFormat.res_w - pstText->stWinAttr.u16width;
    }

    if( pstText->stWinAttr.u16y + pstText->stWinAttr.u16height > pstText->stFormat.res_h )
    {
        pstText->stWinAttr.u16y = pstText->stFormat.res_h - pstText->stWinAttr.u16height;
    }

    return;
}


void _AribCC_SetDefaultCursor(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;

    if(pText == NULL)
    {
        return;
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);

    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        pText->stCursor.u16X = pText->stWinAttr.u16width; // - char_width;
        pText->stCursor.u16Y = 0;
    }
    else
    {
        pText->stCursor.u16X = 0;
        pText->stCursor.u16Y = 0;//char_height;
    }
    pText->stCursor.u16Column = 0;
    pText->stCursor.u16Row = 0;
    return;
}

/****************************************************************************
Active Position backward, Active position goes backward along character path in the length of
character field. When reference point of the character field exceeds the edge of display area
by this movement, move in the opposite side of the display area along the character path of
the active position, for active position up continue to backspace, if reach the column 0. then
return the previous line.
****************************************************************************/

static void _AribCC_BackwardActivePos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;

    if(HI_NULL == pText)
    {
        return;
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);
    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        /*when the reference point exceeds the display area. return the previous line*/
        if( pText->stCursor.u16X < char_width )
        {
            pText->stCursor.u16MaxCol = pText->stWinAttr.u16width/char_width;
            pText->stCursor.u16Column = pText->stCursor.u16MaxCol - 1;
            pText->stCursor.u16X = pText->stWinAttr.u16width - char_width;
            if( pText->stCursor.u16Y > char_height )
            {
                pText->stCursor.u16Row--;
                pText->stCursor.u16Y -= char_height;
            }
            else
            {
                /*return to the last row*/
                pText->stCursor.u16MaxRow = pText->stWinAttr.u16height/char_height;
                pText->stCursor.u16Row = pText->stCursor.u16MaxRow - 1;
                pText->stCursor.u16Y = pText->stWinAttr.u16height;
            }
        }
        else
        {
            pText->stCursor.u16Column--;
            pText->stCursor.u16X -= char_width;
        }
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        if( pText->stCursor.u16Y < char_height )
        {
            pText->stCursor.u16MaxCol = pText->stWinAttr.u16height/char_height;
            pText->stCursor.u16Column = pText->stCursor.u16MaxCol - 1;
            pText->stCursor.u16Y = pText->stWinAttr.u16height - char_height;
#ifdef VERTICAL_CENTER_BASE_LINE
            if( pText->stCursor.u16X + ((char_width*3)>>1) < pText->stWinAttr.u16width )
#else
            if( pText->stCursor.u16X + char_width < pText->stWinAttr.u16width )
#endif
            {
                pText->stCursor.u16Row--;
                pText->stCursor.u16X += char_width;
            }
            else
            {
                /*return to the last row(line)*/
                pText->stCursor.u16MaxRow = pText->stWinAttr.u16width/char_width;
                pText->stCursor.u16Row = pText->stCursor.u16MaxRow - 1;
#ifdef VERTICAL_CENTER_BASE_LINE
                pText->stCursor.u16X = char_width>>1;
#else
                pText->stCursor.u16X = 0;
#endif
            }
        }
        else
        {
            pText->stCursor.u16Column--;
            pText->stCursor.u16Y -= char_height;
        }
    }
    return;
}

/****************************************************************************
Active position goes forward along character path in the length of character path of character
field. when the reference point of the character field exceeds the edge of display area by this
movement, move in the opposite side of the display area along the character path of the active
position, for active position down
go forward, if reach the last column. change line and go to the first column
************************************************************************/
static void _AribCC_ForwardActivePos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;
    HI_U8 font_scale = 0;

    if(HI_NULL == pText)
    {
        return;
    }

    font_scale = pText->u8FontScale;

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        /*when the reference point exceeds the display area. return the previous line*/
        if( pText->stCursor.u16X + char_width >= pText->stWinAttr.u16width )
        {
            pText->u8FontScale = 0;
            _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
            pText->stCursor.u16Column = 0;
            pText->stCursor.u16X = 0;
            if( pText->stCursor.u16Y + char_height <= pText->stWinAttr.u16height )
            {
                pText->stCursor.u16Row++;
                pText->stCursor.u16Y += char_height;
            }
            else
            {
                /*if reach the last line. change to first line*/
                pText->stCursor.u16Row = 0;
                pText->stCursor.u16Y = char_height;
            }
        }
        else
        {
            pText->stCursor.u16Column++;
            pText->stCursor.u16X += char_width;
        }
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        if( pText->stCursor.u16Y + char_height >= pText->stWinAttr.u16height )
        {
            pText->stCursor.u16Column = 0;
            pText->stCursor.u16Y = 0;
            if( pText->stCursor.u16X >= char_width )
            {
                pText->stCursor.u16Row++;
                pText->stCursor.u16X -= char_width;
            }
            else
            {
                pText->stCursor.u16Row = 0;
#ifdef VERTICAL_CENTER_BASE_LINE
                pText->stCursor.u16X = pText->stWinAttr.u16width - (char_width>>1);
#else
                pText->stCursor.u16X = pText->stWinAttr.u16width - char_width;
#endif
            }
        }
        else
        {
            pText->stCursor.u16Column++;
            pText->stCursor.u16Y += char_height;
        }
    }

    pText->u8FontScale = font_scale;

    return;
}

/*************************************************************************
Moves to the next line along line direction in the length of line direction of the character field
When the reference point of the character field exceeds the edge of display area by this movement,
move to the first line of the display area along the line direction
*************************************************************************/
static void _AribCC_DownActivePos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;

    if(HI_NULL == pText)
    {
        return;
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
            /*when the reference point exceeds the display area. return the first line*/
        if( pText->stCursor.u16Y + char_height > pText->stWinAttr.u16height )
        {
            pText->stCursor.u16Row = 0;
            pText->stCursor.u16Y = char_height;
        }
        else
        {
            pText->stCursor.u16Row++;
            pText->stCursor.u16Y += char_height;
        }
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        if( pText->stCursor.u16X < char_width )
        {
            pText->stCursor.u16Row = 0;
#ifdef VERTICAL_CENTER_BASE_LINE
            pText->stCursor.u16X = pText->stWinAttr.u16width - (char_width>>1);
#else
            pText->stCursor.u16X = pText->stWinAttr.u16width - char_width;
#endif
        }
        else
        {
            pText->stCursor.u16Row++;
            pText->stCursor.u16X -= char_width;
        }
    }

    return;
}

/**************************************************************************
Moves to the previous line along line direction in the length of line direction of the character
field. When the reference point of the character field exceeds the edge of display area by this
movement, move to the last line of the display area along the line direction
***************************************************************************/
static void _AribCC_UpActivePos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;
    if (HI_NULL == pText)
    {
        return;
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
    if(( 0== char_width) || ( 0 == char_height))
    {
        return;
    }
    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        /*when the reference point exceeds the display area. return the first line*/
        if( pText->stCursor.u16Y <= char_height )
        {
            pText->stCursor.u16MaxRow = pText->stWinAttr.u16height/char_height;
            pText->stCursor.u16Row = pText->stCursor.u16MaxRow - 1;
            pText->stCursor.u16Y = pText->stWinAttr.u16height;// - char_height;
        }
        else
        {
            pText->stCursor.u16Row--;
            pText->stCursor.u16Y -= char_height;
        }
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
#ifdef VERTICAL_CENTER_BASE_LINE
        if( pText->stCursor.u16X + (char_width>>1) >= pText->stWinAttr.u16width )
#else
        if( pText->stCursor.u16X + char_width >= pText->stWinAttr.u16width )
#endif
        {
            pText->stCursor.u16MaxRow = pText->stWinAttr.u16width/char_width;
            pText->stCursor.u16Row = pText->stCursor.u16MaxRow - 1;
#ifdef VERTICAL_CENTER_BASE_LINE
            pText->stCursor.u16X = (char_width>>1);
#else
            pText->stCursor.u16X = 0;
#endif
        }
        else
        {
            pText->stCursor.u16Row--;
            pText->stCursor.u16X += char_width;
        }
    }

    return;
}

/*****************************************************************
Active position return, Active position down is made, moving to the first position
of the same line
*******************************************************************/
static void _AribCC_ReturnActivePos(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if (HI_NULL == pText)
    {
        return;
    }

    OSD_TRACE("cursor1:x=%d,y=%d,row=%d,column=%d\n",pText->stCursor.u16X,pText->stCursor.u16Y,
                 pText->stCursor.u16Row,pText->stCursor.u16Column);
    _AribCC_DownActivePos(CCType);
    OSD_TRACE("cursor2:x=%d,y=%d,row=%d,column=%d\n",pText->stCursor.u16X,pText->stCursor.u16Y,
                 pText->stCursor.u16Row,pText->stCursor.u16Column);

    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        pText->stCursor.u16Column = 0;
        pText->stCursor.u16X = 0;
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        pText->stCursor.u16Column = 0;
        pText->stCursor.u16Y = 0;
    }
    return;
}

/***************************************************************************
Parameerized active position forward, Active position forward is made in specified times by num
The num range is between 0 and 63
***************************************************************************/
static void _AribCC_ForwardActivePosByParam(HI_S32 CCType,HI_S32 num)
{
    HI_S32 i = 0;
    for( i = 0; i < num; i++ )
    {
        _AribCC_ForwardActivePos(CCType);
    }
    return;
}

/**************************************************************
Active position set, row times position down, column times position forward
the range of row and column is 0-63
****************************************************************/
static void _AribCC_SetActivePos(HI_S32 CCType,HI_S32 row, HI_S32 column)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 i,char_width = 0,char_height = 0;

    if (HI_NULL == pText)
    {
        return;
    }
    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);

    _AribCC_SetDefaultCursor(CCType);

    /*revise the cursor*/
    if( pText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        if( pText->stCursor.u16Y == 0 )
        {
            pText->stCursor.u16Y += char_height;
        }
    }
    else if( pText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
        if( pText->stCursor.u16X == pText->stWinAttr.u16width )
        {
#ifdef VERTICAL_CENTER_BASE_LINE
            pText->stCursor.u16X -= (char_width>>1);
#else
            pText->stCursor.u16X -= char_width;
#endif
        }
    }
    for( i = 0; i < row; i++ )
    {
        _AribCC_DownActivePos(CCType);
    }
    for( i = 0; i < column; i++ )
    {
        _AribCC_ForwardActivePos(CCType);
    }

    return;
}


/*Active Coordinate Position Set, we assume that the coordinate is relative to the window left-top*/
static void _AribCC_SetActivePosByCoordinate(HI_S32 CCType,HI_S32 x, HI_S32 y)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    if(HI_NULL == pText)
    {
        return;
    }

    if( x >= pText->stWinAttr.u16x )
    {
        pText->stCursor.u16X = x - pText->stWinAttr.u16x;
    }
    else
    {
        pText->stCursor.u16X = 0;
    }

    if( y >= pText->stWinAttr.u16y )
    {
        pText->stCursor.u16Y = y - pText->stWinAttr.u16y;
    }
    else
    {
        pText->stCursor.u16Y = 0;
    }
    return;
}

static HI_S32 _AribCC_SwapColor(HI_UNF_CC_COLOR_S *pColor1, HI_UNF_CC_COLOR_S *pColor2)
{
    HI_UNF_CC_COLOR_S color;

    color.u8Alpha = pColor1->u8Alpha;
    color.u8Blue = pColor1->u8Blue;
    color.u8Green = pColor1->u8Green;
    color.u8Red = pColor1->u8Red;

    pColor1->u8Alpha = pColor2->u8Alpha;
    pColor1->u8Blue = pColor2->u8Blue;
    pColor1->u8Green = pColor2->u8Green;
    pColor1->u8Red = pColor2->u8Red;

    pColor2->u8Alpha = color.u8Alpha;
    pColor2->u8Blue = color.u8Blue;
    pColor2->u8Green = color.u8Green;
    pColor2->u8Red = color.u8Red;
    return 0;
}


/*map the arib character set to unicode set*/
static HI_U16 _AribCC_MapUnicode(HI_U8 u8Text, HI_U8 u8CharSet,ARIBCC_PRTDIRECT_E stPrtDirection,HI_U8 *IsNonspace)
{
    HI_U16 ret_unicode = 0;

    static HI_U16 unicodeforkanji[83][94] = {
    /*1 area from 2121*/
    /*area 1 61 0x2212 can't be shown. so changed to ff0d*/
   {0x3000,0x3001,0x3002,0xFF0C,0xFF0E,0x30FB,0xFF1A,0xFF1B,0xFF1F,0xFF01,0x309B,0x309C,0x00B4,0xFF40,0x00A8,0xFF3E,
    0xFFE3,0xFF3F,0x30FD,0x30FE,0x309D,0x309E,0x3003,0x4EDD,0x3005,0x3006,0x3007,0x30FC,0x2015,0x2010,0xFF0F,0xff3C,
    0xff5e,0x2016,0xFF5C,0x2026,0x2025,0x2018,0x2019,0x201C,0x201D,0xFF08,0xFF09,0x3014,0x3015,0xFF3B,0xFF3D,0xFF5B,
    0xFF5D,0x3008,0x3009,0x300A,0x300B,0x300C,0x300D,0x300E,0x300F,0x3010,0x3011,0xFF0B,0xff0d,0x00B1,0x00D7,0x00F7,
    0xFF1D,0x2260,0xFF1C,0xFF1E,0x2266,0x2267,0x221E,0x2234,0x2642,0x2640,0x00B0,0x2032,0x2033,0x2103,0xFFE5,0xFF04,
    0xffe0,0xffe1,0xFF05,0xFF03,0xFF06,0xFF0A,0xFF20,0x00A7,0x2606,0x2605,0x25CB,0x25CF,0x25CE,0x25C7},
    /*2 area from 2221*/
    {0x25C6,0x25a1,0x25A0,0x25B3,0x25B2,0x25BD,0x25BC,0x203B,0x3012,0x2192,0x2190,0x2191,0x2193,0x3013,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0x2208,0x220B,0x2286,0x2287,0x2282,0x2283,0x222A,
    0x2229,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0x2227,0x2228,0xffe2,0x21D2,0x21D4,0x2200,0x2203,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0x2220,0x22A5,0x2312,0x2202,0x2207,
    0x2261,0x2252,0x226A,0x226B,0x221A,0x223D,0x221D,0x2235,0x222B,0x222C,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,0x212B,0x2030,0x266F,0x266D,0x266A,0x2020,0x2021,0x00B6,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0x25EF},
    /*3 area from 2321*/
    {UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0xFF10,
    0xFF11,0xFF12,0xFF13,0xFF14,0xFF15,0xFF16,0xFF17,0xFF18,0xFF19,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    0xFF21,0xFF22,0xFF23,0xFF24,0xFF25,0xFF26,0xFF27,0xFF28,0xFF29,0xFF2A,0xFF2B,0xFF2C,0xFF2D,0xFF2E,0xFF2F,0xFF30,
    0xFF31,0xFF32,0xFF33,0xFF34,0xFF35,0xFF36,0xFF37,0xFF38,0xFF39,0xFF3A,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    0xFF41,0xFF42,0xFF43,0xFF44,0xFF45,0xFF46,0xFF47,0xFF48,0xFF49,0xFF4A,0xFF4B,0xFF4C,0xFF4D,0xFF4E,0xFF4F,0xFF50,
    0xFF51,0xFF52,0xFF53,0xFF54,0xFF55,0xFF56,0xFF57,0xFF58,0xFF59,0xFF5A,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*4 area from 2421*/
    {0x3041,0x3042,0x3043,0x3044,0x3045,0x3046,0x3047,0x3048,0x3049,0x304A,0x304B,0x304C,0x304D,0x304E,0x304F,0x3050,
    0x3051,0x3052,0x3053,0x3054,0x3055,0x3056,0x3057,0x3058,0x3059,0x305A,0x305B,0x305C,0x305D,0x305E,0x305F,0x3060,
    0x3061,0x3062,0x3063,0x3064,0x3065,0x3066,0x3067,0x3068,0x3069,0x306A,0x306B,0x306C,0x306D,0x306E,0x306F,0x3070,
    0x3071,0x3072,0x3073,0x3074,0x3075,0x3076,0x3077,0x3078,0x3079,0x307A,0x307B,0x307C,0x307D,0x307E,0x307F,0x3080,
    0x3081,0x3082,0x3083,0x3084,0x3085,0x3086,0x3087,0x3088,0x3089,0x308A,0x308B,0x308C,0x308D,0x308E,0x308F,0x3090,
    0x3091,0x3092,0x3093,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*5 area from 2521*/
    {0x30A1,0x30A2,0x30A3,0x30A4,0x30A5,0x30A6,0x30A7,0x30A8,0x30A9,0x30AA,0x30AB,0x30AC,0x30AD,0x30AE,0x30AF,0x30B0,
    0x30B1,0x30B2,0x30B3,0x30B4,0x30B5,0x30B6,0x30B7,0x30B8,0x30B9,0x30BA,0x30BB,0x30BC,0x30BD,0x30BE,0x30BF,0x30C0,
    0x30C1,0x30C2,0x30C3,0x30C4,0x30C5,0x30C6,0x30C7,0x30C8,0x30C9,0x30CA,0x30CB,0x30CC,0x30CD,0x30CE,0x30CF,0x30D0,
    0x30D1,0x30D2,0x30D3,0x30D4,0x30D5,0x30D6,0x30D7,0x30D8,0x30D9,0x30DA,0x30DB,0x30DC,0x30DD,0x30DE,0x30DF,0x30E0,
    0x30E1,0x30E2,0x30E3,0x30E4,0x30E5,0x30E6,0x30E7,0x30E8,0x30E9,0x30EA,0x30EB,0x30EC,0x30ED,0x30EE,0x30EF,0x30F0,
    0x30F1,0x30F2,0x30F3,0x30F4,0x30F5,0x30F6,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*6 area from 2621*/
    {0x0391,0x0392,0x0393,0x0394,0x0395,0x0396,0x0397,0x0398,0x0399,0x039A,0x039B,0x039C,0x039D,0x039E,0x039F,0x03A0,
    0x03A1,0x03A3,0x03A4,0x03A5,0x03A6,0x03A7,0x03A8,0x03A9,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    0x03B1,0x03B2,0x03B3,0x03B4,0x03B5,0x03B6,0x03B7,0x03B8,0x03B9,0x03BA,0x03BB,0x03BC,0x03BD,0x03BE,0x03BF,0x03C0,
    0x03C1,0x03C3,0x03C4,0x03C5,0x03C6,0x03C7,0x03C8,0x03C9,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*7 area form 2721*/
    {0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0401,0x0416,0x0417,0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,
    0x041F,0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,
    0x042F,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0451,0x0436,0x0437,0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,
    0x043F,0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,
    0x044F,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*8 area from 2821*/
    {0x2500,0x2502,0x250C,0x2510,0x2518,0x2514,0x251C,0x252C,0x2524,0x2534,0x253C,0x2501,0x2503,0x250F,0x2513,0x251B,
    0x2517,0x2523,0x2533,0x252B,0x253B,0x254B,0x2520,0x252F,0x2528,0x2537,0x253F,0x251D,0x2530,0x2525,0x2538,0x2542,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},

    /*9-15 areas are not used*/

    /*16 area from 3021*/
    {0x4E9C,0x5516,0x5A03,0x963F,0x54C0,0x611B,0x6328,0x59F6,0x9022,0x8475,0x831C,0x7A50,0x60AA,0x63E1,0x6E25,0x65ED,
    0x8466,0x82A6,0x9BF5,0x6893,0x5727,0x65A1,0x6271,0x5B9B,0x59D0,0x867B,0x98F4,0x7D62,0x7DBE,0x9B8E,0x6216,0x7C9F,
    0x88B7,0x5B89,0x5EB5,0x6309,0x6697,0x6848,0x95C7,0x978D,0x674F,0x4EE5,0x4F0A,0x4F4D,0x4F9D,0x5049,0x56F2,0x5937,
    0x59D4,0x5A01,0x5C09,0x60DF,0x610F,0x6170,0x6613,0x6905,0x70BA,0x754F,0x7570,0x79FB,0x7DAD,0x7DEF,0x80C3,0x840E,
    0x8863,0x8B02,0x9055,0x907A,0x533B,0x4E95,0x4EA5,0x57DF,0x80B2,0x90C1,0x78EF,0x4E00,0x58F1,0x6EA2,0x9038,0x7A32,
    0x8328,0x828B,0x9C2F,0x5141,0x5370,0x54BD,0x54E1,0x56E0,0x59FB,0x5F15,0x98F2,0x6DEB,0x80E4,0x852D},
    /*17 area from 3121*/
    {0x9662,0x9670,0x96A0,0x97FB,0x540B,0x53F3,0x5B87,0x70CF,0x7FBD,0x8FC2,0x96E8,0x536F,0x9D5C,0x7ABA,0x4E11,0x7893,
    0x81FC,0x6E26,0x5618,0x5504,0x6B1D,0x851A,0x9C3B,0x59E5,0x53A9,0x6D66,0x74DC,0x958F,0x5642,0x4E91,0x904B,0x96F2,
    0x834F,0x990C,0x53E1,0x55B6,0x5B30,0x5F71,0x6620,0x66F3,0x6804,0x6C38,0x6CF3,0x6D29,0x745B,0x76C8,0x7A4E,0x9834,
    0x82F1,0x885B,0x8A60,0x92ED,0x6DB2,0x75AB,0x76CA,0x99C5,0x60A6,0x8B01,0x8D8A,0x95B2,0x698E,0x53AD,0x5186,0x5712,
    0x5830,0x5944,0x5BB4,0x5EF6,0x6028,0x63A9,0x63F4,0x6CBF,0x6F14,0x708E,0x7114,0x7159,0x71D5,0x733F,0x7E01,0x8276,
    0x82D1,0x8597,0x9060,0x925B,0x9D1B,0x5869,0x65BC,0x6C5A,0x7525,0x51F9,0x592E,0x5965,0x5F80,0x5FDC},
    /*18 area from 3221*/
    {0x62BC,0x65FA,0x6A2A,0x6B27,0x6BB4,0x738B,0x7FC1,0x8956,0x9D2C,0x9D0E,0x9EC4,0x5CA1,0x6C96,0x837B,0x5104,0x5C4B,
    0x61B6,0x81C6,0x6876,0x7261,0x4E59,0x4FFA,0x5378,0x6069,0x6E29,0x7A4F,0x97F3,0x4E0B,0x5316,0x4EEE,0x4F55,0x4F3D,
    0x4FA1,0x4F73,0x52A0,0x53EF,0x5609,0x590F,0x5AC1,0x5BB6,0x5BE1,0x79D1,0x6687,0x679C,0x67B6,0x6B4C,0x6CB3,0x706B,
    0x73C2,0x798D,0x79BE,0x7A3C,0x7B87,0x82B1,0x82DB,0x8304,0x8377,0x83EF,0x83D3,0x8766,0x8AB2,0x5629,0x8CA8,0x8FE6,
    0x904E,0x971E,0x868A,0x4FC4,0x5CE8,0x6211,0x7259,0x753B,0x81E5,0x82BD,0x86FE,0x8CC0,0x96C5,0x9913,0x99D5,0x4ECB,
    0x4F1A,0x89E3,0x56DE,0x584A,0x58CA,0x5EFB,0x5FEB,0x602A,0x6094,0x6062,0x61D0,0x6212,0x62D0,0x6539},
    /*19 area from 3321*/
    {0x9B41,0x6666,0x68B0,0x6D77,0x7070,0x754C,0x7686,0x7D75,0x82A5,0x87F9,0x958B,0x968E,0x8C9D,0x51F1,0x52BE,0x5916,
    0x54B3,0x5BB3,0x5D16,0x6168,0x6982,0x6DAF,0x788D,0x84CB,0x8857,0x8A72,0x93A7,0x9AB8,0x6D6C,0x99A8,0x86D9,0x57A3,
    0x67FF,0x86CE,0x920E,0x5283,0x5687,0x5404,0x5ED3,0x62E1,0x64B9,0x683C,0x6838,0x6BBB,0x7372,0x78BA,0x7A6B,0x899A,
    0x89D2,0x8D6B,0x8F03,0x90ED,0x95A3,0x9694,0x9769,0x5B66,0x5CB3,0x697D,0x984D,0x984E,0x639B,0x7B20,0x6A2B,0x6A7F,
    0x68B6,0x9C0D,0x6F5F,0x5272,0x559D,0x6070,0x62EC,0x6D3B,0x6E07,0x6ED1,0x845B,0x8910,0x8F44,0x4E14,0x9C39,0x53F6,
    0x691B,0x6A3A,0x9784,0x682A,0x515C,0x7AC3,0x84B2,0x91DC,0x938C,0x565B,0x9D28,0x6822,0x8305,0x8431},
    /*20 area from 3421*/
    {0x7CA5,0x5208,0x82C5,0x74E6,0x4E7E,0x4F83,0x51A0,0x5BD2,0x520A,0x52D8,0x52E7,0x5DFB,0x559A,0x582A,0x59E6,0x5B8C,
    0x5B98,0x5BDB,0x5E72,0x5E79,0x60A3,0x611F,0x6163,0x61BE,0x63DB,0x6562,0x67D1,0x6853,0x68FA,0x6B3E,0x6B53,0x6C57,
    0x6F22,0x6F97,0x6F45,0x74B0,0x7518,0x76E3,0x770B,0x7AFF,0x7BA1,0x7C21,0x7DE9,0x7F36,0x7FF0,0x809D,0x8266,0x839E,
    0x89B3,0x8ACC,0x8CAB,0x9084,0x9451,0x9593,0x9591,0x95A2,0x9665,0x97D3,0x9928,0x8218,0x4E38,0x542B,0x5CB8,0x5DCC,
    0x73A9,0x764C,0x773C,0x5CA9,0x7FEB,0x8D0B,0x96C1,0x9811,0x9854,0x9858,0x4F01,0x4F0E,0x5371,0x559C,0x5668,0x57FA,
    0x5947,0x5B09,0x5BC4,0x5C90,0x5E0C,0x5E7E,0x5FCC,0x63EE,0x673A,0x65D7,0x65E2,0x671F,0x68CB,0x68C4},
    /*21 area from 3521*/
    {0x6A5F,0x5E30,0x6BC5,0x6C17,0x6C7D,0x757F,0x7948,0x5B63,0x7A00,0x7D00,0x5FBD,0x898F,0x8A18,0x8CB4,0x8D77,0x8ECC,
    0x8F1D,0x98E2,0x9A0E,0x9B3C,0x4E80,0x507D,0x5100,0x5993,0x5B9C,0x622F,0x6280,0x64EC,0x6B3A,0x72A0,0x7591,0x7947,
    0x7FA9,0x87FB,0x8ABC,0x8B70,0x63AC,0x83CA,0x97A0,0x5409,0x5403,0x55AB,0x6854,0x6A58,0x8A70,0x7827,0x6775,0x9ECD,
    0x5374,0x5BA2,0x811A,0x8650,0x9006,0x4E18,0x4E45,0x4EC7,0x4F11,0x53CA,0x5438,0x5BAE,0x5F13,0x6025,0x6551,0x673D,
    0x6C42,0x6C72,0x6CE3,0x7078,0x7403,0x7A76,0x7AAE,0x7B08,0x7D1A,0x7CFE,0x7D66,0x65E7,0x725B,0x53BB,0x5C45,0x5DE8,
    0x62D2,0x62E0,0x6319,0x6E20,0x865A,0x8A31,0x8DDD,0x92F8,0x6F01,0x79A6,0x9B5A,0x4EA8,0x4EAB,0x4EAC},
    /*22 area from 3621*/
    {0x4F9B,0x4FA0,0x50D1,0x5147,0x7AF6,0x5171,0x51F6,0x5354,0x5321,0x537F,0x53EB,0x55AC,0x5883,0x5CE1,0x5F37,0x5F4A,
    0x602F,0x6050,0x606D,0x631F,0x6559,0x6A4B,0x6CC1,0x72C2,0x72ED,0x77EF,0x80F8,0x8105,0x8208,0x854E,0x90F7,0x93E1,
    0x97FF,0x9957,0x9A5A,0x4EF0,0x51DD,0x5C2D,0x6681,0x696D,0x5C40,0x66F2,0x6975,0x7389,0x6850,0x7C81,0x50C5,0x52E4,
    0x5747,0x5DFE,0x9326,0x65A4,0x6B23,0x6B3D,0x7434,0x7981,0x79BD,0x7B4B,0x7DCA,0x82B9,0x83CC,0x887F,0x895F,0x8B39,
    0x8FD1,0x91D1,0x541F,0x9280,0x4E5D,0x5036,0x53E5,0x533A,0x72D7,0x7396,0x77E9,0x82E6,0x8EAF,0x99C6,0x99C8,0x99D2,
    0x5177,0x611A,0x865E,0x55B0,0x7A7A,0x5076,0x5BD3,0x9047,0x9685,0x4E32,0x6ADB,0x91E7,0x5C51,0x5C48},
    /*23 area from 3721*/
    {0x6398,0x7A9F,0x6C93,0x9774,0x8F61,0x7AAA,0x718A,0x9688,0x7C82,0x6817,0x7E70,0x6851,0x936C,0x52F2,0x541B,0x85AB,
    0x8A13,0x7FA4,0x8ECD,0x90E1,0x5366,0x8888,0x7941,0x4FC2,0x50BE,0x5211,0x5144,0x5553,0x572D,0x73EA,0x578B,0x5951,
    0x5F62,0x5F84,0x6075,0x6176,0x6167,0x61A9,0x63B2,0x643A,0x656C,0x666F,0x6842,0x6E13,0x7566,0x7A3D,0x7CFB,0x7D4C,
    0x7D99,0x7E4B,0x7F6B,0x830E,0x834A,0x86CD,0x8A08,0x8A63,0x8B66,0x8EFD,0x981A,0x9D8F,0x82B8,0x8FCE,0x9BE8,0x5287,
    0x621F,0x6483,0x6FC0,0x9699,0x6841,0x5091,0x6B20,0x6C7A,0x6F54,0x7A74,0x7D50,0x8840,0x8A23,0x6708,0x4EF6,0x5039,
    0x5026,0x5065,0x517C,0x5238,0x5263,0x55A7,0x570F,0x5805,0x5ACC,0x5EFA,0x61B2,0x61F8,0x62F3,0x6372},
    /*24 area from 3821*/
    {0x691C,0x6A29,0x727D,0x72AC,0x732E,0x7814,0x786F,0x7D79,0x770C,0x80A9,0x898B,0x8B19,0x8CE2,0x8ED2,0x9063,0x9375,
    0x967A,0x9855,0x9A13,0x9E78,0x5143,0x539F,0x53B3,0x5E7B,0x5F26,0x6E1B,0x6E90,0x7384,0x73FE,0x7D43,0x8237,0x8A00,
    0x8AFA,0x9650,0x4E4E,0x500B,0x53E4,0x547C,0x56FA,0x59D1,0x5B64,0x5DF1,0x5EAB,0x5F27,0x6238,0x6545,0x67AF,0x6E56,
    0x72D0,0x7CCA,0x88B4,0x80A1,0x80E1,0x83F0,0x864E,0x8A87,0x8DE8,0x9237,0x96C7,0x9867,0x9F13,0x4E94,0x4E92,0x4F0D,
    0x5348,0x5449,0x543E,0x5A2F,0x5F8C,0x5FA1,0x609F,0x68A7,0x6A8E,0x745A,0x7881,0x8A9E,0x8AA4,0x8B77,0x9190,0x4E5E,
    0x9BC9,0x4EA4,0x4F7C,0x4FAF,0x5019,0x5016,0x5149,0x516C,0x529F,0x52B9,0x52FE,0x539A,0x53E3,0x5411},
    /*25 area from 3921*/
    {0x540E,0x5589,0x5751,0x57A2,0x597D,0x5B54,0x5B5D,0x5B8F,0x5DE5,0x5DE7,0x5DF7,0x5E78,0x5E83,0x5E9A,0x5EB7,0x5F18,
    0x6052,0x614C,0x6297,0x62D8,0x63A7,0x653B,0x6602,0x6643,0x66F4,0x676D,0x6821,0x6897,0x69CB,0x6C5F,0x6D2A,0x6D69,
    0x6E2F,0x6E9D,0x7532,0x7687,0x786C,0x7A3F,0x7CE0,0x7D05,0x7D18,0x7D5E,0x7DB1,0x8015,0x8003,0x80AF,0x80B1,0x8154,
    0x818F,0x822A,0x8352,0x884C,0x8861,0x8B1B,0x8CA2,0x8CFC,0x90CA,0x9175,0x9271,0x783F,0x92FC,0x95A4,0x964D,0x9805,
    0x9999,0x9AD8,0x9D3B,0x525B,0x52AB,0x53F7,0x5408,0x58D5,0x62F7,0x6FE0,0x8C6A,0x8F5F,0x9EB9,0x514B,0x523B,0x544A,
    0x56FD,0x7A40,0x9177,0x9D60,0x9ED2,0x7344,0x6F09,0x8170,0x7511,0x5FFD,0x60DA,0x9AA8,0x72DB,0x8FBC},
    /*26 area from 3a21*/
    {0x6B64,0x9803,0x4ECA,0x56F0,0x5764,0x58BE,0x5A5A,0x6068,0x61C7,0x660F,0x6606,0x6839,0x68B1,0x6DF7,0x75D5,0x7D3A,
    0x826E,0x9B42,0x4E9B,0x4F50,0x53C9,0x5506,0x5D6F,0x5DE6,0x5DEE,0x67FB,0x6C99,0x7473,0x7802,0x8A50,0x9396,0x88DF,
    0x5750,0x5EA7,0x632B,0x50B5,0x50AC,0x518D,0x6700,0x54C9,0x585E,0x59BB,0x5BB0,0x5F69,0x624D,0x63A1,0x683D,0x6B73,
    0x6E08,0x707D,0x91C7,0x7280,0x7815,0x7826,0x796D,0x658E,0x7D30,0x83DC,0x88C1,0x8F09,0x969B,0x5264,0x5728,0x6750,
    0x7F6A,0x8CA1,0x51B4,0x5742,0x962A,0x583A,0x698A,0x80B4,0x54B2,0x5D0E,0x57FC,0x7895,0x9DFA,0x4F5C,0x524A,0x548B,
    0x643E,0x6628,0x6714,0x67F5,0x7A84,0x7B56,0x7D22,0x932F,0x685C,0x9BAD,0x7B39,0x5319,0x518A,0x5237},
    /*27 area from 3b21*/
    {0x5BDF,0x62F6,0x64AE,0x64E6,0x672D,0x6BBA,0x85A9,0x96D1,0x7690,0x9BD6,0x634C,0x9306,0x9BAB,0x76BF,0x6652,0x4E09,
    0x5098,0x53C2,0x5C71,0x60E8,0x6492,0x6563,0x685F,0x71E6,0x73CA,0x7523,0x7B97,0x7E82,0x8695,0x8B83,0x8CDB,0x9178,
    0x9910,0x65AC,0x66AB,0x6B8B,0x4ED5,0x4ED4,0x4F3A,0x4F7F,0x523A,0x53F8,0x53F2,0x55E3,0x56DB,0x58EB,0x59CB,0x59C9,
    0x59FF,0x5B50,0x5C4D,0x5E02,0x5E2B,0x5FD7,0x601D,0x6307,0x652F,0x5B5C,0x65AF,0x65BD,0x65E8,0x679D,0x6B62,0x6B7B,
    0x6C0F,0x7345,0x7949,0x79C1,0x7CF8,0x7D19,0x7D2B,0x80A2,0x8102,0x81F3,0x8996,0x8A5E,0x8A69,0x8A66,0x8A8C,0x8AEE,
    0x8CC7,0x8CDC,0x96CC,0x98FC,0x6B6F,0x4E8B,0x4F3C,0x4F8D,0x5150,0x5B57,0x5BFA,0x6148,0x6301,0x6642},
    /*28 area from 3c21*/
    {0x6B21,0x6ECB,0x6CBB,0x723E,0x74BD,0x75D4,0x78C1,0x793A,0x800C,0x8033,0x81EA,0x8494,0x8F9E,0x6C50,0x9E7F,0x5F0F,
    0x8B58,0x9D2B,0x7AFA,0x8EF8,0x5B8D,0x96EB,0x4E03,0x53F1,0x57F7,0x5931,0x5AC9,0x5BA4,0x6089,0x6E7F,0x6F06,0x75BE,
    0x8CEA,0x5B9F,0x8500,0x7BE0,0x5072,0x67F4,0x829D,0x5C61,0x854A,0x7E1E,0x820E,0x5199,0x5C04,0x6368,0x8D66,0x659C,
    0x716E,0x793E,0x7D17,0x8005,0x8B1D,0x8ECA,0x906E,0x86C7,0x90AA,0x501F,0x52FA,0x5C3A,0x6753,0x707C,0x7235,0x914C,
    0x91C8,0x932B,0x82E5,0x5BC2,0x5F31,0x60F9,0x4E3B,0x53D6,0x5B88,0x624B,0x6731,0x6B8A,0x72E9,0x73E0,0x7A2E,0x816B,
    0x8DA3,0x9152,0x9996,0x5112,0x53D7,0x546A,0x5BFF,0x6388,0x6A39,0x7DAC,0x9700,0x56DA,0x53CE,0x5468},
    /*29 area from 3d21*/
    {0x5B97,0x5C31,0x5DDE,0x4FEE,0x6101,0x62FE,0x6D32,0x79C0,0x79CB,0x7D42,0x7E4D,0x7FD2,0x81ED,0x821F,0x8490,0x8846,
    0x8972,0x8B90,0x8E74,0x8F2F,0x9031,0x914B,0x916C,0x96C6,0x919C,0x4EC0,0x4F4F,0x5145,0x5341,0x5F93,0x620E,0x67D4,
    0x6C41,0x6E0B,0x7363,0x7E26,0x91CD,0x9283,0x53D4,0x5919,0x5BBF,0x6DD1,0x795D,0x7E2E,0x7C9B,0x587E,0x719F,0x51FA,
    0x8853,0x8FF0,0x4FCA,0x5CFB,0x6625,0x77AC,0x7AE3,0x821C,0x99FF,0x51C6,0x5FAA,0x65EC,0x696F,0x6B89,0x6DF3,0x6E96,
    0x6F64,0x76FE,0x7D14,0x5DE1,0x9075,0x9187,0x9806,0x51E6,0x521D,0x6240,0x6691,0x66D9,0x6E1A,0x5EB6,0x7DD2,0x7F72,
    0x66F8,0x85AF,0x85F7,0x8AF8,0x52A9,0x53D9,0x5973,0x5E8F,0x5F90,0x6055,0x92E4,0x9664,0x50B7,0x511F},
    /*30 area from 3e21*/
    {0x52DD,0x5320,0x5347,0x53EC,0x54E8,0x5546,0x5531,0x5617,0x5968,0x59BE,0x5A3C,0x5BB5,0x5C06,0x5C0F,0x5C11,0x5C1A,
    0x5E84,0x5E8A,0x5EE0,0x5F70,0x627F,0x6284,0x62DB,0x638C,0x6377,0x6607,0x660C,0x662D,0x6676,0x677E,0x68A2,0x6A1F,
    0x6A35,0x6CBC,0x6D88,0x6E09,0x6E58,0x713C,0x7126,0x7167,0x75C7,0x7701,0x785D,0x7901,0x7965,0x79F0,0x7AE0,0x7B11,
    0x7CA7,0x7D39,0x8096,0x83D6,0x848B,0x8549,0x885D,0x88F3,0x8A1F,0x8A3C,0x8A54,0x8A73,0x8C61,0x8CDE,0x91A4,0x9266,
    0x937E,0x9418,0x969C,0x9798,0x4E0A,0x4E08,0x4E1E,0x4E57,0x5197,0x5270,0x57CE,0x5834,0x58CC,0x5B22,0x5E38,0x60C5,
    0x64FE,0x6761,0x6756,0x6D44,0x72B6,0x7573,0x7A63,0x84B8,0x8B72,0x91B8,0x9320,0x5631,0x57F4,0x98FE},
    /*31 area from 3f21*/
    {0x62ED,0x690D,0x6B96,0x71ED,0x7E54,0x8077,0x8272,0x89E6,0x98DF,0x8755,0x8FB1,0x5C3B,0x4F38,0x4FE1,0x4FB5,0x5507,
    0x5A20,0x5BDD,0x5BE9,0x5FC3,0x614E,0x632F,0x65B0,0x664B,0x68EE,0x699B,0x6D78,0x6DF1,0x7533,0x75B9,0x771F,0x795E,
    0x79E6,0x7D33,0x81E3,0x82AF,0x85AA,0x89AA,0x8A3A,0x8EAB,0x8F9B,0x9032,0x91DD,0x9707,0x4EBA,0x4EC1,0x5203,0x5875,
    0x58EC,0x5C0B,0x751A,0x5C3D,0x814E,0x8A0A,0x8FC5,0x9663,0x976D,0x7B25,0x8ACF,0x9808,0x9162,0x56F3,0x53A8,0x9017,
    0x5439,0x5782,0x5E25,0x63A8,0x6C34,0x708A,0x7761,0x7C8B,0x7FE0,0x8870,0x9042,0x9154,0x9310,0x9318,0x968F,0x745E,
    0x9AC4,0x5D07,0x5D69,0x6570,0x67A2,0x8DA8,0x96DB,0x636E,0x6749,0x6919,0x83C5,0x9817,0x96C0,0x88FE},
    /*32 area from 4021*/
    {0x6F84,0x647A,0x5BF8,0x4E16,0x702C,0x755D,0x662F,0x51C4,0x5236,0x52E2,0x59D3,0x5F81,0x6027,0x6210,0x653F,0x6574,
    0x661F,0x6674,0x68F2,0x6816,0x6B63,0x6E05,0x7272,0x751F,0x76DB,0x7CBE,0x8056,0x58F0,0x88FD,0x897F,0x8AA0,0x8A93,
    0x8ACB,0x901D,0x9192,0x9752,0x9759,0x6589,0x7A0E,0x8106,0x96BB,0x5E2D,0x60DC,0x621A,0x65A5,0x6614,0x6790,0x77F3,
    0x7A4D,0x7C4D,0x7E3E,0x810A,0x8CAC,0x8D64,0x8DE1,0x8E5F,0x78A9,0x5207,0x62D9,0x63A5,0x6442,0x6298,0x8A2D,0x7A83,
    0x7BC0,0x8AAC,0x96EA,0x7D76,0x820C,0x8749,0x4ED9,0x5148,0x5343,0x5360,0x5BA3,0x5C02,0x5C16,0x5DDD,0x6226,0x6247,
    0x64B0,0x6813,0x6834,0x6CC9,0x6D45,0x6D17,0x67D3,0x6F5C,0x714E,0x717D,0x65CB,0x7A7F,0x7BAD,0x7DDA},
    /*33 area from 4121*/
    {0x7E4A,0x7FA8,0x817A,0x821B,0x8239,0x85A6,0x8A6E,0x8CCE,0x8DF5,0x9078,0x9077,0x92AD,0x9291,0x9583,0x9BAE,0x524D,
    0x5584,0x6F38,0x7136,0x5168,0x7985,0x7E55,0x81B3,0x7CCE,0x564C,0x5851,0x5CA8,0x63AA,0x66FE,0x66FD,0x695A,0x72D9,
    0x758F,0x758E,0x790E,0x7956,0x79DF,0x7C97,0x7D20,0x7D44,0x8607,0x8A34,0x963B,0x9061,0x9F20,0x50E7,0x5275,0x53CC,
    0x53E2,0x5009,0x55AA,0x58EE,0x594F,0x723D,0x5B8B,0x5C64,0x531D,0x60E3,0x60F3,0x635C,0x6383,0x633F,0x63BB,0x64CD,
    0x65E9,0x66F9,0x5DE3,0x69CD,0x69FD,0x6F15,0x71E5,0x4E89,0x75E9,0x76F8,0x7A93,0x7CDF,0x7DCF,0x7D9C,0x8061,0x8349,
    0x8358,0x846C,0x84BC,0x85FB,0x88C5,0x8D70,0x9001,0x906D,0x9397,0x971C,0x9A12,0x50CF,0x5897,0x618E},
    /*34 area from 4221*/
    {0x81D3,0x8535,0x8D08,0x9020,0x4FC3,0x5074,0x5247,0x5373,0x606F,0x6349,0x675F,0x6E2C,0x8DB3,0x901F,0x4FD7,0x5C5E,
    0x8CCA,0x65CF,0x7D9A,0x5352,0x8896,0x5176,0x63C3,0x5B58,0x5B6B,0x5C0A,0x640D,0x6751,0x905C,0x4ED6,0x591A,0x592A,
    0x6C70,0x8A51,0x553E,0x5815,0x59A5,0x60F0,0x6253,0x67C1,0x8235,0x6955,0x9640,0x99C4,0x9A28,0x4F53,0x5806,0x5BFE,
    0x8010,0x5CB1,0x5E2F,0x5F85,0x6020,0x614B,0x6234,0x66FF,0x6CF0,0x6EDE,0x80CE,0x817F,0x82D4,0x888B,0x8CB8,0x9000,
    0x902E,0x968A,0x9EDB,0x9BDB,0x4EE3,0x53F0,0x5927,0x7B2C,0x918D,0x984C,0x9DF9,0x6EDD,0x7027,0x5353,0x5544,0x5B85,
    0x6258,0x629E,0x62D3,0x6CA2,0x6FEF,0x7422,0x8A17,0x9438,0x6FC1,0x8AFE,0x8338,0x51E7,0x86F8,0x53EA},
    /*35 area from 4321*/
    {0x53E9,0x4F46,0x9054,0x8FB0,0x596A,0x8131,0x5DFD,0x7AEA,0x8FBF,0x68DA,0x8C37,0x72F8,0x9C48,0x6A3D,0x8AB0,0x4E39,
    0x5358,0x5606,0x5766,0x62C5,0x63A2,0x65E6,0x6B4E,0x6DE1,0x6E5B,0x70AD,0x77ED,0x7AEF,0x7BAA,0x7DBB,0x803D,0x80C6,
    0x86CB,0x8A95,0x935B,0x56E3,0x58C7,0x5F3E,0x65AD,0x6696,0x6A80,0x6BB5,0x7537,0x8AC7,0x5024,0x77E5,0x5730,0x5F1B,
    0x6065,0x667A,0x6C60,0x75F4,0x7A1A,0x7F6E,0x81F4,0x8718,0x9045,0x99B3,0x7BC9,0x755C,0x7AF9,0x7B51,0x84C4,0x9010,
    0x79E9,0x7A92,0x8336,0x5AE1,0x7740,0x4E2D,0x4EF2,0x5B99,0x5FE0,0x62BD,0x663C,0x67F1,0x6CE8,0x866B,0x8877,0x8A3B,
    0x914E,0x92F3,0x99D0,0x6A17,0x7026,0x732A,0x82E7,0x8457,0x8CAF,0x4E01,0x5146,0x51CB,0x558B,0x5BF5},
    /*36 area from 4421*/
    {0x5E16,0x5E33,0x5E81,0x5F14,0x5F35,0x5F6B,0x5FB4,0x61F2,0x6311,0x66A2,0x671D,0x6F6E,0x7252,0x753A,0x773A,0x8074,
    0x8139,0x8178,0x8776,0x8ABF,0x8ADC,0x8D85,0x8DF3,0x929A,0x9577,0x9802,0x9CE5,0x52C5,0x6357,0x76F4,0x6715,0x6C88,
    0x73CD,0x8CC3,0x93AE,0x9673,0x6D25,0x589C,0x690E,0x69CC,0x8FFD,0x939A,0x75DB,0x901A,0x585A,0x6802,0x63B4,0x69FB,
    0x4F43,0x6F2C,0x67D8,0x8FBB,0x8526,0x7DB4,0x9354,0x693F,0x6F70,0x576A,0x58F7,0x5B2C,0x7D2C,0x722A,0x540A,0x91E3,
    0x9DB4,0x4EAD,0x4F4E,0x505C,0x5075,0x5243,0x8C9E,0x5448,0x5824,0x5B9A,0x5E1D,0x5E95,0x5EAD,0x5EF7,0x5F1F,0x608C,
    0x62B5,0x633A,0x63D0,0x68AF,0x6C40,0x7887,0x798E,0x7A0B,0x7DE0,0x8247,0x8A02,0x8AE6,0x8E44,0x9013},
    /*37 area from 4521*/
    {0x90B8,0x912D,0x91D8,0x9F0E,0x6CE5,0x6458,0x64E2,0x6575,0x6EF4,0x7684,0x7B1B,0x9069,0x93D1,0x6EBA,0x54F2,0x5FB9,
    0x64A4,0x8F4D,0x8FED,0x9244,0x5178,0x586B,0x5929,0x5C55,0x5E97,0x6DFB,0x7E8F,0x751C,0x8CBC,0x8EE2,0x985B,0x70B9,
    0x4F1D,0x6BBF,0x6FB1,0x7530,0x96FB,0x514E,0x5410,0x5835,0x5857,0x59AC,0x5C60,0x5F92,0x6597,0x675C,0x6E21,0x767B,
    0x83DF,0x8CED,0x9014,0x90FD,0x934D,0x7825,0x783A,0x52AA,0x5EA6,0x571F,0x5974,0x6012,0x5012,0x515A,0x51AC,0x51CD,
    0x5200,0x5510,0x5854,0x5858,0x5957,0x5B95,0x5CF6,0x5D8B,0x60BC,0x6295,0x642D,0x6771,0x6843,0x68BC,0x68DF,0x76D7,
    0x6DD8,0x6E6F,0x6D9B,0x706F,0x71C8,0x5F53,0x75D8,0x7977,0x7B49,0x7B54,0x7B52,0x7CD6,0x7D71,0x5230},
    /*38 area from 4621*/
    {0x8463,0x8569,0x85E4,0x8A0E,0x8B04,0x8C46,0x8E0F,0x9003,0x900F,0x9419,0x9676,0x982D,0x9A30,0x95D8,0x50CD,0x52D5,
    0x540C,0x5802,0x5C0E,0x61A7,0x649E,0x6D1E,0x77B3,0x7AE5,0x80F4,0x8404,0x9053,0x9285,0x5CE0,0x9D07,0x533F,0x5F97,
    0x5FB3,0x6D9C,0x7279,0x7763,0x79BF,0x7BE4,0x6BD2,0x72EC,0x8AAD,0x6803,0x6A61,0x51F8,0x7A81,0x6934,0x5C4A,0x9CF6,
    0x82EB,0x5BC5,0x9149,0x701E,0x5678,0x5C6F,0x60C7,0x6566,0x6C8C,0x8C5A,0x9041,0x9813,0x5451,0x66C7,0x920D,0x5948,
    0x90A3,0x5185,0x4E4D,0x51EA,0x8599,0x8B0E,0x7058,0x637A,0x934B,0x6962,0x99B4,0x7E04,0x7577,0x5357,0x6960,0x8EDF,
    0x96E3,0x6C5D,0x4E8C,0x5C3C,0x5F10,0x8FE9,0x5302,0x8CD1,0x8089,0x8679,0x5EFF,0x65E5,0x4E73,0x5165},
    /*39 area from 4721*/
    {0x5982,0x5C3F,0x97EE,0x4EFB,0x598A,0x5FCD,0x8A8D,0x6FE1,0x79B0,0x7962,0x5BE7,0x8471,0x732B,0x71B1,0x5E74,0x5FF5,
    0x637B,0x649A,0x71C3,0x7C98,0x4E43,0x5EFC,0x4E4B,0x57DC,0x56A2,0x60A9,0x6FC3,0x7D0D,0x80FD,0x8133,0x81BF,0x8FB2,
    0x8997,0x86A4,0x5DF4,0x628A,0x64AD,0x8987,0x6777,0x6CE2,0x6D3E,0x7436,0x7834,0x5A46,0x7F75,0x82AD,0x99AC,0x4FF3,
    0x5EC3,0x62DD,0x6392,0x6557,0x676F,0x76C3,0x724C,0x80CC,0x80BA,0x8F29,0x914D,0x500D,0x57F9,0x5A92,0x6885,0x6973,
    0x7164,0x72FD,0x8CB7,0x58F2,0x8CE0,0x966A,0x9019,0x877F,0x79E4,0x77E7,0x8429,0x4F2F,0x5265,0x535A,0x62CD,0x67CF,
    0x6CCA,0x767D,0x7B94,0x7C95,0x8236,0x8584,0x8FEB,0x66DD,0x6F20,0x7206,0x7E1B,0x83AB,0x99C1,0x9EA6},
    /*40 area from 4821*/
    {0x51FD,0x7BB1,0x7872,0x7BB8,0x8087,0x7B48,0x6AE8,0x5E61,0x808C,0x7551,0x7560,0x516B,0x9262,0x6E8C,0x767A,0x9197,
    0x9AEA,0x4F10,0x7F70,0x629C,0x7B4F,0x95A5,0x9CE9,0x567A,0x5859,0x86E4,0x96BC,0x4F34,0x5224,0x534A,0x53CD,0x53DB,
    0x5E06,0x642C,0x6591,0x677F,0x6C3E,0x6C4E,0x7248,0x72AF,0x73ED,0x7554,0x7E41,0x822C,0x85E9,0x8CA9,0x7BC4,0x91C6,
    0x7169,0x9812,0x98EF,0x633D,0x6669,0x756A,0x76E4,0x78D0,0x8543,0x86EE,0x532A,0x5351,0x5426,0x5983,0x5E87,0x5F7C,
    0x60B2,0x6249,0x6279,0x62AB,0x6590,0x6BD4,0x6CCC,0x75B2,0x76AE,0x7891,0x79D8,0x7DCB,0x7F77,0x80A5,0x88AB,0x8AB9,
    0x8CBB,0x907F,0x975E,0x98DB,0x6A0B,0x7C38,0x5099,0x5C3E,0x5FAE,0x6787,0x6BD8,0x7435,0x7709,0x7F8E},
    /*41 area from 4921*/
    {0x9F3B,0x67CA,0x7A17,0x5339,0x758B,0x9AED,0x5F66,0x819D,0x83F1,0x8098,0x5F3C,0x5FC5,0x7562,0x7B46,0x903C,0x6867,
    0x59EB,0x5A9B,0x7D10,0x767E,0x8B2C,0x4FF5,0x5F6A,0x6A19,0x6C37,0x6F02,0x74E2,0x7968,0x8868,0x8A55,0x8C79,0x5EDF,
    0x63CF,0x75C5,0x79D2,0x82D7,0x9328,0x92F2,0x849C,0x86ED,0x9C2D,0x54C1,0x5F6C,0x658C,0x6D5C,0x7015,0x8CA7,0x8CD3,
    0x983B,0x654F,0x74F6,0x4E0D,0x4ED8,0x57E0,0x592B,0x5A66,0x5BCC,0x51A8,0x5E03,0x5E9C,0x6016,0x6276,0x6577,0x65A7,
    0x666E,0x6D6E,0x7236,0x7B26,0x8150,0x819A,0x8299,0x8B5C,0x8CA0,0x8CE6,0x8D74,0x961C,0x9644,0x4FAE,0x64AB,0x6B66,
    0x821E,0x8461,0x856A,0x90E8,0x5C01,0x6953,0x98A8,0x847A,0x8557,0x4F0F,0x526F,0x5FA9,0x5E45,0x670D},
    /*42 area from 4a21*/
    {0x798F,0x8179,0x8907,0x8986,0x6DF5,0x5F17,0x6255,0x6CB8,0x4ECF,0x7269,0x9B92,0x5206,0x543B,0x5674,0x58B3,0x61A4,
    0x626E,0x711A,0x596E,0x7C89,0x7CDE,0x7D1B,0x96F0,0x6587,0x805E,0x4E19,0x4F75,0x5175,0x5840,0x5E63,0x5E73,0x5F0A,
    0x67C4,0x4E26,0x853D,0x9589,0x965B,0x7C73,0x9801,0x50FB,0x58C1,0x7656,0x78A7,0x5225,0x77A5,0x8511,0x7B86,0x504F,
    0x5909,0x7247,0x7BC7,0x7DE8,0x8FBA,0x8FD4,0x904D,0x4FBF,0x52C9,0x5A29,0x5F01,0x97AD,0x4FDD,0x8217,0x92EA,0x5703,
    0x6355,0x6B69,0x752B,0x88DC,0x8F14,0x7A42,0x52DF,0x5893,0x6155,0x620A,0x66AE,0x6BCD,0x7C3F,0x83E9,0x5023,0x4FF8,
    0x5305,0x5446,0x5831,0x5949,0x5B9D,0x5CF0,0x5CEF,0x5D29,0x5E96,0x62B1,0x6367,0x653E,0x65B9,0x670B},
    /*43 area from 4b21*/
    {0x6CD5,0x6CE1,0x70F9,0x7832,0x7E2B,0x80DE,0x82B3,0x840C,0x84EC,0x8702,0x8912,0x8A2A,0x8C4A,0x90A6,0x92D2,0x98FD,
    0x9CF3,0x9D6C,0x4E4F,0x4EA1,0x508D,0x5256,0x574A,0x59A8,0x5E3D,0x5FD8,0x5FD9,0x623F,0x66B4,0x671B,0x67D0,0x68D2,
    0x5192,0x7D21,0x80AA,0x81A8,0x8B00,0x8C8C,0x8CBF,0x927E,0x9632,0x5420,0x982C,0x5317,0x50D5,0x535C,0x58A8,0x64B2,
    0x6734,0x7267,0x7766,0x7A46,0x91E6,0x52C3,0x6CA1,0x6B86,0x5800,0x5E4C,0x5954,0x672C,0x7FFB,0x51E1,0x76C6,0x6469,
    0x78E8,0x9B54,0x9EBB,0x57CB,0x59B9,0x6627,0x679A,0x6BCE,0x54E9,0x69D9,0x5E55,0x819C,0x6795,0x9BAA,0x67FE,0x9C52,
    0x685D,0x4EA6,0x4FE3,0x53C8,0x62B9,0x672B,0x6CAB,0x8FC4,0x4FAD,0x7E6D,0x9EBF,0x4E07,0x6162,0x6E80},
    /*44 area from 4c21*/
    {0x6F2B,0x8513,0x5473,0x672A,0x9B45,0x5DF3,0x7B95,0x5CAC,0x5BC6,0x871C,0x6E4A,0x84D1,0x7A14,0x8108,0x5999,0x7C8D,
    0x6C11,0x7720,0x52D9,0x5922,0x7121,0x725F,0x77DB,0x9727,0x9D61,0x690B,0x5A7F,0x5A18,0x51A5,0x540D,0x547D,0x660E,
    0x76DF,0x8FF7,0x9298,0x9CF4,0x59EA,0x725D,0x6EC5,0x514D,0x68C9,0x7DBF,0x7DEC,0x9762,0x9EBA,0x6478,0x6A21,0x8302,
    0x5984,0x5B5F,0x6BDB,0x731B,0x76F2,0x7DB2,0x8017,0x8499,0x5132,0x6728,0x9ED9,0x76EE,0x6762,0x52FF,0x9905,0x5C24,
    0x623B,0x7C7E,0x8CB0,0x554F,0x60B6,0x7D0B,0x9580,0x5301,0x4E5F,0x51B6,0x591C,0x723A,0x8036,0x91CE,0x5F25,0x77E2,
    0x5384,0x5F79,0x7D04,0x85AC,0x8A33,0x8E8D,0x9756,0x67F3,0x85AE,0x9453,0x6109,0x6108,0x6CB9,0x7652},
    /*45 area from 4d21*/
    {0x8AED,0x8F38,0x552F,0x4F51,0x512A,0x52C7,0x53CB,0x5BA5,0x5E7D,0x60A0,0x6182,0x63D6,0x6709,0x67DA,0x6E67,0x6D8C,
    0x7336,0x7337,0x7531,0x7950,0x88D5,0x8A98,0x904A,0x9091,0x90F5,0x96C4,0x878D,0x5915,0x4E88,0x4F59,0x4E0E,0x8A89,
    0x8F3F,0x9810,0x50AD,0x5E7C,0x5996,0x5BB9,0x5EB8,0x63DA,0x63FA,0x64C1,0x66DC,0x694A,0x69D8,0x6D0B,0x6EB6,0x7194,
    0x7528,0x7AAF,0x7F8A,0x8000,0x8449,0x84C9,0x8981,0x8B21,0x8E0A,0x9065,0x967D,0x990A,0x617E,0x6291,0x6B32,0x6C83,
    0x6D74,0x7FCC,0x7FFC,0x6DC0,0x7F85,0x87BA,0x88F8,0x6765,0x83B1,0x983C,0x96F7,0x6D1B,0x7D61,0x843D,0x916A,0x4E71,
    0x5375,0x5D50,0x6B04,0x6FEB,0x85CD,0x862D,0x89A7,0x5229,0x540F,0x5C65,0x674E,0x68A8,0x7406,0x7483},
    /*46 area from 4e21*/
    {0x75E2,0x88CF,0x88E1,0x91CC,0x96E2,0x9678,0x5F8B,0x7387,0x7ACB,0x844E,0x63A0,0x7565,0x5289,0x6D41,0x6E9C,0x7409,
    0x7559,0x786B,0x7C92,0x9686,0x7ADC,0x9F8D,0x4FB6,0x616E,0x65C5,0x865C,0x4E86,0x4EAE,0x50DA,0x4E21,0x51CC,0x5BEE,
    0x6599,0x6881,0x6DBC,0x731F,0x7642,0x77AD,0x7A1C,0x7CE7,0x826F,0x8AD2,0x907C,0x91CF,0x9675,0x9818,0x529B,0x7DD1,
    0x502B,0x5398,0x6797,0x6DCB,0x71D0,0x7433,0x81E8,0x8F2A,0x96A3,0x9C57,0x9E9F,0x7460,0x5841,0x6D99,0x7D2F,0x985E,
    0x4EE4,0x4F36,0x4F8B,0x51B7,0x52B1,0x5DBA,0x601C,0x73B2,0x793C,0x82D3,0x9234,0x96B7,0x96F6,0x970A,0x9E97,0x9F62,
    0x66A6,0x6B74,0x5217,0x52A3,0x70C8,0x88C2,0x5EC9,0x604B,0x6190,0x6F23,0x7149,0x7C3E,0x7DF4,0x806F},
    /*47 area from 4f21*/
    {0x84EE,0x9023,0x932C,0x5442,0x9B6F,0x6AD3,0x7089,0x8CC2,0x8DEF,0x9732,0x52B4,0x5A41,0x5ECA,0x5F04,0x6717,0x697C,
    0x6994,0x6D6A,0x6F0F,0x7262,0x72FC,0x7BED,0x8001,0x807E,0x874B,0x90CE,0x516D,0x9E93,0x7984,0x808B,0x9332,0x8AD6,
    0x502D,0x548C,0x8A71,0x6B6A,0x8CC4,0x8107,0x60D1,0x67A0,0x9DF2,0x4E99,0x4E98,0x9C10,0x8A6B,0x85C1,0x8568,0x6900,
    0x6E7E,0x7897,0x8155,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},

    /*48 area from 5021*/
    {0x5F0C,0x4E10,0x4E15,0x4E2A,0x4E31,0x4E36,0x4E3C,0x4E3F,0x4E42,0x4E56,0x4E58,0x4E82,0x4E85,0x8C6B,0x4E8A,0x8212,
    0x5F0D,0x4E8E,0x4E9E,0x4E9F,0x4EA0,0x4EA2,0x4EB0,0x4EB3,0x4EB6,0x4ECE,0x4ECD,0x4EC4,0x4EC6,0x4EC2,0x4ED7,0x4EDE,
    0x4EED,0x4EDF,0x4EF7,0x4F09,0x4F5A,0x4F30,0x4F5B,0x4F5D,0x4F57,0x4F47,0x4F76,0x4F88,0x4F8F,0x4F98,0x4F7B,0x4F69,
    0x4F70,0x4F91,0x4F6F,0x4F86,0x4F96,0x5118,0x4FD4,0x4FDF,0x4FCE,0x4FD8,0x4FDB,0x4FD1,0x4FDA,0x4FD0,0x4FE4,0x4FE5,
    0x501A,0x5028,0x5014,0x502A,0x5025,0x5005,0x4F1C,0x4FF6,0x5021,0x5029,0x502C,0x4FFE,0x4FEF,0x5011,0x5006,0x5043,
    0x5047,0x6703,0x5055,0x5050,0x5048,0x505A,0x5056,0x506C,0x5078,0x5080,0x509A,0x5085,0x50B4,0x50B2},
    /*49 area from 5121*/
    {0x50C9,0x50CA,0x50B3,0x50C2,0x50D6,0x50DE,0x50E5,0x50ED,0x50E3,0x50EE,0x50F9,0x50F5,0x5109,0x5101,0x5102,0x5116,
    0x5115,0x5114,0x511A,0x5121,0x513A,0x5137,0x513C,0x513B,0x513F,0x5140,0x5152,0x514C,0x5154,0x5162,0x7AF8,0x5169,
    0x516A,0x516E,0x5180,0x5182,0x56D8,0x518C,0x5189,0x518F,0x5191,0x5193,0x5195,0x5196,0x51A4,0x51A6,0x51A2,0x51A9,
    0x51AA,0x51AB,0x51B3,0x51B1,0x51B2,0x51B0,0x51B5,0x51BD,0x51C5,0x51C9,0x51DB,0x51E0,0x8655,0x51E9,0x51ED,0x51F0,
    0x51F5,0x51FE,0x5204,0x520B,0x5214,0x520E,0x5227,0x522A,0x522E,0x5233,0x5239,0x524F,0x5244,0x524B,0x524C,0x525E,
    0x5254,0x526A,0x5274,0x5269,0x5273,0x527F,0x527D,0x528D,0x5294,0x5292,0x5271,0x5288,0x5291,0x8FA8},
    /*50 area from 5221*/
    {0x8FA7,0x52AC,0x52AD,0x52BC,0x52B5,0x52C1,0x52CD,0x52D7,0x52DE,0x52E3,0x52E6,0x98ED,0x52E0,0x52F3,0x52F5,0x52F8,
    0x52F9,0x5306,0x5308,0x7538,0x530D,0x5310,0x530F,0x5315,0x531A,0x5323,0x532F,0x5331,0x5333,0x5338,0x5340,0x5346,
    0x5345,0x4E17,0x5349,0x534D,0x51D6,0x535E,0x5369,0x536E,0x5918,0x537B,0x5377,0x5382,0x5396,0x53A0,0x53A6,0x53A5,
    0x53AE,0x53B0,0x53B6,0x53C3,0x7C12,0x96D9,0x53DF,0x66FC,0x71EE,0x53EE,0x53E8,0x53ED,0x53FA,0x5401,0x543D,0x5440,
    0x542C,0x542D,0x543C,0x542E,0x5436,0x5429,0x541D,0x544E,0x548F,0x5475,0x548E,0x545F,0x5471,0x5477,0x5470,0x5492,
    0x547B,0x5480,0x5476,0x5484,0x5490,0x5486,0x54C7,0x54A2,0x54B8,0x54A5,0x54AC,0x54C4,0x54C8,0x54A8},
    /*51 area from 5321*/
    {0x54AB,0x54C2,0x54A4,0x54BE,0x54BC,0x54D8,0x54E5,0x54E6,0x550F,0x5514,0x54FD,0x54EE,0x54ED,0x54FA,0x54E2,0x5539,
    0x5540,0x5563,0x554C,0x552E,0x555C,0x5545,0x5556,0x5557,0x5538,0x5533,0x555D,0x5599,0x5580,0x54AF,0x558A,0x559F,
    0x557B,0x557E,0x5598,0x559E,0x55AE,0x557C,0x5583,0x55A9,0x5587,0x55A8,0x55DA,0x55C5,0x55DF,0x55C4,0x55DC,0x55E4,
    0x55D4,0x5614,0x55F7,0x5616,0x55FE,0x55FD,0x561B,0x55F9,0x564E,0x5650,0x71DF,0x5634,0x5636,0x5632,0x5638,0x566B,
    0x5664,0x562F,0x566C,0x566A,0x5686,0x5680,0x568A,0x56A0,0x5694,0x568F,0x56A5,0x56AE,0x56B6,0x56B4,0x56C2,0x56BC,
    0x56C1,0x56C3,0x56C0,0x56C8,0x56CE,0x56D1,0x56D3,0x56D7,0x56EE,0x56F9,0x5700,0x56FF,0x5704,0x5709},
    /*52 area from 5421*/
    {0x5708,0x570B,0x570D,0x5713,0x5718,0x5716,0x55C7,0x571C,0x5726,0x5737,0x5738,0x574E,0x573B,0x5740,0x574F,0x5769,
    0x57C0,0x5788,0x5761,0x577F,0x5789,0x5793,0x57A0,0x57B3,0x57A4,0x57AA,0x57B0,0x57C3,0x57C6,0x57D4,0x57D2,0x57D3,
    0x580A,0x57D6,0x57E3,0x580B,0x5819,0x581D,0x5872,0x5821,0x5862,0x584B,0x5870,0x6BC0,0x5852,0x583D,0x5879,0x5885,
    0x58B9,0x589F,0x58AB,0x58BA,0x58DE,0x58BB,0x58B8,0x58AE,0x58C5,0x58D3,0x58D1,0x58D7,0x58D9,0x58D8,0x58E5,0x58DC,
    0x58E4,0x58DF,0x58EF,0x58FA,0x58F9,0x58FB,0x58FC,0x58FD,0x5902,0x590A,0x5910,0x591B,0x68A6,0x5925,0x592C,0x592D,
    0x5932,0x5938,0x593E,0x7AD2,0x5955,0x5950,0x594E,0x595A,0x5958,0x5962,0x5960,0x5967,0x596C,0x5969},
    /*53 area from 5521*/
    {0x5978,0x5981,0x599D,0x4F5E,0x4FAB,0x59A3,0x59B2,0x59C6,0x59E8,0x59DC,0x598D,0x59D9,0x59DA,0x5A25,0x5A1F,0x5A11,
    0x5A1C,0x5A09,0x5A1A,0x5A40,0x5A6C,0x5A49,0x5A35,0x5A36,0x5A62,0x5A6A,0x5A9A,0x5ABC,0x5ABE,0x5ACB,0x5AC2,0x5ABD,
    0x5AE3,0x5AD7,0x5AE6,0x5AE9,0x5AD6,0x5AFA,0x5AFB,0x5B0C,0x5B0B,0x5B16,0x5B32,0x5AD0,0x5B2A,0x5B36,0x5B3E,0x5B43,
    0x5B45,0x5B40,0x5B51,0x5B55,0x5B5A,0x5B5B,0x5B65,0x5B69,0x5B70,0x5B73,0x5B75,0x5B78,0x6588,0x5B7A,0x5B80,0x5B83,
    0x5BA6,0x5BB8,0x5BC3,0x5BC7,0x5BC9,0x5BD4,0x5BD0,0x5BE4,0x5BE6,0x5BE2,0x5BDE,0x5BE5,0x5BEB,0x5BF0,0x5BF6,0x5BF3,
    0x5C05,0x5C07,0x5C08,0x5C0D,0x5C13,0x5C20,0x5C22,0x5C28,0x5C38,0x5C39,0x5C41,0x5C46,0x5C4E,0x5C53},
    /*54 area from 5621*/
    {0x5C50,0x5C4F,0x5B71,0x5C6C,0x5C6E,0x4E62,0x5C76,0x5C79,0x5C8C,0x5C91,0x5C94,0x599B,0x5CAB,0x5CBB,0x5CB6,0x5CBC,
    0x5CB7,0x5CC5,0x5CBE,0x5CC7,0x5CD9,0x5CE9,0x5CFD,0x5CFA,0x5CED,0x5D8C,0x5CEA,0x5D0B,0x5D15,0x5D17,0x5D5C,0x5D1F,
    0x5D1B,0x5D11,0x5D14,0x5D22,0x5D1A,0x5D19,0x5D18,0x5D4C,0x5D52,0x5D4E,0x5D4B,0x5D6C,0x5D73,0x5D76,0x5D87,0x5D84,
    0x5D82,0x5DA2,0x5D9D,0x5DAC,0x5DAE,0x5DBD,0x5D90,0x5DB7,0x5DBC,0x5DC9,0x5DCD,0x5DD3,0x5DD2,0x5DD6,0x5DDB,0x5DEB,
    0x5DF2,0x5DF5,0x5E0B,0x5E1A,0x5E19,0x5E11,0x5E1B,0x5E36,0x5E37,0x5E44,0x5E43,0x5E40,0x5E4E,0x5E57,0x5E54,0x5E5F,
    0x5E62,0x5E64,0x5E47,0x5E75,0x5E76,0x5E7A,0x9EBC,0x5E7F,0x5EA0,0x5EC1,0x5EC2,0x5EC8,0x5ED0,0x5ECF},
    /*55 area from 5721*/
    {0x5ED6,0x5EE3,0x5EDD,0x5EDA,0x5EDB,0x5EE2,0x5EE1,0x5EE8,0x5EE9,0x5EEC,0x5EF1,0x5EF3,0x5EF0,0x5EF4,0x5EF8,0x5EFE,
    0x5F03,0x5F09,0x5F5D,0x5F5C,0x5F0B,0x5F11,0x5F16,0x5F29,0x5F2D,0x5F38,0x5F41,0x5F48,0x5F4C,0x5F4E,0x5F2F,0x5F51,
    0x5F56,0x5F57,0x5F59,0x5F61,0x5F6D,0x5F73,0x5F77,0x5F83,0x5F82,0x5F7F,0x5F8A,0x5F88,0x5F91,0x5F87,0x5F9E,0x5F99,
    0x5F98,0x5FA0,0x5FA8,0x5FAD,0x5FBC,0x5FD6,0x5FFB,0x5FE4,0x5FF8,0x5FF1,0x5FDD,0x60B3,0x5FFF,0x6021,0x6060,0x6019,
    0x6010,0x6029,0x600E,0x6031,0x601B,0x6015,0x602B,0x6026,0x600F,0x603A,0x605A,0x6041,0x606A,0x6077,0x605F,0x604A,
    0x6046,0x604D,0x6063,0x6043,0x6064,0x6042,0x606C,0x606B,0x6059,0x6081,0x608D,0x60E7,0x6083,0x609A},
    /*56 area from 5821*/
    {0x6084,0x609B,0x6096,0x6097,0x6092,0x60A7,0x608B,0x60E1,0x60B8,0x60E0,0x60D3,0x60B4,0x5FF0,0x60BD,0x60C6,0x60B5,
    0x60D8,0x614D,0x6115,0x6106,0x60F6,0x60F7,0x6100,0x60F4,0x60FA,0x6103,0x6121,0x60FB,0x60F1,0x610D,0x610E,0x6147,
    0x613E,0x6128,0x6127,0x614A,0x613F,0x613C,0x612C,0x6134,0x613D,0x6142,0x6144,0x6173,0x6177,0x6158,0x6159,0x615A,
    0x616B,0x6174,0x616F,0x6165,0x6171,0x615F,0x615D,0x6153,0x6175,0x6199,0x6196,0x6187,0x61AC,0x6194,0x619A,0x618A,
    0x6191,0x61AB,0x61AE,0x61CC,0x61CA,0x61C9,0x61F7,0x61C8,0x61C3,0x61C6,0x61BA,0x61CB,0x7F79,0x61CD,0x61E6,0x61E3,
    0x61F6,0x61FA,0x61F4,0x61FF,0x61FD,0x61FC,0x61FE,0x6200,0x6208,0x6209,0x620D,0x620C,0x6214,0x621B},
    /*57 area from 5921*/
    {0x621E,0x6221,0x622A,0x622E,0x6230,0x6232,0x6233,0x6241,0x624E,0x625E,0x6263,0x625B,0x6260,0x6268,0x627C,0x6282,
    0x6289,0x627E,0x6292,0x6293,0x6296,0x62D4,0x6283,0x6294,0x62D7,0x62D1,0x62BB,0x62CF,0x62FF,0x62C6,0x64D4,0x62C8,
    0x62DC,0x62CC,0x62CA,0x62C2,0x62C7,0x629B,0x62C9,0x630C,0x62EE,0x62F1,0x6327,0x6302,0x6308,0x62EF,0x62F5,0x6350,
    0x633E,0x634D,0x641C,0x634F,0x6396,0x638E,0x6380,0x63AB,0x6376,0x63A3,0x638F,0x6389,0x639F,0x63B5,0x636B,0x6369,
    0x63BE,0x63E9,0x63C0,0x63C6,0x63E3,0x63C9,0x63D2,0x63F6,0x63C4,0x6416,0x6434,0x6406,0x6413,0x6426,0x6436,0x651D,
    0x6417,0x6428,0x640F,0x6467,0x646F,0x6476,0x644E,0x652A,0x6495,0x6493,0x64A5,0x64A9,0x6488,0x64BC},
    /*58 area from 5a21*/
    {0x64DA,0x64D2,0x64C5,0x64C7,0x64BB,0x64D8,0x64C2,0x64F1,0x64E7,0x8209,0x64E0,0x64E1,0x62AC,0x64E3,0x64EF,0x652C,
    0x64F6,0x64F4,0x64F2,0x64FA,0x6500,0x64FD,0x6518,0x651C,0x6505,0x6524,0x6523,0x652B,0x6534,0x6535,0x6537,0x6536,
    0x6538,0x754B,0x6548,0x6556,0x6555,0x654D,0x6558,0x655E,0x655D,0x6572,0x6578,0x6582,0x6583,0x8B8A,0x659B,0x659F,
    0x65AB,0x65B7,0x65C3,0x65C6,0x65C1,0x65C4,0x65CC,0x65D2,0x65DB,0x65D9,0x65E0,0x65E1,0x65F1,0x6772,0x660A,0x6603,
    0x65FB,0x6773,0x6635,0x6636,0x6634,0x661C,0x664F,0x6644,0x6649,0x6641,0x665E,0x665D,0x6664,0x6667,0x6668,0x665F,
    0x6662,0x6670,0x6683,0x6688,0x668E,0x6689,0x6684,0x6698,0x669D,0x66C1,0x66B9,0x66C9,0x66BE,0x66BC},
    /*59 area from 5b21*/
    {0x66C4,0x66B8,0x66D6,0x66DA,0x66E0,0x663F,0x66E6,0x66E9,0x66F0,0x66F5,0x66F7,0x670F,0x6716,0x671E,0x6726,0x6727,
    0x9738,0x672E,0x673F,0x6736,0x6741,0x6738,0x6737,0x6746,0x675E,0x6760,0x6759,0x6763,0x6764,0x6789,0x6770,0x67A9,
    0x677C,0x676A,0x678C,0x678B,0x67A6,0x67A1,0x6785,0x67B7,0x67EF,0x67B4,0x67EC,0x67B3,0x67E9,0x67B8,0x67E4,0x67DE,
    0x67DD,0x67E2,0x67EE,0x67B9,0x67CE,0x67C6,0x67E7,0x6A9C,0x681E,0x6846,0x6829,0x6840,0x684D,0x6832,0x684E,0x68B3,
    0x682B,0x6859,0x6863,0x6877,0x687F,0x689F,0x688F,0x68AD,0x6894,0x689D,0x689B,0x6883,0x6AAE,0x68B9,0x6874,0x68B5,
    0x68A0,0x68BA,0x690F,0x688D,0x687E,0x6901,0x68CA,0x6908,0x68D8,0x6922,0x6926,0x68E1,0x690C,0x68CD},
    /*60 area from 5c21*/
    {0x68D4,0x68E7,0x68D5,0x6936,0x6912,0x6904,0x68D7,0x68E3,0x6925,0x68F9,0x68E0,0x68EF,0x6928,0x692A,0x691A,0x6923,
    0x6921,0x68C6,0x6979,0x6977,0x695C,0x6978,0x696B,0x6954,0x697E,0x696E,0x6939,0x6974,0x693D,0x6959,0x6930,0x6961,
    0x695E,0x695D,0x6981,0x696A,0x69B2,0x69AE,0x69D0,0x69BF,0x69C1,0x69D3,0x69BE,0x69CE,0x5BE8,0x69CA,0x69DD,0x69BB,
    0x69C3,0x69A7,0x6A2E,0x6991,0x69A0,0x699C,0x6995,0x69B4,0x69DE,0x69E8,0x6A02,0x6A1B,0x69FF,0x6B0A,0x69F9,0x69F2,
    0x69E7,0x6A05,0x69B1,0x6A1E,0x69ED,0x6A14,0x69EB,0x6A0A,0x6A12,0x6AC1,0x6A23,0x6A13,0x6A44,0x6A0C,0x6A72,0x6A36,
    0x6A78,0x6A47,0x6A62,0x6A59,0x6A66,0x6A48,0x6A38,0x6A22,0x6A90,0x6A8D,0x6AA0,0x6A84,0x6AA2,0x6AA3},
    /*61 area from 5d21*/
    {0x6A97,0x8617,0x6ABB,0x6AC3,0x6AC2,0x6AB8,0x6AB3,0x6AAC,0x6ADE,0x6AD1,0x6ADF,0x6AAA,0x6ADA,0x6AEA,0x6AFB,0x6B05,
    0x8616,0x6AFA,0x6B12,0x6B16,0x9B31,0x6B1F,0x6B38,0x6B37,0x76DC,0x6B39,0x98EE,0x6B47,0x6B43,0x6B49,0x6B50,0x6B59,
    0x6B54,0x6B5B,0x6B5F,0x6B61,0x6B78,0x6B79,0x6B7F,0x6B80,0x6B84,0x6B83,0x6B8D,0x6B98,0x6B95,0x6B9E,0x6BA4,0x6BAA,
    0x6BAB,0x6BAF,0x6BB2,0x6BB1,0x6BB3,0x6BB7,0x6BBC,0x6BC6,0x6BCB,0x6BD3,0x6BDF,0x6BEC,0x6BEB,0x6BF3,0x6BEF,0x9EBE,
    0x6C08,0x6C13,0x6C14,0x6C1B,0x6C24,0x6C23,0x6C5E,0x6C55,0x6C62,0x6C6A,0x6C82,0x6C8D,0x6C9A,0x6C81,0x6C9B,0x6C7E,
    0x6C68,0x6C73,0x6C92,0x6C90,0x6CC4,0x6CF1,0x6CD3,0x6CBD,0x6CD7,0x6CC5,0x6CDD,0x6CAE,0x6CB1,0x6CBE},
    /*62 area from 5e21*/
    {0x6CBA,0x6CDB,0x6CEF,0x6CD9,0x6CEA,0x6D1F,0x884D,0x6D36,0x6D2B,0x6D3D,0x6D38,0x6D19,0x6D35,0x6D33,0x6D12,0x6D0C,
    0x6D63,0x6D93,0x6D64,0x6D5A,0x6D79,0x6D59,0x6D8E,0x6D95,0x6FE4,0x6D85,0x6DF9,0x6E15,0x6E0A,0x6DB5,0x6DC7,0x6DE6,
    0x6DB8,0x6DC6,0x6DEC,0x6DDE,0x6DCC,0x6DE8,0x6DD2,0x6DC5,0x6DFA,0x6DD9,0x6DE4,0x6DD5,0x6DEA,0x6DEE,0x6E2D,0x6E6E,
    0x6E2E,0x6E19,0x6E72,0x6E5F,0x6E3E,0x6E23,0x6E6B,0x6E2B,0x6E76,0x6E4D,0x6E1F,0x6E43,0x6E3A,0x6E4E,0x6E24,0x6EFF,
    0x6E1D,0x6E38,0x6E82,0x6EAA,0x6E98,0x6EC9,0x6EB7,0x6ED3,0x6EBD,0x6EAF,0x6EC4,0x6EB2,0x6ED4,0x6ED5,0x6E8F,0x6EA5,
    0x6EC2,0x6E9F,0x6F41,0x6F11,0x704C,0x6EEC,0x6EF8,0x6EFE,0x6F3F,0x6EF2,0x6F31,0x6EEF,0x6F32,0x6ECC},
    /*63 area from 5f21*/
    {0x6F3E,0x6F13,0x6EF7,0x6F86,0x6F7A,0x6F78,0x6F81,0x6F80,0x6F6F,0x6F5B,0x6FF3,0x6F6D,0x6F82,0x6F7C,0x6F58,0x6F8E,
    0x6F91,0x6FC2,0x6F66,0x6FB3,0x6FA3,0x6FA1,0x6FA4,0x6FB9,0x6FC6,0x6FAA,0x6FDF,0x6FD5,0x6FEC,0x6FD4,0x6FD8,0x6FF1,
    0x6FEE,0x6FDB,0x7009,0x700B,0x6FFA,0x7011,0x7001,0x700F,0x6FFE,0x701B,0x701A,0x6F74,0x701D,0x7018,0x701F,0x7030,
    0x703E,0x7032,0x7051,0x7063,0x7099,0x7092,0x70AF,0x70F1,0x70AC,0x70B8,0x70B3,0x70AE,0x70DF,0x70CB,0x70DD,0x70D9,
    0x7109,0x70FD,0x711C,0x7119,0x7165,0x7155,0x7188,0x7166,0x7162,0x714C,0x7156,0x716C,0x718F,0x71FB,0x7184,0x7195,
    0x71A8,0x71AC,0x71D7,0x71B9,0x71BE,0x71D2,0x71C9,0x71D4,0x71CE,0x71E0,0x71EC,0x71E7,0x71F5,0x71FC},
    /*64 area from 6021*/
    {0x71F9,0x71FF,0x720D,0x7210,0x721B,0x7228,0x722D,0x722C,0x7230,0x7232,0x723B,0x723C,0x723F,0x7240,0x7246,0x724B,
    0x7258,0x7274,0x727E,0x7282,0x7281,0x7287,0x7292,0x7296,0x72A2,0x72A7,0x72B9,0x72B2,0x72C3,0x72C6,0x72C4,0x72CE,
    0x72D2,0x72E2,0x72E0,0x72E1,0x72F9,0x72F7,0x500F,0x7317,0x730A,0x731C,0x7316,0x731D,0x7334,0x732F,0x7329,0x7325,
    0x733E,0x734E,0x734F,0x9ED8,0x7357,0x736A,0x7368,0x7370,0x7378,0x7375,0x737B,0x737A,0x73C8,0x73B3,0x73CE,0x73BB,
    0x73C0,0x73E5,0x73EE,0x73DE,0x74A2,0x7405,0x746F,0x7425,0x73F8,0x7432,0x743A,0x7455,0x743F,0x745F,0x7459,0x7441,
    0x745C,0x7469,0x7470,0x7463,0x746A,0x7476,0x747E,0x748B,0x749E,0x74A7,0x74CA,0x74CF,0x74D4,0x73F1},
    /*65 area from 6121*/
    {0x74E0,0x74E3,0x74E7,0x74E9,0x74EE,0x74F2,0x74F0,0x74F1,0x74F8,0x74F7,0x7504,0x7503,0x7505,0x750C,0x750E,0x750D,
    0x7515,0x7513,0x751E,0x7526,0x752C,0x753C,0x7544,0x754D,0x754A,0x7549,0x755B,0x7546,0x755A,0x7569,0x7564,0x7567,
    0x756B,0x756D,0x7578,0x7576,0x7586,0x7587,0x7574,0x758A,0x7589,0x7582,0x7594,0x759A,0x759D,0x75A5,0x75A3,0x75C2,
    0x75B3,0x75C3,0x75B5,0x75BD,0x75B8,0x75BC,0x75B1,0x75CD,0x75CA,0x75D2,0x75D9,0x75E3,0x75DE,0x75FE,0x75FF,0x75FC,
    0x7601,0x75F0,0x75FA,0x75F2,0x75F3,0x760B,0x760D,0x7609,0x761F,0x7627,0x7620,0x7621,0x7622,0x7624,0x7634,0x7630,
    0x763B,0x7647,0x7648,0x7646,0x765C,0x7658,0x7661,0x7662,0x7668,0x7669,0x766A,0x7667,0x766C,0x7670},
    /*66 area from 6221*/
    {0x7672,0x7676,0x7678,0x767C,0x7680,0x7683,0x7688,0x768B,0x768E,0x7696,0x7693,0x7699,0x769A,0x76B0,0x76B4,0x76B8,
    0x76B9,0x76BA,0x76C2,0x76CD,0x76D6,0x76D2,0x76DE,0x76E1,0x76E5,0x76E7,0x76EA,0x862F,0x76FB,0x7708,0x7707,0x7704,
    0x7729,0x7724,0x771E,0x7725,0x7726,0x771B,0x7737,0x7738,0x7747,0x775A,0x7768,0x776B,0x775B,0x7765,0x777F,0x777E,
    0x7779,0x778E,0x778B,0x7791,0x77A0,0x779E,0x77B0,0x77B6,0x77B9,0x77BF,0x77BC,0x77BD,0x77BB,0x77C7,0x77CD,0x77D7,
    0x77DA,0x77DC,0x77E3,0x77EE,0x77FC,0x780C,0x7812,0x7926,0x7820,0x792A,0x7845,0x788E,0x7874,0x7886,0x787C,0x789A,
    0x788C,0x78A3,0x78B5,0x78AA,0x78AF,0x78D1,0x78C6,0x78CB,0x78D4,0x78BE,0x78BC,0x78C5,0x78CA,0x78EC},
    /*67 area from 6321*/
    {0x78E7,0x78DA,0x78FD,0x78F4,0x7907,0x7912,0x7911,0x7919,0x792C,0x792B,0x7940,0x7960,0x7957,0x795F,0x795A,0x7955,
    0x7953,0x797A,0x797F,0x798A,0x799D,0x79A7,0x9F4B,0x79AA,0x79AE,0x79B3,0x79B9,0x79BA,0x79C9,0x79D5,0x79E7,0x79EC,
    0x79E1,0x79E3,0x7A08,0x7A0D,0x7A18,0x7A19,0x7A20,0x7A1F,0x7980,0x7A31,0x7A3B,0x7A3E,0x7A37,0x7A43,0x7A57,0x7A49,
    0x7A61,0x7A62,0x7A69,0x9F9D,0x7A70,0x7A79,0x7A7D,0x7A88,0x7A97,0x7A95,0x7A98,0x7A96,0x7AA9,0x7AC8,0x7AB0,0x7AB6,
    0x7AC5,0x7AC4,0x7ABF,0x9083,0x7AC7,0x7ACA,0x7ACD,0x7ACF,0x7AD5,0x7AD3,0x7AD9,0x7ADA,0x7ADD,0x7AE1,0x7AE2,0x7AE6,
    0x7AED,0x7AF0,0x7B02,0x7B0F,0x7B0A,0x7B06,0x7B33,0x7B18,0x7B19,0x7B1E,0x7B35,0x7B28,0x7B36,0x7B50},
    /*68 area from 6421*/
    {0x7B7A,0x7B04,0x7B4D,0x7B0B,0x7B4C,0x7B45,0x7B75,0x7B65,0x7B74,0x7B67,0x7B70,0x7B71,0x7B6C,0x7B6E,0x7B9D,0x7B98,
    0x7B9F,0x7B8D,0x7B9C,0x7B9A,0x7B8B,0x7B92,0x7B8F,0x7B5D,0x7B99,0x7BCB,0x7BC1,0x7BCC,0x7BCF,0x7BB4,0x7BC6,0x7BDD,
    0x7BE9,0x7C11,0x7C14,0x7BE6,0x7BE5,0x7C60,0x7C00,0x7C07,0x7C13,0x7BF3,0x7BF7,0x7C17,0x7C0D,0x7BF6,0x7C23,0x7C27,
    0x7C2A,0x7C1F,0x7C37,0x7C2B,0x7C3D,0x7C4C,0x7C43,0x7C54,0x7C4F,0x7C40,0x7C50,0x7C58,0x7C5F,0x7C64,0x7C56,0x7C65,
    0x7C6C,0x7C75,0x7C83,0x7C90,0x7CA4,0x7CAD,0x7CA2,0x7CAB,0x7CA1,0x7CA8,0x7CB3,0x7CB2,0x7CB1,0x7CAE,0x7CB9,0x7CBD,
    0x7CC0,0x7CC5,0x7CC2,0x7CD8,0x7CD2,0x7CDC,0x7CE2,0x9B3B,0x7CEF,0x7CF2,0x7CF4,0x7CF6,0x7CFA,0x7D06},
    /*69 area from 6521*/
    {0x7D02,0x7D1C,0x7D15,0x7D0A,0x7D45,0x7D4B,0x7D2E,0x7D32,0x7D3F,0x7D35,0x7D46,0x7D73,0x7D56,0x7D4E,0x7D72,0x7D68,
    0x7D6E,0x7D4F,0x7D63,0x7D93,0x7D89,0x7D5B,0x7D8F,0x7D7D,0x7D9B,0x7DBA,0x7DAE,0x7DA3,0x7DB5,0x7DC7,0x7DBD,0x7DAB,
    0x7E3D,0x7DA2,0x7DAF,0x7DDC,0x7DB8,0x7D9F,0x7DB0,0x7DD8,0x7DDD,0x7DE4,0x7DDE,0x7DFB,0x7DF2,0x7DE1,0x7E05,0x7E0A,
    0x7E23,0x7E21,0x7E12,0x7E31,0x7E1F,0x7E09,0x7E0B,0x7E22,0x7E46,0x7E66,0x7E3B,0x7E35,0x7E39,0x7E43,0x7E37,0x7E32,
    0x7E3A,0x7E67,0x7E5D,0x7E56,0x7E5E,0x7E59,0x7E5A,0x7E79,0x7E6A,0x7E69,0x7E7C,0x7E7B,0x7E83,0x7DD5,0x7E7D,0x8FAE,
    0x7E7F,0x7E88,0x7E89,0x7E8C,0x7E92,0x7E90,0x7E93,0x7E94,0x7E96,0x7E8E,0x7E9B,0x7E9C,0x7F38,0x7F3A},
    /*70 area from 6621*/
    {0x7F45,0x7F4C,0x7F4D,0x7F4E,0x7F50,0x7F51,0x7F55,0x7F54,0x7F58,0x7F5F,0x7F60,0x7F68,0x7F69,0x7F67,0x7F78,0x7F82,
    0x7F86,0x7F83,0x7F88,0x7F87,0x7F8C,0x7F94,0x7F9E,0x7F9D,0x7F9A,0x7FA3,0x7FAF,0x7FB2,0x7FB9,0x7FAE,0x7FB6,0x7FB8,
    0x8B71,0x7FC5,0x7FC6,0x7FCA,0x7FD5,0x7FD4,0x7FE1,0x7FE6,0x7FE9,0x7FF3,0x7FF9,0x98DC,0x8006,0x8004,0x800B,0x8012,
    0x8018,0x8019,0x801C,0x8021,0x8028,0x803F,0x803B,0x804A,0x8046,0x8052,0x8058,0x805A,0x805F,0x8062,0x8068,0x8073,
    0x8072,0x8070,0x8076,0x8079,0x807D,0x807F,0x8084,0x8086,0x8085,0x809B,0x8093,0x809A,0x80AD,0x5190,0x80AC,0x80DB,
    0x80E5,0x80D9,0x80DD,0x80C4,0x80DA,0x80D6,0x8109,0x80EF,0x80F1,0x811B,0x8129,0x8123,0x812F,0x814B},
    /*71 area from 6721*/
    {0x968B,0x8146,0x813E,0x8153,0x8151,0x80FC,0x8171,0x816E,0x8165,0x8166,0x8174,0x8183,0x8188,0x818A,0x8180,0x8182,
    0x81A0,0x8195,0x81A4,0x81A3,0x815F,0x8193,0x81A9,0x81B0,0x81B5,0x81BE,0x81B8,0x81BD,0x81C0,0x81C2,0x81BA,0x81C9,
    0x81CD,0x81D1,0x81D9,0x81D8,0x81C8,0x81DA,0x81DF,0x81E0,0x81E7,0x81FA,0x81FB,0x81FE,0x8201,0x8202,0x8205,0x8207,
    0x820A,0x820D,0x8210,0x8216,0x8229,0x822B,0x8238,0x8233,0x8240,0x8259,0x8258,0x825D,0x825A,0x825F,0x8264,0x8262,
    0x8268,0x826A,0x826B,0x822E,0x8271,0x8277,0x8278,0x827E,0x828D,0x8292,0x82AB,0x829F,0x82BB,0x82AC,0x82E1,0x82E3,
    0x82DF,0x82D2,0x82F4,0x82F3,0x82FA,0x8393,0x8303,0x82FB,0x82F9,0x82DE,0x8306,0x82DC,0x8309,0x82D9},
    /*72 area from 6821*/
    {0x8335,0x8334,0x8316,0x8332,0x8331,0x8340,0x8339,0x8350,0x8345,0x832F,0x832B,0x8317,0x8318,0x8385,0x839A,0x83AA,
    0x839F,0x83A2,0x8396,0x8323,0x838E,0x8387,0x838A,0x837C,0x83B5,0x8373,0x8375,0x83A0,0x8389,0x83A8,0x83F4,0x8413,
    0x83EB,0x83CE,0x83FD,0x8403,0x83D8,0x840B,0x83C1,0x83F7,0x8407,0x83E0,0x83F2,0x840D,0x8422,0x8420,0x83BD,0x8438,
    0x8506,0x83FB,0x846D,0x842A,0x843C,0x855A,0x8484,0x8477,0x846B,0x84AD,0x846E,0x8482,0x8469,0x8446,0x842C,0x846F,
    0x8479,0x8435,0x84CA,0x8462,0x84B9,0x84BF,0x849F,0x84D9,0x84CD,0x84BB,0x84DA,0x84D0,0x84C1,0x84C6,0x84D6,0x84A1,
    0x8521,0x84FF,0x84F4,0x8517,0x8518,0x852C,0x851F,0x8515,0x8514,0x84FC,0x8540,0x8563,0x8558,0x8548},
    /*73 area from 6921*/
    {0x8541,0x8602,0x854B,0x8555,0x8580,0x85A4,0x8588,0x8591,0x858A,0x85A8,0x856D,0x8594,0x859B,0x85EA,0x8587,0x859C,
    0x8577,0x857E,0x8590,0x85C9,0x85BA,0x85CF,0x85B9,0x85D0,0x85D5,0x85DD,0x85E5,0x85DC,0x85F9,0x860A,0x8613,0x860B,
    0x85FE,0x85FA,0x8606,0x8622,0x861A,0x8630,0x863F,0x864D,0x4E55,0x8654,0x865F,0x8667,0x8671,0x8693,0x86A3,0x86A9,
    0x86AA,0x868B,0x868C,0x86B6,0x86AF,0x86C4,0x86C6,0x86B0,0x86C9,0x8823,0x86AB,0x86D4,0x86DE,0x86E9,0x86EC,0x86DF,
    0x86DB,0x86EF,0x8712,0x8706,0x8708,0x8700,0x8703,0x86FB,0x8711,0x8709,0x870D,0x86F9,0x870A,0x8734,0x873F,0x8737,
    0x873B,0x8725,0x8729,0x871A,0x8760,0x875F,0x8778,0x874C,0x874E,0x8774,0x8757,0x8768,0x876E,0x8759},
    /*74 area from 6a21*/
    {0x8753,0x8763,0x876A,0x8805,0x87A2,0x879F,0x8782,0x87AF,0x87CB,0x87BD,0x87C0,0x87D0,0x96D6,0x87AB,0x87C4,0x87B3,
    0x87C7,0x87C6,0x87BB,0x87EF,0x87F2,0x87E0,0x880F,0x880D,0x87FE,0x87F6,0x87F7,0x880E,0x87D2,0x8811,0x8816,0x8815,
    0x8822,0x8821,0x8831,0x8836,0x8839,0x8827,0x883B,0x8844,0x8842,0x8852,0x8859,0x885E,0x8862,0x886B,0x8881,0x887E,
    0x889E,0x8875,0x887D,0x88B5,0x8872,0x8882,0x8897,0x8892,0x88AE,0x8899,0x88A2,0x888D,0x88A4,0x88B0,0x88BF,0x88B1,
    0x88C3,0x88C4,0x88D4,0x88D8,0x88D9,0x88DD,0x88F9,0x8902,0x88FC,0x88F4,0x88E8,0x88F2,0x8904,0x890C,0x890A,0x8913,
    0x8943,0x891E,0x8925,0x892A,0x892B,0x8941,0x8944,0x893B,0x8936,0x8938,0x894C,0x891D,0x8960,0x895E},
    /*75 area from 6b21*/
    {0x8966,0x8964,0x896D,0x896A,0x896F,0x8974,0x8977,0x897E,0x8983,0x8988,0x898A,0x8993,0x8998,0x89A1,0x89A9,0x89A6,
    0x89AC,0x89AF,0x89B2,0x89BA,0x89BD,0x89BF,0x89C0,0x89DA,0x89DC,0x89DD,0x89E7,0x89F4,0x89F8,0x8A03,0x8A16,0x8A10,
    0x8A0C,0x8A1B,0x8A1D,0x8A25,0x8A36,0x8A41,0x8A5B,0x8A52,0x8A46,0x8A48,0x8A7C,0x8A6D,0x8A6C,0x8A62,0x8A85,0x8A82,
    0x8A84,0x8AA8,0x8AA1,0x8A91,0x8AA5,0x8AA6,0x8A9A,0x8AA3,0x8AC4,0x8ACD,0x8AC2,0x8ADA,0x8AEB,0x8AF3,0x8AE7,0x8AE4,
    0x8AF1,0x8B14,0x8AE0,0x8AE2,0x8AF7,0x8ADE,0x8ADB,0x8B0C,0x8B07,0x8B1A,0x8AE1,0x8B16,0x8B10,0x8B17,0x8B20,0x8B33,
    0x97AB,0x8B26,0x8B2B,0x8B3E,0x8B28,0x8B41,0x8B4C,0x8B4F,0x8B4E,0x8B49,0x8B56,0x8B5B,0x8B5A,0x8B6B},
    /*76 area from 6c21*/
    {0x8B5F,0x8B6C,0x8B6F,0x8B74,0x8B7D,0x8B80,0x8B8C,0x8B8E,0x8B92,0x8B93,0x8B96,0x8B99,0x8B9A,0x8C3A,0x8C41,0x8C3F,
    0x8C48,0x8C4C,0x8C4E,0x8C50,0x8C55,0x8C62,0x8C6C,0x8C78,0x8C7A,0x8C82,0x8C89,0x8C85,0x8C8A,0x8C8D,0x8C8E,0x8C94,
    0x8C7C,0x8C98,0x621D,0x8CAD,0x8CAA,0x8CBD,0x8CB2,0x8CB3,0x8CAE,0x8CB6,0x8CC8,0x8CC1,0x8CE4,0x8CE3,0x8CDA,0x8CFD,
    0x8CFA,0x8CFB,0x8D04,0x8D05,0x8D0A,0x8D07,0x8D0F,0x8D0D,0x8D10,0x9F4E,0x8D13,0x8CCD,0x8D14,0x8D16,0x8D67,0x8D6D,
    0x8D71,0x8D73,0x8D81,0x8D99,0x8DC2,0x8DBE,0x8DBA,0x8DCF,0x8DDA,0x8DD6,0x8DCC,0x8DDB,0x8DCB,0x8DEA,0x8DEB,0x8DDF,
    0x8DE3,0x8DFC,0x8E08,0x8E09,0x8DFF,0x8E1D,0x8E1E,0x8E10,0x8E1F,0x8E42,0x8E35,0x8E30,0x8E34,0x8E4A},
    /*77 area from 6d21*/
    {0x8E47,0x8E49,0x8E4C,0x8E50,0x8E48,0x8E59,0x8E64,0x8E60,0x8E2A,0x8E63,0x8E55,0x8E76,0x8E72,0x8E7C,0x8E81,0x8E87,
    0x8E85,0x8E84,0x8E8B,0x8E8A,0x8E93,0x8E91,0x8E94,0x8E99,0x8EAA,0x8EA1,0x8EAC,0x8EB0,0x8EC6,0x8EB1,0x8EBE,0x8EC5,
    0x8EC8,0x8ECB,0x8EDB,0x8EE3,0x8EFC,0x8EFB,0x8EEB,0x8EFE,0x8F0A,0x8F05,0x8F15,0x8F12,0x8F19,0x8F13,0x8F1C,0x8F1F,
    0x8F1B,0x8F0C,0x8F26,0x8F33,0x8F3B,0x8F39,0x8F45,0x8F42,0x8F3E,0x8F4C,0x8F49,0x8F46,0x8F4E,0x8F57,0x8F5C,0x8F62,
    0x8F63,0x8F64,0x8F9C,0x8F9F,0x8FA3,0x8FAD,0x8FAF,0x8FB7,0x8FDA,0x8FE5,0x8FE2,0x8FEA,0x8FEF,0x9087,0x8FF4,0x9005,
    0x8FF9,0x8FFA,0x9011,0x9015,0x9021,0x900D,0x901E,0x9016,0x900B,0x9027,0x9036,0x9035,0x9039,0x8FF8},
    /*78 area from 6e21*/
    {0x904F,0x9050,0x9051,0x9052,0x900E,0x9049,0x903E,0x9056,0x9058,0x905E,0x9068,0x906F,0x9076,0x96A8,0x9072,0x9082,
    0x907D,0x9081,0x9080,0x908A,0x9089,0x908F,0x90A8,0x90AF,0x90B1,0x90B5,0x90E2,0x90E4,0x6248,0x90DB,0x9102,0x9112,
    0x9119,0x9132,0x9130,0x914A,0x9156,0x9158,0x9163,0x9165,0x9169,0x9173,0x9172,0x918B,0x9189,0x9182,0x91A2,0x91AB,
    0x91AF,0x91AA,0x91B5,0x91B4,0x91BA,0x91C0,0x91C1,0x91C9,0x91CB,0x91D0,0x91D6,0x91DF,0x91E1,0x91DB,0x91FC,0x91F5,
    0x91F6,0x921E,0x91FF,0x9214,0x922C,0x9215,0x9211,0x925E,0x9257,0x9245,0x9249,0x9264,0x9248,0x9295,0x923F,0x924B,
    0x9250,0x929C,0x9296,0x9293,0x929B,0x925A,0x92CF,0x92B9,0x92B7,0x92E9,0x930F,0x92FA,0x9344,0x932E},
    /*79 area from 6f21*/
    {0x9319,0x9322,0x931A,0x9323,0x933A,0x9335,0x933B,0x935C,0x9360,0x937C,0x936E,0x9356,0x93B0,0x93AC,0x93AD,0x9394,
    0x93B9,0x93D6,0x93D7,0x93E8,0x93E5,0x93D8,0x93C3,0x93DD,0x93D0,0x93C8,0x93E4,0x941A,0x9414,0x9413,0x9403,0x9407,
    0x9410,0x9436,0x942B,0x9435,0x9421,0x943A,0x9441,0x9452,0x9444,0x945B,0x9460,0x9462,0x945E,0x946A,0x9229,0x9470,
    0x9475,0x9477,0x947D,0x945A,0x947C,0x947E,0x9481,0x947F,0x9582,0x9587,0x958A,0x9594,0x9596,0x9598,0x9599,0x95A0,
    0x95A8,0x95A7,0x95AD,0x95BC,0x95BB,0x95B9,0x95BE,0x95CA,0x6FF6,0x95C3,0x95CD,0x95CC,0x95D5,0x95D4,0x95D6,0x95DC,
    0x95E1,0x95E5,0x95E2,0x9621,0x9628,0x962E,0x962F,0x9642,0x964C,0x964F,0x964B,0x9677,0x965C,0x965E},
    /*80 area from 7021*/
    {0x965D,0x965F,0x9666,0x9672,0x966C,0x968D,0x9698,0x9695,0x9697,0x96AA,0x96A7,0x96B1,0x96B2,0x96B0,0x96B4,0x96B6,
    0x96B8,0x96B9,0x96CE,0x96CB,0x96C9,0x96CD,0x894D,0x96DC,0x970D,0x96D5,0x96F9,0x9704,0x9706,0x9708,0x9713,0x970E,
    0x9711,0x970F,0x9716,0x9719,0x9724,0x972A,0x9730,0x9739,0x973D,0x973E,0x9744,0x9746,0x9748,0x9742,0x9749,0x975C,
    0x9760,0x9764,0x9766,0x9768,0x52D2,0x976B,0x9771,0x9779,0x9785,0x977C,0x9781,0x977A,0x9786,0x978B,0x978F,0x9790,
    0x979C,0x97A8,0x97A6,0x97A3,0x97B3,0x97B4,0x97C3,0x97C6,0x97C8,0x97CB,0x97DC,0x97ED,0x9F4F,0x97F2,0x7ADF,0x97F6,
    0x97F5,0x980F,0x980C,0x9838,0x9824,0x9821,0x9837,0x983D,0x9846,0x984F,0x984B,0x986B,0x986F,0x9870},
    /*81 area from 7121*/
    {0x9871,0x9874,0x9873,0x98AA,0x98AF,0x98B1,0x98B6,0x98C4,0x98C3,0x98C6,0x98E9,0x98EB,0x9903,0x9909,0x9912,0x9914,
    0x9918,0x9921,0x991D,0x991E,0x9924,0x9920,0x992C,0x992E,0x993D,0x993E,0x9942,0x9949,0x9945,0x9950,0x994B,0x9951,
    0x9952,0x994C,0x9955,0x9997,0x9998,0x99A5,0x99AD,0x99AE,0x99BC,0x99DF,0x99DB,0x99DD,0x99D8,0x99D1,0x99ED,0x99EE,
    0x99F1,0x99F2,0x99FB,0x99F8,0x9A01,0x9A0F,0x9A05,0x99E2,0x9A19,0x9A2B,0x9A37,0x9A45,0x9A42,0x9A40,0x9A43,0x9A3E,
    0x9A55,0x9A4D,0x9A5B,0x9A57,0x9A5F,0x9A62,0x9A65,0x9A64,0x9A69,0x9A6B,0x9A6A,0x9AAD,0x9AB0,0x9ABC,0x9AC0,0x9ACF,
    0x9AD1,0x9AD3,0x9AD4,0x9ADE,0x9ADF,0x9AE2,0x9AE3,0x9AE6,0x9AEF,0x9AEB,0x9AEE,0x9AF4,0x9AF1,0x9AF7},
    /*82 area from 7221*/
    {0x9AFB,0x9B06,0x9B18,0x9B1A,0x9B1F,0x9B22,0x9B23,0x9B25,0x9B27,0x9B28,0x9B29,0x9B2A,0x9B2E,0x9B2F,0x9B32,0x9B44,
    0x9B43,0x9B4F,0x9B4D,0x9B4E,0x9B51,0x9B58,0x9B74,0x9B93,0x9B83,0x9B91,0x9B96,0x9B97,0x9B9F,0x9BA0,0x9BA8,0x9BB4,
    0x9BC0,0x9BCA,0x9BB9,0x9BC6,0x9BCF,0x9BD1,0x9BD2,0x9BE3,0x9BE2,0x9BE4,0x9BD4,0x9BE1,0x9C3A,0x9BF2,0x9BF1,0x9BF0,
    0x9C15,0x9C14,0x9C09,0x9C13,0x9C0C,0x9C06,0x9C08,0x9C12,0x9C0A,0x9C04,0x9C2E,0x9C1B,0x9C25,0x9C24,0x9C21,0x9C30,
    0x9C47,0x9C32,0x9C46,0x9C3E,0x9C5A,0x9C60,0x9C67,0x9C76,0x9C78,0x9CE7,0x9CEC,0x9CF0,0x9D09,0x9D08,0x9CEB,0x9D03,
    0x9D06,0x9D2A,0x9D26,0x9DAF,0x9D23,0x9D1F,0x9D44,0x9D15,0x9D12,0x9D41,0x9D3F,0x9D3E,0x9D46,0x9D48},
    /*83 area from 7321*/
    {0x9D5D,0x9D5E,0x9D64,0x9D51,0x9D50,0x9D59,0x9D72,0x9D89,0x9D87,0x9DAB,0x9D6F,0x9D7A,0x9D9A,0x9DA4,0x9DA9,0x9DB2,
    0x9DC4,0x9DC1,0x9DBB,0x9DB8,0x9DBA,0x9DC6,0x9DCF,0x9DC2,0x9DD9,0x9DD3,0x9DF8,0x9DE6,0x9DED,0x9DEF,0x9DFD,0x9E1A,
    0x9E1B,0x9E1E,0x9E75,0x9E79,0x9E7D,0x9E81,0x9E88,0x9E8B,0x9E8C,0x9E92,0x9E95,0x9E91,0x9E9D,0x9EA5,0x9EA9,0x9EB8,
    0x9EAA,0x9EAD,0x9761,0x9ECC,0x9ECE,0x9ECF,0x9ED0,0x9ED4,0x9EDC,0x9EDE,0x9EDD,0x9EE0,0x9EE5,0x9EE8,0x9EEF,0x9EF4,
    0x9EF6,0x9EF7,0x9EF9,0x9EFB,0x9EFC,0x9EFD,0x9F07,0x9F08,0x76B7,0x9F15,0x9F21,0x9F2C,0x9F3E,0x9F4A,0x9F52,0x9F54,
    0x9F63,0x9F5F,0x9F60,0x9F61,0x9F66,0x9F67,0x9F6C,0x9F6A,0x9F77,0x9F72,0x9F76,0x9F95,0x9F9C,0x9FA0},
    /*84 area from 7421*/
    {0x582F,0x69C7,0x9059,0x7464,0x51DC,0x7199,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},

    /*85-89 areas are not used*/
#if 0
    /*90 area*/
    {0xe0c9,0xe0ca,0xe0cb,0xe0cc,0xe0cd,0xe0ce,0xe0cf,0xe0d0,0xe0d1,0xe0d2,0xe0d3,0xe0d4,
    0xe0d5,0xe0d6,0xe0d7,0xe0d8,0xe0d9,0xe0da,0xe0db,0xe0dc,0xe0dd,0xe0de,0xe0df,0xe0e0,
    0xe0e1,0xe0e2,0xe0e3,0xe0e4,0xe0e5,0xe0e6,0xe0e7,0xe0e8,0xe0e9,0xe0ea,0xe0eb,0xe0ec,
    0xe0ed,0xe0ee,0xe0ef,0xe0f0,0xe0f1,0xe0f2,0xe0f3,0xe0f4,0xe0f5,0xe0f6,0xe0f7,0xe0f8,
    0xe0f9,0xe0fa,0xe0fb,0xe0fc,0xe0fd,0xe0fe,0xe0ff,0xe180,0xe181,0xe182,0xe183,0xe184,
    0xe185,0xe186,0xe187,0xe188,0xe189,0xe18a,0xe18b,0xe18c,0xe18d,0xe18e,0xe18f,0xe190,
    0xe191,0xe192,0xe193,0xe194,0xe195,0xe196,0xe197,0xe198,0xe199,0xe19a,0xe19b,0xe19c,
    0xe19d,0xe19e,0xe19f,0xe1a0,0xe1a1,0xe1a2,0xe1a3,0xe1a4,0xe1a5,0xe1a6},

    //mapping for row = 91
    {0xe1a7,0xe1a8,0xe1a9,0xe1aa,0xe1ab,0xe1ac,0xe1ad,0xe1ae,0xe1af,0xe1b0,0xe1b1,0xe1b2,
    0xe1b3,0xe1b4,0xe1b5,0xe1b6,0xe1b7,0xe1b8,0xe1b9,0xe1ba,0xe1bb,0xe1bc,0xe1bd,0xe1be,
    0xe1bf,0xe1c0,0xe1c1,0xe1c2,0xe1c3,0xe1c4,0xe1c5,0xe1c6,0xe1c7,0xe1c8,0xe1c9,0xe1ca,
    0xe1cb,0xe1cc,0xe1cd,0xe1ce,0xe1cf,0xe1d0,0xe1d1,0xe1d2,0xe1d3,0xe1d4,0xe1d5,0xe1d6,
    0xe1d7,0xe1d8,0xe1d9,0xe1da,0xe1db,0xe1dc,0xe1dd,0xe1de,0xe1df,0xe1e0,0xe1e1,0xe1e2,
    0xe1e3,0xe1e4,0xe1e5,0xe1e6,0xe1e7,0xe1e8,0xe1e9,0xe1ea,0xe1eb,0xe1ec,0xe1ed,0xe1ee,
    0xe1ef,0xe1f0,0xe1f1,0xe1f2,0xe1f3,0xe1f4,0xe1f5,0xe1f6,0xe1f7,0xe1f8,0xe1f9,0xe1fa,
    0xe1fb,0xe1fc,0xe1fd,0xe1fe,0xe1ff,0xe280,0xe281,0xe282,0xe283,0xe284},
    //mapping for row = 92
    {0xe285,0xe286,0xe287,0xe288,0xe289,0xe28a,0xe28b,0xe28c,0xe28d,0xe28e,0x33a1,0x33a5,
    0x339d,0x33a0,0x33a4,0xe28f,0x2488,0x2489,0x248a,0x248b,0x248c,0x248d,0x248e,0x248f,
    0x2490,0xe290,0xe291,0xe292,0xe293,0xe294,0xe295,0xe296,0xe297,0xe298,0xe299,0xe29a,
    0xe29b,0xe29c,0xe29d,0xe29e,0xe29f,0x3233,0x3236,0x3232,0x3231,0x3239,0xe2a0,0x25b6,
    0x25c0,0x3016,0x3017,0xe2a1,0xe2a2,0xe2a3,0xe2a4,0xe2a5,0xe2a6,0xe2a7,0xe2a8,0xe2a9,
    0xe2aa,0xe2ab,0xe2ac,0xe2ad,0xe2ae,0xe2af,0xe2b0,0xe2b1,0xe2b2,0xe2b3,0xe2b4,0xe2b5,
    0xe2b6,0xe2b7,0xe2b8,0xe2b9,0xe2ba,0xe2bb,0xe2bc,0xe2bd,0xe2be,0xe2bf,0xe2c0,0xe2c1,
    0xe2c2,0xe3a7,0xe3a8,0xe2c3,0xe2c4,0xe2c5,0xe2c6,0xe2c7,0xe2c8,0xe2c9},
    //mapping for row = 93
    {0x322a,0x322b,0x322c,0x322d,0x322e,0x322f,0x3230,0x3237,0x337e,0x337d,0x337c,0x337b,
    0xe2ca,0xe2cb,0x3036,0xe2cc,0xe2cd,0xe2ce,0xe2cf,0xe2d0,0xe2d1,0xe2d2,0xe2d3,0xe2d4,
    0xe2d5,0xe2d6,0xe2d7,0xe2d8,0xe2d9,0xe2da,0xe2db,0xe2dc,0xe2dd,0xe2de,0xe2df,0xe2e0,
    0xe2e1,0xe2e2,0x2113,0x338f,0x3390,0x33ca,0x339e,0x33a2,0x3371,0xe2e3,0xe2e4,0xbd,
    0xe2e5,0x2153,0x2154,0xbc,0xbe,0x2155,0x2156,0x2157,0x2158,0x2159,0x215a,0xe2e6,
    0x215b,0xe2e7,0xe2e8,0x2600,0x2601,0x2602,0xe2e9,0xe2ea,0xe2eb,0xe2ec,0xe2ed,0x2666,
    0x2665,0x2663,0x2660,0xe2ee,0xe2ef,0x203c,0x2049,0xe2f1,0xe2f2,0xe2f3,0xe2f4,0xe2f5,
    0xe2f6,0xe2f7,0xe2f8,0xe2f9,0xe2fa,0x266c,0xe2fb,0xe2fc,0xe2fd,0xe2fe},
    //mapping for row = 94
    {0x2160,0x2161,0x2162,0x2163,0x2164,0x2165,0x2166,0x2167,0x2168,0x2169,0x216a,0x216b,
    0x2470,0x2471,0x2472,0x2473,0x2474,0x2475,0x2476,0x2477,0x2478,0x2479,0x247a,0x247b,
    0x247c,0x247d,0x247e,0x247f,0x3251,0x3252,0x3253,0x3254,0xe383,0xe384,0xe385,0xe386,
    0xe387,0xe388,0xe389,0xe38a,0xe38b,0xe38c,0xe38d,0xe38e,0xe38f,0xe390,0xe391,0xe392,
    0xe393,0xe394,0xe395,0xe396,0xe397,0xe398,0xe399,0xe39a,0xe39b,0xe39c,0x3255,0x3256,
    0x3257,0x3258,0x3259,0x325a,0x2460,0x2461,0x2462,0x2463,0x2464,0x2465,0x2466,0x2467,
    0x2468,0x2469,0x246a,0x246b,0x246c,0x246d,0x246e,0x246f,0x2776,0x2777,0x2778,0x2779,
    0x277a,0x277b,0x277c,0x277d,0x277e,0x277f,0x24eb,0x24ec,0x325b,0xe3a6}
#endif
    /*90 area start from 7a21*/
    {0xe500,0xe501,0xe502,0xe503,0xe504,0xe505,UNDEFC,0xe506,0xe507,0xe508,0xe509,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0xe50a,
    0xe50b,UNDEFC,UNDEFC,0xe50c,0xe50d,0xe50e,0xe50f,0xe510,0xe511,0xe512,0xe513,0xe514,0xe515,0xe516,0xe517,0xe518,
    0xe519,0xe51a,0xe51b,0xe51c,0xe51d,0xe51e,0xe51f,0xe520,UNDEFC,UNDEFC,UNDEFC,UNDEFC,0xe0f5,0xe0f6,0xe0f7,0xE5C5,
    0xE5C6,0xE5C7,0xE5C8,0xE5C9,0xE5CA,0xE5CB,0xE5CC,0xE5CD,0xE5CE,0xE5CF,0xE5D0,0xE5D1,0xE5D2,0xE5D3,0xE5D4,0xE521,
    0xE522,0xE5D5,0xE5D6,0xE5D7,0xE5D8,0xE5D9,0xe18f,0xE5EB,0xE191,0xE5DD,0xE5DE,0xE5DF,0xE5E0,0xE5E1,0xE5E2,0xE5E3,
    0xE5E4,0xE5E5,0xE5E6,0xE5E7,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},

    /*91 area start from 7b21*/
    {0xe523,0xe524,0xe525,0xe526,0xe527,0xe528,0xe529,0xe52a,0xe52b,0xe52c,0xe52d,0xe52e,0xe52f,0xe530,0xe531,0xe532,
    0xe533,0xe534,0xe535,0xe536,0xe537,0xe538,0xe539,0xe53a,0xe53b,0xe53c,0xe53d,0xe53e,0xe53f,0xe540,0xe541,0xe542,
    0xe543,0xe544,0xe545,0xe546,0xe547,0xe548,0xe549,0xe54a,0xe54b,0xe54c,0xe54d,0xe54e,0xe54f,0xe550,0xe551,0xe552,
    0xe553,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,
    UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC,UNDEFC},
    /*92 area start from 7c21*/
    {0xe687,0xe688,0xe689,0xe68a,0xe554,0xe555,0xe68b,0xe68c,0xe68d,0xe68e,0xe68f,0xe690,0xe691,0xe692,0xe693,0xe556,
    0xe694,0xe695,0xe696,0xe697,0xe698,0xe699,0xe69a,0xe69b,0xe69c,0xe557,0xe558,0xe559,0xe55a,0xe55b,0xe55c,0xe55d,
    0xe55e,0xe55f,0xe560,0xe561,0xe562,0xe563,0xe564,0xe565,0xe566,0xe69d,0xe69e,0xe69f,0xe700,0xe701,0xe601,0xe702,
    0xe703,0xe684,0xe704,0xe602,0xe603,0xe604,0xe567,0xe568,0xe569,0xe56a,0xe56b,0xe56c,0xe56d,0xe56e,0xe56f,0xe570,
    0xe571,0xe572,0xe573,0xe574,0xe575,0xe576,0xe577,0xe578,0xe579,0xe57a,0xe57b,0xe57c,0xe57d,0xe57e,0xe57f,0xe580,
    0xe581,0xe582,0xe583,0xe584,0xe585,0xe586,0xe587,0xe588,0xe589,0xe58a,0xe706,UNDEFC,UNDEFC,UNDEFC},

    /*93 area start from 7d21*/
    {0x322a,0x322b,0x322c,0x322d,0x322e,0x322f,0x3230,0x3237,0x337e,0x337d,0x337c,0x337b,0x2116,0x2121,0xe713,0xe714,
    0xe715,0x3222,0x3221,0xe58e,0xe58f,0xe590,0xe591,0xe592,0xe593,0xe594,0xe595,0xe596,0xe597,0xe598,0xe599,0xe59a,
    0xe59b,0xe59c,0xe59d,0xe59e,0xe59f,0xe5a0,0xe718,0xe719,0xe71a,0xe71b,0xe71c,0xe71d,0xe71e,UNDEFC,UNDEFC,0xe71f,
    0xe5ea,0xe720,0xe721,0xe722,0xe723,0xe5a1,0xe724,0xe725,0xe726,0xe727,0xe728,0xe729,0xe72a,0xe5a2,0xe5a3,0x2600,
    0x2601,0x2602,0xe72e,0xe72f,0xe730,0xe2ec,0xe5ed,0xe731,0xe732,0xe733,0xe734,0xe5ee,0xe735,0xe736,0xe737,0xe5a4,
    0xe738,0xe5a6,0xe739,0xe5a8,0xe73a,0xe5aa,UNDEFC,0xe5ef,0xe5f0,0xe73b,0xe73c,UNDEFC,UNDEFC,UNDEFC},

    /*94 area start from 7e21*/
    {0x2160,0x2161,0x2162,0x2163,0x2164,0x2165,0x2166,0x2167,0x2168,0x2169,0x216a,0x216b,0x2470,0x2471,0x2472,0x2473,
     0x2474,0x2475,0x2476,0x2477,0x2478,0x2479,0x247a,0x247b,0x247c,0x247d,0x247e,0x247f,0x3251,0x3252,0x3253,0x3254,
     0xe5ab,0xe5ac,0xe5ad,0xe5ae,0xe5af,0xe5b0,0xe5b1,0xe5b2,0xe5b3,0xe5b4,0xe5b5,0xe5b6,0xe5b7,0xe5b8,0xe5b9,0xe5ba,
     0xe5bb,0xe5bc,0xe5bd,0xe5be,0xe5bf,0xe5c0,0xe5c1,0xe5c2,0xe5c3,0xe5c4,0x3255,0x3256,0x3257,0x3258,0x3259,0x325a,
     0x2460,0x2461,0x2462,0x2463,0x2464,0x2465,0x2466,0x2467,0x2468,0x2469,0x246a,0x246b,0x246c,0x246d,0x246e,0x246f,
     0x2776,0x2777,0x2778,0x2779,0x277a,0x277b,0x277c,0x277d,0x277e,0x277f,0x24eb,0x24ec,0x325b,UNDEFC }
    };


    static HI_U16 iso_8859_15_2unicode_map[] =
    {
       0x00A1,0x00A2,0x00A3,0x20AC,0x00A5,0x0160,0x00A7,0x0161,0x00A9,0x00AA,0x00AB,0x00AC,0x00AD,0x00AE,0x00AF,0x00B0,
       0x00B1,0x00B2,0x00B3,0x017D,0x00B5,0x00B6,0x00B7,0x017E,0x00B9,0x00BA,0x00BB,0x0152,0x0153,0x0178,0x00BF,0x00C0,
       0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x00C7,0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,0x00D0,
       0x00D1,0x00D2,0x00D3,0x00D4,0x00D5,0x00D6,0x00D7,0x00D8,0x00D9,0x00DA,0x00DB,0x00DC,0x00DD,0x00DE,0x00DF,0x00E0,
       0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x00E7,0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,0x00F0,
       0x00F1,0x00F2,0x00F3,0x00F4,0x00F5,0x00F6,0x00F7,0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x00FD,0x00FE,0x00FF,
     };


    static HI_U16 special_char_map[][12] =
    {
        {0x0020, 0x007F},
        {0x00A4, 0x00A6, 0x00A8, 0x00B4, 0x00B8, 0x00BC, 0x00BD, 0x00BE},
        {0x2026, 0x2530, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2122, 0x215B, 0x2153, 0x215D, 0x215E}
    };

    *IsNonspace = 0;
    switch(u8CharSet)
    {
        case ARIBCC_CHARSET_ALPHANUMERIC:
        case ARIBCC_CHARSET_PALPHANUMERIC:
        {
            return (u8Text);
        }
        case ARIBCC_CHARSET_KATAKANA:
        case ARIBCC_CHARSET_PKATAKANA:
        {
            HI_U16 hiragana_map[9]={0x30fd,0x30fe,0x2015,0x3002,0x300c,0x300d,0x3001,0x30fb,0x25a0};
            /*Arib,0x21-0x76, unicode: 0x30a1-0x30f6*/
            if( u8Text >= 0x77 && u8Text <= 0x7f )
            {
                ret_unicode = hiragana_map[u8Text-0x77];
            }
            else
            {
                ret_unicode = u8Text + 0x3080; //- 0x21 + 0x30a1;
            }
            /*0x77-0x7e need to be maped in other way*/
        }
        break;
        case ARIBCC_CHARSET_HIRAGANA:
        case ARIBCC_CHARSET_PHIRAGANA:
        {
            HI_U16 hiragana_map[9]={0x309d,0x309e,0x2015,0x3002,0x300c,0x300d,0x3001,0x30fb,0x25a0};
            /*0x77-0x7e need to be maped in other ways*/
            if( u8Text >= 0x77 && u8Text <= 0x7f )
            {
                ret_unicode = hiragana_map[u8Text-0x77];
            }
            else
            {
                /*Arib 0x21-0x73, unicode: 0x3041-0x3093*/
                ret_unicode = u8Text + 0x3020; /*-0x21 + 0x3041*/
            }
        }
            break;
        case ARIBCC_CHARSET_MOSAICA:
            break;
        case ARIBCC_CHARSET_MOSAICB:
            break;
        case ARIBCC_CHARSET_MOSAICC:
            break;
        case ARIBCC_CHARSET_MOSAICD:
            break;
        case ARIBCC_CHARSET_JISX0201KATAKANA:
            /*0x21-0x5f, unicode: 0xff61-0xff9f*/
            ret_unicode = u8Text + 0xff40;
            break;
        case ARIBCC_CHARSET_KANJI:
        {
                HI_U8 table_code,char_code;
                if( gUnicodeFirstByte > 0x20 )
                {
                    table_code = gUnicodeFirstByte - 0x21;
                    if( table_code >= 15 && table_code <= 83 )
                    {
                        table_code -= 7;
                    }
                    else if( table_code >= 89 && table_code <= 93 )
                    {
                        if( table_code == 91 && stPrtDirection==CCARIB_PRINT_VERTICAL )
                        {
                            unicodeforkanji[79][47] = 0x25bc;
                            unicodeforkanji[79][48] = 0x25b2;
                            unicodeforkanji[79][49] = 0x3010;
                            unicodeforkanji[79][50] = 0x3011;
                        }
                        table_code -= 12;
                    }
                    else if( (table_code == 0 && u8Text >= 0x2d && u8Text <= 0x32) ||
                        (table_code == 1 && u8Text == 0x7e) )
                    {
                        *IsNonspace = 1;
                    }

                    if( u8Text > 0x7e || u8Text < 0x21)
                    {
                        HI_ERR_CC("invalide u8Text = %d\n",u8Text);
                        ret_unicode = 0;
                        gUnicodeFirstByte = 0;
                        break;
                    }
                    char_code = u8Text - 0x21;
                    OSD_TRACE("table_code:%d, char_code:%d\n",table_code,char_code);

                    if((table_code < 83) && (char_code < 94))
                    {
                        ret_unicode = unicodeforkanji[table_code][char_code];
                    }
                    else
                    {
                        ret_unicode = UNDEFC;
                    }

                    gUnicodeFirstByte = 0;
                }
                else
                {
                    /*to wait the two bytes to map unicode*/
                    gUnicodeFirstByte = u8Text;
                    ret_unicode = 0;
                }
            break;
        }
        case ARIBCC_CHARSET_DRCS0:
        if( gUnicodeDrcs0Value == 0 )
        {
            gUnicodeDrcs0Value = (u8Text << 8);
        }
        else
        {
            ret_unicode = (gUnicodeDrcs0Value | u8Text);
            gUnicodeDrcs0Value = 0;
        }

            break;
        case ARIBCC_CHARSET_DRCS1:
        case ARIBCC_CHARSET_DRCS2:
        case ARIBCC_CHARSET_DRCS3:
        case ARIBCC_CHARSET_DRCS4:
        case ARIBCC_CHARSET_DRCS5:
        case ARIBCC_CHARSET_DRCS6:
        case ARIBCC_CHARSET_DRCS7:                 /*0x14, DRCS-7*/
        case ARIBCC_CHARSET_DRCS8:
        case ARIBCC_CHARSET_DRCS9:
        case ARIBCC_CHARSET_DRCS10:
        case ARIBCC_CHARSET_DRCS11:
        case ARIBCC_CHARSET_DRCS12:
        case ARIBCC_CHARSET_DRCS13:
        case ARIBCC_CHARSET_DRCS14:
        case ARIBCC_CHARSET_DRCS15:
        /*convert to 0xf0-0xfe, drcs1 mape to 0xf0..,  because the 0xf0..-0xfe..
                 are not used in unicode.*/
            ret_unicode = ((u8CharSet - ARIBCC_CHARSET_DRCS1 + 0xf0)<<8)|u8Text;
            break;

        case ARIBCC_CHARSET_LATIN:
             //1. no mapping in 8895_15 ,return 0x20
             //2. return the mapping
             //3. unicode is same as 8859_15
             if(((u8Text>=0x01)&&(u8Text<=0x1F))||((u8Text>=0x7F)&&(u8Text<=0x9F)))
             {
                  ret_unicode = 0x20;
             } else if (((u8Text>=0x21) && (u8Text<=0x7F)))
             {

                 ret_unicode = iso_8859_15_2unicode_map[u8Text - 0x21];
             }
             else
             {
                 ret_unicode = u8Text;
             }

             break;
        case ARIBCC_CHARSET_SPECIAL:
        {
            /*
             * For special char set refer to ABNTNBR15606 11.4.2.1
             * 0x21, 0x30~0x37, 0x40~4B
            */
            HI_U32 index1 = ((u8Text >> 4) & 0xF) - 2 ;
            HI_U32 index2 = u8Text & 0xF;

            //check size
            if((index1 > 2) || (index2 > 11))
            {
                return 0x20;
            }

            ret_unicode = special_char_map[index1][index2];
        }
        break;

        default:
            return 0x20;
    }
    return ret_unicode;
}


/*the lut table is as following: blink flag(1bit)  blend factor(4bit)  blue(8bit) green(8bit) red(8bit)*/
HI_U8 _AribCC_Color2Index(HI_UNF_CC_COLOR_S color,HI_U8 u8IsFlash)
{
    HI_U8 u8RedIndex,u8GreenIndex,u8BlueIndex, u8Index,u8Alpha;
    HI_U8 au8Offset[4] = {8,65,0,129};
    HI_U8 au8ColorMap[64]={
            0, 15,11,4, 16,17,18,19,9, 20,13,21,2, 22,23,6,
            24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
            8, 40,12,41,42,43,44,45,10,46,14,47,48,49,50,51,
            1, 52,53,5, 54,55,56,57,58,59,60,61,3, 62,63,7};

    if(u8IsFlash == 1)
    {
        u8Alpha = 3;
    }
    else
    {
        u8Alpha = color.u8Alpha / 127;
    }
    if( u8Alpha == 0 )
    {
        return ARIBCC_TRANSPARENT_COLOR;
    }

    u8RedIndex = color.u8Red / 85;
    u8GreenIndex = color.u8Green / 85;
    u8BlueIndex = color.u8Blue / 85;

    u8Index = u8RedIndex*16 + u8GreenIndex*4 + u8BlueIndex;

    /*map the real color to index*/
    u8Index = au8ColorMap[u8Index];

    if( u8Alpha == 2 && u8Index > 7 )
    {
        u8Index++;
    }

    u8Index += au8Offset[u8Alpha];
    return u8Index;
}

static HI_S32 _AribCC_DrawFontBitmap(HI_S32 ccType,HI_S32 x,HI_S32 y,HI_U8 width, HI_U8 height,
                  HI_U8 depth, HI_U8 *pbitmap)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(ccType);
    HI_S32 i,j,align_width,pixel_number;
    HI_U8 fg_color,fg_mid_color,bg_color,bg_mid_color;
    HI_S32 byte_number=0,bit_offset=8;
    HI_U8 pixel_value;
    HI_U8 pbuf[36*36];  /*the biggest font size is 36x36*/

    if(pText == NULL)
        return -1;

    /*first expand the pattern into real index value, the ARIB drcs has the fixed format:
      depth = 2. So each 2 bits standard for one pixel of font bitmap. Each of the value of pettern is as following:
      3: foreground color  2:foreground mid color  1:background color 0: background mid color*/

    fg_color = _AribCC_Color2Index(pText->stFgColor,pText->u8IsFlash);
    fg_mid_color = _AribCC_Color2Index(pText->stFgMidColor,0);
    bg_color = _AribCC_Color2Index(pText->stBgColor,0);
    bg_mid_color = _AribCC_Color2Index(pText->stBgMidColor,0);

    /*if flash is on. Set flash background color*/
    if( pText->u8IsFlash )
    {
        if( pText->stBgColor.u8Alpha == 0 )
        {
            ARIBCC_GRAPHIC_SetFlashBGColor(ccType,pText->stFgColor,pText->stWinColor);
        }
        else
        {
            ARIBCC_GRAPHIC_SetFlashBGColor(ccType,pText->stFgColor,pText->stBgColor);
        }
    }
    byte_number = 0;
    align_width = ((width+3)>>2)*4;

    for( i = 0; i<height; i++ )
    {
        for( j=0; j<width; j++ )
        {
            pixel_number = i*align_width+j;
            bit_offset -= 2;
            pixel_value = (pbitmap[byte_number]>>bit_offset)&0x3;
            if( bit_offset == 0 )
            {
                bit_offset = 8;
                byte_number++;
            }

            if( pixel_value == 3 )
            {
                pbuf[pixel_number] = fg_color;
            }
            else if( pixel_value == 2 )
            {
                pbuf[pixel_number] = fg_mid_color;
            }
            else if( pixel_value == 1 )
            {
                pbuf[pixel_number] = bg_mid_color;
            }
            else if( pixel_value == 0 )
            {
                pbuf[pixel_number] = bg_color;
            }
        }

    }

    /*then call the System memory to frame buffer function*/
    ARIBCC_GRAPHIC_SysMem2FBCopy(ccType,pbuf,width,height,8,align_width,x,y);
    return 0;
}

static ARIBCC_DRCSEntry_S* _AribCC_SearchDrcs(HI_S32 CCType,HI_U8 drcs_type,HI_U8 char_set,HI_U8 code1,
                       HI_U8 code2,HI_U8 width,HI_U8 height)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    ARIBCC_DRCSEntry_S *pSearchDrcs = NULL;

    if(pText == NULL || drcs_type >= MAX_DRCS_TYPE || char_set >= MAX_DRCS_SETS)
    {
        return NULL;
    }
    if( pText->pastDrcs[drcs_type][char_set] == NULL )
    {
        HI_ERR_CC("the drcs %d is empty!\n",drcs_type);
        return NULL;
    }

    pSearchDrcs = pText->pastDrcs[drcs_type][char_set];

    /*convert the character code set to original(0x41-0x4f) */
    if( char_set > ARIBCC_CHARSET_DRCS0 )
    {
        code1 += char_set - 0xf0;
    }
    while(pSearchDrcs->pvNext != NULL)
    {
        if(pSearchDrcs->u8Code1 == code1 && pSearchDrcs->u8Code2 == code2 &&
           pSearchDrcs->u8Width == width && pSearchDrcs->u8Height== height)
        {
            /*find the correct drcs bitmap, return it*/
            return pSearchDrcs;
        }

        pSearchDrcs = pSearchDrcs->pvNext;
    }
    /*check the last list element*/
    if(pSearchDrcs->u8Code1 == code1 && pSearchDrcs->u8Code2 == code2 &&
               pSearchDrcs->u8Width == width && pSearchDrcs->u8Height== height)
    {
        /*find the correct drcs bitmap, return it*/
        return pSearchDrcs;
    }

    return NULL;
}


static HI_S32 _AribCC_DrawDrcs(HI_S32 CCType, HI_S32 x, HI_S32 y, HI_U8 char_set,HI_U16 *pBuf,HI_U8 length)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    ARIBCC_DRCSEntry_S *pSearchDrcs = NULL;
    HI_U8 code1,code2;
    HI_S32 i;
    HI_U8 width,height;
    HI_S32 char_width = 0, char_height = 0;

    if(pText == NULL)
    {
        return HI_FAILURE;
    }

    if( pText->u8FontScale == 0 )
    {
        width = height = pText->u8FontWidth;
    }
    else if( pText->u8FontScale == CHARSIZE_HALF_SINGLE )
    {
        width = pText->u8FontWidth/2;
        height = pText->u8FontHeight;
    }
    else if( pText->u8FontScale == CHARSIZE_HALF_ALL )
    {
        width = height = pText->u8FontWidth/2;
    }
    else
    {
        HI_ERR_CC("there is no drcs font for this font scale!\n");
        return HI_FAILURE;
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);
    for( i = 0; i < length; i++ )
    {
        code1 = ((pBuf[i] & 0xff00)>>8);
        code2 = (pBuf[i] & 0xff);

        /*convert the character code set to original(0x41-0x4f) ,from function _AribCC_MapUnicode*/
        if( char_set > ARIBCC_CHARSET_DRCS0 )
        {
            code1 = char_set - ARIBCC_CHARSET_DRCS1 + 0x41;
        }

        /*first to find the drcs from temporary drcs set*/
        pSearchDrcs = _AribCC_SearchDrcs(CCType,TEMPORARY_DRCS,char_set-ARIBCC_CHARSET_DRCS0,code1,code2,width,height);
        if( pSearchDrcs == NULL )
        {
            /*if the Temporary drcs doesn't include this drcs character, to find it at default drcs*/
            pSearchDrcs = _AribCC_SearchDrcs(CCType,DEFAULT_DRCS,char_set-ARIBCC_CHARSET_DRCS0,code1,code2,width,height);
            if( pSearchDrcs == NULL )
            {
                HI_ERR_CC("The drcs of code1=0x%02x,code2=0x%02x is not found!\n",code1,code2);
                continue;
            }
        }
        /*find the correct drcs bitmap, draw it*/

        _AribCC_DrawFontBitmap(CCType,x,y,pSearchDrcs->u8Width,pSearchDrcs->u8Height,
                          pSearchDrcs->u8Depth,pSearchDrcs->pu8Bitmap);
        x += char_width;
    }
    return HI_FAILURE;
}


static void _AribCC_ClearDrcs(HI_S32 CCType, HI_U8 drcs_type)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    ARIBCC_DRCSEntry_S *pLastDrcs = NULL;
    ARIBCC_DRCSEntry_S *pDelete = NULL;
    HI_S32 i;

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    for( i = 0; i < MAX_DRCS_SETS; i++ )
    {
        if( pText->pastDrcs[drcs_type][i] == NULL )
        {
            continue;
        }
        pLastDrcs = pText->pastDrcs[drcs_type][i];
        while( pLastDrcs->pvNext != NULL )
        {
            pDelete = pLastDrcs;
            pLastDrcs = pLastDrcs->pvNext;
            free(pDelete->pu8Bitmap);
            free(pDelete);
            pDelete = NULL;
        }
        free(pLastDrcs->pu8Bitmap);
        free(pLastDrcs);
        pLastDrcs = NULL;
        pText->pastDrcs[drcs_type][i] = NULL;
    }
    return;
}


static HI_S32 _AribCC_DrawCharBoxVertical(HI_S32 CCType,HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, ARIBCC_TEXTBOX_S box)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if(pText == NULL)
    {
        return HI_FAILURE;
    }
    if( width < 1 || height < 1 )
    {
        HI_ERR_CC("width or height zero! width:%d,height:%d\n",width,height);
        return HI_FAILURE;
    }

    if( box.u8top==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x+width-1, y, x+width-1, y+height-1,pText->stFgColor);
    }
    if( box.u8btm==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x,y,x,y+height-1,pText->stFgColor);
    }
    if( box.u8left==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x,y,x+width-1,y,pText->stFgColor);
    }

    if( box.u8right == 1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x, y+height-1, x+width-1, y+height-1,pText->stFgColor);
    }

    return HI_SUCCESS;
}


static HI_S32 _AribCC_DrawCharBoxHorizontal(HI_S32 CCType,HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, ARIBCC_TEXTBOX_S box)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if(pText == NULL)
    {
        return HI_FAILURE;
    }

    if( width < 1 || height < 1 )
    {
        HI_ERR_CC("width or height zero! width:%d,height:%d\n",width,height);
        return HI_FAILURE;
    }

    if( box.u8top==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x,y,x+width-1,y,pText->stFgColor);
    }
    if( box.u8btm==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x, y+height-1, x+width-1, y+height-1,pText->stFgColor);
    }
    if( box.u8left==1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x,y,x,y+height-1,pText->stFgColor);
    }

    if( box.u8right == 1 )
    {
        ARIBCC_GRAPHIC_DrawLine(CCType,x+width-1, y, x+width-1, y+height-1,pText->stFgColor);
    }

    return HI_SUCCESS;
}

static HI_S32 _AribCC_DrawCaptionToWindow(HI_S32 CCType,HI_U16 *pu16Buf, HI_U16 u16BufLen,HI_U8 u8HasNonspace)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);

    HI_U8 counter,draw_length,space_num;
    HI_S32 x,y;
    HI_S32 char_width = 0, char_height = 0;
    HI_U8 empty_cols;  /*the number of characters that can be drawn at current row*/
    //HI_U8 empty_rows; /*rows that can be drawn from current cursor in window*/

    HI_S32 net_width = 0, net_height = 0;  /*the font width and height without the character interval*/
    /*the variable is to patch the non spacing character of the stream ts20st003_020b.trp h-1*/
    static HI_U16 pre_char_code = 0,pre_is_nonspace=0;
    HI_U8 char_interval;

    HI_U8 is_nonspace_char = 0;

    /*if there is no caption. return*/
    if(pstText == NULL || u16BufLen == 0 )
    {
        HI_ERR_CC("Param invalid!\n");
        return HI_FAILURE;
    }

    OSD_TRACE("_AribCC_DrawCaptionToWindow,Len = %d\n",u16BufLen);
    if( pre_is_nonspace == 1 && _AribCC_IsNonspaceCode(pre_char_code) )
    {
        is_nonspace_char = 1;
    }
    pre_char_code = pu16Buf[u16BufLen-1];
    pre_is_nonspace = u8HasNonspace;

    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);

    if( char_width == 0 || char_height == 0 )
    {
        HI_ERR_CC("character size is zero:width=%d,height=%d\n",char_width,char_height);
        return HI_FAILURE;
    }

    /*horizontal scroll*/
    if( (pstText->u8ScrollType == ARIBCC_CHARSCROLL_FIX)||(pstText->u8ScrollType == ARIBCC_CHARSCROLL_ROLLOUT))
    {
        /*store the caption into the buffer*/
        if( pstText->stCaption.u16Length + pstText->stScrollBuf.u16Length < 64 )
        {
            HI_S32 j;
            for( j=0; j<pstText->stCaption.u16Length; j++ )
            {
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u16text = pstText->stCaption.au16Text[j];
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stBgColor = pstText->stBgColor;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stBgMidColor = pstText->stBgMidColor;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stTextBox = pstText->stTextBox;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8CharInterval = pstText->u8CharInterval;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stEdgeColor = pstText->stEdgeColor;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].ueEdgeType = pstText->u8EdgeType;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stFgColor = pstText->stFgColor;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].stFgMidColor = pstText->stFgMidColor;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8FontHeight = pstText->u8FontHeight;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8FontScale = pstText->u8FontScale;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8FontWidth = pstText->u8FontWidth;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8IsFlash = pstText->u8IsFlash;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8IsUnderline = pstText->u8IsUnderline;
                pstText->stScrollBuf.astScrollText[pstText->stScrollBuf.u16Length].u8LineInterval = pstText->u8LineInterval;
                pstText->stScrollBuf.u16Length++;
            }
            pstText->stCaption.u16Length = 0;
        }
        return 0;
    }
    /*vertical scroll*/
    else if( pstText->u8ScrollType == ARIBCC_LINESCROLL_FIX||pstText->u8ScrollType == ARIBCC_LINESCROLL_ROLLOUT ||
             pstText->u8WinScrollMode == 1 )
    {
        /*we should set the cursor to last line, just consider the horizontal print*/
        pstText->stCursor.u16MaxRow = pstText->stWinAttr.u16height/char_height;
        pstText->stCursor.u16Y = pstText->stWinAttr.u16height;
        pstText->stCursor.u16Row = pstText->stCursor.u16MaxRow - 1;
    }

    /*set text color*/
    ARIBCC_GRAPHIC_SetFGColor(CCType,pstText->stFgColor,pstText->u8IsFlash);
    ARIBCC_GRAPHIC_SetBGColor(CCType,pstText->stBgColor);
    ARIBCC_GRAPHIC_SetMidColor(CCType,pstText->stFgMidColor,pstText->stBgMidColor);
    ARIBCC_GRAPHIC_SetEdgeColor(CCType,pstText->stEdgeColor);

    /*if flash is on. Set flash background color*/
    if( pstText->u8IsFlash )
    {
        if( pstText->stBgColor.u8Alpha == 0 )
        {
            ARIBCC_GRAPHIC_SetFlashBGColor(CCType,pstText->stFgColor,pstText->stWinColor);
        }
        else
        {
            ARIBCC_GRAPHIC_SetFlashBGColor(CCType,pstText->stFgColor,pstText->stBgColor);
        }
    }

    ARIBCC_GRAPHIC_SetUnderline(CCType,pstText->u8IsUnderline);
    ARIBCC_GRAPHIC_SetOrnament(CCType,pstText->u8EdgeType);
    ARIBCC_GRAPHIC_SetFontScale(CCType,(ARIBCC_CHARSIZE_E)pstText->u8FontScale);
    ARIBCC_GRAPHIC_SetPrintDirection(CCType,pstText->stFormat.stPrtDirect);
    ARIBCC_GRAPHIC_SetFontSize(CCType,pstText->u8FontHeight);

    /*draw text to window*/
    counter = 0;
    /*because we set the default to zero. So we should add it to correct position
      if the position control function is not called*/
    OSD_TRACE("pstText->stCursor.u16X = %d,pstText->stCursor.u16Y = %d\n",pstText->stCursor.u16X,pstText->stCursor.u16Y);
    if( pstText->stCursor.u16Y == 0 && pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL)
    {
        pstText->stCursor.u16Y += char_height;
    }
    if( pstText->stCursor.u16X == pstText->stWinAttr.u16width && pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
#ifdef VERTICAL_CENTER_BASE_LINE
        pstText->stCursor.u16X = pstText->stWinAttr.u16width - (char_width>>1);
#else
        pstText->stCursor.u16X = pstText->stWinAttr.u16width - char_width;
#endif
    }

    _AribCC_CalcCharSize(CCType,FONTSIZE_NO_INTERVAL, &net_width,&net_height);
    /*calculate the char interval*/
    if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        char_interval = char_width - net_width;
    }
    else
    {
        char_interval = char_height - net_height;
    }

    while( u16BufLen > 0 )
    {
        /*there are more than one line characters,draw first line*/
        if( pstText->stCursor.u16Y > pstText->stWinAttr.u16height  )
        {
            HI_ERR_CC("reach the last row of the vertical print,Y = %d,Hgigth = %d\n",pstText->stCursor.u16Y,pstText->stWinAttr.u16height);
            return HI_FAILURE;
        }

        empty_cols = (pstText->stWinAttr.u16width - pstText->stCursor.u16X)/char_width;
        if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
        {
            empty_cols = (pstText->stWinAttr.u16height - pstText->stCursor.u16Y)/char_height;
            //empty_rows = (pstText->stCursor.u16X)/char_width;
        }

        /*if columns reach the end of the row, need to change line*/
        if( empty_cols == 0 )
        {
            /*carriage return*/
            if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
            {
                pstText->stCursor.u16Y = 0;
                pstText->stCursor.u16Column = 0;
#ifdef VERTICAL_CENTER_BASE_LINE
                if( pstText->stCursor.u16X >= ((char_width*3)>>1) )
#else
                if( pstText->stCursor.u16X >= char_width )
#endif
                {
                    pstText->stCursor.u16X -= char_width;
                }
                pstText->u8MaxCharWidth = 0;
                pstText->stCursor.u16Row ++;
            }
            else
            {
                pstText->stCursor.u16X = 0;
                pstText->stCursor.u16Column = 0;
                pstText->u8MaxCharHeight = 0;
                if( pstText->u8ScrollType >= ARIBCC_LINESCROLL_FIX ||pstText->u8WinScrollMode == 1)
                {
                    pstText->u8IsWinUpdate = 0;
                    ARIBCC_OSD_Show(CCType);
                    if( pstText->u8ScrollType == ARIBCC_LINESCROLL_FIX &&pstText->u8ScrolledRows == pstText->stCursor.u16MaxRow)
                    {
                        /*if the window is full with caption. we should clear it and restart scroll*/
                        ARIBCC_OSD_ClearScreen(CCType);
                        ARIBCC_OSD_Show(CCType);
                        pstText->u8ScrolledRows = 0;
                    }
                }
                else
                {
                    pstText->stCursor.u16Y += char_height;
                    pstText->stCursor.u16Row ++;
                }
            }
            OSD_TRACE("empty column is zero, change to next row\n");
            continue;
        }

        if( u16BufLen >= empty_cols )
        {
            draw_length = empty_cols;
        }
        else
        {
            draw_length = u16BufLen;
        }

        /*wo should add window position to correct the physical position of display text*/
        x = pstText->stCursor.u16X + pstText->stWinAttr.u16x;
        y = pstText->stCursor.u16Y + pstText->stWinAttr.u16y;

        if( is_nonspace_char )
        {
            space_num = draw_length - 1;
        }
        else
        {
            space_num = draw_length;
        }
        /*first we draw the text background*/
        /* There is no need of drawing text background here */
        #if 0
        if( space_num > 0 && pstText->stBgColor.u8Alpha != 0 )
        {
            if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
            {
                ARIBCC_GRAPHIC_FillWindow(CCType,x,y-char_height,space_num*char_width,char_height,pstText->stBgColor);
            }
            else
            {
#ifdef VERTICAL_CENTER_BASE_LINE
                ARIBCC_GRAPHIC_FillWindow(CCType,x-(char_width>>1),y,char_width,space_num*char_height,pstText->stBgColor);
#else
                ARIBCC_GRAPHIC_FillWindow(CCType,x,y,char_width,space_num*char_height,pstText->stBgColor);
#endif
            }
        }
        #endif

        /*the following if is used to change the top aligned draw to bottom aligned draw*/
        if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
        {
#if 1
            y -= char_height;/*correct y position*/
#else
            HI_S32 scale_line_interval = (char_height - net_height + 1)>>1;
            if( scale_line_interval >= ((pstText->u8LineInterval+1)>>1) )
            {
                /*it's not height and width half*/
                y = y - net_height - ((pstText->u8LineInterval+1)>>1);
            }
            else
            {
                y = y - net_height - ((pstText->u8LineInterval+1)>>2);
            }
#endif
        }
        else
        {
#ifdef VERTICAL_CENTER_BASE_LINE
            x -= (net_width>>1);
            //y += ((char_height - net_height)>>1);
#else
            x += (char_width - net_width + 1)/2;
#endif
        }

        if( is_nonspace_char )
        {
            if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
            {
                if( x >= char_width )
                {
                    x -= char_width;
                }
            }
            else
            {
                if( y >= char_height )
                {
                    y -= char_height;
                }
            }
            is_nonspace_char = 0;
        }
        OSD_TRACE("x = %d, y = %d,row = %d, column = %d,draw_length=%d\n",
                     x,y,pstText->stCursor.u16Row, pstText->stCursor.u16Column,draw_length);

        if( pstText->u8CurCharSet >= ARIBCC_CHARSET_DRCS0 && pstText->u8CurCharSet <= ARIBCC_CHARSET_DRCS15 )
        {
            _AribCC_DrawDrcs(CCType,x,y,pstText->u8CurCharSet, pu16Buf+counter, draw_length);
        }
        else
        {
            if((pstText->stLastDrawCaption.u16Length == draw_length) && (pstText->u16LastRowNum != pstText->stCursor.u16Row))
            {
                HI_U32 i = 0;
                for(i = 0; i < draw_length; i++)
                {
                    if (*(pu16Buf+counter + i) != pstText->stLastDrawCaption.au16Text[i])
                    {
                        break;
                    }
                }
                if(i >= draw_length)
                {
                    OSD_TRACE("                            FillWindow \n");
                    ARIBCC_GRAPHIC_ClearScreen(CCType);
                    //ARIBCC_GRAPHIC_FillWindow(CCType,pstText->stWinAttr.u16x, pstText->stWinAttr.u16y, pstText->stWinAttr.u16width, pstText->stWinAttr.u16height, pstText->stWinColor);
                }
            }
            
            ARIBCC_GRAPHIC_TextOut(CCType,x,y,pu16Buf+counter, draw_length, 0,char_interval);
            pstText->u16LastRowNum = pstText->stCursor.u16Row;
            pstText->stLastDrawCaption.u16Length = draw_length;
            memcpy(pstText->stLastDrawCaption.au16Text, pu16Buf+counter, draw_length * sizeof(HI_U16));
        }

        /*here should add the font box drawing function*/
        if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
        {
            _AribCC_DrawCharBoxHorizontal(CCType,x,y,char_width*u16BufLen,char_height-pstText->u8LineInterval,pstText->stTextBox);
        }
        else
        {
            _AribCC_DrawCharBoxVertical(CCType,x,y,char_width-pstText->u8LineInterval,char_height*u16BufLen,pstText->stTextBox);
        }

        if( char_width > pstText->u8MaxCharWidth )
        {
            pstText->u8MaxCharWidth = char_width;
        }
        if( char_height > pstText->u8MaxCharHeight )
        {
            pstText->u8MaxCharHeight = char_height;
        }

        u16BufLen -= draw_length;
        counter += draw_length;

        if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
        {
            pstText->stCursor.u16X += space_num*char_width;
            pstText->stCursor.u16Column += space_num;
        }
        else if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
        {
            pstText->stCursor.u16Y += space_num*char_height;
            pstText->stCursor.u16Column += space_num;
        }
        OSD_TRACE("after draw %d chars x=%d,y=%d\n",draw_length,pstText->stCursor.u16X,pstText->stCursor.u16Y);
    }

    return HI_SUCCESS;
}

static void _AribCC_SetBitmapFlashColor(HI_U8 *pflash, HI_U8 length,ARIBCC_TEXTATTR_S *pText)
{

    HI_S32 i;
    HI_UNF_CC_COLOR_S fg_color = {0,0,0,0};

    for (i=0; i<length; i++)
    {
        if( ARIBCC_GRAPHIC_GetAribPalette(pflash[i],&fg_color) == -1)
        {
            HI_ERR_CC("get arib palette failed for the flash color %d\n",pflash[i]);
        }
        ARIBCC_GRAPHIC_SetFlashBGColor(ARIBCC_TYPE_SD,fg_color, pText->stWinColor);
    }
    return;
}

static void _AribCC_HorizontalScrollThreadTask(void *parameters)
{
    HI_S32 CCType;
    HI_U8 scroll_type;
    HI_U8 scroll_speed;
    ARIBCC_TEXTATTR_S *pText = NULL;
    HI_S32 x,draw_y,move_y,offset;
    HI_S32 char_w = 0, char_h = 0, i, scroll_buf_length = 0;
    HI_S32 netCharW = 0, netCharH = 0;
    HI_U8 back_font_scale;
    HI_U16 text_length=0;
    ARIBCC_HScrollArgs_S *args = (ARIBCC_HScrollArgs_S*)parameters;

    //prctl(PR_SET_NAME, "SBTVD CC scroll Thread", 0, 0, 0);

    CCType = args->s32CCType;
    scroll_type = args->u8ScrollType;
    scroll_speed = args->u8ScrollSpeed;

    pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("The pText returned is NULL!\n");
        return;
    }
    back_font_scale = pText->u8FontScale;
    move_y = pText->stCursor.u16Y + pText->stWinAttr.u16y;

    scroll_buf_length = pText->stScrollBuf.u16Length;

    if(scroll_buf_length > MAX_SCROLL_BUF_SIZE)
    {
        scroll_buf_length = MAX_SCROLL_BUF_SIZE;
    }

    for(i=0; i<scroll_buf_length; i++)
    {
        pText->u8FontScale = pText->stScrollBuf.astScrollText[i].u8FontScale;
        _AribCC_CalcCharSize(1,FONTSIZE_NO_INTERVAL, &netCharW,&netCharH);
        _AribCC_CalcCharSize(1,FONTSIZE_WITH_INTERVAL, &char_w,&char_h);
        /*bottom aligned*/
        if( pText->stCursor.u16Y == 0 )
        {
            pText->stCursor.u16Y += char_h;
        }
        x = pText->stWinAttr.u16x + pText->stWinAttr.u16width - char_w;
        draw_y = pText->stCursor.u16Y + pText->stWinAttr.u16y - netCharH - (char_h - netCharH + 1)/2;
        move_y = pText->stCursor.u16Y + pText->stWinAttr.u16y - char_h;
        /*the for loop to draw the caption with the scroll effect*/
        text_length += char_w;

        /*set text color*/
        ARIBCC_GRAPHIC_SetFGColor(CCType,pText->stScrollBuf.astScrollText[i].stFgColor,pText->stScrollBuf.astScrollText[i].u8IsFlash);
        ARIBCC_GRAPHIC_SetBGColor(CCType,pText->stScrollBuf.astScrollText[i].stBgColor);
        ARIBCC_GRAPHIC_SetMidColor(CCType,pText->stScrollBuf.astScrollText[i].stFgMidColor,pText->stScrollBuf.astScrollText[i].stBgMidColor);
        ARIBCC_GRAPHIC_SetEdgeColor(CCType,pText->stScrollBuf.astScrollText[i].stEdgeColor);

        /*if flash is on. Set flash background color*/
        if( pText->stScrollBuf.astScrollText[i].u8IsFlash )
        {
            ARIBCC_GRAPHIC_SetFlashBGColor(CCType,pText->stScrollBuf.astScrollText[i].stFgColor,pText->stScrollBuf.astScrollText[i].stBgColor);
        }

        ARIBCC_GRAPHIC_SetUnderline(CCType,pText->stScrollBuf.astScrollText[i].u8IsUnderline);
        ARIBCC_GRAPHIC_SetOrnament(CCType,pText->stScrollBuf.astScrollText[i].ueEdgeType);
        ARIBCC_GRAPHIC_SetFontScale(CCType,(ARIBCC_CHARSIZE_E)pText->stScrollBuf.astScrollText[i].u8FontScale);

        /*first draw the caption*/
        ARIBCC_GRAPHIC_FillRect(CCType,x,move_y,char_w,char_h,pText->stBgColor);
        ARIBCC_GRAPHIC_TextOut(CCType,x,draw_y,&(pText->stScrollBuf.astScrollText[i].u16text),1,1,pText->u8CharInterval);

        /*then move to the left one characters*/
        if( i < scroll_buf_length - 1 )
        {
            pText->u8FontScale = pText->stScrollBuf.astScrollText[i+1].u8FontScale;
            _AribCC_CalcCharSize(1,FONTSIZE_WITH_INTERVAL, &char_w,&char_h);
            ARIBCC_GRAPHIC_RollLeft(CCType,pText->stWinAttr.u16x,move_y,
                                    pText->stWinAttr.u16width,char_h,char_w,
                                    pText->stWinColor,scroll_speed);
        }
        if( gIsHscrollCancel == 1 )
        {
            pText->u8IsHscrollFinish = 1;
            pText->stScrollBuf.u16Length = 0;
            return;
        }
    }

    if( pText->stWinAttr.u16width > char_w*pText->stScrollBuf.u16Length)
    {
        offset = pText->stWinAttr.u16width - text_length;
    }
    else
    {
        offset = 0;
    }
    pText->stScrollBuf.u16Length = 0;
    if( scroll_type == ARIBCC_CHARSCROLL_FIX )
    {
        ARIBCC_GRAPHIC_RollLeft(CCType,pText->stWinAttr.u16x,move_y,
                                pText->stWinAttr.u16width,char_h,offset,
                                pText->stWinColor,scroll_speed);
        /*set the cursor after scrolling*/
        pText->stCursor.u16X = pText->stWinAttr.u16width - offset;
    }
    else if( scroll_type == ARIBCC_CHARSCROLL_ROLLOUT )
    {
        ARIBCC_GRAPHIC_RollLeft(CCType,pText->stWinAttr.u16x,move_y,
                                pText->stWinAttr.u16width,char_h,pText->stWinAttr.u16width,
                                pText->stWinColor,scroll_speed);
        pText->stCursor.u16X = 0;
    }
    pText->u8FontScale = back_font_scale;
    pText->u8IsHscrollFinish = 1;

    return;
}

static HI_S32 _AribCC_ScrollTextHorizontal(HI_S32 CCType,HI_U8 scroll_type,HI_U8 scroll_speed)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    static ARIBCC_HScrollArgs_S args;
    pthread_t hscroll_thread;

    if(pText == NULL)
    {
        return HI_FAILURE;
    }

    if( pText->stScrollBuf.u16Length <= 0 )
    {
        return HI_FAILURE;
    }
    args.s32CCType = CCType;
    args.u8ScrollSpeed = scroll_speed;
    args.u8ScrollType = scroll_type;

    /*first to judge whether the last hscroll command finished or not*/
    if( pText->u8IsHscrollFinish == 0 )
    {
        gIsHscrollCancel = 1;
        while(pText->u8IsHscrollFinish == 0)
        {
            HI_ERR_CC("wait for the last hscroll finish!\n");
            usleep(30000);
        }
        gIsHscrollCancel = 0;
    }

    pText->u8IsHscrollFinish = 0;
    if(pthread_create(&hscroll_thread, NULL, (void* (*)(void *))_AribCC_HorizontalScrollThreadTask, (void*)&args))
    {
        HI_ERR_CC("create hscroll thread failed!\n");
        return HI_FAILURE;
    }
    /*sleep some time to let the thread begin*/
    usleep(1000000);
    pText->pidHscrollThread = hscroll_thread;
    return HI_SUCCESS;
}

static HI_S32 _AribCC_ScrollTextVertical(HI_S32 CCType,HI_U8 scroll_type,HI_U8 scroll_speed)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 char_height = 0,char_width = 0;
    //HI_S32 empty_rows;
    //HI_S32 srcy,scroll_height,scroll,dsty,scroll_step;

    if(pText == NULL)
    {
        return HI_FAILURE;
    }

    _AribCC_CalcCharSize(CCType, FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
    
    //empty_rows = (pText->stWinAttr.u16height - pText->stCursor.u16Y)/char_height;
    OSD_TRACE("window height = %d, cursor.y = %d,char_height=%d,scroll_type=%d\n",
           pText->stWinAttr.u16height,pText->stCursor.u16Y,char_height,scroll_type);

#if 0
    scroll_step = 2;
    dsty = pText->stWinAttr.u16y;
    //scroll = char_height + 1;   /* +1 is to fix the tge bug. The TGE_TextOut_Ext will draw one more line in height*/
    scroll = char_height; /*20090330 find that we can't add this additional 1 of xoceco bug 486*/
    ARIBCC_GRAPHIC_RollupStart(CCType);

    while ( scroll > 0 )
    {
        HI_S32 row_offset;

        if ( scroll >= scroll_step )
        {
            row_offset = scroll_step;
        }
        else
        {
            row_offset = scroll;
        }
        scroll_height = char_height - row_offset;
        srcy = dsty + row_offset;

        ARIBCC_GRAPHIC_Rollup(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16width,dsty,srcy,scroll_height,pText->stWinColor,
                  pText->u8ScrollSpeed);
        ARIBCC_GRAPHIC_ShowWindow(CCType,pText->stWinAttr.u16x,dsty+scroll_height,pText->stWinAttr.u16width,
                      pText->stWinAttr.u16height - scroll_height,
                      0, char_height - scroll,0);


        scroll -= row_offset;
        usleep(15000);
    }
    #else
     ARIBCC_GRAPHIC_Rollup(CCType, pText->stWinAttr.u16x, pText->stWinAttr.u16width, 
                                            pText->stWinAttr.u16y, pText->stWinAttr.u16y+ pText->stWinAttr.u16height, 
                                            char_height, pText->stWinColor, 0/*pText->u8ScrollSpeed*/);
    /*
     ARIBCC_GRAPHIC_ShowWindow(CCType,pText->stWinAttr.u16x,dsty+scroll_height,pText->stWinAttr.u16width,
                      pText->stWinAttr.u16height - scroll_height,
                      0, char_height - scroll,0);*/
    #endif
    /*at the same time, move the off_screen buffer*/

    #if 0

    ARIBCC_GRAPHIC_BlockMove(CCType,0,char_height,pText->stWinAttr.u16width,
                               pText->stWinAttr.u16height - char_height,0,0);
    ARIBCC_GRAPHIC_FillWindow(CCType,0,pText->stWinAttr.u16height-char_height,
                              pText->stWinAttr.u16width,char_height,pText->stWinColor);
    
    #endif

    pText->stCursor.u16Y = pText->stWinAttr.u16height;

    pText->stCursor.u16X = 0;
    return HI_SUCCESS;
}

/*to judge whether the cc and SD windows overlaped.
 * return value: 1: overlaped   0: not overlap */
static HI_S32 _AribCC_IsCCSDOverlap(void)
{
    ARIBCC_TEXTATTR_S *pCCText = _AribCC_GetAribText(ARIBCC_TYPE_CC);
    ARIBCC_TEXTATTR_S *pSDText = _AribCC_GetAribText(ARIBCC_TYPE_SD);
    HI_S32 horizontal_overlap=0,vertical_overlap=0;

    if( pCCText->stWinAttr.u16x >= pSDText->stWinAttr.u16x )
    {
        if( pCCText->stWinAttr.u16x < pSDText->stWinAttr.u16x + pSDText->stWinAttr.u16width )
        {
            horizontal_overlap = 1;
        }
    }
    else
    {
        if( pCCText->stWinAttr.u16x + pCCText->stWinAttr.u16width > pSDText->stWinAttr.u16x )
        {
            horizontal_overlap = 1;
        }
    }

    if( pCCText->stWinAttr.u16y >= pSDText->stWinAttr.u16y )
    {
        if( pCCText->stWinAttr.u16y < pSDText->stWinAttr.u16y + pSDText->stWinAttr.u16height )
        {
            vertical_overlap = 1;
        }
    }
    else
    {
        if( pCCText->stWinAttr.u16y + pCCText->stWinAttr.u16height > pSDText->stWinAttr.u16y )
        {
            vertical_overlap = 1;
        }
    }

    if( (HI_U32)horizontal_overlap & (HI_U32)vertical_overlap )
    {
        return 1;
    }
    return 0;
}

/************************************************************************
* function name: ARIBCC_OSD_Init(...)
* description:  Init osd,init text attribute
*
*
*************************************************************************/
void ARIBCC_OSD_Init()
{
    HI_S32 i;

    for (i=0; i < MAX_ARIBCC_NUM; i++)
    {
        memset(&g_astArib_text[i],0,sizeof(ARIBCC_TEXTATTR_S));

        g_astArib_text[i].stBgColor.u8Alpha = 255;
        g_astArib_text[i].stBgColor.u8Blue = 0;
        g_astArib_text[i].stBgColor.u8Green = 0;
        g_astArib_text[i].stBgColor.u8Red = 0;
        g_astArib_text[i].stFgColor.u8Alpha = 255;
        g_astArib_text[i].stFgColor.u8Blue = 255;
        g_astArib_text[i].stFgColor.u8Green = 255;
        g_astArib_text[i].stFgColor.u8Red = 255;

        //display window
        g_astArib_text[i].stWinAttr.u16x = 0;
        g_astArib_text[i].stWinAttr.u16y = 0;
        g_astArib_text[i].stWinAttr.u16width = 0;
        g_astArib_text[i].stWinAttr.u16height = 0;

        //display plane
        g_astArib_text[i].stFormat.res_h = 0;//540;
        g_astArib_text[i].stFormat.res_w = 0;//960;

        g_astArib_text[i].u8FontHeight = 24;
        g_astArib_text[i].u8FontWidth = 24;
        g_astArib_text[i].stFormat.stPrtDirect = CCARIB_PRINT_HORIZONTAL;
        g_astArib_text[i].u8IsWinUpdate = 1;

        g_astArib_text[i].stClearColor.u8Alpha = 0;
        g_astArib_text[i].stClearColor.u8Red = 0;
        g_astArib_text[i].stClearColor.u8Green = 0;
        g_astArib_text[i].stClearColor.u8Red = 0;

        g_astArib_text[i].stWinColor.u8Red = 8;
        g_astArib_text[i].stWinColor.u8Green = 8;
        g_astArib_text[i].stWinColor.u8Blue = 8;
        g_astArib_text[i].stWinColor.u8Alpha = 0;

        g_astArib_text[i].u8IsFlash = 0;

        g_astArib_text[i].u8MaxCharHeight = 0;
        g_astArib_text[i].u8MaxCharWidth = 0;

        g_astArib_text[i].s32RepeateNum = -1;
        g_astArib_text[i].u8ScrollType = 0;/*No roll up,add by gwx*/
        g_astArib_text[i].u8WinScrollMode = 0;

        g_astArib_text[i].u8IsHscrollFinish = 1;
        g_astArib_text[i].stCaption.aenTable[ARIBCC_GR] = ARIBCC_CODESET_G2;
        g_astArib_text[i].stCaption.aenTable[ARIBCC_GL] = ARIBCC_CODESET_G0;
        g_astArib_text[i].stCaption.aenCharSet[ARIBCC_CODESET_G0] = ARIBCC_CHARSET_ALPHANUMERIC;
        g_astArib_text[i].stCaption.aenCharSet[ARIBCC_CODESET_G1] = ARIBCC_CHARSET_ALPHANUMERIC;
        g_astArib_text[i].stCaption.aenCharSet[ARIBCC_CODESET_G2] = ARIBCC_CHARSET_LATIN;
        g_astArib_text[i].stCaption.aenCharSet[ARIBCC_CODESET_G3] = ARIBCC_CHARSET_SPECIAL;

        g_astArib_text[i].u16LastRowNum = 0;
        memset(&g_astArib_text[i].stLastDrawCaption, 0, sizeof(ARIBCC_CAPTION_S));
    }

    gIsHscrollCancel = 0;
    return;
}

/************************************************************************
* function name: ARIBCC_OSD_DeInit(...)
* description:  DeInit osd
*
*
*************************************************************************/
void ARIBCC_OSD_DeInit(void)
{
    HI_U8 i = 0;
    for ( i = 0; i < MAX_ARIBCC_NUM; i++)
    {
        memset(&g_astArib_text[i],0,sizeof(ARIBCC_TEXTATTR_S));
    }

}


/************************************************************************************
* function name: ARIBCC_OSD_SetCursorPosition(...)
* description:  set text cursor
* arguments:    CCType: 0 sd 1 cc
*
*************************************************************************************/
void ARIBCC_OSD_SetCursorPosition(HI_S32 CCType, HI_S32 request, HI_S32 s32Row, HI_S32 s32Col, HI_S32 s32x, HI_S32 s32y)
{
    ARIBCC_TEXTATTR_S *pAribText = _AribCC_GetAribText(CCType);
    HI_S32 char_width = 0, char_height = 0;
    HI_U8 char_size = 0;

    if( pAribText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    OSD_TRACE("\tPosition Control!Cursur command:%d\n",request);
    /*when do the position control, if the character size isn't normal, we should use the normal
         character size to change position*/
    if( request == SET_CURSOR_APD || request == SET_CURSOR_APU )
    {
        char_size = pAribText->u8FontScale;
        pAribText->u8FontScale = 0;
    }

    /*adjust the default cursor value*/
    _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);
    if( pAribText->stCursor.u16Y == 0 && pAribText->stFormat.stPrtDirect == CCARIB_PRINT_HORIZONTAL )
    {
        pAribText->stCursor.u16Y += char_height;
    }
    if( pAribText->stCursor.u16X == pAribText->stWinAttr.u16width &&
        pAribText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
    {
#ifdef VERTICAL_CENTER_BASE_LINE
        pAribText->stCursor.u16X = pAribText->stWinAttr.u16width - (char_width>>1);
#else
        pAribText->stCursor.u16X = pAribText->stWinAttr.u16width - char_width;
#endif
    }

    switch(request)
    {
        case SET_CURSOR_APB:
            _AribCC_BackwardActivePos(CCType);
            break;
        case SET_CURSOR_APF:
            _AribCC_ForwardActivePos(CCType);
            break;
        case SET_CURSOR_APD:
            _AribCC_DownActivePos(CCType);
            break;
        case SET_CURSOR_APU:
            _AribCC_UpActivePos(CCType);
            break;
        case SET_CURSOR_PAPF:
            _AribCC_ForwardActivePosByParam(CCType,s32Col);
            break;
        case SET_CURSOR_APR:
            _AribCC_ReturnActivePos(CCType);
            break;
        case SET_CURSOR_APS:
            _AribCC_SetActivePos(CCType,s32Row,s32Col);
            break;
        case SET_CURSOR_ACPS:
            _AribCC_SetActivePosByCoordinate(CCType,s32x,s32y);
            break;
        default:
            break;
    }
    /*restore the font scale information*/
    if( request == SET_CURSOR_APD || request == SET_CURSOR_APU )
    {
        pAribText->u8FontScale = char_size;
    }

    OSD_TRACE("Current Cursor: s32x=%d,s32y=%d,row=%d,column=%d\n",pAribText->stCursor.u16X,
           pAribText->stCursor.u16Y,pAribText->stCursor.u16Row,pAribText->stCursor.u16Column);
    return;
}

/********************************************************************************
* function name: ARIBCC_OSD_ClearScreen(...)
* description:  clear the cc screen
* arguments:    CCType: 0 sd 1 cc
*                     just erase the text, the window still shows
*********************************************************************************/
void ARIBCC_OSD_ClearScreen(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pAribText = _AribCC_GetAribText(CCType);
    ARIBCC_TEXTATTR_S *pOtherText;
    HI_S32 other_cc_type = ARIBCC_TYPE_CC;

    if( pAribText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    if( CCType == ARIBCC_TYPE_CC )
    {
        other_cc_type = ARIBCC_TYPE_SD;
    }

    if( pAribText->u8IsHscrollFinish == 0 )
    {
        gIsHscrollCancel = 1;
        while( pAribText->u8IsHscrollFinish == 0 )
        {
            HI_INFO_CC("wait for the hscroll finish!is_scroll_finish:%d,is_scroll_cancel:%d\n",
                         pAribText->u8IsHscrollFinish,gIsHscrollCancel);
            usleep(30000);
        }
        gIsHscrollCancel = 0;
    }

    (HI_VOID)ARIBCC_GRAPHIC_ClearScreen(CCType);

    _AribCC_SetDefaultCursor(CCType);
    pAribText->u8ScrolledRows = 0;
    pAribText->u8IsTextDraw = 0;

    /*if the cc and sd are not permitted to be shown,just return*/
    if( gIsCCShowEnable == 0 )
    {
       return;
    }
    /*the next sentence is only a patch for the special case, now only the ARIBCC_OSD_Show valide
      after the ARIBCC_OSD_DrawText, ARIBCC_OSD_DrawBitmap, ARIBCC_OSD_ClearScreen*/
    gIsTextBitmapDrawn = 1;

    /* added for the horizontal scroll*/
    ARIBCC_GRAPHIC_FillRect(CCType,pAribText->stWinAttr.u16x,pAribText->stWinAttr.u16y,pAribText->stWinAttr.u16width,
                             pAribText->stWinAttr.u16height,pAribText->stWinColor);

#ifdef ARIBCC_SD_SHARE_ONEPLANE
    pOtherText = _AribCC_GetAribText(other_cc_type);
    /*may be the CC and SD are overlap. So we must redraw the another one when one is cleared*/
    if( pOtherText->u8IsTextDraw )
    {
        _AribCC_CheckAndReviseWinPos(other_cc_type);
        ARIBCC_GRAPHIC_ShowWindow(other_cc_type,pOtherText->stWinAttr.u16x,pOtherText->stWinAttr.u16y,pOtherText->stWinAttr.u16width,
                      pOtherText->stWinAttr.u16height,0,0,0);
    }
#endif
    return;
}

/**************************************************************************************************
 * function name: ARIBCC_OSD_SetTextColor(...)
 * description:  set the text color
 * arguments:    CCType: 0 sd 1 cc
 *               place: 0:fore-color 1:fore_mid_color  2: back-color 3:back-mid_color
 *              red,green,blue: 0,85,170,255       alpha: 0,128,255
 *
 **************************************************************************************************/
void ARIBCC_OSD_SetTextColor(HI_S32 CCType, HI_U8 place, HI_U8 red, HI_U8 green,HI_U8 blue, HI_U8 alpha)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);
    HI_UNF_CC_COLOR_S *pstColor = NULL;

    if( pstText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    OSD_TRACE("\tSetTextColor,type:%d, place:%d,red:%d,g:%d,b:%d,alpha:%d\n",CCType,place,
                red,green,blue,alpha);
    switch(place)
    {
        case FG_COLOR_PLACE:
            pstColor = &(pstText->stFgColor);
            break;
        case FG_MID_COLOR_PLACE:
            pstColor = &(pstText->stFgMidColor);
            break;
        case BG_COLOR_PLACE:
            pstColor = &(pstText->stBgColor);
            break;
        case BG_MID_COLOR_PLACE:
            pstColor = &(pstText->stBgMidColor);
            break;
        default:
            break;
    }

    if( pstColor != NULL )
    {
        pstColor->u8Alpha = alpha;
        pstColor->u8Blue = blue;
        pstColor->u8Green = green;
        pstColor->u8Red = red;
    }
    pstText->u8ColorPattern = ARIBCC_COLORPATTERN_NORMAL_POL;
    return;
}


/******************************************************************************************
 * function name: ARIBCC_OSD_SetSize(...)
 * description:  set the text scale(magnify or shrink)
 * arguments:    CCType: 0 sd, 1 cc
 *                     size: 0-standard ,1-1/4size, 2-1/2size ,3-horizontal double,4- vertical double
 *                     5-double in both direction
 ******************************************************************************************/
void ARIBCC_OSD_SetSize(HI_S32 CCType, HI_S32 size)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSet size,type:%d,size:%d\n",CCType,size);
    pText->u8FontScale = size;

    return;
}

/********************************************************************************
 * function name: ARIBCC_OSD_SetFlash(...)
 * description:  text flash command   flash control
 * arguments:    CCType: 0 sd 1 cc
 *                     request: 0-stop  1- start
 ********************************************************************************/
void ARIBCC_OSD_SetFlash(HI_S32 CCType, HI_S32 request)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    pText->u8IsFlash = request;

    return;
}


/*********************************************************************************************
* function name: ARIBCC_OSD_SetPatternPolarity(...)
* description:  POL,system color polarity pattern set.  Pattern Polarity Controls
* arguments:    CCType: 0 sd 1 cc
*                      ColorPattern: 0-Normal polarity
*                                          1-Reverse POL1, invert fg and bg color, fg_mid and bg_mid color
*                                          2-Reverse POL2 (not used)
*********************************************************************************************/
void ARIBCC_OSD_SetPatternPolarity(HI_S32 CCType, HI_S32 ColorPattern)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    if( ColorPattern == ARIBCC_COLORPATTERN_REVERSE_POL2 )
    {
        return;
    }

    if( ColorPattern != pText->u8ColorPattern )
    {
        _AribCC_SwapColor(&pText->stBgColor,&pText->stFgColor);
        _AribCC_SwapColor(&pText->stBgMidColor,&pText->stFgMidColor);
    }

    pText->u8ColorPattern = ColorPattern;
    return;
}

/***************************************************************************************
* function name: ARIBCC_OSD_SetCharBox(...)
* description:  HLC,set character box
* arguments:    CCType: 0 sd 1 cc
*                     btm,rgt,top,lft 0-don't draw   1-draw border
****************************************************************************************/
void ARIBCC_OSD_SetCharBox(HI_S32 CCType, HI_S32 btm, HI_S32 rgt, HI_S32 top, HI_S32 lft)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSetCharBox,type:%d, btm:%d,rgt:%d,top:%d,lft:%d\n",CCType,btm,rgt,top,lft);

    pText->stTextBox.u8btm = btm;
    pText->stTextBox.u8left = lft;
    pText->stTextBox.u8right = rgt;
    pText->stTextBox.u8top = top;

    return;
}

/***************************************************************************************
 * function name: ARIBCC_OSD_SetRepeatNum(...)
 * description:  repeate character
 * arguments:    CCType: 0 sd 1 cc
 *                     number: repeate number.
 *  The repeate code RPC with one parameter P1(1byte) causes a displayable character or mosaic that
 *  immediately follows the code.Number 0 has a special meaning that repeat the char to the end of the line
 ****************************************************************************************/
void ARIBCC_OSD_SetRepeatNum(HI_S32 CCType, HI_S32 number)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("type:%d, RepeateNum:%d\n",CCType,number);
    pText->s32RepeateNum = number;
    return;
}

/****************************************************************************************
 * function name: ARIBCC_OSD_Underline(...)
 * description:  set text underline attributes
 * arguments:    CCType: 0 sd 1 cc
 *                     flag: 0-no underline, 1-underline.
 *****************************************************************************************/
void ARIBCC_OSD_Underline(HI_S32 CCType, HI_S32 flag)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("type:%d, flag:%d\n",CCType,flag);
    pText->u8IsUnderline = flag;
    return;
}


/*****************************************************************************************
* function name: ARIBCC_OSD_SetWritingFormat(...)
* description:  set text writing format
* arguments:    CCType: 0 sd 1 cc
*                      format: writing format.
******************************************************************************************/
void ARIBCC_OSD_SetWritingFormat(HI_S32 CCType, HI_U8 format)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_S32 width=960,height=540;

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSWF,type:%d, format:%d\n",CCType,format);

    switch(format)
    {
        case PRINTH_960X540:
            height = 540;
            width = 960;
            pText->stFormat.stPrtDirect = CCARIB_PRINT_HORIZONTAL;
            break;
        case PRINTV_960X540:
            height = 540;
            width = 960;
            pText->stFormat.stPrtDirect = CCARIB_PRINT_VERTICAL;
            break;
        case PRINTH_720X480:
            height = 480;
            width = 720;
            pText->stFormat.stPrtDirect = CCARIB_PRINT_HORIZONTAL;
            break;
        case PRINTV_720X480:
            height = 480;
            width = 720;
            pText->stFormat.stPrtDirect = CCARIB_PRINT_VERTICAL;
            break;
        default:
            //break;
            return;
    }

    gLatestCCType = (char)CCType;
    if( pText->stFormat.res_h == height && pText->stFormat.res_w == width )
    {
        //return;
    }
    pText->stFormat.res_h = height;
    pText->stFormat.res_w = width;
    _AribCC_SetDefaultCursor(CCType);

    ARIBCC_GRAPHIC_SetResolution((ARIBCC_WINDOWFORMAT_E)format);
    ARIBCC_GRAPHIC_CreateScreen(CCType,pText->stFormat.res_w,pText->stFormat.res_h);

    return;
}

/*************************************************************************************
* function name: ARIBCC_OSD_SetWinColor(...)
* description:  set window color
* arguments:    CCType: 0 sd 1 cc
*
**************************************************************************************/
void ARIBCC_OSD_SetWinColor(HI_S32 CCType, HI_U8 red, HI_U8 green,HI_U8 blue, HI_U8 alpha)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    OSD_TRACE("\tRCS,type:%d, window color r:%d,g:%d,b:%d,a:%d\n",CCType,red,green,blue,alpha);
    if( (pText->stWinColor.u8Alpha != alpha) || (pText->stWinColor.u8Blue != blue)
      ||(pText->stWinColor.u8Green != green) || (pText->stWinColor.u8Red != red) )
    {
        pText->stWinColor.u8Alpha = alpha;
        pText->stWinColor.u8Blue = blue;
        pText->stWinColor.u8Green = green;
        pText->stWinColor.u8Red = red;
        ARIBCC_GRAPHIC_FillWindow(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16y,
                   pText->stWinAttr.u16width,pText->stWinAttr.u16height,pText->stWinColor);
    }

    return;
}


/***************************************************************************************
* function name: ARIBCC_OSD_SetWindowSize(...)
* description:  set window size
* arguments:    CCType: 0 sd 1 cc
*
****************************************************************************************/
void ARIBCC_OSD_SetWindowSize(HI_S32 CCType, HI_S32 width, HI_S32 height)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    if( width <= 0 || (pText->stWinAttr.u16x + width) > pText->stFormat.res_w )
    {
        HI_INFO_CC("window width=%d overflow, screen width:%d\n",width,pText->stFormat.res_w);
        width = pText->stFormat.res_w - pText->stWinAttr.u16x;
    }
    if( height <= 0 || (pText->stWinAttr.u16y + height) > pText->stFormat.res_h )
    {
        HI_INFO_CC("window height=%d overflow, screen height:%d\n",height,pText->stFormat.res_h);
        height = pText->stFormat.res_h - pText->stWinAttr.u16y;
    }

    OSD_TRACE("\tSetWindowSize,width:%d,height:%d\n",width,height);
    if( (pText->stWinAttr.u16width != width) || (pText->stWinAttr.u16height != height) )
    {
        ARIBCC_GRAPHIC_NewWindow(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16y,width,height);
        /*first time to fill the window with transparent color*/
        //ARIBCC_GRAPHIC_FillWindow(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16y,width,height,pText->stWinColor);//dong
        pText->stWinAttr.u16width = width;
        pText->stWinAttr.u16height = height;
    }

    return;
}

/*****************************************************************************************
* function name: ARIBCC_OSD_SetWindowPos(...)
* description:  set window position
* arguments:    CCType: 0 sd 1 cc
*
******************************************************************************************/
void ARIBCC_OSD_SetWindowPos(HI_S32 CCType, HI_S32 x, HI_S32 y)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSetWindowPos,type:%d, window x:%d,y:%d\n",CCType,x,y);

    if( x < 0 || x >= pText->stFormat.res_w )
    {
        HI_WARN_CC("Invalid coordinate of x=%d\n",x);
        x = 0;
    }
    if( y < 0 || y >= pText->stFormat.res_h )
    {
        HI_WARN_CC("Invalide coordinate of y=%d\n",y);
        y = 0;
    }

    if( (pText->stWinAttr.u16x != x) || (pText->stWinAttr.u16y != y) )
    {
        pText->u8IsWinUpdate = 1;
        pText->stWinAttr.u16x = x;
        pText->stWinAttr.u16y = y;
    }

    return;
}

/****************************************************************************************
* function name: ARIBCC_OSD_SetCharacterSize(...)
* description:  set font size in dot unit( normal character size)
* arguments:    CCType: 0 sd 1 cc
*                     size: (dot unit) 16,20,24,30,36
*               the size information is from the stream, user cann't set the font size
*****************************************************************************************/
void ARIBCC_OSD_SetCharacterSize(HI_S32 CCType, HI_S32 size)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSSM,type:%d, font size:%d\n",CCType,size);

    /*the font width and height are identical*/
    pText->u8FontHeight = size;
    pText->u8FontWidth = size;

    /*does it need to calculate the font width*/
    ARIBCC_GRAPHIC_SetFontSize(CCType,size);

    return;
}

/*************************************************************************************
 * function name: ARIBCC_OSD_SetHorizontalSpacing(...)
 * description:  set character interval (set horizontal spacing
 *               by this operation, active point movement is made by the unit of length of frame
 *              design adding character spacing
 * arguments:    CCType: 0 sd 1 cc
 *                     interval: unit(pixel) value(0-12)
 *************************************************************************************/
void ARIBCC_OSD_SetHorizontalSpacing(HI_S32 CCType, HI_S32 interval)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSHS,type:%d, interval:%d\n",CCType,interval);

    pText->u8CharInterval = interval;

    return;
}

/*****************************************************************************************
 * function name: ARIBCC_OSD_SetVerticalSpacing (...)
 * description:  set line interval  set vertical spacing
 *              By this operation, the line movement transition's unit becomes the length of the
 *              of the space between the lines added to the frame design.
 * arguments:    CCType: 0 sd 1 cc
 *                      interval: unit(pixel) value(0-24)
 *****************************************************************************************/
void ARIBCC_OSD_SetVerticalSpacing (HI_S32 CCType, HI_S32 interval)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("\tSVS,type:%d, interval:%d\n",CCType,interval);

    pText->u8LineInterval = interval;

    return;
}

/********************************************************************************************
 * function name: ARIBCC_OSD_SetOrnament(...)
 * description:  ,ORN,set character ornament (border type)
 * arguments:    CCType: 0 sd 1 cc
 *                      number: 0: normal font    1:border font(no shadow and no outline)
 *                      red,green,blue,alpha:  edge color
 ********************************************************************************************/
void ARIBCC_OSD_SetOrnament(HI_S32 CCType, HI_S32 number, HI_U8 red, HI_U8 green, HI_U8 blue, HI_U8 alpha)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    OSD_TRACE("type:%d, ornament:%d\n",CCType,number);

    pText->u8EdgeType = number;
    pText->stEdgeColor.u8Red = red;
    pText->stEdgeColor.u8Green = green;
    pText->stEdgeColor.u8Blue = blue;
    pText->stEdgeColor.u8Alpha = alpha;

    ARIBCC_GRAPHIC_SetEdgeColor(CCType,pText->stEdgeColor);

    return;
}

/*********************************************************************************************
* function name: ARIBCC_OSD_SetScrollInfo(...)
* description:  SCR,set scroll type and speed
* arguments:      CCType: 0 sd 1 cc
*                       type: 0-no scroll, 1-2:line scroll  3-4: whole display scroll
*                       speed: pixel/second
***********************************************************************************************/
void ARIBCC_OSD_SetScrollInfo(HI_S32 CCType, HI_S32 type, HI_S32 speed)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    OSD_TRACE("SetScrollInfo--type:%d, ARIBCC_SCROLLTYPE_E:%d, speed:%d\n",CCType,type,speed);
    pText->u8ScrollSpeed = speed;
    pText->u8ScrollType = type;
    if( type == ARIBCC_LINESCROLL_FIX )
    {
        /*reset scroll count and set the cursor to the last row*/
        pText->u8ScrolledRows = 0;
        pText->stCursor.u16Y = pText->stWinAttr.u16height;
    }

    pText->stLastCursor = pText->stCursor;

    return;
}

/*************************************************************************************
 * function name: ARIBCC_OSD_Pra(...)
 * description: play build-in sound
 * arguments:    number: sound id
 **************************************************************************************/
void ARIBCC_OSD_Pra(HI_S32 number)
{
    //Not implement
    //todu
    return;
}

/************************************************************************************
* function name: ARIBCC_OSD_DrawText(...)
* description: receive closed caption string
* arguments:      CCType: 0:sd, 1:cc
*                       pu8text: pointer to string
*                       u32length: string length
*************************************************************************************/
void ARIBCC_OSD_DrawText(HI_S32 CCType, const HI_U8 *pu8text, HI_U32 u32length)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);
    HI_U8 u8CodeArea,u8CharTable,u8CharSet;
    HI_U32 i;
    HI_U16 u16MapUnicode = 0;
    HI_U16 *pu16buf, u16bufLen;
    HI_U8 u8HasNonspaceCode = 0,u8IsNonSpaceCode=0;
    HI_U16 u16NonespaceCode = 0;

    gUnicodeFirstByte = 0;
    gUnicodeDrcs0Value = 0;

    if( gIsCCShowEnable == 0 )
    {
        return;
    }
    if( pstText == NULL || pu8text == NULL || u32length == 0 )
    {
        HI_ERR_CC("\nGet Arib pu8text return NULL!\n");
        return;
    }

    pstText->stCaption.u16Length = 0;
    gIsTextBitmapDrawn = 1;


    /*first to check the character locate which area(GR or GL)*/
    for( i = 0; i < u32length; i++ )
    {
        if( ((HI_U8*)pu8text)[i] < 0x20 )
        {
            return; //continue;
        }

        /*refer to table code to check the table code(G0,1,2,3)*/
        u8CodeArea = (((HI_U8*)pu8text)[i]&0x80)?ARIBCC_GR:ARIBCC_GL;
        u8CharTable = pstText->stCaption.aenTable[u8CodeArea];
        if(u8CharTable == ARIBCC_CODESET_NONE)
        {
            continue;
        }
        /*refer to the character set code to check the char set(Kanji,Hiragana,...*/
        u8CharSet = pstText->stCaption.aenCharSet[u8CharTable];

        /*then map the char to unicode and store it*/
        u16MapUnicode = _AribCC_MapUnicode(pu8text[i]&0x7f, u8CharSet,pstText->stFormat.stPrtDirect,&u8IsNonSpaceCode);
        if( u8IsNonSpaceCode == 1 )
        {
            u8HasNonspaceCode = 1;
            OSD_TRACE("here is a non space code u8CharSet=%d,pu8text=0x%x\n",u8CharSet,pu8text[i]);
        }

        if( u16MapUnicode == 0 )
        {
            continue;
        }

        /*if there is DRCS character, we should separate the DRCS and normal char*/
        if( u8CharSet >= ARIBCC_CHARSET_DRCS0 && u8CharSet <= ARIBCC_CHARSET_DRCS15 )
        {
            /*if there is any normal character before this DRCS char, we should draw it first*/
            if(( pstText->stCaption.u16Length > 0 ) && (pstText->stCaption.u16Length <= 256))
            {
                _AribCC_DrawCaptionToWindow(CCType,pstText->stCaption.au16Text,pstText->stCaption.u16Length,0);
                pstText->stCaption.u16Length = 0;
            }
        }
        pstText->u8CurCharSet = u8CharSet;

        if( pstText->s32RepeateNum >= 0 )
        {
            HI_S32 j;
            if( u8IsNonSpaceCode == 1 )
            {
                /*we need wait for the next character to compose one character and repeate*/
                u16NonespaceCode = u16MapUnicode;
                continue;
            }
            /*RPC 04/0 has a special meaning that repeat to the end of line*/
            if( pstText->s32RepeateNum == 0 )
            {
                HI_S32 char_width = 0, char_height = 0,empty_cols = 0;
                _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL, &char_width,&char_height);
                if( char_width == 0 || char_height == 0 )
                {
                    HI_WARN_CC("character size is zero:width=%d,height=%d\n",char_width,char_height);
                    char_width = pstText->u8FontWidth;
                    char_height = pstText->u8FontHeight;
                }
                empty_cols = (pstText->stWinAttr.u16width - pstText->stCursor.u16X)/char_width;
                if( pstText->stFormat.stPrtDirect == CCARIB_PRINT_VERTICAL )
                {
                    empty_cols = (pstText->stWinAttr.u16height - pstText->stCursor.u16Y)/char_height;
                }
                pstText->s32RepeateNum = empty_cols;
            }

            /*there is a repeate command before current cmd*/
            OSD_TRACE("\n\nrepeate_num = %d\n\n",pstText->s32RepeateNum);
            for(j=0; j < pstText->s32RepeateNum; j++)
            {
                /*do not overflow the buffer*/
                if( pstText->stCaption.u16Length + 2 < 256 )
                {
                    if( u16NonespaceCode != 0 )
                    {
                        pstText->stCaption.au16Text[pstText->stCaption.u16Length++] = u16NonespaceCode;
                    }
                    pstText->stCaption.au16Text[pstText->stCaption.u16Length++] = u16MapUnicode;
                }
            }
                pstText->s32RepeateNum = -1;
        }
        else
        {
            if( pstText->stCaption.u16Length + 1 < 256 )
            {
                pstText->stCaption.au16Text[pstText->stCaption.u16Length++] = u16MapUnicode;
            }

        }

        /*if there is DRCS character, we should draw it here*/
        if( pstText->u8CurCharSet >= ARIBCC_CHARSET_DRCS0 && pstText->u8CurCharSet <= ARIBCC_CHARSET_DRCS15 )
        {
            if(( pstText->stCaption.u16Length > 0 ) && (pstText->stCaption.u16Length <= 256))
            {
                _AribCC_DrawCaptionToWindow(CCType,pstText->stCaption.au16Text,pstText->stCaption.u16Length,0);
                pstText->stCaption.u16Length = 0;
            }
        }
    }

    /*if there is non-space character. We split the caption at the end of the non-space char*/
    pu16buf = pstText->stCaption.au16Text;
    u16bufLen = pstText->stCaption.u16Length;
    if( u8HasNonspaceCode == 1 )
    {
        u16bufLen = 0;
        for( i = 0; i < pstText->stCaption.u16Length; i++ )
        {
            u16bufLen++;
            if( _AribCC_IsNonspaceCode(pstText->stCaption.au16Text[i]) )
            {
                _AribCC_DrawCaptionToWindow(CCType,pu16buf,u16bufLen,u8HasNonspaceCode);
                pu16buf = &pstText->stCaption.au16Text[i+1];
                u16bufLen = 0;
            }
        }
    }
    if( u16bufLen > 0 )
    {
        _AribCC_DrawCaptionToWindow(CCType,pu16buf,u16bufLen,u8HasNonspaceCode);
    }
    pstText->u8IsTextDraw = 1;

    return;
}


/**************************************************************************************
* function name: ARIBCC_OSD_DrawBitmap(...)
* description:  draw png picture, actually it just store the picture in the buffer
* arguments:    CCType: 0:sd, 1:cc
*                      pu8bitmap: pointer to png structure
*               bitmap definition please refer to b24 Chapter 6.2
***************************************************************************************/
void ARIBCC_OSD_DrawBitmap(HI_S32 CCType, HI_U8 *pu8bitmap)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    HI_U8 flash_color_num;

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }

    if( pu8bitmap == NULL )
    {
        HI_ERR_CC("pu8bitmap is NULL!\n");
        return;
    }

    gIsTextBitmapDrawn = 1;

    /*store the bitmap to pText*/
    pText->stBitmap.u16X = (pu8bitmap[0]<<8)|pu8bitmap[1];
    pText->stBitmap.u16Y = (pu8bitmap[2]<<8)|pu8bitmap[3];

    flash_color_num = pu8bitmap[4];

    pText->stBitmap.pu8PngData = pu8bitmap + 5 + flash_color_num;
    pText->stBitmap.u8HasData = 1;

    /*get the png data from pText then decode and display it*/
    if( pText->stBitmap.u8HasData == 1 )
    {
        //put_png_data(pText->stBitmap.pPngData, 0x10000);
        //put_png_flash_color(pu8bitmap+5,flash_color_num);
        _AribCC_SetBitmapFlashColor(pu8bitmap+5,flash_color_num,pText);
        //decode_draw_png(pText->stBitmap.x, pText->stBitmap.y);
        pText->stBitmap.u8HasData = 0;
    }
    pText->u8IsTextDraw = 1;
    return;
}


/*************************************************************************************
 * function name: ARIBCC_OSD_Show(...)
 * description:  show(draw) the text or png picture
 * arguments:    CCType: 0:sd, 1:cc
 *************************************************************************************/
void ARIBCC_OSD_Show(HI_S32 CCType)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    OSD_TRACE("type:%d\n",CCType);
    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    /*if the cc and sd are not permitted to be shown, just return*/
    if( gIsCCShowEnable == 0 )
    {
        return;
    }
    if( gIsTextBitmapDrawn == 0 )
    {
        return;
    }
    gIsTextBitmapDrawn = 0;

    OSD_TRACE("repeate type:%d\n",CCType);

    if( pText->u8IsWinUpdate == 1 )
    {
        /*window position changed*/
        HI_S32 other_cctype = ARIBCC_TYPE_CC;
        ARIBCC_TEXTATTR_S *pOtherText = NULL;
        OSD_TRACE("***************window update!********\n");
        ARIBCC_GRAPHIC_FillRect(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16y,
                     pText->stWinAttr.u16width,pText->stWinAttr.u16height,pText->stClearColor);
#ifdef ARIBCC_SD_SHARE_ONEPLANE
        other_cctype = (CCType==ARIBCC_TYPE_CC)?ARIBCC_TYPE_SD:ARIBCC_TYPE_CC;
        pOtherText = _AribCC_GetAribText(other_cctype);
        if( pOtherText->u8IsTextDraw )
        {
            ARIBCC_GRAPHIC_ShowWindow(other_cctype,pOtherText->stWinAttr.u16x,pOtherText->stWinAttr.u16y,
                          pOtherText->stWinAttr.u16width,
                          pOtherText->stWinAttr.u16height,0,0,0);
        }
        pText->u8IsWinUpdate = 0;
#endif
    }

    /*first move the to be scrolled text to the end of the window*/
    //ARIBCC_GRAPHIC_ShowWindow(CCType,pText->win_pos.x,pText->win_pos.y,pText->win_pos.width,pText->win_pos.height);

    if( pText->u8WinScrollMode == 1 || pText->u8ScrollType )
    {
        OSD_TRACE("pText->u8WinScrollMode=%d,pText->u8ScrollType=%d\n",
                    pText->u8WinScrollMode,pText->u8ScrollType);
        if( pText->u8WinScrollMode == 1 && pText->u8ScrollType == ARIBCC_NO_SCROLL )
        {
                pText->u8ScrollType = ARIBCC_LINESCROLL_ROLLOUT;
        }
        
        /*do the scroll action*/
        if( pText->u8ScrollType >= ARIBCC_LINESCROLL_FIX )
        {
            OSD_TRACE("Enter vertical scroll text!\n");
            if( pText->u8ScrollType == ARIBCC_LINESCROLL_FIX )
            {
                HI_S32 char_width = 0,char_height = 0;
                _AribCC_CalcCharSize(CCType,FONTSIZE_WITH_INTERVAL,&char_width,&char_height);
                if (0 == char_height)
                {
                    OSD_TRACE("char_height is 0!\n");
                    return;
                }
                pText->stCursor.u16MaxRow = pText->stWinAttr.u16height/char_height;
                if( pText->u8ScrolledRows < pText->stCursor.u16MaxRow )
                {
                    /*reach the maximum scroll times. stop scroll it*/
                    _AribCC_ScrollTextVertical(CCType,pText->u8ScrollType,pText->u8ScrollSpeed);
                    pText->u8ScrolledRows++;
                }
            }
            else
            {
                _AribCC_ScrollTextVertical(CCType,pText->u8ScrollType,pText->u8ScrollSpeed);
            }
        }
        else if( pText->u8ScrollType >= ARIBCC_CHARSCROLL_FIX )
        {
            OSD_TRACE("Enter horizontal scroll text!\n");
            _AribCC_ScrollTextHorizontal(CCType,pText->u8ScrollType,pText->u8ScrollSpeed);
        }
        /*here we remember the current cursor as the next scroll start point*/
        pText->stLastCursor = pText->stCursor;
    }
    else
    {
        HI_S32 is_colorkey_enable = 0;
#ifdef ARIBCC_SD_SHARE_ONEPLANE
        if(CCType == ARIBCC_TYPE_SD)
        {
            ARIBCC_TEXTATTR_S *pCCText = _AribCC_GetAribText(ARIBCC_TYPE_CC);
            if( pCCText->u8IsTextDraw && _AribCC_IsCCSDOverlap())
            {
                is_colorkey_enable = 1;
            }
        }
#endif
        /*copy the window buffer to osd*/
        _AribCC_CheckAndReviseWinPos(CCType);
        ARIBCC_GRAPHIC_ShowWindow(CCType,pText->stWinAttr.u16x,pText->stWinAttr.u16y,pText->stWinAttr.u16width,
                      pText->stWinAttr.u16height,0,0,is_colorkey_enable);
#ifdef ARIBCC_SD_SHARE_ONEPLANE
        if( CCType == ARIBCC_TYPE_CC )
        {
            ARIBCC_TEXTATTR_S *pSDText = _AribCC_GetAribText(ARIBCC_TYPE_SD);
            if( pSDText->u8IsTextDraw && _AribCC_IsCCSDOverlap())
            {
                _AribCC_CheckAndReviseWinPos(ARIBCC_TYPE_SD);
                ARIBCC_GRAPHIC_ShowWindow(ARIBCC_TYPE_SD,pSDText->stWinAttr.u16x,pSDText->stWinAttr.u16y,
                              pSDText->stWinAttr.u16width,
                              pSDText->stWinAttr.u16height,0,0,1);
            }
        }
#endif
    }

    return;
}


/***********************************************************************************
 * Function name: ARIBCC_OSD_DrcsClear(...)
 * Description:  clear the drcs character glyph.
 * Arguments:   CCType: 0:SD, 1:CC
 *                     ClearType: 0-both DRCS from CC Management and DRCS from CC Data
 *                                     1-DRCS from CC Management  (default DRCS)
 *                                     2-DRCS from CC Data        (temperary DRCS)
 * database for DRCS shall have 188 characters, each one shall be a bitmap.
 * max size information is 16k Bytes for cc and SD
 ***********************************************************************************/
void ARIBCC_OSD_DrcsClear(HI_S32 CCType, char ClearType)
{
    if( ClearType == 1 || ClearType == 0 )
    {
        _AribCC_ClearDrcs(CCType,DEFAULT_DRCS);
    }

    if( ClearType == 2 || ClearType == 0 )
    {
        _AribCC_ClearDrcs(CCType,TEMPORARY_DRCS);
    }

    return;
}

/***********************************************************************************
* Function name: ARIBCC_OSD_DrcsEntry(...)
* Description:  Set the drcs character glyph.
* Arguments:   CCType: 0:SD, 1:CC
*                     EntryType: 1- DRCS from CC Management  (default DRCS)
*                                      2-DRCS from CC Data        (temperary DRCS)
*                     type: drcs code set,1-one byte drcs
*                                                  2-two bytes drcs
* if the type is 1, the code1 is the drcs character set the value range is 04/1(DRCS-1) - 04/F(DRCS-F)
* if the type is 2, the drcs code set DRCS-0.both the two codes are the code
* database for DRCS shall have 188 characters, each one shall be a bitmap.
* max size information is 16k Bytes for cc and SD
***********************************************************************************/
void ARIBCC_OSD_DrcsEntry(HI_S32 CCType, HI_U8 EntryType, HI_S32 type, HI_U8 code1, HI_U8 code2,HI_U8 depth,
                         HI_U8 width, HI_U8 height, HI_U8 *pbitmap)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);
    ARIBCC_DRCSEntry_S *pDrcsInsert = NULL;
    ARIBCC_DRCSEntry_S *pLastDrcs;
    HI_U8 code_set,is_update = 0;
    //HI_U8 align_width;
    HI_S32 byte_num;

    if( pText == NULL )
    {
        HI_ERR_CC("\nGet Arib text return NULL!\n");
        return;
    }
    if( depth == 0 || width == 0 || height == 0 || ((type != 1) && (type != 2)))
    {
        HI_ERR_CC("depth=%d, width=%d,height=%d,type=%d\n",depth,width,height,type);
        return;
    }
    OSD_TRACE("CCType:%d,EntryType:%d,type:%d,code1:0x%02x,code2:0x%02x,depth:%d,width:%d,height:%d\n",
                CCType,EntryType,type,code1,code2,depth,width,height);

    /*to calculate the drcs character set*/
    if( type == DRCS_2BYTE )
    {
        code_set = ARIBCC_CHARSET_DRCS0;
    }
    else
    {
        if(code1 < 0x41)
        {
            HI_ERR_CC("\ncode1: 0x%x\n", code1);
            return;
        }
        code_set = code1 - 0x41 + ARIBCC_CHARSET_DRCS1;
    }
    code_set -= ARIBCC_CHARSET_DRCS0;

    EntryType--;
    /*first to check whethere the code is in the list*/
    pDrcsInsert = _AribCC_SearchDrcs(CCType,EntryType,code_set,code1,code2,width,height);
    if( pDrcsInsert != NULL )
    {
        /*this drcs code is alread created,we just update it*/
        is_update = 1;
    }
    else
    {
        pDrcsInsert = (ARIBCC_DRCSEntry_S*)malloc(sizeof(ARIBCC_DRCSEntry_S));
        if( pDrcsInsert == NULL )
        {
            HI_ERR_CC("malloc for drcs failed!\n");
            return;
        }
        pDrcsInsert->pu8Bitmap = NULL;
        pDrcsInsert->pvNext = NULL;
        is_update = 0;
    }

    byte_num = (width*depth*height+7)/8;      /*byte alignment*/
    if( pDrcsInsert->pu8Bitmap == NULL )
    {
        pDrcsInsert->pu8Bitmap = (HI_U8*)malloc(byte_num);
        if( pDrcsInsert->pu8Bitmap == NULL )
        {
            HI_ERR_CC("malloc for pbitmap failed!\n");
            free(pDrcsInsert);
            pDrcsInsert = NULL;
            return;
        }
    }

    /*store the drcs into list*/
    memcpy(pDrcsInsert->pu8Bitmap,pbitmap,byte_num);

    pDrcsInsert->u8Code1 = code1;
    pDrcsInsert->u8Code2 = code2;
    pDrcsInsert->u8Depth = depth;
    pDrcsInsert->u8Height = height;
    pDrcsInsert->u8Width = width;
    /*if update, we just update the data. if newly added. we should insert it into the list*/
    if(is_update == 0)
    {
        if((HI_U8)EntryType >= MAX_DRCS_TYPE)
        {
            free(pDrcsInsert->pu8Bitmap);
            pDrcsInsert->pu8Bitmap = NULL;
            free(pDrcsInsert);
            pDrcsInsert = NULL;
            return;
        }

        pDrcsInsert->pvNext = NULL;
        if( pText->pastDrcs[(HI_U8)EntryType][code_set] == NULL )
        {
            pText->pastDrcs[(HI_U8)EntryType][code_set] = pDrcsInsert;
        }
        else
        {
            pLastDrcs = pText->pastDrcs[(HI_U8)EntryType][code_set];

            while( pLastDrcs->pvNext != NULL )
            {
                pLastDrcs = pLastDrcs->pvNext;
            }

            pLastDrcs->pvNext = pDrcsInsert;
        }
    }
    return;
}

/***********************************************************************************
* Description : set the characters code set
* Arguments: pu8Designate: Code set allocation. Pass the 4 byte char array pointer
*                               byte1: pointer for G0 table
*                               byte2: pointer for G1 table
*                               byte3: pointer for G2 table
*                               byte4: pointer for G3 table
*                  pu8Invoke: Table allocation  pass the 2 byte char array pointer
*                               byte1: Pointer for GR
*                               byte2: Pointer for GL
***********************************************************************************/
void ARIBCC_OSD_SetCharacterCode(HI_S32 CCType, HI_U8 *pu8Designate, HI_U8 *pu8Invoke)
{
    ARIBCC_TEXTATTR_S *pstText = _AribCC_GetAribText(CCType);
    HI_S32 i;

    if(pstText == NULL)
    {
        return;
    }

    for( i = 0; i < 2; i++ )
    {
        pstText->stCaption.aenTable[i] = (ARIBCC_CODE_TABLE_E)pu8Invoke[i];
    }

    for( i = 0; i < 4; i++ )
    {
        pstText->stCaption.aenCharSet[i] = (ARIBCC_CHARACTER_SET_E)pu8Designate[i];
    }

    return;
}

/***********************************************************************************
 * Description :decode specified code
 * Arguments: CCType: 0:sd, 1:cc
 *                   request:0: SP
 *                               1: DEL
 ***********************************************************************************/
void ARIBCC_OSD_SpecialCode(HI_S32 CCType, HI_S32 request)
{
#if 0
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(CCType);

    /*with Brazil stream, there are many many SP/DEL characters.and if we enable this
    the cc behavior will be very strange. especially there is stream record_hd_cc_wrong_position.ts
    if we enable this function the last line will be go to first line. Since there is no special spec
    for Brazil cc. so we disable this function for Brazil cc*/
    return;

    OSD_TRACE("CCType=%d,request=%d\n",CCType,request);

    if(pText == NULL)
    {
        return;
    }

    if( gIsCCShowEnable == 0 )
    {
         return;
    }
    gIsTextBitmapDrawn = 1;
    //pText->caption.length = 0;
    if( request == 0 )
    {
        pText->caption.text[pText->caption.length++] = 0x20;//0x3000;//0x20;
    }
    else if( request == 1 )
    {
        pText->caption.text[pText->caption.length++] = 0x25a0;//0xe521;//0x7f;
    }
    //_AribCC_DrawCaptionToWindow(CCType,pText->caption.text,pText->caption.length,0);
#endif

    return;
}


/***********************************************************************************
 * Function name: ARIBCC_OSD_SetRollupMode(HI_S32 mode)
 * Description:   Set the default rollup mode of the window
 *                      0: the window isn't rollup. normal mode
 *                      1: the window default attributes is rollup. and type set to 4
 *
 *                This function is just only for CC. SD has no scroll
 ***********************************************************************************/
void ARIBCC_OSD_SetRollupMode(HI_S32 mode)
{
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(1);

    OSD_TRACE("\tSetRollupMode,mode=%d\n",mode);
    pText->u8WinScrollMode = mode;
    if( mode == 1 && pText->u8ScrollType == 0)
    {
        pText->u8ScrollType = 4;
    }
    pText->stLastCursor = pText->stCursor;
    return;
}


/****************************************************************
* function name: ARIBCC_OSD_EnableShow
* description: to control whether the cc can be shown or not.
* arguments:enable
*                           0   the cc can't be shown.
*                           1   the cc can be shown now.
*****************************************************************/
void ARIBCC_OSD_EnableShow(HI_S32 enable)
{
    ARIBCC_TEXTATTR_S *pSD = _AribCC_GetAribText(ARIBCC_TYPE_SD);
    ARIBCC_TEXTATTR_S *pText = _AribCC_GetAribText(ARIBCC_TYPE_CC);

    if( enable == 0 )
    {
        /*CC and SD*/
        gIsCCShowEnable = enable;
        /*clear the screen first*/
        ARIBCC_GRAPHIC_FillRect(ARIBCC_TYPE_CC,pText->stWinAttr.u16x,pText->stWinAttr.u16y,pText->stWinAttr.u16width,
                    pText->stWinAttr.u16height,pText->stWinColor);
        ARIBCC_GRAPHIC_FillRect(ARIBCC_TYPE_SD,pSD->stWinAttr.u16x,pSD->stWinAttr.u16y,pSD->stWinAttr.u16width,
                    pSD->stWinAttr.u16height,pSD->stWinColor);
        ARIBCC_GRAPHIC_DestroyScreen(ARIBCC_TYPE_SD);
        ARIBCC_GRAPHIC_DestroyScreen(ARIBCC_TYPE_CC);
    }
    else if( enable == 1 )
    {
        /*CC and SD*/
        gIsCCShowEnable = enable;
        if( (pSD->stFormat.res_w != pText->stFormat.res_w) || (pSD->stFormat.res_h != pText->stFormat.res_h) )
        {
            if( gLatestCCType == ARIBCC_TYPE_CC )
            {
                ARIBCC_GRAPHIC_CreateScreen(ARIBCC_TYPE_CC,pText->stFormat.res_w,pText->stFormat.res_h);
            }
            else if( gLatestCCType == ARIBCC_TYPE_SD )
            {
                ARIBCC_GRAPHIC_CreateScreen(ARIBCC_TYPE_SD,pSD->stFormat.res_w,pSD->stFormat.res_h);
            }
            return;
        }
        ARIBCC_GRAPHIC_CreateScreen(ARIBCC_TYPE_SD,pSD->stFormat.res_w,pSD->stFormat.res_h);
        ARIBCC_GRAPHIC_CreateScreen(ARIBCC_TYPE_CC,pText->stFormat.res_w,pText->stFormat.res_h);
    }
    return;
}

