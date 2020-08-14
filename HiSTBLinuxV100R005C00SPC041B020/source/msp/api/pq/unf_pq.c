/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : hi_unf_pq.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Description   : UNF层封装函数

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_type.h"
#include "hi_unf_pq.h"
#include "hi_mpi_pq.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 \brief 初始化PQ
 \attention \n
无

 \param[in] pszPath:PQ配置文件路径

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_Init(HI_VOID)
{
    HI_CHAR *pszPath = HI_NULL;
    return HI_MPI_PQ_Init(pszPath);
}


/**
 \brief 去初始化PQ
 \attention \n
无

 \param[in] none

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_DeInit(HI_VOID)
{
    return HI_MPI_PQ_DeInit();
}


/**
 \brief Set PQ mode . CNcomment: 设置图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] enImageMode Destination DISP channel PQ mode CNcomment: 目标通道图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_SetImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E enImageMode)
{
    return  HI_MPI_PQ_SetImageMode(enImageMode);
}


/**
 \brief Get PQ mode . CNcomment: 获取图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] penImageMode  pointer of image mode CNcomment: 指针类型，指向图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_GetImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E *penImageMode)
{
    MPI_PQ_CHECK_NULL_PTR(penImageMode);

    return  HI_MPI_PQ_GetImageMode(penImageMode);
}


/**
 \brief Init PQ mode . CNcomment: 初始化图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] penImageMode  pointer of image mode CNcomment: 指针类型，指向图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_InitImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E enImageMode)
{

    return HI_SUCCESS;
}


/**
 \brief Set channel option. CNcomment: 设置通道option值 CNend
 \attention \n
 \param[in] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_SetChanOption(const HI_UNF_PQ_OPT_CHANS_S *pstChanOption)
{
    HI_UNF_DISP_E enChan;

    MPI_PQ_CHECK_NULL_PTR(pstChanOption);

    enChan = pstChanOption->enChan;

    HI_MPI_PQ_SetBrightness((HI_DRV_DISPLAY_E)enChan, pstChanOption->stChanOpt.u32Brightness );
    HI_MPI_PQ_SetContrast((HI_DRV_DISPLAY_E)enChan, pstChanOption->stChanOpt.u32Contrast );
    HI_MPI_PQ_SetHue((HI_DRV_DISPLAY_E)enChan, pstChanOption->stChanOpt.u32Hue);
    HI_MPI_PQ_SetSaturation((HI_DRV_DISPLAY_E)enChan, pstChanOption->stChanOpt.u32Saturation);

    return HI_SUCCESS;
}


/**
 \brief Set channel option. CNcomment: 获取通道option值 CNend
 \attention \n
 \param[out] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_GetChanOption(HI_UNF_PQ_OPT_CHANS_S *pstChanOption)
{
    HI_UNF_DISP_E enChan;
    HI_U32 u32Brightness = 0;
    HI_U32 u32Contrast = 0;
    HI_U32 u32Hue = 0;
    HI_U32 u32Saturation = 0;

    MPI_PQ_CHECK_NULL_PTR(pstChanOption);

    enChan = pstChanOption->enChan;

    HI_MPI_PQ_GetBrightness((HI_DRV_DISPLAY_E)enChan, &u32Brightness);
    HI_MPI_PQ_GetContrast((HI_DRV_DISPLAY_E)enChan, &u32Contrast);
    HI_MPI_PQ_GetHue((HI_DRV_DISPLAY_E)enChan, &u32Hue);
    HI_MPI_PQ_GetSaturation((HI_DRV_DISPLAY_E)enChan, &u32Saturation);

    pstChanOption->stChanOpt.u32Brightness = u32Brightness;
    pstChanOption->stChanOpt.u32Contrast = u32Contrast;
    pstChanOption->stChanOpt.u32Hue = u32Hue;
    pstChanOption->stChanOpt.u32Saturation = u32Saturation;
    pstChanOption->stChanOpt.u32Colortemperature = 0;
    pstChanOption->stChanOpt.u32GammaMode = 0;
    pstChanOption->stChanOpt.u32DynamicContrast = 0;
    pstChanOption->stChanOpt.u32IntelligentColor = 0;

    return HI_SUCCESS;
}


/**
 \brief Set channel common option. CNcomment: 设置通道 common option值 CNend
 \attention \n
 \param[in] pstCommOption pointer of channel common option CNcomment: 指针类型，指向通道common option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_SetCommOption(const HI_UNF_PQ_OPT_COMMON_S *pstCommOption)
{
    MPI_PQ_CHECK_NULL_PTR(pstCommOption);

    return HI_MPI_PQ_SetSharpness(pstCommOption->u32Sharpeness);
}


/**
 \brief Set channel option. CNcomment: 获取通道common option值 CNend
 \attention \n
 \param[out] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道common option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_GetCommOption(HI_UNF_PQ_OPT_COMMON_S *pstCommOption)
{
    HI_S32 s32Ret;
    HI_U32 u32Sharpness = 0;

    MPI_PQ_CHECK_NULL_PTR(pstCommOption);

    s32Ret = HI_MPI_PQ_GetSharpness(&u32Sharpness);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    pstCommOption->u32Sharpeness = u32Sharpness;
    pstCommOption->u32Denoise = 0;
    pstCommOption->u32FilmMode = 0;

    return s32Ret;
}



/**
 \brief Modifies the basic configuration information.  CNcomment:更新PQ配置区信息 CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
 N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_PQ_UpdatePqParam(HI_VOID)
{

    return HI_SUCCESS;
}


/**
 \brief Set the default PQ configuration for video parameter test.  CNcomment: 为入网指标测试设置PQ 的默认值CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
 N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_SetDefaultParam(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_PQ_SetDefaultParam(HI_TRUE);

    return s32Ret;
}

/**
 \brief 获取亮度
 \attention \n
无

 \param[in] pu32Brightness 亮度值,有效范围: 0~100;
 \param[out]

 \retval ::HI_SUCCESS

 */
