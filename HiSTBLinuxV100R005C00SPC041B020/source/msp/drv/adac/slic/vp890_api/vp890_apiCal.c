/** \file apiCal.c
 * apiCal.c
 *
 * This file contains the line and device calibration functions for
 * the Vp890 device API.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7464 $
 * $LastChangedDate: 2010-07-07 15:02:33 -0500 (Wed, 07 Jul 2010) $
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_890_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_api.h"
#include "vp_api_int.h"
#include "vp890_api.h"
#include "vp890_api_int.h"
#include "vp_hal.h"
#include "sys_service.h"

/* Functions that are called only inside this file. */
#if defined (VP890_FXS_SUPPORT) && defined (VP_CSLAC_RUNTIME_CAL_ENABLED)
static void
Vp890CalInit(
    VpLineCtxType *pLineCtx);

static void
Vp890AbvInit(
    VpLineCtxType *pLineCtx);

static void
Vp890AbvSetAdc(
    VpLineCtxType *pLineCtx);

static void
Vp890AbvMeasure(
    VpLineCtxType *pLineCtx);

static int16
Vp890AdcSettling(
    Vp890DeviceObjectType *pDevObj,
    uint8 ecVal,
    uint8 adcConfig);

/* Functions for Cal Line */
static VpStatusType
Vp890CalAbv(
     VpLineCtxType *pLineCtx);

static void
Vp890CalLineInit(
     VpLineCtxType *pLineCtx);

static VpStatusType
Vp890CalVas(
    VpLineCtxType *pLineCtx);

static void
Vp890VasPolRev(
    VpLineCtxType *pLineCtx);

static VpStatusType
Vp890CalVoc(
    VpLineCtxType *pLineCtx);

static VpStatusType
Vp890CalVag(
    VpLineCtxType *pLineCtx);

static VpStatusType
Vp890CalVbg(
    VpLineCtxType *pLineCtx);

static bool
Vp890VocMeasure(
    VpLineCtxType *pLineCtx);

static bool
Vp890VagMeasure(
    VpLineCtxType *pLineCtx);

static bool
Vp890VbgMeasure(
    VpLineCtxType *pLineCtx);

static bool
Vp890VocMeasureInvert(
    VpLineCtxType *pLineCtx);

static bool
Vp890VocMeasureInvert2(
    VpLineCtxType *pLineCtx);

static bool
Vp890VagMeasureInvert(
    VpLineCtxType *pLineCtx);

static bool
Vp890VbgMeasureInvert(
    VpLineCtxType *pLineCtx);

static void
Vp890CalDone(
    VpLineCtxType *pLineCtx);

static void
Vp890VocInit(
    VpLineCtxType *pLineCtx);

static void
Vp890VagInit(
    VpLineCtxType *pLineCtx);

static void
Vp890VbgInit(
    VpLineCtxType *pLineCtx);

static void
Vp890VocSetAdc(
    VpLineCtxType *pLineCtx);

static void
Vp890VagSetAdc(
    VpLineCtxType *pLineCtx);

static void
Vp890VbgSetAdc(
    VpLineCtxType *pLineCtx);

static void
Vp890VocOffset(
    VpLineCtxType *pLineCtx);

static void
Vp890SigGenOffset(
    VpLineCtxType *pLineCtx);

void
Vp890VocOffset2(
    VpLineCtxType *pLineCtx);

static void
Vp890VagOffset(
    VpLineCtxType *pLineCtx);

static void
Vp890VbgOffset(
    VpLineCtxType *pLineCtx);

static void
Vp890VocDone(
    VpLineCtxType *pLineCtx);

static void
Vp890VagDone(
    VpLineCtxType *pLineCtx);

static void
Vp890VbgDone(
    VpLineCtxType *pLineCtx);
#endif /* (VP890_FXS_SUPPORT) && defined (VP_CSLAC_RUNTIME_CAL_ENABLED) */

#ifdef VP890_FXO_SUPPORT
static VpStatusType
Vp890CalBFilter(
    VpLineCtxType       *pLineCtx,
    void                *inputArgs);

static VpStatusType
Vp890CalApplyBFilter(
    VpLineCtxType       *pLineCtx,
    void                *inputArgs);

static VpStatusType
Vp890CalMeasureBFilter(
    VpLineCtxType       *pLineCtx);

#ifdef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL
static void
Vp890ReduceNoiseOutput(
    uint8 oldVpGain,
    VpDeviceIdType deviceId,
    uint8 ecVal);
#endif /* VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL */
#endif /* VP890_FXO_SUPPORT */

#if defined (VP890_FXS_SUPPORT) && defined (VP_CSLAC_RUNTIME_CAL_ENABLED)
/**
 * Vp890CalLine()
 *  This function initiates a calibration operation for analog circuits
 * associated with a given line. See VP-API reference guide for more
 * information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp890CalLine(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint16 tickRate = pDevObj->devProfileData.tickRate;
    VpLineStateType currentState = pLineObj->lineState.usrCurrent;
    uint8 ecVal = pLineObj->ecVal;
    uint8 opCond[VP890_OP_COND_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalLine()"));

    /* Block if device init is not complete */
    if (!(pDevObj->state & VP_DEV_INIT_CMP)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP890_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->state & VP_DEV_IN_CAL) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Return NOT_INITIALIZED from Vp890CalLine()"));
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalLine()"));
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Running Cal Line on Channel %d at time %d target 0x%02X 0x%02X",
        pLineObj->channelId, pDevObj->timeStamp, pLineObj->calLineData.dcFeedRef[0], pLineObj->calLineData.dcFeedRef[1]));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * Don't run calibration if previously complete or provided with calibration
     * coefficients.
     */
    if (pDevObj->stateInt & VP890_SYS_CAL_COMPLETE) {
        pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
        pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Calibration Previously Done. Cal Line Complete"));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* Make sure line calibration can be run */
    switch(currentState) {
        case VP_LINE_STANDBY:
        case VP_LINE_STANDBY_POLREV:
        case VP_LINE_OHT_POLREV:
            pLineObj->lineState.calType =  VP_CSLAC_CAL_NONE;
            Vp890SetFxsLineState(pLineCtx, VP_LINE_OHT);
            break;

        case VP_LINE_OHT:
            break;

        default:
            /* Mark the device as NOT in-calibration */
            pDevObj->state &= ~VP_DEV_IN_CAL;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalLine()"));
            return VP_STATUS_INVALID_ARG;
    }

    pLineObj->calLineData.reversePol = FALSE;

#ifdef VP890_LP_SUPPORT
    /*
     * Force a LPM Exit update before proceeding with Calibration. If this
     * isn't done before setting the device mask, the LPM code to exit LPM will
     * not run (because it otherwise does not touch the device while in
     * calibration). If it is not a LPM device, this function doesn't do
     * anything.
     */
    Vp890LowPowerMode(pDevCtx);
#endif

    /* Mark the device as in-calibration */
    pDevObj->state |= VP_DEV_IN_CAL;

/* This delay could be reduced to LPM max, which is ~400ms. */
#define VP890_MIN_CAL_WAIT_TIME (500)

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_MIN_CAL_WAIT_TIME, tickRate);
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
        VP_ACTIVATE_TIMER;

    Vp890CalLineInit(pLineCtx);

    /* Reprogram the Operating Conditions Register, affected by Set Line State */
    opCond[0] = (VP890_CUT_TXPATH | VP890_CUT_RXPATH | VP890_HIGH_PASS_DIS);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT, VP890_OP_COND_LEN,
        opCond);
    pLineObj->opCond[0] = opCond[0];

    pLineObj->calLineData.calState =  VP890_CAL_INIT;
    pLineObj->lineState.calType = VP_CSLAC_CAL_ABV;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalLine()"));

    return VP_STATUS_SUCCESS;
}

/**
 * Vp890CalAbv()
 *  This function initiates a calibration operation for Absolute Switcher
 * circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information. SWYV SWZV are global for every Channels
 * Line must be in Disconnect state before to start the Calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp890CalAbv(
     VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType  status = VP_STATUS_SUCCESS;
    bool calCleanup = FALSE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalAbv()"));

    if (pLineObj->calLineData.calState == VP890_CAL_INIT
        || pLineObj->calLineData.calState == VP890_CAL_EXIT) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalAbv:  - Setting to Vp890CalInit"));

        pLineObj->calLineData.calState = VP890_CAL_INIT;
        Vp890CalInit(pLineCtx);
    }

    switch(pLineObj->calLineData.calState) {
        case VP890_CAL_INIT:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalAbv: - Running Vp890AbvInit"));
            Vp890AbvInit(pLineCtx);
            break;

        case VP890_CAL_ADC:
        case VP890_CAL_STATE_CHANGE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVp890CalAbv: - Running Vp890AbvSetAdc"));
            Vp890AbvSetAdc(pLineCtx);
            break;

        case VP890_CAL_MEASURE:
        case VP890_CAL_CONVERTER_CHANGE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVp890CalAbv - Running Vp890AbvMeasure"));
            Vp890AbvMeasure(pLineCtx);
            break;

        case VP890_CAL_DONE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nABV Cal Done"));
            calCleanup = TRUE;
            pDevObj->responseData = VP_CAL_SUCCESS;
            break;

        case VP890_CAL_ERROR:
            /* Fall through intentional */
        default:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVp890CalAbv: ERROR - Cal Done"));
            calCleanup = TRUE;
            status = VP_STATUS_FAILURE;
            pDevObj->responseData = VP_CAL_FAILURE;
            break;
    }

    if (calCleanup == TRUE) {
        pLineObj->calLineData.calState =  VP890_CAL_INIT;
        pLineObj->lineState.calType = VP_CSLAC_CAL_VOC;

        /* Restore internal switcher parameters */
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. Regulator Timing 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
            pDevObj->devProfileData.timingParams[0],
            pDevObj->devProfileData.timingParams[1],
            pDevObj->devProfileData.timingParams[2],
            pDevObj->devProfileData.timingParams[3],
            pDevObj->devProfileData.timingParams[4],
            pDevObj->devProfileData.timingParams[5]));

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_REGULATOR_TIMING_WRT,
            VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParams);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE((VP890_CAL_SET),
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalAbv()"));

    return status;
}

/**
 * Vp890AbvInit()
 *  This function initiates a calibration operation for ABV associated with all
 * the lines of a device.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890AbvInit(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 disnVal[VP890_DISN_LEN] = {0x00};
    uint8 vpGain[VP890_VP_GAIN_LEN] = {0x00};
    uint8 isrpMods[VP890_REGULATOR_TIMING_LEN] = {
        0x00, 0x40, 0x00, 0x40, 0x00, 0x40
    };

    uint8 swYZ[VP890_REGULATOR_PARAM_LEN];

    uint8 data;
    uint8 swCal[VP890_BAT_CALIBRATION_LEN];

    uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_SWITCHER_Y};

    uint8 mpiBuffer[8 + VP890_BAT_CALIBRATION_LEN + VP890_REGULATOR_PARAM_LEN +
                        VP890_SYS_STATE_LEN + VP890_DISN_LEN + VP890_REGULATOR_TIMING_LEN +
                        VP890_DC_CAL_REG_LEN + VP890_CONV_CFG_LEN + VP890_VP_GAIN_LEN];
    uint8 mpiIndex = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890AbvInit()"));

    /* Channel specific registers to restore at end of calibration */
    VpMemCpy(pLineObj->calLineData.icr2, pLineObj->icr2Values, VP890_ICR2_LEN);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Reading 0x%02X 0x%02X 0x%02X 0x%02X from ICR2 on Channel %d",
        pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
        pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
        pLineObj->channelId));

    /* Save off current slic state */
    pLineObj->calLineData.sysState[0] = pLineObj->slicValueCache;

    /*
     * Copy the Ringing Voltage to the Floor Voltage, everything else
     * directly from the device profile
     */
    swYZ[0] = pDevObj->devProfileData.swParams[0];

    swYZ[VP890_SWY_LOCATION] =
        (pDevObj->devProfileData.swParams[VP890_SWZ_LOCATION]
        & VP890_VOLTAGE_MASK);
    swYZ[VP890_SWY_LOCATION] |=
        (pDevObj->devProfileData.swParams[VP890_SWY_LOCATION]
        & ~VP890_VOLTAGE_MASK);

    swYZ[2] = pDevObj->devProfileData.swParams[2];

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Vp890AbvInit: swYZ: 0x%02X 0x%02X 0x%02X",
        swYZ[0], swYZ[1], swYZ[2]));

    /* Clear existing battery calibration correction factors */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_BAT_CALIBRATION_RD,
        VP890_BAT_CALIBRATION_LEN, swCal);
    swCal[0] &= ~(VP890_BAT_CAL_SWCAL_MASK);

    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_BAT_CALIBRATION_WRT,
        VP890_BAT_CALIBRATION_LEN, swCal);

    /* Set Floor Voltage to Target Ringing Voltage */
    VpMemCpy(pDevObj->swParamsCache, swYZ, VP890_REGULATOR_PARAM_LEN);
	
	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_REGULATOR_PARAM_WRT,
        VP890_REGULATOR_PARAM_LEN, swYZ);

    /* Setup channel for sense read */
    data = VP890_SS_ACTIVE;
    pLineObj->slicValueCache = data;

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
        VP890_SYS_STATE_LEN, &data);

    /* Set DISN = 0 and Voice Path Gain = 0dB TX */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: DISN initialized to 0x%02X VP Gain to 0x%02X",
        pLineObj->channelId, disnVal[0], vpGain[0]));

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DISN_WRT,
        VP890_DISN_LEN, disnVal);

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/	
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
        VP890_VP_GAIN_LEN, vpGain);

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    /* Disable switcher by setting duty cycle = 0. */
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_REGULATOR_TIMING_WRT,
        VP890_REGULATOR_TIMING_LEN, isrpMods);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Disable Switchers"));

    /* Disconnect SWVS pin and measure SWVS offset */
    pLineObj->dcCalValues[0] = 0x00;
    pLineObj->dcCalValues[1] = (VP890_C_YBAT_SNS_CUT | VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DC_CAL_REG_WRT,
        VP890_DC_CAL_REG_LEN, pLineObj->dcCalValues);

	/*BEGIN: fix MOTO*/
	if (25 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
        VP890_CONV_CFG_LEN, adcConfig);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
        mpiIndex-1, &mpiBuffer[1]);

    /* Wait at least 100ms before collecting data */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_ABV_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    /* Advance state to measure ADC offset */
    pLineObj->calLineData.calState = VP890_CAL_ADC;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvInit()"));

} /* end Vp890AbvInit */

/**
 * Vp890AbvSetAdc ()
 *  This function set the converter to read the right pcm set the right state
 * machine
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 */
void
Vp890AbvSetAdc(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_SWITCHER_Y};

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890AbvSetAdc()"));

    if(pLineObj->calLineData.convCfgChangeReq == TRUE) {
        pLineObj->calLineData.convCfgChangeReq = FALSE;
        pDevObj->vp890SysCalData.swyOffset[0] = 0;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvSetAdc()"));
        return;
    }

    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN,
        adcConfig);

    /*
     * The converter configuration can change by the device if the supplies are
     * still collapsing. So check before we assume they're correct for offset
     * measurement. Try a few times (seperated by ~100ms per attempt) but then
     * give up and force offset = 0V.
     */
    if (adcConfig[0] != VP890_SWITCHER_Y) {
        pDevObj->vp890SysCalData.swyOffset[0]++;

        if (pDevObj->vp890SysCalData.swyOffset[0] < 3) {
            uint8 adcCorrection[] = {VP890_SWITCHER_Y};
            VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT,
                VP890_CONV_CFG_LEN, adcCorrection);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_ABV_LONG,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvSetAdc()"));
            return;
        } else {
            pDevObj->vp890SysCalData.swyOffset[0] = 0;
        }
    } else {
        /* Converter is correct. Read SWY offset voltage */
        pDevObj->vp890SysCalData.swyOffset[0] =
            Vp890AdcSettling(pDevObj, VP890_EC_CH1, VP890_SWITCHER_Y);
    }

    /* This is done to be compatible with VVA P1.1.0 */
    pLineObj->calLineData.typeData.abvData.swyOffset[0] =
        pDevObj->vp890SysCalData.swyOffset[0];

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan 0 Offset: SWY %d",
        ((pDevObj->vp890SysCalData.swyOffset[0] * 7324) / 10000)));

    /* Reconnect Sense pins */
    pLineObj->dcCalValues[0] = 0x00;
    pLineObj->dcCalValues[1] = 0x00;
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Re-enable supply */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("4. Regulator Timing 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
        pDevObj->devProfileData.timingParams[0], pDevObj->devProfileData.timingParams[1],
        pDevObj->devProfileData.timingParams[2], pDevObj->devProfileData.timingParams[3],
        pDevObj->devProfileData.timingParams[4], pDevObj->devProfileData.timingParams[5]));

    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_REGULATOR_TIMING_WRT,
        VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParams);

    /*
     * Things will take time to settle before converter configuration will
     * take hold. It will change otherwise automatically as a result of
     * hook detection change
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_ABV_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    pLineObj->calLineData.calState = VP890_CAL_CONVERTER_CHANGE;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvSetAdc()"));

} /* end Vp890AbvSetAdc  */

