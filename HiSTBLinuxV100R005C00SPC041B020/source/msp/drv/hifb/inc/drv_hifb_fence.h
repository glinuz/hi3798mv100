/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_fence.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_FENCE_H__
#define __DRV_HIFB_FENCE_H__


/*********************************add include here******************************/
#include <sw_sync.h>

#include "drv_hifb_osr.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/

#define HIFB_FENCE_NAME            "hifb_fence"


/*************************** Structure Definition ****************************/
typedef struct
{
    atomic_t UpdateCnt;
    HI_U32   u32FenceValue;
    HI_U32   u32Timeline;
    HI_U32  FrameEndFlag;
    wait_queue_head_t    FrameEndEvent;
    struct sw_sync_timeline *pstTimeline;
    HI_BOOL bFrameHit;
    spinlock_t         lock; 
}HIFB_SYNC_INFO_S;

/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

HI_S32  DRV_HIFB_FenceInit(HIFB_PAR_S *par);
HI_VOID DRV_HIFB_FenceDInit(HIFB_PAR_S *par);
HI_VOID DRV_HIFB_FENCE_FrameEndCallBack(HI_VOID);
HI_BOOL DRV_HIFB_FENCE_IsRefresh(HI_VOID);
HI_S32  DRV_HIFB_FENCE_Create(HI_VOID);
HI_VOID DRV_HIFB_IncRefreshTime(HI_BOOL bLayerEnable);
HI_VOID DRV_HIFB_WaiteRefreshEnd(HI_VOID);
HI_S32  DRV_HIFB_FENCE_Waite(struct sync_fence *fence, long timeout);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
