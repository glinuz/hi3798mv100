/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

******************************************************************************
  File Name             :   teletext_showpage.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2012/03/12
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2012/03/12
    Author              :
Modification            :   Created file
******************************************************************************/

#ifndef __TELETEXT_SHOWPAGE_H__
#define __TELETEXT_SHOWPAGE_H__

#include <pthread.h>

#include "hi_unf_ttx.h"
#include "teletext_def.h"
#include "teletext_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TTX_MAG_NUM (8)     /** The NO  of  magazine  */
#define TTX_COL_NUM (40)   /** The NO  of  column */
#define TTX_LOP_COL_NUM (40)
#define TTX_CMB_COL_NUM (56)
#define TTX_CMB_MAX_COL_NUM (72) /*40+16+16*/
#define TTX_ROW_NUM (25)     /**  The NO of  displayed row */
#define TTX_FLOF_NUM (6)      /**  The  NO  of link page */

#define TTX_NAVIGATION_BAR (1)  /** navigation  bar */
#define TTX_SUBPAGE_BAR (1)   /** Show received subpage */
#define TTX_MAX_OPEN_OBJECTS (17)

#define ACTIVE_OBJECT               (1)
#define ADAPTIVE_OBJECT          (2)
#define PASSIVE_OBJECT             (3)
#define OBJECT_PRIORITY(type)       ((type) << 1)

/* basic colors */
#define TTX_COLOR_BLACK           0x0
#define TTX_COLOR_RED              0x1
#define TTX_COLOR_GREEN            0x2
#define TTX_COLOR_YELLOW         0x3
#define TTX_COLOR_BLUE             0x4
#define TTX_COLOR_MAGENTA         0x5
#define TTX_COLOR_CYAN             0x6
#define TTX_COLOR_WHITE           0x7
#define TTX_COLOR_TRANSPARENT      0x8
#define TTX_COLOR_HALF_RED         0x9
#define TTX_COLOR_HALF_GREEN       0xA
#define TTX_COLOR_HALF_YELLOW      0xB
#define TTX_COLOR_HALF_BLUE       0xC
#define TTX_COLOR_HALF_MAGENTA     0xD
#define TTX_COLOR_HALF_CYAN        0xE
#define TTX_COLOR_GREY             0xF
#define TTX_COLOR_UNDEFINED        0xFF

#define TTX_MAX_TIMESPACE (30 * 1000)     /** 30 second */

#define TTX_COLOR_CONVERT(color) (0xff000000 |(((color) & 0xf00) << 12) |(((color) & 0xf0) << 8) | (((color) & 0xf) << 4))


/** The struct to record  a flash area */
typedef  struct  tagTTX_FLAAREA_S
{
    HI_U8                 u8LineNum; /** Row  */
    HI_U8                 u8ColNum; /** Column */
    HI_U8                 u8DoubleHeight : 1; /**  Double  height or normal height */
    HI_U8                 u8DoubleWidth  : 1;/**  Double  width or normal width */
    HI_U8                 u8DoubleSize   : 1;/**  Double size or normal size */
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_COLOR      ForegdCol; /** Foreground  color */
    HI_UNF_TTX_COLOR      BackgdCol; /** Background color  */
    struct   tagTTX_FLAAREA_S * pstNext;
} TTX_FLAAREA_S, * TTX_FLAAREA_S_PTR;

typedef struct tagTTX_DISP_ATTR
{
    HI_BOOL bDHeight   : 1;
    HI_BOOL bBox       : 1;
    HI_BOOL bConceal   : 1;
    HI_BOOL bUnderline : 1;
    HI_BOOL bInvert    : 1;
    HI_BOOL bSeparated : 1;
    HI_BOOL bDWidth    : 1;
    HI_BOOL bChanged   : 1; /* attributes changed by adaptive objects */

    HI_U8 u8ForeColor;
    HI_U8 u8BackColor;
}TTX_DISP_ATTR;


