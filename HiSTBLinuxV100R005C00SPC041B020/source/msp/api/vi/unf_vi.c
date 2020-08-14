/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_vi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/27
  Description   :
  History       :
  1.Date        : 2009/03/27
    Author      : j00131665
    Modification: Created file
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_mpi_vi.h"

HI_S32 HI_UNF_VI_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Init();

    return s32Ret;
}

HI_S32 HI_UNF_VI_DeInit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_DeInit();

    return s32Ret;
}

HI_S32 HI_UNF_VI_GetDefaultAttr(HI_UNF_VI_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        return HI_ERR_VI_NULL_PTR;
    }
    memset(pstAttr, 0, sizeof(HI_UNF_VI_ATTR_S));
    pstAttr->bVirtual = HI_TRUE;
    pstAttr->enInputMode = HI_UNF_VI_MODE_BT656_576I;
    pstAttr->stInputRect.s32X = 0;
    pstAttr->stInputRect.s32Y = 0;
    pstAttr->stInputRect.s32Width  = 720;
    pstAttr->stInputRect.s32Height = 576;
    pstAttr->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
    pstAttr->enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
    pstAttr->u32BufNum = 6;
	
    return HI_SUCCESS;
}

HI_S32 HI_UNF_VI_Create(HI_UNF_VI_E enViPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phandle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Create(enViPort, pstAttr, phandle);

    return s32Ret;
}

HI_S32 HI_UNF_VI_Destroy(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Destroy(handle);

    return s32Ret;
}

HI_S32 HI_UNF_VI_SetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_SetAttr(handle, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VI_GetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_GetAttr(handle, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VI_SetExternBuffer(HI_HANDLE handle, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_SetExternBuffer(handle, pstBufAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VI_Start(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Start(handle);

    return s32Ret;

}

HI_S32 HI_UNF_VI_Stop(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Stop(handle);

    return s32Ret;

}

HI_S32 HI_UNF_VI_QueueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_QueueFrame(hVI, pFrameInfo);

    return s32Ret;
}

HI_S32 HI_UNF_VI_DequeueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_DequeueFrame(hVI, pFrameInfo);

    return s32Ret;
}

HI_S32 HI_UNF_VI_AcquireFrame(HI_HANDLE handle, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_AcquireFrame(handle, pFrameInfo, u32TimeoutMs);

    return s32Ret;
}

HI_S32 HI_UNF_VI_ReleaseFrame(HI_HANDLE handle, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_ReleaseFrame(handle, pFrameInfo);

    return s32Ret;
}

