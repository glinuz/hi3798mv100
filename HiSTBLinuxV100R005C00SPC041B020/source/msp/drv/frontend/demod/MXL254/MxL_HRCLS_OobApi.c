#include "MxL_HRCLS_Common.h"
 
#ifdef _MXL_HRCLS_OOB_ENABLED_

#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

#define MXL_HERMOD_V1_OOB_OUTPUT_ID 8

static REG_CTRL_INFO_T MxL_OobAciMfCoef[] =
{
  {0x1910, 0x001F, 0x0000},  /* ACI_COEF_ADDR */
  {0x1918, 0x003F, 0x0000},  /* MF_COEF_ADDR */
  {0x1910, 0x8000, 0x8000},  /* ACI_AUTO_INC */
  {0x1918, 0x8000, 0x8000},  /* MF_AUTO_INC */
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_OobAciCoef_0_772MHz[] =     
{
  {0x1914, 0xFFFF, 0x0004}, /* ACI_COEF_DATA */
  {0x1914, 0xFFFF, 0x0009}, 
  {0x1914, 0xFFFF, 0x000d}, 
  {0x1914, 0xFFFF, 0x000f}, 
  {0x1914, 0xFFFF, 0x000d}, 
  {0x1914, 0xFFFF, 0x0005}, 
  {0x1914, 0xFFFF, 0x1ff5}, 
  {0x1914, 0xFFFF, 0x1fe0}, 
  {0x1914, 0xFFFF, 0x1fca}, 
  {0x1914, 0xFFFF, 0x1fb8}, 
  {0x1914, 0xFFFF, 0x1fb3}, 
  {0x1914, 0xFFFF, 0x1fc1}, 
  {0x1914, 0xFFFF, 0x1fe8}, 
  {0x1914, 0xFFFF, 0x002b}, 
  {0x1914, 0xFFFF, 0x0085}, 
  {0x1914, 0xFFFF, 0x00f0}, 
  {0x1914, 0xFFFF, 0x0160}, 
  {0x1914, 0xFFFF, 0x01c5}, 
  {0x1914, 0xFFFF, 0x0212}, 
  {0x1914, 0xFFFF, 0x023c}, 
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_OobAciCoef_1_024MHz[] =     
{
  {0x1914, 0xFFFF, 0x1ff3}, /* ACI_COEF_DATA */ 
  {0x1914, 0xFFFF, 0x1ff5}, 
  {0x1914, 0xFFFF, 0x1ffd}, 
  {0x1914, 0xFFFF, 0x000b}, 
  {0x1914, 0xFFFF, 0x001a}, 
  {0x1914, 0xFFFF, 0x0025}, 
  {0x1914, 0xFFFF, 0x0026}, 
  {0x1914, 0xFFFF, 0x0017}, 
  {0x1914, 0xFFFF, 0x1ff8}, 
  {0x1914, 0xFFFF, 0x1fcf}, 
  {0x1914, 0xFFFF, 0x1fa6}, 
  {0x1914, 0xFFFF, 0x1f8e}, 
  {0x1914, 0xFFFF, 0x1f95}, 
  {0x1914, 0xFFFF, 0x1fc9}, 
  {0x1914, 0xFFFF, 0x002b}, 
  {0x1914, 0xFFFF, 0x00b5}, 
  {0x1914, 0xFFFF, 0x0154}, 
  {0x1914, 0xFFFF, 0x01ef}, 
  {0x1914, 0xFFFF, 0x026a}, 
  {0x1914, 0xFFFF, 0x02ae}, 
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_OobAciCoef_1_544MHz[] =     
{
  {0x1914, 0xFFFF, 0x0002}, /* ACI_COEF_DATA */
  {0x1914, 0xFFFF, 0x0003},
  {0x1914, 0xFFFF, 0x0003},
  {0x1914, 0xFFFF, 0x1ffe},
  {0x1914, 0xFFFF, 0x1ff5},
  {0x1914, 0xFFFF, 0x1fef},
  {0x1914, 0xFFFF, 0x1ff3},
  {0x1914, 0xFFFF, 0x0005},
  {0x1914, 0xFFFF, 0x0022},
  {0x1914, 0xFFFF, 0x0038},
  {0x1914, 0xFFFF, 0x0031},
  {0x1914, 0xFFFF, 0x0000},
  {0x1914, 0xFFFF, 0x1fb1},
  {0x1914, 0xFFFF, 0x1f6b},
  {0x1914, 0xFFFF, 0x1f67},
  {0x1914, 0xFFFF, 0x1fd3},
  {0x1914, 0xFFFF, 0x00b5},
  {0x1914, 0xFFFF, 0x01e1},
  {0x1914, 0xFFFF, 0x02fe},
  {0x1914, 0xFFFF, 0x03ab},
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_OobMfCoef_0_772_1_1544MHz[] =     
{
  {0x191C, 0xFFFF, 0x0001}, /* MF_COEF_DATA */
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x1FFE},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x1FFE},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x1FFD},  
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x1FFD},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x1FFD},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0003},
  {0x191C, 0xFFFF, 0x1FFC},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0004},
  {0x191C, 0xFFFF, 0x1FFC},
  {0x191C, 0xFFFF, 0x1FFF},
  {0x191C, 0xFFFF, 0x0006},
  {0x191C, 0xFFFF, 0x1FFB},
  {0x191C, 0xFFFF, 0x1FFD},
  {0x191C, 0xFFFF, 0x0007},
  {0x191C, 0xFFFF, 0x1FFC},
  {0x191C, 0xFFFF, 0x1FFC},
  {0x191C, 0xFFFF, 0x000A},
  {0x191C, 0xFFFF, 0x1FFA},
  {0x191C, 0xFFFF, 0x1FF8},
  {0x191C, 0xFFFF, 0x000E},
  {0x191C, 0xFFFF, 0x1FFD},
  {0x191C, 0xFFFF, 0x1FF5},
  {0x191C, 0xFFFF, 0x0013},
  {0x191C, 0xFFFF, 0x1FF9},
  {0x191C, 0xFFFF, 0x1FE7},
  {0x191C, 0xFFFF, 0x0021},
  {0x191C, 0xFFFF, 0x0007},
  {0x191C, 0xFFFF, 0x1FD7},
  {0x191C, 0xFFFF, 0x002D},
  {0x191C, 0xFFFF, 0x1FF9},
  {0x191C, 0xFFFF, 0x1F83},
  {0x191C, 0xFFFF, 0x00AA},
  {0x191C, 0xFFFF, 0x00D6},
  {0x191C, 0xFFFF, 0x1DBB},
  {0x191C, 0xFFFF, 0x1EE4},
  {0x191C, 0xFFFF, 0x0919},
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_OobMfCoef_1_024MHz[] =     
{
  {0x191C, 0xFFFF, 0x0000}, /* MF_COEF_DATA */
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x03ff},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x03fe},
  {0x191C, 0xFFFF, 0x0000},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x03ff},
  {0x191C, 0xFFFF, 0x03fe},
  {0x191C, 0xFFFF, 0x03ff},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x0001},
  {0x191C, 0xFFFF, 0x03fd},
  {0x191C, 0xFFFF, 0x03ff},
  {0x191C, 0xFFFF, 0x0007},
  {0x191C, 0xFFFF, 0x0002},
  {0x191C, 0xFFFF, 0x1ff4},
  {0x191C, 0xFFFF, 0x1ffb},
  {0x191C, 0xFFFF, 0x0015},
  {0x191C, 0xFFFF, 0x000b},
  {0x191C, 0xFFFF, 0x1fd1},
  {0x191C, 0xFFFF, 0x1fe9},
  {0x191C, 0xFFFF, 0x0068},
  {0x191C, 0xFFFF, 0x0032},
  {0x191C, 0xFFFF, 0x1f2e},
  {0x191C, 0xFFFF, 0x1f97},
  {0x191C, 0xFFFF, 0x018a},
  {0x191C, 0xFFFF, 0x00b8},
  {0x191C, 0xFFFF, 0x1d07},
  {0x191C, 0xFFFF, 0x1efb},
  {0x191C, 0xFFFF, 0x0968},
  {0,    0,    0}
};


