// ****************************************************************************** 
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co. Ltd.
// File name     :  hdmi2tx_dphy_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  xxx 2016/01/06 21:08:30 Create file
// ******************************************************************************

#include "hi_type.h"
#include "hdmi_reg_dphy.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif


/* Define the struct pointor of the module hdmi2tx_dphy_reg */
volatile S_hdmi2tx_dphy_reg_REGS_TYPE *gophdmi2tx_dphy_regAllReg = NULL;

HI_S32 HDMI_HDMITX_PHY_reg_Init(HI_VOID)
{
    if (NULL != gophdmi2tx_dphy_regAllReg)
    {
        return HI_SUCCESS;
    }
    
	gophdmi2tx_dphy_regAllReg  = (S_hdmi2tx_dphy_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_dphy, 4);

	if(NULL == gophdmi2tx_dphy_regAllReg)
	{
	    HDMI_ERR("ioremap_nocache gophdmi2tx_dphy_regAllReg failed!!!!!!!\n");
	    return HI_FAILURE;
	}
   
    return HI_SUCCESS;
}

HI_S32 HDMI_HDMITX_PHY_reg_DeInit(HI_VOID)
{
    if(HI_NULL != gophdmi2tx_dphy_regAllReg)
    {
        HDMI_IO_UNMAP(gophdmi2tx_dphy_regAllReg);    
        gophdmi2tx_dphy_regAllReg = HI_NULL;
    } 
    
    return HI_SUCCESS;

}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscin_bypass_enSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscin_bypass_en
//  Input       : HI_U32 ureg_sscin_bypass_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscin_bypass_enSet(HI_U32 ureg_sscin_bypass_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);

    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_sscin_bypass_en = ureg_sscin_bypass_en;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscin_bypass_enGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscin_bypass_en
//  Input       : HI_U32 ureg_sscin_bypass_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscin_bypass_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_sscin_bypass_en;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_pllfbmash111_enSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_pllfbmash111_en
//  Input       : HI_U32 ureg_pllfbmash111_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_pllfbmash111_enSet(HI_U32 ureg_pllfbmash111_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_pllfbmash111_en = ureg_pllfbmash111_en;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_pllfbmash111_enGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_pllfbmash111_en
//  Input       : HI_U32 ureg_pllfbmash111_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_pllfbmash111_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_pllfbmash111_en;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_ssc_modeSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_ssc_mode
//  Input       : HI_U32 ureg_ssc_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_ssc_modeSet(HI_U32 ureg_ssc_mode)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_ssc_mode = ureg_ssc_mode;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_ssc_modeGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_ssc_mode
//  Input       : HI_U32 ureg_ssc_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_ssc_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_ssc_mode;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_deep_colorSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_deep_color
//  Input       : HI_U32 ureg_deep_color: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_deep_colorSet(HI_U32 ureg_deep_color)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_deep_color = ureg_deep_color;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_deep_colorGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_deep_color
//  Input       : HI_U32 ureg_deep_color: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_deep_colorGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_deep_color;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscfifo_depth_clrSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscfifo_depth_clr
//  Input       : HI_U32 ureg_sscfifo_depth_clr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscfifo_depth_clrSet(HI_U32 ureg_sscfifo_depth_clr)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_sscfifo_depth_clr = ureg_sscfifo_depth_clr;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscfifo_depth_clrGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscfifo_depth_clr
//  Input       : HI_U32 ureg_sscfifo_depth_clr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscfifo_depth_clrGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_sscfifo_depth_clr;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscfifo_tune_enSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscfifo_tune_en
//  Input       : HI_U32 ureg_sscfifo_tune_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscfifo_tune_enSet(HI_U32 ureg_sscfifo_tune_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_sscfifo_tune_en = ureg_sscfifo_tune_en;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_sscfifo_tune_enGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_sscfifo_tune_en
//  Input       : HI_U32 ureg_sscfifo_tune_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_sscfifo_tune_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_sscfifo_tune_en;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_set_loadSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_set_load
//  Input       : HI_U32 ureg_set_load: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_set_loadSet(HI_U32 ureg_set_load)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_set_load = ureg_set_load;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_set_loadGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_set_load
//  Input       : HI_U32 ureg_set_load: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_set_loadGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_set_load;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_setSet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_set
//  Input       : HI_U32 ureg_set: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_setSet(HI_U32 ureg_set)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_set.bits.reg_set = ureg_set;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_set.u32, "HDMITX_INSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_SET_reg_setGet
//  Description : Set the value of the member HDMITX_INSSC_SET.reg_set
//  Input       : HI_U32 ureg_set: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_SET_reg_setGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_SET o_hdmitx_inssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_SET.u32);
    o_hdmitx_inssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_set.bits.reg_set;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_ssc_syncSet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_ssc_sync
//  Input       : HI_U32 ureg_ssc_sync: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_ssc_syncSet(HI_U32 ureg_ssc_sync)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_fifo.bits.reg_ssc_sync = ureg_ssc_sync;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32, "HDMITX_INSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_ssc_syncGet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_ssc_sync
//  Input       : HI_U32 ureg_ssc_sync: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_ssc_syncGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_fifo.bits.reg_ssc_sync;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_tune_reverseSet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_tune_reverse
//  Input       : HI_U32 ureg_tune_reverse: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_tune_reverseSet(HI_U32 ureg_tune_reverse)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_fifo.bits.reg_tune_reverse = ureg_tune_reverse;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32, "HDMITX_INSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_tune_reverseGet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_tune_reverse
//  Input       : HI_U32 ureg_tune_reverse: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_tune_reverseGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_fifo.bits.reg_tune_reverse;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_upper_threshSet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_sscfifo_upper_thresh
//  Input       : HI_U32 ureg_sscfifo_upper_thresh: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_upper_threshSet(HI_U32 ureg_sscfifo_upper_thresh)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_fifo.bits.reg_sscfifo_upper_thresh = ureg_sscfifo_upper_thresh;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32, "HDMITX_INSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_upper_threshGet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_sscfifo_upper_thresh
//  Input       : HI_U32 ureg_sscfifo_upper_thresh: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_upper_threshGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_fifo.bits.reg_sscfifo_upper_thresh;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_lower_threshSet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_sscfifo_lower_thresh
//  Input       : HI_U32 ureg_sscfifo_lower_thresh: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_lower_threshSet(HI_U32 ureg_sscfifo_lower_thresh)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_fifo.bits.reg_sscfifo_lower_thresh = ureg_sscfifo_lower_thresh;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32, "HDMITX_INSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_lower_threshGet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_sscfifo_lower_thresh
//  Input       : HI_U32 ureg_sscfifo_lower_thresh: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_sscfifo_lower_threshGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_fifo.bits.reg_sscfifo_lower_thresh;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_tune_stepSet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_tune_step
//  Input       : HI_U32 ureg_tune_step: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_tune_stepSet(HI_U32 ureg_tune_step)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_fifo.bits.reg_tune_step = ureg_tune_step;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_fifo.u32, "HDMITX_INSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_FIFO_reg_tune_stepGet
//  Description : Set the value of the member HDMITX_INSSC_FIFO.reg_tune_step
//  Input       : HI_U32 ureg_tune_step: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_FIFO_reg_tune_stepGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_FIFO o_hdmitx_inssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_FIFO.u32);
    o_hdmitx_inssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_fifo.bits.reg_tune_step;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_testdSet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_testd
//  Input       : HI_U32 ureg_testd: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_testdSet(HI_U32 ureg_testd)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_step.bits.reg_testd = ureg_testd;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32, "HDMITX_INSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_testdGet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_testd
//  Input       : HI_U32 ureg_testd: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_testdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_step.bits.reg_testd;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_spanSet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_span
//  Input       : HI_U32 ureg_span: 14 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_spanSet(HI_U32 ureg_span)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_step.bits.reg_span = ureg_span;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32, "HDMITX_INSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_spanGet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_span
//  Input       : HI_U32 ureg_span: 14 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_spanGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_step.bits.reg_span;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_stepSet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_step
//  Input       : HI_U32 ureg_step: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_stepSet(HI_U32 ureg_step)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_step.bits.reg_step = ureg_step;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_step.u32, "HDMITX_INSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_STEP_reg_stepGet
//  Description : Set the value of the member HDMITX_INSSC_STEP.reg_step
//  Input       : HI_U32 ureg_step: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_STEP_reg_stepGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_STEP o_hdmitx_inssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_STEP.u32);
    o_hdmitx_inssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_step.bits.reg_step;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_TUNE_reg_tune_intervalSet
//  Description : Set the value of the member HDMITX_INSSC_TUNE.reg_tune_interval
//  Input       : HI_U32 ureg_tune_interval: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_TUNE_reg_tune_intervalSet(HI_U32 ureg_tune_interval)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_TUNE o_hdmitx_inssc_tune;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_TUNE.u32);
    o_hdmitx_inssc_tune.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_inssc_tune.bits.reg_tune_interval = ureg_tune_interval;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_inssc_tune.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_inssc_tune.u32, "HDMITX_INSSC_TUNE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_INSSC_TUNE_reg_tune_intervalGet
//  Description : Set the value of the member HDMITX_INSSC_TUNE.reg_tune_interval
//  Input       : HI_U32 ureg_tune_interval: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_INSSC_TUNE_reg_tune_intervalGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_INSSC_TUNE o_hdmitx_inssc_tune;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_INSSC_TUNE.u32);
    o_hdmitx_inssc_tune.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_inssc_tune.bits.reg_tune_interval;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_mash111_hk_selSet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_mash111_hk_sel
