/** \file vp890_control.c
 * vp890_control.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Control Functions.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 4881 $
 * $LastChangedDate: 2009-04-09 08:14:55 -0500 (Thu, 09 Apr 2009) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES)  /* Compile only if required */

#include    "vp_api_int.h"
#include    "vp890_api_int.h"
#include    "sys_service.h"

#ifdef VP_CSLAC_SEQ_EN
#ifdef VP890_FXS_SUPPORT
static VpStatusType
SendMsgWaitPulse(
    VpLineCtxType           *pLineCtx,
    VpSendMsgWaitType       *pMsgWait);

static VpStatusType
SendFwdDisc(
    VpLineCtxType           *pLineCtx,
    uint16                  timeInMs);

static VpStatusType
SendPolRevPulse(
    VpLineCtxType           *pLineCtx,
    uint16                  timeInMs);
#endif

#ifdef VP890_FXO_SUPPORT
static VpStatusType
MomentaryLoopOpen(
    VpLineCtxType           *pLineCtx);

static VpStatusType
SendDigit(
    VpLineCtxType           *pLineCtx,
    VpDigitGenerationType   digitType,
    VpDigitType             digit);
#endif

/*******************************************************************************
 * Vp890SendSignal()
 *  This function sends a signal on the line. The type of signal is specified
 * by the type parameter passed. The structure passed specifies the parameters
 * associated with the signal.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The signal specified is applied to the line.
 ******************************************************************************/
VpStatusType
Vp890SendSignal(
    VpLineCtxType       *pLineCtx,
    VpSendSignalType    type,
    void                *pStruct)
{
    VpDigitType           *pDigit;
    VpDigitType           digit    = VP_DIG_NONE;
    VpDevCtxType          *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpStatusType          status = 0;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pStruct == VP_NULL) {
        pDigit = &digit;
    } else {
        pDigit = pStruct;
    }

    switch(type) {
#ifdef VP890_FXO_SUPPORT
        case VP_SENDSIG_DTMF_DIGIT:
            status = SendDigit(pLineCtx, VP_DIGIT_GENERATION_DTMF, *pDigit);
            break;

        case VP_SENDSIG_PULSE_DIGIT:
            pDigit = (VpDigitType *)pStruct;
            if(VP_NULL != pDigit)    //for fix moto
            {
                status = SendDigit(pLineCtx, VP_DIGIT_GENERATION_DIAL_PULSE,
                    *pDigit);
            }
            break;

        case VP_SENDSIG_HOOK_FLASH:
            /* prevent case of *pDigit when user passes VP_NULL */
            status = SendDigit(pLineCtx, VP_DIGIT_GENERATION_DIAL_HOOK_FLASH,
                VP_DIG_NONE);
            break;

        case VP_SENDSIG_MOMENTARY_LOOP_OPEN:
            status = MomentaryLoopOpen(pLineCtx);
            break;
#endif

#ifdef VP890_FXS_SUPPORT
        case VP_SENDSIG_MSG_WAIT_PULSE:
            status = SendMsgWaitPulse(pLineCtx, pStruct);
            break;

        case VP_SENDSIG_FWD_DISCONNECT:
            if (pStruct != VP_NULL) {
                status = SendFwdDisc(pLineCtx, *((uint16 *)pStruct));
            } else {
                VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendSignal() - VP_NULL invalid for FWD_DISCONNECT"));
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_SENDSIG_POLREV_PULSE:
            if (pStruct != VP_NULL) {
                status = SendPolRevPulse(pLineCtx, *((uint16 *)pStruct));
            } else {
                VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendSignal() - VP_NULL invalid for POLREV_PULSE"));
                return VP_STATUS_INVALID_ARG;
            }
            break;
#endif

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendSignal() - Invalid signal type"));
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    return status;
} /* Vp890SendSignal() */

#ifdef VP890_FXS_SUPPORT
/******************************************************************************
 * SendMsgWaitPulse()
 *  This function sends a message waiting pulse to the line specified by the
 * by the pMsgWait parameter passed. The structure specifies a voltage, on-time,
 * off-time, and number of pulses.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The message waiting signal specified is applied to the line.
 ******************************************************************************/
static VpStatusType
SendMsgWaitPulse(
    VpLineCtxType           *pLineCtx,
    VpSendMsgWaitType       *pMsgWait)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    uint8                   ecVal       = pLineObj->ecVal;
    uint16                  tickRate    = pDevObj->devProfileData.tickRate;
    VpLineStateType         currentState = pLineObj->lineState.usrCurrent;
    uint8                   addStep     = 0;
    uint32                  aVolt;
    int32                   userVolt;
    uint8                   seqByte, branchCount;
    uint16                  tempTime, firstTimer, secondTimer;
    uint8                   cmdLen      = 0x08; /* Min Cadence with infinite on */


    /*
     * Set the signal generator parameters to set the A amplitude and frequency
     * "very low". We'll adjust the bias to the user defined MsgWait voltage
     */
    uint8 sigGenBytes[VP890_SIGA_PARAMS_LEN] = {
        0x00, 0x29, 0x73, 0x04, 0x44, 0x00, 0x15, 0x7F, 0xFD, 0x00, 0x00};

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SendMsgWaitPulse() - Function invalid for FXO"));
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If we're already in Ringing, return a failure since we're using a
     * shared resource to accomplish this function.
     */
    if ((currentState == VP_LINE_RINGING) || (currentState == VP_LINE_RINGING_POLREV)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SendMsgWaitPulse() - Shared resource in use - ringing"));
        return VP_STATUS_FAILURE;
    }

    /*
     * If the voltage is 0, it (previously) meant to use the maximum voltage
     * supported by the line. However, that function has been removed so instead
     * of stopping Message Waiting, just return error code to maintain a bit of
     * backward compatibility (max voltage isn't applied, but it isn't stopped
     * either).
     */
    if ((pMsgWait != VP_NULL) && (pMsgWait->voltage == 0)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SendMsgWaitPulse() - 0 voltage not supported"));
        return VP_STATUS_INVALID_ARG;
    }

    /* All parameters passed are good -- proceed */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;    /* No active status */

    for (seqByte = 0; seqByte < VP890_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /*
     * If we were previously running a Message Waiting cadence, stop it and
     * generate the event.
     * If we were previously running another cadence, let it continue and
     * return.
     */
    if ((pMsgWait == VP_NULL) || (pMsgWait->onTime == 0)) {
        VpSetLineState(pLineCtx, currentState);
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
        pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
        pLineObj->processData = VP_SENDSIG_MSG_WAIT_PULSE;
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * Compute the new signal generator A values from the voltage and set the
     * line state that is used to apply the message waiting pulse
     */
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    if (pMsgWait->voltage > 0) {
        userVolt = pMsgWait->voltage;
        pLineObj->intSequence[9] = VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM;
    } else {
        userVolt = -pMsgWait->voltage;
        pLineObj->intSequence[9] = VP_PROFILE_CADENCE_STATE_MSG_WAIT_POLREV;
    }

    /* Scale by same factor as bit resolution */
    aVolt = userVolt * (uint32)VP890_RINGING_BIAS_FACTOR;

    /* Scale down by the bit resolution of the device */
    aVolt /= (uint32)VP890_RINGING_BIAS_SCALE;

    sigGenBytes[VP890_SIGA_BIAS_MSB] = (aVolt >> 8) & 0xFF;
    sigGenBytes[VP890_SIGA_BIAS_LSB] = (aVolt & 0xFF);

    /* Write the new signal generator parameters */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_RINGER_PARAMS_WRT,
       VP890_RINGER_PARAMS_LEN, sigGenBytes);

    /* Clear flag to indicate the generators are NOT in a Ringing Mode */
    pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);

    /*
     * Build the rest of the cadence defined by the user input (message state
     * set above). Start by setting the type of profile to an API Message Wait
     * Pulse type
     */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
        VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT;

    /*
     * Set the timers for on/off pulse duration, scale from mS to tickRate
     * and prevent rounding down to 0
     */
    if (pMsgWait->onTime < (tickRate >> 8)) {
        firstTimer = 3;
    } else {
        firstTimer = MS_TO_TICKRATE(pMsgWait->onTime, tickRate);

        /* Prevent 0 for time (because that means "forever") */
        if (firstTimer <= 2) {
            firstTimer = 3;
        }
    }

    branchCount = 0;
    if (firstTimer > 8192) {
        /* Special Handling for using 16-bit time in 14-bit data fields */
        for (; firstTimer > 8192; branchCount++) {
            firstTimer = ((firstTimer >> 1) & 0x3FFF);
        }
        cmdLen+=2;
    }

    pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = ((firstTimer - 2) >> 8) & 0x1F;
    pLineObj->intSequence[10] |= tempTime;

    tempTime = ((firstTimer - 2) & 0x00FF);
    pLineObj->intSequence[11] = tempTime;

    if (branchCount) {
        pLineObj->intSequence[12] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
        pLineObj->intSequence[12] |= 0x01;  /* On-Time is the step 1 (0 base) */
        pLineObj->intSequence[13] = branchCount;
        addStep+=2;
    }

    /*
     * If the off-time is 0, we will stay in the previous state forever so the
     * cadencer needs to stop where it is
     */
    if (pMsgWait->offTime == 0) {
        pLineObj->intSequence[VP_PROFILE_LENGTH] = cmdLen;
        pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] =
            (0x04 + addStep);
    } else {
        cmdLen+=4;   /* Add two for the next state and two for the off-time */

        /* In-between pulses we'll return to the current state */
        pLineObj->intSequence[12+addStep]
            = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
        pLineObj->intSequence[13+addStep] =
            ConvertApiState2PrfWizState(currentState);

        /*
         * Set the timers for on/off pulse duration, scale from mS to tickRate
         * and prevent rounding down to 0
         */
        if (pMsgWait->offTime < (tickRate >> 8)) {
            secondTimer = 3;
        } else {
            secondTimer = MS_TO_TICKRATE(pMsgWait->offTime, tickRate);

            /* Prevent 0 for time (because that means "forever") */
            if (secondTimer <= 2) {
                secondTimer = 3;
            }
        }

        branchCount = 0;
        if (secondTimer > 8192) {
            cmdLen+=2;   /* Add two for the off-time branch loop */
            /* Special Handling for using 16-bit time in 14-bit data fields */
            for (; secondTimer > 8192; branchCount++) {
                secondTimer = ((secondTimer >> 1) & 0x3FFF);
            }
        }

        pLineObj->intSequence[14+addStep] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
        tempTime = ((secondTimer - 2) >> 8) & 0x1F;
        pLineObj->intSequence[14+addStep] |= tempTime;

        tempTime = ((secondTimer - 2) & 0x00FF);
        pLineObj->intSequence[15+addStep] = tempTime;

        if (branchCount) {
            pLineObj->intSequence[16+addStep] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
            pLineObj->intSequence[16+addStep] |= (0x03 + (addStep / 2));
            pLineObj->intSequence[17+addStep] = branchCount;
            addStep+=2;
        }

        /*
         * If the number of cycles is 0, set the branch to repeat forever. If
         * it's 1, don't add a branch statement because the sequence should end
         * after the first cycle, otherwise subtract 1 from the total number of
         * cycles to force the correct number of "repeats" (branch)
         */

        if (pMsgWait->cycles != 1) {
            cmdLen+=2; /* Two more for this last branch operator */
            pLineObj->intSequence[16+addStep] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
            pLineObj->intSequence[17+addStep] = (pMsgWait->cycles) ?
                (pMsgWait->cycles - 1) : pMsgWait->cycles;
        }
    }

    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = cmdLen;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = cmdLen - 4;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos = &pLineObj->intSequence[8];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* SendMsgWaitPulse() */

/******************************************************************************
 * SendFwdDisc()
 *  This function sends a forward disconnect to the line specified for a duration
 * given in mS.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  A disconnect has been applied to the line, the line state is restored to what
 * it was prior to this function being called. No events are generated while a
 * disconnect is occuring -- the application should know that it is not possible
 * to detect a line condition while no feed is being presented.
 ******************************************************************************/
static VpStatusType
SendFwdDisc(
    VpLineCtxType               *pLineCtx,
    uint16                      timeInMs)
{
    Vp890LineObjectType         *pLineObj     = pLineCtx->pLineObj;
    VpLineStateType             currentState  = pLineObj->lineState.usrCurrent;
    VpProfileCadencerStateTypes cadenceState;

    VpDevCtxType                *pDevCtx      = pLineCtx->pDevCtx;
    Vp890DeviceObjectType       *pDevObj      = pDevCtx->pDevObj;
    VpDeviceIdType              deviceId      = pDevObj->deviceId;

    uint16                      timeIn5mS     = 0;
    uint8                       seqByte, index;

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SendFwdDisc() - Function invalid for FXO"));
        return VP_STATUS_INVALID_ARG;
    }

    cadenceState = ConvertApiState2PrfWizState(currentState);

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = 0x0000;    /* No active status */

    for (seqByte = 0; seqByte < VP890_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Set the cadence type */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_FWD_DISC_INT;

    /* First step is to go to disconnect */
    index = 0;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_PROFILE_CADENCE_STATE_DISCONNECT;

    /* Then wait for the time specified -- rounded to 5mS increments */
    if (timeInMs < 5) {
        timeIn5mS = 1;
    } else {
        timeIn5mS = timeInMs / 5;
    }
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        |= (timeIn5mS >> 8) & 0x1F;

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (timeIn5mS & 0xFF);

    /* Restore the line state */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = cadenceState;

    /* Then wait for 100mS for the detector to become stable */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = 20; /* 5mS per increment */

    index++; /* Adjust one more for length values */
    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = index + 4;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos =
        &pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* SendFwdDisc() */

/******************************************************************************
 * SendPolRevPulse()
 *  This function sends a polarity reversal pulse to the line specified for a
 * duration given in ms.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  A polarity reversal has been applied to the line, the line state is restored
 * to what it was prior to this function being called. A hook event may be
 * generated while the polarity reversal is occuring. The application should
 * ignore as appropriate based on expected loop conditions.
 ******************************************************************************/
static VpStatusType
SendPolRevPulse(
    VpLineCtxType               *pLineCtx,
    uint16                      timeInMs)
{
    Vp890LineObjectType         *pLineObj     = pLineCtx->pLineObj;
    VpLineStateType             currentState  = pLineObj->lineState.usrCurrent;
    VpProfileCadencerStateTypes cadenceState, polRevState;

    VpDevCtxType                *pDevCtx      = pLineCtx->pDevCtx;
    Vp890DeviceObjectType       *pDevObj      = pDevCtx->pDevObj;
    VpDeviceIdType              deviceId      = pDevObj->deviceId;

    uint16                      timeIn5mS     = 0;
    uint8                       seqByte, index;

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SendPolRevPulse() - Function invalid for FXO"));
        return VP_STATUS_INVALID_ARG;
    }

    if (currentState == VP_LINE_DISCONNECT) {
        return VP_STATUS_INVALID_ARG;
    }

    cadenceState = ConvertApiState2PrfWizState(currentState);
    polRevState = ConvertApiState2PrfWizState(VpGetReverseState(currentState));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = 0x0000;    /* No active status */

    for (seqByte = 0; seqByte < VP890_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Set the cadence type */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
        VP_PRFWZ_PROFILE_POLREV_PULSE_INT;

    /* First step is to go to polrev state */
    index = 0;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = polRevState;

    /* Then wait for the time specified -- rounded to 5mS increments */
    if (timeInMs < 5) {
        timeIn5mS = 1;
    } else {
        timeIn5mS = timeInMs / 5;
    }
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        |= (timeIn5mS >> 8) & 0x1F;

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (timeIn5mS & 0xFF);

    /* Restore the line state */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = cadenceState;

    /* Then wait for 100mS for the detector to become stable */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = 20; /* 5mS per increment */

    index++; /* Adjust one more for length values */
    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = index + 4;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos =
        &pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* SendPolRevPulse() */
#endif

#ifdef VP890_FXO_SUPPORT
/******************************************************************************
 * MomentaryLoopOpen()
 *  This function applies a Momentary Loop Open to an FXO line and tests for
 * a parallel off-hook.
 *
 * Preconditions:
 *  The line must first be initialized and must be of FXO type.
 *
 * Postconditions:
 *  A 10ms loop open is applied to the line and line state returns to previous
 * condition. An event is generated indicating if there exists a parallel phone
 * off-hook or not.
 ******************************************************************************/
static VpStatusType
MomentaryLoopOpen(
    VpLineCtxType               *pLineCtx)
{
    Vp890LineObjectType         *pLineObj    = pLineCtx->pLineObj;
    VpLineStateType             currentState = pLineObj->lineState.usrCurrent;
    VpProfileCadencerStateTypes cadenceState;

    VpDevCtxType                *pDevCtx     = pLineCtx->pDevCtx;
    Vp890DeviceObjectType       *pDevObj     = pDevCtx->pDevObj;
    VpDeviceIdType              deviceId     = pDevObj->deviceId;

    uint16                      timeIn5mS    = 0;
    uint8                       seqByte, index;

    if (!(pLineObj->status & VP890_IS_FXO)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("MomentaryLoopOpen() - Function invalid for FXS"));
        return VP_STATUS_INVALID_ARG;
    }

    switch(currentState) {
        case VP_LINE_FXO_OHT:
            cadenceState = VP_PROFILE_CADENCE_STATE_FXO_OHT;
            break;

        case VP_LINE_FXO_LOOP_OPEN:
            cadenceState = VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;
            break;

        case VP_LINE_FXO_LOOP_CLOSE:
            cadenceState = VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE;
            break;

        case VP_LINE_FXO_TALK:
            cadenceState = VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK;
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("MomentaryLoopOpen() - Invalid state"));
            return VP_STATUS_FAILURE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = 0x0000;    /* No active status */

    for (seqByte = 0; seqByte < VP890_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Set the cadence type */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
        VP_PRFWZ_PROFILE_MOMENTARY_LOOP_OPEN_INT;

    /* First step is to go to Loop Open */
    index = 0;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;

    /* Then wait for at least 320ms. The time is higher by 2*ApiTick value
       Afterward, when the cadence ends, CommandInstruction will read the LIU
       bit and generate an event if necessary */
    timeIn5mS = 64;  /* Cadencer Tick is 5ms increments */

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        |= (timeIn5mS >> 8) & 0x1F;

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (timeIn5mS & 0xFF);

    /* Restore the line state */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = cadenceState;

    index++; /* Adjust for length values */
    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = index + 4;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos =
        &pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* MomentaryLoopOpen() */

/******************************************************************************
 * SendDigit()
 *  This function sends a DTMF or Dial Pulse digit on an FXO line. It creates
 * a sequencer compatible profile to control the FXO loop open, loop close, and
 * time operators.
 *
 * Arguments:
 *  *pLineCtx - Line to send a digit on
 *  digitType - Type of digit to send. May indicate DTMF, Dial Pulse,
 *               or Hook Flash
 *  digit     - The digit to send. Used if type of digit is DTMF or Dial Pulse
 *
 * Preconditions:
 *  The line must first be initialized and must be of FXO type.
 *
 * Postconditions:
 *  The digit specified is sent on the line in the form specified (DTMF or Dial
 * Pulse).  This function returns the success code if the line is an FXO type of
 * line, if the digit is between 0 - 9, and if the digit type is either DTMF or
 * Dial Pulse.
 ******************************************************************************/
static VpStatusType
SendDigit(
    VpLineCtxType           *pLineCtx,
    VpDigitGenerationType   digitType,
    VpDigitType             digit)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    uint16                  tempTime, firstTimer, secondTimer;
    uint8                   seqByte;
    uint16                  tickAdjustment;

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
        case VP_DIGIT_GENERATION_DIAL_PULSE:
            if (!(pLineObj->status & VP890_IS_FXO)) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("SendDigit() - Function invalid for FXS"));
                return VP_STATUS_INVALID_ARG;
            }
            if ((VpIsDigit(digit) == FALSE) || (digit == VP_DIG_NONE)) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("SendDigit() - Invalid digit"));
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_DIGIT_GENERATION_DIAL_HOOK_FLASH:
            if (!(pLineObj->status & VP890_IS_FXO)) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("SendDigit() - Function invalid for FXS"));
                return VP_STATUS_INVALID_ARG;
            }

            if ((pLineObj->lineState.currentState != VP_LINE_FXO_TALK)
             && (pLineObj->lineState.currentState != VP_LINE_FXO_LOOP_CLOSE)) {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("SendDigit() - Invalid digitType"));
            return VP_STATUS_INVALID_ARG;
    }

    /* Parameters passed are good -- proceed */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;    /* No active status */

    for (seqByte = 0; seqByte < VP890_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Tick adjustment in 5ms cadence time units */
    tickAdjustment = TICKS_TO_MS(1, pDevObj->devProfileData.tickRate) / 5;

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
            Vp890MuteChannel(pLineCtx, TRUE);
            Vp890SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

            /* Fixed total length and sequence length for FLASH generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0C;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x08;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_SIGGEN);

            pLineObj->intSequence[12]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_SIGGEN);

            pLineObj->intSequence[9] =
                (VP_SEQ_SIGGEN_C_EN | VP_SEQ_SIGGEN_D_EN);

            pLineObj->intSequence[13] = VP_SEQ_SIGGEN_ALL_DISABLED;

            firstTimer = pLineObj->digitGenStruct.dtmfOnTime;
            if (firstTimer > tickAdjustment) {
                firstTimer -= tickAdjustment;
            } else {
                firstTimer = 1;
            }
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (firstTimer >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = (firstTimer & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.dtmfOffTime;
            if (secondTimer > tickAdjustment) {
                secondTimer -= tickAdjustment;
            } else {
                secondTimer = 1;
            }
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (secondTimer >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = (secondTimer & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
                VP_PRFWZ_PROFILE_DTMF_DIG_GEN;
            break;

        case VP_DIGIT_GENERATION_DIAL_PULSE:
            /* Fixed total length and sequence length for DP generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x10;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x0C;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

            pLineObj->intSequence[12]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

            if (pLineObj->lineState.currentState == VP_LINE_FXO_TALK) {
                pLineObj->intSequence[9] =
                    VP_PROFILE_CADENCE_STATE_FXO_OHT;
                pLineObj->intSequence[13] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK;
            } else {
                pLineObj->intSequence[9] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;
                pLineObj->intSequence[13] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE;
            }

            firstTimer = pLineObj->digitGenStruct.breakTime;
            if (firstTimer > tickAdjustment) {
                firstTimer -= tickAdjustment;
            } else {
                firstTimer = 1;
            }
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (firstTimer >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = (firstTimer & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.makeTime;
            if (secondTimer > tickAdjustment * 2) {
                secondTimer -= tickAdjustment * 2;
            } else {
                secondTimer = 1;
            }
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (secondTimer >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = (secondTimer & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            firstTimer = pLineObj->digitGenStruct.dpInterDigitTime;
            if (digit > 1) {
                pLineObj->intSequence[16] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
                pLineObj->intSequence[17] = digit - 1;

                pLineObj->intSequence[18] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
                tempTime = (firstTimer >> 8) & 0x03;
                pLineObj->intSequence[18] |= tempTime;
                tempTime = (firstTimer & 0x00FF);
                pLineObj->intSequence[19] |= tempTime;
            } else {
                pLineObj->intSequence[16] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
                tempTime = (firstTimer >> 8) & 0x03;
                pLineObj->intSequence[16] |= tempTime;
                tempTime = (firstTimer & 0x00FF);
                pLineObj->intSequence[17] |= tempTime;

                pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0E;
                pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB]
                    = 0x0A;
            }

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
                VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN;
            break;

        case VP_DIGIT_GENERATION_DIAL_HOOK_FLASH:
            /* Fixed total length and sequence length for FLASH generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0C;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x08;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

            pLineObj->intSequence[12]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

            if (pLineObj->lineState.currentState == VP_LINE_FXO_TALK) {
                pLineObj->intSequence[9] =
                    VP_PROFILE_CADENCE_STATE_FXO_OHT;
                pLineObj->intSequence[13] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK;
            } else {
                pLineObj->intSequence[9] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;
                pLineObj->intSequence[13] =
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE;
            }

            firstTimer = pLineObj->digitGenStruct.flashTime;
            if (firstTimer > tickAdjustment) {
                firstTimer -= tickAdjustment;
            } else {
                firstTimer = 1;
            }
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (firstTimer >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = (firstTimer & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.dpInterDigitTime;
            if (secondTimer > tickAdjustment) {
                secondTimer -= tickAdjustment;
            } else {
                secondTimer = 1;
            }
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = (secondTimer >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = (secondTimer & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
                VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN;
            break;

        default:
            /*
             * This can only occur if there is an error in the error checking
             * above.
             */
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            VP_ERROR(VpLineCtxType, pLineCtx, ("SendDigit() - Invalid digitType; should not get here"));
            return VP_STATUS_INVALID_ARG;
    }

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos = &pLineObj->intSequence[8];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* SendDigit() */
#endif

