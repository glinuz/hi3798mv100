/** \file vp_CSLAC_types.h
 * vp_CSLAC_types.h
 *
 * Header file for internal variables used in CSLAC type API.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7532 $
 * $LastChangedDate: 2010-07-21 16:38:27 -0500 (Wed, 21 Jul 2010) $
 */

#ifndef VP_CSLAC_TYPES_H
#define VP_CSLAC_TYPES_H

#include "vp_api_fxo_params.h"
#include "vp_api_timer.h"

#define VP_CSLAC_DEV_PROFILE_VERSION_INT_SW_CONFIG_FR   5
#define VP_CSLAC_DEV_PROFILE_VERSION_INT_SW_CONFIG      4
#define VP_CSLAC_DEV_PROFILE_VERSION_LINECARD_PARAM     3
#define VP_CSLAC_DEV_PROFILE_VERSION_VBH_REG            2
#define VP_CSLAC_DEV_PROFILE_VERSION_SW_CONFIG          1

#define VP_CSLAC_FXO_VERSION_DTMF_LEVEL                 1
#define VP_CSLAC_FXO_VERSION_POH                        2

#define VP_CSLAC_DC_PROFILE_VERSION_890                 1

#define VP_CSLAC_RING_PROFILE_VERSION_890               1

#define VP_CSLAC_TICKSTEP_0_5MS     0x80    /**< CSLAC Tickrate for 0.5mS */

#define VP_CID_TIMESCALE            1 /**< Timescale in CID time data */
#define VP_SIZEOF_CID_MSG_BUFFER 16

#define VP_CSLAC_DEV_PROF_TABLE_SIZE            1
#define VP_CSLAC_AC_PROF_TABLE_SIZE             2
#define VP_CSLAC_DC_PROF_TABLE_SIZE             2
#define VP_CSLAC_RINGING_PROF_TABLE_SIZE        2
#define VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE   11
#define VP_CSLAC_TONE_PROF_TABLE_SIZE           10
#define VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE   4
#define VP_CSLAC_METERING_PROF_TABLE_SIZE       2
#define VP_CSLAC_CALLERID_PROF_TABLE_SIZE       2
#define VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE     1
#define VP_CSLAC_CUSTOM_TERM_PROF_TABLE_SIZE    4

#define VP_CSLAC_STD_TONE          0
#define VP_CSLAC_HOWLER_TONE       4
#define VP_CSLAC_AUS_HOWLER_TONE   8
#define VP_CSLAC_NTT_HOWLER_TONE   12
#define VP_CSLAC_TONE_TYPE         5

#define VP_CSLAC_READ_RESPONSE_MASK (VP_LINE_EVID_LLCMD_RX_CMP \
                                   | VP_LINE_EVID_RD_OPTION \
                                   | VP_LINE_EVID_GAIN_CMP)

typedef struct {
    VpProfilePtrType pDevProfileTable[VP_CSLAC_DEV_PROF_TABLE_SIZE];
    VpProfilePtrType pAcProfileTable[VP_CSLAC_AC_PROF_TABLE_SIZE];
    VpProfilePtrType pDcProfileTable[VP_CSLAC_DC_PROF_TABLE_SIZE];
    VpProfilePtrType pRingingProfileTable[VP_CSLAC_RINGING_PROF_TABLE_SIZE];
    VpProfilePtrType pToneCadProfileTable[VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE];
    VpProfilePtrType pToneProfileTable[VP_CSLAC_TONE_PROF_TABLE_SIZE];
    VpProfilePtrType pRingingCadProfileTable[VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE];
    VpProfilePtrType pMeteringProfileTable[VP_CSLAC_METERING_PROF_TABLE_SIZE];
    VpProfilePtrType pCallerIdProfileTable[VP_CSLAC_CALLERID_PROF_TABLE_SIZE];
    VpProfilePtrType pFxoConfigProfileTable[VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE];
    VpProfilePtrType pCustomTermProfileTable[VP_CSLAC_CUSTOM_TERM_PROF_TABLE_SIZE];
} VpCSLACDeviceProfileTableType;

/**
 * The following structure is used by the device as a bitmask indicating
 * whether or not a given profile table entry is valid. In the bit location a
 * value '1' = profile is current, '0' = profile is not current.
 */
