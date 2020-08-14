/******************************************************************************

Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     : pq_hal_dbdr.c
Version       : Initial Draft
Author        : d00241380
Created       : 2014/06/18
Description   : De-blocking and De-Ringning

******************************************************************************/

#include "pq_hal_comm.h"
#include "pq_hal_dbdr.h"


/*  Get DR Enable Reg */
HI_S32 PQ_HAL_GetDREnReg(HI_U32 u32HandleNo, HI_BOOL* pbOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);
    
    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.dr_en;

    return HI_SUCCESS;
}

/* Set DR Demo Enable */
HI_S32 PQ_HAL_EnableDRDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DR_CFG1.bits.dr_demo_en = bOnOff;
    
    return HI_SUCCESS;
}

#if 0
/* 设置DR功能开关 */
HI_S32 PQ_HAL_EnableDR(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL.bits.dr_en = bOnOff;

    return HI_SUCCESS;
}
#endif


