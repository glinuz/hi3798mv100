/** \file vp890_control.c
 * vp890_control.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Control Functions.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7548 $
 * $LastChangedDate: 2010-07-23 16:48:21 -0500 (Fri, 23 Jul 2010) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES)  /* Compile only if required */

#include    "vp_api_int.h"
#include    "vp890_api_int.h"

/* =================================
    Prototypes for Static Functions
   ================================= */
#ifdef VP890_FXS_SUPPORT
static void
Vp890RestartComplete(
    VpDevCtxType *pDevCtx);
#endif

static void
Vp890ServiceInterrupts(
    VpDevCtxType            *pDevCtx);

static void
Vp890ServiceDevTimers(
    VpDevCtxType            *pDevCtx);

static bool
Vp890FindSoftwareInterrupts(
    VpDevCtxType            *pDevCtx);

static VpStatusType
SetDeviceOption(
    VpDevCtxType            *pDevCtx,
    VpDeviceIdType          deviceId,
    VpOptionIdType          option,
    void                    *value);

static VpStatusType
SetLineOption(
    VpLineCtxType           *pLineCtx,
    VpDeviceIdType          deviceId,
    VpOptionIdType          option,
    void                    *value);

static void
SetOptionEventMask(
    Vp890DeviceObjectType   *pDevObj,
    Vp890LineObjectType     *pLineObj,
    VpDeviceIdType          deviceId,
    uint8                   ecVal,
    void                    *value);

/* Function called by SetOptionInternal for Event Masking only */
static void
MaskNonSupportedEvents(
    VpOptionEventMaskType   *pLineEventsMask,
    VpOptionEventMaskType   *pDevEventsMask);

/* Function called by SetOptionInternal to set tx and rx timeslot */
static VpStatusType
SetTimeSlot(
    VpLineCtxType           *pLineCtx,
    uint8                   txSlot,
    uint8                   rxSlot);

/**< Profile index for Generator A/B and C/D starting points (std tone) */
typedef enum
{
    VP890_SIGGEN_AB_START = 8,
    VP890_SIGGEN_CD_START = 16,
    VP890_SIGGEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp890_toneProfileParams;

/*******************************************************************************
 * Vp890ApiTick()
 *  This function should be called on a periodic basis or attached to an
 * interrupt.
 *
 * Arguments:
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  The value passed (by pointer) is set to TRUE if there is an updated event.
 * The user should call the GetEventStatus function to determine the cause of
 * the event (TRUE value set).  This function always returns the success code.
 ******************************************************************************/
VpStatusType
Vp890ApiTick(
    VpDevCtxType        *pDevCtx,
    bool                *pEventStatus)
{
    VpLineCtxType           *pLineCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    uint8                   channelId;
    uint8                   maxChan     = pDevObj->staticInfo.maxChannels;
    uint8                   ecVal;
    uint8                   numIntServiced = 2;
    bool                    tempClkFault, tempBat1Fault, lineInTest, intServiced;

#ifdef VP_CSLAC_SEQ_EN
    bool isSeqRunning = FALSE;
#endif

    uint16                  timeStampPre, tickAdder;

    *pEventStatus = FALSE;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * Can't allow tick functions to proceed until Init Device function has
     * been called. Otherwise, "tickrate" is unknown and initally 0.
     */
    if (pDevObj->devProfileData.tickRate == 0) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * The timestamp is in 0.5mS increments, but the device tickrate is
     * something else. So increment by the scaled amount and detect rollover
     * by finding if the previous value is greater than the new value.
     */
    timeStampPre = pDevObj->timeStamp;
    tickAdder = pDevObj->devProfileData.tickRate / VP_CSLAC_TICKSTEP_0_5MS;
    pDevObj->timeStamp+=tickAdder;

    if (timeStampPre > pDevObj->timeStamp) {
        pDevObj->deviceEvents.signaling |= VP_DEV_EVID_TS_ROLLOVER;
    }

#if defined (VP890_INTERRUPT_LEVTRIG_MODE)
    VpSysEnableInt(deviceId);
#endif

    /* Check if since last tick, one of the lines changed to/from WideBand mode */
    if (pDevObj->lastCodecChange != 0) {
        VpOptionCodecType codecMode;
        /*
         * A wideband mode change was made. Figure out which was the last channel
         * changed and enforce that channel's codec setting on both channels.
         */
        pLineCtx = pDevCtx->pLineCtx[pDevObj->lastCodecChange-1];

        /*
         * Only way line context can be null here is if the codec mode was just
         * set and before calling the tick, the line context was set "free".
         * Highly unusual, but technically possible.
         */
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            codecMode = pLineObj->codec;

            if (codecMode == VP_OPTION_WIDEBAND) {
                pDevObj->ecVal |= VP890_WIDEBAND_MODE;
            } else {
                pDevObj->ecVal &= ~VP890_WIDEBAND_MODE;
            }

            /* Force both lines and device to correct wideband mode */
            for(channelId=0; channelId < maxChan; channelId++ ) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    continue;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineObj->codec = codecMode;

                if (codecMode == VP_OPTION_WIDEBAND) {
                    pLineObj->ecVal |= VP890_WIDEBAND_MODE;
                } else {
                    pLineObj->ecVal &= ~VP890_WIDEBAND_MODE;
                }
            }
        }
        pDevObj->lastCodecChange = 0;
    }

    ecVal = pDevObj->ecVal;

    /* Vp890Service Device Timers */
    Vp890ServiceDevTimers(pDevCtx);

    /*
     * Preclear the lineInTest flag that is used to determine if the TX Buffer
     * needs to be read. It is always read if any line is in test.
     */
    lineInTest = FALSE;

    /* Vp890Service Line Timers and check to see if the sequencer needs to run */
    for(channelId=0; channelId < maxChan; channelId++ ) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx == VP_NULL) {
            continue;
        }
        pLineObj = pLineCtx->pLineObj;

        /* call the appropriate service timer */
        if ((pLineObj->status & VP890_IS_FXO)) {
#ifdef VP890_FXO_SUPPORT
            Vp890ServiceFxoTimers(pDevCtx, pLineCtx, pLineObj, deviceId, pLineObj->ecVal);
#endif
        } else {
#ifdef VP890_FXS_SUPPORT
            Vp890ServiceFxsTimers(pDevCtx, pLineCtx, pLineObj, deviceId, pLineObj->ecVal);

#ifdef VP890_INCLUDE_TESTLINE_CODE
            if (Vp890IsChnlUndrTst(pDevObj, channelId) == TRUE) {
                lineInTest = TRUE;

                /* Clear flag to indicate the generators are NOT in a Ringing Mode */
                pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);
            } else if (pDevObj->currentTest.nonIntrusiveTest == TRUE) {
                lineInTest = TRUE;
            }
#endif
            if (pDevObj->stateInt & VP890_CAL_RELOAD_REQ) {
                Vp890UpdateCalValue(pLineCtx);
            }
#endif
        }
#ifdef VP_CSLAC_SEQ_EN
        /* Evaluate if Cadencing is required */
        if ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) == VP_CADENCE_STATUS_ACTIVE) {
            VP_SEQUENCER(VpLineCtxType, pLineCtx, ("Line %d is Active", channelId));
            isSeqRunning = TRUE;
        }
#endif
    }

#ifdef VP890_LP_SUPPORT
    if (!(pDevObj->stateInt & VP890_IS_FXO_ONLY)) {
        Vp890LowPowerMode(pDevCtx);
    }
#endif

    pDevObj->stateInt &= ~VP890_CAL_RELOAD_REQ;

    /* Reset event pointers pointers */
    pDevObj->dynamicInfo.lastChan = 0;

#ifdef VP_CSLAC_SEQ_EN
    if (isSeqRunning == TRUE) {
        VpServiceSeq(pDevCtx);
    }
#endif

    /*
     * Test the interrupt to see if there is a pending interrupt.  If there is,
     * read the interrupt registers (if running in an interrupt driven mode).
     * If running in polled mode, automatically read the interrupt/status
     * registers.
     */

#if defined (VP890_EFFICIENT_POLLED_MODE)
    /* Poll the device PIO-INT line */
    pDevObj->state |= (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);
#elif defined (VP890_SIMPLE_POLLED_MODE)
    pDevObj->state |= VP_DEV_PENDING_INT;
#endif

    intServiced = FALSE;

    /* Read this buffer once per tick IF there is a line under test. */
    if (lineInTest == TRUE) {
        /* Collect data from the device test data buffer */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_TEST_DATA_RD, VP890_TEST_DATA_LEN,
            pDevObj->testDataBuffer);
        pDevObj->state |= VP_DEV_TEST_BUFFER_READ;
    } else {
        pDevObj->state &= ~VP_DEV_TEST_BUFFER_READ;
    }

    /* Service all pending interrupts (up to 2) */
    while ((pDevObj->state & VP_DEV_PENDING_INT) && (numIntServiced > 0)) {

        VpMpiCmdWrapper(deviceId, ecVal, VP890_UL_SIGREG_RD,
            VP890_UL_SIGREG_LEN, pDevObj->intReg);

        if (numIntServiced == 2) {
            VpMemCpy(pDevObj->intReg2, pDevObj->intReg, VP890_UL_SIGREG_LEN);
        }

        /*
         * Compare it with what we already know.  If different, generate
         * events and update the line status bits
         */
        intServiced = TRUE;
        Vp890ServiceInterrupts(pDevCtx);

        /*
         * If level triggered, the interrupt may have been disabled (to prevent
         * a flood of interrupts), so reenable it.
         */
    #if defined (VP890_INTERRUPT_LEVTRIG_MODE)
        VpSysEnableInt(deviceId);
    #endif

        /* Clear the current interrupt indication */
        pDevObj->state &= ~(VP_DEV_PENDING_INT);
        numIntServiced--;

        /*
         * If operating in Efficient Polled Mode, check to see if the interrupt
         * line is still indicating an active interrupt. If in simple polled mode,
         * repeat the loop and service interrupts (if anything is changed).
         */
    #if defined (VP890_EFFICIENT_POLLED_MODE)
        /* Poll the PIO-INT line */
        pDevObj->state |=
            (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);
    #elif defined (VP890_SIMPLE_POLLED_MODE)
        pDevObj->state |= VP_DEV_PENDING_INT;
    #endif
    }/* End while Interrupts*/

    /* Make sure Vp890ServiceInterrupts() is called at least once per tick to
     * keep the API line status up to date */
    if (intServiced == FALSE) {
        Vp890ServiceInterrupts(pDevCtx);
    }

    /* Update the dial pulse handler for lines that are set for pulse decode,
     * and process events for FXO lines */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            if (pLineObj->status & VP890_INIT_COMPLETE) {
                if (!(pLineObj->status & VP890_IS_FXO)) {
#ifdef VP890_FXS_SUPPORT
                    Vp890ProcessFxsLine(pDevObj, pLineCtx);
#endif
                } else {
#ifdef VP890_FXO_SUPPORT
                    Vp890ProcessFxoLine(pDevObj, pLineCtx);
#endif
                }
            }
        }
    }
    /*******************************************************
     *      HANDLE Clock Fail and Battery Fault Events     *
     *******************************************************/
    /* Get the current status of the clock fault bit */
    if (!(pDevObj->devTimer[VP_DEV_TIMER_WB_MODE_CHANGE] & VP_ACTIVATE_TIMER)) {
        tempClkFault = (pDevObj->intReg[0] & VP890_CFAIL_MASK) ? TRUE : FALSE;
        /*
         * Compare it with what we already know.  If different, generate event
         */
        if(tempClkFault ^ pDevObj->dynamicInfo.clkFault) {
#ifdef VP890_FXS_SUPPORT
            if (!(pDevObj->stateInt & VP890_FORCE_FREE_RUN)) {
                if (tempClkFault) {
                    /* Entering clock fault, possibly a system restart. */
                    Vp890FreeRun(pDevCtx, VP_FREE_RUN_START);

                    /*
                     * Clear the flag used to indicate that Vp890FreeRun() was
                     * called by the application -- because it wasn't.
                     */
                    pDevObj->stateInt &= ~VP890_FORCE_FREE_RUN;
                } else {
                    /*
                     * Exiting clock fault (note: this function does not affect
                     * VP890_FORCE_FREE_RUN flag).
                     */
                    Vp890RestartComplete(pDevCtx);
                }

            }
#endif
            pDevObj->dynamicInfo.clkFault = tempClkFault;
            pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
            pDevObj->eventHandle = pDevObj->timeStamp;
        }
    }

    /* Get the current status of the battery fault bit */
    tempBat1Fault = (pDevObj->intReg[0] & VP890_OCALMY_MASK) ? TRUE : FALSE;
    /*
     * Compare it with what we already know.  If different, generate event
     */
    if(tempBat1Fault ^ pDevObj->dynamicInfo.bat1Fault) {
        pDevObj->dynamicInfo.bat1Fault = tempBat1Fault;
        pDevObj->deviceEvents.faults |= VP_DEV_EVID_BAT_FLT;
        pDevObj->eventHandle = pDevObj->timeStamp;
    }

    /* Collect all event activity and report to the calling function */
    if (Vp890FindSoftwareInterrupts(pDevCtx)) {
        *pEventStatus = TRUE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890ApiTick() */

/*******************************************************************************
 * Vp890IsChnlUndrTst()
 *  This function determines if a particular line of a device is currently
 * running a test.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Device not affected. Return value TRUE if the line is currently running a
 * test, FALSE otherwise.
 ******************************************************************************/
bool
Vp890IsChnlUndrTst(
    Vp890DeviceObjectType *pDevObj,
    uint8 channelId)
{
#ifdef VP890_INCLUDE_TESTLINE_CODE
    if (pDevObj->currentTest.nonIntrusiveTest == FALSE) {
        if ((TRUE == pDevObj->currentTest.prepared) &&
            (channelId == pDevObj->currentTest.channelId)) {
            return TRUE;
        }
    }
#endif
    return FALSE;
}

#ifdef VP890_FXS_SUPPORT
/**
 * Vp890FreeRun()
 *  This function is called by the application when it wants to prepare the
 * system for a restart, or by the VP-API-II internally when a clock fault or
 * other "sign" of a restart is detected.
 *
 * Preconditions:
 *  Conditions defined by purpose of Api Tick.
 *
 * Postconditions:
 *  Device and line are in states 'ready" for a system reboot to occur. Lines
 * are set to VP_LINE_STANDBY if previously ringing.
 */
VpStatusType
Vp890FreeRun(
    VpDevCtxType *pDevCtx,
    VpFreeRunModeType freeRunMode)
{
    VpLineCtxType *pLineCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp890LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal = pDevObj->ecVal;
    VpLineStateType lineState;
    uint8 powerMode[VP890_REGULATOR_CTRL_LEN];

    VP_API_FUNC(VpDevCtxType, pDevCtx, ("Vp890FreeRun Mode %d", freeRunMode));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * Time value is passed in 500us increment. If timeOut = 0, only PCLK
     * recovery exits restart prepare operations. If less than one tick, force
     * a one tick timeout.
     */
    if (freeRunMode == VP_FREE_RUN_STOP) {
        Vp890RestartComplete(pDevCtx);
        /*
         * Clear the device as being forced into free run mode by application.
         * This allows PCLK fault detection to automatically enter/exit free
         * run mode.
         */
        pDevObj->stateInt &= ~VP890_FORCE_FREE_RUN;
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* Take the lines out of Ringing if necessary */
    pLineCtx = pDevCtx->pLineCtx[0];
    if (pLineCtx != VP_NULL) {
        pLineObj = pLineCtx->pLineObj;
        ecVal = pLineObj->ecVal;
        lineState = pLineObj->lineState.usrCurrent;

        if (lineState == VP_LINE_RINGING) {
            Vp890SetLineState(pLineCtx, VP_LINE_STANDBY);
        }
        if (lineState == VP_LINE_RINGING_POLREV) {
            Vp890SetLineState(pLineCtx, VP_LINE_STANDBY_POLREV);
        }
    }

    /* Load the free run timing, if available. Othewise just switch to HP mode */
    if (pDevObj->devProfileData.timingParamsFR[0] != 0x00) {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_TIMING_WRT,
            VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParamsFR);
    } else {
        VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
            VP890_REGULATOR_PARAM_LEN);

        pDevObj->swParamsCache[VP890_SWY_AUTOPOWER_INDEX] |= VP890_SWY_AUTOPOWER_DIS;

        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
            VP890_REGULATOR_PARAM_LEN, pDevObj->swParamsCache);

        /* Change the Switchers to High Power Mode */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_RD,
            VP890_REGULATOR_CTRL_LEN, powerMode);
        powerMode[0] &= ~VP890_SWY_MODE_MASK;
        powerMode[0] |= VP890_SWY_HP;
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_WRT,
            VP890_REGULATOR_CTRL_LEN, powerMode);
    }

    /*
     * Mark the device as being forced into free run mode by application. This
     * prevents auto-recovery when PCLK is restored.
     */
    pDevObj->stateInt |= VP890_FORCE_FREE_RUN;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp890RestartComplete()
 *  This function is called by the VP-API-II internally when a clock fault is
 * removed.
 *
 * Preconditions:
 *  Conditions defined by purpose of Api Tick.
 *
 * Postconditions:
 *  Device and line are in states recovered from a reboot.
 */
void
Vp890RestartComplete(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp890LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pDevObj->ecVal;
    uint8 powerMode[VP890_REGULATOR_CTRL_LEN];

    /* Make sure to use the correct EC value. */
    pLineCtx = pDevCtx->pLineCtx[0];
    if (pLineCtx != VP_NULL) {
        pLineObj = pLineCtx->pLineObj;
        ecVal = pLineObj->ecVal;
    }

    /* Restore normal timing, if available. Othewise just switch to LP mode */
    if (pDevObj->devProfileData.timingParamsFR[0] != 0x00) {
        /* Restore the original timings */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_TIMING_WRT,
            VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParams);
    } else {
        /* Change the Switchers to Low Power Mode */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_RD,
            VP890_REGULATOR_CTRL_LEN, powerMode);
        powerMode[0] &= ~VP890_SWY_MODE_MASK;
        powerMode[0] |= VP890_SWY_LP;
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_WRT,
            VP890_REGULATOR_CTRL_LEN, powerMode);

        /* Relinquish switcher control. */
        pDevObj->swParamsCache[VP890_SWY_AUTOPOWER_INDEX] &= ~VP890_SWY_AUTOPOWER_DIS;
        VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
            VP890_REGULATOR_PARAM_LEN, pDevObj->swParamsCache);
    }
}
#endif

/*******************************************************************************
 * Vp890ServiceInterrupts()
 * This function goes through every line associated with the given device and
 * calls Vp890ServiceFxsInterrupts.
 *
 * Arguments: *pDevCtx - Device context ptr
 *
 * Preconditions: intReg in the device object should contain global signaling
 *                  data as read by Vp890ApiTick
 *
 * Postconditions:
 ******************************************************************************/
static void
Vp890ServiceInterrupts(
    VpDevCtxType    *pDevCtx)
{
    VpLineCtxType           *pLineCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj;
    uint8                   maxChan     = pDevObj->staticInfo.maxChannels;
    uint8                   channelId;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP890_IS_FXO)) {
#ifdef VP890_FXS_SUPPORT
                Vp890ServiceFxsInterrupts(pLineCtx);
#endif
            }
        }
    }
} /* Vp890ServiceInterrupts() */

/*******************************************************************************
 * Vp890ServiceDevTimers()
 * This function services device-level timers.
 *
 * Arguments:  *pDevCtx - Device context ptr
 *
 * Preconditions:  Sould be called once per tick by Vp890ApiTick.
 *
 * Postconditions: Device timers will be serviced.
 ******************************************************************************/
static void
Vp890ServiceDevTimers(
    VpDevCtxType            *pDevCtx)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDevTimerType          devTimer;

    for (devTimer = 0; devTimer < VP_DEV_TIMER_LAST; devTimer++) {

        /* if timer is not active go to next timer */
        if (!(pDevObj->devTimer[devTimer] & VP_ACTIVATE_TIMER)) {
            continue;
        }

        /* get the bits associated only with the time of the timer */
        pDevObj->devTimer[devTimer] &= VP_TIMER_TIME_MASK;

        /* decrement the timer */
        if (pDevObj->devTimer[devTimer] > 0) {
            (pDevObj->devTimer[devTimer])--;
        }

        /* if time left on the timer, active it and move on to the next one */
        if (pDevObj->devTimer[devTimer] != 0) {
            pDevObj->devTimer[devTimer] |= VP_ACTIVATE_TIMER;
            continue;
        }

        if (VP_DEV_TIMER_TESTLINE == devTimer) {
#ifdef VP890_INCLUDE_TESTLINE_CODE
            const void *pTestArgs =
                (const void*)&pDevObj->currentTest.pTestHeap->testArgs;
            uint8 testChanId = pDevObj->currentTest.channelId;
            VpTestLineIntFuncPtrType testline =
                pDevCtx->funPtrsToApiFuncs.TestLineInt;
            if ((testline != VP_NULL)
              && (pDevObj->currentTest.testState != -1)) {
                /*
                 * if the TestLineInt function exists and the
                 * current test state has not been set back to
                 * -1 by test conclude before the timer expired
                 * then run the call back
                 */

                testline(
                    pDevCtx->pLineCtx[testChanId],
                    pDevObj->currentTest.testId,
                    pTestArgs,
                    pDevObj->currentTest.handle,
                    TRUE);
            }
#endif
        } else if (VP_DEV_TIMER_LP_CHANGE == devTimer) {
#ifdef VP890_LP_SUPPORT
            uint8 channelId;
            Vp890LineObjectType *pLineObj;
            VpLineCtxType *pLineCtx;
            bool failureMode = FALSE;

            /* Skip this process if we're in calibration */
            if (!(pDevObj->state & VP_DEV_IN_CAL)) {
                VP_HOOK(VpDevCtxType, pDevCtx, ("Signaling 0x%02X 0x%02X",
                    pDevObj->intReg[0], pDevObj->intReg[1]));
                for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
                    pLineCtx = pDevCtx->pLineCtx[channelId];
                    if (pLineCtx != VP_NULL) {
                        bool lpTermType = FALSE;
                        pLineObj = pLineCtx->pLineObj;

                        if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR) ||
                            (pLineObj->termType == VP_TERM_FXS_SPLITTER_LP) ||
                            (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP)) {
                            lpTermType = TRUE;
                        }

                        if (lpTermType == TRUE) {
                            VP_HOOK(VpLineCtxType, pLineCtx, ("Last Hook State on line %d = %d LP Mode %d CurrentState %d Time %d",
                                channelId, (pLineObj->lineState.condition & VP_CSLAC_HOOK),
                                (pLineObj->status & VP890_LOW_POWER_EN), pLineObj->lineState.currentState, pDevObj->timeStamp));
                            if (pLineObj->lineState.currentState != VP_LINE_DISCONNECT) {
                                if (pLineObj->status & VP890_LOW_POWER_EN) {
                                    /*
                                     * If we're in LP Mode, then the line should be
                                     * detecting on-hook. All other conditions mean
                                     * there could be a leaky line.
                                     */
                                    if (((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                      && ((pDevObj->intReg[channelId]) & VP890_HOOK_MASK) != VP890_HOOK_MASK)) {
                                        failureMode = TRUE;
                                    }
                                    VP_HOOK(VpLineCtxType, pLineCtx, ("1. Failure Mode = %d -- Previous %d Current %d",
                                        failureMode,
                                        (pLineObj->lineState.condition & VP_CSLAC_HOOK),
                                        (pDevObj->intReg[channelId]) & VP890_HOOK_MASK));
                                } else {
                                    /*
                                     * If we're not in LP Mode, then the line should be
                                     * detecting off-hook and the signaling bit should
                                     * be high. Otherwise, error.
                                     */
                                    if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                      && (((pDevObj->intReg[channelId]) & VP890_HOOK_MASK) != VP890_HOOK_MASK)) {
                                        failureMode = TRUE;
                                    }
                                    VP_HOOK(VpLineCtxType, pLineCtx, ("2. Failure Mode = %d -- Previous %d Current %d",
                                        failureMode,
                                        (pLineObj->lineState.condition & VP_CSLAC_HOOK),
                                        (pDevObj->intReg[channelId]) & VP890_HOOK_MASK));
                                }
                            }
                        }

                        /*
                         * If the line was last seen off-hook and is now on-hook as a result
                         * of exiting LP Mode, it could be a leaky line.
                         */
                        if (failureMode == TRUE) {
                            if (pLineObj->leakyLineCnt >= 3) {
                                VP_HOOK(VpLineCtxType, pLineCtx, ("Flag Channel %d for Leaky Line at time %d Signaling 0x%02X LineState %d",
                                    channelId, pDevObj->timeStamp, (pDevObj->intReg[channelId] & VP890_HOOK_MASK),
                                    pLineObj->lineState.usrCurrent));

                                pLineObj->status |= VP890_LINE_LEAK;
                                pDevObj->stateInt &= ~(VP890_LINE0_LP | VP890_LINE1_LP);
                                pLineObj->lineEvents.faults |= VP_LINE_EVID_RES_LEAK_FLT;

                                /* Leaky Line Test is complete */
                                pLineObj->lineState.condition &= ~VP_CSLAC_LINE_LEAK_TEST;
                            } else {
                                VP_HOOK(VpLineCtxType, pLineCtx, ("Potential Leaky Line %d at time %d  ...retry",
                                    channelId, pDevObj->timeStamp));

                                /* Continue Leaky Line Test */
                                pLineObj->leakyLineCnt++;

                                /*
                                 * Make sure timer is restarted. This may
                                 * occur as a result of SetLineState(),
                                 * but it may not.
                                 */
                                if (pDevObj->swParamsCache[VP890_REGULATOR_TRACK_INDEX] & VP890_REGULATOR_FIXED_RING) {
                                    /*
                                     * Longest is using "fixed" ringing mode because the external
                                     * capacitors are generally very large.
                                     */
                                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] =
                                        (MS_TO_TICKRATE(VP890_PWR_SWITCH_DEBOUNCE_FXT, pDevObj->devProfileData.tickRate))
                                        | VP_ACTIVATE_TIMER;
                                } else {
                                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] =
                                        (MS_TO_TICKRATE(VP890_PWR_SWITCH_DEBOUNCE_FUT, pDevObj->devProfileData.tickRate))
                                        | VP_ACTIVATE_TIMER;
                                }
                            }

                            /* Update the line state */
                            for (channelId = 0;
                                 channelId < pDevObj->staticInfo.maxChannels;
                                 channelId++) {
                                Vp890LineObjectType *pLineObjInt;

                                pLineCtx = pDevCtx->pLineCtx[channelId];
                                if (pLineCtx != VP_NULL) {
                                    pLineObjInt = pLineCtx->pLineObj;
                                    if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR) ||
                                        (pLineObj->termType == VP_TERM_FXS_SPLITTER_LP) ||
                                        (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP)) {
                                        VP_HOOK(VpLineCtxType, pLineCtx, ("1. Channel %d Current Linestate %d Current User Linestate %d",
                                            channelId, pLineObjInt->lineState.currentState, pLineObjInt->lineState.usrCurrent));

                                        Vp890SetFxsLineState(pLineCtx, pLineObjInt->lineState.usrCurrent);
                                    }
                                }
                            }
                        } else if (lpTermType == TRUE) {
                            /*
                             * No failure. Recover all hook status, line states
                             * and clear Leaky Line Test Flag
                             */

                            /* Leaky Line Test is complete */
                            pLineObj->lineState.condition &= ~VP_CSLAC_LINE_LEAK_TEST;

                            /*
                             * Low Power Mode exit simply sets the line
                             * to Active in order to maintain smooth
                             * line transients. This step is done to
                             * put the line into the user (API-II)
                             * defined state.
                             */
                            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LPM Timer: Current %d, User Current %d Channel %d",
                                pLineObj->lineState.currentState,
                                pLineObj->lineState.usrCurrent,
                                channelId));

                            if ((pLineObj->lineState.usrCurrent == VP_LINE_STANDBY)
                              && (!(pLineObj->status & VP890_LOW_POWER_EN))
                              && (pLineObj->calLineData.calDone == TRUE)) {     /* Must not occur during the calibration */
                                uint8 lineState[1] = {VP890_SS_IDLE};

                                VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Setting Channel %d to 0x%02X State at time %d",
                                    channelId, lineState[0], pDevObj->timeStamp));
#ifdef VP_CSLAC_SEQ_EN
                                if ((pLineObj->cadence.status & (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_SENDSIG)) !=
                                    (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_SENDSIG)) {
#endif
                                    VpMpiCmdWrapper(pDevObj->deviceId, pLineObj->ecVal, VP890_SYS_STATE_WRT,
                                        VP890_SYS_STATE_LEN, lineState);
                                    pLineObj->slicValueCache = lineState[0];
#ifdef VP_CSLAC_SEQ_EN
                                }
#endif
                            }

                            if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                && (pDevObj->intReg[channelId]) & VP890_HOOK_MASK) {

                                if ((pLineObj->lineState.condition & VP_CSLAC_HOOK) &&
                                    (pLineObj->status & VP890_LOW_POWER_EN)) {
                                    /* The line is on-hook */
                                    pLineObj->lineState.condition &= ~VP_CSLAC_HOOK;
                                } else {
                                    /* Valid off-hook */
                                    VP_HOOK(VpLineCtxType, pLineCtx, ("Valid Off-Hook on line %d at time %d",
                                        channelId, pDevObj->timeStamp));

                                    pLineObj->leakyLineCnt = 0;
                                    pLineObj->status &= ~VP890_LINE_LEAK;

                                    pLineObj->dpStruct.hookSt = TRUE;
                                    pLineObj->dpStruct2.hookSt = TRUE;

                                    if(pLineObj->pulseMode != VP_OPTION_PULSE_DECODE_OFF) {
                                        pLineObj->dpStruct.state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                                        pLineObj->dpStruct.lc_time = 0;

                                        pLineObj->dpStruct2.state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                                        pLineObj->dpStruct2.lc_time = 0;
                                    }

                                    pLineObj->lineEvents.signaling |=
                                        VP_LINE_EVID_HOOK_OFF;

                                    pLineObj->lineState.condition |= VP_CSLAC_HOOK;
                                    pLineObj->lineEventHandle = pDevObj->timeStamp;
                                }
                            }
                        }
                    }
                }
            }
#endif
        } else if (VP_DEV_TIMER_WB_MODE_CHANGE == devTimer) {
        }
    } /* Loop through all device timers */

    return;
}

/*******************************************************************************
 * Vp890FindSoftwareInterrupts()
 *  This function checks for active non-masked device and line events.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Returns true if there is an active, non-masked event on either the device
 * or on a line associated with the device.
 ******************************************************************************/
static bool
Vp890FindSoftwareInterrupts(
    VpDevCtxType            *pDevCtx)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj;
    VpLineCtxType           *pLineCtx;
    uint8                   channelId;
    uint8                   maxChan     = pDevObj->staticInfo.maxChannels;

    VpOptionEventMaskType   eventsMask  = pDevObj->deviceEventsMask;
    VpOptionEventMaskType   *pEvents    = &(pDevObj->deviceEvents);

    /* First clear all device events that are masked */
    pEvents->faults     &= ~(eventsMask.faults);
    pEvents->signaling  &= ~(eventsMask.signaling);
    pEvents->response   &= ~(eventsMask.response);
    pEvents->process    &= ~(eventsMask.process);
    pEvents->test       &= ~(eventsMask.test);
    pEvents->fxo        &= ~(eventsMask.fxo);

    /* Evaluate if any device events remain */
    if (pEvents->faults     ||
        pEvents->signaling  ||
        pEvents->response   ||
        pEvents->process    ||
        pEvents->test       ||
        pEvents->fxo) {
            return TRUE;
    }

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            eventsMask = pLineObj->lineEventsMask;
            pEvents = &(pLineObj->lineEvents);

            /* Clear the line events that are masked */
            pEvents->faults     &= ~(eventsMask.faults);
            pEvents->signaling  &= ~(eventsMask.signaling);
            pEvents->response   &= ~(eventsMask.response);
            pEvents->process    &= ~(eventsMask.process);
            pEvents->test       &= ~(eventsMask.test);
            pEvents->fxo        &= ~(eventsMask.fxo);

#ifdef VP890_FXO_SUPPORT
            /* Clear all FXO events during PLL Recovery */
            if (pLineObj->lineTimers.timers.fxoTimer.pllRecovery) {
                pEvents->fxo = 0;
            }
#endif
            /* Evaluate if any line events remain */
            if (pEvents->faults     ||
                pEvents->signaling  ||
                pEvents->response   ||
                pEvents->process    ||
                pEvents->test       ||
                pEvents->fxo) {
                    return TRUE;
            }
        }
    }

    return FALSE;
} /* Vp890FindSoftwareInterrupts() */

/*******************************************************************************
 * Vp890VirtualISR()
 *  This function is called everytime the device causes an interrupt
 *
 * Preconditions
 *  A device interrupt has just occured
 *
 * Postcondition
 *  This function should be called from the each device's ISR.
 *  This function could be inlined to improve ISR performance.
 ******************************************************************************/
VpStatusType
Vp890VirtualISR(
    VpDevCtxType *pDevCtx)
{
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

#if defined (VP890_INTERRUPT_LEVTRIG_MODE)
    VpSysDisableInt(deviceId);
#endif
    /* Device Interrupt Received */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevObj->state |= VP_DEV_PENDING_INT;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* Vp890VirtualISR() */

/*******************************************************************************

* Vp890SetLineState()
 *  This function is the API-II wrapper function for Vp890SetFxsLineState and
 * Vp890SetFxoLineState.
 *
 * Preconditions:
 *  Same as Vp890SetFxsLineState() / Vp890SetFxoLineState()
 *
 * Postconditions:
 *  Same as Vp890SetFxsLineState() / Vp890SetFxoLineState()
 ******************************************************************************/
VpStatusType
Vp890SetLineState(
    VpLineCtxType           *pLineCtx,
    VpLineStateType         state)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpStatusType            status      = VP_STATUS_SUCCESS;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    VP_API_FUNC(VpLineCtxType, pLineCtx, ("+Vp890SetLineState() State %d", state));

    /* Proceed if device state is either in progress or complete */
    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        VP_API_FUNC(VpLineCtxType, pLineCtx, ("-Vp890SetLineState() - Device Not Ready"));
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Setting Channel %d to State %d",
        pLineObj->channelId, state));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (Vp890IsChnlUndrTst(pDevObj, pLineObj->channelId) == FALSE) {
        if(state == pLineObj->lineState.usrCurrent) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            VP_API_FUNC(VpLineCtxType, pLineCtx, ("-Vp890SetLineState() - Same State"));
            return VP_STATUS_SUCCESS;
        }
    }

    if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
        if (Vp890IsSupportedFxoState(state) == FALSE) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            VP_API_FUNC(VpLineCtxType, pLineCtx, ("-Vp890SetLineState() - Unsupported FXO State"));
            return VP_STATUS_INVALID_ARG;
        }
#endif
    } else {
#ifdef VP890_FXS_SUPPORT
        if (Vp890IsSupportedFxsState(state) == FALSE) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            VP_API_FUNC(VpLineCtxType, pLineCtx, ("-Vp890SetLineState() - Unsupported FXS State"));
            return VP_STATUS_INVALID_ARG;
        }
#endif
    }

    /* Clear the "called from API" flag. This affects the cadencer */
    pLineObj->status &= ~(VP890_SLS_CALL_FROM_API);

    if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
#ifdef VP890_FXO_DELAYED_RING_TRIP
        /*
         * If line is FXO and is currently detecting ringing, set a flag to change
         * state after ringing is over.  Also make sure the voicepath is cut off
         * so that the ringing signal won't be sent out as voice
         */
        if (pLineObj->lineState.condition & VP_CSLAC_RINGING) {
            pLineObj->fxoRingStateFlag = 1;
            pLineObj->fxoRingState = state;
            status = VP_STATUS_SUCCESS;
        } else {
            status = Vp890SetFxoLineState(pLineCtx, state);
        }
#else
            status = Vp890SetFxoLineState(pLineCtx, state);
#endif
#endif
    } else {
#ifdef VP890_FXS_SUPPORT
        /*
         * Special FXS handling to prevent setting the line to ringing if
         * off-hook
         */
        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
         && ((state == VP_LINE_RINGING_POLREV) || (state == VP_LINE_RINGING))) {
            state = pLineObj->ringCtrl.ringTripExitSt;
        }

        status = Vp890SetFxsLineState(pLineCtx, state);
#endif
    }

    if (status == VP_STATUS_SUCCESS) {
        /*
         * Reset the "Count" for leaky line conditions because there are some
         * normal state change conditions that will increment the count, therefore
         * causing exit of LP for non-leaky line
         */
        pLineObj->leakyLineCnt = 0;
        pLineObj->lineState.usrCurrent = state;
    }

    /*
     * Set the "called from API" flag. Convenience for API functions so setting
     * this flag does not need to occur in multiple locations
     */
    pLineObj->status |= VP890_SLS_CALL_FROM_API;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    VP_API_FUNC(VpLineCtxType, pLineCtx, ("-Vp890SetLineState()"));

    return status;
} /* Vp890SetLineState() */

/*******************************************************************************
 * Vp890LLSetSysState()
 *  This function writes to the System State register and based on the state
 * being set, determines if low power mode can or cannot be used, or if the line
 * is recovering from Disconnect to a Feed state. In the latter case, a timer
 * is set to transition through Tip Open first to prevent "ping" of the phone.
 * In this case, the final state is set when the timer expires.
 *
 * Preconditions:
 *  This function is called internally by the API-II only.
 *
 * Postconditions:
 *  The System State Register is updated with either the value passed, or
 * Tip Open. If low power mode termination type exists, a flag in the device
 * object indicating that low power can or cannot be used is modified. If coming
 * out of Disconnect, Tip Open is written and a timer is set.
 ******************************************************************************/
void
Vp890LLSetSysState(
    VpDeviceIdType deviceId,
    VpLineCtxType *pLineCtx,
    uint8 lineState,
    bool writeToDevice)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal = pLineObj->ecVal;
    uint8 lineStatePre[VP890_SYS_STATE_LEN];
#ifdef VP890_FXS_SUPPORT
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    bool lineIsFxs = FALSE;
    uint8 channelId = pLineObj->channelId;
#endif

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890LLSetSysState() SlicState 0x%02X Write %d",
        lineState, writeToDevice));

    lineStatePre[0] = pLineObj->slicValueCache;

#ifdef VP890_FXS_SUPPORT
    if (!(pLineObj->status & VP890_IS_FXO)) {
        lineIsFxs = TRUE;
        if ((Vp890IsChnlUndrTst(pDevObj, channelId) == TRUE) ||
            (pLineObj->status & VP890_LINE_IN_CAL)) {
            pDevObj->stateInt &= ~(VP890_LINE0_LP | VP890_LINE1_LP);
            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("3. Clearing LP flag for channel %d CurrentState %d UserState %d",
                channelId, pLineObj->lineState.currentState,
                pLineObj->lineState.usrCurrent));
        } else {
            bool lpTermType = FALSE;
            bool hookStatus = FALSE;

            if (((pLineObj->termType == VP_TERM_FXS_LOW_PWR) ||
                 (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP) ||
                 (pLineObj->termType == VP_TERM_FXS_SPLITTER_LP))) {
                lpTermType = TRUE;
                VpCSLACGetLineStatus(pLineCtx, VP_INPUT_RAW_HOOK, &hookStatus);
            }
            hookStatus = (pLineObj->lineState.currentState == VP_LINE_STANDBY) ? hookStatus : FALSE;

            if ((lpTermType == TRUE) && (hookStatus == FALSE) && (!(pLineObj->status & VP890_LINE_LEAK))) {
                if ((pLineObj->lineState.currentState == VP_LINE_DISCONNECT) ||
                    (pLineObj->lineState.currentState == VP_LINE_STANDBY)) {

                    if (((lineStatePre[0] & VP890_SS_LINE_FEED_MASK) != VP890_SS_FEED_BALANCED_RINGING)
                     && ((lineStatePre[0] & VP890_SS_LINE_FEED_MASK) != VP890_SS_FEED_UNBALANCED_RINGING)) {
                        pDevObj->stateInt |= (VP890_LINE0_LP | VP890_LINE1_LP);
                        VP_LINE_STATE(VpLineCtxType, pLineCtx,("1. 890-Setting LP flag for channel %d",channelId));
                    } else {
                        VP_LINE_STATE(VpLineCtxType, pLineCtx,("1. Delay Setting LP flag for channel %d",channelId));
                    }
                } else {
                    pDevObj->stateInt &= ~(VP890_LINE0_LP | VP890_LINE1_LP);
                    VP_LINE_STATE(VpLineCtxType, pLineCtx,("1. Clearing LP flag for channel %d Device Status 0x%04X",
                        channelId, pDevObj->stateInt));
                }
            } else {
                pDevObj->stateInt &= ~(VP890_LINE0_LP | VP890_LINE1_LP);
                VP_LINE_STATE(VpLineCtxType, pLineCtx, ("2. Clearing LP flag for channel %d Status 0x%04X",
                    channelId, pLineObj->status));
            }
        }
    }
#endif

    /* Device Write Override: setting flags without a device write */
    if (writeToDevice == FALSE) {
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890LLSetSysState()"));
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("-Vp890LLSetSysState()"));
        return;
    }

#ifdef VP890_FXS_SUPPORT
    if (((lineStatePre[0] & VP890_SS_LINE_FEED_MASK) == VP890_SS_DISCONNECT)
      && (lineIsFxs == TRUE)) {
        pLineObj->nextSlicValue = lineState;

        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("2. Setting Chan %d to System State 0x%02X at Time %d",
            channelId, lineState, pDevObj->timeStamp));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
            &lineState);
        pLineObj->slicValueCache = lineState;
    } else {
#endif
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("1. Setting Chan %d to System State 0x%02X at Time %d",
            channelId, lineState, pDevObj->timeStamp));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
            &lineState);
        pLineObj->slicValueCache = lineState;
#ifdef VP890_FXS_SUPPORT
    }
#endif

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890LLSetSysState()"));
}

/*******************************************************************************
 * LineStateMap
 *  Locally used function by SetFxsLineState and SetFxoLineState to map an API
 * line state to a register-level line state
 *
 * Preconditions:
 *  None. State to byte mapping only.
 *
 * Postconditions:
 *  Returns the byte that should be used in the device System State register
 * for the API State passed.
 ******************************************************************************/
uint8
LineStateMap(
    VpLineStateType state)
{
    VP_API_FUNC_INT(None, VP_NULL, ("+LineStateMap()"));

    switch(state) {
        case VP_LINE_STANDBY:
            return VP890_SS_IDLE;
        case VP_LINE_TIP_OPEN:
            return VP890_SS_TIP_OPEN;

        case VP_LINE_ACTIVE:
        case VP_LINE_TALK:
        case VP_LINE_OHT:
            return VP890_SS_ACTIVE;

        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT_POLREV:
            return VP890_SS_ACTIVE_POLREV;

        case VP_LINE_STANDBY_POLREV:
            return VP890_SS_IDLE_POLREV;

        case VP_LINE_DISCONNECT:
            return VP890_SS_DISCONNECT;
        case VP_LINE_RINGING:
            return VP890_SS_BALANCED_RINGING;

        case VP_LINE_RINGING_POLREV:
            return VP890_SS_BALANCED_RINGING_PR;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_LOOP_OPEN:
            return VP890_SS_FXO_ONHOOK;
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_TALK:
            return VP890_SS_FXO_OFFHOOK;

        default:
            return VP890_SS_DISCONNECT;
    }
} /* LineStateMap() */

/*******************************************************************************
 * Vp890SetLineTone()
 *  This function sets the line tone with the cadence specified on the line.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The tone specified by the tone profile is sent on the line at the cadence
 * specified by the cadence profile.  If the tone is NULL, all line tones are
 * removed.  If the cadence is NULL, the cadence is set to "Always On".  This
 * function returns the success code if the tone cadence is a valid tone cadence
 * and the tone profile is a valid tone profile, or in the case where the user
 * passes in profile indexes, if the tone/cadence indexes are within the range
 * of the device.
 ******************************************************************************/
VpStatusType
Vp890SetLineTone(
    VpLineCtxType       *pLineCtx,
    VpProfilePtrType    pToneProfile,  /**< A pointer to a tone profile, or an
                                        * index into the profile table for the tone
                                        * to put on the line.
                                        */
    VpProfilePtrType    pCadProfile,   /**< A pointer to a tone cadence profile, or
                                        * an index into the profile table for the
                                        * tone cadence to put on the line.
                                        */
    VpDtmfToneGenType   *pDtmfControl) /**< Indicates to send a DTMF tone
                                        * (either upstream or downstream) if
                                        * this parameter is not VP_NULL AND
                                        * the tone specified is VP_PTABLE_NULL
                                        */
{
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpProfilePtrType      pToneProf = VP_PTABLE_NULL;

#ifdef VP_CSLAC_SEQ_EN
    VpProfilePtrType      pCadProf  = VP_PTABLE_NULL;
    int cadenceIndex = VpGetProfileIndex(pCadProfile);
#endif

    VpDigitType           digit     = VP_DIG_NONE;
    VpDirectionType       direction = VP_DIRECTION_INVALID;

    uint8                 ecVal     = pLineObj->ecVal;

    uint8 mpiByte, mpiIndex;
    uint8 converterCfg[VP890_CONV_CFG_LEN];
    uint8 mpiBuffer[2 + VP890_SIGAB_PARAMS_LEN + VP890_SIGCD_PARAMS_LEN];

    /* Initialize SigGen A/B values to 0 */
    uint8 sigGenAB[VP890_SIGAB_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8 sigGenCtrl[] = {0};

    uint8 opCondTarget = pLineObj->opCond[0];

    int toneIndex = VpGetProfileIndex(pToneProfile);
    uint16 profIndex;   /* Used for bit masking the profile index table */

    VpDeviceIdType deviceId = pDevObj->deviceId;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890SetLineTone()"));

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */

    /* Verify a good profile (index or pointer) for the tone */
    if (toneIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pToneProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONE, pToneProfile) != TRUE) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid tone profile"));
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pToneProf = pToneProfile;
    } else if (toneIndex < VP_CSLAC_TONE_PROF_TABLE_SIZE) {
        profIndex = (uint16)toneIndex;
        pToneProf = pDevObj->devProfileTable.pToneProfileTable[profIndex];
        if (!(pDevObj->profEntry.toneProfEntry & (0x0001 << profIndex))) {
            /* The profile is invalid -- error. */
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid tone profile"));
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid tone profile"));
        return VP_STATUS_ERR_PROFILE;
    }

    /* Verify a good profile (index or pointer) for the cadence */
#ifdef VP_CSLAC_SEQ_EN
    if (cadenceIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pCadProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONECAD, pCadProfile) != TRUE) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid cadence profile"));
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pCadProf = pCadProfile;
    } else if (cadenceIndex < VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
        pCadProf = pDevObj->devProfileTable.pToneCadProfileTable[cadenceIndex];
        if (!(pDevObj->profEntry.toneCadProfEntry & (0x0001 << cadenceIndex))) {
            /* The profile is invalid -- error. */
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid cadence profile"));
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetLineTone() - Invalid cadence profile"));
        return VP_STATUS_ERR_PROFILE;
    }
#endif

    if (pDtmfControl != VP_NULL) {
        digit = pDtmfControl->toneId;
        if (VpIsDigit(digit) == FALSE) {
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890SetLineTone-"));
            return VP_STATUS_INVALID_ARG;
        }

        direction = pDtmfControl->dir;
        if (direction != VP_DIRECTION_DS) {
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890SetLineTone-"));
            return VP_STATUS_INVALID_ARG;
        }
    }

    /* All input parameters are valid. */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pLineObj->status & VP890_BAD_LOOP_SUP) {
        pLineObj->status &= ~(VP890_BAD_LOOP_SUP);
        VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT,
            VP890_LOOP_SUP_LEN, pLineObj->loopSup);
    }

    /*
     * Disable signal generator A/B/C/D before making any changes and stop
     * previous cadences
     */
    if (pLineObj->sigGenCtrl[0] != sigGenCtrl[0]) {
        pLineObj->sigGenCtrl[0] = sigGenCtrl[0];
        VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT, VP890_GEN_CTRL_LEN,
            pLineObj->sigGenCtrl);
    }

#ifdef VP_CSLAC_SEQ_EN
    if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
        pLineObj->cadence.pActiveCadence = pCadProf;
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;

        /* We're no longer in the middle of a time function */
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
        pLineObj->cadence.timeRemain = 0;
    }
