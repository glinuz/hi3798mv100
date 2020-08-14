/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include "si_globdefs.h"
#include "si_cpldefs.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_defstx.h"
#include "si_hlviic.h"
#include "si_delay.h"
#include "si_de.h"
#include "si_eeprom.h"
#include "si_txvidp.h"
#include "si_infofrm.h"
#include "si_hdcp.h"
#include "si_vmode.h"
#include "si_audio.h"
#include "si_infpkttb.h"
#include "si_txapidefs.h"
#include "si_edid.h"
#include "si_global.h"
#include "si_defsmddc.h"
#include "si_mpi_hdmi.h"
#include "si_phy.h"
#include "hi_drv_sys.h"

#include "hi_reg_common.h"


Bool F_TxInit;

//---------------------------------------------------------------------
void SI_WriteBlockHDMITXP0(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data )
{
    I2CShortCommandType I2CComm;
    memset((void*)&I2CComm, 0, sizeof(I2CComm));

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;
    BlockWrite_8BAS((I2CShortCommandType *)&I2CComm, Data);
}

//---------------------------------------------------------------------
void SI_WriteBlockHDMITXP1(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data )
{
    I2CShortCommandType I2CComm;
    memset((void*)&I2CComm, 0, sizeof(I2CComm));

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV1;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;
    BlockWrite_8BAS((I2CShortCommandType *)&I2CComm, Data);
}
//---------------------------------------------------------------------
void SI_ReadBlockHDMITXP0(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data )
{
    I2CShortCommandType I2CComm;

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;
    BlockRead_8BAS((I2CShortCommandType *)&I2CComm, Data);
}

//---------------------------------------------------------------------
void SI_ReadBlockHDMITXP1(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data )
{
    I2CShortCommandType I2CComm;

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV1;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;
    BlockRead_8BAS((I2CShortCommandType *)&I2CComm, Data);
}

void SI_Encoder_Adjust(void)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    SI_TX_PHY_DVIEncoder(HI_TRUE);
    SI_TX_CTRL_DVIEncoder(HI_FALSE);

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    SI_TX_PHY_DVIEncoder(HI_FALSE);
    SI_TX_CTRL_DVIEncoder(HI_TRUE);

#endif
}

HI_VOID SI_HW_ResetCtrl(int iEnable)
{
    HI_U32    u32Ctrller;
    if (iEnable == 0)
    {
        u32Ctrller = 0x3f;
    }
#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
    else if (iEnable == 2)
    {
        u32Ctrller = 0x403f;
    }
#endif
    else
    {
        /*reset.*/
        u32Ctrller = 0x23f;
    }
    /* to be confirmed: clk and reset should be set one by one ,not together.*/
    g_pstRegCrg->PERI_CRG67.u32 = u32Ctrller;
}

HI_VOID SI_HW_ResetPhy(int iEnable)
{

    HI_U32    u32Phy;

    u32Phy = g_pstRegCrg->PERI_CRG68.u32;
    if (iEnable == 0)
    {
        u32Phy = 0x01;
    }
    else //reset
    {

        u32Phy = 0x10;
    }
    g_pstRegCrg->PERI_CRG68.u32 = u32Phy;

}


void SI_HW_ResetHDMITX(void)
{
    COM_INFO("--> SI_HW_ResetHDMITX.\n");
    //COM_INFO("new HW reset");


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
    SI_HW_ResetCtrl(1);
    // temp DelayMS(1);
    SI_HW_ResetPhy(1);
    // temp DelayMS(10);
    DelayMS(1);

    SI_HW_ResetPhy(0);
    // temp DelayMS(10);
    DelayMS(1);
    SI_TX_PHY_INIT();
    // temp DelayMS(10);
    DelayMS(1);

    SI_HW_ResetCtrl(0);
    // temp DelayMS(100);
    DelayMS(10);

    //if register isr,need config it
    WriteByteHDMITXP0 (INT_CNTRL_ADDR, 0x02);
    //DelayMS(1);

    // 0x72:0xf6 DDC_DELAY_CNT
    WriteByteHDMITXP0(DDC_DELAY_CNT,DRV_Get_DDCSpeed());

    SI_Encoder_Adjust();

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
    //for 3716Cv200 series the clk is Edge need select to falling edge
    //the default value for 0x72:0x08 is 0x37,and the clock has been holded in falling edge
    //and after the clock edge is changed,we need a swrst.
    //otherwise, there will be cause clock unstable.
    COM_INFO("TX_SYS_CTRL1_ADDR To 0x35 \n");
    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, 0x35);
#elif  defined(CHIP_TYPE_hi3718mv100) \
    || defined(CHIP_TYPE_hi3719mv100) \
    || defined(CHIP_TYPE_hi3796cv100) \
    || defined(CHIP_TYPE_hi3798cv100) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
    COM_INFO("TX_SYS_CTRL1_ADDR To 0x37 \n");
    //WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, 0x37);
#endif
}

HI_BOOL SI_HDMI_Setup_INBoot(HI_U32 *VIC)
{
    //check oe
    if (!SI_TX_PHY_GetOutPutEnable())
    {
        return HI_FALSE;
    }

    //HDMI HPD Flag
    if (!SI_HPD_Status())
        return HI_FALSE;

    //Read VIC form AVI Inforframe
    *VIC = SI_GetAVIInfoFrameVID();

    return HI_TRUE;
}

//-------------------------------------------------------------------
void SI_AssertHDMITX_SWReset(HI_U8 SoftReset)
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0( TX_SWRST_ADDR );
    RegVal |= SoftReset;
    WriteByteHDMITXP0( TX_SWRST_ADDR, SoftReset);
}
//-------------------------------------------------------------------
void SI_ReleaseHDMITX_SWReset( HI_U8 SoftReset )
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0( TX_SWRST_ADDR );
    RegVal &= (~SoftReset);
    WriteByteHDMITXP0( TX_SWRST_ADDR, 0);
}

HI_BOOL SI_IsHDMIResetting(void)
{
    HI_U32    u32Ctrller;
    HI_U32    u32Phy;

   u32Ctrller = g_pstRegCrg->PERI_CRG67.u32;
   u32Phy = g_pstRegCrg->PERI_CRG68.u32;

    if((ReadByteHDMITXP0( TX_SWRST_ADDR ) & 0x01 ) ||
       (u32Ctrller & 0x300) || (u32Phy & 0x10))
    {
        HI_INFO_HDMI("TX_SWRST:%x,CRG67:0x%x,CRG68:0x%x\n", ReadByteHDMITXP0( TX_SWRST_ADDR ), u32Ctrller, u32Phy);
        return HI_TRUE;
    }

    return HI_FALSE;
}

//-------------------------------------------------------------------
void SI_SW_ResetHDMITX(void)
{
    HI_U8 TimeOut = 50;

    COM_INFO("--> SI_SW_ResetHDMITX\n");
    while ( !siiIsTClockStable() && --TimeOut )
    {
        DelayMS(1);         // wait for input pixel clock to stabilze
    }

    if (TimeOut)
    {
        SI_AssertHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
        DelayMS(1);
        SI_ReleaseHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
        DelayMS(1);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
    }
    else
    {
        COM_INFO("SoftReset TMDS is not stable!!\n");
        SI_AssertHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
        DelayMS(1);
        SI_ReleaseHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
        DelayMS(1);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
    }
}

//-------------------------------------------------------------------
void SI_CheckClockStable(void)
{
    HI_U8 TimeOut = 20;
    //HI_U8 RegVal;

    COM_INFO("--> SI_CheckClockStable.\n");
    if(siiIsTClockStable())
    {
        COM_INFO("TClock Stable:%d <--\n", siiIsTClockStable());
        return;
    }

    SI_AssertHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
    DelayMS(5);
    SI_ReleaseHDMITX_SWReset(BIT_TX_SW_RST | BIT_TX_FIFO_RST);
    DelayMS(5);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
    
    while ( !siiIsTClockStable() && --TimeOut )
    {
        DelayMS(5);         // wait for input pixel clock to stabilze
    }

    if (TimeOut == 0)
    {
        SI_AssertHDMITX_SWReset(BIT_TX_SW_RST);
        DelayMS(10);
        SI_ReleaseHDMITX_SWReset(BIT_TX_SW_RST);
        DelayMS(5);          // allow TCLK (sent to Rx across the HDMS link) to stabilize
        COM_INFO("TClock Stable:%d before sw reset\n", siiIsTClockStable());
    }
    COM_INFO("SI_CheckClockStable. <--\n");

}

//-------------------------------------------------------------------
void SI_WakeUpHDMITX(void)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR) | BIT_TX_PD;

#if 0 /*--w00226427 said need cfg to 0x37--*/
    //x6等之前版本0x08默认值 0x34,S40 默认值0x37.不太确定做什么的，先和以往版本保持一致
    RegVal &=  ~BIT_TX_CLOCK_RISING_EDGE;
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, RegVal);         // 0x35->0x37 GVG
    WriteByteHDMITXP0 (INT_CNTRL_ADDR, INT_CONTROL);       // Interrupt pin control defined per board

}

void SI_PowerDownHdmiTx(void)
{
    HI_U8 RegVal;

    //power down hdmi
    RegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);
    RegVal &= ~ BIT_TX_PD;
    WriteByteHDMITXP0 (TX_SYS_CTRL1_ADDR, RegVal);         // 0x35->0x37 GVG
}

//--------------------------------------------------------------------------
void SI_WriteNValue(HI_U32 N)
{
    WriteWordHDMITXP1( N_SVAL_ADDR, (HI_U16) N & 0xFFFF);
    WriteByteHDMITXP1( N_SVAL_ADDR+2, (HI_U16)(N >> 16));
}

//---------------------------------------------------------------------------
void SI_SetMCLK(HI_U8 MCLKIndex)
{
    WriteByteHDMITXP1( FREQ_SVAL_ADDR, MCLKIndex);
}

//---------------------------------------------------------------------------
void SI_InteralMclkEnable(HI_U8 bEnableInternalClk)
{
    HI_U8 u8Value = 0;
    u8Value = ReadByteHDMITXP1( ACR_CTRL_ADDR);
    if(0x01 == (bEnableInternalClk & 0x01))
    {
        u8Value |= 0x04;
    }
    else
    {
        u8Value &= ~0x04;
    }
    WriteByteHDMITXP1( ACR_CTRL_ADDR, u8Value);
}

void SI_SetNCtsEnable(HI_U8 bEnableNCtsPkt)
{
    HI_U8 u8Value = 0;

    COM_INFO("Set N/CTS Packet %d \n",bEnableNCtsPkt);
    u8Value = ReadByteHDMITXP1( ACR_CTRL_ADDR);
    if(0x01 == (bEnableNCtsPkt & 0x01))
    {
        u8Value |= 0x02;
    }
    else
    {
        u8Value &= ~0x02;
    }
    WriteByteHDMITXP1( ACR_CTRL_ADDR, u8Value);
}

//---------------------------------------------------------------------------
void SI_SetIClk(HI_U8 IClk)
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) & CLR_BITS_1_0;
    WriteByteHDMITXP0( TX_VID_CTRL_ADDR, (RegVal | IClk) );
}

//---------------------------------------------------------------------------
HI_U8 SI_IsTXInHDMIMode( void )
{
    return BIT_TXHDMI_MODE & ReadByteHDMITXP1(AUDP_TXCTRL_ADDR);
}
//--------------------------------------------------------------------------
void SI_TX_SetHDMIMode(HI_U8 Enabled)
{
    HI_U8 RegVal;

    COM_INFO("Set HDMI mode to %d.\n", Enabled);

    RegVal = ReadByteHDMITXP1(AUDP_TXCTRL_ADDR);
    if(Enabled)
    {
        WriteByteHDMITXP1(AUDP_TXCTRL_ADDR, RegVal | BIT_TXHDMI_MODE);
    }
    else
    {
        WriteByteHDMITXP1(AUDP_TXCTRL_ADDR, RegVal & (~BIT_TXHDMI_MODE));
    }

    COM_INFO("AUDP_TXCTRL_ADDR:0x%x\n", ReadByteHDMITXP1(AUDP_TXCTRL_ADDR));
}

void SI_SetHdmiVideo(HI_U8 Enabled)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0(DATA_CTRL_ADDR);
    if(HI_TRUE == Enabled)
    {
        WriteByteHDMITXP0(DATA_CTRL_ADDR, RegVal & (~BIT_VID_BLANK));
    }
    else
    {
        WriteByteHDMITXP0(DATA_CTRL_ADDR, RegVal | BIT_VID_BLANK);
    }
}

