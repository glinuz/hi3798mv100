/** \file vp890_control.c
 * vp890_control.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Control Functions.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 6638 $
 * $LastChangedDate: 2010-03-08 16:08:28 -0600 (Mon, 08 Mar 2010) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES) && defined (VP890_FXO_SUPPORT)

#include    "vp_api_int.h"
#include    "vp890_api_int.h"

/* =================================
    Prototypes for Static Functions
   ================================= */
void
Vp890AmpFreqRingDetect(
    VpLineCtxType *pLineCtx,
    VpCslacLineCondType tempAmpSt);

static void
Vp890FxoLoopCurrentMonitor(
    VpLineCtxType           *pLineCtx);

static void
Vp890FxoLowVoltageMonitor(
    VpLineCtxType           *pLineCtx);

static void
MakeLowVoltageCorrections(
    Vp890LineObjectType     *pLineObj,
    uint8                   *pIntReg);

static void
CidCorrectionLookup(
    uint16                  voltage,
    uint16                  *gainFactor,
    uint8                   *dtgVal);

static void
PllRecovery(
    VpLineCtxType           *pLineCtx);

/*******************************************************************************
 * Vp890ProcessFxoLine()
 * This function processes signaling information and generates events for an FXO
 * line.  This function deals only with persistent status signaling information,
 * so it only needs to be called once per tick.
 *
 * Arguments:   *pDevObj    - Device object ptr
 *              *pLineCtx   - Line context ptr
 *
 * Preconditions:   intReg in the device option must contain current signaling
 *                  register data.
 *
 * Postconditions:  FXO signaling data will be handled by generating events or
 *                  setting debounce/delay timers.
 ******************************************************************************/
void
Vp890ProcessFxoLine(
    Vp890DeviceObjectType   *pDevObj,
    VpLineCtxType           *pLineCtx)
{
    Vp890LineObjectType   *pLineObj   = pLineCtx->pLineObj;
    VpDeviceIdType        deviceId    = pDevObj->deviceId;
    uint8                 channelId   = pLineObj->channelId;
    uint8                 ecVal       = pLineObj->ecVal;

    VpFXOTimerType        *pFxoTimers = &pLineObj->lineTimers.timers.fxoTimer;
    bool                  onHookState = TRUE;
    uint8                 intReg      = pDevObj->intReg[channelId];
    uint8                 lsdData;
    VpCslacLineCondType   tempAmpSt, tempDiscSt, tempPolRevSt, tempLiuSt, tempPohSt;
    VpCslacLineCondType   tempAcFault, tempDcFault;

    if(pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_CLOSE
      || pLineObj->lineState.currentState == VP_LINE_FXO_TALK) {
        onHookState = FALSE;
    }

    /*
     * Ignore the detector for a period after the last hook state change, or
     * a longer period after the last hook state change AND if the previous
     * line condition was Ringing
     */
    if ((pFxoTimers->lastStateChange < VP890_FXO_ONHOOK_CHANGE_DEBOUNCE
            && onHookState == TRUE)
        || (pFxoTimers->lastStateChange < VP890_FXO_OFFHOOK_CHANGE_DEBOUNCE
            && onHookState == FALSE)
#ifdef VP_CSLAC_SEQ_EN
        || ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)
            && (pLineObj->intSequence[VP_PROFILE_TYPE_LSB]
                == VP_PRFWZ_PROFILE_MOMENTARY_LOOP_OPEN_INT))
