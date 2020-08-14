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


#include <linux/module.h>
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
#include <linux/semaphore.h>

//hisilicon
#include "hi_type.h"

#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_drv_proc.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#include "hi_kernel_adapt.h"

//mxl68x
#include "MaxLinearDataTypes.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhyCfg.h"



#define DEFAULT_MXL68X_ID          0


#define MXL68X_CODE_DEVICE "MxL68x"
#define MXL68X_CODE_VERSION "1.2.4.3"

struct semaphore mxl68x_sem;
static oem_data_t oemData;

static HI_U32 s_u32mxl68xConnectTimeout = 1000; /* ms */

static HI_U8  g_Mxl68xFireware[]= 
{
  /*
    MxL68x_FW_V1.h contains FW for V1 version of the chip and 
    MxL68x_FW_V1_200.h contains FW for the V1_200 version of the chip
  */
  #include "MxL68x_FW_V1_200.h"
};

static MXL_STATUS_E mxl68x_downloadFirmware(UINT8 DEV_ID)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    UINT32 fwBuffSize;
    UINT8 *fwBuffPtr = NULL;
    //long fileSize = 0;

    fwBuffSize = sizeof(g_Mxl68xFireware)/sizeof(HI_U8);
    fwBuffPtr = g_Mxl68xFireware;
    
    status = MxLWare68x_API_CfgDevFirmwareDownload(DEV_ID, (UINT32)fwBuffSize, fwBuffPtr);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : Firmware Download %d\n", status);
    }
    
    return status;
}

HI_S32 mxl68x_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
   	MXL_STATUS_E status = MXL_SUCCESS;
	UINT8 MxL68xDeviceId;
    //MXL68x_FW_STATUS_E firmwareDownloadStatus;
    
    MxL68xDeviceId = DEFAULT_MXL68X_ID;

	HI_INFO_TUNER("Device: %s (version %s)\n", MXL68X_CODE_DEVICE, MXL68X_CODE_VERSION);
	if (u32TunerPort >= TUNER_NUM)
	{
	HI_ERR_TUNER("Bad u32TunerPort\n");
	return HI_FAILURE;
	}

	HI_INIT_MUTEX(&mxl68x_sem);
	status = down_interruptible(&mxl68x_sem);

    /* i2c config */
	oemData.i2cAddress = g_stTunerOps[u32TunerPort].u32DemodAddress;
	oemData.i2cIndex = enI2cChannel;
    
    // Init driver context
	status = MxLWare68x_API_CfgDrvInit(MxL68xDeviceId, (void *) &oemData);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDrvInit %d\n", status);
        up(&mxl68x_sem);
        return status;
    }
    
    // Config Power Supply
    status = MxLWare68x_API_CfgDevPowerSupply(MxL68xDeviceId, MXL68x_SINGLE_SUPPLY);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDevPowerSupply %d\n", status);
        up(&mxl68x_sem);
        return status;
    }

    // Config XTAL
    status = MxLWare68x_API_CfgDevXtal(MxL68xDeviceId,
                                     MXL68x_XTAL_16MHz,   // 16 MHz
                                     25,                  // 25 pF
                                     MXL_ENABLE,          // Enable CLK out
                                     MXL_FALSE,           // Disable CLK DIV
                                     MXL_FALSE,           // Disable XTAL Sharing
            						 MXL_ENABLE);         // Enable XTAL Cal for Master, Disable for Slave
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDevXtal %d\n", status);
        up(&mxl68x_sem);
        return status;
    }

    // Config Dev Init, switch clock to PLL clock
    status = MxLWare68x_API_CfgDevInit(MxL68xDeviceId,
                                     MXL68x_IF_NA,          // Do not set IF mux
                                     MXL_FALSE);            // Disable RF Loop-Thru
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDevInit %d\n", status);
        up(&mxl68x_sem);
        return status;
    }
    
    // Download firmware
    status = mxl68x_downloadFirmware(MxL68xDeviceId);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDevFirmwareDownload %d\n", status);
        up(&mxl68x_sem);
        return status;
    }
    else
    {
        HI_INFO_TUNER(" Download firmware success. \n");
        MxLWare68x_OEM_SleepInUs(100000); // wait for 100 ms
    }

    // Change Device Operational Mode to Tuner+Demod
    status = MxLWare68x_API_CfgDevOperationalMode(MxL68xDeviceId, MXL68x_TUNER_DEMOD);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("\nErr: MxLWare68x_API_CfgDevOperationalMode %d\n", status);
        up(&mxl68x_sem);
        return status;
    }

    // Config IF Mux and RF loop through
    status = MxLWare68x_API_CfgDevInit(MxL68xDeviceId,
                                     MXL68x_IF_INTERNAL,    // Initernal IF
                                     MXL_FALSE);            // Disable RF Loop-Thru
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("Err : MxLWare68x_API_CfgDevInit %d\n", status);
        up(&mxl68x_sem);
        return status;
    }

    up(&mxl68x_sem);

    return status;
}


