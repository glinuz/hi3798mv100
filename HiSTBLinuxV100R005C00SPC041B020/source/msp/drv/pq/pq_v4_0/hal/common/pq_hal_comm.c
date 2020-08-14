/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_comm.c
  Version       : Initial Draft
  Author        : pengjunwei 00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/
#include "pq_hal_comm.h"
#ifndef HI_BUILD_IN_BOOT
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "hi_osal.h"
#else
#include "hi_common.h"
#include "hi_drv_pdm.h"
#endif



#ifndef HI_BUILD_IN_BOOT

/* VPSS/VDP 本地寄存器，用于初始化参数*/
static S_CAS_REGS_TYPE *sg_pstVPSSRegLocal = NULL;
static S_VDP_REGS_TYPE *sg_pstVDPRegLocal  = NULL;

/* VPSS/VDP 虚拟寄存器，用于配置参数*/
static S_CAS_REGS_TYPE    *sg_pstVPSSRegMem[VPSS_HANDLE_NUM]    = {NULL};
static S_VPSSWB_REGS_TYPE *sg_pstVPSSWbcRegMem[VPSS_HANDLE_NUM] = {NULL};
static S_VDP_REGS_TYPE    *sg_pstVDPRegMem = NULL;

/* PQ HAL初始化标志位*/
static HI_BOOL sg_bHALInit = HI_FALSE;

/* 打印使能开关 */
static HI_U32 sg_PrintType = PQ_PRN_NOTHING;

/* 打印中间结果的暂存区域 */
static HI_S8  sg_PrintMsg[1024];

#if defined(CHIP_TYPE_hi3798cv200)
PQ_MMZ_BUF_S     g_stIPSelAlgBuffer;
S_VDP_REGS_TYPE *g_pstIPSelVdpAlg = HI_NULL;
#endif

/**
 \brief 初始化HAL模块，申请本地寄存器内存;
 \attention \n
  无

 \param[in] none

 \retval ::HI_SUCCESS

 */
HI_S32  PQ_HAL_Init(HI_VOID)
{
    if (HI_TRUE == sg_bHALInit)
    {
        return HI_SUCCESS;
    }

    sg_pstVPSSRegLocal = (S_CAS_REGS_TYPE *)HI_KMALLOC(HI_ID_PQ, sizeof(S_CAS_REGS_TYPE), GFP_KERNEL);
    if (sg_pstVPSSRegLocal == HI_NULL)
    {
        HI_ERR_PQ("sg_pstVPSSRegLocal can not kmalloc!\n");
        return HI_FAILURE;
    }

    memset(sg_pstVPSSRegLocal, 0, sizeof(S_CAS_REGS_TYPE));
    sg_pstVDPRegLocal = (S_VDP_REGS_TYPE *)HI_KMALLOC(HI_ID_PQ, sizeof(S_VDP_REGS_TYPE), GFP_KERNEL);
    if (sg_pstVDPRegLocal == HI_NULL)
    {
        HI_ERR_PQ("sg_pstVDPRegLocal can not kmalloc!\n");
        return HI_FAILURE;
    }

    memset(sg_pstVDPRegLocal, 0, sizeof(S_VDP_REGS_TYPE));

#if defined(CHIP_TYPE_hi3798cv200)
    memset(&g_stIPSelAlgBuffer, 0, sizeof(PQ_MMZ_BUF_S));
    if (HI_SUCCESS != PQ_HAL_MMZ_AllocAndMap("PQ_IPSEL_ALG", HI_NULL, sizeof(S_VDP_REGS_TYPE), 0, &g_stIPSelAlgBuffer))
    {
        HI_ERR_PQ("PQ_IPSEL_ALG memory allocated failed!\n");
        return HI_FAILURE;
    }
    g_pstIPSelVdpAlg = (S_VDP_REGS_TYPE *)g_stIPSelAlgBuffer.pu8StartVirAddr;
#endif

    sg_bHALInit = HI_TRUE;
    return HI_SUCCESS;
}

/**
 \brief 去初始化HAL模块,释放本地寄存器内存;
 \attention \n
  无

 \param[in] none

 \retval ::HI_SUCCESS

 */
HI_S32  PQ_HAL_Deinit(HI_VOID)
{
    HI_S32 i;

    if (HI_FALSE == sg_bHALInit)
    {
        return HI_SUCCESS;
    }

    HI_KFREE(HI_ID_PQ, sg_pstVPSSRegLocal);
    HI_KFREE(HI_ID_PQ, sg_pstVDPRegLocal);

    sg_pstVPSSRegLocal  = NULL;
    sg_pstVDPRegLocal   = NULL;

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        sg_pstVPSSRegMem[i]    = NULL;
        sg_pstVPSSWbcRegMem[i] = NULL;
    }

    sg_pstVDPRegMem   = NULL;

