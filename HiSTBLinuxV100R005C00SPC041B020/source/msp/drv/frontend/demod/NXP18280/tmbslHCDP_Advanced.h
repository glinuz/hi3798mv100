/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP_Advanced.h
 *
 *                %version: 3 %
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/

#pragma once
#ifndef _TMBSL_HCDP_ADVANCED_H
#define _TMBSL_HCDP_ADVANCED_H

#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

typedef enum _HCDP_ConstellationSource
{
    HCDP_ConstellationSource_ADC = 0,
    HCDP_ConstellationSource_FEDR,
    HCDP_ConstellationSource_PDF,
    HCDP_ConstellationSource_DAGC,
    HCDP_ConstellationSource_MF,
    HCDP_ConstellationSource_CAGC,
    HCDP_ConstellationSource_Equalizer,
    HCDP_ConstellationSource_BEDR /* default */
} HCDP_ConstellationSource_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/    
    
tmErrorCode_t
tmbslHCDP_SetClocks(
    tmUnitSelect_t tUnit,
    UInt32 uSamplingClock,
    UInt32 uDClock
);

tmErrorCode_t
tmbslHCDP_GetClocks(
    tmUnitSelect_t tUnit,
    UInt32 *puSamplingClock,
    UInt32 *puDClock
);

tmErrorCode_t
tmbslHCDP_SetIFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 uIFAGCThr
);

tmErrorCode_t
tmbslHCDP_GetIFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 *puIFAGCThr
);

tmErrorCode_t
tmbslHCDP_SetRFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 uRFAGCThrLow,
    UInt16 uRFAGCThrHigh
);

tmErrorCode_t
tmbslHCDP_ConstPreset(
    tmUnitSelect_t tUnit,
    HCDP_ConstellationSource_t eSource
);

tmErrorCode_t
tmbslHCDP_ConstGetIQ(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *pI,
    tmFrontEndFracNb32_t *pQ
);

tmErrorCode_t
tmbslHCDP_FFTPreset(
    tmUnitSelect_t tUnit
);

tmErrorCode_t
tmbslHCDP_FFTGetPoint(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t  *pY
);

tmErrorCode_t
tmbslHCDP_EqualizerPreset(
    tmUnitSelect_t tUnit
);

tmErrorCode_t
tmbslHCDP_EqualizerGetPoint(
    tmUnitSelect_t tUnit,
    UInt8 uTapIndex,
    tmFrontEndFracNb32_t *pI,
    tmFrontEndFracNb32_t *pQ
);

tmErrorCode_t
tmbslHCDP_GetSignalLevel(
    tmUnitSelect_t tUnit,
    UInt16 *puLevel
);

#ifdef __cplusplus
}
#endif  /*__cplusplus*/

#endif /*_TMBSL_HCDP_ADVANCED_H*/
