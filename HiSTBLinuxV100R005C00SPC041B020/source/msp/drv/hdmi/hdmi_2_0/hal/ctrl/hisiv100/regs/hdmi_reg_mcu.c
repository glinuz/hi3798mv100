// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  mcu_cpu_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.1 
// History       :  w00271848 2015/12/22 20:18:05 Create file
// ******************************************************************************


#include "hi_type.h"
#include "hdmi_reg_mcu.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif
/* Define the struct pointor of the module mcu_cpu_reg */
volatile S_mcu_cpu_reg_REGS_TYPE *gopmcu_cpu_regAllReg = NULL;

int HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_Init(void)
{
    if (gopmcu_cpu_regAllReg)
    {
       return HI_SUCCESS;
    }

    gopmcu_cpu_regAllReg = (volatile S_mcu_cpu_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_mcu_cpu_reg,sizeof(S_mcu_cpu_reg_REGS_TYPE));

   
    if (!gopmcu_cpu_regAllReg)
    {
        HDMI_ERR("gopmcu_cpu_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_DeInit(void)
{
    if (gopmcu_cpu_regAllReg)
    {
        HDMI_IO_UNMAP(gopmcu_cpu_regAllReg);
        gopmcu_cpu_regAllReg = 0;
    }
    
    return HI_SUCCESS;
}



//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Set
//  Description : Set the value of the member HDCP2X_HW_VER_B0.hdcp2x_hw_ver_b0
//  Input       : unsigned int uhdcp2x_hw_ver_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Set(unsigned int uhdcp2x_hw_ver_b0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B0 o_hdcp2x_hw_ver_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B0.u32);
    o_hdcp2x_hw_ver_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_hw_ver_b0.bits.hdcp2x_hw_ver_b0 = uhdcp2x_hw_ver_b0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Get
//  Description : Set the value of the member HDCP2X_HW_VER_B0.hdcp2x_hw_ver_b0
//  Input       : unsigned int uhdcp2x_hw_ver_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B0 o_hdcp2x_hw_ver_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B0.u32);
    o_hdcp2x_hw_ver_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_hw_ver_b0.bits.hdcp2x_hw_ver_b0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Set
//  Description : Set the value of the member HDCP2X_HW_VER_B1.hdcp2x_hw_ver_b1
//  Input       : unsigned int uhdcp2x_hw_ver_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Set(unsigned int uhdcp2x_hw_ver_b1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B1 o_hdcp2x_hw_ver_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B1.u32);
    o_hdcp2x_hw_ver_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_hw_ver_b1.bits.hdcp2x_hw_ver_b1 = uhdcp2x_hw_ver_b1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b1.u32, "HDCP2X_HW_VER_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Get
//  Description : Set the value of the member HDCP2X_HW_VER_B1.hdcp2x_hw_ver_b1
//  Input       : unsigned int uhdcp2x_hw_ver_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B1 o_hdcp2x_hw_ver_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B1.u32);
    o_hdcp2x_hw_ver_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_hw_ver_b1.bits.hdcp2x_hw_ver_b1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Set
//  Description : Set the value of the member HDCP2X_HW_VER_B2.hdcp2x_hw_ver_b2
//  Input       : unsigned int uhdcp2x_hw_ver_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Set(unsigned int uhdcp2x_hw_ver_b2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B2 o_hdcp2x_hw_ver_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B2.u32);
    o_hdcp2x_hw_ver_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_hw_ver_b2.bits.hdcp2x_hw_ver_b2 = uhdcp2x_hw_ver_b2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b2.u32, "HDCP2X_HW_VER_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Get
//  Description : Set the value of the member HDCP2X_HW_VER_B2.hdcp2x_hw_ver_b2
//  Input       : unsigned int uhdcp2x_hw_ver_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B2 o_hdcp2x_hw_ver_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B2.u32);
    o_hdcp2x_hw_ver_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_hw_ver_b2.bits.hdcp2x_hw_ver_b2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Set
//  Description : Set the value of the member HDCP2X_HW_VER_B3.hdcp2x_hw_ver_b3
//  Input       : unsigned int uhdcp2x_hw_ver_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Set(unsigned int uhdcp2x_hw_ver_b3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B3 o_hdcp2x_hw_ver_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B3.u32);
    o_hdcp2x_hw_ver_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_hw_ver_b3.bits.hdcp2x_hw_ver_b3 = uhdcp2x_hw_ver_b3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_hw_ver_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Get
//  Description : Set the value of the member HDCP2X_HW_VER_B3.hdcp2x_hw_ver_b3
//  Input       : unsigned int uhdcp2x_hw_ver_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_HW_VER_B3 o_hdcp2x_hw_ver_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_HW_VER_B3.u32);
    o_hdcp2x_hw_ver_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_hw_ver_b3.bits.hdcp2x_hw_ver_b3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Set
//  Description : Set the value of the member HDCP2X_SW_VER_B0.hdcp2x_sw_ver_b0
//  Input       : unsigned int uhdcp2x_sw_ver_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Set(unsigned int uhdcp2x_sw_ver_b0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B0 o_hdcp2x_sw_ver_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B0.u32);
    o_hdcp2x_sw_ver_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sw_ver_b0.bits.hdcp2x_sw_ver_b0 = uhdcp2x_sw_ver_b0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b0.u32, "HDCP2X_SW_VER_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Get
//  Description : Set the value of the member HDCP2X_SW_VER_B0.hdcp2x_sw_ver_b0
//  Input       : unsigned int uhdcp2x_sw_ver_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B0 o_hdcp2x_sw_ver_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B0.u32);
    o_hdcp2x_sw_ver_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sw_ver_b0.bits.hdcp2x_sw_ver_b0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Set
//  Description : Set the value of the member HDCP2X_SW_VER_B1.hdcp2x_sw_ver_b1
//  Input       : unsigned int uhdcp2x_sw_ver_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Set(unsigned int uhdcp2x_sw_ver_b1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B1 o_hdcp2x_sw_ver_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B1.u32);
    o_hdcp2x_sw_ver_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sw_ver_b1.bits.hdcp2x_sw_ver_b1 = uhdcp2x_sw_ver_b1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b1.u32, "HDCP2X_SW_VER_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Get
//  Description : Set the value of the member HDCP2X_SW_VER_B1.hdcp2x_sw_ver_b1
//  Input       : unsigned int uhdcp2x_sw_ver_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B1 o_hdcp2x_sw_ver_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B1.u32);
    o_hdcp2x_sw_ver_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sw_ver_b1.bits.hdcp2x_sw_ver_b1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Set
//  Description : Set the value of the member HDCP2X_SW_VER_B2.hdcp2x_sw_ver_b2
//  Input       : unsigned int uhdcp2x_sw_ver_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Set(unsigned int uhdcp2x_sw_ver_b2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B2 o_hdcp2x_sw_ver_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B2.u32);
    o_hdcp2x_sw_ver_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sw_ver_b2.bits.hdcp2x_sw_ver_b2 = uhdcp2x_sw_ver_b2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b2.u32, "HDCP2X_SW_VER_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Get
//  Description : Set the value of the member HDCP2X_SW_VER_B2.hdcp2x_sw_ver_b2
//  Input       : unsigned int uhdcp2x_sw_ver_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B2 o_hdcp2x_sw_ver_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B2.u32);
    o_hdcp2x_sw_ver_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sw_ver_b2.bits.hdcp2x_sw_ver_b2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Set
//  Description : Set the value of the member HDCP2X_SW_VER_B3.hdcp2x_sw_ver_b3
//  Input       : unsigned int uhdcp2x_sw_ver_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Set(unsigned int uhdcp2x_sw_ver_b3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B3 o_hdcp2x_sw_ver_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B3.u32);
    o_hdcp2x_sw_ver_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sw_ver_b3.bits.hdcp2x_sw_ver_b3 = uhdcp2x_sw_ver_b3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sw_ver_b3.u32, "HDCP2X_SW_VER_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Get
//  Description : Set the value of the member HDCP2X_SW_VER_B3.hdcp2x_sw_ver_b3
//  Input       : unsigned int uhdcp2x_sw_ver_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SW_VER_B3 o_hdcp2x_sw_ver_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SW_VER_B3.u32);
    o_hdcp2x_sw_ver_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sw_ver_b3.bits.hdcp2x_sw_ver_b3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopSet
//  Description : Set the value of the member HDCP2X_AUTH_CTRL.mcu_auth_stop
//  Input       : unsigned int umcu_auth_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopSet(unsigned int umcu_auth_stop)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_CTRL o_hdcp2x_auth_ctrl;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_CTRL.u32);
    o_hdcp2x_auth_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_ctrl.bits.mcu_auth_stop = umcu_auth_stop;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_ctrl.u32, "HDCP2X_AUTH_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopGet
//  Description : Set the value of the member HDCP2X_AUTH_CTRL.mcu_auth_stop
//  Input       : unsigned int umcu_auth_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_CTRL o_hdcp2x_auth_ctrl;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_CTRL.u32);
    o_hdcp2x_auth_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_ctrl.bits.mcu_auth_stop;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startSet
//  Description : Set the value of the member HDCP2X_AUTH_CTRL.mcu_auth_start
//  Input       : unsigned int umcu_auth_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startSet(unsigned int umcu_auth_start)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_CTRL o_hdcp2x_auth_ctrl;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_CTRL.u32);
    o_hdcp2x_auth_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_ctrl.bits.mcu_auth_start = umcu_auth_start;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_ctrl.u32, "HDCP2X_AUTH_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startGet
//  Description : Set the value of the member HDCP2X_AUTH_CTRL.mcu_auth_start
//  Input       : unsigned int umcu_auth_start: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_CTRL o_hdcp2x_auth_ctrl;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_CTRL.u32);
    o_hdcp2x_auth_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_ctrl.bits.mcu_auth_start;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr7
//  Input       : unsigned int uhdcp2x_sec_intr7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Set(unsigned int uhdcp2x_sec_intr7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr7 = uhdcp2x_sec_intr7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr7
//  Input       : unsigned int uhdcp2x_sec_intr7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr6
//  Input       : unsigned int uhdcp2x_sec_intr6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Set(unsigned int uhdcp2x_sec_intr6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr6 = uhdcp2x_sec_intr6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr6
//  Input       : unsigned int uhdcp2x_sec_intr6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr5
//  Input       : unsigned int uhdcp2x_sec_intr5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Set(unsigned int uhdcp2x_sec_intr5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr5 = uhdcp2x_sec_intr5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr5
//  Input       : unsigned int uhdcp2x_sec_intr5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr4
//  Input       : unsigned int uhdcp2x_sec_intr4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Set(unsigned int uhdcp2x_sec_intr4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr4 = uhdcp2x_sec_intr4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr4
//  Input       : unsigned int uhdcp2x_sec_intr4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr3
//  Input       : unsigned int uhdcp2x_sec_intr3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Set(unsigned int uhdcp2x_sec_intr3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr3 = uhdcp2x_sec_intr3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr3
//  Input       : unsigned int uhdcp2x_sec_intr3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr2
//  Input       : unsigned int uhdcp2x_sec_intr2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Set(unsigned int uhdcp2x_sec_intr2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr2 = uhdcp2x_sec_intr2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr2
//  Input       : unsigned int uhdcp2x_sec_intr2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr1
//  Input       : unsigned int uhdcp2x_sec_intr1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Set(unsigned int uhdcp2x_sec_intr1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr1 = uhdcp2x_sec_intr1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr1
//  Input       : unsigned int uhdcp2x_sec_intr1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr0
//  Input       : unsigned int uhdcp2x_sec_intr0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Set(unsigned int uhdcp2x_sec_intr0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr0 = uhdcp2x_sec_intr0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b0.u32, "HDCP2X_SEC_INTR_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B0.hdcp2x_sec_intr0
//  Input       : unsigned int uhdcp2x_sec_intr0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B0 o_hdcp2x_sec_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B0.u32);
    o_hdcp2x_sec_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b0.bits.hdcp2x_sec_intr0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr15
//  Input       : unsigned int uhdcp2x_sec_intr15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Set(unsigned int uhdcp2x_sec_intr15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr15 = uhdcp2x_sec_intr15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr15
//  Input       : unsigned int uhdcp2x_sec_intr15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr14
//  Input       : unsigned int uhdcp2x_sec_intr14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Set(unsigned int uhdcp2x_sec_intr14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr14 = uhdcp2x_sec_intr14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr14
//  Input       : unsigned int uhdcp2x_sec_intr14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr13
//  Input       : unsigned int uhdcp2x_sec_intr13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Set(unsigned int uhdcp2x_sec_intr13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr13 = uhdcp2x_sec_intr13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr13
//  Input       : unsigned int uhdcp2x_sec_intr13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr12
//  Input       : unsigned int uhdcp2x_sec_intr12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Set(unsigned int uhdcp2x_sec_intr12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr12 = uhdcp2x_sec_intr12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr12
//  Input       : unsigned int uhdcp2x_sec_intr12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr11
//  Input       : unsigned int uhdcp2x_sec_intr11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Set(unsigned int uhdcp2x_sec_intr11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr11 = uhdcp2x_sec_intr11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr11
//  Input       : unsigned int uhdcp2x_sec_intr11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr10
//  Input       : unsigned int uhdcp2x_sec_intr10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Set(unsigned int uhdcp2x_sec_intr10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr10 = uhdcp2x_sec_intr10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr10
//  Input       : unsigned int uhdcp2x_sec_intr10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr9
//  Input       : unsigned int uhdcp2x_sec_intr9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Set(unsigned int uhdcp2x_sec_intr9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr9 = uhdcp2x_sec_intr9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr9
//  Input       : unsigned int uhdcp2x_sec_intr9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Set
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr8
//  Input       : unsigned int uhdcp2x_sec_intr8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Set(unsigned int uhdcp2x_sec_intr8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr8 = uhdcp2x_sec_intr8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_intr_b1.u32, "HDCP2X_SEC_INTR_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Get
//  Description : Set the value of the member HDCP2X_SEC_INTR_B1.hdcp2x_sec_intr8
//  Input       : unsigned int uhdcp2x_sec_intr8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_INTR_B1 o_hdcp2x_sec_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_INTR_B1.u32);
    o_hdcp2x_sec_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_intr_b1.bits.hdcp2x_sec_intr8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask7
//  Input       : unsigned int uhdcp2x_sec_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Set(unsigned int uhdcp2x_sec_mask7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask7 = uhdcp2x_sec_mask7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask7
//  Input       : unsigned int uhdcp2x_sec_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask6
//  Input       : unsigned int uhdcp2x_sec_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Set(unsigned int uhdcp2x_sec_mask6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask6 = uhdcp2x_sec_mask6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask6
//  Input       : unsigned int uhdcp2x_sec_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask5
//  Input       : unsigned int uhdcp2x_sec_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Set(unsigned int uhdcp2x_sec_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask5 = uhdcp2x_sec_mask5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask5
//  Input       : unsigned int uhdcp2x_sec_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask4
//  Input       : unsigned int uhdcp2x_sec_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Set(unsigned int uhdcp2x_sec_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask4 = uhdcp2x_sec_mask4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask4
//  Input       : unsigned int uhdcp2x_sec_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask3
//  Input       : unsigned int uhdcp2x_sec_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Set(unsigned int uhdcp2x_sec_mask3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask3 = uhdcp2x_sec_mask3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask3
//  Input       : unsigned int uhdcp2x_sec_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask2
//  Input       : unsigned int uhdcp2x_sec_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Set(unsigned int uhdcp2x_sec_mask2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask2 = uhdcp2x_sec_mask2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask2
//  Input       : unsigned int uhdcp2x_sec_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask1
//  Input       : unsigned int uhdcp2x_sec_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Set(unsigned int uhdcp2x_sec_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask1 = uhdcp2x_sec_mask1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask1
//  Input       : unsigned int uhdcp2x_sec_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask0
//  Input       : unsigned int uhdcp2x_sec_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Set(unsigned int uhdcp2x_sec_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask0 = uhdcp2x_sec_mask0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b0.u32, "HDCP2X_SEC_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B0.hdcp2x_sec_mask0
//  Input       : unsigned int uhdcp2x_sec_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B0 o_hdcp2x_sec_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B0.u32);
    o_hdcp2x_sec_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b0.bits.hdcp2x_sec_mask0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask15
//  Input       : unsigned int uhdcp2x_sec_mask15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Set(unsigned int uhdcp2x_sec_mask15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask15 = uhdcp2x_sec_mask15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask15
//  Input       : unsigned int uhdcp2x_sec_mask15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask14
//  Input       : unsigned int uhdcp2x_sec_mask14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Set(unsigned int uhdcp2x_sec_mask14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask14 = uhdcp2x_sec_mask14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask14
//  Input       : unsigned int uhdcp2x_sec_mask14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask13
//  Input       : unsigned int uhdcp2x_sec_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Set(unsigned int uhdcp2x_sec_mask13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask13 = uhdcp2x_sec_mask13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask13
//  Input       : unsigned int uhdcp2x_sec_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask12
//  Input       : unsigned int uhdcp2x_sec_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Set(unsigned int uhdcp2x_sec_mask12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask12 = uhdcp2x_sec_mask12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask12
//  Input       : unsigned int uhdcp2x_sec_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask11
//  Input       : unsigned int uhdcp2x_sec_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Set(unsigned int uhdcp2x_sec_mask11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask11 = uhdcp2x_sec_mask11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask11
//  Input       : unsigned int uhdcp2x_sec_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask10
//  Input       : unsigned int uhdcp2x_sec_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Set(unsigned int uhdcp2x_sec_mask10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask10 = uhdcp2x_sec_mask10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask10
//  Input       : unsigned int uhdcp2x_sec_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask9
//  Input       : unsigned int uhdcp2x_sec_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Set(unsigned int uhdcp2x_sec_mask9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask9 = uhdcp2x_sec_mask9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask9
//  Input       : unsigned int uhdcp2x_sec_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Set
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask8
//  Input       : unsigned int uhdcp2x_sec_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Set(unsigned int uhdcp2x_sec_mask8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask8 = uhdcp2x_sec_mask8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_mask_b1.u32, "HDCP2X_SEC_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Get
//  Description : Set the value of the member HDCP2X_SEC_MASK_B1.hdcp2x_sec_mask8
//  Input       : unsigned int uhdcp2x_sec_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_MASK_B1 o_hdcp2x_sec_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_MASK_B1.u32);
    o_hdcp2x_sec_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_mask_b1.bits.hdcp2x_sec_mask8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr7
//  Input       : unsigned int uhdcp2x_mcu_intr7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Set(unsigned int uhdcp2x_mcu_intr7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr7 = uhdcp2x_mcu_intr7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr7
//  Input       : unsigned int uhdcp2x_mcu_intr7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr6
//  Input       : unsigned int uhdcp2x_mcu_intr6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Set(unsigned int uhdcp2x_mcu_intr6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr6 = uhdcp2x_mcu_intr6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr6
//  Input       : unsigned int uhdcp2x_mcu_intr6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr5
//  Input       : unsigned int uhdcp2x_mcu_intr5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Set(unsigned int uhdcp2x_mcu_intr5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr5 = uhdcp2x_mcu_intr5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr5
//  Input       : unsigned int uhdcp2x_mcu_intr5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr4
//  Input       : unsigned int uhdcp2x_mcu_intr4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Set(unsigned int uhdcp2x_mcu_intr4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr4 = uhdcp2x_mcu_intr4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr4
//  Input       : unsigned int uhdcp2x_mcu_intr4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr3
//  Input       : unsigned int uhdcp2x_mcu_intr3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Set(unsigned int uhdcp2x_mcu_intr3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr3 = uhdcp2x_mcu_intr3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr3
//  Input       : unsigned int uhdcp2x_mcu_intr3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr2
//  Input       : unsigned int uhdcp2x_mcu_intr2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Set(unsigned int uhdcp2x_mcu_intr2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr2 = uhdcp2x_mcu_intr2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr2
//  Input       : unsigned int uhdcp2x_mcu_intr2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr1
//  Input       : unsigned int uhdcp2x_mcu_intr1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Set(unsigned int uhdcp2x_mcu_intr1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr1 = uhdcp2x_mcu_intr1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr1
//  Input       : unsigned int uhdcp2x_mcu_intr1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr0
//  Input       : unsigned int uhdcp2x_mcu_intr0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Set(unsigned int uhdcp2x_mcu_intr0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = 0;
    o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr0 = uhdcp2x_mcu_intr0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b0.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B0.hdcp2x_mcu_intr0
//  Input       : unsigned int uhdcp2x_mcu_intr0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B0 o_hdcp2x_mcu_intr_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B0.u32);
    o_hdcp2x_mcu_intr_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b0.bits.hdcp2x_mcu_intr0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr15
//  Input       : unsigned int uhdcp2x_mcu_intr15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Set(unsigned int uhdcp2x_mcu_intr15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr15 = uhdcp2x_mcu_intr15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr15
//  Input       : unsigned int uhdcp2x_mcu_intr15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr14
//  Input       : unsigned int uhdcp2x_mcu_intr14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Set(unsigned int uhdcp2x_mcu_intr14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr14 = uhdcp2x_mcu_intr14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr14
//  Input       : unsigned int uhdcp2x_mcu_intr14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr13
//  Input       : unsigned int uhdcp2x_mcu_intr13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Set(unsigned int uhdcp2x_mcu_intr13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr13 = uhdcp2x_mcu_intr13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr13
//  Input       : unsigned int uhdcp2x_mcu_intr13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr12
//  Input       : unsigned int uhdcp2x_mcu_intr12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Set(unsigned int uhdcp2x_mcu_intr12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr12 = uhdcp2x_mcu_intr12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr12
//  Input       : unsigned int uhdcp2x_mcu_intr12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr11
//  Input       : unsigned int uhdcp2x_mcu_intr11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Set(unsigned int uhdcp2x_mcu_intr11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr11 = uhdcp2x_mcu_intr11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr11
//  Input       : unsigned int uhdcp2x_mcu_intr11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr10
//  Input       : unsigned int uhdcp2x_mcu_intr10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Set(unsigned int uhdcp2x_mcu_intr10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr10 = uhdcp2x_mcu_intr10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr10
//  Input       : unsigned int uhdcp2x_mcu_intr10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr9
//  Input       : unsigned int uhdcp2x_mcu_intr9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Set(unsigned int uhdcp2x_mcu_intr9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr9 = uhdcp2x_mcu_intr9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr9
//  Input       : unsigned int uhdcp2x_mcu_intr9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr8
//  Input       : unsigned int uhdcp2x_mcu_intr8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Set(unsigned int uhdcp2x_mcu_intr8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = 0;
    o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr8 = uhdcp2x_mcu_intr8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b1.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B1.hdcp2x_mcu_intr8
//  Input       : unsigned int uhdcp2x_mcu_intr8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B1 o_hdcp2x_mcu_intr_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B1.u32);
    o_hdcp2x_mcu_intr_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b1.bits.hdcp2x_mcu_intr8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr23
//  Input       : unsigned int uhdcp2x_mcu_intr23: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Set(unsigned int uhdcp2x_mcu_intr23)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr23 = uhdcp2x_mcu_intr23;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr23
//  Input       : unsigned int uhdcp2x_mcu_intr23: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr23;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr22
//  Input       : unsigned int uhdcp2x_mcu_intr22: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Set(unsigned int uhdcp2x_mcu_intr22)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr22 = uhdcp2x_mcu_intr22;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr22
//  Input       : unsigned int uhdcp2x_mcu_intr22: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr22;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr21
//  Input       : unsigned int uhdcp2x_mcu_intr21: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Set(unsigned int uhdcp2x_mcu_intr21)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr21 = uhdcp2x_mcu_intr21;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr21
//  Input       : unsigned int uhdcp2x_mcu_intr21: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr21;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr20
//  Input       : unsigned int uhdcp2x_mcu_intr20: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Set(unsigned int uhdcp2x_mcu_intr20)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr20 = uhdcp2x_mcu_intr20;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr20
//  Input       : unsigned int uhdcp2x_mcu_intr20: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr20;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr19
//  Input       : unsigned int uhdcp2x_mcu_intr19: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Set(unsigned int uhdcp2x_mcu_intr19)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr19 = uhdcp2x_mcu_intr19;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr19
//  Input       : unsigned int uhdcp2x_mcu_intr19: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr19;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr18
//  Input       : unsigned int uhdcp2x_mcu_intr18: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Set(unsigned int uhdcp2x_mcu_intr18)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr18 = uhdcp2x_mcu_intr18;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr18
//  Input       : unsigned int uhdcp2x_mcu_intr18: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr18;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr17
//  Input       : unsigned int uhdcp2x_mcu_intr17: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Set(unsigned int uhdcp2x_mcu_intr17)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr17 = uhdcp2x_mcu_intr17;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr17
//  Input       : unsigned int uhdcp2x_mcu_intr17: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr17;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr16
//  Input       : unsigned int uhdcp2x_mcu_intr16: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Set(unsigned int uhdcp2x_mcu_intr16)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = 0;
    o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr16 = uhdcp2x_mcu_intr16;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b2.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B2.hdcp2x_mcu_intr16
//  Input       : unsigned int uhdcp2x_mcu_intr16: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B2 o_hdcp2x_mcu_intr_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B2.u32);
    o_hdcp2x_mcu_intr_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b2.bits.hdcp2x_mcu_intr16;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr31
//  Input       : unsigned int uhdcp2x_mcu_intr31: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Set(unsigned int uhdcp2x_mcu_intr31)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr31 = uhdcp2x_mcu_intr31;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr31
//  Input       : unsigned int uhdcp2x_mcu_intr31: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr31;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr30
//  Input       : unsigned int uhdcp2x_mcu_intr30: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Set(unsigned int uhdcp2x_mcu_intr30)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr30 = uhdcp2x_mcu_intr30;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr30
//  Input       : unsigned int uhdcp2x_mcu_intr30: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr30;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr29
//  Input       : unsigned int uhdcp2x_mcu_intr29: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Set(unsigned int uhdcp2x_mcu_intr29)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr29 = uhdcp2x_mcu_intr29;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr29
//  Input       : unsigned int uhdcp2x_mcu_intr29: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr29;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr28
//  Input       : unsigned int uhdcp2x_mcu_intr28: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Set(unsigned int uhdcp2x_mcu_intr28)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr28 = uhdcp2x_mcu_intr28;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr28
//  Input       : unsigned int uhdcp2x_mcu_intr28: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr28;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr27
//  Input       : unsigned int uhdcp2x_mcu_intr27: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Set(unsigned int uhdcp2x_mcu_intr27)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr27 = uhdcp2x_mcu_intr27;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr27
//  Input       : unsigned int uhdcp2x_mcu_intr27: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr27;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr26
//  Input       : unsigned int uhdcp2x_mcu_intr26: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Set(unsigned int uhdcp2x_mcu_intr26)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr26 = uhdcp2x_mcu_intr26;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr26
//  Input       : unsigned int uhdcp2x_mcu_intr26: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr26;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr25
//  Input       : unsigned int uhdcp2x_mcu_intr25: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Set(unsigned int uhdcp2x_mcu_intr25)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr25 = uhdcp2x_mcu_intr25;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr25
//  Input       : unsigned int uhdcp2x_mcu_intr25: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr25;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Set
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr24
//  Input       : unsigned int uhdcp2x_mcu_intr24: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Set(unsigned int uhdcp2x_mcu_intr24)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = 0;
    o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr24 = uhdcp2x_mcu_intr24;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_intr_b3.u32);

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Get
//  Description : Set the value of the member HDCP2X_MCU_INTR_B3.hdcp2x_mcu_intr24
//  Input       : unsigned int uhdcp2x_mcu_intr24: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_INTR_B3 o_hdcp2x_mcu_intr_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_INTR_B3.u32);
    o_hdcp2x_mcu_intr_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_intr_b3.bits.hdcp2x_mcu_intr24;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask7
//  Input       : unsigned int uhdcp2x_mcu_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Set(unsigned int uhdcp2x_mcu_mask7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask7 = uhdcp2x_mcu_mask7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask7
//  Input       : unsigned int uhdcp2x_mcu_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask6
//  Input       : unsigned int uhdcp2x_mcu_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Set(unsigned int uhdcp2x_mcu_mask6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask6 = uhdcp2x_mcu_mask6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask6
//  Input       : unsigned int uhdcp2x_mcu_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask5
//  Input       : unsigned int uhdcp2x_mcu_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Set(unsigned int uhdcp2x_mcu_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask5 = uhdcp2x_mcu_mask5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask5
//  Input       : unsigned int uhdcp2x_mcu_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask4
//  Input       : unsigned int uhdcp2x_mcu_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Set(unsigned int uhdcp2x_mcu_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask4 = uhdcp2x_mcu_mask4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask4
//  Input       : unsigned int uhdcp2x_mcu_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask3
//  Input       : unsigned int uhdcp2x_mcu_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Set(unsigned int uhdcp2x_mcu_mask3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask3 = uhdcp2x_mcu_mask3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask3
//  Input       : unsigned int uhdcp2x_mcu_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask2
//  Input       : unsigned int uhdcp2x_mcu_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Set(unsigned int uhdcp2x_mcu_mask2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask2 = uhdcp2x_mcu_mask2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask2
//  Input       : unsigned int uhdcp2x_mcu_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask1
//  Input       : unsigned int uhdcp2x_mcu_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Set(unsigned int uhdcp2x_mcu_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask1 = uhdcp2x_mcu_mask1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask1
//  Input       : unsigned int uhdcp2x_mcu_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask0
//  Input       : unsigned int uhdcp2x_mcu_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Set(unsigned int uhdcp2x_mcu_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask0 = uhdcp2x_mcu_mask0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b0.u32, "HDCP2X_MCU_MASK_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B0.hdcp2x_mcu_mask0
//  Input       : unsigned int uhdcp2x_mcu_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B0 o_hdcp2x_mcu_mask_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B0.u32);
    o_hdcp2x_mcu_mask_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b0.bits.hdcp2x_mcu_mask0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask15
//  Input       : unsigned int uhdcp2x_mcu_mask15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Set(unsigned int uhdcp2x_mcu_mask15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask15 = uhdcp2x_mcu_mask15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask15
//  Input       : unsigned int uhdcp2x_mcu_mask15: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask14
//  Input       : unsigned int uhdcp2x_mcu_mask14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Set(unsigned int uhdcp2x_mcu_mask14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask14 = uhdcp2x_mcu_mask14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask14
//  Input       : unsigned int uhdcp2x_mcu_mask14: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask13
//  Input       : unsigned int uhdcp2x_mcu_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Set(unsigned int uhdcp2x_mcu_mask13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask13 = uhdcp2x_mcu_mask13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask13
//  Input       : unsigned int uhdcp2x_mcu_mask13: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask12
//  Input       : unsigned int uhdcp2x_mcu_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Set(unsigned int uhdcp2x_mcu_mask12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask12 = uhdcp2x_mcu_mask12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask12
//  Input       : unsigned int uhdcp2x_mcu_mask12: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask11
//  Input       : unsigned int uhdcp2x_mcu_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Set(unsigned int uhdcp2x_mcu_mask11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask11 = uhdcp2x_mcu_mask11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask11
//  Input       : unsigned int uhdcp2x_mcu_mask11: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask10
//  Input       : unsigned int uhdcp2x_mcu_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Set(unsigned int uhdcp2x_mcu_mask10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask10 = uhdcp2x_mcu_mask10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask10
//  Input       : unsigned int uhdcp2x_mcu_mask10: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask9
//  Input       : unsigned int uhdcp2x_mcu_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Set(unsigned int uhdcp2x_mcu_mask9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask9 = uhdcp2x_mcu_mask9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask9
//  Input       : unsigned int uhdcp2x_mcu_mask9: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask8
//  Input       : unsigned int uhdcp2x_mcu_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Set(unsigned int uhdcp2x_mcu_mask8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask8 = uhdcp2x_mcu_mask8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b1.u32, "HDCP2X_MCU_MASK_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B1.hdcp2x_mcu_mask8
//  Input       : unsigned int uhdcp2x_mcu_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B1 o_hdcp2x_mcu_mask_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B1.u32);
    o_hdcp2x_mcu_mask_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b1.bits.hdcp2x_mcu_mask8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask23
//  Input       : unsigned int uhdcp2x_mcu_mask23: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Set(unsigned int uhdcp2x_mcu_mask23)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask23 = uhdcp2x_mcu_mask23;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask23
//  Input       : unsigned int uhdcp2x_mcu_mask23: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask23;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask22
//  Input       : unsigned int uhdcp2x_mcu_mask22: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Set(unsigned int uhdcp2x_mcu_mask22)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask22 = uhdcp2x_mcu_mask22;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask22
//  Input       : unsigned int uhdcp2x_mcu_mask22: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask22;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask21
//  Input       : unsigned int uhdcp2x_mcu_mask21: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Set(unsigned int uhdcp2x_mcu_mask21)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask21 = uhdcp2x_mcu_mask21;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask21
//  Input       : unsigned int uhdcp2x_mcu_mask21: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask21;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask20
//  Input       : unsigned int uhdcp2x_mcu_mask20: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Set(unsigned int uhdcp2x_mcu_mask20)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask20 = uhdcp2x_mcu_mask20;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask20
//  Input       : unsigned int uhdcp2x_mcu_mask20: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask20;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask19
//  Input       : unsigned int uhdcp2x_mcu_mask19: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Set(unsigned int uhdcp2x_mcu_mask19)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask19 = uhdcp2x_mcu_mask19;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask19
//  Input       : unsigned int uhdcp2x_mcu_mask19: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask19;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask18
//  Input       : unsigned int uhdcp2x_mcu_mask18: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Set(unsigned int uhdcp2x_mcu_mask18)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask18 = uhdcp2x_mcu_mask18;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask18
//  Input       : unsigned int uhdcp2x_mcu_mask18: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask18;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask17
//  Input       : unsigned int uhdcp2x_mcu_mask17: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Set(unsigned int uhdcp2x_mcu_mask17)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask17 = uhdcp2x_mcu_mask17;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask17
//  Input       : unsigned int uhdcp2x_mcu_mask17: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask17;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask16
//  Input       : unsigned int uhdcp2x_mcu_mask16: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Set(unsigned int uhdcp2x_mcu_mask16)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask16 = uhdcp2x_mcu_mask16;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b2.u32, "HDCP2X_MCU_MASK_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B2.hdcp2x_mcu_mask16
//  Input       : unsigned int uhdcp2x_mcu_mask16: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B2 o_hdcp2x_mcu_mask_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B2.u32);
    o_hdcp2x_mcu_mask_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b2.bits.hdcp2x_mcu_mask16;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask31
//  Input       : unsigned int uhdcp2x_mcu_mask31: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Set(unsigned int uhdcp2x_mcu_mask31)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask31 = uhdcp2x_mcu_mask31;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask31
//  Input       : unsigned int uhdcp2x_mcu_mask31: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask31;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask30
//  Input       : unsigned int uhdcp2x_mcu_mask30: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Set(unsigned int uhdcp2x_mcu_mask30)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask30 = uhdcp2x_mcu_mask30;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask30
//  Input       : unsigned int uhdcp2x_mcu_mask30: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask30;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask29
//  Input       : unsigned int uhdcp2x_mcu_mask29: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Set(unsigned int uhdcp2x_mcu_mask29)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask29 = uhdcp2x_mcu_mask29;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask29
//  Input       : unsigned int uhdcp2x_mcu_mask29: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask29;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask28
//  Input       : unsigned int uhdcp2x_mcu_mask28: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Set(unsigned int uhdcp2x_mcu_mask28)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask28 = uhdcp2x_mcu_mask28;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask28
//  Input       : unsigned int uhdcp2x_mcu_mask28: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask28;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask27
//  Input       : unsigned int uhdcp2x_mcu_mask27: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Set(unsigned int uhdcp2x_mcu_mask27)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask27 = uhdcp2x_mcu_mask27;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask27
//  Input       : unsigned int uhdcp2x_mcu_mask27: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask27;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask26
//  Input       : unsigned int uhdcp2x_mcu_mask26: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Set(unsigned int uhdcp2x_mcu_mask26)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask26 = uhdcp2x_mcu_mask26;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask26
//  Input       : unsigned int uhdcp2x_mcu_mask26: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask26;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask25
//  Input       : unsigned int uhdcp2x_mcu_mask25: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Set(unsigned int uhdcp2x_mcu_mask25)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask25 = uhdcp2x_mcu_mask25;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask25
//  Input       : unsigned int uhdcp2x_mcu_mask25: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask25;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Set
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask24
//  Input       : unsigned int uhdcp2x_mcu_mask24: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Set(unsigned int uhdcp2x_mcu_mask24)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask24 = uhdcp2x_mcu_mask24;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_mask_b3.u32, "HDCP2X_MCU_MASK_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Get
//  Description : Set the value of the member HDCP2X_MCU_MASK_B3.hdcp2x_mcu_mask24
//  Input       : unsigned int uhdcp2x_mcu_mask24: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_MASK_B3 o_hdcp2x_mcu_mask_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_MASK_B3.u32);
    o_hdcp2x_mcu_mask_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_mask_b3.bits.hdcp2x_mcu_mask24;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Set
//  Description : Set the value of the member HDCP2X_MCU_STATE_B0.hdcp2x_mcu_state_b0
//  Input       : unsigned int uhdcp2x_mcu_state_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Set(unsigned int uhdcp2x_mcu_state_b0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B0 o_hdcp2x_mcu_state_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B0.u32);
    o_hdcp2x_mcu_state_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_state_b0.bits.hdcp2x_mcu_state_b0 = uhdcp2x_mcu_state_b0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b0.u32, "HDCP2X_MCU_STATE_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Get
//  Description : Set the value of the member HDCP2X_MCU_STATE_B0.hdcp2x_mcu_state_b0
//  Input       : unsigned int uhdcp2x_mcu_state_b0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B0 o_hdcp2x_mcu_state_b0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B0.u32);
    o_hdcp2x_mcu_state_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_state_b0.bits.hdcp2x_mcu_state_b0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Set
//  Description : Set the value of the member HDCP2X_MCU_STATE_B1.hdcp2x_mcu_state_b1
//  Input       : unsigned int uhdcp2x_mcu_state_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Set(unsigned int uhdcp2x_mcu_state_b1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B1 o_hdcp2x_mcu_state_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B1.u32);
    o_hdcp2x_mcu_state_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_state_b1.bits.hdcp2x_mcu_state_b1 = uhdcp2x_mcu_state_b1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b1.u32, "HDCP2X_MCU_STATE_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Get
//  Description : Set the value of the member HDCP2X_MCU_STATE_B1.hdcp2x_mcu_state_b1
//  Input       : unsigned int uhdcp2x_mcu_state_b1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B1 o_hdcp2x_mcu_state_b1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B1.u32);
    o_hdcp2x_mcu_state_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_state_b1.bits.hdcp2x_mcu_state_b1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Set
//  Description : Set the value of the member HDCP2X_MCU_STATE_B2.hdcp2x_mcu_state_b2
//  Input       : unsigned int uhdcp2x_mcu_state_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Set(unsigned int uhdcp2x_mcu_state_b2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B2 o_hdcp2x_mcu_state_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B2.u32);
    o_hdcp2x_mcu_state_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_state_b2.bits.hdcp2x_mcu_state_b2 = uhdcp2x_mcu_state_b2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b2.u32, "HDCP2X_MCU_STATE_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Get
//  Description : Set the value of the member HDCP2X_MCU_STATE_B2.hdcp2x_mcu_state_b2
//  Input       : unsigned int uhdcp2x_mcu_state_b2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B2 o_hdcp2x_mcu_state_b2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B2.u32);
    o_hdcp2x_mcu_state_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_state_b2.bits.hdcp2x_mcu_state_b2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Set
//  Description : Set the value of the member HDCP2X_MCU_STATE_B3.hdcp2x_mcu_state_b3
//  Input       : unsigned int uhdcp2x_mcu_state_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Set(unsigned int uhdcp2x_mcu_state_b3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B3 o_hdcp2x_mcu_state_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B3.u32);
    o_hdcp2x_mcu_state_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_mcu_state_b3.bits.hdcp2x_mcu_state_b3 = uhdcp2x_mcu_state_b3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_mcu_state_b3.u32, "HDCP2X_MCU_STATE_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Get
//  Description : Set the value of the member HDCP2X_MCU_STATE_B3.hdcp2x_mcu_state_b3
//  Input       : unsigned int uhdcp2x_mcu_state_b3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_MCU_STATE_B3 o_hdcp2x_mcu_state_b3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_MCU_STATE_B3.u32);
    o_hdcp2x_mcu_state_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_mcu_state_b3.bits.hdcp2x_mcu_state_b3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_sks_done
//  Input       : unsigned int uhdcp2x_send_sks_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneSet(unsigned int uhdcp2x_send_sks_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_send_sks_done = uhdcp2x_send_sks_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_sks_done
//  Input       : unsigned int uhdcp2x_send_sks_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_send_sks_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_l_done
//  Input       : unsigned int uhdcp2x_send_l_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneSet(unsigned int uhdcp2x_send_l_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_send_l_done = uhdcp2x_send_l_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_l_done
//  Input       : unsigned int uhdcp2x_send_l_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_send_l_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_lc_init_done
//  Input       : unsigned int uhdcp2x_lc_init_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneSet(unsigned int uhdcp2x_lc_init_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_lc_init_done = uhdcp2x_lc_init_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_lc_init_done
//  Input       : unsigned int uhdcp2x_lc_init_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_lc_init_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_pairing_done
//  Input       : unsigned int uhdcp2x_pairing_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneSet(unsigned int uhdcp2x_pairing_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_pairing_done = uhdcp2x_pairing_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_pairing_done
//  Input       : unsigned int uhdcp2x_pairing_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_pairing_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_h_done
//  Input       : unsigned int uhdcp2x_send_h_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneSet(unsigned int uhdcp2x_send_h_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_send_h_done = uhdcp2x_send_h_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_h_done
//  Input       : unsigned int uhdcp2x_send_h_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_send_h_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_km_done
//  Input       : unsigned int uhdcp2x_km_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneSet(unsigned int uhdcp2x_km_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_km_done = uhdcp2x_km_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_km_done
//  Input       : unsigned int uhdcp2x_km_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_km_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_cert_done
//  Input       : unsigned int uhdcp2x_send_cert_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneSet(unsigned int uhdcp2x_send_cert_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_send_cert_done = uhdcp2x_send_cert_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_send_cert_done
//  Input       : unsigned int uhdcp2x_send_cert_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_send_cert_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneSet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_ake_init_done
//  Input       : unsigned int uhdcp2x_ake_init_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneSet(unsigned int uhdcp2x_ake_init_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rx_auth_state.bits.hdcp2x_ake_init_done = uhdcp2x_ake_init_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rx_auth_state.u32, "HDCP2X_RX_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneGet
//  Description : Set the value of the member HDCP2X_RX_AUTH_STATE.hdcp2x_ake_init_done
//  Input       : unsigned int uhdcp2x_ake_init_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RX_AUTH_STATE o_hdcp2x_rx_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RX_AUTH_STATE.u32);
    o_hdcp2x_rx_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rx_auth_state.bits.hdcp2x_ake_init_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneSet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_strm_rdy_done
//  Input       : unsigned int uhdcp2x_strm_rdy_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneSet(unsigned int uhdcp2x_strm_rdy_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_auth_state.bits.hdcp2x_strm_rdy_done = uhdcp2x_strm_rdy_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32, "HDCP2X_RPT_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneGet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_strm_rdy_done
//  Input       : unsigned int uhdcp2x_strm_rdy_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_auth_state.bits.hdcp2x_strm_rdy_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneSet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_strm_id_done
//  Input       : unsigned int uhdcp2x_strm_id_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneSet(unsigned int uhdcp2x_strm_id_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_auth_state.bits.hdcp2x_strm_id_done = uhdcp2x_strm_id_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32, "HDCP2X_RPT_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneGet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_strm_id_done
//  Input       : unsigned int uhdcp2x_strm_id_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_auth_state.bits.hdcp2x_strm_id_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneSet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_send_ack_done
//  Input       : unsigned int uhdcp2x_send_ack_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneSet(unsigned int uhdcp2x_send_ack_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_auth_state.bits.hdcp2x_send_ack_done = uhdcp2x_send_ack_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32, "HDCP2X_RPT_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneGet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_send_ack_done
//  Input       : unsigned int uhdcp2x_send_ack_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_auth_state.bits.hdcp2x_send_ack_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneSet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_id_list_done
//  Input       : unsigned int uhdcp2x_id_list_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneSet(unsigned int uhdcp2x_id_list_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_auth_state.bits.hdcp2x_id_list_done = uhdcp2x_id_list_done;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_auth_state.u32, "HDCP2X_RPT_AUTH_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneGet
//  Description : Set the value of the member HDCP2X_RPT_AUTH_STATE.hdcp2x_id_list_done
//  Input       : unsigned int uhdcp2x_id_list_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_AUTH_STATE o_hdcp2x_rpt_auth_state;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_AUTH_STATE.u32);
    o_hdcp2x_rpt_auth_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_auth_state.bits.hdcp2x_id_list_done;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Set
//  Description : Set the value of the member HDCP2X_AUTH_STATE0.hdcp2x_auth_state0
//  Input       : unsigned int uhdcp2x_auth_state0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Set(unsigned int uhdcp2x_auth_state0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE0 o_hdcp2x_auth_state0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE0.u32);
    o_hdcp2x_auth_state0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_state0.bits.hdcp2x_auth_state0 = uhdcp2x_auth_state0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_state0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_state0.u32, "HDCP2X_AUTH_STATE0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Get
//  Description : Set the value of the member HDCP2X_AUTH_STATE0.hdcp2x_auth_state0
//  Input       : unsigned int uhdcp2x_auth_state0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE0 o_hdcp2x_auth_state0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE0.u32);
    o_hdcp2x_auth_state0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_state0.bits.hdcp2x_auth_state0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Set
//  Description : Set the value of the member HDCP2X_AUTH_STATE1.hdcp2x_auth_state1
//  Input       : unsigned int uhdcp2x_auth_state1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Set(unsigned int uhdcp2x_auth_state1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE1 o_hdcp2x_auth_state1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE1.u32);
    o_hdcp2x_auth_state1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_state1.bits.hdcp2x_auth_state1 = uhdcp2x_auth_state1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_state1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_state1.u32, "HDCP2X_AUTH_STATE1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Get
//  Description : Set the value of the member HDCP2X_AUTH_STATE1.hdcp2x_auth_state1
//  Input       : unsigned int uhdcp2x_auth_state1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE1 o_hdcp2x_auth_state1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE1.u32);
    o_hdcp2x_auth_state1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_state1.bits.hdcp2x_auth_state1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Set
//  Description : Set the value of the member HDCP2X_AUTH_STATE2.hdcp2x_auth_state2
//  Input       : unsigned int uhdcp2x_auth_state2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Set(unsigned int uhdcp2x_auth_state2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE2 o_hdcp2x_auth_state2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE2.u32);
    o_hdcp2x_auth_state2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_state2.bits.hdcp2x_auth_state2 = uhdcp2x_auth_state2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_state2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_state2.u32, "HDCP2X_AUTH_STATE2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Get
//  Description : Set the value of the member HDCP2X_AUTH_STATE2.hdcp2x_auth_state2
//  Input       : unsigned int uhdcp2x_auth_state2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE2 o_hdcp2x_auth_state2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE2.u32);
    o_hdcp2x_auth_state2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_state2.bits.hdcp2x_auth_state2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Set
//  Description : Set the value of the member HDCP2X_AUTH_STATE3.hdcp2x_auth_state3
//  Input       : unsigned int uhdcp2x_auth_state3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Set(unsigned int uhdcp2x_auth_state3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE3 o_hdcp2x_auth_state3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE3.u32);
    o_hdcp2x_auth_state3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_auth_state3.bits.hdcp2x_auth_state3 = uhdcp2x_auth_state3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_auth_state3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_auth_state3.u32, "HDCP2X_AUTH_STATE3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Get
//  Description : Set the value of the member HDCP2X_AUTH_STATE3.hdcp2x_auth_state3
//  Input       : unsigned int uhdcp2x_auth_state3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_AUTH_STATE3 o_hdcp2x_auth_state3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_AUTH_STATE3.u32);
    o_hdcp2x_auth_state3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_auth_state3.bits.hdcp2x_auth_state3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptSet
//  Description : Set the value of the member HDCP2X_CERT_INFO.hdcp2x_cert_rpt
//  Input       : unsigned int uhdcp2x_cert_rpt: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptSet(unsigned int uhdcp2x_cert_rpt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_CERT_INFO o_hdcp2x_cert_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_CERT_INFO.u32);
    o_hdcp2x_cert_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_cert_info.bits.hdcp2x_cert_rpt = uhdcp2x_cert_rpt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_cert_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_cert_info.u32, "HDCP2X_CERT_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptGet
//  Description : Set the value of the member HDCP2X_CERT_INFO.hdcp2x_cert_rpt
//  Input       : unsigned int uhdcp2x_cert_rpt: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_CERT_INFO o_hdcp2x_cert_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_CERT_INFO.u32);
    o_hdcp2x_cert_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_cert_info.bits.hdcp2x_cert_rpt;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verSet
//  Description : Set the value of the member HDCP2X_CERT_VER.hdcp2x_cert_ver
//  Input       : unsigned int uhdcp2x_cert_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verSet(unsigned int uhdcp2x_cert_ver)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_CERT_VER o_hdcp2x_cert_ver;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_CERT_VER.u32);
    o_hdcp2x_cert_ver.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_cert_ver.bits.hdcp2x_cert_ver = uhdcp2x_cert_ver;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_cert_ver.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_cert_ver.u32, "HDCP2X_CERT_VER");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verGet
//  Description : Set the value of the member HDCP2X_CERT_VER.hdcp2x_cert_ver
//  Input       : unsigned int uhdcp2x_cert_ver: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_CERT_VER o_hdcp2x_cert_ver;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_CERT_VER.u32);
    o_hdcp2x_cert_ver.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_cert_ver.bits.hdcp2x_cert_ver;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excSet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp2x_rpt_dev_exc
//  Input       : unsigned int uhdcp2x_rpt_dev_exc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excSet(unsigned int uhdcp2x_rpt_dev_exc)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_info.bits.hdcp2x_rpt_dev_exc = uhdcp2x_rpt_dev_exc;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32, "HDCP2X_RPT_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excGet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp2x_rpt_dev_exc
//  Input       : unsigned int uhdcp2x_rpt_dev_exc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_info.bits.hdcp2x_rpt_dev_exc;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excSet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp2x_rpt_cas_exc
//  Input       : unsigned int uhdcp2x_rpt_cas_exc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excSet(unsigned int uhdcp2x_rpt_cas_exc)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_info.bits.hdcp2x_rpt_cas_exc = uhdcp2x_rpt_cas_exc;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32, "HDCP2X_RPT_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excGet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp2x_rpt_cas_exc
//  Input       : unsigned int uhdcp2x_rpt_cas_exc: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_info.bits.hdcp2x_rpt_cas_exc;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onSet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp20_rpt_on
//  Input       : unsigned int uhdcp20_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onSet(unsigned int uhdcp20_rpt_on)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_info.bits.hdcp20_rpt_on = uhdcp20_rpt_on;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32, "HDCP2X_RPT_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onGet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp20_rpt_on
//  Input       : unsigned int uhdcp20_rpt_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_info.bits.hdcp20_rpt_on;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onSet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp1x_dev_on
//  Input       : unsigned int uhdcp1x_dev_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onSet(unsigned int uhdcp1x_dev_on)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_info.bits.hdcp1x_dev_on = uhdcp1x_dev_on;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_info.u32, "HDCP2X_RPT_INFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onGet
//  Description : Set the value of the member HDCP2X_RPT_INFO.hdcp1x_dev_on
//  Input       : unsigned int uhdcp1x_dev_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_INFO o_hdcp2x_rpt_info;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_INFO.u32);
    o_hdcp2x_rpt_info.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_info.bits.hdcp1x_dev_on;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntSet
//  Description : Set the value of the member HDCP2X_RPT_DEV.hdcp2x_rpt_dep_cnt
//  Input       : unsigned int uhdcp2x_rpt_dep_cnt: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntSet(unsigned int uhdcp2x_rpt_dep_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_DEV o_hdcp2x_rpt_dev;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_DEV.u32);
    o_hdcp2x_rpt_dev.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_dev.bits.hdcp2x_rpt_dep_cnt = uhdcp2x_rpt_dep_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_dev.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_dev.u32, "HDCP2X_RPT_DEV");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntGet
//  Description : Set the value of the member HDCP2X_RPT_DEV.hdcp2x_rpt_dep_cnt
//  Input       : unsigned int uhdcp2x_rpt_dep_cnt: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_DEV o_hdcp2x_rpt_dev;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_DEV.u32);
    o_hdcp2x_rpt_dev.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_dev.bits.hdcp2x_rpt_dep_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntSet
//  Description : Set the value of the member HDCP2X_RPT_DEV.hdcp2x_rpt_dev_cnt
//  Input       : unsigned int uhdcp2x_rpt_dev_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntSet(unsigned int uhdcp2x_rpt_dev_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_DEV o_hdcp2x_rpt_dev;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_DEV.u32);
    o_hdcp2x_rpt_dev.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_dev.bits.hdcp2x_rpt_dev_cnt = uhdcp2x_rpt_dev_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_dev.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_dev.u32, "HDCP2X_RPT_DEV");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntGet
//  Description : Set the value of the member HDCP2X_RPT_DEV.hdcp2x_rpt_dev_cnt
//  Input       : unsigned int uhdcp2x_rpt_dev_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_DEV o_hdcp2x_rpt_dev;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_DEV.u32);
    o_hdcp2x_rpt_dev.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_dev.bits.hdcp2x_rpt_dev_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Set
//  Description : Set the value of the member HDCP2X_RPT_V0.hdcp2x_rpt_v0
//  Input       : unsigned int uhdcp2x_rpt_v0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Set(unsigned int uhdcp2x_rpt_v0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V0 o_hdcp2x_rpt_v0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V0.u32);
    o_hdcp2x_rpt_v0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_v0.bits.hdcp2x_rpt_v0 = uhdcp2x_rpt_v0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_v0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_v0.u32, "HDCP2X_RPT_V0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Get
//  Description : Set the value of the member HDCP2X_RPT_V0.hdcp2x_rpt_v0
//  Input       : unsigned int uhdcp2x_rpt_v0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V0 o_hdcp2x_rpt_v0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V0.u32);
    o_hdcp2x_rpt_v0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_v0.bits.hdcp2x_rpt_v0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Set
//  Description : Set the value of the member HDCP2X_RPT_V1.hdcp2x_rpt_v1
//  Input       : unsigned int uhdcp2x_rpt_v1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Set(unsigned int uhdcp2x_rpt_v1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V1 o_hdcp2x_rpt_v1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V1.u32);
    o_hdcp2x_rpt_v1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_v1.bits.hdcp2x_rpt_v1 = uhdcp2x_rpt_v1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_v1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_v1.u32, "HDCP2X_RPT_V1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Get
//  Description : Set the value of the member HDCP2X_RPT_V1.hdcp2x_rpt_v1
//  Input       : unsigned int uhdcp2x_rpt_v1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V1 o_hdcp2x_rpt_v1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V1.u32);
    o_hdcp2x_rpt_v1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_v1.bits.hdcp2x_rpt_v1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Set
