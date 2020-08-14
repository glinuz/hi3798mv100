/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_unf_pq.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/4/23
  Description   :

******************************************************************************/

#ifndef __HI_UNF_PQ_H__
#define __HI_UNF_PQ_H__

#include "hi_type.h"
#include "hi_unf_disp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/******************************************************************************/


/********************************Struct Definition********************************/
/** \addtogroup      PQ */
/** @{ */  /** <!-- [PQ] */

/**<Image Mode Set Different Parameters Under Different Mode*/
/**<CNcomment:图像模式选择；在不同的模式下配置不同的参数 CNend*/
typedef enum hiUNF_PQ_IMAGE_MODE_E
{
    HI_UNF_OPTION_MODE_NATURAL = 0,  /**<Natural mode*/ /**<CNcomment:自然模式 CNend*/
    HI_UNF_OPTION_MODE_PERSON,       /**<Person mode*/ /**<CNcomment:人物模式 CNend*/
    HI_UNF_OPTION_MODE_FILM,         /**<Film mode*/ /**<CNcomment:电影模式 CNend*/
    HI_UNF_OPTION_MODE_UD,           /**<User Defined mode*/ /**<CNcomment:用户自定义模式 CNend*/
    HI_UNF_OPTION_MODE_NORMAL,       /**<Normal mode*/ /**<CNcomment:正常模式 CNend*/
    HI_UNF_OPTION_MODE_VIDEOPHONE,   /**<Video Phone mode*/ /**<CNcomment:可视电话模式 CNend*/
    HI_UNF_OPTION_MODE_BUTT,
} HI_UNF_PQ_IMAGE_MODE_E;

/**<HD and SD Channels Parameter*/
/**<CNcomment:高标清通道参数 CNend*/
typedef struct hiUNF_PQ_OPT_CHAN_S
{
    HI_U32       u32Brightness;         /**<Brightness; Range:0~100; recommended: 50 */ /**<CNcomment:亮度；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Contrast;           /**<Contrast; Range:0~100; recommended: 50 */ /**<CNcomment:对比度；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Hue;                /**<HuePlus; Range:0~100; recommended: 50 */ /**<CNcomment:色调；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Saturation;         /**<Saturation; Range:0~100; recommended: 50 */ /**<CNcomment:饱和度；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Colortemperature;   /**<Color temperature mode; Range:0~2 Cold,Middle,Warm */ /**<CNcomment: 色温模式；范围: 0~2 冷色温,中间色温,暖色温 CNend*/
    HI_U32       u32GammaMode;          /**<Gamma mode; Range:0~4 Intension_1.8,Intension_2.5,Curve_Light,Curve_Dark, Disabled */ /**<CNcomment:伽马模式选择；范围: 0~4 1.8强度,2.5强度,亮,暗,关闭 CNend*/
    HI_U32       u32DynamicContrast;    /**<Color Dynamic Contrast mode; Range:0~3 Low, Middle,High,Disabled */ /**<CNcomment:动态对比度；范围: 0~3 低,中,高,关闭 CNend*/
    HI_U32       u32IntelligentColor;   /**<Color Intelligent Color; Range:0~5 Blue,Green,BG,Skin,Vivid,Disabled */ /**<CNcomment:智能颜色管理；范围: 0~5 蓝色,绿色,蓝绿增强,肤色,生动,关闭 CNend*/
} HI_UNF_PQ_OPT_CHAN_S;

/**<Common Parameter*/
/**<CNcomment:通用参数 CNend*/
typedef struct hiUNF_PQ_OPT_COMMON_S
{
    HI_U32       u32Sharpeness;      /**<Sharpeness; Range:0~100; recommended: 50 */ /**<CNcomment:锐化强度；范围:0~100；推荐值:50 CNend*/
    HI_U32       u32Denoise;         /**<Denoise; Range:0~1 Disabled,Auto */ /**<CNcomment:降噪使能；范围:0~1 关闭,打开 CNend*/
    HI_U32       u32FilmMode;        /**<FileMode; Range:0~1 Disabled,Auto */ /**<CNcomment:电影模式使能选择；范围:0~1 关闭,打开 CNend*/

} HI_UNF_PQ_OPT_COMMON_S;

