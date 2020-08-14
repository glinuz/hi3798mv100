/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include <linux/time.h>
#include <linux/interrupt.h>
#ifndef _TIMER_
#define _TIMER_


#ifdef HI_ADVCA_FUNCTION_RELEASE
#define DEBUG_PRINTK(fmt, args...)
#else
#define DEBUG_PRINTK(fmt,args...)  printk(fmt,##args)
#endif

#define _1Sec 31
#define _2Sec 62
#define ProgrVPosHPos 0x03

/// //#define ProgrVNegHPos 0x02
#define ProgrVNegHPos 0x01

/// //#define ProgrVPosHNeg 0x01
#define ProgrVPosHNeg 0x02

#define ProgrVNegHNeg 0x00
#define InterlaceVPosHPos 0x07
#define InterlaceVNgeHPos 0x06
#define InterlaceVPosHNeg 0x05
#define InterlaceVNegHNeg 0x04

typedef struct {

  HI_U8 RefrTypeVHPol;
  HI_U16 HTot;
  HI_U16 VFreq;

} SyncInfoType;
// System states
#define PowerDown 0
#define NoSyncDetect 1
#define SyncInChange 2
#define SyncOutOfRange 3
#define SyncInRangeReqVMDetection 4
#define SyncInRangeVMDetected 5
#define SyncInRangeVMNotDetected 6

#define H_TOLERANCE 50  // +- 500 Hz
#define V_TOLERANCE 150  // +- 1.5 Hz

#define INTERLACE_VFREQ_LOW_LIMIT  2500 // 25 Hz
#define PROGRESSIVE_VFREQ_LOW_LIMIT  2300 // 23 Hz
#define INTERLACE_HFREQ_LOW_LIMIT 1500 // 15 kHz
#define PROGRESSIVE_HFREQ_LOW_LIMIT 3000 // 30 kHz
#define INTERLACE_VFREQ_HIGH_LIMIT  6100 // 61 Hz
#define PROGRESSIVE_VFREQ_HIGH_LIMIT  10000 // 100 Hz
#define INTERLACE_HFREQ_HIGH_LIMIT 5500 // 55 kHz
#define PROGRESSIVE_HFREQ_HIGH_LIMIT 9700 // 97 kHz
#define INTERLACE_MASK 0x04

// TX OUTPUT STATES

#define CABLE_NC 0
#define CABLE_C_NO_EDID_ACK 3       // HP High no Ack for 0xA0 slave address
#define CABLE_C_EDID_CORRUPTED 4    // HP High, EDID CRC is wrong
#define CABLE_C_RX_DVI 5            //
#define CABLE_C_RX_HDMI 6
#define CABLE_C_RX_DVI_POWER_OFF 6
#define CABLE_C_RX_HDMI_POWER_ON 7
#define HP_EVENT 8

#define CABLE_UNPLUG_ 0
#define CABLE_PLUGIN_CHECK_EDID 1
#define CABLE_PLUGIN_CHECK_EDID_OK 2
#define CABLE_PLUGIN_CHECK_EDID_CORRUPTED 3
#define CABLE_PLUGIN_HDMI_OUT  4
#define CABLE_PLUGIN_DVI_OUT   5

#define SYNC_CONFIRM_THRESHOLD 5

///void SI_TimerHandler(void);
HI_U8 SI_TimerHandler(void);

void SI_GetSyncInfo(void);
//HI_U16 GetMCUClocksPerVSyncPeriod(void);

void SI_PrintAuthState(void);
HI_U32 SI_timer_diff(struct timeval time_before, struct timeval time_after);
struct timeval si_time_now(void);
HI_VOID SI_timer_start(void);
HI_VOID SI_timer_stop(void);
HI_U32 SI_timer_count(void);

HI_U32 SI_GetHDMIOutputStatus(void);
void SI_SetHDMIOutputStatus(HI_U32 u32OutputState);
HI_U32 SI_GetHDMIOldOutputStatus(void);
void SI_SetHDMIOldOutputStatus(HI_U32 u32OutputOldState);

extern HI_U8 AuthState;

#endif

