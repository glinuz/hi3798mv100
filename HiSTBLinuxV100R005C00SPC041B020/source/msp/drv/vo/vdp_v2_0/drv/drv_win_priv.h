
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_priv.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_PRIV_H__
#define __DRV_WIN_PRIV_H__

#include "drv_disp_com.h"
#include "drv_win_hal.h"
#include "drv_win_prc.h"
#include "drv_win_buffer.h"
#include "drv_disp_buffer.h"
#include "drv_virtual.h"
#include "drv_sync.h"
#include "drv_pq_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* window state */
#define WIN_DEVICE_STATE_CLOSE   0
#define WIN_DEVICE_STATE_OPEN    1
#define WIN_DEVICE_STATE_SUSPEND 2

#define WIN_DISPLAY_MAX_NUMBER 17
#define WIN_VIRTAUL_MAX_NUMBER 17

#define WIN_INDEX_PREFIX_MASK 0xFFFF0000ul
#define WIN_INDEX_PREFIX_SHIFT_NUMBER 16
#define WIN_INDEX_PREFIX      ((HI_U32)HI_ID_VO << WIN_INDEX_PREFIX_SHIFT_NUMBER)

/*VIRTUAL PREFIX*/
//#define WIN_INDEX_VIRTUAL_PREFIX  ((((HI_U32)HI_ID_VO << WIN_INDEX_PREFIX_SHIFT_NUMBER))|0x5a000000)
#define WIN_INDEX_VIRTUAL_CHANNEL ((HI_U32)0xF)

#define WIN_INDEX_DISPID_SHIFT_NUMBER 8
#define WIN_INDEX_DISPID_MASK 0x0000000Ful
#define WIN_INDEX_MASK        0x000000FFul

#define WIN_INDEX_SLAVE_PREFIX  0x00000080ul

#define WIN_FRAME_MIN_WIDTH  64
#define WIN_FRAME_MIN_HEIGHT 64
#define WIN_FRAME_MAX_WIDTH  7680
#define WIN_FRAME_MAX_HEIGHT 4320

#define WIN_MAX_ASPECT_RATIO 16

#define WIN_INRECT_MIN_WIDTH   64
#define WIN_INRECT_MAX_WIDTH   7680
#define WIN_INRECT_MIN_HEIGHT  64
#define WIN_INRECT_MAX_HEIGHT  4320

#define WIN_OUTRECT_MIN_WIDTH  64
#define WIN_OUTRECT_MAX_WIDTH  4096
#define WIN_OUTRECT_MIN_HEIGHT 64
#define WIN_OUTRECT_MAX_HEIGHT 2160

#define WIN_CROPRECT_MAX_OFFSET_TOP     128
#define WIN_CROPRECT_MAX_OFFSET_LEFT    128
#define WIN_CROPRECT_MAX_OFFSET_BOTTOM  128
#define WIN_CROPRECT_MAX_OFFSET_RIGHT   128


/* in 1/100 Hz */
#define WIN_MAX_FRAME_RATE   12000
#define WIN_TRANSFER_CODE_MAX_FRAME_RATE 3000

#define WIN_MAX_FRAME_PLAY_TIME  1

#define WIN_IN_FB_DEFAULT_NUMBER 16
#define WIN_USING_FB_MAX_NUMBER 2

#define WIN_USELESS_FRAME_MAX_NUMBER  16

typedef struct tagWIN_CONFIG_S
{
    HI_DRV_WIN_ATTR_S stAttr;

    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;

    HI_DRV_WIN_ATTR_S stAttrBuf;
    atomic_t bNewAttrFlag;

    HI_DRV_WIN_SRC_INFO_S stSource;

    /* may change when window lives */
    HI_BOOL bQuickOutput;

    /*  */
    HI_DRV_COLOR_SPACE_E enFrameCS;
    HI_U32 u32Fidelity;
    HI_DRV_COLOR_SPACE_E enOutCS;
}WIN_CONFIG_S;

typedef struct tagWIN_STATISTIC_S
{
    HI_U32 u32Reserved;

}WIN_STATISTIC_S;

typedef struct tagWIN_DEBUG_S
{
    HI_U32 u32Reserved;

}WIN_DEBUG_S;

typedef enum tagWIN_FRAME_TYPE_E
{
    WIN_FRAME_NORMAL = 0,
    WIN_FRAME_BLACK,
    WIN_FRAME_FREEZE,
    WIN_FRAME_TYPE_BUTT
}WIN_FRAME_TYPE_E;

typedef struct tagWIN_BUF_NODE_S
{
    HI_DRV_VIDEO_FRAME_S stFrame;
    HI_U32 u32BufId;
    HI_BOOL bIdle;
    WIN_FRAME_TYPE_E enType;
}WIN_BUF_NODE_S;

#define WIN_INVALID_BUFFER_INDEX 0xfffffffful
#define WIN_BLACK_FRAME_INDEX   0xf0000001ul


typedef struct tagWIN_BUFFER_S
{
    WB_POOL_S stWinBP;

    /* for  frame buffer not used ,should moved to wb_pool_s next.*/
    HI_DRV_VIDEO_FRAME_S stUselessFrame[WIN_USELESS_FRAME_MAX_NUMBER];
    HI_U32 u32ULSRdPtr;
    HI_U32 u32ULSWtPtr;
    HI_U32 u32ULSIn;
    HI_U32 u32ULSOut;


    HI_U32 u32UnderLoad;
    spinlock_t stUlsLock;
}WIN_BUFFER_S;

typedef struct tagWIN_FREEZE_PRIV_S
{
    HI_DRV_WIN_SWITCH_E enFreezeMode;
    HI_U32 u32Reserve;
}WIN_FREEZE_PRIV_S;

typedef struct tagWIN_RESET_PRIV_S
{
    HI_DRV_WIN_SWITCH_E enResetMode;
    HI_U32 u32Reserve;
}WIN_RESET_PRIV_S;

typedef enum tagWIN_STATE_E
{
    WIN_STATE_WORK = 0,
    WIN_STATE_PAUSE,
    WIN_STATE_RESUME,
    WIN_STATE_FREEZE,
    WIN_STATE_UNFREEZE,
    WIN_STATE_BUTT
}WIN_STATE_E;

typedef struct tagWIN_DELAY_INFO_S
{
    HI_U32 u32DispRate;  /* in 1/100 Hz */
    HI_U32 T;  /* in ms */

//    volatile HI_U32 u32FrameNumber;
    volatile HI_U32 u32DisplayTime;
    volatile HI_U32 u32CfgTime;
    HI_BOOL bInterlace;
    volatile HI_BOOL bTBMatch;  /* for interlace frame display on interlace timing */
}WIN_DELAY_INFO_S;


typedef struct tagWIN_DISP_INFO_S
{
    HI_U32 u32RefreshRate;
    HI_BOOL bIsInterlace;
    HI_BOOL bIsBtm;
    HI_RECT_S stWinInitialFmt;
    HI_RECT_S stWinCurrentFmt;
}WIN_DISP_INFO_S;

typedef enum tagWINLAYER_OPT_TYPE_E {
    LAYER_OPT_ZORDER_ADJUST = 0,
    LAYER_OPT_BUTT
}WINLAYER_OPT_TYPE_E;

typedef struct tagWIN_LAYER_OPT_S {
    WINLAYER_OPT_TYPE_E layerOptType;
    HI_BOOL             bEffective;
    HI_DRV_DISP_ZORDER_E             enLayerOpt;
}WIN_LAYER_OPT_S;

typedef struct hiDRV_VIDEO_TMP_INFO_S
{
    HI_U32 u32WinInWidth;
    HI_U32 u32WinInHeight;
    HI_U32 u32WinOutWidth;
    HI_U32 u32WinOutHeight;

    HI_DRV_PIX_FORMAT_E  enPixFormat;
    HI_U32 u32FrameWidth;
    HI_U32 u32FrameHeight;

    HI_U32 u32DispWidth;
    HI_U32 u32DispHeight;
}HI_DRV_VIDEO_TMP_INFO_S;

typedef struct hiDRV_VIDEO_MISC_INFO_S
{
    HI_RECT_S stFrameOriginalRect;
    HI_RECT_S stWinOutRect;
    HI_BOOL  bWinSrEnableCurrent;
    HI_BOOL  bWinSrEnableLast;

    HI_PQ_DCI_WIN_S  stWinDciLastConfig;
    HI_BOOL  bWinDciEnableLastConfig;


}HI_DRV_VIDEO_MISC_INFO_S;