/**<Interface Type and Interface Ialue*/
/**<CNcomment:接口类型和通道参数 CNend*/
typedef struct hiUNF_PQ_OPT_CHANS_S
{
    HI_UNF_DISP_E enChan;               /**<interface type */ /**<CNcomment:接口类型 CNend*/
    HI_UNF_PQ_OPT_CHAN_S stChanOpt;     /**<interface value*/ /**<CNcomment:通道参数 CNend*/
} HI_UNF_PQ_OPT_CHANS_S;


/******************************************************************************/

/**<Demo Mode*/
/**<CNcomment:卖场模式 CNend*/
typedef enum hiUNF_PQ_DEMO_E
{
    HI_UNF_PQ_DEMO_SHARPNESS = 0,   /**<Sharpen */ /**<CNcomment:锐化 CNend*/
    HI_UNF_PQ_DEMO_DCI,             /**<Dynamic Contrast Improvement */ /**<CNcomment:自动对比度调节 CNend*/
    HI_UNF_PQ_DEMO_COLOR,           /**<Automatic Color Management */ /**<CNcomment:智能颜色调节 CNend*/
    HI_UNF_PQ_DEMO_SR,              /**<Super Resolution */ /**<CNcomment:超级分辨率 CNend*/
    HI_UNF_PQ_DEMO_TNR,             /**<Time Noise Reduction */ /**<CNcomment:时域噪声消除 CNend*/
    HI_UNF_PQ_DEMO_DEI,             /**<De interlace */ /**<CNcomment:去隔行 CNend*/
    HI_UNF_PQ_DEMO_DBM,             /**<Digital Noise Reduction; include DB DM DR DS*/ /**<CNcomment:数字噪声消除 CNend*/
    HI_UNF_PQ_DEMO_SNR,             /**<Space Noise Reduction */ /**<CNcomment:空域噪声消除 CNend*/
    HI_UNF_PQ_DEMO_ALL,             /**<All Algorithm */ /**<CNcomment:全部算法 CNend*/

    HI_UNF_PQ_DEMO_BUTT
} HI_UNF_PQ_DEMO_E;

/**<Demo Display Mode*/
/**<CNcomment:卖场显示模式 CNend*/
typedef enum hiUNF_PQ_DEMO_MODE_E
{
    HI_UNF_PQ_DEMO_MODE_FIXED_R,    /**<Fixed Enable Right */ /**<CNcomment:固定右侧使能 CNend*/
    HI_UNF_PQ_DEMO_MODE_FIXED_L,    /**<Fixed Enable Left */ /**<CNcomment:固定左侧使能 CNend*/
    HI_UNF_PQ_DEMO_MODE_SCROLL_R,   /**<Enable Right; and Roll from Left to right */ /**<CNcomment:右侧使能；从左向右移动 CNend*/
    HI_UNF_PQ_DEMO_MODE_SCROLL_L,   /**<Enable Left; and Roll from Left to right */ /**<CNcomment:左侧使能；从左向右移动 CNend*/

    HI_UNF_PQ_DEMO_MODE_BUTT
} HI_UNF_PQ_DEMO_MODE_E;

/**<Algorithm Moudle*/
/**<CNcomment:算法模块 CNend*/
typedef enum hiUNF_PQ_MODULE_E
{
    HI_UNF_PQ_MODULE_SHARPNESS = 0, /**<Sharpen */ /**<CNcomment:锐化 CNend*/
    HI_UNF_PQ_MODULE_DCI,           /**<Dynamic Contrast Improvement */ /**<CNcomment:自动对比度调节 CNend*/
    HI_UNF_PQ_MODULE_COLOR,         /**<Automatic Color Management */ /**<CNcomment:智能颜色调节 CNend*/
    HI_UNF_PQ_MODULE_SR,            /**<Super Resolution */ /**<CNcomment:超级分辨率 CNend*/
    HI_UNF_PQ_MODULE_TNR,           /**<Time Noise Reduction */ /**<CNcomment:时域噪声消除 CNend*/
    HI_UNF_PQ_MODULE_DBM,           /**<Digital Noise Reduction; include DB DM DR DS*/ /**<CNcomment:数字噪声消除 CNend*/
    HI_UNF_PQ_MODULE_SNR,           /**<Space Noise Reduction */ /**<CNcomment:空域噪声消除 CNend*/
    HI_UNF_PQ_MODULE_MCDI,          /**<Motion Compensation DeInterlace */ /**<CNcomment:基于运动补偿的去隔行算法 CNend*/

    HI_UNF_PQ_MODULE_ALL,           /**<All Algorithm */ /**<CNcomment:全部算法 CNend*/

    HI_UNF_PQ_MODULE_BUTT
} HI_UNF_PQ_MODULE_E;

