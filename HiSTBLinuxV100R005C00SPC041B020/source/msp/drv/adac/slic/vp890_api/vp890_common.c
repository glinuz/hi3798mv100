/** \file vp890_common.c
 * vp890_common.c
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7364 $
 * $LastChangedDate: 2010-06-17 18:19:26 -0500 (Thu, 17 Jun 2010) $
 */

#include    "vp_api.h"

#if defined (VP_CC_890_SERIES)  /* Compile only if required */

#include    "vp_api_int.h"
#include    "vp890_api_int.h"

static void
ConvertSignedFixed2Csd(
    int32                   fixed,
    uint8                   *csdBuf);

/*******************************************************************************
 * Vp890IsDevReady()
 * This function checks the state of the device against the device complete,
 * the device initilaztion in progress and the device in calibration flags.
 *
 * Arguments:
 *  state           -   current internal state of the device.
 *  checkCal        -   indication that the function needs to check for calibration
 *
 * Returns
 *  TRUE  - If device is ready
 *  FALSE - If the device is busy being initialized or calibrated.
 ******************************************************************************/
bool
Vp890IsDevReady(
    VpDeviceStateType   state,
    bool                checkCal)
{
    /* Proceed if device state is either in progress or complete */
    if (!(state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS))) {
        VP_ERROR(None, VP_NULL, ("Vp890IsDevReady() - device not initilized!!!"));
        return FALSE;
    }
    return TRUE;
} /* Vp890IsDevReady() */


/*******************************************************************************
 * Vp890IsProfileValid()
 * This function checks the validity of a profile passed into the API via
 * a pointer or an index.
 *
 * Arguments:
 *  tableSize       -   size of the profile table being checked
 *  profEntry       -   value of the profile entry being checked
 *  profType        -   type of profile that is being checked
 *  pProfTable      -   pointer to profile table pointers profType
 *  pProfileInput   -   pointer to the profile being checked
 *  pProfileRslt    -   pointer to the resulting profile
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
bool
Vp890IsProfileValid(
    VpProfileType       profType,
    int16               tableSize,
    uint16              profEntry,
    VpProfilePtrType    *pProfTable,
    VpProfilePtrType    pProfileInput,
    VpProfilePtrType    *pProfileRslt)
{
    int                 profIndex   = VpGetProfileIndex(pProfileInput);

    /* Fail if profile index is beyond legal table size */
    if (profIndex >= tableSize) {
        VP_ERROR(None, VP_NULL, ("IsProfileValid() - profIndex exceeds table size"));
        return FALSE;
    }

    /* Input profile is null, -- NULL is legal */
    if (pProfileInput == VP_PTABLE_NULL) {
        *pProfileRslt = VP_PTABLE_NULL;
        return TRUE;
    }

    if (profIndex < 0) {
        /* Is the input profile a vaild profile type? */
        if ( VpVerifyProfileType(profType, pProfileInput)) {
            *pProfileRslt = pProfileInput;
            return TRUE;
        }
    } else if (profIndex < tableSize) {
        /* Does the profile table contain a profile at the requested index? */
        if ((profEntry & (0x01 << profIndex))) {
            *pProfileRslt = pProfTable[profIndex];
            return TRUE;
        }
    }

    VP_ERROR(None, VP_NULL, ("IsProfileValid() - invalid profile"));
    return FALSE;
} /* Vp890IsProfileValid */

/*******************************************************************************
 * Vp890SetDTMFGenerators()
 *  This function sets signal generator A/B for DTMF tone generation.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The signal generators A/B are set to the DTMF frequencies and level required
 * by the digit passed.
 ******************************************************************************/
VpStatusType
Vp890SetDTMFGenerators(
    VpLineCtxType               *pLineCtx,
    VpCidGeneratorControlType   mode,
    VpDigitType                 digit)
{
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType        deviceId  = pDevObj->deviceId;
    uint8                 ecVal     = pLineObj->ecVal;

#ifdef VP_CSLAC_SEQ_EN
    uint8 sigByteCount;
    uint8 sigOffset = VP_CID_PROFILE_FSK_PARAM_LEN + 2;
#endif
    uint8 sigGenCtrl[] = {0};

    uint8 sigGenCDParams[VP890_SIGCD_PARAMS_LEN] = {
        0x00, 0x00, /* Replace with required column Frequency */
        0x1C, 0x32, /* Level = -10dBm */
        0x00, 0x00, /* Replace with required row Frequency */
        0x1C, 0x32  /* Level = -10dBm */
    };

    uint8 columnFreqs[] = {
        0x0C, 0xE5,    /* 1209Hz (1, 4, 7, *) */
        0x0E, 0x40,    /* 1336Hz (2, 5, 8, 0) */
        0x0F, 0xC1,    /* 1477Hz (3, 6, 9, #) */
        0x11, 0x6B     /* 1633Hz (A, B, C, D) */
    };

    uint8 rowFreqs[] = {
        0x07, 0x6F,    /* 697Hz (1, 2, 3, A) */
        0x08, 0x36,    /* 770Hz (4, 5, 6, B) */
        0x09, 0x16,    /* 852Hz (7, 8, 9, C) */
        0x0A, 0x09     /* 941Hz (*, 0, #, D) */
    };

#ifdef VP_CSLAC_SEQ_EN
    /*
     * If we're generating caller ID data set the levels based on the data in
     * the CID profile
     */
    if ((pLineObj->callerId.status & VP_CID_IN_PROGRESS) &&
        (pLineObj->callerId.pCliProfile != VP_PTABLE_NULL)) {
        for (sigByteCount = 0; sigByteCount < (VP890_SIGCD_PARAMS_LEN - 3);
             sigByteCount++) {
            sigGenCDParams[sigByteCount+3] =
                pLineObj->callerId.pCliProfile[sigOffset + sigByteCount];
        }
    } else {
#endif
        /*
         * If it's an FXO line then the DTMF high and low frequency levels are
         * specified in the FXO/Dialing Profile, cached in the line object.
         */
        if (pLineObj->status & VP890_IS_FXO) {
            sigGenCDParams[2] = pLineObj->digitGenStruct.dtmfHighFreqLevel[0];
            sigGenCDParams[3] = pLineObj->digitGenStruct.dtmfHighFreqLevel[1];
            sigGenCDParams[6] = pLineObj->digitGenStruct.dtmfLowFreqLevel[0];
            sigGenCDParams[7] = pLineObj->digitGenStruct.dtmfLowFreqLevel[1];
        }
#ifdef VP_CSLAC_SEQ_EN
    }
#endif

    /* Set the Column Freqs first */
    switch(digit) {
        case 1:
        case 4:
        case 7:
        case VP_DIG_ASTER:
            sigGenCDParams[0] = columnFreqs[0];
            sigGenCDParams[1] = columnFreqs[1];
            break;

        case 2:
        case 5:
        case 8:
        case VP_DIG_ZERO:
            sigGenCDParams[0] = columnFreqs[2];
            sigGenCDParams[1] = columnFreqs[3];
            break;

        case 3:
        case 6:
        case 9:
        case VP_DIG_POUND:
            sigGenCDParams[0] = columnFreqs[4];
            sigGenCDParams[1] = columnFreqs[5];
            break;

        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
            sigGenCDParams[0] = columnFreqs[6];
            sigGenCDParams[1] = columnFreqs[7];
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetDTMFGenerators() - Invalid digit"));
            return VP_STATUS_INVALID_ARG;
    }

    /* Now set the row freqs */
    switch(digit) {
        case 1:
        case 2:
        case 3:
        case VP_DIG_A:
            sigGenCDParams[4] = rowFreqs[0];
            sigGenCDParams[5] = rowFreqs[1];
            break;

        case 4:
        case 5:
        case 6:
        case VP_DIG_B:
            sigGenCDParams[4] = rowFreqs[2];
            sigGenCDParams[5] = rowFreqs[3];
            break;

        case 7:
        case 8:
        case 9:
        case VP_DIG_C:
            sigGenCDParams[4] = rowFreqs[4];
            sigGenCDParams[5] = rowFreqs[5];
            break;

        case VP_DIG_ASTER:
        case VP_DIG_ZERO:
        case VP_DIG_POUND:
        case VP_DIG_D:
            sigGenCDParams[4] = rowFreqs[6];
            sigGenCDParams[5] = rowFreqs[7];
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890SetDTMFGenerators() - Invalid digit"));
            return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_SIGCD_PARAMS_WRT,
        VP890_SIGCD_PARAMS_LEN, sigGenCDParams);

    /*
     * If there is no change to generator control required, it is assumed to be
     * set properly prior to this function call.
     */
    if (mode != VP_CID_NO_CHANGE) {
        /*
         * For DTMF CID, the data passed may be message data, a keyed character
         * (e.g., Mark, Channel Seizure), or End of Transmission. If it's End
         * of Transmission, disable the DTMF generators immediately. Otherwise,
         * enable the DTMF generators
         */
        if ((mode == VP_CID_GENERATOR_DATA)
         || (mode == VP_CID_GENERATOR_KEYED_CHAR)) {
            sigGenCtrl[0] = (VP890_GENC_EN | VP890_GEND_EN);

            /* Setup the line timer for the on-time for DTMF CID */
            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF] =
                MS_TO_TICKRATE(VP_CID_DTMF_ON_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF]
                |= VP_ACTIVATE_TIMER;
#ifdef VP_CSLAC_SEQ_EN
            pLineObj->callerId.dtmfStatus |= VP_CID_ACTIVE_ON_TIME;
#endif
        }
        if (pLineObj->sigGenCtrl[0] != sigGenCtrl[0]) {
            pLineObj->sigGenCtrl[0] = sigGenCtrl[0];
            VpMpiCmdWrapper(deviceId, ecVal, VP890_GEN_CTRL_WRT, VP890_GEN_CTRL_LEN,
                pLineObj->sigGenCtrl);
        }
    }
    return VP_STATUS_SUCCESS;
} /* Vp890SetDTMFGenerators */

