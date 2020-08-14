/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_vdec_alg.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __OPTM_ALG_H__
#define __OPTM_ALG_H__

#include "hi_type.h"

#define OPTM_FRD_IN_PTS_SEQUENCE_LENGTH         100

#define OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD  10
#define OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD  2

#define OPTM_ALG_ABS(x)     (((x) < 0) ? -(x) : (x))
#define OPTM_ALG_ROUND(x)   (((x % 10) > 4) ? (x / 10 + 1) * 10 : x)

/*****************************************************************
 *              New frame rate detect algorithm                 *
 *****************************************************************/
typedef struct tagOPTM_ALG_FRD_S
{
    HI_U32                       InPTSSqn[OPTM_FRD_IN_PTS_SEQUENCE_LENGTH]; /* past PTS information  */
    HI_U32                       nowPTSPtr; /* pointer of past PTS information, pointing to the oldest frame rate in record */
    HI_U32                       length;
    HI_U32                       u320_Pts;
    HI_U32                       u32120_Pts;
    HI_U32                       u32QueCnt;
    HI_U32                       u32QueRate;
    HI_U32                       u32QueStable;
    
    HI_U32                       unableTime;

    HI_U32                       InFrameRateLast;     /*  last input frame rate */
    HI_U32                       StableThreshold;
    HI_U32                       InFrameRateEqueTime;/*  counter of stable frame rate, to avoid display shake caused by shake of frame rate */

    HI_U32                       InFrameRate;
}OPTM_ALG_FRD_S;

HI_VOID OPTM_ALG_FrdInfo_Reset(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 ptsNum);
HI_U32 OPTM_ALG_FrameRateDetect(OPTM_ALG_FRD_S *pPtsInfo,HI_U32 Pts);
HI_U32 OPTM_ALG_InPTSSqn_CalNowRate(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 env);
HI_VOID OPTM_ALG_InPTSSqn_ChangeInFrameRate(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 nowRate);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __OPTM_ALG_H__ */

