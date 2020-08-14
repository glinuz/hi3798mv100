/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-18 16:25:06 #$
  File Revision : $Revision:: 6115 $
------------------------------------------------------------------------------*/
/**
 @file    sony_demod.h

          This file provides the common demodulator control interface.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DEMOD_H
#define SONY_DEMOD_H

/*------------------------------------------------------------------------------
  Device Defines based on pre-compiler BUILD_OPTION
------------------------------------------------------------------------------*/
#define SONY_DRIVER_BUILD_OPTION_CXD2837

#if defined SONY_DRIVER_BUILD_OPTION_CXD2837   /* DVB-T/T2/C Demodulator */
#define SONY_DEMOD_SUPPORT_TERR_OR_CABLE       /**< Driver supports a Terrestrial or Cable system. */
#elif defined SONY_DRIVER_BUILD_OPTION_CXD2839 /* DVB-S/S2 Demodulator */
#define SONY_DEMOD_SUPPORT_DVBS_S2             /**< Driver supports DVBS and S2. */
#elif defined SONY_DRIVER_BUILD_OPTION_CXD2841 /* DVB-T/T2/C/C2/S/S2 Demodulator */
#define SONY_DEMOD_SUPPORT_DVBC2               /**< Driver supports DVBC2. */
#define SONY_DEMOD_SUPPORT_DVBS_S2             /**< Driver supports DVBS and S2. */
#define SONY_DEMOD_SUPPORT_TERR_OR_CABLE       /**< Driver supports a Terrestrial or Cable system. */
#elif defined SONY_DRIVER_BUILD_OPTION_CXD2842 /* DVB-T/T2/C/S/S2 Demodulator */
#define SONY_DEMOD_SUPPORT_DVBS_S2             /**< Driver supports DVBS and S2. */
#define SONY_DEMOD_SUPPORT_TERR_OR_CABLE       /**< Driver supports a Terrestrial or Cable system. */
#elif defined SONY_DRIVER_BUILD_OPTION_CXD2843 /* DVB-T/T2/C/C2 Demodulator */
#define SONY_DEMOD_SUPPORT_DVBC2               /**< Driver supports DVBC2. */
#define SONY_DEMOD_SUPPORT_TERR_OR_CABLE       /**< Driver supports a Terrestrial or Cable system. */
#else
#error SONY_DRIVER_BUILD_OPTION value not recognised
#endif

/*------------------------------------------------------------------------------
  Includes
------------------------------------------------------------------------------*/
#include "sony_common.h"
#include "sony_i2c.h"
#include "sony_dtv.h"

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
#include "sony_dvbt.h"
#include "sony_dvbt2.h"
#include "sony_dvbc.h"
#endif

#ifdef SONY_DEMOD_SUPPORT_DVBC2
#include "sony_dvbc2.h"
#endif


