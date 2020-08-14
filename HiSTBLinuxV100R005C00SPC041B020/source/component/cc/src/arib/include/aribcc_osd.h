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

#ifndef _ARIB_CC_OSD_H_
#define _ARIB_CC_OSD_H_

#include <pthread.h>
#include "hi_type.h"
#include "aribcc_def.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "c"
{
#endif

#define ARIBCC_SD_SHARE_ONEPLANE

#define DEFAULT_DRCS 0

#define TEMPORARY_DRCS 1

#define DRCS_2BYTE  2
#define DRCS_1BYTE  1

#define MAX_DRCS_TYPE 2
#define MAX_DRCS_SETS 16       /*the maximum DRCS character set DRCS-0 ... DRCS-15*/


#define FONTSIZE_NO_INTERVAL 0
#define FONTSIZE_WITH_INTERVAL 1

#define MAX_SCROLL_BUF_SIZE 64

typedef enum tagARIBCC_TYPE_E
{
    ARIBCC_TYPE_SD,
    ARIBCC_TYPE_CC,
} ARIBCC_TYPE_E;

typedef enum tagARIBCC_SETCURSOR_CMD_E
{
    SET_CURSOR_APB=1,       /*active position backward*/
    SET_CURSOR_APF,         /*active position forward*/
    SET_CURSOR_APD,         /*active position down*/
    SET_CURSOR_APU,         /*active position up*/
    SET_CURSOR_PAPF,        /*parameterized active position forward*/
    SET_CURSOR_APR,         /*active position return*/
    SET_CURSOR_APS,         /*active position set*/
    SET_CURSOR_ACPS         /*active coordinate position set*/
} ARIBCC_SETCURSOR_CMD_E;

typedef enum tagARIBCC_COLORPLACE_E
{
    FG_COLOR_PLACE,
    FG_MID_COLOR_PLACE,
    BG_COLOR_PLACE,
    BG_MID_COLOR_PLACE,
} ARIBCC_COLORPLACE_E;

typedef enum tagARIBCC_CHARSIZE_E
{
    CHARSIZE_NORMAL,
    CHARSIZE_HALF_ALL,
    CHARSIZE_HALF_SINGLE,
    CHARSIZE_DOUBLE_HEIGHT,
    CHARSIZE_DOUBLE_WIDTH,
    CHARSIZE_DOUBLE_ALL,
} ARIBCC_CHARSIZE_E;

typedef enum tagARIBCC_WINDOWFORMAT_E
{
    PRINTH_960X540,
    PRINTV_960X540,
    PRINTH_720X480,
    PRINTV_720X480,
} ARIBCC_WINDOWFORMAT_E;

typedef enum tagARIBCC_COLORPATTERN_E
{
    ARIBCC_COLORPATTERN_NORMAL_POL,
    ARIBCC_COLORPATTERN_REVERSE_POL1,
    ARIBCC_COLORPATTERN_REVERSE_POL2,     /*not used*/
} ARIBCC_COLORPATTERN_E;

typedef enum tagARIBCC_SCROLLTYPE_E
{
    ARIBCC_NO_SCROLL,                /*without scroll*/
    ARIBCC_CHARSCROLL_FIX,           /*one line scroll to character direction without roll out*/
    ARIBCC_CHARSCROLL_ROLLOUT,       /*one line scroll to character direction with roll out*/
    ARIBCC_LINESCROLL_FIX,           /*whole display scroll to line direction without roll out*/
    ARIBCC_LINESCROLL_ROLLOUT,       /*whole display scroll to line direction with roll out*/
} ARIBCC_SCROLLTYPE_E;

typedef enum tagARIBCC_PRTDIRECT_E
{
    CCARIB_PRINT_HORIZONTAL,
    CCARIB_PRINT_VERTICAL,
} ARIBCC_PRTDIRECT_E;

typedef enum tagARIBCC_CODE_AREA_E
{
    ARIBCC_GR,    /*GR area 8bits code*/
    ARIBCC_GL,    /*GL area 8bits code*/
} ARIBCC_CODE_AREA_E;


typedef enum tagARIBCC_CODE_TABLE_E
{
    ARIBCC_CODESET_G0,
    ARIBCC_CODESET_G1,
    ARIBCC_CODESET_G2,
    ARIBCC_CODESET_G3,
    ARIBCC_CODESET_NONE
} ARIBCC_CODE_TABLE_E;

typedef enum tagARIBCC_CHARACTER_SET_E
{
    ARIBCC_CHARSET_NONE,                   /* 0 None*/
    ARIBCC_CHARSET_KANJI,
    ARIBCC_CHARSET_ALPHANUMERIC,
    ARIBCC_CHARSET_HIRAGANA,
    ARIBCC_CHARSET_KATAKANA,
    ARIBCC_CHARSET_MOSAICA,
    ARIBCC_CHARSET_MOSAICB,
    ARIBCC_CHARSET_MOSAICC,
    ARIBCC_CHARSET_MOSAICD,
    ARIBCC_CHARSET_PALPHANUMERIC,
    ARIBCC_CHARSET_PHIRAGANA,             /*10 Proportional Hiragana*/
    ARIBCC_CHARSET_PKATAKANA,
    ARIBCC_CHARSET_JISX0201KATAKANA,
    ARIBCC_CHARSET_DRCS0,
    ARIBCC_CHARSET_DRCS1,
    ARIBCC_CHARSET_DRCS2,
    ARIBCC_CHARSET_DRCS3,
    ARIBCC_CHARSET_DRCS4,
    ARIBCC_CHARSET_DRCS5,
    ARIBCC_CHARSET_DRCS6,
    ARIBCC_CHARSET_DRCS7,                 /*0x14, DRCS-7*/
    ARIBCC_CHARSET_DRCS8,
    ARIBCC_CHARSET_DRCS9,
    ARIBCC_CHARSET_DRCS10,
    ARIBCC_CHARSET_DRCS11,
    ARIBCC_CHARSET_DRCS12,
    ARIBCC_CHARSET_DRCS13,
    ARIBCC_CHARSET_DRCS14,
    ARIBCC_CHARSET_DRCS15,
    ARIBCC_CHARSET_MACRO,
    ARIBCC_CHARSET_LATIN = 0x21,
    ARIBCC_CHARSET_SPECIAL,
} ARIBCC_CHARACTER_SET_E;

typedef struct tagARIBCC_CURSOR_S
{
    HI_U16 u16X;
    HI_U16 u16Y;
    HI_U16 u16Row;
    HI_U16 u16Column;
    HI_U16 u16MaxCol;
    HI_U16 u16MaxRow;
} ARIBCC_CURSOR_S;

typedef struct tagARIBCC_TEXTBOX_S
{
    HI_U8 u8btm;
    HI_U8 u8right;
    HI_U8 u8top;
    HI_U8 u8left;
    HI_UNF_CC_COLOR_S stColor;
} ARIBCC_TEXTBOX_S;

typedef struct tagARIBCC_WinAttr_S
{
    HI_U16 u16x;
    HI_U16 u16y;
    HI_U16 u16width;
    HI_U16 u16height;
} ARIBCC_WinAttr_S;

typedef struct tagARIBCC_WinFmt_S
{
    HI_U16 res_w;    /*resolution width*/
    HI_U16 res_h;    /*resolution height*/
    HI_U16 rows;
    HI_U16 columns;
    ARIBCC_PRTDIRECT_E stPrtDirect;
} ARIBCC_WinFmt_S;

typedef struct tagARIBCC_CAPTION_S
{
    HI_U16 au16Text[256];
    HI_U16 u16Length;       /*remaining to be drawn caption u16Length*/
    HI_U16 u16Offset;       /*u16Offset of drawn caption from au16Text[0]*/
    HI_U16 u16IsSpecial;   /*is there any special code(SP(0X20) & DEL(0X7F))*/
    ARIBCC_CODE_TABLE_E aenTable[2];      /*to indicate the code Table of GR & GL*/
    ARIBCC_CHARACTER_SET_E aenCharSet[4];  /*to indicate the character set of each code Table*/
} ARIBCC_CAPTION_S;

typedef struct tagARIBCC_ScrollText_S
{
    HI_U16 u16text;
    HI_U8 u8IsFlash;
    HI_U8 u8IsUnderline;
    HI_U8 ueEdgeType;
    HI_U8 u8CharInterval;
    HI_U8 u8LineInterval;
    HI_U8 u8FontHeight;
    HI_U8 u8FontWidth;
    HI_U8 u8FontScale;        /*font size magnify or shrink*/

    HI_UNF_CC_COLOR_S stFgColor;
    HI_UNF_CC_COLOR_S stFgMidColor;
    HI_UNF_CC_COLOR_S stBgColor;
    HI_UNF_CC_COLOR_S stBgMidColor;
    HI_UNF_CC_COLOR_S stEdgeColor;
    ARIBCC_TEXTBOX_S stTextBox;
} ARIBCC_ScrollText_S;

typedef struct tagARIBCC_ScrollBuf_S
{
    ARIBCC_ScrollText_S astScrollText[MAX_SCROLL_BUF_SIZE];
    HI_U16 u16Length;
} ARIBCC_ScrollBuf_S;

typedef struct tagARIBCC_Bitmap_S
{
    HI_U16 u16X;
    HI_U16 u16Y;
    HI_U8 u8HasData;       /*to indicate whether there is png data received*/
    HI_U8 *pu8PngData;
} ARIBCC_Bitmap_S;

typedef struct taARIBCC_DRCSEntry_S
{
    HI_U8 u8Code1;
    HI_U8 u8Code2;
    HI_U8 u8Depth;
    HI_U8 u8Width;
    HI_U8 u8Height;
    HI_U8 *pu8Bitmap;
    void *pvNext;
} ARIBCC_DRCSEntry_S;

typedef struct tagARIBCC_TEXTATTR_S
{
    HI_U8 u8Type;
    HI_U8 u8IsFlash;
    HI_U8 u8IsUnderline;
    HI_U8 u8EdgeType;
    HI_U8 u8CharInterval;
    HI_U8 u8LineInterval;

    HI_U8 u8FontHeight;
    HI_U8 u8FontWidth;
    /*the above two is set by the ccdecoder. There size must be among (16,20,24,30,36)
        in general. the above two size should be equal with the following two size*/

    HI_U8 u8NormalFontWidth;  /*the normal actual font height*/
    HI_U8 u8NormalFontHeight; /*the normal actual font width*/
    /*the above two size is returned from the TGE_GetTextWidth_Ext and TGE_GetFontHeight_Ext
           when the scale is normal*/

    HI_U8 u8MaxCharHeight;  /*maximum char height of current row*/
    HI_U8 u8MaxCharWidth;   /*maximum char width of current row*/

    HI_U8 u8FontScale;        /*font size magnify or shrink*/
    HI_U8 u8ColorPattern;     /*color polarity, inverse*/
    HI_U16 u16LastRowNum;
    HI_S32 s32RepeateNum;       /*repeate number*/
    HI_U8 u8ScrollType;
    HI_U8 u8ScrollSpeed;      /*pixel/second*/
    HI_U8 u8ScrolledRows;     /*for the no-rollout scroll count*/
    HI_U8 u8WinScrollMode;   /*if set to 1, the window scroll and the u8ScrollType is 4*/

    HI_U8 u8CurCharSet;     /*current character set*/
    HI_UNF_CC_COLOR_S stFgColor;
    HI_UNF_CC_COLOR_S stFgMidColor;
    HI_UNF_CC_COLOR_S stBgColor;
    HI_UNF_CC_COLOR_S stBgMidColor;
    HI_UNF_CC_COLOR_S stEdgeColor;
    ARIBCC_TEXTBOX_S stTextBox;
    ARIBCC_CURSOR_S stCursor;
    ARIBCC_CURSOR_S stLastCursor;       /*for scroll use*/
    ARIBCC_CAPTION_S stCaption;
    ARIBCC_CAPTION_S stLastDrawCaption;
    ARIBCC_ScrollBuf_S stScrollBuf;       /*to store the scroll text*/
    ARIBCC_Bitmap_S stBitmap;

    /*the following elementes are related windows*/
    HI_UNF_CC_COLOR_S stClearColor;     /*transparent*/
    HI_UNF_CC_COLOR_S stWinColor;       /*window color*/
    ARIBCC_WinAttr_S stWinAttr;
    ARIBCC_WinFmt_S stFormat;
    HI_U8 u8IsWinUpdate;  /*is window updated of the above three items*/
    HI_U8 u8IsTextDraw;  /*is this window be drawn with Caption, to use when cc/sd overlap*/

    volatile HI_U8 u8IsHscrollFinish;
    pthread_t pidHscrollThread;
    ARIBCC_DRCSEntry_S *pastDrcs[MAX_DRCS_TYPE][MAX_DRCS_SETS];
} ARIBCC_TEXTATTR_S;

typedef struct tagARIBCC_HScrollArgs_S
{
    HI_S32 s32CCType;
    HI_U8  u8ScrollType;
    HI_U8  u8ScrollSpeed;
} ARIBCC_HScrollArgs_S;


/**
 * pid:  plane id
 * hstart: horizontal start of cc plane
 * vstart: vertical start of cc plane
 * width: panel width
 * height: panel height
 */
void ARIBCC_OSD_Init(void);

void ARIBCC_OSD_DeInit(void);

void ARIBCC_OSD_SetCursorPosition(HI_S32 CCType, HI_S32 request, HI_S32 number1, HI_S32 number2, HI_S32 x, HI_S32 y);

void ARIBCC_OSD_ClearScreen(HI_S32 CCType);

void ARIBCC_OSD_SetTextColor(HI_S32 CCType, HI_U8 place, HI_U8 red, HI_U8 green,HI_U8 blue, HI_U8 alpha);

void ARIBCC_OSD_SetSize(HI_S32 CCType, HI_S32 size);

void ARIBCC_OSD_SetFlash(HI_S32 CCType, HI_S32 request);

void ARIBCC_OSD_SetPatternPolarity(HI_S32 CCType, HI_S32 ColorPattern);

void ARIBCC_OSD_SetCharBox(HI_S32 CCType, HI_S32 btm, HI_S32 rgt, HI_S32 top, HI_S32 lft);

void ARIBCC_OSD_SetRepeatNum(HI_S32 CCType, HI_S32 number);

void ARIBCC_OSD_Underline(HI_S32 CCType, HI_S32 flag);

void ARIBCC_OSD_SetWritingFormat(HI_S32 CCType, HI_U8 format);

void ARIBCC_OSD_SetWinColor(HI_S32 CCType, HI_U8 red, HI_U8 green,HI_U8 blue, HI_U8 alpha);

void ARIBCC_OSD_SetWindowSize(HI_S32 CCType, HI_S32 width, HI_S32 height);

void ARIBCC_OSD_SetWindowPos(HI_S32 CCType, HI_S32 x, HI_S32 y);

void ARIBCC_OSD_SetCharacterSize(HI_S32 CCType, HI_S32 size);

void ARIBCC_OSD_SetHorizontalSpacing(HI_S32 CCType, HI_S32 interval);

void ARIBCC_OSD_SetVerticalSpacing (HI_S32 CCType, HI_S32 interval);

void ARIBCC_OSD_SetOrnament(HI_S32 CCType, HI_S32 number, HI_U8 red, HI_U8 green, HI_U8 blue, HI_U8 alpha);

/*set scroll type*/
void ARIBCC_OSD_SetScrollInfo(HI_S32 CCType, HI_S32 type, HI_S32 speed);


/*play build-in sound*/
void ARIBCC_OSD_Pra(HI_S32 number);

/*receive closed caption string*/
void ARIBCC_OSD_DrawText(HI_S32 CCType, const HI_U8 *pu8text, HI_U32 u32length);


void ARIBCC_OSD_DrawBitmap(HI_S32 CCType, HI_U8 *pu8bitmap);


void ARIBCC_OSD_Show(HI_S32 CCType);

void ARIBCC_OSD_DrcsClear(HI_S32 CCType, char ClearType);

void ARIBCC_OSD_DrcsEntry(HI_S32 CCType, HI_U8 EntryType, HI_S32 type, HI_U8 code1, HI_U8 code2,HI_U8 depth,
                             HI_U8 width, HI_U8 height, HI_U8 *pbitmap);

void ARIBCC_OSD_SetCharacterCode(HI_S32 CCType, HI_U8 *pu8Designate, HI_U8 *pu8Invoke);


void ARIBCC_OSD_SpecialCode(HI_S32 CCType, HI_S32 request);

void ARIBCC_OSD_SetRollupMode(HI_S32 mode);

/**
 * function name: ARIBCC_OSD_EnableShow
 * description: to enable multi program share one plane of the OSD. we add this function to control the
 * cc plane whether show or hide.
 * arguments:
 *        enable: 0   don't show the cc plane. it means the cc can't shown.
 *                   1   show the cc plane.  it means the cc plane can control the plane.
 **/
void ARIBCC_OSD_EnableShow(HI_S32 enable);


#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif
