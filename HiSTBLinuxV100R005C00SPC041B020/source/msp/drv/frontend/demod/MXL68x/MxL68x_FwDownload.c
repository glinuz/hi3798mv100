/*******************************************************************************
 *
 * FILE NAME          : MxL68x_FwDownload.c
 * 
 * AUTHOR             : Dong Liu
 *
 * DATE CREATED       : 01/13/2012
 *
 * DESCRIPTION        : This file contains functions related to firmware 
 *                      download
 *                             
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#include "MxL68x_FwDownload.h"
#include "MxL68x_PhyCfg.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL_Debug.h"

UINT32 MxL68x_I2cMaxWriteLen = MAX_BLOCK_WRITE_LENGTH;

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_GetBigEndian24
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/20/2009
--|
--| DESCRIPTION   : Convert INT8 buf[0..2] into 24-bit big endian
--|
--| RETURN VALUE  : 24-bit unsigned value
--|-------------------------------------------------------------------------------------*/

UINT32 Ctrl_GetBigEndian24(const UINT8 buf[])
{
  return (((UINT32)buf[0]) << 16) | (((UINT32)buf[1]) << 8) | buf[2];
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SetBigEndian24
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/20/2009
--|
--| DESCRIPTION   : Convert 24-bits val into buf[0..2] in big endian
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

void Ctrl_SetBigEndian24(UINT8 buf[], UINT32 val)
{
  buf[2] = val&0xFF;  val >>= 8;
  buf[1] = val&0xFF;  val >>= 8;
  buf[0] = val&0xFF;

  return;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_GetBigEndian32
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/20/2009
--|
--| DESCRIPTION   : Convert INT8 buf[0..2] into 32-bit big endian
--|
--| RETURN VALUE  : 32-bit unsigned value
--|-------------------------------------------------------------------------------------*/

UINT32 Ctrl_GetBigEndian32(const UINT8 buf[])
{
  return (((UINT32)buf[0]) << 24) | (((UINT32)buf[1]) << 16) | (((UINT32)buf[2]) << 8) | buf[3];
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SetBigEndian32
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 11/20/2009
--|
--| DESCRIPTION   : Convert 32-bits val into buf[0..2] in big endian
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

void Ctrl_SetBigEndian32(UINT8 buf[], UINT32 val)
{
  buf[3] = val&0xFF;  val >>= 8;
  buf[2] = val&0xFF;  val >>= 8;
  buf[1] = val&0xFF;  val >>= 8;
  buf[0] = val&0xFF;

  return;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_CalCheckSum
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 05/06/2010
--|
--| DESCRIPTION   : This function calculate CheckSum of input data array.
--|
--| RETURN VALUE  : CheckSum, check sum of input data array
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_CalCheckSum(UINT8 inDataLen, UINT8 *inDataPtr, UINT8 *checkSum)
{
  UINT16 sum = 0;

  if (inDataLen == 0)
  {
    *checkSum = 0;
    return(MXL_SUCCESS);
  }
  else 
  {
    while (inDataLen--)
      sum += *inDataPtr++;

    *checkSum = (UINT8)(sum & 0xFF);
  }
  
  return(MXL_SUCCESS);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_SendDownloadCommand
--| 
--| AUTHOR        : Jun Huang, Dong Liu, Brenndon Lee
--|
--| DATE CREATED  : 11/30/2009, 12/22/2011, 05/28/2010
--|
--| DESCRIPTION   : Send download command into MxL68x device  
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_SendDownloadCommand(UINT8 devId, MXL_CMD_ID_E commandId, void* dataPtr, UINT32 dataLen, UINT16 downloadBlockCnt)
{
  HOST_COMMAND_T downloadFwSeg;
  FW_RESPONSE_T  responsePtr;
  UINT32 i;
  UINT8 status = MXL_SUCCESS;

  // Check if the sending data length exceeds the limitation 
  if (dataLen > (MxL68x_I2cMaxWriteLen - COMMAND_HEADER_LENGTH))
  {
    MxL_DLL_DEBUG0("Invalid command length (%d)\n", dataLen);
    return MXL_FAILURE;
  }

  switch (commandId)
  {
    case HOST_CMD_SET_ADDR_CFG:
      Ctrl_GetBigEndian32((UINT8*)dataPtr);
      break;

    case HOST_CMD_DOWNLOAD_SEGMENT_CFG:
      break;

    case HOST_CMD_DOWNLOAD_DONE_CFG:
      MxL_DLL_DEBUG0("COMMAND_DOWNLOAD_DONE_CFG (0x%08x)\n", Ctrl_GetBigEndian32((UINT8*)dataPtr));
      break;

    default: break;
  }

  // Populate command header
  FORM_CMD_HEADER(downloadFwSeg, commandId, downloadBlockCnt, dataLen);
  
  // Copy data from dataPtr to downloadFwSeg.data
  for (i = 0; i < dataLen; i++)  // Copy data from dataPtr to downloadFwSeg.data
    downloadFwSeg.data[i] = (UINT8)*((UINT8*)dataPtr+i);

  // Calculate the CheckSum
  status |= Ctrl_CalCheckSum(downloadFwSeg.payloadLen, (UINT8 *)dataPtr, &(downloadFwSeg.payloadCheckSum)); 

  if (commandId == HOST_CMD_DOWNLOAD_DONE_CFG)
  {    
    i = 0;    
    do
    { 
      MxLWare68x_OEM_SleepInUs(5000); //orig 100ms
      status |= Ctrl_GetDeviceResp(devId, HOST_CMD_DOWNLOAD_DONE_CFG, &responsePtr);
    }
    while ((status != MXL_SUCCESS) && (i++ < 20));//orig 5 time
    
    if (status != MXL_SUCCESS)  // Means there is error either in Command transfering or response receiving
    { 
      MxL_DLL_DEBUG0("FW Download Error ! \n");
      //return MXL_FAILURE;
    }
  }
    
  status = Ctrl_SendHostCommand(devId, &downloadFwSeg);

  if (status != MXL_SUCCESS)  // Means there is error either in Command transfering or response receiving
  { 
    i = 0;    
    do
    { 
      MxLWare68x_OEM_SleepInUs(100000);
      status |= Ctrl_SendHostCommand(devId, &downloadFwSeg);
    }
    while ((status != MXL_SUCCESS) && (i++ < 5));

    if (status != MXL_SUCCESS) 
    {
      MxL_DLL_DEBUG0("Calling Ctrl_SendHostCommand error ! \n");
      return MXL_FAILURE;
    }

  }
  //else 
  //  MxL_DLL_DEBUG0("No.%d block is downloaded success. \n", downloadBlockCnt);

  if (commandId == HOST_CMD_DOWNLOAD_DONE_CFG)
  {    
    i = 0;    
    do
    { 
      MxLWare68x_OEM_SleepInUs(100000);
      status |= Ctrl_GetDeviceResp(devId, commandId, &responsePtr);
    }
    while ((status != MXL_SUCCESS) && (i++ < 5));
    
    if (status != MXL_SUCCESS)  // Means there is error either in Command transfering or response receiving
    { 
      MxL_DLL_DEBUG0("FW Download Error ! \n");
      return MXL_FAILURE;
    }
  }

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_DownloadFirmwareSegment
--| 
--| AUTHOR        : Jun Huang
--|               : Dong Liu  
--|
--| DATE CREATED  : 11/30/2009 
--|               : 4/20/2012 
--|
--| DESCRIPTION   : Download segment of MBIN format firmware into MxL68x device, add 2ms 
--|                 delay for MxL68x device 
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_DownloadFirmwareSegment(UINT8 devId, UINT32 address, const UINT8* dataPtr, UINT32 dataLen)
{
  UINT32 nSend;       // Bytes that send to device
  UINT32 segmentLength;  // Save original segment length 
  UINT16 seqNum = 0;  // Record the firmware block sequence number that have been downloaded 
  SINT16 PreviousFwDlPercent; 
  UINT8  payload[4];
  UINT8 status = MXL_SUCCESS;
  MXL_STATUS_E tmpStatus = MXL_SUCCESS;
  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  segmentLength = dataLen;
  PreviousFwDlPercent = devContextPtr->fwDownloadPercent;

  Ctrl_SetBigEndian32(payload, address);

  // Send HOST_CMD_SET_ADDR command firstly
     
  tmpStatus = Ctrl_SendDownloadCommand(devId, HOST_CMD_SET_ADDR_CFG, payload, 4, 0);
  
  if (MXL_SUCCESS == tmpStatus)
  {
    //MxL_DLL_DEBUG0("Ctrl_DownloadFirmwareSegment: HOST_CMD_SET_ADDR_CFG success. Segment data length = %d\n",dataLen);

    while (dataLen)
    {
      // Calculate firmware download percent 
      devContextPtr->fwDownloadPercent = PreviousFwDlPercent +  (UINT16)((segmentLength - dataLen) * 100 / segmentLength);      

      // Send segment data, the block length is nSend
      if (dataLen > (MxL68x_I2cMaxWriteLen - COMMAND_HEADER_LENGTH))
      {
        nSend = MxL68x_I2cMaxWriteLen - COMMAND_HEADER_LENGTH;
      }
      else
      {
        nSend = dataLen;
      }

      tmpStatus = Ctrl_SendDownloadCommand(devId, HOST_CMD_DOWNLOAD_SEGMENT_CFG, (void*)dataPtr, nSend, seqNum);

      if (MXL_SUCCESS == tmpStatus)
      {
        // Sending data pointer is moved to next segment 
        seqNum ++;
        dataPtr += nSend;
        dataLen -= nSend;
        address += nSend;        
      }
    }
  }

  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_DownloadFirmware
--| 
--| AUTHOR        : Jun Huang
--|               : Dong Liu  
--|
--| DATE CREATED  : 11/30/2009 
--|               : 04/20/2012 
--|
--| DESCRIPTION   : Download MBIN format firmware file into device 
--|                 MBIN file format is like below:
--|                 MBIN Header (16 bytes)
--|                 Seg#0 Header (8 bytes)
--|                 Seg#0 Data (Variable length in bytes)
--|                 ...
--|                 Seg#N Header (8 bytes)
--|                 Seg#N Data (Variable length in bytes)
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E Ctrl_DownloadFirmware(UINT8 devId, MBIN_FILE_T* mbinPtr, UINT8 enableRun)
{
  MBIN_SEGMENT_T *segmentPtr ;
  UINT32 index;
  UINT32 dummy0 = 0;
  UINT32 segLength;
  UINT32 segAddress;
  UINT8 status = MXL_SUCCESS; 

  PMXL68x_DEV_CONTEXT_T devContextPtr = MxL68x_Ctrl_GetDeviceContext(devId);

  MxL_DLL_DEBUG0("Header ID = %d, Segments number = %d \n",mbinPtr->header.id, mbinPtr->header.numSegments);

  segmentPtr = (MBIN_SEGMENT_T*)(&mbinPtr->data[0]);

  /* Check for proper file format */
  if (mbinPtr->header.id != MBIN_FILE_HEADER_ID )
  {
    MxL_DLL_DEBUG0("Ctrl_DownloadFirmwareInMbin: Invalid file header ID (%c)\n", mbinPtr->header.id);
    return MXL_FAILURE; // If the file is not MBIN format, then exit with MXL_FAILURE 
  }
  
  /* Process each segment */
  for (index = 0; index < mbinPtr->header.numSegments; index++)
  {

    // Calculate firmware download percent 
    devContextPtr->fwDownloadPercent = (UINT16)(index + 1) * 100 / mbinPtr->header.numSegments;

    /* Validate segment header ID */
    if ( segmentPtr->header.id != MBIN_SEGMENT_HEADER_ID)
    {
      MxL_DLL_DEBUG0("Ctrl_DownloadFirmwareInMbin: Invalid segment header ID (%c)\n",segmentPtr->header.id);
      return MXL_FAILURE; // If the segment format is wrong, then exit with MXL_FAILURE 
    }

    /* Get segment data length and start address */
    segLength  = Ctrl_GetBigEndian24(&(segmentPtr->header.len24[0]));
    segAddress = Ctrl_GetBigEndian32(&(segmentPtr->header.address[0]));

    //MxL_DLL_DEBUG0("Segment%d: length: %02x %02x %02x, address: %02x %02x %02x %02x\n", index,
    //  segmentPtr->header.len24[0], segmentPtr->header.len24[1], segmentPtr->header.len24[2],
    //segmentPtr->header.address[0], segmentPtr->header.address[1], segmentPtr->header.address[2], segmentPtr->header.address[3]);
    
    /* Send segment */
    if (MXL_SUCCESS != Ctrl_DownloadFirmwareSegment(devId, segAddress, segmentPtr->data, segLength))
    {
      HI_ERR_TUNER("---->Ctrl_DownloadFirmwareSegment Failed.\n");
      return MXL_FAILURE;
    }
    else
    {
        HI_INFO_TUNER("---->Ctrl_DownloadFirmwareSegment Success.\n");
        MxLWare68x_OEM_SleepInUs(1000);  //2016.06.16: avoid verify failed when build-in
    }

    /* Next segment */
    segmentPtr = (MBIN_SEGMENT_T*)&(segmentPtr->data[((segLength + 3)/4)*4]);

  }   

  if ((MXL_BOOL_E)enableRun == MXL_ENABLE)  
  { 
    // Run firmware after download
    status |= Ctrl_SendDownloadCommand(devId, HOST_CMD_DOWNLOAD_DONE_CFG, &(mbinPtr->header.entryAddress[0]), 4, 0);
  }
  else 
  {
    // Do not run firmware after download 
    status |= Ctrl_SendDownloadCommand(devId, HOST_CMD_DOWNLOAD_DONE_CFG, &dummy0, 4, 0);
  }

  return (MXL_STATUS_E)status;
}
