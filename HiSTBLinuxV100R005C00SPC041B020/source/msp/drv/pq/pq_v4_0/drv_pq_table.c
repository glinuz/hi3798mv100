/******************************************************************************
  *
  * Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
  *
  * This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
  *  and may not be copied, reproduced, modified, disclosed to others, published or used, in
  * whole or in part, without the express prior written permission of Hisilicon.

******************************************************************************
  File Name     : drv_pq_table.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/11/06
  Description   :

******************************************************************************/
#include "hi_math.h"
#include "hi_osal.h"

#include "drv_pq.h"
#include "drv_pq_table.h"
#include "pq_hal_table_default.h"

extern HI_CHAR* pq_options_string;

static HI_BOOL sg_bPqTableInitial   = HI_FALSE;
static HI_U32  sg_u32PhyListNum     = PHY_REG_MAX;
static HI_U32  sg_u32SoftListNum    = SOFT_REG_MAX;
static PQ_PHY_REG_S* sg_pstPhyReg   = HI_NULL;
static PQ_PHY_REG_S* sg_pstSoftReg  = HI_NULL;
static HI_PQ_BIN_MODULE_POS_S sg_astBinPos[HI_PQ_MODULE_BUTT];

static HI_VOID pq_table_GetBootargs(HI_VOID)
{
    if (HI_NULL == pq_options_string)
    {
        return;
    }

    if (0 == HI_OSAL_Strncmp(pq_options_string, "noacmuhd", sizeof("noacmuhd")))
    {
        PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_COLOR, SOURCE_MODE_UHD, HI_FALSE);
    }

    return;
}

static HI_U32 pq_table_CalculateSoftListNum (HI_VOID)
{
    HI_U32 i;

    if (HI_NULL == sg_pstSoftReg)
    {
        return 0;
    }

    for (i = 0; i < SOFT_REG_MAX; i++)
    {
        if ((0 == sg_pstSoftReg[i].u32RegAddr)
            && (0 == sg_pstSoftReg[i].u8Lsb)
            && (0 == sg_pstSoftReg[i].u8Msb))
        {
            break;
        }
    }

    return i;
}

static HI_U32 pq_table_CalculatePhyListNum (HI_VOID)
{
    HI_U32 i;

    if (HI_NULL == sg_pstPhyReg)
    {
        return 0;
    }

    for (i = 0; i < PHY_REG_MAX; i++)
    {
        if ((0 == sg_pstPhyReg[i].u32RegAddr)
            && (0 == sg_pstPhyReg[i].u8Lsb)
            && (0 == sg_pstPhyReg[i].u8Msb))
        {
            break;
        }
    }

    return i;
}

static HI_VOID pq_table_CalculatePositionPerAlg (HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 u32Order = 0;

    /*init variable about pq table*/
    for (i = 0; i < HI_PQ_MODULE_BUTT; i++)
    {
        sg_astBinPos[i].u32StartPos = PHY_REG_MAX - 1;
        sg_astBinPos[i].u32EndPos   = 0;
    }

    /* calculate start position and end position of alg module*/
    for (i = 0; i < PHY_REG_MAX; i++)
    {
        if ((0 == sg_pstPhyReg[i].u32RegAddr)
            && (0 == sg_pstPhyReg[i].u8Lsb)
            && (0 == sg_pstPhyReg[i].u8Msb))
        {
            break;
        }

        /*init start position and end position of pq bin module*/
        u32Order = sg_pstPhyReg[i].u32Module;
        if (u32Order >= HI_PQ_MODULE_BUTT)
        {
            continue;
        }

        sg_astBinPos[u32Order].u32StartPos = MIN2(sg_astBinPos[u32Order].u32StartPos, i);
        sg_astBinPos[u32Order].u32EndPos   = MAX2(sg_astBinPos[u32Order].u32EndPos, i);
    }

    return;
}


