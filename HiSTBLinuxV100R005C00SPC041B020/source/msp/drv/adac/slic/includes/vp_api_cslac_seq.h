/** \file vp_api_cslac_seq.h
 * vp_api_cslac_seq.h
 *
 * Header file for the API-II c files.
 *
 * This file contains the all of the VoicePath API-II function prototypes
 * required to run the CSLAC sequencer.
 *
 * $Revision: 7340 $
 * $LastChangedDate: 2010-06-14 17:47:28 -0500 (Mon, 14 Jun 2010) $
 */

#ifndef VP_API_CSLAC_SEQ_H
#define VP_API_CSLAC_SEQ_H

#include "vp_CSLAC_types.h"

#ifdef VP_CSLAC_SEQ_EN
VpStatusType
VpSeq(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pProfile);

VpStatusType
VpBranchInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

VpStatusType
VpTimeInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

bool
VpCSLACHowlerInit(
    uint8 toneType,
    VpSeqDataType *cadence);

void
InitCadenceVars(
    VpLineCtxType *pLineCtx);

bool
VpServiceSeq(
    VpDevCtxType *pDevCtx);

VpStatusType
VpCidSeq(
    VpLineCtxType *pLineCtx);

void
VpCliStopCli(
    VpLineCtxType *pLineCtx);

VpStatusType
VpCSLACInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);

VpStatusType
VpCSLACStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);

#if defined (VP_CC_790_SERIES)
VpStatusType
Vp790CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);
#endif

#if defined (VP_CC_880_SERIES)
VpStatusType
Vp880CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);
#endif

#if defined (VP_CC_580_SERIES)
VpStatusType
Vp580CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);
#endif

#endif

#endif


