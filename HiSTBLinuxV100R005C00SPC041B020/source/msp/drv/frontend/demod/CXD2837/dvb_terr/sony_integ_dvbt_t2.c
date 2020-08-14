/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-20 14:39:26 #$
  File Revision : $Revision:: 6137 $
------------------------------------------------------------------------------*/

#include "sony_integ.h"
#include "sony_integ_dvbt_t2.h"
#include "sony_demod.h"
#include "sony_demod_dvbt_monitor.h"
#include "sony_demod_dvbt2_monitor.h"
#include "hi_drv_tuner.h"

#ifndef SONY_INTEG_DISABLE_ASCOT_TUNER
#include "sony_tuner_ascot2e.h"
#endif
/*------------------------------------------------------------------------------
 Static Function Prototypes
------------------------------------------------------------------------------*/
/*
 @brief Waits for demodulator lock, polling ::sony_demod_dvbt_monitor_SyncStat
        or ::sony_demod_dvbt2_monitor_SyncStat at 10ms intervals.  Called as
        part of the Tune process.
*/
static sony_result_t dvbt_t2_WaitDemodLock (sony_integ_t * pInteg);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
sony_result_t sony_integ_dvbt_Tune(sony_integ_t * pInteg,
                                   sony_dvbt_tune_param_t * pTuneParam)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbt_Tune");

    if ((!pInteg) || (!pTuneParam) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pInteg->pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    /* Clear cancellation flag. */
    sony_atomic_set (&(pInteg->cancel), 0);
    /* Check bandwidth validity */
    if ((pTuneParam->bandwidth != SONY_DEMOD_BW_5_MHZ) && (pTuneParam->bandwidth != SONY_DEMOD_BW_6_MHZ) &&
        (pTuneParam->bandwidth != SONY_DEMOD_BW_7_MHZ) && (pTuneParam->bandwidth != SONY_DEMOD_BW_8_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }
    /* Set DVB-T profile for acquisition */
    result = sony_demod_dvbt_SetProfile(pInteg->pDemod, pTuneParam->profile);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Tune the demodulator */
    result = sony_demod_dvbt_Tune (pInteg->pDemod, pTuneParam);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if ((pInteg->pTunerTerrCable) && (pInteg->pTunerTerrCable->Tune)) {
#ifndef SONY_DISABLE_I2C_REPEATER
        /* Enable the I2C repeater */
        result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
#endif

        /* Tune the RF part */
        result = pInteg->pTunerTerrCable->Tune (pInteg->pTunerTerrCable, pTuneParam->centerFreqKHz, SONY_DTV_SYSTEM_DVBT, pTuneParam->bandwidth);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

#ifndef SONY_DISABLE_I2C_REPEATER
        /* Disable the I2C repeater */
        result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x00);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
#endif
    }

    /* Reset the demod to enable acquisition */
    result = sony_demod_TuneEnd (pInteg->pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Wait for TS lock */
    result = sony_integ_dvbt_t2_WaitTSLock (pInteg);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt2_Tune(sony_integ_t * pInteg,
                                    sony_dvbt2_tune_param_t * pTuneParam)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbt2_Tune");

    if ((!pInteg) || (!pTuneParam) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pInteg->pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Clear cancellation flag. */
    sony_atomic_set (&(pInteg->cancel), 0);

    /* Check bandwidth validity */
    if ((pTuneParam->bandwidth != SONY_DEMOD_BW_1_7_MHZ) && (pTuneParam->bandwidth != SONY_DEMOD_BW_5_MHZ) &&
        (pTuneParam->bandwidth != SONY_DEMOD_BW_6_MHZ) && (pTuneParam->bandwidth != SONY_DEMOD_BW_7_MHZ) &&
        (pTuneParam->bandwidth != SONY_DEMOD_BW_8_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Configure for manual PLP selection. */
    result = sony_demod_dvbt2_SetPLPConfig (pInteg->pDemod, 0x01, pTuneParam->dataPlpId);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Tune the demodulator */
    result = sony_demod_dvbt2_Tune (pInteg->pDemod, pTuneParam);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if ((pInteg->pTunerTerrCable) && (pInteg->pTunerTerrCable->Tune)) {
#ifndef SONY_DISABLE_I2C_REPEATER
        /* Enable the I2C repeater */
        result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
#endif

        /* Tune the RF part */
        result = pInteg->pTunerTerrCable->Tune (pInteg->pTunerTerrCable, pTuneParam->centerFreqKHz, SONY_DTV_SYSTEM_DVBT2, pTuneParam->bandwidth);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

#ifndef SONY_DISABLE_I2C_REPEATER
        /* Disable the I2C repeater */
        result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x00);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
#endif
    }

    /* Reset the demod to enable acquisition */
    result = sony_demod_TuneEnd (pInteg->pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Wait for demodulator lock */
    result = dvbt_t2_WaitDemodLock (pInteg);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Wait for TS lock */
    result = sony_integ_dvbt_t2_WaitTSLock (pInteg);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Confirm correct PLP selection in acquisition */
    {
        /* In DVB-T2, sony_demod_dvbt2_monitor_DataPLPError occasionally returns
           SONY_RESULT_ERROR_HW_STATE because L1 Post information is not valid at
           that time. (L1POST_OK bit != 1).  This loop handles such cases.
           This issue occurs only under clean signal lab conditions, and will
           therefore not extend acquistion time under normal conditions.
        */
        uint32_t waitTime = 0;
        uint8_t plpNotFound;

        do {
            /* Check cancellation. */
            if (sony_atomic_read (&(pInteg->cancel)) != 0) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
            }

            result = sony_demod_dvbt2_monitor_DataPLPError (pInteg->pDemod, &plpNotFound);

            if (result == SONY_RESULT_ERROR_HW_STATE) {
                if (waitTime >= SONY_DVBT2_L1POST_TIMEOUT) {
                    SONY_TRACE_RETURN (SONY_RESULT_ERROR_TIMEOUT);
                }
                else {
                    SONY_SLEEP (SONY_DVBT_T2_WAIT_LOCK_INTERVAL);
                    waitTime += SONY_DVBT_T2_WAIT_LOCK_INTERVAL;
                }
            }
            else if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result); /* Fatal error */
            }
        } while (result != SONY_RESULT_OK);

        if (plpNotFound) {
            result = SONY_RESULT_OK_CONFIRM;
            pTuneParam->tuneInfo = SONY_DEMOD_DVBT2_TUNE_INFO_INVALID_PLP_ID;
        }
        else {
            pTuneParam->tuneInfo = SONY_DEMOD_DVBT2_TUNE_INFO_OK;
        }
    }

    SONY_TRACE_RETURN (result);
}


sony_result_t sony_integ_dvbt_t2_Scan(sony_integ_t * pInteg,
                                      sony_integ_dvbt_t2_scan_param_t * pScanParam,
                                      sony_integ_dvbt_t2_scan_callback_t callBack)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_integ_dvbt_t2_scan_result_t scanResult;

    SONY_TRACE_ENTER ("sony_integ_dvbt_t2_Scan");

    if ((!pInteg) || (!pScanParam) || (!callBack) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pInteg->pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Clear cancellation flag. */
    sony_atomic_set (&(pInteg->cancel), 0);

    /* Ensure the scan parameters are valid. */
    if (pScanParam->endFrequencyKHz < pScanParam->startFrequencyKHz) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pScanParam->stepFrequencyKHz == 0) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Check bandwidth validity */
    if ((pScanParam->bandwidth != SONY_DEMOD_BW_1_7_MHZ) && (pScanParam->bandwidth != SONY_DEMOD_BW_5_MHZ) &&
        (pScanParam->bandwidth != SONY_DEMOD_BW_6_MHZ) && (pScanParam->bandwidth != SONY_DEMOD_BW_7_MHZ) &&
        (pScanParam->bandwidth != SONY_DEMOD_BW_8_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Check for invalid combination of 1.7MHz scanning with DVB-T */
    if ((pScanParam->system == SONY_DTV_SYSTEM_DVBT) && (pScanParam->bandwidth == SONY_DEMOD_BW_1_7_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Set the start frequency */
    scanResult.centerFreqKHz = pScanParam->startFrequencyKHz;

    /* Set scan mode enabled */
    sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x01);

    /* Scan routine */
    while (scanResult.centerFreqKHz <= pScanParam->endFrequencyKHz) {
        sony_dtv_system_t systemFound = SONY_DTV_SYSTEM_UNKNOWN;

        scanResult.tuneResult = sony_integ_dvbt_t2_BlindTune(pInteg, scanResult.centerFreqKHz, pScanParam->bandwidth, pScanParam->system, &systemFound);
        switch (scanResult.tuneResult) {
        case SONY_RESULT_OK:
            scanResult.system = systemFound;

            /* Channel found, callback to application */
            if (systemFound == SONY_DTV_SYSTEM_DVBT){
                scanResult.dvbtTuneParam.centerFreqKHz = scanResult.centerFreqKHz;
                scanResult.dvbtTuneParam.bandwidth = pScanParam->bandwidth;
                scanResult.dvbtTuneParam.profile = SONY_DVBT_PROFILE_HP;
                callBack (pInteg, &scanResult, pScanParam);
            }
            else if (systemFound == SONY_DTV_SYSTEM_DVBT2) {
                uint8_t numPLPs = 0;
                uint8_t PLPIds[255]={0};
                uint8_t i;
                uint32_t waitTime = 0;

                scanResult.dvbt2TuneParam.centerFreqKHz = scanResult.centerFreqKHz;
                scanResult.dvbt2TuneParam.bandwidth = pScanParam->bandwidth;
                scanResult.dvbt2TuneParam.tuneInfo = SONY_DEMOD_DVBT2_TUNE_INFO_OK;

                /* Obtain the T2 PLP list from Data PLP monitor.
                   In DVB-T2, sony_demod_dvbt2_monitor_DataPLPs occasionally returns
                   SONY_RESULT_ERROR_HW_STATE because L1 Post information is not valid
                   at that time. (L1POST_OK bit != 1).  This loop handles such cases.
                   This issue occurs only under clean signal lab conditions, and will
                   therefore not extend acquistion time under normal conditions.
                */
                for(;;) {
                    /* Check cancellation. */
                    if (sony_atomic_read (&(pInteg->cancel)) != 0) {
                        sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x00);
                        SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
                    }

                    /* Obtain the T2 PLP list from Data PLP monitor */
                    result = sony_demod_dvbt2_monitor_DataPLPs (pInteg->pDemod, PLPIds, &numPLPs);

                    if (result == SONY_RESULT_OK) {
                        /* Callback for each PLPs */
                        for (i = 0; i < numPLPs; i++) {
                            scanResult.dvbt2TuneParam.dataPlpId = PLPIds[i];
                            callBack (pInteg, &scanResult, pScanParam);
                        }
                        break;
                    } else if (result == SONY_RESULT_ERROR_HW_STATE) {
                        if (waitTime >= SONY_DVBT2_L1POST_TIMEOUT) {
                            /* L1 Post information timeout. Error timeout callback */
                            scanResult.tuneResult = SONY_RESULT_ERROR_TIMEOUT;
                            callBack (pInteg, &scanResult, pScanParam);
                            break;
                        } else {
                            SONY_SLEEP (SONY_DVBT_T2_WAIT_LOCK_INTERVAL);
                            waitTime += SONY_DVBT_T2_WAIT_LOCK_INTERVAL;
                        }
                    } else {
                        sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x00);
                        SONY_TRACE_RETURN (result); /* Other (fatal) error */
                    }
                }
            }
            break;

        /* Intentional fall-through. */
        case SONY_RESULT_ERROR_UNLOCK:
        case SONY_RESULT_ERROR_TIMEOUT:
            /* Channel not found, callback to applicaiton for progress updates */
            scanResult.system = SONY_DTV_SYSTEM_UNKNOWN;
            callBack (pInteg, &scanResult, pScanParam);
            break;

        default:
            {
                /* Serious error occurred -> cancel operation. */
                sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x00);
                SONY_TRACE_RETURN (scanResult.tuneResult);
            }
        }

        scanResult.centerFreqKHz += pScanParam->stepFrequencyKHz;

        /* Check cancellation. */
        if (sony_atomic_read (&(pInteg->cancel)) != 0) {
            sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x00);
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
        }
    }

    /* Clear scan mode */
    result = sony_demod_terr_cable_SetScanMode(pInteg->pDemod, pScanParam->system, 0x00);

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt_t2_BlindTune(sony_integ_t * pInteg,
                                           uint32_t centerFreqKHz,
                                           sony_demod_bandwidth_t bandwidth,
                                           sony_dtv_system_t system,
                                           sony_dtv_system_t * pSystemTuned)
{
    sony_result_t result = SONY_RESULT_OK;
    uint8_t channelFound = 0;

    SONY_TRACE_ENTER ("sony_integ_dvbt_t2_BlindTune");

    if ((!pInteg) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (!pInteg->pDemod->scanMode) {
        /* Clear cancellation flag. */
        sony_atomic_set (&(pInteg->cancel), 0);
    }

    /* Confirm the demod is in a valid state to accept this API */
    if ((pInteg->pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Check bandwidth validity */
    if ((bandwidth != SONY_DEMOD_BW_1_7_MHZ) && (bandwidth != SONY_DEMOD_BW_5_MHZ) &&
        (bandwidth != SONY_DEMOD_BW_6_MHZ) && (bandwidth != SONY_DEMOD_BW_7_MHZ) &&
        (bandwidth != SONY_DEMOD_BW_8_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Check for invalid system parameter */
    if ((system != SONY_DTV_SYSTEM_DVBT) && (system != SONY_DTV_SYSTEM_DVBT2) && (system != SONY_DTV_SYSTEM_ANY)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Check for invalid combination of 1.7MHz tune */
    if ((system != SONY_DTV_SYSTEM_DVBT2) && (bandwidth == SONY_DEMOD_BW_1_7_MHZ)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Attempt DVB-T acquisition */
    if ((system == SONY_DTV_SYSTEM_DVBT) || (system == SONY_DTV_SYSTEM_ANY)) {
        sony_dvbt_tune_param_t tuneParam;

        tuneParam.bandwidth = bandwidth;
        tuneParam.centerFreqKHz = centerFreqKHz;
        tuneParam.profile = SONY_DVBT_PROFILE_HP;

        /* Set DVB-T profile to HP to allow detection of hierachical and non-hierachical modes */
        result = sony_demod_dvbt_SetProfile(pInteg->pDemod, SONY_DVBT_PROFILE_HP);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Tune the demodulator */
        result = sony_demod_dvbt_Tune (pInteg->pDemod, &tuneParam);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        if ((pInteg->pTunerTerrCable) && (pInteg->pTunerTerrCable->Tune)) {
#ifndef SONY_DISABLE_I2C_REPEATER
            /* Enable the I2C repeater */
            result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x01);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
#endif

            /* Tune the RF part */
            result = pInteg->pTunerTerrCable->Tune (pInteg->pTunerTerrCable, tuneParam.centerFreqKHz, SONY_DTV_SYSTEM_DVBT, tuneParam.bandwidth);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

#ifndef SONY_DISABLE_I2C_REPEATER
            /* Disable the I2C repeater */
            result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x00);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
#endif
        }

        /* Reset the demod to enable acquisition */
        result = sony_demod_TuneEnd (pInteg->pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Wait for demodulator lock */
        result = dvbt_t2_WaitDemodLock (pInteg);
        switch (result) {
        case SONY_RESULT_OK:
            channelFound = 1;
            *pSystemTuned = SONY_DTV_SYSTEM_DVBT;
            break;

        /* Intentional fall-through */
        case SONY_RESULT_ERROR_TIMEOUT:
        case SONY_RESULT_ERROR_UNLOCK:
            /* Attempt DVB-T2 acquisition */
            break;

        default:
            SONY_TRACE_RETURN(result);
        }
    }
    /* Attempt DVB-T2 acquisition, only if DVB-T failed or didn't run */
    if (((system == SONY_DTV_SYSTEM_DVBT2) || (system == SONY_DTV_SYSTEM_ANY)) && (!channelFound)) {
        sony_dvbt2_tune_param_t tuneParam;

        tuneParam.bandwidth = bandwidth;
        tuneParam.centerFreqKHz = centerFreqKHz;
        tuneParam.dataPlpId = 0; /* Not used in blind acquisition */

        /* Configure for automatic PLP selection. */
        result = sony_demod_dvbt2_SetPLPConfig (pInteg->pDemod, 0x00, 0x00);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Tune the demodulator */
        result = sony_demod_dvbt2_Tune (pInteg->pDemod, &tuneParam);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

#ifdef SAME_TUNER_CONFIG_DVBT_T2
        /* If SAME_TUNER_CONFIG_DVBT_T2 is defined the driver assumes
           that the tuner configuration used for DVBT tuning is identical to
           DVBT2, therefore the tuner does not need to be reconfigured.  This will
           help to reduce blindtune and therefore scan duration. This is only
           valid if SONY_DTV_SYSTEM_ANY is used. */
        if (system == SONY_DTV_SYSTEM_DVBT2)
#endif
        {
            if ((pInteg->pTunerTerrCable) && (pInteg->pTunerTerrCable->Tune)) {
#ifndef SONY_DISABLE_I2C_REPEATER
                /* Enable the I2C repeater */
                result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x01);
                if (result != SONY_RESULT_OK) {
                    SONY_TRACE_RETURN (result);
                }
#endif

                /* Tune the RF part */
                result = pInteg->pTunerTerrCable->Tune (pInteg->pTunerTerrCable, tuneParam.centerFreqKHz, SONY_DTV_SYSTEM_DVBT2, tuneParam.bandwidth);
                if (result != SONY_RESULT_OK) {
                    SONY_TRACE_RETURN (result);
                }

#ifndef SONY_DISABLE_I2C_REPEATER
                /* Disable the I2C repeater */
                result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x00);
                if (result != SONY_RESULT_OK) {
                    SONY_TRACE_RETURN (result);
                }
#endif
            }
        }

        /* Reset the demod to enable acquisition */
        result = sony_demod_TuneEnd (pInteg->pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Wait for demodulator lock */
        result = dvbt_t2_WaitDemodLock (pInteg);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        *pSystemTuned = SONY_DTV_SYSTEM_DVBT2;
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt_t2_WaitTSLock (sony_integ_t * pInteg)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_demod_lock_result_t lock = SONY_DEMOD_LOCK_RESULT_NOTDETECT;
    uint16_t timeout = 0;
    sony_stopwatch_t timer;
    uint8_t continueWait = 1;
    uint32_t elapsed = 0;

    SONY_TRACE_ENTER ("sony_integ_dvbt_t2_WaitTSLock");

    if ((!pInteg) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Wait for TS lock */
    result = sony_stopwatch_start (&timer);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT) {
        timeout = SONY_DVBT_WAIT_TS_LOCK;
    }
    else if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT2) {
        timeout = SONY_DVBT2_WAIT_TS_LOCK;
    }
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    for (;;) {
        result = sony_stopwatch_elapsed(&timer, &elapsed);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        if (elapsed >= timeout) {
            continueWait = 0;
        }

        if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT) {
            result = sony_demod_dvbt_CheckTSLock (pInteg->pDemod, &lock);
        }
        else {
            result = sony_demod_dvbt2_CheckTSLock (pInteg->pDemod, &lock);
        }

        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        switch (lock) {
        case SONY_DEMOD_LOCK_RESULT_LOCKED:
            SONY_TRACE_RETURN (SONY_RESULT_OK);

        case SONY_DEMOD_LOCK_RESULT_UNLOCKED:
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_UNLOCK);

        default:
            /* continue waiting... */
            break;
        }

        /* Check cancellation. */
        if (sony_atomic_read (&(pInteg->cancel)) != 0) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
        }

        if (continueWait) {
            result = sony_stopwatch_sleep (&timer, SONY_DVBT_T2_WAIT_LOCK_INTERVAL);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        } else {
            result = SONY_RESULT_ERROR_TIMEOUT;
            break;
        }
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt_t2_monitor_RFLevel (sony_integ_t * pInteg, int32_t * pRFLeveldB)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbt_t2_monitor_RFLevel");

    if ((!pInteg) || (!pInteg->pDemod) || (!pRFLeveldB)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if ((pInteg->pDemod->system != SONY_DTV_SYSTEM_DVBT) && (pInteg->pDemod->system != SONY_DTV_SYSTEM_DVBT2)) {
        /* Not DVB-T or DVB-T2*/
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    switch (pInteg->pDemod->tunerOptimize) {
#ifndef SONY_INTEG_DISABLE_ASCOT_TUNER
    case SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D:
        {
            uint32_t ifAgc;

            if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT) {
                result = sony_demod_dvbt_monitor_IFAGCOut(pInteg->pDemod, &ifAgc);
            }
            else {
                result = sony_demod_dvbt2_monitor_IFAGCOut(pInteg->pDemod, &ifAgc);
            }

            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            /* Protect against overflow. IFAGC is unsigned 12-bit. */
            if (ifAgc > 0xFFF) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
            }

            /* 2nd order polynomial relationship :
             * RF (dB) = -4E-6 * IFAGC^2 + 0.0594 * IFAGC - 131.24
             * RF (dB*1000) = ((-4 * IFAGC^2) + (59400* IFAGC) - 131240000) / 1000
             */
            *pRFLeveldB = (int32_t) ((-4 * (int32_t) (ifAgc * ifAgc)) + (59400 * (int32_t) ifAgc) - 131240000);
            *pRFLeveldB = (*pRFLeveldB < 0) ? *pRFLeveldB - 500 : *pRFLeveldB + 500;
            *pRFLeveldB /= 1000;
        }
        break;

    case SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E:
        {
            int32_t ifGain;
            int32_t rfGain;

            if (pInteg->pTunerTerrCable) {
                #ifndef SONY_DISABLE_I2C_REPEATER
                    /* Enable the I2C repeater */
                    result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x01);
                    if (result != SONY_RESULT_OK) {
                        SONY_TRACE_RETURN (result);
                    }
                #endif

                result = sony_tuner_ascot2e_ReadGain (pInteg->pTunerTerrCable, &ifGain, &rfGain, 1);
                if (result != SONY_RESULT_OK) {
                    SONY_TRACE_RETURN (result);
                }

                #ifndef SONY_DISABLE_I2C_REPEATER
                    /* Disable the I2C repeater */
                    result = sony_demod_I2cRepeaterEnable (pInteg->pDemod, 0x00);
                    if (result != SONY_RESULT_OK) {
                        SONY_TRACE_RETURN (result);
                    }
                #endif

                /* RF Level dBm = IFOUT - IFGAIN - RFGAIN
                 * IFOUT is the target IF level for tuner, -4.2dBm
                 */
                *pRFLeveldB = 10 * (-420 - ifGain - rfGain);

                /* Subtract fixed offset to compensate for implementation:
                 * - Splitter and LNA = 8.64dB
                 */
                *pRFLeveldB -= 8640;
            }
            else {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
            }
        }
        break;
#endif

    default:
        /* Please add RF level calculation for non ASCOT tuners. */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt_monitor_SSI (sony_integ_t * pInteg, uint8_t * pSSI)
{
    int32_t rfLevel;
    sony_dvbt_tpsinfo_t tps;
    int32_t prel;
    int32_t tempSSI = 0;
    sony_result_t result = SONY_RESULT_OK;

    static const int32_t pRefdBm1000[3][5] = {
    /*    1/2,    2/3,    3/4,    5/6,    7/8,               */
        {-93000, -91000, -90000, -89000, -88000}, /* QPSK    */
        {-87000, -85000, -84000, -83000, -82000}, /* 16-QAM  */
        {-82000, -80000, -78000, -77000, -76000}, /* 64-QAM  */
    };

    SONY_TRACE_ENTER ("sony_integ_dvbt_monitor_SSI");

    if ((!pInteg) || (!pInteg->pDemod) || (!pSSI)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pInteg->pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Get estimated RF Level */
    result = sony_integ_dvbt_t2_monitor_RFLevel (pInteg, &rfLevel);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Monitor TPS for Modulation / Code Rate */
    result = sony_demod_dvbt_monitor_TPSInfo (pInteg->pDemod, &tps);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Ensure correct TPS values. */
    if ((tps.constellation >= SONY_DVBT_CONSTELLATION_RESERVED_3) || (tps.rateHP >= SONY_DVBT_CODERATE_RESERVED_5)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_OTHER);
    }

    /* prel = prec - pref */
    prel = rfLevel - pRefdBm1000[tps.constellation][tps.rateHP];

    /* SSI (Signal Strength Indicator) is calculated from:
     *
     * if (prel < -15dB)             SSI = 0
     * if (-15dB <= prel < 0dB)       SSI = (2/3) * (prel + 15)
     * if (0dB <= prel < 20dB)        SSI = (4 * prel) + 10
     * if (20dB <= prel < 35dB)       SSI = (2/3) * (prel - 20) + 90
     * if (prel >= 35dB)              SSI = 100
     */
    if (prel < -15000) {
        tempSSI = 0;
    }
    else if (prel < 0) {
        /* Note : prel and 2/3 scaled by 10^3 so divide by 10^6 added */
        tempSSI = ((2 * (prel + 15000)) + 1500) / 3000;
    }
    else if (prel < 20000) {
        /* Note : prel scaled by 10^3 so divide by 10^3 added */
        tempSSI = (((4 * prel) + 500) / 1000) + 10;
    }
    else if (prel < 35000) {
        /* Note : prel and 2/3 scaled by 10^3 so divide by 10^6 added */
        tempSSI = (((2 * (prel - 20000)) + 1500) / 3000) + 90;
    }
    else {
        tempSSI = 100;
    }

    /* Clip value to 100% */
    *pSSI = (tempSSI >= 100) ? 100 : (uint8_t)tempSSI;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbt2_monitor_SSI (sony_integ_t * pInteg, uint8_t * pSSI)
{
    int32_t rfLevel;
    sony_dvbt2_plp_constell_t qam;
    sony_dvbt2_plp_code_rate_t codeRate;
    int32_t prel;
    int32_t tempSSI = 0;
    sony_result_t result = SONY_RESULT_OK;

    static const int32_t pRefdBm1000[4][6] = {
    /*    1/2,    3/5,    2/3,    3/4,    4/5,    5/6                */
        {-96000, -95000, -94000, -93000, -92000, -92000}, /* QPSK    */
        {-91000, -89000, -88000, -87000, -86000, -86000}, /* 16-QAM  */
        {-86000, -85000, -83000, -82000, -81000, -80000}, /* 64-QAM  */
        {-82000, -80000, -78000, -76000, -75000, -74000}, /* 256-QAM */
    };

    SONY_TRACE_ENTER ("sony_integ_dvbt2_monitor_SSI");

    if ((!pInteg) || (!pInteg->pDemod) || (!pSSI)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pInteg->pDemod->system != SONY_DTV_SYSTEM_DVBT2) {
        /* Not DVB-T2*/
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Get estimated RF Level */
    result = sony_integ_dvbt_t2_monitor_RFLevel (pInteg, &rfLevel);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Get PLP constellation */
    result = sony_demod_dvbt2_monitor_QAM (pInteg->pDemod, SONY_DVBT2_PLP_DATA, &qam);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Get PLP code rate */
    result = sony_demod_dvbt2_monitor_CodeRate (pInteg->pDemod, SONY_DVBT2_PLP_DATA, &codeRate);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Ensure correct plp info. */
    if ((codeRate > SONY_DVBT2_R5_6) || (qam > SONY_DVBT2_QAM256)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_OTHER);
    }

    /* prel = prec - pref */
    prel = rfLevel - pRefdBm1000[qam][codeRate];

    /* SSI (Signal Strength Indicator) is calculated from:
     *
     * if (prel < -15dB)              SSI = 0
     * if (-15dB <= prel < 0dB)       SSI = (2/3) * (prel + 15)
     * if (0dB <= prel < 20dB)        SSI = 4 * prel + 10
     * if (20dB <= prel < 35dB)       SSI = (2/3) * (prel - 20) + 90
     * if (prel >= 35dB)              SSI = 100
     */
    if (prel < -15000) {
        tempSSI = 0;
    }
    else if (prel < 0) {
        /* Note : prel and 2/3 scaled by 10^3 so divide by 10^6 added */
        tempSSI = ((2 * (prel + 15000)) + 1500) / 3000;
    }
    else if (prel < 20000) {
        /* Note : prel scaled by 10^3 so divide by 10^3 added */
        tempSSI = (((4 * prel) + 500) / 1000) + 10;
    }
    else if (prel < 35000) {
        /* Note : prel and 2/3 scaled by 10^3 so divide by 10^6 added */
        tempSSI = (((2 * (prel - 20000)) + 1500) / 3000) + 90;
    }
    else {
        tempSSI = 100;
    }

    /* Clip value to 100% */
    *pSSI = ((tempSSI >= 100) ? 100 : (uint8_t)tempSSI);

    SONY_TRACE_RETURN (result);
}

/*------------------------------------------------------------------------------
 Static Functions
------------------------------------------------------------------------------*/
static sony_result_t dvbt_t2_WaitDemodLock (sony_integ_t * pInteg)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_demod_lock_result_t lock = SONY_DEMOD_LOCK_RESULT_NOTDETECT;
    uint16_t timeout = 0;
    sony_stopwatch_t timer;
    uint8_t continueWait = 1;
    uint32_t elapsed = 0;

    SONY_TRACE_ENTER ("dvbt_t2_WaitDemodLock");

    if ((!pInteg) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Wait for demod lock */
    result = sony_stopwatch_start (&timer);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT) {
        timeout = SONY_DVBT_WAIT_DEMOD_LOCK;
    }
    else if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT2) {
        timeout = SONY_DVBT2_WAIT_DEMOD_LOCK;
    }
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    for (;;) {
        result = sony_stopwatch_elapsed(&timer, &elapsed);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        if (elapsed >= timeout) {
            continueWait = 0;
        }

        if (pInteg->pDemod->system == SONY_DTV_SYSTEM_DVBT) {
            result = sony_demod_dvbt_CheckDemodLock (pInteg->pDemod, &lock);
        }
        else {
            result = sony_demod_dvbt2_CheckDemodLock (pInteg->pDemod, &lock);
        }

        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        switch (lock) {
        case SONY_DEMOD_LOCK_RESULT_LOCKED:
            SONY_TRACE_RETURN (SONY_RESULT_OK);

        case SONY_DEMOD_LOCK_RESULT_UNLOCKED:
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_UNLOCK);

        default:
            /* continue waiting... */
            break;
        }

        /* Check cancellation. */
        if (sony_atomic_read (&(pInteg->cancel)) != 0) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
        }

        if (continueWait) {
            result = sony_stopwatch_sleep (&timer, SONY_DVBT_T2_WAIT_LOCK_INTERVAL);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        } else {
            HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
            result = SONY_RESULT_ERROR_TIMEOUT;
            break;
        }
    }

    SONY_TRACE_RETURN (result);
}