/**<Spuer Resolution Demo Mode*/
/**<CNcomment:SR演示模式 CNend*/
typedef enum hiUNF_PQ_SR_DEMO_E
{
    HI_UNF_PQ_SR_DISABLE  = 0, /**<Close SR */ /**<CNcomment:关SR,只开缩放 CNend*/
    HI_UNF_PQ_SR_ENABLE_R,     /**<SR open at right hand side */ /**<CNcomment:右边SR CNend*/
    HI_UNF_PQ_SR_ENABLE_L,     /**<SR open at left hand side */ /**<CNcomment:左边SR CNend*/
    HI_UNF_PQ_SR_ENABLE_A,     /**<SR open all screen */ /**<CNcomment:全屏SR CNend*/

    HI_UNF_PQ_SR_DEMO_BUTT
} HI_UNF_PQ_SR_DEMO_E;

/**<Custom Colors Parameter*/
/**<CNcomment:自定义颜色参数 CNend*/
typedef struct  hiUNF_PQ_SIX_BASE_S
{
    HI_U32  u32Red;       /**<Red; Range:0~100 */ /**<CNcomment:红色；范围:0~100 CNend*/
    HI_U32  u32Green;     /**<Green; Range:0~100 */ /**<CNcomment:红色；范围:0~100 CNend*/
    HI_U32  u32Blue;      /**<Blue; Range:0~100 */ /**<CNcomment:红色；范围:0~100 CNend*/

    HI_U32  u32Cyan;      /**<Cyan; Range:0~100 */ /**<CNcomment:青色；范围:0~100 CNend*/
    HI_U32  u32Magenta;   /**<Magenta; Range:0~100 */ /**<CNcomment:品红；范围:0~100 CNend*/
    HI_U32  u32Yellow;    /**<Yellow; Range:0~100 */ /**<CNcomment:黄色；范围:0~100 CNend*/
} HI_UNF_PQ_SIX_BASE_S;

/**<Portrait Colors Parameter*/
/**<CNcomment:人像模式参数 CNend*/
typedef enum hiUNF_PQ_FLESHTONE_E
{
    HI_UNF_PQ_FLESHTONE_GAIN_OFF = 0,   /**<Portrait Model Strength Off */ /**<CNcomment:人像模式关闭 CNend*/
    HI_UNF_PQ_FLESHTONE_GAIN_LOW,       /**<Portrait Model Strength Low */ /**<CNcomment:人像模式强度低 CNend*/
    HI_UNF_PQ_FLESHTONE_GAIN_MID,       /**<Portrait Model Strength Middle */ /**<CNcomment:人像模式强度中 CNend*/
    HI_UNF_PQ_FLESHTONE_GAIN_HIGH,      /**<Portrait Model Strength High */ /**<CNcomment:人像模式强度高 CNend*/

    HI_UNF_PQ_FLESHTONE_GAIN_BUTT
}  HI_UNF_PQ_FLESHTONE_E;

