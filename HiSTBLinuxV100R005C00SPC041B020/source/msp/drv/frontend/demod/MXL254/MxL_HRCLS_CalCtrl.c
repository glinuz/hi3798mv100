/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_CalCtrl.c
 *
 * AUTHOR             : 
 *
 * DATE CREATED       : 
 *
 * DESCRIPTION        : This file contains calibration routines like
 *                      Power Calibration for RX power report
 *
 *****************************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"
#include "MxL_HRCLS_PhyCtrl.h"
#include "MxL_HRCLS_CalCtrl.h"

#define CAL_WAIT_FOR_AVERAGED_POWER_TIMEOUT_MS 500 
#define CAL_LOOP_ITERATION_WAIT_MS 10

#define CAL_USE_MULTIPLE_DFE_CHANNELS 8

#define CAL_CPFF_C02 1596
#define CAL_CPFF_C01 150
#define CAL_CPFF_C00 -8

#define CAL_CPFF_C12 -760
#define CAL_CPFF_C11 3991
#define CAL_CPFF_C10 -1623

#define CAL_CPFF_C22 -3356
#define CAL_CPFF_C21 12448
#define CAL_CPFF_C20 -8563

#define CAL_5_4MHZ_TO_6MHZ_CORR_VALUE_x10 5

#define CAL_CPFF_SECOND_BAND_THRESHOLD_KHZ (SINT32) 12812
#define CAL_CPFF_THIRD_BAND_THRESHOLD_KHZ (SINT32) 26243
#define CAL_CPFF_BANDWIDTH_KHZ (SINT32) 84375

#define CAL_TEMP_COMP_T_REF_CODE        2
#define CAL_TEMP_COMP_T_REF_TEMP_V2     50

#define MXL_HRCLS_CAL_FIRST_FREQ_MHZ_MIN           40

// C_GTTZ
#define CAL_GTTZ_T_REF_CODE 2
#define CAL_GTTZ_T_SLOPE (-3972)

#define CAL_GTTZ_T_REF_TEMP_V2 50
#define CAL_GTTZ_T_SLOPE_V2 (-199)

typedef struct
{
  SINT16 c2;
  SINT16 c1;
  SINT16 c0;
} MXL_HRCLS_TEMP_COMP_COEFFS_T;

static UINT16 MxL_HRCLS_CalFreqPointsMHzBands1to7[MXL_HRCLS_CAL_MIN_FREQ_POINTS] = {
    108, 117, 126, 135, 144, 150, 181, 212, 243, 274, 300, 331, 362, 393, 424, 450, 481, 512, 543, 574, 600, 631, 662, 693, 724, 750, 781, 812, 843, 874, 900, 921, 942, 963, 984, 1002};

#if 0
static UINT16 MxL_HRCLS_CalFreqPointsMHzBand0_opt1[MXL_HRCLS_CAL_POINTS_PER_SEGMENT] = {44, 57, 70, 83, 96};
static UINT16 MxL_HRCLS_CalFreqPointsMHzBand0_opt2[MXL_HRCLS_CAL_POINTS_PER_SEGMENT] = {85, 90, 95, 100, 105};
#endif
static UINT16 MxL_HRCLS_CalSegmentStartFreqs[MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT + 1] = {0, 108, 150, 300, 450, 600, 750, 900, 1002};

static MXL_HRCLS_TEMP_COMP_COEFFS_T MxL_HRCLS_TempCompCoeffsCA[2*3] = {
   {MXL_HRCLS_TEMP_COMP_COEFF_CA002, MXL_HRCLS_TEMP_COMP_COEFF_CA001, MXL_HRCLS_TEMP_COMP_COEFF_CA000},  // [0][0]
   {MXL_HRCLS_TEMP_COMP_COEFF_CA012, MXL_HRCLS_TEMP_COMP_COEFF_CA011, MXL_HRCLS_TEMP_COMP_COEFF_CA010},  // [0][1]
   {MXL_HRCLS_TEMP_COMP_COEFF_CA022, MXL_HRCLS_TEMP_COMP_COEFF_CA021, MXL_HRCLS_TEMP_COMP_COEFF_CA020},  // [0][2]

   {MXL_HRCLS_TEMP_COMP_COEFF_CA102, MXL_HRCLS_TEMP_COMP_COEFF_CA101, MXL_HRCLS_TEMP_COMP_COEFF_CA100},  // [1][0]
   {MXL_HRCLS_TEMP_COMP_COEFF_CA112, MXL_HRCLS_TEMP_COMP_COEFF_CA111, MXL_HRCLS_TEMP_COMP_COEFF_CA110},  // [1][1]
   {MXL_HRCLS_TEMP_COMP_COEFF_CA122, MXL_HRCLS_TEMP_COMP_COEFF_CA121, MXL_HRCLS_TEMP_COMP_COEFF_CA120}   // [1][2]
};

static MXL_HRCLS_TEMP_COMP_COEFFS_T MxL_HRCLS_TempCompCoeffsCB[2*3] = {
   {MXL_HRCLS_TEMP_COMP_COEFF_CB002, MXL_HRCLS_TEMP_COMP_COEFF_CB001, MXL_HRCLS_TEMP_COMP_COEFF_CB000},  // [0][0]
   {MXL_HRCLS_TEMP_COMP_COEFF_CB012, MXL_HRCLS_TEMP_COMP_COEFF_CB011, MXL_HRCLS_TEMP_COMP_COEFF_CB010},  // [0][1]
   {MXL_HRCLS_TEMP_COMP_COEFF_CB022, MXL_HRCLS_TEMP_COMP_COEFF_CB021, MXL_HRCLS_TEMP_COMP_COEFF_CB020},  // [0][2]

   {MXL_HRCLS_TEMP_COMP_COEFF_CB102, MXL_HRCLS_TEMP_COMP_COEFF_CB101, MXL_HRCLS_TEMP_COMP_COEFF_CB100},  // [1][0]
   {MXL_HRCLS_TEMP_COMP_COEFF_CB112, MXL_HRCLS_TEMP_COMP_COEFF_CB111, MXL_HRCLS_TEMP_COMP_COEFF_CB110},  // [1][1]
   {MXL_HRCLS_TEMP_COMP_COEFF_CB122, MXL_HRCLS_TEMP_COMP_COEFF_CB121, MXL_HRCLS_TEMP_COMP_COEFF_CB120}   // [1][2]
};

#define CAL_TEMP_TILTMODE_SEGMENTS_COUNT 7 
static UINT16 MxL_HRCLS_TempTiltCompFreqs[] = {44, 240, 440, 600, 750, 850, 940, 1002}; 
static SINT16 MxL_HRCLS_TempTiltCompOffset_0C[] = {
             6,  8, 10,  14,  20,  30,  35,  48,  // j = 0; 
             3,  4,  8,  11,  20,  32,  42,  54,  // j = 1;
            -3, -3,  6,  12,  21,  32,  43,  56,  // j = 2;
           -18, -6,  6,  11,  22,  34,  48,  59}; // j = 3;
static SINT16 MxL_HRCLS_TempTiltCompOffset_125C[] = {
           -11,-12,-18, -24, -34, -48, -54, -72,  // j = 0;
            -5, -6,-13, -21, -35, -50, -62, -78,  // j = 1;
             2,  2,-11, -20, -37, -51, -68, -84,  // j = 2;
            14,  5,-11, -22, -38, -54, -71, -88}; // j = 3;
            

static SINT16 MxL_HRCLS_Ctrl_CalGetCPFF(UINT16 lockFreqMHz);

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetNormalized
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 03/16/2012
--|
--| DESCRIPTION   : This function converts absolute frequency to normalized  
--|                
--|
--| INPUTS        : device context, freqMHz
--|
--| RETURN VALUE  : normalized frequency, segment index for input frequency 
--|
--|-------------------------------------------------------------------------------------*/

