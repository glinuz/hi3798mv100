/** \file vp890_lp_control.c
 * vp890_lp_control.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Control Functions for LP Termination Types.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 6638 $
 * $LastChangedDate: 2010-03-08 16:08:28 -0600 (Mon, 08 Mar 2010) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES) && defined (VP890_LP_SUPPORT)

#include    "vp_api_int.h"
#include    "vp890_api_int.h"

/* =================================
    Prototypes for Static Functions
   ================================= */
static void
Vp890SetLP(
    bool lpMode,
    VpLineCtxType *pLineCtx);

static void
Vp890WriteLPEnterRegisters(
    VpLineCtxType *pLineOtx,
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 *lineState);

/**
 * Vp890RunLPDisc()
 *  This function implements the Disconnect Enter/Exit for Low Power Mode.
 *
 * Preconditions:
 *  None. Calling function must know that this code should execute.
 *
 * Postconditions:
 *  Initial procedures are performed and timers set to enter or exit Disconnect
 * state for Low Power termination type.
 */
void
Vp890RunLPDisc(
    VpLineCtxType *pLineCtx,
    bool discMode,
    uint8 nextSlicByte)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    uint8                   ecVal       = pLineObj->ecVal;

    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    uint8                   channelId   = pLineObj->channelId;

    uint8 mpiBuffer[6 + VP890_REGULATOR_PARAM_LEN + VP890_ICR2_LEN +
                        VP890_ICR1_LEN + VP890_SYS_STATE_LEN +
                        VP890_ICR3_LEN + VP890_ICR4_LEN];
    uint8 mpiIndex = 0;

    /*
     * myDevState "set" means *this* line is in a LPM state prior to calling
     * this function.
     */
    Vp890DeviceStateIntType myDevState = (channelId == 0) ?
        (pDevObj->stateInt & VP890_LINE0_LP) :
        (pDevObj->stateInt & VP890_LINE1_LP);

    bool hookStatus;
    Vp890LineStatusType leakyLine = pLineObj->status & VP890_LINE_LEAK;

    /*
     * Enter/Exit Disconnect uses Active (w/Polarity) to cause fast charge or
     * discharge of the line.
     */
    uint8 lineState[]  = {VP890_SS_ACTIVE};

    VpCSLACGetLineStatus(pLineCtx, VP_INPUT_RAW_HOOK, &hookStatus);
    leakyLine = (hookStatus == FALSE) ? leakyLine : VP890_LINE_LEAK ;

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("+Vp890RunLPDisc() NextByte 0x%02X", nextSlicByte));

    if ((discMode == TRUE)
     || ((nextSlicByte == VP890_SS_IDLE) && (!(leakyLine)))) {
        /*
         * Either entering Disconnect or Exiting into Standby. Either case, set
         * floor voltage to -70V for LPM T/R voltage.
         */
        pDevObj->swParamsCache[VP890_FLOOR_VOLTAGE_BYTE] &= ~VP890_FLOOR_VOLTAGE_MASK;
        pDevObj->swParamsCache[VP890_FLOOR_VOLTAGE_BYTE] |= 0x0D;   /* 70V */
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_REGULATOR_PARAM_WRT,
            VP890_REGULATOR_PARAM_LEN, pDevObj->swParamsCache);
    }

    if (discMode == TRUE) {        /* Entering Disconnect */
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Entering Disconnect on Chan %d time %d",
            channelId, pDevObj->timeStamp));

        /*
         * There are two cases to consider when entering Disconnect:
         *     1. This line is coming from LPM-Standby
         *     2. This line is coming from a non-LPM state
         *
         *  All cases require the ICR values modified to disable the switcher.
         */

        /*
         * Step 1: Program all ICR registers including Disable Switcher. Note
         * these are writing to cached values only, not to the device -- yet.
         */
        Vp890SetLPRegisters(pDevObj, pLineObj, TRUE);
        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] &= ~VP890_ICR2_ILA_DAC;
        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] |= VP890_ICR2_VOC_DAC_SENSE;
        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX+1] &= ~VP890_ICR2_VOC_DAC_SENSE;

        pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX] |= VP890_ICR2_SWY_CTRL_EN;
        pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX+1] &= ~VP890_ICR2_SWY_CTRL_EN;

        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Entering Disconnect: Channel %d: Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X",
            pLineObj->channelId,
            pLineObj->icr2Values[0], pLineObj->icr2Values[1],
            pLineObj->icr2Values[2], pLineObj->icr2Values[3]));

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR2_WRT,
            VP890_ICR2_LEN, pLineObj->icr2Values);

        /*
         * Force Tip, Ring and line Bias Override and set values to max. This
         * forces values toward ground.
         */
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION] |=
            (VP890_ICR1_TIP_BIAS_OVERRIDE | VP890_ICR1_LINE_BIAS_OVERRIDE);
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION+1] |=
            (VP890_ICR1_TIP_BIAS_OVERRIDE | VP890_ICR1_LINE_BIAS_OVERRIDE);

        pLineObj->icr1Values[VP890_ICR1_RING_BIAS_OVERRIDE_LOCATION] |=
            VP890_ICR1_RING_BIAS_OVERRIDE;
        pLineObj->icr1Values[VP890_ICR1_RING_BIAS_OVERRIDE_LOCATION+1] |=
            VP890_ICR1_RING_BIAS_OVERRIDE;

        if (Vp890ProtectedWriteICR1(pLineObj, deviceId, pLineObj->icr1Values, FALSE) == FALSE) {
            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter Disconnect: Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
                pLineObj->icr1Values[0], pLineObj->icr1Values[1],
                pLineObj->icr1Values[2], pLineObj->icr1Values[3], pLineObj->channelId));

			/*BEGIN: fix MOTO*/
			if (26 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR1_WRT,
                VP890_ICR1_LEN, pLineObj->icr1Values);
        }

        if (myDevState) {
            /*
             * State is ACITVE coming from Standy. No need to modify value
             * value preset for "lineState".
             */
            pLineObj->slicValueCache = lineState[0];
        } else {
            /* State is Polarity conditioned coming from any other state. */
            pLineObj->slicValueCache &= ~VP890_SS_LINE_FEED_MASK;
            pLineObj->slicValueCache |= VP890_SS_ACTIVE;
        }

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
            VP890_SYS_STATE_LEN, &pLineObj->slicValueCache);

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR3_WRT,
            VP890_ICR3_LEN, pLineObj->icr3Values);

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR4_WRT,
            VP890_ICR4_LEN, pLineObj->icr4Values);

        VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
            &mpiBuffer[1]);
        mpiIndex = 0;

        pLineObj->nextSlicValue = VP890_SS_DISCONNECT;

        /* Set Discharge Time based on Supply Configuration. */
        pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] =
            (MS_TO_TICKRATE(Vp890SetDiscTimers(pDevObj),
            pDevObj->devProfileData.tickRate)) | VP_ACTIVATE_TIMER;

    } else {    /* Exiting Disconnect */
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Recovering Chan %d from DISCONNECT at time %d with value 0x%02X",
            pLineObj->channelId, pDevObj->timeStamp, nextSlicByte));

        /*
         * There are two cases to consider when exiting Disconnect:
         *     1. This line is going to VP_LINE_STANDBY.
         *     2. This line is going to non-LPM state.
         */
        pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX] &= ~VP890_ICR2_VOC_DAC_SENSE;
        pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX] &= ~VP890_ICR2_SWY_CTRL_EN;
        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] &= ~VP890_ICR2_ILA_DAC;

        pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX+1] |= VP890_ICR3_LINE_CTRL;

        /*
         * Disable Tip and Ring bias control completely. Set SLIC bias back
         * to normal values.
         */
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION] &=
            ~(VP890_ICR1_TIP_BIAS_OVERRIDE);
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION+1] &=
            (unsigned char)(~(VP890_ICR1_TIP_BIAS_OVERRIDE | VP890_ICR1_LINE_BIAS_OVERRIDE));
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION+1] |=
            (VP890_ICR1_LINE_BIAS_OVERRIDE_NORM);
        pLineObj->icr1Values[VP890_ICR1_RING_BIAS_OVERRIDE_LOCATION] &=
            ~VP890_ICR1_RING_BIAS_OVERRIDE;

        if ((nextSlicByte == VP890_SS_IDLE) && (!(leakyLine))) {
            /*
             * 1. This line is going to LPM-VP_LINE_STANDBY.
             *      Step 1: Enable Switcher and set to 70V floor.
             *      Step 2: Set line to Active (for fast charge)
             *      Step 3: Set line to SLIC-Disconnect (end of timer)
             */
            Vp890SetLPRegisters(pDevObj, pLineObj, TRUE);

            pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] =
                (VP890_ICR2_TIP_SENSE | VP890_ICR2_RING_SENSE);

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Case 1: Channel %d: Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X",
                pLineObj->channelId,
                pLineObj->icr2Values[0], pLineObj->icr2Values[1],
                pLineObj->icr2Values[2], pLineObj->icr2Values[3]));

			/*BEGIN: fix MOTO*/
			if (26 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR2_WRT,
                VP890_ICR2_LEN, pLineObj->icr2Values);

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Case 1: Setting Ch %d to State 0x%02X at time %d",
                channelId, lineState[0], pDevObj->timeStamp));

            pLineObj->nextSlicValue = VP890_SS_DISCONNECT;

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Disconnect Exit: Channel %d: State 0x%02X",
                pLineObj->channelId, lineState[0]));

            pLineObj->slicValueCache = lineState[0];

			/*BEGIN: fix MOTO*/
			if (26 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
                VP890_SYS_STATE_LEN, &pLineObj->slicValueCache);

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Disconnect Exit: Channel %d: Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X",
                pLineObj->channelId,
                pLineObj->icr1Values[0], pLineObj->icr1Values[1],
                pLineObj->icr1Values[2], pLineObj->icr1Values[3]));

			/*BEGIN: fix MOTO*/
			if (26 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR1_WRT,
                VP890_ICR1_LEN, pLineObj->icr1Values);

            VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
                &mpiBuffer[1]);
            mpiIndex = 0;

            /*
             * This prevents LPM Exit Code from running since it is no longer
             * necessary.
             */
            pLineObj->status |= VP890_LOW_POWER_EN;
            pDevObj->stateInt |= (VP890_LINE0_LP | VP890_LINE1_LP);
        } else {
            /*
             * 2. This line is going to non-LPM state, so take care of all LPM
             *    exit handling here.
             *      Step 1: Enable Switcher on this line
             *      Step 2: Set line to Active w/Polarity (for fast charge)
             *      Step 3: Set remaining ICR registers for LPM exit settings.
             *      Step 4: Set line to new SLIC state (end of timer)
             */
            Vp890SetLPRegisters(pDevObj, pLineObj, FALSE);   /* ICR3 and 4 correct */

            /* Update state to match target polarity. */
            lineState[0] |= (VP890_SS_POLARITY_MASK & nextSlicByte);
            pLineObj->nextSlicValue = nextSlicByte;

            /* Restore Supply voltage. */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
                VP890_REGULATOR_PARAM_LEN, pDevObj->devProfileData.swParams);
            VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
                VP890_REGULATOR_PARAM_LEN);

            /* Run LPM Exit Sequence */
            Vp890WriteLPExitRegisters(pLineCtx, deviceId, ecVal, lineState);

            /*
             * This prevents LPM Exit Code from running since it is no longer
             * necessary.
             */
            pLineObj->status &= ~VP890_LOW_POWER_EN;
            pDevObj->stateInt &= ~(VP890_LINE0_LP | VP890_LINE1_LP);
        }

        /*
         * Disable LPM Exit sequence if for some reason it was previously started
         * (e.g., application quickly went from Standby-OHT-Disconnect). When
         * the Tracker Disable Time expires, it will set the line to Disconnect
         * and Disable the Tracker (ICR2). While some of the steps may be what
         * we're also trying to do, Disconnect handling is done with the
         * Disconnect Exit (which also is Disoconnect Enter) timer.
         */
        pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE]
            &= ~VP_ACTIVATE_TIMER;
    }

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("-Vp890RunLPDisc()"));
}