/**<Color Optimization Model*/
/**<CNcomment:颜色增强类型 CNend*/
typedef enum hiUNF_PQ_COLOR_SPEC_MODE_E
{
    HI_UNF_PQ_COLOR_MODE_RECOMMEND = 0, /**<Optimization Model */ /**<CNcomment:最优模式 CNend*/
    HI_UNF_PQ_COLOR_MODE_BLUE,          /**<Blue Model */ /**<CNcomment:蓝色增强模式 CNend*/
    HI_UNF_PQ_COLOR_MODE_GREEN,         /**<Green Model */ /**<CNcomment:绿色增强模式 CNend*/
    HI_UNF_PQ_COLOR_MODE_BG,            /**<Cyan Model */ /**<CNcomment:蓝绿增强模式 CNend*/
    HI_UNF_PQ_COLOR_MODE_ORIGINAL,      /**<Original Model */ /**<CNcomment:原始颜色模式 CNend*/

    HI_UNF_PQ_COLOR_MODE_BUTT
} HI_UNF_PQ_COLOR_SPEC_MODE_E;

/**<Intelligent Colors Form*/
/**<CNcomment:智能颜色类型 CNend*/
typedef enum hiUNF_PQ_COLOR_ENHANCE_E
{
    HI_UNF_PQ_COLOR_ENHANCE_FLESHTONE = 0,    /**<Portrait Model */ /**<CNcomment:人像模式 CNend*/
    HI_UNF_PQ_COLOR_ENHANCE_SIX_BASE,         /**<Custom Model */ /**<CNcomment:自定义颜色 CNend*/
    HI_UNF_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE,  /**<Special Color Model */ /**<CNcomment:颜色增强模式 CNend*/

    HI_UNF_PQ_COLOR_ENHANCE_BUTT
} HI_UNF_PQ_COLOR_ENHANCE_E;

/**<Intelligent Colors Parameter*/
/**<CNcomment:智能颜色参数 CNend*/
typedef struct  hiUNF_PQ_COLOR_ENHANCE_S
{
    HI_UNF_PQ_COLOR_ENHANCE_E  enColorEnhanceType;  /**<Color Model */ /**<CNcomment增强类型 CNend*/
    union
    {
        HI_UNF_PQ_FLESHTONE_E        enFleshtone;   /**<Portrait Model Parameter */ /**<CNcomment人像模式参数 CNend*/
        HI_UNF_PQ_SIX_BASE_S         stSixBase;     /**<Custom Model Parameter */ /**<CNcomment自定义颜色参数 CNend*/
        HI_UNF_PQ_COLOR_SPEC_MODE_E  enColorMode;   /**<Special Color Model Parameter */ /**<CNcomment颜色增强参数 CNend*/
    } unColorGain;
} HI_UNF_PQ_COLOR_ENHANCE_S;

typedef enum hiUNF_PQ_IMAGE_TYPE_E
{
    HI_UNF_PQ_IMAGE_GRAPH = 0,          /**<graph image */ /**<CNcomment: 图形图像 CNend*/
    HI_UNF_PQ_IMAGE_VIDEO,              /**<video image */ /**<CNcomment: 视频图像 CNend*/

    HI_UNF_PQ_IMAGE_BUTT
} HI_UNF_PQ_IMAGE_TYPE_E;

typedef struct hiUNF_PQ_IMAGE_PARAM_S
{
    HI_U32       u32Brightness;         /**<Brightness; Range:0~100; recommended: 50 */ /**<CNcomment:亮度；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Contrast;           /**<Contrast; Range:0~100; recommended: 50 */ /**<CNcomment:对比度；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Hue;                /**<HuePlus; Range:0~100; recommended: 50 */ /**<CNcomment:色调；范围: 0~100；推荐值: 50 CNend*/
    HI_U32       u32Saturation;         /**<Saturation; Range:0~100; recommended: 50 */ /**<CNcomment:饱和度；范围: 0~100；推荐值: 50 CNend*/
} HI_UNF_PQ_IMAGE_PARAM_S;

/******************************* API Declaration *****************************/
/** \addtogroup      PQ */
/** @{ */  /** <!-- [PQ] */

