/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_ext.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_EXT_H__
#define __DRV_WIN_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_drv_win.h"
#include "hi_drv_dev.h"

typedef struct
{
    HI_HANDLE hWin;
    HI_HANDLE hSlvWin;
}WIN_PRIV_STATE_S;
typedef HI_S32 (* FN_WIN_Init)(HI_VOID);
typedef HI_S32 (* FN_WIN_DeInit)(HI_VOID);
typedef HI_S32 (* FN_WIN_Create)(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWin);
typedef HI_S32 (* FN_WIN_Destroy)(HI_HANDLE hWin);
typedef HI_S32 (* FN_WIN_SetAttr)(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);
typedef HI_S32 (* FN_WIN_GetAttr)(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);

//get info for source
typedef HI_S32 (* FN_WIN_GetInfo)(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo);
typedef HI_S32 (* FN_WIN_SetSource)(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc);
typedef HI_S32 (* FN_WIN_GetSource)(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc);
typedef HI_S32 (* FN_WIN_SetEnable)(HI_HANDLE hWin, HI_BOOL bEnable);
typedef HI_S32 (* FN_WIN_GetEnable)(HI_HANDLE hWin, HI_BOOL *pbEnable);
typedef HI_S32 (* FN_WIN_QueueFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);
typedef HI_S32 (* FN_WIN_QueueUselessFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);
typedef HI_S32 (* FN_WIN_DequeueFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);


typedef HI_S32 (* FN_WIN_GetPlayInfo)(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S *pstInfo);

typedef HI_S32 (* FN_WIN_SetZorder)(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E enZFlag);
typedef HI_S32 (* FN_WIN_GetZorder)(HI_HANDLE hWin, HI_U32 *pu32Zorder);
typedef HI_S32 (* FN_WIN_Freeze)(HI_HANDLE hWin, HI_BOOL bEnable,  HI_DRV_WIN_SWITCH_E eRst);
typedef HI_S32 (* FN_WIN_Reset)(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E eRst);
typedef HI_S32 (* FN_WIN_Pause)(HI_HANDLE hWin, HI_BOOL bEnable);

typedef HI_S32 (* FN_WIN_SetStepMode)(HI_HANDLE hWin, HI_BOOL bStepMode);
typedef HI_S32 (* FN_WIN_SetStepPlay)(HI_HANDLE hWin);
/* only for virtual window */
typedef HI_S32 (* FN_WIN_SetExtBuffer)(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf);
typedef HI_S32 (* FN_WIN_AcquireFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
typedef HI_S32 (* FN_WIN_ReleaseFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
//todo
typedef HI_S32 (* FN_WIN_Set3DMode)(HI_HANDLE hWin, HI_BOOL b3DEnable,HI_DRV_DISP_STEREO_E eMode);

typedef HI_S32 (* FN_WIN_SetQuick)(HI_HANDLE hWin, HI_BOOL bEnable);

typedef HI_S32 (* FN_WIN_CapturePicture)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);
typedef HI_S32 (* FN_WIN_CapturePictureRelease)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);

typedef HI_S32 (* FN_WIN_SetRotation)(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E enRotation);
typedef HI_S32 (* FN_WIN_GetRotation)(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E *penRotation);
typedef HI_S32 (* FN_WIN_SetFlip)(HI_HANDLE hWin, HI_BOOL bHoriFlip, HI_BOOL bVertFlip);
typedef HI_S32 (* FN_WIN_GetFlip)(HI_HANDLE hWin, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip);

typedef HI_S32 (* FN_WIN_SendFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
typedef HI_S32 (* FN_WIN_GetLatestFrameInfo)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *frame_info);
typedef HI_S32 (*FN_WIN_UpdatePqData)(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);
typedef HI_S32 (*FN_WIN_Suspend)(PM_BASEDEV_S *pdev, pm_message_t state);
typedef HI_S32 (*FN_WIN_Resume)(PM_BASEDEV_S *pdev);
typedef HI_S32 (*FN_WIN_SetVirAttr)(HI_HANDLE hWin, 
                        HI_U32 u32Width,
                        HI_U32 u32Height,
                        HI_U32 u32FrmRate);
typedef struct
{
    FN_WIN_Init           pfnWinInit;
    FN_WIN_DeInit         pfnWinDeInit;
    FN_WIN_Create         pfnWinCreate;
    FN_WIN_Destroy        pfnWinDestory;
    FN_WIN_SetAttr        pfnWinSetAttr;
    FN_WIN_GetAttr        pfnWinGetAttr;
    FN_WIN_GetInfo        pfnWinGetInfo;
    FN_WIN_SetSource      pfnWinSetSrc;
    FN_WIN_GetSource      pfnWinGetSrc;
    FN_WIN_SetEnable      pfnWinSetEnable;
    FN_WIN_GetEnable      pfnWinGetEnable;
    FN_WIN_QueueFrame     pfnWinQueueFrm;
    FN_WIN_QueueUselessFrame   pWinQueueUselessFrm;
    FN_WIN_DequeueFrame   pfnWinDequeueFrm;
    FN_WIN_GetPlayInfo    pfnWinGetPlayInfo;
    FN_WIN_SetZorder      pfnWinSetZorder;
    FN_WIN_GetZorder      pfnWinGetZorder;
    FN_WIN_Freeze         pfnWinFreeze;
    FN_WIN_Reset          pfnWinReset;
    FN_WIN_Pause          pfnWinPause;
    FN_WIN_SetStepMode    pfnWinSetStepMode;
    FN_WIN_SetStepPlay    pfnWinSetStepPlay;
    FN_WIN_SetExtBuffer   pfnWinSetExtBuffer;
    FN_WIN_AcquireFrame   pfnWinAcquireFrm;
    FN_WIN_ReleaseFrame   pfnWinRlsFrm;
    FN_WIN_Set3DMode      pfnWin3DMode;
    FN_WIN_SetQuick       pfnWinSetQuik;
    FN_WIN_CapturePicture pfnWinCapturePic;
    FN_WIN_CapturePictureRelease pfnWinCapturePicRls;
    FN_WIN_SetRotation    pfnWinSetRotation;
    FN_WIN_GetRotation    pfnWinGetRotation;
    FN_WIN_SetFlip        pfnWinSetFlip;
    FN_WIN_GetFlip        pfnWinGetFlip;
    FN_WIN_SendFrame      pfnWinSendFrm;
    FN_WIN_GetLatestFrameInfo pfnWinGetLatestFrameInfo;
    FN_WIN_UpdatePqData        pfnWinUpdatePqData;	
    FN_WIN_Resume         pfnWinResume;
    FN_WIN_Suspend        pfnWinSuspend;
    FN_WIN_SetVirAttr     pfnWinSetVirAttr;
}WIN_EXPORT_FUNC_S;

HI_S32  DRV_WIN_Register(HI_VOID);
HI_VOID DRV_WIN_UnRegister(HI_VOID);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /* __VO_EXT_H__ */