typedef struct tagWINDOW_S
{
    HI_U32 u32Index;

    /* state */
    HI_BOOL bEnable;
    HI_BOOL bMasked;

    HI_DRV_DISPLAY_E  enDisp;
    HI_DRV_WIN_TYPE_E enType;

    /*the location*/
    HI_U32  u32VideoLayer;
    HI_U32  u32VideoLayerNew;

    HI_U32  u32VideoRegionNo;
    HI_U32  u32VideoRegionNoNew;

    /* window config */
    WIN_CONFIG_S stCfg;
    HI_BOOL bDispInfoChange;

    /* private attribute */
    HI_DRV_WIN_ATTR_S stUsingAttr;

    volatile HI_BOOL bUpState;
    volatile WIN_STATE_E enState;
    volatile WIN_STATE_E enStateNew;

    //reset flag
    volatile HI_BOOL bReset;
    WIN_RESET_PRIV_S stRst;

    /*resume flag.*/
    volatile HI_BOOL bNeedResume;

    // freeze flag
    WIN_FREEZE_PRIV_S stFrz;

    // quickout flag
    HI_BOOL bQuickMode;

    // stepmode flag
    HI_BOOL bStepMode;
    HI_BOOL bVirtScreenMode;

    /* play info */
    volatile WIN_DELAY_INFO_S stDelayInfo;
    volatile HI_BOOL bInInterrupt;
#if defined (CHIP_HIFONEB02)
    volatile HI_BOOL bDcmpEnable;
    volatile HI_BOOL bMute;
#endif
    /* lowdelay report idx*/
    HI_U32 u32LastInLowDelayIdx;
    HI_U32 u32LastOutLowDelayIdx;

    /* statistic info */
    WIN_STATISTIC_S stStatistic;

    /* debug info */
    HI_BOOL     bDebugEn;
    WIN_DEBUG_S stDebug;

    /*  buffer */
    WIN_BUFFER_S stBuffer;
    HI_BOOL bConfigedBlackFrame;

    /* slave window for HD&SD display the same content at the same time */
    HI_HANDLE hSlvWin;
    HI_HANDLE pstMstWin;

    WIN_DISP_INFO_S stDispInfo;
    HI_U32 u32TBTestCount;
    HI_U32 u32TBNotMatchCount;

    HI_DRV_ROT_ANGLE_E enRotation;
    HI_BOOL bVertFlip;
    HI_BOOL bHoriFlip;
    /* video surface function */
    VIDEO_LAYER_FUNCTIONG_S stVLayerFunc;

    /*the lastest frame may be a member of window.
     * this may be a temporary modify,should be discussed with others.
     */
    HI_DRV_VIDEO_FRAME_S  latest_display_frame;
    HI_U32                latest_display_frame_valid;
    DISP_MMZ_BUF_S        stWinCaptureMMZ;
    HI_U32                u32WinCapMMZvalid;
    HI_BOOL               bRestoreFlag;
    HI_U32                u32Zorder;

    WIN_LAYER_OPT_S       stWinLayerOpt;
    HI_DRV_VIDEO_TMP_INFO_S stVideoTmpInfo;

    /*only for developer or mainteiner.*/
    HI_WIN_RROC_FOR_DEVELOPER_S stWinInfoForDeveloper;

    /*misc information, difficult to classify.*/
    HI_DRV_VIDEO_MISC_INFO_S stMiscInfor;

#ifdef HI_VO_WIN_SYNC_SUPPORT
	WIN_SYNC_INFO_S stSyncInfo;
#endif
	HI_HANDLE hFrc;

    HI_DRV_LOWDELAY_STAT_INFO_S stLowdelayStat;
}WINDOW_S;



#define MAX_RELEASE_NO  16

typedef  struct task_struct*    WIN_THREAD;
typedef   wait_queue_head_t    WAIT_QUEUE_HAEAD;

typedef enum tagTHREAT_EVENT_E
{
    EVENT_NOTHING= 0,
    EVENT_RELEASE,
    EVENT_BUTT
}THREAT_EVENT_E;

typedef struct hiWIN_RELEASE_FRM_S
{
    WAIT_QUEUE_HAEAD  stWaitQueHead;
    THREAT_EVENT_E  enThreadEvent;
    WIN_THREAD  hThread;

     HI_DRV_VIDEO_FRAME_S *pstNeedRelFrmNode[MAX_RELEASE_NO];
} WIN_RELEASE_FRM_S;

typedef struct tagDISPLAY_WINDOW_S
{
    WINDOW_S *pstWinArray[HI_DRV_DISPLAY_BUTT][WINDOW_MAX_NUMBER];
    HI_U32    u32WinNumber[HI_DRV_DISPLAY_BUTT];
    VIDEO_LAYER_FUNCTIONG_S stVSurfFunc;
    WIN_RELEASE_FRM_S stWinRelFrame;
    HI_BOOL  bWinManageStatus;

    /*if window system changed, this is a event.*/
    atomic_t   bWindowSytemUpdate[HI_DRV_DISPLAY_BUTT];
}DISPLAY_WINDOW_S;


typedef struct tagVIRTUAL_WINDOW_S
{
    HI_U32    u32WinNumber;
    VIRTUAL_S *pstWinArray[WIN_VIRTAUL_MAX_NUMBER];
}VIRTUAL_WINDOW_S;



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_VO_PRIV_H__  */