/******************************************************************************/
/**
 \brief Set PQ mode . CNcomment: 设置图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] enImageMode Destination DISP channel PQ mode CNcomment: 目标通道图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_SetImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E enImageMode);


/**
 \brief Get PQ mode . CNcomment: 获取图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] penImageMode  pointer of image mode CNcomment: 指针类型，指向图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_GetImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E* penImageMode);


/**
 \brief Init PQ mode . CNcomment: 初始化图像模式 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] penImageMode  pointer of image mode CNcomment: 指针类型，指向图像模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_InitImageMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_MODE_E enImageMode);


/**
 \brief Set channel option. CNcomment: 设置通道option值 CNend
 \attention \n
 \param[in] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_SetChanOption(const HI_UNF_PQ_OPT_CHANS_S* pstChanOption);


/**
 \brief Set channel option. CNcomment: 获取通道option值 CNend
 \attention \n
 \param[out] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_GetChanOption(HI_UNF_PQ_OPT_CHANS_S* pstChanOption);


/**
 \brief Set channel common option. CNcomment: 设置通道 common option值 CNend
 \attention \n
 \param[in] pstCommOption pointer of channel common option CNcomment: 指针类型，指向通道common option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_SetCommOption(const HI_UNF_PQ_OPT_COMMON_S* pstCommOption);


/**
 \brief Set channel option. CNcomment: 获取通道common option值 CNend
 \attention \n
 \param[out] pstChanOption pointer of channel option CNcomment: 指针类型，指向通道common option值 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_GetCommOption(HI_UNF_PQ_OPT_COMMON_S* pstCommOption);


/**
 \brief Modifies the basic configuration information.  CNcomment:更新PQ配置区信息 CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
 N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_PQ_UpdatePqParam(HI_VOID);


/**
 \brief Set the default PQ configuration for video parameter test.  CNcomment: 为入网指标测试设置PQ 的默认值CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
 N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_PQ_SetDefaultParam(HI_VOID);


/**
 \brief Initializes the picture quality unit (PQU). CNcomment: 初始化PQ CNend
 \attention \n
Before calling the PQU, you must call this application programming interface (API).
CNcomment: 调用PQ模块要求首先调用该接口 CNend
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_PQ_DEV_NOT_EXIST No PQ device exists. CNcomment: 设备不存在 CNend
 \retval ::HI_ERR_PQ_NOT_DEV_FILE The file is not a PQ file. CNcomment: 文件非设备 CNend
 \retval ::HI_ERR_PQ_DEV_OPENED The PQ device fails to be started. CNcomment: 打开设备失败 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_Init(HI_VOID);


/**
 \brief Deinitializes the PQU. CNcomment: 去初始化PQ CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_PQ_DeInit(HI_VOID);


/**
 \brief Get Brightness. CNcomment:获取亮度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] pu32Brightness   CNcomment: 亮度值；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetBrightness(HI_UNF_DISP_E enChan, HI_U32* pu32Brightness);


/**
 \brief Set Brightness. CNcomment:设置亮度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32Brightness; Range:0~100 CNcomment: 亮度值；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetBrightness(HI_UNF_DISP_E enChan, HI_U32 u32Brightness);


/**
 \brief Get Contrast. CNcomment:获取对比度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel  CNcomment: 目标通道号 CNend
 \param[out] pu32Contrast; Range:0~100  CNcomment: 对比度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetContrast(HI_UNF_DISP_E enChan, HI_U32* pu32Contrast);


/**
 \brief Set Contrast. CNcomment:设置对比度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32Contrast; Range:0~100  CNcomment: 对比度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetContrast(HI_UNF_DISP_E enChan, HI_U32 u32Contrast);


/**
 \brief Get Saturation. CNcomment:获取饱和度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] pu32Saturation; Range:0~100  CNcomment: 饱和度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetSaturation(HI_UNF_DISP_E enChan, HI_U32* pu32Saturation);


/**
 \brief Set Saturation. CNcomment:设置饱和度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32Saturation; Range:0~100  CNcomment: 饱和度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetSaturation(HI_UNF_DISP_E enChan, HI_U32 u32Saturation);


/**
 \brief Get Hue. CNcomment:获取色调 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] pu32Hue; Range:0~100  CNcomment: 色调；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetHue(HI_UNF_DISP_E enChan, HI_U32* pu32Hue);


/**
 \brief Set Hue. CNcomment:设置色调 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32Hue; Range:0~100  CNcomment: 色调；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success  CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetHue(HI_UNF_DISP_E enChan, HI_U32 u32Hue);


/**
 \brief Set Basic image para. CNcomment:设置图像基本参数. CNend
 \attention \n
 \param[in] enType: Graph or Video  CNcomment: 设置的是图形还是视频 CNend
 \param[in] enChan: Destination DISP channel  CNcomment: 目标通道号 CNend
 \param[in] stParam:Basic Image Param CNcomment:图像基本参数CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetBasicImageParam(HI_UNF_PQ_IMAGE_TYPE_E enType, HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_PARAM_S stParam);


/**
 \brief Get Basic image para. CNcomment:获取图像基本参数 CNend
 \attention \n
 \param[in] enType: Graph or Video  CNcomment: 选择的是图形还是视频 CNend
 \param[in] enChan: Destination DISP channel  CNcomment: 目标通道号 CNend
 \param[in] pstParam:Basic Image Param CNcomment:图像基本参数 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetBasicImageParam(HI_UNF_PQ_IMAGE_TYPE_E enType, HI_UNF_DISP_E enChan, HI_UNF_PQ_IMAGE_PARAM_S* pstParam);


/**
 \brief Get SR Demo Mode. CNcomment:获取SR演示类型 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] *penType, SR Demo Mode  CNcomment:指针，SR 显示模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetSRMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_SR_DEMO_E* penType);


/**
 \brief  Set SR Demo Mode. CNcomment:设置SR演示类型 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] enType, SR Demo Mode  CNcomment:SR 显示模式 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetSRMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_SR_DEMO_E enType);


/**
 \brief Get Sharpness. CNcomment:获取清晰度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] pu32Sharpness; Range:0~100  CNcomment: 清晰度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetSharpness(HI_UNF_DISP_E enChan, HI_U32* pu32Sharpness);


/**
 \brief Set Sharpness. CNcomment:设置清晰度 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32Sharpness; Range:0~100  CNcomment: 清晰度；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS  Success CNcomment: 成功 CNend
 */
