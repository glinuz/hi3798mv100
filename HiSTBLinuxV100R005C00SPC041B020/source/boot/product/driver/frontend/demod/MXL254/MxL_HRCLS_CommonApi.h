/*****************************************************************************************
 * @file    MxL_HRCLS_CommonApi.h
 *
 * @brief   Header file for Hercules MxLWare APIs
 ****************************************************************************************/

#ifndef __MXL_HRCLS_COMMONAPI_H__
#define __MXL_HRCLS_COMMONAPI_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL_HRCLS_OEM_Defines.h"
#include "MxL_HRCLS_Features.h"

#ifdef _MXL_DLL_BUILD_
#undef MXL_HRCLS_API
#define MXL_HRCLS_API extern "C" __declspec(dllexport)
#endif // _MXL_DLL_BUILD_

#ifndef MXL_HRCLS_API
#define MXL_HRCLS_API
#endif

/*****************************************************************************************
    Macros
*****************************************************************************************/

/** @brief Max number of MxL HRCLS devices that can be managed by a MxLWare driver */
#define MXL_HRCLS_MAX_NUM_DEVICES             4

/** @brief MxL device version size in bytes */
#define MXL_HRCLS_VERSION_SIZE                5 //!< (A.B.C.D-RCx)

/** @brief Max length of I2C command */
#define MXL_HRCLS_MAX_COMMAND_DATA_SIZE       252

/** @brief Max number of DFE channels */
#define MXL_HRCLS_MAX_NUM_CHANNELS            25

/** @brief Output interface MPEG enable bit */
#define MXL_HRCLS_OUTPUT_ENABLE_MPEG          (1<<0)
/** @brief Output interface SERDES enable bit */
#define MXL_HRCLS_OUTPUT_ENABLE_SERDES        (1<<1)
/** @brief Output interface IF enable bit */
#define MXL_HRCLS_OUTPUT_ENABLE_IF            (1<<2)

#define MXL_HRCLS_OUTPUT_ENABLE_INTERRUPT_ON_PID  (1<<3)
#define MXL_HRCLS_OUTPUT_ENABLE_OOB               (1<<4)


#define MXL_HRCLS_INTR_MASK_PID                 (1 << 0)
#define MXL_HRCLS_INTR_MASK_CHAN_LOCKED         (1 << 1)
#define MXL_HRCLS_INTR_MASK_DMD0                (1 << 2)
#define MXL_HRCLS_INTR_MASK_DMD1                (1 << 3)
#define MXL_HRCLS_INTR_MASK_DMD2                (1 << 4)
#define MXL_HRCLS_INTR_MASK_DMD3                (1 << 5)
#define MXL_HRCLS_INTR_MASK_DMD4                (1 << 6)
#define MXL_HRCLS_INTR_MASK_DMD5                (1 << 7)
#define MXL_HRCLS_INTR_MASK_DMD6                (1 << 8)
#define MXL_HRCLS_INTR_MASK_DMD7                (1 << 9)
#define MXL_HRCLS_INTR_MASK_DMD8                (1 << 10)
#define MXL_HRCLS_INTR_MASK_FB_TUNER_LOCK       (1 << 11)
#define MXL_HRCLS_INTR_MASK_US_SERDES_UNLOCKED  (1 << 12)

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/


/** @brief Hercules device type */
typedef enum
{
  MXL_HRCLS_DEVICE_265,
  MXL_HRCLS_DEVICE_267,
  MXL_HRCLS_DEVICE_258,
  MXL_HRCLS_DEVICE_269,
  MXL_HRCLS_DEVICE_254,
  MXL_HRCLS_DEVICE_256,
  MXL_HRCLS_DEVICE_255,
  MXL_HRCLS_DEVICE_252,
  MXL_HRCLS_DEVICE_212,
  MXL_HRCLS_DEVICE_213,
  MXL_HRCLS_DEVICE_214
} MXL_HRCLS_DEVICE_E;


/** @brief DFE channel ID identifiers */
typedef enum
{
  MXL_HRCLS_CHAN0 = 0,  
  MXL_HRCLS_CHAN1 = 1,  
  MXL_HRCLS_CHAN2 = 2,  
  MXL_HRCLS_CHAN3 = 3,  
  MXL_HRCLS_CHAN4 = 4,  
  MXL_HRCLS_CHAN5 = 5,  
  MXL_HRCLS_CHAN6 = 6,  
  MXL_HRCLS_CHAN7 = 7,  
  MXL_HRCLS_CHAN8 = 8,  
  MXL_HRCLS_CHAN9 = 9,  
  MXL_HRCLS_CHAN10 = 10,  
  MXL_HRCLS_CHAN11 = 11,  
  MXL_HRCLS_CHAN12 = 12,  
  MXL_HRCLS_CHAN13 = 13,  
  MXL_HRCLS_CHAN14 = 14,  
  MXL_HRCLS_CHAN15 = 15,  
  MXL_HRCLS_CHAN16 = 16,  
  MXL_HRCLS_CHAN17 = 17,  
  MXL_HRCLS_CHAN18 = 18,  
  MXL_HRCLS_CHAN19 = 19,  
  MXL_HRCLS_CHAN20 = 20,  
  MXL_HRCLS_CHAN21 = 21,  
  MXL_HRCLS_CHAN22 = 22,  
  MXL_HRCLS_CHAN23 = 23,
  
  MXL_HRCLS_CHAN_NB = 24
} MXL_HRCLS_CHAN_ID_E;

/** @brief Spectrum API windowing schemes */
typedef enum
{
  MXL_HRCLS_FFT_WINDOW_NONE,
  MXL_HRCLS_FFT_WINDOW_TRIANGLE,
  MXL_HRCLS_FFT_WINDOW_RAISED_COS
} MXL_HRCLS_FFT_WINDOW_TYPE_E;

/** @brief Tuner identifiers */
typedef enum
{
  MXL_HRCLS_FULLBAND_TUNER   = 0,        //!< FullBand tuner
  MXL_HRCLS_NARROWBAND_TUNER = 1         //!< NarrowBand tuner
} MXL_HRCLS_TUNER_ID_E;

/** @brief IF channel identifiers */
typedef enum
{
  MXL_HRCLS_IF0 = 0,
  MXL_HRCLS_IF1 = 1,
  MXL_HRCLS_IF2 = 2,
  MXL_HRCLS_IF3 = 3,
  MXL_HRCLS_MAX_NUM_IF
} MXL_HRCLS_IF_ID_E;

/** @brief Power modes */
typedef enum
{
  MXL_HRCLS_POWER_MODE_SLEEP     = 0,       //!< Sleep mode: running under XTAL clock
  MXL_HRCLS_POWER_MODE_NORMAL    = 1        //!< Normal mode: running under nominal clock
} MXL_HRCLS_PWR_MODE_E;

typedef enum
{
  MXL_HRCLS_HERCULES_CHIP_ID = 0,
  MXL_HRCLS_MINOS_CHIP_ID
} MXL_HRCLS_CHIP_ID_E;

/** @brief Device version info struct */
typedef struct
{
  MXL_HRCLS_CHIP_ID_E chipId;                     //!< Device ID
  UINT8  chipVersion;                             //!< Device version
  UINT8  mxlWareVer[MXL_HRCLS_VERSION_SIZE];      //!< MxLWare version information
  UINT8  firmwareVer[MXL_HRCLS_VERSION_SIZE];     //!< Firmware version information
  UINT8  bootLoaderVer[MXL_HRCLS_VERSION_SIZE];   //!< Boot loader version information
  MXL_BOOL_E firmwareDownloaded;                  //!< Firmware is downloaded
} MXL_HRCLS_DEV_VER_T;

/** @brief Logger data struct */
typedef struct
{
  UINT8* outBufPtr;                         //!< Pointer to output buffer, allocated by caller
  UINT32 outBufLen;                         //!< Size of output buffer, specified by the caller
  UINT32 outPos;                            //!< Current write position
} MXL_HRCLS_LOG_T;

/** @brief Tuner lock status struct */
typedef struct
{
  MXL_BOOL_E tunerLock;                       //!< Status of Tuner Lock
  MXL_BOOL_E agcLockS;                        //!< Status of AGC Lock
  MXL_BOOL_E tunerDone;                       //!< Status of Tuner Done
} MXL_HRCLS_TUNER_LOCK_STATUS_T;

/** @brief Tuner config parameter struct */
typedef struct
{
  UINT8  bandwidthInMHz;                    //!< DFE channel bandwidth in MHz. 6 for 6MHz, 8 for 8MHz.
  UINT32 centerFrequencyInHz;               //!< DFE channel center frequency in Hz
} MXL_HRCLS_TUNER_PARAM_T;