HI_S32 HI_UNF_PQ_GetBrightness(HI_UNF_DISP_E enChan, HI_U32 *pu32Brightness)
{
    HI_S32 s32Ret;
    HI_U32 u32Brightness = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32Brightness);

    s32Ret = HI_MPI_PQ_GetBrightness((HI_DRV_DISPLAY_E)enChan, &u32Brightness);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    *pu32Brightness = u32Brightness;
    return s32Ret;
}

/**
 \brief 设置亮度
 \attention \n
无

 \param[in] u32Brightness, 亮度值,有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetBrightness(HI_UNF_DISP_E enChan, HI_U32 u32Brightness)
{
    if ( u32Brightness > 100)
    {
        HI_ERR_PQ("The brightness is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetBrightness((HI_DRV_DISPLAY_E)enChan, u32Brightness);
}


/**
 \brief 获取对比度
 \attention \n
无

 \param[in]
 \param[out] pu32Contrast 对比度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetContrast(HI_UNF_DISP_E enChan, HI_U32 *pu32Contrast)
{
    HI_S32 s32Ret;
    HI_U32 u32Contrast = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32Contrast);

    s32Ret = HI_MPI_PQ_GetContrast((HI_DRV_DISPLAY_E)enChan, &u32Contrast);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    *pu32Contrast = u32Contrast;
    return s32Ret;
}

/**
 \brief 设置对比度
 \attention \n
无

 \param[in] u32Contrast, 对比度, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetContrast(HI_UNF_DISP_E enChan, HI_U32 u32Contrast)
{
    if ( u32Contrast > 100)
    {
        HI_ERR_PQ("The Contrast is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetContrast((HI_DRV_DISPLAY_E)enChan, u32Contrast);
}

/**
 \brief 获取色调
 \attention \n
无

 \param[in]
 \param[out] pu32Hue：色调, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetHue(HI_UNF_DISP_E enChan, HI_U32 *pu32Hue)
{
    HI_S32 s32Ret;
    HI_U32 u32Hue = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32Hue);

    s32Ret = HI_MPI_PQ_GetHue((HI_DRV_DISPLAY_E)enChan, &u32Hue);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    *pu32Hue = u32Hue;
    return s32Ret;
}

/**
 \brief 设置色调
 \attention \n
无

 \param[in] u32Hue：色调, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetHue(HI_UNF_DISP_E enChan, HI_U32 u32Hue)
{
    if ( u32Hue > 100)
    {
        HI_ERR_PQ("The Hue level is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetHue((HI_DRV_DISPLAY_E)enChan, u32Hue);
}

/**
 \brief 设置图像基本参数
 \attention \n
无
 \param[in] enType: Graph or Video  CNcomment: 设置的是图形还是视频 CNend
 \param[in] enChan: Destination DISP channel  CNcomment: 目标通道号 CNend
 \param[in] stParam:Basic Image Param CNcomment:图像基本参数CNend


 \retval ::HI_SUCCESS CNcomment: 成功 CNend

 */


