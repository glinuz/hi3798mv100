/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_deshoot.h
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2015/05/11
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_DESHOOT_H__
#define __PQ_HAL_DESHOOT_H__

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ds demo mode */
typedef enum
{
    DS_DEMO_ENABLE_L = 0,
    DS_DEMO_ENABLE_R
} DS_DEMO_MODE_E;


/**
 \brief :  Enable or Disable Ds Demo ;

 \param[in] bNROnOff

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_HAL_EnableDeshootDemo(HI_U32 u32HandleNo, HI_BOOL DSDemoEn);


/**
 \brief :  Set Ds Demo Mode;

 \param[in]

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_HAL_SetDeshootDemoMode(HI_U32 u32HandleNo, DS_DEMO_MODE_E enMode);

HI_S32 PQ_HAL_SetDeshootThr(HI_U32 u32HandleNo, HI_U32 u32Width);

HI_S32 PQ_HAL_EnableDeshoot(HI_U32 u32Data, HI_BOOL bOnOff);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