typedef struct tagTTX_TRIPLET_PARSER
{
    HI_U8          u8Data[TTX_LINE_MEM_SIZE];
    HI_U8*        pu8Triplet;
    HI_U8          u8TripletPos;       /* position of the next triplet in buffer */

    HI_BOOL            bOpen         : 1;
    HI_BOOL            bRowOpen      : 1;
    HI_BOOL            bStartDefine  : 1;  /* if is at the start point of an object definition */
    HI_BOOL            bFlashChanged : 1;
    HI_U8            u8ObjectType;
    HI_U8            u8Priority;

    HI_U8            u8ActiveRow;
    HI_U8            u8ActiveCol;
    HI_U8            u8OffsetRow;
    HI_U8            u8OffsetCol;
    HI_U8            u8OriginRow;
    HI_U8            u8OriginCol;

    HI_U8            u8Flash;
    HI_U8            u8GDRCSSubPage;
    HI_U8            u8NDRCSSubPage;
    TTX_DISP_ATTR stAttr;

} TTX_TRIPLET_PARSER;

typedef struct tagTTX_DEF_G0G2_S
{
    HI_UNF_TTX_G0SET_E enG0Set;
    HI_UNF_TTX_G2SET_E enG2Set;
    HI_UNF_TTX_NATION_SET_E enNationSet;
}TTX_DEF_G0G2_S;

typedef struct tagTTX_SEC_G0_S
{
    HI_UNF_TTX_G0SET_E enG0Set;
    HI_UNF_TTX_NATION_SET_E enNationSet;
}TTX_SEC_G0_S;

typedef struct tagTTX_MODIFIED_G0G2_S
{
    HI_UNF_TTX_G0SET_E enG0Set;
    HI_UNF_TTX_G2SET_E enG2Set;
}TTX_MODIFIED_G0G2_S;
/** The struct to record  Page and Magazine charset */
typedef  struct tagTTX_CHARSET_S
{
    HI_U8 u8PgG0Set;  /** G0 set of this page */
    HI_U8 u8PgG1Set;          /** G1 set of this page */
    HI_U8 u8MagG0Set;      /** G0 set of this magazine */
    HI_U8 u8MagG1Set;       /** G1 set of this magazine */
} TTX_CHARSET_S;

/** An important struct, record a page context */
typedef  struct tagTTX_PAGE_CONTEXT_S
{
    HI_UNF_TTX_PAGE_ADDR_S stCurPgAddr;    /**  Current page address */
    HI_UNF_TTX_PAGE_ADDR_S stWantPgAddr;  /**  Expection  page address */
    HI_UNF_TTX_PAGE_ADDR_S stIndexPgAddr;  /** Index  page address   */
    HI_UNF_TTX_PAGE_ADDR_S astLink27[TTX_FLOF_NUM];    /**Link  page address   */
    HI_U32                  u32Flag;
    HI_U32                  u32ValLine;                 /**Valid packet of this page  */
    HI_U32                  u32ExtValLineH;         /**Extern  valid packet H */
    HI_U32                  u32ExtValLineL;          /**Extern  valid packet  L*/
    HI_BOOL                bTtxHiden;                /** Hiden or not  */
    HI_BOOL                bTtxMix; /** Mix  or  not */
    HI_BOOL                bConceal;   /** reveal  conceal or not */
    HI_BOOL                bFlash;
    HI_BOOL                bNavigation; /** navigation bar */
    HI_BOOL                bSubpageMode; /** if <subpage> key(remote controller) pressed */
    HI_BOOL                bSubtitle;                /**Subtile or teletext  */
    HI_BOOL                bHasP24;
    HI_BOOL                bShowP24;
    HI_BOOL                bBlackBgSubstitute;/*black background substitude*/
    HI_BOOL                bHasP27_4;
    HI_BOOL                bHasP27_5;
    HI_BOOL                bHasP28_0;
    HI_BOOL                bHasP28_1;
    HI_BOOL                bHasP28_4;

    HI_U32                 u32aPageText[TTX_ROW_NUM][TTX_CMB_MAX_COL_NUM]; /** Displayed   text*/
    TTX_PAGE_S_PTR   	   pstCurPage;
    TTX_INPUT_NUM_S        stInputNum;          /* Input Number: page number or page subcode */
    HI_U8                  u8Language[4];
    TTX_CHARSET_S          stCharSet;
    TTX_FLAAREA_S *        pFlaAreaHead; /**The head of flash area */

    TTX_P28_0_M29_0_INFO_S stP28_0_Info;
    TTX_P28_1_M29_1_INFO_S stP28_1_Info;
    TTX_P28_4_M29_4_INFO_S stP28_4_Info;

    TTX_MOT_INFO_S    stMOTInfo;

    HI_U8            u8BandStart[3];
    HI_U8            u8BandEnd[3];
    HI_U8            u8BandLeft[3];
    HI_U32          u32ColumnNum;

    HI_BOOL        bUpdateTime;                    /** Update time */
    HI_BOOL        bInvalidReq;                       /**  Invalid  Request page */
    HI_BOOL        bProcessed;                          /** The  page be processed or not */
    HI_BOOL        bAutoPlay;                          /**  Auto play subpage */
    HI_BOOL        bHold;                          /**  hold */
    HI_BOOL        bStart;                                 /** Main thread  start */
    HI_BOOL        bShowTaskStart;             /** Thread  of showpage start */
    HI_BOOL        bTTXSearching;               /**is searching ttx*/

    pthread_mutex_t Mutexlock;
    pthread_t       PidUpdateTask;  /** Pid of update task  thread  */

    HI_HANDLE        hTTX;/** handle of ttx instance */
    HI_UNF_TTX_CB_FN pfnCallBackFunction; /**Callback function  */

    HI_U8  *                pu8PgQueueMemory; /** Page data queue */
    TTX_SEGMENT_QUEUE_S     stPAGEQueue;
    TTX_SEGMENT_QUEUE_S_PTR pstPAGEQueue;
    HI_U32                  u32QueueSize;
    HI_U32  u32MaxIntervalMs; /**<max interval of teletext*/
    HI_U32  u32ZoomFlag;    /* 0:normal, 1:display half top, 2:display bottom half */

} TTX_PAGE_CONTEXT_S, *TTX_PAGE_CONTEXT_S_PTR;

