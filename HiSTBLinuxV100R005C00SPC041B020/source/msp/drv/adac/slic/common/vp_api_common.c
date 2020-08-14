/** \file vp_api_common.c
 * vp_api_common.c
 *
 *  This file contains functions that are common to more than one device type
 * but not accessible to the user
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7350 $
 * $LastChangedDate: 2010-06-15 16:36:44 -0500 (Tue, 15 Jun 2010) $
 */

#include "vp_api_cfg.h"

/* INCLUDES */
#include "vp_api.h"     /* Typedefs and function prototypes for API */
#include "vp_api_int.h" /* Device specific typedefs and function prototypes */
#include "sys_service.h"

#include "vp_hal.h"

#if defined (VP_CC_880_SERIES)
#include "vp880_api_int.h"
#endif

static VpProfileType
ConvertPrfWizPrfType2ApiType(
    const VpProfileWizProfileType type);


#if (defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) \
 || defined(VP_CC_890_SERIES) || defined(VP_CC_580_SERIES))

#undef MPI_CMD_SEARCH
#define MPI_SHORT_FORMAT
#undef MPI_LONG_FORMAT

#if (VP_CC_DEBUG_SELECT & VP_DBG_HAL)
#if defined(MPI_CMD_SEARCH)

#define MPI_CMD_TO_FIND (0x60u)

static bool
VpMpiFindCmd(
    uint8 byteMatch,
    uint8 mpiCmdLen,
    uint8 *dataBuffer);

#endif /* MPI_CMD_SEARCH */
#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_HAL) */
#endif /* (790 | 880 | 890 | 580) */

/**
 * ConvertPrfWizPrfType2ApiType()
 *  This function converts from Profile Wizard profile type to VP-API specific
 * profile type.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
VpProfileType
ConvertPrfWizPrfType2ApiType(
    const VpProfileWizProfileType type)   /* Profile to be converted */
{
    switch (type) {
        case VP_PRFWZ_PROFILE_AC:              return VP_PROFILE_AC;
        case VP_PRFWZ_PROFILE_DC:              return VP_PROFILE_DC;
        case VP_PRFWZ_PROFILE_TONE:            return VP_PROFILE_TONE;
        case VP_PRFWZ_PROFILE_TONECAD:         return VP_PROFILE_TONECAD;
        case VP_PRFWZ_PROFILE_RING:            return VP_PROFILE_RING;
        case VP_PRFWZ_PROFILE_CID_TYPE1:       return VP_PROFILE_CID;
        case VP_PRFWZ_PROFILE_CID_TYPE2:       return VP_PROFILE_CID;
        case VP_PRFWZ_PROFILE_METER:           return VP_PROFILE_METER;
        case VP_PRFWZ_PROFILE_RINGCAD:         return VP_PROFILE_RINGCAD;
        case VP_PRFWZ_PROFILE_DEVICE:          return VP_PROFILE_DEVICE;
        case VP_PRFWZ_PROFILE_FXO_CONFIG:      return VP_PROFILE_FXO_CONFIG;
        case VP_PRFWZ_PROFILE_FXS_CTRL:        return VP_PROFILE_CUSTOM_TERM;
        case VP_PRFWZ_PROFILE_CAL:             return VP_PROFILE_CAL;
        default:                               return VP_NUM_PROFILE_TYPES;
    }
} /* ConvertPrfWizPrfType2ApiType() */

/**
 * VpGetProfileIndex()
 *  This function returns TRUE if the passed profile pointer is an index or just
 * a normal pointer. If the passed profile pointer is an index then the index is
 * also returned.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
int
VpGetProfileIndex (
    const VpProfilePtrType pProfile) /* Given Profile pointer */
{
    if((pProfile >= VP_PTABLE_INDEX1) && (pProfile <= VP_PTABLE_INDEX15)){
        if(pProfile == VP_PTABLE_INDEX1) {return 0;}
        else if(pProfile == VP_PTABLE_INDEX2) {return 1;}
        else if(pProfile == VP_PTABLE_INDEX3) {return 2;}
        else if(pProfile == VP_PTABLE_INDEX4) {return 3;}
        else if(pProfile == VP_PTABLE_INDEX5) {return 4;}
        else if(pProfile == VP_PTABLE_INDEX6) {return 5;}
        else if(pProfile == VP_PTABLE_INDEX7) {return 6;}
        else if(pProfile == VP_PTABLE_INDEX8) {return 7;}
        else if(pProfile == VP_PTABLE_INDEX9) {return 8;}
        else if(pProfile == VP_PTABLE_INDEX10) {return 9;}
        else if(pProfile == VP_PTABLE_INDEX11) {return 10;}
        else if(pProfile == VP_PTABLE_INDEX12) {return 11;}
        else if(pProfile == VP_PTABLE_INDEX13) {return 12;}
        else if(pProfile == VP_PTABLE_INDEX14) {return 13;}
        else if(pProfile == VP_PTABLE_INDEX15) {return 14;}
    }
    return -1;
} /* VpGetProfileIndex() */

/**
 * VpVerifyProfileType()
 *  This function verifies that the profile pointer passed matches the type of
 * profile being passed.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  Returns TRUE if the profile type and profile match.  Otherwise returns
 * FALSE.  Note that a NULL profile is valid and has specific meanings in the
 * API-II depending on the profile.
 */