HI_S32 PQ_TABLE_DeInitPQTable(HI_VOID)
{
    if (sg_bPqTableInitial == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    sg_bPqTableInitial = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 PQ_TABLE_InitPQTable(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    if (sg_bPqTableInitial == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    PQ_CHECK_NULL_PTR(pstPqParam);

    sg_pstPhyReg  = pstPqParam->stPQPhyReg;
    sg_pstSoftReg = pstPqParam->stPQSoftReg;

    if (HI_TRUE == bDefault)
    {
        /*set default pqparam form code*/
        PQ_TABLE_SetTableRegDefault();
    }

    pq_table_CalculatePositionPerAlg();

    sg_u32PhyListNum  = pq_table_CalculatePhyListNum();
    sg_u32SoftListNum = pq_table_CalculateSoftListNum();

    HI_INFO_PQ("Phy Register Num:%d\n", sg_u32PhyListNum);
    HI_INFO_PQ("Soft Register Num:%d\n", sg_u32SoftListNum);

    pstPqParam->stPQFileHeader.u32PhyRegNum = sg_u32PhyListNum;
    pstPqParam->stPQFileHeader.u32SoftRegNum = sg_u32SoftListNum;

    PQ_TABLE_InitSoftTable(HI_TRUE);
    pq_table_GetBootargs();

    sg_bPqTableInitial = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 PQ_TABLE_SetVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32RegValue = 0;

    if (PQ_HAL_CheckVdpValid() == HI_TRUE)
    {
        s32Ret = PQ_HAL_ReadRegister(0, u32Addr, &u32RegValue);
        if (HI_SUCCESS != s32Ret)
        { return HI_FAILURE; }
        PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
        s32Ret = PQ_HAL_WriteRegister(0, u32Addr, u32RegValue);
    }
    else
    {
        HI_ERR_PQ("PQ_HAL_CheckVdpValid false\n");
    }

    return s32Ret;
}

static HI_S32 pq_table_SetVpssRegByHandleNo(HI_U32 u32HandleNo, HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32RegValue = 0;

    s32Ret = PQ_HAL_ReadRegister(u32HandleNo, u32Addr, &u32RegValue);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
    return PQ_HAL_WriteRegister(u32HandleNo, u32Addr, u32RegValue);
}


HI_S32 PQ_TABLE_SetVpssReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    HI_S32 i = 0;

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        if (PQ_HAL_CheckVpssValid(i) == HI_FALSE)
        {
            continue;
        }

        pq_table_SetVpssRegByHandleNo(i, u32Addr, u8Lsb, u8Msb, u32Value);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_TABLE_SetSelVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
#if defined(CHIP_TYPE_hi3798cv200)
    HI_U32 u32RegValue = 0;
    HI_U32 u32OffsetAddr =  u32Addr & REG_OFFSET_ADDR_MASK;

    u32RegValue = *((HI_U32*)((HI_VOID*)g_pstIPSelVdpAlg + u32OffsetAddr));
    PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
    *(HI_U32*)((HI_VOID*)g_pstIPSelVdpAlg + u32OffsetAddr) = u32RegValue;
#endif

    return HI_SUCCESS;
}

static HI_S32 pq_table_SetSelVdpRegByModule(HI_U32 u32Module, HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    if ((HI_PQ_MODULE_DB == u32Module)
        || (HI_PQ_MODULE_DM == u32Module)
        || (HI_PQ_MODULE_DR == u32Module)
        || (HI_PQ_MODULE_DS == u32Module))
    {
        u32Addr += 0xB300;
    }
    else if ((HI_PQ_MODULE_TNR == u32Module) || (HI_PQ_MODULE_SNR == u32Module))
    {
        u32Addr += 0xB000;
    }
    else
    {
        return HI_FAILURE;
    }

    PQ_TABLE_SetSelVdpReg(u32Addr, u8Lsb, u8Msb, u32Value);

    return HI_SUCCESS;
}


HI_S32 PQ_TABLE_LoadPhyList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32HandleNo)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U8 u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = 0; i < sg_u32PhyListNum; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u32Module     = sg_pstPhyReg[i].u32Module;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        u32Addr &= REG_OFFSET_ADDR_MASK;

        if ((HI_U32)enRegType != PQ_COMM_GetAlgTypeID(u32Module))
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

        if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(u32Module))
        {
            u32Addr |= VPSS_REGS_ADDR;
            s32Ret = pq_table_SetVpssRegByHandleNo(u32HandleNo, u32Addr, u8Lsb, u8Msb, u32Value);
        }
        else if (REG_TYPE_VDP == PQ_COMM_GetAlgTypeID(u32Module))
        {
            s32Ret = pq_table_SetSelVdpRegByModule(u32Module, u32Addr, u8Lsb, u8Msb, u32Value);
            if (s32Ret == HI_SUCCESS)
            {
                continue;
            }

            u32Addr |= VDP_REGS_ADDR;
            s32Ret = PQ_TABLE_SetVdpReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }

        pqprint(PQ_PRN_TABLE, "LoadPhyList,Load Addr:0x%x, SourceMode:%d, OutputMode:%d, Module:%d\n", u32Addr, u8SourceMode, u8OutputMode, u32Module);
    }

    return s32Ret;
}

