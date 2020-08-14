// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_cec_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/08 20:14:09 Create file
// ******************************************************************************

#include "hi_type.h"
#include "hdmi_reg_cec.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif

/* Define the struct pointor of the module tx_cec_reg */
volatile S_tx_cec_reg_REGS_TYPE *goptx_cec_regAllReg = NULL;


int HDMI_TX_S_tx_cec_reg_REGS_TYPE_Init(void)
{
    if (goptx_cec_regAllReg)
    {
        return HI_SUCCESS;
    }

    goptx_cec_regAllReg = (volatile S_tx_cec_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_cec_reg,4);

    if (!goptx_cec_regAllReg)
    {
        HDMI_ERR("goptx_cec_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;

}

int HDMI_TX_S_tx_cec_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_cec_regAllReg)
    {
        HDMI_IO_UNMAP(goptx_cec_regAllReg);
        goptx_cec_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}



//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_hw_verSet
//  Description : Set the value of the member CEC_DEV_INFO.cec_hw_ver
//  Input       : unsigned int ucec_hw_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_hw_verSet(unsigned int ucec_hw_ver)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_dev_info.bits.cec_hw_ver = ucec_hw_ver;
    HDMITXRegWrite(pu32RegAddr, o_cec_dev_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_dev_info.u32, "CEC_DEV_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_sub_sysSet
//  Description : Set the value of the member CEC_DEV_INFO.cec_sub_sys
//  Input       : unsigned int ucec_sub_sys: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_sub_sysSet(unsigned int ucec_sub_sys)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_dev_info.bits.cec_sub_sys = ucec_sub_sys;
    HDMITXRegWrite(pu32RegAddr, o_cec_dev_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_dev_info.u32, "CEC_DEV_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_spec_suffixSet
//  Description : Set the value of the member CEC_DEV_INFO.cec_spec_suffix
//  Input       : unsigned int ucec_spec_suffix: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_spec_suffixSet(unsigned int ucec_spec_suffix)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_dev_info.bits.cec_spec_suffix = ucec_spec_suffix;
    HDMITXRegWrite(pu32RegAddr, o_cec_dev_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_dev_info.u32, "CEC_DEV_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_spec_verSet
//  Description : Set the value of the member CEC_DEV_INFO.cec_spec_ver
//  Input       : unsigned int ucec_spec_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_spec_verSet(unsigned int ucec_spec_ver)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_dev_info.bits.cec_spec_ver = ucec_spec_ver;
    HDMITXRegWrite(pu32RegAddr, o_cec_dev_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_dev_info.u32, "CEC_DEV_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_device_idSet
//  Description : Set the value of the member CEC_DEV_INFO.cec_device_id
//  Input       : unsigned int ucec_device_id: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_device_idSet(unsigned int ucec_device_id)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_dev_info.bits.cec_device_id = ucec_device_id;
    HDMITXRegWrite(pu32RegAddr, o_cec_dev_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_dev_info.u32, "CEC_DEV_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_START_PRIOD_cec_start_low_priodSet
//  Description : Set the value of the member CEC_START_PRIOD.cec_start_low_priod
//  Input       : unsigned int ucec_start_low_priod: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_START_PRIOD_cec_start_low_priodSet(unsigned int ucec_start_low_priod)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_START_PRIOD o_cec_start_priod;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_START_PRIOD.u32);
    o_cec_start_priod.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_start_priod.bits.cec_start_low_priod = ucec_start_low_priod;
    HDMITXRegWrite(pu32RegAddr, o_cec_start_priod.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_start_priod.u32, "CEC_START_PRIOD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_START_PRIOD_cec_start_bit_priodSet
//  Description : Set the value of the member CEC_START_PRIOD.cec_start_bit_priod
//  Input       : unsigned int ucec_start_bit_priod: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_START_PRIOD_cec_start_bit_priodSet(unsigned int ucec_start_bit_priod)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_START_PRIOD o_cec_start_priod;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_START_PRIOD.u32);
    o_cec_start_priod.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_start_priod.bits.cec_start_bit_priod = ucec_start_bit_priod;
    HDMITXRegWrite(pu32RegAddr, o_cec_start_priod.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_start_priod.u32, "CEC_START_PRIOD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptySet
//  Description : Set the value of the member CEC_TX_CTRL.auto_clear_intrp_rx_fifo_nempty
//  Input       : unsigned int uauto_clear_intrp_rx_fifo_nempty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptySet(unsigned int uauto_clear_intrp_rx_fifo_nempty)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.auto_clear_intrp_rx_fifo_nempty = uauto_clear_intrp_rx_fifo_nempty;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_rx_clr_all
//  Input       : unsigned int ucec_reg_rx_clr_all: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allSet(unsigned int ucec_reg_rx_clr_all)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_rx_clr_all = ucec_reg_rx_clr_all;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_rx_clr_cur_set
//  Input       : unsigned int ucec_reg_rx_clr_cur_set: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setSet(unsigned int ucec_reg_rx_clr_cur_set)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_rx_clr_cur_set = ucec_reg_rx_clr_cur_set;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_retry_limit
//  Input       : unsigned int ucec_reg_tx_retry_limit: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitSet(unsigned int ucec_reg_tx_retry_limit)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_tx_retry_limit = ucec_reg_tx_retry_limit;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_cmd_cnt
//  Input       : unsigned int ucec_reg_tx_cmd_cnt: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntSet(unsigned int ucec_reg_tx_cmd_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_tx_cmd_cnt = ucec_reg_tx_cmd_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_bfr_ac
//  Input       : unsigned int ucec_reg_tx_bfr_ac: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acSet(unsigned int ucec_reg_tx_bfr_ac)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_tx_bfr_ac = ucec_reg_tx_bfr_ac;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_auto_calc
//  Input       : unsigned int ucec_reg_tx_auto_calc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcSet(unsigned int ucec_reg_tx_auto_calc)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_tx_auto_calc = ucec_reg_tx_auto_calc;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_manual_cmd_setSet
//  Description : Set the value of the member CEC_TX_CTRL.manual_cmd_set
//  Input       : unsigned int umanual_cmd_set: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_manual_cmd_setSet(unsigned int umanual_cmd_set)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.manual_cmd_set = umanual_cmd_set;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_i2c_cec_passthru
//  Input       : unsigned int ucec_reg_i2c_cec_passthru: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruSet(unsigned int ucec_reg_i2c_cec_passthru)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_i2c_cec_passthru = ucec_reg_i2c_cec_passthru;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_force_non_calibSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_force_non_calib
//  Input       : unsigned int ucec_reg_force_non_calib: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_force_non_calibSet(unsigned int ucec_reg_force_non_calib)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_force_non_calib = ucec_reg_force_non_calib;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_calib_cec_en
//  Input       : unsigned int ucec_reg_calib_cec_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enSet(unsigned int ucec_reg_calib_cec_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_calib_cec_en = ucec_reg_calib_cec_en;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_calib_cecSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_calib_cec
//  Input       : unsigned int ucec_reg_calib_cec: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_calib_cecSet(unsigned int ucec_reg_calib_cec)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_calib_cec = ucec_reg_calib_cec;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_flush_tx_ff
//  Input       : unsigned int ucec_reg_flush_tx_ff: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffSet(unsigned int ucec_reg_flush_tx_ff)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_flush_tx_ff = ucec_reg_flush_tx_ff;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_ctl_retry_cntSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_ctl_retry_cnt
//  Input       : unsigned int ucec_ctl_retry_cnt: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_ctl_retry_cntSet(unsigned int ucec_ctl_retry_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_ctl_retry_cnt = ucec_ctl_retry_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_inv_ack_brcstSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_inv_ack_brcst
//  Input       : unsigned int ucec_inv_ack_brcst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_inv_ack_brcstSet(unsigned int ucec_inv_ack_brcst)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_inv_ack_brcst = ucec_inv_ack_brcst;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_nack_hdrSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_nack_hdr
//  Input       : unsigned int ucec_reg_nack_hdr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_nack_hdrSet(unsigned int ucec_reg_nack_hdr)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_nack_hdr = ucec_reg_nack_hdr;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_snoopSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_snoop
//  Input       : unsigned int ucec_reg_snoop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_snoopSet(unsigned int ucec_reg_snoop)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_reg_snoop = ucec_reg_snoop;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_snoop_initiatorSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_snoop_initiator
//  Input       : unsigned int ucec_snoop_initiator: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_snoop_initiatorSet(unsigned int ucec_snoop_initiator)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_snoop_initiator = ucec_snoop_initiator;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_bus_ownerSet
//  Description : Set the value of the member CEC_TX_CTRL.cec_bus_owner
//  Input       : unsigned int ucec_bus_owner: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_bus_ownerSet(unsigned int ucec_bus_owner)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_ctrl.bits.cec_bus_owner = ucec_bus_owner;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_ctrl.u32, "CEC_TX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerSet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_init_cmd_header
//  Input       : unsigned int ucec_rx_init_cmd_header: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerSet(unsigned int ucec_rx_init_cmd_header)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_rx_state.bits.cec_rx_init_cmd_header = ucec_rx_init_cmd_header;
    HDMITXRegWrite(pu32RegAddr, o_cec_rx_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_rx_state.u32, "CEC_RX_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerSet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_dest_cmd_header
//  Input       : unsigned int ucec_rx_dest_cmd_header: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerSet(unsigned int ucec_rx_dest_cmd_header)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_rx_state.bits.cec_rx_dest_cmd_header = ucec_rx_dest_cmd_header;
    HDMITXRegWrite(pu32RegAddr, o_cec_rx_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_rx_state.u32, "CEC_RX_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_errorSet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_error
//  Input       : unsigned int ucec_rx_error: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_errorSet(unsigned int ucec_rx_error)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_rx_state.bits.cec_rx_error = ucec_rx_error;
    HDMITXRegWrite(pu32RegAddr, o_cec_rx_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_rx_state.u32, "CEC_RX_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selSet
//  Description : Set the value of the member CEC_RX_STATE.cec_reg_rx_ff_wr_sel
//  Input       : unsigned int ucec_reg_rx_ff_wr_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selSet(unsigned int ucec_reg_rx_ff_wr_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_rx_state.bits.cec_reg_rx_ff_wr_sel = ucec_reg_rx_ff_wr_sel;
    HDMITXRegWrite(pu32RegAddr, o_cec_rx_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_rx_state.u32, "CEC_RX_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntSet
//  Description : Set the value of the member CEC_RX_STATE.cec_reg_rx_cmd_byte_cnt
//  Input       : unsigned int ucec_reg_rx_cmd_byte_cnt: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntSet(unsigned int ucec_reg_rx_cmd_byte_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_rx_state.bits.cec_reg_rx_cmd_byte_cnt = ucec_reg_rx_cmd_byte_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cec_rx_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_rx_state.u32, "CEC_RX_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_doneSet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_done
//  Input       : unsigned int ucec_auto_ping_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_doneSet(unsigned int ucec_auto_ping_done)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_auto_disc.bits.cec_auto_ping_done = ucec_auto_ping_done;
    HDMITXRegWrite(pu32RegAddr, o_cec_auto_disc.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_auto_disc.u32, "CEC_AUTO_DISC");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_clearSet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_clear
//  Input       : unsigned int ucec_auto_ping_clear: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_clearSet(unsigned int ucec_auto_ping_clear)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_auto_disc.bits.cec_auto_ping_clear = ucec_auto_ping_clear;
    HDMITXRegWrite(pu32RegAddr, o_cec_auto_disc.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_auto_disc.u32, "CEC_AUTO_DISC");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_startSet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_start
//  Input       : unsigned int ucec_auto_ping_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_startSet(unsigned int ucec_auto_ping_start)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_auto_disc.bits.cec_auto_ping_start = ucec_auto_ping_start;
    HDMITXRegWrite(pu32RegAddr, o_cec_auto_disc.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_auto_disc.u32, "CEC_AUTO_DISC");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_opcodeSet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_opcode
//  Input       : unsigned int ucec_cdc_opcode: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_opcodeSet(unsigned int ucec_cdc_opcode)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_cdc_ctrl.bits.cec_cdc_opcode = ucec_cdc_opcode;
    HDMITXRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32, "CEC_CDC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_arb_enSet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_arb_en
//  Input       : unsigned int ucec_cdc_arb_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_enSet(unsigned int ucec_cdc_arb_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_cdc_ctrl.bits.cec_cdc_arb_en = ucec_cdc_arb_en;
    HDMITXRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32, "CEC_CDC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntSet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_arb_cnt
//  Input       : unsigned int ucec_cdc_arb_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntSet(unsigned int ucec_cdc_arb_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_cdc_ctrl.bits.cec_cdc_arb_cnt = ucec_cdc_arb_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_cdc_ctrl.u32, "CEC_CDC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapSet
//  Description : Set the value of the member CEC_AUTODISC_MAP.cec_auto_ping_map
//  Input       : unsigned int ucec_auto_ping_map: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapSet(unsigned int ucec_auto_ping_map)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTODISC_MAP o_cec_autodisc_map;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTODISC_MAP.u32);
    o_cec_autodisc_map.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_autodisc_map.bits.cec_auto_ping_map = ucec_auto_ping_map;
    HDMITXRegWrite(pu32RegAddr, o_cec_autodisc_map.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_autodisc_map.u32, "CEC_AUTODISC_MAP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_INIT_cec_tx_init_idSet
//  Description : Set the value of the member CEC_TX_INIT.cec_tx_init_id
//  Input       : unsigned int ucec_tx_init_id: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_INIT_cec_tx_init_idSet(unsigned int ucec_tx_init_id)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_INIT o_cec_tx_init;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_INIT.u32);
    o_cec_tx_init.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_init.bits.cec_tx_init_id = ucec_tx_init_id;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_init.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_init.u32, "CEC_TX_INIT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_DEST_cec_reg_sd_poll_internSet
//  Description : Set the value of the member CEC_TX_DEST.cec_reg_sd_poll_intern
//  Input       : unsigned int ucec_reg_sd_poll_intern: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_DEST_cec_reg_sd_poll_internSet(unsigned int ucec_reg_sd_poll_intern)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_DEST o_cec_tx_dest;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_DEST.u32);
    o_cec_tx_dest.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_dest.bits.cec_reg_sd_poll_intern = ucec_reg_sd_poll_intern;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_dest.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_dest.u32, "CEC_TX_DEST");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_DEST_cec_tx_dest_idSet
//  Description : Set the value of the member CEC_TX_DEST.cec_tx_dest_id
//  Input       : unsigned int ucec_tx_dest_id: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_DEST_cec_tx_dest_idSet(unsigned int ucec_tx_dest_id)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_DEST o_cec_tx_dest;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_DEST.u32);
    o_cec_tx_dest.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_tx_dest.bits.cec_tx_dest_id = ucec_tx_dest_id;
    HDMITXRegWrite(pu32RegAddr, o_cec_tx_dest.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_tx_dest.u32, "CEC_TX_DEST");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_CAPTUTR_ID_cec_capture_idSet
//  Description : Set the value of the member CEC_CAPTUTR_ID.cec_capture_id
//  Input       : unsigned int ucec_capture_id: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CAPTUTR_ID_cec_capture_idSet(unsigned int ucec_capture_id)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CAPTUTR_ID o_cec_captutr_id;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CAPTUTR_ID.u32);
    o_cec_captutr_id.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_captutr_id.bits.cec_capture_id = ucec_capture_id;
    HDMITXRegWrite(pu32RegAddr, o_cec_captutr_id.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_captutr_id.u32, "CEC_CAPTUTR_ID");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand2Set
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand2
//  Input       : unsigned int utx_cec_operand2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand2Set(unsigned int utx_cec_operand2)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd0.bits.tx_cec_operand2 = utx_cec_operand2;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32, "TX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand1Set
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand1
//  Input       : unsigned int utx_cec_operand1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand1Set(unsigned int utx_cec_operand1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd0.bits.tx_cec_operand1 = utx_cec_operand1;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32, "TX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand0Set
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand0
//  Input       : unsigned int utx_cec_operand0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand0Set(unsigned int utx_cec_operand0)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd0.bits.tx_cec_operand0 = utx_cec_operand0;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32, "TX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_commandSet
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_command
//  Input       : unsigned int utx_cec_command: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_commandSet(unsigned int utx_cec_command)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd0.bits.tx_cec_command = utx_cec_command;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd0.u32, "TX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand6Set
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand6
//  Input       : unsigned int utx_cec_operand6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand6Set(unsigned int utx_cec_operand6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd1.bits.tx_cec_operand6 = utx_cec_operand6;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32, "TX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand5Set
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand5
//  Input       : unsigned int utx_cec_operand5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand5Set(unsigned int utx_cec_operand5)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd1.bits.tx_cec_operand5 = utx_cec_operand5;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32, "TX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand4Set
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand4
//  Input       : unsigned int utx_cec_operand4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand4Set(unsigned int utx_cec_operand4)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd1.bits.tx_cec_operand4 = utx_cec_operand4;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32, "TX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand3Set
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand3
//  Input       : unsigned int utx_cec_operand3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand3Set(unsigned int utx_cec_operand3)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd1.bits.tx_cec_operand3 = utx_cec_operand3;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd1.u32, "TX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand10Set
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand10
//  Input       : unsigned int utx_cec_operand10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand10Set(unsigned int utx_cec_operand10)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd2.bits.tx_cec_operand10 = utx_cec_operand10;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32, "TX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand9Set
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand9
//  Input       : unsigned int utx_cec_operand9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand9Set(unsigned int utx_cec_operand9)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd2.bits.tx_cec_operand9 = utx_cec_operand9;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32, "TX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand8Set
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand8
//  Input       : unsigned int utx_cec_operand8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand8Set(unsigned int utx_cec_operand8)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd2.bits.tx_cec_operand8 = utx_cec_operand8;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32, "TX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand7Set
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand7
//  Input       : unsigned int utx_cec_operand7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand7Set(unsigned int utx_cec_operand7)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd2.bits.tx_cec_operand7 = utx_cec_operand7;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd2.u32, "TX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand14Set
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand14
//  Input       : unsigned int utx_cec_operand14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand14Set(unsigned int utx_cec_operand14)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd3.bits.tx_cec_operand14 = utx_cec_operand14;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32, "TX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand13Set
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand13
//  Input       : unsigned int utx_cec_operand13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand13Set(unsigned int utx_cec_operand13)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd3.bits.tx_cec_operand13 = utx_cec_operand13;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32, "TX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand12Set
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand12
//  Input       : unsigned int utx_cec_operand12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand12Set(unsigned int utx_cec_operand12)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd3.bits.tx_cec_operand12 = utx_cec_operand12;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32, "TX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand11Set
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand11
//  Input       : unsigned int utx_cec_operand11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand11Set(unsigned int utx_cec_operand11)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_cec_cmd3.bits.tx_cec_operand11 = utx_cec_operand11;
    HDMITXRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_cec_cmd3.u32, "TX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand2Set
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand2
//  Input       : unsigned int urx_cec_operand2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand2Set(unsigned int urx_cec_operand2)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd0.bits.rx_cec_operand2 = urx_cec_operand2;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32, "RX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand1Set
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand1
//  Input       : unsigned int urx_cec_operand1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand1Set(unsigned int urx_cec_operand1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd0.bits.rx_cec_operand1 = urx_cec_operand1;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32, "RX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand0Set
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand0
//  Input       : unsigned int urx_cec_operand0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand0Set(unsigned int urx_cec_operand0)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd0.bits.rx_cec_operand0 = urx_cec_operand0;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32, "RX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_commandSet
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_command
//  Input       : unsigned int urx_cec_command: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_commandSet(unsigned int urx_cec_command)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd0.bits.rx_cec_command = urx_cec_command;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd0.u32, "RX_CEC_CMD0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand6Set
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand6
//  Input       : unsigned int urx_cec_operand6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand6Set(unsigned int urx_cec_operand6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd1.bits.rx_cec_operand6 = urx_cec_operand6;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32, "RX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand5Set
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand5
//  Input       : unsigned int urx_cec_operand5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand5Set(unsigned int urx_cec_operand5)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd1.bits.rx_cec_operand5 = urx_cec_operand5;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32, "RX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand4Set
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand4
//  Input       : unsigned int urx_cec_operand4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand4Set(unsigned int urx_cec_operand4)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd1.bits.rx_cec_operand4 = urx_cec_operand4;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32, "RX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand3Set
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand3
//  Input       : unsigned int urx_cec_operand3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand3Set(unsigned int urx_cec_operand3)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd1.bits.rx_cec_operand3 = urx_cec_operand3;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd1.u32, "RX_CEC_CMD1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand10Set
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand10
//  Input       : unsigned int urx_cec_operand10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand10Set(unsigned int urx_cec_operand10)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd2.bits.rx_cec_operand10 = urx_cec_operand10;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32, "RX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand9Set
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand9
//  Input       : unsigned int urx_cec_operand9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand9Set(unsigned int urx_cec_operand9)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd2.bits.rx_cec_operand9 = urx_cec_operand9;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32, "RX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand8Set
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand8
//  Input       : unsigned int urx_cec_operand8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand8Set(unsigned int urx_cec_operand8)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd2.bits.rx_cec_operand8 = urx_cec_operand8;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32, "RX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand7Set
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand7
//  Input       : unsigned int urx_cec_operand7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand7Set(unsigned int urx_cec_operand7)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd2.bits.rx_cec_operand7 = urx_cec_operand7;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd2.u32, "RX_CEC_CMD2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand14Set
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand14
//  Input       : unsigned int urx_cec_operand14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand14Set(unsigned int urx_cec_operand14)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd3.bits.rx_cec_operand14 = urx_cec_operand14;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32, "RX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand13Set
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand13
//  Input       : unsigned int urx_cec_operand13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand13Set(unsigned int urx_cec_operand13)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd3.bits.rx_cec_operand13 = urx_cec_operand13;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32, "RX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand12Set
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand12
//  Input       : unsigned int urx_cec_operand12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand12Set(unsigned int urx_cec_operand12)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd3.bits.rx_cec_operand12 = urx_cec_operand12;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32, "RX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand11Set
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand11
//  Input       : unsigned int urx_cec_operand11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand11Set(unsigned int urx_cec_operand11)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    o_rx_cec_cmd3.bits.rx_cec_operand11 = urx_cec_operand11;
    HDMITXRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_rx_cec_cmd3.u32, "RX_CEC_CMD3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_0_cec_op_abort_0Set
//  Description : Set the value of the member CEC_OP_ABORT_0.cec_op_abort_0
//  Input       : unsigned int ucec_op_abort_0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_0_cec_op_abort_0Set(unsigned int ucec_op_abort_0)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_0 o_cec_op_abort_0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_0.u32);
    o_cec_op_abort_0.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_0.bits.cec_op_abort_0 = ucec_op_abort_0;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_0.u32, "CEC_OP_ABORT_0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_1_cec_op_abort_1Set
//  Description : Set the value of the member CEC_OP_ABORT_1.cec_op_abort_1
//  Input       : unsigned int ucec_op_abort_1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_1_cec_op_abort_1Set(unsigned int ucec_op_abort_1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_1 o_cec_op_abort_1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_1.u32);
    o_cec_op_abort_1.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_1.bits.cec_op_abort_1 = ucec_op_abort_1;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_1.u32, "CEC_OP_ABORT_1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_2_cec_op_abort_2Set
//  Description : Set the value of the member CEC_OP_ABORT_2.cec_op_abort_2
//  Input       : unsigned int ucec_op_abort_2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_2_cec_op_abort_2Set(unsigned int ucec_op_abort_2)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_2 o_cec_op_abort_2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_2.u32);
    o_cec_op_abort_2.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_2.bits.cec_op_abort_2 = ucec_op_abort_2;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_2.u32, "CEC_OP_ABORT_2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_3_cec_op_abort_3Set
//  Description : Set the value of the member CEC_OP_ABORT_3.cec_op_abort_3
//  Input       : unsigned int ucec_op_abort_3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_3_cec_op_abort_3Set(unsigned int ucec_op_abort_3)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_3 o_cec_op_abort_3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_3.u32);
    o_cec_op_abort_3.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_3.bits.cec_op_abort_3 = ucec_op_abort_3;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_3.u32, "CEC_OP_ABORT_3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_4_cec_op_abort_4Set
//  Description : Set the value of the member CEC_OP_ABORT_4.cec_op_abort_4
//  Input       : unsigned int ucec_op_abort_4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_4_cec_op_abort_4Set(unsigned int ucec_op_abort_4)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_4 o_cec_op_abort_4;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_4.u32);
    o_cec_op_abort_4.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_4.bits.cec_op_abort_4 = ucec_op_abort_4;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_4.u32, "CEC_OP_ABORT_4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_5_cec_op_abort_5Set
//  Description : Set the value of the member CEC_OP_ABORT_5.cec_op_abort_5
//  Input       : unsigned int ucec_op_abort_5: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_5_cec_op_abort_5Set(unsigned int ucec_op_abort_5)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_5 o_cec_op_abort_5;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_5.u32);
    o_cec_op_abort_5.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_5.bits.cec_op_abort_5 = ucec_op_abort_5;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_5.u32, "CEC_OP_ABORT_5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_6_cec_op_abort_6Set
//  Description : Set the value of the member CEC_OP_ABORT_6.cec_op_abort_6
//  Input       : unsigned int ucec_op_abort_6: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_6_cec_op_abort_6Set(unsigned int ucec_op_abort_6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_6 o_cec_op_abort_6;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_6.u32);
    o_cec_op_abort_6.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_6.bits.cec_op_abort_6 = ucec_op_abort_6;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_6.u32, "CEC_OP_ABORT_6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_7_cec_op_abort_7Set
//  Description : Set the value of the member CEC_OP_ABORT_7.cec_op_abort_7
//  Input       : unsigned int ucec_op_abort_7: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_7_cec_op_abort_7Set(unsigned int ucec_op_abort_7)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_7 o_cec_op_abort_7;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_7.u32);
    o_cec_op_abort_7.u32 = HDMITXRegRead(pu32RegAddr);
    o_cec_op_abort_7.bits.cec_op_abort_7 = ucec_op_abort_7;
    HDMITXRegWrite(pu32RegAddr, o_cec_op_abort_7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cec_op_abort_7.u32, "CEC_OP_ABORT_7");

    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_hw_verGet
//  Description : Set the value of the member CEC_DEV_INFO.cec_hw_ver
//  Input       : unsigned int ucec_hw_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_hw_verGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_dev_info.bits.cec_hw_ver;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_sub_sysGet
//  Description : Set the value of the member CEC_DEV_INFO.cec_sub_sys
//  Input       : unsigned int ucec_sub_sys: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_sub_sysGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_dev_info.bits.cec_sub_sys;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_spec_suffixGet
//  Description : Set the value of the member CEC_DEV_INFO.cec_spec_suffix
//  Input       : unsigned int ucec_spec_suffix: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_spec_suffixGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_dev_info.bits.cec_spec_suffix;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_spec_verGet
//  Description : Set the value of the member CEC_DEV_INFO.cec_spec_ver
//  Input       : unsigned int ucec_spec_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_spec_verGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_dev_info.bits.cec_spec_ver;
}

//******************************************************************************
//  Function    : HDMI_CEC_DEV_INFO_cec_device_idGet
//  Description : Set the value of the member CEC_DEV_INFO.cec_device_id
//  Input       : unsigned int ucec_device_id: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_DEV_INFO_cec_device_idGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_DEV_INFO o_cec_dev_info;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_DEV_INFO.u32);
    o_cec_dev_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_dev_info.bits.cec_device_id;
}

//******************************************************************************
//  Function    : HDMI_CEC_START_PRIOD_cec_start_low_priodGet
//  Description : Set the value of the member CEC_START_PRIOD.cec_start_low_priod
//  Input       : unsigned int ucec_start_low_priod: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_START_PRIOD_cec_start_low_priodGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_START_PRIOD o_cec_start_priod;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_START_PRIOD.u32);
    o_cec_start_priod.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_start_priod.bits.cec_start_low_priod;
}

//******************************************************************************
//  Function    : HDMI_CEC_START_PRIOD_cec_start_bit_priodGet
//  Description : Set the value of the member CEC_START_PRIOD.cec_start_bit_priod
//  Input       : unsigned int ucec_start_bit_priod: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_START_PRIOD_cec_start_bit_priodGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_START_PRIOD o_cec_start_priod;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_START_PRIOD.u32);
    o_cec_start_priod.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_start_priod.bits.cec_start_bit_priod;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptyGet
//  Description : Set the value of the member CEC_TX_CTRL.auto_clear_intrp_rx_fifo_nempty
//  Input       : unsigned int uauto_clear_intrp_rx_fifo_nempty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.auto_clear_intrp_rx_fifo_nempty;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_rx_clr_all
//  Input       : unsigned int ucec_reg_rx_clr_all: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_rx_clr_all;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_rx_clr_cur_set
//  Input       : unsigned int ucec_reg_rx_clr_cur_set: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_rx_clr_cur_set;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_retry_limit
//  Input       : unsigned int ucec_reg_tx_retry_limit: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_tx_retry_limit;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_cmd_cnt
//  Input       : unsigned int ucec_reg_tx_cmd_cnt: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_tx_cmd_cnt;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_bfr_ac
//  Input       : unsigned int ucec_reg_tx_bfr_ac: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_tx_bfr_ac;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_tx_auto_calc
//  Input       : unsigned int ucec_reg_tx_auto_calc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_tx_auto_calc;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_manual_cmd_setGet
//  Description : Set the value of the member CEC_TX_CTRL.manual_cmd_set
//  Input       : unsigned int umanual_cmd_set: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_manual_cmd_setGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.manual_cmd_set;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_i2c_cec_passthru
//  Input       : unsigned int ucec_reg_i2c_cec_passthru: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_i2c_cec_passthru;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_force_non_calibGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_force_non_calib
//  Input       : unsigned int ucec_reg_force_non_calib: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_force_non_calibGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_force_non_calib;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_calib_cec_en
//  Input       : unsigned int ucec_reg_calib_cec_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_calib_cec_en;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_calib_cecGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_calib_cec
//  Input       : unsigned int ucec_reg_calib_cec: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_calib_cecGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_calib_cec;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_flush_tx_ff
//  Input       : unsigned int ucec_reg_flush_tx_ff: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_flush_tx_ff;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_ctl_retry_cntGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_ctl_retry_cnt
//  Input       : unsigned int ucec_ctl_retry_cnt: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_ctl_retry_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_ctl_retry_cnt;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_inv_ack_brcstGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_inv_ack_brcst
//  Input       : unsigned int ucec_inv_ack_brcst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_inv_ack_brcstGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_inv_ack_brcst;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_nack_hdrGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_nack_hdr
//  Input       : unsigned int ucec_reg_nack_hdr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_nack_hdrGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_nack_hdr;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_reg_snoopGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_reg_snoop
//  Input       : unsigned int ucec_reg_snoop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_reg_snoopGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_reg_snoop;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_snoop_initiatorGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_snoop_initiator
//  Input       : unsigned int ucec_snoop_initiator: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_snoop_initiatorGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_snoop_initiator;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_CTRL_cec_bus_ownerGet
//  Description : Set the value of the member CEC_TX_CTRL.cec_bus_owner
//  Input       : unsigned int ucec_bus_owner: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_CTRL_cec_bus_ownerGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_CTRL o_cec_tx_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_CTRL.u32);
    o_cec_tx_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_ctrl.bits.cec_bus_owner;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerGet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_init_cmd_header
//  Input       : unsigned int ucec_rx_init_cmd_header: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_rx_state.bits.cec_rx_init_cmd_header;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerGet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_dest_cmd_header
//  Input       : unsigned int ucec_rx_dest_cmd_header: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_rx_state.bits.cec_rx_dest_cmd_header;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_rx_errorGet
//  Description : Set the value of the member CEC_RX_STATE.cec_rx_error
//  Input       : unsigned int ucec_rx_error: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_rx_errorGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_rx_state.bits.cec_rx_error;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selGet
//  Description : Set the value of the member CEC_RX_STATE.cec_reg_rx_ff_wr_sel
//  Input       : unsigned int ucec_reg_rx_ff_wr_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_rx_state.bits.cec_reg_rx_ff_wr_sel;
}

//******************************************************************************
//  Function    : HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntGet
//  Description : Set the value of the member CEC_RX_STATE.cec_reg_rx_cmd_byte_cnt
//  Input       : unsigned int ucec_reg_rx_cmd_byte_cnt: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_RX_STATE o_cec_rx_state;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_RX_STATE.u32);
    o_cec_rx_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_rx_state.bits.cec_reg_rx_cmd_byte_cnt;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_doneGet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_done
//  Input       : unsigned int ucec_auto_ping_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_auto_disc.bits.cec_auto_ping_done;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_clearGet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_clear
//  Input       : unsigned int ucec_auto_ping_clear: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_clearGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_auto_disc.bits.cec_auto_ping_clear;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTO_DISC_cec_auto_ping_startGet
//  Description : Set the value of the member CEC_AUTO_DISC.cec_auto_ping_start
//  Input       : unsigned int ucec_auto_ping_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTO_DISC_cec_auto_ping_startGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTO_DISC o_cec_auto_disc;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTO_DISC.u32);
    o_cec_auto_disc.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_auto_disc.bits.cec_auto_ping_start;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_opcodeGet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_opcode
//  Input       : unsigned int ucec_cdc_opcode: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_opcodeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_cdc_ctrl.bits.cec_cdc_opcode;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_arb_enGet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_arb_en
//  Input       : unsigned int ucec_cdc_arb_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_cdc_ctrl.bits.cec_cdc_arb_en;
}

//******************************************************************************
//  Function    : HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntGet
//  Description : Set the value of the member CEC_CDC_CTRL.cec_cdc_arb_cnt
//  Input       : unsigned int ucec_cdc_arb_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CDC_CTRL o_cec_cdc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CDC_CTRL.u32);
    o_cec_cdc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_cdc_ctrl.bits.cec_cdc_arb_cnt;
}

//******************************************************************************
//  Function    : HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapGet
//  Description : Set the value of the member CEC_AUTODISC_MAP.cec_auto_ping_map
//  Input       : unsigned int ucec_auto_ping_map: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_AUTODISC_MAP o_cec_autodisc_map;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_AUTODISC_MAP.u32);
    o_cec_autodisc_map.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_autodisc_map.bits.cec_auto_ping_map;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_INIT_cec_tx_init_idGet
//  Description : Set the value of the member CEC_TX_INIT.cec_tx_init_id
//  Input       : unsigned int ucec_tx_init_id: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_INIT_cec_tx_init_idGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_INIT o_cec_tx_init;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_INIT.u32);
    o_cec_tx_init.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_init.bits.cec_tx_init_id;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_DEST_cec_reg_sd_poll_internGet
//  Description : Set the value of the member CEC_TX_DEST.cec_reg_sd_poll_intern
//  Input       : unsigned int ucec_reg_sd_poll_intern: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_DEST_cec_reg_sd_poll_internGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_DEST o_cec_tx_dest;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_DEST.u32);
    o_cec_tx_dest.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_dest.bits.cec_reg_sd_poll_intern;
}

//******************************************************************************
//  Function    : HDMI_CEC_TX_DEST_cec_tx_dest_idGet
//  Description : Set the value of the member CEC_TX_DEST.cec_tx_dest_id
//  Input       : unsigned int ucec_tx_dest_id: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_TX_DEST_cec_tx_dest_idGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_TX_DEST o_cec_tx_dest;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_TX_DEST.u32);
    o_cec_tx_dest.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_tx_dest.bits.cec_tx_dest_id;
}

//******************************************************************************
//  Function    : HDMI_CEC_CAPTUTR_ID_cec_capture_idGet
//  Description : Set the value of the member CEC_CAPTUTR_ID.cec_capture_id
//  Input       : unsigned int ucec_capture_id: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_CAPTUTR_ID_cec_capture_idGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_CAPTUTR_ID o_cec_captutr_id;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_CAPTUTR_ID.u32);
    o_cec_captutr_id.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_captutr_id.bits.cec_capture_id;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand2Get
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand2
//  Input       : unsigned int utx_cec_operand2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd0.bits.tx_cec_operand2;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand1Get
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand1
//  Input       : unsigned int utx_cec_operand1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd0.bits.tx_cec_operand1;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_operand0Get
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_operand0
//  Input       : unsigned int utx_cec_operand0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_operand0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd0.bits.tx_cec_operand0;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD0_tx_cec_commandGet
//  Description : Set the value of the member TX_CEC_CMD0.tx_cec_command
//  Input       : unsigned int utx_cec_command: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD0_tx_cec_commandGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD0 o_tx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD0.u32);
    o_tx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd0.bits.tx_cec_command;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand6Get
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand6
//  Input       : unsigned int utx_cec_operand6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd1.bits.tx_cec_operand6;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand5Get
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand5
//  Input       : unsigned int utx_cec_operand5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd1.bits.tx_cec_operand5;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand4Get
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand4
//  Input       : unsigned int utx_cec_operand4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd1.bits.tx_cec_operand4;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD1_tx_cec_operand3Get
//  Description : Set the value of the member TX_CEC_CMD1.tx_cec_operand3
//  Input       : unsigned int utx_cec_operand3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD1_tx_cec_operand3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD1 o_tx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD1.u32);
    o_tx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd1.bits.tx_cec_operand3;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand10Get
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand10
//  Input       : unsigned int utx_cec_operand10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd2.bits.tx_cec_operand10;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand9Get
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand9
//  Input       : unsigned int utx_cec_operand9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd2.bits.tx_cec_operand9;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand8Get
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand8
//  Input       : unsigned int utx_cec_operand8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd2.bits.tx_cec_operand8;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD2_tx_cec_operand7Get
//  Description : Set the value of the member TX_CEC_CMD2.tx_cec_operand7
//  Input       : unsigned int utx_cec_operand7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD2_tx_cec_operand7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD2 o_tx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD2.u32);
    o_tx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd2.bits.tx_cec_operand7;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand14Get
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand14
//  Input       : unsigned int utx_cec_operand14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd3.bits.tx_cec_operand14;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand13Get
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand13
//  Input       : unsigned int utx_cec_operand13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd3.bits.tx_cec_operand13;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand12Get
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand12
//  Input       : unsigned int utx_cec_operand12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd3.bits.tx_cec_operand12;
}

//******************************************************************************
//  Function    : HDMI_TX_CEC_CMD3_tx_cec_operand11Get
//  Description : Set the value of the member TX_CEC_CMD3.tx_cec_operand11
//  Input       : unsigned int utx_cec_operand11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_CEC_CMD3_tx_cec_operand11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_CEC_CMD3 o_tx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->TX_CEC_CMD3.u32);
    o_tx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_cec_cmd3.bits.tx_cec_operand11;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand2Get
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand2
//  Input       : unsigned int urx_cec_operand2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd0.bits.rx_cec_operand2;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand1Get
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand1
//  Input       : unsigned int urx_cec_operand1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd0.bits.rx_cec_operand1;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_operand0Get
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_operand0
//  Input       : unsigned int urx_cec_operand0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_operand0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd0.bits.rx_cec_operand0;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD0_rx_cec_commandGet
//  Description : Set the value of the member RX_CEC_CMD0.rx_cec_command
//  Input       : unsigned int urx_cec_command: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD0_rx_cec_commandGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD0 o_rx_cec_cmd0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD0.u32);
    o_rx_cec_cmd0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd0.bits.rx_cec_command;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand6Get
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand6
//  Input       : unsigned int urx_cec_operand6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd1.bits.rx_cec_operand6;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand5Get
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand5
//  Input       : unsigned int urx_cec_operand5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd1.bits.rx_cec_operand5;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand4Get
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand4
//  Input       : unsigned int urx_cec_operand4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd1.bits.rx_cec_operand4;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD1_rx_cec_operand3Get
//  Description : Set the value of the member RX_CEC_CMD1.rx_cec_operand3
//  Input       : unsigned int urx_cec_operand3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD1_rx_cec_operand3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD1 o_rx_cec_cmd1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD1.u32);
    o_rx_cec_cmd1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd1.bits.rx_cec_operand3;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand10Get
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand10
//  Input       : unsigned int urx_cec_operand10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd2.bits.rx_cec_operand10;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand9Get
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand9
//  Input       : unsigned int urx_cec_operand9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd2.bits.rx_cec_operand9;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand8Get
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand8
//  Input       : unsigned int urx_cec_operand8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd2.bits.rx_cec_operand8;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD2_rx_cec_operand7Get
//  Description : Set the value of the member RX_CEC_CMD2.rx_cec_operand7
//  Input       : unsigned int urx_cec_operand7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD2_rx_cec_operand7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD2 o_rx_cec_cmd2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD2.u32);
    o_rx_cec_cmd2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd2.bits.rx_cec_operand7;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand14Get
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand14
//  Input       : unsigned int urx_cec_operand14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd3.bits.rx_cec_operand14;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand13Get
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand13
//  Input       : unsigned int urx_cec_operand13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd3.bits.rx_cec_operand13;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand12Get
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand12
//  Input       : unsigned int urx_cec_operand12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd3.bits.rx_cec_operand12;
}

//******************************************************************************
//  Function    : HDMI_RX_CEC_CMD3_rx_cec_operand11Get
//  Description : Set the value of the member RX_CEC_CMD3.rx_cec_operand11
//  Input       : unsigned int urx_cec_operand11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_RX_CEC_CMD3_rx_cec_operand11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_RX_CEC_CMD3 o_rx_cec_cmd3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->RX_CEC_CMD3.u32);
    o_rx_cec_cmd3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_rx_cec_cmd3.bits.rx_cec_operand11;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_0_cec_op_abort_0Get
//  Description : Set the value of the member CEC_OP_ABORT_0.cec_op_abort_0
//  Input       : unsigned int ucec_op_abort_0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_0_cec_op_abort_0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_0 o_cec_op_abort_0;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_0.u32);
    o_cec_op_abort_0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_0.bits.cec_op_abort_0;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_1_cec_op_abort_1Get
//  Description : Set the value of the member CEC_OP_ABORT_1.cec_op_abort_1
//  Input       : unsigned int ucec_op_abort_1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_1_cec_op_abort_1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_1 o_cec_op_abort_1;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_1.u32);
    o_cec_op_abort_1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_1.bits.cec_op_abort_1;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_2_cec_op_abort_2Get
//  Description : Set the value of the member CEC_OP_ABORT_2.cec_op_abort_2
//  Input       : unsigned int ucec_op_abort_2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_2_cec_op_abort_2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_2 o_cec_op_abort_2;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_2.u32);
    o_cec_op_abort_2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_2.bits.cec_op_abort_2;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_3_cec_op_abort_3Get
//  Description : Set the value of the member CEC_OP_ABORT_3.cec_op_abort_3
//  Input       : unsigned int ucec_op_abort_3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_3_cec_op_abort_3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_3 o_cec_op_abort_3;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_3.u32);
    o_cec_op_abort_3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_3.bits.cec_op_abort_3;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_4_cec_op_abort_4Get
//  Description : Set the value of the member CEC_OP_ABORT_4.cec_op_abort_4
//  Input       : unsigned int ucec_op_abort_4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_4_cec_op_abort_4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_4 o_cec_op_abort_4;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_4.u32);
    o_cec_op_abort_4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_4.bits.cec_op_abort_4;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_5_cec_op_abort_5Get
//  Description : Set the value of the member CEC_OP_ABORT_5.cec_op_abort_5
//  Input       : unsigned int ucec_op_abort_5: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_5_cec_op_abort_5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_5 o_cec_op_abort_5;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_5.u32);
    o_cec_op_abort_5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_5.bits.cec_op_abort_5;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_6_cec_op_abort_6Get
//  Description : Set the value of the member CEC_OP_ABORT_6.cec_op_abort_6
//  Input       : unsigned int ucec_op_abort_6: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_6_cec_op_abort_6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_6 o_cec_op_abort_6;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_6.u32);
    o_cec_op_abort_6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_6.bits.cec_op_abort_6;
}

//******************************************************************************
//  Function    : HDMI_CEC_OP_ABORT_7_cec_op_abort_7Get
//  Description : Set the value of the member CEC_OP_ABORT_7.cec_op_abort_7
//  Input       : unsigned int ucec_op_abort_7: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEC_OP_ABORT_7_cec_op_abort_7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_CEC_OP_ABORT_7 o_cec_op_abort_7;
    pu32RegAddr = (HI_U32*)&(goptx_cec_regAllReg->CEC_OP_ABORT_7.u32);
    o_cec_op_abort_7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cec_op_abort_7.bits.cec_op_abort_7;
}

