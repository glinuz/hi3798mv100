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
#include <linux/fs.h>

#include "pq_hal_zme.h"
#include "pq_mng_zme_coef.h"


#define V0_SCALER1_ENABLE_THD       2
#define V0_READ_FMT_ZME_THD         2
#define V0_HPZME_ENABLE_THD         3
#define V0_PREZME_ENABLE_2_THD      6
#define V0_PREZME_ENABLE_4_THD      12
#define V0_PREZME_ENABLE_8_THD      24
#define WBC_HSCALER_THD             4
#define DHD0_OUT_RESOLUTION_W_THD   3840
#define DHD0_OUT_RESOLUTION_H_THD   2160

#define SHARPEN_SUPPOR_MAX_WITDH    1920
#define SHARPEN_SUPPOR_MAX_HEIGHT   1080

static S_VDP_REGS_TYPE *pVdpReg;

static HI_BOOL sg_bVpssZmeDefault = HI_FALSE;
static HI_BOOL sg_bVdpZmeDefault  = HI_FALSE;
static HI_BOOL sg_bV0ReadFmtIn    = HI_TRUE;
static HI_BOOL sg_bHPZmeEnable    = HI_FALSE;
static HI_BOOL sg_bProcHDCPEn     = HI_FALSE;
static PQ_HAL_PREZME_HOR_MUL_E sg_enV0PreHScaler    = PQ_HAL_PREZME_HOR_DISABLE;
static PQ_HAL_PREZME_VER_MUL_E sg_enV0PreVScaler    = PQ_HAL_PREZME_VER_DISABLE;
static PQ_HAL_FROST_LEVEL_E    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_CLOSE;
static HI_PQ_FROST_LEVEL_E     sg_enProcFrostLevel  = HI_PQ_FROST_LEVEL_CLOSE;
static ZME_WBC_POINT_SEL_E     sg_enWbcPointSel     = ZME_WBC_POINT_V0;


/****************************Load VDP ZME COEF START*****************************************/
static HI_U32 VdpZmeTransCoefAlign(const HI_S16 *ps16Coef, VZME_TAP_E enTap, ZME_COEF_BITARRAY_S *pBitCoef)
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
#if 0/** deal with codecc **/
        case VZME_TAP_2T32P:
            u32Tap = 2;
            break;
#endif
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
static HI_U32 VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_RATIO_E enCoefRatio, VZME_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U8 *pu8Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    /* load horizontal zoom coef */
    VdpZmeTransCoefAlign(g_pVZmeReduceCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);

    memcpy(pu8Addr, stArray.stBit, stArray.u32Size);

    return HI_SUCCESS;
}

/* load vert coef */
static HI_S32 VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_RATIO_E enCoefRatio, VZME_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U8 *pu8Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    /* load vertical zoom coef */
    VdpZmeTransCoefAlign(g_pVZmeReduceCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);
    memcpy(pu8Addr, stArray.stBit, stArray.u32Size);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_VdpLoadCoef(ALG_VZME_MEM_S *pstVZmeCoefMem)
{
    HI_U8 *pu8CurAddr;
    HI_U32 u32PhyAddr;
    HI_U32 u32NumSizeLuma, u32NumSizeChro;
    ALG_VZME_COEF_ADDR_S *pstAddrTmp;

    pu8CurAddr = pstVZmeCoefMem->stMBuf.pu8StartVirAddr;
    u32PhyAddr = pstVZmeCoefMem->stMBuf.u32StartPhyAddr;
    pstAddrTmp = &(pstVZmeCoefMem->stZmeCoefAddr);


    /* H_L8C4:Luma-Hor-8T32P Chroma-Hor-4T32P */
    u32NumSizeLuma = 192; /* (8x18) / 12 * 16 = 192 */
    u32NumSizeChro = 96;  /* (4x18) / 12 * 16 = 96 */
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_8T32P_LH, VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL8_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefH(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_4T32P_CH, VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* V_L6C4:Luma-Vert-6T32P Chroma-Vert-4T32P */
    u32NumSizeLuma = 144;  /* (6x18) / 12 * 16 = 144 */
    u32NumSizeChro = 96;   /* (4x18) / 12 * 16 = 96 */
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_6T32P_LV, VZME_TAP_6T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_4T32P_CV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeChro;
    pu8CurAddr  += u32NumSizeChro;


    /* H_L8:Luma-Hor-8T32P(logic) */
    /* H_L6:Luma-Hor-6T32P(real) */
    u32NumSizeLuma = 192; /* (8x18) / 12 * 16 = 192 */
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_8T32P_LH,  VZME_TAP_8T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    /* V_L4:Luma-Vert-4T32P */
    u32NumSizeLuma = 96; /*(4x18) / 12 * 16 */
    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    VdpZmeLoadCoefV(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VDP_COEF_4T32P_LV,  VZME_TAP_4T32P, pu8CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSizeLuma;
    pu8CurAddr  += u32NumSizeLuma;

    return;
}
/*****************************Load VDP ZME COEF END*****************************************/



/*******************************WBC ZME START***************************************/
static HI_VOID PQ_HAL_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_WBC_DHD0_ZME_HLOFFSET WBC_DHD0_ZME_HLOFFSET;
    U_WBC_DHD0_ZME_HCOFFSET WBC_DHD0_ZME_HCOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 )
    {
        /* VHD layer zoom enable */
        WBC_DHD0_ZME_HLOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32)));
        WBC_DHD0_ZME_HLOFFSET.bits.hor_loffset = s32PhaseL;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32))), WBC_DHD0_ZME_HLOFFSET.u32);

        WBC_DHD0_ZME_HCOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32)));
        WBC_DHD0_ZME_HCOFFSET.bits.hor_coffset = s32PhaseC;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32))), WBC_DHD0_ZME_HCOFFSET.u32);
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
        WBC_DHD0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)));
        WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32PhaseL;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32))), WBC_DHD0_ZME_VOFFSET.u32);

        WBC_DHD0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)));
        WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32PhaseC;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32))), WBC_DHD0_ZME_VOFFSET.u32);
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
        WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
        WBC_DHD0_ZME_HSP.bits.hratio = u32Ratio;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeVerRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    U_WBC_DHD0_ZME_VSR WBC_DHD0_ZME_VSR;

    pVdpReg = PQ_HAL_GetVdpReg();
    if (enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32)) + enLayer * PQ_WBC_OFFSET));
        WBC_DHD0_ZME_VSR.bits.vratio = u32Ratio;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSR.u32);
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
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hlfir_en = u32firMode;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hchfir_en = u32firMode;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vlfir_en = u32firMode;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vchfir_en = u32firMode;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
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
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hlmid_en = bEnable;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hchmid_en = bEnable;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vlmid_en = bEnable;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vchmid_en = bEnable;
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
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
        WBC_DHD0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET));
        WBC_DHD0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }

    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeVerTap(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)
{
    /* vsc_luma_tap does not exist in 3798m_a and 3798m */
    /* vsc_chroma_tap does not exist in 3798m_a and 3798m */
    return;
}

static HI_VOID PQ_HAL_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr, HI_U32 u32CAddr)
{
    pVdpReg = PQ_HAL_GetVdpReg();

    if ( enLayer == VDP_LAYER_WBC_HD0 ||  enLayer == VDP_LAYER_WBC_ME  )
    {
        if (u32Mode == VDP_WBC_PARA_ZME_HOR)
        {
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_HLCOEFAD.u32)) + enLayer * PQ_WBC_OFFSET), u32Addr);
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_HCCOEFAD.u32)) + enLayer * PQ_WBC_OFFSET), u32CAddr);
        }
        else if (u32Mode == VDP_WBC_PARA_ZME_VER)
        {
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_VLCOEFAD.u32)) + enLayer * PQ_WBC_OFFSET), u32Addr);
            PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_VCCOEFAD.u32)) + enLayer * PQ_WBC_OFFSET), u32CAddr);
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
        WBC_DHD0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_PARAUP.u32))));
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
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_PARAUP.u32))), WBC_DHD0_PARAUP.u32);
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
        WBC_DHD0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET));
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = u32Fmt;
        PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
    }
    else
    {}

    return;
}

/* Vdp WBC Cfg Set to reg*/
/* explanation: In WBC We just pay atention to WBC_HD0 */
HI_VOID PQ_HAL_WbcZmeRegCfg(HI_U32 u32LayerId, ALG_VZME_RTL_PARA_S *pstZmeRtlPara, HI_BOOL bFirEnable)
{
    if (u32LayerId >= VDP_ZME_LAYER_WBC0)
    {
        u32LayerId = u32LayerId - VDP_ZME_LAYER_WBC0;
    }

    PQ_HAL_WBC_SetZmeHorRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioHL);
    PQ_HAL_WBC_SetZmeVerRatio(u32LayerId, pstZmeRtlPara->u32ZmeRatioVL);

    PQ_HAL_WBC_SetZmePhaseH(u32LayerId, pstZmeRtlPara->s32ZmeOffsetHL, pstZmeRtlPara->s32ZmeOffsetHC);
    PQ_HAL_WBC_SetZmePhaseV(u32LayerId, pstZmeRtlPara->s32ZmeOffsetVL, pstZmeRtlPara->s32ZmeOffsetVC);

#if 1
    /* zme enable and fir enable should not fix to a value. */
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

    /* It does not exist in 98m_a and 98m */
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


/*******************************Load VPSS ZME COEF START***********************************/
static HI_U32 VpssZmeTransCoefAlign(const HI_S16 *ps16Coef, VZME_TAP_E enTap, ZME_COEF_BITARRAY_S *pBitCoef)
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
#if 0/** deal with codecc **/
        case VZME_TAP_2T32P:
            u32Tap = 2;
            break;
