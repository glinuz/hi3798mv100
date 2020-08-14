/** \file vp890_api.h
 * vp890_api.h
 *
 *  Header file that define all the commands for the Vp890 device.
 *
 * $Revision: 7484 $
 * $LastChangedDate: 2010-07-13 11:55:33 -0500 (Tue, 13 Jul 2010) $
 */

#ifndef VP890_API_H
#define VP890_API_H

#include "vp_hal.h"
#include "vp_CSLAC_types.h"
#include "vp_api_common.h"

#ifdef VP890_INCLUDE_TESTLINE_CODE
#include "vp_api_test.h"
  #ifdef VP890_EZ_MPI_PCM_COLLECT
  #include  "vp_pcm_compute.h"
  #endif
#endif

#define VP890_MAX_NUM_CHANNELS   2

#define VP890_MAX_MPI_DATA      15   /* Max data from any MPI read command */
#define VP890_INT_SEQ_LEN       22

#ifndef VP890_UL_SIGREG_LEN
#define VP890_UL_SIGREG_LEN         0x02
#endif

#ifndef VP890_B1_FILTER_LEN
#define VP890_B1_FILTER_LEN         0x0E
#endif

#ifndef VP890_B2_FILTER_LEN
#define VP890_B2_FILTER_LEN         0x02
#endif

#ifndef VP890_SIGA_PARAMS_LEN
#define VP890_SIGA_PARAMS_LEN       0x0B
#endif

#ifndef VP890_DC_FEED_LEN
#define VP890_DC_FEED_LEN           0x02
#endif


#ifndef VP890_ICR1_LEN
#define VP890_ICR1_LEN              0x04
#endif

#ifndef VP890_ICR2_LEN
#define VP890_ICR2_LEN              0x04
#endif

#ifndef VP890_ICR3_LEN
#define VP890_ICR3_LEN              0x04
#endif

#ifndef VP890_ICR4_LEN
#define VP890_ICR4_LEN              0x04
#endif

#ifndef VP890_DC_CAL_REG_LEN
#define VP890_DC_CAL_REG_LEN        0x02
#endif

#ifndef VP890_LOOP_SUP_LEN
#define VP890_LOOP_SUP_LEN          0x04
#endif

#ifndef VP890_TEST_DATA_LEN
#define VP890_TEST_DATA_LEN         0x0E
#endif

#ifndef VP890_REGULATOR_PARAM_LEN
#define VP890_REGULATOR_PARAM_LEN   0x03
#endif

#ifndef VP890_REGULATOR_TIMING_LEN
#define VP890_REGULATOR_TIMING_LEN  0x06
#endif

#ifndef VP890_REGULATOR_TIMING_LEN
#define VP890_REGULATOR_TIMING_LEN  0x06
#endif

#ifndef VP890_CONV_CFG_LEN
#define VP890_CONV_CFG_LEN      0x01
#endif

/**< Required Vp890 Device and Line Objects for user instantiation if a Vp890
 * device is used
 */

/**< Structure that defines the Vp890 Device Profile. Current as of the first
 * Device Profile version (ver 0).
 */
typedef struct {
    uint8   profVersion;
    uint16  pcmClkRate;      /**< Used to verify valid TX/RX Timeslot setting */
    uint8   mClkMask;
    uint16  tickRate;        /**< Primary API-II tick for this device */
    uint8   devCfg1;
    uint8   clockSlot;

#ifdef VP890_FXS_SUPPORT
    uint8   swParams[VP890_REGULATOR_PARAM_LEN];
    uint8   timingParams[VP890_REGULATOR_TIMING_LEN];
    uint8   timingParamsFR[VP890_REGULATOR_TIMING_LEN];

    bool    peakManagement;
    bool    lowVoltOverride;
#endif
} Vp890DeviceProfileType;

#if defined (VP890_INCLUDE_TESTLINE_CODE)

/* Definitions for Test arguments */
typedef union {
    VpTestConcludeType          conclude;
    VpTestOpenVType             openV;
    VpTestDcRLoopType           dcRloop;
    VpTestAcRLoopType           acRloop;
    VpTest3EleResAltResType     resFltAlt;
    VpTestMSocketType           mSocket;
    VpTestXConnectType          xConnect;
    VpTest3EleCapAltResType     capAlt;
    VpTestLoopCondType          loopCond;
    VpTestLoopbackType          loopback;
    VpTestRampType              ramp;
    VpTestRampInitType          rampInit;
} Vp890TestArgsType;