HI_S32 PQ_TABLE_LoadPhyListOfAlg(HI_PQ_MODULE_E enModule, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U32 u32StartPos = 0;
    HI_U32 u32EndPos = 0;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    u32StartPos = sg_astBinPos[enModule].u32StartPos;
    u32EndPos = sg_astBinPos[enModule].u32EndPos;

    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u32Module     = sg_pstPhyReg[i].u32Module;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        u32Addr &= REG_OFFSET_ADDR_MASK;

        if ((HI_U32)enModule != u32Module)
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

        if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(enModule))
        {
            u32Addr |= VPSS_REGS_ADDR;
            s32Ret = PQ_TABLE_SetVpssReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }
        else if (REG_TYPE_VDP == PQ_COMM_GetAlgTypeID(enModule))
        {
            s32Ret = pq_table_SetSelVdpRegByModule(u32Module, u32Addr, u8Lsb, u8Msb, u32Value);
            if (s32Ret == HI_SUCCESS)
            {
                continue;
            }

            u32Addr |= VDP_REGS_ADDR;
            s32Ret = PQ_TABLE_SetVdpReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }

        pqprint(PQ_PRN_TABLE, "LoadPhyListOfAlg,Load Addr:0x%x, SourceMode:%d, OutputMode:%d, Module:%d\n", u32Addr, u8SourceMode, u8OutputMode, u32Module);
    }

    return s32Ret;
}

HI_S32 PQ_TABLE_LoadMultiList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_MODULE_E enModule;


    for (enModule = 0; enModule < HI_PQ_MODULE_BUTT; enModule++)
    {
        if (PQ_BIN_ADAPT_MULTIPLE != PQ_COMM_GetAlgAdapeType(enModule))
        {
            continue;
        }

        if ((HI_U32)enRegType != PQ_COMM_GetAlgTypeID(enModule))
        {
            continue;
        }

        s32Ret = PQ_TABLE_LoadPhyListOfAlg(enModule, enSourceMode, enOutputMode);
    }

    return s32Ret;
}

HI_S32 PQ_TABLE_InitPhyList(HI_U32 u32ID, HI_PQ_MODULE_E enModule, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE* pVdpVirReg    = NULL;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U32 u32RegValue   = 0;
    HI_U32 u32OffsetAddr = 0x0;
    HI_U32 u32StartPos   = 0;
    HI_U32 u32EndPos     = 0;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    u32StartPos = sg_astBinPos[enModule].u32StartPos;
    u32EndPos   = sg_astBinPos[enModule].u32EndPos;

    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u32Module     = sg_pstPhyReg[i].u32Module;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;
        u32OffsetAddr = u32Addr & REG_OFFSET_ADDR_MASK;

        if ((HI_U32)enModule != u32Module)
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

        if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(enModule))
        {
            pstVpssVirReg = PQ_HAL_GetVpssReg(u32ID);
            PQ_CHECK_NULL_PTR(pstVpssVirReg);

            u32RegValue = *((HI_U32*)((HI_VOID*)pstVpssVirReg + u32OffsetAddr));
            PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
            *(HI_U32*)((HI_VOID*)pstVpssVirReg + u32OffsetAddr) = u32RegValue;
        }
        else if (REG_TYPE_VDP == PQ_COMM_GetAlgTypeID(enModule))
        {
            pVdpVirReg = PQ_HAL_GetVdpReg();
            PQ_CHECK_NULL_PTR(pVdpVirReg);

            if ((HI_PQ_MODULE_DB == enModule)
                || (HI_PQ_MODULE_DM == enModule)
                || (HI_PQ_MODULE_DR == enModule)
                || (HI_PQ_MODULE_DS == enModule)
                || (HI_PQ_MODULE_TNR == enModule)
                || (HI_PQ_MODULE_SNR == enModule))
            {
                //u32OffsetAddr += 0xB300;
                //u32OffsetAddr += 0xB000;
                continue;
            }

            u32RegValue = *((HI_U32*)((HI_VOID*)pVdpVirReg + u32OffsetAddr));
            PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
            *(HI_U32*)((HI_VOID*)pVdpVirReg + u32OffsetAddr) = u32RegValue;
        }

        pqprint(PQ_PRN_TABLE, "InitPhyList, Addr:0x%x, Lsb:%d, Msb:%d, Source:%d, Output:%d, Module:%d, Value:%d\n", u32Addr, u8Lsb, u8Msb, u8SourceMode, u8OutputMode, u32Module, u32Value);
    }

    return HI_SUCCESS;
}

