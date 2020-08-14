/** \file vp890_config.c
 * vp890_config.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Configuration Functions.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7342 $
 * $LastChangedDate: 2010-06-14 18:11:12 -0500 (Mon, 14 Jun 2010) $
 */

/* INCLUDES */
#include    "vp_api.h"

#if defined (VP_CC_890_SERIES)  /* Compile only if required */

#include    "vp_api_int.h"
#include    "vp890_api_int.h"
#include    "sys_service.h"

/* =================================
    Prototypes for Static Functions
   ================================= */
static void
InitDeviceObject(
    Vp890DeviceObjectType *pDevObj);

static void
InitLineObject(
    Vp890LineObjectType *pLineObj);

/* ===================
    Support Functions
   =================== */

/*******************************************************************************
 * VpMakeVp890DeviceObject()
 * This function
 *
 * Arguments:
 *
 *  pDevCtx         -   Device Context to be initialized by other input
 *  pDevObj         -   Device Object to be pointed to by device context
 *
 * Preconditions:
 *
 * Postconditions:
 * Initializes device context to be able to handle VP890 functionality.
 ******************************************************************************/

VpStatusType
VpMakeVp890DeviceObject(
    VpDevCtxType            *pDevCtx,
    Vp890DeviceObjectType   *pDevObj)
{
    InitDeviceObject(pDevObj);

    if (pDevCtx != VP_NULL) {
        return VpMakeVp890DeviceCtx(pDevCtx, pDevObj);
    } else {
        return VP_STATUS_SUCCESS;
    }
} /* VpMakeVp890DeviceObject() */

/*******************************************************************************
 * VpMakeVp890DeviceCtx()
 * This function initializes the device context to handle VP890 functionality.
 *
 * Arguments:
 *
 *  pDevCtx         -   Device Context to be initialized by other input
 *  pDevObj         -   Device Object to be pointed to by device context
 *
 * Preconditions:
 * This function should be called after initializing the device object. This
 * function can be called more than once without modifying the contents of the
 * device object.
 *
 * Postconditions:
 * Initializes device context to be able to handle VP890 functionality.
 ******************************************************************************/
VpStatusType
VpMakeVp890DeviceCtx(
    VpDevCtxType            *pDevCtx,
    Vp890DeviceObjectType   *pDevObj)
{
    uint8 channelCount, maxChan;

    /* redundant checking of the object and context */
    if((pDevCtx == VP_NULL) || (pDevObj == VP_NULL)) {
        VP_ERROR(None, VP_NULL, ("VpMakeVp890DeviceCtx() - Device context and device object cannot be NULL"));
        return VP_STATUS_INVALID_ARG;
    }
    /* Initialize the members of device context */
    pDevCtx->pDevObj    = pDevObj;
    pDevCtx->deviceType = VP_DEV_890_SERIES;

    /*
     * Initialize all of the line context pointers to null in the device context
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (channelCount = 0; channelCount < maxChan; channelCount++) {
        pDevCtx->pLineCtx[channelCount] = VP_NULL;
    }

    /* System Configuration function pointers */
    pDevCtx->funPtrsToApiFuncs.MakeLineObject   = Vp890MakeLineObject;

    /* Initialization function pointers */
    pDevCtx->funPtrsToApiFuncs.InitDevice       = Vp890InitDevice;
    pDevCtx->funPtrsToApiFuncs.InitLine         = Vp890InitLine;
    pDevCtx->funPtrsToApiFuncs.ConfigLine       = Vp890ConfigLine;

#if defined (VP890_FXS_SUPPORT) && defined (VP_CSLAC_RUNTIME_CAL_ENABLED)
    pDevCtx->funPtrsToApiFuncs.CalLine          = Vp890CalLine;
#endif
#if defined (VP890_FXS_SUPPORT)
    pDevCtx->funPtrsToApiFuncs.FreeRun          = Vp890FreeRun;
#endif

    pDevCtx->funPtrsToApiFuncs.Cal              = Vp890Cal;

#ifdef VP_CSLAC_SEQ_EN
#ifdef VP890_FXS_SUPPORT
    pDevCtx->funPtrsToApiFuncs.InitRing         = Vp890InitRing;
    pDevCtx->funPtrsToApiFuncs.InitCid          = Vp890InitCid;
    pDevCtx->funPtrsToApiFuncs.SendCid          = Vp890SendCid;
    pDevCtx->funPtrsToApiFuncs.ContinueCid      = Vp890ContinueCid;
#endif
    pDevCtx->funPtrsToApiFuncs.DtmfDigitDetected = VpCSLACDtmfDigitDetected;
    pDevCtx->funPtrsToApiFuncs.SendSignal       = Vp890SendSignal;
#endif
    pDevCtx->funPtrsToApiFuncs.InitProfile      = Vp890InitProf;

    /* Control function pointers */
    pDevCtx->funPtrsToApiFuncs.ApiTick          = Vp890ApiTick;
    pDevCtx->funPtrsToApiFuncs.VirtualISR       = Vp890VirtualISR;
    pDevCtx->funPtrsToApiFuncs.SetLineState     = Vp890SetLineState;
    pDevCtx->funPtrsToApiFuncs.SetLineTone      = Vp890SetLineTone;

#ifdef VP890_FXS_SUPPORT
    pDevCtx->funPtrsToApiFuncs.SetRelayState    = Vp890SetRelayState;
#endif

    pDevCtx->funPtrsToApiFuncs.SetRelGain       = Vp890SetRelGain;

    pDevCtx->funPtrsToApiFuncs.SetOption        = Vp890SetOption;
    pDevCtx->funPtrsToApiFuncs.DeviceIoAccess   = Vp890DeviceIoAccess;
    pDevCtx->funPtrsToApiFuncs.LowLevelCmd      = Vp890LowLevelCmd;

    /* Status and Query function pointers */
    pDevCtx->funPtrsToApiFuncs.GetEvent         = Vp890GetEvent;
    pDevCtx->funPtrsToApiFuncs.GetLineStatus    = VpCSLACGetLineStatus;
    pDevCtx->funPtrsToApiFuncs.GetDeviceStatus  = Vp890GetDeviceStatus;
    pDevCtx->funPtrsToApiFuncs.GetOption        = Vp890GetOption;
    pDevCtx->funPtrsToApiFuncs.FlushEvents      = Vp890FlushEvents;
    pDevCtx->funPtrsToApiFuncs.GetResults       = Vp890GetResults;
    pDevCtx->funPtrsToApiFuncs.ClearResults     = VpCSLACClearResults;

#ifdef VP890_FXS_SUPPORT
    pDevCtx->funPtrsToApiFuncs.GetRelayState    = Vp890GetRelayState;
#endif

#if (VP_CC_DEBUG_SELECT & VP_DBG_ERROR)
    pDevCtx->funPtrsToApiFuncs.RegisterDump     = Vp890RegisterDump;
#endif

#ifdef VP890_INCLUDE_TESTLINE_CODE
    pDevCtx->funPtrsToApiFuncs.TestLine         = Vp890TestLine;
    pDevCtx->funPtrsToApiFuncs.TestLineInt      = Vp890TestLineInt;
    pDevCtx->funPtrsToApiFuncs.TestLineCallback = Vp890TestLineCallback;
#endif

    return VP_STATUS_SUCCESS;
} /* VpMakeVp890DeviceCtx() */