bool
VpVerifyProfileType(
    VpProfileType type,
    VpProfilePtrType pProfile)
{
    if (pProfile == VP_PTABLE_NULL) {
        return TRUE;
    } else if ((pProfile >= VP_PTABLE_INDEX1)
            && (pProfile <= VP_PTABLE_INDEX15)){
        /* This function does not expect to see profile indexes */
        return FALSE;
    }

    if (ConvertPrfWizPrfType2ApiType((VpProfileWizProfileType)pProfile[VP_PROFILE_TYPE_LSB]) != type) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * VpIsDigit()
 *  This function returns TRUE if the digit passed is a valid VpDigitType,
 * otherwise returns FALSE. Utility function for the API-II.
 */
bool
VpIsDigit(
    VpDigitType digit)
{
    if ((digit >= 0) && (digit <= 9)) {
        return TRUE;
    }

    switch(digit) {
        case VP_DIG_ZERO:
        case VP_DIG_ASTER:
        case VP_DIG_POUND:
        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
        case VP_DIG_NONE:
            return TRUE;
        default:
            return FALSE;
    }
}

/* Code used for CSLAC & 792 */
#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) \
 || defined(VP_CC_890_SERIES) || defined(VP_CC_792_SERIES) \
 || defined(VP_CC_580_SERIES)


/** COMMON INITIALIZATION FUNCTIONS */
/**
 * VpImplementNonMaskEvents()
 *  This function modifies the line and device event structures with the API
 * standard non-masking event bits.  A non-masked event bit is 0.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  The event structures passed are modified by the non-masked event bits.
 */
void
VpImplementNonMaskEvents(
    VpOptionEventMaskType *pLineEventsMask, /**< Line Events Mask to modify for
                                             * non-masking
                                             */
    VpOptionEventMaskType *pDevEventsMask)  /**< Device Events Mask to modify
                                             * for non-masking
                                             */
{
    pLineEventsMask->faults     &= ~VP_API_NONMASK_FAULT_EVENTS;
    pLineEventsMask->signaling  &= ~VP_API_NONMASK_SIGNALING_EVENTS;
    pLineEventsMask->response   &= ~VP_API_NONMASK_RESPONSE_EVENTS;
    pLineEventsMask->test       &= ~VP_API_NONMASK_TEST_EVENTS;
    pLineEventsMask->process    &= ~VP_API_NONMASK_PROCESS_EVENTS;
    pLineEventsMask->fxo        &= ~VP_API_NONMASK_FXO_EVENTS;
    pLineEventsMask->packet     &= ~VP_API_NONMASK_PACKET_EVENTS;

    pDevEventsMask->faults      &= ~VP_API_NONMASK_FAULT_EVENTS;
    pDevEventsMask->signaling   &= ~VP_API_NONMASK_SIGNALING_EVENTS;
    pDevEventsMask->response    &= ~VP_API_NONMASK_RESPONSE_EVENTS;
    pDevEventsMask->test        &= ~VP_API_NONMASK_TEST_EVENTS;
    pDevEventsMask->process     &= ~VP_API_NONMASK_PROCESS_EVENTS;
    pDevEventsMask->fxo         &= ~VP_API_NONMASK_FXO_EVENTS;
    pDevEventsMask->packet      &= ~VP_API_NONMASK_PACKET_EVENTS;

    return;
}

/**
 * VpImplementDefaultSettings()
 *  This function executes the options to set the device/lines to API-II
 * standard default settings.  It may be passed a valid device context, or a
 * valid line context.  The device and line context do not need to be associated
 * with each other.  This is a convenient function for the API itself to use
 * when a device or line is initialized.
 *
 * Preconditions:
 * None
 *
 * Postconditions:
 * The device and line associated with this device is initialized with default
 * values.
 */
VpStatusType
VpImplementDefaultSettings(
    VpDevCtxType *pDevCtx,      /**< Device to implement for default API-II
                                 * options
                                 */
    VpLineCtxType *pLineCtx)    /**< Line to implement for default API-II
                                 * options
                                 */
{
    VpStatusType status = VP_STATUS_SUCCESS;

    VpSetOptionFuncPtrType pSetOption = VP_NULL;
    VpOptionPulseType pulseSpec;
    VpOptionLinePulseType linePulseSpec;
    VpOptionPulseType pulseSpec2;
    VpOptionCriticalFltType criticalFault;
    VpOptionZeroCrossType zeroCross;
    VpOptionPulseModeType pulseMode;
    VpOptionCodecType codec;
    VpOptionPcmHwyType pcmHwy;
    VpOptionLoopbackType loopBack;
    VpOptionLineStateType lineState;
    VpOptionRingControlType ringCtrl;
    VpOptionPcmTxRxCntrlType pcmTxRxCtrl;
    VpOptionDtmfSpecType dtmfSpec;
    VpOptionParkModeType parkMode;
    VpOptionEventMaskType eventMask;
    uint16 slopeRate = VP_OPTION_DEFAULT_DCFEED_SLOPE;
    VpOptionPcmSigCtlType pcmSigCtl;
    VpOptionLinestateCtlModeType linestateCtlMode = VP_OPTION_DEFAULT_LINESTATE_CTL_MODE;

    if ((pDevCtx == VP_NULL) && (pLineCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    if(pDevCtx != VP_NULL) {
        pSetOption = pDevCtx->funPtrsToApiFuncs.SetOption;
    } else {
        pSetOption = pLineCtx->pDevCtx->funPtrsToApiFuncs.SetOption;
    }

    if (pSetOption == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    }

    linePulseSpec.breakMin = pulseSpec.breakMin = VP_OPTION_DEFAULT_DP_BREAK_MIN;
    linePulseSpec.breakMax = pulseSpec.breakMax = VP_OPTION_DEFAULT_DP_BREAK_MAX;
    linePulseSpec.makeMin = pulseSpec.makeMin = VP_OPTION_DEFAULT_DP_MAKE_MIN;
    linePulseSpec.makeMax = pulseSpec.makeMax = VP_OPTION_DEFAULT_DP_MAKE_MAX;
    linePulseSpec.interDigitMin = pulseSpec.interDigitMin = VP_OPTION_DEFAULT_DP_INTER_DIG_MIN;
    linePulseSpec.flashMin = pulseSpec.flashMin = VP_OPTION_DEFAULT_DP_FLASH_MIN;
    linePulseSpec.flashMax = pulseSpec.flashMax = VP_OPTION_DEFAULT_DP_FLASH_MAX;
    linePulseSpec.onHookMin = VP_OPTION_DEFAULT_DP_ON_HOOK_MIN;
    linePulseSpec.offHookMin = VP_OPTION_DEFAULT_DP_OFF_HOOK_MIN;

    pulseSpec2.breakMin = VP_OPTION_DEFAULT_DP_BREAK_MIN2;
    pulseSpec2.breakMax = VP_OPTION_DEFAULT_DP_BREAK_MAX2;
    pulseSpec2.makeMin = VP_OPTION_DEFAULT_DP_MAKE_MIN2;
    pulseSpec2.makeMax = VP_OPTION_DEFAULT_DP_MAKE_MAX2;
    pulseSpec2.interDigitMin = VP_OPTION_DEFAULT_DP_INTER_DIG_MIN2;
    pulseSpec2.flashMin = VP_OPTION_DEFAULT_DP_FLASH_MIN2;
    pulseSpec2.flashMax = VP_OPTION_DEFAULT_DP_FLASH_MAX2;

#ifdef EXTENDED_FLASH_HOOK
    pulseSpec.onHookMin = VP_OPTION_DEFAULT_DP_ON_HOOK_MIN;
    pulseSpec2.onHookMin = VP_OPTION_DEFAULT_DP_ON_HOOK_MIN2;
#endif

    pulseMode = VP_OPTION_DEFAULT_PULSE_MODE;

    criticalFault.acFltDiscEn = VP_OPTION_DEFAULT_CF_AC_DIS_EN;
    criticalFault.dcFltDiscEn = VP_OPTION_DEFAULT_CF_DC_DIS_EN;
    criticalFault.thermFltDiscEn = VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN;

    zeroCross = VP_OPTION_DEFAULT_ZERO_CROSS;

    codec = VP_OPTION_DEFAULT_CODEC_MODE;
    pcmHwy = VP_OPTION_DEFAULT_PCM_HWY;

    pcmTxRxCtrl = VP_OPTION_DEFAULT_PCM_TXRX_CNTRL;

    loopBack = VP_OPTION_DEFAULT_LOOP_BACK;
    lineState.bat = VP_OPTION_DEFAULT_LS_BAT;
    lineState.battRev = VP_OPTION_DEFAULT_LS_BAT_REV;

    eventMask.faults = (uint16)VP_OPTION_DEFAULT_FAULT_EVENT_MASK;
    eventMask.signaling = (uint16)VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK;
    eventMask.response = (uint16)VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK;
    eventMask.test = (uint16)VP_OPTION_DEFAULT_TEST_EVENT_MASK;
    eventMask.process = (uint16)VP_OPTION_DEFAULT_PROCESS_EVENT_MASK;
    eventMask.fxo = (uint16)VP_OPTION_DEFAULT_FXO_EVENT_MASK;
    eventMask.packet = (uint16)VP_OPTION_DEFAULT_PACKET_EVENT_MASK;

    ringCtrl.ringExitDbncDur = VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL;
    ringCtrl.ringTripExitSt = VP_OPTION_DEFAULT_RC_RING_EXIT_STATE;
    ringCtrl.zeroCross = VP_OPTION_DEFAULT_RC_ZERO_CROSS;

    pcmTxRxCtrl = VP_OPTION_DEFAULT_PCM_TXRX_CNTRL;

    dtmfSpec = VP_OPTION_DEFAULT_DTMF_SPEC;

    parkMode.discTime = VP_OPTION_DEFAULT_PARK_MODE_DISC;
    parkMode.standbyTime = VP_OPTION_DEFAULT_PARK_MODE_STANDBY;

    pcmSigCtl.enable = VP_OPTION_DEFAULT_PCM_SIG_CTL_ENABLE;
    pcmSigCtl.ctlTimeslot = VP_OPTION_DEFAULT_PCM_SIG_CTL_CTLTS;
    pcmSigCtl.sigTimeslot = VP_OPTION_DEFAULT_PCM_SIG_CTL_SIGTS;

    if (pDevCtx != VP_NULL) {
        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PULSE,
            &pulseSpec);
        if (status == VP_STATUS_OPTION_NOT_SUPPORTED) {
            /* This device only supports VP_OPTION_ID_PULSE. */
            status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PULSE,
                &linePulseSpec);
        }
        if ((status != VP_STATUS_SUCCESS) &&
            (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PULSE2,
            &pulseSpec2);
        if ((status != VP_STATUS_SUCCESS) &&
            (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        /*
         * Some devices do not support AC/DC Fault detection, so setting the
         * critical fault may not be successful. However, all devices (known)
         * support thermal fault detection, so set that to the default
         */
        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_CRITICAL_FLT,
            &criticalFault);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            criticalFault.acFltDiscEn = FALSE;
            criticalFault.dcFltDiscEn = FALSE;
            status = pSetOption(VP_NULL, pDevCtx,
                VP_DEVICE_OPTION_ID_CRITICAL_FLT, &criticalFault);
            if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
                return status;
            }
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PULSE_MODE,
            &pulseMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_CODEC, &codec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PCM_HWY, &pcmHwy);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_LOOPBACK, &loopBack);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_LINE_STATE,
            &lineState);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_EVENT_MASK,
            &eventMask);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_RING_CNTRL,
            &ringCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PCM_TXRX_CNTRL,
            &pcmTxRxCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_DTMF_SPEC, &dtmfSpec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PARK_MODE, &parkMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_DCFEED_SLOPE, &slopeRate);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PCM_SIG_CTL, &pcmSigCtl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_LINESTATE_CTL_MODE, &linestateCtlMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }
    }

    if (pLineCtx != VP_NULL) {
        /* Init only line level options */
        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PULSE, &linePulseSpec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PULSE_MODE,
            &pulseMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_CODEC, &codec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PCM_HWY, &pcmHwy);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_LOOPBACK,
            &loopBack);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_LINE_STATE,
            &lineState);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_RING_CNTRL,
            &ringCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PCM_TXRX_CNTRL,
            &pcmTxRxCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_DTMF_SPEC, &dtmfSpec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_DCFEED_SLOPE, &slopeRate);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_LINESTATE_CTL_MODE, &linestateCtlMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }
    }

    return VP_STATUS_SUCCESS;
}
#endif /* 790 | 880 | 890 | 792 | 580 */