typedef struct {
    uint16 devProfEntry;
    uint16 acProfEntry;
    uint16 dcProfEntry;
    uint16 ringingProfEntry;
    uint16 toneCadProfEntry;
    uint16 toneProfEntry;
    uint16 ringCadProfEntry;
    uint16 meterProfEntry;
    uint16 cidCadProfEntry;
    uint16 fxoConfigProfEntry;
    uint16 customTermProfEntry;
} VpCSLACProfileTableEntryType;

#define VP_PROF_ENTRY_INVALID   0
#define VP_PROF_ENTRY_VALID     1

typedef union {
    VpOptionCriticalFltType criticalFaultOption;
    VpOptionEventMaskType eventMaskOption;
    VpOptionPulseModeType pulseModeOption;
    VpOptionPulseType pulseTypeOption;
    VpOptionRingControlType ringControlOption;
    VpOptionZeroCrossType zeroCross;
    VpOptionTimeslotType timeSlotOption;
    VpOptionCodecType codecOption;
    VpOptionPcmHwyType pcmHwyOption;
    VpOptionLoopbackType loopBackOption;
    VpOptionLineStateType lineStateOption;
    VpOptionPcmTxRxCntrlType pcmTxRxCtrl;
    VpDeviceIoAccessDataType deviceIoData;
    VpOptionDeviceIoType deviceIo;
    VpLoopCondResultsType loopCond;
    bool autoShutdownEn;
    VpOptionAbsGainType absGain;
} VpGetResultsOptionsDataType;

typedef struct {
    VpGetResultsOptionsDataType optionData;
    uint8 chanId;
    VpOptionIdType optionType;
} VpGetResultsOptionsType;

#define VP_CID_DTMF_ON_TIME    70   /**< Nominal 70mS +/- 5mS */
#define VP_CID_DTMF_OFF_TIME   70   /**< Minimum 65mS */

