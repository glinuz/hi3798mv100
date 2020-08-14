/******************************************************************************

  Copyright (C), 2015-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_tee_video_watermark.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/06/08
  Description   :
  History       :
  1.Date        : 2016/06/08
    Author      : l00185424
    Modification: Created file

*******************************************************************************/
/**
 * \file
 * \brief Describes the API about the video watermark.
          CNcomment:提供视频水印API CNend
 */
#ifndef __HI_TEE_VIDEO_WATERMARK_H__
#define __HI_TEE_VIDEO_WATERMARK_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      VIDEO_WATERMARK */
/** @{ */  /** <!-- [VIDEO_WATERMARK] */

/**Video watermark type */
/**CNcomment:视频水印类型 */
typedef enum hiTEE_VWM_TYPE_E
{
    HI_TEE_VWM_TYPE_NEXGUARD = 1,   /**<Nexguard*/
    HI_TEE_VWM_TYPE_VERIMATRIX = 2, /**<Verimatrix*/
    HI_TEE_VWM_TYPE_BUG
}HI_TEE_VWM_TYPE_E;

/**Nexguard watermark setting data type */
/**CNcomment:Nexguard视频水印参数类型 */
typedef enum hiTEE_VWM_NEXGUARD_SETTING_TYPE_E
{
    HI_TEE_VWM_NEXGUARD_SETTING_8bit =0,        /**<8bit setting*/
    HI_TEE_VWM_NEXGUARD_SETTING_10bit,          /**<10bit setting*/
    HI_TEE_VWM_NEXGUARD_SETTING_HDR,            /**<HDR setting*/
    HI_TEE_VWM_NEXGUARD_SETTING_CERTIFICATE,    /**<Certificate setting*/
    HI_TEE_VWM_NEXGUARD_SETTING_BUTT
}HI_TEE_VWM_NEXGUARD_SETTING_TYPE_E;

/**Video watermark instance attribute */
/**CNcomment:视频水印实例属性 */
typedef struct hiTEE_VWM_INS_ATTR_S
{
    HI_TEE_VWM_TYPE_E enType;       /**<Watermark type */
    HI_U32 u32ServiceId;            /**<Service ID */
    HI_VOID (*pfnGetSetting)(HI_TEE_VWM_NEXGUARD_SETTING_TYPE_E enSettingType, HI_U8 *pu8Data, HI_U32 u32BufLen, HI_U32 *pu32OutputLen);
                                    /**<Get setting data callback, only available for Nexguard*/
}HI_TEE_VWM_INS_ATTR_S;

/**Nexguard watermark control parameter */
/**CNcomment:Nexguard视频水印参数 */
typedef struct hiTEE_VWM_NEXGUARD_PARAM_S
{
    HI_BOOL bEnable;                /**<Enable watermark or not */
    HI_U32 u32Key;                  /**<If use reg key, config here; if use OTP key, don't care. */
    HI_U32 u32SubscriberID;         /**<If use reg SubscriberID, config here; if use OTP SubscriberID, don't care. */
    HI_U8 u8OperatorID;             /**<If use reg peratorID, config here; if use OTP peratorID, don't care. */
}HI_TEE_VWM_NEXGUARD_PARAM_S;

/**Verimatrix watermark control parameter */
/**CNcomment:Verimatrix视频水印参数 */
typedef struct hiTEE_VWM_VERIMATRIX_PARAM_S
{
    HI_U8 version_major;
    HI_U8 version_minor;

    //embedding part
    HI_U8 watermark_on;
    HI_U8 frequency_distance[3][3];
    HI_U8 background_embedding_on;
    HI_U32 embedding_strength_threshold_8[3];
    HI_U32 embedding_strength_threshold_bg_8[3];
    HI_U32 embedding_strength_threshold_10[12];
    HI_U32 embedding_strength_threshold_bg_10[12];
    HI_U32 embedding_strength_threshold_12[48];
    HI_U32 embedding_strength_threshold_bg_12[48];
    HI_U32 direction_max;
    HI_U32 strength_multiply;

    //rendering part
    HI_U8 payload_symbols[1920];
    HI_U32 symbols_rows;
    HI_U32 symbols_cols;
    HI_U32 symbols_xpos;
    HI_U32 symbols_ypos;
    HI_U32 symbol_size;
    HI_U32 spacing_vert;
    HI_U32 spacing_horz;
    HI_U32 symbol_scale_control;

    HI_U32 symbol_phy_addr;
}HI_TEE_VWM_VERIMATRIX_PARAM_S;

