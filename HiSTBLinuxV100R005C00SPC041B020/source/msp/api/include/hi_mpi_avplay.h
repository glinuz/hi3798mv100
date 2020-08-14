/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_avplay.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
  1.Date        : 2009/12/21
    Author      : w58735
    Modification: Created file

*******************************************************************************/
#ifndef __HI_MPI_AVPLAY_H__
#define __HI_MPI_AVPLAY_H__

#include "hi_unf_avplay.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 HI_MPI_AVPLAY_Init(HI_VOID);
HI_S32 HI_MPI_AVPLAY_DeInit(HI_VOID);
HI_S32 HI_MPI_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg);
HI_S32 HI_MPI_AVPLAY_RegisterAcodecLib(const HI_CHAR *pFileName);
HI_S32 HI_MPI_AVPLAY_RegisterVcodecLib(const HI_CHAR *pFileName);
HI_S32 HI_MPI_AVPLAY_FoundSupportDeoder(const HA_FORMAT_E enFormat,HI_U32 * penDstCodecID);
HI_S32 HI_MPI_AVPLAY_ConfigAcodec( const HI_U32 enDstCodecID, HI_VOID *pstConfigStructure);
HI_S32 HI_MPI_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay);
HI_S32 HI_MPI_AVPLAY_Destroy(HI_HANDLE hAvplay);
HI_S32 HI_MPI_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara);
HI_S32 HI_MPI_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_MPI_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_MPI_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_MPI_AVPLAY_DecodeIFrame(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_I_FRAME_S *pstIframe, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture);
HI_S32 HI_MPI_AVPLAY_ReleaseIFrame(HI_HANDLE hAvplay, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture);
HI_S32 HI_MPI_AVPLAY_SetDecodeMode(HI_HANDLE hAvplay, HI_UNF_VCODEC_MODE_E enDecodeMode);

#ifndef __aarch64__
HI_S32 HI_MPI_AVPLAY_RegisterEvent(HI_HANDLE      hAvplay,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN pfnEventCB);
#endif

HI_S32 HI_MPI_AVPLAY_RegisterEvent64(HI_HANDLE      hAvplay,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN64 pfnEventCB);
HI_S32 HI_MPI_AVPLAY_UnRegisterEvent(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent);
HI_S32 HI_MPI_AVPLAY_PreStart(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_MPI_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_MPI_AVPLAY_PreStop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn,const HI_UNF_AVPLAY_PRESTOP_OPT_S *pPreStopOpt);
HI_S32 HI_MPI_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pStop);
HI_S32 HI_MPI_AVPLAY_Pause(HI_HANDLE hAvplay);
HI_S32 HI_MPI_AVPLAY_Tplay(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_TPLAY_OPT_S *pstTplayOpt);
HI_S32 HI_MPI_AVPLAY_Resume(HI_HANDLE hAvplay);
HI_S32 HI_MPI_AVPLAY_Reset(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESET_OPT_S *pstResetOpt);
HI_S32 HI_MPI_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs);

HI_S32 HI_MPI_AVPLAY_PutBuf(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                                       HI_U32 u32ValidDataLen, HI_U32 u32Pts, HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pstExOpt);

