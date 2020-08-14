/** \file vp_api_query.c
 * vp_api_query.c
 *
 *  This file contains the implementation of top level VoicePath API-II
 * Status and Query procedures.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 6827 $
 * $LastChangedDate: 2010-04-01 17:51:38 -0500 (Thu, 01 Apr 2010) $
 */

/* INCLUDES */
#include "vp_api.h"     /* Typedefs and function prototypes for API */

#include "vp_hal.h"
#include "vp_api_int.h" /* Device specific typedefs and function prototypes */
#include "sys_service.h"

#if defined (VP_CC_880_SERIES)
#include "vp880_api_int.h"
#endif

/******************************************************************************
 *                        STATUS AND QUERY FUNCTIONS                          *
 ******************************************************************************/
/**
 * VpGetEvent()
 *  This function is used to get a event from a given device. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  This function can be called upon detecting an interrupt from interrupt
 * context or this function could be called to poll events. This function
 * assumes passed device context has been initialized.
 *
 * Postconditions:
 *  Returns TRUE is there is a new event. Information about this event is
 * filled in to the event pointer (if not zero). If either of its arguments is
 * zero this function returns FALSE.
 *
 */
bool
VpGetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent)
{
    bool gotEvent;
    VpGetEventFuncPtrType GetEvent;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "GetEvent");

    /* Basic argument checking */
    if (pEvent == VP_NULL) {
        VP_ERROR(None, VP_NULL, ("pEvent = VP_NULL in VpGetEvent()"));
        return FALSE;
    } else if (pDevCtx == VP_NULL) {
        pEvent->status = VP_STATUS_INVALID_ARG;
        gotEvent = FALSE;
    } else {
        GetEvent = pDevCtx->funPtrsToApiFuncs.GetEvent;
        if (GetEvent == VP_NULL) {
            pEvent->status = VP_STATUS_FUNC_NOT_SUPPORTED;
            gotEvent = FALSE;
        } else {
            gotEvent = GetEvent(pDevCtx, pEvent);
        }
    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (pEvent->status == VP_STATUS_SUCCESS) {
        if (gotEvent) {
            if (pEvent->pLineCtx != VP_NULL) {
                VP_API_FUNC(VpLineCtxType, pEvent->pLineCtx, ("Vp%s() = %s", "GetEvent", "TRUE"));
            } else {
                VP_API_FUNC(VpDevCtxType, pEvent->pDevCtx, ("Vp%s() = %s", "GetEvent", "TRUE"));
            }
        } else {
            VP_API_FUNC(VpDevCtxType, pDevCtx, ("Vp%s() = %s", "GetEvent", "FALSE"));
        }
    } else {
        VP_API_FUNC(VpDevCtxType, pDevCtx, ("Vp%s(): status = %s", "GetEvent", VpGetString_VpStatusType(pEvent->status)));
    }
#endif

#if (VP_CC_DEBUG_SELECT & VP_DBG_EVENT)
    if ((pEvent->status == VP_STATUS_SUCCESS) && gotEvent) {
        VpDisplayEvent(pEvent);
    }
#endif

    return gotEvent;
} /* VpGetEvent() */

/**
 * VpGetLineStatus()
 *  This function is used to obtain status of a given line with respect to a
 * certain type of line condition. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 * Returns status information about the line condition that was asked for.
 */
VpStatusType
VpGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "GetLineStatus");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(GetLineStatus, (pLineCtx, input, pStatus));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "GetLineStatus", status);
    return status;
} /* VpGetLineStatus() */

/**
 * VpGetDeviceStatus()
 *  This function is used to obtain status of all lines with respect to a
 * certain type of line condition. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Returns status information about the line condition that was asked for.
 */
VpStatusType
VpGetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "GetDeviceStatus");

    /* Basic argument checking */
    if ((pDevCtx == VP_NULL) || (pDeviceStatus == VP_NULL)) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(GetDeviceStatus, (pDevCtx, input, pDeviceStatus));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "GetDeviceStatus", status);
    return status;
} /* VpGetDeviceStatus() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/**
 * VpGetDeviceStatusExt()
 *  This function is used to obtain status of all lines with respect to a
 * certain type of line condition. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Returns status information about the line condition that was asked for.
 */
VpStatusType
VpGetDeviceStatusExt(
    VpDevCtxType *pDevCtx,
    VpDeviceStatusType *pDeviceStatus)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "GetDeviceStatusExt");

    /* Basic argument checking */
    if ((pDevCtx == VP_NULL) || (pDeviceStatus == VP_NULL)) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(GetDeviceStatusExt, (pDevCtx, pDeviceStatus));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "GetDeviceStatusExt", status);
    return status;
} /* VpGetDeviceStatusExt() */
#endif

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
/**
 * VpGetLoopCond()
 *  This function is used to obtain the telephone loop condition for a given
 * line. See VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  This function starts the necessary actions to obtain the loop condition.
 */
VpStatusType
VpGetLoopCond(
    VpLineCtxType *pLineCtx,
    uint16 handle)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "GetLoopCond");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(GetLoopCond, (pLineCtx, handle));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "GetLoopCond", status);
    return status;
} /* VpGetLoopCond() */
#endif

/**
 * VpGetOption()
 *  This function is used to read an option. See VP-API-II documentation for
 * more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Starts the necessary action to be able to read the requested option.
 */
VpStatusType
VpGetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtxParam,
    VpOptionIdType option,
    uint16 handle)
{
    VpDevCtxType *pDevCtx;
    VpStatusType status = VP_STATUS_SUCCESS;
    bool lineSpecific = (pLineCtx != VP_NULL);

    /* Basic argument checking */
    if (lineSpecific) {
        VP_API_ENTER(VpLineCtxType, pLineCtx, "GetOption");
        if (pDevCtxParam != VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
        pDevCtx = pLineCtx->pDevCtx;
    } else {
        pDevCtx = pDevCtxParam;
        VP_API_ENTER(VpDevCtxType, pDevCtx, "GetOption");
        if (pDevCtx == VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
    }

    if (status == VP_STATUS_SUCCESS) {
        status = VP_CALL_DEV_FUNC(GetOption, (pLineCtx, pDevCtxParam, option, handle));
    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (lineSpecific) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "GetOption", status);
    } else {
        VP_API_EXIT(VpDevCtxType, pDevCtx, "GetOption", status);
    }
#endif

    return status;
} /* VpGetOption() */

VpStatusType
VpGetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState)
{
    VpDevCtxType *pDevCtx;
    VpStatusType status = VP_STATUS_SUCCESS;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "GetLineState");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "GetLineState", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    switch (pDevCtx->deviceType) {

#ifdef VP_CC_VCP_SERIES
        case VP_DEV_VCP_SERIES:
#endif
#ifdef VP_CC_VCP2_SERIES
        case VP_DEV_VCP2_SERIES:
#endif
#if defined(VP_CC_VCP_SERIES) || defined(VP_CC_VCP2_SERIES)
            status = VP_CALL_DEV_FUNC(GetLineState, (pLineCtx, pCurrentState));
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES: {
            Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
            *pCurrentState = pLineObj->lineState.usrCurrent;
            break;
        }
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES: {
            Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
            if (pLineObj->status & VP880_LINE_IN_CAL) {
                *pCurrentState = pLineObj->calLineData.usrState;
            } else {
                *pCurrentState = pLineObj->lineState.usrCurrent;
            }
            break;
        }
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES: {
            Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
            *pCurrentState = pLineObj->lineState.usrCurrent;
            break;
        }
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES: {
            Vp580LineObjectType *pLineObj = pLineCtx->pLineObj;
            *pCurrentState = pLineObj->lineState.usrCurrent;
            break;
        }
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES: {
            Vp792LineObjectType *pLineObj = pLineCtx->pLineObj;
            *pCurrentState = pLineObj->currentState;
            break;
        }
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP: {
            VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
            status = VP_CALL_DEV_FUNC(GetLineState, (pLineCtx, pCurrentState));
            break;
        }
#endif

        default:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;

    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "GetLineState", status);
    return status;
}

/**
 * VpFlushEvents()
 *  This function is used to flush all outstanding events from the VTD's event
 * queue. See VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Clears all outstanding events from event queue.
 */
VpStatusType
VpFlushEvents(
    VpDevCtxType *pDevCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "FlushEvents");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(FlushEvents, (pDevCtx));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "FlushEvents", status);
    return status;
} /* VpFlushEvents() */

