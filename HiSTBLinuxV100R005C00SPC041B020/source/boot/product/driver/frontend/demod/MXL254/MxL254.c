/*******************************************************************************
 *
 * FILE NAME          : example254.c
 *
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 07/15/2012
 *
 * DESCRIPTION        : Example code of MxL_HRCLS MxLWare API
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//#include <semaphore.h>
/*#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/semaphore.h>*/

#include "hi_type.h"

//#include "drv_i2c_ext.h"
//#include "drv_gpioi2c_ext.h"
//#include "drv_gpio_ext.h"
//#include "hi_drv_gpioi2c.h"

#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"

#include "hi_debug.h"

//#include "hi_drv_stat.h"
//#include "hi_drv_dev.h"
//#include "hi_drv_reg.h"
//#include "hi_kernel_adapt.h"
//#include "hi_drv_proc.h"

#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
//#include "hi_reg_common.h"


#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_OEM_Drv.h"
//#include "MxL254.h"
#include "hi_type.h"
//#include "drv_i2c_ext.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"


//#include "tuner.h"
/************************************************************************************
  Device ID used in this sample code.
  If more Hercules devices are used at the same time, they should have
  consecutive.
  This is customer's application responsibility to enumerate devices.
 ************************************************************************************/
#define HRCLS_DEVICE_ID 0

#define HRCLS_EXAMPLE_CODE_DEVICE "MxL254"

/************************************************************************************
  Default carystal capacitors value. This value is platform dependent.
 ************************************************************************************/
#define HRCLS_XTAL_CAP_VALUE 0 
#define HRCLS_I2C_ADDRESS 80
//#define HRCLS_PORT_MAX 4
#define MAX_TUNER (4)
/************************************************************************************
  Firmware filename.
 ************************************************************************************/
#define MXL254_CONNCECT_TIMEOUT	1000		/* ms */ 
static HI_U32 s_u32mxl254ConnectTimeout = MXL254_CONNCECT_TIMEOUT;

MXL_BOOL_E  xptSupported;

#ifdef MXL_HRCLS_254_ENABLE

static MXL_STATUS_E mxl_waitForTunerLock(MXL_HRCLS_TUNER_ID_E tunerId);
static MXL_STATUS_E mxl_waitForChannelLock(MXL_HRCLS_CHAN_ID_E chanId);

oem_data_t oemData;
HI_BOOL  g_Mxl254IninFlag = 0;
HI_U32  u32ResetGpioNo = 0;

//HI_U32 u32CurTunerPort = 0;
//struct semaphore mxl254_sem;

static HI_U8  g_Mxl254Fireware[]= 
{
  #include "./MxL_HRCLS_Firmware.h"
};

/**************************************************************************************
  Example code for firmware download
 *************************************************************************************/
static MXL_STATUS_E mxl_downloadFirmware(MXL_BOOL_E * xptSupported)
{
      MXL_STATUS_E result = MXL_FAILURE;
 /************************************************************************************
   Open firmware file in binary mode
  ************************************************************************************/
	UINT16 firmwareSize = 0;
       MXL_STATUS_E status;
	   
	firmwareSize =sizeof(g_Mxl254Fireware)/sizeof(HI_U8); 
		
       /************************************************************************************
         Download firmware. This is a blocking function. Downloading firmware may take
         a while. Actual execution depends also on target platform's I2C speed.
         Define callback function that will be called after every segment is downloaded
         if progress information is required.
        ************************************************************************************/
       // status = MxLWare_HRCLS_API_CfgDevFirmwareDownload(HRCLS_DEVICE_ID, firmwareSize, firmwareBuf, NULL);
       status = MxLWare_HRCLS_API_CfgDevFirmwareDownload(HRCLS_DEVICE_ID, firmwareSize, g_Mxl254Fireware, NULL);
        if (status == MXL_SUCCESS)
        {
          MXL_HRCLS_DEV_VER_T devVerInfo;
          HI_INFO_TUNER("[HRCLS][%d] {%s} Firmware download OK\n", HRCLS_DEVICE_ID, __FUNCTION__);

         /************************************************************************************
           After firmware is successfully downloaded, read its version.
          ************************************************************************************/
          if (MxLWare_HRCLS_API_ReqDevVersionInfo(HRCLS_DEVICE_ID, &devVerInfo) == MXL_SUCCESS)
          {
            result = MXL_SUCCESS;
            HI_INFO_TUNER("[HRCLS][%d] {%s} Firmware ver. %d.%d.%d.%d-RC%d\n", HRCLS_DEVICE_ID, __FUNCTION__,
              devVerInfo.firmwareVer[0], devVerInfo.firmwareVer[1],
              devVerInfo.firmwareVer[2], devVerInfo.firmwareVer[3],
              devVerInfo.firmwareVer[4]);
            if ((devVerInfo.chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devVerInfo.chipVersion != 1))
            {
              *xptSupported = MXL_TRUE;
            } else *xptSupported = MXL_FALSE;

          }
          else
          {
            HI_ERR_TUNER("[HRCLS][%d] {%s} Firmware version read FAILED\n", HRCLS_DEVICE_ID, __FUNCTION__);
          }
        }
        else
        {
          HI_ERR_TUNER("[HRCLS][%d] {%s} Firmware download FAILED (error code: %d)\n", HRCLS_DEVICE_ID, __FUNCTION__, status);
        }
     
  return result;
}