#endif
        default:
            u32Tap = 4;
            break;
    }

    j = 0;
    /* when tap == 8, there are 8 number in one group */
    if (u32Tap == 8)
    {
        /* 4 Bytes，32bits is the unit，every filter number is 10bits,
           There are 8 numbers in one group， take place at one group include 3*4 Bytes,
           Array s32CoefAttr[51] add 1，The last 4 Bytes  only contain two numbers
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
static HI_U32 VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_RATIO_E enCoefRatio, VZME_VPSS_COEF_TYPE_E enCoefType, VZME_TAP_E enZmeTap, HI_U32 *pu32Addr)
{
    ZME_COEF_BITARRAY_S stArray;

    //load chroma horizontal zoom coef
    VpssZmeTransCoefAlign(g_pVZmeReduceVpssCoef[enCoefRatio][enCoefType], enZmeTap, &stArray);
    memcpy(pu32Addr, stArray.s32CoefAttr, stArray.u32Size);

    return HI_SUCCESS;
}

/* Vpss in 3798mv200 is LH6、CH4、LV4、CV4 */
HI_VOID PQ_HAL_VpssLoadCoef(HI_U32 *pu32CurAddr, HI_U32 u32PhyAddr, ALG_VZME_COEF_ADDR_S *pstAddrTmp)
{
    HI_U32 u32NumSize;

    /* HL6 */
    u32NumSize = 256;
    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VPSS_COEF_6T32P_LH, VZME_TAP_6T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHL6_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* HC4 */
    u32NumSize = 256;
    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VPSS_COEF_4T32P_CH, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrHC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* VL4 */
    u32NumSize = 256;
    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VPSS_COEF_4T32P_LV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVL4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    /* VC4 */
    u32NumSize = 256;
    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_1, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_E1, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_E1 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_075, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_075 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_05, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_05 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_033, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_033 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_025, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_025 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    VZmeLoadVpssCoef(PQ_HAL_ZME_COEF_0, PQ_HAL_ZME_VPSS_COEF_4T32P_CV, VZME_TAP_4T32P, pu32CurAddr);
    pstAddrTmp->u32ZmeCoefAddrVC4_0 = u32PhyAddr;
    u32PhyAddr  += u32NumSize;
    pu32CurAddr += u32NumSize / 4;

    return;
}

/***********************************Load VPSS ZME COEF END*******************************************/

/***************************************VPSS ZME START*******************************************/

static HI_S32 PQ_HAL_VPSS_SetZmeEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
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

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeInSize(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, HI_U32 u32Height, HI_U32 u32Width)
{
    switch (ePort)
    {
        case VPSS_REG_HD:

            break;
        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeInSize error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeOutSize(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, HI_U32 u32Height, HI_U32 u32Width)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            pstReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh = u32Height - 1;
            pstReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow = u32Width  - 1;
            break;

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeOutSize error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeFirEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
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

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeFirEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeMidEnable(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_BOOL bEnable)
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

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeMidEnable error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmePhase(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_S32 s32Phase)
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
                HI_ERR_PQ("Error,VPSS_REG_SetZmePhase error\n");
            }
            break;

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmePhase error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeRatio(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_U32 u32Ratio)
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

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeRatio error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeHfirOrder(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, HI_BOOL bVfirst)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            //pstReg->VPSS_VHD_HSP.bits.hfir_order = bVfirst;
            break;

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeHfirOrder error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeInFmt(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, HI_DRV_PIX_FORMAT_E eFormat)
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
            //pstReg->VPSS_VHD0_VSP.bits.zme_in_fmt = u32Format;
            break;

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeInFmt error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeOutFmt(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, HI_DRV_PIX_FORMAT_E eFormat)
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

        default:
            HI_ERR_PQ("Error,PQ_HAL_VPSS_SetZmeOutFmt error\n");
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_VPSS_SetZmeCoefAddr(HI_U32 u32HandleNo, VPSS_REG_PORT_E ePort, S_CAS_REGS_TYPE *pstReg, REG_ZME_MODE_E eMode, HI_U32 u32Addr)
{
    switch (ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                pstReg->VPSS_VHD0_ZME_CHADDR.bits.vhd0_scl_ch = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_HORL)
            {
                pstReg->VPSS_VHD0_ZME_LHADDR.bits.vhd0_scl_lh = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERC)
            {
                pstReg->VPSS_VHD0_ZME_CVADDR.bits.vhd0_scl_cv = u32Addr;
            }
            else if (eMode == REG_ZME_MODE_VERL)
            {
                pstReg->VPSS_VHD0_ZME_LVADDR.bits.vhd0_scl_lv = u32Addr;
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
HI_VOID PQ_HAL_VpssZmeRegCfg(HI_U32 u32LayerId, S_CAS_REGS_TYPE *pstReg, ALG_VZME_RTL_PARA_S *pstZmeRtlPara, HI_BOOL bFirEnable)
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
    {
        /* for fidelity output */
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORL, pstZmeRtlPara->bZmeMdHL);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_HORC, pstZmeRtlPara->bZmeMdHC);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERL, HI_FALSE);
        PQ_HAL_VPSS_SetZmeFirEnable(u32HandleNo, u32LayerId, pstReg, REG_ZME_MODE_VERC, HI_FALSE);
    }
    else
    {
        /* for normal output */
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

    if (u32LayerId == VPSS_REG_SD)
    {
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

static HI_VOID PQ_HAL_ColorSpaceTrans(PQ_HAL_DHD_COLORSPACE_E *penHDOutColorSpace, PQ_HAL_DHD_COLORSPACE_E *penSDOutColorSpace)
{
    HI_DRV_COLOR_SPACE_E enColorSpace_Dhd_HD, enColorSpace_Dhd_SD;

    if ((penHDOutColorSpace == HI_NULL) || (penSDOutColorSpace == HI_NULL))
    {
        return;
    }

    enColorSpace_Dhd_HD = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].enOutColorSpace;
    enColorSpace_Dhd_SD = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_0].enOutColorSpace;

    /* SD */
    if ((HI_DRV_CS_BT601_RGB_LIMITED == enColorSpace_Dhd_SD) || (HI_DRV_CS_BT601_RGB_FULL == enColorSpace_Dhd_SD)
        || (HI_DRV_CS_BT709_RGB_LIMITED == enColorSpace_Dhd_SD) || (HI_DRV_CS_BT709_RGB_FULL == enColorSpace_Dhd_SD)
        || (HI_DRV_CS_BT2020_RGB_LIMITED == enColorSpace_Dhd_SD) || (HI_DRV_CS_BT2020_RGB_FULL == enColorSpace_Dhd_SD))
    {
        *penSDOutColorSpace = PQ_HAL_DHD_OUT_RGB;
    }
    else
    {
        *penSDOutColorSpace = PQ_HAL_DHD_OUT_YUV;
    }

    /* HD */
    if ((HI_DRV_CS_BT601_RGB_LIMITED == enColorSpace_Dhd_HD) || (HI_DRV_CS_BT601_RGB_FULL == enColorSpace_Dhd_HD)
        || (HI_DRV_CS_BT709_RGB_LIMITED == enColorSpace_Dhd_HD) || (HI_DRV_CS_BT709_RGB_FULL == enColorSpace_Dhd_HD)
        || (HI_DRV_CS_BT2020_RGB_LIMITED == enColorSpace_Dhd_HD) || (HI_DRV_CS_BT2020_RGB_FULL == enColorSpace_Dhd_HD))
    {
        *penHDOutColorSpace = PQ_HAL_DHD_OUT_RGB;
    }
    else
    {
        *penHDOutColorSpace = PQ_HAL_DHD_OUT_YUV;
    }

    return;
}

static HI_S32 pq_hal_GetZmeCoefType(PQ_HAL_ZME_COEF_TYPE_E enCoefType,
                                    PQ_HAL_ZME_TAP_E enZmeTap,
                                    PQ_HAL_ZME_VDP_REDUCE_COEF_TYPE_E *penZmeCoefType,
                                    HI_U32 *pu32CoefSize)

{
    HI_U32 u32CoefSize = 0;
    PQ_HAL_ZME_VDP_REDUCE_COEF_TYPE_E enZmeCoefType = PQ_HAL_ZME_VDP_COEF_8T32P_LH;

    switch (enZmeTap)
    {
        case PQ_HAL_ZME_TAP_8T32P:
        {
            if (PQ_HAL_ZME_COEF_TYPE_LH == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_8T32P_LH;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 8 * 18;
            break;
        }
        case PQ_HAL_ZME_TAP_6T32P:
        {
            if (PQ_HAL_ZME_COEF_TYPE_LH == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_6T32P_LH;
            }
            else if (PQ_HAL_ZME_COEF_TYPE_LV == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_6T32P_LV;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 6 * 18;
            break;
        }
        case PQ_HAL_ZME_TAP_4T32P:
        {
            if (PQ_HAL_ZME_COEF_TYPE_LV == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_4T32P_LV;
            }
            else if (PQ_HAL_ZME_COEF_TYPE_CH == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_4T32P_CH;
            }
            else if (PQ_HAL_ZME_COEF_TYPE_CV == enCoefType)
            {
                enZmeCoefType = PQ_HAL_ZME_VDP_COEF_4T32P_CV;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 4 * 18;
            break;
        }
        case PQ_HAL_ZME_TAP_5T32P:
        {
            if (PQ_HAL_ZME_COEF_TYPE_LV == enCoefType)
            {
                enZmeCoefType = VZME_COEF_5T32P_LV;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 5 * 18;
            break;
        }
        case PQ_HAL_ZME_TAP_3T32P:
        {
            if (PQ_HAL_ZME_COEF_TYPE_CV == enCoefType)
            {
                enZmeCoefType = VZME_COEF_3T32P_CV;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 3 * 18;
            break;
        }
        case PQ_HAL_ZME_TAP_8T2P:
        {
            if ((PQ_HAL_ZME_COEF_TYPE_LH == enCoefType) || (PQ_HAL_ZME_COEF_TYPE_CH == enCoefType))
            {
                enZmeCoefType = VZME_COEF_8T2P_HOR;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 8 * 2;
            break;
        }
        case PQ_HAL_ZME_TAP_6T2P:
        {
            if ((PQ_HAL_ZME_COEF_TYPE_LV == enCoefType) || (PQ_HAL_ZME_COEF_TYPE_CV == enCoefType))
            {
                enZmeCoefType = VZME_COEF_6T2P_VER;
            }
            else
            {
                HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
                return HI_FAILURE;
            }

            u32CoefSize = sizeof(HI_S16) * 6 * 2;
            break;
        }
        default:
        {
            HI_ERR_PQ("enZmeTap:%d, enCoefType:%d,unsupport zme coef type!\n", enZmeTap, enCoefType);
            *penZmeCoefType = PQ_HAL_ZME_VDP_COEF_8T32P_LH;
            *pu32CoefSize = 0;
            return HI_FAILURE;
        }
    }

    *penZmeCoefType = enZmeCoefType;
    *pu32CoefSize = u32CoefSize;

    return HI_SUCCESS;
}

/* Coef Choice = Tap + Type */
HI_S32 PQ_HAL_GetZmeCoef(PQ_HAL_ZME_COEF_RATIO_E enCoefRatio, PQ_HAL_ZME_COEF_TYPE_E enCoefType, PQ_HAL_ZME_TAP_E enZmeTap, HI_S16 *ps16Coef)
{
    HI_S32 s32Ret;
    HI_U32 u32CoefSize = 0;
    PQ_HAL_ZME_VDP_REDUCE_COEF_TYPE_E enZmeCoefType = PQ_HAL_ZME_VDP_COEF_8T32P_LH;

    s32Ret =  pq_hal_GetZmeCoefType(enCoefType, enZmeTap, &enZmeCoefType, &u32CoefSize);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (0 == u32CoefSize)
    {
        HI_FATAL_PQ("Wrong Coef Size\n");
        return HI_FAILURE;
    }

    if ((PQ_HAL_FROST_LEVEL_CLOSE != sg_enFrostGlassLevel) && ((enZmeCoefType == PQ_HAL_ZME_VDP_COEF_8T32P_LH) || (enZmeCoefType == PQ_HAL_ZME_VDP_COEF_6T32P_LV)))
    {
        memcpy((HI_VOID *)ps16Coef, (HI_VOID *)g_pVFrostZmeReduceCoef[enCoefRatio][enZmeCoefType][sg_enFrostGlassLevel], u32CoefSize);
    }
    else
    {
        memcpy((HI_VOID *)ps16Coef, (HI_VOID *)g_pVZmeReduceCoef[enCoefRatio][enZmeCoefType], u32CoefSize);
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetZmeDefault(HI_BOOL bOnOff)
{
    sg_bVpssZmeDefault = bOnOff;
    sg_bVdpZmeDefault  = bOnOff;

    return HI_SUCCESS;
}

static HI_VOID PQ_HAL_VpZmeDefaultConfig(HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    if (HI_TRUE == sg_bVdpZmeDefault)
    {
        pstZmeOut->bZmeFirHL   = HI_FALSE;
        pstZmeOut->bZmeFirHC   = HI_FALSE;
        pstZmeOut->bZmeFirVL   = HI_FALSE;
        pstZmeOut->bZmeFirVC   = HI_FALSE;
        pstZmeOut->bZmeDefault = HI_TRUE;  /* 1: rwzb */
    }
    else
    {
        pstZmeOut->bZmeDefault = HI_FALSE; /* 0: no rwzb */
    }
    return;
}

static HI_VOID PQ_HAL_WbcVpZmeDefaultConfig(HI_PQ_WBC_VP_STRATEGY_OUT_S *pstZmeOut)
{
    if (HI_TRUE == sg_bVdpZmeDefault)
    {
        pstZmeOut->bZmeFirHL   = HI_FALSE;
        pstZmeOut->bZmeFirHC   = HI_FALSE;
        pstZmeOut->bZmeFirVL   = HI_FALSE;
        pstZmeOut->bZmeFirVC   = HI_FALSE;
        pstZmeOut->bZmeDefault = HI_TRUE;  /* 1: rwzb */
    }
    else
    {
        pstZmeOut->bZmeDefault = HI_FALSE; /* 0: no rwzb */
    }
    return;
}

static HI_BOOL PQ_HAL_GetDHD0P2IEnableFlag(HI_BOOL bSupportP2IOpen)
{
    if ((HI_TRUE == bSupportP2IOpen) && (HI_FALSE == g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].bProgressive))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_VOID PQ_HAL_V0PreZmeConfig(HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_BOOL *pbReadFrame, HI_PQ_PREZME_HOR_MUL_E *penHScalerMode,
                                     HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_PQ_PREZME_CONFIG_S *pstPreZmeConfig, HI_BOOL *pbOpenP2I)
{
    HI_BOOL bHPZmeEnable;

    /***
        Progressive Output List:
                 in        out          mode  multiple               scheme                zme tap
          1) 3840x2160  1920x1080         1      2       read frame and Ver prescaler  1     8644
          2) 1920x1080  1280x 720        1/4     6       read frame and Ver prescaler 1/2    8543
          3) 3840x2160  1280x 720        1/4     12      read frame and Ver prescaler 1/4    8543
          4) 4096x2304  1280x 720        1/4    12.8     read frame and Ver prescaler 1/8    8644

        Interlace Output List:
                 in        out          mode   multiple              scheme                 zme tap
          1) 720 x 576  720 x 576(288)    1       2      read field and Ver prescaler  1     8644
          2) 1920x1080  1920x1080(540)    1       2      read field and Ver prescaler  1     8644
          3) 720 x 576  720 x 480(240)    1      2.4     read frame and Ver prescaler  1     8644  ---zme出隔行
          4) 1920x1080  720 x 576(288)    1      3.75    read frame and Ver prescaler 1/2    8644
          5) 1920x1080  720 x 480(240)   1/4      18     read frame and Ver prescaler 1/8    8644  ---1080P源的极限场景, 不需要读场
          6) 3840x2160  1920x1080(540)   1/4      16     read frame and Ver prescaler 1/8    8644
          7) 3840x2160  720 x 576(288)   1/3     22.5    read frame and Ver prescaler 1/8    8543  ---需要zme降阶, 8644 -> 8543的场景, cbb消化
          8) 3840x2160  720 x 480(240)   1/4      36     read frame and Ver prescaler 1/8    8543
          9) 4096x2304  720 x 480(240)   1/4     38.4    read frame and Ver prescaler 1/8    8543  ---极限场景, 4K源能否读场?

          以上列举可以看出: FHD源, 除1080i制式输出之外，其余不需要读场; 极限场景需要打开P2I或者读场的, 都是4K源;
        ***/

    *pbOpenP2I = PQ_HAL_GetDHD0P2IEnableFlag(HI_FALSE); /* Modify: 98MV200不再使用P2I方案 */

    if ((HI_FALSE == g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].bProgressive)
        && ((pstZmeIn->u32FrmWIn == pstZmeIn->u32FrmWOut) && (pstZmeIn->u32FrmHIn == pstZmeIn->u32FrmHOut))) /* Interlace output and source resolution == out resolution */
    {
        *pbReadFrame    = HI_FALSE; /* preferential read field; 按接口时序读顶底场 */
        bHPZmeEnable    = HI_FALSE;
        *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
        *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
    }
    else
    {
        *pbReadFrame = HI_TRUE;

        if (pstZmeIn->u32FrmHIn <= V0_READ_FMT_ZME_THD * pstZmeIn->u32FrmHOut) /* (in <= out) or (> 1 ~ <= 2); read frame and close prescaler*/
        {
            bHPZmeEnable    = HI_FALSE;
            *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
        else if ((pstZmeIn->u32FrmHIn > V0_READ_FMT_ZME_THD * pstZmeIn->u32FrmHOut)
                 && (pstZmeIn->u32FrmHIn <= V0_HPZME_ENABLE_THD * pstZmeIn->u32FrmHOut)) /* > 2 ~ <= 3; read frame and Open HPzme */
        {
            /* 1. 假如不是按比例缩放;例如: 垂直在2~3之间; 但水平不在，这样开启水平预缩放可能会导致水平方向上先缩小再放大;
               2. 缩放中的配置--比如: 先垂直后水平，属于逻辑性能的限制，由逻辑自己Cover */
            bHPZmeEnable    = HI_TRUE;
            *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
        else if ((pstZmeIn->u32FrmHIn > V0_READ_FMT_ZME_THD * pstZmeIn->u32FrmHOut)
                 && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_2_THD * pstZmeIn->u32FrmHOut)) /* > 3 ~ <= 6; read frame and 1/2 prescaler*/
        {
            bHPZmeEnable    = HI_TRUE;
            *penVScalerMode = HI_PQ_PREZME_VER_2X;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
        else if ((pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_2_THD * pstZmeIn->u32FrmHOut)
                 && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_4_THD * pstZmeIn->u32FrmHOut)) /* > 6 ~ <= 12; read frame and 1/4 prescaler */
        {
            bHPZmeEnable    = HI_TRUE;
            *penVScalerMode = HI_PQ_PREZME_VER_4X;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
        else if ((pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_4_THD * pstZmeIn->u32FrmHOut)
                 && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_8_THD * pstZmeIn->u32FrmHOut)) /* > 12 ~ <= 24; read frame and 1/8 prescaler */
        {
            bHPZmeEnable    = HI_TRUE;
            *penVScalerMode = HI_PQ_PREZME_VER_8X;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
        else if (pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_8_THD * pstZmeIn->u32FrmHOut)       /* > 24; read field and 1/8 prescaler */
        {
            /* 该倍数只在4K源(Vpss ByPass)时才会进入 */
            bHPZmeEnable    = HI_TRUE;
            *penVScalerMode = HI_PQ_PREZME_VER_8X;
            *penHScalerMode = HI_PQ_PREZME_VER_2X;
        }
        else
        {
            bHPZmeEnable    = HI_FALSE;
            *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
            *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
        }
    }

    pstPreZmeConfig->bPreZme    = bHPZmeEnable;
    pstPreZmeConfig->bPreZmeFir = (HI_TRUE == bHPZmeEnable) ? HI_TRUE : HI_FALSE;

    return;
}

HI_VOID PQ_HAl_TransResolutionToLevel(PQ_HAL_VID_RESOLUTION stVidResolution, PQ_HAL_RES_LEVEL_E *penInResLevel, PQ_HAL_RES_LEVEL_E *penOutResLevel)
{
    PQ_HAL_RES_LEVEL_E enInResLevel, enOutResLevel;

    if (stVidResolution.u32InImageWidth <= 720) /* SD Source */
    {
        enInResLevel = HAL_RES_SD;
    }
    else if (stVidResolution.u32InImageWidth <= 1280) /* HD Source */
    {
        enInResLevel = HAL_RES_HD;
    }
    else if (stVidResolution.u32InImageWidth <= 1920) /* FHD Source */
    {
        enInResLevel = HAL_RES_FHD;
    }
    else /* UHD Source */
    {
        enInResLevel = HAL_RES_UHD;
    }

    if (stVidResolution.u32OutImageWidth <= 720) /* SD OutPut */
    {
        enOutResLevel = HAL_RES_SD;
    }
    else if (stVidResolution.u32OutImageWidth <= 1280) /* HD OutPut */
    {
        enOutResLevel = HAL_RES_HD;
    }
    else if (stVidResolution.u32OutImageWidth <= 1920) /* FHD OutPut */
    {
        enOutResLevel = HAL_RES_FHD;
    }
    else /* UHD OutPut */
    {
        enOutResLevel = HAL_RES_UHD;
    }

    *penInResLevel = enInResLevel;

    return;
}

HI_VOID PQ_HAL_GetFrostLevelLowPara(PQ_HAL_RES_LEVEL_E enInResLevel, PQ_HAL_RES_LEVEL_E enOutResLevel,
                                    HI_PQ_PREZME_HOR_MUL_E *penHScalerMode, HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_BOOL *pbHPZme)
{
    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_LOW;

    switch (enInResLevel)
    {
        case HAL_RES_SD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            break;
        case HAL_RES_HD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_MIDDLE;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_FHD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_2X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                    *pbHPZme = HI_TRUE;
                    break;
                case HAL_RES_HD:
                    break;
                case HAL_RES_FHD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                    break;
                case HAL_RES_UHD:
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_MIDDLE;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_UHD: /* Input FHD */
            *pbHPZme        = HI_TRUE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_4X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                case HAL_RES_HD:
                    break;
                case HAL_RES_FHD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                    break;
                case HAL_RES_UHD:
                    *penVScalerMode      = PQ_HAL_PREZME_VER_8X;
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_MIDDLE;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_BUTT:
            break;
    }
    return;
}

HI_VOID PQ_HAL_GetFrostLevelMiddlePara(PQ_HAL_RES_LEVEL_E enInResLevel, PQ_HAL_RES_LEVEL_E enOutResLevel,
                                       HI_PQ_PREZME_HOR_MUL_E *penHScalerMode, HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_BOOL *pbHPZme)
{
    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_MIDDLE;

    switch (enInResLevel)
    {
        case HAL_RES_SD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            break;
        case HAL_RES_HD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_HIGH;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_FHD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_2X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                    *pbHPZme = HI_TRUE;
                    break;
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                    break;
                case HAL_RES_UHD:
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_HIGH;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_UHD: /* Input FHD */
            *pbHPZme        = HI_TRUE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
            *penVScalerMode = PQ_HAL_PREZME_VER_4X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                    break;
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    *penVScalerMode      = PQ_HAL_PREZME_VER_8X;
                    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_HIGH;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_BUTT:
            break;
    }

    return;
}

HI_VOID PQ_HAL_GetFrostLevelHighPara(PQ_HAL_RES_LEVEL_E enInResLevel, PQ_HAL_RES_LEVEL_E enOutResLevel,
                                     HI_PQ_PREZME_HOR_MUL_E *penHScalerMode, HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_BOOL *pbHPZme)
{
    sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_HIGH;

    switch (enInResLevel)
    {
        case HAL_RES_SD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            break;
        case HAL_RES_HD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    *penVScalerMode = PQ_HAL_PREZME_VER_2X;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_FHD:
            *pbHPZme        = HI_FALSE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_4X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                    *pbHPZme = HI_TRUE;
                    break;
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_UHD: /* Input FHD */
            *pbHPZme        = HI_TRUE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_4X;
            *penVScalerMode = PQ_HAL_PREZME_VER_4X;
            switch (enOutResLevel)
            {
                case HAL_RES_SD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_2X;
                case HAL_RES_HD:
                case HAL_RES_FHD:
                    break;
                case HAL_RES_UHD:
                    *penHScalerMode = PQ_HAL_PREZME_HOR_8X;
                    *penVScalerMode = PQ_HAL_PREZME_VER_8X;
                    break;
                case HAL_RES_BUTT:
                    break;
            }
            break;
        case HAL_RES_BUTT:
            break;
    }

    return;
}


HI_VOID PQ_HAL_SetFrostPreZme(PQ_HAL_FROST_LEVEL_E enFrostLevel, PQ_HAL_RES_LEVEL_E enInResLevel, PQ_HAL_RES_LEVEL_E enOutResLevel,
                              HI_PQ_PREZME_HOR_MUL_E *penHScalerMode, HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_BOOL *pbHPZme)
{
    switch (enFrostLevel)
    {
        case HI_PQ_FROST_LEVEL_LOW:
            PQ_HAL_GetFrostLevelLowPara(enInResLevel, enOutResLevel, penHScalerMode, penVScalerMode, pbHPZme);
            break;
        case HI_PQ_FROST_LEVEL_MIDDLE:
            PQ_HAL_GetFrostLevelMiddlePara(enInResLevel, enOutResLevel, penHScalerMode, penVScalerMode, pbHPZme);
            break;
        case HI_PQ_FROST_LEVEL_HIGH:
            PQ_HAL_GetFrostLevelHighPara(enInResLevel, enOutResLevel, penHScalerMode, penVScalerMode, pbHPZme);
            break;
        case HI_PQ_FROST_LEVEL_CLOSE:
        default:
            *pbHPZme        = PQ_HAL_PREZME_VER_DISABLE;
            *penHScalerMode = PQ_HAL_PREZME_HOR_DISABLE;
            *penVScalerMode = PQ_HAL_PREZME_VER_DISABLE;
            break;
    }

    return;
}

HI_VOID PQ_HAL_FrostCfgTrans(HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, PQ_HAL_FROST_CFG_S *pstFrostCfg)
{
    pstFrostCfg->stV0Resolution.u32InImageWidth   = pstZmeIn->u32FrmWIn;
    pstFrostCfg->stV0Resolution.u32InImageHeight  = pstZmeIn->u32FrmHIn;
    pstFrostCfg->stV0Resolution.u32OutImageWidth  = pstZmeIn->u32FrmWOut;
    pstFrostCfg->stV0Resolution.u32OutImageHeight = pstZmeIn->u32FrmHOut;

    pstFrostCfg->eFrostLevel = pstZmeIn->eFrostLevel;

    if (sg_enProcFrostLevel != HI_PQ_FROST_LEVEL_CLOSE)
    {
        pstFrostCfg->eFrostLevel = sg_enProcFrostLevel;
    }

    sg_enFrostGlassLevel     = pstFrostCfg->eFrostLevel;
}
/*****
    * FrostGlass设置接口:
    * 1. 传递输入输出分辨率及毛玻璃强度；
    * 2. 将输入输出分辨率转化为分辨率等级；
    * 3. 根据毛玻璃强度等级修改传递给cbb的滤波系数；(cbb需要保证在此场景下在刷新滤波系数)
    * 4. 根据以上条件配置抽点抽行策略；
     ******/
HI_S32 PQ_HAL_SetFrostGlassLevel(PQ_HAL_FROST_CFG_S stFrostCfg, HI_BOOL *pbReadFrame, HI_PQ_PREZME_HOR_MUL_E *penHScalerMode,
                                 HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_PQ_PREZME_CONFIG_S *pstPreZmeConfig, HI_BOOL *pbOpenP2I)
{
    PQ_HAL_RES_LEVEL_E enInResLevel  = HAL_RES_HD;
    PQ_HAL_RES_LEVEL_E enOutResLevel = HAL_RES_HD;

    /*毛玻璃场景即不读场也不开启P2I */
    *pbReadFrame = HI_TRUE;
    *pbOpenP2I   = HI_FALSE;

    PQ_HAl_TransResolutionToLevel(stFrostCfg.stV0Resolution, &enInResLevel, &enOutResLevel);
    PQ_HAL_SetFrostPreZme(stFrostCfg.eFrostLevel, enInResLevel, enOutResLevel, penHScalerMode, penVScalerMode, &(pstPreZmeConfig->bPreZme));

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetHDCPAttest(HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_BOOL *pbReadFrame, HI_PQ_PREZME_HOR_MUL_E *penHScalerMode,
                            HI_PQ_PREZME_VER_MUL_E *penVScalerMode, HI_PQ_PREZME_CONFIG_S *pstPreZmeConfig, HI_BOOL *pbOpenP2I)
{
    HI_BOOL bHPZmeEnable;
    HI_U32  u32IntermedHeight, u32IntermedWidth;

    /* 中间分辨率由VDP传递: 第一级缩放(只做缩小)由于有性能限制, 要使用中间级的分辨率计算缩放比 */
    u32IntermedWidth  = pstZmeIn->stHDCPAttestFactor.u32IntermedWidth;
    u32IntermedHeight = pstZmeIn->stHDCPAttestFactor.u32IntermedHeight;

    /* HDCP2.2认证场景即不读场也不开启P2I */
    *pbReadFrame    = HI_TRUE;
    *pbOpenP2I      = HI_FALSE;
    *penHScalerMode = HI_PQ_PREZME_HOR_DISABLE;

    if (pstZmeIn->u32FrmHIn <= V0_READ_FMT_ZME_THD * u32IntermedHeight) /* (in <= out) or (> 1 ~ <= 2); read frame and close prescaler*/
    {
        bHPZmeEnable    = HI_FALSE;
        *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
    }
    else if ((pstZmeIn->u32FrmHIn > V0_READ_FMT_ZME_THD * u32IntermedHeight)
             && (pstZmeIn->u32FrmHIn <= V0_HPZME_ENABLE_THD * u32IntermedHeight)) /* > 2 ~ <= 3; read frame and Open HPzme */
    {
        bHPZmeEnable    = HI_TRUE;
        *penVScalerMode = HI_PQ_PREZME_VER_DISABLE;
    }
    else if ((pstZmeIn->u32FrmHIn > V0_READ_FMT_ZME_THD * u32IntermedHeight)
             && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_2_THD * u32IntermedHeight)) /* > 3 ~ <= 6; read frame and 1/2 prescaler*/
    {
        bHPZmeEnable    = HI_TRUE;
        *penVScalerMode = HI_PQ_PREZME_VER_2X;
    }
    else if ((pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_2_THD * u32IntermedHeight)
             && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_4_THD * u32IntermedHeight)) /* > 6 ~ <= 12; read frame and 1/4 prescaler */
    {
        bHPZmeEnable    = HI_TRUE;
        *penVScalerMode = HI_PQ_PREZME_VER_4X;
    }
    else if ((pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_4_THD * u32IntermedHeight)
             && (pstZmeIn->u32FrmHIn <= V0_PREZME_ENABLE_8_THD * u32IntermedHeight)) /* > 12 ~ <= 24; read frame and 1/8 prescaler */
    {
        bHPZmeEnable    = HI_TRUE;
        *penVScalerMode = HI_PQ_PREZME_VER_8X;
    }
    else if (pstZmeIn->u32FrmHIn > V0_PREZME_ENABLE_8_THD * u32IntermedHeight)       /* > 24; read field and 1/8 prescaler */
    {
        bHPZmeEnable    = HI_TRUE;
        *penVScalerMode = HI_PQ_PREZME_VER_8X;
        *penHScalerMode = HI_PQ_PREZME_VER_2X;
    }

    return HI_SUCCESS;
}


static HI_VOID pq_hal_EnableSharp(HI_BOOL bOnOff)
{
    HI_U32 u32Data = 0;
    U_SPCTRL  SPCTRL;
    S_VDP_REGS_TYPE *pVDPReg = PQ_HAL_GetVdpReg();

    SPCTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVDPReg->SPCTRL.u32)) + u32Data * PQ_VID_OFFSET));
    SPCTRL.bits.sharpen_en = bOnOff;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVDPReg->SPCTRL.u32)) + u32Data * PQ_VID_OFFSET), SPCTRL.u32);

    return;
}

static HI_VOID PQ_HAL_SetSharpConfig(HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    if ((pstZmeOut->stZmeFmt[0].u32FrmOutWidth > SHARPEN_SUPPOR_MAX_WITDH)
        || (pstZmeOut->stZmeFmt[0].u32FrmOutHeight > SHARPEN_SUPPOR_MAX_HEIGHT))
    {
        pq_hal_EnableSharp(HI_FALSE);
        return;
    }

    if ((pstZmeIn->u32FrmWOut == g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Width)
        && (pstZmeIn->u32FrmHOut == g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Height))
    {
        /*not preview*/
        return;
    }

    /*preview mode*/
    if ((g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Width > SHARPEN_SUPPOR_MAX_WITDH)
        || (g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Height > SHARPEN_SUPPOR_MAX_HEIGHT))
    {
        pq_hal_EnableSharp(HI_FALSE);
    }

    return;
}

static HI_VOID PQ_HAL_V0AllZmeResolutionRatioCalc(HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    if (HI_TRUE == pstZmeIn->stHDCPAttestFactor.bHDCPScene) /* HDCP2.2 */
    {
        pstZmeOut->stZmeFmt[0].u32FrmInHeight  = pstZmeIn->u32FrmHIn;
        pstZmeOut->stZmeFmt[0].u32FrmInWidth   = pstZmeIn->u32FrmWIn;
        pstZmeOut->stZmeFmt[0].u32FrmOutHeight = pstZmeIn->stHDCPAttestFactor.u32IntermedHeight;
        pstZmeOut->stZmeFmt[0].u32FrmOutWidth  = pstZmeIn->stHDCPAttestFactor.u32IntermedWidth;

        pstZmeOut->stZmeFmt[1].u32FrmInHeight  = pstZmeIn->stHDCPAttestFactor.u32IntermedHeight;
        pstZmeOut->stZmeFmt[1].u32FrmInWidth   = pstZmeIn->stHDCPAttestFactor.u32IntermedWidth;
        pstZmeOut->stZmeFmt[1].u32FrmOutHeight = pstZmeIn->u32FrmHOut;
        pstZmeOut->stZmeFmt[1].u32FrmOutWidth  = pstZmeIn->u32FrmWOut;

        pstZmeOut->u32ZmeNum = 2; /* zme number */
    }
    else
    {
        if ((pstZmeIn->u32FrmWOut >= DHD0_OUT_RESOLUTION_W_THD) && (pstZmeIn->u32FrmHOut >= DHD0_OUT_RESOLUTION_H_THD)
            && (pstZmeIn->u32FrmHOut >= pstZmeIn->u32FrmHIn * V0_SCALER1_ENABLE_THD) && (pstZmeIn->u32FrmWOut >= pstZmeIn->u32FrmWIn * V0_SCALER1_ENABLE_THD))
        {
            pstZmeOut->stZmeFmt[0].u32FrmInHeight  = pstZmeIn->u32FrmHIn;
            pstZmeOut->stZmeFmt[0].u32FrmInWidth   = pstZmeIn->u32FrmWIn;
            pstZmeOut->stZmeFmt[0].u32FrmOutHeight = pstZmeIn->u32FrmHOut / V0_SCALER1_ENABLE_THD;
            pstZmeOut->stZmeFmt[0].u32FrmOutWidth  = pstZmeIn->u32FrmWOut / V0_SCALER1_ENABLE_THD;

            pstZmeOut->stZmeFmt[1].u32FrmInHeight  = pstZmeOut->stZmeFmt[0].u32FrmOutHeight;
            pstZmeOut->stZmeFmt[1].u32FrmInWidth   = pstZmeOut->stZmeFmt[0].u32FrmOutWidth;
            pstZmeOut->stZmeFmt[1].u32FrmOutHeight = pstZmeIn->u32FrmHOut;
            pstZmeOut->stZmeFmt[1].u32FrmOutWidth  = pstZmeIn->u32FrmWOut;

            pstZmeOut->u32ZmeNum = 2; /* zme number */
        }
        else
        {
            /* Absolutely condition: when open scaler1, prescaler must be closed */
            pstZmeOut->stZmeFmt[0].u32FrmInHeight  = pstZmeIn->u32FrmHIn / (1 << pstZmeOut->eVScalerMode);
            pstZmeOut->stZmeFmt[0].u32FrmInWidth   = pstZmeIn->u32FrmWIn / (1 << pstZmeOut->eHScalerMode);
            pstZmeOut->stZmeFmt[0].u32FrmOutHeight = pstZmeIn->u32FrmHOut;
            pstZmeOut->stZmeFmt[0].u32FrmOutWidth  = pstZmeIn->u32FrmWOut;

            pstZmeOut->stZmeFmt[1].u32FrmInHeight  = pstZmeOut->stZmeFmt[0].u32FrmOutHeight;
            pstZmeOut->stZmeFmt[1].u32FrmInWidth   = pstZmeOut->stZmeFmt[0].u32FrmOutWidth;
            pstZmeOut->stZmeFmt[1].u32FrmOutHeight = pstZmeIn->u32FrmHOut;
            pstZmeOut->stZmeFmt[1].u32FrmOutWidth  = pstZmeIn->u32FrmWOut;

            pstZmeOut->u32ZmeNum = 1;
        }
    }

    pstZmeOut->stZmeFmt[0].u32PixInFmt  = pstZmeIn->u32FmtIn;
    pstZmeOut->stZmeFmt[0].u32PixOutFmt = 0; /* zme output: 0-422; 1-420; 2-444 */
    pstZmeOut->stZmeFmt[1].u32PixInFmt  = 0; /* zme input:  0-422; 1-420; 2-444 */
    pstZmeOut->stZmeFmt[1].u32PixOutFmt = 0; /* zme output: 0-422; 1-420; 2-444 */

    return;
}

static HI_VOID pa_hal_ConfigOneZmeMode(HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    HI_U32 u32FrmOutWidth  = pstZmeOut->stZmeFmt[0].u32FrmOutWidth;
    HI_U32 u32FrmInWidth   = pstZmeOut->stZmeFmt[0].u32FrmInWidth;
    HI_U32 u32FrmOutHeight = pstZmeOut->stZmeFmt[0].u32FrmOutHeight;
    HI_U32 u32FrmInHeight  = pstZmeOut->stZmeFmt[0].u32FrmInHeight;
    HI_U32 u32PixOutFmt    = pstZmeOut->stZmeFmt[0].u32PixOutFmt;
    HI_U32 u32PixInFmt     = pstZmeOut->stZmeFmt[0].u32PixInFmt;

    /* config zme mode; when frame resolution does not have change, can not open fir mode;
               e.g: 1920x1080 -> 1920x1080; other must open fir mode; */
    pstZmeOut->bZmeFirHL = (u32FrmOutWidth == u32FrmInWidth)  ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirVL = (u32FrmOutHeight == u32FrmInHeight) ? HI_FALSE : HI_TRUE;

    /* in format may be 420; out format must be 422; when 420 -> 422;
           vertical need up-sampling, but horizontal does not need */
    pstZmeOut->bZmeFirHC = pstZmeOut->bZmeFirHL;
    if (HI_TRUE == pstZmeOut->bZmeFirVL)
    {
        pstZmeOut->bZmeFirVC = HI_TRUE;
    }
    else
    {
        pstZmeOut->bZmeFirVC = (u32PixOutFmt == u32PixInFmt) ? HI_FALSE : HI_TRUE;
    }

    return;
}

static HI_VOID pa_hal_ConfigTwoZmeMode(HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    HI_U32 u32Zme0PixOutFmt = pstZmeOut->stZmeFmt[0].u32PixOutFmt;
    HI_U32 u32Zme0PixInFmt  = pstZmeOut->stZmeFmt[0].u32PixInFmt;
    HI_U32 u32Zme1PixOutFmt = pstZmeOut->stZmeFmt[1].u32PixOutFmt;
    HI_U32 u32Zme1PixInFmt  = pstZmeOut->stZmeFmt[1].u32PixInFmt;

    if ((pstZmeOut->stZmeFmt[0].u32FrmOutWidth == pstZmeOut->stZmeFmt[0].u32FrmInWidth)
        && (pstZmeOut->stZmeFmt[1].u32FrmOutWidth == pstZmeOut->stZmeFmt[1].u32FrmInWidth))
    {
        pstZmeOut->bZmeFirHL = HI_FALSE;
    }
    else
    {
        pstZmeOut->bZmeFirHL = HI_TRUE;
    }

    if ((pstZmeOut->stZmeFmt[0].u32FrmOutHeight == pstZmeOut->stZmeFmt[0].u32FrmInHeight)
        && (pstZmeOut->stZmeFmt[1].u32FrmOutHeight == pstZmeOut->stZmeFmt[1].u32FrmInHeight))
    {
        pstZmeOut->bZmeFirVL = HI_FALSE;
    }
    else
    {
        pstZmeOut->bZmeFirVL = HI_TRUE;
    }

    pstZmeOut->bZmeFirHC = pstZmeOut->bZmeFirHL;

    /* in format may be 420; out format must be 422; when 420 -> 422;
           vertical need up-sampling, but horizontal does not need */
    if (HI_TRUE == pstZmeOut->bZmeFirVL)
    {
        pstZmeOut->bZmeFirVC = HI_TRUE;
    }
    else if ((u32Zme0PixOutFmt == u32Zme0PixInFmt)
             && (u32Zme1PixOutFmt == u32Zme1PixInFmt))
    {
        pstZmeOut->bZmeFirVC = HI_FALSE;
    }
    else
    {
        pstZmeOut->bZmeFirVC = HI_TRUE;
    }

    return;
}

static HI_VOID PQ_HAL_V0AllZmeModeConfig(HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    pstZmeOut->bZmeMedH  = HI_FALSE; /* zme Median filter enable of horizontal luma: 0-off; 1-on*/
    pstZmeOut->bZmeMedV  = HI_FALSE;

    if (1 == pstZmeOut->u32ZmeNum)
    {
        pa_hal_ConfigOneZmeMode(pstZmeOut);
    }
    else if (2 == pstZmeOut->u32ZmeNum)
    {
        pa_hal_ConfigTwoZmeMode(pstZmeOut);
    }

    return;
}

static HI_PQ_WBC_POINT_SEL_E PQ_HAL_WbcVpWriteBackPointConfig(HI_PQ_WBC_STRATEGY_IN_S *pstZmeIn)
{
    HI_PQ_WBC_POINT_SEL_E   enVpPoint         = HI_PQ_WBC_POINT_V0;

    PQ_HAL_DHD_COLORSPACE_E enHDOutColorSpace = PQ_HAL_DHD_OUT_YUV;
    PQ_HAL_DHD_COLORSPACE_E enSDOutColorSpace = PQ_HAL_DHD_OUT_YUV;

    PQ_HAL_ColorSpaceTrans(&enHDOutColorSpace, &enSDOutColorSpace);

    /***
    回写点选择策略:
    (1) 过指标场景: 回写点在V0;
    (2) 多窗口情况: 选择VP上CSC之后的回写点;
    (3) 水印场景:   选择VP上CSC之后的回写点;
    (4) 毛玻璃场景: 选择VP上CSC之后的回写点;
    (5) 大比例缩小情况:
        即(源->SD显示的缩小比例，如缩小10倍)> (源->HD显示的缩小比例，如缩小6倍), 则选择VP上CSC之后的回写点;
          (SD输出有两级缩小, 可以提升SD输出缩小的图像质量) -- 等价于高清通路是缩小场景;
    (6) Else, 选择V0 Scaler0之前的回写点;
    ***/
    if (HI_TRUE == sg_bVdpZmeDefault) /* rwzb */
    {
        enVpPoint = HI_PQ_WBC_POINT_V0;
    }
    else if (PQ_HAL_DHD_OUT_RGB == enHDOutColorSpace) /* Add 20160325 d00241380 when HD RGB out, write back from V0 */
    {
        enVpPoint = HI_PQ_WBC_POINT_V0;
    }
    else if (HI_TRUE == pstZmeIn->bForceWbcPoint2Vp) /* (1)multi-window, (2)watermark, (3)forstglass; force to vp0 */
    {
        enVpPoint = HI_PQ_WBC_POINT_VP;
    }
    else if ((pstZmeIn->stLayerInfo[0].u32FrmWIn == pstZmeIn->stLayerInfo[1].u32FrmWOut)
             && (pstZmeIn->stLayerInfo[0].u32FrmHIn == pstZmeIn->stLayerInfo[1].u32FrmHOut)) /* Interlace output and source resolution == out resolution */
    {
        /* Add 20160325 d00241380 非指标场景, 但是源与CVBS的分辨率一致也要从V0回写
           e.g.: 576i --> 576p --> 576i(Hdmi), 576i(Cvbs); 回写点在V0;
                 480i --> 480p --> 480i(Hdmi), 480i(Cvbs); 回写点在V0; */
        enVpPoint = HI_PQ_WBC_POINT_V0;
    }
    else if ((pstZmeIn->stLayerInfo[0].u32FrmHOut < pstZmeIn->stLayerInfo[0].u32FrmHIn)
             && (pstZmeIn->stLayerInfo[0].u32FrmHOut > pstZmeIn->stLayerInfo[1].u32FrmHOut))
    {
        /* example: 720x576-->600x480(HD) 需要从V0回写;
           Judgement condition is height of V0, not the height of VP */
        enVpPoint = HI_PQ_WBC_POINT_VP;
    }
    else
    {
        enVpPoint = HI_PQ_WBC_POINT_V0;
    }

    return enVpPoint;
}

static HI_BOOL PQ_HAL_WbcVpZmeOutFrameOrField(HI_PQ_WBC_STRATEGY_IN_S *pstZmeIn)
{
    HI_BOOL bOutFmt = 0;
    /***
      WBC_VP回写数据的格式：本质上是根据HD接口和SD接口之间是否需要帧率转换来决定回写格式：
      (1) 如果HD帧率<SD帧率(如: HD输出1080P24，SD输出NTSC制式), 则WBC_VP回写帧数据;
          (以保证SD输出图像不会出现回退或上下抖动)
      (2) Else, WBC_VP回写场数据. 此种情况需要注意, HD输出60hz, 而SD输出NTSC(59.94hz)的情况,
          HD每隔16s比SD播放快一次, SD丢弃数据时要保证一次丢弃两场, 否则会出现极性反转的情况从而导致图像上下抖动;

      注意: HD-4KP30, SD-NTSC; HD-4KP25, SD-PAL; HD-1080P24, SD-NTSC; 需要V3按帧读数据, Vdp做场的重复适配接口输出;
      ***/
    if (HI_TRUE == pstZmeIn->bForceWbcFieldMode) /* 强制回写场 */
    {
        bOutFmt = 0; /* Frame format for zme output: 0-field; 1-frame */
    }
    else if (g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].u32RefreshRate !=  g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_0].u32RefreshRate)
    {
        bOutFmt = 1;
    }
    else
    {
        bOutFmt = 0;
    }

    return bOutFmt;
}

