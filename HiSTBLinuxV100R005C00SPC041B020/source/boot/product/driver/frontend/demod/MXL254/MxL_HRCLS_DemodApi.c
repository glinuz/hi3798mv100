/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_DemodApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS demod APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

static REG_CTRL_INFO_T MxL_HRCLS_DemodEqualizerSpeedUp[] = 
{
  {0x8084, 0xFFFF, 0xD566},
  {0x8140, 0xFFFF, 0x083A},
  {0x80A0, 0xFFFF, 0x8508},
  {0x80D4, 0xFFFF, 0x4179},
  {0,    0,    0}
};

static INTERLEAVER_LOOKUP_INFO_T MxL_HRCLS_InterDepthLookUpTable[] =
{
  {128, 1}, {128, 1}, {128, 2}, {64,  2}, {128, 3}, {32,  4}, {128, 4}, {16,  8},
  {128, 5}, {8,  16}, {128, 6}, {0,   0}, {128, 7}, {0,   0}, {128, 8}, {0,   0}
};

static BURST_NOISE_LOOKUP_T MxL_HRCLS_BurstNoiseLookup[] = 
{
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  8,   16, 35},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  16,  8,  70},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  32,  4,  140},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  64,  2,  280},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  128, 1,  560},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  128, 2,  1023},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM64,  128, 4,  1023},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 8,   16, 25},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 16,  8,  50},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 32,  4,  100},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 64,  2,  200},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 128, 1,  400},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 128, 2,  800},
  {MXL_HRCLS_ANNEX_B,  MXL_HRCLS_QAM256, 128, 4,  1023},  
  {MXL_HRCLS_ANNEX_A,  MXL_HRCLS_QAM64,  12,  17, 96},  
  {MXL_HRCLS_ANNEX_A,  MXL_HRCLS_QAM256, 12,  17, 72},  
  {(MXL_HRCLS_ANNEX_TYPE_E)0, (MXL_HRCLS_QAM_TYPE_E)0, 0, 0, 0}
};

typedef struct
{
  UINT16 x2;

  SINT32 a;
  SINT32 b;
} MxL_HRCLS_SNR_RANGE_T;

static MxL_HRCLS_SNR_RANGE_T snrRangeQam64[] = {
  {8, -75257, 4633548},
  {19, -32451, 4272396},
  {42, -14646, 3926475},
  {90, -6824, 3594585},
  {191, -3219, 3268550},
  {404, -1523, 2943878},
  {854, -721, 2619375},
  {1803, -341, 2294854},
  {3804, -162, 1970594},
  {8024, -76, 1646463},
  {16925, -36, 1322355},
  {34243, -17, 1005132},  
  {0,0,0}
  };

static MxL_HRCLS_SNR_RANGE_T snrRangeQam16[] = {
  {8, -75257, 4612359},
  {19, -32451, 4251207},
  {42, -14646, 3905286},
  {90, -6824, 3573395},
  {191, -3219, 3247361},
  {404, -1523, 2922689},
  {854, -721, 2598186},
  {1803, -341, 2273666},
  {3804, -162, 1949404},
  {8024, -76, 1625274},
  {16925, -36, 1301166},
  {32612, -18, 992174},
  {0,0,0}
  };

static MxL_HRCLS_SNR_RANGE_T snrRangeQam256[] = {
  {8, -75257, 4638688},
  {19, -32451, 4277536},
  {42, -14646, 3931615},
  {90, -6824, 3599724},
  {191, -3219, 3273690},
  {404, -1523, 2949018},
  {854, -721, 2624516},
  {1803, -341, 2299994},
  {3804, -162, 1975733},
  {8024, -76, 1651603},
  {16925, -36, 1327495},
  {34651, -17, 1008299},
  {0,0,0}
  };

static MxL_HRCLS_SNR_RANGE_T snrRangeQam32[] = {
  {4, -150514, 4612359},
  {10, -60205, 4214419},
  {23, -26694, 3864607},
  {50, -12259, 3526387},
  {107, -5746, 3197869},
  {227, -2710, 2871717},
  {480, -1282, 2546940},
  {1014, -607, 2222508},
  {2140, -287, 1898166},
  {4515, -136, 1573980},
  {9524, -64, 1249834},
  {16306, -34, 961458},
  {0,0,0}
  };

static MxL_HRCLS_SNR_RANGE_T snrRangeQam128[] = {
  {4, -150515, 4623084},
  {10, -60205, 4225143},
  {23, -26694, 3875331},
  {50, -12259, 3537111},
  {107, -5746, 3208594},
  {227, -2710, 2882442},
  {480, -1282, 2557664},
  {1014, -607, 2233232},
  {2140, -287, 1908890},
  {4515, -136, 1584704},
  {9524, -64, 1260558},
  {16714, -33, 967789},
  {0,0,0},
  };

static MxL_HRCLS_SNR_RANGE_T snrRangeQpsk[] = {
  {14, -43004, 4624594},
  {31, -19704, 4288137},
  {67, -9169, 3956929},
  {143, -4304, 3628849},
  {303, -2031, 3302900},
  {641, -961, 2978047},
  {1354, -455, 2653349},
  {2857, -215, 2328987},
  {6027, -102, 2004838},
  {12713, -48, 1680714},
  {26814, -22, 1356603},
  {56554, -10, 1032505},
  {58703, -7, 842441},
  {0,0,0}
  };  

#define MXL_HRCLS_DEFAULT_FAKE_ANNEX_TYPE ((MXL_HRCLS_ANNEX_TYPE_E) ((UINT8) -1))


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgDemReset
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reset demodulators (hw and sw)
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgDemReset(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
    MXL_HRCLS_DMD_ID_E demodId)
{
  UINT8 status = MXL_SUCCESS;

  devContextPtr->demods[(UINT8) demodId].curQamType = MXL_HRCLS_QPSK;
  devContextPtr->demods[(UINT8) demodId].curAnnexType = MXL_HRCLS_DEFAULT_FAKE_ANNEX_TYPE; 
  devContextPtr->demods[(UINT8) demodId].enabled = MXL_FALSE; 
  devContextPtr->demods[(UINT8) demodId].demodFreqRangeIdx = MXL_HRCLS_DEFAULT_DEMOD_FREQ_SEARCH_RANGE_IDX;

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConvertDemodIdToAddr
--|
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24
--|
--| DESCRIPTION   : This function converts to demod address based on demod ID
--|
--| RETURN VALUE  : UINT16 
--|
--|-------------------------------------------------------------------------------------*/

static UINT16 MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(MXL_HRCLS_DMD_ID_E DemodId, UINT16 Addr)
{
  UINT16 offset = (Addr & 0x07FF);
  return (0x1800 + offset + 0x800*DemodId);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/16/2012
--|
--| DESCRIPTION   : This function converts logical demod number to physical
--|
--| INPUTS        : Logical demod number 
--|
--| RETURN VALUE  : Physical demod number 
--|
--|-------------------------------------------------------------------------------------*/
MXL_HRCLS_DMD_ID_E MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E logicalDemodId)
{
  MXL_HRCLS_DMD_ID_E physicalDemodId;

  MXLENTERSTR;
  physicalDemodId = devContextPtr->demodsMap[(UINT8) logicalDemodId];
  MXLEXIT(MxL_HRCLS_DEBUG("logical demod%d -> physical demod%d\n", logicalDemodId, physicalDemodId););
  MXLEXITSTR(physicalDemodId);
  return physicalDemodId;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads current annex type and qam
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
                                  MXL_HRCLS_DMD_ID_E demodId, 
                                  /*@null@*/ /*@out@*/ MXL_HRCLS_ANNEX_TYPE_E * annexPtr,
                                  /*@null@*/ /*@out@*/ MXL_HRCLS_QAM_TYPE_E * qamPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 annex_value = 0;
  UINT16 qam_value = 0;
  
  if (annexPtr)
  {
    if (devContextPtr->demods[(UINT8) demodId].curAnnexType == MXL_HRCLS_DEFAULT_FAKE_ANNEX_TYPE)
    {
      status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devContextPtr->devId, demodId, DMD0_QAM_ANNEX_TYPE, &annex_value);
      *annexPtr = (annex_value == MXL_HRCLS_HW_ANNEX_A)?MXL_HRCLS_ANNEX_A:MXL_HRCLS_ANNEX_B; 
      devContextPtr->demods[(UINT8) demodId].curAnnexType = *annexPtr;
    } else *annexPtr = devContextPtr->demods[(UINT8) demodId].curAnnexType;
  }
  if (qamPtr)
  {
    status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devContextPtr->devId, demodId, DMD0_QAM_TYPE, &qam_value);

    *qamPtr = (qam_value == MXL_HRCLS_HW_16QAM)?MXL_HRCLS_QAM16:
              (qam_value == MXL_HRCLS_HW_64QAM)?MXL_HRCLS_QAM64:
              (qam_value == MXL_HRCLS_HW_256QAM)?MXL_HRCLS_QAM256:
              (qam_value == MXL_HRCLS_HW_1024QAM)?MXL_HRCLS_QAM1024:
              (qam_value == MXL_HRCLS_HW_32QAM)?MXL_HRCLS_QAM32:
              (qam_value == MXL_HRCLS_HW_128QAM)?MXL_HRCLS_QAM128:MXL_HRCLS_QPSK;
  }
  MXLDBG3(MxL_HRCLS_PRINT("[HRCLS][%d][%d] %s. ANNEX=%d, QAM=%d -> status=%d\n",
                  devContextPtr->devId, demodId, __FUNCTION__, (annexPtr)?(*annexPtr):-1, (qamPtr)?(*qamPtr):-1, status););
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDemodCurrentResampleRate 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 4/18/2012
--|                  
--| DESCRIPTION   : Reads current resample rate. If the rate is unknown (demod was locked
--|                 by other host) the value is read from registers. Otherwise it's 
--|                 taken from context memory. 
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error, MXL_NOT_READY if not locked
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDemodCurrentResampleRate(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
                                  MXL_HRCLS_DMD_ID_E demodId, /*@out@*/ UINT32 * resampleRatePtr)
{
  MXL_STATUS_E status;
  MXL_HRCLS_ANNEX_TYPE_E    annexType;  
  UINT8 bankCfgData = 0;

  status = MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, &annexType, NULL);
      
  if (status == MXL_SUCCESS)
  {
    switch(annexType)
    {
      case MXL_HRCLS_ANNEX_B:
      {
        MXL_HRCLS_QAM_TYPE_E      qamType;
        status = MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, NULL, &qamType);
        switch(qamType)
        {
          case MXL_HRCLS_QAM64:
            bankCfgData = 1;
            break;

          case MXL_HRCLS_QAM256:
            bankCfgData = 2;
            break;

          default:
            status = MXL_NOT_READY;
            break;
        }
        break;
      }
      case MXL_HRCLS_ANNEX_A:
        bankCfgData = 0;
        break;
      default:
        status = MXL_NOT_READY;
        break;  
    }
    if (status == MXL_SUCCESS)
    {
      if (devContextPtr->demods[(UINT8) demodId].curResampleRate[bankCfgData] == 0)
      {
        status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devContextPtr->devId, demodId, DMD0_RESAMP_REGISTERS_BANK, bankCfgData);
        if (status == MXL_SUCCESS)
        {
          UINT16 readDataH, readDataL;

          if ((MxL_HRCLS_Ctrl_ReadDemodRegister(devContextPtr->devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_RATE_RATIO_LOW), &readDataL) == MXL_SUCCESS)
            && (MxL_HRCLS_Ctrl_ReadDemodRegister(devContextPtr->devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_RATE_RATIO_HIGH), &readDataH) == MXL_SUCCESS))
          {
            *resampleRatePtr = (UINT32) readDataL | (UINT32) (readDataH << 16);
          }
        }
      } else *resampleRatePtr = devContextPtr->demods[(UINT8) demodId].curResampleRate[bankCfgData];
    } 
  }

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_UpdateDemodRegisterField
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads and updates field belonging to specified demodulator
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateDemodRegisterField(UINT8 devId, 
                                                   MXL_HRCLS_DMD_ID_E demodId, 
                                                   UINT16 regAddr, 
                                                   UINT8 lsbPos, 
                                                   UINT8 fieldWidth, 
                                                   UINT16 newValue) 
{
  UINT8 status = MXL_SUCCESS;

  status = (UINT8) MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, regAddr), lsbPos, fieldWidth, newValue);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_WriteDemodRegister
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Writes values to register belonging to specified demodulator
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_WriteDemodRegister(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, UINT16 newValue)
{
  UINT8 status = MXL_SUCCESS;

  status = (UINT8) MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, regAddr), newValue);

  return (MXL_STATUS_E)status;
}  

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadDemodRegiste
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Read register belonging to specified demodulator
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadDemodRegister(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, UINT16 * Value)
{
  UINT8 status = MXL_SUCCESS;

  status = (UINT8) MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, regAddr), Value);

  return (MXL_STATUS_E)status;
}  

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConfigDemodResampRatio
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 09/10/2011
--|
--| DESCRIPTION   : Config demod resample ratio 
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodResampRatio(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT32 symbolRate[], UINT8 bank[], UINT8 numBank)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 i;
  UINT32 resampleRateRatio;
  UINT16 resampleRegCtrl = 0;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  for (i = 0; i < numBank; i++)
  {
    /* Read the current settings. */
    status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_REGISTERS_BANK), &resampleRegCtrl); 
    resampleRegCtrl &= 0xFFF8;

    /* Select Bank */
    resampleRegCtrl |= bank[i]; 

    if (symbolRate[i] <= 0)
    {
      MXLERR(MxL_HRCLS_ERROR("%s: symbolRate[i] is 0\n", __FUNCTION__););
      return MXL_FAILURE;
    }