#if defined(CHIP_TYPE_hi3798cv200)
    PQ_HAL_MMZ_UnmapAndRelease(&g_stIPSelAlgBuffer);
    g_pstIPSelVdpAlg = HI_NULL;
#endif

    sg_bHALInit = HI_FALSE;

    return HI_SUCCESS;
}


HI_VOID PQ_HAL_CopyRegBySize(HI_U32 *pDstReg, HI_U32 *pSrcReg, HI_U32 u32RegSize)
{
    HI_U32 i = 0;

    for (i = 0; i < u32RegSize; i++)
    {
        PQ_HAL_RegWrite((HI_U32 *)(pDstReg + i), (*(pSrcReg + i)));
    }

    return;
}


/**
 \brief 更新VPSS PQ;
 \attention \n
  无

 \param[in] *pDstVpssAddr  :目标地址
 \param[in] *pSrcVpssAddr  :源地址
 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_UpdateVpssPQ(S_CAS_REGS_TYPE *pDstVpssAddr, S_CAS_REGS_TYPE *pSrcVpssAddr)
{
    HI_U32 u32ByteSize = 0;

#ifdef PQ_ALG_DB
    /* DB */
#if defined(CHIP_TYPE_hi3798cv200)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DB_CTRL, VPSS_DB_CTRL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DB_CTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DB_CTRL), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DB_LUT34, VPSS_DB_LUT40);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DB_LUT34),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DB_LUT34), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DB_LUT44, VPSS_DB_THR3);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DB_LUT44),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DB_LUT44), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DBD_THDEDGE, VPSS_DBD_BLKSIZE);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DBD_THDEDGE),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DBD_THDEDGE), u32ByteSize / 4);

#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DB_CTRL, VPSS_DB_LUT6);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DB_CTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DB_CTRL), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DBD_THDEDGE, VPSS_DBD_FLAT);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DBD_THDEDGE),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DBD_THDEDGE), u32ByteSize / 4);
#endif
#endif

#ifdef PQ_ALG_DM
    /* DM */
#if defined(CHIP_TYPE_hi3798cv200)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DM_DIR, VPSS_DM_THR2);
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DM_DIR, VPSS_DM_LUT11);
#endif
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DM_DIR),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DM_DIR), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_DEI
    /* DEI */
#if defined(CHIP_TYPE_hi3798cv200)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DIECTRL, VPSS_DIEMCGBMCOEF1);
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DIECTRL, VPSS_DIEKMAG2);
#endif
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DIECTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DIECTRL), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_MCDI_RGDIFY, VPSS_MCDI_DEMO);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_MCDI_RGDIFY),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_MCDI_RGDIFY), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_FMD
    /* FMD */
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_PDPCCMOV, VPSS_PDREGION);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_PDPCCMOV),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_PDPCCMOV), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_TNR
    /* TNR */
#if defined(CHIP_TYPE_hi3798cv200)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_TNR_MODE, VPSS_TNR_TE_YCMT_MAPPING_K2 );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_TNR_MODE),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_TNR_MODE), u32ByteSize / 4);
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_TNR_TE_YCEG_MAPPING_CTRL, VPSS_TNR_EDGE_CMOVING_CK );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_TNR_TE_YCEG_MAPPING_CTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_TNR_TE_YCEG_MAPPING_CTRL), u32ByteSize / 4);
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_TNR_CLUT10, VPSS_TNR_CLUT67 );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_TNR_CLUT10),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_TNR_CLUT10), u32ByteSize / 4);
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_TNR_CONTRL, VPSS_TNR_CMOTIONSTR_LUT31 );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_TNR_CONTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_TNR_CONTRL), u32ByteSize / 4);
#endif
#endif

#ifdef PQ_ALG_SNR
    /* SNR */