/**
 * Vp890SetDiscTimers()
 *  This function provides the value for Disconnect Timing using LPM.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None. Returns a value in ms.
 */
uint16
Vp890SetDiscTimers(
    Vp890DeviceObjectType *pDevObj)
{
    if (pDevObj->swParamsCache[VP890_REGULATOR_TRACK_INDEX] & VP890_REGULATOR_FIXED_RING) {
        /*
         * Longest is using "fixed" ringing mode because the external
         * capacitors are generally very large.
         */
        return VP890_FIXED_TRACK_DISABLE_TIME;
    } else {
        return VP890_INVERT_BOOST_DISABLE_TIME;
    }
}

/**
 * Vp890LowPowerMode()
 *  This function is called when the device should be updated for Low Power
 * mode. It determines if the device can be put into low power mode and does
 * (if it can), sets a flag in the device object, and sets the device timer
 * for hook debounce.
 *
 * Preconditions:
 *
 * Postconditions:
 */
void
Vp890LowPowerMode(
    VpDevCtxType *pDevCtx)
{
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VpLineCtxType *pLineCtx;
    bool isValidCtx[VP890_MAX_NUM_CHANNELS] = {FALSE, FALSE};
    Vp890LineObjectType *pLineObj;
    bool lowPower;

    /*
     * FXS exists only on first channel or not at all (FXO only device). But for
     * FXO only device, this function is not called.
     */
    uint8 maxChannels = 1;
    uint8 channelId;
    uint8 ecVal;

#ifdef VP890_INCLUDE_TESTLINE_CODE
    /* We don't want to interact with the line in this state */
    if (pDevObj->currentTest.nonIntrusiveTest == TRUE) {
        return;
    }
#endif

    /* Don't do anything if device is in calibration */
    if (pDevObj->state & VP_DEV_IN_CAL) {
        return;
    }

    for (channelId = 0; channelId < maxChannels; channelId++) {
        if (pDevCtx->pLineCtx[channelId] != VP_NULL) {
            pLineCtx = pDevCtx->pLineCtx[channelId];
            pLineObj = pLineCtx->pLineObj;

            /* Don't enter or exit LPM until all lines have been initialized. */
            if (!(pLineObj->status & VP890_INIT_COMPLETE)) {
                VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Init Line Not Complete"));
                return;
            }
        }
    }

    if (pDevObj->stateInt & VP890_LINE0_LP) {
        lowPower = TRUE;
    } else {
        lowPower = FALSE;
    }

    /*
     * Determine which lines are valid in case we have to adjust their line
     * states. Consider "valid" only those lines that are FXS Low Power type.
     */
    for (channelId = 0; channelId < maxChannels; channelId++) {
        if (pDevCtx->pLineCtx[channelId] != VP_NULL) {
            pLineCtx = pDevCtx->pLineCtx[channelId];
            pLineObj = pLineCtx->pLineObj;
            if ((!(pLineObj->status & VP890_IS_FXO))
               && ((pLineObj->termType == VP_TERM_FXS_LOW_PWR) ||
                   (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP) ||
                   (pLineObj->termType == VP_TERM_FXS_SPLITTER_LP))) {
                isValidCtx[channelId] = TRUE;
                if ((Vp890IsChnlUndrTst(pDevObj, channelId) == TRUE) ||
                    (pLineObj->status & VP890_LINE_LEAK)) {
                    lowPower = FALSE;
                }

                if ((pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                    & VP_ACTIVATE_TIMER) && (lowPower == TRUE)){
                    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Delay LP Enter for PolRev at time %d",
                        pDevObj->timeStamp));
                    return;
                }
            }
        }
    }

    if (lowPower == FALSE) {
        /*
         * Take the device out of low power mode and set channels to correct
         * states. Do not affect device or channels if change has already
         * been made.
         */

        for (channelId = 0; channelId < maxChannels; channelId++) {
            if (isValidCtx[channelId] == TRUE) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                pLineObj = pLineCtx->pLineObj;
                ecVal = pLineObj->ecVal;

                if (pLineObj->status & VP890_LOW_POWER_EN) {
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890LowPowerMode+"));

                    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Exit LPM for Line %d",
                        channelId));

                    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
                        VP890_REGULATOR_PARAM_LEN, pDevObj->devProfileData.swParams);
                    VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
                        VP890_REGULATOR_PARAM_LEN);

                    Vp890SetLP(FALSE, pLineCtx);

                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890LowPowerMode-"));
                }
            }
        }
    } else {
        /*
         * We should be in low power mode because both lines can be put into
         * low power mode. Don't need to call Set Line State in this case for
         * each line because there are a limited number of API-II states that
         * can allow Low Power, and all required the SLIC state to be set to
         * Disconnect.
         */
        for (channelId = 0; channelId < maxChannels; channelId++) {
            if (isValidCtx[channelId] == TRUE) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                pLineObj = pLineCtx->pLineObj;
                ecVal = pLineObj->ecVal;

                if (!(pLineObj->status & VP890_LOW_POWER_EN)) {
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890LowPowerMode+"));

                    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Enter LPM for Line %d", channelId));

                    pDevObj->swParamsCache[VP890_FLOOR_VOLTAGE_BYTE] &= ~VP890_FLOOR_VOLTAGE_MASK;
                    pDevObj->swParamsCache[VP890_FLOOR_VOLTAGE_BYTE] |= 0x0D;   /* 70V */
                    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
                        VP890_REGULATOR_PARAM_LEN, pDevObj->swParamsCache);

                    Vp890SetLP(TRUE, pLineCtx);
                    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890LowPowerMode-"));
                }
            }
        }
    }
}

