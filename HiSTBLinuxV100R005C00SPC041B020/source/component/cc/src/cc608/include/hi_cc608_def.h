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


#ifndef _CC608_DEF_H
#define _CC608_DEF_H

#include <semaphore.h>
#include "cc_queue.h"
#include "hi_type.h"
#include "ccdisp_api.h"
#include "cc_debug.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define CC608ROWS               15  /*Maximum 15 rows*/
#define CC608COLUMNS            33  /*maximun 32 columns*/
#define CC608ROWCHARS           (CC608COLUMNS+32)

#define CC608DATA_QUEUE_SIZE    128
#define CC608DATA_BUFFER_SIZE   3 //fieldnum/cc1/cc2

#define CC608MODULE             0
#define MAX_CC608OBJECT_NUM     2

/* automatic erase of the displayed caption memory if no activity */
/* has occurred for at least 16 seconds */
#define CC608_AUTOERASE_TIMEOUT   (16) /* 16s */

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

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/
typedef enum tagCC608_VBI_FIELD_E
{
    CC608_VBI_FIELD1    = 1,         /**< field 1 */
    CC608_VBI_FIELD2    = 0,         /**< field 2 */
    CC608_VBI_MAX_FIELD = 2  /**< max field */
}CC608_VBI_FIELD_E;

typedef enum
{
    CC608_MODE_NOCAPTION,       /*There is no caption*/
    CC608_MODE_CAPTION,    /*Closed caption   cc1,cc2,cc3,cc4*/
    CC608_MODE_TEXT,       /*text mode  text1,text2,text3,text4*/
    CC608_MODE_XDS,
    CC608_MODE_BUTT
}CC608_MODE_E;

typedef enum
{
    CC608_DISP_PAINTON,       /*draw caption immediately when received*/
    CC608_DISP_POPON,         /*store the caption to non-displa memory still received EOC*/
    CC608_DISP_ROLLUP,        /*rollup text*/
    CC608_DISP_BUTT
}CC608_DISP_STYLE_E;

typedef enum
{
    CC608_BUFFER_ON_SCREEN = 0,  /*on screen buffer*/
    CC608_BUFFER_OFF_SCREEN,     /*off screen buffer*/
    CC608_BUFFER_TEXT_SCREEN,    /*text screen buffer*/
    CC608_BUFFER_BUFF
}CC608_BUFFER_TYPE_E;

typedef struct tagCC608_PENATTR_S
{
    HI_U32       u32BgColor;
    HI_UNF_CC_OPACITY_E     enBgOpacity;
    HI_U32       u32FgColor;
    HI_UNF_CC_OPACITY_E     enFgOpacity;
    HI_BOOL         bIsItalic;  /*is cc italic*/
    HI_BOOL         bIsUnderline;   /*is cc underline*/
    HI_BOOL         bIsFlash;       /*is cc flash on*/
    void *          reserved;       /*reserved for extend use*/
} CC608_PENATTR_S;

typedef struct tagCC608_CAPTION_S
{
    HI_U16      u16Text;            /*unicode*/
    HI_U8       u8IsChar;         /*is filled with a caption?*/
    HI_U8       u8IsDisplay;      /*is the caption displayed*/
    HI_U16      u16PacCode;        /*pac code of the screen*/
} CC608_CAPTION_S;

typedef struct tagCC608_BUFFER_S
{
    CC608_CAPTION_S   stCaption[CC608ROWS][CC608ROWCHARS]; /*unicode*/
    HI_U8             u8CharNum[CC608ROWS];                /*The char numbers of the row*/
    HI_U32            u32SeqNum[CC608ROWS];                /*seq num indicate the draw sequence*/
    HI_U32            u32CurSeq;                           /*current sequence number*/
    HI_U8             u8PacCode[CC608ROWS];                /*PAC CODE of every row*/
    HI_U8             u8IsPacOverwrite;                    /*is there any two pacs of the same row*/
    CC608_PENATTR_S   stLastPenAttr[CC608ROWS];            /*The last character's attributes of this row*/
    HI_U8             u8CurRow;                            /*current row number*/
    HI_U8             u8CurColumn;                         /*current column number*/
} CC608_BUFFER_S;

typedef struct tagCC608_SCREEN_S
{
    HI_UNF_CC_RECT_S  stPhyScreen;   /*panel or crt size*/
    HI_UNF_CC_RECT_S  stStaScreen;   /*safe title area*/
    HI_U8       u8RowHeight;         /*row height The value = sta height/CC608ROWS*/
    HI_U8       u8CharWidth;         /*The width of each character it's value = sta width/CC608COLUMNS*/
    HI_U16      u16CurTextWidth;      /*current cursor of 608 caption. The next text will be drawn at this positoin*/
    HI_U8       u8CurRow;            /*current row of caption*/
    HI_U8       u8CurColumn;         /*current column of caption*/

    HI_U8       u8BaseRow;           /*the base row of rollup type*/
    HI_U8       u8TopRow;            /*the top row of rollup type*/
} CC608_SCREEN_S;

typedef struct tagCC608_OBJECT_S
{
    HI_U8               module_id;
    CC608_MODE_E        enVbiMode;      /*vbi data stream mode(caption,text,xds)*/
    CC608_MODE_E        enCurMode;      /*user selected mode(caption,text)*/
    HI_U8               u8IsModeModified; /*is mode modified, when switch mode.set it to 1*/
    HI_U8               u8IsCaptureText;  /*to indicate whether to capture text*/
    HI_U8               u8IsXDSStarted;  /*is vbi data xds packet*/
    HI_U8               u8IsTextShown;   /*when show text set this variable. When EDM reset it*/
    CC608_DISP_STYLE_E  enDispStyle;         /*pop on, paint, rollup*/
    CC608_SCREEN_S      stScreen;
    CC608_BUFFER_S      stOnBuffer;
    CC608_BUFFER_S      stOffBuffer;
    CC608_BUFFER_S      stTextBuffer;
    CC608_PENATTR_S     stPenAttr;
    CC608_PENATTR_S     stUserPenAttr;
    HI_U8               au8CharWidth[2];  /*each character width*/

    CCDISP_WINDOW_ID_E  enWindowId;

    CRITICAL_SECTION   stCriticalSection; /*mutual exclusive critical section*/
    HI_UNF_CC_608_DATATYPE_E    enCCNum;              /*cc number cc1..cc4,tc1..tc4 */
    HI_U8              u8IsDecStart;            /*is decoder start*/
    TRI_QUEUE          stCCDataQueue;
    HI_U16             au16CCData[CC608DATA_QUEUE_SIZE][CC608DATA_BUFFER_SIZE];
    HI_U8              u8CmdCount;
    HI_S32             s32VChipTimerID;
    HI_S32             s32TimeoutEraseTimerID;
    HI_BOOL            bIgnoreBACWithoutSpace;
    HI_BOOL            bIgnoreExtCharWithoutStdChar;
    HI_BOOL            bOnlyUseSolidBlackBGIn608;
    HI_BOOL            bUnderline;
    HI_BOOL            bPACOverwrite;

    HI_BOOL            bLeadingTailingSpace;
} CC608_OBJECT_S;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif //#ifndef _CC608DEF_H

/*****************************************************************************
*                    End Of File
*****************************************************************************/