typedef struct {
    uint8 slacState;
    uint8 vpGain;
    uint8 opCond;
    uint8 opFunction;
    uint8 switchTimes[VP890_REGULATOR_TIMING_LEN];
    uint8 icr2[VP890_ICR2_LEN];
    uint8 icr3[VP890_ICR3_LEN];
    uint8 icr4[VP890_ICR4_LEN];
    uint8 icr6[VP890_DC_CAL_REG_LEN];
} Vp890LineTestCalType;

typedef struct {
    uint8 adcState;
    int16 nextState; /**< Used when a pcm collection routine is started */

    Vp890TestArgsType testArgs; /**< Saved test input arguments of current test */

    uint8 opCond;                           /**< Saved Operation Condition */
    uint8 opFunction;                       /**< Saved Operation Functions */
    uint8 opMode;                           /**< Saved Operating Mode */
    uint8 sysConfig;                        /** Saved System Configurations */
    uint8 sigCtrl;                          /**< Signal Generator Control */
    uint8 convCfg;                          /**< Saved Converter Config */
    uint8 slacState;                        /**< Saved Slac State */
    uint8 vpGain;                           /**< Voice Path Gain */
    uint8 disn;                             /**< Digital Imped. Scaling Network */
    uint8 lpSuper[VP890_LOOP_SUP_LEN];      /**< Saved Loop Sup. Parameters */
    uint8 dcFeed[VP890_DC_FEED_LEN];        /**< Saved DC Feed Parameters */
    uint8 switchReg[VP890_REGULATOR_PARAM_LEN]; /**< Switching Reg Parameters */
    uint8 switchTimes[VP890_REGULATOR_TIMING_LEN]; /**< Switching Reg Timing */

    uint8 icr1[VP890_ICR1_LEN];
    uint8 icr2[VP890_ICR2_LEN];
    uint8 icr3[VP890_ICR3_LEN];
    uint8 icr4[VP890_ICR4_LEN];
    uint8 icr6[VP890_DC_CAL_REG_LEN];
    uint8 sigGenAB[VP890_SIGA_PARAMS_LEN]; /**< Saved Signal Generator A & B*/
    uint8 b1Filter[VP890_B1_FILTER_LEN];   /**< Saved B1 filter coefficients */
    uint8 b2Filter[VP890_B2_FILTER_LEN];   /**< Saved B2 filter coefficients */

    /* used for collecting PCM data */
    bool pcmRequest;        /** < indication that pcm data was requested */
    VpPcmOperationMaskType operationMask;

    VpPcmOperationResultsType pcmResults; /** < stores the pcm operation results */

    /* Used for common setup functions */
    uint16 commonSetupState;

    /* Used for storing line event mask data */
    VpOptionEventMaskType preTestEventMask;

    /* Used for saving and restoring slac state after calibration */
    uint8 calSlacState;

     /* Used for saving and restoring registers during calibration */
    Vp890LineTestCalType calRegs;

    /* Used for resflt lg speed up*/
    uint16 speedupTime;
    int16 previousAvg;
    int16 vabComputed;
    uint8 loopCnt;
    bool compensate;
    bool lowGain;

    /* Used in the capacitance test */
    int16 adcSampleBuffer[52];
    uint8 requestedSamples;
    uint8 saveConvConfig[VP890_CONV_CFG_LEN];
    bool xtraBuffer;

    /* The following members are for EZ mode calculations only*/
#ifdef VP890_EZ_MPI_PCM_COLLECT
    VpPcmComputeTempType ezPcmTemp;

    /* Used to debug under and overflow pcm collect conditions */
#ifdef VP_DEBUG
    int8 underFlowValue;
    uint32 overCnt;
    uint32 underCnt;
#endif

#endif

} Vp890TestHeapType;