#endif

    /*
     * If tone profile is NULL, and either the pDtmfControl is NULL or it's
     * "digit" member is "Digit None", then shutoff the tone generators, stop
     * any active cadencing and restore the filter coefficients if they need
     * to be. Also, re-enable the audio path if it was disabled by a previous
     * DTMF generation command
     */
    if (((pToneProf == VP_PTABLE_NULL) && (pDtmfControl == VP_NULL))
     || ((pToneProf == VP_PTABLE_NULL) && (digit == VP_DIG_NONE))) {

        if (!(pLineObj->status & VP890_IS_FXO)) {
            /* Restore the normal audio path */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD,
                VP890_CONV_CFG_LEN, converterCfg);

            converterCfg[0] &= ~VP890_CONV_CONNECT_BITS;
            converterCfg[0] |= VP890_METALLIC_AC_V;

            VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT,
                VP890_CONV_CFG_LEN, converterCfg);
        }

#ifdef VP_CSLAC_SEQ_EN
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
#endif
            /*
             * Pre-Or the bits and get the correct values based on the current
             * line state, then update the device
             */
            opCondTarget &= ~(VP890_CUT_TXPATH | VP890_CUT_RXPATH);
            Vp890GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState, &mpiByte);
            opCondTarget |= mpiByte;
            if (opCondTarget != pLineObj->opCond[0]) {
                pLineObj->opCond[0] = opCondTarget;
                VP_INFO(VpLineCtxType, pLineCtx, ("9. Writing 0x%02X to Operating Conditions",
                    pLineObj->opCond[0]));

                VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT,
                    VP890_OP_COND_LEN, pLineObj->opCond);
            }
#ifdef VP_CSLAC_SEQ_EN
        }
#endif

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * If we're here, we're sending some tone.  If it's DTMF, we can stop the
     * active cadencer, set the time to "always on" (since the application will
     * tell us when to start/stop).
     *
     * If "direction" is some value other than the initialized value, then
     * the dtmf structure is passed and not NULL
     */
    if (direction != VP_DIRECTION_INVALID) {
#ifdef VP_CSLAC_SEQ_EN
        /* Disable currently active cadence */
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
#endif
        /* Update the DTMF Generators and make the downstream connection */
        Vp890SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

        if (!(pLineObj->status & VP890_IS_FXO)) {
            VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD,
                VP890_CONV_CFG_LEN, converterCfg);
            converterCfg[0] &= ~VP890_CONV_CONNECT_BITS;
            converterCfg[0] |= VP890_METALLIC_AC_V;

            VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT,
                VP890_CONV_CFG_LEN, converterCfg);
        }

        /*
         * Disable only the receive path since disabling the transmit path
         * also may generate noise upstream (e.g., an unterminated, but
         * assigned timeslot
         */
        opCondTarget &= ~(VP890_HIGH_PASS_DIS | VP890_OPCOND_RSVD_MASK);
        opCondTarget |= VP890_CUT_RXPATH;

        if (opCondTarget != pLineObj->opCond[0]) {
            pLineObj->opCond[0] = opCondTarget;

            VP_INFO(VpLineCtxType, pLineCtx, ("15. Writing 0x%02X to Operating Conditions",
                pLineObj->opCond[0]));
            VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT, VP890_OP_COND_LEN,
                pLineObj->opCond);
        }

        /* Enable only generator C/D */
        if (pLineObj->sigGenCtrl[0] != (VP890_GEND_EN | VP890_GENC_EN)) {
            pLineObj->sigGenCtrl[0] = (VP890_GEND_EN | VP890_GENC_EN);
            VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT, VP890_GEN_CTRL_LEN,
                pLineObj->sigGenCtrl);
        }

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

#if defined (VP_CSLAC_SEQ_EN) && defined (VP890_FXS_SUPPORT)
    /* If we're here, we're sending a Tone, not DTMF */
    if ((pCadProf != VP_PTABLE_NULL)
     && ((pCadProf[VP_CSLAC_TONE_TYPE] == VP_CSLAC_HOWLER_TONE)
      || (pCadProf[VP_CSLAC_TONE_TYPE] == VP_CSLAC_AUS_HOWLER_TONE)
      || (pCadProf[VP_CSLAC_TONE_TYPE] == VP_CSLAC_NTT_HOWLER_TONE))) {
        uint8 sigGenCD[VP890_SIGCD_PARAMS_LEN] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        VpCSLACHowlerInit(pCadProf[VP_CSLAC_TONE_TYPE], &pLineObj->cadence);

        sigGenAB[3] = ((pLineObj->cadence.startFreq >> 8) & 0xFF);
        sigGenAB[4] = (pLineObj->cadence.startFreq & 0xFF);

        sigGenAB[5] = ((pLineObj->cadence.startLevel >> 8) & 0xFF);
        sigGenAB[6] = (pLineObj->cadence.startLevel & 0xFF);

        /* Set the signal generator A parameters to the initial value. */
        pLineObj->cadence.isFreqIncrease = TRUE;

        mpiIndex = 0;

        /* Make sure C/D are cleared */
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGCD_PARAMS_WRT,
            VP890_SIGCD_PARAMS_LEN, sigGenCD);

        /* Program A/B */

		/*BEGIN: fix MOTO*/
		if (21 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGAB_PARAMS_WRT,
            VP890_SIGAB_PARAMS_LEN, sigGenAB);
        /* Clear flag to indicate the generators are NOT in a Ringing Mode */
        pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);

        VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
            mpiIndex-1, &mpiBuffer[1]);

        /* Set the parameters in the line object for cadence use */
        VpMemCpy(pLineObj->cadence.regData, sigGenAB, VP890_SIGAB_PARAMS_LEN);
    } else {
#endif
        /*
         * Send the signal generator parameters to the device and enable the
         * Tone Generators -- add in the first 3 bytes (all 0x00)
         */
        VpMemCpy(&sigGenAB[VP890_SIGAB_FREQ_START], &pToneProf[VP890_SIGGEN_AB_START],
            VP890_SIGAB_PARAMS_LEN);

        mpiIndex = 0;
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGAB_PARAMS_WRT,
            VP890_SIGAB_PARAMS_LEN, sigGenAB);
        /* Clear flag to indicate the generators are NOT in a Ringing Mode */
        pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);

		/*BEGIN: fix MOTO*/
		if (21 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGCD_PARAMS_WRT,
            VP890_SIGCD_PARAMS_LEN, (uint8 *)(&pToneProf[VP890_SIGGEN_CD_START]));

        VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
            &mpiBuffer[1]);
#if defined (VP_CSLAC_SEQ_EN) && defined (VP890_FXS_SUPPORT)
    }
#endif

#ifdef VP_CSLAC_SEQ_EN
    if (pCadProf == VP_PTABLE_NULL) {
        /*
         * If a tone is being actived due to caller ID, then do not stop the
         * cadencer
         */
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        }
#endif
        if (pLineObj->sigGenCtrl[0] != VP890_GEN_ALLON) {
            pLineObj->sigGenCtrl[0] = VP890_GEN_ALLON;
            VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT, VP890_GEN_CTRL_LEN,
                pLineObj->sigGenCtrl);
        }

#ifdef VP_CSLAC_SEQ_EN
    } else {
        pLineObj->cadence.pCurrentPos =
            &(pCadProf[VP_PROFILE_TYPE_SEQUENCER_START]);
        pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
        pLineObj->cadence.length = pCadProf[VP_PROFILE_LENGTH];
        pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_IGNORE_POLARITY;
        pLineObj->cadence.status |= (pCadProf[VP_PROFILE_MPI_LEN] & 0x01) ?
            VP_CADENCE_STATUS_IGNORE_POLARITY : 0;

        /* Nullify any internal sequence so that the API doesn't think that
         * an internal sequence of some sort is running */
        pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_NONE;
    }
#endif

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890SetLineTone() */

/*******************************************************************************
 * Vp890SetRelGain()
 *  This function adjusts the GR and GX values for a given channel of a given
 * device.  It multiplies the profile values by a factor from 1.0 to 4.0 (GX) or
 * from 0.25 to 1.0 (GR).  The adjustment factors are specified in the txLevel
 * and rxLevel parameters, which are 2.14 fixed-point numbers.
 *
 * Arguments:
 *  *pLineCtx  -  Line context to change gains on
 *  txLevel    -  Adjustment to line's relative Tx level
 *  rxLevel    -  Adjustment to line's relative Rx level
 *  handle     -  Handle value returned with the event
 *
 * Preconditions:
 *  The line must first be initialized prior to adjusting the gains.  Any
 * pre-existing results must be cleared by calling VpGetResults() before
 * calling this function.
 *
 * Postconditions:
 *  Returns error if device is not initialized or results are not cleared.
 * Otherwise, generates a VE_LINE_EVID_GAIN_CMP event and saves results in
 * the device object for later retrieval by VpGetResults().
 ******************************************************************************/
