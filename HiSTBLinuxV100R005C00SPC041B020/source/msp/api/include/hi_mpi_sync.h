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
#ifndef __HI_MPI_SYNC_H__
#define __HI_MPI_SYNC_H__

#include "hi_drv_sync.h"
#include "drv_sync_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
	extern "C"
	{
#endif
#endif

HI_S32 HI_MPI_SYNC_Init(HI_VOID);
HI_S32 HI_MPI_SYNC_DeInit(HI_VOID);
HI_VOID HI_MPI_SYNC_GetDefaultAttr(HI_UNF_SYNC_ATTR_S *pstSyncAttr);
HI_S32 HI_MPI_SYNC_Create(HI_UNF_SYNC_ATTR_S *pstSyncAttr, HI_HANDLE *phSync);
HI_S32 HI_MPI_SYNC_Destroy(HI_HANDLE hSync);
HI_S32 HI_MPI_SYNC_SetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr);
HI_S32 HI_MPI_SYNC_GetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr);
HI_S32 HI_MPI_SYNC_Start(HI_HANDLE hSync, SYNC_CHAN_E enChn);
HI_S32 HI_MPI_SYNC_Stop(HI_HANDLE hSync, SYNC_CHAN_E enChn);
HI_S32 HI_MPI_SYNC_Play(HI_HANDLE hSync);
HI_S32 HI_MPI_SYNC_Seek(HI_HANDLE hSync, HI_U32 u32SeekPts);
HI_S32 HI_MPI_SYNC_Pause(HI_HANDLE hSync);
HI_S32 HI_MPI_SYNC_Tplay(HI_HANDLE hSync);
HI_S32 HI_MPI_SYNC_Resume(HI_HANDLE hSync);
HI_S32 HI_MPI_SYNC_SetBufState(HI_HANDLE hSync, SYNC_BUF_STATUS_S stBufStatus);
HI_S32 HI_MPI_SYNC_AudJudge(HI_HANDLE hSync, SYNC_AUD_INFO_S *pAudInfo, SYNC_AUD_OPT_S *pAudOpt);
HI_S32 HI_MPI_SYNC_GetStatus(HI_HANDLE hSync, HI_UNF_SYNC_STATUS_S *pSyncStatus);
HI_S32 HI_MPI_SYNC_SetDDPTestMode(HI_HANDLE hSync, HI_BOOL bEnable);
HI_S32 HI_MPI_SYNC_CheckNewEvent(HI_HANDLE hSync, SYNC_EVENT_S *pstEvent);

HI_S32 HI_MPI_SYNC_SetExtInfo(HI_HANDLE hSync, SYNC_EXT_INFO_E enExtInfo, HI_VOID *pData);

HI_S32 HI_MPI_SYNC_VidJudge(HI_HANDLE hSync, SYNC_VID_INFO_S *pVidInfo, SYNC_VID_OPT_S *pVidOpt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