HI_S32 HI_MPI_AVPLAY_GetSyncVdecHandle(HI_HANDLE hAvplay, HI_HANDLE *phVdec, HI_HANDLE *phSync);
HI_S32 HI_MPI_AVPLAY_GetSndHandle(HI_HANDLE hAvplay, HI_HANDLE *phSnd);
HI_S32 HI_MPI_AVPLAY_GetWindowHandle(HI_HANDLE hAvplay, HI_HANDLE *phWindow);
HI_S32 HI_MPI_AVPLAY_AttachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow);
HI_S32 HI_MPI_AVPLAY_DetachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow);
HI_S32 HI_MPI_AVPLAY_SetWindowRepeat(HI_HANDLE hAvplay, HI_U32 u32Repeat);
HI_S32 HI_MPI_AVPLAY_AttachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd);
HI_S32 HI_MPI_AVPLAY_DetachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd);
HI_S32 HI_MPI_AVPLAY_GetDmxAudChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxAudChn);
HI_S32 HI_MPI_AVPLAY_GetDmxVidChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxVidChn);
HI_S32 HI_MPI_AVPLAY_GetStatusInfo(HI_HANDLE hAvplay, HI_UNF_AVPLAY_STATUS_INFO_S *pstStatusInfo);
HI_S32 HI_MPI_AVPLAY_GetStreamInfo(HI_HANDLE hAvplay, HI_UNF_AVPLAY_STREAM_INFO_S *pstStreamInfo);
HI_S32 HI_MPI_AVPLAY_GetAudioSpectrum(HI_HANDLE hAvplay, HI_U16 *pSpectrum, HI_U32 u32BandNum);
HI_S32 HI_MPI_AVPLAY_IsBuffEmpty(HI_HANDLE hAvplay, HI_BOOL *pbIsEmpty);
HI_S32 HI_MPI_AVPLAY_SwitchDmxAudChn(HI_HANDLE hAvplay, HI_HANDLE hNewDmxAud, HI_HANDLE *phOldDmxAud);
HI_S32 HI_MPI_AVPLAY_PutAudPts(HI_HANDLE hAvplay, HI_U32 u32AudPts);
HI_S32 HI_MPI_AVPLAY_FlushStream(HI_HANDLE hAvplay, HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S *pstFlushOpt);
HI_S32 HI_MPI_AVPLAY_Step(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_STEP_OPT_S *pstStepOpt);
HI_S32 HI_MPI_AVPLAY_Invoke(HI_HANDLE hAvplay, HI_UNF_AVPLAY_INVOKE_E enInvokeType, HI_VOID *pPara);
HI_S32 HI_MPI_AVPLAY_AcqUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S *pstUserData, HI_UNF_VIDEO_USERDATA_TYPE_E *penType);
HI_S32 HI_MPI_AVPLAY_RlsUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S* pstUserData);
HI_S32 HI_MPI_AVPLAY_GetVidChnOpenParam(HI_HANDLE hAvplay, HI_UNF_AVPLAY_OPEN_OPT_S *pstOpenPara);

typedef struct hiMPI_EXT_BUFFER_S
{
#define HI_MPI_MAX_BUF_NUM (16)
    HI_U32  au32FrameBuffer[HI_MPI_MAX_BUF_NUM];
    HI_U32  au32MetadataBuf[HI_MPI_MAX_BUF_NUM];

    HI_S32  as32FrameBufFd[HI_MPI_MAX_BUF_NUM];
    HI_S32  as32MetadataBufFd[HI_MPI_MAX_BUF_NUM];

    HI_U32  u32BufCnt;
    HI_U32  u32FrameBufSize;
    HI_U32  u32MetadataBufSize;
    HI_U32  u32Stride;
    HI_BOOL bVideoBypass;
    HI_U32  u32PrivMetadataOffset;
} HI_MPI_EXT_BUFFER_S;

HI_S32 HI_MPI_AVPLAY_UseExternalBuffer(HI_HANDLE hAvplay, HI_MPI_EXT_BUFFER_S* pstExtBuf);
HI_S32 HI_MPI_AVPLAY_DeleteExternalBuffer(HI_HANDLE hAvplay, HI_MPI_EXT_BUFFER_S* pstExtBuf);
HI_S32 HI_MPI_AVPLAY_SetTunnelMode(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_BOOL enable);
HI_S32 HI_MPI_AVPLAY_GetAdecOutFrameInfo(HI_HANDLE hAvplay, HI_U32 *u32OutFrames, HI_U32 *u32SampleRate);
HI_S32 HI_MPI_AVPLAY_SetAdecOutBufSize(HI_HANDLE hAvplay, HI_U32 u32OutBufFrameCnt);
HI_S32 HI_MPI_AVPLAY_CalculateFRC(HI_HANDLE hAvplay, HI_UNF_VIDEO_FRAME_INFO_S* pstFrame, HI_U32 u32RefreshRate, HI_S32* ps32RepeatCnt);
HI_S32 HI_MPI_AVPLAY_GetLeftStreamFrm(HI_HANDLE hAvplay, HI_U32 *pLeftStreamFrm);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
