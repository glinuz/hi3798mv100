/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_txvptbl.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_txvidp.h"
#include "si_txapidefs.h"
#include "si_vmode.h"
#include "si_hdcp.h"
#include "si_eedefs.h"
#include "si_eeprom.h"
#include "si_phy.h"
#include "drv_global.h"

HI_U8 SiIVidPathInIndex  = 2; /* inYCbCr24[] */
HI_U8 SiIVidPathOutIndex = 1; //YCbCr444

#define BYPASS 0x00
/* TODO */
/* Add code fo RBG to YCbCr Converter and Down Sampleing here */
/* tables to be modified                                      */
/* inRGB24[]                                                  */
/* inRGBDVO12[]                                               */
/* inYCbCr24[]                                                */

ROM const HI_U8 inRGB24[] = {
      BYPASS        // RGB24 out
};

ROM const HI_U8 inRGBDVO12[] = {
      TX_DVO                        // RGB DVO 12 out
};

ROM const HI_U8 inYCbCr24[] = {
      TX_YCbCrToRGB | TX_Dither,   // RGB24 out
      BYPASS                       // YCbCr24 Out
};
#if 0//lc change for Hisilicon
ROM const HI_U8 inYC24[] = {
      TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_422to444 | TX_Dither,                 // YCbCr24 out
      BYPASS                                   // YC24 out
};
#else
ROM const HI_U8 inYC24[] = {
      TX_DeMux | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_DeMux | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_DeMux | BYPASS                                   // YC24 out
};
#endif

ROM const HI_U8 inYCMuxed12[] = {
      TX_DeMux | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_DeMux | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_DeMux                                            // YC24 out
};

ROM const HI_U8 inYCMuxed656_12[] = {
      TX_DeMux | TX_SyncExtr | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_DeMux | TX_SyncExtr | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_DeMux | TX_SyncExtr                                            // YC24 out
};

ROM const HI_U8 inYC656_24[] = {
      TX_SyncExtr | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_SyncExtr | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_SyncExtr                                            // YC24 out
};

ROM const HI_U8 * const inMode[] = {
      inRGB24, inRGBDVO12, inYCbCr24, inYC24, inYCMuxed12, inYCMuxed656_12, inYC656_24
};