void SI_SetHdmiAudio(HI_U8 Enabled)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0(DATA_CTRL_ADDR);
    if(TRUE == Enabled)
    {
        // because need reset fifo first
        // when fifo not reset，and set data as 0； maybe will interrupt compressed data
        WriteByteHDMITXP0(TX_SWRST_ADDR, 0x00);

        WriteByteHDMITXP0(DATA_CTRL_ADDR, RegVal & (~BIT_AUD_MUTE));
        //Normal operation Audio FIFO
        //SI_SetNCtsEnable(HI_TRUE);
    }
    else
    {
        WriteByteHDMITXP0(DATA_CTRL_ADDR, RegVal | BIT_AUD_MUTE);
        //Reset (flush) audio FIFO
        WriteByteHDMITXP0(TX_SWRST_ADDR, BIT_TX_FIFO_RST);
        //SI_SetNCtsEnable(HI_FALSE);
    }
}

//---------------------------------------------------------------------------
void SI_InitAudioPart( void )
{
    WriteByteHDMITXP1(AUD_MODE_ADDR, 0x03);     // En.SPDIF

    SI_WriteNValue(6144); /* clock set */

    //WriteByteHDMITXP1( ACR_CTRL_ADDR, 0x02);    // En. N/CTS packets
    WriteByteHDMITXP1( ACR_CTRL_ADDR, 0x06);    // fix by Q46153, should set MCLK_EN(bit2) to 1,  En. N/CTS packets

    // when Internal Mclk generator is enable , mclk index is invalid
    SI_InteralMclkEnable(SI_ReadByteEEPROM(EE_TX_INTERNAL_MCLK_ADDR));
    SI_SetMCLK(SI_ReadByteEEPROM(EE_TX_MCLK_INDEX_ADDR));
}

//-------------------------------------------------------------------------
static void InitTestCtrlReg(Bool FPGA)
{
    return ;
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    HI_U8 RegVal;

    if(FPGA)
    {
        RegVal = BIT_DVI_ENC_BYPASS;
        WriteByteHDMITXP1(TEST_TX_ADDR, RegVal);

        //HI_INFO_HDMI("InitTestCtrlReg bypass DVI encoder logic\n");
    }
    else
        WriteByteHDMITXP1(TEST_TX_ADDR, 0x00);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
}
//---------------------------------------------------------------------------
/*
When communicating with a DVI sink device, an HDMI Source shall operate in a mode
compatible with that device. This requires that the Source operate under the following limitations:
1. Video pixel encoding shall be RGB.
2. No Video Guard Bands shall be used.
3. No Data Islands shall be transmitted.

An HDMI Source, upon power-up, reset or detection of a new sink device, shall assume that the
sink device operates under DVI 1.0 limitations. An HDMI Source shall determine if the sink device
is an HDMI Sink by following the rules described in Section 8.3.3. Upon detection of an HDMI
Sink, the HDMI Source shall follow all of the HDMI Source-related requirements specified in this
document.

*/
#if defined (DVI_SUPPORT)
void SI_Init_DVITX(void)
{
    HI_U8 abData[4] = {0};
    HI_U8 bVideoMode;
    //HI_U8 bError;
#ifdef _9132_DIAG_PD_DEFAULT_    /// TRUE for Fred. FALSE for Dino
    HI_U8 RegVal;
#endif

    COM_INFO("Enter SI_Init_DVITX.\n");

    //SI_HW_ResetHDMITX(); lc:remove HW Reset
    //  TODO: Since HW reset was removed change color space back to RGB output
    //SI_SW_ResetHDMITX();

#ifdef _9132_DIAG_PD_DEFAULT_         /// TRUE for Fred. FALSE for Dino
    // Power Up internal oscillator; effectively enabling Master I2C and uploading of new vectors from EEPROM.
    RegVal = ReadByteHDMITXP1(DIAG_PD_ADDR);
    WriteByteHDMITXP1(DIAG_PD_ADDR, (RegVal & 0xFD));
    RegVal = ReadByteHDMITXP1(DIAG_PD_ADDR);
    COM_INFO("Power Up internal oscillator.\n");
#endif


    // FPLL is 1.0*IDCK.
    // Internal source termination enabled.
    // Driver level shifter bias enabled.
    //WriteByteHDMITXP0(TX_TMDS_CTRL_ADDR, 0x25);   //TX_TMDS_CTRL_ADDR has del in 1.4 ctrl


    SI_WakeUpHDMITX();
    SI_TX_SetHDMIMode(OFF);       // Reset to DVI mode

    bVideoMode = SI_ReadByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR);
    if(bVideoMode >= 100){
        HI_PRINT("index is out of bounds.\n");
        return ;
    }

    if(SI_ReadByteEEPROM(EE_TX_DE_ENABLED_ADDR)& 0x01)
    {
        SI_UpdateTX_DE(bVideoMode);
        SI_SetDE(ON);
    }
    else
        SI_UpdateTX_656(bVideoMode);

    SI_SetIClk( SI_ReadByteEEPROM(EE_TX_ICLK_ADDR) );

    SI_BlockReadEEPROM ( 4, EE_TX_VIDEOPATH_ADDR, abData );
    COM_INFO("SI_Init_DVITX to SI_SetVideoPath bVideoMode:0x%02x, abData[0]:0x%02x, abData[1]:0x%02x, abData[2]:0x%02x, abData[3]:0x%02x\n",
        bVideoMode, abData[0], abData[1], abData[2], abData[3]);
    SI_SetVideoPath ( bVideoMode, abData );



    SI_DisableInfoFrame(AVI_TYPE);
    SI_DisableInfoFrame(AUD_TYPE);

    ///    WriteByteHDMITXP0(TX_SYS_CTRL4_ADDR, 0x89);        per bugzilla #2991
    InitTestCtrlReg(SI_IsTXFPGA());
    SI_SetIClk(SI_ReadByteEEPROM(EE_TX_ICLK_ADDR));

    /* DVI mode must enable phy immediately */
    SI_EnableHdmiDevice();

    COM_INFO("[HDMITX.c](SI_Init_DVITX): Call SI_TMDS_setup()\n");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    bError = SI_TMDS_setup(bVideoMode);
    if (bError == TMDS_SETUP_FAILED)
    {
        HI_WARN_HDMI ("[HDMITX.C](SI_Init_DVITX): TMDS_SETUP_FAILED\n");
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    /* Enable DVI Interrupt:HPD */
    SI_InitDVIInterruptMasks();

    COM_INFO("<=== SI_Init_DVITX: Done.\n");

    // Enable Interrupts: VSync, Ri check, HotPlug
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);
    COM_INFO("HDMI_INT_MASK_ADDR:0x%x, CLR_MASK:0x%x\n", HDMI_INT_MASK_ADDR, CLR_MASK);