//    resampleRateRatio = ((UINT64)(devContextPtr->adcSampRateInHz)*(UINT64)(1<<24)*3)/(256ULL*4*(UINT64)symbolRate[i]);
    resampleRateRatio = MXL_DIV_ROUND((UINT64) ((UINT64) devContextPtr->adcSampRateInHz * (UINT64) (1<<14) * (UINT64) 3), symbolRate[i]);

    if (resampleRateRatio <= 3*(1<<24))
    {
      /* Interpolator X3 should not be bypassed  */
      resampleRegCtrl &= (UINT16) (~BYPASS_INTERPOLATOR);
    }
    else
    {
      /* Interpolator X3 should be bypassed  */
      resampleRegCtrl |= (UINT16) BYPASS_INTERPOLATOR;
      resampleRateRatio /= 3;
    }
    devContextPtr->demods[(UINT8) demodId].curBypassInterpolator = (resampleRegCtrl & BYPASS_INTERPOLATOR)?MXL_TRUE:MXL_FALSE;

    /* Configure Resample_register bank and Interpolator bypass control  */
    status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_REGISTERS_BANK), resampleRegCtrl); 

    /* Configure Resample Rate Ratio register */
    status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_RATE_RATIO_LOW), resampleRateRatio & 0xFFFF);
    status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_RESAMP_RATE_RATIO_HIGH), (resampleRateRatio >> 16 ) & 0xFFFF);

    
    if (MXL_SUCCESS == status)
    {
      if (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED)
      {
        HOST_COMMAND_T demodResampleRateRatioCfg;

        /* Form command payload */
        demodResampleRateRatioCfg.data[0]    = (UINT8)demodId;
        demodResampleRateRatioCfg.data[1]    = (UINT8) ((resampleRateRatio      )& 0xFF);
        demodResampleRateRatioCfg.data[2]    = (UINT8) ((resampleRateRatio >>  8)& 0xFF);
        demodResampleRateRatioCfg.data[3]    = (UINT8) ((resampleRateRatio >> 16)& 0xFF);
        demodResampleRateRatioCfg.data[4]    = (UINT8) ((resampleRateRatio >> 24)& 0xFF);
        demodResampleRateRatioCfg.payloadLen = 5;
        demodResampleRateRatioCfg.syncChar = 0;
        demodResampleRateRatioCfg.commandId = 0;
        demodResampleRateRatioCfg.payloadCheckSum = 0;

        /* Send host command */
        status |= MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodResampleRateRatioCfg, HOST_CMD_CFG_DEMOD_RESAMPLE_RATE_RATIO, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      }
    }


    devContextPtr->demods[(UINT8) demodId].curResampleRate[bank[i]] = resampleRateRatio;
  }
  return (MXL_STATUS_E) status;
}

/*---------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetMCNSSD
--| 
--| AUTHOR        : Brenndon Lee
--|                 Richard Liu
--|
--| DATE CREATED  : 1/24/2008
--|                 12/21/2010
--|
--| DESCRIPTION   : Read back Counter registers for BER, CER calculation.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_GetMCNSSD(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT8 regCode, UINT32 *dataPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT32 tmp = 0;
  UINT16 lsb = 0;
  UINT16 msb = 0;

  switch (regCode)
  {
    case MXL_HRCLS_CW_CORR_COUNT_SEL:
    case MXL_HRCLS_CW_ERR_COUNT_SEL:
    case MXL_HRCLS_CW_COUNT_SEL:
      /*  Get LSB */
      status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_FECA_MCNSSD_SEL), (UINT16) (regCode * 2));
      status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_FECA_MCNSSD, &lsb);
      /* Get MSB */
	  status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_FECA_MCNSSD_SEL), (UINT16) ((regCode * 2) + 1));
      status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_FECA_MCNSSD, &msb);
      if (status == MXL_SUCCESS)
      {
        tmp = msb << 16;
        tmp |= lsb;
      }
      break;

    case MXL_HRCLS_CORR_BITS_SEL:
 
      status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_FECA_NCBH), &msb);
      status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_FECA_NCBL), &lsb);	  
      if (status == MXL_SUCCESS)
      {
        tmp = msb << 16;
        tmp |= lsb;
      }        
      break;
    
    default:
      status = MXL_INVALID_PARAMETER;
      break;
  }

  *dataPtr = tmp;

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConfigDemodClockEnable
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 12/08/2010
--|
--| DESCRIPTION   : Config demod clocks
--|
--| RETURN VALUE  : MXL_SUCCESS for success or MXL_FAILURE for failed
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodClockEnable(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E demodId, MXL_BOOL_E enable)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regAddr;
  UINT16 regData;
  UINT16 mask;

  regAddr = EXTRACT_ADDR(DMD0_MCLK_EN);
  regAddr += (4 * (UINT8) demodId);

  status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, regAddr, &regData);
  
  if (status == MXL_SUCCESS)
  {
    mask = (UINT16) (1 << EXTRACT_LSBLOC(DMD0_MCLK_EN));
    mask |= (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X1_EN));
    mask |= (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X2_EN));
    mask |= (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X4_EN));
    if (enable == MXL_TRUE)
    {
      regData |= mask;
    } 
    else
    {
      regData &= ~(mask);
    }
    status = MxLWare_HRCLS_OEM_WriteRegister(devContextPtr->devId, regAddr, regData);

    if (status == MXL_SUCCESS)
    {
#if 0      
      if ((status == MXL_SUCCESS) && (enable == MXL_ENABLE))
      {
        status = MxL_HRCLS_Ctrl_WriteDemodRegister(devContextPtr->devId, demodId, EXTRACT_ADDR(DMD0_RESTART), 0xFFFF);
      }
#endif      
    }
  }
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConvertDemodInterruptMask
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 06/21/2012
--|
--| DESCRIPTION   : Convert logical demodulator id to physical demodulator 
--|
--| RETURN VALUE  : Physical demod id 
--|
--|-------------------------------------------------------------------------------------*/
UINT32 MxL_HRCLS_Ctrl_ConvertDemod2PhysicalInterruptMask(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT32 intrMask)
{
  UINT32 newMask = intrMask;
  UINT8 i;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("logicalDemodMask=%x\n", intrMask););
  newMask &= ~(MXL_HRCLS_DEMOD_INTERRUPTS_MASK);
             
  if (intrMask & MXL_HRCLS_DEMOD_INTERRUPTS_MASK)
  {
    for (i = 0; i < devContextPtr->demodsCnt; i++)
    {
      UINT32 logDemodMask, phyDemodMask;

      logDemodMask = (MXL_HRCLS_INTR_MASK_DMD0) << i;
      phyDemodMask = (MXL_HRCLS_INTR_MASK_DMD0) << devContextPtr->demodsMap[i];

      if (intrMask & logDemodMask) newMask |= phyDemodMask;
    }
  }
  MXLEXITSTR(newMask);

  return newMask;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConvertDemod2LogicalInterruptMask
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 06/21/2012
--|
--| DESCRIPTION   : Convert physical demodulator id to logical demodulator id
--|
--| RETURN VALUE  : Logical demod id 
--|
--|-------------------------------------------------------------------------------------*/
UINT32 MxL_HRCLS_Ctrl_ConvertDemod2LogicalInterruptMask(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT32 intrMask)
{
  UINT32 newMask = intrMask;
  UINT8 i;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("physicalDemodMask=%x\n", intrMask););
  
  newMask &= ~(MXL_HRCLS_DEMOD_INTERRUPTS_MASK);

  if (intrMask & MXL_HRCLS_DEMOD_INTERRUPTS_MASK)
  {
    for (i = 0; i < MXL_HRCLS_MAX_NUM_DEMOD; i++)
    {
      UINT32 phyDemodMask;

      phyDemodMask = (MXL_HRCLS_INTR_MASK_DMD0) << i;
      if (intrMask & phyDemodMask)
      { 
        UINT8 o;

        for (o = 0; (o < devContextPtr->demodsCnt) && (devContextPtr->demodsMap[o] != i); o++) continue;
        if (devContextPtr->demodsMap[o] == i) newMask |= (MXL_HRCLS_INTR_MASK_DMD0 << o);
      }
    }
  }
  
  MXLEXITSTR(newMask);

  return newMask;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDemodClockEnable
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 12/08/2010
--|
--| DESCRIPTION   : Get demod clock enable configuration
--|
--| RETURN VALUE  : MXL_SUCCESS for success or MXL_FAILURE for failed
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDemodClockEnable(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, MXL_BOOL_E* enablePtr)
{
  UINT8 status = MXL_FAILURE;
  UINT16 readReg;
  UINT16 regAddr;

  regAddr = EXTRACT_ADDR(DMD0_CLOCK_X1_EN);
  regAddr += (4 * (UINT8) demodId);
  status = MxLWare_HRCLS_OEM_ReadRegister(devId, regAddr, &readReg);
  if (status == MXL_SUCCESS)
  {
    UINT16 mask;
    mask = (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X1_EN)) | (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X2_EN)) | (UINT16) (1 << EXTRACT_LSBLOC(DMD0_CLOCK_X4_EN));

    *enablePtr = ((readReg & mask) == mask)?MXL_TRUE:MXL_FALSE;
  }

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConfigDemodEqualizer
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 06/20/2011
--|
--| DESCRIPTION   : Config Demod equalizer
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodEqualizer(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId)
{
  UINT8 status = MXL_SUCCESS;
  status |= MxL_HRCLS_Ctrl_ProgramRegisters(devId, demodId, MxL_HRCLS_DemodEqualizerSpeedUp);
//  status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EQU_FREQ_SWEEP_LIMIT, 0xD);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SetMpegOutEnable
--|
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/14/2011
--|
--| DESCRIPTION   : Set MPEG Output Enable
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_SetMpegOutEnable(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E demodId, MXL_BOOL_E mpegOutEnable)
{
  UINT8 status = MXL_SUCCESS;
  const MXL_HRCLS_FIELD_T mpeg_oe[] = {{MPEG0_OE}, {MPEG1_OE},
                                 {MPEG2_OE}, {MPEG3_OE},
                                 {MPEG4_OE}, {MPEG5_OE},
                                 {MPEG6_OE}, {MPEG7_OE}, 
                                 {MPEG8_OE}};
  if (mpegOutEnable == MXL_ENABLE)
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, MCK_OE, 1);
  }    
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, mpeg_oe[(UINT8) demodId].regAddr,
                                                      mpeg_oe[(UINT8) demodId].lsbPos,
                                                      mpeg_oe[(UINT8) demodId].fieldWidth,
                                                      (mpegOutEnable == MXL_ENABLE)?1:0);
  MXLDBG2(MxL_HRCLS_PRINT("[HRCLS][%d][%d] SetMpegOutEnable. MPEG=%d -> status=%d\n",
                  devContextPtr->devId, demodId, mpegOutEnable,  status););

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SetMpegPadDrv
--|
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 2011/10/18
--|
--| DESCRIPTION   : Set MPEG Output Interface pad drive strength
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_SetMpegPadDrv(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, MXL_HRCLS_MPEG_PAD_DRV_T* padDrvPtr)
{
  UINT8 status = MXL_SUCCESS;
  const MXL_HRCLS_FIELD_T sync_drive[] = {{PAD_MPEG_0_SYN_DRV}, {PAD_MPEG_1_SYN_DRV},
                                    {PAD_MPEG_2_SYN_DRV}, {PAD_MPEG_3_SYN_DRV},
                                    {PAD_MPEG_4_SYN_DRV}, {PAD_MPEG_5_SYN_DRV},
                                    {PAD_MPEG_6_SYN_DRV}, {PAD_MPEG_7_SYN_DRV}, 
                                    {PAD_MPEG_8_SYN_DRV}};
  const MXL_HRCLS_FIELD_T valid_drive[] = {{PAD_MPEG_0_VAL_DRV}, {PAD_MPEG_1_VAL_DRV},
                                     {PAD_MPEG_2_VAL_DRV}, {PAD_MPEG_3_VAL_DRV},
                                     {PAD_MPEG_4_VAL_DRV}, {PAD_MPEG_5_VAL_DRV},
                                     {PAD_MPEG_6_VAL_DRV}, {PAD_MPEG_7_VAL_DRV}, 
                                     {PAD_MPEG_8_VAL_DRV}};
  const MXL_HRCLS_FIELD_T data_drive[] = {{PAD_MPEG_0_DAT_DRV}, {PAD_MPEG_1_DAT_DRV},
                                     {PAD_MPEG_2_DAT_DRV}, {PAD_MPEG_3_DAT_DRV},
                                     {PAD_MPEG_4_DAT_DRV}, {PAD_MPEG_5_DAT_DRV},
                                     {PAD_MPEG_6_DAT_DRV}, {PAD_MPEG_7_DAT_DRV}, 
                                     {PAD_MPEG_8_DAT_DRV}};

  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, sync_drive[(UINT8) demodId].regAddr,
                                                      sync_drive[(UINT8) demodId].lsbPos,
                                                      sync_drive[(UINT8) demodId].fieldWidth, padDrvPtr->padDrvMpegSyn);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, valid_drive[(UINT8) demodId].regAddr,
                                                      valid_drive[(UINT8) demodId].lsbPos,
                                                      valid_drive[(UINT8) demodId].fieldWidth, padDrvPtr->padDrvMpegVal);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, data_drive[(UINT8) demodId].regAddr,
                                                      data_drive[(UINT8) demodId].lsbPos,
                                                      data_drive[(UINT8) demodId].fieldWidth, padDrvPtr->padDrvMpegDat);
  
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ProgramRegisters
--|
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/19/2011
--|
--| DESCRIPTION   : This function writes multiple registers with provided data array and 
--|                 demod id
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ProgramRegisters(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId , PREG_CTRL_INFO_T ctrlRegInfoPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT16 i, tmp = 0;
  
  i = 0;

  for (;;)
  {
    if ((ctrlRegInfoPtr[i].regAddr == 0) && (ctrlRegInfoPtr[i].mask == 0) && (ctrlRegInfoPtr[i].data == 0)) break;

    // Check if partial bits of register were updated
    if (ctrlRegInfoPtr[i].mask != 0xFFFF)  
    {
      status = MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, ctrlRegInfoPtr[i].regAddr, &tmp);
      if (status != MXL_SUCCESS) break;;
    }
    
    tmp &= ~ctrlRegInfoPtr[i].mask;
    tmp |= ctrlRegInfoPtr[i].data;

    status = MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, ctrlRegInfoPtr[i].regAddr, tmp);
    if (status != MXL_SUCCESS) break;

    i++;
  }

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadDemodRegisterField
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads field belonging to specified demodulator
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadDemodRegisterField(UINT8 devId, 
                                                   MXL_HRCLS_DMD_ID_E demodId, 
                                                   UINT16 regAddr, 
                                                   UINT8 lsbPos, 
                                                   UINT8 fieldWidth, 
                                                   UINT16 * Value) 
{
  UINT8 status;

  status = (UINT8) MxL_HRCLS_Ctrl_ReadRegisterField(devId, MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, regAddr), lsbPos, fieldWidth, Value);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SetDemodAdcIqSwap
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/31/2011
--|                  
--| DESCRIPTION   : Set ADC IQ SWAP control on specified demod
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_SetDemodAdcIqSwap(
    MXL_HRCLS_DEV_CONTEXT_T *devContextPtr,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E  adcIqFlip
    )
{
  UINT8 devId = devContextPtr->devId;
  UINT8 status = MXL_SUCCESS;

  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(
        devId,
        EXTRACT_ADDR(DMD0_SWAP_IQ) + demodId*4,
        EXTRACT_LSBLOC(DMD0_SWAP_IQ),
        EXTRACT_NBITS(DMD0_SWAP_IQ),
        (adcIqFlip == MXL_ENABLE)?1:0);
  }
  else
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(
        devId, 
        EXTRACT_ADDR(DMD0_QAM_IQ_SWAP),
        EXTRACT_LSBLOC(DMD0_QAM_IQ_SWAP) + demodId,
        EXTRACT_NBITS(DMD0_QAM_IQ_SWAP),
        (adcIqFlip == MXL_ENABLE)?1:0);
  }
  return (MXL_STATUS_E)status;
}


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDemodAdcIqSwap
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/31/2011
--|                  
--| DESCRIPTION   : Get ADC IQ SWAP control on specified demod
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDemodAdcIqSwap(
    MXL_HRCLS_DEV_CONTEXT_T *devContextPtr,
    MXL_HRCLS_DMD_ID_E demodId,
    MXL_BOOL_E*  adcIqFlipPtr
    )
{
  UINT8 devId = devContextPtr->devId;
  UINT8 status = MXL_SUCCESS;
  UINT16 iqFlip = 0;

  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
  {
    status |= MxL_HRCLS_Ctrl_ReadRegisterField(
        devId,
        EXTRACT_ADDR(DMD0_SWAP_IQ) + demodId*4,
        EXTRACT_LSBLOC(DMD0_SWAP_IQ),
        EXTRACT_NBITS(DMD0_SWAP_IQ),
        &iqFlip);
  }
  else
  {
    status |= MxL_HRCLS_Ctrl_ReadRegisterField(
        devId, 
        EXTRACT_ADDR(DMD0_QAM_IQ_SWAP),
        EXTRACT_LSBLOC(DMD0_QAM_IQ_SWAP) + demodId,
        EXTRACT_NBITS(DMD0_QAM_IQ_SWAP),
        &iqFlip);
  }
  *adcIqFlipPtr = iqFlip?MXL_ENABLE:MXL_DISABLE;
  return (MXL_STATUS_E)status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  symbolType symbol type
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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodSymbolRate(
    UINT8     devId,                        
    MXL_HRCLS_DMD_ID_E demodId,            
    UINT32    symbRateInHz,                
    UINT32    symbRate256InHz)
{
  UINT8 status = MXL_SUCCESS;
  UINT32 symbolRate[SYM_NUM_B_BANK];
  UINT8 numBank = 0;
  UINT8 bank[SYM_NUM_B_BANK];
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("DemodId=%d, SymbolRate=%u, SymbolRate256=%u\n", demodId, symbRateInHz, symbRate256InHz););

  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt) 
      && (((symbRateInHz <= MXL_HRCLS_DEMOD_SYMBOLRATE_MAX_SPS) && (symbRateInHz >= MXL_HRCLS_DEMOD_SYMBOLRATE_MIN_SPS)) || (symbRateInHz == 0))
      && (((symbRate256InHz <= MXL_HRCLS_DEMOD_SYMBOLRATE_MAX_SPS) && (symbRate256InHz >= MXL_HRCLS_DEMOD_SYMBOLRATE_MIN_SPS)) || (symbRate256InHz == 0)))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      switch (devContextPtr->demods[(UINT8) demodId].curAnnexType)
      {
        case MXL_HRCLS_ANNEX_A:
          symbolRate[0] = (symbRateInHz)?symbRateInHz:MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_A;
          bank[0] = 0;
          numBank = 1;
          break;
        case MXL_HRCLS_ANNEX_B:
          symbolRate[0] = (symbRateInHz)?symbRateInHz:MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_B_QAM64;
          symbolRate[1] = (symbRate256InHz)?symbRate256InHz:MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_B_QAM256;
          bank[0] = 1;
          bank[1] = 2;
          numBank = 2;
          break;
        default:
          status = MXL_INVALID_PARAMETER;
          break;
      }

      /* Configure Resample rate ratio  */
      if (status == MXL_SUCCESS)
      {
        status |= MxL_HRCLS_Ctrl_ConfigDemodResampRatio(devId, demodId, symbolRate, bank, numBank);
        status |= MxL_HRCLS_Ctrl_ConfigDemodEqualizer(devId, demodId);
        devContextPtr->demods[(UINT8) demodId].oobMode = MXL_DISABLE;
      }        
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;    
  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  qamType manual mode, set QAM type directly
 *
 *  @apibrief   Returns value that corresponds with given MxLWare QAM enum 
 *
 *  @usage      Call to get qam value that could be sent to h/w
 *
 *  @return     Value that corresponds with qamType 
 ****************************************************************************************/
