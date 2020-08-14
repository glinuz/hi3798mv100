/** \file vp_api_option.h
 * vp_api_option.h
 *
 * This file contains declaration associated with VP-API Options.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7350 $
 * $LastChangedDate: 2010-06-15 16:36:44 -0500 (Tue, 15 Jun 2010) $
 */

#ifndef VP_API_OPTION
#define VP_API_OPTION

#include "vp_api_types.h"
#include "vp_api_cfg.h"
#include "vp_api_event.h"

/* Option IDs.  (See Options chapter in VP-API-2 Reference Guide.)
 * Line-specific option IDs begin with "VP_OPTION_ID_".  Device-specific
 * option IDs begin with "VP_DEVICE_OPTION_ID_".  When new option IDs are added,
 * the VpOptionValueType struct (below) must be updated accordingly.
 */
typedef enum {
    VP_DEVICE_OPTION_ID_PULSE            = 0x00,
    VP_DEVICE_OPTION_ID_CRITICAL_FLT     = 0x01,
    VP_OPTION_ID_ZERO_CROSS              = 0x02,
    VP_DEVICE_OPTION_ID_RAMP2STBY        = 0x03,
    VP_OPTION_ID_PULSE_MODE              = 0x04,
    VP_OPTION_ID_TIMESLOT                = 0x05,
    VP_OPTION_ID_CODEC                   = 0x06,
    VP_OPTION_ID_PCM_HWY                 = 0x07,
    VP_OPTION_ID_LOOPBACK                = 0x08,
    VP_OPTION_ID_LINE_STATE              = 0x09,
    VP_OPTION_ID_EVENT_MASK              = 0x0A,
    VP_OPTION_ID_RESERVED_1              = 0x0B,
    VP_OPTION_ID_RING_CNTRL              = 0x0C,
    VP_OPTION_ID_RESERVED_2              = 0x0D,
    VP_OPTION_ID_DTMF_MODE               = 0x0E,
    VP_DEVICE_OPTION_ID_DEVICE_IO        = 0x0F,
    VP_OPTION_ID_RESERVED_EVENT_MASK_VCP = 0x10,
    VP_OPTION_ID_PCM_TXRX_CNTRL          = 0x11,
    VP_DEVICE_OPTION_ID_PULSE2           = 0x12,
    VP_OPTION_ID_LINE_IO_CFG             = 0x13,
    VP_DEVICE_OPTION_ID_DEV_IO_CFG       = 0x14,
    VP_OPTION_ID_DTMF_SPEC               = 0x15,
    VP_DEVICE_OPTION_ID_PARK_MODE        = 0x16,
    VP_OPTION_ID_DCFEED_SLOPE            = 0x17,
    VP_OPTION_ID_SWITCHER_CTRL           = 0x18,
    VP_OPTION_ID_HOOK_DETECT_MODE        = 0x19,

    /* New option values may be added in this gap in the future. */
    VP_OPTION_ID_PULSE                   = 0x24,
    VP_OPTION_ID_DEBUG_SELECT            = 0x25,
    VP_OPTION_ID_ABS_GAIN                = 0x26,
    VP_DEVICE_OPTION_ID_PCM_SIG_CTL      = 0x27,
    VP_OPTION_ID_LINESTATE_CTL_MODE      = 0x28,

    VP_NUM_OPTION_IDS
} VpOptionIdType;

/** Parameters for dial pulse, flash, and on-hook */
typedef struct {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
#ifdef EXTENDED_FLASH_HOOK
    uint16 onHookMin;       /**< Minimum on-hook time (in 125uS) */
#endif
} VpOptionPulseType;

typedef struct {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
    uint16 onHookMin;       /**< Minimum on-hook time (in 125uS) */
    uint16 offHookMin;      /**< Minimum off-hook time (in 125uS) */
} VpOptionLinePulseType;