HI_S32 HI_UNF_PQ_SetSharpness(HI_UNF_DISP_E enChan, HI_U32 u32Sharpness);


/**
 \brief Get Color Enhance Gain Level. CNcomment:获取颜色增强值 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[out] pu32ColorGainLevel; Range:0~100  CNcomment: 颜色增强值；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetColorGain(HI_UNF_DISP_E enChan, HI_U32* pu32ColorGainLevel);


/**
 \brief Set Color Enhance Gain Level. CNcomment:设置颜色增强值 CNend
 \attention \n
 \param[in] enChan Destination DISP channel CNcomment: 目标通道号 CNend
 \param[in] u32ColorGainLevel; Range:0~100  CNcomment: 颜色增强值；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetColorGain(HI_UNF_DISP_E enChan, HI_U32 u32ColorGainLevel);


/**
 \brief Get Color Enhance Type and Para. CNcomment:获取颜色增强的类型和参数 CNend
 \attention \n
 \param[out] pstColorEnhanceParam  CNcomment:颜色增强的类型和参数 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetColorEnhanceParam(HI_UNF_PQ_COLOR_ENHANCE_S* pstColorEnhanceParam);


/**
 \brief Set Color Enhance Type and Para. CNcomment:设置颜色增强的类型和参数 CNend
 \attention \n
 \param[out] stColorEnhanceParam  CNcomment:颜色增强的类型和参数 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetColorEnhanceParam(HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam);


/** Dynamic Contrast Improvement(DCI)
 \brief Get Dynamic Contrast Improvement(DCI) Range  CNcomment:获取DCI（动态对比度增强）的强度范围 CNend
 \attention \n
 \param[out] pu32DCIlevel; Range:0~100 CNcomment:动态对比度等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetDynamicContrast(HI_U32* pu32DCIlevel);


/**
 \brief Get DCI Range  CNcomment:获取DCI强度范围 CNend
 \attention \n
 \param[in] u32DCIlevel; Range:0~100 CNcomment:动态对比度等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetDynamicContrast(HI_U32 u32DCIlevel);


/**
 \brief Get noise reduction strength CNcomment:获取降噪强度 CNend
 \attention \n
 \param[out] pu32NRLevel: noise reduction level; Range:0~100 CNcomment:降噪等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetNR(HI_UNF_DISP_E enChan, HI_U32* pu32NRLevel);


/**
 \brief  Set noise reduction strength CNcomment:设置降噪强度 CNend
 \attention \n
 \param[in] u32NRLevel: noise reduction level; Range:0~100 CNcomment:降噪等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetNR(HI_UNF_DISP_E enChan, HI_U32 u32NRLevel);

/**
 \brief Get digital noise reduction strength CNcomment:获取数字降噪强度 CNend
 \attention \n
 \param[out] pu32NRLevel: noise reduction level; Range:0~100 CNcomment:数字降噪等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_GetDNR(HI_UNF_DISP_E enChan, HI_U32* pu32DNRLevel);


/**
 \brief  Set digital noise reduction strength CNcomment:设置数字降噪强度 CNend
 \attention \n
 \param[in] u32NRLevel: noise reduction level; Range:0~100 CNcomment:数字降噪等级；有效范围: 0~100 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend

 */
