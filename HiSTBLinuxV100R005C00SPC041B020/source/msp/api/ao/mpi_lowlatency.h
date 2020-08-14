/******************************************************************************

  Copyright (C), 2011-2015, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :mpi_hiao_track.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/12
  Description  : 
  History       :
  1.Date        : 2012/12/12
    Author      : h00213218
    Modification: Created file

 *******************************************************************************/

#ifndef  __MPI_HIAO_LOWLATENCY_H__
#define  __MPI_HIAO_LOWLATENCY_H__

#include "hi_mpi_ao.h"
#include "hi_drv_ao.h"

HI_BOOL LOWLATENCY_CheckIsLowLatencyTrack(HI_HANDLE hTrack);
HI_S32  LOWLATENCY_Enable(HI_HANDLE hTrack);
HI_S32  LOWLATENCY_Disable(HI_HANDLE hTrack);
HI_S32  LOWLATENCY_SendData(HI_HANDLE hTrack,const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
HI_S32  LOWLATENCY_GetAIPDelayMs(HI_HANDLE hTrack,HI_U32 *pDelayMs);

HI_S32 SND_DMA_Create(HI_VOID);
HI_S32 SND_DMA_Destory(HI_VOID);
HI_S32 SND_DMA_SendData(const HI_UNF_AO_FRAMEINFO_S* pstAOFrame, const HI_U32 u32LatencyMs);
HI_S32 SND_DMA_GetDelayMs(HI_U32 *pu32DelayMs);

#endif