#if defined (HDCP_SUPPORT)

    if(TRUE == SI_IsTXHDCP_EEsetting())
    {
        AuthState = REQ_AUTHENTICATION;
    }
    else
    {
        AuthState = NO_HDCP;
        SI_SendCP_Packet(OFF);
        HDCP_INFO("NO_HDCP\n");
    }
#endif
}
#endif
//---------------------------------------------------------------------------
void SI_Start_HDMITX(void)
{
    HI_U8 abData[4];
    HI_U8 bVideoMode;


    F_TxInit = TRUE;

    COM_INFO("Start HDMI TX\n");
//    SI_SetHdmiVideo(HI_FALSE);/* Disable Video output */

    COM_INFO("Sink is HDMI Compatible!\n");

    SI_TX_SetHDMIMode(ON);    //for hdmi
    udelay(100);

    // FPLL is 1.0*IDCK.
    // Internal source termination enabled.
    // Driver level shifter bias enabled.
    //WriteByteHDMITXP0(TX_TMDS_CTRL_ADDR, 0x25);    //TX_TMDS_CTRL_ADDR has del in 1.4 ctrl
    //udelay(100);

    /* Video */
    bVideoMode = SI_ReadByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR);
#if defined (DVI_SUPPORT)
    if(bVideoMode >= 100){
        HI_PRINT("index is out of bounds.\n");
        return ;
    }
#else
    if(bVideoMode >= 60){
        HI_PRINT("index is out of bounds.\n");
        return ;
    }
#endif

    if(SI_ReadByteEEPROM(EE_TX_DE_ENABLED_ADDR)& 0x01)
    {
        SI_UpdateTX_DE(bVideoMode);
        udelay(50);
        SI_SetDE(ON);
    }
    else
    {
        SI_UpdateTX_656(bVideoMode);
    }
    udelay(200);
    SI_SetIClk( SI_ReadByteEEPROM(EE_TX_ICLK_ADDR) );
    memset(abData, 0, 4);
    SI_BlockReadEEPROM ( 4, EE_TX_VIDEOPATH_ADDR, abData );
    COM_INFO("SI_Start_HDMITX to SI_SetVideoPath bVideoMode:0x%02x, abData[0]:0x%02x, abData[1]:0x%02x, abData[2]:0x%02x, abData[3]:0x%02x\n",
        bVideoMode, abData[0], abData[1], abData[2], abData[3]);
    udelay(100);
    SI_SetVideoPath ( bVideoMode, abData );
    udelay(100);
    /* Audio */
    SI_EnableInfoFrame(AUD_TYPE);
    udelay(100);
    InitTestCtrlReg( SI_IsTXFPGA() );
    udelay(100);
    if (SI_ReadByteEEPROM(EE_AVIINFO_ENABLE))
    {
        COM_INFO(" avi infoframe send\n");
        SI_EnableInfoFrame(AVI_TYPE);
    }
    else
    {
        COM_INFO(" no avi infoframe send\n");
        SI_DisableInfoFrame(AVI_TYPE);
    }

    udelay(100);
    if (SI_ReadByteEEPROM(EE_AUDINFO_ENABLE))
    {
        COM_INFO("audio infoframe send\n");
        SI_EnableInfoFrame(AUD_TYPE);
    }
    else
    {
        COM_INFO("disable Audio inforfarme\n");
        SI_DisableInfoFrame(AUD_TYPE);
    }

    // Enable Interrupts: VSync, Ri check, HotPlug
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);
    udelay(100);
    SI_SetHdmiVideo(HI_TRUE);/* Remove Video Blanking */
    udelay(10);
    SI_SendCP_Packet(OFF);   /* Remove AVMute */
#if defined (HDCP_SUPPORT)
    if(TRUE == SI_IsTXHDCP_EEsetting())
    {
        SI_SendCP_Packet(ON);
        SI_SetEncryption(OFF);     // Must turn encryption off when AVMUTE

        AuthState = REQ_AUTHENTICATION;
        HDCP_INFO("\nBegin to HDCP Authenication!\n");
    }
    else
    {
        AuthState = NO_HDCP;
        HDCP_INFO("NO_HDCP\n");
    }
#endif
    //F_TxInit = FALSE; We directly to set SPDIF Fs!
    COM_INFO("<==== startHDMITX: Done\n");
}




