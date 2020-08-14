/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_effect.h
Version          : Initial Draft
Author           :
Created          : 2016/06/28
Description      :
Function List    :


History          :
Date                Author                Modification
2014/06/28          y00181162              Created file
******************************************************************************/

#ifndef __HI_EFFECT_H__
#define __HI_EFFECT_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include <hi_type.h>
#include <hi_go.h>



/***************************** Macro Definition ******************************/
/** \addtogroup      Effect */
/** @{ */  /** <!-- [Effect] */

/**Definition of the HiEffect error ID*/
/** CNcomment:HiEffect 项目错误ID */
#define HIEFFECT_ERR_APPID (0x80000000L + 0x40000000L)

/**Definition of the HiEffect module ID*/
/** CNcomment:HiEffect 模块ID */
#define HIEFFECT_ERR_MODUID 0x00000000

/** @} */  /*! <!-- Macro Definition end */

/*************************** Structure Definition ****************************/
/** \addtogroup      Effect */
/** @{ */  /** <!-- [Effect] */


typedef enum hiEFFECTLOG_ERRLEVEL_E
{
    HIEFFECT_LOG_LEVEL_DEBUG = 0,  /**<debug-level                                  */
    HIEFFECT_LOG_LEVEL_INFO,       /**<informational                                */
    HIEFFECT_LOG_LEVEL_NOTICE,     /**<normal but significant condition             */
    HIEFFECT_LOG_LEVEL_WARNING,    /**<warning conditions                           */
    HIEFFECT_LOG_LEVEL_ERROR,      /**<error conditions                             */
    HIEFFECT_LOG_LEVEL_CRIT,       /**<critical conditions                          */
    HIEFFECT_LOG_LEVEL_ALERT,      /**<action must be taken immediately             */
    HIEFFECT_LOG_LEVEL_FATAL,      /**<just for compatibility with previous version */
    HIEFFECT_LOG_LEVEL_BUTT
} HIEFFECT_LOG_ERRLEVEL_E;

/**Definition of the HiEffect error code*/
/** CNcomment:HiEffect 错误码定义宏 */
#define HIEFFECT_DEF_ERR(module, errid) \
    ((HI_S32)((HIEFFECT_ERR_APPID) | ((module) << 16) | ((HIEFFECT_LOG_LEVEL_ERROR) << 13) | (errid)))


enum HIEFFECT_ErrorCode_E
{
    ERR_HIEFFECT_CODE_UNDEF,               
    ERR_HIEFFECT_PTR_NULL,                 /**<pointer is NULL*/
    ERR_HIEFFECT_NO_OPEN,                  /**<effect not open*/
    ERR_HIEFFECT_OPENED,                   /**<effect opened*/
    ERR_HIEFFECT_CLOSED,                   /**<effect closed*/

    /* New */
    ERR_HIEFFECT_INVALID_DEVICE,           /**<invalid device */
    ERR_HIEFFECT_INVALID_HANDLE,           /**<invalid handle*/
    ERR_HIEFFECT_INVALID_PARAMETER,        /**<invalid parameter */
    ERR_HIEFFECT_INVALID_OPERATION,        /**<invalid operation*/
    ERR_HIEFFECT_ADDR_FAULT,               /**<address fault */
    ERR_HIEFFECT_NO_MEM                    /**<mem alloc fail */
};

#define HI_ERR_HIEFFECT_CODE_UNDEF\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_CODE_UNDEF)

#define HI_ERR_HIEFFECT_PTR_NULL\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_PTR_NULL)

#define HI_ERR_HIEFFECT_OPENED\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_OPENED)

#define HI_ERR_HIEFFECT_CLOSED\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_CLOSED)

#define HI_ERR_HIEFFECT_NO_OPEN\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_NO_OPEN)


#define HI_ERR_HIEFFECT_INVALID_DEVICE\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_DEVICE)

#define HI_ERR_HIEFFECT_INVALID_HANDLE\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_HANDLE)

#define HI_ERR_HIEFFECT_INVALID_PARAMETER\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_PARAMETER)

#define HI_ERR_HIEFFECT_INVALID_OPERATION\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_OPERATION)

#define HI_ERR_HIEFFECT_ADDR_FAULT\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_ADDR_FAULT)

#define HI_ERR_HIEFFECT_NO_MEM\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_NO_MEM)

