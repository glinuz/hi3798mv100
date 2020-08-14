/** \file vp890_init.c
 * vp890_init.c
 *
 *  This file contains the implementation of the VP-API 890 Series
 *  Initialization Functions.
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

/* =================================
    Prototypes for Static Functions
   ================================= */

static VpStatusType
InitDeviceProfile(
    Vp890DeviceObjectType   *pDevObj,
    VpProfilePtrType        pDevProfile);

static bool
InitDeviceMclk(
    Vp890DeviceObjectType   *pDevObj,
    VpDeviceIdType          deviceId);

static VpStatusType
InitDevicePcnRcn(
    Vp890DeviceObjectType   *pDevObj,
    VpDeviceIdType          deviceId);

#ifdef VP890_FXS_SUPPORT
static VpStatusType
InitFxsLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pDcProfile,
    VpProfilePtrType        pRingProfile);

static VpStatusType
Vp890ProcessDcProfile(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pDcOrFxoProfile,
    VpProfilePtrType        pRingProf);

static void
Vp890CopyDefaultFRProfile(
    Vp890DeviceObjectType *pDevObj);
#endif

#ifdef VP890_FXO_SUPPORT
static VpStatusType
InitFxoLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pFxoProfile);
#endif

static VpStatusType
InitProfToTable(
    VpProfilePtrType        pProfile,
    VpProfilePtrType        pProfileIndex,
    VpProfileType           profType,
    uint16                  tableSize,
    uint16                  *pProfEntry,
    VpProfilePtrType        *pProfTable);

/* ===================
    Support typedefs
   =================== */

#define VP890_DEV_PROFILE_OPERATIONAL_CFG   (2u)
#define VP890_DEV_PROFILE_PK_PWR_MGMT       0x01
#define VP890_DEV_PROFILE_LOW_VOLT_OVERRIDE 0x02

#define VP890_DEV_PROFILE_PCLK_MSB          (6u)
#define VP890_DEV_PROFILE_PCLK_LSB          (7u)
#define VP890_DEV_PROFILE_DEVCFG1           (8u)
#define VP890_DEV_PROFILE_MCLK_CORR         (9u)
#define VP890_DEV_PROFILE_CLOCK_SLOT        (10u)
#define VP890_DEV_PROFILE_MAX_EVENTS        (11u)
#define VP890_DEV_PROFILE_TICKRATE_MSB      (12u)
#define VP890_DEV_PROFILE_TICKRATE_LSB      (13u)
#define VP890_DEV_PROFILE_SWITCHER_CMD      (14u)
#define VP890_DEV_PROFILE_SWITCHER_DATA0    (15u)
#define VP890_DEV_PROFILE_SWITCHER_DATA1    (16u)
#define VP890_DEV_PROFILE_SWITCHER_DATA2    (17u)
#define VP890_DEV_PROFILE_TIMING_DATA_START (18u)
#define VP890_DEV_PROFILE_TIMING_DATA_END   (23u)

#define VP890_DEV_PROF_MIN_VERSION  (4) /* No timing parameters provided */
#define VP890_DEV_PROF_W_TIMING     (5)
#define VP890_DEV_PROF_W_TIMING_FR  (6)

/******************************************************************************
 * Vp890InitDevice()
 * This function calibrates the device and initializes all lines on the device
 * with the AC, DC, and Ringing parameters passed. See VP-API reference guide
 * for more information.
 *
 * Preconditions:
 * This function should be called only after creating and initializing the
 * device context.
 *
 * Postconditions:
 * Device is calibrated and all lines associated with this device are
 * initialized with the AC, DC, and Ringing Paramaters passed (DC and Ringing
 * apply to FXS type lines only).
 ******************************************************************************/
VpStatusType
Vp890InitDevice(
    VpDevCtxType        *pDevCtx,       /**< Pointer to device context for the
                                        * device that will be initialized */
    VpProfilePtrType    pDevProfile,    /**< Pointer to Device Profile */
    VpProfilePtrType    pAcProfile,     /**< Pointer to AC Profile that is applied
                                        * to all FXS lines on this device */
    VpProfilePtrType    pDcProfile,     /**< Pointer to DC Profile that is applied
                                        * to all FXS lines on this device */
    VpProfilePtrType    pRingProfile,   /**< Pointer to Ringing Profile that is
                                        * applied to all FXS lines on this device*/
    VpProfilePtrType    pFxoAcProfile,  /**< Pointer to AC Profile that is applied
                                        * to all FXO lines on this device */
    VpProfilePtrType    pFxoCfgProfile) /**< Pointer to Config Profile that is
                                        * applied to all FXO lines on this device*/
{
    VpLineCtxType           *pLineCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpStatusType            status      = VP_STATUS_INVALID_ARG;
    uint8                   chan, maxChan;

    uint8 mpiClear[] = {
        VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT,
        VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT,
        VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT, VP890_NO_OP_WRT,
        VP890_HW_RESET_WRT
    };

    bool delayInitDeviceCompete = FALSE;

    VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("+Vp890InitDevice()"));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pDevObj->state & VP_DEV_INIT_CMP) {
        pDevObj->state = (VP_DEV_WARM_REBOOT | VP_DEV_INIT_IN_PROGRESS);
    } else {
        pDevObj->state = VP_DEV_INIT_IN_PROGRESS;
    }

    pDevObj->stateInt = VP890_RESET;

    /* CHECK THE DEVICE PROFILE FOR ERRORS */
    status = InitDeviceProfile(pDevObj, pDevProfile);
    if( status != VP_STATUS_SUCCESS ) {
        pDevObj->state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return status;
    }

    /* INIT TIMERS -
     *
     * It is possible that any channel on a device is not used and therefore
     * points to a NULL pointer. Check it and jump over if this is the case
     * to the next line until all possible line numbers for this device have
     * been checked.
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (chan = 0; chan < maxChan; chan++) {
        pLineCtx = pDevCtx->pLineCtx[chan];
        if (pLineCtx != VP_NULL) {
            InitTimerVars(pLineCtx);
        }
    }

    /* MPI RESET -
     *
     * If the MPI Bus gets out of sequence for any reason, a HW reset command
     * will not work and this function may fail. To be sure a reset occurs, the
     * following sequence is required.
     */

    /* Perform NO_OPS to clear the MPI buffers and the HW Reset */
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_NO_OP_WRT, 16, mpiClear);

    VpSysWait(20);

    /* SETUP MASTER CLOCK - */
    if(InitDeviceMclk(pDevObj, deviceId) == TRUE) {
        pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
        pDevObj->state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_FAILURE;
    }

    /* CHECK THE DEVICES PRODUCT CODE AND REVISION CODE */
    status = InitDevicePcnRcn(pDevObj, deviceId);
    if( status != VP_STATUS_SUCCESS ) {
        pDevObj->state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return status;
    }

    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_TX_RX_CSLOT_WRT, VP890_TX_RX_CSLOT_LEN,
        &pDevObj->devProfileData.clockSlot);

#ifdef VP890_FXS_SUPPORT
    /* Load the switching regulator parameters */
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_REGULATOR_PARAM_WRT,
        VP890_REGULATOR_PARAM_LEN, pDevObj->devProfileData.swParams);
    VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
        VP890_REGULATOR_PARAM_LEN);
#endif

    /* Initialize each channel */
    for (chan = 0; chan < maxChan; chan++) {
        pLineCtx = pDevCtx->pLineCtx[chan];

        /* Init only line that have Line Context associated with them */
        if (pLineCtx != VP_NULL) {
            Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
            status = VP_STATUS_INVALID_ARG;
            if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
                status = Vp890InitLine(pLineCtx, pFxoAcProfile, pFxoCfgProfile,
                    VP_NULL);
                delayInitDeviceCompete = TRUE;
#endif
            } else {
#ifdef VP890_FXS_SUPPORT
                status = Vp890InitLine(pLineCtx, pAcProfile, pDcProfile,
                    pRingProfile);
#endif
            }

            /* If any of the lines fail to init then the init device also fail */
            if (status != VP_STATUS_SUCCESS) {
                pDevObj->state &=
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return status;
            }
        }
    }

    status = VpImplementDefaultSettings(pDevCtx, VP_NULL);

    /*
     * This clears the Init Line Events and any other erroneous event that
     * may have been created due to initialization
     */
    Vp890FlushEvents(pDevCtx);

    if (delayInitDeviceCompete == FALSE) {
        VP_FXO_FUNC(VpDevCtxType, pDevCtx, ("No FXO Detected. Generating Init Complete Events"));
        pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
        pDevObj->state |= VP_DEV_INIT_CMP;
    } else {
        VP_FXO_FUNC(VpDevCtxType, pDevCtx, ("FXO Detected. Delaying Init Complete Events"));
    }

    pDevObj->state &= ~(VP_DEV_INIT_IN_PROGRESS);

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}  /* Vp890InitDevice() */


/*******************************************************************************
 * InitDeviceProfile()
 *  This function....
 *
 * Arguments:
 *  pDevObj         -
 *  pDevProfile     -
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
InitDeviceProfile(
    Vp890DeviceObjectType   *pDevObj,
    VpProfilePtrType        pDevProfile)
{
    VpProfilePtrType        pDevProf    = VP_PTABLE_NULL;

    /*
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */
    /* Check the legality of the AC profile */

    if (!Vp890IsProfileValid(VP_PROFILE_DEVICE,
            VP_CSLAC_DEV_PROF_TABLE_SIZE, pDevObj->profEntry.devProfEntry,
            pDevObj->devProfileTable.pDevProfileTable, pDevProfile, &pDevProf)) {
        return VP_STATUS_ERR_PROFILE;
    }

    if (pDevProf == VP_PTABLE_NULL) {
        VP_ERROR(Vp890DeviceObjectType, pDevObj, ("InitDeviceProfile() - NULL device profile not legal"));
        return VP_STATUS_ERR_PROFILE;
    }

    /* Save the device info into the device Obj*/
    pDevObj->devProfileData.profVersion = (uint8)(pDevProf[VP_PROFILE_VERSION]);

    VP_INFO(Vp890DeviceObjectType, pDevObj, ("Device Profile Version %d",
        pDevObj->devProfileData.profVersion));

    if (pDevObj->devProfileData.profVersion < VP890_DEV_PROF_MIN_VERSION) {
        return VP_STATUS_ERR_PROFILE;
    }