static HI_PQ_ZME_BIND_POSITION_E PQ_HAL_WbcVpZmePosition(HI_VOID)
{
    if (HI_FALSE == g_stPqStatus.bIsogenyMode) /* 非同源 */
    {
        return HI_PQ_ZME_BIND_V3;
    }

    return HI_PQ_ZME_BIND_WBC;
}

static HI_PQ_PREZME_HOR_MUL_E PQ_HAL_WbcVpHScalerConfig(HI_U32 u32InWidth)
{
    /* 98CV200 WBC HScaler的开关受缩放性能的限制, 当输入大于等于4K时打开; 后续芯片无法保证判断Hscaler开启的条件的一致性;
       策略中不涉及Hscaler的实现; PQ将Hsacaler + Scaler(WBC)看成一个完整的Scaler;
       Hsacler的策略由底层cbb自己做, 计算ratio配给zme的宽高, 同样在cbb消化;
       */
    if (u32InWidth >= DHD0_OUT_RESOLUTION_W_THD)
    {
        return HI_PQ_PREZME_HOR_2X; /* 固定2抽1 */
    }

    return HI_PQ_PREZME_HOR_DISABLE;
}

static HI_BOOL PQ_HAL_WbcVpZmeP2IConfig(HI_BOOL bWbcAPointProgressive, HI_PQ_WBC_VP_STRATEGY_OUT_S *pstZmeOut)
{
    HI_U32  u32InWidth    = 0;
    HI_U32  u32InHeight   = 0;
    HI_U32  u32OutWidth   = 0;
    HI_U32  u32OutHeight  = 0;
    HI_BOOL bWbcOutFormat = HI_FALSE; /* HI_FALSE: Interlace */
    HI_BOOL bOpenP2I      = HI_FALSE;

    u32InHeight   = pstZmeOut->stZme.u32FrmInHeight;
    u32InWidth    = pstZmeOut->stZme.u32FrmInWidth;
    u32OutWidth   = pstZmeOut->stZme.u32FrmOutWidth;
    u32OutHeight  = pstZmeOut->stZme.u32FrmOutHeight;
    bWbcOutFormat = pstZmeOut->stZme.bOutFmt;

    /***
       WBC_VP P2I的开启策略: (参考3798cv200 算法流程图)
       (1) 如果B点输出Progressive, 则P2I关闭;
       (2) 如果B点输出Interlace, 则分两种情况;
           A) 如果A点输入Interlace, 则P2I关闭;
           B) 如果A点输入Progressive, 则又分三种情况:
              a) 如果源resolution==目的resolution, 则P2I开启且需要匹配场极性;
              b) 如果垂直缩放比<THD'(如1/4, 此阈值需要测试后再确定), 则P2I开启(相当于两级缩小，为了提升大比例缩小的图像效果);
              c) Else, P2I关闭
      ***/
    /* 非同源模式: ZME绑定在V3上, do not care B Point is Progressive or not */
    if (HI_FALSE == g_stPqStatus.bIsogenyMode)
    {
        return HI_FALSE;
    }
    /* 以下同源模式: ZME绑定在WBC上, 因为标清输出必须是Interlace, 则B点输出为 Interlace */
    if (HI_TRUE == bWbcOutFormat) /* WBC输出是Progressive, 即B点Progressive */
    {
        bOpenP2I = HI_FALSE;
    }
    else if (HI_FALSE == bWbcAPointProgressive) /* B点输出Interlace, A点输入Interlace */
    {
        bOpenP2I = HI_FALSE;
    }
    else if (HI_TRUE == bWbcAPointProgressive)  /* B点输出Interlace, A点输入Progressive */
    {
        /* 1. 绝对条件: 即只在4K到Hscaler时, 才会开启,因此不需要在如下的判断条件中除以Hscaler.
           2. cvbs的输出固定是pal或ntsc, 符合如下条件的只有: 576p --> 576i; 480p --> 480i;    */
        if ((u32OutWidth == u32InWidth) && (u32OutHeight == u32InHeight))
        {
            /* e.g.: 576i --> 576p --> 576i(Hdmi), 576i(Cvbs); 回写点在V0上, V0读场, 则A点为Interlace;   不进到该条件;
                     576i --> 576p --> 720p(Hdmi), 576i(Cvbs); 回写点在V0上, V0读帧, 则A点为Progressive; 进到该条件;  */
            bOpenP2I = HI_TRUE;
        }
        else if (u32InHeight > WBC_HSCALER_THD * u32OutHeight)
        {
            bOpenP2I = HI_TRUE;
        }
        else
        {
            bOpenP2I = HI_FALSE;
        }
    }

    return bOpenP2I;
}