/*******************************************************************************
 * Vp890CommandInstruction()
 *  This function implements the Sequencer Command instruction for the Vp890
 * device type.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The command instruction currently being pointed to by the sequencer
 * instruction passed is acted upon.  The sequencer may or may not be advanced,
 * depending on the specific command instruction being executed.
 ******************************************************************************/
VpStatusType
Vp890CommandInstruction(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pSeqData)
{
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;

    uint8                   ecVal     = pLineObj->ecVal;
    uint8                   channelId = pLineObj->channelId;

#ifdef VP890_FXS_SUPPORT
    uint8                   lineState;
    uint8                   lsConfig[VP890_LOOP_SUP_LEN];
    uint16                  tempFreq, tempLevel;
#endif

    /*
     * We know the current value "pSeqData[0]" is 0, now we need to determine if
     * the next command is generator control operator followed by time, or a
     * Line state command -- No other options supported
     */
    VP_SEQUENCER(VpLineCtxType, pLineCtx, ("890 Command 0x%02X", pSeqData[0]));

    switch (pSeqData[0] & VP_SEQ_SUBTYPE_MASK) {
        case VP_SEQ_SUBCMD_SIGGEN: {
            /* Disable everything. */
            uint8 sigGenCtrl[] = {0};

            /* Get the signal generator bits and set. */
            sigGenCtrl[0] |= ((pSeqData[1] & 0x01) ?  VP890_GENA_EN : 0);
            sigGenCtrl[0] |= ((pSeqData[1] & 0x02) ?  VP890_GENB_EN : 0);
            sigGenCtrl[0] |= ((pSeqData[1] & 0x04) ?  VP890_GENC_EN : 0);
            sigGenCtrl[0] |= ((pSeqData[1] & 0x08) ?  VP890_GEND_EN : 0);

            if (pLineObj->sigGenCtrl[0] != sigGenCtrl[0]) {
                pLineObj->sigGenCtrl[0] = sigGenCtrl[0];
                VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT,
                    VP890_GEN_CTRL_LEN, pLineObj->sigGenCtrl);
            }
            }
            break;

        case VP_SEQ_SUBCMD_LINE_STATE:
            switch(pSeqData[1]) {
#ifdef VP890_FXS_SUPPORT
                case VP_PROFILE_CADENCE_STATE_STANDBY:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_STANDBY);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_STANDBY:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_STANDBY_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_TALK:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_TALK);
                    break;

                case VP_PROFILE_CADENCE_STATE_ACTIVE:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_ACTIVE);
                    break;

                case VP_PROFILE_CADENCE_STATE_OHT:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_OHT);
                    break;

                case VP_PROFILE_CADENCE_STATE_DISCONNECT:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_DISCONNECT);
                    break;

                case VP_PROFILE_CADENCE_STATE_RINGING:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_RINGING);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_RINGING:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_RINGING_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_ACTIVE_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_TALK:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_TALK_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_OHT:
                    Vp890SetFxsLineState(pLineCtx, VP_LINE_OHT_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM:
                case VP_PROFILE_CADENCE_STATE_MSG_WAIT_POLREV:
                    VpMemCpy(lsConfig, pLineObj->loopSup, VP890_LOOP_SUP_LEN);
                    if (lsConfig[VP890_LOOP_SUP_RT_MODE_BYTE]
                        & VP890_RING_TRIP_AC) {
                        if (!(pLineObj->status & VP890_BAD_LOOP_SUP)) {
                            pLineObj->status |= VP890_BAD_LOOP_SUP;
                        }

                        /* Force DC Trip */
                        lsConfig[VP890_LOOP_SUP_RT_MODE_BYTE] &=
                            ~VP890_RING_TRIP_AC;
                        VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT,
                            VP890_LOOP_SUP_LEN, lsConfig);
                    }

                    lineState =
                        (pSeqData[1] == VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM) ?
                        VP890_SS_BALANCED_RINGING :
                        VP890_SS_BALANCED_RINGING_PR;

                    Vp890LLSetSysState(deviceId, pLineCtx, lineState, TRUE);
                    break;
#endif

#ifdef VP890_FXO_SUPPORT
                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN:
                    Vp890SetFxoLineState(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_OHT:
                    Vp890SetFxoLineState(pLineCtx, VP_LINE_FXO_OHT);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE:
                    Vp890SetFxoLineState(pLineCtx, VP_LINE_FXO_LOOP_CLOSE);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK:
                    Vp890SetFxoLineState(pLineCtx, VP_LINE_FXO_TALK);
                    break;
#endif

                default:
                    VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CommandInstruction() - Invalid sequencer state command"));
                    return VP_STATUS_INVALID_ARG;
            }
            break;

#ifdef VP890_FXS_SUPPORT
        case VP_SEQ_SUBCMD_START_CID:
        case VP_SEQ_SUBCMD_WAIT_ON:
            if (pLineObj->pCidProfileType1 != VP_PTABLE_NULL) {
                pLineObj->callerId.pCliProfile = pLineObj->pCidProfileType1;

                pLineObj->callerId.status |= VP_CID_IN_PROGRESS;
                if ((pSeqData[0] & VP_SEQ_SUBTYPE_MASK) == VP_SEQ_SUBCMD_WAIT_ON) {
                    VP_CID(VpLineCtxType, pLineCtx, ("890 Command WAIT_ON_CID"));
                    pLineObj->callerId.status |= VP_CID_WAIT_ON_ACTIVE;
                } else {
                    VP_CID(VpLineCtxType, pLineCtx, ("890 Command START_CID"));
                }
                pLineObj->callerId.cliTimer = 1;

                pLineObj->callerId.cliIndex = 0;
                pLineObj->callerId.cliMPIndex = 0;
                pLineObj->callerId.cliMSIndex = 0;

                pLineObj->callerId.status &= ~VP_CID_SIG_B_VALID;

                pLineObj->callerId.status |= VP_CID_PRIMARY_IN_USE;
                pLineObj->callerId.status &= ~VP_CID_SECONDARY_IN_USE;
            }
            break;

        case VP_SEQ_SUBCMD_RAMP_GENERATORS:
            tempFreq = (pLineObj->cadence.regData[3] << 8);
            tempFreq |= pLineObj->cadence.regData[4];

            tempLevel = (pLineObj->cadence.regData[5] << 8);
            tempLevel |= pLineObj->cadence.regData[6];

            if (pLineObj->cadence.isFreqIncrease == TRUE) {
                /* Check if we're at or above the max frequency */
                if (tempFreq >= pLineObj->cadence.stopFreq) {
                    pLineObj->cadence.isFreqIncrease = FALSE;
                    tempFreq -= pLineObj->cadence.freqStep;
                } else {
                    tempFreq += pLineObj->cadence.freqStep;
                }
            } else {
                if (tempFreq <
                    (pLineObj->cadence.startFreq - pLineObj->cadence.freqStep)) {
                    pLineObj->cadence.isFreqIncrease = TRUE;
                    tempFreq += pLineObj->cadence.freqStep;
                } else {
                    tempFreq -= pLineObj->cadence.freqStep;
                }
            }
            pLineObj->cadence.regData[3] = (tempFreq >> 8) & 0xFF;
            pLineObj->cadence.regData[4] = tempFreq & 0xFF;

            /*
             * Check if we're at or above the max level, but make sure we don't
             * wrap around
             */
            if (tempLevel <
                (pLineObj->cadence.stopLevel -
                    ((tempLevel * pLineObj->cadence.levelStep) / 10000))) {

                tempLevel += ((tempLevel * pLineObj->cadence.levelStep) / 10000);

                pLineObj->cadence.regData[5] = (tempLevel >> 8) & 0xFF;
                pLineObj->cadence.regData[6] = tempLevel & 0xFF;
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SIGAB_PARAMS_WRT,
                VP890_SIGAB_PARAMS_LEN, pLineObj->cadence.regData);
            /* Clear flag to indicate the generators are NOT in a Ringing Mode */
            pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);
            break;
#endif

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CommandInstruction() - Invalid sequencer command"));
            return VP_STATUS_INVALID_ARG;
    }

    /*
     * Check to see if there is more sequence data, and if so, move the
     * sequence pointer to the next command. Otherwise, end this cadence
     */
    pLineObj->cadence.index+=2;
    VP_SEQUENCER(VpLineCtxType, pLineCtx, ("New Index %d Length %d",
        pLineObj->cadence.index, pLineObj->cadence.length));

    if (pLineObj->cadence.index <
       (pLineObj->cadence.length + VP_PROFILE_LENGTH + 1)) {
        pSeqData+=2;
        pLineObj->cadence.pCurrentPos = pSeqData;

        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Continue Cadence..."));
    } else {
        VP_SEQUENCER(VpLineCtxType, pLineCtx, ("End Cadence"));

        switch(pLineObj->cadence.pActiveCadence[VP_PROFILE_TYPE_LSB]) {

            case VP_PRFWZ_PROFILE_RINGCAD:
                pLineObj->lineEvents.process |= VP_LINE_EVID_RING_CAD;
                pLineObj->processData = VP_RING_CAD_DONE;
                break;

            case VP_PRFWZ_PROFILE_TONECAD:
                pLineObj->lineEvents.process |= VP_LINE_EVID_TONE_CAD;
                break;

            case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_HOOK_FLASH;
                break;

            case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_PULSE_DIGIT;
                break;

            case VP_PRFWZ_PROFILE_MOMENTARY_LOOP_OPEN_INT:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_MOMENTARY_LOOP_OPEN;
                if (pDevObj->intReg[channelId] & VP890_LIU_MASK) {
                    pLineObj->lineEventHandle = 1;
                } else {
                    pLineObj->lineEventHandle = 0;
                }
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT,
                    VP890_LOOP_SUP_LEN, pLineObj->loopSup);
                break;

            case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_DTMF_DIGIT;
                Vp890MuteChannel(pLineCtx, FALSE);
                break;

            case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_MSG_WAIT_PULSE;
                VpSetLineState(pLineCtx, pLineObj->lineState.usrCurrent);
                break;

            default:
                break;

        }
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
    }

    return VP_STATUS_SUCCESS;
} /* Vp890CommandInstruction() */