/**
 * Vp890AbvMeasure()
 *  This function read switcher value and compare with the value read from the
 * pcm data if the value is bigger than 1.25v this function will make a
 * correction  voltage.
 */
void
Vp890AbvMeasure(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    int32 abvError, abvTarget;

    uint8 mpiBuffer[3 + VP890_REGULATOR_TIMING_LEN + VP890_REGULATOR_PARAM_LEN +
                    VP890_DISN_LEN];
    uint8 mpiIndex = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890AbvMeasure()"));

    if(pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_SWITCHER_Y};

        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvMeasure()"));
        return;
    }

    pLineObj->calLineData.calSet =
        pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE];

    /* Read SWY voltages */
    pLineObj->calLineData.typeData.abvData.swyVolt[0] =
        Vp890AdcSettling(pDevObj, VP890_EC_CH1, VP890_SWITCHER_Y);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan 0 Voltage: SWY %d",
        ((pLineObj->calLineData.typeData.abvData.swyVolt[0] * 7324) / 10000)));

    /* Now have all data necessary to compute error and adjust channel 0 */
    abvTarget = (pLineObj->calLineData.calSet * 5) + 5;   /* Gets it to V scale */
    abvTarget *= 1000;
    abvTarget *= 1000;
    abvTarget /= 7324;   /* Now we're scaled to the PCM data */

    abvError = abvTarget -
        (pLineObj->calLineData.typeData.abvData.swyVolt[0]
       - pDevObj->vp890SysCalData.swyOffset[0]);

    pDevObj->vp890SysCalData.abvError[0] = (((int16)abvError * VP890_V_PCM_LSB) / 10000);

    Vp890BatteryAdjust(pLineCtx);

    /* Device internal switcher parameters */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. Regulator Timing 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
        pDevObj->devProfileData.timingParams[0],
        pDevObj->devProfileData.timingParams[1],
        pDevObj->devProfileData.timingParams[2],
        pDevObj->devProfileData.timingParams[3],
        pDevObj->devProfileData.timingParams[4],
        pDevObj->devProfileData.timingParams[5]));

    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_REGULATOR_TIMING_WRT,
        VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParams);

    /* Restore Switching Regulator Parameters */
    VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
        VP890_REGULATOR_PARAM_LEN);

	/*BEGIN: fix MOTO*/
	if (13 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_REGULATOR_PARAM_WRT,
        VP890_REGULATOR_PARAM_LEN, pDevObj->devProfileData.swParams);

    /* send down the mpi commands */
    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1, &mpiBuffer[1]);

    pLineObj->calLineData.calState = VP890_CAL_DONE;

    /*
     * Things will take time to settle before proceeding with remaining cal
     * steps.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_ABV_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890AbvMeasure()"));
    return;
} /*end Vp890AbvMeasure */

/**
 * Vp890CalInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890CalInit(
     VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data, mpiIndex = 0;
    uint8 mpiBuffer[3 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN + VP890_DEV_MODE_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalInit()"));

    /* Set for Linear Mode and disable AC Coefficients */
    data = VP890_LINEAR_CODEC;
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, &data);

    /* Cut TX/RX PCM and disable HPF */
    pLineObj->opCond[0] = (VP890_CUT_TXPATH | VP890_CUT_RXPATH | VP890_HIGH_PASS_DIS);

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, pLineObj->opCond);

    /* Device Mode */
    pDevObj->devMode[0] &= ~(VP890_DEV_MODE_TEST_DATA);

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DEV_MODE_WRT,
        VP890_DEV_MODE_LEN, pDevObj->devMode);

    /* send down the mpi commands */
    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1, &mpiBuffer[1]);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalInit()"));
}

/**
 * Vp890CalLineInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890CalLineInit(
     VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data, mpiIndex = 0;
    uint8 mpiBuffer[5 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN + VP890_DISN_LEN +
                        VP890_DEV_MODE_LEN + VP890_VP_GAIN_LEN];
    uint8 disnVal[VP890_DISN_LEN] = {0x00};
    uint8 vpGain[VP890_VP_GAIN_LEN] = {0x00};

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalLineInit()"));

    /*
     * Clear flag to indicate Calibration is NOT done. Init calibration type
     * (i.e., what algorithm to start with) and state (step inside the starting
     * algorithm.
     */
    pLineObj->calLineData.calDone = FALSE;
    pLineObj->calLineData.calState = VP890_CAL_INIT;

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DISN_RD, VP890_DISN_LEN,
        pLineObj->calLineData.disnVal);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Read 0x%02X on Channel %d in DISN",
        pLineObj->calLineData.disnVal[0], pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN,
        pLineObj->calLineData.vpGain);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Read 0x%02X on Channel %d in VP GAIN",
        pLineObj->calLineData.vpGain[0], pLineObj->channelId));

     /* Save PCM For Linear Mode and disable AC Coefficients */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN,
        &pLineObj->calLineData.codecReg);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Read 0x%02X on Channel %d in OP Func",
        pLineObj->calLineData.codecReg, pLineObj->channelId));

    /* Set compression to Linear Mode */
    data = VP890_LINEAR_CODEC;
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, &data);

    /* Cut TX/RX PCM and disable HPF */
    pLineObj->opCond[0] = (VP890_CUT_TXPATH | VP890_CUT_RXPATH | VP890_HIGH_PASS_DIS);

	/*BEGIN: fix MOTO*/
	if (10 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, pLineObj->opCond);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. Writing fixed 0x%02X from Channel %d to DISN",
        disnVal[0], pLineObj->channelId));

	/*BEGIN: fix MOTO*/
	if (10 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DISN_WRT,
        VP890_DISN_LEN, disnVal);

	/*BEGIN: fix MOTO*/
	if (10 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
        VP890_VP_GAIN_LEN, vpGain);

    /* Device Mode */
    pDevObj->devMode[0] &= ~(VP890_DEV_MODE_TEST_DATA);

	/*BEGIN: fix MOTO*/
	if (10 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DEV_MODE_WRT,
        VP890_DEV_MODE_LEN, pDevObj->devMode);

    /* send down the mpi commands */
    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1, &mpiBuffer[1]);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalLineInit()"));
}

/**
 * Vp890AdcSettling()
 *  This function read ADC/PCM and set the converter register
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function return the value pcm red.
 */
int16
Vp890AdcSettling(
    Vp890DeviceObjectType *pDevObj,
    uint8 ecVal,
    uint8 adcConfig)
{
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 xdataTemp[VP890_TX_PCM_DATA_LEN];
    int16 tempNew;
    uint8 loopCnt;

    VP_API_FUNC_INT(VpDeviceIdType, &deviceId, ("+Vp890AdcSettling()"));

    /*
     * If the device mode was changed (by other channel), need to change it back
     * so data can be taken. But then need to wait for 1 8KHz sample to update
     * PCM buffer which is taken care of in "for" loop when data is collected.
     */
    if (pDevObj->devMode[0] & VP890_DEV_MODE_TEST_DATA) {
        pDevObj->devMode[0] &= ~(VP890_DEV_MODE_TEST_DATA);
        VpMpiCmdWrapper(deviceId, ecVal, VP890_DEV_MODE_WRT, VP890_DEV_MODE_LEN,
            pDevObj->devMode);
    }

    /*
     * In a future release, this step will be removed as it should not be
     * necessary. The SW cannot really set the Converter Config and immediately
     * take a measurement. "Higher Level" steps should set the converter, wait,
     * then take the measurement.
     */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
        &adcConfig);

    /*
     * Set cconverter to measured point and make sure 125us passes before taking
     * final measurement (time at 8KHz to update data). Make sure the "old" PCM
     * data has been read out.
     */
    for (loopCnt = 0; loopCnt < 2; loopCnt++) {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN,
            xdataTemp);
        tempNew = ( (xdataTemp[0] << 8) | xdataTemp[1]);
        VP_CALIBRATION(None, NULL,("Vp890AdcSettling:LOOP: AdcPcm %d ecVal %d",
            tempNew, ecVal));
    }

    VP_API_FUNC_INT(VpDeviceIdType, &deviceId, ("-Vp890AdcSettling()"));
    return tempNew;
}

/**
 * Vp890CalLineInt()
 *  This function initiates a calibration operation for analog circuits
 * associated with a given line. See VP-API reference guide for more
 * information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp890CalLineInt(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalLineInt()"));

    switch(pLineObj->lineState.calType) {
        case VP_CSLAC_CAL_ABV:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nRunning ABV Calibration on line %d",
                pLineObj->channelId));
            Vp890CalAbv(pLineCtx);
            break;

        case VP_CSLAC_CAL_VOC:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nRunning VOC Calibration on line %d",
                pLineObj->channelId));
            Vp890CalVoc(pLineCtx);
            break;

        case VP_CSLAC_CAL_VAG:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nRunning VAG Calibration on line %d",
                pLineObj->channelId));
            Vp890CalVag(pLineCtx);
            break;

        case VP_CSLAC_CAL_VBG:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nRunning VBG Calibration on line %d",
                pLineObj->channelId));
            Vp890CalVbg(pLineCtx);
            break;

        case VP_CSLAC_CAL_VAS:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nRunning VAS Calibration on line %d",
                pLineObj->channelId));
            Vp890CalVas(pLineCtx);
            break;

        default:
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
            pLineObj->calLineData.calDone = TRUE;
            break;
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalLineInt()"));

    return VP_STATUS_SUCCESS;
}

/**
 * Vp890VocMeasure()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VocMeasure(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 imtValue = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocMeasure()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocMeasure()"));
        return TRUE;
    }

    if (pLineObj->calLineData.reversePol == FALSE) {
        /* Read the metallic current. */
        imtValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. ILA OffsetNorm (10uA) %d",
            (int16)(imtValue * 100 / VP890_ILA_SCALE_1MA)));
        pDevObj->vp890SysCalData.ilaOffsetNorm[pLineObj->channelId] = imtValue;

        /* This is to be compatible with VVA P1.1.0 */
        pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm = imtValue;
    } else {
        /*
         * The error in reverse polarity is less than what can actually be
         * measured. So it's better to ignore what is being read and assume
         * the part is "perfect".
         */
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILA OffsetRev 0"));
    }

    /* We're changing the polarity. So set flag as it will be. */
    if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Normal"));
        pLineObj->calLineData.reversePol = FALSE;
        pLineObj->slicValueCache &= ~VP890_SS_POLARITY_MASK;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Reverse"));

        pLineObj->calLineData.reversePol = TRUE;
        pLineObj->slicValueCache |= VP890_SS_POLARITY_MASK;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
        &pLineObj->slicValueCache);

    /* Re-Disable TRDC Sense */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to Next State */
    pLineObj->calLineData.calState = VP890_CAL_INVERT_POL;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Need to wait for line to settle before changing converter. Time is
     * based on polarity change which is "known" in API-II.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocMeasure()"));

    return TRUE;
} /* end Vp890VocMeasure */

/**
 * Vp890VagMeasure()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VagMeasure(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 ilgValue = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagMeasure()"));

    if (pLineObj->calLineData.reversePol == FALSE) {
        /* Read the longitudinal current. */
        ilgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_LONGITUDINAL_DC_I);
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG OffsetNorm %d", ilgValue));
        pDevObj->vp890SysCalData.ilgOffsetNorm[pLineObj->channelId] = ilgValue;
    } else {
        /*
         * The error in reverse polarity is less than what can actually be
         * measured. So it's better to ignore what is being read and assume
         * the part is "perfect".
         */
    }

    /* We're changing the polarity. So set flag as it will be. */
    if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Normal"));
        pLineObj->calLineData.reversePol = FALSE;
        pLineObj->slicValueCache &= ~VP890_SS_POLARITY_MASK;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Reverse"));
        pLineObj->calLineData.reversePol = TRUE;
        pLineObj->slicValueCache |= VP890_SS_POLARITY_MASK;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
        &pLineObj->slicValueCache);

    /* Re-Disable TRDC Sense */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to Next State */
    pLineObj->calLineData.calState = VP890_CAL_INVERT_POL;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Need to wait for line to settle before changing converter. Time is
     * based on polarity change which is "known" in API-II.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagMeasure()"));

    return TRUE;
} /* end Vp890VagMeasure */

/**
 * Vp890VbgMeasure()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VbgMeasure(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgMeasure()"));

    /* We're changing the polarity. So set flag as it will be. */
    if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Normal"));
        pLineObj->calLineData.reversePol = FALSE;
        pLineObj->slicValueCache &= ~VP890_SS_POLARITY_MASK;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Reverse"));
        pLineObj->calLineData.reversePol = TRUE;
        pLineObj->slicValueCache |= VP890_SS_POLARITY_MASK;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
        &pLineObj->slicValueCache);

    /* Re-Disable TRDC Sense */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to Next State */
    pLineObj->calLineData.calState = VP890_CAL_INVERT_POL;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Need to wait for line to settle before changing converter. Time is
     * based on polarity change which is "known" in API-II.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgMeasure()"));

    return TRUE;
} /* end Vp890VbgMeasure */

/**
 * Vp890VocMeasureInvert()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VocMeasureInvert(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    uint8 channelId = pLineObj->channelId;
    int16 imtValue, vocValue;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocMeasureInvert()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocMeasureInvert()"));
        return TRUE;
    }

    if (pLineObj->calLineData.calState == VP890_CAL_INVERT_POL) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_V};

        /* Read the metallic current. */
        imtValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->calLineData.calState = VP890_CAL_INVERT_POL_VOC;

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

        if (pLineObj->calLineData.reversePol == FALSE) {
            pLineObj->calLineData.typeData.ilaData.ilaNorm = imtValue;
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. ILA Norm %d", imtValue));
        } else {
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. ILA Rev Data Not Used"));
        }
        return TRUE;
    } else {
        /* Read the metallic voltage offset. */
        vocValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_V);
        if (pLineObj->calLineData.reversePol == FALSE) {
            pDevObj->vp890SysCalData.vocOffset[channelId][VP890_NORM_POLARITY] = vocValue;
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Norm Offset %d", vocValue));

            /* This is to be backward compatible with VVA P1.1.0 */
            pLineObj->calLineData.typeData.vocData.vocOffsetNorm = vocValue;
        } else {
            pDevObj->vp890SysCalData.vocOffset[channelId][VP890_REV_POLARITY] = vocValue;
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Rev Offset %d", vocValue));

            /* This is to be backward compatible with VVA P1.1.0 */
            pLineObj->calLineData.typeData.vocData.vocOffsetRev = vocValue;
        }
    }

    /* Re-Enable TRDC Sense */
    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to Pre-Final State */
    pLineObj->calLineData.calState = VP890_CAL_DONE_PRE;

    /* Need to wait a short time before changing ICR2 */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocMeasureInvert()"));

    return TRUE;
} /* end Vp890VocMeasureInvert */

/**
 * Vp890VocMeasureInvert2()
 */
static bool
Vp890VocMeasureInvert2(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocMeasureInvert2()"));

    /* Return to VOC feed, speedup enabled */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restoring 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
        pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->calLineData.icr2);
    VpMemCpy(pLineObj->icr2Values, pLineObj->calLineData.icr2, VP890_ICR2_LEN);

    /* Advance to Final State */
    pLineObj->calLineData.calState = VP890_CAL_DONE;

    /*
     * Need to wait for line to settle before changing converter for final
     * measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    pLineObj->calLineData.convCfgChangeReq = TRUE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocMeasureInvert2()"));

    return TRUE;
} /* end Vp890VocMeasureInvert2 */

