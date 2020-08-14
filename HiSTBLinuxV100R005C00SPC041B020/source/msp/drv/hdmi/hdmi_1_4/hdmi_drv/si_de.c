/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_timer.h"
#include "si_vmode.h"
#include "si_hdmitx.h"
#include "si_eeprom.h"
#include "si_de.h"
#include "si_eedefs.h"

extern const VModeInfoType VModeTables[NMODES];

//-------------------------------------------------------------------------
void SI_UpdateTX_DE(HI_U8 VMode)
{
    HI_U16 HStartPos, VStartPos; HI_U8 Polarity, RegVal;
    HI_U16 HRes, VRes;
    HI_U8 Interlace;
    HI_U8 CustVMOffset;

    if(VMode & 0x80)
    {      // Custom vmodes, DATA are taken from EEPROM
        CustVMOffset = ( VMode & 0x03 ) * CUST_MODE_SIZE;
        Polarity = ReadEECVMPol(CustVMOffset);
        Interlace = Polarity & 0x40;
        Polarity =(~Polarity)& 0x03;
        HStartPos = ReadEEHStartPos(CustVMOffset);
        VStartPos = ReadEEVStartPos(CustVMOffset);
        HRes = ReadEEHRes(CustVMOffset);
        VRes = ReadEEVRes(CustVMOffset);
        COM_INFO("VxH=%dx%d@Custom.\n", VRes, HRes);
    }
    else
    {
        Polarity =(~VModeTables[VMode].Tag.RefrTypeVHPol )& 0x03; //   IY - 10/13/06
        Interlace = VModeTables[VMode].Tag.RefrTypeVHPol & 0x04;
        HStartPos = VModeTables[VMode].Pos.H;
        VStartPos = VModeTables[VMode].Pos.V;
        HRes = VModeTables[VMode].Res.H;
        VRes = VModeTables[VMode].Res.V;
        COM_INFO("VxH=%dx%d@%d.\n", VRes, HRes, VModeTables[VMode].Tag.VFreq);
    }

    COM_INFO("VxH=%dx%d.\n", VRes, HRes);
    // If HD (720p/1080i/1080p), set CSCSEL bit at 0x72:0x48[4]:
   if ((VMode == 2) || (VMode == 3) || (VMode == 11) || (VMode == 13) || (VMode == 14) ||
        ((VMode >= 24) && (VMode <= 27)))
   {
         RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) | SET_BIT_CSCSEL;
   }
   else  // SD. Clear CSCSEL bit
   {
         RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) & ~SET_BIT_CSCSEL;
   }
   
   WriteByteHDMITXP0( TX_VID_CTRL_ADDR, RegVal);

   WriteByteHDMITXP0(DE_HSTART_ADDR, HStartPos & 0xFF);
   WriteByteHDMITXP0(DE_CNTRL_ADDR, (HStartPos>>8) & 0x03);
   WriteWordHDMITXP0(DE_HRES_ADDR, HRes);
   WriteByteHDMITXP0(DE_VSTART_ADDR, VStartPos);
   
   if(Interlace ) // interlace
       WriteWordHDMITXP0(DE_VRES_ADDR,VRes >> 1 );
   else
       WriteWordHDMITXP0(DE_VRES_ADDR,VRes);
       
   RegVal = ReadByteHDMITXP0(DE_CNTRL_ADDR);
   WriteByteHDMITXP0(DE_CNTRL_ADDR,(( Polarity << 4 )| RegVal));
}


//--------------------------------------------------------------------------
void SI_UpdateTX_656(HI_U8 VMode)
{
    HI_U8 RegVal;

//    {{ 4, 0,NSM},           7417,   {ProgrVPosHPos,     6000,   {1650,750}},    {260, 25},  {1280,720},     _16,    
//  {0,40,5,20,220,110,5,0},        0}, // 4   1280 x 720p
/*
  HI_U8 IntAdjMode;
  HI_U16 HLength;
  HI_U16 VLength;
  HI_U16 Top;
  HI_U16 Dly;
  HI_U16 HBit2HSync;
  HI_U16 VBit2VSync;
  HI_U16 Field2Offset;
  */
  
   RegVal = ReadByteHDMITXP0( INTERLACE_ADJ_MODE) & 0xF8;
   WriteByteHDMITXP0( INTERLACE_ADJ_MODE, RegVal | (VModeTables[VMode]._656.IntAdjMode & ~SET_DE_ADJ_BIT));

   WriteWordHDMITXP0( HBIT_TO_HSYNC_ADDR,  VModeTables[VMode]._656.HBit2HSync);
   WriteWordHDMITXP0( FIELD2_HSYNC_OFFSET_ADDR, VModeTables[VMode]._656.Field2Offset);
   WriteWordHDMITXP0( HLENGTH_ADDR, VModeTables[VMode]._656.HLength);
   WriteByteHDMITXP0( VBIT_TO_VSYNC_ADDR,  VModeTables[VMode]._656.VBit2VSync);
   WriteByteHDMITXP0( VLENGTH_ADDR, VModeTables[VMode]._656.VLength);
   WriteWordHDMITXP0( 0x3a, VModeTables[VMode].Tag.Total.Pixels);
   WriteWordHDMITXP0( 0x3c, VModeTables[VMode].Tag.Total.Lines);

   COM_INFO("Vmode=%d. VxH=%dx%d@%d.\n", VMode, VModeTables[VMode].Res.V, VModeTables[VMode].Res.H, VModeTables[VMode].Tag.VFreq);

#if 0 /*--SetCsc in setAttr--*/
   //if HD, set CSCSEL bit at 0x72:0x48[4]:
   if ((VMode == 2) || (VMode == 3) || (VMode == 11) || (VMode == 13) || (VMode == 14) ||
        ((VMode >= 24) && (VMode <= 27)))
   {
         RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) | SET_BIT_CSCSEL;
   }
   else  // No HD. Clear CSCSEL bit
   {
         RegVal = ReadByteHDMITXP0(TX_VID_CTRL_ADDR) & ~SET_BIT_CSCSEL;
   }
   
   WriteByteHDMITXP0( TX_VID_CTRL_ADDR, RegVal);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

}
//--------------------------------------------------------------------------
void SI_SetDE(HI_U8 DE_Enabled)
{
    HI_U8 RegVal;

   RegVal = ReadByteHDMITXP0(DE_CNTRL_ADDR);
   if(DE_Enabled)
       RegVal |= BIT_DE_ENABLED;
   else
       RegVal &= (~BIT_DE_ENABLED);
   WriteByteHDMITXP0(DE_CNTRL_ADDR,RegVal);

}

