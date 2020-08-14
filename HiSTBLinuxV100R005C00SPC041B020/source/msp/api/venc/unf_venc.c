/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_venc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/08
  Description   :
  History       :
  1.Date        : 2009/04/08
    Author      : j00131665
    Modification: Created file
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_mpi_venc.h"

HI_S32 HI_UNF_VENC_Init(HI_VOID)
{
    return HI_MPI_VENC_Init();
}

HI_S32 HI_UNF_VENC_DeInit(HI_VOID)
{
    return HI_MPI_VENC_DeInit();
}

HI_S32 HI_UNF_VENC_Create(HI_HANDLE* phVencChn, const HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    return HI_MPI_VENC_Create(phVencChn, pstAttr);
}

HI_S32 HI_UNF_VENC_Destroy(HI_HANDLE hVencChn)
{
    return HI_MPI_VENC_Destroy(hVencChn);
}

HI_S32 HI_UNF_VENC_AttachInput(HI_HANDLE hVencChn, HI_HANDLE hSrc)
{
    return HI_MPI_VENC_AttachInput(hVencChn, hSrc);
}

HI_S32 HI_UNF_VENC_DetachInput(HI_HANDLE hVencChn)
{
    return HI_MPI_VENC_DetachInput(hVencChn);
}

HI_S32 HI_UNF_VENC_AcquireStream(HI_HANDLE hVencChn, HI_UNF_VENC_STREAM_S* pstStream, HI_U32 u32TimeoutMs)
{
    return HI_MPI_VENC_AcquireStream(hVencChn, pstStream, u32TimeoutMs);
}

HI_S32 HI_UNF_VENC_ReleaseStream(HI_HANDLE hVencChn, const HI_UNF_VENC_STREAM_S* pstStream)
{
    return HI_MPI_VENC_ReleaseStream(hVencChn, pstStream);
}

HI_S32 HI_UNF_VENC_Start(HI_HANDLE hVencChn)
{
    return HI_MPI_VENC_Start(hVencChn);
}

HI_S32 HI_UNF_VENC_Stop(HI_HANDLE hVencChn)
{
    return HI_MPI_VENC_Stop(hVencChn);
}

HI_S32 HI_UNF_VENC_SetAttr(HI_HANDLE hVencChn, const HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    return HI_MPI_VENC_SetAttr(hVencChn, pstAttr);
}

HI_S32 HI_UNF_VENC_GetAttr(HI_HANDLE hVencChn, HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    return HI_MPI_VENC_GetAttr(hVencChn, pstAttr);
}

HI_S32 HI_UNF_VENC_RequestIFrame(HI_HANDLE hVencChn)
{
    return HI_MPI_VENC_RequestIFrame(hVencChn);
}

HI_S32 HI_UNF_VENC_QueueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo)
{
    return HI_MPI_VENC_QueueFrame(hVenc, pstFrameinfo);
}

HI_S32 HI_UNF_VENC_DequeueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo)
{
    return HI_MPI_VENC_DequeueFrame(hVenc, pstFrameinfo);
}

HI_S32 HI_UNF_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    if (NULL == pstAttr)
    {
        return HI_ERR_VENC_NULL_PTR;
    }

    pstAttr->enVencType       = HI_UNF_VCODEC_TYPE_H264;
    pstAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_720P;
    pstAttr->enVencProfile    = HI_UNF_H264_PROFILE_HIGH;
    pstAttr->u32RotationAngle = 0;
    pstAttr->bSlcSplitEn      = HI_FALSE;
    pstAttr->u32SplitSize    = 10*1024;
    pstAttr->u32StrmBufSize   = 1280 * 720 * 2;

    pstAttr->u32Width         = 1280;
    pstAttr->u32Height        = 720;
    pstAttr->u32TargetBitRate = 4 * 1024 * 1024;
    pstAttr->u32InputFrmRate  = 25;
    pstAttr->u32TargetFrmRate = 25;
    pstAttr->u32Gop           = 100;
    pstAttr->u32MaxQp         = 48;
    pstAttr->u32MinQp         = 16;
    pstAttr->bQuickEncode     = HI_FALSE;
    pstAttr->u32DriftRateThr  = (-1);
    pstAttr->u8Priority       = 0;
    pstAttr->u32Qlevel        = 0;
    return HI_SUCCESS;
}

//eof