static REG_CTRL_INFO_T MxL_HRCLS_DemodEqualizerSpeedUpOob_0_772MHz[] = 
{
  {0x1884, 0xFFFF, 0xD5F9},
  {0x1940, 0xFFFF, 0x083A},     // 0x1940 - check this value
  {0x18A0, 0xFFFF, 0x8508},
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_HRCLS_DemodEqualizerSpeedUpOob_1_024MHz[] = 
{
  {0x1884, 0xFFFF, 0xD5FA},
  {0x1940, 0xFFFF, 0x083A},     // 0x1940 - check this value
  {0x18A0, 0xFFFF, 0x8508},
  {0,    0,    0}
};

static REG_CTRL_INFO_T MxL_HRCLS_DemodEqualizerSpeedUpOob_1_544MHz[] = 
{
  {0x1884, 0xFFFF, 0xD5F9},
  {0x1940, 0xFFFF, 0x083A},     // 0x1940 - check this value
  {0x18A0, 0xFFFF, 0x8508},
  {0,    0,    0}
};

static SINT16 MxL_HRCLS_DfeCsfFilerCoeffsOOB[] = 
{
   -13,
    25,
   141,
    83,
  -425,
  -818,
   260,
  3131,
  5810,
};

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConfigDemodEqualizerOOB
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
static MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodEqualizerOOB(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, MXL_HRCLS_OOB_SYM_RATE_E oobSymbolRate)
{
  UINT8 status = MXL_SUCCESS;

  PREG_CTRL_INFO_T programDataPtr = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("demodId=%d, oobSymbolRate=%d\n", demodId, oobSymbolRate););
  switch (oobSymbolRate)
  {
    case MXL_HRCLS_SYM_RATE_0_772MHz:
      programDataPtr = MxL_HRCLS_DemodEqualizerSpeedUpOob_0_772MHz;
      break;

    case MXL_HRCLS_SYM_RATE_1_024MHz:
      programDataPtr = MxL_HRCLS_DemodEqualizerSpeedUpOob_1_024MHz;
      break;

    case MXL_HRCLS_SYM_RATE_1_544MHz:
      programDataPtr = MxL_HRCLS_DemodEqualizerSpeedUpOob_1_544MHz;
      break;

    default:
      return MXL_FAILURE;
  }

  status |= MxL_HRCLS_Ctrl_ProgramRegisters(devId, demodId, programDataPtr);

  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

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
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("oobType=%d\n", oobType););
  if (devContextPtr)
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
        if (OOB_SCTE_55_2_TYPE == oobType)
        {
          if (MXL_TRUE == devContextPtr->oobFec_55_2_Supported)
          {
            devContextPtr->oobType = oobType;
          }
          else
          {
            status = MXL_NOT_SUPPORTED;
            MXLERR(MxL_HRCLS_ERROR("FEC for OOB Type 55-2 not supported.\n"););
          }
        }
        else
        {
          devContextPtr->oobType = oobType;
        }
      } else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status)
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConfigDfeCsfCoeffOOB
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 02/05/2013
--|
--| DESCRIPTION   : Config DFE Channel select filter coefficients for OOB.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDfeCsfCoeffOOB(UINT8 devId, MXL_HRCLS_CHAN_ID_E chanId)
{
  UINT8 status = MXL_SUCCESS;
  HOST_COMMAND_T command;
  UINT8 i;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("Physical channel id =%d\n", chanId););

  /* Form command payload */
  command.data[0] = (UINT8) (chanId); 
  for (i = 0; i < 9; i++)
  {
    command.data[1 + (2*i) + 0] = (UINT8) (MxL_HRCLS_DfeCsfFilerCoeffsOOB[i] & 0x00ff);
    command.data[1 + (2*i) + 1] = (UINT8) ((MxL_HRCLS_DfeCsfFilerCoeffsOOB[i] & 0xff00) >> 8);
  }
  command.payloadLen = 19;
  command.payloadCheckSum = 0;
  command.syncChar = 0;
  command.commandId = 0;
  /* Send host command */
  status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &command, HOST_CMD_CFG_DEV_CHAN_CSF_COEF_OOB, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  oobSymbRate symbol type
 *
 *  @apibrief   \n Even though Auto-detect mode for QAM Type is available in hardware, the
 *                 symbol rate needs to be configured with a corresponding Annex Type.
 *              \n
 *              \n For Annex A, the same symbol rate will be used for all constellations.
 *              \n But for Annex B, the 2 symbol rates need to be provided for auto detection logic : 64QAM and 256 QAM.
 *              \n
 *              \n   - When symbol type MXL_HRCLS_SYM_TYPE_USER_DEFINED_J83A is select
 *              \n     - Only SymbolRate needs to be provided by caller
 *              \n   - When symbol type MXL_HRCLS_SYM_TYPE_USER_DEFINED_J83B is selected
 *              \n     - Both SymbolRate and SymbolRate256 should be provided by caller with a value in the following range: 0 <  Symbol rate < 7.125MHz
 *              \n   - When MXL_HRCLS_SYM_TYPE_J83A or MXL_HRCLS_SYM_TYPE_J83B is selected, the following predefined symbol rate(s) below shall be used
 *              \n     - J.83 A :  6.89MHz
 *              \n     - J.83 B :  5.056941MHz for 64QAM and  5.360537MHz for 256 QAM
 *              \n     - In this case, SymbolRate, SymbolRate256, and OobSymbolRate will be ignored
 *              [TBD] Above explanation modified from original at Page 26.
 *
 *  @usage      See example [TBD]
 *
 *  @equ261     MXL_DEMOD_SYMBOL_RATE_CFG
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodSymbolRateOOB(
    UINT8     devId,                      
    MXL_HRCLS_DMD_ID_E demodId,          
    MXL_HRCLS_OOB_SYM_RATE_E oobSymbRate 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_CHAN_ID_E physicalChanId;
  UINT32 symbolRate[SYM_NUM_B_BANK];
  UINT8  numBank, bank[SYM_NUM_B_BANK];
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("DemodId=%d, SymbolRate=%u\n", demodId, oobSymbRate););
  if ((devContextPtr) && (demodId == devContextPtr->oobDemod))
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
        UINT16 control;
        PREG_CTRL_INFO_T OobAciFilterCoefDataPtr = 0;
        PREG_CTRL_INFO_T OobMfFilterCoefDataPtr = 0;

        demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 

        numBank = 1;
        bank[0] = 0; /* Annex-A bank */
        switch (oobSymbRate)
        {
          case MXL_HRCLS_SYM_RATE_0_772MHz:
            symbolRate[0] = 772000U/* 0x03139094 */;
              OobAciFilterCoefDataPtr = MxL_OobAciCoef_0_772MHz;
              OobMfFilterCoefDataPtr = MxL_OobMfCoef_0_772_1_1544MHz;
            break;

          case MXL_HRCLS_SYM_RATE_1_024MHz:
              symbolRate[0] = 1024000U/* 0x0251C000 */ ;
              OobAciFilterCoefDataPtr = MxL_OobAciCoef_1_024MHz;
              OobMfFilterCoefDataPtr = MxL_OobMfCoef_1_024MHz;
            break;

          case MXL_HRCLS_SYM_RATE_1_544MHz:
            symbolRate[0] = 1544000U/* 0x0189C84A */ ;
              OobAciFilterCoefDataPtr = MxL_OobAciCoef_1_544MHz;
              OobMfFilterCoefDataPtr = MxL_OobMfCoef_0_772_1_1544MHz;
            break;

          default:
            return MXL_FAILURE;
        }

        status |= (UINT8) MxL_HRCLS_Ctrl_ConfigDemodResampRatio(devId, demodId, symbolRate, bank, numBank);

        /* Set Annex-A and MXL_HRCLS_QPSK  */
        control = MXL_HRCLS_HW_QPSK | (MXL_HRCLS_HW_ANNEX_A << EXTRACT_LSBLOC(DMD0_QAM_ANNEX_TYPE));
        status |= (UINT8) MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_QAM_ANNEX_TYPE), control);
        if (status == MXL_SUCCESS)
        { 
          devContextPtr->demods[(UINT8) demodId].curAnnexType = MXL_HRCLS_ANNEX_A;
          devContextPtr->demods[(UINT8) demodId].curQamType = MXL_HRCLS_QPSK;
        }

        /* Watch dog disable <12> */
        status |= (UINT8) MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_QAM_WATCHDOG_ENABLE, 0);

        /* Enable Custom ACI COEF <5> */
        /* Enable Matched Filter custom coefficients <6> */
        status |= (UINT8) MxL_HRCLS_Ctrl_ReadDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_ACI_CUSTOM_COEF_ENABLE), &control);
        control |= (((UINT8) 1 << EXTRACT_LSBLOC(DMD0_ACI_CUSTOM_COEF_ENABLE)) | ((UINT8) 1 << EXTRACT_LSBLOC(DMD0_MF_CUSTOM_COEF_ENABLE)));
        status |= (UINT8) MxL_HRCLS_Ctrl_WriteDemodRegister(devId, demodId, EXTRACT_ADDR(DMD0_ACI_CUSTOM_COEF_ENABLE), control);

        /* Configure control before writing filter data */
        status |= (UINT8) MxL_HRCLS_Ctrl_ProgramRegisters(devId, demodId, MxL_OobAciMfCoef);

        /* Program multiple registers for the specified function */
        status |= (UINT8) MxL_HRCLS_Ctrl_ProgramRegisters(devId, demodId, OobAciFilterCoefDataPtr);
        status |= (UINT8) MxL_HRCLS_Ctrl_ProgramRegisters(devId, demodId, OobMfFilterCoefDataPtr);

        if (OOB_SCTE_55_2_TYPE == devContextPtr->oobType)
        {
          // status |= (UINT8) MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_OC_DIFF_MODE, 0);  // Default.
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_2_CLK_ENABLE, 1); // Turns on clock to FEC block
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_FEC_MODE, 1);
        }
        else
        {
          /* For Alternate differential mode in 55-1, IQ flip API is called */
          // Default.
          // status |= (UINT8) MxL_HRCLS_Ctrl_UpdateDemodRegisterField(devId, demodId, DMD0_OC_DIFF_MODE, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_MPEG_EN, 1); // Also Turns on clock to FEC block
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_FEC_MODE, 0);
        }
        status |= (UINT8) MxL_HRCLS_Ctrl_ConfigDemodEqualizerOOB(devId, demodId, oobSymbRate);
        physicalChanId = MxL_HRCLS_Ctrl_GetOOBPhysicalChannelId(devContextPtr);
        devContextPtr->demods[(UINT8) demodId].oobMode = ((MXL_STATUS_E) status == MXL_SUCCESS)?MXL_ENABLE:MXL_DISABLE;
        if (physicalChanId < MXL_HRCLS_CHAN_NB)
        {
          status |= MxL_HRCLS_Ctrl_ConfigDfeCsfCoeffOOB(devId, physicalChanId);
        }
      } else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status)
  return (MXL_STATUS_E) status;
}

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
                    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("DemodId=%d\n", demodId););
  if ((devContextPtr) && (demodId == devContextPtr->oobDemod))
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
	    // demodId not needed here
        // demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
        if (OOB_SCTE_55_2_TYPE == devContextPtr->oobType)
        { 
          /* Clear register <1> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, RS_CTR_CLEAR, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, RS_CTR_CLEAR, 0);
        }
        else
        {
          /* Freeze register <0> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_FREEZ, 1);

          /* Clear register <1> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_CLEAR, 1);

          // TODO: Optimize
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_FREEZ, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_CLEAR, 0);
        }
      }  else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}


/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  ErrorsPtr Structure with errors data
 *
 *  @apibrief   Returns errors count
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_ReqDemodErrorsOOB(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, MXL_HRCLS_OOB_ERRCNT_T * ErrorsPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT32 tmpData = 0;
  UINT16 readData;
  UINT32 total = 0, corr = 0, uncorr = 0;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("DemodId=%d\n", demodId););
  if ((devContextPtr) && (demodId == devContextPtr->oobDemod) && (ErrorsPtr))
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
        // demodId not needed here
        // demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId); 
        if (OOB_SCTE_55_2_TYPE == devContextPtr->oobType)
        { 
          /* BER & PER */
          /* total */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_TOTAL_PKT_CTR_RB_LO, &readData);
          tmpData = readData;
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_TOTAL_PKT_CTR_RB_HI, &readData);
          tmpData |= (readData << 16);
          total = tmpData;

          /* corr */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_CORR_PKT_ERR_CTR_RB_LO, &readData);
          tmpData = readData;
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_CORR_PKT_ERR_CTR_RB_HI, &readData);
          tmpData |= (readData << 16);
          corr = tmpData;

          /* uncorr */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_UNCORR_PKT_ERR_CTR_RB_LO, &readData);
          tmpData = readData;
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, RS_UNCORR_PKT_ERR_CTR_RB_HI, &readData);
          tmpData |= (readData << 16);
          uncorr = tmpData;

          /* Clear register <1> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, RS_CTR_CLEAR, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, RS_CTR_CLEAR, 0);
        }
        else
        {
          /* Freeze register <0> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_FREEZ, 1);

          /* BER & PER */
          /* total */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, OOB_RS_BLKCNT_RB, &readData);
          total = readData;

          /* corr */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, OOB_RS_BLK_ERRS_RB, &readData);
          corr = readData;

          /* uncorr */
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, OOB_RS_UNCORRECT_RB, &readData);
          uncorr = readData;

          /* Clear register <1> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_CLEAR, 1);

          // TODO: Optimize
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_FREEZ, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_RS_CLEAR, 0);
        }

        /*
           Pre FEC BER = ((corr + uncorr) * 2 * 4) / (total * 8 * 96)
           Post FEC BER = (uncorr * 2 * 4) / (total * 8 * 96)
           */
        ErrorsPtr->cntCorrected = (UINT64) corr;
        ErrorsPtr->cntUncorrected = (UINT64) uncorr;
        ErrorsPtr->cntReceived = (UINT64) total;

      }  else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *
 *  @apibrief   Reset demodulator
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodResetOOB(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId)
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("DemodId=%d\n", demodId););
  if ((devContextPtr) && (demodId == devContextPtr->oobDemod))
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
	    // demodId not needed
        // demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId);
        if (OOB_SCTE_55_2_TYPE == devContextPtr->oobType)
        {
          status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, OOB_2_SOFT_RESET);
        }
        else
        {
          status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, OOB_CTRL_SOFT_RESET);
        }
      } else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
   
  return (MXL_STATUS_E)status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  demodId Demodulator ID
 *  @param[in]  ParamPtr Configuration structure
 *
 *  @apibrief   This functions sets output signal parameters for OOB TS
 *
 *  @usage      See example [TBD]
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/
MXL_STATUS_E MxLWare_HRCLS_API_CfgDemodOutParamsOOB(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId,
                    MXL_HRCLS_OOB_CFG_T * ParamPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 eco4_value = 0;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  const MXL_HRCLS_FIELD_T oob_oe[] = {{OOB0_OE}, {OOB1_OE},
                                {OOB2_OE}, {OOB3_OE},
                                {OOB4_OE}, {OOB5_OE},
                                {OOB6_OE}, {OOB7_OE}, 
                                {OOB8_OE}};

  MXLENTERAPISTR(devId);
  MXLENTERAPI(
    MxL_HRCLS_DEBUG("DemodId=%d\n", demodId);
    MxL_HRCLS_DEBUG("Enable=%d\noobOutMode=%d\nclkPol=%d\nsyncPol=%d\nvalidPol=%d\nenablePn23Const=%d\noob3WireModeEnable=%d\nsyncPulseWidth=%d\npn23Feedback=%d\npn23SyncMode=%d\n",
      ParamPtr->enable,
      ParamPtr->oobOutMode, ParamPtr->clkPol, ParamPtr->syncPol, ParamPtr->validPol, ParamPtr->enablePn23Const, ParamPtr->oob3WireModeEnable, ParamPtr->syncPulseWidth,
      ParamPtr->pn23Feedback, ParamPtr->pn23SyncMode);    
    );
  if ((devContextPtr) && (demodId == devContextPtr->oobDemod))
  {
    if (devContextPtr->oobSupported == MXL_TRUE)
    {
      if (devContextPtr->driverInitialized)
      {
        demodId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(devContextPtr, demodId);
        /* Enable the output pins for QAM Mpeg */

        /* DIG_MPEG_OUTDATA_EN <1> = 1 */
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_MPEG_EN, 1);

        if ((OOB_SCTE_55_1_TYPE == devContextPtr->oobType) && (ParamPtr->oobOutMode != OOB_CRX_DRX_MODE))
        {
          /* 1 = Invert the OOB output MPEG clock, 0 = non invert <2> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_CTRL_CLK_INVT, (ParamPtr->clkPol == MXL_HRCLS_OOB_ACTIVE_LOW)?1:0);

          /* 1 = Invert the OOB output MPEG sync, 0 = non invert <3> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_CTRL_SYNC_INVT, (ParamPtr->syncPol == MXL_HRCLS_OOB_ACTIVE_LOW)?1:0);

          /* 1 = Invert the OOB output valid, 0 = non invert <4> */
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_CTRL_OUTPUT_VALIDINVT, (ParamPtr->validPol == MXL_HRCLS_OOB_ACTIVE_LOW)?1:0);
        }

        /* 1 = OOB PN23 constant enable, 0 = PN23 Gap over 0x47 <7> */
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_PN23_CONST_ENABLE, (ParamPtr->enablePn23Const == MXL_ENABLE)?1:0);
 
        if (MXL_TRUE == devContextPtr->oobTsOutputSupported)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_TS_MODE, (ParamPtr->oob3WireModeEnable == MXL_TRUE)?1:0);
        }

        if (OOB_SCTE_55_1_TYPE == devContextPtr->oobType)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_TS_SYNC_CLK, (ParamPtr->syncPulseWidth == MXL_HRCLS_OOB_SYNC_WIDTH_BYTE)?0:1);
        }

        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_FDBK, (ParamPtr->pn23Feedback == PN23_XOR)?1:0);
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, OOB_SYNC_LOSS_TYPE, (ParamPtr->pn23SyncMode == SYNC_MODE_LOSS)?1:0);

        if (MXL_TRUE == devContextPtr->oobTsOutputSupported)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, oob_oe[(UINT8) demodId].regAddr,
              oob_oe[(UINT8) demodId].lsbPos,
              oob_oe[(UINT8) demodId].fieldWidth,
              ((ParamPtr->enable == MXL_ENABLE) && (ParamPtr->oobOutMode == OOB_CRX_DRX_MODE))?1:0);
        }
        else
        {
          if ((3 <= devContextPtr->chipVersion) && (MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId))
          {
            // CRX/DRX via MPEG_8_DAT/VAL
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, oob_oe[MXL_HERMOD_V1_OOB_OUTPUT_ID].regAddr,
                oob_oe[MXL_HERMOD_V1_OOB_OUTPUT_ID].lsbPos,
                oob_oe[MXL_HERMOD_V1_OOB_OUTPUT_ID].fieldWidth,
                (ParamPtr->enable == MXL_ENABLE)?1:0);

            // ECO_4[1] used for CRX polarity
            status |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_4), &eco4_value);
            eco4_value &= ~(1U<<1);
            eco4_value |= (MXL_HRCLS_OOB_CLK_POSITIVE == ParamPtr->clkPol)?(0x0000):(1U<<1);
            status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4) , eco4_value);
          }
          else
          {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, oob_oe[(UINT8) demodId].regAddr,
              oob_oe[(UINT8) demodId].lsbPos,
              oob_oe[(UINT8) demodId].fieldWidth,
              ((ParamPtr->enable == MXL_ENABLE))?1:0);
          }
        }
      } else status = MXL_NOT_INITIALIZED;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_INVALID_PARAMETER; 
  MXLEXITAPISTR(devId, status);
    
  return (MXL_STATUS_E)status;
}
#endif

