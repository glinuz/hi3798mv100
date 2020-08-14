/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************/

/*-----------------------------------------------------------------------------*/
/* Include Header Files                                                        */
/*-----------------------------------------------------------------------------*/
#include <linux/string.h>      /* memcpy() */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "pq_hal_comm.h"
#include "pq_hal_acm.h"


#define ACM_BUFFER    sizeof(ACM_CONVERT_TABLE) /* Mmz Buffer Size */
#define ACM_LUT_SIZE  sizeof(COLOR_ACM_LUT_S)   /* Lut Size */


static HI_BOOL sg_bACMEnableFlag     = HI_TRUE;
static COLOR_ACM_LUT_S* sg_pstAcmLut = HI_NULL;
static MMZ_BUFFER_S     sg_stACMBuffer;


/* DeInit ACM */
HI_S32 PQ_HAL_DeInitACM(HI_VOID)
{
    sg_bACMEnableFlag = HI_FALSE;
    HI_KFREE(HI_ID_PQ, sg_pstAcmLut);
    HI_DRV_MMZ_UnmapAndRelease(&sg_stACMBuffer);

    return HI_SUCCESS;
}

/* Init ACM */
HI_S32 PQ_HAL_InitACM(HI_VOID)
{
    HI_S32 s32Ret;

    /* MMZ buffer */
    memset(&sg_stACMBuffer, 0, sizeof(MMZ_BUFFER_S));

    s32Ret = HI_DRV_MMZ_AllocAndMap("PQ_ACM", HI_NULL, ACM_BUFFER, 0, &sg_stACMBuffer);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_ACM memory allocated failed!\n");
        return HI_FAILURE;
    }
    memset((HI_U8*)sg_stACMBuffer.pu8StartVirAddr, 0, ACM_BUFFER);

    /* resource alloc */
    sg_pstAcmLut = (void*)HI_KMALLOC(HI_ID_PQ, ACM_LUT_SIZE, GFP_KERNEL);
    if (HI_NULL == sg_pstAcmLut)
    {
        HI_ERR_PQ("sg_pstAcmLut can not malloc!\n");
        HI_DRV_MMZ_UnmapAndRelease(&sg_stACMBuffer);
        return HI_FAILURE;
    }

    memset(sg_pstAcmLut, 0, ACM_LUT_SIZE);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_SetACMAddr(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32CoefAddr)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CAD VP0_ACM_CAD;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CAD.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CAD.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CAD.bits.coef_addr = u32CoefAddr;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CAD.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CAD.u32);

    return;
}

