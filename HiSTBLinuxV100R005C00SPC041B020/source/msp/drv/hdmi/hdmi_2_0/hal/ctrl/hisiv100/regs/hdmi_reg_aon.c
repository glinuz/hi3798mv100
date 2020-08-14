// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_aon_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.1 
// History       :  w00271848 2015/11/28 16:01:42 Create file
// ******************************************************************************
#include "hi_type.h"
#include "hdmi_reg_aon.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif

/* Define the struct pointor of the module tx_aon_reg */
volatile S_tx_aon_reg_REGS_TYPE *goptx_aon_regAllReg = NULL;
int HDMI_TX_S_tx_aon_reg_REGS_TYPE_Init(void)
{
    if (goptx_aon_regAllReg)
    {
        return HI_SUCCESS;
    }
    
    goptx_aon_regAllReg        = (volatile S_tx_aon_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_aon_reg,4);

    if (!goptx_aon_regAllReg)
    {
        HDMI_ERR("goptx_aon_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_tx_aon_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_aon_regAllReg)
    {
        HDMI_IO_UNMAP(goptx_aon_regAllReg);
        goptx_aon_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}




//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_yearSet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_year
//  Input       : unsigned int utx_hw_year: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_yearSet(unsigned int utx_hw_year)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_info.bits.tx_hw_year = utx_hw_year;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_info.u32, "TX_HW_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_monthSet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_month
//  Input       : unsigned int utx_hw_month: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_monthSet(unsigned int utx_hw_month)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_info.bits.tx_hw_month = utx_hw_month;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_info.u32, "TX_HW_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_daySet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_day
//  Input       : unsigned int utx_hw_day: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_daySet(unsigned int utx_hw_day)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_info.bits.tx_hw_day = utx_hw_day;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_info.u32, "TX_HW_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_rtl_versionSet
//  Description : Set the value of the member TX_HW_VERSION.tx_rtl_version
//  Input       : unsigned int utx_rtl_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_rtl_versionSet(unsigned int utx_rtl_version)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_version.bits.tx_rtl_version = utx_rtl_version;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_version.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_version.u32, "TX_HW_VERSION");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_drv_versionSet
//  Description : Set the value of the member TX_HW_VERSION.tx_drv_version
//  Input       : unsigned int utx_drv_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_drv_versionSet(unsigned int utx_drv_version)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_version.bits.tx_drv_version = utx_drv_version;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_version.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_version.u32, "TX_HW_VERSION");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_reg_versionSet
//  Description : Set the value of the member TX_HW_VERSION.tx_reg_version
//  Input       : unsigned int utx_reg_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_reg_versionSet(unsigned int utx_reg_version)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_hw_version.bits.tx_reg_version = utx_reg_version;
    HDMITXRegWrite(pu32RegAddr, o_tx_hw_version.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_hw_version.u32, "TX_HW_VERSION");

    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqSet
//  Description : Set the value of the member TX_AON_RST_CTRL.tx_aon_soft_arst_req
//  Input       : unsigned int utx_aon_soft_arst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqSet(unsigned int utx_aon_soft_arst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_RST_CTRL o_tx_aon_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_RST_CTRL.u32);
    o_tx_aon_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_rst_ctrl.bits.tx_aon_soft_arst_req = utx_aon_soft_arst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_rst_ctrl.u32, "TX_AON_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_osc_div_cntSet
//  Description : Set the value of the member HOTPLUG_ST_CFG.osc_div_cnt
//  Input       : unsigned int uosc_div_cnt: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_osc_div_cntSet(unsigned int uosc_div_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_st_cfg.bits.osc_div_cnt = uosc_div_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32, "HOTPLUG_ST_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_soft_valueSet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_soft_value
//  Input       : unsigned int uhpd_soft_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_soft_valueSet(unsigned int uhpd_soft_value)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_st_cfg.bits.hpd_soft_value = uhpd_soft_value;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32, "HOTPLUG_ST_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlSet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_polarity_ctl
//  Input       : unsigned int uhpd_polarity_ctl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlSet(unsigned int uhpd_polarity_ctl)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_st_cfg.bits.hpd_polarity_ctl = uhpd_polarity_ctl;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32, "HOTPLUG_ST_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_override_enSet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_override_en
//  Input       : unsigned int uhpd_override_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_override_enSet(unsigned int uhpd_override_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_st_cfg.bits.hpd_override_en = uhpd_override_en;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32, "HOTPLUG_ST_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_fillter_enSet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_fillter_en
//  Input       : unsigned int uhpd_fillter_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_fillter_enSet(unsigned int uhpd_fillter_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_st_cfg.bits.hpd_fillter_en = uhpd_fillter_en;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_st_cfg.u32, "HOTPLUG_ST_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdSet
//  Description : Set the value of the member HOTPLUG_FILLTER_CFG.hpd_low_reshold
//  Input       : unsigned int uhpd_low_reshold: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdSet(unsigned int uhpd_low_reshold)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_FILLTER_CFG o_hotplug_fillter_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_FILLTER_CFG.u32);
    o_hotplug_fillter_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_fillter_cfg.bits.hpd_low_reshold = uhpd_low_reshold;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_fillter_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_fillter_cfg.u32, "HOTPLUG_FILLTER_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdSet
//  Description : Set the value of the member HOTPLUG_FILLTER_CFG.hpd_high_reshold
//  Input       : unsigned int uhpd_high_reshold: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdSet(unsigned int uhpd_high_reshold)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_FILLTER_CFG o_hotplug_fillter_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_FILLTER_CFG.u32);
    o_hotplug_fillter_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hotplug_fillter_cfg.bits.hpd_high_reshold = uhpd_high_reshold;
    HDMITXRegWrite(pu32RegAddr, o_hotplug_fillter_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hotplug_fillter_cfg.u32, "HOTPLUG_FILLTER_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_STATE_phy_rx_senseSet
//  Description : Set the value of the member TX_AON_STATE.phy_rx_sense
//  Input       : unsigned int uphy_rx_sense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_STATE_phy_rx_senseSet(unsigned int uphy_rx_sense)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_STATE o_tx_aon_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_STATE.u32);
    o_tx_aon_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_state.bits.phy_rx_sense = uphy_rx_sense;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_state.u32, "TX_AON_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_STATE_hotplug_stateSet
//  Description : Set the value of the member TX_AON_STATE.hotplug_state
//  Input       : unsigned int uhotplug_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_STATE_hotplug_stateSet(unsigned int uhotplug_state)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_STATE o_tx_aon_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_STATE.u32);
    o_tx_aon_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_state.bits.hotplug_state = uhotplug_state;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_state.u32, "TX_AON_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask13Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask13
//  Input       : unsigned int uaon_intr_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask13Set(unsigned int uaon_intr_mask13)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask13 = uaon_intr_mask13;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask12Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask12
//  Input       : unsigned int uaon_intr_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask12Set(unsigned int uaon_intr_mask12)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask12 = uaon_intr_mask12;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask11Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask11
//  Input       : unsigned int uaon_intr_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask11Set(unsigned int uaon_intr_mask11)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask11 = uaon_intr_mask11;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask10Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask10
//  Input       : unsigned int uaon_intr_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask10Set(unsigned int uaon_intr_mask10)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask10 = uaon_intr_mask10;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask9Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask9
//  Input       : unsigned int uaon_intr_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask9Set(unsigned int uaon_intr_mask9)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask9 = uaon_intr_mask9;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask8Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask8
//  Input       : unsigned int uaon_intr_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask8Set(unsigned int uaon_intr_mask8)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask8 = uaon_intr_mask8;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask7Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask7
//  Input       : unsigned int uaon_intr_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask7Set(unsigned int uaon_intr_mask7)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask7 = uaon_intr_mask7;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask6Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask6
//  Input       : unsigned int uaon_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask6Set(unsigned int uaon_intr_mask6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask6 = uaon_intr_mask6;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask5Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask5
//  Input       : unsigned int uaon_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask5Set(unsigned int uaon_intr_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask5 = uaon_intr_mask5;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask4Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask4
//  Input       : unsigned int uaon_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask4Set(unsigned int uaon_intr_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask4 = uaon_intr_mask4;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask1Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask1
//  Input       : unsigned int uaon_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask1Set(unsigned int uaon_intr_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask1 = uaon_intr_mask1;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask0Set
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask0
//  Input       : unsigned int uaon_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask0Set(unsigned int uaon_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_aon_intr_mask.bits.aon_intr_mask0 = uaon_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_mask.u32, "TX_AON_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat13Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat13
//  Input       : unsigned int uaon_intr_stat13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat13Set(unsigned int uaon_intr_stat13)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat13 = uaon_intr_stat13;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32, "TX_AON_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat12Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat12
//  Input       : unsigned int uaon_intr_stat12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat12Set(unsigned int uaon_intr_stat12)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat12 = uaon_intr_stat12;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat11Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat11
//  Input       : unsigned int uaon_intr_stat11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat11Set(unsigned int uaon_intr_stat11)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat11 = uaon_intr_stat11;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat10Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat10
//  Input       : unsigned int uaon_intr_stat10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat10Set(unsigned int uaon_intr_stat10)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat10 = uaon_intr_stat10;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat9Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat9
//  Input       : unsigned int uaon_intr_stat9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat9Set(unsigned int uaon_intr_stat9)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat9 = uaon_intr_stat9;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat8Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat8
//  Input       : unsigned int uaon_intr_stat8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat8Set(unsigned int uaon_intr_stat8)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat8 = uaon_intr_stat8;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat7Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat7
//  Input       : unsigned int uaon_intr_stat7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat7Set(unsigned int uaon_intr_stat7)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat7 = uaon_intr_stat7;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat6Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat6
//  Input       : unsigned int uaon_intr_stat6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat6Set(unsigned int uaon_intr_stat6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat6 = uaon_intr_stat6;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat5Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat5
//  Input       : unsigned int uaon_intr_stat5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat5Set(unsigned int uaon_intr_stat5)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat5 = uaon_intr_stat5;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat4Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat4
//  Input       : unsigned int uaon_intr_stat4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat4Set(unsigned int uaon_intr_stat4)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat4 = uaon_intr_stat4;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat1Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat1
//  Input       : unsigned int uaon_intr_stat1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat1Set(unsigned int uaon_intr_stat1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat1 = uaon_intr_stat1;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat0Set
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat0
//  Input       : unsigned int uaon_intr_stat0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat0Set(unsigned int uaon_intr_stat0)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = 0;
    o_tx_aon_intr_state.bits.aon_intr_stat0 = uaon_intr_stat0;
    HDMITXRegWrite(pu32RegAddr, o_tx_aon_intr_state.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_ddc_speed_cntSet
//  Description : Set the value of the member DDC_MST_CTRL.ddc_speed_cnt
//  Input       : unsigned int uddc_speed_cnt: 9 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_ddc_speed_cntSet(unsigned int uddc_speed_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_ctrl.bits.ddc_speed_cnt = uddc_speed_cnt;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32, "DDC_MST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_dcc_man_enSet
//  Description : Set the value of the member DDC_MST_CTRL.dcc_man_en
//  Input       : unsigned int udcc_man_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_dcc_man_enSet(unsigned int udcc_man_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_ctrl.bits.dcc_man_en = udcc_man_en;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32, "DDC_MST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_ddc_aon_accessSet
//  Description : Set the value of the member DDC_MST_CTRL.ddc_aon_access
//  Input       : unsigned int uddc_aon_access: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_ddc_aon_accessSet(unsigned int uddc_aon_access)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_ctrl.bits.ddc_aon_access = uddc_aon_access;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32, "DDC_MST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outSet
//  Description : Set the value of the member DDC_FIFO_RDATA.ddc_fifo_data_out
//  Input       : unsigned int uddc_fifo_data_out: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outSet(unsigned int uddc_fifo_data_out)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_FIFO_RDATA o_ddc_fifo_rdata;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_FIFO_RDATA.u32);
    o_ddc_fifo_rdata.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_fifo_rdata.bits.ddc_fifo_data_out = uddc_fifo_data_out;
    HDMITXRegWrite(pu32RegAddr, o_ddc_fifo_rdata.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_fifo_rdata.u32, "DDC_FIFO_RDATA");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inSet
//  Description : Set the value of the member DDC_FIFO_WDATA.ddc_fifo_data_in
//  Input       : unsigned int uddc_fifo_data_in: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inSet(unsigned int uddc_fifo_data_in)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_FIFO_WDATA o_ddc_fifo_wdata;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_FIFO_WDATA.u32);
    o_ddc_fifo_wdata.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_fifo_wdata.bits.ddc_fifo_data_in = uddc_fifo_data_in;
    HDMITXRegWrite(pu32RegAddr, o_ddc_fifo_wdata.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_fifo_wdata.u32, "DDC_FIFO_WDATA");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_DATA_CNT_ddc_data_out_cntSet
//  Description : Set the value of the member DDC_DATA_CNT.ddc_data_out_cnt
//  Input       : unsigned int uddc_data_out_cnt: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_DATA_CNT_ddc_data_out_cntSet(unsigned int uddc_data_out_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_DATA_CNT o_ddc_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_DATA_CNT.u32);
    o_ddc_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_data_cnt.bits.ddc_data_out_cnt = uddc_data_out_cnt;
    HDMITXRegWrite(pu32RegAddr, o_ddc_data_cnt.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_data_cnt.u32, "DDC_DATA_CNT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_DATA_CNT_ddc_fifo_data_cntSet
//  Description : Set the value of the member DDC_DATA_CNT.ddc_fifo_data_cnt
//  Input       : unsigned int uddc_fifo_data_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_DATA_CNT_ddc_fifo_data_cntSet(unsigned int uddc_fifo_data_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_DATA_CNT o_ddc_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_DATA_CNT.u32);
    o_ddc_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_data_cnt.bits.ddc_fifo_data_cnt = uddc_fifo_data_cnt;
    HDMITXRegWrite(pu32RegAddr, o_ddc_data_cnt.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_data_cnt.u32, "DDC_DATA_CNT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_segSet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_seg
//  Input       : unsigned int uddc_slave_seg: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_segSet(unsigned int uddc_slave_seg)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_slave_cfg.bits.ddc_slave_seg = uddc_slave_seg;
    HDMITXRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32, "DDC_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_offsetSet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_offset
//  Input       : unsigned int uddc_slave_offset: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_offsetSet(unsigned int uddc_slave_offset)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_slave_cfg.bits.ddc_slave_offset = uddc_slave_offset;
    HDMITXRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32, "DDC_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_addrSet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_addr
//  Input       : unsigned int uddc_slave_addr: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_addrSet(unsigned int uddc_slave_addr)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_slave_cfg.bits.ddc_slave_addr = uddc_slave_addr;
    HDMITXRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_slave_cfg.u32, "DDC_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_fullSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_full
//  Input       : unsigned int uddc_fifo_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_fullSet(unsigned int uddc_fifo_full)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_fifo_full = uddc_fifo_full;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_half_fullSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_half_full
//  Input       : unsigned int uddc_fifo_half_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_half_fullSet(unsigned int uddc_fifo_half_full)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_fifo_half_full = uddc_fifo_half_full;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_emptySet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_empty
//  Input       : unsigned int uddc_fifo_empty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_emptySet(unsigned int uddc_fifo_empty)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_fifo_empty = uddc_fifo_empty;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_rd_in_use
//  Input       : unsigned int uddc_fifo_rd_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useSet(unsigned int uddc_fifo_rd_in_use)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_fifo_rd_in_use = uddc_fifo_rd_in_use;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_wr_in_use
//  Input       : unsigned int uddc_fifo_wr_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useSet(unsigned int uddc_fifo_wr_in_use)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_fifo_wr_in_use = uddc_fifo_wr_in_use;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_in_progSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_in_prog
//  Input       : unsigned int uddc_i2c_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_in_progSet(unsigned int uddc_i2c_in_prog)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_i2c_in_prog = uddc_i2c_in_prog;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_bus_lowSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_bus_low
//  Input       : unsigned int uddc_i2c_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_bus_lowSet(unsigned int uddc_i2c_bus_low)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_i2c_bus_low = uddc_i2c_bus_low;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_no_ackSet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_no_ack
//  Input       : unsigned int uddc_i2c_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_no_ackSet(unsigned int uddc_i2c_no_ack)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_state.bits.ddc_i2c_no_ack = uddc_i2c_no_ack;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_state.u32, "DDC_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CMD_ddc_mst_cmdSet
//  Description : Set the value of the member DDC_MST_CMD.ddc_mst_cmd
//  Input       : unsigned int uddc_mst_cmd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CMD_ddc_mst_cmdSet(unsigned int uddc_mst_cmd)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CMD o_ddc_mst_cmd;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CMD.u32);
    o_ddc_mst_cmd.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_cmd.bits.ddc_mst_cmd = uddc_mst_cmd;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_cmd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_cmd.u32, "DDC_MST_CMD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_sda_oenSet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_oen
//  Input       : unsigned int uddc_sda_oen: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_sda_oenSet(unsigned int uddc_sda_oen)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_sda_oen = uddc_sda_oen;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32, "DDC_MAN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_scl_oenSet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_oen
//  Input       : unsigned int uddc_scl_oen: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_scl_oenSet(unsigned int uddc_scl_oen)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_scl_oen = uddc_scl_oen;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32, "DDC_MAN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_sda_stSet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_st
//  Input       : unsigned int uddc_sda_st: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_sda_stSet(unsigned int uddc_sda_st)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_sda_st = uddc_sda_st;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32, "DDC_MAN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_scl_stSet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_st
//  Input       : unsigned int uddc_scl_st: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_scl_stSet(unsigned int uddc_scl_st)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_scl_st = uddc_scl_st;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32, "DDC_MAN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_STATE_CLR_ddc_clr_no_ackSet
//  Description : Set the value of the member DDC_STATE_CLR.ddc_clr_no_ack
//  Input       : unsigned int uddc_clr_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_STATE_CLR_ddc_clr_no_ackSet(unsigned int uddc_clr_no_ack)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_STATE_CLR o_ddc_state_clr;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_STATE_CLR.u32);
    o_ddc_state_clr.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_state_clr.bits.ddc_clr_no_ack = uddc_clr_no_ack;
    HDMITXRegWrite(pu32RegAddr, o_ddc_state_clr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_state_clr.u32, "DDC_STATE_CLR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_STATE_CLR_ddc_clr_bus_lowSet
//  Description : Set the value of the member DDC_STATE_CLR.ddc_clr_bus_low
//  Input       : unsigned int uddc_clr_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_STATE_CLR_ddc_clr_bus_lowSet(unsigned int uddc_clr_bus_low)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_STATE_CLR o_ddc_state_clr;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_STATE_CLR.u32);
    o_ddc_state_clr.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_state_clr.bits.ddc_clr_bus_low = uddc_clr_bus_low;
    HDMITXRegWrite(pu32RegAddr, o_ddc_state_clr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_state_clr.u32, "DDC_STATE_CLR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_yearGet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_year
//  Input       : unsigned int utx_hw_year: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_yearGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_info.bits.tx_hw_year;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_monthGet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_month
//  Input       : unsigned int utx_hw_month: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_monthGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_info.bits.tx_hw_month;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_INFO_tx_hw_dayGet
//  Description : Set the value of the member TX_HW_INFO.tx_hw_day
//  Input       : unsigned int utx_hw_day: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_INFO_tx_hw_dayGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_INFO o_tx_hw_info;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_INFO.u32);
    o_tx_hw_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_info.bits.tx_hw_day;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_rtl_versionGet
//  Description : Set the value of the member TX_HW_VERSION.tx_rtl_version
//  Input       : unsigned int utx_rtl_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_rtl_versionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_version.bits.tx_rtl_version;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_drv_versionGet
//  Description : Set the value of the member TX_HW_VERSION.tx_drv_version
//  Input       : unsigned int utx_drv_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_drv_versionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_version.bits.tx_drv_version;
}

//******************************************************************************
//  Function    : HDMI_TX_HW_VERSION_tx_reg_versionGet
//  Description : Set the value of the member TX_HW_VERSION.tx_reg_version
//  Input       : unsigned int utx_reg_version: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_HW_VERSION_tx_reg_versionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_HW_VERSION o_tx_hw_version;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_HW_VERSION.u32);
    o_tx_hw_version.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_hw_version.bits.tx_reg_version;
}


//******************************************************************************
//  Function    : HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqGet
//  Description : Set the value of the member TX_AON_RST_CTRL.tx_aon_soft_arst_req
//  Input       : unsigned int utx_aon_soft_arst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_RST_CTRL o_tx_aon_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_RST_CTRL.u32);
    o_tx_aon_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_rst_ctrl.bits.tx_aon_soft_arst_req;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_osc_div_cntGet
//  Description : Set the value of the member HOTPLUG_ST_CFG.osc_div_cnt
//  Input       : unsigned int uosc_div_cnt: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_osc_div_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_st_cfg.bits.osc_div_cnt;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_soft_valueGet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_soft_value
//  Input       : unsigned int uhpd_soft_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_soft_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_st_cfg.bits.hpd_soft_value;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlGet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_polarity_ctl
//  Input       : unsigned int uhpd_polarity_ctl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_st_cfg.bits.hpd_polarity_ctl;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_override_enGet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_override_en
//  Input       : unsigned int uhpd_override_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_override_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_st_cfg.bits.hpd_override_en;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_ST_CFG_hpd_fillter_enGet
//  Description : Set the value of the member HOTPLUG_ST_CFG.hpd_fillter_en
//  Input       : unsigned int uhpd_fillter_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_ST_CFG_hpd_fillter_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_ST_CFG o_hotplug_st_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_ST_CFG.u32);
    o_hotplug_st_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_st_cfg.bits.hpd_fillter_en;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdGet
//  Description : Set the value of the member HOTPLUG_FILLTER_CFG.hpd_low_reshold
//  Input       : unsigned int uhpd_low_reshold: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_FILLTER_CFG o_hotplug_fillter_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_FILLTER_CFG.u32);
    o_hotplug_fillter_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_fillter_cfg.bits.hpd_low_reshold;
}

//******************************************************************************
//  Function    : HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdGet
//  Description : Set the value of the member HOTPLUG_FILLTER_CFG.hpd_high_reshold
//  Input       : unsigned int uhpd_high_reshold: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_HOTPLUG_FILLTER_CFG o_hotplug_fillter_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->HOTPLUG_FILLTER_CFG.u32);
    o_hotplug_fillter_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hotplug_fillter_cfg.bits.hpd_high_reshold;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_STATE_phy_rx_senseGet
//  Description : Set the value of the member TX_AON_STATE.phy_rx_sense
//  Input       : unsigned int uphy_rx_sense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_STATE_phy_rx_senseGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_STATE o_tx_aon_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_STATE.u32);
    o_tx_aon_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_state.bits.phy_rx_sense;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_STATE_hotplug_stateGet
//  Description : Set the value of the member TX_AON_STATE.hotplug_state
//  Input       : unsigned int uhotplug_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_STATE_hotplug_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_STATE o_tx_aon_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_STATE.u32);
    o_tx_aon_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_state.bits.hotplug_state;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask13Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask13
//  Input       : unsigned int uaon_intr_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask13;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask12Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask12
//  Input       : unsigned int uaon_intr_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask12;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask11Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask11
//  Input       : unsigned int uaon_intr_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask11;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask10Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask10
//  Input       : unsigned int uaon_intr_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask10;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask9Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask9
//  Input       : unsigned int uaon_intr_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask9;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask8Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask8
//  Input       : unsigned int uaon_intr_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask8;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask7Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask7
//  Input       : unsigned int uaon_intr_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask7;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask6Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask6
//  Input       : unsigned int uaon_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask6;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask5Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask5
//  Input       : unsigned int uaon_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask5;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask4Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask4
//  Input       : unsigned int uaon_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask4;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask1Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask1
//  Input       : unsigned int uaon_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask1;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_MASK_aon_intr_mask0Get
//  Description : Set the value of the member TX_AON_INTR_MASK.aon_intr_mask0
//  Input       : unsigned int uaon_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_MASK_aon_intr_mask0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_MASK o_tx_aon_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_MASK.u32);
    o_tx_aon_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_mask.bits.aon_intr_mask0;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat13Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat13
//  Input       : unsigned int uaon_intr_stat13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat13;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat12Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat12
//  Input       : unsigned int uaon_intr_stat12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat12;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat11Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat11
//  Input       : unsigned int uaon_intr_stat11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat11;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat10Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat10
//  Input       : unsigned int uaon_intr_stat10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat10;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat9Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat9
//  Input       : unsigned int uaon_intr_stat9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat9;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat8Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat8
//  Input       : unsigned int uaon_intr_stat8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat8;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat7Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat7
//  Input       : unsigned int uaon_intr_stat7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat7;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat6Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat6
//  Input       : unsigned int uaon_intr_stat6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat6;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat5Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat5
//  Input       : unsigned int uaon_intr_stat5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat5;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat4Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat4
//  Input       : unsigned int uaon_intr_stat4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat4;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat1Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat1
//  Input       : unsigned int uaon_intr_stat1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat1;
}

//******************************************************************************
//  Function    : HDMI_TX_AON_INTR_STATE_aon_intr_stat0Get
//  Description : Set the value of the member TX_AON_INTR_STATE.aon_intr_stat0
//  Input       : unsigned int uaon_intr_stat0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AON_INTR_STATE_aon_intr_stat0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AON_INTR_STATE o_tx_aon_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->TX_AON_INTR_STATE.u32);
    o_tx_aon_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_aon_intr_state.bits.aon_intr_stat0;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_ddc_speed_cntGet
//  Description : Set the value of the member DDC_MST_CTRL.ddc_speed_cnt
//  Input       : unsigned int uddc_speed_cnt: 9 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_ddc_speed_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_ctrl.bits.ddc_speed_cnt;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_dcc_man_enGet
//  Description : Set the value of the member DDC_MST_CTRL.dcc_man_en
//  Input       : unsigned int udcc_man_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_dcc_man_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_ctrl.bits.dcc_man_en;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CTRL_ddc_aon_accessGet
//  Description : Set the value of the member DDC_MST_CTRL.ddc_aon_access
//  Input       : unsigned int uddc_aon_access: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CTRL_ddc_aon_accessGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_ctrl.bits.ddc_aon_access;
}

//******************************************************************************
//  Function    : HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outGet
//  Description : Set the value of the member DDC_FIFO_RDATA.ddc_fifo_data_out
//  Input       : unsigned int uddc_fifo_data_out: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_FIFO_RDATA o_ddc_fifo_rdata;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_FIFO_RDATA.u32);
    o_ddc_fifo_rdata.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_fifo_rdata.bits.ddc_fifo_data_out;
}

//******************************************************************************
//  Function    : HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inGet
//  Description : Set the value of the member DDC_FIFO_WDATA.ddc_fifo_data_in
//  Input       : unsigned int uddc_fifo_data_in: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_FIFO_WDATA o_ddc_fifo_wdata;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_FIFO_WDATA.u32);
    o_ddc_fifo_wdata.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_fifo_wdata.bits.ddc_fifo_data_in;
}

//******************************************************************************
//  Function    : HDMI_DDC_DATA_CNT_ddc_data_out_cntGet
//  Description : Set the value of the member DDC_DATA_CNT.ddc_data_out_cnt
//  Input       : unsigned int uddc_data_out_cnt: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_DATA_CNT_ddc_data_out_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_DATA_CNT o_ddc_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_DATA_CNT.u32);
    o_ddc_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_data_cnt.bits.ddc_data_out_cnt;
}

//******************************************************************************
//  Function    : HDMI_DDC_DATA_CNT_ddc_fifo_data_cntGet
//  Description : Set the value of the member DDC_DATA_CNT.ddc_fifo_data_cnt
//  Input       : unsigned int uddc_fifo_data_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_DATA_CNT_ddc_fifo_data_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_DATA_CNT o_ddc_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_DATA_CNT.u32);
    o_ddc_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_data_cnt.bits.ddc_fifo_data_cnt;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_segGet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_seg
//  Input       : unsigned int uddc_slave_seg: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_segGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_slave_cfg.bits.ddc_slave_seg;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_offsetGet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_offset
//  Input       : unsigned int uddc_slave_offset: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_offsetGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_slave_cfg.bits.ddc_slave_offset;
}

