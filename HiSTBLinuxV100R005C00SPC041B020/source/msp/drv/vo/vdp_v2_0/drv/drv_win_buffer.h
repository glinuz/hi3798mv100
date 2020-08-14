
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_buffer.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_wIN_BUFFER_H__
#define __DRV_wIN_BUFFER_H__

#include "drv_disp_com.h"
#include "hi_drv_win.h"
#include "drv_disp_bufcore.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define WIN_BUF_MEM_SRC_SUPPLY  0
#define WIN_BUF_MEM_FB_SUPPLY   1
#define WIN_BUF_MEM_USER_SUPPLY 2

#define WIN_CHECK_NULL_RETURN(p) \
do{ if (!p)    \
    {WIN_FATAL("FUNC %s input NULL Pointer!\n", __FUNCTION__); return HI_FAILURE;} \
}while(0)

#define WIN_CHECK_NULL_RETURN_NULL(p) \
do{ if (!p)    \
    {WIN_FATAL("FUNC %s input NULL Pointer!\n", __FUNCTION__); return HI_NULL;} \
}while(0)

typedef struct tagWIN_BUF_ALLOC_PARA_S
{
    HI_BOOL bFbAllocMem;
    HI_U32 u32BufWidth;
    HI_U32 u32BufHeight;
    HI_U32 u32BufStride;
    HI_DRV_PIX_FORMAT_E eDataFormat;

    // user supply mem
}WIN_BUF_ALLOC_PARA_S;

typedef struct tagWB_SOURCE_INFO_S
{
    HI_HANDLE hSrc;
    PFN_GET_FRAME_CALLBACK pfAcqFrame;
    PFN_PUT_FRAME_CALLBACK pfRlsFrame;
}WB_SOURCE_INFO_S;


#define WB_BUFFER_DEBUG_FRAME_RECORD_NUMBER 32
typedef struct tagWB_DEBUG_INFO_S
{
    HI_U32 u32RecordNumber;

    HI_U32 u32InputFrameID[WB_BUFFER_DEBUG_FRAME_RECORD_NUMBER];
    HI_U32 u32InputPos;
    HI_U32 u32Input;

    HI_U32 u32CfgFrameID[WB_BUFFER_DEBUG_FRAME_RECORD_NUMBER];
    HI_U32 u32CfgPos;
    HI_U32 u32Config;
    
    HI_U32 u32RlsFrameID[WB_BUFFER_DEBUG_FRAME_RECORD_NUMBER];
    HI_U32 u32RlsPos;
    HI_U32 u32Release;

    HI_U32 u32TryQueueFrame;
    HI_U32 u32QueueFrame;
    HI_U32 u32Underload;
    HI_U32 u32Disacard;
}WB_DEBUG_INFO_S;

typedef struct tagWB_POOL_S
{
    HI_U32 u32BufNumber;
    
    HI_U32 u32MemType;
    WIN_BUF_ALLOC_PARA_S stAlloc;
    DISP_BUF_S stBuffer;
    // source info
    WB_SOURCE_INFO_S stSrcInfo;

    /*lastest display and config.*/
    DISP_BUF_NODE_S *pstDisplay;
    DISP_BUF_NODE_S *pstConfig;
    DISP_BUF_NODE_S *pstCapture;
    
    WB_DEBUG_INFO_S *pstDebugInfo;
}WB_POOL_S;

HI_S32 WinBuf_Create(HI_U32 u32BufNum, HI_U32 u32MemType, WIN_BUF_ALLOC_PARA_S *pstAlloc, WB_POOL_S *pstWinBP);
HI_S32 WinBuf_Destroy(WB_POOL_S *pstWinBP);
HI_S32 WinBuf_Reset(WB_POOL_S *pstWinBP);

HI_S32 WinBuf_SetSource(WB_POOL_S *pstWinBP, WB_SOURCE_INFO_S *pstSrc);

// put a new frame into buffer
HI_S32 WinBuf_PutNewFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U32 u32PlayCnt);

// release frame that has been displayed and set configed frame as displayed frame.
HI_S32 WinBuf_RlsAndUpdateUsingFrame(WB_POOL_S *pstWinBP);
HI_S32 WinBuf_RepeatDisplayedFrame(WB_POOL_S *pstWinBP);
HI_S32 WinBuf_DiscardDisplayedFrame(WB_POOL_S *pstWinBP);
HI_DRV_VIDEO_FRAME_S *WinBuf_GetDisplayedFrame(WB_POOL_S *pstWinBP);
HI_DRV_VIDEO_FRAME_S *WinBuf_GetConfigedFrame(WB_POOL_S *pstWinBP);

HI_DRV_VIDEO_FRAME_S *WinBuf_GetConfigFrame(WB_POOL_S *pstWinBP);
HI_S32 WinBuf_SetCaptureFrame(WB_POOL_S *pstWinBP, HI_U32 u32InvalidFlag);
HI_S32 WinBuf_ReleaseCaptureFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_BOOL bForceFlag);
HI_DRV_VIDEO_FRAME_S *WinBuf_GetCapturedFrame(WB_POOL_S *pstWinBP);




HI_S32 WinBuf_ForceReleaseFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 WinBuf_ReleaseOneFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstPreFrame);
HI_S32 WinBuf_FlushWaitingFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstPreFrame);
HI_DRV_VIDEO_FRAME_S *WinBuf_GetFrameByMaxID(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstRefFrame,HI_U32 u32RefID, HI_DRV_FIELD_MODE_E enDstField);
HI_DRV_VIDEO_FRAME_S * WinBuf_GetFrameByDstFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstDstFrame, HI_DRV_VIDEO_FRAME_S *pstRefFrame);
HI_DRV_VIDEO_FRAME_S *WinBuf_GetFrameByDisplayInfo(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstRefFrame, HI_U32 u32RefRate, HI_DRV_FIELD_MODE_E enDstField);
HI_DRV_VIDEO_FRAME_S * WinBuf_GetNewestFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstRefFrame);
HI_S32 WinBuf_GetFullBufNum(WB_POOL_S *pstBP, HI_U32 *pu32BufNum);

// todo
typedef struct tagWB_STATE_S
{
    HI_U32 u32Number;

    HI_U32 u32EmptyRPtr;
    HI_U32 u32EmptyWPtr;

    HI_U32 u32FullRPtr;
    HI_U32 u32FullWPtr;
    
    struct {
        HI_U32 u32State;
        HI_U32 u32Empty;
        HI_U32 u32Full;
        HI_U32 u32FrameIndex;
    }stNode[DISP_BUF_NODE_MAX_NUMBER];

    WB_DEBUG_INFO_S stRecord;

    HI_U32 u32EmptyArray[DISP_BUF_NODE_MAX_NUMBER];
    HI_U32 u32FullArray[DISP_BUF_NODE_MAX_NUMBER];

    HI_DRV_VIDEO_FRAME_S stCurrentFrame;
}WB_STATE_S;

HI_S32 WinBuf_GetStateInfo(WB_POOL_S *pstWinBP, WB_STATE_S *pstWinBufState);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_wIN_BUFFER_H__  */