/**
 * Vp890SetLP()
 *  This function modifies the line/device to enter/exit LPM.
 *
 * Preconditions:
 *
 * Postconditions:
 */
void
Vp890SetLP(
    bool lpMode,
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 ecVal = pLineObj->ecVal;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 lineState[VP890_SYS_STATE_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890SetLP+"));

    /* Need timer here to allow switcher to stabilize whether entering or
     * exiting LPM.
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

    if (lpMode == FALSE) {
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Taking Channel %d out of Low Power Mode at time %d User State %d",
            pLineObj->channelId, pDevObj->timeStamp, pLineObj->lineState.usrCurrent));

        pLineObj->status &= ~VP890_LOW_POWER_EN;

        if (pLineObj->lineState.usrCurrent != VP_LINE_DISCONNECT) {
            bool internalApiOperation = FALSE;

            Vp890SetLPRegisters(pDevObj, pLineObj, FALSE);

#ifdef VP_CSLAC_SEQ_EN
            if ((pLineObj->cadence.status & (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_SENDSIG)) ==
                (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_SENDSIG)) {
                internalApiOperation = TRUE;
            }
#endif
            if ((pLineObj->lineState.usrCurrent == VP_LINE_STANDBY)
             && (internalApiOperation == FALSE)) {
                lineState[0] = VP890_SS_ACTIVE;
                Vp890WriteLPExitRegisters(pLineCtx, deviceId, ecVal,
                    lineState);
                pLineObj->nextSlicValue = VP890_SS_IDLE;
            } else {
                Vp890WriteLPExitRegisters(pLineCtx, deviceId, ecVal,
                    VP_NULL);
            }
        }

        /*
         * Disable LPM Exit sequence. When the Tracker Disable Time expires, it
         * will set the line to Disconnect and Disable the Tracker (ICR2).
         * Obviously, we no longer want to do this.
         */
        pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE]
            &= ~VP_ACTIVATE_TIMER;
    } else {
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Putting Channel %d in Low Power Mode at time %d",
            pLineObj->channelId, pDevObj->timeStamp));

        pLineObj->status |= VP890_LOW_POWER_EN;

        if (pLineObj->lineState.usrCurrent != VP_LINE_DISCONNECT) {
            /* Set timer to wait before making final changes. When this timer
             * expires, the following sequence is run:
             *
             *  - Set SLIC state to Disconnect
             *  - Write the following:
             *      icr2[VP890_ICR2_VOC_DAC_INDEX] |= VP890_ICR2_ILA_DAC;
             *      icr2[VP890_ICR2_VOC_DAC_INDEX] &= ~VP890_ICR2_VOC_DAC_SENSE;
             *      icr2[VP890_ICR2_VOC_DAC_INDEX+1] &= ~VP890_ICR2_ILA_DAC;
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE] =
                (MS_TO_TICKRATE(VP890_TRACKER_DISABLE_TIME,
                    pDevObj->devProfileData.tickRate)) | VP_ACTIVATE_TIMER;

            /*
             * We are entering LPM into VP_LINE_STANDBY. So the required ICR
             * values are not yet set in the line object (as they would be if
             * entering from VP_LINE_DISCONNECT).
             */
            lineState[0] = VP890_SS_DISCONNECT;

            Vp890SetLPRegisters(pDevObj, pLineObj, TRUE);
        } else {
            /*
             * This case occurs from Line Test. The line needs to get back to
             * Disconnect and Line Test calls this function to do that.
             */
            uint16 dischargeTime = Vp890SetDiscTimers(pDevObj);

            /*
             * We are entering LPM into VP_LINE_DISCONNECT. So the required ICR
             * values have been set in the line object, just need to force the
             * required sequence (Active w/polarity, then Disconnect).
             */
            lineState[0] = VP890_SS_ACTIVE;

            /* Set Discharge Time based on Supply Configuration. */
            if (dischargeTime < VP890_TRACKER_DISABLE_TIME) {
                dischargeTime = VP890_TRACKER_DISABLE_TIME;
            }

            /* Set timer to wait before making final changes. When this timer
             * expires, the following sequence is run:
             *
             *  - Set SLIC state to Disconnect
             *  - Write the following:
             *      icr2[VP890_ICR2_VOC_DAC_INDEX] |= VP890_ICR2_ILA_DAC;
             *      icr2[VP890_ICR2_VOC_DAC_INDEX] &= ~VP890_ICR2_VOC_DAC_SENSE;
             *      icr2[VP890_ICR2_VOC_DAC_INDEX+1] &= ~VP890_ICR2_ILA_DAC;
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] =
                (MS_TO_TICKRATE(dischargeTime, pDevObj->devProfileData.tickRate))
                | VP_ACTIVATE_TIMER;

        }

        Vp890WriteLPEnterRegisters(pLineCtx, deviceId, ecVal, lineState);
    }
    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp890SetLP-"));
}

/**
 * Vp890WriteLPExitRegisters()
 *  This function writes the ICR and State values to the device for LPM exit.
 *
 * Preconditions:
 *  None. Modification of line object data only.
 *
 * Postconditions:
 *  The device registers have been modified.
 */
void
Vp890WriteLPExitRegisters(
    VpLineCtxType *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 *lineState)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 mpiBuffer[5 + VP890_SYS_STATE_LEN + VP890_ICR1_LEN + VP890_ICR2_LEN
                      + VP890_ICR3_LEN + VP890_ICR4_LEN];
    uint8 mpiIndex = 0;

    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR3_WRT,
        VP890_ICR3_LEN, pLineObj->icr3Values);

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR4_WRT,
        VP890_ICR4_LEN, pLineObj->icr4Values);

    if (lineState == VP_NULL) {
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing LineState %d on Ch %d",
            pLineObj->lineState.currentState, pLineObj->channelId));

        VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
            &mpiBuffer[1]);
        mpiIndex = 0;

        Vp890SetFxsLineState(pLineCtx, pLineObj->lineState.currentState);
    } else {
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing SLIC State 0x%02X on Ch %d",
            lineState[0], pLineObj->channelId));

        pLineObj->slicValueCache = lineState[0];

		/*BEGIN: fix MOTO*/
		if (22 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
            VP890_SYS_STATE_LEN, &pLineObj->slicValueCache);
    }

    if (Vp890ProtectedWriteICR1(pLineObj, deviceId, pLineObj->icr1Values, FALSE) == FALSE) {
        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
            pLineObj->icr1Values[0], pLineObj->icr1Values[1],
            pLineObj->icr1Values[2], pLineObj->icr1Values[3], pLineObj->channelId));

		/*BEGIN: fix MOTO*/
		if (22 <= mpiIndex)
		{
			return ;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR1_WRT,
            VP890_ICR1_LEN, pLineObj->icr1Values);
    }

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR2_WRT,
        VP890_ICR2_LEN, pLineObj->icr2Values);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
        &mpiBuffer[1]);

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing ICR3 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr3Values[0], pLineObj->icr3Values[1],
        pLineObj->icr3Values[2], pLineObj->icr3Values[3], pLineObj->channelId));

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing ICR4 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr4Values[0], pLineObj->icr4Values[1],
        pLineObj->icr4Values[2], pLineObj->icr4Values[3], pLineObj->channelId));

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Exit: Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr2Values[0], pLineObj->icr2Values[1],
        pLineObj->icr2Values[2], pLineObj->icr2Values[3], pLineObj->channelId));
}