HI_S32 HI_UNF_PQ_SetDNR(HI_UNF_DISP_E enChan, HI_U32 u32DNRLevel);



/**
 \brief  Set Algorithm Moudle on-off  CNcomment:设置算法模块开关 CNend
 \attention \n
 \param[in] enFlags   CNcomment:算法模块 CNend
 \param[in] u32OnOff  CNcomment:算法开关 CNend
 \retval ::HI_SUCCESS Success CNcomment:成功 CNend
 */
HI_S32 HI_UNF_PQ_SetPQModule(HI_UNF_PQ_MODULE_E enFlags, HI_U32 u32OnOff);


/**
 \brief Get Algorithm Moudle on-off  CNcomment:获取算法模块开关 CNend
 \attention \n
 \param[in]  enFlags   CNcomment:算法模块 CNend
 \param[out] pu32OnOff CNcomment:算法开关 CNend
 \retval ::HI_SUCCESS  Success CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_GetPQModule(HI_UNF_PQ_MODULE_E enFlags, HI_U32* pu32OnOff);


/**
 \brief Set Demo Mode on-off  CNcomment:设置卖场模式开关 CNend
 \attention \n
 \param[in] enFlags   CNcomment:算法模块 CNend
 \param[in] u32OnOff  CNcomment:卖场模式开关 CNend
 \retval ::HI_SUCCESS CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_SetDemo(HI_UNF_PQ_DEMO_E enFlags, HI_U32 u32OnOff);


/**
 \brief Set Demo Display Mode  CNcomment:设置卖场显示模式 CNend
 \attention \n
 \param[in] enChan    CNcomment:显示通道 CNend
 \param[in] enMode    CNcomment:卖场显示模式 CNend
 \retval ::HI_SUCCESS Success CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_SetDemoMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_DEMO_MODE_E enMode);


/**
 \brief Get Demo Display Mode  CNcomment:获取卖场显示模式 CNend
 \attention \n
 \param[in] enChan    CNcomment:显示通道 CNend
 \param[out] enMode   CNcomment:卖场显示模式 CNend
 \retval ::HI_SUCCESS Success CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_GetDemoMode(HI_UNF_DISP_E enChan, HI_UNF_PQ_DEMO_MODE_E* penMode);

/**
 \brief Set Demo Display Mode Coordinate CNcomment:设置卖场显示坐标 CNend
 \attention \n
 \param[in] u32X Range:0~100 Default:50 CNcomment:卖场显示分界线横坐标 有效范围:0~100 默认值:50 CNend
 \retval ::HI_SUCCESS Success CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_SetDemoCoordinate(HI_U32 u32X);


/**
 \brief Get Demo Display Mode Coordinate CNcomment:获取卖场显示模式坐标 CNend
 \attention \n
 \param[out] pu32X  Range:0~100 Default:50 CNcomment:卖场显示分界线横坐标 有效范围:0~100 默认值:50 CNend
 \retval ::HI_SUCCESS Success CNcomment:成功 CNend

 */
HI_S32 HI_UNF_PQ_GetDemoCoordinate(HI_U32* pu32X);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __HI_UNF_PQ_H__ */


