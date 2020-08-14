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


HI_S32 PQ_HAL_EnableSharp(HI_BOOL bOnOff)
{
    HI_U32 u32Data = 0;
    S_VDP_REGS_TYPE* pVDPReg = NULL;
    U_SPCTRL  SPCTRL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_EnableSharp() Select Wrong Video Layer ID\n");
        return HI_FAILURE;
    }

    pVDPReg = PQ_HAL_GetVdpReg();
    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pVDPReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET);
    SPCTRL.bits.sharpen_en = bOnOff;
    PQ_HAL_RegWrite(((HI_U32) & (pVDPReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET), SPCTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSharpStrReg(HI_S32 s32ShpStr, PQ_SHARP_GAIN_S* pstSharpGain)
{
    HI_U32 u32Data = 0;
    HI_U32 u32PeakGain, u32EdgeGain;
    HI_U32 u32SharpIntensity = 128;
    S_VDP_REGS_TYPE* pVdpReg = NULL;
    U_SPGAIN   SPGAIN;

    u32PeakGain = ((pstSharpGain->u32PeakGain) * s32ShpStr + 64) / u32SharpIntensity;
    u32EdgeGain = ((pstSharpGain->u32EdgeGain) * s32ShpStr + 64) / u32SharpIntensity;

    pVdpReg = PQ_HAL_GetVdpReg();
    SPGAIN.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SPGAIN.u32) + u32Data * VDP_VID_OFFSET);
    SPGAIN.bits.peak_gain = u32PeakGain;
    SPGAIN.bits.edge_gain = u32EdgeGain;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SPGAIN.u32) + u32Data * VDP_VID_OFFSET), SPGAIN.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableSharpDemo(HI_BOOL bOnOff)
{
    HI_U32 u32Data = 0;
    S_VDP_REGS_TYPE* pVdpReg = NULL;
    U_SPCTRL         SPCTRL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("select wrong V layer id\n");
        return HI_FAILURE;
    }
    pVdpReg = PQ_HAL_GetVdpReg();
    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET);
    SPCTRL.bits.debug_en = bOnOff;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET), SPCTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSharpDemoMode(HI_U32 u32Data, SHARP_DEMO_MODE_E enMode)
{
    S_VDP_REGS_TYPE* pVdpReg = NULL;
    U_SPCTRL         SPCTRL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("select wrong V layer id\n");
        return HI_FAILURE;
    }
    pVdpReg = PQ_HAL_GetVdpReg();
    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET);
    SPCTRL.bits.demo_mode = enMode;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET), SPCTRL.u32);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_SetSharpDemoPos(HI_U32 u32Data, HI_U32 u32Pos)
{
    S_VDP_REGS_TYPE* pVdpReg = NULL;
    U_SPCTRL         SPCTRL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("select wrong V layer id\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET);
    SPCTRL.bits.demo_pos = u32Pos;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET), SPCTRL.u32);

    return;
}

HI_VOID PQ_HAL_GetSharpDemoPos(HI_U32 u32Data, HI_U32* u32Pos)
{
    S_VDP_REGS_TYPE* pVdpReg = NULL;
    U_SPCTRL         SPCTRL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("select wrong V layer id\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SPCTRL.u32) + u32Data * VDP_VID_OFFSET);
    *u32Pos = SPCTRL.bits.demo_pos;

    return;
}


