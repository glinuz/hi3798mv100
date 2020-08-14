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
  Author        : p00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#include "hi_drv_mem.h"
#include "pq_hal_comm.h"
#include "hi_osal.h"


#define VPSS_REGS_ADDR    0xf8cb0000UL
#define VDP_REGS_ADDR     0xf8cc0000UL
#define SPECIAL_REGS_ADDR 0xfffe0000UL

#define VPSS_PQ_REG_SIZE(start, end)\
    (offsetof(S_CAS_REGS_TYPE, end) + sizeof(HI_U32) -\
     offsetof(S_CAS_REGS_TYPE, start))

#define VDP_PQ_REG_SIZE(start, end)\
    (offsetof(S_VDP_REGS_TYPE, end) + sizeof(HI_U32) -\
     offsetof(S_VDP_REGS_TYPE, start))


/* VPSS/VDP 本地寄存器，用于初始化参数*/
static S_CAS_REGS_TYPE* sg_pstVPSSRegLocal = NULL;
static S_VDP_REGS_TYPE* sg_pstVDPRegLocal  = NULL;

/* VPSS/VDP 虚拟寄存器，用于配置参数*/
static S_CAS_REGS_TYPE*    sg_pstVPSSRegMem[VPSS_HANDLE_NUM]    = {NULL};
static S_VPSSWB_REGS_TYPE* sg_pstVPSSWbcRegMem[VPSS_HANDLE_NUM] = {NULL};
static S_VDP_REGS_TYPE*    sg_pstVDPRegMem = NULL;

/* VPSS模块开关参数*/
static PQ_HAL_MODULE_S* sg_pstPQModuleLocal = NULL;
static PQ_HAL_MODULE_S* sg_pstPQModuleMem   = NULL;

/* PQ HAL初始化标志位*/
static HI_BOOL sg_bHALInit = HI_FALSE;

/* 打印使能开关 */
static HI_U32 sg_PrintType = PQ_PRN_NOTHING;

/* 打印中间结果的暂存区域 */
static HI_S8  sg_PrintMsg[1024];


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
#ifndef  __PQ_PLATFORM_BOOT__

    sg_pstVPSSRegLocal = (S_CAS_REGS_TYPE*)HI_KMALLOC(HI_ID_PQ, sizeof(S_CAS_REGS_TYPE), GFP_KERNEL);
    if (sg_pstVPSSRegLocal == HI_NULL)
    {
        HI_ERR_PQ("sg_pstVPSSRegLocal can not kmalloc!\n");
        return HI_FAILURE;
    }
    memset(sg_pstVPSSRegLocal, 0, sizeof(S_CAS_REGS_TYPE));

    sg_pstVDPRegLocal = (S_VDP_REGS_TYPE*)HI_KMALLOC(HI_ID_PQ, sizeof(S_VDP_REGS_TYPE), GFP_KERNEL);
    if (sg_pstVDPRegLocal == HI_NULL)
    {
        HI_ERR_PQ("sg_pstVDPRegLocal can not kmalloc!\n");
        return HI_FAILURE;
    }
    memset(sg_pstVDPRegLocal, 0, sizeof(S_VDP_REGS_TYPE));

    sg_pstPQModuleLocal = (PQ_HAL_MODULE_S*)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_HAL_MODULE_S), GFP_KERNEL);
    if (sg_pstPQModuleLocal == HI_NULL)
    {
        HI_ERR_PQ("sg_pstPQModuleLocal can not kmalloc!\n");
        return HI_FAILURE;
    }
    memset(sg_pstPQModuleLocal, 0, sizeof(PQ_HAL_MODULE_S));
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
#ifndef  __PQ_PLATFORM_BOOT__

    HI_KFREE(HI_ID_PQ, sg_pstVPSSRegLocal);
    HI_KFREE(HI_ID_PQ, sg_pstVDPRegLocal);
    HI_KFREE(HI_ID_PQ, sg_pstPQModuleLocal);
#endif
    sg_pstVPSSRegLocal  = NULL;
    sg_pstVDPRegLocal   = NULL;
    sg_pstPQModuleLocal = NULL;

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        sg_pstVPSSRegMem[i]    = NULL;
        sg_pstVPSSWbcRegMem[i] = NULL;
    }
    sg_pstVDPRegMem   = NULL;
    sg_pstPQModuleMem = NULL;

    sg_bHALInit = HI_FALSE;

    return HI_SUCCESS;
}


