/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_zme.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Author        : p00203646
******************************************************************************/

#include "pq_hal_zme.h"
#include "pq_mng_zme_coef.h"

static HI_BOOL g_bVpssZmeDefault = HI_FALSE;
static HI_BOOL g_bVdpZmeDefault  = HI_FALSE;

/* SR ZME COEF */
/* Luma-Hor-8T2P */
static HI_S16 s16SRZmeCoef_8T2P_Lanc3[2][8] = {
    {4, -22,  40, 468,  40, -22,  4, 0},
    {0,  14, -68, 310, 310, -68, 14, 0},
};
/* 4.667M */
static HI_S16 s16SRZmeCoef_6T32P_6M_a25[2][6] = {
    { -34,  78, 415,  78, -34, 9},
    {   2, -42, 296, 296, -42, 2},
};

static S_VDP_REGS_TYPE* pVdpReg;

HI_S32 PQ_HAL_SetZmeDefault(HI_BOOL bOnOff)
{
    g_bVpssZmeDefault = bOnOff;
    g_bVdpZmeDefault  = bOnOff;

    return HI_SUCCESS;
}


/****************************Load VDP ZME COEF START*****************************************/
static HI_U32 VdpZmeTransCoefAlign(const HI_S16* ps16Coef, VZME_TAP_E enTap, ZME_COEF_BITARRAY_S* pBitCoef)
{
    HI_U32 i, u32Cnt, u32Tap, u32Tmp;

    switch (enTap)
    {
        case VZME_TAP_8T32P:
            u32Tap = 8;
            break;
        case VZME_TAP_6T32P:
            u32Tap = 6;
            break;
        case VZME_TAP_4T32P:
            u32Tap = 4;
            break;
        case VZME_TAP_2T32P:
            u32Tap = 2;
            break;
        default:
            u32Tap = 4;
            break;
    }

    /* Tran ZME coef to 128 bits order */
    u32Cnt = 18 * u32Tap / 12;
    for (i = 0; i < u32Cnt; i++)
    {
        pBitCoef->stBit[i].bits_0 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_1 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_2 = VouBitValue(*ps16Coef++);

        u32Tmp = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_32 = u32Tmp;
        pBitCoef->stBit[i].bits_38 = (u32Tmp >> 2);

        pBitCoef->stBit[i].bits_4 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_5 = VouBitValue(*ps16Coef++);

        u32Tmp = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_64 = u32Tmp;
        pBitCoef->stBit[i].bits_66 = u32Tmp >> 4;

        pBitCoef->stBit[i].bits_7 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_8 = VouBitValue(*ps16Coef++);

        u32Tmp = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_96 = u32Tmp;
        pBitCoef->stBit[i].bits_94 = u32Tmp >> 6;

        pBitCoef->stBit[i].bits_10 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_11 = VouBitValue(*ps16Coef++);
        pBitCoef->stBit[i].bits_12 = 0;
    }

    pBitCoef->u32Size = u32Cnt * sizeof(ZME_COEF_BIT_S);

    return HI_SUCCESS;
}

/* load hor coef */
static HI_U32 VdpZmeLoadCoefH(VZME_COEF_RATIO_E enCoefRatio, VZME_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U8* pu8Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    /* load horizontal zoom coef */
    VdpZmeTransCoefAlign(g_pVZmeCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);
    memcpy(pu8Addr, stArray.stBit, stArray.u32Size);

    return HI_SUCCESS;
}

/* load vert coef */
static HI_S32 VdpZmeLoadCoefV(VZME_COEF_RATIO_E enCoefRatio, VZME_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U8* pu8Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    /* load vertical zoom coef */
    VdpZmeTransCoefAlign(g_pVZmeCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);
    memcpy(pu8Addr, stArray.stBit, stArray.u32Size);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_VdpLoadCoef(ALG_VZME_MEM_S* pstVZmeCoefMem)
{
    HI_U8* pu8CurAddr;
    HI_U32 u32PhyAddr;
    HI_U32 u32NumSizeLuma, u32NumSizeChro;
    ALG_VZME_COEF_ADDR_S* pstAddrTmp;

    pu8CurAddr = (HI_U8*)(pstVZmeCoefMem->stMBuf.u32StartVirAddr);
    u32PhyAddr = pstVZmeCoefMem->stMBuf.u32StartPhyAddr;
    pstAddrTmp = &(pstVZmeCoefMem->stZmeCoefAddr);


    /* H_L8C4:Luma-Hor-8T32P Chroma-Hor-4T32P */
    u32NumSizeLuma = 192; /* (8x18) / 12 * 16 = 192 */
    u32NumSizeChro = 96;  /* (4x18) / 12 * 16 = 96 */
    VdpZmeLoadCoefH(VZME_COEF_1, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_1, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_E1, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_E1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_E1, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_075, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_075 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_075, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_05, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_05 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_05, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_033, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_033 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_033, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_025, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_025 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_025, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_0, VZME_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_0 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrHL8C4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_0, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* V_L6C4:Luma-Vert-6T32P Chroma-Vert-4T32P */
    u32NumSizeLuma = 144;  /* (6x18) / 12 * 16 = 144 */
    u32NumSizeChro = 96;   /* (4x18) / 12 * 16 = 96 */
    VdpZmeLoadCoefV(VZME_COEF_1, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_1, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_E1, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_E1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_E1, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    VdpZmeLoadCoefV(VZME_COEF_075, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_075 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_075, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_05, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_05 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_05, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_033, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_033 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_033, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_025, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_025 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_033, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_0, VZME_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_0 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL6C4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(VZME_COEF_0, VZME_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* V_L4C4:Luma-Vert-4T32P Chroma-Vert-4T32P */
    u32NumSizeLuma = 96; /*(4x18) / 12 * 16 */
    u32NumSizeChro = 96; /*(4x18) / 12 * 16 */
    VdpZmeLoadCoefV(VZME_COEF_1, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_1, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_E1, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_E1 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_E1, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_075, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_075 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_075, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_05, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_05 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_05, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_033, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_033 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_033, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_025, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_025 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_025, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_0, VZME_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_0 = u32PhyAddr;
    pstAddrTmp->u32ZmeCoefAddrVL4C4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(VZME_COEF_0, VZME_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* Chroma-Hor-8T32P */
    u32NumSizeChro = 192; /* (8x18) / 12 * 16 = 192 */
    VdpZmeLoadCoefH(VZME_COEF_1, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_E1, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_075, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_05, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_033, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_025, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(VZME_COEF_0, VZME_COEF_8T32P_CH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* Chroma-Vert-6T32P */
    u32NumSizeChro = 144; /* (6x18) / 12 * 16 = 144 */
    VdpZmeLoadCoefV(VZME_COEF_1, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_E1, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_075, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_05, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_033, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_025, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(VZME_COEF_0, VZME_COEF_6T32P_CV,  VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    return;
}
/*****************************Load VDP ZME COEF END*****************************************/

/*******************************VDP V0 ZME START***************************************/
static HI_VOID PQ_HAL_VID_SetZmeHorLumEn(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hlmsc_en  = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeVerLumEn(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.vlmsc_en  = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    /* logic limit: vlmsc_en in V1 and V4 ==0 */
    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + 1 * VID_OFFSET);
    V0_VSP.bits.vlmsc_en  = 0;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + 1 * VID_OFFSET), V0_VSP.u32);

    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + 4 * VID_OFFSET);
    V0_VSP.bits.vlmsc_en  = 0;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + 4 * VID_OFFSET), V0_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeHorChmEn(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hchmsc_en = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeVerChmEn(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.vchmsc_en = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmePhaseH(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_HLOFFSET V0_HLOFFSET;
    U_V0_HCOFFSET V0_HCOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return;
    }
    /* VHD layer zoom enable */
    V0_HLOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET);
    V0_HLOFFSET.bits.hor_loffset = s32PhaseL;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET), V0_HLOFFSET.u32);

    V0_HCOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET);
    V0_HCOFFSET.bits.hor_coffset = s32PhaseC;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET), V0_HCOFFSET.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmePhaseV(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_VOFFSET V0_VOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return;
    }

    V0_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET);
    V0_VOFFSET.bits.vluma_offset   = s32PhaseL;
    V0_VOFFSET.bits.vchroma_offset = s32PhaseC;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmePhaseVB(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_VBOFFSET V0_VBOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return;
    }

    V0_VBOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET);
    V0_VBOFFSET.bits.vbluma_offset   = s32PhaseL;
    V0_VBOFFSET.bits.vbchroma_offset = s32PhaseC;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeFirEnable2(HI_U32 u32Data, HI_U32 u32bEnableHl, HI_U32 u32bEnableHc, HI_U32 u32bEnableVl, HI_U32 u32bEnableVc)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return;
    }

    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hlfir_en  = u32bEnableHl;
    V0_HSP.bits.hchfir_en = u32bEnableHc;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.vlfir_en  = u32bEnableVl;
    V0_VSP.bits.vchfir_en = u32bEnableVc;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeMidEnable2(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return;
    }
    /* VHD layer zoom enable */
    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hlmid_en = u32bEnable;
    V0_HSP.bits.hchmid_en = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.vlmid_en  = u32bEnable;
    V0_VSP.bits.vchmid_en = u32bEnable;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeVchTap(HI_U32 u32Data, HI_U32 u32VscTap)
{
    /* in 3798cv100,   you can't set vsc_chroma_tap, so this func should fall into chip dir */
    /* in 3798mv100_a, you can't set vsc_chroma_tap, so this func should fall into chip dir */
    /* in 3798mv100,   you can't set vsc_chroma_tap, so this func should fall into chip dir */
    /* in HifoneB2,    you can't set vsc_chroma_tap, so this func should fall into chip dir */
    return;
}

static HI_VOID PQ_HAL_VID_SetZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_HSP V0_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return;
    }

    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hratio = u32Ratio;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_VSR V0_VSR;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return;
    }

    V0_VSR.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET);
    V0_VSR.bits.vratio = u32Ratio;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET), V0_VSR.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_V0_HSP V0_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return;
    }

    V0_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET);
    V0_HSP.bits.hfir_order = u32HfirOrder;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32LAddr, HI_U32 u32CAddr)
{
    U_V0_HLCOEFAD V0_HLCOEFAD;
    U_V0_HCCOEFAD V0_HCCOEFAD;
    U_V0_VLCOEFAD V0_VLCOEFAD;
    U_V0_VCCOEFAD V0_VCCOEFAD;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return;
    }

    if (u32Mode == VDP_VID_PARA_ZME_HOR)
    {
        V0_HLCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET);
        V0_HLCOEFAD.bits.coef_addr = u32LAddr;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET), V0_HLCOEFAD.u32);

        V0_HCCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET);
        V0_HCCOEFAD.bits.coef_addr = u32CAddr;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET), V0_HCCOEFAD.u32);
    }
    else if (u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET);
        V0_VLCOEFAD.bits.coef_addr = u32LAddr;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET), V0_VLCOEFAD.u32);

        V0_VCCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET);
        V0_VCCOEFAD.bits.coef_addr = u32CAddr;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET), V0_VCCOEFAD.u32);
    }
    else
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return;
}

static HI_VOID PQ_HAL_VID_SetParaUpd(HI_U32 u32Data, VDP_VID_PARA_E enMode)
{
    U_V0_PARAUP V0_PARAUP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("error,PQ_HAL_VID_SetParaUpd() select wrong video layer id\n");
        return;
    }

    V0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET);
    if (enMode == VDP_VID_PARA_ZME_HOR)
    {

        V0_PARAUP.bits.v0_hlcoef_upd = 0x1;
        V0_PARAUP.bits.v0_hccoef_upd = 0x1;
    }
    else if (enMode == VDP_VID_PARA_ZME_VER)
    {
        V0_PARAUP.bits.v0_vlcoef_upd = 0x1;
        V0_PARAUP.bits.v0_vccoef_upd = 0x1;
    }
    else
    {
        HI_ERR_PQ("error,PQ_HAL_VID_SetParaUpd() select wrong mode!\n");
    }
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET), V0_PARAUP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeInFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return;
    }

    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.zme_in_fmt = u32Fmt;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_VID_SetZmeOutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= VID_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_VID_SetZmeOutFmt() Select Wrong Video Layer ID\n");
        return;
    }

    V0_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET);
    V0_VSP.bits.zme_out_fmt = u32Fmt;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return;
}

/* Vdp Cfg Set to reg*/
HI_VOID PQ_HAL_VdpZmeRegCfg(HI_U32 u32LayerId, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL bFirEnable)
{
    PQ_HAL_VID_SetZmeHorRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioHL);
    PQ_HAL_VID_SetZmeVerRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioVL);

    PQ_HAL_VID_SetZmePhaseH(u32LayerId, pstZmeRtlPara->s32ZmeOffsetHL, pstZmeRtlPara->s32ZmeOffsetHC);
    PQ_HAL_VID_SetZmePhaseV(u32LayerId, pstZmeRtlPara->s32ZmeOffsetVL, pstZmeRtlPara->s32ZmeOffsetVC);
    PQ_HAL_VID_SetZmePhaseVB(u32LayerId, pstZmeRtlPara->s32ZmeOffsetVLBtm, pstZmeRtlPara->s32ZmeOffsetVCBtm);

    PQ_HAL_VID_SetZmeMidEnable2(u32LayerId, pstZmeRtlPara->bZmeMedHL);
    PQ_HAL_VID_SetZmeHfirOrder(u32LayerId, pstZmeRtlPara->bZmeOrder);

    /* in 3798mv100_a, 3798mv100 and hifoneB2, it does not exist vsc_chroma_tap and vsc_luma_tap */
    PQ_HAL_VID_SetZmeVchTap(u32LayerId, pstZmeRtlPara->bZmeTapVC);

    if (pstZmeRtlPara->bZmeMdHL || pstZmeRtlPara->bZmeMdHC)
    {
        PQ_HAL_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_HOR, pstZmeRtlPara->u32ZmeCoefAddrHL, pstZmeRtlPara->u32ZmeCoefAddrHC);
        PQ_HAL_VID_SetParaUpd(u32LayerId, VDP_VID_PARA_ZME_HOR);
    }

    if (pstZmeRtlPara->bZmeMdVL || pstZmeRtlPara->bZmeMdVC)
    {
        PQ_HAL_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_VER, pstZmeRtlPara->u32ZmeCoefAddrVL, pstZmeRtlPara->u32ZmeCoefAddrVC);
        PQ_HAL_VID_SetParaUpd(u32LayerId, VDP_VID_PARA_ZME_VER);
    }

/* debug start z00128410 2012-1-30 debug config for SDK debug: fixed to copy mode that don't need zme coefficient */
    /*
    pstZmeRtlPara->bZmeMdHL = 1;
    pstZmeRtlPara->bZmeMdHC = 1;
    pstZmeRtlPara->bZmeMdVL = 1;
    pstZmeRtlPara->bZmeMdVC = 1;
    */
/* debug end z00128410 2012-1-30 */

    if (!bFirEnable)
    {   /* for fidelity output */
        PQ_HAL_VID_SetZmeFirEnable2(u32LayerId,
                                    pstZmeRtlPara->bZmeMdHL,
                                    pstZmeRtlPara->bZmeMdHC,
                                    HI_FALSE,
                                    HI_FALSE);
    }
    else
    {   /* for normal output */
        PQ_HAL_VID_SetZmeFirEnable2(u32LayerId,
                                    pstZmeRtlPara->bZmeMdHL,
                                    pstZmeRtlPara->bZmeMdHC,
                                    pstZmeRtlPara->bZmeMdVL,
                                    pstZmeRtlPara->bZmeMdVC);
    }
    /* different with 98m, Vdp Trans FmtIn and FmtOut to PQ */
    PQ_HAL_VID_SetZmeInFmt(u32LayerId, pstZmeRtlPara->u8ZmeYCFmtIn);
    PQ_HAL_VID_SetZmeOutFmt(u32LayerId, pstZmeRtlPara->u8ZmeYCFmtOut);
    PQ_HAL_VID_SetZmeHorLumEn(u32LayerId, pstZmeRtlPara->bZmeEnVL);
    PQ_HAL_VID_SetZmeVerLumEn(u32LayerId, pstZmeRtlPara->bZmeEnVL);
    PQ_HAL_VID_SetZmeHorChmEn(u32LayerId, pstZmeRtlPara->bZmeEnVL);
    PQ_HAL_VID_SetZmeVerChmEn(u32LayerId, pstZmeRtlPara->bZmeEnVL);
    
    return;
}
/*******************************VDP V0 ZME END***************************************/


/*******************************WBC ZME START***************************************/
static HI_VOID PQ_HAL_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_WBC_DHD0_ZME_HLOFFSET WBC_DHD0_ZME_HLOFFSET;
    U_WBC_DHD0_ZME_HCOFFSET WBC_DHD0_ZME_HCOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 )
    {
        /* VHD layer zoom enable */
        WBC_DHD0_ZME_HLOFFSET.u32 = PQ_HAL_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32));
        WBC_DHD0_ZME_HLOFFSET.bits.hor_loffset = s32PhaseL;
        PQ_HAL_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32), WBC_DHD0_ZME_HLOFFSET.u32);

        WBC_DHD0_ZME_HCOFFSET.u32 = PQ_HAL_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32));
        WBC_DHD0_ZME_HCOFFSET.bits.hor_coffset = s32PhaseC;
        PQ_HAL_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32), WBC_DHD0_ZME_HCOFFSET.u32);
    }
    else if (enLayer == VDP_LAYER_WBC_GP0)
    {}

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmePhaseV(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_WBC_DHD0_ZME_VOFFSET WBC_DHD0_ZME_VOFFSET;
    pVdpReg = PQ_HAL_GetVdpReg();

    if (enLayer == VDP_LAYER_WBC_HD0 )
    {
        /* VHD layer zoom enable */
        WBC_DHD0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32));
        WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32PhaseL;
        PQ_HAL_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32), WBC_DHD0_ZME_VOFFSET.u32);

        WBC_DHD0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32));
        WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32PhaseC;
        PQ_HAL_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32), WBC_DHD0_ZME_VOFFSET.u32);
    }
    else if (enLayer == VDP_LAYER_WBC_GP0)
    {}

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeHorRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
        WBC_DHD0_ZME_HSP.bits.hratio = u32Ratio;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeVerRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    U_WBC_DHD0_ZME_VSR WBC_DHD0_ZME_VSR;

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSR.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSR.u32) + enLayer * WBC_OFFSET);
        WBC_DHD0_ZME_VSR.bits.vratio = u32Ratio;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSR.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSR.u32);
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable, HI_U32 u32firMode)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();
    /* WBC zoom enable */
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        if ((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hlfir_en = u32firMode;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hchfir_en = u32firMode;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        /* non_lnr_en does not exist in 3798m_a and 3798m, but exist in hifone; 
           but STB does not care non lnr zme,  reg "non_lnr_en" default value is 0
           when non_lnr_en == 0, DELTA and Zone is invalid
           */
#if 0
        if ((enMode == VDP_ZME_MODE_NONL) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.non_lnr_en = u32bEnable;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }
#endif
        if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vlfir_en = u32firMode;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vchfir_en = u32firMode;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetMidEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        if ((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_HSP.bits.hlmid_en = bEnable;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_HSP.bits.hchmid_en = bEnable;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_VSP.bits.vlmid_en = bEnable;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_ZME_VSP.bits.vchmid_en = bEnable;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeHfirOrder(VDP_LAYER_WBC_E enLayer, HI_U32 u32HfirOrder)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET);
        WBC_DHD0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeVerTap(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)
{
    /* vsc_luma_tap does not exist in hifone */
    /* vsc_chroma_tap does not exist in hifone */
    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr, HI_U32 u32CAddr)
{
    U_WBC_DHD0_HLCOEFAD WBC_DHD0_HLCOEFAD;
    U_WBC_DHD0_HCCOEFAD WBC_DHD0_HCCOEFAD;
    U_WBC_DHD0_VLCOEFAD WBC_DHD0_VLCOEFAD;
    U_WBC_DHD0_VCCOEFAD WBC_DHD0_VCCOEFAD;

    pVdpReg = PQ_HAL_GetVdpReg();

    if ( enLayer == VDP_LAYER_WBC_HD0 ||  enLayer == VDP_LAYER_WBC_ME  )
    {
        if (u32Mode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_DHD0_HLCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_HLCOEFAD.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_HLCOEFAD.bits.coef_addr = u32Addr;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_HLCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HLCOEFAD.u32);

            WBC_DHD0_HCCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_HCCOEFAD.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_HCCOEFAD.bits.coef_addr = u32CAddr;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_HCCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCCOEFAD.u32);
        }
        else if (u32Mode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_DHD0_VLCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_VLCOEFAD.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_VLCOEFAD.bits.coef_addr = u32Addr;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_VLCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_VLCOEFAD.u32);

            WBC_DHD0_VCCOEFAD.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_VCCOEFAD.u32) + enLayer * WBC_OFFSET);
            WBC_DHD0_VCCOEFAD.bits.coef_addr = u32CAddr;
            PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_VCCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_VCCOEFAD.u32);
        }
        else
        {
            HI_ERR_PQ("Error,PQ_HAL_WBC_SetZmeCoefAddr() Select a Wrong Mode!\n");
        }
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetParaUpd (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E enMode)
{
    U_WBC_DHD0_PARAUP WBC_DHD0_PARAUP;
    /* WBC_ME exist in 98cv100, using for MEMC, does not exist in 98mv100_a and 98m */

    pVdpReg = PQ_HAL_GetVdpReg();

    if ( enLayer == VDP_LAYER_WBC_HD0 )
    {
        WBC_DHD0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_PARAUP.u32));
        if (enMode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_DHD0_PARAUP.bits.wbc_hlcoef_upd = 0x1;
            WBC_DHD0_PARAUP.bits.wbc_hccoef_upd = 0x1;
        }
        else if (enMode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_DHD0_PARAUP.bits.wbc_vlcoef_upd = 0x1;
            WBC_DHD0_PARAUP.bits.wbc_vccoef_upd = 0x1;
        }
        else
        {
            HI_ERR_PQ("error,VDP_WBC_DHD0_SetParaUpd() select wrong mode!\n");
        }
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_PARAUP.u32)), WBC_DHD0_PARAUP.u32);
    }
    else
    {}

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeInFmt(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    pVdpReg = PQ_HAL_GetVdpReg();

    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET);
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = u32Fmt;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
    }
    else
    {}

    return;
}

/* Vdp WBC Cfg Set to reg*/
/* explanation: In WBC We just pay atention to WBC_HD0 */
HI_VOID PQ_HAL_WbcZmeRegCfg(HI_U32 u32LayerId, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL bFirEnable)
{
    if (u32LayerId >= VDP_ZME_LAYER_WBC0)
    {
        u32LayerId = u32LayerId - VDP_ZME_LAYER_WBC0;
    }

    PQ_HAL_WBC_SetZmeHorRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioHL);
    PQ_HAL_WBC_SetZmeVerRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioVL);

    PQ_HAL_WBC_SetZmePhaseH(u32LayerId,  pstZmeRtlPara->s32ZmeOffsetHL, pstZmeRtlPara->s32ZmeOffsetHC);
    PQ_HAL_WBC_SetZmePhaseV(u32LayerId,  pstZmeRtlPara->s32ZmeOffsetVL, pstZmeRtlPara->s32ZmeOffsetVC);

#if 1
    /* zme enable and  fir enable should not fix to a value. */
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORL, pstZmeRtlPara->bZmeEnHL, pstZmeRtlPara->bZmeMdHL);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORC, pstZmeRtlPara->bZmeEnHC, pstZmeRtlPara->bZmeMdHC);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERL, pstZmeRtlPara->bZmeEnVL, pstZmeRtlPara->bZmeMdVL);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeEnVC, pstZmeRtlPara->bZmeMdVC);
#else
    /* zme enable and  fir enable should not fix to a value.*/
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORL, 0, 0);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORC, 0, 0);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERL, 0, 0);
    PQ_HAL_WBC_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERC, 0, 0);