/**
 * Vp890VagMeasureInvert()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VagMeasureInvert(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 ilgValue, vagValue;
    int16 vagStaticOffset = 205;    /* static offset of 1.5V on 890 */

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagMeasureInvert()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_LONGITUDINAL_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagMeasureInvert()"));
        return TRUE;
    } else {
        if (pLineObj->calLineData.calState == VP890_CAL_INVERT_POL) {
            uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_TIP_TO_GND_V};

            /* Read the metallic current and voltage. */
            ilgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_LONGITUDINAL_DC_I);
            if (pLineObj->calLineData.reversePol == FALSE) {
                pLineObj->calLineData.typeData.ilaData.ilgNorm = ilgValue;
            }

            VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
                adcConfig);

            pLineObj->calLineData.calState = VP890_CAL_INVERT_POL_VAG_TGND;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagMeasureInvert()"));
            return TRUE;
        } else {
            vagValue = Vp890AdcSettling(pDevObj, ecVal, VP890_TIP_TO_GND_V);
            /* remove static offset of 1.5V on 890 */
            vagValue += vagStaticOffset;
            if (pLineObj->calLineData.reversePol == FALSE) {
                pDevObj->vp890SysCalData.vagOffsetNorm[pLineObj->channelId] = vagValue;

                /* This is to be backward compatible with VVA P1.1.0 */
                pLineObj->calLineData.typeData.vagData.vagOffsetNorm = vagValue;
            } else {
                pDevObj->vp890SysCalData.vagOffsetRev[pLineObj->channelId] = vagValue;

                /* This is to be backward compatible with VVA P1.1.0 */
                pLineObj->calLineData.typeData.vagData.vagOffsetRev = vagValue;
            }
        }
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Norm %d Rev %d OffsetNorm %d OffsetRev %d",
        pLineObj->calLineData.typeData.vagData.vagNorm,
        pLineObj->calLineData.typeData.vagData.vagRev,
        pDevObj->vp890SysCalData.vagOffsetNorm[pLineObj->channelId],
        pDevObj->vp890SysCalData.vagOffsetRev[pLineObj->channelId]
        ));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG Norm %d OffsetNorm %d",
        pLineObj->calLineData.typeData.ilaData.ilgNorm,
        pDevObj->vp890SysCalData.ilgOffsetNorm[pLineObj->channelId]));

    /* Re-Enable TRDC Sense */
    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Return to VOC feed, speedup enabled */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restoring 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
        pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->calLineData.icr2);
    VpMemCpy(pLineObj->icr2Values, pLineObj->calLineData.icr2, VP890_ICR2_LEN);

    /* Advance to Final State */
    pLineObj->calLineData.calState = VP890_CAL_DONE;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /* Need to wait for line to settle before changing converter. */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagMeasureInvert()"));

    return TRUE;
} /* end Vp890VagMeasureInvert */

/**
 * Vp890VbgMeasureInvert()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
bool
Vp890VbgMeasureInvert(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vbgValue;
    int16 vagStaticOffset = 205;    /* static offset of 1.5V on 890 */

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgMeasureInvert()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_RING_TO_GND_V};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgMeasureInvert()"));
        return TRUE;;
    }

    /* Read voltage. */
    vbgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_RING_TO_GND_V);

    /* remove static offset of 1.5V on 890 */
    vbgValue += vagStaticOffset;

    if (pLineObj->calLineData.reversePol == FALSE) {
        pDevObj->vp890SysCalData.vbgOffsetNorm[pLineObj->channelId] = vbgValue;

        /* This is to be backward compatible with VVA P1.1.0 */
        pLineObj->calLineData.typeData.vbgData.vbgOffsetNorm = vbgValue;
    } else {
        pDevObj->vp890SysCalData.vbgOffsetRev[pLineObj->channelId] = vbgValue;

        /* This is to be backward compatible with VVA P1.1.0 */
        pLineObj->calLineData.typeData.vbgData.vbgOffsetRev = vbgValue;
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Norm %d Rev %d OffsetNorm %d OffsetRev %d",
        pLineObj->calLineData.typeData.vbgData.vbgNorm,
        pLineObj->calLineData.typeData.vbgData.vbgRev,
        pDevObj->vp890SysCalData.vbgOffsetNorm[pLineObj->channelId],
        pDevObj->vp890SysCalData.vbgOffsetRev[pLineObj->channelId]
        ));

    /* Re-Enable TRDC Sense */
    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Return to VOC feed, speedup enabled */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restoring 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
        pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->calLineData.icr2);
    VpMemCpy(pLineObj->icr2Values, pLineObj->calLineData.icr2, VP890_ICR2_LEN);

    /* Advance to Final State */
    pLineObj->calLineData.calState = VP890_CAL_DONE;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /* Need to wait for line to settle before changing converter. */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgMeasureInvert()"));

    return TRUE;
} /* end Vp890VbgMeasureInvert */

/**
 * Vp890CalVas()
 *  This function determines the optimal VAS value for both normal and reverse
 * polarity on the line specified by the line context. It should only be called
 * by API-II internal functions.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  VAS values for normal and reverse polarity are computed and stored in the
 * line object. The line state is returned to state it was in prior to starting
 * calibration.
 */
VpStatusType
Vp890CalVas(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    uint8 convCfg[VP890_CONV_CFG_LEN];
    uint8 dcFeed[VP890_DC_FEED_LEN];

    int16 imtNew;
    uint16 imtErr;
    uint16 vasValue;
    uint8 icr2Mods[VP890_ICR2_LEN] = {0x00, 0x00, 0xCC, 0xC8};

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalVas()"));

    if (pLineObj->calLineData.calState != VP890_CAL_INIT) {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN,
            convCfg);
        if ((convCfg[0] & VP890_CONV_CONNECT_BITS) != VP890_METALLIC_DC_I) {
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Converter Check Fail"));

            /*
             * The device internally changed the converter configuration.
             * So we're in a bad condition, no need to measure. Increase
             * VAS and try again.
             */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD, VP890_DC_FEED_LEN,
                dcFeed);

            vasValue = VP890_VAS_CONVERSION(dcFeed[0], dcFeed[1]);
            VpCSLACSetVas(dcFeed, vasValue + 750);

            if (vasValue >= (VP890_VAS_MAX - VP890_VAS_OVERHEAD)) {
                /*
                 * If we're at the maximum VAS value, then that's the
                 * best we can and no sense continuing.
                 */
                VpCSLACSetVas(dcFeed, VP890_VAS_MAX);
                pLineObj->calLineData.calState = VP890_CAL_DONE;
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Giving Up on Converter Check"));
            } else {
                convCfg[0] = VP890_METALLIC_DC_I;
                VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT,
                    VP890_CONV_CFG_LEN, convCfg);

                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                    vasValue, dcFeed[0], dcFeed[1]));
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
                dcFeed);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
               MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;

            pLineObj->calLineData.calState = VP890_CAL_OFFSET;

            return VP_STATUS_SUCCESS;
        }
    }

    switch(pLineObj->calLineData.calState) {
        case VP890_CAL_INIT:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("VAS: Cal Init"));
            pLineObj->calLineData.typeData.vasData.secondPass = FALSE;

            /*
             * Save current dc calibration data. This value is adjusted by cal
             * algorithms to finalize a set that is programmed in the device
             * by Set Line State. It will contain adjusted VOC values for each
             * polarity.
             */
            if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
                pLineObj->calLineData.reversePol = TRUE;

                dcFeed[0] = pLineObj->calLineData.dcFeedPr[0];
                dcFeed[1] = pLineObj->calLineData.dcFeedPr[1];

                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("From DC PolRev 0x%02X 0x%02X",
                    dcFeed[0], dcFeed[1]));
            } else {
                pLineObj->calLineData.reversePol = FALSE;

                dcFeed[0] = pLineObj->calLineData.dcFeed[0];
                dcFeed[1] = pLineObj->calLineData.dcFeed[1];

                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("From DC Normal 0x%02X 0x%02X",
                    dcFeed[0], dcFeed[1]));
            }

            /* Set Battery and Feed for High-Speed switching */
            icr2Mods[0] = pLineObj->calLineData.icr2[0];
            icr2Mods[1] = pLineObj->calLineData.icr2[1];
            icr2Mods[2] |= pLineObj->calLineData.icr2[2];
            icr2Mods[3] |= pLineObj->calLineData.icr2[3];

            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Writing (VAS Mods) 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
                icr2Mods[0], icr2Mods[1], icr2Mods[2], icr2Mods[3],
                pLineObj->channelId));

            VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
                icr2Mods);
            VpMemCpy(pLineObj->icr2Values, icr2Mods, VP890_ICR2_LEN);

            /* Set VAS to minimum */
            VpCSLACSetVas(dcFeed, 0);
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. VAS Writing 0x%04X to DC Feed",
                ((dcFeed[0] << 8) & 0xFF00) | dcFeed[1]));
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
                dcFeed);

            /* Make sure samples are collected from the PCM Test Buffer */
            pDevObj->devMode[0] &= ~VP890_DEV_MODE_TEST_DATA;
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DEV_MODE_WRT, VP890_DEV_MODE_LEN,
                pDevObj->devMode);

            /*
             * Start timers to wait for supply to settle before taking the
             * first measurement
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
               MS_TO_TICKRATE(VP890_VAS_INIT_WAIT,
                   pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;

            /* Increment the state machine */
            pLineObj->calLineData.calState = VP890_CAL_OFFSET;
            break;

        case VP890_CAL_OFFSET:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVAS Cal: Cal Offset"));

            imtNew = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);
            pLineObj->calLineData.typeData.vasData.imtPrev = imtNew;

            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Starting IMT Value %d on Channel %d",
                imtNew, pLineObj->channelId));

            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD, VP890_DC_FEED_LEN,
                dcFeed);

            vasValue = VP890_VAS_CONVERSION(dcFeed[0], dcFeed[1]);
            VpCSLACSetVas(dcFeed, vasValue + 750);

            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("3. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                vasValue, dcFeed[0], dcFeed[1]));

            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
                dcFeed);

            if (vasValue >= (VP890_VAS_MAX - VP890_VAS_OVERHEAD)) {
                /*
                 * If we're at the maximum VAS value, then that's the
                 * best we can and no sense continuing.
                 */
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. Max Overhead Reached"));

                VpCSLACSetVas(dcFeed, VP890_VAS_MAX);
                VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
                    dcFeed);

                Vp890VasPolRev(pLineCtx);
            } else {
                pLineObj->calLineData.calState = VP890_CAL_MEASURE;
            }
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                VP_ACTIVATE_TIMER;
            break;

        case VP890_CAL_MEASURE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("VAS: Cal Measure"));

            imtNew = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);
            imtErr = (imtNew >= pLineObj->calLineData.typeData.vasData.imtPrev)
                ? (imtNew - pLineObj->calLineData.typeData.vasData.imtPrev)
                : (pLineObj->calLineData.typeData.vasData.imtPrev - imtNew);

            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("New IMT Value %d IMT Error %d on Channel %d",
                imtNew, imtErr, pLineObj->channelId));

            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD, VP890_DC_FEED_LEN,
                dcFeed);
            vasValue = VP890_VAS_CONVERSION(dcFeed[0], dcFeed[1]);

            if (imtErr > VP890_VAS_MEAS_ERR) {
                pLineObj->calLineData.typeData.vasData.imtPrev = imtNew;

                VpCSLACSetVas(dcFeed, vasValue + 750);
                VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
                    dcFeed);

                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("3. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                    vasValue, dcFeed[0], dcFeed[1]));

                pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                    MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                        pDevObj->devProfileData.tickRate);

                pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                    VP_ACTIVATE_TIMER;

                if (vasValue >= (VP890_VAS_MAX - VP890_VAS_OVERHEAD)) {
                    /*
                     * If we're at the maximum VAS value, then that's the
                     * best we can and no sense continuing.
                     */
                    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. Max Overhead Reached"));

                    VpCSLACSetVas(dcFeed, VP890_VAS_MAX);
                    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                        VP890_DC_FEED_LEN, dcFeed);

                    if (pLineObj->calLineData.typeData.vasData.secondPass == FALSE) {
                        pLineObj->calLineData.typeData.vasData.secondPass = TRUE;
                        pLineObj->calLineData.calState = VP890_CONVERTER_CHECK;

                        Vp890VasPolRev(pLineCtx);
                        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                            MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                                pDevObj->devProfileData.tickRate);

                        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                            VP_ACTIVATE_TIMER;

                        /* Set VAS to minimum */
                        VpCSLACSetVas(dcFeed, 0);
                        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                            VP890_DC_FEED_LEN, dcFeed);
                    } else {
                        pLineObj->calLineData.calState = VP890_CAL_DONE;
                    }
                }
            } else {
                VpCSLACSetVas(dcFeed, vasValue + VP890_VAS_OVERHEAD);

                if (pLineObj->calLineData.reversePol == FALSE) {
                    pLineObj->calLineData.reversePol = TRUE;
                    pLineObj->calLineData.dcFeed[0] = dcFeed[0];
                    pLineObj->calLineData.dcFeed[1] = dcFeed[1];

                    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAS Normal Polarity Cal OK -- Saving 0x%02X 0x%02X",
                        pLineObj->calLineData.dcFeed[0],
                        pLineObj->calLineData.dcFeed[1]));

                    dcFeed[0] = pLineObj->calLineData.dcFeedPr[0];
                    dcFeed[1] = pLineObj->calLineData.dcFeedPr[1];
                } else {
                    pLineObj->calLineData.reversePol = FALSE;
                    pLineObj->calLineData.dcFeedPr[0] = dcFeed[0];
                    pLineObj->calLineData.dcFeedPr[1] = dcFeed[1];

                    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAS Reverse Polarity Cal OK -- Saving 0x%02X 0x%02X",
                        pLineObj->calLineData.dcFeedPr[0],
                        pLineObj->calLineData.dcFeedPr[1]));

                    dcFeed[0] = pLineObj->calLineData.dcFeed[0];
                    dcFeed[1] = pLineObj->calLineData.dcFeed[1];
                }

                if (pLineObj->calLineData.typeData.vasData.secondPass == FALSE) {
                    pLineObj->calLineData.typeData.vasData.secondPass = TRUE;
                    pLineObj->calLineData.calState = VP890_CONVERTER_CHECK;

                    Vp890VasPolRev(pLineCtx);
                    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                        MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                            pDevObj->devProfileData.tickRate);

                    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                        VP_ACTIVATE_TIMER;

                    /* Set VAS to minimum */
                    VpCSLACSetVas(dcFeed, 0);
                    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                        VP890_DC_FEED_LEN, dcFeed);
                } else {
                    pLineObj->calLineData.calState = VP890_CAL_DONE;
                }
            }
            break;

        case VP890_CAL_INVERT_POL:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("VAS: Reversing Polarity"));
            Vp890SetFxsLineState(pLineCtx,
                VpGetReverseState(pLineObj->lineState.currentState));

            /* Set VAS to minimum */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD,
                VP890_DC_FEED_LEN, pLineObj->calLineData.dcFeed);

            dcFeed[0] = pLineObj->calLineData.dcFeed[0];
            dcFeed[1] = pLineObj->calLineData.dcFeed[1];

            VpCSLACSetVas(dcFeed, 0);
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                VP890_DC_FEED_LEN, dcFeed);
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("1. VAS - VOC Reg 0x%02X 0x%02X",
                dcFeed[0], dcFeed[1]));

            /*
             * Start timers to wait for supply to settle before taking the
             * measurement
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_VAS_MEAS_DELAY,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;

            /* Increment the state machine */
            pLineObj->calLineData.calState = VP890_CONVERTER_CHECK;
            break;

        default:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("VAS Cal Error - Terminate"));
            pLineObj->calLineData.calState = VP890_CAL_DONE;
            break;
    }

    if (pLineObj->calLineData.calState == VP890_CAL_DONE) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("VAS: Cal Completion"));

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restoring 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
            pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
            pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
            pLineObj->channelId));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
            pLineObj->calLineData.icr2);
        VpMemCpy(pLineObj->icr2Values, pLineObj->calLineData.icr2, VP890_ICR2_LEN);

        /*
         * Both dcFeed and dcFeedPr contain the same maximum VAS setting that
         * is required for both normal and polarity reversal conditions to
         * prevent saturation condition. So using either value in the line
         * object is ok.
         */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
            pLineObj->calLineData.dcFeed);

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("5. VAS - VOC Normal 0x%02X 0x%02X Reverse 0x%02X 0x%02X",
            pLineObj->calLineData.dcFeed[0],
            pLineObj->calLineData.dcFeed[1],
            pLineObj->calLineData.dcFeedPr[0],
            pLineObj->calLineData.dcFeedPr[1]));

        /* Only support one channel FXS, so ok to use 0 for first index value */

        /* Second byte contains the lower two bits of VAS */
        pDevObj->vp890SysCalData.vas[0][VP890_NORM_POLARITY] =
            ((pLineObj->calLineData.dcFeed[1] >> 6) & 0x3);

        /* First byte contains the upper two bits of VAS */
        pDevObj->vp890SysCalData.vas[0][VP890_NORM_POLARITY] |=
            ((pLineObj->calLineData.dcFeed[0] << 2) & 0xC);

        /* Second byte contains the lower two bits of VAS */
        pDevObj->vp890SysCalData.vas[0][VP890_REV_POLARITY] =
            ((pLineObj->calLineData.dcFeedPr[1] >> 6) & 0x3);

        /* First byte contains the upper two bits of VAS */
        pDevObj->vp890SysCalData.vas[0][VP890_REV_POLARITY] |=
            ((pLineObj->calLineData.dcFeedPr[0] << 2) & 0xC);

        pLineObj->responseData = VP_CAL_SUCCESS;
        Vp890CalDone(pLineCtx);
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalVas()"));

    return VP_STATUS_SUCCESS;
}

