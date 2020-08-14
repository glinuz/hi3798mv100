/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_priv_avplay.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/05/21
  Description   :
  History       :
  1.Date        : 2010/05/21
    Author      : w58735
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_SYNC_H__
#define __HI_DRV_SYNC_H__

#include "hi_unf_avplay.h"
#include "hi_error_mpi.h"
#include "hi_drv_video.h"

#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif

#define HI_FATAL_SYNC(fmt...) \
            HI_FATAL_PRINT(HI_ID_SYNC, fmt)

#define HI_ERR_SYNC(fmt...) \
            HI_ERR_PRINT(HI_ID_SYNC, fmt)

#define HI_WARN_SYNC(fmt...) \
            HI_WARN_PRINT(HI_ID_SYNC, fmt)

#define HI_INFO_SYNC(fmt...) \
            HI_INFO_PRINT(HI_ID_SYNC, fmt)

#define HI_ERR_VSYNC(enChn, fmt...)\
        /*lint -save -e* */   \
        if (pSync->bPrint)\
        {\
            if (SYNC_CHAN_VID == enChn)\
            {\
                HI_ERR_PRINT(HI_ID_VSYNC, fmt);\
            }\
        }  \
        /*lint -restore */

#define HI_ERR_ASYNC(enChn, fmt...)\
        /*lint -save -e* */   \
        if (pSync->bPrint)\
        {\
            if (SYNC_CHAN_AUD == enChn)\
            {\
                HI_ERR_PRINT(HI_ID_ASYNC, fmt);\
            }\
        } \
        /*lint -restore*/

#define HI_INFO_VSYNC(enChn, fmt...)\
        /*lint -save -e* */   \
        if (pSync->bPrint)\
        {\
            if (SYNC_CHAN_VID == enChn)\
            {\
                HI_INFO_PRINT(HI_ID_VSYNC, fmt);\
            }\
        }  \
        /*lint -restore */

#define HI_INFO_ASYNC(enChn, fmt...)\
        /*lint -save -e* */   \
        if (pSync->bPrint)\
        {\
            if (SYNC_CHAN_AUD == enChn)\
            {\
                HI_INFO_PRINT(HI_ID_ASYNC, fmt);\
            }\
        } \
        /*lint -restore*/

typedef enum hiSYNC_CHAN
{
    SYNC_CHAN_AUD,
    SYNC_CHAN_VID,
    SYNC_CHAN_PCR,
    SYNC_CHAN_SCR,
	SYNC_CHAN_EXT, 
	
    SYNC_CHAN_BUTT
}SYNC_CHAN_E;

typedef enum hiSYNC_STATUS_E
{
    SYNC_STATUS_STOP = 0,
    SYNC_STATUS_PLAY,
    SYNC_STATUS_TPLAY,
    SYNC_STATUS_PAUSE,

    SYNC_STATUS_BUTT
}SYNC_STATUS_E;

typedef enum hiSYNC_PROC_E
{
    SYNC_PROC_DISCARD,
    SYNC_PROC_REPEAT, 
    SYNC_PROC_PLAY,
    SYNC_PROC_QUICKOUTPUT,
    SYNC_PROC_TPLAY,
    SYNC_PROC_CONTINUE,

    SYNC_PROC_BUTT
}SYNC_PROC_E;

typedef enum hiSYNC_AUD_SPEED_ADJUST_E
{
    SYNC_AUD_SPEED_ADJUST_NORMAL,
    SYNC_AUD_SPEED_ADJUST_UP, 
    SYNC_AUD_SPEED_ADJUST_DOWN, 
    SYNC_AUD_SPEED_ADJUST_MUTE_REPEAT,

    SYNC_AUD_SPEED_ADJUST_BUTT
}SYNC_AUD_SPEED_ADJUST_E;


typedef struct hiSYNC_VID_INFO_S
{
    HI_U32   SrcPts;      /* original pts */
    HI_U32   Pts;         /* amendatory pts */
    HI_U32   FrameTime;   /* duration time of one video frame*/
    HI_U32   DispTime;
    HI_BOOL  bProgressive;
	HI_U32   DelayTime;   /* delay time from sync judge to display */
	HI_U32   DispRate;
}SYNC_VID_INFO_S;

typedef struct hiSYNC_VID_OPT_S
{
    SYNC_PROC_E     SyncProc;
    HI_U32          Repeat;
    HI_U32          Discard;
    HI_U32          VdecDiscardTime;
    HI_DRV_VIDEO_TB_ADJUST_E    enTBAdjust;
}SYNC_VID_OPT_S;

typedef struct hiSYNC_AUD_INFO_S
{
    HI_U32   SrcPts;            /* original pts */
    HI_U32   Pts;               /* amendatory pts */
    HI_U32   FrameTime;         /* duration time of one audio frame */
    HI_U32   BufTime;           /* duration time in ao buffer */
    HI_U32   FrameNum;          /* audio frame number in adec buffer */
}SYNC_AUD_INFO_S;

typedef struct hiSYNC_AUD_OPT_S
{
    SYNC_PROC_E               SyncProc;
    SYNC_AUD_SPEED_ADJUST_E   SpeedAdjust;
}SYNC_AUD_OPT_S;

//#ifdef HI_MCE_SUPPORT
HI_S32 HI_DRV_SYNC_Init(HI_VOID);
HI_S32 HI_DRV_SYNC_DeInit(HI_VOID);
HI_S32 HI_DRV_SYNC_Create(HI_UNF_SYNC_ATTR_S *pstSyncAttr, HI_HANDLE *phSync);
HI_S32 HI_DRV_SYNC_Destroy(HI_HANDLE hSync);
HI_S32 HI_DRV_SYNC_Start(HI_HANDLE hSync, SYNC_CHAN_E enChn);
HI_S32 HI_DRV_SYNC_Stop(HI_HANDLE hSync, SYNC_CHAN_E enChn);
HI_S32 HI_DRV_SYNC_Play(HI_HANDLE hSync);
HI_S32 HI_DRV_SYNC_AudJudge(HI_HANDLE hSync, SYNC_AUD_INFO_S *pAudInfo, SYNC_AUD_OPT_S *pAudOpt);
HI_S32 HI_DRV_SYNC_VidJudge(HI_HANDLE hSync, SYNC_VID_INFO_S *pVidInfo, SYNC_VID_OPT_S *pVidOpt);
HI_S32 HI_DRV_SYNC_GetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr);
HI_S32 HI_DRV_SYNC_SetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pSyncAttr);
//#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
