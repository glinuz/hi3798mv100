/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_CommonApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS driver command APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"
#include "MxL_HRCLS_CalCtrl.h"

/* MxLWare Driver version for MxL_HRCLS */
static const UINT8 MxLWare_HRCLS_Version[] = {4, 1, 5, 4, 2};

/* OEM Data pointer array */
void * MxL_HRCLS_OEM_DataPtr[MXL_HRCLS_MAX_NUM_DEVICES];

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  oemDataPtr Pointer to OEM data
 *
 *  @author     Mariusz Murawski
 *
 *  @apibrief   This API initializes the driver
 *
 *  @usage      This API must be called prior to any other API function. Cannot be 
 *              called more than once. In such case it will return
 *              MXL_ALREADY_INITIALIZED. 
 *
 *  @equ261     MXL_DEV_DEVICE_RESET_CFG
 *
 *  @return     MXL_SUCCESS, MXL_ALREADY_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDrvInit(
    UINT8 devId, 
    void* oemDataPtr,
    MXL_HRCLS_DEVICE_E requiredDeviceType)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("oemDataPtr=%p, RequiredDeviceType=%d\n", (oemDataPtr), requiredDeviceType););

  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized == 0)
    {
      // keep driverInitialized here to avoid race
      devContextPtr->driverInitialized = 1;
#ifdef _MXL_DIAG_ENABLED_
      MxL_HRCLS_OEM_DataPtr[devId & 0x03] = oemDataPtr;
#else
      MxL_HRCLS_OEM_DataPtr[devId] = oemDataPtr;
#endif            
      devContextPtr->devId = devId;
      devContextPtr->adcSampRateInHz = MXL_HRCLS_DEFAULT_REF_PLL_FREQ_HZ;
      devContextPtr->deviceType = requiredDeviceType;
      switch (requiredDeviceType)
      {
#ifdef MXL_HRCLS_265_ENABLE        
        case MXL_HRCLS_DEVICE_265:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL265 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_267_ENABLE        
        case MXL_HRCLS_DEVICE_267:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL267 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_258_ENABLE        
        case MXL_HRCLS_DEVICE_258:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL258 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_254_ENABLE        
        case MXL_HRCLS_DEVICE_254:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL254 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_255_ENABLE        
        case MXL_HRCLS_DEVICE_255:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL255 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_256_ENABLE        
        case MXL_HRCLS_DEVICE_256:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL256 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_252_ENABLE        
        case MXL_HRCLS_DEVICE_252:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL252 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_269_ENABLE        
        case MXL_HRCLS_DEVICE_269:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL269 driver initialization\n"););
            break; 
#endif            
#ifdef MXL_HRCLS_212_ENABLE
        case MXL_HRCLS_DEVICE_212:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL212 driver initialization\n"););
            break; 
#endif
#ifdef MXL_HRCLS_213_ENABLE
        case MXL_HRCLS_DEVICE_213:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL213 driver initialization\n"););
            break; 
#endif
#ifdef MXL_HRCLS_214_ENABLE
        case MXL_HRCLS_DEVICE_214:
            MXLDBG1(MxL_HRCLS_DEBUG("MXL214 driver initialization\n"););
            break; 
#endif
        default:
            status = MXL_NOT_SUPPORTED;
            break;
      }
      if (status != MXL_SUCCESS) devContextPtr->driverInitialized = 0; 
    } else status = MXL_ALREADY_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId,status);

  return status;
}

static UINT8 MxL_HRCLS_Ctrl_GetHermodVersionNumber(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  UINT8 versionNo = 0;
  MXL_STATUS_E status;
  UINT16 gpioVal = 0;

  status = MxL_HRCLS_Ctrl_ReadRegisterField(devContextPtr->devId, GPIO_DIN, &gpioVal);
  if (status == MXL_SUCCESS)
  {
    UINT8 gpio2, gpio3;
    gpio2 = (gpioVal & 0x04)?1:0;
    gpio3 = (gpioVal & 0x08)?1:0;
    versionNo = gpio2 ^ gpio3;
  }

  return versionNo;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *
 *  @apibrief   This API resets MxL device
 *
 *  @usage      This API handler requires OEM function, MxLWare_HRCLS_OEM_Reset, which resets
 *              MxL_HRCLS through HW reset pin. In addition, all statistics variables will
 *              be also cleared. And after reset, MXL_DEV_XTAL_SETTINGS_CFG API should be
 *              called followed by a Firmware downloaded from host to device.
 *              \n\n There is no soft reset function in MxL_HRCLS. After power on, HW
 *              reset is the only way to reset MxL_HRCLS. So it is required for OEM to
 *              implement Ctrl_ResetMxL_HRCLS function when using this API.
 *              \n\n Right after chip reset, MxL_HRCLS is running at XTAL clock (27MHz);
 *              and it is required to call MXL_DEV_XTAL_SETTINGS_CFG API to switch to PLL
 *              clock.
 *
 *  @equ261     MXL_DEV_DEVICE_RESET_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevReset(
    UINT8     devId                 
    )
{
  UINT8  status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
    // Reset MxL_HRCLS by toggling Reset pin
      status = MxLWare_HRCLS_OEM_Reset(devId);

      if (status == MXL_SUCCESS)
      {
#ifndef _MXL_DIAG_ENABLED_        
        UINT16 flavorId;

        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_SOC_FLVR, &flavorId);
        flavorId >>= 4;
        flavorId &= 0x0f;
        MXLDBG2(MxL_HRCLS_DEBUG("flavorId=%x\n", flavorId););
        switch (flavorId)
        {
          case MXL_FLVR_MXL252: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_252) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL254: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_254) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL255: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_255) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL256: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_256) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL258: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_258) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL265: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_265) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL267: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_267) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL269: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_269) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL212: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_212) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL213: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_213) status = MXL_NOT_SUPPORTED; break;
          case MXL_FLVR_MXL214: if (devContextPtr->deviceType !=MXL_HRCLS_DEVICE_214) status = MXL_NOT_SUPPORTED; break;
          default: status = MXL_FAILURE; break;
        }
#endif        
        if (status == MXL_SUCCESS)
        {
          UINT16 temp;
#ifdef _MXL_HRCLS_DEMOD_ENABLED_
          if (1)
          {
            MXL_HRCLS_DMD_ID_E demodId;
            for (demodId = MXL_HRCLS_DEMOD0; ((demodId < MXL_HRCLS_MAX_NUM_DEMOD) && (status == MXL_SUCCESS)); demodId = (MXL_HRCLS_DMD_ID_E)(demodId+1))
            {
              status |= MxL_HRCLS_Ctrl_CfgDemReset(devContextPtr, demodId);
            }
          }
#endif // _MXL_HRCLS_DEMOD_ENABLED_
          
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_SOC_ID, &temp);
          if ((temp == MXL_HRCLS_HERCULES_HW_ID) || (temp == MXL_HRCLS_MINOS_HW_ID))
          {
            devContextPtr->chipId = (temp == MXL_HRCLS_HERCULES_HW_ID)?MXL_HRCLS_HERCULES_CHIP_ID:MXL_HRCLS_MINOS_CHIP_ID;
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, CHIP_VERSION, &temp);
            devContextPtr->chipVersion = temp;
            if ((status == MXL_SUCCESS) && (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion == 2))
            {
              devContextPtr->chipVersion += MxL_HRCLS_Ctrl_GetHermodVersionNumber(devContextPtr);
            }
            
            MXLDBG1(MxL_HRCLS_DEBUG("chipId=%s chipVersion=%d\n", (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID)?"HERCULES":"MINOS", devContextPtr->chipVersion);)
