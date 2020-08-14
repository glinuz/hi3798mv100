/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP.h
 *
 *                %version: 8 %
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
#ifndef _TMBSL_HCDP_H
#define _TMBSL_HCDP_H

#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/


/*============================================================================*/
/* HCDP Error Codes                                                           */
/*============================================================================*/

#define HCDP_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define HCDP_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define HCDP_ERR_BAD_UNIT_NUMBER            (HCDP_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define HCDP_ERR_ERR_NO_INSTANCES           (HCDP_ERR_BASE + TM_ERR_NO_INSTANCES)
#define HCDP_ERR_NOT_INITIALIZED            (HCDP_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define HCDP_ERR_ALREADY_SETUP              (HCDP_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define HCDP_ERR_INIT_FAILED                (HCDP_ERR_BASE + TM_ERR_INIT_FAILED)
#define HCDP_ERR_BAD_PARAMETER              (HCDP_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define HCDP_ERR_NOT_SUPPORTED              (HCDP_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define HCDP_ERR_NULL_CONTROLFUNC           (HCDP_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define HCDP_ERR_HW_FAILED                  (HCDP_ERR_COMP + 0x0001)
#define HCDP_ERR_NOT_READY                  (HCDP_ERR_COMP + 0x0002)
#define HCDP_ERR_BAD_VERSION                (HCDP_ERR_COMP + 0x0003)
#define HCDP_ERR_STD_NOT_SET                (HCDP_ERR_COMP + 0x0004)
#define HCDP_ERR_RF_NOT_SET                 (HCDP_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

typedef enum _HCDP_HwCdpSample_t
{
    HCDP_SAMPLE_ES1 = 0,
    HCDP_SAMPLE_ES2,
    HCDP_SAMPLE_ES3,
    HCDP_SAMPLE_UNKNOWN
}HCDP_HwCdpSample_t;

typedef struct _HCDP_LockInd
{
    Bool bCagcSat;
    Bool bDagcSat;
    Bool bAagc;
    Bool bStl;
    Bool bCtl;
    Bool bSctl;
    Bool bDemod;
    Bool bFEC;
    UInt16 uDemodLockTime;
    UInt16 uFecLockTime;
} HCDP_LockInd_t;

typedef enum _HCDP_BERWindow
{
    HCDP_BERWindow_Unknown = 0,
    HCDP_BERWindow_1E5,       /*1e5 bits*/
    HCDP_BERWindow_1E6,       /*1e6 bits*/
    HCDP_BERWindow_1E7,       /*1e7 bits*/
    HCDP_BERWindow_1E8,       /*1e8 bits*/
    HCDP_BERWindow_Max
} HCDP_BERWindow_t;

typedef enum _HCDP_ExtendSymbolRateMode_t /* for extended symbol rate recovery range */
{
    HCDP_ExtendSymbolRateModeDisable,
    HCDP_ExtendSymbolRateModeEnable700ppm,
    HCDP_ExtendSymbolRateModeEnable1500ppm, /* costly in lock time, till 1 sec */
    HCDP_ExtendSymbolRateModeInvalid
} HCDP_ExtendSymbolRateMode_t;


typedef enum _HCDP_InterleaverMode
 {
    /*Nxa*/
    HCDP_InterleaverMode_12_17 = 0,
    /*Nxb*/
    HCDP_InterleaverMode_128_1_A,
    HCDP_InterleaverMode_128_1_B,
    HCDP_InterleaverMode_128_2,
    HCDP_InterleaverMode_128_3,
    HCDP_InterleaverMode_128_4,
    HCDP_InterleaverMode_64_2,
    HCDP_InterleaverMode_32_4,
    HCDP_InterleaverMode_16_8,
    HCDP_InterleaverMode_8_16,
    HCDP_InterleaverMode_NA
} HCDP_InterleaverMode_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslHCDP_Init(
    tmUnitSelect_t tUnit,
    const tmbslFrontEndDependency_t *psSrvFunc
);

tmErrorCode_t
tmbslHCDP_DeInit(
    tmUnitSelect_t tUnit
);

tmErrorCode_t     
tmbslHCDP_GetSWVersion(
    ptmSWVersion_t pSWVersion
);

tmErrorCode_t
tmbslHCDP_GetCdpHwSample(
    tmUnitSelect_t tUnit,
    HCDP_HwCdpSample_t *peCdpHwSample
);

tmErrorCode_t
tmbslHCDP_Reset(
    tmUnitSelect_t tUnit
);

tmErrorCode_t
tmbslHCDP_StartLock(
    tmUnitSelect_t tUnit
);

tmErrorCode_t
tmbslHCDP_GetLockIndicators(
    tmUnitSelect_t tUnit,
    HCDP_LockInd_t *psLockInd
);

tmErrorCode_t
tmbslHCDP_GetLockStatus(
    tmUnitSelect_t tUnit,
    tmbslFrontEndState_t *peLockStatus
);

tmErrorCode_t
tmbslHCDP_GetAAGCAcc(
    tmUnitSelect_t tUnit,
    UInt16 *puIfAGC,
    UInt16 *puRFAGC
);

tmErrorCode_t
tmbslHCDP_SetMod(
    tmUnitSelect_t tUnit,
    tmFrontEndModulation_t eMod
);

tmErrorCode_t
tmbslHCDP_GetMod(
    tmUnitSelect_t tUnit,
    tmFrontEndModulation_t *peMod
);

tmErrorCode_t
tmbslHCDP_SetSR(
    tmUnitSelect_t tUnit,
    UInt32 uSR
);

tmErrorCode_t
tmbslHCDP_GetSR(
    tmUnitSelect_t tUnit,
    UInt32 *puSR
);

tmErrorCode_t
tmbslHCDP_GetConfiguredSR(
    tmUnitSelect_t tUnit,
    UInt32 *puSR
);

tmErrorCode_t
tmbslHCDP_SetFECMode(
    tmUnitSelect_t tUnit,
    tmFrontEndFECMode_t eFECMode
);

tmErrorCode_t
tmbslHCDP_GetFECMode(
    tmUnitSelect_t tUnit,
    tmFrontEndFECMode_t *peFECMode
);

tmErrorCode_t
tmbslHCDP_SetSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t eSI
);

tmErrorCode_t
tmbslHCDP_GetSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t *peSI
);

tmErrorCode_t
tmbslHCDP_GetConfiguredSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t *peSI
);

tmErrorCode_t
tmbslHCDP_GetChannelEsNo(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t  *psEsno
);

tmErrorCode_t
tmbslHCDP_GetBER(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *psBER,
    UInt32 *puUncors
);

tmErrorCode_t
tmbslHCDP_SetBERWindow(
    tmUnitSelect_t tUnit,
    HCDP_BERWindow_t eBERWindow
);

tmErrorCode_t
tmbslHCDP_GetBERWindow(
    tmUnitSelect_t tUnit,
    HCDP_BERWindow_t *peBERWindow
);

tmErrorCode_t
tmbslHCDP_GetLockTime(
    tmUnitSelect_t tUnit,
    UInt16 *uDemodLockTime,
    UInt16 *uFecLockTime
);

tmErrorCode_t
tmbslHCDP_GetFreqOffset(
    tmUnitSelect_t tUnit,
    Int32 *plFreqOffsetHz
);

tmErrorCode_t
tmbslHCDP_GetSignalToISI(
    tmUnitSelect_t tUnit,
    UInt16 *puSignalToISI,
    UInt16 *puEqCenterTapGain
);

tmErrorCode_t
tmbslHCDP_GetSignalToInterference(
    tmUnitSelect_t tUnit,
    UInt16 *puSignalToInterference,
    UInt16 *puEqCenterTapGain
);

tmErrorCode_t
tmbslHCDP_SetIF(
    tmUnitSelect_t tUnit,
    UInt32 uIF
);

tmErrorCode_t
tmbslHCDP_GetConfiguredIF(
    tmUnitSelect_t tUnit,
    UInt32 *puIFFreq
);

tmErrorCode_t
tmbslHCDP_GetIFOffset(
    tmUnitSelect_t tUnit,
    Int32 *plIFOffset
);

tmErrorCode_t
tmbslHCDP_ClearUncor(
    tmUnitSelect_t tUnit
);

tmErrorCode_t
tmbslHCDP_GetSignalQuality(
    tmUnitSelect_t tUnit,
    UInt32 *plSignalQuality
);

tmErrorCode_t
tmbslHCDP_SetExtendSymbolRateRange(
    tmUnitSelect_t tUnit,
    HCDP_ExtendSymbolRateMode_t eMode
);


tmErrorCode_t
tmbslHCDP_GetExtendSymbolRateRange(
    tmUnitSelect_t tUnit,
    HCDP_ExtendSymbolRateMode_t *peMode
);

tmErrorCode_t
tmbslHCDP_GetActualInterleaverMode(
    tmUnitSelect_t tUnit,
    HCDP_InterleaverMode_t *peInterleaverMode
);

tmErrorCode_t
tmbslHCDP_SelectEqualizerSettings(
    tmUnitSelect_t tUnit,
    UInt16 uEqFfLeakageAlpha_BeforeLock,
    UInt16 uEqFfLeakageAlpha_AfterLock
);

tmErrorCode_t
tmbslHCDP_SetAfterLock(
    tmUnitSelect_t tUnit
);

#ifdef __cplusplus
}
#endif 

#endif /*_TMBSL_HCDP_H*/
