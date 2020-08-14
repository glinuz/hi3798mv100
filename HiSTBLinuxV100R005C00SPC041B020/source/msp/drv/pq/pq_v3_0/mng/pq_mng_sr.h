/******************************************************************************
 Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : pq_mng_sr.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2014/01/16
Last Modified :
Description   : PQ MNG层SR模块的数据结构和接口函数
Function List :
History       :
******************************************************************************/

#ifndef __PQ_MNG_SR_H__
#define __PQ_MNG_SR_H__

#include "hi_type.h"
#include "pq_hal_sr.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
    SR_DISABLE  = 0, /* 关SR,只ZME */
    SR_ENABLE_R,     /* 右边SR     */
    SR_ENABLE_L,     /* 左边SR     */
    SR_ENABLE_A,     /* 全屏       */

    SR_DEMO_BUTT
} SR_DEMO_E;


/* 初始化SR模块 */
HI_S32 PQ_MNG_InitSR(HI_VOID);
/* 去初始化SR模块 */
HI_S32 PQ_MNG_DeinitSR(HI_VOID);
/* 开关SR模块 */
HI_S32 PQ_MNG_EnableSR(HI_BOOL bOnOff);
/* 开关SR模块 */
HI_S32 PQ_MNG_GetSREnableFlag(HI_BOOL* bOnOff);
/* 获取SR模块DEMO模式 */
HI_S32 PQ_MNG_GetSRDemo(HI_U32* pu32Type);
/* 开关SR模块DEMO模式 */
HI_S32 PQ_MNG_SetSRDemo(SR_DEMO_E enDemoCtrl);
HI_S32 PQ_MNG_EnableSRDemo(HI_BOOL bOnOff);
/* SR模块中的Sharpen的模块增强和细节增强 */
HI_S32 PQ_MNG_UpdateSRSpMode(HI_U32 u32Width, HI_U32 u32Hight);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