/*******************************************************************************
 * Vp890MakeLineObject()
 * This function
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
VpStatusType
Vp890MakeLineObject(
    VpTermType              termType,
    uint8                   channelId,
    VpLineCtxType           *pLineCtx,
    void                    *pVoidLineObj,
    VpDevCtxType            *pDevCtx)
{
    Vp890LineObjectType       *pLineObj = pVoidLineObj;
    Vp890DeviceObjectType     *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType            deviceId = pDevObj->deviceId;

    /* Basic error checking */
    if (channelId >= pDevObj->staticInfo.maxChannels) {
        VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890MakeLineObject() - Bad channelId"));
        return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    InitLineObject(pLineObj);

    /* Figure out if the line is an FXO or FXS */
    switch (termType) {
        case VP_TERM_FXO_GENERIC:
            if (channelId != 1) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }
            pLineObj->status |= VP890_IS_FXO;

            /* FXO is always physical line 1 ([0:1] range). */
            pLineObj->channelId = 1;
            break;

        case VP_TERM_FXS_ISOLATE_LP:
        case VP_TERM_FXS_LOW_PWR:
        case VP_TERM_FXS_SPLITTER_LP:
            pDevObj->stateInt |= (VP890_LINE0_LP | VP890_LINE1_LP);

        case VP_TERM_FXS_GENERIC:
        case VP_TERM_FXS_SPLITTER:
        case VP_TERM_FXS_ISOLATE:
            pLineObj->channelId = channelId;
            pLineObj->status = VP890_INIT_STATUS;
            break;

        default:
            VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890MakeLineObject() - Unsupported termination type"));
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_VTD_CODE;
    }

    /* Initialize line context */
    pLineCtx->pLineObj  = pLineObj;
    pLineCtx->pDevCtx   = pDevCtx;

    /* Establish the link between device context to line context */
    pDevCtx->pLineCtx[channelId] = pLineCtx;

    /* Initialize line object */
    pLineObj->termType  = termType;

    /* Note that we are not initializing the 'lineId' member because we do not
     * know what it should be initialized (that information is not provided
     * through the VpMakeLineObj() function) and the basic type of this member
     * is defined by the customer and hence this variable could not have one
     * default value. */

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}  /* Vp890MakeLineObject() */

/*******************************************************************************
 * Vp890InitDeviceObject()
 *  This function initializes the Vp890 Device object data structure. It is
 * called only in this file .
 ******************************************************************************/
static void
InitDeviceObject(
    Vp890DeviceObjectType *pDevObj)
{
    VpMemSet(pDevObj, 0, sizeof(Vp890DeviceObjectType));

    pDevObj->staticInfo.maxChannels = VP890_MAX_NUM_CHANNELS;

#ifdef VP_DEBUG
    /* Initialize the debug mask as soon as the device object in created */
    pDevObj->debugSelectMask = VP_OPTION_DEFAULT_DEBUG_SELECT;
#endif
} /* Vp890InitDeviceObject() */

/*******************************************************************************
 * Vp890InitLineObject()
 *  This function initializes the Vp890 Line Object data structure. It is
 * called only in this file .
 ******************************************************************************/
static void
InitLineObject(
    Vp890LineObjectType *pLineObj)
{
    uint16 objSize;
    uint8 *objPtr = (uint8 *)pLineObj;

    for (objSize = 0;
         objSize < (sizeof(Vp890LineObjectType) / sizeof(uint8));
         objSize++) {

        *objPtr = 0;
        objPtr++;
    }
#ifdef VP_DEBUG
    /* Initialize the debug mask as soon as the line object in created */
    pLineObj->debugSelectMask = VP_OPTION_DEFAULT_DEBUG_SELECT;
#endif
} /* Vp890InitLineObject() */


#endif /* VP_CC_890_SERIES */