/*
 *@brief: Input a page context then show a page
 *
 *@param[in] pstCurrentPoint - the handle of page context.
 *
 *@retval ::None
 */
HI_VOID                        TTX_Show_ShowPage(TTX_PAGE_CONTEXT_S  *  pstContextHead);

HI_S32                         TTX_Show_SetArea(HI_UNF_TTX_PAGEAREA_S *  pstPageArea, HI_U32 u32Row, HI_U32 u32Colunm,
                                                HI_U32 u32RowCount,
                                                HI_U32 u32ColumnCount);

HI_VOID                 TTX_Show_SetCharAttr(HI_UNF_TTX_CHARATTR_S * pstCharAttr, HI_U32 u32Index,
                                                    HI_BOOL bContiguous, HI_UNF_TTX_G0SET_E enG0Set,HI_UNF_TTX_G2SET_E enG2Set,
                                                    HI_UNF_TTX_CHARSET_E enCharSet,
                                                    HI_U8 u8NationSet);
HI_UNF_TTX_CHARSET_E    TTX_Show_GetCharSet(HI_U32 u32Index, HI_BOOL bMosMod);

HI_S32                         TTX_Show_SetOSDOutput_ON(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_S32                         TTX_Show_SetOSDOutput_OFF(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_S32                         TTX_Show_SetOSDOutputMsg(TTX_PAGE_CONTEXT_S * pstContextHead, HI_BOOL bEnable);

HI_VOID                        TTX_Show_ShowPrompt(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID                        TTX_Show_ShowPromptTimeOut(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID                        TTX_ShowPromptMsg(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID                        TTX_ShowPromptTimeOutMsg(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID TTX_Show_GetLOPG0G2AndNationSet(TTX_PAGE_CONTEXT_S*  pstContextHead, HI_BOOL bUseDefaultCharSet, HI_UNF_TTX_G0SET_E* penG0Set, HI_UNF_TTX_G2SET_E* penG2Set, HI_UNF_TTX_NATION_SET_E* penNationSet);
HI_VOID TTX_Show_GetModifiedG0G2(HI_U8 u8ModifiedValue, HI_UNF_TTX_G0SET_E* penG0Set, HI_UNF_TTX_G2SET_E* penG2Set);

/*
 *@brief: TTX callback function
 *
 *@param[in] pstCurrentPoint - the handle of page context.
 *@param[in] enCBType - function type, fillrectangle, drawchar....
 *@param[in] pvParam - param  correspond  to function type
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to exec callback function.
 */
HI_S32                         TTX_Show_CallBack(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_CB_E enCBType,
                                                 HI_VOID *pvParam);

HI_VOID TTX_Show_DrawNavigation(TTX_PAGE_CONTEXT_S * pstCurrentPoint, HI_U32 u32MaxColNum);
HI_VOID TTX_Show_DrawNavigationSubpageMode(TTX_PAGE_CONTEXT_S * pstCurrentPoint, HI_U32 u32MaxColNum);

#ifdef __cplusplus
}
#endif
#endif