/**
 \brief 更新VPSS PQ;
 \attention \n
  无

 \param[in] *pDstVpssAddr  : 目标地址
 \param[in] *pSrcVpssAddr  :源地址
 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_UpdateVpssPQ(S_CAS_REGS_TYPE* pDstVpssAddr, S_CAS_REGS_TYPE* pSrcVpssAddr)
{
    HI_U32 u32RegSize;

    /* DEI */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DIELMA2, VPSS_DIEDIR12_14 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DIELMA2),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DIELMA2), u32RegSize);

    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DIEINTPSCL0, VPSS_CCRSCLR1);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DIEINTPSCL0),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DIEINTPSCL0), u32RegSize);

#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    /* iglb_en */
    pDstVpssAddr->VPSS_CTRL.bits.iglb_en = pSrcVpssAddr->VPSS_CTRL.bits.iglb_en;

    /* FMD */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_PDPHISTTHD1, VPSS_PDREGION );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_PDPHISTTHD1),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_PDPHISTTHD1), u32RegSize);

    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c;
    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_l      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_c      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_c;

    /* DR */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DR_CTRL, VPSS_DROVTSTRLUT2 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DR_CTRL),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DR_CTRL), u32RegSize);

    /* DB */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DB_CTRL, VPSS_DB_LUT10 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DB_CTRL),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DB_CTRL), u32RegSize);
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DBD_THDEDGE, VPSS_DBD_BLKSIZE );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DBD_THDEDGE),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DBD_THDEDGE), u32RegSize);
    //u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DB_BORD_FLAG[0], VPSS_DB_BORD_FLAG[127] );
    u32RegSize = sizeof(pSrcVpssAddr->VPSS_DB_BORD_FLAG);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DB_BORD_FLAG[0]),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DB_BORD_FLAG[0]), u32RegSize);

    /* SD DR */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DR_CFG0, VPSS_DNR_INFO);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DR_CFG0),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DR_CFG0), u32RegSize);

    /* VPSS CTRL DNR DB DR   dr_en\db_en\SD dr_en control by vpss*/
    /*
    pDstVpssAddr->VPSS_CTRL.bits.dnr_en = pSrcVpssAddr->VPSS_CTRL.bits.dnr_en;
    pDstVpssAddr->VPSS_CTRL.bits.db_en  = pSrcVpssAddr->VPSS_CTRL.bits.db_en;
    pDstVpssAddr->VPSS_CTRL.bits.dr_en  = pSrcVpssAddr->VPSS_CTRL.bits.dr_en;
    */

    /* HSHARPEN */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_HSPCFG1, VPSS_HSPCFG2);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_HSPCFG1),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_HSPCFG1), u32RegSize);
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_HSPCFG3, VPSS_HSPCFG6);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_HSPCFG3),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_HSPCFG3), u32RegSize);
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_HSPCFG7, VPSS_HSPCFG10);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_HSPCFG7),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_HSPCFG7), u32RegSize);

    //pDstVpssAddr->VPSS_CTRL.bits.hsp_en = pSrcVpssAddr->VPSS_CTRL.bits.hsp_en;
