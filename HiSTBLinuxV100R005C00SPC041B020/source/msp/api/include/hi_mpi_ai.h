/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name             :   hi_mpi_ai.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2012/12/20
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2012/12/20
    Author              :   z40717
Modification            :   Created file
******************************************************************************/

#ifndef  __MPI_AI_H__
#define  __MPI_AI_H__

#include "hi_type.h"
#include "hi_unf_ai.h"
#include "hi_drv_ai.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/******************************* MPI for UNF Sound Init *****************************/
HI_S32 HI_MPI_AI_Init(HI_VOID);

HI_S32 HI_MPI_AI_DeInit(HI_VOID);

HI_S32 HI_MPI_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr);

HI_S32 HI_MPI_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr);

HI_S32 HI_MPI_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr);

HI_S32 HI_MPI_AI_Create(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr, HI_HANDLE *phandle);

HI_S32 HI_MPI_AI_Destroy(HI_HANDLE hAI);

HI_S32 HI_MPI_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable);

HI_S32 HI_MPI_AI_GetEnable(HI_HANDLE hAI, HI_BOOL *pbEnable);

HI_S32 HI_MPI_AI_AcquireFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame);

HI_S32 HI_MPI_AI_ReleaseFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame);

HI_S32 HI_MPI_AI_Attach(HI_HANDLE hAI, HI_HANDLE hDst);

HI_S32 HI_MPI_AI_Detach(HI_HANDLE hAI, HI_HANDLE hDst);

HI_S32 HI_MPI_AI_SetDelay(HI_HANDLE hAI, const HI_UNF_AI_DELAY_S *pstDelay);

HI_S32 HI_MPI_AI_GetDelay(HI_HANDLE hAI, HI_UNF_AI_DELAY_S *pstDelay);

#ifdef __DPT__
HI_S32 HI_MPI_AI_GetStreamType(HI_HANDLE hAI, HI_UNF_AI_HDMI_DATA_TYPE_E *penStreamType);

HI_S32 HI_MPI_AI_SetNRAttr(HI_HANDLE hAI, const HI_UNF_AI_NR_ATTR_S *pstNRAttr);

HI_S32 HI_MPI_AI_GetNRAttr(HI_HANDLE hAI, HI_UNF_AI_NR_ATTR_S *pstNRAttr);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif //__MPI_AI_H__
