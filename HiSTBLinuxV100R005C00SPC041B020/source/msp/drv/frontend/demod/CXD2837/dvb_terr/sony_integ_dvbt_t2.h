/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-07-09 12:36:59 #$
  File Revision : $Revision:: 5796 $
------------------------------------------------------------------------------*/
/**
 @file    sony_integ_dvbt_t2.h

          This file provides the integration layer interface for DVB-T and DVB-T2
          specific demodulator functions.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_INTEG_DVBT_T2_H
#define SONY_INTEG_DVBT_T2_H

#include "sony_demod.h"
#include "sony_integ.h"
#include "sony_demod_dvbt.h"
#include "sony_demod_dvbt2.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define SONY_DVBT_WAIT_DEMOD_LOCK           1000    /**< 1s timeout for wait demodulator lock process for DVB-T channels */
#define SONY_DVBT_WAIT_TS_LOCK              1000    /**< 1s timeout for wait TS lock process for DVB-T channels */
#define SONY_DVBT2_WAIT_DEMOD_LOCK          3500    /**< 3.5s timeout for wait demodulator lock process for DVB-T2 channels */
#define SONY_DVBT2_WAIT_TS_LOCK             1500    /**< 1.5s timeout for wait TS lock process for DVB-T2 channels */
#define SONY_DVBT_T2_WAIT_LOCK_INTERVAL     10      /**< 10ms polling interval for demodulator and TS lock functions */
#define SONY_DVBT2_L1POST_TIMEOUT           300     /**< 300ms timeout for L1Post Valid loop */

/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The parameters used for DVB-T and DVB-T2 scanning.
*/
typedef struct sony_integ_dvbt_t2_scan_param_t{
    /**
     @brief The start frequency in kHz for scanning
    */
    uint32_t startFrequencyKHz;

    /**
     @brief The end frequency in kHz for scanning
    */
    uint32_t endFrequencyKHz;

    /**
     @brief The step frequency in kHz for scanning
    */
    uint32_t stepFrequencyKHz;

    /**
     @brief The bandwidth to use for tuning during the scan
    */
    sony_demod_bandwidth_t bandwidth;

    /**
     @brief The system to attempt to blind tune to at each step.  Use
            ::SONY_DTV_SYSTEM_ANY to run a multiple system scan (DVB-T and
            DVB-T2).
    */
    sony_dtv_system_t system;
}sony_integ_dvbt_t2_scan_param_t;

/**
 @brief The structure used to return a channel located or progress update 
        as part of a DVB-T / DVB-T2 or combined scan.
*/
typedef struct sony_integ_dvbt_t2_scan_result_t{
    /**
     @brief Indicates the current frequency just attempted for the scan.  This would
            primarily be used to calculate scan progress from the scan parameters.
    */
    uint32_t centerFreqKHz;

    /**
     @brief Indicates if the tune result at the current frequency.  SONY_RESULT_OK
            means that a channel has been locked and one of the tuneParam structures
            contain the channel infomration.
    */
    sony_result_t tuneResult;

    /**
     @brief The system of the channel detected by the scan.  This should be used to determine
            which of the following tune param structs are valid.
    */
    sony_dtv_system_t system;

    /**
     @brief The tune params for a located DVB-T channel.
    */
    sony_dvbt_tune_param_t dvbtTuneParam;

    /**
     @brief The tune params for a located DVB-T2 channel.
    */
    sony_dvbt2_tune_param_t dvbt2TuneParam;
}sony_integ_dvbt_t2_scan_result_t;

/*------------------------------------------------------------------------------
 Function Pointers
------------------------------------------------------------------------------*/
/**
 @brief Callback function that is called for every attempted frequency during a 
        scan.  For successful channel results the function is called after 
        demodulator lock but before TS lock is achieved (DVB-T : TPS Lock, 
        DVB-T2 : Demod Lock).
        
        NOTE: for DVB-T2 this function is invoked for each PLP within the signal.
 
 @param pInteg The driver instance.
 @param pResult The current scan result.
 @param pScanParam The current scan parameters.
*/
typedef void (*sony_integ_dvbt_t2_scan_callback_t) (sony_integ_t * pInteg, 
                                                    sony_integ_dvbt_t2_scan_result_t * pResult,
                                                    sony_integ_dvbt_t2_scan_param_t * pScanParam);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
/**
 @brief Performs acquisition to a DVB-T channel. 
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::sony_integ_Cancel to cancel the operation at any time.
 
 @note  For non-hierachical modes, the profile MUST be set to 
        SONY_DVBT_PROFILE_HP.

 @note  For hierachical modes, if the LP stream is selected and is not available, 
        then the HP stream will be output.
          
 @param pInteg The driver instance.
 @param pTuneParam The parameters required for the tune.

 @return SONY_RESULT_OK if tuned successfully to the channel.
*/
sony_result_t sony_integ_dvbt_Tune(sony_integ_t * pInteg,
                                   sony_dvbt_tune_param_t * pTuneParam);