typedef struct {
    Vp890TestHeapType *pTestHeap;
    uint8 testHeapId;

    uint8 channelId;    /**< Channel # for "this" line on the device.  Indexed
                         * starting at 0, should not exceed the max number of
                         * lines supported by the device - 1 (max = 2, then
                         * channelId = {0, 1}
                         */

    bool prepared;          /**< indicates if the current test is prepared */
    bool preparing;         /**< indicates if the test prepare is complete */
    bool concluding;        /**< indicates that the device is concluding a test */
    bool nonIntrusiveTest;  /**< indicates a "stealth" test */
    VpTestIdType testId;    /** < indicates the test currently running */

    int16 testState;        /**< maintains the currnt state of the current TestId */
    uint16 handle;

} Vp890CurrentTestType;

typedef struct {
    int16 nullOffset;
    int16 vabOffset;
    int16 vahOffset;
    int16 valOffset;
    int16 vbhOffset;
    int16 vblOffset;
    int16 batOffset;
    int16 imtOffset;
    int16 ilgOffset;
} Vp890CalOffCoeffs;

#endif /*VP890_INCLUDE_TESTLINE_CODE*/

typedef enum {
    VP890_CAL_INIT = 0,
    VP890_CAL_ERROR,
    VP890_CAL_OFFSET,
    VP890_CAL_OFFSET_VAG_VOC,
    VP890_CAL_VOC_OFFSET,
    VP890_CAL_MEASURE,
    VP890_CAL_SIGEN_A_PHASE1,
    VP890_CAL_SIGEN_A_PHASE2,
    VP890_CAL_SIGEN_A_PHASE3,
    VP890_CAL_SIGEN_A_PHASE4,
    VP890_CAL_MEASURE_2,
    VP890_CAL_FIRST_IMT_SET,
    VP890_CAL_FIRST_IMT_READ,
    VP890_CAL_IMT_OFFSET_SET,
    VP890_CAL_IMT_OFFSET_READ,
    VP890_CAL_IMT_OFFSET_SET_INVERT,
    VP890_CAL_IMT_OFFSET_READ_INVERT,
    VP890_CAL_VOC_READ_INVERT,
    VP890_CAL_IMT_OFFSET_SET_DONE,
    VP890_CAL_IMT_OFFSET_READ_DONE,
    VP890_CAL_VOC_READ_DONE,
    VP890_CAL_FIRST_VOC_READ,
    VP890_CAL_CONVERTER_CHANGE,
    VP890_CAL_ADC,
    VP890_CAL_STATE_CHANGE,
    VP890_CAL_INVERT_POL,
    VP890_CAL_INVERT_POL_VOC,
    VP890_CAL_INVERT_POL_VAG_TGND,
    VP890_CONVERTER_CHECK,
    VP890_CAL_DONE,
    VP890_CAL_VAG_DONE,
    VP890_CAL_DONE_VOC_MEASURE,
    VP890_CAL_DONE_PRE,
    VP890_CAL_EXIT
} Vp890CalState;

typedef enum {
    VP890_BFILT_MEASURE,
    VP890_BFILT_END
} Vp890BFiltCalStates;

#ifndef VP890_B1_FILTER_LEN
#define VP890_B1_FILTER_LEN         0x0E
#endif

#ifndef VP890_B2_FILTER_LEN
#define VP890_B2_FILTER_LEN         0x02
#endif

#ifndef VP890_OP_FUNC_LEN
#define VP890_OP_FUNC_LEN           0x01
#endif

#ifndef VP890_OP_COND_LEN
#define VP890_OP_COND_LEN           0x01
#endif

#ifndef VP890_CONV_CFG_LEN
#define VP890_CONV_CFG_LEN          0x01
#endif

#ifndef VP890_VP_GAIN_LEN
#define VP890_VP_GAIN_LEN           0x01
#endif

#ifndef VP890_DISN_LEN
#define VP890_DISN_LEN              0x01
#endif

#ifndef VP890_SYS_STATE_LEN
#define VP890_SYS_STATE_LEN         0x01
#endif

#ifndef VP890_GEN_CTRL_LEN
#define VP890_GEN_CTRL_LEN          0x01
#endif

#ifndef VP890_DEV_MODE_LEN
#define VP890_DEV_MODE_LEN          0x01
#endif

#ifndef VP890_CID_PARAM_LEN
#define VP890_CID_PARAM_LEN         0x01
#endif

typedef struct {
    VpCalBFilterType inputData;
    uint8 b1FiltData[VP890_B1_FILTER_LEN];
    uint8 b2FiltData[VP890_B2_FILTER_LEN];
    uint8 opFunct[VP890_OP_FUNC_LEN];
    uint8 opCond[VP890_OP_COND_LEN];
    uint8 convCfg[VP890_CONV_CFG_LEN];
    uint8 vpGain[VP890_VP_GAIN_LEN];
    uint16 vRms;
    uint8 listLength;   /* In number of B-Filter coefficient sets */
    int8 currentSet;    /* Number of currently running B-Filter set */
    int8 bestSet;       /* Number of the best set so far */
} Vp890CalBFilterData;

#define VP890_BFILT_SAMPLE_TIME     (70)

typedef enum {
    VP890_RESET             = 0x0000,   /**< Set at the begining of init device */
    VP890_IS_HIGH_VOLTAGE   = 0x0010,   /**< Set when device is high voltage device */
    VP890_IS_SINGLE_CHANNEL = 0x0020,   /**< Set when a single channel device is found*/
    VP890_DEVICE_DETECTED   = 0x0040,   /**< Set when the device is detected */
    VP890_LINE0_IS_FXO      = 0x0080,   /**< Set if device detection indicates line0 as FXO */
    VP890_LINE1_IS_FXO      = 0x0100,   /**< Set if device detection indicates line1 as FXO */
    VP890_WIDEBAND          = 0x0200,   /**< Set if device supports Wideband mode */
    VP890_LINE0_LP          = 0x0400,   /**< Set if line 0 allows low power mode */
    VP890_LINE1_LP          = 0x0800,   /**< Set if line 1 allows low power mode */
    VP890_IS_FXO_ONLY       = 0x1000,   /**< Set when the device contains only FXO lines */
    VP890_SYS_CAL_COMPLETE  = 0x2000,   /**< Set when the system calibration structure has been initialied */
    VP890_CAL_RELOAD_REQ    = 0x4000,   /**< Set when the line calibration values need to be reloaded. */
    VP890_FORCE_FREE_RUN    = 0x8000    /**< Set when app calls VpFreeRun() (start), cleared when called with stop.
                                         * This prevents the VP-API-II from automatically exiting free run mode
                                         * upon PCLK recovery.
                                         */
} Vp890DeviceStateIntType;


/**< Line Status types to minimize code space in line object (compared to each
 * status being maintined by a uint8 type)
 */
typedef enum {
    VP890_INIT_STATUS   = 0x0000,

    VP890_IS_FXO        = 0x0001,   /**< Set if the line is configured for FXO */

    VP890_BAD_LOOP_SUP  = 0x0002,   /**< Set when the Loop Supervision has been
                                     * changed in such a way inconsistent with
                                     * the user's specifications. This is done
                                     * in internal to the API to make some
                                     * functions work (e.g., Msg Wait Pulse).
                                     */

    VP890_DP_SET1_DONE  = 0x0004,   /**< Set when Dial Pulse detection machine
                                     * is "done" on the current dial pulse using
                                     * the "first" set of DP parameters
                                     */

    VP890_DP_SET2_DONE  = 0x0008,   /**< Set when Dial Pulse detection machine
                                     * is "done" on the current dial pulse using
                                     * the 2nd set of DP parameters
                                     */

    VP890_SLS_CALL_FROM_API = 0x0010,   /**< Set if Set Line State is called
                                         * from an API function (e.g., cadence).
                                         */

    VP890_LINE_IN_CAL = 0x0020,     /**< Set while line running cal routine */

    VP890_LOW_POWER_EN = 0x0080,    /**< Set when line is operating in low power
                                     * mode
                                     */

    VP890_LINE_LEAK = 0x0100,      /**< Set when leakage is detected on the line
                                    * such that low power mode is prevented.
                                    */
    VP890_CHECK_LEAK = 0x0200,     /**< Set when API-II is testing for leakage
                                    * on the line
                                    */
    VP890_INIT_COMPLETE = 0x0800,  /**< Set when InitLine has been completed
                                    * on this line.
                                    */
    VP890_PREVIOUS_HOOK = 0x1000,  /**< Set if Last Hook Event reported was
                                    * off-hook, cleared if last event was
                                    * on-hook.
                                    */
    VP890_RING_GEN_NORM = 0x2000,  /**< Set when the Generators are known last
                                    * to be programmed to the application
                                    * specified ringing profile. Cleared
                                    * when line tests are run because the same
                                    * generator is used for non-ringing.
                                    */
    VP890_RING_GEN_REV = 0x4000    /**< Set when the Generators are known last
                                    * to be programmed to the application
                                    * specified ringing profile. Cleared
                                    * when line tests are run because the same
                                    * generator is used for non-ringing.
                                    */
} Vp890LineStatusType;