static UINT8 MxL_HRCLS_Ctrl_GetHwQamType(MXL_HRCLS_QAM_TYPE_E qamType)
{
  UINT8 hw_qam_type;

  switch (qamType)
  {
    case MXL_HRCLS_QAM16: hw_qam_type = MXL_HRCLS_HW_16QAM; break;
    case MXL_HRCLS_QAM64: hw_qam_type = MXL_HRCLS_HW_64QAM; break;
    case MXL_HRCLS_QAM32: hw_qam_type = MXL_HRCLS_HW_32QAM; break;
    case MXL_HRCLS_QAM128: hw_qam_type = MXL_HRCLS_HW_128QAM; break;
    case MXL_HRCLS_QAM256: hw_qam_type = MXL_HRCLS_HW_256QAM; break;
    case MXL_HRCLS_QAM1024: hw_qam_type = MXL_HRCLS_HW_1024QAM; break;
    default:
    case MXL_HRCLS_QPSK: hw_qam_type = MXL_HRCLS_HW_QPSK; break;
  }
  return hw_qam_type;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  annexType Annex-B or A
 *  @param[in]  autoDetectQamType Enable or disable of Auto QAM detect
 *  @param[in]  autoDetectMode Enable or disable of Auto Annex mode detect
 *  @param[in]  qamType manual mode, set QAM type directly
 *
 *  @apibrief   It configures Annex Type and Auto detect mode for QAM Type.
 *
 *  @usage      See example. [TBD Page 30]
 *
 *  @equ261     MXL_DEMOD_ANNEX_QAM_TYPE_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodAnnexQamType(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_HRCLS_ANNEX_TYPE_E annexType,   
    MXL_HRCLS_QAM_TYPE_E qamType     
    )
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, annexType=%d, qamType=%d\n",
          demodId,
          annexType,
          qamType););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt)) 
  {
    if (devContextPtr->driverInitialized)
    {
      HOST_COMMAND_T demodAnnexQamTypeCfg;
      UINT8 hw_annex_type;

      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      hw_annex_type = (annexType == MXL_HRCLS_ANNEX_B)?MXL_HRCLS_HW_ANNEX_B:MXL_HRCLS_HW_ANNEX_A;

      /* Form command payload */
      demodAnnexQamTypeCfg.data[0]    = (UINT8)demodId;
      demodAnnexQamTypeCfg.data[1]    = (UINT8) ((hw_annex_type)                          << 0);
      demodAnnexQamTypeCfg.data[1]   |= (UINT8) (((UINT8) (qamType == MXL_HRCLS_QAM_AUTO)?1:0)    << 1);
      demodAnnexQamTypeCfg.data[1]   |= (UINT8) (((UINT8) ((annexType == MXL_HRCLS_ANNEX_A) && (qamType == MXL_HRCLS_QAM_AUTO))?1:0)<< 2);
      demodAnnexQamTypeCfg.data[1]   |= (UINT8) ((MxL_HRCLS_Ctrl_GetHwQamType(qamType))   << 4);
      demodAnnexQamTypeCfg.payloadLen = 2;
      demodAnnexQamTypeCfg.syncChar = 0;
      demodAnnexQamTypeCfg.commandId = 0;
      demodAnnexQamTypeCfg.payloadCheckSum = 0;

      /* Send host command */
      status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodAnnexQamTypeCfg, HOST_CMD_CFG_DEMOD_ANNEX_QAM_TYPE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      if (status == MXL_SUCCESS)
      {
        devContextPtr->demods[(UINT8) demodId].curQamType   = qamType;
        devContextPtr->demods[(UINT8) demodId].curAnnexType = annexType;
        if (devContextPtr->demods[(UINT8) demodId].oobMode == MXL_ENABLE)
        {
          status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_QAM_WATCHDOG_ENABLE, 1);
          if (status == MXL_SUCCESS)
          {
            status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_ACI_CUSTOM_COEF_ENABLE, 0);
          }
        }
        devContextPtr->demods[(UINT8) demodId].oobMode = MXL_DISABLE;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status; 
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] intrMask interrupt MASK word
 *
 *  @apibrief   This function enables or disables a demod interrupt.
 *              [TBD Page 31]
 *
 *  @usage      \n In order to monitor Demod status after tuning, each demod lock status bit can be enabled.
 *              \n In addition to setting each interrupt enable bit to one, INT_EMABLE_MASK bit has to be set to enable Interrupt.
 *              \n
 *              \n For example, to enable FEC_LOCK and MPEG_LOCK interrupts, the value 0x800A should be passed through InterMask field of parameters.
 *              \n
 *              \n And then to disable FEC_LOCK interrupt, the value 0x0002 should be passed through InterMask field of parameters.
 *
 *  @equ261     MXL_DEMOD_INTR_MASK_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodIntrMask(
    UINT8     devId,                       
    MXL_HRCLS_DMD_ID_E demodId,           
    UINT16    intrMask                   
    )
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, intrMask=%x\n", demodId, intrMask););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt)) 
  {
    if (devContextPtr->driverInitialized)
    {
	  // demodId not needed here
      // demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      // demodId = demodId;
      intrMask = intrMask;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status; 
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodMpegOutParams(
    UINT8     devId,                           /* IN : MxL device id */
    MXL_HRCLS_DMD_ID_E demodId,                /* IN : Demodulator ID number */
    MXL_HRCLS_MPEGOUT_PARAM_T* mpegOutParamPtr /* IN : MPEG out parameters */
    )
{
  UINT8 status = MXL_SUCCESS;
  const MXL_HRCLS_FIELD_T mdval[] = {{DMD0_MDVAL_GATE_EN}, {DMD1_MDVAL_GATE_EN},
                                     {DMD2_MDVAL_GATE_EN}, {DMD3_MDVAL_GATE_EN},
                                     {DMD4_MDVAL_GATE_EN}, {DMD5_MDVAL_GATE_EN},
                                     {DMD6_MDVAL_GATE_EN}, {DMD7_MDVAL_GATE_EN},
                                     {DMD8_MDVAL_GATE_EN}};
  const MXL_HRCLS_FIELD_T mdval_pol[] = {{DMD0_MDVAL_GATE_EN_INV}, {DMD1_MDVAL_GATE_EN_INV},
                                        {DMD2_MDVAL_GATE_EN_INV}, {DMD3_MDVAL_GATE_EN_INV},
                                        {DMD4_MDVAL_GATE_EN_INV}, {DMD5_MDVAL_GATE_EN_INV},
                                        {DMD6_MDVAL_GATE_EN_INV}, {DMD7_MDVAL_GATE_EN_INV},
                                        {DMD8_MDVAL_GATE_EN_INV}};
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, MpegEnabled=%d, lsbOrMsbFirst=%d,"
       "mpegValidPol=%d, mpegSyncPol=%d, mpegSyncPulseWidth=%d, mpegErrorIndication=%d,"
       "3wireMode=%d, padDrvMpegSyn=%d padDrvMpegDat=%d padDrvMpegVal=%d\n",
          demodId, 
          mpegOutParamPtr->enable,
          mpegOutParamPtr->lsbOrMsbFirst,
          mpegOutParamPtr->mpegValidPol,
          mpegOutParamPtr->mpegSyncPol,
          mpegOutParamPtr->mpegSyncPulseWidth,
          mpegOutParamPtr->mpegErrorIndication,
          mpegOutParamPtr->mpeg3WireModeEnable,
          mpegOutParamPtr->mpegPadDrv.padDrvMpegSyn,
          mpegOutParamPtr->mpegPadDrv.padDrvMpegDat,
          mpegOutParamPtr->mpegPadDrv.padDrvMpegVal););
  if ((devContextPtr) && (mpegOutParamPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->xpt.supported == MXL_FALSE)
      {
        UINT16 control_reg_value = 0x0000;    // default values for register 0x03c8
        UINT16 control_reg_address;

        demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
        control_reg_address = MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, EXTRACT_ADDR(DMD0_VALID_ACTIVE_LEVEL));
        control_reg_value |= (UINT16) ((UINT16) ((mpegOutParamPtr->lsbOrMsbFirst == MXL_HRCLS_MPEG_SERIAL_LSB_1ST)?0:1) << EXTRACT_LSBLOC(DMD0_MSB_LSB_FIRST)); 
        control_reg_value |= (UINT16) ((UINT16) ((mpegOutParamPtr->mpegErrorIndication == MXL_TRUE)?1:0) << EXTRACT_LSBLOC(DMD0_MPEG_FRAME_ERROR_INDICATION));
        control_reg_value |= (UINT16) ((UINT16) ((mpegOutParamPtr->mpegValidPol == MXL_HRCLS_MPEG_ACTIVE_LOW)?1:0) << EXTRACT_LSBLOC(DMD0_VALID_ACTIVE_LEVEL)); 
        control_reg_value |= (UINT16) ((UINT16) ((mpegOutParamPtr->mpegSyncPol == MXL_HRCLS_MPEG_ACTIVE_LOW)?1:0) << EXTRACT_LSBLOC(DMD0_SYNC_ACTIVE_LEVEL)); 
        control_reg_value |= (UINT16) ((UINT16) ((mpegOutParamPtr->mpegSyncPulseWidth == MXL_HRCLS_MPEG_SYNC_WIDTH_BIT)?0:1) << EXTRACT_LSBLOC(DMD0_SYNC_PULSE_WIDTH)); 

      // disable - ticket #369
        // control_reg_value |= (UINT16) (1 << EXTRACT_LSBLOC(DMD0_DISABLE_FIFO_READ_LIMIT));    

        MXLDBG3(MxL_HRCLS_DEBUG("CfgDemodMpeg. control_reg=%x, control_addr=%x\n", control_reg_value, EXTRACT_ADDR(DMD0_VALID_ACTIVE_LEVEL)););

        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, mdval[(UINT8) demodId].regAddr,
                          mdval[(UINT8) demodId].lsbPos, mdval[(UINT8) demodId].fieldWidth,
                          (mpegOutParamPtr->mpeg3WireModeEnable == MXL_ENABLE)?1:0);
        if (mpegOutParamPtr->mpeg3WireModeEnable == MXL_ENABLE)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, mdval_pol[(UINT8) demodId].regAddr,
                            mdval_pol[(UINT8) demodId].lsbPos, mdval_pol[(UINT8) demodId].fieldWidth,
                            (mpegOutParamPtr->mpeg3WireModeClkPol == MXL_HRCLS_MPEG_CLK_IN_PHASE)?1:0);
        }
        status |= MxLWare_HRCLS_OEM_WriteRegister(devId, control_reg_address, control_reg_value);
        status |= MxL_HRCLS_Ctrl_SetMpegPadDrv(devId, demodId, &mpegOutParamPtr->mpegPadDrv);
        status |= MxL_HRCLS_Ctrl_SetMpegOutEnable(devContextPtr, demodId, mpegOutParamPtr->enable);
      }
      else
      {
        status = MXL_NOT_SUPPORTED;
        MXLERR(MxL_HRCLS_ERROR("This device does not support regular TS output configuration. Use XPT functions instead\n"););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  mpegClkPol MPEG data validity with clock, Rising or Falling edge
 *  @param[in]  mpegClkPadDrv MPEG clock pad drive mode
 *
 *  @apibrief   Configure global MPEG data output clock polarity. This setting affects output clocks of all four demods.
 *
 *  @usage      This API should be called with MXL_DEMOD_MPEG_OUT_CFG.
 *
 *  @equ261     MXL_DEMOD_CLK_POLARITY_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(
    UINT8     devId,                        
    MXL_HRCLS_MPEG_CLK_FMT_E mpegClkPol,   
    MXL_HRCLS_MPEG_DRV_MODE_E mpegClkPadDrv,
    MXL_HRCLS_MPEG_CLK_RATE_E mpegClkFreq  
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("mpegClkPol=%d, mpegClkPadDrv=%d, mpegClkFreq=%d\n", mpegClkPol, mpegClkPadDrv, mpegClkFreq););
  if (devContextPtr)
  {
   if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->xpt.supported == MXL_FALSE)
      {
        // Mariusz: According to Jinzhou, first write 1 to MCLK_CFG_ENA, next write
        // frequency, and write 0 to MCLK_CFG_ENA
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MCLK_CFG_ENA, 1);
        // set frequency here
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MCLK_CTRL, mpegClkFreq);
        // set pad drive
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, PAD_MPEG_CLK_DRV, mpegClkPadDrv);
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MCLK_CFG_ENA, 0);

        // FIXME: MP_CLK_INV is removed from V1R3B3; should it be MCLK_INV? need verification