#endif
        ) {
        /* Middle of detector Mask. Skip this process */
    } else {

        /* Ringing */
        if(onHookState){
            if (intReg & VP890_RING_DET_MASK) {
                tempAmpSt = VP_CSLAC_RING_AMP_DET;
            } else {
                tempAmpSt = 0;
            }
        } else {
            tempAmpSt = 0;
        }

        if ((pLineObj->lineState.condition & VP_CSLAC_RING_AMP_DET) != tempAmpSt) {
            Vp890AmpFreqRingDetect(pLineCtx, tempAmpSt);
        }
        /* --Ringing */

        /* Polrev */
        /* Ignore while ringing, disconnected, or shortly after a polrev because
           when ringing occurs, several polrevs are seen before ringing is detected. */
        if((pLineObj->lineState.condition & VP_CSLAC_RINGING) != VP_CSLAC_RINGING
        && (pLineObj->lineState.condition & VP_CSLAC_RAW_DISC) != VP_CSLAC_RAW_DISC
         && pFxoTimers->timeLastPolRev > VP890_FXO_POLREV_SILENCE) {
            if (intReg & VP890_POL_MASK) {
                tempPolRevSt = VP_CSLAC_POLREV;
            } else {
                tempPolRevSt = 0;
            }
            if ((pLineObj->lineState.condition & VP_CSLAC_POLREV) != tempPolRevSt) {
                VP_INFO(Vp890DeviceObjectType, pDevObj, ("Signaling Register 0x%02X 0x%02X Channel %d",
                    pDevObj->intReg[0], pDevObj->intReg[1], channelId));

                /*
                 * Workaround - If we see a change in polrev status, force the line
                 * side device to update, then read sigreg again to make sure
                 */
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN, &lsdData);
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_WRT, VP890_LSD_CTL_LEN, &lsdData);
                /* LSD forced to update at this point, reread signaling register */
                VpMpiCmdWrapper(deviceId, ecVal, VP890_NO_UL_SIGREG_RD,
                    VP890_NO_UL_SIGREG_LEN, pDevObj->intReg);
                intReg = pDevObj->intReg[channelId];
                if (intReg & VP890_POL_MASK) {
                    tempPolRevSt = VP_CSLAC_POLREV;
                } else {
                    tempPolRevSt = 0;
                }
            }

            if ((pLineObj->lineState.condition & VP_CSLAC_POLREV) != tempPolRevSt) {
                pLineObj->lineState.condition &= ~VP_CSLAC_POLREV;
                pLineObj->lineState.condition |= tempPolRevSt;
                pLineObj->lineEventHandle = pDevObj->timeStamp;
                pFxoTimers->timeLastPolRev = 0;
                pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                if (tempPolRevSt) {
                    pLineObj->fxoData = VP_POLREV_REVERSE;
                } else {
                    pLineObj->fxoData = VP_POLREV_NORMAL;
                }
            }
        }
        /* --Polrev */

        /* LIU */
        /* Implement the low voltage disconnect/LIU distinction workaround
         * adjustments.  This will modify the contents of intReg */
        if (onHookState && pLineObj->lowVoltageDetection.enabled == TRUE) {
            MakeLowVoltageCorrections(pLineObj, &intReg);
        }
        /* Ignore while ringing, suspecting ringing, or disconnected */
        if(((pLineObj->lineState.condition & VP_CSLAC_RAW_DISC) != VP_CSLAC_RAW_DISC)
         && (!(pLineObj->lineState.condition & VP_CSLAC_RINGING))
         && (onHookState)
         && (pFxoTimers->ringOffDebounce > VP890_FXO_RING_OFF_DEBOUNCE_LIU)) {
            /* Trigger LIU off of either the LIU bit or the LDN bit.  This is
             * done so that the API will match the behavior of the 880 API.
             * The 880 FXO always reports LIU when it reports disconnect. */
            if ((intReg & VP890_LIU_MASK) || (intReg & VP890_LDN_MASK)) {
                tempLiuSt = VP_CSLAC_LIU;
            } else {
                tempLiuSt = 0;
            }

            if ((pLineObj->lineState.condition & VP_CSLAC_LIU) != tempLiuSt) {
                if (tempLiuSt) {
                    if (pFxoTimers->liuDebounce > VP890_FXO_LIU_DEBOUNCE) {
                        /* Start LIU debounce timer if not running */
                        /* With some DC biased ringing, LIU can show up
                           before even the first polrev */
                        pFxoTimers->liuDebounce = 0;
                    }
                } else {
                    if (pFxoTimers->liuDebounce > VP890_FXO_LIU_DEBOUNCE) {
                        /* Use the same timer for LNIU to even out pulse
                           width.  ServiceFxoTimers determines which event
                           to generate when the timer expires */
                        pFxoTimers->liuDebounce = 0;
                    }
                }
            } else if (pFxoTimers->liuDebounce <= VP890_FXO_LIU_DEBOUNCE) {
                    /* If LIU debounce timer is running, stop it */
                    pFxoTimers->liuDebounce = 255;
            }
        }
        /* --LIU */

        /* Disconnect and Feed */
        /* Don't process if currently or recently ringing.  Zero bias ringing
           will give false LDN=1 indications due to integration, and a parallel
           phone going offhook during ringing can also cause LDN=1 */
        if((!(pLineObj->lineState.condition & VP_CSLAC_RINGING))
        && (pFxoTimers->ringOffDebounce > VP890_FXO_RING_OFF_DEBOUNCE_DISC)) {
            /* The AC_FLT flag indicates overcurrent or current starve.  Treat
             * those as disconnect. */
            if ((intReg & VP890_LDN_MASK) ||
                (pLineObj->lineState.condition & VP_CSLAC_AC_FLT))
            {
                tempDiscSt = VP_CSLAC_RAW_DISC;
            } else {
                tempDiscSt = 0;
            }

            if ((pLineObj->lineState.condition & VP_CSLAC_RAW_DISC) != tempDiscSt) {
                pLineObj->lineState.condition &= ~VP_CSLAC_RAW_DISC;
                pLineObj->lineState.condition |= tempDiscSt;

                pLineObj->preDisconnect = tempDiscSt;
                pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce =
                    MS_TO_TICKRATE(VP_FXO_DISCONNECT_DEBOUNCE,
                                    pDevObj->devProfileData.tickRate);

                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Disconnect Change to %d at time %d",
                    tempDiscSt, pDevObj->timeStamp));

                /* If onhook and not in disconnect, start CID correction */
                if(onHookState) {
                    if (!(tempDiscSt)) {
                        /* Start the timer for caller ID correction */
                        pFxoTimers->cidCorrectionTimer = 0;
                    }
                }
            }
        }
        /* --Disconnect and Feed */

        /* Overvoltage */
        if (onHookState == TRUE) {
            if (intReg & VP890_OVIR_MASK) {
                tempDcFault = VP_CSLAC_DC_FLT;
            } else {
                tempDcFault = 0;
            }
            if ((pLineObj->lineState.condition & VP_CSLAC_DC_FLT) != tempDcFault) {
                pLineObj->lineState.condition &= ~VP_CSLAC_DC_FLT;
                pLineObj->lineState.condition |= tempDcFault;
                pLineObj->lineEventHandle = pDevObj->timeStamp;
                pLineObj->lineEvents.faults |= VP_LINE_EVID_DC_FLT;
            }
        }
        /* --Overvoltage */

        /* Overcurrent/POH */
        if (onHookState == FALSE) {
            uint16 absCurrent[VP890_PCM_BUF_SIZE];
            uint8 bufferVal;
            bool overCurrent = TRUE;

            if (pLineObj->currentMonitor.stateValue == VP890_CURRENT_MONITOR_NORMAL
                && !(pLineObj->status & VP890_LINE_IN_CAL))
            {
                /* Initialize values such that first sample forces an update */
                int16 minValue = 32767;
                int16 maxValue = -32767;
                int32 deltaMaxMin = 0;
                int32 averageCurrent = 0;
                for ( bufferVal = 0; bufferVal < VP890_PCM_BUF_SIZE; bufferVal++) {
                    absCurrent[bufferVal] = (pLineObj->currentMonitor.currentBuffer[bufferVal] < 0) ?
                        -pLineObj->currentMonitor.currentBuffer[bufferVal] :
                        pLineObj->currentMonitor.currentBuffer[bufferVal];

                    if (absCurrent[bufferVal] < 1230) {
                        overCurrent = FALSE;
                    }
                    averageCurrent += pLineObj->currentMonitor.currentBuffer[bufferVal];

                    if (minValue >  pLineObj->currentMonitor.currentBuffer[bufferVal]) {
                        minValue =  pLineObj->currentMonitor.currentBuffer[bufferVal];
                    }
                    if (maxValue <  pLineObj->currentMonitor.currentBuffer[bufferVal]) {
                        maxValue =  pLineObj->currentMonitor.currentBuffer[bufferVal];
                    }
                }
                /* Overcurrent Algorithm */
                if (overCurrent == TRUE) {
                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Possible Overcurrent or Current Starve"));
                    tempAcFault = VP_CSLAC_AC_FLT;
                } else {
                    tempAcFault = 0;
                }

                /* POH Algorithm */
                deltaMaxMin = maxValue;
                deltaMaxMin -=  minValue;
                averageCurrent /= (int32)VP890_PCM_BUF_SIZE;
                if (deltaMaxMin > 30) {
                    /*
                     * This is a period where a line transient occurred. Cannot
                     * be evaluated for POH, but triggers when a POH might have
                     * occurred.
                     */
                    pLineObj->currentMonitor.invalidData = TRUE;
                } else {
                    /*
                     * This is a period where line is stable. But if recovering
                     * from being stable, evaluate for POH.
                     */
                    if (pLineObj->currentMonitor.invalidData == TRUE) {
                        pLineObj->currentMonitor.invalidData = FALSE;

                        if (((averageCurrent > 0) && (pLineObj->currentMonitor.steadyStateAverage > 0))
                         || ((averageCurrent < 0) && (pLineObj->currentMonitor.steadyStateAverage < 0))) {
                            int32 absOld = (pLineObj->currentMonitor.steadyStateAverage < 0) ?
                                            -pLineObj->currentMonitor.steadyStateAverage :
                                            pLineObj->currentMonitor.steadyStateAverage;
                            int32 absNew = (averageCurrent < 0) ?
                                            -averageCurrent :
                                            averageCurrent;
                            int32 deltaOldNew = absOld - absNew;
                            int32 absDeltaOldNew = (deltaOldNew < 0) ? -deltaOldNew : deltaOldNew;

                            if (absDeltaOldNew > 50) {
                                /* A phone did something .. but what? */
                                if (deltaOldNew > 0) {
                                    tempPohSt = VP_CSLAC_POH;
                                    pLineObj->lineState.condition |= VP_CSLAC_LIU;
                                } else {
                                    tempPohSt = 0;
                                    pLineObj->lineState.condition &= ~VP_CSLAC_LIU;
                                }

                                /* --POH/Not-POH */
                                if ((pLineObj->lineState.condition & VP_CSLAC_POH) != tempPohSt) {
                                    pLineObj->lineState.condition &= ~VP_CSLAC_POH;
                                    pLineObj->lineState.condition |= tempPohSt;
                                    pLineObj->lineEventHandle = pDevObj->timeStamp;

                                    if (tempPohSt == VP_CSLAC_POH) {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_POH;
                                    } else {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_PNOH;
                                    }
                                }
                            }
                        }
                    }
                    pLineObj->currentMonitor.steadyStateAverage = averageCurrent;
                }
            } else {
                tempAcFault    = pLineObj->lineState.condition & VP_CSLAC_AC_FLT;
            }

            if((pLineObj->lineState.condition & VP_CSLAC_AC_FLT) != tempAcFault) {
                pLineObj->lineState.condition &= ~(VP_CSLAC_AC_FLT);
                pLineObj->lineState.condition |= tempAcFault;
            }
        } else {
            /* Clear this flag if the FXO is back onhook */
            pLineObj->lineState.condition &= ~(VP_CSLAC_AC_FLT);
        }
        /* --Overcurrent/POH */

    }

#ifdef VP890_FXO_DELAYED_RING_TRIP
    /* If ringing has stopped and there was a state change requested during
       ringing, service it now */
    if ((pLineObj->fxoRingStateFlag == 1)
     && (!(pLineObj->lineState.condition & VP_CSLAC_RINGING))) {
        pLineObj->fxoRingStateFlag = 0;
        Vp890SetFxoLineState(pLineCtx, pLineObj->fxoRingState);
    }
#endif
} /* Vp890ProcessFxoLine */

/*******************************************************************************
 * Vp890AmpFreqRingDetect()
 * This function processes the device level ringing detector and generates
 * appropriate event. Called only by Process FXO.
 *
 * Arguments:       *pLineCtx   - Line context ptr
 *                  tempAmp     - Indication of current ringing status.
 *
 * Preconditions:   intReg in the device option must contain current signaling
 *                  register data.
 *
 * Postconditions:  FXO signaling data will be handled by generating events or
 *                  setting debounce/delay timers.
 ******************************************************************************/
void
Vp890AmpFreqRingDetect(
    VpLineCtxType *pLineCtx,
    VpCslacLineCondType tempAmpSt)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;

    VpFXOTimerType          *pFxoTimers = &pLineObj->lineTimers.timers.fxoTimer;

    pLineObj->lineEventHandle = pDevObj->timeStamp;

    if (tempAmpSt) {
        VP_INFO(VpLineCtxType, pLineCtx, ("Ringing Detect at Time %d", pDevObj->timeStamp));
        pLineObj->lineState.condition |= VP_CSLAC_RING_AMP_DET;

        pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_ON;
        pLineObj->lineState.condition |= VP_CSLAC_RINGING;

        if (pFxoTimers->liuDebounce <= VP890_FXO_LIU_DEBOUNCE) {
            /* If LIU debounce timer is running, stop it */
            pFxoTimers->liuDebounce = 255;
        }

        /* Going from disconnect to 0-bias ringing, we won't see a change in
           LDN, but if we see ringing, there must be feed. */
        if (pLineObj->lineState.condition & VP_CSLAC_DISC) {
            pLineObj->lineEvents.fxo |= VP_LINE_EVID_FEED_EN;
            pLineObj->lineState.condition &= ~VP_CSLAC_DISC;
            pLineObj->lineEventHandle = pDevObj->timeStamp;
            /* When we get a feed enable event, start the timer for
             * caller ID correction */
            pFxoTimers->cidCorrectionTimer = 0;
        }

        /* Stop the disconnect timer and clear any raw disconnect indication */
        pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce = 0;
        pLineObj->lineState.condition &= ~VP_CSLAC_RAW_DISC;

    } else {
        VP_INFO(Vp890DeviceObjectType, pDevObj, ("Ringing Remove at Time %d", pDevObj->timeStamp));
        pLineObj->lineState.condition &= ~VP_CSLAC_RING_AMP_DET;

        pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_OFF;
        pLineObj->lineState.condition &= ~VP_CSLAC_RINGING;
        pFxoTimers->ringOffDebounce = 0;
    }
}

/*******************************************************************************
 * Vp890ServiceFxoTimers()
 * This function services FXO-specific line timers.
 *
 * Arguments:   *pDevCtx    -   Device context ptr
 *              *pLineCtx   -   Line context ptr
 *              *pLineObj   -   Line object ptr
 *              deviceId    -   User-defined deviceId
 *              ecVal       -   Enable Channel value including wideband info
 *
 * Preconditions:  Sould be called once per tick by Vp890ApiTick.
 *
 * Postconditions: FXO line timers will be serviced.
 ******************************************************************************/