//  Input       : HI_U32 ureg_mash111_hk_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_mash111_hk_selSet(HI_U32 ureg_mash111_hk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_ssc_hk.bits.reg_mash111_hk_sel = ureg_mash111_hk_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32, "HDMITX_SSC_HK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_mash111_hk_selGet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_mash111_hk_sel
//  Input       : HI_U32 ureg_mash111_hk_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_mash111_hk_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_ssc_hk.bits.reg_mash111_hk_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_hkmash_enSet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_hkmash_en
//  Input       : HI_U32 ureg_hkmash_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_hkmash_enSet(HI_U32 ureg_hkmash_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_ssc_hk.bits.reg_hkmash_en = ureg_hkmash_en;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32, "HDMITX_SSC_HK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_hkmash_enGet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_hkmash_en
//  Input       : HI_U32 ureg_hkmash_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_hkmash_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_ssc_hk.bits.reg_hkmash_en;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_dnumSet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_dnum
//  Input       : HI_U32 ureg_dnum: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_dnumSet(HI_U32 ureg_dnum)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_ssc_hk.bits.reg_dnum = ureg_dnum;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_ssc_hk.u32, "HDMITX_SSC_HK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSC_HK_reg_dnumGet
//  Description : Set the value of the member HDMITX_SSC_HK.reg_dnum
//  Input       : HI_U32 ureg_dnum: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSC_HK_reg_dnumGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSC_HK o_hdmitx_ssc_hk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSC_HK.u32);
    o_hdmitx_ssc_hk.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_ssc_hk.bits.reg_dnum;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_divSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_div
//  Input       : HI_U32 usscin_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_divSet(HI_U32 usscin_div)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status0.bits.sscin_div = usscin_div;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32, "HDMITX_SSCIN_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_divGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_div
//  Input       : HI_U32 usscin_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_divGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

   pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status0.bits.sscin_div;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_reverse_flagSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_reverse_flag
//  Input       : HI_U32 usscin_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_reverse_flagSet(HI_U32 usscin_reverse_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status0.bits.sscin_reverse_flag = usscin_reverse_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32, "HDMITX_SSCIN_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_reverse_flagGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_reverse_flag
//  Input       : HI_U32 usscin_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_reverse_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status0.bits.sscin_reverse_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_stop_flagSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_stop_flag
//  Input       : HI_U32 usscin_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_stop_flagSet(HI_U32 usscin_stop_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status0.bits.sscin_stop_flag = usscin_stop_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32, "HDMITX_SSCIN_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_stop_flagGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_stop_flag
//  Input       : HI_U32 usscin_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_stop_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status0.bits.sscin_stop_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_segmentSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_segment
//  Input       : HI_U32 usscin_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_segmentSet(HI_U32 usscin_segment)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status0.bits.sscin_segment = usscin_segment;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32, "HDMITX_SSCIN_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_segmentGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_segment
//  Input       : HI_U32 usscin_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_segmentGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status0.bits.sscin_segment;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_span_cntSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_span_cnt
//  Input       : HI_U32 usscin_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_span_cntSet(HI_U32 usscin_span_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status0.bits.sscin_span_cnt = usscin_span_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status0.u32, "HDMITX_SSCIN_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS0_sscin_span_cntGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS0.sscin_span_cnt
//  Input       : HI_U32 usscin_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS0_sscin_span_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS0 o_hdmitx_sscin_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS0.u32);
    o_hdmitx_sscin_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status0.bits.sscin_span_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS1_sscin_ssc_offsetSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS1.sscin_ssc_offset
//  Input       : HI_U32 usscin_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS1_sscin_ssc_offsetSet(HI_U32 usscin_ssc_offset)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS1 o_hdmitx_sscin_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS1.u32);
    o_hdmitx_sscin_status1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status1.bits.sscin_ssc_offset = usscin_ssc_offset;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status1.u32, "HDMITX_SSCIN_STATUS1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS1_sscin_ssc_offsetGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS1.sscin_ssc_offset
//  Input       : HI_U32 usscin_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS1_sscin_ssc_offsetGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS1 o_hdmitx_sscin_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS1.u32);
    o_hdmitx_sscin_status1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status1.bits.sscin_ssc_offset;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_minSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS2.sscin_sscfifo_depth_min
//  Input       : HI_U32 usscin_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_minSet(HI_U32 usscin_sscfifo_depth_min)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS2 o_hdmitx_sscin_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS2.u32);
    o_hdmitx_sscin_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status2.bits.sscin_sscfifo_depth_min = usscin_sscfifo_depth_min;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status2.u32, "HDMITX_SSCIN_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_minGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS2.sscin_sscfifo_depth_min
//  Input       : HI_U32 usscin_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_minGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS2 o_hdmitx_sscin_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS2.u32);
    o_hdmitx_sscin_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status2.bits.sscin_sscfifo_depth_min;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_maxSet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS2.sscin_sscfifo_depth_max
//  Input       : HI_U32 usscin_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_maxSet(HI_U32 usscin_sscfifo_depth_max)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS2 o_hdmitx_sscin_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS2.u32);
    o_hdmitx_sscin_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscin_status2.bits.sscin_sscfifo_depth_max = usscin_sscfifo_depth_max;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscin_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscin_status2.u32, "HDMITX_SSCIN_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_maxGet
//  Description : Set the value of the member HDMITX_SSCIN_STATUS2.sscin_sscfifo_depth_max
//  Input       : HI_U32 usscin_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCIN_STATUS2_sscin_sscfifo_depth_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCIN_STATUS2 o_hdmitx_sscin_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCIN_STATUS2.u32);
    o_hdmitx_sscin_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscin_status2.bits.sscin_sscfifo_depth_max;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_divSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_div
//  Input       : HI_U32 usscfb_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_divSet(HI_U32 usscfb_div)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status0.bits.sscfb_div = usscfb_div;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32, "HDMITX_SSCFB_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_divGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_div
//  Input       : HI_U32 usscfb_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_divGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status0.bits.sscfb_div;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_reverse_flagSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_reverse_flag
//  Input       : HI_U32 usscfb_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_reverse_flagSet(HI_U32 usscfb_reverse_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status0.bits.sscfb_reverse_flag = usscfb_reverse_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32, "HDMITX_SSCFB_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_reverse_flagGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_reverse_flag
//  Input       : HI_U32 usscfb_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_reverse_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status0.bits.sscfb_reverse_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_stop_flagSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_stop_flag
//  Input       : HI_U32 usscfb_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_stop_flagSet(HI_U32 usscfb_stop_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status0.bits.sscfb_stop_flag = usscfb_stop_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32, "HDMITX_SSCFB_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_stop_flagGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_stop_flag
//  Input       : HI_U32 usscfb_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_stop_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status0.bits.sscfb_stop_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_segmentSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_segment
//  Input       : HI_U32 usscfb_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_segmentSet(HI_U32 usscfb_segment)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status0.bits.sscfb_segment = usscfb_segment;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32, "HDMITX_SSCFB_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_segmentGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_segment
//  Input       : HI_U32 usscfb_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_segmentGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status0.bits.sscfb_segment;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_span_cntSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_span_cnt
//  Input       : HI_U32 usscfb_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_span_cntSet(HI_U32 usscfb_span_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status0.bits.sscfb_span_cnt = usscfb_span_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status0.u32, "HDMITX_SSCFB_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS0_sscfb_span_cntGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS0.sscfb_span_cnt
//  Input       : HI_U32 usscfb_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS0_sscfb_span_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS0 o_hdmitx_sscfb_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS0.u32);
    o_hdmitx_sscfb_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status0.bits.sscfb_span_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS1_sscfb_ssc_offsetSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS1.sscfb_ssc_offset
//  Input       : HI_U32 usscfb_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS1_sscfb_ssc_offsetSet(HI_U32 usscfb_ssc_offset)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS1 o_hdmitx_sscfb_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS1.u32);
    o_hdmitx_sscfb_status1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status1.bits.sscfb_ssc_offset = usscfb_ssc_offset;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status1.u32, "HDMITX_SSCFB_STATUS1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS1_sscfb_ssc_offsetGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS1.sscfb_ssc_offset
//  Input       : HI_U32 usscfb_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS1_sscfb_ssc_offsetGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS1 o_hdmitx_sscfb_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS1.u32);
    o_hdmitx_sscfb_status1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status1.bits.sscfb_ssc_offset;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_minSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS2.sscfb_sscfifo_depth_min
//  Input       : HI_U32 usscfb_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_minSet(HI_U32 usscfb_sscfifo_depth_min)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS2 o_hdmitx_sscfb_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS2.u32);
    o_hdmitx_sscfb_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status2.bits.sscfb_sscfifo_depth_min = usscfb_sscfifo_depth_min;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status2.u32, "HDMITX_SSCFB_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_minGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS2.sscfb_sscfifo_depth_min
//  Input       : HI_U32 usscfb_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_minGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS2 o_hdmitx_sscfb_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS2.u32);
    o_hdmitx_sscfb_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status2.bits.sscfb_sscfifo_depth_min;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_maxSet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS2.sscfb_sscfifo_depth_max
//  Input       : HI_U32 usscfb_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_maxSet(HI_U32 usscfb_sscfifo_depth_max)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS2 o_hdmitx_sscfb_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS2.u32);
    o_hdmitx_sscfb_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfb_status2.bits.sscfb_sscfifo_depth_max = usscfb_sscfifo_depth_max;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfb_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfb_status2.u32, "HDMITX_SSCFB_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_maxGet
//  Description : Set the value of the member HDMITX_SSCFB_STATUS2.sscfb_sscfifo_depth_max
//  Input       : HI_U32 usscfb_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFB_STATUS2_sscfb_sscfifo_depth_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFB_STATUS2 o_hdmitx_sscfb_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFB_STATUS2.u32);
    o_hdmitx_sscfb_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfb_status2.bits.sscfb_sscfifo_depth_max;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_divSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_div
//  Input       : HI_U32 ufbmash_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_divSet(HI_U32 ufbmash_div)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status0.bits.fbmash_div = ufbmash_div;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32, "HDMITX_SSCFBMASH111_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_divGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_div
//  Input       : HI_U32 ufbmash_div: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_divGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status0.bits.fbmash_div;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_reverse_flagSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_reverse_flag
//  Input       : HI_U32 ufbmash_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_reverse_flagSet(HI_U32 ufbmash_reverse_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status0.bits.fbmash_reverse_flag = ufbmash_reverse_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32, "HDMITX_SSCFBMASH111_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_reverse_flagGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_reverse_flag
//  Input       : HI_U32 ufbmash_reverse_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_reverse_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status0.bits.fbmash_reverse_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_stop_flagSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_stop_flag
//  Input       : HI_U32 ufbmash_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_stop_flagSet(HI_U32 ufbmash_stop_flag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status0.bits.fbmash_stop_flag = ufbmash_stop_flag;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32, "HDMITX_SSCFBMASH111_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_stop_flagGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_stop_flag
//  Input       : HI_U32 ufbmash_stop_flag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_stop_flagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status0.bits.fbmash_stop_flag;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_segmentSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_segment
//  Input       : HI_U32 ufbmash_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_segmentSet(HI_U32 ufbmash_segment)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status0.bits.fbmash_segment = ufbmash_segment;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32, "HDMITX_SSCFBMASH111_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_segmentGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_segment
//  Input       : HI_U32 ufbmash_segment: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_segmentGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status0.bits.fbmash_segment;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_span_cntSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_span_cnt
//  Input       : HI_U32 ufbmash_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_span_cntSet(HI_U32 ufbmash_span_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status0.bits.fbmash_span_cnt = ufbmash_span_cnt;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status0.u32, "HDMITX_SSCFBMASH111_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_span_cntGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS0.fbmash_span_cnt
//  Input       : HI_U32 ufbmash_span_cnt: 15 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS0_fbmash_span_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS0 o_hdmitx_sscfbmash111_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS0.u32);
    o_hdmitx_sscfbmash111_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status0.bits.fbmash_span_cnt;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS1_fbmash_ssc_offsetSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS1.fbmash_ssc_offset
//  Input       : HI_U32 ufbmash_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS1_fbmash_ssc_offsetSet(HI_U32 ufbmash_ssc_offset)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS1 o_hdmitx_sscfbmash111_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS1.u32);
    o_hdmitx_sscfbmash111_status1.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status1.bits.fbmash_ssc_offset = ufbmash_ssc_offset;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status1.u32, "HDMITX_SSCFBMASH111_STATUS1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS1_fbmash_ssc_offsetGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS1.fbmash_ssc_offset
//  Input       : HI_U32 ufbmash_ssc_offset: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS1_fbmash_ssc_offsetGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS1 o_hdmitx_sscfbmash111_status1;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS1.u32);
    o_hdmitx_sscfbmash111_status1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status1.bits.fbmash_ssc_offset;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_minSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS2.fbmash_sscfifo_depth_min
//  Input       : HI_U32 ufbmash_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_minSet(HI_U32 ufbmash_sscfifo_depth_min)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS2 o_hdmitx_sscfbmash111_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS2.u32);
    o_hdmitx_sscfbmash111_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status2.bits.fbmash_sscfifo_depth_min = ufbmash_sscfifo_depth_min;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status2.u32, "HDMITX_SSCFBMASH111_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_minGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS2.fbmash_sscfifo_depth_min
//  Input       : HI_U32 ufbmash_sscfifo_depth_min: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_minGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS2 o_hdmitx_sscfbmash111_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS2.u32);
    o_hdmitx_sscfbmash111_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status2.bits.fbmash_sscfifo_depth_min;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_maxSet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS2.fbmash_sscfifo_depth_max
//  Input       : HI_U32 ufbmash_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_maxSet(HI_U32 ufbmash_sscfifo_depth_max)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS2 o_hdmitx_sscfbmash111_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS2.u32);
    o_hdmitx_sscfbmash111_status2.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_sscfbmash111_status2.bits.fbmash_sscfifo_depth_max = ufbmash_sscfifo_depth_max;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status2.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_sscfbmash111_status2.u32, "HDMITX_SSCFBMASH111_STATUS2");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_maxGet
//  Description : Set the value of the member HDMITX_SSCFBMASH111_STATUS2.fbmash_sscfifo_depth_max
//  Input       : HI_U32 ufbmash_sscfifo_depth_max: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_SSCFBMASH111_STATUS2_fbmash_sscfifo_depth_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_SSCFBMASH111_STATUS2 o_hdmitx_sscfbmash111_status2;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_SSCFBMASH111_STATUS2.u32);
    o_hdmitx_sscfbmash111_status2.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_sscfbmash111_status2.bits.fbmash_sscfifo_depth_max;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch2_hSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch2_h
//  Input       : HI_U32 ureg_aphy_data_ch2_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch2_hSet(HI_U32 ureg_aphy_data_ch2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch2_h = ureg_aphy_data_ch2_h;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32, "HDMITX_AFIFO_DATA_H");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch2_hGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch2_h
//  Input       : HI_U32 ureg_aphy_data_ch2_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch2_hGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch2_h;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch1_hSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch1_h
//  Input       : HI_U32 ureg_aphy_data_ch1_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch1_hSet(HI_U32 ureg_aphy_data_ch1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch1_h = ureg_aphy_data_ch1_h;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32, "HDMITX_AFIFO_DATA_H");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch1_hGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch1_h
//  Input       : HI_U32 ureg_aphy_data_ch1_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch1_hGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch1_h;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch0_hSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch0_h
//  Input       : HI_U32 ureg_aphy_data_ch0_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch0_hSet(HI_U32 ureg_aphy_data_ch0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch0_h = ureg_aphy_data_ch0_h;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_h.u32, "HDMITX_AFIFO_DATA_H");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch0_hGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_H.reg_aphy_data_ch0_h
//  Input       : HI_U32 ureg_aphy_data_ch0_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_H_reg_aphy_data_ch0_hGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_H o_hdmitx_afifo_data_h;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_H.u32);
    o_hdmitx_afifo_data_h.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_h.bits.reg_aphy_data_ch0_h;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_clk_hSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_clk_h
//  Input       : HI_U32 ureg_aphy_data_clk_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_clk_hSet(HI_U32 ureg_aphy_data_clk_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_aphy_data_clk_h = ureg_aphy_data_clk_h;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    //HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_clk_hGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_clk_h
//  Input       : HI_U32 ureg_aphy_data_clk_h: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_clk_hGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_aphy_data_clk_h;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_empt_statusSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_empt_status
//  Input       : HI_U32 utxafifo_empt_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_empt_statusSet(HI_U32 utxafifo_empt_status)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.txafifo_empt_status = utxafifo_empt_status;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_empt_statusGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_empt_status
//  Input       : HI_U32 utxafifo_empt_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_empt_statusGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.txafifo_empt_status;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aempt_statusSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_aempt_status
//  Input       : HI_U32 utxafifo_aempt_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aempt_statusSet(HI_U32 utxafifo_aempt_status)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.txafifo_aempt_status = utxafifo_aempt_status;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aempt_statusGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_aempt_status
//  Input       : HI_U32 utxafifo_aempt_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aempt_statusGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.txafifo_aempt_status;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_full_statusSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_full_status
//  Input       : HI_U32 utxafifo_full_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_full_statusSet(HI_U32 utxafifo_full_status)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.txafifo_full_status = utxafifo_full_status;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_full_statusGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_full_status
//  Input       : HI_U32 utxafifo_full_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_full_statusGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.txafifo_full_status;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aful_statusSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_aful_status
//  Input       : HI_U32 utxafifo_aful_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aful_statusSet(HI_U32 utxafifo_aful_status)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.txafifo_aful_status = utxafifo_aful_status;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aful_statusGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.txafifo_aful_status
//  Input       : HI_U32 utxafifo_aful_status: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_txafifo_aful_statusGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.txafifo_aful_status;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch2_selSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch2_sel
//  Input       : HI_U32 ureg_aphy_data_ch2_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch2_selSet(HI_U32 ureg_aphy_data_ch2_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch2_sel = ureg_aphy_data_ch2_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch2_selGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch2_sel
//  Input       : HI_U32 ureg_aphy_data_ch2_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch2_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch2_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch1_selSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch1_sel
//  Input       : HI_U32 ureg_aphy_data_ch1_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch1_selSet(HI_U32 ureg_aphy_data_ch1_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch1_sel = ureg_aphy_data_ch1_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch1_selGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch1_sel
//  Input       : HI_U32 ureg_aphy_data_ch1_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch1_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch1_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch0_selSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch0_sel
//  Input       : HI_U32 ureg_aphy_data_ch0_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch0_selSet(HI_U32 ureg_aphy_data_ch0_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch0_sel = ureg_aphy_data_ch0_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch0_selGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_aphy_data_ch0_sel
//  Input       : HI_U32 ureg_aphy_data_ch0_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_ch0_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_aphy_data_ch0_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_rstSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_txafifo_rst
//  Input       : HI_U32 ureg_txafifo_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_rstSet(HI_U32 ureg_txafifo_rst)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_txafifo_rst = ureg_txafifo_rst;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_rstGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_txafifo_rst
//  Input       : HI_U32 ureg_txafifo_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_rstGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_txafifo_rst;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_status_rstSet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_txafifo_status_rst
//  Input       : HI_U32 ureg_txafifo_status_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_status_rstSet(HI_U32 ureg_txafifo_status_rst)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_sel.bits.reg_txafifo_status_rst = ureg_txafifo_status_rst;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_sel.u32, "HDMITX_AFIFO_DATA_SEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_status_rstGet
//  Description : Set the value of the member HDMITX_AFIFO_DATA_SEL.reg_txafifo_status_rst
//  Input       : HI_U32 ureg_txafifo_status_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_SEL_reg_txafifo_status_rstGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_SEL o_hdmitx_afifo_data_sel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_SEL.u32);
    o_hdmitx_afifo_data_sel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_sel.bits.reg_txafifo_status_rst;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CLK_reg_aphy_data_clk_lSet