//  Description : Set the value of the member HDCP2X_RPT_V2.hdcp2x_rpt_v2
//  Input       : unsigned int uhdcp2x_rpt_v2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Set(unsigned int uhdcp2x_rpt_v2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V2 o_hdcp2x_rpt_v2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V2.u32);
    o_hdcp2x_rpt_v2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_v2.bits.hdcp2x_rpt_v2 = uhdcp2x_rpt_v2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_v2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_v2.u32, "HDCP2X_RPT_V2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Get
//  Description : Set the value of the member HDCP2X_RPT_V2.hdcp2x_rpt_v2
//  Input       : unsigned int uhdcp2x_rpt_v2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_V2 o_hdcp2x_rpt_v2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_V2.u32);
    o_hdcp2x_rpt_v2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_v2.bits.hdcp2x_rpt_v2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Set
//  Description : Set the value of the member HDCP2X_RPT_K0.hdcp2x_rpt_k0
//  Input       : unsigned int uhdcp2x_rpt_k0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Set(unsigned int uhdcp2x_rpt_k0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_K0 o_hdcp2x_rpt_k0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_K0.u32);
    o_hdcp2x_rpt_k0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_k0.bits.hdcp2x_rpt_k0 = uhdcp2x_rpt_k0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_k0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_k0.u32, "HDCP2X_RPT_K0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Get
//  Description : Set the value of the member HDCP2X_RPT_K0.hdcp2x_rpt_k0
//  Input       : unsigned int uhdcp2x_rpt_k0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_K0 o_hdcp2x_rpt_k0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_K0.u32);
    o_hdcp2x_rpt_k0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_k0.bits.hdcp2x_rpt_k0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Set
//  Description : Set the value of the member HDCP2X_RPT_K1.hdcp2x_rpt_k1
//  Input       : unsigned int uhdcp2x_rpt_k1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Set(unsigned int uhdcp2x_rpt_k1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_K1 o_hdcp2x_rpt_k1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_K1.u32);
    o_hdcp2x_rpt_k1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_k1.bits.hdcp2x_rpt_k1 = uhdcp2x_rpt_k1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_k1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_k1.u32, "HDCP2X_RPT_K1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Get
//  Description : Set the value of the member HDCP2X_RPT_K1.hdcp2x_rpt_k1
//  Input       : unsigned int uhdcp2x_rpt_k1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_K1 o_hdcp2x_rpt_k1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_K1.u32);
    o_hdcp2x_rpt_k1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_k1.bits.hdcp2x_rpt_k1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Set
//  Description : Set the value of the member HDCP2X_RPT_M0.hdcp2x_rpt_m0
//  Input       : unsigned int uhdcp2x_rpt_m0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Set(unsigned int uhdcp2x_rpt_m0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M0 o_hdcp2x_rpt_m0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M0.u32);
    o_hdcp2x_rpt_m0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_m0.bits.hdcp2x_rpt_m0 = uhdcp2x_rpt_m0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_m0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_m0.u32, "HDCP2X_RPT_M0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Get
//  Description : Set the value of the member HDCP2X_RPT_M0.hdcp2x_rpt_m0
//  Input       : unsigned int uhdcp2x_rpt_m0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M0 o_hdcp2x_rpt_m0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M0.u32);
    o_hdcp2x_rpt_m0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_m0.bits.hdcp2x_rpt_m0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Set
//  Description : Set the value of the member HDCP2X_RPT_M1.hdcp2x_rpt_m1
//  Input       : unsigned int uhdcp2x_rpt_m1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Set(unsigned int uhdcp2x_rpt_m1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M1 o_hdcp2x_rpt_m1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M1.u32);
    o_hdcp2x_rpt_m1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_m1.bits.hdcp2x_rpt_m1 = uhdcp2x_rpt_m1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_m1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_m1.u32, "HDCP2X_RPT_M1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Get
//  Description : Set the value of the member HDCP2X_RPT_M1.hdcp2x_rpt_m1
//  Input       : unsigned int uhdcp2x_rpt_m1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M1 o_hdcp2x_rpt_m1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M1.u32);
    o_hdcp2x_rpt_m1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_m1.bits.hdcp2x_rpt_m1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Set
//  Description : Set the value of the member HDCP2X_RPT_M2.hdcp2x_rpt_m2
//  Input       : unsigned int uhdcp2x_rpt_m2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Set(unsigned int uhdcp2x_rpt_m2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M2 o_hdcp2x_rpt_m2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M2.u32);
    o_hdcp2x_rpt_m2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_m2.bits.hdcp2x_rpt_m2 = uhdcp2x_rpt_m2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_m2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_m2.u32, "HDCP2X_RPT_M2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Get
//  Description : Set the value of the member HDCP2X_RPT_M2.hdcp2x_rpt_m2
//  Input       : unsigned int uhdcp2x_rpt_m2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_M2 o_hdcp2x_rpt_m2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_M2.u32);
    o_hdcp2x_rpt_m2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_m2.bits.hdcp2x_rpt_m2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idSet
//  Description : Set the value of the member HDCP2X_RPT_STRM_ID.hdcp2x_rpt_strm_id
//  Input       : unsigned int uhdcp2x_rpt_strm_id: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idSet(unsigned int uhdcp2x_rpt_strm_id)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_STRM_ID o_hdcp2x_rpt_strm_id;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_STRM_ID.u32);
    o_hdcp2x_rpt_strm_id.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_strm_id.bits.hdcp2x_rpt_strm_id = uhdcp2x_rpt_strm_id;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_strm_id.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_strm_id.u32, "HDCP2X_RPT_STRM_ID");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idGet
//  Description : Set the value of the member HDCP2X_RPT_STRM_ID.hdcp2x_rpt_strm_id
//  Input       : unsigned int uhdcp2x_rpt_strm_id: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_STRM_ID o_hdcp2x_rpt_strm_id;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_STRM_ID.u32);
    o_hdcp2x_rpt_strm_id.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_strm_id.bits.hdcp2x_rpt_strm_id;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeSet
//  Description : Set the value of the member HDCP2X_RPT_STRM_TPYE.hdcp2x_rpt_strm_type
//  Input       : unsigned int uhdcp2x_rpt_strm_type: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeSet(unsigned int uhdcp2x_rpt_strm_type)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_STRM_TPYE o_hdcp2x_rpt_strm_tpye;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_STRM_TPYE.u32);
    o_hdcp2x_rpt_strm_tpye.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_rpt_strm_tpye.bits.hdcp2x_rpt_strm_type = uhdcp2x_rpt_strm_type;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_rpt_strm_tpye.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_rpt_strm_tpye.u32, "HDCP2X_RPT_STRM_TPYE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeGet
//  Description : Set the value of the member HDCP2X_RPT_STRM_TPYE.hdcp2x_rpt_strm_type
//  Input       : unsigned int uhdcp2x_rpt_strm_type: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_RPT_STRM_TPYE o_hdcp2x_rpt_strm_tpye;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_RPT_STRM_TPYE.u32);
    o_hdcp2x_rpt_strm_tpye.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_rpt_strm_tpye.bits.hdcp2x_rpt_strm_type;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG0.hdcp2x_timing_cfg0
//  Input       : unsigned int uhdcp2x_timing_cfg0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Set(unsigned int uhdcp2x_timing_cfg0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG0 o_hdcp2x_timing_cfg0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG0.u32);
    o_hdcp2x_timing_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg0.bits.hdcp2x_timing_cfg0 = uhdcp2x_timing_cfg0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg0.u32, "HDCP2X_TIMING_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG0.hdcp2x_timing_cfg0
//  Input       : unsigned int uhdcp2x_timing_cfg0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG0 o_hdcp2x_timing_cfg0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG0.u32);
    o_hdcp2x_timing_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg0.bits.hdcp2x_timing_cfg0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG1.hdcp2x_timing_cfg1
//  Input       : unsigned int uhdcp2x_timing_cfg1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Set(unsigned int uhdcp2x_timing_cfg1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG1 o_hdcp2x_timing_cfg1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG1.u32);
    o_hdcp2x_timing_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg1.bits.hdcp2x_timing_cfg1 = uhdcp2x_timing_cfg1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg1.u32, "HDCP2X_TIMING_CFG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG1.hdcp2x_timing_cfg1
//  Input       : unsigned int uhdcp2x_timing_cfg1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG1 o_hdcp2x_timing_cfg1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG1.u32);
    o_hdcp2x_timing_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg1.bits.hdcp2x_timing_cfg1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG2.hdcp2x_timing_cfg2
//  Input       : unsigned int uhdcp2x_timing_cfg2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Set(unsigned int uhdcp2x_timing_cfg2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG2 o_hdcp2x_timing_cfg2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG2.u32);
    o_hdcp2x_timing_cfg2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg2.bits.hdcp2x_timing_cfg2 = uhdcp2x_timing_cfg2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg2.u32, "HDCP2X_TIMING_CFG2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG2.hdcp2x_timing_cfg2
//  Input       : unsigned int uhdcp2x_timing_cfg2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG2 o_hdcp2x_timing_cfg2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG2.u32);
    o_hdcp2x_timing_cfg2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg2.bits.hdcp2x_timing_cfg2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG3.hdcp2x_timing_cfg3
//  Input       : unsigned int uhdcp2x_timing_cfg3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Set(unsigned int uhdcp2x_timing_cfg3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG3 o_hdcp2x_timing_cfg3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG3.u32);
    o_hdcp2x_timing_cfg3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg3.bits.hdcp2x_timing_cfg3 = uhdcp2x_timing_cfg3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg3.u32, "HDCP2X_TIMING_CFG3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG3.hdcp2x_timing_cfg3
//  Input       : unsigned int uhdcp2x_timing_cfg3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG3 o_hdcp2x_timing_cfg3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG3.u32);
    o_hdcp2x_timing_cfg3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg3.bits.hdcp2x_timing_cfg3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG4.hdcp2x_timing_cfg4
//  Input       : unsigned int uhdcp2x_timing_cfg4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Set(unsigned int uhdcp2x_timing_cfg4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG4 o_hdcp2x_timing_cfg4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG4.u32);
    o_hdcp2x_timing_cfg4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg4.bits.hdcp2x_timing_cfg4 = uhdcp2x_timing_cfg4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg4.u32, "HDCP2X_TIMING_CFG4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG4.hdcp2x_timing_cfg4
//  Input       : unsigned int uhdcp2x_timing_cfg4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG4 o_hdcp2x_timing_cfg4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG4.u32);
    o_hdcp2x_timing_cfg4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg4.bits.hdcp2x_timing_cfg4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG5.hdcp2x_timing_cfg5
//  Input       : unsigned int uhdcp2x_timing_cfg5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Set(unsigned int uhdcp2x_timing_cfg5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG5 o_hdcp2x_timing_cfg5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG5.u32);
    o_hdcp2x_timing_cfg5.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg5.bits.hdcp2x_timing_cfg5 = uhdcp2x_timing_cfg5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg5.u32, "HDCP2X_TIMING_CFG5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG5.hdcp2x_timing_cfg5
//  Input       : unsigned int uhdcp2x_timing_cfg5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG5 o_hdcp2x_timing_cfg5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG5.u32);
    o_hdcp2x_timing_cfg5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg5.bits.hdcp2x_timing_cfg5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG6.hdcp2x_timing_cfg6
//  Input       : unsigned int uhdcp2x_timing_cfg6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Set(unsigned int uhdcp2x_timing_cfg6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG6 o_hdcp2x_timing_cfg6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG6.u32);
    o_hdcp2x_timing_cfg6.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg6.bits.hdcp2x_timing_cfg6 = uhdcp2x_timing_cfg6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg6.u32, "HDCP2X_TIMING_CFG6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG6.hdcp2x_timing_cfg6
//  Input       : unsigned int uhdcp2x_timing_cfg6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG6 o_hdcp2x_timing_cfg6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG6.u32);
    o_hdcp2x_timing_cfg6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg6.bits.hdcp2x_timing_cfg6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG7.hdcp2x_timing_cfg7
//  Input       : unsigned int uhdcp2x_timing_cfg7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Set(unsigned int uhdcp2x_timing_cfg7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG7 o_hdcp2x_timing_cfg7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG7.u32);
    o_hdcp2x_timing_cfg7.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg7.bits.hdcp2x_timing_cfg7 = uhdcp2x_timing_cfg7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg7.u32, "HDCP2X_TIMING_CFG7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG7.hdcp2x_timing_cfg7
//  Input       : unsigned int uhdcp2x_timing_cfg7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG7 o_hdcp2x_timing_cfg7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG7.u32);
    o_hdcp2x_timing_cfg7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg7.bits.hdcp2x_timing_cfg7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG8.hdcp2x_timing_cfg8
//  Input       : unsigned int uhdcp2x_timing_cfg8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Set(unsigned int uhdcp2x_timing_cfg8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG8 o_hdcp2x_timing_cfg8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG8.u32);
    o_hdcp2x_timing_cfg8.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg8.bits.hdcp2x_timing_cfg8 = uhdcp2x_timing_cfg8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg8.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg8.u32, "HDCP2X_TIMING_CFG8");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG8.hdcp2x_timing_cfg8
//  Input       : unsigned int uhdcp2x_timing_cfg8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG8 o_hdcp2x_timing_cfg8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG8.u32);
    o_hdcp2x_timing_cfg8.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg8.bits.hdcp2x_timing_cfg8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG9.hdcp2x_timing_cfg9
//  Input       : unsigned int uhdcp2x_timing_cfg9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Set(unsigned int uhdcp2x_timing_cfg9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG9 o_hdcp2x_timing_cfg9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG9.u32);
    o_hdcp2x_timing_cfg9.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg9.bits.hdcp2x_timing_cfg9 = uhdcp2x_timing_cfg9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg9.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg9.u32, "HDCP2X_TIMING_CFG9");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG9.hdcp2x_timing_cfg9
//  Input       : unsigned int uhdcp2x_timing_cfg9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG9 o_hdcp2x_timing_cfg9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG9.u32);
    o_hdcp2x_timing_cfg9.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg9.bits.hdcp2x_timing_cfg9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG10.hdcp2x_timing_cfg10
//  Input       : unsigned int uhdcp2x_timing_cfg10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Set(unsigned int uhdcp2x_timing_cfg10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG10 o_hdcp2x_timing_cfg10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG10.u32);
    o_hdcp2x_timing_cfg10.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg10.bits.hdcp2x_timing_cfg10 = uhdcp2x_timing_cfg10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg10.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg10.u32, "HDCP2X_TIMING_CFG10");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG10.hdcp2x_timing_cfg10
//  Input       : unsigned int uhdcp2x_timing_cfg10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG10 o_hdcp2x_timing_cfg10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG10.u32);
    o_hdcp2x_timing_cfg10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg10.bits.hdcp2x_timing_cfg10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG11.hdcp2x_timing_cfg11
//  Input       : unsigned int uhdcp2x_timing_cfg11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Set(unsigned int uhdcp2x_timing_cfg11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG11 o_hdcp2x_timing_cfg11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG11.u32);
    o_hdcp2x_timing_cfg11.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg11.bits.hdcp2x_timing_cfg11 = uhdcp2x_timing_cfg11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg11.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg11.u32, "HDCP2X_TIMING_CFG11");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG11.hdcp2x_timing_cfg11
//  Input       : unsigned int uhdcp2x_timing_cfg11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG11 o_hdcp2x_timing_cfg11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG11.u32);
    o_hdcp2x_timing_cfg11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg11.bits.hdcp2x_timing_cfg11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG12.hdcp2x_timing_cfg12
//  Input       : unsigned int uhdcp2x_timing_cfg12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Set(unsigned int uhdcp2x_timing_cfg12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG12 o_hdcp2x_timing_cfg12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG12.u32);
    o_hdcp2x_timing_cfg12.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg12.bits.hdcp2x_timing_cfg12 = uhdcp2x_timing_cfg12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg12.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg12.u32, "HDCP2X_TIMING_CFG12");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG12.hdcp2x_timing_cfg12
//  Input       : unsigned int uhdcp2x_timing_cfg12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG12 o_hdcp2x_timing_cfg12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG12.u32);
    o_hdcp2x_timing_cfg12.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg12.bits.hdcp2x_timing_cfg12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG13.hdcp2x_timing_cfg13
//  Input       : unsigned int uhdcp2x_timing_cfg13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Set(unsigned int uhdcp2x_timing_cfg13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG13 o_hdcp2x_timing_cfg13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG13.u32);
    o_hdcp2x_timing_cfg13.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg13.bits.hdcp2x_timing_cfg13 = uhdcp2x_timing_cfg13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg13.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg13.u32, "HDCP2X_TIMING_CFG13");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG13.hdcp2x_timing_cfg13
//  Input       : unsigned int uhdcp2x_timing_cfg13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG13 o_hdcp2x_timing_cfg13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG13.u32);
    o_hdcp2x_timing_cfg13.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg13.bits.hdcp2x_timing_cfg13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG14.hdcp2x_timing_cfg14
//  Input       : unsigned int uhdcp2x_timing_cfg14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Set(unsigned int uhdcp2x_timing_cfg14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG14 o_hdcp2x_timing_cfg14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG14.u32);
    o_hdcp2x_timing_cfg14.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg14.bits.hdcp2x_timing_cfg14 = uhdcp2x_timing_cfg14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg14.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg14.u32, "HDCP2X_TIMING_CFG14");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG14.hdcp2x_timing_cfg14
//  Input       : unsigned int uhdcp2x_timing_cfg14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG14 o_hdcp2x_timing_cfg14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG14.u32);
    o_hdcp2x_timing_cfg14.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg14.bits.hdcp2x_timing_cfg14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG15.hdcp2x_timing_cfg15
//  Input       : unsigned int uhdcp2x_timing_cfg15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Set(unsigned int uhdcp2x_timing_cfg15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG15 o_hdcp2x_timing_cfg15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG15.u32);
    o_hdcp2x_timing_cfg15.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg15.bits.hdcp2x_timing_cfg15 = uhdcp2x_timing_cfg15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg15.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg15.u32, "HDCP2X_TIMING_CFG15");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG15.hdcp2x_timing_cfg15
//  Input       : unsigned int uhdcp2x_timing_cfg15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG15 o_hdcp2x_timing_cfg15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG15.u32);
    o_hdcp2x_timing_cfg15.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg15.bits.hdcp2x_timing_cfg15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG16.hdcp2x_timing_cfg16
//  Input       : unsigned int uhdcp2x_timing_cfg16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Set(unsigned int uhdcp2x_timing_cfg16)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG16 o_hdcp2x_timing_cfg16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG16.u32);
    o_hdcp2x_timing_cfg16.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg16.bits.hdcp2x_timing_cfg16 = uhdcp2x_timing_cfg16;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg16.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg16.u32, "HDCP2X_TIMING_CFG16");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG16.hdcp2x_timing_cfg16
