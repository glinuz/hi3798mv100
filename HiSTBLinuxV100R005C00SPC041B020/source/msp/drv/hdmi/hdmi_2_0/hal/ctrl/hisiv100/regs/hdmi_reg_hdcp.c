// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_hdcp_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.1 
// History       :  w00271848 2015/12/22 20:06:28 Create file
// ******************************************************************************

#include "hi_type.h"
#include "hdmi_reg_hdcp.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif
/* Define the struct pointor of the module tx_hdcp_reg */
volatile S_tx_hdcp_reg_REGS_TYPE *goptx_hdcp_regAllReg = NULL;


int HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_Init(void)
{
    if (goptx_hdcp_regAllReg)
    {
        return HI_SUCCESS;
    }

    goptx_hdcp_regAllReg = (volatile S_tx_hdcp_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_hdcp_reg,4);

    if (!goptx_hdcp_regAllReg)
    {
        HDMI_ERR("goptx_hdcp_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_hdcp_regAllReg)
    {
        HDMI_IO_UNMAP(goptx_hdcp_regAllReg);
        goptx_hdcp_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}




//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist2_err
//  Input       : unsigned int uhdcp1x_bist2_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errSet(unsigned int uhdcp1x_bist2_err)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = 0;
    o_hdcp1x_mem_ctrl.bits.hdcp1x_bist2_err = uhdcp1x_bist2_err;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist2_err
//  Input       : unsigned int uhdcp1x_bist2_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_bist2_err;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist1_err
//  Input       : unsigned int uhdcp1x_bist1_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errSet(unsigned int uhdcp1x_bist1_err)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = 0;
    o_hdcp1x_mem_ctrl.bits.hdcp1x_bist1_err = uhdcp1x_bist1_err;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist1_err
//  Input       : unsigned int uhdcp1x_bist1_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_bist1_err;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist0_err
//  Input       : unsigned int uhdcp1x_bist0_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errSet(unsigned int uhdcp1x_bist0_err)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = 0;
    o_hdcp1x_mem_ctrl.bits.hdcp1x_bist0_err = uhdcp1x_bist0_err;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_bist0_err
//  Input       : unsigned int uhdcp1x_bist0_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_bist0_err;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_cmd_done
//  Input       : unsigned int uhdcp1x_cmd_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet(unsigned int uhdcp1x_cmd_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = 0;
    o_hdcp1x_mem_ctrl.bits.hdcp1x_cmd_done = uhdcp1x_cmd_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_cmd_done
//  Input       : unsigned int uhdcp1x_cmd_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_cmd_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_cmd_valid
//  Input       : unsigned int uhdcp1x_key_cmd_valid: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validSet(unsigned int uhdcp1x_key_cmd_valid)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_mem_ctrl.bits.hdcp1x_key_cmd_valid = uhdcp1x_key_cmd_valid;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_cmd_valid
//  Input       : unsigned int uhdcp1x_key_cmd_valid: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_key_cmd_valid;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_cmd
//  Input       : unsigned int uhdcp1x_key_cmd: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet(unsigned int uhdcp1x_key_cmd)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_mem_ctrl.bits.hdcp1x_key_cmd = uhdcp1x_key_cmd;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_cmd
//  Input       : unsigned int uhdcp1x_key_cmd: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_key_cmd;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadSet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_load
//  Input       : unsigned int uhdcp1x_key_load: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadSet(unsigned int uhdcp1x_key_load)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_mem_ctrl.bits.hdcp1x_key_load = uhdcp1x_key_load;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_mem_ctrl.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadGet
//  Description : Set the value of the member HDCP1X_MEM_CTRL.hdcp1x_key_load
//  Input       : unsigned int uhdcp1x_key_load: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_MEM_CTRL o_hdcp1x_mem_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_MEM_CTRL.u32);
    o_hdcp1x_mem_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_mem_ctrl.bits.hdcp1x_key_load;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Set
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte3
//  Input       : unsigned int uhdcp1x_key_aksv_byte3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Set(unsigned int uhdcp1x_key_aksv_byte3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte3 = uhdcp1x_key_aksv_byte3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32, "HDCP1X_KEY_AKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Get
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte3
//  Input       : unsigned int uhdcp1x_key_aksv_byte3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte3;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Set
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte2
//  Input       : unsigned int uhdcp1x_key_aksv_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Set(unsigned int uhdcp1x_key_aksv_byte2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte2 = uhdcp1x_key_aksv_byte2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32, "HDCP1X_KEY_AKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Get
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte2
//  Input       : unsigned int uhdcp1x_key_aksv_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte2;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Set
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte1
//  Input       : unsigned int uhdcp1x_key_aksv_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Set(unsigned int uhdcp1x_key_aksv_byte1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte1 = uhdcp1x_key_aksv_byte1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32, "HDCP1X_KEY_AKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Get
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte1
//  Input       : unsigned int uhdcp1x_key_aksv_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Set
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte0
//  Input       : unsigned int uhdcp1x_key_aksv_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Set(unsigned int uhdcp1x_key_aksv_byte0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte0 = uhdcp1x_key_aksv_byte0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_key_aksv0.u32, "HDCP1X_KEY_AKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Get
//  Description : Set the value of the member HDCP1X_KEY_AKSV0.hdcp1x_key_aksv_byte0
//  Input       : unsigned int uhdcp1x_key_aksv_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV0 o_hdcp1x_key_aksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV0.u32);
    o_hdcp1x_key_aksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_key_aksv0.bits.hdcp1x_key_aksv_byte0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Set
//  Description : Set the value of the member HDCP1X_KEY_AKSV1.hdcp1x_key_aksv_byte4
//  Input       : unsigned int uhdcp1x_key_aksv_byte4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Set(unsigned int uhdcp1x_key_aksv_byte4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV1 o_hdcp1x_key_aksv1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV1.u32);
    o_hdcp1x_key_aksv1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_key_aksv1.bits.hdcp1x_key_aksv_byte4 = uhdcp1x_key_aksv_byte4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_key_aksv1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_key_aksv1.u32, "HDCP1X_KEY_AKSV1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Get
//  Description : Set the value of the member HDCP1X_KEY_AKSV1.hdcp1x_key_aksv_byte4
//  Input       : unsigned int uhdcp1x_key_aksv_byte4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_KEY_AKSV1 o_hdcp1x_key_aksv1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_KEY_AKSV1.u32);
    o_hdcp1x_key_aksv1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_key_aksv1.bits.hdcp1x_key_aksv_byte4;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlSet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp2x_amute_ctrl
//  Input       : unsigned int uhdcp2x_amute_ctrl: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlSet(unsigned int uhdcp2x_amute_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_mute_ctrl.bits.hdcp2x_amute_ctrl = uhdcp2x_amute_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32, "HDCP_MUTE_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlGet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp2x_amute_ctrl
//  Input       : unsigned int uhdcp2x_amute_ctrl: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_mute_ctrl.bits.hdcp2x_amute_ctrl;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlSet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp2x_vmute_ctrl
//  Input       : unsigned int uhdcp2x_vmute_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlSet(unsigned int uhdcp2x_vmute_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_mute_ctrl.bits.hdcp2x_vmute_ctrl = uhdcp2x_vmute_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32, "HDCP_MUTE_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlGet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp2x_vmute_ctrl
//  Input       : unsigned int uhdcp2x_vmute_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_mute_ctrl.bits.hdcp2x_vmute_ctrl;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlSet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp1x_amute_ctrl
//  Input       : unsigned int uhdcp1x_amute_ctrl: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlSet(unsigned int uhdcp1x_amute_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_mute_ctrl.bits.hdcp1x_amute_ctrl = uhdcp1x_amute_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32, "HDCP_MUTE_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlGet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp1x_amute_ctrl
//  Input       : unsigned int uhdcp1x_amute_ctrl: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_mute_ctrl.bits.hdcp1x_amute_ctrl;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp1x_vmute_ctrl
//  Input       : unsigned int uhdcp1x_vmute_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet(unsigned int uhdcp1x_vmute_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_mute_ctrl.bits.hdcp1x_vmute_ctrl = uhdcp1x_vmute_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_mute_ctrl.u32, "HDCP_MUTE_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlGet
//  Description : Set the value of the member HDCP_MUTE_CTRL.hdcp1x_vmute_ctrl
//  Input       : unsigned int uhdcp1x_vmute_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_MUTE_CTRL o_hdcp_mute_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_MUTE_CTRL.u32);
    o_hdcp_mute_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_mute_ctrl.bits.hdcp1x_vmute_ctrl;
}

//******************************************************************************
//  Function    : HDMI_HDCP_FUN_SEL_hdcp_fun_selSet
//  Description : Set the value of the member HDCP_FUN_SEL.hdcp_fun_sel
//  Input       : unsigned int uhdcp_fun_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_FUN_SEL_hdcp_fun_selSet(unsigned int uhdcp_fun_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_FUN_SEL o_hdcp_fun_sel;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_FUN_SEL.u32);
    o_hdcp_fun_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_fun_sel.bits.hdcp_fun_sel = uhdcp_fun_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_fun_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_fun_sel.u32, "HDCP_FUN_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_FUN_SEL_hdcp_fun_selGet
//  Description : Set the value of the member HDCP_FUN_SEL.hdcp_fun_sel
//  Input       : unsigned int uhdcp_fun_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_FUN_SEL_hdcp_fun_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP_FUN_SEL o_hdcp_fun_sel;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP_FUN_SEL.u32);
    o_hdcp_fun_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_fun_sel.bits.hdcp_fun_sel;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onSet
//  Description : Set the value of the member HDCP1X_SEC_CFG.hdcp1x_sec_rpt_on
//  Input       : unsigned int uhdcp1x_sec_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onSet(unsigned int uhdcp1x_sec_rpt_on)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SEC_CFG o_hdcp1x_sec_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SEC_CFG.u32);
    o_hdcp1x_sec_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sec_cfg.bits.hdcp1x_sec_rpt_on = uhdcp1x_sec_rpt_on;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sec_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sec_cfg.u32, "HDCP1X_SEC_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onGet
//  Description : Set the value of the member HDCP1X_SEC_CFG.hdcp1x_sec_rpt_on
//  Input       : unsigned int uhdcp1x_sec_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SEC_CFG o_hdcp1x_sec_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SEC_CFG.u32);
    o_hdcp1x_sec_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sec_cfg.bits.hdcp1x_sec_rpt_on;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enSet
//  Description : Set the value of the member HDCP1X_SEC_CFG.hdcp1x_sec_enc_en
//  Input       : unsigned int uhdcp1x_sec_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enSet(unsigned int uhdcp1x_sec_enc_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SEC_CFG o_hdcp1x_sec_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SEC_CFG.u32);
    o_hdcp1x_sec_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sec_cfg.bits.hdcp1x_sec_enc_en = uhdcp1x_sec_enc_en;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sec_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sec_cfg.u32, "HDCP1X_SEC_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enGet
//  Description : Set the value of the member HDCP1X_SEC_CFG.hdcp1x_sec_enc_en
//  Input       : unsigned int uhdcp1x_sec_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SEC_CFG o_hdcp1x_sec_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SEC_CFG.u32);
    o_hdcp1x_sec_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sec_cfg.bits.hdcp1x_sec_enc_en;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_comp
//  Input       : unsigned int uhdcp1x_ri_comp: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compSet(unsigned int uhdcp1x_ri_comp)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_comp = uhdcp1x_ri_comp;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_comp
//  Input       : unsigned int uhdcp1x_ri_comp: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_comp;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_hold
//  Input       : unsigned int uhdcp1x_ri_hold: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdSet(unsigned int uhdcp1x_ri_hold)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_hold = uhdcp1x_ri_hold;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_hold
//  Input       : unsigned int uhdcp1x_ri_hold: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_hold;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_trush
//  Input       : unsigned int uhdcp1x_ri_trush: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushSet(unsigned int uhdcp1x_ri_trush)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_trush = uhdcp1x_ri_trush;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_ri_trush
//  Input       : unsigned int uhdcp1x_ri_trush: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_ri_trush;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_rpt_on
//  Input       : unsigned int uhdcp1x_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onSet(unsigned int uhdcp1x_rpt_on)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_rpt_on = uhdcp1x_rpt_on;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_rpt_on
//  Input       : unsigned int uhdcp1x_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_rpt_on;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_an_stop
//  Input       : unsigned int uhdcp1x_an_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopSet(unsigned int uhdcp1x_an_stop)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_an_stop = uhdcp1x_an_stop;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_an_stop
//  Input       : unsigned int uhdcp1x_an_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_an_stop;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enSet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_enc_en
//  Input       : unsigned int uhdcp1x_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enSet(unsigned int uhdcp1x_enc_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ctrl.bits.hdcp1x_enc_en = uhdcp1x_enc_en;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ctrl.u32, "HDCP1X_ENG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enGet
//  Description : Set the value of the member HDCP1X_ENG_CTRL.hdcp1x_enc_en
//  Input       : unsigned int uhdcp1x_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_CTRL o_hdcp1x_eng_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_CTRL.u32);
    o_hdcp1x_eng_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ctrl.bits.hdcp1x_enc_en;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdySet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_ri_rdy
//  Input       : unsigned int uhdcp1x_ri_rdy: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdySet(unsigned int uhdcp1x_ri_rdy)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_status.bits.hdcp1x_ri_rdy = uhdcp1x_ri_rdy;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32, "HDCP1X_ENG_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdyGet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_ri_rdy
//  Input       : unsigned int uhdcp1x_ri_rdy: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_status.bits.hdcp1x_ri_rdy;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errSet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_bksv_err
//  Input       : unsigned int uhdcp1x_bksv_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errSet(unsigned int uhdcp1x_bksv_err)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_status.bits.hdcp1x_bksv_err = uhdcp1x_bksv_err;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32, "HDCP1X_ENG_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errGet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_bksv_err
//  Input       : unsigned int uhdcp1x_bksv_err: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_status.bits.hdcp1x_bksv_err;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntSet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_i_cnt
//  Input       : unsigned int uhdcp1x_i_cnt: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntSet(unsigned int uhdcp1x_i_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_status.bits.hdcp1x_i_cnt = uhdcp1x_i_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_status.u32, "HDCP1X_ENG_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntGet
//  Description : Set the value of the member HDCP1X_ENG_STATUS.hdcp1x_i_cnt
//  Input       : unsigned int uhdcp1x_i_cnt: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_STATUS o_hdcp1x_eng_status;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_STATUS.u32);
    o_hdcp1x_eng_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_status.bits.hdcp1x_i_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Set
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte3
//  Input       : unsigned int uhdcp1x_key_bksv_byte3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Set(unsigned int uhdcp1x_key_bksv_byte3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte3 = uhdcp1x_key_bksv_byte3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32, "HDCP1X_ENG_BKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Get
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte3
//  Input       : unsigned int uhdcp1x_key_bksv_byte3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte3;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Set
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte2
//  Input       : unsigned int uhdcp1x_key_bksv_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Set(unsigned int uhdcp1x_key_bksv_byte2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte2 = uhdcp1x_key_bksv_byte2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32, "HDCP1X_ENG_BKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Get
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte2
//  Input       : unsigned int uhdcp1x_key_bksv_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte2;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Set
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte1
//  Input       : unsigned int uhdcp1x_key_bksv_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Set(unsigned int uhdcp1x_key_bksv_byte1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte1 = uhdcp1x_key_bksv_byte1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32, "HDCP1X_ENG_BKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Get
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte1
//  Input       : unsigned int uhdcp1x_key_bksv_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Set
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte0
//  Input       : unsigned int uhdcp1x_key_bksv_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Set(unsigned int uhdcp1x_key_bksv_byte0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte0 = uhdcp1x_key_bksv_byte0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv0.u32, "HDCP1X_ENG_BKSV0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Get
//  Description : Set the value of the member HDCP1X_ENG_BKSV0.hdcp1x_key_bksv_byte0
//  Input       : unsigned int uhdcp1x_key_bksv_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV0 o_hdcp1x_eng_bksv0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV0.u32);
    o_hdcp1x_eng_bksv0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_bksv0.bits.hdcp1x_key_bksv_byte0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Set
//  Description : Set the value of the member HDCP1X_ENG_BKSV1.hdcp1x_key_bksv_byte4
//  Input       : unsigned int uhdcp1x_key_bksv_byte4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Set(unsigned int uhdcp1x_key_bksv_byte4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV1 o_hdcp1x_eng_bksv1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV1.u32);
    o_hdcp1x_eng_bksv1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_bksv1.bits.hdcp1x_key_bksv_byte4 = uhdcp1x_key_bksv_byte4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_bksv1.u32, "HDCP1X_ENG_BKSV1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Get
//  Description : Set the value of the member HDCP1X_ENG_BKSV1.hdcp1x_key_bksv_byte4
//  Input       : unsigned int uhdcp1x_key_bksv_byte4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_BKSV1 o_hdcp1x_eng_bksv1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_BKSV1.u32);
    o_hdcp1x_eng_bksv1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_bksv1.bits.hdcp1x_key_bksv_byte4;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Set
//  Description : Set the value of the member HDCP1X_ENG_GEN_AN0.hdcp1x_eng_gen_an0
//  Input       : unsigned int uhdcp1x_eng_gen_an0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Set(unsigned int uhdcp1x_eng_gen_an0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_GEN_AN0 o_hdcp1x_eng_gen_an0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_GEN_AN0.u32);
    o_hdcp1x_eng_gen_an0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_gen_an0.bits.hdcp1x_eng_gen_an0 = uhdcp1x_eng_gen_an0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_gen_an0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_gen_an0.u32, "HDCP1X_ENG_GEN_AN0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Get
//  Description : Set the value of the member HDCP1X_ENG_GEN_AN0.hdcp1x_eng_gen_an0
//  Input       : unsigned int uhdcp1x_eng_gen_an0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_GEN_AN0 o_hdcp1x_eng_gen_an0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_GEN_AN0.u32);
    o_hdcp1x_eng_gen_an0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_gen_an0.bits.hdcp1x_eng_gen_an0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Set
//  Description : Set the value of the member HDCP1X_ENG_GEN_AN1.hdcp1x_eng_gen_an1
//  Input       : unsigned int uhdcp1x_eng_gen_an1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Set(unsigned int uhdcp1x_eng_gen_an1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_GEN_AN1 o_hdcp1x_eng_gen_an1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_GEN_AN1.u32);
    o_hdcp1x_eng_gen_an1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_gen_an1.bits.hdcp1x_eng_gen_an1 = uhdcp1x_eng_gen_an1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_gen_an1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_gen_an1.u32, "HDCP1X_ENG_GEN_AN1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Get
//  Description : Set the value of the member HDCP1X_ENG_GEN_AN1.hdcp1x_eng_gen_an1
//  Input       : unsigned int uhdcp1x_eng_gen_an1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_GEN_AN1 o_hdcp1x_eng_gen_an1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_GEN_AN1.u32);
    o_hdcp1x_eng_gen_an1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_gen_an1.bits.hdcp1x_eng_gen_an1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Set
//  Description : Set the value of the member HDCP1X_ENG_FUN_AN0.hdcp1x_eng_fun_an0
//  Input       : unsigned int uhdcp1x_eng_fun_an0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Set(unsigned int uhdcp1x_eng_fun_an0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_FUN_AN0 o_hdcp1x_eng_fun_an0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_FUN_AN0.u32);
    o_hdcp1x_eng_fun_an0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_fun_an0.bits.hdcp1x_eng_fun_an0 = uhdcp1x_eng_fun_an0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_fun_an0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_fun_an0.u32, "HDCP1X_ENG_FUN_AN0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Get
//  Description : Set the value of the member HDCP1X_ENG_FUN_AN0.hdcp1x_eng_fun_an0
//  Input       : unsigned int uhdcp1x_eng_fun_an0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_FUN_AN0 o_hdcp1x_eng_fun_an0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_FUN_AN0.u32);
    o_hdcp1x_eng_fun_an0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_fun_an0.bits.hdcp1x_eng_fun_an0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Set
//  Description : Set the value of the member HDCP1X_ENG_FUN_AN1.hdcp1x_eng_fun_an1
//  Input       : unsigned int uhdcp1x_eng_fun_an1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Set(unsigned int uhdcp1x_eng_fun_an1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_FUN_AN1 o_hdcp1x_eng_fun_an1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_FUN_AN1.u32);
    o_hdcp1x_eng_fun_an1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_fun_an1.bits.hdcp1x_eng_fun_an1 = uhdcp1x_eng_fun_an1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_fun_an1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_fun_an1.u32, "HDCP1X_ENG_FUN_AN1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Get
//  Description : Set the value of the member HDCP1X_ENG_FUN_AN1.hdcp1x_eng_fun_an1
//  Input       : unsigned int uhdcp1x_eng_fun_an1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_FUN_AN1 o_hdcp1x_eng_fun_an1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_FUN_AN1.u32);
    o_hdcp1x_eng_fun_an1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_fun_an1.bits.hdcp1x_eng_fun_an1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Set
//  Description : Set the value of the member HDCP1X_ENG_RI.hdcp1x_eng_ri1
//  Input       : unsigned int uhdcp1x_eng_ri1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Set(unsigned int uhdcp1x_eng_ri1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_RI o_hdcp1x_eng_ri;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_RI.u32);
    o_hdcp1x_eng_ri.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ri.bits.hdcp1x_eng_ri1 = uhdcp1x_eng_ri1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ri.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ri.u32, "HDCP1X_ENG_RI");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Get
//  Description : Set the value of the member HDCP1X_ENG_RI.hdcp1x_eng_ri1
//  Input       : unsigned int uhdcp1x_eng_ri1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_RI o_hdcp1x_eng_ri;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_RI.u32);
    o_hdcp1x_eng_ri.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ri.bits.hdcp1x_eng_ri1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Set
//  Description : Set the value of the member HDCP1X_ENG_RI.hdcp1x_eng_ri0
//  Input       : unsigned int uhdcp1x_eng_ri0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Set(unsigned int uhdcp1x_eng_ri0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_RI o_hdcp1x_eng_ri;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_RI.u32);
    o_hdcp1x_eng_ri.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_eng_ri.bits.hdcp1x_eng_ri0 = uhdcp1x_eng_ri0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_eng_ri.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_eng_ri.u32, "HDCP1X_ENG_RI");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Get