//  Description : Set the value of the member HDMITX_AFIFO_CLK.reg_aphy_data_clk_l
//  Input       : HI_U32 ureg_aphy_data_clk_l: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CLK_reg_aphy_data_clk_lSet(HI_U32 ureg_aphy_data_clk_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CLK o_hdmitx_afifo_clk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CLK.u32);
    o_hdmitx_afifo_clk.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_clk.bits.reg_aphy_data_clk_l = ureg_aphy_data_clk_l;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_clk.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_clk.u32, "HDMITX_AFIFO_CLK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CLK_reg_aphy_data_clk_lGet
//  Description : Set the value of the member HDMITX_AFIFO_CLK.reg_aphy_data_clk_l
//  Input       : HI_U32 ureg_aphy_data_clk_l: 30 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CLK_reg_aphy_data_clk_lGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CLK o_hdmitx_afifo_clk;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CLK.u32);
    o_hdmitx_afifo_clk.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_clk.bits.reg_aphy_data_clk_l;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch2Set
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch2
//  Input       : HI_U32 ureg_aphy_data_ch2: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch2Set(HI_U32 ureg_aphy_data_ch2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch2 = ureg_aphy_data_ch2;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32, "HDMITX_AFIFO_DATA_L");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch2Get
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch2
//  Input       : HI_U32 ureg_aphy_data_ch2: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch2;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch1Set
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch1
//  Input       : HI_U32 ureg_aphy_data_ch1: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch1Set(HI_U32 ureg_aphy_data_ch1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch1 = ureg_aphy_data_ch1;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32, "HDMITX_AFIFO_DATA_L");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch1Get
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch1
//  Input       : HI_U32 ureg_aphy_data_ch1: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch1;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch0Set
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch0
//  Input       : HI_U32 ureg_aphy_data_ch0: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch0Set(HI_U32 ureg_aphy_data_ch0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch0 = ureg_aphy_data_ch0;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_data_l.u32, "HDMITX_AFIFO_DATA_L");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch0Get
//  Description : Set the value of the member HDMITX_AFIFO_DATA_L.reg_aphy_data_ch0
//  Input       : HI_U32 ureg_aphy_data_ch0: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_DATA_L_reg_aphy_data_ch0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_DATA_L o_hdmitx_afifo_data_l;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_DATA_L.u32);
    o_hdmitx_afifo_data_l.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_data_l.bits.reg_aphy_data_ch0;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d2_dataout_selSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d2_dataout_sel
//  Input       : HI_U32 ureg_d2_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d2_dataout_selSet(HI_U32 ureg_d2_dataout_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_d2_dataout_sel = ureg_d2_dataout_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d2_dataout_selGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d2_dataout_sel
//  Input       : HI_U32 ureg_d2_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d2_dataout_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_d2_dataout_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d1_dataout_selSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d1_dataout_sel
//  Input       : HI_U32 ureg_d1_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d1_dataout_selSet(HI_U32 ureg_d1_dataout_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_d1_dataout_sel = ureg_d1_dataout_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d1_dataout_selGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d1_dataout_sel
//  Input       : HI_U32 ureg_d1_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d1_dataout_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_d1_dataout_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d0_dataout_selSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d0_dataout_sel
//  Input       : HI_U32 ureg_d0_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d0_dataout_selSet(HI_U32 ureg_d0_dataout_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_d0_dataout_sel = ureg_d0_dataout_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_d0_dataout_selGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_d0_dataout_sel
//  Input       : HI_U32 ureg_d0_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_d0_dataout_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_d0_dataout_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_ck_dataout_selSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_ck_dataout_sel
//  Input       : HI_U32 ureg_ck_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_ck_dataout_selSet(HI_U32 ureg_ck_dataout_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_ck_dataout_sel = ureg_ck_dataout_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_ck_dataout_selGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_ck_dataout_sel
//  Input       : HI_U32 ureg_ck_dataout_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_ck_dataout_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_ck_dataout_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_errinjectSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_errinject
//  Input       : HI_U32 ureg_prbs7_errinject: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_errinjectSet(HI_U32 ureg_prbs7_errinject)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_prbs7_errinject = ureg_prbs7_errinject;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_errinjectGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_errinject
//  Input       : HI_U32 ureg_prbs7_errinject: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_errinjectGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_prbs7_errinject;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_clrSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_clr
//  Input       : HI_U32 ureg_prbs7_clr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_clrSet(HI_U32 ureg_prbs7_clr)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_prbs7_clr = ureg_prbs7_clr;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_clrGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_clr
//  Input       : HI_U32 ureg_prbs7_clr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_clrGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_prbs7_clr;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_enSet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_en
//  Input       : HI_U32 ureg_prbs7_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_enSet(HI_U32 ureg_prbs7_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_afifo_channel.bits.reg_prbs7_en = ureg_prbs7_en;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_afifo_channel.u32, "HDMITX_AFIFO_CHANNEL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_enGet
//  Description : Set the value of the member HDMITX_AFIFO_CHANNEL.reg_prbs7_en
//  Input       : HI_U32 ureg_prbs7_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_AFIFO_CHANNEL_reg_prbs7_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_AFIFO_CHANNEL o_hdmitx_afifo_channel;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_AFIFO_CHANNEL.u32);
    o_hdmitx_afifo_channel.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_afifo_channel.bits.reg_prbs7_en;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_txpll_lockSet