/**
 * Vp890WriteLPEnterRegisters()
 *  This function writes the ICR and State values to the device for LPM enter.
 *
 * Preconditions:
 *  None. Modification of line object data only.
 *
 * Postconditions:
 *  The device registers have been modified.
 */
void
Vp890WriteLPEnterRegisters(
    VpLineCtxType *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 *lineState)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 mpiBuffer[5 + VP890_SYS_STATE_LEN + VP890_ICR1_LEN + VP890_ICR2_LEN
                      + VP890_ICR3_LEN + VP890_ICR4_LEN];
    uint8 mpiIndex = 0;

    if (Vp890ProtectedWriteICR1(pLineObj, deviceId, pLineObj->icr1Values, FALSE) == FALSE) {
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR1_WRT,
            VP890_ICR1_LEN, pLineObj->icr1Values);

        VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter: Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
            pLineObj->icr1Values[0], pLineObj->icr1Values[1],
            pLineObj->icr1Values[2], pLineObj->icr1Values[3], pLineObj->channelId));
    }

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR2_WRT,
        VP890_ICR2_LEN, pLineObj->icr2Values);

    pLineObj->slicValueCache = lineState[0];

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
        VP890_SYS_STATE_LEN, &pLineObj->slicValueCache);

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR3_WRT,
        VP890_ICR3_LEN, pLineObj->icr3Values);

	/*BEGIN: fix MOTO*/
	if (22 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR4_WRT,
        VP890_ICR4_LEN, pLineObj->icr4Values);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
        &mpiBuffer[1]);

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter: Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr2Values[0], pLineObj->icr2Values[1],
        pLineObj->icr2Values[2], pLineObj->icr2Values[3], pLineObj->channelId));

    /* Set line to desired state */
    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter: Setting State 0x%02X on Ch %d",
        lineState[0], pLineObj->channelId));

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter: Writing ICR3 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr3Values[0], pLineObj->icr3Values[1],
        pLineObj->icr3Values[2], pLineObj->icr3Values[3], pLineObj->channelId));

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("LP Enter: Writing ICR4 0x%02X 0x%02X 0x%02X 0x%02X on Ch %d",
        pLineObj->icr4Values[0], pLineObj->icr4Values[1],
        pLineObj->icr4Values[2], pLineObj->icr4Values[3], pLineObj->channelId));
}