void
Vp890ServiceFxoTimers(
    VpDevCtxType            *pDevCtx,
    VpLineCtxType           *pLineCtx,
    Vp890LineObjectType     *pLineObj,
    VpDeviceIdType          deviceId,
    uint8                   ecVal)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpFXOTimerType          *pFxoTimers = &pLineObj->lineTimers.timers.fxoTimer;
    uint8                   stateByte;

    /* Set tick adder for 1ms increments */
    uint16 tickAdder = pDevObj->devProfileData.tickRate / (VP_CSLAC_TICKSTEP_0_5MS * 2);

    /* Increment the time since polrev was observed */
    if (pFxoTimers->timeLastPolRev < (0x7FFF - tickAdder)) {
        /*
         * The time is in 1mS increments, but the device tickrate is
         * something else. Increment by the scaled amount.
         */
        pFxoTimers->timeLastPolRev += tickAdder;
    } else {
        /* Max limit the value of last polrev value */
        pFxoTimers->timeLastPolRev = 0x7FFF;
    }

    if (!(pLineObj->status & VP890_LINE_IN_CAL)) {
        if ((pFxoTimers->lastStateChange + tickAdder) >= (0x7FFF - tickAdder)) {
            pFxoTimers->lastStateChange = 0x7FFF;
        } else {
            pFxoTimers->lastStateChange += tickAdder;
            /*
             * If we tried to set the line state to loop closed, but it automatically
             *  got kicked back to loop open, set it back to loop closed
             */
            if ((pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_CLOSE
                 || pLineObj->lineState.currentState == VP_LINE_FXO_TALK)
                && pFxoTimers->lastStateChange < (VP890_FXO_OFFHOOK_CHANGE_DEBOUNCE
                 + tickAdder))
            {
                VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_RD, VP890_SYS_STATE_LEN, &stateByte);

                if ((stateByte & VP890_SS_FXO_STATE_MASK) != VP890_SS_FXO_OFFHOOK) {
                    /* We think we're offhook, but we really got reset to onhook.
                       Set state back to what we want it to be */
                    Vp890SetFxoLineState(pLineCtx, pLineObj->lineState.currentState);
                }
            }
        }
    }

    if (((uint16)pFxoTimers->ringOffDebounce + tickAdder) > 255) {
        pFxoTimers->ringOffDebounce = 255;
    } else {
        pFxoTimers->ringOffDebounce += tickAdder;
    }

    /* Disconnect Debounce timer */
    if (pFxoTimers->ringOffDebounce > VP890_FXO_RING_OFF_DEBOUNCE_DISC) {
        if (pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce >= 1) {
            pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce -= 1;

            if (pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce == 0) {

                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Disconnect debounce timer expired at ts %d PreCond 0x%04X Post 0x%04X",
                    pDevObj->timeStamp, pLineObj->preDisconnect,
                    (pLineObj->lineState.condition & VP_CSLAC_RAW_DISC)));

                if (pLineObj->preDisconnect ==
                    (pLineObj->lineState.condition & VP_CSLAC_RAW_DISC)) {
                    /*
                     * There is a change that persisted longer than the
                     * debounce interval. Evaluate and generate event
                     */
                    pLineObj->lineEventHandle = pDevObj->timeStamp;

                    switch(pLineObj->lineState.usrCurrent) {
                        case VP_LINE_FXO_TALK:
                        case VP_LINE_FXO_LOOP_CLOSE:
                            if (pLineObj->preDisconnect == VP_CSLAC_RAW_DISC) {
                                if (!(pLineObj->lineState.condition & VP_CSLAC_DISC)) {
                                    pLineObj->lineState.condition |= VP_CSLAC_DISC;
                                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_DISCONNECT;
                                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Starting Disconnect Duration with %d at time %d",
                                        pLineObj->lineTimers.timers.fxoTimer.disconnectDuration, pDevObj->timeStamp));
                                }
                                pLineObj->lineTimers.timers.fxoTimer.disconnectDuration =
                                    MS_TO_TICKRATE(VP_FXO_DISC_TO_LOOP_OPEN,
                                        pDevObj->devProfileData.tickRate);
                            } else {
                                if (pLineObj->lineState.condition & VP_CSLAC_DISC) {
                                    pLineObj->lineState.condition &= ~VP_CSLAC_DISC;
                                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_RECONNECT;
                                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Clearing Disconnect Duration at time %d",
                                        pDevObj->timeStamp));
                                }
                                pLineObj->lineTimers.timers.fxoTimer.disconnectDuration = 0;
                            }
                            break;

                        default:
                            if (pLineObj->preDisconnect == VP_CSLAC_RAW_DISC) {
                                if (!(pLineObj->lineState.condition & VP_CSLAC_DISC)) {
                                    pLineObj->lineState.condition |= VP_CSLAC_DISC;
                                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_FEED_DIS;
                                }
                                pLineObj->lineTimers.timers.fxoTimer.disconnectDuration =
                                    MS_TO_TICKRATE(VP_FXO_DISC_TO_LOOP_OPEN,
                                        pDevObj->devProfileData.tickRate);
                                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Starting Disconnect Duration with %d at time %d",
                                    pLineObj->lineTimers.timers.fxoTimer.disconnectDuration, pDevObj->timeStamp));
                            } else {
                                if (pLineObj->lineState.condition & VP_CSLAC_DISC) {
                                    pLineObj->lineState.condition &= ~VP_CSLAC_DISC;
                                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_FEED_EN;
                                }
                                pLineObj->lineTimers.timers.fxoTimer.disconnectDuration = 0;
                                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Clearing Disconnect Duration at time %d",
                                    pDevObj->timeStamp));
                            }
                            break;
                    }
                }
            }
        }
    }

    /* Line In Use debounce timer */
    if (((uint16)pFxoTimers->liuDebounce + tickAdder) > 255) {
        pFxoTimers->liuDebounce = 255;
    } else {
        pFxoTimers->liuDebounce += tickAdder;
        if (pFxoTimers->liuDebounce >= VP890_FXO_LIU_DEBOUNCE
            && pFxoTimers->liuDebounce < (VP890_FXO_LIU_DEBOUNCE + tickAdder)) {
            /* If this timer expires, we know that the LIU state needs to be
               changed to the opposite of what it currently is, so we use the
               current condition to determine which event we generate. */
            if (pLineObj->lineState.condition & VP_CSLAC_LIU) {
                /* Report the event as long as we're not in disconnect */
                if ((pLineObj->lineState.condition & VP_CSLAC_RAW_DISC)
                    != VP_CSLAC_RAW_DISC) {
                    pLineObj->lineState.condition &= ~VP_CSLAC_LIU;
                    pLineObj->lineEventHandle = pDevObj->timeStamp;
                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_LNIU;
                }
            } else {
                /* OK to report this regardless of disconnect */
                pLineObj->lineState.condition |= VP_CSLAC_LIU;
                pLineObj->lineEventHandle = pDevObj->timeStamp;
                pLineObj->lineEvents.fxo |= VP_LINE_EVID_LIU;
            }
        }
    }

    /* Ringing detection timer */
    /* If this timer expires, ringing has ended. */
    if (((uint16)pFxoTimers->ringTimer + tickAdder) > 255) {
        pFxoTimers->ringTimer = 255;
    } else {
        pFxoTimers->ringTimer += tickAdder;
        if ((pFxoTimers->ringTimer >= VP890_FXO_RING_MAX)
         && (pFxoTimers->ringTimer < (VP890_FXO_RING_MAX + tickAdder))) {
            VP_INFO(VpLineCtxType, pLineCtx, ("Ring Timer %d Expires At %d", pFxoTimers->ringTimer, VP890_FXO_RING_MAX));
            if (pLineObj->lineState.condition & VP_CSLAC_RINGING) {
                pLineObj->lineEventHandle = pDevObj->timeStamp;
                pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_OFF;
                pLineObj->lineState.condition &= ~VP_CSLAC_RINGING;
                pFxoTimers->ringOffDebounce = 0;
            }
        }
    }

    /* Caller ID Correction Timer */
    if (((uint16)pFxoTimers->cidCorrectionTimer + tickAdder) > 255) {
        pFxoTimers->cidCorrectionTimer = 255;
    } else {
        pFxoTimers->cidCorrectionTimer += tickAdder;

        /* When the timer reaches the CID correction starting time, check some
         * conditions then set the converter to start measuring avg voltage */
        if (pFxoTimers->cidCorrectionTimer >= VP890_FXO_CID_CORRECTION_START
            && pFxoTimers->cidCorrectionTimer < (VP890_FXO_CID_CORRECTION_START + tickAdder))
        {
            /* Have to make sure the channel is not under test, is void of
             * detections, and is in an onhook state. */
            if (!Vp890IsChnlUndrTst(pDevObj, pLineObj->channelId)
                && Vp890IsDevReady(pDevObj->state, TRUE)
                && (pLineObj->lineState.condition & (VP_CSLAC_RINGING
                                                    | VP_CSLAC_LIU
                                                    | VP_CSLAC_RAW_DISC
                                                    | VP_CSLAC_RING_AMP_DET)) == 0
                && (pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_OPEN
                 || pLineObj->lineState.currentState == VP_LINE_FXO_OHT))
            {
                /* Start measuring average line voltage */
                uint8 convCfg;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD,
                    VP890_CONV_CFG_LEN, &convCfg);

                convCfg &= ~VP890_CONV_CONNECT_BITS;
                convCfg |= VP890_AVG_ONHOOK_V;

                VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT,
                    VP890_CONV_CFG_LEN, &convCfg);
                pLineObj->cidCorrectionCtr = 0;
            } else {
                /* We can't take the measurement now, so restart the process */
                pFxoTimers->cidCorrectionTimer = 0;
            }
        }
        /* When the timer reaches the CID correction ending time, check the
         * conditions again, then read the avg voltage result and set the gain */
        if (pFxoTimers->cidCorrectionTimer >= VP890_FXO_CID_CORRECTION_END
            && pFxoTimers->cidCorrectionTimer < (VP890_FXO_CID_CORRECTION_END + tickAdder))
        {
            /* Have to make sure the channel is not under test, is void of
             * detections, and is in an onhook state. */
            if (!Vp890IsChnlUndrTst(pDevObj, pLineObj->channelId)
                && Vp890IsDevReady(pDevObj->state, TRUE)
                && (pLineObj->lineState.condition & (VP_CSLAC_RINGING
                                                    | VP_CSLAC_LIU
                                                    | VP_CSLAC_RAW_DISC
                                                    | VP_CSLAC_RING_AMP_DET)) == 0
                && (pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_OPEN
                    || pLineObj->lineState.currentState == VP_LINE_FXO_OHT))
            {
                /* Read measurement */
                uint8 data[VP890_TX_PCM_DATA_LEN];
                int8 sample;
                uint16 voltage;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD,
                        VP890_TX_PCM_DATA_LEN, data);
                /* Measurement data is 7 bits, so we have to sign extend it */
                sample = (data[1] & VP890_FXO_LINE_V_MASK) |
                        ((data[1] & VP890_FXO_LINE_V_SIGN_BIT) << 1);

                /* We use cidCorrectionCtr and cidCorrectionSample to make sure
                 * the line voltage is stable and that we don't make our adjustment
                 * based on a reading that includes some kind of glitch on the line.
                 * We look for 3 successive unchanging samples before making the
                 * correction calculation.
                 */
                if (sample == pLineObj->cidCorrectionSample) {
                    pLineObj->cidCorrectionCtr++;
                } else {
                    pLineObj->cidCorrectionCtr = 1;
                }
                pLineObj->cidCorrectionSample = sample;
                if (pLineObj->cidCorrectionCtr == 3) {
                    /* Measurement is signed and in units of 1.28 volts.  We need an
                     * absolute value in a format where 32 = 1.28V and 25 = 1V */
                    voltage = 32 * (sample < 0 ? -(sample) : sample);
                    CidCorrectionLookup(voltage, &pLineObj->gxCidLevel, &pLineObj->cidDtg);
                    VP_INFO(VpLineCtxType, pLineCtx, ("CallerID Correction - Voltage: %d (%dV)  GX factor: %d  DTG: %d",
                        voltage, voltage/25, pLineObj->gxCidLevel, pLineObj->cidDtg));
                    Vp890SetRelGainInt(pLineCtx);
                } else {
                    /* The timing here COULD skip a sample, but we don't care */
                    pFxoTimers->cidCorrectionTimer = VP890_FXO_CID_CORRECTION_START;
                }
            } else {
                /* We can't take the measurement now, so restart the process */
                pFxoTimers->cidCorrectionTimer = 0;
            }
        }
    }

    /* B Filter Calibration Timer */
    /* When this timer expires, call Vp890CalBFilterInt */
    if (((uint16)pFxoTimers->bCalTimer + tickAdder) > 255) {
        pFxoTimers->bCalTimer = 255;
    } else {
        if ((pFxoTimers->bCalTimer >= VP890_BFILT_SAMPLE_TIME)
         && (pFxoTimers->bCalTimer < (VP890_BFILT_SAMPLE_TIME + tickAdder))) {
            if (pLineObj->status & VP890_LINE_IN_CAL) {
                Vp890CalBFilterInt(pLineCtx);
            }
        }
        pFxoTimers->bCalTimer += tickAdder;
    }

    /* B Filter Measurement Timer */
    /* When this timer expires, call Vp890CalMeasureBFilterInt */
    if (((uint16)pFxoTimers->measureBFilterTimer + tickAdder) > 255) {
        pFxoTimers->measureBFilterTimer = 255;
    } else {
        if ((pFxoTimers->measureBFilterTimer >= VP890_BFILT_SAMPLE_TIME)
         && (pFxoTimers->measureBFilterTimer < (VP890_BFILT_SAMPLE_TIME + tickAdder))) {
            if (pLineObj->status & VP890_LINE_IN_CAL) {
                Vp890CalMeasureBFilterInt(pLineCtx);
            }
        }
        pFxoTimers->measureBFilterTimer += tickAdder;
    }

    if (pFxoTimers->pllRecovery != 0) {
        pFxoTimers->pllRecovery--;
        if (pFxoTimers->pllRecovery == 0) {
            PllRecovery(pLineCtx);
        }
    }

    if (!(pLineObj->status & VP890_LINE_IN_CAL)) {
        /* Current Monitor Timer already set in "ticks" */
        if (pFxoTimers->currentMonitorTimer != 0) {
            pFxoTimers->currentMonitorTimer--;

            if (pFxoTimers->currentMonitorTimer == 0) {
                Vp890FxoLoopCurrentMonitor(pLineCtx);
            }
        }
    }

    if (pLineObj->lineTimers.timers.fxoTimer.disconnectDuration > 0) {
        pLineObj->lineTimers.timers.fxoTimer.disconnectDuration--;
        if (pLineObj->lineTimers.timers.fxoTimer.disconnectDuration == 0) {
            if ((pLineObj->lineState.usrCurrent == VP_LINE_FXO_TALK)
             || (pLineObj->lineState.usrCurrent == VP_LINE_FXO_LOOP_CLOSE)) {

                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Disconnect Duration timeout time %d",
                    pDevObj->timeStamp));

                Vp890SetLineState(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
            }
        }
    }

    /* Workaround for low voltage disconnect/LIU distinction */
    if (!(pLineObj->status & VP890_LINE_IN_CAL) &&
          pLineObj->lowVoltageDetection.enabled == TRUE) {
        /* Timer already set in "ticks" */
        if (pFxoTimers->lowVoltageTimer != 0) {
            pFxoTimers->lowVoltageTimer--;

            if (pFxoTimers->lowVoltageTimer == 0) {
                Vp890FxoLowVoltageMonitor(pLineCtx);
            }
        }
    }

    return;
} /* Vp890ServiceFxoTimers() */

