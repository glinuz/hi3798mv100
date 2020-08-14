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
* @file si_drv_tx_api.h
*
* @brief Tx API
*
*****************************************************************************/
#ifndef __SI_DRV_TX_API_H__
#define __SI_DRV_TX_API_H__

/***** #include statements ***************************************************/
#include "si_lib_log_api.h"
#include "si_lib_obj_api.h"
#include "si_drv_common.h"
#include "si_drv_cra_api.h"
#include "si_mod_videopath_api.h"
#include "si_mod_tx_hdmi_api.h"
#ifdef HISI_TX_MHL
#include "si_mod_tx_mhl_api.h"
#endif
#ifndef HDMI_BUILD_IN_BOOT
#include "si_mod_tx_hdcp_api.h"
#endif

/***** public macro definitions **********************************************/
#define SII_DRV_TX_BASE_ADDR_CYPRESS			0x3000

#define SII_DRV_TX_EVENT__HOT_PLUG_CHNG			0x00000001		//!< Hot-Plug status is changed
#define SII_DRV_TX_EVENT__RSEN_CHNG				0x00000002		//!< Rsen status is changed
#define SII_DRV_TX_EVENT__HDMI_STATE_CHNG		0x00000004		//!< HDMI state is changed
#define SII_DRV_TX_EVENT__HDCP_STATE_CHNG		0x00000008		//!< HDCP status changed
#define SII_DRV_TX_EVENT__MHL_STATE_CHNG		0x00000010		//!< MHL State is changed
#define SII_DRV_TX_EVENT_MHL_RCP_RECEIVED		0x00000020		//!< MSC_MSG_RCP  Received
#define SII_DRV_TX_EVENT_MHL_RCPK_RECEIVED		0x00000040		//!< MSC_MSG_RCPK Received
#define SII_DRV_TX_EVENT_MHL_RCPE_RECEIVED		0x00000080		//!< MSC_MSG_RCPE Received
#define SII_DRV_TX_EVENT_MHL_RAP_RECEIVED		0x00000100		//!< MSC_MSG_RAP Received
#define SII_DRV_CEC_EVENT						0x00000200		//!< CEC even happend
#define SII_DRV_TX_EVENT_SCRAMBLE_SUCCESS		0x00000400		//!< SCRAMBLE Success
#define SII_DRV_TX_EVENT_SCRAMBLE_FAIL		    0x00000800		//!< SCRAMBLE Fail
#define SII_DRV_TX_EVENT_ZERO_DRMIF_TIMEOUT     0x00001000      //!< ZERO_DRMIF_TIMEOUT
#define SII_DRV_TX_EVENT_HDRMODE_CHANGE_TIMEOUT 0x00002000      //!< HDRMODE_CHANGE_TIMEOUT


#define LEN_TPI__DDC_FIFO_SIZE              16
#define HDCP2X_CUPD_MAX_TIMEOUT				1000

#define TIMER_START__TX_INTR                150
#define TIMER_START__TX_INTR_PRI            255
#define TIMER_START__TX_INTR_REPEAT         10

#define TIMER_START__HW_UPDATE              1000
#define TIMER_START__HW_UPDATE_PRI			254
#define TIMER_START__HW_UPDATE_200MS_WAIT	200
#define TIMER_START__HW_UPDATE_50MS_WAIT	50
#define TIMER_START__HW_UPDATE_5MS_WAIT		5
#define TIMER_START__HW_UPDATE_100MS_WAIT   100

#define TIMER_START__AVMUTE_CLR_STOP        20
#define TIMER_START__AVMUTE_CLR_STOP_WAIT   280

#define MAX_DS_DEVICE						127

#define LEN_TPI__IF_BUFFER_LENGTH           31

#define TIMER_START_ZERO_DRM_IF             200
#define TIMER_START_ZERO_DRM_IF_WAIT        2000

#define TIMER_START_HDR_MODE_CHANGE         20
#define TIMER_START_HDR_MODE_CHANGE_WAIT    200


/***** public type definitions ***********************************************/

typedef uint32_t SiiDrvTxEvent_t;
typedef void (*TxcbFunc)(SiiInst_t, SiiDrvTxEvent_t);

/**
* @brief Video Pack Mode
*/
typedef enum
{
	SII_DRV_TX_PACK_MODE__8_BPP,         //!<
	SII_DRV_TX_PACK_MODE__10_BPP,        //!<
	SII_DRV_TX_PACK_MODE__12_BPP,        //!<
	SII_DRV_TX_PACK_MODE__16_BPP         //!<
} SiiDrvTxBitDepth_t;