/**
 * VpGetResults()
 *  This function retrives the result based on a previous operation. For more
 * information see VP-API-II user guide.
 *
 * Preconditions:
 *  This function can be called upon detecting an event for which there could
 * be associated data. The results pointer must provide enough storage space
 * the type of result that is anticipated.
 *
 * Postconditions:
 *  It fills in the results based on the event to location provided by results
 * pointer. This function returns an error if either of its arguments is zero.
 */
VpStatusType
VpGetResults(
    VpEventType *pEvent,
    void *pResults)
{
    VpDevCtxType *pDevCtx;
    VpStatusType status;

    if (pEvent == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (pEvent == VP_NULL) {
        VP_API_ENTER(None, VP_NULL, "GetResults");
        VP_API_EXIT(None, VP_NULL, "GetResults", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    } else if (pEvent->pLineCtx == VP_NULL) {
        VP_API_ENTER(VpDevCtxType, pEvent->pDevCtx, "GetResults");
    } else {
        VP_API_ENTER(VpLineCtxType, pEvent->pLineCtx, "GetResults");
    }
#endif

    pDevCtx = pEvent->pDevCtx;
    status = VP_CALL_DEV_FUNC(GetResults, (pEvent, pResults));

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (pEvent->pLineCtx != VP_NULL) {
        VP_API_EXIT(VpLineCtxType, pEvent->pLineCtx, "GetResults", status);
    } else {
        VP_API_EXIT(VpDevCtxType, pEvent->pDevCtx, "GetResults", status);
    }
#endif

#if (VP_CC_DEBUG_SELECT & VP_DBG_EVENT)
    if ((pResults != NULL) && (status == VP_STATUS_SUCCESS)) {
        VpDisplayResults(pEvent, pResults);
    }
#endif

    return status;
} /* VpGetResults() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/**
 * VpClearResults()
 *  This function is used to clear results associated with an event.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Clears results entry (only one at the top of the results queue) that is
 * outstanding and waiting to be read.
 */
VpStatusType
VpClearResults(
    VpDevCtxType *pDevCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "ClearResults");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(ClearResults, (pDevCtx));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "ClearResults", status);
    return status;
} /* VpClearResults() */
#endif

/**
 * VpGetRelayState()
 *  This function returns the value of the lines relay state.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The indicated relay state is set for the given line.
 */
VpStatusType
VpGetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType *pRstate)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "GetRelayState");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(GetRelayState, (pLineCtx, pRstate));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "GetRelayState", status);
    return status;
} /* VpSetRelayState() */

/*
 * VpGetDeviceInfo()
 * This function is used to obtain information about a device. Please see VP-API
 * documentation for more information.
 *
 * Preconditions:
 * This function assumes the passed line/device contexts are created and
 * initialized.
 *
 * Postconditions:
 * Returns the requested information.
 */