//  Description : Set the value of the member HDCP1X_ENG_RI.hdcp1x_eng_ri0
//  Input       : unsigned int uhdcp1x_eng_ri0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_ENG_RI o_hdcp1x_eng_ri;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_ENG_RI.u32);
    o_hdcp1x_eng_ri.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_eng_ri.bits.hdcp1x_eng_ri0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusSet
//  Description : Set the value of the member HDCP1X_RPT_BSTATUS.hdcp1x_bstatus
//  Input       : unsigned int uhdcp1x_bstatus: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusSet(unsigned int uhdcp1x_bstatus)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_BSTATUS o_hdcp1x_rpt_bstatus;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_BSTATUS.u32);
    o_hdcp1x_rpt_bstatus.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_bstatus.bits.hdcp1x_bstatus = uhdcp1x_bstatus;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_bstatus.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_bstatus.u32, "HDCP1X_RPT_BSTATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusGet
//  Description : Set the value of the member HDCP1X_RPT_BSTATUS.hdcp1x_bstatus
//  Input       : unsigned int uhdcp1x_bstatus: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_BSTATUS o_hdcp1x_rpt_bstatus;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_BSTATUS.u32);
    o_hdcp1x_rpt_bstatus.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_bstatus.bits.hdcp1x_bstatus;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistSet
//  Description : Set the value of the member HDCP1X_RPT_KLIST.hdcp1x_rpt_klist
//  Input       : unsigned int uhdcp1x_rpt_klist: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistSet(unsigned int uhdcp1x_rpt_klist)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_KLIST o_hdcp1x_rpt_klist;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_KLIST.u32);
    o_hdcp1x_rpt_klist.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_klist.bits.hdcp1x_rpt_klist = uhdcp1x_rpt_klist;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_klist.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_klist.u32, "HDCP1X_RPT_KLIST");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistGet
//  Description : Set the value of the member HDCP1X_RPT_KLIST.hdcp1x_rpt_klist
//  Input       : unsigned int uhdcp1x_rpt_klist: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_KLIST o_hdcp1x_rpt_klist;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_KLIST.u32);
    o_hdcp1x_rpt_klist.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_klist.bits.hdcp1x_rpt_klist;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readySet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_ready
//  Input       : unsigned int uhdcp1x_sha_ready: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readySet(unsigned int uhdcp1x_sha_ready)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_ready = uhdcp1x_sha_ready;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32, "HDCP1X_SHA_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readyGet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_ready
//  Input       : unsigned int uhdcp1x_sha_ready: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_ready;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsSet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_no_ds
//  Input       : unsigned int uhdcp1x_sha_no_ds: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsSet(unsigned int uhdcp1x_sha_no_ds)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_no_ds = uhdcp1x_sha_no_ds;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32, "HDCP1X_SHA_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsGet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_no_ds
//  Input       : unsigned int uhdcp1x_sha_no_ds: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_no_ds;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startSet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_start
//  Input       : unsigned int uhdcp1x_sha_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startSet(unsigned int uhdcp1x_sha_start)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_start = uhdcp1x_sha_start;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_ctrl.u32, "HDCP1X_SHA_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startGet
//  Description : Set the value of the member HDCP1X_SHA_CTRL.hdcp1x_sha_start
//  Input       : unsigned int uhdcp1x_sha_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CTRL o_hdcp1x_sha_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CTRL.u32);
    o_hdcp1x_sha_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_ctrl.bits.hdcp1x_sha_start;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Set
//  Description : Set the value of the member HDCP1X_SHA_V0.hdcp1x_rpt_vh0
//  Input       : unsigned int uhdcp1x_rpt_vh0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Set(unsigned int uhdcp1x_rpt_vh0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V0 o_hdcp1x_sha_v0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V0.u32);
    o_hdcp1x_sha_v0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_v0.bits.hdcp1x_rpt_vh0 = uhdcp1x_rpt_vh0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_v0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_v0.u32, "HDCP1X_SHA_V0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Get
//  Description : Set the value of the member HDCP1X_SHA_V0.hdcp1x_rpt_vh0
//  Input       : unsigned int uhdcp1x_rpt_vh0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V0 o_hdcp1x_sha_v0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V0.u32);
    o_hdcp1x_sha_v0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_v0.bits.hdcp1x_rpt_vh0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Set
//  Description : Set the value of the member HDCP1X_SHA_V1.hdcp1x_rpt_vh1
//  Input       : unsigned int uhdcp1x_rpt_vh1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Set(unsigned int uhdcp1x_rpt_vh1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V1 o_hdcp1x_sha_v1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V1.u32);
    o_hdcp1x_sha_v1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_v1.bits.hdcp1x_rpt_vh1 = uhdcp1x_rpt_vh1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_v1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_v1.u32, "HDCP1X_SHA_V1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Get
//  Description : Set the value of the member HDCP1X_SHA_V1.hdcp1x_rpt_vh1
//  Input       : unsigned int uhdcp1x_rpt_vh1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V1 o_hdcp1x_sha_v1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V1.u32);
    o_hdcp1x_sha_v1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_v1.bits.hdcp1x_rpt_vh1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Set
//  Description : Set the value of the member HDCP1X_SHA_V2.hdcp1x_rpt_vh2
//  Input       : unsigned int uhdcp1x_rpt_vh2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Set(unsigned int uhdcp1x_rpt_vh2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V2 o_hdcp1x_sha_v2;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V2.u32);
    o_hdcp1x_sha_v2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_v2.bits.hdcp1x_rpt_vh2 = uhdcp1x_rpt_vh2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_v2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_v2.u32, "HDCP1X_SHA_V2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Get
//  Description : Set the value of the member HDCP1X_SHA_V2.hdcp1x_rpt_vh2
//  Input       : unsigned int uhdcp1x_rpt_vh2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V2 o_hdcp1x_sha_v2;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V2.u32);
    o_hdcp1x_sha_v2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_v2.bits.hdcp1x_rpt_vh2;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Set
//  Description : Set the value of the member HDCP1X_SHA_V3.hdcp1x_rpt_vh3
//  Input       : unsigned int uhdcp1x_rpt_vh3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Set(unsigned int uhdcp1x_rpt_vh3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V3 o_hdcp1x_sha_v3;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V3.u32);
    o_hdcp1x_sha_v3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_v3.bits.hdcp1x_rpt_vh3 = uhdcp1x_rpt_vh3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_v3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_v3.u32, "HDCP1X_SHA_V3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Get
//  Description : Set the value of the member HDCP1X_SHA_V3.hdcp1x_rpt_vh3
//  Input       : unsigned int uhdcp1x_rpt_vh3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V3 o_hdcp1x_sha_v3;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V3.u32);
    o_hdcp1x_sha_v3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_v3.bits.hdcp1x_rpt_vh3;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Set
