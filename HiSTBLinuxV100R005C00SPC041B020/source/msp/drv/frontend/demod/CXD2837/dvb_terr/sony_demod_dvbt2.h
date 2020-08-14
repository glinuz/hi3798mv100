/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-06-28 18:03:35 #$
  File Revision : $Revision:: 5683 $
------------------------------------------------------------------------------*/
/**
 @file    sony_demod_dvbt2.h

          This file provides the demodulator control interface specific to DVB-T2.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DEMOD_DVBT2_H
#define SONY_DEMOD_DVBT2_H

#include "sony_common.h"
#include "sony_demod.h"

/*------------------------------------------------------------------------------
 Enumerations
------------------------------------------------------------------------------*/
/**
 @brief DVBT2 specific tune information, stored in the tune param struct
        result.  This should be checked if a call to DVBT2 tune returns
        SONY_RESULT_OK_CONFIRM.
*/
typedef enum {
    /**
     @brief Tune successful.
    */
    SONY_DEMOD_DVBT2_TUNE_INFO_OK,

    /**
     @brief PLP provided in tune params is not available.  The demodulator
            will output the auto PLP in this case.
    */
    SONY_DEMOD_DVBT2_TUNE_INFO_INVALID_PLP_ID,

    /**
     @brief The T2 mode provided in the tune params is not available.  The
            demodulator has selected the mode automatically.
    */
    SONY_DEMOD_DVBT2_TUNE_INFO_INVALID_T2_MODE
} sony_demod_dvbt2_tune_info_t;

/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The tune parameters for a DVB-T2 signal
*/
typedef struct sony_dvbt2_tune_param_t{
    /**
     @brief Center frequency in kHz of the DVB-T2 channel.
    */
    uint32_t centerFreqKHz; 
    
    /**
     @brief Bandwidth of the DVB-T2 channel.
    */
    sony_demod_bandwidth_t bandwidth;
    
    /**
     @brief The data PLP ID to select in acquisition.
    */    
    uint8_t dataPlpId; 
    
    /**
     @brief Specific tune information relating to DVB-T2 acquisition.  If result
            from Tune function is SONY_RESULT_OK_CONFIRM this result code
            will provide more information on the tune process.  Refer to 
            ::sony_demod_dvbt2_tune_info_t for further details on the specific
            codes.
    */
    sony_demod_dvbt2_tune_info_t tuneInfo;   
}sony_dvbt2_tune_param_t;

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
/**
 @brief Enable acquisition on the demodulator for DVB-T2 channels.  Called from
        the integration layer ::sony_integ_dvbt2_Tune API.

 @param pDemod The demodulator instance
 @param pTuneParam The tune parameters.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt2_Tune (sony_demod_t * pDemod,
                                     sony_dvbt2_tune_param_t * pTuneParam);

/**
 @brief Put the demodulator into ::SONY_DEMOD_STATE_SLEEP_T_C state.  Can be called
        from Active, Shutdown or Sleep states.  Called from the integration layer
        ::sony_integ_SleepT_C API.

 @param pDemod The demodulator instance

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt2_Sleep (sony_demod_t * pDemod);

/**
 @brief Check DVB-T demodulator lock status.

 @param pDemod The demodulator instance
 @param pLock Demod lock state

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt2_CheckDemodLock (sony_demod_t * pDemod, 
                                               sony_demod_lock_result_t * pLock);

/**
 @brief Check DVB-T TS lock status.

 @param pDemod The demodulator instance
 @param pLock TS lock state

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt2_CheckTSLock (sony_demod_t * pDemod, 
                                            sony_demod_lock_result_t * pLock);

/**
 @brief Setup the PLP configuration of the demodulator. Selecting both the device 
        PLP operation (automatic/manual PLP select) and the PLP to be selected
        in manual PLP mode.

 @param pDemod The demodulator instance.
 @param autoPLP The auto PLP setting.
        - 0x00: Fully automatic. The first PLP found during acquisition will be output.
        - 0x01: The data PLP ID set by plpId will be output.
               If the PLP with the ID is not found, then a PLP error is indicated
               (::sony_demod_dvbt2_monitor_DataPLPError) but the 
               demod will still output the first found data PLP Id.
 @param plpId The PLP Id to set.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt2_SetPLPConfig (sony_demod_t * pDemod, 
                                             uint8_t autoPLP, 
                                             uint8_t plpId);

#endif /* SONY_DEMOD_DVBT2_H */