/**************************************************************************************
  Example code for driver (mxlware and firmware) version checking
 *************************************************************************************/
static MXL_STATUS_E mxl_checkVersion(/*@out@*/ MXL_HRCLS_DEV_VER_T* verInfoPtr)
{
  /************************************************************************************
    Read MxLWare, Firmware and Bootloader version.
   ************************************************************************************/
  MXL_STATUS_E status = MxLWare_HRCLS_API_ReqDevVersionInfo(HRCLS_DEVICE_ID, verInfoPtr);

  if (status == MXL_SUCCESS)
  {
    HI_INFO_TUNER("[HRCLS][%d] chipVersion=%d MxLWare: %d.%d.%d.%d-RC%d Firmware:%d.%d.%d.%d-RC%d Bootloader:%d.%d.%d.%d fwDownloaded=%d\n",
        HRCLS_DEVICE_ID,
        verInfoPtr->chipVersion,
        verInfoPtr->mxlWareVer[0],
        verInfoPtr->mxlWareVer[1],
        verInfoPtr->mxlWareVer[2],
        verInfoPtr->mxlWareVer[3],
        verInfoPtr->mxlWareVer[4],
        verInfoPtr->firmwareVer[0],
        verInfoPtr->firmwareVer[1],
        verInfoPtr->firmwareVer[2],
        verInfoPtr->firmwareVer[3],
        verInfoPtr->firmwareVer[4],
        verInfoPtr->bootLoaderVer[0],
        verInfoPtr->bootLoaderVer[1],
        verInfoPtr->bootLoaderVer[2],
        verInfoPtr->bootLoaderVer[3],
        verInfoPtr->firmwareDownloaded
        );
  }
  return status;
}

static MXL_STATUS_E mxl_lockDemod(MXL_HRCLS_CHAN_ID_E chanId, MXL_HRCLS_DMD_ID_E demodId, 
                                  UINT32 freqkHz, UINT8 bwMHz, MXL_HRCLS_ANNEX_TYPE_E annexType, 
                                  MXL_HRCLS_QAM_TYPE_E qamType, UINT16 symbolRatekSps, MXL_HRCLS_IQ_FLIP_E iqFlip, MXL_BOOL_E xptSupported)

{
  MXL_STATUS_E status = MXL_SUCCESS;
  
  if (annexType !=  MXL_HRCLS_ANNEX_OOB)
  {
	 
    if (xptSupported == MXL_FALSE)
    {
      MXL_HRCLS_MPEGOUT_PARAM_T mpegParams;

      // sample settings
      mpegParams.enable = MXL_ENABLE;
      mpegParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_MSB_1ST; 
      mpegParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BIT;
      mpegParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegErrorIndication = MXL_DISABLE;
      mpegParams.mpeg3WireModeEnable = MXL_ENABLE;
      mpegParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpeg3WireModeClkPol = MXL_HRCLS_MPEG_CLK_NEGATIVE;  

      status = MxLWare_HRCLS_API_CfgDemodMpegOutParams(HRCLS_DEVICE_ID, demodId, &mpegParams); 
    }
    else
    {
      MXL_HRCLS_XPT_MPEGOUT_PARAM_T mpegXptParams;

      // sample MPEG configuration
      mpegXptParams.enable = MXL_ENABLE;
      mpegXptParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_MSB_1ST;
      mpegXptParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BIT;
      mpegXptParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegClkPol = MXL_HRCLS_MPEG_CLK_POSITIVE;
      mpegXptParams.clkFreq = MXL_HRCLS_MPEG_CLK_56_21MHz; 
      mpegXptParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_2X;  // default setting;
      mpegXptParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_2X;  // default setting;
      mpegXptParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_2X;  // default setting;

      // In MxL254 and NO_MUX_4 mode, outputId = demodId
      // Check MxLWare API User Guide for other modes' mappings
      status = MxLWare_HRCLS_API_CfgXptOutput(HRCLS_DEVICE_ID, (MXL_HRCLS_XPT_OUTPUT_ID_E) demodId, &mpegXptParams);
    }
  }
  status = MxLWare_HRCLS_API_CfgTunerChanTune(HRCLS_DEVICE_ID, chanId, bwMHz, freqkHz * 1000);
  status |= mxl_waitForChannelLock(chanId);
  if (status == MXL_SUCCESS)
  {
    status = MxLWare_HRCLS_API_CfgDemodEnable(HRCLS_DEVICE_ID, demodId, MXL_TRUE);
    if (status == MXL_SUCCESS)
    {
      status = MxLWare_HRCLS_API_CfgDemodAdcIqFlip(HRCLS_DEVICE_ID, demodId, iqFlip);
      if (status == MXL_SUCCESS)
      {
        status = MxLWare_HRCLS_API_CfgDemodAnnexQamType(HRCLS_DEVICE_ID, demodId, annexType, qamType);
        if (status == MXL_SUCCESS)
        {
          if (annexType != MXL_HRCLS_ANNEX_OOB)
          {
            status = MxLWare_HRCLS_API_CfgDemodSymbolRate(HRCLS_DEVICE_ID, demodId, symbolRatekSps * 1000, symbolRatekSps * 1000);
          }
          else
          {
            status = MxLWare_HRCLS_API_CfgDemodSymbolRateOOB(HRCLS_DEVICE_ID, demodId, (MXL_HRCLS_OOB_SYM_RATE_E) symbolRatekSps);
          }
          if (status == MXL_SUCCESS)
          {
            status = MxLWare_HRCLS_API_CfgDemodRestart(HRCLS_DEVICE_ID, demodId);
          }
        }
      }
    }
  }
  return status;
}

/**************************************************************************************
  Example code to wait for tuner lock
 *************************************************************************************/
static MXL_STATUS_E mxl_waitForTunerLock(MXL_HRCLS_TUNER_ID_E tunerId/*TODO: timeout*/)
{
  MXL_STATUS_E status;
  MXL_HRCLS_TUNER_STATUS_E lockStatus = MXL_HRCLS_TUNER_DISABLED;
  do
  {
    MxLWare_HRCLS_OEM_DelayUsec(1000); // TODO: interval to be determined by BE

    status = MxLWare_HRCLS_API_ReqTunerLockStatus(HRCLS_DEVICE_ID, tunerId, &lockStatus);
  } while ((status == MXL_SUCCESS) && (lockStatus != MXL_HRCLS_TUNER_LOCKED));      // TODO: add timeout checking

  return ((status == MXL_SUCCESS) && (lockStatus == MXL_HRCLS_TUNER_LOCKED))?MXL_SUCCESS:MXL_FAILURE;
}

/**************************************************************************************
  Example code to wait for DFE channel lock
 *************************************************************************************/