#ifdef VP890_FXS_SUPPORT
    if (pDevObj->devProfileData.profVersion >= VP890_DEV_PROF_W_TIMING) {
        VP_INFO(Vp890DeviceObjectType, pDevObj, ("Loading Timing Params - "));

        VpMemCpy(pDevObj->devProfileData.timingParams,
            &pDevProf[VP890_DEV_PROFILE_TIMING_DATA_START], VP890_REGULATOR_TIMING_LEN);
    } else {
        pDevObj->devProfileData.timingParams[0] = 0x74;
        pDevObj->devProfileData.timingParams[1] = 0x40;
        pDevObj->devProfileData.timingParams[2] = 0x98;
        pDevObj->devProfileData.timingParams[3] = 0x40;
        pDevObj->devProfileData.timingParams[4] = 0xC0;
        pDevObj->devProfileData.timingParams[5] = 0x40;
    }

    if (pDevProf[VP890_DEV_PROFILE_OPERATIONAL_CFG] & VP890_DEV_PROFILE_PK_PWR_MGMT) {
        pDevObj->devProfileData.peakManagement = TRUE;
    } else {
        pDevObj->devProfileData.peakManagement = FALSE;
    }

    if (pDevProf[VP890_DEV_PROFILE_OPERATIONAL_CFG] & VP890_DEV_PROFILE_LOW_VOLT_OVERRIDE) {
        pDevObj->devProfileData.lowVoltOverride = TRUE;
    } else {
        pDevObj->devProfileData.lowVoltOverride = FALSE;
    }

    if (pDevObj->devProfileData.profVersion >= VP890_DEV_PROF_W_TIMING_FR) {
        VpMemCpy(pDevObj->devProfileData.timingParamsFR,
            &pDevProf[VP890_DEV_PROFILE_TIMING_DATA_START + VP890_REGULATOR_TIMING_LEN],
            VP890_REGULATOR_TIMING_LEN);
    } else {
        Vp890CopyDefaultFRProfile(pDevObj);
    }

    VP_INFO(Vp890DeviceObjectType, pDevObj, ("Saved Timing Params - 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
        pDevObj->devProfileData.timingParams[0], pDevObj->devProfileData.timingParams[1],
        pDevObj->devProfileData.timingParams[2], pDevObj->devProfileData.timingParams[3],
        pDevObj->devProfileData.timingParams[4], pDevObj->devProfileData.timingParams[5]));
#endif

    pDevObj->devProfileData.pcmClkRate =
        (uint16)(((pDevProf[VP890_DEV_PROFILE_PCLK_MSB] << 8) & 0xFF00)
                | (pDevProf[VP890_DEV_PROFILE_PCLK_LSB] & 0x00FF));

    pDevObj->devProfileData.devCfg1 =
        (uint8)(pDevProf[VP890_DEV_PROFILE_DEVCFG1]);
    pDevObj->devProfileData.clockSlot =
        (uint8)(pDevProf[VP890_DEV_PROFILE_CLOCK_SLOT]);

    pDevObj->devProfileData.tickRate =
        (uint16)(((pDevProf[VP890_DEV_PROFILE_TICKRATE_MSB] << 8) & 0xFF00)
                | (pDevProf[VP890_DEV_PROFILE_TICKRATE_LSB] & 0x00FF));

#ifdef VP890_FXS_SUPPORT
    VpMemCpy(pDevObj->devProfileData.swParams,
        &pDevProf[VP890_DEV_PROFILE_SWITCHER_DATA0], VP890_REGULATOR_PARAM_LEN);

    if ((pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE]
       & VP890_VOLTAGE_MASK) < VP890_VOLTAGE_60V) {
        pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE]
            &= ~VP890_VOLTAGE_MASK;
        pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE]
            |= VP890_VOLTAGE_60V;
    }
#endif

    return VP_STATUS_SUCCESS;
}

/*******************************************************************************
 * InitDeviceMclk()
 *  This function attempts to setup and detect the 890 Master clock.
 *
 * Arguments:
 *  pDevObj         -
 *  deviceId        -
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static bool
InitDeviceMclk(
    Vp890DeviceObjectType   *pDevObj,
    VpDeviceIdType          deviceId)
{
    uint8 data[2], clkTestCount  = MAX_CFAIL_TEST;

     /*
     * Setup mclk. The MCLK mask set the mclk frequency, sets the mclk source
     * (the MCLK pin or the PCLK pin), and sets the interrupt pin output drive
     * mode (TTL or open collector)
     */
    data[0] = pDevObj->devProfileData.devCfg1;
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_MCLK_CNT_WRT, VP890_MCLK_CNT_LEN, data);

    /* mask all interrupts except clock fault */
    data[0] = 0x7F;
    data[1] = 0xFF;
    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_INT_MASK_WRT, VP890_INT_MASK_LEN, data);

    /*
     * Wait for the CFAIL bit to clear before proceding. If the CFAIL bit does
     * not clear after several trys, give up and return an error condition. Wait
     * between each read of the status register.
     */
    while (--clkTestCount > 0) {
        VpSysWait(CFAIL_TEST_INTERVAL);
        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_UL_SIGREG_RD, VP890_UL_SIGREG_LEN, data);

        if (!(data[0] & VP890_CFAIL_MASK)) {
            return FALSE;
        }
    }

    VP_ERROR(VpDeviceIdType, &deviceId, ("\nInitDeviceMclk FAILED"));
    return TRUE;
}

/*******************************************************************************
 * InitDevicePcnRcn()
 *  Read in the PCN so we will know what type of device specifically we
 *  are working with.  This affects the max number of lines supported by the
 *  device as well as the type of lines (and may affect the line init)
 *
 *  Note: EC value is not important for this command
 *
 * Arguments:
 *  pDevObj         -
 *  deviceId        -
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
InitDevicePcnRcn(
    Vp890DeviceObjectType   *pDevObj,
    VpDeviceIdType          deviceId)
{
    Vp890PcnType            devicePcn;
    bool                    runHvCheck = TRUE;

    VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_RCN_PCN_RD, VP890_RCN_PCN_LEN,
        pDevObj->staticInfo.rcnPcn);
    devicePcn = (Vp890PcnType)(pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION]);

    /* Simple check to see if we're even talking to the device */
    if (((pDevObj->staticInfo.rcnPcn[0] == 0x00)
      && (pDevObj->staticInfo.rcnPcn[1] == 0x00))
     || ((pDevObj->staticInfo.rcnPcn[0] == 0xFF)
      && (pDevObj->staticInfo.rcnPcn[1] == 0xFF))) {
        return VP_STATUS_FAILURE;
    }

    /* Disable support of non-Production silicon. */
    if (pDevObj->staticInfo.rcnPcn[VP890_RCN_LOCATION] < 2) {
        return VP_STATUS_FAILURE;
    }

    /*
     * Is the PCN one that we recognize as 890 (note: 89010 has same PCN as the
     * 89316, but the enumeration is different and invalid from the device)
     */
    if ((devicePcn >= VP890_LAST_PCN)
#ifdef VP890_FXO_SUPPORT
     || (devicePcn == VP890_DEV_PCN_89010)
#endif
    ) {
        VP_ERROR(Vp890DeviceObjectType, pDevObj, ("InitDevicePcnRcn() - unknown pcn 0x%02x", devicePcn));
        return VP_STATUS_FAILURE;
    }

    /* Check for FXO line Types */
    if (devicePcn == VP890_DEV_PCN_89316) {
        uint8 check89010[VP890_FUSE5_REG_LEN];
        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_FUSE5_REG_RD, VP890_FUSE5_REG_LEN,
            check89010);

       /*
        * Normally, the fuse register is a trim value of vref and never 1. It's
        * only 1 if the device is being "marked" as FXO only.
        */
        if (check89010[0] == 0x01) {
#ifdef VP890_FXO_SUPPORT
            pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION] =  VP890_DEV_PCN_89010;
            pDevObj->stateInt |= (VP890_LINE1_IS_FXO | VP890_IS_FXO_ONLY);
            pDevObj->staticInfo.maxChannels = 2;
            runHvCheck = FALSE;
#else
            return VP_STATUS_FAILURE;
#endif
        } else {
            pDevObj->stateInt |= VP890_LINE1_IS_FXO | VP890_WIDEBAND;
            pDevObj->staticInfo.maxChannels = 2;
        }
    } else {
        pDevObj->stateInt |= VP890_WIDEBAND | VP890_IS_SINGLE_CHANNEL;
        pDevObj->staticInfo.maxChannels = 1;
    }

#ifdef VP890_FXS_SUPPORT
    if (runHvCheck == TRUE) {
        uint8 checkHv[VP890_FUSE1_REG_LEN];
        uint8 ecTestValue = (VP890_EC_TEST_MODE_EN | VP890_EC_CH1);
        uint8 testReg1Data[VP890_TEST_REG1_LEN] = {VP890_TEST_REG1_FUSE_TEST};

        VpMpiCmdWrapper(deviceId, ecTestValue, VP890_TEST_REG1_WRT, VP890_TEST_REG1_LEN,
            testReg1Data);

        VpMpiCmdWrapper(deviceId, ecTestValue, VP890_FUSE1_REG_RD, VP890_FUSE1_REG_LEN,
            checkHv);

        /*
         * A non-blown fuse reads as '1'. So if it is blown (reads = '0') it is
         * marked as a HV device.
         */
        if (!(checkHv[0] & VP890_FUSE1_REG_ILAF_TRIM)) {
            pDevObj->stateInt |= VP890_IS_HIGH_VOLTAGE;
            if (pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION] == VP890_DEV_PCN_89116) {
                pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION] = VP890_DEV_PCN_89136;
            } else if (pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION] == VP890_DEV_PCN_89316) {
                pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION] = VP890_DEV_PCN_89336;
            }
        }
        checkHv[0] = 0x00;
        VpMpiCmdWrapper(deviceId, VP890_EC_CH1, VP890_TEST_REG1_WRT, VP890_TEST_REG1_LEN,
            checkHv);
    }
#endif

    /* Indicate that the device is detected */
    pDevObj->stateInt |= VP890_DEVICE_DETECTED;

    return VP_STATUS_SUCCESS;
}

/*******************************************************************************
 * Vp890InitLine()
 *  This function determines which type of line termination type is being
 *  initialized and calls the proper init line function.
 *
 * Arguments:
 *  pLineCtx        -
 *  pAcProfile      -   AC Profile Selector
 *  pDcOrFxoProfile -   DC of FXO Profile Selector
 *  pRingProfile    -   Ring Profile Selector
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
VpStatusType
Vp890InitLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pDcOrFxoProfile,
    VpProfilePtrType        pRingProfile)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpStatusType            status      = VP_STATUS_INVALID_ARG;

    uint8                   channelId   = pLineObj->channelId;
    uint8                   ecValMap[]  = {VP890_EC_CH1, VP890_EC_CH2};
    uint8                   ecVal       = ecValMap[channelId];
    uint8                   data;

    uint8 alwaysOn[VP890_CADENCE_TIMER_LEN] = {0x3F, 0xFF, 0x00, 0x00};

    /*
     * IO Direction and Control used to restore the device IO to the state
     * set prior to the channel Software Reset
     */
    uint8 ioDirection[VP890_IODIR_REG_LEN];
    uint8 ioData[VP890_IODATA_REG_LEN];
    uint8 swCal[VP890_BAT_CALIBRATION_LEN];

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+Vp890InitLine()"));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->status &= ~VP890_INIT_COMPLETE;
    pLineObj->ecVal = ecVal;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        VP_ERROR(VpLineCtxType, pLineCtx, ("-Vp890InitLine() DEV NOT INIT"));
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    switch (pDevObj->staticInfo.rcnPcn[VP890_PCN_LOCATION]) {
#ifdef VP890_FXS_SUPPORT
        case VP890_DEV_PCN_89116:   /**< FXS - Wideband */
            if ((channelId != 0) || (pLineObj->termType == VP_TERM_FXO_GENERIC)) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }
            /* Clear invalid links between device context to line context */
            pDevCtx->pLineCtx[1] = VP_NULL;
            break;
#endif
        case VP890_DEV_PCN_89316:   /* FXO/FXS-Tracker - Wideband */
            if (((channelId == 0) && (pLineObj->termType == VP_TERM_FXO_GENERIC))
             || ((channelId == 1) && (pLineObj->termType != VP_TERM_FXO_GENERIC))) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }
            break;

#ifdef VP890_FXO_SUPPORT
        case VP890_DEV_PCN_89010:   /* Single Channel FXO */
            if (pLineObj->termType != VP_TERM_FXO_GENERIC) {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }
            /* Clear invalid links between device context to line context */
            pDevCtx->pLineCtx[0] = VP_NULL;
            break;
#endif

        default:
            break;
    }

    pLineObj->status &= ~(VP890_BAD_LOOP_SUP);
    pLineObj->relayState = VP_RELAY_NORMAL;

    pLineObj->internalTestTermApplied = FALSE;

    /* Initialize cached transmit and receive gains for SetRelGain to 1.0. */
    pLineObj->gxBase = 0x4000;
    pLineObj->gxCidLevel = 0x4000;
    pLineObj->gxUserLevel = 0x4000;
    pLineObj->grBase = 0x4000;

    /* Force a line state check */
    pLineObj->lineState.condition = VP_CSLAC_STATUS_INVALID;

    /* Force a codec update */
    pLineObj->codec = VP_NUM_OPTION_CODEC_TYPE_IDS;

    pLineObj->calLineData.calState = VP890_CAL_EXIT;
    pLineObj->calLineData.calDone = FALSE;
    pLineObj->calLineData.dcFeedRef[0] = 0;
    pLineObj->calLineData.dcFeedRef[1] = 0;

    pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;

    /*
     * Read the IO direction and data for the device IO that will be affected
     * by a software reset. Also read the calibrated battery voltage.
     */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_RD, VP890_IODIR_REG_LEN,
        ioDirection);

    VpMpiCmdWrapper(deviceId, ecVal, VP890_IODATA_REG_RD, VP890_IODATA_REG_LEN,
        ioData);

    VpMpiCmdWrapper(deviceId,ecVal, VP890_BAT_CALIBRATION_RD, VP890_BAT_CALIBRATION_LEN,
        swCal);

    /* Software reset the channel */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_SW_RESET_WRT, VP890_SW_RESET_LEN,
        &data);
    VpSysWait(3);

    /*
     * Restore the IO direction and data for the device IO that will be affected
     * by a software reset. Also restore the calibrated battery voltage.
     */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_IODIR_REG_WRT, VP890_IODIR_REG_LEN,
        ioDirection);

    VpMpiCmdWrapper(deviceId, ecVal, VP890_IODATA_REG_WRT, VP890_IODATA_REG_LEN,
        ioData);

    VpMpiCmdWrapper(deviceId,ecVal, VP890_BAT_CALIBRATION_WRT, VP890_BAT_CALIBRATION_LEN,
        swCal);

    /* Operating Conditions - Remove all loopbacks, connect TX/RX PCM Hwy */
    data = VP890_NORMAL_OP_COND_MODE;
    VP_INFO(VpLineCtxType, pLineCtx, ("\n\r17. Writing 0x%02X to Operating Conditions",
        data));
    VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_COND_WRT, VP890_OP_COND_LEN, &data);
    pLineObj->opCond[0] = data;

    /* Disable the internal device cadencer .. done in the API */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_CADENCE_TIMER_WRT, VP890_CADENCE_TIMER_LEN,
        alwaysOn);

    /* Do line type specific stuff */
    switch(pLineObj->termType) {
#ifdef VP890_FXS_SUPPORT
        case VP_TERM_FXS_GENERIC:
        case VP_TERM_FXS_SPLITTER:
        case VP_TERM_FXS_ISOLATE:

#ifdef VP890_LP_SUPPORT
        case VP_TERM_FXS_LOW_PWR:
        case VP_TERM_FXS_SPLITTER_LP:
        case VP_TERM_FXS_ISOLATE_LP:
#endif
            status = InitFxsLine(pLineCtx, pAcProfile, pDcOrFxoProfile,
                pRingProfile);
            break;
#endif

#ifdef VP890_FXO_SUPPORT
        case VP_TERM_FXO_GENERIC:
            status = InitFxoLine(pLineCtx, pAcProfile, pDcOrFxoProfile);
            break;
#endif

        default:
            /* We should NEVER get here */
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890InitLine() - How did you get here? 0x%0x",
                pLineObj->termType));
            status = VP_STATUS_FAILURE;
            break;
    }

    if (status != VP_STATUS_SUCCESS) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp890InitLine() - unable to config line"));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return status;
    }
    status = VpImplementDefaultSettings(VP_NULL, pLineCtx);

    /*
     * Post the line init complete event if status is succesfull and FXS line
     * and not running from Init device
     */
    if (!(pDevObj->state & VP_DEV_INIT_IN_PROGRESS)) {
        if ((status == VP_STATUS_SUCCESS) && (!(pLineObj->status & VP890_IS_FXO))) {
            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("No FXO Detected - Channel %d. Generating Line Init Complete Event",
                channelId));
            pLineObj->lineEvents.response |= VP_LINE_EVID_LINE_INIT_CMP;
        } else {
            VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("FXO Detected - Channel %d. Delaying Line Init Complete Event",
                channelId));
        }
    }

#ifdef VP890_FXS_SUPPORT
    Vp890SetRelayState(pLineCtx, VP_RELAY_NORMAL);
#endif

#ifdef VP_CSLAC_SEQ_EN
    for (data = 0; data < VP890_INT_SEQ_LEN; data++) {
        pLineObj->intSequence[data] = 0x00;
    }
    /* These two are not used by FXO lines!!! */
    pLineObj->callerId.status = 0x00;
    pLineObj->suspendCid = FALSE;

    InitCadenceVars(pLineCtx);
#endif

    InitTimerVars(pLineCtx);

#ifdef VP890_INCLUDE_TESTLINE_CODE
    /* initialize the calibration coeffs */
    pDevObj->calOffsets[channelId].nullOffset = 0;
    pDevObj->calOffsets[channelId].vabOffset = 0;
    pDevObj->calOffsets[channelId].vahOffset = 0;
    pDevObj->calOffsets[channelId].vbhOffset = 0;
#endif /* VP890_INCLUDE_TESTLINE_CODE */

    if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
        /* Start the PLL Recovery workaround at next tick */
        pLineObj->lineTimers.timers.fxoTimer.pllRecovery = 1;
        pLineObj->pllRecoveryState = VP890_PLL_RECOVERY_ST_DISABLE;
#endif
    } else {
        pLineObj->status |= VP890_INIT_COMPLETE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("-Vp890InitLine()"));

    return VP_STATUS_SUCCESS;
} /* Vp890InitLine() */

#ifdef VP890_FXS_SUPPORT
/*******************************************************************************
 * InitFxsLine()
 *  This function initializes an FXS line of a device with the specified
 *  parameters and API default values. It is a "Line Reset".
 *
 *  pLineCtx        -
 *  pAcProfile      -   Pointer to AC coeffs or profile index for this line
 *  pDcOrFxoProfile -   Pointer to DC coeffs or profile index for this line
 *  pRingProfile    -   Pointer to Ringing coeffs or profile index for this line
 *  ecVal           -   Enable Channel Value associated with this line
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to by the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 ******************************************************************************/
static VpStatusType
InitFxsLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pDcProfile,
    VpProfilePtrType        pRingProfile)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    uint8                   ecVal       = pLineObj->ecVal;
    uint8                   data, rngParmCnt;

    uint8 converterCfg[VP890_CONV_CFG_LEN] = {VP890_METALLIC_AC_V};

    VpProfileDataType dcDefaultProf[] = {
        0x00, 0x01, 0x00, 0x08, 0x01, 0x03, /* Header Information */
        0xC6, 0x13, 0xC8, /* Feed Parameters */
        0x1B, 0x84, /* Loop Supervision Parameters */
        0x04  /* Min Floor Voltage 25V */
    };

    VpProfileDataType ringDefaultProf[] = {
        /* Sine, 20 Hz, 1.41 CF, 70.00 Vpk, 0.00 Bias */
        /* Ringing Profile */
        0x00, 0x04, 0x00, 0x12, 0x01, 0x0C,
        0xD2, 0x00, 0x00, 0x00, 0x00, 0x36, 0x39, 0xCE, 0x00, 0x00, 0x00, 0x00,
        0xB0, 0x02,  /* Ring Trip Threshold (24mA) and Current Limit (54mA)*/
        0x0D, /* Max Supply Ringing Voltage 70V */
        0x02  /* Ringing Tracking Mode */
    };

    VpProfilePtrType pDcConfigProf =
        (pDcProfile == VP_NULL) ? dcDefaultProf : pDcProfile;

    VpProfilePtrType pRingConfigProf =
        (pRingProfile == VP_NULL) ? ringDefaultProf : pRingProfile;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+InitFxsLine()"));

    /* Set the cached ICR values to device reset conditions. */
    VpMemSet(pLineObj->icr1Values, 0, VP890_ICR1_LEN);
    VpMemSet(pLineObj->icr2Values, 0, VP890_ICR2_LEN);
    VpMemSet(pLineObj->icr3Values, 0, VP890_ICR3_LEN);
    VpMemSet(pLineObj->icr4Values, 0, VP890_ICR4_LEN);

    pLineObj->dcCalValues[0] = 0x00;
    pLineObj->dcCalValues[1] = 0x00;

    /* Load internal switching regulator parameters from profile */
    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_TIMING_WRT,
        VP890_REGULATOR_TIMING_LEN, pDevObj->devProfileData.timingParams);

    /* Disable Auto System State Control, and Auto Clock Fault */
    data = VP890_AUTO_SSC_DIS;
    VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT, VP890_SS_CONFIG_LEN,
        &data);

    /* Fixed target supported is 48V, enable longitudinal clamps */
    pLineObj->icr3Values[VP890_ICR3_LONG_FIXED_INDEX] |= VP890_ICR3_LONG_FIXED;
    pLineObj->icr3Values[VP890_ICR3_LONG_FIXED_INDEX+1] &= ~VP890_ICR3_LONG_FIXED;
    pLineObj->icr3Values[VP890_ICR3_LONG_UNCLAMP_INDEX] &= ~VP890_ICR3_LONG_UNCLAMP;

    /* Eliminate use of 50V clamp for all conditions */
    pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX] |=
        (VP890_ICR2_SWY_LIM_CTRL1 | VP890_ICR2_SWY_LIM_CTRL);
    pLineObj->icr2Values[VP890_ICR2_SWY_CTRL_INDEX+1] |=
        (VP890_ICR2_SWY_LIM_CTRL1);

#ifdef VP890_LP_SUPPORT
    if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR) ||
        (pLineObj->termType == VP_TERM_FXS_ISOLATE_LP) ||
        (pLineObj->termType == VP_TERM_FXS_SPLITTER_LP)) {
        Vp890SetLPRegisters(pDevObj, pLineObj, TRUE);
        pLineObj->status |= VP890_LOW_POWER_EN;
    }
#endif

    /* Force Vref control and enable it. */
    pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX] |=  VP890_ICR3_VREF_CTRL;
    pLineObj->icr3Values[VP890_ICR3_LINE_CTRL_INDEX+1] |=  VP890_ICR3_VREF_CTRL;

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR3_WRT, VP890_ICR3_LEN, pLineObj->icr3Values);
    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR2_WRT, VP890_ICR2_LEN, pLineObj->icr2Values);

    /*
     * Wait at least 5ms before turning the switchers on for Vref to
     * stabilize. We'll wait 10ms to be safe.
     */
    VpSysWait(80);

    /*
     * Turn on the switching regulator. Disable Overvoltage Protection for High
     * Voltage devices that are configured for HV application, because the
     * threshold is ~100V, exceeded potentially by ringing signals.
     */
    if ((pDevObj->stateInt & VP890_IS_HIGH_VOLTAGE) &&
        (pDevObj->devProfileData.lowVoltOverride == FALSE)) {
        data = VP890_SWY_LP;
    } else {
        data = VP890_SWY_LP | VP890_SWOVP_EN;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_CTRL_WRT,
        VP890_REGULATOR_CTRL_LEN, &data);

    /* Initialize default values for Ringing to NULL */
    for (rngParmCnt = 0; rngParmCnt < VP890_RINGER_PARAMS_LEN; rngParmCnt++) {
        pLineObj->ringingParams[rngParmCnt] = 0;
    }

    /* Set timer type in union to FXS type */
    pLineObj->lineTimers.type = VP_CSLAC_FXS_TIMER;

    /* Inititialize API line state variables */
    pLineObj->lineState.currentState = VP_LINE_DISCONNECT;
    pLineObj->lineState.previous = VP_LINE_DISCONNECT;

    pLineObj->calLineData.dcFeedPr[0] = 0x13;
    pLineObj->calLineData.dcFeedPr[1] = 0xC8;
    pLineObj->calLineData.dcFeed[0] = 0x13;
    pLineObj->calLineData.dcFeed[1] = 0xC8;
    pLineObj->calLineData.dcFeedRef[0] = 0x13;
    pLineObj->calLineData.dcFeedRef[1] = 0xC8;

    pLineObj->calLineData.calDone = FALSE;

    pLineObj->pRingingCadence = VP_PTABLE_NULL;
    pLineObj->pCidProfileType1 = VP_PTABLE_NULL;

    pLineObj->leakyLineCnt = 0;    /* Used only for LP Mode */

    /* Force a line state check and update hook information */
    pLineObj->dpStruct.hookSt = FALSE;
    pLineObj->dpStruct2.hookSt = FALSE;

    VpInitDP(&pLineObj->dpStruct);
    VpInitDP(&pLineObj->dpStruct2);

    /* Complete all other non device senstive items */
    /* Set to Disconnect */
    Vp890SetFxsLineState(pLineCtx, VP_LINE_DISCONNECT);
    pLineObj->lineState.usrCurrent = VP_LINE_DISCONNECT;

    /*
     * Set the pcm buffer update rate based on the tickrate.
     * Note that this ignores CODEC Mode because we know it's init to A-Law
     * (non-Wideband)
     */
    if(pDevObj->devProfileData.tickRate <=160) {
        converterCfg[0] |= VP890_CC_8KHZ_RATE;
        pDevObj->txBufferDataRate = VP890_CC_8KHZ_RATE;

    } else if(pDevObj->devProfileData.tickRate <=320){
        converterCfg[0] |= VP890_CC_4KHZ_RATE;
        pDevObj->txBufferDataRate = VP890_CC_4KHZ_RATE;

    } else if(pDevObj->devProfileData.tickRate <=640){
        converterCfg[0] |= VP890_CC_2KHZ_RATE;
        pDevObj->txBufferDataRate = VP890_CC_2KHZ_RATE;

    } else if(pDevObj->devProfileData.tickRate <=1280){
        converterCfg[0] |= VP890_CC_1KHZ_RATE;
        pDevObj->txBufferDataRate = VP890_CC_1KHZ_RATE;
    } else {
        converterCfg[0] |= VP890_CC_500HZ_RATE;
        pDevObj->txBufferDataRate = VP890_CC_500HZ_RATE;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_CONV_CFG_WRT, VP890_CONV_CFG_LEN,
        converterCfg);

    pDevObj->devMode[0] |= VP890_DEV_MODE_TDIM;
    VpMpiCmdWrapper(deviceId, ecVal, VP890_DEV_MODE_WRT, VP890_DEV_MODE_LEN,
        pDevObj->devMode);

    return Vp890ConfigLine(pLineCtx, pAcProfile, pDcConfigProf, pRingConfigProf);
} /* InitFxsLine */
#endif

#ifdef VP890_FXO_SUPPORT
/*******************************************************************************
 * InitFxoLine()
 * This function ...
 *
 * Arguments:
 *
 * Preconditions:
 *
 * Postconditions:
 ******************************************************************************/