HI_S32 SI_TX_IsHDMImode()
{
    HI_U32 u32Value = 0;

    u32Value = ReadByteHDMITXP1(AUDP_TXCTRL_ADDR);
    if (0x00000001 == (u32Value & 0x00000001))
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

HI_S32 SI_TX_CTRL_DVIEncoder(HI_BOOL bEnable)
{
#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    HI_U32 u32Value;
    /* Taiyan Phy has no dvi encoder,
       so we will use ctrllor's encoder */
    u32Value = ReadByteHDMITXP1(TEST_TX_ADDR);
    if(bEnable)
    {
        u32Value &= ~BIT_DVI_ENC_BYPASS;
    }
    else
    {
        u32Value |= BIT_DVI_ENC_BYPASS;
    }
    COM_INFO("SI_TX_CTRL_DVIEncoder write 0x%x \n",u32Value);
    WriteByteHDMITXP1(TEST_TX_ADDR,u32Value);
    //u32Value = ReadByteHDMITXP1(TEST_TX_ADDR);
#endif
    return HI_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              Tx Initialization Sequence
//
// DISCLAIMER: The following text describes, step by step, the Transmitter's initialization
// sequence. It might not be fully compatible with newer versions of the firmware. Wherever the
// code disagrees with the written description, the code should take precedence.
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note: The text will refer to both the 9034 and 9134 transmitters as "9134". However,
//  With few exceptions, the following description is for both the 9034and 9134 transmitters.
// The 9034 does not support Deep Color and HBRA audio, so any discussion of these topics
//  is related to the 9134 only.
//
//  9134 Main() Function
// ====================
//
//  Following power up, the FW initializes the MCU that it runs on by calling function InitMCU().
// It continues by initializing the 9134 transmitter chip by calling function InitSystem(). Once
//  these initializations are done, the FW falls into the main loop where it:
//
//  (i) monitors the Timer interrupts by calling SI_TimerHandler()
//
//  (ii) Invokes the SHA algorithm for the 2nd stage of the HDCP authentication if needs to do so.
//
// (iii) Runs the serial communications protocol between the FW and the external HDMIGear tool
//  by calling CommunicationHandler().
//
//  Optionally, HDMIGear may disable the monitoring functions of the FW and run just the serial
// communications by choosing the "Set Comm. Mode Only" option from its "Misc" tab. That allows
//  to manually set and read the 9134 registers as needed, without the FW interferring and changing
//  the manual settings.
//
// Throughout the code, calls to "HDMI_PRINT" send strings to the "Debugger" window of HDMIGear. That
// allows the developer to get a real time indication about the execution of the code.
//
//
//  9134 Initialization Sequence
// ============================
//
//
//  1. InitMCU() - 8051 register and pin initialization. Includes serial, timer and I2C bus initializations.
// See comments in file init_mcu.c
//
//  2. InitSystem() - Initialize the 9134 HDMI transmitter. It calls the following functions:
//
//  2.1 WaitForAckHDMITX( 7 ): Wait for ack on I2C bus's address 0x72, for T0 counts. If fails, there is no communication
//     between the MCU and the 9134. Initialization stops.
//
//  2.3 CheckVersioID(): Reads the on-board EEPROM version {0xA8:0x00}. If matches the value
//      #defined by the SiI_ID_EEPROM_VER constant (currently: 0x06), it means that the EEPROM does
//     not need further initializations. If they do not match:
//
//  2.4 SI_WriteDefaultConfigToEEPROM() is called, to write default initial values to the EEPROM. The default values
//     will be discussed below when used by other parts of the code.
//
//     The purpose of the EEPROM is to save the current settings of the FW so that upon the next power up the FW will
//      start with the same configuartion where is was when the previous session ended.
//
//      The FW always reads the setup parameters from the EEPROM. When HDMIGear sends a command to change a
//     parameter, the command is first written to the EEPROM, and read from there by the setup FW. That way there the
//     same code can be used to either read a new command or an old setup configuration that was previously written
//      to the EEPROM.
//
//      The EEPROM addresses and constants are listed under the "Config EEROM Addresses and Default Initialization"
//     section below.
//
//  2.5 InitSystem() calls ReadChipInfo(): Read 3 HI_U8s from registers {0x72:0x02..0x04} into ChipInf[] (Device ID + Rev. #).
//      Data is written to the Debugger window by calling HDMI_PRINT().
//
// 2.6 InitSystem() calls SI_Init_HDMITX() that does the actual 9134 initialization by performing hardware reset, setting audio
//      and video paths and parameters, performing software reset, setting up InfoFrames and resetting the SI_Authentication
//     and Output state machines to their initial states. SI_Init_HDMITX() sets global flag F_TxInit, that tess the FW that it is
//      in the initialization state. SI_Init_HDMITX() then calls the following functions:
//
//  2.6.1 SI_ReadByteEEPROM( EE_TX_DE_ENABLED_ADDR ) & 0x02 ): if EEPROM[0x91]:bit#1 == 1, DVI mode is forced by calling
//        SI_Init_DVITX():
//
// 2.6.2 SI_Init_DVITX(): The functions it calls are a subset of the functions that SI_Init_HDMITX() calls. They will not
//       be repeated here. See 2.6.3 and on.
//
//
//                                  - START OF SI_Init_HDMITX -
//
//
//  2.6.3 SI_HW_ResetHDMITX(): Sends an 18ms negative pulse from P0.1 of the MCU to the Reset pin of the 9134.
//
// 2.6.4 SI_ReadByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR): Identifies the input video format based on its
//       index in table VModeTables[] (file vmode.c). Saves in variable bVideoMode.
//
// 2.6.5 Sets register 0x72:0x82 to 0x25: Sets FPLL=1.0*IDCK; Sets Driver Level-Shifter bias and
//        internal termination per SiI recommendations.
//
//
//
//  2.6.6 SI_InitAudioPart() calls the following functions in order to initialize the audio part of the system:
//
// 2.6.6.1 SI_TX_SetHDMIMode(ON): Sets 0x7A:0x2F[0], to enable HDMI mode.
//
// 2.6.6.2 WriteByteHDMITXP1(AUD_MODE_ADDR, 0x03): Sets bits 0x7A:0x14[1:0], to enable audio in SPDIF mode.
//
// 2.6.6.3 WriteByteHDMITXP1( ACR_CTRL_ADDR, 0x02): sets bit 0x7A:0x01[1], to enable N/CTS packets.
//
//  2.6.6.4 SI_SetMCLK(SI_ReadByteEEPROM(EE_TX_MCLK_INDEX_ADDR)): Sets 0x7A:0x02[2:0] to the required multiple of
//          Fs. The value is taken from the EEPROM, which was previously written to by HDMIGear.
//
// 2.6.7 SI_ReadByteEEPROM(EE_TX_DE_ENABLED_ADDR)& 0x01: Check the value in EEPROM bit 0x91[0].
//        If 0x91[0] is "1" the internal DE generator is enabled, and the 9134 DE registers have to be set.
//       For that purpose SI_UpdateTX_DE() is called, with the input video format as a parameter (2.6.4):
//
// 2.6.8  SI_UpdateTX_DE() checks accesses VModeTables[] with the input video format (bVideoMode) as the table index,
//         and extracts the various parameters setup the DE registers:
//
//  2.6.8.1 0x72:0x48[4] is set for 720p/1080i/1080p (BT 709 conversion) and cleared for all other formats.
//
// 2.6.8.2 0x72:0x32..0x33 are set to VModeTables[bVideoMode].Pos.H (see definition of VModeInfoType in file si_vmode.h).
//
// 2.6.8.3 0x72:0x36..0x37 are set to VModeTables[bVideoMode].Res.H.
//
// 2.6.8.4 0x72:0x34 is set to VModeTables[bVideoMode].Pos.V.
//
//  2.6.8.5 0x72:0x38..0x39 are set to half the value of VModeTables[bVideoMode].Res.V if the input format is interlaced
//          (as indicated by VModeTables[bVideoMode].Tag.RefrTypeVHPol). If not interlaced, they are set to
//         VModeTables[bVideoMode].Res.V.
//
//  2.6.8.6 0x72:0x33 is modified for the correct horizontal and vertical polarities, as defined in
//         VModeTables[bVideoMode].Tag.RefrTypeVHPol.
//
// 2.6.9   SI_SetDE(ON) sets 0x72:0x33[6] to enable the DE generator after all registers have been set.
//
//  2.6.10  If 0x91[0] is "0", SI_UpdateTX_656() is called, with the input video format as a parameter (2.6.4):
//
// 2.6.10.1 0x72:0x3E[2] is cleared, as recommended by SiI. This must be done explicitly, because the default value
//          of 0x72:0x3E[2] is "1".
//
// 2.6.10.2 656 registers: 0x72:0x40..0x47 are loaded with the proper values from the VModeTables[VMode]._656
//           structure (see definition of _656Type in si_vmode.h).
//
//  2.6.10.3 0x72:0x48[4] is set for 720p/1080i/1080p (BT 709 conversion) and cleared for all other formats.
//
//  2.6.11   SI_SetIClk() sets 0x72:0x48[1:0] according to the input pixel replication rate, as sent from HDMIGear.
//
// 2.6.12   SI_BlockReadEEPROM (4, EE_TX_VIDEOPATH_ADDR, abData) reads 4 HI_U8s of video path from the EEPROM to local array
//           abData[]. The data read contains the following info:
//
//  abData[0] = Input Source : 0 - RGB24; 1 - RGB DVO 12; 2 - YCbCr24; ...
// abData[1] = Color Space Conversion (RGB input only): 0 - RGB out; 1 - YCbr (4:4:4) out
//  abData[2] = Clock Edge : 0 - Falling edge; 1 - Rising Edge
//  abData[3] = Deep Color Mode : 0 - 24 bit; 1 - 30 bits; 2 - 36 bits
//
// See file TXVPTbl.c for more details.
//
//  2.6.13  SI_Init_HDMITX() now calls SI_SetVideoPath() with bVideoMode (see 2.6.4) and abData[] as parameters:
//
//  2.6.13.1 SI_SetVideoPath() saves the input and output formats in global variables SiIVidPathInIndex
// and SiIVidPathOutIndex, respectively.
//
//  2.6.13.2 SI_SetVideoPath() calls SI_ConfigTXVideoMode() with the input and output formats as parameters.
//
// 2.6.13.2.1 SI_ConfigTXVideoMode() uses the input format parameter as a pointer to one of the specific arrays
//     defined in bidimensional array inMode[] (file TXVPTbl.c), that describe input formats, and the output format parameter
//             as an offset into that array. The value retrieved this way is used to set/clear bits 0x72:0x4A[5:0] to do the
//     color space conversion, up/down sampling, dithering and sync extraction as needed for the input-to-output
//         conversion.
//
//  2.6.13.2.2 SI_ConfigTXVideoMode() also sets 0x72:0x08[2] depending on the input bus width (that value is taken from
//         entry 0 of the inRGBDVO12[] array, defined as 1st element of the bidimensional inMode[] array).
//
// 2.6.13.3 SI_SetVideoPath() calls SetInputClockEdge() that sets 0x72:0x08[1] to select the polarity of
//   the input clock that will be latch the input data, as sent from HDMIGear.
//
// 2.6.13.4 SI_SetVideoPath() calls SI_SetDeepColor() with the Deep Color mode as a parameter (sent from
//       HDMIGear), to set the Deep Color registers. This is valid for the 9134 only.
//
//  2.6.13.4.1 SI_SetDeepColor() sets bits 0x72:0x49[7:6] according to the Deep Color mode - 00 for 24 bits, 01 for 30,
//         10 for 36.
//
// 2.6.13.4.2 SI_SetDeepColor() sets bits 0x7A:0x2F[4:3] according to the Deep Color mode, 0x7A:0x2F[5] to "1"
//     (needed for Deep Color) and 0x7A:0x2F[6] to enable Deep Color. If Deep Color mode is disabled.
//
// 2.6.13.4.3 SI_SetDeepColor() sets bits 0x72:4A[7:6] to dither the output according to the Deep Color mode,
//             and 0x72:0x4A[5] to enable dithering.
//
//  2.6.13.4.4 If Deep Color is disabled, SI_SetDeepColor() clears 0x7A:0x2F[6:5] and 0x72:4A[7:5].
//
//  2.6.13.5 SI_SetVideoPath() calls SI_TMDS_setup() to set the 9134 PLL registers as part of the
//          Deep Color settings.
//
//  2.6.13.5.1 SI_TMDS_setup() calls functions SI_Mpll_setup() and SI_FApost_setup(). These 3 functions
//     set registers 0x72:0x0C, 0x72:0x80, 0x72:0x82, 0x72:0x83, 0x72:0x84, 0x72:0x85 as a function of
//         the input resolution (taken from VModeTables) and the Deep color mode. The functions use an
//     algorithm that makes that process fast and efficient. The algorithm itself is not discussed here.
//         It is recommended that the user will use these 3 functions exactly as they are brought in the
//         reference code.
//
// 2.6.13.6 SI_SetVideoPath() saves the current state of the InfoFrame control registers,0x7A:3E..0x3F,then
//   calls SI_SW_ResetHDMITX(). That call is redundant when SI_SetVideoPath() is called from
//       SI_Init_HDMITX(), because SI_Init_HDMITX() explicitly calls SI_SW_ResetHDMITX() later. However, the call is included
//   in SI_SetVideoPath() because SI_SetVideoPath() may also be called from other locations in the FW, and SW
//       reset is needed whenever the input or internal clocks change.
//
//  2.6.13.6.1 SI_SW_ResetHDMITX() waits for the input pixel clock to stabilize with respect to the output of the
//       PLL by monitoring 0x72:0x09[0]. It then sets and clears 0x72:0x05[0], to reset the 9134 internal state
//       machines and audio FIFO.
//
// 2.6.13.6.2 SI_SetVideoPath() restores the state of the InfoFrame control registers that were cleared by the SW Reset.
//
// 2.6.14 SI_Init_HDMITX() reads 4 HI_U8s from EEPROM[0x98] into local array abData[]. These 4 HI_U8s carry the following
//         information:
//
//      abAudioPath[0] = Audio format:       0 - SPDIF;  1 - I2S;    2 - DSD;    3 - HBR
//                          Bit 7:      0 - Layout 0 (stereo);  1 - Layout 1 (up to 8 channels)
//      abAudioPath[1] = Sampling Freq Fs:  0 - 44KHz;   2 - 48KHz ...
//  abAudioPath[2] = Sample length : 2, 4, 8, 0xA, 0xC +1 for Max 24. Even values are Max 20. Odd: Max 24.
//  abAudioPath[3] = I2S control bits (for 0x7A:0x1D)
//
//         it then calls SI_Set_AudioInfoFramePacket() with abData[] as a parameter, together with parameters for speaker
//      allocation and level shift value, per EIA 861B, table 8-4.
//
//  2.6.14.1 SI_Set_AudioInfoFramePacket() prepares data HI_U8s 1-4 of the audio InfoFrame, per EIA861B, section 6.3,
//       that include information about the number of audio channels, sampling frequency and speaker mapping. It then
//       calls SI_TX_SendInfoFrame() with the Type (AUDIO) and the frame data HI_U8s as parameters.
//
// 2.6.14.1.1   SI_TX_SendInfoFrame() calls GetInfoFrameMapAddr() that loads parameter &InfoMap with the
//          bit pattern (0x30) needed by 0x7A:0x3E to send the audio InfoFrame repeatedly.
//
//  2.6.14.1.2  Calls GetRestOfInfoFrameHeader() to prepare the first 3 HI_U8s of the Audio InfoFrame header.
//
//  2.6.14.1.3  Writes the header and the data (passed in abData[]) to the 9134 Audio InfoFrame registers 0x7A:0x80..0x87.
//
//  2.6.14.1.4  Sets 0x7A:0x3E[5:4] as obtained in 2.6.14.1.1.
//
// 2.6.15       SI_Init_HDMITX() calls SI_SetAudioPath() with the same abData[] array as a parameter.
//
// 2.6.15.1 SI_SetAudioPath() uses its abData[] parameter to set the audio format and its parameters
//          by writing to registers 0x7a:0x14, 0x7A:0x1D and 0x7A:0x2F. It also sets Channel Status
//      registers 0x7A:0x21 and 0x7A:0x22 in order to send the proper audio status information to
//          the sink in modes other than SPDIF, and I2S_LEN register 0x7A:0x24 to tell the transmitter
//          what the length of the I2S input sample is.
//
// 2.6.16       SI_Init_HDMITX() call to InitTestCtrlReg() should be ignored
//
//  2.6.16      SI_Init_HDMITX() calls SI_SW_ResetHDMITX().
//
//  2.6.16.1    SI_SW_ResetHDMITX() checks for a stable input pixel clock, then sets and clears 0x72:0x05[0].
//      It is important that the SW Reset will be done when the input pixel clock is stable, AFTER
//          all audio and video settings were made and before the InfoFrames are enabled. See PA-0140
//          for a detailed discussion of the SW reset issue.
//
// 2.6.17       SI_Init_HDMITX() calls SI_SendAVIInfoFrame().
//
//  2.6.17.1    SI_SendAVIInfoFrame() reads 13 HI_U8s from EEPROM location 0xD0. The first
//      4 HI_U8s contain the follwoing data:
//
//      Data[0] = ColorSpace : 0 - RGB24;    1 - YC24;   2 - YCbCr24
//          Data[1] = PixRep     : 0 - x1;       1 - x2;     2 - x4  (meaning: "sent 1/2/4 times")
//          Data[2] = CSCType    : 0 - None;     1 - 601;    2 - 709
//          Data[3] = VIC (Video Identification Code): Per CEA-861, table 13.
//
//      and were written to the EEPROM either by function SI_WriteDefaultConfigToEEPROM() or following
//          a configuration command sent from HDMIGear.
//
//  2.6.17.2    SI_SendAVIInfoFrame() prepares the AVI InfoFrame header and checksum, and writes them to
//      0x7A:0x40..0x7A:0x4F (9134 AVI InfoFrrame registers), then sets 0x7A:0x3E[1:0], to send the
//          AVI InfoFrames repeatedly.
//
//  2.6.18      SI_Init_HDMITX() calls SI_WakeUpHDMITX() that takes Tx out of power down mode by setting 0x72:0x8[0]
//      to "1" and sets the interrupt pin assertion level to "1" by clearing 0x72:0x79[1].
//
//
// ************************************************************************************************************
//  2.6.19      SI_Init_HDMITX() calls SI_EnableInfoFrame(AVI_TYPE) that sets 0x7A:0x3E[1:0]
//
//  !!!!!!!!!!!!!!      THIS IS REDUNDANT. 0x7A:0x3E[1:0] WERE ALREADY SET BY SI_SendAVIInfoFrame() - 2.6.17.2.
//
//  ************************************************************************************************************
//
// 2.6.20       SI_Init_HDMITX() calls SI_SendCP_Packet(ON) to mute audio and video transmission.
//
//
//  2.6.21      SI_SendCP_Packet() disables the repetitive sending of the GCP by clearing 0x7A:0x3F[2], then sets
//      0x7A:0xDF to 0x01 to mute audio and video from reaching the receiver. It then monitors 0x7A:0x3F[3]
//          until it becomes "0". Then it re-enables the repetitive sending of GCP.
//
//          SEE the "General Control Packets" section of the 9134 PR regarding the EN bit.
//
// 2.6.22       SI_Init_HDMITX() calls SI_SetEncryption(OFF).
//
// 2.6.22.1 SI_SetEncryption() disables HDCP while AVMUTE by clearing 0x72:0x0F[0].
//
// 2.6.23       SI_Init_HDMITX() calls SysTimerInterrupts(0) to disable the MCU timer0 interrrupts.
//
//  ***************************************************************************************************************************
//
// 2.6.24       SI_Init_HDMITX() checks if the audio source is DVD audio, based on the value of EEPROM[0x98][0]. If yes, it
//      calls SI_SendInfoPacket() 3 times, in order to send Audio Contents protection and ISRC packets to the Rx. The
//          parameters passed to SI_SendInfoPacket() are a pointer to the packet header/data, the packet size and
//      the current power mode.
//
// 2.6.24.1 SI_SendInfoPacket() is similar to SI_TX_SendInfoFrame(). It copies the data passed to it to the proper
//          Tx InfoFrame register bank and sets the EN and RPT bit for that type of InfoFrame.
//
//
//
// !!!!!NOTEs:  1.
//          THIS CODE WILL NEVER EXECUTE, BECAUSE IT IS ONLY CALLED FROM SI_Init_HDMITX(), AND AFTER P. UP THE SYSTEM IS ALWAYS
//      SET TO SPDIF MODE (the "CHECK" will always fail).
//
//      Need to do the check from COMM.C as well, when audio mode is changed from HDMIGear.
//
//          2.
//          THE DATA SENT IN THE ACP AND ISRC1/2 PACKETS IS DUMMY DATA.
//
//      3.
//          Need to send ACP and ISRC InfoFrames for SACD and HBR audio as well.
//
//  ***************************************************************************************************************************
//
//
//
//
// 2.6.25       If the audio source is DVD audio SI_Init_HDMITX() calls SI_DisableInfoPacket(5) followed by SI_EnableInfoPacket(5),
//      that CLEARS, THEN sets the EN and RPT bits for the ISRC1 packets (0x7A:0x3F[1:0]).
//
//      SI_DisableInfoPacket() is currently not used by the FW. The purpose of calling it from SI_Init_HDMITX(), followed
//          by a call to SI_EnableInfoPacket() is to prevent linker error messages ("unused segment"). It is also important
//      for microcontroller with a limited RAM size, because uncalled segments cannot be overlaid.
//
//  2.6.26      SI_Init_HDMITX() calls SI_DisableInfoFrame(AUD_TYPE) followed by SI_EnableInfoFrame(AUD_TYPE), which toggle bits
//          0x7A:0x3E[5:4]. The reason for that is as explained in 2.6.25.
//
// 2.6.27       SI_Init_HDMITX() calls SysTimerInterrupts(1) to enable the MCU timer0 interrrupts.
//
// 2.6.28       SI_Init_HDMITX() clears the interrupt bits for HPD, Drop Sample and Biphase Error (register 0x72:0x71). It also
//          enables the corresponding mask bits in 0x72:0x75.
//
//  2.6.29      SI_Init_HDMITX() calls UpdateHPD_State().
//
//  2.6.29.1    UpdateHPD_State() loops while calling GetSysStat() that reads the hot plug state from 0x72:0x09, waits 100ms,
//      then checks if another HPD interrupt has occurred. If no HPD interrupt has occurred after the 100ms delay,
//      then the connector is in a stable state and the most recent reading of 0x72:0x09 is correct. In that case the
//          loop terminates and global variable OutputState is updated to either CABLE_PLUGIN_CHECK_EDID or CABLE_UNPLUG.
//
//  2.6.29.2    UpdateHPD_State() sets global variable AuthState to REAUTHENTATION_REQ. This is needed because UpdateHPD_State()
//      is also called following Hot Plug events while the system is running with the sink device authenticated.
//
//  2.6.29.3    UpdateHPD_State() calls SI_SendCP_Packet(1) that tells the receiver that no valid audio and video data is being sent
//          to it from the receiver (HDMI 1.1, section 5.3.6).
//
// 2.6.29.4 UpdateHPD_State() calls SI_SetEncryption(OFF) that clears 0x7A:0x0F[0] to disable HDCP encryption. Reason as in 2.6.30.2.
//
// 2.6.30       SI_Init_HDMITX() checks the value of EEPROM[0x96][0]. If set it updates AuthState to REQ_AUTHENTICATION (default). If not,
//          AuthState is set to NO_HDCP (which will cause the FW to avoid the authentication process and sene the data unprotected).
//
//  2.6.31      SI_Init_HDMITX ends by clearing flag F_TxInit.
//
//
//                              - END OF SI_Init_HDMITX -
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Config EEROM Addresses and Default Initialization
//  =================================================
//
// VER_ID_ADDR              0
//  SIIRX_VER_ID                1
//
// EE_EDID_PARSER_ADDR          0x8F        //  init to 0 (use short parser)
//  EE_TX_VIDEO_MODE_INDEX_ADDR     0x90        //  init to 1 (480p)
//  EE_TX_DE_ENABLED_ADDR           0x91        //  Write DE enable (1)
// EE_TX_ICLK_ADDR              0x92        //  Init to 0
//  EE_TX_MCLK_INDEX_ADDR           0x93        //  init to 0x04 (756Fs)
// EE_TX_FPGA               0x95        //  init to 0xFF
//  EE_TX_HDCP              0x96        //  init to 0XFF
//
// EE_TXAVIINFO_ADDR            0xD0        //  DefaultAVIInfoFrame: ROM const BYTE DefaultAVIInfoFrame[] = {
//                                                                  0x20, 0x00, 0x00, 0x00, //  Color Space YC24 ( YCbCr 4:2:2 ),
//                                                                  0x00, 0x00, 0x00, 0x00,
//                                                              0x00, 0x00, 0x00, 0x00,
//                                                                  0x00
//                                                              };
//
//  EE_TX_AUDIOPATH_ADDR            0x98        //  DefaultTXDVDAudio   ROM const BYTE DefaultTXDVDAudio[4] = {
//                                                                  0x00,           //  DVDA vs SPDIF
//                                                              _48kHz,         / Fs
//                                                                  _24Bits | LMax_Minus0,  //  Sample Length
//                                                                      0x40            //  Control reg Page 1, 0x1D
//
//  EE_TX_VIDEOPATH_ADDR            0xC0        //  DefaultTXVPath
//
// ROM const BYTE DefaultTXVPath[3] = { //  Tx Default video //  path
//                      0x03,   //  YC24 Input
//                  0x02,   //  YC24 Output
//                      0x01    //  falling edge input
//                     };
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
