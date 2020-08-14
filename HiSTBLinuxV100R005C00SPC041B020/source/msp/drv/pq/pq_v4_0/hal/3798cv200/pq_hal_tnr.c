/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_tnr.c
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/01/09
 Description   :
******************************************************************************/

#include "hi_type.h"
#include "drv_pq_table.h"
#include "pq_hal_tnr.h"
#include "pq_hal_comm.h"
#include "hi_drv_mem.h"

static HI_S32 sg_s32TMTYMotionGain = 32;
static HI_S32 sg_s32TMTCMotionGain = 32;

#define S10TOS32(x) ((x>0x1FF)?(x-0x400):(x))





/**
 \brief :  Enable or Disable TNr Module;

 \param[in] bNROnOff

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_HAL_EnableTNR(HI_U32 u32HandleNo, HI_BOOL bNROnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_MODE.bits.tnr_y_bypass = !bNROnOff;
        pstVpssReg->VPSS_TNR_MODE.bits.tnr_c_bypass = !bNROnOff;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_MODE.bits.tnr_y_bypass = !bNROnOff;
        g_pstIPSelVdpAlg->V0_TNR_MODE.bits.tnr_c_bypass = !bNROnOff;
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_EnableTNRDemo(HI_U32 u32HandleNo, HI_BOOL bTNRDemoEn)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_MODE.bits.market_en = bTNRDemoEn;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_MODE.bits.market_en = bTNRDemoEn;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTNRDemoMode(HI_U32 u32HandleNo, TNR_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_MODE.bits.market_mode = (HI_U32)enMode;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_MODE.bits.market_mode = (HI_U32)enMode;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrMidStr(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        sg_s32TMTYMotionGain = pstVpssReg->VPSS_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k;
        sg_s32TMTCMotionGain = pstVpssReg->VPSS_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k;
    }
    else
    {
        sg_s32TMTYMotionGain = g_pstIPSelVdpAlg->V0_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k;
        sg_s32TMTCMotionGain = g_pstIPSelVdpAlg->V0_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k;
    }

    return HI_SUCCESS;
}


/**
 \brief Set Tnr Strengh
 \attention \n
无

 \param[in] s32TnrStr  Tnr Strengh[0,100]

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_SetTnrStr(HI_U32 u32HandleNo, HI_U32 u32TnrStr)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k = ((100 - u32TnrStr) * sg_s32TMTYMotionGain + 25) / 50;
        pstVpssReg->VPSS_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k = ((100 - u32TnrStr) * sg_s32TMTCMotionGain + 25) / 50;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k = ((100 - u32TnrStr) * sg_s32TMTYMotionGain + 25) / 50;
        g_pstIPSelVdpAlg->V0_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k = ((100 - u32TnrStr) * sg_s32TMTCMotionGain + 25) / 50;
    }

    return HI_SUCCESS;
}


/**
  \brief 设置TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */
HI_S32 PQ_HAL_SetTnrYMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min = pstTNR_YMotionPixmeanRatio->s32MappingMin;
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max = pstTNR_YMotionPixmeanRatio->s32MappingMax;

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0 = pstTNR_YMotionPixmeanRatio->s32MappingR[0];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1 = pstTNR_YMotionPixmeanRatio->s32MappingR[1];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2 = pstTNR_YMotionPixmeanRatio->s32MappingR[2];

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3 = pstTNR_YMotionPixmeanRatio->s32MappingR[3];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4 = pstTNR_YMotionPixmeanRatio->s32MappingR[4];

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1 = pstTNR_YMotionPixmeanRatio->s32MappingT[0];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2 = pstTNR_YMotionPixmeanRatio->s32MappingT[1];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3 = pstTNR_YMotionPixmeanRatio->s32MappingT[2];

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4 = pstTNR_YMotionPixmeanRatio->s32MappingT[3];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5 = pstTNR_YMotionPixmeanRatio->s32MappingT[4];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6 = pstTNR_YMotionPixmeanRatio->s32MappingT[5];

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1 = pstTNR_YMotionPixmeanRatio->s32MappingK[0];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2 = pstTNR_YMotionPixmeanRatio->s32MappingK[1];

        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3 = pstTNR_YMotionPixmeanRatio->s32MappingK[2];
        pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4 = pstTNR_YMotionPixmeanRatio->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min = pstTNR_YMotionPixmeanRatio->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max = pstTNR_YMotionPixmeanRatio->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0 = pstTNR_YMotionPixmeanRatio->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1 = pstTNR_YMotionPixmeanRatio->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2 = pstTNR_YMotionPixmeanRatio->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3 = pstTNR_YMotionPixmeanRatio->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4 = pstTNR_YMotionPixmeanRatio->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1 = pstTNR_YMotionPixmeanRatio->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2 = pstTNR_YMotionPixmeanRatio->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3 = pstTNR_YMotionPixmeanRatio->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4 = pstTNR_YMotionPixmeanRatio->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5 = pstTNR_YMotionPixmeanRatio->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6 = pstTNR_YMotionPixmeanRatio->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1 = pstTNR_YMotionPixmeanRatio->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2 = pstTNR_YMotionPixmeanRatio->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3 = pstTNR_YMotionPixmeanRatio->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4 = pstTNR_YMotionPixmeanRatio->s32MappingK[3];
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
HI_S32 PQ_HAL_GetTnrYMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_YMotionPixmeanRatio->s32MappingMin   = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min;
        pstTNR_YMotionPixmeanRatio->s32MappingMax   = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max;

        pstTNR_YMotionPixmeanRatio->s32MappingR[0]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0;
        pstTNR_YMotionPixmeanRatio->s32MappingR[1]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1;
        pstTNR_YMotionPixmeanRatio->s32MappingR[2]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2;

        pstTNR_YMotionPixmeanRatio->s32MappingR[3]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3;
        pstTNR_YMotionPixmeanRatio->s32MappingR[4]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4;

        pstTNR_YMotionPixmeanRatio->s32MappingT[0]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1;
        pstTNR_YMotionPixmeanRatio->s32MappingT[1]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2;
        pstTNR_YMotionPixmeanRatio->s32MappingT[2]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3;

        pstTNR_YMotionPixmeanRatio->s32MappingT[3]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4;
        pstTNR_YMotionPixmeanRatio->s32MappingT[4]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5;
        pstTNR_YMotionPixmeanRatio->s32MappingT[5]  = pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6;

        pstTNR_YMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1);
        pstTNR_YMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2);

        pstTNR_YMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3);
        pstTNR_YMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4);
    }
    else
    {
        pstTNR_YMotionPixmeanRatio->s32MappingMin   = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min;
        pstTNR_YMotionPixmeanRatio->s32MappingMax   = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max;

        pstTNR_YMotionPixmeanRatio->s32MappingR[0]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0;
        pstTNR_YMotionPixmeanRatio->s32MappingR[1]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1;
        pstTNR_YMotionPixmeanRatio->s32MappingR[2]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2;

        pstTNR_YMotionPixmeanRatio->s32MappingR[3]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3;
        pstTNR_YMotionPixmeanRatio->s32MappingR[4]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4;

        pstTNR_YMotionPixmeanRatio->s32MappingT[0]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1;
        pstTNR_YMotionPixmeanRatio->s32MappingT[1]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2;
        pstTNR_YMotionPixmeanRatio->s32MappingT[2]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3;

        pstTNR_YMotionPixmeanRatio->s32MappingT[3]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4;
        pstTNR_YMotionPixmeanRatio->s32MappingT[4]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5;
        pstTNR_YMotionPixmeanRatio->s32MappingT[5]  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6;

        pstTNR_YMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1);
        pstTNR_YMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2);

        pstTNR_YMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3);
        pstTNR_YMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4);
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
HI_S32 PQ_HAL_SetTnrCMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min = pstTNR_CMotionPixmeanRatio->s32MappingMin;
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max = pstTNR_CMotionPixmeanRatio->s32MappingMax;

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0 = pstTNR_CMotionPixmeanRatio->s32MappingR[0];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1 = pstTNR_CMotionPixmeanRatio->s32MappingR[1];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2 = pstTNR_CMotionPixmeanRatio->s32MappingR[2];

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3 = pstTNR_CMotionPixmeanRatio->s32MappingR[3];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4 = pstTNR_CMotionPixmeanRatio->s32MappingR[4];

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1 = pstTNR_CMotionPixmeanRatio->s32MappingT[0];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2 = pstTNR_CMotionPixmeanRatio->s32MappingT[1];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3 = pstTNR_CMotionPixmeanRatio->s32MappingT[2];

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4 = pstTNR_CMotionPixmeanRatio->s32MappingT[3];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5 = pstTNR_CMotionPixmeanRatio->s32MappingT[4];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6 = pstTNR_CMotionPixmeanRatio->s32MappingT[5];

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1 = pstTNR_CMotionPixmeanRatio->s32MappingK[0];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2 = pstTNR_CMotionPixmeanRatio->s32MappingK[1];

        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3 = pstTNR_CMotionPixmeanRatio->s32MappingK[2];
        pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4 = pstTNR_CMotionPixmeanRatio->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min = pstTNR_CMotionPixmeanRatio->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max = pstTNR_CMotionPixmeanRatio->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0 = pstTNR_CMotionPixmeanRatio->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1 = pstTNR_CMotionPixmeanRatio->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2 = pstTNR_CMotionPixmeanRatio->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3 = pstTNR_CMotionPixmeanRatio->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4 = pstTNR_CMotionPixmeanRatio->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1 = pstTNR_CMotionPixmeanRatio->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2 = pstTNR_CMotionPixmeanRatio->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3 = pstTNR_CMotionPixmeanRatio->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4 = pstTNR_CMotionPixmeanRatio->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5 = pstTNR_CMotionPixmeanRatio->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6 = pstTNR_CMotionPixmeanRatio->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1 = pstTNR_CMotionPixmeanRatio->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2 = pstTNR_CMotionPixmeanRatio->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3 = pstTNR_CMotionPixmeanRatio->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4 = pstTNR_CMotionPixmeanRatio->s32MappingK[3];
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
HI_S32 PQ_HAL_GetTnrCMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_CMotionPixmeanRatio->s32MappingMin = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min;
        pstTNR_CMotionPixmeanRatio->s32MappingMax = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max;

        pstTNR_CMotionPixmeanRatio->s32MappingR[0]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0;
        pstTNR_CMotionPixmeanRatio->s32MappingR[1]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1;
        pstTNR_CMotionPixmeanRatio->s32MappingR[2]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2;

        pstTNR_CMotionPixmeanRatio->s32MappingR[3]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3;
        pstTNR_CMotionPixmeanRatio->s32MappingR[4]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4;

        pstTNR_CMotionPixmeanRatio->s32MappingT[0]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1;
        pstTNR_CMotionPixmeanRatio->s32MappingT[1]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2;
        pstTNR_CMotionPixmeanRatio->s32MappingT[2]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3;

        pstTNR_CMotionPixmeanRatio->s32MappingT[3]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4;
        pstTNR_CMotionPixmeanRatio->s32MappingT[4]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5;
        pstTNR_CMotionPixmeanRatio->s32MappingT[5]  = pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6;

        pstTNR_CMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1);
        pstTNR_CMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2);

        pstTNR_CMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3);
        pstTNR_CMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4);

    }
    else
    {
        pstTNR_CMotionPixmeanRatio->s32MappingMin = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min;
        pstTNR_CMotionPixmeanRatio->s32MappingMax = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max;

        pstTNR_CMotionPixmeanRatio->s32MappingR[0]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0;
        pstTNR_CMotionPixmeanRatio->s32MappingR[1]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1;
        pstTNR_CMotionPixmeanRatio->s32MappingR[2]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2;

        pstTNR_CMotionPixmeanRatio->s32MappingR[3]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3;
        pstTNR_CMotionPixmeanRatio->s32MappingR[4]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4;

        pstTNR_CMotionPixmeanRatio->s32MappingT[0]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1;
        pstTNR_CMotionPixmeanRatio->s32MappingT[1]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2;
        pstTNR_CMotionPixmeanRatio->s32MappingT[2]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3;

        pstTNR_CMotionPixmeanRatio->s32MappingT[3]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4;
        pstTNR_CMotionPixmeanRatio->s32MappingT[4]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5;
        pstTNR_CMotionPixmeanRatio->s32MappingT[5]  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6;

        pstTNR_CMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1);
        pstTNR_CMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2);

        pstTNR_CMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3);
        pstTNR_CMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4);
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
HI_S32 PQ_HAL_SetTnrYMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min = pstTNR_YMotionMapping->s32MappingMin;
        pstVpssReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max = pstTNR_YMotionMapping->s32MappingMax;

        pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0 = pstTNR_YMotionMapping->s32MappingR[0];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1 = pstTNR_YMotionMapping->s32MappingR[1];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2 = pstTNR_YMotionMapping->s32MappingR[2];

        pstVpssReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3 = pstTNR_YMotionMapping->s32MappingR[3];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4 = pstTNR_YMotionMapping->s32MappingR[4];

        pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1 = pstTNR_YMotionMapping->s32MappingT[0];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2 = pstTNR_YMotionMapping->s32MappingT[1];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3 = pstTNR_YMotionMapping->s32MappingT[2];

        pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4 = pstTNR_YMotionMapping->s32MappingT[3];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5 = pstTNR_YMotionMapping->s32MappingT[4];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6 = pstTNR_YMotionMapping->s32MappingT[5];

        pstVpssReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1 = pstTNR_YMotionMapping->s32MappingK[0];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2 = pstTNR_YMotionMapping->s32MappingK[1];

        pstVpssReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3 = pstTNR_YMotionMapping->s32MappingK[2];
        pstVpssReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4 = pstTNR_YMotionMapping->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min = pstTNR_YMotionMapping->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max = pstTNR_YMotionMapping->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0 = pstTNR_YMotionMapping->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1 = pstTNR_YMotionMapping->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2 = pstTNR_YMotionMapping->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3 = pstTNR_YMotionMapping->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4 = pstTNR_YMotionMapping->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1 = pstTNR_YMotionMapping->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2 = pstTNR_YMotionMapping->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3 = pstTNR_YMotionMapping->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4 = pstTNR_YMotionMapping->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5 = pstTNR_YMotionMapping->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6 = pstTNR_YMotionMapping->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1 = pstTNR_YMotionMapping->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2 = pstTNR_YMotionMapping->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3 = pstTNR_YMotionMapping->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4 = pstTNR_YMotionMapping->s32MappingK[3];
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
HI_S32 PQ_HAL_GetTnrYMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_YMotionMapping->s32MappingMin  = pstVpssReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min;
        pstTNR_YMotionMapping->s32MappingMax  = pstVpssReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max;

        pstTNR_YMotionMapping->s32MappingR[0] = pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0;
        pstTNR_YMotionMapping->s32MappingR[1] = pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1;
        pstTNR_YMotionMapping->s32MappingR[2] = pstVpssReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2;

        pstTNR_YMotionMapping->s32MappingR[3] = pstVpssReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3;
        pstTNR_YMotionMapping->s32MappingR[4] = pstVpssReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4;

        pstTNR_YMotionMapping->s32MappingT[0] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1;
        pstTNR_YMotionMapping->s32MappingT[1] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2;
        pstTNR_YMotionMapping->s32MappingT[2] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3;

        pstTNR_YMotionMapping->s32MappingT[3] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4;
        pstTNR_YMotionMapping->s32MappingT[4] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5;
        pstTNR_YMotionMapping->s32MappingT[5] = pstVpssReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6;

        pstTNR_YMotionMapping->s32MappingK[0] = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1);
        pstTNR_YMotionMapping->s32MappingK[1] = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2);

        pstTNR_YMotionMapping->s32MappingK[2] = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3);
        pstTNR_YMotionMapping->s32MappingK[3] = S10TOS32(pstVpssReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4);
    }
    else
    {
        pstTNR_YMotionMapping->s32MappingMin  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min;
        pstTNR_YMotionMapping->s32MappingMax  = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max;

        pstTNR_YMotionMapping->s32MappingR[0] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0;
        pstTNR_YMotionMapping->s32MappingR[1] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1;
        pstTNR_YMotionMapping->s32MappingR[2] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2;

        pstTNR_YMotionMapping->s32MappingR[3] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3;
        pstTNR_YMotionMapping->s32MappingR[4] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4;

        pstTNR_YMotionMapping->s32MappingT[0] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1;
        pstTNR_YMotionMapping->s32MappingT[1] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2;
        pstTNR_YMotionMapping->s32MappingT[2] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3;

        pstTNR_YMotionMapping->s32MappingT[3] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4;
        pstTNR_YMotionMapping->s32MappingT[4] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5;
        pstTNR_YMotionMapping->s32MappingT[5] = g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6;

        pstTNR_YMotionMapping->s32MappingK[0] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1);
        pstTNR_YMotionMapping->s32MappingK[1] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2);

        pstTNR_YMotionMapping->s32MappingK[2] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3);
        pstTNR_YMotionMapping->s32MappingK[3] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4);
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
HI_S32 PQ_HAL_SetTnrCMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min = pstTNR_CMotionMapping->s32MappingMin;
        pstVpssReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max = pstTNR_CMotionMapping->s32MappingMax;

        pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0 = pstTNR_CMotionMapping->s32MappingR[0];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1 = pstTNR_CMotionMapping->s32MappingR[1];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2 = pstTNR_CMotionMapping->s32MappingR[2];

        pstVpssReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3 = pstTNR_CMotionMapping->s32MappingR[3];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4 = pstTNR_CMotionMapping->s32MappingR[4];

        pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1 = pstTNR_CMotionMapping->s32MappingT[0];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2 = pstTNR_CMotionMapping->s32MappingT[1];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3 = pstTNR_CMotionMapping->s32MappingT[2];

        pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4 = pstTNR_CMotionMapping->s32MappingT[3];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5 = pstTNR_CMotionMapping->s32MappingT[4];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6 = pstTNR_CMotionMapping->s32MappingT[5];

        pstVpssReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1 = pstTNR_CMotionMapping->s32MappingK[0];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2 = pstTNR_CMotionMapping->s32MappingK[1];

        pstVpssReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3 = pstTNR_CMotionMapping->s32MappingK[2];
        pstVpssReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4 = pstTNR_CMotionMapping->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min = pstTNR_CMotionMapping->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max = pstTNR_CMotionMapping->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0 = pstTNR_CMotionMapping->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1 = pstTNR_CMotionMapping->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2 = pstTNR_CMotionMapping->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3 = pstTNR_CMotionMapping->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4 = pstTNR_CMotionMapping->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1 = pstTNR_CMotionMapping->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2 = pstTNR_CMotionMapping->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3 = pstTNR_CMotionMapping->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4 = pstTNR_CMotionMapping->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5 = pstTNR_CMotionMapping->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6 = pstTNR_CMotionMapping->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1 = pstTNR_CMotionMapping->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2 = pstTNR_CMotionMapping->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3 = pstTNR_CMotionMapping->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4 = pstTNR_CMotionMapping->s32MappingK[3];
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
HI_S32 PQ_HAL_GetTnrCMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_CMotionMapping->s32MappingMin  = pstVpssReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min;
        pstTNR_CMotionMapping->s32MappingMax  = pstVpssReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max;

        pstTNR_CMotionMapping->s32MappingR[0] = pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0;
        pstTNR_CMotionMapping->s32MappingR[1] = pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1;
        pstTNR_CMotionMapping->s32MappingR[2] = pstVpssReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2;

        pstTNR_CMotionMapping->s32MappingR[3] = pstVpssReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3;
        pstTNR_CMotionMapping->s32MappingR[4] = pstVpssReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4;

        pstTNR_CMotionMapping->s32MappingT[0] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1;
        pstTNR_CMotionMapping->s32MappingT[1] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2;
        pstTNR_CMotionMapping->s32MappingT[2] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3;

        pstTNR_CMotionMapping->s32MappingT[3] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4;
        pstTNR_CMotionMapping->s32MappingT[4] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5;
        pstTNR_CMotionMapping->s32MappingT[5] = pstVpssReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6;

        pstTNR_CMotionMapping->s32MappingK[0] = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1);
        pstTNR_CMotionMapping->s32MappingK[1] = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2);

        pstTNR_CMotionMapping->s32MappingK[2] = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3);
        pstTNR_CMotionMapping->s32MappingK[3] = S10TOS32(pstVpssReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4);
    }
    else
    {
        pstTNR_CMotionMapping->s32MappingMin  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min;
        pstTNR_CMotionMapping->s32MappingMax  = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max;

        pstTNR_CMotionMapping->s32MappingR[0] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0;
        pstTNR_CMotionMapping->s32MappingR[1] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1;
        pstTNR_CMotionMapping->s32MappingR[2] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2;

        pstTNR_CMotionMapping->s32MappingR[3] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3;
        pstTNR_CMotionMapping->s32MappingR[4] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4;

        pstTNR_CMotionMapping->s32MappingT[0] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1;
        pstTNR_CMotionMapping->s32MappingT[1] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2;
        pstTNR_CMotionMapping->s32MappingT[2] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3;

        pstTNR_CMotionMapping->s32MappingT[3] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4;
        pstTNR_CMotionMapping->s32MappingT[4] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5;
        pstTNR_CMotionMapping->s32MappingT[5] = g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6;

        pstTNR_CMotionMapping->s32MappingK[0] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1);
        pstTNR_CMotionMapping->s32MappingK[1] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2);

        pstTNR_CMotionMapping->s32MappingK[2] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3);
        pstTNR_CMotionMapping->s32MappingK[3] = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4);
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
HI_S32 PQ_HAL_SetTnrFnlMotionYMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min = pstTNR_FnlMotionYMapping->s32MappingMin;
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max = pstTNR_FnlMotionYMapping->s32MappingMax;

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0 = pstTNR_FnlMotionYMapping->s32MappingR[0];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1 = pstTNR_FnlMotionYMapping->s32MappingR[1];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2 = pstTNR_FnlMotionYMapping->s32MappingR[2];

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3 = pstTNR_FnlMotionYMapping->s32MappingR[3];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4 = pstTNR_FnlMotionYMapping->s32MappingR[4];

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1 = pstTNR_FnlMotionYMapping->s32MappingT[0];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2 = pstTNR_FnlMotionYMapping->s32MappingT[1];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3 = pstTNR_FnlMotionYMapping->s32MappingT[2];

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4 = pstTNR_FnlMotionYMapping->s32MappingT[3];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5 = pstTNR_FnlMotionYMapping->s32MappingT[4];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6 = pstTNR_FnlMotionYMapping->s32MappingT[5];

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1 = pstTNR_FnlMotionYMapping->s32MappingK[0];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2 = pstTNR_FnlMotionYMapping->s32MappingK[1];

        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3 = pstTNR_FnlMotionYMapping->s32MappingK[2];
        pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4 = pstTNR_FnlMotionYMapping->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min = pstTNR_FnlMotionYMapping->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max = pstTNR_FnlMotionYMapping->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0 = pstTNR_FnlMotionYMapping->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1 = pstTNR_FnlMotionYMapping->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2 = pstTNR_FnlMotionYMapping->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3 = pstTNR_FnlMotionYMapping->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4 = pstTNR_FnlMotionYMapping->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1 = pstTNR_FnlMotionYMapping->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2 = pstTNR_FnlMotionYMapping->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3 = pstTNR_FnlMotionYMapping->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4 = pstTNR_FnlMotionYMapping->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5 = pstTNR_FnlMotionYMapping->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6 = pstTNR_FnlMotionYMapping->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1 = pstTNR_FnlMotionYMapping->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2 = pstTNR_FnlMotionYMapping->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3 = pstTNR_FnlMotionYMapping->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4 = pstTNR_FnlMotionYMapping->s32MappingK[3];
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