/*******************************************************************************
 * Vp890FxoLoopCurrentMonitor()
 *  This function measures the current on the FXO line while in loop close and
 * detecting line current.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Line object is updated with buffer containing loop current values.
 ******************************************************************************/
static void
Vp890FxoLoopCurrentMonitor(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    uint16                  pcmData;
    uint8                   pcmRegister[VP890_TX_PCM_DATA_LEN];
    uint8                   daaCtrl[VP890_LSD_CTL_LEN];
    uint8                   ecVal       = pLineObj->ecVal;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

#undef FXO_LOOP_MONITOR_DEBUG

#ifdef FXO_LOOP_MONITOR_DEBUG
    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Vp890FXOLoopCurrentMonitor() - state %d at %d (0x%04X)",
        pLineObj->currentMonitor.stateValue, pDevObj->timeStamp, pDevObj->timeStamp));
#endif

    VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN,
        pcmRegister);

#ifdef FXO_LOOP_MONITOR_DEBUG
    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("PCM Data 0x%02X 0x%02X", pcmRegister[0], pcmRegister[1]));
#endif

    pcmData = pcmRegister[0];
    pcmData = (pcmData << 8) & 0xFF00;
    pcmData |= pcmRegister[1];

    switch(pLineObj->currentMonitor.stateValue) {
        case VP890_CURRENT_MONITOR_DISABLED:
            pLineObj->currentMonitor.currentBuffer[0] = (int16)pcmData;

            pLineObj->currentMonitor.offsetMeasurements = 1;

            pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                MS_TO_TICKRATE(140, pDevObj->devProfileData.tickRate);

            pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_OFFSET;
            break;

        case VP890_CURRENT_MONITOR_OFFSET: {
            uint8 i;
            int16 min;
            int16 max;

            /* Insert new sample */
            for (i = (VP890_CM_OFFSET_NUM - 1); i > 0; i--) {
                pLineObj->currentMonitor.currentBuffer[i] =
                    pLineObj->currentMonitor.currentBuffer[i - 1];
            }
            pLineObj->currentMonitor.currentBuffer[0] = (int16)pcmData;

            pLineObj->currentMonitor.offsetMeasurements++;

            /* Need to fill up the samples before checking anything */
            if (pLineObj->currentMonitor.offsetMeasurements < VP890_CM_OFFSET_NUM) {
                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                    MS_TO_TICKRATE(140, pDevObj->devProfileData.tickRate);

                pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_OFFSET;
                break;
            }

            /* Find the min/max of the samples */
            min = pLineObj->currentMonitor.currentBuffer[0];
            max = pLineObj->currentMonitor.currentBuffer[0];
            for(i = 1; i < VP890_CM_OFFSET_NUM; i++) {
                if (pLineObj->currentMonitor.currentBuffer[i] < min) {
                    min = pLineObj->currentMonitor.currentBuffer[i];
                }
                if (pLineObj->currentMonitor.currentBuffer[i] > max) {
                    max = pLineObj->currentMonitor.currentBuffer[i];
                }
            }

            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Current monitor offset min %d max %d", min, max));

            /* If the range of variation is within our tolerance, use the
             * average as the offset and move on */
            if (max - min < VP890_CM_OFFSET_TOLERANCE &&
                max - min > -VP890_CM_OFFSET_TOLERANCE)
            {
                int32 sum = 0;
                for (i = 0; i < VP890_CM_OFFSET_NUM; i++) {
                    sum += pLineObj->currentMonitor.currentBuffer[i];
                }
                pLineObj->currentMonitor.currentOffset = sum / VP890_CM_OFFSET_NUM;

                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Current monitor offset %04hX", pLineObj->currentMonitor.currentOffset));

                /* Re-Enable the Current Sense Path */
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN, daaCtrl);
                daaCtrl[0] &= ~VP890_LSD_CSE_DIS;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_WRT, VP890_LSD_CTL_LEN, daaCtrl);

                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                    MS_TO_TICKRATE(400, pDevObj->devProfileData.tickRate);

                pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_BUFFER0;
                break;
            }

            /* Give up and use the latest sample for the offset if we've
             * exceeded the offset sample limit */
            if (pLineObj->currentMonitor.offsetMeasurements > VP890_CM_OFFSET_NUM_LIMIT) {

                pLineObj->currentMonitor.currentOffset = (int16)pcmData;

                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Current monitor offset didn't converge, using %04hX", pLineObj->currentMonitor.currentOffset));

                /* Re-Enable the Current Sense Path */
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN, daaCtrl);
                daaCtrl[0] &= ~VP890_LSD_CSE_DIS;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_WRT, VP890_LSD_CTL_LEN, daaCtrl);

                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                    MS_TO_TICKRATE(400, pDevObj->devProfileData.tickRate);

                pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_BUFFER0;
                break;
            }

            /* Otherwise, continue offset measurements */
            pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                MS_TO_TICKRATE(140, pDevObj->devProfileData.tickRate);

            pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_OFFSET;
        } break;

        case VP890_CURRENT_MONITOR_BUFFER0:
        case VP890_CURRENT_MONITOR_BUFFER1:
        case VP890_CURRENT_MONITOR_BUFFER2:
        case VP890_CURRENT_MONITOR_BUFFER3:
        case VP890_CURRENT_MONITOR_BUFFER4: {
                uint8 bufferIndex = pLineObj->currentMonitor.stateValue - VP890_CURRENT_MONITOR_BUFFER0;
                pLineObj->currentMonitor.currentBuffer[bufferIndex] = (int16)pcmData;
                pLineObj->currentMonitor.currentBuffer[bufferIndex] -=
                    pLineObj->currentMonitor.currentOffset;

                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                    MS_TO_TICKRATE(140, pDevObj->devProfileData.tickRate);

                pLineObj->currentMonitor.stateValue++;
        } break;

        case VP890_CURRENT_MONITOR_NORMAL: {
                uint8 buffCnt;

#ifdef FXO_LOOP_MONITOR_DEBUG
                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Vp890FXOLoopCurrentMonitor() - offSet %d",
                    pLineObj->currentMonitor.currentOffset));
#endif

                for (buffCnt = (VP890_PCM_BUF_SIZE - 1); buffCnt > 0; buffCnt--) {
                    pLineObj->currentMonitor.currentBuffer[buffCnt] =
                        pLineObj->currentMonitor.currentBuffer[buffCnt - 1];

#ifdef FXO_LOOP_MONITOR_DEBUG
                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Vp890FXOLoopCurrentMonitor() - Buffer %d, Value %d",
                        buffCnt, pLineObj->currentMonitor.currentBuffer[buffCnt]));
#endif
                }

                pLineObj->currentMonitor.currentBuffer[0] = (int16)pcmData;
                pLineObj->currentMonitor.currentBuffer[0] -=
                    pLineObj->currentMonitor.currentOffset;

#ifdef FXO_LOOP_MONITOR_DEBUG
                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Vp890FXOLoopCurrentMonitor() - Buffer 0, Value %d",
                    pLineObj->currentMonitor.currentBuffer[0]));
#endif

                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                    MS_TO_TICKRATE(140, pDevObj->devProfileData.tickRate);
            }
            break;
        default:
            break;
    }

    return;
}