//In: 0 rgb444 1 ycbcr444 2 ycbcr422 3~6
//
HI_U8 SI_ConfigTXVideoMode(HI_U8 In, HI_U8 Out)
{

    HI_U8 TblVal, RegVal;
    
    TblVal = *(inMode[In] + Out);
    RegVal = ReadByteHDMITXP0(TX_VID_MODE_ADDR);
    if( TblVal & TX_DeMux)
        RegVal |=BIT_TX_DEMUX_YC;
    else
        RegVal &= (~BIT_TX_DEMUX_YC);
        
    if( TblVal & TX_SyncExtr)
        RegVal |=BIT_TX_SYNC_EXTR;
    else
        RegVal &= (~BIT_TX_SYNC_EXTR);
        
    if( TblVal & TX_YCbCrToRGB)
    {
        RegVal |= BIT_TX_CSC;
        //RegVal |= BIT_TX_16_235_RANGE;  // expand range
        RegVal &= (~BIT_TX_16_235_RANGE);
    }
    else
    {
        RegVal &= (~BIT_TX_CSC);
        RegVal &= (~BIT_TX_16_235_RANGE);
    }
    
    if( TblVal & TX_Dither)
        RegVal |=BIT_TX_DITHER;
    else
        RegVal &= (~BIT_TX_DITHER);
        
    if( TblVal & TX_422to444)
        RegVal |= BIT_TX_422to444;
    else
        RegVal &= (~BIT_TX_422to444);

    WriteByteHDMITXP0(TX_VID_MODE_ADDR, RegVal);
    COM_INFO("SI_ConfigTXVideoMode in:0x%02x: out:0x%02x, TblVal:0x%x, RegVal:0x%02x\n", In, Out, TblVal, RegVal);
    RegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);
    if( TblVal & TX_DVO)
        RegVal &= (~BIT_BSEL24BITS);
    else
        RegVal |= BIT_BSEL24BITS;
    WriteByteHDMITXP0(TX_SYS_CTRL1_ADDR, RegVal);

    /* TODO */
    /* Add code fo RBG to YCbCr Converter and Down Sampleing here */
    /* In/Out 0 RGB444,1 YUV444,2 YUV422 */
    RegVal = ReadByteHDMITXP0(VID_ACEN_ADDR);
    if(In == 0 && Out == 1)
    {
        RegVal |= BIT_VID_ACEN_RGB2YCBCR;        
    }
    else
    {
        RegVal &= (~BIT_VID_ACEN_RGB2YCBCR);
    }
    WriteByteHDMITXP0(VID_ACEN_ADDR, RegVal);
    
    return TblVal;

}
HI_BOOL SI_PixelRepeated(void)
{
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(HI_UNF_HDMI_ID_0);
    return DRV_Get_IsPixelRepeatFmt(pstVidAttr->enVideoFmt);
}
//---------------------------------------------------------------------------
// Function Name: SI_SetDeepColor
// Function Description:  This function sets Deep Color
//---------------------------------------------------------------------------
void SI_SetDeepColor ( HI_U8 bDeepColor )
{
    HI_U8 bRegVal, bTmpDeepColor;
    HI_U8 bRegVal2;
    HI_U32 u32Value = 0;

    //pw down phy is similar to pll reset
    SI_TX_PHY_PowerDown(HI_TRUE);
    
    bTmpDeepColor = bDeepColor;
    if (bTmpDeepColor == SiI_DeepColor_Off)
    {
        bDeepColor = SiI_DeepColor_24bit;       // Setup everything as 24bpp but do not turn deep color on
    }

    // VID_ACEN_ADDR
    //  [7:6] - Wide Bus
    //          0b00 = 24 bits
    //          0b01 = 30 bits
    //          0b10 = 36 bits
    //bRegVal = (ReadByteHDMITXP0 ( VID_ACEN_ADDR ) & VID_ACEN_DEEP_COLOR_CLR);	
    bRegVal = (ReadByteHDMITXP0 ( VID_IN_MODE ) & VIN_MODE_CLR);	
    
    /* we give a fixed input bit widths value of 12bit for every channel*/
    bRegVal |= 0x06;

#ifndef _FPGA_FRED2_

    WriteByteHDMITXP0 (VID_IN_MODE,  bRegVal);
#else

    //lc change WriteByteHDMITXP0 (VID_ACEN_ADDR,  (bRegVal | 0x80));
    COM_INFO("try to write VID_ACEN_ADDR:0x%02x, data:0x%02x\n", VID_IN_MODE, bRegVal);
    WriteByteHDMITXP0 (VID_IN_MODE,  bRegVal);
#endif

    COM_INFO("[TXVIDP.C](SI_SetDeepColor): Set Deep Color VID_ACEN_ADDR bDeepColor:%d\n", bDeepColor);

    // AUDP_TXCTRL_ADDR
    //  [5:3] ?PACKET_MODE
    //          0b100 = 24 bits
    //          0b101 = 30 bits
    //          0b110 = 36 bits
    // Set / Clear bit 5 separately below.
    bRegVal = (ReadByteHDMITXP1 ( AUDP_TXCTRL_ADDR ) & 0xE7);
    bRegVal |= (bDeepColor << 3);

    if ((bTmpDeepColor != SiI_DeepColor_Off) && (bTmpDeepColor != SiI_DeepColor_24bit))
    {
        bRegVal |= BIT_DEEPCOLOR_EN;
        bRegVal |= 0x20;

        // Enable dithering and set Dither Mode for Deep Color:
        bRegVal2 = (ReadByteHDMITXP0 ( TX_VID_MODE_ADDR ) & CLR_BITS_7_6_5);
        bRegVal2 |= (bDeepColor << 6);
        bRegVal2 |= BIT_DITHER_EN;
        u32Value = 0; //disable vid_dither 0x4F
    }
    else
    {
        bRegVal &= (~BIT_DEEPCOLOR_EN);
       // bRegVal &= (~0x20);
        bRegVal |= 0x20;
        bRegVal2 = (ReadByteHDMITXP0 ( TX_VID_MODE_ADDR ) & CLR_BITS_7_6_5);    // Disable dithering if not DC
        if(!SI_PixelRepeated())
        {
            bRegVal2 |= BIT_DITHER_EN;
            u32Value = 0xe; //Enable D_RC_EN, D_GC_EN, D_BC_EN
        }
    }
    /*set deep color */
    WriteByteHDMITXP0 (  TX_VID_DITHER, u32Value );
    WriteByteHDMITXP1 (  AUDP_TXCTRL_ADDR, bRegVal );
    COM_INFO("[TXVIDP.C](SI_SetDeepColor): Set Deep Color AUDP_TXCTRL_ADDR:0x%02x, data:0x%02x\n", AUDP_TXCTRL_ADDR, bRegVal);
    WriteByteHDMITXP0 (  TX_VID_MODE_ADDR, bRegVal2 );
    COM_INFO("[TXVIDP.C](SI_SetDeepColor): Set Deep Color TX_VID_MODE_ADDR:0x%02x, data:0x%02x\n", TX_VID_MODE_ADDR, bRegVal2);

    SI_TX_PHY_SetDeepColor(bDeepColor);
    
    if (HI_TRUE == SI_HPD_Status())
    {
        SI_TX_PHY_PowerDown(HI_FALSE);
        //DelayMS(1);
    }

    
    return;  
}
//---------------------------------------------------------------------------
// Function Name: SetInputClockEdge
// Function Description:  This function sets Input Clock Edge
//---------------------------------------------------------------------------