VpStatusType
Vp890SetRelGain(
    VpLineCtxType   *pLineCtx,
    uint16          txLevel,
    uint16          rxLevel,
    uint16          handle)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj    = pLineCtx->pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpStatusType            status;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890SetRelGain()"));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pDevObj->deviceEvents.response & VP890_READ_RESPONSE_MASK) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp890SetRelGain() - Waiting to clear previous read"));
        return VP_STATUS_DEVICE_BUSY;
    }

    pLineObj->gxUserLevel = txLevel;
    pLineObj->grUserLevel = rxLevel;

    status = Vp890SetRelGainInt(pLineCtx);

    if (status == VP_STATUS_SUCCESS){
        /* Generate the gain-complete event. */
        pLineObj->lineEvents.response |= VP_LINE_EVID_GAIN_CMP;
        pLineObj->lineEventHandle = handle;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp890SetRelGain() */

/*******************************************************************************
 * Vp890SetOption()
 *  This function determines how to process the Option based on pDevCtx,
 *  pLineCtx, and option type.  The actual options are implemented in static
 *  functions SetDeviceOption and SetLineOption.
 *
 * Preconditions:
 *  The line must first be initialized if a line context is passed, or the
 * device must be initialized if a device context is passed.
 *
 * Postconditions:
 *  The option specified is implemented either on the line, or on the device, or
 * on all lines associated with the device (see the API Reference Guide for
 ******************************************************************************/
VpStatusType
Vp890SetOption(
    VpLineCtxType           *pLineCtx,
    VpDevCtxType            *pDevCtx,
    VpOptionIdType          option,
    void                    *value)
{
    Vp890DeviceObjectType   *pDevObj;
    VpDeviceIdType          deviceId;
    VpDevCtxType            *pDevCtxLocal;
    VpStatusType            status          = VP_STATUS_INVALID_ARG;

    VP_API_FUNC_INT(None, VP_NULL, ("+Vp890SetOption()"));

    /* Error checking.  Either device or line context must be valid, but
     * not both. */
    if (pDevCtx != VP_NULL) {
        if (pLineCtx != VP_NULL) {
            return VP_STATUS_INVALID_ARG;
        }
    } else {
        if (pLineCtx == VP_NULL) {
            return VP_STATUS_INVALID_ARG;
        }
    }

    /* Get Device Object based on input param */
    if (pDevCtx != VP_NULL) {
        pDevObj = pDevCtx->pDevObj;
    } else {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
    }

    deviceId = pDevObj->deviceId;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pDevCtx != VP_NULL) {
        status = SetDeviceOption(pDevCtx, deviceId, option, value);
    } else {
        status = SetLineOption(pLineCtx, deviceId, option, value);
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* Vp890SetOption() */

/*******************************************************************************
 * SetDeviceOption()
 *  This function determines how to process Options based on pDevCtx. Any
 *  options that are line specific, loop through all existing lines
 *  associated with pDevCtx calling SetLineOption() with the LineCtx
 *  associated with that line.
 *
 *  Any options that are device specific, Carry out their task here.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  This function returns the success code if the option associated with this
 *  function completes without issues.
 ******************************************************************************/
static VpStatusType
SetDeviceOption(
    VpDevCtxType        *pDevCtx,
    VpDeviceIdType      deviceId,
    VpOptionIdType      option,
    void                *value)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpLineCtxType           *pLineCtx;
    Vp890LineObjectType     *pLineObj;
    VpStatusType            status      = VP_STATUS_SUCCESS;
    VpOptionDeviceIoType    deviceIo;
    uint8                   ioDirection = 0;
    uint8                   ecVal = 0;
    uint8                   chanId;

    VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("+SetDeviceOption()"));

    /*
     * Valid Device Context, we already know Line context is NULL (higher
     * layer SW, process on device if device option, or process on all lines
     * associated with device if line option
     */
    switch (option) {
        /* Line Options */
        case VP_OPTION_ID_EVENT_MASK:  /* Affects Line and Device */

#ifdef VP890_FXS_SUPPORT
        case VP_OPTION_ID_ZERO_CROSS:
        case VP_OPTION_ID_PULSE_MODE:
        case VP_OPTION_ID_LINE_STATE:
        case VP_OPTION_ID_RING_CNTRL:
        case VP_OPTION_ID_SWITCHER_CTRL:
#endif

        case VP_OPTION_ID_TIMESLOT:
        case VP_OPTION_ID_CODEC:
        case VP_OPTION_ID_PCM_HWY:
        case VP_OPTION_ID_LOOPBACK:
        case VP_OPTION_ID_PCM_TXRX_CNTRL:
#ifdef CSLAC_GAIN_ABS
        case VP_OPTION_ID_ABS_GAIN:
#endif
            /*
             * Loop through all of the valid channels associated with this
             * device. Init status variable in case there are currently no
             * line contexts associated with this device
             */
            status = VP_STATUS_SUCCESS;
            for (chanId = 0; chanId < pDevObj->staticInfo.maxChannels; chanId++) {

                pLineCtx = pDevCtx->pLineCtx[chanId];

                if (pLineCtx == VP_NULL) {
                    continue;
                }

#ifdef VP890_FXS_SUPPORT
                if ((option == VP_OPTION_ID_ZERO_CROSS) ||
                    (option == VP_OPTION_ID_PULSE_MODE) ||
                    (option == VP_OPTION_ID_LINE_STATE) ||
                    (option == VP_OPTION_ID_RING_CNTRL) ||
                    (option == VP_OPTION_ID_SWITCHER_CTRL)){

                    bool onlyFXO = TRUE;
                    uint8 lastChannel = (pDevObj->staticInfo.maxChannels - 1);

                    pLineObj = pLineCtx->pLineObj;

                    /* This device has at least 1 FXS, SetOption will succeed */
                    if (!(pLineObj->status & VP890_IS_FXO)) {
                        onlyFXO = FALSE;
                        status = SetLineOption(pLineCtx, deviceId, option, value);
                    /* Only FXO on this device */
                    } else if ((onlyFXO == TRUE) && (chanId == lastChannel)) {
                        status = VP_STATUS_OPTION_NOT_SUPPORTED;
                    /* Just bailout in case there is at least 1 FXS on this device */
                    } else {
                        break;
                    }
                } else {
                    status = SetLineOption(pLineCtx, deviceId, option, value);
                }
#endif
                if (VP_STATUS_SUCCESS != status) {
                    return status;
                }
            }
            break;

#ifdef VP890_FXS_SUPPORT
        case VP_DEVICE_OPTION_ID_PULSE:
            if (pDevObj->stateInt & VP890_IS_FXO_ONLY) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }
            pDevObj->pulseSpecs = *((VpOptionPulseType *)value);
            break;

        case VP_DEVICE_OPTION_ID_PULSE2:
            if (pDevObj->stateInt & VP890_IS_FXO_ONLY) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }
            pDevObj->pulseSpecs2 = *((VpOptionPulseType *)value);
            break;

        case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
            if (pDevObj->stateInt & VP890_IS_FXO_ONLY) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }

            pDevObj->criticalFault = *((VpOptionCriticalFltType *)value);

            if ((pDevObj->criticalFault.acFltDiscEn == TRUE)
             || (pDevObj->criticalFault.dcFltDiscEn == TRUE)) {
                pDevObj->criticalFault.acFltDiscEn = FALSE;
                pDevObj->criticalFault.dcFltDiscEn = FALSE;
                return VP_STATUS_INVALID_ARG;
            }

            for (chanId = 0; chanId < pDevObj->staticInfo.maxChannels; chanId++) {
                uint8 tempSysConfig[VP890_SS_CONFIG_LEN];

                pLineCtx = pDevCtx->pLineCtx[chanId];
                if (pLineCtx == VP_NULL) {
                    continue;
                }
                pLineObj = pLineCtx->pLineObj;
                if (pLineObj->status & VP890_IS_FXO) {
                    continue;
                }
                VpMpiCmdWrapper(deviceId, pLineObj->ecVal, VP890_SS_CONFIG_RD,
                    VP890_SS_CONFIG_LEN, tempSysConfig);

                if (pDevObj->criticalFault.thermFltDiscEn == TRUE) {
                    tempSysConfig[0] |= VP890_ATFS_EN;
                } else {
                    tempSysConfig[0] &= ~(VP890_ATFS_EN);
                }
                VpMpiCmdWrapper(deviceId, pLineObj->ecVal, VP890_SS_CONFIG_WRT,
                    VP890_SS_CONFIG_LEN, tempSysConfig);
            }
            break;
#endif

        case VP_DEVICE_OPTION_ID_DEVICE_IO:
            VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("+SetDeviceOption() - OPTION_ID_DEVICE_IO"));

            deviceIo = *(VpOptionDeviceIoType *)(value);
            ioDirection = deviceIo.directionPins_31_0 << 1;
            ioDirection &= ~VP890_IODIR_IO1_MASK;

            /* Default 00 is input */
            if ((deviceIo.directionPins_31_0 & 1) == VP_IO_OUTPUT_PIN) {
                /* Driven output only is supported. */
                ioDirection |= VP890_IODIR_IO1_OUTPUT;
            }

            for(chanId=0; chanId < pDevObj->staticInfo.maxChannels; chanId++ ) {
                pLineCtx = pDevCtx->pLineCtx[chanId];
                if (pLineCtx != VP_NULL) {
                    pLineObj = pLineCtx->pLineObj;
                    ecVal |= pLineObj->ecVal;

#ifdef VP890_FXS_SUPPORT
                    if ((pLineObj->termType == VP_TERM_FXS_SPLITTER_LP)
                     || (pLineObj->termType == VP_TERM_FXS_SPLITTER)
                     || (pLineObj->termType == VP_TERM_FXS_ISOLATE)
                     || (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP)) {
                        /* Splitter/Isolate I/O1 direction must be unchanged */
                        /*
                         * Note that this works only if the line contexts are
                         * associated with the device context passed to this
                         * function -- which is not a requirement.
                         */
                        uint8 localIoDir[VP890_IODIR_REG_LEN];
                        VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_RD,
                            VP890_IODIR_REG_LEN, localIoDir);

                        ioDirection &= ~VP890_IODIR_IO1_MASK;
                        ioDirection |= (localIoDir[0] & VP890_IODIR_IO1_MASK);
                    }
#endif
                }
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_WRT, VP890_IODIR_REG_LEN,
                &ioDirection);
            VP_API_FUNC_INT(VpDevCtxType, pDevCtx, (" -SetDeviceOption() - OPTION_ID_DEVICE_IO"));
            break;

#ifdef VP_DEBUG
        case VP_OPTION_ID_DEBUG_SELECT:
            /* Update the debugSelectMask in the Device Object. */
            pDevObj->debugSelectMask = *(uint32 *)value;
            break;
#endif

        default:
            status = VP_STATUS_OPTION_NOT_SUPPORTED;
            VP_ERROR(VpDevCtxType, pDevCtx, ("SetDeviceOption() - Device option not supported 0x%02X",
                option));
            break;
    }

    return status;
} /* SetDeviceOption() */

/*******************************************************************************
 * SetLineOption()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
SetLineOption(
    VpLineCtxType           *pLineCtx,
    VpDeviceIdType          deviceId,
    VpOptionIdType          option,
    void                    *value)
{
    VpDevCtxType            *pDevCtxLocal   = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj        = pDevCtxLocal->pDevObj;
    VpStatusType            status          = VP_STATUS_SUCCESS;

    uint8                   ecVal           = pLineObj->ecVal;
    uint8                   txSlot, rxSlot;
    uint8                   mpiData, mpiByte;
    uint8                   tempLoopBack[VP890_LOOPBACK_LEN];

#ifdef VP890_FXS_SUPPORT
    uint8                   tempSysConfig[VP890_SS_CONFIG_LEN];
#endif

    VP_API_FUNC_INT(VpDeviceIdType, &deviceId, ("+SetLineOption()"));

    switch (option) {
        /* Device Options - invalid arg */
        case VP_DEVICE_OPTION_ID_DEVICE_IO:

#ifdef VP890_FXS_SUPPORT
        case VP_DEVICE_OPTION_ID_PULSE:
        case VP_DEVICE_OPTION_ID_PULSE2:
        case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
#endif
            return VP_STATUS_INVALID_ARG;

#ifdef CSLAC_GAIN_ABS
        case VP_OPTION_ID_ABS_GAIN:
            status = VpCSLACSetAbsGain(pLineCtx, ((VpOptionAbsGainType *)value));
            break;
#endif

        case VP_OPTION_ID_EVENT_MASK:
             SetOptionEventMask(pDevObj, pLineObj, deviceId, ecVal, value);
             break;

        case VP_OPTION_ID_TIMESLOT:
            txSlot = ((VpOptionTimeslotType *)value)->tx;
            rxSlot = ((VpOptionTimeslotType *)value)->rx;
            status = SetTimeSlot(pLineCtx, txSlot, rxSlot);
            break;

        case VP_OPTION_ID_CODEC:
            status = Vp890SetCodec(pLineCtx, *((VpOptionCodecType *)value));
            break;

        case VP_OPTION_ID_PCM_HWY:
            if (*((VpOptionPcmHwyType *)value) != VP_OPTION_HWY_A) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("SetLineOption() - Invalid PCM Highway option"));
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_OPTION_ID_LOOPBACK:
            /* Timeslot loopback via loopback register */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOPBACK_RD,
                VP890_LOOPBACK_LEN, tempLoopBack);

            switch(*((VpOptionLoopbackType *)value)) {
                case VP_OPTION_LB_TIMESLOT:
                    tempLoopBack[0] |= VP890_INTERFACE_LOOPBACK_EN;
                    pDevObj->devMode[0] &= ~(VP890_DEV_MODE_CH21PT | VP890_DEV_MODE_CH12PT);
                    break;

                case VP_OPTION_LB_OFF:
                    tempLoopBack[0] &= ~(VP890_INTERFACE_LOOPBACK_EN);
                    pDevObj->devMode[0] &= ~(VP890_DEV_MODE_CH21PT | VP890_DEV_MODE_CH12PT);
                    break;

                case VP_OPTION_LB_CHANNELS:
                    tempLoopBack[0] &= ~(VP890_INTERFACE_LOOPBACK_EN);
                    pDevObj->devMode[0] |= (VP890_DEV_MODE_CH21PT | VP890_DEV_MODE_CH12PT);
                    break;

                case VP_OPTION_LB_DIGITAL:
                default:
                    VP_ERROR(VpLineCtxType, pLineCtx, ("SetLineOption() - Invalid Loopback option"));
                    return VP_STATUS_INVALID_ARG;
            }

            VP_INFO(VpLineCtxType, pLineCtx, ("1. Loopback 0x%02X", tempLoopBack[0]));
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOPBACK_WRT,
                VP890_LOOPBACK_LEN, tempLoopBack);
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DEV_MODE_WRT,
                VP890_DEV_MODE_LEN, pDevObj->devMode);
            break;