/**
 * Vp890VasPolRev()
 *  This function inverts polarity of line and sets reversePol flag in line
 * object (calibration) appropriately.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Line polarity is reversed. Line Object updated.
 */
void
Vp890VasPolRev(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VasPolRev()"));

    if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Normal"));
        pLineObj->slicValueCache &= ~VP890_SS_POLARITY_MASK;
        pLineObj->calLineData.reversePol = FALSE;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Changing Polarity to Reverse"));
        pLineObj->slicValueCache |= VP890_SS_POLARITY_MASK;
        pLineObj->calLineData.reversePol = TRUE;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_WRT, VP890_SYS_STATE_LEN,
        &pLineObj->slicValueCache);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VasPolRev()"));
}

/**
 * Vp890CalVoc()
 *  This function sets the VOC values in dcFeed as specified by the device
 * dc feed register, with VOS value passed. It does not actually access the
 * device, just simply computes the correct hex values for the dc feed reg.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  Line not affected. Values in dcFeed contain the VOC values passed.
 */

VpStatusType
Vp890CalVoc(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalVoc()"));

    switch(pLineObj->calLineData.calState) {
        case VP890_CAL_INIT:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_INIT",
                pLineObj->channelId));
            Vp890VocInit(pLineCtx);
            break;

        case VP890_CAL_ADC:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_ADC",
                pLineObj->channelId));
            Vp890VocSetAdc(pLineCtx);
            break;

        case VP890_CAL_VOC_OFFSET:
        case VP890_CAL_OFFSET:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_OFFSET",
                pLineObj->channelId));
            Vp890VocOffset(pLineCtx);
            break;

        case VP890_CAL_SIGEN_A_PHASE1:
        case VP890_CAL_SIGEN_A_PHASE2:
        case VP890_CAL_SIGEN_A_PHASE3:
        case VP890_CAL_SIGEN_A_PHASE4:
            Vp890SigGenOffset(pLineCtx);
            break;

        case VP890_CAL_MEASURE_2:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_MEASURE_2",
                pLineObj->channelId));
            Vp890VocOffset2(pLineCtx);
            break;

        case VP890_CAL_MEASURE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_MEASURE",
                pLineObj->channelId));
            Vp890VocMeasure(pLineCtx);
            break;

        case VP890_CAL_INVERT_POL:
        case VP890_CAL_INVERT_POL_VOC:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_INVERT_POL",
                pLineObj->channelId));
            Vp890VocMeasureInvert(pLineCtx);
            break;

        case VP890_CAL_DONE:
        case VP890_CAL_DONE_VOC_MEASURE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_DONE",
                pLineObj->channelId));
            Vp890VocDone(pLineCtx);
            break;

        case VP890_CAL_DONE_PRE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VOC Cal State VP890_CAL_DONE_PRE",
                pLineObj->channelId));
            Vp890VocMeasureInvert2(pLineCtx);
            break;

        case VP890_CAL_ERROR:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVOC Cal Error"));
            pLineObj->responseData = VP_CAL_FAILURE;
            Vp890VocDone(pLineCtx);
            Vp890CalDone(pLineCtx);

            pLineObj->calLineData.calState = VP890_CAL_EXIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE((VP890_CAL_SET),
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            break;

        default:
            pLineObj->calLineData.calState = VP890_CAL_INIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            break;
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalVoc()"));

    return VP_STATUS_SUCCESS;
} /*end Vp890CalVoc */

/**
 * Vp890CalVag()
 *  This function sets the VAG values in dcFeed as specified by the device
 * dc feed register, with VAG value passed. It does not actually access the
 * device, just simply computes the correct hex values for the dc feed reg.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  Line not affected. Values in dcFeed contain the VAG values passed.
 */

VpStatusType
Vp890CalVag(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalVag()"));

    switch(pLineObj->calLineData.calState) {
        case VP890_CAL_INIT:    /* This case can be suppressed */
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_INIT",
                pLineObj->channelId));
            Vp890VagInit(pLineCtx);
            break;

        case VP890_CAL_ADC:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_ADC",
                pLineObj->channelId));
            Vp890VagSetAdc(pLineCtx);
            break;

        case VP890_CAL_OFFSET:
        case VP890_CAL_OFFSET_VAG_VOC:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_OFFSET",
                pLineObj->channelId));
            Vp890VagOffset(pLineCtx);
            break;

        case VP890_CAL_MEASURE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_MEASURE",
                pLineObj->channelId));
            Vp890VagMeasure(pLineCtx);
            break;

        case VP890_CAL_INVERT_POL:
        case VP890_CAL_INVERT_POL_VAG_TGND:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_INVERT_POL",
                pLineObj->channelId));
            Vp890VagMeasureInvert(pLineCtx);
            break;

        case VP890_CAL_DONE:
        case VP890_CAL_VAG_DONE:
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State VP890_CAL_DONE",
                pLineObj->channelId));
            Vp890VagDone(pLineCtx);
            break;

        case VP890_CAL_ERROR:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVOC Cal Error"));
            pLineObj->responseData = VP_CAL_FAILURE;
            Vp890VagDone(pLineCtx);
            Vp890CalDone(pLineCtx);

            pLineObj->calLineData.calState = VP890_CAL_EXIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE((VP890_CAL_SET),
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            break;

        default:
            pLineObj->calLineData.calState = VP890_CAL_INIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            break;
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalVag()"));

    return VP_STATUS_SUCCESS;

} /*end Vp890CalVag */

/**
 * Vp890CalVbg()
 *  This function sets the VBG values in dcFeed as specified by the device
 * dc feed register, with VAG value passed. It does not actually access the
 * device, just simply computes the correct hex values for the dc feed reg.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  Line not affected. Values in dcFeed contain the VAG values passed.
 */

VpStatusType
Vp890CalVbg(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalVbg()"));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d VAG Cal State %d",
        pLineObj->channelId, pLineObj->calLineData.calState));

    switch(pLineObj->calLineData.calState) {
        case VP890_CAL_INIT:
            Vp890VbgInit(pLineCtx);
            break;

        case VP890_CAL_ADC:
            Vp890VbgSetAdc(pLineCtx);
            break;

        case VP890_CAL_OFFSET:
            Vp890VbgOffset(pLineCtx);
            break;

        case VP890_CAL_MEASURE:
            Vp890VbgMeasure(pLineCtx);
            break;

        case VP890_CAL_INVERT_POL:
            Vp890VbgMeasureInvert(pLineCtx);
            break;

        case VP890_CAL_DONE:
            Vp890VbgDone(pLineCtx);
            break;

        case VP890_CAL_ERROR:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("\nVOC Cal Error"));
            pLineObj->responseData = VP_CAL_FAILURE;
            Vp890VbgDone(pLineCtx);
            Vp890CalDone(pLineCtx);

            pLineObj->calLineData.calState = VP890_CAL_EXIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE((VP890_CAL_SET),
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            break;

        default:
            pLineObj->calLineData.calState = VP890_CAL_INIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            break;
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalVbg()"));

    return VP_STATUS_SUCCESS;
} /*end Vp890CalVag */

/**
 * Vp890CalDone()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890CalDone(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 convCfg[VP890_CONV_CFG_LEN];

    uint8 mpiBuffer[7 + VP890_DEV_MODE_LEN + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN +
                        VP890_CONV_CFG_LEN + VP890_ICR2_LEN + VP890_DISN_LEN +
                        VP890_VP_GAIN_LEN];
    uint8 mpiIndex = 0;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalDone()"));

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Calibration Done"));

    /* Restore Device Mode */
    pDevObj->devMode[0] |= VP890_DEV_MODE_TEST_DATA;
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DEV_MODE_WRT,
       VP890_DEV_MODE_LEN, pDevObj->devMode);

    /* Set use of programmed coefficients and Codec Mode */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restore 0x%02X on Channel %d in OP Func",
        pLineObj->calLineData.codecReg, pLineObj->channelId));

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
       VP890_OP_FUNC_LEN, &pLineObj->calLineData.codecReg);

/*    Vp890SetCodec(pLineCtx, pLineObj->codec); */

    /* Cut TX/RX PCM and enable HPF */
    pLineObj->opCond[0] = (VP890_CUT_TXPATH | VP890_CUT_RXPATH);

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
       VP890_OP_COND_LEN, pLineObj->opCond);

    /* Connect A/D to AC T/R Input */
    convCfg[0] = (VP890_METALLIC_AC_V | pDevObj->txBufferDataRate);

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
       VP890_CONV_CFG_LEN, convCfg);

    /* Restore off channel specific registers */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Restoring (Cal Done) 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->calLineData.icr2[0], pLineObj->calLineData.icr2[1],
        pLineObj->calLineData.icr2[2], pLineObj->calLineData.icr2[3],
        pLineObj->channelId));

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_ICR2_WRT,
       VP890_ICR2_LEN, pLineObj->calLineData.icr2);
    VpMemCpy(pLineObj->icr2Values, pLineObj->calLineData.icr2, VP890_ICR2_LEN);

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. Restoring 0x%02X to Channel %d to DISN, VP Gain to 0x%02X",
        pLineObj->calLineData.disnVal[0], pLineObj->channelId, pLineObj->calLineData.vpGain[0]));

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DISN_WRT,
       VP890_DISN_LEN, pLineObj->calLineData.disnVal);

	/*BEGIN: fix MOTO*/
	if (17 <= mpiIndex)
	{
		return ;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
        VP890_VP_GAIN_LEN, pLineObj->calLineData.vpGain);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
        mpiIndex-1, &mpiBuffer[1]);

    /*
     * Restore Line State (note: this won't update correctly the calibrated DC
     * feed settings because we're still in calibration. So we have to do that
     * manually)
     */
    Vp890SetFxsLineState(pLineCtx, pLineObj->lineState.usrCurrent);

    /* Check the polarity and load Calibrated DC Feed values accordingly */
    if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
            pLineObj->calLineData.dcFeedPr);
    } else {
        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
            pLineObj->calLineData.dcFeed);
    }

    if (pLineObj->responseData == VP_CAL_SUCCESS) {
        pLineObj->calLineData.calDone = TRUE;
    }

    pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
    pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;

    /* Mark the device as not-in-calibration and complete. */
    pDevObj->state &= ~VP_DEV_IN_CAL;
    pDevObj->stateInt |= VP890_SYS_CAL_COMPLETE;

    Vp890LLSetSysState(deviceId, pLineCtx, 0, FALSE);

#ifdef VP890_LP_SUPPORT
    /* Force an update on the line */
    Vp890LowPowerMode(pDevCtx);
#endif

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890CalDone()"));
}

/**
 * Vp890VocInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VocInit(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 converterCfg[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_V};
    uint8 dcFeed20mA[VP890_DC_FEED_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocInit()"));

    /* Calibration to start at 20ma, incremented to 25, 32, 40mA later */
    dcFeed20mA[0] = pLineObj->calLineData.dcFeedRef[0];
    dcFeed20mA[1] = (pLineObj->calLineData.dcFeedRef[1] & ~VP890_ILA_MASK) + 2;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Vp890VocInit: Register 0x%02X 0x%02X Channel %d",
        dcFeed20mA[0], dcFeed20mA[1], pLineObj->channelId));

    VpMemCpy(pLineObj->calLineData.dcFeed, pLineObj->calLineData.dcFeedRef,
        VP890_DC_FEED_LEN);

    VpMemCpy(pLineObj->calLineData.dcFeedPr, pLineObj->calLineData.dcFeedRef,
        VP890_DC_FEED_LEN);

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
        dcFeed20mA);

    /* Sense VAB Voltage */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
        converterCfg);

    /* Advanced to state for ADC calibration */
    pLineObj->calLineData.calState = VP890_CAL_ADC;

    /* Start timer to cause internal line calibration function to execute */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocInit()"));
} /* end Vp890VocInit */

/**
 * Vp890VagInit()
 *  This function initiates a calibration operation for VAG
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VagInit(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 converterCfg[VP890_CONV_CFG_LEN] = {VP890_TIP_TO_GND_V};

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagInit()"));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Vp890VagInit: Register Normal: 0x%02X 0x%02X Register Reverse: 0x%02X 0x%02X",
        pLineObj->calLineData.dcFeed[0],
        pLineObj->calLineData.dcFeed[1],
        pLineObj->calLineData.dcFeedPr[0],
        pLineObj->calLineData.dcFeedPr[1]));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
        pLineObj->calLineData.dcFeed);

    /* Sense VAG Voltage */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
        converterCfg);

    /* Advanced to state for ADC calibration */
    pLineObj->calLineData.calState = VP890_CAL_ADC;

    /* Start timer to cause internal line calibration function to execute */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagInit()"));
} /* end Vp890VagInit */

/**
 * Vp890VbgInit()
 *  This function initiates a calibration operation for VBG
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VbgInit(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal = pLineObj->ecVal;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 converterCfg[VP890_CONV_CFG_LEN] = {VP890_RING_TO_GND_V};

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgInit()"));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Vp890VbgInit: Register 0x%02X 0x%02X",
        pLineObj->calLineData.dcFeed[0],
        pLineObj->calLineData.dcFeed[1]));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
        pLineObj->calLineData.dcFeed);

    /* Sense VBG Voltage */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
        converterCfg);

    /* Advanced to state for ADC calibration */
    pLineObj->calLineData.calState = VP890_CAL_ADC;

    /* Start timer to cause internal line calibration function to execute */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgInit()"));
} /* end Vp890VbgInit */

/**
 * Vp890VocSetAdc ()
 *  This function set the converter to read the right pcm
 *  and set the right state machine , takes care for Pol Rev
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VocSetAdc(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vocVolt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocSetAdc()"));

    /* Read the Normal VOC Value raw from PCM */
    vocVolt = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Norm: %d", vocVolt));
        pLineObj->calLineData.typeData.vocData.vocNorm = vocVolt;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Rev: %d", vocVolt));
        pLineObj->calLineData.typeData.vocData.vocRev = vocVolt;
    }

    /* Setup channel for Normal ILA and VAB offset read. Allow VAB to collapse */
    /* Disable TRDC sense -- forces ILA when on-hook */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state, where we can read Metalic Current */
    pLineObj->calLineData.calState = VP890_CAL_OFFSET;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Wait 100ms for hook conditions to settle and converter change internal to
     * the device. Then we can change the converter back and make the
     * measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocSetAdc()"));
} /* end Vp890VocSetAdc */

/**
 * Vp890VagSetAdc ()
 *  This function set the converter to read the right pcm
 *  and set the right state machine , takes care for Pol Rev
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VagSetAdc(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vagVolt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagSetAdc()"));

    /* Read the Normal VAG Value raw from PCM */
    vagVolt = Vp890AdcSettling(pDevObj, ecVal, VP890_TIP_TO_GND_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Norm: %d", vagVolt));
        pLineObj->calLineData.typeData.vagData.vagNorm = vagVolt;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Rev: %d", vagVolt));
        pLineObj->calLineData.typeData.vagData.vagRev = vagVolt;
    }

    /* Setup channel for Normal ILG and VAG offset read. Allow VAG to collapse */
    /* Disable TRDC sense -- forces ILG when on-hook */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state, where we can read Longitudinal Current */
    pLineObj->calLineData.calState = VP890_CAL_OFFSET;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Wait 20ms before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagSetAdc()"));
} /* end Vp890VagSetAdc */