/**
 * Vp890SetLPRegisters()
 *  This function modifies the line object ICR register values. It does not
 * write to the device.
 *
 * Preconditions:
 *  None. Modification of line object data only.
 *
 * Postconditions:
 *  The line object data (ICR values) have been modified.
 */
void
Vp890SetLPRegisters(
    Vp890DeviceObjectType *pDevObj,
    Vp890LineObjectType *pLineObj,
    bool lpModeTo)
{
    VP_API_FUNC_INT(None, VP_NULL, ("Vp890SetLPRegisters+"));

    if (lpModeTo == TRUE) {
        /* Set Line Bias, remove other bias control */
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION] =
            VP890_ICR1_LINE_BIAS_OVERRIDE;
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION+1] = 0x08;

        pLineObj->icr1Values[VP890_ICR1_RING_AND_DAC_LOCATION] &=
            ~VP890_ICR1_RING_BIAS_DAC_MASK;
        pLineObj->icr1Values[VP890_ICR1_RING_AND_DAC_LOCATION+1] &=
            ~VP890_ICR1_RING_BIAS_DAC_MASK;

        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] =
            (VP890_ICR2_DAC_SENSE | VP890_ICR2_FEED_SENSE
           | VP890_ICR2_TIP_SENSE | VP890_ICR2_RING_SENSE);

        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX+1] =
            (VP890_ICR2_FEED_SENSE
            | VP890_ICR2_TIP_SENSE | VP890_ICR2_RING_SENSE);

        pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX] =
            (VP890_ICR2_SWY_LIM_CTRL1 | VP890_ICR2_SWY_LIM_CTRL);
        pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX+1] =
            (VP890_ICR2_SWY_LIM_CTRL1);

        pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX] |= VP890_ICR3_LINE_CTRL;
        pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX+1] |= VP890_ICR3_LINE_CTRL;

        pLineObj->icr4Values[VP890_ICR4_SUP_INDEX] |=
            (VP890_ICR4_SUP_DAC_CTRL | VP890_ICR4_SUP_DET_CTRL
            | VP890_ICR4_SUP_POL_CTRL);
        pLineObj->icr4Values[VP890_ICR4_SUP_INDEX+1] |=
            (VP890_ICR4_SUP_DAC_CTRL | VP890_ICR4_SUP_DET_CTRL
            | VP890_ICR4_SUP_POL_CTRL);
    } else {
        pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX] &= ~VP890_ICR3_LINE_CTRL;

        pLineObj->icr4Values[VP890_ICR4_SUP_INDEX] &=
            ~(VP890_ICR4_SUP_DAC_CTRL | VP890_ICR4_SUP_DET_CTRL
            | VP890_ICR4_SUP_POL_CTRL);

        /* Remove previously set SW control of ICR1 */
        pLineObj->icr1Values[VP890_ICR1_BIAS_OVERRIDE_LOCATION] &=
            ~VP890_ICR1_LINE_BIAS_OVERRIDE;

        pLineObj->icr2Values[VP890_ICR2_SENSE_INDEX] &=
            ~(VP890_ICR2_RING_SENSE | VP890_ICR2_TIP_SENSE |
              VP890_ICR2_DAC_SENSE | VP890_ICR2_FEED_SENSE);
    }

    VP_API_FUNC_INT(None, VP_NULL, ("Vp890SetLPRegisters-"));
}

#endif /* VP_CC_890_SERIES */

