/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
// This file contains MCU definitions as defined on a particular board.
// Also any behaivor associated with the board is defined here.

#ifndef _MCUDEFS_
#define _MCUDEFS_

sbit Master         = P1^4;

// These are global variable for the buttons, and I/Os to 801/861 and panel
sbit Reset_HDMITX_Pin  = P0^1;


sbit Reset_SPDIF_Decoder_Pin	= P0^2;

sbit DDC_SCL       = P1^4;
sbit DDC_SDA       = P1^5;
sbit SCL           = P1^6;
sbit SDA           = P1^7;

//-------------------------------------------------------------------------------
sbit SCDTPin      = P2^0;

sbit detect        = P3^3; // signals cable connection
sbit v_sync        = P1^1;

sbit TXInt_Pin  = P3^3;

//-------------------------------------------------------------------------------
sbit LED1_Pin = P2^0;
sbit LED2_Pin = P2^1;
//-------------------------------------------------------------------------------


#define OSC 11059200			// Oscillator frequency of 11.0592 MHz
#define REF_FREQ 92160000       // OSC/12 * 10 (mul by 10 to incr. resolution of VSync )

#define INT_CONTROL	0x00		// Interrupt pin is push-pull and active high (this is normally 0x06)

#endif

