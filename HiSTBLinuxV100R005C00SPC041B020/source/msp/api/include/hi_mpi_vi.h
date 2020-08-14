/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_mpi_vi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

 *******************************************************************************/

#ifndef __HI_MPI_VI_H__
#define __HI_MPI_VI_H__

#include "drv_vi_ioctl.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
 #endif
#endif

#define MIN_VI_FB_NUM 4
#define MAX_VI_FB_NUM 16

HI_S32         HI_MPI_VI_Init(HI_VOID);
HI_S32         HI_MPI_VI_DeInit(HI_VOID);
HI_S32         HI_MPI_VI_Create(HI_UNF_VI_E enViPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phVi);
HI_S32         HI_MPI_VI_Destroy(HI_HANDLE handle);
HI_S32         HI_MPI_VI_SetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr);
HI_S32         HI_MPI_VI_GetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr);
HI_S32         HI_MPI_VI_Attach(HI_HANDLE hVi, HI_HANDLE hDst);
HI_S32         HI_MPI_VI_Detach(HI_HANDLE hVi, HI_HANDLE hDst);
HI_S32         HI_MPI_VI_SetExternBuffer(HI_HANDLE handle, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr);
HI_S32         HI_MPI_VI_Start(HI_HANDLE handle);
HI_S32         HI_MPI_VI_Stop(HI_HANDLE handle);
HI_S32         HI_MPI_VI_QueueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
HI_S32         HI_MPI_VI_DequeueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
HI_S32         HI_MPI_VI_AcquireFrame(HI_HANDLE handle, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs);
HI_S32         HI_MPI_VI_ReleaseFrame(HI_HANDLE handle, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__HI_MPI_VI_H__
