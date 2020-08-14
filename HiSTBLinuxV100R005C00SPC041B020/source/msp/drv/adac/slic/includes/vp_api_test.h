/** \file vp_api_test.h
 * file vp_api_test.h
 *
 * This file contains declaration associated with VP-API Test features.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7301 $
 * $LastChangedDate: 2010-06-08 14:43:02 -0500 (Tue, 08 Jun 2010) $
 */

#ifndef VP_API_TEST_H
#define VP_API_TEST_H

#include "vp_api_types.h"
#include "vp_api_option.h"
#include "vp_api_event.h"   /* For VpDigitType */

#define VP_TEST_NOT_MEASURED            0x8000    /* a value of -1 to indicate not measured */
                                        /* chosen because it is an impermissible value in those tests where used */

/** Tests supported by VpTestLine function */
typedef enum
{
    VP_TEST_ID_PREPARE        = 0x00, /* Prepare for a test sequence */
    VP_TEST_ID_CONCLUDE       = 0x01, /* Conclude a test sequence */
    VP_TEST_ID_SET_SENSE_GAIN = 0x02, /* Set ILG and IMT gains high or low */
    VP_TEST_ID_OPEN_VDC       = 0x03, /* Measure open DC voltage */
    VP_TEST_ID_OPEN_VAC       = 0x04, /* Measure open AC voltage */
    VP_TEST_ID_DIFF_VAC       = 0x05, /* Measure differential AC voltage */
    VP_TEST_ID_DC_RLOOP       = 0x06, /* Measure DC loop resistance */
    VP_TEST_ID_AC_RLOOP       = 0x07, /* Measure AC loop impedance */
    VP_TEST_ID_NULL_IGND      = 0x08, /* Null ground current */
    VP_TEST_ID_3ELE_RES       = 0x09, /* Measure 3-element insulation resistance */
    VP_TEST_ID_3ELE_CAP       = 0x0A, /* Measure 3-element capacitance */
    VP_TEST_ID_IDC            = 0x0B, /* Measure DC currents */
    VP_TEST_ID_IAC            = 0x0C, /* Measure AC currents */
    VP_TEST_ID_KEYPAD         = 0x0D, /* Keypad DTMF and Pulse dial test */
    VP_TEST_ID_NOISE          = 0x0E, /* Measure Active or Passive Noise 3, 3.4 & 15KhZ */
    VP_TEST_ID_SNR_QDIST      = 0x0F, /* Measure SNR and quantization distortion */
    VP_TEST_ID_ARB_1TONE      = 0x10, /* Measure arbitrary single tone */
    VP_TEST_ID_TONE_GEN       = 0x11, /* Generate a single/dual/quad-tone */
    VP_TEST_ID_DIALTONE       = 0x12, /* Detect and measure dial tone (Inward test) */
    VP_TEST_ID_HYBRID_LOSS    = 0x13, /* Measure trans-hybrid loss */
    VP_TEST_ID_ALT_REN        = 0x14, /* Alternative method used to measure REN */
    VP_TEST_ID_HOOK_STATUS    = 0x15, /* Check Hook Status (while in test mode) */
    VP_TEST_ID_PCM_COLLECT    = 0x16, /* Collect PCM samples */
    VP_TEST_ID_AC_RLOOP_PHASE = 0x17, /* Rloop with Phase information */
    VP_TEST_ID_SET_BATTERIES  = 0x18, /* Select batteries for line testing */
    VP_TEST_ID_MONITOR_IV     = 0x19, /* Non-Intrusive Monitoring Test */
    VP_TEST_ID_UNBAL_TONE     = 0x1A, /* Generates an unbalanced tracing tone */
    VP_TEST_ID_3ELE_RES_VMID  = 0x1B, /* Measure 3-element insulation resistance with a specified vMid */
    VP_TEST_ID_AC_TRANS       = 0x1C, /* Measures digital-digital transmission characteristics */
    VP_TEST_ID_GEN_TEST       = 0x1D, /* Multi-functional primitive */
    VP_TEST_ID_SLOPE_REN      = 0x1E, /* REN test with a ramp instead of step */
    /* Test IDs above this line are supported by one or more VCP devices.
       Their values must not change.  More VCP tests might be added above this
       line in the future. */

    VP_TEST_ID_RSVD_MIN       = 0x23, /* Custom VCP tests (not currently used) */
    VP_TEST_ID_RSVD_MAX       = 0x23,

    /* The values of the constants below this line can change from time. */

    VP_TEST_ID_3ELE_RES_HG,     /* Alternative 3-element resistance high gain */
    VP_TEST_ID_3ELE_RES_LG,     /* Alternative 3-element resistance low gain */
    VP_TEST_ID_3ELE_CAP_CSLAC,  /* CSLAC 3-element capacitance test */
    VP_TEST_ID_MSOCKET_TYPE2,   /* Master Socket test for UK termination */
    VP_TEST_ID_XCONNECT,        /* Cross Connect detection test */
    VP_TEST_ID_LOOP_CONDITIONS, /* Measure current loop conditions */
    VP_TEST_ID_LOOPBACK,        /* Setup loopback conditions */
    VP_TEST_ID_LINE_IMPD,       /* Measure impedance at a frequency */
    VP_TEST_ID_RING_TRIP,       /* Test ability to detect ring trip */
    VP_TEST_ID_LOOP_DETECT,     /* Test ability to source current and detect
                                 * off-hook */
    VP_TEST_ID_CALIBRATE,       /* Perform line calibration */
    VP_TEST_ID_USE_LINE_CAL,    /* Copy dev cal factors from lineObj to devObj */
    VP_TEST_ID_OPEN_VXC,        /* Measure both DC and AC voltages */

    VP_TEST_ID_GR909_HEMF,      /* Hazardous Potential Voltage Test for AC/DC
                                 * Tip and Ring */
    VP_TEST_ID_GR909_EMF,       /* FEMF test for AC/DC Tip and Ring */
    VP_TEST_ID_GR909_RES_FLT,   /* Resistive Faults (T/R, T-Gnd, R-Gnd) */
    VP_TEST_ID_GR909_OFF_HOOK,  /* Receiver Off-Hook */
    VP_TEST_ID_GR909_RINGERS,   /* REN per FCC part 68 (1REN = 7Kohm) */
    VP_TEST_ID_GR909_ALL,       /* Run all GR909 tests */
    VP_TEST_ID_DELAY,           /* Implements non-blocking delay */
    VP_TEST_ID_RAMP_INIT,       /* Implements a ramp to voltage test init */
    VP_TEST_ID_RAMP,            /* Implements a ramp to voltage test */
    VP_TEST_ID_FLT_DSCRM,       /* Implements the Fault discrimination test (880 890 only)*/
    VP_TEST_ID_PREPARE_EXT,     /* Prepare for a test sequence without modifying the low power state  */

    VP_TEST_ID_SET_SENSE_GAIN_792, /* Set Low/Normal/High sense gain for VCP792.
                                    * The API translates this to the value of
                                    * VP_TEST_ID_SET_SENSE_GAIN before sending
                                    * it down to the VCP. */

    VP_NUM_TEST_IDS,

    VP_TEST_ID_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestIdType;

/**< The following are test line error codes */
typedef enum {
    VP_TEST_STATUS_SUCCESS            = 0,  /**< Test Completed Successfully */
    VP_TEST_STATUS_LINE_NOT_READY     = 1,  /**< Line Not in Test-mode (no VP_TEST_ID_PREPARE) */
    VP_TEST_STATUS_RESOURCE_NA        = 2,  /**< Line Not available for Test (already in Test) */
    VP_TEST_STATUS_TESTING            = 3,  /**< Test underway - not aborted */
    VP_TEST_STATUS_VOUT_OOR           = 4,  /**< Voltage Out-of-range */
    VP_TEST_STATUS_VAB_OOR            = 5,  /**< A-B Voltage is out-of-range */
    VP_TEST_STATUS_ILG_OOR            = 6,  /**< Measured IILG is out-of-range */
    VP_TEST_STATUS_VCM_OOR            = 7,  /**< Measured VCM is out-of-range */
    VP_TEST_STATUS_IAHL_OOR           = 8,  /**< Measured current or voltage is out-of-range in Res or Cap test */
    VP_TEST_STATUS_IBHL_OOR           = 9,  /**< " " */
    VP_TEST_STATUS_IAHH_OOR           = 10, /**< " " */
    VP_TEST_STATUS_IBHH_OOR           = 11, /**< " " */
    VP_TEST_STATUS_VABHL_OOR          = 12, /**< " " */
    VP_TEST_STATUS_IALL_OOR           = 13, /**< " " */
    VP_TEST_STATUS_IBLL_OOR           = 14, /**< " " */
    VP_TEST_STATUS_IA_OOR             = 15, /**< Measured Current is out-of-range */
    VP_TEST_STATUS_IB_OOR             = 16, /**< " " */
    VP_TEST_STATUS_IMT_OOR            = 17, /**< Measured IMT is out-of-range */
    VP_TEST_STATUS_IMT_DC_OOR         = 18, /**< " " */
    VP_TEST_STATUS_15KHZ_ERROR        = 19, /**< More than 1 15Khz Test requested */
    VP_TEST_STATUS_TIMEOUT            = 20, /**< Failed to complete in specified time */
    VP_TEST_STATUS_ABORTED            = 21, /**< Test aborted */
    VP_TEST_STATUS_INTERNAL_ERROR     = 22, /**< Internal error (SLAC integrator) */
    VP_TEST_STATUS_NO_CONVERGENCE     = 23, /**< NULL_IGND test failed to converge */
    VP_TEST_STATUS_TEST_NOT_SUPPORTED = 24, /**< Requested test is not supported (due to device configuration) */
    VP_TEST_STATUS_VTIPBIAS_OOR       = 25, /**< Tip Bias Voltage Out of Range */
    VP_TEST_STATUS_VRINGBIAS_OOR      = 26, /**< Ring Bias Voltage Out of Range */
    VP_TEST_STATUS_VMID_OOR           = 27, /**< User specified vMid voltage is out of range */
    VP_TEST_STATUS_GAIN_OOR           = 28, /**< User specified gain is out of range */
    VP_TEST_STATUS_VA_OOR             = 29, /**< Measured Voltage is out-of-range */
    VP_TEST_STATUS_VB_OOR             = 30, /**< " " */
    VP_TEST_STATUS_IMTOFFSET_OOR      = 31, 
    VP_TEST_STATUS_BUF_OVERFLOW       = 32, /* test would exceed the buffer storage available */
    VP_TEST_STATUS_NUM_TYPES,               /**< NOT an ERROR, Just used for coding purposes */
    VP_TEST_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestStatusType;

typedef struct {
    uint16 gMvErr;
    int16 gMvOff;
    uint16 gLvErr;
    int16 gLvOff;
    uint16 gMiErr;
    int16 gMiOff;
    uint16 gLiErr;
    int16 gLiOff;
    uint16 sAvErr;
    int16 sAvOff;
    uint16 sAiErr;
    int16 sAiOff;
    uint16 sBvErr;
    int16 sBvOff;
    uint16 sBiErr;
    int16 sBiOff;

    uint16 sABMvErr;
    int16 sABMvOff;
    uint16 sABMiErr;
    int16 sABMiOff;

    uint16 sABLvErr;
    int16 sABLvOff;
    uint16 sABLiErr;
    int16 sABLiOff;

    uint16 sABMHiErr;
    int16 sABMHiOff;
    uint16 sABLHiErr;
    int16 sABLHiOff;
} VpTestCalibrationDataType;

/* Definitions used for VP_TEST_ID_DELAY */
typedef struct {
    uint16 timerVal;    /* Value in 125uS steps */
} VpTestTimerType;

/* Definitions used for VP_TEST_ID_CONCLUDE */
typedef struct {
    bool abortTest;
} VpTestConcludeType;

/* Definitions used for VP_TEST_ID_SET_SENSE_GAIN */
typedef enum {
    VP_SENSE_GAIN_HIGH = 0,
    VP_SENSE_GAIN_LOW  = 1,
    VP_SENSE_GAIN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSenseGainType;

typedef struct {
    VpSenseGainType imt;
    VpSenseGainType ilg;
} VpTestSetSenseGainType;

/* 792-specific definitions for VP_TEST_ID_SET_SENSE_GAIN_VP792 */
typedef enum {
    VP792_SENSE_GAIN_0dB_GAIN   = 0x0000, /* absolute gain = 1 */
    VP792_SENSE_GAIN_28dB_GAIN  = 0x0010, /* absolute gain = 25 */
    VP792_SENSE_GAIN_10dB_LOSS  = 0x0020, /* absolute gain = 1/3 */
    VP792_SENSE_GAIN_RSVD       = 0x0030,
    VP792_SENSE_GAIN_ENUM_SIZE  = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} Vp792SenseGainType;

typedef struct {
    Vp792SenseGainType senseGain;
} VpTestSet792SenseGainType;

/* Definitions used for VP_TEST_ID_SET_BATTERIES */
typedef enum {
    VP_BS_LOW  = 0,
    VP_BS_HIGH = 1,
    VP_BS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSetBatteriesType;

typedef struct {
    VpSetBatteriesType bs1;
    VpSetBatteriesType bs0;
} VpTestSetBatteriesType;

/* Definitions used for VP_TEST_ID_OPEN_VDC, VP_TEST_ID_OPEN_VAC */
typedef enum {
    VP_TEST_RING     = 0,
    VP_TEST_TIP      = 1,
    VP_TEST_TIP_RING = 2,
    VP_TEST_TIP_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestTipSelectType;

typedef struct {
    bool calMode;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    VpTestTipSelectType tip;
} VpTestOpenVType;

/* Definitions used for VP_TEST_ID_DIFF_VAC */
typedef struct {
    uint16 integrateTime;   /* Integration time in units of 125us */
    int16 gvsaCal;
    int16 gvsbCal;
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestDiffVacType;

/* Definitions used for VP_TEST_IDC, VP_TEST_IAC */
typedef struct {
    bool calMode;
    int16 vCm;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestIdcType;

typedef struct {
    int16 gimtCal;
    int16 gilgCal;
    int16 vCm;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestIacType;

typedef struct {
    int16 ia;
    int16 ib;
} VpTestResultVixType;

/* Definitions used for VP_TEST_ID_DC_RLOOP */
typedef struct {
    bool calMode;
    int16 iTestLevel;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestDcRLoopType;

typedef struct {
    int16 vab;
    int16 ilg;
    bool currentSaturation;
} VpTestResultDcRlType;

/* Definitions used for VP_TEST_ID_AC_RLOOP */
typedef struct {
    bool calMode;
    uint16 freq;
    int16 vTestLevel;
    int16 vBias;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    VpTestTipSelectType tip; /* Used by VE880 only */
} VpTestAcRLoopType;

typedef struct {
    int16 imt;
    int16 ilg;
    int16 vab;
    int32 freq;
} VpTestResultAcRlType;

/* Definition used for VP_TEST_ID_AC_RLOOP_PHASE (in addition to VpTestAcRLoopType) */
typedef struct {
    int16 imt;
    int16 ilg;
    int16 phase;
} VpTestResultAcRlPhaseType;

/* Definitions used for VP_TEST_ID_LOOP_COND */
typedef enum {
    VP_LOOP_COND_TEST_VSAB  = 1,
    VP_LOOP_COND_TEST_VSAG  = 2,
    VP_LOOP_COND_TEST_VSBG  = 3,
    VP_LOOP_COND_TEST_RLOOP = 4,
    VP_LOOP_COND_TEST_ILG   = 5,
    VP_LOOP_COND_TEST_IMT   = 6,
    VP_LOOP_COND_TEST_BAT1  = 7,
    VP_LOOP_COND_TEST_BAT2  = 8,
    VP_LOOP_COND_TEST_BAT3  = 9,
    VP_LOOP_COND_TEST_METER = 10,
    VP_LOOP_COND_TEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLoopCondTestType;

typedef struct {
    bool calMode;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    VpLoopCondTestType loopCond; /* Indicates which condition to measure */
} VpTestLoopCondType;

typedef struct {
    VpLoopCondTestType condition;
    int16 data;
    bool calibrated;
    int16 limit;            /* Limit or target value */
} VpTestResultLoopCondType;

/* Definitions used for VP_TEST_ID_XCONNECT */
typedef enum {
    VP_XCONNECT_DISC_V    = 0,
    VP_XCONNECT_12VVOC_I  = 1,
    VP_XCONNECT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpXConnectType;

typedef struct {
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    VpXConnectType measReq; /* Indicates which measurement sequence to run */
    bool isFeedPositive;    /* VP_XCONNECT_12VVOC_I feed sign depends on measured EMF */
    uint8 phase;            /* Test phase number */
} VpTestXConnectType;

typedef struct {
    VpXConnectType condition;
    int16 data;
} VpTestResultXConnectType;

/* Definitions used for VP_TEST_ID_LOOPBACK */
typedef enum {
    VP_LOOPBACK_TEST_CODEC  = 1,
    VP_LOOPBACK_TEST_ANALOG = 2,
    VP_LOOPBACK_TEST_BFILTER = 3,
    VP_LOOPBACK_TEST_TIMESLOT = 4,
    VP_LOOPBACK_TEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLoopbackTestType;

typedef struct {
    uint16 waitTime;            /* Indicates how long before collecting samples */
    uint16 loopbackTime;        /* Indicates Loopback duration in units of 125us */
    VpLoopbackTestType loopback;/* Indicates which condition to setup */
} VpTestLoopbackType;

typedef struct {
    VpLoopbackTestType  loopback; /* Indicates which condition was setup*/
    void *pApplicationInfo;
} VPTestResultLoopbackType;

/* Definitions used for VP_TEST_LINE_IMPD */
typedef struct {
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    bool filterMode;
    int16 vcm;              /* Common mode voltage for measurement */
    int16 vDiff;            /* Differential voltage for measurement */
    uint16 freq;
    int16 amp;
} VpTestLineImpdType;

typedef struct {
    bool iaOor;         /* If set IA out of range */
    bool ibOor;         /* If set IB out of range */
    uint16 iDiff;           /* RMS differential current */
} VpTestResultLineImpdType;

/**< Typedefs for Advanced Tests */
/* Definitions used for VP_TEST_TONE_GEN */
typedef struct {
    uint16 duration;
    uint16 freq1;
    int16 amp1;
    uint16 freq2;
    int16 amp2;
    uint16 freq3;
    int16 amp3;
    uint16 freq4;
    int16 amp4;
} VpTestToneGenType;

/* Definitions used for VP_TEST_ALT_REN */
typedef struct {
    uint16 integrateTime;
    int16 vDiff;
} VpTestRenType;

typedef struct {
    int16 imt;
    int16 imtDc;
} VpTestResultRenType;

/* Definitions used for VP_TEST_ID_SLOPE_REN */
typedef enum {
    VP_TEST_TYPE_UNDEFINED = 0,
    VP_TEST_TYPE_AVG       = 1,
    VP_TEST_TYPE_RMS       = 2,
    VP_TEST_TYPE_DFT       = 3,
    VP_TEST_TYPE_RAW       = 4,
    VP_TEST_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestMathType;

typedef enum {
    VP_TEST_SPEED_UNDEFINED = 0,
    VP_TEST_SPEED_LOW       = 1,
    VP_TEST_SPEED_MED       = 2,
    VP_TEST_SPEED_HIGH      = 3,
    VP_TEST_SPEED_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestDcbwSpeedType;

typedef struct {
    uint16 slopeTime;
    uint16 integrateTime;
    uint16 settlingTime;
    int16 vDiff;
    VpTestMathType type;
    VpTestDcbwSpeedType speed;
} VpTestSRenType;

typedef struct {
    int16 imtOffset;
    int16 imt;
    int16 imtDc;
} VpTestResultSRenType;

/* Definitions used for VP_TEST_ID_3ELE_RES*/
typedef struct {
    bool calMode;
    int16 vCm;
    int16 vDiff;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestResType;

typedef struct {
    int16 iahl;
    int16 ibhl;
    int16 iahh;
    int16 ibhh;
    int16 iall;
    int16 ibll;
    int16 vabhl;
} VpTestResultResType;

/* Definition used for VP_TEST_ID_3ELE_HG and LG */
typedef struct {
    int16 feedBias;
    bool shiftZeroFeed;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTest3EleResAltResType;

typedef struct {
    bool highGain;
    int16 vanf;
    int16 vbnf;
    int16 imnf;
    int16 ilnf;
    int16 varf;
    int16 vbrf;
    int16 imrf;
    int16 ilrf;
    int16 vazf;
    int16 vbzf;
    int16 imzf;
    int16 ilzf;
} VpTestResultAltResType;

/* Definition used for VP_TEST_ID_MSOCKET_TYPE2 */
typedef struct {
    uint8 dummy;
} VpTestMSocketType;

typedef struct {
    int16 vanf;
    int16 vbnf;
    int16 imnf;
    int16 varf1;
    int16 vbrf1;
    int16 imrf1;
    int16 varf2;
    int16 vbrf2;
    int16 imrf2;
    int16 varf3;
    int16 vbrf3;
    int16 imrf3;
} VpTestResultMSockType;

/* Definition used for VP_TEST_ID_3ELE_CAP_CSLAC */
typedef struct {
    bool calMode;       /* Enable calibration */
    uint16 testAmp;     /* Test signal amplitude (mV RMS) */
    uint8 testFreq;     /* Test signal frequency [0,4], 0=2666.016Hz -> 4=280.151Hz */
    int16 bias;         /* Signal generator bias (V) */
} VpTest3EleCapAltResType;

typedef struct {
    bool accuracyFlag;
    uint8 freq;
    int16 va;
    int16 vb;
    int16 im;
    int16 il;
    int32 tipCapCal;
    int32 ringCapCal;
} VpTestResultAltCapType;

/* Definition used for VP_TEST_ID_3ELE_RES_VMID */
typedef struct {
    bool calMode;
    int16 vCm;
    int16 vDiff;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    int16 vMid;
} VpTestResVmidType;

typedef struct {
    int16 vdc;
    int16 vac;
} VpTestResultVxcType;

/* Definitions used for VP_TEST_3ELE_CAP */
typedef struct {
    bool calMode;
    int16 vCm;
    int16 vDiff;
    uint16 freq;
    int16 gimtCal;
    int16 gilgCal;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
} VpTestCapType;

typedef struct {
    int16 iahl;
    int16 iahh;
    int16 ibhh;
    int16 iall;
    int16 ibll;
    int16 vabhl;
} VpTestResultCapType;

/* Definitions used for VP_TEST_NOISE */
typedef enum {
    VP_FILTER_NONE   = 0,
    VP_FILTER_3KHZ   = 1,
    VP_FILTER_CMSG   = 2,
    VP_FILTER_15KHZ  = 3,
    VP_FILTER_3_4KHZ = 4,
    VP_FILTER_D      = 5,
    VP_FILTER_PSOPH  = 6,
    VP_NUM_TEST_FILTER_TYPES,
    VP_FILTER_ENUM_RSVD    = FORCE_SIGNED_ENUM,
    VP_FILTER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestFilterType;

typedef struct {
    uint16 integrateTime;
    VpTestFilterType filter;
    int16 amp;
    bool useBuffer;
} VpTestNoiseType;

/* Definitions used for VP_TEST_HYBRID_LOSS */
typedef struct {
    uint16 integrateTime;
    uint16 freq;
    int16 amp;
    bool useBuffer;
} VpTestTransHybridLossType;

/* Definitions used for VP_TEST_ID_NULL_IGND */
typedef struct {
    int16 vTestLevel;
    int16 iTarget;
    VpTestTipSelectType tip;
    uint16 settlingTime;
} VpTestNullIGndType;

/* Definitions used for VP_TEST_KEYPAD */
typedef enum {
    VP_KEY_TEST_DTMF  = 0,
    VP_KEY_TEST_PULSE = 1,
    VP_KEY_TEST_BOTH  = 2,
    VP_NUM_KEY_TEST_TYPES,
    VP_KEY_TEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpKeyTestType;

typedef struct {
    VpKeyTestType kTest;
    uint16 timeout;
    uint16 freq;
    int16 amp;
    uint16 fftSize;
    uint16 threshold;
    VpOptionPulseType pulseOpt;
    bool useBuffer;
} VpTestKeypadType;

typedef enum {
    VP_KT_DTMF_DET   = 0,
    VP_KT_DTMF_MES   = 1,
    VP_KT_PULSE      = 2,
    VP_KT_THRESH_DET = 3,
    VP_NUM_KT_TYPES,
    VP_KT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpKeyTestResultType;

typedef struct {
    VpKeyTestResultType kTestResult;
    VpDigitType digit;
    uint16 lowFreq;
    uint16 lowAmp;
    uint16 highFreq;
    uint16 highAmp;
    uint16 fftSize;
    uint16 minBreak;
    uint16 maxBreak;
    uint16 aveBreak;
    uint16 pulsePerSec;
    bool hookFlashDet;
    uint16 hookFlashDuration;
    bool disconnectDet;
} VpTestResultKeyType;

/* Definitions used for VP_TEST_SNR_QDIST */
typedef struct {
    uint16 integrateTime;          /* Integration time in units of 125us */
    int16 amp;
    bool useBuffer;
} VpTestSnrQDistorType;

typedef struct {
    uint16 vrmsNoise;
    uint16 vrmsSignal;
} VpTestResultSnrQDistorType;

/* Definitions used for VP_TEST_ARB_1TONE */
typedef struct {
    uint16 timeout;
    uint16 fftSize;
    uint16 threshold;
    bool useBuffer;
} VpTestArb1ToneType;

/* Definitions used for VP_TEST_DIALTONE */
typedef struct {
    bool inward;
    uint16 timeout;
    uint16 detectGran;
    uint16 threshold;
    uint16 fftSize;
    uint16 duration;
    bool useBuffer;
} VpTestDialToneType;

typedef struct {
    uint16 rmsPwr;
    uint16 highFreq;
    uint16 highAmp;
    uint16 secFreq;
    uint16 secAmp;
    uint16 thdFreq;
    uint16 thdAmp;
} VpTestResultToneType;

/**< The following types are for GR909 tests */
/* Definitions used for  */
typedef struct {
    bool testFail;
    bool acFault;
    bool dcFault;
    uint16 acVolt;
    int16 dcVolt;
} VpTestResultGR909EmfType;

/* Definitions used for VP_TEST_ID_GR909_RES_FLT */
typedef struct {
    bool testFail;
    bool rtrFlt;
    bool rtgFlt;
    bool rrgFlt;
    uint16 rtr;
    uint16 rtg;
    uint16 rrg;
} VpTestResultGR909ResFltType;

/* Definitions used for VP_TEST_ID_GR909_OFF_HOOK */
typedef struct {
    bool offHook;
} VpTestResultGR909OffHookType;

/* Definitions used for VP_TEST_ID_GR909_RINGERS */
typedef struct {
    bool testFailed;
    bool renHigh;
    bool renLow;
    uint16 ren;
} VpTestResultGR909RenType;

/* Definitions used for VP_TEST_ID_GR909_ALL */
typedef struct {
    bool testFailed;
    bool hvFailed;
    bool emfFailed;
    bool resFailed;
    bool offHook;
    bool ringersFailed;
} VpTestResultGR909AllType;

/* Definitions used for VP_TEST_ID_RAMP_INIT */
typedef struct {
    int16 dcVstart;
    int16 dcVend;
    int16 vRate;
    int16 bias;
    VpTestTipSelectType tip;
} VpTestRampInitType;

/* Definitions used for VP_TEST_ID_RAMP */
typedef struct {
    int16 dcVoltage;
    int16 vRate;
    int16 bias;
    VpTestTipSelectType tip;
} VpTestRampType;

typedef struct {
    int16 imt;
    int16 vRate;
} VpTestResultRampType;

/* Definitions used for VP_TEST_ID_MONITOR_IV */
typedef enum {
    VP_TEST_TYPE_DC = 0,
    VP_TEST_TYPE_AC = 1,
    VP_TEST_TYPE_ACDC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpMonitorIvFilterType;

typedef enum {
    VP_TEST_MEAS_VA  = 0,
    VP_TEST_MEAS_VB  = 1,
    VP_TEST_MEAS_VAB = 2,
    VP_TEST_MEAS_IMT = 3,
    VP_TEST_MEAS_ILG = 4,
    VP_TEST_MEAS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpMonitorIvTestMeasurementType;

typedef struct {
    bool calMode;
    uint16 integrateTime;               /* Integration time in units of 125us */
    uint16 settlingTime;                /* Filter settling time in units of 125us */
    VpMonitorIvFilterType filterType;   /* Measurement Type (AC/DC) */
    VpMonitorIvTestMeasurementType measurement; /* measurement source */
} VpTestMonitorIvType;

/* Definitions used for VP_TEST_ID_UNBAL_TONE */
typedef struct {
    uint16 freq;
    int16 vTestlevel;
    int16 vTipBias;
    int16 vRingBias;
    uint16 duration;
    VpTestTipSelectType tip;
} VpTestUnbalToneType;

/* Definitions used for VP_TEST_ID_PCM_COLLECT */
typedef struct {
    uint16 sampleTime;
} VpTestPcmCollectType;

typedef struct {
    uint16 resultsSize;
    uint16 samples;
    VpOptionCodecType codec;
    uint16 reserved;
    uint32 address;
} VpTestResultPcmCollectType;

/* Definitions used for VP_FLT_DSCRM */
typedef struct {
    int16 vtgFeed;
    int16 vrgFeed;
    int16 vtrFeed;
    int16 vtrAC;
    int16 vtoAC;
    int16 vroAC;
    int16 vtuBalAC;
    int16 vruBalAC;
    bool hookDet;
    bool gnkDet;
} VpTestResultFltDscrmType;

/* Definitions for VP_TEST_ID_GEN_TEST */
/* Generic Test Defines */
typedef enum {
    VP_TEST_FILTER_UNDEFINED = 0,
    VP_TEST_FILTER_LPF       = 1,
    VP_TEST_FILTER_HPF       = 2,
    VP_TEST_FILTER_NONE      = 3,
    VP_TEST_FILTER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestGenFilterType;

typedef enum {
    VP_TEST_DRIVE_UNDEFINED = 0,
    VP_TEST_DRIVE_NONE      = 1,
    VP_TEST_DRIVE_A         = 2,
    VP_TEST_DRIVE_B         = 3,
    VP_TEST_DRIVE_BOTH      = 4,
    VP_TEST_DRIVE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestGenDriveType;

typedef enum {
    VP_TEST_SIGNAL_UNDEFINED = 0,
    VP_TEST_SIGNAL_A         = 1,
    VP_TEST_SIGNAL_B         = 2,
    VP_TEST_SIGNAL_CM        = 3,
    VP_TEST_SIGNAL_DIFF      = 4,
    VP_TEST_SIGNAL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestGenSignalType;

typedef enum {
    VP_TEST_MEAS_UNDEFINED     = 0,
    VP_TEST_MEAS_LOW_GAIN      = 1,
    VP_TEST_MEAS_NORM_GAIN     = 2,
    VP_TEST_MEAS_HIGH_GAIN     = 3,
    VP_TEST_MEAS_SKIP          = 4,
    VP_TEST_MEAS_IMT_NORM_GAIN = 5,
    VP_TEST_MEAS_IMT_HIGH_GAIN = 6,
    VP_TEST_MEAS_VAB_LOW_GAIN  = 7,
    VP_TEST_MEAS_VAB_NORM_GAIN = 8,
    VP_TEST_MEAS_VAB_HIGH_GAIN = 9,
    VP_TEST_MEAS_ILG_NORM_GAIN = 10,
    VP_TEST_MEAS_ILG_HIGH_GAIN = 11,
    VP_TEST_MEAS_GEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestGenGainType;

typedef struct {
    VpTestGenFilterType filter;
    VpTestMathType type;
    VpTestGenDriveType  driveMode;
    VpTestGenSignalType  signalMode;
    int16 vTestLevel;
    uint16 freq;
    int16 vTipBias;
    int16 vRingBias;
    uint16 integrateTime;   /* Integration time in units of 125us */
    uint16 settlingTime;    /* Filter settling time in units of 125us */
    VpTestGenGainType iaMeas;
    VpTestGenGainType vaMeas;
    VpTestGenGainType ibMeas;
    VpTestGenGainType vbMeas;
} VpTestGenTestType;

typedef struct
{
    int16 iaReal;
    int16 iaImag;
    int16 vaReal;
    int16 vaImag;
    int16 ibReal;
    int16 ibImag;
    int16 vbReal;
    int16 vbImag;
} VpTestResultGenTestType;

/* Definitions for Test results */
typedef union {
    bool                          hstatus;
    int16                         sVout;
    uint16                        uVout;
    int16                         vcm;
    uint16                        vrms;
    VpTestResultVxcType           vxc;
    VpTestResultDcRlType          dcvab;
    VpTestResultAcRlType          acimt;
    VpTestResultAcRlPhaseType     acimtp;
    VpTestResultVixType           vix;
    VpTestResultResType           res;
    VpTestResultAltResType        resAlt;
    VpTestResultMSockType         mSock;
    VpTestResultXConnectType      xConnect;
    VpTestResultAltCapType        capAlt;
    VpTestResultCapType           cap;
    VpTestResultKeyType           keypad;
    VpTestResultSnrQDistorType    snrqd;
    VpTestResultToneType          tone;
    VpTestResultRenType           ren;
    VpTestResultLoopCondType      loopCond;
    VPTestResultLoopbackType      loopback;

    bool                          calFailed;
    bool                          ringTripFail;
    bool                          loopFnd;
    bool                          loopBackTestFail;
    VpTestResultLineImpdType      lineImpedance;
    VpTestResultGR909EmfType      emf;
    VpTestResultGR909ResFltType   resFault;
    VpTestResultGR909OffHookType  offHookFault;
    VpTestResultGR909RenType      ringers;
    VpTestResultGR909AllType      gr909All;
    VpTestResultRampType          ramp;
    VpTestResultPcmCollectType    pcmCollect;
    VpTestResultFltDscrmType      fltDscrm;
    VpTestResultGenTestType       gen;
    VpTestResultSRenType          sren;
} VpTestResultsUnionType;

typedef struct {
    VpTestIdType testId;            /* Test identifier */
    VpTestStatusType errorCode;     /* Error code if Test Failed */
    VpTestResultsUnionType result;  /* Return Results Union */
} VpTestResultType;

/* The following enum is used for event associated with VpSelfTest() */
typedef enum
{
    VP_STEST_SUCCESS = 0,
    VP_STEST_FAIL    = 1,
    VP_STEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSelfTestResultIdType;

#endif /* VP_API_TEST_H */