//  Input       : unsigned int uhdcp2x_timing_cfg16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG16 o_hdcp2x_timing_cfg16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG16.u32);
    o_hdcp2x_timing_cfg16.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg16.bits.hdcp2x_timing_cfg16;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG17.hdcp2x_timing_cfg17
//  Input       : unsigned int uhdcp2x_timing_cfg17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Set(unsigned int uhdcp2x_timing_cfg17)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG17 o_hdcp2x_timing_cfg17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG17.u32);
    o_hdcp2x_timing_cfg17.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg17.bits.hdcp2x_timing_cfg17 = uhdcp2x_timing_cfg17;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg17.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg17.u32, "HDCP2X_TIMING_CFG17");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG17.hdcp2x_timing_cfg17
//  Input       : unsigned int uhdcp2x_timing_cfg17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG17 o_hdcp2x_timing_cfg17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG17.u32);
    o_hdcp2x_timing_cfg17.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg17.bits.hdcp2x_timing_cfg17;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG18.hdcp2x_timing_cfg18
//  Input       : unsigned int uhdcp2x_timing_cfg18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Set(unsigned int uhdcp2x_timing_cfg18)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG18 o_hdcp2x_timing_cfg18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG18.u32);
    o_hdcp2x_timing_cfg18.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg18.bits.hdcp2x_timing_cfg18 = uhdcp2x_timing_cfg18;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg18.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg18.u32, "HDCP2X_TIMING_CFG18");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG18.hdcp2x_timing_cfg18
//  Input       : unsigned int uhdcp2x_timing_cfg18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG18 o_hdcp2x_timing_cfg18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG18.u32);
    o_hdcp2x_timing_cfg18.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg18.bits.hdcp2x_timing_cfg18;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG19.hdcp2x_timing_cfg19
//  Input       : unsigned int uhdcp2x_timing_cfg19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Set(unsigned int uhdcp2x_timing_cfg19)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG19 o_hdcp2x_timing_cfg19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG19.u32);
    o_hdcp2x_timing_cfg19.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg19.bits.hdcp2x_timing_cfg19 = uhdcp2x_timing_cfg19;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg19.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg19.u32, "HDCP2X_TIMING_CFG19");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG19.hdcp2x_timing_cfg19
//  Input       : unsigned int uhdcp2x_timing_cfg19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG19 o_hdcp2x_timing_cfg19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG19.u32);
    o_hdcp2x_timing_cfg19.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg19.bits.hdcp2x_timing_cfg19;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG20.hdcp2x_timing_cfg20
//  Input       : unsigned int uhdcp2x_timing_cfg20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Set(unsigned int uhdcp2x_timing_cfg20)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG20 o_hdcp2x_timing_cfg20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG20.u32);
    o_hdcp2x_timing_cfg20.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg20.bits.hdcp2x_timing_cfg20 = uhdcp2x_timing_cfg20;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg20.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg20.u32, "HDCP2X_TIMING_CFG20");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG20.hdcp2x_timing_cfg20
//  Input       : unsigned int uhdcp2x_timing_cfg20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG20 o_hdcp2x_timing_cfg20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG20.u32);
    o_hdcp2x_timing_cfg20.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg20.bits.hdcp2x_timing_cfg20;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG21.hdcp2x_timing_cfg21
//  Input       : unsigned int uhdcp2x_timing_cfg21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Set(unsigned int uhdcp2x_timing_cfg21)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG21 o_hdcp2x_timing_cfg21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG21.u32);
    o_hdcp2x_timing_cfg21.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg21.bits.hdcp2x_timing_cfg21 = uhdcp2x_timing_cfg21;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg21.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg21.u32, "HDCP2X_TIMING_CFG21");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG21.hdcp2x_timing_cfg21
//  Input       : unsigned int uhdcp2x_timing_cfg21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG21 o_hdcp2x_timing_cfg21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG21.u32);
    o_hdcp2x_timing_cfg21.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg21.bits.hdcp2x_timing_cfg21;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG22.hdcp2x_timing_cfg22
//  Input       : unsigned int uhdcp2x_timing_cfg22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Set(unsigned int uhdcp2x_timing_cfg22)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG22 o_hdcp2x_timing_cfg22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG22.u32);
    o_hdcp2x_timing_cfg22.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg22.bits.hdcp2x_timing_cfg22 = uhdcp2x_timing_cfg22;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg22.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg22.u32, "HDCP2X_TIMING_CFG22");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG22.hdcp2x_timing_cfg22
//  Input       : unsigned int uhdcp2x_timing_cfg22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG22 o_hdcp2x_timing_cfg22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG22.u32);
    o_hdcp2x_timing_cfg22.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg22.bits.hdcp2x_timing_cfg22;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG23.hdcp2x_timing_cfg23
//  Input       : unsigned int uhdcp2x_timing_cfg23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Set(unsigned int uhdcp2x_timing_cfg23)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG23 o_hdcp2x_timing_cfg23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG23.u32);
    o_hdcp2x_timing_cfg23.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg23.bits.hdcp2x_timing_cfg23 = uhdcp2x_timing_cfg23;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg23.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg23.u32, "HDCP2X_TIMING_CFG23");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG23.hdcp2x_timing_cfg23
//  Input       : unsigned int uhdcp2x_timing_cfg23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG23 o_hdcp2x_timing_cfg23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG23.u32);
    o_hdcp2x_timing_cfg23.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg23.bits.hdcp2x_timing_cfg23;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG24.hdcp2x_timing_cfg24
//  Input       : unsigned int uhdcp2x_timing_cfg24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Set(unsigned int uhdcp2x_timing_cfg24)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG24 o_hdcp2x_timing_cfg24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG24.u32);
    o_hdcp2x_timing_cfg24.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg24.bits.hdcp2x_timing_cfg24 = uhdcp2x_timing_cfg24;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg24.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg24.u32, "HDCP2X_TIMING_CFG24");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG24.hdcp2x_timing_cfg24
//  Input       : unsigned int uhdcp2x_timing_cfg24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG24 o_hdcp2x_timing_cfg24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG24.u32);
    o_hdcp2x_timing_cfg24.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg24.bits.hdcp2x_timing_cfg24;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG25.hdcp2x_timing_cfg25
//  Input       : unsigned int uhdcp2x_timing_cfg25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Set(unsigned int uhdcp2x_timing_cfg25)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG25 o_hdcp2x_timing_cfg25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG25.u32);
    o_hdcp2x_timing_cfg25.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg25.bits.hdcp2x_timing_cfg25 = uhdcp2x_timing_cfg25;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg25.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg25.u32, "HDCP2X_TIMING_CFG25");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG25.hdcp2x_timing_cfg25
//  Input       : unsigned int uhdcp2x_timing_cfg25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG25 o_hdcp2x_timing_cfg25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG25.u32);
    o_hdcp2x_timing_cfg25.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg25.bits.hdcp2x_timing_cfg25;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG26.hdcp2x_timing_cfg26
//  Input       : unsigned int uhdcp2x_timing_cfg26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Set(unsigned int uhdcp2x_timing_cfg26)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG26 o_hdcp2x_timing_cfg26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG26.u32);
    o_hdcp2x_timing_cfg26.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg26.bits.hdcp2x_timing_cfg26 = uhdcp2x_timing_cfg26;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg26.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg26.u32, "HDCP2X_TIMING_CFG26");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG26.hdcp2x_timing_cfg26
//  Input       : unsigned int uhdcp2x_timing_cfg26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG26 o_hdcp2x_timing_cfg26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG26.u32);
    o_hdcp2x_timing_cfg26.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg26.bits.hdcp2x_timing_cfg26;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG27.hdcp2x_timing_cfg27
//  Input       : unsigned int uhdcp2x_timing_cfg27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Set(unsigned int uhdcp2x_timing_cfg27)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG27 o_hdcp2x_timing_cfg27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG27.u32);
    o_hdcp2x_timing_cfg27.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg27.bits.hdcp2x_timing_cfg27 = uhdcp2x_timing_cfg27;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg27.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg27.u32, "HDCP2X_TIMING_CFG27");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG27.hdcp2x_timing_cfg27
//  Input       : unsigned int uhdcp2x_timing_cfg27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG27 o_hdcp2x_timing_cfg27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG27.u32);
    o_hdcp2x_timing_cfg27.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg27.bits.hdcp2x_timing_cfg27;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG28.hdcp2x_timing_cfg28
//  Input       : unsigned int uhdcp2x_timing_cfg28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Set(unsigned int uhdcp2x_timing_cfg28)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG28 o_hdcp2x_timing_cfg28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG28.u32);
    o_hdcp2x_timing_cfg28.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg28.bits.hdcp2x_timing_cfg28 = uhdcp2x_timing_cfg28;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg28.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg28.u32, "HDCP2X_TIMING_CFG28");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG28.hdcp2x_timing_cfg28
//  Input       : unsigned int uhdcp2x_timing_cfg28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG28 o_hdcp2x_timing_cfg28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG28.u32);
    o_hdcp2x_timing_cfg28.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg28.bits.hdcp2x_timing_cfg28;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG29.hdcp2x_timing_cfg29
//  Input       : unsigned int uhdcp2x_timing_cfg29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Set(unsigned int uhdcp2x_timing_cfg29)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG29 o_hdcp2x_timing_cfg29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG29.u32);
    o_hdcp2x_timing_cfg29.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg29.bits.hdcp2x_timing_cfg29 = uhdcp2x_timing_cfg29;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg29.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg29.u32, "HDCP2X_TIMING_CFG29");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG29.hdcp2x_timing_cfg29
//  Input       : unsigned int uhdcp2x_timing_cfg29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG29 o_hdcp2x_timing_cfg29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG29.u32);
    o_hdcp2x_timing_cfg29.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg29.bits.hdcp2x_timing_cfg29;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG30.hdcp2x_timing_cfg30
//  Input       : unsigned int uhdcp2x_timing_cfg30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Set(unsigned int uhdcp2x_timing_cfg30)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG30 o_hdcp2x_timing_cfg30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG30.u32);
    o_hdcp2x_timing_cfg30.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg30.bits.hdcp2x_timing_cfg30 = uhdcp2x_timing_cfg30;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg30.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg30.u32, "HDCP2X_TIMING_CFG30");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG30.hdcp2x_timing_cfg30
//  Input       : unsigned int uhdcp2x_timing_cfg30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG30 o_hdcp2x_timing_cfg30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG30.u32);
    o_hdcp2x_timing_cfg30.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg30.bits.hdcp2x_timing_cfg30;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Set
//  Description : Set the value of the member HDCP2X_TIMING_CFG31.hdcp2x_timing_cfg31
//  Input       : unsigned int uhdcp2x_timing_cfg31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Set(unsigned int uhdcp2x_timing_cfg31)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG31 o_hdcp2x_timing_cfg31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG31.u32);
    o_hdcp2x_timing_cfg31.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_timing_cfg31.bits.hdcp2x_timing_cfg31 = uhdcp2x_timing_cfg31;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg31.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_timing_cfg31.u32, "HDCP2X_TIMING_CFG31");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Get
//  Description : Set the value of the member HDCP2X_TIMING_CFG31.hdcp2x_timing_cfg31
//  Input       : unsigned int uhdcp2x_timing_cfg31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_TIMING_CFG31 o_hdcp2x_timing_cfg31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_TIMING_CFG31.u32);
    o_hdcp2x_timing_cfg31.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_timing_cfg31.bits.hdcp2x_timing_cfg31;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Set
//  Description : Set the value of the member HDCP2X_GEN_IN0.hdcp2x_gen_in0
//  Input       : unsigned int uhdcp2x_gen_in0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Set(unsigned int uhdcp2x_gen_in0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN0 o_hdcp2x_gen_in0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN0.u32);
    o_hdcp2x_gen_in0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in0.bits.hdcp2x_gen_in0 = uhdcp2x_gen_in0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in0.u32, "HDCP2X_GEN_IN0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Get
//  Description : Set the value of the member HDCP2X_GEN_IN0.hdcp2x_gen_in0
//  Input       : unsigned int uhdcp2x_gen_in0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN0 o_hdcp2x_gen_in0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN0.u32);
    o_hdcp2x_gen_in0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in0.bits.hdcp2x_gen_in0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Set
//  Description : Set the value of the member HDCP2X_GEN_IN1.hdcp2x_gen_in1
//  Input       : unsigned int uhdcp2x_gen_in1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Set(unsigned int uhdcp2x_gen_in1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN1 o_hdcp2x_gen_in1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN1.u32);
    o_hdcp2x_gen_in1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in1.bits.hdcp2x_gen_in1 = uhdcp2x_gen_in1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in1.u32, "HDCP2X_GEN_IN1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Get
//  Description : Set the value of the member HDCP2X_GEN_IN1.hdcp2x_gen_in1
//  Input       : unsigned int uhdcp2x_gen_in1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN1 o_hdcp2x_gen_in1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN1.u32);
    o_hdcp2x_gen_in1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in1.bits.hdcp2x_gen_in1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Set
//  Description : Set the value of the member HDCP2X_GEN_IN2.hdcp2x_gen_in2
//  Input       : unsigned int uhdcp2x_gen_in2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Set(unsigned int uhdcp2x_gen_in2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN2 o_hdcp2x_gen_in2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN2.u32);
    o_hdcp2x_gen_in2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in2.bits.hdcp2x_gen_in2 = uhdcp2x_gen_in2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in2.u32, "HDCP2X_GEN_IN2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Get
//  Description : Set the value of the member HDCP2X_GEN_IN2.hdcp2x_gen_in2
//  Input       : unsigned int uhdcp2x_gen_in2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN2 o_hdcp2x_gen_in2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN2.u32);
    o_hdcp2x_gen_in2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in2.bits.hdcp2x_gen_in2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Set
//  Description : Set the value of the member HDCP2X_GEN_IN3.hdcp2x_gen_in3
//  Input       : unsigned int uhdcp2x_gen_in3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Set(unsigned int uhdcp2x_gen_in3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN3 o_hdcp2x_gen_in3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN3.u32);
    o_hdcp2x_gen_in3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in3.bits.hdcp2x_gen_in3 = uhdcp2x_gen_in3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in3.u32, "HDCP2X_GEN_IN3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Get
//  Description : Set the value of the member HDCP2X_GEN_IN3.hdcp2x_gen_in3
//  Input       : unsigned int uhdcp2x_gen_in3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN3 o_hdcp2x_gen_in3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN3.u32);
    o_hdcp2x_gen_in3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in3.bits.hdcp2x_gen_in3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Set
//  Description : Set the value of the member HDCP2X_GEN_IN4.hdcp2x_gen_in4
//  Input       : unsigned int uhdcp2x_gen_in4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Set(unsigned int uhdcp2x_gen_in4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN4 o_hdcp2x_gen_in4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN4.u32);
    o_hdcp2x_gen_in4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in4.bits.hdcp2x_gen_in4 = uhdcp2x_gen_in4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in4.u32, "HDCP2X_GEN_IN4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Get
//  Description : Set the value of the member HDCP2X_GEN_IN4.hdcp2x_gen_in4
//  Input       : unsigned int uhdcp2x_gen_in4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN4 o_hdcp2x_gen_in4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN4.u32);
    o_hdcp2x_gen_in4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in4.bits.hdcp2x_gen_in4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Set
//  Description : Set the value of the member HDCP2X_GEN_IN5.hdcp2x_gen_in5
//  Input       : unsigned int uhdcp2x_gen_in5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Set(unsigned int uhdcp2x_gen_in5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN5 o_hdcp2x_gen_in5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN5.u32);
    o_hdcp2x_gen_in5.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in5.bits.hdcp2x_gen_in5 = uhdcp2x_gen_in5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in5.u32, "HDCP2X_GEN_IN5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Get
//  Description : Set the value of the member HDCP2X_GEN_IN5.hdcp2x_gen_in5
//  Input       : unsigned int uhdcp2x_gen_in5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN5 o_hdcp2x_gen_in5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN5.u32);
    o_hdcp2x_gen_in5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in5.bits.hdcp2x_gen_in5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Set
//  Description : Set the value of the member HDCP2X_GEN_IN6.hdcp2x_gen_in6
//  Input       : unsigned int uhdcp2x_gen_in6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Set(unsigned int uhdcp2x_gen_in6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN6 o_hdcp2x_gen_in6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN6.u32);
    o_hdcp2x_gen_in6.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in6.bits.hdcp2x_gen_in6 = uhdcp2x_gen_in6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in6.u32, "HDCP2X_GEN_IN6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Get
//  Description : Set the value of the member HDCP2X_GEN_IN6.hdcp2x_gen_in6
//  Input       : unsigned int uhdcp2x_gen_in6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN6 o_hdcp2x_gen_in6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN6.u32);
    o_hdcp2x_gen_in6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in6.bits.hdcp2x_gen_in6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Set
//  Description : Set the value of the member HDCP2X_GEN_IN7.hdcp2x_gen_in7
//  Input       : unsigned int uhdcp2x_gen_in7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Set(unsigned int uhdcp2x_gen_in7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN7 o_hdcp2x_gen_in7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN7.u32);
    o_hdcp2x_gen_in7.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in7.bits.hdcp2x_gen_in7 = uhdcp2x_gen_in7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in7.u32, "HDCP2X_GEN_IN7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Get
//  Description : Set the value of the member HDCP2X_GEN_IN7.hdcp2x_gen_in7
//  Input       : unsigned int uhdcp2x_gen_in7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN7 o_hdcp2x_gen_in7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN7.u32);
    o_hdcp2x_gen_in7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in7.bits.hdcp2x_gen_in7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Set
//  Description : Set the value of the member HDCP2X_GEN_IN8.hdcp2x_gen_in8
//  Input       : unsigned int uhdcp2x_gen_in8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Set(unsigned int uhdcp2x_gen_in8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN8 o_hdcp2x_gen_in8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN8.u32);
    o_hdcp2x_gen_in8.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in8.bits.hdcp2x_gen_in8 = uhdcp2x_gen_in8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in8.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in8.u32, "HDCP2X_GEN_IN8");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Get
//  Description : Set the value of the member HDCP2X_GEN_IN8.hdcp2x_gen_in8
//  Input       : unsigned int uhdcp2x_gen_in8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN8 o_hdcp2x_gen_in8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN8.u32);
    o_hdcp2x_gen_in8.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in8.bits.hdcp2x_gen_in8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Set
//  Description : Set the value of the member HDCP2X_GEN_IN9.hdcp2x_gen_in9
//  Input       : unsigned int uhdcp2x_gen_in9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Set(unsigned int uhdcp2x_gen_in9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN9 o_hdcp2x_gen_in9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN9.u32);
    o_hdcp2x_gen_in9.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in9.bits.hdcp2x_gen_in9 = uhdcp2x_gen_in9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in9.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in9.u32, "HDCP2X_GEN_IN9");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Get
