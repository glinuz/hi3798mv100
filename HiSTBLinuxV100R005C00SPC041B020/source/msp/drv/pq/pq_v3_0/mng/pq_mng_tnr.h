/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_tnr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/1/9
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_TNR_H__
#define __PQ_MNG_TNR_H__

#include "pq_hal_tnr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/**
 \brief 初始化TNR模块
 \attention \n
无

 \param[in] none;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_InitTnr(HI_VOID);


/**
 \brief 去初始化TNR模块
 \attention \n
无

 \param[in] none;

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_DeInitTnr(HI_VOID);


/**
 \brief 开关TNR模块
 \attention \n
无

 \param[in] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_EnableTnr(HI_BOOL bOnOff);


/**
 \brief 获取TNR使能状态
 \attention \n
无

 \param[out] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_GetTnrEnableFlag(HI_BOOL* pbOnOff);


/**
 \brief 开关NR模块DEMO模式
 \attention \n
无

 \param[in] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_EnableTnrDemo(HI_BOOL bOnOff);


/**
 \brief :  set tnr demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_SetTnrDemoMode(TNR_DEMO_MODE_E enMode);

/**
 \brief :  get tnr demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_GetTnrDemoMode(TNR_DEMO_MODE_E* penMode);


/**
\brief Set Tnr Strengh
\attention \n
无

\param[in] s32TnrStr  Tnr Strengh[0,100]

\retval ::HI_SUCCESS

*/

HI_S32 PQ_MNG_SetTnrStrength(HI_S32 s32TnrStr);


/**
 \brief 获取NR强度
 \attention \n
无

 \param[in] s32NrStr  NR强度值，取值范围[0,255]

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_MNG_GetTnrStrength(HI_S32* ps32TnrStr);


/**
  \brief 设置TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio);


/**
  \brief 获取TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio);


/**
  \brief 设置TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio);


/**
  \brief 获取TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio);


/**
  \brief 设置TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrYMotionMapping(TNR_MAPPING_S* pstTNR_YMotionMapping);


/**
  \brief 获取TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrYMotionMapping(TNR_MAPPING_S* pstTNR_YMotionMapping);


/**
  \brief 设置TNR的CMotonMapping 参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrCMotionMapping(TNR_MAPPING_S* pstTNR_CMotionMapping);


/**
  \brief 获取TNR的CMotonMapping参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrCMotionMapping(TNR_MAPPING_S* pstTNR_CMotionMapping);


/**
  \brief 设置TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTNR_FnlMotionYMapping);


/**
  \brief 获取TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTNR_FnlMotionYMapping);


/**
  \brief 设置TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTNR_FnlMotionCMapping);



/**
  \brief 获取TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTNR_FnlMotionCMapping);

/**
  \brief 设置 TNR的Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_YfmotionMapping

  \retval ::HI_SUCCESS

  */
HI_S32 PQ_MNG_SetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTNR_YfmotionMapping);

/**
  \brief 获取TNR的Y Motion Mapping  参数
  \attention \n
 无

  \param[in] pstTNR_YfmotionMapping

  \retval ::HI_SUCCESS

  */
HI_S32 PQ_MNG_GetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTNR_YfmotionMapping);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*End of #ifndef __MNG_PQ_NR_H__ */



