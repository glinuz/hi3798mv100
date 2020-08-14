/*******************************************************************************
 *
 * FILE NAME          : MxL68x_PhyCfg.cpp
 * 
 * AUTHOR             : Mahendra Kondur, Dong Liu
 *
 * DATE CREATED       : 02/17/2012, 02/20/2012
 *
 * DESCRIPTION        : This file contains demod and RF control parameters.
 *                      add a new function named Ctrl_WriteRegField
 *                             
 *******************************************************************************
 *                Copyright (c) 2012, MaxLinear, Inc.
 ******************************************************************************/

#include "MxL68x_PhyCfg.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhySpurTable.h"
#include "MxL_Debug.h"

// Assure MxL68x_Dev[] is zero initialized
MXL68x_DEV_CONTEXT_T MxL68x_Dev[MXL68X_MAX_DEVICES];

// Analog NTSC application mode setting
REG_CTRL_INFO_T MxL68x_AnalogNtsc[] = 
{
  {0x0C, 0xFF, 0x08},
  {0x13, 0xFF, 0x00},
  {0x53, 0xFF, 0x7E},
  {0x57, 0xFF, 0x91},
  {0x5C, 0xFF, 0xB1},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x56},
  {0x88, 0xFF, 0x44},
  {0x93, 0xFF, 0x33},
  {0x97, 0xFF, 0xA3},
  {0xBA, 0xFF, 0x40},
  {0x98, 0xFF, 0xA5},
  {0x9B, 0xFF, 0x20},
  {0x9C, 0xFF, 0x20},
  {0xA0, 0xFF, 0x10},
  {0xA5, 0xFF, 0x0F},
  {0xC2, 0xFF, 0xAB},
  {0xC5, 0xFF, 0x7C},
  {0xD5, 0xFF, 0x05},
  {0xD9, 0xFF, 0x00},
  {0xEA, 0xFF, 0x00},
  {0xDC, 0xFF, 0x1C},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP for NTSC HIF mode
REG_CTRL_INFO_T MxL68x_Ntsc_RfLutSwpHIF[] = 
{
  {0x4A, 0xFF, 0x1B},
  {0x4B, 0xFF, 0x1A},
  {0x4C, 0xFF, 0x19},
  {0x4D, 0xFF, 0x16},
  {0x4E, 0xFF, 0x15},
  {0x4F, 0xFF, 0x14},
  {0x50, 0xFF, 0x13},
  {0x51, 0xFF, 0x12},
  {0x52, 0xFF, 0x17},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP for NTSC - 16 MHz XTAL LIF mode
REG_CTRL_INFO_T MxL68x_Ntsc_16MHzRfLutSwpLIF[] = 
{
  {0x4A, 0xFF, 0x1B},
  {0x4B, 0xFF, 0x1A},
  {0x4C, 0xFF, 0x19},
  {0x4D, 0xFF, 0x17},
  {0x4E, 0xFF, 0x16},
  {0x4F, 0xFF, 0x15},
  {0x50, 0xFF, 0x14},
  {0x51, 0xFF, 0x13},
  {0x52, 0xFF, 0x12},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP for NTSC - 24 MHz XTAL LIF mode
REG_CTRL_INFO_T MxL68x_Ntsc_24MHzRfLutSwpLIF[] = 
{
  {0x4A, 0xFF, 0x1C},
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x19},
  {0x4E, 0xFF, 0x17},
  {0x4F, 0xFF, 0x16},
  {0x50, 0xFF, 0x15},
  {0x51, 0xFF, 0x14},
  {0x52, 0xFF, 0x13},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP for NTSC - 24 MHz XTAL LIF mode
REG_CTRL_INFO_T MxL68x_Ntsc_HRCRfLutSwpLIF[] = 
{
  {0x4A, 0xFF, 0x1C},
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x19},
  {0x4E, 0xFF, 0x17},
  {0x4F, 0xFF, 0x16},
  {0x50, 0xFF, 0x15},
  {0x51, 0xFF, 0x14},
  {0x52, 0xFF, 0x13},
  {0xDD, 0xFF, 0x01},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP settings mode PAL LIF
REG_CTRL_INFO_T MxL68x_Pal_RfLutSwpLIF[] = 
{
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x19},
  {0x4E, 0xFF, 0x17},
  {0x4F, 0xFF, 0x16},
  {0x50, 0xFF, 0x15},
  {0x51, 0xFF, 0x14},
  {0x52, 0xFF, 0x13},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP settings mode PAL-D HIF
REG_CTRL_INFO_T MxL68x_PalD_RfLutSwpHIF[] = 
{
  {0x4B, 0xFF, 0x14},
  {0x4C, 0xFF, 0x13},
  {0x4D, 0xFF, 0x15},
  {0x4E, 0xFF, 0x1B},
  {0x4F, 0xFF, 0x1E},
  {0x50, 0xFF, 0x16},
  {0x51, 0xFF, 0x1A},
  {0x52, 0xFF, 0x19},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP settings mode PAL-I HIF
REG_CTRL_INFO_T MxL68x_PalI_RfLutSwpHIF[] = 
{
  {0x4B, 0xFF, 0x16},
  {0x4C, 0xFF, 0x15},
  {0x4D, 0xFF, 0x14},
  {0x4E, 0xFF, 0x13},
  {0x4F, 0xFF, 0x1B},
  {0x50, 0xFF, 0x19},
  {0x51, 0xFF, 0x1A},
  {0x52, 0xFF, 0x17},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP settings mode PAL-BG 8 MHz BW HIF
REG_CTRL_INFO_T MxL68x_PalBG_8MHzBW_RfLutSwpHIF[] = 
{
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x19},
  {0x4E, 0xFF, 0x17},
  {0x4F, 0xFF, 0x16},
  {0x50, 0xFF, 0x15},
  {0x51, 0xFF, 0x14},
  {0x52, 0xFF, 0x13},
  {0,    0,    0}
};

// DFE_RFLUT_DIV_IN_SWP settings mode PAL-BG 7 MHz BW HIF
REG_CTRL_INFO_T MxL68x_PalBG_7MHzBW_RfLutSwpHIF[] = 
{
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x15},
  {0x4E, 0xFF, 0x14},
  {0x4F, 0xFF, 0x13},
  {0x50, 0xFF, 0x16},
  {0x51, 0xFF, 0x17},
  {0x52, 0xFF, 0x19},
  {0,    0,    0}
};

// Analog PAL application mode setting
REG_CTRL_INFO_T MxL68x_AnalogPal[] = 
{
  //{0x0C, 0xFF, 0x0C}, // Not need according to Pegasus
  {0x0F, 0xFF, 0x00},
  {0x13, 0xFF, 0x00},
  {0x53, 0xFF, 0x7E},
  {0x57, 0xFF, 0x91},
  {0x5C, 0xFF, 0xB1},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x56},
  {0x88, 0xFF, 0x44},
  {0x93, 0xFF, 0x33},
  {0x97, 0xFF, 0xA3},
  {0x98, 0xFF, 0xA5},
  {0x9B, 0xFF, 0x20},
  {0x9C, 0xFF, 0x20},
  {0xA0, 0xFF, 0x00},
  {0xA5, 0xFF, 0x0F},
  {0xC2, 0xFF, 0xAB},
  {0xC5, 0xFF, 0x7C},
  {0xD5, 0xFF, 0x05},
  {0xD9, 0xFF, 0x00},
  {0xEA, 0xFF, 0x00},
  {0x49, 0xFF, 0x1D},
  {0x4A, 0xFF, 0x1C},
  {0xDD, 0xFF, 0x06},
  {0xDC, 0xFF, 0x1C},
  {0,    0,    0}
};

// Analog SECAM application mode setting
REG_CTRL_INFO_T MxL68x_AnalogSecam[] = 
{
  //{0x0C, 0xFF, 0x0B}, // Not need according to Pegasus
  {0x0F, 0xFF, 0x00},
  {0x13, 0xFF, 0x00},
  {0x53, 0xFF, 0x7E},
  {0x57, 0xFF, 0x91},
  {0x5C, 0xFF, 0xB1},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x56},
  {0x88, 0xFF, 0x44},
  {0x93, 0xFF, 0x33},
  {0x97, 0xFF, 0xA3},
  {0xBA, 0xFF, 0x40}, 
  {0x98, 0xFF, 0x95},
  {0x9B, 0xFF, 0x66},
  {0x9C, 0xFF, 0x1F},
  {0xA0, 0xFF, 0x10},
  {0xA5, 0xFF, 0x0F},
  {0xC2, 0xFF, 0xA8},
  {0xC5, 0xFF, 0xBD},
  {0xD5, 0xFF, 0x05},
  {0xD9, 0xFF, 0x00},
  {0xEA, 0xFF, 0x00},
  {0x49, 0xFF, 0x1D},
  {0x4A, 0xFF, 0x1C},
  {0x4B, 0xFF, 0x1B},
  {0x4C, 0xFF, 0x1A},
  {0x4D, 0xFF, 0x19},
  {0x4E, 0xFF, 0x17},
  {0x4F, 0xFF, 0x16},
  {0x50, 0xFF, 0x15},
  {0x51, 0xFF, 0x14},
  {0x52, 0xFF, 0x13},
  {0xDD, 0xFF, 0x06},
  {0xDC, 0xFF, 0x1C},
  {0,    0,    0}
};