#if defined(CHIP_TYPE_hi3798cv200)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_SNR_ENABLE, VPSS_SNR_DEMO);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_SNR_ENABLE),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_SNR_ENABLE), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(MOTION_EDGE_LUT_01a, MOTION_EDGE_LUT_16h);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->MOTION_EDGE_LUT_01a),
                         (HI_VOID *)(&pSrcVpssAddr->MOTION_EDGE_LUT_01a), u32ByteSize / 4);

    u32ByteSize = VPSS_PQ_REG_SIZE(COLOR_BASED_LUT_01a, COLOR_BASED_LUT_16d);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->COLOR_BASED_LUT_01a),
                         (HI_VOID *)(&pSrcVpssAddr->COLOR_BASED_LUT_01a), u32ByteSize / 4);
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_SNR_ENABLE, MOTION_EDGE_LUT_32);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_SNR_ENABLE),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_SNR_ENABLE), u32ByteSize / 4);
#endif

#endif

#ifdef PQ_ALG_DERING
    /* DERING */
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DR_CTRL, VPSS_DR_THR26);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DR_CTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DR_CTRL), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_DESHOOT
    /* DESHOOT */
    u32ByteSize = VPSS_PQ_REG_SIZE(VPSS_DS_CTRL, VPSS_DS_THR20);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVpssAddr->VPSS_DS_CTRL),
                         (HI_VOID *)(&pSrcVpssAddr->VPSS_DS_CTRL), u32ByteSize / 4);
#endif

#if defined(CHIP_TYPE_hi3796mv200)
    u32ByteSize = 0;
#endif

    return;
}


/**
 \brief 更新VDP PQ;
 \attention \n
  无

 \param[in] *pDstVdpAddr  :目标地址
 \param[in] *pSrcVdpAddr  :源地址
 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_UpdateVdpPQ(S_VDP_REGS_TYPE *pDstVdpAddr, S_VDP_REGS_TYPE *pSrcVdpAddr)
{
    HI_U32 u32ByteSize = 0;

    if (HI_NULL == pDstVdpAddr)
    {
        HI_TRACE(HI_LOG_LEVEL_ERROR, HI_ID_PQ, "pDstVdpAddr is NULL!\n");
        return;
    }

    if (HI_NULL == pSrcVdpAddr)
    {
        HI_TRACE(HI_LOG_LEVEL_ERROR, HI_ID_PQ, "pSrcVdpAddr is NULL!\n");
        return;
    }

#ifdef PQ_ALG_ARTDS
    /* DESHOOT */
    u32ByteSize = VDP_PQ_REG_SIZE(V0_ZME_SHOOTCTRL_HL, V0_ZME_SHOOTCTRL_HL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_ZME_SHOOTCTRL_HL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_ZME_SHOOTCTRL_HL), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_ZME_SHOOTCTRL_VL, V0_ZME_SHOOTCTRL_VL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_ZME_SHOOTCTRL_VL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_ZME_SHOOTCTRL_VL), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_ZME2_SHOOTCTRL_HL, V0_ZME2_SHOOTCTRL_HL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_ZME2_SHOOTCTRL_HL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_ZME2_SHOOTCTRL_HL), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_ZME2_SHOOTCTRL_VL, V0_ZME2_SHOOTCTRL_VL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_ZME2_SHOOTCTRL_VL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_ZME2_SHOOTCTRL_VL), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_SHARPEN
    u32ByteSize = VDP_PQ_REG_SIZE(SPCTRL, SPGRADCONTRAST);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->SPCTRL),
                         (HI_VOID *)(&pSrcVdpAddr->SPCTRL), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_ACM
    u32ByteSize = VDP_PQ_REG_SIZE(VP0_ACM_CTRL, VP0_ACM_PARA_DATA);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->VP0_ACM_CTRL),
                         (HI_VOID *)(&pSrcVdpAddr->VP0_ACM_CTRL), u32ByteSize / 4);
    /* addr and reg update */
    pDstVdpAddr->VP0_ACM_CAD.bits.coef_addr      = pSrcVdpAddr->VP0_ACM_CAD.bits.coef_addr;
    pDstVdpAddr->VP0_PARAUP.bits.vp0_acmcoef_upd = pSrcVdpAddr->VP0_PARAUP.bits.vp0_acmcoef_upd;
#endif

#ifdef PQ_ALG_LCACM
    u32ByteSize = VDP_PQ_REG_SIZE(VHDACM0, VHDACM7);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->VHDACM0),
                         (HI_VOID *)(&pSrcVdpAddr->VHDACM0), u32ByteSize / 4);
#endif


