/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_XptApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 12/01/2012
 *
 * DESCRIPTION        : Implementation file for Hercules XPT APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"

#ifdef _MXL_HRCLS_XPT_ENABLED_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

// InClockInMHz  = 337.5
// OutClockInMHz = [10.55 21.09 25.96 28.16 42.18 56.21 84.37 112.50]
// ClockDiv      = round(InClockInMHz ./ OutClockInMHz) = [32 16 13 12 8 6 4 3]
//static UINT8 MxL_XptClockDiv[8] = {32, 16, 13, 12, 8, 6, 4, 3};
static const MXL_HRCLS_FIELD32_T ncoCountMin[] = {{NCO_COUNT_MIN0}, {NCO_COUNT_MIN1},
                                             {NCO_COUNT_MIN2}, {NCO_COUNT_MIN3},
                                             {NCO_COUNT_MIN4}, {NCO_COUNT_MIN5},
                                             {NCO_COUNT_MIN6}, {NCO_COUNT_MIN7}};
static const MXL_HRCLS_FIELD32_T enable[] = {{ENABLE_OUTPUT0}, {ENABLE_OUTPUT1}, {ENABLE_OUTPUT2}, {ENABLE_OUTPUT3}, {ENABLE_OUTPUT4}, {ENABLE_OUTPUT5}, {ENABLE_OUTPUT6}, {ENABLE_OUTPUT7}};

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_WriteXptRegister
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Writes 32-bit regData to XPT register at 32-bit regAddr
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_WriteXptRegister(UINT8 devId, UINT32 regAddr, UINT32 regData)
{
  UINT8 status;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regAddr=0x%08x, regData=0x%08x\n", devId, regAddr, regData););
  
  regAddr -= MXL_HRCLS_XPT_BASEADDR;
  status   = MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_L),  (regAddr & 0xFFFF));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_M),  (regAddr >>    16));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_L),  (regData & 0xFFFF));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_M),  (regData >>    16));
  
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadXptRegister
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Read 32-bit regData from XPT register at 32-bit regAddr
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_ReadXptRegister(UINT8 devId, UINT32 regAddr, /*@out@*/ UINT32 *regData)
{
  UINT8  status;
  UINT16 regDataL;
  UINT16 regDataM;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regAddr=0x%08x\n", devId, regAddr););

  regAddr -= MXL_HRCLS_XPT_BASEADDR;
  status   = MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_L),  (regAddr & 0xFFFF));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_M),  (regAddr >>    16));
  if (status == MXL_SUCCESS)
  {
    status  |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_M), &regDataM);
    status  |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_L), &regDataL);
    if (status == MXL_SUCCESS)
    {
      *regData = ((UINT32)regDataM << 16) | (UINT32)regDataL;
    } else *regData = 0;
  }
  
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_WriteXptAddrAutoInc
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/30/2013
--|                  
--| DESCRIPTION   : Writes 32-bit regData to XPT register at 32-bit regAddr
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_WriteXptAddrAutoInc(UINT8 devId, UINT32 regAddr)
{
  UINT8 status;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regAddr=0x%08x\n", devId, regAddr););

  regAddr -= MXL_HRCLS_XPT_BASEADDR;
  status   = MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_L),  (regAddr & 0xFFFF));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_ADDR_M),  (regAddr >>    16));
  
  MXLEXITSTR(status);
  
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_WriteXptDataAutoInc
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/30/2013
--|                  
--| DESCRIPTION   : Writes 32-bit regData to XPT register at 32-bit regAddr
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_WriteXptDataAutoInc(UINT8 devId, UINT32 regData)
{
  UINT8 status;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regData=0x%08x\n", devId, regData););

  status   = MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_L),  (regData & 0xFFFF));
  status  |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(XPT_INDIRC_DATA_M),  (regData >>    16));
  
  MXLEXITSTR(status);
  
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_UpdateXptField
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Update XPT field at 32-bit regAddr to newValue
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateXptField(UINT8 devId, UINT32 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT32 newValue)
{
  UINT8  status;
  UINT32 regData;
  UINT32 newregData;
  UINT32 mask;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regAddr=0x%08x, lsbPos=%d, fieldWidth=%d, newValue=0x%x\n", devId, regAddr, lsbPos, fieldWidth, newValue););

  status = MxL_HRCLS_Ctrl_ReadXptRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    mask = (UINT32) MXL_MASK(lsbPos, fieldWidth);
    newregData = (regData & ~mask) | ((newValue << lsbPos) & mask);

#ifdef MXL_HRCLS_DISCARD_UPDATE_IF_NO_CHANGE
    if (newregData != regData)
#endif 
    {
      status |= MxL_HRCLS_Ctrl_WriteXptRegister(devId, regAddr, newregData); 
    }
  }
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadXptField
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Read XPT field value at 32-bit regAddr
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_ReadXptField(UINT8 devId, UINT32 regAddr, UINT8 lsbPos, UINT8 fieldWidth, /*@out@*/ UINT32 *valuePtr)
{
  UINT8  status;
  UINT32 regData = 0;
  UINT32 mask;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId=%d, regAddr=0x%08x, lsbPos=%d, fieldWidth=%d\n", devId, regAddr, lsbPos, fieldWidth););
  status = MxL_HRCLS_Ctrl_ReadXptRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    mask = (UINT32) MXL_MASK(lsbPos, fieldWidth);
    regData &= mask;
    regData >>= lsbPos;

    if (valuePtr)
    {
      *valuePtr = regData;
    }
  } else status = MXL_FAILURE;

  MXLEXIT(MxL_HRCLS_DEBUG("Value=0x%x\n", (valuePtr != NULL)?*valuePtr:0););
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_Ctrl_ValidateXPTMode
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Validate if XPT mode is supported for given SKU
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_BOOL_E MxLWare_HRCLS_Ctrl_ValidateXPTMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_XPT_MODE_E mode)
{
  MXL_BOOL_E result = MXL_FALSE;
  UINT8 i;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("mode=%d\n", mode););

  if (devContextPtr->xpt.supported == MXL_TRUE)
  {
    i = 0;

    while ((i < devContextPtr->xpt.modesCnt) && (devContextPtr->xpt.modes[i].mode != mode)) i++;
    if (i < devContextPtr->xpt.modesCnt)
    {
      result = MXL_TRUE;
      devContextPtr->xpt.currentMode = &devContextPtr->xpt.modes[i]; 
      devContextPtr->demodsMap = devContextPtr->xpt.currentMode->demodsMap;
      devContextPtr->demodsCnt = devContextPtr->xpt.currentMode->demodsCnt;
      devContextPtr->dfeChanCount = devContextPtr->xpt.currentMode->dfeChanCount;
      devContextPtr->dfeChanMap = devContextPtr->xpt.currentMode->dfeChanMap;
    }
  }
  MXLEXITSTR(result);
  return result;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Get physical outputId from logical outputId
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_HRCLS_XPT_OUTPUT_ID_E MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_XPT_OUTPUT_ID_E outputId)
{
  MXL_HRCLS_XPT_OUTPUT_ID_E physicalOutputId = MXL_HRCLS_XPT_OUT_INVALID;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("outputId=%d\n", outputId););

  if ((devContextPtr->xpt.supported == MXL_TRUE) && ((UINT8) outputId < devContextPtr->xpt.outputsCnt) && (devContextPtr->xpt.currentMode))
  {
    if (devContextPtr->xpt.currentMode->outputMap != NULL) // not parallel-mode
    {
      physicalOutputId = devContextPtr->xpt.currentMode->outputMap[(UINT8) outputId];
    }
  }

  MXLEXITSTR(physicalOutputId);
  return physicalOutputId;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_Ctrl_GetPhysicalXptDemodId
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Get physical demodId from logical demodId
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

static MXL_HRCLS_DMD_ID_E MxLWare_HRCLS_Ctrl_GetPhysicalXptDemodId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E demodId)
{
  MXL_HRCLS_DMD_ID_E physicalDemodId = MXL_HRCLS_MAX_NUM_DEMOD;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("demodId=%d\n", demodId););

  if ((devContextPtr->xpt.supported == MXL_TRUE) && ((UINT8) demodId < devContextPtr->xpt.currentMode->demodsCnt) && (devContextPtr->xpt.currentMode))
  {
    physicalDemodId = devContextPtr->xpt.currentMode->demodsMap[(UINT8) demodId];
  }
  MXLEXITSTR(physicalDemodId);

  return physicalDemodId;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgXpt
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Configure XPT mux mode
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgXpt(
    UINT8   devId,
    MXL_HRCLS_XPT_MODE_E mode)
{
  UINT8 status;
  UINT32 regData;
  UINT32 mask;
  const UINT32 pidMuxMask[5] = {0x0FF0, 0x0550, 0x0110, 0x0110, 0x0110}; 
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("mode=%d\n", mode););
  
  if (devContextPtr->driverInitialized)
  {
    if (devContextPtr->xpt.supported == MXL_TRUE)
    {
      if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, mode) == MXL_TRUE)
      {
        HOST_COMMAND_T xptCfg;
        // new mode pointer copied to devContextPtr->xpt.currentMode

        xptCfg.payloadLen = 0;
        xptCfg.payloadCheckSum = 0;
        xptCfg.syncChar = 0;
        xptCfg.commandId = 0;

        // Initialize all PIDs to 0. Host-command also disables all outputs
        status = MxL_HRCLS_Ctrl_SendHostCommand(devContextPtr->devId, &xptCfg, HOST_CMD_XPT_INITPIDS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        MxLWare_HRCLS_OEM_DelayUsec(10*1000); // tbd: profile timinig

        // Update the PID hardware-context mask for new muxmode
        {
          // Slice0
          mask = EXTRACT_MASK32(PID_MUX_SELECT0) | EXTRACT_MASK32(KNOWN_PID_MUX_SELECT0);
          status |= MxL_HRCLS_Ctrl_ReadXptRegister(devId, EXTRACT_ADDR32(PID_MUX_SELECT0), &regData);
          if (status == MXL_SUCCESS)
          {
            if (regData & mask) // if hardware using bank-1
            {
              regData &= ~mask;
              regData |=  pidMuxMask[devContextPtr->xpt.currentMode->xptMap]; 
            }
            status |= MxL_HRCLS_Ctrl_WriteXptRegister(devId, EXTRACT_ADDR32(PID_MUX_SELECT0), regData);		  
          }

          // Slice1
          mask = EXTRACT_MASK32(PID_MUX_SELECT1) | EXTRACT_MASK32(KNOWN_PID_MUX_SELECT1);
          status |= MxL_HRCLS_Ctrl_ReadXptRegister(devId, EXTRACT_ADDR32(PID_MUX_SELECT1), &regData);
          if (status == MXL_SUCCESS)
          {
            if (regData & mask) // if hardware using bank-1
            {
              regData &= ~mask;
              regData |=  pidMuxMask[devContextPtr->xpt.currentMode->xptMap]; 
            }
            status |= MxL_HRCLS_Ctrl_WriteXptRegister(devId, EXTRACT_ADDR32(PID_MUX_SELECT1), regData);		  
          }
        }

        // Enable mux settings
        switch (devContextPtr->xpt.currentMode->xptMap)
        {
          case MXL_HRCLS_XPT_MAP_1_TO_1:
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE0, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE1, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, ENABLE_PARALLEL_OUTPUT, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, TS_CLK_OUT_EN_PARALLEL, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, MODE_27MHZ, 0);
            MXLDBG2(MxL_HRCLS_DEBUG("XPT config: 1-to-1 mode\n"););
            break;

          case MXL_HRCLS_XPT_MAP_2_TO_1:
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE0, 2);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE1, 2);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, ENABLE_PARALLEL_OUTPUT, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, TS_CLK_OUT_EN_PARALLEL, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, MODE_27MHZ, 0);
            MXLDBG2(MxL_HRCLS_DEBUG("XPT config: 2-to-1 mode\n"););
            break;

          case MXL_HRCLS_XPT_MAP_4_TO_1:
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE0, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE1, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, ENABLE_PARALLEL_OUTPUT, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, TS_CLK_OUT_EN_PARALLEL, 0);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, MODE_27MHZ, 0);
            MXLDBG2(MxL_HRCLS_DEBUG("XPT config: 4-to-1 mode\n"););
            break;

          case MXL_HRCLS_XPT_MAP_PARALLEL:
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE0, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE1, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, ENABLE_PARALLEL_OUTPUT, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, TS_CLK_OUT_EN_PARALLEL, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, MODE_27MHZ, 0);
            MXLDBG2(MxL_HRCLS_DEBUG("XPT config: parallel mode\n"););
            break;

          case MXL_HRCLS_XPT_MAP_CABLECARD:
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE0, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, STREAM_MUXMODE1, 3);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, ENABLE_PARALLEL_OUTPUT, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, TS_CLK_OUT_EN_PARALLEL, 1);
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, MODE_27MHZ, 1);
            MXLDBG2(MxL_HRCLS_DEBUG("XPT config: cable card mode\n"););
            break;

          case MXL_HRCLS_XPT_MAP_INVALID:
          default:
            status = MXL_NOT_SUPPORTED;
            break;
        }
        if (status == MXL_SUCCESS)
        {
          HOST_COMMAND_T setupSchemeCmd;

          setupSchemeCmd.data[0] = devContextPtr->xpt.currentMode->demodScheme;
          setupSchemeCmd.payloadLen = 1;
          setupSchemeCmd.payloadCheckSum = 0;
          setupSchemeCmd.syncChar = 0;
          setupSchemeCmd.commandId = 0;
    
          status = MxL_HRCLS_Ctrl_SendHostCommand(devContextPtr->devId, &setupSchemeCmd, HOST_CMD_XPT_SET_SCHEME, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
        }
      } else status = MXL_INVALID_PARAMETER;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_NOT_INITIALIZED;

  MXLEXITAPISTR(devId, status);

  return (MXL_STATUS_E)status;
}

static UINT16 MxL_HRCLS_Ctrl_GetPadDrvValue(MXL_HRCLS_MPEG_DRV_MODE_E padDrv)
{
  UINT16 driveVal = 0;
  
  switch (padDrv)
  {
    case MXL_HRCLS_MPEG_DRV_MODE_1X: driveVal = 0; break;
    default:
    case MXL_HRCLS_MPEG_DRV_MODE_2X: driveVal = 1; break;
    case MXL_HRCLS_MPEG_DRV_MODE_3X: driveVal = 2; break;
    case MXL_HRCLS_MPEG_DRV_MODE_4X: driveVal = 3; break;
    case MXL_HRCLS_MPEG_DRV_MODE_5X: driveVal = 4; break;
    case MXL_HRCLS_MPEG_DRV_MODE_6X: driveVal = 5; break;
    case MXL_HRCLS_MPEG_DRV_MODE_7X: driveVal = 6; break;
    case MXL_HRCLS_MPEG_DRV_MODE_8X: driveVal = 7; break;
  }
  return driveVal;
}

static UINT8 MxL_HRCLS_Ctrl_XptGetClockDivider(MXL_HRCLS_MPEG_CLK_RATE_E clkFreq)
{
  UINT8 divider;
  
  switch (clkFreq)
  {   
    case MXL_HRCLS_MPEG_CLK_10_55MHz: divider = 32; break;
    case MXL_HRCLS_MPEG_CLK_21_09MHz: divider = 16; break;
    case MXL_HRCLS_MPEG_CLK_25_96MHz: divider = MXL_HRCLS_XPT_CLKDIV_27_00MHz; break;
    case MXL_HRCLS_MPEG_CLK_28_16MHz: divider = 12; break;
    case MXL_HRCLS_MPEG_CLK_42_18MHz: divider = 8; break;
    case MXL_HRCLS_MPEG_CLK_56_21MHz: divider = 6; break;
    case MXL_HRCLS_MPEG_CLK_84_37MHz: divider = 4; break;
    default:
    case MXL_HRCLS_MPEG_CLK_112_50MHz: divider = 3; break;
  }

  return divider;
}

MXL_STATUS_E MxLWare_Ctrl_ConfigureCommonClock(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_MPEG_CLK_RATE_E mpegClkFreq, MXL_HRCLS_MPEG_CLK_FMT_E mpegClkPol, MXL_HRCLS_MPEG_DRV_MODE_E mpegClkPadDrv)
{
  UINT8 status = (UINT8) MXL_SUCCESS;
  UINT16 eco_value = 0;
  UINT32 regData;
  UINT8 devId;
  UINT8 clockDivider = MxL_HRCLS_Ctrl_XptGetClockDivider(mpegClkFreq);
  UINT8 i;
  MXL_HRCLS_XPT_OUTPUT_ID_E physicalOutputId = MXL_HRCLS_XPT_OUT_INVALID;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("mpegClkFreq=%d, mpegClkPol=%d, mpegClkPadDrv=%d\n", mpegClkFreq, mpegClkPol, mpegClkPadDrv););
  devId = devContextPtr->devId;

  // Select output clock source - first valid (as per mux-mode) outputId 
  for (i = 0; (i < devContextPtr->xpt.outputsCnt) && (status == MXL_SUCCESS); i++)
  {
    physicalOutputId = MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId(devContextPtr, (MXL_HRCLS_XPT_OUTPUT_ID_E)i); 
    if (physicalOutputId !=  MXL_HRCLS_XPT_OUT_INVALID)
    {
      devContextPtr->xpt.clkSrcOutputId = (MXL_HRCLS_XPT_OUTPUT_ID_E)i;
      break;
    }
  }

  if (physicalOutputId !=  MXL_HRCLS_XPT_OUT_INVALID)
  {
    // ECO_8[7] Set MPEG_CLK polarity using 
    status |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_8), &eco_value);
    eco_value &= ~(1U<<7);
    eco_value |= (mpegClkPol == MXL_HRCLS_MPEG_CLK_POSITIVE)?(0):(1U<<7);
    status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_8), eco_value);

    // Update CLK pad drive strength
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, PAD_MPEG_CLK_DRV, MxL_HRCLS_Ctrl_GetPadDrvValue(mpegClkPadDrv));

    // Set clock frequency for clock source
    status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, 
        ncoCountMin[(UINT8)physicalOutputId].regAddr, 
        ncoCountMin[(UINT8)physicalOutputId].lsbPos, 
        ncoCountMin[(UINT8)physicalOutputId].fieldWidth,
        clockDivider);

    // Enable TS ouput for clock source
    status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, enable[(UINT8) physicalOutputId].regAddr,
        enable[(UINT8) physicalOutputId].lsbPos,
        enable[(UINT8) physicalOutputId].fieldWidth, 1);

    // ECO_4[3:2] used to select clock source (fix to 0 = TS2)
    status |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_4), &eco_value);
    if (status == MXL_SUCCESS)
    {
      eco_value &= ~((1U<<2) | (1U<<3));
      eco_value |= ((devContextPtr->xpt.clkSrcOutputId & 0x03) << 2);
      status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4), (UINT16)eco_value);
    }

    if (status == MXL_SUCCESS)
    {
      status = MxL_HRCLS_Ctrl_UpdateXptField(devId, XPT_DELAY_SEL_MDAT_6, 4);
      status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, XPT_DELAY_SEL_MPVAL_6, 4);
      status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, XPT_DELAY_SEL_MPSYNC_6, 4);
    }

    // Enable 4-wire mode for all valid outputs
    for (i = 0; (i < devContextPtr->xpt.outputsCnt) && (status == MXL_SUCCESS); i++)
    {
      physicalOutputId = MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId(devContextPtr, (MXL_HRCLS_XPT_OUTPUT_ID_E)i); 
      if (physicalOutputId !=  MXL_HRCLS_XPT_OUT_INVALID)
      {
        status = MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_4), &eco_value);
        if (status == MXL_SUCCESS)
        {
          // According to PG, TS2 -> ECO_4[4]; TS3 -> ECO_4[5]
          eco_value &= ~(1U << (physicalOutputId + 2));
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4) , (UINT16)eco_value);
          eco_value |=  (1U << (physicalOutputId + 2));
          status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4) , (UINT16)eco_value);
        }

        // Enable TS_CLK_OUT_EN for all TS (sets correct VAL signal + continuous clock for the TS out, needed for 4-wire mode) 
        status |= MxL_HRCLS_Ctrl_ReadXptRegister(devId, EXTRACT_ADDR32(TS_CLK_OUT_EN0), &regData);
        regData |= (1UL << physicalOutputId);
        status |= MxL_HRCLS_Ctrl_WriteXptRegister(devId, EXTRACT_ADDR32(TS_CLK_OUT_EN0), regData);
      }
    }

    devContextPtr->xpt.commonClkFreq = mpegClkFreq;
  } else status = MXL_FAILURE; 

  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgXptOutput
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Configure XPT output parameters for given outputId
--|                 Called after MxLWare_HRCLS_API_CfgXpt
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgXptOutput(
    UINT8   devId,
    MXL_HRCLS_XPT_OUTPUT_ID_E outputId,
    MXL_HRCLS_XPT_MPEGOUT_PARAM_T * outMpegParams)
{
  UINT8  i;
  UINT8  cond1;
  UINT8  cond2;
  UINT8  cond3;
  UINT32 regData;
  UINT16 eco_value;
  MXL_HRCLS_XPT_OUTPUT_ID_E physicalOutputId;
  MXL_BOOL_E mpegClk;
  UINT32 clockVal;
  UINT8  status = MXL_SUCCESS;
  
  const MXL_HRCLS_FIELD32_T lsbOrMsbFirst[] = {{LSB_FIRST0}, {LSB_FIRST1}, {LSB_FIRST2}, {LSB_FIRST3}, {LSB_FIRST4}, {LSB_FIRST5}, {LSB_FIRST6}, {LSB_FIRST7}};
  const MXL_HRCLS_FIELD32_T mpegSyncPulseWidth[] = {{SYNC_FULL_BYTE0}, {SYNC_FULL_BYTE1}, {SYNC_FULL_BYTE2}, {SYNC_FULL_BYTE3}, {SYNC_FULL_BYTE4}, {SYNC_FULL_BYTE5}, {SYNC_FULL_BYTE6}, {SYNC_FULL_BYTE7}};
  const MXL_HRCLS_FIELD32_T mpegValidPol[] = {{VALID_POLARITY0}, {VALID_POLARITY1}, {VALID_POLARITY2}, {VALID_POLARITY3}, {VALID_POLARITY4}, {VALID_POLARITY5}, {VALID_POLARITY6}, {VALID_POLARITY7}};
  const MXL_HRCLS_FIELD32_T mpegSyncPol[] = {{SYNC_POLARITY0}, {SYNC_POLARITY1}, {SYNC_POLARITY2}, {SYNC_POLARITY3}, {SYNC_POLARITY4}, {SYNC_POLARITY5}, {SYNC_POLARITY6}, {SYNC_POLARITY7}};
  const MXL_HRCLS_FIELD32_T mpegClkPol[] = {{CLOCK_POLARITY0}, {CLOCK_POLARITY1}, {CLOCK_POLARITY2}, {CLOCK_POLARITY3}, {CLOCK_POLARITY4}, {CLOCK_POLARITY5}, {CLOCK_POLARITY6}, {CLOCK_POLARITY7}};
  const MXL_HRCLS_FIELD_T   mpeg_oe[] = {{MPEG0_OE}, {MPEG1_OE}, {MPEG2_OE}, {MPEG3_OE}, {MPEG4_OE}, {MPEG5_OE}, {MPEG6_OE}, {MPEG7_OE}, {MPEG8_OE}};


  const MXL_HRCLS_FIELD_T padSynDrv[] = {{PAD_MPEG_1_SYN_DRV}, {PAD_MPEG_2_SYN_DRV},
                                           {PAD_MPEG_3_SYN_DRV}, {PAD_MPEG_4_SYN_DRV},
                                           {PAD_MPEG_5_SYN_DRV}, {PAD_MPEG_6_SYN_DRV},
                                           {PAD_MPEG_7_SYN_DRV}, {PAD_MPEG_8_SYN_DRV}};
  const MXL_HRCLS_FIELD_T padValDrv[] = {{PAD_MPEG_1_VAL_DRV}, {PAD_MPEG_2_VAL_DRV},
                                           {PAD_MPEG_3_VAL_DRV}, {PAD_MPEG_4_VAL_DRV},
                                           {PAD_MPEG_5_VAL_DRV}, {PAD_MPEG_6_VAL_DRV},
                                           {PAD_MPEG_7_VAL_DRV}, {PAD_MPEG_8_VAL_DRV}};
  const MXL_HRCLS_FIELD_T padDatDrv[] = {{PAD_MPEG_1_DAT_DRV}, {PAD_MPEG_2_DAT_DRV},
                                           {PAD_MPEG_3_DAT_DRV}, {PAD_MPEG_4_DAT_DRV},
                                           {PAD_MPEG_5_DAT_DRV}, {PAD_MPEG_6_DAT_DRV},
                                           {PAD_MPEG_7_DAT_DRV}, {PAD_MPEG_8_DAT_DRV}};

#define MXL_HRCLS_PARALLEL_PAD_DRV_COUNT 12
  const MXL_HRCLS_FIELD_T parallelPadDrv[MXL_HRCLS_PARALLEL_PAD_DRV_COUNT] = 
                                             {{PAD_MPEG_3_SYN_DRV}, {PAD_MPEG_3_DAT_DRV}, {PAD_MPEG_3_VAL_DRV}, 
                                              {PAD_MPEG_4_SYN_DRV}, {PAD_MPEG_4_DAT_DRV}, {PAD_MPEG_4_VAL_DRV}, 
                                              {PAD_MPEG_5_SYN_DRV}, {PAD_MPEG_5_DAT_DRV},
                                              {PAD_MPEG_6_SYN_DRV}, {PAD_MPEG_6_DAT_DRV}, {PAD_MPEG_6_VAL_DRV},
                                              {PAD_MPEG_CLK_DRV}};
                                                                                        
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("outputId=%d, enable=%d, lsbOrMsbFirst=%d, mpegSyncPulseWidth=%d, mpegValidPol=%d, mpegSyncPol=%d, mpegClkPol=%d, clkFreq=%d, SYN pad drv=%d, VAL pad drv=%d, DAT pad drv=%d\n", 
                               outputId, 
                               outMpegParams->enable,
                               outMpegParams->lsbOrMsbFirst,
                               outMpegParams->mpegSyncPulseWidth,
                               outMpegParams->mpegValidPol,
                               outMpegParams->mpegSyncPol,
                               outMpegParams->mpegClkPol,
                               outMpegParams->clkFreq,
                               outMpegParams->mpegPadDrv.padDrvMpegSyn,
                               outMpegParams->mpegPadDrv.padDrvMpegVal,
                               outMpegParams->mpegPadDrv.padDrvMpegDat););

  if (devContextPtr->driverInitialized)
  {    
    if (devContextPtr->xpt.supported == MXL_TRUE)
    {
      if (devContextPtr->xpt.currentMode)
      {
        UINT8 clockDivider = MxL_HRCLS_Ctrl_XptGetClockDivider(outMpegParams->clkFreq);
        physicalOutputId = MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId(devContextPtr, outputId); 

        cond1 =  (physicalOutputId != MXL_HRCLS_XPT_OUT_INVALID)
              || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD) 
              || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL);       // valid physicalOutputId for any serial mode
        
        cond2 =  (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD) 
              && (outMpegParams->clkFreq != MXL_HRCLS_MPEG_CLK_25_96MHz);  // invalid clock for cable-card mode 

        // cond3 - check if Hercules_V3. Then if the requested frequency is the same as already set
        cond3 = ((devContextPtr->xpt.commonClockEnabled == MXL_FALSE) || (outMpegParams->enable == MXL_DISABLE) || (outMpegParams->clkFreq == devContextPtr->xpt.commonClkFreq));
        MXLERR(
          if (cond3 == 0)
          {
            MxL_HRCLS_ERROR("[HRCLS] Common clock frequency is already set to (%d). It cannot be changed while at least one output is on.\n", devContextPtr->xpt.commonClkFreq);
          }
          ) 

        // Confirm valid parameters
        if ((cond1) && (!cond2) && (cond3))
        {
          status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, lsbOrMsbFirst[(UINT8) physicalOutputId].regAddr,
                                                         lsbOrMsbFirst[(UINT8) physicalOutputId].lsbPos,
                                                         lsbOrMsbFirst[(UINT8) physicalOutputId].fieldWidth, (outMpegParams->lsbOrMsbFirst == MXL_HRCLS_MPEG_SERIAL_LSB_1ST)?1:0);
          
          status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, mpegSyncPulseWidth[(UINT8) physicalOutputId].regAddr,
                                                         mpegSyncPulseWidth[(UINT8) physicalOutputId].lsbPos,
                                                         mpegSyncPulseWidth[(UINT8) physicalOutputId].fieldWidth, (outMpegParams->mpegSyncPulseWidth == MXL_HRCLS_MPEG_SYNC_WIDTH_BYTE)?1:0);
          
          status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, mpegValidPol[(UINT8) physicalOutputId].regAddr,
                                                         mpegValidPol[(UINT8) physicalOutputId].lsbPos,
                                                         mpegValidPol[(UINT8) physicalOutputId].fieldWidth, (outMpegParams->mpegValidPol == MXL_HRCLS_MPEG_ACTIVE_HIGH)?1:0);

          status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, mpegSyncPol[(UINT8) physicalOutputId].regAddr,
                                                         mpegSyncPol[(UINT8) physicalOutputId].lsbPos,
                                                         mpegSyncPol[(UINT8) physicalOutputId].fieldWidth, (outMpegParams->mpegSyncPol == MXL_HRCLS_MPEG_ACTIVE_HIGH)?1:0);

          if (devContextPtr->xpt.commonClockEnabled == MXL_FALSE)
          {
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, mpegClkPol[(UINT8) physicalOutputId].regAddr,
                                                           mpegClkPol[(UINT8) physicalOutputId].lsbPos,
                                                           mpegClkPol[(UINT8) physicalOutputId].fieldWidth, ((outMpegParams->mpegClkPol == MXL_HRCLS_MPEG_CLK_POSITIVE) && (devContextPtr->xpt.currentMode->mode != MXL_HRCLS_XPT_MODE_CABLECARD))?1:0);
          }

          // Configure xpto clock polarity, xpto sync polarity, xpto valid polarity
          if   ((devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL)
             || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD))
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_VALID_POLARITY, (MXL_HRCLS_MPEG_ACTIVE_LOW   == outMpegParams->mpegValidPol)?0:1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_SYNC_POLARITY,  (MXL_HRCLS_MPEG_ACTIVE_LOW   == outMpegParams->mpegSyncPol)?0:1);
            regData  = (MXL_HRCLS_MPEG_CLK_POSITIVE != outMpegParams->mpegClkPol)?(0xFF):(0x00);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_CLK_POLARITY, regData);
          }
          else // serial mode
          {
            if (devContextPtr->xpt.commonClockEnabled == MXL_FALSE)
            {
              status  |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, XPTO_CLK_POLARITY,   (UINT16*)&regData);
              if (MXL_HRCLS_MPEG_CLK_POSITIVE != outMpegParams->mpegClkPol)
              {
                regData |=  (1UL << physicalOutputId);
              }
              else // (MXL_HRCLS_MPEG_CLK_POSITIVE != outMpegParams->mpegClkPol)
              {
                regData &= ~(1UL << physicalOutputId);
              }
              if (status == MXL_SUCCESS)
              {
                status = MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_CLK_POLARITY, regData);
              }
            }
          }

          // Configure xpto ts mode, xpto fifo byp
          if (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPT_TS_MODE, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_FIFO_BYP, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_FIFO_RST, 1);
          }
          else if (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPT_TS_MODE, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_FIFO_RST, 0);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_FIFO_BYP, 0);
          }
          else // serial mode
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPT_TS_MODE, 0);
          }


          // Configure nco clock divider
          status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, 
                                                  ncoCountMin[(UINT8)physicalOutputId].regAddr, 
                                                  ncoCountMin[(UINT8)physicalOutputId].lsbPos, 
                                                  ncoCountMin[(UINT8)physicalOutputId].fieldWidth,
                                                  clockDivider);
          
          // Configure pad strength
          if   ((devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL)
             || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD))
          {
            for (i = 0; (i < MXL_HRCLS_PARALLEL_PAD_DRV_COUNT) && (status == MXL_SUCCESS); i++)
            {
              status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, parallelPadDrv[i].regAddr,
                                                                  parallelPadDrv[i].lsbPos,
                                                                  parallelPadDrv[i].fieldWidth,
                                                                  MxL_HRCLS_Ctrl_GetPadDrvValue(outMpegParams->mpegPadDrv.padDrvMpegSyn));
            }
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, padSynDrv[(UINT8) physicalOutputId].regAddr,
                                                                padSynDrv[(UINT8) physicalOutputId].lsbPos,
                                                                padSynDrv[(UINT8) physicalOutputId].fieldWidth,
                                                                MxL_HRCLS_Ctrl_GetPadDrvValue(outMpegParams->mpegPadDrv.padDrvMpegSyn));
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, padValDrv[(UINT8) physicalOutputId].regAddr,
                                                                padValDrv[(UINT8) physicalOutputId].lsbPos,
                                                                padValDrv[(UINT8) physicalOutputId].fieldWidth,
                                                                MxL_HRCLS_Ctrl_GetPadDrvValue(outMpegParams->mpegPadDrv.padDrvMpegVal));
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, padDatDrv[(UINT8) physicalOutputId].regAddr,
                                                                padDatDrv[(UINT8) physicalOutputId].lsbPos,
                                                                padDatDrv[(UINT8) physicalOutputId].fieldWidth,
                                                                MxL_HRCLS_Ctrl_GetPadDrvValue(outMpegParams->mpegPadDrv.padDrvMpegDat));
          }

          // Configure xpto clock extend
          if   ((devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL)
             || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD))
          {
            regData = (clockDivider & 0x1)?0xff:0x00;
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_CLOCK_EXTEND, regData);
          }
          else // serial mode
          {
            if (devContextPtr->xpt.commonClockEnabled == MXL_FALSE)
            {
              status  |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, XPTO_CLOCK_EXTEND, (UINT16*)&regData);

              if (clockDivider & 0x1) 
              {
                regData |=  (1U << physicalOutputId); // odd frequency divider,  clock extend = 1
              }
              else
              {
                regData &= ~(1U << physicalOutputId); // even frequency divider, clock extend = 0
              }
              if (status == MXL_SUCCESS)
              {
                status = MxL_HRCLS_Ctrl_UpdateRegisterField(devId, XPTO_CLOCK_EXTEND, regData);
              }
            }
          }

          // Configure topad xtal clk inv
          if (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, TOPAD_XTAL_CLK_INV, (MXL_HRCLS_MPEG_CLK_POSITIVE == outMpegParams->mpegClkPol));
          }

          // Enable/disable 27MHz clock for cablecard mode
          if (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD)
          {
            status |= MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal);
            if (MXL_SUCCESS == status)
            {
              if (outMpegParams->enable == MXL_ENABLE)
              {
                clockVal |= (1UL<<14);  // tsp0_clk
              }
              else
              {
                clockVal &= ~(1UL<<14); // tsp0_clk
              }
              status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
            }
          }

          // Enable/disable output
          if ((devContextPtr->xpt.commonClockEnabled == MXL_FALSE) || 
              (MxLWare_HRCLS_Ctrl_GetPhysicalXptOutputId(devContextPtr, devContextPtr->xpt.clkSrcOutputId) != physicalOutputId) || 
              (outMpegParams->enable == MXL_ENABLE))
          {
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, enable[(UINT8) physicalOutputId].regAddr,
                enable[(UINT8) physicalOutputId].lsbPos,
                enable[(UINT8) physicalOutputId].fieldWidth, (outMpegParams->enable == MXL_ENABLE)?1:0);
          }
          
          // Enable/disable pads
          if ((devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_PARALLEL)
             || (devContextPtr->xpt.currentMode->mode == MXL_HRCLS_XPT_MODE_CABLECARD))
          {
            // Parallel mode - mpeg3_oe, mpeg4_oe, mpeg5_oe, mpeg6_oe
            for (i = 3; i <= 6; i++)
            {
              status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, mpeg_oe[i].regAddr,
                  mpeg_oe[i].lsbPos,
                  mpeg_oe[i].fieldWidth,
                  (outMpegParams->enable == MXL_ENABLE)?1:0);
            }
          }
          else
          {
            // Serial mode - physical xpt-output0->mpeg1_oe, physical xpt-output1->mpeg2_oe
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, mpeg_oe[((UINT8)physicalOutputId) + 1].regAddr,
                mpeg_oe[((UINT8)physicalOutputId) + 1].lsbPos,
                mpeg_oe[((UINT8)physicalOutputId) + 1].fieldWidth,
                (outMpegParams->enable == MXL_ENABLE)?1:0);
          }
		  
          // MPEG clock pad: Disable- if all MPEG outputs disabled, Enable- if one or more MPEG outputs enabled
          mpegClk = MXL_DISABLE;
          for (i = 0; i <= 8; i++)
          {
            regData = 0;
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devContextPtr->devId, mpeg_oe[i].regAddr,
                mpeg_oe[i].lsbPos,
                mpeg_oe[i].fieldWidth,
                (UINT16*)&regData);
            if ((MXL_SUCCESS == status) && (1 == regData))
            {
              mpegClk = MXL_ENABLE;
              break;
            }
          }
          if (MXL_ENABLE == mpegClk)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, MCK_OE, 1);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devContextPtr->devId, MCK_OE, 0);
          }

          if ((status == MXL_SUCCESS) && (devContextPtr->xpt.commonClockEnabled == MXL_TRUE))
          {
            status = MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_4), &eco_value);
            if (status == MXL_SUCCESS)
            {
              // According to PG, TS2 -> ECO_4[4]; TS3 -> ECO_4[5]
              eco_value &= ~(1U << (physicalOutputId + 2));
              status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4) , (UINT16)eco_value);
              eco_value |=  (1U << (physicalOutputId + 2));
              status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_4) , (UINT16)eco_value);
            }
          }
        } else status = MXL_INVALID_PARAMETER;
      } else status = MXL_NOT_READY;
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_NOT_INITIALIZED;

  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgXptPidTable
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/25/2013
--|                  
--| DESCRIPTION   : Configure PIDs for a given input stream
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgXptPidTable(
    UINT8   devId,
    MXL_HRCLS_DMD_ID_E demodId,
    UINT8   pidCnt,
    MXL_HRCLS_XPT_PID_ENTRY_T* pidsPtr,
    MXL_BOOL_E defaultDrop)
{
  UINT8  status;
  UINT8  i;
  UINT32 regData;
  UINT32 regData2;
  UINT32 regAddr;
  UINT32 maskValid; 
  UINT32 maskDrop; 
  UINT32 maskRemap; 
  UINT32 maskValue; 
  UINT32 maskPidMask; 
  UINT32 maskRemapValue; 
  UINT32 maskPortId;
  UINT32 portId;
  const UINT32 pidMuxMask[4] = {0x0000, 0x0FF0, 0x0550, 0x0110}; 
  MXL_HRCLS_DMD_ID_E physicalDemodId;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(
    {
      MxL_HRCLS_DEBUG("demodId=%d, pidCnt=%d\n", demodId, pidCnt);
      for (i = 0; i < pidCnt; i++) MxL_HRCLS_DEBUG("%d. %5d -> %5d\n", i, pidsPtr[i].pidValue, pidsPtr[i].pidRemapValue);
    }
             );

  if (devContextPtr->driverInitialized)
  {    
    if (devContextPtr->xpt.supported == MXL_TRUE)
    {
      if (devContextPtr->xpt.currentMode)
      {
        if ((demodId < devContextPtr->xpt.currentMode->demodsCnt) && (pidCnt < MXL_HRCLS_XPT_MAX_PIDS))
        {
          physicalDemodId = MxLWare_HRCLS_Ctrl_GetPhysicalXptDemodId(devContextPtr, demodId); 

          // Configure all regular PIDs
          {
            maskValid      = EXTRACT_MASK32(PID_VALID0); 
            maskDrop       = EXTRACT_MASK32(PID_DROP0); 
            maskRemap      = EXTRACT_MASK32(PID_REMAP0); 
            maskValue      = EXTRACT_MASK32(PID_VALUE0); 
            maskPidMask    = EXTRACT_MASK32(PID_MASK0); 
            maskRemapValue = EXTRACT_MASK32(PID_REMAP_VALUE0); 
            maskPortId     = EXTRACT_MASK32(PID_PORT_ID0);
            
            if (physicalDemodId <= MXL_HRCLS_DEMOD4)
            {
              regAddr = EXTRACT_ADDR32(PID_VALID0) + (physicalDemodId-MXL_HRCLS_DEMOD1) * 0x200;
            }
            else // (physicalDemodId >= MXL_HRCLS_DEMOD5)
            {
              regAddr = EXTRACT_ADDR32(PID_VALID1) + (physicalDemodId-MXL_HRCLS_DEMOD5) * 0x200;
            }
            portId = physicalDemodId - MXL_HRCLS_DEMOD1;

            // Write regular PID values (use auto increment)
            status = MxL_HRCLS_Ctrl_WriteXptAddrAutoInc(devId, regAddr);
            for (i = 0; (i < pidCnt) && (status == MXL_SUCCESS); i++)
            {
#ifdef _MXL_HRCLS_XPT_KNOWN_PIDS_ENABLED_
              if (pidsPtr[i].pidValue > MXL_HRCLS_XPT_MAX_KNOWN_PID_VALUE) // regular PID
#endif // _MXL_HRCLS_XPT_KNOWN_PIDS_ENABLED_                
              {
                regData = (maskValid                                                                                                    )
                        | (maskDrop    & (((MXL_TRUE == pidsPtr[i].pidDrop)?(1UL):(0UL))                << EXTRACT_LSBLOC32(PID_DROP0 )))
                        | (maskRemap   & (((pidsPtr[i].pidValue==pidsPtr[i].pidRemapValue)?(0UL):(1UL)) << EXTRACT_LSBLOC32(PID_REMAP0)))
                        | (maskValue   & (pidsPtr[i].pidValue                                           << EXTRACT_LSBLOC32(PID_VALUE0)))
                        | (maskPidMask & (0x1FFFUL                                                      << EXTRACT_LSBLOC32(PID_MASK0 )))
                        ;
                status |= MxL_HRCLS_Ctrl_WriteXptDataAutoInc(devId, regData);

                regData = (maskRemapValue & (((pidsPtr[i].pidValue==pidsPtr[i].pidRemapValue)?(0UL):(pidsPtr[i].pidRemapValue)) << EXTRACT_LSBLOC32(PID_REMAP_VALUE0)))
                        | (maskPortId     & ((portId)                                                                           << EXTRACT_LSBLOC32(PID_PORT_ID0)));
                status |= MxL_HRCLS_Ctrl_WriteXptDataAutoInc(devId, regData);
              }
            } 
          }

          // Configure all known PIDs
#ifdef _MXL_HRCLS_XPT_KNOWN_PIDS_ENABLED_
          {
            maskValid      = EXTRACT_MASK32(KNOWN_PID_VALID0); 
            maskDrop       = EXTRACT_MASK32(KNOWN_PID_DROP0); 
            maskRemap      = EXTRACT_MASK32(KNOWN_PID_REMAP0); 
            maskRemapValue = EXTRACT_MASK32(KNOWN_PID_REMAP_VALUE0); 
            
            if (physicalDemodId <= MXL_HRCLS_DEMOD4)
            {
              regAddr = EXTRACT_ADDR32(KNOWN_PID_VALID0) + (physicalDemodId-MXL_HRCLS_DEMOD1) * 0x080;
            }
            else // (physicalDemodId >= MXL_HRCLS_DEMOD5)
            {
              regAddr = EXTRACT_ADDR32(KNOWN_PID_VALID1) + (physicalDemodId-MXL_HRCLS_DEMOD5) * 0x080;
            }

            for (i = 0; (i < pidCnt) && (status == MXL_SUCCESS); i++)
            {
              if (pidsPtr[i].pidValue <= MXL_HRCLS_XPT_MAX_KNOWN_PID_VALUE) // known PID
              {
                regData = (maskValid                                                                                                                                 )
                        | (maskDrop       & (((MXL_TRUE == pidsPtr[i].pidDrop)?(1UL):(0UL))                                     << EXTRACT_LSBLOC32(KNOWN_PID_DROP0)))
                        | (maskRemap      & (((pidsPtr[i].pidValue==pidsPtr[i].pidRemapValue)?(0UL):(1UL))                      << EXTRACT_LSBLOC32(KNOWN_PID_REMAP0)))
                        | (maskRemapValue & (((pidsPtr[i].pidValue==pidsPtr[i].pidRemapValue)?(0UL):(pidsPtr[i].pidRemapValue)) << EXTRACT_LSBLOC32(KNOWN_PID_REMAP_VALUE0)))
                        ;
                status |= MxL_HRCLS_Ctrl_WriteXptRegister(devId, regAddr + 4*pidsPtr[i].pidValue, regData);
              }
            } 
          }
#endif // _MXL_HRCLS_XPT_KNOWN_PIDS_ENABLED_

          // Toggle bank A/B & configure default drop 
          if (physicalDemodId <= MXL_HRCLS_DEMOD4)
          {
            status |= MxL_HRCLS_Ctrl_ReadXptRegister(devId, EXTRACT_ADDR32(PID_CONTEXT_SELECT0), &regData);
            status |= MxL_HRCLS_Ctrl_ReadXptField(devId, STREAM_MUXMODE0, &regData2);
            if (status == MXL_SUCCESS)
            {
              regData ^= 1UL << EXTRACT_LSBLOC32(PID_CONTEXT_SELECT0);
              regData ^= 1UL << EXTRACT_LSBLOC32(KNOWN_CONTEXT_REG_SELECT0);
              regData ^= pidMuxMask[regData2];

              if (MXL_TRUE == defaultDrop)
              {
                regData |=  (EXTRACT_MASK32(PID_DEFAULT_DROP0) << (physicalDemodId-MXL_HRCLS_DEMOD1));
              }
              else // (MXL_FALSE == defaultDrop)
              {
                regData &= ~(EXTRACT_MASK32(PID_DEFAULT_DROP0) << (physicalDemodId-MXL_HRCLS_DEMOD1));
              }
              status = MxL_HRCLS_Ctrl_WriteXptRegister(devId, EXTRACT_ADDR32(PID_CONTEXT_SELECT0), regData);
            }
          }
          else // (physicalDemodId >= MXL_HRCLS_DEMOD5) 
          {
            status |= MxL_HRCLS_Ctrl_ReadXptRegister(devId, EXTRACT_ADDR32(PID_CONTEXT_SELECT1), &regData);
            status |= MxL_HRCLS_Ctrl_ReadXptField(devId, STREAM_MUXMODE1, &regData2);
            if (status == MXL_SUCCESS)
            {
              regData ^= 1UL << EXTRACT_LSBLOC32(PID_CONTEXT_SELECT1);
              regData ^= 1UL << EXTRACT_LSBLOC32(KNOWN_CONTEXT_REG_SELECT1);
              regData ^= pidMuxMask[regData2];

              if (MXL_TRUE == defaultDrop)
              {
                regData |=  (EXTRACT_MASK32(PID_DEFAULT_DROP4) << (physicalDemodId-MXL_HRCLS_DEMOD5));
              }
              else // (MXL_FALSE == defaultDrop)
              {
                regData &= ~(EXTRACT_MASK32(PID_DEFAULT_DROP4) << (physicalDemodId-MXL_HRCLS_DEMOD5));
              }
              status = MxL_HRCLS_Ctrl_WriteXptRegister(devId, EXTRACT_ADDR32(PID_CONTEXT_SELECT1), regData);
            }
          }
        } else status = MXL_INVALID_PARAMETER;
      } else status = MXL_NOT_READY; 
    } else status = MXL_NOT_SUPPORTED;
  } else status = MXL_NOT_INITIALIZED;

  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgXptHeaderWords
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 02/28/2013
--|                  
--| DESCRIPTION   : Configure input header words
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgXptHeaderWords(UINT8 devId, 
                                                 MXL_HRCLS_DMD_ID_E demodId, 
                                                 MXL_HRCLS_XPT_HEADER_SIZE_E  wordCnt, 
                                                 UINT32 *wordPtr)
{
  UINT8 status;
  UINT8 i;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  UINT8 insertWordsCnt = (wordCnt == MXL_HRCLS_XPT_HDR_SIZE_0_BYTES)?0:(wordCnt == MXL_HRCLS_XPT_HDR_SIZE_4_BYTES)?1:(wordCnt == MXL_HRCLS_XPT_HDR_SIZE_8_BYTES)?2:3;
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(
    {
      MxL_HRCLS_DEBUG("demodId=%d, wordCnt=%x\n", demodId, wordCnt);
      if (wordPtr)
      {
        for (i = 0; i < insertWordsCnt; i++) MxL_HRCLS_DEBUG("%d. 0x%04x\n", i, wordPtr[i]);
      } else MxL_HRCLS_DEBUG("wordPtr is NULL\n");
    }
             );

  if (wordPtr && devContextPtr->driverInitialized)
  {    
    if (devContextPtr->xpt.supported == MXL_TRUE)
    {
      if (devContextPtr->xpt.currentMode)
      {
        if ((demodId < devContextPtr->xpt.currentMode->demodsCnt) && (wordCnt <= MXL_HRCLS_XPT_MAX_HEADER_WORDS))
        {
          MXL_HRCLS_DMD_ID_E physicalDemodId = MxLWare_HRCLS_Ctrl_GetPhysicalXptDemodId(devContextPtr, demodId);

          status = MxL_HRCLS_Ctrl_WriteXptAddrAutoInc(devId, EXTRACT_ADDR32(INP0_MERGE_HDR0) + 12*(physicalDemodId - MXL_HRCLS_DEMOD1));
          for (i = 0; (i < insertWordsCnt) && (status == MXL_SUCCESS); i++)
          { 
            status |= MxL_HRCLS_Ctrl_WriteXptDataAutoInc(devId, wordPtr[i]);
          }
          if (physicalDemodId <= MXL_HRCLS_DEMOD4)
          {
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, APPEND_BYTES0, insertWordsCnt);
          }
          else // (physicalDemodId > MXL_HRCLS_DEMOD4)
          {
            status |= MxL_HRCLS_Ctrl_UpdateXptField(devId, APPEND_BYTES1, insertWordsCnt);
          }
        } else status = MXL_INVALID_PARAMETER;
      } else status = MXL_NOT_READY; 
    } else status = MXL_NOT_SUPPORTED;
  }
  else 
  {
    if (wordPtr == NULL) status = MXL_INVALID_PARAMETER;
    else status = MXL_NOT_INITIALIZED;
  }
  
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E)status;
}

#endif
