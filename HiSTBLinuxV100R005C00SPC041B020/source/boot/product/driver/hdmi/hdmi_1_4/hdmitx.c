/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
#include "linux/string.h"
#else
#include "delay.h"
#endif
#include "globdefs.h"
//#include "cpldefs.h"
#include "defstx.h"
#include "hdmitx.h"
#include "defstx.h"
#include "hlviic.h"
//#include "de.h"
//#include "eeprom.h"
#include "txvidp.h"
#include "infofrm.h"
//#include "hdcp.h"
//#include "vmode.h"
//#include "audio.h"
//#include "infpkttb.h"
#include "siitxapidefs.h"
//#include "edid.h"
//#include "global.h"
//#include "defsmddc.h"
//#include "mpi_hdmi.h"
#include "hi_unf_hdmi.h"
#include "phy.h"
#include "hi_drv_hdmi.h"
#include "hi_reg_common.h"
//---------------------------------------------------------------------
void DelayMS (HI_U16 count)
{
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    //usleep(count*1000);
     HI_U32 i = 0;

     while(i<count * 1000)
     {
          i++;
     }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    //msleep(count);
    udelay(count * 1000);
}

void Encoder_Adjust(void)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    TX_PHY_DVIEncoder(HI_TRUE);
    TX_CTRL_DVIEncoder(HI_FALSE);

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    TX_PHY_DVIEncoder(HI_FALSE);
    TX_CTRL_DVIEncoder(HI_TRUE);

#endif
}


HI_VOID HW_ResetCtrl(int iEnable)
{
    U_PERI_CRG67 crg67;

    crg67.u32 = g_pstRegCrg->PERI_CRG67.u32;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    if (iEnable == 0)
    {
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 0;
        crg67.bits.hdmitx_ctrl_srst_req     = 0;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
    }
    else
    {
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 1;
        crg67.bits.hdmitx_ctrl_srst_req     = 1;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
    }

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)

    if (iEnable == 0)
    {
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 0;
        crg67.bits.hdmitx_ctrl_srst_req     = 0;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        //crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
    }
    else
    {
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 1;
        crg67.bits.hdmitx_ctrl_srst_req     = 1;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        //crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
    }
#endif
    g_pstRegCrg->PERI_CRG67.u32 = crg67.u32;
}

HI_VOID HW_ResetPhy(int iEnable)
{
    U_PERI_CRG68 crg68;

    crg68.u32 = g_pstRegCrg->PERI_CRG68.u32;
    if (iEnable == 0)
    {
        crg68.bits.hdmitx_phy_bus_cken  = 1;
        crg68.bits.hdmitx_phy_srst_req  = 0;
    }
    else
    {
        crg68.bits.hdmitx_phy_bus_cken  = 0;
        crg68.bits.hdmitx_phy_srst_req  = 1;
    }
    g_pstRegCrg->PERI_CRG68.u32 = crg68.u32;
}

void HW_ResetHDMITX(void)
{
    HDMIPrint("--> HW_ResetHDMITX.\n");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    HdmiHardwareReset(1);
    DelayMS(18);
    HdmiHardwareReset(0);
    DelayMS(50);

    hdmiPhyInit();
    DelayMS(1);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    // The Procedures for Hw reset
    // HW Reset controller
    // Hw Reset Phy
    // delay 50us
    // Release Hw reset Phy
    // cfg Phy
    // Release HW reset for controller
    // Delay 5ms for pll stable

    //SI_HdmiHardwareReset(1);
    //udelay(100);
    HW_ResetCtrl(1);
    DelayMS(1);
    HW_ResetPhy(1);
    DelayMS(1);

    HW_ResetPhy(0);
    DelayMS(1);
    TX_PHY_INIT();
    DelayMS(1);

    HW_ResetCtrl(0);
    DelayMS(10);

    //if register isr,need config it
    WriteByteHDMITXP0 (INT_CNTRL_ADDR, 0x02);
    DelayMS(1);

    // 0x72:0xf6 DDC_DELAY_CNT
    WriteByteHDMITXP0(DDC_DELAY_CNT,0x1a);

    Encoder_Adjust();

    // Edge select:
    // 0 = Latch Input on Falling Edge
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
    //for 3716Cv200 series the clk is Edge need select to falling edge
    //the default value for 0x72:0x08 is 0x37,and the clock has been holded in falling edge
    //and after the clock edge is changed,we need a swrst.
    //otherwise, there will be cause clock unstable.
    HDMIPrint("Set 0x08:0x35 Hw Rst \n");
    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, 0x35);
