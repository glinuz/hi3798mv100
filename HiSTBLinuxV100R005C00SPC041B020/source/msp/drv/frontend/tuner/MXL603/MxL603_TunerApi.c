/*******************************************************************************
 *
 * FILE NAME          : MxL603_TunerApi.cpp
 * 
 * AUTHOR             : Dong Liu 
 *                      
 *
 * DATE CREATED       : 07/12/2011
 *                      11/30/2011
 *
 * DESCRIPTION        : This file contains MxL603 driver APIs
 *
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/
//#include "drv_tuner_ext.h"
#include "drv_tuner_ioctl.h"
#include "MxL603_TunerApi.h"
#include "MxL603_TunerCfg.h"
#include "MxL_Debug.h"

/* MxLWare Driver version for MxL603 */
static const UINT8 MxLWare603DrvVersion[] = {1, 1, 3, 1, 0}; 

/* OEM Data pointer array */
void * MxL603_OEM_DataPtr[MXL603_MAX_NUM_DEVICES];

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDrvInit
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API must be called prior to any other API function.  
--|                 Cannot be called more than once.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDrvInit(UINT8 devId, void* oemDataPtr)
{
  MXL_STATUS status = MXL_SUCCESS;

  if (oemDataPtr)
  {
    if (devId < MXL603_MAX_NUM_DEVICES) MxL603_OEM_DataPtr[devId] = oemDataPtr;
    else status = MXL_INVALID_PARAMETER;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDevSoftReset
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API is used to reset MxL603 tuner device. After reset,
--|                 all the device regiaters and modules will be set to power-on  
--|                 default state. 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILED 
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDevSoftReset(UINT8 devId)
{
  UINT8 status = MXL_SUCCESS;
  
  //MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  // Write 0xFF with 0 to reset tuner 
  status = MxLWare603_OEM_WriteRegister(devId, AIC_RESET_REG, 0x00); 
  
  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDevOverwriteDefaults
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : Register(s) that requires default values to be overwritten 
--|                 during initialization
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDevOverwriteDefaults(UINT8 devId, 
                                                  MXL_BOOL singleSupply_3_3V)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 readData = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  status |= MxL603_Ctrl_ProgramRegisters(devId, MxL603_OverwriteDefaults);

  status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x01);
  status |= MxLWare603_OEM_ReadRegister(devId, 0x31, &readData);
  readData &= 0x2F;
  readData |= 0xD0;
  status |= MxLWare603_OEM_WriteRegister(devId, 0x31, readData);
  status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x00);


  /* If Single supply 3.3v is used */
  if (MXL_ENABLE == singleSupply_3_3V)
    status |= MxLWare603_OEM_WriteRegister(devId, MAIN_REG_AMP, 0x04);

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDevXtal
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API is used to configure XTAL settings of MxL603 tuner
--|                 device. XTAL settings include frequency, capacitance & 
--|                 clock out
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDevXtal(UINT8 devId, MXL603_XTAL_SET_CFG_T xtalCfg)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 control = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  // XTAL freq and cap setting, Freq set is located at bit<5>, cap bit<4:0> 
  // and  XTAL clock out enable <0>
  if ((xtalCfg.xtalFreqSel == MXL603_XTAL_16MHz) || (xtalCfg.xtalFreqSel == MXL603_XTAL_24MHz))
  {
    control = (UINT8)((xtalCfg.xtalFreqSel << 5) | (xtalCfg.xtalCap & 0x1F));  
    control |= (xtalCfg.clkOutEnable << 7);
    status = MxLWare603_OEM_WriteRegister(devId, XTAL_CAP_CTRL_REG, control);

    // XTAL frequency div 4 setting <1> 
    control = (0x01 & (UINT8)xtalCfg.clkOutDiv);
    
    // XTAL sharing mode
    if (xtalCfg.XtalSharingMode == MXL_ENABLE) control |= 0x40;
    else control &= 0x01;

    // program Clock out div & Xtal sharing
    status |= MxLWare603_OEM_WriteRegister(devId, XTAL_ENABLE_DIV_REG, control); 

    // Main regulator re-program
    if (MXL_ENABLE == xtalCfg.singleSupply_3_3V)
      status |= MxLWare603_OEM_WriteRegister(devId, MAIN_REG_AMP, 0x14);
  }
  else 
    status |= MXL_INVALID_PARAMETER;
  
  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDevPowerMode
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function configures MxL603 power mode 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDevPowerMode(UINT8 devId, MXL603_PWR_MODE_E powerMode)
{
  UINT8 status = MXL_SUCCESS;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  switch(powerMode)
  {
    case MXL603_PWR_MODE_SLEEP:
      break;

    case MXL603_PWR_MODE_ACTIVE:
      status |= MxLWare603_OEM_WriteRegister(devId, TUNER_ENABLE_REG, MXL_ENABLE);
      status |= MxLWare603_OEM_WriteRegister(devId, START_TUNE_REG, MXL_ENABLE);
      break;

    case MXL603_PWR_MODE_STANDBY:
      status |= MxLWare603_OEM_WriteRegister(devId, START_TUNE_REG, MXL_DISABLE);
      status |= MxLWare603_OEM_WriteRegister(devId, TUNER_ENABLE_REG, MXL_DISABLE);
      break;

    default:
      status |= MXL_INVALID_PARAMETER;
  }

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgDevGPO
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API configures GPO pin of MxL603 tuner device.
--|                 There is only 1 GPO pin available in MxL603 device.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgDevGPO(UINT8 devId, MXL603_GPO_STATE_E gpoState)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 regData = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  switch(gpoState)
  {
    case MXL603_GPO_AUTO_CTRL:
    case MXL603_GPO_HIGH:
    case MXL603_GPO_LOW:
      status = MxLWare603_OEM_ReadRegister(devId, GPO_SETTING_REG, &regData);
      if (MXL603_GPO_AUTO_CTRL == gpoState)
        regData &= 0xFE;
      else
      {
        regData &= 0xFC;
        regData |= (UINT8)(0x01 | (gpoState << 1)); 
      }
     
      status |= MxLWare603_OEM_WriteRegister(devId, GPO_SETTING_REG, regData);
      break;

    default:
      status = MXL_INVALID_PARAMETER;
  }

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_ReqDevVersionInfo
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function is used to get MxL603 version information.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_ReqDevVersionInfo(UINT8 devId, 
                                            MXL603_VER_INFO_T* mxlDevVerInfoPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 readBack = 0;
  UINT8 k = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  if (mxlDevVerInfoPtr)
  {
    status |= MxLWare603_OEM_ReadRegister(devId, CHIP_ID_REQ_REG, &readBack);
    mxlDevVerInfoPtr->chipId = (readBack & 0xFF); 

    status |= MxLWare603_OEM_ReadRegister(devId, CHIP_VERSION_REQ_REG, &readBack);
    mxlDevVerInfoPtr->chipVersion = (readBack & 0xFF); 

    MxL_DLL_DEBUG0("Chip ID = 0x%d, Version = 0x%d \n", mxlDevVerInfoPtr->chipId, 
                                                        mxlDevVerInfoPtr->chipVersion);
    
    // Get MxLWare version infromation
    for (k = 0; k < MXL603_VERSION_SIZE; k++)
      mxlDevVerInfoPtr->mxlwareVer[k] = MxLWare603DrvVersion[k];
  }
  else 
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_ReqDevGPOStatus
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API is used to get GPO pin's status information from
--|                 MxL603 tuner device.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_ReqDevGPOStatus(UINT8 devId, 
                                          MXL603_GPO_STATE_E* gpoStatusPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 regData = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  if (gpoStatusPtr)
  {
    status = MxLWare603_OEM_ReadRegister(devId, GPO_SETTING_REG, &regData);

    // GPO1 bit<1:0>
    if ((regData & 0x01) == 0) *gpoStatusPtr = MXL603_GPO_AUTO_CTRL;
    else *gpoStatusPtr = (MXL603_GPO_STATE_E)((regData & 0x02) >> 1);
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgTunerMode
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This fucntion is used to configure MxL603 tuner's 
--|                 application modes like DVB-T, DVB-C, ISDB-T etc.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgTunerMode(UINT8 devId, 
                                       MXL603_TUNER_MODE_CFG_T tunerModeCfg)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 dfeRegData = 0;
  MXL603_REG_CTRL_INFO_T* tmpRegTable;

  MxL_DLL_DEBUG0("%s: Signal Mode = %d, IF Freq = %d, xtal = %d, IF Gain = %d", 
                                                __FUNCTION__,
                                                tunerModeCfg.signalMode,
                                                tunerModeCfg.ifOutFreqinKHz,
                                                tunerModeCfg.xtalFreqSel,
                                                tunerModeCfg.ifOutGainLevel); 

  switch(tunerModeCfg.signalMode)
  {
    case MXL603_DIG_DVB_C:
    case MXL603_DIG_J83B:
      tmpRegTable = MxL603_DigitalDvbc;
      status = MxL603_Ctrl_ProgramRegisters(devId, tmpRegTable);

      if (tunerModeCfg.ifOutFreqinKHz < HIGH_IF_35250_KHZ)
      {
        // Low power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
      }
      else
      {
        // High power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
      }

      if (tunerModeCfg.xtalFreqSel == MXL603_XTAL_16MHz) dfeRegData = 0x0D;
      else if (tunerModeCfg.xtalFreqSel == MXL603_XTAL_24MHz) dfeRegData = 0x0E;
      else status |= MXL_INVALID_PARAMETER;
        
      status |= MxLWare603_OEM_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

      break;

    case MXL603_DIG_ISDBT_ATSC:
      tmpRegTable = MxL603_DigitalIsdbtAtsc;
      status = MxL603_Ctrl_ProgramRegisters(devId, tmpRegTable);

      if (tunerModeCfg.ifOutFreqinKHz < HIGH_IF_35250_KHZ)
      {
        // Low power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xF9);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x18);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_PWR, 0xF1);
      }
      else
      {
        // High power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_PWR, 0xB1);
      }

      if (MXL603_XTAL_16MHz == tunerModeCfg.xtalFreqSel) dfeRegData = 0x0D;
      else if (MXL603_XTAL_24MHz == tunerModeCfg.xtalFreqSel) dfeRegData = 0x0E;
      else status |= MXL_INVALID_PARAMETER;
      
      status |= MxLWare603_OEM_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

      dfeRegData = 0;
      switch(tunerModeCfg.ifOutGainLevel)
      {
        case 0x09: dfeRegData = 0x44; break;
        case 0x08: dfeRegData = 0x43; break;
        case 0x07: dfeRegData = 0x42; break;
        case 0x06: dfeRegData = 0x41; break;
        case 0x05: dfeRegData = 0x40; break;
        default: break;
      }
      status |= MxLWare603_OEM_WriteRegister(devId, DFE_DACIF_GAIN, dfeRegData);
     
      break;

    case MXL603_DIG_DVB_T:
      tmpRegTable = MxL603_DigitalDvbt;
      status = MxL603_Ctrl_ProgramRegisters(devId, tmpRegTable);

      if (tunerModeCfg.ifOutFreqinKHz < HIGH_IF_35250_KHZ)
      {
        // Low power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x18);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_PWR, 0xF1);
      }
      else
      {
        // High power
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
        status |= MxLWare603_OEM_WriteRegister(devId, DIG_ANA_IF_PWR, 0xB1);
      }
      
      if (MXL603_XTAL_16MHz == tunerModeCfg.xtalFreqSel) dfeRegData = 0x0D;
      else if (MXL603_XTAL_24MHz == tunerModeCfg.xtalFreqSel) dfeRegData = 0x0E;
      else status |= MXL_INVALID_PARAMETER;
      
      status |= MxLWare603_OEM_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

      dfeRegData = 0;
      switch(tunerModeCfg.ifOutGainLevel)
      {
        case 0x09: dfeRegData = 0x44; break;
        case 0x08: dfeRegData = 0x43; break;
        case 0x07: dfeRegData = 0x42; break;
        case 0x06: dfeRegData = 0x41; break;
        case 0x05: dfeRegData = 0x40; break;
        default: break;
      }
      status |= MxLWare603_OEM_WriteRegister(devId, DFE_DACIF_GAIN, dfeRegData);
      break;

    default:
      status = MXL_INVALID_PARAMETER;
      break;
  }

  if (status == MXL_SUCCESS)  
  {
    // XTAL calibration
    status |= MxLWare603_OEM_WriteRegister(devId, XTAL_CALI_SET_REG, 0x00);   
    status |= MxLWare603_OEM_WriteRegister(devId, XTAL_CALI_SET_REG, 0x01);   

    // 50 ms sleep after XTAL calibration
    MxLWare603_OEM_Sleep(50);
  }

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgTunerAGC
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function is used to configure AGC settings of MxL603
--|                 tuner device.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgTunerAGC(UINT8 devId, MXL603_AGC_CFG_T agcCfg)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 regData = 0; 

  MxL_DLL_DEBUG0("%s, AGC sel = %d, attack point set = %d, Flip = %d \n", 
                                                __FUNCTION__, 
                                                agcCfg.agcType,
                                                agcCfg.setPoint, 
                                                agcCfg.agcPolarityInverstion);

  if ((agcCfg.agcPolarityInverstion <= MXL_ENABLE) && 
      (agcCfg.agcType <= MXL603_AGC_EXTERNAL))
  {
    // AGC selecton <3:2> and mode setting <0>
    status |= MxLWare603_OEM_ReadRegister(devId, AGC_CONFIG_REG, &regData); 
    regData &= 0xF2; // Clear bits <3:2> & <0>
    regData = (UINT8) (regData | (agcCfg.agcType << 2) | 0x01);
    status |= MxLWare603_OEM_WriteRegister(devId, AGC_CONFIG_REG, regData);

    // AGC set point <6:0>
    status |= MxLWare603_OEM_ReadRegister(devId, AGC_SET_POINT_REG, &regData);
    regData &= 0x80; // Clear bit <6:0>
    regData |= agcCfg.setPoint;
    status |= MxLWare603_OEM_WriteRegister(devId, AGC_SET_POINT_REG, regData);

    // AGC Polarity <4>
    status |= MxLWare603_OEM_ReadRegister(devId, AGC_FLIP_REG, &regData);
    regData &= 0xEF; // Clear bit <4>
    regData |= (agcCfg.agcPolarityInverstion << 4);
    status |= MxLWare603_OEM_WriteRegister(devId, AGC_FLIP_REG, regData);
  }
  else
    status = MXL_INVALID_PARAMETER;

  return(MXL_STATUS) status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgTunerLoopThrough
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function is used to enable or disable Loop-Through
--|                 settings of MxL603 tuner device.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgTunerLoopThrough(UINT8 devId, MXL_BOOL loopThroughCtrl)
{
  UINT8 status = MXL_SUCCESS;

  MxL_DLL_DEBUG0("%s", __FUNCTION__); 

  if (loopThroughCtrl <= MXL_ENABLE)
  {
    status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x01);

    if (loopThroughCtrl == MXL_ENABLE)
      status |= MxLWare603_OEM_WriteRegister(devId, 0x96, 0x10);
    else
      status |= MxLWare603_OEM_WriteRegister(devId, 0x96, 0x00);

    status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x00);
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgTunerChanTune
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This API configures RF channel frequency and bandwidth. 
--|                 Radio Frequency unit is Hz, and Bandwidth is in MHz units.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgTunerChanTune(UINT8 devId, 
                                           MXL603_CHAN_TUNE_CFG_T chanTuneCfg)
{
  UINT32 frequency;
  UINT32 freq = 0;
  UINT8 status = MXL_SUCCESS;
  UINT8 regData = 0;
  UINT8 agcData = 0;
  UINT8 dfeTuneData = 0;
  UINT8 dfeCdcData = 0;
  
  MxL_DLL_DEBUG0("%s, signal type = %d, Freq = %d, BW = %d, Xtal = %d \n",  
                                              __FUNCTION__,
                                              chanTuneCfg.signalMode, 
                                              chanTuneCfg.freqInHz, 
                                              chanTuneCfg.bandWidth, 
                                              chanTuneCfg.xtalFreqSel);

  // Abort Tune
  status |= MxLWare603_OEM_WriteRegister(devId, START_TUNE_REG, 0x00); 

  if (chanTuneCfg.startTune == MXL_ENABLE)
  {
    if (chanTuneCfg.signalMode <= MXL603_DIG_J83B) 
    {
      // RF Frequency VCO Band Settings 
      if (chanTuneCfg.freqInHz < 700000000) 
      {
        status |= MxLWare603_OEM_WriteRegister(devId, 0x7C, 0x1F);
        if ((chanTuneCfg.signalMode == MXL603_DIG_DVB_C) || (chanTuneCfg.signalMode == MXL603_DIG_J83B)) 
          regData = 0xC1;
        else
          regData = 0x81;
        
      }
      else 
      {
        status |= MxLWare603_OEM_WriteRegister(devId, 0x7C, 0x9F);
        if ((chanTuneCfg.signalMode == MXL603_DIG_DVB_C) || (chanTuneCfg.signalMode == MXL603_DIG_J83B)) 
          regData = 0xD1;
        else
          regData = 0x91;
        
      }

      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x01);
      status |= MxLWare603_OEM_WriteRegister(devId, 0x31, regData);
      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x00);
      
      // Bandwidth <7:0>
      switch(chanTuneCfg.bandWidth)
      {
        case MXL603_CABLE_BW_6MHz:
        case MXL603_CABLE_BW_7MHz:
        case MXL603_CABLE_BW_8MHz:
        case MXL603_TERR_BW_6MHz:
        case MXL603_TERR_BW_7MHz:
        case MXL603_TERR_BW_8MHz:
            status |= MxLWare603_OEM_WriteRegister(devId, CHAN_TUNE_BW_REG, (UINT8)chanTuneCfg.bandWidth);
            
            // Frequency
            if((enMxl603TunerSigType[devId] == HI_UNF_TUNER_SIG_TYPE_CAB) &&
               ((HI_UNF_DEMOD_DEV_TYPE_3130I == enMxl603DemodType) ||
                (HI_UNF_DEMOD_DEV_TYPE_3130E == enMxl603DemodType) ||
                (HI_UNF_DEMOD_DEV_TYPE_J83B == enMxl603DemodType)))        
            {
                frequency = chanTuneCfg.freqInHz / 1000;
                /* Calculate RF Channel = DIV(64*RF(Hz), 1E6) */
                frequency *= 64;
                //freq = (UINT32)(frequency / 1000000); 
                freq = (UINT32)(frequency / 1000);
            }
            else
            {
                frequency = chanTuneCfg.freqInHz;
                /* Calculate RF Channel = DIV(64*RF(Hz), 1E6) */
                frequency *= 4;
                freq = (UINT32)(frequency / 62500); 
            }

            // Set RF  
            status |= MxLWare603_OEM_WriteRegister(devId, CHAN_TUNE_LOW_REG, (UINT8)(freq & 0xFF));
            status |= MxLWare603_OEM_WriteRegister(devId, CHAN_TUNE_HI_REG, (UINT8)((freq >> 8 ) & 0xFF));
            break;

        default:
          status |= MXL_INVALID_PARAMETER;
          break;
      }
    
      // Power up tuner module
      status |= MxLWare603_OEM_WriteRegister(devId, TUNER_ENABLE_REG, 0x01);
      
      // Start Sequencer settings
      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x01); 
      status |= MxLWare603_OEM_ReadRegister(devId, 0x96, &regData);
      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x00); 
      
      status |= MxLWare603_OEM_ReadRegister(devId, 0xB6, &agcData);
      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x01); 
      status |= MxLWare603_OEM_ReadRegister(devId, 0x60, &dfeTuneData);
      status |= MxLWare603_OEM_ReadRegister(devId, 0x5F, &dfeCdcData);

      // Check if LT is enabled
      if ((regData & 0x10) == 0x10)
      {
        // dfe_agc_auto = 0 & dfe_agc_rf_bo_w = 14
        agcData &= 0xBF;
        agcData |= 0x0E;
        
        // dfe_seq_tune_rf1_bo = 14
        dfeTuneData &= 0xC0;
        dfeTuneData |= 0x0E;
        
        // dfe_seq_cdc_rf1_bo = 14
        dfeCdcData &= 0xC0;
        dfeCdcData |= 0x0E;
      }
      else
      {
        // dfe_agc_auto = 1 & dfe_agc_rf_bo_w = 0
        agcData |= 0x40;
        agcData &= 0xC0;
        
        // dfe_seq_tune_rf1_bo = 55
        dfeTuneData &= 0xC0;
        dfeTuneData |= 0x37;

        // dfe_seq_cdc_rf1_bo = 55
        dfeCdcData &= 0xC0;
        dfeCdcData |= 0x37;
      }
      
      status |= MxLWare603_OEM_WriteRegister(devId, 0x60, dfeTuneData); 
      status |= MxLWare603_OEM_WriteRegister(devId, 0x5F, dfeCdcData); 
      status |= MxLWare603_OEM_WriteRegister(devId, 0x00, 0x00); 
      status |= MxLWare603_OEM_WriteRegister(devId, 0xB6, agcData); 
      
      // Bit <0> 1 : start , 0 : abort calibrations
      status |= MxLWare603_OEM_WriteRegister(devId, START_TUNE_REG, 0x01); 

      // Sleep 15 ms
      MxLWare603_OEM_Sleep(15);

      // dfe_agc_auto = 1 
      agcData = (agcData | 0x40);
      status |= MxLWare603_OEM_WriteRegister(devId, 0xB6, agcData); 

    }
    else
      status = MXL_INVALID_PARAMETER;
  }

  return (MXL_STATUS)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_CfgTunerIFOutParam
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function is used to configure IF out settings of MxL603 
--|                 tuner device.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_CfgTunerIFOutParam(UINT8 devId, MXL603_IF_OUT_CFG_T ifOutCfg)
{
  UINT16 ifFcw;
  UINT8 status = MXL_SUCCESS;
  UINT8 readData = 0;
  UINT8 control = 0;
 
  MxL_DLL_DEBUG0("%s, Manual set = %d \n", __FUNCTION__, ifOutCfg.manualFreqSet); 

  // Read back register for manual IF Out 
  status = MxLWare603_OEM_ReadRegister(devId, IF_FREQ_SEL_REG, &readData);

  if (ifOutCfg.manualFreqSet == MXL_ENABLE)
  {
    MxL_DLL_DEBUG0("%s, IF Freq = %d \n", __FUNCTION__, ifOutCfg.manualIFOutFreqInKHz); 

    // IF out manual setting : bit<5>
    readData |= 0x20;
    status = MxLWare603_OEM_WriteRegister(devId, IF_FREQ_SEL_REG, readData);

    // Manual IF freq set
    ifFcw = (UINT16)(ifOutCfg.manualIFOutFreqInKHz * 8192 / 216000);
    control = (ifFcw & 0xFF); // Get low 8 bit 
    status |= MxLWare603_OEM_WriteRegister(devId, IF_FCW_LOW_REG, control); 

    control = ((ifFcw >> 8) & 0x0F); // Get high 4 bit 
    status |= MxLWare603_OEM_WriteRegister(devId, IF_FCW_HIGH_REG, control);
  }
  else if (ifOutCfg.manualFreqSet == MXL_DISABLE)
  {
    // bit<5> = 0, use IF frequency from IF frequency table  
    readData &= 0xC0;

    // IF Freq <4:0>
    readData |= ifOutCfg.ifOutFreq;
    status |= MxLWare603_OEM_WriteRegister(devId, IF_FREQ_SEL_REG, readData);
  }
  else
    status |= MXL_INVALID_PARAMETER;

  if (status == MXL_SUCCESS)
  {
    // Set spectrum invert, gain level and IF path 
    // Spectrum invert indication is bit<7:6>
    if (ifOutCfg.ifInversion <= MXL_ENABLE)
    {
      control = 0;
      if (MXL_ENABLE == ifOutCfg.ifInversion) control = 0x3 << 6;
    
      // Gain level is bit<3:0> 
      control += (ifOutCfg.gainLevel & 0x0F);
      control |= (0x20); // Enable IF out
      status |= MxLWare603_OEM_WriteRegister(devId, IF_PATH_GAIN_REG, control);
    } 
    else
      status |= MXL_INVALID_PARAMETER;
  }

  return(MXL_STATUS) status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_ReqTunerAGCLock
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function returns AGC Lock status of MxL603 tuner.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_ReqTunerAGCLock(UINT8 devId, MXL_BOOL* agcLockStatusPtr)
{
  MXL_STATUS status = MXL_SUCCESS;
  UINT8 regData = 0;
  MXL_BOOL lockStatus = MXL_UNLOCKED;

  MxL_DLL_DEBUG0("%s", __FUNCTION__);

  if (agcLockStatusPtr)
  {
    status = MxLWare603_OEM_ReadRegister(devId, AGC_SAGCLOCK_STATUS_REG, &regData);  
    if ((regData & 0x08) == 0x08) lockStatus = MXL_LOCKED;

    *agcLockStatusPtr =  lockStatus;
    
    MxL_DLL_DEBUG0(" Agc lock = %d", (UINT8)*agcLockStatusPtr); 
  }
  else
    status = MXL_INVALID_PARAMETER;

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_ReqTunerLockStatus
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function returns Tuner Lock status of MxL603 tuner.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_API_ReqTunerLockStatus(UINT8 devId, MXL_BOOL* rfLockPtr, 
                                                          MXL_BOOL* refLockPtr)
{
  MXL_STATUS status = MXL_SUCCESS;
  UINT8 regData = 0;
  MXL_BOOL rfLockStatus = MXL_UNLOCKED;
  MXL_BOOL refLockStatus = MXL_UNLOCKED;

  MxL_DLL_DEBUG0("%s", __FUNCTION__);

  if ((rfLockPtr) && (refLockPtr))
  {
    status = MxLWare603_OEM_ReadRegister(devId, RF_REF_STATUS_REG, &regData);  

    if ((regData & 0x02) == 0x02) rfLockStatus = MXL_LOCKED;
    if ((regData & 0x01) == 0x01) refLockStatus = MXL_LOCKED;
    
    MxL_DLL_DEBUG0(" RfSynthStatus = %d, RefSynthStatus = %d", (UINT8)rfLockStatus,
                                                               (UINT8)refLockStatus); 

    *rfLockPtr =  rfLockStatus;
    *refLockPtr = refLockStatus;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_API_ReqTunerRxPower
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/10/2011  
--|
--| DESCRIPTION   : This function returns RF input power in dBm.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, MXL_FAILED
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS MxLWare603_API_ReqTunerRxPower(UINT8 devId, SINT16* rxPwrPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 regData = 0;
  UINT16 tmpData = 0;

  MxL_DLL_DEBUG0("%s", __FUNCTION__);

  if (rxPwrPtr)
  {
    // RF input power low <7:0>
    status = MxLWare603_OEM_ReadRegister(devId, RFPIN_RB_LOW_REG, &regData);
    tmpData = regData;

    // RF input power high <1:0>
    status |= MxLWare603_OEM_ReadRegister(devId, RFPIN_RB_HIGH_REG, &regData);
    tmpData |= (regData & 0x03) << 8;

    // Fractional last 2 bits
    *rxPwrPtr = (tmpData & 0x01FF) * 25;  //100 times dBm

    if (tmpData & 0x02) *rxPwrPtr += 50;;
    if (tmpData & 0x01) *rxPwrPtr += 25;
    if (tmpData & 0x0200) *rxPwrPtr -= 128*100;
      
    MxL_DLL_DEBUG0(" Rx power = %d times of 0.01dBm \n", *rxPwrPtr);
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS)status;
}

