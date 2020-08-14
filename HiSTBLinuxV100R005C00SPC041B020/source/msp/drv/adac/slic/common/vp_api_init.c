/** \file vp_api_init.c
 * vp_api_init.c
 *
 *  This file contains the implementation of top level VoicePath API-II
 * Initialization procedures.
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
 *                        INITIALIZATION FUNCTIONS                            *
 ******************************************************************************/
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/*
 * VpBootLoad()
 * This function is used to download the boot image to the device.
 * This function must be called upon power on reset for VCP classes
 * of devices to download boot image for these devices. This function is
 * not applicable for CSLAC class of devices. See VP-API-II documentation
 * for more information about this function.
 *
 * Preconditions:
 * Device context must be created before calling this function.
 *
 * Postconditions:
 * This function downloads the boot image to the part and configures the part
 * to start excecuting the image that was downloaded.
 */
VpStatusType
VpBootLoad(
    VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "BootLoad");

    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(BootLoad, (pDevCtx, state, pImageBuffer, bufferSize, pScratchMem, validation));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "BootLoad", status);
    return status;
} /* VpBootLoad() */

/*
 * VpBootSlac()
 * This function is used to download the boot image to one or all SLACs
 * controlled by a VCP device without affecting the code on the VCP or other
 * SLACs.
 *
 * Preconditions:
 * Either pLineCtx or pDevCtx must be valid.  The other must be VP_NULL.
 *
 * Postconditions:
 * If pLineCtx is not VP_NULL, the SLAC associated with the given line will be
 * reset and loaded with the image provided.  If pDevCtx is not VP_NULL, all
 * SLACs associated with the given VCP device will be reset and loaded.
 */
VpStatusType
VpBootSlac(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtxParam,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize)
{
    VpDevCtxType *pDevCtx;
    VpStatusType status = VP_STATUS_SUCCESS;
    bool singleSlac = (pLineCtx != VP_NULL);

    /* Basic argument checking */
    if (singleSlac) {
        VP_API_ENTER(VpLineCtxType, pLineCtx, "BootSlac");
        if (pDevCtxParam != VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
        pDevCtx = pLineCtx->pDevCtx;
    } else {
        pDevCtx = pDevCtxParam;
        VP_API_ENTER(VpDevCtxType, pDevCtx, "BootSlac");
        if (pDevCtx == VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
    }
    if (pImageBuffer == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    }

    if (status == VP_STATUS_SUCCESS) {
        status = VP_CALL_DEV_FUNC(BootSlac, (pLineCtx, pDevCtxParam, pImageBuffer, bufferSize));
    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (singleSlac) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "BootSlac", status);
    } else {
        VP_API_EXIT(VpDevCtxType, pDevCtx, "BootSlac", status);
    }
#endif

    return status;
} /* VpBootSlac() */
#endif

/*
 * VpInitDevice()
 *  This function calibrates the device and initializes all lines (for which
 * line context has been created and intialized) on the device with the AC, DC,
 * and Ringing parameters passed. See VP-API reference guide for more
 * information.
 *
 * Preconditions:
 *  This function should be called only after creating and initializing the
 * device context and line context (atleast for those lines which need service).
 *
 * Postconditions:
 *  Device is calibrated and all lines (for whom line context has been created
 * and initialized) associated with this device are initialized with the AC, DC,
 * and Ringing Paramaters passed (DC and Ringing apply to FXS type lines only).
 */
VpStatusType
VpInitDevice(
    VpDevCtxType *pDevCtx,          /**< Pointer to device context for the
                                     * device that will be initialized
                                     */
    VpProfilePtrType pDevProfile,   /**< Pointer to Device Profile */
    VpProfilePtrType pAcProfile,    /**< Pointer to AC Profile that is applied
                                     * to all FXS lines on this device
                                     */
    VpProfilePtrType pDcProfile,    /**< Pointer to DC Profile that is applied
                                     * to all FXS lines on this device
                                     */
    VpProfilePtrType pRingProfile,  /**< Pointer to Ringing Profile that is
                                     * applied to all FXS lines on this device
                                     */
    VpProfilePtrType pFxoAcProfile, /**< Pointer to AC Profile that is applied
                                     * to all FXO lines on this device
                                     */
    VpProfilePtrType pFxoCfgProfile)/**< Pointer to Config Profile that is
                                     * applied to all FXO lines on this device
                                     */
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "InitDevice");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(InitDevice, (pDevCtx, pDevProfile, pAcProfile, pDcProfile, pRingProfile, pFxoAcProfile, pFxoCfgProfile));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "InitDevice", status);
    return status;
} /* VpInitDevice() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/*
 * VpInitSlac()
 *  This function performs the same operations on a per-SLAC basis that
 * VpInitDevice() does for all devices. See VpInitDevice() for details.
 *
 * Preconditions:
 *  The device and line context must be created and initialized and the
 * controller device must be initialized (via VpInitDevice).
 *
 * Postconditions:
 *  SLAC is calibrated and all lines associated with this SLAC are initialized
 * with the AC, DC, and Ringing Paramaters passed.
 */
