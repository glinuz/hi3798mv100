/******************************************************************************
 *
 * Copyright 2012, Silicon Image, Inc.  All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of: Silicon Image, Inc., 1140
 * East Arques Avenue, Sunnyvale, California 94085
 *
 *****************************************************************************/
/**
 * @file si_drv_tx_cbus_defs.h
 *
 * @brief Cbus definitions
 *
 *****************************************************************************/

#ifndef __SI_MOD_TX_CBUS_DEFS_H__
#define __SI_MOD_TX_CBUS_DEFS_H__

#include "si_datatypes.h"

// initialize MHL Devcap Registers with the correct values
#define DEVCAP_VAL_DEV_STATE			0x00
#define DEVCAP_VAL_MHL_VERSION			MHL_VERSION
#define DEVCAP_VAL_DEV_CAT				MHL_DEV_CAT_SOURCE								/*0x02*/
#define DEVCAP_VAL_ADOPTER_ID_H			(SILICON_IMAGE_ADOPTER_ID >> 8)					/*0x01*/		
#define DEVCAP_VAL_ADOPTER_ID_L			(SILICON_IMAGE_ADOPTER_ID & 0xFF)				/*0x42*/
#define DEVCAP_VAL_VID_LINK_MODE		MHL_DEV_VID_LINK_MODE							/*0x7F*/
#define DEVCAP_VAL_AUD_LINK_MODE		MHL_DEV_AUD_LINK_MODE							/*0x03*/
#define DEVCAP_VAL_VIDEO_TYPE			MHL_VIDEO_TYPE									/*0x8F*/
#define DEVCAP_VAL_LOG_DEV_MAP			MHL_LOGICAL_DEVICE_MAP							/*0x41*/
#define DEVCAP_VAL_BANDWIDTH			MHL_BANDWIDTH_LIMIT_75MHZ						/*0x0F*/
#define DEVCAP_VAL_FEATURE_FLAG			MHL_FEATURE_FLAG_SUPPORT						/*0x1F*/
#define DEVCAP_VAL_DEVICE_ID_H			(MHL_DEVICE_ID >> 8)							/*0x86*/
#define DEVCAP_VAL_DEVICE_ID_L			(MHL_DEVICE_ID & 0xFF)							/*0x30*/
#define DEVCAP_VAL_SCRATCHPAD_SIZE		MHL_SCRATCHPAD_SIZE								/*0x10*/
#define DEVCAP_VAL_INT_STAT_SIZE		MHL_INT_STAT_SIZE								/*0x33*/
#define DEVCAP_VAL_RESERVED				0x00

enum {
	
	MHL_IDLE				= 0,			/* IDLE State */
	MHL_END_OF_FRAME		= 0x32,			/* End of Frame */
	MHL_ACK					= 0x33,			/* Command or Data byte acknowledge */
	MHL_NACK				= 0x34,			/* Command or Data byte not acknowledge */
	MHL_ABORT				= 0x35,			/* Transaction abort */
	MHL_WRITE_STAT			= 0x60 | 0x80,	/* Write one status register strip top bit */
	MHL_SET_INT				= 0x60,			/* Write one interrupt register */
	MHL_READ_DEVCAP			= 0x61,			/* Read one register */
	MHL_GET_STATE			= 0x62,			/* Read CBUS revision level from follower */
	MHL_GET_VENDOR_ID		= 0x63,			/* Read vendor ID value from follower */
	MHL_SET_HPD				= 0x64,			/* Set Hot Plug Detect in follower */
	MHL_CLR_HPD				= 0x65,			/* Clear Hot Plug Detect in follower */
	MHL_SET_CAP_ID			= 0x66,			/* Set Capture ID for downstream device */
	MHL_GET_CAP_ID			= 0x67,			/* Get Capture ID from downstream device */
	MHL_MSC_MSG				= 0x68,			/* VS command to send RCP sub-commands */
	MHL_GET_VS_ERRORCODE	= 0x69,			/* Get Vendor-Specific command error code */
	MHL_GET_DDC_ERRORCODE	= 0x6A,			/* Get DDC channel command error code */
	MHL_GET_MSC_ERRORCODE	= 0x6B,			/* Get MSC command error code */
	MHL_WRITE_BURST			= 0x6C,			/* Write 1-16 bytes to responder's scratchpad */
	MHL_GET_SC3_ERRORCODE	= 0x6D,			/* Get channel 3 command error code */
	MHL_WRITE_XSTAT			= 0x70,			/* Write one extended status register */
	MHL_READ_XDEVCAP		= 0x71,			/* Read one extended devcap register */
	
	/* let the rest of these float, they are software specific */
	MHL_READ_EDID_BLOCK,
	MHL_SEND_3D_REQ_OR_FEAT_REQ,
};

/*
 * DEVCAP offsets
 */
enum {
	DEVCAP_OFFSET_DEV_STATE,
	DEVCAP_OFFSET_MHL_VERSION,
	DEVCAP_OFFSET_DEV_CAT,
	DEVCAP_OFFSET_ADOPTER_ID_H,
	DEVCAP_OFFSET_ADOPTER_ID_L,
	DEVCAP_OFFSET_VID_LINK_MODE,
	DEVCAP_OFFSET_AUD_LINK_MODE,
	DEVCAP_OFFSET_VIDEO_TYPE,
	DEVCAP_OFFSET_LOG_DEV_MAP,
	DEVCAP_OFFSET_BANDWIDTH,
	DEVCAP_OFFSET_FEATURE_FLAG,
	DEVCAP_OFFSET_DEVICE_ID_H,
	DEVCAP_OFFSET_DEVICE_ID_L,
	DEVCAP_OFFSET_SCRATCHPAD_SIZE,
	DEVCAP_OFFSET_INT_STAT_SIZE,
	DEVCAP_OFFSET_RESERVED,
	/* this one must be last */
	DEVCAP_SIZE
};

/* Version that this chip supports */
#define	MHL_VER_MAJOR						0x30	// bits 4..7
#define	MHL_VER_MINOR						0x00	// bits 0..3
#define MHL_VERSION							(MHL_VER_MAJOR | MHL_VER_MINOR)
#define MHL_VERSION_2						0x20

