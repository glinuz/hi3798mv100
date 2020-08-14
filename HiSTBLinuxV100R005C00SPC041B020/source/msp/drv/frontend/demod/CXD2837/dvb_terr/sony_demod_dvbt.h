/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date: 2012-05-23 17:02:07 #$
  File Revision : $Revision: 5683 $
------------------------------------------------------------------------------*/
/**
 @file    sony_demod_dvbt.h

          This file provides the demodulator control interface specific to DVB-T.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DEMOD_DVBT_H
#define SONY_DEMOD_DVBT_H

#include "sony_common.h"
#include "sony_demod.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The tune parameters for a DVB-T signal
*/
typedef struct sony_dvbt_tune_param_t{
    uint32_t centerFreqKHz;             /**< Center frequency in kHz of the DVB-T channel */
    sony_demod_bandwidth_t bandwidth;   /**< Bandwidth of the DVB-T channel */
    sony_dvbt_profile_t profile;        /**< Indicates the HP/LP profile to be selected. */
}sony_dvbt_tune_param_t;

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
/**
 @brief Configure the DVBT profile for acquisition, Low Priority or High Priority.

 @param pDemod The demodulator instance
 @param profile The profile to use for DVB-T acquisition.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_SetProfile (sony_demod_t * pDemod,
                                          sony_dvbt_profile_t profile);

/**
 @brief Enable acquisition on the demodulator for DVB-T channels.  Called from
        the integration layer ::sony_integ_dvbt_Tune API.

 @param pDemod The demodulator instance
 @param pTuneParam The tune parameters.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_Tune (sony_demod_t * pDemod,
                                    sony_dvbt_tune_param_t * pTuneParam);

/**
 @brief Put the demodulator into ::SONY_DEMOD_STATE_SLEEP_T_C state.  Can be called
        from Active, Shutdown or Sleep states.  Called from the integration layer
        ::sony_integ_SleepT_C API.

 @param pDemod  The demodulator instance

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_Sleep (sony_demod_t * pDemod);

/**
 @brief Check DVB-T demodulator lock status.

 @param pDemod  The demodulator instance
 @param pLock Demod lock state

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_CheckDemodLock (sony_demod_t * pDemod, 
                                              sony_demod_lock_result_t * pLock);

/**
 @brief Check DVB-T TS lock status.

 @param pDemod  The demodulator instance
 @param pLock TS lock state

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_dvbt_CheckTSLock (sony_demod_t * pDemod, 
                                           sony_demod_lock_result_t * pLock);

#endif /* SONY_DEMOD_DVBT_H */
