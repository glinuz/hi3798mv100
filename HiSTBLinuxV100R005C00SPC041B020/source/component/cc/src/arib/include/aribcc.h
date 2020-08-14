/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/

#ifndef __ARIBCC_H__
#define __ARIBCC_H__

#include "hi_type.h"
#include "hi_unf_cc.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif


HI_S32 AribCC_Init(HI_VOID);

HI_S32 AribCC_DeInit(HI_VOID);

HI_VOID* AribCC_Create(HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfig);

HI_S32 AribCC_Destroy(HI_VOID* hAribCC);

HI_S32 AribCC_Start(HI_VOID* hAribCC);

HI_S32 AribCC_Stop(HI_VOID* hAribCC);

HI_S32 AribCC_Reset(HI_VOID* hAribCC);

HI_S32 AribCC_Config(HI_VOID* hAribCC, HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfigParam);

HI_S32 AribCC_GetConfig(HI_VOID* hAribCC, HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribCCConfig);

HI_S32 AribCC_GetCaptionInfo(HI_VOID* hAribCC, HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo);

HI_S32 AribCC_InjectPESData(HI_VOID* hAribCC, HI_U8 *pu8PesData, HI_U32 u32DataLen);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif

