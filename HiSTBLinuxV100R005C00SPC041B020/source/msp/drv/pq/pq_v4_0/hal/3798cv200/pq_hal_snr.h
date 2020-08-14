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


/* tnr demo mode */
typedef enum
{
    SNR_DEMO_ENABLE_L = 0,
    SNR_DEMO_ENABLE_R
} SNR_DEMO_MODE_E;

typedef struct  hiPQ_HAL_SNR_MEAN_RATIO_S
{
    HI_S32 s32MappingMax;    /*meanedgeratio max映射表最大值 0~127 */
    HI_S32 s32MappingMin;    /*meanedgeratio min映射表最小值 0~127 */
    HI_S32 s32MappingX[6];   /*meanedgeratio X*/
    HI_S32 s32MappingY[5];   /*meanedgeratio Y*/
    HI_S32 s32MappingK[4];   /*meanedgeratio K*/

} PQ_HAL_SNR_MEAN_RATIO_S;

typedef struct  hiPQ_HAL_SNR_EDGE_STR_S
{
    HI_S32 s32MappingMax;    /*edgestrmapping max映射表最大值 0~63*/
    HI_S32 s32MappingMin;    /*edgestrmapping min映射表最小值 0~63*/
    HI_S32 s32MappingX[8];   /*edgestrmapping X*/
    HI_S32 s32MappingY[7];   /*edgestrmapping Y*/
    HI_S32 s32MappingK[6];   /*edgestrmapping K*/

} PQ_HAL_SNR_EDGE_STR_S;

HI_S32 PQ_HAL_SetSNRCbCrLut (HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetSNRMotionEdgeLut (HI_U32 u32HandleNo, HI_U32 u32Width);

/* Set Snr Strength */
HI_S32 PQ_HAL_SetSnrStr(HI_U32 u32Data, HI_U32 u32SnrStr);

HI_S32 PQ_HAL_SetNoNoiseGain(HI_U32 u32Data, HI_U32 u32NoNoiseGain);

HI_U32 PQ_HAL_SetNoiseCore(HI_U32 u32HandleNo, HI_U32 u32NoiseCore);

/* Enable or Disable Snr Module */
HI_S32 PQ_HAL_EnableSNR(HI_U32 u32HandleNo, HI_BOOL bSNROnOff);

/* Enable or Disable Snr Demo */
HI_S32 PQ_HAL_EnableSNRDemo(HI_U32 u32HandleNo, HI_BOOL bSNRDemoEn);

HI_S32 PQ_HAL_SetSNRDemoMode(HI_U32 u32HandleNo, SNR_DEMO_MODE_E enMode);

HI_S32 PQ_HAL_SetSnrMidStr(HI_U32 u32HandleNo);

HI_S32 PQ_HAL_SetSnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos);

HI_S32 PQ_HAL_SetSnrMeanRatio(HI_U32 u32HandleNo, PQ_HAL_SNR_MEAN_RATIO_S* pstMeanRatio);

HI_S32 PQ_HAL_GetSnrMeanRatio(HI_U32 u32HandleNo, PQ_HAL_SNR_MEAN_RATIO_S* pstMeanRatio);

HI_S32 PQ_HAL_SetSnrEdgeStr(HI_U32 u32HandleNo, PQ_HAL_SNR_EDGE_STR_S* pstEdgeStr);

HI_S32 PQ_HAL_GetSnrEdgeStr(HI_U32 u32HandleNo, PQ_HAL_SNR_EDGE_STR_S* pstEdgeStr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

