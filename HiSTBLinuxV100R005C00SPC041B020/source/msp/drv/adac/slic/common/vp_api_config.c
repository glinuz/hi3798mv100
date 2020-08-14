/** \file vp_api_config.c
 * vp_api_config.c
 *
 *  This file contains the implementation of top level VoicePath API-II.
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
 *                     SYSTEM CONFIGURATION FUNCTIONS                         *
 ******************************************************************************/
/**
 * VpMakeDeviceObject()
 *  This function creates a device context using the information that is
 * provided. This funciton should be the first API function that should be
 * called. This function is like C++ constructor. If the passed device type is
 * not valid or the code for the device type is not compiled in, this function
 * returns error. Please see VP-API documentation for more information.
 *
 * Preconditions:
 *  The device context, device object pointers must be non zero and device type
 * must be valid. The type of device object should match with device type. The
 * deviceId must uniquely determine a chipselect for the device of interest in
 * HAL layer.
 *
 * Postconditions:
 *  The device context and device object are initialized and this function
 * returns success if context is created properly.
 */
VpStatusType
VpMakeDeviceObject(
    VpDeviceType deviceType,    /**< Device Type */
    VpDeviceIdType deviceId,    /**< Hardware chip select for this device */

    VpDevCtxType *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
    void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8 i;
    VpTempFuncPtrType *funcPtr;
    VpStatusType status;
    VP_API_ENTER(None, VP_NULL, "MakeDeviceObject");

    /* Basic argument checking */
    if ((pDevObj == VP_NULL) || (pDevCtx == VP_NULL)) {
        VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    } else if ((deviceType != VP_DEV_VCP_SERIES) &&
        (deviceType != VP_DEV_VCP2_SERIES) &&
        (deviceType != VP_DEV_880_SERIES) &&
        (deviceType != VP_DEV_890_SERIES) &&
        (deviceType != VP_DEV_790_SERIES) &&
        (deviceType != VP_DEV_580_SERIES) &&
        (deviceType != VP_DEV_792_SERIES) &&
        (deviceType != VP_DEV_792_GROUP) &&
        (deviceType != VP_DEV_KWRAP)) {
        VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_ERR_VTD_CODE);
        return VP_STATUS_ERR_VTD_CODE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if(pDevCtx != VP_NULL) {
        /* User wants to create device context as well; First clear it */
        /* Initialize All the funciton pointers to zero */
        funcPtr = (VpTempFuncPtrType *)&pDevCtx->funPtrsToApiFuncs;
        for(i = 0; i < (sizeof(ApiFunctions) / sizeof(VpTempFuncPtrType)); i++){
            *funcPtr = VP_NULL;
            funcPtr++;
        }

        /* Initialize all the line context pointers to null */
        for(i = 0; i < VP_MAX_LINES_PER_DEVICE; i++) {
            pDevCtx->pLineCtx[i] = VP_NULL;
        }
    }

    pDevCtx->deviceType = deviceType;

    switch (deviceType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            ((VpVcpDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VpMakeVcpDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            ((VpVcp2DeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = Vcp2MakeDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            status = VpMakeVp880DeviceObject(pDevCtx, pDevObj);
            ((Vp880DeviceObjectType *)pDevObj)->deviceId = deviceId;
            ((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0] = 0;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            status = VpMakeVp890DeviceObject(pDevCtx, pDevObj);
            ((Vp890DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            status = VpMakeVp790DeviceObject(pDevCtx, pDevObj);
            ((Vp790DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            status = Vp792MakeDeviceObject(pDevCtx, pDevObj);
            ((Vp792DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_792_GROUP)
        case VP_DEV_792_GROUP:
            pDevCtx->pDevObj = pDevObj;
            pDevCtx->funPtrsToApiFuncs.GetEvent = Vp792GroupGetEvent;
            ((Vp792GroupDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VP_STATUS_SUCCESS;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            status = VpMakeVp580DeviceObject(pDevCtx, pDevObj);
            ((Vp580DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;
            break;
#endif
        default:
            /*
             * This error check should be performed pointers are initialized, so
             * this code should never be executed -- error checking above
             */
            status = VP_STATUS_ERR_VTD_CODE;
            break;
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", status);
    return status;
} /* VpMakeDeviceObject() */

/**
 * VpMakeDeviceCtx()
 *  This function links a device object to a device context and initializes
 * the function pointers.  This function must be called if VpMakeDeviceObject
 * has not been called with a valid device context. This function enables more
 * than one device context to be created for a shared device object referring to
 * to a given device. This feature is useful when more than one process is
 * controlling a given device.
 *
 * Preconditions:
 *  The device context and device object pointers must be non zero. Device
 * Object must be created before calling this function.
 *
 * Postconditions:
 *  The device context and device object are linked and the device context
 * function pointers are initialized.
 */
VpStatusType
VpMakeDeviceCtx(
    VpDeviceType deviceType,    /**< Device Type - must match type in pDevObj */
    VpDevCtxType *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
    void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8 i;
    VpStatusType status;
    /* Pointer to a function pointer array (of same function type) */
    VpTempFuncPtrType *funcPtr;

    VP_API_ENTER(None, VP_NULL, "MakeDeviceCtx");

    /* Basic argument checking */
    if ((pDevCtx == VP_NULL) || (pDevObj == VP_NULL)) {
        VP_API_EXIT(None, VP_NULL, "MakeDeviceCtx", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    /* First Initialize all the funciton pointers to zero */
    funcPtr = (VpTempFuncPtrType *)&pDevCtx->funPtrsToApiFuncs;
    for(i = 0; i < (sizeof(ApiFunctions) / sizeof(VpTempFuncPtrType)); i++) {
        *funcPtr = VP_NULL;
        funcPtr++;
    }

    /* Initialize all the line context pointers to null */
    for(i = 0; i < VP_MAX_LINES_PER_DEVICE; i++) {
        pDevCtx->pLineCtx[i] = VP_NULL;
    }

    pDevCtx->deviceType = deviceType;

    switch (deviceType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            status = VpMakeVcpDeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            status = Vcp2MakeDeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            status = VpMakeVp880DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            status = VpMakeVp890DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            status = VpMakeVp790DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            status = Vp792MakeDeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            status = VpMakeVp580DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;
            break;
#endif
        default:
            status = VP_STATUS_ERR_VTD_CODE;
            break;
    }

    VP_API_EXIT(None, VP_NULL, "MakeDeviceCtx", status);
    return status;
} /* VpMakeDeviceCtx() */

/**
 * VpMakeLineObject()
 *  This function initializes a line context using the information that is
 * passed. This function is like a C++ constructor. It initializes the passed
 * line context and line object based on the paramters provided. The passed line
 * object type should match with the type of device object type. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  This function assumes device context has already been created and
 * initialized. This function should only be called after downloading the boot
 * image the device when applicable (like for VCP class of devices).
 *
 * Postconditions:
 *  This function initializes the line context/line object. Line related VP-API
 * functions can be called after calling this function.
 */
VpStatusType
VpMakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "MakeLineObject");

    /* Basic argument checking */
    if ((pLineObj == VP_NULL) || (pDevCtx == VP_NULL) || (pLineCtx == VP_NULL)) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        pLineCtx->pLineObj = VP_NULL;
        status = VP_CALL_DEV_FUNC(MakeLineObject, (termType, channelId, pLineCtx, pLineObj, pDevCtx));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "MakeLineObject", status);
    return status;
} /* VpMakeLineObject() */

/**
 * VpMakeLineCtx()
 *  This function makes the association from line context to line object and
 * device context. It must be called if VpMakeLineObject() is called with line
 * context set to VP_NULL. This function allows line objects and line contexts
 * to be created in separate steps. In multiprocess environment this function
 * allows associating more than one line contexts with one shared line object
 * for a given line.
 *
 * Preconditions:
 *  None of the arguments can be VP_NULL. Line object must be created before
 * calling this function.
 *
 * Postconditions:
 *  The line context is associated with the line object and device context.
 */
VpStatusType
VpMakeLineCtx(
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx)
{
    VpLineInfoType lineInfo;
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "MakeLineCtx");

    /* Basic argument checking */
    if ((pLineCtx == VP_NULL) || (pLineObj == VP_NULL) || (pDevCtx == VP_NULL)) {
        VP_API_EXIT(VpDevCtxType, pDevCtx, "MakeLineCtx", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * Make the device context member of the line context point to the
     * device context passed. This links this line context with a specific
     * device context.
     */
    pLineCtx->pDevCtx = pDevCtx;

    /*
     * Make the line object member of this line context point to the passed
     * line object pointer
     */
    pLineCtx->pLineObj = pLineObj;

    /* Get channel id from line context */
    lineInfo.pLineCtx = pLineCtx;
    lineInfo.pDevCtx = VP_NULL;
    status = VpGetLineInfo(&lineInfo);

    /*
     * Make the indexed line context array in the device context point to the
     * line context passed.
     */
    if (status == VP_STATUS_SUCCESS) {
        pDevCtx->pLineCtx[lineInfo.channelId] = pLineCtx;
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "MakeLineCtx", status);
    return status;
} /* VpMakeLineCtx() */

/**
 * VpFreeLineCtx()
 *  This function frees the association from line context to device context. It
 * must be called if the application is freeing the memory associated with the
 * line context or object.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  All areas of the API-II where this line context may be used is set to
 * VP_NULL.
 */
VpStatusType
VpFreeLineCtx(
    VpLineCtxType *pLineCtx)
{
    uint8 channelId;
    VpDevCtxType *pDevCtx;
    void *pLineObj, *pDevObj;
    VpDeviceIdType deviceId;    /**< Hardware chip select for this device */
    VP_API_ENTER(VpLineCtxType, pLineCtx, "VpFreeLineCtx");

    if (pLineCtx == VP_NULL) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "VpFreeLineCtx", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    pDevObj = pDevCtx->pDevObj;
    pLineObj = pLineCtx->pLineObj;

    /*
     * Get the channel ID in the device context that is associated with this
     * line context.
     */

    switch(pDevCtx->deviceType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            channelId = ((VpVcpLineObjectType *)pLineObj)->channelId;
            deviceId = ((VpVcpDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            channelId = ((VpVcp2LineObjectType *)pLineObj)->channelId;
            deviceId = ((VpVcp2DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            channelId = ((Vp890LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            channelId = ((Vp792LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp792DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            channelId = ((Vp580LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
#if 0
            VP_API_EXIT(VpLineCtxType, pLineCtx, "VpFreeLineCtx", VP_STATUS_FUNC_NOT_SUPPORTED);
            return VP_STATUS_FUNC_NOT_SUPPORTED;
#endif
            channelId = ((VpKWrapLineObjectType *)pLineObj)->channelId;
            deviceId = ((VpKWrapDeviceObjectType *)pDevObj)->deviceId;
            break;

#endif

        default:
            VP_API_EXIT(VpLineCtxType, pLineCtx, "VpFreeLineCtx", VP_STATUS_INVALID_ARG);
            return VP_STATUS_INVALID_ARG;
    }

    /* Free the line context of this device */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevCtx->pLineCtx[channelId] = VP_NULL;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    VP_API_EXIT(VpLineCtxType, pLineCtx, "VpFreeLineCtx", VP_STATUS_SUCCESS);
    return VP_STATUS_SUCCESS;
} /* VpFreeLineCtx() */

/**
 * VpMapLineId()
 *  This function assignes a lineId to a line object. It is used by the
 * application when mapping a device and channel Id to a specific line
 * context is not convenient (or easy). The value of lineId is reported
 * in the event structure.
 *
 * Preconditions:
 *  The line context and device object pointers must be non zero. Device
 * Object must be created before calling this function.
 *
 * Postconditions:
 *  The device context and device object are linked and the device context
 * function pointers are initialized.
 */
VpStatusType
VpMapLineId(
    VpLineCtxType *pLineCtx,   /**< Line Context to map to lineId */
    VpLineIdType lineId)       /**< Value assigned as line Id */
{
    VpDevCtxType *pDevCtx;
    VpDeviceIdType deviceId;
    void *pLineObj, *pDevObj;
    VpDeviceType devType;

    VpStatusType status = VP_STATUS_SUCCESS;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "MapLineId");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "MapLineId", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    pLineObj = pLineCtx->pLineObj;

    devType = pDevCtx->deviceType;
    pDevObj = pDevCtx->pDevObj;

    switch (devType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            deviceId = ((VpVcpDeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((VpVcpLineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            deviceId = ((VpVcp2DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((VpVcp2LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp880LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            deviceId = ((Vp890DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp890LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif


#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp790LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            deviceId = ((Vp792DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp792LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp580LineObjectType *)pLineObj)->lineId = lineId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;
            break;
#endif

        default:
            deviceId = deviceId;    /* Accommodate compiler bug in GCC 3.4.5 */
            status = VP_STATUS_ERR_VTD_CODE;
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "MapLineId", status);
    return status;
} /* VpMapLineId() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/**
 * VpMapSlacId()
 *  This function assignes a slacId to a device object. It is used by the
 * application when mapping a SLAC ID to a specific device context using a
 * common deviceId. The value of slacId is reported in the event structure.
 *
 * Preconditions:
 *  The device context and device object pointers must be non zero. Device
 *  Object must be created before calling this function.
 *
 * Postconditions:
 *  The device context and device object are linked and the device context
 * function pointers are initialized.
 */
VpStatusType
VpMapSlacId(
    VpDevCtxType *pDevCtx,   /**< Device Context to map to lineId */
    uint8 slacId)            /**< Value assigned as slac Id */
{
    VpStatusType status = VP_STATUS_SUCCESS;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "MapSlacId");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        VP_API_EXIT(None, VP_NULL, "MapSlacId", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    }

    switch (pDevCtx->deviceType) {

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES: {
            Vp792DeviceObjectType *pDevObj = pDevCtx->pDevObj;
            VpDeviceIdType deviceId = pDevObj->deviceId;
            if (VP792_MAX_SLAC_ID < slacId) {
                status = VP_STATUS_INVALID_ARG;
                break;
            }
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            pDevObj->slacId = slacId;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
        }
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = KWrapMapSlacId(pDevCtx, slacId);
            break;
#endif

        default:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "MapSlacId", status);
    return status;
}
#endif