HI_S32 mxl68x_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
	MXL_STATUS_E status = 0;
#if 1
	UINT32 freqkHz;
	MXL68x_BW_E bwMHz = MXL68x_BW_6MHz;
    UINT8 MxL68xDeviceId = DEFAULT_MXL68X_ID;

	if(u32TunerPort >= TUNER_NUM)
    {
       HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,TUNER_NUM);
       return HI_FAILURE;
    }
    
    if(HI_UNF_TUNER_SIG_TYPE_ISDB_T != g_stTunerOps[u32TunerPort].enSigType)
	{ 
	    HI_ERR_TUNER( "enSigType(%d) is unsupported \n",g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }

    freqkHz = pstChannel->u32Frequency;
    switch(pstChannel->unSRBW.u32BandWidth)
	{
	    case 6000:
    		bwMHz =MXL68x_BW_6MHz;
    		break;
	    case 7000:
    		bwMHz =MXL68x_BW_7MHz;
    		break;
	    case 8000:
    		bwMHz =MXL68x_BW_8MHz;
    		break;
	    default:
    		HI_ERR_TUNER( "BW(%d KHz) is unsupported \n",pstChannel->unSRBW.u32BandWidth);
    		return HI_FAILURE;			
	}
     
    status = down_interruptible(&mxl68x_sem);	

    // Set or Change Channel
    status = MxLWare68x_API_CfgTunerChanTune(MxL68xDeviceId,
                                             MXL68x_VIEW_MODE,
                                             bwMHz,
                                             freqkHz*1000/*473143000*/,
                                             MXL68x_AUTO_SPEC_INV_DISABLE,
                                             MXL_FALSE);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("\nErr: MxLWare68x_API_CfgTunerChanTune %d. [f:%dkHz,B:%d]\n", status,freqkHz,bwMHz);
        up(&mxl68x_sem);
        return status;
    }

    up(&mxl68x_sem);
#endif
    return status;
}

  
HI_S32 mxl68x_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
#if 1
    MXL_STATUS_E status =MXL_FAILURE;
    MXL_BOOL_E rfSynthLock, refSynthLock;
    MXL_BOOL_E rsLockA, rsLockB, rsLockC, tmccLock;
    UINT8  MxL68xDeviceId = DEFAULT_MXL68X_ID;

    HI_TUNER_CHECKPOINTER( penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
      HI_ERR_TUNER("Bad u32TunerPort\n");
      return HI_FAILURE;
    }

    // Check Tuner lock status
    status = MxLWare68x_API_ReqTunerLockStatus(MxL68xDeviceId, &rfSynthLock, &refSynthLock);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("\nErr: MxLWare68x_API_ReqTunerLockStatus %d\n", status);
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_TUNER("***** Tuner Lock status: RF Synth Lock = %d, REF Synth Lock = %d \n", rfSynthLock, refSynthLock);
        if((rfSynthLock != MXL_LOCKED) && (refSynthLock != MXL_LOCKED))
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            return HI_SUCCESS;
        }
    }

    // Check Demod lock status
    status = MxLWare68x_API_ReqDemodLockStatus(MxL68xDeviceId, &tmccLock, &rsLockA, &rsLockB, &rsLockC);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("\nErr: MxLWare68x_API_ReqDemodLockStatus %d\n", status);
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_TUNER("***** Demod Lock status: Frame lock = %d, Layer_A RS = %d, Layer_B RS = %d, Layer_C RS = %d \n", tmccLock, rsLockA, rsLockB, rsLockC);
        if  ((MXL_LOCKED == tmccLock) &&((MXL_LOCKED == rsLockA)||(MXL_LOCKED == rsLockB)||(MXL_LOCKED == rsLockC)))
        {
          *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
        }
        else
        {
          *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        }
    }