static MXL_STATUS_E mxl_waitForChannelLock(MXL_HRCLS_CHAN_ID_E chanId/*TODO: timeout*/)
{
  MXL_STATUS_E status;
  MXL_HRCLS_CHAN_STATUS_E lockStatus = MXL_HRCLS_CHAN_DISABLED;
  do
  {
    MxLWare_HRCLS_OEM_DelayUsec(1000);   // TODO: interval to be determined by BE

    status = MxLWare_HRCLS_API_ReqTunerChanStatus(HRCLS_DEVICE_ID, chanId, &lockStatus);
  } while ((status == MXL_SUCCESS) && (lockStatus != MXL_HRCLS_CHAN_LOCKED));      // TODO: add timeout checking

  return ((status == MXL_SUCCESS) && (lockStatus == MXL_HRCLS_CHAN_LOCKED))?MXL_SUCCESS:MXL_FAILURE;
}

/**************************************************************************************
  Example code for FB tuner config 
 *************************************************************************************/
static MXL_STATUS_E mxl_enableFbTuner(void)
{
  MXL_STATUS_E status;

  /************************************************************************************
    Enable Fullband tuner
   ************************************************************************************/
  status = MxLWare_HRCLS_API_CfgTunerEnable(HRCLS_DEVICE_ID, MXL_HRCLS_FULLBAND_TUNER);
  if (status != MXL_FAILURE)
  {
    /************************************************************************************
      Wait for FB tuner to lock
     ************************************************************************************/
    status = mxl_waitForTunerLock(MXL_HRCLS_FULLBAND_TUNER);  
    if (status != MXL_SUCCESS)
    {
      HI_INFO_TUNER("[HRCLS][%d] {%s} Fullband tuner synth lock TIMEOUT!\n", HRCLS_DEVICE_ID, __FUNCTION__);
    }
  }
  else
  {
  }

  return MXL_SUCCESS;
}


HI_S32 mxl254_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
	MXL_STATUS_E status = 0;
	MXL_HRCLS_CHAN_ID_E chanId = 0;
	MXL_HRCLS_DMD_ID_E demodId = 0;
	UINT32 freqkHz;
	UINT8 bwMHz=8;
	MXL_HRCLS_QAM_TYPE_E qamType;
	UINT16 symbolRatekSps;
	MXL_HRCLS_ANNEX_TYPE_E annexType = MXL_HRCLS_ANNEX_A ;
	//HI_S32 s32FuncRet = HI_SUCCESS;
	
	if(u32TunerPort >= MAX_TUNER)
      {
           HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,MAX_TUNER);
           return HI_FAILURE;
       }
	switch(u32TunerPort)
	{
	    case 0:
		chanId = MXL_HRCLS_CHAN0;
		demodId = MXL_HRCLS_DEMOD0;
		break;
	    case 1:
		chanId = MXL_HRCLS_CHAN1;
		demodId = MXL_HRCLS_DEMOD1;
		break;
	    case 2:
		chanId = MXL_HRCLS_CHAN2;
		demodId = MXL_HRCLS_DEMOD2;
		break;
	    case 3:
		chanId = MXL_HRCLS_CHAN3;
		demodId = MXL_HRCLS_DEMOD3;
		break;
	    default:
		HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,MAX_TUNER);
		return HI_FAILURE;
		break;
				
	}
       switch(pstChannel->enQamType)
	{
	    case QAM_TYPE_16:
		qamType =MXL_HRCLS_QAM16 ;
		break;
	    case QAM_TYPE_32:
		qamType =MXL_HRCLS_QAM32 ;
		break;
	    case QAM_TYPE_64:
		qamType =MXL_HRCLS_QAM64 ;
		break;
	    case QAM_TYPE_128:
		qamType =MXL_HRCLS_QAM128 ;
		break;
	    case QAM_TYPE_256:
		qamType =MXL_HRCLS_QAM256 ;
		break;
	    default:
		HI_ERR_TUNER( "QamType(%d) is unsupported \n",pstChannel->enQamType);
		return HI_FAILURE;
		break;				
	}

	g_stTunerOps[u32TunerPort].u32CurrQamMode = pstChannel->enQamType;
      
       switch(g_stTunerOps[u32TunerPort].enSigType)
	{ 
	    case HI_UNF_TUNER_SIG_TYPE_CAB:
		     annexType = MXL_HRCLS_ANNEX_A; 
		     bwMHz = 8;
		     break;
	    case HI_UNF_TUNER_SIG_TYPE_J83B:
		     annexType = MXL_HRCLS_ANNEX_B; 
		     bwMHz = 6;
		     break;
	    default:
		     break;
       }
      // status = down_interruptible(&mxl254_sem);	
      symbolRatekSps = pstChannel->unSRBW.u32SymbolRate / 1000; /* KHz */
	freqkHz = pstChannel->u32Frequency;


	status = mxl_lockDemod(chanId, demodId, freqkHz, bwMHz, annexType, qamType, symbolRatekSps, MXL_HRCLS_IQ_AUTO, xptSupported);
	//status = mxl_lockDemod(0, 0, freqkHz, bwMHz, annexType, qamType, symbolRatekSps, MXL_HRCLS_IQ_AUTO, xptSupported);

	//MXL_HRCLS_ANNEX_A  6M带宽
	

  	if (status != MXL_SUCCESS)
  	{
		//up(&mxl254_sem);
    		return status;
  	}
	#if 0
  	MxLWare_HRCLS_OEM_DelayUsec(100000); 
  	{
   	 	MXL_BOOL_E qamLock, fecLock, mpegLock, relock;
    		status = MxLWare_HRCLS_API_ReqDemodAllLockStatus(HRCLS_DEVICE_ID, demodId, &qamLock, &fecLock, &mpegLock, &relock);

    		//status |= MxLWare_HRCLS_API_CfgUpdateDemodSettings(HRCLS_DEVICE_ID, demodId);

  	}
	#endif
	 //up(&mxl254_sem);
    return status;
}

  
/**************************************************************************************
   Example code for device initialization, and FB and NB tuner handover
 *************************************************************************************/

//HI_S32 mxl254_init(HI_U32 u32TunerPort,HI_U32 u32I2cChannel, HI_U32 u32DevAddress)
HI_S32 mxl254_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
      
  /************************************************************************************
    Driver initialization. Call this API function before any other function.
    Pass NULL as oemDataPtr as no extra data needs to be provided to OEM function.
    Device (devId) must be enumerated by customer software. devId enumaration must 
    always start from 0.    
   ************************************************************************************/

	MXL_STATUS_E status;
	//UINT16 regData = 0xfe;
	MXL_HRCLS_DEV_VER_T devVerInfo;
	//HI_S8 bRet = -1;

	HI_INFO_TUNER("Device: %s (version %s)\n", HRCLS_EXAMPLE_CODE_DEVICE, HRCLS_EXAMPLE_CODE_VERSION);
	if (u32TunerPort >= MAX_TUNER)
	{
	HI_ERR_TUNER("Bad u32TunerPort\n");
	return HI_FAILURE;
	}

	//HI_INIT_MUTEX(&mxl254_sem);
	//status = down_interruptible(&mxl254_sem);
	//oemData.i2cAddress = u32DevAddress; 
	oemData.i2cAddress = g_stTunerOps[u32TunerPort].u32DemodAddress;
	oemData.i2cIndex = enI2cChannel;
	//u32ResetGpioNo = s_u32ResetGpioNo[u32TunerPort]; 
	
	 if(!g_Mxl254IninFlag)
	 {
	        if(enTunerDevType == HI_UNF_TUNER_DEV_TYPE_MXL214)
	        {
	            status = MxLWare_HRCLS_API_CfgDrvInit(HRCLS_DEVICE_ID, (void *) &oemData, MXL_HRCLS_DEVICE_214);
	        }
	    	else
	    	{
	    	    status = MxLWare_HRCLS_API_CfgDrvInit(HRCLS_DEVICE_ID, (void *) &oemData, MXL_HRCLS_DEVICE_254);
	    	}
	    	if (status != MXL_SUCCESS)
	    	{
		    	HI_ERR_TUNER("[HRCLS][%d] Driver initialization FAILED\n", HRCLS_DEVICE_ID);
		    	//up(&mxl254_sem);
		    	return status;
	    	}
          
		  /************************************************************************************
		    Perform hardware reset of the device.
		   ************************************************************************************/
		  status = MxLWare_HRCLS_API_CfgDevReset(HRCLS_DEVICE_ID);
		  if (status != MXL_SUCCESS)
		  {
		    HI_ERR_TUNER("[HRCLS][%d] . reset FAILED\n", HRCLS_DEVICE_ID);
		    //up(&mxl254_sem);
		    return status;
		  }
          g_Mxl254IninFlag = HI_TRUE;
    }
	  /************************************************************************************
	    Capacitor value is don't care.
	   ************************************************************************************/
	  status = MxLWare_HRCLS_API_CfgDevXtalSetting(HRCLS_DEVICE_ID, HRCLS_XTAL_CAP_VALUE);
	  //HRCLS_XTAL_CAP_VALUE 可以调整晶振
	  if (status != MXL_SUCCESS)
	  {
	    HI_ERR_TUNER("[HRCLS][%d] Xtal setting FAILED\n", HRCLS_DEVICE_ID);
           //up(&mxl254_sem);
	    return status;
	  }
#if 0
	  status = MxLWare_HRCLS_API_CfgTunerDsCalDataLoad(HRCLS_DEVICE_ID);
	  if (status != MXL_SUCCESS)
	  {
	       HI_ERR_TUNER("[HRCLS][%d] Calibration data not available. Power reporting will not be accurate,status :%d \n", HRCLS_DEVICE_ID,status);
	       up(&mxl254_sem);
		return status;
	  }
#endif    
	  /************************************************************************************
	    Check MxLWare, Firmware and Bootloader version.
	   ************************************************************************************/
	  status = mxl_checkVersion(&devVerInfo);
	  if (status != MXL_SUCCESS)
	  {
		HI_ERR_TUNER("[HRCLS][%d] Version checking FAILED!\n", HRCLS_DEVICE_ID);
		//up(&mxl254_sem);
		return status;
	  }

	  /************************************************************************************
	    Make sure firmware is not already downloaded
	   ************************************************************************************/
	  if (devVerInfo.firmwareDownloaded == MXL_TRUE)
	  {
	    // Something must be wrong, because we just went through reset
	    HI_ERR_TUNER("[HRCLS][%d] Firmware already running. Forgot about h/w reset?\n", HRCLS_DEVICE_ID);
	    //up(&mxl254_sem);
	    return MXL_FAILURE;
	  }

	  /************************************************************************************
	    Download firmware
	   ************************************************************************************/
	  status = mxl_downloadFirmware(&xptSupported);
	  if (status != MXL_SUCCESS)
	  {
	       HI_ERR_TUNER("[HRCLS][%d] Firmware download FAILED\n", HRCLS_DEVICE_ID);
              //up(&mxl254_sem);
		return status;
	  }

	// xptSupported = MXL_TRUE;
	  /************************************************************************************
	    Enable fullband tuner 
	   ************************************************************************************/
	  status = mxl_enableFbTuner();
	  if (status != MXL_SUCCESS)
	  {
	    HI_ERR_TUNER("[HRCLS][%d] Enable FB tuner FAILED\n", HRCLS_DEVICE_ID);
	    //up(&mxl254_sem);
	    return HI_FAILURE;
	  }
	  
	  if (xptSupported == MXL_TRUE)
	  {
	    // This step can be skipped as NO_MUX_4 is a default mode
	    status = MxLWare_HRCLS_API_CfgXpt(HRCLS_DEVICE_ID, MXL_HRCLS_XPT_MODE_NO_MUX_4); 
	  }
	  //else
	 // {
	   // status = MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(HRCLS_DEVICE_ID, MXL_HRCLS_MPEG_CLK_POSITIVE, MXL_HRCLS_MPEG_DRV_MODE_1X, MXL_HRCLS_MPEG_CLK_42_18MHz);
	  //}

	  #if 1
	  // Only needed for 4-wire TS mode, skip for 3-wire mode
	  status = MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(HRCLS_DEVICE_ID, MXL_HRCLS_MPEG_CLK_POSITIVE, MXL_HRCLS_MPEG_DRV_MODE_1X, MXL_HRCLS_MPEG_CLK_56_21MHz);
	  if (status != MXL_SUCCESS)
	  {
		HI_ERR_TUNER("[HRCLS][%d] Global MPEG params setting FAILED\n", HRCLS_DEVICE_ID);
		return status;
	  }
	  #endif
	  if (status != MXL_SUCCESS)
	  {
	    HI_ERR_TUNER("[HRCLS][%d] Global MPEG params setting failed\n", HRCLS_DEVICE_ID);
	    //up(&mxl254_sem);
	    return HI_FAILURE;
	  }

	  //up(&mxl254_sem);
  
    return HI_SUCCESS;
}

HI_S32 mxl254_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
   MXL_STATUS_E status = MXL_SUCCESS;
   UINT16  snrtemp = 0;
   HI_TUNER_CHECKPOINTER( pu32SNR);
   status = MxLWare_HRCLS_API_ReqDemodSnr(HRCLS_DEVICE_ID, u32TunerPort,&snrtemp);
   if (status != MXL_SUCCESS)
  {
    HI_ERR_TUNER("[HRCLS][%d] MxLWare_HRCLS_API_ReqDemodSnr failed \n",__LINE__);

    return HI_FAILURE;
  }

   pu32SNR[0] = (HI_U32)snrtemp;
  
  return HI_SUCCESS;
}
HI_VOID mxl254_connect_timeout(HI_U32 u32ConnectTimeout)
{
    s_u32mxl254ConnectTimeout = u32ConnectTimeout;
    return;
}
HI_S32 mxl254_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
	 return 0; 
}
HI_S32 mxl254_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
   
     MXL_STATUS_E status =MXL_FAILURE;
     MXL_BOOL_E qamLock =MXL_FALSE;
     MXL_BOOL_E relock =MXL_FALSE;
     MXL_BOOL_E mpegLock =MXL_FALSE;
     MXL_BOOL_E fecLock =MXL_FALSE;
  
   // MXL_HRCLS_TUNER_STATUS_E tunerlockStatus = MXL_HRCLS_TUNER_UNLOCKED;
   // MXL_HRCLS_CHAN_STATUS_E demodstatus = MXL_HRCLS_CHAN_UNLOCKED;
	
    HI_TUNER_CHECKPOINTER( penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    status = MxLWare_HRCLS_API_ReqDemodAllLockStatus(HRCLS_DEVICE_ID, u32TunerPort, &qamLock, &fecLock, &mpegLock, &relock);

     if  ((MXL_TRUE == qamLock) &&(MXL_TRUE == fecLock)&&(MXL_TRUE == mpegLock))
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    else
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    }
    
    return HI_SUCCESS;
}

/*HI_S32 mxl254_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    MXL_STATUS_E status = MXL_FAILURE;
    UINT16   PwrIndBuV=0;
    //MXL_HRCLS_RX_PWR_ACCURACY_E accuracy =MXL_HRCLS_PWR_INVALID ;
    MXL_HRCLS_RX_PWR_ACCURACY_E accuracy =MXL_HRCLS_PWR_AVERAGED ;
	
    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
     status = MxLWare_HRCLS_API_ReqTunerRxPwr(HRCLS_DEVICE_ID,u32TunerPort,&PwrIndBuV,&accuracy);
     if ((MXL_SUCCESS != status) || (MXL_HRCLS_PWR_INVALID == accuracy ))
    {
	   HI_ERR_TUNER("mxl254_get_signal_strength error!\n");
          return HI_FAILURE;
    }

    pu32SignalStrength[1] = (HI_U32)(PwrIndBuV / 10 - 5);
    return HI_SUCCESS;
}

HI_S32 mxl254_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{

    HI_U32 au32Tmp = 0;
   
	HI_U32 i=0;
	HI_U32 j=0;
	HI_U32 au32Ber= 0;
    MXL_STATUS_E status = MXL_FAILURE;
    MXL_HRCLS_DMD_STAT_CNT_T   statscount={0};
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
	
    status = MxLWare_HRCLS_API_ReqDemodErrorStat(HRCLS_DEVICE_ID,u32TunerPort,&statscount);
    if (MXL_SUCCESS != status)
    {
	   HI_ERR_TUNER("mxl254_get_ber error!\n");
       return HI_FAILURE;
    }
	#if 1
	if (statscount.CwReceived > 0)
	{
		//cer
		au32Tmp = (HI_U32) u64mod((HI_U64)(statscount.CwErrCount *10000000) ,(statscount.CwReceived*1));
		//ber FEC before
		//au32Ber = (HI_U32) u64mod((HI_U64)(statscount.CorrBits * 1000000) , (statscount.CwReceived * 204 * 8));
		au32Ber = (HI_U32) u64mod((HI_U64)(statscount.CorrBits * 6127) , (statscount.CwReceived));//6127 = 10000000 /(204*8)
			
		au32Ber  = au32Ber	+ 26*(au32Tmp);
 
		if (au32Ber != 0)
		{
			while (au32Ber >= 10)
			{
			    au32Ber = au32Ber /10;
			    i++;
				if(au32Ber >= 1000)
				{ 
				   j = i;
				   au32Tmp = au32Ber;
				   
				}
			}
		}

	    pu32ber[0] = (HI_U32)au32Ber;
	    pu32ber[1] = (HI_U32)(au32Tmp % 1000 );
	    pu32ber[2] = (HI_U32)(7 - i);
		
	}
	else
	{
		pu32ber[0] = (HI_U32)1;
	    pu32ber[1] = (HI_U32)0;
	    pu32ber[2] = (HI_U32)0;

	}

	status = MxLWare_HRCLS_API_CfgDemodErrorStatClear(HRCLS_DEVICE_ID,u32TunerPort);
	if (MXL_SUCCESS != status)
	{
		HI_ERR_TUNER("mxl254_get_ber error!\n");
		return HI_FAILURE;
	}

#endif
#if 0
	if (statscount.CwReceived > 0)
	{
		//cer
		au32Tmp = (HI_U32) u64mod((HI_U64)(statscount.CwErrCount * 10000000) ,(HI_U64)(statscount.CwReceived*1));
		//ber FEC before
		au32Ber = (HI_U32) u64mod((HI_U64)(statscount.CorrBits * 10000000) , (statscount.CwReceived * 204 * 8)) + (26 * au32Tmp);
		if (au32Ber != 0)
		{
			while (au32Ber >= 10)
			{
			    au32Ber = au32Ber /10;
			    i++;
				if(au32Tmp >= 1000)
				{ 
				   j = i;
				   au32Tmp = au32Ber;
				}
			}
		}

	    pu32ber[0] = (HI_U32)au32Ber;
	    pu32ber[1] = (HI_U32)(au32Tmp % 1000 );
	    pu32ber[2] = (HI_U32)(7 - i);
		}
		else
		{
			pu32ber[0] = (HI_U32)1;
		    pu32ber[1] = (HI_U32)0;
		    pu32ber[2] = (HI_U32)0;
		
		}
#endif

	return HI_SUCCESS;
}

HI_S32 mxl254_get_powerspecdata(HI_U32 u32TunerPort, HI_U32 u32freqStartInHz,HI_U32 u32freqStepInHz,HI_U32 u32numOfFreqSteps,HI_S16 *ps16powerData)
{
	MXL_STATUS_E status = MXL_FAILURE;
    UINT16   equivalentNoiseBw = 0;
	HI_TUNER_CHECKPOINTER( ps16powerData);
	if (u32TunerPort >= MAX_TUNER)
	{
		HI_ERR_TUNER("Bad u32TunerPort\n");
		return HI_FAILURE;
	}
	status = MxLWare_HRCLS_API_ReqTunerPowerSpectrum(HRCLS_DEVICE_ID,u32freqStartInHz,u32freqStepInHz,u32numOfFreqSteps,ps16powerData);
    if (MXL_SUCCESS != status) 
	{
	   HI_ERR_TUNER("mxl254_get_powerspecdata error!\n");
	   return HI_FAILURE;
	}
	return HI_SUCCESS;
}*/


#endif
#define HRCLS_EXAMPLE_CODE_VERSION "1.10"

