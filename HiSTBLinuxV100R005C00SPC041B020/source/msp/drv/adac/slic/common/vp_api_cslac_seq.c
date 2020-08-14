/** \file vp_api_cslac_seq.c
 * vp_api_cslac_seq.c
 *
 *  This file contains functions that are required to run the CSLAC sequencer.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7361 $
 * $LastChangedDate: 2010-06-17 15:31:18 -0500 (Thu, 17 Jun 2010) $
 */

#include "vp_api_cfg.h"
#ifdef VP_CSLAC_SEQ_EN

/* INCLUDES */
#include "vp_api.h"     /* Typedefs and function prototypes for API */
#include "vp_api_cslac_seq.h"
#include "vp_api_int.h" /* Device specific typedefs and function prototypes */
#include "sys_service.h"

#if defined (VP_CC_790_SERIES) || \
    (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))

static bool
VpFSKGeneratorReady(
    VpLineCtxType *pLineCtx);

static bool
VpDTMFGeneratorReady(
    VpLineCtxType *pLineCtx);

static VpStatusType
VpCtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

static bool
VpCliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

static void
VpCtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 digit);

static VpDigitType
VpConvertCharToDigitType(
    char digit);

static void
VpCtrlSetDTMFGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);

static VpStatusType
VpCtrlDetectDTMF(
    VpLineCtxType *pLineCtx,
    bool mode);
#endif

static void
VpCtrlMuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

/**
 * VpSeq()
 *  This function calls the appropriate sequencer function based on the current
 * position in the sequencer and the device type.
 *
 * Preconditions:
 *  The profile passed must be pointing to an instruction that is supported by
 * the device type.
 *
 * Postconditions:
 *  The instruction specified by the profile data is called.  The line context
 * is passed to the called function.  Note:  The line context may be valid or
 * VP_NULL, this function is not affected.  This function returns the success
 * code as long as the pointer is pointing to an instruction that is supported
 * by the device.
 */
VpStatusType
VpSeq(
    VpLineCtxType *pLineCtx,    /**< Line that has an active sequencer */
    VpProfilePtrType pProfile)  /**< Sequence profile, pointing to current
                                 * location in sequence, not typically the
                                 * starting address
                                 */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;

    VP_SEQUENCER(VpLineCtxType, pLineCtx, ("+VpSeq()"));

    /*
     * This function is passed a pointer that starts at the current position of
     * the cadence sequence, controlled by the API
     */
    if (pProfile == VP_NULL) {
        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("NULL Sequence Profile"));
        return VP_STATUS_INVALID_ARG;
    }

    VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Sequence Command 0x%02X 0x%02X",
        pProfile[0], pProfile[1]));

    switch(pProfile[0] & VP_SEQ_OPERATOR_MASK) {
        case VP_SEQ_SPRCMD_COMMAND_INSTRUCTION:
            switch(pDevCtx->deviceType) {
#if defined (VP_CC_890_SERIES)
                case VP_DEV_890_SERIES:
                    return Vp890CommandInstruction(pLineCtx, pProfile);
#endif

#if defined (VP_CC_880_SERIES)
                case VP_DEV_880_SERIES:
                    return Vp880CommandInstruction(pLineCtx, pProfile);
#endif

#if defined (VP_CC_790_SERIES)
                case VP_DEV_790_SERIES:
                    return Vp790CommandInstruction(pLineCtx, pProfile);
#endif

#if defined (VP_CC_580_SERIES)
                case VP_DEV_580_SERIES:
                    return Vp580CommandInstruction(pLineCtx, pProfile);
#endif

                default:
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_SEQ_SPRCMD_TIME_INSTRUCTION:
            return VpTimeInstruction(pLineCtx, pProfile);

        case VP_SEQ_SPRCMD_BRANCH_INSTRUCTION:
            return VpBranchInstruction(pLineCtx, pProfile);

        default:
            return VP_STATUS_INVALID_ARG;
    }
} /* VpSeq() */

/**
 * VpServiceSeq()
 *  This function tests the line status for an active sequence, and calls the
 * VpSeq function if there is an active sequence.  However, this function does
 * not check to see if the operation being pointed to by the current sequence is
 * supported.
 *
 * Preconditions:
 *  The device context cannot be VP_NULL, and only Vp790 and Vp880 currently
 * supported.
 *
 * Postconditions:
 *  If there is an active cadence that is supported by the line, then it is
 * called (via VpSeq).  If the current operation is not supported, the line
 * object active cadence is removed (i.e., set to inactive).
 */
bool
VpServiceSeq(
    VpDevCtxType *pDevCtx)  /**< Device that has a sequence.  The sequence may
                             * not be active
                             */
{
    uint8 channelId, maxChannels;
    VpDeviceInfoType deviceInfo;
    VpLineCtxType *pLineCtx;
    void *pLineObj;

    /*
     * pCadence is initialized to VP_NULL to remove compiler warnings
     * (.. pCadence might be used uninitialized..), but this function is called
     * only from API functions for devices that require cadence support.
     * Therefore, pCadence is initialized by the line object association below
     */
    VpSeqDataType *pCadence = VP_NULL;

    deviceInfo.pDevCtx = pDevCtx;
    deviceInfo.pLineCtx = VP_NULL;
    VpGetDeviceInfo(&deviceInfo);

    maxChannels = deviceInfo.numLines;

    for (channelId = 0; channelId < maxChannels; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            switch(pDevCtx->deviceType) {
#if defined (VP_CC_890_SERIES)
                case VP_DEV_890_SERIES:
                    pCadence = &((Vp890LineObjectType *)pLineObj)->cadence;
                    /*
                     * Override the maxChannel value from Get Device Info
                     * because the physical looping needs to go from 0:1, even
                     * if the device is only a single line (w/channel = 1).
                     */
                    maxChannels = VP890_MAX_NUM_CHANNELS;
                    break;
#endif
#if defined (VP_CC_880_SERIES)
                case VP_DEV_880_SERIES:
                    pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
                    break;
#endif
#if defined (VP_CC_790_SERIES)
                case VP_DEV_790_SERIES:
                    pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
                    break;
#endif

#if defined (VP_CC_580_SERIES)
                case VP_DEV_580_SERIES:
                    pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
                    break;
#endif

                default:
                    return FALSE;
            }

            if((pCadence->status & VP_CADENCE_STATUS_ACTIVE) == VP_CADENCE_STATUS_ACTIVE ) {
                VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Line Object 0x%04X on Channel %d",
                    pCadence->status, channelId));

                if(VpSeq(pLineCtx, pCadence->pCurrentPos) != VP_STATUS_SUCCESS) {
                    pCadence->status &= ~VP_CADENCE_STATUS_ACTIVE;
                    pCadence->pActiveCadence = VP_PTABLE_NULL;
                }
            }
        }
    }

    return TRUE;
} /* VpServiceSeq() */

/**
 * VpBranchInstruction()
 *  This function implements the Sequencer Branch instruction for the CSLAC
 * device types.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The branch count is either set if this is the first time for this branch, or
 * the branch count is decremented if this branch instruction has been executed
 * before.  If the branch count is decremented to 0, the sequencer index is
 * increased.  If the branch count is 0 at the first time the particular branch
 * is executed, the branch is repeated forever.  If the branch count is not 0,
 * the sequencer is set back to the instruction specified in the profile.  This
 * function can only return VP_SUCCESS since any valid combination of "branch
 * to" and "branch count" is valid.
 */
VpStatusType
VpBranchInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpSeqDataType *pCadence;
    VpOptionEventMaskType *pLineEvents;
    uint8 length, index;
    uint8 *pEventData;
    VpLineStateType lineState;
    uint8 branchDepth;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    uint8 *pIntSeqType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            lineState = ((Vp790LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp790LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            if (!(((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            lineState = ((Vp880LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp880LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            if (!(((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp890LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp890LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp890LineObjectType *)pLineObj)->processData;
            lineState = ((Vp890LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp890LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif


#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            if (!(((Vp580DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp580LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp580LineObjectType *)pLineObj)->processData;
            lineState = ((Vp580LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp580LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    length = pCadence->length;
    index = pCadence->index;

    if (pCadence->status & VP_CADENCE_STATUS_BRANCHING) {
        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Branching Length %d Index %d At %d",
            length, index, pCadence->branchAt));

        /*
         * We're already branching, but possibly at a step after this point. In
         * other words, we may have been branched back to a branch step
         * Determine if we are repeating this step, or if we are being branched
         * back from a later step
         */
        if (index < pCadence->branchAt) {
             /*
              * We're at an earlier step in the branch loop, so use the second
              * set of branch timers
              */
             branchDepth = VP_CSLAC_BRANCH_LVL_1;

             if (!(pCadence->status & VP_CADENCE_STATUS_BRANCHING_LVL2)) {
                 pCadence->status |= VP_CADENCE_STATUS_BRANCHING_LVL2;
                 pCadence->count[branchDepth] = pSeqData[1];
             }
        } else {
            /* This is a continuation from this branch */
            branchDepth = VP_CSLAC_BRANCH_LVL_0;
        }

        if (pCadence->count[branchDepth] > 0) {
            /*
             * If the repeat value set in the profile is = 0, this means repeat
             * forever.  Therefore, don't decrement the actual count value
             */
            if (pSeqData[1] != 0) {
                pCadence->count[branchDepth]--;
            }

            /* Send the profile pointer back to the branch location */
            /* Account for header offset */
            pCadence->index = (((pSeqData[0] & 0x1F) * 2)
                + VP_PROFILE_TYPE_SEQUENCER_START);
            pCadence->pCurrentPos =
                &(pCadence->pActiveCadence[pCadence->index]);
        } else {
            /*
             * We don't need to repeat this branch.  Just see if the profile is
             * complete
             */

            index+=2;
            if (index < (length + VP_PROFILE_LENGTH + 1)) {
                pCadence->index = index;
                pCadence->pCurrentPos+=2;
                if (pCadence->status & VP_CADENCE_STATUS_BRANCHING_LVL2) {
                    pCadence->status &= ~VP_CADENCE_STATUS_BRANCHING_LVL2;
                } else {
                    pCadence->status &= ~VP_CADENCE_STATUS_BRANCHING;
                }
            } else {  /* The profile is complete. */
                switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                    case VP_PRFWZ_PROFILE_METERING_GEN:
                        pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                        break;

                    case VP_PRFWZ_PROFILE_RINGCAD:
                        pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                        *pEventData = VP_RING_CAD_DONE;
                        break;

                    case VP_PRFWZ_PROFILE_TONECAD:
                        pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                        break;

                    case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_HOOK_FLASH;
                        break;

                    case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_PULSE_DIGIT;
                        break;

                    case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_DTMF_DIGIT;
                        VpCtrlMuteChannel(pLineCtx, FALSE);
                        break;

                    case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                        VpSetLineState(pLineCtx, lineState);
                        *pIntSeqType = 0;
                        break;

                    default:
                        break;

                }
                pCadence->status = VP_CADENCE_RESET_VALUE;
            }
        }
    } else {
        /*
         * We are not branching, so this is the first branching loop. Set the
         * parameter to indicate this step is branching.
         */
        branchDepth = VP_CSLAC_BRANCH_LVL_0;
        pCadence->count[branchDepth] = pSeqData[1];
        pCadence->branchAt = index;

        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Branch To %d, Count %d",
            pCadence->branchAt, pCadence->count[branchDepth]));

        if(pCadence->count[branchDepth] == 0) {
            /*
             * This means branch forever.  Implement by setting to max value
             * here, and not decreasing in steps above
             */
            pCadence->count[branchDepth] = 0xFF;
        } else {
            /* Repeat already (following lines) */
            pCadence->count[branchDepth]--;
        }
        /* Account for header offset */
        pCadence->index =
            (((pSeqData[0] & 0x1F) * 2) + VP_PROFILE_TYPE_SEQUENCER_START);
        pCadence->pCurrentPos = &(pCadence->pActiveCadence[pCadence->index]);
        pCadence->status |= VP_CADENCE_STATUS_BRANCHING;
    }

    /* If we've disabled the cadence, clear the active cadence pointer */
    if (!(pCadence->status & VP_CADENCE_STATUS_ACTIVE)) {
        pCadence->pActiveCadence = VP_PTABLE_NULL;
    }

    return VP_STATUS_SUCCESS;
}

/**
 * VpTimeInstruction()
 *  This function implements the Sequencer Time instruction for the CSLAC device
 * types.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The timer is decremented and when it decreases to 0, the pointer in the
 * sequence profile (passed) is updated to the next command past the time
 * operator currently being executed. If there are no more operators, then
 * the cadence is stopped.
 */
VpStatusType
VpTimeInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpSeqDataType *pCadence;
    VpCallerIdType *pCid;
    VpOptionEventMaskType *pLineEvents;
    uint8 *pEventData;
    uint16 tickRate;
    bool forever = FALSE;

    VpLineStateType lineState;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    uint8 *pIntSeqType;
    uint16 msInTick;

    /* Time in sequence is in 5mS incremements.  We need to convert to TICKS */
    uint16 timeInSeq = ( (( (uint16)pSeqData[0] & 0x1F) << 8) | (uint16)pSeqData[1]);

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {

                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp790DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            pCid = &((Vp790LineObjectType *)pLineObj)->callerId;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            lineState = ((Vp790LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp790LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            if (!(((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
            pCid = &((Vp880LineObjectType *)pLineObj)->callerId;
#else
            pCid = VP_NULL;
#endif
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            lineState = ((Vp880LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp880LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            if (!(((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp890LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp890DeviceObjectType *)pDevObj)->devProfileData.tickRate;
#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
            pCid = &((Vp890LineObjectType *)pLineObj)->callerId;
#else
            pCid = VP_NULL;
#endif
            pLineEvents = &((Vp890LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp890LineObjectType *)pLineObj)->processData;
            lineState = ((Vp890LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp890LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            if (!(((Vp580DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp580DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            pLineEvents = &((Vp580LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp580LineObjectType *)pLineObj)->processData;
            lineState = ((Vp580LineObjectType *)pLineObj)->lineState.usrCurrent;
            pCid = VP_NULL;
            pIntSeqType =  &((Vp580LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (pCadence->status & VP_CADENCE_STATUS_MID_TIMER) {
        pCadence->timeRemain--;
    } else {
        pCadence->status |= VP_CADENCE_STATUS_MID_TIMER;
        pCadence->timeRemain = MS_TO_TICKRATE((timeInSeq * 5), tickRate);

        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Time Operator %d", pCadence->timeRemain));

        if (pCadence->timeRemain == 0) {
            /* Always is selected.  End the cadence and leave the state as is */
            pCadence->status = VP_CADENCE_RESET_VALUE;
            forever = TRUE;
        } else {
            /*
             * Find out how long in ms 1 "tick" is, then subtract that amount
             * from the time required in the cadence. Lower limit 1 tick.
             */

            msInTick = TICKS_TO_MS(1, tickRate);
            /*
             * If the time specified in the sequence can be executed with at
             * least one tick, then subtract one "tick" worth of time
             */
            if ((timeInSeq * 5) > msInTick) {
                pCadence->timeRemain =
                    MS_TO_TICKRATE(((timeInSeq * 5) - msInTick), tickRate);
            }
        }
    }

    /* If the time is over, move on to the next sequence if there is one */
    if (pCadence->timeRemain == 0) {
        pCadence->index+=2;

        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("1. Time Operator Increment Index to %d",
            pCadence->index));

        if (pCadence->index < (pCadence->length + VP_PROFILE_LENGTH + 1)) {
            VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Time Operator -- Current Data 0x%02X 0x%02X pCad 0x%02X 0x%02X",
                pSeqData[0], pSeqData[1], pCadence->pCurrentPos[0], pCadence->pCurrentPos[1]));
            pSeqData+=2;
            pCadence->pCurrentPos = pSeqData;
            VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Time Operator -- Next Data 0x%02X 0x%02X pCad 0x%02X 0x%02X",
                pSeqData[0], pSeqData[1], pCadence->pCurrentPos[0], pCadence->pCurrentPos[1]));

        } else {  /* The profile is complete. */
            switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                case VP_PRFWZ_PROFILE_METERING_GEN:
                    pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                    break;

                case VP_PRFWZ_PROFILE_RINGCAD:
                    if (forever == FALSE) {
                        pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                        *pEventData = VP_RING_CAD_DONE;
                    }
                    break;

                case VP_PRFWZ_PROFILE_TONECAD:
                    pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                    break;

                case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_HOOK_FLASH;
                    break;

                case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_PULSE_DIGIT;
                    break;

                case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_DTMF_DIGIT;
                    VpCtrlMuteChannel(pLineCtx, FALSE);
                    break;

                case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                    *pIntSeqType = 0;
                    break;

                case VP_PRFWZ_PROFILE_FWD_DISC_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_FWD_DISCONNECT;
                    *pIntSeqType = 0;
                    break;

                case VP_PRFWZ_PROFILE_TIP_OPEN_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_TIP_OPEN_PULSE;
                    *pIntSeqType = 0;
                    break;

                case VP_PRFWZ_PROFILE_POLREV_PULSE_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_POLREV_PULSE;
                    *pIntSeqType = 0;
                    break;

                default:
                    break;

            }
            pCadence->status = VP_CADENCE_RESET_VALUE;
        }
        pCadence->status &= ~VP_CADENCE_STATUS_MID_TIMER;
    } else {
        /* Check to see if we're in the middle of a Wait on function. If so,
         * check to see if we still need to wait on CID (only supported wait
         * on operator). If CID is complete, terminate the timer function.
         * If not, continue..
         */
#if defined (VP_CC_580_SERIES) || defined (VP_CC_790_SERIES) \
|| (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) \
|| (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))

        if ((pCid != VP_NULL) && (pCid->status & VP_CID_WAIT_ON_ACTIVE)) {
            VP_CID(VpLineCtxType, pLineCtx, ("CID Status 0x%04X", pCid->status));
            if (pCid->status & VP_CID_IN_PROGRESS) {
                /* Do nothing */
                VP_CID(VpLineCtxType, pLineCtx, ("CID In Progress"));
            } else {
                VP_CID(VpLineCtxType, pLineCtx, ("Terminating Timer"));

                /* Terminate this timer operation and the wait on */
                pCid->status &= ~ VP_CID_WAIT_ON_ACTIVE;
                pCadence->status &= ~VP_CADENCE_STATUS_MID_TIMER;
                pCadence->timeRemain = 0;
                VP_SEQUENCER(VpLineCtxType, pLineCtx, ("2. Time Operator Increment Index to %d",
                    pCadence->index));

                pCadence->index+=2;

                if (pCadence->index <
                    (pCadence->length + VP_PROFILE_LENGTH + 1)) {
                    pSeqData+=2;
                    pCadence->pCurrentPos = pSeqData;
                } else {  /* The profile is complete. */
                    switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                        case VP_PRFWZ_PROFILE_METERING_GEN:
                            pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                            break;

                        case VP_PRFWZ_PROFILE_RINGCAD:
                            pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                            *pEventData = VP_RING_CAD_DONE;
                            break;

                        case VP_PRFWZ_PROFILE_TONECAD:
                            pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                            break;

                        case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_HOOK_FLASH;
                            break;

                        case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_PULSE_DIGIT;
                            break;

                        case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_DTMF_DIGIT;
                            VpCtrlMuteChannel(pLineCtx, FALSE);
                            break;

                        case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                            VpSetLineState(pLineCtx, lineState);
                            *pIntSeqType = 0;
                            break;

                        case VP_PRFWZ_PROFILE_FWD_DISC_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_FWD_DISCONNECT;
                            *pIntSeqType = 0;
                            break;

                        case VP_PRFWZ_PROFILE_TIP_OPEN_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_TIP_OPEN_PULSE;
                            *pIntSeqType = 0;
                            break;

                        case VP_PRFWZ_PROFILE_POLREV_PULSE_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_POLREV_PULSE;
                            *pIntSeqType = 0;
                            break;

                        default:
                            break;

                    }
                    pCadence->status = VP_CADENCE_RESET_VALUE;
                }
            }
        }
#endif
    }
    /* If we've disabled the cadence, clear the active cadence pointer */
    if (!(pCadence->status & VP_CADENCE_STATUS_ACTIVE)) {
        pCadence->pActiveCadence = VP_PTABLE_NULL;
    }
    return VP_STATUS_SUCCESS;
}

#if (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
    (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT))
/**
 * VpCSLACHowlerInit()
 *  This function fills in the cadence structure for special howler tones (those
 * requiring frequency, level, or both increase or decrease during the cadence).
 *
 * Preconditions:
 *  None. Data filler only.
 *
 * Postconditions:
 *  The cadence structure is filled with start, stop, and step information
 * required to implement the special howler cadence passed.
 */
bool
VpCSLACHowlerInit(
    uint8 toneType,
    VpSeqDataType *cadence)
{
    bool returnValue = TRUE;

    switch(toneType) {
        case VP_CSLAC_HOWLER_TONE:
            cadence->startFreq = 0x0888;
            cadence->startLevel = 0x07DB;
            cadence->freqStep = 0x0104;
            cadence->levelStep = 0x0030;
            cadence->stopFreq = 0x2222;
            cadence->stopLevel = 0x7FFF;
            break;

        case VP_CSLAC_AUS_HOWLER_TONE:
            cadence->startFreq = 0x1000;
            cadence->startLevel = 0x07DB;
            cadence->freqStep = 0x00B8;
            cadence->levelStep = 0x0018;
            cadence->stopFreq = 0x2222;
            cadence->stopLevel = 0x7FFF;
            break;

        case VP_CSLAC_NTT_HOWLER_TONE:
            cadence->startFreq = 0x0444;
            cadence->startLevel = 0x07DB;
            cadence->freqStep = 0x0000;
            cadence->levelStep = 0x0040;
            cadence->stopFreq = 0x0444;
            cadence->stopLevel = 0x7FFF;
            break;

        default:
            returnValue = FALSE;
            break;
    }

    return returnValue;
}
#endif

#if defined (VP_CC_790_SERIES) || \
   (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
   (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))
/**
 * VpCidSeq()
 *  This function services an active Caller ID Timer. This function runs when
 * the CLI timer for the passed channel is active. The function reads the
 * current caller ID sequence that is pointed to in the caller ID structure for
 * the given channel. This pointer is assigned when ever the function
 * CliStartCli is called. Additionally, the CliStartCli function sets the timer
 * to 1 to seed the CLI process.
 *
 *  This routine is broken up into two functional stages. The first stage
 * handles CLI tasks that are not time related while the second stage handles
 * time related task. Non-time related tasks include things such as muting a
 * channel, pol-rev and EOT (end of transmission). Time related tasks include
 * timing of the MARK signal, the SEIZURE signal and ACK detection as well as
 * timing for sending encoded data bytes to the device FSK generator.
 *
 * Preconditions:
 *  This Function must be called from the ApiTick function.
 *
 * Postconditions:
 *  The Caller ID State Machine is updated. Returns TRUE, if a user defined
 * event was encountered
 */
VpStatusType
VpCidSeq(
    VpLineCtxType *pLineCtx)    /**< Line that has an active CID sequence */
{
    VpStatusType retFlag = VP_STATUS_SUCCESS;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    VpSetLineStateFuncPtrType SetLineState;

    VpDeviceIdType deviceId;
    uint16 tickRate;
    uint8 ecVal, channelId;
    uint8 indexVal;  /* Use this when several index vals are needed */

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    VpCallerIdType *pCidStruct;
    VpLineStateType lineState;

    VpDigitType digit;

    uint16 uiCliOpCode;
    uint8 startOfCliData, mpiLen;
    uint16 cliTimer = 0;
    uint16 tempDebounceTime = 0;

    uint16 index;
    uint8 scratchData[1];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()+"));

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            SetLineState = Vp790SetLineStateInt;
            lineState = ((Vp790LineObjectType *)pLineObj)->lineState.currentState;

            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            tickRate =
                ((Vp790DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;

            switch(channelId) {
                case 0: ecVal = VP790_EC_CH1;   break;
                case 1: ecVal = VP790_EC_CH2;   break;
                case 2: ecVal = VP790_EC_CH3;   break;
                case 3: ecVal = VP790_EC_CH4;   break;
                default:
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                    return VP_STATUS_FAILURE;
            }
            break;

#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            SetLineState = Vp880SetLineStateInt;
            lineState = ((Vp880LineObjectType *)pLineObj)->lineState.currentState;
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;

            if (!(((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            ecVal = ((Vp880LineObjectType *)pLineObj)->ecVal;
            break;
#endif

#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
        case VP_DEV_890_SERIES:
            SetLineState = Vp890SetFxsLineState;

            lineState = ((Vp890LineObjectType *)pLineObj)->lineState.currentState;

            pCidStruct = &((Vp890LineObjectType *)pLineObj)->callerId;
            if (!(((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_INIT_CMP)) {
                VP_CID(VpLineCtxType, pLineCtx, ("1. VpCidSeq() -- VP_STATUS_DEV_NOT_INITIALIZED"));
                VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp890DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                VP_CID(VpLineCtxType, pLineCtx, ("2. VpCidSeq() -- VP_STATUS_DEV_NOT_INITIALIZED"));
                VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            tickRate =
                ((Vp890DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp890LineObjectType *)pLineObj)->channelId;
            ecVal = ((Vp890LineObjectType *)pLineObj)->ecVal;
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Determine if the timer is running. */
    if(pCidStruct->cliTimer > 0) {
        uint8 bufferCnt = 0;
        pCidStruct->cliTimer--;
        if (pCidStruct->status & VP_CID_REPEAT_MSG) {
            while ((VpFSKGeneratorReady(pLineCtx) == TRUE) && (bufferCnt < 2)) {
                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    pCidStruct->currentData);
                bufferCnt++;
            };
        }

        if(pCidStruct->cliTimer != 0) {
            VP_CID(VpLineCtxType, pLineCtx, ("1. VpCidSeq() -- Running Timer %d",
                pCidStruct->cliTimer));
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
            return VP_STATUS_SUCCESS;
        }
    } else {
        VP_CID(VpLineCtxType, pLineCtx, ("2. VpCidSeq() -- Running Timer %d",
            pCidStruct->cliTimer));
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
        return VP_STATUS_SUCCESS;
    }
    pCidStruct->status &= ~VP_CID_REPEAT_MSG;

    /*
     * Find where the start of the CLI command data is (excluding the MPI
     * command/data used to set the tone generator(s)
     */
    mpiLen = pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN];

    /*
     * Start of CLI commands on the LSB (word aligned). Exact location found
     * by adding the start elements offset to the end of the mpi command data
     * (found from the location of the mpi command length + the actual length
     * of the mpi data).
     */
    startOfCliData = VP_CID_PROFILE_FSK_PARAM_LEN + mpiLen +
        VP_CID_PROFILE_START_OF_ELEMENTS_LSB;

    /* Get the current index for the CLI profile. */
    index = pCidStruct->cliIndex;
    pCidStruct->cliDebounceTime = 0;

    /*
     * This section of code tests to see if a CPE ACK was received.
     * If the variable cliAwaitTone is TRUE, then test to see if the ACK
     * was received prior to the timeout specified in the CLI_DETECT portion
     * of the CLI profile. If the ACK was not received, you can not send the
     * CID information so terminate the CLI sequence.
     */
    if (pCidStruct->status & VP_CID_AWAIT_TONE) {
        VpCtrlDetectDTMF(pLineCtx, FALSE);
        /*
         * This would have been set to VP_DIG_NONE prior to starting the DTMF
         * digit detection. So any other value (whether in make or break
         * interval) is indication of DTMF digit detected during detection
         * interval.
         */

        digit = pCidStruct->digitDet;

        if ((digit == pCidStruct->cliDetectTone1)
         || (digit == pCidStruct->cliDetectTone2)) {
            /* The ACK tone was detected, continue with Caller ID */
        } else {
            /* Ack tone not detected, stop Caller ID */
            VpCliStopCli(pLineCtx);
            VP_CID(VpLineCtxType, pLineCtx, ("Ack Tone Not Detected"));
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
            return VP_STATUS_SUCCESS;
        }
    }

    /*
     * If the sending of FSK data needs to be terminated, then send the
     * terminate FSK control to the FSK generator. This happens at the end
     * of all FSK related signaling such as channel seizure.
     */
    if (pCidStruct->status & VP_CID_TERM_FSK) {
        pCidStruct->status &= ~VP_CID_TERM_FSK;
        pCidStruct->cliTimer = MS_TO_TICKRATE((tickRate >> 8), tickRate);

        switch(pCidStruct->pCliProfile[startOfCliData + index]) {
            case VP_CLI_MESSAGE:
            case VP_CLI_CHANSEIZURE:
            case VP_CLI_MARKSIGNAL:
                break;

            default:
                VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
                break;
        }
        VP_CID(VpLineCtxType, pLineCtx, ("Terminating FSK"));
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
        return VP_STATUS_SUCCESS;
    }

    /*
     * Handle zero-time-length codes. Zero time codes are op-codes in the
     * profile that are executed but have no time associated with them. In
     * other words the CLI sequence immediately moves on to the next state.
     * they include POL-REV, Channel mute, and EOT. The while loop churns
     * through the profile until a time related element is encountered.
     */
    uiCliOpCode = pCidStruct->pCliProfile[startOfCliData + index];

    VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code 0x%02X", uiCliOpCode));

    while ( (index <= pCidStruct->pCliProfile[startOfCliData - 2]) &&
            ( (uiCliOpCode == VP_CLI_POLREV) ||
              (uiCliOpCode == VP_CLI_EOT) ||
              (uiCliOpCode == VP_CLI_MUTEON) ||
              (uiCliOpCode == VP_CLI_MUTEOFF)) ) {

        switch (uiCliOpCode) {
            /* Mute both the upstream and down stream transmission paths. */
            case VP_CLI_MUTEON:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_MUTEON"));
                index+=2;
                pCidStruct->status |= VP_CID_MUTE_ON;
                VpCtrlMuteChannel(pLineCtx, TRUE);
                break;

            /*
             * Re-enable audio transmission in both the upstream and downstream
             * directions.
             */
            case VP_CLI_MUTEOFF:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_MUTEOFF"));
                index+=2;
                pCidStruct->status &= ~(VP_CID_MUTE_ON);
                VpCtrlMuteChannel(pLineCtx, FALSE);
                break;

            /* Invert the polarity of the line. */
            case VP_CLI_POLREV:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_POLREV"));

                index+=2;
                switch(lineState) {
                    case VP_LINE_STANDBY:
                    case VP_LINE_ACTIVE:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_ACTIVE_POLREV"));
                        SetLineState(pLineCtx, VP_LINE_ACTIVE_POLREV);
                        break;

                    case VP_LINE_ACTIVE_POLREV:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_ACTIVE"));
                        SetLineState(pLineCtx, VP_LINE_ACTIVE);
                        break;

                    case VP_LINE_TALK:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_TALK_POLREV"));
                        SetLineState(pLineCtx, VP_LINE_TALK_POLREV);
                        break;

                    case VP_LINE_TALK_POLREV:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_TALK"));
                        SetLineState(pLineCtx, VP_LINE_TALK);
                        break;

                    case VP_LINE_OHT:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_OHT_POLREV"));
                        SetLineState(pLineCtx, VP_LINE_OHT_POLREV);
                        break;

                    case VP_LINE_OHT_POLREV:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV to VP_LINE_OHT"));
                        SetLineState(pLineCtx, VP_LINE_OHT);
                        break;

                    default:
                        VP_CID(VpLineCtxType, pLineCtx, ("VP_CLI_POLREV ERROR!! Unknown State %d", lineState));
                        break;
                }
                break;

            /* Indicates the End Of Transmission for the CLI sequence */
            case VP_CLI_EOT:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_EOT"));
                VpCliStopCli(pLineCtx);
                return VP_STATUS_SUCCESS;

            default:
                index+=2;
                break;
        }
        uiCliOpCode = pCidStruct->pCliProfile[startOfCliData + index];
    }

    /*
     * Process all time based CLI profile codes. This includes timing of
     * channel seizure, ACK detect, MARK, and the message data.
     */
    if (index <= pCidStruct->pCliProfile[startOfCliData - 2]) {
        /* Switch on CLI Profile Element type at the current index. */
        switch (pCidStruct->pCliProfile[startOfCliData + index]) {

            /*
             * Set up the CLI sequence for detection of the CPE ACK. This state
             * will stop any running sequence, disable the sig gen, stop any
             * FSK activity, and set a time out for the tone detection. If
             * the time-out time is reached, the CPE did not ACK and the CLI
             * sequence will be aborted.
             */
            case VP_CLI_DETECT:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_DETECT"));

               /* Turn off the tone generator and turn FSK off */
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);

                /* Read the timeout time */
                index++;
                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);
                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                /* Read which tones to detect. */
                index+=2;

                /*
                 * The data from profile wizard can be shifted 4 right and be
                 * interpreted directly as a VpDigitType (other than 0xFF for
                 * Digit Type None)
                 */
                pCidStruct->cliDetectTone1 =
                    pCidStruct->pCliProfile[startOfCliData + index];
                if (pCidStruct->cliDetectTone1 != 0xFF) {
                    pCidStruct->cliDetectTone1 =
                        ((pCidStruct->cliDetectTone1 >> 4) & 0xFF);
                }
                if (VpIsDigit(pCidStruct->cliDetectTone1) == FALSE) {
                    VpCliStopCli(pLineCtx);
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                    return VP_STATUS_INVALID_ARG;
                }

                index+=2;
                pCidStruct->cliDetectTone2 =
                    pCidStruct->pCliProfile[startOfCliData + index];

                if (pCidStruct->cliDetectTone2 != 0xFF) {
                    pCidStruct->cliDetectTone2 =
                        ((pCidStruct->cliDetectTone2 >> 4) & 0xFF);
                }
                if (VpIsDigit(pCidStruct->cliDetectTone2) == FALSE) {
                    VpCliStopCli(pLineCtx);
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
                    return VP_STATUS_INVALID_ARG;
                }

                /* Start the tone detector. */
                index+=2;
                VpCtrlDetectDTMF(pLineCtx, TRUE);
                break;

            /*
             * This case sets up the signal generator to generate tones that
             * are defined in the profile. This will include things like the call
             * waiting beep. This case does not actually start the signal generator
             * it only sets it up.
             */
            case VP_CLI_ALERTTONE:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_ALERTTONE"));

                /* Stop any running sequences and disable any FSK activity. */
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);

                /*
                 * Set the timer to return as soon as possible before enabling the
                 * tone generator.
                 */
                index++;    /* Get to the length of MPI data to send */
                cliTimer = (tickRate >> 8);

                /*
                 * Send the MPI data to the device starting at the point after the
                 * length of data field
                 */
                indexVal = startOfCliData + index + 1;
                VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
                    pCidStruct->pCliProfile[startOfCliData + index],
                    (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));
                pCidStruct->status &= ~VP_CID_SIG_B_VALID;

                /* Get to the next command after the MPI data */
                index += pCidStruct->pCliProfile[startOfCliData + index];
                index += 2;
                break;

            /*
             * This case starts the signal generator for the time specified in
             * the CLI profile. It is assumed the signal generator was set up
             * first in the previous case.
             */
            case VP_CLI_ALERTTONE2:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_ALERTTONE2"));

                VpCtrlSetCliTone(pLineCtx, TRUE);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

            /*
             * This case creates a silent period of the time specified in the
             * profile. This is done by disabling the signal generator and
             * FSK generator.
             */
            case VP_CLI_SILENCE:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_SILENCE"));

                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

             /*
              * This case creates a silent period and prevents hook switch from
              * being detected for cliDebouncing time. Currently the debounce
              * period does nothing and should be enabled by creating a mask hook
              * method in the LIU.
              */
            case VP_CLI_SILENCE_MASKHOOK:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_SILENCE_MASKHOOK"));

                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index++;
                tempDebounceTime =
                    (pCidStruct->pCliProfile[startOfCliData + index] << 8) & 0xFF00;
                index++;
                tempDebounceTime |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                tempDebounceTime *= VP_CID_TIMESCALE;

                pCidStruct->cliDebounceTime =
                    MS_TO_TICKRATE(tempDebounceTime, tickRate);

                index+=2;
                break;

            /*
             * This case creates the channel seizure signal for the time specified.
             */
            case VP_CLI_CHANSEIZURE:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_CHANSEIZURE"));

                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                VpCtrlSetCliTone(pLineCtx, FALSE);
                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    VP_FSK_CHAN_SEIZURE);

                pCidStruct->currentData = VP_FSK_CHAN_SEIZURE;
                pCidStruct->status |= VP_CID_REPEAT_MSG;

                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                pCidStruct->status |= VP_CID_TERM_FSK;
                break;

             /*
              * This case creates the mark signal for the time specified using the
              * FSK generator.
              */
            case VP_CLI_MARKSIGNAL:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_MARKSIGNAL"));

                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    VP_FSK_MARK_SIGNAL);

                pCidStruct->currentData = VP_FSK_MARK_SIGNAL;
                pCidStruct->status |= VP_CID_REPEAT_MSG;

                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

            /* This case sends the actual message data (FSK Format). */
            case VP_CLI_MESSAGE: {
                uint8 bufferCnt = 0;

                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_MESSAGE"));

                /* If the FSK Generator was not setup before, set it now */
                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                while ((VpFSKGeneratorReady(pLineCtx) == TRUE) && (bufferCnt < 2)) {
                    bufferCnt++;

                    /* Send next data in buffer */
                    if(VpCliGetEncodedByte(pLineCtx, scratchData) == TRUE) {
                        VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_DATA,
                            scratchData[0]);
                    } else {
                        /* We're done. Go to the next element */
                        /*
                         * Last value, '1' is a flag to the device specific
                         * API-II that this step can tolerate suspending CID
                         * until all generator data is sent. A '0' indicates
                         * that CID cannot be suspended.
                         */
                        VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 1);
                        index+=2;
                        break;
                    }
                };

                /* Force a check for buffer update at the next tick  */
                cliTimer = (tickRate >> 8);
                }
                break;

            /* This case sends the actual message data (DTMF Format). */
            case VP_CLI_DTMF_MESSAGE:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code VP_CLI_DTMF_MESSAGE"));

                if (VpDTMFGeneratorReady(pLineCtx) == TRUE) {
                    /* Send next data in buffer */
                    if(VpCliGetEncodedByte(pLineCtx, scratchData) == TRUE) {
                        VpCtrlSetDTMFGen(pLineCtx, VP_CID_GENERATOR_DATA,
                            VpConvertCharToDigitType(scratchData[0]));
                    } else {
                        /* We're done. Go to the next element */
                        VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
                        index+=2;
                    }
                }

                /* Force a check for buffer update at the next tick  */
                cliTimer = (tickRate >> 8);
                break;

            /* Shouldn't be possible */
            default:
                VP_CID(VpLineCtxType, pLineCtx, ("CID Op-Code ERROR"));

                index += 2;
                break;

        } /* End of Switch (CLI Element Type) */
    } /* if index <= number of elements */

    /*
     * If the CLI sequencer has indexed passed the number of elements in the
     * the profile, then stop the CLI sequence. This condition is true if
     * the ACK was not received, the EOT marker in the profile was reached
     * or an error occurred. Otherwise, continue normally.
     */
    if (index > pCidStruct->pCliProfile[startOfCliData - 2]) {
        VP_CID(VpLineCtxType, pLineCtx, ("Stopping CID"));
        VpCliStopCli(pLineCtx);
    } else {
        pCidStruct->cliIndex = index;
        pCidStruct->cliTimer = MS_TO_TICKRATE(cliTimer, tickRate);
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("VpCidSeq()-"));
    return retFlag;
}

/**
 * VpFSKGeneratorReady()
 *  This function returns TRUE if the FSK Generator is ready to accept the next
 * CID message byte, FALSE otherwise.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None. Status of FSK Generator only is reported. Otherwise line is unaffected
 */
bool
VpFSKGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790FSKGeneratorReady(pLineCtx);
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            return Vp880FSKGeneratorReady(pLineCtx);
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            return Vp890FSKGeneratorReady(pLineCtx);
#endif

        default:
            return TRUE;
    }
}

/**
 * VpDTMFGeneratorReady()
 *  This function returns TRUE if the DTMF Generator is ready to accept the next
 * CID message byte, FALSE otherwise.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None. Status of DTMF Generator only is reported. Otherwise line is unaffected
 */
bool
VpDTMFGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    bool returnVal = TRUE;

#if defined (VP_CC_880_SERIES) || defined (VP_CC_890_SERIES)
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    uint16 tickRate;

    VpCslacTimers *pLineTimers;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    VpCallerIdType *pCidStruct = VP_NULL;

    switch (deviceType) {

#ifdef VP_CC_880_SERIES
        case VP_DEV_880_SERIES:
            pLineTimers = &((Vp880LineObjectType *)pLineObj)->lineTimers.timers;

#if defined (VP880_FXS_SUPPORT)
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
#endif
            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            break;
#endif

#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            pLineTimers = &((Vp890LineObjectType *)pLineObj)->lineTimers.timers;
#if defined (VP890_FXS_SUPPORT)
            pCidStruct = &((Vp890LineObjectType *)pLineObj)->callerId;
#endif
            tickRate =
                ((Vp890DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            break;
#endif

        default:
            return TRUE;
    }

    if ((!(pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] & VP_ACTIVATE_TIMER))&&
        (pCidStruct != VP_NULL)) {
        /*
         * Timer appears available. If we're in an on-time already, change to
         * the off-time and return FALSE (i.e., cannot program another DTMF
         * number yet). If it's in an off-time, it's now complete so return
         * TRUE.
         */
        if (pCidStruct->dtmfStatus & VP_CID_ACTIVE_ON_TIME) {
            pCidStruct->dtmfStatus &= ~VP_CID_ACTIVE_ON_TIME;

            /* Setup the line timer for the off-time for DTMF CID */
            pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] =
                MS_TO_TICKRATE(VP_CID_DTMF_OFF_TIME, tickRate);

            pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] |= VP_ACTIVATE_TIMER;
            pCidStruct->dtmfStatus |= VP_CID_ACTIVE_OFF_TIME;

            VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
            returnVal = FALSE;
        } else if (pCidStruct->dtmfStatus & VP_CID_ACTIVE_OFF_TIME) {
            pCidStruct->dtmfStatus &= ~VP_CID_ACTIVE_OFF_TIME;

            /* Nothing more to do here */
            returnVal = TRUE;
        }
    } else {
        returnVal = FALSE;
    }
#endif

    return returnVal;
}

#if defined (VP_CC_790_SERIES) || (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
   (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))
/**
 * VpCliStopCli()
 *  This function stops the CLI sequence on the passed line.
 *
 * Preconditions
 *  None
 *
 * Postconditions
 *  The caller ID sequence, if running, will be aborted.
 */
void
VpCliStopCli(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    void *pLineObj = pLineCtx->pLineObj;

    uint8 *pEventData;
    VpOptionEventMaskType *pLineEvents;

    VpCallerIdType *pCidStruct;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            pCidStruct->cliTimer = 0;
            Vp790SetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            break;

#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            pCidStruct->cliTimer = 0;
            Vp880SetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            break;
#endif

#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
        case VP_DEV_890_SERIES:
            pCidStruct = &((Vp890LineObjectType *)pLineObj)->callerId;
            pCidStruct->cliTimer = 0;
            Vp890SetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
            pLineEvents = &((Vp890LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp890LineObjectType *)pLineObj)->processData;
            break;
#endif

        default:
            return;
    }

    VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
    VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
    VpCtrlDetectDTMF(pLineCtx, FALSE);

    pCidStruct->status &= ~(VP_CID_MUTE_ON);
    VpCtrlMuteChannel(pLineCtx, FALSE);

    pCidStruct->status &= ~VP_CID_IN_PROGRESS;
    pLineEvents->process |= VP_LINE_EVID_CID_DATA;
    *pEventData = VP_CID_DATA_TX_DONE;
}

/**
 * VpCliGetEncodedByte()
 *  This function returns an encoded byte of data that is suitable for writing
 * the FSK generator (device dependent).
 *
 * Preconditions
 *  Must have a valid CLI packet in to work from.
 *
 * Postconditions
 *  The per-channel caller ID buffer will be updated with encoded data.
 */
bool
VpCliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790CliGetEncodedByte(pLineCtx, pByte);
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            return Vp880CliGetEncodedByte(pLineCtx, pByte);
#endif

#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
        case VP_DEV_890_SERIES:
            return Vp890CliGetEncodedByte(pLineCtx, pByte);
#endif

        default:
            return FALSE;
    }
}

/**
 * VpCtrlSetCliTone()
 *  This function is called by the API internally to enable or disable the
 * signal generator used for Caller ID.
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  The signal generator used for CID tones is enabled/disabled indicated by
 * the mode parameter passed.
 */
VpStatusType
VpCtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790CtrlSetCliTone(pLineCtx, mode);
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            return Vp880CtrlSetCliTone(pLineCtx, mode);
#endif

#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
        case VP_DEV_890_SERIES:
            return Vp890CtrlSetCliTone(pLineCtx, mode);
#endif

        default:
            return VP_STATUS_FUNC_NOT_SUPPORTED;
    }
    return VP_STATUS_SUCCESS;
}
#endif

/**
 * VpCtrlDetectDTMF()
 *  This function is called by the API internally to enable or disable the
 * system level dtmf detector (if present).
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  If implemented, DTMF detection is enabled (or disabled) in the system
 * services layer. This function does not call the system services function for
 * devices that support internal DTMF detection.
 */
VpStatusType
VpCtrlDetectDTMF(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpDeviceIdType deviceId;
    uint8 channelId;
    VpCallerIdType *pCidStruct = VP_NULL;

    void *pDevObj = pDevCtx->pDevObj;
    void *pLineObj = pLineCtx->pLineObj;

    if ((pDevObj == VP_NULL) || (pLineObj == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
#if defined (VP880_FXS_SUPPORT)
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
#endif
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp890LineObjectType *)pLineObj)->channelId;
#if defined (VP890_FXS_SUPPORT)
            pCidStruct = &((Vp890LineObjectType *)pLineObj)->callerId;
#endif
            break;
#endif

        default:
            return VP_STATUS_FUNC_NOT_SUPPORTED;
    }

    /*
     * IF enabling DTMF detection, make sure the TX PCM is enabled. Otherwise,
     * return TX/RX to states determined by Mute On/Off and linestate
     */
    if (mode == TRUE) {
        if (pCidStruct != VP_NULL) {
            pCidStruct->status |= VP_CID_AWAIT_TONE;
            pCidStruct->digitDet = VP_DIG_NONE;
        }
        VpCtrlMuteChannel(pLineCtx, TRUE);
        VpSysDtmfDetEnable(deviceId, channelId);
    } else {
        if (pCidStruct != VP_NULL) {
            pCidStruct->status &= ~(VP_CID_AWAIT_TONE);
        }
        VpSysDtmfDetDisable(deviceId, channelId);
        VpCtrlMuteChannel(pLineCtx, FALSE);
    }

    return VP_STATUS_SUCCESS;
}
#endif

/**
 * VpCtrlSetFSKGen()
 *  This function is called by the CID sequencer executed internally by the API
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  The data indicated by mode and data is applied to the line. Mode is used
 * to indicate whether the data is "message", or a special character. The
 * special characters are "channel siezure" (alt. 1/0), "mark" (all 1), or
 * "end of transmission".
 */
#if (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
    (defined (VP_CC_790_SERIES))
void
VpCtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 data)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            Vp790CtrlSetFSKGen(pLineCtx, mode, data);
            break;
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            Vp880CtrlSetFSKGen(pLineCtx, mode, data);
            break;
#endif

#if defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)
        case VP_DEV_890_SERIES:
            Vp890CtrlSetFSKGen(pLineCtx, mode, data);
            break;
#endif

        default:
            break;
    }
    return;
}

/**
 * VpConvertCharToDigitType()
 *  This function is called by the CID sequencer executed internally by the API.
 * It converts a character to a VpDigitType and is used for functions requiring
 * a VpDigitType specifically.
 *
 * Preconditions:
 *  None. Utility function only.
 *
 * Postconditions:
 *  The character passed is converted/returned as a VpDigitType
 */
VpDigitType
VpConvertCharToDigitType(
    char digit)
{
    VpDigitType vpDig;

    switch(digit) {
        case '0':
            vpDig = VP_DIG_ZERO;
            break;

        case 'A':
            vpDig = VP_DIG_A;
            break;

        case 'B':
            vpDig = VP_DIG_B;
            break;

        case 'C':
            vpDig = VP_DIG_C;
            break;

        case 'D':
            vpDig = VP_DIG_D;
            break;

        case '*':
            vpDig = VP_DIG_ASTER;
            break;

        case '#':
            vpDig = VP_DIG_POUND;
            break;

        default:
            vpDig = (uint8)digit-48;
            break;
    }
    return vpDig;
}

/**
 * VpCtrlSetDTMFGen()
 *  This function sets the DTMF generators of the device and if DTMF message
 * data is in progress, disables the TX/RX PCM highway. If this is the end
 * of DTMF message data transmission, then the TX/RX PCM is re-enabled based
 * on the line state and the tx/rx mode set for "talk" states.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The DTMF signal generators are set to the CID specified level and the digit
 * passed is applied to the line (if mode == VP_CID_GENERATOR_DATA).
 */
void
VpCtrlSetDTMFGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;

    if (mode == VP_CID_GENERATOR_DATA) {
        VpCtrlMuteChannel(pLineCtx, TRUE);
    } else {
        VpCtrlMuteChannel(pLineCtx, FALSE);
    }

    switch (pDevCtx->deviceType) {
#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            Vp880SetDTMFGenerators(pLineCtx, mode, digit);
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            Vp890SetDTMFGenerators(pLineCtx, mode, digit);
            break;
#endif
        default:
            break;
    }
    return;
}
#endif

/**
 * VpCtrlMuteChannel()
 *  This function disables the TX/RX PCM highway if mode == TRUE, otherwise it
 * enables the TX/RX PCM highway based on line state and the option set for
 * TX/RX enable mode in talk states.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The TX/RX PCM mode is set on the line.
 */
void
VpCtrlMuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            Vp790MuteChannel(pLineCtx, mode);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            Vp880MuteChannel(pLineCtx, mode);
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            Vp890MuteChannel(pLineCtx, mode);
            break;
#endif
        default:
            break;
    }
    return;
}

/**
 * InitCadenceVars()
 *  This function initializes the Cadence (sequencer) Variables in the line
 * object associated with the line context passed.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  The VpSeqDataType structure variables passed in the line context are
 * initialized to pre-determined values.
 */
void
InitCadenceVars(
    VpLineCtxType *pLineCtx)    /**< Line to initialize API Cadence (Sequencer)
                                 * Variables for
                                 */
{
    VpDeviceType deviceType = pLineCtx->pDevCtx->deviceType;
    void *pLineObj = pLineCtx->pLineObj;
    VpSeqDataType *pCadence;
    uint8 countLoop;

    switch(deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            pCadence = &((Vp890LineObjectType *)pLineObj)->cadence;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
            break;
#endif

        default:
            /* Nothing known to initialize */
            return;
    }

    pCadence->pActiveCadence = VP_PTABLE_NULL;
    pCadence->pCurrentPos = VP_PTABLE_NULL;
    pCadence->status = VP_CADENCE_RESET_VALUE;    /* No active status */
    pCadence->branchAt = 0;
    pCadence->index = 0;
    pCadence->length = 0;

    for (countLoop = 0; countLoop < VP_CSLAC_MAX_BRANCH_DEPTH; countLoop++) {
        pCadence->count[countLoop] = 0;
    }

    pCadence->timeRemain = 0;
}

/**
 * VpCSLACInitMeter()
 *  This function is used to initialize metering parameters. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function initializes metering parameters as per given profile.
 */
VpStatusType
VpCSLACInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile)
{
#if defined(VP_CC_790_SERIES) || (defined(VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT))
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId;
    VpProfilePtrType pMetering;
    VpCSLACDeviceProfileTableType *pDevProfTable;
    VpProfileDataType *pMpiData;

    uint8 channelId, ecVal, meterProfEntry;
    bool deviceInit = FALSE;
    int tableSize = VP_CSLAC_METERING_PROF_TABLE_SIZE;

    int meterIndex = VpGetProfileIndex(pMeterProfile);

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            pDevProfTable = &((Vp790DeviceObjectType *)pDevObj)->devProfileTable;
            deviceInit = (((Vp790DeviceObjectType *)pDevObj)->status.state
                & VP_DEV_INIT_CMP);
            meterProfEntry =
                ((Vp790DeviceObjectType *)pDevObj)->profEntry.meterProfEntry;

            switch(channelId) {
                case 0: ecVal = VP790_EC_CH1;   break;
                case 1: ecVal = VP790_EC_CH2;   break;
                case 2: ecVal = VP790_EC_CH3;   break;
                case 3: ecVal = VP790_EC_CH4;   break;
                default:
                    return VP_STATUS_FAILURE;
            }
            break;
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            ecVal = ((Vp880LineObjectType *)pLineObj)->ecVal;
            pDevProfTable = &((Vp880DeviceObjectType *)pDevObj)->devProfileTable;
            deviceInit = (((Vp880DeviceObjectType *)pDevObj)->state
                & VP_DEV_INIT_CMP);

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                deviceInit = FALSE;
            }

            meterProfEntry =
                ((Vp880DeviceObjectType *)pDevObj)->profEntry.meterProfEntry;
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (!(deviceInit)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * If the profile passed is an index, make sure it's in the valid range
     * and if so, set the currently used profile to it.
     */
    if ((meterIndex >= 0) && (meterIndex < tableSize)) {
        if (!(meterProfEntry & (0x01 << meterIndex))) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_PROFILE;
        }

        pMetering = pDevProfTable->pMeteringProfileTable[meterIndex];
        /* Valid Cadence Entry. Set it if the profile has been initialized */
        if (pMetering != VP_PTABLE_NULL) {
            pMpiData = (VpProfileDataType *)(&pMetering[VP_PROFILE_MPI_LEN+1]);
            VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
                pMetering[VP_PROFILE_MPI_LEN], pMpiData);
        }
    } else if (meterIndex >= tableSize) {
        /* It's an index, but it's out of range */
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    } else {
        /* This is a valid metering pointer. Set it */
        pMpiData = (VpProfileDataType *)(&pMeterProfile[VP_PROFILE_MPI_LEN+1]);
        VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD,
            pMeterProfile[VP_PROFILE_MPI_LEN], pMpiData);
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
#endif /* 880 || 790 */
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACStartMeter()
 *  This function starts (can also abort) metering pulses on the line. See
 * VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  Metering pulses are transmitted on the line.
 */
VpStatusType
VpCSLACStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters)
{
#if defined(VP_CC_790_SERIES) || (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT))
    uint16 tempTime;
    uint8 *pIntSequence;
    VpSeqDataType *pCadence;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpOptionEventMaskType *pLineEvents;
    VpDeviceIdType deviceId;
    uint8 *pMeterCnt;

    bool deviceInit = FALSE;
    uint8 index;
    bool timeOops = FALSE;

    VpSetLineStateFuncPtrType SetLineState =
        pDevCtx->funPtrsToApiFuncs.SetLineState;

    VpLineStateType currentState;
    uint8 seqLen, seqByte;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            pIntSequence = &((Vp790LineObjectType *)pLineObj)->intSequence[0];
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            currentState = ((Vp790LineObjectType *)pLineObj)->lineState.currentState;
            seqLen = VP790_INT_SEQ_LEN;
            deviceInit = (((Vp790DeviceObjectType *)pDevObj)->status.state
                          & VP_DEV_INIT_CMP);
            pMeterCnt = &((Vp790LineObjectType *)pLineObj)->processData;
            break;
#endif

#if defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pIntSequence = &((Vp880LineObjectType *)pLineObj)->intSequence[0];
            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            currentState = ((Vp880LineObjectType *)pLineObj)->lineState.currentState;
            seqLen = VP880_INT_SEQ_LEN;
            deviceInit = (((Vp880DeviceObjectType *)pDevObj)->state
                          & VP_DEV_INIT_CMP);

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->state & VP_DEV_IN_CAL) {
                deviceInit = FALSE;
            }

            pMeterCnt = &((Vp880LineObjectType *)pLineObj)->processData;
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (!(deviceInit)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * If we're not in a valid line state where metering is possible, generate
     * an event that metering was aborted for FXS lines and return success.
     * Error on FXO lines.
     */
    switch(currentState) {
        case VP_LINE_TIP_OPEN:
        case VP_LINE_DISCONNECT:
            pLineEvents->process |= VP_LINE_EVID_MTR_ABORT;
            *pMeterCnt = pCadence->meteringBurst;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_SUCCESS;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_TALK:
        case VP_LINE_FXO_RING_GND:
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG;

        default:
            break;
    }

    /* Clear out the internal sequencer */
    for (seqByte = 0; seqByte < seqLen; seqByte++) {
        pIntSequence[seqByte] = 0x00;
    }

    /* Stop all other cadences if they were active */
    pCadence->status = VP_CADENCE_RESET_VALUE;

    /*
     * Number of meters = 0 will stop metering. Generate the event, report
     * number of metering pulses generated to this point and return.
     */
    if (numMeters == 0) {
        pLineEvents->process |= VP_LINE_EVID_MTR_ABORT;
        *pMeterCnt = pCadence->meteringBurst;
        pCadence->pActiveCadence = VP_PTABLE_NULL;
        SetLineState(pLineCtx, currentState);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * We are starting a new metering session. Reset the metering pulse count
     * that is used if metering is aborted.
     */
    pCadence->meteringBurst = 0;

    /*
     * Build the sequence in the order that it will be executed. Not necessary,
     * but makes reading this code easier.
     */

    /* Set the type of profile to "metering internal" */
    pIntSequence[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_METERING_GEN;

    /* And set the starting command to start metering */
    index = 0;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_METERING);
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START + index] = 0x01;

    /*
     * Set the metering on time after metering is enabled. Time command takes
     * two bytes. Metering time is specified in 10mS incr, Cadence in 5mS.
     * If the time specified is non-zero by the user, prevent the math from
     * converting it to 0.
     */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] =
        VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = onTime * 2;  /* Change from 10mS to 5mS ticks */
    if (tempTime) {
        tempTime = ((tempTime - 2) >> 8) & 0x03;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;
        if (tempTime == 0) {
            timeOops = TRUE;
        }
        index++;
        tempTime = ((onTime * 2 - 2) & 0x00FF);
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;
        if (tempTime == 0) {
            timeOops = TRUE;
        }

        if (timeOops == TRUE) {
            pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= 1;
        }
    } else {
        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;
    }

    /*
     * If the on-time is = 0, the Sequencer automatically suspends indefinitely
     * at the current state. Otherwise, it will proceed to the next step.
     */

    /* Then turn the metering off */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_METERING);

    index++;    /* This should set index = 5 at this point */
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;

    /*
     * Set the metering off time after metering is enabled. Time command takes
     * two bytes. Metering time is specified in 10mS incr, Cadence in 5mS.
     * If the time specified is non-zero by the user, prevent the math from
     * converting it to 0.
     */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] =
        VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = offTime * 2;  /* Change from 10mS to 5mS ticks */
    if (tempTime) {
        timeOops = FALSE;
        tempTime = ((tempTime - 2) >> 8) & 0x03;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;

        if (tempTime == 0) {
            timeOops = TRUE;
        }

        index++;
        tempTime = ((offTime * 2 - 2) & 0x00FF);
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;

        if (tempTime == 0) {
            timeOops = TRUE;
        }

        if (timeOops == TRUE) {
            pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= 1;
        }
    } else {
        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;
    }

    /*
     * Condition of numMeters = 0 (stop metering) is taken care of at top.
     * Then, condition numMeters != 0 and on-time is (meter forever) is taken
     * care of as a consequence of the Sequencer (i.e., suspend for any time
     * operator set to 0).
     * All other operators will count some number of metering pulses, end, then
     * go back to the state the line was in when the sequence started.
     */

    /* Can only be 1 or > 1, not 0 */
    if (numMeters > 1) {
        /*
         * If more than 1, we'll branch back to the start of the metering on
         * until all metering pulses occur.
         */
        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] =
            VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;

        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = numMeters - 1;
    } else {
        /* If exactly equal to 1, no branch is necessary. */
    }

    /*
     * When the metering is complete, return to the current line state. Note
     * that this step is not reached if metering is infinite (on-time = 0)
     */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] =
        (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] =
        ConvertApiState2PrfWizState(currentState);

    /* Sequence Length is index + 1 because index is 0 based */
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index + 1;

    /*
     * Profile Length is always sequence lengh + 4 because of header
     * definition.
     */
    pIntSequence[VP_PROFILE_LENGTH] =
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] + 4;

    pCadence->index = VP_PROFILE_TYPE_SEQUENCER_START;
    pCadence->length = pIntSequence[VP_PROFILE_LENGTH];

    pCadence->pActiveCadence = &pIntSequence[0];
    pCadence->pCurrentPos = &pIntSequence[8];

    pCadence->status |= VP_CADENCE_STATUS_ACTIVE;
    pCadence->status |= VP_CADENCE_STATUS_METERING;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
#endif /* 790 || 880 */
    return VP_STATUS_SUCCESS;
}
#endif  /* VP_CSLAC_SEQ_EN */