typedef struct {
    uint16  volts;
    uint16  gain;
} Vp890CidCorrectionPair;

typedef enum {
    VP890_PLL_RECOVERY_ST_RESET,
    VP890_PLL_RECOVERY_ST_DISABLE,
    VP890_PLL_RECOVERY_ST_ENABLE_1,
    VP890_PLL_RECOVERY_ST_ENABLE_2,
    VP890_PLL_RECOVERY_ST_MEASURE,
    VP890_PLL_RECOVERY_ST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req*/
} Vp890PllRecoveryStateType;

typedef struct {
    int16 imtPrev;
    bool secondPass;
} Vp890VasCalData;

typedef struct {
    int16 ilaNorm;
    int16 ilgNorm;

    /* This is to be backward compatible with VVA P1.1.0 */
    int16 ilaOffsetNorm;
} Vp890IlaCalData;

typedef struct {
    int16 vocNorm;
    int16 vocRev;

    /* This is to be backward compatible with VVA P1.1.0 */
    int16 vocOffsetNorm;
    int16 vocOffsetRev;
} Vp890VocCalData;

typedef struct {
    int16 vagNorm;
    int16 vagRev;

    /* This is to be backward compatible with VVA P1.1.0 */
    int16 vagOffsetNorm;
    int16 vagOffsetRev;
} Vp890VagCalData;

typedef struct {
    int16 vbgNorm;
    int16 vbgRev;

    /* This is to be backward compatible with VVA P1.1.0 */
    int16 vbgOffsetNorm;
    int16 vbgOffsetRev;
} Vp890VbgCalData;

typedef struct {
    int16 swyVolt[1];   /**< One per channel used to measure SWY Voltage */

    /* This is to be compatible with VVA P1.1.0 */
    int16 swyOffset[1];
} Vp890AbvCalData;

typedef struct {
    Vp890VasCalData vasData;
    Vp890IlaCalData ilaData;
    Vp890VocCalData vocData;
    Vp890VagCalData vagData;
    Vp890VbgCalData vbgData;
    Vp890AbvCalData abvData;
    Vp890CalBFilterData bFilterData;
} Vp890CalTypeData;

typedef struct {
    bool calDone;           /**< TRUE when calibration has been performed on
                             * this line
                             */
    bool reversePol;        /**< TRUE when line is in reverse polarity */

    Vp890CalTypeData typeData;

    uint8 codecReg;
    int16 calSet;                               /* user set for all Cal */

    uint8 dcFeedRef[VP890_DC_FEED_LEN];         /* Copied from dc profile */
    uint8 dcFeed[VP890_DC_FEED_LEN];            /* use for VAS and VOC */
    uint8 dcFeedPr[VP890_DC_FEED_LEN];          /* use for VAS and VOC */

    Vp890CalState calState;
    uint8 sysState[VP890_SYS_STATE_LEN];

    uint8 icr2[VP890_ICR2_LEN];

    uint8 disnVal[VP890_DISN_LEN];
    uint8 vpGain[VP890_VP_GAIN_LEN];

    bool forceCalDataWrite;
    bool convCfgChangeReq;

    /* Signal generator calibration temporary backup */
    uint8 sigGenA[VP890_SIGA_PARAMS_LEN];
    uint8 calReg[VP890_DC_CAL_REG_LEN];
} Vp890CalLineData;

typedef enum {
    VP890_CURRENT_MONITOR_DISABLED,
    VP890_CURRENT_MONITOR_OFFSET,
    VP890_CURRENT_MONITOR_BUFFER0,
    VP890_CURRENT_MONITOR_BUFFER1,
    VP890_CURRENT_MONITOR_BUFFER2,
    VP890_CURRENT_MONITOR_BUFFER3,
    VP890_CURRENT_MONITOR_BUFFER4,
    VP890_CURRENT_MONITOR_NORMAL
} Vp890FxoCurrentMonitorStates;