#ifdef PQ_ALG_DCI
    u32ByteSize = VDP_PQ_REG_SIZE(VP0_DCICTRL, VP0_DCI_STATICS_DATA);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->VP0_DCICTRL),
                         (HI_VOID *)(&pSrcVdpAddr->VP0_DCICTRL), u32ByteSize / 4);
    /* addr and reg update */
    pDstVdpAddr->V0_DCICOEFAD.bits.dci_coef_addr = pSrcVdpAddr->V0_DCICOEFAD.bits.dci_coef_addr;
    pDstVdpAddr->V0_PARAUP.bits.v0_dcicoef_upd   = pSrcVdpAddr->V0_PARAUP.bits.v0_dcicoef_upd;
#endif

#ifdef PQ_ALG_ACC
    u32ByteSize = VDP_PQ_REG_SIZE(VHDACCTHD1, VHDACCTOTAL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->VHDACCTHD1),
                         (HI_VOID *)(&pSrcVdpAddr->VHDACCTHD1), u32ByteSize / 4);
#endif

#ifdef PQ_ALG_DB
    /* DB */
    u32ByteSize = VDP_PQ_REG_SIZE(V0_DB_CTRL, V0_DB_CTRL);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_DB_CTRL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_DB_CTRL), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_DB_LUT34, V0_DB_LUT40);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_DB_LUT34),
                         (HI_VOID *)(&pSrcVdpAddr->V0_DB_LUT34), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_DB_LUT44, V0_DB_THR3);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_DB_LUT44),
                         (HI_VOID *)(&pSrcVdpAddr->V0_DB_LUT44), u32ByteSize / 4);

    u32ByteSize = VDP_PQ_REG_SIZE(V0_DBD_THDEDGE, V0_DBD_BLKSIZE);
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_DBD_THDEDGE),
                         (HI_VOID *)(&pSrcVdpAddr->V0_DBD_THDEDGE), u32ByteSize / 4);
#endif
#ifdef PQ_ALG_TNR
    /* TNR */
    u32ByteSize = VDP_PQ_REG_SIZE(V0_TNR_MODE, V0_TNR_TE_YCMT_MAPPING_K2 );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_TNR_MODE),
                         (HI_VOID *)(&pSrcVdpAddr->V0_TNR_MODE), u32ByteSize / 4);
    u32ByteSize = VDP_PQ_REG_SIZE(V0_TNR_TE_YCEG_MAPPING_CTRL, V0_TNR_EDGE_CMOVING_CK );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_TNR_TE_YCEG_MAPPING_CTRL),
                         (HI_VOID *)(&pSrcVdpAddr->V0_TNR_TE_YCEG_MAPPING_CTRL), u32ByteSize / 4);
    u32ByteSize = VDP_PQ_REG_SIZE(V0_TNR_CLUT10, V0_TNR_CLUT67 );
    PQ_HAL_CopyRegBySize((HI_VOID *)(&pDstVdpAddr->V0_TNR_CLUT10),
                         (HI_VOID *)(&pSrcVdpAddr->V0_TNR_CLUT10), u32ByteSize / 4);
#endif

#if defined(CHIP_TYPE_hi3796mv200)
    u32ByteSize = 0;
#endif

    return;
}

/**
 \brief 更新VPSS寄存器;
 \attention \n
  无

 \param[in] u32HandleNo  :VPSS通道号
 \param[in] *pstVPSSReg  :由驱动传过来的寄存器指针

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdateVpssReg(HI_U32 u32HandleNo, S_CAS_REGS_TYPE *pstVPSSReg, S_VPSSWB_REGS_TYPE *pstVPSSWbcReg)
{
    HI_BOOL bReload = HI_FALSE;

    if (u32HandleNo >= VPSS_HANDLE_NUM)
    {
        HI_ERR_PQ("VPSS HandleNo[%d] over range!\n", u32HandleNo);
        return HI_FAILURE;
    }

    if (sg_pstVPSSWbcRegMem[u32HandleNo] != pstVPSSWbcReg)
    {
        sg_pstVPSSWbcRegMem[u32HandleNo] = pstVPSSWbcReg;
    }

    if (sg_pstVPSSRegMem[u32HandleNo] != pstVPSSReg)
    {
        sg_pstVPSSRegMem[u32HandleNo] = pstVPSSReg;
        bReload = HI_TRUE;
    }

    if (sg_pstVPSSRegMem[u32HandleNo] != NULL && bReload == HI_TRUE)
    {
        PQ_HAL_UpdateVpssPQ(sg_pstVPSSRegMem[u32HandleNo], sg_pstVPSSRegLocal);
    }

    return HI_SUCCESS;
}

/**
 \brief 更新VDP寄存器;
 \attention \n
  无

 \param[in] *pstVDPReg  :由驱动传过来的VPSS寄存器指针

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdateVdpReg(S_VDP_REGS_TYPE *pstVDPReg)
{
    HI_BOOL bReload = HI_FALSE;

    if (sg_pstVDPRegMem != pstVDPReg)
    {
        sg_pstVDPRegMem = pstVDPReg;
        bReload = HI_TRUE;
    }

    if (sg_pstVDPRegMem != NULL && bReload == HI_TRUE)
    {
        PQ_HAL_UpdateVdpPQ(sg_pstVDPRegMem, sg_pstVDPRegLocal);
    }

    return HI_SUCCESS;
}

/**
 \brief 检查VPSS当前u32HandleNo对应虚拟寄存器是否有效;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::HI_BOOL

 */