/*******************************************************************************
 * Vp890FxoLowVoltageMonitor()
 *  This function measures the voltage on the FXO line while in loop open and
 * gathers information used to adjust the LIU and disconnect detection
 *
 * Preconditions:
 *  This function assumes that the converter config is set to measure
 * average on hook line voltage.  We never set it to anything else while
 * onhook, so this should always be true.
 *
 * Postconditions:
 *  Line object is updated with information for making the LIU and disconnect
 * adjustments.
 ******************************************************************************/
static void
Vp890FxoLowVoltageMonitor(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    uint8                   ecVal       = pLineObj->ecVal;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    uint8 data[VP890_TX_PCM_DATA_LEN];

    VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD,
            VP890_TX_PCM_DATA_LEN, data);

    if (data[1] == 0) {
        /* Incremement the count of samples indicating true disconnect, and
         * stop counting at the limit. */
        if (pLineObj->lowVoltageDetection.numDisc < VP890_FXO_LOW_VOLTAGE_DISC_COUNT) {
            pLineObj->lowVoltageDetection.numDisc++;
        }

        /* Clear out any count of samples of not-disconnect */
        pLineObj->lowVoltageDetection.numNotDisc = 0;

    } else {
        /* If we're currently indicating disconnect, count the number of samples
         * that indicate NOT disconnect. */
        if (pLineObj->lowVoltageDetection.numDisc >= VP890_FXO_LOW_VOLTAGE_DISC_COUNT) {
            pLineObj->lowVoltageDetection.numNotDisc++;

            /* If this count reaches its limit, clear the disconnect count.
             * Also reset this count because it is only used when the disconnect
             * count is full. */
            if (pLineObj->lowVoltageDetection.numNotDisc >= VP890_FXO_LOW_VOLTAGE_NOTDISC_COUNT) {
                pLineObj->lowVoltageDetection.numDisc = 0;
                pLineObj->lowVoltageDetection.numNotDisc = 0;
                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Cleared true disconnect"));
            }
        }
    }

    /* Set the timer for the next measurement. */
    pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer =
        MS_TO_TICKRATE(VP890_FXO_LOW_VOLTAGE_TIMER_LEN, pDevObj->devProfileData.tickRate);

}

/*******************************************************************************
 * MakeLowVoltageCorrections()
 *  This function makes corrections to the signaling register buffer based on
 * voltage readings made by Vp890FxoLowVoltageMonitor().
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  The LIU and LDN bits in 'intReg' are adjusted based on voltage measurements.
 ******************************************************************************/
static void
MakeLowVoltageCorrections(
    Vp890LineObjectType *pLineObj,
    uint8 *pIntReg)
{
    if ((*pIntReg & VP890_LDN_MASK) &&
        (pLineObj->lowVoltageDetection.numDisc < VP890_FXO_LOW_VOLTAGE_DISC_COUNT))
    {
        /* The device is indicating LDN, but the voltage measurements have not
         * confirmed a zero voltage.  Instead of 0, we must be seeing some small
         * voltage, so change the bits to indicate LIU instead of disconnect. */
        *pIntReg &= ~VP890_LDN_MASK;
        *pIntReg |= VP890_LIU_MASK;
    }
}

/**
 * Vp890IsSupportedFxoState()
 *  This function checks to see if the state passed is a supproted FXO state of
 * the 890 API
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None.
 */
bool
Vp890IsSupportedFxoState(
    VpLineStateType state)
{
    switch (state) {
        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_TALK:
            return TRUE;

        default:
            return FALSE;
    }
}

/*******************************************************************************
 * SetFxoLineState()
 *  This function sets the line state for a given channel of a given device. The
 * valid line states are defined in the VpLineState type.
 *
 * Preconditions:
 *  The line must first be initialized prior to setting the line state.  The
 * state must be a valid line state as defined in the VpLineState type.
 *
 * Postconditions:
 *  Returns success code if the channel can be set and the line state is valid
 * for the type of line specified by the line context.  If successfull, the line
 * specified by the line context is set to the line state specified.
 ******************************************************************************/
VpStatusType
Vp890SetFxoLineState(
    VpLineCtxType           *pLineCtx,
    VpLineStateType         state)
{
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;

    VpStatusType            status;
    uint8                   chanId    = pLineObj->channelId;
    uint8                   ecVal     = pLineObj->ecVal;
    uint8                   stateByte, stateBytePre;
    uint8                   opCondData, opCondByte, gainData, opFuncData;
    uint8                   intData[VP890_INT_MASK_LEN];
    uint8                   daaCtrl[VP890_LSD_CTL_LEN];

   /* Note that the System State Register may be programmed twice. */
    uint8 mpiBuffer[6 + VP890_OP_COND_LEN + VP890_VP_GAIN_LEN + VP890_OP_FUNC_LEN +
                    VP890_SYS_STATE_LEN + VP890_SYS_STATE_LEN + VP890_INT_MASK_LEN];
    uint8 mpiIndex  = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890SetFxoLineState()"));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_INT_MASK_RD, VP890_INT_MASK_LEN, intData);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_RD, VP890_SYS_STATE_LEN, &stateByte);
    stateBytePre = stateByte;
    stateByte &= ~(VP890_SS_FXO_STATE_MASK | VP890_SS_SPE_MASK);

    /* If the line state is set to OHT, we must enable CID gain correction and
     * disable the programmed X filter */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN, &gainData);
    gainData &= ~VP890_CIDCOR_MASK;
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN, &opFuncData);
    opFuncData |= VP890_ENABLE_X;

    /*
     * Read the status of the Operating Conditions register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     * This also performs the line type/state verification.
     */
    opCondData = pLineObj->opCond[0];

    opCondData &= ~(VP890_CUT_TXPATH | VP890_CUT_RXPATH);
    opCondData &= ~(VP890_HIGH_PASS_DIS | VP890_OPCOND_RSVD_MASK);
    status = Vp890GetTxRxPcmMode(pLineObj, state, &opCondByte);
    if (status == VP_STATUS_SUCCESS) {
        opCondData |= opCondByte;
    } else {
        return status;
    }

    /* Does the CODEC have to be in Active for simple on/off-hook? */
    switch(state) {
        case VP_LINE_FXO_OHT:
            stateByte |= VP890_SS_SPE_MASK;
            gainData |= VP890_CIDCOR_MASK;
            opFuncData &= ~VP890_ENABLE_X;
        case VP_LINE_FXO_LOOP_OPEN:
            /* The LIU and RING_DET interrupt masks share the same bits as POH
             * and POH_SIGN.  They will be masked when the FXO goes offhook, so
             * we need to restore them for LIU and RING events when onhook. */
            intData[chanId] &= ~VP890_LIU_MASK;
            intData[chanId] &= ~VP890_RING_DET_MASK;
            if ( (pLineObj->lineEventsMask.fxo & VP_LINE_EVID_LIU) &&
                 (pLineObj->lineEventsMask.fxo & VP_LINE_EVID_LNIU) ) {
                intData[chanId] |= VP890_LIU_MASK;
            }
            if ( (pLineObj->lineEventsMask.fxo & VP_LINE_EVID_RING_ON) &&
                 (pLineObj->lineEventsMask.fxo & VP_LINE_EVID_RING_OFF) ) {
                intData[chanId] |= VP890_RING_DET_MASK;
            }

            stateByte |= VP890_SS_FXO_ONHOOK;
            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Vp890SetFxoLineState() - Onhook at %d (0x%04X)",
                pDevObj->timeStamp, pDevObj->timeStamp));
            /*-----------------7/24/2008 6:18PM-----------------
             * Disable the current monitor and restore DAA configuration if
             * on-hook occurs in the middle of calibration phase.
             * --------------------------------------------------*/
            pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer = 0;
            if (pLineObj->currentMonitor.stateValue == VP890_CURRENT_MONITOR_OFFSET ||
                pLineObj->currentMonitor.stateValue == VP890_CURRENT_MONITOR_DISABLED)
            {
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN, daaCtrl);
                daaCtrl[0] &= ~VP890_LSD_CSE_DIS;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_WRT, VP890_LSD_CTL_LEN, daaCtrl);
            }
            pLineObj->currentMonitor.stateValue = VP890_CURRENT_MONITOR_DISABLED;

            /* Turn on the low voltage monitor if needed */
            if (pLineObj->lowVoltageDetection.enabled == TRUE) {
                pLineObj->lowVoltageDetection.numDisc = 0;
                pLineObj->lowVoltageDetection.numNotDisc = 0;
                pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer =
                    MS_TO_TICKRATE(VP890_FXO_LOW_VOLTAGE_TIMER_LEN, pDevObj->devProfileData.tickRate);
                VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Enable low voltage timer %d\n",
                    pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer));
            }
            break;

        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_TALK:
            /* Mask the POH and POH_SIGN bits because we don't use them, and
             * they can toggle frequently, causing excessive interrupts */
            intData[chanId] |= VP890_POH_SIGN_MASK;
            intData[chanId] |= VP890_POH_DET_MASK;

            stateByte |= VP890_SS_FXO_OFFHOOK;

            /*-----------------7/24/2008 6:23PM-----------------
             * Start calibration phase of current monitor.
             * --------------------------------------------------*/
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN, daaCtrl);
            daaCtrl[0] |= VP890_LSD_CSE_DIS;
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LSD_CTL_WRT, VP890_LSD_CTL_LEN, daaCtrl);

            pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer =
                MS_TO_TICKRATE(500, pDevObj->devProfileData.tickRate);
            pLineObj->currentMonitor.currentOffset = 0;

            /* Turn off the low voltage monitor */
            pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer = 0;

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Vp890SetFxoLineState() - Offhook at %d (0x%04X), timer %d",
                pDevObj->timeStamp, pDevObj->timeStamp, pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer));
         break;

        case VP_LINE_FXO_RING_GND:
        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetFxoLineState() - Invalid FXO State"));
            return VP_STATUS_INVALID_ARG;
    }
    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("14. Writing 0x%02X to Operating Conditions",
        opCondData));
    pLineObj->opCond[0] = opCondData;
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, pLineObj->opCond);

	/*BEGIN: fix MOTO*/
	if (13 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
        VP890_VP_GAIN_LEN, &gainData);

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("FXO Set Line State Operating Functions 0x%02X",
        opFuncData));

	/*BEGIN: fix MOTO*/
	if (13 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, &opFuncData);

	/*BEGIN: fix MOTO*/
	if (13 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    /* Start the lastStateChange timer and set the line state */
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
        VP890_SYS_STATE_LEN, &stateByte);

    if (state == VP_LINE_FXO_OHT) {
        /* For OHT, the SS register needs to be written twice to force SPE=1.
           The device state machine forces SPE=0 when it transitions between
           major states, so writing this a second time will override that. */

		/*BEGIN: fix MOTO*/
		if (13 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
            VP890_SYS_STATE_LEN, &stateByte);

    }
    pLineObj->slicValueCache = stateByte;

	/*BEGIN: fix MOTO*/
	if (13 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_INT_MASK_WRT,
        VP890_INT_MASK_LEN, intData);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
        mpiIndex-1, &mpiBuffer[1]);

    pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 0;

    pLineObj->lineState.previous = pLineObj->lineState.currentState;
    pLineObj->lineState.currentState = state;

    /* This will apply the callerID gain correction factor if we changed to
     * an onhook state, or remove it if we changed to an offhook state. */
    Vp890SetRelGainInt(pLineCtx);

    if ((stateBytePre & VP890_SS_FXO_STATE_BITS) == VP890_SS_FXO_DISCONNECT) {
        /*
         * Kick off the FXO Disconnect to OHT workaround. If coming from
         * Disconnect, the next state must be OHT -- unless someone accessed
         * the device directly. Then, they're on their own...
         */
       if (pLineObj->lineTimers.timers.fxoTimer.pllRecovery == 0) {
            /* Start at very next tick */
           pLineObj->lineTimers.timers.fxoTimer.pllRecovery = 1;
           pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_DISABLE;
       }
    }

    return VP_STATUS_SUCCESS;
} /* SetFxoLineState() */