static void SetInputClockEdge( HI_U8 bOn )
{
    HI_U8 bRegVal;

    bRegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);
    if( bOn )
        bRegVal |= BIT_TX_CLOCK_RISING_EDGE;
    else
        bRegVal &= (~BIT_TX_CLOCK_RISING_EDGE);
    WriteByteHDMITXP0(TX_SYS_CTRL1_ADDR, bRegVal);

}
//------------------------------------------------------------------------------
// Function Name: SI_SetVideoPath
// Function Description: This functiom sets video path, for for different Input/Output
//                       Video formats, it doesn't include setting Resolution dependent parameters
// Accepts: poinetr on Video path parameters
// Returns: none
// Globals: none
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////
// abVideoPath[0] = Input Source : 0 - RGB24; 1 - RGB DVO 12; 2 - YCbCr24; 3 - YC24; 4 - YCbCr4:2:2
// abVideoPath[1] = Color Space Conversion : 0 - RGB out; 1 - YCbr (4:4:4) out
// abVideoPath[2] = Clock Edge : 0 - Falling edge; 1 - Rising Edge
// abVideoPath[3] = Deep Color Mode : 0 - 24 bit; 1 - 30 bits; 2 - 36 bits
////////////////////////////////////////////////////////////////////////////////////////////



void SI_SetVideoPath ( HI_U8 bVMode, HI_U8 * abVideoPath )
{
    //HI_U8 bError;
    static HI_U8 InfCtrl1, InfCtrl2;
    COM_INFO("SI_SetVideoPath bVMode:0x%02x\n", bVMode);
    SI_BlockWriteEEPROM( 4, EE_TX_VIDEOPATH_ADDR , abVideoPath);

    SiIVidPathInIndex = abVideoPath[0];
    SiIVidPathOutIndex = abVideoPath[1];
    COM_INFO("SI_SetVideoPath setting video path:0x%02x,0x%02x,0x%02x,0x%02x\n", abVideoPath[0], abVideoPath[1], abVideoPath[2], abVideoPath[3]);
    SI_ConfigTXVideoMode( abVideoPath[0], abVideoPath[1]);
    SetInputClockEdge( abVideoPath[2] );
    SI_SetDeepColor ( abVideoPath[3] );


    InfCtrl1 = ReadByteHDMITXP1(INF_CTRL1);    // save packet buffer control regs
    InfCtrl2 = ReadByteHDMITXP1(INF_CTRL2);
    //DEBUG_PRINTK("SI_SetVideoPath to do SI_SW_ResetHDMITX\n");
    //SI_SW_ResetHDMITX();               // Reset internal state machines and allow TCLK to Rx to stabilize

    WriteByteHDMITXP1(INF_CTRL1, InfCtrl1);        // Retrieve packet buffer control regs
    WriteByteHDMITXP1(INF_CTRL2, InfCtrl2);
    COM_INFO("SI_SetVideoPath end\n");
#if defined (HDCP_SUPPORT)
    SI_PrintAuthState();
#endif
}
//------------------------------------------------------------------------------
// Function Name: SI_GetVideoPath
// Function Description: This functiom gets video path, for for different Input/Output
//                       Video formats, it doesn't include setting Resolution dependent parameters
// Accepts: none
// Returns: poinetr on Video path parameters
// Globals: none
//------------------------------------------------------------------------------
void SI_GetVideoPath ( HI_U8 * abVideoPath )
{
    HI_U8 bRegVal;
    COM_INFO("SI_GetVideoPath\n");
    abVideoPath[0] = SiIVidPathInIndex;
    abVideoPath[1] = SiIVidPathOutIndex;

    bRegVal = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);
    abVideoPath[2] = 0;
    if( bRegVal & BIT_TX_CLOCK_RISING_EDGE )
        abVideoPath[2]|= SiI_RisingVideoClockEdge;

    bRegVal = ReadByteHDMITXP1 (  AUDP_TXCTRL_ADDR );
    if (!(bRegVal & BIT_DEEPCOLOR_EN))
    {
        abVideoPath[3] = 0xFF;
    }
    else
    {
        abVideoPath[3] = bRegVal >> 3;
        abVideoPath[3] &= 0x03;         // extract Deep Color  bits
    }
}