HI_S32 PQ_HAL_GetTnrFnlMotionYMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_FnlMotionYMapping->s32MappingMin = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min;
        pstTNR_FnlMotionYMapping->s32MappingMax = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max;

        pstTNR_FnlMotionYMapping->s32MappingR[0]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0;
        pstTNR_FnlMotionYMapping->s32MappingR[1]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1;
        pstTNR_FnlMotionYMapping->s32MappingR[2]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2;

        pstTNR_FnlMotionYMapping->s32MappingR[3]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3;
        pstTNR_FnlMotionYMapping->s32MappingR[4]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4;

        pstTNR_FnlMotionYMapping->s32MappingT[0]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1;
        pstTNR_FnlMotionYMapping->s32MappingT[1]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2;
        pstTNR_FnlMotionYMapping->s32MappingT[2]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3;

        pstTNR_FnlMotionYMapping->s32MappingT[3]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4;
        pstTNR_FnlMotionYMapping->s32MappingT[4]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5;
        pstTNR_FnlMotionYMapping->s32MappingT[5]  = pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6;

        pstTNR_FnlMotionYMapping->s32MappingK[0]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1);
        pstTNR_FnlMotionYMapping->s32MappingK[1]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2);

        pstTNR_FnlMotionYMapping->s32MappingK[2]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3);
        pstTNR_FnlMotionYMapping->s32MappingK[3]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4);
    }
    else
    {
        pstTNR_FnlMotionYMapping->s32MappingMin = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min;
        pstTNR_FnlMotionYMapping->s32MappingMax = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max;

        pstTNR_FnlMotionYMapping->s32MappingR[0]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0;
        pstTNR_FnlMotionYMapping->s32MappingR[1]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1;
        pstTNR_FnlMotionYMapping->s32MappingR[2]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2;

        pstTNR_FnlMotionYMapping->s32MappingR[3]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3;
        pstTNR_FnlMotionYMapping->s32MappingR[4]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4;

        pstTNR_FnlMotionYMapping->s32MappingT[0]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1;
        pstTNR_FnlMotionYMapping->s32MappingT[1]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2;
        pstTNR_FnlMotionYMapping->s32MappingT[2]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3;

        pstTNR_FnlMotionYMapping->s32MappingT[3]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4;
        pstTNR_FnlMotionYMapping->s32MappingT[4]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5;
        pstTNR_FnlMotionYMapping->s32MappingT[5]  = g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6;

        pstTNR_FnlMotionYMapping->s32MappingK[0]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1);
        pstTNR_FnlMotionYMapping->s32MappingK[1]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2);

        pstTNR_FnlMotionYMapping->s32MappingK[2]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3);
        pstTNR_FnlMotionYMapping->s32MappingK[3]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4);
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

