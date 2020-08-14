/** \file vp_api_int.h
 * vp_api_int.h
 *
 * Header file for the API-II c files. This file contains the declarations
 * and defintions required to implement the VTD specific VP-API. This file
 * is used by VP-API internally. This file should not be included by the
 * application.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7317 $
 * $LastChangedDate: 2010-06-10 13:41:44 -0500 (Thu, 10 Jun 2010) $
 */

#ifndef VP_API_INT_H
#define VP_API_INT_H

#include "vp_api_cfg.h"
#include "vp_api_types.h"
#include "vp_api.h"
#include "vp_api_option.h"

#include "vp_debug.h"

/******************************************************************************
 *                              VP-API Defines                                *
 ******************************************************************************/
#define VP_ALL_LINES            0xFF  /**< Internal constant used to identify
                                       * all lines
                                       */
#define VP_ALL_SLACS          0xFFFF  /**< Internal constant used to identify
                                       * all SLACs
                                       */

/**< Profile types as defined by Profile Wizard */
typedef enum {
    VP_PRFWZ_PROFILE_AC = 0x00,
    VP_PRFWZ_PROFILE_DC = 0x01,
    VP_PRFWZ_PROFILE_TONE = 0x02,
    VP_PRFWZ_PROFILE_TONECAD = 0x03,
    VP_PRFWZ_PROFILE_RING = 0x04,
    VP_PRFWZ_PROFILE_CID_TYPE1 = 0x05,
    VP_PRFWZ_PROFILE_CID_TYPE2 = 0x06,
    VP_PRFWZ_PROFILE_METER = 0x07,
    VP_PRFWZ_PROFILE_RINGCAD = 0x08,
    VP_PRFWZ_PROFILE_TEST_CRITERIA = 0x09,
    VP_PRFWZ_PROFILE_TEST_TOPOLOGY = 0x0A,
    VP_PRFWZ_PROFILE_FXS_CTRL = 0x0B,
    VP_PRFWZ_PROFILE_CAL = 0x0C,
    VP_PRFWZ_PROFILE_METERING_GEN,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_DTMF_DIG_GEN,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_LINE_CAL_INT,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_FWD_DISC_INT,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_POLREV_PULSE_INT,      /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_MOMENTARY_LOOP_OPEN_INT,   /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_TIP_OPEN_INT,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_NONE,                  /**< Indicates that internal
                                                 profile data is not valid */
    VP_PRFWZ_PROFILE_880_CAL_DATA = 0xFC,
    VP_PRFWZ_PROFILE_890_CAL_DATA = 0xFD,

    VP_PRFWZ_PROFILE_FXO_CONFIG = 0xFE,
    VP_PRFWZ_PROFILE_DEVICE = 0xFF,
    VP_PRFWZ_PROFILE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req.*/
} VpProfileWizProfileType;

typedef enum {
    VP_PRFWZ_CUSTOM_ST_STANDBY = 0x00,
    VP_PRFWZ_CUSTOM_ST_ACTIVE = 0x02,
    VP_PRFWZ_CUSTOM_ST_ACTIVE_PR = 0x03,
    VP_PRFWZ_CUSTOM_ST_TIP_OPEN = 0x04,
    VP_PRFWZ_CUSTOM_ST_OHT = 0x06,
    VP_PRFWZ_CUSTOM_ST_OHT_PR = 0x07,
    VP_PRFWZ_CUSTOM_ST_DISCONNECT = 0x08,
    VP_PRFWZ_CUSTOM_ST_RINGING = 0x09,
    VP_PRFWZ_CUSTOM_ST_DET_MAP = 0xFF,
    VP_PRFWZ_CUSTOM_ST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req.*/
} VpProfileWizCustomStateType;

/** These locations are common to all profiles used in the API */
typedef enum
{
    VP_PROFILE_TYPE_MSB = 0,
    VP_PROFILE_TYPE_LSB = 1,
    VP_PROFILE_INDEX = 2,
    VP_PROFILE_LENGTH = 3,
    VP_PROFILE_VERSION = 4,
    VP_PROFILE_MPI_LEN = 5,
    VP_PROFILE_DATA_START = 6,
    VP_PROFILE_HDR_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProfileHeaderFieldType;

/* Define the Ringing Type (balanced/unbalanced) offset from end of MPI data */
#define VP_PROFILE_RING_TYPE_OFFSET     1

/** These are the locations of parameters in version 01 ringing profiles */
typedef enum
{
    VP_RING_PROFILE_RING_TRIP   = 18,
    VP_RING_PROFILE_CURRENT_LIM = 19,
    VP_RING_PROFILE_MAX_RING_V  = 20,
    VP_RING_PROFILE_TRACK_MODE  = 21,
    VP_RING_PROFILE_ENUM_SIZE  =  FORCE_STANDARD_C_ENUM_SIZE /* Portability Req */
} VpRingProfileFieldType;

/** These are the locations of parameters in version 01 DC profiles */
typedef enum
{
    VP_DC_PROFILE_THRESHOLDS =  9,
    VP_DC_PROFILE_DEBOUNCES  =  10,
    VP_DC_PROFILE_FLOOR_V    =  11,
    VP_DC_PROFILE_HOOK_HYST  =  12,
    VP_DC_PROFILE_ENUM_SIZE  =  FORCE_STANDARD_C_ENUM_SIZE /* Portability Req */
} VpDcProfileFieldType;

typedef enum
{
    /*
     * These parameters are the absolute locations of the specified value in
     * the CID profile
     */
    VP_CID_PROFILE_FSK_PARAM_LEN = 6,
    VP_CID_PROFILE_FSK_PARAM_CMD = 7,
    VP_CID_PRFT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCidProfileFieldType;

/*
 * These parameters are offset from the end of the FSK data to the value
 * specified
 */
#define VP_CID_PROFILE_CHECKSUM_OFFSET_MSB      (1)
#define VP_CID_PROFILE_CHECKSUM_OFFSET_LSB      (2)
#define VP_CID_PROFILE_LENGTH_OF_ELEMENTS_MSB   (3)
#define VP_CID_PROFILE_LENGTH_OF_ELEMENTS_LSB   (4)
#define VP_CID_PROFILE_START_OF_ELEMENTS_MSB    (5)
#define VP_CID_PROFILE_START_OF_ELEMENTS_LSB    (6)

/** These locations are for Tone and Ringing Cadence (Sequencer) profiles */
#define VP_PROFILE_TYPE_SEQUENCER_COUNT_MSB (6)
#define VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB (7)
#define VP_PROFILE_TYPE_SEQUENCER_START     (8)

/** These locations are for Calibration profiles */
#define VP_CAL_PROFILE_NUM_ENTRIES  (6)
#define VP_CAL_PROFILE_FIRST_ENTRY  (8)

/******************************************************************************
 *                        API Specific common FUNCTION PROTOTYPES             *
 ******************************************************************************/
void
VpImplementNonMaskEvents(
    VpOptionEventMaskType *pLineEventsMask,
    VpOptionEventMaskType *pDevEventsMask);

VpStatusType
VpImplementDefaultSettings(
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx);

EXTERN int
VpGetProfileIndex (
    const VpProfilePtrType pProfile);

EXTERN bool
VpVerifyProfileType(
    VpProfileType type,
    VpProfilePtrType pProfile);

EXTERN bool
VpIsDigit(
    VpDigitType digit);

/******************************************************************************
 *                        VCP Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
#if defined (VP_CC_VCP_SERIES)
VpStatusType
VpMakeVcpDeviceObject(
    VpDevCtxType *pDevCtx,
    VpVcpDeviceObjectType *pDevObj);

VpStatusType
VpMakeVcpDeviceCtx(
    VpDevCtxType *pDevCtx,
    VpVcpDeviceObjectType *pDevObj);

#endif /* VP_CC_VCP_SERIES */

/******************************************************************************
 *                        VCP2 Specific FUNCTION PROTOTYPES                   *
 ******************************************************************************/
#if defined (VP_CC_VCP2_SERIES)
VpStatusType
Vcp2MakeDeviceObject(
    VpDevCtxType *pDevCtx,
    VpVcp2DeviceObjectType *pDevObj);

VpStatusType
Vcp2MakeDeviceCtx(
    VpDevCtxType *pDevCtx,
    VpVcp2DeviceObjectType *pDevObj);

EXTERN bool
Vcp2ReadCalFlag(
    VpLineCtxType *pLineCtx);

EXTERN void
Vcp2SetCalFlag(
    VpLineCtxType *pLineCtx);
#endif /* VP_CC_VCP2_SERIES */

/******************************************************************************
 *                        Kernel Specific FUNCTION PROTOTYPES                 *
 ******************************************************************************/
#if defined (VP_CC_KWRAP)
  #include "vp_kernel.h"
#endif /* VP_CC_KWRAP */


/******************************************************************************
 *                        790 Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
#if defined (VP_CC_790_SERIES)

#ifndef VP790_EC_CH1
  #define VP790_EC_CH1    0x01
  #define VP790_EC_CH2    0x02
  #define VP790_EC_CH3    0x04
  #define VP790_EC_CH4    0x08
#endif

VpStatusType
VpMakeVp790DeviceObject(
    VpDevCtxType *pDevCtx,
    Vp790DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp790DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp790DeviceObjectType *pDevObj);

void
Vp790MuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

bool
Vp790FSKGeneratorReady(
    VpLineCtxType *pLineCtx);

bool
Vp790CliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

VpStatusType
Vp790CtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

void
Vp790CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 data);
#endif /* VP_CC_790_SERIES */

/******************************************************************************
 *                        792 Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/

#if defined (VP_CC_792_SERIES)
VpStatusType
Vp792MakeDeviceObject(
    VpDevCtxType            *pDevCtx,
    Vp792DeviceObjectType   *pDevObj);

VpStatusType
Vp792MakeDeviceCtx(
    VpDevCtxType            *pDevCtx,
    Vp792DeviceObjectType   *pDevObj);

#ifdef VP_CC_792_GROUP
bool
Vp792GroupGetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent);
#endif
#endif /* VP_CC_792_SERIES */

/******************************************************************************
 *                        880 Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
#if defined (VP_CC_880_SERIES)

#ifndef VP880_EC_CH1
  #define VP880_EC_CH1                0x01
  #define VP880_EC_CH2                0x02
#endif

#ifndef VP880_WIDEBAND_MODE
  #define VP880_WIDEBAND_MODE         0x20
#endif

VpStatusType
VpMakeVp880DeviceObject(
    VpDevCtxType *pDevCtx,
    Vp880DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp880DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp880DeviceObjectType *pDevObj);

void
Vp880MuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

bool
Vp880FSKGeneratorReady(
    VpLineCtxType *pLineCtx);

bool
Vp880CliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

VpStatusType
Vp880CtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

void
Vp880CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 data);

VpStatusType
Vp880SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);

VpStatusType
Vp880SetLineStateInt(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp880SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

void
Vp880LLSetSysState(
    VpDeviceIdType deviceId,
    VpLineCtxType *pLineCtx,
    uint8 lineState,
    bool writeToDevice);
#endif /* VP_CC_880_SERIES */


/******************************************************************************
 *                        890 Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
#if defined (VP_CC_890_SERIES)
  #include "vp890_api_int.h"
#endif /* VP_CC_890_SERIES */


/******************************************************************************
 *                        580 Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
#if defined (VP_CC_580_SERIES)
VpStatusType
VpMakeVp580DeviceObject(
    VpDevCtxType *pDevCtx,
    Vp580DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp580DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp580DeviceObjectType *pDevObj);

#endif /* VP_CC_580_SERIES */

/* Functions used internal to the API -- device dependant */
#if defined (VP_CC_790_SERIES) || defined (VP_CC_880_SERIES) \
 || defined (VP_CC_580_SERIES) || defined (VP_CC_890_SERIES)

/* Converts ms to units of API_TICKRATE */
#define MS_TO_TICKRATE(MS, API_TICKRATE)  \
    ((uint16)(((((uint32)MS*256*2)/API_TICKRATE)+1)/2))
#define TICKS_TO_MS(TICKS, API_TICKRATE)  \
    (uint16) (((uint32)API_TICKRATE * (uint32)TICKS) / 256)
#define MS_TO_TICKS_ROUND_UP(MS, API_TICKRATE)  \
    ((uint16)(((((uint32)MS*256*2 + API_TICKRATE - 1)/API_TICKRATE)+1)/2))
/* Gives frequency in units of 0.3662109375 Hz for signal generators */
#define TICKRATE_TO_HZ(API_TICKRATE)  \
    ((uint16)((uint32)(0x80000000 / API_TICKRATE) / 0x0C00))

#define NO_DATA     0x00
#define NOOP_CMD    0x06  /**< Standard No operation comnmand for all devices */

#define MAX_CFAIL_TEST      10
#define CFAIL_TEST_INTERVAL 10

/**< CallerID Profile Data structure definitions */
#define VP_FSK_MARK_SIGNAL     0xFF
#define VP_FSK_CHAN_SEIZURE    0x55

typedef enum
{
    VP_CLI_NULL = 0,
    VP_CLI_POLREV,              /**< No Parameters */
    VP_CLI_MUTEON,              /**< No Parameters */
    VP_CLI_MUTEOFF,             /**< No Parameters */
    VP_CLI_ALERTTONE,           /**< Duration/Tone definition parameters */
    VP_CLI_ALERTTONE2,          /**< Internal Alert Tone indicator */
    VP_CLI_SILENCE,             /**< Duration Parameter */
    VP_CLI_SILENCE_MASKHOOK,    /**< Duration/Masked-hook length parameters */
    VP_CLI_DETECT,              /**< Tone and Timeout parameters */
    VP_CLI_CHANSEIZURE,         /**< Duration Parameter */
    VP_CLI_MARKSIGNAL,          /**< Duration Parameter */
    VP_CLI_MESSAGE,             /**< No Parameters */
    VP_CLI_USER_DEFINED,        /**< Not supported in API-II */
    VP_CLI_EOT = 0x0D,          /**< No Parameters */
    VP_CLI_DTMF_MESSAGE = 0x0E, /**< Message data to be sent in DTMF format */
    VP_CLI_ELM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpApiElementsType;

/* VP API Function Prototypes - Not associated with a function pointer */
int16
VpConvertToInt16(
    uint8 *dataPtr);

int32
VpConvertToInt32(
    uint8 *dataPtr);

VpStatusType
VpCSLACSetAbsGain(
    VpLineCtxType *pLineCtx,
    VpOptionAbsGainType *gains);

uint8
ConvertApiState2PrfWizState(
    const VpLineStateType state);

void
InitTimerVars(
    VpLineCtxType *pLineCtx);

bool
VpUpdateDP(
    uint16 tickRate,
    VpOptionPulseType *pPulseSpecs,
    VpDialPulseDetectType *pDpStruct,
    VpOptionEventMaskType *pLineEvents);

void
VpInitDP(
    VpDialPulseDetectType *pDpStruct);

VpStatusType
VpCSLACGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

VpStatusType
VpCSLACClearResults(
    VpDevCtxType *pDevCtx);


VpStatusType
VpCSLACDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense);

VpLineStateType
VpGetReverseState(
    VpLineStateType currentState);

void
VpCSLACSetVas(
    uint8 *dcFeed,
    uint16 vasValue);

#ifdef CSLAC_GAIN_RELATIVE
void
VpConvertFixed2Csd(
    uint16 fixed,
    uint8 *csdBuf);

uint16
VpConvertCsd2Fixed(
    uint8 *csdBuf);
#endif
#endif

#if defined (VP_CC_790_SERIES)
VpStatusType
Vp790SetLineStateInt(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp790SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);
#endif


#if defined (VP_CC_580_SERIES)
VpStatusType
Vp580SetLineStateInt(

    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp580SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

#endif

uint16
VpComputeSquareRoot(
    uint32 number);

#endif /* VP_API_INT_H */
