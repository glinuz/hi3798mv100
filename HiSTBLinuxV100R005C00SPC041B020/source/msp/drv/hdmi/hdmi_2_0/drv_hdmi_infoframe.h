
#ifndef __INFOFRAME_H_
#define __INFOFRAME_H_

#include "hi_type.h"


#define SET_BIT(x) (0x1 << (x))

#define HDMI_INFOFRAME_HEADER_SIZE  4
#define HDMI_AVI_INFOFRAME_SIZE    13
#define HDMI_SPD_INFOFRAME_SIZE    25
#define HDMI_AUDIO_INFOFRAME_SIZE  10

enum hdmi_infoframe_type {
	HDMI_INFOFRAME_TYPE_VENDOR = 0x81,
	HDMI_INFOFRAME_TYPE_AVI = 0x82,
	HDMI_INFOFRAME_TYPE_SPD = 0x83,
	HDMI_INFOFRAME_TYPE_AUDIO = 0x84,
	HDMI_INFOFRAME_TYPE_MPEG = 0X85,
	HDMI_INFOFRAME_TYPE_GBD = 0X86,
    HDMI_INFOFRAME_TYPE_DRM = 0x87,
    
	HDMI_INFOFRAME_TYPE_BUTT,
};


enum hdmi_colorspace {
	HDMI_COLORSPACE_RGB,
	HDMI_COLORSPACE_YCbCr422,
	HDMI_COLORSPACE_YCbCr444,
	HDMI_COLORSPACE_YCbCr420,
	HDMI_COLORSPACE_BUTT,
};

enum hdmi_scan_mode {
	HDMI_SCAN_MODE_NONE,
	HDMI_SCAN_MODE_OVERSCAN,
	HDMI_SCAN_MODE_UNDERSCAN,
	HDMI_SCAN_MODE_FUTURE,          /* reserved */
	HDMI_SCAN_MODE_BUTT
};

enum hdmi_colorimetry {
	HDMI_COLORIMETRY__NONE,
	HDMI_COLORIMETRY__ITU_601,
	HDMI_COLORIMETRY__ITU_709,
	HDMI_COLORIMETRY__EXTENDED,
};

enum hdmi_picture_aspect {
	HDMI_PICTURE_ASPECT_NONE,
	HDMI_PICTURE_ASPECT_4_3,
	HDMI_PICTURE_ASPECT_16_9,
	HDMI_PICTURE_ASPECT_64_27,
	HDMI_PICTURE_ASPECT_256_135,
	HDMI_PICTURE_ASPECT_FUTURE,
	HDMI_PICTURE_ASPECT_BUTT,
};

enum hdmi_active_aspect {
	HDMI_ACTIVE_ASPECT_16_9_TOP = 2,
	HDMI_ACTIVE_ASPECT_14_9_TOP = 3,
	HDMI_ACTIVE_ASPECT_16_9_CENTER = 4,
	HDMI_ACTIVE_ASPECT_PICTURE = 8,
	HDMI_ACTIVE_ASPECT_4_3 = 9,
	HDMI_ACTIVE_ASPECT_16_9 = 10,
	HDMI_ACTIVE_ASPECT_14_9 = 11,
	HDMI_ACTIVE_ASPECT_4_3_SP_14_9 = 13,
	HDMI_ACTIVE_ASPECT_16_9_SP_14_9 = 14,
	HDMI_ACTIVE_ASPECT_16_9_SP_4_3 = 15,
	HDMI_ACTIVE_ASPECT_BUTT
};

enum hdmi_extended_colorimetry {
	HDMI_EXTENDED_COLORIMETRY_XV_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_XV_YCC_709,
	HDMI_EXTENDED_COLORIMETRY_S_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601,
	HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB,
	HDMI_EXTENDED_COLORIMETRY_2020_const_luminous,
	HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous,
	HDMI_EXTENDED_COLORIMETRY_BUTT
};

enum hdmi_quantization_range {
	HDMI_QUANTIZATION_RANGE_DEFAULT,
	HDMI_QUANTIZATION_RANGE_LIMITED,
	HDMI_QUANTIZATION_RANGE_FULL,
	HDMI_QUANTIZATION_RANGE_BUTT
};

/* non-uniform picture scaling */
enum hdmi_nups {
	HDMI_NUPS_UNKNOWN,
	HDMI_NUPS_HORIZONTAL,
	HDMI_NUPS_VERTICAL,
	HDMI_NUPS_BOTH,
};

enum hdmi_ycc_quantization_range {
	HDMI_YCC_QUANTIZATION_RANGE_LIMITED,
	HDMI_YCC_QUANTIZATION_RANGE_FULL,
	HDMI_YCC_QUANTIZATION_RANGE_BUTT
};