/** Method for line control when critical faults are detected */
typedef struct {
    /**< The line is set to disconnect when the specified fault is active and
     * the "En" bit is set TRUE
     */
    bool acFltDiscEn;           /**< AC fault detected */
    bool dcFltDiscEn;           /**< DC fault detected */
    bool thermFltDiscEn;        /**< Thermal fault detected */
} VpOptionCriticalFltType;

/** Method for zero-cross control */
typedef enum {
    VP_OPTION_ZC_M4B  = 0, /**< Zero-Cross On - Make before break */
    VP_OPTION_ZC_B4M  = 1, /**< Zero-Cross On - Break before make */
    VP_OPTION_ZC_NONE = 2, /**< Turn Zero-Cross control off */
    VP_OPTION_ZC_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_OPTION_ZC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpOptionZeroCrossType;

/** Dial Pulse decode enable/disable */
typedef enum {
    VP_OPTION_PULSE_DECODE_OFF = 0, /**< Disable Pulse Decode */
    VP_OPTION_PULSE_DECODE_ON  = 1, /**< Enable Pulse Decode */
    VP_OPTION_PULSE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPulseModeType;

/** Transmit/Receive Timeslot setting (timeslot and control) */
typedef struct {
    uint8 tx;   /**< 8-bit TX timeslot */
    uint8 rx;   /**< 8-bit RX timeslot */
} VpOptionTimeslotType;

typedef enum {
    VP_OPTION_ALAW     = 0, /**< Select G.711 A-Law PCM encoding */
    VP_OPTION_MLAW     = 1, /**< Select G.711 Mu-Law PCM encoding */
    VP_OPTION_LINEAR   = 2, /**< Select Linear PCM encoding */
    VP_OPTION_WIDEBAND = 3, /**< Select Wideband PCM encoding */
    VP_NUM_OPTION_CODEC_TYPE_IDS,
    VP_OPTION_CODEC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionCodecType;

/** PCM Highway Selection (B valid on select devices only) */
typedef enum {
    VP_OPTION_HWY_A          = 0, /**< Select the 'A' PCM Highway */
    VP_OPTION_HWY_B          = 1, /**< Select the 'B' PCM Highway */
    VP_OPTION_HWY_TX_A_RX_B  = 2, /**< Transmit on Highway A, receive on B */
    VP_OPTION_HWY_TX_B_RX_A  = 3, /**< Transmit on Highway A, receive on A */
    VP_OPTION_HWY_TX_AB_RX_A = 4, /**< Transmit on Highway A and B, receive on A */
    VP_OPTION_HWY_TX_AB_RX_B = 5, /**< Transmit on Highway A and B, receive on B */
    VP_OPTION_HWY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmHwyType;

/** Loopback option selection */
typedef enum {
    VP_OPTION_LB_OFF      = 0, /**< All loopbacks off */

    /* Following loopback options are supported for CSLAC and VCP only */
    VP_OPTION_LB_TIMESLOT = 1, /**< Perform a timeslot loopback */
    VP_OPTION_LB_DIGITAL  = 2, /**< Perform a full-digital loopback */
    VP_OPTION_LB_CHANNELS = 3, /**< Connects FXO to FXS line on same device */

    VP_NUM_LB_OPTIONS,
    VP_OPTION_LB_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionLoopbackType;

/** Active Line State battery supply selection */
typedef enum {
    VP_OPTION_BAT_AUTO  = 0, /**< Automatic Batery selection */
    VP_OPTION_BAT_HIGH  = 1, /**< Use High Batery */
    VP_OPTION_BAT_LOW   = 2, /**< Use Low Batery */
    VP_OPTION_BAT_BOOST = 3, /**< Include Positive Batery */
    VP_NUM_OPTION_BAT_IDS,
    VP_OPTION_BAT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionBatType;

/** Active Line State battery supply selection */
typedef struct {
    bool battRev;       /**< Smooth/Abrupt Battery Reversal (TRUE = abrupt) */

    VpOptionBatType bat;    /**< Battery selection for Active line state */
} VpOptionLineStateType;

/** Ring control option */
typedef enum {

    /* FXS */
    VP_LINE_STANDBY         = 0x00, /**< Low power line feed state */
    VP_LINE_TIP_OPEN        = 0x01, /**< Tip open circuit state */
    VP_LINE_ACTIVE          = 0x02, /**< Line Feed w/out VF */
    VP_LINE_ACTIVE_POLREV   = 0x03, /**< Polarity Reversal Line Feed w/out VF */
    VP_LINE_TALK            = 0x04, /**< Normal off-hook Active State; Voice Enabled */
    VP_LINE_TALK_POLREV     = 0x05, /**< Normal Active with reverse polarity; Voice Enabled */
    VP_LINE_OHT             = 0x06, /**< On-Hook tranmission state */
    VP_LINE_OHT_POLREV      = 0x07, /**< Polarity Reversal On-Hook tranmission state */
    VP_LINE_DISCONNECT      = 0x08, /**< Denial of service */
    VP_LINE_RINGING         = 0x09, /**< Ringing state */
    VP_LINE_RINGING_POLREV  = 0x0A, /**< Ringing w/Polarity Reversal */
    VP_LINE_STANDBY_POLREV  = 0x10, /**< Low power polrev line feed state */
    VP_LINE_PARK            = 0x11, /**< Park mode */
    VP_LINE_RING_OPEN       = 0x12, /**< Ring open */
    VP_LINE_HOWLER          = 0x13, /**< Howler */
    VP_LINE_TESTING         = 0x14, /**< Testing */
    VP_LINE_DISABLED        = 0x15, /**< Disabled */
    VP_LINE_NULLFEED        = 0x16, /**< Null-feed */

    /* FXO */
    VP_LINE_FXO_OHT,        /**< FXO Line providing Loop Open w/VF */
    VP_LINE_FXO_LOOP_OPEN,  /**< FXO Line providing Loop Open w/out VF */
    VP_LINE_FXO_LOOP_CLOSE, /**< FXO Line providing Loop Close w/out VF */
    VP_LINE_FXO_TALK,       /**< FXO Line providing Loop Close w/VF */
    VP_LINE_FXO_RING_GND,   /**< FXO Line providing Ring Ground (GS only)*/

    VP_NUM_LINE_STATES,
    VP_LINE_STATE_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_LINE_STATE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineStateType;

typedef struct {
    VpOptionZeroCrossType zeroCross;    /**< LCAS zero cross control */

    uint16 ringExitDbncDur; /**< Ringing Exit Debounce Duration; Used during end
                             * of ON periods of ringing cadences; 125uS
                             * resolution
                             */

    VpLineStateType ringTripExitSt; /**< State to automatically switch to upon
                                     * ring trip
                                     */
}  VpOptionRingControlType;

/** DTMF detection option */
typedef enum {
    VP_OPTION_DTMF_DECODE_OFF = 0, /**< Disable DTMF Digit Decode */
    VP_OPTION_DTMF_DECODE_ON  = 1, /**< Enable DTMF Digit  Decode */
    VP_OPTION_DTMF_GET_STATUS = 2, /**< Do not change anything; Just get the DTMF status  */
    VP_NUM_OPTION_DTMF_IDS,
    VP_OPTION_DTMF_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionDtmfModeControlType;

/* Device I/O Option related definitions */
typedef enum {
    VP_IO_INPUT_PIN  = 0, /* Configure GPIO pin as input pin */
    VP_IO_OUTPUT_PIN = 1, /* Configure GPIO pin as output pin */
    VP_IO_DIR_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceIoDirectionType;

typedef enum {
    VP_OUTPUT_DRIVEN_PIN = 0, /* Configure as TTL/CMOS output pin */
    VP_OUTPUT_OPEN_PIN   = 1, /* Configure as open collector/drain output pin */
    VP_OUTPUT_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceOutputPinType;

typedef struct {
    uint32 directionPins_31_0;      /* Device specific IO pin direction
                                     * (Pins 0 - 31) */
    uint32 directionPins_63_32;     /* Device specific IO pin direction
                                     * (Pins 32 - 63) */
    uint32 outputTypePins_31_0;     /* Output pin type (Pins 0 - 31) */
    uint32 outputTypePins_63_32;    /* Output pin type (Pins 32 - 63) */
} VpOptionDeviceIoType;

/* Definition for line I/O config option */
typedef struct {
    uint8 direction;
    uint8 outputType;
} VpOptionLineIoConfigType;

/* Definition for device I/O config option */
typedef struct {
    VpOptionLineIoConfigType lineIoConfig[VP_MAX_LINES_PER_DEVICE];
} VpOptionDeviceIoConfigType;

typedef enum {
    VP_OPTION_PCM_BOTH      = 0, /* Enable both PCM transmit and receive paths */
    VP_OPTION_PCM_RX_ONLY   = 1, /* Enable PCM receive path only */
    VP_OPTION_PCM_TX_ONLY   = 2, /* Enable PCM transmit path only */
    VP_OPTION_PCM_ALWAYS_ON = 3, /* Prevents disabling of PCM path */
    VP_PCM_TXRX_CNTRL_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_PCM_TXRX_CNTRL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmTxRxCntrlType;

/** Direction Specification */
typedef enum {
    VP_DIRECTION_DS,
    VP_DIRECTION_US,
    VP_DIRECTION_INVALID,   /**< Used by the API to determine if the direction
                             * field is valid */
    VP_DIRECTION_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDirectionType;

#define VP_LINE_FLAG_BYTES ((VP_MAX_LINES_PER_DEVICE + 7) / 8)

/** DTMF detection option control */
typedef struct {
    VpOptionDtmfModeControlType dtmfControlMode; /**< DTMF detection
                                                  * Enable/Disable */
    VpDirectionType direction;                  /**< Detection direction */
    uint32 dtmfDetectionSetting;                /**< Indicates the DTMF
                                                 *   detection setting for first
                                                 *    32 lines */
    uint8 dtmfResourcesRemaining;               /**< DTMF decoder resources
                                                 *   remaining */
    uint8 dtmfDetectionEnabled[VP_LINE_FLAG_BYTES];
                                                /**< DTMF detection setting for
                                                 *   lines 7-0, 15-8, etc. */
} VpOptionDtmfModeType;

/** Regional DTMF Specs */
typedef enum {
    VP_OPTION_DTMF_SPEC_ATT  = 0, /* Q.24 AT&T */
    VP_OPTION_DTMF_SPEC_NTT  = 1, /* Q.24 NTT */
    VP_OPTION_DTMF_SPEC_AUS  = 2, /* Q.24 Australian */
    VP_OPTION_DTMF_SPEC_BRZL = 3, /* Q.24 Brazilian */
    VP_OPTION_DTMF_SPEC_ETSI = 4, /* ETSI ES 201 235-3 v1.3.1 */
    VP_OPTION_DTMF_SPEC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionDtmfSpecType;

/**< The following types are for Park Mode options */
typedef struct {
    uint16 discTime;    /**< Specified in 500ms increments, up to 8 seconds */
    uint16 standbyTime; /**< Specified in 100ms increments, up to 8 seconds */
} VpOptionParkModeType;

/** Hook detection modes  */
typedef enum {
    VP_OPTION_HOOKDET_NORMAL         = 0,  /* normal hook detection behavior */
    VP_OPTION_HOOKDET_DISC_IS_ONHOOK = 1,  /* in the VP_LINE_DISCONNECT or VP_LINE_DISABLED
                                              state, the hook status is always considered
                                              to be on-hook */
    VP_OPTION_HOOKDET_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionHookDetectModeType;

/* Definition for Absolute Level option */
typedef struct {
    int16 gain_AToD;
    int16 gain_DToA;
} VpOptionAbsGainType;

#define VP_OPTION_ABS_GAIN_QUIET        (-32767)
#define VP_OPTION_ABS_GAIN_NO_CHANGE    (+32767)
#define VP_ABS_GAIN_UNKNOWN             (VP_OPTION_ABS_GAIN_NO_CHANGE - 1)

/* Option value struct for VP_DEVICE_OPTION_ID_PCM_SIG_CTL: */
typedef struct {
    bool enable;
    uint8 ctlTimeslot;
    uint8 sigTimeslot;
} VpOptionPcmSigCtlType;

/* Option value struct for VP_OPTION_ID_LINESTATE_CTL_MODE: */
typedef enum {
    VP_OPTION_LINESTATE_CTL_NORMAL     = 0x0000,
    VP_OPTION_LINESTATE_CTL_PCM        = 0x0080
} VpOptionLinestateCtlModeType;

/* The following struct can be passed to VpGetResults() if the option ID is not
   known at compile time, to ensure that the buffer is large enough regardless
   of the option type. */
typedef union {
    VpOptionPulseType pulse;                       /* VP_DEVICE_OPTION_ID_PULSE        */
                                                   /* VP_DEVICE_OPTION_ID_PULSE2       */
    VpOptionCriticalFltType criticalFlt;           /* VP_DEVICE_OPTION_ID_CRITICAL_FLT */
    VpOptionZeroCrossType zeroCross;               /* VP_OPTION_ID_ZERO_CROSS          */
    uint16 ramp2stby;                              /* VP_DEVICE_OPTION_ID_RAMP2STBY    */
    VpOptionPulseModeType pulseMode;               /* VP_OPTION_ID_PULSE_MODE          */
    VpOptionTimeslotType timeslot;                 /* VP_OPTION_ID_TIMESLOT            */
    VpOptionCodecType codec;                       /* VP_OPTION_ID_CODEC               */
    VpOptionPcmHwyType pcmHwy;                     /* VP_OPTION_ID_PCM_HWY             */
    VpOptionLoopbackType loopback;                 /* VP_OPTION_ID_LOOPBACK            */
    VpOptionLineStateType lineState;               /* VP_OPTION_ID_LINE_STATE          */
    VpOptionEventMaskType eventMask;               /* VP_OPTION_ID_EVENT_MASK          */
    VpOptionRingControlType ringControl;           /* VP_OPTION_ID_RING_CNTRL          */
    VpOptionDtmfModeType dtmfMode;                 /* VP_OPTION_ID_DTMF_MODE           */
    VpOptionDeviceIoType deviceIo;                 /* VP_DEVICE_OPTION_ID_DEVICE_IO    */
    VpOptionPcmTxRxCntrlType pcmTxRxCntrl;         /* VP_OPTION_ID_PCM_TXRX_CNTRL      */
    VpOptionDeviceIoConfigType deviceIoConfig;     /* VP_DEVICE_OPTION_ID_DEV_IO_CFG   */
    VpOptionLineIoConfigType lineIoConfig;         /* VP_OPTION_ID_LINE_IO_CFG         */
    VpOptionDtmfSpecType dtmfSpec;                 /* VP_OPTION_ID_DTMF_SPEC           */
    VpOptionParkModeType parkMode;                 /* VP_DEVICE_OPTION_ID_PARK_MODE    */
    VpOptionLinePulseType linePulse;               /* VP_OPTION_ID_PULSE               */
    uint16 dcFeedSlope;                            /* VP_OPTION_ID_DCFEED_SLOPE        */
    bool switcherCtrl;                             /* VP_OPTION_ID_SWITCHER_CTRL       */
    uint32 debugSelect;                            /* VP_OPTION_ID_DEBUG_SELECT        */
    VpOptionAbsGainType absGain;                   /* VP_OPTION_ID_ABS_GAIN            */
    VpOptionHookDetectModeType hookDetectMode;     /* VP_OPTION_ID_HOOK_DETECT_MODE    */
    VpOptionPcmSigCtlType pcmSigCtl;               /* VP_DEVICE_OPTION_ID_PCM_SIG_CTL  */
    VpOptionLinestateCtlModeType linestateCtlMode; /* VP_OPTION_ID_LINESTATE_CTL_MODE  */
} VpOptionValueType;

#endif /* VP_API_OPTION */
