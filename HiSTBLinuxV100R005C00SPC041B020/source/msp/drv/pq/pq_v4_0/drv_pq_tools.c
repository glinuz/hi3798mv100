/******************************************************************************
  *
  * Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
  *
  * This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
  *  and may not be copied, reproduced, modified, disclosed to others, published or used, in
  * whole or in part, without the express prior written permission of Hisilicon.

******************************************************************************
  File Name     : drv_pq_tools.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/06/02
  Description   :

******************************************************************************/
#include "drv_pq_table.h"
#include "drv_pq.h"
#include "pq_hal_comm.h"

static HI_BOOL pq_tools_IsSelVdpReg(HI_U32 u32RegAddr)
{
    HI_U32 u32OffsetAddr =	u32RegAddr & REG_OFFSET_ADDR_MASK;

    if ((u32RegAddr & REG_BASE_ADDR_MASK) != VDP_REGS_ADDR)
    {
        return HI_FALSE;
    }

    if ((u32OffsetAddr >= 0xD800) && (u32OffsetAddr <= 0xEE7C))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_S32 pq_tools_UpdatePhyList(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U8 u8SourceMode, HI_U8 u8OutputMode, HI_U32 u32Value)
{
    HI_U32 i;
    HI_U32 u32PhyListNum = PQ_TABLE_GetPhyListNum();
    PQ_PHY_REG_S* pstPhyReg = PQ_TABLE_GetPhyReg();

    PQ_CHECK_NULL_PTR(pstPhyReg);

    for (i = 0; i < u32PhyListNum; i++)
    {
        if ((u32Addr & REG_OFFSET_ADDR_MASK) != pstPhyReg[i].u32RegAddr)
        {
            continue;
        }
        if (u8Lsb != pstPhyReg[i].u8Lsb)
        {
            continue;
        }
        if (u8Msb != pstPhyReg[i].u8Msb)
        {
            continue;
        }
        if ((SOURCE_MODE_NO != pstPhyReg[i].u8SourceMode) && (u8SourceMode != pstPhyReg[i].u8SourceMode))
        {
            continue;
        }
        if ((SOURCE_MODE_NO != pstPhyReg[i].u8OutputMode) && (u8OutputMode != pstPhyReg[i].u8OutputMode))
        {
            continue;
        }

        pstPhyReg[i].u32Value = u32Value;
        return i;
    }

    return HI_FAILURE;
}

static HI_S32 pq_tools_SetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32Value)
{
    HI_S32 i;
    HI_U32 u32Addr;
    HI_U8  u8SourceMode, u8OutputMode;
    HI_U32 u32SoftListNum    = PQ_TABLE_GetSoftListNum();
    PQ_PHY_REG_S* pstSoftReg = PQ_TABLE_GetSoftReg();

    PQ_CHECK_NULL_PTR(pstSoftReg);

    for (i = 0; i < u32SoftListNum;  i++)
    {
        u32Addr       = pstSoftReg[i].u32RegAddr;
        u8SourceMode  = pstSoftReg[i].u8SourceMode;
        u8OutputMode  = pstSoftReg[i].u8OutputMode;

        if (u32Addr != u32Lut)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        pstSoftReg[i].u32Value = u32Value;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}


HI_S32 PQ_TOOLS_SetRegister(HI_PQ_REGISTER_S* pstAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Num = -1;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();
    PQ_OUTPUT_MODE_E enOutputMode = PQ_COMM_GetOutputMode();
    PQ_PHY_REG_S* pstPhyReg = PQ_TABLE_GetPhyReg();

    PQ_CHECK_NULL_PTR(pstPhyReg);
	PQ_CHECK_NULL_PTR(pstAttr);

    if (PQ_HAL_IsSpecialReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        return pq_tools_SetSoftTable(pstAttr->u32RegAddr,
                                     pstAttr->u8SourceMode,
                                     pstAttr->u8OutputMode,
                                     pstAttr->u32Value);
    }

    s32Num = pq_tools_UpdatePhyList(pstAttr->u32RegAddr,
                                    pstAttr->u8Lsb,
                                    pstAttr->u8Msb,
                                    pstAttr->u8SourceMode,
                                    pstAttr->u8OutputMode,
                                    pstAttr->u32Value);

    if (HI_FAILURE == s32Num)
    {
        pqprint(PQ_PRN_TABLE, "Warning! not find Register[Address:0x%x, Bit:%d~%d],SourceMode:[%d],OutputMode:[%d]\n", \
                pstAttr->u32RegAddr, pstAttr->u8Lsb, pstAttr->u8Msb, pstAttr->u8SourceMode, pstAttr->u8OutputMode);
    }
    else
    {
        if ((SOURCE_MODE_NO != pstPhyReg[s32Num].u8SourceMode)
            && (pstAttr->u8SourceMode != enSourceMode))
        {
            HI_ERR_PQ("Current Source Mode:[%d], Set Source Mode:[%d],Not Set Physical Reg\n",
                      enSourceMode, pstAttr->u8SourceMode);
            return HI_SUCCESS;
        }

        if ((OUTPUT_MODE_NO != pstPhyReg[s32Num].u8OutputMode)
            && (pstAttr->u8OutputMode != enOutputMode))
        {
            HI_ERR_PQ("Current Output Mode:[%d], Set Output Mode:[%d],Not Set Physical Reg\n",
                      enOutputMode, pstAttr->u8OutputMode);
            return HI_SUCCESS;
        }
    }

    if (pq_tools_IsSelVdpReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        return PQ_TABLE_SetSelVdpReg(pstAttr->u32RegAddr,
                                     pstAttr->u8Lsb,
                                     pstAttr->u8Msb,
                                     pstAttr->u32Value);
    }

    if (PQ_HAL_IsVpssReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        s32Ret = PQ_TABLE_SetVpssReg(pstAttr->u32RegAddr,
                                     pstAttr->u8Lsb,
                                     pstAttr->u8Msb,
                                     pstAttr->u32Value);
    }
    else if (PQ_HAL_IsVdpReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        s32Ret = PQ_TABLE_SetVdpReg(pstAttr->u32RegAddr,
                                    pstAttr->u8Lsb,
                                    pstAttr->u8Msb,
                                    pstAttr->u32Value);
    }
    else
    {
        HI_ERR_PQ("not VPSS/VDP/Special Register!\n");
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 PQ_TOOLS_GetRegister(HI_PQ_REGISTER_S* pstAttr)
{
    HI_U32 u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    HI_U32 i;
    HI_U32 u32PhyListNum = PQ_TABLE_GetPhyListNum();
    PQ_PHY_REG_S* pstPhyReg = PQ_TABLE_GetPhyReg();

    PQ_CHECK_NULL_PTR(pstPhyReg);
	PQ_CHECK_NULL_PTR(pstAttr);

    if (PQ_HAL_IsSpecialReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        pstAttr->u32Value = PQ_TABLE_GetSoftTable(pstAttr->u32RegAddr, pstAttr->u8SourceMode, pstAttr->u8OutputMode, 0);
        return HI_SUCCESS;
    }

    for (i = 0; i < u32PhyListNum; i++)
    {
        u32Addr       = pstPhyReg[i].u32RegAddr;
        u32Value      = pstPhyReg[i].u32Value;
        u8Lsb         = pstPhyReg[i].u8Lsb;
        u8Msb         = pstPhyReg[i].u8Msb;
        u8SourceMode  = pstPhyReg[i].u8SourceMode;
        u8OutputMode  = pstPhyReg[i].u8OutputMode;

        if (u32Addr != (pstAttr->u32RegAddr & REG_OFFSET_ADDR_MASK))
        {
            continue;
        }

        if (u8Lsb != pstAttr->u8Lsb)
        {
            continue;
        }

        if (u8Msb != pstAttr->u8Msb)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != pstAttr->u8SourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != pstAttr->u8OutputMode))
        {
            continue;
        }

        pstAttr->u32Value = u32Value;
        return HI_SUCCESS;
    }

    HI_ERR_PQ("Error! not find Phy Register List[Address:0x%x, Bit:%d~%d],SourceMode:[%d], OutputMode:[%d]\n",
              pstAttr->u32RegAddr, pstAttr->u8Lsb, pstAttr->u8Msb, pstAttr->u8SourceMode, pstAttr->u8OutputMode);

    return HI_FAILURE;
}

HI_S32 PQ_TOOLS_GetBinPhyAddr(HI_U32* pu32Addr)
{
    PQ_CHECK_NULL_PTR(pu32Addr);

    DRV_PQ_GetBinPhyAddr(pu32Addr);

    return HI_SUCCESS;
}