#endif
    /* set media fir. */
    PQ_HAL_WBC_SetMidEnable(VDP_LAYER_WBC_HD0, VDP_ZME_MODE_HORL, pstZmeRtlPara->bZmeMedHL);
    PQ_HAL_WBC_SetMidEnable(VDP_LAYER_WBC_HD0, VDP_ZME_MODE_HORC, pstZmeRtlPara->bZmeMedHC);
    PQ_HAL_WBC_SetMidEnable(VDP_LAYER_WBC_HD0, VDP_ZME_MODE_VERL, pstZmeRtlPara->bZmeMedVL);
    PQ_HAL_WBC_SetMidEnable(VDP_LAYER_WBC_HD0, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeMedVC);

    /* set zme order, h first or v first */
    PQ_HAL_WBC_SetZmeHfirOrder(VDP_LAYER_WBC_HD0, pstZmeRtlPara->bZmeOrder);

    /* set v chroma zme tap */
    PQ_HAL_WBC_SetZmeVerTap(VDP_LAYER_WBC_HD0, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeTapVC);

    /* set hor fir coef addr, and set updata flag */
    if (pstZmeRtlPara->u32ZmeRatioHL)
    {
        PQ_HAL_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_HD0, VDP_WBC_PARA_ZME_HOR, pstZmeRtlPara->u32ZmeCoefAddrHL, pstZmeRtlPara->u32ZmeCoefAddrHC);
        PQ_HAL_WBC_SetParaUpd(VDP_LAYER_WBC_HD0, VDP_WBC_PARA_ZME_HOR);
    }

    /* set ver fir coef addr, and set updata flag */
    if (pstZmeRtlPara->u32ZmeRatioVL)
    {
        PQ_HAL_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_HD0, VDP_WBC_PARA_ZME_VER, pstZmeRtlPara->u32ZmeCoefAddrVL, pstZmeRtlPara->u32ZmeCoefAddrVC);
        PQ_HAL_WBC_SetParaUpd(VDP_LAYER_WBC_HD0, VDP_WBC_PARA_ZME_VER);
    }

    PQ_HAL_WBC_SetZmeInFmt(VDP_LAYER_WBC_HD0, VDP_PROC_FMT_SP_422);

    return;
}

/*******************************WBC ZME END***************************************/


/*********************************SR ZME******************************************/

/*------------------------Load VDP SR ZME COEF START-----------------------------*/
static HI_VOID PQ_HAL_VdpSRZmeCoef(VDP_SR_ZME_COEF_S* pstVdpSrZmeCoef)
{
    HI_U32 i;
    HI_U32 u32SRHLSize, u32SRHCSize;
    HI_U32 u32SRVLSize, u32SRVCSize;

    u32SRHLSize = 8;
    u32SRHCSize = 8;
    u32SRVLSize = 6;
    u32SRVCSize = 6;

    /* Horizon */
    for (i = 0; i < u32SRHLSize; i++)
    {
        pstVdpSrZmeCoef->s16HLCoef0[i] = s16SRZmeCoef_8T2P_Lanc3[0][i];
    }
    for (i = 0; i < u32SRHLSize; i++)
    {
        pstVdpSrZmeCoef->s16HLCoef1[i] = s16SRZmeCoef_8T2P_Lanc3[1][i];
    }
    for (i = 0; i < u32SRHCSize; i++)
    {
        pstVdpSrZmeCoef->s16HCCoef0[i] = s16SRZmeCoef_8T2P_Lanc3[0][i];
    }
    for (i = 0; i < u32SRHCSize; i++)
    {
        pstVdpSrZmeCoef->s16HCCoef1[i] = s16SRZmeCoef_8T2P_Lanc3[1][i];
    }

    /* Vertical */
    for (i = 0; i < u32SRVLSize; i++)
    {
        pstVdpSrZmeCoef->s16VLCoef0[i] = s16SRZmeCoef_6T32P_6M_a25[0][i];
    }
    for (i = 0; i < u32SRVLSize; i++)
    {
        pstVdpSrZmeCoef->s16VLCoef1[i] = s16SRZmeCoef_6T32P_6M_a25[1][i];
    }
    for (i = 0; i < u32SRVCSize; i++)
    {
        pstVdpSrZmeCoef->s16VCCoef0[i] = s16SRZmeCoef_6T32P_6M_a25[0][i];
    }
    for (i = 0; i < u32SRVCSize; i++)
    {
        pstVdpSrZmeCoef->s16VCCoef1[i] = s16SRZmeCoef_6T32P_6M_a25[1][i];
    }

    return;
}
/*-------------------------Load VDP SR ZME COEF END-------------------------------------*/

static HI_VOID PQ_HAL_SR_SetZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_HSP SR_HSP;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
    SR_HSP.bits.hratio = u32Ratio;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_SR_SetZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_VSR SR_VSR;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SR_VSR.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSR.u32) + u32Data * VID_OFFSET);
    SR_VSR.bits.vratio = u32Ratio;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSR.u32) + u32Data * VID_OFFSET), SR_VSR.u32);

    return;
}

/* confine of using: u32firMode == 1 in SR */
static HI_VOID PQ_HAL_SR_SetZmeEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable, HI_U32 u32firMode)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmeEnable() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    /* VHD layer zoom enable */
    if ((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
        SR_HSP.bits.hlmsc_en = u32bEnable;
        SR_HSP.bits.hlfir_en = u32firMode;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);
    }
    if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
        SR_HSP.bits.hchmsc_en = u32bEnable;
        SR_HSP.bits.hchfir_en = u32firMode;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);
    }
    if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
        SR_VSP.bits.vlmsc_en = u32bEnable;
        SR_VSP.bits.vlfir_en = u32firMode;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);
    }
    if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
        SR_VSP.bits.vchmsc_en = u32bEnable;
        SR_VSP.bits.vchfir_en = u32firMode;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);
    }

    return;
}

/* vbluma and vbchoma offset */
static HI_VOID PQ_HAL_SR_SetZmePhase(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase)
{
    U_SR_HLOFFSET  SR_HLOFFSET;
    U_SR_HCOFFSET  SR_HCOFFSET;
    U_SR_VOFFSET   SR_VOFFSET;
    U_SR_VBOFFSET  SR_VBOFFSET;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmePhase() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    /* VHD layer zoom enable */
    if ((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HLOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HLOFFSET.u32) + u32Data * VID_OFFSET);
        SR_HLOFFSET.bits.hor_loffset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HLOFFSET.u32) + u32Data * VID_OFFSET), SR_HLOFFSET.u32);
    }

    if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HCOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOFFSET.u32) + u32Data * VID_OFFSET);
        SR_HCOFFSET.bits.hor_coffset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOFFSET.u32) + u32Data * VID_OFFSET), SR_HCOFFSET.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VOFFSET.u32) + u32Data * VID_OFFSET);
        SR_VOFFSET.bits.vluma_offset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VOFFSET.u32) + u32Data * VID_OFFSET), SR_VOFFSET.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VOFFSET.u32) + u32Data * VID_OFFSET);
        SR_VOFFSET.bits.vchroma_offset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VOFFSET.u32) + u32Data * VID_OFFSET), SR_VOFFSET.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERB) || (enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VBOFFSET.u32) + u32Data * VID_OFFSET);
        SR_VBOFFSET.bits.vbluma_offset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VBOFFSET.u32) + u32Data * VID_OFFSET), SR_VBOFFSET.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERB) || (enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VBOFFSET.u32) + u32Data * VID_OFFSET);
        SR_VBOFFSET.bits.vbchroma_offset = s32Phase;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VBOFFSET.u32) + u32Data * VID_OFFSET), SR_VBOFFSET.u32);
    }

    return;
}

static HI_VOID PQ_HAl_SR_SetZmeMidEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAl_SR_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    /* VHD layer zoom enable */
    if ((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
        SR_HSP.bits.hlmid_en = u32bEnable;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);
    }

    if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
        SR_HSP.bits.hchmid_en = u32bEnable;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
        SR_VSP.bits.vlmid_en = u32bEnable;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);
    }

    if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
        SR_VSP.bits.vchmid_en = u32bEnable;
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);
    }

    return;
}

static HI_VOID PQ_HAL_SR_SetZmeInFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
    SR_VSP.bits.zme_in_fmt = u32Fmt;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_SR_SetZmeOutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetZmeOutFmt() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SR_VSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET);
    SR_VSP.bits.zme_out_fmt = u32Fmt;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VSP.u32) + u32Data * VID_OFFSET), SR_VSP.u32);

    return;
}