//******************************************************************************
//  Function    : HDMI_DDC_SLAVE_CFG_ddc_slave_addrGet
//  Description : Set the value of the member DDC_SLAVE_CFG.ddc_slave_addr
//  Input       : unsigned int uddc_slave_addr: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_SLAVE_CFG_ddc_slave_addrGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_SLAVE_CFG o_ddc_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_SLAVE_CFG.u32);
    o_ddc_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_slave_cfg.bits.ddc_slave_addr;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_fullGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_full
//  Input       : unsigned int uddc_fifo_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_fullGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_fifo_full;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_half_fullGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_half_full
//  Input       : unsigned int uddc_fifo_half_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_half_fullGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_fifo_half_full;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_emptyGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_empty
//  Input       : unsigned int uddc_fifo_empty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_emptyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_fifo_empty;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_rd_in_use
//  Input       : unsigned int uddc_fifo_rd_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_fifo_rd_in_use;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_fifo_wr_in_use
//  Input       : unsigned int uddc_fifo_wr_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_fifo_wr_in_use;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_in_progGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_in_prog
//  Input       : unsigned int uddc_i2c_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_in_progGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_i2c_in_prog;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_bus_lowGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_bus_low
//  Input       : unsigned int uddc_i2c_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_bus_lowGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_i2c_bus_low;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_STATE_ddc_i2c_no_ackGet
//  Description : Set the value of the member DDC_MST_STATE.ddc_i2c_no_ack
//  Input       : unsigned int uddc_i2c_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_STATE_ddc_i2c_no_ackGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_i2c_no_ack;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_CMD_ddc_mst_cmdGet
//  Description : Set the value of the member DDC_MST_CMD.ddc_mst_cmd
//  Input       : unsigned int uddc_mst_cmd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_CMD_ddc_mst_cmdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MST_CMD o_ddc_mst_cmd;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CMD.u32);
    o_ddc_mst_cmd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_cmd.bits.ddc_mst_cmd;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_sda_oenGet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_oen
