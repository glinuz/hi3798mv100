/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_win.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_WIN_H__
#define __HI_DRV_WIN_H__

#include "hi_drv_disp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#ifndef HI_ADVCA_FUNCTION_RELEASE
#define HI_FATAL_WIN(fmt...) \
            HI_FATAL_PRINT(HI_ID_VO, fmt)

#define HI_ERR_WIN(fmt...) \
            HI_ERR_PRINT(HI_ID_VO, fmt)

#define HI_WARN_WIN(fmt...) \
            HI_WARN_PRINT(HI_ID_VO, fmt)

#define HI_INFO_WIN(fmt...) \
            HI_INFO_PRINT(HI_ID_VO, fmt)
#else

#define HI_FATAL_WIN(fmt...)
#define HI_ERR_WIN(fmt...)
#define HI_WARN_WIN(fmt...)
#define HI_INFO_WIN(fmt...)
#endif

#define DEF_MAX_WIN_NUM_ON_SINGLE_DISP 17
#define DEF_MAX_WIN_NUM_ON_VIRTUAL_DISP 17

#define  WIN_HDR_CMD_ARGS_SIZE  128

/* window type */
typedef enum hiDRV_WIN_TYPE_E
{
    HI_DRV_WIN_ACTIVE_SINGLE = 0,
    HI_DRV_WIN_VITUAL_SINGLE,
    HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE,
    HI_DRV_WIN_ACTIVE_SLAVE,
    HI_DRV_WIN_BUTT
}HI_DRV_WIN_TYPE_E;


/* window swtich mode, in reset mode */
typedef enum hiDRV_WIN_SWITCH_E
{
    HI_DRV_WIN_SWITCH_LAST = 0,
    HI_DRV_WIN_SWITCH_BLACK = 1,
    HI_DRV_WIN_SWITCH_BUTT
} HI_DRV_WIN_SWITCH_E;


typedef enum hiMPI_WINDOW_FROST_E
{
    HI_DRV_WINDOW_FROST_CLOSE = 0x0 ,       /**<Close  */ /**<CNcomment:¹Ø±Õ CNend*/
    HI_DRV_WINDOW_FROST_LOW         ,       /**<Low    */ /**<CNcomment:Èõ   CNend*/
    HI_DRV_WINDOW_FROST_MIDDLE      ,       /**<Middle */ /**<CNcomment:ÖÐ   CNend*/
    HI_DRV_WINDOW_FROST_HIGH        ,       /**<High   */ /**<CNcomment:Ç¿   CNend*/

    HI_DRV_WINDOW_FROST_BUTT
}HI_DRV_WINDOW_FROST_E;


/* window attribute */
typedef struct hiDRV_WIN_ATTR_S
{
    HI_BOOL bVirtual;

    /* not change when window lives */
    HI_DRV_DISPLAY_E  enDisp;

    /* may change when window lives */
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    HI_DRV_ASP_RAT_MODE_E enARCvrs;

    HI_BOOL bUseCropRect;
    HI_RECT_S stInRect;
    HI_DRV_CROP_RECT_S stCropRect;

    HI_RECT_S stOutRect;

    /* only for virtual window */
    HI_BOOL             bUserAllocBuffer;
    HI_U32              u32BufNumber; /* [1,16] */
    HI_DRV_PIX_FORMAT_E enDataFormat;
    
    /* ONLY for CES exhibition,to close HDR path */
    HI_BOOL             bCloseHdrPath;
    /* debug for Hisi-self path:1->Hisi-self hdr path,0->other hdr path */
    HI_BOOL             bHisiHdr;
}HI_DRV_WIN_ATTR_S;

/* window information */
typedef struct hiDRV_WIN_INFO_S
{
    HI_DRV_WIN_TYPE_E eType;

    HI_HANDLE hPrim;
    HI_HANDLE hSec;
}HI_DRV_WIN_INFO_S;

typedef enum hiDRV_WINDOW_MODE_E
{
    HI_DRV_WINDOW_MODE_NORMAL = 0,      /* normal win mode */
    HI_DRV_WINDOW_MODE_DOLBY,           /* dolby hdr win mode,will occupy two video layers. */
    HI_DRV_WINDOW_MODE_HDR,             /* hdr win mode is the same with dolby hdr win. */
    HI_DRV_WINDOW_MODE_BUTT
}HI_DRV_WINDOW_MODE_E;

typedef struct hiDRV_DOLBY_META_INFO_S
{
    HI_U32 u32PhyAddr;
	
    HI_U32 u32CompLen;
    HI_U32 u32DmCfgLen;
    HI_U32 u32DmRegLen;
    HI_U32 u32UpMdLen;
    HI_U32 u32TotalLen;//buffer length to store these 4 structures,currently is 64KB.
	
    HI_DRV_DISP_OUT_TYPE_E enDispOutType;
}HI_DRV_DOLBY_META_INFO_S;

typedef HI_S32 (*PFN_GET_FRAME_CALLBACK)(HI_HANDLE hHd, HI_DRV_VIDEO_FRAME_S *pstFrm);
typedef HI_S32 (*PFN_PUT_FRAME_CALLBACK)(HI_HANDLE hHd, HI_DRV_VIDEO_FRAME_S *pstFrm);
typedef HI_S32 (*PFN_GET_WIN_INFO_CALLBACK)(HI_HANDLE hHd, HI_DRV_WIN_PRIV_INFO_S *pstWin);

/* source information.
   window will get / release frame or send private info to sourec
   by function pointer */
typedef struct hiDRV_WIN_SRC_INFO_S
{
    HI_HANDLE hSrc;

    PFN_GET_FRAME_CALLBACK pfAcqFrame;
    PFN_PUT_FRAME_CALLBACK pfRlsFrame;
    PFN_GET_WIN_INFO_CALLBACK pfSendWinInfo;

    HI_HANDLE hSecondSrc;//for Dolby HDR dual use.
    HI_U32    u32Resrve1;
}HI_DRV_WIN_SRC_INFO_S;