//  Description : Set the value of the member APHY_STATUS0.ad_txpll_lock
//  Input       : HI_U32 uad_txpll_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_txpll_lockSet(HI_U32 uad_txpll_lock)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_status0.bits.ad_txpll_lock = uad_txpll_lock;
    HDMITXRegWrite(pu32RegAddr, o_aphy_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_status0.u32, "APHY_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_txpll_lockGet
//  Description : Set the value of the member APHY_STATUS0.ad_txpll_lock
//  Input       : HI_U32 uad_txpll_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_txpll_lockGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_status0.bits.ad_txpll_lock;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_txpll_hvflagSet
//  Description : Set the value of the member APHY_STATUS0.ad_txpll_hvflag
//  Input       : HI_U32 uad_txpll_hvflag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_txpll_hvflagSet(HI_U32 uad_txpll_hvflag)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_status0.bits.ad_txpll_hvflag = uad_txpll_hvflag;
    HDMITXRegWrite(pu32RegAddr, o_aphy_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_status0.u32, "APHY_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_txpll_hvflagGet
//  Description : Set the value of the member APHY_STATUS0.ad_txpll_hvflag
//  Input       : HI_U32 uad_txpll_hvflag: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_txpll_hvflagGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_status0.bits.ad_txpll_hvflag;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_rxsenseSet
//  Description : Set the value of the member APHY_STATUS0.ad_rxsense
//  Input       : HI_U32 uad_rxsense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_rxsenseSet(HI_U32 uad_rxsense)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_status0.bits.ad_rxsense = uad_rxsense;
    HDMITXRegWrite(pu32RegAddr, o_aphy_status0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_status0.u32, "APHY_STATUS0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_STATUS0_ad_rxsenseGet
//  Description : Set the value of the member APHY_STATUS0.ad_rxsense
//  Input       : HI_U32 uad_rxsense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_STATUS0_ad_rxsenseGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_STATUS0 o_aphy_status0;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_STATUS0.u32);
    o_aphy_status0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_status0.bits.ad_rxsense;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_divselSet
//  Description : Set the value of the member APHY_TOP_PD.reg_divsel
//  Input       : HI_U32 ureg_divsel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_divselSet(HI_U32 ureg_divsel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_divsel = ureg_divsel;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_divselGet
//  Description : Set the value of the member APHY_TOP_PD.reg_divsel
//  Input       : HI_U32 ureg_divsel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_divselGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_divsel;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_txpll_pdSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_txpll_pd
//  Input       : HI_U32 ureg_gc_txpll_pd: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_txpll_pdSet(HI_U32 ureg_gc_txpll_pd)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_txpll_pd = ureg_gc_txpll_pd;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_txpll_pdGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_txpll_pd
//  Input       : HI_U32 ureg_gc_txpll_pd: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_txpll_pdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_txpll_pd;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_rxsense
//  Input       : HI_U32 ureg_gc_pd_rxsense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseSet(HI_U32 ureg_gc_pd_rxsense)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd_rxsense = ureg_gc_pd_rxsense;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_rxsense
//  Input       : HI_U32 ureg_gc_pd_rxsense: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd_rxsense;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_rtermSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_rterm
//  Input       : HI_U32 ureg_gc_pd_rterm: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_rtermSet(HI_U32 ureg_gc_pd_rterm)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd_rterm = ureg_gc_pd_rterm;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_rtermGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_rterm
//  Input       : HI_U32 ureg_gc_pd_rterm: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_rtermGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd_rterm;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_ldoSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_ldo
//  Input       : HI_U32 ureg_gc_pd_ldo: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_ldoSet(HI_U32 ureg_gc_pd_ldo)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd_ldo = ureg_gc_pd_ldo;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_ldoGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_ldo
//  Input       : HI_U32 ureg_gc_pd_ldo: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_ldoGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd_ldo;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_deSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_de
//  Input       : HI_U32 ureg_gc_pd_de: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_deSet(HI_U32 ureg_gc_pd_de)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd_de = ureg_gc_pd_de;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_deGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_de
//  Input       : HI_U32 ureg_gc_pd_de: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_deGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd_de;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_bistSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_bist
//  Input       : HI_U32 ureg_gc_pd_bist: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_bistSet(HI_U32 ureg_gc_pd_bist)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd_bist = ureg_gc_pd_bist;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pd_bistGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd_bist
//  Input       : HI_U32 ureg_gc_pd_bist: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pd_bistGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd_bist;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pdSet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd
//  Input       : HI_U32 ureg_gc_pd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pdSet(HI_U32 ureg_gc_pd)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_top_pd.bits.reg_gc_pd = ureg_gc_pd;
    HDMITXRegWrite(pu32RegAddr, o_aphy_top_pd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_top_pd.u32, "APHY_TOP_PD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TOP_PD_reg_gc_pdGet
//  Description : Set the value of the member APHY_TOP_PD.reg_gc_pd
//  Input       : HI_U32 ureg_gc_pd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TOP_PD_reg_gc_pdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TOP_PD o_aphy_top_pd;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TOP_PD.u32);
    o_aphy_top_pd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_top_pd.bits.reg_gc_pd;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_de_clkSet
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_de_clk
//  Input       : HI_U32 ureg_isel_main_de_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_de_clkSet(HI_U32 ureg_isel_main_de_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_imain.bits.reg_isel_main_de_clk = ureg_isel_main_de_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_imain.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_imain.u32, "APHY_DRIVER_IMAIN");
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_de_clkGet
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_de_clk
//  Input       : HI_U32 ureg_isel_main_de_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_de_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_imain.bits.reg_isel_main_de_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d2Set
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d2
//  Input       : HI_U32 ureg_isel_main_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d2Set(HI_U32 ureg_isel_main_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_imain.bits.reg_isel_main_d2 = ureg_isel_main_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_imain.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_imain.u32, "APHY_DRIVER_IMAIN");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d2Get
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d2
//  Input       : HI_U32 ureg_isel_main_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_imain.bits.reg_isel_main_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d1Set
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d1
//  Input       : HI_U32 ureg_isel_main_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d1Set(HI_U32 ureg_isel_main_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_imain.bits.reg_isel_main_d1 = ureg_isel_main_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_imain.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_imain.u32, "APHY_DRIVER_IMAIN");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d1Get
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d1
//  Input       : HI_U32 ureg_isel_main_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_imain.bits.reg_isel_main_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d0Set
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d0
//  Input       : HI_U32 ureg_isel_main_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d0Set(HI_U32 ureg_isel_main_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_imain.bits.reg_isel_main_d0 = ureg_isel_main_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_imain.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_imain.u32, "APHY_DRIVER_IMAIN");
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d0Get
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_d0
//  Input       : HI_U32 ureg_isel_main_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_imain.bits.reg_isel_main_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_clkSet
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_clk
//  Input       : HI_U32 ureg_isel_main_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_clkSet(HI_U32 ureg_isel_main_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_imain.bits.reg_isel_main_clk = ureg_isel_main_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_imain.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_imain.u32, "APHY_DRIVER_IMAIN");
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IMAIN_reg_isel_main_clkGet
//  Description : Set the value of the member APHY_DRIVER_IMAIN.reg_isel_main_clk
//  Input       : HI_U32 ureg_isel_main_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IMAIN_reg_isel_main_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IMAIN o_aphy_driver_imain;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IMAIN.u32);
    o_aphy_driver_imain.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_imain.bits.reg_isel_main_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_pre_d0Set
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_pre_d0
//  Input       : HI_U32 ureg_isel_pre_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_pre_d0Set(HI_U32 ureg_isel_pre_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_ipre.bits.reg_isel_pre_d0 = ureg_isel_pre_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32, "APHY_DRIVER_IPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_pre_d0Get
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_pre_d0
//  Input       : HI_U32 ureg_isel_pre_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_pre_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_ipre.bits.reg_isel_pre_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_pre_clkSet
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_pre_clk
//  Input       : HI_U32 ureg_isel_pre_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_pre_clkSet(HI_U32 ureg_isel_pre_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_ipre.bits.reg_isel_pre_clk = ureg_isel_pre_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32, "APHY_DRIVER_IPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_pre_clkGet
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_pre_clk
//  Input       : HI_U32 ureg_isel_pre_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_pre_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_ipre.bits.reg_isel_pre_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d2Set
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d2
//  Input       : HI_U32 ureg_isel_main_de_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d2Set(HI_U32 ureg_isel_main_de_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_ipre.bits.reg_isel_main_de_d2 = ureg_isel_main_de_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32, "APHY_DRIVER_IPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d2Get
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d2
//  Input       : HI_U32 ureg_isel_main_de_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_ipre.bits.reg_isel_main_de_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d1Set
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d1
//  Input       : HI_U32 ureg_isel_main_de_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d1Set(HI_U32 ureg_isel_main_de_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_ipre.bits.reg_isel_main_de_d1 = ureg_isel_main_de_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32, "APHY_DRIVER_IPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d1Get
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d1
//  Input       : HI_U32 ureg_isel_main_de_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_ipre.bits.reg_isel_main_de_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d0Set
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d0
//  Input       : HI_U32 ureg_isel_main_de_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d0Set(HI_U32 ureg_isel_main_de_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_ipre.bits.reg_isel_main_de_d0 = ureg_isel_main_de_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_ipre.u32, "APHY_DRIVER_IPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d0Get
//  Description : Set the value of the member APHY_DRIVER_IPRE.reg_isel_main_de_d0
//  Input       : HI_U32 ureg_isel_main_de_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPRE o_aphy_driver_ipre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPRE.u32);
    o_aphy_driver_ipre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_ipre.bits.reg_isel_main_de_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d1Set
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_d1
//  Input       : HI_U32 ureg_isel_pre_de_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d1Set(HI_U32 ureg_isel_pre_de_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_iprede.bits.reg_isel_pre_de_d1 = ureg_isel_pre_de_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32, "APHY_DRIVER_IPREDE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d1Get
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_d1
//  Input       : HI_U32 ureg_isel_pre_de_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_iprede.bits.reg_isel_pre_de_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d0Set
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_d0
//  Input       : HI_U32 ureg_isel_pre_de_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d0Set(HI_U32 ureg_isel_pre_de_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_iprede.bits.reg_isel_pre_de_d0 = ureg_isel_pre_de_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32, "APHY_DRIVER_IPREDE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d0Get
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_d0
//  Input       : HI_U32 ureg_isel_pre_de_d0: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_iprede.bits.reg_isel_pre_de_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_clkSet
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_clk
//  Input       : HI_U32 ureg_isel_pre_de_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_clkSet(HI_U32 ureg_isel_pre_de_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_iprede.bits.reg_isel_pre_de_clk = ureg_isel_pre_de_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32, "APHY_DRIVER_IPREDE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_clkGet
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_de_clk
//  Input       : HI_U32 ureg_isel_pre_de_clk: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_iprede.bits.reg_isel_pre_de_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d2Set
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_d2
//  Input       : HI_U32 ureg_isel_pre_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d2Set(HI_U32 ureg_isel_pre_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_iprede.bits.reg_isel_pre_d2 = ureg_isel_pre_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32, "APHY_DRIVER_IPREDE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d2Get
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_d2
//  Input       : HI_U32 ureg_isel_pre_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_iprede.bits.reg_isel_pre_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d1Set
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_d1
//  Input       : HI_U32 ureg_isel_pre_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d1Set(HI_U32 ureg_isel_pre_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_iprede.bits.reg_isel_pre_d1 = ureg_isel_pre_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_iprede.u32, "APHY_DRIVER_IPREDE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d1Get
//  Description : Set the value of the member APHY_DRIVER_IPREDE.reg_isel_pre_d1
//  Input       : HI_U32 ureg_isel_pre_d1: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_IPREDE o_aphy_driver_iprede;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_IPREDE.u32);
    o_aphy_driver_iprede.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_iprede.bits.reg_isel_pre_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d2Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d2
//  Input       : HI_U32 ureg_rsel_pre_de_d2: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d2Set(HI_U32 ureg_rsel_pre_de_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_de_d2 = ureg_rsel_pre_de_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d2Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d2
//  Input       : HI_U32 ureg_rsel_pre_de_d2: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_de_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d1Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d1
//  Input       : HI_U32 ureg_rsel_pre_de_d1: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d1Set(HI_U32 ureg_rsel_pre_de_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_de_d1 = ureg_rsel_pre_de_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d1Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d1
//  Input       : HI_U32 ureg_rsel_pre_de_d1: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_de_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d0Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d0
//  Input       : HI_U32 ureg_rsel_pre_de_d0: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d0Set(HI_U32 ureg_rsel_pre_de_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_de_d0 = ureg_rsel_pre_de_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d0Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_d0
//  Input       : HI_U32 ureg_rsel_pre_de_d0: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_de_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_clkSet
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_clk
//  Input       : HI_U32 ureg_rsel_pre_de_clk: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_clkSet(HI_U32 ureg_rsel_pre_de_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_de_clk = ureg_rsel_pre_de_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_clkGet
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_de_clk
//  Input       : HI_U32 ureg_rsel_pre_de_clk: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_de_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d2Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d2
//  Input       : HI_U32 ureg_rsel_pre_d2: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d2Set(HI_U32 ureg_rsel_pre_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_d2 = ureg_rsel_pre_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d2Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d2
//  Input       : HI_U32 ureg_rsel_pre_d2: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d1Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d1
//  Input       : HI_U32 ureg_rsel_pre_d1: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d1Set(HI_U32 ureg_rsel_pre_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_d1 = ureg_rsel_pre_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d1Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d1
//  Input       : HI_U32 ureg_rsel_pre_d1: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d0Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d0
//  Input       : HI_U32 ureg_rsel_pre_d0: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d0Set(HI_U32 ureg_rsel_pre_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_d0 = ureg_rsel_pre_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d0Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_d0
//  Input       : HI_U32 ureg_rsel_pre_d0: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_clkSet
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_clk
//  Input       : HI_U32 ureg_rsel_pre_clk: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_clkSet(HI_U32 ureg_rsel_pre_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_rsel_pre_clk = ureg_rsel_pre_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_clkGet
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_rsel_pre_clk
//  Input       : HI_U32 ureg_rsel_pre_clk: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_rsel_pre_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_isel_pre_de_d2Set
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_isel_pre_de_d2
//  Input       : HI_U32 ureg_isel_pre_de_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_isel_pre_de_d2Set(HI_U32 ureg_isel_pre_de_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_driver_rpre.bits.reg_isel_pre_de_d2 = ureg_isel_pre_de_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_driver_rpre.u32, "APHY_DRIVER_RPRE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_DRIVER_RPRE_reg_isel_pre_de_d2Get
//  Description : Set the value of the member APHY_DRIVER_RPRE.reg_isel_pre_de_d2
//  Input       : HI_U32 ureg_isel_pre_de_d2: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_DRIVER_RPRE_reg_isel_pre_de_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_DRIVER_RPRE o_aphy_driver_rpre;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_DRIVER_RPRE.u32);
    o_aphy_driver_rpre.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_driver_rpre.bits.reg_isel_pre_de_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d2Set
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d2
//  Input       : HI_U32 ureg_rt_d2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d2Set(HI_U32 ureg_rt_d2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_rterm_ctrl.bits.reg_rt_d2 = ureg_rt_d2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32, "APHY_RTERM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d2Get
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d2
//  Input       : HI_U32 ureg_rt_d2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_rterm_ctrl.bits.reg_rt_d2;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d1Set
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d1
//  Input       : HI_U32 ureg_rt_d1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d1Set(HI_U32 ureg_rt_d1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_rterm_ctrl.bits.reg_rt_d1 = ureg_rt_d1;
    HDMITXRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32, "APHY_RTERM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d1Get
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d1
//  Input       : HI_U32 ureg_rt_d1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_rterm_ctrl.bits.reg_rt_d1;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d0Set
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d0
//  Input       : HI_U32 ureg_rt_d0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d0Set(HI_U32 ureg_rt_d0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_rterm_ctrl.bits.reg_rt_d0 = ureg_rt_d0;
    HDMITXRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32, "APHY_RTERM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_d0Get
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_d0
//  Input       : HI_U32 ureg_rt_d0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_d0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_rterm_ctrl.bits.reg_rt_d0;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_clkSet
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_clk
//  Input       : HI_U32 ureg_rt_clk: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_clkSet(HI_U32 ureg_rt_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_rterm_ctrl.bits.reg_rt_clk = ureg_rt_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_rterm_ctrl.u32, "APHY_RTERM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_RTERM_CTRL_reg_rt_clkGet
//  Description : Set the value of the member APHY_RTERM_CTRL.reg_rt_clk
//  Input       : HI_U32 ureg_rt_clk: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_RTERM_CTRL_reg_rt_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_RTERM_CTRL o_aphy_rterm_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_RTERM_CTRL.u32);
    o_aphy_rterm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_rterm_ctrl.bits.reg_rt_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sr_clkSet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sr_clk
//  Input       : HI_U32 ureg_sr_clk: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sr_clkSet(HI_U32 ureg_sr_clk)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_misc_ctrl.bits.reg_sr_clk = ureg_sr_clk;
    HDMITXRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32, "APHY_MISC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sr_clkGet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sr_clk
//  Input       : HI_U32 ureg_sr_clk: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sr_clkGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_misc_ctrl.bits.reg_sr_clk;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sel_rxsenseSet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sel_rxsense
//  Input       : HI_U32 ureg_sel_rxsense: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sel_rxsenseSet(HI_U32 ureg_sel_rxsense)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_misc_ctrl.bits.reg_sel_rxsense = ureg_sel_rxsense;
    HDMITXRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32, "APHY_MISC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sel_rxsenseGet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sel_rxsense
//  Input       : HI_U32 ureg_sel_rxsense: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sel_rxsenseGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_misc_ctrl.bits.reg_sel_rxsense;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sel_ldoSet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sel_ldo
//  Input       : HI_U32 ureg_sel_ldo: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sel_ldoSet(HI_U32 ureg_sel_ldo)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_misc_ctrl.bits.reg_sel_ldo = ureg_sel_ldo;
    HDMITXRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_misc_ctrl.u32, "APHY_MISC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_MISC_CTRL_reg_sel_ldoGet
//  Description : Set the value of the member APHY_MISC_CTRL.reg_sel_ldo
//  Input       : HI_U32 ureg_sel_ldo: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_MISC_CTRL_reg_sel_ldoGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_MISC_CTRL o_aphy_misc_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_MISC_CTRL.u32);
    o_aphy_misc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_misc_ctrl.bits.reg_sel_ldo;
}

//******************************************************************************
//  Function    : HDMI_APHY_TEST_CTRL_reg_test_dcSet
//  Description : Set the value of the member APHY_TEST_CTRL.reg_test_dc
//  Input       : HI_U32 ureg_test_dc: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TEST_CTRL_reg_test_dcSet(HI_U32 ureg_test_dc)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TEST_CTRL o_aphy_test_ctrl;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TEST_CTRL.u32);
    o_aphy_test_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_test_ctrl.bits.reg_test_dc = ureg_test_dc;
    HDMITXRegWrite(pu32RegAddr, o_aphy_test_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_test_ctrl.u32, "APHY_TEST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TEST_CTRL_reg_test_dcGet
//  Description : Set the value of the member APHY_TEST_CTRL.reg_test_dc
//  Input       : HI_U32 ureg_test_dc: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TEST_CTRL_reg_test_dcGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TEST_CTRL o_aphy_test_ctrl;
   pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TEST_CTRL.u32);
    o_aphy_test_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_test_ctrl.bits.reg_test_dc;
}

//******************************************************************************
//  Function    : HDMI_APHY_TEST_CTRL_reg_testSet
//  Description : Set the value of the member APHY_TEST_CTRL.reg_test
//  Input       : HI_U32 ureg_test: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TEST_CTRL_reg_testSet(HI_U32 ureg_test)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TEST_CTRL o_aphy_test_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TEST_CTRL.u32);
    o_aphy_test_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_test_ctrl.bits.reg_test = ureg_test;
    HDMITXRegWrite(pu32RegAddr, o_aphy_test_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_test_ctrl.u32, "APHY_TEST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TEST_CTRL_reg_testGet
//  Description : Set the value of the member APHY_TEST_CTRL.reg_test
//  Input       : HI_U32 ureg_test: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TEST_CTRL_reg_testGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TEST_CTRL o_aphy_test_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TEST_CTRL.u32);
    o_aphy_test_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_test_ctrl.bits.reg_test;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_gc_txpll_en_sscdiv
//  Input       : HI_U32 ureg_gc_txpll_en_sscdiv: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivSet(HI_U32 ureg_gc_txpll_en_sscdiv)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_gc_txpll_en_sscdiv = ureg_gc_txpll_en_sscdiv;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_gc_txpll_en_sscdiv
//  Input       : HI_U32 ureg_gc_txpll_en_sscdiv: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_gc_txpll_en_sscdiv;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_ref_fb_div2Set
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_gc_txpll_en_ref_fb_div2
//  Input       : HI_U32 ureg_gc_txpll_en_ref_fb_div2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_ref_fb_div2Set(HI_U32 ureg_gc_txpll_en_ref_fb_div2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_gc_txpll_en_ref_fb_div2 = ureg_gc_txpll_en_ref_fb_div2;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_ref_fb_div2Get
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_gc_txpll_en_ref_fb_div2
//  Input       : HI_U32 ureg_gc_txpll_en_ref_fb_div2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_ref_fb_div2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_gc_txpll_en_ref_fb_div2;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_icp_ictrlSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_icp_ictrl
//  Input       : HI_U32 ureg_txpll_icp_ictrl: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_icp_ictrlSet(HI_U32 ureg_txpll_icp_ictrl)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_txpll_icp_ictrl = ureg_txpll_icp_ictrl;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_icp_ictrlGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_icp_ictrl
//  Input       : HI_U32 ureg_txpll_icp_ictrl: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_icp_ictrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_txpll_icp_ictrl;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_postSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_post
//  Input       : HI_U32 ureg_txpll_divsel_post: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_postSet(HI_U32 ureg_txpll_divsel_post)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_txpll_divsel_post = ureg_txpll_divsel_post;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_postGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_post
//  Input       : HI_U32 ureg_txpll_divsel_post: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_postGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_txpll_divsel_post;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_loopSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_loop
//  Input       : HI_U32 ureg_txpll_divsel_loop: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_loopSet(HI_U32 ureg_txpll_divsel_loop)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_txpll_divsel_loop = ureg_txpll_divsel_loop;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_loopGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_loop
//  Input       : HI_U32 ureg_txpll_divsel_loop: 7 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_loopGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_txpll_divsel_loop;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_inSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_in
//  Input       : HI_U32 ureg_txpll_divsel_in: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_inSet(HI_U32 ureg_txpll_divsel_in)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_txpll_divsel_in = ureg_txpll_divsel_in;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_inGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_divsel_in
//  Input       : HI_U32 ureg_txpll_divsel_in: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_inGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_txpll_divsel_in;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_clkin_selSet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_clkin_sel
//  Input       : HI_U32 ureg_txpll_clkin_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_clkin_selSet(HI_U32 ureg_txpll_clkin_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

   pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_ctrl.bits.reg_txpll_clkin_sel = ureg_txpll_clkin_sel;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_ctrl.u32, "APHY_TXPLL_CTRL");
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_CTRL_reg_txpll_clkin_selGet
//  Description : Set the value of the member APHY_TXPLL_CTRL.reg_txpll_clkin_sel
//  Input       : HI_U32 ureg_txpll_clkin_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_CTRL_reg_txpll_clkin_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_CTRL o_aphy_txpll_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_CTRL.u32);
    o_aphy_txpll_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_ctrl.bits.reg_txpll_clkin_sel;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_TEST_reg_txpll_testSet
//  Description : Set the value of the member APHY_TXPLL_TEST.reg_txpll_test
//  Input       : HI_U32 ureg_txpll_test: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_TEST_reg_txpll_testSet(HI_U32 ureg_txpll_test)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_TEST o_aphy_txpll_test;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_TEST.u32);
    o_aphy_txpll_test.u32 = HDMITXRegRead(pu32RegAddr);
    o_aphy_txpll_test.bits.reg_txpll_test = ureg_txpll_test;
    HDMITXRegWrite(pu32RegAddr, o_aphy_txpll_test.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aphy_txpll_test.u32, "APHY_TXPLL_TEST");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_APHY_TXPLL_TEST_reg_txpll_testGet
//  Description : Set the value of the member APHY_TXPLL_TEST.reg_txpll_test
//  Input       : HI_U32 ureg_txpll_test: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_APHY_TXPLL_TEST_reg_txpll_testGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_APHY_TXPLL_TEST o_aphy_txpll_test;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->APHY_TXPLL_TEST.u32);
    o_aphy_txpll_test.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aphy_txpll_test.bits.reg_txpll_test;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lock_timesSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lock_times
//  Input       : HI_U32 ureg_pll_lock_times: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lock_timesSet(HI_U32 ureg_pll_lock_times)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_pll_lock_times = ureg_pll_lock_times;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lock_timesGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lock_times
//  Input       : HI_U32 ureg_pll_lock_times: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lock_timesGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_pll_lock_times;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lockext_selSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lockext_sel
//  Input       : HI_U32 ureg_pll_lockext_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lockext_selSet(HI_U32 ureg_pll_lockext_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_pll_lockext_sel = ureg_pll_lockext_sel;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lockext_selGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lockext_sel
//  Input       : HI_U32 ureg_pll_lockext_sel: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lockext_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_pll_lockext_sel;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_unlock_selSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_unlock_sel
//  Input       : HI_U32 ureg_pll_unlock_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_unlock_selSet(HI_U32 ureg_pll_unlock_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_pll_unlock_sel = ureg_pll_unlock_sel;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_unlock_selGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_unlock_sel
//  Input       : HI_U32 ureg_pll_unlock_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_unlock_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_pll_unlock_sel;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lock_selSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lock_sel
//  Input       : HI_U32 ureg_pll_lock_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lock_selSet(HI_U32 ureg_pll_lock_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_pll_lock_sel = ureg_pll_lock_sel;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_pll_lock_selGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_pll_lock_sel
//  Input       : HI_U32 ureg_pll_lock_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_pll_lock_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_pll_lock_sel;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_ref_unlock_timerSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_ref_unlock_timer
//  Input       : HI_U32 ureg_ref_unlock_timer: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_ref_unlock_timerSet(HI_U32 ureg_ref_unlock_timer)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_ref_unlock_timer = ureg_ref_unlock_timer;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_ref_unlock_timerGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_ref_unlock_timer
//  Input       : HI_U32 ureg_ref_unlock_timer: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_ref_unlock_timerGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_ref_unlock_timer;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_ref_lock_timerSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_ref_lock_timer
//  Input       : HI_U32 ureg_ref_lock_timer: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_ref_lock_timerSet(HI_U32 ureg_ref_lock_timer)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_ref_lock_timer = ureg_ref_lock_timer;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_ref_lock_timerGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_ref_lock_timer
//  Input       : HI_U32 ureg_ref_lock_timer: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_ref_lock_timerGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_ref_lock_timer;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_fb_prd_selSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_fb_prd_sel
//  Input       : HI_U32 ureg_fb_prd_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_fb_prd_selSet(HI_U32 ureg_fb_prd_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_fb_prd_sel = ureg_fb_prd_sel;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_fb_prd_selGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_fb_prd_sel
//  Input       : HI_U32 ureg_fb_prd_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_fb_prd_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_fb_prd_sel;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_fb_div_selSet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_fb_div_sel
//  Input       : HI_U32 ureg_fb_div_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_fb_div_selSet(HI_U32 ureg_fb_div_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_ctrl.bits.reg_fb_div_sel = ureg_fb_div_sel;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_ctrl.u32, "PLL_LOCK_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_CTRL_reg_fb_div_selGet
//  Description : Set the value of the member PLL_LOCK_CTRL.reg_fb_div_sel
//  Input       : HI_U32 ureg_fb_div_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_CTRL_reg_fb_div_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_CTRL o_pll_lock_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_CTRL.u32);
    o_pll_lock_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_ctrl.bits.reg_fb_div_sel;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_hdmi_clk_lockSet
//  Description : Set the value of the member PLL_LOCK_STATUS.hdmi_clk_lock
//  Input       : HI_U32 uhdmi_clk_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_hdmi_clk_lockSet(HI_U32 uhdmi_clk_lock)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_status.bits.hdmi_clk_lock = uhdmi_clk_lock;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_status.u32, "PLL_LOCK_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_hdmi_clk_lockGet
//  Description : Set the value of the member PLL_LOCK_STATUS.hdmi_clk_lock
//  Input       : HI_U32 uhdmi_clk_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_hdmi_clk_lockGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_status.bits.hdmi_clk_lock;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_pll_lock_oneshotSet
//  Description : Set the value of the member PLL_LOCK_STATUS.pll_lock_oneshot
//  Input       : HI_U32 upll_lock_oneshot: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_pll_lock_oneshotSet(HI_U32 upll_lock_oneshot)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_status.bits.pll_lock_oneshot = upll_lock_oneshot;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_status.u32, "PLL_LOCK_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_pll_lock_oneshotGet
//  Description : Set the value of the member PLL_LOCK_STATUS.pll_lock_oneshot
//  Input       : HI_U32 upll_lock_oneshot: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_pll_lock_oneshotGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_status.bits.pll_lock_oneshot;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_test_pll_cntSet
//  Description : Set the value of the member PLL_LOCK_STATUS.test_pll_cnt
//  Input       : HI_U32 utest_pll_cnt: 17 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_test_pll_cntSet(HI_U32 utest_pll_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_status.bits.test_pll_cnt = utest_pll_cnt;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_status.u32, "PLL_LOCK_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_test_pll_cntGet
//  Description : Set the value of the member PLL_LOCK_STATUS.test_pll_cnt
//  Input       : HI_U32 utest_pll_cnt: 17 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_test_pll_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_status.bits.test_pll_cnt;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_pll_ena_edgeSet
//  Description : Set the value of the member PLL_LOCK_STATUS.pll_ena_edge
//  Input       : HI_U32 upll_ena_edge: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_pll_ena_edgeSet(HI_U32 upll_ena_edge)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_status.bits.pll_ena_edge = upll_ena_edge;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_status.u32, "PLL_LOCK_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_pll_ena_edgeGet
//  Description : Set the value of the member PLL_LOCK_STATUS.pll_ena_edge
//  Input       : HI_U32 upll_ena_edge: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_pll_ena_edgeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;
    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_status.bits.pll_ena_edge;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_clk_fb_existSet
//  Description : Set the value of the member PLL_LOCK_STATUS.clk_fb_exist
//  Input       : HI_U32 uclk_fb_exist: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_clk_fb_existSet(HI_U32 uclk_fb_exist)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    o_pll_lock_status.bits.clk_fb_exist = uclk_fb_exist;
    HDMITXRegWrite(pu32RegAddr, o_pll_lock_status.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pll_lock_status.u32, "PLL_LOCK_STATUS");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PLL_LOCK_STATUS_clk_fb_existGet
//  Description : Set the value of the member PLL_LOCK_STATUS.clk_fb_exist
//  Input       : HI_U32 uclk_fb_exist: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_PLL_LOCK_STATUS_clk_fb_existGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PLL_LOCK_STATUS o_pll_lock_status;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->PLL_LOCK_STATUS.u32);
    o_pll_lock_status.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pll_lock_status.bits.clk_fb_exist;
}

//******************************************************************************
//  Function    : HDMI_REG_TXDPHY_DUMMY_CTRL_reg_txdphy_dummy_ctrlSet
//  Description : Set the value of the member REG_TXDPHY_DUMMY_CTRL.reg_txdphy_dummy_ctrl
//  Input       : HI_U32 ureg_txdphy_dummy_ctrl: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_REG_TXDPHY_DUMMY_CTRL_reg_txdphy_dummy_ctrlSet(HI_U32 ureg_txdphy_dummy_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
    U_REG_TXDPHY_DUMMY_CTRL o_reg_txdphy_dummy_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->REG_TXDPHY_DUMMY_CTRL.u32);
    o_reg_txdphy_dummy_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_reg_txdphy_dummy_ctrl.bits.reg_txdphy_dummy_ctrl = ureg_txdphy_dummy_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_reg_txdphy_dummy_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_reg_txdphy_dummy_ctrl.u32, "REG_TXDPHY_DUMMY_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_REG_TXDPHY_DUMMY_CTRL_reg_txdphy_dummy_ctrlGet