/**
 * Vp890VbgSetAdc ()
 *  This function set the converter to read the right pcm
 *  and set the right state machine , takes care for Pol Rev
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VbgSetAdc(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vbgVolt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgSetAdc()"));

    /* Read the Normal VBG Value raw from PCM */
    vbgVolt = Vp890AdcSettling(pDevObj, ecVal, VP890_RING_TO_GND_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Norm: %d", vbgVolt));
        pLineObj->calLineData.typeData.vbgData.vbgNorm = vbgVolt;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Rev: %d", vbgVolt));
        pLineObj->calLineData.typeData.vbgData.vbgRev = vbgVolt;
    }

    /* Setup channel for Normal IMT and VBG offset read. Allow VBG to collapse */
    /* Disable TRDC sense -- forces IMT when on-hook */
    pLineObj->dcCalValues[1] |=
        (VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT | VP890_DCCAL_RSVD1);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state, where we can read metalic Current */
    pLineObj->calLineData.calState = VP890_CAL_OFFSET;
    pLineObj->calLineData.convCfgChangeReq = TRUE;

    /*
     * Wait before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_LONG, pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgSetAdc()"));
} /* end Vp890VbgSetAdc */

/**
 * Vp890VocOffset ()
 *
 *  This function computes and save the offset value for the ABV calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *
 */
void
Vp890VocOffset(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 imtValue, vabOffset;
    uint8 channelId = pLineObj->channelId;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocOffset()"));

    /* Now (20ms later) it should be safe to change converter configuration */
    if(pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocOffset()"));
        return;
    }

    /* Read the metallic current. Should be "high". */
    if (pLineObj->calLineData.calState == VP890_CAL_OFFSET) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_V};

        imtValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);

        if (pLineObj->calLineData.reversePol == FALSE) {
            uint8 dcFeedRegister[VP890_DC_FEED_LEN];

            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD,
                VP890_DC_FEED_LEN, dcFeedRegister);
            dcFeedRegister[1] &= ~VP890_ILA_MASK;

            if (pDevObj->vp890SysCalData.ila20[channelId] == 0) {
                pDevObj->vp890SysCalData.ila20[channelId] = imtValue;
                dcFeedRegister[1] |= (VP890_ILA_MASK & 7);
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILA PCM Measured 20mA: %d Converted (10uA) %d Channel %d",
                    imtValue, (uint16)(imtValue * 100 / VP890_ILA_SCALE_1MA), channelId));
            } else if (pDevObj->vp890SysCalData.ila25[channelId] == 0) {
                pDevObj->vp890SysCalData.ila25[channelId] = imtValue;
                dcFeedRegister[1] |= (VP890_ILA_MASK & 14);
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILA Measured 25mA: %d Converted (10uA) %d Channel %d",
                    imtValue, (uint16)(imtValue * 100 / VP890_ILA_SCALE_1MA), channelId));
            } else if (pDevObj->vp890SysCalData.ila32[channelId] == 0) {
                pDevObj->vp890SysCalData.ila32[channelId] = imtValue;
                dcFeedRegister[1] |= (VP890_ILA_MASK & 22);
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILA Measured 32mA: %d Converted (10uA) %d Channel %d",
                    imtValue, (uint16)(imtValue * 100 / VP890_ILA_SCALE_1MA), channelId));
            } else if (pDevObj->vp890SysCalData.ila40[channelId] == 0) {
                pDevObj->vp890SysCalData.ila40[channelId] = imtValue;
                dcFeedRegister[1] = pLineObj->calLineData.dcFeedRef[1];
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILA Measured 40mA: %d Converted (10uA) %d Channel %d",
                    imtValue, (uint16)(imtValue * 100 / VP890_ILA_SCALE_1MA), channelId));
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                VP890_DC_FEED_LEN, dcFeedRegister);

            /* Wait for converter to stabilize */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

            /* Continue to ILG only if all ILA calibration done */
            if (pDevObj->vp890SysCalData.ila40[channelId] == 0) {
                return;
            }
        } else {
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. ILA Rev: %d", imtValue));
        }

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->calLineData.calState = VP890_CAL_VOC_OFFSET;

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        return;
    } else {
        /* Read the VAB Offset Voltage (should be low at this point) */
        vabOffset = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_V);
        if (pLineObj->calLineData.reversePol == FALSE) {
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Norm Offset: %d", vabOffset));
            pDevObj->vp890SysCalData.vocOffset[channelId][VP890_NORM_POLARITY] = vabOffset;

            /* This is to be backward compatible with VVA P1.1.0 */
            pLineObj->calLineData.typeData.vocData.vocOffsetNorm = vabOffset;
        } else {
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Rev Offset: %d", vabOffset));
            pDevObj->vp890SysCalData.vocOffset[channelId][VP890_REV_POLARITY] = vabOffset;

            /* This is to be backward compatible with VVA P1.1.0 */
            pLineObj->calLineData.typeData.vocData.vocOffsetRev = vabOffset;
        }
    }

    /*
     * Disable the VOC DAC, re-enable the TRDC sense. Then need to wait before
     * switching converter.
     */
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX] |= VP890_ICR2_VOC_DAC_SENSE;
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX+1] &= ~VP890_ICR2_VOC_DAC_SENSE;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Writing (VOC Offset) 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->icr2Values[0], pLineObj->icr2Values[1],
        pLineObj->icr2Values[2], pLineObj->icr2Values[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->icr2Values);

    /* Advance to next state */
    pLineObj->calLineData.calState = VP890_CAL_SIGEN_A_PHASE1;

    /*
     * Wait 20ms before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocOffset()"));
} /* end Vp890VocOffset */

/**
 * Vp890SigGenOffset ()
 */
void
Vp890SigGenOffset(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP890_EC_CH1, VP890_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 converterCfg[VP890_CONV_CFG_LEN];
    int16 vabOffset;

    uint8 mpiBuffer[3 + VP890_SIGA_PARAMS_LEN + VP890_SYS_STATE_LEN + VP890_DC_CAL_REG_LEN];
    uint8 mpiIndex = 0;

    switch (pLineObj->calLineData.calState) {
        case VP890_CAL_SIGEN_A_PHASE1:{
            uint8 sigGenA[VP890_SIGA_PARAMS_LEN] =  /* set almost 0V */
                {0x00, 0x00, 0x00, 0x0A, 0xAB, 0x00, 0x02, 0x00, 0x00 ,0x00, 0x00};
            uint8 slacState[VP890_SYS_STATE_LEN] = {VP890_SS_BALANCED_RINGING};

            /* Save the registers */
            VpMpiCmdWrapper(deviceId, ecVal[channelId], VP890_SIGA_PARAMS_RD,
                VP890_SIGA_PARAMS_LEN, pLineObj->calLineData.sigGenA);

            pLineObj->calLineData.sysState[0] = pLineObj->slicValueCache;

            VpMemCpy(pLineObj->calLineData.calReg, pLineObj->dcCalValues,
                VP890_DC_CAL_REG_LEN);
            pLineObj->dcCalValues[0] = 0x00;
            pLineObj->dcCalValues[1] = 0x02;

            /* Update the state to be able to calibrate the signal generator */
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGA_PARAMS_WRT,
               VP890_SIGA_PARAMS_LEN, sigGenA);

            pLineObj->slicValueCache = slacState[0];

			/*BEGIN: fix MOTO*/
			if (17 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
               VP890_SYS_STATE_LEN, slacState);

			/*BEGIN: fix MOTO*/
			if (17 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DC_CAL_REG_WRT,
               VP890_DC_CAL_REG_LEN, pLineObj->dcCalValues);

            VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0], mpiIndex-1,
                &mpiBuffer[1]);

            /* Update the state */
            pLineObj->calLineData.calState = VP890_CAL_SIGEN_A_PHASE2;

            /* Wait for converter to stabilize */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;
        }

        case VP890_CAL_SIGEN_A_PHASE2:
            converterCfg[0] = VP890_METALLIC_DC_V;
            /* Force the converter change. Don't care about data yet. */
            VpMpiCmdWrapper(deviceId, ecVal[channelId], VP890_CONV_CFG_WRT,
                VP890_CONV_CFG_LEN, converterCfg);

            /* Update the state */
            pLineObj->calLineData.calState = VP890_CAL_SIGEN_A_PHASE3;

            /* Wait for converter to stabilize */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP890_CAL_SIGEN_A_PHASE3:
            vabOffset = Vp890AdcSettling(pDevObj, ecVal[channelId], VP890_METALLIC_DC_V);

            pDevObj->vp890SysCalData.sigGenAError[channelId][0] = vabOffset;

            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("SigGenA Offset (10mV): %d Channel %d",
                (int16)((pDevObj->vp890SysCalData.sigGenAError[channelId][VP890_NORM_POLARITY] -
                pDevObj->vp890SysCalData.vocOffset[channelId][VP890_NORM_POLARITY]) *
                VP890_V_PCM_LSB/VP890_V_SCALE), channelId));

            /* restore the registers */
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SIGA_PARAMS_WRT,
               VP890_SIGA_PARAMS_LEN, pLineObj->calLineData.sigGenA);

            pLineObj->slicValueCache = pLineObj->calLineData.sysState[0];
			
			/*BEGIN: fix MOTO*/
			if (17 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_SYS_STATE_WRT,
               VP890_SYS_STATE_LEN, pLineObj->calLineData.sysState);

            VpMemCpy(pLineObj->dcCalValues, pLineObj->calLineData.calReg,
                VP890_DC_CAL_REG_LEN);

			/*BEGIN: fix MOTO*/
			if (17 <= mpiIndex)
			{
				return ;
			}
			/*END: fix MOTO*/
            mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_DC_CAL_REG_WRT,
               VP890_DC_CAL_REG_LEN, pLineObj->dcCalValues);

            VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
                mpiIndex-1, &mpiBuffer[1]);

            /* Update the state */
            pLineObj->calLineData.calState = VP890_CAL_SIGEN_A_PHASE4;

            /* Wait for converter to stabilize */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP890_CAL_SIGEN_A_PHASE4:
            converterCfg[0] = VP890_TIP_TO_GND_V;
            /* Force the converter change. Don't care about data yet. */
            VpMpiCmdWrapper(deviceId, ecVal[channelId], VP890_CONV_CFG_WRT,
                VP890_CONV_CFG_LEN, converterCfg);

            /* Update the state */
            pLineObj->calLineData.calState = VP890_CAL_MEASURE_2;

            /* Wait for converter to stabilize */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        default:
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890SigGenOffset(): Unexpected state!"));
            return;
    }
}

/**
 * Vp890VocOffset2 ()
 */
void
Vp890VocOffset2(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocOffset2()"));

    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state */
    pLineObj->calLineData.calState = VP890_CAL_MEASURE;

    /*
     * Wait 20ms before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    pLineObj->calLineData.convCfgChangeReq = TRUE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocOffset2()"));
} /* end Vp890VocOffset2 */

/**
 * Vp890VagOffset ()
 *
 *  This function computes and save the offset value for the VAG calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *
 */
void
Vp890VagOffset(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 ilgValue, vagOffset;
    int16 vagStaticOffset = 205;    /* static offset of 1.5V on 890 */

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagOffset()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_LONGITUDINAL_DC_I};
        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->calLineData.convCfgChangeReq = FALSE;
        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagOffset()"));
        return;
    } else {
        if (pLineObj->calLineData.calState == VP890_CAL_OFFSET) {
            uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_TIP_TO_GND_V};

            /* Read the longitudinal current */
            ilgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_LONGITUDINAL_DC_I);

            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG Norm: %d", ilgValue));
                pLineObj->calLineData.typeData.ilaData.ilgNorm = ilgValue;
            } else {
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG Rev: %d", ilgValue));
            }

            VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
                adcConfig);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

            pLineObj->calLineData.calState =  VP890_CAL_OFFSET_VAG_VOC;
            return;
        } else {
            /* Read the VAG Offset Voltage */
            vagOffset = Vp890AdcSettling(pDevObj, ecVal, VP890_TIP_TO_GND_V);

            /* remove static offset of 1.5V on 890 */
            vagOffset += vagStaticOffset;

            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Norm Offset: %d", vagOffset));
                pDevObj->vp890SysCalData.vagOffsetNorm[pLineObj->channelId] = vagOffset;

                /* This is to be backward compatible with VVA P1.1.0 */
                pLineObj->calLineData.typeData.vagData.vagOffsetNorm = vagOffset;
            } else {
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Rev Offset: %d", vagOffset));
                pDevObj->vp890SysCalData.vagOffsetRev[pLineObj->channelId] = vagOffset;

                /* This is to be backward compatible with VVA P1.1.0 */
                pLineObj->calLineData.typeData.vagData.vagOffsetNorm = vagOffset;
            }
        }
    }

    /*
     * Disable the VAG DAC, re-enable the TRDC sense. Then need to wait before
     * switching converter.
     */
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX] |= VP890_ICR2_VOC_DAC_SENSE;
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX+1] &= ~VP890_ICR2_VOC_DAC_SENSE;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Writing (VOC Offset) 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->icr2Values[0], pLineObj->icr2Values[1],
        pLineObj->icr2Values[2], pLineObj->icr2Values[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->icr2Values);

    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state */
    pLineObj->calLineData.calState = VP890_CAL_MEASURE;

    /*
     * Wait 20ms before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagOffset()"));
} /* end Vp890VagOffset */

/**
 * Vp890VbgOffset ()
 *
 *  This function computes and save the offset value for the VBG calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *
 */
void
Vp890VbgOffset(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vbgOffset;
    int16 vagStaticOffset = 205;        /* static offset of 1.5V on 890 */

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgOffset()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_RING_TO_GND_V};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgOffset()"));
        return;
    }

    /* Read the VBG Offset Voltage */
    vbgOffset = Vp890AdcSettling(pDevObj, ecVal, VP890_RING_TO_GND_V);

    /* remove static offset of 1.5V on 890 */
    vbgOffset += vagStaticOffset;

    if (pLineObj->calLineData.reversePol == FALSE) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Norm Offset: %d", vbgOffset));
        pDevObj->vp890SysCalData.vbgOffsetNorm[pLineObj->channelId] = vbgOffset;

        /* This is to be backward compatible with VVA P1.1.0 */
        pLineObj->calLineData.typeData.vbgData.vbgOffsetNorm = vbgOffset;
    } else {
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Rev Offset: %d", vbgOffset));
        pDevObj->vp890SysCalData.vbgOffsetRev[pLineObj->channelId] = vbgOffset;

        /* This is to be backward compatible with VVA P1.1.0 */
        pLineObj->calLineData.typeData.vbgData.vbgOffsetRev = vbgOffset;
    }

    /*
     * Disable the VAG DAC, re-enable the TRDC sense. Then need to wait before
     * switching converter.
     */
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX] |= VP890_ICR2_VOC_DAC_SENSE;
    pLineObj->icr2Values[VP890_ICR2_VOC_DAC_INDEX+1] &= ~VP890_ICR2_VOC_DAC_SENSE;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Writing (VOC Offset) 0x%02X 0x%02X 0x%02X 0x%02X to ICR2 on Channel %d",
        pLineObj->icr2Values[0], pLineObj->icr2Values[1],
        pLineObj->icr2Values[2], pLineObj->icr2Values[3],
        pLineObj->channelId));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN,
        pLineObj->icr2Values);

    pLineObj->dcCalValues[1] &= ~(VP890_C_RING_SNS_CUT | VP890_C_TIP_SNS_CUT);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_WRT, VP890_DC_CAL_REG_LEN,
        pLineObj->dcCalValues);

    /* Advance to next state */
    pLineObj->calLineData.calState = VP890_CAL_MEASURE;

    /*
     * Wait 20ms before setting converter config. Otherwise it will change
     * state while trying to take the measurement.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP890_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgOffset()"));
} /* end Vp890VbgOffset */

