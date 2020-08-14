// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  video_path_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00271848
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/06/15 16:25:01 Create file
// ******************************************************************************
#include "hi_type.h"
#include "hdmi_reg_video_path.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif

/* Define the struct pointor of the module video_path_reg */
volatile S_video_path_reg_REGS_TYPE *gopvideo_path_regAllReg = NULL;


int HDMI_TX_S_video_path_reg_REGS_TYPE_Init(void)
{
    if (gopvideo_path_regAllReg)
    {
        return HI_SUCCESS;
    }

    gopvideo_path_regAllReg = (volatile S_video_path_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_video_path_reg,4);

    if (!gopvideo_path_regAllReg)
    {
        HDMI_ERR("gopvideo_path_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit(void)
{
    if (gopvideo_path_regAllReg)
    {
        HDMI_IO_UNMAP(gopvideo_path_regAllReg);
        gopvideo_path_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_sync_polaritySet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_sync_polarity
//  Input       : unsigned int ureg_sync_polarity: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_sync_polaritySet(unsigned int ureg_sync_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_timing_gen_ctrl.bits.reg_sync_polarity = ureg_sync_polarity;
    HDMITXRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32, "TIMING_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_sync_polarityGet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_sync_polarity
//  Input       : unsigned int ureg_sync_polarity: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_sync_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_timing_gen_ctrl.bits.reg_sync_polarity;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_timing_selSet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_timing_sel
//  Input       : unsigned int ureg_timing_sel: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_timing_selSet(unsigned int ureg_timing_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_timing_gen_ctrl.bits.reg_timing_sel = ureg_timing_sel;
    HDMITXRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32, "TIMING_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_timing_selGet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_timing_sel
//  Input       : unsigned int ureg_timing_sel: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_timing_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_timing_gen_ctrl.bits.reg_timing_sel;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_extmodeSet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_extmode
//  Input       : unsigned int ureg_extmode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_extmodeSet(unsigned int ureg_extmode)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_timing_gen_ctrl.bits.reg_extmode = ureg_extmode;
    HDMITXRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32, "TIMING_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_extmodeGet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_extmode
//  Input       : unsigned int ureg_extmode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_extmodeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_timing_gen_ctrl.bits.reg_extmode;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_timing_gen_enSet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_timing_gen_en
//  Input       : unsigned int ureg_timing_gen_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_timing_gen_enSet(unsigned int ureg_timing_gen_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_timing_gen_ctrl.bits.reg_timing_gen_en = ureg_timing_gen_en;
    HDMITXRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_timing_gen_ctrl.u32, "TIMING_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TIMING_GEN_CTRL_reg_timing_gen_enGet
//  Description : Set the value of the member TIMING_GEN_CTRL.reg_timing_gen_en
//  Input       : unsigned int ureg_timing_gen_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TIMING_GEN_CTRL_reg_timing_gen_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_TIMING_GEN_CTRL o_timing_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TIMING_GEN_CTRL.u32);
    o_timing_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_timing_gen_ctrl.bits.reg_timing_gen_en;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countSet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG0.hsync_high_to_low_count
//  Input       : unsigned int uhsync_high_to_low_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countSet(unsigned int uhsync_high_to_low_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG0 o_hsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG0.u32);
    o_hsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hsync_timing_config0.bits.hsync_high_to_low_count = uhsync_high_to_low_count;
    HDMITXRegWrite(pu32RegAddr, o_hsync_timing_config0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hsync_timing_config0.u32, "HSYNC_TIMING_CONFIG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countGet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG0.hsync_high_to_low_count
//  Input       : unsigned int uhsync_high_to_low_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG0 o_hsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG0.u32);
    o_hsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hsync_timing_config0.bits.hsync_high_to_low_count;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countSet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG0.hsync_total_count
//  Input       : unsigned int uhsync_total_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countSet(unsigned int uhsync_total_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG0 o_hsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG0.u32);
    o_hsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hsync_timing_config0.bits.hsync_total_count = uhsync_total_count;
    HDMITXRegWrite(pu32RegAddr, o_hsync_timing_config0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hsync_timing_config0.u32, "HSYNC_TIMING_CONFIG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countGet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG0.hsync_total_count
//  Input       : unsigned int uhsync_total_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG0 o_hsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG0.u32);
    o_hsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hsync_timing_config0.bits.hsync_total_count;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countSet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG1.hsync_de_end_count
//  Input       : unsigned int uhsync_de_end_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countSet(unsigned int uhsync_de_end_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG1 o_hsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG1.u32);
    o_hsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hsync_timing_config1.bits.hsync_de_end_count = uhsync_de_end_count;
    HDMITXRegWrite(pu32RegAddr, o_hsync_timing_config1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hsync_timing_config1.u32, "HSYNC_TIMING_CONFIG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countGet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG1.hsync_de_end_count
//  Input       : unsigned int uhsync_de_end_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG1 o_hsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG1.u32);
    o_hsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hsync_timing_config1.bits.hsync_de_end_count;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countSet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG1.hsync_de_start_count
//  Input       : unsigned int uhsync_de_start_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countSet(unsigned int uhsync_de_start_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG1 o_hsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG1.u32);
    o_hsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hsync_timing_config1.bits.hsync_de_start_count = uhsync_de_start_count;
    HDMITXRegWrite(pu32RegAddr, o_hsync_timing_config1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hsync_timing_config1.u32, "HSYNC_TIMING_CONFIG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countGet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG1.hsync_de_start_count
//  Input       : unsigned int uhsync_de_start_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG1 o_hsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG1.u32);
    o_hsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hsync_timing_config1.bits.hsync_de_start_count;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countSet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG2.hsync_low_to_high_count
//  Input       : unsigned int uhsync_low_to_high_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countSet(unsigned int uhsync_low_to_high_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG2 o_hsync_timing_config2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG2.u32);
    o_hsync_timing_config2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hsync_timing_config2.bits.hsync_low_to_high_count = uhsync_low_to_high_count;
    HDMITXRegWrite(pu32RegAddr, o_hsync_timing_config2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hsync_timing_config2.u32, "HSYNC_TIMING_CONFIG2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countGet
//  Description : Set the value of the member HSYNC_TIMING_CONFIG2.hsync_low_to_high_count
//  Input       : unsigned int uhsync_low_to_high_count: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_HSYNC_TIMING_CONFIG2 o_hsync_timing_config2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->HSYNC_TIMING_CONFIG2.u32);
    o_hsync_timing_config2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hsync_timing_config2.bits.hsync_low_to_high_count;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countSet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG0.vsync_high_to_low_count
//  Input       : unsigned int uvsync_high_to_low_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countSet(unsigned int uvsync_high_to_low_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG0 o_vsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG0.u32);
    o_vsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsync_timing_config0.bits.vsync_high_to_low_count = uvsync_high_to_low_count;
    HDMITXRegWrite(pu32RegAddr, o_vsync_timing_config0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsync_timing_config0.u32, "VSYNC_TIMING_CONFIG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countGet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG0.vsync_high_to_low_count
//  Input       : unsigned int uvsync_high_to_low_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG0 o_vsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG0.u32);
    o_vsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vsync_timing_config0.bits.vsync_high_to_low_count;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countSet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG0.vsync_total_count
//  Input       : unsigned int uvsync_total_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countSet(unsigned int uvsync_total_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG0 o_vsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG0.u32);
    o_vsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsync_timing_config0.bits.vsync_total_count = uvsync_total_count;
    HDMITXRegWrite(pu32RegAddr, o_vsync_timing_config0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsync_timing_config0.u32, "VSYNC_TIMING_CONFIG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countGet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG0.vsync_total_count
//  Input       : unsigned int uvsync_total_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG0 o_vsync_timing_config0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG0.u32);
    o_vsync_timing_config0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vsync_timing_config0.bits.vsync_total_count;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countSet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG1.vsync_de_end_count
//  Input       : unsigned int uvsync_de_end_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countSet(unsigned int uvsync_de_end_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG1 o_vsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG1.u32);
    o_vsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsync_timing_config1.bits.vsync_de_end_count = uvsync_de_end_count;
    HDMITXRegWrite(pu32RegAddr, o_vsync_timing_config1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsync_timing_config1.u32, "VSYNC_TIMING_CONFIG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countGet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG1.vsync_de_end_count
//  Input       : unsigned int uvsync_de_end_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG1 o_vsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG1.u32);
    o_vsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vsync_timing_config1.bits.vsync_de_end_count;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countSet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG1.vsync_de_start_count
//  Input       : unsigned int uvsync_de_start_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countSet(unsigned int uvsync_de_start_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG1 o_vsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG1.u32);
    o_vsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsync_timing_config1.bits.vsync_de_start_count = uvsync_de_start_count;
    HDMITXRegWrite(pu32RegAddr, o_vsync_timing_config1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsync_timing_config1.u32, "VSYNC_TIMING_CONFIG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countGet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG1.vsync_de_start_count
//  Input       : unsigned int uvsync_de_start_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG1 o_vsync_timing_config1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG1.u32);
    o_vsync_timing_config1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vsync_timing_config1.bits.vsync_de_start_count;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countSet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG2.vsync_low_to_high_count
//  Input       : unsigned int uvsync_low_to_high_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countSet(unsigned int uvsync_low_to_high_count)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG2 o_vsync_timing_config2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG2.u32);
    o_vsync_timing_config2.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsync_timing_config2.bits.vsync_low_to_high_count = uvsync_low_to_high_count;
    HDMITXRegWrite(pu32RegAddr, o_vsync_timing_config2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsync_timing_config2.u32, "VSYNC_TIMING_CONFIG2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countGet
//  Description : Set the value of the member VSYNC_TIMING_CONFIG2.vsync_low_to_high_count
//  Input       : unsigned int uvsync_low_to_high_count: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VSYNC_TIMING_CONFIG2 o_vsync_timing_config2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VSYNC_TIMING_CONFIG2.u32);
    o_vsync_timing_config2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vsync_timing_config2.bits.vsync_low_to_high_count;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_increase_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.increase_en
//  Input       : unsigned int uincrease_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_increase_enSet(unsigned int uincrease_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.increase_en = uincrease_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_increase_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.increase_en
//  Input       : unsigned int uincrease_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_increase_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.increase_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_mix_color_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.mix_color_en
//  Input       : unsigned int umix_color_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_mix_color_enSet(unsigned int umix_color_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.mix_color_en = umix_color_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_mix_color_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.mix_color_en
//  Input       : unsigned int umix_color_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_mix_color_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.mix_color_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_cbar_pattern_selSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.cbar_pattern_sel
//  Input       : unsigned int ucbar_pattern_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_cbar_pattern_selSet(unsigned int ucbar_pattern_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.cbar_pattern_sel = ucbar_pattern_sel;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_cbar_pattern_selGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.cbar_pattern_sel
//  Input       : unsigned int ucbar_pattern_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_cbar_pattern_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.cbar_pattern_sel;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.bar_pattern_extmode
//  Input       : unsigned int ubar_pattern_extmode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeSet(unsigned int ubar_pattern_extmode)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.bar_pattern_extmode = ubar_pattern_extmode;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.bar_pattern_extmode
//  Input       : unsigned int ubar_pattern_extmode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.bar_pattern_extmode;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_replace_pattern_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.replace_pattern_en
//  Input       : unsigned int ureplace_pattern_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_replace_pattern_enSet(unsigned int ureplace_pattern_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.replace_pattern_en = ureplace_pattern_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_replace_pattern_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.replace_pattern_en
//  Input       : unsigned int ureplace_pattern_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_replace_pattern_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.replace_pattern_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_mask_pattern_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.mask_pattern_en
//  Input       : unsigned int umask_pattern_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_mask_pattern_enSet(unsigned int umask_pattern_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.mask_pattern_en = umask_pattern_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_mask_pattern_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.mask_pattern_en
//  Input       : unsigned int umask_pattern_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_mask_pattern_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.mask_pattern_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_square_pattern_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.square_pattern_en
//  Input       : unsigned int usquare_pattern_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_square_pattern_enSet(unsigned int usquare_pattern_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.square_pattern_en = usquare_pattern_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_square_pattern_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.square_pattern_en
//  Input       : unsigned int usquare_pattern_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_square_pattern_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.square_pattern_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_colorbar_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.colorbar_en
//  Input       : unsigned int ucolorbar_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_colorbar_enSet(unsigned int ucolorbar_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.colorbar_en = ucolorbar_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_colorbar_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.colorbar_en
//  Input       : unsigned int ucolorbar_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_colorbar_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.colorbar_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_solid_pattern_enSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.solid_pattern_en
//  Input       : unsigned int usolid_pattern_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_solid_pattern_enSet(unsigned int usolid_pattern_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.solid_pattern_en = usolid_pattern_en;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_solid_pattern_enGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.solid_pattern_en
//  Input       : unsigned int usolid_pattern_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_solid_pattern_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.solid_pattern_en;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_video_formatSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.video_format
//  Input       : unsigned int uvideo_format: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_video_formatSet(unsigned int uvideo_format)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.video_format = uvideo_format;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_video_formatGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.video_format
//  Input       : unsigned int uvideo_format: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_video_formatGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.video_format;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_tpg_enableSet
//  Description : Set the value of the member PATTERN_GEN_CTRL.tpg_enable
//  Input       : unsigned int utpg_enable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_tpg_enableSet(unsigned int utpg_enable)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pattern_gen_ctrl.bits.tpg_enable = utpg_enable;
    HDMITXRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pattern_gen_ctrl.u32, "PATTERN_GEN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PATTERN_GEN_CTRL_tpg_enableGet
//  Description : Set the value of the member PATTERN_GEN_CTRL.tpg_enable
//  Input       : unsigned int utpg_enable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PATTERN_GEN_CTRL_tpg_enableGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PATTERN_GEN_CTRL o_pattern_gen_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PATTERN_GEN_CTRL.u32);
    o_pattern_gen_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pattern_gen_ctrl.bits.tpg_enable;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crSet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_cr
//  Input       : unsigned int usolid_pattern_cr: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crSet(unsigned int usolid_pattern_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_solid_pattern_config.bits.solid_pattern_cr = usolid_pattern_cr;
    HDMITXRegWrite(pu32RegAddr, o_solid_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_solid_pattern_config.u32, "SOLID_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crGet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_cr
//  Input       : unsigned int usolid_pattern_cr: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_solid_pattern_config.bits.solid_pattern_cr;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_ySet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_y
//  Input       : unsigned int usolid_pattern_y: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_ySet(unsigned int usolid_pattern_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_solid_pattern_config.bits.solid_pattern_y = usolid_pattern_y;
    HDMITXRegWrite(pu32RegAddr, o_solid_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_solid_pattern_config.u32, "SOLID_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_yGet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_y
//  Input       : unsigned int usolid_pattern_y: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_solid_pattern_config.bits.solid_pattern_y;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbSet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_cb
//  Input       : unsigned int usolid_pattern_cb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbSet(unsigned int usolid_pattern_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_solid_pattern_config.bits.solid_pattern_cb = usolid_pattern_cb;
    HDMITXRegWrite(pu32RegAddr, o_solid_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_solid_pattern_config.u32, "SOLID_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbGet
//  Description : Set the value of the member SOLID_PATTERN_CONFIG.solid_pattern_cb
//  Input       : unsigned int usolid_pattern_cb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_SOLID_PATTERN_CONFIG o_solid_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->SOLID_PATTERN_CONFIG.u32);
    o_solid_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_solid_pattern_config.bits.solid_pattern_cb;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_crSet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_cr
//  Input       : unsigned int umask_pattern_cr: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_crSet(unsigned int umask_pattern_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_mask_pattern_config.bits.mask_pattern_cr = umask_pattern_cr;
    HDMITXRegWrite(pu32RegAddr, o_mask_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mask_pattern_config.u32, "MASK_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_crGet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_cr
//  Input       : unsigned int umask_pattern_cr: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_mask_pattern_config.bits.mask_pattern_cr;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_ySet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_y
//  Input       : unsigned int umask_pattern_y: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_ySet(unsigned int umask_pattern_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_mask_pattern_config.bits.mask_pattern_y = umask_pattern_y;
    HDMITXRegWrite(pu32RegAddr, o_mask_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mask_pattern_config.u32, "MASK_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_yGet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_y
//  Input       : unsigned int umask_pattern_y: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_mask_pattern_config.bits.mask_pattern_y;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbSet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_cb
//  Input       : unsigned int umask_pattern_cb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbSet(unsigned int umask_pattern_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_mask_pattern_config.bits.mask_pattern_cb = umask_pattern_cb;
    HDMITXRegWrite(pu32RegAddr, o_mask_pattern_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mask_pattern_config.u32, "MASK_PATTERN_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbGet
//  Description : Set the value of the member MASK_PATTERN_CONFIG.mask_pattern_cb
//  Input       : unsigned int umask_pattern_cb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MASK_PATTERN_CONFIG o_mask_pattern_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MASK_PATTERN_CONFIG.u32);
    o_mask_pattern_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_mask_pattern_config.bits.mask_pattern_cb;
}

//******************************************************************************
//  Function    : HDMI_BAR_EXT_CONFIG_square_heightSet
//  Description : Set the value of the member BAR_EXT_CONFIG.square_height
//  Input       : unsigned int usquare_height: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BAR_EXT_CONFIG_square_heightSet(unsigned int usquare_height)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BAR_EXT_CONFIG o_bar_ext_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BAR_EXT_CONFIG.u32);
    o_bar_ext_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_bar_ext_config.bits.square_height = usquare_height;
    HDMITXRegWrite(pu32RegAddr, o_bar_ext_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_bar_ext_config.u32, "BAR_EXT_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_BAR_EXT_CONFIG_square_heightGet
//  Description : Set the value of the member BAR_EXT_CONFIG.square_height
//  Input       : unsigned int usquare_height: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BAR_EXT_CONFIG_square_heightGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BAR_EXT_CONFIG o_bar_ext_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BAR_EXT_CONFIG.u32);
    o_bar_ext_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_bar_ext_config.bits.square_height;
}

//******************************************************************************
//  Function    : HDMI_BAR_EXT_CONFIG_colorbar_widthSet
//  Description : Set the value of the member BAR_EXT_CONFIG.colorbar_width
//  Input       : unsigned int ucolorbar_width: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BAR_EXT_CONFIG_colorbar_widthSet(unsigned int ucolorbar_width)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BAR_EXT_CONFIG o_bar_ext_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BAR_EXT_CONFIG.u32);
    o_bar_ext_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_bar_ext_config.bits.colorbar_width = ucolorbar_width;
    HDMITXRegWrite(pu32RegAddr, o_bar_ext_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_bar_ext_config.u32, "BAR_EXT_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_BAR_EXT_CONFIG_colorbar_widthGet
//  Description : Set the value of the member BAR_EXT_CONFIG.colorbar_width
//  Input       : unsigned int ucolorbar_width: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BAR_EXT_CONFIG_colorbar_widthGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BAR_EXT_CONFIG o_bar_ext_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BAR_EXT_CONFIG.u32);
    o_bar_ext_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_bar_ext_config.bits.colorbar_width;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A0_bar_pattern_a0Set
//  Description : Set the value of the member CBAR_PATTERN_A0.bar_pattern_a0
//  Input       : unsigned int ubar_pattern_a0: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A0_bar_pattern_a0Set(unsigned int ubar_pattern_a0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A0 o_cbar_pattern_a0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A0.u32);
    o_cbar_pattern_a0.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a0.bits.bar_pattern_a0 = ubar_pattern_a0;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a0.u32, "CBAR_PATTERN_A0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A0_bar_pattern_a0Get
//  Description : Set the value of the member CBAR_PATTERN_A0.bar_pattern_a0
//  Input       : unsigned int ubar_pattern_a0: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A0_bar_pattern_a0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A0 o_cbar_pattern_a0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A0.u32);
    o_cbar_pattern_a0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a0.bits.bar_pattern_a0;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A1_bar_pattern_a1Set
//  Description : Set the value of the member CBAR_PATTERN_A1.bar_pattern_a1
//  Input       : unsigned int ubar_pattern_a1: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A1_bar_pattern_a1Set(unsigned int ubar_pattern_a1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A1 o_cbar_pattern_a1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A1.u32);
    o_cbar_pattern_a1.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a1.bits.bar_pattern_a1 = ubar_pattern_a1;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a1.u32, "CBAR_PATTERN_A1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A1_bar_pattern_a1Get
//  Description : Set the value of the member CBAR_PATTERN_A1.bar_pattern_a1
//  Input       : unsigned int ubar_pattern_a1: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A1_bar_pattern_a1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A1 o_cbar_pattern_a1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A1.u32);
    o_cbar_pattern_a1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a1.bits.bar_pattern_a1;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A2_bar_pattern_a2Set
//  Description : Set the value of the member CBAR_PATTERN_A2.bar_pattern_a2
//  Input       : unsigned int ubar_pattern_a2: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A2_bar_pattern_a2Set(unsigned int ubar_pattern_a2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A2 o_cbar_pattern_a2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A2.u32);
    o_cbar_pattern_a2.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a2.bits.bar_pattern_a2 = ubar_pattern_a2;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a2.u32, "CBAR_PATTERN_A2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A2_bar_pattern_a2Get
//  Description : Set the value of the member CBAR_PATTERN_A2.bar_pattern_a2
//  Input       : unsigned int ubar_pattern_a2: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A2_bar_pattern_a2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A2 o_cbar_pattern_a2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A2.u32);
    o_cbar_pattern_a2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a2.bits.bar_pattern_a2;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A3_bar_pattern_a3Set
//  Description : Set the value of the member CBAR_PATTERN_A3.bar_pattern_a3
//  Input       : unsigned int ubar_pattern_a3: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A3_bar_pattern_a3Set(unsigned int ubar_pattern_a3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A3 o_cbar_pattern_a3;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A3.u32);
    o_cbar_pattern_a3.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a3.bits.bar_pattern_a3 = ubar_pattern_a3;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a3.u32, "CBAR_PATTERN_A3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A3_bar_pattern_a3Get
//  Description : Set the value of the member CBAR_PATTERN_A3.bar_pattern_a3
//  Input       : unsigned int ubar_pattern_a3: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A3_bar_pattern_a3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A3 o_cbar_pattern_a3;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A3.u32);
    o_cbar_pattern_a3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a3.bits.bar_pattern_a3;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A4_bar_pattern_a4Set
//  Description : Set the value of the member CBAR_PATTERN_A4.bar_pattern_a4
//  Input       : unsigned int ubar_pattern_a4: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A4_bar_pattern_a4Set(unsigned int ubar_pattern_a4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A4 o_cbar_pattern_a4;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A4.u32);
    o_cbar_pattern_a4.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a4.bits.bar_pattern_a4 = ubar_pattern_a4;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a4.u32, "CBAR_PATTERN_A4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A4_bar_pattern_a4Get
//  Description : Set the value of the member CBAR_PATTERN_A4.bar_pattern_a4
//  Input       : unsigned int ubar_pattern_a4: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A4_bar_pattern_a4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A4 o_cbar_pattern_a4;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A4.u32);
    o_cbar_pattern_a4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a4.bits.bar_pattern_a4;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A5_bar_pattern_a5Set
//  Description : Set the value of the member CBAR_PATTERN_A5.bar_pattern_a5
//  Input       : unsigned int ubar_pattern_a5: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A5_bar_pattern_a5Set(unsigned int ubar_pattern_a5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A5 o_cbar_pattern_a5;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A5.u32);
    o_cbar_pattern_a5.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a5.bits.bar_pattern_a5 = ubar_pattern_a5;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a5.u32, "CBAR_PATTERN_A5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A5_bar_pattern_a5Get
//  Description : Set the value of the member CBAR_PATTERN_A5.bar_pattern_a5
//  Input       : unsigned int ubar_pattern_a5: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A5_bar_pattern_a5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A5 o_cbar_pattern_a5;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A5.u32);
    o_cbar_pattern_a5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a5.bits.bar_pattern_a5;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A6_bar_pattern_a6Set
//  Description : Set the value of the member CBAR_PATTERN_A6.bar_pattern_a6
//  Input       : unsigned int ubar_pattern_a6: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A6_bar_pattern_a6Set(unsigned int ubar_pattern_a6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A6 o_cbar_pattern_a6;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A6.u32);
    o_cbar_pattern_a6.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a6.bits.bar_pattern_a6 = ubar_pattern_a6;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a6.u32, "CBAR_PATTERN_A6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A6_bar_pattern_a6Get
//  Description : Set the value of the member CBAR_PATTERN_A6.bar_pattern_a6
//  Input       : unsigned int ubar_pattern_a6: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A6_bar_pattern_a6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A6 o_cbar_pattern_a6;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A6.u32);
    o_cbar_pattern_a6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a6.bits.bar_pattern_a6;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A7_bar_pattern_a7Set
//  Description : Set the value of the member CBAR_PATTERN_A7.bar_pattern_a7
//  Input       : unsigned int ubar_pattern_a7: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A7_bar_pattern_a7Set(unsigned int ubar_pattern_a7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A7 o_cbar_pattern_a7;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A7.u32);
    o_cbar_pattern_a7.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_a7.bits.bar_pattern_a7 = ubar_pattern_a7;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_a7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_a7.u32, "CBAR_PATTERN_A7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_A7_bar_pattern_a7Get
//  Description : Set the value of the member CBAR_PATTERN_A7.bar_pattern_a7
//  Input       : unsigned int ubar_pattern_a7: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_A7_bar_pattern_a7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_A7 o_cbar_pattern_a7;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_A7.u32);
    o_cbar_pattern_a7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_a7.bits.bar_pattern_a7;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B0_bar_pattern_b0Set
//  Description : Set the value of the member CBAR_PATTERN_B0.bar_pattern_b0
//  Input       : unsigned int ubar_pattern_b0: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B0_bar_pattern_b0Set(unsigned int ubar_pattern_b0)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B0 o_cbar_pattern_b0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B0.u32);
    o_cbar_pattern_b0.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b0.bits.bar_pattern_b0 = ubar_pattern_b0;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b0.u32, "CBAR_PATTERN_B0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B0_bar_pattern_b0Get
//  Description : Set the value of the member CBAR_PATTERN_B0.bar_pattern_b0
//  Input       : unsigned int ubar_pattern_b0: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B0_bar_pattern_b0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B0 o_cbar_pattern_b0;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B0.u32);
    o_cbar_pattern_b0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b0.bits.bar_pattern_b0;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B1_bar_pattern_b1Set
//  Description : Set the value of the member CBAR_PATTERN_B1.bar_pattern_b1
//  Input       : unsigned int ubar_pattern_b1: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B1_bar_pattern_b1Set(unsigned int ubar_pattern_b1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B1 o_cbar_pattern_b1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B1.u32);
    o_cbar_pattern_b1.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b1.bits.bar_pattern_b1 = ubar_pattern_b1;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b1.u32, "CBAR_PATTERN_B1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B1_bar_pattern_b1Get
//  Description : Set the value of the member CBAR_PATTERN_B1.bar_pattern_b1
//  Input       : unsigned int ubar_pattern_b1: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B1_bar_pattern_b1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B1 o_cbar_pattern_b1;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B1.u32);
    o_cbar_pattern_b1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b1.bits.bar_pattern_b1;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B2_bar_pattern_b2Set
//  Description : Set the value of the member CBAR_PATTERN_B2.bar_pattern_b2
//  Input       : unsigned int ubar_pattern_b2: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B2_bar_pattern_b2Set(unsigned int ubar_pattern_b2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B2 o_cbar_pattern_b2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B2.u32);
    o_cbar_pattern_b2.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b2.bits.bar_pattern_b2 = ubar_pattern_b2;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b2.u32, "CBAR_PATTERN_B2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B2_bar_pattern_b2Get
//  Description : Set the value of the member CBAR_PATTERN_B2.bar_pattern_b2
//  Input       : unsigned int ubar_pattern_b2: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B2_bar_pattern_b2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B2 o_cbar_pattern_b2;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B2.u32);
    o_cbar_pattern_b2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b2.bits.bar_pattern_b2;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B3_bar_pattern_b3Set
//  Description : Set the value of the member CBAR_PATTERN_B3.bar_pattern_b3
//  Input       : unsigned int ubar_pattern_b3: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B3_bar_pattern_b3Set(unsigned int ubar_pattern_b3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B3 o_cbar_pattern_b3;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B3.u32);
    o_cbar_pattern_b3.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b3.bits.bar_pattern_b3 = ubar_pattern_b3;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b3.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b3.u32, "CBAR_PATTERN_B3");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B3_bar_pattern_b3Get
//  Description : Set the value of the member CBAR_PATTERN_B3.bar_pattern_b3
//  Input       : unsigned int ubar_pattern_b3: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B3_bar_pattern_b3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B3 o_cbar_pattern_b3;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B3.u32);
    o_cbar_pattern_b3.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b3.bits.bar_pattern_b3;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B4_bar_pattern_b4Set
//  Description : Set the value of the member CBAR_PATTERN_B4.bar_pattern_b4
//  Input       : unsigned int ubar_pattern_b4: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B4_bar_pattern_b4Set(unsigned int ubar_pattern_b4)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B4 o_cbar_pattern_b4;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B4.u32);
    o_cbar_pattern_b4.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b4.bits.bar_pattern_b4 = ubar_pattern_b4;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b4.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b4.u32, "CBAR_PATTERN_B4");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B4_bar_pattern_b4Get
//  Description : Set the value of the member CBAR_PATTERN_B4.bar_pattern_b4
//  Input       : unsigned int ubar_pattern_b4: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B4_bar_pattern_b4Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B4 o_cbar_pattern_b4;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B4.u32);
    o_cbar_pattern_b4.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b4.bits.bar_pattern_b4;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B5_bar_pattern_b5Set
//  Description : Set the value of the member CBAR_PATTERN_B5.bar_pattern_b5
//  Input       : unsigned int ubar_pattern_b5: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B5_bar_pattern_b5Set(unsigned int ubar_pattern_b5)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B5 o_cbar_pattern_b5;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B5.u32);
    o_cbar_pattern_b5.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b5.bits.bar_pattern_b5 = ubar_pattern_b5;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b5.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b5.u32, "CBAR_PATTERN_B5");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B5_bar_pattern_b5Get
//  Description : Set the value of the member CBAR_PATTERN_B5.bar_pattern_b5
//  Input       : unsigned int ubar_pattern_b5: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B5_bar_pattern_b5Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B5 o_cbar_pattern_b5;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B5.u32);
    o_cbar_pattern_b5.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b5.bits.bar_pattern_b5;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B6_bar_pattern_b6Set
//  Description : Set the value of the member CBAR_PATTERN_B6.bar_pattern_b6
//  Input       : unsigned int ubar_pattern_b6: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B6_bar_pattern_b6Set(unsigned int ubar_pattern_b6)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B6 o_cbar_pattern_b6;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B6.u32);
    o_cbar_pattern_b6.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b6.bits.bar_pattern_b6 = ubar_pattern_b6;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b6.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b6.u32, "CBAR_PATTERN_B6");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B6_bar_pattern_b6Get
//  Description : Set the value of the member CBAR_PATTERN_B6.bar_pattern_b6
//  Input       : unsigned int ubar_pattern_b6: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B6_bar_pattern_b6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B6 o_cbar_pattern_b6;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B6.u32);
    o_cbar_pattern_b6.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b6.bits.bar_pattern_b6;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B7_bar_pattern_b7Set
//  Description : Set the value of the member CBAR_PATTERN_B7.bar_pattern_b7
//  Input       : unsigned int ubar_pattern_b7: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B7_bar_pattern_b7Set(unsigned int ubar_pattern_b7)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B7 o_cbar_pattern_b7;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B7.u32);
    o_cbar_pattern_b7.u32 = HDMITXRegRead(pu32RegAddr);
    o_cbar_pattern_b7.bits.bar_pattern_b7 = ubar_pattern_b7;
    HDMITXRegWrite(pu32RegAddr, o_cbar_pattern_b7.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cbar_pattern_b7.u32, "CBAR_PATTERN_B7");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CBAR_PATTERN_B7_bar_pattern_b7Get
//  Description : Set the value of the member CBAR_PATTERN_B7.bar_pattern_b7
//  Input       : unsigned int ubar_pattern_b7: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CBAR_PATTERN_B7_bar_pattern_b7Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CBAR_PATTERN_B7 o_cbar_pattern_b7;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CBAR_PATTERN_B7.u32);
    o_cbar_pattern_b7.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cbar_pattern_b7.bits.bar_pattern_b7;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdSet
//  Description : Set the value of the member FORMAT_DET_CONFIG.pixel_cnt_threhold
//  Input       : unsigned int upixel_cnt_threhold: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdSet(unsigned int upixel_cnt_threhold)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_format_det_config.bits.pixel_cnt_threhold = upixel_cnt_threhold;
    HDMITXRegWrite(pu32RegAddr, o_format_det_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_format_det_config.u32, "FORMAT_DET_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdGet
//  Description : Set the value of the member FORMAT_DET_CONFIG.pixel_cnt_threhold
//  Input       : unsigned int upixel_cnt_threhold: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_format_det_config.bits.pixel_cnt_threhold;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_fdt_status_clearSet
//  Description : Set the value of the member FORMAT_DET_CONFIG.fdt_status_clear
//  Input       : unsigned int ufdt_status_clear: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_fdt_status_clearSet(unsigned int ufdt_status_clear)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_format_det_config.bits.fdt_status_clear = ufdt_status_clear;
    HDMITXRegWrite(pu32RegAddr, o_format_det_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_format_det_config.u32, "FORMAT_DET_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_fdt_status_clearGet
//  Description : Set the value of the member FORMAT_DET_CONFIG.fdt_status_clear
//  Input       : unsigned int ufdt_status_clear: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_fdt_status_clearGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_format_det_config.bits.fdt_status_clear;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueSet
//  Description : Set the value of the member FORMAT_DET_CONFIG.vsync_polarity_value
//  Input       : unsigned int uvsync_polarity_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueSet(unsigned int uvsync_polarity_value)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_format_det_config.bits.vsync_polarity_value = uvsync_polarity_value;
    HDMITXRegWrite(pu32RegAddr, o_format_det_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_format_det_config.u32, "FORMAT_DET_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueGet
//  Description : Set the value of the member FORMAT_DET_CONFIG.vsync_polarity_value
//  Input       : unsigned int uvsync_polarity_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_format_det_config.bits.vsync_polarity_value;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueSet
//  Description : Set the value of the member FORMAT_DET_CONFIG.hsync_polarity_value
//  Input       : unsigned int uhsync_polarity_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueSet(unsigned int uhsync_polarity_value)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_format_det_config.bits.hsync_polarity_value = uhsync_polarity_value;
    HDMITXRegWrite(pu32RegAddr, o_format_det_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_format_det_config.u32, "FORMAT_DET_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueGet
//  Description : Set the value of the member FORMAT_DET_CONFIG.hsync_polarity_value
//  Input       : unsigned int uhsync_polarity_value: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_format_det_config.bits.hsync_polarity_value;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_sync_polarity_forceSet
//  Description : Set the value of the member FORMAT_DET_CONFIG.sync_polarity_force
//  Input       : unsigned int usync_polarity_force: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_sync_polarity_forceSet(unsigned int usync_polarity_force)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_format_det_config.bits.sync_polarity_force = usync_polarity_force;
    HDMITXRegWrite(pu32RegAddr, o_format_det_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_format_det_config.u32, "FORMAT_DET_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FORMAT_DET_CONFIG_sync_polarity_forceGet
//  Description : Set the value of the member FORMAT_DET_CONFIG.sync_polarity_force
//  Input       : unsigned int usync_polarity_force: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FORMAT_DET_CONFIG_sync_polarity_forceGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FORMAT_DET_CONFIG o_format_det_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FORMAT_DET_CONFIG.u32);
    o_format_det_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_format_det_config.bits.sync_polarity_force;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_vsync_polaritySet
//  Description : Set the value of the member FDET_STATUS.vsync_polarity
//  Input       : unsigned int uvsync_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_vsync_polaritySet(unsigned int uvsync_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_status.bits.vsync_polarity = uvsync_polarity;
    HDMITXRegWrite(pu32RegAddr, o_fdet_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_status.u32, "FDET_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_vsync_polarityGet
//  Description : Set the value of the member FDET_STATUS.vsync_polarity
//  Input       : unsigned int uvsync_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_vsync_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_status.bits.vsync_polarity;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_hsync_polaritySet
//  Description : Set the value of the member FDET_STATUS.hsync_polarity
//  Input       : unsigned int uhsync_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_hsync_polaritySet(unsigned int uhsync_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_status.bits.hsync_polarity = uhsync_polarity;
    HDMITXRegWrite(pu32RegAddr, o_fdet_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_status.u32, "FDET_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_hsync_polarityGet
//  Description : Set the value of the member FDET_STATUS.hsync_polarity
//  Input       : unsigned int uhsync_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_hsync_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_status.bits.hsync_polarity;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_interlacedSet
//  Description : Set the value of the member FDET_STATUS.interlaced
//  Input       : unsigned int uinterlaced: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_interlacedSet(unsigned int uinterlaced)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_status.bits.interlaced = uinterlaced;
    HDMITXRegWrite(pu32RegAddr, o_fdet_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_status.u32, "FDET_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_STATUS_interlacedGet
//  Description : Set the value of the member FDET_STATUS.interlaced
//  Input       : unsigned int uinterlaced: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_STATUS_interlacedGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_STATUS o_fdet_status;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_STATUS.u32);
    o_fdet_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_status.bits.interlaced;
}

//******************************************************************************
//  Function    : HDMI_FDET_HORI_RES_hsync_total_cntSet
//  Description : Set the value of the member FDET_HORI_RES.hsync_total_cnt
//  Input       : unsigned int uhsync_total_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_HORI_RES_hsync_total_cntSet(unsigned int uhsync_total_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_HORI_RES o_fdet_hori_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_HORI_RES.u32);
    o_fdet_hori_res.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_hori_res.bits.hsync_total_cnt = uhsync_total_cnt;
    HDMITXRegWrite(pu32RegAddr, o_fdet_hori_res.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_hori_res.u32, "FDET_HORI_RES");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_HORI_RES_hsync_total_cntGet
//  Description : Set the value of the member FDET_HORI_RES.hsync_total_cnt
//  Input       : unsigned int uhsync_total_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_HORI_RES_hsync_total_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_HORI_RES o_fdet_hori_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_HORI_RES.u32);
    o_fdet_hori_res.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_hori_res.bits.hsync_total_cnt;
}

//******************************************************************************
//  Function    : HDMI_FDET_HORI_RES_hsync_active_cntSet
//  Description : Set the value of the member FDET_HORI_RES.hsync_active_cnt
//  Input       : unsigned int uhsync_active_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_HORI_RES_hsync_active_cntSet(unsigned int uhsync_active_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_HORI_RES o_fdet_hori_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_HORI_RES.u32);
    o_fdet_hori_res.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_hori_res.bits.hsync_active_cnt = uhsync_active_cnt;
    HDMITXRegWrite(pu32RegAddr, o_fdet_hori_res.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_hori_res.u32, "FDET_HORI_RES");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_HORI_RES_hsync_active_cntGet
//  Description : Set the value of the member FDET_HORI_RES.hsync_active_cnt
//  Input       : unsigned int uhsync_active_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_HORI_RES_hsync_active_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_HORI_RES o_fdet_hori_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_HORI_RES.u32);
    o_fdet_hori_res.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_hori_res.bits.hsync_active_cnt;
}

//******************************************************************************
//  Function    : HDMI_FDET_VERT_RES_vsync_total_cntSet
//  Description : Set the value of the member FDET_VERT_RES.vsync_total_cnt
//  Input       : unsigned int uvsync_total_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_VERT_RES_vsync_total_cntSet(unsigned int uvsync_total_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_VERT_RES o_fdet_vert_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_VERT_RES.u32);
    o_fdet_vert_res.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_vert_res.bits.vsync_total_cnt = uvsync_total_cnt;
    HDMITXRegWrite(pu32RegAddr, o_fdet_vert_res.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_vert_res.u32, "FDET_VERT_RES");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_VERT_RES_vsync_total_cntGet
//  Description : Set the value of the member FDET_VERT_RES.vsync_total_cnt
//  Input       : unsigned int uvsync_total_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_VERT_RES_vsync_total_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_VERT_RES o_fdet_vert_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_VERT_RES.u32);
    o_fdet_vert_res.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_vert_res.bits.vsync_total_cnt;
}

//******************************************************************************
//  Function    : HDMI_FDET_VERT_RES_vsync_active_cntSet
//  Description : Set the value of the member FDET_VERT_RES.vsync_active_cnt
//  Input       : unsigned int uvsync_active_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_VERT_RES_vsync_active_cntSet(unsigned int uvsync_active_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_VERT_RES o_fdet_vert_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_VERT_RES.u32);
    o_fdet_vert_res.u32 = HDMITXRegRead(pu32RegAddr);
    o_fdet_vert_res.bits.vsync_active_cnt = uvsync_active_cnt;
    HDMITXRegWrite(pu32RegAddr, o_fdet_vert_res.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_fdet_vert_res.u32, "FDET_VERT_RES");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_FDET_VERT_RES_vsync_active_cntGet
//  Description : Set the value of the member FDET_VERT_RES.vsync_active_cnt
//  Input       : unsigned int uvsync_active_cnt: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_FDET_VERT_RES_vsync_active_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_FDET_VERT_RES o_fdet_vert_res;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->FDET_VERT_RES.u32);
    o_fdet_vert_res.u32 = HDMITXRegRead(pu32RegAddr);
    return o_fdet_vert_res.bits.vsync_active_cnt;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_spatial_dualSet
//  Description : Set the value of the member DITHER_CONFIG.dither_spatial_dual
//  Input       : unsigned int udither_spatial_dual: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_spatial_dualSet(unsigned int udither_spatial_dual)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_spatial_dual = udither_spatial_dual;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_dither_config.u32, "DITHER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_spatial_dualGet
//  Description : Set the value of the member DITHER_CONFIG.dither_spatial_dual
//  Input       : unsigned int udither_spatial_dual: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_spatial_dualGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_dither_config.bits.dither_spatial_dual;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_spatial_enSet
//  Description : Set the value of the member DITHER_CONFIG.dither_spatial_en
//  Input       : unsigned int udither_spatial_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_spatial_enSet(unsigned int udither_spatial_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_spatial_en = udither_spatial_en;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_dither_config.u32, "DITHER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_spatial_enGet
//  Description : Set the value of the member DITHER_CONFIG.dither_spatial_en
//  Input       : unsigned int udither_spatial_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_spatial_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_dither_config.bits.dither_spatial_en;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_rnd_enSet
//  Description : Set the value of the member DITHER_CONFIG.dither_rnd_en
//  Input       : unsigned int udither_rnd_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_rnd_enSet(unsigned int udither_rnd_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_rnd_en = udither_rnd_en;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_dither_config.u32, "DITHER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_rnd_enGet
//  Description : Set the value of the member DITHER_CONFIG.dither_rnd_en
//  Input       : unsigned int udither_rnd_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_rnd_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_dither_config.bits.dither_rnd_en;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_modeSet
//  Description : Set the value of the member DITHER_CONFIG.dither_mode
//  Input       : unsigned int udither_mode: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_modeSet(unsigned int udither_mode)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_mode = udither_mode;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_dither_config.u32, "DITHER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_modeGet
//  Description : Set the value of the member DITHER_CONFIG.dither_mode
//  Input       : unsigned int udither_mode: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_dither_config.bits.dither_mode;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_rnd_bypSet
//  Description : Set the value of the member DITHER_CONFIG.dither_rnd_byp
//  Input       : unsigned int udither_rnd_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_rnd_bypSet(unsigned int udither_rnd_byp)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_rnd_byp = udither_rnd_byp;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_dither_config.u32, "DITHER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DITHER_CONFIG_dither_rnd_bypGet
//  Description : Set the value of the member DITHER_CONFIG.dither_rnd_byp
//  Input       : unsigned int udither_rnd_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DITHER_CONFIG_dither_rnd_bypGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_dither_config.bits.dither_rnd_byp;
}

//******************************************************************************
//  Function    : HDMI_CLIP_CTRL_clip_rgb_modeSet
//  Description : Set the value of the member CLIP_CTRL.clip_rgb_mode
//  Input       : unsigned int uclip_rgb_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_CTRL_clip_rgb_modeSet(unsigned int uclip_rgb_mode)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_CTRL o_clip_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_CTRL.u32);
    o_clip_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_ctrl.bits.clip_rgb_mode = uclip_rgb_mode;
    HDMITXRegWrite(pu32RegAddr, o_clip_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_ctrl.u32, "CLIP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_CTRL_clip_rgb_modeGet
//  Description : Set the value of the member CLIP_CTRL.clip_rgb_mode
//  Input       : unsigned int uclip_rgb_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_CTRL_clip_rgb_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_CTRL o_clip_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_CTRL.u32);
    o_clip_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_ctrl.bits.clip_rgb_mode;
}

//******************************************************************************
//  Function    : HDMI_CLIP_CTRL_range_clip_bypSet
//  Description : Set the value of the member CLIP_CTRL.range_clip_byp
//  Input       : unsigned int urange_clip_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_CTRL_range_clip_bypSet(unsigned int urange_clip_byp)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_CTRL o_clip_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_CTRL.u32);
    o_clip_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_ctrl.bits.range_clip_byp = urange_clip_byp;
    HDMITXRegWrite(pu32RegAddr, o_clip_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_ctrl.u32, "CLIP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_CTRL_range_clip_bypGet
//  Description : Set the value of the member CLIP_CTRL.range_clip_byp
//  Input       : unsigned int urange_clip_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_CTRL_range_clip_bypGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_CTRL o_clip_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_CTRL.u32);
    o_clip_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_ctrl.bits.range_clip_byp;
}

//******************************************************************************
//  Function    : HDMI_CLIP_Y_CONFIG_clip_y_maxSet
//  Description : Set the value of the member CLIP_Y_CONFIG.clip_y_max
//  Input       : unsigned int uclip_y_max: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_Y_CONFIG_clip_y_maxSet(unsigned int uclip_y_max)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_Y_CONFIG o_clip_y_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_Y_CONFIG.u32);
    o_clip_y_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_y_config.bits.clip_y_max = uclip_y_max;
    HDMITXRegWrite(pu32RegAddr, o_clip_y_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_y_config.u32, "CLIP_Y_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_Y_CONFIG_clip_y_maxGet
//  Description : Set the value of the member CLIP_Y_CONFIG.clip_y_max
//  Input       : unsigned int uclip_y_max: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_Y_CONFIG_clip_y_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_Y_CONFIG o_clip_y_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_Y_CONFIG.u32);
    o_clip_y_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_y_config.bits.clip_y_max;
}

//******************************************************************************
//  Function    : HDMI_CLIP_Y_CONFIG_clip_y_minSet
//  Description : Set the value of the member CLIP_Y_CONFIG.clip_y_min
//  Input       : unsigned int uclip_y_min: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_Y_CONFIG_clip_y_minSet(unsigned int uclip_y_min)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_Y_CONFIG o_clip_y_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_Y_CONFIG.u32);
    o_clip_y_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_y_config.bits.clip_y_min = uclip_y_min;
    HDMITXRegWrite(pu32RegAddr, o_clip_y_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_y_config.u32, "CLIP_Y_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_Y_CONFIG_clip_y_minGet
//  Description : Set the value of the member CLIP_Y_CONFIG.clip_y_min
//  Input       : unsigned int uclip_y_min: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_Y_CONFIG_clip_y_minGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_Y_CONFIG o_clip_y_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_Y_CONFIG.u32);
    o_clip_y_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_y_config.bits.clip_y_min;
}

//******************************************************************************
//  Function    : HDMI_CLIP_C_CONFIG_clip_c_maxSet
//  Description : Set the value of the member CLIP_C_CONFIG.clip_c_max
//  Input       : unsigned int uclip_c_max: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_C_CONFIG_clip_c_maxSet(unsigned int uclip_c_max)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_C_CONFIG o_clip_c_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_C_CONFIG.u32);
    o_clip_c_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_c_config.bits.clip_c_max = uclip_c_max;
    HDMITXRegWrite(pu32RegAddr, o_clip_c_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_c_config.u32, "CLIP_C_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_C_CONFIG_clip_c_maxGet
//  Description : Set the value of the member CLIP_C_CONFIG.clip_c_max
//  Input       : unsigned int uclip_c_max: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_C_CONFIG_clip_c_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_C_CONFIG o_clip_c_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_C_CONFIG.u32);
    o_clip_c_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_c_config.bits.clip_c_max;
}

//******************************************************************************
//  Function    : HDMI_CLIP_C_CONFIG_clip_c_minSet
//  Description : Set the value of the member CLIP_C_CONFIG.clip_c_min
//  Input       : unsigned int uclip_c_min: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_C_CONFIG_clip_c_minSet(unsigned int uclip_c_min)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_C_CONFIG o_clip_c_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_C_CONFIG.u32);
    o_clip_c_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_clip_c_config.bits.clip_c_min = uclip_c_min;
    HDMITXRegWrite(pu32RegAddr, o_clip_c_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_clip_c_config.u32, "CLIP_C_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CLIP_C_CONFIG_clip_c_minGet
//  Description : Set the value of the member CLIP_C_CONFIG.clip_c_min
//  Input       : unsigned int uclip_c_min: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CLIP_C_CONFIG_clip_c_minGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CLIP_C_CONFIG o_clip_c_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CLIP_C_CONFIG.u32);
    o_clip_c_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_clip_c_config.bits.clip_c_min;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_errorSet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_error
//  Input       : unsigned int ucap_stat_error: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_errorSet(unsigned int ucap_stat_error)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.cap_stat_error = ucap_stat_error;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_errorGet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_error
//  Input       : unsigned int ucap_stat_error: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_errorGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.cap_stat_error;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_busySet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_busy
//  Input       : unsigned int ucap_stat_busy: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_busySet(unsigned int ucap_stat_busy)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.cap_stat_busy = ucap_stat_busy;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_busyGet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_busy
//  Input       : unsigned int ucap_stat_busy: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_busyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.cap_stat_busy;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_doneSet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_done
//  Input       : unsigned int ucap_stat_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_doneSet(unsigned int ucap_stat_done)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.cap_stat_done = ucap_stat_done;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_cap_stat_doneGet
//  Description : Set the value of the member PXL_CAP_CTRL.cap_stat_done
//  Input       : unsigned int ucap_stat_done: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_cap_stat_doneGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.cap_stat_done;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_show_point_enSet
//  Description : Set the value of the member PXL_CAP_CTRL.show_point_en
//  Input       : unsigned int ushow_point_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_show_point_enSet(unsigned int ushow_point_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.show_point_en = ushow_point_en;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_show_point_enGet
//  Description : Set the value of the member PXL_CAP_CTRL.show_point_en
//  Input       : unsigned int ushow_point_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_show_point_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.show_point_en;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_soft_trigger_enSet
//  Description : Set the value of the member PXL_CAP_CTRL.soft_trigger_en
//  Input       : unsigned int usoft_trigger_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_soft_trigger_enSet(unsigned int usoft_trigger_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.soft_trigger_en = usoft_trigger_en;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_soft_trigger_enGet
//  Description : Set the value of the member PXL_CAP_CTRL.soft_trigger_en
//  Input       : unsigned int usoft_trigger_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_soft_trigger_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.soft_trigger_en;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_auto_trigger_enSet
//  Description : Set the value of the member PXL_CAP_CTRL.auto_trigger_en
//  Input       : unsigned int uauto_trigger_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_auto_trigger_enSet(unsigned int uauto_trigger_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_ctrl.bits.auto_trigger_en = uauto_trigger_en;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_ctrl.u32, "PXL_CAP_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_CTRL_auto_trigger_enGet
//  Description : Set the value of the member PXL_CAP_CTRL.auto_trigger_en
//  Input       : unsigned int uauto_trigger_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_CTRL_auto_trigger_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_CTRL o_pxl_cap_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_CTRL.u32);
    o_pxl_cap_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_ctrl.bits.auto_trigger_en;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_POSITION_cap_line_positionSet
//  Description : Set the value of the member PXL_CAP_POSITION.cap_line_position
//  Input       : unsigned int ucap_line_position: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_POSITION_cap_line_positionSet(unsigned int ucap_line_position)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_POSITION o_pxl_cap_position;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_POSITION.u32);
    o_pxl_cap_position.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_position.bits.cap_line_position = ucap_line_position;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_position.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_position.u32, "PXL_CAP_POSITION");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_POSITION_cap_line_positionGet
//  Description : Set the value of the member PXL_CAP_POSITION.cap_line_position
//  Input       : unsigned int ucap_line_position: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_POSITION_cap_line_positionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_POSITION o_pxl_cap_position;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_POSITION.u32);
    o_pxl_cap_position.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_position.bits.cap_line_position;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_POSITION_cap_pixel_positionSet
//  Description : Set the value of the member PXL_CAP_POSITION.cap_pixel_position
//  Input       : unsigned int ucap_pixel_position: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_POSITION_cap_pixel_positionSet(unsigned int ucap_pixel_position)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_POSITION o_pxl_cap_position;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_POSITION.u32);
    o_pxl_cap_position.u32 = HDMITXRegRead(pu32RegAddr);
    o_pxl_cap_position.bits.cap_pixel_position = ucap_pixel_position;
    HDMITXRegWrite(pu32RegAddr, o_pxl_cap_position.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pxl_cap_position.u32, "PXL_CAP_POSITION");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PXL_CAP_POSITION_cap_pixel_positionGet
//  Description : Set the value of the member PXL_CAP_POSITION.cap_pixel_position
//  Input       : unsigned int ucap_pixel_position: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PXL_CAP_POSITION_cap_pixel_positionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_PXL_CAP_POSITION o_pxl_cap_position;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->PXL_CAP_POSITION.u32);
    o_pxl_cap_position.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pxl_cap_position.bits.cap_pixel_position;
}

//******************************************************************************
//  Function    : HDMI_CAP_Y_VALUE_capture_y_valueSet
//  Description : Set the value of the member CAP_Y_VALUE.capture_y_value
//  Input       : unsigned int ucapture_y_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_Y_VALUE_capture_y_valueSet(unsigned int ucapture_y_value)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_Y_VALUE o_cap_y_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_Y_VALUE.u32);
    o_cap_y_value.u32 = HDMITXRegRead(pu32RegAddr);
    o_cap_y_value.bits.capture_y_value = ucapture_y_value;
    HDMITXRegWrite(pu32RegAddr, o_cap_y_value.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cap_y_value.u32, "CAP_Y_VALUE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CAP_Y_VALUE_capture_y_valueGet
//  Description : Set the value of the member CAP_Y_VALUE.capture_y_value
//  Input       : unsigned int ucapture_y_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_Y_VALUE_capture_y_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_Y_VALUE o_cap_y_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_Y_VALUE.u32);
    o_cap_y_value.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cap_y_value.bits.capture_y_value;
}

//******************************************************************************
//  Function    : HDMI_CAP_CB_VALUE_capture_cb_valueSet
//  Description : Set the value of the member CAP_CB_VALUE.capture_cb_value
//  Input       : unsigned int ucapture_cb_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_CB_VALUE_capture_cb_valueSet(unsigned int ucapture_cb_value)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_CB_VALUE o_cap_cb_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_CB_VALUE.u32);
    o_cap_cb_value.u32 = HDMITXRegRead(pu32RegAddr);
    o_cap_cb_value.bits.capture_cb_value = ucapture_cb_value;
    HDMITXRegWrite(pu32RegAddr, o_cap_cb_value.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cap_cb_value.u32, "CAP_CB_VALUE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CAP_CB_VALUE_capture_cb_valueGet
//  Description : Set the value of the member CAP_CB_VALUE.capture_cb_value
//  Input       : unsigned int ucapture_cb_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_CB_VALUE_capture_cb_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_CB_VALUE o_cap_cb_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_CB_VALUE.u32);
    o_cap_cb_value.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cap_cb_value.bits.capture_cb_value;
}

//******************************************************************************
//  Function    : HDMI_CAP_CR_VALUE_capture_cr_valueSet
//  Description : Set the value of the member CAP_CR_VALUE.capture_cr_value
//  Input       : unsigned int ucapture_cr_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_CR_VALUE_capture_cr_valueSet(unsigned int ucapture_cr_value)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_CR_VALUE o_cap_cr_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_CR_VALUE.u32);
    o_cap_cr_value.u32 = HDMITXRegRead(pu32RegAddr);
    o_cap_cr_value.bits.capture_cr_value = ucapture_cr_value;
    HDMITXRegWrite(pu32RegAddr, o_cap_cr_value.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cap_cr_value.u32, "CAP_CR_VALUE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CAP_CR_VALUE_capture_cr_valueGet
//  Description : Set the value of the member CAP_CR_VALUE.capture_cr_value
//  Input       : unsigned int ucapture_cr_value: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CAP_CR_VALUE_capture_cr_valueGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_CAP_CR_VALUE o_cap_cr_value;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->CAP_CR_VALUE.u32);
    o_cap_cr_value.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cap_cr_value.bits.capture_cr_value;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_modeSet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_mode
//  Input       : unsigned int ureg_csc_mode: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(unsigned int ureg_csc_mode)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_mode = ureg_csc_mode;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32, "MULTI_CSC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_modeGet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_mode
//  Input       : unsigned int ureg_csc_mode: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_mode;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_saturate_en
//  Input       : unsigned int ureg_csc_saturate_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(unsigned int ureg_csc_saturate_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_saturate_en = ureg_csc_saturate_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32, "MULTI_CSC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enGet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_saturate_en
//  Input       : unsigned int ureg_csc_saturate_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_saturate_en;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_dither_enSet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_dither_en
//  Input       : unsigned int ureg_csc_dither_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_dither_enSet(unsigned int ureg_csc_dither_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_dither_en = ureg_csc_dither_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32, "MULTI_CSC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_dither_enGet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_dither_en
//  Input       : unsigned int ureg_csc_dither_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_dither_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_dither_en;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_coef_extSet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_coef_ext
//  Input       : unsigned int ureg_csc_coef_ext: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_coef_extSet(unsigned int ureg_csc_coef_ext)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_coef_ext = ureg_csc_coef_ext;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32, "MULTI_CSC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_coef_extGet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_coef_ext
//  Input       : unsigned int ureg_csc_coef_ext: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_coef_extGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_coef_ext;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_enSet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_en
//  Input       : unsigned int ureg_csc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_enSet(unsigned int ureg_csc_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_en = ureg_csc_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32, "MULTI_CSC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_CTRL_reg_csc_enGet
//  Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_en
//  Input       : unsigned int ureg_csc_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_CTRL_reg_csc_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_en;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Set
//  Description : Set the value of the member MULTI_CSC_COEFF11.reg_csc_r1c1
//  Input       : unsigned int ureg_csc_r1c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Set(unsigned int ureg_csc_r1c1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF11 o_multi_csc_coeff11;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF11.u32);
    o_multi_csc_coeff11.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff11.bits.reg_csc_r1c1 = ureg_csc_r1c1;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff11.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff11.u32, "MULTI_CSC_COEFF11");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Get
//  Description : Set the value of the member MULTI_CSC_COEFF11.reg_csc_r1c1
//  Input       : unsigned int ureg_csc_r1c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF11 o_multi_csc_coeff11;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF11.u32);
    o_multi_csc_coeff11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff11.bits.reg_csc_r1c1;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Set
//  Description : Set the value of the member MULTI_CSC_COEFF12.reg_csc_r1c2
//  Input       : unsigned int ureg_csc_r1c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Set(unsigned int ureg_csc_r1c2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF12 o_multi_csc_coeff12;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF12.u32);
    o_multi_csc_coeff12.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff12.bits.reg_csc_r1c2 = ureg_csc_r1c2;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff12.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff12.u32, "MULTI_CSC_COEFF12");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Get
//  Description : Set the value of the member MULTI_CSC_COEFF12.reg_csc_r1c2
//  Input       : unsigned int ureg_csc_r1c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF12 o_multi_csc_coeff12;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF12.u32);
    o_multi_csc_coeff12.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff12.bits.reg_csc_r1c2;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Set
//  Description : Set the value of the member MULTI_CSC_COEFF13.reg_csc_r1c3
//  Input       : unsigned int ureg_csc_r1c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Set(unsigned int ureg_csc_r1c3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF13 o_multi_csc_coeff13;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF13.u32);
    o_multi_csc_coeff13.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff13.bits.reg_csc_r1c3 = ureg_csc_r1c3;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff13.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff13.u32, "MULTI_CSC_COEFF13");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Get
//  Description : Set the value of the member MULTI_CSC_COEFF13.reg_csc_r1c3
//  Input       : unsigned int ureg_csc_r1c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF13 o_multi_csc_coeff13;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF13.u32);
    o_multi_csc_coeff13.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff13.bits.reg_csc_r1c3;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Set
//  Description : Set the value of the member MULTI_CSC_COEFF21.reg_csc_r2c1
//  Input       : unsigned int ureg_csc_r2c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Set(unsigned int ureg_csc_r2c1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF21 o_multi_csc_coeff21;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF21.u32);
    o_multi_csc_coeff21.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff21.bits.reg_csc_r2c1 = ureg_csc_r2c1;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff21.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff21.u32, "MULTI_CSC_COEFF21");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Get
//  Description : Set the value of the member MULTI_CSC_COEFF21.reg_csc_r2c1
//  Input       : unsigned int ureg_csc_r2c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF21 o_multi_csc_coeff21;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF21.u32);
    o_multi_csc_coeff21.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff21.bits.reg_csc_r2c1;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Set
//  Description : Set the value of the member MULTI_CSC_COEFF22.reg_csc_r2c2
//  Input       : unsigned int ureg_csc_r2c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Set(unsigned int ureg_csc_r2c2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF22 o_multi_csc_coeff22;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF22.u32);
    o_multi_csc_coeff22.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff22.bits.reg_csc_r2c2 = ureg_csc_r2c2;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff22.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff22.u32, "MULTI_CSC_COEFF22");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Get
//  Description : Set the value of the member MULTI_CSC_COEFF22.reg_csc_r2c2
//  Input       : unsigned int ureg_csc_r2c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF22 o_multi_csc_coeff22;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF22.u32);
    o_multi_csc_coeff22.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff22.bits.reg_csc_r2c2;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Set
//  Description : Set the value of the member MULTI_CSC_COEFF23.reg_csc_r2c3
//  Input       : unsigned int ureg_csc_r2c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Set(unsigned int ureg_csc_r2c3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF23 o_multi_csc_coeff23;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF23.u32);
    o_multi_csc_coeff23.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff23.bits.reg_csc_r2c3 = ureg_csc_r2c3;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff23.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff23.u32, "MULTI_CSC_COEFF23");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Get
//  Description : Set the value of the member MULTI_CSC_COEFF23.reg_csc_r2c3
//  Input       : unsigned int ureg_csc_r2c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF23 o_multi_csc_coeff23;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF23.u32);
    o_multi_csc_coeff23.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff23.bits.reg_csc_r2c3;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Set
//  Description : Set the value of the member MULTI_CSC_COEFF31.reg_csc_r3c1
//  Input       : unsigned int ureg_csc_r3c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Set(unsigned int ureg_csc_r3c1)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF31 o_multi_csc_coeff31;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF31.u32);
    o_multi_csc_coeff31.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff31.bits.reg_csc_r3c1 = ureg_csc_r3c1;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff31.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff31.u32, "MULTI_CSC_COEFF31");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Get
//  Description : Set the value of the member MULTI_CSC_COEFF31.reg_csc_r3c1
//  Input       : unsigned int ureg_csc_r3c1: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF31 o_multi_csc_coeff31;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF31.u32);
    o_multi_csc_coeff31.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff31.bits.reg_csc_r3c1;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Set
//  Description : Set the value of the member MULTI_CSC_COEFF32.reg_csc_r3c2
//  Input       : unsigned int ureg_csc_r3c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Set(unsigned int ureg_csc_r3c2)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF32 o_multi_csc_coeff32;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF32.u32);
    o_multi_csc_coeff32.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff32.bits.reg_csc_r3c2 = ureg_csc_r3c2;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff32.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff32.u32, "MULTI_CSC_COEFF32");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Get
//  Description : Set the value of the member MULTI_CSC_COEFF32.reg_csc_r3c2
//  Input       : unsigned int ureg_csc_r3c2: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF32 o_multi_csc_coeff32;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF32.u32);
    o_multi_csc_coeff32.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff32.bits.reg_csc_r3c2;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Set
//  Description : Set the value of the member MULTI_CSC_COEFF33.reg_csc_r3c3
//  Input       : unsigned int ureg_csc_r3c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Set(unsigned int ureg_csc_r3c3)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF33 o_multi_csc_coeff33;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF33.u32);
    o_multi_csc_coeff33.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_coeff33.bits.reg_csc_r3c3 = ureg_csc_r3c3;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_coeff33.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_coeff33.u32, "MULTI_CSC_COEFF33");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Get
//  Description : Set the value of the member MULTI_CSC_COEFF33.reg_csc_r3c3
//  Input       : unsigned int ureg_csc_r3c3: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_COEFF33 o_multi_csc_coeff33;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_COEFF33.u32);
    o_multi_csc_coeff33.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_coeff33.bits.reg_csc_r3c3;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_ySet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_Y.reg_csc_inoffset_y
//  Input       : unsigned int ureg_csc_inoffset_y: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_ySet(unsigned int ureg_csc_inoffset_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_Y o_multi_csc_inoffset_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_Y.u32);
    o_multi_csc_inoffset_y.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_inoffset_y.bits.reg_csc_inoffset_y = ureg_csc_inoffset_y;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_inoffset_y.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_inoffset_y.u32, "MULTI_CSC_INOFFSET_Y");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_yGet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_Y.reg_csc_inoffset_y
//  Input       : unsigned int ureg_csc_inoffset_y: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_Y o_multi_csc_inoffset_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_Y.u32);
    o_multi_csc_inoffset_y.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_inoffset_y.bits.reg_csc_inoffset_y;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbSet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_CB.reg_csc_inoffset_cb
//  Input       : unsigned int ureg_csc_inoffset_cb: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbSet(unsigned int ureg_csc_inoffset_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_CB o_multi_csc_inoffset_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_CB.u32);
    o_multi_csc_inoffset_cb.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_inoffset_cb.bits.reg_csc_inoffset_cb = ureg_csc_inoffset_cb;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_inoffset_cb.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_inoffset_cb.u32, "MULTI_CSC_INOFFSET_CB");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbGet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_CB.reg_csc_inoffset_cb
//  Input       : unsigned int ureg_csc_inoffset_cb: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_CB o_multi_csc_inoffset_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_CB.u32);
    o_multi_csc_inoffset_cb.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_inoffset_cb.bits.reg_csc_inoffset_cb;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crSet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_CR.reg_csc_inoffset_cr
//  Input       : unsigned int ureg_csc_inoffset_cr: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crSet(unsigned int ureg_csc_inoffset_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_CR o_multi_csc_inoffset_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_CR.u32);
    o_multi_csc_inoffset_cr.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_inoffset_cr.bits.reg_csc_inoffset_cr = ureg_csc_inoffset_cr;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_inoffset_cr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_inoffset_cr.u32, "MULTI_CSC_INOFFSET_CR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crGet
//  Description : Set the value of the member MULTI_CSC_INOFFSET_CR.reg_csc_inoffset_cr
//  Input       : unsigned int ureg_csc_inoffset_cr: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_INOFFSET_CR o_multi_csc_inoffset_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_INOFFSET_CR.u32);
    o_multi_csc_inoffset_cr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_inoffset_cr.bits.reg_csc_inoffset_cr;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_ySet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_Y.reg_csc_outoffset_y
//  Input       : unsigned int ureg_csc_outoffset_y: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_ySet(unsigned int ureg_csc_outoffset_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_Y o_multi_csc_outoffset_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_Y.u32);
    o_multi_csc_outoffset_y.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_outoffset_y.bits.reg_csc_outoffset_y = ureg_csc_outoffset_y;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_outoffset_y.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_outoffset_y.u32, "MULTI_CSC_OUTOFFSET_Y");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_yGet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_Y.reg_csc_outoffset_y
//  Input       : unsigned int ureg_csc_outoffset_y: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_Y o_multi_csc_outoffset_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_Y.u32);
    o_multi_csc_outoffset_y.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_outoffset_y.bits.reg_csc_outoffset_y;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbSet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_CB.reg_csc_outoffset_cb
//  Input       : unsigned int ureg_csc_outoffset_cb: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbSet(unsigned int ureg_csc_outoffset_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_CB o_multi_csc_outoffset_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_CB.u32);
    o_multi_csc_outoffset_cb.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_outoffset_cb.bits.reg_csc_outoffset_cb = ureg_csc_outoffset_cb;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_outoffset_cb.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_outoffset_cb.u32, "MULTI_CSC_OUTOFFSET_CB");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbGet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_CB.reg_csc_outoffset_cb
//  Input       : unsigned int ureg_csc_outoffset_cb: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_CB o_multi_csc_outoffset_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_CB.u32);
    o_multi_csc_outoffset_cb.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_outoffset_cb.bits.reg_csc_outoffset_cb;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crSet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_CR.reg_csc_outoffset_cr
//  Input       : unsigned int ureg_csc_outoffset_cr: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crSet(unsigned int ureg_csc_outoffset_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_CR o_multi_csc_outoffset_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_CR.u32);
    o_multi_csc_outoffset_cr.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_outoffset_cr.bits.reg_csc_outoffset_cr = ureg_csc_outoffset_cr;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_outoffset_cr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_multi_csc_outoffset_cr.u32, "MULTI_CSC_OUTOFFSET_CR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crGet
//  Description : Set the value of the member MULTI_CSC_OUTOFFSET_CR.reg_csc_outoffset_cr
//  Input       : unsigned int ureg_csc_outoffset_cr: 13 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_MULTI_CSC_OUTOFFSET_CR o_multi_csc_outoffset_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_OUTOFFSET_CR.u32);
    o_multi_csc_outoffset_cr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_outoffset_cr.bits.reg_csc_outoffset_cr;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selSet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_vert_cbcr_sel
//  Input       : unsigned int ureg_vert_cbcr_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selSet(unsigned int ureg_vert_cbcr_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_vert_cbcr_sel = ureg_vert_cbcr_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32, "VIDEO_DWSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selGet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_vert_cbcr_sel
//  Input       : unsigned int ureg_vert_cbcr_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dwsm_ctrl.bits.reg_vert_cbcr_sel;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_vert_byp
//  Input       : unsigned int ureg_dwsm_vert_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet(unsigned int ureg_dwsm_vert_byp)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_vert_byp = ureg_dwsm_vert_byp;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32, "VIDEO_DWSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypGet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_vert_byp
//  Input       : unsigned int ureg_dwsm_vert_byp: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dwsm_ctrl.bits.reg_dwsm_vert_byp;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_vert_en
//  Input       : unsigned int ureg_dwsm_vert_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(unsigned int ureg_dwsm_vert_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_vert_en = ureg_dwsm_vert_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32, "VIDEO_DWSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enGet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_vert_en
//  Input       : unsigned int ureg_dwsm_vert_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dwsm_ctrl.bits.reg_dwsm_vert_en;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_hori_filter_en
//  Input       : unsigned int ureg_hori_filter_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(unsigned int ureg_hori_filter_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_hori_filter_en = ureg_hori_filter_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32, "VIDEO_DWSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enGet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_hori_filter_en
//  Input       : unsigned int ureg_hori_filter_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dwsm_ctrl.bits.reg_hori_filter_en;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_hori_en
//  Input       : unsigned int ureg_dwsm_hori_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(unsigned int ureg_dwsm_hori_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_hori_en = ureg_dwsm_hori_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32, "VIDEO_DWSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enGet
//  Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_hori_en
//  Input       : unsigned int ureg_dwsm_hori_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dwsm_ctrl.bits.reg_dwsm_hori_en;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_pxl_div_en
//  Input       : unsigned int ureg_pxl_div_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(unsigned int ureg_pxl_div_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_pxl_div_en = ureg_pxl_div_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_pxl_div_en
//  Input       : unsigned int ureg_pxl_div_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_pxl_div_en;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_demux_cb_or_cr
//  Input       : unsigned int ureg_demux_cb_or_cr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crSet(unsigned int ureg_demux_cb_or_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_demux_cb_or_cr = ureg_demux_cb_or_cr;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_demux_cb_or_cr
//  Input       : unsigned int ureg_demux_cb_or_cr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_demux_cb_or_cr;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_cbcr_order
//  Input       : unsigned int ureg_cbcr_order: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderSet(unsigned int ureg_cbcr_order)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_cbcr_order = ureg_cbcr_order;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_cbcr_order
//  Input       : unsigned int ureg_cbcr_order: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_cbcr_order;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polaritySet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_yc_mux_polarity
//  Input       : unsigned int ureg_yc_mux_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polaritySet(unsigned int ureg_yc_mux_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_yc_mux_polarity = ureg_yc_mux_polarity;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polarityGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_yc_mux_polarity
//  Input       : unsigned int ureg_yc_mux_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_yc_mux_polarity;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_ddr_polaritySet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_ddr_polarity
//  Input       : unsigned int ureg_ddr_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_ddr_polaritySet(unsigned int ureg_ddr_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_ddr_polarity = ureg_ddr_polarity;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_ddr_polarityGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_ddr_polarity
//  Input       : unsigned int ureg_ddr_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_ddr_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_ddr_polarity;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_pixel_rateSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_pixel_rate
//  Input       : unsigned int ureg_pixel_rate: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_pixel_rateSet(unsigned int ureg_pixel_rate)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_pixel_rate = ureg_pixel_rate;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_pixel_rateGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_pixel_rate
//  Input       : unsigned int ureg_pixel_rate: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_pixel_rateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_pixel_rate;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_blank_replace_en
//  Input       : unsigned int ureg_blank_replace_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enSet(unsigned int ureg_blank_replace_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_blank_replace_en = ureg_blank_replace_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_blank_replace_en
//  Input       : unsigned int ureg_blank_replace_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_blank_replace_en;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_yc_mux_en
//  Input       : unsigned int ureg_yc_mux_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enSet(unsigned int ureg_yc_mux_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_yc_mux_en = ureg_yc_mux_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_yc_mux_en
//  Input       : unsigned int ureg_yc_mux_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_yc_mux_en;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_ddr_enSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_ddr_en
//  Input       : unsigned int ureg_ddr_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_ddr_enSet(unsigned int ureg_ddr_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_ddr_en = ureg_ddr_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_ddr_enGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_ddr_en
//  Input       : unsigned int ureg_ddr_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_ddr_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_ddr_en;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_demux_420_en
//  Input       : unsigned int ureg_demux_420_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(unsigned int ureg_demux_420_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_demux_420_en = ureg_demux_420_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_data_align_ctrl.u32, "DATA_ALIGN_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DATA_ALIGN_CTRL_reg_demux_420_enGet
//  Description : Set the value of the member DATA_ALIGN_CTRL.reg_demux_420_en
//  Input       : unsigned int ureg_demux_420_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_data_align_ctrl.bits.reg_demux_420_en;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_Y_reg_blank_ySet
//  Description : Set the value of the member BLANK_DATA_Y.reg_blank_y
//  Input       : unsigned int ureg_blank_y: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_Y_reg_blank_ySet(unsigned int ureg_blank_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_Y o_blank_data_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_Y.u32);
    o_blank_data_y.u32 = HDMITXRegRead(pu32RegAddr);
    o_blank_data_y.bits.reg_blank_y = ureg_blank_y;
    HDMITXRegWrite(pu32RegAddr, o_blank_data_y.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_blank_data_y.u32, "BLANK_DATA_Y");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_Y_reg_blank_yGet
//  Description : Set the value of the member BLANK_DATA_Y.reg_blank_y
//  Input       : unsigned int ureg_blank_y: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_Y_reg_blank_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_Y o_blank_data_y;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_Y.u32);
    o_blank_data_y.u32 = HDMITXRegRead(pu32RegAddr);
    return o_blank_data_y.bits.reg_blank_y;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_CB_reg_blank_cbSet
//  Description : Set the value of the member BLANK_DATA_CB.reg_blank_cb
//  Input       : unsigned int ureg_blank_cb: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_CB_reg_blank_cbSet(unsigned int ureg_blank_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_CB o_blank_data_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_CB.u32);
    o_blank_data_cb.u32 = HDMITXRegRead(pu32RegAddr);
    o_blank_data_cb.bits.reg_blank_cb = ureg_blank_cb;
    HDMITXRegWrite(pu32RegAddr, o_blank_data_cb.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_blank_data_cb.u32, "BLANK_DATA_CB");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_CB_reg_blank_cbGet
//  Description : Set the value of the member BLANK_DATA_CB.reg_blank_cb
//  Input       : unsigned int ureg_blank_cb: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_CB_reg_blank_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_CB o_blank_data_cb;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_CB.u32);
    o_blank_data_cb.u32 = HDMITXRegRead(pu32RegAddr);
    return o_blank_data_cb.bits.reg_blank_cb;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_CR_reg_blank_crSet
//  Description : Set the value of the member BLANK_DATA_CR.reg_blank_cr
//  Input       : unsigned int ureg_blank_cr: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_CR_reg_blank_crSet(unsigned int ureg_blank_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_CR o_blank_data_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_CR.u32);
    o_blank_data_cr.u32 = HDMITXRegRead(pu32RegAddr);
    o_blank_data_cr.bits.reg_blank_cr = ureg_blank_cr;
    HDMITXRegWrite(pu32RegAddr, o_blank_data_cr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_blank_data_cr.u32, "BLANK_DATA_CR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_BLANK_DATA_CR_reg_blank_crGet
//  Description : Set the value of the member BLANK_DATA_CR.reg_blank_cr
//  Input       : unsigned int ureg_blank_cr: 12 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_BLANK_DATA_CR_reg_blank_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_BLANK_DATA_CR o_blank_data_cr;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->BLANK_DATA_CR.u32);
    o_blank_data_cr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_blank_data_cr.bits.reg_blank_cr;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_inver_sync
//  Input       : unsigned int ureg_inver_sync: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(unsigned int ureg_inver_sync)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_inver_sync = ureg_inver_sync;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_inver_syncGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_inver_sync
//  Input       : unsigned int ureg_inver_sync: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_inver_sync;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_syncmask_en
//  Input       : unsigned int ureg_syncmask_en: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(unsigned int ureg_syncmask_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_syncmask_en = ureg_syncmask_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_syncmask_en
//  Input       : unsigned int ureg_syncmask_en: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_syncmask_en;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_datamask_enSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_datamask_en
//  Input       : unsigned int ureg_datamask_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_datamask_enSet(unsigned int ureg_datamask_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_datamask_en = ureg_datamask_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_datamask_enGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_datamask_en
//  Input       : unsigned int ureg_datamask_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_datamask_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_datamask_en;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitrev_en
//  Input       : unsigned int ureg_bitrev_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enSet(unsigned int ureg_bitrev_en)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_bitrev_en = ureg_bitrev_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitrev_en
//  Input       : unsigned int ureg_bitrev_en: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_bitrev_en;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_cr
//  Input       : unsigned int ureg_bitmask_cr: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crSet(unsigned int ureg_bitmask_cr)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_bitmask_cr = ureg_bitmask_cr;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_cr
//  Input       : unsigned int ureg_bitmask_cr: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_bitmask_cr;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_cb
//  Input       : unsigned int ureg_bitmask_cb: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbSet(unsigned int ureg_bitmask_cb)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_bitmask_cb = ureg_bitmask_cb;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_cb
//  Input       : unsigned int ureg_bitmask_cb: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_bitmask_cb;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_ySet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_y
//  Input       : unsigned int ureg_bitmask_y: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_ySet(unsigned int ureg_bitmask_y)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_bitmask_y = ureg_bitmask_y;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_bitmask_yGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_bitmask_y
//  Input       : unsigned int ureg_bitmask_y: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_yGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_bitmask_y;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cr_sel
//  Input       : unsigned int ureg_vmux_cr_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(unsigned int ureg_vmux_cr_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cr_sel = ureg_vmux_cr_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cr_sel
//  Input       : unsigned int ureg_vmux_cr_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_vmux_cr_sel;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cb_sel
//  Input       : unsigned int ureg_vmux_cb_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(unsigned int ureg_vmux_cb_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cb_sel = ureg_vmux_cb_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cb_sel
//  Input       : unsigned int ureg_vmux_cb_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_vmux_cb_sel;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_y_sel
//  Input       : unsigned int ureg_vmux_y_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(unsigned int ureg_vmux_y_sel)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_y_sel = ureg_vmux_y_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32, "VIDEO_DMUX_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selGet
//  Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_y_sel
//  Input       : unsigned int ureg_vmux_y_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
     
    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_dmux_ctrl.bits.reg_vmux_y_sel;
}

