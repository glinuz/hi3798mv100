/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : si_edid_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/8/14
  Description   :
  History       :
  Date          : 2013/8/14
  Author        : wx178178
  Modification  :
*******************************************************************************/
#ifndef __SI_EDID_H_
#define __SI_EDID_H_

#include "si_typedefs.h"
#include "si_hlviic.h"
#include "si_globdefs.h"
#include "si_global.h"

#define UNKNOWN_DISPLAY 0
#define DVI_DISPLAY 1
#define HDMI_DISPLAY 2


#define  EDID_READ_FIRST_BLOCK_ERR   0x1
#define  EDID_READ_SECOND_BLOCK_ERR  0x2
#define  EDID_READ_THIRD_BLOCK_ERR   0x3
#define  EDID_READ_FOURTH_BLOCK_ERR  0x4

#define  EDID_SIZE           0x200
#define  EDID_BLOCK_SIZE     0x80
#define  STANDARDTIMING_SIZE 12

#define  BIT0                0x00
#define  BIT1                0x02
#define  BIT2                0x04
#define  BIT3                0x08
#define  BIT4                0x10
#define  BIT5                0x20
#define  BIT6                0x40
#define  BIT7                0x80
#define  AUDIO_DATA_BLOCK    0x01
#define  VIDEO_DATA_BLOCK    0x02
#define  VENDOR_DATA_BLOCK   0x03
#define  SPEAKER_DATA_BLOCK  0x04
#define  VESA_DTC_DATA_BLOCK 0x05
#define  USE_EXT_DATA_BLOCK  0x07

#define  VIDEO_CAPABILITY_DATA_BLOCK      0x00
#define  VENDOR_SPECIFIC_VIDEO_DATA_BLOCK 0x01
#define  RESERVED_VESA_DISPLAY_DEVICE     0x02
#define  RESERVED_VESA_VIDEO_DATA_BLOCK   0x03
#define  RESERVED_HDMI_VIDEO_DATA_BLOCK   0x04
#define  COLORIMETRY_DATA_BLOCK           0x05
#define  CEA_MISCELLANENOUS_AUDIO_FIELDS  0x10
#define  VENDOR_SPECIFIC_AUDIO_DATA_BLOCK 0x11
#define  RESERVED_HDMI_AUDIO_DATA_BLOCK   0x12

// Codes of EDID Errors

#define NO_ERR 0
// 1-7 reserved for I2C Errors
#define BAD_HEADER 8
#define VER_DONT_SUPPORT_861B 9
#define _1ST_BLOCK_CRC_ERROR 10
#define _2ND_BLOCK_CRC_ERROR 11
#define EXTENSION_BLOCK_CRC_ERROR 11
#define NO_861B_EXTENSION 12
#define NO_HDMI_SIGNATURE 13
#define BLOCK_MAP_ERROR 14
#define CRC_CEA861EXTENSION_ERROR 15
#define EDID_MAX_ERR_NUM 16

#define  AUDIO_FORMAT_CODE   0x78
#define  AUDIO_MAX_CHANNEL   0x07
#define  DATA_BLOCK_LENGTH   0x1F
#define  DATA_BLOCK_TAG_CODE 0xE0
#define  EDID_VIC            0x7F
#define  NATIVE_FORMAT       0x80

#define  DDC_ADDR            0xA0
#define  EXT_BLOCK_ADDR      0x7E  
#define  FIRST_DETAILED_TIMING_ADDR 0x36
#define  EXT_VER_TAG         0x02
#define  EXT_REVISION        0x02

#define  XVYCC601            0x01
#define  XVYCC709            0x02
#define  SYCC601             0x04
#define  ADOBE_XYCC601       0x08
#define  ADOBE_RGB           0x10


#define  NUM_EXTENSIONS_ADDR 0x7E


typedef struct 
{
	HI_U8 header[8];
	/* Vendor & product info */
	HI_U8 mfg_id[2];
	HI_U8 prod_code[2];
	HI_U8 serial[4]; /* FIXME: byte order */
	HI_U8 mfg_week;
	HI_U8 mfg_year;
	/* EDID version */
	HI_U8 version;
	HI_U8 revision;
	/* Display info: */
	HI_U8 input;
	HI_U8 width_cm;
	HI_U8 height_cm;
	HI_U8 gamma;
	HI_U8 features;
	/* Color characteristics */
	HI_U8 red_green_low;
	HI_U8 black_white_low;
	HI_U8 red_x;
	HI_U8 red_y;
	HI_U8 green_x;
	HI_U8 green_y;
	HI_U8 blue_x;
	HI_U8 blue_y;
	HI_U8 white_x;
	HI_U8 white_y;
    HI_U8 est_timing[3];
    HI_U8 std_timing[16];
    HI_U8 detailed_timing[4];
	/* Number of 128 byte ext. blocks */
	HI_U8 extensions;
	/* Checksum */
	HI_U8 checksum;
} __attribute__((packed))EDID_FIRST_BLOCK_INFO;

typedef struct
{
    HI_U8 pixel_clk[2];
    HI_U8 h_active;
    HI_U8 h_blank;
    HI_U8 h_active_blank;
    HI_U8 v_active;
    HI_U8 v_blank;
    HI_U8 v_active_blank;
    HI_U8 h_sync_offset;
    HI_U8 h_sync_pulse_width;
    HI_U8 vs_offset_pulse_width;
    HI_U8 hs_offset_vs_offset;
    HI_U8 h_image_size;
    HI_U8 v_image_size;
    HI_U8 h_v_image_size;
    HI_U8 h_border;
    HI_U8 v_border;
    HI_U8 flags;
}DETAILED_TIMING_BLOCK;

typedef struct 
{
    HI_UNF_ENC_FMT_E enTimingType;
    HI_U32 u32Hact;
    HI_U32 u32Vact;
    HI_U32 u32VerFreq;                      /*  0.01 Hz*/
    HI_U32 u32PixClkKHz;                    /* pixel_clock  in KHz*/
    HI_U16 u16RB;                           /*Reduced Blanking*/
}  EDID_DETAILED_STRING_S;

/******************************************************************************/
/* Public  interfaces                                                         */
/******************************************************************************/
HI_S32 SI_ReadSinkEDID(HI_VOID);
HI_U8 SI_ParseEDID(HI_U8 *DisplayType);
HI_S32 SI_SetForceSinkCapability(HI_VOID);
HI_U8 SI_Proc_ReadEDIDBlock(HI_U8 *DataBlock, HI_U32 *size);
HI_U8 SI_Force_GetEDID(HI_U8 *datablock, HI_U32 *length);
HI_U8 Transfer_VideoTimingFromat_to_VModeTablesIndex(HI_UNF_ENC_FMT_E unfFmt);
HI_U8 SI_ByteReadEDID(HI_U8, HI_U8, HI_U8 *);
HI_S32 SI_GetHdmiSinkCaps(HI_UNF_EDID_BASE_INFO_S *pCapability);



#endif
