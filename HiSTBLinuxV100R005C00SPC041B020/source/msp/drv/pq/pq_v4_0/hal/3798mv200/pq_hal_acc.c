/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_acc.c
  Version       : Initial Draft
  Author        :
  Created       : 2016/09/23
  Description   :

******************************************************************************/
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "pq_hal_acc.h"


HI_VOID PQ_HAL_SetAccAccEn(HI_U32 u32Data, HI_U32 acc_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD1 VHDACCTHD1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccEn Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD1.bits.acc_en = acc_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD1.u32);

    return;
}

HI_VOID PQ_HAL_SetAccAccMode(HI_U32 u32Data, HI_U32 acc_mode)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD1 VHDACCTHD1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccMode Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD1.bits.acc_mode = acc_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD1.u32);

    return;
}

HI_VOID PQ_HAL_SetAccThdMedLow(HI_U32 u32Data, HI_U32 thd_med_low)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD1 VHDACCTHD1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccThdMedLow Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD1.bits.thd_med_low = thd_med_low;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD1.u32);

    return;
}

HI_VOID PQ_HAL_SetAccThdHigh(HI_U32 u32Data, HI_U32 thd_high)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD1 VHDACCTHD1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccThdHigh Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD1.bits.thd_high = thd_high;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD1.u32);

    return;
}

HI_VOID PQ_HAL_SetAccThdLow(HI_U32 u32Data, HI_U32 thd_low)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD1 VHDACCTHD1;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccThdLow Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD1.bits.thd_low = thd_low;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD1.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD1.u32);

    return;
}

HI_VOID PQ_HAL_SetAccAccRst(HI_U32 u32Data, HI_U32 acc_rst)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD2 VHDACCTHD2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccRst Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD2.bits.acc_rst = acc_rst;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD2.u32);

    return;
}

HI_VOID PQ_HAL_SetAccDbgMode(HI_U32 u32Data, HI_U32 acc_dbg_mode)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD2 VHDACCTHD2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccRst Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD2.bits.acc_dbg_mode = acc_dbg_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD2.u32);

    return;
}

HI_VOID PQ_HAL_SetAccDbgEn(HI_U32 u32Data, HI_U32 acc_dbg_en)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD2 VHDACCTHD2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccRst Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD2.bits.acc_dbg_en = acc_dbg_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD2.u32);

    return;
}

HI_VOID PQ_HAL_SetAccAccMultiple(HI_U32 u32Data, HI_U32 acc_multiple)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD2 VHDACCTHD2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccMultiple Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD2.bits.acc_multiple = acc_multiple;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD2.u32);

    return;
}

HI_VOID PQ_HAL_SetAccThdMedHigh(HI_U32 u32Data, HI_U32 thd_med_high)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTHD2 VHDACCTHD2;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccThdMedHigh Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTHD2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTHD2.bits.thd_med_high = thd_med_high;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTHD2.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTHD2.u32);

    return;
}

HI_VOID PQ_HAL_SetAccCnt3Low(HI_U32 u32Data, HI_U32 cnt3_low)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACC3LOW VHDACC3LOW;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccCnt3Low Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACC3LOW.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3LOW.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACC3LOW.bits.cnt3_low = cnt3_low;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3LOW.u32)) + u32Data * PQ_VID_OFFSET), VHDACC3LOW.u32);

    return;
}

HI_VOID PQ_HAL_SetAccCnt3Med(HI_U32 u32Data, HI_U32 cnt3_med)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACC3MED VHDACC3MED;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccCnt3Med Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACC3MED.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3MED.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACC3MED.bits.cnt3_med = cnt3_med;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3MED.u32)) + u32Data * PQ_VID_OFFSET), VHDACC3MED.u32);

    return;
}

HI_VOID PQ_HAL_SetAccCnt3High(HI_U32 u32Data, HI_U32 cnt3_high)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACC3HIGH VHDACC3HIGH;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccCnt3High Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACC3HIGH.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3HIGH.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACC3HIGH.bits.cnt3_high = cnt3_high;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC3HIGH.u32)) + u32Data * PQ_VID_OFFSET), VHDACC3HIGH.u32);

    return;
}

HI_VOID PQ_HAL_SetAccCnt8MedLow(HI_U32 u32Data, HI_U32 cnt8_med_low)
{

    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACC8MLOW VHDACC8MLOW;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccCnt8MedLow Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACC8MLOW.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC8MLOW.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACC8MLOW.bits.cnt8_med_low = cnt8_med_low;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC8MLOW.u32)) + u32Data * PQ_VID_OFFSET), VHDACC8MLOW.u32);

    return;
}

HI_VOID PQ_HAL_SetAccCnt8MedHigh(HI_U32 u32Data, HI_U32 cnt8_med_high)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACC8MHIGH VHDACC8MHIGH;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccCnt8MedHigh Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACC8MHIGH.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC8MHIGH.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACC8MHIGH.bits.cnt8_med_high = cnt8_med_high;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACC8MHIGH.u32)) + u32Data * PQ_VID_OFFSET), VHDACC8MHIGH.u32);

    return;
}

HI_VOID PQ_HAL_SetAccAccPixTotal(HI_U32 u32Data, HI_U32 acc_pix_total)
{
    S_VDP_REGS_TYPE *pstVdpReg = HI_NULL;
    U_VHDACCTOTAL VHDACCTOTAL;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, SetAccAccPixTotal Select Wrong Layer ID\n");
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VHDACCTOTAL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTOTAL.u32)) + u32Data * PQ_VID_OFFSET));
    VHDACCTOTAL.bits.acc_pix_total = acc_pix_total;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVdpReg->VHDACCTOTAL.u32)) + u32Data * PQ_VID_OFFSET), VHDACCTOTAL.u32);

    return;
}