#ifdef VP890_FXS_SUPPORT
        /* Line Options */
        case VP_OPTION_ID_PULSE_MODE:
            /* Option does not apply to FXO */
            if (pLineObj->status & VP890_IS_FXO) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }
            pLineObj->pulseMode = *((VpOptionPulseModeType *)value);
            VpInitDP(&pLineObj->dpStruct);
            VpInitDP(&pLineObj->dpStruct2);
            break;

        case VP_OPTION_ID_LINE_STATE:
            /* Option does not apply to FXO */
            if (pLineObj->status & VP890_IS_FXO) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }
            /*
             * Only supports one type of battery control, so make sure it
             * is set correctly. If not, return error otherwise continue
             */
            if (((VpOptionLineStateType *)value)->bat != VP_OPTION_BAT_AUTO) {
                VP_ERROR(VpLineCtxType, pLineCtx, ("SetLineOption() - Invalid battery option"));
                return VP_STATUS_INVALID_ARG;
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD,
                VP890_SS_CONFIG_LEN, tempSysConfig);

            if (((VpOptionLineStateType *)value)->battRev == TRUE) {
                tempSysConfig[0] &= ~(VP890_SMOOTH_PR_EN);
            } else {
                tempSysConfig[0] |= VP890_SMOOTH_PR_EN;
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT,
                VP890_SS_CONFIG_LEN, tempSysConfig);
            break;

        case VP_OPTION_ID_ZERO_CROSS:
            /* Option does not apply to FXO */
            if (pLineObj->status & VP890_IS_FXO) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD,
                VP890_SS_CONFIG_LEN, tempSysConfig);

            if (*(VpOptionZeroCrossType *)value == VP_OPTION_ZC_NONE) {
                tempSysConfig[0] |= VP890_ZXR_DIS;
            } else {
                tempSysConfig[0] &= ~(VP890_ZXR_DIS);
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT,
                VP890_SS_CONFIG_LEN, tempSysConfig);

            pLineObj->ringCtrl.zeroCross = *((VpOptionZeroCrossType *)value);
            break;

        case VP_OPTION_ID_RING_CNTRL: {
            VpOptionRingControlType ringCtrl = *((VpOptionRingControlType *)value);

            /* Option does not apply to FXO */
            if (pLineObj->status & VP890_IS_FXO) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }

            if (Vp890IsSupportedFxsState(ringCtrl.ringTripExitSt) == FALSE) {
                return VP_STATUS_INVALID_ARG;
            }

            pLineObj->ringCtrl = ringCtrl;

            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD,
                VP890_SS_CONFIG_LEN, tempSysConfig);

            if (pLineObj->ringCtrl.zeroCross == VP_OPTION_ZC_NONE) {
                tempSysConfig[0] |= VP890_ZXR_DIS;
            } else {
                tempSysConfig[0] &= ~(VP890_ZXR_DIS);
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT,
                VP890_SS_CONFIG_LEN, tempSysConfig);
            }
            break;

        case VP_OPTION_ID_SWITCHER_CTRL: {
            bool shutDownEn = *((bool *)value);
            uint8 ssConfig[VP890_SS_CONFIG_LEN];

            /* Option does not apply to FXO */
            if (pLineObj->status & VP890_IS_FXO) {
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD, VP890_SS_CONFIG_LEN, ssConfig);

            if (shutDownEn == TRUE) {
                ssConfig[0] |= VP890_AUTO_BAT_SHUTDOWN_EN;
            } else {
                ssConfig[0] &= ~VP890_AUTO_BAT_SHUTDOWN_EN;
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT, VP890_SS_CONFIG_LEN, ssConfig);
            }
            break;
#endif

        case VP_OPTION_ID_PCM_TXRX_CNTRL:
            pLineObj->pcmTxRxCtrl = *((VpOptionPcmTxRxCntrlType *)value);

            mpiData = pLineObj->opCond[0];

            VP_INFO(VpLineCtxType, pLineCtx, ("16.a Data Read 0x%02X from Operating Conditions",
                mpiData));

            mpiData &= ~(VP890_CUT_TXPATH | VP890_CUT_RXPATH);
            mpiData &= ~(VP890_HIGH_PASS_DIS | VP890_OPCOND_RSVD_MASK);

            Vp890GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState, &mpiByte);
            mpiData |= mpiByte;
            VP_INFO(VpLineCtxType, pLineCtx, ("16.b Writing 0x%02X to Operating Conditions",
                mpiData));
            if (mpiData != pLineObj->opCond[0]) {
                VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT, VP890_OP_COND_LEN,
                    &mpiData);
                pLineObj->opCond[0] = mpiData;
            }
            break;

#ifdef VP_DEBUG
        case VP_OPTION_ID_DEBUG_SELECT:
            /* Update the debugSelectMask in the Line Object. */
            pLineObj->debugSelectMask = *(uint32 *)value;
            break;
#endif

        default:
            status = VP_STATUS_OPTION_NOT_SUPPORTED;
            VP_ERROR(VpLineCtxType, pLineCtx, ("SetLineOption() - Line option not supported 0x%02X",
                option));
            break;
    }

    return status;
} /* SetLineOption() */

/*******************************************************************************
 * SetOptionEventMask()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static void
SetOptionEventMask(
    Vp890DeviceObjectType   *pDevObj,
    Vp890LineObjectType     *pLineObj,
    VpDeviceIdType          deviceId,
    uint8                   ecVal,
    void                    *value)
{
    uint8                   chanId      = pLineObj->channelId;
    VpOptionEventMaskType   *pEventsMask, *pNewEventsMask;
    uint16                  eventMask;
    uint8                   tempData[VP890_INT_MASK_LEN];

    VP_API_FUNC_INT(VpDeviceIdType, &deviceId, ("+SetOptionEventMask()"));

    pNewEventsMask = (VpOptionEventMaskType *)value;

    /* Zero out the line-specific bits before setting the
     * deviceEventsMask in the device object. */
    pEventsMask = &pDevObj->deviceEventsMask;

    pEventsMask->faults     = pNewEventsMask->faults    & VP_EVCAT_FAULT_DEV_EVENTS;
    pEventsMask->signaling  = pNewEventsMask->signaling & VP_EVCAT_SIGNALING_DEV_EVENTS;
    pEventsMask->response   = pNewEventsMask->response  & VP_EVCAT_RESPONSE_DEV_EVENTS;
    pEventsMask->test       = pNewEventsMask->test      & VP_EVCAT_TEST_DEV_EVENTS;
    pEventsMask->process    = pNewEventsMask->process   & VP_EVCAT_PROCESS_DEV_EVENTS;
    pEventsMask->fxo        = pNewEventsMask->fxo       & VP_EVCAT_FXO_DEV_EVENTS;

    /* Zero out the device-specific bits before setting the
     * lineEventsMask in the line object. */
    pEventsMask = &pLineObj->lineEventsMask;

    pEventsMask->faults     = pNewEventsMask->faults    & ~VP_EVCAT_FAULT_DEV_EVENTS;
    pEventsMask->signaling  = pNewEventsMask->signaling & ~VP_EVCAT_SIGNALING_DEV_EVENTS;
    pEventsMask->response   = pNewEventsMask->response  & ~VP_EVCAT_RESPONSE_DEV_EVENTS;
    pEventsMask->test       = pNewEventsMask->test      & ~VP_EVCAT_TEST_DEV_EVENTS;
    pEventsMask->process    = pNewEventsMask->process   & ~VP_EVCAT_PROCESS_DEV_EVENTS;
    pEventsMask->fxo        = pNewEventsMask->fxo       & ~VP_EVCAT_FXO_DEV_EVENTS;

    /* Unmask the unmaskable defined in vp_api_common.c */
    VpImplementNonMaskEvents(&pLineObj->lineEventsMask, &pDevObj->deviceEventsMask);

    /* Mask those events that the VP890 API-II cannot generate */
    MaskNonSupportedEvents(&pLineObj->lineEventsMask, &pDevObj->deviceEventsMask);

    /*
     * The next code section prevents the device from interrupting
     * the processor if all of the events associated with the
     * specific hardware interrupt are masked
     */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_INT_MASK_RD, VP890_INT_MASK_LEN, tempData);

    /* Keep Clock Fault Interrupt Enabled to support auto-free run mode. */
    tempData[0] &= ~VP890_CFAIL_MASK;

    if (!(pLineObj->status & VP890_IS_FXO)) {
    /* Line is FXS */
        /* Mask off the FXO events */
        pLineObj->lineEventsMask.fxo |= VP_EVCAT_FXO_MASK_ALL;

        /*
         * Never mask the thermal fault interrupt otherwise the
         * actual thermal fault may not be seen by the VP-API-II.
         */
        tempData[chanId] &= ~VP890_TEMPA_MASK;

        /* Evaluate for signaling events */
        eventMask = pLineObj->lineEventsMask.signaling;

        /*
         * Never mask the hook interrupt otherwise interrupt modes
         * of the VP-API-II for LPM types won't work -- hook status
         * is never updated, leaky line never properly detected.
         */
        tempData[chanId] &= ~VP890_HOOK_MASK;

        /*
         * Never mask the gkey interrupt otherwise interrupt modes
         * of the VP-API-II won't support "get line status"
         * correctly.
         */
        tempData[chanId] &= ~VP890_GNK_MASK;

        /* Disable Overcurrent Alaram on device until debounce put in place */
        tempData[chanId] &= ~(VP890_OCALMY_MASK);
    } else {
    /* Line is FXO */
        /* Mask off the FXS events */
        pLineObj->lineEventsMask.signaling |= VP890_FXS_SIGNALING_EVENTS;

        /* Evaluate for fxo events */
        eventMask = pLineObj->lineEventsMask.fxo;

        if ((eventMask & VP_LINE_EVID_RING_ON)
         && (eventMask & VP_LINE_EVID_RING_OFF)) {
            tempData[chanId] |= VP890_RING_DET_MASK;
        } else {
            tempData[chanId] &= ~VP890_RING_DET_MASK;
        }

        if ((eventMask & VP_LINE_EVID_LIU)
         && (eventMask & VP_LINE_EVID_LNIU)) {
            tempData[chanId] |= VP890_LIU_MASK;
        } else {
            tempData[chanId] &= ~VP890_LIU_MASK;
        }

        if (eventMask & VP_LINE_EVID_POLREV) {
            tempData[chanId] |= VP890_POL_MASK;
        } else {
            tempData[chanId] &= ~VP890_POL_MASK;
        }

        if ((eventMask & VP_LINE_EVID_DISCONNECT)
         && (eventMask & VP_LINE_EVID_RECONNECT)
         && (eventMask & VP_LINE_EVID_FEED_DIS)
         && (eventMask & VP_LINE_EVID_FEED_EN)) {
            tempData[chanId] |= VP890_LDN_MASK;
        } else {
            tempData[chanId] &= ~VP890_LDN_MASK;
        }

        /* Evaluate for line faults events */
        eventMask = pLineObj->lineEventsMask.faults;
        if (eventMask & VP_LINE_EVID_AC_FLT) {
            tempData[chanId] |= VP890_OVIR_MASK;
        } else {
            tempData[chanId] &= ~VP890_OVIR_MASK;
        }

        /* Workaround:  If VISTAT is masked, changing the FXO state can clear
         * existing interrupts.  Force it to be unmasked to avoid this.
         *
         * Specific error case:  Start with FXO offhook and disconnected.
         * Reconnect feed, this will pull the interrupt low.  Go onhook, and
         * the interrupt will go back to high if VISTAT is masked. */
        tempData[chanId] &= ~VP890_VISTAT_MASK;
    }
    VpMpiCmdWrapper(deviceId, ecVal, VP890_INT_MASK_WRT, VP890_INT_MASK_LEN, tempData);

    return;
} /* SetOptionEventMask() */

/*******************************************************************************
 * MaskNonSupportedEvents()
 *  This function masks the events that are not supported by the VP890 API-II.
 * It should only be called by SetOptionInternal when event masks are being
 * modified.
 *
 * Arguments:
 *   pLineEventsMask - Line Events Mask to modify for non-masking
 *   pDevEventsMask  - Device Events Mask to modify for non-masking
 *
 * Preconditions:
 *  None. Utility function to modify event structures only.
 *
 * Postconditions:
 *  Event structures passed are modified with masked bits for non-supported
 * VP890 API-II events.
 ******************************************************************************/