#endif

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    /* iglb_en */
    pDstVpssAddr->VPSS_CTRL.bits.iglb_en = pSrcVpssAddr->VPSS_CTRL.bits.iglb_en;
    /* TNR */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_TNR_MODE, VPSS_TNR_TE_YCMT_MAPPING_K2 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_TNR_MODE),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_TNR_MODE), u32RegSize);
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_TNR_TE_YCEG_MAPPING_CTRL, VPSS_TNR_EDGE_CMOVING_CK );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_TNR_TE_YCEG_MAPPING_CTRL),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_TNR_TE_YCEG_MAPPING_CTRL), u32RegSize);
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_TNR_CLUT10, VPSS_TNR_CLUT67 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_TNR_CLUT10),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_TNR_CLUT10), u32RegSize);

    /* FMD */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_PDPHISTTHD1, VPSS_PDREGION );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_PDPHISTTHD1),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_PDPHISTTHD1), u32RegSize);

    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c;
    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_l      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_c      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_c;

    /* DM */
    pDstVpssAddr->VPSS_DB_CTRL.bits.dm_en = pSrcVpssAddr->VPSS_DB_CTRL.bits.dm_en;

    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DM_DIR, VPSS_DM_LUT4 );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DM_DIR),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DM_DIR), u32RegSize);

    /* DB */
    pDstVpssAddr->VPSS_DB_CTRL.bits.db_lum_ver_en          = pSrcVpssAddr->VPSS_DB_CTRL.bits.db_lum_ver_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.db_lum_hor_en          = pSrcVpssAddr->VPSS_DB_CTRL.bits.db_lum_hor_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.db_chr_ver_en          = pSrcVpssAddr->VPSS_DB_CTRL.bits.db_chr_ver_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.db_chr_hor_en          = pSrcVpssAddr->VPSS_DB_CTRL.bits.db_chr_hor_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.db_chr_hor_mid_grey_en = pSrcVpssAddr->VPSS_DB_CTRL.bits.db_chr_hor_mid_grey_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.dbm_demo_en            = pSrcVpssAddr->VPSS_DB_CTRL.bits.dbm_demo_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.dbm_demo_mode          = pSrcVpssAddr->VPSS_DB_CTRL.bits.dbm_demo_mode;
    pDstVpssAddr->VPSS_DB_CTRL.bits.det_size_en            = pSrcVpssAddr->VPSS_DB_CTRL.bits.det_size_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.det_hy_en              = pSrcVpssAddr->VPSS_DB_CTRL.bits.det_hy_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.det_hc_en              = pSrcVpssAddr->VPSS_DB_CTRL.bits.det_hc_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.det_vy_en              = pSrcVpssAddr->VPSS_DB_CTRL.bits.det_vy_en;
    pDstVpssAddr->VPSS_DB_CTRL.bits.dbm_out_mode           = pSrcVpssAddr->VPSS_DB_CTRL.bits.dbm_out_mode;
    pDstVpssAddr->VPSS_DB_CTRL.bits.dbm_out_sel_gain       = pSrcVpssAddr->VPSS_DB_CTRL.bits.dbm_out_sel_gain;

    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DB_DIR, VPSS_DBD_BLKSIZE );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DB_DIR),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DB_DIR), u32RegSize);

    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DB_BORD_FLAG[0], VPSS_DB_BORD_FLAG[128] );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DB_BORD_FLAG),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DB_BORD_FLAG), u32RegSize);

    /* VPSS CTRL DB DM dbm_en control by vpss*/
    /*
    pDstVpssAddr->VPSS_CTRL.bits.dbm_en = pSrcVpssAddr->VPSS_CTRL.bits.dbm_en;
    */
#endif

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    /* FMD */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_PDPHISTTHD1, VPSS_PDREGION );
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_PDPHISTTHD1),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_PDPHISTTHD1), u32RegSize);

    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_c;
    pDstVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l = pSrcVpssAddr->VPSS_DIECTRL.bits.die_out_sel_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_l      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_l;
    pDstVpssAddr->VPSS_PDCTRL.bits.dir_mch_c      = pSrcVpssAddr->VPSS_PDCTRL.bits.dir_mch_c;

    /* DR */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DR_CFG0, VPSS_DR_CFG1);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DR_CFG0),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DR_CFG0), u32RegSize);

    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_DNR_INFO, VPSS_DNR_INFO);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_DNR_INFO),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_DNR_INFO), u32RegSize);

    pDstVpssAddr->VPSS_CTRL.bits.dr_en = pSrcVpssAddr->VPSS_CTRL.bits.dr_en;

    /* SHARPEN:LTI,CTI */
    u32RegSize = VPSS_PQ_REG_SIZE(VPSS_VHD_LTI_CTRL, VPSS_VHD_CTI_THD);
    memcpy((HI_VOID*)(&pDstVpssAddr->VPSS_VHD_LTI_CTRL),
           (HI_VOID*)(&pSrcVpssAddr->VPSS_VHD_LTI_CTRL), u32RegSize);
#endif
}


