/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-20 11:17:54 #$
  File Revision : $Revision:: 6131 $
------------------------------------------------------------------------------*/
/**
 @file    sony_demod_dvbt_monitor.h

          This file provides the DVB-T demodulator monitor interface.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DEMOD_DVBT_MONITOR_H
#define SONY_DEMOD_DVBT_MONITOR_H

#include "sony_demod.h"
#include "sony_dvbt.h"

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/
/**
 @brief Sync state monitor.

 @param pDemod The demodulator instance.
 @param pSyncStat The demodulator state.
        - 0: WAIT_AGC,       
        - 1: WAIT_MGD, 
        - 2: WAIT_FFCR, 
        - 3: WAIT_ARGD, 
        - 4: WAIT_TITP, 
        - 5: WAIT_TPS, 
        - 6: TPS_LOCK
 @param pTSLockStat Indicates the TS lock condition.
        - 0: TS not locked.
        - 1: TS locked.
 @param pUnlockDetected Indicates the early unlock condition.
        - 0: No early unlock.
        - 1: Early unlock.

 @return SONY_RESULT_OK if successful and pSyncStat, pTSLockStat valid.
*/
sony_result_t sony_demod_dvbt_monitor_SyncStat (sony_demod_t * pDemod, 
                                                uint8_t * pSyncStat, 
                                                uint8_t * pTSLockStat, 
                                                uint8_t * pUnlockDetected);

/**
 @brief Monitor the IFAGC value. 
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pIFAGCOut The IFAGC output register value. Range 0x000 - 0xFFF. Unitless.

 @return SONY_RESULT_OK if successful and pIFAGCOut valid.
*/
sony_result_t sony_demod_dvbt_monitor_IFAGCOut (sony_demod_t * pDemod, 
                                                uint32_t * pIFAGCOut);

/**
 @brief Monitor the detected mode/guard (not TPS mode/guard).

 @param pDemod The demodulator instance.
 @param pMode Mode estimation result.
 @param pGuard Guard interval estimation result.

 @return SONY_RESULT_OK if successful and pMode, pGuard valid.
*/
sony_result_t sony_demod_dvbt_monitor_ModeGuard (sony_demod_t * pDemod,
                                                 sony_dvbt_mode_t * pMode, 
                                                 sony_dvbt_guard_t * pGuard);

/**
 @brief Monitors the detected carrier offset of the currently tuned channel.    

        To get the estimated center frequency of the current channel:
        Freq_Est = Freq_Tune + pOffset;

        This function will compensate for an inverting tuner architecture if the 
        demodulator has been configured accordignly using ::SONY_DEMOD_CONFIG_SPECTRUM_INV 
        config option for ::sony_demod_SetConfig.

 @param pDemod The demodulator instance.
 @param pOffset Carrier offset value (Hz).

 @return SONY_RESULT_OK if successful and pOffset valid.
*/
sony_result_t sony_demod_dvbt_monitor_CarrierOffset (sony_demod_t * pDemod, 
                                                     int32_t * pOffset);

/**
 @brief Monitor the Pre-Viterbi BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre viterbi decoder) x 1e7.

 @return SONY_RESULT_OK if successful and pBER valid.
*/
sony_result_t sony_demod_dvbt_monitor_PreViterbiBER (sony_demod_t * pDemod, 
                                                     uint32_t * pBER);

/**
 @brief Monitor the Pre-RS BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre reed solomon decoder) x 1e7.

 @return SONY_RESULT_OK if successful and pBER valid.
*/
sony_result_t sony_demod_dvbt_monitor_PreRSBER (sony_demod_t * pDemod, 
                                                uint32_t * pBER);

/**
 @brief Monitor the TPS information.

 @param pDemod The demodulator instance.
 @param pInfo The TPS information.

 @return SONY_RESULT_OK if successful and pInfo valid.
*/
sony_result_t sony_demod_dvbt_monitor_TPSInfo (sony_demod_t * pDemod, 
                                               sony_dvbt_tpsinfo_t * pInfo);

/**
 @brief Monitors the number RS (Reed Solomon) errors detected by the 
        RS decoder over 1 second. Also known as the code word reject count.

 @param pDemod The demodulator instance.
 @param pPEN The number of RS errors detected over 1 second.

 @return SONY_RESULT_OK if successful and pPEN valid. 
*/
sony_result_t sony_demod_dvbt_monitor_PacketErrorNumber (sony_demod_t * pDemod, 
                                                         uint32_t * pPEN);

/**
 @brief Monitors the channel spectrum sense.  To ensure correct polarity detection
        please use the ::SONY_DEMOD_CONFIG_SPECTRUM_INV config option in 
        ::sony_demod_SetConfig to select the appropriate spectrum inversion
        for the tuner output.

 @param pDemod The demodulator instance.
 @param pSense Indicates the spectrum sense.

 @return SONY_RESULT_OK if successful and pSense is valid.
*/
sony_result_t sony_demod_dvbt_monitor_SpectrumSense (sony_demod_t * pDemod, 
                                                     sony_demod_terr_cable_spectrum_sense_t * pSense);

/**
 @brief Monitors the estimated SNR register value, clipped to a 
        maximum of 40dB.

 @param pDemod The demodulator instance.
 @param pSNR The estimated SNR in dBx1000.

 @return SONY_RESULT_OK if successful and pSNR is valid.
*/
sony_result_t sony_demod_dvbt_monitor_SNR (sony_demod_t * pDemod, 
                                           int32_t * pSNR);

/**
 @brief Monitor the sampling frequency offset value.

 @param pDemod The demodulator instance.
 @param pPPM The sampling frequency offset in ppm x 100.
             Range: +/- 220ppm.

 @return SONY_RESULT_OK if pPPM is valid.
*/
sony_result_t sony_demod_dvbt_monitor_SamplingOffset (sony_demod_t * pDemod, 
                                                      int32_t * pPPM);

/**
 @brief Monitor the DVB-T quality metric. Based on Nordig SQI 
        equation.
 
 @param pDemod The demodulator instance.
 @param pQuality The quality as a percentage (0-100).

 @return SONY_RESULT_OK if successful and pQuality valid.
*/
sony_result_t sony_demod_dvbt_monitor_Quality (sony_demod_t * pDemod, 
                                               uint8_t * pQuality);


/**
 @brief Monitor the DVB-T PER (Packet Error Rate) parameters.

 @param pDemod The demod instance.
 @param pPER The estimated PER x 1e6.
 
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_monitor_PER (sony_demod_t * pDemod, 
                                           uint32_t * pPER);

#endif /* SONY_DEMOD_DVBT_MONITOR_H */