/* Code used for CSLAC only */
#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) \
 || defined(VP_CC_890_SERIES) || defined(VP_CC_580_SERIES)

#if defined(VP_CC_880_SERIES) || defined(VP_CC_890_SERIES)
int16
VpConvertToInt16(
    uint8 *dataPtr)
{
    return (int16)((((uint16)dataPtr[0] << 8) & 0xFF00) | (dataPtr[1] & 0xFF));
}

int32
VpConvertToInt32(
    uint8 *dataPtr)
{
    return (int32)((((uint32)dataPtr[0] << 24) & 0xFF000000)
                 | (((uint32)dataPtr[1] << 16) & 0x00FF0000)
                 | (((uint32)dataPtr[2] << 8) & 0x0000FF00)
                 | ((uint32)dataPtr[3] & 0xFF));
}

/**
 * VpCSLACSetAbsGain()
 *
 *    This function implements VP_OPTION_ID_ABS_GAIN for 880 and 890 devices.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
#ifdef CSLAC_GAIN_ABS
VpStatusType
VpCSLACSetAbsGain(
    VpLineCtxType *pLineCtx,
    VpOptionAbsGainType *gains)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpDeviceIdType deviceId;
    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    VpStatusType status = VP_STATUS_SUCCESS;

    uint8 ecVal, gxCmd, grCmd, grCmdLen, gxCmdLen;

    int16 grMaxGain;
    int16 grMinGain;

    int16 gxMaxGain;
    int16 gxMinGain;

    int16 *pGxAbsGain, *pGrAbsGain;

    bool updateGr = FALSE;
    bool updateGx = FALSE;

#define VP_GX_TABLE_SIZE (50)
    /* This is the quiet value. Overwrite if a non-quiet level is specified. */
    uint8 gxValue[] = {0xF8, 0xF8};

    uint8 gxGainLookup[VP_GX_TABLE_SIZE] = {
       /* GX Setting ABS: FXS    880/890-FXO */
       /* ---------------------------------- */
        0x01, 0x90,  /*  -6dB      -12dB     */
        0xAA, 0xC4,  /* -5.5dB    -11.5dB    */
        0xCA, 0xD3,  /*  -5dB      -11dB     */
        0x5E, 0xA2,  /* -4.5dB    -10.5dB    */
        0x33, 0x52,  /*  -4dB      -10dB     */
        0x22, 0xA1,  /* -3.5dB     -9.5dB    */
        0x2A, 0xA1,  /*  -3dB      -9dB      */
        0x3D, 0xF1,  /* -2.5dB     -8.5dB    */
        0x2A, 0x21,  /*  -2dB      -8dB      */
        0x32, 0xA0,  /* -1.5dB     -7.5dB    */
        0xBB, 0xA0,  /*  -1dB      -7dB      */
        0x3C, 0xB0,  /* -0.5dB     -6.5dB    */
        0xA9, 0xF0,  /*  0dB       -6dB      */
        0xAB, 0x30,  /* +0.5dB     -5.5dB    */
        0xAC, 0x20,  /*  +1dB      -5dB      */
        0x5A, 0x10,  /* +1.5dB     -4.5dB    */
        0xA5, 0x10,  /*  +2dB      -4dB      */
        0x22, 0x10,  /* +2.5dB     -3.5dB    */
        0xAA, 0x00,  /*  +3dB      -3dB      */
        0xCE, 0x00,  /* +3.5dB     -2.5dB    */
        0x23, 0x00,  /*  +4dB      -2dB      */
        0xA1, 0x00,  /* +4.5dB     -1.5dB    */
        0x31, 0x00,  /*  +5dB      -1dB      */
        0xA0, 0x00,  /* +5.5dB     -0.5dB    */
        0xE0, 0x00   /*  +6dB       0dB      */
    };