/**
 * Vp890VocDone()
 *  This function end the calibration operation for VOC
 *  associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VocDone(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 imtValue, vocValue;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VocDone()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocDone()"));
        return;
    } else {
        if (pLineObj->calLineData.calState != VP890_CAL_DONE_VOC_MEASURE) {
            uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_METALLIC_DC_V};

            /* Read the metallic current offset */
            VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Collecting ILA Offset Values..."));
            imtValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_I);
            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("2. ILA OffsetNorm (10uA) %d",
                    (int16)(imtValue * 100 / VP890_ILA_SCALE_1MA)));
                pDevObj->vp890SysCalData.ilaOffsetNorm[pLineObj->channelId] = imtValue;

                /* This is to be compatible with VVA P1.1.0 */
                pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm = imtValue;
            }
            VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
                adcConfig);

            pLineObj->calLineData.calState = VP890_CAL_DONE_VOC_MEASURE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocDone()"));
            return;
        }
    }

    /* Read the metallic voltage */
    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Collecting VOC Values..."));
    vocValue = Vp890AdcSettling(pDevObj, ecVal, VP890_METALLIC_DC_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        pLineObj->calLineData.typeData.vocData.vocNorm = vocValue;
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Norm %d", vocValue));
    } else {
        pLineObj->calLineData.typeData.vocData.vocRev = vocValue;
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VOC Rev %d", vocValue));
    }

    /* Make adjustments */
    Vp890AdjustVoc(pLineCtx, ((pLineObj->calLineData.dcFeedRef[0] >> 2) & 0x7), FALSE);
    Vp890AdjustIla(pLineCtx, (pLineObj->calLineData.dcFeedRef[1] & VP890_ILA_MASK));

    /* Move on to VAS Calibration */
    pLineObj->lineState.calType = VP_CSLAC_CAL_VAG;
    pLineObj->calLineData.calState = VP890_CAL_INIT;

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
       (MS_TO_TICKRATE(VP890_VAS_INIT_WAIT, pDevObj->devProfileData.tickRate))
      | VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocDone()"));
} /* end Vp890VOCDone */

/**
 * Vp890VagDone()
 *  This function end the calibration operation for VAG
 *  associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VagDone(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 ilgValue, vagValue, ilgActual, vagActual, vagActualRev;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VagDone()"));

    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_LONGITUDINAL_DC_I};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagDone()"));
        return;
    } else {
        if (pLineObj->calLineData.calState == VP890_CAL_DONE) {
            uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_TIP_TO_GND_V};

            /* Read the longitudinal current offset */
            ilgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_LONGITUDINAL_DC_I);

            if (pLineObj->calLineData.reversePol == FALSE) {
                pDevObj->vp890SysCalData.ilgOffsetNorm[pLineObj->channelId] = ilgValue;
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG Offset Norm %d", ilgValue));
            }

            VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
                adcConfig);

            pLineObj->calLineData.calState = VP890_CAL_VAG_DONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VocDone()"));
            return;
        } else {
            /* Read the longitudinal voltage */
            vagValue = Vp890AdcSettling(pDevObj, ecVal, VP890_TIP_TO_GND_V);
            if (pLineObj->calLineData.reversePol == FALSE) {
                pLineObj->calLineData.typeData.vagData.vagNorm = vagValue;
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Norm %d", vagValue));
            } else {
                pLineObj->calLineData.typeData.vagData.vagRev = vagValue;
                VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Rev %d", vagValue));
            }
        }
    }

    ilgActual =
        pLineObj->calLineData.typeData.ilaData.ilgNorm
      - pDevObj->vp890SysCalData.ilgOffsetNorm[pLineObj->channelId];

    vagActual =
        pLineObj->calLineData.typeData.vagData.vagNorm
      - pDevObj->vp890SysCalData.vagOffsetNorm[pLineObj->channelId];

    vagActualRev =
        pLineObj->calLineData.typeData.vagData.vagRev
      - pDevObj->vp890SysCalData.vagOffsetRev[pLineObj->channelId];

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VAG Norm %d Rev %d OffsetNorm %d OffsetRev %d",
        pLineObj->calLineData.typeData.vagData.vagNorm,
        pLineObj->calLineData.typeData.vagData.vagRev,
        pDevObj->vp890SysCalData.vagOffsetNorm[pLineObj->channelId],
        pDevObj->vp890SysCalData.vagOffsetRev[pLineObj->channelId]));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("ILG Norm %d OffsetNorm %d",
        pLineObj->calLineData.typeData.ilaData.ilgNorm,
        pDevObj->vp890SysCalData.ilgOffsetNorm[pLineObj->channelId]));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: ILG Actual Norm %d",
        pLineObj->channelId, ilgActual));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: VAG Actual Norm %d Rev Norm %d",
        pLineObj->channelId, vagActual, vagActualRev));

    /* Move on to VBG Calibration */
    pLineObj->lineState.calType = VP_CSLAC_CAL_VBG;
    pLineObj->calLineData.calState = VP890_CAL_INIT;

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
       MS_TO_TICKRATE(VP890_VAS_INIT_WAIT,
           pDevObj->devProfileData.tickRate);

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
       VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VagDone()"));
} /* end Vp890VagDone */

/**
 * Vp890VbgDone()
 *  This function end the calibration operation for VBG
 *  associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp890VbgDone(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal = pLineObj->ecVal;
    int16 vbgValue, vbgActual, vbgActualRev;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890VbgDone()"));

    /* Read the voltage, raw data from PCM */
    if (pLineObj->calLineData.convCfgChangeReq == TRUE) {
        uint8 adcConfig[VP890_CONV_CFG_LEN] = {VP890_RING_TO_GND_V};
        pLineObj->calLineData.convCfgChangeReq = FALSE;

        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            adcConfig);

        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP890_CONVERTER_MEAS_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgDone()"));
        return;
    }

    vbgValue = Vp890AdcSettling(pDevObj, ecVal, VP890_RING_TO_GND_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        pLineObj->calLineData.typeData.vbgData.vbgNorm = vbgValue;
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Norm %d", vbgValue));
    } else {
        pLineObj->calLineData.typeData.vbgData.vbgRev = vbgValue;
        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Rev %d", vbgValue));
    }

    vbgActual =
        pLineObj->calLineData.typeData.vbgData.vbgNorm
      - pDevObj->vp890SysCalData.vbgOffsetNorm[pLineObj->channelId];

    vbgActualRev =
        pLineObj->calLineData.typeData.vbgData.vbgRev
      - pDevObj->vp890SysCalData.vbgOffsetRev[pLineObj->channelId];

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("VBG Norm %d Rev %d OffsetNorm %d OffsetRev %d",
        pLineObj->calLineData.typeData.vbgData.vbgNorm,
        pLineObj->calLineData.typeData.vbgData.vbgRev,
        pDevObj->vp890SysCalData.vbgOffsetNorm[pLineObj->channelId],
        pDevObj->vp890SysCalData.vbgOffsetRev[pLineObj->channelId]));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: VBG Actual Norm %d Rev Norm %d",
        pLineObj->channelId, vbgActual, vbgActualRev));

    /* Move on to VAS Calibration */
    pLineObj->lineState.calType = VP_CSLAC_CAL_VAS;
    pLineObj->calLineData.calState = VP890_CAL_INIT;

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
       MS_TO_TICKRATE(VP890_VAS_INIT_WAIT,
           pDevObj->devProfileData.tickRate);

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
       VP_ACTIVATE_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890VbgDone()"));
} /* end Vp890VbgDone */
#endif /* (VP890_FXS_SUPPORT) && defined (VP_CSLAC_RUNTIME_CAL_ENABLED) */

#ifdef VP890_FXS_SUPPORT
/**
 * Vp890AdjustIla()
 *  This function adjusts the line object data for the adjusted ILA value. No
 * changes are made however if ILA calibration was not previously done.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  If previous calibration done, return TRUE and adjust line object data.
 * Otherwise, return FALSE and no line object change made.
 */
bool
Vp890AdjustIla(
    VpLineCtxType *pLineCtx,
    uint8 targetIla)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 channelId = pLineObj->channelId;
    int32 imtMeasured = 0;
    uint8 imtTarget;

    int16 ilaError, imtActual;
    uint8 ilaAdjust;

    if (targetIla < 4) {            /* 18mA to 22mA */
        imtMeasured = pDevObj->vp890SysCalData.ila20[channelId];
        imtTarget = 20;
    } else if (targetIla < 10) {    /* 23mA to 28mA */
        imtMeasured = pDevObj->vp890SysCalData.ila25[channelId];
        imtTarget = 25;
    } else if (targetIla < 18) {    /* 29mA to 36mA */
        imtMeasured = pDevObj->vp890SysCalData.ila32[channelId];
        imtTarget = 32;
    } else {                        /* 37mA and higher */
        imtMeasured = pDevObj->vp890SysCalData.ila40[channelId];
        imtTarget = 40;
    }

    if (imtMeasured == 0) {
        return FALSE;
    }

    imtActual = imtMeasured - pDevObj->vp890SysCalData.ilaOffsetNorm[channelId];
    ilaError = imtActual - (imtTarget * VP890_ILA_SCALE_1MA);

    /* ILA Scale: 500uA = 273 at PCM */
    if (ABS(ilaError) >= (VP890_ILA_SCALE_1MA / 2)) {
        uint8 tempIlaValue = pLineObj->calLineData.dcFeed[VP890_ILA_INDEX] & VP890_ILA_MASK;
        int8 tempLowValue = (int8)tempIlaValue;
        ilaAdjust = ((ABS(ilaError)+(VP890_ILA_SCALE_1MA / 2)) / VP890_ILA_SCALE_1MA);
        if (ilaError < 0) {
            tempIlaValue += ilaAdjust;
            if (tempIlaValue <= VP890_ILA_MASK) {
                pLineObj->calLineData.dcFeed[VP890_ILA_INDEX] += ilaAdjust;
            } else {
                pLineObj->calLineData.dcFeed[VP890_ILA_INDEX] |= VP890_ILA_MASK;
            }
        } else {
            tempLowValue -= ilaAdjust;
            if (tempLowValue >= 0) {
                pLineObj->calLineData.dcFeed[VP890_ILA_INDEX] -= ilaAdjust;
            } else {
                pLineObj->calLineData.dcFeed[VP890_ILA_INDEX] &= ~VP890_ILA_MASK;
            }
        }
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: ILA Actual Norm (10uA) %d",
        channelId,
        (int16)(imtActual * 100 / VP890_ILA_SCALE_1MA)));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: ILA Target (10uA) %d ILA Error Norm (10uA) %d",
        channelId,
        (int16)(((targetIla + 18) * 100)),
        (int16)(ilaError * 100 / VP890_ILA_SCALE_1MA)));

    return TRUE;
}

/**
 * Vp890AdjustVoc()
 *  This function adjusts the line object data for the adjusted VOC value. No
 * changes are made however if VOC calibration was not previously done.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  If previous calibration done, return TRUE and adjust line object data.
 * Otherwise, return FALSE and no line object change made.
 */
bool
Vp890AdjustVoc(
    VpLineCtxType *pLineCtx,
    uint8 targetVoc,
    bool previousCal)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 channelId = pLineObj->channelId;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    int16 vocActual, vocActualRev;
    int32 pcmTargetVoc;

    int16 vocErrorList[VP890_NUM_POLARITY];
    uint8 vocErrIndex;

    uint8 *dcFeedByte[VP890_NUM_POLARITY];

    dcFeedByte[VP890_NORM_POLARITY] = &(pLineObj->calLineData.dcFeed[0]);
    dcFeedByte[VP890_REV_POLARITY] = &(pLineObj->calLineData.dcFeedPr[0]);

    pcmTargetVoc = (int32)(targetVoc * 3);
    pcmTargetVoc += 36;
    pcmTargetVoc *= VP890_V_1V_SCALE;
    pcmTargetVoc /= VP890_V_1V_RANGE;

    if (previousCal == FALSE) {
        vocActual =
            pLineObj->calLineData.typeData.vocData.vocNorm
          - pDevObj->vp890SysCalData.vocOffset[channelId][VP890_NORM_POLARITY];

        vocActualRev =
            pLineObj->calLineData.typeData.vocData.vocRev
          - pDevObj->vp890SysCalData.vocOffset[channelId][VP890_REV_POLARITY];

        /*
         * Target is always positive. Normal feed is positive. Negative error means
         * voltage is too low (magnitude), positive means too high (magnitude).
         */
        pDevObj->vp890SysCalData.vocError[channelId][VP890_NORM_POLARITY] = (vocActual - (int16)pcmTargetVoc);

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("(In 10mV): VOC Norm %d Rev %d OffsetNorm %d OffsetRev %d Channel %d",
            (int16)(pLineObj->calLineData.typeData.vocData.vocNorm  * VP890_V_PCM_LSB/VP890_V_SCALE),
            (int16)(pLineObj->calLineData.typeData.vocData.vocRev  * VP890_V_PCM_LSB/VP890_V_SCALE),
            (int16)(pDevObj->vp890SysCalData.vocOffset[channelId][VP890_NORM_POLARITY]  * VP890_V_PCM_LSB/VP890_V_SCALE),
            (int16)(pDevObj->vp890SysCalData.vocOffset[channelId][VP890_REV_POLARITY]  * VP890_V_PCM_LSB/VP890_V_SCALE),
            channelId));

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: VOC (10mV) Actual Norm %d Rev Norm %d",
            channelId,
            (int16)(vocActual  * VP890_V_PCM_LSB/VP890_V_SCALE),
            (int16)(vocActualRev  * VP890_V_PCM_LSB/VP890_V_SCALE)));

        /*
         * Target is always positive. Reverse feed is negative. Negative error means
         * voltage is too low (magnitude), positive means too high (magnitude).
         */
        pDevObj->vp890SysCalData.vocError[channelId][VP890_REV_POLARITY] = (-vocActualRev - (int16)pcmTargetVoc);

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: VOC Target %d VOC Error Norm %d Error Rev %d",
            channelId,
            (int16)((targetVoc * 3 + 36) * 100),
            (int16)(pDevObj->vp890SysCalData.vocError[channelId][VP890_NORM_POLARITY]  * VP890_V_PCM_LSB/VP890_V_SCALE),
            (int16)(pDevObj->vp890SysCalData.vocError[channelId][VP890_REV_POLARITY]  * VP890_V_PCM_LSB/VP890_V_SCALE)));

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: DC Feed Values Normal Before 0x%02X 0x%02X",
            channelId, pLineObj->calLineData.dcFeed[0],
            pLineObj->calLineData.dcFeed[1]));

        VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: DC Feed Values Reverse Before 0x%02X 0x%02X",
            channelId, pLineObj->calLineData.dcFeedPr[0],
            pLineObj->calLineData.dcFeedPr[1]));
    }

    /*
     * Adjust if error is more than 1/2 a step size for each parameter based
     * on PCM scale.
     */
    vocErrorList[VP890_NORM_POLARITY] = pDevObj->vp890SysCalData.vocError[channelId][VP890_NORM_POLARITY];
    vocErrorList[VP890_REV_POLARITY] = pDevObj->vp890SysCalData.vocError[channelId][VP890_REV_POLARITY];

    for (vocErrIndex = 0; vocErrIndex < VP890_NUM_POLARITY; vocErrIndex++) {
        /* VOC Scale: 1.5V = 204.8 at PCM. Adjust to account for bit shift */

        if (ABS(vocErrorList[vocErrIndex]) >= 205) {
            if (vocErrorList[vocErrIndex] < 0) {
                /* Error is low, so need to increase VOC */

                /* Saturate the value, to prevent the rollover */
                if ((*dcFeedByte[vocErrIndex] & VP890_VOC_MASK) !=
                    VP890_VOC_MASK) {

                    /* Not saturated within scale. So can adjust up */
                    *dcFeedByte[vocErrIndex] += 0x04;

                } else if ((*dcFeedByte[vocErrIndex] & VP890_VOC_LOW_RANGE) ==
                    VP890_VOC_LOW_RANGE) {

                    /*
                     * Saturated within scale, but not within device. Change
                     * scale (moves up 3V) and clear incremental values or we'll
                     *  end up at the top of the high range.
                     */
                    *dcFeedByte[vocErrIndex] &= ~VP890_VOC_MASK;
                    *dcFeedByte[vocErrIndex] &= ~VP890_VOC_LOW_RANGE;
                }
            } else {
                /* Error is high, so need to decrease VOC */

                /* Saturate the value, to prevent the rollover */
                if ((*dcFeedByte[vocErrIndex] & VP890_VOC_MASK) != 0x00) {
                    /* Not saturated within scale. So can adjust down */
                    *dcFeedByte[vocErrIndex] -= 0x04;
                } else if ((*dcFeedByte[vocErrIndex] & VP890_VOC_LOW_RANGE) !=
                    VP890_VOC_LOW_RANGE) {

                    /*
                     * Saturated within scale, but not within device. Change
                     * scale (moves down 3V) and max incremental values or we'll
                     *  end up at the bottom of the low range.
                     */
                    *dcFeedByte[vocErrIndex] |= VP890_VOC_MASK;
                    *dcFeedByte[vocErrIndex] |= VP890_VOC_LOW_RANGE;
                }
            }
        }
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: DC Feed Values Normal After 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeed[0],
        pLineObj->calLineData.dcFeed[1]));

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan %d: DC Feed Values Reverse After 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeedPr[0],
        pLineObj->calLineData.dcFeedPr[1]));

    return TRUE;
}