static VpStatusType
InitFxoLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pFxoProfile)
{
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    uint8                   ecVal       = pLineObj->ecVal;
    uint8                   data;
    uint8                   loopSup[VP890_LOOP_SUP_LEN];

    pLineObj->lineState.currentState    = VP_LINE_FXO_LOOP_OPEN;
    pLineObj->lineState.previous        = VP_LINE_FXO_LOOP_OPEN;
    pLineObj->lineTimers.type           = VP_CSLAC_FXO_TIMER;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("+InitFxoLine()"));

    /* Disable auto system state control and auto LDN switching */
    data = VP890_AUTO_SSC_DIS | VP890_LDE_DIS | VP890_APOL_8KHZ;
    VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT, VP890_SS_CONFIG_LEN, &data);

    /* Use the device amplitude+frequency detector.  */
    VpMemCpy(loopSup, pLineObj->loopSup, VP890_LOOP_SUP_LEN);
    loopSup[0] |= VP890_RDSEL_MASK;
    VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT, VP890_LOOP_SUP_LEN, loopSup);
    VpMemCpy(pLineObj->loopSup, loopSup, VP890_LOOP_SUP_LEN);

    pLineObj->status |= VP890_IS_FXO;

    pLineObj->digitGenStruct.dtmfOnTime             = VP_FXO_DTMF_ON_DEFAULT;
    pLineObj->digitGenStruct.dtmfOffTime            = VP_FXO_DTMF_OFF_DEFAULT;
    pLineObj->digitGenStruct.breakTime              = VP_FXO_PULSE_BREAK_DEFAULT;
    pLineObj->digitGenStruct.makeTime               = VP_FXO_PULSE_MAKE_DEFAULT;
    pLineObj->digitGenStruct.flashTime              = VP_FXO_FLASH_HOOK_DEFAULT;
    pLineObj->digitGenStruct.dpInterDigitTime       = VP_FXO_INTERDIG_DEFAULT;
    pLineObj->digitGenStruct.dtmfHighFreqLevel[0]   = 0x1C;
    pLineObj->digitGenStruct.dtmfHighFreqLevel[1]   = 0x32;
    pLineObj->digitGenStruct.dtmfLowFreqLevel[0]    = 0x1C;
    pLineObj->digitGenStruct.dtmfLowFreqLevel[1]    = 0x32;

    pLineObj->lowVoltageDetection.enabled = FALSE;

    /* Activate Codec and enable Supervision */
    Vp890SetFxoLineState(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
    pLineObj->lineState.usrCurrent = VP_LINE_FXO_LOOP_OPEN;

    /* Set the line condition to DISC so that once we see feed, the callerID
     * correction calculation will run */
    pLineObj->lineState.condition |= (VP_CSLAC_DISC | VP_CSLAC_RAW_DISC);

    return Vp890ConfigLine(pLineCtx, pAcProfile, pFxoProfile, VP_PTABLE_NULL);
} /* InitFxoLine() */
#endif

/*******************************************************************************
 * Vp890ConfigLine()
 *  This function reloads a line of a device with the specified parameters.
 *
 * Arguments:
 *  pAcProfile      - Pointer to AC coefficient data or profile index to be
 *                    applied to this line.
 *  pDcOrFxoProfile - Pointer to DC Feed (FXS) or Cfg (FX0) profile or profile
 *                    index to be applied to this line.
 *  pRingProfile    - Pointer to Ringing profile or profile index to apply to
 *                    this line
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to be the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 ******************************************************************************/
VpStatusType
Vp890ConfigLine(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pAcProfile,
    VpProfilePtrType        pDcOrFxoProfile,
    VpProfilePtrType        pRingProfile)
{
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx        = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj        = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId        = pDevObj->deviceId;

    VpProfileDataType       *pMpiData;
    VpProfilePtrType        pAcProf         = VP_PTABLE_NULL;
    VpProfilePtrType        pDcFxoCfgProf   = VP_PTABLE_NULL;

#ifdef VP890_FXS_SUPPORT
    VpProfilePtrType        pRingProf       = VP_PTABLE_NULL;
    uint8                   sysStateConfig[VP890_SS_CONFIG_LEN];
#endif

    uint8                   ecVal           = pLineObj->ecVal;

    uint8                   data;
    uint8                   gainCSD[VP890_GR_GAIN_LEN];
    uint8                   loopSupervision[VP890_LOOP_SUP_LEN];
    uint8                   profileIndex;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /* Check the legality of the AC profile */
    if (!Vp890IsProfileValid(VP_PROFILE_AC,
            VP_CSLAC_AC_PROF_TABLE_SIZE, pDevObj->profEntry.acProfEntry,
            pDevObj->devProfileTable.pAcProfileTable, pAcProfile, &pAcProf)) {

        return VP_STATUS_ERR_PROFILE;
    }

    if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
        /* Check the legality of the FXO profile */
        if (!Vp890IsProfileValid(VP_PROFILE_FXO_CONFIG,
                VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE,
                pDevObj->profEntry.fxoConfigProfEntry,
                pDevObj->devProfileTable.pFxoConfigProfileTable,
                pDcOrFxoProfile, &pDcFxoCfgProf)) {

            return VP_STATUS_ERR_PROFILE;
        }
#endif
    } else {
#ifdef VP890_FXS_SUPPORT
        /* Check the legality of the DC profile */
        if (!Vp890IsProfileValid(VP_PROFILE_DC, VP_CSLAC_DC_PROF_TABLE_SIZE,
                pDevObj->profEntry.dcProfEntry,
                pDevObj->devProfileTable.pDcProfileTable,
                pDcOrFxoProfile, &pDcFxoCfgProf)) {

            return VP_STATUS_ERR_PROFILE;
        }
#endif
    }

#ifdef VP890_FXS_SUPPORT
    /* Check the legality of the Ringing profile */
    if (!Vp890IsProfileValid(VP_PROFILE_RING, VP_CSLAC_RINGING_PROF_TABLE_SIZE,
            pDevObj->profEntry.ringingProfEntry,
            pDevObj->devProfileTable.pRingingProfileTable, pRingProfile,
            &pRingProf)) {

        return VP_STATUS_ERR_PROFILE;
    }