//  Description : Set the value of the member HDCP2X_GEN_IN9.hdcp2x_gen_in9
//  Input       : unsigned int uhdcp2x_gen_in9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN9 o_hdcp2x_gen_in9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN9.u32);
    o_hdcp2x_gen_in9.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in9.bits.hdcp2x_gen_in9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Set
//  Description : Set the value of the member HDCP2X_GEN_IN10.hdcp2x_gen_in10
//  Input       : unsigned int uhdcp2x_gen_in10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Set(unsigned int uhdcp2x_gen_in10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN10 o_hdcp2x_gen_in10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN10.u32);
    o_hdcp2x_gen_in10.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in10.bits.hdcp2x_gen_in10 = uhdcp2x_gen_in10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in10.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in10.u32, "HDCP2X_GEN_IN10");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Get
//  Description : Set the value of the member HDCP2X_GEN_IN10.hdcp2x_gen_in10
//  Input       : unsigned int uhdcp2x_gen_in10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN10 o_hdcp2x_gen_in10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN10.u32);
    o_hdcp2x_gen_in10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in10.bits.hdcp2x_gen_in10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Set
//  Description : Set the value of the member HDCP2X_GEN_IN11.hdcp2x_gen_in11
//  Input       : unsigned int uhdcp2x_gen_in11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Set(unsigned int uhdcp2x_gen_in11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN11 o_hdcp2x_gen_in11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN11.u32);
    o_hdcp2x_gen_in11.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in11.bits.hdcp2x_gen_in11 = uhdcp2x_gen_in11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in11.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in11.u32, "HDCP2X_GEN_IN11");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Get
//  Description : Set the value of the member HDCP2X_GEN_IN11.hdcp2x_gen_in11
//  Input       : unsigned int uhdcp2x_gen_in11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN11 o_hdcp2x_gen_in11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN11.u32);
    o_hdcp2x_gen_in11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in11.bits.hdcp2x_gen_in11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Set
//  Description : Set the value of the member HDCP2X_GEN_IN12.hdcp2x_gen_in12
//  Input       : unsigned int uhdcp2x_gen_in12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Set(unsigned int uhdcp2x_gen_in12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN12 o_hdcp2x_gen_in12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN12.u32);
    o_hdcp2x_gen_in12.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in12.bits.hdcp2x_gen_in12 = uhdcp2x_gen_in12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in12.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in12.u32, "HDCP2X_GEN_IN12");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Get
//  Description : Set the value of the member HDCP2X_GEN_IN12.hdcp2x_gen_in12
//  Input       : unsigned int uhdcp2x_gen_in12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN12 o_hdcp2x_gen_in12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN12.u32);
    o_hdcp2x_gen_in12.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in12.bits.hdcp2x_gen_in12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Set
//  Description : Set the value of the member HDCP2X_GEN_IN13.hdcp2x_gen_in13
//  Input       : unsigned int uhdcp2x_gen_in13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Set(unsigned int uhdcp2x_gen_in13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN13 o_hdcp2x_gen_in13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN13.u32);
    o_hdcp2x_gen_in13.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in13.bits.hdcp2x_gen_in13 = uhdcp2x_gen_in13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in13.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in13.u32, "HDCP2X_GEN_IN13");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Get
//  Description : Set the value of the member HDCP2X_GEN_IN13.hdcp2x_gen_in13
//  Input       : unsigned int uhdcp2x_gen_in13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN13 o_hdcp2x_gen_in13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN13.u32);
    o_hdcp2x_gen_in13.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in13.bits.hdcp2x_gen_in13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Set
//  Description : Set the value of the member HDCP2X_GEN_IN14.hdcp2x_gen_in14
//  Input       : unsigned int uhdcp2x_gen_in14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Set(unsigned int uhdcp2x_gen_in14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN14 o_hdcp2x_gen_in14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN14.u32);
    o_hdcp2x_gen_in14.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in14.bits.hdcp2x_gen_in14 = uhdcp2x_gen_in14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in14.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in14.u32, "HDCP2X_GEN_IN14");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Get
//  Description : Set the value of the member HDCP2X_GEN_IN14.hdcp2x_gen_in14
//  Input       : unsigned int uhdcp2x_gen_in14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN14 o_hdcp2x_gen_in14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN14.u32);
    o_hdcp2x_gen_in14.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in14.bits.hdcp2x_gen_in14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Set
//  Description : Set the value of the member HDCP2X_GEN_IN15.hdcp2x_gen_in15
//  Input       : unsigned int uhdcp2x_gen_in15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Set(unsigned int uhdcp2x_gen_in15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN15 o_hdcp2x_gen_in15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN15.u32);
    o_hdcp2x_gen_in15.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in15.bits.hdcp2x_gen_in15 = uhdcp2x_gen_in15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in15.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in15.u32, "HDCP2X_GEN_IN15");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Get
//  Description : Set the value of the member HDCP2X_GEN_IN15.hdcp2x_gen_in15
//  Input       : unsigned int uhdcp2x_gen_in15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN15 o_hdcp2x_gen_in15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN15.u32);
    o_hdcp2x_gen_in15.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in15.bits.hdcp2x_gen_in15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Set
//  Description : Set the value of the member HDCP2X_GEN_IN16.hdcp2x_gen_in16
//  Input       : unsigned int uhdcp2x_gen_in16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Set(unsigned int uhdcp2x_gen_in16)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN16 o_hdcp2x_gen_in16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN16.u32);
    o_hdcp2x_gen_in16.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in16.bits.hdcp2x_gen_in16 = uhdcp2x_gen_in16;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in16.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in16.u32, "HDCP2X_GEN_IN16");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Get
//  Description : Set the value of the member HDCP2X_GEN_IN16.hdcp2x_gen_in16
//  Input       : unsigned int uhdcp2x_gen_in16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN16 o_hdcp2x_gen_in16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN16.u32);
    o_hdcp2x_gen_in16.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in16.bits.hdcp2x_gen_in16;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Set
//  Description : Set the value of the member HDCP2X_GEN_IN17.hdcp2x_gen_in17
//  Input       : unsigned int uhdcp2x_gen_in17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Set(unsigned int uhdcp2x_gen_in17)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN17 o_hdcp2x_gen_in17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN17.u32);
    o_hdcp2x_gen_in17.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in17.bits.hdcp2x_gen_in17 = uhdcp2x_gen_in17;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in17.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in17.u32, "HDCP2X_GEN_IN17");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Get
//  Description : Set the value of the member HDCP2X_GEN_IN17.hdcp2x_gen_in17
//  Input       : unsigned int uhdcp2x_gen_in17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN17 o_hdcp2x_gen_in17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN17.u32);
    o_hdcp2x_gen_in17.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in17.bits.hdcp2x_gen_in17;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Set
//  Description : Set the value of the member HDCP2X_GEN_IN18.hdcp2x_gen_in18
//  Input       : unsigned int uhdcp2x_gen_in18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Set(unsigned int uhdcp2x_gen_in18)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN18 o_hdcp2x_gen_in18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN18.u32);
    o_hdcp2x_gen_in18.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in18.bits.hdcp2x_gen_in18 = uhdcp2x_gen_in18;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in18.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in18.u32, "HDCP2X_GEN_IN18");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Get
//  Description : Set the value of the member HDCP2X_GEN_IN18.hdcp2x_gen_in18
//  Input       : unsigned int uhdcp2x_gen_in18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN18 o_hdcp2x_gen_in18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN18.u32);
    o_hdcp2x_gen_in18.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in18.bits.hdcp2x_gen_in18;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Set
//  Description : Set the value of the member HDCP2X_GEN_IN19.hdcp2x_gen_in19
//  Input       : unsigned int uhdcp2x_gen_in19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Set(unsigned int uhdcp2x_gen_in19)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN19 o_hdcp2x_gen_in19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN19.u32);
    o_hdcp2x_gen_in19.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in19.bits.hdcp2x_gen_in19 = uhdcp2x_gen_in19;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in19.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in19.u32, "HDCP2X_GEN_IN19");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Get
//  Description : Set the value of the member HDCP2X_GEN_IN19.hdcp2x_gen_in19
//  Input       : unsigned int uhdcp2x_gen_in19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN19 o_hdcp2x_gen_in19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN19.u32);
    o_hdcp2x_gen_in19.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in19.bits.hdcp2x_gen_in19;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Set
//  Description : Set the value of the member HDCP2X_GEN_IN20.hdcp2x_gen_in20
//  Input       : unsigned int uhdcp2x_gen_in20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Set(unsigned int uhdcp2x_gen_in20)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN20 o_hdcp2x_gen_in20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN20.u32);
    o_hdcp2x_gen_in20.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in20.bits.hdcp2x_gen_in20 = uhdcp2x_gen_in20;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in20.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in20.u32, "HDCP2X_GEN_IN20");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Get
//  Description : Set the value of the member HDCP2X_GEN_IN20.hdcp2x_gen_in20
//  Input       : unsigned int uhdcp2x_gen_in20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN20 o_hdcp2x_gen_in20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN20.u32);
    o_hdcp2x_gen_in20.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in20.bits.hdcp2x_gen_in20;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Set
//  Description : Set the value of the member HDCP2X_GEN_IN21.hdcp2x_gen_in21
//  Input       : unsigned int uhdcp2x_gen_in21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Set(unsigned int uhdcp2x_gen_in21)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN21 o_hdcp2x_gen_in21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN21.u32);
    o_hdcp2x_gen_in21.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in21.bits.hdcp2x_gen_in21 = uhdcp2x_gen_in21;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in21.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in21.u32, "HDCP2X_GEN_IN21");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Get
//  Description : Set the value of the member HDCP2X_GEN_IN21.hdcp2x_gen_in21
//  Input       : unsigned int uhdcp2x_gen_in21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN21 o_hdcp2x_gen_in21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN21.u32);
    o_hdcp2x_gen_in21.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in21.bits.hdcp2x_gen_in21;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Set
//  Description : Set the value of the member HDCP2X_GEN_IN22.hdcp2x_gen_in22
//  Input       : unsigned int uhdcp2x_gen_in22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Set(unsigned int uhdcp2x_gen_in22)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN22 o_hdcp2x_gen_in22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN22.u32);
    o_hdcp2x_gen_in22.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in22.bits.hdcp2x_gen_in22 = uhdcp2x_gen_in22;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in22.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in22.u32, "HDCP2X_GEN_IN22");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Get
//  Description : Set the value of the member HDCP2X_GEN_IN22.hdcp2x_gen_in22
//  Input       : unsigned int uhdcp2x_gen_in22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN22 o_hdcp2x_gen_in22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN22.u32);
    o_hdcp2x_gen_in22.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in22.bits.hdcp2x_gen_in22;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Set
//  Description : Set the value of the member HDCP2X_GEN_IN23.hdcp2x_gen_in23
//  Input       : unsigned int uhdcp2x_gen_in23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Set(unsigned int uhdcp2x_gen_in23)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN23 o_hdcp2x_gen_in23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN23.u32);
    o_hdcp2x_gen_in23.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in23.bits.hdcp2x_gen_in23 = uhdcp2x_gen_in23;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in23.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in23.u32, "HDCP2X_GEN_IN23");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Get
//  Description : Set the value of the member HDCP2X_GEN_IN23.hdcp2x_gen_in23
//  Input       : unsigned int uhdcp2x_gen_in23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN23 o_hdcp2x_gen_in23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN23.u32);
    o_hdcp2x_gen_in23.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in23.bits.hdcp2x_gen_in23;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Set
//  Description : Set the value of the member HDCP2X_GEN_IN24.hdcp2x_gen_in24
//  Input       : unsigned int uhdcp2x_gen_in24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Set(unsigned int uhdcp2x_gen_in24)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN24 o_hdcp2x_gen_in24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN24.u32);
    o_hdcp2x_gen_in24.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in24.bits.hdcp2x_gen_in24 = uhdcp2x_gen_in24;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in24.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in24.u32, "HDCP2X_GEN_IN24");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Get
//  Description : Set the value of the member HDCP2X_GEN_IN24.hdcp2x_gen_in24
//  Input       : unsigned int uhdcp2x_gen_in24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN24 o_hdcp2x_gen_in24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN24.u32);
    o_hdcp2x_gen_in24.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in24.bits.hdcp2x_gen_in24;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Set
//  Description : Set the value of the member HDCP2X_GEN_IN25.hdcp2x_gen_in25
//  Input       : unsigned int uhdcp2x_gen_in25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Set(unsigned int uhdcp2x_gen_in25)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN25 o_hdcp2x_gen_in25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN25.u32);
    o_hdcp2x_gen_in25.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in25.bits.hdcp2x_gen_in25 = uhdcp2x_gen_in25;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in25.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in25.u32, "HDCP2X_GEN_IN25");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Get
//  Description : Set the value of the member HDCP2X_GEN_IN25.hdcp2x_gen_in25
//  Input       : unsigned int uhdcp2x_gen_in25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN25 o_hdcp2x_gen_in25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN25.u32);
    o_hdcp2x_gen_in25.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in25.bits.hdcp2x_gen_in25;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Set
//  Description : Set the value of the member HDCP2X_GEN_IN26.hdcp2x_gen_in26
//  Input       : unsigned int uhdcp2x_gen_in26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Set(unsigned int uhdcp2x_gen_in26)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN26 o_hdcp2x_gen_in26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN26.u32);
    o_hdcp2x_gen_in26.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in26.bits.hdcp2x_gen_in26 = uhdcp2x_gen_in26;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in26.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in26.u32, "HDCP2X_GEN_IN26");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Get
//  Description : Set the value of the member HDCP2X_GEN_IN26.hdcp2x_gen_in26
//  Input       : unsigned int uhdcp2x_gen_in26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN26 o_hdcp2x_gen_in26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN26.u32);
    o_hdcp2x_gen_in26.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in26.bits.hdcp2x_gen_in26;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Set
//  Description : Set the value of the member HDCP2X_GEN_IN27.hdcp2x_gen_in27
//  Input       : unsigned int uhdcp2x_gen_in27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Set(unsigned int uhdcp2x_gen_in27)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN27 o_hdcp2x_gen_in27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN27.u32);
    o_hdcp2x_gen_in27.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in27.bits.hdcp2x_gen_in27 = uhdcp2x_gen_in27;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in27.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in27.u32, "HDCP2X_GEN_IN27");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Get
//  Description : Set the value of the member HDCP2X_GEN_IN27.hdcp2x_gen_in27
//  Input       : unsigned int uhdcp2x_gen_in27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN27 o_hdcp2x_gen_in27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN27.u32);
    o_hdcp2x_gen_in27.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in27.bits.hdcp2x_gen_in27;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Set
//  Description : Set the value of the member HDCP2X_GEN_IN28.hdcp2x_gen_in28
//  Input       : unsigned int uhdcp2x_gen_in28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Set(unsigned int uhdcp2x_gen_in28)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN28 o_hdcp2x_gen_in28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN28.u32);
    o_hdcp2x_gen_in28.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in28.bits.hdcp2x_gen_in28 = uhdcp2x_gen_in28;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in28.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in28.u32, "HDCP2X_GEN_IN28");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Get
//  Description : Set the value of the member HDCP2X_GEN_IN28.hdcp2x_gen_in28
//  Input       : unsigned int uhdcp2x_gen_in28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN28 o_hdcp2x_gen_in28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN28.u32);
    o_hdcp2x_gen_in28.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in28.bits.hdcp2x_gen_in28;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Set
//  Description : Set the value of the member HDCP2X_GEN_IN29.hdcp2x_gen_in29
//  Input       : unsigned int uhdcp2x_gen_in29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Set(unsigned int uhdcp2x_gen_in29)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN29 o_hdcp2x_gen_in29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN29.u32);
    o_hdcp2x_gen_in29.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in29.bits.hdcp2x_gen_in29 = uhdcp2x_gen_in29;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in29.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in29.u32, "HDCP2X_GEN_IN29");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Get
//  Description : Set the value of the member HDCP2X_GEN_IN29.hdcp2x_gen_in29
//  Input       : unsigned int uhdcp2x_gen_in29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN29 o_hdcp2x_gen_in29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN29.u32);
    o_hdcp2x_gen_in29.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in29.bits.hdcp2x_gen_in29;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Set
//  Description : Set the value of the member HDCP2X_GEN_IN30.hdcp2x_gen_in30
//  Input       : unsigned int uhdcp2x_gen_in30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Set(unsigned int uhdcp2x_gen_in30)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN30 o_hdcp2x_gen_in30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN30.u32);
    o_hdcp2x_gen_in30.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in30.bits.hdcp2x_gen_in30 = uhdcp2x_gen_in30;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in30.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in30.u32, "HDCP2X_GEN_IN30");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Get
//  Description : Set the value of the member HDCP2X_GEN_IN30.hdcp2x_gen_in30
//  Input       : unsigned int uhdcp2x_gen_in30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN30 o_hdcp2x_gen_in30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN30.u32);
    o_hdcp2x_gen_in30.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in30.bits.hdcp2x_gen_in30;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Set
//  Description : Set the value of the member HDCP2X_GEN_IN31.hdcp2x_gen_in31
//  Input       : unsigned int uhdcp2x_gen_in31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Set(unsigned int uhdcp2x_gen_in31)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN31 o_hdcp2x_gen_in31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN31.u32);
    o_hdcp2x_gen_in31.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_in31.bits.hdcp2x_gen_in31 = uhdcp2x_gen_in31;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_in31.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_in31.u32, "HDCP2X_GEN_IN31");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Get
//  Description : Set the value of the member HDCP2X_GEN_IN31.hdcp2x_gen_in31
//  Input       : unsigned int uhdcp2x_gen_in31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_IN31 o_hdcp2x_gen_in31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_IN31.u32);
    o_hdcp2x_gen_in31.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_in31.bits.hdcp2x_gen_in31;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Set
//  Description : Set the value of the member HDCP2X_GEN_OUT0.hdcp2x_gen_out0
//  Input       : unsigned int uhdcp2x_gen_out0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Set(unsigned int uhdcp2x_gen_out0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT0 o_hdcp2x_gen_out0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT0.u32);
    o_hdcp2x_gen_out0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out0.bits.hdcp2x_gen_out0 = uhdcp2x_gen_out0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out0.u32, "HDCP2X_GEN_OUT0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Get
//  Description : Set the value of the member HDCP2X_GEN_OUT0.hdcp2x_gen_out0
//  Input       : unsigned int uhdcp2x_gen_out0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT0 o_hdcp2x_gen_out0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT0.u32);
    o_hdcp2x_gen_out0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out0.bits.hdcp2x_gen_out0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Set
