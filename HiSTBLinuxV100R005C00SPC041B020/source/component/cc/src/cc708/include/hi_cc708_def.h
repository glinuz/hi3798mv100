
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

#ifndef _CC708_DEF_H__
#define _CC708_DEF_H__

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "hi_type.h"
#include "ccdisp_api.h"
#include "cc_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define CRITICAL_SECTION pthread_mutex_t
#define EnterCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_lock(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_lock failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define LeaveCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_unlock(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_unlock failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define InitializeCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_init(x,NULL); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_init failure,ret = 0x%x\n",ret); \
        } \
    }while(0)
    
#define DeleteCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_destroy(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_destroy failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

/**********************************/
/* DTVCC Character Codes                      */
/**********************************/

/*C0 Code Space*/
#define C0_NUL  ((HI_U8) 0x0)
#define C0_ETX  ((HI_U8) 0x3)
#define C0_CR   ((HI_U8) 0xD)        // carriage return (soft)
#define C0_HCR ((HI_U8) 0xE)
#define C0_FF ((HI_U8) 0xC)
#define C0_EXT1 ((HI_U8) 0x10)       // code space extension - 1 HI_U8

/*C1 Code Space */
#define C1_DLC      ((HI_U8) 0x8E)   // DelayCancel
#define C1_RST      ((HI_U8) 0x8F)   // Reset


#define MAX_CC_WINDOWS       8
#define MAX_CC_ROWS          15
#define MAX_CC_COLUMNS    55
#define MAX_CC_COLUMNS_4B3       32
#define MAX_CC_COLUMNS_14B9      37
#define MAX_CC_COLUMNS_16B9      42
#define MAX_CC_SUPERCELL_ABSOLUTE_ANCHOR_HORZ_4B3 32
#define MAX_CC_SUPERCELL_ABSOLUTE_ANCHOR_VERT_4B3 15
#define MAX_CC_ABSOLUTE_ANCHOR_HORZ_4B3 160
#define MAX_CC_ABSOLUTE_ANCHOR_HORZ_14B9 185
#define MAX_CC_ABSOLUTE_ANCHOR_HORZ_16B9 210
#define MAX_CC_ABSOLUTE_ANCHOR_VERT   75
#define MAX_CC_RELATIVE_ANCHOR   100
#define MAX_CC_PEN_STYLE       8
#define MAX_CC_WINDOW_STYLE    8

#define MAX_CC_AP 9


