/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_timer.h"
#include "si_vmode.h"
#include "si_delay.h"
#include "si_video.h"

extern HI_U8 SysState;
extern ROM const VModeInfoType VModeTables[NMODES];
extern SyncInfoType SyncInfo;

int si_abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

/*
//--------------------------------------------------------
HI_U16   GetPixelClock(HI_U8 Index){
   return  VModeTables[Index].PixClk;
}
*/
//--------------------------------------------------------
static HI_U8 CompareWithVMTableRef(HI_U8 Index, HI_U16 ClksForLine){


   if(VModeTables[Index].Tag.RefrTypeVHPol!=SyncInfo.RefrTypeVHPol)
      return 0;
//
//  as polaraty is not implemented at this FPGA


   if(si_abs(VModeTables[Index].Tag.VFreq - SyncInfo.VFreq ) > V_TOLERANCE)
         return 0;
   if(si_abs(VModeTables[Index].Tag.Total.Lines - SyncInfo.HTot)>15)
         return 0;
   if(si_abs(VModeTables[Index].Tag.Total.Pixels -  ClksForLine)>4)
         return 0;
   return 1;

}
//-----------------------------------------------------------------------
//      Compare timing ifor with the vmode reference table, but polarity information is ignored
//-----------------------------------------------------------------------

static HI_U8 CompareWithVMTableRefIgnorPol(HI_U8 Index, HI_U16 ClksForLine){

HI_U16 HSyncsPerFrame;

   if((VModeTables[Index].Tag.RefrTypeVHPol & 0xFC)!=(SyncInfo.RefrTypeVHPol & 0xFC))
      return 0;
//
//  as polaraty is not implemented at this FPGA

   HSyncsPerFrame =  SyncInfo.HTot;

   if(si_abs(VModeTables[Index].Tag.VFreq - SyncInfo.VFreq ) > V_TOLERANCE)
         return 0;
   if(si_abs(VModeTables[Index].Tag.Total.Lines - HSyncsPerFrame)>15)
         return 0;
   if(si_abs(VModeTables[Index].Tag.Total.Pixels -  ClksForLine)>4)
         return 0;
   return 1;

}
//---------------------------------------------------------
void SI_GetVModeData(HI_U8 Index, HI_U8 * Data) {
   Data[0] = VModeTables[Index].ModeId.Mode_C1;
   Data[1] = VModeTables[Index].ModeId.Mode_C2;
   Data[2] = VModeTables[Index].ModeId.SubMode;
   Data[3] = VModeTables[Index].Res.H & 0xff;
   Data[4] = VModeTables[Index].Res.H >> 8;
   Data[5] = VModeTables[Index].Res.V & 0xff;
   Data[6] = VModeTables[Index].Res.V >> 8;

}

//---------------------------------------------------------
HI_U8 SI_VideoModeDetection(HI_U8 * Index)
{
    HI_U8 Res = 0;
    HI_U16 ClocksForLine;

    ClocksForLine = SI_GetPixelsForHSync();

    for((*Index) = 0; (*Index) < NMODES; (*Index)++)
    {
        if(CompareWithVMTableRef(*Index, ClocksForLine))
        {   Res  = 1;
            break;
        }
    }
  
    if((*Index) == NMODES)
    {
        //Preset mode not found, search ignoring polarity then:
        for((*Index) = 0; (*Index) < NMODES; (*Index)++)
        {
             Res  = 2;
             if(CompareWithVMTableRefIgnorPol(*Index, ClocksForLine))
                    break;
        }
    }
    return Res;
}



