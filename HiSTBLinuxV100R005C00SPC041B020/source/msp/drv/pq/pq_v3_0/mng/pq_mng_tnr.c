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

#include "hi_drv_mem.h" /* kmalloc() */

#include "pq_hal_comm.h"
#include "pq_mng_tnr.h"
#include "drv_pq_table.h"

static HI_BOOL g_bTNRInitFlag = HI_FALSE;
static HI_BOOL sg_bTNR_En = HI_FALSE;
static HI_S32 sg_s32TnrStr;
static TNR_DEMO_MODE_E sg_enMode = TNR_DEMO_ENABLE_R;

static HI_U16 TNR_CbCr[16][32] =
{
    //0  1  2   3   4   5   6   7  8  9 10 11 12 13 14 15 16  17 18  19  20  21  22  23 24 25 26 27 28 29 30 31
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 8
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 10, 10, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 9
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 10
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 11
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 12
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 13
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 14
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}  // 15
};



/**
 \brief 初始化TNR模块

 \param[in] none;
 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_InitTnr()
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;
    sg_s32TnrStr = 50;
    sg_bTNR_En = HI_TRUE;

    if (HI_TRUE == g_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_HAL_SetTNRRegist(u32HandleNo, TNR_CbCr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Set tnr register error!\n");
        return HI_FAILURE;
    }

    (HI_VOID)PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_TNR);

    PQ_HAL_SetTnrMidStr(u32HandleNo);
    g_bTNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}



/**
 \brief 去初始化TNR模块

 \param[in] none;
 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_DeInitTnr()
{
    if (HI_FALSE == g_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

    g_bTNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}


/**
 \brief 开关TNR模块
 \attention \n
无

 \param[in] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_EnableTnr(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableTNR(u32HandleNo, bOnOff);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in tnr enable!\n");
                return HI_FAILURE;
            }
        }
    }

    sg_bTNR_En = bOnOff;

    return HI_SUCCESS;
}


/**
 \brief 获取TNR使能状态
 \attention \n
无

 \param[out] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_MNG_GetTnrEnableFlag(HI_BOOL* pbOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    PQ_CHECK_NULL_PTR(pbOnOff);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = HI_SUCCESS;
            break;
        }

    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in get TNR enable!\n");
        return HI_FAILURE;
    }

    *pbOnOff = sg_bTNR_En;

    return HI_SUCCESS;
}


/**
 \brief :  Enable or Disable Tnr Demo ;

 \param[in] bNROnOff

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_EnableTnrDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableTNRDemo(u32HandleNo, bOnOff);
        }

    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/**
 \brief :  set tnr demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_MNG_SetTnrDemoMode(TNR_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTNRDemoMode(u32HandleNo, enMode);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    sg_enMode = enMode;

    return HI_SUCCESS;
}

/**
 \brief :  get tnr demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_GetTnrDemoMode(TNR_DEMO_MODE_E* penMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    PQ_CHECK_NULL_PTR(penMode);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = HI_SUCCESS;
            break;
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in get tnr demo mode!\n");
        return HI_FAILURE;
    }

    *penMode = sg_enMode;

    return HI_SUCCESS;
}

/**
\brief Set Tnr Strengh
\attention \n
无

\param[in] s32TnrStr  Tnr Strengh[0,100]

\retval ::HI_SUCCESS

*/

HI_S32 PQ_MNG_SetTnrStrength(HI_S32 s32TnrStr)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    if ((s32TnrStr < 0) || (s32TnrStr > 100))
    {
        HI_ERR_PQ("the value[%d] of setting NR strength is out of range!\n", s32TnrStr);
        return HI_FAILURE;

    }

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrStr(u32HandleNo, s32TnrStr);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in setting TNR strength!\n");
                return HI_FAILURE;
            }
        }
    }

    sg_s32TnrStr = s32TnrStr;

    return HI_SUCCESS;
}


/**
 \brief 获取NR强度
 \attention \n
无

 \param[in] s32NrStr  NR强度值，取值范围[0,255]

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_MNG_GetTnrStrength(HI_S32* ps32TnrStr)
{
    PQ_CHECK_NULL_PTR(ps32TnrStr);
    *ps32TnrStr = sg_s32TnrStr;

    return HI_SUCCESS;
}

/**
  \brief 设置TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

*/

HI_S32 PQ_MNG_SetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_YMotionPixmeanRatio);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrYMotionPixMeanRatio(u32HandleNo, pstTNR_YMotionPixmeanRatio);

        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Pixmean to Ymotion Ratio!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/**
  \brief 获取TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_YMotionPixmeanRatio);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrYMotionPixMeanRatio(u32HandleNo, pstTNR_YMotionPixmeanRatio);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrYMotionPixMeanRatio(0, pstTNR_YMotionPixmeanRatio);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in getting TNR Pixmean to Ymotion Ratio!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 设置TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_CMotionPixmeanRatio);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrCMotionPixMeanRatio(u32HandleNo, pstTNR_CMotionPixmeanRatio);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Pixmean to Cmotion Ratio!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

/**
  \brief 获取TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_CMotionPixmeanRatio);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrCMotionPixMeanRatio(u32HandleNo, pstTNR_CMotionPixmeanRatio);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrCMotionPixMeanRatio(0, pstTNR_CMotionPixmeanRatio);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in getting TNR Pixmean to Cmotion Ratio!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 设置TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrYMotionMapping(TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_YMotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrYMotionMapping(u32HandleNo, pstTNR_YMotionMapping);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Motion Detecttion YMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

/**
  \brief 获取TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrYMotionMapping(TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_YMotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrYMotionMapping(u32HandleNo, pstTNR_YMotionMapping);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrYMotionMapping(0, pstTNR_YMotionMapping);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in getting TNR Motion Detecttion YMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

/**
  \brief 设置TNR的CMotonMapping 参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrCMotionMapping(TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_CMotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrCMotionMapping(u32HandleNo, pstTNR_CMotionMapping);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Motion Detecttion CMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 获取TNR的CMotonMapping参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrCMotionMapping(TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_CMotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrCMotionMapping(u32HandleNo, pstTNR_CMotionMapping);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrCMotionMapping(0, pstTNR_CMotionMapping);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in Getting TNR Motion Detecttion CMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}






/**
  \brief 设置TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FnlMotionYMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrFnlMotionYMapping(u32HandleNo, pstTNR_FnlMotionYMapping);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Final Y Motion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 获取TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FnlMotionYMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrFnlMotionYMapping(u32HandleNo, pstTNR_FnlMotionYMapping);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrFnlMotionYMapping(0, pstTNR_FnlMotionYMapping);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in getting TNR Final Y Motion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 设置TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_SetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FnlMotionCMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrFnlMotionCMapping(u32HandleNo, pstTNR_FnlMotionCMapping);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in setting TNR Final C Motion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


/**
  \brief 获取TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_MNG_GetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FnlMotionCMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrFnlMotionCMapping(u32HandleNo, pstTNR_FnlMotionCMapping);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrFnlMotionCMapping(0, pstTNR_FnlMotionCMapping);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in Getting TNR Final C Motion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

HI_S32 PQ_MNG_SetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTNR_FmotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FmotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrFmotionMapping(u32HandleNo, pstTNR_FmotionMapping);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in Setting TNR FMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 PQ_MNG_GetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTNR_FmotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstTNR_FmotionMapping);

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetTnrFmotionMapping(u32HandleNo, pstTNR_FmotionMapping);
            break;
        }
        //s32Ret = PQ_HAL_GetTnrFmotionMapping(0, pstTNR_FmotionMapping);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in Getting TNR FMotion Mapping!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