/** @brief Tuner lock status */
typedef enum
{
  MXL_HRCLS_TUNER_DISABLED,                 //!< Tuner disabled
  MXL_HRCLS_TUNER_UNLOCKED,                 //!< Tuner enabled but not locked
  MXL_HRCLS_TUNER_LOCKED,                   //!< Tuner enabled and locked
} MXL_HRCLS_TUNER_STATUS_E;

/** @brief DFE Channel lock status */
typedef enum
{
  MXL_HRCLS_CHAN_DISABLED,                  //!< DFE channel disabled
  MXL_HRCLS_CHAN_UNLOCKED,                  //!< DFE channel enabled but not locked
  MXL_HRCLS_CHAN_LOCKED,                    //!< DFE channel enabled and locked
} MXL_HRCLS_CHAN_STATUS_E;

/** @brief Reported power value accuracy */
typedef enum
{
  MXL_HRCLS_PWR_INVALID,                    //!< Channel not tuned or not 
  MXL_HRCLS_PWR_ROUGH,                      //!< Readback from gain cal rssi and after channel settle - DVGA rssi before averaging.
  MXL_HRCLS_PWR_AVERAGED,                   //!< Averaged value. Most accurate. 
} MXL_HRCLS_RX_PWR_ACCURACY_E;

typedef struct
{
  UINT8         segmentIndex;
  UINT32        segmentSize;
  UINT32        segmentBytesSent;
} MXL_HRCLS_FIRMWARE_DOWNLOAD_STATS_T;  

typedef struct
{
  UINT16        currentFreqMHz;

  UINT16        currentCalPointIndex;
  UINT16        calPointsCount;
} MXL_HRCLS_DSCAL_STATS_T;

typedef enum
{
  MXL_HRCLS_CB_FW_DOWNLOAD_STATS = 0,
  MXL_HRCLS_CB_DS_CAL_STATS
} MXL_HRCLS_CB_TYPE_E;


