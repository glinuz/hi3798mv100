
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_cast.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_CAST_H__
#define __DRV_DISP_CAST_H__

#include "drv_disp_com.h"
#include "drv_disp_hal.h"
#include "drv_disp_buffer.h"
#include "drv_disp_isr.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define DISP_CAST_MIN_W 320
#define DISP_CAST_MAX_W 1920
#define DISP_CAST_MIN_H 240
#define DISP_CAST_MAX_H 1080

#define DISP_CAST_BUFFER_MIN_NUMBER 3
#define DISP_CAST_BUFFER_MAX_NUMBER 16

#define DISP_CAST_LOWDLY_THRESHOLD_NUMERATOR  50
#define DISP_CAST_LOWDLY_THRESHOLD_DENOMINATOR 100
#define DISP_CAST_LOWDLY_LINENUM_INTERVAL     10
/* in ms */
#define DISP_CAST_MAX_FRAME_RATE 3000

#define CAST_BUFFER_STATE_IN_CFGLIST_WRITING      1
#define CAST_BUFFER_STATE_IN_CFGLIST_WRITE_FINISH 2



typedef enum hiCAST_RETRIVE_LIST_NODE_STATUS_E
{
    CAST_RETRIVE_NODE_EMPTY = 0,
    CAST_RETRIVE_NODE_REALLOCATE,
    CAST_RETRIVE_NODE_READY_TO_RETURN,
    CAST_RETRIVE_NODE_BUTT,
} CAST_RETRIVE_LIST_NODE_STATUS_E;

typedef struct tagDISP_CAST_ATTR_S
{
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */

	HI_RECT_S stIn;
	HI_BOOL bInterlace;
	HI_U32 u32InRate;
    HI_DRV_COLOR_SPACE_E eInColorSpace;

	HI_RECT_S stOut;
	HI_U32 u32OutRate;
    HI_DRV_COLOR_SPACE_E eOutColorSpace;

	// store output informaiton
	HI_DRV_VIDEO_FRAME_S stFrameDemo;

    HI_BOOL               bCrop;
    HI_DRV_CROP_RECT_S    stCropRect;
}DISP_CAST_ATTR_S;

typedef struct tagDISP_SNAPSHOT_PRIV_FRAME_S
{
    HI_U32 u32Magic;                  /* NOTICE!!!sizeof(HI_DRV_VIDEO_PRIVATE_S)=288 bytes,however *
                                       * user space only copy 64*4=256 bytes.so put u32Magic ahead.*/
    HI_DRV_VIDEO_PRIVATE_S stPrivInfo;
}DISP_SNAPSHOT_PRIV_FRAME_S;

typedef enum
{
    MIRA_SET_CREATE_PTS = 0,
    MIRA_SET_AQUIRE_PTS,
    MIRA_FLAG_BUTT
}MIRA_GET_PTS_E;

typedef struct tagDISP_Attach_PAIR_S
{
    /*push mode, get the func ptr from back mode.*/
    HI_HANDLE hSink;
    HI_VOID* pfnQueueFrm;
    HI_VOID* pfnDequeueFrame;
}DISP_Attach_PAIR_S;


#define DISPLAY_ATTACH_CNT_MAX   3

typedef struct tagDISP_CAST_S
{
    //state
    HI_BOOL bOpen;
    HI_BOOL bEnable;
    HI_BOOL bMasked;
    atomic_t bBufBusy;
    HI_BOOL bAttached;
	HI_U32 u32Ref;

    HI_BOOL bLowDelay;
	/* wbc controll between cast and snapshot. */
	/* Should cast shedule wbc? default no.  */
	volatile HI_BOOL	bScheduleWbc;
	volatile HI_BOOL	bScheduleWbcStatus;
    // cfg
    HI_DRV_DISP_CAST_CFG_S stConfig;

    // disp info
    HI_DRV_DISPLAY_E eDisp;
    //HI_BOOL bDispSet;
    //HI_BOOL bDispUpdate;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_BOOL bToGetDispInfo;

    // private attr
    DISP_WBC_E eWBC;
    //HI_BOOL bAttrUpdate;
    DISP_CAST_ATTR_S stAttr;

    HI_U32 u32Periods;
    HI_U32 u32TaskCount;

    //mirrorcast
    //DISP_MIRACAST_S stMrCt;

    //algrithm operation

    // buffer
    BUF_POOL_S stBP;
    HI_U32 u32FrameCnt;
    HI_U32 u32LastCfgBufId;
    HI_U32 u32LastFrameBufId;

    //component operation
    DISP_INTF_OPERATION_S stIntfOpt;

    DISP_Attach_PAIR_S attach_pairs[DISPLAY_ATTACH_CNT_MAX];
    struct task_struct  *kThreadReleaseFrame;

    /*for cast proc infor stastics.*/
    HI_U32 u32CastAcquireTryCnt;
    HI_U32 u32CastAcquireOkCnt;
    HI_U32 u32CastReleaseTryCnt;
    HI_U32 u32CastReleaseOkCnt;

    HI_U32 u32CastIntrCnt;
}DISP_CAST_S;


typedef struct tagDISP_SNAPSHOT_S
{
    HI_BOOL bWork;
    BUF_POOL_S stBP;
}DISP_SNAPSHOT_S;


typedef struct tagCAST_RELEASE_PTR_S
{
    atomic_t  atReleaseNodeStatus;
    HI_U32   u32BufID;
    HI_BOOL  bInternalRelease;
}CAST_RELEASE_PTR_S;

HI_S32 DISP_CastCreate(HI_DRV_DISPLAY_E enDisp,
                       HI_DISP_DISPLAY_INFO_S *pstInfo,
                       HI_DRV_DISP_CAST_CFG_S *pstCfg,
                       DISP_CAST_S** cast_ptr);
HI_S32 DISP_CastDestroy(HI_VOID* hCast);
HI_S32 DISP_CastSetEnable(HI_VOID* cast_ptr, HI_BOOL bEnable);
HI_S32 DISP_CastGetEnable(HI_VOID* cast_ptr, HI_BOOL *pbEnable);
HI_S32 DISP_CastAcquireFrame(DISP_CAST_S* hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 DISP_CastReleaseFrame(DISP_CAST_S* hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 DISP_Cast_AttachSink(DISP_CAST_S* cast_ptr, HI_HANDLE hSink);
HI_S32 DISP_Cast_DeAttachSink(DISP_CAST_S* cast_ptr, HI_HANDLE hSink);

HI_S32 DISP_Cast_SetAttr(DISP_CAST_S * cast_ptr, HI_DRV_DISP_Cast_Attr_S *castAttr);
HI_S32 DISP_Cast_GetAttr(DISP_CAST_S * cast_ptr, HI_DRV_DISP_Cast_Attr_S *castAttr);

HI_S32 DISP_Acquire_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_VOID **snapshotHandle, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 DISP_Release_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_VOID* snapshotHandle, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 DISP_SnapshotDestroy(HI_VOID* snapshot_ptr);


HI_VOID DISP_CastCBSetDispMode(DISP_CAST_S * hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID DISP_CastCBWork(HI_HANDLE hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);

HI_VOID DISP_CastCB_GenarateFrame(HI_VOID *cast_ptr, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID DISP_CastPushFrame(HI_VOID *cast_ptr, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID DISP_CastGetDlyStatus(HI_HANDLE cast_ptr, HI_BOOL *pbLowDly);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_X_H__  */