#ifdef _MXL_HRCLS_SERDES_ENABLED_
            devContextPtr->serDesLineRate =  MXL_HRCLS_DEFAULT_SERDES_LINERATE;
            devContextPtr->serDesMode[(UINT8) MXL_HRCLS_SERDES_LANE0] = MXL_HRCLS_SERDES_MODE_DISABLED; 
            devContextPtr->serDesMode[(UINT8) MXL_HRCLS_SERDES_LANE1] = MXL_HRCLS_SERDES_MODE_DISABLED;
            devContextPtr->serDesSlot2Chan[(UINT8) MXL_HRCLS_SERDES_LANE0] = MXL_HRCLS_CHAN2;
            devContextPtr->serDesSlot2Chan[(UINT8) MXL_HRCLS_SERDES_LANE1] = MXL_HRCLS_CHAN2;
#endif // _MXL_HRCLS_SERDES_ENABLED_
            devContextPtr->nbTunerPowerState = MXL_OFF;
            devContextPtr->fbTunerPowerState = MXL_ON;
            devContextPtr->currentAfeTiltMode = MXL_HRCLS_AFE_FLAT_LNABO_0;
            status |= MxL_HRCLS_Ctrl_InitializeDeviceParameters(devContextPtr);
          }
          else
          {
            MXLERR(MxL_HRCLS_ERROR("[MXL][HRCLS][%d] Wrong ChipId=%d\n", devId, temp););
            status = MXL_FAILURE;
          }
        }
#ifndef _MXL_DIAG_ENABLED_        
        else
        {
          MXLERR(MxL_HRCLS_ERROR("[MXL][HRCLS][%d] Requested device does not match existing hardware device\n", devId);)
        }
#endif        
      }        
      else
      {
        MXLERR(MxL_HRCLS_ERROR("[MXL][HRCLS][%d] OEM reset returned FAIL. Chip is not reset\n", devId););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device Id
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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevXtalSetting(
   UINT8     devId,                      
   UINT8     xtalCap                    
   )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("xtalCap is ignored!\n"););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->chipVersion)
      {
        status = MxL_HRCLS_Ctrl_CfgDevXtalSetting(devContextPtr, xtalCap);
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

// [REVIEW] New API
/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  enable MXL_ENABLE to enable clock out; MXL_DISABLE to disable
 *  @param[in]  amplitude Ref clock out amplitude
 *
 *  @apibrief   This API configures ref clock out
 *
 *  @usage      This API needs to be called after MxLWare_HRCLS_API_CfgDevXtalSetting
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevRefClockOut(
   UINT8      devId,                       
   MXL_BOOL_E enable,                   
   UINT8      amplitude                 
   )
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXL_STATUS_E status;

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("enable=%d, amplitude=%d\n", (UINT8) enable, amplitude););
  if ((devContextPtr) && (amplitude < MXL_HRCLS_CLOCK_OUT_AMPLITUDE_MAX))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->clockOutSupported == MXL_TRUE)
      {
        status = MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_CLKOUT_ENA, (enable == MXL_ENABLE)?1:0); 
        if (status == MXL_SUCCESS)
        {
          status = MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_CLKOUT_AMP, (UINT16) (enable == MXL_ENABLE)?amplitude:0); 
          if (status == MXL_SUCCESS)
          {
            status = MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DIG_ANA_XTAL_UPDATE);  
          }
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  mbinBufferSize  Firmware buffer size in unit of bytes
 *  @param[in]  mbinBufferPtr Pointer of the buffer that stores MBIN format firmware
 *
 *  @apibrief   This API downloads firmware file that stored in \p mbinBufferPtr buffer
 *              to MxL_HRCLS through I2C interface. The firmware will run if it's
 *              successfully downloaded
 *
 *  @usage      This API should be executed after MxLWare_HRCLS_API_CfgDevXtalSetting API.
 *              After firmware is downloaded, it's required to wait at least 3ms before
 *              calling other APIs
 *
 *  @equ261     MXL_DEV_FIRMWARE_DOWNLOAD_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevFirmwareDownload(
    UINT8     devId,                 
    UINT32    mbinBufferSize,       
    UINT8*    mbinBufferPtr,
    MXL_CALLBACK_FN_T fwCallbackFn        
    )
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXL_STATUS_E status = MXL_SUCCESS;

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("mbinBufferSize=%u, callback %s\n", mbinBufferSize, (fwCallbackFn)?"registered":"not registered"););
  mbinBufferSize = mbinBufferSize; // anti-warning
  if ((devContextPtr) && (mbinBufferPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 verAB, verCD;

      if ((MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FIRMWARE_VER_AB, &verAB) == MXL_SUCCESS)
          && (MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FIRMWARE_VER_CD, &verCD) == MXL_SUCCESS))
      {
        if ((verAB) || (verCD))
        {
          status = MXL_NOT_SUPPORTED;
          MXLERR(MxL_HRCLS_ERROR("[HRCLS] Firmware was already downloaded\n"));
        }
      } else status = MXL_FAILURE;
      
      if (status == MXL_SUCCESS)
      {
        if ((MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_LAST_ERROR_CODE, 0x00) != MXL_SUCCESS)
            || (MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_LAST_ERROR_PARM_HI, 0x00) != MXL_SUCCESS)
            || (MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_LAST_ERROR_PARM_LO, 0x00) != MXL_SUCCESS))
        {
          status = MXL_FAILURE;
        }
      }
      if ((status == MXL_SUCCESS) && ((status = MxL_HRCLS_Ctrl_DownloadFirmwareInMbin(devContextPtr, (MBIN_FILE_T*)(mbinBufferPtr), MXL_ENABLE, fwCallbackFn)) == MXL_SUCCESS))
      {
        devContextPtr->firmwareStatus = FIRMWARE_STATUS_LOADED;
        MxLWare_HRCLS_OEM_DelayUsec(10*1000);
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}


/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  powerMode MxL_HRCLS power mode
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_POWER_MODE_SLEEP     = 0, // Sleep mode: running under xtal clock
 *      MXL_HRCLS_POWER_MODE_NORMAL    = 1  // Normal mode: running under nominal clock
 *    } MXL_HRCLS_PWR_MODE_E;
 *    @endcode
 *
 *  @apibrief   This API can be called to put MxL_HRCLS into sleep mode from normal mode
 *              \n - MXL_HRCLS_POWER_MODE_SLEEP: MxL_HRCLS runs under xtal frequency (27MHz), and it consumes the lowest power;
 *              \n - MXL_HRCLS_POWER_MODE_NORMAL: MxL_HRCLS runs under PLL frequency
 *
 *  @usage      This API should be called to put MxL_HRCLS into sleep mode. The following mode transitions are allowed:
 *              \n - When device is in SLEEP mode (for example, right after POR)
 *              \n * It enter NORMAL mode through MxLWare_HRCLS_API_CfgDevXtalSetting API
 *              \n - When devices is in NORMAL mode
 *              \n * It can enter SLEEP mode by calling this API with PowerMode= MXL_HRCLS_POWER_MODE_SLEEP
 *
 *  @equ261     MXL_DEV_POWER_MODE_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevPowerMode(
    UINT8     devId,                      
    MXL_HRCLS_PWR_MODE_E powerMode       
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("powerMode=%s\n", (powerMode == MXL_HRCLS_POWER_MODE_SLEEP)?"SLEEP":"WAKE UP"););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      HOST_COMMAND_T command;
      /* Form command payload */
      command.data[0] = (UINT8) ((powerMode == MXL_HRCLS_POWER_MODE_SLEEP)?0:1); 
      command.payloadLen = 1;
      command.payloadCheckSum = 0;
      command.syncChar = 0;
      command.commandId = 0;
      /* Send host command */
      status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_DEV_POWER_MODE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      if ((status == MXL_SUCCESS) && (powerMode == MXL_HRCLS_POWER_MODE_SLEEP))
      {
        devContextPtr->nbTunerPowerState = MXL_DISABLE;
        devContextPtr->fbTunerPowerState = MXL_DISABLE;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
 *    } MXL_HRCLS_DEV_VER_T;
 *    @endcode
 *
 *  @apibrief   This API returns MxL device identification number and version information.
 *
 *  @usage      After device is connected (opened), it can be called at any time to retrieve information.
 *
 *  @equ261     MXL_DEV_VERSION_INFO_REQ
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDevVersionInfo(
    UINT8     devId,                        /* IN : MxL device id*/
    MXL_HRCLS_DEV_VER_T* devVerPtr          /* OUT: device versions */
    )
{
  UINT32 k;
  UINT32 fwVer;
  UINT8 status = MXL_SUCCESS;
  UINT16 verAB = 0;
  UINT16 verCD = 0;
  UINT16 verRC = 0;

  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (devVerPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      devContextPtr->firmwareStatus = FIRMWARE_STATUS_UNKNOWN;

      devVerPtr->chipId  = devContextPtr->chipId;
      devVerPtr->chipVersion = (UINT8) devContextPtr->chipVersion;
      for (k = 0; k < MXL_HRCLS_VERSION_SIZE; ++k)
      {
        devVerPtr->mxlWareVer[k] = 0;
        devVerPtr->firmwareVer[k] = 0;
        devVerPtr->bootLoaderVer[k] = 0;
      }

      MXLDBG1(MxL_HRCLS_DEBUG("DeviceId=0x%x, Chip Id = 0x%d, Device Version = 0x%x\n", devId, devVerPtr->chipId, devVerPtr->chipVersion);)

      // Get MxLWare version information
      for (k = 0; k < MXL_HRCLS_VERSION_SIZE; k++)
        devVerPtr->mxlWareVer[k] = MxLWare_HRCLS_Version[k];

      MxL_HRCLS_DEBUG("MxLWare version: 0x%x.0x%x.0x%x.0x%x-RC%d\n", 
                     devVerPtr->mxlWareVer[0],
                     devVerPtr->mxlWareVer[1], 
                     devVerPtr->mxlWareVer[2],
                     devVerPtr->mxlWareVer[3],
                     devVerPtr->mxlWareVer[4]);
#ifdef _MXL_DIAG_ENABLED_
      if (0 != (Diag_flag & MXL_HRCLS_DIAG_FLAG_DISABLE_BOOTLOADER_CHECKING))
      {
      }
      else
#endif // _MXL_DIAG_ENABLED_
      {
        status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_BOOTLOADER_VER_CD, &verCD);
        if (verCD != 0)
        {
          status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_BOOTLOADER_VER_AB, &verAB);
          status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_BOOTLOADER_VER_RC, &verRC);

          devVerPtr->bootLoaderVer[0] = (verAB>>8);
          devVerPtr->bootLoaderVer[1] = (verAB&0xFF);
          devVerPtr->bootLoaderVer[2] = (verCD>>8);
          devVerPtr->bootLoaderVer[3] = (verCD&0xFF);
          devVerPtr->bootLoaderVer[4] = (UINT8)(verRC>>8);
  
          status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FIRMWARE_VER_AB, &verAB);
          status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FIRMWARE_VER_CD, &verCD);
          status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FIRMWARE_VER_RC, &verRC);

          devVerPtr->firmwareVer[0] = (verAB>>8);
          devVerPtr->firmwareVer[1] = (verAB&0xFF);
          devVerPtr->firmwareVer[2] = (verCD>>8);
          devVerPtr->firmwareVer[3] = (verCD&0xFF);
          devVerPtr->firmwareVer[4] = (UINT8)(verRC>>8);
        }
      }

      fwVer =   (((UINT32)(devVerPtr->firmwareVer[0])) << 0)
              + (((UINT32)(devVerPtr->firmwareVer[1])) << 8)
              + (((UINT32)(devVerPtr->firmwareVer[2])) << 16)
              + (((UINT32)(devVerPtr->firmwareVer[3])) << 24);
  
      devContextPtr->firmwareStatus = fwVer?FIRMWARE_STATUS_LOADED:FIRMWARE_STATUS_BOOTLOADER;  
      devVerPtr->firmwareDownloaded = (fwVer)?MXL_TRUE:MXL_FALSE;
  
      MXLDBG2(MxL_HRCLS_DEBUG("Firmware status=%d\n", devContextPtr->firmwareStatus););
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  
  return (MXL_STATUS_E) status;
}

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

MXL_STATUS_E MxLWare_HRCLS_API_ReqDevPowerMode(
    UINT8     devId,      
    MXL_HRCLS_PWR_MODE_E* powerModePtr   
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (powerModePtr))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 dataRb = 0;
      status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, HCLK_SEL, &dataRb);
      if (status == MXL_SUCCESS)
      {
        *powerModePtr = (dataRb == 1)?MXL_HRCLS_POWER_MODE_SLEEP:MXL_HRCLS_POWER_MODE_NORMAL;
        MXLDBG2(MxL_HRCLS_DEBUG("powerMode = %s\n", (*powerModePtr == MXL_HRCLS_POWER_MODE_SLEEP)?"SLEEP":"NORMAL"););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("bandwidth=%dMHz\n", bandWidthInMhz););
  if ((devContextPtr) && (coeffsTabPtr) && ((bandWidthInMhz == 6) || (bandWidthInMhz == 8)))
  {
    if (devContextPtr->driverInitialized)
    {
      HOST_COMMAND_T command;
      UINT8 i;
      /* Form command payload */
      command.data[0] = (UINT8) ((bandWidthInMhz == 6)?DFE_CHN_BW_6_MODE:DFE_CHN_BW_8_MODE); 
      for (i = 0; i < 9; i++)
      {
        command.data[1 + (2*i) + 0] = (UINT8) (coeffsTabPtr[i] & 0x00ff);
        command.data[1 + (2*i) + 1] = (UINT8) ((coeffsTabPtr[i] & 0xff00) >> 8);
      }
      command.payloadLen = 19;
      command.payloadCheckSum = 0;
      command.syncChar = 0;
      command.commandId = 0;
      /* Send host command */
      status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_DEV_CHAN_CSF_COEF, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
  
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[in]  bandWidthInMhz NB channel bandwidth in MHz, 8 for 8Mhz, 6 for 6MHz
 *  @param[in]  centerFrequencyInHz NB center frequency in Hz
 *
 *  @apibrief   This API tunes a DFE channel to specified center frequency and channel bandwidth
 *
 *  @usage      After firmware is downloaded, this API can be called to config a DFE channel.
 *
 *  @equ261     MXL_TUNER_CHAN_TUNE_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerChanTune(
    UINT8     devId,                         
    MXL_HRCLS_CHAN_ID_E chanId,             
    UINT8     bandWidthInMhz,              
    UINT32    centerFrequencyInHz         
    )
{
  /* Hack input parameters */
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("chanId=%u, bandWidth=%uMHz, frequency=%uHz\n", chanId, bandWidthInMhz, centerFrequencyInHz););
  if ((devContextPtr) && ((chanId < devContextPtr->dfeChanCount) || (chanId == MXL_HRCLS_CHAN_NB))
      && ((bandWidthInMhz == 6) || (bandWidthInMhz == 8)))
  {
    if (devContextPtr->driverInitialized)
    {
#ifndef _MXL_DIAG_ENABLED_      
      if (((centerFrequencyInHz >= MXL_HRCLS_WB_CHANNEL_FREQ_MIN_HZ) && (centerFrequencyInHz <= MXL_HRCLS_WB_CHANNEL_FREQ_MAX_HZ))
          || ((chanId == MXL_HRCLS_CHAN_NB) && (centerFrequencyInHz >= MXL_HRCLS_NB_CHANNEL_FREQ_MIN_HZ) && (centerFrequencyInHz <= MXL_HRCLS_NB_CHANNEL_FREQ_MAX_HZ)))
#else
      if (1)
#endif                
      {
        if (((chanId == MXL_HRCLS_CHAN_NB) && (devContextPtr->nbTunerPowerState == MXL_ON)) ||
             ((chanId != MXL_HRCLS_CHAN_NB) && (devContextPtr->fbTunerPowerState == MXL_ON)))
        {
          MXL_HRCLS_CHAN_ID_E phyChanId;

          phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);
          MXLDBG2(MxL_HRCLS_DEBUG("Logical channel%d mapped to physical channel%d\n", chanId, phyChanId););

          status = MxL_HRCLS_Ctrl_CfgTunerChanTune(devContextPtr, phyChanId, bandWidthInMhz, centerFrequencyInHz);
          if (status == MXL_SUCCESS) devContextPtr->dfeChannels[(UINT8) phyChanId].freqHz = centerFrequencyInHz;
        }
        else
        {
          status = MXL_NOT_READY;
          MXLERR(MxL_HRCLS_ERROR("%s tuner must be enabled first\n", (chanId == MXL_HRCLS_CHAN_NB)?"NB":"FB"););
        }
      } else status = MXL_INVALID_PARAMETER;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      MXL_HRCLS_CHAN_ID_E phyChanId;
      HOST_COMMAND_T command;

      phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);
      /* Form command payload */
      command.data[0] = (UINT8) phyChanId; 
      command.data[1] = (UINT8) 0; 
      command.data[2] = (UINT8) 0; 
      command.payloadLen = 3;
      command.payloadCheckSum = 0;
      command.syncChar = 0;
      command.commandId = 0;
      /* Send host command */
      status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_CHAN_FINE_RETUNE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      
      deltaFreqHz = deltaFreqHz;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  outInterfaceEnableMap Output interface enable map (bitmap of MPEG, SERDES, IF)
 *
 *  @reltype
 *    @code
 *    #define MXL_HRCLS_OUTPUT_ENABLE_MPEG              (1<<0)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_SERDES            (1<<1)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_IF                (1<<2)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_INTERRUPT_ON_PID  (1<<3)
 *    #define MXL_HRCLS_OUTPUT_ENABLE_OOB               (1<<4)
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
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("interfaceMap=0x%x\n", outInterfaceEnableMap););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      HOST_COMMAND_T command;
      /* Form command payload */
      command.data[0] = (UINT8) outInterfaceEnableMap; 
      command.payloadLen = 1;
      command.payloadCheckSum = 0;
      command.syncChar = 0;
      command.commandId = 0;
      /* Send host command */
      status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_NARROWBAND_OUT_INTERFACE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *
 *  @apibrief   This API disables a NB DFE channel
 *
 *  @usage      This API can be used to disable a DFE channel
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerChannelDisable(
    UINT8     devId,                 
    MXL_HRCLS_CHAN_ID_E  chanId     
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("chanId=%d\n", chanId););
  if ((devContextPtr) && ((chanId < devContextPtr->dfeChanCount) || (chanId == MXL_HRCLS_CHAN_NB)))
  {
    if (devContextPtr->driverInitialized)
    {
      MXL_HRCLS_CHAN_ID_E phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);
      {
        HOST_COMMAND_T command;
        /* Form command payload */
        command.data[0] = ((UINT8) phyChanId);
        command.data[1] = 0;
        command.data[2] = 0;
        command.data[3] = 0;
        command.data[4] = 0;
        command.payloadLen = 5;
        command.payloadCheckSum = 0;
        command.syncChar = 0;
        command.commandId = 0;
        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_CHAN_TUNE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

        if ((status == MXL_SUCCESS) && (chanId == MXL_HRCLS_CHAN_NB))
        {
          devContextPtr->nbTunerPowerState = MXL_OFF;
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  tunerId Tuner ID (0: fullband, 1: narrowband)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_FULLBAND_TUNER   = 0,  //!< FullBand tuner
 *      MXL_HRCLS_NARROWBAND_TUNER = 1   //!< NarrowBand tuner
 *    } MXL_HRCLS_TUNER_ID_E;
 *    @endcode
 *
 *  @apibrief   This API turns on specified tuner (fullband or narrowband tuner)
 *
 *  @usage      This API can be called any time to enabled a tuner
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE or MXL_NOT_SUPPORTED
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerEnable(
    UINT8     devId,                          
    MXL_HRCLS_TUNER_ID_E tunerId             
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (tunerId == MXL_HRCLS_NARROWBAND_TUNER)
      {
        if (devContextPtr->nbTunerPowerState != MXL_OFF) 
        {
          status = MXL_NOT_SUPPORTED;
          MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] NB tuner was already enabled\n"));
        }
      }
      else
      {
        if (devContextPtr->fbTunerPowerState != MXL_OFF) 
        {
          status = MXL_NOT_SUPPORTED;
          MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] FB tuner was already enabled\n"));
        }
      }

      if (status == MXL_SUCCESS)
      {
        HOST_COMMAND_T command;
        /* Form command payload */
        command.data[0] = (UINT8) (tunerId == MXL_HRCLS_FULLBAND_TUNER)?MXL_HRCLS_HOST_COMMAND_FB_TUNER_ID:MXL_HRCLS_HOST_COMMAND_NB_TUNER_ID;
        command.data[0] |= (UINT8) (1 << 1);  // enable
        command.payloadLen = 1;
        command.payloadCheckSum = 0;
        command.syncChar = 0;
        command.commandId = 0;
        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_ENABLE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

        if (status == MXL_SUCCESS)
        {
          if (tunerId == MXL_HRCLS_FULLBAND_TUNER) 
              devContextPtr->fbTunerPowerState = MXL_ON;
          else
              devContextPtr->nbTunerPowerState = MXL_ON;  
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  tunerId Tuner ID (0: fullband, 1: narrowband)
 *
 *  @reltype
 *    @code
 *    typedef enum
 *    {
 *      MXL_HRCLS_FULLBAND_TUNER   = 0,  //!< FullBand tuner
 *      MXL_HRCLS_NARROWBAND_TUNER = 1   //!< NarrowBand tuner
 *    } MXL_HRCLS_TUNER_ID_E;
 *    @endcode
 *
 *  @apibrief   This API turns off specified tuner (fullband or narrowband tuner)
 *
 *  @usage      This API can be called any time to disable a tuner
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerDisable(
    UINT8     devId,                        
    MXL_HRCLS_TUNER_ID_E tunerId           
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (tunerId == MXL_HRCLS_NARROWBAND_TUNER)
      {
        if (devContextPtr->nbTunerPowerState != MXL_ON) 
        {
          status = MXL_NOT_SUPPORTED;
          MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] NB tuner was already disabled\n"));
        }
      }
      else
      {
        if (devContextPtr->fbTunerPowerState != MXL_ON) 
        {
          status = MXL_NOT_SUPPORTED;
          MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] FB tuner was already disabled\n"));
        }
      }
      if (status == MXL_SUCCESS)
      {
        HOST_COMMAND_T command;
        /* Form command payload */
        command.data[0] = (UINT8) (tunerId == MXL_HRCLS_FULLBAND_TUNER)?MXL_HRCLS_HOST_COMMAND_FB_TUNER_ID:MXL_HRCLS_HOST_COMMAND_NB_TUNER_ID;
        command.data[0] |= (UINT8) (0 << 1);  // disable
        command.payloadLen = 1;
        command.payloadCheckSum = 0;
        command.syncChar = 0;
        command.commandId = 0;
        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_ENABLE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        if (status == MXL_SUCCESS)
        {
          if (tunerId == MXL_HRCLS_FULLBAND_TUNER)
            devContextPtr->fbTunerPowerState = MXL_OFF;
          else
            devContextPtr->nbTunerPowerState = MXL_OFF;
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

#ifdef _MXL_HRCLS_IFOUT_ENABLED_
/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  ifOutId IFOUT ID
 *  @param[in]  enable Enable or disable selected IF output
 *  @param[in]  ifFreqInHz IF Frequency in HZ (3500000 <= ifFreqInHz <= 44000000)
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

MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerIfOutParams(
    UINT8     devId,
    MXL_HRCLS_IF_ID_E ifOutId,
    MXL_BOOL_E  enable,
    UINT32    ifFreqInHz,
    UINT16    signalLevel
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("enable=%d, freq=%dHz, signalLevel=%d(%d.%ddBuV)\n", (enable == MXL_TRUE)?1:0, ifFreqInHz, signalLevel, signalLevel / 2, signalLevel % 2););
  if ((devContextPtr) && 
      (((ifFreqInHz >= MXL_HRCLS_IFOUT_FREQ_MIN_HZ) && (ifFreqInHz <= MXL_HRCLS_IFOUT_FREQ_MAX_HZ) 
        && (signalLevel >= MXL_HRCLS_IFOUT_LEVEL_MIN) && (signalLevel <= MXL_HRCLS_IFOUT_LEVEL_MAX)) || (enable == MXL_DISABLE))
      )
  {
    if (devContextPtr->driverInitialized)
    {
      if ((devContextPtr->ifOutSupported == MXL_TRUE) && ((UINT8) ifOutId < devContextPtr->ifOutCnt))
      {
        MXL_HRCLS_IF_ID_E phyIfOutId;
        HOST_COMMAND_T command;

        phyIfOutId = MxL_HRCLS_Ctrl_GetPhysicalIfOutId(devContextPtr, ifOutId);

        /* Form command payload */
        command.data[0] = (UINT8) (phyIfOutId == MXL_HRCLS_IF0)?0:(phyIfOutId == MXL_HRCLS_IF1)?1:(phyIfOutId == MXL_HRCLS_IF2)?2:3;
        command.data[0] |= (UINT8) ((enable == MXL_ENABLE)?0x04:0x00);
        command.data[0] |= (UINT8) ((signalLevel & 0x1f) << 3);
        command.data[1] = (UINT8) ((signalLevel & 0x1fe0) >> 5);
        command.data[2] = (UINT8) (ifFreqInHz & 0x000000ff);
        command.data[3] = (UINT8) ((ifFreqInHz & 0x0000ff00) >> 8);
        command.data[4] = (UINT8) ((ifFreqInHz & 0x00ff0000) >> 16);
        command.data[5] = (UINT8) ((ifFreqInHz & 0xff000000) >> 24);

        command.payloadLen = 6;
        command.payloadCheckSum = 0;
        command.syncChar = 0;
        command.commandId = 0;
        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_TUNER_IFOUT_PARAMS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}
#endif // _MXL_HRCLS_IFOUT_ENABLED_

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

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerDsCalDataLoad(UINT8 devId)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      status = MxL_HRCLS_Ctrl_CalReadCoeffsFromFile(devContextPtr);
      MXLERR(
      if (status != MXL_SUCCESS)
      {
        MxL_HRCLS_ERROR("[HRCLS] Cannot open or read calibration file\n");
      }
      )
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  numRefFreq Number of reference frequencies
 *  @param[in]  refFreqInHzPtr Pointer to reference frequency table
 *  @param[in]  refPwrIndBuVPtr Pointer to reference power level table
 *
 *  @apibrief   This API calibrates the MxL device for downstream power reporting.
 *
 *  @usage      This API can be called to calibrate MxL device for better accuracy in power
 *              reporting. The calibration result (correction coefficients) will be saved
 *              to non-volatile storage (NVRAM) through OEM function
 *              MxLWare_HRCLS_OEM_SaveNVRAMFile()
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
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("numRefFreq=%d\n", numRefFreq););
  if ((devContextPtr) && (numRefFreq) && (refFreqInMHzPtr) && (refPwrIndBuVPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      MXL_HRCLS_CAL_DATA_T calData;

      UINT16 feature;
      if ((MxLWare_HRCLS_OEM_ReadRegister(devId, 0xF098, &feature) == MXL_SUCCESS)
          && (MxLWare_HRCLS_OEM_WriteRegister(devId, 0xF098, feature | 0x800) == MXL_SUCCESS))
      {
        calData.callbackFnPtr = callbackFn;
        status = MxL_HRCLS_Ctrl_CalUpdateFreqPoints(devContextPtr, refFreqInMHzPtr[0], &calData);
        if (status == MXL_SUCCESS)
        {			 
          status = MxL_HRCLS_Ctrl_CalUpdateRefPowerAtFreqPoints(devContextPtr, numRefFreq, refFreqInMHzPtr, refPwrIndBuVPtr, &calData); 

          if (status == MXL_SUCCESS)
          {
            UINT8 tiltIndex;

            for (tiltIndex = (UINT8) MXL_HRCLS_AFE_FLAT_LNABO_NON0; (tiltIndex <= (UINT8) MXL_HRCLS_AFE_TILT4) && (status == MXL_SUCCESS); tiltIndex++)
            {
#ifdef MXL_HRCLS_DS_CAL_TEST            
              SINT16 ave_error = 0;
              SINT16 max_error = 0;
#endif            
              // set tilt mode
              status = MxL_HRCLS_Ctrl_SelectAFETiltMode(devContextPtr, 
                                  (MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E) tiltIndex, 
                                  MXL_HRCLS_AFE_MODE_CHANGE_TIMEOUT_MS); //TODO: change timeout value
              calData.currentTiltIndex = (tiltIndex - MXL_HRCLS_AFE_FLAT_LNABO_0);                                
              if (status == MXL_SUCCESS)
              {
                devContextPtr->currentAfeBO = MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(devContextPtr);
                status = MxL_HRCLS_Ctrl_CalReadReferencePower(devContextPtr, &calData);  
                if (status == MXL_SUCCESS)
                {
                  UINT8 segmentId;

                  for (segmentId = 0; segmentId < MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT; segmentId++)
                  {
                    MXL_HRCLS_CAL_COEFF_T coeff;
#ifdef MXL_HRCLS_DS_CAL_TEST                  
                    MXL_HRCLS_CAL_COEFF_T coeff_float;
                    MXL_HRCLS_CAL_COEFF_T coeff_delta;
                    SINT16 max_delta_x, max_delta_y;

                    status = MxL_HRCLS_Ctrl_CalProduceCoeffsFloat(segmentId, &calData, &coeff_float);
                    MXLDBG1(MxL_HRCLS_DEBUG("COEFFS[%d;%d] c2=%d, c1=%d, c0=%d\n", tiltIndex, segmentId, coeff_float.c2, coeff_float.c1, coeff_float.c0););
#endif                  
                    status = MxL_HRCLS_Ctrl_CalProduceCoeffs(segmentId, &calData, &coeff);
                    MXLDBG1(MxL_HRCLS_DEBUG("COEFFS[%d;%d] c2=%d, c1=%d, c0=%d\n", tiltIndex, segmentId, coeff.c2, coeff.c1, coeff.c0););
#ifdef MXL_HRCLS_DS_CAL_TEST                  
                    coeff_delta.c2 = coeff.c2 - coeff_float.c2;                  
                    coeff_delta.c1 = coeff.c1 - coeff_float.c1;                  
                    coeff_delta.c0 = coeff.c0 - coeff_float.c0;                  
                    
                    max_delta_x = (coeff_delta.c2)?(MXL_DIV_ROUND(((-coeff_delta.c1) * 100), (2 * coeff_delta.c2))):0;
                    max_delta_y = MXL_DIV_ROUND((coeff_delta.c2 * max_delta_x * max_delta_x), 100) + (coeff_delta.c1 * max_delta_x) + coeff_delta.c0 * 100;
                    max_delta_y = MXL_ABS(MXL_DIV_ROUND(max_delta_y, 10));
                    ave_error += max_delta_y;
                    if (max_delta_y > max_error) max_error = max_delta_y;
                    MxL_HRCLS_DEBUG("max_delta_x=%d, max_delta_y=%d\n", max_delta_x, max_delta_y);
#endif                  
                    
                    if (status == MXL_SUCCESS)
                    {
                      memcpy(&devContextPtr->pwrReportData.pwrCorrCoeffs[tiltIndex-MXL_HRCLS_AFE_FLAT_LNABO_0][segmentId], &coeff, sizeof(MXL_HRCLS_CAL_COEFF_T));
                      if (tiltIndex == MXL_HRCLS_AFE_FLAT_LNABO_NON0)
                      {
                        MXLDBG2(MxL_HRCLS_DEBUG("Copying coeficients to LNA flat B/O zero profile\n"););
                        memcpy(&devContextPtr->pwrReportData.pwrCorrCoeffs[(UINT8) (MXL_HRCLS_AFE_FLAT_LNABO_0-MXL_HRCLS_AFE_FLAT_LNABO_0)][segmentId], &coeff, sizeof(MXL_HRCLS_CAL_COEFF_T));
                      }
                    }
                    MXLERR(
                    else
                    {
                      MxL_HRCLS_ERROR("[HRCLS] %s Producing compensation coeffs failed\n", __FUNCTION__);
                    }
                    )
                  }
                }
                else
                {
                  break;
                }
              }
              MXLERR(
              else
              {
                MxL_HRCLS_ERROR("[HRCLS] %s Cannot change tilt mode (status=%d)\n", __FUNCTION__, status);
              }
              )
#ifdef MXL_HRCLS_DS_CAL_TEST            
              MxL_HRCLS_DEBUG("tilt average error: %d (max: %d)\n", MXL_DIV_ROUND(ave_error, MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT), max_error);
#endif            
            }
            if (status == MXL_SUCCESS)
            {
              devContextPtr->pwrReportData.firstFreqMHz = calData.freqPoints[0].freqMHz;
              status = MxL_HRCLS_Ctrl_CalSaveCoeffsToFile(devContextPtr);
              if (status != MXL_SUCCESS)
              {
                MXLERR(MxL_HRCLS_ERROR("[HRCLS] Cannot save calibration data to file\n"););
              }
              else
              {
                status = MxL_HRCLS_Ctrl_SelectAFETiltMode(devContextPtr, MXL_HRCLS_AFE_RESTORE, 
                    MXL_HRCLS_AFE_MODE_CHANGE_TIMEOUT_MS); //TODO: change timeout value
                devContextPtr->currentAfeBO = MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(devContextPtr);
                MXLERR(
                if (status != MXL_SUCCESS)
                {
                  MxL_HRCLS_ERROR("[HRCLS] %s Cannot restore AFE tilt\n", __FUNCTION__);
                }
                )
              }
            }
          }
        }
        status = MxLWare_HRCLS_OEM_WriteRegister(devId, 0xF098, feature&(~0x800));
      } else status = MXL_FAILURE;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
 *  @usage      After a tuner is enabled, this API can be called at any time to check 
 *              if the specified tuner is disabled, unlocked, or locked.
 *
 *  @equ261     MXL_TUNER_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerLockStatus(
    UINT8     devId,
    MXL_HRCLS_TUNER_ID_E tunerId,
    MXL_HRCLS_TUNER_STATUS_E* lockStatusPtr
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("Tuner= %s\n", (tunerId == MXL_HRCLS_FULLBAND_TUNER)?"FB":"NB"););
  if ((devContextPtr) && (lockStatusPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 data;
      if (tunerId == MXL_HRCLS_FULLBAND_TUNER)
      {
        if (devContextPtr->fbTunerPowerState != MXL_OFF)
        {
          status = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_AFE_TUNER_STATUS, &data);
          if (status == MXL_SUCCESS)
          {
            *lockStatusPtr = ((data & 0x03) == 0x02)?MXL_HRCLS_TUNER_LOCKED:MXL_HRCLS_TUNER_UNLOCKED;

            if (*lockStatusPtr == MXL_HRCLS_TUNER_LOCKED)
            {
              devContextPtr->currentAfeTiltMode = MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(devContextPtr);
            }
          }
        } else *lockStatusPtr = MXL_HRCLS_TUNER_DISABLED;
      }
      else
      {
        if (devContextPtr->nbTunerPowerState != MXL_OFF)
        {
          status = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_AFE_TUNER_STATUS, &data);
          if (status == MXL_SUCCESS)
          {
            *lockStatusPtr = (((data >> 2) & 0x03) == 0x00)?MXL_HRCLS_TUNER_UNLOCKED:MXL_HRCLS_TUNER_LOCKED;
          }
        } else *lockStatusPtr = MXL_HRCLS_TUNER_DISABLED;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
 *  @usage      After a DFE channel is tuned, this API can be called at any time to check
 *              if the specified DFE channel is disabled, unlocked, or locked.
 *
 *  @equ261     MXL_TUNER_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerChanStatus(
    UINT8     devId,
    MXL_HRCLS_CHAN_ID_E chanId,
    MXL_HRCLS_CHAN_STATUS_E* statusPtr
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("chanId=%d\n", chanId););
  if ((devContextPtr) && (statusPtr) && ((chanId < devContextPtr->dfeChanCount) || (chanId == MXL_HRCLS_CHAN_NB)))
  {
    if (devContextPtr->driverInitialized)
    {
      MXL_HRCLS_CHAN_ID_E phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);
      MXL_HRCLS_RX_PWR_ACCURACY_E localAccuracy;
       
      status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, phyChanId, NULL, &localAccuracy);

      *statusPtr = (localAccuracy == MXL_HRCLS_PWR_INVALID)?MXL_HRCLS_CHAN_UNLOCKED:MXL_HRCLS_CHAN_LOCKED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  chanId DFE channel ID
 *  @param[out] rxPwrIndBuVPtr pointer to returned Rx Power in dBuV 
 *
 *  @apibrief   This API returns instantaneous RX power of a DFE channel in dBuV 
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
    )
{
  MXL_STATUS_E    status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("chanId=%d\n", chanId););
  if ((devContextPtr) && (rxPwrIndBuVPtr) && ((chanId < devContextPtr->dfeChanCount) || (chanId == MXL_HRCLS_CHAN_NB)))
  {
    if (devContextPtr->driverInitialized)
    {
      MXL_HRCLS_CHAN_ID_E phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);
      MXL_HRCLS_RX_PWR_ACCURACY_E localAccuracy;
      UINT16 power;
       
      status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, phyChanId, &power, &localAccuracy);
      if ((status == MXL_SUCCESS) && (localAccuracy != MXL_HRCLS_PWR_INVALID))
      {
        if (phyChanId != MXL_HRCLS_CHAN_NB)
        {
          devContextPtr->currentAfeTiltMode = MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(devContextPtr);
          devContextPtr->currentAfeBO = MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(devContextPtr);
          status = MxL_HRCLS_Ctrl_CalGetCorrected(devContextPtr, MXL_DIV_ROUND(devContextPtr->dfeChannels[(UINT8) phyChanId].freqHz, 1000000), power, rxPwrIndBuVPtr); 
        }
        else
        {
          status = MxL_HRCLS_Ctrl_CalGetCorrectedPSM(devContextPtr, MXL_DIV_ROUND(devContextPtr->dfeChannels[(UINT8) phyChanId].freqHz, 1000000), power, rxPwrIndBuVPtr); 
        }
      } else *rxPwrIndBuVPtr=power;
      if (accuracyPtr) *accuracyPtr = localAccuracy;
      MXLDBG1(MxL_HRCLS_DEBUG("Channel%d power: %d.%ddBuV\n", chanId, (*rxPwrIndBuVPtr) / 10, (*rxPwrIndBuVPtr) % 10););
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
 *              Available windowing schemes are None, triangular(default) and raised cosine. 
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgTunerPowerSpectrum(
                         UINT8     devId,
                         UINT8     avgWindow,
                         MXL_HRCLS_FFT_WINDOW_TYPE_E windowType,
                         UINT16*   equivalentNoiseBwPtr)
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  UINT8 avgSetting;
  UINT8 windowSetting;
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("avgWindow=%d Window:%d\n", avgWindow, windowType););
 
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED)
      {
        if ((avgWindow >= MXL_HRCLS_FFT_AVG_SETTING_MIN) && (avgWindow <= MXL_HRCLS_FFT_AVG_SETTING_MAX) && (0 == (avgWindow & (avgWindow - 1))))
        {
          avgSetting = 0;
          while(avgWindow != (1<<avgSetting))
          {
            avgSetting++;
          }
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_SPEC_API_AVG_SETTING, avgSetting);
          windowSetting = (MXL_HRCLS_FFT_WINDOW_NONE == windowType)?MXL_HRCLS_FW_FFT_WINDOW_NONE:(MXL_HRCLS_FFT_WINDOW_TRIANGLE == windowType)?MXL_HRCLS_FW_FFT_WINDOW_TRIANGLE:MXL_HRCLS_FW_FFT_WINDOW_RAISED_COS;
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_SPEC_API_WIN_SETTING, windowSetting);
          if (equivalentNoiseBwPtr)
          {
            *equivalentNoiseBwPtr = (MXL_HRCLS_FFT_WINDOW_NONE == windowType)?MXL_HRCLS_EQU_NOISE_BW_NONE:(MXL_HRCLS_FFT_WINDOW_TRIANGLE == windowType)?MXL_HRCLS_EQU_NOISE_BW_TRIANGLE:MXL_HRCLS_EQU_NOISE_BW_RAISED_COS;
          }
          else
          {
            MXLDBG2(MxL_HRCLS_DEBUG("Parameter equivalentNoiseBwPtr is NULL\n"););
          }
        }
        else
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("Valid settings: Averaging-[1,2,4,8,16,32,64,128]\n"););
        }
      } 
      else 
      {
        MXLERR(MxL_HRCLS_ERROR("Averaging configuration is not supported without firmware loaded. Load firmware first\n");); 
        status = MXL_NOT_SUPPORTED;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  
  return (MXL_STATUS_E)status;
}

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
 *              The returned buffer contains power in units of dBmV with 2 decimal places, 
 *              in range of -327.68 dBmV to +327.67 dBmV. The absolute accuracy of the 
 *              measurement shall be +/- 3 dB.  The relative accuracy (from bin to bin) 
 *              shall be +/- 1 dB in the range +15 dBmV to -15 dBmV.
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
                         SINT16*   powerBufPtr)
{
  UINT32  currFreqStartInHz;
  UINT32  freqStepNum; 
  UINT32  stepsReq; 
  UINT16  stepsRsp = 0; 
  UINT16  stepCtr;
  UINT8 status = MXL_SUCCESS;
  UINT8 readStatus = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("freqStartInHz=%d, freqStepInHz=%d, numOfFreqSteps=%d\n", freqStartInHz, freqStepInHz, numOfFreqSteps););
 
  if ((devContextPtr) && (powerBufPtr) && (numOfFreqSteps)) 
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED)
      {
        UINT16 feature;
        status |= MxLWare_HRCLS_OEM_ReadRegister(devId, 0xF098, &feature);
        status |= MxLWare_HRCLS_OEM_WriteRegister(devId, 0xF098, feature|0x800);
        status |= MxL_HRCLS_Ctrl_ReqTunerTemperatureCode(devContextPtr); // Updates current temperature in context.

        if ((freqStartInHz >= MXL_HRCLS_FFT_START_FREQ_HZ_MIN) && ((UINT32) ((UINT32) freqStartInHz + (UINT32) (numOfFreqSteps * freqStepInHz)) <= MXL_HRCLS_FFT_END_FREQ_HZ_MAX) 
            && (freqStepInHz >= MXL_HRCLS_FFT_STEP_SIZE_HZ_MIN) && (freqStepInHz <= MXL_HRCLS_FFT_STEP_SIZE_HZ_MAX))
        {
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_FFT_SPEC_ENA, 1);
          
          // Read current tilt mode
          devContextPtr->currentAfeTiltMode = MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(devContextPtr);
          devContextPtr->currentAfeBO = MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(devContextPtr);      

          freqStepNum = 0;
          currFreqStartInHz = freqStartInHz;
          while ((freqStepNum < numOfFreqSteps) && (MXL_SUCCESS == status))
          {
            stepsReq = numOfFreqSteps - freqStepNum;  
            if (stepsReq > SPECTRUM_BIN_LIMIT)
            {
              stepsReq = SPECTRUM_BIN_LIMIT;
            }
            MXLDBG2(MxL_HRCLS_DEBUG("currFreqStartInHz =%d, stepsReq =%d\n", currFreqStartInHz, stepsReq););
            status |= MxL_HRCLS_Ctrl_CfgTunerFFTRunMode(devContextPtr, MXL_HRCLS_FFT_MODE_SPECTRUM_API, (MXL_HRCLS_FFT_SOURCE_TYPE_E)0/*D/C*/, 0/*D/C*/, (MXL_HRCLS_FFT_DEBUG_TYPE_E) 0/*D/C*/, currFreqStartInHz, freqStepInHz, stepsReq);
            if (MXL_SUCCESS == status)
            {
#ifndef _MXL_DIAG_ENABLED_              
              status =  MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_FFT_RSP_LEN, 0, 16, 0, MXL_FALSE, 600, &stepsRsp);
#else
              MxLWare_HRCLS_OEM_DelayUsec(510*1000); // Depends on averaging setting.
              status = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FFT_RSP_LEN, &stepsRsp);
              if (stepsRsp == 0) status = MXL_FAILURE;
#endif
              if (status == MXL_SUCCESS) 
              {
                MXLDBG2(MxL_HRCLS_DEBUG("freqStepNum =%d stepsRsp:%d\n", freqStepNum, stepsRsp););
//                status |= MxL_HRCLS_Ctrl_ReadTable16(devId, MXL_HRCLS_TABLE_ID_FFT_HOST_CMD_DATA, (UINT16 *) &powerBufPtr[freqStepNum], stepsRsp, 0);
                status |= MxL_HRCLS_Ctrl_ReadBlock16(devId, MXL_HRCLS_BLK_READ_SPEC_API, 0, stepsRsp, (UINT16*)&powerBufPtr[freqStepNum]);

                for (stepCtr = 0; (stepCtr < stepsRsp) && (MXL_SUCCESS == status); stepCtr++)
                {
                  status |= MxL_HRCLS_Ctrl_CalGetCorrected(devContextPtr,
                      MXL_DIV_ROUND((currFreqStartInHz + (stepCtr * freqStepInHz)+ freqStepInHz/2), 1000000),
                      MXL_DIV_ROUND_S((powerBufPtr[freqStepNum + stepCtr]*10), 64),
                      (UINT16*)&powerBufPtr[freqStepNum + stepCtr]);
                }
                freqStepNum += stepsRsp;
                currFreqStartInHz = (freqStepNum * freqStepInHz) + freqStartInHz;
                MXLDBG2(MxL_HRCLS_DEBUG("freqStepNum =%d freqStepInHz:%d currFreqStartInHz:%d \n", freqStepNum, freqStepInHz, currFreqStartInHz););
              }
              else
              {
                UINT16 errorCode = MXL_HRCLS_FFT_ERROR_OK;
                readStatus = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FFT_ERR_CODE, &errorCode);
                if (readStatus == MXL_SUCCESS)
                {
                  if (MXL_HRCLS_FFT_ERROR_RESOLUTION_NB == errorCode)
                  {
                    MXLERR(MxL_HRCLS_ERROR("Resolutions finer than 100KHz not avilable below 42.1875MHz and above 1054.6875MHz:%d\n", errorCode););
                    status = MXL_INVALID_PARAMETER;
                  }
                  else if (MXL_HRCLS_FFT_ERROR_RESOLUTION == errorCode)
                  {
                    MXLERR(MxL_HRCLS_ERROR("Requested resolution in currently unavailable. Free at least 1 channel to proceed:%d\n", errorCode););
                    status = MXL_NOT_READY;
                  }
                  else
                  {
                    MXLERR(MxL_HRCLS_ERROR("Invalid resolution:%d\n", errorCode););
                    status = MXL_INVALID_PARAMETER;
                  }
                }
                else
                {
                  MXLERR(MxL_HRCLS_ERROR("Failed to read error code\n"););
                }
              }
            }
            else
            {
              MXLERR(MxL_HRCLS_ERROR("FFT engine is not ready\n"););
              status = MXL_NOT_READY;
            }
          }
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_FFT_SPEC_ENA, 0);
        }
        else
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("FFT supports frequency range %dHz - %dHz, min step size: %dHz max step size:%d\n", 
                                  MXL_HRCLS_FFT_START_FREQ_HZ_MIN, MXL_HRCLS_FFT_END_FREQ_HZ_MAX, 
                                  MXL_HRCLS_FFT_STEP_SIZE_HZ_MIN , MXL_HRCLS_FFT_STEP_SIZE_HZ_MAX););
        }
        status |= MxLWare_HRCLS_OEM_WriteRegister(devId, 0xF098, feature&(~0x800));
      } 
      else 
      {
        MXLERR(MxL_HRCLS_ERROR("FFT dump is not supported without firmware loaded. Load firmware first\n");); 
        status = MXL_NOT_SUPPORTED;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  
  return (MXL_STATUS_E)status;
}

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
 *    #define MXL_HRCLS_INTR_MASK_PID               (1 << 0)
 *    #define MXL_HRCLS_INTR_MASK_CHAN_LOCKED       (1 << 1)
 *    #define MXL_HRCLS_INTR_MASK_FB_TUNER_LOCKED   (1 << 2)
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
                         UINT32     intrMask)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("trigger=%s, level=%s, globalEnable=%d, mask=0x%x\n", (edgeTrigger == MXL_TRUE)?"edge":"level", (positiveTrigger == MXL_TRUE)?"pos":"neg", (globalEnableInterrupt == MXL_TRUE)?1:0, intrMask););
 
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 controlReg = 0;
      UINT16 maskReg = 0;

      devContextPtr->interruptMask = intrMask;
#ifdef _MXL_HRCLS_DEMOD_ENABLED_
      intrMask = MxL_HRCLS_Ctrl_ConvertDemod2PhysicalInterruptMask(devContextPtr, intrMask);     
#endif
      maskReg = intrMask & 0xffff;
      status = MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_MBVAR_HOSTINT_MASK, maskReg);
      if (status == MXL_SUCCESS)
      {
        HOST_COMMAND_T command;

        devContextPtr->interruptMask = intrMask;
        controlReg |= (UINT16) ((edgeTrigger == MXL_TRUE)?MXL_HRCLS_INTR_CTRL_EDGE_TRIGGER:0);
        controlReg |= (UINT16) ((positiveTrigger == MXL_TRUE)?MXL_HRCLS_INTR_CTRL_ACTIVE_HIGH:0);
        controlReg |= (UINT16) ((intrMask)?MXL_HRCLS_INTR_CTRL_GLOBAL_ENABLE:0);  // disable interrupt machine if mask is zero
        controlReg |= (UINT16) ((globalEnableInterrupt == MXL_FALSE)?MXL_HRCLS_INTR_CTRL_GPIO_DISABLE:0); // enable or disable GPIO interrupt output 

        /* Form command payload */
        command.data[0] = (UINT8)((controlReg & 0x00FF) >> 0);
        command.data[1] = (UINT8)((controlReg         ) >> 8);
        command.payloadLen = 2;
        command.payloadCheckSum = 0;
        command.syncChar = 0;
        command.commandId = 0;
        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_DEV_HOST_INTERRUPT_CONTROL, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

        MXLDBG2(MxL_HRCLS_DEBUG("interrupt control register value: %04x\n", controlReg););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] intrStatusPtr Current interrupt status 
 *  @param[out] intrMaskPtr Current interrupt mask
 *
 *  @reltype
 *    @code
 *    #define MXL_HRCLS_INTR_MASK_PID               (1 << 0)
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
                         UINT32 *  intrMaskPtr)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
 
  if ((devContextPtr) && (intrStatusPtr)) 
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 statusReg;
      UINT32 outStatusReg;

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
      if (intrMaskPtr) *intrMaskPtr = MxL_HRCLS_Ctrl_ConvertDemod2LogicalInterruptMask(devContextPtr, devContextPtr->interruptMask);
#else
      if (intrMaskPtr) *intrMaskPtr = devContextPtr->interruptMask;
#endif
      status = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_MBVAR_HOSTINT_STATUS, &statusReg);
      outStatusReg = (UINT32) statusReg;
#ifdef _MXL_HRCLS_DEMOD_ENABLED_
      outStatusReg = MxL_HRCLS_Ctrl_ConvertDemod2LogicalInterruptMask(devContextPtr, outStatusReg);
#endif      
      if (status == MXL_SUCCESS) *intrStatusPtr = ((UINT32) outStatusReg) & 0xffff;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}


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
                         SINT16*   temperaturePtr)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  SINT16 grayCodeToTemp[] =  MxL_HRCLS_GRAY_CODE_TEMPERATURE;
  UINT16 grayCode;
  MXLENTERAPISTR(devId);
 
  if ((devContextPtr) && (temperaturePtr)) 
  {
    if ((devContextPtr->driverInitialized) && (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED))
    {
      if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (devContextPtr->chipVersion == 1))
      {
        status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_TSENS_READBACK, &grayCode); // Read back temp sensor
        if (MXL_SUCCESS == status)
        {
          *temperaturePtr = grayCodeToTemp[grayCode]; 
        }
      }
      else
      {
        status = MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_TEMPERATURE_DEGREES, (UINT16*)temperaturePtr);
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

