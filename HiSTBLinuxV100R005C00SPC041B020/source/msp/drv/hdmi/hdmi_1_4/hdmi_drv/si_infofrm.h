/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _INFOFRM_
#define _INFOFRM_

#include "si_isr.h"
#include "drv_global.h"

#define VENDORSPEC_TYPE     0x81
#define AVI_TYPE            0x82
#define SPD_TYPE            0x83
#define AUD_TYPE            0x84
#define MPEG_TYPE           0x85
#define GAMUT_METADATA_TYPE 0x00
//-----------------------------------
//  InfoFrame masks
//-----------------------------------
#define AVI_MASK 0x01
#define SP_MASK 0x02
#define AUD_MASK 0x04
#define MPEG_MASK 0x08
#define UNKNOWN_MASK 0x10
#define CP_MASK  0x80

#define BIT_AVI_EN_REPEAT  0x0003
#define BIT_SPD_EN_REPEAT  0x000C
#define BIT_AUD_EN_REPEAT  0x0030
#define BIT_MPEG_EN_REPEAT 0x00C0
#define BIT_GENERIC1_EN_REPEAT  0x0300
#define BIT_GENERIC2_EN_REPEAT  0x3000
#define GAMUT_METADATA_EN_REPEAT 0xc0

#define  AVI_SIZE 15
#define  SPD_IF_SIZE 31
#define  AUD_IF_SIZE  10
#define  MPEG_IF_SIZE  31
#define  GENERIC1_IF_SIZE 31
#define  GENERIC2_IF_SIZE 31


#define AVI_IF_ADDR 0x40
#define SPD_IF_ADDR  0x60
#define AUD_IF_ADDR 0x80
#define MPEG_IF_ADDR 0xA0
#define GENERIC1_IF_ADDR 0xC0
#define GENERIC2_IF_ADDR 0xE0
#define CP_IF_ADDR   0xDF // Contain Protect 1- HI_U8 Frame Info Frame

#define BIT_CP_AVI_MUTE_SET    0x01
#define BIT_CP_AVI_MUTE_CLEAR  0x10

/* XVYCC */
#define RGB2XVYCC_CT      0x50
#define R2Y_COEFF_LOW     0x51
#define R2Y_COEFF_UP      0x52
#define G2Y_COEFF_LOW     0x53
#define G2Y_COEFF_UP      0x54
#define B2Y_COEFF_LOW     0x55
#define B2Y_COEFF_UP      0x56
#define R2Cb_COEFF_LOW    0x57
#define R2Cb_COEFF_UP     0x58
#define G2Cb_COEFF_LOW    0x59
#define G2Cb_COEFF_UP     0x5a
#define B2Cb_COEFF_LOW    0x5b
#define B2Cb_COEFF_UP     0x5c
#define R2Cr_COEFF_LOW    0x5d
#define R2Cr_COEFF_UP     0x5e
#define G2Cr_COEFF_LOW    0x5f
#define G2Cr_COEFF_UP     0x60
#define B2Cr_COEFF_LOW    0x61
#define B2Cr_COEFF_UP     0x62
#define RGB_OFFSET_LOW    0x63
#define RGB_OFFSET_UP     0x64
#define Y_OFFSET_LOW      0x65
#define Y_OFFSET_UP       0x66
#define CbCr_OFFSET_LOW   0x67
#define CbCr_OFFSET_UP    0x68


/*
typedef struct {
   HI_U8 Type;
   HI_U8 Version;
   HI_U8 Length;
   HI_U8 CheckSum;
   HI_U8 Data[25];
} InfoFrameType;
*/

#define siiIsTClockStable()  ( ReadByteHDMITXP0( TX_STAT_ADDR ) & 0x01 )

typedef struct {
    HI_U8 Type;
    HI_U8 Version;
    HI_U8 Length;
    HI_U8 CheckSum;

}HeaderType;

typedef struct {
   HI_U8 DestAddr;			// InfoFrame Register address: 0xA0 for MPEG; 0x40 for AVI...
   HI_U16 CtrlBitsAddr;		// Bit masks of 7A:3E..3F to enable & repeat InfoFrames
   HI_U8 BlockSize;			// InfoFrame size (HI_U8s): AVI: 15; Audio: 10...
   HI_U8 CheckSumProp;
} InfoMapType;

void InfoFrameIntHandler(HDMIIntType * );
void SI_SendAVIInfoFrame(void );
void SI_SendCP_Packet( HI_U8 bMuteAv );
//HI_U8 SiI_SendInfoFrame( InfoFrameType *, HI_U8 );
Bool SI_EnableInfoFrame(HI_U8 );
HI_U8 SI_SendInfoPacket(HI_U8 * /*Data*/, HI_U8 /*PcktSize*/, HI_U8 /*PacketId*/, HI_U8 /*Mode*/);
//HI_U8 SiI_SendInfoFrame( InfoFrameType * /*Info*/, HI_U8 /*Mode*/ );
Bool SI_EnableInfoPacket( HI_U8 /* PacketId */);
Bool SI_EnableInfoFrame(HI_U8 /*InfoFrameType*/);
Bool SI_DisableInfoPacket( HI_U8 /* PacketId */);
Bool SI_DisableInfoFrame(HI_U8 /*InfoFrameType*/);
HI_U8 SI_TX_SendInfoFrame( HI_U8 , HI_U8 * );

HI_U8 SI_3D_Setting(HI_U32 u83DFormat);
HI_U8 SI_Set_VSI_3D_FramePacking(void);
HI_U8 SI_Set_VSI_3D_FieldAlt(void);
HI_U8 SI_Set_VSI_3D_LineAlt(void);
HI_U8 SI_Set_VSI_3D_SidebySide_Full(void);
HI_U8 SI_Set_VSI_3D_L_Depth(void);
HI_U8 SI_Set_VSI_3D_L_Depth_2Graphic_Depth(void);
HI_U8 SI_Set_VSI_3D_TopandBottom_Half(void);
HI_U8 SI_Set_VSI_3D_SidebySide_Half(void);

HI_U8 SI_4K_Setting(HI_U8 u324KFormat);
HI_U8 SI_Set_VSI_4K2K(HI_U32 u324KFormat);

//set vsdb mode,and base on vsdb mode, to setting 3d infoframe or 4k infoframe
HI_U8 SI_VSDB_Setting(VSDB_MODE_E mode,HI_U32 u32Fmt);


HI_U8 SI_GetAVIInfoFrameVID(void);


#endif