/* Device Category */
#define MHL_DEV_CATEGORY_OFFSET				DEVCAP_OFFSET_DEV_CAT
#define MHL_DEV_CAT_SINK					0x01
#define MHL_DEV_CAT_SOURCE					0x02
#define MHL_DEV_CAT_DONGLE					0x03
#define MHL_DEV_CAT_SELF_POWERED_DONGLE		0x13
#define OTHER_DEV_CAT						0x80

#define MHL_DEV_CATEGORY_POW_BIT			0x10
#define MHL_DEV_CATEGORY_PLIM2_0			0x60

#define I_VBUS_PRE_DISCOVERY				100
#define I_VBUS_SOURCE_TO_DONGLE				200
#define I_VBUS_POST_DISCOVERY				500

/* Silicon Image's Adopter Id, assigned by MHL, LLC. */
#define SILICON_IMAGE_ADOPTER_ID			322		

/* Video Link Mode */
#define MHL_DEV_VID_LINK_SUPP_RGB444		0x01
#define MHL_DEV_VID_LINK_SUPP_YCBCR444		0x02
#define MHL_DEV_VID_LINK_SUPP_YCBCR422		0x04
#define MHL_DEV_VID_LINK_SUPP_PPIXEL		0x08
#define MHL_DEV_VID_LINK_SUPP_ISLANDS		0x10	//redundant with Bits1-3
#define MHL_DEV_VID_LINK_SUPP_VGA			0x20	//NA for Tx
#define MHL_DEV_VID_LINK_SUPP_16BPP			0x40
#define MHL_DEV_VID_LINK_MODE				( \
											MHL_DEV_VID_LINK_SUPP_RGB444		\
											| MHL_DEV_VID_LINK_SUPP_YCBCR444	\
											| MHL_DEV_VID_LINK_SUPP_YCBCR422	\
											| MHL_DEV_VID_LINK_SUPP_PPIXEL		\
											| MHL_DEV_VID_LINK_SUPP_16BPP		\
											)

/* Audio Link Mode Support */
#define MHL_DEV_AUD_LINK_2CH				0x01
#define MHL_DEV_AUD_LINK_8CH				0x02
#define MHL_DEV_AUD_LINK_MODE				(MHL_DEV_AUD_LINK_2CH | MHL_DEV_AUD_LINK_8CH)

/* VIDEO TYPES */
#define MHL_VT_GRAPHICS						0x01
#define MHL_VT_PHOTO						0x02
#define MHL_VT_CINEMA						0x04
#define MHL_VT_GAMES						0x08
#define MHL_SUPP_VT							0x80
#define MHL_VIDEO_TYPE						( \
											MHL_VT_GRAPHICS | MHL_VT_PHOTO	\
											| MHL_VT_CINEMA | MHL_VT_GAMES	\
											| MHL_SUPP_VT \
											)

/* Logical Dev Map */
#define MHL_DEV_LD_DISPLAY					0x01
#define MHL_DEV_LD_VIDEO					0x02
#define MHL_DEV_LD_AUDIO					0x04
#define MHL_DEV_LD_MEDIA					0x08
#define MHL_DEV_LD_TUNER					0x10
#define MHL_DEV_LD_RECORD					0x20
#define MHL_DEV_LD_SPEAKER					0x40
#define MHL_DEV_LD_GUI						0x80
#define MHL_LOGICAL_DEVICE_MAP				(MHL_DEV_LD_VIDEO | MHL_DEV_LD_AUDIO | MHL_DEV_LD_MEDIA | MHL_DEV_LD_GUI)

/* Bandwidth */
#define MHL_BANDWIDTH_LIMIT					22	/* 225 MHz */
#define MHL_BANDWIDTH_LIMIT_75MHZ			0x0F /* 15*5=75MHz*/

/* Feature Flag in the devcap */
#define MHL_DEV_FEATURE_FLAG_OFFSET			DEVCAP_OFFSET_FEATURE_FLAG
#define MHL_FEATURE_RCP_SUPPORT				0x01
#define MHL_FEATURE_RAP_SUPPORT				0x02
#define MHL_FEATURE_SP_SUPPORT				0x04
#define MHL_FEATURE_UCP_SEND_SUPPORT		0x08
#define MHL_FEATURE_UCP_RECV_SUPPORT		0x10
#define MHL_FEATURE_RBP_SUPPORT				0x40
#define MHL_FEATURE_FLAG_SUPPORT			( \
											MHL_FEATURE_RCP_SUPPORT			\
											| MHL_FEATURE_RAP_SUPPORT		\
											| MHL_FEATURE_SP_SUPPORT		\
											| MHL_FEATURE_UCP_SEND_SUPPORT	\
											| MHL_FEATURE_UCP_RECV_SUPPORT	\
											)

#define MHL_DEVICE_ID						0x8630	/*TRANSCODER_DEVICE_ID*/
#define MHL_SCRATCHPAD_SIZE					0x10
#define MHL_DEVCAP_SIZE						0x10
#define MHL_XDEVCAP_SIZE					0x10
#define MHL_INT_STAT_SIZE					0x33

/* manually define highest number */
#define MHL_MAX_BUFFER_SIZE					MHL_SCRATCHPAD_SIZE

#define MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET	0x80
#define BIT_ECBUS_S_075							(SII_BIT0)
#define BIT_ECBUS_S_8BIT						(SII_BIT1)
#define BIT_ECBUS_S_12BIT						(SII_BIT2)
#define BIT_ECBUS_D_150							(SII_BIT4)
#define BIT_ECBUS_D_8BIT						(SII_BIT5)
#define XDEVCAP_VAL_ECBUS_SPEEDS				(BIT_ECBUS_S_075 | BIT_ECBUS_S_8BIT | BIT_ECBUS_S_12BIT | BIT_ECBUS_D_150 | BIT_ECBUS_D_8BIT)

#define MHL_DEV_XCAP_TMDS_SPEEDS_REG_OFFSET		0x81
#define	BIT_TMDS_150							(SII_BIT0)
#define	BIT_TMDS_300							(SII_BIT1)
#define	BIT_TMDS_600							(SII_BIT2)
#define XDEVCAP_VAL_TMDS_SPEEDS					(BIT_TMDS_150 | BIT_TMDS_300 | BIT_TMDS_600)