#define VP_GR_TABLE_SIZE (50)
    /* This is the quiet value. Overwrite if a non-quiet level is specified. */
    uint8 grValue[] = {0x8F, 0x87};

    uint8 grGainLookup[VP_GR_TABLE_SIZE] = {
       /* GR Setting ABS: FXS/890-FXO  880-FXO  */
       /* ------------------------------------- */
        0xA9, 0x72,  /*     -12dB       -9dB    */
        0xB5, 0x42,  /*     -11.5dB     -8.5dB  */
        0x87, 0x32,  /*     -11dB       -8dB    */
        0xBA, 0x22,  /*     -10.5dB     -7.5dB  */
        0xC4, 0x22,  /*     -10dB       -7dB    */
        0x22, 0xA1,  /*     -9.5dB      -6.5dB  */
        0x23, 0xA1,  /*     -9dB        -6dB    */
        0xBF, 0xA1,  /*     -8.5dB      -5.5dB  */
        0xAA, 0xA1,  /*     -8dB        -5dB    */
        0x62, 0xB1,  /*     -7.5dB      -4.5dB  */
        0x2B, 0xB1,  /*     -7dB        -4dB    */
        0x3B, 0xC1,  /*     -6.5dB      -3.5dB  */
        0xA8, 0x71,  /*     -6dB        -3dB    */
        0xAE, 0x41,  /*     -5.5dB      -2.5dB  */
        0x8F, 0x31,  /*     -5dB        -2dB    */
        0xCA, 0x21,  /*     -4.5dB      -1.5dB  */
        0xA4, 0x21,  /*     -4dB        -1dB    */
        0x22, 0xA0,  /*     -3.5dB      -0.5dB  */
        0xA2, 0xA0,  /*     -3dB         0dB    */
        0x87, 0xA0,  /*     -2.5dB      +0.5dB  */
        0xAA, 0xA0,  /*     -2dB        +1dB    */
        0x42, 0xB0,  /*     -1.5dB      +1.5dB  */
        0x5B, 0xB0,  /*     -1dB        +2dB    */
        0xBB, 0xC0,  /*     -0.5dB      +2.5dB  */
        0xA8, 0xF0   /*     0dB         +3dB    */
    };

    switch (deviceType) {
#ifdef VP_CC_880_SERIES
        case VP_DEV_880_SERIES:
            gxCmd = VP880_GX_GAIN_WRT;
            grCmd = VP880_GR_GAIN_WRT;
            grCmdLen = VP880_GR_GAIN_LEN;
            gxCmdLen = VP880_GX_GAIN_LEN;
            ecVal = ((Vp880LineObjectType *)pLineObj)->ecVal;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pGxAbsGain = &((Vp880LineObjectType *)pLineObj)->gain.absGxGain;
            pGrAbsGain = &((Vp880LineObjectType *)pLineObj)->gain.absGrGain;

            if (((Vp880LineObjectType *)pLineObj)->status & VP880_IS_FXO) {
                gxMaxGain = 0;
                gxMinGain = -120;

                grMaxGain = 30;
                grMinGain = -90;
            } else {
                gxMaxGain = 60;
                gxMinGain = -60;

                grMaxGain = 0;
                grMinGain = -120;
            }
            break;
#endif

#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            gxCmd = VP890_GX_GAIN_WRT;
            grCmd = VP890_GR_GAIN_WRT;
            grCmdLen = VP890_GR_GAIN_LEN;
            gxCmdLen = VP890_GX_GAIN_LEN;
            ecVal = ((Vp890LineObjectType *)pLineObj)->ecVal;
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            pGxAbsGain = &((Vp890LineObjectType *)pLineObj)->absGxGain;
            pGrAbsGain = &((Vp890LineObjectType *)pLineObj)->absGrGain;

            if (((Vp890LineObjectType *)pLineObj)->status & VP890_IS_FXO) {
                gxMaxGain = 0;
                gxMinGain = -120;

                grMaxGain = 0;
                grMinGain = -120;
            } else {
                gxMaxGain = 60;
                gxMinGain = -60;

                grMaxGain = 0;
                grMinGain = -120;
            }
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    VP_LINE_STATE(VpLineCtxType, pLineCtx, ("AToD: %d - DToA: %d",
        gains->gain_AToD, gains->gain_DToA));

    if (gains->gain_AToD != VP_OPTION_ABS_GAIN_NO_CHANGE) {
        if (gains->gain_AToD != VP_OPTION_ABS_GAIN_QUIET) {
            /* Roundoff to nearest 0.5dB step */
            int16 tempGain = ABS(gains->gain_AToD);
            int16 gainRound = tempGain % 5;
            tempGain += ((gainRound < 3) ? (-gainRound) : (5 - gainRound));
            tempGain = ((gains->gain_AToD < 0) ? -tempGain : tempGain);

            /* Limit the gain to device/line specific ranges */
            if (tempGain > gxMaxGain) {
                tempGain = gxMaxGain;
                status = VP_STATUS_INPUT_PARAM_OOR;
            } else if (tempGain < gxMinGain) {
                status = VP_STATUS_INPUT_PARAM_OOR;
                tempGain = gxMinGain;
            }

            /* Save the actual gain being applied before converting to index */
            *pGxAbsGain = tempGain;

            /* Convert to index */
            tempGain = (tempGain - gxMinGain);
            tempGain = tempGain / 5;

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Final AToD Value: %d",
                *pGxAbsGain));

			/*BEGIN: fix MOTO*/
			if ((uint8)tempGain * 2 + 1 >= VP_GX_TABLE_SIZE)
			{
				return VP_STATUS_INVALID_ARG;
			}
			/*END: fix MOTO*/
            gxValue[0] = gxGainLookup[(uint8)tempGain * 2];
            gxValue[1] = gxGainLookup[(uint8)tempGain * 2 + 1];
        }
        VpMpiCmd(deviceId, ecVal, gxCmd, gxCmdLen, gxValue);
        updateGx = TRUE;
    }

    if (gains->gain_DToA != VP_OPTION_ABS_GAIN_NO_CHANGE) {
        if (gains->gain_DToA != VP_OPTION_ABS_GAIN_QUIET) {
            /* Roundoff to nearest 0.5dB step */
            int16 tempGain = ABS(gains->gain_DToA);
            int16 gainRound = tempGain % 5;
            tempGain += ((gainRound < 3) ? (-gainRound) : (5 - gainRound));
            tempGain = ((gains->gain_DToA < 0) ? -tempGain : tempGain);

            /* Limit the gain to device/line specific ranges */
            if (tempGain > grMaxGain) {
                status = VP_STATUS_INPUT_PARAM_OOR;
                tempGain = grMaxGain;
            } else if (tempGain < grMinGain) {
                status = VP_STATUS_INPUT_PARAM_OOR;
                tempGain = grMinGain;
            }

            /* Save the actual gain being applied before converting to index */
            *pGrAbsGain = tempGain;

            /* Convert to index */
            tempGain = (tempGain - grMinGain);
            tempGain = tempGain / 5;

            VP_LINE_STATE(VpLineCtxType, pLineCtx, ("Final DToA Value: %d",
                *pGrAbsGain));

            grValue[0] = grGainLookup[(uint8)tempGain * 2];
            grValue[1] = grGainLookup[(uint8)tempGain * 2 + 1];
        }
        VpMpiCmd(deviceId, ecVal, grCmd, grCmdLen, grValue);
        updateGr = TRUE;
    }

#ifdef CSLAC_GAIN_RELATIVE
    /* Update cached transmit and receive gains for SetRelGain */
    switch (deviceType) {
#ifdef VP_CC_880_SERIES
        case VP_DEV_880_SERIES:
            if (updateGx == TRUE) {
                ((Vp880LineObjectType *)pLineObj)->gain.gxInt =
                    0x4000 + VpConvertCsd2Fixed(gxValue);
            }
            if (updateGr == TRUE) {
                ((Vp880LineObjectType *)pLineObj)->gain.grInt =
                    VpConvertCsd2Fixed(grValue);
            }
            break;
#endif
#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            if (updateGx == TRUE) {
                ((Vp890LineObjectType *)pLineObj)->gxBase =
                    0x4000 + VpConvertCsd2Fixed(gxValue);
            }
            if (updateGr == TRUE) {
                ((Vp890LineObjectType *)pLineObj)->grBase =
                    VpConvertCsd2Fixed(grValue);
            }
            break;
#endif
        default:
            break;
    }
#endif

    return status;
}
#endif
#endif

/**
 * ConvertApiState2PrfWizState()
 *
 *    Maps an API-II line state into the equivelent state value used in cadence
 *    profiles.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
uint8
ConvertApiState2PrfWizState(
    const VpLineStateType state)
{
    VpProfileCadencerStateTypes conversion[] = {
        VP_PROFILE_CADENCE_STATE_STANDBY, VP_PROFILE_CADENCE_STATE_TIP_OPEN,
        VP_PROFILE_CADENCE_STATE_ACTIVE, VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE,
        VP_PROFILE_CADENCE_STATE_TALK, VP_PROFILE_CADENCE_STATE_POLREV_TALK,
        VP_PROFILE_CADENCE_STATE_OHT, VP_PROFILE_CADENCE_STATE_POLREV_OHT,
        VP_PROFILE_CADENCE_STATE_DISCONNECT, VP_PROFILE_CADENCE_STATE_RINGING,
        VP_PROFILE_CADENCE_STATE_POLREV_RINGING,
        VP_PROFILE_CADENCE_STATE_FXO_OHT, VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN,
        VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE,
        VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK,
        VP_PROFILE_CADENCE_STATE_POLREV_STANDBY
    };

    return (uint8)(conversion[state]);
} /* ConvertApiState2PrfWizState() */

