//------------------------------------------------------------------------------
// Copyright 2007, Silicon Image, Inc.  All rights reserved.
//
// No part of this work may be reproduced, modified, distributed, transmitted,
// transcribed, or translated into any language or computer format, in any form
// or by any means without written permission of: Silicon Image, Inc.,
// 1060 East Arques Avenue, Sunnyvale, California 94085
//------------------------------------------------------------------------------
#ifndef __AMF_H__
#define __AMF_H__
        
#include "si_typedefs.h"
#include "si_config.h"

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

// Audio states
#define STATE_AUDIO__MUTED          0
#define STATE_AUDIO__REQUEST_AUDIO  1
#define STATE_AUDIO__AUDIO_READY    2
#define STATE_AUDIO__ON             3

// Video states
#define STATE_VIDEO__MUTED          1
#define STATE_VIDEO__UNMUTE			2
#define STATE_VIDEO__ON             3
#if (PEBBLES_VIDEO_STATUS_2ND_CHECK==ENABLE)
#define STATE_VIDEO__CHECKED        4
#endif


//Audio modes
#define AUDIO_MODE__PCM			0
#define AUDIO_MODE__DSD			1	
#define AUDIO_MODE__HBR			2
#define AUDIO_MODE__NOT_INIT	0x0F
// On, Off
#define ON      1
#define OFF     0

// Set, Clear
#define SET     0xFF
#define CLEAR   0x00


//------------------------------------------------------------------------------
// Global Status Structure
//------------------------------------------------------------------------------
typedef struct {
    HI_U8 AudioState;
    HI_U8 ColorDepth;
	HI_U8 VideoState;
	HI_U8 AudioMode;   //101 added for handling HBR
    HI_U8 PortSelection;
	HI_U8 ResolutionChangeCount;	   //for zone workaround
	HI_U8 VideoStabilityCheckCount; //for ODCK disappear workaround
} GlobalStatus_t; 



//------------------------------------------------------------------------------
// Global status variable
//------------------------------------------------------------------------------
extern GlobalStatus_t CurrentStatus;



//------------------------------------------------------------------------------
// Function Prototypes for mainline code
//------------------------------------------------------------------------------
void SystemDataReset(void);



//------------------------------------------------------------------------------
// Function Prototypes for interrupt handler routines
//------------------------------------------------------------------------------
void SetupInterruptMasks(void);
void AudioUnmuteHandler(void);
void TurnAudioMute (HI_U8 qOn);
void TurnVideoMute(HI_U8 qOn);
void TurnPowerDown(HI_U8 qOn);
void PollInterrupt(void);

//------------------------------------------------------------------------------
// Function Prototypes for register access routines
//------------------------------------------------------------------------------
HI_U8 SI_CEC_RegisterRead(HI_U16 regAddr);
void SI_CEC_RegisterWrite(HI_U16 regAddr, HI_U8 value);
void SI_CEC_RegisterModify(HI_U16 regAddr, HI_U8 mask, HI_U8 value);
void SI_CEC_RegisterBitToggle(HI_U16 regAddr, HI_U8 mask);

void SI_CEC_RegisterReadBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length);
void SI_CEC_RegisterWriteBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length);




#endif  // __AMF_H__