static HI_VOID PQ_HAL_SR_SetHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_SR_HSP SR_HSP;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SR_SetHfirOrder() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    SR_HSP.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET);
    SR_HSP.bits.hfir_order = u32HfirOrder;
    PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HSP.u32) + u32Data * VID_OFFSET), SR_HSP.u32);

    return;
}

static HI_VOID PQ_HAL_SR_SetZmeVerTap(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)
{
    /* SR: in 3798cv100 and 3798mv100, it does not exist vsc_chroma_tap and vsc_luma_tap */
    return;
}

static HI_VOID PQ_HAL_SR_SetZmeCoef(HI_U32 u32Data, VDP_SR_ZME_MODE_E enMode, VDP_SR_ZME_COEF_S* pstVdpSrZmeCoef)
{
    U_SR_HCOEF0        SR_HCOEF0;
    U_SR_HCOEF1        SR_HCOEF1;
    U_SR_HCOEF2        SR_HCOEF2;
    U_SR_HCOEF3        SR_HCOEF3;
    U_SR_HCOEF4        SR_HCOEF4;
    U_SR_HCOEF5        SR_HCOEF5;
    U_SR_HCOEF6        SR_HCOEF6;
    U_SR_HCOEF7        SR_HCOEF7;
    U_SR_HCOEF8        SR_HCOEF8;
    U_SR_HCOEF9        SR_HCOEF9;
    U_SR_HCOEF10       SR_HCOEF10;
    U_SR_HCOEF11       SR_HCOEF11;

    U_SR_VCOEF0        SR_VCOEF0;
    U_SR_VCOEF1        SR_VCOEF1;
    U_SR_VCOEF2        SR_VCOEF2;
    U_SR_VCOEF3        SR_VCOEF3;
    U_SR_VCOEF4        SR_VCOEF4;
    U_SR_VCOEF5        SR_VCOEF5;
    U_SR_VCOEF6        SR_VCOEF6;
    U_SR_VCOEF7        SR_VCOEF7;

    if (u32Data >= VDP_CHN_MAX)
    {
        HI_ERR_PQ("Error,VDP_SR_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return;
    }

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enMode == VDP_SR_ZME_MODE_HOR)
    {
        SR_HCOEF0.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF0.u32) + u32Data * VID_OFFSET);
        SR_HCOEF0.bits.hlcoef00 = pstVdpSrZmeCoef->s16HLCoef0[0];
        SR_HCOEF0.bits.hlcoef01 = pstVdpSrZmeCoef->s16HLCoef0[1];
        SR_HCOEF0.bits.hlcoef02 = pstVdpSrZmeCoef->s16HLCoef0[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF0.u32) + u32Data * VID_OFFSET), SR_HCOEF0.u32);

        SR_HCOEF1.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF1.u32) + u32Data * VID_OFFSET);
        SR_HCOEF1.bits.hlcoef03 = pstVdpSrZmeCoef->s16HLCoef0[3];
        SR_HCOEF1.bits.hlcoef04 = pstVdpSrZmeCoef->s16HLCoef0[4];
        SR_HCOEF1.bits.hlcoef05 = pstVdpSrZmeCoef->s16HLCoef0[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF1.u32) + u32Data * VID_OFFSET), SR_HCOEF1.u32);

        SR_HCOEF2.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF2.u32) + u32Data * VID_OFFSET);
        SR_HCOEF2.bits.hlcoef06 = pstVdpSrZmeCoef->s16HLCoef0[6];
        SR_HCOEF2.bits.hlcoef07 = pstVdpSrZmeCoef->s16HLCoef0[7];
        SR_HCOEF2.bits.hlcoef10 = pstVdpSrZmeCoef->s16HLCoef1[0];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF2.u32) + u32Data * VID_OFFSET), SR_HCOEF2.u32);

        SR_HCOEF3.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF3.u32) + u32Data * VID_OFFSET);
        SR_HCOEF3.bits.hlcoef11 = pstVdpSrZmeCoef->s16HLCoef1[1];
        SR_HCOEF3.bits.hlcoef12 = pstVdpSrZmeCoef->s16HLCoef1[2];
        SR_HCOEF3.bits.hlcoef13 = pstVdpSrZmeCoef->s16HLCoef1[3];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF3.u32) + u32Data * VID_OFFSET), SR_HCOEF3.u32);

        SR_HCOEF4.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF4.u32) + u32Data * VID_OFFSET);
        SR_HCOEF4.bits.hlcoef14 = pstVdpSrZmeCoef->s16HLCoef1[4];
        SR_HCOEF4.bits.hlcoef15 = pstVdpSrZmeCoef->s16HLCoef1[5];
        SR_HCOEF4.bits.hlcoef16 = pstVdpSrZmeCoef->s16HLCoef1[6];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF4.u32) + u32Data * VID_OFFSET), SR_HCOEF4.u32);

        SR_HCOEF5.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF5.u32) + u32Data * VID_OFFSET);
        SR_HCOEF5.bits.hlcoef17 = pstVdpSrZmeCoef->s16HLCoef1[7];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF5.u32) + u32Data * VID_OFFSET), SR_HCOEF5.u32);

        SR_HCOEF6.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF6.u32) + u32Data * VID_OFFSET);
        SR_HCOEF6.bits.hccoef00 = pstVdpSrZmeCoef->s16HCCoef0[0];
        SR_HCOEF6.bits.hccoef01 = pstVdpSrZmeCoef->s16HCCoef0[1];
        SR_HCOEF6.bits.hccoef02 = pstVdpSrZmeCoef->s16HCCoef0[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF6.u32) + u32Data * VID_OFFSET), SR_HCOEF6.u32);

        SR_HCOEF7.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF7.u32) + u32Data * VID_OFFSET);
        SR_HCOEF7.bits.hccoef03 = pstVdpSrZmeCoef->s16HCCoef0[3];
        SR_HCOEF7.bits.hccoef04 = pstVdpSrZmeCoef->s16HCCoef0[4];
        SR_HCOEF7.bits.hccoef05 = pstVdpSrZmeCoef->s16HCCoef0[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF7.u32) + u32Data * VID_OFFSET), SR_HCOEF7.u32);

        SR_HCOEF8.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF8.u32) + u32Data * VID_OFFSET);
        SR_HCOEF8.bits.hccoef06 = pstVdpSrZmeCoef->s16HCCoef0[6];
        SR_HCOEF8.bits.hccoef07 = pstVdpSrZmeCoef->s16HCCoef0[7];
        SR_HCOEF8.bits.hccoef10 = pstVdpSrZmeCoef->s16HCCoef1[0];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF8.u32) + u32Data * VID_OFFSET), SR_HCOEF8.u32);

        SR_HCOEF9.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF9.u32) + u32Data * VID_OFFSET);
        SR_HCOEF9.bits.hccoef11 = pstVdpSrZmeCoef->s16HCCoef1[1];
        SR_HCOEF9.bits.hccoef12 = pstVdpSrZmeCoef->s16HCCoef1[2];
        SR_HCOEF9.bits.hccoef13 = pstVdpSrZmeCoef->s16HCCoef1[3];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF9.u32) + u32Data * VID_OFFSET), SR_HCOEF9.u32);

        SR_HCOEF10.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF10.u32) + u32Data * VID_OFFSET);
        SR_HCOEF10.bits.hccoef14 = pstVdpSrZmeCoef->s16HCCoef1[4];
        SR_HCOEF10.bits.hccoef15 = pstVdpSrZmeCoef->s16HCCoef1[5];
        SR_HCOEF10.bits.hccoef16 = pstVdpSrZmeCoef->s16HCCoef1[6];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF10.u32) + u32Data * VID_OFFSET), SR_HCOEF10.u32);

        SR_HCOEF11.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_HCOEF11.u32) + u32Data * VID_OFFSET);
        SR_HCOEF11.bits.hccoef17 = pstVdpSrZmeCoef->s16HCCoef1[7];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_HCOEF11.u32) + u32Data * VID_OFFSET), SR_HCOEF11.u32);
    }
    else if (enMode == VDP_SR_ZME_MODE_VER)
    {
        SR_VCOEF0.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF0.u32) + u32Data * VID_OFFSET);
        SR_VCOEF0.bits.vlcoef00 = pstVdpSrZmeCoef->s16VLCoef0[0];
        SR_VCOEF0.bits.vlcoef01 = pstVdpSrZmeCoef->s16VLCoef0[1];
        SR_VCOEF0.bits.vlcoef02 = pstVdpSrZmeCoef->s16VLCoef0[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF0.u32) + u32Data * VID_OFFSET), SR_VCOEF0.u32);

        SR_VCOEF1.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF1.u32) + u32Data * VID_OFFSET);
        SR_VCOEF1.bits.vlcoef03 = pstVdpSrZmeCoef->s16VLCoef0[3];
        SR_VCOEF1.bits.vlcoef04 = pstVdpSrZmeCoef->s16VLCoef0[4];
        SR_VCOEF1.bits.vlcoef05 = pstVdpSrZmeCoef->s16VLCoef0[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF1.u32) + u32Data * VID_OFFSET), SR_VCOEF1.u32);

        SR_VCOEF2.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF2.u32) + u32Data * VID_OFFSET);
        SR_VCOEF2.bits.vlcoef10 = pstVdpSrZmeCoef->s16VLCoef1[0];
        SR_VCOEF2.bits.vlcoef11 = pstVdpSrZmeCoef->s16VLCoef1[1];
        SR_VCOEF2.bits.vlcoef12 = pstVdpSrZmeCoef->s16VLCoef1[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF2.u32) + u32Data * VID_OFFSET), SR_VCOEF2.u32);

        SR_VCOEF3.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF3.u32) + u32Data * VID_OFFSET);
        SR_VCOEF3.bits.vlcoef13 = pstVdpSrZmeCoef->s16VLCoef1[3];
        SR_VCOEF3.bits.vlcoef14 = pstVdpSrZmeCoef->s16VLCoef1[4];
        SR_VCOEF3.bits.vlcoef15 = pstVdpSrZmeCoef->s16VLCoef1[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF3.u32) + u32Data * VID_OFFSET), SR_VCOEF3.u32);

        SR_VCOEF4.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF4.u32) + u32Data * VID_OFFSET);
        SR_VCOEF4.bits.vccoef00 = pstVdpSrZmeCoef->s16VCCoef0[0];
        SR_VCOEF4.bits.vccoef01 = pstVdpSrZmeCoef->s16VCCoef0[1];
        SR_VCOEF4.bits.vccoef02 = pstVdpSrZmeCoef->s16VCCoef0[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF4.u32) + u32Data * VID_OFFSET), SR_VCOEF4.u32);

        SR_VCOEF5.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF5.u32) + u32Data * VID_OFFSET);
        SR_VCOEF5.bits.vccoef03 = pstVdpSrZmeCoef->s16VCCoef0[3];
        SR_VCOEF5.bits.vccoef04 = pstVdpSrZmeCoef->s16VCCoef0[4];
        SR_VCOEF5.bits.vccoef05 = pstVdpSrZmeCoef->s16VCCoef0[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF5.u32) + u32Data * VID_OFFSET), SR_VCOEF5.u32);

        SR_VCOEF6.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF6.u32) + u32Data * VID_OFFSET);
        SR_VCOEF6.bits.vccoef10 = pstVdpSrZmeCoef->s16VCCoef1[0];
        SR_VCOEF6.bits.vccoef11 = pstVdpSrZmeCoef->s16VCCoef1[1];
        SR_VCOEF6.bits.vccoef12 = pstVdpSrZmeCoef->s16VCCoef1[2];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF6.u32) + u32Data * VID_OFFSET), SR_VCOEF6.u32);

        SR_VCOEF7.u32 = PQ_HAL_RegRead((HI_U32) & (pVdpReg->SR_VCOEF7.u32) + u32Data * VID_OFFSET);
        SR_VCOEF7.bits.vccoef13 = pstVdpSrZmeCoef->s16VCCoef1[3];
        SR_VCOEF7.bits.vccoef14 = pstVdpSrZmeCoef->s16VCCoef1[4];
        SR_VCOEF7.bits.vccoef15 = pstVdpSrZmeCoef->s16VCCoef1[5];
        PQ_HAL_RegWrite(((HI_U32) & (pVdpReg->SR_VCOEF7.u32) + u32Data * VID_OFFSET), SR_VCOEF7.u32);
    }
    else
    {
        HI_ERR_PQ("Error,VDP_SR_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return;
}

/* Vdp SR Cfg Set to reg */
HI_VOID PQ_HAL_SRZmeRegCfg(HI_U32 u32LayerId, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL bFirEnable)
{
    VDP_SR_ZME_COEF_S stVdpSrZmeCoef;
    
    if (u32LayerId >= VDP_ZME_LAYER_SR)
    {
        u32LayerId = u32LayerId - VDP_ZME_LAYER_SR;
    }

    PQ_HAL_SR_SetZmeHorRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioHL);
    PQ_HAL_SR_SetZmeVerRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioVL);

    PQ_HAL_SR_SetZmePhase(u32LayerId,  pstZmeRtlPara->s32ZmeOffsetHL, pstZmeRtlPara->s32ZmeOffsetHC);

    /* zme enable and fir enable should not fix to a value; fither mode ==1 */
    PQ_HAL_SR_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORL, pstZmeRtlPara->bZmeEnHL, 1);
    PQ_HAL_SR_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HORC, pstZmeRtlPara->bZmeEnHC, 1);
    PQ_HAL_SR_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERL, pstZmeRtlPara->bZmeEnVL, 1);
    PQ_HAL_SR_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeEnVC, 1);

    /* set media fir */
    PQ_HAl_SR_SetZmeMidEnable(u32LayerId, VDP_ZME_MODE_HORL, pstZmeRtlPara->bZmeMedHL);
    PQ_HAl_SR_SetZmeMidEnable(u32LayerId, VDP_ZME_MODE_HORC, pstZmeRtlPara->bZmeMedHC);
    PQ_HAl_SR_SetZmeMidEnable(u32LayerId, VDP_ZME_MODE_VERL, pstZmeRtlPara->bZmeMedVL);
    PQ_HAl_SR_SetZmeMidEnable(u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeMedVC);

    /* set zme order, h first or v first */
    PQ_HAL_SR_SetHfirOrder(u32LayerId, pstZmeRtlPara->bZmeOrder);
    /* set v chroma zme tap */
    PQ_HAL_SR_SetZmeVerTap(u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara->bZmeTapVC);
    /* Set zme coefficient Hor and Vert */
    PQ_HAL_VdpSRZmeCoef(&stVdpSrZmeCoef);
    /* set hor fir coef */
    PQ_HAL_SR_SetZmeCoef(u32LayerId, VDP_SR_ZME_MODE_HOR, &(stVdpSrZmeCoef));

    /* set ver fir coef */
    PQ_HAL_SR_SetZmeCoef(u32LayerId, VDP_SR_ZME_MODE_VER, &(stVdpSrZmeCoef));

    PQ_HAL_SR_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_444);
    PQ_HAL_SR_SetZmeOutFmt(u32LayerId, VDP_PROC_FMT_SP_444);

    return;
}