static HI_BOOL PQ_HAL_WbcVpZmeWriteBackPointIsProgressive(HI_VOID)
{
    if (ZME_WBC_POINT_V0 == sg_enWbcPointSel)
    {
        return sg_bV0ReadFmtIn; /* V0 读入数据隔逐行 */
    }

    return g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].bProgressive;
}

static HI_VOID PQ_HAL_WbcVpZmeResolutionRatioCalc(HI_PQ_WBC_STRATEGY_IN_S *pstZmeIn, HI_PQ_WBC_VP_STRATEGY_OUT_S *pstZmeOut)
{
    HI_U32  bWbcAPointPixFmt = 0; /* A point video format: 0-422; 1-420; 2-444 */
    HI_U32  u32InWidth       = 0;
    HI_U32  u32InHeight      = 0;
    HI_U32  u32OutWidth      = 0;
    HI_U32  u32OutHeight     = 0;

    if (ZME_WBC_POINT_V0 == sg_enWbcPointSel)
    {
        u32InWidth   = pstZmeIn->stLayerInfo[0].u32FrmWIn / (1 << (HI_U32)sg_enV0PreHScaler);
        u32InHeight  = pstZmeIn->stLayerInfo[0].u32FrmHIn / (1 << (HI_U32)sg_enV0PreVScaler);
        u32OutWidth  = pstZmeIn->stLayerInfo[1].u32FrmWOut; /* V3 FrmWOut */
        u32OutHeight = pstZmeIn->stLayerInfo[1].u32FrmHOut; /* V3 FrmHOut */

        if (2 == pstZmeIn->stLayerInfo[0].u32FmtIn) /* pix format: 0-422; 1-420; 2-444 */
        {
            bWbcAPointPixFmt = 0; /* pix format: 0-422; 1-420; 2-444 */
        }
        else
        {
            bWbcAPointPixFmt = pstZmeIn->stLayerInfo[0].u32FmtIn; /* V0 读入数据格式 */
        }
    }
    else if (ZME_WBC_POINT_VP == sg_enWbcPointSel)
    {
        /* 当绑定在VP上, 配给缩放的宽高应该是接口的宽高, 不是层的宽高; 否则PIP、预览无法回写全屏;
           Remark: 窗口宽高等于V0上实际宽高, 制式宽高等于VP上的宽高 */
        u32InWidth       = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Width;
        u32InHeight      = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].stFmtRect.s32Height;
        u32OutWidth      = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_0].stFmtRect.s32Width;
        u32OutHeight     = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_0].stFmtRect.s32Height;
        bWbcAPointPixFmt = 0; /* pix format: 0-422; 1-420; 2-444 */
    }

    /* Scaler Info Calc */
    pstZmeOut->stZme.u32FrmInHeight  = u32InHeight;
    pstZmeOut->stZme.u32FrmInWidth   = u32InWidth; /* can not divide 2 */
    pstZmeOut->stZme.u32FrmOutWidth  = u32OutWidth;
    pstZmeOut->stZme.u32FrmOutHeight = u32OutHeight;

    pstZmeOut->stZme.u32PixInFmt     = bWbcAPointPixFmt;
    pstZmeOut->stZme.u32PixOutFmt    = 0; /* 0-422; 1-420; 2-444; write back must be 422 */

    return;
}


