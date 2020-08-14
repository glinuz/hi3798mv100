/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_proc.c
  Version       : v2.0
  Author        : Hisilicon multimedia software group
  Created       : 2015/02/01 
  Author        : t273561
  Description   :
  History       :
  UpDate        : 2015/07/04
  Author        : l232354
  Modification  :
*******************************************************************************/

#include "drv_hdmi_proc.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_edid.h"
#include "hdmi_platform.h"

/******************************************************************************/
/* Macro define                                                               */
/******************************************************************************/
#define PROC_FEILD_RATE_DEVIATION	50
#define PROC_PIXL_FREQ_DEVIATION	100
#define HDMI_VIC_PC_BASE 			((HDMI_VIC_VIRTUAL_BASE)+0x100)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)		(sizeof(a)/sizeof(a[0]))
#endif
#define STR_YES				"YES"
#define STR_NO				"NO"
#define STR_NA				"N/A"
#define STR_ERROR			"ERROR"
#define STR_RUN				"RUN"
#define STR_STOP			"STOP"
#define STR_ON				"ON"
#define STR_OFF			    "OFF"
#define CHECK_MAX_VALUE(out, in, max)    out = (HI_U32)(((in) >= (max))? (max) : (in))

#define CHECK_RET_SUCCESS(ret) \
do{ \
	if(HI_SUCCESS != ret){ \
		HDMI_ERR("Error In %s,Line %d,return not success!\n",__FUNCTION__,__LINE__); \
		return;\
	} \
}while(0) 

#define CHECK_POINTER(p)\
do{	\
	if(HI_NULL == p){ \
		HDMI_ERR("%s %d null pointer!\n", __FUNCTION__, __LINE__);\
		return; \
	} \
}while(0)

/******************************************************************************/
/* Data structures                                                            */
/******************************************************************************/
typedef union {
    struct
    {
        HI_U8    bzScanInfo            : 2; // [0..1] 
        HI_U8    bzBarDataPresent      : 2; // [2..3] 
        HI_U8    bzActFmtInfoPresent   : 1; // [5] 
        HI_U8    bzColorSpace          : 3; // [6..7] 
    } bits;
    HI_U8    u8;
} HDMI_AVIIF_BYTE1_U;

typedef union {
    struct
    {
        HI_U8    bzActAspRatio         : 4; // [0..3] 
        HI_U8    bzPictureAspectRatio  : 2; // [4..5] 
        HI_U8    bzColorimetry         : 2; // [6..7] 
    } bits;
    HI_U8    u8;
} HDMI_AVIIF_BYTE2_U;

typedef union {
    struct
    {
        HI_U8    bzPictureScaling     	: 2; // [0..1] 
        HI_U8    bzRgbQuantization   	: 2; // [2..3] 
        HI_U8    bzExtColorimetry 		: 3; // [4..6] 
        HI_U8    bzItContentValid	 	: 1; // [7] 
    } bits;
    HI_U8    u8;
} HDMI_AVIIF_BYTE3_U;

typedef union {
    struct
    {
        HI_U8    bzPixelRepetition     	: 4; // [0..3] 
        HI_U8    bzITContentType   	   	: 2; // [4..5] 
        HI_U8    bzYCCQuantizationRange	: 2; // [6..7] 
    } bits;
    HI_U8    u8;
} HDMI_AVIIF_BYTE5_U;

typedef union {
    struct
    {
        HI_U8    bzAudioChanelCnt     	: 3; // [0..2] 
        HI_U8    bzF13   	   			: 1; // [3] 
        HI_U8    bzAduioCodingType		: 4; // [4..7] 
    } bits;

    HI_U8    u8;

} HDMI_AUDIO_IF_BYTE1_U;

typedef union {
    struct
    {
        HI_U8    bzAudioSampingSize     : 2; // [0..1] 
        HI_U8    bzAudioSampingFreq  	: 3; // [2..4] 
        HI_U8    bzF25toF27				: 3; // [5..7] 
    } bits;
    HI_U8    u8;
} HDMI_AUDIO_IF_BYTE2_U;

/* EIA-CEA-861-F P66 */
typedef union {
    struct
    {
        HI_U8    bzLFE_PlayBack     	: 2; // [0..1] 
        HI_U8    bzF52 	   				: 1; // [2] 
        HI_U8    bzLevelShiftValue 		: 4; // [3..6] 
        HI_U8    bzDownMixInhibit		: 1; // [7] 
    } bits;
    HI_U8    u8;
} HDMI_AUDIO_IF_BYTE5_U;

typedef struct {
	HI_U32				u32Vic; 
	HI_U32				u32PixlFreq;				/* KHz */
	HI_U32				u32HorActive;	
	HI_U32				u32VerActive;
	HI_U32				u32FieldRate;				/* 0.01Hz */
	HI_CHAR				*pcFormat;
}HDMI_FMT_NAME_PARAM_S;


/******************************************************************************/
/* Global Data                                                                */
/******************************************************************************/
/* EIA-CEA-861-F P66 */
const static HI_CHAR *s_LFEPlayBack2Str[] = {
	"UNKNOW",			//0b00
	"0 dB",				//0b01
	"+10 dB",			//0b10
	"Reserved",			//0b11
};

/* EIA-CEA-861-F P159 */
const static HI_CHAR *s_eActAspectRatio2Str[] = {
	"Reserved",			//0000 reserved,
	"Reserved",			//0001 reserved
	"16:9_TOP",			//HDMI_ACTIVE_ASPECT_16_9_TOP = 2,
	"14:9_TOP",			//HDMI_ACTIVE_ASPECT_14_9_TOP = 3,
	"16:9_CENTER",	    //HDMI_ACTIVE_ASPECT_16_9_CENTER = 4,
	"Reserved",			//0101 - 0111 reserved,
	"Reserved",			//0101 - 0111 reserved,
	"Reserved",			//0101 - 0111 reserved,
	"PICTURE",	        //HDMI_ACTIVE_ASPECT_PICTURE = 8,		 
	"4:3",		        //HDMI_ACTIVE_ASPECT_4_3 = 9,		 
	"16:9",		        //HDMI_ACTIVE_ASPECT_16_9 = 10,		 
	"14:9",		        //HDMI_ACTIVE_ASPECT_14_9 = 11,
	"Reserved",			//1100 reserved,
	"4:3_SP_14_9",		//HDMI_ACTIVE_ASPECT_4_3_SP_14_9 = 13,
	"16:9_SP_14_9",		//HDMI_ACTIVE_ASPECT_16_9_SP_14_9 = 14,
	"16:9_SP_4_3",		//HDMI_ACTIVE_ASPECT_16_9_SP_4_3 = 15,
    "UNKNOW"            //HDMI_ACTIVE_ASPECT_BUTT
};

/* EIA-CEA-861-F P43 */
const static HI_CHAR *s_ScanInfo2Str[] = {
	"NONE",			    //0b00
	"OVER_SCAN",		//0b01
	"UNDERS_SCAN",		//0b10
	"Reserved",			//0b11
};

/* EIA-CEA-861-F P44 */
const static HI_CHAR *s_eColorimetry2Str[] = {
	"No Data",			//0b00
	"SMPTE 170M",		//0b01
	"ITU-R BT.709",		//0b10
	"Extended",			//0b11	
};

/* EIA-CEA-861-F P44 */
const static HI_CHAR *s_ePicAspectRatio2Str[] = {
	"NONE",			//HDMI_PICTURE_ASPECT_NONE,
	"4:3",			//HDMI_PICTURE_ASPECT_4_3,
	"16:9",			//HDMI_PICTURE_ASPECT_16_9,
	"64:27",        //HDMI_PICTURE_ASPECT_64_27
	"256:135",      //HDMI_PICTURE_ASPECT_256_135
	"FUTURE",		//HDMI_PICTURE_ASPECT_FUTURE,
	"UNKNOW",		//HDMI_PICTURE_ASPECT_BUTT,
 };

/* EIA-CEA-861-F P43*/
const static HI_CHAR *s_BarDataPresent2Str[] = {
	"NONE",			//0b00 Bar Data not present 
	"HnVp",			//0b01 Vert. BarInfo present 
	"HpVn",			//0b10 Horiz. BarInfo present 
	"HpVp",			//0b11 Vert. and Horiz. BarInfo present 
};


/* EIA-CEA-861-F P49*/
const static HI_CHAR *s_ITContentType2Str[] = {
	"GRAPHICS",			//0b00 
	"PHOTO",			//0b01 
	"CINEMA",			//0b10 
	"GAME",				//0b11 
};

/* EIA-CEA-861-F P50*/
const static HI_CHAR *s_eYCCQuantization2Str[] = {
	"LIMITED",			//0b00   HDMI_YCC_QUANTIZATION_RANGE_LIMITED
	"FULL",				//0b01   HDMI_YCC_QUANTIZATION_RANGE_FULL
	"UNKNOW",			//0b10   HDMI_YCC_QUANTIZATION_RANGE_BUTT
    "UNKNOW",           //0b11   HDMI_YCC_QUANTIZATION_RANGE_BUTT
};

const static HI_CHAR *s_PixelRepetition2Str[] = {
	"No Repetition",	        //0b0000
	"2 Times",			//0b0001
	"3 Times",			//0b0010
	"4 Times",			//0b0011
	"5 Times",			//0b0100
	"6 Times",			//0b0101
	"7 Times",			//0b0110
	"8 Times",			//0b0111
	"9 Times",			//0b1000
	"10 Times",			//0b1001
	"Reserved",			//0x0a
	"Reserved",			//0x0b
	"Reserved",			//0x0c
	"Reserved",			//0x0d
	"Reserved",			//0x0e
	"Reserved",			//0x0f
};

/* EIA-CEA-861-F P62 */
const static HI_CHAR *s_AudioCodeType2Str[] = {
	"STREAM",		    //0b0000
	"L-PCM",			//0b0001
	"AC3",				//0b0010
	"MPEG1",			//0b0011
	"MP3",				//0b0100
	"MPEG2",			//0b0101
	"AAC_LC",			//0b0110
	"DTS",				//0b0111
	"ATRAC",			//0b1000
	"OneBitAudio",		//0b1001
	"EAC3",		        //0b1010
	"DTS-HD",			//0b1011
	"MAT",				//0b1100
	"DST",				//0b1101
	"WMA_PRO",			//0b1110
	"Reserved",		    //0b1111
	"UNKNOW",		    //0b
};

/* EIA-CEA-861-F P62 */
const static HI_CHAR *s_AudioChannel2Str[] = 
{
	"STR_HEADER",	//0b000
	"2_CH",		//0b001
	"3_CH",		//0b010
	"4_CH",		//0b011
	"5_CH",		//0b100
	"6_CH",		//0b101
	"7_CH",		//0b110
	"8_CH",		//0b111
};

/* EIA-CEA-861-F P62 */
const static HI_CHAR *s_AudioSampingFreq2Str[] = {
	"STR_HEADER",	//0b000
	"32 KHz",			//0b001
	"44.1 kHz",			//0b010
	"48 KHz",			//0b011
	"88.2 KHz",			//0b100
	"96 KHz",			//0b101
	"176.4 KHz",		//0b110
	"192 KHz",			//0b111
};

/* EIA-CEA-861-F P62 */
const static HI_CHAR *s_AudioSampingSize2Str[] = {
	"STR_HEADER",	//0b00
	"16 bit",			//0b01
	"20 bit",			//0b10
	"24 bit",			//0b11	
};

const static HI_CHAR *s_eRunState2Str[] = {
//	"NONE",				//HDMI_RUN_STATE_NONE 	= 0x00,
    "INIT",				//HDMI_RUN_STATE_INIT 	= 0x01,
    "OPEN",				//HDMI_RUN_STATE_OPEN 	= 0x02,
    "START",			//HDMI_RUN_STATE_START 	= 0x04,
    "STOP",				//HDMI_RUN_STATE_STOP 	= 0x08,
    "CLOSE",			//HDMI_RUN_STATE_CLOSE 	= 0x10, 
    "DEINIT",			//HDMI_RUN_STATE_DEINIT	= 0x20,
};

const static HI_CHAR *s_eTmdsMode2Str[] = {
    "NONE",				//HDMI_TMDS_MODE_NONE = 0,
    "DVI",				//HDMI_TMDS_MODE_DVI = 1,
    "HDMI1.4",			//HDMI_TMDS_MODE_HDMI_1_4 = 2,
    "HDMI2.0",			//HDMI_TMDS_MODE_HDMI_2_0 =3,
    "AUTO",				//HDMI_TMDS_MODE_AUTO = 4,
    "UNKNOW",           //HDMI_TMDS_MODE_BUTT
};

#ifdef HDMI_HDR_SUPPORT
const static HI_CHAR *s_eHdrMode2Str[] = {
    "DISABLE",          // HDMI_HDR_MODE_DISABLE = 0,
    "DOLBY_NORMAL",     // HDMI_HDR_MODE_DOLBY_NORMAL = 1,
    "DOLBY_TUNNELING",  // HDMI_HDR_MODE_DOLBY_TUNNELING = 2,
    "HDR",              // HDMI_HDR_MODE_HDR = 3,
    "HDR_AUTHEN",       // HDMI_HDR_MODE_CEA_861_3_AUTHEN = 4
    "UNKNOW",           // HDMI_HDR_MODE_BUTT,
};

const static HI_CHAR *s_eHdrEotfType2Str[] = {
    "SDR_LUMIN",        // HDMI_HDR_EOTF_SDR_LUMIN = 0,
    "HDR_LUMIN",        // HDMI_HDR_EOTF_HDR_LUMIN = 1,
    "SMPTE_ST_2084",    // HDMI_HDR_EOTF_SMPTE_ST_2048 = 2,
    "FUTURE",           // HDMI_HDR_EOTF_FUTURE = 3,
    "UNKNOW",           // HDMI_HDR_EOTF_BUTT,
};

const static HI_CHAR *s_eHdrColorimetry[] = {
    "NONE",
    "ITU_601",
    "ITU_709",
    "EXTENDED",
    "XV_YCC_601",
    "XV_YCC_709",
    "S_YCC_601",
    "ADOBE_YCC_601",
    "ADOBE_RGB",
    "2020_const_luminous",
    "2020_nconst_luminous",
};

const static HI_CHAR *s_eHdrUserMode2Str[] = {
	"SDR",          //HDMI_HDR_USERMODE_SDR = 0x00,
	"HDR10",	    //HDMI_HDR_USERMODE_HDR10,       
	"DOLBY",		//HDMI_HDR_USERMODE_DOLBY,       
	"UNKNOW",		//HDMI_HDR_USERMODE_BUTT,
};

#endif

const static HI_CHAR *s_eHdcpMode2Str[] = {
    "AUTO",             //HDMI_HDCP_MODE_AUTO=0,
    "HDCP1.4",			//HDMI_HDCP_MODE_1_4 = 1,
    "HDCP2.2",			//HDMI_HDCP_MODE_2_2=2,
    "UNKNOW",			//HDMI_HDCP_MODE_BUTT,
};

#ifdef HDMI_CEC_SUPPORT
const static HI_CHAR *s_eNetwork2Str[] = {
	"TV",				//HDMI_CEC_LOGICALADD_TV               = 0X00, 
    "RECORDDEV_1",		//HDMI_CEC_LOGICALADD_RECORDDEV_1      = 0X01, 
    "RECORDDEV_2",		//HDMI_CEC_LOGICALADD_RECORDDEV_2      = 0X02, 
    "TUNER_1",			//HDMI_CEC_LOGICALADD_TUNER_1          = 0X03, 
    "PLAYDEV_1",		//HDMI_CEC_LOGICALADD_PLAYDEV_1        = 0X04,
    "AUDIOSYSTEM",		//HDMI_CEC_LOGICALADD_AUDIOSYSTEM      = 0X05,
    "TUNER_2",			//HDMI_CEC_LOGICALADD_TUNER_2          = 0X06,
    "TUNER_3",			//HDMI_CEC_LOGICALADD_TUNER_3          = 0X07, 
    "PLAYDEV_2",		//HDMI_CEC_LOGICALADD_PLAYDEV_2        = 0X08, 
    "RECORDDEV_3",		//HDMI_CEC_LOGICALADD_RECORDDEV_3      = 0X09, 
    "TUNER_4",			//HDMI_CEC_LOGICALADD_TUNER_4          = 0X0A, 
    "PLAYDEV_3",		//HDMI_CEC_LOGICALADD_PLAYDEV_3        = 0X0B, 
    "RESERVED_1",		//HDMI_CEC_LOGICALADD_RESERVED_1       = 0X0C, 
    "RESERVED_2",		//HDMI_CEC_LOGICALADD_RESERVED_2       = 0X0D, 
    "SPECIALUSE",		//HDMI_CEC_LOGICALADD_SPECIALUSE       = 0X0E, 
    "BROADCAST",		//HDMI_CEC_LOGICALADD_BROADCAST        = 0X0F, 
						//HDMI_CEC_LOGICALADD_BUTT
};
#endif

#if ((defined HDMI_HDR_SUPPORT) && (defined HDMI_CHIP_TYPE_HI3798CV200))
const static HI_CHAR *s_eHdrDbgMode2Str[] = {
    "DISABLE",
    "OE",
    "AVMUTE",
    "UNKNOW",
};
#endif

const static HI_CHAR *s_eColorSpace2Str[] = {
	/* EIA-CEA-861-F P43 */
	"RGB",				//HDMI_COLORSPACE_RGB = 0, 		0b000
	"YCbCr422",			//HDMI_COLORSPACE_YCbCr422 =1 ,	0b001
	"YCbCr444",			//HDMI_COLORSPACE_YCbCr444 = 2,	0b010
	"YCbCr420",			//HDMI_COLORSPACE_YCbCr420 = 3,	0b011
	"UNKNOW" 
};
	
const static HI_CHAR *s_PictureScaling2Str[] = {
	"UNKNOW", 			//0b00
	"HpVn", 				//0b01 Picture has been scaled horizontally 
	"HnVp ",				//0b10 Picture has been scaled vertically
	"HpVp", 				//0b11 Picture has been scaled horizontally and vertically
};

const static HI_CHAR *s_eRgbQuantization2Str[] = {
	"DEFAULT",				//HDMI_QUANTIZATION_RANGE_DEFAULT	0b00
	"LIMITED",				//HDMI_QUANTIZATION_RANGE_LIMITED	0b01
	"FULL", 				//HDMI_QUANTIZATION_RANGE_FULL		0b10
	"UNKNOW", 			    //HDMI_QUANTIZATION_RANGE_BUTT	0b11
};

const static HI_CHAR *s_eExtColorimetry2Str[] = {
	"XV_YCC601", 			//	0b000 HDMI_EXTENDED_COLORIMETRY_XV_YCC_601
	"XV_YCC709" ,			//	0b001 HDMI_EXTENDED_COLORIMETRY_XV_YCC_709
	"S_YCC601",				//	0b010 HDMI_EXTENDED_COLORIMETRY_S_YCC_601
	"ADOBE_YCC601",			//	0b011 HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601
	"ADOBE_RGB", 			//	0b100 HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB
	"BT2020_cYCC", 			//	0b101 HDMI_EXTENDED_COLORIMETRY_2020_const_luminous
	"BT2020_RGB/YCC", 		//	0b110 HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous
	"UNKNOW"				//	0b111 HDMI_EXTENDED_COLORIMETRY_BUTT
};

const static HI_CHAR *s_SoundIntf2Str[] = {
    "I2S",				//HDMI_AUDIO_INTERFACE__I2S = 0,
    "SPDIF",			//HDMI_AUDIO_INTERFACE__SPDIF = 1, 
    "HBRA",				//HDMI_AUDIO_INTERFACE__HBRA = 2, 
    "UNKNOW",			//HDMI_AUDIO_INTERFACE__BUTT
};

const static HI_CHAR *s_aStr_Speaker[] = {

	"FL_FR",			//HDMI_EDID_AUDIO_SPEAKER_FL_FR   ,
    "LFE",				//HDMI_EDID_AUDIO_SPEAKER_LFE     , 
    "FC",				//HDMI_EDID_AUDIO_SPEAKER_FC      , 
    "RL_RR",			//HDMI_EDID_AUDIO_SPEAKER_RL_RR   ,
    "RC",				//HDMI_EDID_AUDIO_SPEAKER_RC      ,
    "FLC_FRC",			//HDMI_EDID_AUDIO_SPEAKER_FLC_FRC , 
    "RLC_RRC",			//HDMI_EDID_AUDIO_SPEAKER_RLC_RRC ,
    "FLW_FRW",			//HDMI_EDID_AUDIO_SPEAKER_FLW_FRW ,
    "FLH_FRH",			//HDMI_EDID_AUDIO_SPEAKER_FLH_FRH ,
    "TC",				//HDMI_EDID_AUDIO_SPEAKER_TC      ,
    "FCH",				//HDMI_EDID_AUDIO_SPEAKER_FCH     ,
	"UNKNOW"			//HDMI_EDID_AUDIO_SPEAKER_BUTT    ,
};

const static HI_CHAR *s_eStereMode2Str[] = {
	"FRAME_PACK",		//HDMI_EDID_3D_FRAME_PACKETING				 = 0x00, /**<3d type:Frame Packing*/
	"FIELD_ALTER",		//HDMI_EDID_3D_FIELD_ALTERNATIVE			 = 0x01, /**<3d type:Field alternative*/
	"LINE_ALTERN",		//HDMI_EDID_3D_LINE_ALTERNATIVE				 = 0x02, /**<3d type:Line alternative*/
	"SBS_FULL",			//HDMI_EDID_3D_SIDE_BY_SIDE_FULL			 = 0x03, /**<3d type:Side by side full*/
	"L_DEPTH",			//HDMI_EDID_3D_L_DEPTH						 = 0x04, /**<3d type:L+depth*/
	"L_DEPTH_GGD",		//HDMI_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH = 0x05, /**<3d type:L+depth+Graphics+Graphics-depth*/
	"TAndB",			//HDMI_EDID_3D_TOP_AND_BOTTOM 				 = 0x06, /**<3d type:Top and Bottom*/
    "Reserved",         //
	"SByS_HALF",		//HDMI_EDID_3D_SIDE_BY_SIDE_HALF
    "NONE"              //HDMI_3D_BUTT
};

/*VESA Enhanced EDID Standard Release A, Rev.1 Page 16 */
const static HI_CHAR *s_EstabTiming2Str[] = {
	"V800X600_60",			//HDMI_EDID_ESTABTIMG_VESA_800X600_60 		,0
	"V800X600_56",			//HDMI_EDID_ESTABTIMG_VESA_800X600_56			,
	"V640X480_75",			//HDMI_EDID_ESTABTIMG_VESA_640X480_75			,
	"V640X480_72",			//HDMI_EDID_ESTABTIMG_VESA_640X480_72			,
	"V640X480_67",			//HDMI_EDID_ESTABTIMG_IBM_VGA_640X480_67		,
	"V640X480_60",			//HDMI_EDID_ESTABTIMG_IBM_XGA_640X480_60		,5
	"V720X400_88",			//HDMI_EDID_ESTABTIMG_IBM_XGA2_720X400_88		,
	"V720X400_70",			//HDMI_EDID_ESTABTIMG_IBM_XGA_720X400_70		,
	"V1280X1024_75",		//HDMI_EDID_ESTABTIMG_VESA_1280X1024_75		,
	"V1024X768_75",			//HDMI_EDID_ESTABTIMG_VESA_1024X768_75		,
	"V1024X768_70",			//HDMI_EDID_ESTABTIMG_VESA_1024X768_70		,
	"V1024X768_60",			//HDMI_EDID_ESTABTIMG_VESA_1024X768_60		,11
	"V1024X768_87",			//HDMI_EDID_ESTABTIMG_IBM_1024X768_87			,
	"V832X624_75",			//HDMI_EDID_ESTABTIMG_APPLE_MACII_832X624_75	,
	"V800X600_75",			//HDMI_EDID_ESTABTIMG_VESA_800X600_75			,
	"V800X600_72",			//HDMI_EDID_ESTABTIMG_VESA_800X600_72			,
	"V1152X870_75",			//HDMI_EDID_ESTABTIMG_VESA_1152X870_75		,
    "UNKNOW"	       		//HDMI_EDID_ESTABTIMG_BUTT					,
 };

const static HI_CHAR *s_eDefaultAction2Str[] = {
	"NONE",         //HDMI_DEFAULT_ACTION_NULL, 
	"HDMI",			//HDMI_DEFAULT_ACTION_HDMI,
	"DVI",			//HDMI_DEFAULT_ACTION_DVI,
	"UNKNOW"		//HDMI_DEFAULT_ACTION_BUTT,
};

const static HI_CHAR *s_eDeepColor2Str[] = {
	"24",           //HDMI_DEEP_COLOR_24BIT = 0x00,
	"30",			//HDMI_DEEP_COLOR_30BIT,       
	"36",			//HDMI_DEEP_COLOR_36BIT,       
	"48",			//HDMI_DEEP_COLOR_48BIT,       
    "OFF",          //HDMI_DEEP_COLOR_OFF   = 0xff,
    "UNKNOW"        //HDMI_DEEP_COLOR_BUTT
};

const static HI_CHAR *s_eTransitionSate2Str[] = {
//	"NONE",          //HDMI_TRANSITION_NONE     = 0x0,
	"BOOT->MCE",	 //HDMI_TRANSITION_BOOT_MCE = 0x1,
	"MCE->APP",		 //HDMI_TRANSITION_MCE_APP  = 0x2,
	"BOOT->APP",	 //HDMI_TRANSITION_BOOT_APP = 0x4,
};


const static HI_CHAR *s_eDispFMT2Str[] = {

#ifdef HDMI_CHIP_HI3536
       "1080P@60",    //HI_HDMI_VIDEO_FMT_1080P_60 = 0,
       "1080P@50",    //HI_HDMI_VIDEO_FMT_1080P_50,
       "1080P@30",    //HI_HDMI_VIDEO_FMT_1080P_30,
       "1080P@25",    //HI_HDMI_VIDEO_FMT_1080P_25,
       "1080P@24",    //HI_HDMI_VIDEO_FMT_1080P_24,
       "1080I@60",    //HI_HDMI_VIDEO_FMT_1080i_60,
       "1080I@50",    //HI_HDMI_VIDEO_FMT_1080i_50,
       "720P@60",     //HI_HDMI_VIDEO_FMT_720P_60,
       "720P@50",     //HI_HDMI_VIDEO_FMT_720P_50,
       "756P@50",     //HI_HDMI_VIDEO_FMT_576P_50,
       "480P@60",     //HI_HDMI_VIDEO_FMT_480P_60,

       "PAL",         //HI_HDMI_VIDEO_FMT_PAL,                  /* B D G H I PAL */
       "PAL_N",       //HI_HDMI_VIDEO_FMT_PAL_N,                /* (N)PAL        */
       "PAL_NC",      //HI_HDMI_VIDEO_FMT_PAL_Nc,               /* (Nc)PAL       */
       "NTSC",        //HI_HDMI_VIDEO_FMT_NTSC,                 /* (M)NTSC       */
       "NTSC_J",      //HI_HDMI_VIDEO_FMT_NTSC_J,               /* NTSC-J        */
       "PAL_M",       //HI_HDMI_VIDEO_FMT_NTSC_PAL_M,           /* (M)PAL        */
       "SECAM_SIN",    //HI_HDMI_VIDEO_FMT_SECAM_SIN,            /**< SECAM_SIN*/
       "SECAM_COS",    //HI_HDMI_VIDEO_FMT_SECAM_COS,            /**< SECAM_COS*/

       "640X480@60",    //HI_HDMI_VIDEO_FMT_861D_640X480_60,
       "800X600@60",    //HI_HDMI_VIDEO_FMT_VESA_800X600_60,
       "1024X768@60",   //HI_HDMI_VIDEO_FMT_VESA_1024X768_60,
       "1280X720@60",    //HI_HDMI_VIDEO_FMT_VESA_1280X720_60,
       "1280X800@60",    //HI_HDMI_VIDEO_FMT_VESA_1280X800_60,
       "1280X1024@60",    //HI_HDMI_VIDEO_FMT_VESA_1280X1024_60,
       "1366X768@60",    //HI_HDMI_VIDEO_FMT_VESA_1366X768_60,
       "1440X900@60",    //HI_HDMI_VIDEO_FMT_VESA_1440X900_60,
       "1440X900@60_RB",    //HI_HDMI_VIDEO_FMT_VESA_1440X900_60_RB,
       "1600X900@60_RB",    //HI_HDMI_VIDEO_FMT_VESA_1600X900_60_RB,
       "1600X1200@60",    //HI_HDMI_VIDEO_FMT_VESA_1600X1200_60,
       "1680X1050@60",    //HI_HDMI_VIDEO_FMT_VESA_1680X1050_60,
       "1920X1080@60",    //HI_HDMI_VIDEO_FMT_VESA_1920X1080_60,
       "1920X1200@60",    //HI_HDMI_VIDEO_FMT_VESA_1920X1200_60,
       "2048X1152@60",    //HI_HDMI_VIDEO_FMT_VESA_2048X1152_60,    /* Currently, not support */

       "2560x1440@30",    //HI_HDMI_VIDEO_FMT_2560x1440_30,
       "2560x1600@60",    //HI_HDMI_VIDEO_FMT_2560x1600_60,
       "1920x2160@30",    //HI_HDMI_VIDEO_FMT_1920x2160_30,

       "3840X2160P@24",    //HI_HDMI_VIDEO_FMT_3840X2160P_24,        /** 93:3840x2160p @ 24Hz    No Repetition */
       "3840X2160P@25",    //HI_HDMI_VIDEO_FMT_3840X2160P_25,        /** 94:3840x2160p @ 25Hz    No Repetition */
       "3840X2160P@30",    //HI_HDMI_VIDEO_FMT_3840X2160P_30,        /** 95:3840x2160p @ 30Hz    No Repetition */
       "3840X2160P@50",    //HI_HDMI_VIDEO_FMT_3840X2160P_50,        /** 96:3840x2160p @ 50Hz    No Repetition */
       "3840X2160P@60",    //HI_HDMI_VIDEO_FMT_3840X2160P_60,        /** 97:3840x2160p @ 60Hz    No Repetition */

       "4096X2160P@24",    //HI_HDMI_VIDEO_FMT_4096X2160P_24,        /** 98:4096x2160p @ 24Hz    No Repetition */
       "4096X2160P@25",    //HI_HDMI_VIDEO_FMT_4096X2160P_25,        /** 99:4096x2160p @ 25Hz    No Repetition */
       "4096X2160P@30",    //HI_HDMI_VIDEO_FMT_4096X2160P_30,        /** 100:4096x2160p @ 30Hz   No Repetition */
       "4096X2160P@50",    //HI_HDMI_VIDEO_FMT_4096X2160P_50,        /** 101:4096x2160p @ 50Hz   No Repetition */
       "4096X2160P@60",    //HI_HDMI_VIDEO_FMT_4096X2160P_60,        /** 102:4096x2160p @ 60Hz   No Repetition */
       "CUSTOMER_DEFINE",    //HI_HDMI_VIDEO_FMT_VESA_CUSTOMER_DEFINE, /** Currently, not support */
       "FMT_UNKNOW",         //HI_HDMI_VIDEO_FMT_BUTT
#else
    "1080P@60",         //HI_DRV_DISP_FMT_1080P_60 = 0,     /**<1080p 60 Hz*/
    "1080P@50",         //HI_DRV_DISP_FMT_1080P_50,         /**<1080p 50 Hz*/
    "1080P@30",         //HI_DRV_DISP_FMT_1080P_30,         /**<1080p 30 Hz*/
    "1080P@25",         //HI_DRV_DISP_FMT_1080P_25,         /**<1080p 25 Hz*/
    "1080P@24",         //HI_DRV_DISP_FMT_1080P_24,         /**<1080p 24 Hz*/
    "1080I@60",         //HI_DRV_DISP_FMT_1080i_60,         /**<1080i 60 Hz*/
    "1080I@50",         //HI_DRV_DISP_FMT_1080i_50,         /**<1080i 60 Hz*/
    "720P@60",          //HI_DRV_DISP_FMT_720P_60,          /**<720p 60 Hz*/
    "720P@50",          //HI_DRV_DISP_FMT_720P_50,          /**<720p 50 Hz */
    "576P@50",          //HI_DRV_DISP_FMT_576P_50,          /**<576p 50 Hz*/
    "480P@60",          //HI_DRV_DISP_FMT_480P_60,          /**<480p 60 Hz*/

    "PAL",              //HI_DRV_DISP_FMT_PAL,              /* B D G H I PAL */
    "PAL_B",            //HI_DRV_DISP_FMT_PAL_B,            /* B PAL， Australia */
    "PAL_B1",           //HI_DRV_DISP_FMT_PAL_B1,           /* B1 PAL, Hungary */
    "PAL_D",            //HI_DRV_DISP_FMT_PAL_D,            /* D PAL, China */
    "PAL_D1",           //HI_DRV_DISP_FMT_PAL_D1,           /* D1 PAL， Poland */
    "PAL_G",            //HI_DRV_DISP_FMT_PAL_G,            /* G PAL， Europe */
    "PAL_H",            //HI_DRV_DISP_FMT_PAL_H,            /* H PAL， Europe */
    "PAL_K",            //HI_DRV_DISP_FMT_PAL_K,            /* K PAL， Europe */
    "PAL_I",            //HI_DRV_DISP_FMT_PAL_I,            /* I PAL，U.K. */
    "PAL_N",            //HI_DRV_DISP_FMT_PAL_N,            /* N PAL, Jamaica/Uruguay */
    "PAL_Nc",           //HI_DRV_DISP_FMT_PAL_Nc,           /* Nc PAL, Argentina:21 */
    "PAL_M",            //HI_DRV_DISP_FMT_PAL_M,            /* M PAL, 525 lines */
    "PAL_60",           //HI_DRV_DISP_FMT_PAL_60,           /* 60 PAL */

    "NTSC",             //HI_DRV_DISP_FMT_NTSC,             /* (M)NTSC       */
    "NTSC_J",           //HI_DRV_DISP_FMT_NTSC_J,           /* NTSC-J        */
    "NTSC_443",         //HI_DRV_DISP_FMT_NTSC_443,          /* (M)PAL        */

    "SECAM_SIN",        //HI_DRV_DISP_FMT_SECAM_SIN,      /**< SECAM_SIN*/
    "SECAM_COS",        //HI_DRV_DISP_FMT_SECAM_COS,      /**< SECAM_COS*/
    "SECAM_L",          //HI_DRV_DISP_FMT_SECAM_L,        /**< France*/
    "SECAM_B",          //HI_DRV_DISP_FMT_SECAM_B,        /**< Middle East*/
    "SECAM_G",          //HI_DRV_DISP_FMT_SECAM_G,        /**< Middle East*/
    "SECAM_D",          //HI_DRV_DISP_FMT_SECAM_D,        /**< Eastern Europe*/
    "SECAM_K",          //HI_DRV_DISP_FMT_SECAM_K,        /**< Eastern Europe*/
    "SECAM_H",          //HI_DRV_DISP_FMT_SECAM_H,        /**< Line SECAM:34*/

    "1440x576i@50",     //HI_DRV_DISP_FMT_1440x576i_50,
    "1440x480i@60",     //HI_DRV_DISP_FMT_1440x480i_60, /*sequnce:36*/
    "1080P@24_FP",      //HI_DRV_DISP_FMT_1080P_24_FP,
    "720P@60_FP",       //HI_DRV_DISP_FMT_720P_60_FP,
    "720P@50_FP",       //HI_DRV_DISP_FMT_720P_50_FP,
    "640x480@60",       //HI_DRV_DISP_FMT_861D_640X480_60,

    "800x600@60",       //HI_DRV_DISP_FMT_VESA_800X600_60,
    "1024x768@60",      //HI_DRV_DISP_FMT_VESA_1024X768_60,
    "1280x720@60",      //HI_DRV_DISP_FMT_VESA_1280X720_60,
    "1280x800@60",      //HI_DRV_DISP_FMT_VESA_1280X800_60,
    "1280x1024@60",     //HI_DRV_DISP_FMT_VESA_1280X1024_60,
    "1360x768@60",      //HI_DRV_DISP_FMT_VESA_1360X768_60,         //Rowe
    "1366x768@60",      //HI_DRV_DISP_FMT_VESA_1366X768_60,
    "1400x1050@60",     //HI_DRV_DISP_FMT_VESA_1400X1050_60,        //Rowe
    "1440x900@60",      //HI_DRV_DISP_FMT_VESA_1440X900_60,
    "1440x900@60_RB",   //HI_DRV_DISP_FMT_VESA_1440X900_60_RB,
    "1600x900@60_RB",   //HI_DRV_DISP_FMT_VESA_1600X900_60_RB,
    "1600x1200@60",     //HI_DRV_DISP_FMT_VESA_1600X1200_60,
    "1680x1050@60",     //HI_DRV_DISP_FMT_VESA_1680X1050_60,       //Rowe
    "1680x1050@60_RB",  //HI_DRV_DISP_FMT_VESA_1680X1050_60_RB,       //Rowe
    "1920x1080@60",     //HI_DRV_DISP_FMT_VESA_1920X1080_60,
    "1920x1200@60",     //HI_DRV_DISP_FMT_VESA_1920X1200_60,
    "1920x1440@60",     //HI_DRV_DISP_FMT_VESA_1920X1440_60,
    "2048x1152@60",     //HI_DRV_DISP_FMT_VESA_2048X1152_60,
    "2560x1440@60_RB",  //HI_DRV_DISP_FMT_VESA_2560X1440_60_RB,
    "2560x1600@60",     //HI_DRV_DISP_FMT_VESA_2560X1600_60_RB, /*sequence:60*/

    "3840x2160@24",     //HI_DRV_DISP_FMT_3840X2160_24,/*reserve for extention*/
    "3840x2160@25",     //HI_DRV_DISP_FMT_3840X2160_25,
    "3840x2160@30",     //HI_DRV_DISP_FMT_3840X2160_30,
    "3840x2160@50",     //HI_DRV_DISP_FMT_3840X2160_50,
    "3840x2160@60",     //HI_DRV_DISP_FMT_3840X2160_60,

    "4096x2160@24",     //HI_DRV_DISP_FMT_4096X2160_24,/*reserve for extention*/
    "4096x2160@25",     //HI_DRV_DISP_FMT_4096X2160_25,
    "4096x2160@30",     //HI_DRV_DISP_FMT_4096X2160_30,
    "4096x2160@50",     //HI_DRV_DISP_FMT_4096X2160_50,
    "4096x2160@60",     //HI_DRV_DISP_FMT_4096X2160_60,

    "3840x2160@23.976", //HI_DRV_DISP_FMT_3840X2160_23_976,
    "3840x2160@29.97",  //HI_DRV_DISP_FMT_3840X2160_29_97,
    "3840x2160@59.94",  //HI_DRV_DISP_FMT_720P_59_94,
    "1080P@59.94",      //HI_DRV_DISP_FMT_1080P_59_94,
    "1080P@29.97",      //HI_DRV_DISP_FMT_1080P_29_97,
    "1080P@23.976",     //HI_DRV_DISP_FMT_1080P_23_976,
    "1080I@59.94",      //HI_DRV_DISP_FMT_1080i_59_94,

    "CUSTOM",           //HI_DRV_DISP_FMT_CUSTOM,
    "UNKNOW"            //HI_DRV_DISP_FMT_BUTT
#endif    
};

const static HI_CHAR *s_eBitDepth2Str[] = {
    "08 Bit",      //HDMI_VIDEO_BITDEPTH_8,
    "10 Bit",      //HDMI_VIDEO_BITDEPTH_10,
    "12 Bit",      //HDMI_VIDEO_BITDEPTH_12,
    "16 Bit",      //HDMI_VIDEO_BITDEPTH_16,
    "08 Bit(OFF)", //HDMI_VIDEO_BITDEPTH_OFF,
    "OFF/UNKNOW"   //HDMI_VIDEO_BITDEPTH_BUTT
};

const static HI_CHAR *s_eAudioChannel2Str[] = {
	"Reserved",
    "Reserved",
    "2_CH", //HDMI_AUDIO_FORMAT_2CH = 0x02,
    "3_CH", //HDMI_AUDIO_FORMAT_3CH,
    "4_CH", //HDMI_AUDIO_FORMAT_4CH,
    "5_CH", //HDMI_AUDIO_FORMAT_5CH,
    "6_CH", //HDMI_AUDIO_FORMAT_6CH,
    "7_CH", //HDMI_AUDIO_FORMAT_7CH,
    "8_CH", //HDMI_AUDIO_FORMAT_8CH
    "UNKNOW" //HDMI_AUDIO_FORMAT_BUTT
};

static const HDMI_FMT_NAME_PARAM_S s_stFmtParam[]= {
	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    Fmt************************/
	{						 1, 25175 ,  640 ,480 , 6000 ,	"640X480_60 4:3"		},		// 1. 640 x 480p @ 60 VGA
	{						 2, 27000 ,  720 ,480 , 6000 ,	"480P_60 4:3"			},		// 2,3 720 x 480p
	{						 3, 27000 ,  720 ,480 , 6000 ,	"480P_60 16:9"			},		// 2,3 720 x 480p
	{						 4, 74170 ,  1280,720 , 6000 ,	"720P_60 16:9"			},		// 4   1280 x 720p
	{						 5, 74170 ,  1920,1080, 6000 ,	"1080i_60 16:9" 		},		// 5 1920 x 1080i
	{						 6, 27000 ,  720 ,480 , 6000 ,	"NTSC 4:3" 				},		// 6,7 720 x 480i, pix repl
	{						 7, 27000 ,  720 ,480 , 6000 ,	"NTSC 16:9" 			},		// 6,7 720 x 480i, pix repl

	{						 8, 27000 ,  1440,240 , 6000 ,	"240P_60 4:3" 			},		// 
	{						 9, 27000 ,  1440,240 , 6000 ,	"240P_60 16:9" 			},		// 
	{						10, 54000 ,  720 ,480 , 6000 ,	"2880X480i_60 4:3" 		},		// 
	{						11, 54000 ,  720 ,480 , 6000 ,	"2880X480i_60 16:9" 	},		// 
	{						12, 54000 ,  2880,240 , 6000 ,	"2880X240P_60 4:3" 		},		// 
	{						13, 54000 ,  2880,240 , 6000 ,	"2880X240P_60 16:9" 	},		// 
	{						14, 54000 ,  1440,480 , 6000 ,	"1440X480P_60 4:3" 		},		// 
	{						15, 54000 ,  1440,480 , 6000 ,	"1440X480P_60 16:9" 	},		// 
	
	{						16, 148500,  1920,1080, 6000 ,	"1080P_60 16:9" 		},		// 16 1920 x 1080p
	{						17, 27000 ,  720 ,576 , 5000 ,	"576P_50 4:3"			},		// 17,18 720 x 576p
	{						18, 27000 ,  720 ,576 , 5000 ,	"576P_50 16:9"			},		// 17,18 720 x 576p
	{						19, 74250 ,  1280,720 , 5000 ,	"720P_50 16:9"			},		// 19 1280 x 720p
	{						20, 74250 ,  1920,1080, 5000 ,	"1080i_50 16:9" 		},		// 20 1920 x 1080i
	{						21, 27000 ,  1440,576 , 5000 ,	"PAL 4:3"				},		// 21,22 1440 x 576i
	{						22, 27000 ,  1440,576 , 5000 ,	"PAL 16:9"				},		// 21,22 1440 x 576i
	
	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    Fmt************************/
	{						31, 148500,  1440,576 , 5000 ,	"1080P_50 16:9" 		},		// 31(1) 1920 x 1080p
	{						32, 74170 ,  1920,1080, 2400 ,	"1080P_24 16:9" 		},		// 32(2) 1920 x 1080p
	{						33, 74250 ,  1920,1080, 2500 ,	"1080P_25 16:9" 		},		// 33(3) 1920 x 1080p
	{						34, 74170 ,  1920,1080, 3000 ,	"1080P_30 16:9" 		},		// 34(4) 1920 x 1080p

	{						60, 59400 ,  1280,720 , 2400 ,	"720P_24 16:9"			},		// 60   1280 x 720p
	{						61, 74250 ,  1280,720 , 2500 ,	"720P_25 16:9"			},		// 61   1280 x 720p
	{						62, 74250 ,  1280,720 , 3000 ,	"720P_30 16:9"			},		// 62   1280 x 720p
	/*4K * 2K*/
	{						93,297000 ,  3840,2160, 2400 ,	"3840X2160P24 16:9" 		},		
	{						94,297000 ,  3840,2160, 2500 ,	"3840X2160P25 16:9" 		},		
	{						95,297000 ,  3840,2160, 3000 ,	"3840X2160P30 16:9" 		},		
	{						96,594000 ,  3840,2160, 5000 ,	"3840X2160P50 16:9" 		},		
	{						97,594000 ,  3840,2160, 6000 ,	"3840X2160P60 16:9" 		},		

	{						98,297000 ,  4096,2160, 2400 ,	"4096X2160P24 256:135" 		},		
	{						99,297000 ,  4096,2160, 2500 ,	"4096X2160P25 256:135" 		},		
	{					   100,297000 ,  4096,2160, 3000 ,	"4096X2160P30 256:135" 		},		
	{					   101,594000 ,  4096,2160, 5000 ,	"4096X2160P50 256:135" 		},
	{					   102,594000 ,  4096,2160, 6000 ,	"4096X2160P60 256:135" 		},

	{					   103,297000 ,  3840,2160, 2400 ,	"3840X2160P24 64:27" 			},		
	{					   104,297000 ,  3840,2160, 2500 ,	"3840X2160P25 64:27" 			},		
	{					   105,297000 ,  3840,2160, 3000 ,	"3840X2160P30 64:27" 			},		
	{					   106,594000 ,  3840,2160, 5000 ,	"3840X2160P50 64:27" 			},		
	{					   107,594000 ,  3840,2160, 6000 ,	"3840X2160P60 64:27" 			},
	
#ifdef DVI_SUPPORT
	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    Fmt************************/
	
	{ HDMI_VIC_PC_BASE+ 	 9, 40000 , 800 ,600 ,	6000 ,	"800X600_60"	},		// 800x600@60.317
	{ HDMI_VIC_PC_BASE+ 	13, 65000 , 1024,768 ,	6000 ,	"1024X768_60"	},		// 1024x768@60
	{ HDMI_VIC_PC_BASE+ 	18, 162000, 1600,1200,	6000 ,	"1600X1200_60"	},		// 1600x1200@60

	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    Fmt************************/
	{ HDMI_VIC_PC_BASE+ 	25, 108000, 1280,1024,	6000 ,	"1280X1024_60"	},		// 1280x1024@60
	{ HDMI_VIC_PC_BASE+ 	28, 85500 , 1360,768 ,	6000 ,	"1366X768_60"	},		// 1360x768@60
	{ HDMI_VIC_PC_BASE+ 	41, 154000, 1920,1200,	6000 ,	"1920X1200_60"	},		// 1920x1200@59.95
#endif
	/*for 2k & 4k ,we use VIRTUAL VIC*/
	{ HDMI_EDID_VIRTUAL_VIC_3840X2160_30, 297000,  3840,2160, 3000 ,	"3840X2160P_30" 		}, 		// 1 2160P@30,3840X2160_30
	{ HDMI_EDID_VIRTUAL_VIC_3840X2160_25, 297000,  3840,2160, 2500 ,	"3840X2160P_25" 		}, 		// 2 2160P@25,3840X2160_25
	{ HDMI_EDID_VIRTUAL_VIC_3840X2160_24, 297000,  3840,2160, 2400 ,	"3840X2160P_24" 		}, 		// 3 2160P@24,3840X2160_24
	{ HDMI_EDID_VIRTUAL_VIC_4096X2160_24, 297000,  4096,2160, 2400 ,	"4096X2160P_24" 		}, 		// 4 4k*2K SMTP 4096*2160_24

};

const static HI_CHAR* g_pszVersion = "HDMI Version:   "MAKE_VERSION;


/******************************************************************************/
/* Private Interface                                                          */
/******************************************************************************/
/** Get format with VIC
**/
static HI_CHAR * Proc_Vic2Format(HI_U32 u32Vic)
{
	HI_U32 i = 0;
	HDMI_FMT_NAME_PARAM_S *pstFmtParam = HI_NULL;

	for (i = 0; i < ARRAY_SIZE(s_stFmtParam); i++)
	{
		pstFmtParam = (HDMI_FMT_NAME_PARAM_S *)&s_stFmtParam[i];
		if (   (pstFmtParam != HI_NULL)
			&& (pstFmtParam->u32Vic == u32Vic)    )
		{
			return pstFmtParam->pcFormat;
		}
	}

	return HI_NULL;
	
}

/** Get format with StdTiming
**/
 static HI_CHAR * Proc_StdTiming2Format(HDMI_EDID_STD_TIMING_S *pstStdTiming)
{
	HI_U32 i = 0;
	HDMI_FMT_NAME_PARAM_S *pstFmtParam = HI_NULL;

	if (pstStdTiming == HI_NULL)
	{
		return HI_NULL;
	}

	for (i=0;i<ARRAY_SIZE(s_stFmtParam);i++)
	{
		pstFmtParam = (HDMI_FMT_NAME_PARAM_S *)&s_stFmtParam[i];
		if (   (pstFmtParam != HI_NULL) 
			&& (pstFmtParam->u32HorActive == pstStdTiming->u32HorActive)
			&& (pstFmtParam->u32VerActive == pstStdTiming->u32VerActive)
			&& (pstFmtParam->u32FieldRate >= pstStdTiming->u32RefreshRate)
			&& ((pstFmtParam->u32FieldRate-PROC_FEILD_RATE_DEVIATION) <= pstStdTiming->u32RefreshRate)  )
		{
			return pstFmtParam->pcFormat;
		}
	}

	return HI_NULL;
	
}

/** print software status
**/
static HI_VOID Proc_SWVersion(struct seq_file *pFile)
{
	PROC_PRINT(pFile, "%s\n", g_pszVersion);

    return;
}

/** print app config information
**/
static HI_VOID Proc_AppAttrInfo(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
	HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_APP_ATTR_S *pstAppAttr = HI_NULL;
    HI_U32 u32Index = 0;

	pstHdmiDev = GetHdmiDevice(enHdmiID);
	CHECK_POINTER(pstHdmiDev);

    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);
	PROC_PRINT(pFile, "-------------------------- APPAttr -----------------------------\n");
    /* line 1 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HDMIEnable", pstAppAttr->bEnableHdmi? STR_YES: STR_NO);
    CHECK_MAX_VALUE(u32Index, pstAppAttr->enHdmiAction, HDMI_DEFAULT_ACTION_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "DefaultAction", s_eDefaultAction2Str[u32Index]);

    /* line 2 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "VideoEnable", pstAppAttr->bEnableVideo? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "AudioEnable", pstAppAttr->bEnableAudio? STR_YES: STR_NO);

    /* line 3 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "AviInfoEnable", pstAppAttr->bEnableAviInfoFrame? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "AudioInfoEnable", pstAppAttr->bEnableAudInfoFrame? STR_YES: STR_NO);

    /* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "xvYCCMode", pstAppAttr->bxvYCCMode? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "bHDCPEnable", pstAppAttr->bHDCPEnable? STR_YES: STR_NO);

    /* line 5 */
    CHECK_MAX_VALUE(u32Index, pstAppAttr->enDeepColorMode, HDMI_DEEP_COLOR_48BIT + 1);
    PROC_PRINT(pFile, "%-15s: %-20s  ", "DeepColorMode", s_eDeepColor2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "SpdInfoEnable", pstAppAttr->bEnableSpdInfoFrame? STR_YES: STR_NO);

    /* line 6 */
    CHECK_MAX_VALUE(u32Index, pstAppAttr->enOutColorSpace, HDMI_COLORSPACE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  ", "OutColorSpace", s_eColorSpace2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "MpegInfoEnable", pstAppAttr->bEnableMpegInfoFrame? STR_YES: STR_NO);

    /* line 7 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "ColorSpaceAdapt", pstAppAttr->bEnableClrSpaceAdapt? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "DeepColorAdapt", pstAppAttr->bEnableDeepClrAdapt? STR_YES: STR_NO);

    /* line 8 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "DebugEnable", pstAppAttr->bDebugFlag? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "CtsAuthEnable", pstAppAttr->bAuthMode? STR_YES: STR_NO);
    
    /* line 9 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "enHDCPMode", s_eHdcpMode2Str[pstAppAttr->enHDCPMode]);
#ifdef HDMI_HDR_SUPPORT
    PROC_PRINT(pFile, "%-15s: %-20s  ", "DrmInfoEnable", pstAppAttr->bEnableDRMInfoFrame? STR_YES: STR_NO);
#endif
	PROC_PRINT(pFile, "\n");
    return;
}

/** print software status
**/
static HI_VOID Proc_SWStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
	HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
	HI_U32 i = 0;
	HI_U32 u32Index = 0;

	pstHdmiDev = GetHdmiDevice(enHdmiID);
	CHECK_POINTER(pstHdmiDev);
	PROC_PRINT(pFile, "-------------------------- SWStatus ----------------------------\n");
	
	/* line 1 */
	PROC_PRINT(pFile, "%-15s: %-20s  ", "ThreadRun", (HDMI_THREAD_STATE_RUN == pstHdmiDev->stThreadInfo.kThreadTimerSate)? STR_YES: STR_NO);		
    PROC_PRINT(pFile, "%-15s:", "RunStatus");
        
    if (HDMI_RUN_STATE_NONE < pstHdmiDev->enRunState)
    {
        for (i = 0; i < ARRAY_SIZE(s_eRunState2Str); i++)
        {
            if (pstHdmiDev->enRunState & (1<<i))
            {
                PROC_PRINT(pFile," %s", s_eRunState2Str[i]);
            }
        }
    }
    else
    {
        PROC_PRINT(pFile," %s", "NONE");
    }
    PROC_PRINT(pFile,"\n");

	/* line 2 */
    CHECK_MAX_VALUE(u32Index, pstHdmiDev->enTmdsMode, HDMI_TMDS_MODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  \n", "TMDSMode", s_eTmdsMode2Str[u32Index]);
    //PROC_PRINT(pFile, "%-15s: %-20s \n", "YUV420Enable", pstHdmiDev->bYUV420Enable? STR_YES: STR_NO);

	/* line 3 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "KernelCnt", pstHdmiDev->u32KernelCnt);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "UserCnt", pstHdmiDev->u32UserCnt);

	/* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "KCallBack", (HI_NULL != pstHdmiDev->pKCallback)? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "UCallBackCnt", pstHdmiDev->u32UserCallBackCnt);

	/* line 5 */
    PROC_PRINT(pFile, "%-15s:", "TransitState");
	if (HDMI_TRANSITION_NONE < pstHdmiDev->enTransitionState)
	{
		for (i = 0; i < ARRAY_SIZE(s_eTransitionSate2Str); i++)
		{
			if (pstHdmiDev->enTransitionState & (1<<i))
			{
				PROC_PRINT(pFile," %s", s_eTransitionSate2Str[i]);
			}
		}
	}
	else
	{
		PROC_PRINT(pFile, " %s", "NONE");
	}
    PROC_PRINT(pFile, "\n");

    /* line 6 */
    PROC_PRINT(pFile, "%-15s: %-20s \n", "EmiEnable", pstHdmiDev->bEmiEnable ? STR_YES: STR_NO);

    /* line 7 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "attachInTime", pstHdmiDev->stIntfStatus.u32AttachInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "attachOutTime", pstHdmiDev->stIntfStatus.u32AttachOutTime);
    
    /* line 8 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "deattachInTime", pstHdmiDev->stIntfStatus.u32DeAttachInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "deattachOutTime", pstHdmiDev->stIntfStatus.u32DeAttachOutTime);
    
    /* line 9 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "preformatInTime", pstHdmiDev->stIntfStatus.u32PreformatInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "preformatOuTime", pstHdmiDev->stIntfStatus.u32PreformatOutTime);
    
    /* line 10 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "setformatInTime", pstHdmiDev->stIntfStatus.u32SetformatInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "setformatOuTime", pstHdmiDev->stIntfStatus.u32SetformatOutTime);

    /* line 11 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "resumeInTime", pstHdmiDev->stIntfStatus.u32ResumeInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "resumeOutTime", pstHdmiDev->stIntfStatus.u32ResumeOutTime);
    
    /* line 12 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "suspendInTime", pstHdmiDev->stIntfStatus.u32SuspendInTime);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "suspendOutTime", pstHdmiDev->stIntfStatus.u32SuspendOutTime);

    return;
}

/** print CEC status
**/
#ifdef HDMI_CEC_SUPPORT
static HI_VOID Proc_CECStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HI_U32 i = 0;
    HI_BOOL bTmp = HI_FALSE;
    HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_CEC_INFO_S *pstCecInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstCecInfo = &(pstHdmiDev->stCecInfo);
    DRV_HDMI_CecStatusGet(pstHdmiDev, pstCecInfo);
//    PROC_PRINT(pFile, "-------------------------- CECtatus ----------------------------\n");

	/* line 1 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "CECEnable", pstCecInfo->bCecEnable? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "CECNetValid", pstCecInfo->bNetValid ? STR_YES: STR_NO);

	/* line 2 */
    PROC_PRINT(pFile, "%-15s: %-20x  ", "CECLogicAddr", pstHdmiDev->stCecInfo.u8LogicalAddr);
    PROC_PRINT(pFile, "%-15s:", "CECNetwork");
    bTmp = HI_FALSE;
    
	for (i = 0; i < ARRAY_SIZE(s_eNetwork2Str); i++)
	{
		if (pstHdmiDev->stCecInfo.au8Network[i])
		{
			bTmp = HI_TRUE;
			PROC_PRINT(pFile, " %s", s_eNetwork2Str[i]);
		}
	}
	if (HI_FALSE == bTmp)
	{
		PROC_PRINT(pFile, " %s", "NONE");
	}
	PROC_PRINT(pFile, "\n");

	/* line 3 */
    PROC_PRINT(pFile, "%-15s: %02x.%02x.%02x.%02x \n", "CECPhyAddr",
                            pstHdmiDev->stCecInfo.au8PhysicalAddr[0],
                            pstHdmiDev->stCecInfo.au8PhysicalAddr[1],
                            pstHdmiDev->stCecInfo.au8PhysicalAddr[2],
                            pstHdmiDev->stCecInfo.au8PhysicalAddr[3]);

    return;
}	
#endif

/** print HDCP status
**/
#ifdef HDMI_HDCP_SUPPORT
static HI_VOID Proc_HDCPStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_HDCP_INFO_S *pstHdcpInfo = HI_NULL;
    HDMI_SINK_CAPABILITY_S  *pstSinkCaps = HI_NULL;
    HI_U32 u32Index = 0;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdcpInfo = &(pstHdmiDev->stHdcpInfo);
    pstSinkCaps = &(pstHdmiDev->stEdidInfo.stCapability);

//    PROC_PRINT(pFile, "-------------------------- HDCPtatus ----------------------------\n");

    /* line 1 */ 

    PROC_PRINT(pFile, "%-15s: %-20s  ", "HDCP2.2Support", pstSinkCaps->stHdcpSupport.bHdcp22Support ? STR_YES : STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HDCP1.4Support", pstSinkCaps->stHdcpSupport.bHdcp14Support ? STR_YES : STR_NO);

    /* line 2 */
    CHECK_MAX_VALUE(u32Index, pstHdcpInfo->enHdcpMode, HDMI_HDCP_MODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HDCPMode", s_eHdcpMode2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "Hdcp1.4KeyLoad", pstHdcpInfo->bHdcp1Prep? STR_YES: STR_NO);
    
    /* line 3 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HDCPEnable", pstHdcpInfo->bHdcpIsOn? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HDCPIsOn", pstHdcpInfo->bHdcpWasOn? STR_YES: STR_NO);

    /* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HdcpAuthStatus", pstHdcpInfo->bHdcpAuthSuc? STR_YES: STR_NO);
//    PROC_PRINT(pFile, "%-20s: %-20s \n", "IsRepeater","NONE");

    /* HDCP 的认证过程需要补充 */

    return;	
}
#endif

#if ((defined HDMI_HDR_SUPPORT) && (defined HDMI_CHIP_TYPE_HI3798CV200))
static HI_VOID Proc_HDRStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HDMI_DEVICE_S           *pstHdmiDev     = HI_NULL;
    HDMI_HDR_INFO_S         *pstHdrInfo     = HI_NULL;
    HI_U32                   u32Index       = 0;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdrInfo = &(pstHdmiDev->stHdrInfo);
    CHECK_POINTER(pstHdrInfo);
    
    /* line 1 */ 
    CHECK_MAX_VALUE(u32Index, pstHdrInfo->enHdrDebugMode, HDMI_HDR_DEBUG_MODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HdrDebugMode", s_eHdrDbgMode2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "ZeroDrmSendTime", pstHdrInfo->stZeroDrmIfTimer.u32Time);

    /* line 2 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "HdrModeChnTime", pstHdrInfo->stHdrModeChnTimer.u32Time);

    PROC_PRINT(pFile, "\n");
    return;	
}
#endif

/** print HardWare status
**/
static HI_VOID Proc_HWStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HDMI_DEVICE_S           *pstHdmiDev = HI_NULL;
    HDMI_HAL_S              *pstHdmiHal	= HI_NULL;
    HDMI_HARDWARE_STATUS_S	stHwStatus;
    HDMI_COMMON_STATUS_S    *pstComStatus = HI_NULL;
    HDMI_PHY_STATUS_S       *pstPhyStatus = HI_NULL;
    HDMI_PHY_CONFIG_S       *pstPhyConfig = HI_NULL;
    HI_U32                  u32Index = 0;
    HDMI_EMI_STATUS_S       stEmiStatus;
    
#if defined (HDMI_SUPPORT_LOGIC_SILICON)
    HDMI_SCDC_STATUS_S stScdcStatus;
#endif

#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
    HDMI_VIDEO_STATUS_S     *pstVideoStat = HI_NULL;
    pstVideoStat = &stHwStatus.stVideoStatus;
#endif

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdmiHal = pstHdmiDev->pstHdmiHal;
    CHECK_POINTER(pstHdmiHal);
    pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiHal, &stHwStatus);
    pstComStatus = &(stHwStatus.stCommonStatus);
    pstPhyStatus = &(stHwStatus.stPhyStatus);
    pstPhyConfig = &(pstPhyStatus->stPhyConfig);


    PROC_PRINT(pFile, "-------------------------- HWStatus ----------------------------\n");

    /* line 1 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HotPlug", pstComStatus->bHotPlug? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "Rsen", pstComStatus->bRsen? STR_YES: STR_NO);

    /* line 2 */	
    PROC_PRINT(pFile, "%-15s: %-20s  ",	"PhyOutputEnable", pstPhyStatus->bPhyOe? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "PhyPowerEnable", pstPhyStatus->bPhyPowerOn? STR_YES: STR_NO);

    /* line 3 */
    CHECK_MAX_VALUE(u32Index, pstComStatus->enTmdsMode, HDMI_TMDS_MODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  ",	"TMDSMode", s_eTmdsMode2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "AvMute", pstComStatus->bAvMute? STR_YES: STR_NO);

    
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
    PROC_PRINT(pFile, "-------------------------- Detect Timming -----------------------\n");
    //pstHdmiHal->HAL_HDMI_HardwareStatusGet(,HDMI_HARDWARE_STATUS_S stHwStatus);
    PROC_PRINT(pFile, "%-15s: %-20s  ",  "SyncSwEnable", pstVideoStat->bSyncSwEnable ? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HsyncPolarity", pstVideoStat->bHsyncPolarity? "P" : "N");

    PROC_PRINT(pFile, "%-15s: %-20s  ",  "Progressive", pstVideoStat->bProgressive ? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "VsyncPolarity", pstVideoStat->bVsyncPolarity? "P" : "N");

    PROC_PRINT(pFile, "%-15s: %-20u  ",  "HsyncTotal", pstVideoStat->u32HsyncTotal);
    PROC_PRINT(pFile, "%-15s: %-20u \n", "HactiveCnt", pstVideoStat->u32HactiveCnt);

    PROC_PRINT(pFile, "%-15s: %-20u  ",  "VsyncTotal", pstVideoStat->u32VsyncTotal);
    PROC_PRINT(pFile, "%-15s: %-20u \n", "VactiveCnt", pstVideoStat->u32VactiveCnt);
#endif

#if defined (HDMI_SUPPORT_LOGIC_SILICON)
    pstHdmiHal->HAL_HDMI_ScdcStatusGet(pstHdmiHal, &stScdcStatus);
    /* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "SourceScramble", stScdcStatus.bSourceScrambleOn? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "SinkScramble", stScdcStatus.bSinkScrambleOn? STR_YES: STR_NO);

    /* line 5 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "TMDSBitClkRatio", stScdcStatus.u8TmdsBitClkRatio);
    PROC_PRINT(pFile, "%-15s: 0x%-20x \n", "DataSwing", pstPhyConfig->u8DataSwing);

    /* line 6 */
    PROC_PRINT(pFile, "%-15s: 0x%-20x", "CLKSwing", pstPhyConfig->u8CLKSwing);
    PROC_PRINT(pFile, "%-15s: 0x%-20x \n", "SrcTermination", pstPhyConfig->u8SrcTermination);

    /* line 7 */
    PROC_PRINT(pFile, "%-15s: 0x%-20x", "TxVnbValue", pstPhyConfig->u8TxVnbValue);
    PROC_PRINT(pFile, "%-15s: 0x%-20x \n", "AdjustClk", pstPhyConfig->u8AdjustClk);

    /* line 8 */
    PROC_PRINT(pFile, "%-15s: 0x%-20x", "TxRegZone", pstPhyConfig->u8TxRegZone);
#endif    
    pstHdmiDev->pstHdmiHal->HAL_HDMI_EmiStatusGet(pstHdmiDev->pstHdmiHal, &stEmiStatus);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "EmiEnable", stEmiStatus.bHWEmiEnable ? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s", "EmiDebugEnable", stEmiStatus.bDebugEnable ? STR_YES: STR_NO);

    PROC_PRINT(pFile, "\n");

    return;
}

static HI_VOID Proc_IFRawData(struct seq_file *pFile, HI_U32 u32Len, 
                                HI_U32 *pu32Index, HI_U32 *pu32Line, HI_U8 *paData)
{   
    CHECK_POINTER(pFile);
    CHECK_POINTER(paData);
    CHECK_POINTER(pu32Index);
    CHECK_POINTER(pu32Line);

    for (; ((*pu32Index) < u32Len) && ((*pu32Index) < HDMI_INFO_FRAME_MAX_SIZE); (*pu32Index)++)
    {
        if ((*pu32Index) >= *pu32Line)
        {
            *pu32Line += 8;
            break;
        }
        else if (0 == (*pu32Index)%4)
        {
            PROC_PRINT(pFile," ");
        }
    	PROC_PRINT(pFile,"%02x ", paData[(*pu32Index)]);
    }
    PROC_PRINT(pFile,"\n");

    return;
}

/** print video info, include videattr, aviinfo and videopath
**/
static HI_VOID Proc_VideoInfo(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HI_U32 i = 0, len = 0, u32Line = 8;
    HI_U8 *paData = HI_NULL;
    HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_HAL_S  *pstHdmiHal = HI_NULL;
    HDMI_HARDWARE_STATUS_S stHwStatus;
    HDMI_COMMON_STATUS_S *pstComStatus = HI_NULL;
    HDMI_VO_ATTR_S *pstVOAttr = HI_NULL;
    HDMI_APP_ATTR_S *pstAppAttr = HI_NULL;
    HDMI_AVIIF_BYTE1_U	unAviByte1;
    HDMI_AVIIF_BYTE2_U	unAviByte2;
    HDMI_AVIIF_BYTE3_U	unAviByte3;
    HDMI_AVIIF_BYTE5_U	unAviByte5;
    HI_U32 u32Index = 0;
    HI_U32 u32VideoCodeIndex = 0;
    HI_CHAR *pTmpFmt = HI_NULL;
    HI_CHAR *pVSIFTmpFmt = HI_NULL;

    HDMI_VIDEO_CODE_E  enHdmiVic = 0;
    enum hdmi_video_format format = 0;
     
    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdmiHal = pstHdmiDev->pstHdmiHal;
    pstVOAttr  = &(pstHdmiDev->stAttr.stVOAttr);
    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);
    CHECK_POINTER(pstHdmiHal);
    pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiHal, &stHwStatus);
    pstComStatus = &stHwStatus.stCommonStatus;

    /* Hardware Video/Audio Path */
    unAviByte1.u8 = stHwStatus.stInfoFrameStatus.u8AVI[4];
    unAviByte2.u8 = stHwStatus.stInfoFrameStatus.u8AVI[5];
    unAviByte3.u8 = stHwStatus.stInfoFrameStatus.u8AVI[6];
    unAviByte5.u8 = stHwStatus.stInfoFrameStatus.u8AVI[8];

    PROC_PRINT(pFile, "------------ VideoAttr --------------------------- AVIIfno --------------\n");

    /* line 1 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enVideoTiming, HDMI_VIDEO_TIMING_BUTT);

    // find vic according to Voattr
    u32VideoCodeIndex = DRV_HDMI_VicSearch(u32Index, pstVOAttr->enPictureAspect, HI_FALSE);   // search vic
    PROC_PRINT(pFile, "%-15s: %-20s |", "VideoTiming", s_stCEA861FVideoCodesDes[u32VideoCodeIndex].pu8FmtStr);  
    PROC_PRINT(pFile, "%-15s: %-20s \n", "AVIInfoEnable", stHwStatus.stInfoFrameStatus.bAVIEnable? STR_YES: STR_NO);

    /* line 2 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->u32DispFmt, ARRAY_SIZE(s_eDispFMT2Str) - 1);
    PROC_PRINT(pFile, "%-15s: %-20s |", "DispFmt", s_eDispFMT2Str[u32Index]);
    // modified by q00352704 @2015.08.10
    // to modify the proc info when the video format is 4k x 2k
    // find vic according to InfoFrame
    // 4k x 2k
    format = (stHwStatus.stInfoFrameStatus.u8VSIF[7] >> 5) & 0x3;
    if (format == HDMI_VIDEO_FORMAT_4K)
    {
        enHdmiVic   = stHwStatus.stInfoFrameStatus.u8VSIF[8];    // HDMI_VIC
        if(enHdmiVic >= 1 && enHdmiVic <= ARRAY_SIZE(s_stH14bHdmiVideoCodes_4K))
        {
            pVSIFTmpFmt = s_stH14bHdmiVideoCodes_4K[enHdmiVic - 1].pu8FmtStr; // s_stH14bHdmiVideoCodes_4K index = enHdmiVic - 1
        }
    }
    else if(format == HDMI_VIDEO_FORMAT_3D)
    {
        // TODO: 3D Format
    }
    else
    {
        // normal 
        pTmpFmt = s_stCEA861FVideoCodesDes[stHwStatus.stInfoFrameStatus.u8AVI[7]].pu8FmtStr;
    }
    PROC_PRINT(pFile, "%-15s: %s(VIC=%2d)\n", "CurrentFormat", pTmpFmt==HI_NULL? "": pTmpFmt,stHwStatus.stInfoFrameStatus.u8AVI[7]);   

    /* line 3 */
    PROC_PRINT(pFile, "%-15s: %-20d |", "PixelClk", pstVOAttr->u32ClkFs);
    PROC_PRINT(pFile, "%-15s: %s(HDMI_VIC=%2d)\n", "VSIFormat", pVSIFTmpFmt==HI_NULL? "": pVSIFTmpFmt, format==HDMI_VIDEO_FORMAT_4K? stHwStatus.stInfoFrameStatus.u8VSIF[8]:0);   

    /* line 4 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enInBitDepth, HDMI_VIDEO_BITDEPTH_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "InBitDepth", s_eBitDepth2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "BarDataPresent", s_BarDataPresent2Str[unAviByte1.bits.bzBarDataPresent]);

    /* line 5 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enInColorSpace, HDMI_COLORSPACE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "InColorSpace", s_eColorSpace2Str[u32Index]);
    u32Index   = unAviByte1.bits.bzColorSpace & 0x3;
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ColorSpace", s_eColorSpace2Str[u32Index]);

    /* line 6 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enColorimetry, HDMI_COLORIMETRY__EXTENDED);
    PROC_PRINT(pFile, "%-15s: %-20s |", "Colorimetry", s_eColorimetry2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "Colorimetry", s_eColorimetry2Str[unAviByte2.bits.bzColorimetry]);

    /* line 7 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enPictureAspect, HDMI_PICTURE_ASPECT_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "PicAspectRatio", s_ePicAspectRatio2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "PicAspectRatio", s_ePicAspectRatio2Str[unAviByte2.bits.bzPictureAspectRatio]);

    /* line 8 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enActiveAspect, HDMI_ACTIVE_ASPECT_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "ActAspectRatio", s_eActAspectRatio2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ActAspectRatio", s_eActAspectRatio2Str[unAviByte2.bits.bzActAspRatio]);

    /* line 9 */
    PROC_PRINT(pFile, "%-15s: %-20d |", "PixelRepeat", pstVOAttr->u32PixelRepeat);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "PixelRepeat", s_PixelRepetition2Str[unAviByte5.bits.bzPixelRepetition]);

    /* line 10 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enYCCQuantization, HDMI_YCC_QUANTIZATION_RANGE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "YCCQuantization", s_eYCCQuantization2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "YCCQuantization", s_eYCCQuantization2Str[unAviByte5.bits.bzYCCQuantizationRange]);

    /* line 11 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enRGBQuantization, HDMI_QUANTIZATION_RANGE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "RGBQuantization", s_eRgbQuantization2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "RGBQuantization", s_eRgbQuantization2Str[unAviByte3.bits.bzRgbQuantization]);

    /* line 12 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enExtendedColorimetry, HDMI_EXTENDED_COLORIMETRY_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "ExtColorimetry", s_eExtColorimetry2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ExtColorimetry", s_eExtColorimetry2Str[unAviByte3.bits.bzExtColorimetry]);

    /* line 13 */
    CHECK_MAX_VALUE(u32Index, pstVOAttr->enStereoMode, HDMI_3D_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "StereoMode", s_eStereMode2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ItContentValid", unAviByte3.bits.bzItContentValid? STR_YES: STR_NO);

    /* line 14 */
    PROC_PRINT(pFile, "%-15s: %-20d |", "bVSyncPol", pstVOAttr->bVSyncPol);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "bHSyncPol", pstVOAttr->bHSyncPol);

    /* line 15 */
    PROC_PRINT(pFile, "------------ VedioPath ---------------|");
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ITContentType", s_ITContentType2Str[unAviByte5.bits.bzITContentType]);

    /* line 16 */
    PROC_PRINT(pFile,"%-15s: %-20s |",	"VideoMute", stHwStatus.stVideoStatus.bVideoMute? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "PicScaling", s_PictureScaling2Str[unAviByte3.bits.bzPictureScaling]);

    /* line 17 */
    CHECK_MAX_VALUE(u32Index, stHwStatus.stVideoStatus.enOutBitDepth, HDMI_VIDEO_BITDEPTH_BUTT);
    PROC_PRINT(pFile,"%-15s: %-20s |",	"OutBitDepth", s_eBitDepth2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ActFmtPresent", unAviByte1.bits.bzActFmtInfoPresent? STR_YES: STR_NO);

    /* line 18 */
    CHECK_MAX_VALUE(u32Index, stHwStatus.stVideoStatus.enOutColorSpace, HDMI_COLORSPACE_BUTT);
    PROC_PRINT(pFile,"%-15s: %-20s |",	"OutColorSpace", s_eColorSpace2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ScanInfo", s_ScanInfo2Str[unAviByte1.bits.bzScanInfo]);

    /* line 19 */
    PROC_PRINT(pFile,"%-15s: %-20s |",  "YCbCr420_422", stHwStatus.stVideoStatus.bYCbCr420_422? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s:\n", "AVIInfoRawData");

    /* line 20 */
    PROC_PRINT(pFile,"%-15s: %-20s |",  "YCbCr422_444", stHwStatus.stVideoStatus.bYCbCr422_444? STR_YES: STR_NO);
    paData = stHwStatus.stInfoFrameStatus.u8AVI;
    len = paData[2] + 4;

    i = 0;
    Proc_IFRawData(pFile, len, &i, &u32Line, paData);

    /* line 21 */
    PROC_PRINT(pFile,"%-15s: %-20s |",  "YCbCr444_422", stHwStatus.stVideoStatus.bYCbCr444_422? STR_YES: STR_NO);
    Proc_IFRawData(pFile, len, &i, &u32Line, paData);

    /* line 22*/
    PROC_PRINT(pFile,"%-15s: %-20s |",  "YCbCr422_420", stHwStatus.stVideoStatus.bYCbCr422_420? STR_YES: STR_NO);
    Proc_IFRawData(pFile, len, &i, &u32Line, paData);

    /* line 23 */
    PROC_PRINT(pFile,"%-15s: %-20s |",  "RGB2YCbCr", stHwStatus.stVideoStatus.bRGB2YCbCr? STR_YES: STR_NO);
    PROC_PRINT(pFile,"%-15s:\n", "VSInfoRawData");

    /* line 24 */
    PROC_PRINT(pFile,"%-15s: %-20s |",  "YCbCr2RGB", stHwStatus.stVideoStatus.bYCbCr2RGB? STR_YES: STR_NO);
    paData  = stHwStatus.stInfoFrameStatus.u8VSIF;
    u32Line = 8;
    len     = paData[2] + 4;
    i       = 0;
    Proc_IFRawData(pFile, len, &i, &u32Line, paData);

    	/* line 25 */
    PROC_PRINT(pFile,"%-15s: %-20s |", "Dither",stHwStatus.stVideoStatus.bDither? STR_YES: STR_NO);
    Proc_IFRawData(pFile, len, &i, &u32Line, paData);

    return;
}

/** print HDR info, include hdrattr, drminfo
**/
#ifdef HDMI_HDR_SUPPORT
static HI_VOID Proc_HDRInfo(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HI_U8            i          = 0;
    HI_U32           u32Line    = 0;
    HI_U32           len        = 0;
    HI_U8           *paData     = HI_NULL;
    HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
    HDMI_HDR_ATTR_S *pstHdrAttr = HI_NULL;
    HDMI_HARDWARE_STATUS_S stHwStatus;
    HDMI_HAL_S      *pstHdmiHal = HI_NULL;
    HI_U32          u32Index    = 0;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdrAttr = &pstHdmiDev->stAttr.stHdrAttr;

    pstHdmiHal = pstHdmiDev->pstHdmiHal;
    CHECK_POINTER(pstHdmiHal);

    pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiHal, &stHwStatus);
 
    PROC_PRINT(pFile, "--------------------------------- HDRAttr -------------------------------\n");  
    /* line 1 */
    CHECK_MAX_VALUE(u32Index, pstHdrAttr->enUserHdrMode, HDMI_HDR_USERMODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "UserHdrMode", s_eHdrUserMode2Str[u32Index]);
    
    /* line 2 */
    CHECK_MAX_VALUE(u32Index, pstHdrAttr->enHdrMode, HDMI_HDR_MODE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s  ", "HdrMode", s_eHdrMode2Str[u32Index]);
    CHECK_MAX_VALUE(u32Index, pstHdrAttr->enEotfType, HDMI_EOTF_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HdrEotfType", s_eHdrEotfType2Str[u32Index]);

    /* line 3 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "HdrMetaDataID", pstHdrAttr->enMetadataId);  
    CHECK_MAX_VALUE(u32Index, pstHdrAttr->enColorimetry, HDMI_HDR_EXTENDED_COLORIMETRY_2020_non_const_luminous);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "HdrColorimetry", s_eHdrColorimetry[u32Index]);

    /* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "DispPrim0_X", pstHdrAttr->unDescriptor.stType1.u16Primaries0_X);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "DispPrim0_Y", pstHdrAttr->unDescriptor.stType1.u16Primaries0_Y);

    /* line 5 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "DispPrim1_X", pstHdrAttr->unDescriptor.stType1.u16Primaries1_X);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "DispPrim1_Y", pstHdrAttr->unDescriptor.stType1.u16Primaries1_Y);

    /* line 6 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "DispPrim2_X", pstHdrAttr->unDescriptor.stType1.u16Primaries2_X);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "DispPrim2_Y", pstHdrAttr->unDescriptor.stType1.u16Primaries2_Y);

    /* line 7 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "WhitePoint_X", pstHdrAttr->unDescriptor.stType1.u16WhitePoint_X);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "WhitePoint_Y", pstHdrAttr->unDescriptor.stType1.u16WhitePoint_Y);
    
    /* line 8 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "MaxLuminance", pstHdrAttr->unDescriptor.stType1.u16MaxLuminance);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "MinLuminance", pstHdrAttr->unDescriptor.stType1.u16MinLuminance);

    /* line 9 */
    PROC_PRINT(pFile, "%-15s: %-20d  ", "MaxLightLevel", pstHdrAttr->unDescriptor.stType1.u16MaxLightLevel);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "AverLightLevel", pstHdrAttr->unDescriptor.stType1.u16AverageLightLevel);
    
    PROC_PRINT(pFile, "--------------------------------- DRMInfo -------------------------------\n");  

    /* line 10 */
    PROC_PRINT(pFile, "%-15s: %-20s  ", "DRMInfoEnable", stHwStatus.stInfoFrameStatus.bDRMEnable? STR_YES: STR_NO);
    CHECK_MAX_VALUE(u32Index, stHwStatus.stInfoFrameStatus.u8DRM[4], HDMI_EOTF_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "EotfType", s_eHdrEotfType2Str[u32Index]);
    
    /* line 11 */
    PROC_PRINT(pFile, "%-15s: %-20d \n", "MetadataID", stHwStatus.stInfoFrameStatus.u8DRM[5]);

    /* line 12 */
    PROC_PRINT(pFile, "%-15s: ", "DRMInfoRawData");
    paData  = stHwStatus.stInfoFrameStatus.u8DRM;
    u32Line = 16;
    len     = paData[2] + 4;
	for (i = 0; (i < len) && (i < HDMI_INFO_FRAME_MAX_SIZE); i++)
	{
        if (0 == i%4 && i != 0)
        {
            PROC_PRINT(pFile," ");
        }
		PROC_PRINT(pFile,"%02x ", paData[i]);
        if(i % 15 == 0 && i != 0)
        {
            PROC_PRINT(pFile,"\n                ");
        }
	}
	PROC_PRINT(pFile, "\n");
}
#endif

/** print audio info, include videattr, aviinfo and videopath
**/
static HI_VOID Proc_AudioInfo(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
    HI_U32 i = 0, len = 0;
    HI_U8 *paData = HI_NULL;
    HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_HAL_S  *pstHdmiHal = HI_NULL;
    HDMI_HARDWARE_STATUS_S  stHwStatus;
    HDMI_AO_ATTR_S *pstAOAttr = HI_NULL;
    HDMI_APP_ATTR_S *pstAppAttr = HI_NULL;

    HDMI_AUDIO_IF_BYTE1_U unAudioByte1;
    HDMI_AUDIO_IF_BYTE2_U unAudioByte2;
    HDMI_AUDIO_IF_BYTE5_U unAudioByte5;
    HI_U32 u32Index = 0;
    
    pstHdmiDev = GetHdmiDevice(enHdmiID);
    CHECK_POINTER(pstHdmiDev);
    pstHdmiHal = pstHdmiDev->pstHdmiHal;
    pstAOAttr  = &(pstHdmiDev->stAttr.stAOAttr);
    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);
    CHECK_POINTER(pstHdmiHal);
    pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiHal, &stHwStatus);

    unAudioByte1.u8 = stHwStatus.stInfoFrameStatus.u8AUDIO[4];
    unAudioByte2.u8 = stHwStatus.stInfoFrameStatus.u8AUDIO[5];
    unAudioByte5.u8 = stHwStatus.stInfoFrameStatus.u8AUDIO[8];

	PROC_PRINT(pFile, "------------ AudioAttr --------------------------- AudioIfno ------------\n");

	/* line 1 */
    CHECK_MAX_VALUE(u32Index, pstAOAttr->enSoundIntf, HDMI_AUDIO_INTERFACE__BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "SoundIntf", s_SoundIntf2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "AudioInfoEnable", stHwStatus.stInfoFrameStatus.bAUDIOEnable? STR_YES: STR_NO);
    
	/* line 2 */
    CHECK_MAX_VALUE(u32Index, pstAOAttr->enAudioCode, HDMI_AUDIO_CODING_TYPE_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "CodeType", s_AudioCodeType2Str[u32Index]);
	PROC_PRINT(pFile, "%-15s: %-20s \n", "CodeType", s_AudioCodeType2Str[unAudioByte1.bits.bzAduioCodingType]);

	/* line 3 */
    CHECK_MAX_VALUE(u32Index, pstAOAttr->enChanels, HDMI_AUDIO_FORMAT_BUTT);
    PROC_PRINT(pFile, "%-15s: %-20s |", "ChannelCnt", s_eAudioChannel2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "ChannelCnt", s_AudioChannel2Str[unAudioByte1.bits.bzAudioChanelCnt]);

	/* line 4 */
    PROC_PRINT(pFile, "%-15s: %-20d |", "SampleFreq", pstAOAttr->enSampleFs);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "SampleFreq", s_AudioSampingFreq2Str[unAudioByte2.bits.bzAudioSampingFreq]);

	/* line 5 */
    PROC_PRINT(pFile, "%-15s: %-20d |", "SampleDepth", pstAOAttr->enSampleDepth);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "SampleDepth", stHwStatus.stAudioStatus.enSampleDepth);

	/* line 6 */
    PROC_PRINT(pFile, "%-15s: %-20s |", "DownSample", pstAOAttr->bDownSample? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "SampleSize", s_AudioSampingSize2Str[unAudioByte2.bits.bzAudioSampingSize]);

	/* line 7 */
	PROC_PRINT(pFile, "------------ AudioPath ---------------|");
    PROC_PRINT(pFile, "%-15s: %-20s \n", "DownMixInhibit", unAudioByte5.bits.bzDownMixInhibit? STR_YES: STR_NO);

	/* line 8 */
	PROC_PRINT(pFile, "%-15s: %-20s |", "AudioEnable", stHwStatus.stInfoFrameStatus.bAUDIOEnable? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20d \n", "LevelShiftValue", unAudioByte5.bits.bzLevelShiftValue);

	/* line 9 */
	PROC_PRINT(pFile, "%-15s: %-20s |", "AudioMute", stHwStatus.stAudioStatus.bAudioMute? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-15s: %-20s \n", "LFEPlayBack", s_LFEPlayBack2Str[unAudioByte5.bits.bzLFE_PlayBack]);

	/* line 10 */
    CHECK_MAX_VALUE(u32Index, stHwStatus.stAudioStatus.enSoundIntf, HDMI_AUDIO_INTERFACE__BUTT);
	PROC_PRINT(pFile, "%-15s: %-20s |", "SoundIntf", s_SoundIntf2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: 0x%02x (%d)\n", "Channel/SpeakerAlloc", stHwStatus.stInfoFrameStatus.u8AUDIO[7], 
                                                                       stHwStatus.stInfoFrameStatus.u8AUDIO[7]);

	/* line 11 */
    CHECK_MAX_VALUE(u32Index, stHwStatus.stAudioStatus.enLayout, HDMI_AUDIO_FORMAT_BUTT);
	PROC_PRINT(pFile, "%-15s: %-20s |", "ChannelCnt", s_eAudioChannel2Str[u32Index]);
    PROC_PRINT(pFile, "%-15s: \n", "AudioInfoRawData") ;

    /* line 12 */
	PROC_PRINT(pFile, "%-15s: %-20d |", "SampleFreq", stHwStatus.stAudioStatus.enSampleFs);
	paData = stHwStatus.stInfoFrameStatus.u8AUDIO;
    len = paData[2] + 4;
	for (i = 0; (i < len) && (i <HDMI_INFO_FRAME_MAX_SIZE); i++)
	{
        if (i >= 8)
        {
            break;
        }
        else if (0 == i%4)
        {
            PROC_PRINT(pFile," ");
        }
		PROC_PRINT(pFile,"%02x ", paData[i]);
	}
	PROC_PRINT(pFile, "\n");

	/* line 13 */
	PROC_PRINT(pFile, "%-15s: %-20d |", "SampleDepth", stHwStatus.stAudioStatus.enSampleDepth);
    len = paData[2] + 4;
	for (; (i < len) && (i <HDMI_INFO_FRAME_MAX_SIZE); i++)
	{
        if (i >= 16)
        {
            break;
        }
        else if (0 == i%4)
        {
            PROC_PRINT(pFile," ");
        }
		PROC_PRINT(pFile,"%02x ", paData[i]);
	}
	PROC_PRINT(pFile, "\n");

	/* line 14 */
	PROC_PRINT(pFile, "%-15s: %-20s |", "DownSample", stHwStatus.stAudioStatus.bDownSample? STR_YES: STR_NO);
	PROC_PRINT(pFile, "\n");

#if defined (HDMI_SUPPORT_LOGIC_SILICON)

	/* line 15 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "CTS", stHwStatus.stAudioStatus.u32RegCtsvalue);
	PROC_PRINT(pFile, "\n");

	/* line 16 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Ref_CTS", stHwStatus.stAudioStatus.u32RefCtsvalue);
	PROC_PRINT(pFile, "\n");

	/* line 17 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "N",  stHwStatus.stAudioStatus.u32RegNvalue);
	PROC_PRINT(pFile, "\n");

	/* line 18 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Ref_N",  stHwStatus.stAudioStatus.u32RefNvalue);
	PROC_PRINT(pFile, "\n");

	/* line 19 */
	PROC_PRINT(pFile, "%-15s: %-20s |", "AudioFIFO", STR_NA);
    PROC_PRINT(pFile, "\n");
#endif
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
	/* line 15 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Ref_CTS", stHwStatus.stAudioStatus.u32RefCtsvalue);
	PROC_PRINT(pFile, "\n");

	/* line 16 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Reg_CTS", stHwStatus.stAudioStatus.u32RegCtsvalue);
	PROC_PRINT(pFile, "\n");

	/* line 17 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Ref_N",  stHwStatus.stAudioStatus.u32RefNvalue);
	PROC_PRINT(pFile, "\n");

	/* line 18 */
	PROC_PRINT(pFile, "%-15s: %-20u |", "Reg_N",  stHwStatus.stAudioStatus.u32RegNvalue);
	PROC_PRINT(pFile, "\n");
#endif
    return;
}

/** print event information
**/
static HI_VOID Proc_EventInfo(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
	HI_U32 i=0,j=0;
	HI_U32 u32TotalPool = 0;
 	HI_S32 s32Ret = HI_SUCCESS;
	HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
	HDMI_EVENT_INFO_S *pstEvtInfo = HI_NULL;
	HDMI_EVENT_RUN_CNT_S stRunCnt;
	HDMI_EVENT_RUN_CTRL_S stCtrl;
	
    pstHdmiDev = GetHdmiDevice(enHdmiID);
	CHECK_POINTER(pstHdmiDev);
	pstEvtInfo = &pstHdmiDev->stEventInfo;
	CHECK_POINTER(pstEvtInfo);

	s32Ret = DRV_HDMI_EventPoolTotalGet(pstEvtInfo,&u32TotalPool);
	CHECK_RET_SUCCESS(s32Ret);

	for (i = 0; i < u32TotalPool; i++)
	{
		if(DRV_HDMI_EventPoolStatusGet(pstEvtInfo,i+1,&stCtrl,&stRunCnt)==HI_SUCCESS)
		{
            PROC_PRINT(pFile,"----------------- TaskID=%u Event Pool[%u] Status ----------------------\n",stCtrl.u32PoolID,i);
			/* line1 */
			PROC_PRINT(pFile,"CNT|%-8s|%-3s|%-5s|%-8s|%-8s|%-8s|%-7s|%-7s|%-7s\n",
												"ErrTotal",
												"HPD",
												"UnHPD",
												"EdidFail",
												"HdcpFail",
												"HdcpSucc",
												"RsenCon",
												"RsenDis",
												"HdcpUsr");
			/* line2 */
			PROC_PRINT(pFile,"WR:|%-8u|%-3u|%-5u|%-8u|%-8u|%-8u|%-7u|%-7u|%-7u\n",
												stRunCnt.u32ErrWdCnt,
												stRunCnt.u32HPDWrCnt,	
												stRunCnt.u32UnHPDWrCnt,	
												stRunCnt.u32EdidFailWrCnt,
												stRunCnt.u32HdcpFailWrCnt,
												stRunCnt.u32HdcpSuccWrCnt,
												stRunCnt.u32RsenConWrCnt,
												stRunCnt.u32RsenDisWrCnt,
												stRunCnt.u32HdcpUserWrCnt);
			/* line3 */
			PROC_PRINT(pFile,"RD:|%-8u|%-3u|%-5u|%-8u|%-8u|%-8u|%-7u|%-7u|%-7u\n",
												stRunCnt.u32ErrRdCnt,
												stRunCnt.u32HPDRdCnt,		
												stRunCnt.u32UnHPDRdCnt,
												stRunCnt.u32EdidFailRdCnt,
												stRunCnt.u32HdcpFailRdCnt,
												stRunCnt.u32HdcpSuccRdCnt,
												stRunCnt.u32RsenConRdCnt,
												stRunCnt.u32RsenDisRdCnt,
												stRunCnt.u32HdcpUserRdCnt);
			/* line4 */
			PROC_PRINT(pFile,"Memory[WkFlg=%1d |RdAble=%2d| RdPtr=%-2d| WrPtr=%-2d]:",
												stCtrl.bWakeUpFlag,
												stCtrl.u32RdAbleCnt,
												stCtrl.u32ReadPtr,
												stCtrl.u32WritePtr);
			for (j = 0; j < ARRAY_SIZE(stCtrl.enEventPool); j++)
			{
                if (HDMI_EVENT_BUTT > stCtrl.enEventPool[j])
                {
                    PROC_PRINT(pFile," 0x%02x", stCtrl.enEventPool[j]);
                }
			}
			PROC_PRINT(pFile,"\n");
		}
	}

	return ;
}

/** print the edid raw data
**/
static HI_VOID Proc_EdidRawData(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
	HI_U8 *pu8EdidRaw = HI_NULL;
	HI_U32 u32RawLen = 0, i = 0;
	HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
	HDMI_EDID_INFO_S *pstEdidInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
	CHECK_POINTER(pstHdmiDev);

	pstEdidInfo = &pstHdmiDev->stEdidInfo;
	pu8EdidRaw = (HI_U8 *)HDMI_KMALLOC(HDMI_EDID_TOTAL_SIZE);
	CHECK_POINTER(pu8EdidRaw);

    u32RawLen = HDMI_EDID_TOTAL_SIZE;
    u32RawLen = DRV_HDMI_EdidRawGet(pstEdidInfo, pu8EdidRaw, u32RawLen);
    
    /* Raw data */
    PROC_PRINT(pFile, "--------------------------------------- EDIDRawData -----------------------------------------\n");
    if(0xffffffff != u32RawLen)
    {
        for (i = 0; i < u32RawLen && i < HDMI_EDID_TOTAL_SIZE; i++)
        {
            if (0 == i)
            {
                PROC_PRINT(pFile, "/*%02xH:*/ ", i);
            }
    
            PROC_PRINT(pFile, "0x%02x,", pu8EdidRaw[i]);
    
            if ((0 == (i+1) % 4) && ((i+1) < u32RawLen))
            {
                PROC_PRINT(pFile,"  ");
            }
    
            if ((0 == (i + 1) % 16) && ((i+1) < u32RawLen))
            {
                PROC_PRINT(pFile, "\n/*%02xH:*/ ", i);
            }
        }
        PROC_PRINT(pFile, "\n");
    }

    HDMI_KFREE(pu8EdidRaw);
    pu8EdidRaw = HI_NULL;

    return;
}

/** print the edid software status
**/
static HI_VOID Proc_EdidStatus(struct seq_file *pFile, HDMI_DEVICE_ID_E enHdmiID)
{
	HI_S32 s32Ret;
	HDMI_EDID_STATUS_S stStatus;
	HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
	HDMI_EDID_INFO_S *pstEdidInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
	CHECK_POINTER(pstHdmiDev);

	pstEdidInfo = &pstHdmiDev->stEdidInfo;
    OSAL_MEMSET(&stStatus, 0x0, sizeof(HDMI_EDID_STATUS_S));

    s32Ret = DRV_HDMI_EdidStatusGet(pstEdidInfo, &stStatus);
    if (HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("call DRV_HDMI_EdidStatusGet failed!!!\n");
        return;
    }
 
    PROC_PRINT(pFile, "------------------------------ SWStatus -------------------------------------------\n");

    /* line 1 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "CapFromSink", stStatus.bCapSink? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %-20d \n", "RawUpdateErrCnt", stStatus.u32RawUpdateErrCnt);    

    /* line 2 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "CapIsValid", stStatus.bCapValid? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %-20d \n", "ParseErrorType", stStatus.enParseErr);

    /* line 3 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "RawIsValid", stStatus.bRawValid? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: 0x%08x\n","ParseWarnType", stStatus.u32ParseWarn);

    /* line 4 */
    PROC_PRINT(pFile, "%-20s: %-20d ", "RawGetErrCnt", stStatus.u32RawGetErrCnt);
    PROC_PRINT(pFile, "%-20s: %-20d \n",  "RawLength", stStatus.u32RawLen);

    return;
}

/** get capability from edid of sink
**/
static HI_S32 Proc_GetCapInfo(HDMI_DEVICE_ID_E enHdmiID, HDMI_SINK_CAPABILITY_S **pstCap)
{
    HI_S32 s32Ret;
    HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
    HDMI_EDID_INFO_S *pstEdidInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiID);
    if (HI_NULL == pstHdmiDev)
    {
        HDMI_ERR("get pstHdmiDev=null failed!\n");
        return HI_FAILURE;
    }
    pstEdidInfo = &pstHdmiDev->stEdidInfo;

    s32Ret = DRV_HDMI_EdidCapabilityGet(pstEdidInfo, pstCap);
    if (HDMI_EDID_DATA_INVALID == s32Ret)
    {
        HDMI_ERR("Current capability invalid, force to get Raw!!!\n");
        s32Ret = DRV_HDMI_EdidUpdate(pstEdidInfo, HDMI_EDID_UPDATE_SINK);
        if (HI_SUCCESS != s32Ret)
        {
            HDMI_ERR("Please make sure HDMI cable connected & start!\n");
            return HI_FAILURE;
        }
        
        s32Ret = DRV_HDMI_EdidCapabilityGet(pstEdidInfo, pstCap);
        if (HDMI_EDID_DATA_INVALID == s32Ret)
        {
            HDMI_ERR("Get the edid capability error, s32Ret=%d, pstCap=%p!\n", s32Ret, pstCap);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


/** print the basic capability of sink
**/
static HI_VOID Proc_SinkBasicCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0;
	HI_CHAR StrTmpArray[25];

    CHECK_POINTER(pstCap);
    /* basic */
    PROC_PRINT(pFile, "------------------------------ BasicCap -------------------------------------------\n");

    /* line 1 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "HDMI1.4Support", pstCap->bSupportHdmi? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %-1d.%-1d\n","1stBlockVersion", pstCap->u8Version, pstCap->u8Revision);

    /* line 2 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "HDMI2.0Support", pstCap->bSupportHdmi_2_0? STR_YES: STR_NO);
    OSAL_MEMSET(StrTmpArray, '\0', sizeof(StrTmpArray));
    HDMI_OSAL_Strncpy(StrTmpArray, pstCap->stMfrsInfo.u8MfrsName, 4);
    PROC_PRINT(pFile, "%-20s: %-20s \n", "ManufacturerName", StrTmpArray);

    /* line 3 */
    PROC_PRINT(pFile, "%-20s: %-20d ", "MaxTMDSClock(MHz)", pstCap->u32MaxTMDSClock);
    PROC_PRINT(pFile, "%-20s: %-20u \n", "ProductCode", pstCap->stMfrsInfo.u32ProductCode);

    /* line 4 */
    PROC_PRINT(pFile, "%-20s: %-20u ", "SerialNumber", pstCap->stMfrsInfo.u32SerialNumber);
    PROC_PRINT(pFile, "%-20s: %-20u \n", "WeekOfManufacture", pstCap->stMfrsInfo.u32Week);

    /* line 5 */
    PROC_PRINT(pFile, "%-20s: %-20u ", "MaxDispWidth", pstCap->stBasicDispPara.u8MaxImageWidth);
    PROC_PRINT(pFile, "%-20s: %-20u \n", "MaxDispHeight", pstCap->stBasicDispPara.u8MaxImageHeight);

    /* line 6 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "SCDCSupport", pstCap->bSupportSCDC? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %-20u \n", "YearOfManufacture", pstCap->stMfrsInfo.u32Year);

    /* line 7 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "DVIDualSupport", pstCap->bSupportDVIDual? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %-20s \n", "CECAddrIsValid", pstCap->stCECAddr.bPhyAddrValid? STR_YES: STR_NO);

    /* line 8 */
    PROC_PRINT(pFile, "%-20s: %-20s ", "AISupport", pstCap->bSupportsAI? STR_YES: STR_NO);
    PROC_PRINT(pFile, "%-20s: %02x.%02x.%02x.%02x\n", "CECAddr", pstCap->stCECAddr.u8PhyAddrA, 
               pstCap->stCECAddr.u8PhyAddrB, pstCap->stCECAddr.u8PhyAddrC, pstCap->stCECAddr.u8PhyAddrD);

    /* line 9 */
    PROC_PRINT(pFile, "%-20s: %-20u ", "ExtBlockCnt", pstCap->u8ExtBlockNum);
    /* speaker */
    PROC_PRINT(pFile, "%-20s:", "SpeakerSupport");
    for (i = 0; i < ARRAY_SIZE(pstCap->bSupportAudioSpeaker) && i < ARRAY_SIZE(s_aStr_Speaker);i++)
    {
        PROC_PRINT(pFile, " %s",pstCap->bSupportAudioSpeaker[i]? s_aStr_Speaker[i]: "");
    }
    PROC_PRINT(pFile,"\n");

    return;
}

/** print the video capability of sink
**/
static HI_VOID Proc_SinkVideoCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0;
    HI_CHAR *pTmpFmt = HI_NULL;

    CHECK_POINTER(pstCap);
    pTmpFmt = Proc_Vic2Format(pstCap->u32NativeFormat);
    CHECK_POINTER(pTmpFmt);

    PROC_PRINT(pFile ,"------------------------------ VidoCap --------------------------------------------\n");

    /* line 1 */
    PROC_PRINT(pFile, "%-20s: %-s(VIC %2d)\n", "NativeFormat", (pTmpFmt==HI_NULL? "": pTmpFmt), pstCap->u32NativeFormat);  

    /* line 2 ColorSpace */
    PROC_PRINT(pFile, "%-20s:", "ColorSpace");
    if (pstCap->stColorSpace.bRGB444)
    {
        PROC_PRINT(pFile, " %-s", "RGB444");  
    }
    if (pstCap->stColorSpace.bYCbCr444)
    {
        PROC_PRINT(pFile, " %-s", "YCbCr444");    
    }
    if (pstCap->stColorSpace.bYCbCr422)
    {
        PROC_PRINT(pFile, " %-s", "YCbCr422");    
    }
    if (pstCap->u32OnlySupportY420VICNum || pstCap->u32SupportY420VICNum)
    {
        PROC_PRINT(pFile, " %-s", "YCbCr420");    
    }
    PROC_PRINT(pFile, "\n");

    /* line 3 DeepColor */
    PROC_PRINT(pFile,"%-20s:",  "DeepColor");
    if (pstCap->stDeepColor.bDeepColor30Bit)
    {
        PROC_PRINT(pFile, " %-s", "RGB_30Bit"); 
    }   
    if (pstCap->stDeepColor.bDeepColor36Bit)
    {
        PROC_PRINT(pFile, " %-s", "RGB_36Bit");   
    }
    if (pstCap->stDeepColor.bDeepColor48Bit)
    {
        PROC_PRINT(pFile, " %-s", "RGB_48Bit");   
    }
    if (pstCap->stDeepColor.bDeepColorY444)
    {
        PROC_PRINT(pFile, " %-s", "YCbCr444_SameRGB");    
    }   
    PROC_PRINT(pFile, "\n");

    /* line 4 DeepColorY420 */
    PROC_PRINT(pFile, "%-20s:",  "YCbCr420DeepColor");
    if (pstCap->stDeepColorY420.bDeepColor30Bit)
    {
        PROC_PRINT(pFile, " %-s", "30Bit"); 
    }   
    if (pstCap->stDeepColorY420.bDeepColor36Bit)
    {
        PROC_PRINT(pFile, " %-s", "36Bit");   
    }
    if (pstCap->stDeepColorY420.bDeepColor48Bit)
    {
        PROC_PRINT(pFile, " %-s", "48Bit");   
    }
    PROC_PRINT(pFile, "\n");

    /* line 5 Y420Format[Also] */
    PROC_PRINT(pFile, "%-20s:", "YCbCr420[Also]");
    for (i = 0; i < pstCap->u32SupportY420VICNum; i++)
    {
        PROC_PRINT(pFile, " %u",pstCap->au32SupportY420Format[i]);
    }
    PROC_PRINT(pFile, "\n");

    /* line 6 Y420Format[Olny] */
    PROC_PRINT(pFile, "%-20s:", "YCbCr420[Olny]");
    for (i = 0;i < pstCap->u32OnlySupportY420VICNum; i++)
    {
        PROC_PRINT(pFile, " %u", pstCap->au32OnlySupportY420Format[i]);
    }
    PROC_PRINT(pFile, "\n");

    /* line 7 Colorimetry  */
    PROC_PRINT(pFile, "%-20s:", "Colorimetry");
    if (pstCap->stColorMetry.bxvYCC601)
    {
        PROC_PRINT(pFile, " %-s", "xvYCC601");    
    }
    if (pstCap->stColorMetry.bxvYCC709)
    {
        PROC_PRINT(pFile, " %-s", "xvYCC709");    
    }
    if (pstCap->stColorMetry.bsYCC601)
    {
        PROC_PRINT(pFile, " %-s", "sYCC601"); 
    }
    if (pstCap->stColorMetry.bAdobleYCC601)
    {
        PROC_PRINT(pFile, " %-s", "AdobleYCC601");    
    }
    if (pstCap->stColorMetry.bAdobleRGB)
    {
        PROC_PRINT(pFile, " %-s", "AdobleRGB");   
    }
    if (pstCap->stColorMetry.bBT2020cYCC)
    {
        PROC_PRINT(pFile, " %-s", "BT2020cYCC");  
    }
    if (pstCap->stColorMetry.bBT2020YCC)
    {
        PROC_PRINT(pFile, " %-s", "BT2020YCC");   
    }
    if (pstCap->stColorMetry.bBT2020RGB)
    {
        PROC_PRINT(pFile, " %-s", "BT2020RGB");   
    }
    PROC_PRINT(pFile, "\n");

    return;
}

/** print the video capability of sink
**/
static HI_VOID Proc_SinkFormatCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0, j = 0;
    HI_CHAR *pTmpFmt = HI_NULL;
	HI_CHAR aTmpArray[25];

    CHECK_POINTER(pstCap);
    pTmpFmt = Proc_Vic2Format(pstCap->u32NativeFormat);
    CHECK_POINTER(pTmpFmt);

    PROC_PRINT(pFile, "------------------------------ FormatCap ------------------------------------------\n");

    /* line 1 */

	for (i = 0; i < pstCap->u32SupportVICNum; i++)
	{
		pTmpFmt = Proc_Vic2Format(pstCap->au32SupportFormat[i]);
		if (pTmpFmt != HI_NULL)
		{
			PROC_PRINT(pFile,"%-20s ", pTmpFmt);
		}
		else
		{
			PROC_PRINT(pFile,"(VIC)%-15u ", pstCap->au32SupportFormat[i]);
		}

		j++;
		if ((j % 4)==0)
		{
			PROC_PRINT(pFile,"\n");
		}
	}

	for (i = 0; i < ARRAY_SIZE(pstCap->stStdTiming); i++)
	{
		pTmpFmt = Proc_StdTiming2Format(&pstCap->stStdTiming[i]);
		if (pTmpFmt != HI_NULL)
		{
			PROC_PRINT(pFile,"%-20s ", pTmpFmt);
			j++;
			if ((j % 4) == 0)
			{
				PROC_PRINT(pFile, "\n");
			}
		}
		else if ((pstCap->stStdTiming[i].u32HorActive != 0) && 
                 (pstCap->stStdTiming[i].u32VerActive != 0))
		{
			int n = 0;
			OSAL_MEMSET(aTmpArray, '\0', sizeof(aTmpArray));
			n = HDMI_OSAL_Snprintf(aTmpArray, 21, "%dx%d", pstCap->stStdTiming[i].u32HorActive,				
											          pstCap->stStdTiming[i].u32VerActive);
			if ((pstCap->stStdTiming[i].u32RefreshRate / 100) != 0)
			{
				HDMI_OSAL_Snprintf(aTmpArray+n,21-n, "_%d", (pstCap->stStdTiming[i].u32RefreshRate / 100));
			}
			PROC_PRINT(pFile,"%-20s ", aTmpArray);
			j++;
			if ((j % 4) == 0)
			{
				PROC_PRINT(pFile, "\n");
			}
		}
		
	}

	/*VESA Enhanced EDID Standard Release A, Rev.1 Page 16 */
	for (i = 0; i < pstCap->u32EstabNum && pstCap->au32EstabTiming[i] < ARRAY_SIZE(s_EstabTiming2Str); i++)
	{
		PROC_PRINT(pFile, "%-20s ", s_EstabTiming2Str[i]);
		j++;
		if ((j % 4) == 0)
		{
			PROC_PRINT(pFile, "\n");
		}
	}
    if ((j % 4)!= 0)
    {
        PROC_PRINT(pFile, "\n");
    }

    return;
}

/** print the 3D capability of sink
**/
static HI_VOID Proc_Sink3DCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0;

    CHECK_POINTER(pstCap);
	PROC_PRINT(pFile, "------------------------------ 3DCap ----------------------------------------------\n");

    /* line 1 */
	PROC_PRINT(pFile,"%-20s: %-20s ", "3DSupport", pstCap->st3DInfo.bSupport3D? STR_YES: STR_NO);
	PROC_PRINT(pFile,"%-20s: %-20s \n", "3DOsdDisparity", pstCap->bSupport3dOsdDisparity? STR_YES: STR_NO);

    /* line 2 */
    PROC_PRINT(pFile,"%-20s: %-20s ",	"3DDualView ", pstCap->bSupport3dDualView? STR_YES: STR_NO);
	PROC_PRINT(pFile,"%-20s: %-20s \n", "3DIndepView", pstCap->bSupport3dIndependentView? STR_YES: STR_NO);

    /* line 3 */
	PROC_PRINT(pFile,"%-20s:",	"3DTypeSupport");

	for (i = 0 ;i < ARRAY_SIZE(pstCap->st3DInfo.bSupport3DType); i++)
	{
		if (pstCap->st3DInfo.bSupport3DType[i] && i<ARRAY_SIZE(s_eStereMode2Str))
		{
            PROC_PRINT(pFile," %s", s_eStereMode2Str[i]);  
        }
	}
	PROC_PRINT(pFile,"\n");

}

/** print the audio capability of sink
**/
static HI_VOID Proc_SinkAudioCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0, j = 0;
    HDMI_AUDIO_FORMAT_CODE_E enAudFmtCode;
    HDMI_EDID_AUDIO_INFO_S *pAudioInfo = HI_NULL;

    CHECK_POINTER(pstCap);
    PROC_PRINT(pFile, "------------------------------ AudioCap -------------------------------------------\n");

  	/* audio info */
	for ( i= 0; i < pstCap->u32AudioInfoNum; i++)
	{
        /* line 1 */
		PROC_PRINT(pFile,"%s.%-d: \n", "NO", i);
        enAudFmtCode = pstCap->stAudioInfo[i].enAudFmtCode;
        pAudioInfo = &pstCap->stAudioInfo[i];

        /* line 2 */
		PROC_PRINT(pFile,"%-20s: %-20s ", "CodeType", s_AudioCodeType2Str[enAudFmtCode]);
		PROC_PRINT(pFile,"%-20s: %-20d \n",	"MaxChannelNum", pAudioInfo->u8AudChannel);

        /* line 3 */
		if (pAudioInfo->u32MaxBitRate)
		{
			PROC_PRINT(pFile,"%-20s: %-20d ", "MaxBitRate(KHz)", pAudioInfo->u32MaxBitRate);
		}
		else
		{
			PROC_PRINT(pFile,"%-20s: %-20s ", "MaxBitRate(KHz)", STR_NA);
		}

		PROC_PRINT(pFile,"%-20s:", "BitDepth");
		if (pAudioInfo->u32SupportBitDepthNum == 0)
		{
			PROC_PRINT(pFile," %s", STR_NA);
		}
		for (j = 0; j < pAudioInfo->u32SupportBitDepthNum; j++)
		{
			PROC_PRINT(pFile," %d", pAudioInfo->enSupportBitDepth[j]);
		}
		PROC_PRINT(pFile,"\n");

        /* line 4 */
		PROC_PRINT(pFile,"%-20s:", "SampleRate(Hz)");	
		for ( j = 0; j < pAudioInfo->u32SupportSampleRateNum; j++)
		{
			PROC_PRINT(pFile," %d", pAudioInfo->enSupportSampleRate[j]);
		}
		PROC_PRINT(pFile,"\n");
	}

    return;
}


/** print the hdr capability of sink
**/
static HI_VOID Proc_SinkHdrCap(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    CHECK_POINTER(pstCap);
    PROC_PRINT(pFile, "------------------------------- HdrCap -------------------------------------------\n");
    /* line 1 */
    PROC_PRINT(pFile,"%-20s: %-20s ", "HdrEotfSdr", pstCap->stHdrCap.stEotf.bEotfSdr ? STR_YES: STR_NO);
    PROC_PRINT(pFile,"%-20s: %-20s \n", "HdrEotfHdr", pstCap->stHdrCap.stEotf.bEotfHdr ? STR_YES: STR_NO);
    /* line 2 */
    PROC_PRINT(pFile,"%-20s: %-20s ", "HdrEotfSt2084", pstCap->stHdrCap.stEotf.bEotfSmpteSt2084 ? STR_YES: STR_NO);
    PROC_PRINT(pFile,"%-20s: %-20d \n", "MaxLum", pstCap->stHdrCap.u8MaxLuminance_CV);
    /* line 3 */
    PROC_PRINT(pFile,"%-20s: %-20d ", "AveLum", pstCap->stHdrCap.u8AverageLumin_CV);
    PROC_PRINT(pFile,"%-20s: %-20d \n", "MinLum", pstCap->stHdrCap.u8MinLuminance_CV);

    PROC_PRINT(pFile, "------------------------------- DolbyCap -----------------------------------------\n");
    PROC_PRINT(pFile,"%-20s: 0x%-18x ", "DolbyOUI", pstCap->stDolbyCap.u32DolbyOui);
    PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyCapsVer", pstCap->stDolbyCap.u8VSVDBVersion);
    /* line 1 */
    PROC_PRINT(pFile,"%-20s: %-20s ", "DolbySu_Y422", pstCap->stDolbyCap.bYUV422_12Bit ? STR_YES: STR_NO);
    PROC_PRINT(pFile,"%-20s: %-20s\n", "DolbySu_2160P60", pstCap->stDolbyCap.b2160P60 ? STR_YES: STR_NO);
    /* line 2 */
    PROC_PRINT(pFile,"%-20s: %-20s ", "DolbyYccQran", pstCap->stDolbyCap.bYccQrangeSelectable ? STR_YES: STR_NO);
    PROC_PRINT(pFile,"%-20s: %-20s\n", "DolbyRgbQran", pstCap->stDolbyCap.bRgbQrangeSelectable ? STR_YES: STR_NO);
    /* line 3 */
    PROC_PRINT(pFile,"%-20s: %-20d ", "DolbyRed_X", pstCap->stDolbyCap.u16Red_X);
    PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyRed_Y", pstCap->stDolbyCap.u16Red_Y);
    /* line 4 */
    PROC_PRINT(pFile,"%-20s: %-20d ", "DolbyGreen_X", pstCap->stDolbyCap.u16Green_X);
    PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyGreen_Y", pstCap->stDolbyCap.u16Green_Y);
    /* line 5 */
    PROC_PRINT(pFile,"%-20s: %-20d ", "DolbyBlue_X", pstCap->stDolbyCap.u16Blue_X);
    PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyBlue_Y", pstCap->stDolbyCap.u16Blue_Y);
    /* line 6 */
    PROC_PRINT(pFile,"%-20s: %-20d ", "DolbyMinLum", pstCap->stDolbyCap.u16MinLuminance);
    PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyMaxLum", pstCap->stDolbyCap.u16MaxLuminance);

    if(0 == pstCap->stDolbyCap.u8VSVDBVersion)
    {
        /* line 7 */
        PROC_PRINT(pFile,"%-20s: %-20d ", "DolbyWhite_X", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer0.u16White_X);
        PROC_PRINT(pFile,"%-20s: %-20d\n", "DolbyWhite_Y", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer0.u16White_Y);
        /* line 8 */
        PROC_PRINT(pFile,"%-20s: %-20d ", "DMmajorVer", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer0.u8DMmajorVersion);
        PROC_PRINT(pFile,"%-20s: %-20d\n", "DMminorVer", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer0.u8DMminorVersion);
    }
    else if(1 == pstCap->stDolbyCap.u8VSVDBVersion)
    {
        /* line 7 */
        PROC_PRINT(pFile,"%-20s: %-20d ", "DMVersion", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer1.u8DMVersion);
        PROC_PRINT(pFile,"%-20s: %-20d\n", "Colorimetry", pstCap->stDolbyCap.unOtherCaps.stDolbyCapsVer1.bColorimetry);
    }
}


/** print the detail timing of sink
**/
static HI_VOID Proc_SinkDetailTiming(struct seq_file *pFile, HDMI_SINK_CAPABILITY_S *pstCap)
{
    HI_S32 i = 0;
	/* Detail Timing */
	if (pstCap->u32PerferTimingNum > 0)
	{
		/* line1 */
		PROC_PRINT(pFile, "------------------------------ DetailTiming ------------------------------------------\n");

		/* line2 */
		PROC_PRINT(pFile,"%-5s:%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s\n",
				"[NO.]","HACT","VACT","P/I","PClk","AspW","AspH","HFB","HPW","HBB","VFB","VPW","VBB","ImgW","ImgH","IHS","IVS","IDV");
		for (i = 0; i < pstCap->u32PerferTimingNum; i++)
		{
			PROC_PRINT(pFile,"[%3u]:%-4u|%-4u|%-4c|%-3uM|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4s|%-4s|%-4s\n",
					i,
					pstCap->stPerferTiming[i].u32HACT,
					pstCap->stPerferTiming[i].u32VACT,
					pstCap->stPerferTiming[i].bInterlace ? 'I' : 'P',
					pstCap->stPerferTiming[i].u32PixelClk / 1000,
					pstCap->stPerferTiming[i].u32AspectRatioW,
					pstCap->stPerferTiming[i].u32AspectRatioH,
					pstCap->stPerferTiming[i].u32HFB,
					pstCap->stPerferTiming[i].u32HPW,
					pstCap->stPerferTiming[i].u32HBB,
					pstCap->stPerferTiming[i].u32VFB,
					pstCap->stPerferTiming[i].u32VPW,
					pstCap->stPerferTiming[i].u32VBB,
					pstCap->stPerferTiming[i].u32ImageWidth,
					pstCap->stPerferTiming[i].u32ImageHeight,
					pstCap->stPerferTiming[i].bIHS ? STR_YES : STR_NO,
					pstCap->stPerferTiming[i].bIVS ? STR_YES : STR_NO,
					pstCap->stPerferTiming[i].bIDV ? STR_YES : STR_NO );
		}
	}

    return;	
}

/******************************************************************************/
/* Public interface                                                           */
/******************************************************************************/
HI_S32 DRV_HDMI_ReadProc(struct seq_file *pFile, HI_VOID *pArg)
{
    PROC_PRINT(pFile,"\n%s\n\n",VERSION_STRING);
    Proc_SWVersion(pFile);
	Proc_AppAttrInfo(pFile, HDMI_DEVICE_ID0);
	Proc_SWStatus(pFile, HDMI_DEVICE_ID0);
#ifdef HDMI_HDCP_SUPPORT
	Proc_HDCPStatus(pFile, HDMI_DEVICE_ID0);
#endif
#ifdef HDMI_CEC_SUPPORT
	Proc_CECStatus(pFile, HDMI_DEVICE_ID0);
#endif
#if ((defined HDMI_HDR_SUPPORT) && (defined HDMI_CHIP_TYPE_HI3798CV200))
    Proc_HDRStatus(pFile, HDMI_DEVICE_ID0);
#endif
	Proc_HWStatus(pFile, HDMI_DEVICE_ID0);
	Proc_EventInfo(pFile, HDMI_DEVICE_ID0);
        
	return HI_SUCCESS;
}

HI_S32 DRV_HDMI_VOReadProc(struct seq_file *pFile, HI_VOID *pArg)
{
    PROC_PRINT(pFile,"\n%s\n\n",VERSION_STRING);

    Proc_SWVersion(pFile);
	Proc_VideoInfo(pFile, HDMI_DEVICE_ID0);
#ifdef HDMI_HDR_SUPPORT    
    Proc_HDRInfo(pFile, HDMI_DEVICE_ID0);
#endif        
	return HI_SUCCESS;
}

HI_S32 DRV_HDMI_AOReadProc(struct seq_file *pFile, HI_VOID *pArg)
{
    PROC_PRINT(pFile,"\n%s\n\n",VERSION_STRING);

    Proc_SWVersion(pFile);
	Proc_AudioInfo(pFile, HDMI_DEVICE_ID0);
        
	return HI_SUCCESS;
}

HI_S32 DRV_HDMI_SinkReadProc(struct seq_file *pFile, HI_VOID *pArg)
{
    HI_S32 s32Ret;
    HDMI_SINK_CAPABILITY_S *pstCap = HI_NULL;

	PROC_PRINT(pFile,"\n%s\n\n", VERSION_STRING);

    Proc_SWVersion(pFile);
    Proc_EdidRawData(pFile, HDMI_DEVICE_ID0);
    Proc_EdidStatus(pFile, HDMI_DEVICE_ID0);
    s32Ret = Proc_GetCapInfo(HDMI_DEVICE_ID0, &pstCap);
    if (HI_SUCCESS == s32Ret && HI_NULL != pstCap)
    {
        Proc_SinkBasicCap(pFile, pstCap);
        Proc_SinkVideoCap(pFile, pstCap);
        Proc_SinkFormatCap(pFile, pstCap);
        Proc_Sink3DCap(pFile, pstCap);
        Proc_SinkAudioCap(pFile, pstCap);
        Proc_SinkHdrCap(pFile, pstCap);
        Proc_SinkDetailTiming(pFile, pstCap);
    }
    else
    {
        HDMI_ERR("call Proc_GetCapInfo failed! s32Ret: %d, pstCap: %p\n", s32Ret, pstCap);
    }
	
	return HI_SUCCESS;
}