//  Description : Set the value of the member HDCP1X_SHA_V4.hdcp1x_rpt_vh4
//  Input       : unsigned int uhdcp1x_rpt_vh4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Set(unsigned int uhdcp1x_rpt_vh4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V4 o_hdcp1x_sha_v4;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V4.u32);
    o_hdcp1x_sha_v4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_v4.bits.hdcp1x_rpt_vh4 = uhdcp1x_rpt_vh4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_v4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_v4.u32, "HDCP1X_SHA_V4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Get
//  Description : Set the value of the member HDCP1X_SHA_V4.hdcp1x_rpt_vh4
//  Input       : unsigned int uhdcp1x_rpt_vh4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_V4 o_hdcp1x_sha_v4;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_V4.u32);
    o_hdcp1x_sha_v4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_v4.bits.hdcp1x_rpt_vh4;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enSet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_check_errcnt_en
//  Input       : unsigned int uhdcp1x_check_errcnt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enSet(unsigned int uhdcp1x_check_errcnt_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_ctrl.bits.hdcp1x_check_errcnt_en = uhdcp1x_check_errcnt_en;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32, "HDCP1X_CHK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enGet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_check_errcnt_en
//  Input       : unsigned int uhdcp1x_check_errcnt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_ctrl.bits.hdcp1x_check_errcnt_en;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selSet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_check_start_sel
//  Input       : unsigned int uhdcp1x_check_start_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selSet(unsigned int uhdcp1x_check_start_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_ctrl.bits.hdcp1x_check_start_sel = uhdcp1x_check_start_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32, "HDCP1X_CHK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selGet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_check_start_sel
//  Input       : unsigned int uhdcp1x_check_start_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_ctrl.bits.hdcp1x_check_start_sel;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_prioritySet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_ddc_req_priority
//  Input       : unsigned int uhdcp1x_ddc_req_priority: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_prioritySet(unsigned int uhdcp1x_ddc_req_priority)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_ctrl.bits.hdcp1x_ddc_req_priority = uhdcp1x_ddc_req_priority;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32, "HDCP1X_CHK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_priorityGet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_ddc_req_priority
//  Input       : unsigned int uhdcp1x_ddc_req_priority: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_priorityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_ctrl.bits.hdcp1x_ddc_req_priority;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enSet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_auto_check_en
//  Input       : unsigned int uhdcp1x_auto_check_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enSet(unsigned int uhdcp1x_auto_check_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_ctrl.bits.hdcp1x_auto_check_en = uhdcp1x_auto_check_en;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_ctrl.u32, "HDCP1X_CHK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enGet
//  Description : Set the value of the member HDCP1X_CHK_CTRL.hdcp1x_auto_check_en
//  Input       : unsigned int uhdcp1x_auto_check_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_CTRL o_hdcp1x_chk_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_CTRL.u32);
    o_hdcp1x_chk_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_ctrl.bits.hdcp1x_auto_check_en;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threSet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_linecnt_thre
//  Input       : unsigned int uhdcp1x_linecnt_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threSet(unsigned int uhdcp1x_linecnt_thre)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_start.bits.hdcp1x_linecnt_thre = uhdcp1x_linecnt_thre;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32, "HDCP1X_CHK_START");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threGet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_linecnt_thre
//  Input       : unsigned int uhdcp1x_linecnt_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_start.bits.hdcp1x_linecnt_thre;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntSet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ddc_req_fcnt
//  Input       : unsigned int uhdcp1x_ddc_req_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntSet(unsigned int uhdcp1x_ddc_req_fcnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_start.bits.hdcp1x_ddc_req_fcnt = uhdcp1x_ddc_req_fcnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32, "HDCP1X_CHK_START");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntGet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ddc_req_fcnt
//  Input       : unsigned int uhdcp1x_ddc_req_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_start.bits.hdcp1x_ddc_req_fcnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntSet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ri_chk2_fcnt
//  Input       : unsigned int uhdcp1x_ri_chk2_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntSet(unsigned int uhdcp1x_ri_chk2_fcnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_start.bits.hdcp1x_ri_chk2_fcnt = uhdcp1x_ri_chk2_fcnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32, "HDCP1X_CHK_START");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntGet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ri_chk2_fcnt
//  Input       : unsigned int uhdcp1x_ri_chk2_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_start.bits.hdcp1x_ri_chk2_fcnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntSet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ri_chk1_fcnt
//  Input       : unsigned int uhdcp1x_ri_chk1_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntSet(unsigned int uhdcp1x_ri_chk1_fcnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_start.bits.hdcp1x_ri_chk1_fcnt = uhdcp1x_ri_chk1_fcnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_start.u32, "HDCP1X_CHK_START");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntGet
//  Description : Set the value of the member HDCP1X_CHK_START.hdcp1x_ri_chk1_fcnt
//  Input       : unsigned int uhdcp1x_ri_chk1_fcnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_START o_hdcp1x_chk_start;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_START.u32);
    o_hdcp1x_chk_start.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_start.bits.hdcp1x_ri_chk1_fcnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntSet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_nodone_err_cnt