/**
* @brief Video Color formats
*/
typedef enum
{
    SII_DRV_TX_CLRSPC__PASSTHRU,         //!<

    SII_DRV_TX_CLRSPC__YC444_601,        //!<
    SII_DRV_TX_CLRSPC__YC422_601,        //!<
    SII_DRV_TX_CLRSPC__YC420_601,        //!<

    SII_DRV_TX_CLRSPC__YC444_709,        //!<
    SII_DRV_TX_CLRSPC__YC422_709,        //!<
    SII_DRV_TX_CLRSPC__YC420_709,        //!<

    SII_DRV_TX_CLRSPC__XVYCC444_601,     //!<
    SII_DRV_TX_CLRSPC__XVYCC422_601,     //!<
    SII_DRV_TX_CLRSPC__XVYCC420_601,     //!<

    SII_DRV_TX_CLRSPC__XVYCC444_709,     //!<
    SII_DRV_TX_CLRSPC__XVYCC422_709,     //!<
    SII_DRV_TX_CLRSPC__XVYCC420_709,     //!<

    SII_DRV_TX_CLRSPC__YC444_2020,       //!<
    SII_DRV_TX_CLRSPC__YC422_2020,       //!<
    SII_DRV_TX_CLRSPC__YC420_2020,       //!<

    SII_DRV_TX_CLRSPC__RGB_FULL,         //!<
    SII_DRV_TX_CLRSPC__RGB_LIMITED       //!<
} SiiDrvTxClrSpc_t;

/**
* @brief Video Conversion Standard
*/
typedef enum
{
	SII_DRV_TX_CONV_STD__BT_709,
	SII_DRV_TX_CONV_STD__BT_601,
	SII_DRV_TX_CONV_STD__BT_2020_non_const_luminous,
	SII_DRV_TX_CONV_STD__BT_2020_const_luminous,
	SII_DRV_TX_CONV_STD__BUTT
}SiiDrvTxConvStd_t;

//! DDC bus access error codes
typedef enum ddcComErr
{
	SI_TX_DDC_ERROR_CODE_NO_ERROR    = 0x00, //!< Success
	SI_TX_DDC_ERROR_CODE_TIMEOUT     = 0x01, //!< DDC bus is not granted within timeout
	SI_TX_DDC_ERROR_CODE_NO_ACK      = 0x02, //!< No ACK from DDC device
	SI_TX_DDC_ERROR_CODE_BUSY        = 0x03, //!< DDC bus is busy
	SI_TX_DDC_ERROR_CODE_TX_HW       = 0x04,
	SI_TX_DDC_ERROR_CODE_LIM_EXCEED  = 0x05

} ddcComErr_t;

/**
* CBUS mode
*/
typedef enum
{
    SII_DRV_TX_CBUS_MODE__NO_CONNECTION,      //!< Cbus not connected
    SII_DRV_TX_CBUS_MODE__oCBUS_UNKNOWN_PEER, //!< Unknown peer
    SII_DRV_TX_CBUS_MODE__oCBUS_MHL1_2_PEER,  //!< MHL 1/2 peer
    SII_DRV_TX_CBUS_MODE__oCBUS_MHL3_PEER,    //!< MHL3 peer
    SII_DRV_TX_CBUS_MODE__TRANSITIONAL,       //!< CBUS transition mode
    SII_DRV_TX_CBUS_MODE__eCBUS_S,            //!< Single ended CBUS connection
    SII_DRV_TX_CBUS_MODE__eCBUS_D,            //!< Differential CBUS connection
}SiiDrvTxCbusMode_t;

/**
* Structure for Device Capabilities.
*/
typedef struct
{
	uint8_t state;
	uint8_t mhl_version;
	uint8_t deviceCategory;
	uint8_t adopterIdHigh;
	uint8_t adopterIdLow;
	uint8_t vid_link_mode;
	uint8_t audLinkMode;
	uint8_t videoType;
	uint8_t logicalDeviceMap;
	uint8_t bandWidth;
	uint8_t featureFlag;
	uint8_t deviceIdHigh;
	uint8_t deviceIdLow;
	uint8_t scratchPadSize;
	uint8_t int_state_size;
	uint8_t reserved;
} SiiDrvTxMHLDevCap_t;

/**
* @breif Video path Color Info configuration data
*/
typedef struct 
{
	SiiDrvTxConvStd_t   inputClrConvStd;
    SiiDrvTxClrSpc_t    inputClrSpc;
	SiiDrvBitDepth_t    inputVidDcDepth;
}SiiDrvTxColorInfoCfg_t;

typedef enum
{
    SII_DRV_TX_COLORSPACE_RGB,
    SII_DRV_TX_COLORSPACE_YCbCr422,
    SII_DRV_TX_COLORSPACE_YCbCr444,    
    SII_DRV_TX_COLORSPACE_YCbCr420,
    SII_DRV_TX_COLORSPACE_BUTT,
} SiiDrvTxColorSpace_E;

typedef enum
{
    SII_DRV_TX_QUANTIZATION_DEFAULT,
    SII_DRV_TX_QUANTIZATION_LIMITED,
    SII_DRV_TX_QUANTIZATION_FULL,
    SII_DRV_TX_QUANTIZATION_BUTT
} SiiDrvTxQuantization_E;

/**
* @breif Video path CSC Info configuration data
*/
typedef struct 
{
    SiiDrvTxColorSpace_E    enInClrSpc;
	SiiDrvTxConvStd_t       enClrConvStd;
    SiiDrvTxQuantization_E  enInQuan;
    SiiDrvTxColorSpace_E    enOutClrSpc;
    SiiDrvTxQuantization_E  enOutQuan;
}SiiDrvTxCscCfg_t;

/**
* Structure for Extended Device Capabilities.
*/
typedef struct {
	uint8_t ecbus_speeds;
	uint8_t tmds_speeds;
	uint8_t ecbus_dev_roles;
	uint8_t log_dev_mapx;
	uint8_t reserved_4;
	uint8_t reserved_5;
	uint8_t reserved_6;
	uint8_t reserved_7;
	uint8_t reserved_8;
	uint8_t reserved_9;
	uint8_t reserved_a;
	uint8_t reserved_b;
	uint8_t reserved_c;
	uint8_t reserved_d;
	uint8_t reserved_e;
	uint8_t reserved_f;
} SiiDrvTxMHLXDevCap_t;

/**
* @brief Static constructor configuration
*/
typedef struct
{
    SiiDrvCraAddr_t		baseAddr;
    uint32_t            *pu32CtrlVirBaseAddr;
    SiiInst_t           instCra;
	bool_t				bHdcp2xEn;
	bool_t				bMhlen;
	bool_t				bMhl3en;
	bool_t				bEsmcEn;
	bool_t				bMDTEn;
	bool_t				bProgramDino;
	bool_t				bVidPathEn;
	bool_t				bCpiEn;

} SiiDrvTxConfig_t;


typedef struct
{
    SiiResetType_t resetSrc;
    bool_t         reset;
}SiiReset_t;

typedef struct
{
    SiiInterruptType_t intSrc;
    bool_t             enable;
}SiiInterrupt_t;

typedef struct
{
    bool_t   bScdcEnable;
	bool_t   b3DOsdDisparity;        //!< 
	bool_t   bDualView;              //!< 
    bool_t   bIndependentView;       //!< 
	bool_t   bLTE340MscsScramble;    //!< 
	bool_t   bReadReqCapable;        //!< 
	bool_t   bScdcPresent;           //!< 
	bool_t   bDc30bit420;            //!< 
	bool_t   bDc36bit420;            //!< 
	bool_t   bDc48bit420;            //!<
    uint32_t vclk_mb;                //!< video clock supported by sink in MB
} SiiModTxScdcSinKCaps_t;

/***** call-back functions ***************************************************/

/*****************************************************************************/
/**
* @brief Tx Driver notification call-back register function
* @note  This function is called by application to register Tx driver 
*        notification call-back function
*
* @param[in]  inst        Handle to instance
* @param[in]  cbFunc      Pointer to call back function
* @param[in]  inst        call back function parameter
*
*****************************************************************************/
void SiiDrvTxRegisterCallBack(SiiInst_t inst, TxcbFunc cbFunc, SiiInst_t data);

/***** public functions ******************************************************/

/*****************************************************************************/
/**
* @brief Tx driver constructor
*
* @param[in]  pNameStr   Name of instance
* @param[in]  pConfig    Static configuration parameters
*
* @retval                Handle to instance
*
*****************************************************************************/
SiiInst_t SiiDrvTxCreate(char *pNameStr, SiiDrvTxConfig_t *pConfig);

/*****************************************************************************/
/**
* @brief Tx driver destructor
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiDrvTxDelete(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Tx Hardware initialize
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiDrvTxHardwareInit(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Tx Software initialize
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiDrvTxSoftwareInit(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Tx Hardware status get
*
* @param[in]  inst       Handle to instance
* @param[out] hwStatus	 Hardware status
*
*****************************************************************************/
void SiiDrvTxHardwareStatusGet(SiiInst_t inst, SiiHardwareStatus_t *hwStatus);
/*****************************************************************************/
/**
* @brief Tx Hardware status set
*
* @param[in]  inst       Handle to instance
* @param[out] pScdcStatus	 scdc status
*
*****************************************************************************/
void SiiDrvTxHwScdcStatusGet(SiiInst_t inst, SiiScdcStatus_t *pScdcStatus);

/*****************************************************************************/
/**
* @brief Tx Hardware status set
*
* @param[in]  inst       Handle to instance
* @param[in] pScdcStatus	 scdc status
*
*****************************************************************************/
void SiiDrvTxHwScdcStatusSet(SiiInst_t inst, SiiScdcStatus_t *pScdcStatus);


/*****************************************************************************/
/**
* @brief Software reset or clear reset.
*
* @param[in]  inst       Handle to instance
* @param[in]  resetSrc   Reset type
* @param[in]  onOff      Boolean to control Reset request 
*                        - @ref true   Reset Software.
*                        - @ref false  Clear Software Reset.
*
*****************************************************************************/
void SiiDrvTxReset(SiiInst_t inst, SiiResetType_t resetSrc, bool_t onOff);

