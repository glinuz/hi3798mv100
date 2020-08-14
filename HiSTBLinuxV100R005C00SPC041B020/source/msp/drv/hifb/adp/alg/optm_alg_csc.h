/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : optm_alg_csc.h
Version             : Initial Draft
Author              : this file get from alg
Created             : 2014/08/06
Description         : ALG这一层代码是由算法提供的
Function List       :
History             :
Date                       Author                   Modification
2014/08/06                 y00181162                Created file
******************************************************************************/

#ifndef __OPTM_ALG_CSC_H__
#define __OPTM_ALG_CSC_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include "drv_hifb_adp.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
typedef enum
{
    ALG_CS_eUnknown      = 0,
    ALG_CS_eYCbCr_709    = 1,
    ALG_CS_eYCbCr_601    = 2,
    ALG_CS_eRGB_709      = 3,
    ALG_CS_TYPE_BUTT

} OPTM_ALG_CS_TYPE_E;


typedef enum
{
    ALG_CS_RANGE_eUnknown       = 0,
    ALG_CS_RANGE_FULL           = 1,
    ALG_CS_RANGE_LMTD           = 2,
    ALG_CS_RANGE_BUTT

} OPTM_ALG_CS_RANGE_E;

typedef struct
{
    OPTM_ALG_CS_TYPE_E eCsType;
    OPTM_ALG_CS_RANGE_E eCsRange;
}OPTM_ALG_CS_S;

typedef struct
{
    //ALG_CS_TYPE_E eInputCS;       //input color space type, should be set carefully according to the application situation.
    //ALG_CS_TYPE_E eOutputCS;      //output color space type, should be set carefully according to the application situation.
    //ALG_CS_RANGE_E eInputR;       //input color value range( full or limited), should be set carefully according to the application situation.
    //ALG_CS_RANGE_E eOutputR;      //output color value range( full or limited), should be set carefully according to the application situation.
    OPTM_COLOR_SPACE_E eInputCS;
    OPTM_COLOR_SPACE_E eOutputCS;
    HI_BOOL bIsBGRIn;

    HI_U32 u32Bright;      //bright adjust value,range[0,100],default setting 50;
    HI_U32 u32Contrst;     //contrast adjust value,range[0,100],default setting 50;
    HI_U32 u32Hue;         //hue adjust value,range[0,100],default setting 50;
    HI_U32 u32Satur;       //saturation adjust value,range[0,100],default setting 50;
    HI_U32 u32Kr;          //red component gain adjust value for color temperature adjust,range[0,100],default setting 50;
    HI_U32 u32Kg;          //green component gain adjust value for color temperature adjust,range[0,100],default setting 50;
    HI_U32 u32Kb;          //blue component gain adjust value for color temperature adjust,range[0,100],default setting 50;
    HI_U32 u32Rgb2YuvScale2p;
    HI_U32 u32Rgb2YuvMin;
    HI_U32 u32Rgb2YuvMax;
} OPTM_ALG_CSC_DRV_PARA_S;

typedef struct
{
    HI_S32 s32CscDcIn_0;    //input color space DC value of component 0;
    HI_S32 s32CscDcIn_1;    //input color space DC value of component 1;
    HI_S32 s32CscDcIn_2;    //input color space DC value of component 2;

    HI_S32 s32CscDcOut_0;   //output color space DC value of component 0;
    HI_S32 s32CscDcOut_1;   //output color space DC value of component 1;
    HI_S32 s32CscDcOut_2;   //output color space DC value of component 2;

    HI_S32 s32Rgb2YuvOut_0; //output color space DC value of component 0;
    HI_S32 s32Rgb2YuvOut_1; //output color space DC value of component 1;
    HI_S32 s32Rgb2YuvOut_2; //output color space DC value of component 2;

    HI_S32 s32CscCoef_00;   //member 00 of 3*3 matrix
    HI_S32 s32CscCoef_01;   //member 01 of 3*3 matrix
    HI_S32 s32CscCoef_02;   //member 02 of 3*3 matrix

    HI_S32 s32CscCoef_10;   //member 10 of 3*3 matrix
    HI_S32 s32CscCoef_11;   //member 11 of 3*3 matrix
    HI_S32 s32CscCoef_12;   //member 12 of 3*3 matrix

    HI_S32 s32CscCoef_20;   //member 20 of 3*3 matrix
    HI_S32 s32CscCoef_21;   //member 21 of 3*3 matrix
    HI_S32 s32CscCoef_22;   //member 22 of 3*3 matrix

    HI_S32 s32Rgb2YuvCoef_00;   //member 00 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_01;   //member 01 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_02;   //member 02 of 3*3 matrix

    HI_S32 s32Rgb2YuvCoef_10;   //member 10 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_11;   //member 11 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_12;   //member 12 of 3*3 matrix

    HI_S32 s32Rgb2YuvCoef_20;   //member 20 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_21;   //member 21 of 3*3 matrix
    HI_S32 s32Rgb2YuvCoef_22;   //member 22 of 3*3 matrix
} OPTM_ALG_CSC_RTL_PARA_S;


/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

HI_VOID OPTM_ALG_CscCoefSet(OPTM_ALG_CSC_DRV_PARA_S *pstCscDrvPara, OPTM_ALG_CSC_RTL_PARA_S *pstCscRtlPara);


#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __OPTM_ALG_CSC_H__ */