#define MHL_DEV_XCAP_ECBUS_DEV_ROLES_REG_OFFSET	0x82
#define	BIT_DEV_HOST							(SII_BIT0)
#define	BIT_DEV_DEVICE							(SII_BIT1)
#define	BIT_DEV_CHARGER							(SII_BIT2)
#define	BIT_DEV_HID_HOST						(SII_BIT3)
#define	BIT_DEV_HID_DEVICE						(SII_BIT4)
#define XDEVCAP_VAL_ECBUS_DEV_ROLES				(BIT_DEV_HOST | BIT_DEV_DEVICE | BIT_DEV_CHARGER)

#define MHL_DEV_XCAP_LOG_DEV_MAP_REG_OFFSET	0x83
#define	BIT_LD_PHONE							(SII_BIT0)
#define XDEVCAP_VAL_LOG_DEV_MAP					BIT_LD_PHONE

#define XDEVCAP_VAL_RESERVED					0x00

/*********************************************************************/

typedef enum
{
    BURST_3D_VIC        = 0x0010,
    BURST_3D_DTD        = 0x0011,
    HEV_VIC             = 0x0020,
    HEV_DTDA            = 0x0021,
    HEV_DTDB            = 0x0022,
    VC_ASSIGN           = 0x0038,
    VC_CONFIRM          = 0x0039,
    AUD_DELAY           = 0x0040,
    ADT_BURSTID         = 0x0041,
    BIST_SETUP          = 0x0051,
    BIST_RETURN_STAT    = 0x0052,
    EMSC_SUPPORT        = 0x0061,
    HID_PAYLOAD         = 0x0062
} _BurstId_e;

//Virtual Channel FEATURE_ID Codes
#define FEATURE_ID_EMSC		(0x00)
#define FEATURE_ID_USB		(0x01)
#define FEATURE_ID_AUDIO	(0x02)
#define FEATURE_ID_IP		(0x03)
#define FEATURE_ID_VIDEO	(0x04)
#define FEATURE_ID_HID		(0x05)

//Virtual Channel Responder RESPONSE Codes
#define RESPONSE_ACCEPT				(0x00)
#define RESPONSE_INVALID_VC_NUM		(0x01)
#define RESPONSE_INVALID_FEATURE_ID (0x02)

#define RESPONSE_INVALID_CHANNEL_SIZE (0x03)

typedef struct _Mhl2HighLow_t
{
    uint8_t high;
    uint8_t low;
} Mhl2HighLow_t;

// see MHL2.0 spec section 5.9.1.2
typedef struct _MhlVideoDescriptor_t
{
    uint8_t reservedHigh;
    uint8_t FrameSequential;	//FS_SUPP
    uint8_t TopBottom;			//TB_SUPP
    uint8_t LeftRight;			//LR_SUPP
    uint8_t reservedLow;
} MhlVideoDescriptor_t;

typedef struct _MhlVideoFormatData_t
{
    Mhl2HighLow_t         burstId;
    uint8_t               checkSum;
    uint8_t               totalEntries;
    uint8_t               sequenceIndex;
    uint8_t               numEntriesThisBurst;
    MhlVideoDescriptor_t  videoDescriptors[5];
} MhlVideoFormatData_t;

typedef struct _MhlVirtualChannelDescriptor_t
{
    uint8_t ucVC_NUM;
    uint8_t ucFEATURE_ID;
    uint8_t ucCHANNEL_SIZE_RESPONSE;
} MhlVirtualChannelDescriptor_t;

typedef struct _MhlVcConfirmFormatData_t
{
    Mhl2HighLow_t                 burstId;
    uint8_t                       ucCheckSum;
    uint8_t                       ucTotalEntries;
    uint8_t                       ucSequenceIndex;
    uint8_t                       ucNumEntriesThisBurst;
    MhlVirtualChannelDescriptor_t virtualChannelDescriptors[3];
    uint8_t                       ucReserved;
} MhlVcConfirmFormatData_t, MhlVcAssignFormatData_t;

typedef enum
{
	MHL_TEST_ADOPTER_ID				= 0x0000,
	burst_id_3D_VIC					= 0x0010,
	burst_id_3D_DTD					= 0x0011,
	burst_id_HEV_VIC				= 0x0020,
	burst_id_HEV_DTDA				= 0x0021,
	burst_id_HEV_DTDB				= 0x0022,
	burst_id_VC_ASSIGN				= 0x0038,
	burst_id_VC_CONFIRM				= 0x0039,
	burst_id_AUD_DELAY				= 0x0040,
	burst_id_ADT_BURSTID			= 0x0041,
	burst_id_BIST_SETUP				= 0x0051,
	burst_id_BIST_RETURN_STAT		= 0x0052,
	burst_id_BIST_DISCARD			= 0x0053,
	burst_id_BIST_ECHO_REQUEST		= 0x0054,
	burst_id_BIST_ECHO_RESPONSE		= 0x0055,
	burst_id_EMSC_SUPPORT			= 0x0061,
	burst_id_HID_PAYLOAD			= 0x0062,
	burst_id_BLK_RCV_BUFFER_INFO	= 0x0063,
	burst_id_BITS_PER_PIXEL_FMT		= 0x0064,
	adopter_id_RANGE_START			= 0x0080,
	LOCAL_ADOPTER_ID				= SILICON_IMAGE_ADOPTER_ID,
	/* add new burst ID's above here */

	/* Burst ID's are a 16-bit big-endian quantity. */
	burst_id_16_BITS_REQUIRED		= 0x8000
} BurstId_e;




typedef struct  {
	uint8_t high;
	uint8_t low;
} MHL_high_low_t;

typedef struct  {
	uint8_t high;
	uint8_t low;
} MHL_burst_id_t;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t remaining_length;
} EMSC_BLK_ADOPT_ID_PAYLD_HDR;

#define ENDIAN_CONVERT_16(a) \
	((((uint16_t)((a).high))<<8)|((uint16_t)((a).low)))
#define BURST_ID(bid) (enum BurstId_e)ENDIAN_CONVERT_16(bid)

#define HIGH_BYTE_16(x) (uint8_t)((x >> 8) & 0xFF)
#define LOW_BYTE_16(x)  (uint8_t)(x & 0xFF)
#define ENCODE_BURST_ID(id) {HIGH_BYTE_16(id), LOW_BYTE_16(id)}