HI_S32 PQ_HAL_SetAccPara(HI_U32 u32Data, PQ_HAL_ACC_PARA_S stAccPara)
{
    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error, Select Wrong Layer ID\n");

        return HI_FAILURE;
    }

    PQ_HAL_SetAccAccEn(u32Data,   stAccPara.stAccCfg.acc_en);
    PQ_HAL_SetAccAccMode(u32Data, stAccPara.stAccCfg.acc_mode);
    PQ_HAL_SetAccDbgMode(u32Data, stAccPara.stAccCfg.acc_dbg_mode);
    PQ_HAL_SetAccDbgEn(u32Data,   stAccPara.stAccCfg.acc_dbg_en);
    PQ_HAL_SetAccAccRst(u32Data,  stAccPara.stAccCfg.acc_rst);

    PQ_HAL_SetAccThdMedLow(u32Data,   stAccPara.thd_med_low);
    PQ_HAL_SetAccThdHigh(u32Data,     stAccPara.thd_high);
    PQ_HAL_SetAccThdLow(u32Data,      stAccPara.thd_low);
    PQ_HAL_SetAccAccMultiple(u32Data, stAccPara.acc_multiple);
    PQ_HAL_SetAccThdMedHigh(u32Data,  stAccPara.thd_med_high);

    //PQ_HAL_SetAccCnt3Low(u32Data,     stAccPara.cnt3_low);
    //PQ_HAL_SetAccCnt3Med(u32Data,     stAccPara.cnt3_med);
    //PQ_HAL_SetAccCnt3High(u32Data,    stAccPara.cnt3_high);
    //PQ_HAL_SetAccCnt8MedLow(u32Data,  stAccPara.cnt8_med_low);
    //PQ_HAL_SetAccCnt8MedHigh(u32Data, stAccPara.cnt8_med_high);
    //PQ_HAL_SetAccAccPixTotal(u32Data, stAccPara.acc_pix_total);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetAccTab(HI_U32 u32Data, HI_U32 *upTable)
{
    S_VDP_REGS_TYPE *pstVdpReg  = HI_NULL;

    U_VHDACCLOWN  VHDACCLOW[3];
    U_VHDACCMEDN  VHDACCMED[3];
    U_VHDACCHIGHN VHDACCHIGH[3];
    U_VHDACCMLN   VHDACCML[3];
    U_VHDACCMHN   VHDACCMH[3];

    HI_U32 i = 0;

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error! SetAccTab() Select Wrong Layer ID!\n");
        return HI_FAILURE;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();

    for (i = 0; i < 3; i++)
    {
        VHDACCLOW[i].u32 = 0;
        VHDACCLOW[i].bits.table_data1n = upTable[0 + i * 3];
        VHDACCLOW[i].bits.table_data2n = upTable[1 + i * 3];
        VHDACCLOW[i].bits.table_data3n = upTable[2 + i * 3];
        PQ_HAL_RegWrite((HI_U32 *)(HI_ULONG)(&(pstVdpReg->VHDACCLOWN[i].u32)), VHDACCLOW[i].u32);

        VHDACCMED[i].u32 = 0;
        VHDACCMED[i].bits.table_data1n = upTable[9 + 0 + i * 3];
        VHDACCMED[i].bits.table_data2n = upTable[9 + 1 + i * 3];
        VHDACCMED[i].bits.table_data3n = upTable[9 + 2 + i * 3];
        PQ_HAL_RegWrite((HI_U32 *)(HI_ULONG)(&(pstVdpReg->VHDACCMEDN[i].u32)), VHDACCMED[i].u32);

        VHDACCHIGH[i].u32 = 0;
        VHDACCHIGH[i].bits.table_data1n = upTable[18 + 0 + i * 3];
        VHDACCHIGH[i].bits.table_data2n = upTable[18 + 1 + i * 3];
        VHDACCHIGH[i].bits.table_data3n = upTable[18 + 2 + i * 3];
        PQ_HAL_RegWrite((HI_U32 *)(HI_ULONG)(&(pstVdpReg->VHDACCHIGHN[i].u32)), VHDACCHIGH[i].u32);

        VHDACCML[i].u32 = 0;
        VHDACCML[i].bits.table_data1n = upTable[27 + 0 + i * 3];
        VHDACCML[i].bits.table_data2n = upTable[27 + 1 + i * 3];
        VHDACCML[i].bits.table_data3n = upTable[27 + 2 + i * 3];
        PQ_HAL_RegWrite((HI_U32 *)(HI_ULONG)(&(pstVdpReg->VHDACCMLN[i].u32)), VHDACCML[i].u32);

        VHDACCMH[i].u32 = 0;
        VHDACCMH[i].bits.table_data1n = upTable[36 + 0 + i * 3];
        VHDACCMH[i].bits.table_data2n = upTable[36 + 1 + i * 3];
        VHDACCMH[i].bits.table_data3n = upTable[36 + 2 + i * 3];
        PQ_HAL_RegWrite((HI_U32 *)(HI_ULONG)(&(pstVdpReg->VHDACCMHN[i].u32)), VHDACCMH[i].u32);
    }

    return HI_SUCCESS;
}


