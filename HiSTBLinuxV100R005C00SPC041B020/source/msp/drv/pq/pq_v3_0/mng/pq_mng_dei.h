/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dei.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/06/16
  Description   :

******************************************************************************/


#ifndef __PQ_MNG_DEI_H__
#define __PQ_MNG_DEI_H__

#include "hi_type.h"
#include "pq_hal_dei.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


HI_S32 PQ_MNG_InitDei(HI_VOID);
HI_S32 PQ_MNG_DeInitDei(HI_VOID);
HI_S32 PQ_MNG_GetGlobalMotion(MOTION_INPUT_S* pstMotionInput);
/* enable or disable dei demo */
HI_S32 PQ_MNG_EnableDeiDemo(HI_BOOL bOnOff);
/* set dei demo mode */
HI_S32 PQ_MNG_SetDeiDemoMode(DEI_DEMO_MODE_E enMode);
/* get dei demo mode */
HI_S32 PQ_MNG_GetDeiDemoMode(DEI_DEMO_MODE_E* penMode);
/* set vpss market coordinate */
HI_S32 PQ_MNG_SetVpssMktCoordinate(HI_U32 u32DbgPos);
HI_S32 PQ_MNG_GetGlobMotiGain(HI_U32* pu32Gain);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