typedef struct  {
	uint8_t rx_unload_ack;
	uint8_t length_remaining;
} standard_transport_header_t;

#define STD_TRANSPORT_HDR_SIZE \
	sizeof(struct  standard_transport_header_t)

typedef struct  {
	/* use the BURST_ID macro to access this */
	MHL_burst_id_t burst_id;
	uint8_t blk_rcv_buffer_size_low;
	uint8_t blk_rcv_buffer_size_high;
} block_rcv_buffer_info_t;

/* see MHL2.0 spec section 5.9.1.2 */
typedef struct  {
	uint8_t reserved_high;
	unsigned char frame_sequential;	/*FB_SUPP*/
	unsigned char top_bottom;		/*TB_SUPP*/
	unsigned char left_right;		/*LR_SUPP*/
	unsigned char reserved_low;
} MHL2_video_descriptor_t;

typedef struct {
	unsigned char frame_sequential;	/*FB_SUPP*/
	unsigned char top_bottom;		/*TB_SUPP*/
	unsigned char left_right;		/*LR_SUPP*/
	unsigned char reserved_low;
} MHL3_vdi_l_t;

typedef struct {
	unsigned char reserved;
} MHL3_vdi_h_t;

/* see MHL3.0 spec section 5.11 */
typedef struct  {
	MHL3_vdi_l_t vdi_l;
	MHL3_vdi_h_t vdi_h;
} MHL3_video_descriptor_t;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t checksum;
	uint8_t total_entries;
	uint8_t sequence_index;
} MHL3_burst_header_t;

typedef struct  {
	MHL3_burst_header_t header;
	uint8_t num_entries_this_burst;
	MHL2_video_descriptor_t video_descriptors[5];
} MHL2_video_format_data_t;

typedef struct  {
	uint8_t vic_cea861f;
	uint8_t reserved;
} MHL3_hev_vic_descriptor_t;

typedef struct  {
	MHL3_burst_header_t header;
	uint8_t num_entries_this_burst;
	MHL3_hev_vic_descriptor_t video_descriptors[5];
} MHL3_hev_vic_data_t;

typedef struct  {
	MHL_high_low_t pixel_clock_in_MHz;
	MHL_high_low_t h_active_in_pixels;
	MHL_high_low_t h_blank_in_pixels;
	MHL_high_low_t h_front_porch_in_pixels;
	MHL_high_low_t h_sync_width_in_pixels;
	uint8_t h_flags;
} MHL3_hev_dtd_a_payload_t;

typedef struct  {
	MHL_high_low_t v_total_in_lines;
	uint8_t v_blank_in_lines;	/* note 7 for table 5-16 is wrong */
	uint8_t v_front_porch_in_lines;	/* note 7 for table 5-16 is wrong */
	uint8_t v_sync_width_in_lines;	/* note 7 for table 5-16 is wrong */
	uint8_t v_refresh_rate_in_fields_per_second;
	uint8_t v_flags;
	uint8_t reserved[4];
} MHL3_hev_dtd_b_payload_t;

typedef struct   {
	MHL3_burst_header_t header;
	MHL3_hev_dtd_a_payload_t payload;
} MHL3_hev_dtd_a_data_t;

typedef struct  {
	MHL3_burst_header_t header;
	MHL3_hev_dtd_b_payload_t payload;
} MHL3_hev_dtd_b_data_t;

typedef struct  {
	uint8_t sequence_index;
	MHL3_hev_dtd_a_payload_t a;
	MHL3_hev_dtd_b_payload_t b;
	MHL3_video_descriptor_t _3d_info;
} MHL3_hev_dtd_item_t;

typedef struct  {
	unsigned char VIC;
	unsigned char native;
} cea_short_descriptor_t;

typedef struct {
	MHL3_hev_vic_descriptor_t mhl3_hev_vic_descriptor;
	MHL3_video_descriptor_t _3d_info;
} MHL3_hev_vic_item_t;

typedef struct {
	cea_short_descriptor_t svd;
	MHL3_video_descriptor_t _3d_info;
} MHL3_3d_vic_item_t;

typedef struct 
{
	unsigned char horz_blanking_11_8;
	unsigned char horz_active_11_8;
} HorzActiveBlankingHigh_t;

typedef struct 
{
	unsigned char vert_blanking_11_8;
	unsigned char vert_active_11_8;
} VertActiveBlankingHigh_t;

typedef struct 
{
	unsigned char vert_sync_pulse_width_3_0;
	unsigned char vert_sync_offset_3_0;
} VertSyncOffsetWidth_t;
typedef struct 
{
	unsigned char vert_sync_pulse_width_5_4;
	unsigned char vert_sync_offset_5_4;
	unsigned char horz_sync_pulse_width_9_8;
	unsigned char horz_sync_offset_9_8;

} HsVsOffsetPulseWidth_t;

typedef struct 
{
	unsigned char vert_image_size_in_mm_11_8;
	unsigned char horz_image_size_in_mm_11_8;
} ImageSizeHigh_t;

typedef struct 
{
	unsigned char stereo_bit_0;
	unsigned char sync_signal_options;
	unsigned char sync_signal_type;
	unsigned char stereo_bits_2_1;
	unsigned char interlaced;
} Flags_t;

typedef struct  {
	uint8_t pixel_clock_low;
	uint8_t pixel_clock_high;
	uint8_t horz_active_7_0;
	uint8_t horz_blanking_7_0;
	HorzActiveBlankingHigh_t horz_active_blanking_high;
	uint8_t vert_active_7_0;
	uint8_t vert_blanking_7_0;
	VertActiveBlankingHigh_t vert_active_blanking_high;
	uint8_t horz_sync_offset_7_0;
	uint8_t horz_sync_pulse_width7_0;
	VertSyncOffsetWidth_t vert_sync_offset_width;
	HsVsOffsetPulseWidth_t hs_vs_offset_pulse_width;
	uint8_t horz_image_size_in_mm_7_0;
	uint8_t vert_image_size_in_mm_7_0;
	ImageSizeHigh_t image_size_high;
	uint8_t horz_border_in_lines;
	uint8_t vert_border_in_pixels;
	Flags_t flags;
} detailed_timing_descriptor_t;