/**
 \brief 更新VDP PQ;
 \attention \n
  无

 \param[in] *pDstVdpAddr  : 目标地址
 \param[in] *pSrcVdpAddr  :源地址
 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_UpdateVdpPQ(S_VDP_REGS_TYPE* pDstVdpAddr, S_VDP_REGS_TYPE* pSrcVdpAddr)
{
    HI_U32 u32RegSize;

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


    /* ACM */
    u32RegSize = VDP_PQ_REG_SIZE(VP0_ACM_CTRL, VP0_ACM_ADJ);
    memcpy((HI_VOID*)(&pDstVdpAddr->VP0_ACM_CTRL),
           (HI_VOID*)(&pSrcVdpAddr->VP0_ACM_CTRL), u32RegSize);
    pDstVdpAddr->VP0_ACM_CAD.bits.coef_addr = pSrcVdpAddr->VP0_ACM_CAD.bits.coef_addr;
    pDstVdpAddr->VP0_PARAUP.bits.vp0_acmcoef_upd = pSrcVdpAddr->VP0_PARAUP.bits.vp0_acmcoef_upd;

    /* DCI */
    u32RegSize = VDP_PQ_REG_SIZE(VP0_DCICTRL, VP0_DCIADJSW10);
    memcpy((HI_VOID*)(&pDstVdpAddr->VP0_DCICTRL),
           (HI_VOID*)(&pSrcVdpAddr->VP0_DCICTRL), u32RegSize);
    pDstVdpAddr->VP0_DCI_CAD.bits.coef_addr = pSrcVdpAddr->VP0_DCI_CAD.bits.coef_addr;
    pDstVdpAddr->VP0_PARAUP.bits.vp0_acccoef_upd = pSrcVdpAddr->VP0_PARAUP.bits.vp0_acccoef_upd;

#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    /* SR */
    u32RegSize = VDP_PQ_REG_SIZE(SR_CTRL, SR_SMOOTH);
    memcpy((HI_VOID*)(&pDstVdpAddr->SR_CTRL),
           (HI_VOID*)(&pSrcVdpAddr->SR_CTRL), u32RegSize);
    u32RegSize = VDP_PQ_REG_SIZE(SR_RINGDT0, SR_DIR_CTRL);
    memcpy((HI_VOID*)(&pDstVdpAddr->SR_RINGDT0),
           (HI_VOID*)(&pSrcVdpAddr->SR_RINGDT0), u32RegSize);
#endif

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    /* SR */
    u32RegSize = VDP_PQ_REG_SIZE(SR_CTRL, SR_SMOOTH);
    memcpy((HI_VOID*)(&pDstVdpAddr->SR_CTRL),
           (HI_VOID*)(&pSrcVdpAddr->SR_CTRL), u32RegSize);
#endif

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    /* Sharpen */
    u32RegSize = VDP_PQ_REG_SIZE(SPCTRL, SPEDGEEI);
    memcpy((HI_VOID*)(&pDstVdpAddr->SPCTRL),
           (HI_VOID*)(&pSrcVdpAddr->SPCTRL), u32RegSize);
#endif

    return;
}

/**
 \brief 更新VPSS寄存器;
 \attention \n
  无

 \param[in] u32HandleNo  :VPSS通道号
 \param[in] *pstVPSSReg  :  由驱动传过来的寄存器指针

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdateVpssReg(HI_U32 u32HandleNo, S_CAS_REGS_TYPE* pstVPSSReg, S_VPSSWB_REGS_TYPE* pstVPSSWbcReg)
{
    HI_BOOL bReload = HI_FALSE;

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
HI_S32 PQ_HAL_UpdateVdpReg(S_VDP_REGS_TYPE* pstVDPReg)
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
 \brief 更新PQ Module开关参数;
 \attention \n
  无

 \param[in] *pstPQModule

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdatePQModule(PQ_HAL_MODULE_S* pstPQModule)
{
    HI_BOOL bReload = HI_FALSE;

    if (sg_pstPQModuleMem != pstPQModule)
    {
        sg_pstPQModuleMem = pstPQModule;
        bReload = HI_TRUE;
    }

    if (sg_pstPQModuleMem != NULL && bReload == HI_TRUE)
    {
        memcpy(sg_pstPQModuleMem, sg_pstPQModuleLocal, sizeof(PQ_HAL_MODULE_S));
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
S_CAS_REGS_TYPE* PQ_HAL_GetVpssReg(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVPSSReg = HI_NULL;

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
S_VPSSWB_REGS_TYPE* PQ_HAL_GetVpssWbcReg(HI_U32 u32HandleNo)
{
    S_VPSSWB_REGS_TYPE* pstVPSSWbcReg = HI_NULL;

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
S_VDP_REGS_TYPE* PQ_HAL_GetVdpReg(HI_VOID)
{
    S_VDP_REGS_TYPE* pstVDPReg = NULL;

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
 \brief 获取PQ Module开关参数;
 \attention \n
  无

 \param[in] none

 \retval ::PQ Module寄存器指针

 */
