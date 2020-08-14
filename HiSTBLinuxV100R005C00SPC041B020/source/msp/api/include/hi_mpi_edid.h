/******************************************************************************

   Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.

******************************************************************************
	File Name     : mpi_edid.h
	Version       : Initial Draft
	Author        : Hisilicon TC group
	Created       : 2012/05/26
	Description   :
	History       :
	1.Date        : 2012/05/26
	Author	      : l214567
	Modification  : Created file

*******************************************************************************/

#ifndef __HI_MPI_EDID_H__
#define __HI_MPI_EDID_H__


#include <linux/types.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_common.h"
#include "hi_unf_disp.h"
#include "list.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define HI_FATAL_EDID(fmt...) \
            HI_FATAL_PRINT(HI_ID_EDID, fmt)

#define HI_ERR_EDID(fmt...) \
            HI_ERR_PRINT(HI_ID_EDID, fmt)

#define HI_WARN_EDID(fmt...) \
            HI_WARN_PRINT(HI_ID_EDID, fmt)

#define HI_INFO_EDID(fmt...) \
            HI_INFO_PRINT(HI_ID_EDID, fmt)

#define HI_DBG_EDID(fmt...) \
                        HI_DBG_PRINT(HI_ID_EDID, fmt)

#else

#define HI_FATAL_EDID(fmt...)
#define HI_ERR_EDID(fmt...)
#define HI_WARN_EDID(fmt...)
#define HI_INFO_EDID(fmt...) 
#define HI_DBG_EDID(fmt...) 

#endif

typedef enum tagEDID_STEREO_VIEWING_TYPE_E
{
    EDID_STEREO_VIEWING_NO    = 0x00,        /*Normal Display No Stereo*/
    EDID_STEREO_VIEWING_SEQUENTIAL_R,        /*Field sequential stereo, right image when stereo sync signal = 1*/
    EDID_STEREO_VIEWING_SEQUENTIAL_L,        /*Field sequential stereo, left image when stereo sync signal = 1*/
    EDID_STEREO_VIEWING_INTERLEAVED_2R,      /*2-way interleaved stereo, right image on even lines*/
    EDID_STEREO_VIEWING_INTERLEAVED_2L,      /*2-way interleaved stereo, left image on even lines*/
    EDID_STEREO_VIEWING_INTERLEAVED_4,       /*4-way interleaved stereo*/
    EDID_STEREO_VIEWING_INTERLEAVED_SBS,     /*Side-by-Side interleaved stereo*/
    EDID_STEREO_VIEWING_BUTT
} EDID_STEREO_VIEWING_TYPE_E;

typedef struct tagEDID_CUSTOMER_TIMING_S		/* EDID CUSTOMER DEFINE TIMING*/
{
    struct list_head                    head;						/* Header */
    HI_U32 u32DetailTimingNum;
    EDID_STEREO_VIEWING_TYPE_E   enStereoType;
    HI_UNF_EDID_DETAIL_TIMING_S stDetailTiming;
} EDID_DETAIL_TIMING_S;

typedef struct EDID_SIMPLE_TIMING_S
{
    struct list_head    head;
    HI_UNF_EDID_SIMPLE_TIMING_S stSimpleTiming;
} EDID_SIMPLE_TIMING_S;


/*input signal type*/
typedef enum tagEDID_SIGNAL_TYPE_E
{
    EDID_SIGNAL_ANALOG    = 0x00,
    EDID_SIGNAL_NUDEFIND_DIGITAL,
    EDID_SIGNAL_DVI,
    EDID_SIGNAL_HDMI_A,
    EDID_SIGNAL_HDMI_B,
    EDID_SIGNAL_MDDI,                            /*Mobile Display Digital Interface */
    EDID_SIGNAL_DISPLAYPORT,

} EDID_SIGNAL_TYPE_E;

/*Signal Level Standard*/
typedef enum tagEDID_SIGNAL_LEVEL_STD_E
{
    EDID_SIGNAL_LEVE07_03    = 0x00,						/* 0.700, 0.300 (1.000 V P-P)*/
    EDID_SIGNAL_LEVE07_02,								/* 0.714, 0.286 (1.000 V P-P)*/
    EDID_SIGNAL_LEVE10_04,								/* 1.000, 0.400 (1.400 V P-P)*/
    EDID_SIGNAL_LEVE07_00,								/* 0.700, 0.000 (0.700 V P-P) SEE EVC STD. */

} EDID_SIGNAL_LEVEL_STD_E;

/*Display Type*/
typedef enum tagDID_DISPLAY_TYPE_E
{
   EDID_MONOCHROME_DISPLAY = 0,						/* Monochrome grayscale display*/
    EDID_RGB_COLOR_DISPLAY,							/* RGB color display*/
    EDID_NON_RGB_DISPLAY,							/* Non-RGB multicolor display*/
    EDID_UNDEFINED_DISPLAY,

} EDID_DISPLAY_TYPE_E;

/* 支持的颜色BIT EDID 1.4 Add*/
typedef enum tagEDID_COLO_DEPTH_E
{
    EDID_COLO_DEPTH_NUDEFIND = 0x00,
    EDID_COLO_DEPTH_6BIT,
    EDID_COLO_DEPTH_8BIT,
    EDID_COLO_DEPTH_10BIT,
    EDID_COLO_DEPTH_12BIT,
    EDID_COLO_DEPTH_14BIT,
    EDID_COLO_DEPTH_16BIT,

} EDID_COLO_DEPTH_E ;

/* 支持的颜色编码方式 EDID 1.4 Add*/
typedef enum tagEDID_COLO_ENCODING_E
{
    EDID_COLO_ENCODING_RGB444 = 0x00,          /*RGB 4:4:4*/
    EDID_COLO_ENCODING_RGB444YUV444,           /*RGB 4:4:4 & YCrCb 4:4:4*/
    EDID_COLO_ENCODING_RGB444YUV422,           /*RGB 4:4:4 & YCrCb 4:2:2*/
    EDID_COLO_ENCODING_RGB444YUV444YUV422,     /*RGB 4:4:4 & YCrCb 4:4:4 & YCrCb 4:2:2*/

} EDID_COLO_ENCODING_E;

typedef enum tagEDID_CUSTOMER_TYPE_E
{
    EDID_CUSTOMER_MANUFACTURER_INFO  = 0x00, /*Manufacturer Specified Display Descriptors*/
    EDID_CUSTOMER_DUMMY_INFO         = 0x10, /*Dummy Descriptor EDID 1.4 Add*/
    EDID_CUSTOMER_RESERVED           = 0xF6, /*Reserved*/
    EDID_CUSTOMER_EST_TIMING         = 0xF7, /*Established Timings III EDID 1.4 Add*/
    EDID_CUSTOMER_CVT_TIMING         = 0xF8, /*CVT 3 Byte Timing Codes EDID 1.4 Add*/
    EDID_CUSTOMER_DCM                = 0xF9, /*Display Color Management (DCM) Data EDID 1.4 Add*/
    EDID_CUSTOMER_STANDARD_TIMING    = 0xFA, /*Standard Timing Identifications*/
    EDID_CUSTOMER_COLOR_POINT        = 0xFB, /*Color Point Data*/
    EDID_CUSTOMER_MONITOR_NAME       = 0xFC, /*Display Product Name*/
    EDID_CUSTOMER_MONITOR_RANGE      = 0xFD, /*Monitor range limits, binary coded*/
    EDID_CUSTOMER_ALPANUMERIC_DATA   = 0xFE, /*Alphanumeric Data String*/
    EDID_CUSTOMER_SERIAL_INFO        = 0xFF, /*Display Product Serial Number*/

} EDID_CUSTOMER_TYPE_E;

typedef enum tagEDID_EXTENSION_TYPE_E
{
    EDID_EXTENSION_RESERVED = 0X00,
    EDID_EXTENSION_AUDIO,
    EDID_EXTENSION_VIDEO,
    EDID_EXTENSION_VENDOR,               /*Vendor Specific*/
    EDID_EXTENSION_SPEAKER,
    EDID_EXTENSION_VESA_DTC,
    EDID_EXTENSION_UET       = 0X07,     /*Use Extended Tag*/

} EDID_EXTENSION_TYPE_E;

/**定义音频压缩方式枚举*/
typedef enum tagEDID_AUDIO_COMPRESSED_TYPE_E
{
    EDID_AUDIO_UNCOMPRESSED  = 0x00,
    EDID_AUDIO_COMPRESSED_PCM,
    EDID_AUDIO_COMPRESSED_AC3,
    EDID_AUDIO_COMPRESSED_MPEG1,
    EDID_AUDIO_COMPRESSED_MP3,
    EDID_AUDIO_COMPRESSED_MPEG2,
    EDID_AUDIO_COMPRESSED_AAC,
    EDID_AUDIO_COMPRESSED_DTS,
    EDID_AUDIO_COMPRESSED_ATRAC,
    EDID_AUDIO_COMPRESSED_1BIT,
    EDID_AUDIO_COMPRESSED_DOLBY,
    EDID_AUDIO_COMPRESSED_DTS_HD,
    EDID_AUDIO_COMPRESSED_MLP,
    EDID_AUDIO_COMPRESSED_DST,
    EDID_AUDIO_COMPRESSED_WMA,
    EDID_AUDIO_CODE_BUTT,

} EDID_AUDIO_COMPRESSED_TYPE_E;

/*Supported Aspect Ratios*/
typedef enum tagEDID_ASPECT_RATIOS_E
{
    EDID_ASPECT_RATIOS_159    = 0x01 << 0,          /*15:9*/
    EDID_ASPECT_RATIOS_54     = 0x01 << 1,          /*5:4*/
    EDID_ASPECT_RATIOS_1610   = 0x01 << 2,          /*16:10*/
    EDID_ASPECT_RATIOS_169    = 0x01 << 3,          /*16:9*/
    EDID_ASPECT_RATIOS_43     = 0x01 << 4,          /*4:3*/

} EDID_ASPECT_RATIOS_E;
/*Type CVT Blanking Support:*/
typedef enum tagEDID_BLANKING_SUPPORT_E
{
    EDID_BLANKING_NO_SUPPORT   = 0x00,          /*No Support*/
    EDID_BLANKING_SUPPORT_STANDARD   ,          /*Standard*/
    EDID_BLANKING_SUPPORT_REDUCED    ,          /*Reduced */
    EDID_BLANKING_SUPPORT_ALL        ,          /*Standard and Reduced Support*/

} EDID_BLANKING_SUPPORT_E;

typedef struct tagEDID_COLOR_CHARA_S				/* Color characteristics */
{
    HI_FLOAT floatRedX;
    HI_FLOAT floatRedY;
    HI_FLOAT floatGreenX;
    HI_FLOAT floatGreenY;
    HI_FLOAT floatBlueX;
    HI_FLOAT floatBlueY;
    HI_FLOAT floatWhiteX;
    HI_FLOAT floatWhiteY;
} EDID_COLOR_CHARA_S;

typedef struct tagEDID_SIGNAL_INFO_OTHER_S		/* Basic Display Parameters */
{
    HI_BOOL bBlank2Black;						/*display expects a blank-to-black setup*/
    HI_BOOL bSeparateSyncs;
    HI_BOOL bCompositeSync;
    HI_BOOL bGreenSync;							/*sync. on green video*/
    HI_BOOL bSerrationVsyncs;					/*serration of the Vsync.*/
    HI_BOOL bStandby;							/*bSuspend supported*/
    HI_BOOL bSuspend;							/*bSuspend supported*/
    HI_BOOL bActiveOff;							/*Active Off/Very Low Power*/
    HI_BOOL bsRGB;								/*Standard Default Color Space, sRGB*/
    HI_BOOL bPreferredTiming;					/*Preferred Timing*/
    HI_BOOL bDefaultGTF;						/*Default GTF supported*/

} EDID_SIGNAL_INFO_OTHER_S;

typedef struct tagEDID_BASE_PARA_S					/* Basic Display Parameters */
{
    EDID_SIGNAL_TYPE_E			enInputSignalType;	/*input signal type*/
    EDID_SIGNAL_LEVEL_STD_E		enSignalLevel;	/*Signal Level Standard*/
    EDID_DISPLAY_TYPE_E			enDisplayType;	/*Display Type*/
    EDID_SIGNAL_INFO_OTHER_S		stFeatureSupport;
    HI_U8                               u32ImgeWMax;/*unit : cm*/
    HI_U8                               u32ImgeHMax;/*unit : cm*/
    HI_U32                            u32Gamma;   /*value*100 */
    EDID_COLO_DEPTH_E            enColorDepth;     /* 支持的颜色BIT EDID 1.4 Add*/
    EDID_COLO_ENCODING_E         enColorEncoding;  /* 支持的颜色编码方式 EDID 1.4 Add*/

} EDID_DISP_BASE_PARA_S;

typedef struct tagEDID_VERSION_S				/* EDID u8Version */
{
    HI_U8 u8Version;
    HI_U8 u8Revision;
} EDID_VERSION_S;

typedef struct tagEDID_VENDOR_S	                /* Vendor & product info */
{
    HI_CHAR	cMfName[3];						/*Manufacturer name*/
    HI_U16	u16ProductCode;
    HI_U32	u32SerialNum;		                            /* FIXME: byte order */
    HI_U8	u32MfWeek;
    HI_U32	u32MfYear;
    HI_CHAR* pMonitorName;						/*monitor name*/
    HI_CHAR*	pMonitorSerialNum;						/*monitor Serial number*/

} EDID_VENDOR_S;

typedef struct tagEDID_DETAILED_STRING_S
{
    HI_CHAR string[20];
} __attribute__((packed)) EDID_DETAILED_STRING_S;

/*GTF Secondary Curve Block Definition*/
typedef struct tagEDID_GTF_SECONDARY_S
{
    HI_U8 u8HFreqStart_KHz;						/* need to multiply by 2 */
    HI_U8 u8C;									/* need to divide by 2 */
    HI_U16 u16M;
    HI_U8 u8K;
    HI_U8 u8J;									/* need to divide by 2 */
} EDID_GTF_SECONDARY_S;

/*CVT Support Definition*/
typedef struct tagEDID_CVT_DEFINITION_S
{
    HI_U8                           u8Version;	    	/* Version Number: Set to 0x03 */
    HI_U16                          u16PixelsPerLine;           /*Maximum Active Pixels per Line*/
    HI_U8                           u8ARSupport;         /*Support Aspect Ratios*/
    HI_U8                           u8ScalingSupport;    /*Type of Display Scaling Supported*/
    EDID_ASPECT_RATIOS_E     enAspRatioPref;       /*Preferred Aspect Ratio:*/
    EDID_BLANKING_SUPPORT_E  enBlankingType;		/* need to multiply by 10 */
} EDID_CVT_DEFINITION_S;

#define DETAILED_RANGE_INDICATE_SECONDARY_GTF    0x02          /*0x20 Secondary GTF supported*/
#define DETAILED_RANGE_INDICATE_CVT   0x04           /*0x40 CVT supported*/

typedef struct tagEDID_DETAILED_RANGE_S
{
    HI_U16 u32VFreqMin_KHz;
    HI_U16 u32VFreqMax_KHz;
    HI_U16 u32HFreqMin_KHz;
    HI_U16 u32HFreqMax_KHz;
    HI_U32 u32PixClkMax_KHz;
    HI_U8  u8Indicate;						        /* indicate :0x20 Secondary GTF supported; :0x40 CVT supported*/
    union
    {
        EDID_GTF_SECONDARY_S     stGTFInfo;
        EDID_CVT_DEFINITION_S    stCVTInfo;
    } data;
} EDID_DETAILED_RANGE_S;


/*Additional Color Point Descriptor Definition*/
typedef struct tagEDID_DETAILED_COLOR_POINT_S
{
    HI_FLOAT            floatWhiteX;
    HI_FLOAT            floatWhiteY;
    HI_FLOAT            floatGamma;								/* need to divide by 100 then add 1 */
} EDID_DETAILED_COLOR_POINT_S;

/*Color Management Data Descriptor Definition*/
typedef struct tagEDID_DETAILED_COLOR_MANAGEMENT_S
{
    HI_U8  u8Version;								/* Version Number: Set to 0x03 */
    HI_U16 u16RedA3;
    HI_U16 u16RedA2;
    HI_U16 u16GreenA3;
    HI_U16 u16GreenA2;
    HI_U16 u16BlueA3;
    HI_U16 u16BlueA2;
} __attribute__((packed)) EDID_DETAILED_COLOR_MANAGEMENT_S;


typedef struct tagEDID_CVT_TIMING_S
{
    HI_U8 code[3];
} __attribute__((packed)) EDID_CVT_TIMING_S;

typedef struct tagEDID_STD_TIMING_S
{
    HI_U8 u8HSize;										/* need to multiply by 8 then add 248 */
    HI_U8 u8VfreqAspect;
} __attribute__((packed)) EDID_STD_TIMING_S;

typedef struct tagEDID_CUSTOMER_OTHER_S				/* EDID CUSTOMER OTHER*/
{
    struct list_head                    head;								/* Header */
    EDID_CUSTOMER_TYPE_E			enCustomType;
    union
    {
        EDID_DETAILED_STRING_S	    stString;
        EDID_DETAILED_RANGE_S	    stDetailFreqRange;
        EDID_DETAILED_COLOR_POINT_S	stColor[2];
        EDID_DETAILED_COLOR_MANAGEMENT_S stColorManage;
    } data;
} EDID_CUSTOMER_OTHER_S;


/*HDMI */


/**定义HDMI EDID视频信息*/
typedef struct tagEDID_VIDEO_TIMING_S
{
    struct list_head            head;
    HI_U16                      u32Hact;
    HI_U16                      u32Vact;
    HI_U16                      u32VerFreq;
    HI_BOOL                     bProgressive;
    HI_U32                    u32AspectWidth;
    HI_U32                    u32AspectHeight;
    HI_UNF_EDID_TIMING_ATTR_E            enTimingAttr;
    HI_UNF_EDID_TIMING_TYPE_E            enTimingType;
} EDID_VIDEO_TIMING_S;



/**定义HDMI EDID音频信息*/
typedef struct tagEDID_AUDIO_S
{
    struct list_head head;
    EDID_AUDIO_COMPRESSED_TYPE_E enAudioCompressType;
    HI_U8 u8AudioChannel;                                /*audio_channel*/
    HI_U8 u8SampleRate;                                  /*sample rate*/
    HI_U8 u8SampleDepth;                                    /*sample bit depth*/
    HI_U16 u16BitRateMax;                             /*enter max bit rate*/

} EDID_AUDIO_S;


/**定义HDMI EDID其他信息*/
typedef struct tagEDID_EXTENSION_OTHER_S
{
    struct list_head head;
    EDID_EXTENSION_TYPE_E enExtenType;
    HI_U8 u8Length;
    HI_U8* pu8Data;

} EDID_EXTENSION_OTHER_S;

typedef struct tagEDID_NATIVE_FORMATS_S
{
    HI_BOOL bSupportUnderScan;						/*sUnderscan is supported*/
    HI_BOOL bSupportAudiob;						    /*Basic Audio is supported*/
    HI_BOOL bSupportYPbPr444;						    /*supported YPbPr4:4:4*/
    HI_BOOL bSupportYPbPr422;						    /*supported YPbPr4:2:2*/

} EDID_NATIVE_FORMATS_S;

typedef struct tagEDID_LATENCY_PRESENT_S
{
    HI_U16  u16VideoLatency;     /**/
    HI_U16  u16AudioLatency;     /**/

} EDID_LATENCY_PRESENT_S;

typedef struct tagEDID_HDMI_PRESENT_S
{
    HI_BOOL b3DPresent;     /**/
    HI_U8   u8HDMI_3DLen;    /**/
    HI_U8   u8HDMI_VICLen;   /**/
    HI_U8*   pu8Data;        /*HDMI present EDID data address*/

} EDID_HDMI_PRESENT_S;

/*HDMI Support Definition*/
typedef struct tagEDID_VIDEO_FORMATS_S
{
    HI_BOOL                         bDVI_Dual;		/*Sink supports DVI dual-link operation*/
    HI_BOOL                         bDC_Y444;		/*Sink supports YCBCR 4:4:4 in Deep Color modes*/
    HI_BOOL                         bDC_30bit;		/*Sink supports 30 bits/pixel (10 bits/color)*/
    HI_BOOL                         bDC_36bit;		/*Sink supports 30 bits/pixel (12 bits/color)*/
    HI_BOOL                         bDC_48bit;		/*Sink supports 30 bits/pixel (16 bits/color)*/
    HI_BOOL                         bSupportsAI;	/*Sink supports ACP/ISRC1 or ISRC2 packets*/
    HI_U16                          u16TMDS_ClkMax;	/*maximum TMDS clock MHz*/
    HI_BOOL                         bCNC[4];         /*CNC0: Graphics; CNC1: Photo; CNC2: Cinema; CNC3: Game ; */
    EDID_LATENCY_PRESENT_S*   pstFieldsLatencyPresent;     /*Latency Fields Present*/
    EDID_LATENCY_PRESENT_S*   pstIntelaceFieldsLatencyPresent;     /*interlaced Latency Fields_Present*/
    EDID_HDMI_PRESENT_S*      stHDMIVideoPresent;     /*HDMI Video present*/

} EDID_VIDEO_FORMATS_S;

typedef struct tagEDID_EXTENSION_INFO
{
    HI_U8                           block;
    HI_U8                           u8Version;
    HI_U16                          u16CEC_ID;
    HI_U16                          u8DetailedTimingNum;
    EDID_NATIVE_FORMATS_S    stNativeFormats;
    HI_U32							u32Speaker;             /*supported  u32Speaker*/
    EDID_AUDIO_S*             pstAudioInfo;
    EDID_VIDEO_TIMING_S*       pstVideoTiming;
    EDID_VIDEO_FORMATS_S*      pstVideoFormat;
    EDID_EXTENSION_OTHER_S*   pstExitensionOther;

} EDID_EXTENSION_INFO;


typedef struct tagEDID_INFO_S
{
    HI_UNF_EDID_VIDEO_PORT_E        enVideoPort;                  /*gear into video port  */
    
    EDID_SIMPLE_TIMING_S*			pstSimpleTimingList;	/* Est. stTimings and mfg rsvd stTimings*/
    HI_U32                          u32SimpleTimingNum; 
    EDID_DETAIL_TIMING_S*	pstDetailTimingList;	/* Detailing stTimings  */
    HI_U32                          u32DetailTimingNum;
    EDID_VENDOR_S			stVendor;				/* Vendor & product info */
    
    EDID_VERSION_S			stVersion;			/* EDID Version */
    EDID_DISP_BASE_PARA_S	stDevDispInfo;		/* Display info: */
    EDID_COLOR_CHARA_S		stColorPara;		/* Color characteristics */

    EDID_CUSTOMER_OTHER_S*	pstCustomerOther;	/* Detailing stTimings  */
    EDID_EXTENSION_INFO*      pstExitenInfo; /* EDID 2.0 HDMI Descriptors*/
} EDID_INFO_S;


HI_S32 HI_MPI_EDID_EdidParse(HI_U8 *u8Edid, HI_U32 u32EdidLength, EDID_INFO_S *pEdidInfo);

HI_VOID HI_MPI_EDID_EdidRelease(EDID_INFO_S *pstEdidInfo);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