#endif
    return HI_SUCCESS;
}

HI_S32 mxl68x_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    MXL_STATUS_E status = MXL_FAILURE;
    UINT8  MxL68xDeviceId = DEFAULT_MXL68X_ID;
    SINT32 rxPower; //RF Input Power in dBm
        
    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
    status = MxLWare68x_API_ReqTunerRxPower(MxL68xDeviceId,&rxPower);
    if ((MXL_SUCCESS != status))
    {
        HI_ERR_TUNER("mxl68x_get_signal_strength error!\n");
        return HI_FAILURE;
    }

    pu32SignalStrength[1] = (HI_U32)((0-rxPower)/100);
    HI_INFO_TUNER("RF Input Power: %d dBm,ss:%d. \n",rxPower,pu32SignalStrength[1]);
    return HI_SUCCESS;
}

HI_S32 mxl68x_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    UINT8  MxL68xDeviceId = DEFAULT_MXL68X_ID;
    SINT32 cnrInt = 0;
    UINT32 cnrRaw = 0;
    
    HI_TUNER_CHECKPOINTER( pu32SNR);
    status = MxLWare68x_API_ReqDemodCNR(MxL68xDeviceId, &cnrRaw, &cnrInt);
    if (status != MXL_SUCCESS)
    {
        HI_ERR_TUNER("\nErr: MxLWare68x_API_ReqDemodCNR %d\n", status);
        return HI_FAILURE;
    }

    *pu32SNR = cnrInt;
    HI_INFO_TUNER("CNR: %d dB\n", *pu32SNR);
    return HI_SUCCESS;
}

HI_S32 mxl68x_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    MXL_STATUS_E status = MXL_FAILURE;
    MXL68x_BER_T berA, berB, berC;
    HI_U32 wIter = 0;
    HI_U32 woIter = 0;
    HI_U32 bitsPkts = 1;
    
    UINT8  MxL68xDeviceId = DEFAULT_MXL68X_ID;
    
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
	
    status = MxLWare68x_API_ReqDemodBER(MxL68xDeviceId, &berA, &berB, &berC);
    if (MXL_SUCCESS != status)
    {
	   HI_ERR_TUNER("mxl68x_get_ber error!\n");
       return HI_FAILURE;
    }
    
    wIter = berA.bitErrCount_W_Iter;
    woIter = berA.bitErrCount_WO_Iter;
    bitsPkts = berA.numOfBits;

    if(wIter != 0)
    {
        pu32ber[0] = (HI_U32)(wIter* 10000000/bitsPkts);
        pu32ber[1] = 0;
        pu32ber[2] = 7;
    }
    else
    
    {
        pu32ber[0] = 0;
        pu32ber[1] = 0;
        pu32ber[2] = 0;
    }
    
    HI_INFO_TUNER("Layer A : BER %dx10e%d \n",pu32ber[0],pu32ber[2]);
