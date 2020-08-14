/*****************************************************************************************
 * @file    MxL_HRCLS_DemodApi.h
 *
 * @brief   Header file for MxLWare SerDes APIs
 ****************************************************************************************/
#ifndef __MXL_HRCLS_SERDES_API__
#define __MXL_HRCLS_SERDES_API__

#include "MaxLinearDataTypes.h"

/** @brief Max number of channels per DownStream SerDes lane */
#define MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5400 16
#define MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5184 12
#define MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_2700 8

/** @brief Max number of SerDes downstream lanes */
#define MXL_HRCLS_SERDES_DS_LANES_CNT 2

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

typedef enum
{
  MXL_HRCLS_SERDES_SLOT0 = 0,
  MXL_HRCLS_SERDES_SLOT1 = 1,
  MXL_HRCLS_SERDES_SLOT2 = 2,
  MXL_HRCLS_SERDES_SLOT3 = 3,
  MXL_HRCLS_SERDES_SLOT4 = 4,
  MXL_HRCLS_SERDES_SLOT5 = 5,
  MXL_HRCLS_SERDES_SLOT6 = 6,
  MXL_HRCLS_SERDES_SLOT7 = 7,
  MXL_HRCLS_SERDES_SLOT8 = 8,
  MXL_HRCLS_SERDES_SLOT9 = 9,
  MXL_HRCLS_SERDES_SLOT10 = 10,
  MXL_HRCLS_SERDES_SLOT11 = 11,
  MXL_HRCLS_SERDES_SLOT12 = 12,
  MXL_HRCLS_SERDES_SLOT13 = 13,
  MXL_HRCLS_SERDES_SLOT14 = 14,
  MXL_HRCLS_SERDES_SLOT15 = 15,
} MXL_HRCLS_SERDES_SLOT_ID_E;

/** @brief DownStream or UpStream SerDes lane ID */
typedef enum
{
  MXL_HRCLS_SERDES_LANE0 = 0,               //!< DownStream or UpStream SerDes lane 0
  MXL_HRCLS_SERDES_LANE1 = 1,               //!< DownStream or UpStream SerDes lane 1
} MXL_HRCLS_SERDES_LANE_ID_E;

/** @brief SERDES modes */
typedef enum
{
  MXL_HRCLS_SERDES_MODE_DISABLED         = 0, //!< Disabled (powered down)
  MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE = 1, //!< Enabled in full linerate (5400MHz or 5184MHz)
  MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE = 2, //!< Enabled in half linerate (2700MHz) 
} MXL_HRCLS_SERDES_MODE_E;

/** @brief SERDES PRBS modes */
typedef enum
{
  MXL_HRCLS_SERDES_PRBS7  = 0,              //!< PRBS7  sequence
  MXL_HRCLS_SERDES_PRBS15 = 1,              //!< PRBS15 sequence
  MXL_HRCLS_SERDES_PRBS23 = 2,              //!< PRBS23 sequence
  MXL_HRCLS_SERDES_PRBS31 = 3,              //!< PRBS31 sequence
  MXL_HRCLS_SERDES_PRBS22 = 4               //!< PRBS22 sequence
} MXL_HRCLS_SERDES_PRBS_MODE_E;

/** @brief SERDES coding modes */
typedef enum
{
  MXL_HRCLS_SERDES_CODING_CRC = 0,          //!< Cyclic redundancy check (CRC), for 5400MHz linerate only
  MXL_HRCLS_SERDES_CODING_RS  = 1,          //!< Reed-Solomon (RS) coding, for all linerates
} MXL_HRCLS_SERDES_CODING_E;

/** @brief UpStream SerDes error counter struct */
typedef struct
{
  UINT32 cntCorrected;                      //!< Corrected codewords (always 0 for CRC mode)
  UINT32 cntUncorrected;                    //!< Uncorrected codewords (simply error counter in CRC mode)
  UINT32 cntReceived;                       //!< Total received codewords
} MXL_HRCLS_SERDES_US_ERRCNT_T;

