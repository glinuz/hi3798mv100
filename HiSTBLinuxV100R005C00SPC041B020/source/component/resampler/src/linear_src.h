
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : linear_src.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/21
  Description   : linear resampler for interleaved pcm
  History       :
  1.Date        : 2010/01/21
    Author      : z40717
    Modification: Created file

 *******************************************************************************/

#ifndef  __HI_LINEAR_SRC_H__
#define  __HI_LINEAR_SRC_H__

#include "hi_type.h"
#include "resampler_public.h"
#include "resampler_statname.h"

#define MAX_LINEAR_SPEED_ADJUST 100  /* MAX_LINEAR_SPEED_ADJUST/1000 */


HI_VOID  LinearSRC_UpdateChange(SRC_Linear *pLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                                HI_S32 Channels);

HI_S32   LinearSRC_CheckDoSRC(SRC_Linear *pLinearSrc);

HI_VOID  LinearSRC_SetSpeedAdjust(SRC_Linear *pLinearSrc, HI_S32 SpeedAdjust);

HI_S32   LinearSRC_ProcessFrame(SRC_Linear*   pInst,
                                HI_S32 *      pInPcmBuf,
                                HI_S32 *      pOutpcmBuf,
                                HI_S32        InSamps);

HI_VOID  Volume_ProcessFrame(HI_S32 *     ps32PcmBuf,
                             HI_S32       InSamps,
                             HI_S32       BitPerSample,
                             HI_S32       Channels,
                             HI_S16       s16Volume);

HI_VOID  LinearSRC_Create(SRC_Linear **ppLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                          HI_S32 Channels);

HI_VOID  LinearSRC_Destroy(SRC_Linear *pLinearSrc);
HI_S32   LinearSRC_GetMaxOutputNum(SRC_Linear *pLinearSrc, HI_S32 insamps);
HI_S32   LinearSRC_CheckUpdate(SRC_Linear *pLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                               HI_S32 Channels);

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