PQ_HAL_MODULE_S* PQ_HAL_GetPQModule(HI_VOID)
{
    PQ_HAL_MODULE_S* pstPQModule = NULL;

    if (sg_pstPQModuleMem == NULL)
    {
        pstPQModule = sg_pstPQModuleLocal;
    }
    else
    {
        pstPQModule = sg_pstPQModuleMem;
    }

    return pstPQModule;
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
    S_CAS_REGS_TYPE* pstVPSSReg = HI_NULL;
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
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
        *(HI_U32*)((HI_VOID*)pstVPSSReg + u32OffsetAddr) = u32Value;
    }
    else if (PQ_HAL_IsVdpReg(u32RegAddr) == HI_TRUE)
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        if (pstVDPReg == HI_NULL)
        {
            HI_ERR_PQ("VDP REG is null point!\n");
            return HI_FAILURE;
        }
        *(HI_U32*)((HI_VOID*)pstVDPReg + u32OffsetAddr) = u32Value;
    }
    else
    {}

    return HI_SUCCESS;
}

/* 读寄存器 */
HI_S32 PQ_HAL_ReadRegister(HI_U32 u32HandleNo, HI_U32 u32RegAddr, HI_U32* pu32Value)
{
    S_CAS_REGS_TYPE* pstVPSSReg = HI_NULL;
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
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
        *pu32Value = *((HI_U32*)((HI_VOID*)pstVPSSReg + u32OffsetAddr));
    }
    else if (PQ_HAL_IsVdpReg(u32RegAddr) == HI_TRUE)
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        if (pstVDPReg == NULL)
        {
            HI_ERR_PQ("VDP REG is null point!\n");
            return HI_FAILURE;
        }
        *pu32Value = *((HI_U32*)((HI_VOID*)pstVDPReg + u32OffsetAddr));
    }
    else
    {}

    return HI_SUCCESS;
}

/* 按照起始BIT位复制U32参数 */
HI_VOID PQ_HAL_CopyU32ByBit(HI_U32 u32Src, HI_U32 u32SrcStartBit, HI_U32* pu32Dst, HI_U32 u32DstStartBit, HI_U32 u32Num)
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
HI_VOID  PQ_HAL_U32SetBit( HI_U32* pulData, HI_U8 ucBitNo)
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
HI_VOID PQ_HAL_U32ClearBit( HI_U32* pulData, HI_U8 ucBitNo)
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
HI_VOID PQ_HAL_SetU32ByBit( HI_U32* pulData, HI_U8 ucMaxBit, HI_U8 ucMinBit, HI_U32 ulValue)
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
        HI_ERR_PQ("NTDRIVER_BITS_u32SetData bit over range!\n");
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

HI_U32 PQ_HAL_RegRead(volatile HI_U32 a)
{
    return (*((HI_U32*)a));
}

HI_VOID PQ_HAL_RegWrite(volatile HI_U32 a, HI_U32 value)
{
    *((HI_U32*)a) = value;

    return;
}

HI_S32 PQ_REG_RegWrite(volatile HI_U32* a, HI_U32 b)
{
    *a = b;
    return HI_SUCCESS;
}

HI_U32 PQ_REG_RegRead(volatile HI_U32* a)
{
    return (*(a));
}

HI_S32 PQ_HAL_PrintMsg(HI_U32 type, const HI_S8* format, ...)
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

HI_S32 PQ_HAL_GetVpssDitherEn(HI_U32 u32HandleNo, HI_BOOL* bOnOff)
{
#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *bOnOff = pstVpssVirReg->VPSS_VHD0CTRL.bits.vhd0_dither_en;
#endif
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetDnrDitherEn(HI_U32 u32HandleNo, HI_BOOL* bOnOff)
{
#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *bOnOff = pstVpssVirReg->VPSS_DNR_INFO.bits.dnr_dither_en;
#endif
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetVdpDitherEn(HI_BOOL* bOnOff)
{
#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;

    pstVdpReg = PQ_HAL_GetVdpReg();
    WBC_DHD0_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->WBC_DHD0_DITHER_CTRL.u32));/* VDP_LAYER_WBC_HD0 */
    *bOnOff = WBC_DHD0_DITHER_CTRL.bits.dither_en;
#endif
    return HI_SUCCESS;
}