void SI_TX_InvertSyncPol(HI_BOOL bInvert)
{
    HI_U8 RegVal = 0;
    
    RegVal = ReadByteHDMITXP0(DE_CNTRL_ADDR);
    COM_INFO("DE_CNTRL_ADDR:0x%x, Before RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
    if(bInvert)
    {
        RegVal |= 0x30;
        COM_INFO("Neagtiv Polarity DE_CNTRL_ADDR:0x%x, change RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
    }
    else
    {
        RegVal &= (~0x30);
        COM_INFO("Positive Polarity DE_CNTRL_ADDR:0x%x, change RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
    }
    WriteByteHDMITXP0(DE_CNTRL_ADDR,RegVal);    
}

void SI_TX_CSC709Select(HI_BOOL bSpace709)
{
    HI_U8 RegVal = 0;

    RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR);
    COM_INFO("TX_VID_CTRL_ADDR:0x%x, Before RegVal:0x%x\n", TX_VID_CTRL_ADDR, RegVal);

    if(bSpace709)
    {
        RegVal |= BIT_VID_CTRL_CSCSEL;
        COM_INFO("709 Standard TX_VID_CTRL_ADDR:0x%x, change RegVal:0x%x\n", TX_VID_CTRL_ADDR, RegVal);
    }
    else
    {
        RegVal &= (~BIT_VID_CTRL_CSCSEL);
        COM_INFO("601 Standard TX_VID_CTRL_ADDR:0x%x, change RegVal:0x%x\n", TX_VID_CTRL_ADDR, RegVal);
    }
    WriteByteHDMITXP0(TX_VID_CTRL_ADDR,RegVal);

}