//  Input       : unsigned int uhdcp1x_nodone_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntSet(unsigned int uhdcp1x_nodone_err_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_err.bits.hdcp1x_nodone_err_cnt = uhdcp1x_nodone_err_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32, "HDCP1X_CHK_ERR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntGet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_nodone_err_cnt
//  Input       : unsigned int uhdcp1x_nodone_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_err.bits.hdcp1x_nodone_err_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntSet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_notchg_err_cnt
//  Input       : unsigned int uhdcp1x_notchg_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntSet(unsigned int uhdcp1x_notchg_err_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_err.bits.hdcp1x_notchg_err_cnt = uhdcp1x_notchg_err_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32, "HDCP1X_CHK_ERR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntGet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_notchg_err_cnt
//  Input       : unsigned int uhdcp1x_notchg_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_err.bits.hdcp1x_notchg_err_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntSet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_000frm_err_cnt
//  Input       : unsigned int uhdcp1x_000frm_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntSet(unsigned int uhdcp1x_000frm_err_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_err.bits.hdcp1x_000frm_err_cnt = uhdcp1x_000frm_err_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32, "HDCP1X_CHK_ERR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntGet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_000frm_err_cnt
//  Input       : unsigned int uhdcp1x_000frm_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_err.bits.hdcp1x_000frm_err_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntSet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_127frm_err_cnt
//  Input       : unsigned int uhdcp1x_127frm_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntSet(unsigned int uhdcp1x_127frm_err_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_err.bits.hdcp1x_127frm_err_cnt = uhdcp1x_127frm_err_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_err.u32, "HDCP1X_CHK_ERR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntGet
//  Description : Set the value of the member HDCP1X_CHK_ERR.hdcp1x_127frm_err_cnt
//  Input       : unsigned int uhdcp1x_127frm_err_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_ERR o_hdcp1x_chk_err;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_ERR.u32);
    o_hdcp1x_chk_err.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_err.bits.hdcp1x_127frm_err_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateSet
//  Description : Set the value of the member HDCP1X_CHK_STATE.hdcp1x_fsm_state
//  Input       : unsigned int uhdcp1x_fsm_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateSet(unsigned int uhdcp1x_fsm_state)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_STATE o_hdcp1x_chk_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_STATE.u32);
    o_hdcp1x_chk_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_chk_state.bits.hdcp1x_fsm_state = uhdcp1x_fsm_state;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_chk_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_chk_state.u32, "HDCP1X_CHK_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateGet
//  Description : Set the value of the member HDCP1X_CHK_STATE.hdcp1x_fsm_state
//  Input       : unsigned int uhdcp1x_fsm_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_CHK_STATE o_hdcp1x_chk_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_CHK_STATE.u32);
    o_hdcp1x_chk_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_chk_state.bits.hdcp1x_fsm_state;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enSet
//  Description : Set the value of the member HDCP2X_ENC_CTRL.hdcp2x_enc_en
//  Input       : unsigned int uhdcp2x_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enSet(unsigned int uhdcp2x_enc_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_ENC_CTRL o_hdcp2x_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_ENC_CTRL.u32);
    o_hdcp2x_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_enc_ctrl.bits.hdcp2x_enc_en = uhdcp2x_enc_en;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_enc_ctrl.u32, "HDCP2X_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enGet
//  Description : Set the value of the member HDCP2X_ENC_CTRL.hdcp2x_enc_en
//  Input       : unsigned int uhdcp2x_enc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_ENC_CTRL o_hdcp2x_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_ENC_CTRL.u32);
    o_hdcp2x_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_enc_ctrl.bits.hdcp2x_enc_en;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selSet
//  Description : Set the value of the member HDCP2X_DBG_CTRL.hdcp2x_uart_sel
//  Input       : unsigned int uhdcp2x_uart_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selSet(unsigned int uhdcp2x_uart_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_DBG_CTRL o_hdcp2x_dbg_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_DBG_CTRL.u32);
    o_hdcp2x_dbg_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_dbg_ctrl.bits.hdcp2x_uart_sel = uhdcp2x_uart_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_dbg_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_dbg_ctrl.u32, "HDCP2X_DBG_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selGet
//  Description : Set the value of the member HDCP2X_DBG_CTRL.hdcp2x_uart_sel
//  Input       : unsigned int uhdcp2x_uart_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_DBG_CTRL o_hdcp2x_dbg_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_DBG_CTRL.u32);
    o_hdcp2x_dbg_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_dbg_ctrl.bits.hdcp2x_uart_sel;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateSet
//  Description : Set the value of the member HDCP2X_ENC_STATE.hdcp2x_enc_state
//  Input       : unsigned int uhdcp2x_enc_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateSet(unsigned int uhdcp2x_enc_state)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_ENC_STATE o_hdcp2x_enc_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_ENC_STATE.u32);
    o_hdcp2x_enc_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_enc_state.bits.hdcp2x_enc_state = uhdcp2x_enc_state;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_enc_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_enc_state.u32, "HDCP2X_ENC_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateGet
//  Description : Set the value of the member HDCP2X_ENC_STATE.hdcp2x_enc_state
//  Input       : unsigned int uhdcp2x_enc_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_ENC_STATE o_hdcp2x_enc_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP2X_ENC_STATE.u32);
    o_hdcp2x_enc_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_enc_state.bits.hdcp2x_enc_state;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Set
//  Description : Set the value of the member HDCP1X_RPT_V0.hdcp1x_rpt_calc_vh0
//  Input       : unsigned int uhdcp1x_rpt_calc_vh0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Set(unsigned int uhdcp1x_rpt_calc_vh0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V0 o_hdcp1x_rpt_v0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V0.u32);
    o_hdcp1x_rpt_v0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_v0.bits.hdcp1x_rpt_calc_vh0 = uhdcp1x_rpt_calc_vh0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_v0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_v0.u32, "HDCP1X_RPT_V0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Get
//  Description : Set the value of the member HDCP1X_RPT_V0.hdcp1x_rpt_calc_vh0
//  Input       : unsigned int uhdcp1x_rpt_calc_vh0: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V0 o_hdcp1x_rpt_v0;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V0.u32);
    o_hdcp1x_rpt_v0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_v0.bits.hdcp1x_rpt_calc_vh0;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Set
//  Description : Set the value of the member HDCP1X_RPT_V1.hdcp1x_rpt_calc_vh1
//  Input       : unsigned int uhdcp1x_rpt_calc_vh1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Set(unsigned int uhdcp1x_rpt_calc_vh1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V1 o_hdcp1x_rpt_v1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V1.u32);
    o_hdcp1x_rpt_v1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_v1.bits.hdcp1x_rpt_calc_vh1 = uhdcp1x_rpt_calc_vh1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_v1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_v1.u32, "HDCP1X_RPT_V1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Get