#ifdef VP890_FXS_SUPPORT
/*******************************************************************************
 * Vp890FSKGeneratorReady()
 *  This function is used for Caller ID to determine if the FSK generator is
 *  ready to accept another byte. It uses the device caller ID state machine
 *  and signaling (caller ID status) register. This function should be called
 *  from an API internal function only.
 *
 * Prototype is in vp_api_int.h
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Returns:
 *  TRUE if the FSK generator for Caller ID can accept a byte, FALSE otherwise.
 ******************************************************************************/
bool
Vp890FSKGeneratorReady(
    VpLineCtxType         *pLineCtx)
{
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    VpDeviceIdType        deviceId  = pDevObj->deviceId;
    uint8                 ecVal     = pLineObj->ecVal;
    uint8                 cidState;

    /* Check the Generator State */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CID_PARAM_RD,
        VP890_CID_PARAM_LEN, pLineObj->cidParam);
    cidState = (pLineObj->cidParam[0] & VP890_CID_STATE_MASK);

    /* Check to see if the device is ready to accept (more) CID data */
    if (cidState == VP890_CID_STATE_FULL_D) {
        VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Generator FULL"));
        return FALSE;
    } else {
        VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Generator READY"));
        return TRUE;
    }
} /* Vp890FSKGeneratorReady() */

/*******************************************************************************
 * Vp890CliGetEncodedByte()
 *  This function returns an encoded byte of data that is suitable for writing
 *  the FSK generator (device dependent).
 *
 * Preconditions
 *  Must have a valid CLI packet in to work from.
 *
 * Postconditions
 *  The per-channel caller ID buffer will be updated with encoded data.
 *
 ******************************************************************************/
bool
Vp890CliGetEncodedByte(
    VpLineCtxType           *pLineCtx,
    uint8                   *pByte)
{
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    VpOptionEventMaskType   *pLineEvents    = &(pLineObj->lineEvents);
    VpCallerIdType          *pCidStruct     = &(pLineObj->callerId);

    uint8                   nextByte        = '\0';

    uint8                   checkSumIndex   = VP_CID_PROFILE_FSK_PARAM_LEN
                + pLineObj->callerId.pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN]
                + VP_CID_PROFILE_CHECKSUM_OFFSET_LSB;

    if (pLineObj->callerId.status & VP_CID_MID_CHECKSUM) {
        pLineObj->callerId.status &= ~VP_CID_MID_CHECKSUM;
        pCidStruct->status |= VP_CID_END_OF_MSG;
        *pByte = '\0';
        return FALSE;
    }

    /* Check to determine which buffer is in use to index the message data */
    if (pCidStruct->status & VP_CID_PRIMARY_IN_USE) {
        /*
         * If the index is at the length of the buffer, we need to switch
         * buffers if there is more data
         */
        if (pCidStruct->cliMPIndex >= pCidStruct->primaryMsgLen) {
            /*
             * At the end of the Primary Buffer. Flag an event and indicate to
             * the API that this buffer is no longer being used and we can
             * accept more data
             */
            pCidStruct->status &= ~VP_CID_PRIMARY_IN_USE;
            pCidStruct->status &= ~VP_CID_PRIMARY_FULL;

            if (pCidStruct->status & VP_CID_SECONDARY_FULL) {
                pLineEvents->process |= VP_LINE_EVID_CID_DATA;
                pLineObj->processData = VP_CID_DATA_NEED_MORE_DATA;

                pCidStruct->status |= VP_CID_SECONDARY_IN_USE;
                pCidStruct->cliMSIndex = 1;
                *pByte = pCidStruct->secondaryBuffer[0];
                nextByte = pCidStruct->secondaryBuffer[1];

                VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Buffer -- Switching to Secondary 0x%02X",
                    *pByte));
            } else {
                if (pLineObj->callerId.pCliProfile[checkSumIndex]) {
                    *pByte = (uint8)(~pLineObj->callerId.cidCheckSum + 1);
                    pLineObj->callerId.status |= VP_CID_MID_CHECKSUM;

                    VP_CID(VpLineCtxType, pLineCtx, ("1. 890 FSK Buffer -- From Primary 0x%02X",
                        *pByte));
                } else {
                    *pByte = '\0';
                }
            }
        } else {
            *pByte = pCidStruct->primaryBuffer[pCidStruct->cliMPIndex];

            /* Get the next byte to be sent after the current byte */
            if ((pCidStruct->cliMPIndex+1) >= pCidStruct->primaryMsgLen) {
                if (pCidStruct->status & VP_CID_SECONDARY_FULL) {
                    nextByte = pCidStruct->secondaryBuffer[0];

                    VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Buffer -- From Secondary 0x%02X",
                        *pByte));
                }
            } else {
                nextByte =
                    pCidStruct->primaryBuffer[pCidStruct->cliMPIndex+1];

                    VP_CID(VpLineCtxType, pLineCtx, ("2. 890 FSK Buffer -- From Primary 0x%02X",
                        *pByte));
            }
        }
        pCidStruct->cliMPIndex++;
    } else if (pCidStruct->status & VP_CID_SECONDARY_IN_USE) {
        /*
         * If the index is at the length of the buffer, we need to switch
         * buffers if there is more data
         */
        if (pCidStruct->cliMSIndex >= pCidStruct->secondaryMsgLen) {
            /*
             * At the end of the Secondary Buffer. Flag an event and indicate to
             * the API that this buffer is no longer being used and is empty
             */
            pLineEvents->process |= VP_LINE_EVID_CID_DATA;
            pLineObj->processData = VP_CID_DATA_NEED_MORE_DATA;

            pCidStruct->status &= ~VP_CID_SECONDARY_IN_USE;
            pCidStruct->status &= ~VP_CID_SECONDARY_FULL;

            if (pCidStruct->status & VP_CID_PRIMARY_FULL) {
                pLineEvents->process |= VP_LINE_EVID_CID_DATA;
                pLineObj->processData = VP_CID_DATA_NEED_MORE_DATA;

                pCidStruct->status |= VP_CID_PRIMARY_IN_USE;
                pCidStruct->cliMPIndex = 1;
                *pByte = pCidStruct->primaryBuffer[0];
                nextByte = pCidStruct->primaryBuffer[1];
                VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Buffer -- Switching to Primary 0x%02X",
                    *pByte));
            } else {
                /* There is no more data in either buffer */
                if (pLineObj->callerId.pCliProfile[checkSumIndex]) {
                    *pByte = (uint8)(~pLineObj->callerId.cidCheckSum + 1);
                    pLineObj->callerId.status |= VP_CID_MID_CHECKSUM;
                    VP_CID(VpLineCtxType, pLineCtx, ("890 FSK Buffer -- Checksum 0x%02X",
                        *pByte));
                } else {
                    *pByte = '\0';
                }
            }
        } else {
            *pByte = pCidStruct->secondaryBuffer[pCidStruct->cliMSIndex];

            /* Get the next byte to be sent after the current byte */
            if ((pCidStruct->cliMSIndex+1) >= pCidStruct->secondaryMsgLen) {
                if (pCidStruct->status & VP_CID_PRIMARY_FULL) {
                    nextByte = pCidStruct->primaryBuffer[0];
                    VP_CID(VpLineCtxType, pLineCtx, ("3. 890 FSK Buffer -- From Primary 0x%02X",
                        *pByte));
                }
            } else {
                nextByte =
                    pCidStruct->secondaryBuffer[pCidStruct->cliMSIndex+1];
                    VP_CID(VpLineCtxType, pLineCtx, ("2. 890 FSK Buffer -- From Secondary 0x%02X",
                        *pByte));
            }
        }
        pCidStruct->cliMSIndex++;
    }

    if ((!(pCidStruct->status & VP_CID_PRIMARY_IN_USE))
     && (!(pCidStruct->status & VP_CID_SECONDARY_IN_USE))) {
        if(pCidStruct->status & VP_CID_MID_CHECKSUM) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if ((nextByte == '\0')
    && (!(pLineObj->callerId.pCliProfile[checkSumIndex]))) {
        VP_CID(VpLineCtxType, pLineCtx, ("890 Caller ID -- EOM"));
        pCidStruct->status |= VP_CID_END_OF_MSG;
    }

    return TRUE;
} /* Vp890CliGetEncodedByte() */

/*******************************************************************************
 * Vp890CtrlSetCliTone()
 *  This function is called by the API internally to enable or disable the
 *  signal generator used for Caller ID.
 *
 * Preconditions:
 *  The line context must be valid (pointing to a Vp890 line object type
 *
 * Postconditions:
 *  The signal generator used for CID tones is enabled/disabled indicated by
 *  the mode parameter passed.
 *
 ******************************************************************************/
VpStatusType
Vp890CtrlSetCliTone(
    VpLineCtxType         *pLineCtx,
    bool                  mode)
{
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType        deviceId  = pDevObj->deviceId;
    uint8                 ecVal     = pLineObj->ecVal;
    uint8                 sigGenCtrl[] = {0};

    /*
     * This function should only be called when the Caller ID sequence is
     * generating an alerting tone. We're using the C/D generators, so disable
     * A/B and enable C/D only (if mode == TRUE).
     */
    if (mode == TRUE) {
        sigGenCtrl[0] = (VP890_GENC_EN | VP890_GEND_EN);
    }

    if (pLineObj->sigGenCtrl[0] != sigGenCtrl[0]) {
        pLineObj->sigGenCtrl[0] = sigGenCtrl[0];
        VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT, VP890_GEN_CTRL_LEN,
            pLineObj->sigGenCtrl);
    }

    return VP_STATUS_SUCCESS;
} /* Vp890CtrlSetCliTone() */

/*******************************************************************************
 * Vp890CtrlSetFSKGen()
 *  This function is called by the CID sequencer executed internally by the API
 *
 * Preconditions:
 *  The line context must be valid (pointing to a VP890 line object type
 *
 * Postconditions:
 *  The data indicated by mode and data is applied to the line. Mode is used
 * to indicate whether the data is "message", or a special character. The
 * special characters are "channel siezure" (alt. 1/0), "mark" (all 1), or
 * "end of transmission".
 *
 ******************************************************************************/
void
Vp890CtrlSetFSKGen(
    VpLineCtxType               *pLineCtx,
    VpCidGeneratorControlType   mode,
    uint8                       data)
{
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType        deviceId  = pDevObj->deviceId;
    uint8                 ecVal     = pLineObj->ecVal;
    bool                  moreData  = TRUE;
    uint8                 fskParam[VP890_CID_PARAM_LEN];

    fskParam[0] = pLineObj->cidParam[0];
    fskParam[0] &= ~(VP890_CID_FRAME_BITS);

    switch(mode) {
        case VP_CID_SIGGEN_EOT:
            if (data == 0) {
                /* Stop Transmission Immediately */
                Vp890MuteChannel(pLineCtx, FALSE);
                fskParam[0] |= VP890_CID_DIS;
            } else {
                /* Wait until the device is complete */
                pLineObj->suspendCid = TRUE;
            }
            moreData = FALSE;
            break;

        case VP_CID_GENERATOR_DATA:
            Vp890MuteChannel(pLineCtx, TRUE);

            fskParam[0] |= (VP890_CID_FB_START_0 | VP890_CID_FB_STOP_1);
            fskParam[0] &= ~(VP890_CID_DIS);
            if ((pLineObj->callerId.status & VP_CID_END_OF_MSG) ||
                (pLineObj->callerId.status & VP_CID_MID_CHECKSUM)) {
                fskParam[0] |= VP890_CID_EOM;
            } else {
                fskParam[0] &= ~(VP890_CID_EOM);
            }
            break;

        case VP_CID_GENERATOR_KEYED_CHAR:
            Vp890MuteChannel(pLineCtx, TRUE);
            fskParam[0] &= ~(VP890_CID_EOM | VP890_CID_DIS);

            switch(data) {
                case VP_FSK_CHAN_SEIZURE:
                    fskParam[0] |=
                        (VP890_CID_FB_START_0 | VP890_CID_FB_STOP_1);
                    break;

                case VP_FSK_MARK_SIGNAL:
                    fskParam[0] |=
                        (VP890_CID_FB_START_1 | VP890_CID_FB_STOP_1);
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (fskParam[0] != pLineObj->cidParam[0]) {
        pLineObj->cidParam[0] = fskParam[0];
        VpMpiCmdWrapper(deviceId, ecVal, VP890_CID_PARAM_WRT,
            VP890_CID_PARAM_LEN, pLineObj->cidParam);
    }

    if (moreData == TRUE) {
        VP_CID(VpLineCtxType, pLineCtx, ("890 Caller ID Data 0x%02X",
            data));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CID_DATA_WRT,
            VP890_CID_DATA_LEN, &data);
    }

    return;
} /* Vp890CtrlSetFSKGen() */

/*******************************************************************************
 * Vp890LoadCidBuffers()
 *  This function loads cid data into the line objects cid buffers
 *  If length is within the size of just the primary buffer size, then only
 *  fill the primary buffer. Otherwise (the length exceeds the size of the
 *  primary buffer size) "low fill" the primary buffer and max fill the
 *  secondary buffer. This has the affect of causing a CID Data event
 *  quickly and giving the application a maximum amount of time to refill
 *  the message buffer
 *
 * Arguments:
 *   length     -
 *   *pCid      -
 *   pCidData   -
 *
 * Preconditions:
 *  none are needed
 *
 * Postconditions:
 * Caller ID information is saved to a line objects primary and secondary
 * CID buffers.
 *
 ******************************************************************************/
void
Vp890LoadCidBuffers(
    uint8           length,
    VpCallerIdType  *pCid,
    uint8p          pCidData)
{
    uint8 byteCnt1, byteCnt2;

    if (length <= VP_SIZEOF_CID_MSG_BUFFER) {
        pCid->primaryMsgLen = length;
        pCid->secondaryMsgLen = 0;
    } else {
        pCid->primaryMsgLen = (length - VP_SIZEOF_CID_MSG_BUFFER);
        pCid->secondaryMsgLen = VP_SIZEOF_CID_MSG_BUFFER;
    }

    /*
     * Copy the message data to the primary API buffer. If we're here, there's
     * at least one byte of primary message data. So a check is not necessary
     */
    pCid->status |= VP_CID_PRIMARY_FULL;
    for (byteCnt1 = 0; byteCnt1 < pCid->primaryMsgLen; byteCnt1++) {
        pCid->primaryBuffer[byteCnt1] = pCidData[byteCnt1];
        pCid->cidCheckSum += pCidData[byteCnt1];
        pCid->cidCheckSum = pCid->cidCheckSum % 256;
    }

    /* Copy the message data to the secondary API buffer if there is any */
    if (pCid->secondaryMsgLen > 0) {
        pCid->status |= VP_CID_SECONDARY_FULL;
        for (byteCnt2 = 0; (byteCnt2 < pCid->secondaryMsgLen); byteCnt2++) {
            pCid->secondaryBuffer[byteCnt2] = pCidData[byteCnt2 + byteCnt1];
            pCid->cidCheckSum += pCidData[byteCnt2 + byteCnt1];
            pCid->cidCheckSum =  pCid->cidCheckSum % 256;
        }
    }
    return;
} /* Vp890LoadCidBuffers() */

/*******************************************************************************
 * Vp890SendCid()
 *  This function may be used to send Caller ID information on-demand. It
 * accepts an amount of CID message data up to a "full" buffer amount (2 times
 * the amount of the size used for ContinueCID). It low fills the primary buffer
 * such that the application is interrupted at the earliest time when the API
 * is ready to accept more data.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. The length of the
 * message (indicated by the length field passed) must not exceed the buffer
 * size.
 *
 * Postconditions:
 * Caller ID information is transmitted on the line.
 ******************************************************************************/
VpStatusType
Vp890SendCid(
    VpLineCtxType           *pLineCtx,
    uint8                   length,
    VpProfilePtrType        pCidProfile,
    uint8p                  pCidData)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpCallerIdType          *pCid       = &pLineObj->callerId;

    VpProfilePtrType        pCidProfileLocal;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendCid() - FXO does not support CID"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length > (2 * VP_SIZEOF_CID_MSG_BUFFER)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendCid() - length exceeds internal msg buffer"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    /* Check the legality of the Ring CID profile */
    if (!Vp890IsProfileValid(VP_PROFILE_CID,
        VP_CSLAC_CALLERID_PROF_TABLE_SIZE,
        pDevObj->profEntry.cidCadProfEntry,
        pDevObj->devProfileTable.pCallerIdProfileTable,
        pCidProfile, &pCidProfileLocal))
    {
        return VP_STATUS_ERR_PROFILE;
    }

    /* Can't send a null profile */
    if (pCidProfileLocal == VP_PTABLE_NULL) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SendCid() - NULL CID profile is not allowed"));
        return VP_STATUS_ERR_PROFILE;
    }

    /* If we're here, all parameters passed are valid */
    VpSysEnterCritical(pDevObj->deviceId, VP_CODE_CRITICAL_SEC);

    pCid->pCliProfile = pCidProfileLocal;
    pCid->cliIndex = 0;
    pCid->cliMPIndex = 0;
    pCid->cliMSIndex = 0;

    pCid->status |= VP_CID_IN_PROGRESS;
    pCid->status &= ~VP_CID_SIG_B_VALID;
    pCid->status &= ~VP_CID_REPEAT_MSG;
    pCid->status &= ~VP_CID_END_OF_MSG;
    pCid->cliTimer = 1;
    pCid->cidCheckSum = 0;

    /* load up the internal CID buffers */
    Vp890LoadCidBuffers(length, pCid, pCidData);

    pCid->status |= VP_CID_PRIMARY_IN_USE;
    pCid->status &= ~VP_CID_SECONDARY_IN_USE;

    VpSysExitCritical(pDevObj->deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;

} /* Vp890SendCid() */

/*******************************************************************************
 * Vp890ContinueCid()
 *  This function is called to provide more caller ID data (in response to
 *  Caller ID data event from the VP-API). See VP-API-II  documentation
 *  for more information about this function.
 *
 *  When this function is called, the buffer that is in use is flagged
 *  by the VpCliGetEncodeByte() function in vp_api_common.c file. That
 *  function implements the logic of when to switch between the primary
 *  and secondary buffer. This function just needs to fill the bufffer that
 *  is not currently in use, starting with the primary (because the primary
 *  buffer is also used first for the first part of the message).
 *
 * Arguments:
 *  *pLineCtx   -
 *  length      -
 *  pCidData    -
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 *  devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Continues to transmit Caller ID information on the line.
 ******************************************************************************/
VpStatusType
Vp890ContinueCid(
    VpLineCtxType           *pLineCtx,
    uint8                   length,
    uint8p                  pCidData)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpCallerIdType          *pCid       = &pLineObj->callerId;

    uint8                   byteCount   = 0;
    uint8                   *pBuffer;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890ContinueCid() - FXO does not support CID"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length > (VP_SIZEOF_CID_MSG_BUFFER)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890ContinueCid() - length exceeds internal msg buffer"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    VpSysEnterCritical(pDevObj->deviceId, VP_CODE_CRITICAL_SEC);

    if (!(pLineObj->callerId.status & VP_CID_PRIMARY_IN_USE)) {
        /* Fill the primary buffer */
        pCid->status |= VP_CID_PRIMARY_FULL;
        pCid->primaryMsgLen = length;
        pBuffer = &(pCid->primaryBuffer[0]);
    } else {
        /* Fill the secondary buffer */
        pCid->status |= VP_CID_SECONDARY_FULL;
        pCid->secondaryMsgLen = length;
        pBuffer = &(pCid->secondaryBuffer[0]);
    }

    /* Copy the message data to the API buffer */
    for (byteCount = 0; (byteCount < length); byteCount++) {
        pBuffer[byteCount] = pCidData[byteCount];

        pCid->cidCheckSum += pBuffer[byteCount];
        pCid->cidCheckSum = pCid->cidCheckSum % 256;
    }
    VpSysExitCritical(pDevObj->deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* Vp890ContinueCid() */
#endif
#endif /* VP_CSLAC_SEQ_EN */
#endif
