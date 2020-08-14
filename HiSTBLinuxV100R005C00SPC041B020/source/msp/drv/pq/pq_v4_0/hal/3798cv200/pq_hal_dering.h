/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dering.h
  Version       : Initial Draft
  Author        : l00216738
  Created       : 2015/09/01
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_DERING_H__
#define __PQ_HAL_DERING_H__

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* dering demo mode */
typedef enum
{
    DR_DEMO_ENABLE_L = 0,
    DR_DEMO_ENABLE_R
} DR_DEMO_MODE_E;


/**
 \brief :  Enable or Disable Dering Demo ;

 \param[in] bNROnOff

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_HAL_EnableDeringDemo(HI_U32 u32HandleNo, HI_BOOL DeringDemoEn);


/**
 \brief :  Set Dering Demo Mode;

 \param[in]

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_HAL_SetDeringDemoMode(HI_U32 u32HandleNo, DR_DEMO_MODE_E enMode);

HI_S32 PQ_HAL_EnableDering(HI_U32 u32Data, HI_BOOL bOnOff);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


