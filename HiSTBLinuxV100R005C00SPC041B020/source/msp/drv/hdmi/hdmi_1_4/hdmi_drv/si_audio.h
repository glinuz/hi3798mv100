/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"

#ifndef __audio_h_20100413_
#define __audio_h_20100413_

HI_U8 IsAudioDisabled(void);
Bool SI_IsSPDIFSelected( void );
void SI_TXAudio( HI_U8 );
void SI_SetAudioPath ( HI_U8 * );
void SI_GetAudioPath ( HI_U8 * );
void SI_Set_AudioInfoFramePacket ( HI_U8 * abAudioPath, HI_U8 bChSpkAlloc, HI_U8 bDM_LSV );

#define BIT_DC_EN					0x40
#define PACKET_MODE					0x18
#define THIRTY_BITS_PER_PIXEL		0x08
#define THIRTY_SIX_BITS_PER_PIXEL	0x10
#define MUTE_AUDIO					0x02
#define ENABLE_ALL					0x00
#define MUTE_ALL					0x06

#define OVER_RUN                    0x02
#define UNDER_RUN                   0x01
#define SPDIF_PAR                   0x40

#endif

