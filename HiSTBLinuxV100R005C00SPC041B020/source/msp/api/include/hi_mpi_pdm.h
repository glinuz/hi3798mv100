/******************************************************************************

  Copyright (C), 2011-2014, HiSilicon Technologies Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_pdm.h
  Version       : Initial Draft
  Author        : HiSilicon multimedia software group
  Created       : 2011-xx-xx
  Description   : hi_unf_pdm.h header file
  History       :
  1.Date        : 2011/xx/xx
    Author      : 
    Modification: This file is created.

******************************************************************************/

#ifndef __HI_MPI_PDM_H__
#define __HI_MPI_PDM_H__

#include "hi_unf_common.h"
#include "hi_unf_mce.h"
#include "hi_unf_pdm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  /* End of #ifdef __cplusplus */

#define HI_MPI_PDM_BASEPARAM_PMOC (HI_UNF_PDM_BASEPARAM_BUTT - 1)

typedef struct hiMPI_PDM_PMOCPARAM_S
{
    HI_U32      u32Flag;
}HI_MPI_PDM_PMOCPARAM_S;

HI_S32  HI_MPI_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData);

HI_S32  HI_MPI_PDM_UpdateBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData);

HI_S32  HI_MPI_PDM_GetLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam);

HI_S32  HI_MPI_PDM_UpdateLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam);

HI_S32  HI_MPI_PDM_GetLogoContent(HI_U8 *pu8Content, HI_U32 u32Size);

HI_S32  HI_MPI_PDM_UpdateLogoContent(HI_U8 *pu8Content, HI_U32 u32Size);

HI_S32  HI_MPI_PDM_GetPlayParam(HI_CHAR *PartName, HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

HI_S32  HI_MPI_PDM_UpdatePlayParam(HI_CHAR *PartName, HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

HI_S32  HI_MPI_PDM_GetPlayContent(HI_CHAR *PartName, HI_U8 *pu8Content, HI_U32 u32Size);

HI_S32  HI_MPI_PDM_UpdatePlayContent(HI_CHAR *PartName, HI_U8 *pu8Content, HI_U32 u32Size);

HI_S32  HI_MPI_PDM_GetDeviceInfo(HI_CHAR * pstrCfgName, HI_CHAR * pstrCfgValue, HI_U32 u32Size);
/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif




