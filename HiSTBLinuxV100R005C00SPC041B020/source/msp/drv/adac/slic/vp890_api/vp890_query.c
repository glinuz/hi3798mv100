/** \file vp890_query.c
 * vp890_query.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Status and Query Functions.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7484 $
 * $LastChangedDate: 2010-07-13 11:55:33 -0500 (Tue, 13 Jul 2010) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES)  /* Compile only if required */

#include    "vp_api_int.h"
#include    "vp890_api_int.h"
#include    "sys_service.h"

/* ========================
    Local Type Definitions
   ======================== */

typedef enum {
    VP890_EVT_IDX_FAULT = 0,
    VP890_EVT_IDX_SIGNALING,
    VP890_EVT_IDX_RESPONSE,
    VP890_EVT_IDX_PROCESS,
    VP890_EVT_IDX_FXO,
    VP890_EVT_IDX_TEST,
    VP890_EVT_IDX_MAX,
    VP890_EVT_IDX_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} Vp890EventIndexType;


/* =================================
    Prototypes for Static Functions
   ================================= */

static uint16
GetEventId(
    uint16                  eventIdx,
    VpEventCategoryType     *pEventCat,
    VpOptionEventMaskType   *pObjEvents,
    VpOptionEventMaskType   *pObjEventMask);

static uint16
ParseEventMask(
    uint16                  *pEvent,
    uint16                  *pEventMask);

static void
FillRestOfpEvent(
    VpDevCtxType            *pDevCtx,
    VpLineCtxType           *pLineCtx,
    VpEventType             *pEvent,
    bool                    devEvents);

static VpStatusType
GetDeviceOption(
    VpDevCtxType            *pDevCtx,
    VpOptionIdType          option,
    uint16                  handle);

static VpStatusType
GetLineOption(
    VpLineCtxType           *pLineCtx,
    VpOptionIdType          option,
    uint16                  handle);

/* ============================
    Status and Query Functions
   ============================ */

/*******************************************************************************
 * Vp890GetEvent()
 *  This function reports new events that occured on the device. This function
 * returns one event for each call to it. It should be called repeatedly until
 * no more events are reported for a specific device.  This function does not
 * access the device, it returns status from the phantom registers that are
 * maintained by the API tick routine.
 *
 * Arguments:
 *  pDevCtx -
 *  pEvent  - Pointer to the results event structure
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  Returns true if there is an active event for the device.
 ******************************************************************************/
bool
Vp890GetEvent(
    VpDevCtxType            *pDevCtx,
    VpEventType             *pEvent)
{
    Vp890DeviceObjectType   *pDevObj        = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId        = pDevObj->deviceId;
    Vp890LineObjectType     *pLineObj;
    VpLineCtxType           *pLineCtx;
    VpEventCategoryType     eventCatType;

    uint8                   maxChan         = pDevObj->staticInfo.maxChannels;
    uint8                   chan;
    uint16                  eventCatIdx;

    pEvent->status = VP_STATUS_SUCCESS;
    pEvent->hasResults = FALSE;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Look for active device events first */
    for (eventCatIdx = VP890_EVT_IDX_FAULT; eventCatIdx < VP_NUM_EVCATS; eventCatIdx++) {
        eventCatType = VP_NUM_EVCATS;

        /* Determine the event Id */
        pEvent->eventId = GetEventId(eventCatIdx, &eventCatType,
                        &pDevObj->deviceEvents, &pDevObj->deviceEventsMask);

        /*
         * if the event id is 0 then no events are present for the
         * type of event currently being checked and we can continue
         * to the next type of event in the loop.
         */
        if (pEvent->eventId == 0x0000) {
            continue;
        }

        pEvent->deviceId        = deviceId;
        pEvent->channelId       = 0;
        pEvent->eventCategory   = eventCatType;
        pEvent->pDevCtx         = pDevCtx;
        pEvent->pLineCtx        = VP_NULL;
        pEvent->parmHandle      = pDevObj->eventHandle;

        FillRestOfpEvent(pDevCtx, VP_NULL, pEvent, TRUE);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return TRUE;
    }
    /*
     * No device events, now look for Line events -- but make sure the line
     * context is valid before looking for a line object
     */
    for(chan = pDevObj->dynamicInfo.lastChan; chan < maxChan; chan++) {
        pLineCtx = pDevCtx->pLineCtx[chan];

        /* skip line if null */
        if (pLineCtx == VP_NULL) {
            continue;
        }

        /* The line context is valid, create a line object and initialize
         * the event arrays for this line
         */
        pLineObj = pLineCtx->pLineObj;

        /* Check this line events */
        for (eventCatIdx = VP890_EVT_IDX_FAULT; eventCatIdx < VP_NUM_EVCATS; eventCatIdx++) {
            eventCatType = VP_NUM_EVCATS;

            /* Determine the event Id */
            pEvent->eventId = GetEventId(eventCatIdx, &eventCatType,
                            &pLineObj->lineEvents, &pLineObj->lineEventsMask);

            /* move to the next channel if none were found on the current one */
            if (pEvent->eventId == 0x0000) {
                continue;
            }

            pEvent->deviceId        = deviceId;
            pEvent->channelId       = chan;
            pEvent->eventCategory   = eventCatType;
            pEvent->pDevCtx         = pDevCtx;
            pEvent->pLineCtx        = pDevCtx->pLineCtx[chan];
            pEvent->parmHandle      = pLineObj->lineEventHandle;
            pEvent->lineId          = pLineObj->lineId;

            FillRestOfpEvent(pDevCtx, pLineCtx, pEvent, FALSE);

            /*
             * We're returning, so update the device last channel that
             * was checked so we start at the next channel
             */
            pDevObj->dynamicInfo.lastChan = chan + 1;
            if (pDevObj->dynamicInfo.lastChan >= maxChan) {
                pDevObj->dynamicInfo.lastChan = 0;
            }
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return TRUE;
        }

    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return FALSE;
} /* Vp890GetEvent() */

/*******************************************************************************
 * GetEventId()
 *  This function maps the local VP890_EVT_IDX_xyz types to the corresponding
 *  VP-API Event Categories, then calls into the ParseEventMask.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *
 ******************************************************************************/
static uint16
GetEventId(
    uint16                  eventCatIdx,
    VpEventCategoryType     *pEventCat,
    VpOptionEventMaskType   *pObjEvents,
    VpOptionEventMaskType   *pObjEventMask)
{
    switch(eventCatIdx) {
        case VP890_EVT_IDX_FAULT:
            *pEventCat = VP_EVCAT_FAULT;
            return ParseEventMask(&pObjEvents->faults, &pObjEventMask->faults);

        case VP890_EVT_IDX_SIGNALING:
            *pEventCat = VP_EVCAT_SIGNALING;
            return ParseEventMask(&pObjEvents->signaling, &pObjEventMask->signaling);

        case VP890_EVT_IDX_RESPONSE:
            *pEventCat = VP_EVCAT_RESPONSE;
            return ParseEventMask(&pObjEvents->response, &pObjEventMask->response);

        case VP890_EVT_IDX_PROCESS:
            *pEventCat = VP_EVCAT_PROCESS;
            return ParseEventMask(&pObjEvents->process, &pObjEventMask->process);

        case VP890_EVT_IDX_FXO:
            *pEventCat = VP_EVCAT_FXO;
            return ParseEventMask(&pObjEvents->fxo, &pObjEventMask->fxo);

        case VP890_EVT_IDX_TEST:
            *pEventCat = VP_EVCAT_TEST;
            return ParseEventMask(&pObjEvents->test, &pObjEventMask->test);

        default:
            break;
    }
    return 0x0000;
} /* GetEventId() */

/*******************************************************************************
 * ParseEventMask()
 *  This function performs a check on active device/line events and compares the
 * event with the event mask.  The event is cleared, and if the event is
 * unmasked it gets returned to the calling function via the return value.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *  If the returned value is other than 0x0000, the event being returned is
 * cleared in the device object.
 ******************************************************************************/
static uint16
ParseEventMask(
    uint16 *pEvent,
    uint16 *pEventMask)
{
    uint8   i;
    uint16  mask;

    for (i = 0, mask = 0x0001; i < 16; i++, (mask = mask << 1)) {
        /* Check to see if an event MAY be reported */
        if ((mask & *pEvent) == 0) {
            continue;
        }
        *pEvent    &= (~mask);

        /* If the event is not masked, return the event */
        if ((mask & *pEventMask) == 0) {
            return mask;
        }
    }
    return 0x0000;
} /* ParseEventMask() */

/*******************************************************************************
 * FillRestOfpEvent()
 *  This function fills out the remainder of the pEvent structure
 *  (eventData and hasResults) based on the an event catagory and event id.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *
 ******************************************************************************/
static void
FillRestOfpEvent(
    VpDevCtxType            *pDevCtx,
    VpLineCtxType           *pLineCtx,
    VpEventType             *pEvent,
    bool                    devEvents)

{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    Vp890LineObjectType     *pLineObj;
    uint8                   channelId = 0;

    if(pLineCtx != VP_NULL){
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;
    } else {
        pLineObj = VP_NULL;
    }

	/*BEGIN: fix MOTO*/
	if (VP_NULL == pLineObj)
	{
		return ;
	}
	/*END: fix MOTO*/

    if (VP_EVCAT_FAULT == pEvent->eventCategory) {
        if (devEvents) {
            if (VP_DEV_EVID_CLK_FLT == pEvent->eventId) {
                pEvent->eventData = (pDevObj->dynamicInfo.clkFault ? TRUE : FALSE);

            } else if (VP_DEV_EVID_BAT_FLT == pEvent->eventId) {

                if ((pDevObj->dynamicInfo.bat1Fault == TRUE) ||
                    (pDevObj->dynamicInfo.bat2Fault == TRUE) ||
                    (pDevObj->dynamicInfo.bat3Fault == TRUE))
                {
                    pEvent->eventData = TRUE;
                } else {
                    pEvent->eventData = FALSE;
                }
            }
        } else { /* Line Event */

            if (VP_LINE_EVID_THERM_FLT == pEvent->eventId) {
                if (pEvent->eventId == VP_LINE_EVID_THERM_FLT) {
                    if ((pDevObj->intReg2[channelId] & VP890_TEMPA_MASK) !=
                        (pDevObj->intReg[channelId] & VP890_TEMPA_MASK)) {

                        pEvent->eventData = (pDevObj->intReg2[channelId] & VP890_TEMPA_MASK)
                            ? TRUE : FALSE;

                        pLineObj->lineEvents.faults |= VP_LINE_EVID_THERM_FLT;
                        pDevObj->intReg2[channelId] &= ~VP890_TEMPA_MASK;
                        pDevObj->intReg2[channelId] |= (pDevObj->intReg[channelId] & VP890_TEMPA_MASK);
                    } else {
                        pEvent->eventData = (pDevObj->intReg[channelId] & VP890_TEMPA_MASK)
                            ? TRUE : FALSE;
                    }
                }
            } else if (VP_LINE_EVID_AC_FLT == pEvent->eventId) {
                pEvent->eventData =
                    (pLineObj->lineState.condition & VP_CSLAC_AC_FLT) ? TRUE : FALSE;
            } else if (VP_LINE_EVID_DC_FLT == pEvent->eventId) {
                pEvent->eventData =
                    (pLineObj->lineState.condition & VP_CSLAC_DC_FLT) ? TRUE : FALSE;
            }
        }

    } else if (VP_EVCAT_RESPONSE == pEvent->eventCategory) {
        if (devEvents) {
            if (VP_LINE_EVID_RD_OPTION == pEvent->eventId) {

                pEvent->channelId = pDevObj->getResultsOption.chanId;
                pEvent->pLineCtx = pDevCtx->pLineCtx[pEvent->channelId];

                if (pEvent->pLineCtx != VP_NULL) {
                    Vp890LineObjectType *pLineObjLocal = pEvent->pLineCtx->pLineObj;
                    pEvent->lineId = pLineObjLocal->lineId;
                }
                pEvent->hasResults = TRUE;
                pEvent->eventData = pDevObj->getResultsOption.optionType;

            } else if (VP_DEV_EVID_IO_ACCESS_CMP == pEvent->eventId) {
                pEvent->hasResults = TRUE;

            } else if (VP_DEV_EVID_DEV_INIT_CMP == pEvent->eventId) {
                pEvent->eventData = 1;
            } else if (VP_EVID_CAL_CMP == pEvent->eventId) {
                pEvent->eventData = pDevObj->responseData;
            }
        } else { /* Line Event */
            pEvent->eventData = pLineObj->responseData;

            if ((VP_LINE_EVID_LLCMD_RX_CMP == pEvent->eventId) ||
                (VP_LINE_EVID_GAIN_CMP == pEvent->eventId) ||
                (VP_LINE_EVID_RD_LOOP == pEvent->eventId)) {
                pEvent->hasResults = TRUE;
            }

            if (pLineObj->responseData == (uint8)VP_CAL_GET_SYSTEM_COEFF) {
                pEvent->eventData = pDevObj->mpiLen;
                pEvent->hasResults = TRUE;
                /*
                 * Prevent future cal complete events from being
                 * indicated as having results data.
                 */
                pLineObj->responseData = (uint8)VP_CAL_ENUM_SIZE;
            }
        }
    } else if (!devEvents && (VP_EVCAT_SIGNALING == pEvent->eventCategory)) {
        if (VP_LINE_EVID_DTMF_DIG == pEvent->eventId ) {
            /*
             * Upper bits are used for the timestamp.
             * Lower bits are used for the digit and the make/break bit.
             */
            pEvent->eventData = (pDevObj->timeStamp << 5) | pLineObj->dtmfDigitSense;
        } else {
            pEvent->eventData = pLineObj->signalingData;
        }
#ifdef VP890_INCLUDE_TESTLINE_CODE
    } else if (!devEvents && (VP_EVCAT_TEST == pEvent->eventCategory)) {

        /* testId is an enum type representing a primitive name */
        if ( VP_LINE_EVID_TEST_CMP == pEvent->eventId) {
            pEvent->eventData = pDevObj->testResults.testId;
            pEvent->hasResults = TRUE;
        }
#endif
    } else if (!devEvents && (VP_EVCAT_PROCESS == pEvent->eventCategory)) {

        pEvent->eventData = pLineObj->processData;

    } else if (!devEvents && (VP_EVCAT_FXO == pEvent->eventCategory)) {

        pEvent->eventData = pLineObj->fxoData;
    }
    return;
} /* FillRestOfpEvent() */

/*******************************************************************************
 * Vp890GetDeviceStatus()
 *  This function returns the status of all lines on a device for the type being
 * requested.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  The location pointed to by the uint32 pointer passed is set (on a per line
 * basis) to either '1' if the status if TRUE on the given line, or '0' if the
 * status is FALSE on the given line for the status being requested.
 ******************************************************************************/
VpStatusType
Vp890GetDeviceStatus(
    VpDevCtxType            *pDevCtx,
    VpInputType             input,
    uint32                  *pDeviceStatus)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    uint8                   maxChan     = pDevObj->staticInfo.maxChannels;
    uint8                   channelId;
    bool                    status      = FALSE;
    VpLineCtxType           *pLineCtx;

    *pDeviceStatus = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if(pLineCtx != VP_NULL) {
            VpCSLACGetLineStatus(pLineCtx, input, &status);
        } else {
			status = FALSE;
		}
        *pDeviceStatus |= (((status == TRUE) ? 1 : 0) << channelId);
    }
    return VP_STATUS_SUCCESS;
} /* Vp890GetDeviceStatus() */

/*******************************************************************************
 * Vp890GetOption()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
VpStatusType
Vp890GetOption(
    VpLineCtxType       *pLineCtx,
    VpDevCtxType        *pDevCtx,
    VpOptionIdType      option,
    uint16              handle)
{
    VpStatusType status = VP_STATUS_INVALID_ARG;

    if(pLineCtx != VP_NULL){
        Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
        /* Do not allow FXS specific options on an FXO line */
        if (pLineObj->status & VP890_IS_FXO) {
            switch(option) {
                case VP_OPTION_ID_ZERO_CROSS:
                case VP_OPTION_ID_PULSE_MODE:
                case VP_OPTION_ID_LINE_STATE:
                case VP_OPTION_ID_RING_CNTRL:
                    return VP_STATUS_INVALID_ARG;
                default:
                    status = GetLineOption(pLineCtx, option, handle);
            }
        } else {
            status = GetLineOption(pLineCtx, option, handle);
        }
        if (status != VP_STATUS_SUCCESS) {
            VpDevCtxType *pDevCtxLocal = pLineCtx->pDevCtx;
            status = GetDeviceOption(pDevCtxLocal, option, handle);
        }
        return status;
    } else {
        switch(option) {
            case VP_OPTION_ID_PULSE_MODE:
            case VP_OPTION_ID_TIMESLOT:
            case VP_OPTION_ID_CODEC:
            case VP_OPTION_ID_PCM_HWY:
            case VP_OPTION_ID_LOOPBACK:
            case VP_OPTION_ID_LINE_STATE:
            case VP_OPTION_ID_EVENT_MASK:
            case VP_OPTION_ID_ZERO_CROSS:
            case VP_OPTION_ID_RING_CNTRL:
            case VP_OPTION_ID_PCM_TXRX_CNTRL:
            case VP_OPTION_ID_SWITCHER_CTRL:
#ifdef CSLAC_GAIN_ABS
            case VP_OPTION_ID_ABS_GAIN:
#endif
                return VP_STATUS_INVALID_ARG;

            default:
                return GetDeviceOption(pDevCtx, option, handle);
        }
    }
} /* Vp890GetOption() */

/*******************************************************************************
 * GetDeviceOption()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
GetDeviceOption(
    VpDevCtxType        *pDevCtx,
    VpOptionIdType      option,
    uint16              handle)
{
    Vp890DeviceObjectType *pDevObj  = pDevCtx->pDevObj;
    VpLineCtxType *pLineCtx;
    Vp890LineObjectType *pLineObj;
    VpDeviceIdType deviceId         = pDevObj->deviceId;

    VpStatusType status             = VP_STATUS_SUCCESS;
    VpGetResultsOptionsDataType
            *pOptionData            = &(pDevObj->getResultsOption.optionData);
    VpOptionDeviceIoType *ioOption  = &(pOptionData->deviceIo);
    uint8 ecVal = 0;
    uint8 chanId;
    uint8 ioDirection;

    if (pDevObj->deviceEvents.response & VP890_READ_RESPONSE_MASK) {
        VP_WARNING(VpDevCtxType, pDevCtx, ("GetDeviceOption() - Waiting to clear previous read"));
        return VP_STATUS_DEVICE_BUSY;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    switch (option) {
        case VP_DEVICE_OPTION_ID_PULSE:
            pOptionData->pulseTypeOption = pDevObj->pulseSpecs;
            break;

        case VP_DEVICE_OPTION_ID_PULSE2:
            pOptionData->pulseTypeOption = pDevObj->pulseSpecs2;
            break;

        case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
            pOptionData->criticalFaultOption = pDevObj->criticalFault;
            break;

        case VP_DEVICE_OPTION_ID_DEVICE_IO:
            for(chanId=0; chanId < pDevObj->staticInfo.maxChannels; chanId++ ) {
                pLineCtx = pDevCtx->pLineCtx[chanId];
                if (pLineCtx == VP_NULL) {
                    continue;
                }
                pLineObj = pLineCtx->pLineObj;
                ecVal |= pLineObj->ecVal;
            }
            VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_RD, VP890_IODIR_REG_LEN,
                &ioDirection);
            /* Bits 2:4 of the register correspond to bits 1:3 of the option */
            ioOption->directionPins_31_0 =
                (ioDirection & ~VP890_IODIR_IO1_MASK) >> 1;
            if ((ioDirection & VP890_IODIR_IO1_MASK) == VP890_IODIR_IO1_INPUT) {
                ioOption->directionPins_31_0 |= VP_IO_INPUT_PIN;
            } else {
                ioOption->directionPins_31_0 |= VP_IO_OUTPUT_PIN;

                if ((ioDirection & VP890_IODIR_IO1_MASK) ==
                    VP890_IODIR_IO1_OUTPUT) {
                    ioOption->outputTypePins_31_0 = VP_OUTPUT_DRIVEN_PIN;
                } else if ((ioDirection & VP890_IODIR_IO1_MASK) ==
                            VP890_IODIR_IO1_OPEN_DRAIN) {
					status = VP_STATUS_INVALID_ARG;
                }
            }
            break;

        default:
            status = VP_STATUS_OPTION_NOT_SUPPORTED;
            VP_ERROR(VpDevCtxType, pDevCtx, ("GetDeviceOption() - Device option not supported"));
            break;
    }

    if (status == VP_STATUS_SUCCESS) {
        pDevObj->getResultsOption.optionType = option;
        pDevObj->deviceEvents.response |= VP_LINE_EVID_RD_OPTION;
        pDevObj->eventHandle = handle;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* GetDeviceOption() */

/*******************************************************************************
 * GetLineOption()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
GetLineOption(
    VpLineCtxType           *pLineCtx,
    VpOptionIdType          option,
    uint16                  handle)
{
    VpDevCtxType            *pDevCtxLocal   = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj        = pDevCtxLocal->pDevObj;

    VpGetResultsOptionsDataType
                            *pOptionData    = &(pDevObj->getResultsOption.optionData);
    VpStatusType             status         = VP_STATUS_SUCCESS;

    VpDeviceIdType          deviceId        = pDevObj->deviceId;
    uint8                   ecVal           = pLineObj->ecVal;
    uint8                   tempLoopBack[VP890_LOOPBACK_LEN];
    uint8                   txSlot, rxSlot;
    uint8                   tempSysConfig;

    if (pDevObj->deviceEvents.response & VP890_READ_RESPONSE_MASK) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("GetLineOption() - Waiting to clear previous read"));
        return VP_STATUS_DEVICE_BUSY;
    }

    /*
     * If this function can be executed, we will either access the MPI
     * and/or shared data. So it is best to label the entire function as
     * Code Critical so the data being accessed cannot be changed while
     * trying to be accessed
     */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pDevObj->getResultsOption.chanId = pLineObj->channelId;

    switch (option) {
        /* Line Options */
#ifdef CSLAC_GAIN_ABS
        case VP_OPTION_ID_ABS_GAIN:
            pOptionData->absGain.gain_AToD = pLineObj->absGxGain;
            pOptionData->absGain.gain_DToA = pLineObj->absGrGain;
            break;
#endif

        case VP_OPTION_ID_PULSE_MODE:
            pOptionData->pulseModeOption = pLineObj->pulseMode;
            break;

        case VP_OPTION_ID_TIMESLOT:
            VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_TS_RD,
                VP890_TX_TS_LEN, &txSlot);

            VpMpiCmdWrapper(deviceId, ecVal, VP890_RX_TS_RD,
                VP890_RX_TS_LEN, &rxSlot);

            pOptionData->timeSlotOption.tx = (txSlot & VP890_TX_TS_MASK);

            pOptionData->timeSlotOption.rx = (rxSlot & VP890_RX_TS_MASK);
            break;

        case VP_OPTION_ID_CODEC:
            pOptionData->codecOption = pLineObj->codec;
            break;

        case VP_OPTION_ID_PCM_HWY:
            pOptionData->pcmHwyOption = VP_OPTION_HWY_A;
            break;

        case VP_OPTION_ID_LOOPBACK:
            /* Timeslot loopback via loopback register */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOPBACK_RD,
                VP890_LOOPBACK_LEN, tempLoopBack);

            if ((tempLoopBack[0] & VP890_INTERFACE_LOOPBACK_EN) ==
                 VP890_INTERFACE_LOOPBACK_EN) {
                pOptionData->loopBackOption = VP_OPTION_LB_TIMESLOT;
            } else {
                if((pDevObj->devMode[0] & (VP890_DEV_MODE_CH21PT | VP890_DEV_MODE_CH12PT)) ==
                    (VP890_DEV_MODE_CH21PT | VP890_DEV_MODE_CH12PT)) {
                    pOptionData->loopBackOption = VP_OPTION_LB_CHANNELS;
                } else {
                    pOptionData->loopBackOption = VP_OPTION_LB_OFF;
                }
            }
            break;

        case VP_OPTION_ID_LINE_STATE:
            /* Battery control is automatic, so force it */
            pOptionData->lineStateOption.bat = VP_OPTION_BAT_AUTO;

            /* Smooth/Abrupt PolRev is controlled in the device */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD,
                VP890_SS_CONFIG_LEN, &tempSysConfig);

            if (tempSysConfig & VP890_SMOOTH_PR_EN) {
                pOptionData->lineStateOption.battRev = FALSE;
            } else {
                pOptionData->lineStateOption.battRev = TRUE;
            }
            break;

        case VP_OPTION_ID_EVENT_MASK:
            /*
             * In SetOption(), we force all line-specific bits in the
             * deviceEventsMask to zero.  Likewise, we force all device-
             * specific bits in the lineEventsMask to zero.  This allows
             * us to simply OR the two together here.
             */
            pOptionData->eventMaskOption.faults =
                pLineObj->lineEventsMask.faults |
                pDevObj->deviceEventsMask.faults;
            pOptionData->eventMaskOption.signaling =
                pLineObj->lineEventsMask.signaling |
                pDevObj->deviceEventsMask.signaling;
            pOptionData->eventMaskOption.response =
                pLineObj->lineEventsMask.response |
                pDevObj->deviceEventsMask.response;
            pOptionData->eventMaskOption.test =
                pLineObj->lineEventsMask.test |
                pDevObj->deviceEventsMask.test;
            pOptionData->eventMaskOption.process =
                pLineObj->lineEventsMask.process |
                pDevObj->deviceEventsMask.process;
            pOptionData->eventMaskOption.fxo =
                pLineObj->lineEventsMask.fxo |
                pDevObj->deviceEventsMask.fxo;
            break;

        case VP_OPTION_ID_ZERO_CROSS:
            pOptionData->zeroCross = pLineObj->ringCtrl.zeroCross;
            break;

        case VP_OPTION_ID_RING_CNTRL:
            pOptionData->ringControlOption = pLineObj->ringCtrl;
            break;

        case VP_OPTION_ID_PCM_TXRX_CNTRL:
            pOptionData->pcmTxRxCtrl = pLineObj->pcmTxRxCtrl;
            break;

        case VP_OPTION_ID_SWITCHER_CTRL: {
                uint8 ssConfig[VP890_SS_CONFIG_LEN];

                VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD, VP890_SS_CONFIG_LEN,
                    ssConfig);

                if (ssConfig[0] & VP890_AUTO_BAT_SHUTDOWN_EN) {
                    pOptionData->autoShutdownEn = TRUE;
                } else {
                    pOptionData->autoShutdownEn = FALSE;
                }
            }
            break;

        default:
            status = VP_STATUS_OPTION_NOT_SUPPORTED;
            VP_ERROR(VpLineCtxType, pLineCtx, ("GetLineOption() - Line option not supported"));
            break;
    }

    if (status == VP_STATUS_SUCCESS) {
        pDevObj->getResultsOption.optionType = option;
        pDevObj->deviceEvents.response |= VP_LINE_EVID_RD_OPTION;
        pDevObj->eventHandle = handle;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* GetLineOption() */

/*******************************************************************************
 * Vp890FlushEvents()
 *  This function clears out all events on the device and all events on all
 * lines associated with the device passed.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  All active device events are cleared, and all active line events associated
 * with this device are cleared.
 ******************************************************************************/
VpStatusType
Vp890FlushEvents(
    VpDevCtxType            *pDevCtx)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    VpLineCtxType           *pLineCtx;
    Vp890LineObjectType     *pLineObj;
    uint8                   channelId;
    uint8                   maxChan     = pDevObj->staticInfo.maxChannels;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pDevObj->deviceEvents.faults    = 0;
    pDevObj->deviceEvents.signaling = 0;
    pDevObj->deviceEvents.response  = 0;
    pDevObj->deviceEvents.test      = 0;
    pDevObj->deviceEvents.process   = 0;
    pDevObj->deviceEvents.fxo       = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            pLineObj->lineEvents.faults     = 0;
            pLineObj->lineEvents.signaling  = 0;
            pLineObj->lineEvents.response   = 0;
            pLineObj->lineEvents.test       = 0;
            pLineObj->lineEvents.process    = 0;
            pLineObj->lineEvents.fxo        = 0;
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890FlushEvents() */

/*******************************************************************************
 * Vp890GetResults()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
VpStatusType
Vp890GetResults(
    VpEventType             *pEvent,
    void                    *pResults)
{
    VpDevCtxType            *pDevCtx    = pEvent->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpStatusType            status      = VP_STATUS_SUCCESS;

    VpGetResultsOptionsDataType *pOptionData =
        &(pDevObj->getResultsOption.optionData);

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (VP_EVCAT_RESPONSE == pEvent->eventCategory) {
        if (VP_LINE_EVID_LLCMD_RX_CMP == pEvent->eventId) {

            uint8 cmdByte;
            for (cmdByte = 0; cmdByte < pDevObj->mpiLen; cmdByte++) {
                ((uint8 *)pResults)[cmdByte] = pDevObj->mpiData[cmdByte];
            }

        } else if (VP_LINE_EVID_GAIN_CMP == pEvent->eventId) {

            *(VpRelGainResultsType *)pResults =  pDevObj->relGainResults;

        } else if (VP_DEV_EVID_IO_ACCESS_CMP == pEvent->eventId) {

            *((VpDeviceIoAccessDataType *)pResults) = pOptionData->deviceIoData;

        } else if (VP_LINE_EVID_RD_OPTION == pEvent->eventId) {

            switch(pDevObj->getResultsOption.optionType) {
                case VP_DEVICE_OPTION_ID_PULSE:
                    *(VpOptionPulseType *)pResults =
                        pDevObj->pulseSpecs;
                    break;

                case VP_DEVICE_OPTION_ID_PULSE2:
                    *(VpOptionPulseType *)pResults =
                        pDevObj->pulseSpecs2;
                    break;

                case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                    *(VpOptionCriticalFltType *)pResults =
                        pOptionData->criticalFaultOption;
                    break;

                case VP_DEVICE_OPTION_ID_DEVICE_IO:
                    *(VpOptionDeviceIoType *)pResults =
                        pOptionData->deviceIo;
                    break;

                case VP_OPTION_ID_RING_CNTRL:
                    *(VpOptionRingControlType *)pResults =
                        pOptionData->ringControlOption;
                    break;

                case VP_OPTION_ID_ZERO_CROSS:
                    *(VpOptionZeroCrossType *)pResults =
                        pOptionData->zeroCross;
                    break;

                case VP_OPTION_ID_PULSE_MODE:
                    *((VpOptionPulseModeType *)pResults) =
                        pOptionData->pulseModeOption;
                    break;

                case VP_OPTION_ID_TIMESLOT:
                    *(VpOptionTimeslotType *)pResults =
                        pOptionData->timeSlotOption;
                    break;

                case VP_OPTION_ID_CODEC:
                    *((VpOptionCodecType *)pResults) =
                        pOptionData->codecOption;
                    break;

                case VP_OPTION_ID_PCM_HWY:
                    *((VpOptionCodecType *)pResults) =
                        pOptionData->pcmHwyOption;
                    break;

                case VP_OPTION_ID_LOOPBACK:
                    *((VpOptionLoopbackType *)pResults) =
                        pOptionData->loopBackOption;
                    break;

                case VP_OPTION_ID_LINE_STATE:
                    *((VpOptionLineStateType *)pResults) =
                        pOptionData->lineStateOption;
                    break;

                case VP_OPTION_ID_EVENT_MASK:
                    *((VpOptionEventMaskType *)pResults) =
                        pOptionData->eventMaskOption;
                    break;

                case VP_OPTION_ID_PCM_TXRX_CNTRL:
                    *((VpOptionPcmTxRxCntrlType *)pResults) =
                        pOptionData->pcmTxRxCtrl;
                    break;

                case VP_OPTION_ID_SWITCHER_CTRL:
                    *((bool *)pResults) =
                        pOptionData->autoShutdownEn;
                    break;

#ifdef CSLAC_GAIN_ABS
                case VP_OPTION_ID_ABS_GAIN:
                    *(VpOptionAbsGainType *)pResults =
                        pOptionData->absGain;
                    break;
#endif

                default:
                    status = VP_STATUS_INVALID_ARG;
                    VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890GetResults() - Invalid option type"));
                    break;
            }
        } else if (VP_EVID_CAL_CMP == pEvent->eventId) {
            if (pResults == VP_NULL) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                uint8 *pMpiData;
                uint8 commandByte;

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Vp890GetResults - VP_EVID_CAL_CMP"));

                pMpiData = (uint8 *)pResults;
                pMpiData[VP_PROFILE_TYPE_MSB] = VP_DEV_890_SERIES;
                pMpiData[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_CAL;
                pMpiData[VP_PROFILE_INDEX] = 0;
                pMpiData[VP_PROFILE_LENGTH] = VP890_CAL_STRUCT_SIZE + 2;
                pMpiData[VP_PROFILE_VERSION] = 0;
                pMpiData[VP_PROFILE_MPI_LEN] = 0;
                commandByte = VP_PROFILE_DATA_START;

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ABV Error Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.abvError[0], sizeof(pDevObj->vp890SysCalData.abvError[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.abvError[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.abvError[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Offset Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vocOffset[0][0], sizeof(pDevObj->vp890SysCalData.vocOffset[0][0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocOffset[0][0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocOffset[0][0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Error Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vocError[0][0], sizeof(pDevObj->vp890SysCalData.vocError[0][0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocError[0][0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocError[0][0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Offset Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vocOffset[0][1], sizeof(pDevObj->vp890SysCalData.vocOffset[0][1])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocOffset[0][1] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocOffset[0][1]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VOC Error Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vocError[0][1], sizeof(pDevObj->vp890SysCalData.vocError[0][1])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocError[0][1] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vocError[0][1]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SigGenA Error Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.sigGenAError[0][0], sizeof(pDevObj->vp890SysCalData.sigGenAError[0][0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.sigGenAError[0][0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.sigGenAError[0][0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SigGenA Error Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.sigGenAError[0][1], sizeof(pDevObj->vp890SysCalData.sigGenAError[0][1])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.sigGenAError[0][1] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.sigGenAError[0][1]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA-20mA Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ila20[0], sizeof(pDevObj->vp890SysCalData.ila20[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila20[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila20[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA-25mA Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ila25[0], sizeof(pDevObj->vp890SysCalData.ila25[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila25[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila25[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA-32mA Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ila32[0], sizeof(pDevObj->vp890SysCalData.ila32[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila32[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila32[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA-40mA Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ila40[0], sizeof(pDevObj->vp890SysCalData.ila40[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila40[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ila40[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILA Offset Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ilaOffsetNorm[0], sizeof(pDevObj->vp890SysCalData.ilaOffsetNorm[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ilaOffsetNorm[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ilaOffsetNorm[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("ILG Offset Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.ilgOffsetNorm[0], sizeof(pDevObj->vp890SysCalData.ilgOffsetNorm[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ilgOffsetNorm[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ilgOffsetNorm[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAS Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vas[0][0], sizeof(pDevObj->vp890SysCalData.vas[0][0])));
                pMpiData[commandByte++] = pDevObj->vp890SysCalData.vas[0][0];

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAS Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vas[0][1], sizeof(pDevObj->vp890SysCalData.vas[0][1])));
                pMpiData[commandByte++] = pDevObj->vp890SysCalData.vas[0][1];

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAG Offset Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vagOffsetNorm[0], sizeof(pDevObj->vp890SysCalData.vagOffsetNorm[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vagOffsetNorm[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vagOffsetNorm[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VAG Offset Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vagOffsetRev[0], sizeof(pDevObj->vp890SysCalData.vagOffsetRev[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vagOffsetRev[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vagOffsetRev[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VBG Offset Norm Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vbgOffsetNorm[0], sizeof(pDevObj->vp890SysCalData.vbgOffsetNorm[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vbgOffsetNorm[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vbgOffsetNorm[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("VBG Offset Rev Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.vbgOffsetRev[0], sizeof(pDevObj->vp890SysCalData.vbgOffsetRev[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vbgOffsetRev[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.vbgOffsetRev[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("SWY Offset Ch 0 %d Size %d",
                    pDevObj->vp890SysCalData.swyOffset[0], sizeof(pDevObj->vp890SysCalData.swyOffset[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.swyOffset[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.swyOffset[0]) & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Tip Cap Ch 0 %li Size %d",
                    pDevObj->vp890SysCalData.tipCapCal[0], sizeof(pDevObj->vp890SysCalData.tipCapCal[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.tipCapCal[0] >> 24) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.tipCapCal[0] >> 16) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.tipCapCal[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)(pDevObj->vp890SysCalData.tipCapCal[0] & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Ring Cap Ch 0 %li Size %d",
                    pDevObj->vp890SysCalData.ringCapCal[0], sizeof(pDevObj->vp890SysCalData.ringCapCal[0])));
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ringCapCal[0] >> 24) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ringCapCal[0] >> 16) & 0xFF);
                pMpiData[commandByte++] = (uint8)((pDevObj->vp890SysCalData.ringCapCal[0] >> 8) & 0xFF);
                pMpiData[commandByte++] = (uint8)(pDevObj->vp890SysCalData.ringCapCal[0] & 0xFF);

                VP_CALIBRATION(VpDevCtxType, pDevCtx, ("Final Command Byte Value %d", commandByte));
            }
        } else {
            VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890GetResults() - Invalid event ID"));
            status = VP_STATUS_INVALID_ARG;
        }
#ifdef VP890_INCLUDE_TESTLINE_CODE
    } else if (VP_EVCAT_TEST == pEvent->eventCategory) {

        if (VP_LINE_EVID_TEST_CMP == pEvent->eventId) {
            *((VpTestResultType *)pResults) = pDevObj->testResults;
        } else {
            VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890GetResults() - Invalid event ID"));
            status = VP_STATUS_INVALID_ARG;
        }
#endif
    } else {
        VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890GetResults() - Invalid event category"));
        status = VP_STATUS_INVALID_ARG;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp890GetResults() */


#ifdef VP890_FXS_SUPPORT
/**
 * Vp890GetRelayState()
 *  This function returns the current relay state of VP890 device.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  The indicated relay state is returned for the given line.
 */
VpStatusType
Vp890GetRelayState(
    VpLineCtxType           *pLineCtx,
    VpRelayControlType      *pRstate)
{
    VpDevCtxType            *pDevCtx;
    Vp890DeviceObjectType   *pDevObj;
    Vp890LineObjectType     *pLineObj;
    VpDeviceIdType          deviceId;

    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    pLineObj = pLineCtx->pLineObj;
    pDevObj = pDevCtx->pDevObj;
    deviceId = pDevObj->deviceId;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    *pRstate = pLineObj->relayState;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;

} /* Vp890SetRelayState() */
#endif /* VP890_FXS_SUPPORT */



#if (VP_CC_DEBUG_SELECT & VP_DBG_ERROR)
/**
 * Vp890RegisterDump()
 *  Dump all known 890 device and line specific registers (for debug purposes).
 *
 * Returns:
 */
VpStatusType
Vp890RegisterDump(
    VpDevCtxType *pDevCtx)
{
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 channelId, ecVal;

    uint8 intSwRegParam[VP890_REGULATOR_TIMING_LEN];
    uint8 swYZ[VP890_REGULATOR_PARAM_LEN];
    uint8 devType[VP890_DEVTYPE_LEN];
    uint8 testReg1[VP890_TEST_REG1_LEN];
    uint8 txRxSlot[VP890_TX_RX_CSLOT_LEN];
    uint8 devCfg[VP890_DCR_LEN];
    uint8 opMode[VP890_OP_MODE_LEN];
    uint8 sigReg[VP890_NO_UL_SIGREG_LEN];
    uint8 intMask[VP890_INT_MASK_LEN];
    uint8 intRev[VP890_REV_INFO_LEN];
    uint8 swCtrl[VP890_REGULATOR_CTRL_LEN];
    uint8 devMode[VP890_DEV_MODE_LEN];

    uint8 registerIndex;

    VP_ERROR(VpDevCtxType, pDevCtx,("Device Registers:"));
    ecVal = pDevObj->ecVal;

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DEVTYPE_RD, VP890_DEVTYPE_LEN, devType);
    VpSysDebugPrintf("\n\rVP890_DEVTYPE (0x%02X) ", VP890_DEVTYPE_RD);
    for (registerIndex = 0; registerIndex < VP890_DEVTYPE_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", devType[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DEV_MODE_RD, VP890_DEV_MODE_LEN, devMode);
    VpSysDebugPrintf("\n\rVP890_DEV_MODE (0x%02X) ", VP890_DEV_MODE_RD);
    for (registerIndex = 0; registerIndex < VP890_DEV_MODE_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", devMode[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_TEST_REG1_RD, VP890_TEST_REG1_LEN, testReg1);
    VpSysDebugPrintf("\n\rVP890_TEST_REG1 (0x%02X) ", VP890_TEST_REG1_RD);
    for (registerIndex = 0; registerIndex < VP890_TEST_REG1_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", testReg1[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_RX_CSLOT_RD, VP890_TX_RX_CSLOT_LEN, txRxSlot);
    VpSysDebugPrintf("\n\rVP890_TX_RX_CSLOT (0x%02X) ", VP890_TX_RX_CSLOT_RD);
    for (registerIndex = 0; registerIndex < VP890_TX_RX_CSLOT_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", txRxSlot[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_DCR_RD, VP890_DCR_LEN, devCfg);
    VpSysDebugPrintf("\n\rVP890_DCR (0x%02X) ", VP890_DCR_RD);
    for (registerIndex = 0; registerIndex < VP890_DCR_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", devCfg[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_MODE_RD, VP890_OP_MODE_LEN, opMode);
    VpSysDebugPrintf("\n\rVP890_OP_MODE (0x%02X) ", VP890_OP_MODE_RD);
    for (registerIndex = 0; registerIndex < VP890_OP_MODE_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", opMode[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_NO_UL_SIGREG_RD, VP890_NO_UL_SIGREG_LEN, sigReg);
    VpSysDebugPrintf("\n\rVP890_NO_UL_SIGREG (0x%02X) ", VP890_NO_UL_SIGREG_RD);
    for (registerIndex = 0; registerIndex < VP890_NO_UL_SIGREG_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", sigReg[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_INT_MASK_RD, VP890_INT_MASK_LEN, intMask);
    VpSysDebugPrintf("\n\rVP890_INT_MASK (0x%02X) ", VP890_INT_MASK_RD);
    for (registerIndex = 0; registerIndex < VP890_INT_MASK_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", intMask[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_REV_INFO_RD, VP890_REV_INFO_LEN, intRev);
    VpSysDebugPrintf("\n\rVP890_REV_INFO (0x%02X) ", VP890_REV_INFO_RD);
    for (registerIndex = 0; registerIndex < VP890_REV_INFO_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", intRev[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_RD, VP890_REGULATOR_PARAM_LEN, swYZ);
    VpSysDebugPrintf("\n\rVP890_REGULATOR_PARAM (0x%02X) ", VP890_REGULATOR_PARAM_RD);
    for (registerIndex = 0; registerIndex < VP890_REGULATOR_PARAM_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", swYZ[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_RD, VP890_REGULATOR_CTRL_LEN, swCtrl);
    VpSysDebugPrintf("\n\rVP890_REGULATOR_CTRL (0x%02X) ", VP890_REGULATOR_CTRL_RD);
    for (registerIndex = 0; registerIndex < VP890_REGULATOR_CTRL_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", swCtrl[registerIndex]);
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_TIMING_RD, VP890_REGULATOR_TIMING_LEN, intSwRegParam);
    VpSysDebugPrintf("\n\rVP890_REGULATOR_TIMING (0x%02X) ", VP890_REGULATOR_TIMING_RD);
    for (registerIndex = 0; registerIndex < VP890_REGULATOR_TIMING_LEN; registerIndex++) {
        VpSysDebugPrintf("0x%02X ", intSwRegParam[registerIndex]);
    }

    for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
        uint8 sysState[VP890_SYS_STATE_LEN];
        uint8 sysStateCfg[VP890_SS_CONFIG_LEN];
        uint8 convCfg[VP890_CONV_CFG_LEN];

        uint8 icr1[VP890_ICR1_LEN];
        uint8 icr2[VP890_ICR2_LEN];
        uint8 icr3[VP890_ICR3_LEN];
        uint8 icr4[VP890_ICR4_LEN];
        uint8 icr5[VP890_ICR5_LEN];
        uint8 dcCal[VP890_DC_CAL_REG_LEN];

        uint8 disn[VP890_DISN_LEN];
        uint8 vpGain[VP890_VP_GAIN_LEN];
        uint8 grGain[VP890_GR_GAIN_LEN];
        uint8 gxGain[VP890_GX_GAIN_LEN];

        uint8 x[VP890_X_FILTER_LEN];
        uint8 r[VP890_R_FILTER_LEN];

        uint8 b1[VP890_B1_FILTER_LEN];
        uint8 b2[VP890_B2_FILTER_LEN];

        uint8 z1[VP890_Z1_FILTER_LEN];
        uint8 z2[VP890_Z2_FILTER_LEN];

        uint8 opFunc[VP890_OP_FUNC_LEN];
        uint8 opCond[VP890_OP_COND_LEN];

        uint8 ioData[VP890_IODATA_REG_LEN];
        uint8 ioDir[VP890_IODIR_REG_LEN];
        uint8 batCal[VP890_BAT_CALIBRATION_LEN];

        uint8 txSlot[VP890_TX_TS_LEN];
        uint8 rxSlot[VP890_RX_TS_LEN];

        uint8 dcFeed[VP890_DC_FEED_LEN];
        uint8 loopSup[VP890_LOOP_SUP_LEN];

        uint8 sigABParams[VP890_SIGA_PARAMS_LEN];
        uint8 sigCDParams[VP890_SIGCD_PARAMS_LEN];

        uint8 cadenceParams[VP890_CADENCE_TIMER_LEN];
        uint8 cidParams[VP890_CID_PARAM_LEN];

        ecVal = ((channelId == 0) ? VP890_EC_CH1 : VP890_EC_CH2);
        ecVal |= pDevObj->ecVal;

        VpSysDebugPrintf("\n\rCHANNEL %d", channelId);

        VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN, vpGain);
        VpSysDebugPrintf("\n\rVP890_VP_GAIN (0x%02X) ", VP890_VP_GAIN_RD);
        for (registerIndex = 0; registerIndex < VP890_VP_GAIN_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", vpGain[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_IODATA_REG_RD, VP890_IODATA_REG_LEN, ioData);
        VpSysDebugPrintf("\n\rVP890_IODATA_REG (0x%02X) ", VP890_IODATA_REG_RD);
        for (registerIndex = 0; registerIndex < VP890_IODATA_REG_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", ioData[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_RD, VP890_IODIR_REG_LEN, ioDir);
        VpSysDebugPrintf("\n\rVP890_IODIR_REG (0x%02X) ", VP890_IODIR_REG_RD);
        for (registerIndex = 0; registerIndex < VP890_IODIR_REG_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", ioDir[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_BAT_CALIBRATION_RD, VP890_BAT_CALIBRATION_LEN, batCal);
        VpSysDebugPrintf("\n\rVP890_BAT_CALIBRATION (0x%02X) ", VP890_BAT_CALIBRATION_RD);
        for (registerIndex = 0; registerIndex < VP890_BAT_CALIBRATION_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", batCal[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SYS_STATE_RD, VP890_SYS_STATE_LEN, sysState);
        VpSysDebugPrintf("\n\rVP890_SYS_STATE (0x%02X) ", VP890_SYS_STATE_RD);
        for (registerIndex = 0; registerIndex < VP890_SYS_STATE_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", sysState[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD, VP890_SS_CONFIG_LEN, sysStateCfg);
        VpSysDebugPrintf("\n\rVP890_SS_CONFIG (0x%02X) ", VP890_SS_CONFIG_RD);
        for (registerIndex = 0; registerIndex < VP890_SS_CONFIG_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", sysStateCfg[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN, convCfg);
        VpSysDebugPrintf("\n\rVP890_CONV_CFG (0x%02X) ", VP890_CONV_CFG_RD);
        for (registerIndex = 0; registerIndex < VP890_CONV_CFG_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", convCfg[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN, opFunc);
        VpSysDebugPrintf("\n\rVP890_CODEC_REG (0x%02X) ", VP890_OP_FUNC_RD);
        for (registerIndex = 0; registerIndex < VP890_OP_FUNC_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", opFunc[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_RD, VP890_OP_COND_LEN, opCond);
        VpSysDebugPrintf("\n\rVP890_OP_COND (0x%02X) ", VP890_OP_COND_RD);
        for (registerIndex = 0; registerIndex < VP890_OP_COND_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", opCond[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_DISN_RD, VP890_DISN_LEN, disn);
        VpSysDebugPrintf("\n\rVP890_DISN (0x%02X) ", VP890_DISN_RD);
        for (registerIndex = 0; registerIndex < VP890_DISN_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", disn[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_GX_GAIN_RD, VP890_GX_GAIN_LEN, gxGain);
        VpSysDebugPrintf("\n\rVP890_GX_GAIN (0x%02X) ", VP890_GX_GAIN_RD);
        for (registerIndex = 0; registerIndex < VP890_GX_GAIN_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", gxGain[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_GR_GAIN_RD, VP890_GR_GAIN_LEN, grGain);
        VpSysDebugPrintf("\n\rVP890_GR_GAIN (0x%02X) ", VP890_GR_GAIN_RD);
        for (registerIndex = 0; registerIndex < VP890_GR_GAIN_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", grGain[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_B1_FILTER_RD, VP890_B1_FILTER_LEN, b1);
        VpSysDebugPrintf("\n\rVP890_B1_FILTER (0x%02X) ", VP890_B1_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_B1_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", b1[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_B2_FILTER_RD, VP890_B2_FILTER_LEN, b2);
        VpSysDebugPrintf("\n\rVP890_B2_FILTER (0x%02X) ", VP890_B2_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_B2_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", b2[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_X_FILTER_RD, VP890_X_FILTER_LEN, x);
        VpSysDebugPrintf("\n\rVP890_X_FILTER (0x%02X) ", VP890_X_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_X_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", x[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_R_FILTER_RD, VP890_R_FILTER_LEN, r);
        VpSysDebugPrintf("\n\rVP890_R_FILTER (0x%02X) ", VP890_R_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_R_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", r[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_Z1_FILTER_RD, VP890_Z1_FILTER_LEN, z1);
        VpSysDebugPrintf("\n\rVP890_Z1_FILTER (0x%02X) ", VP890_Z1_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_Z1_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", z1[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_Z2_FILTER_RD, VP890_Z2_FILTER_LEN, z2);
        VpSysDebugPrintf("\n\rVP890_Z2_FILTER (0x%02X) ", VP890_Z2_FILTER_RD);
        for (registerIndex = 0; registerIndex < VP890_Z2_FILTER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", z2[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_TX_TS_RD, VP890_TX_TS_LEN, txSlot);
        VpSysDebugPrintf("\n\rVP890_TX_TS (0x%02X) ", VP890_TX_TS_RD);
        for (registerIndex = 0; registerIndex < VP890_TX_TS_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", txSlot[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_RX_TS_RD, VP890_RX_TS_LEN, rxSlot);
        VpSysDebugPrintf("\n\rVP890_RX_TS (0x%02X) ", VP890_RX_TS_RD);
        for (registerIndex = 0; registerIndex < VP890_RX_TS_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", rxSlot[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_RD, VP890_DC_FEED_LEN, dcFeed);
        VpSysDebugPrintf("\n\rVP890_DC_FEED (0x%02X) ", VP890_DC_FEED_RD);
        for (registerIndex = 0; registerIndex < VP890_DC_FEED_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", dcFeed[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_RD, VP890_LOOP_SUP_LEN, loopSup);
        VpSysDebugPrintf("\n\rVP890_LOOP_SUP (0x%02X) ", VP890_LOOP_SUP_RD);
        for (registerIndex = 0; registerIndex < VP890_LOOP_SUP_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", loopSup[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SIGA_PARAMS_RD, VP890_SIGA_PARAMS_LEN, sigABParams);
        VpSysDebugPrintf("\n\rVP890_SIGA_PARAMS (0x%02X) ", VP890_SIGA_PARAMS_RD);
        for (registerIndex = 0; registerIndex < VP890_SIGA_PARAMS_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", sigABParams[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_SIGCD_PARAMS_RD, VP890_SIGCD_PARAMS_LEN, sigCDParams);
        VpSysDebugPrintf("\n\rVP890_SIGCD_PARAMS (0x%02X) ", VP890_SIGCD_PARAMS_RD);
        for (registerIndex = 0; registerIndex < VP890_SIGCD_PARAMS_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", sigCDParams[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CADENCE_TIMER_RD, VP890_CADENCE_TIMER_LEN, cadenceParams);
        VpSysDebugPrintf("\n\rVP890_CADENCE_TIMER (0x%02X) ", VP890_CADENCE_TIMER_RD);
        for (registerIndex = 0; registerIndex < VP890_CADENCE_TIMER_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", cadenceParams[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_CID_PARAM_RD, VP890_CID_PARAM_LEN, cidParams);
        VpSysDebugPrintf("\n\rVP890_CID_PARAM (0x%02X) ", VP890_CID_PARAM_RD);
        for (registerIndex = 0; registerIndex < VP890_CID_PARAM_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", cidParams[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR1_RD, VP890_ICR1_LEN, icr1);
        VpSysDebugPrintf("\n\rVP890_ICR1 (0x%02X) ", VP890_ICR1_RD);
        for (registerIndex = 0; registerIndex < VP890_ICR1_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", icr1[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_RD, VP890_ICR2_LEN, icr2);
        VpSysDebugPrintf("\n\rVP890_ICR2 (0x%02X) ", VP890_ICR2_RD);
        for (registerIndex = 0; registerIndex < VP890_ICR2_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", icr2[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR3_RD, VP890_ICR3_LEN, icr3);
        VpSysDebugPrintf("\n\rVP890_ICR3 (0x%02X) ", VP890_ICR3_RD);
        for (registerIndex = 0; registerIndex < VP890_ICR3_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", icr3[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR4_RD, VP890_ICR4_LEN, icr4);
        VpSysDebugPrintf("\n\rVP890_ICR4 (0x%02X) ", VP890_ICR4_RD);
        for (registerIndex = 0; registerIndex < VP890_ICR4_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", icr4[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR5_RD, VP890_ICR5_LEN, icr5);
        VpSysDebugPrintf("\n\rVP890_ICR5 (0x%02X) ", VP890_ICR5_RD);
        for (registerIndex = 0; registerIndex < VP890_ICR5_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", icr5[registerIndex]);
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_CAL_REG_RD, VP890_DC_CAL_REG_LEN, dcCal);
        VpSysDebugPrintf("\n\rVP890_DC_CAL_REG (0x%02X) ", VP890_DC_CAL_REG_RD);
        for (registerIndex = 0; registerIndex < VP890_DC_CAL_REG_LEN; registerIndex++) {
            VpSysDebugPrintf("0x%02X ", dcCal[registerIndex]);
        }
    }

    VpSysDebugPrintf("\n\r");

    return VP_STATUS_SUCCESS;
}
#endif

#endif /* VP_CC_890_SERIES */
