/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file
*
* @brief Rx/Tx Shared interface declarations.
*
*****************************************************************************/

#ifndef __SI_DRV_COMMON_API_H__
#define __SI_DRV_COMMON_API_H__

/***** #include statements ***************************************************/

/***** public macro definitions **********************************************/
#define SII_INFOFRAME_MAX_LEN           31
#define SII_EDID_BLOCK_SIZE             128
#define SII_EDID_TOTAL_BLOCKS           4
#define SII_EDID_MAX_LEN                (SII_EDID_TOTAL_BLOCKS * SII_EDID_BLOCK_SIZE)
#define SII_BKSV_LIST_BYTES             5
#define SII_HDCP2X_RCVID_LENGTH         5
/***** public type definitions ***********************************************/
/**
* @brief HDCP Status
*/typedef enum
{
    SII_HDCP14_IDEL,
    SII_HDCP14_INIT,
    SII_HDCP14_RESERVE,
    SII_HDCP14_CONFIG_AV,
    SII_HDCP14_HDCP_PREP,
    SII_HDCP14_HDCP_AUTH1,
    SII_HDCP14_HDCP_AUTH2,
    SII_HDCP14_HDCP_AUTH3,
    SII_HDCP14_AUTH_FAIL,
}SiiHdcp14Status_E;

/**
* @brief TMDS Modes
*/typedef enum
{
	SII_TMDS_MODE__NONE,
	SII_TMDS_MODE__DVI,
	SII_TMDS_MODE__HDMI1,
	SII_TMDS_MODE__HDMI2,
	SII_TMDS_MODE__AUTO
} SiiTmdsMode_t;

/**
* @brief Interrupt type
*/typedef enum
{
	SII_INT_TYPE__NONE,
	SII_INT_TYPE__HPD_RSEN,
	SII_INT_TYPE__HDCP,
	SII_INT_TYPE__HDCP2x,
	SII_INT_TYPE__CEC,
} SiiInterruptType_t;

/**
* @brief Reset type
*/typedef enum
{
	SII_RESET_TYPE__NONE,
	SII_RESET_TYPE__SOFTWARE,
	SII_RESET_TYPE__AUDIO,
	SII_RESET_TYPE__TPI,
	SII_RESET_TYPE__HDCP2x,
	SII_RESET_TYPE__CEC,
} SiiResetType_t;


/**
* @brief Horizontal and Vertical Sync Polarities
*/typedef enum
{
	SII_HV_SYNC_POL__HPVP,
	SII_HV_SYNC_POL__HPVN,
	SII_HV_SYNC_POL__HNVP,
	SII_HV_SYNC_POL__HNVN,
	SII_HV_SYNC_POL__BUTT,
} SiiHvSyncPol_t;

/**
* @brief Horizontal and Vertical Sync Polarities
*/typedef struct
{
	bool_t  bVSyncPol;
	bool_t  bHSyncPol;
	bool_t  bCSyncPol;
	bool_t  bDEPol;
	bool_t  bSwapHsCs;
} SiiOutPutSyncCfg_t;

/**
* @brief Info Frame IDs
*/typedef enum
{
	SII_INFO_FRAME_ID__AVI,
	SII_INFO_FRAME_ID__AUDIO,
	SII_INFO_FRAME_ID__VS,
	SII_INFO_FRAME_ID__SPD,
	SII_INFO_FRAME_ID__GBD,
    SII_INFO_FRAME_ID__DRM,
	SII_INFO_FRAME_ID__MPEG,
	SII_INFO_FRAME_ID__ISRC,
	SII_INFO_FRAME_ID__ISRC2,
	SII_INFO_FRAME_ID__GCP,
	SII_INFO_FRAME_ID__ACP,

} SiiInfoFrameId_t;

/**
* @brief Audio Sampling Frequrency
*/
typedef enum
{
	SII_AUDIO_FS__22_05KHZ,
	SII_AUDIO_FS__24KHZ,
	SII_AUDIO_FS__32KHZ,
	SII_AUDIO_FS__44_1KHZ,
	SII_AUDIO_FS__48KHZ,
	SII_AUDIO_FS__88_2KHZ,
	SII_AUDIO_FS__96KHZ,
	SII_AUDIO_FS__176_4KHZ,
	SII_AUDIO_FS__192KHZ,
	SII_AUDIO_FS__768KHZ
} SiiAudioFs_t;

/**
* @brief Audio Channel Layout
*/
typedef enum
{
	AUDIO_FORMAT__2CH = 0x02,
	AUDIO_FORMAT__3CH,
	AUDIO_FORMAT__4CH,
	AUDIO_FORMAT__5CH,
	AUDIO_FORMAT__6CH,
	AUDIO_FORMAT__7CH,
	AUDIO_FORMAT__8CH
} SiiAudioCh_t;

/**
* @brief Rgb Yuv Convertion,add by 00273561
*/
typedef enum
{
    SII_COLORSPACE_NO_CONVERT,
	SII_COLORSAPCE_RGB2YUV,
	SII_COLORSAPCE_YUV2RGB,
} SiiColorConvertion_t;


/**
* @brief phy parameter,add by 00273561
*/
typedef struct{

    uint8_t     u8_data_swing;            //data-swing
    uint8_t     u8_clk_swing;             //CLK-swing
    uint8_t     u8_src_termination;       //source termination
    uint8_t     u8_drv_vnb_value;         //Tx driver vnb value setting	
    uint8_t     u8_clk_src_fine_djust;    // Fine adjust clock source termination
	uint8_t		u8_pll_band;			  // PLL band
	uint8_t		u8_pll_loop;			  // PLL loop
	uint8_t		u8_pll_vco;			  	  // PLL vco
	uint8_t		u8_pll_pi;			  	  // PLL pi
	uint8_t		u8_brg_bias; 		  	  // brg bias
	uint8_t		u8_data_fine_djust; 	  // Fine adjust data source termination
	uint8_t		u8_data_drv_ctrl; 	      // data drv ctrl
	uint8_t		u8_pll_ldo; 	     	  // pll ldo
	uint8_t		u8_pll_zone; 	     	  // pll zone
	uint8_t		u8_pll_charge; 	     	  // pll charge	
	uint8_t		u8_in_brg; 	     	  	  // in brg
    uint8_t     u8_tx_reg_zone;          // tx_reg_zone
    uint8_t     u8_rise_time;             //rise time
    uint8_t     u8_fall_time;             // fall time

}SiiDrvPhyPara_t;



/**
* @brief HDCP Status
*/
typedef enum
{
	SII_DRV_HDCP_STATUS__OFF,                //!< Authentication is not enabled
	SII_DRV_HDCP_STATUS__SUCCESS_1X,         //!< Authentication succeeded for HDCP 1.X
	SII_DRV_HDCP_STATUS__SUCCESS_22,         //!< Authentication succeeded for HDCP 2.2
	SII_DRV_HDCP_STATUS__AUTHENTICATING,     //!< Authentication is in progress
	SII_DRV_HDCP_STATUS__FAILED,             //!< Authentication failed and does not re-try
} SiiDrvHdcpStatus_t;

/**
* @brief Downstream HDCP Failure Reason
*/
typedef enum
{
	SII_DRV_HDCP_FAILURE__NONE,              //!< No failure detected so far
	SII_DRV_HDCP_FAILURE__NACK,              //!< Downstream device does not acknowledge HDCP registers; firmware continues trying
	SII_DRV_HDCP_FAILURE__DEV_EXC,           //!< Too many devices; firmware does not try until HPD Low to High transition
	SII_DRV_HDCP_FAILURE__CAS_EXC,           //!< Cascade exceeded error; firmware does not try until HPD Low to High transition
	SII_DRV_HDCP_FAILURE__V,                 //!< V verification failed; firmware continues trying
	SII_DRV_HDCP_FAILURE__TIMEOUT,           //!< Authentication timed out; firmware continues trying
	SII_DRV_HDCP_FAILURE__OTHER,             //!< Other authentication errors; firmware continues trying
} SiiDrvHdcpFailure_t;


/**
* @brief HDCP 2.2 content type
*/
typedef enum
{
	SII_DRV_HDCP_CONTENT_TYPE__0,            //!< HDCP for content type 0
	SII_DRV_HDCP_CONTENT_TYPE__1             //!< HDCP for content type 1
} SiiDrvHdcpContentType_t;

/**
* @brief HDCP2x core code update status
*/
typedef enum
{
    SII_DRV_HDCP2X_CUPD_CHK__DONE,		     //!< Code Update Done
    SII_DRV_HDCP2X_CUPD_CHK__FAIL,		     //!< Code Updated Failed
    SII_DRV_HDCP2X_CUPD_CHK__ERROR,	         //!< Code Udated Timedout
}SiiDrvHdcp2xCupdChkStat_t;

/**
* @brief Error type returned by BKSV/RxID reading functions
*/
typedef enum
{
	SII_DRV_HDCP_KSV_LOAD__OK,               //!< Success
	SII_DRV_HDCP_KSV_LOAD__NOT_AVAILABLE,    //!< BKSV/RxID list was attempted to be read
	                                         //!< before authentication or after failure
	SII_DRV_HDCP_KSV_LOAD__BUFFER_ERROR,     //!< Buffer loading error
} SiiDrvHdcpKsvLoadError_t;

/**
* @brief Video Color formats
*/
typedef enum
{
    SII_DRV_CLRSPC__PASSTHRU,

    SII_DRV_CLRSPC__YC444_601,
    SII_DRV_CLRSPC__YC422_601,
    SII_DRV_CLRSPC__YC420_601,

    SII_DRV_CLRSPC__YC444_709,
    SII_DRV_CLRSPC__YC422_709,
    SII_DRV_CLRSPC__YC420_709,

    SII_DRV_CLRSPC__XVYCC444_601,
    SII_DRV_CLRSPC__XVYCC422_601,
    SII_DRV_CLRSPC__XVYCC420_601,

    SII_DRV_CLRSPC__XVYCC444_709,
    SII_DRV_CLRSPC__XVYCC422_709,
    SII_DRV_CLRSPC__XVYCC420_709,

    SII_DRV_CLRSPC__YC444_2020,
    SII_DRV_CLRSPC__YC422_2020,
    SII_DRV_CLRSPC__YC420_2020,

    SII_DRV_CLRSPC__RGB_FULL,
    SII_DRV_CLRSPC__RGB_LIMITED
} SiiDrvClrSpc_t;

/**
* @brief Video Conversion Standard
*/
typedef enum
{
	SII_DRV_CONV_STD__BT_709,
	SII_DRV_CONV_STD__BT_601,
	SII_DRV_CONV_STD__BT_2020_NON_CONST_LUMINOUS,
	SII_DRV_CONV_STD__BT_2020_CONST_LUMINOUS
}SiiDrvConvStd_t;

/**
* @brief Video bit depth
*/
typedef enum
{
    //SII_DRV_BIT_DEPTH__PASSTHOUGH,
    SII_DRV_BIT_DEPTH__8_BIT,
    SII_DRV_BIT_DEPTH__10_BIT,
    SII_DRV_BIT_DEPTH__12_BIT,
    SII_DRV_BIT_DEPTH__16_BIT
} SiiDrvBitDepth_t;

typedef struct
{
	uint8_t              reg0;
	uint8_t              reg1;
} IntStat_t;

/**
* @brief EDID buffer
*/typedef struct
{
    uint16_t         validLen;
	uint8_t          b[SII_EDID_MAX_LEN];
} SiiEdid_t;

/**
* @brief EDID's LipSync Info
*/
#if 1
typedef struct
{
	bool_t latencyPresent;
	bool_t ILatencyPresent;
	uint8_t videoLatency;
	uint8_t audioLatency;
	uint8_t IVideoLatency;
	uint8_t IAudioLatency;
} SiiLipSyncInfo_t;
#endif
/**
* @brief Info Frames
*/typedef struct
{
	SiiInfoFrameId_t ifId;
	uint8_t          b[SII_INFOFRAME_MAX_LEN];
} SiiInfoFrame_t;

/**
* @brief Channle Status
*/typedef struct
{
	uint8_t          i2s_chst0;
	uint8_t          i2s_chst1;
	uint8_t          i2s_chst2;
	uint8_t          i2s_chst3;
	uint8_t          i2s_chst4;
	uint8_t          i2s_chst5;
	uint8_t          i2s_chst6;
} SiiChannelStatus_t;

/**
* @brief Audio Format type
*/typedef struct
{
	SiiAudioFs_t   audioFs;
	uint32_t       n;
	uint32_t       cts;
	uint8_t        layout1;
	uint8_t        dsd;
	uint8_t        hbrA;
	uint8_t        spdif;
	uint8_t        i2s;
	uint8_t        downSample;
} SiiAudioFormat_t;

/**
* @brief KSV data object
*/
typedef struct
{
	uint8_t  d[SII_BKSV_LIST_BYTES];
} SiiDrvHdcpKsv_t;

/**
* @brief KSV data object
*/
typedef struct
{
	uint16_t        length;
	uint8_t*  		pList;
	uint8_t*		pListStart;
} SiiDrvHdcpKsvList_t;

/**
* @brief Downstream device HDCP topology information
*/
typedef struct
{
	uint8_t         deviceCount;		//!< Total number of attached HDCP devices.
	uint8_t         depth;				//!< Number of attached repeater levels.
	uint8_t         maxDevsExceeded;	//!< \c 1: more than supported number of devices attached.
										//!< \c 0: \c deviceCount is valid.
	uint8_t         maxCascadeExceeded; //!< \c 1: more than supported number of levels attached.
										//!< \c 0: \c depth is valid.
	uint8_t         hdcp20RepeaterDs;	//!< \c 1: there is an HDCP 2.0 compliant repeater in the topology.
										//!< \c 0: there is no HDCP 2.0 compliant repeaters in the topology.
	uint8_t         hdcp1xRepeaterDs;	//!< \c 1: there is an HDCP 1.x compliant repeater in the topology.
										//!< \c 0: there is no HDCP 1.x compliant repeaters in the topology.
} SiiDrvHdcpTopology_t;

