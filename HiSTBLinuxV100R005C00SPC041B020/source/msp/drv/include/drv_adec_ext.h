/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_ao.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/02/13
  Description   :
  History       :
  1.Date        : 2006/02/13
    Author      :
    Modification: Created file

  2.Date         : 2006/2/13
******************************************************************************/

#ifndef __HI_DRV_ADEC_H__
#define __HI_DRV_ADEC_H__

#include "hi_type.h"
#include "hi_mpi_adec.h"
#include "hi_drv_ao.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#ifdef HI_MCE_SUPPORT
#define ADEC_KERNEL_DECODE
#endif

#ifdef ADEC_KERNEL_DECODE

typedef enum hiADEC_KERNEL_TYPE_E
{
    ADEC_KEL_TPYE_MPEG,   /**<mpeg */
    ADEC_KEL_TPYE_BUTT
} ADEC_KERNEL_TYPE_E;

typedef struct hiADEC_SLIM_ATTR_S
{
    ADEC_KERNEL_TYPE_E enCodecType;
    HI_U32             u32InBufSize;                  /* Input buffer  size              */
    HI_U32             u32OutBufNum;               /* Output buffer number, buffer size depend on  u32CodecID       */
} ADEC_SLIM_ATTR_S;

HI_S32          HI_DRV_ADEC_Open(HI_HANDLE *phAdec, ADEC_SLIM_ATTR_S *pstAdecAttr);
HI_S32          HI_DRV_ADEC_Close(HI_HANDLE hAdec);
HI_S32          HI_DRV_ADEC_Reset(HI_HANDLE hAdec);
HI_S32          HI_DRV_ADEC_SetAttr(HI_HANDLE hAdec, ADEC_SLIM_ATTR_S *pstAdecAttr);
HI_S32          HI_DRV_ADEC_GetAttr(HI_HANDLE hAdec, ADEC_SLIM_ATTR_S *pstAdecAttr);
HI_S32          HI_DRV_ADEC_SendStream(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S *pstStream, HI_U32 u32PtsMs);
HI_S32          HI_DRV_ADEC_ReceiveFrame(HI_HANDLE hAdec, AO_FRAMEINFO_S *pstAOFrame,
                                         ADEC_EXTFRAMEINFO_S *pstExtInfo);
HI_S32          HI_DRV_ADEC_ReleaseFrame(HI_HANDLE hAdec, const AO_FRAMEINFO_S *pstAOFrame);
HI_S32          HI_DRV_ADEC_GetStatusInfo(HI_HANDLE hAdec, ADEC_STATUSINFO_S *pstStatusinfo);
HI_S32          HI_DRV_ADEC_Pull(HI_HANDLE hAdec);

#endif

HI_S32          HI_DRV_ADEC_Init(HI_VOID);
HI_VOID         HI_DRV_ADEC_DeInit(HI_VOID);

HI_S32          ADEC_DRV_ModInit(HI_VOID);
HI_VOID         ADEC_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif //__HI_DRV_ADEC_H__