#if 0  //TODO:  multi layer
    wIter = berB.bitErrCount_W_Iter;
    woIter = berB.bitErrCount_WO_Iter;
    bitsPkts = berA.numOfBits;
    HI_INFO_TUNER("Layer B : BER - (with Iterative) %5.2e \t (without Iterative) %5.2e \n",
           (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));
    
    wIter = berC.bitErrCount_W_Iter;
    woIter = berC.bitErrCount_WO_Iter;
    bitsPkts = berA.numOfBits;
    HI_INFO_TUNER("Layer C : BER - (with Iterative) %5.2e \t (without Iterative) %5.2e \n",
            (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));
#endif
    
	return HI_SUCCESS;
}


HI_S32 mxl68x_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
#if 1
	MXL_STATUS_E status;
    UINT8  MxL68xDeviceId = DEFAULT_MXL68X_ID;
    MXL68x_MPEG_MODE_E mpegMode;
    MXL68x_MPEG_CLK_FREQ_E mpegClkFreq;
    MXL68x_MPEG_DATA_FMT_E mpegDataFmt;
    MXL68x_MPEG_CLK_PHASE_E mpegClkPhase;    
    MXL68x_MPEG_POLARITY_E mpegSyncPol;
    MXL68x_MPEG_POLARITY_E mpegValidPol;
    MXL68x_MPEG_SYNC_PULSE_E mpegSyncPulseWidth;
    MXL68x_MPEG_ERROR_IND_E mpegErrIndicator;
    MXL68x_TS_PKT_SIZE_E tsPktSize;

    status = MxLWare68x_API_ReqDemodMpegOutSet(MxL68xDeviceId,
	                                  &mpegMode,
                                      &mpegClkFreq,
                                      &mpegDataFmt,
                                      &mpegClkPhase,    
                                      &mpegSyncPol,
                                      &mpegValidPol,
                                      &mpegSyncPulseWidth,
                                      &mpegErrIndicator,
                                      &tsPktSize);

	if( HI_UNF_TUNER_OUTPUT_MODE_FULLBAND_SERIAL_3WIRE == enTsType )
	{
        mpegMode = MXL68x_MPEG_MODE_3WIRE; // MCLK, MSYNC & MDAT
	}
    else
    {
        mpegMode = MXL68x_MPEG_MODE_NORMAL; // MERR, MVAL, MSYNC, MCLK & MDAT
    }

    mpegClkFreq = MXL68x_MPEG_CLK_64MHZ;
    //mpegClkPhase = MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG;
    tsPktSize = MXL68x_TS_PKT_188_BYTES;

    ///*
    HI_INFO_TUNER("MpegOut--mpegMode:%d,mpegClkFreq:%d,mpegDataFmt:%d,mpegClkPhase:%d,mpegSyncPol:%d,"
                  "mpegValidPol:%d,mpegSyncPulseWidth:%d,mpegErrIndicator:%d,tsPktSize:%d \n",
                            mpegMode,mpegClkFreq,mpegDataFmt,mpegClkPhase,mpegSyncPol,
                            mpegValidPol,mpegSyncPulseWidth,mpegErrIndicator,tsPktSize);
    //*/
	status = MxLWare68x_API_CfgDemodMPEGOut(MxL68xDeviceId,
	                                        mpegMode,
                                            mpegClkFreq,
                                            mpegDataFmt,
                                            mpegClkPhase,    
                                            mpegSyncPol,
                                            mpegValidPol,
                                            mpegSyncPulseWidth,
                                            mpegErrIndicator,
                                            tsPktSize);
                    
	if (status != MXL_SUCCESS)
	{
    	HI_ERR_TUNER("DeviceId[%d] MxLWare68x_API_CfgDemodMPEGOut FAILED\n", MxL68xDeviceId);
    	return status;
	}
#endif
	return HI_SUCCESS; 
}

HI_VOID mxl68x_connect_timeout(HI_U32 u32ConnectTimeout)
{
    s_u32mxl68xConnectTimeout = u32ConnectTimeout;
    return;
}

HI_S32 mxl68x_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
	return HI_SUCCESS;
}