typedef struct {
	uint8_t index;
	detailed_timing_descriptor_t dtd_cea_861;
	MHL3_video_descriptor_t _3d_info;
} MHL3_3d_dtd_item_t;

typedef struct   {
	uint8_t cea861f_spkr_alloc[3];
} MHL3_speaker_allocation_data_block_t;

typedef struct  {
	uint8_t format_flags;
	union {
		uint8_t short_descs[9];
		MHL3_speaker_allocation_data_block_t spkr_alloc_db[3];
	} descriptors;
	uint8_t reserved;
} MHL3_adt_payload_t;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t checksum;
	uint8_t delay_h;
	uint8_t delay_m;
	uint8_t delay_l;
	uint8_t reserved[10];
} MHL3_audio_delay_burst_t;

typedef struct  {
	MHL3_burst_header_t header;
	MHL3_adt_payload_t payload;
} MHL3_adt_data_t;

typedef struct  {
	MHL_burst_id_t burst_ids[5];
} MHL3_emsc_support_payload_t;

typedef struct  {
	MHL3_burst_header_t header;
	uint8_t num_entries_this_burst;
	MHL3_emsc_support_payload_t payload;
} MHL3_emsc_support_data_t;

typedef enum {
	VIEW_PIX_FMT_24BPP,
	VIEW_PIX_FMT_16BPP
} view_pixel_fmt_e;

typedef struct  {
	uint8_t stream_id;
	uint8_t stream_pixel_format;
} MHL3_bits_per_pixel_fmt_descriptor_t;

typedef struct  {
	MHL3_burst_header_t header;
	uint8_t num_entries_this_burst;

	/* reserve 5 for use with WRITE_BURST
	   actual length is variable, indicated by
	   num_entries_this_burst
	 */
	/* todo change this to 1 when WRITE_BURST OPTION is removed */
	MHL3_bits_per_pixel_fmt_descriptor_t descriptors[5];
} MHL_bits_per_pixel_fmt_data_t;

typedef union  {
	MHL2_video_descriptor_t mhl2_3d_descriptor;
	MHL3_video_descriptor_t mhl3_3d_descriptor;
	MHL3_hev_vic_descriptor_t mhl3_hev_vic_descriptor;
	MHL3_hev_dtd_item_t mhl3_hev_dtd;
} video_burst_descriptor_u;

#if 0
typedef struct  mhl3_vsif_t {
	info_frame_header_t header;
	uint8_t checksum;
	uint8_t iee_oui[3];
	uint8_t pb4;
	uint8_t pb5_reserved;
	uint8_t pb6;
	MHL_high_low_t mhl_hev_fmt_type;
	uint8_t pb9;

	MHL_high_low_t av_delay_sync;
};
#endif
/* the enum's in the following section are
	defined "in position" to avoid
	shifting on the fly
*/
#define MHL3_VSIF_TYPE		0x81
#define MHL3_VSIF_VERSION	0x03
#define IEEE_OUI_MHL3		0x7CA61D

#define	PB4_MASK_MHL_VID_FMT	0x03
typedef enum {
	mhl_vid_fmt_no_additional,
	mhl_vid_fmt_3d_fmt_present,
	mhl_vid_fmt_multi_view,
	mhl_vid_fmt_dual_3d
} mhl_vid_fmt_e;

#define PB4_MASK_MHL_3D_FMT_TYPE	0x1C
typedef enum {
	MHL_3D_FMT_TYPE_FS,		/* Frame Sequential */
	MHL_3D_FMT_TYPE_TB		= 0x04,	/* Top-Bottom */
	MHL_3D_FMT_TYPE_LR		= 0x08,	/* Left-Right */
	MHL_3D_FMT_TYPE_FS_TB	= 0x0C,	/* Frame Sequential Top-Bottom */
	MHL_3D_FMT_TYPE_FS_LR	= 0x10,	/* Frame Sequential Left-Right */
	MHL_3D_FMT_TYPE_TBLR	= 0x14	/* Top-Bottom-Left-Right */
} mhl_3d_fmt_type_e;

#define PB4_MASK_SEP_AUD	0x20
typedef enum {
	mhl_sep_audio_not_available,
	mhl_sep_audio_available = 0x20
} mhl_sep_audio_e;

#define PB4_MASK_RESERVED	0xC0

#define MHL3_VSIF_PB4(vid_fmt, _3d_fmt_type, sep_aud) \
	(uint8_t)(((vid_fmt)&PB4_MASK_MHL_VID_FMT) | \
	((_3d_fmt_type)&PB4_MASK_MHL_3D_FMT_TYPE) | \
	((sep_aud)&PB4_MASK_SEP_AUD))

#define PB6_MASK_MHL_HEV_FMT		0x03
typedef enum {
	mhl_hev_fmt_no_additional,
	mhl_hev_fmt_hev_present,
	mhl_hev_fmt_reserved_2,
	mhl_hev_fmt_reserved_3
} mhl_hev_fmt_e;

#define PB6_MASK_RESERVED	0xFC
#define MHL3_VSIF_PB6(hev_fmt) (uint8_t)((hev_fmt) & PB6_MASK_MHL_HEV_FMT)

#define PB9_MASK_AV_DELAY_SYNC_19_16	0x0F
#define PB9_MASK_AV_DELAY_DIR			0x10

typedef enum {
	mhl_av_delay_dir_audio_earlier,
	mhl_av_delay_dir_video_earlier = 0x10
} mhl_av_delay_dir_e;

#define PB9_MASK_RESERVED	0xE0

#define MHL3_VSIF_PB9(delay_sync, delay_dir) \
	(uint8_t)((((delay_sync) >> 16) & PB9_MASK_AV_DELAY_SYNC_19_16) | \
	((delay_dir) & PB9_MASK_AV_DELAY_DIR))
#if 0
typedef struct  info_frame_t {
	union {
		info_frame_header_t header;
		avi_info_frame_t avi;
		vendor_specific_info_frame_t vendorSpecific;
		mhl3_vsif_t mhl3_vsif;
		unr_info_frame_t unr;
	} body;
};
#endif
typedef enum {
	InfoFrameType_AVI,
	InfoFrameType_VendorSpecific,	
	InfoFrameType_VendorSpecific_MHL3,
	InfoFrameType_Audio
} InfoFrameType_e;