enum hdmi_content_type {
	HDMI_CONTENT_TYPE_NONE,
	HDMI_CONTENT_TYPE_PHOTO,
	HDMI_CONTENT_TYPE_CINEMA,
	HDMI_CONTENT_TYPE_GAME,
};


typedef enum hdmi_video_code{
    HDMI_640X480P60_4_3 = 1,
    HDMI_720X480P60_4_3,
    HDMI_720X480P60_16_9,
    HDMI_1280X720P60_16_9,
    HDMI_1920X1080I60_16_9,
    HDMI_1440X480I60_4_3,
    HDMI_1440X480I60_16_9,	    // 7
    HDMI_1440X240P60_4_3,
    HDMI_1440X240P60_16_9, 
    HDMI_2880X480I60_4_3,
    HDMI_2880X480I60_16_9,
    HDMI_2880X240P60_4_3,	    // 12
    HDMI_2880X240P60_16_9,    
    HDMI_1440X480P60_4_3,
    HDMI_1440X480P60_16_9,  
    HDMI_1920X1080P60_16_9, 
    HDMI_720X576P50_4_3,
    HDMI_720X576P50_16_9,
    HDMI_1280X720P50_16_9,
    HDMI_1920X1080I50_16_9,
    HDMI_1440X576I50_4_3,
    HDMI_1440X576I50_16_9,
    HDMI_1440X288P50_4_3,
    HDMI_1440X288P50_16_9,      // 24

    HDMI_2880X576I50_4_3,
	HDMI_2880X576I50_16_9,
	HDMI_2880X288P50_4_3,
	HDMI_2880X288P50_16_9,
	HDMI_1440X576P50_4_3,
	HDMI_1440X576P50_16_9,      // 30
    
    HDMI_1920X1080P50_16_9, 
    HDMI_1920X1080P24_16_9,
    HDMI_1920X1080P25_16_9,
    HDMI_1920X1080P30_16_9,
    HDMI_2880X480P60_4_3,
    HDMI_2880X480P60_16_9,
    HDMI_2880X576P50_4_3,
    HDMI_2880X576P50_16_9,      // 38
	HDMI_1920X1080I50_16_9_1250,
	HDMI_1920X1080I100_16_9,
	HDMI_1280X720P100_16_9,
	HDMI_720X576P100_4_3,
	HDMI_720X576P100_16_9,
	HDMI_1440X576I100_4_3,
	HDMI_1440X576I100_16_9,	    // 45
	HDMI_1920X1080I120_16_9,
	HDMI_1280X720P120_16_9,
	HDMI_720X480P120_4_3,
	HDMI_720X480P120_16_9,
	HDMI_1440X480I120_4_3,
	HDMI_1440X480I120_16_9,		// 51
	HDMI_720X576P200_4_3,
	HDMI_720X576P200_16_9,
	HDMI_1440X576I200_4_3,
	HDMI_1440X576I200_16_9,
	HDMI_720X480P240_4_3,
	HDMI_720X480P240_16_9,
	HDMI_1440X480I240_4_3,
	HDMI_1440X480I240_16_9,		// 59
	HDMI_1280X720P24_16_9,
	HDMI_1280X720P25_16_9,
	HDMI_1280X720P30_16_9,
	HDMI_1920X1080P120_16_9,
	HDMI_1920X1080P100_16_9,
    HDMI_1280X720P24_64_27,
    HDMI_1280X720P25_64_27,
    HDMI_1280X720P30_64_27,
    HDMI_1280X720P50_64_27,
    HDMI_1280X720P60_64_27,     // 69
	HDMI_1280X720P100_64_27,
	HDMI_1280X720P120_64_27,
    HDMI_1920X1080P24_64_27,
    HDMI_1920X1080P25_64_27,
    HDMI_1920X1080P30_64_27,
    HDMI_1920X1080P50_64_27,
    HDMI_1920X1080P60_64_27,	// 76
	HDMI_1920X1080P100_64_27,
	HDMI_1920X1080P120_64_27,
	HDMI_1680X720P24_64_27,
	HDMI_1680X720P25_64_27,
	HDMI_1680X720P30_64_27,
	HDMI_1680X720P50_64_27,
	HDMI_1680X720P60_64_27,		// 83
	HDMI_1680X720P100_64_27,
	HDMI_1680X720P120_64_27,
	HDMI_2560X1080P24_64_27,
	HDMI_2560X1080P25_64_27,
	HDMI_2560X1080P30_64_27,
	HDMI_2560X1080P50_64_27,
	HDMI_2560X1080P60_64_27,
	HDMI_2560X1080P100_64_27,
	HDMI_2560X1080P120_64_27,	// 92
	
    HDMI_3840X2160P24_16_9,
    HDMI_3840X2160P25_16_9,
    HDMI_3840X2160P30_16_9,
    HDMI_3840X2160P50_16_9,
    HDMI_3840X2160P60_16_9,
    HDMI_4096X2160P24_256_135,
    HDMI_4096X2160P25_256_135,
    HDMI_4096X2160P30_256_135,
    HDMI_4096X2160P50_256_135,
    HDMI_4096X2160P60_256_135,
    HDMI_3840X2160P24_64_27,
    HDMI_3840X2160P25_64_27,
    HDMI_3840X2160P30_64_27,
    HDMI_3840X2160P50_64_27,
    HDMI_3840X2160P60_64_27,	// 107
	HDMI_VIDEO_CODE_BUTT
}HDMI_VIC_E;

