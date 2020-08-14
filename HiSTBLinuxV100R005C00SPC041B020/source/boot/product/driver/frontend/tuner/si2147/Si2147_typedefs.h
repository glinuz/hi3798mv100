/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API types used by commands and properties
   FILE: Si2147_typedefs.h
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   Si2147_TYPEDEFS_H
#define   Si2147_TYPEDEFS_H

 #define BUF_SIZE 1000
/* define the maximum possible channels (1002MHz - 43MHz) / 6MHz  (required for the channelscan array)*/
#define MAX_POSSIBLE_CHANNELS 160
/* _additional_defines_point */
#define MAX_LENGTH             80

#define DOWNLOAD_ON_CHANGE 1
#define DOWNLOAD_ALWAYS 0

// define the tuner broadcast address for common patch download.
#define TUNER_BROADCAST_ADDRESS 0xc4

typedef struct L1_Si2147_Context {
  L0_Context                 *i2c;
  L0_Context                  i2cObj;
  Si2147_CmdObj              *cmd;
  Si2147_CmdObj               cmdObj;
  Si2147_CmdReplyObj         *rsp;
  Si2147_CmdReplyObj          rspObj;
  Si2147_PropObj             *prop;
  Si2147_PropObj              propObj;
  Si2147_PropObj             *propShadow;
  Si2147_PropObj              propShadowObj;
  Si2147_COMMON_REPLY_struct *status;
  Si2147_COMMON_REPLY_struct  statusObj;
  /*chip rev constants for integrity checking */
  unsigned char chiprev;
  unsigned char part;
  /* Last 2 digits of part number */
  unsigned char partMajorVersion;
  unsigned char partMinorVersion;
  unsigned char partRomid;
 /* Channel Scan Globals */
  /* Global array to store the list of found channels */
  unsigned long ChannelList[MAX_POSSIBLE_CHANNELS];
  /* ChannelScanPal needs to store the PAL type also so allocate 4 chars each for that */
  char ChannelType[MAX_POSSIBLE_CHANNELS][4];
  /* Number of found channels from a channel scan */
  int ChannelListSize;
  /* global variable for property settings mode.  Can be either DOWNLOAD_ON_CHANGE (recommended) DOWNLOAD_ALWAYS. */
  int propertyWriteMode;
/* _additional_struct_members_point */
} L1_Si2147_Context;

/* _additional_definitions_start */
#ifndef __VID_FILT_STRUCT__
#define __VID_FILT_STRUCT__
/* struct to read video filter file with varying line lengths */
typedef struct  {
  char vid_filt_len;
  unsigned char vid_filt_table [16];
} vid_filt_struct;
#endif /* __VID_FILT_STRUCT__ */

#ifndef __FIRMWARE_STRUCT__
#define __FIRMWARE_STRUCT__
typedef struct  {
  unsigned char firmware_len;
  unsigned char firmware_table[16];
} firmware_struct;
#endif /* __FIRMWARE_STRUCT__ */

/* _additional_definitions_point */

#endif /* Si2147_TYPEDEFS_H */