/*define CHECK MACRO*/
#define CC708DECPARAM_UPPER_CHECK(argu,maxval,args,default_val) \
{ \
        if( (argu) >= (maxval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(<%d) check failed(%d)!\n", \
                (args), \
                (maxval), \
                (argu)); \
                argu = (default_val); \
        } \
}

#define CC708OSDPARAM_UPPER_CHECK_RETURN_VOID(argu,maxval,args) \
{ \
        if( (argu) >= (maxval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param<%d) check failed(%d)!\n", \
                (args), \
                (maxval), \
                (argu)); \
                return ; \
        } \
}

#define CC708OSDPARAM_CHECK_RETURN_VOID(argu,minval,maxval,args) \
{ \
        if( (argu) >= (maxval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param<%d) check failed(%d)!\n", \
                (args), \
                (maxval), \
                (argu)); \
                return ; \
        } \
        else if( (argu) <= (minval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param>%d) check failed(%d)!\n", \
                (args), \
                (minval), \
                (argu)); \
                return ; \
        } \
}

#define CC708OSDPARAM_CHECK_RETURN_VAL(argu,minval,maxval,args,val) \
{ \
        if( (argu) >= (maxval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param<%d) check failed(%d)!\n", \
                (args), \
                (maxval), \
                (argu)); \
                return val; \
        } \
        else if( (argu) <= (minval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param>%d) check failed(%d)!\n", \
                (args), \
                (minval), \
                (argu)); \
                return val; \
        } \
}

#define CC708OSDPARAM_UPPER_CHECK_RETURN_VAL(argu,maxval,args,val) \
{ \
        if( (argu) >= (maxval) ) \
        { \
                HI_INFO_CC("Parameter Error! %s validation(param<%d) check failed(%d)!\n", \
                (args), \
                (maxval), \
                (argu)); \
                return val; \
        } \
}

#define CC708OSDPARAM_CHECK_NULL_POINTER_RETURN_VOID(argu,args) \
{ \
        if((argu) == NULL) \
        { \
                HI_INFO_CC("Parameter Error! %s validation check failed!\n", \
                (args)); \
                return ; \
        } \
}

#define CC708OSDPARAM_CHECK_NULL_POINTER_RETURN_VAL(argu,args,val) \
{ \
        if((argu) == NULL) \
        { \
                HI_INFO_CC("Parameter Error! %s validation check failed!\n", \
                (args)); \
                return val; \
        } \
}

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

// print direction typedef
typedef enum tagCC708_PrintDirection_E
{
    CC_PD_LEFT_RIGHT,
    CC_PD_RIGHT_LEFT,
    CC_PD_TOP_BOTTOM,
    CC_PD_BOTTOM_TOP,
    MAX_CC_PD,
}CC708_PrintDirection_E;

// scroll direction typedef
typedef enum tagCC708_ScrollDirection_E
{
    CC_SD_LEFT_RIGHT,
    CC_SD_RIGHT_LEFT,
    CC_SD_TOP_BOTTOM,
    CC_SD_BOTTOM_TOP,
    MAX_CC_SD,
}CC708_ScrollDirection_E;

/*****************  display data ************/
// display effect typedef
typedef enum tagCC708_DisplayEffect_E
{
    CC_EFFECT_SNAP,
    CC_EFFECT_FADE,
    CC_EFFECT_WIPE,
    MAX_CC_EFFECT,
}CC708_DisplayEffect_E;

// effect direction typedef
typedef enum tagCC708_EffectDirection_E
{
    CC_DED_LEFT_RIGHT,
    CC_DED_RIGHT_LEFT,
    CC_DED_TOP_BOTTOM,
    CC_DED_BOTTOM_TOP,
    MAX_CC_DED,
}CC708_EffectDirection_E;

// offset typedef
typedef enum tagCC708_TextOffset_E
{
    CC_OS_SUBSCRIPT,
    CC_OS_NORMAL,
    CC_OS_SUPERSCRIPT,
    MAX_CC_OFFSET,
}CC708_TextOffset_E;

// text tag typedef
typedef enum tagCC708_TextTag_E
{
    CC_TT_DIALOG,
    CC_TT_SOURCE_ID,
    CC_TT_ELECTRONIC_VOICE,
    CC_TT_OTHER_LANGUAGE,
    CC_TT_VOICEOVER,
    CC_TT_AUDIBLE_TRANSLATION,
    CC_TT_SUBTITLE_TRANSLATION,
    CC_TT_VOICE_QUALITY,
    CC_TT_LYRICS,
    CC_TT_SOUND_EFFECT,
    CC_TT_MUSICAL_SCORE,
    CC_TT_EXPLETIVE,
    CC_TT_UNDEFINED_12,
    CC_TT_UNDEFINED_13,
    CC_TT_UNDEFINED_14,
    CC_TT_NON_DISPLAYED_TEXT,
    MAX_CC_TT,
}CC708_TextTag_E;

/*Window Attribution definition*/
typedef struct tagCC708_WindowAttr_S
{
    HI_UNF_CC_JUSTIFY_E     enJustify;
    CC708_PrintDirection_E  enPrintDirection;
    CC708_ScrollDirection_E enScrollDirection;
    HI_UNF_CC_WORDWRAP_E    enWordWrap;
    CC708_DisplayEffect_E   enDisplayEffect;
    CC708_EffectDirection_E enEffectDirection;
    HI_U8                   u8EffectSpeed;     /*in 0.5 second units,*/
    HI_U32       u32FillColor;
    HI_UNF_CC_OPACITY_E     enFillOpacity;
    HI_UNF_CC_EdgeType_E    enBorderType;
    HI_U32       u32BorderColor;
}CC708_WindowAttr_S;

/*pen attribution definition*/
typedef struct tagCC708_PenAttr_S
{
    HI_UNF_CC_FONTSIZE_E enFontSize;
    HI_UNF_CC_FONTNAME_E enFontName;
    CC708_TextOffset_E   enTextOffset;
    HI_U8                u8IsItalic;
    HI_U8                u8IsUnderline;
    HI_UNF_CC_EdgeType_E enEdgeType;
    HI_U32    u32FGColor;
    HI_UNF_CC_OPACITY_E  enFGOpacity;
    HI_U32    u32BGColor;
    HI_UNF_CC_OPACITY_E  enBGOpacity;
    HI_U32    u32EdgeColor;
}CC708_PenAttr_S;

typedef struct tagCC708_PenDef_S
{
    HI_UNF_CC_FONTSIZE_E enFontSize;
    HI_UNF_CC_FONTNAME_E enFontName;
    CC708_TextTag_E      enTextTag;
    CC708_TextOffset_E   enTextOffset;
    HI_UNF_CC_EdgeType_E enEdgeType;
    
    HI_U8                u8IsItalic;
    HI_U8                u8IsUnderline;

    HI_U32                u32EdgeColor;
    HI_U32                u32FGColor;
    HI_U8                u8FGOpacity;
    HI_U32                u32BGColor;
    HI_U8                u8BGOpacity;
}CC708_PenDef_S;

typedef struct tagCC708_TextDef_S
{
    HI_U16              u16Caption;     /* if equal to zero, there is no u16Caption */
    HI_U8               u8IsPenChanged; /* if the text attributes changed from this position */
    CC708_PenDef_S      stPen;
}CC708_TextDef_S;

typedef struct tagCC708_Cursor_S
{
    HI_S32     s32X;
    HI_S32     s32Y;
    HI_S32     s32CurRow;
    HI_S32     s32CurColumn;
}CC708_Cursor_S;

typedef struct tagCC708_Window_S
{
    HI_S32         s32WindowID;
    HI_U8          u8Priority;
    HI_U8          u8AnchorPoint;
    HI_U8          u8RelativePositioning;   /*0--absolute, 1-- relative*/
    HI_U8          u8AnchorVertical;
    HI_U8          u8AnchorHorizontal;
    HI_U8          u8RowCount;
    HI_U8          u8ColumnCount;
    HI_U8          u8IsRowLock;
    HI_U8          u8IsColumnLock;
    HI_U8          u8IsVisible;
    HI_U8          u8WindowStyleID;
    HI_U8          u8PenStyleID;

    /*follows are window attribution*/
    HI_UNF_CC_JUSTIFY_E        enTextJustify;
    CC708_PrintDirection_E     enTextPrintDirection;
    CC708_ScrollDirection_E    enTextScrollDirection;
    HI_UNF_CC_WORDWRAP_E       enWordWrap;
    CC708_DisplayEffect_E      enDisplayEffect;
    CC708_EffectDirection_E    enEffectDirection;
    HI_UNF_CC_EdgeType_E       enBorderType;
    HI_U8                      u8EffectSpeed;
    HI_U32                     u32FillColor;
    HI_U32                     u32FillOpacity;
    HI_U32                     u32BorderColor;

    /*follows are window position*/
    HI_U16 u16X;
    HI_U16 u16Y;
    HI_U16 u16Width;
    HI_U16 u16Height;

    /* row attributes */
    HI_S32                as32RowWidth[MAX_CC_ROWS];
    HI_S32                as32RowHeight[MAX_CC_ROWS];
    HI_BOOL               bAddedTailingSpaceWidth[MAX_CC_ROWS];

    CC708_Cursor_S        stCursor;
    CC708_PenDef_S        stCurPen;
    CC708_PenDef_S        stPrevPen;
    HI_BOOL               bPenColorChanged;

    /*the maximum row of a window is 15, the maximum column of a window due to the display ratio*/
    CC708_TextDef_S       astText[MAX_CC_ROWS][MAX_CC_COLUMNS];
    HI_U8                 u8IsEmpty;
    HI_BOOL               bPendingRow; /*if not left justified and window is visible, set this flag and draw the caption until next control command.*/
    HI_S32                s32PendingRow;
 
    HI_S32                s32OverlapLowerWindowMap;// window map which has lower u8Priority and overlaps with current one
    HI_S32                s32OverlapHigherWindowMap;// window map which has higher u8Priority and overlaps with current one

    //HI_UNF_CC_RECT_S CCClipArea[32];
    //HI_U8                u8ClipAreaNum;

    /* window display order */
    /* 0 is the first displayed window on the screen, 7 is the last */
    /* -1 is the default value */
    HI_S32                s32DisplayOrder;

}CC708_Window_S;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/