/**
 @brief Performs acquisition to a DVB-T2 channel. 
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::sony_integ_Cancel to cancel the operation at any time.
 
        During a tune the device will wait for a T2 P1 symbol in order to decode the 
        L1 pre signalling and then begin demodulation. If the data PLP ID 
        (::sony_dvbt2_tune_param_t::dataPlpId) or the associated common PLP is not 
        found in the channel, the device will always select the first found PLP and 
        output the associated TS. In this case, an indicator in the 
        ::sony_dvbt2_tune_param_t::tuneInfo will be set.
          
 @param pInteg The driver instance.
 @param pTuneParam The parameters required for the tune.

 @return SONY_RESULT_OK if tuned successfully to the channel.
*/
sony_result_t sony_integ_dvbt2_Tune(sony_integ_t * pInteg,
                                    sony_dvbt2_tune_param_t * pTuneParam);

/**
 @brief Attempts to acquire to the channel at the center frequency provided. The
        system can be specified directly or set to ::SONY_DTV_SYSTEM_ANY to allow
        tuning to DVB-T or DVB-T2 for unknown cases.

        This function blocks the calling thread until the demod has locked or has 
        timed out. Use ::sony_integ_Cancel to cancel the operation at any time.

        For TS lock please call the wait TS lock function
        ::sony_integ_dvbt_t2_WaitTSLock.
 
        NOTE: For T2 the PLP selected will be the first found in the L1 pre signalling.
        Use ::sony_demod_dvbt2_monitor_DataPLPs to obtain a full list of PLPs contained
        in the T2 signal.
          
 @param pInteg The driver instance.
 @param centerFreqKHz The center frequency of the channel to attempt acquisition on
 @param bandwidth The bandwidth of the channel
 @param system The system to attempt to tune to, use ::SONY_DTV_SYSTEM_ANY to attempt
               both DVB-T and DVB-T2
 @param pSystemTuned The system of the channel located by the blind tune.

 @return SONY_RESULT_OK if tuned successfully to the channel.
*/
sony_result_t sony_integ_dvbt_t2_BlindTune(sony_integ_t * pInteg,
                                           uint32_t centerFreqKHz,
                                           sony_demod_bandwidth_t bandwidth,
                                           sony_dtv_system_t system,
                                           sony_dtv_system_t * pSystemTuned);

/**
 @brief Performs a scan over the spectrum specified. 

        The scan can perform a multiple system scan for DVB-T and DVB-T2 channels by 
        setting the ::sony_integ_dvbt_t2_scan_param_t::system to ::SONY_DTV_SYSTEM_ANY
        and setting the.

        Blocks the calling thread while scanning. Use ::sony_integ_Cancel to cancel 
        the operation at any time.
 
 @param pInteg The driver instance.
 @param pScanParam The scan parameters.
 @param callBack User registered call-back to receive scan progress information and 
        notification of found channels. The call back is called for every attempted 
        frequency during a scan.

 @return SONY_RESULT_OK if scan completed successfully.
        
*/
sony_result_t sony_integ_dvbt_t2_Scan(sony_integ_t * pInteg,
                                      sony_integ_dvbt_t2_scan_param_t * pScanParam,
                                      sony_integ_dvbt_t2_scan_callback_t callBack);

/**
 @brief Polls the demodulator waiting for TS lock at 10ms intervals up to a system dependant 
        timeout duration (DVB-T : 1s, DVB-T2 : 1.5s)

 @param pInteg The driver instance

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_integ_dvbt_t2_WaitTSLock (sony_integ_t * pInteg);

/**
 @brief This function returns the estimated RF level based on demodulator gain measurements
        and a tuner dependant conversion calculation.  The calculation provided in this monitor
        has been optimised for either Sony Evaluation Board PCB-0085-MV3.0 or PCB-0093-MV1.0, and 
        may require modifications for your own HW integration.  Please contact Sony support 
        for advice on characterising your own implementation.

 @param pInteg The driver instance
 @param pRFLeveldB The RF Level estimation in dB * 1000

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_integ_dvbt_t2_monitor_RFLevel (sony_integ_t * pInteg, int32_t * pRFLeveldB);

/**
 @brief DVB-T monitor for SSI (Signal Strength Indicator), based on the RF Level monitor value
        ::sony_integ_dvbt_t2_monitor_RFLevel.

 @note The RF Level monitor function should be optimised for your HW configuration before using
       this monitor.

 @param pInteg The driver instance
 @param pSSI The Signal Strength Indicator value in %

 @return SONY_RESULT_OK if successful
*/
sony_result_t sony_integ_dvbt_monitor_SSI (sony_integ_t * pInteg, uint8_t * pSSI);

/**
 @brief DVB-T2 monitor for SSI (Signal Strength Indicator), based on the RF Level monitor value
        ::sony_integ_dvbt_t2_monitor_RFLevel.

 @note The RF Level monitor function should be optimised for your HW configuration before using
       this monitor.

 @param pInteg The driver instance
 @param pSSI The Signal Strength Indicator value in %

 @return SONY_RESULT_OK if successful
*/
sony_result_t sony_integ_dvbt2_monitor_SSI (sony_integ_t * pInteg, uint8_t * pSSI);
#endif /* SONY_INTEG_DVBT_T2_H */
