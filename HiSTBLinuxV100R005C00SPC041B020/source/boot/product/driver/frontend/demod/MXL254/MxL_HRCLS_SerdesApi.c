/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_SerdesApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS SerDes APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"

#ifdef _MXL_HRCLS_SERDES_ENABLED_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

#define MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS 100
#define MXL_HRCLS_SERDES_DEFAULT_TUNE_BANDWIDTH 6
#define MXL_HRCLS_SERDES_DEFAULT_TUNE_FREQUENCY_HZ 666000000


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_UpdateSerdesRegisterField
--| 
--| AUTHOR        : Sailesh Kandula 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads and updates field 
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                            UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, 
                                                            UINT16 newValue)
{
  regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
  return MxL_HRCLS_Ctrl_UpdateRegisterField(devId, regAddr, lsbPos, fieldWidth, newValue);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0  (for Serdes)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                                  UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
   regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
   return(MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero( devId, regAddr, bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0  (for Serdes)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                                  UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
   regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
   return(MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne( devId, regAddr, bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SerdesTuneSlot2
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 03/15/2012
--|
--| DESCRIPTION   : Tune DFE channel that goes to slot 2 of serdes 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_SerdesTuneSlot2(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  MXL_STATUS_E status;
  MXL_HRCLS_RX_PWR_ACCURACY_E accuracy;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("SerDesId=%d, DFE channel=%d\n", serDesId, devContextPtr->serDesSlot2Chan[(UINT8) serDesId]););
  status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], NULL, &accuracy);
  if ((status == MXL_SUCCESS) && (accuracy == MXL_HRCLS_PWR_INVALID))
  {
    UINT64 timeNow, timeout;
    status = MxL_HRCLS_Ctrl_CfgTunerChanTune(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], 
                              MXL_HRCLS_SERDES_DEFAULT_TUNE_BANDWIDTH,
                              MXL_HRCLS_SERDES_DEFAULT_TUNE_FREQUENCY_HZ);
    if (status == MXL_SUCCESS)
    {
      MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      timeout = timeNow + ((UINT64) MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS * 1000);
      do
      {
        MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Average channel tunning time */ 
        status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], NULL, &accuracy);
        MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      } while ((accuracy == MXL_HRCLS_PWR_INVALID) && (status == MXL_SUCCESS) && (timeNow < timeout));
      status = ((status == MXL_SUCCESS) && (accuracy != MXL_HRCLS_PWR_INVALID))?MXL_SUCCESS:MXL_FAILURE;
    }
    MXLERR(
    if (status != MXL_SUCCESS)
    {
      MxL_HRCLS_ERROR("%s. Cannot tune channel %d for serDes lane %d, slot 2 (status %d, accuracy %d, timeout %dms)\n", __FUNCTION__, 
                    devContextPtr->serDesSlot2Chan[(UINT8) serDesId], serDesId, status, accuracy,
                    MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS);
    }
    )
  }
