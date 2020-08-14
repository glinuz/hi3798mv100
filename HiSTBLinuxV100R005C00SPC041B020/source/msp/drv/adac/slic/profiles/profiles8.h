/*
 * profiles8.h --
 *
 * This header file exports the Profile data types
 *
 * Copyright (c) 2008 Zarlink Semiconductor Inc.
 *
 * Project Info --
 *   File:  V:\dbuckley\uvb\sdk\apps\qs_le71hr8921_connect\LE71HR8921.vpw
 *   Type:  Custom Design Using Zarlink Semiconductor VE890 - Le89316
 *   Date:  Friday, August 10, 2007 16:30:18
 *
 *   This file was generated with Profile Wizard Version: 1.7.6
 */

#ifndef PROFILES8_H
#define PROFILES8_H

#ifdef VP_API_TYPES_H
#include "vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif


/************** Device_Parameters **************/
/* Device Profile */
extern const VpProfileDataType DEV_PROFILE[];

/************** AC_Coefficients **************/
extern const VpProfileDataType AC_FXS_RF50_600[];    /* AC FXS RF50 600 Ohm Coefficients */
extern const VpProfileDataType AC_FXS_RF50_EU[];     /* AC FXS RF50 270+750//150nF Coefficients */
extern const VpProfileDataType AC_FXO_600_NORMAL[];  /* AC FXO 600 Ohm Normal Coefficients */
extern const VpProfileDataType AC_FXO_EU_NORMAL[];   /* AC FXO 270+750//150nF Normal Coefficients */

/************** DC_Parameters **************/
extern const VpProfileDataType DC_FXS_REGION1[];     /* DC FXS Region 1 Parameters */

/************** Ring_Parameters **************/
extern const VpProfileDataType RING_20HZ_SINE[];     /* Ringing 20Hz, Sine Wave, 50vrms */

/************** Call_Progress_Tones **************/
extern const VpProfileDataType TONE_DIAL[];          /* US Dial Tone (C_CONT) */
extern const VpProfileDataType TONE_RINGBACK[];      /* US Ringback Tone (C_00) */
extern const VpProfileDataType TONE_BUSY[];          /* US Busy Tone (C_01) */
extern const VpProfileDataType TONE_REORDER[];       /* US Reorder tone */
extern const VpProfileDataType TONE_ROH[];           /* Receiver Off-Hook */

/************** Cadence_Definitions **************/
extern const VpProfileDataType TONE_CAD_DIAL0[];     /* Standard dial tone cadence */
extern const VpProfileDataType TONE_CAD_DIAL1[];     /* Stutter dial tone cadence */
extern const VpProfileDataType TONE_CAD_BUSY0[];     /* Busy signal cadence (0.5 sec on 0.5 sec off) */
extern const VpProfileDataType TONE_CAD_REORDER[];   /* Tone Reorder cadence (0.25 sec on 0.25 sec off) */
extern const VpProfileDataType TONE_CAD_ROH[];       /* Receiver Off-Hook */
extern const VpProfileDataType TONE_CAD_RNGBACK[];   /* Ringback Cadence */
extern const VpProfileDataType RING_CAD_STD[];       /* Standard Ringing Cadence (2 sec on 4 sec off) */

/************** Caller_ID **************/
extern const VpProfileDataType CLI_TYPE1_US[];
/************** FXO_Dialing_Profile **************/
extern const VpProfileDataType FXO_DIALING_REGION1[];/* FXO Dialing Region 1 profile */

#endif /* PROFILES8_H */