#endif

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Load AC Coefficients */
    if (pAcProf != VP_PTABLE_NULL) {
        profileIndex = VP_PROFILE_MPI_LEN + 1;
        pMpiData = (VpProfileDataType *)(&pAcProfile[profileIndex]);
        VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD, pAcProfile[VP_PROFILE_MPI_LEN],
            pMpiData);

        /* Operating Functions - Use loaded coefficients */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN, &data);
        data |= VP890_ENABLE_LOADED_COEFFICIENTS;
        VP_INFO(VpLineCtxType, pLineCtx, ("\n\r7. Writing 0x%02X to Operating Functions",
            data));

        VpMpiCmdWrapper(deviceId, ecVal, VP890_OP_FUNC_WRT,VP890_OP_FUNC_LEN, &data);

        /* Update cached transmit and receive gains for SetRelGain, and reset'
         * the levels from VpSetRelGain and caller ID correction */
        VpMpiCmdWrapper(deviceId, ecVal, VP890_GX_GAIN_RD, VP890_GX_GAIN_LEN, gainCSD);
        pLineObj->gxBase = 0x4000 + VpConvertCsd2Fixed(gainCSD);
        pLineObj->gxCidLevel = 0x4000;
        pLineObj->gxUserLevel = 0x4000;
        pLineObj->absGxGain = VP_ABS_GAIN_UNKNOWN;

        VpMpiCmdWrapper(deviceId, ecVal, VP890_GR_GAIN_RD, VP890_GR_GAIN_LEN, gainCSD);
        pLineObj->grBase = VpConvertCsd2Fixed(gainCSD);
        pLineObj->grUserLevel = 0x4000;
        pLineObj->absGrGain = VP_ABS_GAIN_UNKNOWN;

        if (pLineObj->status & VP890_IS_FXO) {
            /* For FXO, cache the DTG value so that we can restore it after
             * changing it for CID correction.  */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN,
                &pLineObj->userDtg);
            pLineObj->userDtg &= VP890_DTG_MASK;

            /* If the FXO is in an onhook state, we should be applying callerID
             * corrections.  Loading an AC profile blows away some of the
             * settings used for this, so calling SetLineState will reapply
             * the correction settings. */
            if (pLineObj->lineState.usrCurrent == VP_LINE_FXO_LOOP_OPEN
                || pLineObj->lineState.usrCurrent == VP_LINE_FXO_OHT)
            {
                Vp890SetLineState(pLineCtx, pLineObj->lineState.usrCurrent);
            }
        }
    }

    if (pLineObj->status & VP890_IS_FXO) {
#ifdef VP890_FXO_SUPPORT
        uint8 loopSupParam;

        /* Configure an FXO line type */
        if (pDcFxoCfgProf != VP_PTABLE_NULL) {
            VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_RD, VP890_LOOP_SUP_LEN,
                loopSupervision);
            /* Cache the loop supervision register */
            VpMemCpy(pLineObj->loopSup, loopSupervision, VP890_LOOP_SUP_LEN);

            profileIndex = VP_FXO_DIALING_PROFILE_DISC_VOLTAGE_MIN;
            loopSupParam = (pDcFxoCfgProf[profileIndex] << 4);
            loopSupParam &= VP890_LDN_DISC_THRESH_MASK;
            loopSupervision[1] &= ~(VP890_LDN_DISC_THRESH_MASK);
            loopSupervision[1] |= loopSupParam;

            /* If the lowest disconnect threshold is selected, enable the low
             * voltage disconnect/LIU distinction workaround.  This is needed
             * because when the device is set for 2.56V disconnect threshold,
             * it will actually report disconnect at significantly higher
             * voltages when it should report LIU instead. */
            if (loopSupParam == 0) {
                pLineObj->lowVoltageDetection.enabled = TRUE;
                pLineObj->lowVoltageDetection.numDisc = 0;
                pLineObj->lowVoltageDetection.numNotDisc = 0;
                /* If onhook at this point, start the measurement timer */
                if (pLineObj->lineState.currentState == VP_LINE_FXO_LOOP_OPEN
                    || pLineObj->lineState.currentState == VP_LINE_FXO_OHT)
                {
                    pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer =
                        MS_TO_TICKRATE(VP890_FXO_LOW_VOLTAGE_TIMER_LEN, pDevObj->devProfileData.tickRate);
                    VP_FXO_FUNC(VpLineCtxType, pLineCtx, ("Enabling low voltage timer in cfgl %d\n",
                        pLineObj->lineTimers.timers.fxoTimer.lowVoltageTimer));
                }
            } else {
                pLineObj->lowVoltageDetection.enabled = FALSE;
                pLineObj->lineTimers.timers.fxoTimer.currentMonitorTimer = 0;
            }

            profileIndex = VP_FXO_DIALING_PROFILE_RING_VOLTAGE_MIN;
            loopSupParam = (pDcFxoCfgProf[profileIndex] >> 2);
            loopSupParam &= VP890_TRAMP_MSB_MASK;
            loopSupervision[0] &= ~(VP890_TRAMP_MSB_MASK);
            loopSupervision[0] |= loopSupParam;
            loopSupParam = (pDcFxoCfgProf[profileIndex] << 6);
            loopSupParam &= VP890_TRAMP_LSB_MASK;
            loopSupervision[1] &= ~(VP890_TRAMP_LSB_MASK);
            loopSupervision[1] |= loopSupParam;

            profileIndex = VP_FXO_DIALING_PROFILE_LIU_THRESHOLD_MIN;
            loopSupParam = (pDcFxoCfgProf[profileIndex] << 1);
            loopSupParam &= VP890_LIU_THRESH_MASK;
            loopSupervision[0] &= ~(VP890_LIU_THRESH_MASK);
            loopSupervision[0] |= loopSupParam;

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MIN;
            loopSupervision[2] = pDcFxoCfgProf[profileIndex];
            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX;
            loopSupervision[3] = pDcFxoCfgProf[profileIndex];

            /* Widen the Ring detect period window by 10% due to error in the
             * period detector.  "+ 9" makes the operation round up */
            loopSupervision[2] -= (loopSupervision[2] + 9) / 10;
            loopSupervision[3] += (loopSupervision[3] + 9) / 10;

            /* Cache the Min/Max Ringing Detect Periods. */
            pLineObj->ringDetMin = loopSupervision[2];
            pLineObj->ringDetMax = loopSupervision[3];

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_MSB;
            pLineObj->digitGenStruct.dtmfOnTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_LSB;
            pLineObj->digitGenStruct.dtmfOnTime |=  pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_MSB;
            pLineObj->digitGenStruct.dtmfOffTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_LSB;
            pLineObj->digitGenStruct.dtmfOffTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_BREAK;
            pLineObj->digitGenStruct.breakTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_MAKE;
            pLineObj->digitGenStruct.makeTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_MSB;
            pLineObj->digitGenStruct.flashTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_LSB;
            pLineObj->digitGenStruct.flashTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_MSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_LSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                pDcFxoCfgProf[profileIndex];

            profileIndex = VP_PROFILE_VERSION;
            if (pDcFxoCfgProf[profileIndex] >= VP_CSLAC_FXO_VERSION_DTMF_LEVEL) {
                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_MSB;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[0] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_LSB;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[1] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_MSB;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[0] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_LSB;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[1] =
                    pDcFxoCfgProf[profileIndex];
            } else {
                pLineObj->digitGenStruct.dtmfHighFreqLevel[0] = 0x1C;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[1] = 0x32;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[0] = 0x1C;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[1] = 0x32;
            }

            profileIndex = VP_PROFILE_VERSION;
            if (pDcFxoCfgProf[profileIndex] >= VP_CSLAC_FXO_VERSION_POH) {
                profileIndex = VP_FXO_DIALING_PROFILE_POH_INT_TIME;
                pLineObj->dPoh = pDcFxoCfgProf[profileIndex];
                loopSupParam = (pDcFxoCfgProf[profileIndex] << 2);
                loopSupParam &= VP890_POH_INT_TIME_MASK;
                loopSupervision[1] &= ~(VP890_POH_INT_TIME_MASK);
                loopSupervision[1] |= loopSupParam;

                profileIndex = VP_FXO_DIALING_PROFILE_POH_THRESHOLD;
                loopSupParam = (pDcFxoCfgProf[profileIndex] << 4);
                loopSupParam &= VP890_POH_THRESH_MASK;
                loopSupervision[0] &= ~(VP890_POH_THRESH_MASK);
                loopSupervision[0] |= loopSupParam;
            } else {
                /* Default to 128ms integrate time and 7.284mA */
                loopSupervision[1] &= ~(VP890_POH_INT_TIME_MASK);
                loopSupervision[1] |= 0x08;  /* DPOH = 10b = 128ms */
                pLineObj->dPoh = 0x02;
                loopSupervision[0] &= ~(VP890_POH_THRESH_MASK);
                loopSupervision[0] |= 0x70;  /* TPOH = 111b = 7.284mA */
            }

            VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT, VP890_LOOP_SUP_LEN, loopSupervision);
            VpMemCpy(pLineObj->loopSup, loopSupervision, VP890_LOOP_SUP_LEN);

            /* Treat configline as a state change for ignoring event detectors
             * if the FxoCfg profile was changed */
            pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 0;
        }

        /* Cache this so we don't have to read it all the time */
        VpMemCpy(loopSupervision, pLineObj->loopSup, VP890_LOOP_SUP_LEN);
        pLineObj->lineTimers.timers.fxoTimer.maxPeriod = loopSupervision[3];
#endif
    } else {
#ifdef VP890_FXS_SUPPORT
        /* Configure an FXS line type */

        /* Set Loop Supervision and DC Feed */
        if (pDcFxoCfgProf != VP_PTABLE_NULL) {
            Vp890ProcessDcProfile(pLineCtx, pDcFxoCfgProf, pRingProf);
        }

        /* Ringing changed if profile passed */
        if (pRingProf != VP_PTABLE_NULL) {
            uint8 tempRingPr[255];
            int16 biasErr;

            /*
             * Clear flags to indicate generators are not programmed to user
             * specified Ringing values.
             */
            pLineObj->status &= ~(VP890_RING_GEN_NORM | VP890_RING_GEN_REV);

            /*
             * Ringing Profile May affect the system state register, so read
             * what it is before the profile, and set it back to all values
             * except what can change in the profile
             */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_RD,
                VP890_SS_CONFIG_LEN, sysStateConfig);

            profileIndex = VP_PROFILE_MPI_LEN + 1;
            pMpiData = (VpProfileDataType *)&pRingProf[profileIndex];

            VpMemCpy(tempRingPr, pMpiData, pRingProf[VP_PROFILE_MPI_LEN]);

            biasErr = (int16)((((uint16)(tempRingPr[2]) << 8) & 0xFF00) +
                ((uint16)(tempRingPr[3]) & 0x00FF));

            /* Apply the offset calibration to the BIAS */
            if ((pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) == 0x00) {
                /* Normal polarity */
                biasErr -= ((pDevObj->vp890SysCalData.sigGenAError[pLineObj->channelId][0] -
                    pDevObj->vp890SysCalData.vocOffset[pLineObj->channelId][VP890_NORM_POLARITY]) *
                    16 / 10);
            } else {
                /* Reverse polarity */
                biasErr += ((pDevObj->vp890SysCalData.sigGenAError[pLineObj->channelId][0] -
                    pDevObj->vp890SysCalData.vocOffset[pLineObj->channelId][VP890_REV_POLARITY]) *
                    16 / 10);
            }
            tempRingPr[2] = (uint8)((biasErr >> 8) & 0x00FF);
            tempRingPr[3] = (uint8)(biasErr & 0x00FF);

            VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD, pRingProf[VP_PROFILE_MPI_LEN], tempRingPr);

            VpMemCpy(pLineObj->ringingParams, &pRingProf[profileIndex + 1],
                VP890_RINGER_PARAMS_LEN);

            if (pRingProf[VP_PROFILE_VERSION] == VP_CSLAC_RING_PROFILE_VERSION_890) {
                VpMemCpy(loopSupervision, pLineObj->loopSup, VP890_LOOP_SUP_LEN);

                pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE]
                    &= ~VP890_SWREG_RING_V_MASK;
                /* Add 1 (5V) to the ringing amplitude to provide extra room */
                pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE]
                    |= (pRingProf[VP_RING_PROFILE_MAX_RING_V] + 1);

                /* Set for Fixed or Tracking Mode */
                pDevObj->devProfileData.swParams[VP890_SWREG_TRACKING_BYTE]
                    &= ~VP890_SWREG_TRACKING_MASK;
                pDevObj->devProfileData.swParams[VP890_SWREG_TRACKING_BYTE]
                    |= (pRingProf[VP_RING_PROFILE_TRACK_MODE]);

                /* Reload the same floor voltage, in case of low power */
                pDevObj->swParamsCache[VP890_SWREG_TRACKING_BYTE] =
                    pDevObj->devProfileData.swParams[VP890_SWREG_TRACKING_BYTE];
                pDevObj->swParamsCache[VP890_SWREG_RING_V_BYTE] =
                    pDevObj->devProfileData.swParams[VP890_SWREG_RING_V_BYTE];

                loopSupervision[VP890_LOOP_SUP_RT_MODE_BYTE]
                    = pRingProf[VP_RING_PROFILE_RING_TRIP]; /* Whole byte */
                loopSupervision[VP890_LOOP_SUP_ILR_BYTE]
                    &= ~(VP890_RING_I_LIMIT_MASK);
                loopSupervision[VP890_LOOP_SUP_ILR_BYTE]
                    |= pRingProf[VP_RING_PROFILE_CURRENT_LIM];

                VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT, VP890_LOOP_SUP_LEN,
                    loopSupervision);
                VpMemCpy(pLineObj->loopSup, loopSupervision, VP890_LOOP_SUP_LEN);
                VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
                    VP890_REGULATOR_PARAM_LEN, pDevObj->swParamsCache);
            }

            /* Nothing in this register should be allowed to change, but the
             * Ringing profile may have changed this value to be compatible
             * with other device profiles. So correct it. */
            VpMpiCmdWrapper(deviceId, ecVal, VP890_SS_CONFIG_WRT, VP890_SS_CONFIG_LEN,
                sysStateConfig);
        }
        /* Update the device per calibration values if available */
        if (pDevObj->stateInt & VP890_SYS_CAL_COMPLETE) {
            Vp890UpdateCalValue(pLineCtx);
        }
#endif
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890ConfigLine() */