/**
 * InitTimerVars()
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
InitTimerVars(
    VpLineCtxType *pLineCtx)    /**< Line to initialize API Timer (internal)
                                 * Variables for
                                 */
{
    VpDeviceType deviceType = pLineCtx->pDevCtx->deviceType;
    void *pLineObj = pLineCtx->pLineObj;
    VpCslacTimerStruct *pTimer;

    switch(deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pTimer = &((Vp790LineObjectType *)pLineObj)->lineTimers;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pTimer = &((Vp880LineObjectType *)pLineObj)->lineTimers;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            pTimer = &((Vp890LineObjectType *)pLineObj)->lineTimers;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pTimer = &((Vp580LineObjectType *)pLineObj)->lineTimers;
            break;
#endif
        default:
            /* Nothing known to initialize */
            return;
    }

    if (pTimer->type == VP_CSLAC_FXS_TIMER) {
#if defined (VP_CC_790_SERIES) || defined (VP_CC_580_SERIES) \
 || (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) \
 || (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))
        uint8 i;
        for (i = 0; i < VP_LINE_TIMER_LAST; i++) {
            pTimer->timers.timer[i] = 0;
        }
#endif
    } else {
#if (defined(VP_CC_880_SERIES) && defined (VP880_FXO_SUPPORT)) || \
    (defined(VP_CC_890_SERIES) && defined (VP890_FXO_SUPPORT)) || defined(VP_CC_580_SERIES)
        pTimer->timers.fxoTimer.highToLowTime = 0;
        pTimer->timers.fxoTimer.prevHighToLowTime = 0x7FFF;
        pTimer->timers.fxoTimer.noCount = TRUE;
        pTimer->timers.fxoTimer.timeLastPolRev = 0x7FFF;
        pTimer->timers.fxoTimer.timePrevPolRev = 0x7FFF;
        pTimer->timers.fxoTimer.lastStateChange = 0;
        pTimer->timers.fxoTimer.disconnectDebounce = 0xFFFF;
        pTimer->timers.fxoTimer.liuDebounce = 0xFF;
        pTimer->timers.fxoTimer.ringOffDebounce = 0;
        pTimer->timers.fxoTimer.ringTimer = 0;
        pTimer->timers.fxoTimer.bCalTimer = 0xFF;
        pTimer->timers.fxoTimer.cidCorrectionTimer = 0;
        pTimer->timers.fxoTimer.pllRecovery = 0;
        pTimer->timers.fxoTimer.lowVoltageTimer = 0;
#endif
    }
}

