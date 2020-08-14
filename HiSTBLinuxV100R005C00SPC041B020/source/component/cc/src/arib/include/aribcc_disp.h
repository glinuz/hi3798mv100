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

#ifndef _ARIB_CC_DISP_H_
#define _ARIB_CC_DISP_H_

#include "hi_type.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

HI_S32 ARIBCC_DISP_Create(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DISP_Destroy(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DISP_Start(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DISP_Stop(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DISP_Reset(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DISP_Cycle(ARIBCC_CaptionType_E enCaptionType);


#ifdef __cplusplus
 #if __cplusplus
 }
 #endif
#endif

#endif


