#define DEFAULT_MXL68X_ID          0

#include "stdio.h"
#include "string.h"
#include "MaxLinearDataTypes.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhyCfg.h"

MXL_STATUS_E downloadFirmware(UINT8 DEV_ID);

void main()
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 MxL68xDeviceId;
/*
  REAL64 wIter = 0;
  REAL64 woIter = 0;
  REAL64 bitsPkts = 1;

  REAL64 cnr;
  
*/
  SINT32 cnrInt = 0;
  UINT32 cnrRaw = 0;
  MXL_BOOL_E rfSynthLock, refSynthLock;
  MXL_BOOL_E rsLockA, rsLockB, rsLockC, tmccLock;

  MXL68x_BER_T berA, berB, berC;
  MXL68x_PER_T perA, perB, perC;

  MXL_BOOL_E ewsAlarm;
  MXL68x_GUARD_INTERVAL_E gi;
  MXL68x_ISDBT_MODE_E isdbtMode;
  MXL68x_TMCC_INFO_T tmccA, tmccB, tmccC;
  MXL68x_FW_STATUS_E firmwareDownloadStatus;

  MxL68xDeviceId = DEFAULT_MXL68X_ID;

   // Init driver context
  status = MxLWare68x_API_CfgDrvInit(MxL68xDeviceId, NULL);

  if (status != MXL_SUCCESS)
    printf ("Err : MxLWare68x_API_CfgDrvInit %d\n", status);

  // Config Power Supply
  status = MxLWare68x_API_CfgDevPowerSupply(MxL68xDeviceId,
                                            MXL68x_SINGLE_SUPPLY);
  if (status != MXL_SUCCESS)
    printf ("Err : MxLWare68x_API_CfgDevPowerSupply %d\n", status);

  // Config XTAL
  status = MxLWare68x_API_CfgDevXtal(MxL68xDeviceId,
                                     MXL68x_XTAL_16MHz,   // 16 MHz
                                     25,                          // 25 pF
	                         MXL_ENABLE,        // Enable CLK out
	                         MXL_FALSE,           // Disable CLK DIV
	                         MXL_FALSE,          // Disable XTAL Sharing
							 MXL_ENABLE);          // Enable XTAL Cal for Master, Disable for Slave
  if (status != MXL_SUCCESS)
    printf ("Err : MxLWare68x_API_CfgDevXtal %d\n", status);

  // Config Dev Init, switch clock to PLL clock
  status = MxLWare68x_API_CfgDevInit(MxL68xDeviceId,
                                     MXL68x_IF_NA,          // Do not set IF mux
                                     MXL_FALSE);            // Disable RF Loop-Thru
  if (status != MXL_SUCCESS)
    printf ("Err : MxLWare68x_API_CfgDevInit %d\n", status);

  firmwareDownloadStatus = Ctrl_CheckFirmwareStatus(MxL68xDeviceId);
  if (firmwareDownloadStatus != MXL68x_FW_STATUS_LOADED)
  {
    // Download firmware
    status = downloadFirmware(MxL68xDeviceId);
    if (status != MXL_SUCCESS)
      printf ("Err : MxLWare68x_API_CfgDevFirmwareDownload %d\n", status);
    else
    {
       printf(" Download firmware success, Press any key to continue ...");
       getchar();
    }

    MxLWare68x_OEM_SleepInUs(100000); // wait for 100 ms
  }
  else printf(" Firmware has been downloaded. \n");

  // Change Device Operational Mode to Tuner+Demod
  status = MxLWare68x_API_CfgDevOperationalMode(MxL68xDeviceId, MXL68x_TUNER_DEMOD);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_CfgDevOperationalMode %d\n", status);

  // Config IF Mux and RF loop through
  status = MxLWare68x_API_CfgDevInit(MxL68xDeviceId,
                                     MXL68x_IF_INTERNAL,    // Initernal IF
                                     MXL_FALSE);            // Disable RF Loop-Thru
  if (status != MXL_SUCCESS)
    printf ("Err : MxLWare68x_API_CfgDevInit %d\n", status);

  // Set or Change Channel
  status = MxLWare68x_API_CfgTunerChanTune(MxL68xDeviceId,
				   	                       MXL68x_VIEW_MODE,
                               MXL68x_BW_6MHz,
                               473143000,
                               MXL68x_AUTO_SPEC_INV_DISABLE,
							   MXL_FALSE);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_CfgTunerChanTune %d\n", status);

  // Check Tuner lock status
  status = MxLWare68x_API_ReqTunerLockStatus(MxL68xDeviceId, &rfSynthLock, &refSynthLock);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_ReqTunerLockStatus %d\n", status);

  printf ("***** Tuner Lock status: RF Synth Lock = %d, REF Synth Lock = %d \n", rfSynthLock, refSynthLock);

  // Wait for 1 seconds
  MxLWare68x_OEM_SleepInUs(1000000);

  // Check Demod lock status
  status = MxLWare68x_API_ReqDemodLockStatus(MxL68xDeviceId, &tmccLock, &rsLockA, &rsLockB, &rsLockC);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_ReqDemodLockStatus %d\n", status);

  printf ("***** Demod Lock status: Frame lock = %d, Layer_A RS = %d, Layer_B RS = %d, Layer_C RS = %d \n", tmccLock, rsLockA, rsLockB, rsLockC);

  // Inquiring TMCC information
  status = MxLWare68x_API_ReqDemodTMCCParams(MxL68xDeviceId, &ewsAlarm, &gi, &isdbtMode, &tmccA, &tmccB, &tmccC);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_ReqDemodLockStatus %d\n", status);

  printf ("***** TMCC info:  ewsAlarm %d, isdbtMode %d, gi %d\n", ewsAlarm, isdbtMode, gi);

  printf("Layer A: Mod %d, CR %d, IL %d, Segs %d\n",tmccA.modulationInfo,tmccA.codeRateInfo,tmccA.interleaverLenInfo,tmccA.numOfSegments);
  printf("Layer B: Mod %d, CR %d, IL %d, Segs %d\n",tmccB.modulationInfo,tmccB.codeRateInfo,tmccB.interleaverLenInfo,tmccB.numOfSegments);
  printf("Layer C: Mod %d, CR %d, IL %d, Segs %d\n",tmccC.modulationInfo,tmccC.codeRateInfo,tmccC.interleaverLenInfo,tmccC.numOfSegments);

  // Configure the number of frames to use for BER calculation
  status = MxLWare68x_API_CfgDemodBerPer(MxL68xDeviceId, 4);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_CfgDemodBerPer %d\n", status);

  // Wait for 1 seconds
  MxLWare68x_OEM_SleepInUs(1000000);

  // Get Demod CNR value
  status = MxLWare68x_API_ReqDemodCNR(MxL68xDeviceId, &cnrRaw, &cnrInt);
  if (status != MXL_SUCCESS)
    printf ("\nErr: MxLWare68x_API_ReqDemodCNR %d\n", status);
  else
  {
/*  
    cnr = (REAL64)cnrInt/1000.0;
    printf("CNR: %f dB\n", cnr);
*/	
  }

  // Retrive BER
  status = MxLWare68x_API_ReqDemodBER(MxL68xDeviceId, &berA, &berB, &berC);