VpStatusType
VpGetDeviceInfo(
    VpDeviceInfoType *pDeviceInfo)
{
    VpLineCtxType *pLineCtx = VP_NULL;
    VpDevCtxType *pDevCtx;
    VpDeviceType devType;
    void *pDevObj;
    VpStatusType status = VP_STATUS_SUCCESS;

    /* Basic argument checking */
    if (pDeviceInfo == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        pLineCtx = pDeviceInfo->pLineCtx;
        pDevCtx = pDeviceInfo->pDevCtx;

        if ((pLineCtx == VP_NULL) && (pDevCtx == VP_NULL)) {
            status = VP_STATUS_INVALID_ARG;
        }
    }

    if (status != VP_STATUS_SUCCESS) {
        VP_API_EXIT(None, VP_NULL, "GetDeviceInfo", status);
        return status;
    }

    /* Extract device info from...*/
    if (pLineCtx != VP_NULL) {
        /* Line context */
        pDevCtx = pLineCtx->pDevCtx;
        pDeviceInfo->pDevCtx = pDevCtx;
    } else {
        /* Device context */
        pDevCtx = pDeviceInfo->pDevCtx;
        pDeviceInfo->pLineCtx = VP_NULL;
    }

    pDevObj = pDevCtx->pDevObj;
    devType = pDevCtx->deviceType;
    pDeviceInfo->deviceType = devType;
    pDeviceInfo->revCode = 0;
    pDeviceInfo->slacId = 0;
    pDeviceInfo->productCode = 0;
    pDeviceInfo->featureList.testLoadSwitch = VP_UNKNOWN;
    pDeviceInfo->featureList.internalTestTermination = VP_UNKNOWN;

    switch(devType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            pDeviceInfo->numLines =
                ((VpVcpDeviceObjectType *)pDevObj)->maxChannels;
            pDeviceInfo->deviceId =
                ((VpVcpDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            pDeviceInfo->numLines =
                ((VpVcp2DeviceObjectType *)pDevObj)->numChannels;
            pDeviceInfo->deviceId =
                ((VpVcp2DeviceObjectType *)pDevObj)->deviceId;
            pDeviceInfo->productCode =
                ((VpVcp2DeviceObjectType *)pDevObj)->product;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pDeviceInfo->numLines =
                ((Vp880DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pDeviceInfo->revCode =
                ((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0];
            pDeviceInfo->productCode =
                (uint16)((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[1];

            if (((Vp880DeviceObjectType *)pDevObj)->stateInt & VP880_HAS_TEST_LOAD_SWITCH) {
                pDeviceInfo->featureList.testLoadSwitch = VP_AVAILABLE;
            } else {
                pDeviceInfo->featureList.testLoadSwitch = VP_NOT_AVAILABLE;
            }

            /* The internal test termination is available when the test load
             * switch is not, as long as the product is newer than VC */
            if (pDeviceInfo->featureList.testLoadSwitch == VP_NOT_AVAILABLE &&
                pDeviceInfo->revCode > VP880_REV_VC)
            {
                pDeviceInfo->featureList.internalTestTermination = VP_AVAILABLE;
            } else {
                pDeviceInfo->featureList.internalTestTermination = VP_NOT_AVAILABLE;
            }
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            pDeviceInfo->revCode =
                ((Vp890DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0];
            pDeviceInfo->productCode =
                (uint16)((Vp890DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[1];

            pDeviceInfo->featureList.testLoadSwitch = VP_NOT_AVAILABLE;
            pDeviceInfo->featureList.internalTestTermination = VP_AVAILABLE;
            /*
             * This special test is done becuase the FXO is on physical line 1
             * (range [0:1]) but is a single channel device. So either the 890
             * specific library has to take care of all special instances where
             * channelId looping starts at 1 with maxChannels = 1 (normal looping
             * starts at 0), or we just have to change the number reported to
             * the application for maxChannels -- what is done here.
             */
#ifdef VP890_FXO_SUPPORT
            if(pDeviceInfo->productCode == (uint16)VP890_DEV_PCN_89010) {
                pDeviceInfo->numLines = 1;
            } else {
#endif
                pDeviceInfo->numLines =
                    ((Vp890DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
#ifdef VP890_FXO_SUPPORT
            }
#endif

            pDeviceInfo->deviceId =
                ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDeviceInfo->numLines =
                ((Vp790DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            pDeviceInfo->revCode =
                ((Vp790DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0];
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            pDeviceInfo->numLines =
                ((Vp792DeviceObjectType *)pDevObj)->maxChannels;
            pDeviceInfo->deviceId =
                ((Vp792DeviceObjectType *)pDevObj)->deviceId;
            pDeviceInfo->slacId =
                ((Vp792DeviceObjectType *)pDevObj)->slacId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pDeviceInfo->numLines =
                ((Vp580DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = KWrapGetDeviceInfo(pDevCtx, pDeviceInfo);
            break;
#endif
        default:
            pDeviceInfo->numLines = 0;
            status = VP_STATUS_FAILURE;
            break;
    }

    if (pDeviceInfo->numLines > VP_MAX_LINES_PER_DEVICE) {
        pDeviceInfo->numLines = VP_MAX_LINES_PER_DEVICE;
    }

    VP_API_EXIT(None, VP_NULL, "GetDeviceInfo", status);
    return status;
} /* VpGetDeviceInfo() */

/**
 * VpGetLineInfo()
 *  This function is used to obtain information about a line. Please see VP-API
 * documentation for more information.
 *
 * Preconditions:
 *  This function assumes the line context and device context are already
 * created.
 *
 * Postconditions:
 *  Returns the requested information.
 */
VpStatusType
VpGetLineInfo(
    VpLineInfoType *pLineInfo)
{
    VpStatusType status = VP_STATUS_SUCCESS;

    if (
        (pLineInfo == VP_NULL) ||
        ((pLineInfo->pDevCtx == VP_NULL) && (pLineInfo->pLineCtx == VP_NULL))
    ) {
        VP_API_ENTER(None, VP_NULL, "GetLineInfo");
        VP_API_EXIT(None, VP_NULL, "GetLineInfo", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    } else if (pLineInfo->pLineCtx != VP_NULL) {
        VP_API_ENTER(VpLineCtxType, pLineInfo->pLineCtx, "GetLineInfo");

        if (pLineInfo->pDevCtx != VP_NULL) {
            VP_API_EXIT(VpLineCtxType, pLineInfo->pLineCtx, "GetLineInfo", VP_STATUS_INVALID_ARG);
            return VP_STATUS_INVALID_ARG;
        }
        pLineInfo->pDevCtx = pLineInfo->pLineCtx->pDevCtx;
    } else /* (pLineInfo->pLineCtx == VP_NULL) && (pLineInfo->pDevCtx != VP_NULL) */ {
        uint8 channelId = pLineInfo->channelId;
        VP_API_ENTER(VpDevCtxType, pLineInfo->pDevCtx, "GetLineInfo");

        if (channelId >= VP_MAX_LINES_PER_DEVICE) {
            status = VP_STATUS_INVALID_ARG;
        } else {
            pLineInfo->pLineCtx = pLineInfo->pDevCtx->pLineCtx[channelId];
            if (pLineInfo->pLineCtx == VP_NULL) {
                status = VP_STATUS_INVALID_LINE;
            }
        }

        if (status != VP_STATUS_SUCCESS) {
            VP_API_EXIT(VpDevCtxType, pLineInfo->pDevCtx, "GetLineInfo", status);
            return status;
        }
    }

    /* Got valid Line Context, Device Context.  Get the other info from the Line Object. */
    switch (pLineInfo->pDevCtx->deviceType) {

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES: {
            Vp880LineObjectType *pLineObj = (Vp880LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES: {
            Vp890LineObjectType *pLineObj = (Vp890LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES: {
            Vp790LineObjectType *pLineObj = (Vp790LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES: {
            Vp792LineObjectType *pLineObj = (Vp792LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES: {
            Vp580LineObjectType *pLineObj = (Vp580LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES: {
            VpVcpLineObjectType *pLineObj = (VpVcpLineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES: {
            VpVcp2LineObjectType *pLineObj = (VpVcp2LineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP: {
            VpKWrapLineObjectType *pLineObj = (VpKWrapLineObjectType *)pLineInfo->pLineCtx->pLineObj;
            pLineInfo->channelId = pLineObj->channelId;
            pLineInfo->termType = pLineObj->termType;
            pLineInfo->lineId = pLineObj->lineId;
            break;
        }
#endif

        default:
            status = VP_STATUS_INVALID_ARG;
    }

    VP_API_EXIT(VpLineCtxType, pLineInfo->pLineCtx, "GetLineInfo", status);
    return status;
} /* VpGetLineInfo() */

/**
 * VpDtmfDigitDetected()
 *  This function is used to set a value in the API-II indicating that a DTMF
 * digit was detected in an external application/process.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  A value in the API-II is set which indicates the digit detected. The most
 * recent value is stored.
 */
VpStatusType
VpDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "DtmfDigitDetected");

    /* Basic argument checking */
    if ((pLineCtx == VP_NULL) || (VpIsDigit(digit) == FALSE)) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(DtmfDigitDetected, (pLineCtx, digit, sense));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "DtmfDigitDetected", status);
    return status;
}

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/**
 * VpQuery()
 *  This function queries a SLAC register for the specified line.
 * For more information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Initiates SLAC register read operation.
 */
VpStatusType
VpQuery(
    VpLineCtxType *pLineCtx,
    VpQueryIdType queryId,
    uint16 handle)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "Query");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(Query, (pLineCtx, queryId, handle));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "Query", status);
    return status;
} /* VpSelfTest() */
#endif