//  Description : Set the value of the member REG_TXDPHY_DUMMY_CTRL.reg_txdphy_dummy_ctrl
//  Input       : HI_U32 ureg_txdphy_dummy_ctrl: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_REG_TXDPHY_DUMMY_CTRL_reg_txdphy_dummy_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_REG_TXDPHY_DUMMY_CTRL o_reg_txdphy_dummy_ctrl;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->REG_TXDPHY_DUMMY_CTRL.u32);
    o_reg_txdphy_dummy_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_reg_txdphy_dummy_ctrl.bits.reg_txdphy_dummy_ctrl;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_ssc_mode_fb
//  Input       : HI_U32 ureg_ssc_mode_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbSet(HI_U32 ureg_ssc_mode_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_set.bits.reg_ssc_mode_fb = ureg_ssc_mode_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32, "HDMITX_FBSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_ssc_mode_fb
//  Input       : HI_U32 ureg_ssc_mode_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_set.bits.reg_ssc_mode_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_sscfifo_depth_clr_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_sscfifo_depth_clr_fb
//  Input       : HI_U32 ureg_sscfifo_depth_clr_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_sscfifo_depth_clr_fbSet(HI_U32 ureg_sscfifo_depth_clr_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_set.bits.reg_sscfifo_depth_clr_fb = ureg_sscfifo_depth_clr_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32, "HDMITX_FBSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_sscfifo_depth_clr_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_sscfifo_depth_clr_fb
//  Input       : HI_U32 ureg_sscfifo_depth_clr_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_sscfifo_depth_clr_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_set.bits.reg_sscfifo_depth_clr_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_sscfifo_tune_en_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_sscfifo_tune_en_fb
//  Input       : HI_U32 ureg_sscfifo_tune_en_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_sscfifo_tune_en_fbSet(HI_U32 ureg_sscfifo_tune_en_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_set.bits.reg_sscfifo_tune_en_fb = ureg_sscfifo_tune_en_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32, "HDMITX_FBSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_sscfifo_tune_en_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_sscfifo_tune_en_fb
//  Input       : HI_U32 ureg_sscfifo_tune_en_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_sscfifo_tune_en_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_set.bits.reg_sscfifo_tune_en_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_set_load_fb
//  Input       : HI_U32 ureg_set_load_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_U32 ureg_set_load_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_set.bits.reg_set_load_fb = ureg_set_load_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32, "HDMITX_FBSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_set_load_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_set_load_fb
//  Input       : HI_U32 ureg_set_load_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_set_load_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_set.bits.reg_set_load_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_set_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_set_fb
//  Input       : HI_U32 ureg_set_fb: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_set_fbSet(HI_U32 ureg_set_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_set.bits.reg_set_fb = ureg_set_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_set.u32, "HDMITX_FBSSC_SET");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_SET_reg_set_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_SET.reg_set_fb
//  Input       : HI_U32 ureg_set_fb: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_SET_reg_set_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_SET o_hdmitx_fbssc_set;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_SET.u32);
    o_hdmitx_fbssc_set.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_set.bits.reg_set_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_ssc_sync_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_ssc_sync_fb
