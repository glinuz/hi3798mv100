/*****************************************************************************************
 * @file    MxL_HRCLS_DemodApi.h
 *
 * @brief   Header file for MxLWare Serdes APIs
 ****************************************************************************************/
#ifndef __MXL_HRCLS_DEMOD_API__
#define __MXL_HRCLS_DEMOD_API__
 
#include "MaxLinearDataTypes.h"

/*****************************************************************************************
    Macros
*****************************************************************************************/

/** @brief Equalizer FFE filter length */
#define MXL_HRCLS_FFE_INFO_LENGTH             16
/** @brief Equalizer Spur filter length */
#define MXL_HRCLS_SPUR_INFO_LENGTH            32
/** @brief Equalizer Dfe filter length */
#define MXL_HRCLS_DFE_INFO_LENGTH             72 

/** @brief Demod interrupt mask for FEC lost */
#define MXL_HRCLS_INTR_FEC_LOST_EN            0x0001
/** @brief Demod interrupt mask for FEC lock */
#define MXL_HRCLS_INTR_FEC_LOCK_EN            0x0002
/** @brief Demod interrupt mask for MPEG lost */
#define MXL_HRCLS_INTR_MPEG_LOST_EN           0x0004
/** @brief Demod interrupt mask for MPEG lock */
#define MXL_HRCLS_INTR_MPEG_LOCK_EN           0x0008
/** @brief Demod interrupt mask for QAM lock */
#define MXL_HRCLS_INTR_QAM_LOCK_GAINED_EN     0x0010
/** @brief Demod interrupt mask for QAM lost */
#define MXL_HRCLS_INTR_QAM_LOCK_LOST_EN       0x0020

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

/** @brief Demod channel identifiers */
typedef enum
{
  MXL_HRCLS_DEMOD0 = 0,
  MXL_HRCLS_DEMOD1 = 1,
  MXL_HRCLS_DEMOD2 = 2,
  MXL_HRCLS_DEMOD3 = 3,
  MXL_HRCLS_DEMOD4 = 4,
  MXL_HRCLS_DEMOD5 = 5,
  MXL_HRCLS_DEMOD6 = 6,
  MXL_HRCLS_DEMOD7 = 7,
  MXL_HRCLS_DEMOD8 = 8,
  MXL_HRCLS_MAX_NUM_DEMOD
} MXL_HRCLS_DMD_ID_E;

typedef enum
{
  MXL_HRCLS_XPT_MODE_NO_MUX_4,
  MXL_HRCLS_XPT_MODE_MUX_4,
  MXL_HRCLS_XPT_MODE_MUX_2,
  MXL_HRCLS_XPT_MODE_MUX_2_B,
  MXL_HRCLS_XPT_MODE_MUX_3,
  MXL_HRCLS_XPT_MODE_MUX_1,
  MXL_HRCLS_XPT_MODE_PARALLEL,
  MXL_HRCLS_XPT_MODE_CABLECARD,
  MXL_HRCLS_XPT_MODE_NO_MUX_2,

  MXL_HRCLS_XPT_MODE_MAX
} MXL_HRCLS_XPT_MODE_E;  

/** @brief OOB error counter struct */
typedef struct
{
  UINT32 cntCorrected;                      //!< Corrected packets
  UINT32 cntUncorrected;                    //!< Uncorrected packets
  UINT32 cntReceived;                       //!< Received packets
} MXL_HRCLS_OOB_ERRCNT_T;

/** @brief OOB Type SCTE 55-1 or 55-2*/
typedef enum
{
  OOB_SCTE_55_1_TYPE = 0,
  OOB_SCTE_55_2_TYPE
} MXL_HRCLS_OOB_TYPE_E;


/** @brief Output mode for OOB demodulator */
typedef enum
{
  OOB_CRX_DRX_MODE = 0,
  OOB_MPEG_TS_MODE
} MXL_HRCLS_OOB_OUT_MODE_E;

/** @brief OOB symbol rates */
typedef enum
{
  MXL_HRCLS_SYM_RATE_0_772MHz = 0,
  MXL_HRCLS_SYM_RATE_1_024MHz,
  MXL_HRCLS_SYM_RATE_1_544MHz
} MXL_HRCLS_OOB_SYM_RATE_E;

/** @brief MPEG data format */
typedef enum
{
  MXL_HRCLS_MPEG_SERIAL_LSB_1ST = 0,
  MXL_HRCLS_MPEG_SERIAL_MSB_1ST,

  MXL_HRCLS_MPEG_SYNC_WIDTH_BIT = 0,
  MXL_HRCLS_MPEG_SYNC_WIDTH_BYTE
} MXL_HRCLS_MPEG_DATA_FMT_E;

/** @brief MPEG clock format */
typedef enum
{
  MXL_HRCLS_MPEG_ACTIVE_LOW = 0,
  MXL_HRCLS_MPEG_ACTIVE_HIGH,

  MXL_HRCLS_MPEG_CLK_POSITIVE  = 0,
  MXL_HRCLS_MPEG_CLK_NEGATIVE,

  MXL_HRCLS_MPEG_CLK_IN_PHASE = 0,
  MXL_HRCLS_MPEG_CLK_INVERTED,
} MXL_HRCLS_MPEG_CLK_FMT_E;

/** @brief MPEG clock speeds */
typedef enum
{
  MXL_HRCLS_MPEG_CLK_10_55MHz  = 0,
  MXL_HRCLS_MPEG_CLK_21_09MHz,
  MXL_HRCLS_MPEG_CLK_25_96MHz,
  MXL_HRCLS_MPEG_CLK_28_16MHz,
  MXL_HRCLS_MPEG_CLK_42_18MHz,
  MXL_HRCLS_MPEG_CLK_56_21MHz,
  MXL_HRCLS_MPEG_CLK_84_37MHz,
  MXL_HRCLS_MPEG_CLK_112_50MHz
} MXL_HRCLS_MPEG_CLK_RATE_E;

/** @brief MPEG pad drive modes */
typedef enum
{
  MXL_HRCLS_MPEG_DRV_MODE_1X = 0,
  MXL_HRCLS_MPEG_DRV_MODE_2X,
  MXL_HRCLS_MPEG_DRV_MODE_3X,
  MXL_HRCLS_MPEG_DRV_MODE_4X,
  MXL_HRCLS_MPEG_DRV_MODE_5X,
  MXL_HRCLS_MPEG_DRV_MODE_6X,
  MXL_HRCLS_MPEG_DRV_MODE_7X,
  MXL_HRCLS_MPEG_DRV_MODE_8X,
} MXL_HRCLS_MPEG_DRV_MODE_E;

/** @brief MPEG pad drive parameters */
typedef struct
{
  MXL_HRCLS_MPEG_DRV_MODE_E padDrvMpegSyn;
  MXL_HRCLS_MPEG_DRV_MODE_E padDrvMpegDat;
  MXL_HRCLS_MPEG_DRV_MODE_E padDrvMpegVal;
} MXL_HRCLS_MPEG_PAD_DRV_T;

/** @brief Annex types */
typedef enum
{
  MXL_HRCLS_ANNEX_B = 0,
  MXL_HRCLS_ANNEX_A,
  MXL_HRCLS_ANNEX_OOB,
} MXL_HRCLS_ANNEX_TYPE_E;

/** @brief QAM types */
typedef enum
{
  MXL_HRCLS_QAM16 = 0,
  MXL_HRCLS_QAM64,
  MXL_HRCLS_QAM256,
  MXL_HRCLS_QAM1024,
  MXL_HRCLS_QAM32,
  MXL_HRCLS_QAM128,
  MXL_HRCLS_QPSK,
  MXL_HRCLS_QAM_AUTO
} MXL_HRCLS_QAM_TYPE_E;

typedef enum
{
  MXL_HRCLS_IQ_NORMAL = MXL_DISABLE,
  MXL_HRCLS_IQ_FLIPPED = MXL_ENABLE,
  MXL_HRCLS_IQ_AUTO
} MXL_HRCLS_IQ_FLIP_E;


/** @brief demod status counters */
typedef struct
{
  UINT32 CwCorrCount;                       //!< Corrected code words
  UINT32 CwErrCount;                        //!< Uncorrected code words
  UINT32 CwReceived;                        //!< Total received code words
  UINT32 CorrBits;                          //!< Counter for corrected bits
  UINT32 ErrMpeg;                           //!< Counter for erred MPEG frames
  UINT32 ReceivedMpeg;                      //!< Counter for received MPEG frames
} MXL_HRCLS_DMD_STAT_CNT_T;

/** @brief MPEGOUT parameter struct */
typedef struct
{
  MXL_BOOL_E                  enable;               //!< Enable or disable MPEG OUT on selected demod */
  MXL_HRCLS_MPEG_DATA_FMT_E lsbOrMsbFirst;        //!< In Serial mode, transmit MSB first or LSB
  MXL_HRCLS_MPEG_DATA_FMT_E mpegSyncPulseWidth;   //!< In serial mode, it can be configured with either 1 bit or 1 byte.
  MXL_HRCLS_MPEG_CLK_FMT_E  mpegValidPol;         //!< VALID polarity, active high or low
  MXL_HRCLS_MPEG_CLK_FMT_E  mpegSyncPol;          //!< SYNC byte(0x47) indicator, Active high or low
  MXL_BOOL_E                  mpegErrorIndication;  //!< If enabled, TEI bit shall be set when TS packet is broken
  MXL_BOOL_E                  mpeg3WireModeEnable;  //!< 0: disable 3 wire mode 1: enable 3 wire mode
  MXL_HRCLS_MPEG_PAD_DRV_T  mpegPadDrv;           //!< Configure MPEG output pad drive strength
  MXL_HRCLS_MPEG_CLK_FMT_E  mpeg3WireModeClkPol;  //!< Clock polarity in 3 wire mode
} MXL_HRCLS_MPEGOUT_PARAM_T;

typedef struct
{
  MXL_BOOL_E                enable;
  MXL_HRCLS_MPEG_DATA_FMT_E lsbOrMsbFirst;
  MXL_HRCLS_MPEG_DATA_FMT_E mpegSyncPulseWidth;
  MXL_HRCLS_MPEG_CLK_FMT_E  mpegValidPol;
  MXL_HRCLS_MPEG_CLK_FMT_E  mpegSyncPol;
  MXL_HRCLS_MPEG_CLK_FMT_E  mpegClkPol;
  MXL_HRCLS_MPEG_CLK_RATE_E clkFreq;
  MXL_HRCLS_MPEG_PAD_DRV_T  mpegPadDrv;
} MXL_HRCLS_XPT_MPEGOUT_PARAM_T;

typedef enum
{
  MXL_HRCLS_XPT_OUT_0 = 0,
  MXL_HRCLS_XPT_OUT_1,
  MXL_HRCLS_XPT_OUT_2,
  MXL_HRCLS_XPT_OUT_3,
  MXL_HRCLS_XPT_OUT_4,
  MXL_HRCLS_XPT_OUT_5,
  MXL_HRCLS_XPT_OUT_6,
  MXL_HRCLS_XPT_OUT_7,

  MXL_HRCLS_XPT_OUT_INVALID
} MXL_HRCLS_XPT_OUTPUT_ID_E;

typedef enum
{
  MXL_HRCLS_XPT_HDR_SIZE_0_BYTES,
  MXL_HRCLS_XPT_HDR_SIZE_4_BYTES,
  MXL_HRCLS_XPT_HDR_SIZE_8_BYTES,
  MXL_HRCLS_XPT_HDR_SIZE_12_BYTES
} MXL_HRCLS_XPT_HEADER_SIZE_E;  

typedef struct
{
  UINT16      pidValue;
  UINT16      pidRemapValue;
  MXL_BOOL_E  pidDrop;
} MXL_HRCLS_XPT_PID_ENTRY_T;

/** @brief PN23 mode */
typedef enum
{
  PN23_XNOR = 0,
  PN23_XOR,

  SYNC_MODE_ERROR = 0,
  SYNC_MODE_LOSS,
} MXL_HRCLS_PN23_MPSYNC_E;

/** @brief OOB data format */
typedef enum
{
  MXL_HRCLS_OOB_SYNC_WIDTH_BIT = 0,
  MXL_HRCLS_OOB_SYNC_WIDTH_BYTE
} MXL_HRCLS_OOB_DATA_FMT_E;

/** @brief OOB clock format */
typedef enum
{
  MXL_HRCLS_OOB_ACTIVE_LOW = 0,
  MXL_HRCLS_OOB_ACTIVE_HIGH,

  MXL_HRCLS_OOB_CLK_POSITIVE  = 0,
  MXL_HRCLS_OOB_CLK_NEGATIVE,

  MXL_HRCLS_OOB_CLK_IN_PHASE = 0,
  MXL_HRCLS_OOB_CLK_INVERTED,
} MXL_HRCLS_OOB_CLK_FMT_E;

