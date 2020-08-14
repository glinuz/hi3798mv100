/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : optm_alg_zme_coef.h
Version             : Initial Draft
Author              :
Created             : 2014/09/09
Description         : ALG这一层代码是由算法提供的
Function List       :
History             :
Date                       Author                   Modification
2014/09/09                 y00181162                Created file
******************************************************************************/

#ifndef __OPTM_ALG_ZME_COEF_H__
#define __OPTM_ALG_ZME_COEF_H__


/*********************************add include here******************************/
#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/


extern HI_S16 OPTM_s16ZmeCoef_8T32P_Cubic[18][8];
extern HI_S16 OPTM_s16ZmeCoef_8T32P_Lanc3[18][8];
extern HI_S16 OPTM_s16ZmeCoef_8T32P_Lanc2[18][8];
extern HI_S16 OPTM_s16ZmeCoef_8T32P_3M_a19[18][8];
extern HI_S16 OPTM_s16ZmeCoef_6T32P_Cubic[18][6];
extern HI_S16 OPTM_s16ZmeCoef_6T32P_6M_a25[18][6];
extern HI_S16 OPTM_s16ZmeCoef_6T32P_5M_a25[18][6];
extern HI_S16 OPTM_s16ZmeCoef_6T32P_4M_a20[18][6];
extern HI_S16 OPTM_s16ZmeCoef_6T32P_3M_a15[18][6];
extern HI_S16 OPTM_s16ZmeCoef_4T32P_Cubic[18][4];
extern HI_S16 OPTM_s16ZmeCoef_4T32P_5M_a15[18][4];
extern HI_S16 OPTM_s16ZmeCoef_4T32P_4M_a15[18][4];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_Cubic[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_6M_1_3[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_5M_1_0[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_4M_0_1[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_5M[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_Gus2_6_75M_a0_5[18][2];
extern HI_S16 OPTM_s16ZmeCoef_2T32P_Gus2_6M_a0_5[18][2];
extern HI_S16 OPTM_s16ZmeCoef_8T8P_Cubic[8][8];
extern HI_S16 OPTM_s16ZmeCoef_8T8P_Lanc3[8][8];
extern HI_S16 OPTM_s16ZmeCoef_8T8P_Lanc2[8][8];
extern HI_S16 OPTM_s16ZmeCoef_8T8P_3M_a19[8][8];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Cubic[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Lanc2_6M_a15[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Lanc2_5M_a15[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Lanc2_4_5M_a15[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Lanc2_4M_a15[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_Lanc2_3M_a13[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_6M_a15[16][4];
extern HI_S16 OPTM_s16ZmeCoef_4T16P_3M_a13[16][4];
extern HI_S16 OPTM_s16ZmeCoef_2T8P_Gus2_6_75M_a0_5[8][2];
extern HI_S16 OPTM_s16ZmeCoef_2T8P_4M_0_1[8][2];
extern HI_S16 OPTM_s16ZmeCoef_2T16P_Gus2_6_75M_a0_5[16][2];
extern HI_S16 OPTM_s16ZmeCoef_2T16P_Gus2_6M_a0_5[16][2];
extern HI_S16 OPTM_s16ZmeCoef_2T16P_4M_0_1[16][2];

/**================================= begin ==================================**/
/**
 **and from HiFoneB2 4K TinyZME, 2T16P align with 0 by d00241380
 **/
extern HI_S16 OPTM_s16TinyZmeCoef_2T16P_Gus2_6_75M_a0_5[16][4];
extern HI_S16 OPTM_s16TinyZmeCoef_2T16P_Gus2_6M_a0_5[16][4];
extern HI_S16 OPTM_s16TinyZmeCoef_2T16P_4M_0_1[16][4];
/**================================== end ====================================**/


/******************************* API declaration *****************************/

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __OPTM_ALG_ZME_COEF_H__ */