#ifdef MP_CLK_INV
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MP_CLK_INV, ((mpegClkPol == MXL_HRCLS_MPEG_CLK_POSITIVE)?0:1));
#else // MP_CLK_INV
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, MCLK_INV, ((mpegClkPol == MXL_HRCLS_MPEG_CLK_POSITIVE)?0:1));
#endif // MP_CLK_INV
      }
#ifdef _MXL_HRCLS_XPT_ENABLED_  
      else if ((devContextPtr->xpt.fourWireModeSupported == MXL_TRUE) && (devContextPtr->xpt.currentMode->mode !=  MXL_HRCLS_XPT_MODE_PARALLEL) && (devContextPtr->xpt.currentMode->mode != MXL_HRCLS_XPT_MODE_CABLECARD))
      {
        status = MxLWare_Ctrl_ConfigureCommonClock(devContextPtr, mpegClkFreq, mpegClkPol, mpegClkPadDrv);

        if (status == MXL_SUCCESS) devContextPtr->xpt.commonClockEnabled = MXL_TRUE;
      }
#endif      
      else
      {
        status = MXL_NOT_SUPPORTED;
        MXLERR(MxL_HRCLS_ERROR("This device does not support regular TS output configuration. Use XPT functions instead\n"););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E)  status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodAdcIqFlip(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_HRCLS_IQ_FLIP_E  adcIqFlip               
    )
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  /* Enable = 1, disable = 0
   * Flip the I/Q path after ADC
   * DIG_ADCIQ_FLIP <4>
   */

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, AdcIqFlip=%d\n", demodId, adcIqFlip););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      if (adcIqFlip == MXL_HRCLS_IQ_AUTO)
      {
        if (devContextPtr->autoSpectrumInversionSupported == MXL_TRUE)
        {
          devContextPtr->demods[(UINT8) demodId].autoSpectrumInversionEnabled = MXL_TRUE;
        } else status = MXL_NOT_SUPPORTED;
      }
      else
      {
        devContextPtr->demods[(UINT8) demodId].autoSpectrumInversionEnabled = MXL_FALSE;
        status = MxL_HRCLS_Ctrl_SetDemodAdcIqSwap(devContextPtr, demodId, (adcIqFlip == MXL_HRCLS_IQ_NORMAL)?MXL_FALSE:MXL_TRUE);
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodEqualizerFilter(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_BOOL_E  equalizerSetting        
    )
{  
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, equalizerSetting=%d\n", demodId, equalizerSetting););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EQU_SPUR_BYPASS, (equalizerSetting == MXL_ENABLE)?1:0);
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}


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
 *  @retval     MXL_SUCCESS If the counter is successfully cleared
 *  @retval     MXL_FAILURE If I2C error occured
 *  @retval     MXL_NOT_INITIALIZED If the devId relates to MxL device that was not
 *                                  initialized
 *  @retval     MXL_INVALID_PARAMETER If either devId or demodId are out of range
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodErrorStatClear(
    UINT8     devId,                    
    MXL_HRCLS_DMD_ID_E demodId         
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_FECA_STAMP, 0x0055);
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_Ctrl_ReqDemodSnrLinear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 5/2/2012
--|                  
--| DESCRIPTION   : Finds a,b coeffs for given snr_rawValue and calculates SNR:
--|                 snr = a*(rawValue) + b 
--|                 
--| RETURN VALUE  : snr dBx10 
--|
--|-------------------------------------------------------------------------------------*/

static UINT16 MxLWare_HRCLS_Ctrl_ReqDemodSnrLinear(MxL_HRCLS_SNR_RANGE_T * snrArray, UINT16 rawValue)
{
  UINT32 snr = 0;
  UINT8 index = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("snrArray=%p, rawValue=%d\n", snrArray, rawValue););

  if (rawValue)
  {
    while ((snrArray[index].x2) && (rawValue > snrArray[index].x2)) index++;

    if (snrArray[index].x2 == 0) index--;
    MXLDBG3(MxL_HRCLS_DEBUG("index=%d, a=%d, b=%d\n", index, snrArray[index].a, snrArray[index].b););
    snr = snrArray[index].a * rawValue + snrArray[index].b;
    snr = MXL_DIV_ROUND(snr, 10000);
  }
  MXLEXITSTR(snr);

  return (UINT16) snr;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] snrPtr SNR, fixed point format with 1 decimal digit (dBx10)
 *  @param[out] mseSizePtr MSE size
 *
 *  @apibrief   This API retrieves SNR of channel.
 *
 *  @usage      After tuning, this API can be called at any time to check channel status.
 *
 *  @equ261     MXL_DEMOD_SNR_REQ
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodSnr(
    UINT8     devId,                 
    MXL_HRCLS_DMD_ID_E demodId,     
    UINT16*   snrPtr               
    )
{
  UINT8 status = MXL_SUCCESS;

  MXL_HRCLS_QAM_TYPE_E qamType = MXL_HRCLS_QAM16;
  UINT16 regData = 0;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt) && (snrPtr) )
  {
    if (devContextPtr->driverInitialized)
    {
      MxL_HRCLS_SNR_RANGE_T * snrArray = NULL;
      /* Read QAM and Annex type */
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, NULL, &qamType);

      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EQU_DEBUG_MSE_CALC_COEF, MXL_HRCLS_MSE_AVG_COEF); 
      if (FIRMWARE_STATUS_LOADED == devContextPtr->firmwareStatus)
      {
        status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_MSE_BASE + 2*demodId, &regData);
      }
      else
      {
        status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS, MXL_HRCLS_READ_MSE_START_ADDRESS); 
        status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_DATA, &regData);
      }
      MXLDBG3(MxL_HRCLS_DEBUG("SNR : QAM Type = %d, read_value = 0x%x\n", qamType, regData););

      switch (qamType)
      {
        case MXL_HRCLS_QAM16:
          //          x = 128;
          snrArray = snrRangeQam16; 
          break;

        case MXL_HRCLS_QAM64:
          //          x = 122;
          snrArray = snrRangeQam64; 
          break;

        case MXL_HRCLS_QAM256:
          //          x = 120;
          snrArray = snrRangeQam256; 
          break;

        case MXL_HRCLS_QAM32: 
          //          x = 256;
          snrArray = snrRangeQam32; 
          break;

        case MXL_HRCLS_QAM128: 
          //          x = 250;
          snrArray = snrRangeQam128; 
          break;

        case MXL_HRCLS_QPSK:
          //          x = 71;
          snrArray = snrRangeQpsk; 
          break;

        default:
          //          x = 20;
          snrArray = snrRangeQpsk; 
          break;
      }

      /*
       * MSE = 10log10(regData * 2^ -(N + Log2(1/16)) * 8/K)
       * SNR = -1 * MSE
       * n = 16 (No. of Bits)
       * K = es/(M/2)2
       * E & M change with QAM types
       * x = 2^-(N + log2(1/16)) * 8/k
       */
      *snrPtr = MxLWare_HRCLS_Ctrl_ReqDemodSnrLinear(snrArray, regData);
      MXLDBG3(MxL_HRCLS_DEBUG("SNR = (%d)dBx10\n", *snrPtr);); 
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
 
  return (MXL_STATUS_E) status;           
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] berPtr BER, float point format
 *  @param[out] uncodedBerPtr Uncoded BER, float point format
 *  @param[out] cerPtr CW error rate, float point format
 *
 *  @apibrief   This API retrieves BER, Uncoded BER, and CER of channel.
 *
 *  @usage      \n After turning, this API can be called at any time to check BER, Uncoded BER, and CER of a demod channel.
 *              \n However, this API must be called less than 1 second in order to update consecutive BER, UBER, and CER.
 *              \n If the integer calculation is used, then this API should be called every 900ms.
 *              \n The integer calculation is less accurate than floating calculation, and the error will be within 2%.
 *              [TBD]: Do we add more details on usage as in Page 38?
 *
 *  @equ261     MXL_DEMOD_BER_UNCODED_BER_CER_REQ
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodErrorStat(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_HRCLS_DMD_STAT_CNT_T * statsPtr 
    )
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXL_HRCLS_ANNEX_TYPE_E annexType = MXL_HRCLS_ANNEX_A;
  UINT16 regData = 0;
  UINT8  status = MXL_SUCCESS;

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (statsPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      /* Check Annex Type */
      status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, &annexType, NULL);

      /* Clear the internal HW counters to avoid overflow */
      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_FECA_STAMP, 0x0055);

      status |= MxL_HRCLS_Ctrl_GetMCNSSD(devId, demodId, MXL_HRCLS_CW_CORR_COUNT_SEL, &statsPtr->CwCorrCount);
      status |= MxL_HRCLS_Ctrl_GetMCNSSD(devId, demodId, MXL_HRCLS_CW_ERR_COUNT_SEL, &statsPtr->CwErrCount);
      status |= MxL_HRCLS_Ctrl_GetMCNSSD(devId, demodId, MXL_HRCLS_CW_COUNT_SEL, &statsPtr->CwReceived);
      status |= MxL_HRCLS_Ctrl_GetMCNSSD(devId, demodId, MXL_HRCLS_CORR_BITS_SEL, &statsPtr->CorrBits);

      status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_FECA_FRCNT, &regData);
      /* ERR_MPEG */
      if (annexType == MXL_HRCLS_ANNEX_A) 
      {
        statsPtr->ReceivedMpeg = 0xFFFF - regData;
        statsPtr->ErrMpeg = statsPtr->CwErrCount;
      }
      else
      {
        statsPtr->ReceivedMpeg = regData;

        status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_FECA_MEF, &regData); 
        statsPtr->ErrMpeg = regData;
      }
  
      MXLDBG2(MxL_HRCLS_PRINT("CwCorrCount = %d\n", statsPtr->CwCorrCount););
      MXLDBG2(MxL_HRCLS_PRINT("CwErrCount = %d\n", statsPtr->CwErrCount););
      MXLDBG2(MxL_HRCLS_PRINT("CwReceived = %d\n", statsPtr->CwReceived););
      MXLDBG2(MxL_HRCLS_PRINT("CorrBits = %d\n", statsPtr->CorrBits););
      MXLDBG2(MxL_HRCLS_PRINT("ReceivedMpeg = %d\n", statsPtr->ReceivedMpeg););
      MXLDBG2(MxL_HRCLS_PRINT("ErrMpeg = %d\n", statsPtr->ErrMpeg););

    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E) status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodAnnexQamType(
    UINT8     devId,                       
    MXL_HRCLS_DMD_ID_E demodId,           
    MXL_HRCLS_ANNEX_TYPE_E* annexTypePtr,
    MXL_HRCLS_QAM_TYPE_E* qamTypePtr    
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);


  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt) && (annexTypePtr) &&
    (qamTypePtr))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, annexTypePtr, qamTypePtr);
#ifdef _MXL_DIAG_ENABLED_
      /* Check if QAM or Annex type has been changed since last update */
      if (devContextPtr->demods[demodId].curQamType   != *qamTypePtr ||
          devContextPtr->demods[demodId].curAnnexType != *annexTypePtr)
      {
        status |= DMD_UploadCustomCoefs(demodId, *annexTypePtr, *qamTypePtr);
      }