typedef enum 
{
    HDMI_VIDEO_TIMING_UNKNOWN,
    HDMI_VIDEO_TIMING_640X480P_60000,        /* 1: 640x480p  @ 60Hz     No Repetition */  
    HDMI_VIDEO_TIMING_720X480P_60000,        /* 2: 720x480p  @ 60Hz     No Repetition */
    HDMI_VIDEO_TIMING_720X480P_120000,
	HDMI_VIDEO_TIMING_720X480P_240000,      // 4
	
    HDMI_VIDEO_TIMING_720X576P_50000,        /* 17:720x576p  @ 50Hz     No Repetition */    // 5
	HDMI_VIDEO_TIMING_720X576P_100000,
	HDMI_VIDEO_TIMING_720X576P_200000,      // 7

    HDMI_VIDEO_TIMING_1280X720P_24000,
    HDMI_VIDEO_TIMING_1280X720P_25000,
    HDMI_VIDEO_TIMING_1280X720P_30000,      // 10
  
    HDMI_VIDEO_TIMING_1280X720P_50000,       /* 19:1280x720p @ 50Hz     No Repetition */
    HDMI_VIDEO_TIMING_1280X720P_60000,       /* 4: 1280x720p @ 60Hz     No Repetition */
    HDMI_VIDEO_TIMING_1280X720P_100000,
	HDMI_VIDEO_TIMING_1280X720P_120000,     // 14
    
	HDMI_VIDEO_TIMING_1440X240P_60000,      // 15
	HDMI_VIDEO_TIMING_1440X288P_50000,
	HDMI_VIDEO_TIMING_1440X480I_60000,
	HDMI_VIDEO_TIMING_1440X480P_60000,
	HDMI_VIDEO_TIMING_1440X480I_120000,
	HDMI_VIDEO_TIMING_1440X480I_240000,
	
	HDMI_VIDEO_TIMING_1440X576I_50000,      // 21
	HDMI_VIDEO_TIMING_1440X576P_50000,
	HDMI_VIDEO_TIMING_1440X576I_60000,
	HDMI_VIDEO_TIMING_1440X576I_100000,
	HDMI_VIDEO_TIMING_1440X576I_200000,
	
	HDMI_VIDEO_TIMING_2880X288P_50000,      // 26
	HDMI_VIDEO_TIMING_2880X480I_60000,
	HDMI_VIDEO_TIMING_2880X480P_60000,
	HDMI_VIDEO_TIMING_2880X240I_60000,
	HDMI_VIDEO_TIMING_2880X576I_50000,
	HDMI_VIDEO_TIMING_2880X576P_50000,
	
	HDMI_VIDEO_TIMING_1680X720P_24000,      // 32
	HDMI_VIDEO_TIMING_1680X720P_25000,
	HDMI_VIDEO_TIMING_1680X720P_30000,
	HDMI_VIDEO_TIMING_1680X720P_50000,
	HDMI_VIDEO_TIMING_1680X720P_60000,
	HDMI_VIDEO_TIMING_1680X720P_100000,
	HDMI_VIDEO_TIMING_1680X720P_120000,
	
	HDMI_VIDEO_TIMING_2560X1080P_24000,     // 39
	HDMI_VIDEO_TIMING_2560X1080P_25000,
	HDMI_VIDEO_TIMING_2560X1080P_30000,
	HDMI_VIDEO_TIMING_2560X1080P_50000,
	HDMI_VIDEO_TIMING_2560X1080P_60000,
	HDMI_VIDEO_TIMING_2560X1080P_100000,
	HDMI_VIDEO_TIMING_2560X1080P_120000,    // 45
	
	HDMI_VIDEO_TIMING_1920X1080I_60000,      /* 5: 1920x1080i@ 59.94Hz  No Repetition */
    HDMI_VIDEO_TIMING_1920X1080P_60000,      /* 16:1920x1080p@ 60Hz     No Repetition */
    HDMI_VIDEO_TIMING_1920X1080I_50000,      /* 20:1920x1080i@ 50Hz     No Repetition */
    HDMI_VIDEO_TIMING_1920X1080P_50000,      /* 31:1920x1080p @ 50Hz    No Repetition */
    HDMI_VIDEO_TIMING_1920X1080P_24000,      /* 32:1920x1080p @ 24Hz    No Repetition */
    HDMI_VIDEO_TIMING_1920X1080P_25000,      /* 33:1920x1080p @ 25Hz    No Repetition */
    HDMI_VIDEO_TIMING_1920X1080P_30000,      /* 34:1920x1080p @ 30Hz    No Repetition */
    HDMI_VIDEO_TIMING_1920X1080I_100000,
	HDMI_VIDEO_TIMING_1920X1080I_120000,
	HDMI_VIDEO_TIMING_1920X1080P_120000,
	HDMI_VIDEO_TIMING_1920X1080P_100000,    // 56
	
	HDMI_VIDEO_TIMING_3840X2160P_24000,      /* 93:3840x2160p @ 24Hz    No Repetition */
    HDMI_VIDEO_TIMING_3840X2160P_25000,      /* 94:3840x2160p @ 25Hz    No Repetition */
    HDMI_VIDEO_TIMING_3840X2160P_30000,      /* 95:3840x2160p @ 30Hz    No Repetition */
    HDMI_VIDEO_TIMING_3840X2160P_50000,      /* 96:3840x2160p @ 50Hz    No Repetition */
    HDMI_VIDEO_TIMING_3840X2160P_60000,      /* 97:3840x2160p @ 60Hz    No Repetition */
    HDMI_VIDEO_TIMING_4096X2160P_24000,      /* 98:4096x2160p @ 24Hz    No Repetition */
    HDMI_VIDEO_TIMING_4096X2160P_25000,      /* 98:4096x2160p @ 25Hz    No Repetition */
    HDMI_VIDEO_TIMING_4096X2160P_30000,      /* 98:4096x2160p @ 30Hz    No Repetition */
    HDMI_VIDEO_TIMING_4096X2160P_50000,      /* 98:4096x2160p @ 50Hz    No Repetition */
    HDMI_VIDEO_TIMING_4096X2160P_60000,      /* 98:4096x2160p @ 60Hz    No Repetition */

    HDMI_VIDEO_TIMING_VESA_DEFINE,

    HDMI_VIDEO_TIMING_VESA_800X600_60,
    HDMI_VIDEO_TIMING_VESA_848X480_60,
    HDMI_VIDEO_TIMING_VESA_1024X768_60,
	HDMI_VIDEO_TIMING_VESA_1280X720_60,
    HDMI_VIDEO_TIMING_VESA_1280X768_60,
    HDMI_VIDEO_TIMING_VESA_1280X768_60_RB,
    HDMI_VIDEO_TIMING_VESA_1280X800_60,
    HDMI_VIDEO_TIMING_VESA_1280X800_60_RB,
    HDMI_VIDEO_TIMING_VESA_1280X960_60,
    HDMI_VIDEO_TIMING_VESA_1280X1024_60,
    HDMI_VIDEO_TIMING_VESA_1360X768_60,
    HDMI_VIDEO_TIMING_VESA_1366X768_60,
    HDMI_VIDEO_TIMING_VESA_1400X1050_60,
    HDMI_VIDEO_TIMING_VESA_1440X900_60,
    HDMI_VIDEO_TIMING_VESA_1440X900_60_RB,
    HDMI_VIDEO_TIMING_VESA_1440X1050_60,
    HDMI_VIDEO_TIMING_VESA_1440X1050_60_RB,
    HDMI_VIDEO_TIMING_VESA_1600X900_60_RB,
    HDMI_VIDEO_TIMING_VESA_1600X1200_60,
    HDMI_VIDEO_TIMING_VESA_1680X1050_60,
    HDMI_VIDEO_TIMING_VESA_1680X1050_60_RB,
    HDMI_VIDEO_TIMING_VESA_1792X1344_60,
    HDMI_VIDEO_TIMING_VESA_1856X1392_60,
    HDMI_VIDEO_TIMING_VESA_1920X1080_60,
    HDMI_VIDEO_TIMING_VESA_1920X1200_60,
    HDMI_VIDEO_TIMING_VESA_1920X1200_60_RB,
    HDMI_VIDEO_TIMING_VESA_1920X1440_60,
    HDMI_VIDEO_TIMING_VESA_2048x1152_60,
    HDMI_VIDEO_TIMING_VESA_2560x1440_60_RB,
    HDMI_VIDEO_TIMING_VESA_2560x1600_60,
    HDMI_VIDEO_TIMING_VESA_2560x1600_60_RB,    

    HDMI_VIDEO_TIMING_USER_DEFINE,

    HDMI_VIDEO_TIMING_USER_1920x2160_30,
    HDMI_VIDEO_TIMING_USER_2560X1440_30,
    HDMI_VIDEO_TIMING_USER_1280X720_60,
    HDMI_VIDEO_TIMING_USER_1366X768_60,
    HDMI_VIDEO_TIMING_USER_1600X900_60_RB,
    HDMI_VIDEO_TIMING_USER_1920X1080_60,
    HDMI_VIDEO_TIMING_USER_2048X1152_60,

	HDMI_VIDEO_TIMING_BUTT
}HDMI_VIDEO_TIMING_E;

