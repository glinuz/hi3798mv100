/*******************************************************************************
 *
 * FILE NAME          : MxL68x_PhyCtrlApi.cpp
 * 
 * AUTHOR             : Mahendra Kondur, Dong Liu, Jay Hung 
 *
 * DATE CREATED       : 01/13/2012, 3/16/2012, 3/9/2015
 *
 * DESCRIPTION        : This file contains MxL68x driver APIs
 *
 *
 *******************************************************************************
 *                Copyright (c) 2012, MaxLinear, Inc.
 ******************************************************************************/

#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhyCfg.h"
#include "MxL68x_FwDownload.h"
#include "MxL_Debug.h"

/* MxLWare Driver version for MxL68x */
const UINT8 MxLWareDrvVersion[] = {1, 2, 4, 3};
// Candidate version number, 0 means for release version 
const UINT8 BuildNumber = 0; 

/* OEM Data pointer array */
void * MxL68x_OEM_DataPtr[MXL68X_MAX_DEVICES];

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDrvInit
--| 
--| AUTHOR        : Mahendra Kondur, Dong Liu
--|
--| DATE CREATED  : 01/25/2012, 02/22/2012 
--|
--| DESCRIPTION   :  This API must be called prior to any other API function. 
--|                  Cannot be called more than once. In such case it will 
--|                  return MXL_ALREADY_INITIALIZED.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_ALREADY_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDrvInit(
	            			UINT8 devId, 
				            void* oemDataPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  if (devContextPtr)
  {
    //if (devContextPtr->driverStatus == MXL68x_DRV_UNINITIALISED) //gbiao
    {
      MxL_DLL_DEBUG0("MxLWare68x_API_CfgDrvInit, Init Context [%d]\n",devId);
      // Driver is not initialised
      MxL68x_OEM_DataPtr[devId] = oemDataPtr;
      devContextPtr->driverStatus = MXL68x_DRV_INITIALISED;    // change to enum
      devContextPtr->ifOutFreqInKHz = MXL68x_IF_FREQ_4000_KHZ;  // ISDB-T default setting 
      devContextPtr->xtalFreqInMHz = MXL68x_XTAL_16MHz;    // Default XTAL freq
      devContextPtr->ifGainLevel = 13;   // default value    
      devContextPtr->devVersion = MXL68x_VER_NA;
      devContextPtr->devOpMode = MXL68x_TUNER_DEMOD;
      devContextPtr->pllClkReady = MXL_FALSE;
      devContextPtr->firmwareStatus = MXL68x_FW_STATUS_UNKNOWN;
      devContextPtr->mxl68xType = UNKNOWN_DEVICE;
      devContextPtr->rfLoopThrough = MXL_DISABLE;
      devContextPtr->fwDownloadPercent = 0; 
    } 
    //else 
    //  status = MXL_ALREADY_INITIALIZED;
  } 
  else 
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDrvDeInit
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 02/22/2012 
--|
--| DESCRIPTION   : This API is called when user want to reset specified device 
--|                 context array to zero. After caling it will return 
--|                 MXL_NOT_INITIALIZED.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDrvDeInit(UINT8 devId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  if (devContextPtr)
  {
    MxL_DLL_DEBUG0("MxLWare68x_API_CfgDrvDeInit, De-Init Context [%d]\n",devId);
    // Un-register the interrupt handler call back function.
    if (devContextPtr->intSrcId != MXL68x_INT_NONE)
      status = Ctrl_UnRegisterIntHandlerFunc(devId);

    devContextPtr->driverStatus = MXL68x_DRV_UNINITIALISED;
    devContextPtr->pllClkReady = MXL_FALSE;
    devContextPtr->firmwareStatus = MXL68x_FW_STATUS_UNKNOWN;
    devContextPtr->mxl68xType = UNKNOWN_DEVICE;   
  } 
  else 
    status = MXL_INVALID_PARAMETER;
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);

  return status;
}
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevPowerSupply 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/05/2012
--|
--| DESCRIPTION   : This function will configure MxL68x device power supply 
--|                source. Single supply of 3.3V or Dual supply of 1.8v & 3.3V
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevPowerSupply(
                UINT8 devId,
                MXL68x_PWR_SUPPLY_E devPwrSupplyCfg)
{
  UINT16 regData1, regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevPowerSupply [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Enable Schmitt Trigger for I2C bus signals 
      status = MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG - 1, &regData1);  // High 16 bit 
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG, &regData);   // Low 16 bit 
      regData |= 0x03;  // Set Bit<1> and Bit<0> with 1 
      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_CLK_PHASE_REG - 1, regData1);   // High 16 bit 
      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_CLK_PHASE_REG, regData);   // Low 16 bit 

      // Perform Demod soft reset
      status |= Ctrl_CfgDevReset(devId, MXL68x_DEMOD_RESET);
  
      // Inquire MxL68x device type 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_ID_VERSION_REG, &regData);
      if (MXL_SUCCESS == status)
      {
        //HI_ERR_TUNER("---->read [0x%x],regData:0x%04x\n",DMD_ID_VERSION_REG,regData);
        if ((regData & 0x03) == MXL681_DEVICE_ID) 
          devContextPtr->mxl68xType = MXL681_DEVICE;
        else if ((regData & 0x03) == MXL683_DEVICE_ID)
          devContextPtr->mxl68xType = MXL683_DEVICE;
        else devContextPtr->mxl68xType = UNKNOWN_DEVICE;
      }

      // Configure single or dual power supply
      regData = (MXL68x_SINGLE_SUPPLY == devPwrSupplyCfg ) ? 0x14 : 0x10;

      status |= MxL68x_Ctrl_WriteRegister(devId, POWER_SUPPLY_REG, regData);        
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevXtal 
--| 
--| AUTHOR        : Dong Liu, Prithvi 
--|
--| DATE CREATED  : 03/20/2012, 06/25/2012
--|
--| DESCRIPTION   : This fucntion is used to condifure device crystal 
--|                 properties.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevXtal( 
                UINT8 devId,            
                MXL68x_XTAL_FREQ_E xtalFreqSel,    
                UINT8 xtalCap,                                                                
                MXL_BOOL_E clkOutEnable,     
                MXL_BOOL_E clkOutDiv,
                MXL_BOOL_E enableXtalSharing,
		            MXL_BOOL_E enableXtalCal)                                      
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevXtal [%d]\n",devId);

  // Check for possible invalid parameters
  if ((devContextPtr) && (xtalCap > 0) && (xtalCap < 32))
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, XTAL_CFG_REG, &regData); 
      

      //Removing this to solve issues where slace cap needs to be changed to 5pf
      //if(MXL_ENABLE == enableXtalCal)
      {
        // Set XTAL cap value bits <4:0>
        regData = regData & 0xE0;
        regData |= (xtalCap & 0x1F);
      }      

      // Set XTAL frequency bit <5>
      regData = (xtalFreqSel == MXL68x_XTAL_16MHz) ? (regData & 0x00DF) : (regData | 0x0020);
      
      // Set CLK OUT bit <7>
      regData = (clkOutEnable == MXL_ENABLE) ? (regData | 0x0080) : (regData & 0x007F);
  
      // Program XTAL freq, clk out & xtal cap
      status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CFG_REG, regData);      
      regData = 0;

      if(MXL_ENABLE == enableXtalCal) // For Master Only
      {
        //Program Xtal Aplitude to a higher value
        status |= MxL68x_Ctrl_WriteRegister(devId, DFE_DIG_ANA_XTAL_TH, 0x0003);   
      }

      status |= MxL68x_Ctrl_ReadRegister(devId, XTAL_SHARING_REG, &regData); 
      
      // Set CLK OUT DIV <0>
      regData = (clkOutDiv == MXL_ENABLE) ? (regData | 0x0001) : (regData & 0x00FE);
  
      // Set XTAL sharing
      regData = (enableXtalSharing == MXL_ENABLE) ? (regData & 0x01) : (regData | 0x00);

      // Program XTAL sharing & clk out div
      status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_SHARING_REG, regData);    

      if(MXL_ENABLE == enableXtalCal)
      {
        // Program Xtal external bias register 
        regData = 0;
        status |= MxL68x_Ctrl_ReadRegister(devId, DFE_XTAL_EXT_BIAS_REG, &regData); 
        regData = ((regData & 0x00C0) | 0x000A);  // Program Bit<5:0> with 0x0A 
        status |= MxL68x_Ctrl_WriteRegister(devId, DFE_XTAL_EXT_BIAS_REG, regData); 
        regData |= 0x0080;  // set bit<7>  to 1
        status |= MxL68x_Ctrl_WriteRegister(devId, DFE_XTAL_EXT_BIAS_REG, regData); 
        regData &= 0x007F;  // set bit<7> to 0 
        status |= MxL68x_Ctrl_WriteRegister(devId, DFE_XTAL_EXT_BIAS_REG, regData); 
      }      
      else
      {
        //Changed as per RFIC requirement to drive the voltage swing on slaves up
        status |= MxL68x_Ctrl_WriteRegister(devId, DFE_XTAL_EXT_BIAS_REG, 0x008A); 
      }

      // Add 1 ms waiting time 
      MxLWare68x_OEM_SleepInUs(1000);
      
      // XTAL calibration
      status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x0000); 
      status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x0001);	  
      // Add 1 ms waiting time 
      MxLWare68x_OEM_SleepInUs(1000);	  
      status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x0000);
      
      // Add 70 ms waiting time 
      MxLWare68x_OEM_SleepInUs(70000);

      // Update Device Context
     devContextPtr->xtalFreqInMHz = xtalFreqSel;
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}	

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevOperationalMode
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 4/24/2012
--|
--| DESCRIPTION   :This API configures MxL68x operational mode.
--|                Valid operational modes are Tuner + Demod or Tuner only
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevOperationalMode(   
                UINT8 devId,			
                MXL68x_DEV_MODE_E deviceOpMode)
{
  UINT16 msbRegData = 0;
  UINT16 lsbRegData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevOperationalMode [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      switch(deviceOpMode)
      {
        case MXL68x_TUNER_ONLY: 

          // Disable Demod
          // status |= Ctrl_CfgDemodEnable(devId, MXL_DISABLE);

          // Switch SoC to XTAL clock
          status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_XTAL_CLOCK);

          // Enable Tuner 
          status |= MxL68x_Ctrl_WriteRegister(devId, TUNER_ENABLE_REG, MXL_ENABLE);

          // START_TUNE = 0
          status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, MXL_DISABLE);

          // update Device Context
          devContextPtr->devOpMode = MXL68x_TUNER_ONLY;
          
          break; 

        case MXL68x_TUNER_DEMOD: 

          // unclock gate CPU
          // Read DMD_GPIO_FUNC_MODE_REG high 16 bit  
          status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, &msbRegData);
          status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &lsbRegData);
          lsbRegData &= 0xFFFD; // Set Bit<1> as 0 
          
          // Write high 16 bit 
          status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, msbRegData); 
          // Write low 16 bit 
          status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG, lsbRegData);

          // Enable demodulator clock to access demodulator registers, 0x3003 write 0xF89 
          // Read high 16 bit 
           
          // Read DMD_CCTL_RSTRQ_N_REG, read MSB 
          status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, &msbRegData); 

          // Write high 16 bit 
          status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, msbRegData); 
          // Write low 16 bit 
          status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG, 0x0F89);

          // Switch SoC to XTAL clock
          status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_XTAL_CLOCK);        

          // START_TUNE = 0
          status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, MXL_DISABLE);

          // configure Tuner defaults overwrite
          // Set Tuner Application mode to ISDB-T
          status |= MxLWare68x_API_CfgTunerApplicationMode(devId, MXL68x_DIG_ISDBT);

          // Enable Tuner
          status |= MxL68x_Ctrl_WriteRegister(devId, TUNER_ENABLE_REG, MXL_ENABLE);

          //Update device context OP mode
          devContextPtr->devOpMode = MXL68x_TUNER_DEMOD;

          break; 

        default: status = MXL_INVALID_PARAMETER; break; 
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CfgDevSwitchClock
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/05/2012
--|
--| DESCRIPTION   : This function is used to swtich to PLL or XTAl clock.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_CfgDevSwitchClock(
                       UINT8 devId,
                       MXL68x_CLK_SWITCH_E clkType)
{
  UINT16 k = 0;
  UINT16 msbRegData = 0;
  UINT16 lsbRegData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Ctrl_CfgDevSwitchClock: Clock Switch to %d (0: PLL clock, 1: Xtal clock) [%d]\n", clkType, devId);
  
  if (clkType == MXL68x_PLL_CLOCK)
  {
    while (k < SEQ_STATE_RB_POLLING_TIMES)
    {
      // Check if SEQ_STATE_RB bit<1:0> equals 0x0A3
      status |= MxL68x_Ctrl_ReadRegister(devId, DFE_SEQ_STATE_RB_REG, &lsbRegData);     
      if (((lsbRegData & 0x0F) >= DFE_SEQ_STATE_VALUE) && (status == MXL_SUCCESS))
      {
        MxL_DLL_DEBUG0("Switch to PLL clock 0x%02X \n", lsbRegData);
        break; 
      }
      k++;
						// Wait for 5ms
						MxLWare68x_OEM_SleepInUs(10000);
				}

    if ((k >= SEQ_STATE_RB_POLLING_TIMES) && ((lsbRegData & 0x0F) != DFE_SEQ_STATE_VALUE))
    {
      MxL_DLL_DEBUG0("Err: Switching to PLL clock, SEQ_STATE_RB[0x%x] not equals 0x0A! retry:%d \n",lsbRegData,k);
      status = MXL_FAILURE; 
      devContextPtr->pllClkReady = MXL_FALSE;
    }
  }

  if (MXL_SUCCESS == status)
  {
    // Read high 16 bits 
    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, &msbRegData); 
    // Read low 16 bits 
    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_RSTRQ_N_REG, &lsbRegData);
    
    switch (clkType)
    {
      // Switch to PLL clock, set bit<13> to 1 
      case MXL68x_PLL_CLOCK: 
        lsbRegData |= 0x2000; 
        devContextPtr->pllClkReady = MXL_TRUE;
        break; 

      // Switch to XTAL clock, set bit<13> to 0  
      case MXL68x_XTAL_CLOCK: 
        lsbRegData &= 0xDFFF; 
        devContextPtr->pllClkReady = MXL_FALSE;
        break; 

      default: status = MXL_INVALID_PARAMETER; break;
    }    
    // Write low 16 bit 
    status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, msbRegData);
    status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG, (lsbRegData|0x0008));
  }

		// Wait for 15 ms
		MxLWare68x_OEM_SleepInUs(15000);
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);

  return (MXL_STATUS_E)status; 
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevInit 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 06/06/2012
--|
--| DESCRIPTION   : This fucntion will initilize device settings and should be 
--|                 used before downloading firmware.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevInit(
                UINT8 devId,                 
                MXL68x_IF_SRC_E ifSrc,
                MXL_BOOL_E enableRfLoopThru)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevInit [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      if (devContextPtr->firmwareStatus == MXL68x_FW_STATUS_UNKNOWN)
      {       
        // Enable Tuner
        status = MxL68x_Ctrl_WriteRegister(devId, TUNER_ENABLE_REG, 0x0001);

        // Start tune = 1
        status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, 0x0001); 

        // Switch to PLL Clock
        status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_PLL_CLOCK);
      }

      if (devContextPtr->firmwareStatus != MXL68x_FW_STATUS_UNKNOWN)
      {
        if ((devContextPtr->mxl68xType == MXL681_DEVICE) && (ifSrc != MXL68x_IF_NA))
        {
          // Select IF source
          status |= MxL68x_Ctrl_ReadRegister(devId, IF_FREQ_SEL_REG, &regData);
          regData = (ifSrc == MXL68x_IF_EXTERNAL)? (regData | 0xC0) : (regData & 0x3F);  // Bit<7:6>
          status |= MxL68x_Ctrl_WriteRegister(devId, IF_FREQ_SEL_REG, regData);

          status |= MxL68x_Ctrl_ReadRegister(devId, DIG_ANA_IF_CFG_0, &regData);
          regData = (ifSrc == MXL68x_IF_EXTERNAL)? (regData & 0xBF) : (regData | 0x40);  // Bit<6> = 0 
          status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, regData);

          status |= MxL68x_Ctrl_ReadRegister(devId, DIG_INT_AAF_EN_REG, &regData);
          regData = (ifSrc == MXL68x_IF_EXTERNAL)? (regData & 0xBF) : (regData | 0x40);  // Bit<6> = 0 
          status |= MxL68x_Ctrl_WriteRegister(devId, DIG_INT_AAF_EN_REG, regData);
        }
        
        if(devContextPtr->mxl68xType == MXL683_DEVICE)
        {
          //Loop-through is enabled only for MxL683 Removed PORT_HIGH/LOW logic

          MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevInit: MxL683 LT %d\n",enableRfLoopThru);
          // Configure RF Loop Through option
          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_LT_CONFIG_REG, &regData);
          
          // For MxL683 RF LT EN default is high 
          // Bit<4> = 1, turn on loop through : 0, turn on loop through  
          //If Loop Through 
          if (MXL_FALSE == enableRfLoopThru)
            regData &= 0xFFEF;  
          else 
            regData |= 0x10; 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_LT_CONFIG_REG, regData);  
          devContextPtr->rfLoopThrough = enableRfLoopThru;
        }
        else
        {
          MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevInit: MxL681 LT Disabled\n");

          // Configure RF Loop Through option
          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_LT_CONFIG_REG, &regData);
          
          // For MxL681 disable RF LT 
          // Bit<4> = 0, turn off loop through  
          regData &= 0xFFEF;  

          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_LT_CONFIG_REG, regData);
          devContextPtr->rfLoopThrough = MXL_FALSE;
        }
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevFirmwareDownload 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/17/2012
--|
--| DESCRIPTION   : This function is used to download MxL68x Firmware.  
--|                 If user want to use MxL68xFirmware instead of mbin file, the input 
--|                 parameter definition shall be: 
--|                 mbinBufferPtr = (UINT8 *)MxL68xFirmware; 
--|                 mbinBufferSize = sizeof(MxL68xFirmware);
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevFirmwareDownload( 
                UINT8 devId,
                UINT32 mbinBufferSize,                    
                UINT8 *mbinBufferPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 mailbox = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  UINT8 retry = 0;
  UINT8 retry_max_time = 20;

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevFirmwareDownload [%d]\n",devId);

  // Check for invalid parameters
  if ((devContextPtr) && (mbinBufferPtr) && (mbinBufferSize))
  {
    if (devContextPtr->driverStatus)
    {
        // Check if FW is already loaded or not
        if (devContextPtr->firmwareStatus == MXL68x_FW_STATUS_LOADED)
        {
            status = MXL_SUCCESS;          
        }
        else  //gbiao test...
        {
            // Download firmware only if PLL clock is ready
            if (devContextPtr->pllClkReady == MXL_TRUE)
            {
              // Download Firmware
              HI_INFO_TUNER("---->Download Firmware\n");
              status |= Ctrl_DownloadFirmware(devId, (MBIN_FILE_T*)mbinBufferPtr, (UINT8)MXL_ENABLE);
              if (status == MXL_SUCCESS)
              {
                devContextPtr->firmwareStatus = MXL68x_FW_STATUS_LOADED;
                devContextPtr->fwDownloadPercent = FIRMWARE_DOWNLOAD_DONE_PERCENT; 
              }
              else 
              {
                HI_ERR_TUNER("---->Download Firmware failed.\n");
                devContextPtr->firmwareStatus = MXL68x_FW_STATUS_UNKNOWN;
                devContextPtr->fwDownloadPercent = FIRMWARE_DOWNLOAD_FAIL_PERCENT;
              }
            }
            else
              status = MXL_FAILURE;
        }

         for(retry = 0; retry < retry_max_time; retry++)
         {
          // Wait for 100 ms 
              MxLWare68x_OEM_SleepInUs(10000); 
        	  MxL68x_Ctrl_ReadRegister(devId, 0xA8CC, &mailbox);
             
              if(mailbox!=0xABCD)
              {
        	      MxL_DLL_DEBUG0("FW Download Error Mailbox 0x%x [%d],retry:%d\n",mailbox,devId,retry);
                  MxLWare68x_OEM_ReadRegister(devId, DMD_ID_VERSION_REG, &mailbox);
                  HI_INFO_TUNER("---->read reg:0x%x for test,data:0x%x [0x1012] .\n",DMD_ID_VERSION_REG,mailbox);

                    //status = MXL_FAILURE;
        	    }
              else
              {
        	      MxL_DLL_DEBUG0("FW Download Success Mailbox 0x%x [%d],retry:%d\n",mailbox,devId,retry);
                  break;
              }	  
         }
		 
         if(retry >= retry_max_time)
         {
             HI_ERR_TUNER("---->retry Mailbox up to max time[%d].\n",retry_max_time);
             //status = MXL_FAILURE;
         }
     
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevPowerMode
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/01/2011
--|
--| DESCRIPTION   : Call FW to Set MxL68x power consumption mode
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevPowerMode(
                UINT8 devId,
                MXL68x_PWR_MODE_E powerMode)
{
  UINT8 status = MXL_SUCCESS;
  HOST_COMMAND_T pwrmodeSet;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevPowerMode [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      switch(powerMode)
      {
        case MXL68x_PWR_MODE_ACTIVE: 
          // Populate command header
          FORM_CMD_HEADER(pwrmodeSet, HOST_CMD_DEV_POWER_MODE_ON_CFG, 0, 0);   
          status |= Ctrl_SendHostCommand(devId, &pwrmodeSet);
          
          // Program tuner related registers 
          status |= MxL68x_Ctrl_WriteRegister(devId, TUNER_ENABLE_REG, MXL_ENABLE);
          status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, MXL_ENABLE);
          
          break;

        case MXL68x_PWR_MODE_STANDBY: 
          // Populate command header
          FORM_CMD_HEADER(pwrmodeSet, HOST_CMD_DEV_POWER_MODE_STANDBY_CFG, 0, 0);   
          status |= Ctrl_SendHostCommand(devId, &pwrmodeSet);
          break;
          
        case MXL68x_PWR_MODE_EWS: 
          // Populate command header
          FORM_CMD_HEADER(pwrmodeSet, HOST_CMD_DEV_POWER_MODE_AUTO_EWS_CFG, 0, 0);   
          status |= Ctrl_SendHostCommand(devId, &pwrmodeSet);
          break;

        default:
          status = MXL_INVALID_PARAMETER;
          break;
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}  

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevSoftwareReset
--| 
--| AUTHOR        : Joy Zhang
--|
--| DATE CREATED  : 03/12/2014
--|
--| DESCRIPTION   : This function will reset the device or respective device 
--|                 functional modules based on parameter.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevSoftwareReset(
                UINT8 devId,
                MXL68x_RESET_TYPE_E rstType)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevSoftwareReset, rstType = %d (0: Device Reset, 1: Demod only reset, 2: Tuner only reset) [%d]\n", rstType, devId);
  
  // Perform soft reset
  status = Ctrl_CfgDevReset(devId, rstType);

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CfgDemodEnable
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/09/2011
--|
--| DESCRIPTION   : This function is used to config MxL68x's demod block  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_CfgDemodEnable(UINT8 devId, MXL_BOOL_E demodEnable)
{
  UINT16 msbRegData = 0;
  UINT16 lsbRegData = 0;
  UINT8 status = MXL_SUCCESS;

  MxL_DLL_DEBUG0("Ctrl_CfgDemodEnable, set = %d (0 = disable, 1 = enable) [%d]\n", demodEnable,devId);
  
  status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_DEMRQ_REG - 1, &msbRegData);
  status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_DEMRQ_REG, &lsbRegData);
  
  // Enable or Disable demod
  lsbRegData = (demodEnable == MXL_ENABLE) ? 0x0001 : 0x0000;
  
  status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_DEMRQ_REG - 1, 0x0000); 
  status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_DEMRQ_REG, lsbRegData);  
  
  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevGPIO
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/04/2011
--|
--| DESCRIPTION   : This function config IF out, include IF frequency selection,  
--|                 spectrum polarity AND output level. IF path is fixed to 1. 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevGPIO (
                UINT8 devId,
                MXL68x_GPIO_ID_E gpioId,
                MXL68x_GPIO_TYPE_E  gpioType,
                MXL68x_GPIO_STATE_E gpioState)
{
  UINT32 gpioRegData = 0;
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevGPIO [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Read MSB
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, &regData); 
      gpioRegData = (regData << 16);
      
      // Read LSB
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &regData);  
      gpioRegData |= regData;
      
      // Set corresponding GPIO enable bit, from bit<20> (0 is input, 1 is output)
      if (gpioType == MXL68x_GPIO_OUTPUT) 
      {
        // Enable GPIO as Output
        gpioRegData |= (0x00100000 << (gpioId - 1)); 
        
        // Set GPO as HIGH or LOW
        if (gpioState == MXL68x_GPIO_HIGH)
          gpioRegData |= (0x00002000 << (gpioId - 1)); 
        else
          gpioRegData &= ~(0x00002000 << (gpioId - 1)); 
      }
      else 
      {
        // Disable GPIO output
        gpioRegData &= ~(0x00100000 << (gpioId - 1)); 
      }

      // Write high 16 bit
      regData = (UINT16)((gpioRegData & 0xFFFF0000) >> 16); 
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, regData); 

      // Write low 16 bit 
      regData = (UINT16)(gpioRegData & 0x0FFFF); 
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG, regData); 

    }
    else
      status = MXL_NOT_INITIALIZED;

  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDevVersionInfo
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/01/2011 
--|
--| DESCRIPTION   : This function returns MxL68x Chip revsion information, 
--|                 MxLWare, formware and bootloader etc. version information.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/
  
MXL_STATUS_E MxLWare68x_API_ReqDevVersionInfo(
                UINT8 devId,
                UINT8* chipIdPtr,  
                UINT8* chipVersionPtr,
                UINT8 mxlWareVer[MXL68X_VERSION_SIZE],
                UINT8 firmwareVer[MXL68X_VERSION_SIZE],
                UINT8 bootloaderVer[MXL68X_VERSION_SIZE])
{
  UINT16 lsbRegData = 0;
  UINT8 status = MXL_SUCCESS;
  UINT8 k = 0;
  FW_RESPONSE_T devVerInfoRsp;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDevVersionInfo [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      for (k = 0; k < MXL68X_VERSION_SIZE; k++)
      {
        mxlWareVer[k] = MxLWareDrvVersion[k];
        firmwareVer[k] = 0;
        bootloaderVer[k] = 0;
      }


      MxL_DLL_DEBUG0("MxLWare version: 0x%x.0x%x.0x%x.0x%x\n", 
                                                  mxlWareVer[0], 
                                                  mxlWareVer[1], 
                                                  mxlWareVer[2], 
                                                  mxlWareVer[3]);

      // We need read both 32 bit data at one time.       
      status = MxL68x_Ctrl_ReadRegister(devId, DMD_ID_VERSION_REG - 1, &lsbRegData);
      // Read low 16 bit , bit<15-12>: chip version, Bit<1:0>: Chip ID
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_ID_VERSION_REG, &lsbRegData);

      if (MXL_SUCCESS == status) 
      {
        // 0x02 is MxL683, 0x03 is MxL681
        *chipIdPtr = (lsbRegData & 0x03); 
        *chipVersionPtr = (lsbRegData >> 12) & 0x0F; 
      
        if (devContextPtr->firmwareStatus == MXL68x_FW_STATUS_LOADED)
        {
          // Means firmware has been loaded success before, get version information by FW communication 
          status |= Ctrl_GetDeviceResp(devId, HOST_CMD_DEV_VERSION_INFO_REQ, &devVerInfoRsp);

          if ((status == MXL_SUCCESS) &&
              (devVerInfoRsp.errorCode == RESPONSE_ID_SUCCESS) && 
              (devVerInfoRsp.payloadLen == MXL68X_VERSION_SIZE * 2))
          {
            MxL_DLL_DEBUG0("Received FW version info from Firmware successfully\n");

            // Response Data[0:3] : Firmware version     
            for (k = 0; k < MXL68X_VERSION_SIZE; k++) 
              firmwareVer[k] = devVerInfoRsp.data[k];  

            // Response Data[4:7] : Boot loader version     
            for (k = 0; k < MXL68X_VERSION_SIZE; k++) 
              bootloaderVer[k] = devVerInfoRsp.data[k + MXL68X_VERSION_SIZE];  

            status |= MxL68x_Ctrl_ReadRegister(devId, MAILBOX_REG_FIRMWARE_RC_VER, &lsbRegData);
            //firmwareVer[MXL68X_VERSION_SIZE-1] = (lsbRegData & 0xFF);
            MxL_DLL_DEBUG0("FW version : %u.%u.%u.%u-RC%d \n", firmwareVer[0], firmwareVer[1], firmwareVer[2], firmwareVer[3], lsbRegData);
            MxL_DLL_DEBUG0("BootLoader version: 0x%x.0x%x.0x%x.0x%x\n",
              bootloaderVer[0], bootloaderVer[1],bootloaderVer[2], bootloaderVer[3]);
          }
        } 
      } 
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDevGPIOStatus
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/13/2012
--|
--| DESCRIPTION   : This function is suesd to retrieve status of GPIO pins  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDevGPIOStatus( 
                UINT8 devId,
                MXL68x_GPIO_ID_E gpioId,
                MXL68x_GPIO_TYPE_E  *gpioTypePtr,
                MXL68x_GPIO_STATE_E *gpioStatePtr)
{
  UINT32 regValue, gpioPos; 
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;   
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDevGPIOStatus [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Read high 16 bit firstly 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, &regData); 
      regValue = (UINT32)regData << 16; 
  
      // Read low 16 bit secondly 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &regData);  
      regValue = regValue | regData;     

      // Direction bit location begin from bit<20> to bit<26>
      gpioPos = gpioId + 20 - 1; 
      *gpioTypePtr = (MXL68x_GPIO_TYPE_E)((regValue >> gpioPos) & 0x01);    

      gpioPos = gpioId + 13 - 1; 
      *gpioStatePtr = (MXL68x_GPIO_STATE_E)((regValue >> gpioPos) & 0x01);  
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDevIntrStatus
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 02/20/2012
--|
--| DESCRIPTION   : This function will retrieve MxL68x interrupt source.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDevIntrStatus( 
                UINT8 devId,
                UINT8* intrStatusPtr)
{
  UINT16 readBack = 0; 
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDevIntrStatus [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status = MxL68x_Ctrl_ReadRegister(devId, DMD_INTERRUPT_MAILBOX_REG, &readBack);
      *intrStatusPtr = (UINT8)readBack;
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodBerPer
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 4/21/2011 
--|
--| DESCRIPTION   : Set BER and PER measurement period 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDemodBerPer(
                UINT8 devId, 
                UINT8 numFrames)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodBerPer [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Set BER measurement period, bit<8:0> of DMD_RS_BER_PER_FRAME
      status = MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_FRAME, &regData);
      regData = (regData & 0xFF00) | (numFrames & 0x00FF);
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_RS_BER_PER_FRAME, regData);
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_RS_BER_PER_FRAME_MAILBOX_REG, regData);
      regData = 0;

      // Start Ber stat is moved from BER measurement (toggle 0 to 1)
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_START, &regData);
      regData &= 0xFFFE; // Bit<0> = 0
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_RS_BER_PER_START, regData); 
      regData |= 0x01; // Bit<0> = 1
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_RS_BER_PER_START, regData); 
    }
    else
      status = MXL_NOT_INITIALIZED;

  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodBerPer
--| 
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 9/25/2012
--|
--| DESCRIPTION   : return the BER PER End Flag and the numBer Frames
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodBerPerEndFlag(
                UINT8 devId, 
                UINT8 *numFramesPtr,
                MXL_BOOL_E *berEndFlagPtr)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodBerPerEndFlg [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_END_FLG, &regData);
      if(0x0001 & regData)//Bit<0>
      {
        *berEndFlagPtr = MXL_TRUE;
      }
      else
      {
        *berEndFlagPtr = MXL_FALSE;
      }


      regData = 0;

      // Get BER measurement period, bit<8:0> of DMD_RS_BER_PER_FRAME
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_FRAME, &regData);
      *numFramesPtr = 0x00FF & regData;
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodClearBerPerEndFlag
--| 
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 10/01/2012
--|
--| DESCRIPTION   : Clears the BER End Flag, the flag will be set again after "num frames" duration
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_API_CfgDemodClearBerPerEndFlag(
                UINT8 devId)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodClearBerPerEndFlag [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_CLR, &regData);
      regData = regData | 0xFF00; // To clear set bit<15:8> to 1
      //Clear the BER PER end flag to restart another iteration of BER / PER calculation
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_RS_BER_PER_CLR, regData); 
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodNullPacketFilter 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/04/2012 
--|
--| DESCRIPTION   : Configure filter NULL packet of certain layer 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDemodNullPacketFilter(
	               UINT8 devId, 
	               MXL_BOOL_E enableLayerA,
												    MXL_BOOL_E enableLayerB,
												    MXL_BOOL_E enableLayerC,
												    MXL_BOOL_E enableSkipNullPacket)
{
  UINT16 readBack, control;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodNullPacketFilter [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
						// Read DMD_CNST_CTRL register, TS_Out setting field is on the bit<15:8>. 
						status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CNST_CTRL, &readBack);
						control = (readBack >> 8) & 0x00FF; // Shift high 8 bit to low 8 bit position. 

						if (enableSkipNullPacket == MXL_DISABLE) control |= 0x08; // Bit<3> = 1 
						else control &= 0xF7; // Bit<3> = 0      

						if (enableLayerA == MXL_DISABLE) control |= 0x04; // Bit<2> = 1   
						else control &= 0xFB; // Bit<2> = 0          
				        
						if (enableLayerB == MXL_DISABLE) control |= 0x02; // Bit<1> = 1 
						else control &= 0xFD; // Bit<1> = 0 

						if (enableLayerC == MXL_DISABLE) control |= 0x01; // Bit<0> = 1 
						else control &= 0xFE; // Bit<0> = 0 

						control = ((control << 8) & 0xFF00) | (readBack & 0x00FF); // Move TS_Out related parameter setting to high 8 bit position
						status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CNST_CTRL, control);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E)status;
}

/*---------------------------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodRSDecIterDec 
--| 
--| AUTHOR        : Dong Liu 
--|
--| DATE CREATED  : 12/01/2011
--|
--| DESCRIPTION   : Configure if TS out has to have undergone RS and 
--|                 Iterative decoding.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDemodRSDecIterDec(
	               UINT8 devId, 
                MXL_BOOL_E enableIterativeDecoder,
                MXL_BOOL_E enableRsDecoder)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodRSDecIterDec [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_FEC_OUT_CTRL, &regData);

      // Bit<4> = 1, means before RS
      if (enableRsDecoder == MXL_DISABLE) regData |= 0x0010; 
      else regData &= 0xFFEF; // Bit<4> = 0, default is with RS 

      // DMD_FEC_ITER_CTRL is same address as DMD_FEC_OUT_CTRL
      // Bit<9:8> = 11B, means without interative decoding 
      if (enableIterativeDecoder == MXL_DISABLE)  regData |= 0x0300; 
      else regData &=  0xFCFF; // Bit<9:8> = 00B, default is has iterative decoding

      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_FEC_OUT_CTRL, regData); 
    
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*---------------------------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodMPEGOut
--| 
--| AUTHOR        : Dong Liu, Jay Hung 
--|
--| DATE CREATED  : 05/02/2012, 3/9/2015
--|
--| DESCRIPTION   : This API is used to configure MxL68x MPEG TS out interface
--|                 This API is available only in tuner+demod mode and can be 
--|                 called anytime after configuring device in Tuner+Demod 
--|                 operational mode.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|------------------------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDemodMPEGOut( 
                UINT8 devId,  
                MXL68x_MPEG_MODE_E mpegMode,
                MXL68x_MPEG_CLK_FREQ_E mpegClkFreq,
                MXL68x_MPEG_DATA_FMT_E mpegDataFmt,
                MXL68x_MPEG_CLK_PHASE_E mpegClkPhase,    
                MXL68x_MPEG_POLARITY_E mpegSyncPol,
                MXL68x_MPEG_POLARITY_E mpegValidPol,
                MXL68x_MPEG_SYNC_PULSE_E mpegSyncPulseWidth,
                MXL68x_MPEG_ERROR_IND_E mpegErrIndicator,
                MXL68x_TS_PKT_SIZE_E tsPktSize)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  UINT16 tmpData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodMPEGOut [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_OUT_CFG_REG, &regData);  

      // Configure Normal or 3-Wire mode
      if (mpegMode == MXL68x_MPEG_MODE_NORMAL) regData &= 0xFFFB; // Bit<2> = 0
      else if (mpegMode == MXL68x_MPEG_MODE_3WIRE) regData |= 0x0004; // Bit<2> = 1

      // Configure MPEG clock freq & CLK rate configuration
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_CFG_REG, &tmpData);

      if (mpegClkFreq == MXL68x_MPEG_CLK_64MHZ) tmpData &= 0xFFFE; // Bit<0> = 0
      else if ((mpegClkFreq == MXL68x_MPEG_CLK_32MHZ) || (mpegClkFreq == MXL68x_MPEG_CLK_16MHZ))
		      tmpData |= 0x0001; // Bit<0> = 1

      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_CLK_CFG_REG, tmpData);  
      
      // Set Clock frequency in bit<1:0> 
      regData &= 0xFFFC; // Clear bit<1:0>
      if (mpegClkFreq != MXL68x_MPEG_CLK_NA) regData |= (UINT16)(mpegClkFreq);

      // Config MPEG VALID polarity <8>
      if (mpegValidPol == MXL68x_POLARITY_NORMAL) regData &= 0xFEFF;
      else if (mpegValidPol == MXL68x_POLARITY_INVERTED) regData |= 0x0100;

      // Config MPEG SYNC polarity <6>
      if (mpegSyncPol == MXL68x_POLARITY_NORMAL) regData &= 0xFFBF; // Bit<6> = 0
      else if (mpegSyncPol == MXL68x_POLARITY_INVERTED) regData |= 0x0040;  // Bit<4> = 1

      // Config MPEG Data format <4>
      if (mpegDataFmt == MXL68x_MPEG_DATA_MSB_FIRST) regData &= 0xFFEF; // Bit<4> = 0
      else if (mpegDataFmt == MXL68x_MPEG_DATA_LSB_FIRST) regData |= 0x0010;  // Bit<4> = 1

      // Config TS Packet size <5>
      if (tsPktSize == MXL68x_TS_PKT_204_BYTES) regData &= 0xFFDF; // Bit<5> = 0
      else if(tsPktSize == MXL68x_TS_PKT_188_BYTES) regData |= 0x0020;  // Bit<5> = 1

      // Config MPEG SYNC pulse width <7>
      if (mpegSyncPulseWidth == MXL68x_MPEG_SYNC_PULSE_1BYTE) regData &= 0xFF7F; // Bit<7> = 0
      else if (mpegSyncPulseWidth == MXL68x_MPEG_SYNC_PULSE_1BIT) regData |= 0x0080;  // Bit<7> = 1

      // Config MPEG ERR Indicator <9>
      if (mpegErrIndicator == MXL68x_MPEG_ERROR_IND_ENABLE) regData &= 0xFDFF;  // Bit<9> = 0
      else if (mpegErrIndicator == MXL68x_MPEG_ERROR_IND_DISABLE) regData |= 0x0200; // Bit<9> = 1

      // Config MPEG clock polarity
      switch(mpegClkPhase)
      {
        case MXL68x_MPEG_CLK_PHASE_SHIFT_90_DEG:
        case MXL68x_MPEG_CLK_PHASE_SHIFT_180_DEG:
          regData &= 0xFFF7; // Bit<3> = 0
          break;

        case MXL68x_MPEG_CLK_PHASE_SHIFT_270_DEG:
        case MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG:
          regData |= 0x0008; // Bit<3> = 1 
          break;

        default:
          break;
      }

      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_OUT_CFG_REG, regData);  
      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_OUT_CFG_MAILBOX_REG, regData);

      // Config MPEG CLK Phase <7>
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG-1, &regData);  
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG, &tmpData);  

      switch(mpegClkPhase)
      {
        case MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG:
        case MXL68x_MPEG_CLK_PHASE_SHIFT_180_DEG:
          tmpData &= 0xFF7F; // Bit<7> = 0
          break;

        case MXL68x_MPEG_CLK_PHASE_SHIFT_270_DEG:
        case MXL68x_MPEG_CLK_PHASE_SHIFT_90_DEG:
          tmpData |= 0x0080; // Bit<7> = 1
          break;

        default:
          break;
      }
      
      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_CLK_PHASE_REG - 1, regData);  
      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_CLK_PHASE_REG, tmpData);        
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDemodMPEGTSDriveStrength
--| 
--| AUTHOR        : Jay Hung
--|
--| DATE CREATED  : 3/9/2015
--|
--| DESCRIPTION   :This API configures MxL68x MPEG TS Drive Strength 1X/3X/5X/7X
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDemodMPEGTSDriveStrength(
                UINT8 devId,
                MXL68x_TS_DRV_STR_E tsDriveStrength)
{
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  UINT16 regData_1 = 0, regData_2 = 0;

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDemodMPEGTSDriveStrength [%d]\n",devId);

  // Config MPEG TS Drive Strength: MVAL/MSYN/MDAT/MCLK
  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_TS_DRV_STR_REG_1, &regData_1);  
      status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_TS_DRV_STR_REG_2, &regData_2);  

      switch(tsDriveStrength)
      {
        case MXL68x_TS_DRV_STR_1X:      
          regData_2 &= 0xFFC3; // 0x3009<5:2> = 0
          regData_1 &= 0xFFC3; // 0x3007<5:2> = 0    
          break;          
        case MXL68x_TS_DRV_STR_3X:
          regData_2 &= 0xFFC3; // 0x3009<5:2> = 0
          regData_1 |= 0x003C; // 0x3007<5:2> = 0xf            
          break;
        case MXL68x_TS_DRV_STR_5X:
          regData_2 |= 0x003C; // 0x3009<5:2> = 0xf        
          regData_1 &= 0xFFC3; // 0x3007<5:2> = 0          
          break;
        case MXL68x_TS_DRV_STR_7X:
          regData_2 |= 0x003C; // 0x3009<5:2> = 0xf        
          regData_1 |= 0x003C; // 0x3007<5:2> = 0xf          
          break;
        default:
		  status = MXL_INVALID_PARAMETER;
          break;
      }

	  if(status == MXL_SUCCESS)
	  {
	      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_TS_DRV_STR_REG_1, regData_1);  
	      status |= MxL68x_Ctrl_WriteRegister(devId, MPEG_TS_DRV_STR_REG_2, regData_2);      
	  }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n", status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodCNR 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 2/17/2012
--|
--| DESCRIPTION   : This function returns TS CNR(Signal to Noise Ratio).
--|                 In fixed point implementation, the unit is 0.001dB 
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodCNR(   
                UINT8 devId,
                UINT32* cnrRawPtr,
                SINT32* cnrPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  UINT32 tmp = 0;
  UINT32 cnrRaw = 0;
  UINT32 cnY = 0;
  UINT8 numOfBitsUsed = 0;
  UINT32 cnR = 0;
  HOST_COMMAND_T dmdCnrRead;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  // LUT used while calculating CNR value
  UINT32 cnrLut[32] = 
  {    0,     0,  3010,  4771,  6021,  6990,  7782,  8451, \
    9031,  9542, 10000, 10414, 10792, 11139, 11461, 11761, \
    12041, 12304, 12553, 12788, 13010, 13222, 13424, 13617, \
    13802, 13979, 14150, 14314, 14472, 14624, 14771, 14914 \
  };

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodCNR [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      *cnrPtr = 0 ; 

      //Send Host Command to copy BER PER to mailbox registers
      FORM_CMD_HEADER(dmdCnrRead, HOST_CMD_DEMOD_CNR_RB, 0, 0);

      status |= Ctrl_SendHostCommand(devId, &dmdCnrRead);
      
      // Add 2 ms waiting time 
      MxLWare68x_OEM_SleepInUs(2000);
      
      // Read bit error count before Iterative decoding 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CNR_RB_LO_MAILBOX, &regData);
      cnrRaw = regData; // Low 16 bit 

      // High 16 bit, total 32 bits  
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CNR_RB_HI_MAILBOX, &regData);
      cnrRaw += ((regData) << 16 );

      *cnrRawPtr = cnrRaw;

      if(cnrRaw == 0) 
      { 
        *cnrPtr = 0;
        
        status = MXL_FAILURE; // Means has issue ! 
      } 
      else 
      {
        cnrRaw = (cnrRaw < 340) ? 340 : cnrRaw;
               
        // Calculate number of bits used
        tmp = cnrRaw - 270;

        while (tmp)
        {
          numOfBitsUsed++;                 
          tmp = tmp >> 1;
        }
 
        // Cacluate cnR
        cnR = (numOfBitsUsed >= 5) ?  (numOfBitsUsed - 5) : 0;
        cnY = (cnrRaw - 270) >> cnR;
        cnY = (cnY < 1)? 1 : cnY;  
        cnY = (cnY > 31)? 31: cnY; 
     
        *cnrPtr = (3010 * (23 - cnR)) - 11500 - cnrLut[cnY];
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodLockStatus
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 02/21/2012
--|
--| DESCRIPTION   : This API is used to retrieve TMCC & RS lock of MxL68x demod  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodLockStatus(        
                UINT8 devId,
                MXL_BOOL_E *tmccLockPtr,				
                MXL_BOOL_E *layerARSLockPtr,
                MXL_BOOL_E *layerBRSLockPtr,
                MXL_BOOL_E *layerCRSLockPtr)
{
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  HI_INFO_TUNER("MxLWare68x_API_ReqDemodLockStatus [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // TMCC lock status
      if (tmccLockPtr)
      { 
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_STATUS, &regData);
        if (tmccLockPtr) *tmccLockPtr = (MXL_BOOL_E)((regData >> 2) & 0x01); // Bit <2>
      }

      // Layer A & B RS lock status
      if ((layerARSLockPtr) || (layerBRSLockPtr))
      {
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_LOCK_A_B, &regData);
        
        if (layerARSLockPtr) *layerARSLockPtr = (MXL_BOOL_E)((regData >> 14) & 0x01); // Bit<14>
        if (layerBRSLockPtr) *layerBRSLockPtr = (MXL_BOOL_E)((regData >> 15) & 0x01); // Bit<15>
      }

      // Layer C RS lock status
      if (layerCRSLockPtr)
      {
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_LOCK_C, &regData);
        *layerCRSLockPtr = (MXL_BOOL_E)(regData & 0x01); // Bit<0>
      }    
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  HI_INFO_TUNER("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodNullPacketFilter
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 10/10/2012
--|
--|
--| DESCRIPTION   : Get current TS data output property, such as enable or
--|                 disable NULL packet data output for every layer 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodNullPacketFilter(
                UINT8 devId, 
											 				MXL_BOOL_E* enableLayerAPtr,
												 			MXL_BOOL_E* enableLayerBPtr,
																MXL_BOOL_E* enableLayerCPtr,
																MXL_BOOL_E* enableSkipNullPacketPtr)
{
  UINT16 readBack;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodNullPacketFilter [%d]\n",devId); 

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    { 
		    // Read DMD_CNST_CTRL firstly. 
		    status = MxL68x_Ctrl_ReadRegister(devId, DMD_CNST_CTRL, &readBack);
		    if (MXL_SUCCESS == status)
		    {
				    readBack = (readBack >> 8) & 0x00FF;
				    *enableLayerCPtr = (readBack & 0x01)? MXL_DISABLE : MXL_ENABLE;   // Bit<0>
				    *enableLayerBPtr = ((readBack & 0x02) >> 1)? MXL_DISABLE : MXL_ENABLE; // Bit<1>  
				    *enableLayerAPtr = ((readBack & 0x04) >> 2)? MXL_DISABLE : MXL_ENABLE; // Bit<2> 
				    *enableSkipNullPacketPtr = ((readBack & 0x08) >> 3)? MXL_DISABLE : MXL_ENABLE; // Bit<3>  
		    }
		    else 
		    {
				    *enableSkipNullPacketPtr = MXL_DISABLE;
				    *enableLayerAPtr = MXL_DISABLE;
				    *enableLayerBPtr = MXL_DISABLE;
				    *enableLayerCPtr = MXL_DISABLE;
		    }
		    MxL_DLL_DEBUG0("Skip NULL packet enable = %d, Layer A = %d, Layer B = %d, Layer C = %d \n",
					    *enableSkipNullPacketPtr, *enableLayerAPtr, *enableLayerBPtr, *enableLayerCPtr);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodRsIterSetting
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 10/10/2012
--|
--| DESCRIPTION   : Get current RS decoder and Iterative decoding setting, 
--|                 with or without these features.    
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodRsIterSetting(
                UINT8 devId, 
																MXL_BOOL_E* enableIterativeDecoder, 
																MXL_BOOL_E* enableRsDecoder)
{
  UINT16 readBack;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodRsIterSetting [%d]\n",devId); 

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    { 
		    status = MxL68x_Ctrl_ReadRegister(devId, DMD_FEC_OUT_CTRL, &readBack);

		    // Check Bit<4> of DMD_FEC_OUT_CTRL register for RS decoder enable or disable setting 
		    if ((readBack & 0x10) == 0x10) *enableRsDecoder = MXL_DISABLE; // Without RS decoder 
		    else *enableRsDecoder = MXL_ENABLE; // With RS decoder 

		    // Check Bit<9:8> of DMD_FEC_OUT_CTRL for Iterative decoding 
		    if ((readBack & 0x0300) == 0x0300) 
				    *enableIterativeDecoder = MXL_DISABLE; // Without interative
		    else if ((readBack & 0x0300) == 0) 
				    *enableIterativeDecoder = MXL_ENABLE; 
		    else *enableIterativeDecoder = MXL_DISABLE; 

		    MxL_DLL_DEBUG0("RS decoder enable = %d, Iterative decoder enable = %d \n",
				    *enableRsDecoder, *enableIterativeDecoder);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodMpegOutSet
--| 
--| AUTHOR        : Dong Liu, Jay Hung
--|
--| DATE CREATED  : 10/10/2012, 3/9/2015
--|
--| DESCRIPTION   : Get current Mpeg Out Setting parameters 
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodMpegOutSet(
                UINT8 devId,
                MXL68x_MPEG_MODE_E *mpegModePtr,
                MXL68x_MPEG_CLK_FREQ_E *mpegClkFreqPtr,
                MXL68x_MPEG_DATA_FMT_E *mpegDataFmtPtr,
                MXL68x_MPEG_CLK_PHASE_E *mpegClkPhasePtr,    
                MXL68x_MPEG_POLARITY_E *mpegSyncPolPtr,
                MXL68x_MPEG_POLARITY_E *mpegValidPolPtr, 
                MXL68x_MPEG_SYNC_PULSE_E *mpegSyncPulseWidthPtr,         
                MXL68x_MPEG_ERROR_IND_E *mpegErrIndicatorPtr,      
                MXL68x_TS_PKT_SIZE_E *tsPktSizePtr)
{
  UINT16 readBack, clkRotate, clkInvert;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodMpegOutSet [%d]\n",devId); 

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    { 
		    status = MxL68x_Ctrl_ReadRegister(devId, MPEG_OUT_CFG_REG, &readBack);
		    readBack = (readBack & 0x03FF);

		    *mpegClkFreqPtr = (MXL68x_MPEG_CLK_FREQ_E)(readBack & 0x03); // Bit<1:0>
      *mpegModePtr = (MXL68x_MPEG_MODE_E)((readBack & 0x04) >> 2); // Bit<2>
		    clkInvert = (readBack & 0x08) >> 3; // Bit<3>  
		    *mpegDataFmtPtr = (MXL68x_MPEG_DATA_FMT_E)((readBack & 0x10) >> 4); // Bit<4> 
		    *tsPktSizePtr = (MXL68x_TS_PKT_SIZE_E)((readBack & 0x20) >> 5); // Bit<5>
		    *mpegSyncPolPtr = (MXL68x_MPEG_POLARITY_E)((readBack & 0x40) >> 6); // Bit<6>
		    *mpegSyncPulseWidthPtr = (MXL68x_MPEG_SYNC_PULSE_E)((readBack & 0x80) >> 7); // Bit<7>
		    *mpegValidPolPtr = (MXL68x_MPEG_POLARITY_E)((readBack & 0x100) >> 8); // Bit<8>
		    *mpegErrIndicatorPtr = (MXL68x_MPEG_ERROR_IND_E)((readBack & 0x200) >> 9); // Bit<9>

		    status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG - 1, &readBack);
		    status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_CLK_PHASE_REG, &readBack);
		    clkRotate = (readBack & 0x0080) >> 7;

      if ((clkRotate == 0) && (clkInvert == 1))
        *mpegClkPhasePtr = MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG; 
      else if ((clkRotate == 1) && (clkInvert == 0))
        *mpegClkPhasePtr = MXL68x_MPEG_CLK_PHASE_SHIFT_90_DEG; 
      else if ((clkRotate == 0) && (clkInvert == 0))
        *mpegClkPhasePtr = MXL68x_MPEG_CLK_PHASE_SHIFT_180_DEG; 
      else if ((clkRotate == 1) && (clkInvert == 1))
        *mpegClkPhasePtr = MXL68x_MPEG_CLK_PHASE_SHIFT_270_DEG; 
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodMPEGTSDriveStrength
--| 
--| AUTHOR        : Jay Hung
--|
--| DATE CREATED  : 3/9/2015
--|
--| DESCRIPTION   :This API is used to retrieve MxL68x MPEG TS Drive Strength Status
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodMPEGTSDriveStrength(
                UINT8 devId,
                MXL68x_TS_DRV_STR_E *tsDriveStrengthPtr)
{
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  UINT16 readBack, tsDrvStr1, tsDrvStr2;

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodMPEGTSDriveStrength [%d]\n", devId);

  // Get MPEG TS Drive Strength Status: MVAL/MSYN/MDAT/MCLK
  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
		status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_TS_DRV_STR_REG_1, &readBack);
		tsDrvStr1 = (readBack & 0x003c) >> 2; // 0x3007<5:2>
		status |= MxL68x_Ctrl_ReadRegister(devId, MPEG_TS_DRV_STR_REG_2, &readBack);
		tsDrvStr2 = (readBack & 0x003c) >> 2; // 0x3009<5:2> 

		if(status == MXL_SUCCESS)
		{
			if ((tsDrvStr2 == 0) && (tsDrvStr1 == 0))
			  *tsDriveStrengthPtr = MXL68x_TS_DRV_STR_1X; 
			else if ((tsDrvStr2 == 0) && (tsDrvStr1 == 0xF))
			  *tsDriveStrengthPtr = MXL68x_TS_DRV_STR_3X; 
			else if ((tsDrvStr2 == 0xF) && (tsDrvStr1 == 0))
			  *tsDriveStrengthPtr = MXL68x_TS_DRV_STR_5X; 
			else if ((tsDrvStr2 == 0xF) && (tsDrvStr1 == 0xF))
			  *tsDriveStrengthPtr = MXL68x_TS_DRV_STR_7X;	  
			else  *tsDriveStrengthPtr = MXL68x_TS_DRV_STR_NA;	  

			MxL_DLL_DEBUG0("TS Drive Strength = %d\n", *tsDriveStrengthPtr);
		}
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n", status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodBER
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/01/2011
--|
--| DESCRIPTION   : This function returns TS BER(Bit Error Rate). 
--|                 Include with/without Iterative decoding 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodBER(
                UINT8 devId,   
                MXL68x_BER_T *layerABerPtr,    
                MXL68x_BER_T *layerBBerPtr,
                MXL68x_BER_T *layerCBerPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  MXL_BOOL_E tmccLock;
  UINT8 mode = 0;
  UINT8 seg = 0;
  UINT8 cr = 0;
  UINT8 numCR = 0;
  UINT8 denCR = 1;
  UINT8 mod = 0;
  UINT8 i = 0;
  MXL68x_ISDBT_MODE_E isdbtModeInfo;   
  MXL68x_TMCC_INFO_T tmccInfo[MXL68x_ISDBT_LAYER_ALL];
  MXL68x_BER_T *berInfoPtr[MXL68x_ISDBT_LAYER_ALL] = {NULL, NULL, NULL}; 
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  HOST_COMMAND_T dmdBerperRead;

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodBER [%d]\n",devId);

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    { 
      // validate input pointers
      if ((layerABerPtr) || (layerBBerPtr) || (layerCBerPtr))
      {
        // Check TMCC lock status
        status |= MxLWare68x_API_ReqDemodLockStatus(devId, &tmccLock, NULL, NULL, NULL);

        MxL_DLL_DEBUG0(" Check Demod Lock = %d \n", tmccLock); 

        if (tmccLock == MXL_ON)
        {
          // Check if BER cal done flag else BER value will be invalid
          status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_END_FLG, &regData);
          
          if (regData == 1)
          {
           
            // Get Modulation, CR, Mode & segment info for all layers
            status |= MxLWare68x_API_ReqDemodTMCCParams(devId,
                                                        NULL,
                                                        NULL,
                                                        &isdbtModeInfo,
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_A],
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_B],
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_C]);


            if (status == MXL_SUCCESS)
            {
              berInfoPtr[MXL68x_ISDBT_LAYER_A] = layerABerPtr;
              berInfoPtr[MXL68x_ISDBT_LAYER_B] = layerBBerPtr;
              berInfoPtr[MXL68x_ISDBT_LAYER_C] = layerCBerPtr;

              for (i = 0; i < MXL68x_ISDBT_LAYER_ALL; i++)
              {
                if (berInfoPtr[i])
                {
                  // Get mode info
                  if (isdbtModeInfo <= MXL68x_ISDBT_MODE3)
                  { 
                    mode = (((UINT8)isdbtModeInfo) << 1);
                    mode = (mode==0) ? 1 : mode;
                  }
                  else
                  { 
                    status = MXL_FAILURE;
                  }
                  
                  // Get CR
                  if (tmccInfo[i].codeRateInfo <= MXL68x_CR_7_8)
                  {
                    cr = (UINT8)tmccInfo[i].codeRateInfo;
                    Ctrl_AssignNumDenCR(&numCR, &denCR, cr);
                  }
                  else
                  {
                    //status = MXL_FAILURE; //gbiao
                  }

                  // Get modulation
                  if (tmccInfo[i].modulationInfo <= MXL68x_MODULATION_QAM64)
                  {
                      mod = (((UINT8)tmccInfo[i].modulationInfo)<< 1);
                  }
                  else
                  {
                      //status = MXL_FAILURE; //gbiao
                  }

                  // Get number of segments
                  if (tmccInfo[i].numOfSegments)
                  {
                      seg = tmccInfo[i].numOfSegments;
                  }
                  else
                  {
                      status = MXL_FAILURE;
                  }

                  status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_FRAME, &regData);

                  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodBER:%d: index %d, mode %d, seg %d, mod %d, numCR %d, denCR %d, frm %d\n",__LINE__, i, mode, seg, mod, numCR, denCR, (regData & 0x0F));

                  if (seg >= 255)  // Means there is no signal on this layer 
                  {
                    berInfoPtr[i]->numOfBits = 0;
                    berInfoPtr[i]->bitErrCount_WO_Iter = 0;
                    berInfoPtr[i]->bitErrCount_W_Iter = 0;
                  }
                  else 
                  {
                    // Calculate Number of Bits used for BER calculation
                    berInfoPtr[i]->numOfBits = (UINT32)((96 * (mode) * (seg) * numCR * mod * 204 * (regData & 0xFF))/denCR);
                    berInfoPtr[i]->numOfBits = berInfoPtr[i]->numOfBits;

                    if(MXL68x_ISDBT_LAYER_A == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_A_BER_RB, 0, 0);
                    }
                    else if(MXL68x_ISDBT_LAYER_B == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_B_BER_RB, 0, 0);
                    }
                    else if(MXL68x_ISDBT_LAYER_C == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_C_BER_RB, 0, 0);
                    } 

                    status |= Ctrl_SendHostCommand(devId, &dmdBerperRead);
                    // Add 1 ms waiting time 
                    MxLWare68x_OEM_SleepInUs(2000);
                    
                    // Read bit error count before Iterative decoding 
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_DATA_LO_MAILBOX, &regData);
                    berInfoPtr[i]->bitErrCount_WO_Iter = regData; // Low 16 bit 

                    // High 8 bit, total 32 bits  
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_DATA_HI_MAILBOX, &regData);
                    berInfoPtr[i]->bitErrCount_WO_Iter += ((regData) << 16 );

                    // Read bit error count after Iterative decoding  
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_ITER_DATA_LO_MAILBOX, &regData);
                    berInfoPtr[i]->bitErrCount_W_Iter = regData; // Low 16 bit 

                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_ITER_DATA_HI_MAILBOX, &regData);
                    berInfoPtr[i]->bitErrCount_W_Iter += ((regData) << 16 );
                  }                  

                  MxL_DLL_DEBUG0(" Layer %d, total bit = %d,  BER Cnt_WO_Iter = %d, BER Cnt_W_Iter = %d , status = %d \n", 
                                 i, berInfoPtr[i]->numOfBits, berInfoPtr[i]->bitErrCount_WO_Iter, berInfoPtr[i]->bitErrCount_W_Iter, status);
                }
              }
            }
          } 
          else
          {
            for (i = 0; i < MXL68x_ISDBT_LAYER_ALL; i++)
            {
              if (berInfoPtr[i])
              {
                berInfoPtr[i]->numOfBits = 0;
                berInfoPtr[i]->bitErrCount_WO_Iter = 0;
                berInfoPtr[i]->bitErrCount_W_Iter = 0;
              }
            }
            status = MXL_FAILURE;
          }
        }
        else
          status = MXL_FAILURE;
      }  
      else
        status = MXL_INVALID_PARAMETER;
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodBER, return status %d (0 = success) [%d]\n", status,devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodPER
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/13/2012
--|
--| DESCRIPTION   : This API is used to calculate PER value of MxL68x demod. 
--|                 PER = layerAPktErrCount->pktErrCount_WO_ITERPtr /
                          layerAPktErrCount->numOfPkts
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodPER(
                UINT8 devId,   
                MXL68x_PER_T *layerAPktErrCountPtr,    
                MXL68x_PER_T *layerBPktErrCountPtr,
                MXL68x_PER_T *layerCPktErrCountPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  MXL_BOOL_E tmccLock;
  UINT8 mode = 0;
  UINT8 seg = 0;
  UINT8 cr = 0;
  UINT8 numCR = 0;
  UINT8 denCR = 1;
  UINT8 mod = 0;
  UINT8 i = 0;
  MXL68x_ISDBT_MODE_E isdbtModeInfo;   
  MXL68x_TMCC_INFO_T tmccInfo[MXL68x_ISDBT_LAYER_ALL];
  MXL68x_PER_T       *perInfoPtr[MXL68x_ISDBT_LAYER_ALL] ={NULL, NULL,NULL}; 
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  HOST_COMMAND_T dmdBerperRead;

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodPER [%d]\n",devId);

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    { 
      // validate input pointers
      if ((layerAPktErrCountPtr) || (layerBPktErrCountPtr) || (layerCPktErrCountPtr))
      {
        // Check TMCC lock status
        status |= MxLWare68x_API_ReqDemodLockStatus(devId, &tmccLock, NULL, NULL, NULL);
        MxL_DLL_DEBUG0(" Check Demod Lock = %d \n", tmccLock); 

        if (tmccLock == MXL_ON)
        {
          // Check if PER cal done flag else PER value will be invalid
          status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_END_FLG, &regData);

          if (regData == 1)
          {

            // Get Modulation, CR, Mode & segment info for all layers
            status |= MxLWare68x_API_ReqDemodTMCCParams(devId,
                                                        NULL,
                                                        NULL,
                                                        &isdbtModeInfo,
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_A],
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_B],
                                                        &tmccInfo[MXL68x_ISDBT_LAYER_C]);


            if (status == MXL_SUCCESS)
            {
              perInfoPtr[MXL68x_ISDBT_LAYER_A] = layerAPktErrCountPtr;
              perInfoPtr[MXL68x_ISDBT_LAYER_B] = layerBPktErrCountPtr;
              perInfoPtr[MXL68x_ISDBT_LAYER_C] = layerCPktErrCountPtr;

              for (i = 0; i < MXL68x_ISDBT_LAYER_ALL; i++)
              {
                if (perInfoPtr[i])
                {
                  // Get mode info
                  if (isdbtModeInfo <= MXL68x_ISDBT_MODE3)
                  { 
                    mode = (((UINT8)isdbtModeInfo) << 1);
                    mode = (mode==0) ? 1 : mode;
                  }
                  else
                  { 
                    status = MXL_FAILURE;
                  }

                  // Get CR
                  if (tmccInfo[i].codeRateInfo <= MXL68x_CR_7_8)
                  {
                    cr = (UINT8)tmccInfo[i].codeRateInfo;
                    Ctrl_AssignNumDenCR(&numCR, &denCR, cr);
                  }
                  else
                  {
                    status = MXL_FAILURE;
                  }

                  // Get modulation
                  if (tmccInfo[i].modulationInfo <= MXL68x_MODULATION_QAM64) mod = (((UINT8)tmccInfo[i].modulationInfo) << 1);
                  else status = MXL_FAILURE;

                  // Get segment
                  if (tmccInfo[i].numOfSegments) seg = tmccInfo[i].numOfSegments;
                  else status = MXL_FAILURE;

                  if (seg >= 255)  // Means there is no signal on this layer 
                  {
                    perInfoPtr[i]->numOfPkts = 0;
                    perInfoPtr[i]->pktErrCount_WO_Iter = 0;
                    perInfoPtr[i]->pktErrCount_W_Iter = 0;
                  }
                  else 
                  {
                    // retrieve number of frames
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_FRAME, &regData);

                    // Calculate Number of Frames used for PER calculation
                    perInfoPtr[i]->numOfPkts = (UINT32)(96 * (mode) * (seg) * numCR * mod * (regData & 0xFF));
                    perInfoPtr[i]->numOfPkts = (UINT32)(perInfoPtr[i]->numOfPkts/(denCR*8));

                    if(MXL68x_ISDBT_LAYER_A == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_A_PER_RB, 0, 0);
                    }
                    if(MXL68x_ISDBT_LAYER_B == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_B_PER_RB, 0, 0);
                    }
                    if(MXL68x_ISDBT_LAYER_C == i)
                    {
                      //Send Host Command to copy BER PER to mailbox registers
                      FORM_CMD_HEADER(dmdBerperRead, HOST_CMD_DEMOD_LAYER_C_PER_RB, 0, 0);
                    }  

                    status |= Ctrl_SendHostCommand(devId, &dmdBerperRead);

                    // Add 1 ms waiting time 
                    MxLWare68x_OEM_SleepInUs(2000);
                    
                    // Read bit error count before Iterative decoding 
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_DATA_LO_MAILBOX, &regData);
                    perInfoPtr[i]->pktErrCount_WO_Iter = regData; // Low 16 bit 

                    // High 8 bit, total 32 bits  
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_DATA_HI_MAILBOX, &regData);
                    perInfoPtr[i]->pktErrCount_WO_Iter += ((regData & 0xFF) << 16 );

                    // Read bit error count after Iterative decoding  
                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_ITER_DATA_LO_MAILBOX, &regData);
                    perInfoPtr[i]->pktErrCount_W_Iter = regData; // Low 16 bit 

                    status |= MxL68x_Ctrl_ReadRegister(devId, DMD_RS_BER_PER_ITER_DATA_HI_MAILBOX, &regData);
                    perInfoPtr[i]->pktErrCount_W_Iter += ((regData & 0xFF) << 16 );
                  }

                  MxL_DLL_DEBUG0(" Layer %d, Total PKT = %d, PER Cnt_WO_Iter = %d, PER Cnt_W_Iter = %d \n", 
                                 i, perInfoPtr[i]->numOfPkts, perInfoPtr[i]->pktErrCount_WO_Iter, perInfoPtr[i]->pktErrCount_W_Iter); 
                }
              }            
            }
          }
          else
          {
            for (i = 0; i < MXL68x_ISDBT_LAYER_ALL; i++)
            {
              if (perInfoPtr[i])
              {
                perInfoPtr[i]->numOfPkts = 0;
                perInfoPtr[i]->pktErrCount_WO_Iter = 0;
                perInfoPtr[i]->pktErrCount_W_Iter = 0;
              }
            }
            status = MXL_FAILURE;
          }
        }
        else
          status = MXL_FAILURE;
      }  
      else
        status = MXL_INVALID_PARAMETER;

    }
    else
      status = MXL_NOT_INITIALIZED;

  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodTMCCParams
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/01/2011
--|
--| DESCRIPTION   : This API is used to retrieve various demod status 
--|                 including TMCC parameters 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodTMCCParams(
                UINT8 devId,
                MXL_BOOL_E *ewsAlarmPtr,   
                MXL68x_GUARD_INTERVAL_E *guardIntervalInfoPtr, 
                MXL68x_ISDBT_MODE_E *isdbtModeInfoPtr,   
                MXL68x_TMCC_INFO_T *tmccInfoLayerAPtr,  
                MXL68x_TMCC_INFO_T *tmccInfoLayerBPtr,              
                MXL68x_TMCC_INFO_T *tmccInfoLayerCPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 demodStatusReg0 = 0;
  UINT16 demodStatusReg1 = 0;
  UINT16 demodStatusReg2 = 0;
  UINT16 demodStatusReg3 = 0;
  UINT16 tmp = 0;
  MXL68x_TMCC_INFO_T *tmccInfo[MXL68x_ISDBT_LAYER_ALL];
  UINT8 i = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MXL68x_TMCC_REG_MASK tmccRegMask[MXL68x_ISDBT_LAYER_ALL] = 
  {
    {
      LAYER_A_MODULATION_MASK, LAYER_A_CR_MASK, LAYER_A_IL_MASK, LAYER_A_SEG_MASK,
      LAYER_A_MODULATION_BIT_SHIFT, LAYER_A_CR_BIT_SHIFT, LAYER_A_IL_BIT_SHIFT, 
      LAYER_A_SEG_BIT_SHIFT
    },
    {
      LAYER_B_MODULATION_MASK, LAYER_B_CR_MASK, LAYER_B_IL_MASK, LAYER_B_SEG_MASK,
      LAYER_B_MODULATION_BIT_SHIFT, LAYER_B_CR_BIT_SHIFT, LAYER_B_IL_BIT_SHIFT,
      LAYER_B_SEG_BIT_SHIFT
    },
    {
      LAYER_C_MODULATION_MASK, LAYER_C_CR_MASK, LAYER_C_IL_MASK, LAYER_C_SEG_MASK,
      LAYER_C_MODULATION_BIT_SHIFT, LAYER_C_CR_BIT_SHIFT, LAYER_C_IL_BIT_SHIFT,
      LAYER_C_SEG_BIT_SHIFT
    }
  };

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodTMCCParams [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_TMCC_INFO_REG_0, &demodStatusReg0);

      // Emergency Alaram Signal status <2>
      if (ewsAlarmPtr)
      {
        if (demodStatusReg0 & 0x0004) *ewsAlarmPtr = MXL_ON;
        else *ewsAlarmPtr = MXL_OFF;
      }

      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_STATUS_INFO_REG, &tmp);

      // Guard Interval info <1:0>
      if (guardIntervalInfoPtr) 
        *guardIntervalInfoPtr = (MXL68x_GUARD_INTERVAL_E)(tmp & 0x0003);

      // ISDB-T mode
      if (isdbtModeInfoPtr)
        *isdbtModeInfoPtr = (MXL68x_ISDBT_MODE_E)(((tmp & 0x000C) >> 2)-1);
      
      // TMCC information
      if ((tmccInfoLayerAPtr) || (tmccInfoLayerBPtr) || (tmccInfoLayerCPtr))
      {
        // Readback TMCC status registers
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_TMCC_INFO_REG_1, &demodStatusReg1);
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_TMCC_INFO_REG_2, &demodStatusReg2);
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_TMCC_INFO_REG_3, &demodStatusReg3);

        tmccInfo[0] = tmccInfoLayerAPtr;
        tmccInfo[1] = tmccInfoLayerBPtr;
        tmccInfo[2] = tmccInfoLayerCPtr;

        for (i = 0; i <= MXL68x_ISDBT_LAYER_C; i++)
        {
          if (tmccInfo[i])
          {
            // Modulation information
            tmp = (demodStatusReg0 & tmccRegMask[i].modulationMask) >> tmccRegMask[i].modulationShift;
            if (tmp <= MXL68x_MODULATION_QAM64) tmccInfo[i]->modulationInfo = (MXL68x_MODULATION_E)tmp;
            else tmccInfo[i]->modulationInfo = MXL68x_MODULATION_UNKNOWN;
            
            // Code Rate information
            tmp = (demodStatusReg1 & tmccRegMask[i].codeRateMask) >> tmccRegMask[i].codeRateShift;
            if (tmp <= MXL68x_CR_7_8) tmccInfo[i]->codeRateInfo = (MXL68x_CODE_RATE_E)tmp;
            else tmccInfo[i]->codeRateInfo = MXL68x_CR_UNKNOWN;

            // Interleaver information
            if (i == MXL68x_ISDBT_LAYER_A)
              tmp = (demodStatusReg1 & tmccRegMask[i].interleaverMask) >> tmccRegMask[i].interleaverShift;
            else
              tmp = (demodStatusReg2 & tmccRegMask[i].interleaverMask) >> tmccRegMask[i].interleaverShift;

            if (tmp <= MXL68x_CR_7_8) tmccInfo[i]->interleaverLenInfo = (MXL68x_INTERLEAVER_LEN_E)tmp;
            else tmccInfo[i]->interleaverLenInfo = MXL68x_INTERLEAVER_UNKNOWN;

            // Number of Segments 
            if (i == MXL68x_ISDBT_LAYER_C)
              tmp = (demodStatusReg3 & tmccRegMask[i].segmentMask) >> tmccRegMask[i].segmentShift;
            else
              tmp = (demodStatusReg2 & tmccRegMask[i].segmentMask) >> tmccRegMask[i].segmentShift;

            if ((tmp) && (tmp < 14)) tmccInfo[i]->numOfSegments = (UINT8)tmp;
            else tmccInfo[i]->numOfSegments = 0xFF;

            MxL_DLL_DEBUG0("Layer %d : mod = %d, CR = %d, Interleave = %d, Segment = %d \n",
             i, tmccInfo[i]->modulationInfo,tmccInfo[i]->codeRateInfo,tmccInfo[i]->interleaverLenInfo,tmccInfo[i]->numOfSegments);
          }
        }
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodAcData
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 2/22/2012
--|
--| DESCRIPTION   : Init call back function or read AC data from stored structure.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodAcData(
	               UINT8 devId, 
                UINT8 *acDataPtr)
{
  UINT16 k;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodAcData [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      if (devContextPtr->fwIntRetData.ValidFlag == MXL_VALID)
      {
        // Read saved AC datsa to return parameter structure 
        for (k = 0; k < devContextPtr->fwIntRetData.DataLength ; k++)
          acDataPtr[k] = devContextPtr->fwIntRetData.Data[k];

        devContextPtr->fwIntRetData.ValidFlag = MXL_INVALID;
      }
      else
      {
        status = MXL_FAILURE;
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;
  
  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodFreqOffset
--| 
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 2/22/2013
--|
--| DESCRIPTION   : Function to RB Freq Off Parameters
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodFreqOffset(
	               UINT8 devId, 
	               SINT32 *carrierFreqOff,
	               SINT32 *samplingFreqOff)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodFreqOffset [%d]\n",devId);

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    {
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CARRIER_FREQ_OFF_LO_MAILBOX, &regData);
        *carrierFreqOff = regData; // Low 16 bit 

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CARRIER_FREQ_OFF_HI_MAILBOX, &regData);
        *carrierFreqOff += ((regData & 0xFFFF) << 16 );

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_SAMP_FREQ_OFF_LO_MAILBOX, &regData);
        *samplingFreqOff = regData; // Low 16 bit 

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_SAMP_FREQ_OFF_HI_MAILBOX, &regData);
        *samplingFreqOff += ((regData & 0xFFFF) << 16 );
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E) status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodFreqOffsetInKHzAndPPM
--| 
--| AUTHOR        : Prithvi Thomas
--|                 Dong Liu 
--|
--| DATE CREATED  : 2/26/2013
--|                 03/16/2013   
--|
--| DESCRIPTION   : Function to RB Freq Off Parameters in Khz and PPM
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodFreqOffsetInKHzAndPPM(
	               UINT8 devId, 
                SINT32 *carrierFreqOffInKHz,
                SINT32 *samplingFreqOffInPPM)
{
  SINT64 temp = 0;
  SINT32 carrierFreqOff,samplingFreqOff;
  UINT16 regData = 0;
  UINT8 status = MXL_SUCCESS;

  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodFreqOffsetInKHzAndPPM [%d]\n",devId);

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    {
        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CARRIER_FREQ_OFF_LO_MAILBOX, &regData);
        carrierFreqOff = regData; // Low 16 bit 

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CARRIER_FREQ_OFF_HI_MAILBOX, &regData);
        carrierFreqOff |= ((regData & 0xFFFF) << 16 );

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_SAMP_FREQ_OFF_LO_MAILBOX, &regData);
        samplingFreqOff = regData; // Low 16 bit 

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_SAMP_FREQ_OFF_HI_MAILBOX, &regData);
        samplingFreqOff |= ((regData & 0xFFFF) << 16 );

        temp = (SINT64)(carrierFreqOff)*16000LL;
        temp = temp >> 28;
        *carrierFreqOffInKHz = (SINT32)(temp);

        temp = (SINT64)(samplingFreqOff & 0x3FFFFFFF); // Take only 30 bits
        temp = (temp * -507937LL) >> 30;
        *samplingFreqOffInPPM = (SINT32)(temp+492064LL);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E) status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqDemodSpectrumInversion 
--| 
--| AUTHOR        : Prithvi Thomas 
--|
--| DATE CREATED  : 12/10/2012  
--|
--| DESCRIPTION   : This returns the status of spectrum inversion. This api is to
--|                 be called only if the Frame Sync is currently achieved. If this
--|                 API is called when there is no frame sync, the value returned 
--|                 may not reflect the right state of spectrum inversion
--|   
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqDemodSpectrumInversion(
                UINT8 devId,
                MXL_BOOL_E *isSpectrumInverted)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqDemodSpectrumInversion [%d]\n",devId);

  //Make Default false
  *isSpectrumInverted = MXL_FALSE;

  if (devContextPtr) 
  {
    if (devContextPtr->driverStatus)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, DFE_GCOMP_IQ_SWITCH, &regData);

      if(regData & (1<<7))//Bit 7 is IQ Swicth Indicator
      {
        *isSpectrumInverted = MXL_TRUE;
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;


  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E) status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerIFOutParam
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 02/21/2012
--|
--| DESCRIPTION   : This function config IF out, include IF frequency selection,  
--|                 spectrum polarity AND output level. IF path is fixed to 1. 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerIFOutParams(
                UINT8 devId,
                UINT16 ifOutFreqInKHz,
                MXL_BOOL_E invertIFPolarity,
                UINT8 gainLevel)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  UINT16 ifManualFreq;
  UINT8 i = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  UINT16 presetIfFreq[MAX_PRESET_IF_FREQ][2] = 
  {
    { MXL68x_IF_FREQ_3650_KHZ, 0x00 }, 
    { MXL68x_IF_FREQ_4000_KHZ, 0x01 },   
    { MXL68x_IF_FREQ_4100_KHZ, 0x02 },   
    { MXL68x_IF_FREQ_4150_KHZ, 0x03 },
    { MXL68x_IF_FREQ_4500_KHZ, 0x04 },
    { MXL68x_IF_FREQ_4570_KHZ, 0x05 },
    { MXL68x_IF_FREQ_5000_KHZ, 0x06 },
    { MXL68x_IF_FREQ_5380_KHZ, 0x07 },
    { MXL68x_IF_FREQ_6000_KHZ, 0x08 },
    { MXL68x_IF_FREQ_6280_KHZ, 0x09 },
    { MXL68x_IF_FREQ_7200_KHZ, 0x0A },
    { MXL68x_IF_FREQ_82500_KHZ, 0x0B },
    { MXL68x_IF_FREQ_35250_KHZ, 0x0C },
    { MXL68x_IF_FREQ_36000_KHZ, 0x0D },
    { MXL68x_IF_FREQ_36150_KHZ, 0x0E },
    { MXL68x_IF_FREQ_36650_KHZ, 0x0F },
    { MXL68x_IF_FREQ_44000_KHZ, 0x10 }
  };
  
  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerIFOutParams, IF Out Freq = %d, Gain = %d, Invert = %d [%d]\n",
                  ifOutFreqInKHz, gainLevel, invertIFPolarity,devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Read back register for manual IF Out 
      status = MxL68x_Ctrl_ReadRegister(devId, IF_FREQ_SEL_REG, &regData);
      
      for (i = 0; i < MAX_PRESET_IF_FREQ; i++)
        if (ifOutFreqInKHz == presetIfFreq[i][0]) break;
      
      if (i == MAX_PRESET_IF_FREQ) 
      {
        // IF out manual setting : bit<5>
        regData |= 0x0020;

        // Manual IF freq set
        ifManualFreq = (UINT16)(ifOutFreqInKHz * 8192 / 216000);
        status |= MxL68x_Ctrl_WriteRegister(devId, IF_FCW_LOW_REG, (ifManualFreq & 0xFF)); 
        status |= MxL68x_Ctrl_WriteRegister(devId, IF_FCW_HIGH_REG, ((ifManualFreq >> 8) & 0x0F));

      }
      else
      {
        // bit<5> = 0, use IF frequency from IF frequency table  
        regData &= 0xFFC0;
        
        // update register setting for preset IF freq
        regData |= presetIfFreq[i][1];
      }

      if (status == MXL_SUCCESS)
      {
        // Program pre-defined IF freq
        status |= MxL68x_Ctrl_WriteRegister(devId, IF_FREQ_SEL_REG, regData);  

        // Set spectrum invert, gain level and IF path 
        // Spectrum invert indication is bit<7:6>
        if (MXL_ENABLE == invertIFPolarity) regData = 0x3 << 6;
        else regData = 0;

        // Gain level is bit<3:0> and set IF Path1 as output 
        regData |= (gainLevel & 0x0F) | (0x01 << 4);
        status |= MxL68x_Ctrl_WriteRegister(devId, IF_PATH_GAIN_REG, regData); 

        // update Device context
        devContextPtr->ifGainLevel = gainLevel;
        devContextPtr->ifOutFreqInKHz = ifOutFreqInKHz;        
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerAGC
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/06/2012
--|
--| DESCRIPTION   : This API configures AGC selection, type and setting point
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerAGC( 
                UINT8 devId,
                MXL68x_AGC_TYPE_E agcType,
                UINT8 setPoint,
                MXL_BOOL_E agcPolarityInversion)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerAGC, AGC type = %d, set point = %d, polarity = %d [%d]\n",
                  agcType, setPoint, agcPolarityInversion,devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // AGC selecton <3:2> and mode setting <0>
      status = MxL68x_Ctrl_ReadRegister(devId, AGC_CONFIG_REG, &regData); 
      regData &= 0xFFF2; // Clear bit<3:2> and bit<0>
      regData |= (UINT16)((agcType << 2) | 0x01);  // Only has AGC1 
      status |= MxL68x_Ctrl_WriteRegister(devId, AGC_CONFIG_REG, regData);

      // AGC set point <6:0>
      status |= MxL68x_Ctrl_ReadRegister(devId, AGC_SET_POINT_REG, &regData);
      regData &= 0xFF80; // Clear bit <6:0>
      regData |= setPoint;
      status |= MxL68x_Ctrl_WriteRegister(devId, AGC_SET_POINT_REG, regData);

      // AGC Polarity <4>
      status |= MxL68x_Ctrl_ReadRegister(devId, AGC_FLIP_REG, &regData);
      regData &= 0xFFEF; // Clear bit <4>
      regData |= (agcPolarityInversion << 4);
      status |= MxL68x_Ctrl_WriteRegister(devId, AGC_FLIP_REG, regData);
    }
    else
      status = MXL_NOT_INITIALIZED;

  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerApplicationMode
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/13/2012
--|
--| DESCRIPTION   : This fucntion is used to configure MxL68x tuner's 
--|                 application mode. 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerApplicationMode(
                UINT8 devId,
                MXL68x_SIGNAL_MODE_E signalMode)
{
  UINT16 dfeRegData = 0;
  UINT16 control1 = 0;
  UINT16 control2 = 0;
  UINT8 IfFreqSel = 1;  // Default is 4MHz IF for ISDB-T mode 
  UINT8 status = MXL_SUCCESS;
  REG_CTRL_INFO_T* tmpRegTable;
  HOST_COMMAND_T HostCommand;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerApplicationMode [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerApplicationMode: Tuner Signal mode = %d\n", signalMode);

      //UnclockGate CPU here
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG-1, &control1);
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &control2);
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG-1, control1);
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG, (control2 & 0xFFFD));

      // Change to XTAL clock before configuring signal type (App mode)
      // Switch to XTAL Clock
      // clkType = MXL68x_XTAL_CLOCK;
      status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_XTAL_CLOCK);    

      // Start Tune = 0
      status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, MXL_DISABLE);

      // If firmware is active, then use FW host command for application mode settings 
      //     Check if firmware has been downloaded and actived or not. 
      devContextPtr->firmwareStatus = Ctrl_CheckFirmwareStatus(devId);
      if ((MXL68x_FW_STATUS_LOADED == devContextPtr->firmwareStatus))
      {
        switch(devContextPtr->ifOutFreqInKHz)
        {
          case MXL68x_IF_FREQ_3650_KHZ:   IfFreqSel = 0x00; break; 
          case MXL68x_IF_FREQ_4000_KHZ:   IfFreqSel = 0x01; break;   
          case MXL68x_IF_FREQ_4100_KHZ:   IfFreqSel = 0x02; break;   
          case MXL68x_IF_FREQ_4150_KHZ:   IfFreqSel = 0x03; break;   
          case MXL68x_IF_FREQ_4500_KHZ:   IfFreqSel = 0x04; break;   
          case MXL68x_IF_FREQ_4570_KHZ:   IfFreqSel = 0x05; break;   
          case MXL68x_IF_FREQ_5000_KHZ:   IfFreqSel = 0x06; break;   
          case MXL68x_IF_FREQ_5380_KHZ:   IfFreqSel = 0x07; break;   
          case MXL68x_IF_FREQ_6000_KHZ:   IfFreqSel = 0x08; break;   
          case MXL68x_IF_FREQ_6280_KHZ:   IfFreqSel = 0x09; break;   
          case MXL68x_IF_FREQ_7200_KHZ:   IfFreqSel = 0x0A; break;   
          case MXL68x_IF_FREQ_82500_KHZ:  IfFreqSel = 0x0B; break;   
          case MXL68x_IF_FREQ_35250_KHZ:  IfFreqSel = 0x0C; break;   
          case MXL68x_IF_FREQ_36000_KHZ:  IfFreqSel = 0x0D; break;   
          case MXL68x_IF_FREQ_36150_KHZ:  IfFreqSel = 0x0E; break;   
          case MXL68x_IF_FREQ_36650_KHZ:  IfFreqSel = 0x0F; break;   
          case MXL68x_IF_FREQ_44000_KHZ:  IfFreqSel = 0x10; break;  
          default: status |= MXL_INVALID_PARAMETER; break;
        }

        dfeRegData = ((devContextPtr->xtalFreqInMHz & 0x01) << 13) + \
                      ((IfFreqSel & 0x1F) << 8) + \
                      ((devContextPtr->ifGainLevel & 0x0F) << 4) + \
                      (signalMode & 0x0F);
        status |= MxL68x_Ctrl_WriteRegister(devId, MXL68X_FW_MAIL_BOX_REG, dfeRegData);
        
        // Send command to FW 
        FORM_CMD_HEADER(HostCommand, HOST_CMD_TUNER_APP_DEP_SETTING, 0, 0);
        HostCommand.payloadCheckSum = 0;
        if (MXL_FAILURE == (MXL_STATUS_E)Ctrl_SendHostCommand(devId, &HostCommand))    
          status |= MXL_FAILURE;
 
        // Wait for 100 ms 
        MxLWare68x_OEM_SleepInUs(100000);  

        // check if firmware implementation is success or not 
        status |= MxL68x_Ctrl_ReadRegister(devId, MXL68X_FW_MAIL_BOX_REG, &dfeRegData);

        if (dfeRegData == 0xFFFF)
        {
          MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerApplicationMode: FW Programs App Mode Settings\n");
          // Clear the FW read Back register
          status |= MxL68x_Ctrl_WriteRegister(devId, MXL68X_FW_MAIL_BOX_REG, 0x00);
          
          switch (signalMode)
          {
            case MXL68x_DIG_DVB_C:
            case MXL68x_DIG_J83B:
              //Disable TT Spur Fix  by writing into mailbox reg
              status |= MxL68x_Ctrl_WriteRegister(devId, DFE_TT_SPUR_ENABLE, 0);   
       
              //Make sure the CPU is Clock Gated for cable modes
               status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG-1, &control1);
               status |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &control2);
               status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG-1, control1);
               status |= MxL68x_Ctrl_WriteRegister(devId, DMD_GPIO_FUNC_MODE_REG, (control2 | 0x0002));
             break; 

            default:
               //Do Nothing as we just Un-Clock Gated the CPU 
             break; 
          }           
        }
        else
        {
          MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerApplicationMode: FW Fails to Program App Mode Settings");
        }
        // Update device context Signal mode
        devContextPtr->signalType = signalMode;
        devContextPtr->firmwareStatus = Ctrl_CheckFirmwareStatus(devId);

      }
      else  // Implement tuner application setting in host side 
      {
        MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerApplicationMode: FW is Either Clock Gated or Not Loaded");

        switch(signalMode)
        {
          case MXL68x_ANA_NTSC_MODE:

            status = MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_AnalogNtsc);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
      
              if (devContextPtr->xtalFreqInMHz == MXL68x_XTAL_16MHz) 
              {
                tmpRegTable = MxL68x_Ntsc_16MHzRfLutSwpLIF;
                dfeRegData = 0x00;
              }
              else 
              {
                tmpRegTable = MxL68x_Ntsc_24MHzRfLutSwpLIF;
                dfeRegData = 0x01;
              }
                
              status |= MxL68x_Ctrl_WriteRegister(devId, 0xDD, dfeRegData);    
            }
            else
            {
              // High power
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
              tmpRegTable = MxL68x_Ntsc_RfLutSwpHIF;  
            }
              
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);    
            break;

          case MXL68x_ANA_PAL_BG:
            tmpRegTable = MxL68x_AnalogPal;
            status = MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
            }
            break;

          case MXL68x_ANA_PAL_I:
            status |= MxL68x_Ctrl_WriteRegister(devId, 0x0C, 0x0B);

            tmpRegTable = MxL68x_AnalogPal;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalI_RfLutSwpHIF);
            }
            break;

          case MXL68x_ANA_PAL_D:
            status |= MxL68x_Ctrl_WriteRegister(devId, 0x0C, 0x0C);

            tmpRegTable = MxL68x_AnalogPal;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalD_RfLutSwpHIF);
            }
            break;

          case MXL68x_ANA_SECAM_I:
            
            status |= MxL68x_Ctrl_WriteRegister(devId, 0x0C, 0x0B);
            tmpRegTable = MxL68x_AnalogSecam;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
            }
            break;

          case MXL68x_ANA_SECAM_L:
            status |= MxL68x_Ctrl_WriteRegister(devId, 0x0C, 0x0C);

            tmpRegTable = MxL68x_AnalogSecam;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
#ifdef CUSTOMER_SPECIFIC_SETTING_1
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
#else 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0x7E);
#endif 
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
            }
            break;

          case MXL68x_DIG_DVB_C:
          case MXL68x_DIG_J83B: 
            tmpRegTable = MxL68x_DigitalDvbc;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x10);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
            }
            
            if (devContextPtr->xtalFreqInMHz == MXL68x_XTAL_16MHz) dfeRegData = 0x0D;
            else dfeRegData = 0x0E;
              
            status |= MxL68x_Ctrl_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

            break;

          case MXL68x_DIG_ATSC:
            tmpRegTable = MxL68x_DigitalIsdbtAtsc;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xF9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x18);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_PWR, 0xF1);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_PWR, 0xB1);
            }
 
            if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz) dfeRegData = 0x0D;
            else if (MXL68x_XTAL_24MHz == devContextPtr->xtalFreqInMHz) dfeRegData = 0x0E;
              
            status |= MxL68x_Ctrl_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

            dfeRegData = 0;
            // IF gain based settings
            switch(devContextPtr->ifGainLevel)
            {
              case 0x09: dfeRegData = 0x44; break;
              case 0x08: dfeRegData = 0x43; break;
              case 0x07: dfeRegData = 0x42; break;
              case 0x06: dfeRegData = 0x41; break;
              case 0x05: dfeRegData = 0x40; break;
            }
            status |= MxL68x_Ctrl_WriteRegister(devId, DFE_DACIF_GAIN, dfeRegData);

            break;

          case MXL68x_DIG_DVB_T:
            tmpRegTable = MxL68x_DigitalDvbt;
            status |= MxL68x_Ctrl_ProgramRegisters(devId, tmpRegTable);

            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
            {
              // Low power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xFE);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x18);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_PWR, 0xF1);
            }
            else
            {
              // High power if settings
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_0, 0xD9);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_CFG_1, 0x16);
              status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_IF_PWR, 0xB1);
            }
 
            if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz) dfeRegData = 0x0D;
            else if (MXL68x_XTAL_24MHz == devContextPtr->xtalFreqInMHz) dfeRegData = 0x0E;
              
            status |= MxL68x_Ctrl_WriteRegister(devId, DFE_CSF_SS_SEL, dfeRegData);

            dfeRegData = 0;

            // IF gain based settings
            switch(devContextPtr->ifGainLevel)
            {
              case 0x09: dfeRegData = 0x44; break;
              case 0x08: dfeRegData = 0x43; break;
              case 0x07: dfeRegData = 0x42; break;
              case 0x06: dfeRegData = 0x41; break;
              case 0x05: dfeRegData = 0x40; break;
            }

            status |= MxL68x_Ctrl_WriteRegister(devId, DFE_DACIF_GAIN, dfeRegData);
            
            break;

          default:
            status = MXL_INVALID_PARAMETER;
            break;
        }
        
        if (MXL_SUCCESS == status)
        {
          // XTAL calibration
          status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x00);   
          status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x01);   
          status |= MxL68x_Ctrl_WriteRegister(devId, XTAL_CALI_SET_REG, 0x00); 
          
          // 50 ms sleep after XTAL calibration
          MxLWare68x_OEM_SleepInUs(50000); 

          // Update device context Signal mode
          devContextPtr->signalType = signalMode;
        }
      } // end of else (host implement tuner application mode setting) 
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerChanTune
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 3/18/2013
--|
--| DESCRIPTION   : This API configures RF channel frequency and bandwidth. 
--|                 Radio Frequency unit is Hz, and Bandwidth is in MHz unit
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerChanTune(
                UINT8 devId,
                MXL68x_TUNE_TYPE_E tuneType,
                MXL68x_BW_E bandWidth,
                UINT32 freqInHz,
                MXL68x_AUTO_SPEC_INV autoSpecInv,
                MXL_BOOL_E useChanTuneInt) 
{
  UINT32 frequency;
  UINT32 freq = 0;
  UINT16 regData = 0, fwXtalBwSettings = 0, tuneDone = 0, control = 0, ampRB = 0, ampLinRB = 0;
  SINT16 tempBias = 0;
  UINT8 chanFound = 0, index_i = 0;
  UINT16 tuneTimeOut = 0;
  UINT8 status = MXL_SUCCESS;  
  HOST_COMMAND_T chanTune;
  FW_RESPONSE_T getChanDataRsp;
  MXL68x_RESET_TYPE_E devReset; 
  MXL68x_CHAN_DEP_FREQ_TABLE_T *freqLutPtr = NULL;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);
  MXL68x_NTSC_CARRIERS_E ntscCarrier = MXL68x_NTSC_CARRIER_NA;
  SINT32 rxPwr = -10000;

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerChanTune [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      MxL_DLL_DEBUG0("signal type = %d, Freq = %d, BW = %d, Xtal = %d, NTSC Carrier =%d  useChanTuneInt = %d\n", 
          devContextPtr->signalType, freqInHz, bandWidth, devContextPtr->xtalFreqInMHz, ntscCarrier, useChanTuneInt); 
      // Disable Demod
      if (devContextPtr->devOpMode == MXL68x_TUNER_DEMOD)
        status |= Ctrl_CfgDemodEnable(devId, MXL_DISABLE);        

      // Switch to XTAL Clock
      status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_XTAL_CLOCK);        

      // START_TUNE = 0
      status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, MXL_DISABLE);

      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_XTAL_REG, 0x0000); // set all bits to 0 (including AutoSpecInv)
      status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_FREQ_REG, 0x0000);

      if (freqInHz < 700000000) 
      {
        // Program cable mode settings - IF related
        status |= MxL68x_Ctrl_WriteRegister(devId, 0x7C, 0x1F);
        if ((devContextPtr->signalType == MXL68x_DIG_DVB_C) || 
            (devContextPtr->signalType == MXL68x_DIG_J83B)) 
        {
          regData = 0xC1;
        }
        else
        {
          regData = 0x81;    
        }
      }
      else 
      {
        // Program cable mode settings - IF related
        status |= MxL68x_Ctrl_WriteRegister(devId, 0x7C, 0x9F);

        if ((devContextPtr->signalType == MXL68x_DIG_DVB_C) || 
            (devContextPtr->signalType == MXL68x_DIG_J83B)) 
        {
          regData = 0xD1;
        }
        else
        {
          regData = 0x91;
        }
      }
      // Program IF register
      status |= MxL68x_Ctrl_WriteRegister(devId, 0x131, regData);

      // frequency dependent XTAL Bias Settings for all analog modes
      if(devContextPtr->signalType <= MXL68x_ANA_SECAM_L)
      {
        status |= MxL68x_Ctrl_ReadRegister(devId, ANA_DIG_REFSX_XTAL_AMP_RB_REG, &ampRB);
        status |= MxL68x_Ctrl_ReadRegister(devId, ANA_DIG_REFSX_XTAL_AMP_LIN_RB_REG, &ampLinRB);
        ampLinRB = (ampLinRB & 0x30) >> 4; //using only bits <5:4>



        if (freqInHz < 300000000)
        {
          tempBias = (ampRB + ampLinRB - 5);
          regData = (tempBias < 0) ? 0 : tempBias;
        }
        else
        {
          regData = (ampRB + ampLinRB);
        }
        status |= MxL68x_Ctrl_WriteRegister(devId, DIG_ANA_XTAL_XTAL_EXT_BIAS, regData);

        MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerChanTune ampRB %d ampLinRB %d regData %d\n",ampRB,ampLinRB,regData);

        regData = 0;
      }

      switch(devContextPtr->signalType)
      {
        // Signal type is NTSC mode
        case MXL68x_ANA_NTSC_MODE:

          // If 16MHz XTAL is used then need to program registers based on frequency value
          if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz)
          {
            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_16MHzRfLutSwpLIF);
            else 
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_RfLutSwpHIF);

            if (MXL68x_NTSC_CARRIER_NA == ntscCarrier)
            {
              // Select spur shifting LUT to be used based NTSC, HRC or IRC frequency
              if ((freqInHz % 1000000) == 0) 
              {
                // SPUR LUT based on IF freq
                if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
                  freqLutPtr = MXL68x_NTSC_LUT_XTAL_16MHZ_LIF;
                else 
                  freqLutPtr = MXL68x_NTSC_LUT_XTAL_16MHZ_HIF;
              }
              else if (((freqInHz - 12500) % 1000000) == 0) freqLutPtr = MXL68x_NTSC_LUT_IRC_16MHZ;
              else if (((freqInHz - 143000) % 1000000) == 0) freqLutPtr = MXL68x_PAL_MN_LUT;  
              else 
              {
                freqLutPtr = MXL68x_NTSC_LUT_HRC_16MHZ;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_HRCRfLutSwpLIF);
              }		  
            }           
          } // end of if (16 MHz XTAL
          else if (MXL68x_XTAL_24MHz == devContextPtr->xtalFreqInMHz)
          {
            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ)
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_24MHzRfLutSwpLIF);
            else 
              status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_RfLutSwpHIF);

            if (MXL68x_NTSC_CARRIER_NA == ntscCarrier)
            {
              // Select spur shifting LUT to be used based NTSC, HRC or IRC frequency
              if ((freqInHz % 1000000) == 0) freqLutPtr = MXL68x_NTSC_LUT_XTAL_24MHZ;
              else if (((freqInHz - 12500) % 1000000) == 0) freqLutPtr = MXL68x_NTSC_LUT_IRC_24MHZ;
              else if (((freqInHz - 143000) % 1000000) == 0) freqLutPtr = MXL68x_PAL_MN_LUT; 
              else 
              {
                freqLutPtr = MXL68x_NTSC_LUT_HRC_24MHZ;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Ntsc_HRCRfLutSwpLIF);
              }         
            }
          } // end of if (24 MHz XTAL 

          // program spur lut
          if (freqLutPtr)
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

          // Signal type is PAL D
        case MXL68x_ANA_PAL_D:

          // If 16MHz XTAL is used then need to program registers based on frequency value
          if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz)
          {
            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
              freqLutPtr = MXL68x_PAL_D_LUT_XTAL_16MHZ_LIF;
            else freqLutPtr = MXL68x_PAL_D_LUT_XTAL_16MHZ_HIF;

          } // end of if (16 MHz XTAL
          else if (MXL68x_XTAL_24MHz == devContextPtr->xtalFreqInMHz)
          {
            freqLutPtr = MXL68x_PAL_D_LUT_XTAL_24MHZ;
          } // end of if (24 MHz XTAL 

          // program spur lut
          if (freqLutPtr) 
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

        case MXL68x_ANA_PAL_BG:

          switch(bandWidth)
          {
            case MXL68x_BW_7MHz:
              regData = 0x09;
              break;

            case MXL68x_BW_8MHz:
              regData = 0x0A;
              break;

            default: 
              break;		  
          }
          // program dfe ana mode for 7 & 8 MHz BW
          if ((bandWidth == MXL68x_BW_7MHz) || (bandWidth == MXL68x_BW_8MHz))
            status |= MxL68x_Ctrl_WriteRegister(devId, 0x0C, regData);

          // If 16MHz XTAL is used then need to program registers based on frequency value
          if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz)
          {
            // PAL - BG 7 MHz Frequency range
            if (MXL68x_BW_7MHz == bandWidth) 
            {
              if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
              {
                freqLutPtr = MXL68x_PAL_BG_7MHZ_LUT_XTAL_16MHZ_LIF;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
              }
              else
              {
                freqLutPtr = MXL68x_PAL_BG_7MHZ_LUT_XTAL_16MHZ_HIF;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalBG_7MHzBW_RfLutSwpHIF);
              }
            }
            else 
            {
              if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
              {
                freqLutPtr = MXL68x_PAL_BG_8MHZ_LUT_XTAL_16MHZ_LIF;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
              }
              else
              {
                freqLutPtr = MXL68x_PAL_BG_8MHZ_LUT_XTAL_16MHZ_HIF;
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalBG_8MHzBW_RfLutSwpHIF);
              }
            }        
          } // end of if (XTAL_16MHz     
          else if (MXL68x_XTAL_24MHz == devContextPtr->xtalFreqInMHz)
          { 
            // If 16MHz XTAL is used then need to program registers based on frequency value
            // PAL - BG 7 MHz Frequency range
            if (MXL68x_BW_7MHz == bandWidth) 
            {
              freqLutPtr = MXL68x_PAL_BG_7MHZ_LUT_XTAL_24MHZ;

              if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
              else
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalBG_7MHzBW_RfLutSwpHIF);		
            }		 
            else 
            {
              freqLutPtr = MXL68x_PAL_BG_8MHZ_LUT_XTAL_24MHZ;   

              if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_Pal_RfLutSwpLIF);
              else
                status |= MxL68x_Ctrl_ProgramRegisters(devId, MxL68x_PalBG_8MHzBW_RfLutSwpHIF);		
            }
          } // end of if (XTAL_24MHz

          // program spur lut
          if (freqLutPtr)
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

        case MXL68x_ANA_PAL_I:    

          // If 16MHz XTAL is used then need to program registers based on frequency value
          if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz) 
          {
            if (devContextPtr->ifOutFreqInKHz < MXL68x_IF_FREQ_35250_KHZ) 
              freqLutPtr = MXL68x_PAL_I_LUT_XTAL_16MHZ_LIF;
            else 
              freqLutPtr = MXL68x_PAL_I_LUT_XTAL_16MHZ_HIF;	   
          }
          else 
            freqLutPtr = MXL68x_PAL_I_LUT_XTAL_24MHZ;	

          // program spur lut
          if (freqLutPtr)	  
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

        case MXL68x_ANA_SECAM_L:      
          // If 16MHz XTAL is used then need to program registers based on frequency value
          if (MXL68x_XTAL_16MHz == devContextPtr->xtalFreqInMHz) freqLutPtr = MXL68x_SECAM_L_LUT_XTAL_16MHZ;
          else freqLutPtr = MXL68x_SECAM_L_LUT_XTAL_24MHZ;

          // program spur lut
          if (freqLutPtr)
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

        case MXL68x_DIG_DVB_C:
        case MXL68x_DIG_J83B:

          if ((bandWidth == MXL68x_BW_6MHz) || (bandWidth == MXL68x_BW_8MHz))
          {
            if (bandWidth == MXL68x_BW_6MHz) freqLutPtr = MXL68x_DIG_CABLE_LUT_BW_6MHZ;
            else freqLutPtr = MXL68x_DIG_CABLE_LUT_BW_8MHZ;

            // program dig mode related spur lut
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);
          }
          break;

        case MXL68x_DIG_ATSC:
        case MXL68x_DIG_ISDBT:
        case MXL68x_DIG_DVB_T:
          if (bandWidth == MXL68x_BW_6MHz) freqLutPtr = MXL68x_DIG_TERR_LUT_BW_6MHZ;
          else if (bandWidth == MXL68x_BW_7MHz) freqLutPtr = MXL68x_DIG_TERR_LUT_BW_7MHZ;
          else freqLutPtr = MXL68x_DIG_TERR_LUT_BW_8MHZ;

          // program dig mode related spur lut
          if (freqLutPtr) 
            status |= Ctrl_SetRfFreqLutReg(devId, freqInHz, freqLutPtr);

          break;

        default: 
          break;
      }

      /* Calculate RF Channel = DIV(64*RF(Hz), 1E6) */
      frequency = freqInHz / 1000;
      frequency = frequency * 64;
      freq = (UINT32)((frequency + 500) / 1000); 

      // According to firmware status choose the channel tune is done by firmware or host SW 
      // Firmware is loaded and active, process channel tune by firmware 
      if ((MXL68x_FW_STATUS_LOADED == devContextPtr->firmwareStatus) && 
          (MXL68x_DIG_ISDBT == devContextPtr->signalType))  
      {         
        //Loop Through Configs need to be done for every channel change in SoC Mode
        if (MXL_ENABLE == devContextPtr->rfLoopThrough)
        {
          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_AGC_AUTO_REG, &regData);
          control = (regData & 0x80) | 0x0E;  // Bit<6> = 0, Bit<5:0> = 14 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_AGC_AUTO_REG, control);

          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_SEQ_TUNE_RF1_BO_REG, &regData);
          control = (regData & 0xC0) | 0x0E;  // Bit<5:0> = 14 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_SEQ_TUNE_RF1_BO_REG, control);

          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_SEQ_CDC_RF1_BO_REG, &regData);
          control = (regData & 0xC0) | 0x0E;  // Bit<5:0> = 14 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_SEQ_CDC_RF1_BO_REG, control);
        }
        else 
        {
          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_AGC_AUTO_REG, &regData);
          control = (regData & 0x80) | 0x40;  // Bit<6> = 1, Bit<5:0> = 0 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_AGC_AUTO_REG, control);

          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_SEQ_TUNE_RF1_BO_REG, &regData);
          control = (regData & 0xC0) | 0x37;  // Bit<5:0> = 55
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_SEQ_TUNE_RF1_BO_REG, control);

          status |= MxL68x_Ctrl_ReadRegister(devId, DFE_SEQ_CDC_RF1_BO_REG, &regData);
          control = (regData & 0xC0) | 0x37;  // Bit<5:0> = 55 
          status |= MxL68x_Ctrl_WriteRegister(devId, DFE_SEQ_CDC_RF1_BO_REG, control);    
        }

        MxL_DLL_DEBUG0("Firmware is active, program channel tune by FW \n");
        //Program frequency value
        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_FREQ_REG, (UINT16)freq);

        switch(bandWidth)
        {
          case MXL68x_BW_6MHz: fwXtalBwSettings = fwXtalBwSettings | (0x00 << 2); break;  
          case MXL68x_BW_7MHz: fwXtalBwSettings = fwXtalBwSettings | (0x01 << 2); break; 
          case MXL68x_BW_8MHz: fwXtalBwSettings = fwXtalBwSettings | (0x02 << 2); break; 
          default: break; 
        }

        status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CHAN_SCAN_TUNE_XTAL_REG, &regData);

        fwXtalBwSettings = fwXtalBwSettings | ((devContextPtr->xtalFreqInMHz & 0x01) << 1);  // Set Xtal type , bit<1>
        fwXtalBwSettings = fwXtalBwSettings | (tuneType & 0x01);  // Set Tune type , bit<0>

        if(MXL68x_AUTO_SPEC_INV_ENABLE == autoSpecInv)
        {
          fwXtalBwSettings = fwXtalBwSettings | 0x10;  // Enable Auto Spec Inv bit <4> = 1
        }
        else
        {
          fwXtalBwSettings = fwXtalBwSettings & 0xEF;  // Disable Auto Spec Inv bit <4> = 0
        }

        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_XTAL_REG, fwXtalBwSettings);

        //Send FW Host Command for channel tune
        FORM_CMD_HEADER(chanTune, HOST_CMD_TUNER_CHAN_TUNE_CFG, 0, 0);    
        status |= Ctrl_SendHostCommand(devId, &chanTune);

        if(useChanTuneInt == MXL_FALSE)
        {

          //Wait for 50ms
          MxLWare68x_OEM_SleepInUs(50000);

          tuneTimeOut = 0;
          //Poll till RxPower is >= -92dBm or 100ms
          //If RxPower is less than -92dBm after 100ms of polling,
          //Assume no channel present
          while((tuneTimeOut<=10) && (rxPwr<-9200) )
          {
            status |= MxLWare68x_API_ReqTunerRxPower( devId, &rxPwr);

            MxLWare68x_OEM_SleepInUs(10000);
            tuneTimeOut++;
          }

          //if RxPwr is >= -92dBm, there may be a channel, so poll for another 3seconds
          if(rxPwr >=-9200)
          {
            tuneTimeOut = 0;

            //time out here is 3 sec for tune completion
            while( (tuneDone != 1) && (tuneTimeOut <= 300) )
            {
              // Add 10 ms waiting time - poll every 10ms
              MxLWare68x_OEM_SleepInUs(10000);

              //reacd back for checking tune done
              status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CHAN_SCAN_TUNE_XTAL_REG, &tuneDone);
              regData = tuneDone;  // Save reading data of 0x3028 

              //Channel Found Indication is Bit 7
              chanFound  = (tuneDone & 0x80) >> 7;
              //Tune Done Indication is Bit 6
              tuneDone = (tuneDone & 0x40) >> 6;

              tuneTimeOut++;
            }

            MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerChanTune: TuneDone = %d, ChanFound = %d, Time = %dms \n",tuneDone,chanFound,(tuneTimeOut*10));

            if(chanFound == 1)
            {
              // Means firmware has been loaded success before, get version information by FW communication 
              status |= Ctrl_GetDeviceResp(devId, HOST_CMD_TUNER_TUNE_RESULT_REQ, &getChanDataRsp);

              if ((getChanDataRsp.errorCode == RESPONSE_ID_SUCCESS) && (status == MXL_SUCCESS)  
                  && (getChanDataRsp.payloadLen == FW_CHAN_TUNE_DATA_LENGTH))
              {
                for(index_i=0; index_i<FW_CHAN_TUNE_DATA_LENGTH; index_i++)
                {
                  HI_INFO_TUNER("ChData[%d] = 0x%02X\n",index_i,getChanDataRsp.data[index_i]);
                }
              }
            }
            //Removing this as part of the cable cut issue. To facilitate the FW to Auto re-tune when this problem is detected, we do not reset the values here
            //it is reset at the start of this API
            //status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_XTAL_REG, 0x0000); // set all bits to 0 (including AutoSpecInv)
            //status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CHAN_SCAN_TUNE_FREQ_REG, 0x0000);
          }
        }
      }
      else   // Analog Modes, perform channel tune by host
      {
        MxL_DLL_DEBUG0("Firmware is inactive, program channel tune by host SW \n");

        // Tune mode scan or view <0>
        status |= MxL68x_Ctrl_ReadRegister(devId, TUNE_MODE_REG, &regData);
        regData &= 0xFE;

        if (MXL68x_SCAN_MODE == tuneType) regData |= 0x01;

        status |= MxL68x_Ctrl_WriteRegister(devId, TUNE_MODE_REG, regData);

        // Bandwidth <1:0>
        status |= MxL68x_Ctrl_ReadRegister(devId, CHAN_TUNE_BW_REG, &regData);
        regData = (UINT16)((regData & 0xFFFC) | bandWidth);

        status |= MxL68x_Ctrl_WriteRegister(devId, CHAN_TUNE_BW_REG, regData);

        // Set RF  
        status |= MxL68x_Ctrl_WriteRegister(devId, CHAN_TUNE_LOW_REG, (UINT8)(freq & 0xFF));
        status |= MxL68x_Ctrl_WriteRegister(devId, CHAN_TUNE_HI_REG, (UINT8)((freq >> 8 ) & 0xFF));

        if (devContextPtr->signalType <= MXL68x_ANA_SECAM_L)
        {
          status |= MxLWare68x_API_CfgTunerEnableFineTune(devId, MXL_DISABLE);
          status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_CTRL_REG_1, 0);
          status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_CTRL_REG_1, 0x20); 
        }

        // Start Tune  = 1
        MxL_DLL_DEBUG0(" MxLWare68x_API_CfgTunerChanTune, Start tune \n");
        status |= MxL68x_Ctrl_WriteRegister(devId, START_TUNE_REG, 0x0001);

        // Switch to PLL Clock
        // clkType = MXL68x_PLL_CLOCK;
        status |= Ctrl_CfgDevSwitchClock(devId, MXL68x_PLL_CLOCK);

        if (devContextPtr->devOpMode == MXL68x_TUNER_DEMOD)
        {
          // Enable Demod
          status |= Ctrl_CfgDemodEnable(devId, MXL_ENABLE);

          // Demod data path reset
          devReset = MXL68x_DEMOD_DATAPATH_RESET;
          status |= Ctrl_CfgDevReset(devId, devReset);
        } 
        if ((devContextPtr->signalType != MXL68x_DIG_DVB_C) && (devContextPtr->signalType != MXL68x_DIG_J83B))
        {            
          MxL_DLL_DEBUG0(" TT Spur Mitigation Host Command to FW \n");
          //Send FW Host Command to enable TeleText Spur Mitigation
          FORM_CMD_HEADER(chanTune, HOST_CMD_TUNER_TT_SPUR_MIT_EN_CFG, 0, 0);    
          status |= Ctrl_SendHostCommand(devId, &chanTune);
        }
      }
    } 
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  HI_INFO_TUNER("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerEnableFineTune 
--| 
--| AUTHOR        : Dong Liu 
--|
--| DATE CREATED  : 12/02/2011  
--|
--| DESCRIPTION   : This API enables/disables fine tune. Fine tune should be
--|                 enabled before fine tune starts and disabled after 
--|                 fine tune operation finishes. This API should be used when
--|                 MxL68x tuner is configured in Analog mode.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerEnableFineTune(
                UINT8 devId,
                MXL_BOOL_E enableFineTune)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 fineTuneReg0 = 0;
  UINT16 fineTuneReg1 = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerEnableFineTune [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status = MxL68x_Ctrl_ReadRegister(devId, FINE_TUNE_CTRL_REG_0, &fineTuneReg0);
      status |= MxL68x_Ctrl_ReadRegister(devId, FINE_TUNE_CTRL_REG_1, &fineTuneReg1);

      // Bit <2> set to 0 (disable) and 1(enable)
      fineTuneReg0 &= 0xFB;

      // Bit <5> set to 0 (disable) and 1(enable)
      fineTuneReg1 &= 0xDF;

      if (MXL_ENABLE == enableFineTune)
      {
        fineTuneReg0 |= 0x04;
        fineTuneReg1 |= 0x20;
      }
      else
      {
        // Bit <1:0> set to 0 (disable) and 1(enable)
        fineTuneReg0 &= 0xFC;
      }
      
      status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_CTRL_REG_0, fineTuneReg0);    
      status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_CTRL_REG_1, fineTuneReg1);    
    }
    else
      status = MXL_NOT_INITIALIZED;

  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgTunerFineTune 
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/02/2011
--|
--| DESCRIPTION   : This API process fine tune function when tuner works in  
--|                 analog mode. 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgTunerFineTune(
                UINT8 devId,
                MXL68x_FINE_TUNE_STEP_E fineTuneStep)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgTunerFineTune [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      status = MxL68x_Ctrl_ReadRegister(devId, FINE_TUNE_SET_REG, &regData);

      // Fine tune <1:0>
      regData &= 0xFFFC;
      status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_SET_REG, regData);

      if (MXL68x_FINE_TUNE_STEP_UP == fineTuneStep) regData |= TUNE_INCREASE;
      else regData |= TUNE_DECREASE;

      // Program fine tune step
      status |= MxL68x_Ctrl_WriteRegister(devId, FINE_TUNE_SET_REG, regData); 
    
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqTunerAGCStatus
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/02/2011
--|
--| DESCRIPTION   : This API will retrieves MxL68x Tuner AGC lock status.
--|                 AGC Gain = rawAgcGain / 2^6
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqTunerAGCStatus(  
                UINT8 devId,
                MXL_BOOL_E* agcLockStatusPtr,
                UINT16* rawAgcGainPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  UINT16 tempData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqTunerAGCStatus [%d]\n",devId);

  if (devContextPtr)
  {
    if ((devContextPtr->driverStatus) && (rawAgcGainPtr != NULL))
    {
      *rawAgcGainPtr = 0;

      // Read low 8 bit 
      status = MxL68x_Ctrl_ReadRegister(devId, RF_TOTAL_GAIN_RB_REG, &regData);
      tempData = regData & 0xFF;

      // Read high 4 bit 
      status |= MxL68x_Ctrl_ReadRegister(devId, RF_TOTAL_GAIN_RB_REG + 1, &regData);
      regData = ((regData & 0x0F) << 8) + tempData;

      if (MXL_SUCCESS == status) *rawAgcGainPtr = regData; 
      
      // Readback AGC lock status
      status |= MxL68x_Ctrl_ReadRegister(devId, AGC_SAGCLOCK_STATUS_REG, &regData);  

      if ((regData & 0x08) == 0x08) *agcLockStatusPtr = MXL_ON;
      else *agcLockStatusPtr = MXL_OFF;
    
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqTunerLockStatus
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 2/21/2011
--|
--| DESCRIPTION   : This function returns Tuner Lock status of after tuning.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqTunerLockStatus(
                UINT8 devId,
                MXL_BOOL_E* rfSynthLockPtr,
                MXL_BOOL_E* refSynthLockPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  HI_INFO_TUNER("MxLWare68x_API_ReqTunerLockStatus [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Readback tuner lock status
      status = MxL68x_Ctrl_ReadRegister(devId, RF_REF_STATUS_REG, &regData);  

      // Bit<0>
      if ((regData & 0x01) == 0x01) *refSynthLockPtr = MXL_YES;
      else *refSynthLockPtr = MXL_NO;

      // Bit<1>
      if ((regData & 0x02) == 0x02) *rfSynthLockPtr = MXL_YES;
      else *rfSynthLockPtr = MXL_NO;

      HI_INFO_TUNER(" RfSynthStatus = %d, RefSynthStatus = %d", (UINT8)*rfSynthLockPtr,(UINT8)*refSynthLockPtr); 

    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  HI_INFO_TUNER("RETURN: %d [%d]\n",status, devId);
  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_ReqTunerRxPower 
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 02/21/2012
--|
--| DESCRIPTION   : This function returns RF input power in 0.01dBm.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_ReqTunerRxPower(
                UINT8 devId,
                SINT32* rxPwrPtr)
{
  UINT32 dfeRegData = 0;
  UINT16 regData = 0, tmpData = 0;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_ReqTunerRxPower [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {   


      if ((MXL68x_FW_STATUS_LOADED == devContextPtr->firmwareStatus))
      {
        Ctrl_DfeRegRead(devId, RFPIN_RB_LOW_REG, 2, &dfeRegData);
        tmpData = (UINT16)(dfeRegData & 0x3FF);
      }
      else
      {
        // RF input power low <7:0>
        status = MxL68x_Ctrl_ReadRegister(devId, RFPIN_RB_LOW_REG, &regData);
        tmpData = regData;

        // RF input power high <1:0>
        status |= MxL68x_Ctrl_ReadRegister(devId, RFPIN_RB_HIGH_REG, &regData);
        tmpData |= (regData & 0x03) << 8;        
      }

      // Fractional last 2 bits
      *rxPwrPtr= (tmpData & 0x01FF) * 25; // Change the unit to 0.01 dBm 

      if (tmpData & 0x02) *rxPwrPtr += 50;
      if (tmpData & 0x01) *rxPwrPtr += 25;
      if (tmpData & 0x0200) *rxPwrPtr -= 128*100;
        
      MxL_DLL_DEBUG0(" Rx power = %d *0.01 dBm \n", *rxPwrPtr);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  return (MXL_STATUS_E)status;
}


/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_API_CfgDevCustSpecificSettings 
--|
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 09/18/2012
--|
--| DESCRIPTION   : Use this function to perform customer specific settings
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_API_CfgDevCustSpecificSettings(
                UINT8 devId,
                UINT16 mask)
{
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("MxLWare68x_API_CfgDevCustSpecificSettings [%d]\n",devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {   
      status |= MxL68x_Ctrl_WriteRegister(devId, CUST_SET_REG, mask);
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  MxL_DLL_DEBUG0("RETURN: %d [%d]\n",status, devId);
  return (MXL_STATUS_E)status;
}

