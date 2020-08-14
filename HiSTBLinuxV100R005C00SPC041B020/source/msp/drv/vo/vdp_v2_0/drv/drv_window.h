
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_window.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WINDOW_H__
#define __DRV_WINDOW_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_video.h"
#include "hi_drv_win.h"
#include "drv_win_hal.h"

#ifdef HI_VO_WIN_SYNC_SUPPORT
#include "drv_sync.h"
#endif




#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */
/******************************************************************************
    local function and macro
******************************************************************************/
#define WinGetType(pstWin)    (pstWin->enType)
#define WinGetLayerID(pstWin) (pstWin->eLayer)
#define WinGetDispID(pstWin)  (pstWin->enDisp)

#define WinCheckDeviceOpen()    \
{                                \
    if (WIN_DEVICE_STATE_OPEN != s_s32WindowGlobalFlag)  \
    {                            \
        WIN_ERROR("WIN is not inited or suspended in %s!\n", __FUNCTION__); \
        return HI_ERR_VO_NO_INIT;  \
    }                             \
}

#define WinCheckNullPointer(ptr) \
{                                \
    if (!ptr)                    \
    {                            \
        WIN_ERROR("WIN Input null pointer in %s!\n", __FUNCTION__); \
        return HI_ERR_VO_NULL_PTR;  \
    }                             \
}

#define WinCheckWindow(hWin, pstWin) \
{                                    \
    pstWin = WinGetWindow(hWin);     \
    if (!pstWin)                      \
    {                                \
        WIN_ERROR("WIN is not exist! %#x\n", hWin); \
        return HI_ERR_VO_WIN_NOT_EXIST; \
    }  \
}

#define WinCheckSlaveWindow(pstWin) \
{                                   \
    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin)) \
    {                               \
        if (!pstWin->hSlvWin)        \
        {                           \
            WIN_ERROR("WIN Slave window is lost in %s\n", __FUNCTION__); \
            return HI_ERR_VO_SLAVE_WIN_LOST;    \
        }                           \
    }                               \
}

#define WINDOW_INVALID_ID 0xFFFFFFFFul

#define WINDOW_MAX_NUMBER 17


#define WINDOW_INDEX_MASK        0x00000FFFl
#define WINDOW_INDEX_NUMBER_MASK 0x000000FFl

HI_S32 WIN_Init(HI_VOID);
HI_S32 WIN_DeInit(HI_VOID);

HI_S32 WIN_Suspend(HI_VOID);
HI_S32 WIN_Resume(HI_VOID);

//HI_S32 WIN_SetMode(HI_DRV_VO_MODE_E enDevMode);
//HI_S32 WIN_GetMode(HI_DRV_VO_MODE_E *enDevMode);

HI_S32 WIN_Create(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWin,HI_BOOL bVirtScreen);
HI_S32 WIN_Destroy(HI_HANDLE hWin);
HI_S32 WIN_CheckAttachState(HI_HANDLE hWin,HI_BOOL *pbSrcAttached,HI_BOOL *pbSinkAttached);

HI_S32 WIN_SetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);
HI_S32 WIN_GetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);

//get info for source
HI_S32 WIN_GetInfo(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo);

HI_S32 WIN_SetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc);
HI_S32 WIN_GetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc);

HI_S32 WIN_SetEnable(HI_HANDLE hWin, HI_BOOL bEnable);
HI_S32 WIN_GetEnable(HI_HANDLE hWin, HI_BOOL *pbEnable);

HI_S32 WIN_QueueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);
HI_S32 WIN_QueueSyncFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo, HI_U32 *pu32FenceFd);
HI_S32 WIN_QueueUselessFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);
HI_S32 WIN_DequeueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);


HI_S32 WIN_GetPlayInfo(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S *pstInfo);

HI_S32 WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E enZFlag);
HI_S32 WIN_GetZorder(HI_HANDLE hWin, HI_U32 *pu32Zorder);

HI_S32 WIN_Freeze(HI_HANDLE hWin, HI_BOOL bEnable,  HI_DRV_WIN_SWITCH_E eRst);
HI_S32 WIN_GetFreezeStatus(HI_HANDLE hWin, HI_BOOL *pbEnable, HI_DRV_WIN_SWITCH_E *penFrz);

HI_S32 WIN_Reset(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E eRst);

HI_S32 WIN_Pause(HI_HANDLE hWin, HI_BOOL bEnable);

