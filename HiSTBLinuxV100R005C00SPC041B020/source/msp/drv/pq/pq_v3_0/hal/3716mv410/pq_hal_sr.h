/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_sr.h
  Version       : Initial Draft
  Author        :
  Created       : 2014/01/16
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_SR_H__
#define __PQ_HAL_SR_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* sr_disp_state: SR支持四种显示状态 */
typedef enum hiSR_ZME_E
{
    SR_AND_ZME_EN = 0x0 , /* 正常显示, ZME+SR */
    SR_ONLY_ZME_EN      , /* 正常显示, ZME, 无SR */
    SR_AND_ZME_EN_L     , /* demo显示, 左边ZME+SR, 右边ZME */
    SR_AND_ZME_EN_R     , /* demo显示, 左边ZME, 右边ZME+SR */

    SR_ZME_EN_BUTT
} SR_ZME_E;

typedef enum hiSR_SHARP_MODE_E
{
    SR_SHARP_MODE_DETAIL = 0x0 , /* 细节增强 */
    SR_SHARP_MODE_TRAD         , /* 方向增强 */

    SR_SHARP_MODE_BUTT
} SR_SHARP_MODE_E;

typedef enum hiSR_SHARP_E
{
    SR_SHARP_UENABLE = 0x0 ,
    SR_SHARP_ENABLE        ,

    SR_SHARP_BUTT
} SR_SHARP_E;


HI_VOID PQ_HAL_GetSREnable(HI_U32 u32ChId, HI_BOOL* bOnOff);
/* 开关SR模块ZME DEMO模式 */
HI_VOID PQ_HAL_SR_EnZme(HI_U32 u32ChId, SR_ZME_E enZme);
/* SR模块中的Sharpen的模块增强和细节增强 */
HI_VOID PQ_HAL_SetSRSpMode(HI_U32 u32ChId, SR_SHARP_MODE_E eHsh_nodir);
HI_VOID PQ_HAL_GetSRSharpStr(HI_U32 u32ChId, HI_U32* pu32SharpStr);
HI_VOID PQ_HAL_SetSRSharpStr(HI_U32 u32ChId, HI_U32 u32SharpStr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