struct hdmi_avi_infoframe {
	enum hdmi_infoframe_type type;
	HI_U8 version;
	HI_U8 length;
	enum hdmi_colorspace colorspace;
	HI_BOOL active_info_valid;
	HI_BOOL horizontal_bar_valid;
	HI_BOOL vertical_bar_valid;
	enum hdmi_scan_mode scan_mode;
	enum hdmi_colorimetry colorimetry;
	enum hdmi_picture_aspect picture_aspect;
	enum hdmi_active_aspect active_aspect;
	HI_BOOL itc;
	enum hdmi_extended_colorimetry extended_colorimetry;
	enum hdmi_quantization_range quantization_range;
	enum hdmi_nups nups;
	//enum hdmi_video_code video_code;
    HDMI_VIDEO_TIMING_E video_timing; 
    HI_U32              disp_fmt;    
    enum hdmi_ycc_quantization_range ycc_quantization_range;
	enum hdmi_content_type content_type;
	HI_U8 pixel_repeat;
	HI_U16 top_bar;
	HI_U16 bottom_bar;
	HI_U16 left_bar;
	HI_U16 right_bar;
};

enum hdmi_eotf_type
{
    HDMI_EOTF_SDR_LUMIN,         /* Traditional gamma - SDR Luminance Range  */
    HDMI_EOTF_HDR_LUMIN,         /* Traditional gamma - HDR Luminance Range  */
    HDMI_EOTF_SMPTE_ST_2048,     /* SMPTE ST 2084  */
    HDMI_EOTF_FUTURE_USE,        /* Future EOTF  */
    HDMI_EOTF_BUTT,
};