/*******************************************************************************
 * CidCorrectionLookup()
 *  This function does a linear interpolation table lookup to find a caller ID
 * gain correction factor based on a measured line voltage.
 *
 * Arguments:
 *  voltage - Measured voltage on a scale where 1V = 25 units and

           1.28V = 32 units.

 * Returns:
 *  If the given voltage is outside the range in the table, returns the nearest
 * gain value in the table.
 *  If the given voltage matches a table entry, returns the gain factor of that
 * entry.
 *  If the given voltage is between two entries, returns a gain factor
 * interpolated from the nearest two entries.
 ******************************************************************************/
static void
CidCorrectionLookup(
    uint16                  voltage,
    uint16                  *gainFactor,
    uint8                   *dtgVal)
{
    /* Lookup table for FXO callerID gain adjustments based on line voltage.
     * Voltage measurements are in units of 1.28, or 32/25, so we use a voltage
     * representation where 1V = 25 units and 1.28V = 32 units.
     * If you add/remove elements, make sure to change VP890_CID_CORRECT_TABLE_SIZE
     */
#define VP890_CID_CORRECTION_DTG12_TABLE_SIZE 13
    Vp890CidCorrectionPair tableDtg12[VP890_CID_CORRECTION_DTG12_TABLE_SIZE] =
    {   {/* 48.6 V */ 1216, 16843 /* Gain factor 1.028 */},
        {/* 51.2 V */ 1280, 18661 /* Gain factor 1.139 */},
        {/* 53.8 V */ 1344, 20578 /* Gain factor 1.256 */},
        {/* 56.3 V */ 1408, 22577 /* Gain factor 1.378 */},
        {/* 58.9 V */ 1472, 24674 /* Gain factor 1.506 */},
        {/* 61.4 V */ 1536, 26870 /* Gain factor 1.640 */},
        {/* 64.0 V */ 1600, 29164 /* Gain factor 1.780 */},
        {/* 66.6 V */ 1664, 31539 /* Gain factor 1.925 */},
        {/* 69.1 V */ 1728, 34013 /* Gain factor 2.076 */},
        {/* 71.7 V */ 1792, 36569 /* Gain factor 2.232 */},
        {/* 74.2 V */ 1856, 39240 /* Gain factor 2.395 */},
        {/* 76.8 V */ 1920, 41992 /* Gain factor 2.563 */},
        {/* 79.4 V */ 1984, 44827 /* Gain factor 2.736 */}
    };
#define VP890_CID_CORRECTION_DTG6_TABLE_SIZE 12
    Vp890CidCorrectionPair tableDtg6[VP890_CID_CORRECTION_DTG6_TABLE_SIZE] =
    {   {/* 34.6 V */  864, 16957 /* Gain factor 1.035 */},
        {/* 35.8 V */  896, 18235 /* Gain factor 1.113 */},
        {/* 37.1 V */  928, 19562 /* Gain factor 1.194 */},
        {/* 38.4 V */  960, 20939 /* Gain factor 1.278 */},
        {/* 39.7 V */  992, 22364 /* Gain factor 1.365 */},
        {/* 41.0 V */ 1024, 23822 /* Gain factor 1.454 */},
        {/* 42.2 V */ 1056, 25346 /* Gain factor 1.547 */},
        {/* 43.5 V */ 1088, 26903 /* Gain factor 1.642 */},
        {/* 44.8 V */ 1120, 28508 /* Gain factor 1.740 */},
        {/* 46.1 V */ 1152, 30163 /* Gain factor 1.841 */},
        {/* 47.4 V */ 1184, 31850 /* Gain factor 1.944 */},
        {/* 48.6 V */ 1216, 33604 /* Gain factor 2.051 */}
    };
#define VP890_CID_CORRECTION_DTG0_TABLE_SIZE 6
    Vp890CidCorrectionPair tableDtg0[VP890_CID_CORRECTION_DTG0_TABLE_SIZE] =
    {   {/* 28.2 V */  704, 22479 /* Gain factor 1.372 */},
        {/* 29.4 V */  736, 24560 /* Gain factor 1.499 */},
        {/* 30.7 V */  768, 26739 /* Gain factor 1.632 */},
        {/* 32.0 V */  800, 29016 /* Gain factor 1.771 */},
        {/* 33.3 V */  832, 31392 /* Gain factor 1.916 */},
        {/* 34.6 V */  864, 33849 /* Gain factor 2.066 */}
    };

    Vp890CidCorrectionPair *pTable;
    uint8 tableSize;
    uint8 index;

    VP_API_FUNC_INT(None, VP_NULL, ("+CidCorrectionLookup()"));

    /* Choose which table and DTG setting to use based on the voltage range */
    if (voltage < tableDtg6[0].volts) {
        pTable = tableDtg0;
        tableSize = VP890_CID_CORRECTION_DTG0_TABLE_SIZE;
        *dtgVal = VP890_DTG_0DB;
    } else if (voltage < tableDtg12[0].volts) {
        pTable = tableDtg6;
        tableSize = VP890_CID_CORRECTION_DTG6_TABLE_SIZE;
        *dtgVal = VP890_DTG_6DB;
    } else {
        pTable = tableDtg12;
        tableSize = VP890_CID_CORRECTION_DTG12_TABLE_SIZE;
        *dtgVal = VP890_DTG_12DB;
    }

    if (voltage <= pTable[0].volts) {
        /* Voltage is equal to or less than the lowest entry in the table, so
         * return the gain of the lowest entry */
        *gainFactor = pTable[0].gain;
        return;
    }
    for (index = 0; index < tableSize-1; index++){
        if (voltage < pTable[index+1].volts) {
            /* Linear Interpolation
             * vm = voltage     v[i] = pTable[index].volts
             * g = gain result  g[i] = pTable[index].gain
             * Formula: g = (( vm - v[i] ) / ( v[i+1] - v[i] )) * ( g[i+1] - g[i] ) + g[i]
             * Terms:   g = ((   termA   ) / (      termB    )) * (     termC     ) + g[i]
             * Fixed point reorder:  g = ( termC / termB ) * termA + g[i]
             */
            uint16 termA, termB, termC;
            termA = voltage - pTable[index].volts;
            termB = pTable[index+1].volts - pTable[index].volts;
            termC = pTable[index+1].gain - pTable[index].gain;

            *gainFactor = ( termC / termB ) * termA + pTable[index].gain;
            return;
        }
    }
    /* Voltage is equal to or beyond the highest entry in the table, so return
     * the gain of the highest entry */
    *gainFactor = pTable[index].gain;
    return;
} /* CidCorrectionLookup() */