/** @brief OOB output parameters */
typedef struct
{
  MXL_HRCLS_PN23_MPSYNC_E pn23SyncMode;     //!< IN
  MXL_HRCLS_PN23_MPSYNC_E pn23Feedback;     //!< IN
  MXL_HRCLS_OOB_DATA_FMT_E syncPulseWidth;  //!< IN
  MXL_BOOL_E oob3WireModeEnable;            //!< IN
  MXL_BOOL_E enablePn23Const;               //!< IN
  MXL_HRCLS_OOB_CLK_FMT_E validPol;        //!< IN
  MXL_HRCLS_OOB_CLK_FMT_E syncPol;         //!< IN
  MXL_HRCLS_OOB_CLK_FMT_E clkPol;          //!< IN
  MXL_HRCLS_OOB_OUT_MODE_E oobOutMode;      //!< IN
  MXL_BOOL_E enable;
} MXL_HRCLS_OOB_CFG_T;

/** @brief Equalizer filter parameters */
typedef struct
{
  SINT16 ffeInfo[MXL_HRCLS_FFE_INFO_LENGTH];      //!< FFE filter
  SINT16 spurInfo[MXL_HRCLS_SPUR_INFO_LENGTH];    //!< Spur filter
  SINT16 dfeInfo[MXL_HRCLS_DFE_INFO_LENGTH];      //!< DFE filter
  UINT8  dsEqMainLocation;                        //!< Location of leading tap (in FFE)
  UINT8  dsEqDfeTapNum;                           //!< Number of taps in DFE
} MXL_HRCLS_EQU_FILTER_INFO_T;

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  oob type - SCTE 55-1/55-2
 *
 *  @apibrief   \n Set the oob specification type to be used. Default is 55-1
 *
 *  @usage      
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodOOBType(
    UINT8     devId,                      
    MXL_HRCLS_OOB_TYPE_E oobType
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  symbRateInHz For user defined A/B symbol rate
 *  @param[in]  symbRate256InHz For user defined B symbol rate
 *
 *  @apibrief   \n Even though Auto-detect mode for QAM Type is available in hardware, the
 *                 symbol rate needs to be configured with a corresponding Annex Type.
 *              \n
 *              \n For Annex A, the same symbol rate will be used for all constellations.
 *              \n But for Annex B, the 2 symbol rates need to be provided for auto detection logic : 64QAM and 256 QAM.
 *              \n
 *              \n   - When ANNEX A is select
 *              \n     - Only SymbolRate needs to be provided by caller
 *              \n   - When ANNEX B is selected
 *              \n     - Both SymbolRate and SymbolRate256 should be provided by caller with a value in the following range: 0 <  Symbol rate < 7.125MHz
 *              [TBD] Above explanation modified from original at Page 26.
 *
 *  @usage      See example [TBD]
 *
 *  @equ261     MXL_DEMOD_SYMBOL_RATE_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodSymbolRate(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT32    symbRateInHz,
    UINT32    symbRate256InHz
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId 0-8
 *  @param[in]  oobSymbRate OOB symbol rate
 *
 *  @apibrief   \n For symbol type SYM_TYPE_OOB, one of the following predefined symbol rate(s) below shall be specified
 *              \n   - MXL_HRCLS_SYM_RATE_0_772MHz = 0,
 *              \n   - MXL_HRCLS_SYM_RATE_1_024MHz = 1
 *              \n   - MXL_HRCLS_SYM_RATE_1_544MHz = 2
 *              [TBD] Explanation correct?
 *
 *  @usage      [TBD]
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodSymbolRateOOB(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_OOB_SYM_RATE_E oobSymbRate
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  paramPtr OOB configuration structure
 *
 *  @apibrief   This functions sets output signal parameters for OOB TS
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodOutParamsOOB(UINT8 devId,
                    MXL_HRCLS_DMD_ID_E demodId,
                    MXL_HRCLS_OOB_CFG_T * paramPtr
                    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[out] ErrorsPtr Structure of errors statistics
 *
 *  @apibrief   Retrieves OOB errors statistics
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodErrorsOOB(UINT8 devId,
                    MXL_HRCLS_DMD_ID_E demodId,
                    MXL_HRCLS_OOB_ERRCNT_T * ErrorsPtr
                    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *
 *  @apibrief   Clear OOB errors statistics
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodErrorsOOBClear(UINT8 devId,
                    MXL_HRCLS_DMD_ID_E demodId
                    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  mpegOutParamPtr pointer to MPEG out parameters
 *
 *  @apibrief   This API configures mpeg output for specified demodulator.
 *
 *  @usage      [TBD Page 28]
 *
 *  @equ261     MXL_DEMOD_MPEG_OUT_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodMpegOutParams(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_MPEGOUT_PARAM_T* mpegOutParamPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  mpegClkPol MPEG data validity with clock, Rising or Falling edge
 *  @param[in]  mpegClkPadDrv MPEG clock pad drive mode
 *  @param[in]  mpegClkFreq MPEG clock frequency for internal clock source
 *
 *  @apibrief   Configure global MPEG data output clock polarity. This setting affects output clocks of all four demods.
 *
 *  @usage      This API should be called with MXL_DEMOD_MPEG_OUT_CFG.
 *
 *  @equ261     MXL_DEMOD_CLK_POLARITY_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(
    UINT8     devId,
    MXL_HRCLS_MPEG_CLK_FMT_E mpegClkPol,
    MXL_HRCLS_MPEG_DRV_MODE_E mpegClkPadDrv,
    MXL_HRCLS_MPEG_CLK_RATE_E mpegClkFreq
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  annexType Annex-B or A
 *  @param[in]  qamType manual mode, set QAM type directly
 *
 *  @apibrief   It configures Annex Type and Auto detect mode for QAM Type.
 *
 *  @usage      See example. [TBD Page 30]
 *
 *  @equ261     MXL_DEMOD_ANNEX_QAM_TYPE_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodAnnexQamType(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_ANNEX_TYPE_E annexType,
    MXL_HRCLS_QAM_TYPE_E qamType
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *
 *  @apibrief   This API clears error statistics data
 *
 *  @usage      This function should be used to clear demodulator error counter. Right
 *              after signal acquisition, the error counter may hold some garbage data
 *              that should not be considered valid. Call this function to clears the
 *              counter.
 *
 *  @equ261     MXL_DEMOD_RESET_STAT_COUNTER_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodErrorStatClear(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  adcIqFlip Enable or disable Flip the I/Q path after ADC
 *
 *  @apibrief   It enables or disables flip the I/Q path.
 *
 *  @usage      This API should be called for Annex-A in 25ms after channel tune. It is optional for Annex-B.
 *
 *  @equ261     MXL_DEMOD_ADC_IQ_FLIP_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodAdcIqFlip(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_IQ_FLIP_E adcIqFlip
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  equalizerSetting Enable or disable equalizer filter
 *
 *  @apibrief   It enables or disables equalizer filter
 *
 *  @usage      This API can be called to enable or disable equalizer
 *
 *  @equ261     MXL_DEMOD_EQUALIZER_FILTER_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodEqualizerFilter(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E  equalizerSetting
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  demodEnable Enable or disable demod hardware
 *
 *  @apibrief   This API enables or disables demod hardware
 *
 *  @usage      This API can be called to enable or disable demod hardware
 *
 *  @equ261     MXL_DEMOD_PSM_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodEnable(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E  demodEnable
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *
 *  @apibrief   This API restarts the specified demod.
 *
 *  @usage      This API should be called in case API MXL_DEMOD_ALL_LOCK_STATUS_REQ has returned RetuneRequired=MXL_BOOL_E_TRUE.
 *
 *  @equ261     MXL_DEMOD_RESTART_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodRestart(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *
 *  @apibrief   This API inverts the spectrum of the addressed demod.
 *
 *  @usage      This API should be called for auto spectrum inversion at the time of channel scan.
 *
 *  @equ261     MXL_DEMOD_INVERT_SPECTRUM_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodInvertSpectrum(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] annexTypePtr Provide a current Annex Type
 *  @param[out] qamTypePtr Provide a current QAM type
 *
 *  @apibrief   This API provides current settings of Annex and QAM Type.
 *
 *  @usage      After reset, this API can be used to retrieve Annex and QAM type.
 *
 *  @equ261     MXL_DEMOD_ANNEX_QAM_TYPE_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodAnnexQamType(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_ANNEX_TYPE_E* annexTypePtr,
    MXL_HRCLS_QAM_TYPE_E* qamTypePtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] intrStatusPtr Interrupt status
 *  @param[out] intrMaskPtr Interrupt mask
 *
 *  @apibrief   This API returns Demod interrupt status information.
 *
 *  @usage      After receiving interrupt, this API shall be used to figure out an interrupt source.
 *
 *  @equ261     MXL_DEMOD_INTR_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodIntrStatus(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT16*   intrStatusPtr,
    UINT16*   intrMaskPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] snrPtr SNR, fixed point format with 6 fractional bits
 *
 *  @apibrief   This API retrieves SNR of channel.
 *
 *  @usage      After tuning, this API can be called at any time to check channel status.
 *
 *  @equ261     MXL_DEMOD_SNR_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodSnr(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT16*   snrPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] statsPtr Statistics structure
 *
 *  @apibrief   This API retrieves error statistics data
 *
 *  @usage      \n After tuning, this API can be called at any time to check BER, Uncoded BER, and CER of a demod channel.
 *              \n However, this API must be called less than 1 second in order to update consecutive BER, UBER, and CER.
 *              [TBD]: Do we add more details on usage as in Page 38?
 *
 *  @equ261     MXL_DEMOD_BER_UNCODED_BER_CER_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodErrorStat(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_DMD_STAT_CNT_T * statsPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] carrierOffsetInHzPtr Carrier offset frequency in Hz
 *
 *  @apibrief   This API retrieves the carrier offset.
 *
 *  @usage      After channel tuning, it can be called at any time.
 *
 *  @equ261     MXL_DEMOD_CARRIER_OFFSET_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodCarrierOffset(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    SINT32*   carrierOffsetInHzPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *
 *  @apibrief   \n This API updates some demod settings - Eg. Burst noise improvements
 *
 *  @usage      \n After demod all lock, it can be called at any time.
 *
 *  @equ261     
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgUpdateDemodSettings(
    UINT8   devId,
    MXL_HRCLS_DMD_ID_E demodId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] equInfoPtr equalizer info
 *
 *  @apibrief   \n This API retrieves Demodulator equalizer taps information.
 *              \n Each filter tap is a complex number and an even address has a real part of a tap,
 *              \n an odd address has an imaginary part of a tap.
 *
 *  @usage      \n After channel tuning, it can be called at any time. The following equation shows an example to express Filter Tap information.
 *              \n
 *              \n Ex) FFE Tap = ParamPtr->FfeInfo[even number address] + j*ParamPtr->FfeInfo[odd number address];
 *              \n       Spur Tap = ParamPtr->SpurInfo[even number address] + j*ParamPtr->SpurInfo [odd number address];
 *              \n       Dfe Tap = ParamPtr->DfeInfo [even number address] + j*ParamPtr->DfeInfo [odd number address];
 *
 *  @equ261     MXL_DEMOD_EQUALIZER_FILTER_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodEqualizerFilter(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_HRCLS_EQU_FILTER_INFO_T* equInfoPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] interDepthIPtr Interleaver I
 *  @param[out] interDepthJPtr Interleaver J
 *
 *  @apibrief   This API retrieves Demodulator interleaver depth I/J information.
 *
 *  @usage      After channel tuning, it can be called at any time.
 *
 *  @equ261     MXL_DEMOD_INTERLEAVER_DEPTH_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodInterleaverDepth(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT8*    interDepthIPtr,
    UINT8*    interDepthJPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] timingOffsetPtr Timing offset (ppm unit)
 *
 *  @apibrief   This API retrieves Demodulator timing offset information.
 *
 *  @usage      After channel tuning, it can be called at any time.
 *
 *  @equ261     MXL_DEMOD_TIMING_OFFSET_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodTimingOffset(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    SINT16*   timingOffsetPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] demodEnablePtr MXL_ENABLE or MXL_DISABLE
 *
 *  @apibrief   This API returns the power state of specified demodulator and mpeg output.
 *
 *  @usage      This API can be called any time.
 *
 *  @equ261     MXL_DEMOD_PSM_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodEnable(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E* demodEnablePtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] qamLockStatusPtr  Demod Qam lock status,  MXL_LOCKED=lock, MXL_UNLOCKED=unlock
 *  @param[out] fecLockStatusPtr  Demod Fec lock status,  MXL_LOCKED=lock, MXL_UNLOCKED=unlock
 *  @param[out] mpegLockStatusPtr Demod Mpeg lock status, MXL_LOCKED=lock, MXL_UNLOCKED=unlock
 *  @param[out] retuneRequiredPtr Demod Retune Required, MXL_BOOL_E_TRUE=required, MXL_BOOL_E_FALSE=not required
 *
 *  @apibrief   \n It retrieves QAM, FEC and MPEG lock status of the addressed demod.
 *              \n It also indicates whether a retune is required.
 *
 *  @usage      \n After tuning the channel, Demod QAM/FEC/MPEG lock statuses can be retrieved by this API.
 *              \n In case RetuneRequired flag is set, the API - MXL_DEMOD_RESTART_CFG should be called.
 *              \n A second call to this API should be made to retrieve the QAM/FEC/MPEG lock statuses again.
 *
 *  @equ261     MXL_DEMOD_ALL_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodAllLockStatus(
    UINT8     devId,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E* qamLockStatusPtr,
    MXL_BOOL_E* fecLockStatusPtr,
    MXL_BOOL_E* mpegLockStatusPtr,
    MXL_BOOL_E* retuneRequiredPtr
    );


/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  enable  Enable/disable interrupt on PID
 *  @param[in]  pidArrayPtr List of PIDs which will generate interrupt. Can be NULL
 *              enable == MXL_FALSE
 *  @param[in]  pidCnt Number of PIDs in the pidArrayPtr array
 *
 *  @apibrief   This function configures interrupt on PID functionality 
 *
 *  @usage      Call this function anytime to configure interrupt on PID. Note, that
 *              only designated demodulator supports this functionality. If demodId
 *              is wrong, corresponding error code will be returned.   
 *
 *  @retval     MXL_SUCCESS if success
 *  @retval     MXL_FAILURE if I2C error
 *  @retval     MXL_NOT_INITIALIZED if the device was not initialized first
 *  @retval     MXL_INVALID_PARAMETER if one of the parameters is out of range
 *  @retval     MXL_NOT_SUPPORTED if demodId points to demodulator that does not support
                                  interrupt in PID
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodInterruptOnPid(
    UINT8               devId,
    MXL_HRCLS_DMD_ID_E  demodId,
    MXL_BOOL_E          enable,
    UINT16 *            pidArrayPtr,
    UINT8               pidCnt
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  bufferPtr buffer pointer to which the captured TS will be transferred. The
 *                        buffer has to be at least of 188*sizeof(UINT16) size 
 *
 *  @apibrief   This function copies the TS packet that triggered interrupt on PID
 *
 *  @usage      Call this function anytime after firmware is downloaded and interrupt on 
 *              PID functionality is configured. 
 *
 *  @retval     MXL_SUCCESS if success
 *  @retval     MXL_FAILURE if I2C error
 *  @retval     MXL_NOT_INITIALIZED if the device was not initialized first
 *  @retval     MXL_INVALID_PARAMETER if one of the parameters is out of range
 *  @retval     MXL_NOT_SUPPORTED if demodId points to demodulator that does not support
                                  interrupt in PID
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodCapturedTsPacket(
    UINT8                 devId,
    MXL_HRCLS_DMD_ID_E    demodId,
    UINT16 *              bufferPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  searchRangeIdx frequency search range index 
 *
 *  @apibrief   This function sets frequency search range for given demodulator 
 *
 *  @usage      Call this function anytime after firmware is downloaded
 *
 *  @retval     MXL_SUCCESS if success
 *  @retval     MXL_FAILURE if I2C error
 *  @retval     MXL_NOT_INITIALIZED if the device was not initialized first
 *  @retval     MXL_INVALID_PARAMETER if one of the parameters is out of range
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodFreqSearchRange(
    UINT8               devId,
    MXL_HRCLS_DMD_ID_E  demodId,
    UINT8               searchRangeIdx
    );

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgXptHeaderWords(UINT8 devId, 
    MXL_HRCLS_DMD_ID_E demodId, 
    MXL_HRCLS_XPT_HEADER_SIZE_E wordCnt, 
    UINT32 *wordPtr);

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgXptPidTable(
    UINT8   devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT8   pidCnt,
    MXL_HRCLS_XPT_PID_ENTRY_T* pidsPtr,
    MXL_BOOL_E defaultDrop);

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgXptOutput(
    UINT8   devId,
    MXL_HRCLS_XPT_OUTPUT_ID_E outputId,
    MXL_HRCLS_XPT_MPEGOUT_PARAM_T * outMpegParams);

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgXpt(
    UINT8   devId,
    MXL_HRCLS_XPT_MODE_E mode);

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodConstellationValue(
    UINT8	  devId,						
    MXL_HRCLS_DMD_ID_E demodId,
    SINT16 *iVal,
    SINT16 *qVal);

#endif

