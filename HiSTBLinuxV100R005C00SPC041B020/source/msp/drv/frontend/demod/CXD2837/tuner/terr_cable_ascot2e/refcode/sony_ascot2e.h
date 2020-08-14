/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : 2012/07/24
  File Revision : 1.0.3.0
------------------------------------------------------------------------------*/
/**
 @file    sony_ascot2e.h

          This file provides the ASCOT2E tuner control interface.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_ASCOT2E_H
#define SONY_ASCOT2E_H

#include "sony_common.h"
#include "sony_i2c.h"

/*------------------------------------------------------------------------------
  Defines
------------------------------------------------------------------------------*/
/**
 @brief Version of this driver.
*/
#define SONY_ASCOT2E_VERSION     "1.0.3.0"

/**
 @brief Default I2C slave address of the ASCOT2E tuner.
*/
#define SONY_ASCOT2E_ADDRESS     0xC0

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/**
 @brief ASCOT2E tuner state.
*/
typedef enum {
    SONY_ASCOT2E_STATE_UNKNOWN,   /**< ASCOT2E state is Unknown */
    SONY_ASCOT2E_STATE_SLEEP,     /**< ASCOT2E state is Sleep */
    SONY_ASCOT2E_STATE_ACTIVE     /**< ASCOT2E state is Active */
} sony_ascot2e_state_t;

/**
 @brief Analog/Digital terrestrial broadcasting system definitions supported by ASCOT2E.
*/
typedef enum {
    SONY_ASCOT2E_TV_SYSTEM_UNKNOWN,
    /* Analog */
    SONY_ASCOT2E_ATV_MN_EIAJ,  /**< System-M (Japan) */
    SONY_ASCOT2E_ATV_MN_SAP,   /**< System-M (US) */
    SONY_ASCOT2E_ATV_MN_A2,    /**< System-M (Korea) */
    SONY_ASCOT2E_ATV_BG,       /**< System-B/G */
    SONY_ASCOT2E_ATV_I,        /**< System-I */
    SONY_ASCOT2E_ATV_DK,       /**< System-D/K */
    SONY_ASCOT2E_ATV_L,        /**< System-L */
    SONY_ASCOT2E_ATV_L_DASH,   /**< System-L DASH */
    /* Digital */
    SONY_ASCOT2E_DTV_8VSB,     /**< ATSC 8VSB */
    SONY_ASCOT2E_DTV_QAM,      /**< US QAM */
    SONY_ASCOT2E_DTV_ISDBT_6,  /**< ISDB-T 6MHzBW */
    SONY_ASCOT2E_DTV_ISDBT_7,  /**< ISDB-T 7MHzBW */
    SONY_ASCOT2E_DTV_ISDBT_8,  /**< ISDB-T 8MHzBW */
    SONY_ASCOT2E_DTV_DVBT_5,   /**< DVB-T 5MHzBW */
    SONY_ASCOT2E_DTV_DVBT_6,   /**< DVB-T 6MHzBW */
    SONY_ASCOT2E_DTV_DVBT_7,   /**< DVB-T 7MHzBW */
    SONY_ASCOT2E_DTV_DVBT_8,   /**< DVB-T 8MHzBW */
    SONY_ASCOT2E_DTV_DVBT2_1_7,/**< DVB-T2 1.7MHzBW */
    SONY_ASCOT2E_DTV_DVBT2_5,  /**< DVB-T2 5MHzBW */
    SONY_ASCOT2E_DTV_DVBT2_6,  /**< DVB-T2 6MHzBW */
    SONY_ASCOT2E_DTV_DVBT2_7,  /**< DVB-T2 7MHzBW */
    SONY_ASCOT2E_DTV_DVBT2_8,  /**< DVB-T2 8MHzBW */
    SONY_ASCOT2E_DTV_DVBC,     /**< DVB-C */
    SONY_ASCOT2E_DTV_DVBC2_6,  /**< DVB-C2 6MHzBW */
    SONY_ASCOT2E_DTV_DVBC2_8,  /**< DVB-C2 8MHzBW */
    SONY_ASCOT2E_DTV_DTMB,     /**< DTMB */
    
    SONY_ASCOT2E_ATV_MIN = SONY_ASCOT2E_ATV_MN_EIAJ, /**< Minimum analog terrestrial system */
    SONY_ASCOT2E_ATV_MAX = SONY_ASCOT2E_ATV_L_DASH,  /**< Maximum analog terrestrial system */
    SONY_ASCOT2E_DTV_MIN = SONY_ASCOT2E_DTV_8VSB,    /**< Minimum digital terrestrial system */
    SONY_ASCOT2E_DTV_MAX = SONY_ASCOT2E_DTV_DTMB,    /**< Maximum digital terrestrial system */
    SONY_ASCOT2E_TV_SYSTEM_NUM                       /**< Number of supported broadcasting system */
} sony_ascot2e_tv_system_t;

/**
 @brief Macro to check that the system is analog terrestrial or not.
*/
#define SONY_ASCOT2E_IS_ATV(tvSystem) ((tvSystem >= SONY_ASCOT2E_ATV_MIN) && (tvSystem <= SONY_ASCOT2E_ATV_MAX))

/**
 @brief Macro to check that the system is digital terrestrial or not.
*/
#define SONY_ASCOT2E_IS_DTV(tvSystem) ((tvSystem >= SONY_ASCOT2E_DTV_MIN) && (tvSystem <= SONY_ASCOT2E_DTV_MAX))

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/

/**
 @brief ASCOT2E settings that may need to change depend on customer's system.
*/
typedef struct sony_ascot2e_adjust_param_t {
    uint8_t IF_OUT_SEL;   /**< Addr:0x05 Bit[2]   : IFOUT pin select (0: IFOUT1, 1: IFOUT2) 0xFF means Auto. (by config flags) */
    uint8_t AGC_SEL;      /**< Addr:0x05 Bit[4:3] : AGC pin select (0: AGC1, 1: AGC2) 0xFF means Auto (by config flags) */
    uint8_t MIX_OLL;      /**< Addr:0x0C Bit[2:0] : Internal RFAGC detection level */
    uint8_t RF_GAIN;      /**< Addr:0x0D Bit[5:0] : RFVGA gain. 0xFF means Auto (RF_GAIN_SEL = 1) */
    uint8_t IF_BPF_GC;    /**< Addr:0x0E Bit[2:0] : IF_BPF gain */
    uint8_t FIF_OFFSET;   /**< Addr:0x0E Bit[7:3] : 5bit signed. IF offset (kHz) = FIF_OFFSET x 50 */
    uint8_t BW_OFFSET;    /**< Addr:0x0F Bit[4:0] : 5bit signed. BW offset (kHz) = BW_OFFSET x 50 */
    uint8_t BW;           /**< Addr:0x12 Bit[5:4] : 6MHzBW(0x00) or 7MHzBW(0x01) or 8MHzBW(0x02) */
    uint8_t RF_OLDET;     /**< Addr:0x49 Bit[4:0] : RF_OLDET_OLL/RF_OLDET_ENX */
    uint8_t IF_BPF_F0;    /**< Addr:0x4A Bit[2:0] : IF filter center offset */
} sony_ascot2e_adjust_param_t;

/**
 @brief The ASCOT2E tuner definition which allows control of the ASCOT2E tuner device 
        through the defined set of functions.
*/
typedef struct sony_ascot2e_t {
    uint32_t                  xtalFreqMHz;  /**< Xtal frequency for ASCOT2E in MHz. */
    uint8_t                   i2cAddress;   /**< I2C slave address of the ASCOT2E tuner (8-bit form - 8'bxxxxxxx0) */
    sony_i2c_t*               pI2c;         /**< I2C API instance. */
    uint32_t                  flags;        /**< ORed value of SONY_ASCOT2E_CONFIG_XXXX */

    /* For saving current setting */
    sony_ascot2e_state_t      state;        /**< The driver operating state. */
    uint32_t                  frequencykHz; /**< Currently RF frequency(kHz) tuned. */
    sony_ascot2e_tv_system_t  tvSystem;     /**< Current broadcasting system tuned. */

    /* For sony_ascot2e_ReadGain */
    uint8_t                   rfagcreg;     /**< RFAGC value from register after tuning */
    uint8_t                   rfagcValid;   /**< rfagcreg is valid or not */

    /**
     @brief Adjustment parameter table (SONY_ASCOT2E_TV_SYSTEM_NUM size)
    */
    const sony_ascot2e_adjust_param_t *pParamTable;

    void*                     user;         /**< User defined data. */
} sony_ascot2e_t;

/*
 Config flag definitions. (ORed value should be set to flags argument of Create API.)
*/
#define SONY_ASCOT2E_CONFIG_EXT_REF         0x80000000  /**< Use external XTAL (Ref input). */

/**
 @name IF/AGC pin individually setting.
       Used if sony_ascot2e_adjust_param_t::IFAGCSEL == 0xFF
*/
/**@{*/
#define SONY_ASCOT2E_CONFIG_IF2_ATV         0x01000000  /**< IF2 is used for Analog. (Otherwise IF1 is used.) */
#define SONY_ASCOT2E_CONFIG_AGC2_ATV        0x02000000  /**< AGC2 is used for Analog. (Otherwise AGC1 is used.) */
#define SONY_ASCOT2E_CONFIG_IF2_DTV         0x04000000  /**< IF2 is used for Digital. (Otherwise IF1 is used.) */
#define SONY_ASCOT2E_CONFIG_AGC2_DTV        0x08000000  /**< AGC2 is used for Digital. (Otherwise AGC1 is used.) */
/**@}*/

/**
 @name IF/AGC pin setting for normal cases.
       Used if sony_ascot2e_adjust_param_t::IFAGCSEL == 0xFF
*/
/**@{*/
/**
 @brief IF/AGC 1 is used for both Analog and Digital
*/
#define SONY_ASCOT2E_CONFIG_IFAGCSEL_ALL1   0x00000000
/**
 @brief IF/AGC 2 is used for both Analog and Digital
*/
#define SONY_ASCOT2E_CONFIG_IFAGCSEL_ALL2   (SONY_ASCOT2E_CONFIG_IF2_ATV | SONY_ASCOT2E_CONFIG_AGC2_ATV | \
                                             SONY_ASCOT2E_CONFIG_IF2_DTV | SONY_ASCOT2E_CONFIG_AGC2_DTV)
/**
 @brief IF/AGC 1 is used for Analog, 2 is used for Digital
*/
#define SONY_ASCOT2E_CONFIG_IFAGCSEL_A1D2   (SONY_ASCOT2E_CONFIG_IF2_DTV | SONY_ASCOT2E_CONFIG_AGC2_DTV)
/**
 @brief IF/AGC 1 is used for Digital, 2 is used for Analog
*/
#define SONY_ASCOT2E_CONFIG_IFAGCSEL_D1A2   (SONY_ASCOT2E_CONFIG_IF2_ATV | SONY_ASCOT2E_CONFIG_AGC2_ATV)
/**@}*/

/*------------------------------------------------------------------------------
  APIs
------------------------------------------------------------------------------*/
/**
 @brief Set up the ASCOT2E tuner driver.

        This MUST be called before calling sony_ascot2e_Initialize.

 @param pTuner      Reference to memory allocated for the ASCOT2E instance.
                    The create function will setup this ASCOT2E instance.
 @param xtalFreqMHz The frequency of the ASCOT2E crystal.
 @param i2cAddress  The ASCOT2E tuner I2C slave address in 8-bit form.
 @param pI2c        The I2C APIs that the ASCOT2E driver will use as the I2C interface.
 @param flags       Configuration flags. It should be ORed value of SONY_ASCOT2E_CONFIG_XXXX.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_Create(sony_ascot2e_t *pTuner, uint32_t xtalFreqMHz,
    uint8_t i2cAddress, sony_i2c_t *pI2c, uint32_t flags);

/**
 @brief Initialize the ASCOT2E tuner.
        
        This MUST be called before calling sony_ascot2e_Tune.

 @param pTuner       The ASCOT2E tuner instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_Initialize(sony_ascot2e_t *pTuner);

/**
 @brief Tune to a given RF frequency with broadcasting system.

        To complete tuning, sony_ascot2e_TuneEnd should be called after waiting 50ms.

 @param pTuner       The ASCOT2E tuner instance.
 @param frequencykHz RF frequency to tune. (kHz)
 @param tvSystem     The type of broadcasting system to tune.

 @return SONY_RESULT_OK if tuner successfully tuned.
*/
sony_result_t sony_ascot2e_Tune(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem);

/**
 @brief Completes the ASCOT2E tuning sequence.

        This MUST be called after calling sony_ascot2e_Tune and 50ms wait.

 @param pTuner       The ASCOT2E tuner instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_TuneEnd(sony_ascot2e_t *pTuner);

/**
 @brief Shift RF frequency.

        This API shift RF frequency without VCO calibration.
        This API is normally useful for analog TV "fine tuning" that
        shift RF frequency without picture distortion.
        NOTE: Please check the frequency range that VCO calibration is unnecessary.
              (See hardware specification sheet.)

 @param pTuner       The ASCOT2E tuner instance.
 @param frequencykHz RF frequency to tune. (kHz)

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_ShiftFRF(sony_ascot2e_t *pTuner, uint32_t frequencykHz);

/**
 @brief Put the ASCOT2E tuner into Sleep state.
 
        From this state the tuner can be directly tuned by calling sony_ascot2e_Tune.

 @param pTuner       The ASCOT2E tuner instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_Sleep(sony_ascot2e_t *pTuner);

/**
 @brief Write to GPO0 or GPO1.

 @param pTuner       The ASCOT2E tuner instance.
 @param id           Pin ID 0 = GPO0, 1 = GPO1
 @param val          Output logic level, 0 = Low, 1 = High

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_SetGPO(sony_ascot2e_t *pTuner, uint8_t id, uint8_t val);

/**
 @brief Set the RF external circuit control pin.
        (Set RF_EXT_CTRL register.)

 @param pTuner       The ASCOT2E tuner instance.
 @param enable       The value to set. (Enable(1) or Disable(0))

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_SetRfExtCtrl(sony_ascot2e_t *pTuner, uint8_t enable);

/**
 @brief Read the IF and RF gain levels in dB from the tuner.

 @param pTuner          The ASCOT2E tuner instance.
 @param pIFGain         IF gain of the tuner in dB * 100
 @param pRFGain         RF gain of the tuner in dB * 100
 @param forceRFAGCRead  If this flag is 1, read RFAGC value from the tuner,
                        so the return value will be accurate.
                        But reading RFAGC may affect to the IF signal quality.
                        If this flag is 0, previous RFAGC value saved
                        in the sony_ascot2e_t will be used.
                        RFAGC value is read only when previous value is not exist.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_ReadGain(sony_ascot2e_t *pTuner, int32_t *pIFGain, int32_t *pRFGain, uint8_t forceRFAGCRead);

/**
 @brief RF filter compensation setting for VHF-L band.
        (Please see RFVGA Description of datasheet.)
        New setting will become effective after next tuning.

        mult = VL_TRCKOUT_COEFF(8bit unsigned) / 128
        ofs  = VL_TRCKOUT_OFS(8bit 2s complement)
        (compensated value) = (original value) * mult + ofs

 @param pTuner       The ASCOT2E tuner instance.
 @param coeff        VL_TRCKOUT_COEFF (multiplier)
 @param offset       VL_TRCKOUT_OFS (additional term)

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_ascot2e_RFFilterConfig(sony_ascot2e_t *pTuner, uint8_t coeff, uint8_t offset);

#endif /* SONY_ASCOT2E_H */