/**Watermark parameter */
/**CNcomment:视频水印参数 */
typedef union hiTEE_VWM_PARAM_U
{
    HI_TEE_VWM_NEXGUARD_PARAM_S     stNexguard;     /**<Nexguard watermark parameter */
    HI_TEE_VWM_VERIMATRIX_PARAM_S   stVerimatrix;   /**<Verimatrix watermark parameter */
}HI_TEE_VWM_PARAM_U;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      VIDEO_WATERMARK */
/** @{ */  /** <!-- [VIDEO_WATERMARK] */

/**
\brief Initializes the video watermark module.CNcomment:初始化VMW模块 CNend
\attention \n
Before calling ::HI_TEE_VWM_Create to create an watermark instance, you must call this API to initialize the module.
CNcomment 在调用::HI_TEE_VWM_Create创建水印实例前，要求首先调用本接口以初始化模块 CNend
\param[in] enType   Video watermark type. It can accept one type or HI_TEE_VWM_TYPE_NEXGUARD | HI_TEE_VWM_TYPE_VERIMATRIX.
CNcomment:需初始化的视频水印类型,可以接受一种或多种类型(HI_TEE_VWM_TYPE_NEXGUARD | HI_TEE_VWM_TYPE_VERIMATRIX) . CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_Init(HI_TEE_VWM_TYPE_E enType);

/**
\brief Deinitializes the video watermark module.CNcomment:去初始化VMW模块 CNend
\attention \n
Before calling ::HI_TEE_VWM_Create to create an watermark instance, you must call this API to initialize the module.
CNcomment 在调用::HI_TEE_VWM_Create创建水印实例前，要求首先调用本接口以初始化模块 CNend
\param[in] enType   Video watermark type. It can accept one type or HI_TEE_VWM_TYPE_NEXGUARD | HI_TEE_VWM_TYPE_VERIMATRIX.
CNcomment:需去初始化的视频水印类型,可以接受一种或多种类型(HI_TEE_VWM_TYPE_NEXGUARD | HI_TEE_VWM_TYPE_VERIMATRIX) . CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_DeInit(HI_TEE_VWM_TYPE_E enType);

/**
\brief Create a video watermark instance.CNcomment:创建一个视频水印实例 CNend
\attention \n
You need indicate which type to use. Don't need to care u32ServiceId if you use Nexguard.
CNcomment 需指定水印类型;如果是Nexguard,不需要关心u32ServiceId参数 CNend
\param[in] pstInstAttr  Video watermark instance attribute. CNcomment:视频水印实例参数. CNend
\param[out] phVWM       Output the handle of video watermark instance. CNcomment:视频水印实例句柄. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_Create(HI_TEE_VWM_INS_ATTR_S *pstInstAttr, HI_HANDLE *phVWM);

/**
\brief Destroy a video watermark instance.CNcomment:销毁一个视频水印实例 CNend
\attention \n
    None. CNcomment 无 CNend
\param[in] hVWM     Video watermark instance handle. CNcomment:视频水印实例句柄. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_Destroy(HI_HANDLE hVWM);

/**
\brief Set video watermark parameter and start/stop it.CNcomment:设置水印参数以及启动/停止水印控制 CNend
\attention \n
    None. CNcomment 无 CNend
\param[in] hVWM         Video watermark instance handle. CNcomment:视频水印实例句柄. CNend
\param[in] punParam     Video watermark parameter. CNcomment:视频水印实例参数. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_SetParameter(HI_HANDLE hVWM, HI_TEE_VWM_PARAM_U *punParam);

/**
\brief Get video watermark parameter.CNcomment:获取水印参数 CNend
\attention \n
    None. CNcomment 无 CNend
\param[in] hVWM         Video watermark instance handle. CNcomment:视频水印实例句柄. CNend
\param[out] punParam    Video watermark parameter. CNcomment:视频水印实例参数. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_INVALID_PARAM  Invalid input parameter. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_TEE_VWM_GetParameter(HI_HANDLE hVWM, HI_TEE_VWM_PARAM_U *punParam);


/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif

