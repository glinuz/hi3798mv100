/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_edid.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/12/10
  Description   :
  History       :
  Date          : 2014/12/10
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __DRV_HDMI_EDID_H__
#define __DRV_HDMI_EDID_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#include "hi_type.h"
#include "drv_hdmi_infoframe.h"
#include "drv_hdmi_common.h"

#define HDMI_EDID_BLOCK_SIZE		    	128
#define HDMI_EDID_MAX_BLOCK_NUM		 	    4
#define HDMI_EDID_TOTAL_SIZE          		((HDMI_EDID_BLOCK_SIZE)*(HDMI_EDID_MAX_BLOCK_NUM))

#define HDMI_EDID_MAX_HDMI14_TMDS_RATE		340000          //in KHz
#define HDMI_EDID_MAX_STDTIMNG_COUNT		8
#define HDMI_EDID_MAX_VIC_COUNT 			128
#define HDMI_EDID_MAX_AUDIO_CAP_COUNT  	    16
#define HDMI_EDID_MAX_SAMPE_RATE_NUM 		8
#define HDMI_EDID_MAX_BIT_DEPTH_NUM  		6
#define HDMI_EDID_MAX_DETAIL_TIMING_COUNT	10
#define HDMI_EDID_MAX_SINKNAME_COUNT        14


/**EDID established timing enum */
typedef enum hiHDMI_EDID_ESTAB_TIMING_E{

	HDMI_EDID_ESTABTIMG_VESA_800X600_60 		,
	HDMI_EDID_ESTABTIMG_VESA_800X600_56			,
	HDMI_EDID_ESTABTIMG_VESA_640X480_75			,
	HDMI_EDID_ESTABTIMG_VESA_640X480_72			,
	HDMI_EDID_ESTABTIMG_IBM_VGA_640X480_67		,
	HDMI_EDID_ESTABTIMG_IBM_XGA_640X480_60		,
	HDMI_EDID_ESTABTIMG_IBM_XGA2_720X400_88		,
	HDMI_EDID_ESTABTIMG_IBM_XGA_720X400_70		,

	HDMI_EDID_ESTABTIMG_VESA_1280X1024_75		,
	HDMI_EDID_ESTABTIMG_VESA_1024X768_75		,
	HDMI_EDID_ESTABTIMG_VESA_1024X768_70		,
	HDMI_EDID_ESTABTIMG_VESA_1024X768_60		,
	HDMI_EDID_ESTABTIMG_IBM_1024X768_87			,
	HDMI_EDID_ESTABTIMG_APPLE_MACII_832X624_75	,
	HDMI_EDID_ESTABTIMG_VESA_800X600_75			,
	HDMI_EDID_ESTABTIMG_VESA_800X600_72			,

	HDMI_EDID_ESTABTIMG_VESA_1152X870_75		,

	HDMI_EDID_ESTABTIMG_BUTT					,

}HDMI_EDID_ESTAB_TIMING_E;

/**EDID detailed Timing Info struct*/
typedef struct
{
    HI_U32                        u32VFB;          /**<vertical front blank*/
    HI_U32                        u32VBB;          /**<vertical back blank*/
    HI_U32                        u32VACT;         /**<vertical active area*/
    HI_U32                        u32HFB;          /**<horizonal front blank*/
    HI_U32                        u32HBB;          /**<horizonal back blank*/
    HI_U32                        u32HACT;         /**<horizonal active area*/
    HI_U32                        u32VPW;          /**<vertical sync pluse width*/
    HI_U32                        u32HPW;          /**<horizonal sync pluse width*/
    HI_BOOL                       bIDV;            /**< flag of data valid signal is needed flip*/
    HI_BOOL                       bIHS;            /**<flag of horizonal sync pluse is needed flip*/
    HI_BOOL                       bIVS;            /**<flag of vertical sync pluse is needed flip*/
    HI_U32                        u32ImageWidth;   /**<image width */
    HI_U32                        u32ImageHeight;  /**<image height */
    HI_U32                        u32AspectRatioW; /**<aspect ratio width */
    HI_U32                        u32AspectRatioH; /**<aspect ratio height */
    HI_BOOL                       bInterlace;      /**<flag of interlace */
    HI_S32                        u32PixelClk;     /**<pixelc clk for this timing */
} HDMI_EDID_PRE_TIMING_S;

/**EDID standard Timing Info struct*/
typedef struct
{
	HI_U32	u32HorActive;
	HI_U32	u32VerActive;
	HI_U32	u32RefreshRate;
}HDMI_EDID_STD_TIMING_S;


/** virtual video code ,private define for VSDB 4k-format */
#define		HDMI_EDID_MAX_REAL_VIC			255U
#define		HDMI_VIC_VIRTUAL_BASE			(HDMI_EDID_MAX_REAL_VIC)
typedef enum
{
	HDMI_EDID_VIRTUAL_VIC_3840X2160_30 	= HDMI_VIC_VIRTUAL_BASE+1,
	HDMI_EDID_VIRTUAL_VIC_3840X2160_25 	,
	HDMI_EDID_VIRTUAL_VIC_3840X2160_24	,
	HDMI_EDID_VIRTUAL_VIC_4096X2160_24 	,
	HDMI_EDID_VIRTUAL_VIC_VSDB_BUTT	,
}HDMI_EDID_VIRTUAL_VIC_E;




/**EDID Audio Infomation */
typedef struct
{
    HDMI_AUDIO_FORMAT_CODE_E        enAudFmtCode;
    HDMI_SAMPLE_RATE_E              enSupportSampleRate[HDMI_EDID_MAX_SAMPE_RATE_NUM];
    HI_U32                          u32SupportSampleRateNum;
    HI_U8                           u8AudChannel;      /**<Aud Channel of the coding type*/

    HDMI_AUDIO_BIT_DEPTH_E          enSupportBitDepth[HDMI_EDID_MAX_BIT_DEPTH_NUM];
    HI_U32                          u32SupportBitDepthNum;
    HI_U32                          u32MaxBitRate;   /* kHz*/
}HDMI_EDID_AUDIO_INFO_S;

/**EDID Audio speaker enum*/
typedef enum
{
    HDMI_EDID_AUDIO_SPEAKER_FL_FR   ,
    HDMI_EDID_AUDIO_SPEAKER_LFE     ,
    HDMI_EDID_AUDIO_SPEAKER_FC      ,
    HDMI_EDID_AUDIO_SPEAKER_RL_RR   ,
    HDMI_EDID_AUDIO_SPEAKER_RC      ,
    HDMI_EDID_AUDIO_SPEAKER_FLC_FRC ,
    HDMI_EDID_AUDIO_SPEAKER_RLC_RRC ,
    HDMI_EDID_AUDIO_SPEAKER_FLW_FRW ,
    HDMI_EDID_AUDIO_SPEAKER_FLH_FRH ,
    HDMI_EDID_AUDIO_SPEAKER_TC      ,
    HDMI_EDID_AUDIO_SPEAKER_FCH     ,
    HDMI_EDID_AUDIO_SPEAKER_BUTT    ,
} HDMI_EDID_AUDIO_SPEAKER_E;

/**EDID MANUFACTURE information */
typedef struct
{
    HI_U8               u8MfrsName[4];
    HI_U32              u32ProductCode;
    HI_U32              u32SerialNumber;
    HI_U32              u32Week;
    HI_U32              u32Year;
    HI_U8               u8pSinkName[HDMI_EDID_MAX_SINKNAME_COUNT];
}HDMI_EDID_MANUFACTURE_INFO_S;


/**EDID deep color information */
typedef struct
{
    HI_BOOL    bDeepColorY444   ;
    HI_BOOL    bDeepColor30Bit  ;
    HI_BOOL    bDeepColor36Bit  ;
    HI_BOOL    bDeepColor48Bit  ;
} HDMI_EDID_DEEP_COLOR_S;

/**EDID Colorimety Info struct*/
typedef struct
{
    HI_BOOL    bxvYCC601      ;
    HI_BOOL    bxvYCC709      ;
    HI_BOOL    bsYCC601       ;
    HI_BOOL    bAdobleYCC601  ;
    HI_BOOL    bAdobleRGB     ;
    HI_BOOL    bBT2020cYCC    ;
	HI_BOOL    bBT2020YCC     ;
	HI_BOOL    bBT2020RGB     ;
} HDMI_EDID_COLORIMETRY_S;


/**EDID color space Info struct*/
typedef struct
{
    HI_BOOL    bRGB444        ;
    HI_BOOL    bYCbCr422      ;
    HI_BOOL    bYCbCr444      ;
    HI_BOOL    bYCbCr420      ;
} HDMI_EDID_COLOR_SPACE_S;


/**EDID cec address Info struct*/
typedef struct
{
    HI_BOOL    bPhyAddrValid   ;
    HI_U8      u8PhyAddrA      ;
    HI_U8      u8PhyAddrB      ;
    HI_U8      u8PhyAddrC      ;
    HI_U8      u8PhyAddrD      ;
} HDMI_EDID_CEC_ADDRESS_S;


/**EDID deep color YUV420 Info struct*/
typedef struct
{
    //HI_BOOL    bDeepColorY420   ;
    HI_BOOL    bDeepColor30Bit  ;
    HI_BOOL    bDeepColor36Bit  ;
    HI_BOOL    bDeepColor48Bit  ;
} HDMI_EDID_DEEP_COLOR_Y420_S;





/**HDMI 3d enum type*/
typedef enum
{
    HDMI_EDID_3D_FRAME_PACKETING                 = 0x00, /**<3d type:Frame Packing*/
    HDMI_EDID_3D_FIELD_ALTERNATIVE               = 0x01, /**<3d type:Field alternative*/
    HDMI_EDID_3D_LINE_ALTERNATIVE                = 0x02, /**<3d type:Line alternative*/
    HDMI_EDID_3D_SIDE_BY_SIDE_FULL               = 0x03, /**<3d type:Side by side full*/
    HDMI_EDID_3D_L_DEPTH                         = 0x04, /**<3d type:L+depth*/
    HDMI_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH = 0x05, /**<3d type:L+depth+Graphics+Graphics-depth*/
    HDMI_EDID_3D_TOP_AND_BOTTOM                  = 0x06, /**<3d type:Top and Bottom*/
    HDMI_EDID_3D_SIDE_BY_SIDE_HALF               = 0x08, /**<3d type:Side by side half*/
    HDMI_EDID_3D_BUTT,
}HDMI_EDID_3D_TYPE_E;


/**3d Info struct*/
typedef struct
{
    HI_BOOL    bSupport3D  ;
    HI_BOOL    bSupport3DType[HDMI_EDID_3D_BUTT] ;
} HDMI_EDID_3D_INFO_S;


/* EDID return_value type of DRV_HDMI_EdidCapabilityGet function */
typedef enum{
	HDMI_EDID_DATA_INVALID,			/* return invalid Capability*/
	HDMI_EDID_DATA_VALIDSINK,		/* return valid sink Capability*/
	HDMI_EDID_DATA_VALIDTEST,		/* return valid test Capability */
	HDMI_EDID_DATA_BUTT
}HDMI_EDID_DATA_E;

/* EDID update mode*/
typedef enum{
	HDMI_EDID_UPDATE_SINK,			/* update from SINK by DDC(HAL support interface). */
	HDMI_EDID_UPDATE_TEST0,			/* update from EDID_TEST,and the following same. */
	HDMI_EDID_UPDATE_TEST1,
	HDMI_EDID_UPDATE_TEST2,
	HDMI_EDID_UPDATE_TEST3,
	HDMI_EDID_UPDATE_TEST4,
	HDMI_EDID_UPDATE_TEST5,
	HDMI_EDID_UPDATE_TEST6,
	HDMI_EDID_UPDATE_TEST7,
	HDMI_EDID_UPDATE_TEST8,
	HDMI_EDID_UPDATE_TEST9,
	HDMI_EDID_UPDATE_TEST10,
	HDMI_EDID_UPDATE_TEST11,
	HDMI_EDID_UPDATE_TEST12,
	HDMI_EDID_UPDATE_TEST13,
	HDMI_EDID_UPDATE_TEST14,
	HDMI_EDID_UPDATE_TEST15,
	HDMI_EDID_UPDATE_TEST16,
	HDMI_EDID_UPDATE_TEST17,
	HDMI_EDID_UPDATE_TEST18,
	HDMI_EDID_UPDATE_TEST19,
	HDMI_EDID_UPDATE_TEST20,
	HDMI_EDID_UPDATE_TEST21,
	HDMI_EDID_UPDATE_TEST22,
	HDMI_EDID_UPDATE_TEST23,
	HDMI_EDID_UPDATE_TEST24,
	HDMI_EDID_UPDATE_DEBUG,
	HDMI_EDID_UPDATE_BUTT,
}HDMI_EDID_UPDATE_MODE_E;

/* EDID parse-error number.
NOTE:parse-error meaning that something we don't expect occurs,
and it can lead  other's raw field to  parse error.So we stop and return FAILURE when occurred.
User should read the status when returned  FAILURE.*/
typedef enum hiHDMI_EDID_PARSE_ERR_E{
	EDID_PARSE_ERR_NONE,
	EDID_PARSE_ERR_CHECKSUM,
	EDID_PARSE_ERR_HEADER,
	EDID_PARSE_ERR_FST_BLK_VER,
	EDID_PARSE_ERR_TAG_UNKNOWN,
	EDID_PARSE_ERR_CEA_REVERION,
	EDID_PARSE_ERR_BUTT
}HDMI_EDID_PARSE_ERR_E;

/* EDID parse-warning information bit shift code.
NOTE:parse-warning meaning that something we don't expect occurs,
but it can't lead  other's raw field to  parse error.
User should read the status  to check when parsing is finished.*/
typedef enum hiHDMI_EDID_PARSE_WARN_E{
	EDID_PARSE_WARN_NONE,
	EDID_PARSE_WARN_VENDOR_INVALID,
	EDID_PARSE_WARN_DTD_OVER,
	EDID_PARSE_WARN_DTD_INVALID,
	EDID_PARSE_WARN_EXT_BLK_ZERO,
	EDID_PARSE_WARN_EXT_BLK_OVER,
	EDID_PARSE_WARN_AUDIO_CNT_OVER,
	EDID_PARSE_WARN_AUDIO_FMT_INVALID,
	EDID_PARSE_WARN_VIC_CNT_OVER,
	EDID_PARSE_WARN_VIC_INVALID,
	EDID_PARSE_WARN_VSDB_INVALID,
	EDID_PARSE_WARN_HFVSDB_INVALID,
	EDID_PARSE_WARN_SPKDB_INVALID,
	EDID_PARSE_WARN_Y420VDB_VIC_OVER,
	EDID_PARSE_WARN_BLOCKLEN_INVALID,
	EDID_PARSE_WARN_BUTT
}HDMI_EDID_PARSE_WARN_E;

typedef struct hiHDMI_EDID_AUDIO_CAPABILITY_S
{
    HI_BOOL             bAudioFmtSupported[HDMI_EDID_MAX_AUDIO_CAP_COUNT-1];
    HI_U32              u32AudioSampleRateSupported[HDMI_EDID_MAX_SAMPE_RATE_NUM]; /**<PCM smprate capability */
    HI_U32              u32MaxPcmChannels;                                         /**<u32MaxPcmChannels    */
}HDMI_AUDIO_CAPABILITY_S;

/* EDID status information */
typedef struct hiHDMI_EDID_STAT_S{
	HI_BOOL			bCapSink;				/* [current] is Capability parse from SINK or EDID_TEST */
	HI_BOOL			bCapValid;				/* [current] is Capability valid */
	HI_BOOL			bRawValid;				/* [current] is Raw data valid */
	HI_U32			u32RawLen;
	HDMI_EDID_PARSE_ERR_E	enParseErr;		/* [current] parse error code,in Byte */
	HI_U32			u32ParseWarn;			/* [current] parse warning ,in bits shift,see HDMI_EDID_PARSE_WARN_E */
	HI_U32			u32RawGetErrCnt;		/* error from Raw Data getting count */
	HI_U32			u32CapGetErrCnt;		/* error from Capability getting count */
	HI_U32			u32RawUpdateErrCnt;		/* error from Raw Data updating count */

}HDMI_EDID_STATUS_S;
/** EDID Phosphor or Filter Chromaticity **/
typedef struct hiHDMI_EDID_PHOSPHOR_S
{
    HI_U16 u16Red_x;
    HI_U16 u16Red_y;
    HI_U16 u16Green_x;
    HI_U16 u16Green_y;
    HI_U16 u16Blue_x;
    HI_U16 u16Blue_y;
    HI_U16 u16White_x;
    HI_U16 u16White_y;

}HDMI_EDID_PHOSPHOR_S;

#define DOLBY_IEEE_OUI  0x00D046

typedef struct hiHDMI_EDID_DOLBY_CAPS_VER_0_S
{
    HI_U8       u8DMmajorVersion;       /* the major version of display management implemented. only support when u8VSVDBVersion = 0.*/
    HI_U8       u8DMminorVersion;       /* the minor version of display management implemented. only support when u8VSVDBVersion = 0.*/
    HI_U16      u16White_X;             /* white point chromaticity coordinate x   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N), only support when u8VSVDBVersion = 0. */
    HI_U16      u16White_Y;             /* white point  chromaticity coordinate y  ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N), only support when u8VSVDBVersion = 0.*/
}HDMI_EDID_DOLBY_CAPS_VER_0_S;

typedef struct hiHDMI_EDID_DOLBY_CAPS_VER_1_S
{
    HI_U8       u8DMVersion;            /* 0:based on display management v2.x; 1:based on the video and blending pipeline v3.x; 2-7: reserved. only support when u8VSVDBVersion = 1. */
    HI_BOOL     bColorimetry;           /* this bit is valid only u8VSVDBVersion = 1. 0: Dolby Vision HDMI sink's colorimetry is close to Rec.709, 1: EDR HDMI sink's colorimetry is close to P3, if Byte[9] to Byte[14] are present, ignores this bit. *//**<CNcomment:若为0，Dolby Vision Sink设备色域接近Rec.709，若为1，Dolby Vision Sink设备色域接近P3，但如果Byte[9]到Byte[14]有声明，则忽略此标志。*/
}HDMI_EDID_DOLBY_CAPS_VER_1_S;

typedef union hiHDMI_EDID_DOLBY_CAPS_U
{
    HDMI_EDID_DOLBY_CAPS_VER_0_S stDolbyCapsVer0;
    HDMI_EDID_DOLBY_CAPS_VER_1_S stDolbyCapsVer1;
}HDMI_EDID_DOLBY_CAPS_U;

/* Dolby capability.see <Dolby  Vision  HDMI Transmission Specification> */
typedef struct hiHDMI_EDID_DOLBY_CAP_S
{
    HI_BOOL     bYccQrangeSelectable;   /**<flag of RGB Quantization Range selectable.when HI_TRUE,sink expect full/limited range is selectable;or sink expect default range.  *//**<CNcomment:Sinkòa?óRGBá??ˉ·??§ê?·??ú/?T?? ·??§?é???￡HI_TRUE±íê?sinkòa?ó·??§?é??￡?HI_FALSE±íê?sinkòa?ó??è?·??§?￡*/
    HI_BOOL     bRgbQrangeSelectable;   /**<flag of YCC Quantization Range selectable.when HI_TRUE,sink expect full/limited range is selectable;or sink expect default range.  *//**<CNcomment:Sinkòa?óYCCá??ˉ·??§ê?·??ú/?T?? ·??§?é???￡HI_TRUE±íê?sinkòa?ó·??§?é??￡?HI_FALSE±íê?sinkòa?ó??è?·??§?￡*/

    HI_U8       u8VSVDBVersion;         /* the version of VSVDB, if this equal to 1, the value of u16MinLuminance and u16MaxLuminance only 7 bits, the Rx, Ry, Gx, Gy, Bx, By only 8 bits. see <Dolby  Vision  HDMI Transmission Specification issue 2.6>*/
    HI_BOOL     bYUV422_12Bit;          /* support(HI_TRUE) or not support(HI_FALSE) a YUV422-12Bit dolby singal */
    HI_BOOL     b2160P60;               /* capable of processing a max timming 3840X2160p60(HI_TRUE) /3840X2160p30(HI_FALSE) */
    HI_BOOL     bGlobalDimming;         /* support(HI_TRUE) or not support(HI_FALSE) global dimming. */

    HI_U16      u16Red_X;               /* red primary chromaticity coordinate x    ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Red_Y;               /* red primary chromaticity coordinate y    ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N)*/
    HI_U16      u16Green_X;             /* green primary chromaticity coordinate x ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Green_Y;             /* green primary chromaticity coordinate y ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Blue_X;              /* blue primary chromaticity coordinate x   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Blue_Y;              /* blue primary chromaticity coordinate y   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16MinLuminance;        /*  minimum luminance value of the target display,[0,4095] cd/m^2 */
    HI_U16      u16MaxLuminance;        /*  maximum luminance value of the target display,[0,4095] cd/m^2 */

    HDMI_EDID_DOLBY_CAPS_U unOtherCaps; /* other caps in Dolby VSVDB. distinction between V0(u8VSVDBVersion=0) and V1(u8VSVDBVersion=1).*//**<CNcomment:Dolby的其他能力，依据Dolby VSVDB版本V0(u8VSVDBVersion=0)或V1来区分(u8VSVDBVersion=1)*/
    HI_U32      u32DolbyOui;             /* Dolby OUI */
}HDMI_EDID_DOLBY_CAP_S;


/* EOTF support. */
typedef struct hiHDMI_EDID_EOTF_S
{
    HI_BOOL bEotfSdr;               /* Traditional gamma - SDR Luminance Range  */
    HI_BOOL bEotfHdr;               /* Traditional gamma - HDR Luminance Range  */
    HI_BOOL bEotfSmpteSt2084;       /* SMPTE ST 2084 */
    HI_BOOL bEotfFuture;            /* Future EOTF  */
}HDMI_EDID_EOTF_S;

/* Supported Static Metadata Descriptor  */
typedef struct hiHDMI_EDID_METADATA_TYPE_S
{
    HI_BOOL bDescriptorType1;       /* Static Metadata Type 1   */
}HDMI_EDID_HDR_METADATA_TYPE_S;

/* HDR capability,see <CEA-861.3-2015> */
typedef struct hiHDMI_EDID_HDR_CAP_S{

    HDMI_EDID_EOTF_S                  stEotf;                 /*EOTF support. */
    HDMI_EDID_HDR_METADATA_TYPE_S     stMetadata;             /*static metadata descriptor type(ID) support. */
    HI_U8                             u8MaxLuminance_CV;      /*Desired Content Max Luminance Data. Real value(in 1cd/m^2) = 50.2^(CV/32) */
    HI_U8                             u8AverageLumin_CV;      /*Desired Content Max Frame-average Luminance. Real value(in 1cd/m^2) = 50.2^(CV/32) */
    HI_U8                             u8MinLuminance_CV;      /*Desired Content Min Luminance Data. Real value(in 1cd/m^2) = u8MaxLuminance_CV * (u8MinLuminance_CV/255)^2 / 100  */
}HDMI_EDID_HDR_CAP_S;

typedef struct hiHDMI_EDID_VIDEO_CAPABILITY_S
{
    HI_BOOL                     bDolbySupport;
    HI_BOOL                     bHdrSupport;
    HDMI_EDID_DOLBY_CAP_S       stDolbyCaps;
    HDMI_EDID_HDR_CAP_S         stHdrCaps;
    HDMI_EDID_COLORIMETRY_S     stColorMetry;
}HDMI_VIDEO_CAPABILITY_S;

/**EDID Display Base Parameters struct*//**CNcomment:EDID 显示屏基本参数 */
typedef struct hiHDMI_EDID_BASE_DISP_PARA_S
{
    HI_U8      u8MaxImageWidth;               /**<The disp image max width  (0~255)cm*//**<CNcomment: 可显示图像最大宽 */
    HI_U8      u8MaxImageHeight;              /**<The disp image max height (0~255)cm*//**<CNcomment: 可显示图像最大高 */
}HDMI_EDID_BASE_DISP_PARA_S;

/**HDMI sink capability of interface*/
typedef struct
{
    HI_BOOL                         bSupportHdmi;
    HI_U32                          u32NativeFormat;
	HI_U32							u32EstabNum;
	HI_U32							au32EstabTiming[HDMI_EDID_ESTABTIMG_BUTT];
	HDMI_EDID_STD_TIMING_S			stStdTiming[HDMI_EDID_MAX_STDTIMNG_COUNT];
	HI_U32							u32SupportVICNum;
	HI_U32                         	au32SupportFormat[HDMI_EDID_MAX_VIC_COUNT];
	HDMI_EDID_3D_INFO_S             st3DInfo;
    HDMI_EDID_DEEP_COLOR_S          stDeepColor;
    HDMI_EDID_COLORIMETRY_S         stColorMetry;
	HI_U8							u8MDBit;
    HDMI_EDID_COLOR_SPACE_S         stColorSpace;

    HDMI_EDID_AUDIO_INFO_S          stAudioInfo[HDMI_EDID_MAX_AUDIO_CAP_COUNT];
    HI_U32                          u32AudioInfoNum;
    HI_BOOL                         bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_BUTT];

    HI_U8                           u8ExtBlockNum;

    HI_U8                           u8Version;
    HI_U8                           u8Revision;
    HDMI_EDID_MANUFACTURE_INFO_S    stMfrsInfo;

    HDMI_EDID_CEC_ADDRESS_S         stCECAddr;
    HI_BOOL                         bSupportDVIDual;
    HI_BOOL                         bSupportsAI;
	HI_U32							u32PerferTimingNum;
    HDMI_EDID_PRE_TIMING_S	        stPerferTiming[HDMI_EDID_MAX_DETAIL_TIMING_COUNT];

	HI_U32							u32MaxTMDSClock;
	HI_BOOL							bILatencyFieldsPresent;
	HI_BOOL							bLatencyFieldsPresent;
	HI_BOOL							bHDMIVideoPresent;
	HI_U8							u8VideoLatency ;
	HI_U8							u8AudioLatency;
	HI_U8							u8InterlacedVideoLatency;
	HI_U8							u8InterlacedAudioLatency;

    HI_BOOL                         bSupportScramble;
    HI_BOOL                         bSupportHdmi_2_0;
	HI_BOOL							bSupportSCDC;
    HI_BOOL                         bSupportRRCapable;
    HI_BOOL                         bSupportLte340McscScrameble;
	HI_BOOL                         bSupport3dOsdDisparity;
    HI_BOOL                         bSupport3dDualView;
    HI_BOOL                         bSupport3dIndependentView;
	HI_U32							u32SupportY420VICNum;
	HI_U32                          au32SupportY420Format[HDMI_EDID_MAX_VIC_COUNT];
	HI_U32							u32OnlySupportY420VICNum;
	HI_U32                         	au32OnlySupportY420Format[HDMI_EDID_MAX_VIC_COUNT];
    HDMI_EDID_DEEP_COLOR_Y420_S     stDeepColorY420;
    HDMI_EDID_PHOSPHOR_S            stPhosOrChromCap;
    HDMI_EDID_HDR_CAP_S             stHdrCap;
    HDMI_EDID_DOLBY_CAP_S           stDolbyCap;
    HDMI_HDCP_CAPABILITY_S          stHdcpSupport;
    HDMI_EDID_BASE_DISP_PARA_S      stBasicDispPara;
}HDMI_SINK_CAPABILITY_S;

/* EDID  all information description */
typedef struct hiHDMI_EDID_INFO_S{
	HDMI_EDID_STATUS_S		stStatus;
	HI_U8					au8EdidRaw[HDMI_EDID_TOTAL_SIZE];
	HDMI_SINK_CAPABILITY_S	stCapability;
}HDMI_EDID_INFO_S;

/**
\brief	get Capability after parsing
\param[in]   @pstEdidInfo   	pointer to a HDMI_EDID_INFO_S
\param[out] @pCapability		pointer point to a HDMI_SINK_CAPABILITY_S
\retval see HDMI_EDID_DATA_E
*/
HDMI_EDID_DATA_E DRV_HDMI_EdidCapabilityGet(HDMI_EDID_INFO_S *pstEdidInfo,\
									 HDMI_SINK_CAPABILITY_S **pstCapability);

/**
\brief	get EDID raw data
\param[in]    @pstEdidInfo   	pointer to a HDMI_EDID_INFO_S
\param[in]	@u32Len		length in bytes[0~512]
\param[out]  @pu8RawData	pointer to a destination data buffer
\retval  read length[0~512] / HI_FALURE
*/
HI_S32 DRV_HDMI_EdidRawGet(HDMI_EDID_INFO_S *pstEdidInfo,\
											HI_U8 *pu8RawData, \
											HI_U32 u32Len);

/**
\brief	force to get EDID raw data from sink,default to update RAW & CAPABLITY
\param[in]    @pstEdidInfo   	pointer to a HDMI_EDID_INFO_S
\param[in]	@u32Len		length in bytes[0~512]
\param[out]  @pu8RawData	pointer to a destination data buffer
\retval  read length[0~512] / HI_FALURE
*/
HI_S32 DRV_HDMI_EdidRawForceGet(HDMI_EDID_INFO_S *pstEdidInfo,\
											HI_U8 *pu8RawData,\
											HI_U32 u32Len);


/**
\brief	update EDID raw data & parse EDID to a HDMI_SINK_CAPABILITY_S
\param[in]    @pstEdidInfo  	pointer to a HDMI_EDID_INFO_S
\param[in]	@eMode 		see HDMI_EDID_UPDATE_MODE_E
\retval  HI_SUCCESS;HI_FAILURE.
*/
HI_S32 DRV_HDMI_EdidUpdate(HDMI_EDID_INFO_S *pstEdidInfo,\
									HDMI_EDID_UPDATE_MODE_E eMode);

/**
\brief	get EDID status
\param[in]    @pstEdidInfo   	pointer to a HDMI_EDID_INFO_S
\param[out]	@pstStat	      		pointer to a HDMI_EDID_STATUS_S
\retval  HI_SUCCESS;HI_FAILURE.
*/
HI_S32 DRV_HDMI_EdidStatusGet(HDMI_EDID_INFO_S *pstEdidInfo,\
											HDMI_EDID_STATUS_S *pstStat);

HI_S32 DRV_HDMI_EdidReset(HDMI_EDID_INFO_S *pstEdidInfo);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __DRV_HDMI_EDID_H__ */