static HI_VOID PQ_HAL_WbcVpZmeModeConfig(HI_PQ_WBC_VP_STRATEGY_OUT_S *pstZmeOut)
{
    /* config zme mode; when frame resolution does not have change, can not open fir mode; e.g: (480i-->480i) or (576i --> 576i); other must open fir mode; */
    pstZmeOut->bZmeFirHL = (pstZmeOut->stZme.u32FrmOutWidth  == pstZmeOut->stZme.u32FrmInWidth)  ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirVL = (pstZmeOut->stZme.u32FrmOutHeight == pstZmeOut->stZme.u32FrmInHeight) ? HI_FALSE : HI_TRUE;
    /* in format may be 420(write back from v0) or must be 422(write back from vp0), out format must be 422; when 420 -> 422; vertical need up-sampling, but horizontal does not need */
    pstZmeOut->bZmeFirHC = (HI_FALSE == pstZmeOut->bZmeFirHL) ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirVC = (HI_FALSE == pstZmeOut->bZmeFirVL) ? ((pstZmeOut->stZme.u32PixOutFmt == pstZmeOut->stZme.u32PixInFmt) ? HI_FALSE : HI_TRUE) : HI_TRUE;

    pstZmeOut->bZmeMedH  = HI_FALSE; /* zme Median filter enable of horizontal: 0-off; 1-on*/
    pstZmeOut->bZmeMedV  = HI_FALSE;

    return;
}