void
MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask,
    VpOptionEventMaskType *pDevEventsMask)
{
    VP_API_FUNC_INT(None, VP_NULL, ("+MaskNonSupportedEvents()"));

    pLineEventsMask->faults     |= VP890_NONSUPPORT_FAULT_EVENTS;
    pLineEventsMask->signaling  |= VP890_NONSUPPORT_SIGNALING_EVENTS;
    pLineEventsMask->response   |= VP890_NONSUPPORT_RESPONSE_EVENTS;
    pLineEventsMask->test       |= VP890_NONSUPPORT_TEST_EVENTS;
    pLineEventsMask->process    |= VP890_NONSUPPORT_PROCESS_EVENTS;
    pLineEventsMask->fxo        |= VP890_NONSUPPORT_FXO_EVENTS;

    pDevEventsMask->faults      |= VP890_NONSUPPORT_FAULT_EVENTS;
    pDevEventsMask->signaling   |= VP890_NONSUPPORT_SIGNALING_EVENTS;
    pDevEventsMask->response    |= VP890_NONSUPPORT_RESPONSE_EVENTS;
    pDevEventsMask->test        |= VP890_NONSUPPORT_TEST_EVENTS;
    pDevEventsMask->process     |= VP890_NONSUPPORT_PROCESS_EVENTS;
    pDevEventsMask->fxo         |= VP890_NONSUPPORT_FXO_EVENTS;

    return;
} /* MaskNonSupportedEvents() */

/*******************************************************************************
 * Vp890SetCodec()
 *  This function sets the codec mode on the line specified.
 *
 * Arguments:
 *   pLineCtx   - Line Context
 *   codec      - Encoding, as defined by LineCodec typedef
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The codec mode on the line is set.  This function returns the success code
 * if the codec mode specified is supported.
 ******************************************************************************/
VpStatusType
Vp890SetCodec(
    VpLineCtxType           *pLineCtx,
    VpOptionCodecType       codec)
{
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;

    uint8                   codecReg;
    uint8                   ecVal     = pLineObj->ecVal;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890SetCodec()"));

    /* Basic error checking */
    if ((codec != VP_OPTION_LINEAR) &&
        (codec != VP_OPTION_ALAW)   &&
        (codec != VP_OPTION_MLAW)   &&
        (codec != VP_OPTION_WIDEBAND)) {

        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetCodec() - Invalid codec"));
        return VP_STATUS_INVALID_ARG;
    }

    /* Adjust the EC value for Wideband mode as needed */
    ecVal &= ~VP890_WIDEBAND_MODE;
    ecVal |= ((codec == VP_OPTION_WIDEBAND) ? VP890_WIDEBAND_MODE : 0);

    /*
     * Wideband requires 1/2 rate reduction in device programmed rate to
     * maintain the same real sample rate.
     */
    if(((pLineObj->codec == VP_OPTION_WIDEBAND) && (codec != VP_OPTION_WIDEBAND))
    || ((pLineObj->codec != VP_OPTION_WIDEBAND) && (codec == VP_OPTION_WIDEBAND))) {
        uint8 converterCfg[VP890_CONV_CFG_LEN];
        uint8 newValue;

        pDevObj->devTimer[VP_DEV_TIMER_WB_MODE_CHANGE] =
            MS_TO_TICKRATE(VP_WB_CHANGE_MASK_TIME,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN,
            converterCfg);
        converterCfg[0] &= ~VP890_CC_RATE_MASK;

        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("CODEC Change to %sWideband Mode at Time %d",
            ((codec != VP_OPTION_WIDEBAND) ? "non-" : ""), pDevObj->timeStamp));

        /* Adjust the pcm buffer update rate based on the tickrate and CODEC */
        if(pDevObj->devProfileData.tickRate <=160) {
            newValue = ((codec == VP_OPTION_WIDEBAND) ? VP890_CC_4KHZ_RATE : VP890_CC_8KHZ_RATE);
        } else if(pDevObj->devProfileData.tickRate <=320){
            newValue = ((codec == VP_OPTION_WIDEBAND) ? VP890_CC_2KHZ_RATE : VP890_CC_4KHZ_RATE);
        } else if(pDevObj->devProfileData.tickRate <=640){
            newValue = ((codec == VP_OPTION_WIDEBAND) ? VP890_CC_1KHZ_RATE : VP890_CC_2KHZ_RATE);
        } else if(pDevObj->devProfileData.tickRate <=1280){
            newValue = ((codec == VP_OPTION_WIDEBAND) ? VP890_CC_500HZ_RATE : VP890_CC_1KHZ_RATE);
        } else {
            newValue = VP890_CC_500HZ_RATE;
        }

        pDevObj->txBufferDataRate = newValue;
        converterCfg[0] |= newValue;

       /*
         * If channel is going to Wideband mode, we can immediately update the
         * device object. But if leaving Wideband mode, we have to let the tick
         * manage it because the other line may still be in Wideband mode.
         */
        if (codec == VP_OPTION_WIDEBAND) {
            pDevObj->ecVal |= VP890_WIDEBAND_MODE;
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            converterCfg);
        pDevObj->lastCodecChange = pLineObj->channelId + 1;
    }

    /* Read the current state of the codec register */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN, &codecReg);

    /* Enable the desired CODEC mode */
    switch(codec) {
        /* Wideband same CODEC mode as Linear PCM */
        case VP_OPTION_WIDEBAND:
        case VP_OPTION_LINEAR:      /* 16 bit linear PCM */
            codecReg |= VP890_LINEAR_CODEC;
            break;

        case VP_OPTION_ALAW:                /* A-law PCM */
            codecReg &= ~(VP890_LINEAR_CODEC | VP890_ULAW_CODEC);
            break;

        case VP_OPTION_MLAW:                /* u-law PCM */
            codecReg |= VP890_ULAW_CODEC;
            codecReg &= ~(VP890_LINEAR_CODEC);
            break;

        default:
            /* Cannot reach here.  Error checking at top */
            break;
    } /* Switch */

    VP_INFO(VpLineCtxType, pLineCtx, ("1. Writing 0x%02X to Operating Functions EC 0x%02X",
        codecReg, ecVal));
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_WRT, VP890_OP_FUNC_LEN, &codecReg);

    pLineObj->codec = codec;
    pLineObj->ecVal = ecVal;

    return VP_STATUS_SUCCESS;
} /* Vp890SetCodec() */

/*******************************************************************************
 * SetTimeSlot()
 *  This function set the RX and TX timeslot for a device channel. Valid
 * timeslot numbers start at zero. The upper bound is system dependent.
 *
 * Arguments:
 *   pLineCtx   - Line Context
 *   txSlot     - The TX PCM timeslot
 *   rxSlot     - The RX PCM timeslot
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The timeslots on the line are set.  This function returns the success code
 * if the timeslot numbers specified are within the range of the device based on
 * the PCLK rate.
 ******************************************************************************/
static VpStatusType
SetTimeSlot(
    VpLineCtxType           *pLineCtx,
    uint8                   txSlot,
    uint8                   rxSlot)
{
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;

    uint8                   mpiBuffer[2 + VP890_TX_TS_LEN + VP890_RX_TS_LEN];
    uint8                   mpiIndex = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+SetTimeSlot()"));

    /* device only supports 127 time slots */
    if ((VP890_TX_TS_MAX < txSlot) || (VP890_RX_TS_MAX < rxSlot)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SetTimeSlot() - Time slot out of range.  rx %d, tx %d",
            rxSlot, txSlot));
        return VP_STATUS_INPUT_PARAM_OOR;
    }

    /* Validate the tx time slot value */
    if(txSlot >= pDevObj->devProfileData.pcmClkRate / 64) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SetTimeSlot() - Bad Tx time slot value"));
        return VP_STATUS_INPUT_PARAM_OOR;
    }

    /* Validate the rx time slot value */
    if(rxSlot >= pDevObj->devProfileData.pcmClkRate / 64) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("SetTimeSlot() - Bad Rx time slot value"));
        return VP_STATUS_INVALID_ARG;
    }

    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_TX_TS_WRT,
        VP890_TX_TS_LEN, &txSlot);

	/*BEGIN: fix MOTO*/
	if (4 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_RX_TS_WRT,
        VP890_RX_TS_LEN, &rxSlot);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
        &mpiBuffer[1]);

    return VP_STATUS_SUCCESS;
} /* SetTimeSlot() */

/*******************************************************************************
 * Vp890DeviceIoAccess()
 *  This function is used to access device IO pins of the Vp890. See API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Reads/Writes from device IO pins.
 ******************************************************************************/
VpStatusType
Vp890DeviceIoAccess(
    VpDevCtxType             *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData)
{
    Vp890DeviceObjectType    *pDevObj   = pDevCtx->pDevObj;
    VpLineCtxType            *pLineCtx;
    Vp890LineObjectType      *pLineObj;
    VpDeviceIdType           deviceId   = pDevObj->deviceId;
    uint8                    ecVal      = 0;
    uint8                    ioData;
    uint8                    chanId;
    VpStatusType             status     = VP_STATUS_SUCCESS;

    VpDeviceIoAccessDataType *pAccessData =
        &(pDevObj->getResultsOption.optionData.deviceIoData);

    VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("+Vp890DeviceIoAccess()"));

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Get wideband mode info from each channel's ecVal. */
    for(chanId=0; chanId < pDevObj->staticInfo.maxChannels; chanId++ ) {
        pLineCtx = pDevCtx->pLineCtx[chanId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            ecVal |= pLineObj->ecVal;

            if ((pLineObj->termType == VP_TERM_FXS_SPLITTER_LP)
             || (pLineObj->termType == VP_TERM_FXS_SPLITTER)
             || (pLineObj->termType == VP_TERM_FXS_ISOLATE)) {
                if ((pDeviceIoData->accessMask_31_0 & 0x1)
                 && (pDeviceIoData->accessType == VP_DEVICE_IO_WRITE)) {
                    status = VP_STATUS_DEDICATED_PINS;
                }
                /* Can't let I/O1 Change on Splitter/Isolate Types */
                pDeviceIoData->accessMask_31_0 &= 0xfffffffe;
            }
        }
    }

    *pAccessData = *pDeviceIoData;

    VpMpiCmdWrapper(deviceId, ecVal, VP890_IODATA_REG_RD, VP890_IODATA_REG_LEN,
        &ioData);
    if (pDeviceIoData->accessType == VP_DEVICE_IO_READ) {
        pAccessData->deviceIOData_31_0 =
            ioData & pDeviceIoData->accessMask_31_0;
        pAccessData->deviceIOData_63_32 = 0;
    } else { /* pDeviceIoData->accessType == VP_DEVICE_IO_WRITE */
        ioData &= ~(pDeviceIoData->accessMask_31_0 & VP890_IODATA_IOMASK);
        ioData |= pDeviceIoData->deviceIOData_31_0 &
            (pDeviceIoData->accessMask_31_0 & VP890_IODATA_IOMASK);
        VpMpiCmdWrapper(deviceId, ecVal, VP890_IODATA_REG_WRT, VP890_IODATA_REG_LEN,
            &ioData);
    }
    pDevObj->deviceEvents.response |= VP_DEV_EVID_IO_ACCESS_CMP;

    VP_API_FUNC_INT(VpDevCtxType, pDevCtx, (" -Vp890DeviceIoAccess()"));
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp890DeviceIoAccess() */

/*******************************************************************************
 * Vp890LowLevelCmd()
 *  This function provides direct MPI access to the line/device.
 *
 * Preconditions:
 *  The device associated with the line, and the line must first be initialized.
 *
 * Postconditions:
 *  The command data is passed over the MPI bus and affects only the line passed
 * if the command is line specific, and an event is generated.  If a read
 * command is performed, the user must read the results or flush events.  This

 * function returns the success code if the device is not already in a state
 * where the results must be read.
 ******************************************************************************/
VpStatusType
Vp890LowLevelCmd(
    VpLineCtxType       *pLineCtx,
    uint8               *pCmdData,
    uint8               len,
    uint16              handle)
{
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;
    uint8                   ecVal     = pLineObj->ecVal;

    int i; /* For-loop var */

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890LowLevelCmd()"));

    if (pLineObj->lineEvents.response & VP890_READ_RESPONSE_MASK) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp890LowLevelCmd() - Waiting to clear previous read"));
        return VP_STATUS_DEVICE_BUSY;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    if(pCmdData[0] & 0x01) { /* Read Command */
        VpMpiCmdWrapper(deviceId, ecVal, pCmdData[0], len, &(pDevObj->mpiData[0]));
        pDevObj->mpiLen = len;
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_RX_CMP;
    } else {
        VpMpiCmdWrapper(deviceId, ecVal, pCmdData[0], len, &pCmdData[1]);
        for (i = 0; i < len; i++) {
            pDevObj->mpiData[i] = pCmdData[i];
        }
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_TX_CMP;
    }
    pLineObj->lineEventHandle = handle;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* Vp890LowLevelCmd() */

#endif /* VP_CC_890_SERIES */