/*******************************************************************************
 * Vp890MuteChannel()
 *  This function disables or enables the PCM highway for the selected line and
 * should only be called by API internal functions.
 *
 * Arguments:
 *  pLineCtx    - Line affected
 *  mode        - TRUE = Disable TX/RX, FALSE = enable
 *
 * Preconditions:
 *  The line context must be valid (i.e., pointing to a valid Vp890 line object
 * type).
 *
 * Postconditions:
 *  If mode is TRUE the TX/RX path is cut. If FALSE, the TX/RX path is enabled
 * according to the current line state and mode used for talk states.
 ******************************************************************************/
void
Vp890MuteChannel(
    VpLineCtxType         *pLineCtx,
    bool                  mode)
{
    VpDevCtxType          *pDevCtx  = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    Vp890LineObjectType   *pLineObj = pLineCtx->pLineObj;
    uint8                 ecVal     = pLineObj->ecVal;
    VpDeviceIdType        deviceId  = pDevObj->deviceId;

    uint8                 preState, postState, mpiByte;

    /*
     * Read the status of the Operating Conditions register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     */
    preState = pLineObj->opCond[0];

    postState = preState;
    postState &= ~(VP890_CUT_TXPATH | VP890_CUT_RXPATH);
    postState &= ~(VP890_HIGH_PASS_DIS | VP890_OPCOND_RSVD_MASK);

    /*
     * If disabling, simple. Otherwise enable based on the current line state
     * and the state of the "talk" option. The "talk" option is maintained in
     * the line object and abstracted in GetTxRxMode() function
     */

    Vp890GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState, &mpiByte);

    if (mode == TRUE) {
        /*
         * If awaiting DTMF detection, enable TX, disable RX. This is higher
         * priority than Mute mode. Otherwise, disable both TX and RX.
         */
        postState |= VP890_CUT_RXPATH;  /* Mute == TRUE always cuts RX path */
#ifdef VP_CSLAC_SEQ_EN
        if (!(pLineObj->callerId.status & VP_CID_AWAIT_TONE)) {
#endif
            /* Not awaiting tone, TX Path is disabled as well */
            postState |= VP890_CUT_TXPATH;
#ifdef VP_CSLAC_SEQ_EN
        }
#endif
    } else {
        /*
         * It's possible that a Mute off is occuring because of end of DTMF
         * detection, or end of data generation, or end of Mute period. However,
         * we only need to check if Mute On is still enabled since DTMF
         * detection will not occur while data is being generated.
         */
#ifdef VP_CSLAC_SEQ_EN
        if (pLineObj->callerId.status & VP_CID_MUTE_ON) {
            /*
             * Some "other" operation completed, but we're still in a Mute On
             * period.
             */
            postState |= (VP890_CUT_RXPATH | VP890_CUT_TXPATH);
        } else  {
#endif
            postState |= mpiByte;
#ifdef VP_CSLAC_SEQ_EN
        }
#endif
    }

    if (postState != preState) {
        VP_INFO(VpLineCtxType, pLineCtx, ("\n\r10. Writing 0x%02X to Op Conditions",
            postState));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT, VP890_OP_COND_LEN,
            &postState);
        pLineObj->opCond[0] = postState;
    }
    return;
} /* Vp890MuteChannel() */