/** @brief SerDes linerate modes */
typedef enum
{
  MXL_HRCLS_SERDES_LINERATE_5400MHZ = 0,    //!< SerDes linerate 5400MHz (full:5400, half:2700)
  MXL_HRCLS_SERDES_LINERATE_5184MHZ = 1,    //!< SerDes linerate 5184MHz (full only)
} MXL_HRCLS_SERDES_LINERATE_E;

typedef enum
{
  MXL_HRCLS_TXDAC_DISABLE = MXL_DISABLE,
  MXL_HRCLS_TXDAC_ENABLE  = MXL_ENABLE,
  MXL_HRCLS_TXDAC_EXTERNAL_CONTROL
} MXL_HRCLS_TXDAC_STATE_E;

/** @brief TXDAC output swing setting */
typedef enum
{
  MXL_HRCLS_TXDAC_OUT_MIN,                  //!< Swing 369mVp-p
  MXL_HRCLS_TXDAC_OUT_DEFAULT,              //!< Swing 400mVp-p
  MXL_HRCLS_TXDAC_OUT_MAX,                  //!< Swing 433mVp-p
} MXL_HRCLS_TXDAC_OUT_SWING_E;

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesId SerDes lane ID (0 or 1)
 *  @param[in]  lineRate SerDes linerate (5400 or 5184MHz)
 *  @param[in]  mode SerDes mode (disable, half, full) for specified SerDes lane
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LINERATE_5400MHZ = 0, // SerDes linerate 5400MHz (full:5400, half:2700)
 *      MXL_HRCLS_SERDES_LINERATE_5184MHZ = 1, // SerDes linerate 5184MHz (full only)
 *    } MXL_HRCLS_SERDES_LINERATE_E;
 *    @endcode
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_MODE_DISABLED         = 0, // Disabled (powered down)
 *      MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE = 1, // Enabled in full linerate (5400MHz or 5184MHz)
 *      MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE = 2, // Enabled in half linerate (2700MHz)
 *    } MXL_HRCLS_SERDES_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API sets SerDes mode (disable, half, full) for specified SerDes lane
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesMode(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E  serDesId,
    MXL_HRCLS_SERDES_LINERATE_E lineRate,
    MXL_HRCLS_SERDES_MODE_E mode
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  dsCoding Coding scheme (RS or CRC)
 *  @param[in]  scramblerEnable Enable/disable scrambler (for 5184MHz linerate only)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_CODING_CRC = 0, // For 5400MHz (full:5400, half:2700) linerate only
 *      MXL_HRCLS_SERDES_CODING_RS = 1,  // For all linerates
 *    } MXL_HRCLS_SERDES_CODING_E;
 *    @endcode
 *
 *  @apibrief   This API sets DownStream SerDes parameters
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_HRCLS_SERDES_CODING_E dsCoding,
    MXL_BOOL_E scramblerEnable
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  enable Enable or disable DownStream PRBS
 *  @param[in]  dsPrbsMode PRBS polynomial 7/15/23/31
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_PRBS7  = 0, // PRBS7  sequence
 *      MXL_HRCLS_SERDES_PRBS15 = 1, // PRBS15 sequence
 *      MXL_HRCLS_SERDES_PRBS23 = 2, // PRBS23 sequence
 *      MXL_HRCLS_SERDES_PRBS31 = 3  // PRBS31 sequence
 *    } MXL_HRCLS_SERDES_PRBS_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API enables or disables PRBS on the selected DownStream SerDes interface
 *
 *  @usage      When enable == MXL_ENABLE:  set output mux to use PRBS and enable PRBS sequence
 *              When enable == MXL_DISABLE: set output mux to use normal (non-PRBS) and disable PRBS sequence
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPrbsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_BOOL_E  enable,
    MXL_HRCLS_SERDES_PRBS_MODE_E dsPrbsMode
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  preEmphasisSetting pre-emphasis setting
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *
 *  @apibrief   This API configures DownStream SerDes pre-emphasis
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPreEmphasis(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    UINT8     preEmphasisSetting
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  dsAmplitude Amplitude (TBD: unit to be determined after characterization)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *
 *  @apibrief   This API configures DownStream SerDes output swing
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsAmplitude(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    UINT8     dsAmplitude
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  serDesSlotId DownStream SerDes slot ID
 *  @param[in]  chanId DFE channel ID
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *
 *  @apibrief   This API configures DownStream SerDes XBAR
 *
 *  @usage      This API should be called to connect dfe channel (chanId) to selected SerDes
 *              slot (\a serDesSlotId) on specified DownStream SerDes (serDesDsId).
 *
 *              \n Available range of serDesSlotId
 *              - 0-7  for 2700MHz half linerate
 *              - 0-15 for 5400MHz full linerate
 *              - 0-11 for 5184MHz linerate
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsCrossBar(
    UINT8                       devId,
    MXL_HRCLS_SERDES_LANE_ID_E  serDesDsId,
    MXL_HRCLS_SERDES_SLOT_ID_E  serDesSlotId,
    MXL_HRCLS_CHAN_ID_E         chanId
    );


/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesDsId DownStream SerDes lane ID (0 or 1)
 *  @param[in]  phyInverted MXL_TRUE if inverted, MXL_FALSE if not
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_LANE0, // DownStream SerDes lane 0
 *      MXL_HRCLS_SERDES_LANE1, // DownStream SerDes lane 1
 *    } MXL_HRCLS_SERDES_LANE_ID_E;
 *    @endcode
 *
 *  @apibrief   This API configures DownStream SerDes polarity
 *
 *  @usage      This function is not mandatory. Call it before MxLWare_HRCLS_API_CfgSerDesMode
 *              to configure DS phy polarity.
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPhyPolarity(
    UINT8 devId, 
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId, 
    MXL_BOOL_E phyInverted
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  usCoding Coding scheme (RS or CRC)
 *  @param[in]  usRsCorrectEnable RS correction enable: for all linerates
 *  @param[in]  descramblerEnable Enable/disable descrambler (for 5184MHz linerate only)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_CODING_CRC = 0, // For 5400MHz (full:5400, half:2700) linerate only
 *      MXL_HRCLS_SERDES_CODING_RS = 1,  // For all linerates
 *    } MXL_HRCLS_SERDES_CODING_E;
 *    @endcode
 *
 *  @apibrief   This API sets UpStream SerDes parameters
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsParams(
    UINT8     devId,                                
    MXL_HRCLS_SERDES_CODING_E usCoding,
    MXL_BOOL_E  usRsCorrectEnable,
    MXL_BOOL_E  descramblerEnable
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  S1 sync_num (refer to usage, and API ReqSerDesUsSyncLockStatus)
 *  @param[in]  S2 sync_not_det_num (refer to usage, and API ReqSerDesUsSyncLockStatus)
 *  @param[in]  S3 synclost_range_num (refer to usage, and API ReqSerDesUsSyncLockStatus)
 *
 *  @apibrief   This API sets UpStream SerDes synchronization lock/lost detection
 *              parameters for both upstream channels
 *
 *  @usage      Frame synchronization lock is achieved when the K28 comma symbols, either
 *              data frames or filler frames, are found in their nominal position sync_num (S1)
 *              times sequentially.
 *
 *              The frame synchronization is considered lost when the frame synchronization
 *              symbol is not detected "sync_not_det_num" (S2) times in last "synclost_range_num"
 *              (S3) consecutive frames. Note that the window of "synclost_range_num" consecutive
 *              frames is not a moving window and starts at the 1st missing K28 comma symbol after
 *              frame synchronization is achieved.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncParams(
    UINT8     devId,
    UINT8     S1,
    UINT8     S2,
    UINT8     S3
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  equalizerSetting Equalizer setting for UpStream SerDes eye compliance
 *
 *  @apibrief   This API configures UpStream SerDes equalizer
 *
 *  @usage      TBD: units to be determined
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsEqualizer(
    UINT8     devId,
    UINT8     equalizerSetting
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  usPrbsCheckerEnable 0: normal mode; 1: PRBS checker enabled
 *  @param[in]  usPrbsMode PRBS mode 7/15/23/31; only applicable if PRBS checker is enabled
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_SERDES_PRBS7  = 0, // PRBS7  sequence
 *      MXL_HRCLS_SERDES_PRBS15 = 1, // PRBS15 sequence
 *      MXL_HRCLS_SERDES_PRBS23 = 2, // PRBS23 sequence
 *      MXL_HRCLS_SERDES_PRBS31 = 3  // PRBS31 sequence
 *    } MXL_HRCLS_SERDES_PRBS_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API configures UpStream SerDes PRBS settings.
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbs(
    UINT8     devId,
    MXL_BOOL_E  usPrbsCheckerEnable,
    MXL_HRCLS_SERDES_PRBS_MODE_E usPrbsMode
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API clears sync lost counter
 *
 *  @usage      Call to clear SERDES upstream's 'synchronization lost' counter. 
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncLostCntClear(
    UINT8   devId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API clears error counter
 *
 *  @usage      Call to clear SERDES upstream's error counter. 
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsErrCntClear(
    UINT8     devId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API clears PRBS error counter
 *
 *  @usage      Call to clear SERDES upstream's PRBS error counter.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbsErrCntClear(
    UINT8   devId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] errCntPtr pointer to returned UpStream SerDes error counter struct
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT32 cntCorrected;   // Corrected codewords (always 0 for CRC mode)
 *      UINT32 cntUncorrected; // Uncorrected codewords (simply error counter in CRC mode)
 *      UINT32 cntReceived;    // Total received codewords
 *    } MXL_HRCLS_SERDES_US_ERRCNT_T;
 *    @endcode
 *
 *  @apibrief   This API returns UpStream SerDes RS error counters
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsErrCnt(
    UINT8     devId,
    /*@out@*/ MXL_HRCLS_SERDES_US_ERRCNT_T* errCntPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] errCntPtr Pointer to returned PRBS error counters on specified UpStream SerDes
 *
 *  @apibrief   This API returns UpStream SerDes PRBS error counters
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsPrbsErrCnt(
    UINT8   devId,
    /*@out@*/ UINT32* errCntPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] syncLockStatusPtr Pointer to returned sync lock status
 *  @param[out] syncLostCntPtr Pointer to returned sync lost counter
 *
 *  @apibrief   This API returns UpStream SerDes lock status and sync lost count
 *
 *  @usage      Frame synchronization lock is achieved when the K28 comma symbols, either
 *              data frames or filler frames, are found in their nominal position sync_num (S1)
 *              times sequentially.
 *
 *              The frame synchronization is considered lost when the frame synchronization
 *              symbol is not detected "sync_not_det_num" (S2) times in last "synclost_range_num"
 *              (S3) consecutive frames. Note that the window of "synclost_range_num" consecutive
 *              frames is not a moving window and starts at the 1st missing K28 comma symbol after
 *              frame synchronization is achieved.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsSyncLockStatus(
    UINT8   devId,
    /*@out@*/ MXL_BOOL_E* syncLockStatusPtr,
    /*@out@*/ UINT8*    syncLostCntPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  phyInverted MXL_TRUE if US phy is inverted, MXL_FALSE if not 
 *
 *  @apibrief   This API configures up stream phy polarity
 *
 *  @usage      Call this function before MxLWare_HRCLS_API_CfgSerDesMode to modify
 *              default US phy polarity.
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPhyPolarity(
    UINT8 devId, 
    MXL_BOOL_E phyInverted
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  enable Enable or disable upstream hardware
 *  @param[in]  outputSwing Set output swing 
 *
 *  @apibrief   This API configures up stream
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevUpStreamParams(
    UINT8     devId,
    MXL_HRCLS_TXDAC_STATE_E  enable,
    MXL_HRCLS_TXDAC_OUT_SWING_E outputSwing
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesId Serdes lane ID that should be reset 
 *
 *  @apibrief   This API reset serdes PHY 
 *
 *  @usage      Should be called only if there is a problem with establish stable 
 *              connection between Hercules device and backend.
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesPhyReset(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serDesId);

#endif
