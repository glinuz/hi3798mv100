/******************************************************************************
 Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : pq_mng_sharpen.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2013/10/12
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __PQ_MNG_SHARPEN_H__
#define __PQ_MNG_SHARPEN_H__

#include "hi_type.h"
#include "pq_hal_sharpen.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_InitSharpen(PQ_PARAM_S* pstPqParam);
/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_DeinitSharpen(HI_VOID);
/* Set Sharpen Strength, Range: 0~255 */
HI_S32 PQ_MNG_SetSharpenStr(HI_S32 s32ShpStr, HI_U32 u32SourceMode, HI_U32 u32OutputMode);
/* Get Sharpen Strength, Range: 0~255 */
HI_S32 PQ_MNG_GetSharpenStr(HI_S32* ps32ShpStr);
/* Enable Sharpen Moudle(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpen(HI_BOOL bOnOff);
/* Get Sharpen Enable(Sharpen and HSharpen) */
HI_S32 PQ_MNG_GetSharpenEnable(HI_BOOL* pbOnOff);
/* Enable Sharpen Demo(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpenDemo(HI_BOOL bOnOff);
HI_S32 PQ_MNG_SetSharpDemoMode(SHARP_DEMO_MODE_E enMode);
HI_S32 PQ_MNG_GetSharpDemoMode(SHARP_DEMO_MODE_E* enMode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