/*******************************************************************************
 * Vp890GetTxRxPcmMode()
 *  This function returns the TX/RX PCM bits for the PCM (enable/disable) mode
 * corresponding to the state passed. The results should be or'-ed with the
 * bits set to 0 prior to calling this function.
 *
 * Arguments:
 *  state   - The state associating with PCM mode
 *  mpiByte - Device Specific byte
 *
 * Preconditions:
 *  None. Mapping function only.
 *
 * Postconditions:
 *  None. Mapping function only.
 ******************************************************************************/
VpStatusType
Vp890GetTxRxPcmMode(
    Vp890LineObjectType *pLineObj,
    VpLineStateType     state,
    uint8               *mpiByte)
{
    if (VP_OPTION_PCM_RX_ONLY == pLineObj->pcmTxRxCtrl) {
         *mpiByte = VP890_CUT_TXPATH;
    } else if (VP_OPTION_PCM_TX_ONLY == pLineObj->pcmTxRxCtrl) {
        *mpiByte = VP890_CUT_RXPATH;
    } else if (VP_OPTION_PCM_ALWAYS_ON == pLineObj->pcmTxRxCtrl) {
        *mpiByte = 0x00;
        return VP_STATUS_SUCCESS;
    } else {
        *mpiByte = 0x00;
    }

    switch(state) {
        /* Non-Talk States */
        case VP_LINE_STANDBY:
        case VP_LINE_STANDBY_POLREV:
        case VP_LINE_ACTIVE:
        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_DISCONNECT:
        case VP_LINE_RINGING:
        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP890_IS_FXO) {
                VP_ERROR(Vp890LineObjectType, pLineObj, ("Vp890GetTxRxPcmMode() - Invalid FXO state %02X", state));
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP890_CUT_TXPATH | VP890_CUT_RXPATH);
            break;

        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_RING_GND:
            if (!(pLineObj->status & VP890_IS_FXO)) {
                VP_ERROR(Vp890LineObjectType, pLineObj, ("Vp890GetTxRxPcmMode() - Invalid FXS state %02X", state));
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP890_CUT_TXPATH | VP890_CUT_RXPATH);
            break;

        /* Talk States */
        case VP_LINE_TALK:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT:
        case VP_LINE_OHT_POLREV:
            if (pLineObj->status & VP890_IS_FXO) {
                VP_ERROR(Vp890LineObjectType, pLineObj, ("Vp890GetTxRxPcmMode() - Invalid FXO state %02X", state));
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_TALK:
            if (!(pLineObj->status & VP890_IS_FXO)) {
                VP_ERROR(Vp890LineObjectType, pLineObj, ("Vp890GetTxRxPcmMode() - Invalid FXS state %02X", state));
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_LINE_TIP_OPEN:
            return VP_STATUS_INVALID_ARG;

        default:
            break;
    }
    return VP_STATUS_SUCCESS;
} /* Vp890GetTxRxPcmMode() */

/*******************************************************************************
 * Vp890SetRelGainInt()
 *  This function adjusts the GR and GX values for a given channel of a given
 * device.  It multiplies the profile values by a user-specified factor and
 * possibly a callerID correction factor determined elsewhere in the API.  The
 * DTG bit of the VP_GAIN register will also be set for callerID correction.
 *
 * Arguments:
 *  *pLineCtx  -  Line context to change gains on
 *  rxLevel    -  Adjustment to line's relative Rx level
 *
 * Preconditions:
 *
 * Postconditions:
 *  Returns error if device is not initialized.
 *  GX and GR will be
 ******************************************************************************/
VpStatusType
Vp890SetRelGainInt(
    VpLineCtxType   *pLineCtx)
{
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj        = pLineCtx->pDevCtx->pDevObj;
    VpDeviceIdType          deviceId        = pDevObj->deviceId;
    VpRelGainResultsType    *relGainResults = &pDevObj->relGainResults;
    uint8                   ecVal           = pLineObj->ecVal;
    uint32                  gxInt, grInt;
    uint8                   gainCSD[VP890_GX_GAIN_LEN];
    uint8                   vpGainData;
    uint8                   mpiBuffer[3 + VP890_VP_GAIN_LEN + VP890_GX_GAIN_LEN +
                                      VP890_GR_GAIN_LEN] ;
    uint8                   mpiIndex = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+SetRelGainInt()"));

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    relGainResults->gResult = VP_GAIN_SUCCESS;

    /* Multiply the profile gain values by the user's adjustments. */
    gxInt = (uint32)pLineObj->gxBase * pLineObj->gxUserLevel / 16384;
    grInt = (uint32)pLineObj->grBase * pLineObj->grUserLevel / 16384;

    /* If overflow or underflow occurred, generate out-of-range result. */
    /* Requirement: 1.0 <= gxInt <= 4.0 */
    if ((gxInt < (uint32)0x500) || (gxInt > (uint32)0x10000)) {
        relGainResults->gResult |= VP_GAIN_GX_OOR;
        gxInt = pLineObj->gxBase;
    }
    /* Requirement: 0.25 <= grInt <= 1.0 */
    if ((grInt < (uint32)0x1000) || (grInt > (uint32)0x4000)) {
        relGainResults->gResult |= VP_GAIN_GR_OOR;
        grInt = pLineObj->grBase;
    }

    /* If this function is being called for an on-hook FXO line, we need to
     * apply the caller ID correction factor - a previously computed value
     * that is set to 1 by default - and set the Digital Transmit Gain bit */
    if (pLineObj->status & VP890_IS_FXO) {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN, &vpGainData);
        vpGainData &= ~VP890_DTG_MASK;

        if (pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_OPEN
          || pLineObj->lineState.currentState == VP_LINE_FXO_OHT) {
            gxInt = (uint32)gxInt * pLineObj->gxCidLevel / 16384;
            /* Requirement: 1.0 <= gxInt < 5.0
             * If outside of this range, set it to the limit */
            if (gxInt >= (uint32)0x14000) {
                gxInt = (uint32)0x13FFF;
            } else if (gxInt < (uint32)0x500) {
                gxInt = 0x500;
            }
            /* Apply the DTG value calculated for callerId correction */
            vpGainData |= pLineObj->cidDtg;
        } else {
            vpGainData |= pLineObj->userDtg;
        }
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
            VP890_VP_GAIN_LEN, &vpGainData);
    }
    /*
     * Write adjusted gain values to the device, and remember them for
     * VpGetResults().
     */
    ConvertSignedFixed2Csd((int32)gxInt - 0x4000, gainCSD);
    relGainResults->gxValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];

	/*BEGIN: fix MOTO*/
	if (8 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_GX_GAIN_WRT,
        VP890_GX_GAIN_LEN, gainCSD);

    ConvertSignedFixed2Csd((int32)grInt, gainCSD);
    relGainResults->grValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];

	/*BEGIN: fix MOTO*/
	if (8 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_GR_GAIN_WRT,
        VP890_GR_GAIN_LEN, gainCSD);

    /* send down the mpi commands */
    VpMpiCmdWrapper(deviceId, ecVal, mpiBuffer[0], mpiIndex-1, &mpiBuffer[1]);

    return VP_STATUS_SUCCESS;
} /* Vp890SetRelGainInt() */