//  Description : Set the value of the member HDCP1X_RPT_V1.hdcp1x_rpt_calc_vh1
//  Input       : unsigned int uhdcp1x_rpt_calc_vh1: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V1 o_hdcp1x_rpt_v1;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V1.u32);
    o_hdcp1x_rpt_v1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_v1.bits.hdcp1x_rpt_calc_vh1;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Set
//  Description : Set the value of the member HDCP1X_RPT_V2.hdcp1x_rpt_calc_vh2
//  Input       : unsigned int uhdcp1x_rpt_calc_vh2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Set(unsigned int uhdcp1x_rpt_calc_vh2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V2 o_hdcp1x_rpt_v2;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V2.u32);
    o_hdcp1x_rpt_v2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_v2.bits.hdcp1x_rpt_calc_vh2 = uhdcp1x_rpt_calc_vh2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_v2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_v2.u32, "HDCP1X_RPT_V2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Get
//  Description : Set the value of the member HDCP1X_RPT_V2.hdcp1x_rpt_calc_vh2
//  Input       : unsigned int uhdcp1x_rpt_calc_vh2: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V2 o_hdcp1x_rpt_v2;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V2.u32);
    o_hdcp1x_rpt_v2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_v2.bits.hdcp1x_rpt_calc_vh2;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Set
//  Description : Set the value of the member HDCP1X_RPT_V3.hdcp1x_rpt_calc_vh3
//  Input       : unsigned int uhdcp1x_rpt_calc_vh3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Set(unsigned int uhdcp1x_rpt_calc_vh3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V3 o_hdcp1x_rpt_v3;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V3.u32);
    o_hdcp1x_rpt_v3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_v3.bits.hdcp1x_rpt_calc_vh3 = uhdcp1x_rpt_calc_vh3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_v3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_v3.u32, "HDCP1X_RPT_V3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Get
//  Description : Set the value of the member HDCP1X_RPT_V3.hdcp1x_rpt_calc_vh3
//  Input       : unsigned int uhdcp1x_rpt_calc_vh3: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V3 o_hdcp1x_rpt_v3;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V3.u32);
    o_hdcp1x_rpt_v3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_v3.bits.hdcp1x_rpt_calc_vh3;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Set
//  Description : Set the value of the member HDCP1X_RPT_V4.hdcp1x_rpt_calc_vh4
//  Input       : unsigned int uhdcp1x_rpt_calc_vh4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Set(unsigned int uhdcp1x_rpt_calc_vh4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V4 o_hdcp1x_rpt_v4;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V4.u32);
    o_hdcp1x_rpt_v4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_rpt_v4.bits.hdcp1x_rpt_calc_vh4 = uhdcp1x_rpt_calc_vh4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_rpt_v4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_rpt_v4.u32, "HDCP1X_RPT_V4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Get
//  Description : Set the value of the member HDCP1X_RPT_V4.hdcp1x_rpt_calc_vh4
//  Input       : unsigned int uhdcp1x_rpt_calc_vh4: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_RPT_V4 o_hdcp1x_rpt_v4;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_RPT_V4.u32);
    o_hdcp1x_rpt_v4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_rpt_v4.bits.hdcp1x_rpt_calc_vh4;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkSet
//  Description : Set the value of the member HDCP1X_SHA_CHECK.hdcp1x_sha_check
//  Input       : unsigned int uhdcp1x_sha_check: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkSet(unsigned int uhdcp1x_sha_check)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CHECK o_hdcp1x_sha_check;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CHECK.u32);
    o_hdcp1x_sha_check.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_check.bits.hdcp1x_sha_check = uhdcp1x_sha_check;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_check.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_check.u32, "HDCP1X_SHA_CHECK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkGet
//  Description : Set the value of the member HDCP1X_SHA_CHECK.hdcp1x_sha_check
//  Input       : unsigned int uhdcp1x_sha_check: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_CHECK o_hdcp1x_sha_check;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_CHECK.u32);
    o_hdcp1x_sha_check.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_check.bits.hdcp1x_sha_check;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidSet
//  Description : Set the value of the member HDCP1X_SHA_RESULT.hdcp1x_sha_invalid
//  Input       : unsigned int uhdcp1x_sha_invalid: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidSet(unsigned int uhdcp1x_sha_invalid)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_RESULT o_hdcp1x_sha_result;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_RESULT.u32);
    o_hdcp1x_sha_result.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_result.bits.hdcp1x_sha_invalid = uhdcp1x_sha_invalid;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_result.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_result.u32, "HDCP1X_SHA_RESULT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidGet
//  Description : Set the value of the member HDCP1X_SHA_RESULT.hdcp1x_sha_invalid
//  Input       : unsigned int uhdcp1x_sha_invalid: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_RESULT o_hdcp1x_sha_result;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_RESULT.u32);
    o_hdcp1x_sha_result.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_result.bits.hdcp1x_sha_invalid;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okSet
//  Description : Set the value of the member HDCP1X_SHA_RESULT.hdcp1x_sha_ok
//  Input       : unsigned int uhdcp1x_sha_ok: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okSet(unsigned int uhdcp1x_sha_ok)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_RESULT o_hdcp1x_sha_result;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_RESULT.u32);
    o_hdcp1x_sha_result.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp1x_sha_result.bits.hdcp1x_sha_ok = uhdcp1x_sha_ok;
    HDMITXRegWrite(pu32RegAddr, o_hdcp1x_sha_result.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp1x_sha_result.u32, "HDCP1X_SHA_RESULT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okGet
//  Description : Set the value of the member HDCP1X_SHA_RESULT.hdcp1x_sha_ok
//  Input       : unsigned int uhdcp1x_sha_ok: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP1X_SHA_RESULT o_hdcp1x_sha_result;
    pu32RegAddr = (HI_U32*)&(goptx_hdcp_regAllReg->HDCP1X_SHA_RESULT.u32);
    o_hdcp1x_sha_result.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp1x_sha_result.bits.hdcp1x_sha_ok;
}