//  Description : Set the value of the member HDCP2X_GEN_OUT1.hdcp2x_gen_out1
//  Input       : unsigned int uhdcp2x_gen_out1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Set(unsigned int uhdcp2x_gen_out1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT1 o_hdcp2x_gen_out1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT1.u32);
    o_hdcp2x_gen_out1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out1.bits.hdcp2x_gen_out1 = uhdcp2x_gen_out1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out1.u32, "HDCP2X_GEN_OUT1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Get
//  Description : Set the value of the member HDCP2X_GEN_OUT1.hdcp2x_gen_out1
//  Input       : unsigned int uhdcp2x_gen_out1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT1 o_hdcp2x_gen_out1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT1.u32);
    o_hdcp2x_gen_out1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out1.bits.hdcp2x_gen_out1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Set
//  Description : Set the value of the member HDCP2X_GEN_OUT2.hdcp2x_gen_out2
//  Input       : unsigned int uhdcp2x_gen_out2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Set(unsigned int uhdcp2x_gen_out2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT2 o_hdcp2x_gen_out2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT2.u32);
    o_hdcp2x_gen_out2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out2.bits.hdcp2x_gen_out2 = uhdcp2x_gen_out2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out2.u32, "HDCP2X_GEN_OUT2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Get
//  Description : Set the value of the member HDCP2X_GEN_OUT2.hdcp2x_gen_out2
//  Input       : unsigned int uhdcp2x_gen_out2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT2 o_hdcp2x_gen_out2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT2.u32);
    o_hdcp2x_gen_out2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out2.bits.hdcp2x_gen_out2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Set
//  Description : Set the value of the member HDCP2X_GEN_OUT3.hdcp2x_gen_out3
//  Input       : unsigned int uhdcp2x_gen_out3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Set(unsigned int uhdcp2x_gen_out3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT3 o_hdcp2x_gen_out3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT3.u32);
    o_hdcp2x_gen_out3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out3.bits.hdcp2x_gen_out3 = uhdcp2x_gen_out3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out3.u32, "HDCP2X_GEN_OUT3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Get
//  Description : Set the value of the member HDCP2X_GEN_OUT3.hdcp2x_gen_out3
//  Input       : unsigned int uhdcp2x_gen_out3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT3 o_hdcp2x_gen_out3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT3.u32);
    o_hdcp2x_gen_out3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out3.bits.hdcp2x_gen_out3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Set
//  Description : Set the value of the member HDCP2X_GEN_OUT4.hdcp2x_gen_out4
//  Input       : unsigned int uhdcp2x_gen_out4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Set(unsigned int uhdcp2x_gen_out4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT4 o_hdcp2x_gen_out4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT4.u32);
    o_hdcp2x_gen_out4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out4.bits.hdcp2x_gen_out4 = uhdcp2x_gen_out4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out4.u32, "HDCP2X_GEN_OUT4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Get
//  Description : Set the value of the member HDCP2X_GEN_OUT4.hdcp2x_gen_out4
//  Input       : unsigned int uhdcp2x_gen_out4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT4 o_hdcp2x_gen_out4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT4.u32);
    o_hdcp2x_gen_out4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out4.bits.hdcp2x_gen_out4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Set
//  Description : Set the value of the member HDCP2X_GEN_OUT5.hdcp2x_gen_out5
//  Input       : unsigned int uhdcp2x_gen_out5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Set(unsigned int uhdcp2x_gen_out5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT5 o_hdcp2x_gen_out5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT5.u32);
    o_hdcp2x_gen_out5.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out5.bits.hdcp2x_gen_out5 = uhdcp2x_gen_out5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out5.u32, "HDCP2X_GEN_OUT5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Get
//  Description : Set the value of the member HDCP2X_GEN_OUT5.hdcp2x_gen_out5
//  Input       : unsigned int uhdcp2x_gen_out5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT5 o_hdcp2x_gen_out5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT5.u32);
    o_hdcp2x_gen_out5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out5.bits.hdcp2x_gen_out5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Set
//  Description : Set the value of the member HDCP2X_GEN_OUT6.hdcp2x_gen_out6
//  Input       : unsigned int uhdcp2x_gen_out6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Set(unsigned int uhdcp2x_gen_out6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT6 o_hdcp2x_gen_out6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT6.u32);
    o_hdcp2x_gen_out6.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out6.bits.hdcp2x_gen_out6 = uhdcp2x_gen_out6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out6.u32, "HDCP2X_GEN_OUT6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Get
//  Description : Set the value of the member HDCP2X_GEN_OUT6.hdcp2x_gen_out6
//  Input       : unsigned int uhdcp2x_gen_out6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT6 o_hdcp2x_gen_out6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT6.u32);
    o_hdcp2x_gen_out6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out6.bits.hdcp2x_gen_out6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Set
//  Description : Set the value of the member HDCP2X_GEN_OUT7.hdcp2x_gen_out7
//  Input       : unsigned int uhdcp2x_gen_out7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Set(unsigned int uhdcp2x_gen_out7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT7 o_hdcp2x_gen_out7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT7.u32);
    o_hdcp2x_gen_out7.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out7.bits.hdcp2x_gen_out7 = uhdcp2x_gen_out7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out7.u32, "HDCP2X_GEN_OUT7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Get
//  Description : Set the value of the member HDCP2X_GEN_OUT7.hdcp2x_gen_out7
//  Input       : unsigned int uhdcp2x_gen_out7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT7 o_hdcp2x_gen_out7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT7.u32);
    o_hdcp2x_gen_out7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out7.bits.hdcp2x_gen_out7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Set
//  Description : Set the value of the member HDCP2X_GEN_OUT8.hdcp2x_gen_out8
//  Input       : unsigned int uhdcp2x_gen_out8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Set(unsigned int uhdcp2x_gen_out8)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT8 o_hdcp2x_gen_out8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT8.u32);
    o_hdcp2x_gen_out8.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out8.bits.hdcp2x_gen_out8 = uhdcp2x_gen_out8;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out8.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out8.u32, "HDCP2X_GEN_OUT8");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Get
//  Description : Set the value of the member HDCP2X_GEN_OUT8.hdcp2x_gen_out8
//  Input       : unsigned int uhdcp2x_gen_out8: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT8 o_hdcp2x_gen_out8;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT8.u32);
    o_hdcp2x_gen_out8.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out8.bits.hdcp2x_gen_out8;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Set
//  Description : Set the value of the member HDCP2X_GEN_OUT9.hdcp2x_gen_out9
//  Input       : unsigned int uhdcp2x_gen_out9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Set(unsigned int uhdcp2x_gen_out9)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT9 o_hdcp2x_gen_out9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT9.u32);
    o_hdcp2x_gen_out9.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out9.bits.hdcp2x_gen_out9 = uhdcp2x_gen_out9;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out9.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out9.u32, "HDCP2X_GEN_OUT9");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Get
//  Description : Set the value of the member HDCP2X_GEN_OUT9.hdcp2x_gen_out9
//  Input       : unsigned int uhdcp2x_gen_out9: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT9 o_hdcp2x_gen_out9;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT9.u32);
    o_hdcp2x_gen_out9.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out9.bits.hdcp2x_gen_out9;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Set
//  Description : Set the value of the member HDCP2X_GEN_OUT10.hdcp2x_gen_out10
//  Input       : unsigned int uhdcp2x_gen_out10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Set(unsigned int uhdcp2x_gen_out10)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT10 o_hdcp2x_gen_out10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT10.u32);
    o_hdcp2x_gen_out10.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out10.bits.hdcp2x_gen_out10 = uhdcp2x_gen_out10;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out10.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out10.u32, "HDCP2X_GEN_OUT10");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Get
//  Description : Set the value of the member HDCP2X_GEN_OUT10.hdcp2x_gen_out10
//  Input       : unsigned int uhdcp2x_gen_out10: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT10 o_hdcp2x_gen_out10;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT10.u32);
    o_hdcp2x_gen_out10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out10.bits.hdcp2x_gen_out10;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Set
//  Description : Set the value of the member HDCP2X_GEN_OUT11.hdcp2x_gen_out11
//  Input       : unsigned int uhdcp2x_gen_out11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Set(unsigned int uhdcp2x_gen_out11)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT11 o_hdcp2x_gen_out11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT11.u32);
    o_hdcp2x_gen_out11.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out11.bits.hdcp2x_gen_out11 = uhdcp2x_gen_out11;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out11.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out11.u32, "HDCP2X_GEN_OUT11");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Get
//  Description : Set the value of the member HDCP2X_GEN_OUT11.hdcp2x_gen_out11
//  Input       : unsigned int uhdcp2x_gen_out11: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT11 o_hdcp2x_gen_out11;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT11.u32);
    o_hdcp2x_gen_out11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out11.bits.hdcp2x_gen_out11;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Set
//  Description : Set the value of the member HDCP2X_GEN_OUT12.hdcp2x_gen_out12
//  Input       : unsigned int uhdcp2x_gen_out12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Set(unsigned int uhdcp2x_gen_out12)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT12 o_hdcp2x_gen_out12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT12.u32);
    o_hdcp2x_gen_out12.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out12.bits.hdcp2x_gen_out12 = uhdcp2x_gen_out12;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out12.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out12.u32, "HDCP2X_GEN_OUT12");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Get
//  Description : Set the value of the member HDCP2X_GEN_OUT12.hdcp2x_gen_out12
//  Input       : unsigned int uhdcp2x_gen_out12: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT12 o_hdcp2x_gen_out12;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT12.u32);
    o_hdcp2x_gen_out12.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out12.bits.hdcp2x_gen_out12;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Set
//  Description : Set the value of the member HDCP2X_GEN_OUT13.hdcp2x_gen_out13
//  Input       : unsigned int uhdcp2x_gen_out13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Set(unsigned int uhdcp2x_gen_out13)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT13 o_hdcp2x_gen_out13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT13.u32);
    o_hdcp2x_gen_out13.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out13.bits.hdcp2x_gen_out13 = uhdcp2x_gen_out13;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out13.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out13.u32, "HDCP2X_GEN_OUT13");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Get
//  Description : Set the value of the member HDCP2X_GEN_OUT13.hdcp2x_gen_out13
//  Input       : unsigned int uhdcp2x_gen_out13: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT13 o_hdcp2x_gen_out13;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT13.u32);
    o_hdcp2x_gen_out13.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out13.bits.hdcp2x_gen_out13;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Set
//  Description : Set the value of the member HDCP2X_GEN_OUT14.hdcp2x_gen_out14
//  Input       : unsigned int uhdcp2x_gen_out14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Set(unsigned int uhdcp2x_gen_out14)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT14 o_hdcp2x_gen_out14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT14.u32);
    o_hdcp2x_gen_out14.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out14.bits.hdcp2x_gen_out14 = uhdcp2x_gen_out14;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out14.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out14.u32, "HDCP2X_GEN_OUT14");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Get
//  Description : Set the value of the member HDCP2X_GEN_OUT14.hdcp2x_gen_out14
//  Input       : unsigned int uhdcp2x_gen_out14: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT14 o_hdcp2x_gen_out14;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT14.u32);
    o_hdcp2x_gen_out14.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out14.bits.hdcp2x_gen_out14;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Set
//  Description : Set the value of the member HDCP2X_GEN_OUT15.hdcp2x_gen_out15
//  Input       : unsigned int uhdcp2x_gen_out15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Set(unsigned int uhdcp2x_gen_out15)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT15 o_hdcp2x_gen_out15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT15.u32);
    o_hdcp2x_gen_out15.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out15.bits.hdcp2x_gen_out15 = uhdcp2x_gen_out15;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out15.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out15.u32, "HDCP2X_GEN_OUT15");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Get
//  Description : Set the value of the member HDCP2X_GEN_OUT15.hdcp2x_gen_out15
//  Input       : unsigned int uhdcp2x_gen_out15: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT15 o_hdcp2x_gen_out15;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT15.u32);
    o_hdcp2x_gen_out15.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out15.bits.hdcp2x_gen_out15;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Set
//  Description : Set the value of the member HDCP2X_GEN_OUT16.hdcp2x_gen_out16
//  Input       : unsigned int uhdcp2x_gen_out16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Set(unsigned int uhdcp2x_gen_out16)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT16 o_hdcp2x_gen_out16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT16.u32);
    o_hdcp2x_gen_out16.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out16.bits.hdcp2x_gen_out16 = uhdcp2x_gen_out16;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out16.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out16.u32, "HDCP2X_GEN_OUT16");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Get
//  Description : Set the value of the member HDCP2X_GEN_OUT16.hdcp2x_gen_out16
//  Input       : unsigned int uhdcp2x_gen_out16: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT16 o_hdcp2x_gen_out16;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT16.u32);
    o_hdcp2x_gen_out16.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out16.bits.hdcp2x_gen_out16;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Set
//  Description : Set the value of the member HDCP2X_GEN_OUT17.hdcp2x_gen_out17
//  Input       : unsigned int uhdcp2x_gen_out17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Set(unsigned int uhdcp2x_gen_out17)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT17 o_hdcp2x_gen_out17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT17.u32);
    o_hdcp2x_gen_out17.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out17.bits.hdcp2x_gen_out17 = uhdcp2x_gen_out17;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out17.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out17.u32, "HDCP2X_GEN_OUT17");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Get
//  Description : Set the value of the member HDCP2X_GEN_OUT17.hdcp2x_gen_out17
//  Input       : unsigned int uhdcp2x_gen_out17: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT17 o_hdcp2x_gen_out17;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT17.u32);
    o_hdcp2x_gen_out17.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out17.bits.hdcp2x_gen_out17;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Set
//  Description : Set the value of the member HDCP2X_GEN_OUT18.hdcp2x_gen_out18
//  Input       : unsigned int uhdcp2x_gen_out18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Set(unsigned int uhdcp2x_gen_out18)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT18 o_hdcp2x_gen_out18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT18.u32);
    o_hdcp2x_gen_out18.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out18.bits.hdcp2x_gen_out18 = uhdcp2x_gen_out18;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out18.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out18.u32, "HDCP2X_GEN_OUT18");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Get
//  Description : Set the value of the member HDCP2X_GEN_OUT18.hdcp2x_gen_out18
//  Input       : unsigned int uhdcp2x_gen_out18: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT18 o_hdcp2x_gen_out18;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT18.u32);
    o_hdcp2x_gen_out18.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out18.bits.hdcp2x_gen_out18;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Set
//  Description : Set the value of the member HDCP2X_GEN_OUT19.hdcp2x_gen_out19
//  Input       : unsigned int uhdcp2x_gen_out19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Set(unsigned int uhdcp2x_gen_out19)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT19 o_hdcp2x_gen_out19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT19.u32);
    o_hdcp2x_gen_out19.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out19.bits.hdcp2x_gen_out19 = uhdcp2x_gen_out19;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out19.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out19.u32, "HDCP2X_GEN_OUT19");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Get
//  Description : Set the value of the member HDCP2X_GEN_OUT19.hdcp2x_gen_out19
//  Input       : unsigned int uhdcp2x_gen_out19: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT19 o_hdcp2x_gen_out19;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT19.u32);
    o_hdcp2x_gen_out19.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out19.bits.hdcp2x_gen_out19;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Set
//  Description : Set the value of the member HDCP2X_GEN_OUT20.hdcp2x_gen_out20
//  Input       : unsigned int uhdcp2x_gen_out20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Set(unsigned int uhdcp2x_gen_out20)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT20 o_hdcp2x_gen_out20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT20.u32);
    o_hdcp2x_gen_out20.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out20.bits.hdcp2x_gen_out20 = uhdcp2x_gen_out20;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out20.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out20.u32, "HDCP2X_GEN_OUT20");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Get
//  Description : Set the value of the member HDCP2X_GEN_OUT20.hdcp2x_gen_out20
//  Input       : unsigned int uhdcp2x_gen_out20: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT20 o_hdcp2x_gen_out20;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT20.u32);
    o_hdcp2x_gen_out20.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out20.bits.hdcp2x_gen_out20;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Set
//  Description : Set the value of the member HDCP2X_GEN_OUT21.hdcp2x_gen_out21
//  Input       : unsigned int uhdcp2x_gen_out21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Set(unsigned int uhdcp2x_gen_out21)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT21 o_hdcp2x_gen_out21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT21.u32);
    o_hdcp2x_gen_out21.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out21.bits.hdcp2x_gen_out21 = uhdcp2x_gen_out21;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out21.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out21.u32, "HDCP2X_GEN_OUT21");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Get
//  Description : Set the value of the member HDCP2X_GEN_OUT21.hdcp2x_gen_out21
//  Input       : unsigned int uhdcp2x_gen_out21: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT21 o_hdcp2x_gen_out21;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT21.u32);
    o_hdcp2x_gen_out21.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out21.bits.hdcp2x_gen_out21;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Set
//  Description : Set the value of the member HDCP2X_GEN_OUT22.hdcp2x_gen_out22
//  Input       : unsigned int uhdcp2x_gen_out22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Set(unsigned int uhdcp2x_gen_out22)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT22 o_hdcp2x_gen_out22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT22.u32);
    o_hdcp2x_gen_out22.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out22.bits.hdcp2x_gen_out22 = uhdcp2x_gen_out22;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out22.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out22.u32, "HDCP2X_GEN_OUT22");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Get
//  Description : Set the value of the member HDCP2X_GEN_OUT22.hdcp2x_gen_out22
//  Input       : unsigned int uhdcp2x_gen_out22: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT22 o_hdcp2x_gen_out22;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT22.u32);
    o_hdcp2x_gen_out22.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out22.bits.hdcp2x_gen_out22;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Set
//  Description : Set the value of the member HDCP2X_GEN_OUT23.hdcp2x_gen_out23
//  Input       : unsigned int uhdcp2x_gen_out23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Set(unsigned int uhdcp2x_gen_out23)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT23 o_hdcp2x_gen_out23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT23.u32);
    o_hdcp2x_gen_out23.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out23.bits.hdcp2x_gen_out23 = uhdcp2x_gen_out23;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out23.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out23.u32, "HDCP2X_GEN_OUT23");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Get
//  Description : Set the value of the member HDCP2X_GEN_OUT23.hdcp2x_gen_out23
//  Input       : unsigned int uhdcp2x_gen_out23: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT23 o_hdcp2x_gen_out23;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT23.u32);
    o_hdcp2x_gen_out23.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out23.bits.hdcp2x_gen_out23;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Set
//  Description : Set the value of the member HDCP2X_GEN_OUT24.hdcp2x_gen_out24
//  Input       : unsigned int uhdcp2x_gen_out24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Set(unsigned int uhdcp2x_gen_out24)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT24 o_hdcp2x_gen_out24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT24.u32);
    o_hdcp2x_gen_out24.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out24.bits.hdcp2x_gen_out24 = uhdcp2x_gen_out24;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out24.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out24.u32, "HDCP2X_GEN_OUT24");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Get