enum hdmi_static_metadata_type
{
    HDMI_HDR_METADATA_ID_0,      /* Static Metadata Type 1  */
    HDMI_HDR_METADATA_ID_BUTT,
};

struct hdmi_meta_descriptor_1st
{
    HI_U16  u16Primaries0_X;        /* display_primaries_x[0],range[0,50000],in units of 0.00002 */
    HI_U16  u16Primaries0_Y;        /* display_primaries_y[0],range[0,50000],in units of 0.00002 */
    HI_U16  u16Primaries1_X;        /* display_primaries_x[1],range[0,50000],in units of 0.00002 */
    HI_U16  u16Primaries1_Y;        /* display_primaries_y[1],range[0,50000],in units of 0.00002 */
    HI_U16  u16Primaries2_X;        /* display_primaries_x[2],range[0,50000],in units of 0.00002 */
    HI_U16  u16Primaries2_Y;        /* display_primaries_y[2],range[0,50000],in units of 0.00002 */
    HI_U16  u16WhitePoint_X;        /* white_point_x,range[0,50000],in units of 0.00002 */
    HI_U16  u16WhitePoint_Y;        /* white_point_y,range[0,50000],in units of 0.00002 */
    HI_U16  u16MaxLuminance;        /* max_display_mastering_luminance,in units of 1 cd/m^2 */
    HI_U16  u16MinLuminance;        /* min_display_mastering_luminance,in units of 0.0001 cd/m^2 */
    HI_U16  u16MaxLightLevel;       /* Maximum Content Light Level,in units of 1 cd/m2 */
    HI_U16  u16AverageLightLevel;   /* Maximum Frame-average Light Level,in units of 1 cd/m^2 */
};

union hdmi_meta_Descriptor
{
    struct hdmi_meta_descriptor_1st    stType1;              /* Static Metadata Descriptor Type1 */
};

struct hdmi_drm_infoframe
{
    enum hdmi_infoframe_type type;
    HI_U8 version;
	HI_U8 length;
    enum hdmi_eotf_type eotf_type;
    enum hdmi_static_metadata_type metadata_type; 
    union hdmi_meta_Descriptor meta_descriptor;
};

