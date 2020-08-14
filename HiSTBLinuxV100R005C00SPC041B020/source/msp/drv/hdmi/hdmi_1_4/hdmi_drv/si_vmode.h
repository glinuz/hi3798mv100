/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_timer.h"

//                  Aspect ratio
#define _4     1  // 4:3
#define _16    2  // 16:9
#define _4or16   3  // 16:9
#define _16_10   4  // 16:10
#define _5_4   5  // 5:4



#define NSM    0 // no sub. mode

#define PC_BASE 60

#if defined (DVI_SUPPORT)
#define NMODES 100
#else
#define NMODES PC_BASE
#endif

#define VICMODES 5

void SI_GetVModeData(HI_U8, HI_U8 *);
HI_U8 SI_VideoModeDetection(HI_U8 *);
void VMode_Handler(void);
HI_U16 GetPixelClock( HI_U8 );

//----------------------------------------------------------
typedef struct {
        HI_U8 FPixTMDS;
        HI_U16 Fs[3];
} PixPerSPDIFType;
//----------------------------------------------------------
typedef struct {
        HI_U16 FPixTMDS;
        HI_U32 Fs[7];

} NValType;
//----------------------------------------------------------

typedef struct {
   HI_U8 Mode_C1;
   HI_U8 Mode_C2;
   HI_U8 SubMode;
   HI_UNF_ENC_FMT_E  enUNFFmt_C1;
   HI_UNF_ENC_FMT_E  enUNFFmt_C2;
}ModeIdType;

typedef struct {
   HI_U16 Pixels;
   HI_U16 Lines;
} PxlLnTotalType;

typedef struct {
   HI_U16 H;
   HI_U16 V;
} HVPositionType;

typedef struct {
   HI_U16 H;
   HI_U16 V;
} HVResolutionType;

typedef struct{
   HI_U8 RefrTypeVHPol;
   HI_U16 VFreq;
   PxlLnTotalType Total;
} TagType;

typedef struct {
  HI_U8 IntAdjMode;
  HI_U16 HLength;
  HI_U16 VLength;
  HI_U16 Top;
  HI_U16 Dly;
  HI_U16 HBit2HSync;
  HI_U16 VBit2VSync;
  HI_U16 Field2Offset;

}  _656Type;

typedef struct {
   ModeIdType ModeId;
   HI_U16 PixClk;
   TagType Tag;
   HVPositionType Pos;
   HVResolutionType Res;
   HI_U8 AspectRatio;
   _656Type _656;
   HI_U8 PixRep;
} VModeInfoType;

typedef struct {
   HVPositionType HVPos;
   HVResolutionType HVRes;
} DEType;
//--------------------------------------------------------------------
typedef struct {

   HI_U16 CapHalfLineCnt;
   HI_U16 PreEqPulseCnt;
   HI_U16 SerratedCnt;
   HI_U16 PostEqPulseCnt;
   HI_U8 Field2BackPorch;

} InterlaceCSyncType;

typedef struct {
   HI_UNF_ENC_FMT_E fmt;
} HDMIVICInfoType;


extern const VModeInfoType VModeTables[NMODES];
extern const HDMIVICInfoType VICModeTables[VICMODES];