/**
 * Vp890BatteryAdjust()
 *  This function programs the device battery calibration based on either
 * calibrated or provided error value.
 */
void
Vp890BatteryAdjust(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 swCal[VP890_BAT_CALIBRATION_LEN];
    uint16 swCalError;
    int32 abvError =
        (pDevObj->vp890SysCalData.abvError[0] * 10000) / VP890_V_PCM_LSB;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Chan 0 Voltage Error: SWY %d",
        (((int16)abvError * 7324) / 10000)));

    /* Write the correction value to CH1 register. Steps in 1.25V increment */
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_BAT_CALIBRATION_RD,
        VP890_BAT_CALIBRATION_LEN, swCal);
    swCal[0] &= ~(VP890_BAT_CAL_SWCAL_MASK);

    /* Conversion from 7.324mV to 1.25V */
    swCalError = (ABS(abvError) / VP890_ABV_STEP);
    if (((ABS(abvError) + (VP890_ABV_STEP / 2)) /  VP890_ABV_STEP) > swCalError) {
        swCalError+=1;
    }
    swCalError = (swCalError > 3) ? 3 : swCalError;
    swCal[0] |= (swCalError << 3);

    /*
     * Positive error means voltage is too low (not negative enough). Positive
     * adjustment makes the battery voltage more negative.
     */
    swCal[0] |= (abvError > 0) ? 0 : VP890_BAT_CAL_SWCAL_SIGN;

    VP_CALIBRATION(VpLineCtxType, pLineCtx, ("Ch 0: Battery Calibration Correction 0x%02X 0x%02X",
        swCal[0], swCal[1]));

    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_BAT_CALIBRATION_WRT,
        VP890_BAT_CALIBRATION_LEN, swCal);
}
#endif

/**
 * Vp890Cal()
 *  This function calibrates a selected block of the device/line.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp890Cal(
    VpLineCtxType       *pLineCtx,
    VpCalType           calType,
    void                *inputArgs)
{
#ifdef VP890_FXS_SUPPORT
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 profileIndex;
    uint8 *profileData;
#endif

    VpStatusType status = VP_STATUS_SUCCESS;

    switch (calType) {
#ifdef VP890_FXS_SUPPORT
        case VP_CAL_GET_SYSTEM_COEFF:
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            if (pDevObj->stateInt & VP890_SYS_CAL_COMPLETE) {
                /* Data length is header (6 bytes) +  890 calibration data */
                pDevObj->mpiLen = (6 + VP890_CAL_STRUCT_SIZE);
                VP_CALIBRATION(VpLineCtxType, pLineCtx,
                    ("Calibration Data Length %d", pDevObj->mpiLen));

                pLineObj->responseData = (uint8)VP_CAL_GET_SYSTEM_COEFF;
                pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
            } else {
                status = VP_STATUS_LINE_NOT_CONFIG;
            }
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;

        case VP_CAL_APPLY_SYSTEM_COEFF:
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            profileData = (uint8 *)inputArgs;

            if (profileData == VP_NULL) {
                VpMemSet(&pDevObj->vp890SysCalData, 0, sizeof(Vp890SysCalResultsType));

                pDevObj->stateInt &= ~(VP890_SYS_CAL_COMPLETE | VP890_CAL_RELOAD_REQ);
                pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
                pLineObj->responseData = VP_CAL_SUCCESS;
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_SUCCESS;
            }

            if ((profileData[VP_PROFILE_TYPE_LSB] != VP_PRFWZ_PROFILE_CAL) ||
                (profileData[VP_PROFILE_TYPE_MSB] != VP_DEV_890_SERIES)) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }

            if (profileData[VP_PROFILE_LENGTH] < (VP890_CAL_STRUCT_SIZE + 2)) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }

            profileIndex = VP_PROFILE_DATA_START;

            pDevObj->vp890SysCalData.abvError[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ABV Y Error %d", pDevObj->vp890SysCalData.abvError[0]));

            pDevObj->vp890SysCalData.vocOffset[0][0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Norm Ch 0 Offset %d", pDevObj->vp890SysCalData.vocOffset[0][0]));

            pDevObj->vp890SysCalData.vocError[0][0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Norm Ch 0 Error %d", pDevObj->vp890SysCalData.vocError[0][0]));

            pDevObj->vp890SysCalData.vocOffset[0][1] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Rev Ch 0 Offset %d", pDevObj->vp890SysCalData.vocOffset[0][1]));

            pDevObj->vp890SysCalData.vocError[0][1] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Rev Ch 0 Error %d", pDevObj->vp890SysCalData.vocError[0][1]));

            pDevObj->vp890SysCalData.sigGenAError[0][0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SigGenA Norm Ch 0 Error %d", pDevObj->vp890SysCalData.sigGenAError[0][0]));

            pDevObj->vp890SysCalData.sigGenAError[0][1] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SigGenA Rev Ch 0 Error %d", pDevObj->vp890SysCalData.sigGenAError[0][1]));

            pDevObj->vp890SysCalData.ila20[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA 20mA Ch 0 %d", pDevObj->vp890SysCalData.ila20[0]));

            pDevObj->vp890SysCalData.ila25[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA 25mA Ch 0 %d", pDevObj->vp890SysCalData.ila25[0]));

            pDevObj->vp890SysCalData.ila32[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA 32mA Ch 0 %d", pDevObj->vp890SysCalData.ila32[0]));

            pDevObj->vp890SysCalData.ila40[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA 40mA Ch 0 %d", pDevObj->vp890SysCalData.ila40[0]));

            pDevObj->vp890SysCalData.ilaOffsetNorm[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA Offset Norm Ch 0 %d", pDevObj->vp890SysCalData.ilaOffsetNorm[0]));

            pDevObj->vp890SysCalData.ilgOffsetNorm[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILG Offset Norm Ch 0 %d", pDevObj->vp890SysCalData.ilgOffsetNorm[0]));

            pDevObj->vp890SysCalData.vas[0][0] = profileData[profileIndex++];
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAS Norm Ch 0 %d", pDevObj->vp890SysCalData.vas[0][0]));

            pDevObj->vp890SysCalData.vas[0][1] = profileData[profileIndex++];
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAS Rev Ch 0 %d", pDevObj->vp890SysCalData.vas[0][1]));

            pDevObj->vp890SysCalData.vagOffsetNorm[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAG Offset Norm Ch 0 %d", pDevObj->vp890SysCalData.vagOffsetNorm[0]));

            pDevObj->vp890SysCalData.vagOffsetRev[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAG Offset Rev Ch 0 %d", pDevObj->vp890SysCalData.vagOffsetRev[0]));

            pDevObj->vp890SysCalData.vbgOffsetNorm[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VBG Offset Norm Ch 0 %d", pDevObj->vp890SysCalData.vbgOffsetNorm[0]));

            pDevObj->vp890SysCalData.vbgOffsetRev[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VBG Offset Rev Ch 0 %d", pDevObj->vp890SysCalData.vbgOffsetRev[0]));

            pDevObj->vp890SysCalData.swyOffset[0] = VpConvertToInt16(&profileData[profileIndex]);
            profileIndex+=2;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SWY Offset Ch 0 %d", pDevObj->vp890SysCalData.swyOffset[0]));

            pDevObj->vp890SysCalData.tipCapCal[0] = VpConvertToInt32(&profileData[profileIndex]);
            profileIndex+=4;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Tip Cap Ch 0 %li", pDevObj->vp890SysCalData.tipCapCal[0]));

            pDevObj->vp890SysCalData.ringCapCal[0] = VpConvertToInt32(&profileData[profileIndex]);
            profileIndex+=4;
            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Ring Cap Ch 0 %li", pDevObj->vp890SysCalData.ringCapCal[0]));

            VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Calibration Data Length - %d", profileIndex));

            pDevObj->stateInt |= (VP890_SYS_CAL_COMPLETE | VP890_CAL_RELOAD_REQ);
            pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
            pLineObj->responseData = VP_CAL_SUCCESS;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#ifdef VP890_FXO_SUPPORT
        case VP_CAL_BFILTER:
            return Vp890CalBFilter(pLineCtx, inputArgs);

        case VP_CAL_APPLY_BFILTER:
            return Vp890CalApplyBFilter(pLineCtx, inputArgs);

        case VP_CAL_MEASURE_BFILTER:
            return Vp890CalMeasureBFilter(pLineCtx);
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    return status;
}

#ifdef VP890_FXO_SUPPORT
/**
 * Vp890CalBFilter()
 *  This function starts the B-Filter adaptive balance calibration.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
static VpStatusType
Vp890CalBFilter(
    VpLineCtxType       *pLineCtx,
    void                *inputArgs)
{
    Vp890LineObjectType *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx           = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;
    uint8 ecVal                     = pLineObj->ecVal;
    uint8 mpiLen;
    VpProfilePtrType pAcProfile;
    uint16 tempGxCidLevel;
    uint16 tempGxUserLevel;
    uint16 tempGrUserLevel;
    VpStatusType vpStatus;

    uint8 mpiBuffer[3 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN + VP890_CONV_CFG_LEN];
    uint8 mpiIndex = 0;

    Vp890CalBFilterData *bFilterData = &pLineObj->calLineData.typeData.bFilterData;

    uint8 convCfg[VP890_CONV_CFG_LEN] = {
        VP890_ENH_B_FILTER_AVG_DET
    };

    uint8 opFunctions[VP890_OP_FUNC_LEN] = {
        VP890_ENABLE_LOADED_COEFFICIENTS | VP890_LINEAR_CODEC
    };

    uint8 opCond[VP890_OP_COND_LEN] = {
        VP890_TXPATH_DIS | VP890_NOISE_GEN_EN
    };

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalBFilter()"));

    if (inputArgs == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    if ( ((VpCalBFilterType *)inputArgs)->pAcProfile == VP_NULL ) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Check the legality of the AC profile and look up the profile table
     * entry if necessary */
    if (!Vp890IsProfileValid(VP_PROFILE_AC,
            VP_CSLAC_AC_PROF_TABLE_SIZE, pDevObj->profEntry.acProfEntry,
            pDevObj->devProfileTable.pAcProfileTable,
            ((VpCalBFilterType *)inputArgs)->pAcProfile,
            &pAcProfile)) {

        return VP_STATUS_ERR_PROFILE;
    }

    //for fix moto
    if (pAcProfile == VP_NULL) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Invalid NULL AC profile"));
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->status |= VP890_LINE_IN_CAL;

    /*
     * Copy input parameters to internal line obect. Used to "continue" running
     * calibration after this function returns
     */
    bFilterData->inputData = *(VpCalBFilterType *)inputArgs;
    bFilterData->inputData.pAcProfile = pAcProfile;

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Profile check: %p, %02X %02X %02X %02X %02X %02X",
        bFilterData->inputData.pAcProfile,
        bFilterData->inputData.pAcProfile[0],
        bFilterData->inputData.pAcProfile[1],
        bFilterData->inputData.pAcProfile[2],
        bFilterData->inputData.pAcProfile[3],
        bFilterData->inputData.pAcProfile[4],
        bFilterData->inputData.pAcProfile[5]));

    /*
     * Initialize vRms starting point to cause initial loading during
     * measurement and comparison phase
     */
    bFilterData->vRms = 0xFFFF;
    bFilterData->currentSet = 0;
    bFilterData->bestSet = 0;

    /* The MPI length field in this profile describes the length of
     * each entry, so (Length)/(length per entry) = number of entries.
     * Length is (profile_length - 2) because profile length includes the
     * version and MPI_LEN fields */
    mpiLen = pAcProfile[VP_PROFILE_MPI_LEN];

    if (VP_PROFILE_MPI_LEN + mpiLen + 1 >= pAcProfile[VP_PROFILE_LENGTH]) {
        /* This profile contains no additional B-filter sets */
        VP_ERROR(VpLineCtxType, pLineCtx, ("AC profile does not contain extra B-filter sets (based on profile length)"));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }

    bFilterData->listLength = pAcProfile[VP_PROFILE_MPI_LEN + mpiLen + 1];

    if (bFilterData->listLength == 0) {
        /* This profile contains no additional B-filter sets */
        VP_ERROR(VpLineCtxType, pLineCtx, ("AC profile does not contain extra B-filter sets (based on list length byte)"));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalBFilter() - listLength: %d  target: %d   TS: %d",
        bFilterData->listLength,
        bFilterData->inputData.vRms,
        pDevObj->timeStamp));

    /* The first set of coefficients to try are in the main body of the AC
     * profile (index 0).  Use ConfigLine to apply them, and restore relative
     * gain settings afterward. */
    tempGxUserLevel = pLineObj->gxUserLevel;
    tempGxCidLevel = pLineObj->gxCidLevel;
    tempGrUserLevel = pLineObj->grUserLevel;

    vpStatus = Vp890ConfigLine(pLineCtx, pAcProfile, VP_PTABLE_NULL,
        VP_PTABLE_NULL);
    if (vpStatus != VP_STATUS_SUCCESS) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CalBFilter() - ConfigLine returned %d", vpStatus));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return vpStatus;
    }

    pLineObj->gxUserLevel = tempGxUserLevel;
    pLineObj->gxCidLevel = tempGxCidLevel;
    pLineObj->grUserLevel = tempGrUserLevel;
    vpStatus = Vp890SetRelGainInt(pLineCtx);
    if (vpStatus != VP_STATUS_SUCCESS) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CalBFilter() - Vp890SetRelGainInt returned %d", vpStatus));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return vpStatus;
    }

    /* Save off current register content that will be modified */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN,
        bFilterData->opFunct);

    bFilterData->opCond[0] = pLineObj->opCond[0];

    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN,
        bFilterData->convCfg);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN,
        bFilterData->vpGain);

#ifdef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL
    Vp890ReduceNoiseOutput(bFilterData->vpGain[0], deviceId, ecVal);
#endif

    /* Program device to measure B-Filter levels and start noise generator */
    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r5. Writing 0x%02X to Operating Functions",
        opFunctions[0]));
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, opFunctions);

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r7. Writing 0x%02X to Operating Conditions",
        opCond[0]));
    pLineObj->opCond[0] = opCond[0];

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, opCond);

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
        VP890_CONV_CFG_LEN, convCfg);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
        mpiIndex-1, &mpiBuffer[1]);

    /* Set timer for measurement */
    pLineObj->lineTimers.timers.fxoTimer.bCalTimer = 0;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp890CalBFilterInt()
 *  This function calibrates a selected block of the device/line.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp890CalBFilterInt(
    VpLineCtxType       *pLineCtx)
{
    Vp890LineObjectType *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx           = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;
    uint8 ecVal                     = pLineObj->ecVal;

    uint8 mpiIndex                  = 0;
    uint8 mpiBuffer[6 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN +
                        VP890_CONV_CFG_LEN + VP890_VP_GAIN_LEN +
                        VP890_B1_FILTER_LEN + VP890_B2_FILTER_LEN];

    Vp890CalBFilterData *bFilterData = &pLineObj->calLineData.typeData.bFilterData;
    uint8 bFilterResult[VP890_TX_PCM_DATA_LEN];
    uint16 vRms;

    uint8 convCfg[VP890_CONV_CFG_LEN] = {
        VP890_ENH_B_FILTER_AVG_DET
    };

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalBFilterInt()"));

    /* Read the current B-Filter Measured Data */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN,
        bFilterResult);
    vRms = (bFilterResult[0] << 8) | bFilterResult[1];

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalBFilterInt() - Set: %d    vRms: %d    TS: %d",
        bFilterData->currentSet, vRms, pDevObj->timeStamp));

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Profile check: %p, %02X %02X %02X %02X %02X %02X",
        bFilterData->inputData.pAcProfile,
        bFilterData->inputData.pAcProfile[0],
        bFilterData->inputData.pAcProfile[1],
        bFilterData->inputData.pAcProfile[2],
        bFilterData->inputData.pAcProfile[3],
        bFilterData->inputData.pAcProfile[4],
        bFilterData->inputData.pAcProfile[5]));

    /*
     * Compare it with "best known" value. If better, the current known value,
     * save the current B-Filter Coefficients and value.  Always save the first
     * set.
     */

    if (vRms < bFilterData->vRms || bFilterData->currentSet == 0) {
        bFilterData->vRms = vRms;

        /* Copy the current "better" set into the calibration data */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_B1_FILTER_RD, VP890_B1_FILTER_LEN,
            bFilterData->b1FiltData);
        VpMpiCmdWrapper(deviceId, ecVal, VP890_B2_FILTER_RD, VP890_B2_FILTER_LEN,
            bFilterData->b2FiltData);
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("              - BEST"));

        bFilterData->bestSet = bFilterData->currentSet;
    }

    /* NOTE:  Since index 0 refers to the coefficients in the main body of the
     * profile, the additional sets at the end start at index 1 */

    /*
     * Stop if the current measured data meets the customer requirements, OR
     * if all sets have been tested. Otherwise, repeat with the next set.
     */
    if ((bFilterData->vRms < bFilterData->inputData.vRms)
     || (bFilterData->currentSet >= bFilterData->listLength)) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r6. Writing 0x%02X to Operating Functions",
            bFilterData->opFunct[0]));

        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
            VP890_OP_FUNC_LEN, bFilterData->opFunct);

        VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r8. Writing 0x%02X to Operating Conditions",
            bFilterData->opCond[0]));
        pLineObj->opCond[0] = bFilterData->opCond[0];

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
            VP890_OP_COND_LEN, bFilterData->opCond);

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
            VP890_CONV_CFG_LEN, bFilterData->convCfg);

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
            VP890_VP_GAIN_LEN, bFilterData->vpGain);

        VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalBFilterInt() - Done"));

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_B1_FILTER_WRT,
            VP890_B1_FILTER_LEN, bFilterData->b1FiltData);

		/*BEGIN: fix MOTO*/
		if (26 <= mpiIndex)
		{
			return VP_STATUS_FAILURE;
		}
		/*END: fix MOTO*/
        mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_B2_FILTER_WRT,
            VP890_B2_FILTER_LEN, bFilterData->b2FiltData);

        VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
            mpiIndex-1, &mpiBuffer[1]);

        pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
        pLineObj->lineEventHandle = (uint16)(bFilterData->bestSet);
        pLineObj->responseData = bFilterData->vRms;

        VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalBFilterInt() - Done"));
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("bFilterData->vRms: %d\nbFilterData->inputData.vRms: %d\nbFilterData->currentSet: %d\nbFilterData->listLength: %d",bFilterData->vRms,bFilterData->inputData.vRms,bFilterData->currentSet,bFilterData->listLength));
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("lineEvents.response = %04X", pLineObj->lineEvents.response));
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("lineEventHandle = %04X", pLineObj->lineEventHandle));
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("responseData = %04X", pLineObj->responseData));

        pLineObj->status &= ~VP890_LINE_IN_CAL;
    } else {
        /* Load the next set of coefficients and start the measurement */
        VpProfileDataType   *pMpiData;
        uint8               profileIndex;
        uint8               firstEntry;
        uint8               bFilterLength;
        uint8               filterDataCnt;

        bFilterData->currentSet++;

        firstEntry =
            (VP_PROFILE_MPI_LEN + bFilterData->inputData.pAcProfile[VP_PROFILE_MPI_LEN] + 3);

        bFilterLength = bFilterData->inputData.pAcProfile[firstEntry - 1];

        profileIndex = firstEntry + ((bFilterData->currentSet - 1) * bFilterLength);

        pMpiData = (VpProfileDataType *)(&bFilterData->inputData.pAcProfile[profileIndex]);

        for (filterDataCnt = 0; filterDataCnt < bFilterLength; filterDataCnt++) {
            VP_CALIBRATION(VpLineCtxType, pLineCtx,("0x%02X", pMpiData[filterDataCnt]));
        }

        /* Program this set of coefficients */
        VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD, bFilterLength, pMpiData);

        /* Make sure the converter configuration is correct */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
            convCfg);

        /* Timer is set to 65ms+ so that we can be sure the next measurement
         * will be a full 32ms taken with this set of coefficients. */
        pLineObj->lineTimers.timers.fxoTimer.bCalTimer = 0;
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalBFilterInt() - Setting timer for set %d",
            bFilterData->currentSet));
    }

    return VP_STATUS_SUCCESS;
}

