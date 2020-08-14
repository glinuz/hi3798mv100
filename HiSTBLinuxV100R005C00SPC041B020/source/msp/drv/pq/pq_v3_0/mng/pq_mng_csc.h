/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_csc.h
  Version       : Initial Draft
  Author        : l00212594
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_CSC_H__
#define __PQ_MNG_CSC_H__

#include "hi_type.h"
#include "pq_hal_csc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*窗口类型*/
typedef enum hiCSC_WINDOW_E
{
    CSC_WINDOW_MAIN = 0,
    CSC_WINDOW_SUB,

    CSC_WINDOW_BUTT
} CSC_WINDOW_E;

/*亮度/对比度/色调/饱和度设定*/
typedef struct hiPICTURE_SETTING_S
{
    HI_U16 u16Brightness;
    HI_U16 u16Contrast;
    HI_U16 u16Hue;
    HI_U16 u16Saturation;
} PICTURE_SETTING_S;

/*色温设定*/
typedef struct hiCOLOR_TEMPERATURE_S
{
    HI_S16 s16RedGain;
    HI_S16 s16GreenGain;
    HI_S16 s16BlueGain;
    HI_S16 s16RedOffset;
    HI_S16 s16GreenOffset;
    HI_S16 s16BlueOffset;
} COLOR_TEMPERATURE_S;

/*色彩空间设定*/
typedef struct hiCOLOR_SPACE_S
{
    COLOR_SPACE_TYPE_E u16InputColorSpace;    /*输入色彩空间*/
    COLOR_SPACE_TYPE_E u16OutputColorSpace;   /*输出色彩空间*/
    HI_BOOL            bFullRange;            /*0:limit,1:full range*/
} COLOR_SPACE_S;


/**
 \brief 获取CSC色彩空间转换;
 \attention \n
无

 \param[in] enDisplayId
 \param[out] *pstCscMode

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_GetCSCMode(HAL_DISP_LAYER_E enDisplayId, CSC_MODE_S* pstCscMode);

/**
 \brief 设置CSC色彩空间转换;
 \attention \n
无

 \param[in] enDisplayId
 \param[in] *pstCscMode

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_SetCSCMode(HAL_DISP_LAYER_E enDisplayId, CSC_MODE_S* pstCscMode);

/**
 \brief 设置标清CSC图像设定;
 \attention \n
无

 \param[in] *pstPictureSetting;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_SetSDPictureSetting(PICTURE_SETTING_S* pstPictureSetting, COLOR_TEMPERATURE_S* pstColorTemp);

/**
 \brief 获取标清CSC图像设定;
 \attention \n
无

 \param[out] *pstPictureSetting;
 \param[out] *pstColorTemp;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_GetSDPictureSetting(PICTURE_SETTING_S* pstPictureSetting, COLOR_TEMPERATURE_S* pstColorTemp);

/**
 \brief 设置高清CSC图像设定;
 \attention \n
无

 \param[in] *pstPictureSetting;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_SetHDPictureSetting(PICTURE_SETTING_S* pstPictureSetting, COLOR_TEMPERATURE_S* pstColorTemp);

/**
 \brief 获取高清CSC图像设定;
 \attention \n
无

 \param[out] *pstPictureSetting;
 \param[out] *pstColorTemp;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_GetHDPictureSetting(PICTURE_SETTING_S* pstPictureSetting, COLOR_TEMPERATURE_S* pstColorTemp);

HI_S32 PQ_MNG_GetCscCoef(CSC_MODE_E enCscMode, CSC_COEF_S* pstCscCoef, CSC_DCCOEF_S* pstCscDCCoef);

HI_S32 PQ_MNG_Get8BitCscCoef(CSC_MODE_E enCscMode, CSC_COEF_S* pstCscCoef, CSC_DCCOEF_S* pstCscDCCoef);

HI_S32 PQ_MNG_SetIsogenyMode(HI_BOOL bIsogenyMode);

HI_S32 PQ_MNG_GetIsogenyMode(HI_BOOL* pbIsogenyMode);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