#ifdef VP890_FXS_SUPPORT
/*******************************************************************************
 * Vp890ProcessDcProfile()
 *  This function processes the FXS DC profile with workaround and modifications
 * for calibration.
 *
 * Arguments:
 *  pDcOrFxoProfile - Pointer to DC Feed (FXS) or Cfg (FX0) profile or profile
 *                    index to be applied to this line.
 *
 * Preconditions:
 *  Called from Vp890ConfigLine only.
 *
 * Postconditions:
 *  DC Profile is processed. Affects DC Feed, Loop Supervision, and depending
 * on Profile version affects Battery setting.
 ******************************************************************************/
VpStatusType
Vp890ProcessDcProfile(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pDcProf,
    VpProfilePtrType        pRingProf)
{
    Vp890LineObjectType     *pLineObj       = pLineCtx->pLineObj;
    VpDevCtxType            *pDevCtx        = pLineCtx->pDevCtx;
    Vp890DeviceObjectType   *pDevObj        = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId        = pDevObj->deviceId;
    uint8                   ecVal           = pLineObj->ecVal;
    uint8                   icr5Speedup[VP890_ICR5_LEN];
    uint8                   loopSupervision[VP890_LOOP_SUP_LEN];

    uint8                   profileIndex    = VP_PROFILE_MPI_LEN + 1;
    VpProfileDataType       *pMpiData =
        (VpProfileDataType *)&pDcProf[profileIndex];

    /* Write the Profile Data, dc feed and all. Also set the reference */
    VpMpiCmdWrapper(deviceId, ecVal, NOOP_CMD, pDcProf[VP_PROFILE_MPI_LEN], pMpiData);

    if (pDcProf[VP_PROFILE_VERSION] == 0) {
        pLineObj->calLineData.dcFeedRef[0] = pDcProf[VP890_VOC_PROFILE_POSITION_V0];
        pLineObj->calLineData.dcFeedRef[1] = pDcProf[VP890_VOC_PROFILE_POSITION_V0+1];
    } else {
        /* Profile V1 & V2 */
        pLineObj->calLineData.dcFeedRef[0] = pDcProf[VP890_VOC_PROFILE_POSITION_V1];
        pLineObj->calLineData.dcFeedRef[1] = pDcProf[VP890_VOC_PROFILE_POSITION_V1+1];
    }

    if (pDcProf[VP_PROFILE_VERSION] >= 2) {
        /* This profile contains a hook hysteresis value */
        pLineObj->hookHysteresis = pDcProf[VP890_HOOK_HYST_POSITION];
    } else {
        pLineObj->hookHysteresis = 0;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR5_RD, VP890_ICR5_LEN, icr5Speedup);
    icr5Speedup[VP890_ICR5_FEED_HOLD_INDEX] &= ~VP890_ICR5_FEED_HOLD_MASK;

    /* Device value is x + 18mA, so threshold is > 35mA */
    if ((pLineObj->calLineData.dcFeedRef[VP890_ILA_INDEX] & VP890_ILA_MASK) > 17) {
        icr5Speedup[VP890_ICR5_FEED_HOLD_INDEX] |= 0xF0;
    } else {
        icr5Speedup[VP890_ICR5_FEED_HOLD_INDEX] |= 0xA0;
    }

    VpMpiCmdWrapper(deviceId, ecVal, VP890_ICR5_WRT, VP890_ICR5_LEN, icr5Speedup);

    /*
     * Update the line object and device (dc feed register) for calibrated
     * values if possible.
     */
    if (!(pDevObj->stateInt & VP890_SYS_CAL_COMPLETE)) {
        /* Calibration not previously run, no values provided. Max out VAS */
        VpCSLACSetVas(pLineObj->calLineData.dcFeedRef, VP890_VAS_MAX);
        VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT, VP890_DC_FEED_LEN,
            pLineObj->calLineData.dcFeedRef);
    }

    if (pDcProf[VP_PROFILE_VERSION] >= VP_CSLAC_DC_PROFILE_VERSION_890) {
        pDevObj->devProfileData.swParams[VP890_SWREG_FLOOR_V_BYTE]
            &= ~VP890_SWREG_FLOOR_V_MASK;
        pDevObj->devProfileData.swParams[VP890_SWREG_FLOOR_V_BYTE]
            |= pDcProf[VP_DC_PROFILE_FLOOR_V];

        VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_RD, VP890_LOOP_SUP_LEN,
            loopSupervision);
        /* Cache the loop supervision register */
        VpMemCpy(pLineObj->loopSup, loopSupervision, VP890_LOOP_SUP_LEN);
        loopSupervision[VP890_LOOP_SUP_THRESH_BYTE]
            &= ~(VP890_GKEY_THRESH_MASK | VP890_SWHOOK_THRESH_MASK);
        loopSupervision[VP890_LOOP_SUP_THRESH_BYTE]
            |= pDcProf[VP_DC_PROFILE_THRESHOLDS];
        loopSupervision[VP890_LOOP_SUP_DEBOUNCE_BYTE]
            &= (unsigned char)(~(VP890_GKEY_DEBOUNCE_MASK | VP890_SWHOOK_DEBOUNCE_MASK));
        loopSupervision[VP890_LOOP_SUP_DEBOUNCE_BYTE]
            |= pDcProf[VP_DC_PROFILE_DEBOUNCES];

        /* If off-hook -> apply the hysteresis */
        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK) == VP_CSLAC_HOOK) {
            if ((loopSupervision[VP890_LOOP_SUP_THRESH_BYTE] & 0x07)
                >= pLineObj->hookHysteresis) {
                loopSupervision[VP890_LOOP_SUP_THRESH_BYTE] -= pLineObj->hookHysteresis;
            } else {
                loopSupervision[VP890_LOOP_SUP_THRESH_BYTE] &= 0xF8;
            }
        }

        VpMpiCmdWrapper(deviceId, ecVal, VP890_LOOP_SUP_WRT, VP890_LOOP_SUP_LEN, loopSupervision);
        VpMemCpy(pLineObj->loopSup, loopSupervision, VP890_LOOP_SUP_LEN);

        /* Do not update the floor voltage in low power, the shadow register is still updated */
        if ((pDevObj->stateInt & VP890_LINE0_LP) == FALSE) {
            VpMpiCmdWrapper(deviceId, ecVal, VP890_REGULATOR_PARAM_WRT,
                VP890_REGULATOR_PARAM_LEN, pDevObj->devProfileData.swParams);
            VpMemCpy(pDevObj->swParamsCache, pDevObj->devProfileData.swParams,
                VP890_REGULATOR_PARAM_LEN);
        }
    }

    return VP_STATUS_SUCCESS;
}

/*******************************************************************************
 * Vp890UpdateCalValue()
 *  This function loads the device with calibration values provided by VpCal()
 * "Apply System Coefficient" process.
 *
 * Preconditions:
 *  System calibration values provided or calibration previously run.
 *
 * Postconditions:
 *  The device is loaded per the applied calibration values.
 ******************************************************************************/
bool
Vp890UpdateCalValue(
    VpLineCtxType *pLineCtx)
{
    Vp890LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp890DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal = pLineObj->ecVal;
    uint8 channelId = pLineObj->channelId;

    bool calStatus = FALSE;

    if ((pLineObj->calLineData.dcFeedRef[0] == 0x00) || (pLineObj->status & VP890_IS_FXO)) {
        return calStatus;
    }

    /*
     * Update the target DC Feed registers used after calibration has
     * been run. So we don't need to replace VAS (0 only if calibration
     * not run, but then corrected during calibration).
     */
    pLineObj->calLineData.dcFeed[0] &= ~VP890_VOC_VALUE_MASK;
    pLineObj->calLineData.dcFeed[0] |=
        (pLineObj->calLineData.dcFeedRef[0] & VP890_VOC_VALUE_MASK);

    pLineObj->calLineData.dcFeedPr[0] &= ~VP890_VOC_VALUE_MASK;
    pLineObj->calLineData.dcFeedPr[0] |=
        (pLineObj->calLineData.dcFeedRef[0] & VP890_VOC_VALUE_MASK);

    pLineObj->calLineData.dcFeedPr[1] &= ~VP890_ILA_MASK;
    pLineObj->calLineData.dcFeedPr[1] |=
        (pLineObj->calLineData.dcFeedRef[1] & VP890_ILA_MASK);

    pLineObj->calLineData.dcFeed[1] &= ~VP890_ILA_MASK;
    pLineObj->calLineData.dcFeed[1] |=
        (pLineObj->calLineData.dcFeedRef[1] & VP890_ILA_MASK);

    /*
     * Adjust for the errors if previously calibrated. The ILA function
     * returns "TRUE" if an adjustment was made meaning calibration
     * done previously, FALSE if not.
     */
    if (Vp890AdjustIla(pLineCtx, (pLineObj->calLineData.dcFeedRef[1] & VP890_ILA_MASK)) == TRUE) {
        uint16 vasVoltScale;
        calStatus = TRUE;

        pLineObj->calLineData.calDone = TRUE;

        Vp890AdjustVoc(pLineCtx, ((pLineObj->calLineData.dcFeedRef[0] >> 2) & 0x7), TRUE);

        /* Set VAS to device calibrated values */
        vasVoltScale = (3000 + (uint16)pDevObj->vp890SysCalData.vas[channelId][VP890_NORM_POLARITY] * 750);
        VpCSLACSetVas(pLineObj->calLineData.dcFeed, vasVoltScale);

        vasVoltScale = (3000 + (uint16)pDevObj->vp890SysCalData.vas[channelId][VP890_REV_POLARITY] * 750);
        VpCSLACSetVas(pLineObj->calLineData.dcFeedPr, vasVoltScale);

        if (pLineObj->slicValueCache & VP890_SS_POLARITY_MASK) {
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                VP890_DC_FEED_LEN, pLineObj->calLineData.dcFeedPr);
        } else {
            VpMpiCmdWrapper(deviceId, ecVal, VP890_DC_FEED_WRT,
                VP890_DC_FEED_LEN, pLineObj->calLineData.dcFeed);
        }

        Vp890BatteryAdjust(pLineCtx);
    }
    return calStatus;
}
#endif

