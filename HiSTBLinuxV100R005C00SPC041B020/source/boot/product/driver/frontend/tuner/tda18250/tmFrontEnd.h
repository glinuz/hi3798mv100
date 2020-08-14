/**
  Copyright (C) 2007 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmFrontEnd.h
 *                %version: CFR_FEAP#1 %
 *
 * \date          %date_modified%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
 * \verbatim
   Date          Modified by CRPRNr  TASKNr  Maintenance description
   -------------|-----------|-------|-------|-----------------------------------
    26-Mar-2008 | B.GUILLOT | 13122 | 23456 | Creation
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/

#ifndef TMFRONTEND_H
#define TMFRONTEND_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/

/* standard*/
typedef enum _tmFrontEndStandard_t {
    tmFrontEndStandardDVBT = 0,
    tmFrontEndStandardDVBS,
    tmFrontEndStandardDVBC,
    tmFrontEndStandardDSS,
    tmFrontEndStandardBSD,
    tmFrontEndStandardDVBH,
    tmFrontEndStandardAnalogDVBT,
    tmFrontEndStandardDVBS2,
    tmFrontEndStandardDVBT2,
    tmFrontEndStandardMax
} tmFrontEndStandard_t, *ptmFrontEndStandard_t;

/* spectral inversion*/
typedef enum _tmFrontEndSpecInv_t {
    tmFrontEndSpecInvAuto = 0,
    tmFrontEndSpecInvOff,
    tmFrontEndSpecInvOn,
    tmFrontEndSpecInvMax
} tmFrontEndSpecInv_t, *ptmFrontEndSpecInv_t;

/* modulation*/
typedef enum _tmFrontEndModulation_t {
    tmFrontEndModulationAuto = 0,
    tmFrontEndModulationBpsk,
    tmFrontEndModulationQpsk,
    tmFrontEndModulationQam4,
    tmFrontEndModulationPsk8,
    tmFrontEndModulationQam16,
    tmFrontEndModulationQam32,
    tmFrontEndModulationQam64,
    tmFrontEndModulationQam128,
    tmFrontEndModulationQam256,
    tmFrontEndModulationQam512,
    tmFrontEndModulationQam1024,
    tmFrontEndModulation8VSB,
    tmFrontEndModulation16VSB,
    tmFrontEndModulationQam,
    tmFrontEndModulationMax
} tmFrontEndModulation_t, *ptmFrontEndModulation_t;

/* viterbi rate*/
typedef enum _tmFrontEndCodeRate_t {
    tmFrontEndCodeRateAuto = 0,
    tmFrontEndCodeRate_1_4,
    tmFrontEndCodeRate_1_3,
    tmFrontEndCodeRate_2_5,
    tmFrontEndCodeRate_1_2,
    tmFrontEndCodeRate_3_5,
    tmFrontEndCodeRate_2_3,
    tmFrontEndCodeRate_3_4,
    tmFrontEndCodeRate_4_5,
    tmFrontEndCodeRate_5_6,
    tmFrontEndCodeRate_6_7,
    tmFrontEndCodeRate_7_8,
    tmFrontEndCodeRate_8_9,
    tmFrontEndCodeRate_9_10,
    tmFrontEndCodeRate_NotRelevant,
    tmFrontEndCodeRateMax
} tmFrontEndCodeRate_t, *ptmFrontEndCodeRate_t;

/* frequency offset*/
typedef enum _tmFrontEndRfOffset_t {
    tmFrontEndRfOffsetAuto = 0,
    tmFrontEndRfOffsetNull,
    tmFrontEndRfOffsetPlus125,
    tmFrontEndRfOffsetMinus125,
    tmFrontEndRfOffsetPlus166,
    tmFrontEndRfOffsetMinus166,
    tmFrontEndRfOffsetPlus333,
    tmFrontEndRfOffsetMinus333,
    tmFrontEndRfOffsetPlus500,
    tmFrontEndRfOffsetMinus500,
    tmFrontEndRfOffsetMax
} tmFrontEndRfOffset_t, *ptmFrontEndRfOffset_t;

/* frequency offset*/
typedef enum _tmFrontEndRfOffsetMode_t {
    tmFrontEndRfOffsetModeAuto,
    tmFrontEndRfOffsetModeManual,
    tmFrontEndRfOffsetModeMax
} tmFrontEndRfOffsetMode_t, *ptmFrontEndRfOffsetMode_t;

/* guard interval*/
typedef enum _tmFrontEndGI_t {
    tmFrontEndGIAuto = 0,
    tmFrontEndGI_1_32,
    tmFrontEndGI_1_16,
    tmFrontEndGI_1_8,
    tmFrontEndGI_1_4,
    tmFrontEndGI_1_128,
    tmFrontEndGI_19_128,
    tmFrontEndGI_19_256,
    tmFrontEndGIMax
} tmFrontEndGI_t, *ptmFrontEndGI_t;

/* fast Fourrier transform size*/
typedef enum _tmFrontEndFft_t {
    tmFrontEndFftAuto = 0,
    tmFrontEndFft2K,
    tmFrontEndFft8K,
    tmFrontEndFft4K,
    tmFrontEndFft16K,
    tmFrontEndFft32K,
    tmFrontEndFftMax
} tmFrontEndFft_t, *ptmFrontEndFft_t;

/* hierarchy*/
typedef enum _tmFrontEndHier_t {
    tmFrontEndHierAuto = 0,
    tmFrontEndHierNo,
    tmFrontEndHierAlpha1,
    tmFrontEndHierAlpha2,
    tmFrontEndHierAlpha4,
    tmFrontEndHierMax
} tmFrontEndHier_t, *ptmFrontEndHier_t;

/* priority*/
typedef enum _tmFrontEndPrior_t {
    tmFrontEndPriorAuto = 0,
    tmFrontEndPriorHigh,
    tmFrontEndPriorLow,
    tmFrontEndPriorMax
} tmFrontEndPrior_t, *ptmFrontEndPrior_t;

/* roll off */
typedef enum _tmFrontEndRollOff_t {
    tmFrontEndRollOffAuto = 0,
    tmFrontEndRollOff_0_15,
    tmFrontEndRollOff_0_20,
    tmFrontEndRollOff_0_25,
    tmFrontEndRollOff_0_35,
    tmFrontEndRollOffMax
} tmFrontEndRollOff_t, *ptmFrontEndRollOff_t;

/* continuous tone */
typedef enum _tmFrontEndContinuousTone_t {
    tmFrontEndContinuousToneAuto = 0,
    tmFrontEndContinuousToneOff,
    tmFrontEndContinuousTone22KHz,
    tmFrontEndContinuousToneMax
} tmFrontEndContinuousTone_t, *ptmFrontEndContinuousTone_t;

typedef enum _tmFrontEndChannelType_t
{
    tmFrontEndChannelTypeNone    = 0x00,  /* No detection         */
    tmFrontEndChannelTypeDigital = 0x01,  /* Digital channel      */
    tmFrontEndChannelTypeAnalog  = 0x02,  /* Analog channel       */
    tmFrontEndChannelTypeUnknown = 0x20   /* unknown channel type */
} tmFrontEndChannelType_t;

typedef enum _tmFrontEndChannelConfidence_t
{
    tmFrontEndConfidenceNotAvailable,
    tmFrontEndConfidenceNull,
    tmFrontEndConfidenceLow,
    tmFrontEndConfidenceMedium,
    tmFrontEndConfidenceHigh
} tmFrontEndConfidence_t;

/* not used */
typedef enum _tmFrontEndDVBT2PLPType_t
{
    tmFrontEndDVBT2PLPTypeAuto,
    tmFrontEndDVBT2PLPTypeCommon,
    tmFrontEndDVBT2PLPType1,
    tmFrontEndDVBT2PLPType2,
    tmFrontEndDVBT2PLPTypeMax
} tmFrontEndDVBT2PLPType_t;

/* not used */
typedef enum _tmFrontEndDVBT2PLPPayloadType_t
{
    tmFrontEndDVBT2PLPPayloadTypeAuto,
    tmFrontEndDVBT2PLPPayloadTypeGFPS,
    tmFrontEndDVBT2PLPPayloadTypeGCS,
    tmFrontEndDVBT2PLPPayloadTypeGSE,
    tmFrontEndDVBT2PLPPayloadTypeTS,
    tmFrontEndDVBT2PLPPayloadTypeMax
} tmFrontEndDVBT2PLPPayloadType_t;

/* not used */
typedef enum _tmFrontEndDVBT2FECType_t
{
    tmFrontEndDVBT2FECTypeAuto,
    tmFrontEndDVBT2FECType16K,
    tmFrontEndDVBT2FECType64K,
    tmFrontEndDVBT2FECTypeMax
} tmFrontEndDVBT2FECType_t;

typedef enum _tmFrontEndRotationState_t
{
    tmFrontEndRotationStateAuto,
    tmFrontEndRotationStateOn,
    tmFrontEndRotationStateOff,
    tmFrontEndRotationStateMax
} tmFrontEndRotationState;

typedef enum _tmFrontEndDVBT2InputType_t
{
    tmFrontEndDVBT2InputTypeAuto,
    tmFrontEndDVBT2InputTypeSISO,
    tmFrontEndDVBT2InputTypeMISO,
    tmFrontEndDVBT2InputTypeMax
} tmFrontEndDVBT2InputType_t;

typedef enum _tmFrontEndFECMode_t
{
    tmFrontEndFECModeUnknown = 0,
    tmFrontEndFECModeAnnexA,
    tmFrontEndFECModeAnnexB,
    tmFrontEndFECModeAnnexC,
    tmFrontEndFECModeMax
} tmFrontEndFECMode_t;

typedef struct _tmFrontEndFracNb8_t
{
    Int8 lInteger;
    UInt8 uDivider;
}tmFrontEndFracNb8_t;

typedef struct _tmFrontEndFracNb32_t
{
    Int32 lInteger;
    UInt32 uDivider;
}tmFrontEndFracNb32_t;

#ifdef __cplusplus
}
#endif

#endif /* TMFRONTEND_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
