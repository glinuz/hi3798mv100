/******************************************************************************

   Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
   File Name     : hi_unf_pwm.h
   Version       : Initial Draft
   Author        : Hisilicon multimedia software group
   Created       : 2013/10/28
   Description   :
   History       :
   1.Date        : 2013/10/28
    Author      : h00183514
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 **\brief : define PWM module information
 * \brief supply infor about display.
 */

#ifndef __HI_UNF_PWM_H__
#define __HI_UNF_PWM_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      PWM */
/** @{ */  /** <!-- [PWM] */

/**enum define about PWM*/
/**CNcomment: 定义PWM枚举*/
typedef enum hiUNF_PWM_E
{
    HI_UNF_PWM_0 = 0,   /**< PWM0 */
    HI_UNF_PWM_1,       /**< PWM1 */
    HI_UNF_PWM_2,       /**< PWM2 */
    
    HI_UNF_PWM_BUTT
}HI_UNF_PWM_E;

/**Define the PWM attributes*/
/**CNcomment: 定义PWM属性结构体*/
typedef struct hiUNF_PWM_ATTR_S
{
    HI_U32      u32Freq;        /**< Frequency(HZ)*//**< CNcomment: 频率, 单位HZ*/
    HI_U32      u32DutyRatio;   /**< Duty ratio(calculated to three decimal places)*//**<CNcomment: 占空比，保留小数点后三位*/
}HI_UNF_PWM_ATTR_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      PWM */
/** @{ */  /** <!-- [PWM] */

/**
\brief Initializes the PWM module.CNcomment:初始化PWM模块 CNend
\attention \n
Before calling anyother interface, you must call this application programming interface (API) first.
CNcomment 在调用PWM模块其他接口前，要求首先调用本接口 CNend
\param  N/A
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_DEV_NOT_EXIST    There is no PWM. CNcomment:PWM设备不存在 CNend
\retval ::HI_ERR_PWM_OPEN_ERR         Open PWM failed. CNcomment:PWM打开失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_Init(HI_VOID);

/**
\brief DeInitializes the PWM module.CNcomment:去初始化PWM模块 CNend
\attention N/A
\param  N/A
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_DEV_NOT_EXIST    There is no PWM. CNcomment:PWM设备不存在 CNend
\retval ::HI_ERR_PWM_CLOSE_ERR        Open PWM failed. CNcomment:PWM关闭失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_DeInit(HI_VOID);

/**
\brief Opem PWM.CNcomment:打开PWM CNend
\attention N/A
\param[in] enPWM  PWM device. CNcomment:PWM设备，见::HI_UNF_PWM_E CNend
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_INVALID_PARA     Parameter is invalid. CNcomment:参数无效 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_Open(HI_UNF_PWM_E enPWM);

/**
\brief Close PWM.CNcomment:关闭PWM CNend
\attention N/A
\param[in] enPWM  PWM device. CNcomment:PWM设备，见::HI_UNF_PWM_E CNend
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_INVALID_PARA     Parameter is invalid. CNcomment:参数无效 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_Close(HI_UNF_PWM_E enPWM);

/**
\brief Set PWM attributes.CNcomment:获取PWM属性 CNend
\attention N/A
\param[in] enPWM  PWM device. CNcomment:PWM设备，见::HI_UNF_PWM_E CNend
\param[out] pstAttr  Pointer of PWM attributes. CNcomment:PWM属性指针，见::HI_UNF_PWM_ATTR_S CNend
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_INVALID_PARA     Parameter is invalid. CNcomment:参数无效 CNend
\retval ::HI_ERR_PWM_INVALID_OPT      Opration is invalid. CNcomment:操作无效 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_GetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr);

/**
\brief Set PWM attributes.CNcomment:设置PWM属性 CNend
\attention N/A
\param[in] enPWM  PWM device. CNcomment:PWM设备，见::HI_UNF_PWM_E CNend
\param[in] pstAttr  Pointer of PWM attributes. CNcomment:PWM属性指针，见::HI_UNF_PWM_ATTR_S CNend
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_INVALID_PARA     Parameter is invalid. CNcomment:参数无效 CNend
\retval ::HI_ERR_PWM_INVALID_OPT      Opration is invalid. CNcomment:操作无效 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_SetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr);

/**
\brief Send signal to PWM.CNcomment:向PWM发送信号 CNend
\attention N/A
\param[in] enPWM  PWM device. CNcomment:PWM设备，见::HI_UNF_PWM_E CNend
\param[in] u32CarrierTimeUs  The duration of carrier signal(us). CNcomment:载波信号持续时间(us) CNend
\param[in] u32LowLevelTimeUs  The duration of low-level signal(us). CNcomment:低信号持续时间(us) CNend
\retval ::HI_SUCCESS                  Success CNcomment:成功 CNend
\retval ::HI_ERR_PWM_INVALID_PARA     Parameter is invalid. CNcomment:参数无效 CNend
\retval ::HI_ERR_PWM_INVALID_OPT      Opration is invalid. CNcomment:操作无效 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PWM_SendSignal(HI_UNF_PWM_E enPWM, HI_U32 u32CarrierTimeUs, HI_U32 u32LowLevelTimeUs);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif


