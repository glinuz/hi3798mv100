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

#include "drv_pq_comm.h"
#include "pq_hal_tnr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32 PQ_MNG_SetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTnrYMotionPixmeanRatio);

HI_S32 PQ_MNG_GetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTnrYMotionPixmeanRatio);

HI_S32 PQ_MNG_SetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTnrCMotionPixmeanRatio);

HI_S32 PQ_MNG_GetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTnrCMotionPixmeanRatio);

HI_S32 PQ_MNG_SetTnrYMotionMapping(TNR_MAPPING_S* pstTnrYMotionMapping);

HI_S32 PQ_MNG_GetTnrYMotionMapping(TNR_MAPPING_S* pstTnrYMotionMapping);

HI_S32 PQ_MNG_SetTnrCMotionMapping(TNR_MAPPING_S* pstTnrCMotionMapping);

HI_S32 PQ_MNG_GetTnrCMotionMapping(TNR_MAPPING_S* pstTnrCMotionMapping);

HI_S32 PQ_MNG_SetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTnrFnlMotionYMapping);

HI_S32 PQ_MNG_GetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTnrFnlMotionYMapping);

HI_S32 PQ_MNG_SetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTnrFnlMotionCMapping);

HI_S32 PQ_MNG_GetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTnrFnlMotionCMapping);

HI_S32 PQ_MNG_SetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTnrYfmotionMapping);

HI_S32 PQ_MNG_GetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTnrYfmotionMapping);

HI_S32 PQ_MNG_RegisterTNR(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterTNR(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*End of #ifndef __MNG_PQ_NR_H__ */