// Digital DVB-C application mode setting
REG_CTRL_INFO_T MxL68x_DigitalDvbc[] = 
{
  {0x0F, 0xFF, 0x20},
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
  {0xD5, 0xFF, 0x05},
  {0xEA, 0xFF, 0x00},
  {0xDC, 0xFF, 0x1C},
  {0,    0,    0}
};

// Digital ISDBT & ATSC application mode setting
REG_CTRL_INFO_T MxL68x_DigitalIsdbtAtsc[] = 
{
  {0x0F, 0xFF, 0x20},
  {0x0C, 0xFF, 0x00},
  {0x13, 0xFF, 0x04},
  {0x53, 0xFF, 0xFE},
  {0x57, 0xFF, 0x90},
  {0x62, 0xFF, 0xC2},
  {0x6E, 0xFF, 0x01},
  {0x6F, 0xFF, 0x51},
  {0x87, 0xFF, 0x77},
#ifdef CUSTOMER_SPECIFIC_SETTING_1
  {0x88, 0xFF, 0x77},
#else
  {0x88, 0xFF, 0x55},
#endif
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
  {0xD5, 0xFF, 0x03},
  {0xD9, 0xFF, 0x04},
  {0,    0,    0}
};

// Digital DVB-T 6MHz application mode setting
REG_CTRL_INFO_T MxL68x_DigitalDvbt[] = 
{
  {0x0F, 0xFF, 0x20},
  {0x13, 0xFF, 0x04},
  {0x0C, 0xFF, 0x00},
  {0x53, 0xFF, 0xFE},
  {0x57, 0xFF, 0x90},
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
  {0xD5, 0xFF, 0x03},
  {0xD9, 0xFF, 0x04},
  {0,    0,    0}
};

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_GetDeveiceContext
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 01/21/2012
--|
--| DESCRIPTION   : Retrieve MxL68x device context
--|
--| INPUTS        : devId 
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

PMXL68x_DEV_CONTEXT_T MxL68x_Ctrl_GetDeviceContext(UINT8 devId)
{
  return (devId < MXL68X_MAX_DEVICES) ? (&MxL68x_Dev[devId]) : NULL;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CheckIntegration
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/06/2010
--|
--| DESCRIPTION   : This function check if received data is integrate or not by 
--|                 compare CheckSum.
--|
--| RETURN VALUE  : MXL_SUCCESS: data is complete, MXL_FAILURE: not complete
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_CheckIntegration(UINT8 readCheckSum, UINT8 readDataLen, UINT8 *readDataPtr)
{
  UINT16 sum = 0;
  UINT8 len;

  if (readDataLen == 0)
    return(MXL_SUCCESS);  // Do not need to check integrate 
    
  for (len = 0; len < readDataLen; len ++)
    sum += *readDataPtr++;
 
  if ((sum & 0xff) == readCheckSum) 
    return(MXL_SUCCESS);  // Data is complete 
  else 
    return(MXL_FAILURE); // Data is not complete
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SendHostCommand
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/19/2010
--|
--| DESCRIPTION   : This function send host command to device, not read response.
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_SendHostCommand(UINT8 devId, PHOST_COMMAND_T commandPtr)
{
  UINT16 bufferSize,i;
  MXL_STATUS_E retStatus = MXL_SUCCESS;
  UINT16 RegAddr; 
  UINT8 writeBuffer[sizeof(HOST_COMMAND_T)]; 

  // If there is command data, copy it into writeBuffer
  if (commandPtr->payloadLen > 0)
  {
    for (i = 0; i < commandPtr->payloadLen; i++)  // Copy data from commandPtr->data to writeBuffer
      writeBuffer[2 + i] = commandPtr->data[i];

    bufferSize = commandPtr->payloadLen + 2; 
  }
  else 
    bufferSize = 2; 

  RegAddr = commandPtr->syncChar; 
  RegAddr = (RegAddr << 8) + (UINT16)(commandPtr->commandId);
  writeBuffer[0] = commandPtr->payloadCheckSum; 
  writeBuffer[1] = commandPtr->payloadLen;  

  // Send host command to MxL68x 
  retStatus = MxLWare68x_OEM_WriteBlock(devId, RegAddr, bufferSize, writeBuffer);

  return retStatus;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_DfeRegRead
--|
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 07/31/2012
--|
--| DESCRIPTION   : This uses a host command to read DFE register (typically multibyte registers)
--|                 This is needed as for multibyte access, the dfe register access need to be one shot
--|                 They cannot be separate accesses in host
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_DfeRegRead(UINT8 devId, UINT16 regAddr, UINT8 numRegs, UINT32* regDataPtr )
{

  UINT8 status = MXL_SUCCESS;
  HOST_COMMAND_T dfeRegRead;
  UINT16 tmp = 0;
  UINT8 checksum = 0;

  FORM_CMD_HEADER(dfeRegRead, HOST_CMD_DEV_DFE_REG_READ, 0, 3);  

  dfeRegRead.data[0] = (regAddr >> 8) & 0xFF;
  checksum +=  dfeRegRead.data[0];
  
  dfeRegRead.data[1] = regAddr & 0xFF;
  checksum +=  dfeRegRead.data[1];
  
  dfeRegRead.data[2] = numRegs;
  checksum +=  dfeRegRead.data[2];

  dfeRegRead.payloadCheckSum = checksum;

  status |= Ctrl_SendHostCommand(devId, &dfeRegRead);

  // Add 1 ms waiting time 
  MxLWare68x_OEM_SleepInUs(1000);

  status |= MxL68x_Ctrl_ReadRegister(devId,DFE_FW_REG_BUFFER_MSB, &tmp);
  *regDataPtr = tmp;
  
  status |= MxL68x_Ctrl_ReadRegister(devId,DFE_FW_REG_BUFFER_LSB, &tmp);
  *regDataPtr = (*regDataPtr << 16) | tmp;

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_DfeRegWrite
--|
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 07/31/2012
--|
--| DESCRIPTION   : This uses a host command to write DFE register (typically multibyte registers)
--|                 This is needed as for multibyte access, the dfe register access need to be one shot
--|                 They cannot be separate accesses in host
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_DfeRegWrite(UINT8 devId, UINT16 regAddr, UINT8 numRegs, UINT32 regData )
{

  UINT8 status = MXL_SUCCESS;
  HOST_COMMAND_T dfeRegWrite;
  UINT8 checksum = 0;

  FORM_CMD_HEADER(dfeRegWrite, HOST_CMD_DEV_DFE_REG_WRITE, 0, 7);  

  dfeRegWrite.data[0] = (regAddr >> 8) & 0xFF;
  checksum +=  dfeRegWrite.data[0];
  
  dfeRegWrite.data[1] = regAddr & 0xFF;
  checksum +=  dfeRegWrite.data[1];
  
  // Send the 32 bits of Reg Data 
  dfeRegWrite.data[2] = numRegs;
  checksum +=  dfeRegWrite.data[2];

  dfeRegWrite.data[3] = (regData >> 24) & 0xFF;
  checksum +=  dfeRegWrite.data[3];
  
  dfeRegWrite.data[4] = (regData >> 16) & 0xFF;
  checksum +=  dfeRegWrite.data[4];

  dfeRegWrite.data[5] = (regData >> 8) & 0xFF;
  checksum +=  dfeRegWrite.data[5];
  
  dfeRegWrite.data[6] = regData & 0xFF;
  checksum +=  dfeRegWrite.data[6];
  
  dfeRegWrite.payloadCheckSum = checksum;

  status |= Ctrl_SendHostCommand(devId, &dfeRegWrite);

  // Add 1 ms waiting time 
  MxLWare68x_OEM_SleepInUs(1000);

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_GetDeviceResp
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/19/2010
--|
--| DESCRIPTION   : This function gets device response from certain Sub address.
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_GetDeviceResp(UINT8 devId, UINT16 subAddr, PFW_RESPONSE_T responsePtr)
{
  UINT16 bufferSize,i;
  UINT8 status = MXL_SUCCESS;
  UINT8 readBuffer[sizeof(HOST_COMMAND_T)];

  // Read response header firstly 
  status |= MxLWare68x_OEM_ReadBlock(devId, subAddr, COMMAND_HEADER_LENGTH, readBuffer);

  MxL_DLL_DEBUG0("Block read back : SYNC=0x%02X, errorCode=0x%02X, checkSum=0x%02X, dataLen=0x%02X. \n",
     readBuffer[0],readBuffer[1],readBuffer[2],readBuffer[3]);

  if (status == MXL_SUCCESS)  // Read block success 
  {
    responsePtr->syncChar = readBuffer[0];
    responsePtr->errorCode = (MXL_RSP_ID_E)readBuffer[1];
    responsePtr->payloadCheckSum = readBuffer[2];
    responsePtr->payloadLen = readBuffer[3];

    // Check if previous command execution success or not
    if (responsePtr->errorCode == RESPONSE_ID_SUCCESS)
    {
      // Check if need to read response data
      if (responsePtr->payloadLen > 0)
      {
        bufferSize = responsePtr->payloadLen;
     
        // Read Response data 
        status |= MxLWare68x_OEM_ReadBlock(devId, subAddr, bufferSize, readBuffer);  

        // Check if received response data is complete or not 
        Ctrl_CheckIntegration(responsePtr->payloadCheckSum, (UINT8)bufferSize, readBuffer);

        if (status == MXL_SUCCESS) // Read response data success 
        {
          for (i = 0; i < bufferSize; i++)  // Copy data from readBuffer to responsePtr->data
            responsePtr->data[i] = readBuffer[i]; 
/*
         MxL_DLL_DEBUG0("Block read back : SYNC=0x%02X, errorCode=0x%02X, checkSum=0x%02X, dataLen=0x%02X. \n",
                                  readBuffer[0],readBuffer[1],readBuffer[2],readBuffer[3]);
*/
        }
      }
    }
    else 
      status = MXL_FAILURE;
  }
  return((MXL_STATUS_E)status);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_HostComm
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/19/2010
--|
--| DESCRIPTION   : This function send host command and get response between host
--|                 and firmware.
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_HostComm(UINT8 devId, PHOST_COMMAND_T commandPtr, PFW_RESPONSE_T responsePtr)
{
  UINT16 bufferSize,i;
  UINT16 RegAddr; 
  UINT8 status = MXL_SUCCESS;
  UINT8 writeBuffer[sizeof(HOST_COMMAND_T)]; 
  UINT8 readBuffer[sizeof(HOST_COMMAND_T)];

  // If there is command data, copy it into writeBuffer
  if (commandPtr->payloadLen > 0)
  {
    for (i = 0; i < commandPtr->payloadLen; i++)  // Copy data from commandPtr->data to writeBuffer
      writeBuffer[2 + i] = commandPtr->data[i];

    /* Write Buffer include both payloadCheckSum and payloadLen two bytes */
    bufferSize = commandPtr->payloadLen + 2; 
  }
  else 
    bufferSize = 2; // Write Buffer only contais payloadCheckSum and payloadLen two bytes 

  // Form RegAddr using syncChar and Command ID bytes 
  RegAddr = commandPtr->syncChar; 
  RegAddr = (RegAddr << 8) + (UINT16)(commandPtr->commandId);
  // Write buffer begining two bytes are payload check sum and payload data length 
  writeBuffer[0] = commandPtr->payloadCheckSum; 
  writeBuffer[1] = commandPtr->payloadLen;  

  // Send host command to MxL68x 
  status = MxLWare68x_OEM_WriteBlock(devId, RegAddr, bufferSize, writeBuffer);

  // Read response header firstly 
  status |= MxLWare68x_OEM_ReadBlock(devId, (UINT16)commandPtr->commandId, COMMAND_HEADER_LENGTH, readBuffer);

  MxL_DLL_DEBUG0("Block read back : SYNC=0x%02X, errorCode=0x%02X, checkSum=0x%02X, dataLen=0x%02X. \n",
     readBuffer[0],readBuffer[1],readBuffer[2],readBuffer[3]);

  if (status == MXL_SUCCESS)  // Block read operation is successful 
  {
    responsePtr->syncChar = readBuffer[0];
    responsePtr->errorCode = (MXL_RSP_ID_E)readBuffer[1];
    responsePtr->payloadCheckSum = readBuffer[2];
    responsePtr->payloadLen = readBuffer[3];

    // Check if previous command execution success or not
    if (responsePtr->errorCode == RESPONSE_ID_SUCCESS)
    {
      // Check if need to read response data (check if Payload length is large than 0 ) 
      if (responsePtr->payloadLen > 0)
      {
        bufferSize = responsePtr->payloadLen;
     
        // Read Response data 
        status |= MxLWare68x_OEM_ReadBlock(devId, (UINT16)commandPtr->commandId, bufferSize, readBuffer);  

        // Check if received response data is complete or not 
        status |= Ctrl_CheckIntegration(responsePtr->payloadCheckSum, (UINT8)bufferSize, readBuffer);

        if (status == MXL_SUCCESS) // Read response data success 
        {
          for (i = 0; i < bufferSize; i++)  // Copy data from readBuffer to responsePtr->data
            responsePtr->data[i] = readBuffer[i]; 
        }
      }
    }
    else return(MXL_FAILURE); // There is erro because response errorCode is not RESPONSE_ID_SUCCESS 
  }

  return((MXL_STATUS_E)status);
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_ProgramRegisters
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

MXL_STATUS_E MxL68x_Ctrl_ProgramRegisters(UINT8 devId, PREG_CTRL_INFO_T ctrlRegInfoPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT16 i = 0;
  UINT16 tmp = 0;
  
  i = 0;
  while (1)   
  {
    if ((ctrlRegInfoPtr[i].regAddr == 0) && (ctrlRegInfoPtr[i].mask == 0) && (ctrlRegInfoPtr[i].data == 0)) 
      break; 

    // Check if partial bits of register were updated
    if (ctrlRegInfoPtr[i].mask != 0xFFFF)  
    {
      status = MxL68x_Ctrl_ReadRegister(devId,ctrlRegInfoPtr[i].regAddr, &tmp);
      if (status != MXL_SUCCESS) break;
    }
    
    tmp &= ~ctrlRegInfoPtr[i].mask;
    tmp |= ctrlRegInfoPtr[i].data;

    status = MxL68x_Ctrl_WriteRegister(devId,ctrlRegInfoPtr[i].regAddr, tmp);
    if (status != MXL_SUCCESS) break;

    i++;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SetRfFreqLutReg
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/29/2012
--|
--| DESCRIPTION   : This function is called by MxL601_ConfigTunerChanTune, 
--|                 configure 0xDC, 0xDD, 0xF0 and 0xF1 registers when XTAL is 
--|                 16MHz and 24Hz case 
--|  
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_SetRfFreqLutReg(UINT8 devId, UINT32 FreqInHz, PMXL68x_CHAN_DEP_FREQ_TABLE_T freqLutPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 idx = 0;
  UINT16 readBack, regSetData[MAX_SPUR_REG_NUM] = {0, 0, 0, 0};
  CHAN_DEP_SPUR_REG_T MxL68x_SPUR_REGISTER;

  MxL68x_SPUR_REGISTER.SpurRegNum = MAX_SPUR_REG_NUM;
  MxL68x_SPUR_REGISTER.SpurRegAddr[0] =  DFE_RFLUT_SWP1_REG;
  MxL68x_SPUR_REGISTER.SpurRegAddr[1] =  DFE_RFLUT_DIV_MOD_REG;
  MxL68x_SPUR_REGISTER.SpurRegAddr[2] =  DFE_REFLUT_BYP_REG;
  MxL68x_SPUR_REGISTER.SpurRegAddr[3] =  DFE_REFSX_INT_MOD_REG;

  if (freqLutPtr)
  {
    // Find and get default value firstly.
    for (idx = 0; 0 != freqLutPtr->centerFreqHz; idx++, freqLutPtr++)
    {
      if (freqLutPtr->centerFreqHz == 1)  
      {
        // When center frequency is 1 means corresponding data is default value 
        regSetData[0] = freqLutPtr->rfLutSwp1Reg;
        regSetData[1] = freqLutPtr->rfLutDivInBypReg;
        regSetData[2] = freqLutPtr->refLutBypReg;
        regSetData[3] = freqLutPtr->refIntModReg;
        
        break;
      } // end of if ((freqLutPtr->centerFreqHz -
    } // end of for (idx = 0;

    // Check in LUT
    for (idx = 0; 0 != freqLutPtr->centerFreqHz; idx++, freqLutPtr++)
    {
      if ((freqLutPtr->centerFreqHz - SPUR_SHIFT_FREQ_WINDOW) <= FreqInHz &&
          (freqLutPtr->centerFreqHz + SPUR_SHIFT_FREQ_WINDOW) >= FreqInHz)
      {
        regSetData[0] = freqLutPtr->rfLutSwp1Reg;
        regSetData[1] = freqLutPtr->rfLutDivInBypReg;
        regSetData[2] = freqLutPtr->refLutBypReg;
        regSetData[3] = freqLutPtr->refIntModReg;

        break;
      } // end of if ((freqLutPtr->centerFreqHz -
    } // end of for (idx = 0;
  }

  // Program registers
  for (idx = 0; idx < MxL68x_SPUR_REGISTER.SpurRegNum; idx++)
  {
    if (MxL68x_SPUR_REGISTER.SpurRegAddr[idx] == DFE_REFLUT_BYP_REG)
    {
      status |= MxL68x_Ctrl_ReadRegister(devId, MxL68x_SPUR_REGISTER.SpurRegAddr[idx], &readBack);
      regSetData[idx] |= (readBack & 0x00DF);  // Protect bit <4:0>, clear bit<5>, 1101 1111 = 0xDF  
    }
    status |= MxL68x_Ctrl_WriteRegister(devId, MxL68x_SPUR_REGISTER.SpurRegAddr[idx], regSetData[idx]);
  }

  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CfgDevReset
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/31/2011
--|
--| DESCRIPTION   : This function will reset the device or respective device 
--|                 functional modules based on parameter.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_CfgDevReset(UINT8 devId, MXL68x_RESET_TYPE_E resetType)
{ 
  UINT8 status = MXL_SUCCESS;
  UINT16 msbRegData = 0;
  UINT16 lsbRegData = 0;
  HOST_COMMAND_T tunerReset;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Ctrl_CfgDevReset\n");

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      // Read DMD_CCTL_RSTRQ_N_REG, read MSB 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, &msbRegData); 
      
      // Read DMD_CCTL_RSTRQ_N_REG, read LSB 
      status |= MxL68x_Ctrl_ReadRegister(devId, DMD_CCTL_RSTRQ_N_REG, &lsbRegData);  
          
      switch (resetType)
      {
        // Bit<31:16>, Set bit<25> as 1 
        case MXL68x_DEMOD_DATAPATH_RESET: msbRegData |= 0x0200; break; 

        // Bit<31:16>, Set bit<24> as 1 
        case MXL68x_DEMOD_RESET: msbRegData |= 0x0100; break; 

        case MXL68x_TUNER_RESET: 
          FORM_CMD_HEADER(tunerReset, HOST_CMD_TUNER_DFE_RESET_CFG, 0, 0);   
          status |= Ctrl_SendHostCommand(devId, &tunerReset);
          break; 

        default: 
          break;   
      }

      if(MXL68x_TUNER_RESET != resetType)
      {
        // Program MSB part of the reset register
        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, msbRegData); 

        // Write LSB part of the reset register
        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG, (lsbRegData|0x0008));

        // Tuner & Demod blocks are active reset, clear reset bits to start reset function
        // REVIST for TUNER RESET - MK
        switch (resetType)
        {
          // Bit<31:16>, Set bit<25> as 0 
          case MXL68x_DEMOD_DATAPATH_RESET: msbRegData &= 0xFDFF; break; 

          // Bit<31:16>, Set bit<24> as 0 
          case MXL68x_DEMOD_RESET: msbRegData &= 0xFEFF; break; 

          default: 
            break;   
        }

        // Program MSB part of the reset register
        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG - 1, msbRegData); 

        // Write LSB part of the reset register
        status |= MxL68x_Ctrl_WriteRegister(devId, DMD_CCTL_RSTRQ_N_REG, (lsbRegData|0x0008));
      }
    }
    else
      status = MXL_NOT_INITIALIZED;
  }
  else
    status = MXL_INVALID_PARAMETER;

  return(MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_RegisterIntHandlerFunc
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 2/21/2012
--|
--| DESCRIPTION   : Register interrupt handler call back function according 
--|                 that corresponding to specified device ID
--| 
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_RegisterIntHandlerFunc(UINT8 devId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 k;
  INTERRUPT_TRIGGERED_CB_FN FwInterruptHandleFunc = Ctrl_IntHandlerForIntSource1;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Ctrl_RegisterIntHandlerFunc \n");

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
      devContextPtr->fwIntRetData.ValidFlag = MXL_INVALID;  // Set the ValidFlag as invalid

      for (k = 0; k <FW_CHAN_TUNE_DATA_LENGTH ; k++)
        devContextPtr->fwIntRetData.Data[k] = 0;

      // Register call back function to Ctrl_GetFwIntData
						switch (devContextPtr->intSrcId)
      {
        case MXL68x_INT_SOURCE1: 
									 FwInterruptHandleFunc = Ctrl_IntHandlerForIntSource1; 
          MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource1 function is registered. \n");
          break; 

        case MXL68x_INT_SOURCE2: 
									 FwInterruptHandleFunc = Ctrl_IntHandlerForIntSource2; 
          MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource2 function is registered. \n");
          break; 

        case MXL68x_INT_SOURCE3: 
									 FwInterruptHandleFunc = Ctrl_IntHandlerForIntSource3; 
          MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource3 function is registered. \n");
          break; 

        case MXL68x_INT_SOURCE4: 
									 FwInterruptHandleFunc = Ctrl_IntHandlerForIntSource4; 
          MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource4 function is registered. \n");
          break; 

        default: break; 
      }      
      MxL_DLL_DEBUG0("Interrupt source %d is registered. \n", devContextPtr->intSrcId);
      status = MxLWare68x_OEM_RegisterCallBack(FwInterruptHandleFunc, devContextPtr->intSrcId);      
    } 
    else 
      status = MXL_NOT_INITIALIZED;
  } 
  else 
    status = MXL_INVALID_PARAMETER;

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_UnRegisterIntHandlerFunc
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 2/21/2012
--|
--| DESCRIPTION   : un-register interrupt handler call back function according 
--|                 that corresponding to specified device ID
--| 
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_UnRegisterIntHandlerFunc(UINT8 devId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Ctrl_UnRegisterIntHandlerFunc \n");

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
					 devContextPtr->fwIntRetData.ValidFlag = MXL_INVALID;
					 status = MxLWare68x_OEM_UnRegisterCallBack(devContextPtr->intSrcId);
				}
    else 
      status = MXL_NOT_INITIALIZED;
  } 
  else 
    status = MXL_INVALID_PARAMETER;

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_IntHandlerForIntSource1
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/07/2011
--|
--| DESCRIPTION   : Response firmware interrupt, read back data from firmware 
--|                 according to interrupt type.
--| 
--|---------------------------------------------------------------------------*/

void Ctrl_IntHandlerForIntSource1(void)
{
  UINT8 devId = MXL68X_MAX_DEVICES;
  PMXL68x_DEV_CONTEXT_T devContextPtr;

  MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource1 \n");

		for (devId = 0; devId < MXL68X_MAX_DEVICES; devId++)
  {
			 devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId); 
				if (devContextPtr)  
				{
						if (devContextPtr->driverStatus && 
							   (devContextPtr->intSrcId == MXL68x_INT_SOURCE1))
						{   
        // Find the device's ID that its I2C address is same as interrupt source 1  
        MxL_DLL_DEBUG0("Find Device %d interrupt source is SOURCE1. \n", devId);
        break; 
      } // End of if (devContextPtr->driverStatus)
    } // End of if (devContextPtr)
    else MxL_DLL_DEBUG0("Device %d 's driver context is empty.", devId);      
  } // End of for (k = 0;

  if (devId < MXL68X_MAX_DEVICES) 
			 Ctrl_InterruptProcess(devId);
		else 
			 MxL_DLL_DEBUG0("Calling interrupt handler function for device %d is failed.", devId);

  return;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_IntHandlerForIntSource2
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/07/2011
--|
--| DESCRIPTION   : Response firmware interrupt, read back data from firmware 
--|                 according to interrupt type.
--| 
--|---------------------------------------------------------------------------*/

void Ctrl_IntHandlerForIntSource2(void)
{
  UINT8 devId = MXL68X_MAX_DEVICES;
  PMXL68x_DEV_CONTEXT_T devContextPtr;

  MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource2 \n");

		for (devId = 0; devId < MXL68X_MAX_DEVICES; devId++)
  {
			 devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId); 
				if (devContextPtr)  
				{
						if (devContextPtr->driverStatus && 
							   (devContextPtr->intSrcId == MXL68x_INT_SOURCE2))
						{   
        // Find the device's ID that its I2C address is same as interrupt source 2  
        break; 
      } // End of if (devContextPtr->driverStatus
    } // End of if (devContextPtr)
    else MxL_DLL_DEBUG0("Device %d 's driver context is empty.", devId);      
  } // End of for (k = 0;

  if (devId < MXL68X_MAX_DEVICES) 
			 Ctrl_InterruptProcess(devId);
		else 
			 MxL_DLL_DEBUG0("Calling interrupt handler function for device %d is failed.", devId);

  return;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_IntHandlerForIntSource3
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/07/2011
--|
--| DESCRIPTION   : Response firmware interrupt, read back data from firmware 
--|                 according to interrupt type.
--| 
--|---------------------------------------------------------------------------*/

void Ctrl_IntHandlerForIntSource3(void)
{
  UINT8 devId = MXL68X_MAX_DEVICES;
  PMXL68x_DEV_CONTEXT_T devContextPtr;

  MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource3 \n");

		for (devId = 0; devId < MXL68X_MAX_DEVICES; devId++)
  {
			 devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId); 
				if (devContextPtr)  
				{
						if (devContextPtr->driverStatus && 
							   (devContextPtr->intSrcId == MXL68x_INT_SOURCE3))
						{   
        // Find the device's ID that its I2C address is same as interrupt source 3  
        break; 
      } // End of if (devContextPtr->driverStatus
    } // End of if (devContextPtr)
    else MxL_DLL_DEBUG0("Device %d 's driver context is empty.", devId);      
  } // End of for (k = 0;

  if (devId < MXL68X_MAX_DEVICES) 
			 Ctrl_InterruptProcess(devId);
		else 
			 MxL_DLL_DEBUG0("Calling interrupt handler function for device %d is failed.", devId);

  return;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_IntHandlerForIntSource4
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/07/2011
--|
--| DESCRIPTION   : Response firmware interrupt, read back data from firmware 
--|                 according to interrupt type.
--| 
--|---------------------------------------------------------------------------*/

void Ctrl_IntHandlerForIntSource4(void)
{
  UINT8 devId = MXL68X_MAX_DEVICES;
  PMXL68x_DEV_CONTEXT_T devContextPtr;

  MxL_DLL_DEBUG0("Ctrl_IntHandlerForIntSource4 \n");

		for (devId = 0; devId < MXL68X_MAX_DEVICES; devId++)
  {
			 devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId); 
				if (devContextPtr)  
				{
						if (devContextPtr->driverStatus && 
							   (devContextPtr->intSrcId == MXL68x_INT_SOURCE4))
						{   
        // Find the device's ID that its I2C address is same as interrupt source 4  
        break; 
      } // End of if (devContextPtr->driverStatus
    } // End of if (devContextPtr)
    else MxL_DLL_DEBUG0("Device %d 's driver context is empty.", devId);      
  } // End of for (k = 0;

  if (devId < MXL68X_MAX_DEVICES) 
			 Ctrl_InterruptProcess(devId);
		else 
			 MxL_DLL_DEBUG0("Calling interrupt handler function for device %d is failed.", devId);

  return;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_InterruptProcess
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/07/2011
--|
--| DESCRIPTION   : Response firmware interrupt, read back data from firmware 
--|                 according to interrupt type.
--| 
--|---------------------------------------------------------------------------*/

//#define  DEBUG_INT_READ_DATA_FUNCTION 1

void Ctrl_InterruptProcess(UINT8 devId)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 k;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Ctrl_InterruptProcess \n");

  if (devContextPtr)
  {
    if (devContextPtr->driverStatus)
    {
#ifdef DEBUG_INT_READ_DATA_FUNCTION
						devContextPtr->fwIntRetData.ValidFlag = MXL_VALID;
						for (k = 0; k < ISDBT_AC_DATA_LENGTH; k++)
										devContextPtr->fwIntRetData.Data[k] = k+0x01; 
      devContextPtr->fwIntRetData.DataLength = ISDBT_AC_DATA_LENGTH;
#else 
						FW_RESPONSE_T getIntRetDataRsp;
						HOST_COMMAND_T acDataToGpoCfg;
						UINT16 readBack = 0, control;

						// Check if current interrupt is AC Data interrupt or not 
						status |= MxL68x_Ctrl_ReadRegister(devId, DMD_INTERRUPT_MAILBOX_REG, &readBack);

						if ((readBack & DMD_INT_MASK_AC_DATA) == DMD_INT_MASK_AC_DATA)
						{
								// AC data interupt comes , then clear this mask 
								control = readBack & 0xFFFE; // Clear DMD_INT_MASK_AC_DATA bit 
								status |= MxL68x_Ctrl_WriteRegister(devId, DMD_INTERRUPT_MAILBOX_REG, control);

								if (devContextPtr->acDataGpoOut == MXL_ENABLE)
								{
										// Send HOST_CMD_DEMOD_AC_DATA_GPO_CFG command to firmware 
										FORM_CMD_HEADER(acDataToGpoCfg, HOST_CMD_DEMOD_ACDATA_GPO_CFG, 1, 0);
										status |= Ctrl_SendHostCommand(devId, &acDataToGpoCfg);
								}
								else // AC data is not streamed out to GPO 
								{
										// Get AC data from firmware 
										status |= Ctrl_GetDeviceResp(devId, HOST_CMD_DEMOD_AC_DATA_REQ, &getIntRetDataRsp);

										if ((status == MXL_SUCCESS) && (getIntRetDataRsp.errorCode == RESPONSE_ID_SUCCESS) 
													&& (getIntRetDataRsp.payloadLen == ISDBT_AC_DATA_LENGTH))
										{
												devContextPtr->fwIntRetData.ValidFlag = MXL_VALID;
												devContextPtr->fwIntRetData.DataLength = getIntRetDataRsp.payloadLen;
												for (k = 0; k <getIntRetDataRsp.payloadLen; k++)
														devContextPtr->fwIntRetData.Data[k] = getIntRetDataRsp.data[k]; 
										}
										else 
												MxL_DLL_DEBUG0("Ctrl_InterruptProcess: Response has error !");
								}
						}
				 
						if ((readBack & DMD_INT_MASK_FW_CHAN_TUNE) == DMD_INT_MASK_FW_CHAN_TUNE)
						{
								// FW channel Tune interupt comes , then clear this mask 
								control = readBack & 0xFFFD; // Clear DMD_INT_MASK_FW_CHAN_TUNE bit 
								status |= MxL68x_Ctrl_WriteRegister(devId, DMD_INTERRUPT_MAILBOX_REG, control);

								// Get AC data from firmware 
								status |= Ctrl_GetDeviceResp(devId, HOST_CMD_TUNER_TUNE_RESULT_REQ, &getIntRetDataRsp);

								if ((status == MXL_SUCCESS) && (getIntRetDataRsp.errorCode == RESPONSE_ID_SUCCESS) 
											&& (getIntRetDataRsp.payloadLen == FW_CHAN_TUNE_DATA_LENGTH))
								{
										devContextPtr->fwIntRetData.ValidFlag = MXL_VALID;
										devContextPtr->fwIntRetData.DataLength = getIntRetDataRsp.payloadLen;
										for (k = 0; k <getIntRetDataRsp.payloadLen; k++)
												devContextPtr->fwIntRetData.Data[k] = getIntRetDataRsp.data[k]; 
								}
								else 
										MxL_DLL_DEBUG0("Ctrl_InterruptProcess: Response has error !");    
						}
#endif 
    } 
    else 
      MxL_DLL_DEBUG0("Device %d driver context is not initialized.", devId);
  } 
  else 
    MxL_DLL_DEBUG0("Device %d driver context is empty.", devId);

  return;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CheckFirmwareStatus
--| 
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 12/31/2011
--|
--| DESCRIPTION   : This function will detect if Firmware is loaded or not.  
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER or
--|                 MXL_FAILURE
--|---------------------------------------------------------------------------*/

MXL68x_FW_STATUS_E Ctrl_CheckFirmwareStatus(UINT8 devId)
{
  MXL68x_FW_STATUS_E fwStatus = MXL68x_FW_STATUS_UNKNOWN;
  UINT8 mxlStatus = MXL_SUCCESS;
  UINT16 fwDlStatus = 0;
  UINT16 fwClkStatus = 0;

  // Readk back 0x302B bit<0> to know if firmware has been downloaded 
  mxlStatus |= MxL68x_Ctrl_ReadRegister(devId, FIRMWARE_LOADED_CHECK_REG - 1, &fwDlStatus);
  mxlStatus |= MxL68x_Ctrl_ReadRegister(devId, FIRMWARE_LOADED_CHECK_REG, &fwDlStatus);

  // Readk back 0x300B bit<1> to know if CPU is clock gated
  mxlStatus |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG - 1, &fwClkStatus);
  mxlStatus |= MxL68x_Ctrl_ReadRegister(devId, DMD_GPIO_FUNC_MODE_REG, &fwClkStatus);
  
  // Bit<0> = 1 means firmware has been downloaded successfully 
  if ((mxlStatus == MXL_SUCCESS) && ((fwDlStatus & 0x01) == 0x01) && ((fwClkStatus & 0x02) == 0x00))
  {
    fwStatus = MXL68x_FW_STATUS_LOADED;
  }
  else if ((mxlStatus == MXL_SUCCESS) && ((fwDlStatus & 0x01) == 0x01) && ((fwClkStatus & 0x02) == 0x02))
  {
    fwStatus = MXL68x_FW_STATUS_CLKGATED;
  }
  else if ((mxlStatus == MXL_SUCCESS) && ((fwDlStatus & 0x01) == 0x00) && ((fwClkStatus & 0x02) == 0x00))
  {
    fwStatus = MXL68x_FW_STATUS_BOOTLOADER;
  }
  else
  {
    fwStatus = MXL68x_FW_STATUS_UNKNOWN;  
  }

  return fwStatus;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_WriteRegister
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 04/12/2011
--|
--| DESCRIPTION   : This function implement 16 bit Addr/Data register Write operation,  
--|                 add semaphores mechanism to avoid I2C bus conflict.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxL68x_Ctrl_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 pageNumber;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  if (regAddr < GINJO_DFE_REGISTER_BOUNDARY)  
  {
    if (devContextPtr->firmwareStatus != MXL68x_FW_STATUS_LOADED)
    {
      // Bit<9> and Bit<8> for page number  
      pageNumber = (regAddr & GINJO_DFE_REG_PAGE_MASK) >> GINJO_DFE_REG_WIDTH; 
     
      // change to respective register page bank
      status |= MxLWare68x_OEM_WriteRegister(devId, DFE_PAGE_CTRL_REG, pageNumber);
      regAddr = regAddr & GINJO_DFE_REG_ADDR_MASK; // Bit<7:0> total 8 bit 
    }
    // Program LSB part of register
    status |= MxLWare68x_OEM_WriteRegister(devId, (regAddr), (regData & 0xFF));

    if (regData & 0xFF00)
    {
      // Program MSB part of register
      status |= MxLWare68x_OEM_WriteRegister(devId, ((regAddr) + 1), ((regData & 0xFF00) >> 8));
    }
  }
  else
    status |= MxLWare68x_OEM_WriteRegister(devId, regAddr, regData);

  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_ReadRegister
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 04/12/2008
--|
--| DESCRIPTION   : This function call MxLWare68x_Ctrl_ReadRegister function, complete 
--|                 16 bit addr / data read operation 
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxL68x_Ctrl_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *dataPtr)
{
  UINT16 pageNumber;
  UINT8 status = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  // Means it belongs to DFE 
  if (regAddr < GINJO_DFE_REGISTER_BOUNDARY)  
  {
    if (devContextPtr->firmwareStatus != MXL68x_FW_STATUS_LOADED)
    {
      // Bit<9> and Bit<8> for page number  
      pageNumber = (regAddr & GINJO_DFE_REG_PAGE_MASK) >> GINJO_DFE_REG_WIDTH; 
     
      // change to respective register page bank
      status |= MxLWare68x_OEM_WriteRegister(devId, DFE_PAGE_CTRL_REG, pageNumber);

      regAddr &= GINJO_DFE_REG_ADDR_MASK; 
    }
    status |= MxLWare68x_OEM_ReadRegister(devId, regAddr, dataPtr);
  }
  else 
    status = MxLWare68x_OEM_ReadRegister(devId, regAddr, dataPtr);

  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_AssignNumDenCR
--| 
--| AUTHOR        : Prithvi Thomas
--|
--| DATE CREATED  : 02/24/2012
--|
--| DESCRIPTION   : Used to Map Numerator and Denominator values for 
--|                    for corresponding CodeRates                 
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_AssignNumDenCR(UINT8 *num, UINT8 *den,  UINT8 cr)
{
  UINT8 status = MXL_SUCCESS;

  switch (cr)
  {
    case MXL68x_CR_1_2:
      *num = 1;
      *den = 2;
      break;
      
    case MXL68x_CR_2_3:
      *num = 2;
      *den = 3;      
      break;
      
    case MXL68x_CR_3_4:
      *num = 3;
      *den = 4;      
      break;
      
    case MXL68x_CR_5_6:
      *num = 5;
      *den = 6;      
      break;
      
    case MXL68x_CR_7_8:
      *num = 7;
      *den = 8;      
      break;
      
    default:
      // Assign 1 by default to avoid division by 0
      *num = 1;
      *den = 1;
      status = MXL_FAILURE;
      break;      
  }
  return (MXL_STATUS_E)status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_MulDiv32Bit
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/09/2009
--|
--| DESCRIPTION   : Complete a*b/c calculation (all are 32bit integers) 
--|                 The principle is:
--|                  MulData1 = (b1*2^p1) + r1  
--|                  MulData2 = (b2*2^p2) + r2  
--|                  Result = MulData1 * MulData2 / DivData
--|                  RetVal1 = (b1*2^p1) * (b2*2^p2) / (b3*2^p3) 
--|                  RetVal2 = (b1*2^p1) * r2 / (b3*2^p3) 
--|                  RetVal3 = r1 * (b2*2^p2) / (b3*2^p3) 
--|                  Result = RetVal1 + RetVal2 + RetVal3
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_MulDiv32Bit(UINT32 MulData1, UINT32 MulData2, UINT32 DivData, UINT32* RtnVal)
{
  UINT32 RetVal1,RetVal2,RetVal3; 
  UINT16 b1,b2,b3;
  SINT16 p1,p2,p3;
  UINT16 r1=0,r2=0;

  Ctrl_Decomp32Bit(MulData1,&b1,&p1);
  if (p1 > 0) r1 = (UINT16)MulData1 - (b1 << p1);

  Ctrl_Decomp32Bit(MulData2,&b2,&p2);
  if (p2 > 0) r2 = (UINT16)MulData2 - (b2 << p2);

  Ctrl_Decomp32Bit(DivData,&b3,&p3);
      
  Ctrl_SubMulDiv(b1,b2,b3,p1,p2,p3,&RetVal1);
  Ctrl_SubMulDiv(b1,r2,b3,p1,0,p3,&RetVal2);
  Ctrl_SubMulDiv(r1,b2,b3,0,p2,p3,&RetVal3);

  *RtnVal = RetVal1 + RetVal2 + RetVal3;

  return (MXL_SUCCESS);
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SubMulDiv
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/09/2009
--|
--| DESCRIPTION   : Complete the calculation  
--|                 RetVal = (b1*2^p1) * (b2*2^p2) / (b3*2^p3) 
--|                        = ( b1*b2/b3)*2^(p1+p2-p3) 
--|  
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_SubMulDiv(UINT16 b1,UINT16 b2,UINT16 b3,SINT16 p1,SINT16 p2,SINT16 p3,UINT32 *RetVal)
{
  SINT16 p4;
  UINT32 b4,b5; 

  p4 = p1 + p2 - p3;

  if ((p4 >= 32) && (p3 < 0))
  {
    p4 = p1 + p2; // Because of p3 <0, hence force p3 = 0 
    b5 = b3 >> (-p3);
  }
  else if ((p4 <= -32) && (p3 > 0))
  {
    p4 = p1 + p2; // Because of p3 >0, force p3 = 0 
    b5 = (UINT32)b3 << p3;
  }
  else b5 = b3;
 
  if (p4 > 0) 
  { 
    if (Ctrl_HighBit(b5) > p4)
    {
      b5 = (UINT32)(b5 >> p4);
      *RetVal = (UINT32)b1 * b2 / b5;       
    }
    else 
    {
      b4 = (UINT32)b1 * b2 / b5;
      *RetVal = (UINT32)(b4 << p4);
    }
  }
  else 
  {    
    b4 = (UINT32)b1 * b2 / b5;
    p4 = - p4;
    // Round operation
    if ( Ctrl_HighBit(b4)+1 == p4 && p4 >1 )
      b4 = b4 + (b4 >> 1);

    *RetVal = (UINT32)(b4 >> p4);
   }
   return(MXL_SUCCESS);
}
/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_Decomp32Bit
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/09/2009
--|
--| DESCRIPTION   : Decompose 32 bit unsigned integer into a power(16 bit Signed) 
--|                 and basis (16 bit unsigned integer). In = b*Power(2,p);
--|  
--|---------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_Decomp32Bit(UINT32 In, UINT16* b, SINT16* p)
{
  UINT16 Bas;
  SINT16 Pwr;
  UINT8 k,NonZeroP=31; 
  UINT32 Mask = 0x80000000;
  
   for(k=0;k<32;k++)
   {
     if ((In & Mask) == 0x00) 
     {
       Mask = Mask >> 1;
       NonZeroP = NonZeroP - 1;
     }
     else break;
   }

  if (NonZeroP > 15) 
  { 
    Pwr = NonZeroP - 15;
    Bas = (UINT16)((In >> Pwr) & 0x0000FFFF); 
  }
  else if (NonZeroP <15)
  {
    Pwr =  NonZeroP -15;
    Bas = (UINT16)((In << (15-NonZeroP)) & 0x0000FFFF);
  }
  else 
  { 
    Pwr = 0;
    Bas = (UINT16)In; 
  }

  * b = Bas;
  * p = Pwr;

  return(MXL_SUCCESS);
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_HighBit
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 12/30/2009
--|
--| DESCRIPTION   : Get the first non-zero bit position of input parameter
--|  
--|---------------------------------------------------------------------------*/

UINT8 Ctrl_HighBit(UINT32 In)
{
   UINT8 k,NonZeroP = 31; 
   UINT32 Mask = 0x80000000;
  
   for (k = 0; k < 32; k++)
   {
     if ((In & Mask) == 0x00) 
     {
       Mask = Mask >> 1;
       NonZeroP = NonZeroP - 1;
     }
     else  break;
   }
   return(NonZeroP);
}