/*
  wIter = berA.bitErrCount_W_Iter;
  woIter = berA.bitErrCount_WO_Iter;
  bitsPkts = berA.numOfBits;
  printf("Layer A : BER - (with Iterative) %5.2e \t (without Iterative) %5.2e \n",
         (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));

  wIter = berB.bitErrCount_W_Iter;
  woIter = berB.bitErrCount_WO_Iter;
  bitsPkts = berA.numOfBits;
  printf("Layer B : BER - (with Iterative) %5.2e \t (without Iterative) %5.2e \n",
         (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));

  wIter = berC.bitErrCount_W_Iter;
  woIter = berC.bitErrCount_WO_Iter;
  bitsPkts = berA.numOfBits;
  printf("Layer C : BER - (with Iterative) %5.2e \t (without Iterative) %5.2e \n",
          (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));

  // Retrieve PER
  status = MxLWare68x_API_ReqDemodPER(MxL68xDeviceId, &perA, &perB, &perC);

  wIter = perA.pktErrCount_W_Iter;
  woIter = perA.pktErrCount_WO_Iter;
  bitsPkts = perA.numOfPkts;
  printf("Layer A : PER - (with Iterative) = %5.2e \t (without Iterative) %5.2e \n",
          (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));

  wIter = perB.pktErrCount_W_Iter;
  woIter = perB.pktErrCount_WO_Iter;
  bitsPkts = perB.numOfPkts;
  printf("Layer B : PER - (with Iterative) = %5.2e \t (without Iterative) %5.2e \n",
          (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));

  wIter = perB.pktErrCount_W_Iter;
  woIter = perB.pktErrCount_WO_Iter;
  bitsPkts = perB.numOfPkts;
  printf("Layer C : PER - (with Iterative) = %5.2e \t (without Iterative) %5.2e \n",
          (REAL32)(wIter/bitsPkts), (REAL32)(woIter/bitsPkts));
		  
*/

}

MXL_STATUS_E downloadFirmware(UINT8 DEV_ID)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT32 fwBuffSize;
  UINT8 *fwBuffPtr = NULL;
  long fileSize = 0;
  FILE * fw;
/*
Download the firmware by either opening the file and reading the contents as shown in the demo code here,
or by including the header files provided in the source code release, as shown below:

	const unsigned char firmware[] =
	{ 
	#ifdef MXL68X_FIRMWARE_V1
	#include "MxL68x_FW_V1.h"
	#else
	#include "MxL68x_FW_V1_200.h"
	#endif 
	};

	fwBuffSize = sizeof(firmware);
	fwBuffPtr = firmware;

MxL68x_FW_V1.h contains FW for V1 version of the chip and 
MxL68x_FW_V1_200.h contains FW for the V1_200 version of the chip
*/

  fw = fopen(".\\Firmware\\ISDBT_FW_RAM.mbin", "rb");

  fseek(fw, 0, SEEK_END);
  fileSize = ftell(fw);
  fseek(fw, 0, SEEK_SET);

  if (fileSize)
  {
    fwBuffSize = fileSize;

    fwBuffPtr = (UINT8 *)malloc(1 + (sizeof(UINT8) * fwBuffSize));
    fileSize = fread(fwBuffPtr, sizeof(UINT8), fwBuffSize, fw);

    if (fileSize == (long)fwBuffSize)
    {
      status = MxLWare68x_API_CfgDevFirmwareDownload(DEV_ID,
                                                     (UINT32)fwBuffSize,
                                                     fwBuffPtr);

      if (status != MXL_SUCCESS)
        printf ("Err : Firmware Download %d\n", status);
    }
    else
    {
      printf ("Err : File Read Size = %d FW size = %d \n", fileSize, fwBuffSize);
    }
  }
  else
  {
    printf ("Err : FW file size %d\n", fileSize);
  }

  if (fw) fclose(fw);
  if (fwBuffPtr) free (fwBuffPtr);

  return status;
}