enum hdmi_audio_sample_size {
	HDMI_AUDIO_SAMPLE_SIZE_STREAM,
	HDMI_AUDIO_SAMPLE_SIZE_16,
	HDMI_AUDIO_SAMPLE_SIZE_20,
	HDMI_AUDIO_SAMPLE_SIZE_24,
};

enum hdmi_audio_sample_frequency {
	HDMI_AUDIO_SAMPLE_FREQUENCY_STREAM,
	HDMI_AUDIO_SAMPLE_FREQUENCY_32000,
	HDMI_AUDIO_SAMPLE_FREQUENCY_44100,
	HDMI_AUDIO_SAMPLE_FREQUENCY_48000,
	HDMI_AUDIO_SAMPLE_FREQUENCY_88200,
	HDMI_AUDIO_SAMPLE_FREQUENCY_96000,
	HDMI_AUDIO_SAMPLE_FREQUENCY_176400,
	HDMI_AUDIO_SAMPLE_FREQUENCY_192000,
};

enum hdmi_audio_coding_type_ext {
	HDMI_AUDIO_CODING_TYPE_EXT_STREAM,
	HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC,
	HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC_V2,
	HDMI_AUDIO_CODING_TYPE_EXT_MPEG_SURROUND,
};

enum hdmi_audio_lfe_playback_level{
	HDMI_AUDIO_LFE_PLAYBACK_NO,
	HDMI_AUDIO_LFE_PLAYBACK_0_DB,
	HDMI_AUDIO_LFE_PLAYBACK_10_DB,
	HDMI_AUDIO_LFE_PLAYBACK_RESERVED,
};

enum hdmi_audio_code {
	HDMI_AUDIO_CODING_TYPE_STREAM,
	HDMI_AUDIO_CODING_TYPE_PCM,
	HDMI_AUDIO_CODING_TYPE_AC3,
	HDMI_AUDIO_CODING_TYPE_MPEG1,
	HDMI_AUDIO_CODING_TYPE_MP3,
	HDMI_AUDIO_CODING_TYPE_MPEG2,
	HDMI_AUDIO_CODING_TYPE_AAC_LC,
	HDMI_AUDIO_CODING_TYPE_DTS,
	HDMI_AUDIO_CODING_TYPE_ATRAC,
	HDMI_AUDIO_CODING_TYPE_DSD,
	HDMI_AUDIO_CODING_TYPE_EAC3,
	HDMI_AUDIO_CODING_TYPE_DTS_HD,
	HDMI_AUDIO_CODING_TYPE_MLP,
	HDMI_AUDIO_CODING_TYPE_DST,
	HDMI_AUDIO_CODING_TYPE_WMA_PRO,
    HDMI_AUDIO_CODING_TYPE_RESERVED,
	HDMI_AUDIO_CODING_TYPE_BUTT,
};

struct hdmi_audio_infoframe {
	enum hdmi_infoframe_type type;
	HI_U8 version;
	HI_U8 length;
	HI_U8 channels;
	enum hdmi_audio_code coding_type;
	enum hdmi_audio_sample_size sample_size;
	enum hdmi_audio_sample_frequency sample_frequency;
	enum hdmi_audio_coding_type_ext coding_type_ext;
	HI_U8 channel_allocation;
	enum hdmi_audio_lfe_playback_level lfe_playback_level;
	HI_U8 level_shift_value;
	HI_BOOL downmix_inhibit;

};

enum hdmi_ieee_oui{
   HDMI14_IEEE_OUI =0x00000c03,//HDMI1.4
   HDMI20_IEEE_OUI =0x00c45dd8,//HDMI2.0
};

enum hdmi_video_format{
    HDMI_VIDEO_FORMAT_NONE = 0x0,
    HDMI_VIDEO_FORMAT_4K = 0x1,
    HDMI_VIDEO_FORMAT_3D = 0x2,    
};

enum hdmi_vic{
    HDMI_VIC_4K_3840X2160_30 = 0x1,
    HDMI_VIC_4K_3840X2160_25 = 0x2,   
    HDMI_VIC_4K_3840X2160_24 = 0x3,  
    HDMI_VIC_4K_4096X2160_24 = 0x4,  
};

enum hdmi_3d_structure{
    HDMI_3D_FRAME_PACKING = 0x0,
    HDMI_3D_FIELD_ALTERNATIVE = 0x1,
    HDMI_3D_LINE_ALTERNATIVE = 0x2,
    HDMI_3D_SIDE_BY_SIDE_FULL = 0x3,
    HDMI_3D_L_DEPTH  = 0x4,
    HDMI_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH = 0x05,
    HDMI_3D_TOP_AND_BOTTOM = 0x6,
    HDMI_3D_SIDE_BY_SIDE_HALF = 0x08,
    HDMI_3D_BUTT,
};

