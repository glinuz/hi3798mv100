/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _ISR_
#define _ISR_

#include "si_audio.h"

typedef struct
{
  HI_U8 Audio;
  HI_U8 Sys;
  HI_U8 Info;
} HDMIIntType;

void OnlyExtInterruptEnable(void);
//void SysTimerInterrupts(HI_U8);
void ClrHDMIRXIntInRX(void);
HI_U32 SI_HPD_Status( void );
HI_U32 SI_RSEN_Status( void );
void SI_InterruptHandler( void );
void PrepareForAudioStatTest(HI_U8);
void ReadAudioStatData (HI_U8 *);
HI_U32 SI_HDCP_SetHPDAuthCount( HI_U32 Count );
HI_U32 SI_Is_HDCPAuth_DetectHPD( void );
void SI_InitDVIInterruptMasks(void);
void SI_RiCheckInterruptMask(HI_U8);
void ClearBiphaseSampleDroppedInterrupts( void );
HI_U8 CheckBiphaseSampleDroppedInterrupts( void );
Bool SI_WaitPixClockStable(void);
void SI_HotPlugInOrOut(void);		

void SI_GetErrCount(HDMI_INT_Err_Conut *pHDMI_INT_ERRCount);

extern void SI_SetEncryption(HI_U8);

void SI_EnableInterrupts(void);
void SI_EnableHDMICEC(Bool bCECEn);

#endif