VpStatusType
VpInitSlac(
    VpLineCtxType *pLineCtx,         /**< Pointer any line context associated
                                     * with the SLAC being initialized.
                                     */
    VpProfilePtrType pDevProfile,   /**< Pointer to Device Profile */
    VpProfilePtrType pAcProfile,    /**< Pointer to AC Profile that is applied
                                     * to all FXS lines on this SLAC
                                     */
    VpProfilePtrType pDcProfile,    /**< Pointer to DC Profile that is applied
                                     * to all FXS lines on this SLAC
                                     */
    VpProfilePtrType pRingProfile)  /**< Pointer to Ringing Profile that is
                                     * applied to all FXS lines on this SLAC
                                     */
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "InitSlac");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(InitSlac, (pLineCtx, pDevProfile, pAcProfile, pDcProfile, pRingProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "InitSlac", status);
    return status;
} /* VpInitSlac() */
#endif

/**
 * VpInitLine()
 *  This function initializes the line with the AC, DC, and Ringing parameters
 * passed. It resets the line, loads the default options, performs calibration
 * (if applicable) and it also results in an event being generated.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized also device must
 * be initialized (via VpInitDevice).
 *
 * Postconditions:
 *  The line is initialized with the AC, DC, and Ringing parameters passed.
 * DC and Ringing parameters apply to FXS lines only.
 */
VpStatusType
VpInitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "InitLine");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(InitLine, (pLineCtx, pAcProfile, pDcFeedOrFxoCfgProfile, pRingProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "InitLine", status);
    return status;
} /* VpInitLine() */

/*
 * VpFreeRun()
 *  This function prepares the devices and lines for a system restart.
 *
 * Preconditions:
 *  This function should be called only after creating and initializing the
 * device context and line context (atleast for those lines which need service).
 *
 * Postconditions:
 *  Device and lines are in a system restart "ready" state.
 */
VpStatusType
VpFreeRun(
    VpDevCtxType *pDevCtx,          /**< Pointer to device context for the
                                     * device that will be initialized
                                     */
    VpFreeRunModeType freeRunMode)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "FreeRun");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(FreeRun, (pDevCtx, freeRunMode));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "FreeRun", status);
    return status;
} /* VpFreeRun() */

/**
 * VpConfigLine()
 *  This function initializes the line with the AC, DC, and Ringing parameters
 * passed. See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized also device must
 * be initialized (via VpInitDevice).
 *
 * Postconditions:
 *  The line is initialized with the AC, DC, and Ringing parameters passed.
 * DC and Ringing parameters apply to FXS lines only.
 */