typedef enum {
    VP_CID_GENERATOR_DATA,
    VP_CID_GENERATOR_KEYED_CHAR,
    VP_CID_SIGGEN_EOT,
    VP_CID_NO_CHANGE,           /**< Load tones only, do not change generator
                                 * state
                                 */
    VP_CID_GENERATOR_MARKOUT,
    VP_CID_CTRL_TYPE_CNT,

    VP_CID_GENC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCidGeneratorControlType;

typedef enum {
    VP_CID_RESET_VALUE = 0x0000,

    VP_CID_IS_DEBOUNCE = 0x0001,        /**< Set when debounce has expired */
    VP_CID_CLIAWAITTONE = 0x0002,
    VP_CID_REPEAT_MSG = 0x0004,
    VP_CID_AWAIT_TONE = 0x0008,         /**< Set if waiting for CPE ACK Tone */

    VP_CID_TERM_FSK = 0x0010,           /**< Set if stopping FSK generator */
    VP_CID_END_OF_MSG = 0x0020,         /**< Set if API detects an end of
                                         * message in the buffered data
                                         */
    VP_CID_SIG_B_VALID = 0x0040,        /**< Set if sigGenB is valid for ones */
    VP_CID_IN_PROGRESS = 0x0080,        /**< Set if Caller ID is in progress */

    VP_CID_ACTIVE_RING_SEQ = 0x0100,    /**< Set if active CID Ring sequence */
    VP_CID_PRIMARY_FULL = 0x0200,       /**< Set when primary buffer contains
                                         * CID data
                                         */
    VP_CID_SECONDARY_FULL = 0x0400,     /**< Set when secondary buffer contains
                                         * CID data
                                         */
    VP_CID_PRIMARY_IN_USE = 0x0800,     /**< Set when primary buffer in use */
    VP_CID_SECONDARY_IN_USE = 0x1000,   /**< Set when secondary buffer in use */
    VP_CID_WAIT_ON_ACTIVE = 0x2000,     /**< Set when a "wait on" mode is
                                         * active. Should only be cleared in
                                         * the timer function (since currently
                                         * support only for Wait on CID or time
                                         */
    VP_CID_MID_CHECKSUM = 0x4000,       /**< Set when a checksum value is being
                                         * sent.
                                         */
    VP_CID_MUTE_ON = 0x8000,            /**< Set when a "Mute On" CID operator
                                         * is active.
                                         */

    VP_CIDS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCidStatusFlagType;

typedef enum {
    VP_CID_DTMF_RESET_VALUE = 0x0000,
    VP_CID_ACTIVE_ON_TIME = 0x0001,
    VP_CID_ACTIVE_OFF_TIME = 0x0002,
    VP_CID_DTMFS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCidDtmfStatusType;

/** Caller ID variables used by the line */
typedef struct {
    VpCidStatusFlagType status;
    VpCidDtmfStatusType dtmfStatus; /**< Used only for DTMF CID Message Data */

    uint16 cliTimer;        /**< Time in increments of device ticks */
    uint8 currentData;      /**< Data to repeat on the line */
    uint16 cidCheckSum;     /**< Checksum to append to message data */

    uint8 cliDebounceTime;  /**< Hook debouce specified in device ticks */
    VpProfilePtrType pCliProfile;   /**< Currently running CID Profile */

    VpDigitType cliDetectTone1;
    VpDigitType cliDetectTone2;

    uint8 cliMPIndex;       /**< Current Index into primary message buffer */
    uint8 cliMSIndex;       /**< Current Index into secondary message buffer */

    uint8 cliIndex;         /**< Current Index into profile data */

    uint8 primaryBuffer[VP_SIZEOF_CID_MSG_BUFFER];
    uint8 secondaryBuffer[VP_SIZEOF_CID_MSG_BUFFER];

    uint8 primaryMsgLen;    /**< Length of data in the primary buffer */
    uint8 secondaryMsgLen;  /**< Length of data in the secondary buffer */
    VpDigitType digitDet;   /**< Stores the last DTMF digit detected during
                             * Caller ID (if enabled via tone detection).
                             */
} VpCallerIdType;

typedef enum {
    VP_CADENCE_RESET_VALUE = 0x0000,
    VP_CADENCE_STATUS_ACTIVE = 0x0001,
    VP_CADENCE_STATUS_BRANCHING  = 0x0002,
    VP_CADENCE_STATUS_MID_TIMER  = 0x0004,
    VP_CADENCE_STATUS_SENDSIG  = 0x0008,
    VP_CADENCE_STATUS_METERING = 0x0010,
    VP_CADENCE_STATUS_BRANCHING_LVL2  = 0x0020,
    VP_CADENCE_STATUS_IGNORE_POLARITY = 0x0040,
    VP_CADENCE_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCadenceStatusType;

/** Variables to control the cadence on the line using the Sequencer */
#define VP_CSLAC_MAX_BRANCH_DEPTH   2
#define VP_CSLAC_BRANCH_LVL_0       0
#define VP_CSLAC_BRANCH_LVL_1       1

typedef struct {
    VpProfilePtrType pActiveCadence;    /**< Currently Active Cadence */
    VpProfilePtrType pCurrentPos;       /**< Current position in profile */

    VpCadenceStatusType status;

    uint8 index;        /**< Index in the the cadence table   */
    uint8 length;       /**< Length of executable cadence instructions */

    /**< Remaining number of repeats to perform for each branch depth */
    uint8 count[VP_CSLAC_MAX_BRANCH_DEPTH];

    uint16 timeRemain;
    uint8 branchAt;     /**< Outermost step we're branching */
    uint8 meteringBurst;    /**< Tracks the number of metering pulses sent */

    /* The following variables are for controlling signal generator ramp */
#define VPCSLAC_MAX_GENERATOR_DATA 11
    uint8 regData[VPCSLAC_MAX_GENERATOR_DATA];  /**< Scratchpad to reduce MPI
                                                 * traffic.
                                                 */
    /*
     * These values should correspond to device specific values for the
     * freqency/level being set -- not a generic value.
     */
    uint16 startFreq;
    bool isFreqIncrease;
    uint16 stopFreq;
    uint16 freqStep;    /**< Step size assumes a 50mS interval. */

    uint16 startLevel;
    uint16 stopLevel;
    uint16 levelStep;  /**< Step size assumes a 50mS interval. Note: It is
                        * possible to support a negative step, but the first
                        * use of this function is UK type howler tone which only
                        * increases in level.
                        */
} VpSeqDataType;

/** Variables to control the CID sequence on the line using the CID Sequencer */
typedef struct {
    VpProfilePtrType pActiveCadence;    /**< Currently Active Cadence */
    VpProfilePtrType pCurrentPos;       /**< Current position in profile */

    uint8 index;        /**< Index in the the cadence table   */
    uint16 timeRemain;
} VpCidSeqDataType;

typedef VpStatusType
(*VpSeqInstructionFuncPtrType) (
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSequenceData);

#define VP_SEQ_OPERATOR_MASK 0xE0
#define VP_SEQ_SUBTYPE_MASK  0x1F
#define VP_SEQUENCER_TICKRATE   0x0500  /* 5mS - Used with MS_TO_TICKRATE */

/**< The super commands are the upper 3 bits of the command byte */
typedef enum {
    VP_SEQ_SPRCMD_COMMAND_INSTRUCTION = 0x00,
    VP_SEQ_SPRCMD_TIME_INSTRUCTION = 0x20,
    VP_SEQ_SPRCMD_BRANCH_INSTRUCTION = 0x40,
    VP_SEQ_SPRCMD_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSeqSuperCommandsType;

/**< The sub commands are the lower 5 bits of the command byte */
typedef enum {
    VP_SEQ_SUBCMD_SIGGEN = 0x00,
    VP_SEQ_SUBCMD_LINE_STATE = 0x01,
    VP_SEQ_SUBCMD_START_CID = 0x02,
    VP_SEQ_SUBCMD_WAIT_ON = 0x07,   /**< Wait for CID or time to continue */

    VP_SEQ_SUBCMD_RAMP_GENERATORS = 0x08,   /**< Forces frequency/amplitude
                                             * change of the tone generator.
                                             * Tone Cadence associated with this
                                             * command must be specified as
                                             * "ramp" type. The period between
                                             * adjustments is fixed 50mS.
                                             */

    /**< API Internal Use only operators */
    VP_SEQ_SUBCMD_METERING = 0x10,  /**< Cadence Metering */

    VP_SEQ_SUBCMD_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSeqSubCommandsType;

typedef enum {
    VP_SEQ_SIGGEN_ALL_DISABLED = 0x00,
    VP_SEQ_SIGGEN_A_EN = 0x01,
    VP_SEQ_SIGGEN_B_EN = 0x02,
    VP_SEQ_SIGGEN_C_EN = 0x04,
    VP_SEQ_SIGGEN_D_EN = 0x08,
    VP_SEQ_SIGGEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSeqSigGenCtrlType;

typedef enum {
    VP_PROFILE_CADENCE_STATE_STANDBY = 0,
    VP_PROFILE_CADENCE_STATE_TIP_OPEN,
    VP_PROFILE_CADENCE_STATE_TALK,
    VP_PROFILE_CADENCE_STATE_ACTIVE,
    VP_PROFILE_CADENCE_STATE_RSVD,
    VP_PROFILE_CADENCE_STATE_OHT,
    VP_PROFILE_CADENCE_STATE_DISCONNECT,
    VP_PROFILE_CADENCE_STATE_RINGING,
    VP_PROFILE_CADENCE_STATE_POLREV_STANDBY,
    VP_PROFILE_CADENCE_STATE_POLREV_TIP_OPEN,
    VP_PROFILE_CADENCE_STATE_POLREV_TALK,
    VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE,
    VP_PROFILE_CADENCE_STATE_POLREV_RSVD,
    VP_PROFILE_CADENCE_STATE_POLREV_OHT,
    VP_PROFILE_CADENCE_STATE_POLREV_DISCONNECT,
    VP_PROFILE_CADENCE_STATE_POLREV_RINGING,
    VP_PROFILE_CADENCE_STATE_FXO_OHT,
    VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN,
    VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE,
    VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK,
    VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM,
    VP_PROFILE_CADENCE_STATE_MSG_WAIT_POLREV,
    VP_PROFILE_CADENCES_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpProfileCadencerStateTypes;

/* struct VpDialPulseDetectType is for FXS lines when detecting digits */
typedef enum {
    VP_DP_DETECT_STATE_LOOP_OPEN,
    VP_DP_DETECT_STATE_LOOP_CLOSE,
    VP_DP_DETECT_STATE_IDLE,
    VP_DP_DETECTS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDialPulseDetectStatesType;

typedef struct {
    int digits;
    uint16 lo_time;
    uint16 lc_time;
    VpDialPulseDetectStatesType state;   /**< Dial Pulse State Machine state */
    bool hookSt;
    uint8 signalingData;
} VpDialPulseDetectType;

/* struct VpDigitGenerationDataType is for FXO lines when generating digits */
typedef struct {
    uint16 dtmfOnTime;
    uint16 dtmfOffTime;
    uint8 breakTime;
    uint8 makeTime;
    uint16 flashTime;
    uint16 dpInterDigitTime;
    uint8 dtmfHighFreqLevel[2];
    uint8 dtmfLowFreqLevel[2];
} VpDigitGenerationDataType;

/** Virtual Device Registers - Used to reduce MPI accesses to the Device */
typedef struct {
    uint8 sigRegMSB;    /**< Signaling Register's Most Significant byte */
    uint8 sigRegLSB;    /**< Signaling Register's Least Significant byte */
    uint8 iMaskMSB;     /**< Interrupt mask for the current line */
    uint8 iMaskLSB;     /**< Interrupt mask for the current line */
    uint8 iMaskMSBSP;   /**< Interrupt mask for the current line */
    uint8 iMaskLSBSP;   /**< Interrupt mask for the current line */
    uint8 ccR1SP;
    uint8 ccR5SP;       /**< Scratchpad for CCR5 used during metering */
    uint8 ccR8SP;       /**< Scratchpad for CCR8 used during calibration */
    uint8 ioReg;        /**< IO Register for QSLAC SLIC states */
} VpVirtualDeviceReg;

typedef enum {
    /**< Indicates if function is running from InitDevice. Not the same as in
     * the middle of an initialization process (including calibration). Used to
     * allow InitDevice function calling other functions to be enabled. Otherwise,
     * some functions will return "Device Not Initialized" error.
     */
    VP_DEV_INIT_IN_PROGRESS = 0x0001,

    VP_DEV_INIT_CMP = 0x0002,    /**< Set if device has been initialized */
    VP_DEV_IN_CAL = 0x0004,      /**< Set if device is busy calibrating */
    VP_DEV_PENDING_INT = 0x0008, /**< Set if there is a pending interrupt */

    VP_DEV_ABS_BAT_CAL = 0x0010, /**< Set when running ABS (batt switch) Cal */
    VP_DEV_ABV_CAL = 0x0020,     /**< Set when running ABV Calibration on Tracker */
    VP_DEV_ABV_CAL_ABS = 0x0040, /**< Set when running ABV Calibration on ABS */

    VP_DEV_TEST_BUFFER_READ = 0x0100, /**< Set if test buffer was read this tick */

     /**< Set if device in process of warm reboot. Note that warm reboot
      * detection alone only helps skipping of VpCalCodec() because it is run
      * with VpInitDevice(). But it does not ensure that VpCalLine() was
      * previously run.
      */
    VP_DEV_WARM_REBOOT = 0x0200

} VpDeviceStateType;

typedef struct {
    VpDeviceStateType state;
    uint8 globStatReg;      /**< Holds state of the device stat reg */
    uint8 calibrating;      /**< Calibration timer */
    uint8 numIntServiced;   /**< Max interrupts to be serviced during tick */
} VpCSLACDeviceStatusType;

#define VP_CSLAC_MAX_RCN_PCN_SIZE   2
typedef struct {
    uint8 rcnPcn[VP_CSLAC_MAX_RCN_PCN_SIZE];    /**< Revision Code Number and
                                                 * Product Code Number as
                                                 * applicable
                                                 */
    uint8 maxChannels;  /**< How many lines supported by this device */
    bool testSupport;   /**< TRUE if this device supports line tests. */
} VpDeviceStaticInfoType;

typedef struct {
    uint8 lastChan;     /**< The last line checked */
    bool clkFault;      /** TRUE if a clock fault is active */
    bool bat1Fault;     /** TRUE if a bat1 fault is active and device level */
    bool bat2Fault;     /** TRUE if a bat2 fault is active and device level */
    bool bat3Fault;     /** TRUE if a bat3 fault is active and device level */
} VpDeviceDynamicInfoType;

/** Line state variables used by the line (api internal) */
typedef enum {
    VP_CSLAC_STATUS_INVALID = 0x0000,

    /* FXS Type Status */
    VP_CSLAC_HOOK = 0x0001,
    VP_CSLAC_GKEY = 0x0002,

    /* FXO Type Status */
    VP_CSLAC_RAW_DISC = 0x0001,     /*
                                     * Set on 890 when device level disconnect
                                     * status is set. Cleared otherwise.
                                     */

    VP_CSLAC_RINGING = 0x0004,      /**< Set when Ringing detected on Line */
    VP_CSLAC_DISC = 0x0008,         /**< Set when Disconnect detected on Line */

    VP_CSLAC_AC_FLT = 0x0010,
    VP_CSLAC_DC_FLT = 0x0020,
    VP_CSLAC_THERM_FLT = 0x0040,
    VP_CSLAC_CAL_ENABLE = 0x0080,   /**< Set when the line is in a state that
                                     * will allow device calibration
                                     */

    VP_CSLAC_CAL_FAIL = 0x0100,
    VP_CSLAC_POLREV = 0x0200,
    VP_CSLAC_POLREV_REPORTED = 0x0400,
    VP_CSLAC_LIU = 0x0800,

    VP_CSLAC_LINE_LEAK_TEST = 0x1000,   /**< Set when line is being tested for
                                         * resistive leak. Clear when test is
                                         * complete.
                                         */
    VP_CSLAC_RING_AMP_DET = 0x2000,
    VP_CSLAC_POH = 0x4000,
    VP_CSLAC_STATUS_VALID = 0x8000
} VpCslacLineCondType;

typedef enum {
    VP_CSLAC_CAL_NONE = 0x0000,
    VP_CSLAC_CAL_VOC,
    VP_CSLAC_CAL_VAG,
    VP_CSLAC_CAL_VBG,
    VP_CSLAC_CAL_ILA,
    VP_CSLAC_CAL_VAS,
    VP_CSLAC_CAL_ABV,
    VP_CSLAC_CAL_CLEANUP,
    VP_CSLAC_CAL_TYPE_SIZE,
    VP_CSLAC_CAL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCslacCalType;

typedef struct {
    VpLineStateType currentState;   /**< Current state of the line */
    VpLineStateType previous;   /**< Previous sate of the line */

    VpLineStateType usrCurrent; /**< Current user set state of the line */

    VpCslacLineCondType condition;
    VpCslacCalType calType;
} VpApiIntLineStateType;

typedef enum
{
    VP_CUSTOM_TERM_SLIC_TYPE = 6,
    VP_CUSTOM_TERM_NUM_STATES = 7,
    VP_CUSTOM_TERM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vpCustomTermProfileParams;

typedef enum
{
    VP_FXO_DIAL_PRFL_DTMF_ON_MSB = 6,
    VP_FXO_DIAL_PRFL_DTMF_ON_LSB = 7,
    VP_FXO_DIAL_PRFL_DTMF_OFF_MSB = 8,
    VP_FXO_DIAL_PRFL_DTMF_OFF_LSB = 9,
    VP_FXO_DIAL_PRFL_FLASH_HOOK_MSB = 10,
    VP_FXO_DIAL_PRFL_FLASH_HOOK_LSB = 11,
    VP_FXO_DIAL_PRFL_PULSE_BREAK = 12,
    VP_FXO_DIAL_PRFL_PULSE_MAKE = 13,
    VP_FXO_DIAL_PRFL_INTERDIG_MSB = 14,
    VP_FXO_DIAL_PRFL_INTERDIG_LSB = 15,
    VP_FXO_DIAL_PRFL_RING_PRD_MAX = 16,
    VP_FXO_DIAL_PRFL_RING_PRD_MIN = 17,
    VP_FXO_DIAL_PRFL_RING_VOLT_MIN = 18,
    VP_FXO_DIAL_PRFL_DISC_VOLT_MIN = 19,
    VP_FXO_DIAL_PRFL_LIU_THRESH_MIN = 20,
    VP_FXO_DIAL_PRFL_RSVD = 21,
    VP_FXO_DIAL_PRFL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vpCslac_fxo_dialPrflParams;
#endif