#if (defined(VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
    (defined(VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
     defined(VP_CC_580_SERIES) || defined(VP_CC_790_SERIES)

/**
 * VpUpdateDP()
 *  This function measures the timing of the on/off-hook conditions for Dial
 * Pulsing and Flash Hook events (as well as the immediate Off-Hook, and long
 * On-Hook events).
 *
 * Preconditions:
 *  The pointers passed (Dial Pulse specifications, Dial Pulse data structure,
 * and Line Events structure) cannot be VP_NULL.
 *
 * Postconditions:
 *  The line events structure is updated with an event if the on/off-hook
 * variables provided in the dial pulse structure meet the specifications
 * provided in the dial pulse specification structure. Note: This function has
 * no knowledge of line context/objects, so it can be used for any type of
 * line.
 */
bool
VpUpdateDP(
    uint16 tickRate,    /**< Device API Tickrate used to measure real
                         * on/off-hook time
                         */

    VpOptionPulseType *pPulseSpecs,     /**< Dial Pulse specifications to apply
                                         * to on/off-hooks
                                         */
    VpDialPulseDetectType *pDpStruct,   /**< Structure used to maintain dial
                                         * pulse status
                                         */
    VpOptionEventMaskType *pLineEvents) /**< Line event structure to be modified
                                         * if an event is detected (and needs to
                                         * be reported).
                                         */
{
    bool eventStatus = FALSE;
    uint16 break_min, break_max;
    uint16 make_min, make_max;
    uint16 flash_min, flash_max;
    uint16 interDigitMin, conversionFactor;
    uint16 onHook_min;

    if ((pPulseSpecs == VP_NULL)
     || (pDpStruct == VP_NULL)
     || (pLineEvents == VP_NULL)) {
        return FALSE;
    }

    /* Get the specs in 125us units */
    break_min = pPulseSpecs->breakMin;
    break_max= pPulseSpecs->breakMax;
    make_min = pPulseSpecs->makeMin;
    make_max = pPulseSpecs->makeMax;
    flash_min = pPulseSpecs->flashMin;
    flash_max = pPulseSpecs->flashMax;

#ifdef EXTENDED_FLASH_HOOK
    onHook_min = pPulseSpecs->onHookMin;
#else
    onHook_min = flash_max;
#endif

    interDigitMin = pPulseSpecs->interDigitMin;
#define VP_API_TICKRATE_CONVERSION   (32)
    conversionFactor = (tickRate / VP_API_TICKRATE_CONVERSION);

    switch(pDpStruct->state) {
        case VP_DP_DETECT_STATE_IDLE:
            if(pDpStruct->hookSt) {
                /*
                 * We are off-hook after being on-hook for a "long time". Start
                 * dial pulsing
                 */
                pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                pDpStruct->lc_time = 0;
                eventStatus = TRUE;
                pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                pLineEvents->signaling &= ~VP_LINE_EVID_EXTD_FLASH;
                pLineEvents->signaling |= VP_LINE_EVID_HOOK_OFF;

                VP_HOOK(None, NULL, ("DP Off-Hook"));
            }
            break;

        case VP_DP_DETECT_STATE_LOOP_OPEN:
            pDpStruct->lo_time += conversionFactor;

            if(pDpStruct->hookSt) {
                /* Detected off-hook */

                if ((pDpStruct->lo_time >= break_min)
                 && (pDpStruct->lo_time <= break_max)) {
                    /*
                     * We think we just dialed a pulse, but don't count it if
                     * this sequence of on/off hooks has already been marked
                     * as invalid (-1)
                     */
                    if (pDpStruct->digits != -1) {
                        pDpStruct->digits++;
                    }

                    VP_HOOK(None, NULL, ("Digit Open Time %d",
                        pDpStruct->lo_time));

                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                } else if ((pDpStruct->lo_time >= flash_min)
                        && (pDpStruct->lo_time <= flash_max)) {
                    /* We did a hook flash */
                    pDpStruct->signalingData =
                        pDpStruct->lo_time * (tickRate >> 8);

                    eventStatus = TRUE;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                    pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                    pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                    pLineEvents->signaling &= ~VP_LINE_EVID_EXTD_FLASH;
                    pLineEvents->signaling |= VP_LINE_EVID_FLASH;

                    VpInitDP(pDpStruct);
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                    pDpStruct->lo_time = flash_max-1;

                    VP_HOOK(None, NULL, ("Generating Flash Hook Event in DP Detect"));
                } else if ((pDpStruct->lo_time > flash_max)
                        && (pDpStruct->lo_time <= onHook_min)) {
                    /*
                     * We did something between hook flash and on-hook. This is
                     * defined as a "new call" event.
                     */
                    pDpStruct->signalingData =
                        pDpStruct->lo_time * (tickRate >> 8);

                    eventStatus = TRUE;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                    pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                    pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                    pLineEvents->signaling |= VP_LINE_EVID_EXTD_FLASH;
                    pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;

                    VpInitDP(pDpStruct);
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                    pDpStruct->lo_time = onHook_min-1;

                    VP_HOOK(None, NULL, ("Generating New Call Event in DP Detect"));
                } else {
                    /* This occurs for invalid digits */
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;

                    /* Mark this sequence of digits invalid */
                    pDpStruct->digits = -1;
                }
                pDpStruct->lc_time = 0;
            } else {
                if (pDpStruct->lo_time > onHook_min) {
                    VP_HOOK(None, NULL, ("Generating Hook-On Event in DP Detect"));

                    /* We're on-hook, report and start over */
                    VpInitDP(pDpStruct);
                    eventStatus = TRUE;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                    pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                    pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                    pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                    pLineEvents->signaling |= VP_LINE_EVID_HOOK_ON;
                    pLineEvents->signaling &= ~VP_LINE_EVID_EXTD_FLASH;
                }
            }
            break;

        case VP_DP_DETECT_STATE_LOOP_CLOSE:
            /* Limit the value lc_time can reach so we don't overflow */
            if(pDpStruct->lc_time < (0xFFFF - conversionFactor)) {
                pDpStruct->lc_time += conversionFactor;
            }

            /* Check to see if we're still off-hook */
            if(pDpStruct->hookSt) {
                /* We're still off-hook. Did we reach the interdigit time? */
                if (pDpStruct->lc_time >= interDigitMin &&
                    pDpStruct->lc_time < interDigitMin + conversionFactor) {
                    /* Interdigit time reached. Report digits collected */
                    if (pDpStruct->digits > 0) {
                        /* We have dialed digits */
                        eventStatus = TRUE;
                        pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                        pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                        pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                        pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                        pLineEvents->signaling |= VP_LINE_EVID_PULSE_DIG;
                        pDpStruct->signalingData = pDpStruct->digits;
                        pLineEvents->signaling &= ~VP_LINE_EVID_EXTD_FLASH;
                    } else {
                        /*
                         * If we're still off-hook, we either didn't collect
                         * digits, the digits were invalid, this was an initial
                         * off-hook or a Hook Flash. If the digits were not
                         * invalid, the event has been reported. Only report an
                         * event for invalid digits.
                         */
                        if (pDpStruct->digits < 0) {
                            eventStatus = TRUE;
                            pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                            pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                            pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                            pLineEvents->signaling &= ~VP_LINE_EVID_BREAK_MAX;
                            pLineEvents->signaling |= VP_LINE_EVID_PULSE_DIG;
                            pLineEvents->signaling &= ~VP_LINE_EVID_EXTD_FLASH;
                            pDpStruct->signalingData = VP_DIG_NONE;
                        }
                    }
                }
            } else {
                /* Detected on-hook. */

                /* If the interdigit time has passed or there are no digits
                 * counted yet, this is a new pulse sequence */
                if (pDpStruct->lc_time >= interDigitMin ||
                    pDpStruct->digits == 0)
                {
                    eventStatus = TRUE;
                    pLineEvents->signaling |= VP_LINE_EVID_STARTPULSE;
                    VpInitDP(pDpStruct);

                }
                /* If this isn't the beginning of a pulse sequence and we're
                 * outside of the make min/max, mark this sequence invalid */
                else if (pDpStruct->lc_time > make_max ||
                         pDpStruct->lc_time < make_min)
                {
                    pDpStruct->digits = -1;
                }

                VP_HOOK(None, NULL, ("Digit Close Time %d",
                    pDpStruct->lc_time));

                pDpStruct->state = VP_DP_DETECT_STATE_LOOP_OPEN;
                pDpStruct->lo_time = 0;
            }
            break;

        default:
            return FALSE;
    }

    return eventStatus;
}

/**
 * VpInitDP()
 *  Initializes the dial pulse structure variable passed to values required by
 * VpUpdateDP.
 *
 * Preconditions:
 *  The pointer passed (Dial data structure) cannot be VP_NULL.
 *
 * Postconditions:
 *  The data passed in the dial pulse data structure is initialized.
 */
void
VpInitDP(
    VpDialPulseDetectType *pDpStruct)   /**< Dial pulse structure to init */
{
    if (pDpStruct != VP_NULL) {
        pDpStruct->digits = 0;

        if (pDpStruct->hookSt == FALSE) {
            pDpStruct->state = VP_DP_DETECT_STATE_IDLE;
            pDpStruct->lo_time = 0xFFFF;
            pDpStruct->lc_time = 0;
        } else {
            pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
            pDpStruct->lo_time = 0;
            pDpStruct->lc_time = 0xFFFF;
        }
    }
    return;
}
#endif

/**
 * VpCSLACGetLineStatus()
 *  This function returns the status of the type being request for the line
 * (context) being passed.
 *
 * Preconditions:
 *  The line context pointer passed must be valid.
 *
 * Postconditions:
 *  The location pointed to by the boolean pointer passed is set to either TRUE
 * or FALSE depending on the status of the line for the type of input requested.
 * This function returns the success status code if the information requested
 * is valid for the line type (FXO/FXS) being passed.
 */
VpStatusType
VpCSLACGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

#if defined (VP_CC_580_SERIES) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
    defined (VP_CC_790_SERIES) || \
    (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT))
    VpDialPulseDetectType *pDpStruct = VP_NULL;
    bool resLeak = FALSE;
    VpOptionPulseModeType pulseMode = VP_OPTION_PULSE_DECODE_OFF;
#endif

    VpApiIntLineStateType *pLineState;
    VpDeviceDynamicInfoType *pDynamicInfo;
    bool fxo;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDpStruct = &((Vp790LineObjectType *)pLineObj)->dpStruct;
            pLineState = &((Vp790LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp790DeviceObjectType *)pDevObj)->dynamicInfo;
            fxo = FALSE;
            pulseMode = ((Vp790LineObjectType *)pLineObj)->pulseMode;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
#ifdef VP880_FXS_SUPPORT
            pDpStruct = &((Vp880LineObjectType *)pLineObj)->dpStruct;
            pulseMode = ((Vp880LineObjectType *)pLineObj)->pulseMode;
            resLeak = ((((Vp880LineObjectType *)pLineObj)->status) & VP880_LINE_LEAK)
                ? TRUE : FALSE;
#endif

            pLineState = &((Vp880LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp880DeviceObjectType *)pDevObj)->dynamicInfo;
            if (((Vp880LineObjectType *)pLineObj)->status & VP880_IS_FXO) {
                fxo = TRUE;
            } else {
                fxo = FALSE;
            }
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
#ifdef VP890_FXS_SUPPORT
            pDpStruct = &((Vp890LineObjectType *)pLineObj)->dpStruct;
            pulseMode = ((Vp890LineObjectType *)pLineObj)->pulseMode;
            resLeak = ((((Vp890LineObjectType *)pLineObj)->status) & VP890_LINE_LEAK)
                ? TRUE : FALSE;
#endif
            pLineState = &((Vp890LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp890DeviceObjectType *)pDevObj)->dynamicInfo;
            if (((Vp890LineObjectType *)pLineObj)->status & VP890_IS_FXO) {
                fxo = TRUE;
            } else {
                fxo = FALSE;
            }
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pDpStruct = &((Vp580LineObjectType *)pLineObj)->dpStruct;
            pLineState = &((Vp580LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp580DeviceObjectType *)pDevObj)->dynamicInfo;
            if (((Vp580LineObjectType *)pLineObj)->status & VP580_IS_FXO) {
                fxo = TRUE;
            } else {
                fxo = FALSE;
            }
            pulseMode = ((Vp580LineObjectType *)pLineObj)->pulseMode;
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (fxo == FALSE) {
        switch(input) {
#if  defined (VP_CC_580_SERIES) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
     defined (VP_CC_790_SERIES) || \
    (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT))
            case VP_INPUT_HOOK:
                if(pulseMode == VP_OPTION_PULSE_DECODE_ON) {
                    if ((pDpStruct != VP_NULL) && (pDpStruct->state == VP_DP_DETECT_STATE_IDLE)) {
                        *pStatus = FALSE;
                    } else {
                        *pStatus = TRUE;
                    }
                } else {
                    if ((pLineState->condition & VP_CSLAC_HOOK)
                     && (!(pLineState->condition & VP_CSLAC_LINE_LEAK_TEST))) {
                        *pStatus = TRUE;
                    } else {
                        *pStatus = FALSE;
                    }
                }
                break;

            case VP_INPUT_RAW_HOOK:
                if ((pLineState->condition & VP_CSLAC_HOOK)
                 && (!(pLineState->condition & VP_CSLAC_LINE_LEAK_TEST))) {
                    *pStatus = TRUE;
                } else {
                    *pStatus = FALSE;
                }
                break;

            case VP_INPUT_GKEY:
                *pStatus = (pLineState->condition & VP_CSLAC_GKEY)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_CLK_FLT:
                *pStatus = (pDynamicInfo->clkFault) ? TRUE : FALSE;
                break;

            case VP_INPUT_THERM_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_THERM_FLT)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_AC_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_AC_FLT)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_DC_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_DC_FLT)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_BAT1_FLT:
                *pStatus = (pDynamicInfo->bat1Fault) ? TRUE : FALSE;
                break;

            case VP_INPUT_BAT2_FLT:
                *pStatus = (pDynamicInfo->bat2Fault) ? TRUE : FALSE;
                break;

            case VP_INPUT_BAT3_FLT:
                *pStatus = (pDynamicInfo->bat3Fault) ? TRUE : FALSE;
                break;

			case VP_INPUT_RES_LEAK:
                *pStatus = resLeak;
				break;
