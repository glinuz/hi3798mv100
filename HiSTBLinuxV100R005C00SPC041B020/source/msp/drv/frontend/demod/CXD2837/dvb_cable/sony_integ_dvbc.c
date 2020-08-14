/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-20 13:53:18 #$
  File Revision : $Revision:: 6134 $
------------------------------------------------------------------------------*/
#include "sony_integ.h"
#include "sony_integ_dvbc.h"
#include "sony_demod.h"
#include "sony_demod_dvbc.h"
#include "sony_demod_dvbc_monitor.h"

#ifndef SONY_INTEG_DISABLE_ASCOT_TUNER
#include "sony_tuner_ascot2e.h"
#endif

/*------------------------------------------------------------------------------
 Static Function Prototypes
------------------------------------------------------------------------------*/
/*
 @brief Waits for demodulator lock, polling ::sony_demod_dvbc_monitor_SyncStat at 
        10ms intervals.  Called as part of the Tune process.
*/
static sony_result_t dvbc_WaitDemodLock (sony_integ_t * pInteg);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
sony_result_t sony_integ_dvbc_Tune(sony_integ_t * pInteg,
                                   sony_dvbc_tune_param_t * pTuneParam)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbc_Tune");

    if ((!pInteg) || (!pTuneParam) || (!pInteg->pDemod)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
    
    if ((pInteg->pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (!pInteg->pDemod->scanMode) {
        /* Clear cancellation flag. */
        sony_atomic_set (&(pInteg->cancel), 0);
    }

    /* Tune the demodulator */
    result = sony_demod_dvbc_Tune (pInteg->pDemod);
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
        result = pInteg->pTunerTerrCable->Tune (pInteg->pTunerTerrCable, pTuneParam->centerFreqKHz, SONY_DTV_SYSTEM_DVBC, SONY_DEMOD_BW_8_MHZ);
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
    result = sony_integ_dvbc_WaitTSLock (pInteg);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }    

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbc_Scan(sony_integ_t * pInteg,
                                   sony_integ_dvbc_scan_param_t * pScanParam,
                                   sony_integ_dvbc_scan_callback_t callBack)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_integ_dvbc_scan_result_t scanResult;

    SONY_TRACE_ENTER ("sony_integ_dvbc_Scan");

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
    
    /* Enable scan mode */
    result = sony_demod_terr_cable_SetScanMode(pInteg->pDemod, SONY_DTV_SYSTEM_DVBC, 0x01);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Set the start frequency */
    scanResult.centerFreqKHz = pScanParam->startFrequencyKHz;

    /* Scan routine */
    while (scanResult.centerFreqKHz <= pScanParam->endFrequencyKHz) {
        sony_dvbc_tune_param_t tuneParam;

        tuneParam.centerFreqKHz = scanResult.centerFreqKHz;

        scanResult.tuneResult = sony_integ_dvbc_Tune(pInteg, &tuneParam);
        switch (scanResult.tuneResult) {
        case SONY_RESULT_OK:          
            /* Channel found, callback to application */         
            scanResult.tuneParam = tuneParam;
            scanResult.centerFreqKHz = tuneParam.centerFreqKHz;
            callBack (pInteg, &scanResult, pScanParam);
            break;

        /* Intentional fall-through. */
        case SONY_RESULT_ERROR_UNLOCK:
        case SONY_RESULT_ERROR_TIMEOUT:
            /* Channel not found, callback to applicaiton for progress updates */
            callBack (pInteg, &scanResult, pScanParam);
            break;

        default:
            /* Serious error occurred -> cancel operation. */
            result = sony_demod_terr_cable_SetScanMode(pInteg->pDemod, SONY_DTV_SYSTEM_DVBC, 0x00);
            SONY_TRACE_RETURN (scanResult.tuneResult);
        }

        scanResult.centerFreqKHz += pScanParam->stepFrequencyKHz;

        /* Check cancellation. */
        if (sony_atomic_read (&(pInteg->cancel)) != 0) {
            result = sony_demod_terr_cable_SetScanMode(pInteg->pDemod, SONY_DTV_SYSTEM_DVBC, 0x00);
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_CANCEL);
        }
    }

    /* Clear scan mode */
    result = sony_demod_terr_cable_SetScanMode(pInteg->pDemod, SONY_DTV_SYSTEM_DVBC, 0x00);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbc_WaitTSLock (sony_integ_t * pInteg)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbc_WaitTSLock");

    if (!pInteg) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
    
    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    result = dvbc_WaitDemodLock(pInteg);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_integ_dvbc_monitor_RFLevel (sony_integ_t * pInteg, int32_t * pRFLeveldB)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_integ_dvbc_monitor_RFLevel");

    if ((!pInteg) || (!pInteg->pDemod) || (!pRFLeveldB)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pInteg->pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pInteg->pDemod->system != SONY_DTV_SYSTEM_DVBC) {
        /* Not DVB-C*/
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    switch (pInteg->pDemod->tunerOptimize) {
#ifndef SONY_INTEG_DISABLE_ASCOT_TUNER
    case SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D:
        {
            uint32_t ifAgc;

            result = sony_demod_dvbc_monitor_IFAGCOut(pInteg->pDemod, &ifAgc);
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

/*------------------------------------------------------------------------------
 Static Functions
------------------------------------------------------------------------------*/
static sony_result_t dvbc_WaitDemodLock (sony_integ_t * pInteg)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_demod_lock_result_t lock = SONY_DEMOD_LOCK_RESULT_NOTDETECT;
    sony_stopwatch_t timer;
    uint8_t continueWait = 1;
    uint32_t elapsed = 0;

    SONY_TRACE_ENTER ("dvbc_WaitDemodLock");

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

    for (;;) {
        result = sony_stopwatch_elapsed(&timer, &elapsed);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
  
        if (elapsed >= SONY_DVBC_WAIT_DEMOD_LOCK) {
            continueWait = 0;
        }

        result = sony_demod_dvbc_CheckDemodLock (pInteg->pDemod, &lock);
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
            result = sony_stopwatch_sleep (&timer, SONY_DVBC_WAIT_LOCK_INTERVAL);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        } 
        else {
            result = SONY_RESULT_ERROR_TIMEOUT;
            break;
        }
    }
    
    SONY_TRACE_RETURN (result);
}