HI_S32 HI_UNF_PQ_SetBasicImageParam(HI_UNF_PQ_IMAGE_TYPE_E enType, HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_PARAM_S stParam)
{
    HI_PQ_IMAGE_PARAM_S stImageParam;

    stImageParam.u16Brightness = stParam.u32Brightness;
    stImageParam.u16Contrast   = stParam.u32Contrast;
    stImageParam.u16Hue        = stParam.u32Hue;
    stImageParam.u16Saturation = stParam.u32Saturation;

    return HI_MPI_PQ_SetBasicImageParam((HI_MPI_PQ_IMAGE_TYPE_E)enType, (HI_DRV_DISPLAY_E)enChan, stImageParam);
}

/**
 \brief 获取图像基本参数
 \attention \n
无
 \param[in] enType: Graph or Video  CNcomment: 选择的是图形还是视频 CNend
 \param[in] enChan: Destination DISP channel  CNcomment: 目标通道号 CNend
 \param[in] pstParam:Basic Image Param CNcomment:图像基本参数 CNend


 \retval ::HI_SUCCESS CNcomment: 成功 CNend

 */


HI_S32 HI_UNF_PQ_GetBasicImageParam(HI_UNF_PQ_IMAGE_TYPE_E enType, HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_PARAM_S *pstParam)
{
    HI_S32 s32Ret;
    HI_PQ_IMAGE_PARAM_S stImageParam = {0};

    MPI_PQ_CHECK_NULL_PTR(pstParam);

    s32Ret = HI_MPI_PQ_GetBasicImageParam((HI_MPI_PQ_IMAGE_TYPE_E)enType, (HI_DRV_DISPLAY_E)enChan, &stImageParam);

    pstParam->u32Brightness = stImageParam.u16Brightness;
    pstParam->u32Contrast   = stImageParam.u16Contrast;
    pstParam->u32Hue        = stImageParam.u16Hue;
    pstParam->u32Saturation = stImageParam.u16Saturation;

    return s32Ret;
}

