/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_drv_vi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/18
  Description   :
  History       :
  1.Date        : 2013/04/18
    Author      : z00184432
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_VI_H__
#define __HI_DRV_VI_H__

#include "hi_debug.h"
#include "hi_module.h"
#include "drv_vi_ioctl.h"
#include "hi_unf_vi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define HI_FATAL_VI(fmt...) HI_FATAL_PRINT(HI_ID_VI, fmt)
#define HI_ERR_VI(fmt...) HI_ERR_PRINT(HI_ID_VI, fmt)
#define HI_WARN_VI(fmt...) HI_WARN_PRINT(HI_ID_VI, fmt)
#define HI_INFO_VI(fmt...) HI_INFO_PRINT(HI_ID_VI, fmt)
#define HI_DEBUG_VI(fmt...) HI_DBG_PRINT(HI_ID_VI, fmt)

HI_S32         HI_DRV_VI_Create(VI_CREATE_S *pstCreate, HI_VOID *file);
HI_S32         HI_DRV_VI_Destroy(HI_HANDLE hVi);
HI_S32         HI_DRV_VI_SetAttr(HI_HANDLE hVi, HI_UNF_VI_ATTR_S *pstAttr);
HI_S32         HI_DRV_VI_SetExtBuf(HI_UNF_VI_E enViPort, HI_UNF_VI_BUFFER_ATTR_S *pstBufAttr);
HI_S32         HI_DRV_VI_CreateVpssPort(HI_HANDLE hVi, VI_VPSS_PORT_S *pstVpssPort);
HI_S32         HI_DRV_VI_DestroyVpssPort(HI_HANDLE hVi, VI_VPSS_PORT_S *pstVpssPort);
HI_S32         HI_DRV_VI_Suspend(HI_VOID);
HI_S32         HI_DRV_VI_Resume(HI_VOID);
HI_S32         HI_DRV_VI_Start(HI_HANDLE hVi);
HI_S32         HI_DRV_VI_Stop(HI_HANDLE hVi);
HI_S32         HI_DRV_VI_DequeueFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pstFrame);
HI_S32         HI_DRV_VI_QueueFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pstFrame);
HI_S32         HI_DRV_VI_UsrAcquireFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pstFrame, HI_U32 u32TimeoutMs);
HI_S32         HI_DRV_VI_UsrReleaseFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pstFrame);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__HI_DRV_VI_H__
