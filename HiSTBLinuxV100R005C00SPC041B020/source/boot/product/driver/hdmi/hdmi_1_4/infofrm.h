/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _INFOFRM_
#define _INFOFRM_

//#include "isr.h"
#include "typedefs.h"

#define AVI_TYPE   0x82
#define SPD_TYPE   0x83
#define AUD_TYPE   0x84
#define MPEG_TYPE  0x85
#define VENDORSPEC_TYPE  0x85

#define GENETIC_TYPE 0x00
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

//void InfoFrameIntHandler(HDMIIntType * );
//void SendAVIInfoFrame(void );
void SendAVIInfoFrame(HI_U8 *Data,HI_U8 Len);
void Send4KInfoFrame(HI_U8 *Data,HI_U8 Len);


void SendCP_Packet( HI_U8 bMuteAv );
//HI_U8 SiI_SendInfoFrame( InfoFrameType *, HI_U8 );
Bool EnableInfoFrame(HI_U8 );
HI_U8 SiI_SendInfoPacket(HI_U8 * /*Data*/, HI_U8 /*PcktSize*/, HI_U8 /*PacketId*/, HI_U8 /*Mode*/);
//HI_U8 SiI_SendInfoFrame( InfoFrameType * /*Info*/, HI_U8 /*Mode*/ );
Bool EnableInfoPacket( HI_U8 /* PacketId */);
Bool EnableInfoFrame(HI_U8 /*InfoFrameType*/);
Bool DisableInfoPacket( HI_U8 /* PacketId */);
Bool DisableInfoFrame(HI_U8 /*InfoFrameType*/);
void CalcCheckSumIFPacket( HeaderType * , HI_U8 * );
HI_U8 SiI_TX_SendInfoFrame( HI_U8 , HI_U8 * );

#endif