VpStatusType
VpConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "ConfigLine");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(ConfigLine, (pLineCtx, pAcProfile, pDcFeedOrFxoCfgProfile, pRingProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "ConfigLine", status);
    return status;
} /* VpConfigLine() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
/**
 * VpSetBFilter()
 *  This function enables the B-Filter and programs it with the B-Filter values
 * provided in the ac profile, or disables the B-Filter.
 *
 * Preconditions:
 *  The line must be created and initialized before calling this function.
 *
 * Postconditions:
 *  The B-Filter is programmed to either disabled values or to the values passed
 * in the AC-Profile.
 */
VpStatusType
VpSetBFilter(
    VpLineCtxType *pLineCtx,
    VpBFilterModeType bFiltMode,
    VpProfilePtrType pAcProfile)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "SetBFilter");

    /* Basic argument checking */
    if (
        (pLineCtx == VP_NULL) ||
        ((bFiltMode == VP_BFILT_EN) && (pAcProfile == VP_NULL))
    ) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(SetBFilter, (pLineCtx, bFiltMode, pAcProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "SetBFilter", status);
    return status;
} /* VpSetBFilter() */

/**
 * VpSetBatteries()
 *  This function sets the battery values in the device, which for some devices
 * may result in improved feed performance.
 *
 * Preconditions:
 *  The device must be created and initialized before calling this function.
 *
 * Postconditions:
 *  The device is programmed to use the battery values provided, or programmed
 * to use measured battery voltages.
 */
VpStatusType
VpSetBatteries(
    VpLineCtxType *pLineCtx,
    VpBatteryModeType battMode,
    VpBatteryValuesType *pBatt)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "SetBatteries");

    /* Basic argument checking */
    if (
        (pLineCtx == VP_NULL) ||
        (((unsigned int)battMode == VP_BFILT_EN) && (pBatt == VP_NULL))
    ) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(SetBatteries, (pLineCtx, battMode, pBatt));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "SetBatteries", status);
    return status;
} /* VpSetBatteries() */

/**
 * VpCalCodec()
 *  This function initiates a calibration operation for analog circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
VpCalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "CalCodec");

    /* Basic argument checking */
    if (
        (pLineCtx == VP_NULL) ||
        ((mode != VP_DEV_CAL_NOW) && (mode != VP_DEV_CAL_NBUSY))
    ) {
       status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(CalCodec, (pLineCtx, mode));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "CalCodec", status);
    return status;
} /* VpCalCodec() */
#endif

/**
 * VpCalLine()
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
VpCalLine(
    VpLineCtxType *pLineCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "CalLine");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(CalLine, (pLineCtx));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "CalLine", status);
    return status;
} /* VpCalLine() */

/**
 * VpCal()
 *  This function runs a selected calibration option.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action. Event
 * indicates if calibration was successfull and if results are available.
 */
VpStatusType
VpCal(
    VpLineCtxType *pLineCtx,
    VpCalType calType,
    void *inputArgs)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "Cal");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(Cal, (pLineCtx, calType, inputArgs));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "Cal", status);
    return status;
} /* VpCal() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CSLAC_SEQ_EN)
/**
 * VpInitRing()
 *  This function is used to initialize Ringing and CID profile to a given line.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The line context and device context should be created initialized. The boot
 * image should be downloaded before calling this function (for applicable
 * devices).
 *
 * Postconditions:
 *  Applies the Caller ID and Ringing profile.
 */
VpStatusType
VpInitRing(
    VpLineCtxType *pLineCtx,        /**< Line context */
    VpProfilePtrType pCadProfile,   /**< Ringing cadence profile */
    VpProfilePtrType pCidProfile)   /**< Caller ID profile */
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "InitRing");

    /* Basic argument checking */
    if (pCadProfile == VP_NULL && pCidProfile != VP_NULL) {
        /* It doesn't make sense to load a caller ID profile with
         * no ring cadence */
        VP_ERROR(VpLineCtxType, pLineCtx, ("VpInitRing(): Cannot use a NULL ring cadence with a non-NULL caller ID profile"));
        status = VP_STATUS_INVALID_ARG;
    } else if (pLineCtx == VP_NULL) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("VpInitRing(): Invalid NULL pLineCtx"));
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(InitRing, (pLineCtx, pCadProfile, pCidProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "InitRing", status);
    return status;
} /* VpInitRing() */