static HI_S32 PQ_HAL_Split_Table(HI_S16 ps16OriLUT[9][13][29], ACM_SPLIT_TABLE* pSplitTbl)
{
    HI_S32* apLUT[8];
    HI_S32 i, j, k;
    HI_S32 s32TblIndex;

    if (HI_NULL == pSplitTbl)
    {
        HI_ERR_PQ("pSplitTbl is NULL pointer!\n");
        return HI_FAILURE;
    }

    apLUT[0] = pSplitTbl->LUT0;
    apLUT[1] = pSplitTbl->LUT1;
    apLUT[2] = pSplitTbl->LUT2;
    apLUT[3] = pSplitTbl->LUT3;
    apLUT[4] = pSplitTbl->LUT4;
    apLUT[5] = pSplitTbl->LUT5;
    apLUT[6] = pSplitTbl->LUT6;
    apLUT[7] = pSplitTbl->LUT7;

    for (k = 0; k < 29; k++)
    {
        for (j = 0; j < 13; j++)
        {
            for (i = 0; i < 9; i++)
            {
                s32TblIndex = (i & 1) + ((j & 1) << 1) + ((k & 1) << 2);
                *apLUT[s32TblIndex] = ps16OriLUT[i][j][k];
                apLUT[s32TblIndex]++;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable0(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT0_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT0_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT0[i] & 0x1FF) | ((pstYSplitTable->LUT0[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT0[i] & 0x1FF) | ((pstSSplitTable->LUT0[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT0[i] & 0x7F) | ((pstHSplitTable->LUT0[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT0_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT0[i] & 0x1FF) | ((pstYSplitTable->LUT0[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT0[i] & 0x1FF) | ((pstSSplitTable->LUT0[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT0[i] & 0x7F) | ((pstHSplitTable->LUT0[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT0_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT0_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT0_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT0_CONF[(i - 4) / 4].u32Data[3] = x4;

    }

    x1 = 0;
    x2 = 0;
    x3 = 0;
    x4 = 0;

    /* others */
    tmp = (pstYSplitTable->LUT0[LUT0_PIXEL_NUM - 1] & 0x1FF);
    x1  = tmp; /* word1: 0~17 (18bit) */

    tmp = (pstSSplitTable->LUT0[LUT0_PIXEL_NUM - 1] & 0x1FF);
    x1 |= tmp << 18; /* word1: 18~31 (14bits) */
    x2  = tmp >> 14; /* word2: 0~3 (4bits) */

    tmp = (pstHSplitTable->LUT0[LUT0_PIXEL_NUM - 1] & 0x7F);
    x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

    pstConverTable->LUT0_CONF[i / 4].u32Data[0] = x1;
    pstConverTable->LUT0_CONF[i / 4].u32Data[1] = x2;
    pstConverTable->LUT0_CONF[i / 4].u32Data[2] = 0;
    pstConverTable->LUT0_CONF[i / 4].u32Data[3] = 0;

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable1(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT1_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT1_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT1[i] & 0x1FF) | ((pstYSplitTable->LUT1[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT1[i] & 0x1FF) | ((pstSSplitTable->LUT1[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT1[i] & 0x7F) | ((pstHSplitTable->LUT1[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT1_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT1[i] & 0x1FF) | ((pstYSplitTable->LUT1[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT1[i] & 0x1FF) | ((pstSSplitTable->LUT1[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT1[i] & 0x7F) | ((pstHSplitTable->LUT1[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT1_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT1_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT1_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT1_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable2(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT2_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT2_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT2[i] & 0x1FF) | ((pstYSplitTable->LUT2[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT2[i] & 0x1FF) | ((pstSSplitTable->LUT2[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT2[i] & 0x7F) | ((pstHSplitTable->LUT2[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT2_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT2[i] & 0x1FF) | ((pstYSplitTable->LUT2[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT2[i] & 0x1FF) | ((pstSSplitTable->LUT2[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT2[i] & 0x7F) | ((pstHSplitTable->LUT2[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT2_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT2_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT2_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT2_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    /* others */
    tmp = (pstYSplitTable->LUT2[LUT2_PIXEL_NUM - 2] & 0x1FF) | ((pstYSplitTable->LUT2[LUT2_PIXEL_NUM - 1] & 0x1FF) << 9);
    x1  = tmp; /* word1: 0~17 (18bit) */

    tmp = (pstSSplitTable->LUT2[LUT2_PIXEL_NUM - 2] & 0x1FF) | ((pstSSplitTable->LUT2[LUT2_PIXEL_NUM - 1] & 0x1FF) << 9);
    x1 |= tmp << 18; /* word1: 18~31 (14bits) */
    x2  = tmp >> 14; /* word2: 0~3 (4bits) */

    tmp = (pstHSplitTable->LUT2[LUT2_PIXEL_NUM - 2] & 0x7F) | ((pstHSplitTable->LUT2[LUT2_PIXEL_NUM - 1] & 0x7F) << 7);
    x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

    pstConverTable->LUT2_CONF[i / 4].u32Data[0] = x1;
    pstConverTable->LUT2_CONF[i / 4].u32Data[1] = x2;

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable3(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT3_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT3_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT3[i] & 0x1FF) | ((pstYSplitTable->LUT3[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT3[i] & 0x1FF) | ((pstSSplitTable->LUT3[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT3[i] & 0x7F) | ((pstHSplitTable->LUT3[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT3_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT3[i] & 0x1FF) | ((pstYSplitTable->LUT3[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT3[i] & 0x1FF) | ((pstSSplitTable->LUT3[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT3[i] & 0x7F) | ((pstHSplitTable->LUT3[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT3_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT3_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT3_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT3_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable4(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT4_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT4_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT4[i] & 0x1FF) | ((pstYSplitTable->LUT4[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT4[i] & 0x1FF) | ((pstSSplitTable->LUT4[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT4[i] & 0x7F) | ((pstHSplitTable->LUT4[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT4_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT4[i] & 0x1FF) | ((pstYSplitTable->LUT4[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT4[i] & 0x1FF) | ((pstSSplitTable->LUT4[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT4[i] & 0x7F) | ((pstHSplitTable->LUT4[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT4_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT4_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT4_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT4_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    /* others */
    tmp = (pstYSplitTable->LUT4[LUT4_PIXEL_NUM - 2] & 0x1FF) | ((pstYSplitTable->LUT4[LUT4_PIXEL_NUM - 1] & 0x1FF) << 9);
    x1  = tmp; /* word1: 0~17 (18bit) */

    tmp = (pstSSplitTable->LUT4[LUT4_PIXEL_NUM - 2] & 0x1FF) | ((pstSSplitTable->LUT4[LUT4_PIXEL_NUM - 1] & 0x1FF) << 9);
    x1 |= tmp << 18; /* word1: 18~31 (14bits) */
    x2  = tmp >> 14; /* word2: 0~3 (4bits) */

    tmp = (pstHSplitTable->LUT4[LUT4_PIXEL_NUM - 2] & 0x7F) | ((pstHSplitTable->LUT4[LUT4_PIXEL_NUM - 1] & 0x7F) << 7);
    x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

    pstConverTable->LUT4_CONF[i / 4].u32Data[0] = x1;
    pstConverTable->LUT4_CONF[i / 4].u32Data[1] = x2;

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable5(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT5_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT5_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT5[i] & 0x1FF) | ((pstYSplitTable->LUT5[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT5[i] & 0x1FF) | ((pstSSplitTable->LUT5[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT5[i] & 0x7F) | ((pstHSplitTable->LUT5[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT5_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT5[i] & 0x1FF) | ((pstYSplitTable->LUT5[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT5[i] & 0x1FF) | ((pstSSplitTable->LUT5[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT5[i] & 0x7F) | ((pstHSplitTable->LUT5[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT5_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT5_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT5_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT5_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable6(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT6_PIXEL_NUM / 4 * 4;)
    {

        if (i >= LUT6_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT6[i] & 0x1FF) | ((pstYSplitTable->LUT6[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT6[i] & 0x1FF) | ((pstSSplitTable->LUT6[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT6[i] & 0x7F) | ((pstHSplitTable->LUT6[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT6_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT6[i] & 0x1FF) | ((pstYSplitTable->LUT6[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT6[i] & 0x1FF) | ((pstSSplitTable->LUT6[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT6[i] & 0x7F) | ((pstHSplitTable->LUT6[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT6_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT6_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT6_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT6_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_ConvertTable7(ACM_SPLIT_TABLE* pstYSplitTable, ACM_SPLIT_TABLE* pstHSplitTable,
                                   ACM_SPLIT_TABLE* pstSSplitTable, ACM_CONVERT_TABLE* pstConverTable)
{
    HI_U32 tmp = 0;
    HI_S32 i = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;

    /* convert talbe 0 */
    for (i = 0; i < LUT7_PIXEL_NUM / 4 * 4;)
    {
        if (i >= LUT7_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT7[i] & 0x1FF) | ((pstYSplitTable->LUT7[i + 1] & 0x1FF) << 9);
        x1  = tmp; /* word1: 0~17 (18bit) */

        tmp = (pstSSplitTable->LUT7[i] & 0x1FF) | ((pstSSplitTable->LUT7[i + 1] & 0x1FF) << 9);
        x1 |= tmp << 18; /* word1: 18~31 (14bits) */
        x2  = tmp >> 14; /* word2: 0~3 (4bits) */

        tmp = (pstHSplitTable->LUT7[i] & 0x7F) | ((pstHSplitTable->LUT7[i + 1] & 0x7F) << 7);
        x2 |= tmp << 4;  /* word2: 4~17 (14bits) */

        i += 2;
        if (i >= LUT7_PIXEL_NUM - 1)
        { break; }

        tmp = (pstYSplitTable->LUT7[i] & 0x1FF) | ((pstYSplitTable->LUT7[i + 1] & 0x1FF) << 9);
        x2 |= tmp << 18; /* word2: 18~31 (14bits) */
        x3  = tmp >> 14; /* word3: 0~3 (4bits) */

        tmp = (pstSSplitTable->LUT7[i] & 0x1FF) | ((pstSSplitTable->LUT7[i + 1] & 0x1FF) << 9);
        x3 |= tmp << 4;  /* word3: 4~21 (18bits) */

        tmp = (pstHSplitTable->LUT7[i] & 0x7F) | ((pstHSplitTable->LUT7[i + 1] & 0x7F) << 7);
        x3 |= tmp << 22; /* word3: 22~31 (10bits) */
        x4  = tmp >> 10; /* word4: 0~3 (4bits) */

        i += 2;

        pstConverTable->LUT7_CONF[(i - 4) / 4].u32Data[0] = x1;
        pstConverTable->LUT7_CONF[(i - 4) / 4].u32Data[1] = x2;
        pstConverTable->LUT7_CONF[(i - 4) / 4].u32Data[2] = x3;
        pstConverTable->LUT7_CONF[(i - 4) / 4].u32Data[3] = x4;
    }

    return HI_SUCCESS;
}

/* 设置ACM参数 */
HI_S32 PQ_HAL_SetACMParam(COLOR_ACM_LUT_S* pstColorData)
{
    ACM_SPLIT_TABLE*   pstSplitTable_Y = HI_NULL;
    ACM_SPLIT_TABLE*   pstSplitTable_H = HI_NULL;
    ACM_SPLIT_TABLE*   pstSplitTable_S = HI_NULL;
    ACM_CONVERT_TABLE* pstConvertTable = HI_NULL;

    PQ_CHECK_NULL_PTR(pstColorData);

    pstConvertTable = (void*)HI_KMALLOC(HI_ID_PQ, sizeof(ACM_CONVERT_TABLE), GFP_ATOMIC);
    if (HI_NULL == pstConvertTable)
    {
        HI_ERR_PQ("pstConvertTable is invaild!\n");
        return HI_FAILURE;
    }

    pstSplitTable_Y = (void*)HI_KMALLOC(HI_ID_PQ, sizeof(ACM_SPLIT_TABLE), GFP_ATOMIC);
    pstSplitTable_H = (void*)HI_KMALLOC(HI_ID_PQ, sizeof(ACM_SPLIT_TABLE), GFP_ATOMIC);
    pstSplitTable_S = (void*)HI_KMALLOC(HI_ID_PQ, sizeof(ACM_SPLIT_TABLE), GFP_ATOMIC);

    if (!(pstSplitTable_Y && pstSplitTable_H && pstSplitTable_S))
    {
        if (pstSplitTable_Y)
        {
            HI_KFREE(HI_ID_PQ, pstSplitTable_Y);
        }
        if (pstSplitTable_H)
        {
            HI_KFREE(HI_ID_PQ, pstSplitTable_H);
        }
        if (pstSplitTable_S)
        {
            HI_KFREE(HI_ID_PQ, pstSplitTable_S);
        }

        HI_KFREE(HI_ID_PQ, pstConvertTable);
        HI_ERR_PQ("SplitTable is invaild!\n");
        return HI_FAILURE;
    }

    memcpy(sg_pstAcmLut->as16Luma, pstColorData->as16Luma, sizeof(COLOR_LUT_S));
    memcpy(sg_pstAcmLut->as16Hue, pstColorData->as16Hue, sizeof(COLOR_LUT_S));
    memcpy(sg_pstAcmLut->as16Sat, pstColorData->as16Sat, sizeof(COLOR_LUT_S));
    /* Debug Start: For Print ACM LUT CURVE */
    /* Debug End: For Print ACM LUT CURVE */

    memset(pstConvertTable, 0x00, sizeof(ACM_CONVERT_TABLE));
    memset(pstSplitTable_Y, 0x00, sizeof(ACM_SPLIT_TABLE));
    memset(pstSplitTable_H, 0x00, sizeof(ACM_SPLIT_TABLE));
    memset(pstSplitTable_S, 0x00, sizeof(ACM_SPLIT_TABLE));
    /* split table */
    PQ_HAL_Split_Table(pstColorData->as16Luma, pstSplitTable_Y);
    PQ_HAL_Split_Table(pstColorData->as16Hue,  pstSplitTable_H);
    PQ_HAL_Split_Table(pstColorData->as16Sat,  pstSplitTable_S);
    /* convert */
    PQ_HAL_ConvertTable0(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable1(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable2(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable3(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable4(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable5(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable6(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);
    PQ_HAL_ConvertTable7(pstSplitTable_Y, pstSplitTable_H, pstSplitTable_S, pstConvertTable);

    memcpy(sg_stACMBuffer.pu8StartVirAddr, pstConvertTable, sizeof(ACM_CONVERT_TABLE));

    PQ_HAL_SetACMAddr(VDP_LAYER_VP0, sg_stACMBuffer.u32StartPhyAddr);
    PQ_HAL_UpdatACMCoef(VDP_LAYER_VP0);

    HI_KFREE(HI_ID_PQ, pstConvertTable);
    HI_KFREE(HI_ID_PQ, pstSplitTable_Y);
    HI_KFREE(HI_ID_PQ, pstSplitTable_H);
    HI_KFREE(HI_ID_PQ, pstSplitTable_S);

    return HI_SUCCESS;
}

/* Set ACM Enable */
HI_VOID PQ_HAL_EnableACM(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_en = bOnOff;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    sg_bACMEnableFlag = bOnOff;

    return;
}

/* 获取ACM开关标志 */
HI_S32 PQ_HAL_GetACMEnableFlag(HI_BOOL* bOnOff)
{
    *bOnOff = sg_bACMEnableFlag;

    return HI_SUCCESS;
}

/* 卖场模式选择 */
HI_VOID PQ_HAL_SetACMDemoMode(PQ_HAL_LAYER_VP_E u32ChId, ACM_DEMO_MODE_E enMode)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_dbg_mode = enMode;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

/* Enable Acm Demo */
HI_VOID PQ_HAL_EnableACMDemo(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_dbg_en = bOnOff;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_SetACMCbcrThd(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_cbcrthr = u32Data;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_SetACMStretch(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_stretch = u32Data;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_SetACMClipRange(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_cliprange = u32Data;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_SetACMGain(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Luma, HI_U32 u32Hue, HI_U32 u32Sat)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_ADJ.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_ADJ.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_ADJ.bits.acm_gain0 = u32Luma;
    VP0_ACM_ADJ.bits.acm_gain1 = u32Hue;
    VP0_ACM_ADJ.bits.acm_gain2 = u32Sat;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_ADJ.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_ADJ.u32);

    return;
}

HI_VOID PQ_HAL_GetACMGain(PQ_HAL_LAYER_VP_E u32ChId, HI_U32* pu32Luma, HI_U32* pu32Hue, HI_U32* pu32Sat)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_ADJ.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_ADJ.u32) + u32ChId * VDP_VP_OFFSET);
    *pu32Luma = VP0_ACM_ADJ.bits.acm_gain0;
    *pu32Hue  = VP0_ACM_ADJ.bits.acm_gain1;
    *pu32Sat  = VP0_ACM_ADJ.bits.acm_gain2;

    return;
}

HI_VOID PQ_HAL_UpdatACMCoef(PQ_HAL_LAYER_VP_E u32ChId)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_PARAUP   VP0_PARAUP;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_PARAUP.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_PARAUP.bits.vp0_acmcoef_upd = 1;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_PARAUP.u32) + u32ChId * VDP_VP_OFFSET), VP0_PARAUP.u32);

    return;
}

HI_S32 PQ_HAL_GetACMLumaTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(pstColorData, sg_pstAcmLut->as16Luma, sizeof(COLOR_LUT_S));
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetACMHueTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(pstColorData, sg_pstAcmLut->as16Hue, sizeof(COLOR_LUT_S));
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetACMSatTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(pstColorData, sg_pstAcmLut->as16Sat, sizeof(COLOR_LUT_S));
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetACMLumaTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(sg_pstAcmLut->as16Luma, pstColorData, sizeof(COLOR_LUT_S));
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetACMHueTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(sg_pstAcmLut->as16Hue, pstColorData, sizeof(COLOR_LUT_S));
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetACMSatTbl(COLOR_LUT_S* pstColorData)
{
    if (HI_NULL == pstColorData)
    {
        HI_ERR_PQ("pstColorData is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(sg_pstAcmLut->as16Sat, pstColorData, sizeof(COLOR_LUT_S));
    return PQ_HAL_SetACMParam(sg_pstAcmLut);
}

HI_VOID PQ_HAL_SetACMDemoPos(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Pos)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_VP0_ACM_CTRL   VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_dbg_pos = u32Pos;
    PQ_HAL_RegWrite(((HI_U32) & (pstVdpReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_GetACMDemoPos(PQ_HAL_LAYER_VP_E u32ChId, HI_U32* u32Pos)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_VP0_ACM_CTRL   VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    *u32Pos = VP0_ACM_CTRL.bits.acm_dbg_pos;

    return;
}
#if 0
/* 获取ACM参数 */
static HI_S32 PQ_HAL_GetACMParam(COLOR_ACM_LUT_S* pstColorTable)
{
    if (HI_NULL == sg_pstAcmLut)
    {
        HI_ERR_PQ("sg_pstAcmLut is NULL!\n");
        return HI_FAILURE;
    }

    memcpy(pstColorTable, sg_pstAcmLut, sizeof(COLOR_ACM_LUT_S));

    return HI_SUCCESS;
}

static HI_VOID PQ_HAL_SetACMCliporWrap(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if (u32ChId >= VDP_VP_MAX)
    {
        HI_ERR_PQ("Error,Select Wrong CHANNEL ID\n");
        return;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    VP0_ACM_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET);
    VP0_ACM_CTRL.bits.acm_cliporwrap = u32Data;
    PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_ACM_CTRL.u32) + u32ChId * VDP_VP_OFFSET), VP0_ACM_CTRL.u32);

    return;
}
#endif