#endif // _MXL_DIAG_ENABLED_
      devContextPtr->demods[(UINT8) demodId].curQamType   = *qamTypePtr;
      devContextPtr->demods[(UINT8) demodId].curAnnexType = *annexTypePtr;      
      MXLDBG2(MxL_HRCLS_DEBUG("AnnexType=%d, QamType=%d\n", *annexTypePtr, *qamTypePtr););
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodCarrierOffset(
    UINT8     devId,             
    MXL_HRCLS_DMD_ID_E demodId,      
    SINT32*   carrierOffsetInHzPtr  
    )
{
  UINT8 status = MXL_SUCCESS;
  UINT8 byPass;
  UINT16 regData = 0;
  UINT32 symbolRateInHz = 0;
  UINT32 resampleRateRatio = 0;

  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (carrierOffsetInHzPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      if (status == MXL_SUCCESS)
      {
        status = MxL_HRCLS_Ctrl_GetDemodCurrentResampleRate(devContextPtr, demodId, &resampleRateRatio);
        byPass = (devContextPtr->demods[(UINT8) demodId].curBypassInterpolator == MXL_TRUE)?1:0;
        if ((status == MXL_SUCCESS) && (resampleRateRatio))
        {
//          symbolRateInHz = ((UINT64)(devContextPtr->adcSampRateInHz)*(UINT64)(1<<14)*(byPass?1:3))/((UINT64) resampleRateRatio);
          symbolRateInHz = MXL_DIV_ROUND((UINT64)(devContextPtr->adcSampRateInHz)*(UINT64)(1<<14)*(byPass?1:3), resampleRateRatio);
          if (FIRMWARE_STATUS_LOADED == devContextPtr->firmwareStatus)
          {
            status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_CARRIER_OFFSET_BASE + 2*demodId, &regData);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_ADDRESS), MXL_HRCLS_READ_CARRIER_OFFSET_ADDRESS);
            /* Read the current settings. */
            status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_DATA, &regData);
          }
          devContextPtr->demods[(UINT8) demodId].curSymbolRateInHz = symbolRateInHz;

          /* CarrierOffset = SymbolRate * Singed(regData) * 2^(-N+1+log2(1/16)) */
          /* N = number of bits, therefore, N = 16  */
          *carrierOffsetInHzPtr = (SINT32)((SINT64)symbolRateInHz * (SINT64)((SINT16)regData) / (SINT64)(1<<19));

          MXLDBG2(MxL_HRCLS_DEBUG("CarrierOffset=%dHz, symbolRate=%dSps\n", *carrierOffsetInHzPtr, symbolRateInHz););
        }
        else
        {
          MXLERR(MxL_HRCLS_ERROR("%s: RATE_RESAMP_RATE has not been programmed\n", __FUNCTION__););
          status = MXL_FAILURE;
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDemodCurrentInterleaverDepth(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E demodId, MXL_HRCLS_ANNEX_TYPE_E annexType, /*@out@*/ UINT8 * lengthIPtr, /*@out@*/ UINT8 * lengthJPtr)
{
  UINT8 status = MXL_SUCCESS;
  PINTERLEAVER_LOOKUP_INFO_T interDepthLoopUpTable = MxL_HRCLS_InterDepthLookUpTable;
      
  if (annexType == MXL_HRCLS_ANNEX_B) /* Annex_B */
  {
    UINT16 regData = 0;
    /* Before readback index for interleaver depth, clear out bit6-3 */
    status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devContextPtr->devId, demodId, DMD0_FECB_INT_D, 0x00);  
    /* Read the current settings. */
    status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devContextPtr->devId, demodId, DMD0_FECB_INT_D, &regData);
    /* Interleaver Depth I, J <6:3>
     * regData = Control word(4bits) becomes 
     * the address of InterleaverDepth LookUp Table
     */
    regData &= 0x000F;

    MXLDBG2(MxL_HRCLS_DEBUG("Interleaver depth Index : %d\n", regData););
    *lengthIPtr = interDepthLoopUpTable[regData].interDepthI;
    *lengthJPtr = interDepthLoopUpTable[regData].interDepthJ;
  }
  else           /* Annex_A */
  {
    *lengthIPtr = 12;
    *lengthJPtr = 17;
  }
  
  return (MXL_STATUS_E) status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodInterleaverDepth(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    UINT8*    interDepthIPtr,           
    UINT8*    interDepthJPtr           
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_ANNEX_TYPE_E annexType = MXL_HRCLS_ANNEX_A;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (interDepthIPtr) && (interDepthJPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      /* Read the current Annex Type <10> */
      status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, &annexType, NULL);
      status |= MxL_HRCLS_Ctrl_GetDemodCurrentInterleaverDepth(devContextPtr, demodId, annexType, interDepthIPtr, interDepthJPtr);

      MXLDBG2(MxL_HRCLS_DEBUG("InterDepthI=%d, InterDepthJ=%d\n", *interDepthIPtr, *interDepthJPtr););
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  
  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E) status;
}

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
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););

  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if ((devContextPtr->chipVersion > 1) && (devContextPtr->chipId ==  MXL_HRCLS_HERCULES_CHIP_ID))
      {
        UINT8 i;
        UINT8 depthI, depthJ;
        MXL_HRCLS_ANNEX_TYPE_E annexType = MXL_HRCLS_ANNEX_A;
        MXL_HRCLS_QAM_TYPE_E qamType;

        demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
  
        /* Read the current Annex Type <10> */
        status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, &annexType, &qamType);
        status |= MxL_HRCLS_Ctrl_GetDemodCurrentInterleaverDepth(devContextPtr, demodId, annexType, &depthI, &depthJ);

        i = 0;
        while ((MxL_HRCLS_BurstNoiseLookup[i].annexType != 0) 
            || (MxL_HRCLS_BurstNoiseLookup[i].qamType   != 0) 
            || (MxL_HRCLS_BurstNoiseLookup[i].depthI    != 0) 
            || (MxL_HRCLS_BurstNoiseLookup[i].depthJ    != 0))
        {
          if ((annexType == MxL_HRCLS_BurstNoiseLookup[i].annexType)
           && (qamType   == MxL_HRCLS_BurstNoiseLookup[i].qamType)
           && (depthI    == MxL_HRCLS_BurstNoiseLookup[i].depthI)
           && (depthJ    == MxL_HRCLS_BurstNoiseLookup[i].depthJ))
          { 
            status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_MAX_FREEZE_WIN_LEN, MxL_HRCLS_BurstNoiseLookup[i].dmdMaxFreezeWinLen); 
            MXLDBG2(MxL_HRCLS_DEBUG("demodId=%d, annexType=%d, qamType=%d, depthI:%d, depthJ:%d, dmdMaxFreezeWinLen:%d\n",
                             demodId,    annexType,    qamType,    depthI,    depthJ,    MxL_HRCLS_BurstNoiseLookup[i].dmdMaxFreezeWinLen);)
            break;
          }
          else
          {
            i++;
          }
        }
        if    ((MxL_HRCLS_BurstNoiseLookup[i].annexType == 0) 
            && (MxL_HRCLS_BurstNoiseLookup[i].qamType   == 0) 
            && (MxL_HRCLS_BurstNoiseLookup[i].depthI    == 0) 
            && (MxL_HRCLS_BurstNoiseLookup[i].depthJ    == 0))
        {
          MXLERR(MxL_HRCLS_DEBUG("BurstNoiseLookup not found, demodId=%d, annexType=%d, qamType=%d, depthI:%d, depthJ:%d\n",
                                                       demodId,    annexType,    qamType,    depthI,    depthJ););
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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/
#define _DEMOD_EQUALIZER_BLOCK_READ_
MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodEqualizerFilter(
    UINT8     devId,                       
    MXL_HRCLS_DMD_ID_E demodId,           
    MXL_HRCLS_EQU_FILTER_INFO_T* equInfoPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (equInfoPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 regData = 0, leadingFfeTap = 0, leadingFfeTapIndex = 0;
      UINT8 i;
#ifdef _DEMOD_EQUALIZER_BLOCK_READ_
      HOST_COMMAND_T demodEqualizerReq;
      UINT16  rspLenBytes = 0;
#endif
  
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
#ifdef _DEMOD_EQUALIZER_BLOCK_READ_
      /* Form command payload */
      demodEqualizerReq.data[0]    = (UINT8)demodId;
      demodEqualizerReq.payloadLen = 3;
      demodEqualizerReq.syncChar = 0;
      demodEqualizerReq.commandId = 0;
      demodEqualizerReq.payloadCheckSum = 0;

      status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_TRUE, 1, &rspLenBytes); // To ensure that some other thread is not reading back the equalizer settings. Timeout not needed.
      if (MXL_SUCCESS == status)
      {
        demodEqualizerReq.data[1]    = (UINT8)MXL_HRCLS_EQUALIZER_FFE;
        demodEqualizerReq.data[2]    = MXL_HRCLS_FFE_INFO_LENGTH;
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodEqualizerReq, HOST_CMD_REQ_DEMOD_EQUALIZER_COEFFICIENTS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        if (MXL_SUCCESS == status)
        {
          status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_FALSE, 10, &rspLenBytes);
          if (MXL_SUCCESS == status)
          {
            status = MxL_HRCLS_Ctrl_ReadBlock16(devId, MXL_HRCLS_BLK_READ_DMD_EQUALIZER_FILTER, 0, MXL_HRCLS_FFE_INFO_LENGTH, (UINT16*)&(equInfoPtr->ffeInfo[0]));
            for (i=0; i<MXL_HRCLS_FFE_INFO_LENGTH; i++)
            {
              equInfoPtr->ffeInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) equInfoPtr->ffeInfo[i], (SINT16) 66);  // 32767 / 500 = 65.5 (-500 <-> +500)
            }
          }
        } 
      } else status = MXL_NOT_READY;  

      if (status == MXL_SUCCESS)
      {
        status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_TRUE, 1, &rspLenBytes);
        if (MXL_SUCCESS == status)
        {
          demodEqualizerReq.data[1]    = (UINT8)MXL_HRCLS_EQUALIZER_SPUR;
          demodEqualizerReq.data[2]    = MXL_HRCLS_SPUR_INFO_LENGTH;
          status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodEqualizerReq, HOST_CMD_REQ_DEMOD_EQUALIZER_COEFFICIENTS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
          if (MXL_SUCCESS == status)
          {
            status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_FALSE, 10, &rspLenBytes);
            if (MXL_SUCCESS == status)
            {
              status = MxL_HRCLS_Ctrl_ReadBlock16(devId, MXL_HRCLS_BLK_READ_DMD_EQUALIZER_FILTER, 0, MXL_HRCLS_SPUR_INFO_LENGTH, (UINT16*)&(equInfoPtr->spurInfo[0]));
              for (i=0; i<MXL_HRCLS_SPUR_INFO_LENGTH; i++)
              {
                equInfoPtr->spurInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) (((UINT16)equInfoPtr->spurInfo[i])<<4), (SINT16) (2 * 16));
              }
            }
          }
        } else status = MXL_NOT_READY;
      }
      
      if (status == MXL_SUCCESS)
      {
        if ((devContextPtr->chipVersion > 1) || (devContextPtr->chipId ==  MXL_HRCLS_MINOS_CHIP_ID))
        {
          equInfoPtr->dsEqDfeTapNum = (MXL_HRCLS_DFE_EQUALIZER_TAPS_COUNT * 2);
        }
        else
        {
          equInfoPtr->dsEqDfeTapNum = (MXL_HRCLS_DFE_EQUALIZER_TAPS_COUNT_V1 * 2);
        }
        status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_TRUE, 1, &rspLenBytes);
        if (MXL_SUCCESS == status)
        {
          demodEqualizerReq.data[1]    = (UINT8)MXL_HRCLS_EQUALIZER_DFE;
          demodEqualizerReq.data[2]    = equInfoPtr->dsEqDfeTapNum;
          status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodEqualizerReq, HOST_CMD_REQ_DEMOD_EQUALIZER_COEFFICIENTS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
          if (MXL_SUCCESS == status)
          {
            status = MxL_HRCLS_Ctrl_WaitForFieldValue(devId, MAILBOX_REG_DMD_EQUALIZER_RSP_LEN, 0, 16, 0, MXL_FALSE, 10, &rspLenBytes);
            if (MXL_SUCCESS == status)
            {
              status = MxL_HRCLS_Ctrl_ReadBlock16(devId, MXL_HRCLS_BLK_READ_DMD_EQUALIZER_FILTER, 0, equInfoPtr->dsEqDfeTapNum, (UINT16*)&(equInfoPtr->dfeInfo[0]));
              for (i=0; i<equInfoPtr->dsEqDfeTapNum ; i++)
              {
                equInfoPtr->dfeInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) equInfoPtr->dfeInfo[i], (SINT16) 66);  // 32767 / 500 = 65.5 (-500 <-> +500)
              }
            }
          }
        } else status = MXL_NOT_READY;
      }
