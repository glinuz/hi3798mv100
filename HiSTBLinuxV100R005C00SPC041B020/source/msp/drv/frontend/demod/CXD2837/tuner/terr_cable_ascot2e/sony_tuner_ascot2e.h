/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-08-14 11:02:20 #$
  File Revision : $Revision:: 5934 $
------------------------------------------------------------------------------*/
/**

 @file    sony_tuner_ascot2e.h

          This file provides the DVB port of the Sony ASCOT2E tuner driver.

          This driver wraps around the Ascot2E driver provided by 
          sony_ascot2e.h by using an instance of the Ascot2E (sony_ascot2e_t) 
          driver in the ::sony_dvb_tuner_t::user pointer.
*/
/*----------------------------------------------------------------------------*/
#ifndef SONY_TUNER_ASCOT2E_H_
#define SONY_TUNER_ASCOT2E_H_

/*------------------------------------------------------------------------------
 Includes
------------------------------------------------------------------------------*/
#include "sony_tuner_terr_cable.h"
#include "sony_ascot2e.h"

/*------------------------------------------------------------------------------
 Driver Version
------------------------------------------------------------------------------*/
extern const char* sony_tuner_ascot2e_version;  /**< ASCOT2E driver version */

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define SONY_TUNER_ASCOT2E_OFFSET_CUTOFF_HZ         50000   /**< Maximum carrier offset frequency before requiring a retune */

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/**
 @brief Creates an instance of the ASCOT2E tuner driver
 
 @param pTuner The tuner driver instance to create. Memory 
        must have been allocated for this instance before 
        creation.
 @param xtalFreqMHz The crystal frequency of the tuner (MHz). 
        Supports 4MHz or 41MHz.
 @param i2cAddress The I2C address of the ASCOT2E device.
        Typically 0xC0.
 @param pI2c The I2C driver that the tuner driver will use for 
        communication.
 @param configFlags See ::SONY_ASCOT2E_CONFIG_EXT_REF,
            ::SONY_ASCOT2E_CONFIG_IFAGCSEL_ALL1,
            ::SONY_ASCOT2E_CONFIG_IFAGCSEL_ALL2,
            ::SONY_ASCOT2E_CONFIG_IFAGCSEL_A1D2,
            ::SONY_ASCOT2E_CONFIG_IFAGCSEL_D1A2,
            ::SONY_ASCOT2E_CONFIG_NVMSEL_AUTO,
            ::SONY_ASCOT2E_CONFIG_NVMSEL_0_1,
            ::SONY_ASCOT2E_CONFIG_NVMSEL_0_3 defined in 
            \link sony_ascot2e.h \endlink
 @param pAscot2ETuner The Ascot2E tuner driver pointer to use. 
        Memory must have been allocated for the Ascot2E driver structure. 
 
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_tuner_ascot2e_Create (sony_tuner_terr_cable_t * pTuner,
                                         uint32_t xtalFreqMHz,                                     
                                         uint8_t i2cAddress,
                                         sony_i2c_t * pI2c,
                                         uint32_t configFlags, 
                                         sony_ascot2e_t * pAscot2ETuner);

/**
 @brief RF filter compensation setting for VHF-L band.
 (Please see RFVGA Description of datasheet.)
 New setting will become effective after next tuning.

 mult = VL_TRCKOUT_COEFF(8bit unsigned) / 128
 ofs  = VL_TRCKOUT_OFS(8bit 2s complement)
 (compensated value) = (original value) * mult + ofs

 @param pTuner Instance of the tuner driver.
 @param coeff  VL_TRCKOUT_COEFF (multiplier)
 @param offset VL_TRCKOUT_OFS (additional term)

 @return SONY_RESULT_OK if successful.
 */
sony_result_t sony_tuner_ascot2e_RFFilterConfig (sony_tuner_terr_cable_t * pTuner, uint8_t coeff, uint8_t offset);

/**
 @brief Write to GPIO0 or GPIO1, pins 33 and 34 respectively.

 @param pTuner  Instance of the tuner driver.
 @param id      Pin ID 0 = GPIO0, 1 = GPIO1
 @param value   Output logic level, 0 = Low, 1 = High

 @return SONY_RESULT_OK if successful.
 */
sony_result_t sony_tuner_ascot2e_SetGPO (sony_tuner_terr_cable_t * pTuner, uint8_t id, uint8_t value);

/**
 @brief Read the IF and RF gain levels in dB from the tuner

 @param pTuner  Instance of the tuner driver.
 @param pIFGain IF gain of the tuner in dB * 100      
 @param pRFGain RF gain of the tuner in dB * 100
 @param forceRFAGCRead  If this flag is 1, read RFAGC value from the tuner,
                        so the return value will be accurate.
                        But reading RFAGC may affect to the IF signal quality.
                        If this flag is 0, previous RFAGC value saved
                        in the sony_ascot2e_t will be used.
                        RFAGC value is read only when previous value is not exist.

 @return SONY_RESULT_OK if successful.
 */
sony_result_t sony_tuner_ascot2e_ReadGain (sony_tuner_terr_cable_t * pTuner, int32_t * pIFGain, int32_t * pRFGain, uint8_t forceRFAGCRead);
#endif /* SONY_TUNER_ASCOT2E_H_ */
