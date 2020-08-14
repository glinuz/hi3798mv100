/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-05-22 17:14:49 #$
  File Revision : $Revision:: 5275 $
------------------------------------------------------------------------------*/
/**
 @file    sony_integ_dvbc.h

          This file provides the integration layer interface for DVB-C specific
          demodulator functions.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_INTEG_DVBC_H
#define SONY_INTEG_DVBC_H

#include "sony_demod.h"
#include "sony_integ.h"
#include "sony_demod_dvbc.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define SONY_DVBC_WAIT_DEMOD_LOCK           1000    /**< 1s timeout for wait demodulator lock */
#define SONY_DVBC_WAIT_LOCK_INTERVAL        10      /**< 10ms interval for demodulator lock polls */

/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The parameters used for DVB-C scanning.
*/
typedef struct sony_integ_dvbc_scan_param_t{
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
}sony_integ_dvbc_scan_param_t;

/**
 @brief The structure used to return a channel located or progress update 
        as part of a DVB-C scan.
*/
typedef struct sony_integ_dvbc_scan_result_t{
    /**
     @brief Indicates the current frequency just attempted for the scan.  This would
            primarily be used to calculate scan progress from the scan parameters.
    */
    uint32_t centerFreqKHz;

    /**
     @brief Indicates if the tune result at the current frequency.  SONY_RESULT_OK
            means that a channel is locked and the tuneParam structure contains 
            the channel infomration.
    */
    sony_result_t tuneResult;

    /**
     @brief The tune params for located DVB-C channel.
    */
    sony_dvbc_tune_param_t tuneParam;
}sony_integ_dvbc_scan_result_t;

/*------------------------------------------------------------------------------
 Function Pointers
------------------------------------------------------------------------------*/
/**
 @brief Callback function that is called for every attempted frequency during a DVB-C
        scan.  For successful channel results the function is called after TS lock
        is achieved.
 
 @param pInteg The driver instance.
 @param pResult The current scan result.
 @param pScanParam The current scan parameters.
*/
typedef void (*sony_integ_dvbc_scan_callback_t) (sony_integ_t * pInteg, 
                                                 sony_integ_dvbc_scan_result_t * pResult,
                                                 sony_integ_dvbc_scan_param_t * pScanParam);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
/**
 @brief Performs acquisition to the channel. 
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::sony_integ_Cancel to cancel the operation at any time.
          
 @param pInteg The driver instance.
 @param pTuneParam The parameters required for the tune.

 @return SONY_RESULT_OK if tuned successfully to the channel.
*/
sony_result_t sony_integ_dvbc_Tune(sony_integ_t * pInteg,
                                   sony_dvbc_tune_param_t * pTuneParam);

/**
 @brief Performs a scan over the spectrum specified. 

        Blocks the calling thread while scanning. Use ::sony_integ_Cancel to cancel 
        the operation at any time.
 
 @param pInteg The driver instance.
 @param pScanParam The scan parameters.
 @param callBack User registered call-back to receive scan progress information and 
        notification of found channels. The call back is called for every attempted 
        frequency during a scan.

 @return SONY_RESULT_OK if scan completed successfully.
        
*/
sony_result_t sony_integ_dvbc_Scan(sony_integ_t * pInteg,
                                   sony_integ_dvbc_scan_param_t * pScanParam,
                                   sony_integ_dvbc_scan_callback_t callBack);

/**
 @brief Polls the demodulator waiting for TS lock over a maximum of 1s at 10ms intervals

 @param pInteg The driver instance

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_integ_dvbc_WaitTSLock (sony_integ_t * pInteg);

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
sony_result_t sony_integ_dvbc_monitor_RFLevel (sony_integ_t * pInteg, int32_t * pRFLeveldB);
#endif /* SONY_INTEG_DVBC_H */
