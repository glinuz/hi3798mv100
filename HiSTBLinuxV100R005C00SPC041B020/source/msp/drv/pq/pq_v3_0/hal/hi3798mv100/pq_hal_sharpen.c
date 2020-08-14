/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_sharpen.c
  Version       : Initial Draft
  Author        :
  Created       : 2013/11/2
  Description   : Hisilicon multimedia software group
******************************************************************************/
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "pq_hal_sharpen.h"


/* Enable Sharpen LTI, There is Only HD in 98M */
static HI_S32 PQ_HAL_EnableLTI(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_VHD_LTI_CTRL.bits.lti_en = bOnOff;

    return HI_SUCCESS;
}

/* Enable Sharpen CTI, There is Only HD in 98M */
static HI_S32 PQ_HAL_EnableCTI(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_VHD_CTI_CTRL.bits.cti_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableSharp(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret  = PQ_HAL_EnableLTI(u32HandleNo, bOnOff);
            s32Ret |= PQ_HAL_EnableCTI(u32HandleNo, bOnOff);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in sharpen enable!\n");
                return HI_FAILURE;
            }
        }
    }
    return HI_SUCCESS;
}

/* Set sharpen Strength reg, dGlbGain: Global variable; Range:0~255 */
static HI_S32 PQ_HAL_SetSharpStr(HI_U32 u32HandleNo, HI_S32 s32ShpStr, PQ_SHARP_GAIN_S* pstSharpGain)
{
    HI_U32 u32SharpIntensity = 128;
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    /* VHD */
    pstVpssVirReg->VPSS_VHD_LTI_CTRL.bits.lgain_ratio = ((pstSharpGain->u32HD_Lgain_ratio) * s32ShpStr + 64) / u32SharpIntensity;
    pstVpssVirReg->VPSS_VHD_CTI_CTRL.bits.cgain_ratio = ((pstSharpGain->u32HD_Cgain_ratio) * s32ShpStr + 64) / u32SharpIntensity;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSharpStrReg(HI_S32 s32ShpStr, PQ_SHARP_GAIN_S* pstSharpGain)
{
    HI_U32 u32HandleNo;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            PQ_HAL_SetSharpStr(u32HandleNo, s32ShpStr, pstSharpGain);
        }
    }

    return HI_SUCCESS;
}

/* Enable Sharpen CTI Demo */
static HI_S32 PQ_HAL_EnableCTIDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_VHD_CTI_CTRL.bits.cti_demo_en = bOnOff;

    return HI_SUCCESS;
}

/* Enable Sharpen LTI Demo */
static HI_S32 PQ_HAL_EnableLTIDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_VHD_LTI_CTRL.bits.lti_demo_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableSharpDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret  = PQ_HAL_EnableLTIDemo(u32HandleNo, bOnOff);
            s32Ret |= PQ_HAL_EnableCTIDemo(u32HandleNo, bOnOff);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in sharpen demo enable!\n");
                return HI_FAILURE;
            }
        }
    }
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSharpDemoMode(HI_U32 u32Data, SHARP_DEMO_MODE_E enMode)
{
    return HI_SUCCESS;
}

HI_VOID PQ_HAL_SetSharpDemoPos(HI_U32 u32Data, HI_U32 u32Pos)
{
    return;
}

HI_VOID PQ_HAL_GetSharpDemoPos(HI_U32 u32Data, HI_U32* u32Pos)
{
    return;
}