HI_BOOL PQ_HAL_CheckVpssValid(HI_U32 u32HandleNo)
{
    if (u32HandleNo >= VPSS_HANDLE_NUM)
    {
        HI_ERR_PQ("VPSS HandleNo=%d is over range!\n", u32HandleNo);
        return HI_FALSE;
    }

    if (sg_pstVPSSRegMem[u32HandleNo] == NULL)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/**
 \brief 检查VDP寄存器是否有效;
 \attention \n
  无

 \param[in] none

 \retval ::HI_BOOL

 */
HI_BOOL PQ_HAL_CheckVdpValid(HI_VOID)
{
    if (sg_pstVDPRegMem == NULL)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/**
 \brief 获取VPSS当前寄存器;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::VPSS寄存器指针

 */
S_CAS_REGS_TYPE *PQ_HAL_GetVpssReg(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE *pstVPSSReg = HI_NULL;

    if (u32HandleNo >= VPSS_HANDLE_NUM)
    {
        HI_ERR_PQ("VPSS HandleNo=%d is over range!\n", u32HandleNo);
        return NULL;
    }

    if (sg_pstVPSSRegMem[u32HandleNo] == NULL)
    {
        pstVPSSReg = sg_pstVPSSRegLocal;
    }
    else
    {
        pstVPSSReg = sg_pstVPSSRegMem[u32HandleNo];
    }

    return pstVPSSReg;
}


/**
 \brief 获取VPSS WBC INFO;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::VPSS WBC INFO

 */
S_VPSSWB_REGS_TYPE *PQ_HAL_GetVpssWbcReg(HI_U32 u32HandleNo)
{
    S_VPSSWB_REGS_TYPE *pstVPSSWbcReg = HI_NULL;

    if (u32HandleNo >= VPSS_HANDLE_NUM)
    {
        HI_ERR_PQ("VPSS HandleNo=%d is over range!\n", u32HandleNo);
        return NULL;
    }

    if (sg_pstVPSSWbcRegMem[u32HandleNo] != HI_NULL)
    {
        pstVPSSWbcReg = sg_pstVPSSWbcRegMem[u32HandleNo];
    }
    else
    {
        HI_ERR_PQ("ERROR: VPSS Wbc REG is null point!\n");
    }

    return pstVPSSWbcReg;
}


/**
 \brief 获取VDP当前寄存器;
 \attention \n
  无

 \param[in] none

 \retval ::VDP寄存器指针

 */
S_VDP_REGS_TYPE *PQ_HAL_GetVdpReg(HI_VOID)
{
    S_VDP_REGS_TYPE *pstVDPReg = NULL;

    if (sg_pstVDPRegMem == NULL)
    {
        pstVDPReg = sg_pstVDPRegLocal;
    }
    else
    {
        pstVDPReg = sg_pstVDPRegMem;
    }

    return pstVDPReg;
}


/**
 \brief 写寄存器;
 \attention \n
  无

 \param[in] u32HandleNo
 \param[in] u32RegAddr
 \param[in] u32Value

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_WriteRegister(HI_U32 u32HandleNo, HI_U32 u32RegAddr, HI_U32 u32Value)
{
    S_CAS_REGS_TYPE *pstVPSSReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    HI_U32 u32OffsetAddr = 0x0;

    u32OffsetAddr = u32RegAddr & REG_OFFSET_ADDR_MASK;

    if ((PQ_HAL_IsVpssReg(u32RegAddr) == HI_TRUE) && (u32HandleNo < VPSS_HANDLE_NUM))
    {
        pstVPSSReg = PQ_HAL_GetVpssReg(u32HandleNo);

        if (pstVPSSReg == HI_NULL)
        {
            HI_ERR_PQ("VPSS REG is null point!\n");
            return HI_FAILURE;
        }

        *(HI_U32 *)((HI_VOID *)pstVPSSReg + u32OffsetAddr) = u32Value;
    }
    else if (PQ_HAL_IsVdpReg(u32RegAddr) == HI_TRUE)
    {
        pstVDPReg = PQ_HAL_GetVdpReg();

        if (pstVDPReg == HI_NULL)
        {
            HI_ERR_PQ("VDP REG is null point!\n");
            return HI_FAILURE;
        }

        *(HI_U32 *)((HI_VOID *)pstVDPReg + u32OffsetAddr) = u32Value;
    }
    else
    {}

    return HI_SUCCESS;
}

/* 读寄存器 */
HI_S32 PQ_HAL_ReadRegister(HI_U32 u32HandleNo, HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    S_CAS_REGS_TYPE *pstVPSSReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    HI_U32 u32OffsetAddr = 0x0;

    u32OffsetAddr = u32RegAddr & REG_OFFSET_ADDR_MASK;

    if ((PQ_HAL_IsVpssReg(u32RegAddr) == HI_TRUE) && (u32HandleNo < VPSS_HANDLE_NUM))
    {
        pstVPSSReg = PQ_HAL_GetVpssReg(u32HandleNo);

        if (pstVPSSReg == NULL)
        {
            HI_ERR_PQ("VPSS REG is null point!\n");
            return HI_FAILURE;
        }

        *pu32Value = *((HI_U32 *)((HI_VOID *)pstVPSSReg + u32OffsetAddr));
    }
    else if (PQ_HAL_IsVdpReg(u32RegAddr) == HI_TRUE)
    {
        pstVDPReg = PQ_HAL_GetVdpReg();

        if (pstVDPReg == NULL)
        {
            HI_ERR_PQ("VDP REG is null point!\n");
            return HI_FAILURE;
        }

        *pu32Value = *((HI_U32 *)((HI_VOID *)pstVDPReg + u32OffsetAddr));
    }
    else
    {}

    return HI_SUCCESS;
}

/* 按照起始BIT位复制U32参数 */
HI_VOID PQ_HAL_CopyU32ByBit(HI_U32 u32Src, HI_U32 u32SrcStartBit, HI_U32 *pu32Dst, HI_U32 u32DstStartBit, HI_U32 u32Num)
{
    HI_U32 u32SrcTmp = 0;
    HI_U32 u32DstTmp = 0;
    HI_U32 u32DstTmpLow  = 0;
    HI_U32 u32DstTmpHigh = 0;

    if (((u32SrcStartBit + u32Num) > 32) || (u32DstStartBit + u32Num) > 32)
    {
        HI_ERR_PQ("param is illegal!!\n");
        return;
    }

    /* 保存src需要拷贝的u32Num个bits，并移动到dst指定的位置; 将高位清0 */
    u32SrcTmp = u32Src << (32 - u32SrcStartBit - u32Num);

    /* 将指定的u32Num位移到低bit位上 */
    u32SrcTmp = u32SrcTmp >> (32 - u32Num);
    u32SrcTmp = u32SrcTmp << u32DstStartBit;

    /* 保存dst的0bit到u32DstStartBit位 */
    u32DstTmpLow = *pu32Dst << (32 - u32DstStartBit); /* 将高位清0 */
    u32DstTmpLow = u32DstTmpLow >> (32 - u32DstStartBit);

    /* 保存dst的u32DstStartBit + u32Num位到32bit */
    u32DstTmpHigh = *pu32Dst >> (u32DstStartBit + u32Num);
    u32DstTmpHigh = u32DstTmpHigh << (u32DstStartBit + u32Num);

    /* 计算结果 */
    u32DstTmp = u32DstTmpHigh | u32SrcTmp | u32DstTmpLow;
    *pu32Dst = u32DstTmp;

    return ;
}

/**
 \brief 根据bit位更新U32参数
 \attention \n
  无

 \param[in] pulData,ucBitNo;

 \retval ::HI_SUCCESS

 */
HI_VOID  PQ_HAL_U32SetBit( HI_U32 *pulData, HI_U8 ucBitNo)
{
    HI_U32 ulBitsMask, ulData;

    if (ucBitNo > 31)
    {
        HI_ERR_PQ("PQ_MNG_U32SetBit bit over range!\n");
        return;
    }

    /* Bits Mask */
    ulBitsMask = 0x00000001;
    ulBitsMask = ulBitsMask << ucBitNo;

    ulData   = *pulData;
    *pulData = ulData | ulBitsMask;
}

/**
 \brief 将U32对应的bit位清零
 \attention \n
  无

 \param[in] pulData,ucBitNo;

 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_U32ClearBit( HI_U32 *pulData, HI_U8 ucBitNo)
{
    HI_U32 ulBitsMask, ulData;

    if (ucBitNo > 31)
    {
        HI_ERR_PQ("PQ_MNG_U32ClearBit bit over range!\n");
        return;
    }

    /* Bits Mask */
    ulBitsMask = 0x00000001;
    ulBitsMask = ulBitsMask << ucBitNo;

    ulData   = *pulData;
    *pulData = ulData & (~ulBitsMask);
}

/**
 \brief 从U32参数根据最高位和最低获取对应的值
 \attention \n
  无

 \param[in]  ulData,  ucMaxBit,  ucMinBit;

 \retval ::HI_U32

 */
HI_U32 PQ_HAL_GetU32ByBit( HI_U32 ulData, HI_U8 ucMaxBit, HI_U8 ucMinBit)
{
    HI_U8 ucFlagEr = HI_FALSE;

    if (ucMaxBit < ucMinBit)
    {
        ucFlagEr = HI_TRUE;
    }

    if (ucMaxBit > 31 || ucMinBit > 31)
    {
        ucFlagEr = HI_TRUE;
    }

    if (ucFlagEr == HI_TRUE)
    {
        HI_ERR_PQ("NTDRIVER_BITS_u32GetData bit over range!\n");
        return ucFlagEr;
    }

    ulData = ulData << (31 - ucMaxBit);
    ulData = ulData >> (31 - ucMaxBit);
    ulData = (ulData >> ucMinBit);
    return ulData;
}

/**
 \brief 根据最高位和最低位设置U32参数对应的值
 \attention \n
  无

 \param[in] pulData, ucMaxBit, ucMinBit, ulValue;

 \retval ::none

 */
HI_VOID PQ_HAL_SetU32ByBit( HI_U32 *pulData, HI_U8 ucMaxBit, HI_U8 ucMinBit, HI_U32 ulValue)
{
    HI_U8 ucFlagEr = HI_FALSE;
    HI_U32 ulBitsMask, ulData;

    if (ucMaxBit < ucMinBit)
    {
        ucFlagEr = HI_TRUE;
    }

    if (ucMaxBit > 31 || ucMinBit > 31)
    {
        ucFlagEr = HI_TRUE;
    }

    if (ucFlagEr == HI_TRUE)
    {
        HI_ERR_PQ("bit over range:maxbit[%d],minbit[%d]\n", ucMaxBit, ucMinBit);
        return;
    }

    /* Bits Mask */
    ulBitsMask = 0xFFFFFFFF;
    ulBitsMask = ulBitsMask >> ucMinBit;
    ulBitsMask = ulBitsMask << (31 - ucMaxBit + ucMinBit);
    ulBitsMask = ulBitsMask >> (31 - ucMaxBit);

    ulData   = *pulData;
    ulData   = ulData & (~ulBitsMask);
    ulValue  = (ulValue << ucMinBit) & ulBitsMask;
    *pulData = ulData | ulValue;
}

HI_BOOL PQ_HAL_IsVpssReg(HI_U32 u32RegAddr)
{
    if ((u32RegAddr & REG_BASE_ADDR_MASK) == VPSS_REGS_ADDR)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL PQ_HAL_IsVdpReg(HI_U32 u32RegAddr)
{
    if ((u32RegAddr & REG_BASE_ADDR_MASK) == VDP_REGS_ADDR)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL PQ_HAL_IsSpecialReg(HI_U32 u32RegAddr)
{
    if ((u32RegAddr & REG_BASE_ADDR_MASK) == SPECIAL_REGS_ADDR)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 PQ_HAL_PrintMsg(HI_U32 type, const HI_S8 *format, ...)
{
    va_list args;
    HI_S32  nTotalChar = 0;

    /* 信息类型过滤 */
    if ((PQ_PRN_ALWS != type) && (0 == (sg_PrintType & type))) /* 该类别打印没有打开 */
    {
        return HI_FAILURE;
    }

    /* 将信息打印成字符串 */
    va_start( args, format );
    nTotalChar = HI_OSAL_Vsnprintf( sg_PrintMsg, sizeof(sg_PrintMsg), format, args );
    va_end( args );

    if ((nTotalChar <= 0) || (nTotalChar >= 1023))
    {
        return HI_FAILURE;
    }

#ifndef  HI_ADVCA_FUNCTION_RELEASE
    HI_PRINT("%s", sg_PrintMsg);
#endif

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetPrintType(HI_U32 type)
{
    sg_PrintType = type;
    return HI_SUCCESS;
}

#else
S_VDP_REGS_TYPE *PQ_HAL_GetVdpReg(HI_VOID)
{
    S_VDP_REGS_TYPE *pstVDPReg = NULL;

    pstVDPReg = (S_VDP_REGS_TYPE *)VDP_REGS_ADDR;

    return pstVDPReg;
}
#endif

HI_VOID PQ_HAL_RegWrite(volatile HI_U32 *a, HI_U32 value)
{
    *a = value;

    return;
}

HI_U32 PQ_HAL_RegRead(volatile HI_U32 *a)
{
    return (*a);
}


HI_S32 PQ_HAL_AllocAndMap(const char *bufname, char *zone_name, HI_U32 u32Size, int align, PQ_MMZ_BUF_S *pstMBuf)
{
    HI_S32 s32Ret;

#ifndef HI_BUILD_IN_BOOT
#ifdef  HI_PQ_SMMU_SUPPORT
    SMMU_BUFFER_S stMMU;
    s32Ret = HI_DRV_SMMU_AllocAndMap( bufname,
                                      u32Size, 0, &stMMU);

    if (s32Ret == HI_SUCCESS)
    {

        pstMBuf->u32StartPhyAddr = stMMU.u32StartSmmuAddr;
        pstMBuf->pu8StartVirAddr = stMMU.pu8StartVirAddr;
        pstMBuf->u32Size = stMMU.u32Size;
    }

#else
    MMZ_BUFFER_S stMMZ;

    s32Ret = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, u32Size, align, &stMMZ);

    if (!s32Ret)
    {
        pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->pu8StartVirAddr = stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size         = stMMZ.u32Size;
    }

#endif
#else

    if (HI_SUCCESS == HI_DRV_PDM_AllocReserveMem(bufname, u32Size, &pstMBuf->u32StartPhyAddr))
    {
        pstMBuf->pu8StartVirAddr = (HI_U8 *)pstMBuf->u32StartPhyAddr; /*to do*/
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }

#endif

    return s32Ret;
}


HI_VOID PQ_HAL_UnmapAndRelease(PQ_MMZ_BUF_S *pstMBuf)
{
#ifndef HI_BUILD_IN_BOOT

#ifdef HI_PQ_SMMU_SUPPORT
    SMMU_BUFFER_S stMMU;

    stMMU.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
    stMMU.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
    stMMU.u32Size = pstMBuf->u32Size;
    HI_DRV_SMMU_UnmapAndRelease(&stMMU);
#else
    MMZ_BUFFER_S stMMZ;

    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
    stMMZ.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
    stMMZ.u32Size         = pstMBuf->u32Size;

    HI_DRV_MMZ_UnmapAndRelease(&stMMZ);
#endif

#endif

    return;
}

HI_S32 PQ_HAL_MMZ_AllocAndMap(const HI_CHAR *pchBufName,
                              HI_CHAR *pchZoneName,
                              HI_U32 u32Size,
                              HI_S32 s32Align,
                              PQ_MMZ_BUF_S *pstMBuf)
{
    HI_S32 s32Ret = HI_FAILURE;
#ifndef HI_BUILD_IN_BOOT
    MMZ_BUFFER_S stMMZ;

    s32Ret = HI_DRV_MMZ_AllocAndMap(pchBufName, pchZoneName, u32Size, s32Align, &stMMZ);
    if (HI_SUCCESS == s32Ret)
    {
        pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->pu8StartVirAddr = stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size         = stMMZ.u32Size;
    }
#else
    s32Ret = HI_DRV_PDM_AllocReserveMem(pchBufName, u32Size, &pstMBuf->u32StartPhyAddr);
    if (HI_SUCCESS == s32Ret)
    {
        pstMBuf->pu8StartVirAddr = (HI_U8 *)pstMBuf->u32StartPhyAddr;
    }
#endif

    return s32Ret;
}

HI_VOID PQ_HAL_MMZ_UnmapAndRelease(PQ_MMZ_BUF_S *pstMBuf)
{
#ifndef HI_BUILD_IN_BOOT
    MMZ_BUFFER_S stMMZ;

    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
    stMMZ.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
    stMMZ.u32Size         = pstMBuf->u32Size;
    HI_DRV_MMZ_UnmapAndRelease(&stMMZ);
#endif

    return;
}