HI_S32 WIN_SetStepMode(HI_HANDLE hWin, HI_BOOL bStepMode);
HI_S32 WIN_SetStepPlay(HI_HANDLE hWin);

/* only for virtual window */
HI_S32 WIN_SetExtBuffer(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf);
HI_S32 WIN_AcquireFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
HI_S32 WIN_ReleaseFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
HI_S32 WIN_AttachSink(HI_HANDLE hWin, HI_HANDLE hSink);
HI_S32 WIN_DetachSink(HI_HANDLE hWin, HI_HANDLE hSink);
HI_S32 WIN_SetVirtualAttr(HI_HANDLE hWin, HI_U32 u32Width,HI_U32 u32Height,HI_U32 u32FrmRate);

HI_S32 WIN_SetQuick(HI_HANDLE hWin, HI_BOOL bEnable);
HI_S32 WIN_GetQuick(HI_HANDLE hWin, HI_BOOL *pbEnable);

HI_S32 WIN_CapturePicture(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);
HI_S32 WIN_CapturePictureRelease(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);

HI_S32 WIN_SetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E enRotation);
HI_S32 WIN_GetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E *penRotation);

HI_S32 WIN_SetFlip(HI_HANDLE hWin, HI_BOOL bHoriFlip, HI_BOOL bVertFlip);
HI_S32 WIN_GetFlip(HI_HANDLE hWin, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip);

HI_S32 WIN_GetUnload(HI_HANDLE hWin, HI_U32 *pu32Times);

HI_S32 WIN_SendFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
HI_S32 WIN_DestroyStillFrame(HI_DRV_VIDEO_FRAME_S *pStillFrameinfo);
HI_U32 WinGetIndex(HI_HANDLE hWin, HI_DRV_DISPLAY_E *enDisp, HI_U32 *u32WinIndex);
HI_S32 WinForceClearCapture(HI_HANDLE hWin);
HI_S32 WIN_Test(HI_HANDLE hWin);
typedef struct tagWIN_HANDLE_ARRAY_S
{
    HI_U32 u32WinNumber;
    HI_HANDLE ahWinHandle[DEF_MAX_WIN_NUM_ON_SINGLE_DISP];
}WIN_HANDLE_ARRAY_S;

HI_S32 Win_DebugGetHandle(HI_DRV_DISPLAY_E enDisp, WIN_HANDLE_ARRAY_S *pstWin);


HI_U32 WinGetPrefix(HI_U32 u32WinIndex);
HI_U32 WinGetDispId(HI_U32 u32WinIndex);
HI_U32 WinGetId(HI_U32 u32WinIndex);


#define WIN_PROC_BUFFER_MAX_NUMBER 32
#define WIN_PROC_DEBUG_FRAME_RECORD_NUMBER 32
typedef struct tagWINBUF_STATE_S
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
    }stNode[WIN_PROC_BUFFER_MAX_NUMBER];

    struct {
        HI_U32 u32RecordNumber;

        HI_U32 u32InputFrameID[WIN_PROC_DEBUG_FRAME_RECORD_NUMBER];
        HI_U32 u32InputPos;
        HI_U32 u32Input;

        HI_U32 u32CfgFrameID[WIN_PROC_DEBUG_FRAME_RECORD_NUMBER];
        HI_U32 u32CfgPos;
        HI_U32 u32Config;

        HI_U32 u32RlsFrameID[WIN_PROC_DEBUG_FRAME_RECORD_NUMBER];
        HI_U32 u32RlsPos;
        HI_U32 u32Release;

        HI_U32 u32TryQueueFrame;
        HI_U32 u32QueueFrame;
        HI_U32 u32Underload;
        HI_U32 u32Disacard;
    }stRecord;

    HI_U32 u32EmptyArray[WIN_PROC_BUFFER_MAX_NUMBER];
    HI_U32 u32FullArray[WIN_PROC_BUFFER_MAX_NUMBER];

    HI_DRV_VIDEO_FRAME_S stCurrentFrame;
}WINBUF_STATE_S;