void SiiDrvTxInterruptEnableSet(SiiInst_t inst, SiiInterruptType_t intSrc, bool_t onOff);
#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief Interrupt enable or disable.
*
* @param[in]  inst       Handle to instance
* @param[in]  intSrc     Interrupt type
* @param[in]  onOff      Boolean to control Interrupt request 
*                        - @ref true   Enable Interrupt.
*                        - @ref false  Disable Interrupt.
*
*****************************************************************************/
void SiiDrvTxInterruptEnableSet(SiiInst_t inst, SiiInterruptType_t intSrc, bool_t onOff);

/*****************************************************************************/
/**
* @brief Downstream Edid interrogation.
*
* @param[in]  inst       Handle to instance
* @param[out] pEdid      256 byte of EDID data
*
*****************************************************************************/
bool_t SiiDrvTxEdidGet(SiiInst_t inst, SiiEdid_t* pEdid);

/*****************************************************************************/
/**
* @brief Downstream Edid's LipSync interrogation.
*
* @param[in]  inst			Handle to instance
* @param[out] lipSync		Edid'd Lipsync Info
*
*****************************************************************************/
void SiiDrvTxLipSyncInfoGet(SiiInst_t inst, SiiLipSyncInfo_t* lipSync);

/*****************************************************************************/
/**
* @brief Hot-plug status.
*
* @param[in]  inst       Handle to instance
*
* @retval     #true      Hot-Plug is active
* @retval     #false     Hot-Plug is not active
*
*****************************************************************************/
bool_t SiiDrvTxHotPlugStatusGet(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief RSen (TMDS input impedance) status.
*
* @param[in]  inst       Handle to instance
*
* @retval     #true      Activate impedance.
* @retval     #false     Enable high impedance.
*
*****************************************************************************/
bool_t SiiDrvTxRsenStatusGet(SiiInst_t inst);
#endif
/*****************************************************************************/
/**
* @brief TMDS mode control.
*
* @param[in]  inst       Handle to instance
* @param[in]  tmdsMode   Can be chanaged to the following TMDS modes:
*                        - @ref SII_TMDS_MODE__OFF      Turn off TMDS signal off.
*                        - @ref SII_TMDS_MODE__AUTO     Enable TMDS output signal. Mode is automatically selected based on downstream EDID.
*                        - @ref SII_TMDS_MODE__DVI      Enable DVI compliant TMDS signal.
*                        - @ref SII_TMDS_MODE__HDMI1    Enable HDMI1 compliant TMDS signal.
*                        - @ref SII_TMDS_MODE__HDMI2    Enable HDMI2 compliant TMDS signal.
*
*****************************************************************************/
void SiiDrvTxTmdsModeSet(SiiInst_t inst, SiiTmdsMode_t tmdsMode);

#ifdef HDMI_SCDC_SUPPORT
/*****************************************************************************/
/**
* @brief scdc config.
*
* @param[in]  inst        Handle to instance
* @param[in]  scdcSinkCaps  Scdc Sink Caps
*
*****************************************************************************/
void SiiDrvTxScdcConfig(SiiInst_t inst, SiiModTxScdcSinKCaps_t *scdcSinkCaps);
#endif

#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief Current used TMDS mode status.
*
* @param[in]  inst      Handle to instance
*
* @retval               #SII_TMDS_MODE__OFF      TMDS output signal is turned off.
* @retval               #SII_TMDS_MODE__DVI      TMDS output signal is DVI compliant.
* @retval               #SII_TMDS_MODE__HDMI1    TMDS output signal is HDMI1 compliant.
* @retval               #SII_TMDS_MODE__HDMI2    TMDS output signal is HDMI2 compliant.
*
*****************************************************************************/
SiiTmdsMode_t SiiDrvTxTmdsModeStatusGet(SiiInst_t inst);
#ifdef HDMI_HDR_SUPPORT
/**
* @brief Zero DRM Infoframe timer start
*
* @param[in]  inst          Handle to instance
* @param[in]  pstTimer      DRM Infoframe send timer configs
*                           bTimerStart : Zero DRMIF timer start or stop
*                           u32Time     : Zero DRMIF send time
*                           
*****************************************************************************/

void SiiDrvTxZeroDrmIfTimerStart(SiiInst_t inst, SiiHdrTimerCfg_S *pstTimer);

/**
* @brief HDR mode change timer start
*
* @param[in]  inst          Handle to instance
* @param[in]  pstTimer      HDR mode change timer configs
*                           bTimerStart : HDR mode change timer start or stop
*                           u32Time     : HDR mode change wait time 
*                           
*****************************************************************************/
void SiiDrvTxHdrModeChangeTimerStart(SiiInst_t inst, SiiHdrTimerCfg_S *pstTimer);
#endif
#endif
#ifdef HDMI_CEC_SUPPORT
/*****************************************************************************/
/**
* @brief CEC Physical Address.
*
* @param[in]  inst       Handle to instance
*
* @retval                CEC Physical Address
*
*****************************************************************************/
uint16_t SiiDrvTxCecPhysicalAddrGet(SiiInst_t inst);
#endif

/*****************************************************************************/
/**
* @brief AV-Mute control.
*
* @param[in]  inst    Handle to instance
* @param[in]  onOff   Boolean to control AV-Mute request for downstream device
*                        - @ref true   Requests downstream device to mute audio/video.
*                        - @ref false  Requests downstream device to unmute audio/video.
*
*****************************************************************************/
void SiiDrvTxAvMuteSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief Info-frame control.
*
* @param[in]  inst        Handle to instance
* @param[in]  ifId        Info-Frame/Packet type
* @param[in]  pInfoFrame  Info-Frame/Packet content
*
*****************************************************************************/
void SiiDrvTxInfoframeSet(SiiInst_t inst, SiiInfoFrameId_t ifId, const SiiInfoFrame_t *pInfoFrame);

/*****************************************************************************/
/**
* @brief Info-frame enable/disable control.
*
* @param[in]  inst        Handle to instance
* @param[in]  ifId        Info-Frame/Packet type
* @param[in]  onOff       Boolean to control enable/disable info-frames/packets
*                         - @ref true       Enable Info-frame/packet transmission.
*                         - @ref false      Disable Info-frame/packet transmission.
*
*****************************************************************************/
void SiiDrvTxInfoframeOnOffSet(SiiInst_t inst, SiiInfoFrameId_t ifId, bool_t onOff);
#if 0
/*****************************************************************************/
/**
* @brief Info-frame get.
*
* @param[in]  inst        Handle to instance
* @param[in]  ifId        Info-Frame/Packet type
* @param[in]  pInfoFrame  Info-Frame/Packet content
*
*****************************************************************************/
void SiiDrvTxInfoframeGet(SiiInst_t inst, SiiInfoFrameId_t ifId, SiiInfoFrame_t *pInfoFrame);
/*****************************************************************************/
/**
* @brief Info-frame enable/disable get.
*
* @param[in]  inst        Handle to instance
* @param[in]  ifId        Info-Frame/Packet type
* @param[in]  *onOff       Boolean to get enable/disable info-frames/packets
*                         - @ref true       Info-frame/packet transmission is enable.
*                         - @ref false      Info-frame/packet transmission is disable .
*
*****************************************************************************/
void SiiDrvTxInfoframeOnOffGet(SiiInst_t inst, SiiInfoFrameId_t ifId, bool_t* onOff);
#endif
#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief Audio input control.
*
* @param[in]  inst    Handle to instance
* @param[in]  onOff   Boolean to control Audio input 
*                        - @ref true   enable audio input.
*                        - @ref false  disable audio input.
*
*****************************************************************************/
void SiiDrvTxAudioEnableSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief Audio-Mute control.
*
* @param[in]  inst    Handle to instance
* @param[in]  onOff   Boolean to control Audio-Mute request for downstream device
*                        - @ref true   Requests downstream device to mute audio.
*                        - @ref false  Requests downstream device to unmute audio.
*
*****************************************************************************/
void SiiDrvTxAudioMuteSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief Audio channel-Status header control.
*
* @param[in]  inst            Handle to instance
* @param[in]  pChannelStatus  Channel-status header content
*
*****************************************************************************/
void SiiDrvTxChannelStatusSet(SiiInst_t inst, const SiiChannelStatus_t *pChannelStatus);

/*****************************************************************************/
/**
* @brief Audio format set.
*
* @param[in]  inst            Handle to instance
* @param[in]  pAudioFormat    Audio format data structure
*
*****************************************************************************/
void SiiDrvTxAudioFormatSet(SiiInst_t inst, const SiiAudioFormat_t *pAudioFormat);
/*****************************************************************************/
/**
* @brief Audio format get.
*
* @param[in]  inst              Handle to instance
*
* @retval     SiiAudioFormat_t  Audio Format Info
*
*****************************************************************************/
SiiAudioFormat_t SiiDrvTxAudioFormatStatusGet(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief video input pram set.
*
* @param[in]  inst              Handle to instance
* @param[in]  pstBlackPram      Video input pram
*
* @retval     void
*
*****************************************************************************/
void SiiDrvTxVideoInputPramSet(SiiInst_t inst, SiiBlackPramInfo_t *pstBlackPram);

/*****************************************************************************/
/**
* @brief video black data set.
*
* @param[in]  inst              Handle to instance
* @param[in]  onOff             enable or disable cmd
*
* @retval     void
*
*****************************************************************************/
void SiiDrvTxVideoBlackDataSet(SiiInst_t inst, bool_t onOff);

#endif

/*****************************************************************************/
/**
* @brief pattern tpg enable.
*
* @param[in]  inst              Handle to instance
* @param[in]  onOff             enable or disable cmd
*
* @retval     void
*
*****************************************************************************/
void SiiDrvTxPatternTpgEnable(SiiInst_t inst, bool_t onOff);

#ifdef HDMI_HDCP_SUPPORT
/*****************************************************************************/
/**
* @brief Content protection control.
*
* @param[in]  inst        Handle to instance
* @param[in]  onOff       Boolean to enable/disable content protection
*                         - @ref true       Enable audio/video content protection.
*                         - @ref false      Disable audio/video content protection.
*
*
*****************************************************************************/
void SiiDrvTxHdcpProtectionSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief HDCP version set.
*
* @param[in]  inst        Handle to instance
* @param[in]  hdcpVer     HDCP version
*                         - @ref SII_DRV_DS_HDCP_VER__1X.      
*                         - @ref SII_DRV_DS_HDCP_VER__22.
*
*****************************************************************************/
void SiiDrvTxHdcpVersionSet(SiiInst_t inst, SiiModDsHdcpVersion_t hdcpVer);

/*****************************************************************************/
/**
* @brief HDCP status.
*
* @param[in]  inst            Handle to instance
* @param[out] pHdcpStatus     Audio format parameters.
*                             - @ref SII_DRV_HDCP_STATUS__OFF              Authentication is not enabled
*                             - @ref SII_DRV_HDCP_STATUS__SUCCESS_1X       Authentication succeeded for HDCP 1.X
*                             - @ref SII_DRV_HDCP_STATUS__SUCCESS_22       Authentication succeeded for HDCP 2.2
*                             - @ref SII_DRV_HDCP_STATUS__AUTHENTICATING   Authentication is in progress
*                             - @ref SII_DRV_HDCP_STATUS__FAILED           Authentication failed and does not re-try
*
*****************************************************************************/
void SiiDrvTxHdcpStateStatusGet(SiiInst_t inst, SiiDrvHdcpStatus_t *pHdcpStatus);
//void SiiDrvTxHdcpFailureReasonStatusGet(SiiInst_t inst, SiiDrvHdcpFailureStatus_t *pFailure);	//TODO : Implement Later 

/*****************************************************************************/
/**
* @brief Sink Hdcp capability get.
*
* @param[in]  inst            Handle to instance
* @param[out] pstHdcpSupport  sink support Hdcp or not
*                             - @ref bHdcp14 = 1 sink support Hdcp1.4, else not support.     
*                             - @ref bHdcp22 = 1 sink support Hdcp2.2, else not support.
*
*****************************************************************************/
void SiiDrvTxHdcpCapabilityGet(SiiInst_t inst, SiiHdcpCaps_t *pstHdcpSupport);

/*****************************************************************************/
/**
* @brief Interrogates downstream BKSV list from downstream device 
*        (for downstream repeater application only)
*
* @param[in]  inst            Handle to instance
* @param[out  pBksvList       List of downstream BKSVs.
*
*****************************************************************************/
void SiiDrvTxHdcpKsvListGet(SiiInst_t inst, SiiDrvHdcpKsvList_t *pBksvList);

/*****************************************************************************/
/**
* @brief User acknowledge for KSV list approval
*
* @param[in]  inst            Handle to instance
* @param[out  bApproved       KSV list approval.
*                             - @ref TRUE      List is approved
*                             - @ref FALSE     List is rovocated
*
*****************************************************************************/
void SiiDrvTxHdcpKsvListApprovalSet(SiiInst_t inst, bool_t bApproved);

/*****************************************************************************/
/**
* @brief HDCP topology configuration status.
*
* @param[in]  inst            Handle to instance
* @param[out] pTopology       Topology data structure.
*
*****************************************************************************/
void SiiDrvTxHdcpTopologyGet(SiiInst_t inst,  SiiDrvHdcpTopology_t *pTopology);

/*****************************************************************************/
/**
* @brief HDCP2.2 Content type configuration status.
*
* @param[in]  inst            Handle to instance
* @param[in] pContentType    Content indicator.
*                             - @ref SII_DRV_HDCP_CONTENT_TYPE__0
*                             - @ref SII_DRV_HDCP_CONTENT_TYPE__1
*
*****************************************************************************/
void SiiDrvTxHdcp2ContentTypeSet(SiiInst_t inst, SiiDrvHdcpContentType_t *pContentType);
//void SiiDrvTxHdcp2ContentTypeGet(SiiInst_t inst, SiiDrvHdcpContentType_t *pContentType);	//TODO : Implement Later

/*****************************************************************************/
/**
* @brief HDCP2x Code Update Status.
*
* @param[in]  inst            Handle to instance
* @param[in]  pHdcp2xCupdStat        hdcp2x Code update Status
*                             - @ref SII_DRV_HDCP2X_CUPD_CHK__ERROR
*                             - @ref SII_DRV_HDCP2X_CUPD_CHK__FAIL
*                             - @ref SII_DRV_HDCP2X_CUPD_CHK__DONE
*
*****************************************************************************/
void SiiDrvTxHdcp2xCupdStatusGet(SiiInst_t inst, SiiDrvHdcp2xCupdChkStat_t *pHdcp2xCupdStat);

#endif

/*****************************************************************************/
/**
* @brief Video Mute control.
*
* @param[in]  inst    Handle to instance
* @param[in]  onOff   Boolean to control Video Mute request for downstream device
*                        - @ref true   Requests downstream device to mute video.
*                        - @ref false  Requests downstream device to unmute video.
*
*****************************************************************************/
void SiiDrvTxVideoMuteSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief H/V-Sync Polarity Set.
*
* @param[in]  inst		Handle to instance
* @param[in]  hvSyncPol	Can be changed to the following polarities:
*
* @retval     #SII_HV_SYNC_POL__HPVP   Positive hor. sync / Positive ver. sync.
* @retval     #SII_HV_SYNC_POL__HPVN   Positive hor. sync / Negative ver. sync.
* @retval     #SII_HV_SYNC_POL__HNVP   Negative hor. sync / Positive ver. sync.
* @retval     #SII_HV_SYNC_POL__HNVN   Negative hor. sync / Negative ver. sync.
*
*****************************************************************************/
void SiiDrvTxHvSyncPolaritySet(SiiInst_t inst, SiiOutPutSyncCfg_t *pstOutPutSyncCfg);


/*****************************************************************************/
/**
* @brief Output video bit depth control.
* @note  Enables TMDS deep color mode if 10 or 12 bit is requested for both 4:2:0 and 4:4:4 signals.
*
* @param[in]  inst            Handle to instance
* @param[in]  bitDepth        Bit Depth of tmds output signal
*                             - @ref SII_DRV_BIT_DEPTH__8_BIT
*                             - @ref SII_DRV_BIT_DEPTH__10_BIT
*                             - @ref SII_DRV_BIT_DEPTH__12_BIT
*
*****************************************************************************/
void SiiDrvTxOutputBitDepthSet(SiiInst_t inst, SiiDrvBitDepth_t bitDepth);

/*****************************************************************************/
/**
* @brief Output mapping
* @note  
*
* @param[in]  inst            Handle to instance
* @param[in]  bYCbcr422        
*                             - @ref true
*                             - @ref false                           
*
*****************************************************************************/
void SiiDrvTxOutputMappingSet(SiiInst_t inst, bool_t bYCbcr422);

/*****************************************************************************/
/**
* @brief Configure Video Path input Color Space, Bit depth and Color conversion standard.
*
* @param[in]  inst    Handle to instance
* @param[in]  pClrInfo Video path Input Color Information data. 
*
*****************************************************************************/
void SiiDrvTxColorInfoConfig(SiiInst_t inst, SiiDrvTxColorInfoCfg_t *clrInfo);

/*****************************************************************************/
/**
* @brief Configure Video Path CSC.
*
* @param[in]  inst    Handle to instance
* @param[in]  pstCscInfo Video path CSC Information data. 
*
*****************************************************************************/
void SiiDrvTxCscConfig(SiiInst_t inst, SiiDrvTxCscCfg_t *pstCscInfo);

/*****************************************************************************/
/**
* @brief Configure Video Path  Color Space yuv<->rgb.
*
* @param[in]  inst    Handle to instance
* @param[in]  convertion Video path yuv <->rgb . 
*
*****************************************************************************/
void SiiDrvTxYuvRgbConvertion(SiiInst_t inst, SiiColorConvertion_t *convertion);

/*****************************************************************************/
/**
* @brief Output color space format.
* @note  Input color space format is detemined by AVI info-frame.
* @note  Use @ref SII_DRV_TX_CLRSPC__PASSTHRU to remain output color space unchanged.
*
* @param[in]  inst     Handle to instance
* @param[in]  clrSpc   Color Space format.
*                      - @ref SII_DRV_TX_CLRSPC__PASSTHRU
*                      - @ref SII_DRV_TX_CLRSPC__YC444_601
*                      - @ref SII_DRV_TX_CLRSPC__YC422_601
*                      - @ref SII_DRV_TX_CLRSPC__YC420_601
*                      - @ref SII_DRV_TX_CLRSPC__YC444_709
*                      - @ref SII_DRV_TX_CLRSPC__YC422_709
*                      - @ref SII_DRV_TX_CLRSPC__YC420_709
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC444_601
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC422_601
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC420_601
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC444_709
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC422_709
*                      - @ref SII_DRV_TX_CLRSPC__XVYCC420_709
*                      - @ref SII_DRV_TX_CLRSPC__YC444_2020
*                      - @ref SII_DRV_TX_CLRSPC__YC422_2020
*                      - @ref SII_DRV_TX_CLRSPC__YC420_2020
*                      - @ref SII_DRV_TX_CLRSPC__RGB_FULL
*                      - @ref SII_DRV_TX_CLRSPC__RGB_LIMITED
*
*****************************************************************************/
void SiiDrvTxOutputColorSpaceSet(SiiInst_t inst, SiiDrvTxClrSpc_t *clrSpc);

/*****************************************************************************/
/**
* @brief Phy power on or off.
*
* @param[in]  inst       Handle to instance
* @param[in]  onOff      Boolean to control phy power 
*                        - @ref true   Power on.
*                        - @ref false  Power off.
*
*****************************************************************************/
void SiiDrvTxPhyPowerOnOffSet(SiiInst_t inst, bool_t onOff);

/*****************************************************************************/
/**
* @brief Phy output enable or disable.
*
* @param[in]  inst       Handle to instance
* @param[in]  onOff      Boolean to control phy output 
*                        - @ref true   Output enable.
*                        - @ref false  Output disable.
*
*****************************************************************************/
void SiiDrvTxPhyOutputEnableSet(SiiInst_t inst, bool_t enable);

/*****************************************************************************/
/**
* @brief phy swing¡¢pll¡¢bandwidth config.
*
* @param[in]  inst       Handle to instance
* @param[in]  tmdsClk    Tmds clock 
*
*****************************************************************************/
void SiiDrvTxPhyConfig(SiiInst_t inst, SiiDrvPhyPara_t *pst_phy_param);


/*****************************************************************************/
/**
* @brief ddc speed config.
*
* @param[in]  inst       Handle to instance
* @param[in]  ddcspeed    DDC speed 
*
*****************************************************************************/
void SiiDrvTxDdcSpeedConfig(SiiInst_t inst, uint32_t ddcspeed);


/*****************************************************************************/
/**
* @brief transcode mode bypass config.
*
* @param[in]  inst       Handle to instance
* @param[in]  b_enable    enable 
*
*****************************************************************************/
void SiiDrvTxTansCodeBypassConfig(SiiInst_t inst, bool_t b_enable);
#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief Phy output state get.
*
* @param[in]  inst       Handle to instance
*
* @retval     #true      phy output enable.
* @retval     #false     phy output disable.
*
*****************************************************************************/
bool_t SiiDrvTxPhyOutputEnableGet(SiiInst_t inst);
#endif
#ifdef HISI_TX_MHL
/*****************************************************************************/
/**
* @brief Get Cbus Connection Mode
*
* @param[in]  inst		Handle to instance
* @param[out] cbusMode	Cbus connection mode
*
*****************************************************************************/
void SiiDrvTxCbusModeGet(SiiInst_t inst, SiiDrvTxCbusMode_t *cbus_mode);

/*****************************************************************************/
/**
* @brief Get Peer's Device Capabilities Status
*
* @param[in]  inst    Handle to instance
* @param[out]  peerDevCaps Device Capabilities data
*
*****************************************************************************/
void SiiDrvTxPeerDevcapsGet(SiiInst_t inst, SiiDrvTxMHLDevCap_t *peerDevCaps);


/*****************************************************************************/
/**
* @brief Set Device Capabilities
*
* @param[in]  inst    Handle to instance
* @param[in]  devCaps Device Capabilities to be set
*
*****************************************************************************/
void SiiDrvTxDevCapsSet(SiiInst_t inst, SiiDrvTxMHLDevCap_t *devCaps);


/*****************************************************************************/
/**
* @brief Get Peer's Extended Device Capabilities Status
*
* @param[in]  inst    Handle to instance
* @param[out]  peerXDevCaps	Extended Device Capabilities data
*
*****************************************************************************/
void SiiDrvTxPeerXDevcapsGet(SiiInst_t inst, SiiDrvTxMHLXDevCap_t *peerXDevCaps);


/*****************************************************************************/
/**
* @brief Set Extended Device Capabilities
*
* @param[in]  inst    Handle to instance
* @param[in]  xDevCaps	Extended Device Capabilities to be set
*
*****************************************************************************/
void SiiDrvTxXDevCapsSet(SiiInst_t inst, SiiDrvTxMHLXDevCap_t *xDevCaps);



/*******Internal Testing APIs*********************************************************/
//Enable/Disable Cbus TDM API.
void SiiDrvTxEnableTDM(SiiInst_t inst, bool_t *enable);
//Enable/Disable MHL3 AVLink
void SiiDrvTxEnableMHL3AVLink(SiiInst_t inst, bool_t *enable);
//Start EMSC Flood Testing
void SiiDrvTxEnableEMSCFloodTest(SiiInst_t inst);
//Start MDT Flood Testing
void SiiDrvTxEnableMDTFloodTest(SiiInst_t inst);
#endif
#endif // __SI_DRV_TX_API_H__