HI_U32 PQ_TABLE_GetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultValue)
{
    HI_S32 i;
    HI_U32 u32Addr, u32Value;
    HI_U8  u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstSoftReg);

    for (i = 0; i < sg_u32SoftListNum;  i++)
    {
        u32Addr       = sg_pstSoftReg[i].u32RegAddr;
        u32Value      = sg_pstSoftReg[i].u32Value;
        u8SourceMode  = sg_pstSoftReg[i].u8SourceMode;
        u8OutputMode  = sg_pstSoftReg[i].u8OutputMode;

        if (HI_FALSE == PQ_HAL_IsSpecialReg(u32Addr))
        {
            break;
        }

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

        return u32Value;

    }

    return u32DefaultValue;

}

HI_VOID pq_table_ProcessInitSoftTable(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 u32ColorTempRed   = 50;
    HI_U32 u32ColorTempGreen = 50;
    HI_U32 u32ColorTempBlue  = 50;
    HI_U8  u8Value = u32Value & 0x000000ff;

    HI_INFO_PQ("u32Addr:%#x, u8Value:%d\n", u32Addr, u8Value);

    switch (u32Addr)
    {
        case PQ_SOFT_COLOR_TEMP_R:
            if (GET_ALG(HI_PQ_MODULE_CSC)
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp)
            {
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp(&u32ColorTempRed, &u32ColorTempGreen, &u32ColorTempBlue);
                u32ColorTempRed = u32Value;
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp(u32ColorTempRed, u32ColorTempGreen, u32ColorTempBlue);
            }
            break;
        case PQ_SOFT_COLOR_TEMP_G:
            if (GET_ALG(HI_PQ_MODULE_CSC)
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp)
            {
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp(&u32ColorTempRed, &u32ColorTempGreen, &u32ColorTempBlue);
                u32ColorTempGreen = u32Value;
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp(u32ColorTempRed, u32ColorTempGreen, u32ColorTempBlue);
            }
            break;
        case PQ_SOFT_COLOR_TEMP_B:
            if (GET_ALG(HI_PQ_MODULE_CSC)
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp
                && GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp)
            {
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->GetColorTemp(&u32ColorTempRed, &u32ColorTempGreen, &u32ColorTempBlue);
                u32ColorTempBlue = u32Value;
                GET_ALG_FUN(HI_PQ_MODULE_CSC)->SetColorTemp(u32ColorTempRed, u32ColorTempGreen, u32ColorTempBlue);
            }
            break;
        case PQ_SOFT_HDR_SATU:
            DRV_PQ_SetHdrSaturation(u8Value);
            break;
        case PQ_SOFT_MODULE_FMD_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_FMD, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_FMD_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_FMD, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_FMD_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_FMD, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_TNR_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_TNR, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_TNR_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_TNR, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_TNR_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_TNR, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_SNR_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SNR, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_SNR_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SNR, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_SNR_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SNR, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_DB_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DB, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_DB_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DB, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_DB_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DB, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_DR_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DR, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_DR_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DR, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_DR_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DR, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_DM_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DM, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_DM_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DM, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_DM_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DM, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_SHARPN_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SHARPNESS, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_SHARPN_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SHARPNESS, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_SHARPN_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_SHARPNESS, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_DCI_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DCI, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_DCI_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DCI, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_DCI_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DCI, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_ACM_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_COLOR, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_ACM_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_COLOR, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_ACM_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_COLOR, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_CSC_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_CSC, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_CSC_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_CSC, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_CSC_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_CSC, SOURCE_MODE_UHD, u8Value);
            break;
        case PQ_SOFT_MODULE_DS_SD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DS, SOURCE_MODE_SD, u8Value);
            break;
        case PQ_SOFT_MODULE_DS_HD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DS, SOURCE_MODE_HD, u8Value);
            break;
        case PQ_SOFT_MODULE_DS_UHD:
            PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_DS, SOURCE_MODE_UHD, u8Value);
            break;
        default:
        {
            break;
        }
    }
}