MXLDBG2(  
  else
  {
    if (accuracy != MXL_HRCLS_PWR_INVALID)
    {
      MxL_HRCLS_DEBUG("DFE channel %d already tuned (state=%d). Skipping\n", devContextPtr->serDesSlot2Chan[(UINT8) serDesId], accuracy);
    }
  }
  )
  MXLEXITSTR(status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SerdesEnableClock
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 03/15/2012
--|
--| DESCRIPTION   : Perform 2A sequence (PG) 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_SerdesEnableClock(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  UINT8 status = MXL_SUCCESS;
  UINT32 clockVal;

  status |= MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal);
  if (serDesId == MXL_HRCLS_SERDES_LANE0)
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_SERDES0_CLK_ENA, 1);
    clockVal |= (UINT32) (((UINT32) 1 << 10) | ((UINT32) 1 << 18) | ((UINT32) 1 << 19));
  }
  else
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_SERDES1_CLK_ENA, 1);
    clockVal |= (UINT32) (((UINT32) 1 << 10) | ((UINT32) 1 << 20) | ((UINT32) 1 << 21));
  }
  status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
  status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_WBCLK_EN, 1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_UPX_CLK_EN, 1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_WBCLK_RESET, 0);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_UPX_RESET, 0);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_CLR_DP_NBAND, 0); 

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesMode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure line rate for serdes phy. According to PG 12/01/2012
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesMode(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesId,
    MXL_HRCLS_SERDES_LINERATE_E lineRate,
    MXL_HRCLS_SERDES_MODE_E mode
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("devId=%d, serDesId=%d, lineRate=%d, mode=%d\n", devId, serDesId, lineRate, mode););

  MXL_HRCLS_SERDES_SWAP(serDesId);
  MXLDBG3(MxL_HRCLS_DEBUG("serDesId after swap=%d\n", serDesId););

  if ((devContextPtr) && (serDesId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 dataRb = 0;
      MXL_HRCLS_SERDES_LANE_ID_E otherSerdesLane;

      otherSerdesLane = (serDesId == MXL_HRCLS_SERDES_LANE1)?MXL_HRCLS_SERDES_LANE0:MXL_HRCLS_SERDES_LANE1;

      devContextPtr->serDesMode[(UINT8) serDesId] = mode;
      devContextPtr->serDesLineRate = lineRate;

      status |= MxL_HRCLS_Ctrl_InitializeDeviceParameters(devContextPtr);

      if(mode == MXL_HRCLS_SERDES_MODE_DISABLED)  //Disable SERDES
      {
        UINT32 clockVal = 0;
        // Serdes
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_IN_EN_INT, 0);
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 1);
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_OUT_EN_INT, 0);
        if (serDesId == MXL_HRCLS_SERDES_LANE0)
        {
          status |=  MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_RX_RESET, 1);
        }
        else
        {
          status |=  MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_RX_RESET, 1);
        }

        // Clock and Phy
        if ((status == MXL_SUCCESS) 
            && (MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal) == MXL_SUCCESS))
        {
          if(serDesId == MXL_HRCLS_SERDES_LANE1) 
          {
            clockVal &= (UINT32) (~(1 << 21));  // bit 21
          }
          else
          {
            clockVal &= (UINT32) (~(1 << 19));  // bit 19
          }
          status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal); 
        } else status = MXL_FAILURE;

        if (status == MXL_SUCCESS)
        {
          status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_RESET_ALL, 1);
          status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_REF_SSP_EN, 0);
          status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_TEST_POWERDOWN, 1);
        }

        if (status == MXL_SUCCESS)
        {
          if(serDesId == MXL_HRCLS_SERDES_LANE1)
          {
            clockVal &= (UINT32) (~(1 << 20)); /* Bit 20, serdes1_tx_clk */
          }
          else
          {
            clockVal &= (UINT32) (~(1 << 18)); /* Bit 18, serdes0_tx_clk */
          }
          status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
        }

        /* Disable resampler only if in HALF mode and both serdes are disabled or
         *  if the current serdes is in full mode */

        if ((status == MXL_SUCCESS) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_DISABLED))
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_RESET, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 0 );
          clockVal &= (UINT32) (~(1 << 10)); /* serdes_dfe */
          status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
        }
      }
      else  /* enable the serdes in either full mode or half mode*/
      {
        status = MxL_HRCLS_Ctrl_SerdesTuneSlot2(devContextPtr, serDesId);

        if (status == MXL_SUCCESS)
        {
          if(devContextPtr->serDesLineRate  == MXL_HRCLS_SERDES_LINERATE_5400MHZ)
          {
            UINT16 eco8_value = 0;
            MXL_BOOL_E mixed_mode = MXL_FALSE;

            // 1A
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_MODE, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_HALF_MODE_INT, (mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?1:0);

            // 1B
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_RATE, 1);

            // 2A
            status |= MxL_HRCLS_Ctrl_SerdesEnableClock(devId, serDesId);

            // 2B
            mixed_mode = (((mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE))
                || ((mode == MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)))?MXL_TRUE:MXL_FALSE;
            
            MXLDBG3(
              if (mixed_mode == MXL_TRUE)
              {
                MxL_HRCLS_DEBUG("Serdes mixed A mode detected\n");
              }
            )

            status |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_8), &eco8_value);
            if (mixed_mode == MXL_TRUE)
            {
              eco8_value |= (UINT16) (0x01);
            }
            else
            {
              eco8_value &= (UINT16) (~(0x01));
            }
            status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_8), eco8_value);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_MPLL_MULTIPLIER, (mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?50:100);
          }
          else
          {
            // 1A
            status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_MODE, 0);
            // 1B
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_RATE, 0);

            // 2A
            status |= MxL_HRCLS_Ctrl_SerdesEnableClock(devId, serDesId);

            // 2B
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_MPLL_MULTIPLIER, 96);
          }

          // 2C Program PHY 
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_TEST_POWERDOWN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_REF_SSP_EN, 1);
          if (serDesId == MXL_HRCLS_SERDES_LANE0)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_TX_RESET, 0);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_RX_RESET, 0);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_TX_RESET, 0);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_RX_RESET, 0);
          }

          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_RESET_ALL, 0);

          MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Delay for 10 ms */

          if (serDesId ==  MXL_HRCLS_SERDES_LANE0)
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES0_PHY_STATUS_READY, &dataRb);
          else
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES1_PHY_STATUS_READY, &dataRb);

          if ((status == MXL_SUCCESS) && (dataRb != 0))
          {
            // 3A
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_DOUT_SEL, 0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 0);
          }
          else
          {
            MXLERR(MxL_HRCLS_ERROR("Serdes PHY is not ready !!!\n"););
            status = MXL_FAILURE;
          }
        }
        else
        {
          MXLERR(MxL_HRCLS_ERROR("Cannot tune DFE channel %d for serdes slot 2\n", devContextPtr->serDesSlot2Chan[(UINT8) serDesId]););
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes parameters
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_HRCLS_SERDES_CODING_E dsCoding,
    MXL_BOOL_E scramblerEnable
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, dsCoding=%d, scramblerEnable=%d\n", serDesDsId, dsCoding, scramblerEnable););

  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5184MHZ
          && dsCoding == MXL_HRCLS_SERDES_CODING_CRC)
      {
        status = MXL_INVALID_PARAMETER;
        MXLERR(MxL_HRCLS_ERROR("[HRCLS] CRC cannot be enabled for 5184 mode\n"););
      }
      else
      {
        UINT16 useCrc = (dsCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
        status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_RS_USE_CRC, useCrc);

        if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5184MHZ)
        {
          /* Program the scrambler for type B only */
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_SCRAM_ENAB, (scramblerEnable == MXL_ENABLE)?1:0);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_SCRAM_SELF_SYNC, (scramblerEnable == MXL_ENABLE)?1:0);
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPrbsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes scrambler parameters
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPrbsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_BOOL_E  enable,
    MXL_HRCLS_SERDES_PRBS_MODE_E dsPrbsMode
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt) && 
         ((dsPrbsMode == MXL_HRCLS_SERDES_PRBS31) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS23) 
       || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS15) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS7) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS22)))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesLineRate  == MXL_HRCLS_SERDES_LINERATE_5400MHZ )
      {
        // Re-arranged according to tickets #120, #124
        if (enable == MXL_ENABLE)
        {
          if (dsPrbsMode != MXL_HRCLS_SERDES_PRBS22)
          {
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 1);

            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_RST, 0);
            
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_SEL, (UINT16) dsPrbsMode); /* Program PRBS mode */
          } else status = MXL_INVALID_PARAMETER;
        }
        else
        {
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_RST, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 0);
        }
      }
      else /* SERDES Type B */
      {
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 0);

        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_FRAMER_MODE_CISCO,  (enable == MXL_ENABLE)?4:2);
        // NO need to program dsPrbsMode for type B
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPreEmphasis 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes pre-emphasis
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPreEmphasis(
    UINT8     devId,                
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,  
    UINT8     preEmphasisSetting          
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      status =  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_PREEMPH_GEN3, (UINT16) preEmphasisSetting);
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsAmplitude 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes signal amplitude
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsAmplitude(
    UINT8     devId,                     
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    UINT8     dsAmplitude                 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
       status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_AMPLITUDE_GEN3, (UINT16) dsAmplitude);
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsCrossBar 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes crossbar setting
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsCrossBar(
    UINT8     devId,          
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_HRCLS_SERDES_SLOT_ID_E serDesSlotId,                
    MXL_HRCLS_CHAN_ID_E chanId                      
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, serDesSlotId=%d, chanId=%d\n", serDesDsId, serDesSlotId, chanId););

  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if (devContextPtr)
  {
    if ((serDesDsId < devContextPtr->serDesDSLanesCnt)
      && (serDesSlotId < devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId]) 
      && ((chanId < devContextPtr->dfeChanCount || chanId == MXL_HRCLS_CHAN_NB)))
    {
      if (devContextPtr->driverInitialized)
      {
        {
          MXL_HRCLS_CHAN_ID_E phyChanId;
          phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);

          if (phyChanId == MXL_HRCLS_CHAN_NB) phyChanId = MXL_HRCLS_CHAN0;

          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(
              devId, 
              serDesDsId,
              EXTRACT_ADDR(SERDES0_REG_NB0_SEL) + ((UINT16) serDesSlotId >> 1)*4, 
              ((UINT16) serDesSlotId & 1) ? 8:0 /* lsbPos: 0 for even slots and 8 for odd slots */, 
              EXTRACT_NBITS(SERDES0_REG_NB0_SEL),  
              phyChanId);
          if ((status == MXL_SUCCESS) && (serDesSlotId == MXL_HRCLS_SERDES_SLOT2)) 
          {
            devContextPtr->serDesSlot2Chan[(UINT8) serDesDsId] = phyChanId;
          }

          MXLDBG1(MxL_HRCLS_DEBUG("Channel %d connected to slot %d of SerDes%d\n", chanId, serDesSlotId, serDesDsId););
        }
      } else status = MXL_NOT_INITIALIZED;
    }
    else
    {
      status = MXL_INVALID_PARAMETER;
      MXLERR(
        MxL_HRCLS_ERROR("Invalid parameter in Xbar setting: ");
        if (serDesDsId < devContextPtr->serDesDSLanesCnt)
        {
          if (serDesSlotId < devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId])
          {
            MxL_HRCLS_ERROR("chanId should be less than %d or exactly %d\n", devContextPtr->dfeChanCount, MXL_HRCLS_CHAN_NB);
          } else MxL_HRCLS_ERROR("serDesSlotId should be less than %d\n", devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId]);
        } else MxL_HRCLS_ERROR("serDesDsId should be less than %d\n", devContextPtr->serDesDSLanesCnt);
      )
    }
  } 
  else 
  {
    status = MXL_INVALID_PARAMETER;
  }
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPhyPolarity 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/18/2012
--|                  
--| DESCRIPTION   : Invert DS phy
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPhyPolarity(UINT8 devId, 
        MXL_HRCLS_SERDES_LANE_ID_E serDesDsId, 
        MXL_BOOL_E phyInverted)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, DS phyInverted=%d\n", serDesDsId, (phyInverted==MXL_TRUE)?1:0););
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_INVERT, (phyInverted == MXL_ENABLE)?1:0);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream serdes settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsParams(
    UINT8     devId,   
    MXL_HRCLS_SERDES_CODING_E usCoding,   
    MXL_BOOL_E  usRsCorrectEnable,
    MXL_BOOL_E  descramblerEnable 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        devContextPtr->serDesUsPrbsMode = MXL_FALSE;
        if ((usCoding == MXL_HRCLS_SERDES_CODING_CRC) && (devContextPtr->serDesLineRate  == MXL_HRCLS_SERDES_LINERATE_5184MHZ))
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("Cannot use CRC in B mode (5184MHz)\n"););
        }
        else
        {
          UINT16 useCrc = (usCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
          /* Set Us encoding for both serdes 0 and serdes 1 */
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_USE_CRC, useCrc);

          if (devContextPtr->serDesLineRate  == MXL_HRCLS_SERDES_LINERATE_5184MHZ)
          {
            /* Set Us scramble settings for both serdes 0 and serdes 1. Scramle settings are valid only for mode B */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_SCRAM_ENAB, (descramblerEnable == MXL_ENABLE)?1:0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_SCRAM_SELF_SYNC, (descramblerEnable == MXL_ENABLE)?1:0);
          }
          /* Enable/Disable correction */
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DISABLE_CORRECTION, (usRsCorrectEnable == MXL_ENABLE)?0:1);

          // 3B
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN, 1);
          
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_IN_EN_INT, 1);
          
          status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
          status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
          status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);

          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_OUT_EN_INT, 1);
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsSyncParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream synchronization settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncParams(
    UINT8     devId,
    UINT8     S1,
    UINT8     S2,
    UINT8     S3
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        UINT16 regValue = 0;
        UINT8 shift, mask;

        mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NUM)); 
        if (S1 > mask)
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("S1 should be less than or equal %d\n", mask););
        }
        shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NUM);
        regValue = (S1 & mask) << shift;

        mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM)); 
        if (S2 > mask)
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("S2 should be less than or equal %d\n", mask););
        }
        shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM);
        regValue |= (S2 & mask) << shift;

        mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM)); 
        if (S3 > mask)
        {
          status = MXL_INVALID_PARAMETER;
          MXLERR(MxL_HRCLS_ERROR("S3 should be less than or equal %d\n", mask););
        }
        shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM);
        regValue |= (S3 & mask) << shift;

        if (status == MXL_SUCCESS)
        {
          status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_REG_US_MODEA_SYNC_NUM), regValue);
          if (status == MXL_SUCCESS)
          {
            status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES1_REG_US_MODEA_SYNC_NUM), regValue);
          }        
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsEqualizer 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream equalizer settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsEqualizer(
    UINT8     devId,     
    UINT8     equalizerSetting
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (equalizerSetting <= MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_PHY_RX_EQ))))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX_EQ, (UINT16) equalizerSetting);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPrbs 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream serdes PRBS 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbs(
    UINT8     devId,                        
    MXL_BOOL_E  usPrbsCheckerEnable,         
    MXL_HRCLS_SERDES_PRBS_MODE_E usPrbsMode 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) &&
      ((usPrbsMode == MXL_HRCLS_SERDES_PRBS7) || (usPrbsMode == MXL_HRCLS_SERDES_PRBS15)
      || (usPrbsMode == MXL_HRCLS_SERDES_PRBS23) || (usPrbsMode == MXL_HRCLS_SERDES_PRBS31)
      || (usPrbsMode == MXL_HRCLS_SERDES_PRBS22)) )

  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        devContextPtr->serDesUsPrbsMode = usPrbsCheckerEnable;
        if (devContextPtr->serDesLineRate  == MXL_HRCLS_SERDES_LINERATE_5400MHZ )
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX0_ALIGN_EN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_PHY_RX0_ALIGN_EN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
          /* Configure Us parameters of both SERDES0 and SERDES1 */
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_LSB_FIRST_IN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
  //        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1) ;

          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
  //        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsErrCnt 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream error counters 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsErrCnt(
    UINT8     devId,
    MXL_HRCLS_SERDES_US_ERRCNT_T* errCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (errCntPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
        {
          UINT16 correctedAddrLo, correctedAddrHi, uncorrectedAddrLo, uncorrectedAddrHi;
          UINT16 receivedAddrHi, receivedAddrLo;

          UINT16 readDataHi = 0, readDataLo = 0;
          
          correctedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_LSB); 
          correctedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_MSB);
          uncorrectedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_LSB);
          uncorrectedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_MSB);
          receivedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_LSB);
          receivedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_MSB);

          if ((MxLWare_HRCLS_OEM_ReadRegister(devId, correctedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, correctedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntCorrected = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;

          if ((status == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, uncorrectedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, uncorrectedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntUncorrected = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;

          if ((status == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, receivedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, receivedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntReceived = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;
        }
        else
        {
            errCntPtr->cntCorrected = 0; 
            errCntPtr->cntUncorrected = 0; 
            errCntPtr->cntReceived = 0; 
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
 } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsErrCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear upstream error counter 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsErrCntClear(
    UINT8     devId
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr) 
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_CLEAR_CNT); 
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPrbsErrCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear upstream error counter 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbsErrCntClear(
    UINT8     devId
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_RX_CLR); 
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsPrbsErrCnt 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream error counters in PRBS mode 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsPrbsErrCnt(
    UINT8   devId,
    UINT32* errCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (errCntPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesUsPrbsMode == MXL_TRUE)
        {
          UINT16 regAddrHi, regAddrLo;
          UINT16 readDataHi =  0, readDataLo = 0, maskHi = 0xffff;
          if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)
          {
            regAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_MODEA_ERR_CNT_MSB);
            regAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_MODEA_ERR_CNT_LSB);

            maskHi = 0xffff;
          }
          else // MXL_HRCLS_SERDES_LINERATE_5184MHZ
          {
            regAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_RX_ERR_MSB);
            regAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_RX_ERR_LSB);

            maskHi = 0x00ff;
          }
          if ((MxLWare_HRCLS_OEM_ReadRegister(devId, regAddrLo, &readDataLo) == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, regAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            *errCntPtr = (readDataLo | ((readDataHi & maskHi) << 16));
          }                                                                     
        } 
        else
        {
          *errCntPtr = 0;
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsSyncLockStatus 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream synchronization status 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsSyncLockStatus(
    UINT8   devId,
    MXL_BOOL_E* syncLockStatusPtr,
    UINT8*    syncLostCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && ((syncLockStatusPtr) || (syncLostCntPtr)))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
        {
          UINT16 regData = 0;

          status = MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC), &regData);

          if (syncLockStatusPtr)
          {
            *syncLockStatusPtr = (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC)))?MXL_TRUE:MXL_FALSE;  
          }
          if (syncLostCntPtr)
          {
            *syncLostCntPtr = (UINT8) MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, SERDES0_US_SYNCLOST_CNT);
          }
        }
        else
        {
          if (syncLockStatusPtr) *syncLockStatusPtr = MXL_FALSE;
          if (syncLostCntPtr) *syncLostCntPtr = 0;
        }
        MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] Serdes US sync status: %s\nSync lost cnt: %d\n", (syncLockStatusPtr)?((*syncLockStatusPtr == MXL_TRUE)?"OK":"LOST"):"----", 
                                                                                          (syncLostCntPtr)?(*syncLostCntPtr):(-1)););
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsSyncLostCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear sync lost counter
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncLostCntClear(
    UINT8   devId
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_SYNCLOST_CNT_CLR);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPhyPolarity 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/18/2012
--|                  
--| DESCRIPTION   : Invert US phy
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPhyPolarity(UINT8 devId, MXL_BOOL_E phyInverted)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("US phyInverted=%d\n", (phyInverted==MXL_TRUE)?1:0););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX_INVERT, (phyInverted == MXL_ENABLE)?1:0);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  enable Enable or disable upstream hardware
 *
 *  @apibrief   This API configures up stream
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevUpStreamParams(
    UINT8     devId,
    MXL_HRCLS_TXDAC_STATE_E  enable,
    MXL_HRCLS_TXDAC_OUT_SWING_E outputSwing
    )
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  UINT8 status = MXL_SUCCESS;

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("enable=%d, swing=%d\n", (UINT8) enable, (UINT8) outputSwing););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        HOST_COMMAND_T cfgDevUsParams;
        UINT8 enableHw;
        UINT8 outputSwingHw;

        enableHw = (enable == MXL_HRCLS_TXDAC_DISABLE)?MXL_HRCLS_HW_TXDAC_DISABLE:
                   (enable == MXL_HRCLS_TXDAC_ENABLE)?MXL_HRCLS_HW_TXDAC_ENABLE:MXL_HRCLS_HW_TXDAC_EXTERNAL;
        outputSwingHw = (outputSwing == MXL_HRCLS_TXDAC_OUT_MIN)?MXL_HRCLS_HW_TXDAC_SWING_MIN:
                        (outputSwing == MXL_HRCLS_TXDAC_OUT_MAX)?MXL_HRCLS_HW_TXDAC_SWING_MAX:MXL_HRCLS_HW_TXDAC_SWING_DEFAULT;

        /* Form command payload */
        cfgDevUsParams.data[0]    = (((UINT8)enableHw      << 0) & 0x03) 
                                  | (((UINT8)outputSwingHw << 2) & 0x0C);
        cfgDevUsParams.payloadLen = 1;
        cfgDevUsParams.syncChar   = 0;
        cfgDevUsParams.commandId  = 0;
        cfgDevUsParams.payloadCheckSum = 0;

        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &cfgDevUsParams, HOST_CMD_CFG_DEV_UPSTREAM_PARAMS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

        /* Wait for some time (20ms) for host command to complete */
        MxLWare_HRCLS_OEM_DelayUsec(20*1000);

      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

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

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesPhyReset(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesId);
  if ((devContextPtr) && (serDesId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, serDesId, SERDES0_REG_PHY_BPR_RESET);
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);
  return status;
}

#endif // _MXL_HRCLS_SERDES_ENABLED_