#if 1
typedef struct hiWIN_RROC_FOR_DEVELOPER_S
{
    /*sr dci location in layer, for debug use.*/
    VIDEO_LAYER_PROC_S stSrDciPhysicalInfo;
    /*the current win num, if > 2, sr not open.*/
    HI_U32 u32WinNum;

    /*only in 4k, sr can be opened now.*/
    HI_RECT_S eCurrentFmt;
    /*when scaled down ,sr should not be opened.*/
    HI_BOOL bExistScaleDown_WhenRatioRevise;
    /*whether in 3d or not.*/
    HI_BOOL bIn3DMode;

    /*sr effect activated or not;*/
    HI_BOOL bHorSrOpenInPreProcess;
    HI_BOOL bVerSrOpenInPreProcess;

    HI_BOOL bHorSrOpenInPostProcess;
    HI_BOOL bVerSrOpenInPostProcess;


    /*frame size.*/
    HI_RECT_S stOringinFrameSize;
    HI_RECT_S stFinalWinOutputSize;
    HI_RECT_S stOutputSizeOfV0;

    HI_RECT_S stVdpRequire;
    HI_RECT_S stVpssGive;

    HI_RECT_S stSrOutputSize;

    /*the information of dci.*/
    HI_BOOL   bDciOpen;
    HI_RECT_S stOriginDCIPositionInFrame;
    HI_RECT_S stDciFrameSize;

    /*dci input size, the finale position should give a revise.*/
    HI_RECT_S stDciEffecttiveContentInputSize;
    HI_RECT_S stWinFinalPosition;

    /*for dcmp alg debug.*/
    HI_BOOL  bLayerOfWinHasDcmpCapbility;
    HI_BOOL  bDcmpExpectbyWin;
    HI_BOOL  bCmpVpssGive;

    /*for 10bit bitwidth debug.*/
    HI_U32   u32BitWidthSupportbyLayer;
    HI_U32   u32ExpectBitWidthFromVpss;
    HI_U32   u32BitWidthVpssGive;

}HI_WIN_RROC_FOR_DEVELOPER_S;
#endif

typedef struct tagWIN_PROC_INFO_S
{
    HI_DRV_WIN_TYPE_E enType;
    HI_U32  u32Index;
    HI_U32  u32Zorder;
    HI_U32  u32LayerId;
    HI_U32  u32LayerRegionNo;

    HI_BOOL bEnable;
    HI_BOOL bMasked;
    HI_U32  u32WinState;

    HI_BOOL bReset;
    HI_DRV_WIN_SWITCH_E enResetMode;
    HI_DRV_WIN_SWITCH_E enFreezeMode;

    HI_BOOL bQuickMode;
    HI_BOOL bStepMode;
    HI_BOOL bVirtualCoordinate;

    /*just for dcmp and mute */
    volatile HI_BOOL bDcmpEnable;
    volatile HI_BOOL bMute;

    /* not change when window lives */
    /* source info */
    HI_U32 hSrc;
    HI_VOID* pfAcqFrame;
    HI_VOID* pfRlsFrame;
    HI_VOID* pfSendWinInfo;

    /* attribute */
    //HI_DISP_DISPLAY_INFO_S stRefDispInfo;
    HI_DRV_WIN_ATTR_S stAttr;
    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;

    /* slave window for HD&SD display the same content at the same time */
    HI_HANDLE hSlvWin;

    /* debug info */
    HI_BOOL bDebugEn;
    HI_U32 u32TBNotMatchCount;

    /* statistic info */
    //WIN_STATISTIC_S stStatistic;
    HI_U32 u32ULSIn;
    HI_U32 u32ULSOut;
    HI_U32 u32UnderLoad;
    /*  buffer state */
    WINBUF_STATE_S stBufState;

    HI_DRV_LOWDELAY_STAT_INFO_S stLowdelayStat;
    
    /*only for developer or mainteiner.*/
    HI_WIN_RROC_FOR_DEVELOPER_S stWinInfoForDeveloper;
}WIN_PROC_INFO_S;

HI_S32 WinGetProcIndex(HI_HANDLE hWin, HI_U32 *p32Index);
HI_S32 WinGetProcInfo(HI_HANDLE hWin, WIN_PROC_INFO_S *pstInfo);

HI_S32 WinGetCurrentImg(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 WinCaptureFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U32 *stMMZPhyAddr, HI_U32 *stMMZlen);
HI_S32 WinReleaseCaptureFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 WinFreeCaptureMMZBuf(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *cap_frame);
HI_S32 WinCapturePause(HI_HANDLE hWin, HI_BOOL bCaptureStart);
HI_S32 WinReleaseStillFrame(HI_DRV_VIDEO_FRAME_S *pStillFrameInfo);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_WINDOW_H__  */

