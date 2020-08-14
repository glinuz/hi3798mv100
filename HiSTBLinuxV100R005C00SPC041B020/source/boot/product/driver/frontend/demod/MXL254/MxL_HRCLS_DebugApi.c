/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_DebugApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 02/10/2012
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS debug APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/


#include "MxL_HRCLS_Common.h"

#ifdef _MXL_HRCLS_DEBUG_ENABLED_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[out] dsCalLogPtr pointer to returned downstream calibration log
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT8* outBufPtr;  // Pointer to output buffer, allocated by caller
 *      UINT32 outBufLen;  // Size of output buffer, specified by the caller
 *      UINT32 outPos;     // Current write position
 *    } MXL_HRCLS_LOG_T;
 *    @endcode
 *
 *  @apibrief   This API returns downstream calibration log
 *
 *  @usage      This API can be called right after MxLWare_HRCLS_API_CfgTunerDsCal to obtain
 *              downstream calibration log
 *
 *  @equ261     MXL_TUNER_DSCAL_INFO_REQ
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_HRCLS_API MXL_STATUS_E MxLWare_HRCLS_API_ReqTunerDsCalInfo(
    UINT8     devId,
    MXL_HRCLS_LOG_T* dsCalLogPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (dsCalLogPtr) && (dsCalLogPtr->outBufPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT32 maxPrint = dsCalLogPtr->outBufLen - dsCalLogPtr->outPos;
      UINT32 charsWritten;          
      char * stringPos = (char *) &dsCalLogPtr->outBufPtr[dsCalLogPtr->outPos];
      UINT8 tiltIndex, segmentIndex;

      for (tiltIndex = 0; (tiltIndex < MXL_HRCLS_CAL_TILT_MODES_COUNT) && (status == MXL_SUCCESS) && (maxPrint); tiltIndex++)
      {
        charsWritten = snprintf(stringPos, maxPrint, "TiltIndex:%d\n", tiltIndex);
        maxPrint -= charsWritten;
        stringPos += charsWritten;
        for (segmentIndex = 0; (segmentIndex < MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT) && (status == MXL_SUCCESS) && (maxPrint); segmentIndex++)
        {
          MXL_HRCLS_CAL_COEFF_T * coeffs = &devContextPtr->pwrReportData.pwrCorrCoeffs[tiltIndex][segmentIndex];
          charsWritten = snprintf(stringPos, maxPrint, "Segment:%d | c2=%3d c1=%3d c0=%3d\n", segmentIndex, coeffs->c2, coeffs->c1, coeffs->c0);
          maxPrint -= charsWritten;
          stringPos += charsWritten;
        }
      }
      if (maxPrint == 0) 
      {
        MXLERR(MxL_HRCLS_ERROR("Output buffer size is too small to write all debug info into it\n"););
        status = MXL_INVALID_PARAMETER;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return((MXL_STATUS_E)status);
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  cmdString null-terminated command String
 *  @param[out] cliLogPtr pointer to result string log
 *
 *  @reltype
 *    @code
 *    typedef struct
 *    {
 *      UINT8* outBufPtr;  // Pointer to output buffer, allocated by caller
 *      UINT32 outBufLen;  // Size of output buffer, specified by the caller
 *      UINT32 outPos;     // Current write position (length of returned string)
 *    } MXL_HRCLS_LOG_T;
 *    @endcode
 *
 *  @apibrief   This API is for command line based debugging only
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS or MXL_FAILURE
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDebugCli(
    UINT8     devId,                    
    SINT8*    cmdString,               
    MXL_HRCLS_LOG_T* outLogPtr        
    )
{
  MXL_STATUS_E status = MXL_NOT_SUPPORTED;

  devId = devId; //anti-warning
  cmdString = cmdString;
  outLogPtr = outLogPtr;  

  return status; 
}

#endif