#elif defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    HDMIPrint("Set 0x08:0x37 Hw Rst \n");
    //WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, 0x37);//default value 0x37,need not set
#endif
}

//-------------------------------------------------------------------
void AssertHDMITX_SWReset(HI_U8 SoftReset)
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0( TX_SWRST_ADDR );
    RegVal |= SoftReset;
    WriteByteHDMITXP0( TX_SWRST_ADDR, SoftReset);
}
//-------------------------------------------------------------------
void ReleaseHDMITX_SWReset( HI_U8 SoftReset )
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0( TX_SWRST_ADDR );
    RegVal &= (~SoftReset);
    WriteByteHDMITXP0( TX_SWRST_ADDR, 0);
}
//-------------------------------------------------------------------
void SW_ResetHDMITX(void)
{
    HI_U8 TimeOut = 100;
    HI_U8 RegVal;

    HDMIPrint("--> SW_ResetHDMITX.\n");

    //power down hdmi
    RegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);
    RegVal &= ~ BIT_TX_PD;
    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, RegVal);         // 0x35->0x37 GVG

    AssertHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
    //DelayMS(1);
    DelayMS(5);
    ReleaseHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
    DelayMS(1);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
    
    while ( !siiIsTClockStable() && --TimeOut )
    {
        DelayMS(1);         // wait for input pixel clock to stabilze
    }

    if (TimeOut == 0)
    {
    
        AssertHDMITX_SWReset(BIT_TX_SW_RST);
        DelayMS(10);
        ReleaseHDMITX_SWReset(BIT_TX_SW_RST);
        DelayMS(1);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
        HDMIPrint("TClock UnStable before sw reset <--\n");
    }
    //DelayMS(5);     // allow TCLK (sent to Rx across the HDMS link) to stabilize

    //DelayMS(1);

}
//-------------------------------------------------------------------
void WakeUpHDMITX(void)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR) | BIT_TX_PD;
    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, RegVal);         // 0x35->0x37 GVG
    WriteByteHDMITXP0 (INT_CNTRL_ADDR, INT_CONTROL);       // Interrupt pin control defined per board

}
//---------------------------------------------------------------------------
void SetIClk(HI_U8 IClk)
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) & CLR_BITS_1_0;
    WriteByteHDMITXP0( TX_VID_CTRL_ADDR, (RegVal | IClk) );
}

//--------------------------------------------------------------------------
void TX_SetHDMIMode(HI_U8 Enabled)
{
    HI_U8 RegVal;

    HDMIPrint("Set HDMI mode to %d.\n", Enabled);

    RegVal = ReadByteHDMITXP1(AUDP_TXCTRL_ADDR);
    if(Enabled)
    {
        WriteByteHDMITXP1(AUDP_TXCTRL_ADDR, RegVal | BIT_TXHDMI_MODE);
    }
    else
    {
        WriteByteHDMITXP1(AUDP_TXCTRL_ADDR, RegVal & (~BIT_TXHDMI_MODE));
    }
}

HI_S32 TX_CTRL_DVIEncoder(HI_BOOL bEnable)
{
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    HI_U32 u32Value;
    /* Taiyan Phy has no dvi encoder,
       so we will use ctrllor's encoder */
    u32Value = ReadByteHDMITXP1(TEST_TX_ADDR);
    if (bEnable)
    {
        u32Value &= ~BIT_DVI_ENC_BYPASS;
    }
    else
    {
        u32Value |= BIT_DVI_ENC_BYPASS;
    }
    HDMIPrint("SI_TX_CTRL_DVIEncoder write 0x%x \n",u32Value);
    WriteByteHDMITXP1(TEST_TX_ADDR,u32Value);
    //u32Value = ReadByteHDMITXP1(TEST_TX_ADDR);
#endif
    return HI_SUCCESS;
}

HI_U32 TX_RSEN_Status( void )
{
    return (GetSysStat() & (BIT_RSEN))? HI_TRUE : HI_FALSE;
}