//  Input       : HI_U32 ureg_ssc_sync_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_ssc_sync_fbSet(HI_U32 ureg_ssc_sync_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_fifo.bits.reg_ssc_sync_fb = ureg_ssc_sync_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32, "HDMITX_FBSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_ssc_sync_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_ssc_sync_fb
//  Input       : HI_U32 ureg_ssc_sync_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_ssc_sync_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_fifo.bits.reg_ssc_sync_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_tune_reverse_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_tune_reverse_fb
//  Input       : HI_U32 ureg_tune_reverse_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_tune_reverse_fbSet(HI_U32 ureg_tune_reverse_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_fifo.bits.reg_tune_reverse_fb = ureg_tune_reverse_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32, "HDMITX_FBSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_tune_reverse_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_tune_reverse_fb
//  Input       : HI_U32 ureg_tune_reverse_fb: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_tune_reverse_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_fifo.bits.reg_tune_reverse_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_upper_thresh_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_sscfifo_upper_thresh_fb
//  Input       : HI_U32 ureg_sscfifo_upper_thresh_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_upper_thresh_fbSet(HI_U32 ureg_sscfifo_upper_thresh_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_fifo.bits.reg_sscfifo_upper_thresh_fb = ureg_sscfifo_upper_thresh_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32, "HDMITX_FBSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_upper_thresh_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_sscfifo_upper_thresh_fb
//  Input       : HI_U32 ureg_sscfifo_upper_thresh_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_upper_thresh_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_fifo.bits.reg_sscfifo_upper_thresh_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_lower_thresh_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_sscfifo_lower_thresh_fb
//  Input       : HI_U32 ureg_sscfifo_lower_thresh_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_lower_thresh_fbSet(HI_U32 ureg_sscfifo_lower_thresh_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_fifo.bits.reg_sscfifo_lower_thresh_fb = ureg_sscfifo_lower_thresh_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32, "HDMITX_FBSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_lower_thresh_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_sscfifo_lower_thresh_fb
//  Input       : HI_U32 ureg_sscfifo_lower_thresh_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_sscfifo_lower_thresh_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_fifo.bits.reg_sscfifo_lower_thresh_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_tune_step_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_tune_step_fb
//  Input       : HI_U32 ureg_tune_step_fb: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_tune_step_fbSet(HI_U32 ureg_tune_step_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_fifo.bits.reg_tune_step_fb = ureg_tune_step_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_fifo.u32, "HDMITX_FBSSC_FIFO");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_FIFO_reg_tune_step_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_FIFO.reg_tune_step_fb
//  Input       : HI_U32 ureg_tune_step_fb: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_FIFO_reg_tune_step_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_FIFO o_hdmitx_fbssc_fifo;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_FIFO.u32);
    o_hdmitx_fbssc_fifo.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_fifo.bits.reg_tune_step_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_testd_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_testd_fb
