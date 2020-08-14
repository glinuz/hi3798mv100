/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-08-10 10:34:51 #$
  File Revision : $Revision:: 5923 $
------------------------------------------------------------------------------*/
/**
 @file    sony_integ.h

          This file provides the integration layer control interface.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_INTEG_H
#define SONY_INTEG_H

#include "sony_demod.h"

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
#include "sony_tuner_terr_cable.h"
#endif

#include "sony_common.h"

/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The high level driver object.
        This is the primary interface used for controlling the demodulator and 
        connected tuner devices.

        This object is the combination of the demodulator and the tuner devices
        to represent a single front end system. It can be used wholly, partly or 
        just as a reference for developing the application software.
*/
typedef struct sony_integ_t {
    sony_demod_t * pDemod;                          /**< Instance of the demodulator. */
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    sony_tuner_terr_cable_t * pTunerTerrCable;      /**< The connected tuner for terrestrial and cable systems. */
#endif

    sony_atomic_t cancel;                           /**< Cancellation indicator variable. */

    void * user;                                    /**< User data. */
} sony_integ_t;

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
#if defined SONY_DRIVER_BUILD_OPTION_CXD2839 /* S/S2 only device */
/**
 @brief Construct the driver.

        This function is called by the application in order to setup the 
        ::sony_demod_t structure and provide references for the 
        ::sony_integ_t::pDemod, ::sony_integ_t::pTunerSat and 
        ::sony_integ_t::pLnbc members.

        This MUST be called before calling ::sony_integ_InitializeS.

@note Passing NULL as pTunerSat or pLnbc will disable communication
      with the device without throwing an arguement error.

        Notes on driver configuration:
        - By default, the demodulator uses an inverted AGC.  If the connected 
          tuner uses a non-inverted IFAGC, call ::sony_demod_SetConfig with 
          ::SONY_DEMOD_CONFIG_IFAGCNEG = 0 to setup the demodulator with postive 
          IFAGC sense, after calling ::sony_integ_InitializeS.
        - By default, the driver disables the RFAIN ADC and monitor on the device, 
          call ::sony_demod_SetConfig with ::SONY_DEMOD_CONFIG_RFAIN_ENABLE = 1
          to enable it, after calling ::sony_integ_InitializeS.

 @note  Memory is not allocated dynamically.

 @param pInteg The driver object to create. It must be a valid pointer to 
        allocated memory for a ::sony_integ_t structure.
 @param xtalFreq The frequency of the demod crystal.
 @param i2cAddress The demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.
 @param pDemod Reference to memory allocated for the demodulator instance. The 
        create function will setup this demodulator instance also.
 @param pTunerSat The tuner driver to use with this instance of the driver for 
        DVB-S / S2 systems. Note : The tuner I2C interface should have been setup 
        before this call.
 @param pLnbc The LNB control interface.
 
 @return SONY_RESULT_OK if successfully created integration layer driver structure.
*/
#else /* Devices not supporting satellite system */
/**
 @brief Construct the driver.

        This function is called by the application in order to setup the 
        ::sony_demod_t structure and provide references for the 
        ::sony_integ_t::pDemod and ::sony_integ_t::pTunerTerrCable members.

        This MUST be called before calling ::sony_integ_InitializeS or 
          ::sony_integ_InitializeT_C.

@note Passing NULL as pTunerTerrCable will disable communication
      with the device without throwing an arguement error.

        Notes on driver configuration:
        - By default, the demodulator uses an inverted AGC.  If the connected 
          tuner uses a non-inverted IFAGC, call ::sony_demod_SetConfig with 
          ::SONY_DEMOD_CONFIG_IFAGCNEG = 0 to setup the demodulator with postive 
          IFAGC sense, after calling ::sony_integ_InitializeS or 
          ::sony_integ_InitializeT_C.
        - By default, the driver disables the RFAIN ADC and monitor on the device, 
          call ::sony_demod_SetConfig with ::SONY_DEMOD_CONFIG_RFAIN_ENABLE = 1
          to enable it, after calling ::sony_integ_InitializeS or 
          ::sony_integ_InitializeT_C.

 @note  Memory is not allocated dynamically.

 @param pInteg The driver object to create. It must be a valid pointer to 
        allocated memory for a ::sony_integ_t structure.
 @param xtalFreq The frequency of the demod crystal.
 @param i2cAddress The demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.
 @param pDemod Reference to memory allocated for the demodulator instance. The 
        create function will setup this demodulator instance also.
 @param pTunerTerrCable The tuner driver to use with this instance of the driver 
        for DVB-T / T2 / C / C2 systems. Note : The tuner I2C interface should 
        have been setup before this call.
 
 @return SONY_RESULT_OK if successfully created integration layer driver structure.
*/
#endif
sony_result_t sony_integ_Create (sony_integ_t * pInteg,
                                 sony_demod_xtal_t xtalFreq,
                                 uint8_t i2cAddress,
                                 sony_i2c_t * pDemodI2c,
                                 sony_demod_t * pDemod
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
                                 ,sony_tuner_terr_cable_t * pTunerTerrCable
#endif
                                 );

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief Initialize the demodulator and terrestrial / cable tuner.  After this 
        operation, the demodulator and tuner are in a low power state (demod 
        state = ::SONY_DEMOD_STATE_SLEEP_T_C) awaiting tune or scan commands.

        Should only be used from Power On (::SONY_DEMOD_STATE_UNKNOWN).  Calling 
        from Active or Shutdown states will cause a demodulator reset, clearing
        all current configuration settings.

 @param pInteg The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_integ_InitializeT_C (sony_integ_t * pInteg);
#endif

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief Put the demodulator and tuner devices into a low power state for DVB-T, T2
        C and C2 systems.  This function can be called from SHUTDOWN or ACTIVE states.
        In ::SONY_DEMOD_STATE_SLEEP_T_C GPIO and demodulator configuration options are 
        available.  Calling this function from ::SONY_DEMOD_STATE_SLEEP_S will 
        reconfigure the demodulator for use with DVB terrestrial or cable systems.

 @param pInteg The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_integ_SleepT_C (sony_integ_t * pInteg);
#endif

/**
 @brief Shutdown the demodulator and tuner parts into a low power disabled state.
        This state can be used to manually transition between DVB-S / S2 and 
        DVB-T /T2 / C / C2 operating modes.  Call ::sony_integ_SleepS (DVB-S / S2)
        or ::sony_integ_SleepT_C (DVB-T /T2 / C / C2) dependant on the system 
        requirements of the applicaiton.  ::sony_integ_Shutdown can be directly 
        called from SLEEP or ACTIVE states.

 @param pInteg The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_integ_Shutdown (sony_integ_t * pInteg);

/**
 @brief Cancels current Tune or Scan operation in the demod and tuner parts.
        This function is thread safe, calling thread will get the result
        SONY_RESULT_ERROR_CANCEL.

 @param pInteg The driver instance.

 @return SONY_RESULT_OK if able to cancel the pending operation.
*/
sony_result_t sony_integ_Cancel (sony_integ_t * pInteg);

#endif /* SONY_INTEG_H */
