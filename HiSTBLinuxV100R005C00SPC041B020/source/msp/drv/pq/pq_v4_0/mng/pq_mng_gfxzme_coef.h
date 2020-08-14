/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_gfxzme_coef.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/10/15
  Description   :

******************************************************************************/


#ifndef __PQ_GFXZME_COEF_H__
#define __PQ_GFXZME_COEF_H__


/*********************************add include here******************************/
#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/
extern HI_S16 PQ_s16GZmeCoef_8T32P_Cubic[18][8];
extern HI_S16 PQ_s16GZmeCoef_8T32P_Lanc3[18][8];
extern HI_S16 PQ_s16GZmeCoef_8T32P_Lanc2[18][8];
extern HI_S16 PQ_s16GZmeCoef_8T32P_3M_a19[18][8];
extern HI_S16 PQ_s16GZmeCoef_6T32P_Cubic[18][6];
extern HI_S16 PQ_s16GZmeCoef_6T32P_6M_a25[18][6];
extern HI_S16 PQ_s16GZmeCoef_6T32P_5M_a25[18][6];
extern HI_S16 PQ_s16GZmeCoef_6T32P_4M_a20[18][6];
extern HI_S16 PQ_s16GZmeCoef_6T32P_3M_a15[18][6];
extern HI_S16 PQ_s16GZmeCoef_4T32P_Cubic[18][4];
extern HI_S16 PQ_s16GZmeCoef_4T32P_5M_a15[18][4];
extern HI_S16 PQ_s16GZmeCoef_4T32P_4M_a15[18][4];
extern HI_S16 PQ_s16GZmeCoef_2T32P_Cubic[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_6M_1_3[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_5M_1_0[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_4M_0_1[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_5M[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_Gus2_6_75M_a0_5[18][2];
extern HI_S16 PQ_s16GZmeCoef_2T32P_Gus2_6M_a0_5[18][2];
extern HI_S16 PQ_s16GZmeCoef_8T8P_Cubic[8][8];
extern HI_S16 PQ_s16GZmeCoef_8T8P_Lanc3[8][8];
extern HI_S16 PQ_s16GZmeCoef_8T8P_Lanc2[8][8];
extern HI_S16 PQ_s16GZmeCoef_8T8P_3M_a19[8][8];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Cubic[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Lanc2_6M_a15[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Lanc2_5M_a15[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Lanc2_4_5M_a15[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Lanc2_4M_a15[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_Lanc2_3M_a13[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_6M_a15[16][4];
extern HI_S16 PQ_s16GZmeCoef_4T16P_3M_a13[16][4];
extern HI_S16 PQ_s16GZmeCoef_2T8P_Gus2_6_75M_a0_5[8][2];
extern HI_S16 PQ_s16GZmeCoef_2T8P_4M_0_1[8][2];
extern HI_S16 PQ_s16GZmeCoef_2T16P_Gus2_6_75M_a0_5[16][2];
extern HI_S16 PQ_s16GZmeCoef_2T16P_Gus2_6M_a0_5[16][2];
extern HI_S16 PQ_s16GZmeCoef_2T16P_4M_0_1[16][2];

/**================================= begin ==================================**/
/**
 **and from HiFoneB2 4K TinyZME, 2T16P align with 0 by d00241380
 **/
extern HI_S16 PQ_s16TinyZmeCoef_2T16P_Gus2_6_75M_a0_5[16][4];
extern HI_S16 PQ_s16TinyZmeCoef_2T16P_Gus2_6M_a0_5[16][4];
extern HI_S16 PQ_s16TinyZmeCoef_2T16P_4M_0_1[16][4];
/**================================== end ====================================**/

/*---------------- Gfx Reduce Zme Coef ----------------------------*/
/* 6 tap, 8 phase, 6 bit */
extern HI_S16 PQ_s16GZmeCoef6Tap_64M_a15[8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_cubic [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_6M_a15 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_55M_a15 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_5M_a15 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_45M_a15 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_4M_a15 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_36M_a10[8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_3M_a19 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_3M_a10 [8][8];
extern HI_S16 PQ_s16GZmeCoef6Tap_2M_a20 [8][8];

/* 4 tap, 16 phase, 6 bit */
extern HI_S16 PQ_s16GZmeCoef4Tap_64M_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_cubic [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_6M_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_55M_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_5M_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_5M_8_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_48M_a10[16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_45M_a15 [16][4];
extern HI_S16 PQ_s16GZmeCoef4Tap_4M_a15 [16][4];


/******************************* API declaration *****************************/

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __OPTM_ALG_ZME_COEF_H__ */