typedef struct
{
    uint8_t             tmdsBitClkRatio;
    bool_t              bSourceScrambleOn;
    bool_t              bSinkScrambleOn;
}SiiScdcStatus_t;

typedef struct
{
    bool_t              bHdcp2_2Enable;
    bool_t              bHdcp1_4Enable;
    bool_t              bRepeaterOn;
    uint8_t             BKSVs[5];
    uint8_t             AKSVs[5];
    uint8_t             hdcpStatus;
    //SiiDrvTxHdcpInternalState_t  authState;
}SiiHdcpStatus_t;

typedef struct
{
    uint8_t              u8DataSwing;      //data-swing
    uint8_t              u8CLKSwing;       //CLK-swing
    uint8_t              u8SrcTermination; //source termination
    uint8_t              u8TxVnbValue;     //Tx driver vnb value setting 
    uint8_t              u8AdjustClk;      //Fine adjust clock source termination 
    uint8_t              u8TxRegZone;      //tx_reg_zone
}SiiPhyConfig_t;

typedef struct
{
    uint8_t              hotPlug;
    uint8_t              rsen;
    bool_t               phyOutput;
    uint8_t              phyPower;
    uint8_t              phyBitDepth;
    uint8_t              outBitDepth;
    uint8_t              tmdsMode;
    uint8_t              avMute;
    uint8_t              audioMute;
    uint8_t              audioEnable;
    SiiAudioFormat_t     audioFmt;
    SiiChannelStatus_t   channelStatus;
	uint8_t				 bIfOn[SII_INFO_FRAME_ID__MPEG + 1];
    SiiInfoFrame_t       infoframe[SII_INFO_FRAME_ID__MPEG + 1];
    SiiPhyConfig_t       PhyConfig;
}SiiHdmiStatus_t;

typedef enum
{
    DITHER__12_TO_10,
    DITHER__12_TO_8,
    DITHER__10_TO_8,
    DITHER__NOCHNG,
}SiiDitherCfg_t;

typedef enum
{
    QUANTIZATION_VIDEO_LEVELS, // 16 - 235
    QUANTIZATION_PC_LEVELS // 0 - 255
}QuantizationLevel_t;

typedef struct 
{
	bool_t              bVideoMute;
    bool_t              in420Ydemux;
    bool_t              out420Ydemux;
    bool_t              YUV2RGB;
    bool_t              RGB2YUV;
    bool_t              YUV420_422;
    bool_t              YUV422_444;
    bool_t              YUV444_422;
    bool_t              YUV422_420; 
    bool_t              dither;
	bool_t              bVSyncPol;
	bool_t              bHSyncPol;
	bool_t              bCSyncPol;
	bool_t              bDEPol;
	bool_t              bSwapHsCs;
    SiiHvSyncPol_t      hvSyncPol;
    SiiDitherCfg_t		OutDitherMode;
    QuantizationLevel_t eCscOutQuan;
}SiiVideoStatus_t;


typedef struct 
{
	uint16_t            u16Y_G;
	uint16_t            u16Cb_B;
	uint16_t            u16Cr_R;
}SiiBlacFramekInfo_t;


typedef struct 
{
	bool_t  bBlacEnable;
    uint8_t inColorSpace;   
    uint8_t inBitDepth;
    uint8_t inQuantization;	
}SiiBlackPramInfo_t;


typedef struct 
{
	bool_t  bHdcp14;
    bool_t  bHdcp22;
}SiiHdcpCaps_t;

typedef struct 
{
    SiiHdmiStatus_t  hdmiStatus;
    SiiVideoStatus_t videoStatus;
    SiiHdcpStatus_t  hdcpStatus;
}SiiHardwareStatus_t;

typedef struct 
{
    bool_t              bTimerStart;
    uint32_t            u32Time;
}SiiHdrTimerCfg_S;

/***** public functions ******************************************************/

void SiiDrvInfoFrameCalculateChecksum(SiiInfoFrame_t *pInfoFrame);
void SiiDrvClearInfoFrame(SiiInfoFrameId_t ifId, SiiInfoFrame_t *pInfoFrame);
void SiiDrvLogInfoFrame(SiiInfoFrame_t *pInfoFrame);

#endif // __SI_DRV_COMMON_API_H__