#if defined (VP_CSLAC_SEQ_EN) && defined (VP890_FXS_SUPPORT)
/*******************************************************************************
 * Vp890InitRing()
 *  This function is used to initialize the ringing profile and caller ID
 * cadence on a given line.
 *
 * Arguments:
 *  *pLineCtx       - Line Context to modify Ringing Parameters for
 *  pCadProfile     - Pointer of a Ringing Cadence profile, or the index into
 *                    the Ringing Cadence profile table.
 *  pCidProfile     - Pointer of a Caller ID profile, or the index into the
 *                    Caller ID profile table.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to by the line context passed is initialized with the
 * ringing and caller ID profile specified.  The profiles may be specified as
 * either an index into the devic profile table or by profile pointers. This
 * function returns the success code if the device has been initialized and both
 * indexes (if indexes are passed) are within the range of the device profile
 * table.
 ******************************************************************************/
VpStatusType
Vp890InitRing(
    VpLineCtxType           *pLineCtx,
    VpProfilePtrType        pCadProfile,
    VpProfilePtrType        pCidProfile)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP890_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Check the legality of the Ring CAD profile */
    if (!Vp890IsProfileValid(VP_PROFILE_RINGCAD,
        VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE,
        pDevObj->profEntry.ringCadProfEntry,
        pDevObj->devProfileTable.pRingingCadProfileTable,
        pCadProfile, &pLineObj->pRingingCadence))
    {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }

    /* Check the legality of the Ring CID profile */
    if (!Vp890IsProfileValid(VP_PROFILE_CID,
        VP_CSLAC_CALLERID_PROF_TABLE_SIZE,
        pDevObj->profEntry.cidCadProfEntry,
        pDevObj->devProfileTable.pCallerIdProfileTable,
        pCidProfile, &pLineObj->pCidProfileType1))
    {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890InitRing() */

/*******************************************************************************
 * Vp890InitCid()
 *  This function is used to send caller ID information. It accepts an amount of
 * CID message data up to a "full" buffer amount (2 times the amount of the
 * size used for ContinueCID). It low fills the primary buffer such that the
 * application is interrupted at the earliest time when the API is ready to
 * accept more data.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. This function needs to be called before placing the line in to
 * ringing state.
 *
 * Postconditions:
 *  This function transmits the given CID information on the line (when the line
 * is placed in the ringing state).
 ******************************************************************************/
VpStatusType
Vp890InitCid(
    VpLineCtxType           *pLineCtx,
    uint8                   length,
    uint8p                  pCidData)
{
    VpDevCtxType            *pDevCtx    = pLineCtx->pDevCtx;
    Vp890LineObjectType     *pLineObj   = pLineCtx->pLineObj;
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpCallerIdType          *pCid       = &pLineObj->callerId;

    /* Get out if device state is not ready */
    if (!Vp890IsDevReady(pDevObj->state, TRUE)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP890_IS_FXO) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890InitCid() - FXO does not support CID"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length > (2 * VP_SIZEOF_CID_MSG_BUFFER)) {
        VP_ERROR(VpLineCtxType, pLineCtx, ("Vp890InitCid() - length exceeds internal msg buffer"));
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pCid->cliIndex = 0;
    pCid->cliMPIndex = 0;
    pCid->cliMSIndex = 0;

    /* Stop CID if it was in progress */
    pCid->cliTimer = 0;
    pCid->status = VP_CID_RESET_VALUE;
    pLineObj->suspendCid = FALSE;
    pCid->dtmfStatus = VP_CID_DTMF_RESET_VALUE;
    pCid->cidCheckSum = 0;

    /* load up the internal CID buffers */
    Vp890LoadCidBuffers(length, pCid, pCidData);

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp890InitCid() */
#endif /* VP_CSLAC_SEQ_EN */

/*******************************************************************************
 * Vp890InitProf()
 *  This function is used to initialize profile tables.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  Stores the given profile at the specified index of the profile table.
 ******************************************************************************/
VpStatusType
Vp890InitProf(
    VpDevCtxType            *pDevCtx,
    VpProfileType           type,
    VpProfilePtrType        pProfileIndex,
    VpProfilePtrType        pProfile)
{
    Vp890DeviceObjectType   *pDevObj    = pDevCtx->pDevObj;
    VpDeviceIdType          deviceId    = pDevObj->deviceId;
    VpStatusType            status      = VP_STATUS_SUCCESS;


    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    /*
     * The correct types are passed, but check to make sure the specific profile
     * type being initialized is valid as well as the index value
     */
    switch (type) {
        case VP_PROFILE_DEVICE:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_DEVICE,
                        VP_CSLAC_DEV_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.devProfEntry,
                        pDevObj->devProfileTable.pDevProfileTable);
            break;

        case VP_PROFILE_AC:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_AC,
                        VP_CSLAC_AC_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.acProfEntry,
                        pDevObj->devProfileTable.pAcProfileTable);
            break;

#if defined (VP890_FXS_SUPPORT)
        case VP_PROFILE_DC:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_DC,
                        VP_CSLAC_DC_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.dcProfEntry,
                        pDevObj->devProfileTable.pDcProfileTable);
            break;

        case VP_PROFILE_RING:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_RING,
                        VP_CSLAC_RINGING_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.ringingProfEntry,
                        pDevObj->devProfileTable.pRingingProfileTable);
            break;
#endif

        case VP_PROFILE_TONE:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_TONE,
                        VP_CSLAC_TONE_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.toneProfEntry,
                        pDevObj->devProfileTable.pToneProfileTable);
            break;

#if defined (VP890_FXO_SUPPORT)
        case VP_PROFILE_FXO_CONFIG:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_FXO_CONFIG,
                        VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.fxoConfigProfEntry,
                        pDevObj->devProfileTable.pFxoConfigProfileTable);
            break;
#endif

#if defined (VP_CSLAC_SEQ_EN)
#if defined (VP890_FXS_SUPPORT)
        case VP_PROFILE_RINGCAD:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_RINGCAD,
                        VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.ringCadProfEntry,
                        pDevObj->devProfileTable.pRingingCadProfileTable);
            break;

        case VP_PROFILE_CID:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_CID,
                        VP_CSLAC_CALLERID_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.cidCadProfEntry,
                        pDevObj->devProfileTable.pCallerIdProfileTable);
            break;
#endif

        case VP_PROFILE_TONECAD:
            status = InitProfToTable(pProfile, pProfileIndex, VP_PROFILE_TONECAD,
                        VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE,
                        &pDevObj->profEntry.toneCadProfEntry,
                        pDevObj->devProfileTable.pToneCadProfileTable);
            break;

#endif /* VP_CSLAC_SEQ_EN */

        default:
            status = VP_STATUS_INVALID_ARG;
            VP_ERROR(VpDevCtxType, pDevCtx, ("Vp890InitProf - Invalid profile type"));
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* Vp890InitProf() */


/*******************************************************************************
 * InitProfToTable()
 * This function loads a profile into the a profile table..
 *
 * Arguments:
 *  pProfile        -   profile to load into table
 *  profType        -   type of profile
 *  tableSize       -   max size of the profile table corresponding to profType
 *  pProfEntry      -   pointer to profile tables entry table
 *  pProfTable      -   pointer to the profile table where the input profile
 *                      pointer will be stored.
 *
 * Returns
 *  TRUE  - If device is ready
 *  FALSE - If the device is busy being initialized or calibrated.
 ******************************************************************************/
static VpStatusType
InitProfToTable(
    VpProfilePtrType    pProfile,
    VpProfilePtrType    pProfileIndex,
    VpProfileType       profType,
    uint16              tableSize,
    uint16              *pProfEntry,
    VpProfilePtrType    *pProfTable)

{
    /*
     * If the profile data is an index, indicated by Get Profile Index return
     * value of > -1, return an error (cannot init an indexed entry with an
     * index).
     */
    int profileIndex = VpGetProfileIndex(pProfile);
    if (profileIndex >= 0) {
        VP_ERROR(None, VP_NULL, ("InitProfToTable() - invalid profIndex"));
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If pProfileIndex is -1, the profile is of pointer type and invalid,
     * otherwise it is an index.  If it's an index, make sure the range is
     * valid.
     */
    profileIndex = VpGetProfileIndex(pProfileIndex);
    if ((profileIndex >= tableSize) || (profileIndex < 0)) {
        VP_ERROR(None, VP_NULL, ("InitProfToTable() - profIndex exceeds table size"));
        return VP_STATUS_INVALID_ARG;
    }

    if(VpVerifyProfileType(profType, pProfile) == TRUE) {
        /*  If the profile is null, clear the flag  associated with it */
        if (pProfile == VP_PTABLE_NULL) {
            *pProfEntry &= ~(0x01 << profileIndex);
        } else {
            *pProfEntry |= (0x01 << profileIndex);
        }
        pProfTable[profileIndex] = pProfile;
        return VP_STATUS_SUCCESS;
    }
    VP_ERROR(None, VP_NULL, ("InitProfToTable() - invalid profile type"));
    return VP_STATUS_ERR_PROFILE;
}

#ifdef VP890_FXS_SUPPORT
static void
Vp890CopyDefaultFRProfile(
    Vp890DeviceObjectType *pDevObj)
{
    /*
     * The 1st timing value can't be 0x00, so it's a marker value -> no profile
     * available
     */
    uint8 timingParamsFR_DFLT[VP890_INT_SWREG_PARAM_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    VpMemCpy(pDevObj->devProfileData.timingParamsFR, timingParamsFR_DFLT,
        VP890_REGULATOR_TIMING_LEN);
}
#endif
#endif /* VP_CC_890_SERIES */