HI_S32 PQ_TABLE_InitSoftTable(HI_BOOL bInit)
{
    HI_S32 i;
    HI_U32 u32Addr, u32Value;

    if ((bInit == HI_FALSE) && (PQ_TUN_NORMAL == g_stPqStatus.enTunMode))
    {
        return HI_FAILURE;
    }

    if (bInit == HI_TRUE)
    {
        for (i = 0; i < HI_PQ_MODULE_BUTT; i++)
        {
            if (HI_NULL == GET_ALG(i))
            {
                continue;
            }
            PQ_COMM_SetMoudleCtrl(i, SOURCE_MODE_NO, HI_TRUE);
            PQ_COMM_SetMoudleCtrl(i, SOURCE_MODE_SD, HI_TRUE);
            PQ_COMM_SetMoudleCtrl(i, SOURCE_MODE_HD, HI_TRUE);
            PQ_COMM_SetMoudleCtrl(i, SOURCE_MODE_UHD, HI_TRUE);
        }
    }

    PQ_CHECK_NULL_PTR(sg_pstSoftReg);

    for (i = 0; i < sg_u32SoftListNum;  i++)
    {
        u32Addr       = sg_pstSoftReg[i].u32RegAddr;
        u32Value      = sg_pstSoftReg[i].u32Value;

        if (HI_FALSE == PQ_HAL_IsSpecialReg(u32Addr))
        {
            break;
        }

        pq_table_ProcessInitSoftTable(u32Addr, u32Value);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_TABLE_FindBinPos(HI_PQ_MODULE_E enModule, HI_U32* pu32StartPos, HI_U32* pu32EndPos)
{
    *pu32StartPos = sg_astBinPos[enModule].u32StartPos;
    *pu32EndPos = sg_astBinPos[enModule].u32EndPos;

    return HI_SUCCESS;
}

HI_S32 PQ_TABLE_SetTableRegDefault(HI_VOID)
{
    HI_U32 i;
    PQ_PHY_REG_S* pstPhyRegDefault  = PQ_TABLE_GetPhyRegDefault();
    PQ_PHY_REG_S* pstSoftRegDefault = PQ_TABLE_GetSoftRegDefault();

    PQ_CHECK_NULL_PTR(pstPhyRegDefault);
    PQ_CHECK_NULL_PTR(pstSoftRegDefault);
    PQ_CHECK_NULL_PTR(sg_pstPhyReg);
    PQ_CHECK_NULL_PTR(sg_pstSoftReg);

    memset(sg_pstPhyReg, 0, sizeof(PQ_PHY_REG_S)*PHY_REG_MAX);
    memset(sg_pstSoftReg, 0, sizeof(PQ_PHY_REG_S)*SOFT_REG_MAX);

    for (i = 0; i < PHY_REG_MAX; i++)
    {
        if ((0 == pstPhyRegDefault[i].u32RegAddr)
            && (0 == pstPhyRegDefault[i].u8Lsb)
            && (0 == pstPhyRegDefault[i].u8Msb))
        {
            break;
        }
        sg_pstPhyReg[i] = pstPhyRegDefault[i];
    }

    for (i = 0; i < SOFT_REG_MAX; i++)
    {
        if ((0 == pstSoftRegDefault[i].u32RegAddr)
            && (0 == pstSoftRegDefault[i].u8Lsb)
            && (0 == pstSoftRegDefault[i].u8Msb))
        {
            break;
        }
        sg_pstSoftReg[i] = pstSoftRegDefault[i];
    }

    return HI_SUCCESS;
}


PQ_PHY_REG_S* PQ_TABLE_GetPhyReg(HI_VOID)
{
    return sg_pstPhyReg;
}

PQ_PHY_REG_S* PQ_TABLE_GetSoftReg(HI_VOID)
{
    return sg_pstSoftReg;
}

HI_U32 PQ_TABLE_GetPhyListNum(HI_VOID)
{
    return sg_u32PhyListNum;
}

HI_U32 PQ_TABLE_GetSoftListNum(HI_VOID)
{
    return sg_u32SoftListNum;
}