static VpStatusType
Vp890CalApplyBFilter(
    VpLineCtxType       *pLineCtx,
    void                *inputArgs)
{
    Vp890LineObjectType *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx           = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;
    uint8 ecVal                     = pLineObj->ecVal;
    VpProfilePtrType pAcProfile;
    uint16 index;
    VpProfileDataType *pMpiData;
    uint8 mpiLen;
    uint8 listLength;
    uint8 bFilterLength;
    uint8 firstEntry;
    uint8 bFilterSetIndex;
    uint8 filterDataCnt;
    uint16 tempGxCidLevel;
    uint16 tempGxUserLevel;
    uint16 tempGrUserLevel;
    VpStatusType vpStatus;

    index = ((VpCalApplyBFilterType *)inputArgs)->index;
    pAcProfile = ((VpCalApplyBFilterType *)inputArgs)->pAcProfile;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalMeasureBFilter()"));

    if (pAcProfile == VP_NULL) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Invalid NULL AC profile"));
        return VP_STATUS_ERR_PROFILE;
    }

    if (index > 0xFF) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Invalid index %d", index));
        return VP_STATUS_INVALID_ARG;
    }

    /* Check the legality of the AC profile and look up the profile table
     * entry if necessary */
    if (!Vp890IsProfileValid(VP_PROFILE_AC,
            VP_CSLAC_AC_PROF_TABLE_SIZE, pDevObj->profEntry.acProfEntry,
            pDevObj->devProfileTable.pAcProfileTable,
            ((VpCalApplyBFilterType *)inputArgs)->pAcProfile,
            &pAcProfile)) {

        return VP_STATUS_ERR_PROFILE;
    }

    //for fix moto
    if (pAcProfile == VP_NULL) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Invalid NULL AC profile"));
        return VP_STATUS_ERR_PROFILE;
    }

    if (index == 0) {
        /* Index 0 refers to the coefficients in the main body of the AC
         * profile.  Use ConfigLine to apply them, and restore relative
         * gain settings afterward. */
        tempGxUserLevel = pLineObj->gxUserLevel;
        tempGxCidLevel = pLineObj->gxCidLevel;
        tempGrUserLevel = pLineObj->grUserLevel;

        vpStatus = Vp890ConfigLine(pLineCtx, pAcProfile, VP_PTABLE_NULL,
            VP_PTABLE_NULL);
        if (vpStatus != VP_STATUS_SUCCESS) {
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CalApplyBFilter() - ConfigLine returned %d", vpStatus));
            return vpStatus;
        }

        pLineObj->gxUserLevel = tempGxUserLevel;
        pLineObj->gxCidLevel = tempGxCidLevel;
        pLineObj->grUserLevel = tempGrUserLevel;
        vpStatus = Vp890SetRelGainInt(pLineCtx);
        if (vpStatus != VP_STATUS_SUCCESS) {
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890CalApplyBFilter() - Vp890SetRelGainInt returned %d", vpStatus));
            return vpStatus;
        }

        pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;

        return VP_STATUS_SUCCESS;
    }

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalApplyBFilter Profile check: %p, %02X %02X %02X %02X %02X %02X",
        pAcProfile,
        pAcProfile[0],
        pAcProfile[1],
        pAcProfile[2],
        pAcProfile[3],
        pAcProfile[4],
        pAcProfile[5]));

    mpiLen = pAcProfile[VP_PROFILE_MPI_LEN];

    if (VP_PROFILE_MPI_LEN + mpiLen + 1 >= pAcProfile[VP_PROFILE_LENGTH]) {
        /* This profile contains no additional B-filter sets */
        VP_ERROR(VpLineCtxType, pLineCtx,
            ("AC profile does not contain extra B-filter sets (based on profile length). %d >= %d",
            VP_PROFILE_MPI_LEN + mpiLen + 1, pAcProfile[VP_PROFILE_LENGTH]));
        return VP_STATUS_ERR_PROFILE;
    }

    listLength = pAcProfile[VP_PROFILE_MPI_LEN + mpiLen + 1];

    /* NOTE:  Since index==0 refers to the coefficients in the main body of the
     * profile, the additional sets at the end start at index 1 */

    if (index > listLength) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("index too high.  index %d, listLength %d", index, listLength));
        return VP_STATUS_INVALID_ARG;
    }

    bFilterLength = pAcProfile[VP_PROFILE_MPI_LEN + mpiLen + 2];

    firstEntry = VP_PROFILE_MPI_LEN + pAcProfile[VP_PROFILE_MPI_LEN] + 3;

    bFilterSetIndex = firstEntry + ((index - 1) * bFilterLength);

    pMpiData = (VpProfileDataType *)(&pAcProfile[bFilterSetIndex]);

    for (filterDataCnt = 0; filterDataCnt < bFilterLength; filterDataCnt++) {
        VP_CALIBRATION(VpLineCtxType, pLineCtx,("0x%02X", pMpiData[filterDataCnt]));
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Program this set of coefficients */
    VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD, bFilterLength, pMpiData);

    pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

static VpStatusType
Vp890CalMeasureBFilter(
    VpLineCtxType       *pLineCtx)
{
    Vp890LineObjectType *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx           = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;
    uint8 ecVal                     = pLineObj->ecVal;

    uint8 mpiIndex                  = 0;
    uint8 mpiBuffer[3 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN + VP890_CONV_CFG_LEN];

    Vp890CalBFilterData *bFilterData = &pLineObj->calLineData.typeData.bFilterData;

    uint8 convCfg[VP890_CONV_CFG_LEN] = {
        VP890_ENH_B_FILTER_AVG_DET
    };

    uint8 opFunctions[VP890_OP_FUNC_LEN] = {
        VP890_ENABLE_LOADED_COEFFICIENTS | VP890_LINEAR_CODEC
    };

    uint8 opCond[VP890_OP_COND_LEN] = {
        VP890_TXPATH_DIS | VP890_NOISE_GEN_EN
    };

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalMeasureBFilter()"));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->status |= VP890_LINE_IN_CAL;

    /* Save off current register content that will be modified */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN,
        bFilterData->opFunct);
    bFilterData->opCond[0] = pLineObj->opCond[0];

    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN,
        bFilterData->convCfg);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN,
        bFilterData->vpGain);

#ifdef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL
    Vp890ReduceNoiseOutput(bFilterData->vpGain[0], deviceId, ecVal);
#endif

    /* Program device to measure B-Filter levels and start noise generator */
    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r5. Writing 0x%02X to Operating Functions",
        opFunctions[0]));
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, opFunctions);

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r7. Writing 0x%02X to Operating Conditions",
        opCond[0]));
    pLineObj->opCond[0] = opCond[0];

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, opCond);

	/*BEGIN: fix MOTO*/
	if (6 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
        VP890_CONV_CFG_LEN, convCfg);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],  mpiIndex-1,
        &mpiBuffer[1]);

    /* Set timer for measurement */
    pLineObj->lineTimers.timers.fxoTimer.measureBFilterTimer = 0;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

VpStatusType
Vp890CalMeasureBFilterInt(
    VpLineCtxType       *pLineCtx)
{
    Vp890LineObjectType *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx           = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;
    uint8 ecVal                     = pLineObj->ecVal;
    uint8 mpiIndex                  = 0;
    uint8 mpiBuffer[4 + VP890_OP_FUNC_LEN + VP890_OP_COND_LEN +
                    VP890_CONV_CFG_LEN + VP890_VP_GAIN_LEN];

    Vp890CalBFilterData *bFilterData = &pLineObj->calLineData.typeData.bFilterData;
    uint8 bFilterResult[VP890_TX_PCM_DATA_LEN];
    uint16 vRms;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890CalMeasureBFilterInt()"));

    /* Read the current B-Filter Measured Data */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN,
        bFilterResult);
    vRms = (bFilterResult[0] << 8) | bFilterResult[1];

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalMeasureBFilterInt() - vRms: %d    TS: %d",
        vRms, pDevObj->timeStamp));

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r6. Writing 0x%02X to Operating Functions",
        bFilterData->opFunct[0]));

    /* Restore saved registers */
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_FUNC_WRT,
        VP890_OP_FUNC_LEN, bFilterData->opFunct);

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("\n\r8. Writing 0x%02X to Operating Conditions",
        bFilterData->opCond[0]));

    pLineObj->opCond[0] = bFilterData->opCond[0];

	/*BEGIN: fix MOTO*/
	if (8 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_OP_COND_WRT,
        VP890_OP_COND_LEN, bFilterData->opCond);

	/*BEGIN: fix MOTO*/
	if (8 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_CONV_CFG_WRT,
        VP890_CONV_CFG_LEN, bFilterData->convCfg);

	/*BEGIN: fix MOTO*/
	if (8 <= mpiIndex)
	{
		return VP_STATUS_FAILURE;
	}
	/*END: fix MOTO*/
    mpiIndex = VpCSLACBuildMpiBuffer(mpiIndex, mpiBuffer, VP890_VP_GAIN_WRT,
        VP890_VP_GAIN_LEN, bFilterData->vpGain);

    VpMpiCmdWrapper(deviceId, pLineObj->ecVal, mpiBuffer[0],
        mpiIndex-1, &mpiBuffer[1]);

    pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
    pLineObj->responseData = vRms;

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890CalMeasureBFilterInt() - Done"));
    VP_CALIBRATION(VpLineCtxType, pLineCtx,("lineEvents.response = %04X", pLineObj->lineEvents.response));
    VP_CALIBRATION(VpLineCtxType, pLineCtx,("responseData = %04X", pLineObj->responseData));

    pLineObj->status &= ~VP890_LINE_IN_CAL;

    return VP_STATUS_SUCCESS;
}

#ifdef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL
/* Adjust the digital gains as much as possible to lower the level of the
 * noise generator without changing the measurement results.
 * To do so, we increase the digital receive loss (DRL) while increasing
 * the digital transmit gain (DTG) by the same amount. */
static void
Vp890ReduceNoiseOutput(
    uint8 oldVpGain,
    VpDeviceIdType deviceId,
    uint8 ecVal)
{
    /* These lookup tables are indexed by the values of DRL and DTG that we
     * just read from the device.  The results of the lookup will be the
     * DRL and DTG values that give the most possible reduction in the noise
     * output level.
     * The restraints are that DRL is limited to 0db, -6db, -12dB, or -18dB
     * and DTG is limited to 0dB, +6dB, and +12dB.  This means that we can
     * achieve the best reduction if DTG is 0dB and DRL is 0dB or -6dB,
     * allowing us to change both by 12.  If DTG is already +12 or DRL is
     * already -18, we can make no change.
     */
    uint8 vpGainDrlLut[][4] = {
                 /* DRL=0dB         DRL=6dB         DRL=-12dB       DRL=-18dB */
    /* DTG=0dB */{VP890_DRL_12DB, VP890_DRL_18DB, VP890_DRL_18DB, VP890_DRL_18DB},
    /* DTG=6dB */{VP890_DRL_6DB,  VP890_DRL_12DB, VP890_DRL_18DB, VP890_DRL_18DB},
    /* DTG=12dB*/{VP890_DRL_0DB,  VP890_DRL_6DB,  VP890_DRL_12DB, VP890_DRL_18DB},
    /* DTG=rsv */{VP890_DRL_12DB, VP890_DRL_18DB, VP890_DRL_18DB, VP890_DRL_18DB},
    };
    uint8 vpGainDtgLut[][4] = {
                 /* DRL=00          DRL=01          DRL=10          DRL=11 */
    /* DTG=0dB */{VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_6DB,  VP890_DTG_0DB},
    /* DTG=6dB */{VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_6DB},
    /* DTG=12dB*/{VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_12DB},
    /* DTG=rsv */{VP890_DTG_12DB, VP890_DTG_12DB, VP890_DTG_6DB,  VP890_DTG_0DB},
    };
    uint8 drlIndex;
    uint8 dtgIndex;
    uint8 newVpGain;

    drlIndex = (oldVpGain & VP890_DRL_MASK);
    drlIndex <<= VP890_DRL_BITSHIFT;
    dtgIndex = (oldVpGain & VP890_DTG_MASK);

    newVpGain = oldVpGain;
    newVpGain &= ~VP890_DRL_MASK;
    newVpGain &= ~VP890_DTG_MASK;
    newVpGain |= vpGainDrlLut[dtgIndex][drlIndex];
    newVpGain |= vpGainDtgLut[dtgIndex][drlIndex];

    VP_CALIBRATION(VpLineCtxType, pLineCtx,("Vp890ReduceNoiseOutput(): oldVpGain 0x%02X, drlIndex %d, dtgIndex %d, newVpGain 0x%02X",
        oldVpGain, drlIndex, dtgIndex, newVpGain));

    VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_WRT, VP890_VP_GAIN_LEN, &newVpGain);
}
#endif /* VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL */
#endif /* VP890_FXO_SUPPORT */
#endif /* VP_CC_890_SERIES */