HI_S32 PQ_HAL_SetTnrFnlMotionCMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min = pstTNR_FnlMotionCMapping->s32MappingMin;
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max = pstTNR_FnlMotionCMapping->s32MappingMax;

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0 = pstTNR_FnlMotionCMapping->s32MappingR[0];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1 = pstTNR_FnlMotionCMapping->s32MappingR[1];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2 = pstTNR_FnlMotionCMapping->s32MappingR[2];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3 = pstTNR_FnlMotionCMapping->s32MappingR[3];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4 = pstTNR_FnlMotionCMapping->s32MappingR[4];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = pstTNR_FnlMotionCMapping->s32MappingT[0];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = pstTNR_FnlMotionCMapping->s32MappingT[1];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = pstTNR_FnlMotionCMapping->s32MappingT[2];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = pstTNR_FnlMotionCMapping->s32MappingT[3];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = pstTNR_FnlMotionCMapping->s32MappingT[4];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = pstTNR_FnlMotionCMapping->s32MappingT[5];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = pstTNR_FnlMotionCMapping->s32MappingK[0];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = pstTNR_FnlMotionCMapping->s32MappingK[1];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = pstTNR_FnlMotionCMapping->s32MappingK[2];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = pstTNR_FnlMotionCMapping->s32MappingK[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min = pstTNR_FnlMotionCMapping->s32MappingMin;
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max = pstTNR_FnlMotionCMapping->s32MappingMax;

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0 = pstTNR_FnlMotionCMapping->s32MappingR[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1 = pstTNR_FnlMotionCMapping->s32MappingR[1];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2 = pstTNR_FnlMotionCMapping->s32MappingR[2];

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3 = pstTNR_FnlMotionCMapping->s32MappingR[3];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4 = pstTNR_FnlMotionCMapping->s32MappingR[4];

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = pstTNR_FnlMotionCMapping->s32MappingT[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = pstTNR_FnlMotionCMapping->s32MappingT[1];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = pstTNR_FnlMotionCMapping->s32MappingT[2];

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = pstTNR_FnlMotionCMapping->s32MappingT[3];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = pstTNR_FnlMotionCMapping->s32MappingT[4];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = pstTNR_FnlMotionCMapping->s32MappingT[5];

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = pstTNR_FnlMotionCMapping->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = pstTNR_FnlMotionCMapping->s32MappingK[1];

        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = pstTNR_FnlMotionCMapping->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = pstTNR_FnlMotionCMapping->s32MappingK[3];
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

HI_S32 PQ_HAL_GetTnrFnlMotionCMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_FnlMotionCMapping->s32MappingMin = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min;
        pstTNR_FnlMotionCMapping->s32MappingMax = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max;

        pstTNR_FnlMotionCMapping->s32MappingR[0]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0;
        pstTNR_FnlMotionCMapping->s32MappingR[1]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1;
        pstTNR_FnlMotionCMapping->s32MappingR[2]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2;

        pstTNR_FnlMotionCMapping->s32MappingR[3]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3;
        pstTNR_FnlMotionCMapping->s32MappingR[4]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4;

        pstTNR_FnlMotionCMapping->s32MappingT[0]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1;
        pstTNR_FnlMotionCMapping->s32MappingT[1]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2;
        pstTNR_FnlMotionCMapping->s32MappingT[2]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3;

        pstTNR_FnlMotionCMapping->s32MappingT[3]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4;
        pstTNR_FnlMotionCMapping->s32MappingT[4]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5;
        pstTNR_FnlMotionCMapping->s32MappingT[5]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6;

        pstTNR_FnlMotionCMapping->s32MappingK[0]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1);
        pstTNR_FnlMotionCMapping->s32MappingK[1]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2);

        pstTNR_FnlMotionCMapping->s32MappingK[2]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3);
        pstTNR_FnlMotionCMapping->s32MappingK[3]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4);
    }
    else
    {
        pstTNR_FnlMotionCMapping->s32MappingMin = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min;
        pstTNR_FnlMotionCMapping->s32MappingMax = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max;

        pstTNR_FnlMotionCMapping->s32MappingR[0]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0;
        pstTNR_FnlMotionCMapping->s32MappingR[1]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1;
        pstTNR_FnlMotionCMapping->s32MappingR[2]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2;

        pstTNR_FnlMotionCMapping->s32MappingR[3]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3;
        pstTNR_FnlMotionCMapping->s32MappingR[4]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4;

        pstTNR_FnlMotionCMapping->s32MappingT[0]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1;
        pstTNR_FnlMotionCMapping->s32MappingT[1]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2;
        pstTNR_FnlMotionCMapping->s32MappingT[2]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3;

        pstTNR_FnlMotionCMapping->s32MappingT[3]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4;
        pstTNR_FnlMotionCMapping->s32MappingT[4]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5;
        pstTNR_FnlMotionCMapping->s32MappingT[5]  = g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6;

        pstTNR_FnlMotionCMapping->s32MappingK[0]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1);
        pstTNR_FnlMotionCMapping->s32MappingK[1]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2);

        pstTNR_FnlMotionCMapping->s32MappingK[2]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3);
        pstTNR_FnlMotionCMapping->s32MappingK[3]  = S10TOS32(g_pstIPSelVdpAlg->V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrFmotionMapping(HI_U32 u32HandleNo, TNR_FMOTION_MAPPING_S* pstFmotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_CLUT50.bits.data0 = pstFmotionMapping->s16YFmotion[0];
        pstVpssReg->VPSS_TNR_CLUT50.bits.data1 = pstFmotionMapping->s16YFmotion[1];
        pstVpssReg->VPSS_TNR_CLUT50.bits.data2 = pstFmotionMapping->s16YFmotion[2];
        pstVpssReg->VPSS_TNR_CLUT50.bits.data3 = pstFmotionMapping->s16YFmotion[3];

        pstVpssReg->VPSS_TNR_CLUT51.bits.data0 = pstFmotionMapping->s16YFmotion[4];
        pstVpssReg->VPSS_TNR_CLUT51.bits.data1 = pstFmotionMapping->s16YFmotion[5];
        pstVpssReg->VPSS_TNR_CLUT51.bits.data2 = pstFmotionMapping->s16YFmotion[6];
        pstVpssReg->VPSS_TNR_CLUT51.bits.data3 = pstFmotionMapping->s16YFmotion[7];

        pstVpssReg->VPSS_TNR_CLUT52.bits.data0 = pstFmotionMapping->s16YFmotion[8];
        pstVpssReg->VPSS_TNR_CLUT52.bits.data1 = pstFmotionMapping->s16YFmotion[9];
        pstVpssReg->VPSS_TNR_CLUT52.bits.data2 = pstFmotionMapping->s16YFmotion[10];
        pstVpssReg->VPSS_TNR_CLUT52.bits.data3 = pstFmotionMapping->s16YFmotion[11];

        pstVpssReg->VPSS_TNR_CLUT53.bits.data0 = pstFmotionMapping->s16YFmotion[12];
        pstVpssReg->VPSS_TNR_CLUT53.bits.data1 = pstFmotionMapping->s16YFmotion[13];
        pstVpssReg->VPSS_TNR_CLUT53.bits.data2 = pstFmotionMapping->s16YFmotion[14];
        pstVpssReg->VPSS_TNR_CLUT53.bits.data3 = pstFmotionMapping->s16YFmotion[15];

        pstVpssReg->VPSS_TNR_CLUT54.bits.data0 = pstFmotionMapping->s16YFmotion[16];
        pstVpssReg->VPSS_TNR_CLUT54.bits.data1 = pstFmotionMapping->s16YFmotion[17];
        pstVpssReg->VPSS_TNR_CLUT54.bits.data2 = pstFmotionMapping->s16YFmotion[18];
        pstVpssReg->VPSS_TNR_CLUT54.bits.data3 = pstFmotionMapping->s16YFmotion[19];

        pstVpssReg->VPSS_TNR_CLUT55.bits.data0 = pstFmotionMapping->s16YFmotion[20];
        pstVpssReg->VPSS_TNR_CLUT55.bits.data1 = pstFmotionMapping->s16YFmotion[21];
        pstVpssReg->VPSS_TNR_CLUT55.bits.data2 = pstFmotionMapping->s16YFmotion[22];
        pstVpssReg->VPSS_TNR_CLUT55.bits.data3 = pstFmotionMapping->s16YFmotion[23];

        pstVpssReg->VPSS_TNR_CLUT56.bits.data0 = pstFmotionMapping->s16YFmotion[24];
        pstVpssReg->VPSS_TNR_CLUT56.bits.data1 = pstFmotionMapping->s16YFmotion[25];
        pstVpssReg->VPSS_TNR_CLUT56.bits.data2 = pstFmotionMapping->s16YFmotion[26];
        pstVpssReg->VPSS_TNR_CLUT56.bits.data3 = pstFmotionMapping->s16YFmotion[27];

        pstVpssReg->VPSS_TNR_CLUT57.bits.data0 = pstFmotionMapping->s16YFmotion[28];
        pstVpssReg->VPSS_TNR_CLUT57.bits.data1 = pstFmotionMapping->s16YFmotion[29];
        pstVpssReg->VPSS_TNR_CLUT57.bits.data2 = pstFmotionMapping->s16YFmotion[30];
        pstVpssReg->VPSS_TNR_CLUT57.bits.data3 = pstFmotionMapping->s16YFmotion[31];
        //COLOR
        pstVpssReg->VPSS_TNR_CLUT60.bits.data0 = pstFmotionMapping->s16CFmotion[0];
        pstVpssReg->VPSS_TNR_CLUT60.bits.data1 = pstFmotionMapping->s16CFmotion[1];
        pstVpssReg->VPSS_TNR_CLUT60.bits.data2 = pstFmotionMapping->s16CFmotion[2];
        pstVpssReg->VPSS_TNR_CLUT60.bits.data3 = pstFmotionMapping->s16CFmotion[3];

        pstVpssReg->VPSS_TNR_CLUT61.bits.data0 = pstFmotionMapping->s16CFmotion[4];
        pstVpssReg->VPSS_TNR_CLUT61.bits.data1 = pstFmotionMapping->s16CFmotion[5];
        pstVpssReg->VPSS_TNR_CLUT61.bits.data2 = pstFmotionMapping->s16CFmotion[6];
        pstVpssReg->VPSS_TNR_CLUT61.bits.data3 = pstFmotionMapping->s16CFmotion[7];

        pstVpssReg->VPSS_TNR_CLUT62.bits.data0 = pstFmotionMapping->s16CFmotion[8];
        pstVpssReg->VPSS_TNR_CLUT62.bits.data1 = pstFmotionMapping->s16CFmotion[9];
        pstVpssReg->VPSS_TNR_CLUT62.bits.data2 = pstFmotionMapping->s16CFmotion[10];
        pstVpssReg->VPSS_TNR_CLUT62.bits.data3 = pstFmotionMapping->s16CFmotion[11];

        pstVpssReg->VPSS_TNR_CLUT63.bits.data0 = pstFmotionMapping->s16CFmotion[12];
        pstVpssReg->VPSS_TNR_CLUT63.bits.data1 = pstFmotionMapping->s16CFmotion[13];
        pstVpssReg->VPSS_TNR_CLUT63.bits.data2 = pstFmotionMapping->s16CFmotion[14];
        pstVpssReg->VPSS_TNR_CLUT63.bits.data3 = pstFmotionMapping->s16CFmotion[15];

        pstVpssReg->VPSS_TNR_CLUT64.bits.data0 = pstFmotionMapping->s16CFmotion[16];
        pstVpssReg->VPSS_TNR_CLUT64.bits.data1 = pstFmotionMapping->s16CFmotion[17];
        pstVpssReg->VPSS_TNR_CLUT64.bits.data2 = pstFmotionMapping->s16CFmotion[18];
        pstVpssReg->VPSS_TNR_CLUT64.bits.data3 = pstFmotionMapping->s16CFmotion[19];

        pstVpssReg->VPSS_TNR_CLUT65.bits.data0 = pstFmotionMapping->s16CFmotion[20];
        pstVpssReg->VPSS_TNR_CLUT65.bits.data1 = pstFmotionMapping->s16CFmotion[21];
        pstVpssReg->VPSS_TNR_CLUT65.bits.data2 = pstFmotionMapping->s16CFmotion[22];
        pstVpssReg->VPSS_TNR_CLUT65.bits.data3 = pstFmotionMapping->s16CFmotion[23];

        pstVpssReg->VPSS_TNR_CLUT66.bits.data0 = pstFmotionMapping->s16CFmotion[24];
        pstVpssReg->VPSS_TNR_CLUT66.bits.data1 = pstFmotionMapping->s16CFmotion[25];
        pstVpssReg->VPSS_TNR_CLUT66.bits.data2 = pstFmotionMapping->s16CFmotion[26];
        pstVpssReg->VPSS_TNR_CLUT66.bits.data3 = pstFmotionMapping->s16CFmotion[27];

        pstVpssReg->VPSS_TNR_CLUT67.bits.data0 = pstFmotionMapping->s16CFmotion[28];
        pstVpssReg->VPSS_TNR_CLUT67.bits.data1 = pstFmotionMapping->s16CFmotion[29];
        pstVpssReg->VPSS_TNR_CLUT67.bits.data2 = pstFmotionMapping->s16CFmotion[30];
        pstVpssReg->VPSS_TNR_CLUT67.bits.data3 = pstFmotionMapping->s16CFmotion[31];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data0 = pstFmotionMapping->s16YFmotion[0];
        g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data1 = pstFmotionMapping->s16YFmotion[1];
        g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data2 = pstFmotionMapping->s16YFmotion[2];
        g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data3 = pstFmotionMapping->s16YFmotion[3];

        g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data0 = pstFmotionMapping->s16YFmotion[4];
        g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data1 = pstFmotionMapping->s16YFmotion[5];
        g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data2 = pstFmotionMapping->s16YFmotion[6];
        g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data3 = pstFmotionMapping->s16YFmotion[7];

        g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data0 = pstFmotionMapping->s16YFmotion[8];
        g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data1 = pstFmotionMapping->s16YFmotion[9];
        g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data2 = pstFmotionMapping->s16YFmotion[10];
        g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data3 = pstFmotionMapping->s16YFmotion[11];

        g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data0 = pstFmotionMapping->s16YFmotion[12];
        g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data1 = pstFmotionMapping->s16YFmotion[13];
        g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data2 = pstFmotionMapping->s16YFmotion[14];
        g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data3 = pstFmotionMapping->s16YFmotion[15];

        g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data0 = pstFmotionMapping->s16YFmotion[16];
        g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data1 = pstFmotionMapping->s16YFmotion[17];
        g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data2 = pstFmotionMapping->s16YFmotion[18];
        g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data3 = pstFmotionMapping->s16YFmotion[19];

        g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data0 = pstFmotionMapping->s16YFmotion[20];
        g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data1 = pstFmotionMapping->s16YFmotion[21];
        g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data2 = pstFmotionMapping->s16YFmotion[22];
        g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data3 = pstFmotionMapping->s16YFmotion[23];

        g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data0 = pstFmotionMapping->s16YFmotion[24];
        g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data1 = pstFmotionMapping->s16YFmotion[25];
        g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data2 = pstFmotionMapping->s16YFmotion[26];
        g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data3 = pstFmotionMapping->s16YFmotion[27];

        g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data0 = pstFmotionMapping->s16YFmotion[28];
        g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data1 = pstFmotionMapping->s16YFmotion[29];
        g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data2 = pstFmotionMapping->s16YFmotion[30];
        g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data3 = pstFmotionMapping->s16YFmotion[31];
        //COLOR
        g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data0 = pstFmotionMapping->s16CFmotion[0];
        g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data1 = pstFmotionMapping->s16CFmotion[1];
        g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data2 = pstFmotionMapping->s16CFmotion[2];
        g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data3 = pstFmotionMapping->s16CFmotion[3];

        g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data0 = pstFmotionMapping->s16CFmotion[4];
        g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data1 = pstFmotionMapping->s16CFmotion[5];
        g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data2 = pstFmotionMapping->s16CFmotion[6];
        g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data3 = pstFmotionMapping->s16CFmotion[7];

        g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data0 = pstFmotionMapping->s16CFmotion[8];
        g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data1 = pstFmotionMapping->s16CFmotion[9];
        g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data2 = pstFmotionMapping->s16CFmotion[10];
        g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data3 = pstFmotionMapping->s16CFmotion[11];

        g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data0 = pstFmotionMapping->s16CFmotion[12];
        g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data1 = pstFmotionMapping->s16CFmotion[13];
        g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data2 = pstFmotionMapping->s16CFmotion[14];
        g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data3 = pstFmotionMapping->s16CFmotion[15];

        g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data0 = pstFmotionMapping->s16CFmotion[16];
        g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data1 = pstFmotionMapping->s16CFmotion[17];
        g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data2 = pstFmotionMapping->s16CFmotion[18];
        g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data3 = pstFmotionMapping->s16CFmotion[19];

        g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data0 = pstFmotionMapping->s16CFmotion[20];
        g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data1 = pstFmotionMapping->s16CFmotion[21];
        g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data2 = pstFmotionMapping->s16CFmotion[22];
        g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data3 = pstFmotionMapping->s16CFmotion[23];

        g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data0 = pstFmotionMapping->s16CFmotion[24];
        g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data1 = pstFmotionMapping->s16CFmotion[25];
        g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data2 = pstFmotionMapping->s16CFmotion[26];
        g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data3 = pstFmotionMapping->s16CFmotion[27];

        g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data0 = pstFmotionMapping->s16CFmotion[28];
        g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data1 = pstFmotionMapping->s16CFmotion[29];
        g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data2 = pstFmotionMapping->s16CFmotion[30];
        g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data3 = pstFmotionMapping->s16CFmotion[31];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetTnrFmotionMapping(HI_U32 u32HandleNo, TNR_FMOTION_MAPPING_S* pstFmotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstFmotionMapping->s16YFmotion[0] = pstVpssReg->VPSS_TNR_CLUT50.bits.data0;
        pstFmotionMapping->s16YFmotion[1] = pstVpssReg->VPSS_TNR_CLUT50.bits.data1;
        pstFmotionMapping->s16YFmotion[2] = pstVpssReg->VPSS_TNR_CLUT50.bits.data2;
        pstFmotionMapping->s16YFmotion[3] = pstVpssReg->VPSS_TNR_CLUT50.bits.data3;

        pstFmotionMapping->s16YFmotion[4] = pstVpssReg->VPSS_TNR_CLUT51.bits.data0;
        pstFmotionMapping->s16YFmotion[5] = pstVpssReg->VPSS_TNR_CLUT51.bits.data1;
        pstFmotionMapping->s16YFmotion[6] = pstVpssReg->VPSS_TNR_CLUT51.bits.data2;
        pstFmotionMapping->s16YFmotion[7] = pstVpssReg->VPSS_TNR_CLUT51.bits.data3;

        pstFmotionMapping->s16YFmotion[8] = pstVpssReg->VPSS_TNR_CLUT52.bits.data0;
        pstFmotionMapping->s16YFmotion[9] = pstVpssReg->VPSS_TNR_CLUT52.bits.data1;
        pstFmotionMapping->s16YFmotion[10] = pstVpssReg->VPSS_TNR_CLUT52.bits.data2;
        pstFmotionMapping->s16YFmotion[11] = pstVpssReg->VPSS_TNR_CLUT52.bits.data3;

        pstFmotionMapping->s16YFmotion[12] = pstVpssReg->VPSS_TNR_CLUT53.bits.data0;
        pstFmotionMapping->s16YFmotion[13] = pstVpssReg->VPSS_TNR_CLUT53.bits.data1;
        pstFmotionMapping->s16YFmotion[14] = pstVpssReg->VPSS_TNR_CLUT53.bits.data2;
        pstFmotionMapping->s16YFmotion[15] = pstVpssReg->VPSS_TNR_CLUT53.bits.data3;

        pstFmotionMapping->s16YFmotion[16] = pstVpssReg->VPSS_TNR_CLUT54.bits.data0;
        pstFmotionMapping->s16YFmotion[17] = pstVpssReg->VPSS_TNR_CLUT54.bits.data1;
        pstFmotionMapping->s16YFmotion[18] = pstVpssReg->VPSS_TNR_CLUT54.bits.data2;
        pstFmotionMapping->s16YFmotion[19] = pstVpssReg->VPSS_TNR_CLUT54.bits.data3;

        pstFmotionMapping->s16YFmotion[20] = pstVpssReg->VPSS_TNR_CLUT55.bits.data0;
        pstFmotionMapping->s16YFmotion[21] = pstVpssReg->VPSS_TNR_CLUT55.bits.data1;
        pstFmotionMapping->s16YFmotion[22] = pstVpssReg->VPSS_TNR_CLUT55.bits.data2;
        pstFmotionMapping->s16YFmotion[23] = pstVpssReg->VPSS_TNR_CLUT55.bits.data3;

        pstFmotionMapping->s16YFmotion[24] = pstVpssReg->VPSS_TNR_CLUT56.bits.data0;
        pstFmotionMapping->s16YFmotion[25] = pstVpssReg->VPSS_TNR_CLUT56.bits.data1;
        pstFmotionMapping->s16YFmotion[26] = pstVpssReg->VPSS_TNR_CLUT56.bits.data2;
        pstFmotionMapping->s16YFmotion[27] = pstVpssReg->VPSS_TNR_CLUT56.bits.data3;

        pstFmotionMapping->s16YFmotion[28] = pstVpssReg->VPSS_TNR_CLUT57.bits.data0;
        pstFmotionMapping->s16YFmotion[29] = pstVpssReg->VPSS_TNR_CLUT57.bits.data1;
        pstFmotionMapping->s16YFmotion[30] = pstVpssReg->VPSS_TNR_CLUT57.bits.data2;
        pstFmotionMapping->s16YFmotion[31] = pstVpssReg->VPSS_TNR_CLUT57.bits.data3;
        //COLOR
        pstFmotionMapping->s16CFmotion[0] = pstVpssReg->VPSS_TNR_CLUT60.bits.data0;
        pstFmotionMapping->s16CFmotion[1] = pstVpssReg->VPSS_TNR_CLUT60.bits.data1;
        pstFmotionMapping->s16CFmotion[2] = pstVpssReg->VPSS_TNR_CLUT60.bits.data2;
        pstFmotionMapping->s16CFmotion[3] = pstVpssReg->VPSS_TNR_CLUT60.bits.data3;

        pstFmotionMapping->s16CFmotion[4] = pstVpssReg->VPSS_TNR_CLUT61.bits.data0;
        pstFmotionMapping->s16CFmotion[5] = pstVpssReg->VPSS_TNR_CLUT61.bits.data1;
        pstFmotionMapping->s16CFmotion[6] = pstVpssReg->VPSS_TNR_CLUT61.bits.data2;
        pstFmotionMapping->s16CFmotion[7] = pstVpssReg->VPSS_TNR_CLUT61.bits.data3;

        pstFmotionMapping->s16CFmotion[8] = pstVpssReg->VPSS_TNR_CLUT62.bits.data0;
        pstFmotionMapping->s16CFmotion[9] = pstVpssReg->VPSS_TNR_CLUT62.bits.data1;
        pstFmotionMapping->s16CFmotion[10] = pstVpssReg->VPSS_TNR_CLUT62.bits.data2;
        pstFmotionMapping->s16CFmotion[11] = pstVpssReg->VPSS_TNR_CLUT62.bits.data3;

        pstFmotionMapping->s16CFmotion[12] = pstVpssReg->VPSS_TNR_CLUT63.bits.data0;
        pstFmotionMapping->s16CFmotion[13] = pstVpssReg->VPSS_TNR_CLUT63.bits.data1;
        pstFmotionMapping->s16CFmotion[14] = pstVpssReg->VPSS_TNR_CLUT63.bits.data2;
        pstFmotionMapping->s16CFmotion[15] = pstVpssReg->VPSS_TNR_CLUT63.bits.data3;

        pstFmotionMapping->s16CFmotion[16] = pstVpssReg->VPSS_TNR_CLUT64.bits.data0;
        pstFmotionMapping->s16CFmotion[17] = pstVpssReg->VPSS_TNR_CLUT64.bits.data1;
        pstFmotionMapping->s16CFmotion[18] = pstVpssReg->VPSS_TNR_CLUT64.bits.data2;
        pstFmotionMapping->s16CFmotion[19] = pstVpssReg->VPSS_TNR_CLUT64.bits.data3;

        pstFmotionMapping->s16CFmotion[20] = pstVpssReg->VPSS_TNR_CLUT65.bits.data0;
        pstFmotionMapping->s16CFmotion[21] = pstVpssReg->VPSS_TNR_CLUT65.bits.data1;
        pstFmotionMapping->s16CFmotion[22] = pstVpssReg->VPSS_TNR_CLUT65.bits.data2;
        pstFmotionMapping->s16CFmotion[23] = pstVpssReg->VPSS_TNR_CLUT65.bits.data3;

        pstFmotionMapping->s16CFmotion[24] = pstVpssReg->VPSS_TNR_CLUT66.bits.data0;
        pstFmotionMapping->s16CFmotion[25] = pstVpssReg->VPSS_TNR_CLUT66.bits.data1;
        pstFmotionMapping->s16CFmotion[26] = pstVpssReg->VPSS_TNR_CLUT66.bits.data2;
        pstFmotionMapping->s16CFmotion[27] = pstVpssReg->VPSS_TNR_CLUT66.bits.data3;

        pstFmotionMapping->s16CFmotion[28] = pstVpssReg->VPSS_TNR_CLUT67.bits.data0;
        pstFmotionMapping->s16CFmotion[29] = pstVpssReg->VPSS_TNR_CLUT67.bits.data1;
        pstFmotionMapping->s16CFmotion[30] = pstVpssReg->VPSS_TNR_CLUT67.bits.data2;
        pstFmotionMapping->s16CFmotion[31] = pstVpssReg->VPSS_TNR_CLUT67.bits.data3;
    }
    else
    {
        pstFmotionMapping->s16YFmotion[0] = g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data0;
        pstFmotionMapping->s16YFmotion[1] = g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data1;
        pstFmotionMapping->s16YFmotion[2] = g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data2;
        pstFmotionMapping->s16YFmotion[3] = g_pstIPSelVdpAlg->V0_TNR_CLUT50.bits.data3;
        pstFmotionMapping->s16YFmotion[4] = g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data0;
        pstFmotionMapping->s16YFmotion[5] = g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data1;
        pstFmotionMapping->s16YFmotion[6] = g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data2;
        pstFmotionMapping->s16YFmotion[7] = g_pstIPSelVdpAlg->V0_TNR_CLUT51.bits.data3;
        pstFmotionMapping->s16YFmotion[8] = g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data0;
        pstFmotionMapping->s16YFmotion[9] = g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data1;
        pstFmotionMapping->s16YFmotion[10] = g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data2;
        pstFmotionMapping->s16YFmotion[11] = g_pstIPSelVdpAlg->V0_TNR_CLUT52.bits.data3;
        pstFmotionMapping->s16YFmotion[12] = g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data0;
        pstFmotionMapping->s16YFmotion[13] = g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data1;
        pstFmotionMapping->s16YFmotion[14] = g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data2;
        pstFmotionMapping->s16YFmotion[15] = g_pstIPSelVdpAlg->V0_TNR_CLUT53.bits.data3;
        pstFmotionMapping->s16YFmotion[16] = g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data0;
        pstFmotionMapping->s16YFmotion[17] = g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data1;
        pstFmotionMapping->s16YFmotion[18] = g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data2;
        pstFmotionMapping->s16YFmotion[19] = g_pstIPSelVdpAlg->V0_TNR_CLUT54.bits.data3;
        pstFmotionMapping->s16YFmotion[20] = g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data0;
        pstFmotionMapping->s16YFmotion[21] = g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data1;
        pstFmotionMapping->s16YFmotion[22] = g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data2;
        pstFmotionMapping->s16YFmotion[23] = g_pstIPSelVdpAlg->V0_TNR_CLUT55.bits.data3;
        pstFmotionMapping->s16YFmotion[24] = g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data0;
        pstFmotionMapping->s16YFmotion[25] = g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data1;
        pstFmotionMapping->s16YFmotion[26] = g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data2;
        pstFmotionMapping->s16YFmotion[27] = g_pstIPSelVdpAlg->V0_TNR_CLUT56.bits.data3;
        pstFmotionMapping->s16YFmotion[28] = g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data0;
        pstFmotionMapping->s16YFmotion[29] = g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data1;
        pstFmotionMapping->s16YFmotion[30] = g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data2;
        pstFmotionMapping->s16YFmotion[31] = g_pstIPSelVdpAlg->V0_TNR_CLUT57.bits.data3;

        //COLOR
        pstFmotionMapping->s16CFmotion[0] = g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data0;
        pstFmotionMapping->s16CFmotion[1] = g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data1;
        pstFmotionMapping->s16CFmotion[2] = g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data2;
        pstFmotionMapping->s16CFmotion[3] = g_pstIPSelVdpAlg->V0_TNR_CLUT60.bits.data3;
        pstFmotionMapping->s16CFmotion[4] = g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data0;
        pstFmotionMapping->s16CFmotion[5] = g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data1;
        pstFmotionMapping->s16CFmotion[6] = g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data2;
        pstFmotionMapping->s16CFmotion[7] = g_pstIPSelVdpAlg->V0_TNR_CLUT61.bits.data3;
        pstFmotionMapping->s16CFmotion[8] = g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data0;
        pstFmotionMapping->s16CFmotion[9] = g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data1;
        pstFmotionMapping->s16CFmotion[10] = g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data2;
        pstFmotionMapping->s16CFmotion[11] = g_pstIPSelVdpAlg->V0_TNR_CLUT62.bits.data3;
        pstFmotionMapping->s16CFmotion[12] = g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data0;
        pstFmotionMapping->s16CFmotion[13] = g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data1;
        pstFmotionMapping->s16CFmotion[14] = g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data2;
        pstFmotionMapping->s16CFmotion[15] = g_pstIPSelVdpAlg->V0_TNR_CLUT63.bits.data3;
        pstFmotionMapping->s16CFmotion[16] = g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data0;
        pstFmotionMapping->s16CFmotion[17] = g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data1;
        pstFmotionMapping->s16CFmotion[18] = g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data2;
        pstFmotionMapping->s16CFmotion[19] = g_pstIPSelVdpAlg->V0_TNR_CLUT64.bits.data3;
        pstFmotionMapping->s16CFmotion[20] = g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data0;
        pstFmotionMapping->s16CFmotion[21] = g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data1;
        pstFmotionMapping->s16CFmotion[22] = g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data2;
        pstFmotionMapping->s16CFmotion[23] = g_pstIPSelVdpAlg->V0_TNR_CLUT65.bits.data3;
        pstFmotionMapping->s16CFmotion[24] = g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data0;
        pstFmotionMapping->s16CFmotion[25] = g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data1;
        pstFmotionMapping->s16CFmotion[26] = g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data2;
        pstFmotionMapping->s16CFmotion[27] = g_pstIPSelVdpAlg->V0_TNR_CLUT66.bits.data3;
        pstFmotionMapping->s16CFmotion[28] = g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data0;
        pstFmotionMapping->s16CFmotion[29] = g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data1;
        pstFmotionMapping->s16CFmotion[30] = g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data2;
        pstFmotionMapping->s16CFmotion[31] = g_pstIPSelVdpAlg->V0_TNR_CLUT67.bits.data3;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_TNR_MARKET.bits.market_coordinate = u32XPos;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_TNR_MARKET.bits.market_coordinate = u32XPos;
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetTNRCbCrLut(HI_U32 u32HandleNo, HI_U8 TLUT_CbCr[][32])
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;
    U_V0_TNR_CLUT20  V0_TNR_CLUT20;
    U_VPSS_TNR_CLUT20  VPSS_TNR_CLUT20;
    HI_U32 i, j;
    HI_U32* pu32Lut = HI_NULL;


    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pu32Lut = (HI_U32*) & (pstVpssReg->VPSS_TNR_CLUT20.u32);

        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < 32;)
            {
                VPSS_TNR_CLUT20.bits.data0 = TLUT_CbCr[i][j + 0];
                VPSS_TNR_CLUT20.bits.data1 = TLUT_CbCr[i][j + 1];
                VPSS_TNR_CLUT20.bits.data2 = TLUT_CbCr[i][j + 2];
                VPSS_TNR_CLUT20.bits.data3 = TLUT_CbCr[i][j + 3];

                *(pu32Lut++) = VPSS_TNR_CLUT20.u32;
                j += 4;
            }
        }

        return HI_SUCCESS;

    }

    pu32Lut = (HI_U32*) & (g_pstIPSelVdpAlg->V0_TNR_CLUT20.u32);

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 32;)
        {
            V0_TNR_CLUT20.bits.data0 = TLUT_CbCr[i][j + 0];
            V0_TNR_CLUT20.bits.data1 = TLUT_CbCr[i][j + 1];
            V0_TNR_CLUT20.bits.data2 = TLUT_CbCr[i][j + 2];
            V0_TNR_CLUT20.bits.data3 = TLUT_CbCr[i][j + 3];

            *(pu32Lut++) = V0_TNR_CLUT20.u32;
            j += 4;
        }
    }

    return HI_SUCCESS;
}