/**Screen refresh callback*/
/** CNcomment:屏幕刷新回调函数*/
typedef HI_S32 (*pRefresh_Callback)(HI_VOID);
typedef enum hiEFFECT_MODE_E
{
    HI_EFFECT_NONE = 0, 			/**<Copy *//**<CNcomment:拷贝显示*/
    HI_EFFECT_TURNPAGE, 			/**<Turnpage *//**<CNcomment:翻页*/
    HI_EFFECT_ROLLPAGE, 			/**<Rollpage *//**<CNcomment:卷轴*/
    HI_EFFECT_VERTICALSHUTTER,		/**<Vertical shutter *//**<CNcomment:垂直百叶窗*/
    HI_EFFECT_HORIZONTALSHUTTER,    /**<Horizontal shutter *//**<CNcomment:水平百叶窗*/
    HI_EFFECT_LEFTIN,				/**<Leftin*//**<CNcomment:从左抽出*/
    HI_EFFECT_TOPIN, 				/**<Topin*//**<CNcomment:从上抽出*/
    HI_EFFECT_TRANSIN, 				/**<Transin*//**<CNcomment:渐进渐出*/
    HI_EFFECT_ROTATE,  				/**<Rotate*//**<CNcomment:螺旋*/
    HI_EFFECT_CENTEROUT,  			/**<Centerout*//**<CNcomment:中央渐出*/
    HI_EFFECT_CENTERIN, 			/**<Centerin*//**<CNcomment:中央渐入*/
} EFFECT_MODE_E;

typedef enum hiEFFECT_SPEED
{
    HI_EFFECT_SPEED_FAST   = 1,
    HI_EFFECT_SPEED_NORMAL = 2,
    HI_EFFECT_SPEED_SLOW = 4,
} EFFECT_SPEED;


/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** \addtogroup      Effect */
/** @{ */  /** <!-- [Effect] */

/** 
\brief Effect init function. CNcomment:特效初始化函数 CNend

\attention \n
CNcomment: 注册屏幕刷新回调函数并设置显示模式 
bLetfbox 为HI_TRUE, 表示图像比例与屏幕比例不一致时添加黑边 
bLetfbox 为HI_FALSE, 表示图像缩放成全屏,如果图形比例与屏幕比例不一致会导致变形. 
无论那种方式,图像都全屏显示 CNend
\param[in] pfCallBack Screen refresh callback function.CNcomment:屏幕刷新回调函数 CNend
\param[in] bLetfbox Picture dispaly mode.CNcomment:图形显示方式 CNend
\retval ::HI_ERR_HIEFFECT_OPENED
\retval ::HI_ERR_HIEFFECT_PTR_NULL
\retval ::HI_SUCCESS

\see \n
::HI_Effect_Init \n
::HI_Effect_Deinit
*/
extern HI_S32  HI_Effect_Init(pRefresh_Callback pfCallBack, HI_BOOL bLetfbox);

/** 
\brief Effect deinit function. CNcomment:特效去初始化函数 CNend
\attention \n
N/A
\param N/A. CNcomment:无 CNend
\retval ::HI_ERR_HIEFFECT_NO_OPEN
\retval ::HI_SUCCESS
\see \n
::HI_GO_Init \n
::HI_Effect_Deinit
*/
extern HI_S32  HI_Effect_Deinit(HI_VOID);

/** 
\brief Effect play function. CNcomment:特效播放函数 CNend

\attention \n
CNcomment: 特效播放函数, 该函数为阻塞函数,特效完成才退出
所有surface的像素格式必须保持一致
目前支持的格式:
    HIGO_PF_4444,
    HIGO_PF_0444,
    HIGO_PF_1555,
    HIGO_PF_0555,
    HIGO_PF_565, CNend
\param[in] hNewSurface new surface
\param[in] hScreenSurface Screen suface
\param[in] mode Effect mode.
\retval ::HI_ERR_HIEFFECT_NO_OPEN
\retval ::HI_ERR_HIEFFECT_INVALID_PARAMETER
\retval ::HI_ERR_HIEFFECT_INVALID_OPERATION
\retval ::HI_ERR_HIEFFECT_NO_MEM
\retval ::HI_SUCCESS
\see \n
N/A. CNcomment:无 CNend
*/
extern HI_S32  HI_Effect_Play(HI_HANDLE hNewSurface, HI_HANDLE hScreenSurface, EFFECT_MODE_E mode);


/** 
\brief Effect Set Layer handle function. CNcomment:设置特效句柄 CNend
\attention \n
N/A
\param[in] Layer Layer handle
\retval ::HI_SUCCESS
\see \n
N/A. CNcomment:无 CNend
*/
extern HI_S32 HI_Effect_SetLayer(HI_HANDLE Layer);

/** 
\brief Effect Stop function. CNcomment:特效播放中止,可以提高按键响应速度 CNend
\attention \n
N/A
\param N/A. CNcomment:无 CNend
\retval ::HI_ERR_HIEFFECT_NO_OPEN
\retval ::HI_SUCCESS
\see \n
N/A. CNcomment:无
*/
extern HI_S32  HI_Effect_Stop(HI_VOID);

/** 
\brief Set Effect Speed function. CNcomment:设置特效播放速度 CNend
\attention \n
N/A
\param[in] eSpeed Effect Speed   
\retval ::HI_ERR_HIEFFECT_NO_OPEN
\retval ::HI_ERR_HIEFFECT_INVALID_PARAMETER
\retval ::HI_SUCCESS
\see \n
N/A. CNcomment:无 CNend
*/
extern HI_S32  HI_Effect_SetSpeed(EFFECT_SPEED eSpeed);

/** @} */  /*! <!-- API declaration end */
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_PEFFECT_H__ */
