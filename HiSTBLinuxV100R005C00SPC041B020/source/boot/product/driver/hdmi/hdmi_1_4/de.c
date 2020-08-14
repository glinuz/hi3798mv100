/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "typedefs.h"
#include "timer.h"
#include "vmode.h"
#include "hdmitx.h"
//#include "eeprom.h"
#include "de.h"
//#include "eedefs.h"

extern const VModeInfoType VModeTables[NMODES];

//--------------------------------------------------------------------------
void UpdateTX_656(HI_U8 VMode)
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
#if 0 /*--Read only--*/
   WriteWordHDMITXP0( 0x3a, VModeTables[VMode].Tag.Total.Pixels);
   WriteWordHDMITXP0( 0x3c, VModeTables[VMode].Tag.Total.Lines);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

   //HDMIPrint("Vmode=%d. VxH=%dx%d@%d.\n", VMode, VModeTables[VMode].Res.V, VModeTables[VMode].Res.H, VModeTables[VMode].Tag.VFreq);
    
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

}
