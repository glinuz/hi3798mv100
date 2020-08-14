/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_lcacm.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#include <linux/string.h>      /* memcpy() */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "pq_hal_comm.h"
#include "pq_hal_lcacm.h"


static HI_BOOL sg_bACMEnableFlag = HI_TRUE;


HI_VOID PQ_HAL_SetAcm3En(HI_U32 u32Data, HI_U32 acm3_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcm3En Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm3_en = acm3_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcm2En(HI_U32 u32Data, HI_U32 acm2_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcm2En Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm2_en = acm2_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcm1En(HI_U32 u32Data, HI_U32 acm1_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcm1En Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm1_en = acm1_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcm0En(HI_U32 u32Data, HI_U32 acm0_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcm0En Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm0_en = acm0_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmSecBlk0(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmSecBlk0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_sec_blk = acm_sec_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmFirBlk0(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmFirBlk0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_fir_blk = acm_fir_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDUOff0(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDUOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_d_u_off = acm_d_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCUOff0(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCUOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_c_u_off = acm_c_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBUOff0(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBUOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_b_u_off = acm_b_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAUOff0(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM0 VHDACM0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAUOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM0.bits.acm_a_u_off = acm_a_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM0.u32)) + u32Data * PQ_VP_OFFSET), VHDACM0.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmTestEn(HI_U32 u32Data, HI_U32 acm_test_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmTestEn Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_test_en = acm_test_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDbgMode(HI_U32 u32Data, HI_U32 acm_dbg_mode)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmTestEn Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_dbg_mode = acm_dbg_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmProtMode(HI_U32 u32Data, HI_U32 bw_pro_mode)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmTestEn Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.bw_pro_mode = bw_pro_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDVOff0(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDVOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_d_v_off = acm_d_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCVOff0(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCVOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_c_v_off = acm_c_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBVOff0(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBVOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_b_v_off = acm_b_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAVOff0(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM1 VHDACM1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAVOff0 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM1.bits.acm_a_v_off = acm_a_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM1.u32)) + u32Data * PQ_VP_OFFSET), VHDACM1.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmSecBlk1(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmSecBlk1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_sec_blk = acm_sec_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmFirBlk1(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmFirBlk1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_fir_blk = acm_fir_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDUOff1(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDUOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_d_u_off = acm_d_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCUOff1(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCUOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_c_u_off = acm_c_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBUOff1(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBUOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_b_u_off = acm_b_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAUOff1(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM2 VHDACM2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAUOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM2.bits.acm_a_u_off = acm_a_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM2.u32)) + u32Data * PQ_VP_OFFSET), VHDACM2.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDVOff1(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM3 VHDACM3;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDVOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM3.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM3.bits.acm_d_v_off = acm_d_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET), VHDACM3.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCVOff1(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM3 VHDACM3;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCVOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM3.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM3.bits.acm_c_v_off = acm_c_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET), VHDACM3.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBVOff1(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM3 VHDACM3;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBVOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM3.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM3.bits.acm_b_v_off = acm_b_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET), VHDACM3.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAVOff1(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM3 VHDACM3;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAVOff1 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM3.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM3.bits.acm_a_v_off = acm_a_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM3.u32)) + u32Data * PQ_VP_OFFSET), VHDACM3.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmSecBlk2(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmSecBlk2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_sec_blk = acm_sec_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmFirBlk2(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmFirBlk2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_fir_blk = acm_fir_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDUOff2(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDUOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_d_u_off = acm_d_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCUOff2(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCUOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_c_u_off = acm_c_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBUOff2(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBUOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_b_u_off = acm_b_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAUOff2(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM4 VHDACM4;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAUOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM4.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM4.bits.acm_a_u_off = acm_a_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM4.u32)) + u32Data * PQ_VP_OFFSET), VHDACM4.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDVOff2(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM5 VHDACM5;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDVOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM5.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM5.bits.acm_d_v_off = acm_d_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET), VHDACM5.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCVOff2(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM5 VHDACM5;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCVOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM5.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM5.bits.acm_c_v_off = acm_c_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET), VHDACM5.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBVOff2(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM5 VHDACM5;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBVOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM5.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM5.bits.acm_b_v_off = acm_b_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET), VHDACM5.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAVOff2(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM5 VHDACM5;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAVOff2 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM5.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM5.bits.acm_a_v_off = acm_a_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM5.u32)) + u32Data * PQ_VP_OFFSET), VHDACM5.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmSecBlk3(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmSecBlk3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_sec_blk = acm_sec_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmFirBlk3(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmFirBlk3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_fir_blk = acm_fir_blk;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDUOff3(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDUOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_d_u_off = acm_d_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCUOff3(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCUOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_c_u_off = acm_c_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBUOff3(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBUOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_b_u_off = acm_b_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAUOff3(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM6 VHDACM6;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAUOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM6.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM6.bits.acm_a_u_off = acm_a_u_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM6.u32)) + u32Data * PQ_VP_OFFSET), VHDACM6.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmDVOff3(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM7 VHDACM7;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmDVOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM7.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM7.bits.acm_d_v_off = acm_d_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET), VHDACM7.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmCVOff3(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM7 VHDACM7;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmCVOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM7.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM7.bits.acm_c_v_off = acm_c_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET), VHDACM7.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmBVOff3(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM7 VHDACM7;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmBVOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM7.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM7.bits.acm_b_v_off = acm_b_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET), VHDACM7.u32);

    return;
}

HI_VOID PQ_HAL_SetAcmAVOff3(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACM7 VHDACM7;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAcmAVOff3 Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACM7.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET));
    VHDACM7.bits.acm_a_v_off = acm_a_v_off;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACM7.u32)) + u32Data * PQ_VP_OFFSET), VHDACM7.u32);

    return;
}

HI_VOID PQ_HAL_EnableACM(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff)
{
    //   PQ_HAL_SetAcm0En(VDP_LAYER_VP0, bOnOff);
    PQ_HAL_SetAcm1En(VDP_LAYER_VP0, bOnOff);
    PQ_HAL_SetAcm2En(VDP_LAYER_VP0, bOnOff);
    PQ_HAL_SetAcm3En(VDP_LAYER_VP0, bOnOff);

    sg_bACMEnableFlag = bOnOff;

    return;
}

HI_S32 PQ_HAL_GetACMEnableFlag(HI_BOOL *bOnOff)
{
    *bOnOff = sg_bACMEnableFlag;

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_EnableACMDemo(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff)
{
    PQ_HAL_SetAcmTestEn(u32ChId, bOnOff);

    return;
}

HI_VOID PQ_HAL_SetACMDemoMode(PQ_HAL_LAYER_VP_E u32ChId, HAL_ACM_DEMO_MODE_E enACMDemoMode)
{
    PQ_HAL_SetAcmDbgMode(u32ChId, enACMDemoMode);

    return;
}

HI_VOID PQ_HAL_SetLowCostAcmPara(HI_U32 enLayer, PQ_HAL_ACM_PARA_S *pstAcmPara)
{
    PQ_HAL_SetAcm3En     ( enLayer, pstAcmPara->stAcmCfg.acm3_en);
    PQ_HAL_SetAcm2En     ( enLayer, pstAcmPara->stAcmCfg.acm2_en);
    PQ_HAL_SetAcm1En     ( enLayer, pstAcmPara->stAcmCfg.acm1_en);
    //PQ_HAL_SetAcm0En     ( enLayer, pstAcmPara->stAcmCfg.acm0_en);
    PQ_HAL_SetAcmSecBlk0 ( enLayer, pstAcmPara->stAcmCfg.acm0_sec_blk);
    PQ_HAL_SetAcmFirBlk0 ( enLayer, pstAcmPara->stAcmCfg.acm0_fir_blk);
    PQ_HAL_SetAcmTestEn  ( enLayer, pstAcmPara->stAcmCfg.acm_test_en);
    PQ_HAL_SetAcmDbgMode ( enLayer, pstAcmPara->stAcmCfg.acm_dbg_mode);
    PQ_HAL_SetAcmProtMode( enLayer, pstAcmPara->stAcmCfg.bw_pro_mode);
    PQ_HAL_SetAcmSecBlk1 ( enLayer, pstAcmPara->stAcmCfg.acm1_sec_blk);
    PQ_HAL_SetAcmFirBlk1 ( enLayer, pstAcmPara->stAcmCfg.acm1_fir_blk);
    PQ_HAL_SetAcmSecBlk2 ( enLayer, pstAcmPara->stAcmCfg.acm2_sec_blk);
    PQ_HAL_SetAcmFirBlk2 ( enLayer, pstAcmPara->stAcmCfg.acm2_fir_blk);
    PQ_HAL_SetAcmSecBlk3 ( enLayer, pstAcmPara->stAcmCfg.acm3_sec_blk);
    PQ_HAL_SetAcmFirBlk3 ( enLayer, pstAcmPara->stAcmCfg.acm3_fir_blk);

    PQ_HAL_SetAcmDUOff0  ( enLayer, pstAcmPara->acm0_d_u_off);
    PQ_HAL_SetAcmCUOff0  ( enLayer, pstAcmPara->acm0_c_u_off);
    PQ_HAL_SetAcmBUOff0  ( enLayer, pstAcmPara->acm0_b_u_off);
    PQ_HAL_SetAcmAUOff0  ( enLayer, pstAcmPara->acm0_a_u_off);

    PQ_HAL_SetAcmDVOff0  ( enLayer, pstAcmPara->acm0_d_v_off);
    PQ_HAL_SetAcmCVOff0  ( enLayer, pstAcmPara->acm0_c_v_off);
    PQ_HAL_SetAcmBVOff0  ( enLayer, pstAcmPara->acm0_b_v_off);
    PQ_HAL_SetAcmAVOff0  ( enLayer, pstAcmPara->acm0_a_v_off);

    PQ_HAL_SetAcmDUOff1  ( enLayer, pstAcmPara->acm1_d_u_off);
    PQ_HAL_SetAcmCUOff1  ( enLayer, pstAcmPara->acm1_c_u_off);
    PQ_HAL_SetAcmBUOff1  ( enLayer, pstAcmPara->acm1_b_u_off);
    PQ_HAL_SetAcmAUOff1  ( enLayer, pstAcmPara->acm1_a_u_off);
    PQ_HAL_SetAcmDVOff1  ( enLayer, pstAcmPara->acm1_d_v_off);
    PQ_HAL_SetAcmCVOff1  ( enLayer, pstAcmPara->acm1_c_v_off);
    PQ_HAL_SetAcmBVOff1  ( enLayer, pstAcmPara->acm1_b_v_off);
    PQ_HAL_SetAcmAVOff1  ( enLayer, pstAcmPara->acm1_a_v_off);

    PQ_HAL_SetAcmDUOff2  ( enLayer, pstAcmPara->acm2_d_u_off);
    PQ_HAL_SetAcmCUOff2  ( enLayer, pstAcmPara->acm2_c_u_off);
    PQ_HAL_SetAcmBUOff2  ( enLayer, pstAcmPara->acm2_b_u_off);
    PQ_HAL_SetAcmAUOff2  ( enLayer, pstAcmPara->acm2_a_u_off);
    PQ_HAL_SetAcmDVOff2  ( enLayer, pstAcmPara->acm2_d_v_off);
    PQ_HAL_SetAcmCVOff2  ( enLayer, pstAcmPara->acm2_c_v_off);
    PQ_HAL_SetAcmBVOff2  ( enLayer, pstAcmPara->acm2_b_v_off);
    PQ_HAL_SetAcmAVOff2  ( enLayer, pstAcmPara->acm2_a_v_off);

    PQ_HAL_SetAcmDUOff3  ( enLayer, pstAcmPara->acm3_d_u_off);
    PQ_HAL_SetAcmCUOff3  ( enLayer, pstAcmPara->acm3_c_u_off);
    PQ_HAL_SetAcmBUOff3  ( enLayer, pstAcmPara->acm3_b_u_off);
    PQ_HAL_SetAcmAUOff3  ( enLayer, pstAcmPara->acm3_a_u_off);
    PQ_HAL_SetAcmDVOff3  ( enLayer, pstAcmPara->acm3_d_v_off);
    PQ_HAL_SetAcmCVOff3  ( enLayer, pstAcmPara->acm3_c_v_off);
    PQ_HAL_SetAcmBVOff3  ( enLayer, pstAcmPara->acm3_b_v_off);
    PQ_HAL_SetAcmAVOff3  ( enLayer, pstAcmPara->acm3_a_v_off);

    return;
}