/*******************************************************************************
 * ConvertSignedFixed2Csd()
 *  This function returns a four-nibble CSD (canonical signed digit) number
 * whose value matches (as nearly as possible) the supplied signed 2.14
 * fixed-point number.
 *
 * Preconditions:
 *
 * Postconditions:
 *  The CSD number will be placed into a two-byte array (high byte first) at
 * the address specified in the csdBuf parameter.
 ******************************************************************************/
static void
ConvertSignedFixed2Csd(
    int32 fixed,
    uint8 *csdBuf)
{
#define CSD_NIBBLES 4
    uint16 error, power, greaterPower, smallerPower, distGreater, distSmaller;
    uint16 C, m, result;
    int32 sum = 0;
    int8 n, gp, sp;

    /* Data structure for holding the four terms composing the CSD number. */
    typedef struct {
        bool sign;
        int power;
    } term;
    term t[CSD_NIBBLES + 1];

    t[0].power = 0;
    t[0].sign = 0;

    /*
     * Split the 2.14 value into a sum of powers of 2,
     *   s1 * 2^p1  +  s2 * 2^p2  +  s3 * 2^p3  +  s4 * 2^p4
     * where for term x,
     *   sx = 1 or -1,
     *   px <= 0.
     */
    for (n = 1; n <= CSD_NIBBLES; n++) {

        if (sum == fixed) break;

        /*
         * If current sum is less than actual value, then the next term
         * should be added; otherwise the next term should be
         * subtracted.
         */
        if (sum < fixed) {
            t[n].sign = 0;
            error = fixed - sum;
        } else {
            t[n].sign = 1;
            error = sum - fixed;
        }

        /* If error > 1, then term = +/-1. */
        if (error > 0x4000) {
            t[n].power = 0;
        } else {

            /*
             * Calculate greaterPower = the smallest power of 2 greater
             * than error.  Calculate smallerPower = the largest power
             * of 2 less than error.
             */
            greaterPower = 0x4000; gp = 0;
            for (power = 0x2000; power > error; power >>= 1) {
                greaterPower >>= 1; gp--;
            }
            smallerPower = greaterPower >> 1; sp = gp - 1;

            /*
             * Is error closer to greaterPower or smallerPower?
             * Whichever is closer, choose that for the value of the
             * next term.
             */
            distGreater = greaterPower - error;
            distSmaller = error - smallerPower;
            if (distGreater < distSmaller) {
                t[n].power = gp;
            } else {
                t[n].power = sp;
            }

            /*
             * The power of this term can differ from the power of the
             * previous term by no more than 7.
             */
            if (t[n - 1].power - t[n].power > 7) {
                t[n].power = t[n - 1].power - 7;
            }
        }

        /* Add or subtract the term to the sum, depending on sign. */
        if (t[n].sign == 0) {
            sum += (uint16)1 << (14 + t[n].power);
        } else {
            sum -= (uint16)1 << (14 + t[n].power);
        }
    }

    /*
     * If we reached the exact value with terms left over, fill these
     * extra terms with dummy values which don't affect the CSD value.
     */
    while (n <= CSD_NIBBLES) {
        if (n == 1) {
            t[1] = t[0];
            t[2].power = 0;
            t[2].sign = 1;
            n += 2;
        } else {
            /*
             * Increase the number of terms by replacing the last term
             * with two new terms whose sum is the old term.
             */
            if (t[n - 1].power == t[n - 2].power) {
                t[n - 1].power--;
                t[n] = t[n - 1];
            } else {
                t[n] = t[n - 1];
                t[n - 1].power++;
                t[n].sign = !(t[n - 1].sign);
            }
            n++;
        }
    }

    /* Compute nibble values from the terms. */
    result = 0;
    for (n = 1; n <= CSD_NIBBLES; n++) {
        int8 bitPos = (n - 1) * 4;
        C = (t[n].sign != t[n - 1].sign);
        m = -(t[n].power - t[n - 1].power);
        result |= (C << (bitPos + 3)) | (m << bitPos);
    }

    /* Split the uint16 result into high and low bytes. */
    csdBuf[0] = (uint8)(result >> 8);
    csdBuf[1] = (uint8)(result & 0xFF);
} /* ConvertFixed2Csd() */


/*******************************************************************************
 * Vp890ProtectedWriteICR1()
 *  This function is a wrapper for writing to ICR1.  If the internal test
 * termination is applied, ICR1 must not be changed, so this function copies
 * the data into the line object cache and returns without writing anything to
 * the device.  If the internal test termination is not applied, the write
 * is performed.
 *
 * Returns TRUE if ICR1 is currently being protected. Meaning, the ICR1 values
 * are cached, but not actually written to the device.
 *
 * Note: This function must be called from within a critical section.
 ******************************************************************************/
bool
Vp890ProtectedWriteICR1(
    Vp890LineObjectType *pLineObj,
    VpDeviceIdType deviceId,
    uint8 *icr1Values,
    bool writeToDevice)
{
    VpMemCpy(pLineObj->icr1Values, icr1Values, VP890_ICR1_LEN);

    if (pLineObj->internalTestTermApplied == TRUE) {
        return TRUE;
    } else {
        if (writeToDevice == TRUE) {
            VpMpiCmdWrapper(deviceId, pLineObj->ecVal, VP890_ICR1_WRT,
                VP890_ICR1_LEN, icr1Values);
        }
    }
    return FALSE;
}

#endif /* VP_CC_890_SERIES */