/**
 * VpInitCid()
 *  This function is used to send caller ID information. See VP-API reference
 * guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. This function needs to be called before placing the line in to
 * ringing state.
 *
 * Postconditions:
 *  This function transmits the given CID information on the line (when the line
 * is placed in the ringing state).
 */
VpStatusType
VpInitCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "InitCid");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(InitCid, (pLineCtx, length, pCidData));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "InitCid", status);
    return status;
} /* VpInitCid() */

/**
 * VpInitMeter()
 *  This function is used to initialize metering parameters. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. This function needs to be called before placing the line in to
 * ringing state.
 *
 * Postconditions:
 *  This function initializes metering parameters as per given profile.
 */
VpStatusType
VpInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile)
{
    VpStatusType status;
    VP_API_ENTER(VpLineCtxType, pLineCtx, "InitMeter");

    /* Basic argument checking */
    if (pLineCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else if (pMeterProfile == VP_NULL) {
        /* If metering profile is null, there is nothing to do. */
        status = VP_STATUS_SUCCESS;
    } else {
        VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
        status = VP_CALL_DEV_FUNC(InitMeter, (pLineCtx, pMeterProfile));
    }

    VP_API_EXIT(VpLineCtxType, pLineCtx, "InitMeter", status);
    return status;
} /* VpInitMeter() */
#endif

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
/**
 * VpInitCustomTerm()
 *  This function is used to initialize the I/O control for a custom termination
 * type.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. It is required to be called for term type CUSTOM before calling
 * VpInitDevice()/VpInitLine().
 *
 * Postconditions:
 *  This function initializes I/O control parameters as per given profile. When
 * called with device context only, all custom lines on the device are affected
 * by the profile passed. When a line context only is passed, then only that
 * line context is affected.
 */
VpStatusType
VpInitCustomTermType(
    VpDevCtxType *pDevCtxParam,
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile)
{
    VpDevCtxType *pDevCtx;
    VpStatusType status = VP_STATUS_SUCCESS;
    bool singleLine = (pLineCtx != VP_NULL);

    /* Basic argument checking */
    if (singleLine) {
        VP_API_ENTER(VpLineCtxType, pLineCtx, "InitCustomTermType");
        if (pDevCtxParam != VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
        pDevCtx = pLineCtx->pDevCtx;
    } else {
        VP_API_ENTER(VpDevCtxType, pDevCtxParam, "InitCustomTermType");
        pDevCtx = pDevCtxParam;
        if (pDevCtx == VP_NULL) {
            status = VP_STATUS_INVALID_ARG;
        }
    }

    if (status == VP_STATUS_SUCCESS) {
        status = VP_CALL_DEV_FUNC(InitCustomTerm, (pDevCtxParam, pLineCtx, pCustomTermProfile));
    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (singleLine) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "InitCustomTermType", status);
    } else {
        VP_API_EXIT(VpDevCtxType, pDevCtxParam, "InitCustomTermType", status);
    }
#endif

    return status;
} /* VpInitCustomTermType() */
#endif

/**
 * VpInitProfile()
 *  This function is used to initialize profile tables in VCP. See
 * VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Stores the given profile at the specified index of the profile table.
 */
VpStatusType
VpInitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "InitProfile");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL || pProfileIndex == VP_PTABLE_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(InitProfile, (pDevCtx, type, pProfileIndex, pProfile));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "InitProfile", status);
    return status;
} /* VpInitProfile() */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KWRAP)
/**
 * VpSoftReset()
 *  This function resets VCP without requiring another image load.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and must be created and initialized before calling this function.
 *
 * Postconditions:
 *  The part is reset.
 */
VpStatusType
VpSoftReset(
    VpDevCtxType *pDevCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "SoftReset");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(SoftReset, (pDevCtx));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "SoftReset", status);
    return status;
} /* VpSoftReset() */
#endif
