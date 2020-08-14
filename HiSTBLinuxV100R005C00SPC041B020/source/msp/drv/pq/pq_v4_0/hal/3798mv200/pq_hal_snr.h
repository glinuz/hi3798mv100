/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_snr.h
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/10/19
 Description   :
******************************************************************************/
#ifndef __PQ_HAL_SNR_H__
#define __PQ_HAL_SNR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


typedef enum
{
    SNR_DEMO_ENABLE_L = 0,
    SNR_DEMO_ENABLE_R
} SNR_DEMO_MODE_E;

typedef struct
{
    HI_S32 edgeoristrength;
    HI_S32 edgemeanth1;
    HI_S32 edgemeanth2;
    HI_S32 edgemeanth3;
    HI_S32 edgemeanth4;
    HI_S32 edgemeanth5;
    HI_S32 edgemeanth6;
    HI_S32 edgemeanth7;
    HI_S32 edgemeanth8;
    HI_S32 edgemeank1;
    HI_S32 edgemeank2;
    HI_S32 edgemeank3;
    HI_S32 edgemeank4;
    HI_S32 edgemeank5;
    HI_S32 edgemeank6;
    HI_S32 edgemeank7;
    HI_S32 edgemeank8;
    HI_S32 edgeoriratio;
    HI_S32 ratio1;
    HI_S32 ratio2;
    HI_S32 ratio3;
    HI_S32 ratio4;
    HI_S32 ratio5;
    HI_S32 ratio6;
    HI_S32 ratio7;
    HI_S32 ratio8;
    HI_S32 edgestrth1;
    HI_S32 edgestrth2;
    HI_S32 edgestrth3;
    HI_S32 edgestrk1;
    HI_S32 edgestrk2;
    HI_S32 edgestrk3;
    HI_S32 edge1;
    HI_S32 edge2;
    HI_S32 edge3;

} PQ_HAL_SNR_PARA_S;




HI_S32 PQ_HAL_SetSNRCbCrLut (HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetSNRMotionEdgeLut (HI_U32 u32HandleNo, HI_U32 u32Width);

HI_S32 PQ_HAL_SetSnrStr(HI_U32 u32Data, HI_U32 u32SnrStr);

HI_S32 PQ_HAL_EnableSNR(HI_U32 u32HandleNo, HI_BOOL bSNROnOff);

HI_S32 PQ_HAL_EnableSNRDemo(HI_U32 u32HandleNo, HI_BOOL bSNRDemoEn);

HI_S32 PQ_HAL_SetSNRDemoMode(HI_U32 u32HandleNo, SNR_DEMO_MODE_E enMode);

HI_S32 PQ_HAL_SetSnrMidStr(HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetSnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos);

HI_S32 PQ_HAL_SetSNRRatioMap(HI_U32 u32HandleNo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