/*------------------------------------------------------------------------------
  Defines
------------------------------------------------------------------------------*/
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief Calculate the demodulator IF Freq setting ::sony_demod_t::iffreqConfig.
        ((IFFREQ/Sampling Freq at Down Converter DSP module) * Down converter's dynamic range + 0.5                
*/
#define SONY_DEMOD_MAKE_IFFREQ_CONFIG(iffreq) ((uint32_t)(((iffreq)/41.0)*16777216.0 + 0.5))
#endif

#define SONY_DEMOD_MAX_TS_CLK_KHZ   10250   /**< Maximum allowed manual TS clock rate setting in parallel mode. Do not change. */

#define SONY_DEMOD_MAX_CONFIG_MEMORY_COUNT 100 /**< The maximum number of entries in the configuration memory table */

/**
 @brief Freeze all registers in the SLV-T device.  This API is used by the monitor functions to ensure multiple separate 
        register reads are from the same snapshot 

 @note This should not be manually called or additional instances added into the driver unless under specific instruction.
*/
#define SLVT_FreezeReg(pDemod) ((pDemod)->pI2c->WriteOneRegister ((pDemod)->pI2c, (pDemod)->i2cAddressSLVT, 0x01, 0x01))

/**
 @brief Unfreeze all registers in the SLV-T device 
*/
#define SLVT_UnFreezeReg(pDemod) ((void)((pDemod)->pI2c->WriteOneRegister ((pDemod)->pI2c, (pDemod)->i2cAddressSLVT, 0x01, 0x00)))

/*------------------------------------------------------------------------------
  Enumerations
------------------------------------------------------------------------------*/
/**
 @brief Demodulator crystal frequency.
*/
typedef enum {
    SONY_DEMOD_XTAL_16000KHz,           /**< 16 MHz */
    SONY_DEMOD_XTAL_20500KHz,           /**< 20.5 MHz */
    SONY_DEMOD_XTAL_24000KHz,           /**< 24 MHz */
    SONY_DEMOD_XTAL_41000KHz            /**< 41 MHz */
} sony_demod_xtal_t;

/**
 @brief The demodulator Chip ID mapping.
*/
typedef enum {
    SONY_DEMOD_CHIP_ID_CXD2837 = 0xB1,  /**< CXD2837  DVB-T/T2/C */
    SONY_DEMOD_CHIP_ID_CXD2839 = 0xA5,  /**< CXD2839  DVB-S/S2 */
    SONY_DEMOD_CHIP_ID_CXD2841 = 0xA7,  /**< CXD2841  DVB-T/T2/C/C2/S/S2 */
    SONY_DEMOD_CHIP_ID_CXD2842 = 0xA5,  /**< CXD2842  DVB-T/T2/C/S/S2 */
    SONY_DEMOD_CHIP_ID_CXD2843 = 0xA4,  /**< CXD2843  DVB-T/T2/C/C2 */
} sony_demod_chip_id_t;

/**
 @brief Demodulator software state.
*/
typedef enum {
    SONY_DEMOD_STATE_UNKNOWN,           /**< Unknown. */
    SONY_DEMOD_STATE_SHUTDOWN,          /**< Chip is in Shutdown state */
    SONY_DEMOD_STATE_SLEEP_T_C,         /**< Chip is in Sleep state for DVB-T / T2 / C / C2 */
    SONY_DEMOD_STATE_SLEEP_S,           /**< Chip is in Sleep state for DVB-S / S2 */
    SONY_DEMOD_STATE_ACTIVE_T_C,        /**< Chip is in Active state for DVB-T / T2 / C / C2 */
    SONY_DEMOD_STATE_ACTIVE_S,          /**< Chip is in Active state for DVB-S / S2 */
    SONY_DEMOD_STATE_INVALID            /**< Invalid, result of an error during a state change. */
} sony_demod_state_t;

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief System bandwidth.
*/
typedef enum {
    SONY_DEMOD_BW_UNKNOWN = 0,          /**< Unknown bandwidth */
    SONY_DEMOD_BW_1_7_MHZ = 1,          /**< 1.7MHz bandwidth (Valid option for DVB-T2 only) */
    SONY_DEMOD_BW_5_MHZ = 5,            /**< 5MHz bandwidth (Valid option for DVB-T / T2) */
    SONY_DEMOD_BW_6_MHZ = 6,            /**< 6MHz bandwidth (Valid option for DVB-T / T2 / C2) */
    SONY_DEMOD_BW_7_MHZ = 7,            /**< 7MHz bandwidth (Valid option for DVB-T / T2) */
    SONY_DEMOD_BW_8_MHZ = 8             /**< 8MHz bandwidth (Valid option for DVB-T / T2 / C / C2) */
} sony_demod_bandwidth_t;

/**
 @brief Enumeration of supported sony tuner models used for optimising the 
        demodulator configuration.
*/
typedef enum {
    SONY_DEMOD_TUNER_OPTIMIZE_UNKNOWN,  /**< Non-Sony Tuner. */
    SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D,  /**< Sony ASCOT2D derived tuners. */
    SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E,  /**< Sony ASCOT2E derived tuners. */
    SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR  /**< Sony ASCOT2XR derived tuners. */
} sony_demod_tuner_optimize_t;

/**
 @brief Enumeration of spectrum inversion monitor values.
*/
typedef enum {
    SONY_DEMOD_TERR_CABLE_SPECTRUM_NORMAL = 0,             /**< Spectrum normal sense. */
    SONY_DEMOD_TERR_CABLE_SPECTRUM_INV                     /**< Spectrum inverted. */
} sony_demod_terr_cable_spectrum_sense_t;
#endif

/**
 @brief Configuration options for the demodulator.
*/
typedef enum {
    /**
     @brief Parallel or serial TS output selection.

            Value:
            - 0: Serial output.
            - 1: Parallel output (Default).
    */
    SONY_DEMOD_CONFIG_PARALLEL_SEL,

    /**
     @brief Serial output pin of TS data.

            Value:
            - 0: Output from TSDATA0
            - 1: Output from TSDATA7 (Default).
    */
    SONY_DEMOD_CONFIG_SER_DATA_ON_MSB,

    /**
     @brief Parallel/Serial output bit order on TS data.

            Value (Parallel):
            - 0: MSB TSDATA[0]
            - 1: MSB TSDATA[7] (Default).
            Value (Serial):
            - 0: LSB first
            - 1: MSB first (Default).
    */
    SONY_DEMOD_CONFIG_OUTPUT_SEL_MSB,

    /**
     @brief TS valid active level.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).
    */
    SONY_DEMOD_CONFIG_TSVALID_ACTIVE_HI,

    /**
     @brief TS sync active level.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).
    */
    SONY_DEMOD_CONFIG_TSSYNC_ACTIVE_HI,

    /**
     @brief TS error active level.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).
    */
    SONY_DEMOD_CONFIG_TSERR_ACTIVE_HI,

    /**
     @brief TS clock inversion setting.

            Value:
            - 0: Falling/Negative edge.
            - 1: Rising/Positive edge (Default).
    */
    SONY_DEMOD_CONFIG_LATCH_ON_POSEDGE,

    /**
     @brief Serial TS clock gated on valid TS data or is continuous.
            Value is stored in demodulator structure to be applied during Sleep to Active
            transition.  

            Value:
            - 0: Gated
            - 1: Continuous (Default)
    */
    SONY_DEMOD_CONFIG_TSCLK_CONT,

    /**
     @brief Disable/Enable TS clock during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active (default)
            - 1 : Disable during TS packet gap
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    SONY_DEMOD_CONFIG_TSCLK_MASK,

    /**
     @brief Disable/Enable TSVALID during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active 
            - 1 : Disable during TS packet gap (default)
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    SONY_DEMOD_CONFIG_TSVALID_MASK,
    
    /**
     @brief Disable/Enable TSERR during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active (default)
            - 1 : Disable during TS packet gap
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    SONY_DEMOD_CONFIG_TSERR_MASK,

    /**
     @brief Configure the driving current for the TS Clk pin.

            - 0 : 8mA (Default)
            - 1 : 10mA
    */
    SONY_DEMOD_CONFIG_TSCLK_CURRENT_10mA,

    /**
     @brief Configure the driving current for the TS Sync / TS Valid 
            / TS Data / TS Error pins.

            - 0 : 8mA (Default)
            - 1 : 10mA
    */
    SONY_DEMOD_CONFIG_TS_CURRENT_10mA,

    /**
     @brief This configuration can be used to configure the demodulator to output a TS waveform that is
            backwards compatible with previous generation demodulators (CXD2834 / CXD2835 / CXD2836). 
            This option should not be used unless specifically required to overcome a HW configuration issue.

            The demodulator will have the following settings, which will override any prior individual 
            configuration:
            - Disable TS packet gap insertion.
            - Parallel TS maximum bit rate of 82MBps
            - Serial TS clock frequency fixed at 82MHz
            
            Values:
            - 0 : Backwards compatible mode disabled
            - 1 : Backwards compatible mode enabled
    */
    SONY_DEMOD_CONFIG_TS_BACKWARDS_COMPATIBLE,

    /**
    @brief Writes a 12-bit value to the PWM output.
        Please note the actual PWM precision.
        - 12-bit.
        0xFFF => DVDD
        0x000 => GND
    */
    SONY_DEMOD_CONFIG_PWM_VALUE,

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
     /**
     @brief Set the TS clock rate (data period) manually.
            *NOT recommended for normal use (testing/debug only).*
            If SONY_DEMOD_CONFIG_PARALLEL_SEL = 0 (serial TS), then this configuration will
            have no effect.
            If SONY_DEMOD_CONFIG_TS_AUTO_RATE_ENABLE = 1, then this configuration will have no
            effect.
            Value (DVB-T/T2):
            - Desired TS clock rate (data period) in kHz.
            Value (DVB-C):
            - 1/2 desired TS clock rate (data period) in kHz.
    */
    SONY_DEMOD_CONFIG_TSIF_SDPR,

    /**
     @brief Enable or disable the auto TS clock rate (data period).
            *NOT recommended for normal use (testing/debug only).*
            If DEMOD_CONFIG_PARALLEL_SEL = 0 (serial TS), then this configuration will
            have no effect.
            Value:
            - 0: Disable the TS auto rate.
                 TS clock rate = SONY_DEMOD_CONFIG_TSIF_SDPR.
            - 1: Enable the TS auto rate (Default).
                 TS clock rate is automatic.

            @note Not available in DVB-C mode.
    */
    SONY_DEMOD_CONFIG_TS_AUTO_RATE_ENABLE,

    /**
     @brief Configure the clock frequency for Serial TS in terrestrial and cable active states.
            Value is stored in demodulator structure to be applied during Sleep to Active
            transition.  
            Only valid when SONY_DEMOD_CONFIG_PARALLEL_SEL = 0 (serial TS).

            - 0 : Invalid
            - 1 : 82.00MHz (Default)
            - 2 : 65.60MHz
            - 3 : 54.67MHz
            - 4 : 41.00MHz 
            - 5 : 32.80MHz
    */
    SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ,

    /**
     @brief IFAGC sense configuration.

            Value:
            - 0: Positive IFAGC.
            - 1: Inverted IFAGC (Default)
    */
    SONY_DEMOD_CONFIG_IFAGCNEG,

    /**
     @brief Configure the full-scale range of the ADC input to the IFAGC.

            Value:
            - 0: 1.4Vpp (Default)
            - 1: 1.0Vpp
            - 2: 0.7Vpp
    */
    SONY_DEMOD_CONFIG_IFAGC_ADC_FS,

    /**
     @brief Spectrum inversion configuration for the terrestrial / cable tuner. 
            
            Value: 
            - 0: Normal (Default).
            - 1: Inverted.
    */
    SONY_DEMOD_CONFIG_SPECTRUM_INV,

    /**
     @brief RFAIN ADC enable/disable.
            Must be asleep for this setting to take effect on next tune/scan.
            Use ::sony_demod_terr_cable_monitor_RFAIN to monitor the input.

            Value:
            - 0: Disable RFAIN ADC and monitor (Default).
            - 1: Enable RFAIN ADC and monitor.
    */
    SONY_DEMOD_CONFIG_RFAIN_ENABLE,

    /**
     @brief Set the measurment period for Pre-RS BER (DVB-T/C).

            This is a 5 bit value with a default of 14.
    */
    SONY_DEMOD_CONFIG_DVBTC_BERN_PERIOD,

    /**
     @brief Set the measurment period for Pre-Viterbi BER (DVB-T).

            This is a 3 bit value with a default of 1.
    */
    SONY_DEMOD_CONFIG_DVBT_VBER_PERIOD,

    /**
     @brief Set the measurment period for Pre-BCH BER (DVB-T2/C2) and 
            Post-BCH FER (DVB-T2/C2).

            This is a 4 bit value with a default of 8.
    */
    SONY_DEMOD_CONFIG_DVBT2C2_BBER_MES,

    /**
     @brief Set the measurment period for Pre-LDPC BER (DVB-T2/C2).

            This is a 4 bit value with a default of 8.
    */
    SONY_DEMOD_CONFIG_DVBT2C2_LBER_MES,

    /**
     @brief Set the measurment period for PER (DVB-T/C).

            This is a 4 bit value with a default of 10.
    */
    SONY_DEMOD_CONFIG_DVBTC_PER_MES,

    /**
     @brief Set the measurment period for PER (DVB-T2/C2).

            This is a 4 bit value with a default of 10.
    */
    SONY_DEMOD_CONFIG_DVBT2C2_PER_MES,
#endif
} sony_demod_config_id_t;

/**
 @brief Demodulator lock status.
*/
typedef enum {
    SONY_DEMOD_LOCK_RESULT_NOTDETECT, /**< Neither "Lock" or "Unlock" conditions are met, lock status cannot be determined */
    SONY_DEMOD_LOCK_RESULT_LOCKED,    /**< "Lock" condition is found. */
    SONY_DEMOD_LOCK_RESULT_UNLOCKED   /**< No signal was found or the signal was not the required system. */
} sony_demod_lock_result_t;

/**
 @brief Mode select for the multi purpose GPIO pins
*/
typedef enum {
    /** @brief GPIO pin is configured as an output */
    SONY_DEMOD_GPIO_MODE_OUTPUT = 0x00, 

    /** @brief GPIO pin is configured as an input */
    SONY_DEMOD_GPIO_MODE_INPUT = 0x01, 
        
    /** 
     @brief GPIO pin is configured to output an PWM signal which can be configured using the 
            ::sony_demod_SetConfig function with the config ID ::SONY_DEMOD_CONFIG_PWM_VALUE.
    */
    SONY_DEMOD_GPIO_MODE_PWM = 0x03, 

    /** @brief GPIO pin is configured to output TS error */
    SONY_DEMOD_GPIO_MODE_TS_ERROR = 0x04, 

    /** @brief GPIO pin is configured to output the FEF timing indicator (DVB-T2 Only) */
    SONY_DEMOD_GPIO_MODE_FEF_PART = 0x05, 

    /** @brief GPIO pin is configured to output DiSEqC Transmit Enable */
    SONY_DEMOD_GPIO_MODE_DISEQC_TX_EN = 0x08, 

    /** @brief GPIO pin is configured to output DiSEqC Receive Enable */
    SONY_DEMOD_GPIO_MODE_DISEQC_RX_EN = 0x09
} sony_demod_gpio_mode_t;

/**
 @brief TS serial clock frequency options
*/
typedef enum {
    SONY_DEMOD_SERIAL_TS_CLK_HIGH_FULL,   /** High frequency, full rate */
    SONY_DEMOD_SERIAL_TS_CLK_MID_FULL,    /** Mid frequency, full rate */
    SONY_DEMOD_SERIAL_TS_CLK_LOW_FULL,    /** Low frequency, full rate */
    SONY_DEMOD_SERIAL_TS_CLK_HIGH_HALF,   /** High frequency, half rate */
    SONY_DEMOD_SERIAL_TS_CLK_MID_HALF,    /** Mid frequency, half rate */
    SONY_DEMOD_SERIAL_TS_CLK_LOW_HALF     /** Low frequency, half rate */
} sony_demod_serial_ts_clk_t ;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief List of register values for IF frequency configuration.  Used for handling 
        tuners that output a different IF depending on the expected channel BW.
        Should be set using ::SONY_DEMOD_MAKE_IFFREQ_CONFIG macro.
*/
typedef struct {
    uint32_t configDVBT_5;              /**< DVB-T 5MHz */
    uint32_t configDVBT_6;              /**< DVB-T 6MHz */
    uint32_t configDVBT_7;              /**< DVB-T 7MHz */
    uint32_t configDVBT_8;              /**< DVB-T 8MHz */
    uint32_t configDVBT2_1_7;           /**< DVB-T2 1.7MHz */
    uint32_t configDVBT2_5;             /**< DVB-T2 5MHz */
    uint32_t configDVBT2_6;             /**< DVB-T2 6MHz */
    uint32_t configDVBT2_7;             /**< DVB-T2 7MHz */
    uint32_t configDVBT2_8;             /**< DVB-T2 8MHz */
    uint32_t configDVBC2_6;             /**< DVB-C2 6MHz */
    uint32_t configDVBC2_8;             /**< DVB-C2 8MHz */
    uint32_t configDVBC;                /**< DVB-C */
} sony_demod_iffreq_config_t;
#endif
/**
 @brief The demodulator configuration memory table entry. Used to store a register or
        bit modification made through either the ::sony_demod_SetConfig or 
        ::sony_demod_SetAndSaveRegisterBits APIs.
*/
typedef struct {
    uint8_t slaveAddress;               /**< Slave address of register */
    uint8_t bank;                       /**< Bank for register */
    uint8_t registerAddress;            /**< Register address */
    uint8_t value;                      /**< Value to write to register */
    uint8_t bitMask;                    /**< Bit mask to apply on the value */
} sony_demod_config_memory_t;

/**
 @brief The demodulator definition which allows control of the demodulator device 
        through the defined set of functions. This portion of the driver is seperate 
        from the tuner portion and so can be operated independently of the tuner.
*/
typedef struct sony_demod_t {
    /**
    @brief The demodulator crystal frequency.
    */
    sony_demod_xtal_t xtalFreq;

    /**
     @brief SLVT I2C address (8-bit form - 8'bxxxxxxx0).
    */
    uint8_t i2cAddressSLVT;

    /**
     @brief SLVX I2C address (8-bit form - 8'bxxxxxxx0). Fixed to i2cAddressSLVT + 4.
    */
    uint8_t i2cAddressSLVX;

    /**
     @brief I2C API instance.
    */
    sony_i2c_t * pI2c;

    /**
    @brief The driver operating state.
    */
    sony_demod_state_t state;

    /**
    @brief The current system.
    */
    sony_dtv_system_t system;

    /**
     @brief Auto detected chip ID at initialisation.
    */
    sony_demod_chip_id_t chipId;

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    /**
     @brief The current bandwidth, terrestrial and cable systems only.
    */
    sony_demod_bandwidth_t bandwidth;

    /**
     @brief IF frequency configuration for terrestrial and cable systems. Configure 
            prior to initialisation. Use the ::SONY_DEMOD_MAKE_IFFREQ_CONFIG macro for 
            configuration.
    */
    sony_demod_iffreq_config_t iffreqConfig;

    /**
     @brief Stores the terrestrial / cable tuner model for demodulator specific optimisations.
    */
    sony_demod_tuner_optimize_t tunerOptimize;

    /**
     @brief RFAIN ADC enable/disable. Must be configured prior to initialisation.
            Only change this indicator during the ::SONY_DEMOD_STATE_SLEEP_T_C state. 
            Use ::sony_demod_SetConfig to configure this flag.
    */
    uint8_t enableRfain;

    /**
     @brief Scan mode enable/disable. Only change this indicator during the
            SONY_DEMOD_STATE_SLEEP_T_C.
    */
    uint8_t scanMode;

    /**
     @brief The sense configured on the demodulator with
            ::sony_demod_SetConfig.
    */
    sony_demod_terr_cable_spectrum_sense_t confSense;

    /**
     @brief The serial TS clock frequency option for terrestrial and cable active states. 
            This is configured using ::sony_demod_SetConfig with the 
            SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ option.
    */
    sony_demod_serial_ts_clk_t serialTsClkFreqTerrCable;
#endif

    /**
     @brief The serial TS clock mode for all active states.  This is configured using
            ::sony_demod_SetConfig with the SONY_DEMOD_CONFIG_TSCLK_CONT option.
    */
    uint8_t serialTsClockModeContinuous;

    /**
     @brief A table of the demodulator configuration changes stored from the 
            ::sony_demod_SetConfig and ::sony_demod_SetAndSaveRegisterBits functions.
    */
    sony_demod_config_memory_t configMemory[SONY_DEMOD_MAX_CONFIG_MEMORY_COUNT];

    /**
     @brief The index of the last valid entry in the configMemory table
    */
    uint8_t configMemoryLastEntry;

    /**
     @brief User defined data.
    */
    void * user;
} sony_demod_t;

/**
 @brief Register definition structure for TS clock configurations.    
 */
typedef struct {
    uint8_t serialClkMode;      /**< Serial clock mode (gated or continuous) */
    uint8_t serialDutyMode;     /**< Serial clock duty mode (full rate or half rate) */
    uint8_t tsClkPeriod;        /**< TS clock period */
    uint8_t clkSelTsIf;         /**< TS clock frequency (low, mid or high) */
} sony_demod_ts_clk_configuration_t;

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/
/**
 @brief Set up the demodulator.

        This MUST be called before calling ::sony_demod_InitializeS or ::sony_demod_InitializeT_C.

 @param pDemod Reference to memory allocated for the demodulator instance. The create 
        function will setup this demodulator instance.
 @param xtalFreq The frequency of the demod crystal.
 @param i2cAddress The demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_Create (sony_demod_t * pDemod,
                                 sony_demod_xtal_t xtalFreq,
                                 uint8_t i2cAddress,
                                 sony_i2c_t * pDemodI2c);

/**
 @brief Initialize the demodulator, into Terrestrial / Cable mode from a power on state.
        For Satellite systems please use ::sony_demod_InitializeS.
        
        Can also be used to reset the demodulator from any state back to 
        ::SONY_DEMOD_STATE_SLEEP_T_C.  Please note this will reset all demodulator registers
        clearing any configuration settings.

        This API also clears the demodulator configuration memory table.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_InitializeT_C (sony_demod_t * pDemod);

/**
 @brief Initialize the demodulator, into Satellite mode from a power on state.
        For Terrestrial / Cable systems please use ::sony_demod_InitializeT_C.
        
        Can also be used to reset the demodulator from any state back to 
        ::SONY_DEMOD_STATE_SLEEP_S.  Please note this will reset all demodulator registers
        clearing any configuration settings.

        This API also clears the demodulator configuration memory table.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_InitializeS (sony_demod_t * pDemod);

/**
 @brief Put the demodulator into Sleep state for Terrestrial and Cable mode.  From
        this state the demodulator can be directly tuned to any T / T2 / C / C2 signal,
        or have the mode changed to Satellite by calling sony_demod_Sleep_S.

        If currently in ::SONY_DEMOD_STATE_SHUTDOWN the configuration memory will be loaded
        back into the demodulator.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_SleepT_C (sony_demod_t * pDemod);

/**
 @brief Put the demodulator into Sleep state for Terrestrial and Cable mode.  From
        this state the demodulator can be directly tuned to any S / S2 signal, or have 
        the mode changed to Terrestrial / Cable by calling sony_demod_Sleep_T_C.

        If currently in ::SONY_DEMOD_STATE_SHUTDOWN the configuration memory will be loaded
        back into the demodulator.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_SleepS (sony_demod_t * pDemod);

/**
 @brief Shutdown the demodulator.

        The device is placed in "Shutdown" state.
        ::sony_demod_SleepT_C or ::sony_demod_SleepS must be called to re-initialise the
        device and driver for future acquisitions.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_Shutdown (sony_demod_t * pDemod);

/**
 @brief Completes the demodulator acquisition setup.
        Must be called after system specific demod and RF tunes.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_TuneEnd (sony_demod_t * pDemod);

/**
 @brief Soft reset the demodulator.
        The soft reset will begin the devices acquisition process.

 @param pDemod The demod instance.

 @return SONY_RESULT_OK if successfully reset.
*/
sony_result_t sony_demod_SoftReset (sony_demod_t * pDemod);

/**
 @brief Set configuration options on the demodulator.

 @param pDemod The demodulator instance.
 @param configId The configuration ID to set. See ::sony_demod_config_id_t.
 @param value The associated value. Depends on the configId.

 @return SONY_RESULT_OK if successfully set the configuration option.
*/
sony_result_t sony_demod_SetConfig (sony_demod_t * pDemod, 
                                    sony_demod_config_id_t configId, 
                                    int32_t value);

/**
 @brief Configure the demodulator to forward I2C messages to the
        output port for tuner control.

 @param pDemod The demodulator instance.
 @param enable Enable / Disable I2C repeater

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_I2cRepeaterEnable (sony_demod_t * pDemod, 
                                            uint8_t enable);

/**
 @brief Setup the GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param enable Set enable (1) or disable (0).
 @param mode GPIO pin mode

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_GPIOSetConfig (sony_demod_t * pDemod, 
                                        uint8_t id, 
                                        uint8_t enable, 
                                        sony_demod_gpio_mode_t mode);

/**
 @brief Read the GPIO value.
        The GPIO should have been configured as an input (Read) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param pValue The current value of the GPIO.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_GPIORead (sony_demod_t * pDemod, 
                                   uint8_t id, 
                                   uint8_t * pValue);

/**
 @brief Write the GPIO value.
        The GPIO should have been configured as an output (Write) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param value The value to set as output.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_GPIOWrite (sony_demod_t * pDemod, 
                                    uint8_t id, 
                                    uint8_t value);

/**
 @brief Get the Chip ID of the connected demodulator.
 
 @param pDemod The demodulator instance.
 @param pChipId Pointer to receive the IP ID into.

 @return SONY_RESULT_OK if pChipId is valid.
*/
sony_result_t sony_demod_ChipID (sony_demod_t * pDemod, 
                                 sony_demod_chip_id_t * pChipId);

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief Monitor the value of the 10bit ADC on the RFAIN demodulator pin
 
 @param pDemod The demodulator instance.
 @param pRFAIN The value of the 10bit ADC RFAIN

 @return SONY_RESULT_OK if pRFAIN is valid.
*/
sony_result_t sony_demod_terr_cable_monitor_RFAIN (sony_demod_t * pDemod, 
                                                   uint16_t * pRFAIN);
#endif

/**
 @brief Set a specific value with bit mask to any demod register.  
        NOTE : This API should only be used under instruction from Sony 
        support. Manually modifying any demodulator register could have a negative 
        effect for performance or basic functionality.
         
 @param pDemod The demodulator instance.
 @param slaveAddress Slave address of configuration setting
 @param bank Demodulator bank of configuration setting
 @param registerAddress Register address of configuration setting
 @param value The value being written to this register
 @param bitMask The bit mask used on the register
 
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_SetAndSaveRegisterBits (sony_demod_t * pDemod, 
                                                 uint8_t slaveAddress, 
                                                 uint8_t bank, 
                                                 uint8_t registerAddress,
                                                 uint8_t value,
                                                 uint8_t bitMask);

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
/**
 @brief Enable / disable scan mode for acquisition in the demodulator.

 @param pDemod The demodulator instance
 @param system The system used for scanning
 @param scanModeEnabled State of scan mode to set

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_terr_cable_SetScanMode (sony_demod_t * pDemod, 
                                      sony_dtv_system_t system,
                                      uint8_t scanModeEnabled);
#endif


/**
 @brief Set the TS clock mode and frequency based on the demod struct
        members.  Called internally as part of each Sleep to Active
        state transition.

 @param pDemod The demodulator instance
 @param system The tuning system

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_demod_SetTsClockModeAndFreq (sony_demod_t * pDemod, sony_dtv_system_t system);

#endif /* SONY_DEMOD_H */