enum {
	MHL_MSC_MSGE					= 0x02,	/* MSC_MSG Error sub-command */
	MHL_MSC_MSG_RCP					= 0x10, /* RCP sub-command */
	MHL_MSC_MSG_RCPK				= 0x11, /* RCP Acknowledge sub-command */
	MHL_MSC_MSG_RCPE				= 0x12, /* RCP Error sub-command */
	MHL_MSC_MSG_RAP					= 0x20, /* Mode Change Warning sub-command */
	MHL_MSC_MSG_RAPK				= 0x21, /* MCW Acknowledge sub-command */
	MHL_MSC_MSG_RBP					= 0x22, /* Remote Button Protocol sub-command */
	MHL_MSC_MSG_RBPK				= 0x23, /* RBP Acknowledge sub-command */
	MHL_MSC_MSG_RBPE				= 0x24, /* RBP Error sub-command */
	MHL_MSC_MSG_UCP					= 0x30, /* UCP sub-command */
	MHL_MSC_MSG_UCPK				= 0x31, /* UCP Acknowledge sub-command */
	MHL_MSC_MSG_UCPE				= 0x32, /* UCP Error sub-command */
	MHL_MSC_MSG_RUSB				= 0x40, /* Request USB host role */
	MHL_MSC_MSG_RUSBK				= 0x41, /* Acknowledge request for USB host role */
	MHL_MSC_MSG_RHID				= 0x42, /* Request HID host role */
	MHL_MSC_MSG_RHIDK				= 0x43, /* Acknowledge request for HID host role */
	MHL_MSC_MSG_ATT					= 0x50,	/* Request attention sub-command */
	MHL_MSC_MSG_ATTK				= 0x51, /* ATT Acknowledge sub-command */
	MHL_MSC_MSG_BIST_TRIGGER		= 0x60,
	MHL_MSC_MSG_BIST_REQUEST_STAT	= 0x61,
	MHL_MSC_MSG_BIST_READY			= 0x62,
	MHL_MSC_MSG_BIST_STOP			= 0x63,
};

#define BIST_TRIGGER_E_CBUS_TX			0x01
#define BIST_TRIGGER_E_CBUS_RX			0x02
#define BIST_TRIGGER_E_CBUS_LOOPBACK	0x04
#define BIST_TRIGGER_E_CBUS_TYPE_MASK	0x08
#define BIST_TRIGGER_TEST_E_CBUS_S		0x00
#define BIST_TRIGGER_TEST_E_CBUS_D		0x08
#define BIST_TRIGGER_AVLINK_TX			0x10
#define BIST_TRIGGER_AVLINK_RX			0x20
#define BIST_TRIGGER_IMPEDANCE_TEST		0x40

#define BIST_TRIGGER_ECBUS_AV_LINK_MASK	(BIST_TRIGGER_AVLINK_TX | \
					 BIST_TRIGGER_AVLINK_RX)
#define BIST_TRIGGER_ECBUS_TX_RX_MASK (BIST_TRIGGER_E_CBUS_TX | \
					BIST_TRIGGER_E_CBUS_RX | \
					BIST_TRIGGER_E_CBUS_LOOPBACK)

#define BIST_TRIGGER_OPERAND_VALID_MASK	0x7B

#define BIST_READY_E_CBUS_READY		0x01
#define BIST_READY_AVLINK_READY		0x02
#define BIST_READY_TERM_READY		0x04
#define BIST_READY_E_CBUS_ERROR		0x10
#define BIST_READY_AVLINK_ERROR		0x20
#define BIST_READY_TERM_ERROR		0x40

#define RCPE_NO_ERROR				0x00
#define RCPE_INEFFECTIVE_KEY_CODE	0x01
#define RCPE_BUSY					0x02

#define MHL_RCP_KEY_RELEASED_MASK	0x80
#define MHL_RCP_KEY_ID_MASK			0x7F

#define RBPE_NO_ERROR					0x00
#define RBPE_INEFFECTIVE_BUTTON_CODE	0x01
#define RBPE_BUSY						0x02

#define MHL_RBP_BUTTON_RELEASED_MASK	0x80
#define MHL_RBP_BUTTON_ID_MASK			0x7F

#define T_PRESS_MODE			300
#define T_HOLD_MAINTAIN			2000
#define T_RAP_WAIT_MIN			100
#define T_RAP_WAIT_MAX			1000

/* RAP action codes */
#define MHL_RAP_CMD_POLL			0x00	/* Just do an ack */
#define MHL_RAP_CMD_CONTENT_ON		0x10	/* Turn content stream ON */
#define MHL_RAP_CMD_CONTENT_OFF		0x11	/* Turn content stream OFF */
#define MHL_RAP_CMD_CBUS_MODE_DOWN	0x20
#define MHL_RAP_CMD_CBUS_MODE_UP	0x21

/* RAPK status codes */
#define MHL_RAPK_NO_ERR				0x00	/* RAP action recognized & supported */
#define MHL_RAPK_UNRECOGNIZED		0x01	/* Unknown RAP action code received */
#define MHL_RAPK_UNSUPPORTED		0x02	/* Rcvd RAP action code not supported */
#define MHL_RAPK_BUSY				0x03	/* Responder too busy to respond */

/*
 * Error status codes for RCPE messages
 */
/* No error. (Not allowed in RCPE messages) */
#define MHL_RCPE_STATUS_NO_ERROR				0x00
/* Unsupported/unrecognized key code */
#define MHL_RCPE_STATUS_INEFFECTIVE_KEY_CODE	0x01
/* Responder busy. Initiator may retry message */
#define MHL_RCPE_STATUS_BUSY					0x02

/*
 * Error status codes for RBPE messages
 */
/* No error. (Not allowed in RBPE messages) */
#define MHL_RBPE_STATUS_NO_ERROR				0x00
/* Unsupported/unrecognized button code */
#define MHL_RBPE_STATUS_INEFFECTIVE_BUTTON_CODE	0x01
/* Responder busy. Initiator may retry message */
#define MHL_RBPE_STATUS_BUSY					0x02

/*
 * Error status codes for UCPE messages
 */
