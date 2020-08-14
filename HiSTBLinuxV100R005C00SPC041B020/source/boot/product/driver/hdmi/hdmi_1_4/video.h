/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _VIDEO_
#define _VIDEO_

#include "infofrm.h"
//-----------------------------------------------------
// Video registers
//-----------------------------------------------------
#define HRES_L_ADDR 0x3A
#define HRES_H_ADDR 0x3B
#define VRES_L_ADDR 0x3C
#define VRES_H_ADDR 0x3D
#define VID_CTRL_ADDR 0x48
#define VID_MODE_ADDR 0x4A
#define BLANKDATA_CH0_ADDR 0x4B		// Defines the video blanking value for channel 0
#define BLANKDATA_CH1_ADDR 0x4C		// Defines the video blanking value for channel 1
#define BLANKDATA_CH2_ADDR 0x4D		// Defines the video blanking value for channel 2
#define DE_PIX1_ADDR 0x4E			// Number pixels per DE (read only)
#define DE_PIX2_ADDR 0x4F
#define DE_LINE1_ADDR 0x4E			// Number pixels per DE (read only)
#define DE_LINE2_ADDR 0x4F
#define VID_STAT_ADDR 0x55
//----------------------------------------------------------------------
// CSync Interlace regs
#define FIELD2BACKPORCH_ADDR 0x54

#define  CAPHALFLINE_ADDR  0x57
#define  PREEQPULSCNT_ADDR 0x5A
#define  SERRATEDCNT_ADDR  0x5D
#define  POSTEQPULSECNT_ADDR    0x5E
//----------------------------------------------------------
#define SCALE_ADDR          0x5F
#define G_Y_OFFSET_ADDR     0x60
#define RB_CbCr_OFFSET_ADDR 0x62
#define G_Y_BLANK_ADDR      0x64
#define RB_CbCr_BLANK_ADDR  0x66
//-----------------------------------------------------------------------
//   VIDEO MODE BITS

#define  BIT_EN_INS_CSYNC 0x01
#define  BIT_444to422   0x02
#define  BIT_422to444   0x04
#define  BIT_CSC        0x08

#define  BIT_DITHER     0x20
#define  BIT_MUX_YC     0x40
#define  BIT_INS_SAVEAV 0x80

#define BIT_16_235_RANGE   0x10

//-----------------------------------------------------------------------
// VIDEO CONTRL BITS
#define BIT_BT709                       0x01
#define BIT_EXT_BIT_MODE                0x02
#define BIT_INSERT_CSYNC_ON_AOUT        0x08
#define BIT_ENCSYNC_ON_HSYNC            0x10
#define BIT_ENCSYNC_ON_VSYNC            0x20
#define BIT_INVERT_HSYNC                0x40
#define BIT_INVERT_VSYNC                0x80



//--------------------------------------------------------------------
#define RGB24 0
#define YCbCr24 1
#define YC24 2

#define SEPARATE_SYNC 0
#define COMP_ON_HSYNC 1
#define COMP_ON_VSYNC 2
#define SYNC_ON_Y 3

typedef struct {

        HI_U8  InpMode;
        HI_U8  OutModeVsRGBIn;
        HI_U8  OutModeVsYCbCrIn;
        HI_U8  OutModeVsYCIn;
        HI_U8  OutSyncVsRGBIn;
        HI_U8  OutSyncVsYCbCrIn;
        HI_U8  OutSyncVsYCIn;

} VideoCfgType;

typedef struct {
     HI_U8  EE_Addr;
     VideoCfgType VROM;
}VideoCfgROMType;

HI_U16 GetHSyncsPerFrame(void);
HI_U16 GetPixelsForHSync(void);
HI_U8 GetRefrTypeHVPol(void);
HI_U8 CheckSyncDetect(void);
void GetTimingParameters(HI_U8 *);
void SelectInputFormat(void);
void UpdateVideoPath(HI_U8);
void SelectInVideoSpace(HI_U8);
void SetPixRepIn(HI_U8);
void SetPixRepDiv(HI_U8);
HI_U8 GetPixRepIn(void);
void CorrectPixRepDiv(void);
HI_U8 IsMUXor656(void);
void UpdateVideoPathForDemo( HI_U8 );


#endif