static UINT8 MxL_HRCLS_Ctrl_CalGetNormalized(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT16 freqMHz, UINT8 * segmentId)
{
  UINT8 s=0;
  UINT16 normFreq;
  UINT16 startFreq;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d\n", freqMHz););

  if (freqMHz != MxL_HRCLS_CalSegmentStartFreqs[1])
  {
    while ((s < MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT) && (freqMHz > MxL_HRCLS_CalSegmentStartFreqs[s + 1])) s++;
  } else s = 1;
  startFreq = (s == 0)?(devContextPtr->pwrReportData.firstFreqMHz):MxL_HRCLS_CalSegmentStartFreqs[s];
  normFreq = freqMHz - startFreq;
  normFreq *= 10;
  normFreq = MXL_DIV_ROUND(normFreq, (MxL_HRCLS_CalSegmentStartFreqs[s + 1] - startFreq));

  if (segmentId) *segmentId=s;
  MXLEXIT(MxL_HRCLS_DEBUG("segmentId=%d\n", s););
  MXLEXITSTR(normFreq);

  return (UINT8) normFreq;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCGTF_V2
--| 
--| AUTHOR        : Mariusz Murawski, Sudhamshu Ramachandra
--|
--| DATE CREATED  : 04/26/2012, 10/24/2012
--|
--| DESCRIPTION   : Calculates CGTF component for given frequency and temperature
--|                 Due to temperature sensor changes for V2/3, currentTemp is now in degree C.
--|
--| INPUTS        : device context, freqMHz, currentTemp
--|
--| RETURN VALUE  : CGTF value (dBx100) 
--|
--|-------------------------------------------------------------------------------------*/

static SINT16 MxL_HRCLS_Ctrl_CalGetCGTF_V2(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT16 freqMHz, UINT8 currentTemp)
{
  SINT16 cgtf = 0;
  MXL_HRCLS_TEMP_COMP_COEFFS_T * coeffs;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d, currentTemp=%d\n", freqMHz, currentTemp););

  if ((devContextPtr->currentAfeTiltMode == MXL_HRCLS_AFE_FLAT_LNABO_0) || (devContextPtr->currentAfeTiltMode == MXL_HRCLS_AFE_FLAT_LNABO_NON0))
  {
    UINT8 groupId = 0;   // i;
    UINT8 tId = 0;       // j;
    SINT16 c2, c1, c0;
    SINT64 y = 0;
    SINT64 c_tf;

    MXLDBG2(MxL_HRCLS_DEBUG("Temperature compensation value for flat mode\n"););

    groupId = devContextPtr->currentAfeBO;
    if (groupId < 4) groupId = 0;
      else if (groupId < 7) groupId = 1;
        else groupId = 2;

    if (currentTemp >= CAL_TEMP_COMP_T_REF_TEMP_V2) tId = 1;
    if (freqMHz < 700) coeffs = (MXL_HRCLS_TEMP_COMP_COEFFS_T *) MxL_HRCLS_TempCompCoeffsCA; else coeffs = (MXL_HRCLS_TEMP_COMP_COEFFS_T *) MxL_HRCLS_TempCompCoeffsCB;

    c2 = coeffs[tId*3 + groupId].c2;
    c1 = coeffs[tId*3 + groupId].c1;
    c0 = coeffs[tId*3 + groupId].c0;

    y  = (SINT64) c2 * (SINT64) freqMHz * (SINT64) freqMHz;
    y += (SINT64) c1 * (SINT64) freqMHz * (SINT64) 1024;
    y += (SINT64) c0 * (SINT64) 1024    * (SINT64) 1024;

    if (currentTemp >= CAL_TEMP_COMP_T_REF_TEMP_V2)
    {
      c_tf = (SINT64) MXL_DIV_ROUND_S(y * (SINT64) (currentTemp - CAL_TEMP_COMP_T_REF_TEMP_V2), 45);
    }
    else
    {
      c_tf = (SINT64) MXL_DIV_ROUND_S(y * (SINT64) (CAL_TEMP_COMP_T_REF_TEMP_V2 - currentTemp), 35);
    }
    c_tf = MXL_DIV_ROUND_S(c_tf, MXL_DIV_ROUND(1024*1024*1024, 100));
    MXLDBG2(MxL_HRCLS_DEBUG("c2=%d, c1=%d, c0=%d, j=%d, i=%d, t=%d, c_tf=%Ld\n", c2, c1, c0, tId, groupId, currentTemp, (long long int) c_tf););
    
    cgtf = (SINT16) c_tf;
  }
  else
  {
    MXLDBG2(MxL_HRCLS_DEBUG("Temperature compensation value for tilt mode\n"););

    if ((freqMHz >= MxL_HRCLS_TempTiltCompFreqs[0]) && (freqMHz <= MxL_HRCLS_TempTiltCompFreqs[CAL_TEMP_TILTMODE_SEGMENTS_COUNT]))
    {
      UINT8 segmentId = 0;
      SINT16 * MxL_HRCLS_TempTiltCompOffset;
      SINT32 k, y;
      UINT8 tiltModeId = (devContextPtr->currentAfeTiltMode - MXL_HRCLS_AFE_TILT1); 
      
      while (freqMHz > MxL_HRCLS_TempTiltCompFreqs[segmentId+1]) segmentId++;
      if (currentTemp >= CAL_TEMP_COMP_T_REF_TEMP_V2) 
        MxL_HRCLS_TempTiltCompOffset = MxL_HRCLS_TempTiltCompOffset_125C; 
        else 
        MxL_HRCLS_TempTiltCompOffset = MxL_HRCLS_TempTiltCompOffset_0C; 

      k = MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + (segmentId + 1)] - MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + segmentId];
      k *= 100;
      k = MXL_DIV_ROUND_S(k, (MxL_HRCLS_TempTiltCompFreqs[segmentId+1] - MxL_HRCLS_TempTiltCompFreqs[segmentId]));
      y = k * (freqMHz - MxL_HRCLS_TempTiltCompFreqs[segmentId]) + (MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + segmentId] * 100);
      MXLDBG2(MxL_HRCLS_DEBUG("segmentId=%d, tiltModeId=%d, k=%d, y=%d\n", segmentId, tiltModeId, k, y););

      if (currentTemp >= CAL_TEMP_COMP_T_REF_TEMP_V2)
      {
        cgtf = MXL_DIV_ROUND_S((y * (currentTemp - CAL_TEMP_COMP_T_REF_TEMP_V2)), 45);
      }
      else
      {
        cgtf = MXL_DIV_ROUND_S((y * (CAL_TEMP_COMP_T_REF_TEMP_V2 - currentTemp)), 35);
      }
      cgtf = MXL_DIV_ROUND_S(cgtf, 32);  // default multiplier of values in MxL_HRCLS_TempTiltCompOffset
    }
  }
  MXLEXITSTR(cgtf);

  return cgtf;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCGTF 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/26/2012
--|
--| DESCRIPTION   : Calculates CGTF component for given frequency and temperature
--|                
--|
--| INPUTS        : device context, freqMHz, currentTemp
--|
--| RETURN VALUE  : CGTF value (dBx100) 
--|
--|-------------------------------------------------------------------------------------*/
static SINT16 MxL_HRCLS_Ctrl_CalGetCGTF(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT16 freqMHz, UINT8 currentTemp)
{
  SINT16 cgtf = 0;
  MXL_HRCLS_TEMP_COMP_COEFFS_T * coeffs;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d, currentTemp=%d\n", freqMHz, currentTemp););

  if ((devContextPtr->currentAfeTiltMode == MXL_HRCLS_AFE_FLAT_LNABO_0) || (devContextPtr->currentAfeTiltMode == MXL_HRCLS_AFE_FLAT_LNABO_NON0))
  {
    UINT8 groupId = 0;   // i;
    UINT8 tId = 0;       // j;
    SINT16 c2, c1, c0;
    SINT64 y = 0;
    SINT64 c_tf;

    MXLDBG2(MxL_HRCLS_DEBUG("Temperature compensation value for flat mode\n"););

    groupId = devContextPtr->currentAfeBO;
    if (groupId < 4) groupId = 0;
      else if (groupId < 7) groupId = 1;
        else groupId = 2;

    if (currentTemp >= CAL_TEMP_COMP_T_REF_CODE) tId = 1;
    if (freqMHz < 700) coeffs = (MXL_HRCLS_TEMP_COMP_COEFFS_T *) MxL_HRCLS_TempCompCoeffsCA; else coeffs = (MXL_HRCLS_TEMP_COMP_COEFFS_T *) MxL_HRCLS_TempCompCoeffsCB;

    c2 = coeffs[tId*3 + groupId].c2;
    c1 = coeffs[tId*3 + groupId].c1;
    c0 = coeffs[tId*3 + groupId].c0;

    y  = (SINT64) c2 * (SINT64) freqMHz * (SINT64) freqMHz;
    y += (SINT64) c1 * (SINT64) freqMHz * (SINT64) 1024;
    y += (SINT64) c0 * (SINT64) 1024    * (SINT64) 1024;

    if (currentTemp >= CAL_TEMP_COMP_T_REF_CODE)
    {
      c_tf = (SINT64) MXL_DIV_ROUND_S(y * (SINT64) (currentTemp - CAL_TEMP_COMP_T_REF_CODE), 3);
    }
    else
    {
      c_tf = (SINT64) MXL_DIV_ROUND_S(y * (SINT64) (CAL_TEMP_COMP_T_REF_CODE - currentTemp), 2);
    }
    c_tf = MXL_DIV_ROUND_S(c_tf, MXL_DIV_ROUND(1024*1024*1024, 100));
    MXLDBG2(MxL_HRCLS_DEBUG("c2=%d, c1=%d, c0=%d, j=%d, i=%d, t=%d, c_tf=%Ld\n", c2, c1, c0, tId, groupId, currentTemp, (long long int) c_tf););
    
    cgtf = (SINT16) c_tf;
  }
  else
  {
    MXLDBG2(MxL_HRCLS_DEBUG("Temperature compensation value for tilt mode\n"););

    if ((freqMHz >= MxL_HRCLS_TempTiltCompFreqs[0]) && (freqMHz <= MxL_HRCLS_TempTiltCompFreqs[CAL_TEMP_TILTMODE_SEGMENTS_COUNT]))
    {
      UINT8 segmentId = 0;
      SINT16 * MxL_HRCLS_TempTiltCompOffset;
      SINT32 k, y;
      UINT8 tiltModeId = (devContextPtr->currentAfeTiltMode - MXL_HRCLS_AFE_TILT1); 
      
      while (freqMHz > MxL_HRCLS_TempTiltCompFreqs[segmentId+1]) segmentId++;
      if (currentTemp >= CAL_TEMP_COMP_T_REF_CODE) 
        MxL_HRCLS_TempTiltCompOffset = MxL_HRCLS_TempTiltCompOffset_125C; 
        else 
        MxL_HRCLS_TempTiltCompOffset = MxL_HRCLS_TempTiltCompOffset_0C; 

      k = MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + (segmentId + 1)] - MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + segmentId];
      k *= 100;
      k = MXL_DIV_ROUND_S(k, (MxL_HRCLS_TempTiltCompFreqs[segmentId+1] - MxL_HRCLS_TempTiltCompFreqs[segmentId]));
      y = k * (freqMHz - MxL_HRCLS_TempTiltCompFreqs[segmentId]) + (MxL_HRCLS_TempTiltCompOffset[tiltModeId * (CAL_TEMP_TILTMODE_SEGMENTS_COUNT+1) + segmentId] * 100);
      MXLDBG2(MxL_HRCLS_DEBUG("segmentId=%d, tiltModeId=%d, k=%d, y=%d\n", segmentId, tiltModeId, k, y););

      if (currentTemp >= CAL_TEMP_COMP_T_REF_CODE)
      {
        cgtf = MXL_DIV_ROUND_S((y * (currentTemp - CAL_TEMP_COMP_T_REF_CODE)), 3);
      }
      else
      {
        cgtf = MXL_DIV_ROUND_S((y * (CAL_TEMP_COMP_T_REF_CODE - currentTemp)), 2);
      }
      cgtf = MXL_DIV_ROUND_S(cgtf, 32);  // default multiplier of values in MxL_HRCLS_TempTiltCompOffset
    }
  }
  MXLEXITSTR(cgtf);

  return cgtf;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCGTTZ_V2
--| 
--| AUTHOR        : Mariusz Murawski, Sudhamshu Ramachandra
--|
--| DATE CREATED  : 04/26/2012, 10/24/2012
--|
--| DESCRIPTION   : Calculates CGTTZ component for given frequency
--|                 Due to temperature sensor changes for V2/3, currentTemp is now in degree C.
--|
--| INPUTS        : device context, freqMHz, currentTemp
--|
--| RETURN VALUE  : CGTTZ value (dBx100) 
--|
--|-------------------------------------------------------------------------------------*/
static SINT16 MxL_HRCLS_Ctrl_CalGetCGTTZ_V2(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, SINT16 currentTemp)
{
  SINT32 cgttz = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("currentTemp=%d\n", currentTemp););

  devContextPtr = devContextPtr;
  //MxL_HRCLS_DEBUG("currentTemp=%d\n", currentTemp);
  cgttz = ((SINT32) currentTemp - (SINT32) CAL_GTTZ_T_REF_TEMP_V2) * ((SINT32) CAL_GTTZ_T_SLOPE_V2) * (SINT32) 100;
  cgttz = MXL_DIV_ROUND_S(cgttz, 65536);
  
  MXLEXITSTR(cgttz);
  
  return (SINT16) cgttz;
}



/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCGTTZ
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/26/2012
--|
--| DESCRIPTION   : Calculates CGTTZ component for given frequency
--|                
--|
--| INPUTS        : device context, freqMHz, currentTemp
--|
--| RETURN VALUE  : CGTTZ value (dBx100) 
--|
--|-------------------------------------------------------------------------------------*/
static SINT16 MxL_HRCLS_Ctrl_CalGetCGTTZ(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT8 currentTemp)
{
  SINT32 cgttz = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("currentTemp=%d\n", currentTemp););

  devContextPtr = devContextPtr;
  //MxL_HRCLS_DEBUG("currentTemp=%d\n", currentTemp);
  cgttz = ((SINT32) currentTemp - (SINT32) CAL_GTTZ_T_REF_CODE) * ((SINT32) CAL_GTTZ_T_SLOPE) * (SINT32) 100;
  cgttz = MXL_DIV_ROUND_S(cgttz, 65536);
  
  MXLEXITSTR(cgttz);
  
  return (SINT16) cgttz;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCGFF 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 03/16/2012
--|
--| DESCRIPTION   : Calculates CGFF component for given frequency
--|                
--|
--| INPUTS        : device context, freqMHz
--|
--| RETURN VALUE  : CGFF value (dBx10) 
--|
--|-------------------------------------------------------------------------------------*/
static SINT16 MxL_HRCLS_Ctrl_CalGetCGFF(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT16 freqMHz)
{
  SINT16 cgff = 0;
  UINT8 segmentId = 0;
  UINT8 normalizedFreq = MxL_HRCLS_Ctrl_CalGetNormalized(devContextPtr, freqMHz, &segmentId);
  MXL_HRCLS_CAL_COEFF_T * coeffs;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d\n", freqMHz););

  MXLDBG2(MxL_HRCLS_DEBUG("normalized=%d, segment=%d\n", normalizedFreq, segmentId););
  if (segmentId < MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT)
  {
    SINT32 temp = 0;

//    devContextPtr->currentAfeTiltMode = MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(devContextPtr);

    coeffs = &devContextPtr->pwrReportData.pwrCorrCoeffs[(UINT8) devContextPtr->currentAfeTiltMode-MXL_HRCLS_AFE_FLAT_LNABO_0][segmentId];

    MXLDBG2(MxL_HRCLS_DEBUG("c2=%d, c1=%d, c0=%d\n", coeffs->c2, coeffs->c1, coeffs->c0););

    temp = coeffs->c2 * (normalizedFreq * normalizedFreq) + coeffs->c1 * (normalizedFreq * 10) + coeffs->c0 * 100;
    temp = MXL_DIV_ROUND(temp, 100);
    cgff = (SINT16) temp;
  }
  MXLEXITSTR(cgff);

  return cgff;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCorrected 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/29/2012
--|
--| DESCRIPTION   : This function takes rawPower (power value read from the IC),
--|                 calculates correction coefficients and returns corrected value
--|
--| INPUTS        : device context, freqMHz, rawPower [dBuVx10]
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, correctedPower [dBuVx10]
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_CalGetCorrected(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
                                      UINT16 freqMHz,
                                      UINT16 rawPower,
                                      UINT16 * correctedPower)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d, rawPower=%d\n", freqMHz, rawPower););
  if (correctedPower)
  {
    SINT16 c_gff = 0;
    SINT16 c_gtf = 0;
    SINT16 c_pff = MxL_HRCLS_Ctrl_CalGetCPFF(freqMHz);
    SINT16 correction = 0;

    if ((devContextPtr->pwrReportData.dataLoaded == MXL_TRUE) && (freqMHz >= devContextPtr->pwrReportData.firstFreqMHz) 
        && (freqMHz <= MxL_HRCLS_CalSegmentStartFreqs[MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT]))
    {
      c_gff = MxL_HRCLS_Ctrl_CalGetCGFF(devContextPtr, freqMHz);
    }
//    devContextPtr->currentAfeBO = MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(devContextPtr);
    if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
    {
      c_gtf = MxL_HRCLS_Ctrl_CalGetCGTF(devContextPtr, freqMHz, devContextPtr->currentTemp);
    }
    else
    {
      c_gtf = MxL_HRCLS_Ctrl_CalGetCGTF_V2(devContextPtr, freqMHz, devContextPtr->currentTemp);
    }

    correction = c_gff + c_gtf + c_pff;
    correction = MXL_DIV_ROUND_S(correction, 10);
    *correctedPower = rawPower - correction;
    MXLDBG2(
    MxL_HRCLS_DEBUG("====================================================\n");
    MxL_HRCLS_DEBUG("Power calculation for f=%dMHz\n", freqMHz);
    MxL_HRCLS_DEBUG("c_gff = %d dBx100\n", c_gff);
    MxL_HRCLS_DEBUG("c_gtf = %d dBx100\n", c_gtf);
    MxL_HRCLS_DEBUG("c_pff = %d dBx100\n", c_pff);
    MxL_HRCLS_DEBUG("power = rawPower - c_gff - c_gtf - c_pff\n");
    MxL_HRCLS_DEBUG("%d.%ddBuV = %d.%ddBuV %c %01d.%02ddB %c %01d.%02ddB %c %01d.%02ddB\n", 
          *correctedPower / 10, *correctedPower % 10,
          rawPower / 10, rawPower % 10,
          (c_gff>=0)?'-':'+',
          c_gff / 100, MXL_ABS(c_gff % 100),
          (c_gtf>=0)?'-':'+',
          c_gtf / 100, MXL_ABS(c_gtf % 100),
          (c_pff>=0)?'-':'+',
          c_pff / 100, MXL_ABS(c_pff % 100));
    MxL_HRCLS_DEBUG("====================================================\n");
    )
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCorrectedPSM
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/26/2012
--|
--| DESCRIPTION   : This function takes rawPower (power value read from the IC),
--|                 calculates correction coefficients and returns corrected value
--|                 for PSM (low-power tuner)
--|
--| INPUTS        : device context, freqMHz, rawPower [dBuVx10]
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER, correctedPower [dBuVx10]
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_CalGetCorrectedPSM(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
                                        UINT16 freqMHz,
                                        UINT16 rawPower,
                                        UINT16 * correctedPower)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqMHz=%d, rawPower=%d\n", freqMHz, rawPower););
  if (correctedPower)
  {
    if (status == MXL_SUCCESS)
    {
      SINT16 c_gttz = 0;
      SINT16 c_gttz_now;
      SINT16 c_gttz_handover;
      SINT16 c_gtf = 0;
      SINT16 c_gff = 0;
      SINT16 c_pff = MxL_HRCLS_Ctrl_CalGetCPFF(freqMHz);
      SINT16 correction = 0;
      UINT16 regData;
      UINT8  tempHandover = 0;

      status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_NB_HOST_HANDOVER_BKP, &regData);
      if (status == MXL_SUCCESS)
      {
        if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
        {
          tempHandover = (regData >> 5) & 0x07;
        }
        else
        {
          status = MxL_HRCLS_Ctrl_MapCodeToTempInDegrees(((regData >> 5) & MXL_HRCLS_TEMP_CODE_MAP_MASK), &tempHandover);
        }
      }

      if ((devContextPtr->pwrReportData.dataLoaded == MXL_TRUE) && (freqMHz >= devContextPtr->pwrReportData.firstFreqMHz)
          && (freqMHz <= MxL_HRCLS_CalSegmentStartFreqs[MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT]))
      {
        c_gff = MxL_HRCLS_Ctrl_CalGetCGFF(devContextPtr, freqMHz);
      }

      if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
      {
        c_gttz_now = MxL_HRCLS_Ctrl_CalGetCGTTZ(devContextPtr, devContextPtr->currentTemp);
        c_gttz_handover = MxL_HRCLS_Ctrl_CalGetCGTTZ(devContextPtr, tempHandover);
      }
      else
      {
        c_gttz_now = MxL_HRCLS_Ctrl_CalGetCGTTZ_V2(devContextPtr, devContextPtr->currentTemp);
        c_gttz_handover = MxL_HRCLS_Ctrl_CalGetCGTTZ_V2(devContextPtr, tempHandover);
      }
      c_gttz = c_gttz_now - c_gttz_handover;
      if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
      {
        c_gtf = MxL_HRCLS_Ctrl_CalGetCGTF(devContextPtr, freqMHz, devContextPtr->currentTemp);
      }
      else
      {
        c_gtf = MxL_HRCLS_Ctrl_CalGetCGTF_V2(devContextPtr, freqMHz, devContextPtr->currentTemp);
      }

      correction = c_gttz + c_gtf + c_gff + c_pff;
      correction = MXL_DIV_ROUND_S(correction, 10);

      *correctedPower = rawPower - correction;

      MXLDBG1(
          MxL_HRCLS_DEBUG("====================================================\n");
          MxL_HRCLS_DEBUG("Power calculation for PSM @ f=%dMHz\n", freqMHz);
          MxL_HRCLS_DEBUG("c_gttz = %d dBx100\n", c_gttz);
          MxL_HRCLS_DEBUG("c_gtf  = %d dBx100\n", c_gtf);
          MxL_HRCLS_DEBUG("c_gff  = %d dBx100\n", c_gff);
          MxL_HRCLS_DEBUG("c_pff  = %d dBx100\n", c_pff);
          MxL_HRCLS_DEBUG("power = rawPower - cgttz - cgtf - cgff - cpff\n");
          MxL_HRCLS_DEBUG("%d.%ddBuV = %d.%ddBuV %c %01d.%02ddB %c %01d.%02ddB %c %01d.%02ddB %c %01d.%02ddB\n",
            *correctedPower / 10, *correctedPower % 10,
            rawPower / 10, rawPower % 10,
            (c_gttz >= 0)?'-':'+', c_gttz / 100, MXL_ABS(c_gttz % 100),
            (c_gtf >= 0)?'-':'+', c_gtf / 100, MXL_ABS(c_gtf % 100),
            (c_gff >= 0)?'-':'+', c_gff / 100, MXL_ABS(c_gff % 100),
            (c_pff >= 0)?'-':'+', c_pff / 100, MXL_ABS(c_pff % 100));
          MxL_HRCLS_DEBUG("====================================================\n");
          )
    }
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalUpdateFreqPoints
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/06/2012
--|
--| DESCRIPTION   : This function creates array of frequency points at which calibration
--|                 will be performed. 
--|                 There are MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT *  MXL_HRCLS_CAL_POINTS_PER_SEGMENT + 1
--|                 points. If firstFreqMHz == 108, there will be one segment less 
--|                 because second segment also starts from 108MHz.
--|
--| INPUTS        : device context, firstFreqMHz 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CalUpdateFreqPoints(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
                                      UINT16 firstFreqMHz, 
                                      MXL_HRCLS_CAL_DATA_T * calData)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT16 band0_freqPoints[MXL_HRCLS_CAL_POINTS_PER_SEGMENT] = {0};
  UINT8 i = 0, startIndex = 0;

  devContextPtr = devContextPtr; // anti-warning

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("firstFreqMHz=%d\n", firstFreqMHz););

  if (firstFreqMHz != MxL_HRCLS_CalSegmentStartFreqs[1])
  {
    if ((firstFreqMHz >= MXL_HRCLS_CAL_FIRST_FREQ_MHZ_MIN) && (firstFreqMHz < MxL_HRCLS_CalSegmentStartFreqs[1]))
    {
      UINT16 delta_firstFreq_nextSegment = MxL_HRCLS_CalSegmentStartFreqs[1] - firstFreqMHz;

      for (i = 0; i < MXL_HRCLS_CAL_POINTS_PER_SEGMENT; i++) band0_freqPoints[i] = firstFreqMHz + (i * MXL_DIV_ROUND(delta_firstFreq_nextSegment, MXL_HRCLS_CAL_POINTS_PER_SEGMENT));
    } else status = MXL_INVALID_PARAMETER;
  }

  if (status == MXL_SUCCESS)
  {
    if (band0_freqPoints[0])
    {
      UINT16 segmentStartFreq = band0_freqPoints[0];
      UINT16 segmentWidth = MxL_HRCLS_CalSegmentStartFreqs[1] - segmentStartFreq;

      calData->segmentsCount = MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT;
      calData->freqPointsCount = MXL_HRCLS_CAL_MAX_FREQ_POINTS;
      for (i = 0; i < MXL_HRCLS_CAL_POINTS_PER_SEGMENT; i++)
      {
        UINT16 normFreq;

        calData->freqPoints[i].freqMHz = band0_freqPoints[i];
        calData->freqPoints[i].segmentIdx = 0;

        // Calculate normalized frequency (in segment)
        normFreq = (calData->freqPoints[i].freqMHz - segmentStartFreq) * 100;
        normFreq /= segmentWidth;
        calData->freqPoints[i].normFreqx100 = (UINT8) normFreq;
        MXLDBG2(MxL_HRCLS_DEBUG("Freq: %dMHz, normFreq: %01d.%02d, segmentId: %d\n", calData->freqPoints[i].freqMHz, 
              normFreq/100, normFreq % 100, calData->freqPoints[i].segmentIdx););
      }
      startIndex = i;
    }
    else
    {
      calData->segmentsCount = MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT - 1;
      calData->freqPointsCount = MXL_HRCLS_CAL_MAX_FREQ_POINTS - MXL_HRCLS_CAL_POINTS_PER_SEGMENT;
    }

    for (i = 0; i < MXL_HRCLS_CAL_MIN_FREQ_POINTS; i++)
    {
      UINT16 segmentStartFreq;
      UINT16 segmentWidth;
      UINT16 normFreq;
      UINT8 o;

      calData->freqPoints[startIndex + i].freqMHz = MxL_HRCLS_CalFreqPointsMHzBands1to7[i];
      
      // find correct segment for this frequency - needed for frequency normalization
      o = 0;
      while ((o < MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT) && (calData->freqPoints[startIndex + i].freqMHz >= MxL_HRCLS_CalSegmentStartFreqs[o+1])) o++;
      if (o == MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT) o--;
      
      segmentStartFreq = MxL_HRCLS_CalSegmentStartFreqs[o];
      segmentWidth = MxL_HRCLS_CalSegmentStartFreqs[o + 1] - segmentStartFreq;
      calData->freqPoints[startIndex + i].segmentIdx = o;

      // Calculate normalized frequency (in segment)
      normFreq = (calData->freqPoints[startIndex + i].freqMHz - segmentStartFreq) * 100;
      normFreq = MXL_DIV_ROUND(normFreq, segmentWidth);
      calData->freqPoints[startIndex + i].normFreqx100 = (UINT8) normFreq;
      MXLDBG2(MxL_HRCLS_DEBUG("Freq: %dMHz, normFreq: %01d.%02d, segmentId: %d\n", calData->freqPoints[startIndex + i].freqMHz, 
              normFreq/100, normFreq % 100, calData->freqPoints[startIndex + i].segmentIdx););
    }
  }
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetInterpolated
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/06/2012
--|
--| DESCRIPTION   : Function returnes interpolated (linear) value of reference power.
--|                 X and Y boundaries must be delivered as well as current X for which
--|                 Y must be returned. If current freq is higher than the last segment
--|                 boundary, last segment will be used for SINT16erpolation.
--|
--| INPUTS        : device context, Freq boundaries, power level boundaries, current 
--|                 freq for which power level must be returned.
--|
--| RETURN VALUE  : SINT16erpolated power level
--|
--|-------------------------------------------------------------------------------------*/
static UINT16 MxL_HRCLS_Ctrl_CalGetInterpolated(UINT16 currentFreqMHz, UINT16 lowFreqMHz, UINT16 highFreqMHz, UINT16 lowFreqPowerdBuV, UINT16 highFreqPowerdBuV)
{
  UINT16 currentPowerdBuV = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("currentFreqMHz=%d, lowFreqMHz=%d, highFreqMHz=%d, lowFreqPowerdBuV=%d, highFreqPowerdBuV=%d\n", 
                        currentFreqMHz, lowFreqMHz, highFreqMHz, lowFreqPowerdBuV, highFreqPowerdBuV););
  if ((lowFreqMHz < highFreqMHz) && (currentFreqMHz >= lowFreqMHz))
  {
    UINT16 rangeMHz = highFreqMHz - lowFreqMHz;
    SINT16 deltaPowerdB = (SINT16) highFreqPowerdBuV - (SINT16) lowFreqPowerdBuV;

    SINT16 proportionalDeltaPowerdB = MXL_DIV_ROUND((SINT16) (currentFreqMHz - lowFreqMHz) * deltaPowerdB , (SINT16) rangeMHz);

    currentPowerdBuV = lowFreqPowerdBuV + proportionalDeltaPowerdB;
  }
  MXLEXITSTR(currentPowerdBuV);
  return currentPowerdBuV;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalGetCPFF
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/09/2012
--|
--| DESCRIPTION   : This function calculates compensation for polyphase filter 
--|                 frequency variations for given frequency in MHz.
--|                 Returns correction value in dBx10
--|
--| INPUTS        : Input frequency in MHz 
--|
--| RETURN VALUE  : compensation value in dBx10
--|
--|-------------------------------------------------------------------------------------*/
static SINT16 MxL_HRCLS_Ctrl_CalGetCPFF(UINT16 lockFreqMHz)
{
  SINT32 F_kHz;
  SINT32 freqMHz;
  SINT32 delta_F;
  SINT64 c_2, c_1, c_0;
  SINT64 cpff;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("lockFreqMHz=%d\n", lockFreqMHz););

  freqMHz = (SINT32) lockFreqMHz * 1000000;  
  delta_F = ((freqMHz / CAL_CPFF_BANDWIDTH_KHZ) + 500) / 1000; 
  delta_F *= CAL_CPFF_BANDWIDTH_KHZ;
  delta_F *= 1000;
    
  F_kHz = (freqMHz>delta_F)?(freqMHz-delta_F):-(freqMHz-delta_F);
  F_kHz = MXL_DIV_ROUND(F_kHz, 1000);

  if (F_kHz < CAL_CPFF_SECOND_BAND_THRESHOLD_KHZ)
  {
    c_2 = CAL_CPFF_C02;
    c_1 = CAL_CPFF_C01;
    c_0 = CAL_CPFF_C00;
  }
  else
  if (F_kHz < CAL_CPFF_THIRD_BAND_THRESHOLD_KHZ)
  {
    c_2 = CAL_CPFF_C12;
    c_1 = CAL_CPFF_C11;
    c_0 = CAL_CPFF_C10;
  }
  else
  {
    c_2 = CAL_CPFF_C22;
    c_1 = CAL_CPFF_C21;
    c_0 = CAL_CPFF_C20;
  }

  F_kHz /= 16;   // divide by 16 (PG formula)
  
  /* 
     F_kHz is scalled to 1000 Hz, so the equation has to be balanced
    
     cpff * 1e+3 = (c_2 * f*1e+3 * f*1e+3) / 1e+3 + c_1 * f*1e+3 + c_0 * 1e+3 
  */
  cpff = c_2 * (MXL_DIV_ROUND(((F_kHz) * (F_kHz)), 1000)) + c_1 * (F_kHz) + c_0 * 1000;

  cpff /= (1 << 12);   // divide by 2^12 (PG formula - all coeffs are scalled up by 2^12)
  cpff = MXL_DIV_ROUND(cpff, 10);   // change to dBx100

//  MxL_HRCLS_DEBUG("F_kHz=%d, delta_F=%d, cpff=%ld\n", F_kHz, delta_F, cpff);
  MXLEXITSTR(((SINT16) cpff));

  return (SINT16) cpff;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalUpdateRefPowerAtFreqPoints
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/06/2012
--|
--| DESCRIPTION   : This function interpolates user provided reference power level samples
--|                 and populates context calibration points structure. 
--|                 Valid parameters have to be provided. The function does not validate
--|                 them.   
--|
--| INPUTS        : device context, frequency points count, frequencies array [MHz], 
--|                 power samples array [dBuVx10] 
--|
--| RETURN VALUE  : MXL_SUCCESS
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CalUpdateRefPowerAtFreqPoints(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
                      UINT16 freqPointsCnt, 
                      UINT16 * freqMHzPtr, 
                      UINT16 * refPowerdBuVPtr, 
                      MXL_HRCLS_CAL_DATA_T * calData)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 i;
  UINT8 o = 0;
 
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("freqPointsCnt=%d\n", freqPointsCnt););
  devContextPtr = devContextPtr; // anti-warning
  for (i = 0; i < calData->freqPointsCount; i++)
  {
    UINT16 power;
    UINT16 freqMHz;
    freqMHz = calData->freqPoints[i].freqMHz;

    if (freqPointsCnt > 1)
    {
      if (freqMHz < freqMHzPtr[freqPointsCnt - 1])
      {
        while ((o < (freqPointsCnt - 1)) && (freqMHz > freqMHzPtr[o + 1])) 
        {
          o++;
        }
      }
      else
      {
        o = freqPointsCnt - 2;
      }
      power = MxL_HRCLS_Ctrl_CalGetInterpolated(freqMHz, freqMHzPtr[o], freqMHzPtr[o+1], refPowerdBuVPtr[o], refPowerdBuVPtr[o+1]);
    } else power = refPowerdBuVPtr[0];      
    power -= CAL_5_4MHZ_TO_6MHZ_CORR_VALUE_x10;
    calData->freqPoints[i].refPowerdBuV = power;
    MXLDBG2(
      MxL_HRCLS_DEBUG("Interpolated ref power @ %dMHz = %d.%ddBuV (corrected by %d.%ddB)\n", 
                freqMHz, power / 10, power % 10, 
                CAL_5_4MHZ_TO_6MHZ_CORR_VALUE_x10 / 10, CAL_5_4MHZ_TO_6MHZ_CORR_VALUE_x10 % 10); 
      );
  }
  MXLEXITSTR(status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadAveragedPower
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/06/2012
--|
--| DESCRIPTION   : This function polls DFE channel lock status and returns once
--|                 it is locked and averaged power level is available or timeout 
--|                 occured.
--|              
--|
--| INPUTS        : device context, physical channel Id, pointer to power value,
--|                 timeout in ms 
--|
--| RETURN VALUE  : MXL_SUCCESS
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ReadAveragedPower(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
                  MXL_HRCLS_CHAN_ID_E phyChanId, 
                  UINT16 * powerPtr, UINT16 timeoutMs)
{
  MXL_STATUS_E status = MXL_SUCCESS; 
  MXL_HRCLS_RX_PWR_ACCURACY_E accuracy;
  UINT64 timeNow = 0, timeout;
  
  MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
  timeout = timeNow + (timeoutMs * 1000);
  do
  {
    MxLWare_HRCLS_OEM_DelayUsec(CAL_LOOP_ITERATION_WAIT_MS * 1000);  
    status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, phyChanId, powerPtr, &accuracy);
    MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
  } while ((accuracy != MXL_HRCLS_PWR_AVERAGED) && (timeNow < timeout) && (status == MXL_SUCCESS)); 

  MXLERR(
  if (status != MXL_SUCCESS)
  {
    MxL_HRCLS_ERROR("[HRCLS] %s error during reading power level ch%d\n", __FUNCTION__, phyChanId);
  }
  )
  if (accuracy != MXL_HRCLS_PWR_AVERAGED) 
  {
    MXLERR(MxL_HRCLS_ERROR("[HRCLS] %s timed out (%dms)\n", __FUNCTION__, timeoutMs););
    status = MXL_FAILURE;
  }
//  MxL_HRCLS_DEBUG("%d: %d\n", phyChanId, accuracy);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_RunCalibrationCycle
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/06/2012
--|
--| DESCRIPTION   : Starts tuning dfe channels to prevoiusly defined frequencies 
--|                 and reads power values at those points. This functions uses more
--|                 than one DFE channel in order to speed up the process.
--|              
--|
--| INPUTS        : device context, title mode index (to put result in correct array index) 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CalReadReferencePower(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
                                        MXL_HRCLS_CAL_DATA_T * calData)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 freqIndex = 0;
  UINT8 freqMax = calData->freqPointsCount;
  MXL_HRCLS_DSCAL_STATS_T calStats;
  UINT8 channelsInUse;

  MXLENTERSTR;
  calStats.calPointsCount = (MXL_HRCLS_AFE_TILT4 * calData->freqPointsCount);
  channelsInUse = MXL_MIN(devContextPtr->dfeMaxChannels, CAL_USE_MULTIPLE_DFE_CHANNELS); 
  MXLDBG2(MxL_HRCLS_DEBUG("channelsInUse=%d\n", channelsInUse););
  while ((freqIndex < freqMax) && (status == MXL_SUCCESS))
  {
    UINT8 i;

    for (i = 0; (i < channelsInUse) && ((freqIndex + i) < freqMax) && (status == MXL_SUCCESS); i++)
    {
      status = MxL_HRCLS_Ctrl_CfgTunerChanTune(devContextPtr, (MXL_HRCLS_CHAN_ID_E) i, 6, (UINT32) calData->freqPoints[freqIndex + i].freqMHz * 1000000); 
    }

    if (status == MXL_SUCCESS)
    {
      for (i = 0; (i < channelsInUse) && ((freqIndex + i) < freqMax) && (status == MXL_SUCCESS); i++)
      {
        UINT16 rawPower = 0;
        status = MxL_HRCLS_Ctrl_ReadAveragedPower(devContextPtr, (MXL_HRCLS_CHAN_ID_E) i, &rawPower, CAL_WAIT_FOR_AVERAGED_POWER_TIMEOUT_MS);
        
        if (status == MXL_SUCCESS) 
        {
          // Calculated power (read back from IC)
          calData->freqPoints[freqIndex+i].deltaPowerdB = rawPower;
          // Reference power (PG: Ri)
          calData->freqPoints[freqIndex+i].deltaPowerdB -= calData->freqPoints[freqIndex+i].refPowerdBuV;
          // C_PFF
          calData->freqPoints[freqIndex+i].deltaPowerdB -= MXL_DIV_ROUND(MxL_HRCLS_Ctrl_CalGetCPFF(calData->freqPoints[freqIndex + i].freqMHz), 10);
          // C_GTF
          if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
          {
            calData->freqPoints[freqIndex+i].deltaPowerdB -= MXL_DIV_ROUND(MxL_HRCLS_Ctrl_CalGetCGTF(devContextPtr, calData->freqPoints[freqIndex + i].freqMHz, devContextPtr->currentTemp), 10);
          }
          else
          {
            calData->freqPoints[freqIndex+i].deltaPowerdB -= MXL_DIV_ROUND(MxL_HRCLS_Ctrl_CalGetCGTF_V2(devContextPtr, calData->freqPoints[freqIndex + i].freqMHz, devContextPtr->currentTemp), 10);
          }

          MXLDBG2(MxL_HRCLS_DEBUG("freq=%dMHz, rawPower=%d, refPower=%d, delta=%d\n", calData->freqPoints[freqIndex + i].freqMHz, rawPower, calData->freqPoints[freqIndex+i].refPowerdBuV, calData->freqPoints[freqIndex+i].deltaPowerdB););

          calStats.currentFreqMHz = calData->freqPoints[freqIndex + i].freqMHz;
          calStats.currentCalPointIndex = (calData->currentTiltIndex * calData->freqPointsCount) + (freqIndex + i);
          if (calData->callbackFnPtr) status = (MXL_STATUS_E) calData->callbackFnPtr(devContextPtr->devId, MXL_HRCLS_CB_DS_CAL_STATS, (void *) &calStats);  
        }
        MXLERR(
        else
        {
          MxL_HRCLS_ERROR("Error during reading channel power for DS calibration\n");
        }
        )
      }
    }
    MXLERR(
    else
    {
      MxL_HRCLS_ERROR("Error during tuning channels for DS calibration\n");
    }
    )
    freqIndex += channelsInUse;
  }
  MXLEXITSTR(status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalFindFirstFreqPointInSegment
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/14/2012
--|
--| DESCRIPTION   : This function returnes index of the first frequency point that  
--|                 belongs to required segmentId.
--|
--| INPUTS        : segmentId, calData 
--|
--| RETURN VALUE  : index of first frequency in required segmentId. If not found,  
--|                 returns number of frequency points. No error value provided
--|-------------------------------------------------------------------------------------*/
static UINT8 MxL_HRCLS_Ctrl_CalFindFirstFreqPointInSegment(UINT8 segmentId, MXL_HRCLS_CAL_DATA_T * calData)
{
  UINT8 freqPointIdx = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("segmentId=%d\n", segmentId););
  while ((calData->freqPoints[freqPointIdx].segmentIdx != segmentId) && (freqPointIdx < calData->freqPointsCount)) freqPointIdx++;
  MXLEXITSTR(freqPointIdx);

  return freqPointIdx;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalProduceCoeffs
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/14/2012
--|
--| DESCRIPTION   : This function calculates coefficients C2,C1,C0 for power reporting 
--|                 correction. 
--|
--| INPUTS        : segmentId, calData, coeff (output data)
--|
--| RETURN VALUE  : MXL_SUCCESS 
--|                 
--|-------------------------------------------------------------------------------------*/
#if !defined(_MXL_HRCLS_FLOAT_CALC_ENABLED_) || defined(MXL_HRCLS_DS_CAL_TEST)
#if 0
MXL_STATUS_E MxL_HRCLS_Ctrl_CalProduceCoeffs(UINT8 segmentId, MXL_HRCLS_CAL_DATA_T * calData, MXL_HRCLS_CAL_COEFF_T * coeff)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 i;

  SINT32 X = 0;
  SINT32 Y = 0;
  SINT32 Z = 0;

  SINT32 A = 0; 
  SINT32 B = 0;
  SINT32 C = 0;
  SINT32 D = 0;
  SINT32 E = 0;

  SINT32 F1, F2, F3;
  SINT32 D1, D2, D3;

  SINT32 C0, C1, C2;

  MXLENTERSTR;

  coeff->c2 = 0;
  coeff->c1 = 0;
  coeff->c0 = 0;

  i = MxL_HRCLS_Ctrl_CalFindFirstFreqPointInSegment(segmentId, calData);

  while ((i < calData->freqPointsCount) && (calData->freqPoints[i].segmentIdx == segmentId))
  {
    SINT32 sample = 0;

//    MxL_HRCLS_DEBUG("deltaPowerdB=%d, normFreqx100=%d\n", freqPoints[i].deltaPowerdB, freqPoints[i].normFreqx100);
    sample = (SINT32) calData->freqPoints[i].deltaPowerdB;
    // X = sum(Oi[n])
    X += sample; 

    sample *= (SINT32) calData->freqPoints[i].normFreqx100;
    // Y = sum(Oi[n] * Xi[n])
    Y += sample;
    
    sample *= (SINT32) calData->freqPoints[i].normFreqx100;
    // Z = sum(Oi[n] * Xi[n] * Xi[n])
    Z += sample; 


    sample = (SINT32) calData->freqPoints[i].normFreqx100;
    // B = sum(Xi[n])
    B += sample;

    sample *= (SINT32) calData->freqPoints[i].normFreqx100;
    // C = sum(Xi[n] * Xi[n])
    C += sample;

    sample *= (SINT32) calData->freqPoints[i].normFreqx100;
    // D = sum(Xi[n] * Xi[n] * Xi[n])
    D += sample;
    
    sample *= (SINT32) calData->freqPoints[i].normFreqx100;
    // E = sum(Xi[n] * Xi[n] * Xi[n] * Xi[n])
    E += sample;

    A++;

    i++;
  }

  X *= 100;
  
  Z = MXL_DIV_ROUND(Z, 100);
  B *= 10;
  C = MXL_DIV_ROUND(C, 10);
  D = MXL_DIV_ROUND(D, 1000);
  E = MXL_DIV_ROUND(E, 100000);
  MXLDBG2(MxL_HRCLS_DEBUG("X=%d, Y=%d, Z=%d, A=%d, B=%d, C=%d, D=%d, E=%d\n", X, Y, Z, A, B, C, D, E););

  if (A)
  {
    F1 = MXL_DIV_ROUND(B, A);
    F2 = MXL_DIV_ROUND(C, A);
    F3 = MXL_DIV_ROUND((A*D*1000 - C*B), (A*C - MXL_DIV_ROUND((B*B), 1000)));

    D1 = A;
    D2 = C - MXL_DIV_ROUND(MXL_DIV_ROUND((B*B) , 1000) , A);
    D3 = E - MXL_DIV_ROUND(((C*C)/1000),  A) - ((A * D - (C * B)/1000) * (A * D - (C * B)/1000)) / (A * (A * C - (B * B)/1000)); 

    MXLDBG2(MxL_HRCLS_DEBUG("F1=%d, F2=%d, F3=%d, D1=%d, D2=%d, D3=%d\n", F1, F2, F3, D1, D2, D3);)

    C0 = X;
    C1 = Y - MXL_DIV_ROUND((F1 * C0) , 1000);  
    C2 = Z - MXL_DIV_ROUND((F2 * C0) , 1000) - MXL_DIV_ROUND((F3 * C1) , 1000); 
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d, C2=%d\n", C0, C1, C2););

    C0 = MXL_DIV_ROUND(C0 , D1);
    C1 = MXL_DIV_ROUND((C1 * 1000) , D2);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d\n", C0, C1););

/*
    C2 = MXL_DIV_ROUND((C2 * 10) , D3);
    C1 = MXL_DIV_ROUND(C1 , 100) - MXL_DIV_ROUND((F3 * C2) , 1000);
    C0 = MXL_DIV_ROUND(C0 , 100) - MXL_DIV_ROUND((F1 * C1) , 1000) - MXL_DIV_ROUND((F2 * C2) , 1000);
*/    
    C2 = MXL_DIV_ROUND((C2 * 100) , D3);
    C1 = MXL_DIV_ROUND(C1 , 10) - MXL_DIV_ROUND((F3 * C2) , 1000);
    C0 = MXL_DIV_ROUND(C0 , 10) - MXL_DIV_ROUND((F1 * C1) , 1000) - MXL_DIV_ROUND((F2 * C2) , 1000);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d, C2=%d\n", C0, C1, C2););

    coeff->c2 = C2;
    coeff->c1 = C1;
    coeff->c0 = C0;
    MXLDBG2(MxL_HRCLS_DEBUG("segment%d, c2=%d, c1=%d, c0=%d\n", segmentId, C2, C1, C0););
  }
  MXLEXITSTR(status);

  return status;
}
#else
MXL_STATUS_E MxL_HRCLS_Ctrl_CalProduceCoeffs(UINT8 segmentId, MXL_HRCLS_CAL_DATA_T * calData, MXL_HRCLS_CAL_COEFF_T * coeff)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 i;

  SINT32 X = 0;
  SINT32 Y = 0;
  SINT32 Z = 0;

  SINT32 A = 0; 
  SINT32 B = 0;
  SINT32 C = 0;
  SINT32 D = 0;
  SINT32 E = 0;

  SINT32 F1, F2, F3;
  SINT32 D1, D2, D3;

  SINT32 C0, C1, C2;

  coeff->c2 = 0;
  coeff->c1 = 0;
  coeff->c0 = 0;

  i = MxL_HRCLS_Ctrl_CalFindFirstFreqPointInSegment(segmentId, calData);

  // Allow one more frequency point per segment. So each segment have 6 frequencies including 0 and 1 (normalized)
  while ((i < calData->freqPointsCount) && ((calData->freqPoints[i].segmentIdx == segmentId) || ((i) && (calData->freqPoints[i-1].segmentIdx == segmentId))))
  {
    SINT32 sample = 0;
    SINT32 normFreqx100;

    // If it is an extra frequency point (1 normalized) than set it explicitly as 100. Otherwise it will be
    // treated as 0 (normalized) of the next segment.
    normFreqx100 = (calData->freqPoints[i].segmentIdx == segmentId)?calData->freqPoints[i].normFreqx100:100;

//    MxL_HRCLS_DEBUG("deltaPowerdB=%d, normFreqx100=%d\n", freqPoints[i].deltaPowerdB, freqPoints[i].normFreqx100);
    sample = (SINT32) calData->freqPoints[i].deltaPowerdB;
    // X = sum(Oi[n])
    X += sample; 

    sample *= normFreqx100; 
    // Y = sum(Oi[n] * Xi[n])
    Y += sample;
    
    sample *= normFreqx100; 
    // Z = sum(Oi[n] * Xi[n] * Xi[n])
    Z += sample; 


    sample = normFreqx100; 
    // B = sum(Xi[n])
    B += sample;

    sample *= normFreqx100; 
    // C = sum(Xi[n] * Xi[n])
    C += sample;

    sample *= normFreqx100; 
    // D = sum(Xi[n] * Xi[n] * Xi[n])
    D += sample;
    
    sample *= normFreqx100; 
    // E = sum(Xi[n] * Xi[n] * Xi[n] * Xi[n])
    E += sample;

    A++;

    i++;
  }

  X *= 1000;
  Y *= 10;  
  Z = MXL_DIV_ROUND_S(Z, 10);
  B *= 100;
//  C = MXL_DIV_ROUND(C, 10);
  D = MXL_DIV_ROUND_S(D, 100);
  E = MXL_DIV_ROUND_S(E, 10000);
  MXLDBG2(MxL_HRCLS_DEBUG("X=%d, Y=%d, Z=%d, A=%d, B=%d, C=%d, D=%d, E=%d\n", X, Y, Z, A, B, C, D, E););

  if (A)
  {
    F1 = MXL_DIV_ROUND_S(B, A);
    F2 = MXL_DIV_ROUND_S(C, A);
    F3 = MXL_DIV_ROUND_S((A*D*10000 - C*B), MXL_DIV_ROUND_S((A*C*10000 - (B*B)), 10000));

    D1 = A;
    D2 = MXL_DIV_ROUND_S((C*10000 - MXL_DIV_ROUND_S((B*B) , A)), 10000);
    D3 = E - MXL_DIV_ROUND_S(MXL_DIV_ROUND_S((C*C), 10000),  A) - ((A * D - MXL_DIV_ROUND_S((C * B),10000)) * (A * D - MXL_DIV_ROUND_S((C * B),10000))) / (A * (A * C - MXL_DIV_ROUND_S((B * B),10000))); 

    MXLDBG2(MxL_HRCLS_DEBUG("F1=%d, F2=%d, F3=%d, D1=%d, D2=%d, D3=%d\n", F1, F2, F3, D1, D2, D3);)

    C0 = X;
    C1 = MXL_DIV_ROUND_S((Y*10000 - (F1 * C0)), 10000);  
    C2 = MXL_DIV_ROUND_S((Z*10000 - (F2 * C0) - (F3 * C1)), 10000);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d, C2=%d\n", C0, C1, C2););

    C0 = MXL_DIV_ROUND_S(C0 , D1);
    C1 = MXL_DIV_ROUND_S((C1 * 10000) , D2);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d\n", C0, C1););

    C2 = MXL_DIV_ROUND_S((C2 * 100) , D3);
//    C1 = MXL_DIV_ROUND(C1 , 1000) - MXL_DIV_ROUND((F3 * C2) , 10000000);
    C1 = MXL_DIV_ROUND_S(C1 - MXL_DIV_ROUND_S((F3 * C2) , 100), 100);
//    C0 = MXL_DIV_ROUND(C0 , 10000) + MXL_DIV_ROUND((-(F1 * C1) - (F2 * C2)), 1000);
    C0 = MXL_DIV_ROUND_S(C0 - MXL_DIV_ROUND_S(((F1 * C1) + (F2 * C2)), 100), 100);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%d, C1=%d, C2=%d\n", C0, C1, C2););

    coeff->c2 = C2;
    coeff->c1 = C1;
    coeff->c0 = C0;
//    MXLDBG2(MxL_HRCLS_DEBUG("segment%d, c2=%d, c1=%d, c0=%d\n", segmentId, C2, C1, C0););
  }

  return status;
}
#endif
#endif

#ifdef _MXL_HRCLS_FLOAT_CALC_ENABLED_
#ifdef MXL_HRCLS_DS_CAL_TEST
MXL_STATUS_E MxL_HRCLS_Ctrl_CalProduceCoeffsFloat(UINT8 segmentId, MXL_HRCLS_CAL_DATA_T * calData, MXL_HRCLS_CAL_COEFF_T * coeff)
#else
MXL_STATUS_E MxL_HRCLS_Ctrl_CalProduceCoeffs(UINT8 segmentId, MXL_HRCLS_CAL_DATA_T * calData, MXL_HRCLS_CAL_COEFF_T * coeff)
#endif
{
  MXL_STATUS_E status = 0;
  UINT8 i;

  REAL32 X = 0;
  REAL32 Y = 0;
  REAL32 Z = 0;

  REAL32 A = 0; 
  REAL32 B = 0;
  REAL32 C = 0;
  REAL32 D = 0;
  REAL32 E = 0;

  REAL32 F1, F2, F3;
  REAL32 D1, D2, D3;

  REAL32 C0, C1, C2;

  coeff->c2 = 0;
  coeff->c1 = 0;
  coeff->c0 = 0;

  i = MxL_HRCLS_Ctrl_CalFindFirstFreqPointInSegment(segmentId, calData);

  while ((i < calData->freqPointsCount) && (calData->freqPoints[i].segmentIdx == segmentId))
  {
    REAL32 sample = calData->freqPoints[i].deltaPowerdB / 10.0;
    REAL32 normFreq = (REAL32) calData->freqPoints[i].normFreqx100 / 100.0;

//    MxL_HRCLS_DEBUG("deltaPowerdB=%d, normFreqx100=%d, sample=%3.4f, normFreq=%3.4f\n", freqPoints[i].deltaPowerdB, freqPoints[i].normFreqx100, sample, normFreq);

    // X = sum(Oi[n])
    X += sample; 

    sample *= normFreq;
    // Y = sum(Oi[n] * Xi[n])
    Y += sample;
    
    sample *= normFreq; 
    // Z = sum(Oi[n] * Xi[n] * Xi[n])
    Z += sample; 


    sample = normFreq; 
    // B = sum(Xi[n])
    B += sample;

    sample *= normFreq;
    // C = sum(Xi[n] * Xi[n])
    C += sample;

    sample *= normFreq; 
    // D = sum(Xi[n] * Xi[n] * Xi[n])
    D += sample;
    
    sample *= normFreq; 
    // E = sum(Xi[n] * Xi[n] * Xi[n] * Xi[n])
    E += sample;

    A++;

    i++;
  }

  MXLDBG2(MxL_HRCLS_DEBUG("X=%3.4f, Y=%3.4f, Z=%3.4f, A=%3.4f, B=%3.4f, C=%3.4f, D=%3.4f, E=%3.4f\n", X, Y, Z, A, B, C, D, E););

  if (A)
  {
    F1 = B / A;
    F2 = C / A;
    F3 = (A*D - C*B) / (A*C - B*B);

    D1 = A;
    D2 = C - (B*B) / A;
    D3 = E - (C*C) / A - ((A * D - C * B) * (A * D - C * B)) / (A * (A * C - B * B)); // TODO: check ranges

    MXLDBG2(MxL_HRCLS_DEBUG("F1=%3.4f, F2=%3.4f, F3=%3.4f, D1=%3.4f, D2=%3.4f, D3=%3.4f\n", F1, F2, F3, D1, D2, D3););

    C0 = X;
    C1 = Y - (F1 * C0);  
    C2 = Z - (F2 * C0) - (F3 * C1); 
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%3.4f, C1=%3.4f, C2=%3.4f\n", C0, C1, C2););

    C0 = C0 / D1;
    C1 = C1 / D2;
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%3.4f, C1=%3.4f\n", C0, C1););

    C2 = C2 / D3;
    C1 = C1 - (F3 * C2);
    C0 = C0 - (F1 * C1) - (F2 * C2);
    MXLDBG2(MxL_HRCLS_DEBUG("C0=%3.4f, C1=%3.4f, C2=%3.4f\n", C0, C1, C2););

    coeff->c2 = (SINT32) (C2 * 100.0);
    coeff->c1 = (SINT32) (C1 * 100.0); 
    coeff->c0 = (SINT32) (C0 * 100.0);
  }

  return status;
}
#endif

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalSaveCoeffsToFile
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/14/2012
--|
--| DESCRIPTION   : Save calibration data to file 
--|
--| INPUTS        : devContextPtr 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE 
--|                 
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CalSaveCoeffsToFile(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_PWR_REPORTING_DATA_T * fileBuffer = &devContextPtr->pwrReportData;
  UINT8 checksum = 0;
  UINT16 i;

  MXLENTERSTR;
  fileBuffer->fileSignature = 0;
  fileBuffer->fileDataFormat = 0;
  fileBuffer->fileRecordSize = sizeof(MXL_HRCLS_CAL_COEFF_T);
  fileBuffer->fileRecordsCnt = MXL_HRCLS_CAL_TILT_MODES_COUNT * MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT;
  fileBuffer->fileCheckSum = 0;
  fileBuffer->dataLoaded = MXL_TRUE;

  for (i = 0; i < sizeof(MXL_HRCLS_PWR_REPORTING_DATA_T); i++) checksum += ((UINT8 *) fileBuffer)[i];
  fileBuffer->fileCheckSum = checksum;

  status = MxLWare_HRCLS_OEM_SaveNVRAMFile(devContextPtr->devId, (UINT8 *) fileBuffer, sizeof(MXL_HRCLS_PWR_REPORTING_DATA_T));
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalReadCoeffsFromFile
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/14/2012
--|
--| DESCRIPTION   : This function reads calibration data from nvram file 
--|                 
--| INPUTS        : devContextPtr 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|                 
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CalReadCoeffsFromFile(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_PWR_REPORTING_DATA_T * fileBuffer = &devContextPtr->pwrReportData;

  MXLENTERSTR;
  status = MxLWare_HRCLS_OEM_LoadNVRAMFile(devContextPtr->devId, (UINT8 *) fileBuffer, sizeof(MXL_HRCLS_PWR_REPORTING_DATA_T));
  if (status == MXL_SUCCESS)
  {
    UINT8 checkSum;
    UINT32 i;

    checkSum = fileBuffer->fileCheckSum;
    fileBuffer->fileCheckSum = 0;
  
    for (i = 0; i < sizeof(MXL_HRCLS_PWR_REPORTING_DATA_T); i++) checkSum -= ((UINT8 *) fileBuffer)[i];

    if (checkSum) status = MXL_INVALID_PARAMETER;
  }
  MXLEXITSTR(status);

  return status;
}