/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  oemDataPtr Pointer to customer's specific data. This data will not be 
 *              processed by MxLWare and will be accessible by OEM functions.  
 *              This parameter is not required. Use NULL if not needed
 *  @param[in]  requiredDeviceType device type which should be supported by driver's 
 *              instance related with current devId.
 *
 *  @apibrief   This API initializes MxLWare driver
 *
 *  @usage      This is the first API called by host to initialize MxLWare driver. At this
 *              moment, customer's software may assign OEM specific data (oemDataPtr) 
 *              to devId. This pointer will be accessible by OEM functions.
 *              This function does not validate required device type. Required type will
 *              be stored in context and verified in MxLWare_HRCLS_API_CfgDevReset.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDrvInit(
                UINT8     devId,
    /*@null@*/  void *    oemDataPtr,
                MXL_HRCLS_DEVICE_E requiredDeviceType
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API resets MxL device
 *
 *  @usage      This API handler requires OEM function, MxLWare_HRCLS_OEM_Reset, which resets
 *              MxL_HRCLS through HW reset pin. In addition, all statistics variables will
 *              be also cleared. At this time, required device type declared in 
 *              MxLWare_HRCLS_API_CfgDrvInit will be verified with physical device. 
 *              If requiredDeviceType does not match the actual type read from MxL device, 
 *              error will be returned. 
 *              \n\n There is no soft reset function in MxL_HRCLS. After power on, HW
 *              reset is the only way to reset MxL_HRCLS. So it is required for OEM to
 *              implement Ctrl_ResetMxL_HRCLS function when using this API.
 *              \n\n Right after chip reset, MxL_HRCLS is running at XTAL clock (27MHz);
 *              and it is required to call MXL_DEV_XTAL_SETTINGS_CFG API to switch to PLL
 *              clock.
 *              \n\n CfgDevXtalSetting and CfgDevFirmwareDownload should be called after this API.
 *
 *  @equ261     MXL_DEV_DEVICE_RESET_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER, MXL_NOT_SUPPORTED
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevReset(
    UINT8     devId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  xtalCap Internal Capacitor value in parallel with XTAL
 *
 *  @apibrief   This API configures XTAL settings
 *
 *  @usage      This API needs to be called once the MxL device gets out of reset after
 *              power on. It is responsible for switching the device operation from Xtal
 *              clock (27MHz) to nominal CPU-clock (TBD) and must be called before the
 *              MxLWare_HRCLS_API_CfgDevFirmwareDownload API is called.
 *
 *  @equ261     MXL_DEV_XTAL_SETTINGS_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevXtalSetting(
   UINT8     devId,
   UINT8     xtalCap
   );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  enable MXL_ENABLE to enable clock out; MXL_DISABLE to disable
 *  @param[in]  amplitude Reference clock out amplitude
 *
 *  @apibrief   This API configures ref clock out amplitude and slew rate.
 *
 *  @usage      This API needs to be called after MxLWare_HRCLS_API_CfgDevXtalSetting
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevRefClockOut(
   UINT8     devId,
   MXL_BOOL_E  enable,
   UINT8     amplitude
   );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  mbinBufferSize  Firmware buffer size in unit of bytes
 *  @param[in]  mbinBufferPtr Pointer of the buffer that stores MBIN format firmware
 *  @param[in]  fwCallbackFn Pointer to callback function that will be periodically 
 *              called to report fw download progress or for other purposes defined
 *              by customer software
 *
 *  @apibrief   This API downloads firmware file that stored in \p mbinBufferPtr buffer
 *              to MxL_HRCLS through I2C interface. The firmware will run if it's
 *              successfully downloaded. If fwCallbackFn is defined (not NULL), it will
 *              be called after each segment is downloaded. It might be used for progress
 *              reporting.
 *
 *  @usage      This API should be executed after MxLWare_HRCLS_API_CfgDevXtalSetting API.
 *              After firmware is downloaded, it's required to wait at least 3ms before
 *              calling other APIs
 *
 *  @equ261     MXL_DEV_FIRMWARE_DOWNLOAD_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevFirmwareDownload(
                UINT8     devId,
                UINT32    mbinBufferSize,
                UINT8*    mbinBufferPtr,
    /*@null@*/  MXL_CALLBACK_FN_T fwCallbackFn
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  powerMode MxL_HRCLS power mode
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_POWER_MODE_SLEEP    = 0, // Sleep mode: running under XTAL clock
 *      MXL_HRCLS_POWER_MODE_NORMAL   = 1  // Normal mode: running under nominal clock
 *    } MXL_HRCLS_PWR_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API can be called to put MxL_HRCLS into sleep mode from normal mode,
 *              or wakeup to normal operating mode.
 *              \n - MXL_HRCLS_POWER_MODE_SLEEP: MxL_HRCLS runs under XTAL frequency (27MHz),
 *              and it consumes the lowest power;
 *              \n - MXL_HRCLS_POWER_MODE_NORMAL: MxL_HRCLS runs under PLL frequency
 *
 *  @usage      This API can be called to put MxL_HRCLS into sleep mode or wakeup to normal operating mode.
 *
 *  @equ261     MXL_DEV_POWER_MODE_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevPowerMode(
    UINT8     devId,
    MXL_HRCLS_PWR_MODE_E powerMode
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] devVerPtr pointer to returned version info struct
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT8  devId;                                 // Device ID
 *      UINT8  devVer;                                // Device version
 *      UINT8  mxlWareVer[MXL_HRCLS_VERSION_SIZE];    // MxLWare version information
 *      UINT8  firmwareVer[MXL_HRCLS_VERSION_SIZE];   // Firmware version information
 *      UINT8  bootLoaderVer[MXL_HRCLS_VERSION_SIZE]; // Boot loader version information
 *      MXL_BOOL_E firmwareDownloaded;                // Firmware is already downloaded
 *    } MXL_HRCLS_DEV_VER_T;
 *    @endcode
 *
 *  @apibrief   This API returns MxL device identification number and version information.
 *
 *  @usage      After device is connected (opened), it can be called at any time to retrieve information.
 *
 *  @equ261     MXL_DEV_VERSION_INFO_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDevVersionInfo(
              UINT8     devId,
    /*@out@*/ MXL_HRCLS_DEV_VER_T* devVerPtr 
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] powerModePtr pointer to returned power mode
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_POWER_MODE_SLEEP     = 0, // Sleep mode: running under XTAL clock
 *      MXL_HRCLS_POWER_MODE_NORMAL    = 1  // Normal mode: running under nominal clock
 *    } MXL_HRCLS_PWR_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API returns current power mode
 *
 *  @usage      This API can be called any time after device is connected (opened) to query
 *              its power mode.
 *
 *  @equ261     MXL_DEV_POWER_MODE_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDevPowerMode(
    UINT8     devId,
    MXL_HRCLS_PWR_MODE_E* powerModePtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  bandWidthInMhz DFE channel bandwidth for which coefficients are provided
 *                             (in MHz, 8 for 8Mhz, 6 for 6MHz)
 *  @param[in]  coeffsTabPtr Array of 9 16-bits coefficients
 *
 *  @apibrief   This API overwrites default CSF coefficients for given bandwidth
 *
 *  @usage      After firmware is downloaded, this API can be called to modify CSF coeffs 
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerCSFCoeffs(
    UINT8     devId,
    UINT8     bandWidthInMhz,
    SINT16 *  coeffsTabPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[in]  bandWidthInMhz DFE channel bandwidth in MHz, 8 for 8Mhz, 6 for 6MHz
 *  @param[in]  centerFrequencyInHz DFE channel center frequency in Hz
 *
 *  @apibrief   This API tunes a DFE channel to specified center frequency and channel bandwidth
 *
 *  @usage      After firmware is downloaded, this API can be called to config a DFE channel.
 *
 *  @equ261     MXL_TUNER_CHAN_TUNE_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerChanTune(
    UINT8     devId,
    MXL_HRCLS_CHAN_ID_E chanId,
    UINT8     bandWidthInMhz,
    UINT32    centerFrequencyInHz
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[in]  deltaFreqHz Delta frequency steps; step resolution is about 1.3KHz
 *
 *  @apibrief   This API fine tune a DFE channel with specified frequency delta
 *
 *  @usage      This API can be called after the DFE channel is tune through
 *              \a MxLWare_HRCLS_API_CfgTunerChanTune
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerChanFineRetune(
    UINT8     devId,
    MXL_HRCLS_CHAN_ID_E chanId,
    SINT16    deltaFreqHz
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  outInterfaceEnableMap Output interface enable map (bitmap of MPEG, SERDES, IF)
 *
 *  @reltype
 *    @code
 *    #define MXL_HRCLS_OUTPUT_ENABLE_MPEG        (1<<0)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_SERDES      (1<<1)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_IF          (1<<2)
 *    @endcode
 *
 *  @apibrief   This API is for configuring low power tuner output interface
 *
 *  @usage      This API can be called to enable one or multiple output interface
 *              for low power tuner (MPEG, SERDES, or IF)
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerNarrowBandOutInterface(
    UINT8     devId,
    UINT8     outInterfaceEnableMap
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *
 *  @apibrief   This API disables a DFE channel
 *
 *  @usage      This API can be used to disable a DFE channel
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerChannelDisable(
    UINT8               devId,
    MXL_HRCLS_CHAN_ID_E chanId
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  tunerId Tuner ID (0: fullband, 1: narrowband)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_FULLBAND_TUNER   = 0,  // FullBand tuner
 *      MXL_HRCLS_NARROWBAND_TUNER = 1   // NarrowBand tuner
 *    } MXL_HRCLS_TUNER_ID_E;
 *    @endcode
 *
 *  @apibrief   This API turns on specified tuner (fullband or narrowband tuner)
 *
 *  @usage      This API can be called any time to enabled a tuner
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerEnable(
    UINT8     devId,                       
    MXL_HRCLS_TUNER_ID_E tunerId          
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  tunerId Tuner ID (0: fullband, 1: narrowband)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_FULLBAND_TUNER   = 0,  // FullBand tuner
 *      MXL_HRCLS_NARROWBAND_TUNER = 1   // NarrowBand tuner
 *    } MXL_HRCLS_TUNER_ID_E;
 *    @endcode
 *
 *  @apibrief   This API turns off specified tuner (fullband or narrowband tuner). If fullband
 *              tuner is selected, all DFE channels attached to this tuner will be disabled as
 *              well. If narrowband tuner is selected, also its DFE channel will be disabled.
 *
 *  @usage      This API can be called any time after a tuner has been enabled
 *              The tuner can be re-enabled by calling MxLWare_HRCLS_API_CfgTunerEnable
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerDisable(
    UINT8     devId,                        
    MXL_HRCLS_TUNER_ID_E tunerId           
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  numRefFreq Number of reference frequencies
 *  @param[in]  refFreqInMHzPtr Pointer to reference frequency table. Provide 
 *                              frequencies in [MHz]
 *  @param[in]  refPwrIndBuVPtr Pointer to reference power level table. Provide power 
 *                              level in [dBuV x 10] (505 = 50.5 dBuV)
 *  @param[in]  callbackFn Pointer to callback function
 *
 *  @reltype
 *    @code
 *    typedef UINT32 (*MXL_CALLBACK_FN_T)(UINT8 devId, UINT32 callbackType, void * callbackPayload);
 *
 *    typedef struct
 *    {
 *      UINT16        currentFreqMHz;
 *
 *      UINT16        currentCalPointIndex;
 *      UINT16        calPointsCount;
 *    } MXL_HRCLS_DSCAL_STATS_T;
 *
 *    typedef enum
 *    {
 *      MXL_HRCLS_CB_FW_DOWNLOAD_STATS = 0,
 *      MXL_HRCLS_CB_DS_CAL_STATS
 *    } MXL_HRCLS_CB_TYPE_E;
 *
 *    @endcode
 *
 *  @apibrief   This API calibrates the MxL device for downstream power reporting.
 *
 *  @usage      This API can be called to calibrate MxL device for better accuracy in power
 *              reporting. The calibration result (correction coefficients) will be saved
 *              to non-volatile storage (NVRAM) through OEM function
 *              MxLWare_HRCLS_OEM_SaveNVRAMFile().
 *              If callbackFn pointer is provided, the registered function will be called
 *              periodically to update on current progress. 
 *              In callback function argument's list, callbackType will be set to 
 *              MXL_HRCLS_CB_DS_CAL_STATS, and callbackPayload will be of type
 *              MXL_HRCLS_DSCAL_STATS_T.
 *              The first frequency on the list determines the lowest frequency for each
 *              reported power will be calibrated. 
 *
 *  @equ261     MXL_TUNER_CREATE_RX_PWR_CAL_COEF_CFG
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerDsCal(
    UINT8     devId,
    UINT16    numRefFreq,
    UINT16*   refFreqInMHzPtr,
    UINT16*   refPwrIndBuVPtr,
    /*@null@*/  MXL_CALLBACK_FN_T callbackFn
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API loads calibration data from nvram file 
 *
 *  @usage      This API loads coefficients from a file stored in NVRAM. The coefficients
 *              were previously generated by MxLWare_HRCLS_API_CfgTunerDsCal. If this
 *              function fails, no coefficients are loaded and power reporting cannot
 *              be considered as reliable.
 *
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerDsCalDataLoad(UINT8 devId);

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  ifOutId IFOUT ID
 *  @param[in]  enable Enable or disable selected IF output
 *  @param[in]  ifFreqInHz IF Frequency in HZ (3500000 < ifFreqInHz < 44000000)
 *  @param[in]  signalLevel The desired output level for IF output in dBuV scale with 
 *              0.5dB resolution (signalLevel=200 -> 100dBuV)
 *
 *  @apibrief   This API configures IFOUT parameters for a specified IF channel
 *
 *  @usage      This API can be used to configure IFOUT parameters for a specified IF channel
 *              Range of ifFreqInHz:
 *                 - In 8 MHz Bandwidth mode, 4.5 MHz <= IF Frequency <= 44 MHz, in steps of 0.5MHz
 *                 - In 6 MHz Bandwidth mode, 3.5 MHz <= IF Frequency <= 44 MHz, in steps of 0.5MHz
 *
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerIfOutParams(
    UINT8     devId,
    MXL_HRCLS_IF_ID_E ifOutId,
    MXL_BOOL_E enable,
    UINT32    ifFreqInHz,
    UINT16    signalLevel
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] dsCalLogPtr pointer to returned downstream calibration log
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT8* outBufPtr;  // Pointer to output buffer, allocated by caller
 *      UINT32 outBufLen;  // Size of output buffer, specified by the caller
 *      UINT32 outPos;     // Current write position
 *    } MXL_HRCLS_LOG_T;
 *    @endcode
 *
 *  @apibrief   This API returns downstream calibration log
 *
 *  @usage      This API can be called right after MxLWare_HRCLS_API_CfgTunerDsCal to obtain
 *              downstream calibration log
 *
 *  @equ261     MXL_TUNER_DSCAL_INFO_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerDsCalInfo(
    UINT8     devId,
    MXL_HRCLS_LOG_T* dsCalLogPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  tunerId         Tuner ID
 *  @param[out] lockStatusPtr   Pointer to returned tuner status
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_FULLBAND_TUNER   = 0,  // FullBand tuner
 *      MXL_HRCLS_NARROWBAND_TUNER = 1   // NarrowBand tuner
 *    } MXL_HRCLS_TUNER_ID_E;
 *    @endcode
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_TUNER_DISABLED,           // Tuner disabled
 *      MXL_HRCLS_TUNER_UNLOCKED,           // Tuner enabled but not locked
 *      MXL_HRCLS_TUNER_LOCKED,             // Tuner enabled and locked
 *    } MXL_HRCLS_TUNER_STATUS_E;
 *    @endcode
 *
 *  @apibrief   This API returns lock status of a tuner (FB or NB)
 *
 *  @usage      This API can be called at any time to check if the specified tuner 
 *              is disabled, unlocked, or locked.
 *
 *  @equ261     MXL_TUNER_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerLockStatus(
    UINT8     devId,
    MXL_HRCLS_TUNER_ID_E tunerId,
    MXL_HRCLS_TUNER_STATUS_E* lockStatusPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[out] statusPtr Pointer to returned DFE channel status
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_CHAN_DISABLED,           // DFE channel disabled
 *      MXL_HRCLS_CHAN_UNLOCKED,           // DFE channel enabled but not locked
 *      MXL_HRCLS_CHAN_LOCKED,             // DFE channel enabled and locked
 *    } MXL_HRCLS_CHAN_STATUS_E;
 *    @endcode
 *
 *  @apibrief   This API returns status of a DFE channel
 *
 *  @usage      This API can be called at any time to check if the specified DFE channel 
 *              is disabled, unlocked, or locked.
 *
 *  @equ261     MXL_TUNER_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerChanStatus(
    UINT8     devId,
    MXL_HRCLS_CHAN_ID_E chanId,
    MXL_HRCLS_CHAN_STATUS_E* statusPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[out] rxPwrIndBuVPtr pointer to returned Rx Power in dBuV with 0.1dB resolution
 *  @param[out] accuracyPtr pointer to accuracy estimation of returned power level
 *
 *  @apibrief   This API returns instantaneous RX power of a DFE channel in dBuV*10 (505 = 50.5dBuV).
 *
 *  @usage      After channel is tuned, this API can be called at any time to get
 *              instantaneous RX power of a DFE channel
 *
 *  @equ261     MXL_TUNER_RF_RX_PWR_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerRxPwr(
    UINT8                   devId,
    MXL_HRCLS_CHAN_ID_E     chanId,
    UINT16*                 rxPwrIndBuVPtr,
    MXL_HRCLS_RX_PWR_ACCURACY_E * accuracyPtr
    );

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device ID
 *  @param[in]  Averaging window setting. Averaing window length has to be a power of 2
 *
 *  @apibrief   This API configures averaging window and windowing scheme for spectrum API samples.
 *
 *  @usage      This API can be called if averaging window needs to be modified.
 *              Once the averaging is configured, it remains unchanged for 
 *              all subsequent calls to MxLWare_HRCLS_API_ReqTunerPowerSpectrum.
 *              Default averaging setting after firmware download is 32.
 *              Valid averaging window lengths are [1,2,4,8,16,32,64,128]
 *              Valid windowing scheme settings are [0 = None, 1 = Triangle,2 = Raised cosine]
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerPowerSpectrum(
                         UINT8     devId,
                         UINT8     avgWindow,
                         MXL_HRCLS_FFT_WINDOW_TYPE_E windowType,
                         UINT16*   equivalentNoiseBwPtr);

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device ID
 *  @param[in]  freqStartInHz start frequency in Hz
 *  @param[in]  freqStepInHz frequency step in Hz
 *  @param[in]  numOfFreqSteps number of frequency steps
 *  @param[out] powerBufPtr output buffer with capacity >= numOfFreqSteps*sizeof(SINT16)
 *
 *  @apibrief   This API retrieves power spectrum in specified frequency range
 *
 *  @usage      This API can be used to retrieve power spectrum in specified frequency range
 *              The returned buffer contains power in units of dBuV with 2 decimal places.
 *              Range and accuracy will be further defined.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerPowerSpectrum(
                         UINT8     devId,
                         UINT32    freqStartInHz,
                         UINT32    freqStepInHz,
                         UINT32    numOfFreqSteps,
                         SINT16*   powerBufPtr);

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  cmdString null-terminated command String
 *  @param[out] cliLogPtr pointer to result string log
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT8* outBufPtr;  // Pointer to output buffer, allocated by caller
 *      UINT32 outBufLen;  // Size of output buffer, specified by the caller
 *      UINT32 outPos;     // Current write position (length of returned string)
 *    } MXL_HRCLS_LOG_T;
 *    @endcode
 *
 *  @apibrief   This API is for command line based debugging only
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDebugCli(
    UINT8     devId,
    SINT8*    cmdString,
    MXL_HRCLS_LOG_T* cliLogPtr
    );


/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  edgeTrigger MXL_TRUE if the device should generate edge on interrupt,
                MXL_FALSE if level trigger is required
 *  @param[in]  positiveTrigger MXL_TRUE for rising edge (in edge mode) or active state -
                high (in level mode)
 *  @param[in]  globalEnableInterrupt MXL_FALSE to disable interrupt functionality               
 *  @param[in]  intrMask Interrupt mask 
 *
 *  @reltype
 *    @code
 *    #define MXL_HRCLS_INTR_MASK_PID                 (1 << 0)
 *    #define MXL_HRCLS_INTR_MASK_CHAN_LOCKED         (1 << 1)
 *    #define MXL_HRCLS_INTR_MASK_DMD0                (1 << 2)
 *    #define MXL_HRCLS_INTR_MASK_DMD1                (1 << 3)
 *    #define MXL_HRCLS_INTR_MASK_DMD2                (1 << 4)
 *    #define MXL_HRCLS_INTR_MASK_DMD3                (1 << 5)
 *    #define MXL_HRCLS_INTR_MASK_DMD4                (1 << 6)
 *    #define MXL_HRCLS_INTR_MASK_DMD5                (1 << 7)
 *    #define MXL_HRCLS_INTR_MASK_DMD6                (1 << 8)
 *    #define MXL_HRCLS_INTR_MASK_DMD7                (1 << 9)
 *    #define MXL_HRCLS_INTR_MASK_DMD8                (1 << 10)
 *    #define MXL_HRCLS_INTR_MASK_FB_TUNER_LOCKED     (1 << 11)
 *    @endcode
 *
 *  @apibrief   This API configures sources of interrupt
 *
 *  @usage      Call this API whenever after firmware download 
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDevIntrMask(
                         UINT8      devId,
                         MXL_BOOL_E edgeTrigger,
                         MXL_BOOL_E positiveTrigger,
                         MXL_BOOL_E globalEnableInterrupt,
                         UINT32     intrMask);

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] intrStatusPtr Current interrupt status 
 *  @param[out] intrMaskPtr Current interrupt mask
 *
 *  @reltype
 *    @code
 *    #define MXL_HRCLS_INTR_MASK_PID                 (1 << 0)
 *    #define MXL_HRCLS_INTR_MASK_CHAN_LOCKED         (1 << 1)
 *    #define MXL_HRCLS_INTR_MASK_DMD0                (1 << 2)
 *    #define MXL_HRCLS_INTR_MASK_DMD1                (1 << 3)
 *    #define MXL_HRCLS_INTR_MASK_DMD2                (1 << 4)
 *    #define MXL_HRCLS_INTR_MASK_DMD3                (1 << 5)
 *    #define MXL_HRCLS_INTR_MASK_DMD4                (1 << 6)
 *    #define MXL_HRCLS_INTR_MASK_DMD5                (1 << 7)
 *    #define MXL_HRCLS_INTR_MASK_DMD6                (1 << 8)
 *    #define MXL_HRCLS_INTR_MASK_DMD7                (1 << 9)
 *    #define MXL_HRCLS_INTR_MASK_DMD8                (1 << 10)
 *    #define MXL_HRCLS_INTR_MASK_FB_TUNER_LOCKED     (1 << 11)
 *    @endcode
 *
 *  @apibrief   This API should be called after receiving interrupt signal from the chip
 *              in order to determine its source. 
 *
 *  @usage      Call this function after receiving interrupt signal or periodically to 
 *              check if any interrupt occured
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDevIntrStatus(
                         UINT8     devId,
                         UINT32 *  intrStatusPtr,
                         UINT32 *  intrMaskPtr);

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] temperaturePtr Current temperature in degrees.
 *
 *
 *  @apibrief   Used to readout temperature in degrees
 *             
 *
 *  @usage      Call this function anytime after firmware download.
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDevTemperature(
                         UINT8     devId,
                         SINT16*   temperaturePtr);

#ifdef _MXL_HRCLS_SERDES_ENABLED_
  #include "MxL_HRCLS_SerdesApi.h"
#endif  

#if defined(_MXL_HRCLS_DEMOD_ENABLED_) || defined(_MXL_HRCLS_OOB_ENABLED_)
  #include "MxL_HRCLS_DemodApi.h"
#endif

#endif /* __MXL_HRCLS_COMMONAPI_H__*/