/*******************************************************************************
 * PllRecovery()
 *  This function is part of the software fix for the PLL recovery which appears
 * when the FXO line comes out of Disconnect. This solution monitors ICR1
 * for PLL detection, and when fails "flips the switch" to force a PLL restart.
 * This function is called from ServiceFxoTimers when timer pllRecovery expires.
 ******************************************************************************/
static void
PllRecovery(
    VpLineCtxType           *pLineCtx)
{
    Vp890LineObjectType     *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId  = pDevObj->deviceId;
    uint8                   ecVal     = pLineObj->ecVal;
    uint8                   icr1Data[VP890_ICR1_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+PllRecovery()"));

    /* Every state requires read of ICR1 */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_RD, VP890_ICR1_LEN, icr1Data);
    VpMemCpy(pLineObj->icr1Values, icr1Data, VP890_ICR1_LEN);

    /* We're done if the PLL started ok, or if we corrected it */
    if ((icr1Data[0] == 0x00)
     && ((icr1Data[VP890_SUB_ST_IDX] & VP890_SUB_ST_MASK) == VP890_SUB_ST_FXO_ON_HOOK_SUP)) {
        /* Done. */

        if (pLineObj->pllRecoveryState != VP890_PLL_RECOVERY_ST_RESET) {
            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("PLL OK at time %d Starting Final Delay",
                pDevObj->timeStamp));

            pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_RESET;
            Vp890SetFxoLineState(pLineCtx, pLineObj->lineState.usrCurrent);

            /*
             * Final Delay to allow device level debouncers to stabilize under
             * worst case loop-open condition. The "140" comes from 140ms device
             * level filtering.
             */
            pLineObj->lineTimers.timers.fxoTimer.pllRecovery =
                MS_TO_TICKRATE((VP_FXO_DISCONNECT_DEBOUNCE+140),
                pDevObj->devProfileData.tickRate);
            return;
        }

        /*
         * Generate the Device Init Complete Event if this occurred as a result
         * of device level initialization. Clear the line initialization flag
         * as well in this case, but don't generate the line init complete event
         * (it is implied by device init complete).
         * Generate the Line Init Complete Event if this occurred as a result of
         * Line Init but not Device Init (which calls Line Init).
         */
        if (!(pDevObj->state & VP_DEV_INIT_CMP)) {
            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Called From Init Device. Generating Event at time %d.",
                pDevObj->timeStamp));
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
            pDevObj->state |= VP_DEV_INIT_CMP;
        } else if (!(pLineObj->status & VP890_INIT_COMPLETE)) {
            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Called From Init Line Channel %d. Generating Event at time %d.",
                pLineObj->channelId, pDevObj->timeStamp));

            pLineObj->lineEvents.response |= VP_LINE_EVID_LINE_INIT_CMP;
        }
        pLineObj->status |= VP890_INIT_COMPLETE;

        pDevObj->state |= VP_DEV_PENDING_INT;

        return;
    }

    VP_FXO_FUNC(None, NULL, ("PLL FAILED -- 0x%02X 0x%02X 0x%02X 0x%02X at time %d",
        icr1Data[0], icr1Data[1], icr1Data[2], icr1Data[3], pDevObj->timeStamp));

    if(pLineObj->pllRecoveryState == VP890_PLL_RECOVERY_ST_RESET) {
        pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_DISABLE;
    }

    switch(pLineObj->pllRecoveryState) {
        case VP890_PLL_RECOVERY_ST_DISABLE:    {
            uint8 icr1allOff[] = {0xC0, 0x00, 0x00, 0x04};
            uint8 icr1analogEn[] = {0xC0, 0x80, 0x00, 0x04};

            VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_WRT, VP890_ICR1_LEN, icr1allOff);
            VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_WRT, VP890_ICR1_LEN, icr1analogEn);
            VpMemCpy(pLineObj->icr1Values, icr1analogEn, VP890_ICR1_LEN);

            pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_ENABLE_1;
            pLineObj->lineTimers.timers.fxoTimer.pllRecovery =
                MS_TO_TICKRATE(VP890_PLL_RECOVER_INIT_DELAY,
                pDevObj->devProfileData.tickRate);
            }
            break;

        case VP890_PLL_RECOVERY_ST_ENABLE_1: {
            uint8 icr1rstOff[] = {0xC0, 0xC0, 0x00, 0x04};

            VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_WRT, VP890_ICR1_LEN, icr1rstOff);
            VpMemCpy(pLineObj->icr1Values, icr1rstOff, VP890_ICR1_LEN);

            pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_ENABLE_2;
            pLineObj->lineTimers.timers.fxoTimer.pllRecovery =
                MS_TO_TICKRATE(VP890_PLL_RECOVER_MEAS_DELAY_1,
                    pDevObj->devProfileData.tickRate);
            }
            break;

        case VP890_PLL_RECOVERY_ST_ENABLE_2: {
            uint8 icr1normal[] = {0x00, 0xC0, 0x00, 0x04};

            VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_WRT, VP890_ICR1_LEN, icr1normal);
            VpMemCpy(pLineObj->icr1Values, icr1normal, VP890_ICR1_LEN);

            pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_MEASURE;
            pLineObj->lineTimers.timers.fxoTimer.pllRecovery =
                MS_TO_TICKRATE(VP890_PLL_RECOVER_MEAS_DELAY_2,
                    pDevObj->devProfileData.tickRate);
            }
            break;

        case VP890_PLL_RECOVERY_ST_MEASURE:
            /*
             * If we're here, the device is in a failed state. Just trap on a
             * maximum number of tries.
             */
            if (pLineObj->pllRecoverAttempts < 10) {
                pLineObj->pllRecoverAttempts++;
                pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_DISABLE;

                /* Come back at very next tick */
                pLineObj->lineTimers.timers.fxoTimer.pllRecovery = 1;
            } else {
                /*
                 * Failed. Don't come back, but let the init complete events
                 * be generated.
                */
                if (!(pDevObj->state & VP_DEV_INIT_CMP)) {
                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Called From Init Device. Generating Event."));
                    pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
                    pDevObj->state |= VP_DEV_INIT_CMP;
                    pLineObj->status |= VP890_INIT_COMPLETE;
                } else if (!(pLineObj->status & VP890_INIT_COMPLETE)) {
                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Called From Init Line - Channel %d. Generating Event.",
                        pLineObj->channelId));

                    pLineObj->lineEvents.response |= VP_LINE_EVID_LINE_INIT_CMP;
                    pLineObj->status |= VP890_INIT_COMPLETE;
                }
            }
            break;

        default:
            break;
    }
}

#endif /* VP_CC_890_SERIES */