//  Input       : unsigned int uddc_sda_oen: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_sda_oenGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_sda_oen;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_scl_oenGet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_oen
//  Input       : unsigned int uddc_scl_oen: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_scl_oenGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_scl_oen;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_sda_stGet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_st
//  Input       : unsigned int uddc_sda_st: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_sda_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_sda_st;
}

//******************************************************************************
//  Function    : HDMI_DDC_MAN_CTRL_ddc_scl_stGet
//  Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_st
//  Input       : unsigned int uddc_scl_st: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MAN_CTRL_ddc_scl_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_scl_st;
}

//******************************************************************************
//  Function    : HDMI_DDC_STATE_CLR_ddc_clr_no_ackGet
//  Description : Set the value of the member DDC_STATE_CLR.ddc_clr_no_ack
//  Input       : unsigned int uddc_clr_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_STATE_CLR_ddc_clr_no_ackGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_STATE_CLR o_ddc_state_clr;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_STATE_CLR.u32);
    o_ddc_state_clr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_state_clr.bits.ddc_clr_no_ack;
}

//******************************************************************************
//  Function    : HDMI_DDC_STATE_CLR_ddc_clr_bus_lowGet
//  Description : Set the value of the member DDC_STATE_CLR.ddc_clr_bus_low
//  Input       : unsigned int uddc_clr_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_STATE_CLR_ddc_clr_bus_lowGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_DDC_STATE_CLR o_ddc_state_clr;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_STATE_CLR.u32);
    o_ddc_state_clr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_state_clr.bits.ddc_clr_bus_low;
}