enum hdmi_3d_ext_data{
    HDMI_3D_PICTURE_ODD_LEFT_ODD_RIGHT = 0x4,
    HDMI_3D_PICTURE_ODD_LEFT_EVEN_RIGHT = 0x5,
    HDMI_3D_PICTURE_EVEN_LEFT_ODD_RIGHT = 0x6,
    HDMI_3D_PICTURE_EVEN_LEFT_EVEN_RIGHT = 0x7,
};

typedef struct {
    enum hdmi_ieee_oui ieee;
	enum hdmi_video_format format;
    enum hdmi_vic vic;
    enum hdmi_3d_structure _3d_structure;
    HI_U8 _3d_meta_present;
    enum hdmi_3d_ext_data _3d_ext_data;
    HI_U8 _3d_metadata_type;
    HI_U8 _3d_metadata_length;
    HI_U8 _3d_metadata[20];
}hdmi_14_vsif_content;//for hdmi1.4

typedef struct {
    enum hdmi_ieee_oui ieee;
    HI_U8 version;
    HI_U8 _3d_valid;
    HI_U8 _3d_f_structure;
    HI_U8 _3d_additional_info_present;
    HI_U8 _3d_disparity_data_present;
    HI_U8 _3d_meta_present;
    HI_U8 _3d_f_ext_data;
    HI_U8 _3d_dual_view;
    HI_U8 _3d_view_dependency;
    HI_U8 _3d_preferred_2d_view;
    HI_U8 _3d_disparity_data_version;
    HI_U8 _3d_disparity_data_length;
    HI_U8 _3d_disparity_data[10];
    HI_U8 _3d_ext_data;
    HI_U8 _3d_metadata_type;
    HI_U8 _3d_metadata_length;
    HI_U8 _3d_metadata[10];
}hdmi_forum_vsif_content;//for hdmi2.0

struct hdmi_vendor_specific_infoframe {
	enum hdmi_infoframe_type type;
	HI_U8 version;
	HI_U8 length;

    union {
        hdmi_14_vsif_content  h14_vsif_content;
        hdmi_forum_vsif_content  hf_vsif_content;
    }vsif_content;

};

typedef union
{
    struct
    {
        HI_U8    bz_Affected_Gamut_Seq_Num          : 4   ; // [0..3] 
        HI_U8    bz_GBD_profile                     : 3   ; // [4..6] 
        HI_U8    bz_Next_Field                      : 1   ; // [7] 
    } bits;

    HI_U8    u8;

} HDMI_GUMAT_HB1_U;

typedef union
{
    struct
    {
        HI_U8    bz_Current_Gamut_Seq_Num           : 4   ; // [0..3] 
        HI_U8    bz_Packet_Seq                      : 2   ; // [4..5] 
        HI_U8    bz_Reseved                         : 1   ; // [6]
        HI_U8    bz_No_Crnt_GBD                     : 1   ; // [7] 
    } bits;

    HI_U8    u8;

} HDMI_GUMAT_HB2_U;

typedef union
{
    struct
    {
        HI_U8    bz_GBD_Color_Space            : 3   ; // [0..2] 
        HI_U8    bz_GBD_Color_Precision        : 2   ; // [3..4] 
        HI_U8    bz_Reseved0                    : 1   ; // [5]
        HI_U8    bz_Reseved1                    : 1   ; // [6]
        HI_U8    bz_Format_Flag                : 1   ; // [7] 
    } bits;

    HI_U8    u8;

} HDMI_GUMAT_RANGE_GDB0_U;



typedef struct {
    HI_U8                       u8HB0;
    HDMI_GUMAT_HB1_U            u8HB1;
    HDMI_GUMAT_HB2_U            u8HB2;
    HDMI_GUMAT_RANGE_GDB0_U     u8GDB0;
    HI_U8                       u8Min_Red_Data_H;
    HI_U8                       u8Mid_Red_Data_M;   //u8Max_Red_Data_L||Max_Red_Data_H
    HI_U8                       u8Max_Red_Data_L;
    HI_U8                       u8Min_Green_Data_H;
    HI_U8                       u8Mid_Green_Data_M; //u8Max_Green_Data_L||Max_Green_Data_H
    HI_U8                       u8Max_Green_Data_L;
    HI_U8                       u8Min_Blue_Data_H;
    HI_U8                       u8Mid_Blue_Data_M;  //u8Max_Blue_Data_L||Max_Blue_Data_H
    HI_U8                       u8Max_Blue_Data_L;
    
}HDMI_GDB_PACK_S;