/* No error. (Not allowed in UCPE messages) */
#define MHL_UCPE_STATUS_NO_ERROR				0x00
/* Unsupported/unrecognized key code */
#define MHL_UCPE_STATUS_INEFFECTIVE_KEY_CODE	0x01

/* Extended Device Capability Registers 7.12.1 */
enum {
	XDEVCAP_START					= 0x80,
	XDEVCAP_ADDR_ECBUS_SPEEDS		= XDEVCAP_START,
	XDEVCAP_ADDR_TMDS_SPEEDS		= 0x81,
	XDEVCAP_ADDR_ECBUS_DEV_ROLES	= 0x82,
	XDEVCAP_ADDR_LOG_DEV_MAPX		= 0x83,
	XDEVCAP_LIMIT,	/* don't hard-code this one */
	XDEVCAP_ADDR_RESERVED_4			= 0x84,
	XDEVCAP_ADDR_RESERVED_5			= 0x85,
	XDEVCAP_ADDR_RESERVED_6			= 0x86,
	XDEVCAP_ADDR_RESERVED_7			= 0x87,
	XDEVCAP_ADDR_RESERVED_8			= 0x88,
	XDEVCAP_ADDR_RESERVED_9			= 0x89,
	XDEVCAP_ADDR_RESERVED_A			= 0x8A,
	XDEVCAP_ADDR_RESERVED_B			= 0x8B,
	XDEVCAP_ADDR_RESERVED_C			= 0x8C,
	XDEVCAP_ADDR_RESERVED_D			= 0x8D,
	XDEVCAP_ADDR_RESERVED_E			= 0x8E,
	XDEVCAP_ADDR_RESERVED_F			= 0x8F,
	XDEVCAP_ADDR_LAST,	/* this	one must be last */
	XDEVCAP_SIZE					= XDEVCAP_ADDR_LAST - XDEVCAP_START
};

#define XDEVCAP_OFFSET(reg) (reg - XDEVCAP_ADDR_ECBUS_SPEEDS)


/* XDEVCAP - eCBUS Speeds 7.12.1.1 */
#define MHL_XDC_ECBUS_S_075			0x01
#define MHL_XDC_ECBUS_S_8BIT		0x02
#define MHL_XDC_ECBUS_S_12BIT		0x04
#define MHL_XDC_ECBUS_D_150			0x10
#define MHL_XDC_ECBUS_D_8BIT		0x20

/* XDEVCAP - TMDS Speeds 7.12.1.2 */
#define MHL_XDC_TMDS_000			0x00
#define MHL_XDC_TMDS_150			0x01
#define MHL_XDC_TMDS_300			0x02
#define MHL_XDC_TMDS_600			0x04

/* XDEVCAP - Device Roles 7.12.1.3 */
#define MHL_XDC_DEV_HOST			0x01
#define MHL_XDC_DEV_DEVICE			0x02
#define MHL_XDC_DEV_CHARGER			0x04
#define MHL_XDC_HID_HOST			0x08
#define MHL_XDC_HID_DEVICE			0x10

/* XDEVCAP - Extended Logical Device Map 7.12.1.4 */
#define MHL_XDC_LD_PHONE			0x01

/* Extended Device Status Registers 7.12.2 */
enum {
	XDEVSTAT_OFFSET_CURR_ECBUS_MODE,
	XDEVSTAT_OFFSET_AVLINK_MODE_STATUS,
	XDEVSTAT_OFFSET_AVLINK_MODE_CONTROL,
	XDEVSTAT_OFFSET_MULTI_SINK_STATUS,
	XDEVSTAT_OFFSET_RESERVED_04,
	XDEVSTAT_OFFSET_RESERVED_05,
	XDEVSTAT_OFFSET_RESERVED_06,
	XDEVSTAT_OFFSET_RESERVED_07,
	XDEVSTAT_OFFSET_RESERVED_08,
	XDEVSTAT_OFFSET_RESERVED_09,
	XDEVSTAT_OFFSET_RESERVED_0A,
	XDEVSTAT_OFFSET_RESERVED_0B,
	XDEVSTAT_OFFSET_RESERVED_0C,
	XDEVSTAT_OFFSET_RESERVED_0D,
	XDEVSTAT_OFFSET_RESERVED_0E,
	XDEVSTAT_OFFSET_RESERVED_0F,
	XDEVSTAT_OFFSET_RESERVED_10,
	XDEVSTAT_OFFSET_RESERVED_11,
	XDEVSTAT_OFFSET_RESERVED_12,
	XDEVSTAT_OFFSET_RESERVED_13,
	XDEVSTAT_OFFSET_RESERVED_14,
	XDEVSTAT_OFFSET_RESERVED_15,
	XDEVSTAT_OFFSET_RESERVED_16,
	XDEVSTAT_OFFSET_RESERVED_17,
	XDEVSTAT_OFFSET_RESERVED_18,
	XDEVSTAT_OFFSET_RESERVED_19,
	XDEVSTAT_OFFSET_RESERVED_1A,
	XDEVSTAT_OFFSET_RESERVED_1B,
	XDEVSTAT_OFFSET_RESERVED_1C,
	XDEVSTAT_OFFSET_RESERVED_1D,
	XDEVSTAT_OFFSET_RESERVED_1E,
	XDEVSTAT_OFFSET_RESERVED_1F,
	/* this one must be last */
	XDEVSTAT_SIZE
};

/* XDEVSTAT - Current eCBUS Mode 7.12.2.1 */
#define MHL_XSTATUS_REG_CBUS_MODE		0x90
#define MHL_XDS_SLOT_MODE_8BIT			0x00
#define MHL_XDS_SLOT_MODE_6BIT			0x01
#define MHL_XDS_ECBUS_S					0x04
#define MHL_XDS_ECBUS_D					0x08

#define MHL_XDS_LINK_CLOCK_75MHZ		0x00
#define MHL_XDS_LINK_CLOCK_150MHZ		0x10
#define MHL_XDS_LINK_CLOCK_300MHZ		0x20
#define MHL_XDS_LINK_CLOCK_600MHZ		0x30

/* XDEVSTAT - AV Link Mode Status 7.12.2.2 */
#define MHL_XDS_LINK_STATUS_NO_SIGNAL		0x00
#define MHL_XDS_LINK_STATUS_CRU_LOCKED		0x01
#define MHL_XDS_LINK_STATUS_TMDS_NORMAL		0x02
#define MHL_XDS_LINK_STATUS_TMDS_RESERVED	0x03