/**************************************SR ZME END******************************************/

/*******************************Load VPSS ZME COEF START***********************************/
static HI_U32 VpssZmeTransCoefAlign(const HI_S16* ps16Coef, VZME_TAP_E enTap, ZME_COEF_BITARRAY_S* pBitCoef)
{
    HI_U32 i, j, u32Tap, k;
    HI_S32 s32CoefTmp1, s32CoefTmp2, s32CoefTmp3;

    switch (enTap)
    {
        case VZME_TAP_8T32P:
            u32Tap = 8;
            break;
        case VZME_TAP_6T32P:
            u32Tap = 6;
            break;
        case VZME_TAP_4T32P:
            u32Tap = 4;
            break;
        case VZME_TAP_2T32P:
            u32Tap = 2;
            break;
        default:
            u32Tap = 4;
            break;
    }

    j = 0;
    /* when tap == 8, there are 8 number in one group */
    if (u32Tap == 8)
    {
        /* 4 Bytes£¬32bits is the unit£¬every filter number is 10bits, 
           There are 8 numbers in one group£¬ take place at one group include 3*4 Bytes, 
           Array s32CoefAttr[51] add 1£¬The last 4 Bytes  only contain two numbers
           */
        for (i = 0; i < 17; i++)
        {
            s32CoefTmp1 = (HI_S32) * ps16Coef++;
            s32CoefTmp2 = (HI_S32) * ps16Coef++;
            s32CoefTmp3 = (HI_S32) * ps16Coef++;
            pBitCoef->s32CoefAttr[j++] = (s32CoefTmp1 & 0x3ff) + ((s32CoefTmp2 << 10) & 0xffc00) + (s32CoefTmp3 << 20);

            s32CoefTmp1 = (HI_S32) * ps16Coef++;
            s32CoefTmp2 = (HI_S32) * ps16Coef++;
            s32CoefTmp3 = (HI_S32) * ps16Coef++;
            pBitCoef->s32CoefAttr[j++] = (s32CoefTmp1 & 0x3ff) + ((s32CoefTmp2 << 10) & 0xffc00) + (s32CoefTmp3 << 20);

            s32CoefTmp1 = (HI_S32) * ps16Coef++;
            s32CoefTmp2 = (HI_S32) * ps16Coef++;
            pBitCoef->s32CoefAttr[j++] = (s32CoefTmp1 & 0x3ff) + ((s32CoefTmp2 << 10) & 0xffc00);
        }
        
        pBitCoef->u32Size = 17 * 3 * 4; /* size unit is Byte */
    }
    else
    {
        for (i = 0; i < 17; i++)
        {
            for (k = 1; k <= (u32Tap / 2); k++)
            {
                s32CoefTmp1 = (HI_S32) * ps16Coef++;
                s32CoefTmp2 = (HI_S32) * ps16Coef++;
                pBitCoef->s32CoefAttr[j++] = (s32CoefTmp1 & 0xffff) + (s32CoefTmp2 << 16);
            }
        }
        pBitCoef->u32Size = 17 * (u32Tap / 2) * 4;
    }

    return HI_SUCCESS;
}