static HI_S32 PQ_HAL_GetV0ZmeStrategy (HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    PQ_HAL_FROST_CFG_S stFrostCfg;

    PQ_CHECK_NULL_PTR(pstZmeOut);

    if ((HI_TRUE == pstZmeIn->stHDCPAttestFactor.bHDCPScene)
        || (HI_TRUE == sg_bProcHDCPEn))
    {
        sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_CLOSE;
        PQ_HAL_SetHDCPAttest(pstZmeIn, &(pstZmeOut->bReadFmtIn), &(pstZmeOut->eHScalerMode),
                             &(pstZmeOut->eVScalerMode), &(pstZmeOut->stPreZmeConfig), &(pstZmeOut->bOpenP2I));
    }
    else
    {
        /* 读场、抽行、抽点、P2I 策略 */
        if ((pstZmeIn->eFrostLevel != HI_PQ_FROST_LEVEL_CLOSE)
            || (sg_enProcFrostLevel != HI_PQ_FROST_LEVEL_CLOSE))
        {
            PQ_HAL_FrostCfgTrans(pstZmeIn, &stFrostCfg);
            PQ_HAL_SetFrostGlassLevel(stFrostCfg, &(pstZmeOut->bReadFmtIn), &(pstZmeOut->eHScalerMode),
                                      &(pstZmeOut->eVScalerMode), &(pstZmeOut->stPreZmeConfig), &(pstZmeOut->bOpenP2I));
        }
        else
        {
            sg_enFrostGlassLevel = PQ_HAL_FROST_LEVEL_CLOSE;
            PQ_HAL_V0PreZmeConfig(pstZmeIn, &(pstZmeOut->bReadFmtIn), &(pstZmeOut->eHScalerMode),
                                  &(pstZmeOut->eVScalerMode), &(pstZmeOut->stPreZmeConfig), &(pstZmeOut->bOpenP2I));
        }
    }

    sg_bV0ReadFmtIn   = pstZmeOut->bReadFmtIn;
    sg_enV0PreHScaler = pstZmeOut->eHScalerMode;
    sg_enV0PreVScaler = pstZmeOut->eVScalerMode;
    sg_bHPZmeEnable   = pstZmeOut->stPreZmeConfig.bPreZme;

    PQ_HAL_V0AllZmeResolutionRatioCalc(pstZmeIn, pstZmeOut);
    PQ_HAL_SetSharpConfig(pstZmeIn, pstZmeOut);
    PQ_HAL_V0AllZmeModeConfig(pstZmeOut);
    PQ_HAL_VpZmeDefaultConfig(pstZmeOut);

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_GetV1ZmeStrategy (HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_PQ_LAYER_STRATEGY_OUT_S *pstZmeOut)
{
    PQ_CHECK_NULL_PTR(pstZmeOut);

    pstZmeOut->eHScalerMode = HI_PQ_PREZME_HOR_DISABLE;
    pstZmeOut->eVScalerMode = HI_PQ_PREZME_VER_DISABLE;
    pstZmeOut->bOpenP2I     = HI_FALSE; /* read frame */

    if (HI_TRUE == g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].bProgressive)
    {
        pstZmeOut->bReadFmtIn = HI_TRUE;  /* read frame */
    }
    else
    {
        pstZmeOut->bReadFmtIn = HI_FALSE; /* read field */
    }

    pstZmeOut->bZmeFirHL = (pstZmeIn->u32FrmWOut == pstZmeIn->u32FrmWIn) ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirVL = (pstZmeIn->u32FrmHOut == pstZmeIn->u32FrmHIn) ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirHC = (HI_FALSE == pstZmeOut->bZmeFirHL) ? HI_FALSE : HI_TRUE;
    pstZmeOut->bZmeFirVC = (HI_FALSE == pstZmeOut->bZmeFirVL) ? ((pstZmeIn->u32FmtIn == 0) ? HI_FALSE : HI_TRUE) : HI_TRUE;

    pstZmeOut->bZmeMedH  = HI_FALSE; /* zme Median filter enable of horizontal luma: 0-off; 1-on*/
    pstZmeOut->bZmeMedV  = HI_FALSE;

    pstZmeOut->u32ZmeNum = 1; /* chroma up-sampling */
    pstZmeOut->stZmeFmt[0].u32FrmInWidth   = pstZmeIn->u32FrmWIn;
    pstZmeOut->stZmeFmt[0].u32FrmInHeight  = pstZmeIn->u32FrmHIn;
    pstZmeOut->stZmeFmt[0].u32FrmOutWidth  = pstZmeIn->u32FrmWOut;
    pstZmeOut->stZmeFmt[0].u32FrmOutHeight = pstZmeIn->u32FrmHOut;
    pstZmeOut->stZmeFmt[0].u32PixInFmt     = pstZmeIn->u32FmtIn;
    pstZmeOut->stZmeFmt[0].u32PixOutFmt    = 0;

    PQ_HAL_VpZmeDefaultConfig(pstZmeOut);

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_GetWbcVpZmeStrategy (HI_PQ_WBC_STRATEGY_IN_S *pstZmeIn, HI_PQ_WBC_VP_STRATEGY_OUT_S *pstZmeOut)
{
    HI_BOOL bWbcAPointProgressive = HI_TRUE; /* Wbc A point frame format: 0-field; 1-frame */

    PQ_CHECK_NULL_PTR(pstZmeOut);

    pstZmeOut->enVpPoint = PQ_HAL_WbcVpWriteBackPointConfig(pstZmeIn);
    sg_enWbcPointSel     = (ZME_WBC_POINT_SEL_E)pstZmeOut->enVpPoint;

    /* 计算WBC层的输入输出宽、高、帧/场 */
    PQ_HAL_WbcVpZmeResolutionRatioCalc(pstZmeIn, pstZmeOut);

    bWbcAPointProgressive    = PQ_HAL_WbcVpZmeWriteBackPointIsProgressive();
    pstZmeOut->bReadFmtIn    = bWbcAPointProgressive; /* Frame format for zme Intput: 0-field; 1-frame */
    pstZmeOut->stZme.bOutFmt = PQ_HAL_WbcVpZmeOutFrameOrField(pstZmeIn);
    pstZmeOut->eZmePosition  = PQ_HAL_WbcVpZmePosition();

    pstZmeOut->bOpenP2I      = PQ_HAL_WbcVpZmeP2IConfig(bWbcAPointProgressive, pstZmeOut);
    pstZmeOut->eHScalerMode  = PQ_HAL_WbcVpHScalerConfig(pstZmeOut->stZme.u32FrmInWidth);

    /* e.g.: 576i -> 576p -> 576i(Hdmi), 576i(Cvbs); Write back from V0, V0 read field, A point is interlace */
    pstZmeOut->stZme.bInFmt  = (HI_TRUE == pstZmeOut->bOpenP2I) ? 0 : bWbcAPointProgressive;

    PQ_HAL_WbcVpZmeModeConfig(pstZmeOut);
    PQ_HAL_WbcVpZmeDefaultConfig(pstZmeOut);

    pqprint(PQ_PRN_ZME, "V0 WIn:%d, HIn:%d, WOut:%d, HOut:%d\t", pstZmeIn->stLayerInfo[0].u32FrmWIn, pstZmeIn->stLayerInfo[0].u32FrmHIn,
            pstZmeIn->stLayerInfo[0].u32FrmWOut, pstZmeIn->stLayerInfo[0].u32FrmHOut);
    pqprint(PQ_PRN_ZME, "V3 WIn:%d, HIn:%d, WOut:%d, HOut:%d\n", pstZmeIn->stLayerInfo[1].u32FrmWIn, pstZmeIn->stLayerInfo[1].u32FrmHIn,
            pstZmeIn->stLayerInfo[1].u32FrmWOut, pstZmeIn->stLayerInfo[1].u32FrmHOut);
    pqprint(PQ_PRN_ZME, "OUT WIn:%d, HIn:%d, WOut:%d, HOut:%d\n", pstZmeOut->stZme.u32FrmInWidth, pstZmeOut->stZme.u32FrmInHeight,
            pstZmeOut->stZme.u32FrmOutWidth, pstZmeOut->stZme.u32FrmOutHeight);

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_GetWbcDhdZmeStrategy (HI_PQ_LAYER_STRATEGY_IN_S *pstZmeIn, HI_PQ_WBC_DHD0_STRATEGY_OUT_S *pstZmeOut)
{
    PQ_CHECK_NULL_PTR(pstZmeOut);

    pstZmeOut->eHScalerMode   = HI_PQ_PREZME_HOR_2X; /* 由于逻辑性能的限制, Hscaler固定抽行1/2 */
    pstZmeOut->bInWbcVpZmeFmt = g_stPqStatus.stTimingInfo[HI_PQ_DISPLAY_1].bProgressive; /* 图形与视频叠加之后回写 */

    pstZmeOut->stZme.u32FrmInHeight  = pstZmeIn->u32FrmHIn;
    pstZmeOut->stZme.u32FrmInWidth   = pstZmeIn->u32FrmWIn; /* can not divide 2 */
    pstZmeOut->stZme.u32FrmOutWidth  = pstZmeIn->u32FrmWOut;
    pstZmeOut->stZme.u32FrmOutHeight = pstZmeIn->u32FrmHOut;
    /* Layer strategry: input 444, output 420 */
    pstZmeOut->stZme.u32PixInFmt     = 2; /* 0-422; 1-420; 2-444 */
    pstZmeOut->stZme.u32PixOutFmt    = 1; /* 0-422; 1-420; 2-444 */

    pstZmeOut->bZmeFirHL = HI_TRUE; /* 0-copy mode; 1-FIR filter mode*/
    pstZmeOut->bZmeFirVL = HI_TRUE;
    pstZmeOut->bZmeFirHC = HI_TRUE;
    pstZmeOut->bZmeFirVC = HI_TRUE;

    pstZmeOut->bZmeMedH  = HI_FALSE; /* zme Median filter enable of horizontal luma: 0-off; 1-on*/
    pstZmeOut->bZmeMedV  = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetVdpZmeStrategy (VDP_ZME_LAYER_E enLayerId, HI_PQ_ZME_STRATEGY_IN_U *pstZmeIn, HI_PQ_ZME_STRATEGY_OUT_U *pstZmeOut)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (VDP_ZME_LAYER_V0 == enLayerId)
    {
        s32Ret = PQ_HAL_GetV0ZmeStrategy(&pstZmeIn->stLayerStrategy, &(pstZmeOut->stStrategyLayerId));
    }
    else if (VDP_ZME_LAYER_V1 == enLayerId)
    {
        s32Ret = PQ_HAL_GetV1ZmeStrategy(&pstZmeIn->stLayerStrategy, &(pstZmeOut->stStrategyLayerId));
    }
    else if (VDP_ZME_LAYER_WBC0 == enLayerId)
    {
        s32Ret = PQ_HAL_GetWbcVpZmeStrategy(&pstZmeIn->stWbcStrategy, &(pstZmeOut->stStrategyWbcVp));
    }
    else if (VDP_ZME_LAYER_WBC_DHD == enLayerId)
    {
        s32Ret = PQ_HAL_GetWbcDhdZmeStrategy(&pstZmeIn->stLayerStrategy, &(pstZmeOut->stStrategyWbcDhd));
    }
    else
    {
        HI_ERR_PQ("LayerId:%d, unsupport zme strategy!\n", enLayerId);
    }

    return s32Ret;
}


HI_S32 PQ_HAL_SetProcFrostLevel(HI_PQ_FROST_LEVEL_E enFrostLevel)
{
    sg_enProcFrostLevel = enFrostLevel;
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetHDCPEn(HI_BOOL bOnOff)
{
    sg_bProcHDCPEn = bOnOff;
    return HI_SUCCESS;
}