#endif
            default:
                return VP_STATUS_INVALID_ARG;
        }
    } else {
        switch(input) {
            case VP_INPUT_CLK_FLT:
                *pStatus = (pDynamicInfo->clkFault) ? TRUE : FALSE;
                break;

            case VP_INPUT_RINGING:
                *pStatus = (pLineState->condition & VP_CSLAC_RINGING)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_POLREV:
                *pStatus = (pLineState->condition & VP_CSLAC_POLREV)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_LIU:
                *pStatus = (pLineState->condition & VP_CSLAC_LIU)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_FEED_DIS:
            case VP_INPUT_DISCONNECT:
                *pStatus = (pLineState->condition & VP_CSLAC_DISC)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_FEED_EN:
            case VP_INPUT_CONNECT:
                *pStatus = (pLineState->condition & VP_CSLAC_DISC)
                    ? FALSE : TRUE;
                break;

            default:
                return VP_STATUS_INVALID_ARG;
        }
    }
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACClearResults()
 *  This function clears the device read events so that normal read operations
 * may occur. It is done by the application when the device is busy (in read
 * moode) and the application does not know what to read.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  Device is no longer busy with read status.
 */
VpStatusType
VpCSLACClearResults(
    VpDevCtxType *pDevCtx)
{
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpDeviceIdType deviceId;
    VpOptionEventMaskType *pDeviceEvents;

    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDeviceEvents = &((Vp790DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pDeviceEvents = &((Vp880DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            pDeviceEvents = &((Vp890DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pDeviceEvents = &((Vp580DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDeviceEvents->response &= (~VP_CSLAC_READ_RESPONSE_MASK);
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACDtmfDigitDetected()
 *  This function is used to set a value in the API-II CSLAC library (where
 * applicable) indicating that a DTMF digit was detected in an external
 * application/process.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  A value in the API-II is set which indicates the digit detected. The most
 * recent value is stored.
 */
#if defined (VP_CC_880_SERIES) || defined (VP_CC_890_SERIES) || defined (VP_CC_790_SERIES)
VpStatusType
VpCSLACDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceIdType deviceId;
    VpDeviceType deviceType = pDevCtx->deviceType;
#ifdef VP_CSLAC_SEQ_EN
#if (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT)) || \
    defined (VP_CC_790_SERIES)
    VpCallerIdType *pCidStruct = VP_NULL;
#endif
#endif
    VpOptionEventMaskType *pLineEvents;
    uint16p pDtmfDigitSense;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
#ifdef VP_CSLAC_SEQ_EN
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
#endif
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pDtmfDigitSense = &((Vp790LineObjectType *)pLineObj)->dtmfDigitSense;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
#if defined (VP_CSLAC_SEQ_EN) && defined (VP880_FXS_SUPPORT)
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
#endif
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pDtmfDigitSense = &((Vp880LineObjectType *)pLineObj)->dtmfDigitSense;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
#if defined (VP_CSLAC_SEQ_EN) && defined (VP890_FXS_SUPPORT)
            pCidStruct = &((Vp890LineObjectType *)pLineObj)->callerId;
#endif
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            pLineEvents = &((Vp890LineObjectType *)pLineObj)->lineEvents;
            pDtmfDigitSense = &((Vp890LineObjectType *)pLineObj)->dtmfDigitSense;
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    switch (sense) {
        case VP_DIG_SENSE_BREAK:
        case VP_DIG_SENSE_MAKE:
#ifdef VP_CSLAC_SEQ_EN
#if (defined (VP_CC_880_SERIES) && defined (VP880_FXS_SUPPORT)) || \
    (defined (VP_CC_790_SERIES)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXS_SUPPORT))
            /*
             * If the CID sequencer is waiting for a CPE ACK tone, report
             * the DTMF event to the CID sequencer, but mask it from the
             * application.
             */
            if ((pCidStruct != VP_NULL) && ((pCidStruct->status & VP_CID_AWAIT_TONE) != 0)) {
                VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
                pCidStruct->digitDet = digit;
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_SUCCESS;
            }
#endif
#endif
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Toggle the DTMF_DIG event.  If two DTMF_DIG events are received within
     * the same API tick period, report neither. */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pLineEvents->signaling ^= VP_LINE_EVID_DTMF_DIG;
    *pDtmfDigitSense = digit | sense;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}
#endif

/**
 * VpGetReverseState()
 *  This function returns the polarity inverted API-II state from the state
 * passed.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  None. Helper function only.
 */
VpLineStateType
VpGetReverseState(
    VpLineStateType currentState)
{
    switch(currentState) {
        case VP_LINE_STANDBY:
            return VP_LINE_STANDBY_POLREV;

        case VP_LINE_ACTIVE:
            return VP_LINE_ACTIVE_POLREV;

        case VP_LINE_ACTIVE_POLREV:
            return VP_LINE_ACTIVE;

        case VP_LINE_TALK:
            return VP_LINE_TALK_POLREV;

        case VP_LINE_TALK_POLREV:
            return VP_LINE_TALK;

        case VP_LINE_OHT:
            return VP_LINE_OHT_POLREV;

        case VP_LINE_OHT_POLREV:
            return VP_LINE_OHT;

        case VP_LINE_RINGING:
            return VP_LINE_RINGING_POLREV;

        case VP_LINE_RINGING_POLREV:
            return VP_LINE_RINGING;

        case VP_LINE_STANDBY_POLREV:
            return VP_LINE_STANDBY;

        default:
            return currentState;
    }
}

/**
 * VpCSLACSetVas()
 *  This function sets the VAS values in dcFeed as specified by the device
 * dc feed register, with VAS value passed. It does not actually access the
 * device, just simply computes the correct hex values for the dc feed reg.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  Line not affected. Values in dcFeed contain the VAS values passed.
 */
void
VpCSLACSetVas(
    uint8 *dcFeed,
    uint16 vasValue)
{
    uint16 regValue = 0;

    if (vasValue >= 3000) {
        regValue = (vasValue - 3000) / 750;
    }

    dcFeed[0] &= 0xFC;
    dcFeed[1] &= 0x3F;

    dcFeed[0] |= ((uint8)(regValue & 0xC) >> 2);
    dcFeed[1] |= ((uint8)(regValue & 0x3) << 6);
}

#ifdef CSLAC_GAIN_RELATIVE
/**
 * VpConvertCsd2Fixed()
 *  This function returns a 2.14 fixed-point number whose value matches (as
 * nearly as possible) the value of a given CSD (canonical signed digit)
 * number.
 *
 * Preconditions:
 *  The CSD number must be split into a two-byte array consisting of the high
 * byte followed by the low byte.  Its value must be between 0 and 4.0.
 *
 * Postconditions:
 *  If the value of the passed CSD number is less than 0, 0 will be returned.
 * If the value is greater than or equal to 4.0, 65535 will be returned,
 * which means 3.99994 in 2.14 representation.
 */
uint16
VpConvertCsd2Fixed(
    uint8 *csdBuf)
{
    uint16 csd = (csdBuf[0] << 8) + csdBuf[1];
    int32 result;
    int8 bitPos, C, m;

    /* 2.14 fixed-point format has values ranging from 0 to 3.999.... The bits
     * have the following values:
     *
     *   bit    15    14    13    12          2     1     0
     *       +-----+-----+-----+-----+     +-----+-----+-----+
     * value | 2^1 | 2^0 | 2^-1| 2^-2| ... |2^-12|2^-13|2^-14|
     *       +-----+-----+-----+-----+     +-----+-----+-----+
     */

    /*
     * CSD format is as follows:
     *
     *   bit   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
     *       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * field |C40|    m40    |C30|    m30    |C20|    m20    |C10|    m10    |
     *       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *
     * where the represented value is calculated as follows (Cxy = 0 or 1
     * above corresponds to Cxy = 1 or -1, respectively, below):
     *
     *      C10 * 2^-m10 * (
     *          1 + C20 * 2^-m20 * (
     *              1 + C30 * 2^-m30 * (
     *                  1 + C40 * 2^-m40 )))
     */

    /*
     * Alternate formula which can be computed with unsigned ints:
     *
     *          C10 * 2^-m10
     *        + C10 * C20 * 2^(-m10-m20)
     *        + C10 * C20 * C30 * 2^(-m10-m20-m30)
     *        + C10 * C20 * C30 * C40 * 2^(-m10-m20-m30-m40)
     */

    C = m = result = 0;
    for (bitPos = 0; bitPos < 16; bitPos += 4) {
        C ^= (csd >> (bitPos + 3)) & 1;
        m += (csd >> bitPos) & 7;
        if (C == 0) {
            result += 0x4000 >> m;
        } else {
            result -= 0x4000 >> m;
        }
    }

    /* Ensure that the result can be stored in a uint16. */
    if (result > 0xFFFF) {
        result = 0xFFFF;
    } else if (result < 0) {
        result = 0;
    }

    return result;
} /* VpConvertCsd2Fixed() */

/**
 * VpConvertFixed2Csd()
 *  This function returns a four-nibble CSD (canonical signed digit) number
 * whose value matches (as nearly as possible) the supplied 2.14 fixed-point
 * number.
 *
 * Preconditions:
 *
 * Postconditions:
 *  The CSD number will be placed into a two-byte array (high byte first) at
 * the address specified in the csdBuf parameter.
 */
void
VpConvertFixed2Csd(
    uint16 fixed,
    uint8 *csdBuf)
{
#define CSD_NIBBLES 4
    uint16 error, power, greaterPower, smallerPower, distGreater, distSmaller;
    uint16 C, m, result, sum = 0;
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
} /* VpConvertFixed2Csd() */
#endif
#endif

#if (defined(VP_CC_890_SERIES) && defined(VP890_INCLUDE_TESTLINE_CODE)) \
    || (defined(VP_CC_880_SERIES) && defined(VP880_INCLUDE_TESTLINE_CODE)) \
    || defined(VP_CC_792_SERIES)
uint16
VpComputeSquareRoot(
    uint32 number)
{
    uint8 iteration;
    int32 sqrtEst = 2;
    const int32 sqrtShift = number / 2;
    const uint8 newtonItt = 3;

    /*
     * Find an estimate of the result in the correct octave
     * (approximately 1.5 bits of accuracy)
     */
    while ((sqrtEst * sqrtEst) < sqrtShift) {
        sqrtEst *= 2;
    }

    /*
     * Use Newton's iteration to improve the estimate of the square root
     * If the accuracy is N bits, on Newton's iteration increase the accuracy
     * to 2N+1 bits.
     */
     for (iteration = 0; iteration < newtonItt; iteration++) {
        if (0 == sqrtEst) {
            break;
        } else {
            sqrtEst = (sqrtEst +  (number / sqrtEst)) / 2 ;
        }
     }

    return (uint16)sqrtEst;
}
#endif

/**
 * VpMemCpyCheck - Copy one area of memory to another while checking if any of
 * the data changed.
 *
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Return: TRUE if any of the data from-to was different.
 */
EXTERN bool
VpMemCpyCheck(
    uint8 *dest,
    uint8 *src,
    uint16 count)
{
    bool dataChange = FALSE;
    uint16 currentIndex = 0;

    while (currentIndex < count) {
        currentIndex++;
        if (dest[currentIndex] != src[currentIndex]) {
            dataChange = TRUE;
            dest[currentIndex] = src[currentIndex];
        }
    }
    return dataChange;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 */
EXTERN void *
VpMemCpy(
    void * dest,
    const void *src,
    uint16 count)
{
    char *tmp = (char *) dest, *s = (char *) src;

    while (count--)
        *tmp++ = *s++;

    return dest;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 */
EXTERN void *
VpMemSet(
    void * s,
    int c,
    uint16 count)
{
    char *xs = (char *) s;

    while (count--)
        *xs++ = (char)c;

    return s;
}

#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) \
 || defined(VP_CC_890_SERIES) || defined(VP_CC_580_SERIES)
/**
 * Wrapper for VpMpiCmd() for purposes of providing VP_DBG_HAL output
 */
void
VpMpiCmdWrapper(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 mpiCmd,
    uint8 mpiCmdLen,
    uint8 *dataBuffer)
{
#if (VP_CC_DEBUG_SELECT & VP_DBG_HAL)
    /* Limit input from VpDeviceIdType to 255 */
#ifdef MPI_SHORT_FORMAT
    if (mpiCmd == 0xCF) {
        VP_HAL(None, NULL, ("Total Length: 17"));
    } else {
        VP_HAL(None, NULL, ("Device: %d EC Value: 0x%02X Type: %s Cmd: 0x%02X Total Length: %d",
            deviceId, ecVal, ((mpiCmd & 0x1) ? "READ" : "WRITE"), mpiCmd, (3 + mpiCmdLen)));
    }
#endif
#ifdef MPI_LONG_FORMAT
    {
        uint8 cmdIndex;

        if (mpiCmd == 0xCF) {
            VP_HAL(None, NULL, ("Cmd 0xCF - Read Length: 17"));
        } else {
            VP_HAL(None, NULL, ("%s Cmd 0x%02X", ((mpiCmd & 0x1) ? "READ" : "WRITE"), mpiCmd));
            for (cmdIndex = 0; cmdIndex < mpiCmdLen; cmdIndex++) {
                VP_HAL(None, NULL, (" 0x%02X", dataBuffer[cmdIndex]));
            }
        }
    }
#endif
#ifdef MPI_CMD_SEARCH
    if ((mpiCmd == MPI_CMD_TO_FIND)
     || (VpMpiFindCmd(MPI_CMD_TO_FIND, mpiCmdLen, dataBuffer) == TRUE)) {
        uint8 cmdIndex;

        VP_HAL(None, NULL, ("%s Cmd 0x%02X", ((mpiCmd & 0x1) ? "READ" : "WRITE"), mpiCmd));
        for (cmdIndex = 0; cmdIndex < mpiCmdLen; cmdIndex++) {
            VP_HAL(None, NULL, (" 0x%02X", dataBuffer[cmdIndex]));
        }
    }
#endif
#endif

    VpMpiCmd(deviceId, ecVal, mpiCmd, mpiCmdLen, dataBuffer);
}

#if (VP_CC_DEBUG_SELECT & VP_DBG_HAL) && defined (MPI_CMD_SEARCH)
/**
 * Function primarily used to find a specific write command that is part of the
 * MPI data buffer. A simple implementation looks just through the data buffer
 * for a raw match, which could come from MPI data rather than command. A more
 * complex implementation could use known commands+cmd_len to match for commands
 * only.
 */
bool
VpMpiFindCmd(
    uint8 byteMatch,
    uint8 mpiCmdLen,
    uint8 *dataBuffer)
{
    uint8 indexCnt = 0;
    for (indexCnt = 0; indexCnt < mpiCmdLen; indexCnt++) {
        if (dataBuffer[indexCnt] == byteMatch) {
            return TRUE;
        }
    }
    return FALSE;
}
#endif

/* Used for buffering MPI data to reduce MPI traffic */
uint8
VpCSLACBuildMpiBuffer(
    uint8 index,
    uint8 *mpiBuffer,
    uint8 mpiCmd,
    uint8 mpiCmdLen,
    uint8 *mpiData)
{
    mpiBuffer[index++] = mpiCmd;
    VpMemCpy(&mpiBuffer[index], mpiData, (uint16)mpiCmdLen);

    return (index + mpiCmdLen);
}
#endif