#else

      status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_EXT_SPACE_AUTOREAD_EN, 0x00);
      MxLWare_HRCLS_OEM_DelayUsec(1000);

      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS_AUTO_INCREMENT, (UINT16) 1); 
      equInfoPtr->dsEqDfeTapNum = 56;

      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS, (UINT16) MXL_HRCLS_READ_FFE_START_ADDRESS); 
      for (i=0; i<MXL_HRCLS_FFE_INFO_LENGTH; i++)
      {
        status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_DATA), &regData);
        equInfoPtr->ffeInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) regData, (SINT16) 66);  // 32767 / 500 = 65.5 (-500 <-> +500)
      }
      
      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS, (UINT16) MXL_HRCLS_READ_SPUR_START_ADDRESS); 
      for (i=0; i<MXL_HRCLS_SPUR_INFO_LENGTH; i++)
      {
        status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_DATA), &regData);
		    regData <<= 4;
        equInfoPtr->spurInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) regData, (SINT16) (2 * 16));
      }

      status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS, (UINT16) MXL_HRCLS_READ_DFE_START_ADDRESS); 
      for (i=0; i<equInfoPtr->dsEqDfeTapNum ; i++)
      {
        status |= MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_DATA), &regData);
		    equInfoPtr->dfeInfo[i] = (SINT16) MXL_DIV_ROUND_S((SINT16) regData, (SINT16) 66);  // 32767 / 500 = 65.5 (-500 <-> +500)
      }
      status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_EXT_SPACE_AUTOREAD_EN, 0x01);
#endif      

      if (status == MXL_SUCCESS)
      {
        status = MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EQU_FFE_TAP_LEAD_POSITION), &regData);
        leadingFfeTap = MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, DMD0_EQU_FFE_TAP_LEAD_POSITION); 
  //      leadMode = MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, DMD0_EQU_FFE_TAP_LEAD_MODE); 

        switch (leadingFfeTap)
        {
          case 0: leadingFfeTapIndex = 4; break;
          case 1: leadingFfeTapIndex = 5; break;
          case 2: leadingFfeTapIndex = 6; break;
          default:
          case 3: leadingFfeTapIndex = 7; break;
        }
        equInfoPtr->ffeInfo[leadingFfeTapIndex * 2] = 1000;
        equInfoPtr->ffeInfo[leadingFfeTapIndex * 2 + 1] = 0;
        equInfoPtr->dsEqMainLocation = (UINT8) leadingFfeTapIndex;

        MXLDBG2(MxL_HRCLS_DEBUG("DFE taps count = %d, leading FFE tap = %d\n", equInfoPtr->dsEqDfeTapNum, leadingFfeTapIndex););
      }

      /* DsEqMainLocation, do later */
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

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
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodTimingOffset(
    UINT8     devId,                
    MXL_HRCLS_DMD_ID_E demodId,    
    SINT16*   timingOffsetPtr     
    )
{
  UINT8 status = MXL_SUCCESS;
  SINT16 godardAcc = 0;

  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (timingOffsetPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      if (status == MXL_SUCCESS)
      {
        SINT64 temp;
        SINT64 tmpRate2;
        UINT32 resampleRate;

        status = MxL_HRCLS_Ctrl_GetDemodCurrentResampleRate(devContextPtr, demodId, &resampleRate);

        if (resampleRate)
        {
          /* Calculate timing offset (ppm unit) */
          status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_GODARD_ACC, (UINT16*) &godardAcc);

          tmpRate2 = (SINT64) resampleRate; 
          temp = (SINT32) godardAcc;
          temp *= 2000000;
          temp = MXL_DIV_ROUND_S(temp, tmpRate2);
          temp = MXL_DIV_ROUND_S(temp, (SINT64) 4);
          *timingOffsetPtr = (SINT16) temp;

          MXLDBG2(MxL_HRCLS_DEBUG("timingOffset = %dppm\n", *timingOffsetPtr););
        }
        else
        {
          status = MXL_NOT_READY;
          MXLERR(MxL_HRCLS_ERROR("%s: RATE_RESAMP_RATE has not been programmed\n", __FUNCTION__););
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
 *  @param[in]  demodId Demodulator ID number
 *  @param[in]  demodEnable Enable or disable demod hardware
 *
 *  @apibrief   This API enables or disables demod hardware
 *
 *  @usage      This API can be called to enable or disable demod hardware
 *
 *  @equ261     MXL_DEMOD_PSM_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodEnable(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_BOOL_E  demodEnable             
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, demodEnable=%d\n", demodId, (demodEnable==MXL_TRUE)?1:0););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
  
      if (devContextPtr->xpt.supported == MXL_FALSE)
      {
        status |= MxL_HRCLS_Ctrl_SetMpegOutEnable(devContextPtr, demodId, demodEnable);
      }
      status |= MxL_HRCLS_Ctrl_ConfigDemodClockEnable(devContextPtr, demodId, demodEnable);
      if (status == MXL_SUCCESS) devContextPtr->demods[(UINT8) demodId].enabled = demodEnable;

      if ((devContextPtr->demods[(UINT8) demodId].oobMode == MXL_TRUE) && (MXL_DISABLE == demodEnable))
      {
        if (MXL_TRUE == devContextPtr->oobFec_55_2_Supported)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_2_CLK_ENABLE, 0);
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
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] demodEnablePtr MXL_ENABLE or MXL_DISABLE
 *
 *  @apibrief   This API returns the power state of specified demodulator and mpeg output.
 *
 *  @usage      This API can be called any time.
 *
 *  @equ261     MXL_DEMOD_PSM_REQ
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodEnable(
    UINT8     devId,                        
    MXL_HRCLS_DMD_ID_E demodId,            
    MXL_BOOL_E* demodEnablePtr            
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (demodEnablePtr) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status = MxL_HRCLS_Ctrl_GetDemodClockEnable(devId, demodId, demodEnablePtr);
      devContextPtr->demods[(UINT8) demodId].enabled = *demodEnablePtr;
    } else status = MXL_NOT_INITIALIZED;
    MXLEXITAPI(MxL_HRCLS_DEBUG("demodEnable=%d\n", (*demodEnablePtr)););
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *  @param[out] qamLockStatusPtr  Demod Qam lock status,  MXL_TRUE=lock, MXL_FALSE=unlock.
 *  Pass NULL if don't care
 *  @param[out] fecLockStatusPtr  Demod Fec lock status,  MXL_TRUE=lock, MXL_FALSE=unlock.
 *  Pass NULL if don't care
 *  @param[out] mpegLockStatusPtr Demod Mpeg lock status, MXL_TRUE=lock, MXL_FALSE=unlock.
 *  Pass NULL if don't care
 *  @param[out] retuneRequiredPtr Demod Retune Required, MXL_TRUE=required, MXL_FALSE=not
 *  required. Pass NULL if don't care
 *
 *  @apibrief   \n It retrieves QAM, FEC and MPEG lock status of the addressed demod.
 *              \n It also indicates whether a retune is required.
 *
 *  @usage      \n After tuning the channel, Demod QAM/FEC/MPEG lock statuses can be retrieved by this API.
 *              \n Pass NULL as a parameter if corresponding lock status is don't care.
 *              \n In case RetuneRequired flag is set, the API - MXL_DEMOD_RESTART_CFG should be called.
 *              \n A second call to this API should be made to retrieve the QAM/FEC/MPEG lock statuses again.
 *
 *  @equ261     MXL_DEMOD_ALL_LOCK_STATUS_REQ
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodAllLockStatus(
    UINT8     devId,                    
    MXL_HRCLS_DMD_ID_E demodId,        
    MXL_BOOL_E* qamLockStatusPtr,     
    MXL_BOOL_E* fecLockStatusPtr,    
    MXL_BOOL_E* mpegLockStatusPtr,  
    MXL_BOOL_E* retuneRequiredPtr  
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_ANNEX_TYPE_E annexType = MXL_HRCLS_ANNEX_A;
  UINT16 readBack = 0;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXL_BOOL_E qamLockStatus = MXL_FALSE, fecLockStatus = MXL_FALSE, mpegLockStatus = MXL_FALSE, retuneRequired = MXL_FALSE;
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt) &&
    ((qamLockStatusPtr) || (fecLockStatusPtr) || (mpegLockStatusPtr) || (retuneRequiredPtr)))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 

      if (devContextPtr->demods[(UINT8) demodId].enabled == MXL_TRUE)
      {
        retuneRequired = MXL_FALSE; 

        // QAM
        status |= (UINT8) MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_QAM_LOCK, &readBack);
        qamLockStatus = (readBack & 0x0001)?MXL_TRUE:MXL_FALSE;

        // FEC
        /* Check Annex Type and QAM TYPE  */
        status |= MxL_HRCLS_Ctrl_GetDemodCurrentAnnexAndQam(devContextPtr, demodId, &annexType, NULL);

        status |= (UINT8) MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_FECB_TR_SYNC), &readBack);
        fecLockStatus = ((readBack >> 1) & 0x0001)?MXL_TRUE:MXL_FALSE;
        mpegLockStatus = (readBack & 0x0001)?MXL_TRUE:MXL_FALSE;

        if (devContextPtr->demods[(UINT8) demodId].oobMode == MXL_TRUE)
        {
          if (OOB_SCTE_55_2_TYPE == devContextPtr->oobType)
          {
            status |= (UINT8) MxL_HRCLS_Ctrl_ReadRegisterField(devId, LOCK_SIGNAL_RB, &readBack);
          }
          else
          {
            status |= (UINT8) MxL_HRCLS_Ctrl_ReadRegisterField(devId, OOB_SYNC_MPEG_RB, &readBack);
          }
          fecLockStatus = (readBack)?MXL_TRUE:MXL_FALSE;
          mpegLockStatus = fecLockStatus;
        }
        else
        if (annexType == MXL_HRCLS_ANNEX_A)
        {
          // Flip lock bit for MXL_HRCLS_ANNEX_A type
          fecLockStatus = (MXL_TRUE == fecLockStatus)?MXL_FALSE:MXL_TRUE;
        }

        /* Check Retune Required */
        if ((MXL_TRUE == fecLockStatus) && (MXL_TRUE == mpegLockStatus))
        {
          /* Read EQU_FAGC_GAIN */
          if (FIRMWARE_STATUS_LOADED == devContextPtr->firmwareStatus)
          {
            status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MAILBOX_REG_FAGC_BASE + 2*demodId, &readBack);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_ADDRESS, (UINT16) MXL_HRCLS_READ_EQU_FAGC_GAIN_ADDRESS);
            status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_DATA, &readBack);
          }
          readBack &= 0x0FFF; /* EQU_FAGC_GAIN[11:0] */

          if (readBack < 9)
          {
            retuneRequired = MXL_TRUE;
            qamLockStatus  = MXL_FALSE;
            fecLockStatus  = MXL_FALSE;
            mpegLockStatus = MXL_FALSE;
          }
        }
        MXLDBG2(
        MxL_HRCLS_DEBUG("devId=0x%x, DemodId=%d, QamLockStatus:%d, FecLockStatus:%d, MpegLockStatus:%d, RetuneRequired:%d\n",
            devId, demodId, qamLockStatus, fecLockStatus, mpegLockStatus, retuneRequired);
        );
      } 
      else 
      {
        status = MXL_NOT_READY; 
        MXLERR(MxL_HRCLS_ERROR("Demodulator has to be enabled first\n"););
      }

      if (status == MXL_SUCCESS)
      {
        if (qamLockStatusPtr) *qamLockStatusPtr = qamLockStatus;
        if (fecLockStatusPtr) *fecLockStatusPtr = fecLockStatus;
        if (mpegLockStatusPtr) *mpegLockStatusPtr = mpegLockStatus;
        if (retuneRequiredPtr) *retuneRequiredPtr = retuneRequired;
      }

    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_RestartDemod - MXL_DEMOD_RESTART_CFG
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 05/06/2011
--|
--| DESCRIPTION   : Restart Demod (config data not lost)
--|                 
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodRestart(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId)
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  if ((devContextPtr)  && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      HOST_COMMAND_T demodRestartCfg;
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 

      status = MxL_HRCLS_Ctrl_ConfigDemodClockEnable(devContextPtr, demodId, MXL_TRUE); 
      if (status == MXL_SUCCESS)
      {

        /* Form command payload */
        demodRestartCfg.data[0] = (UINT8)demodId; // Only least 4 bits used right now.
        demodRestartCfg.data[0] |= ((devContextPtr->demods[(UINT8) demodId].autoSpectrumInversionEnabled == MXL_TRUE)?0x10:0x00);
        demodRestartCfg.payloadLen = 1;
        demodRestartCfg.syncChar = 0;
        demodRestartCfg.commandId = 0;
        demodRestartCfg.payloadCheckSum = 0;

        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &demodRestartCfg, HOST_CMD_CFG_DEMOD_RESTART, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        if (status == MXL_SUCCESS)
        {
          status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EQU_FREQ_SWEEP_LIMIT, (UINT16) devContextPtr->demods[(UINT8) demodId].demodFreqRangeIdx);
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/*---------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgDemodInvertSpectrum- MXL_DEMOD_INVERT_SPECTRUM_CFG
--|
--| AUTHOR        : SidS 
--|
--| DATE CREATED  : 05/10/2011
--|
--| DESCRIPTION   : This function inverts the current carrier offset 
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodInvertSpectrum(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId)
{
  UINT8  status = MXL_SUCCESS;
  UINT16 regData = 0;
  MXL_BOOL_E  adcIqFlip = MXL_DISABLE;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr)  && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      /* Set Carrier Offset Address and clear auto-increment bit*/ 
      if (FIRMWARE_STATUS_LOADED == devContextPtr->firmwareStatus)
      {
        HOST_COMMAND_T carrierOffsetFlipCfg;

        /* Form command payload */
        carrierOffsetFlipCfg.data[0]    = (UINT8)demodId; // Only least 4 bits used right now.
        carrierOffsetFlipCfg.payloadLen = 1;
        carrierOffsetFlipCfg.syncChar = 0;
        carrierOffsetFlipCfg.commandId = 0;
        carrierOffsetFlipCfg.payloadCheckSum = 0;

        /* Send host command */
        status |= MxL_HRCLS_Ctrl_SendHostCommand(devId, &carrierOffsetFlipCfg, HOST_CMD_CFG_DEMOD_CARRIER_OFFSET_FLIP, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
      }
      else
      {
        status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_ADDRESS), MXL_HRCLS_READ_CARRIER_OFFSET_ADDRESS);
        /* Read Carrier Offset */ 
        status |= MxL_HRCLS_Ctrl_ReadDemodRegisterField(devId, demodId, DMD0_EXTENDED_SPACE_DATA, &regData);
        /* Flip and write Carrier Offset */ 
        status |= MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_EXTENDED_SPACE_DATA), (UINT16)(-(SINT16)regData));
      }
      /* Get IQ_SWAP settings */ 
      status |= MxL_HRCLS_Ctrl_GetDemodAdcIqSwap(devContextPtr, demodId, &adcIqFlip);
      /* Flip and write IQ_SWAP */ 
      status |= MxL_HRCLS_Ctrl_SetDemodAdcIqSwap(
          devContextPtr,
          demodId,
          (adcIqFlip==MXL_ENABLE)?MXL_DISABLE:MXL_ENABLE
          );
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E)status;
}