/* source information.
   window will get / release frame or send private info to sourec
   by function pointer */
typedef struct hiDRV_WIN_SRC_HANDLE_S
{
    HI_MOD_ID_E enSrcMode;
    
    HI_HANDLE hSrc;

    HI_HANDLE hSecondSrc;//for Dolby HDR dual use.
    
    HI_U32    u32Resrve1;
}HI_DRV_WIN_SRC_HANDLE_S;

/* window current play information, player gets it and adjust Audio and Video
   play rate */
typedef struct hiDRV_WIN_PLAY_INFO_S
{
    HI_U32    u32DelayTime; /* in ms */
    HI_U32    u32DispRate;  /* in 1/100 Hz */
    HI_U32    u32FrameNumInBufQn;
    HI_DRV_VIDEO_FRAME_S    newest_playframeinfo;
    HI_BOOL   bTBMatch;  /* for interlace frame display on interlace timing */
}HI_DRV_WIN_PLAY_INFO_S;

typedef struct hiDRV_WIN_INTF_S
{
    HI_VOID* pfAcqFrame;
    HI_VOID* pfRlsFrame;
    HI_VOID* pfSetWinAttr;

    HI_U32    u32Resrve0;
    HI_U32    u32Resrve1;
}HI_DRV_WIN_INTF_S;

typedef struct hiWIN_DOLBY_LIB_INFO_S
{
	HI_CHAR cBuildtime[128];
    HI_CHAR cVersion[128];
    HI_CHAR cUserName[32];
    HI_U32  u32UserID[4];
	HI_U32  bValid;
}WIN_DOLBY_LIB_INFO_S;

/* commands for debug.*/
typedef enum hiWIN_DEBUG_CMD_E
{
    WIN_DEBUG_SET_HISI_HDR_PATH = 0x0,
    WIN_DEBUG_CMD_BUTT
}WIN_DEBUG_CMD_E;

typedef struct hiWIN_DEBUG_CMD_AND_ARGS_S
{
    HI_HANDLE          hWindow;
    WIN_DEBUG_CMD_E    enDebugCmd;
    HI_U8              u8Args[WIN_HDR_CMD_ARGS_SIZE];
}WIN_DEBUG_CMD_AND_ARGS_S;


HI_S32 HI_DRV_WIN_Create(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow);
HI_S32 HI_DRV_WIN_Destroy(HI_HANDLE hWindow);
HI_S32 HI_DRV_WIN_SetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);
HI_S32 HI_DRV_WIN_GetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr);
HI_S32 HI_DRV_WIN_GetInfo(HI_HANDLE hWindow, HI_DRV_WIN_INFO_S *pInfo);
HI_S32 HI_DRV_WIN_SetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc);
HI_S32 HI_DRV_WIN_GetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc);
HI_S32 HI_DRV_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable);
HI_S32 HI_DRV_WIN_GetEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable);
HI_S32 HI_DRV_WIN_QFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 HI_DRV_WIN_QULSFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo);
HI_S32 HI_DRV_WIN_DQFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 HI_DRV_WIN_GetPlayInfo(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pInfo);
HI_S32 HI_DRV_WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E ZFlag);
HI_S32 HI_DRV_WIN_GetZorder(HI_HANDLE hWin, HI_U32 *pu32Zorder);

HI_S32 HI_DRV_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enMode);
HI_S32 HI_DRV_WIN_GetPlayInfo(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pstInfo);

HI_S32 HI_DRV_WIN_GetDelayMs(HI_HANDLE hWindow, HI_U32 *pu32DelayTime);

HI_S32 HI_DRV_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 HI_DRV_WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E ZFlag);
HI_S32 HI_DRV_WIN_Freeze(HI_HANDLE hWin, HI_BOOL bEnable,  HI_DRV_WIN_SWITCH_E eRst);
HI_S32 HI_DRV_WIN_GetLatestFrameInfo(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *frame_info);
HI_S32 HI_DRV_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enSwitch);
HI_S32 HI_DRV_WIN_Pause(HI_HANDLE hWin, HI_BOOL bEnable);
HI_S32 HI_DRV_WIN_SetStepMode(HI_HANDLE hWin, HI_BOOL bStepMode);
HI_S32 HI_DRV_WIN_SetStepPlay(HI_HANDLE hWin);
HI_S32 HI_DRV_WIN_SetExtBuffer(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf);
HI_S32 HI_DRV_WIN_AcquireFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
HI_S32 HI_DRV_WIN_ReleaseFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
HI_S32 HI_DRV_WIN_SetQuick(HI_HANDLE hWin, HI_BOOL bEnable);
HI_S32 HI_DRV_WIN_CapturePicture(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);
HI_S32 HI_DRV_WIN_CapturePictureRelease(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic);
HI_S32 HI_DRV_WIN_SetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E enRotation);
HI_S32 HI_DRV_WIN_GetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E *penRotation);
HI_S32 HI_DRV_WIN_SetFlip(HI_HANDLE hWin, HI_BOOL bHoriFlip, HI_BOOL bVertFlip);
HI_S32 HI_DRV_WIN_GetFlip(HI_HANDLE hWin, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip);
HI_S32 HI_DRV_WIN_SendFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo);
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)  
HI_S32 HI_DRV_WIN_UpdatePqData(HI_U32 u32UpdateType, PQ_PARAM_S* pstPqParam);
#endif
HI_S32 HI_DRV_WIN_Init(HI_VOID);
HI_S32 HI_DRV_WIN_DeInit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __HI_DRV_WIN_H__ */