#define VP890_PCM_BUF_SIZE    5
typedef struct {
    Vp890FxoCurrentMonitorStates    stateValue;
    int16                           currentBuffer[VP890_PCM_BUF_SIZE];
    int16                           currentOffset;

    uint8                           offsetMeasurements;

    /* This is updated when delta across all data is minimal */
    int32                           steadyStateAverage;

    /* Set TRUE when last set of data experienced a line transient */
    bool                            invalidData;
} Vp890FxoCurrentMonitorType;

typedef struct {
    bool enabled;
    uint8 numDisc;
    uint8 numNotDisc;
} Vp890FxoLowVoltageMonitorType;

#define VP890_SYS_CAL_POLARITY_LENGTH  2
#define VP890_SYS_CAL_CHANNEL_LENGTH   1

/* Contains calibration error values -- in +/-10mv LSB */
typedef struct {
    int16 abvError[1];

    int16 vocOffset[VP890_SYS_CAL_CHANNEL_LENGTH][VP890_SYS_CAL_POLARITY_LENGTH];
    int16 vocError[VP890_SYS_CAL_CHANNEL_LENGTH][VP890_SYS_CAL_POLARITY_LENGTH];

    int16 sigGenAError[VP890_SYS_CAL_CHANNEL_LENGTH][VP890_SYS_CAL_POLARITY_LENGTH];

    int16 ila20[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 ila25[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 ila32[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 ila40[VP890_SYS_CAL_CHANNEL_LENGTH];

    int16 ilaOffsetNorm[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 ilgOffsetNorm[VP890_SYS_CAL_CHANNEL_LENGTH];

    uint8 vas[VP890_SYS_CAL_CHANNEL_LENGTH][VP890_SYS_CAL_POLARITY_LENGTH];

    int16 vagOffsetNorm[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 vagOffsetRev[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 vbgOffsetNorm[VP890_SYS_CAL_CHANNEL_LENGTH];
    int16 vbgOffsetRev[VP890_SYS_CAL_CHANNEL_LENGTH];

    int16 swyOffset[VP890_SYS_CAL_CHANNEL_LENGTH];     /**< Used to hold SWY Offset */

    /* Used for capacitance line test only */
    int32 tipCapCal[VP890_SYS_CAL_CHANNEL_LENGTH];
    int32 ringCapCal[VP890_SYS_CAL_CHANNEL_LENGTH];
} Vp890SysCalResultsType;

#define VP890_CAL_STRUCT_SIZE   (18*sizeof(int16) + 2*sizeof(uint8) + 2*sizeof(int32))

/* Vp890 specific Device Object */
typedef struct {
    VpDeviceIdType                  deviceId;           /* Device Id indication */

    VpDeviceStaticInfoType          staticInfo;         /**< Info that will not change during runtime. */
    VpDeviceDynamicInfoType         dynamicInfo;        /**< Info that will change during runtime */

    VpDeviceStateType               state;

    Vp890DeviceStateIntType         stateInt;

    VpOptionEventMaskType           deviceEventsMask;
    VpOptionEventMaskType           deviceEvents;
    uint16                          eventHandle;        /** Application defined event handle */
    uint16                          timeStamp;          /**< Used to track event timing. Increment by ticks */
    VpGetResultsOptionsType         getResultsOption;
    VpOptionCriticalFltType         criticalFault;

    VpRelGainResultsType            relGainResults;

    /* Used only to reduce MPI traffic (avoid read/modify/write */
    uint8                           swParamsCache[VP890_REGULATOR_PARAM_LEN];
    uint8                           devMode[VP890_DEV_MODE_LEN];

    uint16                          devTimer[VP_DEV_TIMER_LAST];

    Vp890DeviceProfileType          devProfileData;
    VpCSLACDeviceProfileTableType   devProfileTable;
    VpCSLACProfileTableEntryType    profEntry;

#ifdef VP890_INCLUDE_TESTLINE_CODE
    VpTestResultType                testResults;
#endif

    uint8                           intReg[VP890_UL_SIGREG_LEN];  /**< Holds signaling data info for the device */
    uint8                           intReg2[VP890_UL_SIGREG_LEN];

    uint8                           responseData;   /**< Holds data for Response events on the device */

    uint8                           txBufferDataRate;   /**< Buffer for MPI Low level reads to hold
                                                         * maximum amount of MPI data that is possible  */

    uint8                           mpiData[VP890_MAX_MPI_DATA];  /**< Buffer for MPI Low level reads to
                                                                   * hold maximum amount of MPI data that
                                                                   * is possible
                                                                   */
    uint8                           mpiLen;       /**< Length of data to be copied into mpiData buffer */

    /*  Two sets of dial pulse specifications are provide to support NTT dial
     * pulse detection windows of 10pps and 20pps while excluding 15pps */
    VpOptionPulseType pulseSpecs;
    VpOptionPulseType pulseSpecs2;

    uint8 testDataBuffer[VP890_TEST_DATA_LEN];

#if defined (VP890_INCLUDE_TESTLINE_CODE)
    Vp890CurrentTestType currentTest;
    Vp890CalOffCoeffs calOffsets[VP890_MAX_NUM_CHANNELS];
#endif /* VP890_INCLUDE_TESTLINE_CODE */

#if defined (VP_CC_890_SERIES) || defined (VP_CC_KWRAP)
    Vp890SysCalResultsType vp890SysCalData;
#endif

    /* For runtime enabling of debug output: */
    uint32 debugSelectMask;

    /* Used to hold the device level ecVal (to keep track of WB mode). */
    uint8 ecVal;

    /*
     * Holds the last channel information changed to/from Wideband mode since
     * the last tick. If = 0, no change has been made. If = 1, ch0 if = 2, ch1.
     * This is a handshake with the tick which detects it is not 0, makes the
     * appropriate change and sets it back to 0.
     */
    uint8 lastCodecChange;
} Vp890DeviceObjectType;

/* 890 specific Line Object */
typedef struct {
    uint8                           channelId;
    uint8                           ecVal;

    VpTermType                      termType;

    VpLineIdType                    lineId;             /* Application provided value for mapping a
                                                         * line to a line context */

    VpApiIntLineStateType           lineState;          /**< Line state info used for state
                                                         * transition handling and recall */
    Vp890LineStatusType             status;

    VpOptionCodecType               codec;

    /**< Used to delay slic write for LPM. */
    uint8                           nextSlicValue;

   /**< Used to reduce MPI reads. */
    uint8                           slicValueCache;
    uint8                           opCond[VP890_OP_COND_LEN];

    VpCslacTimerStruct              lineTimers;         /**< Timers for "this" line */
    VpOptionEventMaskType           lineEvents;
    VpOptionEventMaskType           lineEventsMask;
    uint16                          lineEventHandle;    /**< Line specific event handle information */
    uint16                          signaling1;
    uint16                          signaling2;

    uint16                          dtmfDigitSense;     /**< Used to hold the DTMF digit reported
                                                         * with VpDtmfDigitDetected() until the
                                                         * VP_LINE_EVID_DTMF_DIG is generated. */
    uint8                           signalingData;      /**< Holds data for Signaling events on this line */
    uint8                           fxoData;            /**< Holds data for FXO events on this line */
    uint8                           processData;        /**< Holds data for Process events on this line */
    uint16                          responseData;       /**< Holds data for Response events on this line */

    uint8                           fxoRingStateFlag;   /**< Indicates a state change request during ringing,
                                                             to be serviced when ringing is over */
    VpLineStateType                 fxoRingState;       /**< State to change to when ringing is over */

    uint8                           preDisconnect;      /**< The disconnect state prior to timer start */

    /*
     * Array to hold ringing parameters used in the Signal Generator.  This is
     * needed when signal generator A is set to a tone, then set back to ringing
     * without the user re-specifying the ringing profile
     */
    uint8                           ringingParams[VP890_SIGA_PARAMS_LEN];

    VpProfilePtrType                pRingingCadence;    /**< Currently used ringing cadence on
                                                         * this line */

    VpOptionRingControlType         ringCtrl;

    VpProfilePtrType                pCidProfileType1;   /**< Currently used caller ID profile
                                                         * on this line for sequenced cid */

    VpDialPulseDetectType           dpStruct;           /**< Used on FXS lines for detecting pulse
                                                         * digits*/

    VpDialPulseDetectType           dpStruct2;          /**< Used on FXS lines for detecting pulse
                                                         * digits using 2nd set of dp specs. */

    VpOptionPulseModeType           pulseMode;

    VpDigitGenerationDataType       digitGenStruct;     /**< Used on FXO lines for
                                                         * generating pulse digits */

    uint8                           ringDetMax;         /**< Stores the user specified maximum
                                                         * ringing detect period for FXO lines. This
                                                         * value may be outside the device range, in
                                                         * which case the SW will implement upper
                                                         * period detection */

    uint8                           ringDetMin;         /**< Stores the user specified maximum
                                                         * ringing detect period for FXO lines that
                                                         * is within the device range. This value is
                                                         * used as "minimum" that is detected by SW.
                                                         * Actual minimum period is supported by the
                                                         * device period detector itself. */

    uint8                           dPoh;               /**< Stores the user specified parallel
                                                         * offhook integration time for use
                                                         * in timing for the POH event */

    VpOptionPcmTxRxCntrlType        pcmTxRxCtrl;   /* Defines how the PCM highway is
                                                    * set for "talk" linestates */

    VpRelayControlType              relayState;    /* Used to hold current line
                                                    * relay state */

    /* Holds user definition for Loop Supervision configuration when
     * "badLoopSup" is TRUE */
    uint8                           loopSup[VP890_LOOP_SUP_LEN];

    uint16                          gxBase;       /**< Cached GX register, in 2.14 int format */
    uint16                          gxCidLevel;   /**< GX Caller ID correction factor (2.14) */
    uint16                          gxUserLevel;  /**< User specified relative GX level (2.14) */
    uint16                          grBase;       /**< Cached GR register, in 2.14 int format */
    uint16                          grUserLevel;  /**< User specified relative GR level (2.14) */

    int16 absGxGain; /**< Cached ABS A-to-D Gain, simplifies code. */
    int16 absGrGain; /**< Cached ABS D-to-A Gain, simplifies code. */

    uint8                           userDtg;             /* User-configured DTG setting */
    uint8                           cidDtg;              /* DTG register setting for CID
                                                          * correction */
    int8                            cidCorrectionSample; /* Last line voltage sample for CID
                                                          * correction */
    uint8                           cidCorrectionCtr;    /* Counter for CID correction line
                                                          * voltage samples */
    Vp890PllRecoveryStateType       pllRecoveryState;    /* State for the pll issue fix */
    uint8                           pllRecoverAttempts;    /* Number if pll recovery retries */

    Vp890CalLineData                calLineData;    /* Data used during VpCalLine() */

    Vp890FxoCurrentMonitorType      currentMonitor;
    Vp890FxoLowVoltageMonitorType   lowVoltageDetection;

#ifdef VP_CSLAC_SEQ_EN
    VpCallerIdType                  callerId; /**< Caller ID related information */
    VpSeqDataType                   cadence;  /**< Sequencer related information */
    VpCidSeqDataType                cidSeq;   /**< CID Sequencer related information */

    bool                            suspendCid;
    uint8                           cidParam[VP890_CID_PARAM_LEN];

    /**< Array to control internally run sequences */
    VpProfileDataType               intSequence[VP890_INT_SEQ_LEN];
#endif

    uint8 leakyLineCnt; /*
                         * Keeps track of # of times off-hook was detected (LP Mode)
                         * that was not further verified after exiting LP Mode. Reset
                         * when off-hook is verified.
                         */

    uint8 icr1Values[VP890_ICR1_LEN];   /**< Cached to minimize device access */
    uint8 icr2Values[VP890_ICR2_LEN];   /**< Cached to minimize device access */
    uint8 icr3Values[VP890_ICR3_LEN];   /**< Cached to minimize device access */
    uint8 icr4Values[VP890_ICR4_LEN];   /**< Cached to minimize device access */

    uint8 sigGenCtrl[VP890_GEN_CTRL_LEN];   /**< Cached to minimize device access */
    uint8 dcCalValues[VP890_DC_CAL_REG_LEN];

    /* Loop supervision parameters */
    uint8 hookHysteresis;

    bool internalTestTermApplied;

    /* For runtime enabling of debug output: */
    uint32 debugSelectMask;

} Vp890LineObjectType;


#endif /* VP890_API_H */