/* XDEVSTAT - AV Link Mode Control 7.12.2.3 */
#define MHL_STATUS_REG_AV_LINK_MODE_CONTROL	0x92
#define MHL_XDS_LINK_RATE_1_5_GBPS			0x00
#define MHL_XDS_LINK_RATE_3_0_GBPS			0x01
#define MHL_XDS_LINK_RATE_6_0_GBPS			0x02
#define MHL_XDS_ATT_CAPABLE					0x08

/* XDEVSTAT - Multi-Sink Status 7.12.2.4 */
#define MHL_XDS_SINK_STATUS_1_HPD_LOW		0x00
#define MHL_XDS_SINK_STATUS_1_HPD_HIGH		0x01
#define MHL_XDS_SINK_STATUS_2_HPD_LOW		0x00
#define MHL_XDS_SINK_STATUS_2_HPD_HIGH		0x04
#define MHL_XDS_SINK_STATUS_3_HPD_LOW		0x00
#define MHL_XDS_SINK_STATUS_3_HPD_HIGH		0x10
#define MHL_XDS_SINK_STATUS_4_HPD_LOW		0x00
#define MHL_XDS_SINK_STATUS_4_HPD_HIGH		0x40

/*
 * Define format of Write Burst used in MHL 3
 * to assign TDM slots to virtual channels.
 */
typedef struct  {
	uint8_t vc_num;
	uint8_t feature_id;
	union {
		uint8_t channel_size;
		uint8_t response;
	} req_resp;
} virt_chan_info;

#define MAX_VC_ENTRIES	3
typedef struct  {
	MHL3_burst_header_t header;
	uint8_t num_entries_this_burst;
	virt_chan_info vc_info[MAX_VC_ENTRIES];
	uint8_t reserved;
} tdm_alloc_burst;

/* BIST_SETUP WRITE_BURST 15.1.1 */
#define BIST_ECBUS_PATTERN_UNSPECIFIED	0x00
#define BIST_ECBUS_PATTERN_PRBS			0x01
#define BIST_ECBUS_PATTERN_FIXED_8		0x02
#define BIST_ECBUS_PATTERN_FIXED_10		0x03
#define BIST_ECBUS_PATTERN_MAX			BIST_ECBUS_PATTERN_FIXED_10

#define BIST_AVLINK_DATA_RATE_UNSPECIFIED	0x00
#define BIST_AVLINK_DATA_RATE_1500MBPS		0x01
#define BIST_AVLINK_DATA_RATE_3000MBPS		0x02
#define BIST_AVLINK_DATA_RATE_6000MBPS		0x03
#define BIST_AVLINK_DATA_RATE_MAX			BIST_AVLINK_DATA_RATE_6000MBPS

#define BIST_AVLINK_PATTERN_UNSPECIFIED		0x00
#define BIST_AVLINK_PATTERN_PRBS			0x01
#define BIST_AVLINK_PATTERN_FIXED_8			0x02
#define BIST_AVLINK_PATTERN_FIXED_10		0x03
#define BIST_AVLINK_PATTERN_MAX				BIST_AVLINK_PATTERN_FIXED_10

#define BIST_IMPEDANCE_MODE_AVLINK_TX_LOW		0x00
#define BIST_IMPEDANCE_MODE_AVLINK_TX_HIGH		0x01
#define BIST_IMPEDANCE_MODE_AVLINK_RX			0x02
#define BIST_IMPEDANCE_MODE_RESERVED_1			0x03
#define BIST_IMPEDANCE_MODE_ECBUS_D_TX_LOW		0x04
#define BIST_IMPEDANCE_MODE_ECBUS_D_TX_HIGH		0x05
#define BIST_IMPEDANCE_MODE_ECBUS_D_RX			0x06
#define BIST_IMPEDANCE_MODE_RESERVED_2			0x07
#define BIST_IMPEDANCE_MODE_ECBUS_S_TX_LOW		0x08
#define BIST_IMPEDANCE_MODE_ECBUS_S_TX_HIGH		0x09
#define BIST_IMPEDANCE_MODE_ECBUS_S_RX			0x0A
#define BIST_IMPEDANCE_MODE_MAX					BIST_IMPEDANCE_MODE_ECBUS_S_RX

typedef struct  {
	uint8_t burst_id_h;
	uint8_t burst_id_l;
	uint8_t checksum;
	uint8_t e_cbus_duration;
	uint8_t e_cbus_pattern;
	uint8_t e_cbus_fixed_h;
	uint8_t e_cbus_fixed_l;
	uint8_t reserved;
	uint8_t avlink_data_rate;
	uint8_t avlink_pattern;
	uint8_t avlink_video_mode;
	uint8_t avlink_duration;
	uint8_t avlink_fixed_h;
	uint8_t avlink_fixed_l;
	uint8_t avlink_randomizer;
	uint8_t impedance_mode;
} bist_setup_burst;

/* BIST_RETURN_STAT WRITE_BURST 15.1.2 */
typedef struct  {
	uint8_t burst_id_h;
	uint8_t burst_id_l;
	uint8_t checksum;
	uint8_t reserved[9];
	uint8_t e_cbus_stat_h;
	uint8_t e_cbus_stat_l;
	uint8_t avlink_stat_h;
	uint8_t avlink_stat_l;
} bist_return_stat_burst;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t	remaining_length;
} bist_discard_burst_hdr;

typedef struct  {
	bist_discard_burst_hdr hdr;
	uint8_t payload[13];
} bist_discard_burst;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t	remaining_length;
} bist_echo_request_burst_hdr;

typedef struct  {
	bist_echo_request_burst_hdr hdr;
	uint8_t payload[13];
} bist_echo_request_burst;

typedef struct  {
	MHL_burst_id_t burst_id;
	uint8_t	remaining_length;
} bist_echo_response_burst_hdr;

typedef struct  {
	bist_echo_response_burst_hdr hdr;
	uint8_t payload[13];
} bist_echo_response_burst;

#define MHL_T_src_vbus_cbus_stable_min	100

#endif //__SI_MOD_TX_CBUS_DEFS_H__