//  Description : Set the value of the member HDCP2X_GEN_OUT24.hdcp2x_gen_out24
//  Input       : unsigned int uhdcp2x_gen_out24: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT24 o_hdcp2x_gen_out24;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT24.u32);
    o_hdcp2x_gen_out24.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out24.bits.hdcp2x_gen_out24;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Set
//  Description : Set the value of the member HDCP2X_GEN_OUT25.hdcp2x_gen_out25
//  Input       : unsigned int uhdcp2x_gen_out25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Set(unsigned int uhdcp2x_gen_out25)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT25 o_hdcp2x_gen_out25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT25.u32);
    o_hdcp2x_gen_out25.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out25.bits.hdcp2x_gen_out25 = uhdcp2x_gen_out25;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out25.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out25.u32, "HDCP2X_GEN_OUT25");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Get
//  Description : Set the value of the member HDCP2X_GEN_OUT25.hdcp2x_gen_out25
//  Input       : unsigned int uhdcp2x_gen_out25: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT25 o_hdcp2x_gen_out25;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT25.u32);
    o_hdcp2x_gen_out25.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out25.bits.hdcp2x_gen_out25;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Set
//  Description : Set the value of the member HDCP2X_GEN_OUT26.hdcp2x_gen_out26
//  Input       : unsigned int uhdcp2x_gen_out26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Set(unsigned int uhdcp2x_gen_out26)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT26 o_hdcp2x_gen_out26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT26.u32);
    o_hdcp2x_gen_out26.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out26.bits.hdcp2x_gen_out26 = uhdcp2x_gen_out26;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out26.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out26.u32, "HDCP2X_GEN_OUT26");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Get
//  Description : Set the value of the member HDCP2X_GEN_OUT26.hdcp2x_gen_out26
//  Input       : unsigned int uhdcp2x_gen_out26: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT26 o_hdcp2x_gen_out26;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT26.u32);
    o_hdcp2x_gen_out26.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out26.bits.hdcp2x_gen_out26;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Set
//  Description : Set the value of the member HDCP2X_GEN_OUT27.hdcp2x_gen_out27
//  Input       : unsigned int uhdcp2x_gen_out27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Set(unsigned int uhdcp2x_gen_out27)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT27 o_hdcp2x_gen_out27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT27.u32);
    o_hdcp2x_gen_out27.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out27.bits.hdcp2x_gen_out27 = uhdcp2x_gen_out27;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out27.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out27.u32, "HDCP2X_GEN_OUT27");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Get
//  Description : Set the value of the member HDCP2X_GEN_OUT27.hdcp2x_gen_out27
//  Input       : unsigned int uhdcp2x_gen_out27: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT27 o_hdcp2x_gen_out27;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT27.u32);
    o_hdcp2x_gen_out27.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out27.bits.hdcp2x_gen_out27;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Set
//  Description : Set the value of the member HDCP2X_GEN_OUT28.hdcp2x_gen_out28
//  Input       : unsigned int uhdcp2x_gen_out28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Set(unsigned int uhdcp2x_gen_out28)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT28 o_hdcp2x_gen_out28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT28.u32);
    o_hdcp2x_gen_out28.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out28.bits.hdcp2x_gen_out28 = uhdcp2x_gen_out28;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out28.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out28.u32, "HDCP2X_GEN_OUT28");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Get
//  Description : Set the value of the member HDCP2X_GEN_OUT28.hdcp2x_gen_out28
//  Input       : unsigned int uhdcp2x_gen_out28: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT28 o_hdcp2x_gen_out28;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT28.u32);
    o_hdcp2x_gen_out28.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out28.bits.hdcp2x_gen_out28;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Set
//  Description : Set the value of the member HDCP2X_GEN_OUT29.hdcp2x_gen_out29
//  Input       : unsigned int uhdcp2x_gen_out29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Set(unsigned int uhdcp2x_gen_out29)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT29 o_hdcp2x_gen_out29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT29.u32);
    o_hdcp2x_gen_out29.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out29.bits.hdcp2x_gen_out29 = uhdcp2x_gen_out29;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out29.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out29.u32, "HDCP2X_GEN_OUT29");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Get
//  Description : Set the value of the member HDCP2X_GEN_OUT29.hdcp2x_gen_out29
//  Input       : unsigned int uhdcp2x_gen_out29: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT29 o_hdcp2x_gen_out29;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT29.u32);
    o_hdcp2x_gen_out29.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out29.bits.hdcp2x_gen_out29;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Set
//  Description : Set the value of the member HDCP2X_GEN_OUT30.hdcp2x_gen_out30
//  Input       : unsigned int uhdcp2x_gen_out30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Set(unsigned int uhdcp2x_gen_out30)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT30 o_hdcp2x_gen_out30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT30.u32);
    o_hdcp2x_gen_out30.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out30.bits.hdcp2x_gen_out30 = uhdcp2x_gen_out30;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out30.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out30.u32, "HDCP2X_GEN_OUT30");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Get
//  Description : Set the value of the member HDCP2X_GEN_OUT30.hdcp2x_gen_out30
//  Input       : unsigned int uhdcp2x_gen_out30: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT30 o_hdcp2x_gen_out30;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT30.u32);
    o_hdcp2x_gen_out30.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out30.bits.hdcp2x_gen_out30;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Set
//  Description : Set the value of the member HDCP2X_GEN_OUT31.hdcp2x_gen_out31
//  Input       : unsigned int uhdcp2x_gen_out31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Set(unsigned int uhdcp2x_gen_out31)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT31 o_hdcp2x_gen_out31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT31.u32);
    o_hdcp2x_gen_out31.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_gen_out31.bits.hdcp2x_gen_out31 = uhdcp2x_gen_out31;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_gen_out31.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_gen_out31.u32, "HDCP2X_GEN_OUT31");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Get
//  Description : Set the value of the member HDCP2X_GEN_OUT31.hdcp2x_gen_out31
//  Input       : unsigned int uhdcp2x_gen_out31: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_GEN_OUT31 o_hdcp2x_gen_out31;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_GEN_OUT31.u32);
    o_hdcp2x_gen_out31.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_gen_out31.bits.hdcp2x_gen_out31;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Set
//  Description : Set the value of the member HDCP2X_SEC_IN0.hdcp2x_sec_in0
//  Input       : unsigned int uhdcp2x_sec_in0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Set(unsigned int uhdcp2x_sec_in0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN0 o_hdcp2x_sec_in0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN0.u32);
    o_hdcp2x_sec_in0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in0.bits.hdcp2x_sec_in0 = uhdcp2x_sec_in0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in0.u32, "HDCP2X_SEC_IN0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Get
//  Description : Set the value of the member HDCP2X_SEC_IN0.hdcp2x_sec_in0
//  Input       : unsigned int uhdcp2x_sec_in0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN0 o_hdcp2x_sec_in0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN0.u32);
    o_hdcp2x_sec_in0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in0.bits.hdcp2x_sec_in0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Set
//  Description : Set the value of the member HDCP2X_SEC_IN1.hdcp2x_sec_in1
//  Input       : unsigned int uhdcp2x_sec_in1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Set(unsigned int uhdcp2x_sec_in1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN1 o_hdcp2x_sec_in1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN1.u32);
    o_hdcp2x_sec_in1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in1.bits.hdcp2x_sec_in1 = uhdcp2x_sec_in1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in1.u32, "HDCP2X_SEC_IN1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Get
//  Description : Set the value of the member HDCP2X_SEC_IN1.hdcp2x_sec_in1
//  Input       : unsigned int uhdcp2x_sec_in1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN1 o_hdcp2x_sec_in1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN1.u32);
    o_hdcp2x_sec_in1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in1.bits.hdcp2x_sec_in1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Set
//  Description : Set the value of the member HDCP2X_SEC_IN2.hdcp2x_sec_in2
//  Input       : unsigned int uhdcp2x_sec_in2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Set(unsigned int uhdcp2x_sec_in2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN2 o_hdcp2x_sec_in2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN2.u32);
    o_hdcp2x_sec_in2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in2.bits.hdcp2x_sec_in2 = uhdcp2x_sec_in2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in2.u32, "HDCP2X_SEC_IN2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Get
//  Description : Set the value of the member HDCP2X_SEC_IN2.hdcp2x_sec_in2
//  Input       : unsigned int uhdcp2x_sec_in2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN2 o_hdcp2x_sec_in2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN2.u32);
    o_hdcp2x_sec_in2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in2.bits.hdcp2x_sec_in2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Set
//  Description : Set the value of the member HDCP2X_SEC_IN3.hdcp2x_sec_in3
//  Input       : unsigned int uhdcp2x_sec_in3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Set(unsigned int uhdcp2x_sec_in3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN3 o_hdcp2x_sec_in3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN3.u32);
    o_hdcp2x_sec_in3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in3.bits.hdcp2x_sec_in3 = uhdcp2x_sec_in3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in3.u32, "HDCP2X_SEC_IN3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Get
//  Description : Set the value of the member HDCP2X_SEC_IN3.hdcp2x_sec_in3
//  Input       : unsigned int uhdcp2x_sec_in3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN3 o_hdcp2x_sec_in3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN3.u32);
    o_hdcp2x_sec_in3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in3.bits.hdcp2x_sec_in3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Set
//  Description : Set the value of the member HDCP2X_SEC_IN4.hdcp2x_sec_in4
//  Input       : unsigned int uhdcp2x_sec_in4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Set(unsigned int uhdcp2x_sec_in4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN4 o_hdcp2x_sec_in4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN4.u32);
    o_hdcp2x_sec_in4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in4.bits.hdcp2x_sec_in4 = uhdcp2x_sec_in4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in4.u32, "HDCP2X_SEC_IN4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Get
//  Description : Set the value of the member HDCP2X_SEC_IN4.hdcp2x_sec_in4
//  Input       : unsigned int uhdcp2x_sec_in4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN4 o_hdcp2x_sec_in4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN4.u32);
    o_hdcp2x_sec_in4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in4.bits.hdcp2x_sec_in4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Set
//  Description : Set the value of the member HDCP2X_SEC_IN5.hdcp2x_sec_in5
//  Input       : unsigned int uhdcp2x_sec_in5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Set(unsigned int uhdcp2x_sec_in5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN5 o_hdcp2x_sec_in5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN5.u32);
    o_hdcp2x_sec_in5.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in5.bits.hdcp2x_sec_in5 = uhdcp2x_sec_in5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in5.u32, "HDCP2X_SEC_IN5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Get
//  Description : Set the value of the member HDCP2X_SEC_IN5.hdcp2x_sec_in5
//  Input       : unsigned int uhdcp2x_sec_in5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN5 o_hdcp2x_sec_in5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN5.u32);
    o_hdcp2x_sec_in5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in5.bits.hdcp2x_sec_in5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Set
//  Description : Set the value of the member HDCP2X_SEC_IN6.hdcp2x_sec_in6
//  Input       : unsigned int uhdcp2x_sec_in6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Set(unsigned int uhdcp2x_sec_in6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN6 o_hdcp2x_sec_in6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN6.u32);
    o_hdcp2x_sec_in6.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in6.bits.hdcp2x_sec_in6 = uhdcp2x_sec_in6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in6.u32, "HDCP2X_SEC_IN6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Get
//  Description : Set the value of the member HDCP2X_SEC_IN6.hdcp2x_sec_in6
//  Input       : unsigned int uhdcp2x_sec_in6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN6 o_hdcp2x_sec_in6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN6.u32);
    o_hdcp2x_sec_in6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in6.bits.hdcp2x_sec_in6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Set
//  Description : Set the value of the member HDCP2X_SEC_IN7.hdcp2x_sec_in7
//  Input       : unsigned int uhdcp2x_sec_in7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Set(unsigned int uhdcp2x_sec_in7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN7 o_hdcp2x_sec_in7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN7.u32);
    o_hdcp2x_sec_in7.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_in7.bits.hdcp2x_sec_in7 = uhdcp2x_sec_in7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_in7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_in7.u32, "HDCP2X_SEC_IN7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Get
//  Description : Set the value of the member HDCP2X_SEC_IN7.hdcp2x_sec_in7
//  Input       : unsigned int uhdcp2x_sec_in7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_IN7 o_hdcp2x_sec_in7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_IN7.u32);
    o_hdcp2x_sec_in7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_in7.bits.hdcp2x_sec_in7;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Set
//  Description : Set the value of the member HDCP2X_SEC_OUT0.hdcp2x_sec_out0
//  Input       : unsigned int uhdcp2x_sec_out0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Set(unsigned int uhdcp2x_sec_out0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT0 o_hdcp2x_sec_out0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT0.u32);
    o_hdcp2x_sec_out0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out0.bits.hdcp2x_sec_out0 = uhdcp2x_sec_out0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out0.u32, "HDCP2X_SEC_OUT0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Get
//  Description : Set the value of the member HDCP2X_SEC_OUT0.hdcp2x_sec_out0
//  Input       : unsigned int uhdcp2x_sec_out0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT0 o_hdcp2x_sec_out0;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT0.u32);
    o_hdcp2x_sec_out0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out0.bits.hdcp2x_sec_out0;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Set
//  Description : Set the value of the member HDCP2X_SEC_OUT1.hdcp2x_sec_out1
//  Input       : unsigned int uhdcp2x_sec_out1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Set(unsigned int uhdcp2x_sec_out1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT1 o_hdcp2x_sec_out1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT1.u32);
    o_hdcp2x_sec_out1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out1.bits.hdcp2x_sec_out1 = uhdcp2x_sec_out1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out1.u32, "HDCP2X_SEC_OUT1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Get
//  Description : Set the value of the member HDCP2X_SEC_OUT1.hdcp2x_sec_out1
//  Input       : unsigned int uhdcp2x_sec_out1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT1 o_hdcp2x_sec_out1;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT1.u32);
    o_hdcp2x_sec_out1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out1.bits.hdcp2x_sec_out1;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Set
//  Description : Set the value of the member HDCP2X_SEC_OUT2.hdcp2x_sec_out2
//  Input       : unsigned int uhdcp2x_sec_out2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Set(unsigned int uhdcp2x_sec_out2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT2 o_hdcp2x_sec_out2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT2.u32);
    o_hdcp2x_sec_out2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out2.bits.hdcp2x_sec_out2 = uhdcp2x_sec_out2;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out2.u32, "HDCP2X_SEC_OUT2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Get
//  Description : Set the value of the member HDCP2X_SEC_OUT2.hdcp2x_sec_out2
//  Input       : unsigned int uhdcp2x_sec_out2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT2 o_hdcp2x_sec_out2;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT2.u32);
    o_hdcp2x_sec_out2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out2.bits.hdcp2x_sec_out2;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Set
//  Description : Set the value of the member HDCP2X_SEC_OUT3.hdcp2x_sec_out3
//  Input       : unsigned int uhdcp2x_sec_out3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Set(unsigned int uhdcp2x_sec_out3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT3 o_hdcp2x_sec_out3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT3.u32);
    o_hdcp2x_sec_out3.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out3.bits.hdcp2x_sec_out3 = uhdcp2x_sec_out3;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out3.u32, "HDCP2X_SEC_OUT3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Get
//  Description : Set the value of the member HDCP2X_SEC_OUT3.hdcp2x_sec_out3
//  Input       : unsigned int uhdcp2x_sec_out3: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT3 o_hdcp2x_sec_out3;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT3.u32);
    o_hdcp2x_sec_out3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out3.bits.hdcp2x_sec_out3;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Set
//  Description : Set the value of the member HDCP2X_SEC_OUT4.hdcp2x_sec_out4
//  Input       : unsigned int uhdcp2x_sec_out4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Set(unsigned int uhdcp2x_sec_out4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT4 o_hdcp2x_sec_out4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT4.u32);
    o_hdcp2x_sec_out4.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out4.bits.hdcp2x_sec_out4 = uhdcp2x_sec_out4;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out4.u32, "HDCP2X_SEC_OUT4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Get
//  Description : Set the value of the member HDCP2X_SEC_OUT4.hdcp2x_sec_out4
//  Input       : unsigned int uhdcp2x_sec_out4: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT4 o_hdcp2x_sec_out4;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT4.u32);
    o_hdcp2x_sec_out4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out4.bits.hdcp2x_sec_out4;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Set
//  Description : Set the value of the member HDCP2X_SEC_OUT5.hdcp2x_sec_out5
//  Input       : unsigned int uhdcp2x_sec_out5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Set(unsigned int uhdcp2x_sec_out5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT5 o_hdcp2x_sec_out5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT5.u32);
    o_hdcp2x_sec_out5.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out5.bits.hdcp2x_sec_out5 = uhdcp2x_sec_out5;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out5.u32, "HDCP2X_SEC_OUT5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Get
//  Description : Set the value of the member HDCP2X_SEC_OUT5.hdcp2x_sec_out5
//  Input       : unsigned int uhdcp2x_sec_out5: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT5 o_hdcp2x_sec_out5;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT5.u32);
    o_hdcp2x_sec_out5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out5.bits.hdcp2x_sec_out5;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Set
//  Description : Set the value of the member HDCP2X_SEC_OUT6.hdcp2x_sec_out6
//  Input       : unsigned int uhdcp2x_sec_out6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Set(unsigned int uhdcp2x_sec_out6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT6 o_hdcp2x_sec_out6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT6.u32);
    o_hdcp2x_sec_out6.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out6.bits.hdcp2x_sec_out6 = uhdcp2x_sec_out6;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out6.u32, "HDCP2X_SEC_OUT6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Get
//  Description : Set the value of the member HDCP2X_SEC_OUT6.hdcp2x_sec_out6
//  Input       : unsigned int uhdcp2x_sec_out6: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT6 o_hdcp2x_sec_out6;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT6.u32);
    o_hdcp2x_sec_out6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out6.bits.hdcp2x_sec_out6;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Set
//  Description : Set the value of the member HDCP2X_SEC_OUT7.hdcp2x_sec_out7
//  Input       : unsigned int uhdcp2x_sec_out7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Set(unsigned int uhdcp2x_sec_out7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT7 o_hdcp2x_sec_out7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT7.u32);
    o_hdcp2x_sec_out7.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp2x_sec_out7.bits.hdcp2x_sec_out7 = uhdcp2x_sec_out7;
    HDMITXRegWrite(pu32RegAddr, o_hdcp2x_sec_out7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp2x_sec_out7.u32, "HDCP2X_SEC_OUT7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Get
//  Description : Set the value of the member HDCP2X_SEC_OUT7.hdcp2x_sec_out7
//  Input       : unsigned int uhdcp2x_sec_out7: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HDCP2X_SEC_OUT7 o_hdcp2x_sec_out7;
    pu32RegAddr = (HI_U32*)&(gopmcu_cpu_regAllReg->HDCP2X_SEC_OUT7.u32);
    o_hdcp2x_sec_out7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp2x_sec_out7.bits.hdcp2x_sec_out7;
}

