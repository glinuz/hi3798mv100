/******************************************************************************
*
* Copyright (C) 2015 -2016  Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_tnr.h
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2016/05/25
 Description   :
******************************************************************************/
#ifndef __PQ_HAL_TNR_H__
#define __PQ_HAL_TNR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/* tnr demo mode */
typedef enum
{
    TNR_DEMO_ENABLE_L = 0,
    TNR_DEMO_ENABLE_R
} TNR_DEMO_MODE_E;


HI_S32 PQ_HAL_SetTnrStr(HI_U32 u32HandleNo, HI_U32 u32TnrStr);

HI_S32 PQ_HAL_EnableTNR(HI_U32 u32HandleNo, HI_BOOL bNROnOff);

HI_S32 PQ_HAL_EnableTNRDemo(HI_U32 u32HandleNo, HI_BOOL bTNRDemoEn);

HI_S32 PQ_HAL_SetTNRDemoMode(HI_U32 u32HandleNo, TNR_DEMO_MODE_E enMode);

HI_S32 PQ_HAL_SetTnrMidStr(HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetTnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos);

HI_S32 PQ_HAL_SetTNRBlendingAlphaLut(HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetTNRMotionStrLut(HI_U32 u32HandleNo);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