/**
 \brief 获取饱和度
 \attention \n
无

 \param[out] pu32Saturation：饱和度, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetSaturation(HI_UNF_DISP_E enChan, HI_U32 *pu32Saturation)
{
    HI_S32 s32Ret;
    HI_U32 u32Saturation = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32Saturation);

    s32Ret = HI_MPI_PQ_GetSaturation((HI_DRV_DISPLAY_E)enChan, &u32Saturation);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    *pu32Saturation = u32Saturation;
    return s32Ret;
}

/**
 \brief 设置饱和度
 \attention \n
无

 \param[in] u32Saturation：饱和度,有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetSaturation(HI_UNF_DISP_E enChan, HI_U32 u32Saturation)
{
    if ( u32Saturation > 100)
    {
        HI_ERR_PQ("The Saturation level is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetSaturation((HI_DRV_DISPLAY_E)enChan, u32Saturation);
}

/**
 \brief 获取降噪强度
 \attention \n
无

 \param[out] pu32NRLevel: 降噪等级, 有效范围: 0~100


 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetNR(HI_UNF_DISP_E enChan, HI_U32 *pu32NRLevel)
{
    MPI_PQ_CHECK_NULL_PTR(pu32NRLevel);

    return HI_MPI_PQ_GetTNR(pu32NRLevel);
}

/**
 \brief 设置降噪强度
 \attention \n
无

 \param[in] u32NRLevel: 降噪等级, 有效范围: 0~100

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetNR(HI_UNF_DISP_E enChan, HI_U32 u32NRLevel)
{
    if ( u32NRLevel > 100)
    {
        HI_ERR_PQ("The NR level is out of range!\n");

        return HI_FAILURE;
    }

    HI_MPI_PQ_SetSNR(u32NRLevel);
    HI_MPI_PQ_SetTNR(u32NRLevel);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_PQ_GetDNR(HI_UNF_DISP_E enChan, HI_U32 *pu32DNRLevel)
{
    MPI_PQ_CHECK_NULL_PTR(pu32DNRLevel);

    return HI_MPI_PQ_GetDeBlocking(pu32DNRLevel);
}

HI_S32 HI_UNF_PQ_SetDNR(HI_UNF_DISP_E enChan, HI_U32 u32DNRLevel)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( u32DNRLevel > 100)
    {
        HI_ERR_PQ("The DNR level is out of range!\n");

        return s32Ret;
    }

    s32Ret  = HI_MPI_PQ_SetDeBlocking(u32DNRLevel);
    s32Ret |= HI_MPI_PQ_SetDeMosquito(u32DNRLevel);

    return s32Ret;
}


/**
 \brief 获取SR演示类型
 \attention \n
无

 \param[out] *penType


 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetSRMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_SR_DEMO_E *penType)
{
    MPI_PQ_CHECK_NULL_PTR(penType);


    return HI_MPI_PQ_GetSRMode((HI_PQ_SR_DEMO_E *)penType);
}

/**
 \brief 设置SR演示类型
 \attention \n
无

 \param[in] enType

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetSRMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_SR_DEMO_E enType)
{
    return HI_MPI_PQ_SetSRMode((HI_PQ_SR_DEMO_E)enType);
}

/**
 \brief 获取清晰度
 \attention \n
无

 \param[out] pu32Sharpness：清晰度, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetSharpness(HI_UNF_DISP_E enChan, HI_U32 *pu32Sharpness)
{
    HI_S32 s32Ret;
    HI_U32 u32Sharpness = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32Sharpness);

    s32Ret = HI_MPI_PQ_GetSharpness(&u32Sharpness);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    *pu32Sharpness = u32Sharpness;
    return s32Ret;
}

/**
 \brief 设置清晰度
 \attention \n
无

 \param[in] u32Sharpness：清晰度, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetSharpness(HI_UNF_DISP_E enChan, HI_U32 u32Sharpness)
{
    if ( u32Sharpness > 100)
    {
        HI_ERR_PQ("The Sharpness level is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetSharpness(u32Sharpness);
}


/**
 \brief 获取颜色增强
 \attention \n
无

 \param[out] pu32ColorGainLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetColorGain(HI_UNF_DISP_E enChan, HI_U32 *pu32ColorGainLevel)
{
    HI_S32 s32Ret;
    HI_U32 u32ColorGainLevel = 0;

    MPI_PQ_CHECK_NULL_PTR(pu32ColorGainLevel);

    s32Ret = HI_MPI_PQ_GetColorGain(&u32ColorGainLevel);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    *pu32ColorGainLevel = u32ColorGainLevel;
    return s32Ret;
}

/**
 \brief 设置颜色增强
 \attention \n
无

 \param[in] enColorGainLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetColorGain(HI_UNF_DISP_E enChan, HI_U32 u32ColorGainLevel)
{
    if ( u32ColorGainLevel > 100)
    {
        HI_ERR_PQ("The ColorGain level is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetColorGain(u32ColorGainLevel);
}

/**
 \brief 获取肤色增强
 \attention \n
  无

 \param[out] pu32FleshToneLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetFleshTone(HI_UNF_DISP_E enChan, HI_UNF_PQ_FLESHTONE_E *pu32FleshToneLevel)
{
    MPI_PQ_CHECK_NULL_PTR(pu32FleshToneLevel);

    return HI_MPI_PQ_GetFleshTone(pu32FleshToneLevel);
}

/**
 \brief 设置肤色增强
 \attention \n
  无

 \param[in] enFleshToneLevel，参考HI_COLOR_GAIN_E

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetFleshTone(HI_UNF_DISP_E enChan, HI_UNF_PQ_FLESHTONE_E enFleshToneLevel)
{
    if ( enFleshToneLevel >= HI_UNF_PQ_FLESHTONE_GAIN_BUTT)
    {
        HI_ERR_PQ("The FleshTone level is out of range!\n");

        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetFleshTone(enFleshToneLevel);
}

/**
 \brief 获取PQ模块开关
 \attention \n
  无

 \param[in] enFlags
 \param[in] pu32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetPQModule( HI_UNF_PQ_MODULE_E enFlags, HI_U32 *pu32OnOff)
{
    MPI_PQ_CHECK_NULL_PTR(pu32OnOff);

    if (enFlags >= HI_UNF_PQ_MODULE_BUTT)
    {
        return HI_FAILURE;
    }

    return HI_MPI_PQ_GetPQModule(enFlags, pu32OnOff);
}

/**
 \brief 设置PQ模块开关
 \attention \n
  无

 \param[in] enFlags
 \param[in] u32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetPQModule( HI_UNF_PQ_MODULE_E enFlags, HI_U32 u32OnOff)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_UNF_PQ_MODULE_BUTT <= enFlags)
    {
        return s32Ret;
    }

    s32Ret  = HI_MPI_PQ_SetPQModule(enFlags, u32OnOff);

    return s32Ret;
}

/**
 \brief 设置卖场模式开关
 \attention \n
无

 \param[in] enFlags
 \param[in] u32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetDemo( HI_UNF_PQ_DEMO_E enFlags, HI_U32 u32OnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    if (HI_UNF_PQ_DEMO_BUTT <= enFlags)
    {
        return HI_FAILURE;
    }

    if (HI_UNF_PQ_DEMO_ALL == enFlags)
    {
        for (i = HI_UNF_PQ_DEMO_SHARPNESS; i < HI_UNF_PQ_DEMO_ALL; i++)
        {
            s32Ret  = HI_MPI_PQ_SetDemo((HI_UNF_PQ_DEMO_E)i, u32OnOff);
        }
    }
    else
    {
        s32Ret  = HI_MPI_PQ_SetDemo(enFlags, u32OnOff);
    }

    return s32Ret;
}

/**
 \brief 设置卖场模式显示方式
 \attention \n
无

 \param[in] enChan
 \param[in] enMode

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetDemoMode( HI_UNF_DISP_E enChan, HI_UNF_PQ_DEMO_MODE_E enMode)
{
    return HI_MPI_PQ_SetDemoMode((HI_DRV_DISPLAY_E)enChan, (HI_PQ_DEMO_MODE_E)enMode);
}


/**
 \brief 获取卖场模式显示方式
 \attention \n
无

 \param[in] enChan
 \param[in] enMode

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetDemoMode( HI_UNF_DISP_E enChan, HI_UNF_PQ_DEMO_MODE_E *penMode)
{
    if (HI_NULL == penMode)
    {
        return HI_FAILURE;
    }

    return HI_MPI_PQ_GetDemoMode((HI_DRV_DISPLAY_E)enChan, (HI_PQ_DEMO_MODE_E *)penMode);
}

/**
 \brief 获取颜色增强的类型和强度
 \attention \n
无

 \param[out] pstColorEnhanceParam:颜色增强的类型和强度;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetColorEnhanceParam(HI_UNF_PQ_COLOR_ENHANCE_S *pstColorEnhanceParam)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_PQ_COLOR_ENHANCE_E    enType;
    HI_UNF_PQ_FLESHTONE_E enFleshToneLevel = HI_UNF_PQ_FLESHTONE_GAIN_OFF;
    HI_UNF_PQ_SIX_BASE_S stSixBase = {0};
    HI_UNF_PQ_COLOR_SPEC_MODE_E enColorMode = HI_UNF_PQ_COLOR_MODE_RECOMMEND;

    MPI_PQ_CHECK_NULL_PTR(pstColorEnhanceParam);

    enType = pstColorEnhanceParam->enColorEnhanceType;

    if (HI_UNF_PQ_COLOR_ENHANCE_FLESHTONE == enType)
    {
        s32Ret = HI_MPI_PQ_GetFleshTone(&enFleshToneLevel);
        if (HI_FAILURE == s32Ret )
        {
            return HI_FAILURE;
        }
        pstColorEnhanceParam->unColorGain.enFleshtone = enFleshToneLevel;
    }
    else if (HI_UNF_PQ_COLOR_ENHANCE_SIX_BASE == enType)
    {
        s32Ret = HI_MPI_PQ_GetSixBaseColor(&stSixBase);
        if (HI_FAILURE == s32Ret )
        {
            return HI_FAILURE;
        }

        pstColorEnhanceParam->unColorGain.stSixBase = stSixBase;
    }
    else if (HI_UNF_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE == enType)
    {
        s32Ret = HI_MPI_PQ_GetColorEnhanceMode(&enColorMode);
        if (HI_FAILURE == s32Ret )
        {
            return HI_FAILURE;
        }

        pstColorEnhanceParam->unColorGain.enColorMode = enColorMode;
    }

    return s32Ret;
}


/**
 \brief 设置颜色增强的类型和强度
 \attention \n
无

 \param[out] enColorEnhanceType:颜色增强的类型和强度;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_SetColorEnhanceParam(HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam)
{
    HI_UNF_PQ_COLOR_ENHANCE_E    enType;
    enType = stColorEnhanceParam.enColorEnhanceType;
    HI_UNF_PQ_SIX_BASE_S stSixBaseColor;
    HI_UNF_PQ_COLOR_SPEC_MODE_E enColorSpecMode;

    if (HI_UNF_PQ_COLOR_ENHANCE_FLESHTONE == enType)
    {
        return HI_MPI_PQ_SetFleshTone(stColorEnhanceParam.unColorGain.enFleshtone);
    }
    else if (HI_UNF_PQ_COLOR_ENHANCE_SIX_BASE == enType)
    {
        stSixBaseColor = stColorEnhanceParam.unColorGain.stSixBase;
        return HI_MPI_PQ_SetSixBaseColor((HI_PQ_SIX_BASE_COLOR_S *)&stSixBaseColor);
    }
    else if (HI_UNF_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE == enType)
    {
        enColorSpecMode = stColorEnhanceParam.unColorGain.enColorMode;
        return HI_MPI_PQ_SetColorEnhanceMode((HI_PQ_COLOR_SPEC_MODE_E)enColorSpecMode);
    }

    return HI_FAILURE;
}


/**
 \brief 获取DCI（动态对比度增强）的强度范围
 \attention \n
无

 \param[out] pu32DCIlevel:动态对比度等级, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_UNF_PQ_GetDynamicContrast(HI_U32 *pu32DCIlevel)
{
    MPI_PQ_CHECK_NULL_PTR(pu32DCIlevel);

    return HI_MPI_PQ_GetDciLevel(pu32DCIlevel);
}


/**
 \brief 设置DCI（动态对比度增强）的强度范围
 \attention \n
无

 \param[in] u32DCIlevel:动态对比度等级, 有效范围: 0~100;

 \retval ::HI_SUCCESS

 */
HI_S32 HI_UNF_PQ_SetDynamicContrast(HI_U32 u32DCIlevel)
{
    if (u32DCIlevel > 100)
    {
        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetDciLevel(u32DCIlevel);
}

HI_S32 HI_UNF_PQ_SetDemoCoordinate(HI_U32 u32X)
{
    if (u32X > 100)
    {
        return HI_FAILURE;
    }

    return HI_MPI_PQ_SetDemoCoordinate(u32X);
}

HI_S32 HI_UNF_PQ_GetDemoCoordinate(HI_U32 *pu32X)
{
    MPI_PQ_CHECK_NULL_PTR(pu32X);

    return HI_MPI_PQ_GetDemoCoordinate(pu32X);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