/* load hor and vert coef */
static HI_U32 VpssZmeLoadCoef(VZME_COEF_RATIO_E enCoefRatio, VZME_VPSS_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U32* pu32Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    /* load chroma horizontal zoom coef */
    VpssZmeTransCoefAlign(g_pVZmeVpssCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);
    memcpy(pu32Addr, stArray.s32CoefAttr, stArray.u32Size);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_VpssLoadCoef(HI_U32* pu32CurAddr, HI_U32 u32PhyAddr, ALG_VZME_COEF_ADDR_S* pstAddrTmp)
{
    HI_U32 u32NumSize;

    /* HL8 */
    u32NumSize = 256; 
    VpssZmeLoadCoef(VZME_COEF_1, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;
    
    VpssZmeLoadCoef(VZME_COEF_E1, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_075, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_05, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_033, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_025, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_0, VZME_VPSS_COEF_8T32P_LH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* HC8 */
    u32NumSize = 256;
    VpssZmeLoadCoef(VZME_COEF_1, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_E1, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_075, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_05, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_033, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_025, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_0, VZME_VPSS_COEF_8T32P_CH, VZME_TAP_8T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC8_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* VL6 */
    u32NumSize = 256;
    VpssZmeLoadCoef(VZME_COEF_1, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_E1, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_075, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_05, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_033, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_025, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_0, VZME_VPSS_COEF_6T32P_LV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* VC6 */
    u32NumSize = 256;
    VpssZmeLoadCoef(VZME_COEF_1, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_E1, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_075, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_05, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_033, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_025, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VpssZmeLoadCoef(VZME_COEF_0, VZME_VPSS_COEF_6T32P_CV, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    return;
}
/****************************Load VPSS ZME COEF END****************************************/

/********************************VPSS ZME START*******************************************/
static HI_S32 PQ_HAL_VPSS_SetZmeEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if ((eMode ==  REG_ZME_MODE_HORL ) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hlmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hchmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vlmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vchmsc_en = bEnable;
            }
            break;
        case VPSS_REG_SD:
            if ((eMode == REG_ZME_MODE_HORL ) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hlmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hchmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vlmsc_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vchmsc_en = bEnable;
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeInSize(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, HI_U32 u32Height, HI_U32 u32Width)
{
    /* It does not exist ih and iw in HiFoneB2 */
    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeOutSize(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, HI_U32 u32Height, HI_U32 u32Width)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            pstReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh = u32Height - 1;
            pstReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow = u32Width  - 1;
            break;
        case VPSS_REG_SD:
            pstReg->VPSS_VSD_ZMEORESO.bits.vsd_zme_oh = u32Height - 1;
            pstReg->VPSS_VSD_ZMEORESO.bits.vsd_zme_ow = u32Width  - 1;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeOutSize error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeFirEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if ((eMode ==  REG_ZME_MODE_HORL ) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hlfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hchfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vlfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vchfir_en = bEnable;
            }
            break;
        case VPSS_REG_SD:
            if ((eMode == REG_ZME_MODE_HORL ) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hlfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hchfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vlfir_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vchfir_en = bEnable;
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeFirEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeMidEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if ((eMode == REG_ZME_MODE_HORL) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hlmid_en = bEnable;
            }
            if ((eMode ==  REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_HSP.bits.hchmid_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vlmid_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VHD0_VSP.bits.vchmid_en = bEnable;
            }
            break;
        case VPSS_REG_SD:
            if ((eMode == REG_ZME_MODE_HORL) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hlmid_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_HORC) || (eMode == REG_ZME_MODE_HOR) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_HSP.bits.hchmid_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERL) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vlmid_en = bEnable;
            }
            if ((eMode == REG_ZME_MODE_VERC) || (eMode == REG_ZME_MODE_VER) || (eMode == REG_ZME_MODE_ALL))
            {
                pstReg->VPSS_VSD_VSP.bits.vchmid_en = bEnable;
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeMidEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmePhase(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_S32 s32Phase)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                pstReg->VPSS_VHD0_HCOFFSET.bits.hor_coffset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_HORL)
            {
                pstReg->VPSS_VHD0_HLOFFSET.bits.hor_loffset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_VERC)
            {
                pstReg->VPSS_VHD0_VOFFSET.bits.vchroma_offset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_VERL)
            {
                pstReg->VPSS_VHD0_VOFFSET.bits.vluma_offset = s32Phase;
            }
            else
            {
                HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmePhase error\n");
            }
            break;
        case VPSS_REG_SD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                pstReg->VPSS_VSD_HCOFFSET.bits.hor_coffset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_HORL)
            {
                pstReg->VPSS_VSD_HLOFFSET.bits.hor_loffset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_VERC)
            {
                pstReg->VPSS_VSD_VOFFSET.bits.vchroma_offset = s32Phase;
            }
            else if (eMode == REG_ZME_MODE_VERL)
            {
                pstReg->VPSS_VSD_VOFFSET.bits.vluma_offset = s32Phase;
            }
            else
            {
                HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmePhase error\n");
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmePhase error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeRatio(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_U32 u32Ratio)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HOR)
            {
                pstReg->VPSS_VHD0_HSP.bits.hratio = u32Ratio;
            }
            else
            {
                pstReg->VPSS_VHD0_VSR.bits.vratio = u32Ratio;
            }
            break;
        case VPSS_REG_SD:
            if (eMode == REG_ZME_MODE_HOR)
            {
                pstReg->VPSS_VSD_HSP.bits.hratio = u32Ratio;
            }
            else
            {
                pstReg->VPSS_VSD_VSR.bits.vratio = u32Ratio;
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeRatio error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeHfirOrder(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, HI_BOOL bVfirst)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            pstReg->VPSS_VHD0_HSP.bits.hfir_order = bVfirst;
            break;
        case VPSS_REG_SD:
            pstReg->VPSS_VSD_HSP.bits.hfir_order  = bVfirst;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeHfirOrder error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeInFmt(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, HI_DRV_PIX_FORMAT_E eFormat)
{
    HI_U32 u32Format;

    switch (eFormat)
    {
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_YUV400:
        case HI_DRV_PIX_FMT_YUV422_1X2:
        case HI_DRV_PIX_FMT_YUV420p:
        case HI_DRV_PIX_FMT_YUV410p:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_YUV_444:
        case HI_DRV_PIX_FMT_YUV422_2X1:
        case HI_DRV_PIX_FMT_YUV411:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
            u32Format = 0x0;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeInFmt error\n");
            return HI_FAILURE;
    }

    switch (ePort)
    {
        case VPSS_REG_HD:
            pstReg->VPSS_VHD0_VSP.bits.zme_in_fmt = u32Format;
            break;
        case VPSS_REG_SD:
            pstReg->VPSS_VSD_VSP.bits.zme_in_fmt = u32Format;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeInFmt error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeOutFmt(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, HI_DRV_PIX_FORMAT_E eFormat)
{
    HI_U32 u32Format;

    switch (eFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
        case HI_DRV_PIX_FMT_ARGB8888:   /* sp420->sp422->csc->rgb */
        case HI_DRV_PIX_FMT_ABGR8888:
      //case HI_DRV_PIX_FMT_KBGR8888:
            u32Format = 0x0;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeOutFmt error\n");
            return HI_FAILURE;
    }

    switch (ePort)
    {
        case VPSS_REG_HD:
            pstReg->VPSS_VHD0_VSP.bits.zme_out_fmt = u32Format;
            break;
        case VPSS_REG_SD:
            pstReg->VPSS_VSD_VSP.bits.zme_out_fmt = u32Format;
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeOutFmt error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeCoefAddr(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE* pstReg, REG_ZME_MODE_E eMode, HI_U32 u32Addr)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                pstReg->VPSS_VHD0_ZME_CHADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_HORL)
            {
                pstReg->VPSS_VHD0_ZME_LHADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERC)
            {
                pstReg->VPSS_VHD0_ZME_CVADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERL)
            {
                pstReg->VPSS_VHD0_ZME_LVADDR = u32Addr;
            }
            else
            {
                HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeCoefAddr error\n");
            }
            break;
        case VPSS_REG_SD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                pstReg->VPSS_VSD_ZME_CHADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_HORL)
            {
                pstReg->VPSS_VSD_ZME_LHADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERC)
            {
                pstReg->VPSS_VSD_ZME_CVADDR = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERL)
            {
                pstReg->VPSS_VSD_ZME_LVADDR = u32Addr;
            }
            else
            {
                HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeCoefAddr error\n");
            }
            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeCoefAddr error\n");
            break;
    }

    return HI_SUCCESS;
}

/* Vpss Cfg Set to reg */
HI_VOID PQ_HAL_VpssZmeRegCfg(HI_U32 u32LayerId, S_CAS_REGS_TYPE* pstReg, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL bFirEnable)
{
    HI_U32 ih;
    HI_U32 iw;
    HI_U32 oh;
    HI_U32 ow;
    HI_U32 u32HandleNo = 0;

    ih = pstZmeRtlPara->u32ZmeHIn;
    iw = pstZmeRtlPara->u32ZmeWIn;
    oh = pstZmeRtlPara->u32ZmeHOut;
    ow = pstZmeRtlPara->u32ZmeWOut;

    if (ih == oh && iw == ow && iw > 1920 && ih > 1200)
    {
        PQ_HAL_VPSS_SetZmeEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_ALL, HI_FALSE);
    }
    else
    {
        PQ_HAL_VPSS_SetZmeEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_ALL, HI_TRUE);
    }
    
/* debug start z00128410 2012-1-30; debug config for SDK debug: fixed to copy mode that don't need zme coefficient */
    /*
    pstZmeRtlPara->bZmeMdHL = 0;
    pstZmeRtlPara->bZmeMdHC = 0;
    pstZmeRtlPara->bZmeMdVL = 0;
    pstZmeRtlPara->bZmeMdVC = 0;
    */
/* debug end z00128410 2012-1-30 */

    if (!bFirEnable)
    {   /* for fidelity output */
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORL, pstZmeRtlPara->bZmeMdHL);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORC, pstZmeRtlPara->bZmeMdHC);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, HI_FALSE);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, HI_FALSE);
    }
    else
    {   /* for normal output */
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORL, pstZmeRtlPara->bZmeMdHL);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORC, pstZmeRtlPara->bZmeMdHC);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, pstZmeRtlPara->bZmeMdVL);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, pstZmeRtlPara->bZmeMdVC);
    }

    PQ_HAL_VPSS_SetZmeMidEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORL, pstZmeRtlPara->bZmeMedHL);
    PQ_HAL_VPSS_SetZmeMidEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORC, pstZmeRtlPara->bZmeMedHC);
    PQ_HAL_VPSS_SetZmeMidEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, pstZmeRtlPara->bZmeMedVL);
    PQ_HAL_VPSS_SetZmeMidEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, pstZmeRtlPara->bZmeMedVC);

    PQ_HAL_VPSS_SetZmePhase(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, pstZmeRtlPara->s32ZmeOffsetVL);
    PQ_HAL_VPSS_SetZmePhase(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, pstZmeRtlPara->s32ZmeOffsetVC);

    PQ_HAL_VPSS_SetZmeRatio(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HOR, pstZmeRtlPara->u32ZmeRatioHL);
    PQ_HAL_VPSS_SetZmeRatio(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VER, pstZmeRtlPara->u32ZmeRatioVL);

    /* hfir_order 0: Hor first; 1: Ver First */
    if ((iw <= 1920) && (ow <= 1920))
    {
        pstZmeRtlPara->bZmeOrder = 0;
        PQ_HAL_VPSS_SetZmeHfirOrder(u32HandleNo, u32LayerId, pstReg, pstZmeRtlPara->bZmeOrder);
    }
    else if ((iw <= 1920) && (ow > 1920))
    {
        pstZmeRtlPara->bZmeOrder = 1;
        PQ_HAL_VPSS_SetZmeHfirOrder(u32HandleNo, u32LayerId, pstReg, pstZmeRtlPara->bZmeOrder);
    }
    else if ((iw > 1920) && (ow <= 1920))
    {
        pstZmeRtlPara->bZmeOrder = 0;
        PQ_HAL_VPSS_SetZmeHfirOrder(u32HandleNo, u32LayerId, pstReg, pstZmeRtlPara->bZmeOrder);
    }
    else if ((iw > 1920) && (ow > 1920))
    {
        pstZmeRtlPara->bZmeOrder = 0;
        PQ_HAL_VPSS_SetZmeHfirOrder(u32HandleNo, u32LayerId, pstReg, pstZmeRtlPara->bZmeOrder);
    }

    if (pstZmeRtlPara->u8ZmeYCFmtIn == PQ_ALG_ZME_PIX_FORMAT_SP420)
    {
        PQ_HAL_VPSS_SetZmeInFmt(u32HandleNo, u32LayerId, pstReg, HI_DRV_PIX_FMT_NV21);
    }
    else
    {
        PQ_HAL_VPSS_SetZmeInFmt(u32HandleNo, u32LayerId, pstReg, HI_DRV_PIX_FMT_NV61_2X1);
    }

    if (pstZmeRtlPara->u8ZmeYCFmtOut == PQ_ALG_ZME_PIX_FORMAT_SP420)
    {
        PQ_HAL_VPSS_SetZmeOutFmt(u32HandleNo, u32LayerId, pstReg, HI_DRV_PIX_FMT_NV21);
    }
    else
    {
        PQ_HAL_VPSS_SetZmeOutFmt(u32HandleNo, u32LayerId, pstReg, HI_DRV_PIX_FMT_NV61_2X1);
    }

    PQ_HAL_VPSS_SetZmeInSize(u32HandleNo, u32LayerId, pstReg, ih, iw);
    PQ_HAL_VPSS_SetZmeOutSize(u32HandleNo, u32LayerId, pstReg, oh, ow);

    PQ_HAL_VPSS_SetZmeCoefAddr(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORL, pstZmeRtlPara->u32ZmeCoefAddrHL);
    PQ_HAL_VPSS_SetZmeCoefAddr(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORC, pstZmeRtlPara->u32ZmeCoefAddrHC);
    PQ_HAL_VPSS_SetZmeCoefAddr(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, pstZmeRtlPara->u32ZmeCoefAddrVL);
    PQ_HAL_VPSS_SetZmeCoefAddr(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, pstZmeRtlPara->u32ZmeCoefAddrVC);

    return;
}

/*******************************VPSS ZME END*********************************/