/*
HI_S32 hdmi_avi_infoframe_init(struct hdmi_avi_infoframe *frame);
HI_S32 hdmi_avi_infoframe_pack(struct hdmi_avi_infoframe *frame, HI_VOID *buffer,
				HI_U32 size);


HI_S32 hdmi_audio_infoframe_init(struct hdmi_audio_infoframe *frame);
HI_S32 hdmi_audio_infoframe_pack(struct hdmi_audio_infoframe *frame,
				  HI_VOID *buffer, HI_U32 size);
HI_S32 hdmi_vendor_specific_infoframe_init(struct hdmi_vendor_specific_infoframe *frame);
HI_S32 hdmi_vendor_specific_infoframe_pack(struct hdmi_vendor_specific_infoframe *frame,
				   HI_VOID *buffer, HI_U32 size);
*/

typedef enum hdmi_3d_structure HDMI_3D_MODE_E;
typedef enum hdmi_colorspace HDMI_COLORSPACE_E;
typedef enum hdmi_quantization_range HDMI_QUANTIZATION_RANGE;
typedef enum hdmi_ycc_quantization_range HDMI_YCC_QUANTIZATION_RANGE;
typedef enum hdmi_colorimetry HDMI_COLORIMETRY_E;
typedef enum hdmi_extended_colorimetry HDMI_EXTENDED_COLORIMETRY; 
typedef enum hdmi_picture_aspect HDMI_PICTURE_ASPECT;
typedef enum hdmi_active_aspect HDMI_ACTIVE_ASPECT;
typedef enum hdmi_audio_code HDMI_AUDIO_FORMAT_CODE_E;
typedef enum hdmi_video_code HDMI_VIDEO_CODE_E;
typedef enum hdmi_vic        HDMI_VSIF_VIC_E;
typedef enum hdmi_infoframe_type HDMI_INFOFRAME_ID_E;

typedef struct hdmi_avi_infoframe HDMI_AVI_INFOFRAME_S;
typedef struct hdmi_audio_infoframe HDMI_AUDIO_INFOFRAME_S;
typedef struct hdmi_vendor_specific_infoframe HDMI_VENDOR_INFOFRAME_S;

typedef enum hdmi_eotf_type     HDMI_EOTF_TYPE_E;
typedef enum hdmi_static_metadata_type HDMI_STATIC_META_TYPE_ID_E;
typedef union hdmi_meta_Descriptor  HDMI_META_DESCRIPTOR_UN;
typedef struct hdmi_meta_descriptor_1st HDMI_META_DES_1ST_S;
typedef struct hdmi_drm_infoframe HDMI_DRM_INFOFRAME_S;

typedef struct
{
    HDMI_AVI_INFOFRAME_S    stAVIInfoFrame;
    HDMI_AUDIO_INFOFRAME_S  stAudioInfoFrame;
    HDMI_VENDOR_INFOFRAME_S stVendorInfoFrame;
    HDMI_GDB_PACK_S         stGDBPack;
    HDMI_DRM_INFOFRAME_S    stDynRanInfoFrame;    
}HDMI_INFOFRAME_S;

typedef union
{
    HDMI_AVI_INFOFRAME_S    stAVIInfoFrame;
    HDMI_AUDIO_INFOFRAME_S  stAudioInfoFrame;
    HDMI_VENDOR_INFOFRAME_S stVendorInfoFrame;
    HDMI_GDB_PACK_S         stGDBPack;
    HDMI_DRM_INFOFRAME_S    stDynRanInfoFrame;
}HDMI_INFOFRAME_U;

HI_S32 DRV_HDMI_AVIInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable);

HI_S32 DRV_HDMI_VendorInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable);

HI_S32 DRV_HDMI_AudioInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable);

HI_S32 DRV_HDMI_GBDPackSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable);

HI_S32 DRV_HDMI_InfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HDMI_INFOFRAME_ID_E enInfoFrameId, HDMI_INFOFRAME_U* uInfoframe);

HI_S32 DRV_HDMI_InfoFrameEnableSet(HDMI_INFOFRAME_S* pstInfoFrame, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable);

HDMI_VIDEO_TIMING_E DRV_HDMI_VideoTimingLookup(HDMI_VIDEO_CODE_E enVic, HDMI_PICTURE_ASPECT enAspect);
HDMI_VIDEO_TIMING_E DRV_HDMI_VsifVideoTimingLookup(HDMI_VSIF_VIC_E enVic, HDMI_PICTURE_ASPECT enAspect);

HI_S32 DRV_HDMI_DRMInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable);

#endif /* __INFOFRAME_H_ */