/*---------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgDemodInterruptOnPid
--|
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 05/31/2012
--|
--| DESCRIPTION   : This function configures interrupt on PID for given demodulator 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED
--|
--|-------------------------------------------------------------------------------------*/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodInterruptOnPid(
    UINT8               devId,
    MXL_HRCLS_DMD_ID_E  demodId,
    MXL_BOOL_E          enable,
    UINT16 *            pidArrayPtr,
    UINT8               pidCnt
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(
  {
    UINT8 i;
    MxL_HRCLS_DEBUG("demodId=%d, enable=%d, pidCnt=%d\n", demodId, enable, pidCnt);
    if ((enable == MXL_TRUE) && (pidCnt) && (pidArrayPtr))
    {
      for (i = 0; i < pidCnt; i++) MxL_HRCLS_DEBUG("0x%04x ", pidArrayPtr[i]);
      MxL_HRCLS_DEBUG("\n");
    }
  }
  );
  if ((devContextPtr) && (pidCnt <= MXL_HRCLS_INTERRUPT_ON_PID_CNT) && ((enable == MXL_FALSE) || ((pidArrayPtr) && (pidCnt))))
  {
    if (demodId == devContextPtr->wakeOnWanDemod)
    {
      if (devContextPtr->driverInitialized)
      {
        if (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED)
        {
          HOST_COMMAND_T pidFilter;
          UINT8 i;

          pidFilter.data[0] = (enable == MXL_TRUE)?pidCnt:0;
          pidFilter.data[1] = 0;
          if (enable == MXL_TRUE)
          {
            for (i = 0; i < pidCnt; i++)
            {
              pidFilter.data[i*2 + 2] = (pidArrayPtr[i] >> 8) & 0xff;
              pidFilter.data[i*2 + 3] = (pidArrayPtr[i] & 0xff);
            }         
            pidFilter.payloadLen = 2 + (2 * pidCnt);
          } else pidFilter.payloadLen = 2;

          status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &pidFilter, HOST_CMD_PIF_CFG_ALL_PIDFILTERS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        } else status = MXL_NOT_SUPPORTED;
      } else status = MXL_NOT_INITIALIZED;
    } 
    else 
    {
      MXLERR(MxL_HRCLS_DEBUG("Demod%d does not support interrupt on PID. Use demod%d\n", demodId, devContextPtr->wakeOnWanDemod););
      status = MXL_NOT_SUPPORTED;
    }
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);

  return status;
}

/*---------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqDemodCapturedTsPacket
--|
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 07/02/2012
--|
--| DESCRIPTION   : This function reads captured TS packet. Use it after getting
--|                 an interrupt from 'interrupt on pid' source 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodCapturedTsPacket(
    UINT8                 devId,
    MXL_HRCLS_DMD_ID_E    demodId,
    UINT16 *              bufferPtr
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, bufferPtr=%p\n", demodId, bufferPtr););

  if ((devContextPtr) && (bufferPtr))
  {
    if (demodId == devContextPtr->wakeOnWanDemod)
    {
      if (devContextPtr->driverInitialized)
      {
        status = MxL_HRCLS_Ctrl_ReadTable16(devId, MXL_HRCLS_TABLE_ID_PID_TS_PACKET, bufferPtr, MXL_HRCLS_TS_BUFFER_NHALFWORDS, 0);
MXLDBG2(
        if (1)
        {
          UINT8 i;
          MxL_HRCLS_DEBUG("Captured TS packet:\n");
          for (i = 0; i < MXL_HRCLS_TS_BUFFER_NHALFWORDS; i++)
          {
            if ((i % 5) == 0)
            {
              MxL_HRCLS_DEBUG("\n%3d: ", i*2);
            }
            MxL_HRCLS_DEBUG("%02x %02x ", (bufferPtr[i] >> 8) & 0xff, bufferPtr[i] & 0xff);
          }
          MxL_HRCLS_DEBUG("\n");
        }
        )
      } else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId, status);
  return (status);
}

/*---------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgDemodFreqSearchRange
--|
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 07/10/2012
--|
--| DESCRIPTION   : 
--|                
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodFreqSearchRange(
    UINT8               devId,
    MXL_HRCLS_DMD_ID_E  demodId,
    UINT8               searchRangeIdx
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d, searchRangeIdx=%d\n", demodId, searchRangeIdx););

  if ((devContextPtr) && (demodId < devContextPtr->demodsCnt) && (searchRangeIdx <= MXL_HRCLS_DEMOD_FREQ_SEARCH_RANGE_IDX_MAX))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
      status = MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_EQU_FREQ_SWEEP_LIMIT, searchRangeIdx);
      devContextPtr->demods[(UINT8) demodId].demodFreqRangeIdx = searchRangeIdx;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  
  MXLEXITAPISTR(devId, status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID number
 *
 *  @apibrief   This API returns the constellation value of the specified demod.
 *
 *  @usage      This API can be called any time.
 *
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodConstellationValue(
    UINT8     devId,                        
    MXL_HRCLS_DMD_ID_E demodId,
    SINT16 *iVal,
    SINT16 *qVal
    )
{
  MXL_STATUS_E status = MXL_FAILURE;
  UINT16 softDecValueReg, regVal;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););
  if ((devContextPtr) && (iVal && qVal) && (demodId < devContextPtr->demodsCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId);
      /* Check the correctness of MxL_HRCLS_Ctrl_ConvertDemodIdToAddr() */
      if (devContextPtr->demods[(UINT8) demodId].enabled == MXL_TRUE)
      {
        softDecValueReg = MxL_HRCLS_Ctrl_ConvertDemodIdToAddr(demodId, EXTRACT_ADDR(DMD0_SOFTDEC_VALUE));
        do
        {
          status = MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, softDecValueReg, &regVal);
        } while ((status == MXL_SUCCESS) && (((regVal & 0x0400) >> 10) == 1));
        if (status == MXL_SUCCESS)
        {
          *iVal = (SINT16)(regVal & 0x03FF);
          status = MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, softDecValueReg, &regVal);
          *qVal = (SINT16)(regVal & 0x03FF);
        }
      } else status = MXL_NOT_READY;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return status;
}

#endif // of _MXL_HRCLS_DEMOD_ENABLED_