//  Input       : HI_U32 ureg_testd_fb: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_testd_fbSet(HI_U32 ureg_testd_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_step.bits.reg_testd_fb = ureg_testd_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32, "HDMITX_FBSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_testd_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_testd_fb
//  Input       : HI_U32 ureg_testd_fb: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_testd_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_step.bits.reg_testd_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_span_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_span_fb
//  Input       : HI_U32 ureg_span_fb: 14 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_span_fbSet(HI_U32 ureg_span_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_step.bits.reg_span_fb = ureg_span_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32, "HDMITX_FBSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_span_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_span_fb
//  Input       : HI_U32 ureg_span_fb: 14 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_span_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_step.bits.reg_span_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_step_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_step_fb
//  Input       : HI_U32 ureg_step_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_step_fbSet(HI_U32 ureg_step_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_step.bits.reg_step_fb = ureg_step_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_step.u32, "HDMITX_FBSSC_STEP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_STEP_reg_step_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_STEP.reg_step_fb
//  Input       : HI_U32 ureg_step_fb: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_STEP_reg_step_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_STEP o_hdmitx_fbssc_step;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_STEP.u32);
    o_hdmitx_fbssc_step.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_step.bits.reg_step_fb;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_TUNE_reg_tune_interval_fbSet
//  Description : Set the value of the member HDMITX_FBSSC_TUNE.reg_tune_interval_fb
//  Input       : HI_U32 ureg_tune_interval_fb: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_TUNE_reg_tune_interval_fbSet(HI_U32 ureg_tune_interval_fb)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_TUNE o_hdmitx_fbssc_tune;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_TUNE.u32);
    o_hdmitx_fbssc_tune.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmitx_fbssc_tune.bits.reg_tune_interval_fb = ureg_tune_interval_fb;
    HDMITXRegWrite(pu32RegAddr, o_hdmitx_fbssc_tune.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmitx_fbssc_tune.u32, "HDMITX_FBSSC_TUNE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMITX_FBSSC_TUNE_reg_tune_interval_fbGet
//  Description : Set the value of the member HDMITX_FBSSC_TUNE.reg_tune_interval_fb
//  Input       : HI_U32 ureg_tune_interval_fb: 32 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
HI_S32 HDMI_HDMITX_FBSSC_TUNE_reg_tune_interval_fbGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMITX_FBSSC_TUNE o_hdmitx_fbssc_tune;

    pu32RegAddr = (HI_U32*)&(gophdmi2tx_dphy_regAllReg->HDMITX_FBSSC_TUNE.u32);
    o_hdmitx_fbssc_tune.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmitx_fbssc_tune.bits.reg_tune_interval_fb;
}

