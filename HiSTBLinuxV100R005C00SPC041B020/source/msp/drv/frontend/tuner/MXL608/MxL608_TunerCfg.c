/*******************************************************************************
 *
 * FILE NAME          : MxL608_TunerCfg.cpp
 * 
 * AUTHOR             : Dong Liu                 
 *
 * DATE CREATED       : 08/29/2012
 *
 * DESCRIPTION        : This file contains demod and RF control parameters.
 *                      add a new function named Ctrl_WriteRegField
 *                             
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#include "MxL608_TunerCfg.h"
#include "MxL608_TunerApi.h"
#include "MxL_Debug.h"

MXL608_REG_CTRL_INFO_T MxL608_OverwriteDefaults[] = 
{
  {0x14, 0xFF, 0x13},
  {0x6D, 0xFF, 0x8A},
  {0x6D, 0xFF, 0x0A},
  {0xDF, 0xFF, 0x19},
  {0x45, 0xFF, 0x1B},
  {0xA9, 0xFF, 0x59},
  {0xAA, 0xFF, 0x6A},
  {0xBE, 0xFF, 0x4C},
  {0xCF, 0xFF, 0x25},
  {0xD0, 0xFF, 0x34},
  {0x77, 0xFF, 0xE7},
  {0x78, 0xFF, 0xE3},
  {0x6F, 0xFF, 0x51},
  {0x7B, 0xFF, 0x84},
  {0x7C, 0xFF, 0x9F},
  {0x56, 0xFF, 0x41},
  {0xCD, 0xFF, 0x64},
  {0xC3, 0xFF, 0x2C},
  {0x9D, 0xFF, 0x61},
  {0xF7, 0xFF, 0x52},
  {0x58, 0xFF, 0x81},
  {0x00, 0xFF, 0x01},
  {0x62, 0xFF, 0x02},
  {0x00, 0xFF, 0x00},
  {0,    0,    0}
};

// Digital DVB-C application mode setting
MXL608_REG_CTRL_INFO_T MxL608_DigitalDvbc[] = 
{
  {0x0C, 0xFF, 0x00},
  {0x13, 0xFF, 0x04},
  {0x53, 0xFF, 0x7E},
  {0x57, 0xFF, 0x91},
  {0x5C, 0xFF, 0xB1},
  {0x62, 0xFF, 0xF2},
  {0x6E, 0xFF, 0x03},
  {0x6F, 0xFF, 0xD1},
  {0x87, 0xFF, 0x77},
  {0x88, 0xFF, 0x55},
  {0x93, 0xFF, 0x33},
  {0x97, 0xFF, 0x03},
  {0xBA, 0xFF, 0x40},
  {0x98, 0xFF, 0xAF},
  {0x9B, 0xFF, 0x20},
  {0x9C, 0xFF, 0x1E},
  {0xA0, 0xFF, 0x18},
  {0xA5, 0xFF, 0x09},
  {0xC2, 0xFF, 0xA9},
  {0xC5, 0xFF, 0x7C},
  {0xCD, 0xFF, 0x64},
  {0xCE, 0xFF, 0x7C},
  {0xD5, 0xFF, 0x04},
  {0xD9, 0xFF, 0x00},
  {0xEA, 0xFF, 0x00},
  {0xDC, 0xFF, 0x1C},
  {0,    0,    0}
};

// Digital ISDBT & ATSC application mode setting
MXL608_REG_CTRL_INFO_T MxL608_DigitalIsdbtAtsc[] = 
{
  {0x0C, 0xFF, 0x00},
  {0x13, 0xFF, 0x04},
  {0x53, 0xFF, 0xFE},
  {0x57, 0xFF, 0x91},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x77},
  {0x88, 0xFF, 0x55},
  {0x93, 0xFF, 0x22},
  {0x97, 0xFF, 0x02},
  {0xBA, 0xFF, 0x30},
  {0x98, 0xFF, 0xAF},
  {0x9B, 0xFF, 0x20},
  {0x9C, 0xFF, 0x1E},
  {0xA0, 0xFF, 0x18},
  {0xA5, 0xFF, 0x09},
  {0xC2, 0xFF, 0xA9},
  {0xC5, 0xFF, 0x7C},
  {0xCD, 0xFF, 0xEB},
  {0xCE, 0xFF, 0x7F},
  {0xD5, 0xFF, 0x03},
  {0xD9, 0xFF, 0x04},
  {0,    0,    0}
};

// Digital DVB-T 6MHz application mode setting
MXL608_REG_CTRL_INFO_T MxL608_DigitalDvbt[] = 
{
  {0x0C, 0xFF, 0x00},
  {0x13, 0xFF, 0x04},
  {0x53, 0xFF, 0xFE},
  {0x57, 0xFF, 0x91},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x77},
  {0x88, 0xFF, 0x55},
  {0x93, 0xFF, 0x22},
  {0x97, 0xFF, 0x02},
  {0xBA, 0xFF, 0x30},
  {0x98, 0xFF, 0xAF},
  {0x9B, 0xFF, 0x20},
  {0x9C, 0xFF, 0x1E},
  {0xA0, 0xFF, 0x18},
  {0xA5, 0xFF, 0x09},
  {0xC2, 0xFF, 0xA9},
  {0xC5, 0xFF, 0x7C},
  {0xCD, 0xFF, 0x64},
  {0xCE, 0xFF, 0x7C},
  {0xD5, 0xFF, 0x03},
  {0xD9, 0xFF, 0x04},
  {0,    0,    0}
};

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL608_Ctrl_ProgramRegisters
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 07/23/2010 
--|
--| DESCRIPTION   : This function writes multiple registers with provided data array.
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxL608_Ctrl_ProgramRegisters(UINT8 devId, PMXL608_REG_CTRL_INFO_T ctrlRegInfoPtr)
{
  MXL_STATUS status = MXL_TRUE;
  UINT16 i = 0;
  UINT8 tmp = 0;
  
  while (status == MXL_TRUE)
  {
    if ((ctrlRegInfoPtr[i].regAddr == 0) && (ctrlRegInfoPtr[i].mask == 0) && (ctrlRegInfoPtr[i].data == 0)) break;

    // Check if partial bits of register were updated
    if (ctrlRegInfoPtr[i].mask != 0xFF)  
    {
      status = MxLWare608_OEM_ReadRegister(devId,ctrlRegInfoPtr[i].regAddr, &tmp);
      if (status != MXL_TRUE) break;
    }
    
    tmp &= (UINT8) ~ctrlRegInfoPtr[i].mask;
    tmp |= (UINT8) ctrlRegInfoPtr[i].data;

    status = MxLWare608_OEM_WriteRegister(devId,ctrlRegInfoPtr[i].regAddr, tmp);
    if (status != MXL_TRUE) break;

    i++;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL608_Ctrl_WriteRegField
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 11/3/2009
--|
--| DESCRIPTION   : This function writes registers field that defined by Mask 
--|                 and bit width. The write field position and width are defined 
--|                 by mask byte. For example, if want to write 0x02 to a 
--|                 register bit<5:4>, the input parameter shall be  
--|                 ctrlRegInfoPtr struct:{ Reg_Addr, 0x30, (0x02)<<4 }
--|                 after writing, RegAddr content is --10---- (Bin Format)
--|                   
--|---------------------------------------------------------------------------*/

MXL_STATUS MxL608_Ctrl_WriteRegField(UINT8 devId, PMXL608_REG_CTRL_INFO_T ctrlRegInfoPtr)
{
  MXL_STATUS status = MXL_TRUE;
  UINT8 tmp = 0;
  
  if ((ctrlRegInfoPtr->regAddr == 0) && (ctrlRegInfoPtr->mask == 0) && (ctrlRegInfoPtr->data == 0))
    return MXL_FALSE;

  // Check if partial bits of register were updated
  if (ctrlRegInfoPtr->mask != 0xFF)  
  {
      status = MxLWare608_OEM_ReadRegister(devId, ctrlRegInfoPtr->regAddr, &tmp);
      if (status != MXL_TRUE) return status;
   }

  tmp &= (UINT8)~ctrlRegInfoPtr->mask;  // Clear the field that need to set value 
  tmp |= (UINT8)(ctrlRegInfoPtr->data & ctrlRegInfoPtr->mask); 

  status = MxLWare608_OEM_WriteRegister(devId, ctrlRegInfoPtr->regAddr, tmp);

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL608_Ctrl_SetRfFreqLutTblReg
--|
--| AUTHOR        : Joy Zhang
--|
--| DATE CREATED  : 8/14/2014
--|
--| DESCRIPTION   : This function is called by MxL608_ConfigTunerChanTune, 
--|                 configure 0xEA and 0xEB registers when XTAL is 
--|                 16MHz and 24Hz case 
--|  
--|---------------------------------------------------------------------------*/

MXL_STATUS MxL608_Ctrl_SetRfFreqLutTblReg(UINT8 devId, UINT32 FreqInHz, PMXL608_CHAN_DEPENDENT_FREQ_TABLE_T freqLutPtr, MXL608_XTAL_FREQ_E xtalFreq)
{
  UINT8 status = MXL_TRUE;
  UINT8 idx = 0;
  UINT8 regSetData[MXL608_MAX_SPUR_REG_NUM] = {0, 0};
   
  if (freqLutPtr)
  {
    // Find and get default value firstly. 
    for (idx = 0; 0 != freqLutPtr->centerFreqHz; idx++, freqLutPtr++)
    {
      if (freqLutPtr->centerFreqHz == 1)  
      {
        // When center frequency is 1 means corresponding data is default value 
		status = MxLWare608_OEM_ReadRegister(devId, DFE_REFLUT_BYP_REG, &regSetData[0]); 
		  if (status != MXL_TRUE) return (MXL_STATUS)status;
		regSetData[0] &= 0xDF; //0xEA<5>: Enable spur sifting
		regSetData[0] |= (freqLutPtr->reg_0xEA & 0x20);
        regSetData[1] = freqLutPtr->reg_0xEB;
        break;
      } // end of if ((freqLutPtr->centerFreqHz -
    } // end of for (idx = 0;

    // Check in LUT
    for (idx = 0; 0 != freqLutPtr->centerFreqHz; idx++, freqLutPtr++)
    {
      if ((freqLutPtr->centerFreqHz - MXL608_SPUR_SHIFT_FREQ_WINDOW) <= FreqInHz &&
          (freqLutPtr->centerFreqHz + MXL608_SPUR_SHIFT_FREQ_WINDOW) >= FreqInHz &&
          freqLutPtr->xtalFreq == xtalFreq) // Apply only in selected xtal mode
      {
		status = MxLWare608_OEM_ReadRegister(devId, DFE_REFLUT_BYP_REG, &regSetData[0]); 
		  if (status != MXL_TRUE) return (MXL_STATUS)status;
		regSetData[0] &= 0xDF; //0xEA<5>: Enable spur sifting
		regSetData[0] |= (freqLutPtr->reg_0xEA & 0x20);
        regSetData[1] = freqLutPtr->reg_0xEB;
        break;
      } // end of if ((freqLutPtr->centerFreqHz -
    } // end of for (idx = 0;
  }

  // Program registers
  for (idx = 0; idx < MxL608_SPUR_REGISTER.SpurRegNum; idx++)
    status |= MxLWare608_OEM_WriteRegister(devId, MxL608_SPUR_REGISTER.SpurRegAddr[idx], regSetData[idx]);

  return(MXL_STATUS)status;
}
