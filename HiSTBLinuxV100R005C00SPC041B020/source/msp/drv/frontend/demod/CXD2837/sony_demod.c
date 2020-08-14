/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-21 11:19:46 #$
  File Revision : $Revision:: 6152 $
------------------------------------------------------------------------------*/

#include "sony_integ.h"
#include "sony_demod.h"
#include "sony_stdlib.h"

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
#include "sony_demod_dvbt.h"
#include "sony_demod_dvbt2.h"
#include "sony_demod_dvbc.h"
#endif

#ifdef SONY_DEMOD_SUPPORT_DVBC2
#include "sony_demod_dvbc2.h"
#endif
/*------------------------------------------------------------------------------
 Static Function Prototypes
------------------------------------------------------------------------------*/
/**
 @brief Configure the demodulator from Shutdown to Sleep T / C state
*/
static sony_result_t SDtoSLT(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from Shutdown to Sleep S state
*/
static sony_result_t SDtoSLS(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from any Sleep T / C state to Shutdown
*/
static sony_result_t SLTtoSD(sony_demod_t * pDemod);

#if 0
/**
 @brief Configure the demodulator from any Sleep S state to Shutdown
*/
static sony_result_t SLStoSD(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from Sleep S to Sleep T / C state
*/
static sony_result_t SLStoSLT(sony_demod_t * pDemod);
#endif

/**
 @brief Configure the demodulator from Sleep T / C to Sleep S state
*/
static sony_result_t SLTtoSLS(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from any state to Sleep T / C state.
        This is used as a demodulator reset, all demodulator configuration
        settings will be lost.
*/
static sony_result_t XtoSLT(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from any state to Sleep S state.
        This is used as a demodulator reset, all demodulator configuration
        settings will be lost.
*/
static sony_result_t XtoSLS(sony_demod_t * pDemod);

/**
 @brief Clear the demodulator configuration memory table.  Use
        this API to empty the table of previous entries.  This is called 
        automatically in the sony_integ_InitializeT_C and 
        sony_integ_InitializeS API's.
*/
static sony_result_t clearConfigMemory (sony_demod_t * pDemod);

/**
 @brief Iterate through the demodulator configuration memory table and write
        each entry to the device.  This is called automatically during a 
        transition from ::SONY_DEMOD_STATE_SHUTDOWN to 
        ::SONY_DEMOD_STATE_SLEEP_T_C or ::SONY_DEMOD_STATE_SLEEP_S or states.
*/
static sony_result_t loadConfigMemory (sony_demod_t * pDemod);

/**
 @brief Save an entry into the demodulator configuration memory table.
 
 @param pDemod The demodulator instance.
 @param slaveAddress Slave address of configuration setting
 @param bank Demodulator bank of configuration setting
 @param registerAddress Register address of configuration setting
 @param value The value being written to this register
 @param mask The bit mask used on the register
*/
static sony_result_t setConfigMemory (sony_demod_t * pDemod, 
                                                 uint8_t slaveAddress, 
                                                 uint8_t bank, 
                                                 uint8_t registerAddress,
                                                 uint8_t value,
                                                 uint8_t bitMask);

/*------------------------------------------------------------------------------
 System Support Dependant Function Removal
------------------------------------------------------------------------------*/
#ifndef SONY_DEMOD_SUPPORT_DVBC2
#define sony_demod_dvbc2_Sleep(pDemod) SONY_RESULT_ERROR_NOSUPPORT
#endif

#ifndef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
#define sony_demod_dvbc_Sleep(pDemod) SONY_RESULT_ERROR_NOSUPPORT
#define sony_demod_dvbt_Sleep(pDemod) SONY_RESULT_ERROR_NOSUPPORT
#define sony_demod_dvbt2_Sleep(pDemod) SONY_RESULT_ERROR_NOSUPPORT
#endif

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
sony_result_t sony_demod_Create (sony_demod_t * pDemod,
                                 sony_demod_xtal_t xtalFreq,
                                 uint8_t i2cAddress,
                                 sony_i2c_t * pDemodI2c)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_demod_Create");

    if ((!pDemod) || (!pDemodI2c)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Check for invalid crystal selection */
    if ((xtalFreq == SONY_DEMOD_XTAL_16000KHz) || (xtalFreq == SONY_DEMOD_XTAL_24000KHz)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    sony_memset (pDemod, 0, sizeof (sony_demod_t));

    pDemod->xtalFreq = xtalFreq;
    pDemod->i2cAddressSLVT = i2cAddress;
    pDemod->i2cAddressSLVX = i2cAddress + 4;
    pDemod->pI2c = pDemodI2c;
    pDemod->state = SONY_DEMOD_STATE_UNKNOWN;
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    pDemod->serialTsClkFreqTerrCable = SONY_DEMOD_SERIAL_TS_CLK_MID_FULL;
#endif
    pDemod->serialTsClockModeContinuous = 1;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_InitializeT_C (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_InitializeT_C");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    pDemod->serialTsClkFreqTerrCable = SONY_DEMOD_SERIAL_TS_CLK_MID_FULL;
#endif
    pDemod->serialTsClockModeContinuous = 1;

    /* Initialize causes demodulator register reset */
    result = XtoSLT(pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    pDemod->state = SONY_DEMOD_STATE_SLEEP_T_C;

    result = sony_demod_ChipID (pDemod, &(pDemod->chipId));
    if (result != SONY_RESULT_OK) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (result);
    }

    if ((pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2837) && 
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2839) && 
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2841) &&
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2842) &&
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2843)) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Clear Config memory in Initialize API */
    result = clearConfigMemory (pDemod);
    if (result != SONY_RESULT_OK) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_InitializeS (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_InitializeS");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    pDemod->serialTsClkFreqTerrCable = SONY_DEMOD_SERIAL_TS_CLK_MID_FULL;
#endif
    pDemod->serialTsClockModeContinuous = 1;

    /* Initialize causes demodulator register reset */
    result = XtoSLS(pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    pDemod->state = SONY_DEMOD_STATE_SLEEP_S;

    result = sony_demod_ChipID (pDemod, &(pDemod->chipId));
    if (result != SONY_RESULT_OK) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (result);
    }

    if ((pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2837) && 
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2839) && 
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2841) &&
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2842) &&
        (pDemod->chipId != SONY_DEMOD_CHIP_ID_CXD2843)) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* Clear Config memory in Initialize API */
    result = clearConfigMemory (pDemod);
    if (result != SONY_RESULT_OK) {
        pDemod->state = SONY_DEMOD_STATE_INVALID;
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_SleepT_C (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_Sleep_T_C");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state == SONY_DEMOD_STATE_SHUTDOWN) {
        result = SDtoSLT(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#if 0    
    else if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_S) {
        /* First transition to SLEEP_S state */
        result = sony_demod_dvbs_s2_Sleep(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Then transfer to SLEEP_T_C state */
        result = SLStoSLT(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#endif    
    else if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_T_C) {
        switch (pDemod->system) {
        case SONY_DTV_SYSTEM_DVBT:
            result = sony_demod_dvbt_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBT2:
            result = sony_demod_dvbt2_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC:
            result = sony_demod_dvbc_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC2:
            result = sony_demod_dvbc2_Sleep(pDemod);
            break;

        default:
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
        }

        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_T_C) {
        /* Do nothing */
    }
#if 0    
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_S) {
        /* Transfer to SLEEP_T_C state */
        result = SLStoSLT(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }        

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#endif    
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    pDemod->state = SONY_DEMOD_STATE_SLEEP_T_C;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_SleepS (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_SleepS");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state == SONY_DEMOD_STATE_SHUTDOWN) {
        result = SDtoSLS(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
    else if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* First transition to SLEEP_T_C state */
        switch (pDemod->system) {
        case SONY_DTV_SYSTEM_DVBT:
            result = sony_demod_dvbt_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBT2:
            result = sony_demod_dvbt2_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC:
            result = sony_demod_dvbc_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC2:
            result = sony_demod_dvbc2_Sleep(pDemod);
            break;

        default:
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
        }

        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Then transfer to SLEEP_S state */
        result = SLTtoSLS(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#if 0    
    else if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_S) {
        result = sony_demod_dvbs_s2_Sleep(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }           
    }
#endif
#if 0
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_S) {
        /* Do nothing */
    }
#endif    
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_T_C) {
        /* Transfer to SLEEP_S state */
        result = SLTtoSLS(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }        

        /* Load Config memory if returning from Shutdown state */
        result = loadConfigMemory (pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    pDemod->state = SONY_DEMOD_STATE_SLEEP_S;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_Shutdown (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_Shutdown");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* First transition to SLEEP_T_C state */
        switch (pDemod->system) {
        case SONY_DTV_SYSTEM_DVBT:
            result = sony_demod_dvbt_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBT2:
            result = sony_demod_dvbt2_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC:
            result = sony_demod_dvbc_Sleep(pDemod);
            break;

        case SONY_DTV_SYSTEM_DVBC2:
            result = sony_demod_dvbc2_Sleep(pDemod);
            break;

        default:
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
        }

        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }

        /* Then transfer to SHUTDOWN state */
        result = SLTtoSD(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#if 0    
    else if (pDemod->state == SONY_DEMOD_STATE_ACTIVE_S) {
        /* First transition to SLEEP_S state */
        result = sony_demod_dvbs_s2_Sleep(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }   

        /* Then transfer to SHUTDOWN state */
        result = SLStoSD(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#endif    
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_T_C) {
        /* Transfer to SHUTDOWN state */
        result = SLTtoSD(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#if 0    
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_S) {
        /* Transfer to SHUTDOWN state */
        result = SLStoSD(pDemod);
        if (result != SONY_RESULT_OK) {
            pDemod->state = SONY_DEMOD_STATE_INVALID;
            SONY_TRACE_RETURN (result);
        }
    }
#endif    
    else if (pDemod->state == SONY_DEMOD_STATE_SHUTDOWN) {
        /* Do Nothing */
    }
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);    
    }

    pDemod->state = SONY_DEMOD_STATE_SHUTDOWN;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_TuneEnd (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_TuneEnd");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
     
    if ((pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S)) {
        /* This api is accepted in ACTIVE states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SW Reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xFE, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable TS output */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xC3, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_SoftReset (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_SoftReset");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S) &&
        (pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
        /* This api is accepted in SLEEP and ACTIVE states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SW Reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xFE, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_SetConfig (sony_demod_t * pDemod, 
                                    sony_demod_config_id_t configId, 
                                    int32_t value)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_SetConfig");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S) && 
        (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S)) {
        /* This api is accepted in SLEEP and ACTIVE states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    switch (configId) {
    case SONY_DEMOD_CONFIG_PARALLEL_SEL:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C4h     [7]    1'b0       OSERIALEN
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC4, (uint8_t) (value ? 0x00 : 0x80), 0x80);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_SER_DATA_ON_MSB:       
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C4h     [3]    1'b1       OSEREXCHGB7
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC4, (uint8_t) (value ? 0x08 : 0x00), 0x08);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_OUTPUT_SEL_MSB:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C4h     [4]    1'b0       OWFMT_LSB1STON
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC4, (uint8_t) (value ? 0x00 : 0x10), 0x10);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSVALID_ACTIVE_HI:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C5h     [1]    1'b0       OWFMT_VALINV
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC5, (uint8_t) (value ? 0x00 : 0x02), 0x02);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSSYNC_ACTIVE_HI:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C5h     [2]    1'b0       OWFMT_STINV
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC5, (uint8_t) (value ? 0x00 : 0x04), 0x04);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSERR_ACTIVE_HI:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     CBh     [0]    1'b0       OWFMT_ERRINV
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xCB, (uint8_t) (value ? 0x00 : 0x01), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_LATCH_ON_POSEDGE:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * ---------------------------------------------------
         * <SLV-T>  00h     C5h     [0]    1'b1       OWFMT_CKINV
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC5, (uint8_t) (value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSCLK_CONT:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* Store the serial clock mode */
        pDemod->serialTsClockModeContinuous = (uint8_t) value;
        break;

    case SONY_DEMOD_CONFIG_TSCLK_MASK:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        if (value > 0x1F) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
        }

        /* slave    Bank    Addr    Bit      default     Name
         * -------------------------------------------------------------
         * <SLV-T>  00h    C6h     [4:2]     3'b000      OWFMT_CKDISABLE
         * <SLV-T>  00h    C6h     [0]       1'b0        OWFMT_CKDISABLE
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00,  0xC6, (uint8_t) value, 0x1D);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSVALID_MASK:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        if (value > 0x1F) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
        }

        /* slave    Bank    Addr    Bit      default     Name
         * -------------------------------------------------------------
         * <SLV-T>  00h     C8h     [4:2]    3'b000      OWFMT_VALDISABLE
         * <SLV-T>  00h     C8h     [0]      1'b1        OWFMT_VALDISABLE
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC8, (uint8_t) value, 0x1D);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSERR_MASK:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        if (value > 0x1F) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
        }

        /* slave    Bank    Addr    Bit      default     Name
         * -------------------------------------------------------------
         * <SLV-T>  00h     C9h     [4:2]    3'b000      OWFMT_ERRDISABLE
         * <SLV-T>  00h     C9h     [0]      1'b0        OWFMT_ERRDISABLE
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xC9, (uint8_t) value, 0x1D);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TSCLK_CURRENT_10mA:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * --------------------------------------------------------
         * <SLV-T>  00h     83h     [0]    1'b0       OREG_TSCLK_C
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0x83, (uint8_t) (value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TS_CURRENT_10mA:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * --------------------------------------------------------
         * <SLV-T>  00h     83h     [1]    1'b0       OREG_TSSYNC_C
         * <SLV-T>  00h     83h     [2]    1'b0       OREG_TSVALID_C
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0x83, (uint8_t) (value ? 0x06 : 0x00), 0x06);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * --------------------------------------------------------
         * <SLV-T>  00h     84h     [7:0]  8'h00      OREG_TSDATA_C
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0x84, (uint8_t) (value ? 0xFF : 0x00), 0xFF);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        {
            uint8_t data;

            /* Set SLV-X Bank : 0x00 */
            if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            /* slave    Bank    Addr    Bit    default   Value        Name        
             * -------------------------------------------------------------------
             * <SLV-X>  00h     A5h     [3:0]   8'h00     8'h04     OREG_GPIP2_SEL  
             */
            if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0xA5, &data, 1) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            /* Set GPIO2 Drivability, only if in TSERR mode */
            if ((data & 0x0F) == SONY_DEMOD_GPIO_MODE_TS_ERROR) {
                /* slave    Bank    Addr    Bit    default    Name
                 * --------------------------------------------------------
                 * <SLV-X>  00h     85h     [2]    1'b0       OREG_GPIO2_C
                 */
                if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x85, (uint8_t) (value ? 0x04 : 0x00), 0x04) != SONY_RESULT_OK) {
                    SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
                }
            }
        }
        break;

    case SONY_DEMOD_CONFIG_TS_BACKWARDS_COMPATIBLE:
        /* This register can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* Slave    Bank    Addr    Bit      Default    Name
         * -----------------------------------------------------------
         * <SLV-T>  00h     D3h     [0]      1'b0       OTSRATECTRLOFF
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xD3, (uint8_t)(value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Slave    Bank    Addr    Bit      Default    Name
         * ------------------------------------------------------
         * <SLV-T>  00h     DEh     [0]      1'b0       OTSIN_OFF
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xDE, (uint8_t)(value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_PWM_VALUE:
        /* Slave    Bank    Addr    Bit      Default    Value    Name
         * -------------------------------------------------------------------
         * <SLV-T>  00h     B7h     [0]      1'b0       1'b1     OREG_RFAGCSEL
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xB7, (uint8_t)(value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* slave    Bank    Addr    Bit    default    Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>  00h     B2h     [3:0]    8'h00      8'h0x     OREG_GDA_VAL_RFAGC[11:8]
         * <SLV-T>  00h     B3h     [7:0]    8'h00      8'hxx     OREG_GDA_VAL_RFAGC[7:0]
         */
        {
            uint8_t data[2];
            data[0] = (uint8_t) (((uint16_t)value >> 8) & 0x0F);
            data[1] = (uint8_t) ((uint16_t)value & 0xFF);

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xB2, data[0], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x00, 0xB3, data[1], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        }

        break;

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    case SONY_DEMOD_CONFIG_TSIF_SDPR:
        if (pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) {
            /* This api is accepted in SLEEP state only */
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* Argument checking. */
        if ((value <= 0) || (value > SONY_DEMOD_MAX_TS_CLK_KHZ)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        {
            uint32_t val = 0x00;
            uint8_t data[3];
            uint32_t uvalue = (uint32_t) value;

            /*
             * OREG_TSIF_WR_SMOOTH_DP = (82(MHz) / TSCLK(MHz)) * 2^14 ;
             * Notes: 82000 * (2^14) = 1343488000
             */
            val = (1343488000u + uvalue / 2) / uvalue;
            data[0] = (uint8_t) ((val & 0x3F0000) >> 16);
            data[1] = (uint8_t) ((val & 0xFF00) >> 8);
            data[2] = (uint8_t) (val & 0xFF);

            /* Slave    Bank     Addr    Bit      Default    Value     Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     E4h     [5:0]    8'h04      8'hxx     OREG_TSIF_WR_SMOOTH_DP[21:16]
             * <SLV-T>   10h     E5h     [7:0]    8'hC5      8'hxx     OREG_TSIF_WR_SMOOTH_DP[15:8]
             * <SLV-T>   10h     E6h     [7:0]    8'hC1      8'hxx     OREG_TSIF_WR_SMOOTH_DP[7:0]
             */
            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xE4, data[0], 0x3F);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xE5, data[1], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xE6, data[2], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        }

        {
            uint32_t val = 0x00;
            uint8_t data[4];
            uint32_t uvalue = (uint32_t) value;

            /*
             * OREG_SP_WR_SMOOTH_DP[28:0] = (82(MHz) / TSCLK(MHz)) * 2^15 ;
             * Notes: 82000 * (2^15) = 2686976000
             */
            val = (2686976000u + uvalue / 2) / uvalue;
            data[0] = (uint8_t) ((val & 0x1F000000) >> 24);
            data[1] = (uint8_t) ((val & 0xFF0000) >> 16);
            data[2] = (uint8_t) ((val & 0xFF00) >> 8);
            data[3] = (uint8_t) (val & 0xFF);

            /* Slave    Bank     Addr    Bit      Default    Value     Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   20h     DCh     [4:0]    8'h00      8'hxx     OREG_SP_WR_SMOOTH_DP[28:24]
             * <SLV-T>   20h     DDh     [7:0]    8'h09      8'hxx     OREG_SP_WR_SMOOTH_DP[23:26]
             * <SLV-T>   20h     DEh     [7:0]    8'h12      8'hxx     OREG_SP_WR_SMOOTH_DP[15:8]
             * <SLV-T>   20h     DFh     [7:0]    8'hCB      8'hxx     OREG_SP_WR_SMOOTH_DP[7:0]
             */
            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0xDC, data[0], 0x1F);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT,  0x20, 0xDD, data[1], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0xDE, data[2], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }

            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0xDF, data[3], 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        }
        break;

    case SONY_DEMOD_CONFIG_TS_AUTO_RATE_ENABLE:
        if (pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) {
            /* This api is accepted in SLEEP state only */
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        /* slave    Bank    Addr    Bit    default    Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>  10h     E3h     [0]      8'h01      8'h00     OREG_TSIF_AUTO_RATE_EN
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xE3, (uint8_t)(value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* slave    Bank    Addr    Bit    default    Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>  20h     D8h     [0]      8'h01      8'h00     OREG_SP_AUTO_RATE_EN
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0xD8, (uint8_t)(value ? 0x01 : 0x00), 0x01);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ:
        if (pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) {
            /* This api is accepted in SLEEP state only */
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }

        if ((value < 1) || (value > 6)) {
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_RANGE);
        }
	
        /* Store the clock frequency mode for terrestrial and cable systems */
        pDemod-> serialTsClkFreqTerrCable = (sony_demod_serial_ts_clk_t) value;
        break;

    case SONY_DEMOD_CONFIG_IFAGCNEG:
        /* slave    Bank    Addr    Bit    default     Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     CBh     [6]      8'h48      8'h08      OCTL_IFAGCNEG
         */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xCB, (uint8_t) (value ? 0x40 : 0x00), 0x40);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_IFAGC_ADC_FS:
        {
            uint8_t data;

            if (value == 0) {
                data = 0x50; /* 1.4Vpp - Default */
            }
            else if (value == 1) {
                data = 0x39; /* 1.0Vpp */
            }
            else if (value == 2) {
                data = 0x28; /* 0.7Vpp */
            }
            else {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
            }

            /* Slave     Bank    Addr   Bit      Default    Name
             * -------------------------------------------------------------------
             * <SLV-T>   10h     CDh    [7:0]    8'h50      OCTL_IFAGC_TARGET[7:0]
             */
            result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0xCD, data, 0xFF);
            if (result != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (result);
            }
        }
        break;

    case SONY_DEMOD_CONFIG_SPECTRUM_INV:
        /* Store the configured sense. */
        pDemod->confSense = value ? SONY_DEMOD_TERR_CABLE_SPECTRUM_INV : SONY_DEMOD_TERR_CABLE_SPECTRUM_NORMAL;
        break;

    case SONY_DEMOD_CONFIG_RFAIN_ENABLE:
        /* This setting can change only in SLEEP state */
        if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
        }
        
        pDemod->enableRfain = (uint8_t)(value ? 1 : 0);
        break;

    case SONY_DEMOD_CONFIG_DVBTC_BERN_PERIOD:
        /* Set the measurment period for Pre-RS BER (DVB-T/C). */
        /* Verify range of value */
        if ((value < 0) || (value > 31)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * ------------------------------------------------
        * <SLV-T>   10h     60h    [4:0]   OREG_BERN_PERIOD
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0x60, (uint8_t) (value & 0x1F), 0x1F);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_DVBT_VBER_PERIOD:
        /* Set the measurment period for Pre-Viterbi BER (DVB-T). */
        /* Verify range of value */
        if ((value < 0) || (value > 7)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * ----------------------------------------------------
        * <SLV-T>  10h     6Fh    [2:0]   OREG_VBER_PERIOD_SEL
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0x6F, (uint8_t) (value & 0x07), 0x07);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_DVBT2C2_BBER_MES:
        /* Set the measurment period for Pre-BCH BER (DVB-T2/C2) and Post-BCH FER (DVB-T2/C2). */
        /* Verify range of value */
        if ((value < 0) || (value > 15)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * ---------------------------------------------
        * <SLV-T>  20h     72h    [3:0]   OREG_BBER_MES
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0x72, (uint8_t) (value & 0x0F), 0x0F);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_DVBT2C2_LBER_MES:
        /* Set the measurment period for Pre-LDPC BER (DVB-T2/C2). */
        /* Verify range of value */
        if ((value < 0) || (value > 15)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * ---------------------------------------------
        * <SLV-T>  20h     6Fh    [3:0]   OREG_LBER_MES
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x20, 0x6F, (uint8_t) (value & 0x0F), 0x0F);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_DVBTC_PER_MES:
        /* Set the measurment period for PER (DVB-T/C). */
        /* Verify range of value */
        if ((value < 0) || (value > 15)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * --------------------------------------------
        * <SLV-T>  10h     5Ch    [3:0]   OREG_PER_MES
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x10, 0x5C, (uint8_t) (value & 0x0F), 0x0F);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;

    case SONY_DEMOD_CONFIG_DVBT2C2_PER_MES:
        /* Set the measurment period for PER (DVB-T2/C2). */
        /* Verify range of value */
        if ((value < 0) || (value > 15)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_RANGE);
        }

        /* slave    Bank    Addr   Bit     Name
        * -----------------------------------------------
        * <SLV-T>  24h     DCh    [3:0]   OREG_SP_PER_MES
        */
        result = sony_demod_SetAndSaveRegisterBits(pDemod, pDemod->i2cAddressSLVT, 0x24, 0xDC, (uint8_t) (value & 0x0F), 0x0F);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
        break;
#endif

    default:
        /* Unknown ID */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_I2cRepeaterEnable (sony_demod_t * pDemod, uint8_t enable)
{
    SONY_TRACE_ENTER ("sony_demod_I2cRepeaterEnable");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* slave    Bank    Addr    Bit    default    Value          Name
     * ----------------------------------------------------------------------------------
     * <SLV-X>  ALL     08h    [0]      8'h00      8'h01     OREG_REPEN
     */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x08, (uint8_t) (enable ? 0x01 : 0x00), 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

sony_result_t sony_demod_GPIOSetConfig (sony_demod_t * pDemod, 
                                        uint8_t id, 
                                        uint8_t enable, 
                                        sony_demod_gpio_mode_t mode)
{
    uint8_t gpioModeSelAddr;
    uint8_t gpioBitSel;

    SONY_TRACE_ENTER ("sony_demod_GPIOSetConfig");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S) && 
        (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S) &&
        (pDemod->state != SONY_DEMOD_STATE_SHUTDOWN)) {
        /* This api is accepted in SLEEP, ACTIVE and SHUTDOWN states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (id > 2) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* TS Error only available on GPIO2 */
    if ((mode == SONY_DEMOD_GPIO_MODE_TS_ERROR) && (id != 2)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    gpioModeSelAddr = (uint8_t)(0xA3 + id);
    gpioBitSel = (uint8_t)(1 << id);
    
    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Slave    Bank    Addr    Bit      Default   Name
     * -----------------------------------------------------------   
     * <SLV-X>  00h     A3h     [3:0]    4'h00     OREG_GPIO0_SEL
     * <SLV-X>  00h     A4h     [3:0]    4'h00     OREG_GPIO1_SEL
     * <SLV-X>  00h     A5h     [3:0]    4'h00     OREG_GPIO2_SEL
     */
    if (sony_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddressSLVX, gpioModeSelAddr, (uint8_t)mode, 0x0F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    {
        uint8_t enableHiZ;

        if (mode == SONY_DEMOD_GPIO_MODE_INPUT) {
            /* HiZ enabled when pin is GPI */
            enableHiZ = (uint8_t)(0x01 << id);
        }
        else {
            /* HiZ determined by enable parameter */
            enableHiZ = enable ? 0x00 : (uint8_t)(0x01 << id);
        }

        /* Set HiZ setting for selected pin */
        /* Slave    Bank    Addr    Bit      Default    Name            Meaning
         * -----------------------------------------------------------------------------------
         * <SLV-X>  00h     82h     [2:0]    3'b111     OREG_GPIO_HIZ    0: HiZ Off, 1: HiZ On
         */
        if (sony_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddressSLVX, 0x82, enableHiZ, gpioBitSel) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Set drivability current for GPIO 2 */
    if (id == 2) {
        uint8_t drivability = 0x00; /* Default 8mA */
        
        /* If GPIO2 is set to TSERR, set to same drivability as TS Valid */
        if (mode == SONY_DEMOD_GPIO_MODE_TS_ERROR) {
            uint8_t data;
            /* slave    Bank    Addr    Bit    default      Name
             * -----------------------------------------------------------
             * <SLV-T>  00h     83h     [2]    1'b0         OREG_TSVALID_C
             */
            if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x83, &data, 1) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }        

            if (data & 0x04) {
                /* TS pins set to 10mA */
                drivability = 0x04; 
            }
        }

        /* slave    Bank    Addr    Bit    default     Name
         * --------------------------------------------------------
         * <SLV-X>  00h     85h     [2]    1'b0        OREG_GPIO2_C
         */
        if (sony_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddressSLVX, 0x85, drivability, 0x04) != SONY_RESULT_OK){
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }        
    }

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

sony_result_t sony_demod_GPIORead (sony_demod_t * pDemod, 
                                   uint8_t id, 
                                   uint8_t * pValue)
{
    uint8_t rdata = 0x00;

    SONY_TRACE_ENTER ("sony_demod_GPIORead");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S) && 
        (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S) &&
        (pDemod->state != SONY_DEMOD_STATE_SHUTDOWN)) {
        /* This api is accepted in SLEEP, ACTIVE and SHUTDOWN states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (id > 2) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Slave    Bank    Addr    Bit      Name
    * ------------------------------------------------------------
    * <SLV-X>   00h     A0h     [2:0]    IREG_GPIO_IN
    */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0xA0, &rdata, 1) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    *pValue = (rdata & (0x01 << id))? 1 : 0;

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

sony_result_t sony_demod_GPIOWrite (sony_demod_t * pDemod, 
                                    uint8_t id, 
                                    uint8_t value)
{
    SONY_TRACE_ENTER ("sony_demod_GPIOWrite");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S) && 
        (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S) &&
        (pDemod->state != SONY_DEMOD_STATE_SHUTDOWN)) {
        /* This api is accepted in SLEEP, ACTIVE and SHUTDOWN states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Slave    Bank    Addr    Bit      Default     Name
    * ------------------------------------------------------------
    * <SLV-X>   00h     A2h     [2:0]    3'b000      IREG_GPIO_OUT
    */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVX, 0xA2, (uint8_t) (value ? (0x01 << id) : 0x00), (0x01 << id)) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

sony_result_t sony_demod_ChipID (sony_demod_t * pDemod, 
                                 sony_demod_chip_id_t * pChipId)
{
    uint8_t data = 0;
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_ChipID");

    if ((!pDemod) || (!pChipId)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Chip ID is available on both banks, SLV-T register is aligned to legacy devices so check this first,
     * if this fails (due to device being in SHUTDOWN state) read from SLV-X 
     */

    /* Set SLV-T Bank : 0x00 */
    result = pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00);

    if (result == SONY_RESULT_OK) {
        /*SLV-T OK so read SLV-T register */

        /* slave    Bank    Addr    Bit               NAME
         * -----------------------------------------------------------
         * <SLV-T>   00h     FDh     [7:0]            CHIP_ID
         */
        if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xFD, &data, 1) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }
    else if (result == SONY_RESULT_ERROR_I2C) {
        /*SLV-T failed, so try SLV-X */
        /* Set SLV-X Bank : 0x00 */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        /* slave    Bank    Addr    Bit               NAME
         * -----------------------------------------------------------
         * <SLV-X>   00h     FDh     [7:0]            CHIP_ID_SYS
         */
        if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0xFD, &data, 1) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }
    else {
        SONY_TRACE_RETURN (result);
    }

    *pChipId = (sony_demod_chip_id_t) data;

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
sony_result_t sony_demod_terr_cable_monitor_RFAIN (sony_demod_t * pDemod, 
                                                   uint16_t * pRFAIN)
{
    uint8_t data[2];
    SONY_TRACE_ENTER ("sony_demod_terr_cable_monitor_RFAIN");

    if ((!pDemod) || (!pRFAIN)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (!pDemod->enableRfain) {
        /* RFAIN ADC is disabled */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }
    
    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit      NAME
     * -----------------------------------------------------------
     * <SLV-T>   00h     50h    [1:0]    IRAMON_ADCDATA[9:8]
     * <SLV-T>   00h     51h    [7:0]    IRAMON_ADCDATA[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x50, &data[0], 2) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    *pRFAIN = (uint16_t)(((uint32_t)(data[0] & 0x03) << 8) + data[1]);

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}
#endif

sony_result_t sony_demod_SetAndSaveRegisterBits (sony_demod_t * pDemod, 
                                                 uint8_t slaveAddress, 
                                                 uint8_t bank, 
                                                 uint8_t registerAddress,
                                                 uint8_t value,
                                                 uint8_t bitMask)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("sony_demod_SetAndSaveRegisterBits");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
    
    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S) && 
        (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_S)) {
        /* This api is accepted in SLEEP and ACTIVE states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set the bank */
    result = pDemod->pI2c->WriteOneRegister (pDemod->pI2c, slaveAddress, 0x00, bank);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Write the register value */
    result = sony_i2c_SetRegisterBits (pDemod->pI2c, slaveAddress, registerAddress, value, bitMask);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Store the updated setting */
    result = setConfigMemory (pDemod, slaveAddress, bank, registerAddress, value, bitMask);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
sony_result_t sony_demod_terr_cable_SetScanMode (sony_demod_t * pDemod, sony_dtv_system_t system, uint8_t scanModeEnabled)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_terr_cable_SetScanMode");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (system == SONY_DTV_SYSTEM_DVBC) {
        /* Set SLV-T Bank : 0x40 */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x40) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        {
            uint8_t data = scanModeEnabled? 0x20 : 0x00;

            /* slave    Bank    Addr    Bit    default   Value          Name
             * ---------------------------------------------------------------------------------
             * <SLV-T>   40h     86h     [5]      1'b0       1'b1         OREG_AR_SCANNING
             */
            if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x86, data, 0x20) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
    }

    pDemod->scanMode = scanModeEnabled;

    SONY_TRACE_RETURN (result);
}
#endif

sony_result_t sony_demod_SetTsClockModeAndFreq (sony_demod_t * pDemod, sony_dtv_system_t system)
{
    sony_result_t result = SONY_RESULT_OK;
    uint8_t serialTs;
    uint8_t tsRateCtrlOff;
    uint8_t tsInOff;
    sony_demod_ts_clk_configuration_t tsClkConfiguration;

    const sony_demod_ts_clk_configuration_t serialTsClkSettings [2][6] = 
    {{ /* Gated Clock */
       /* OSERCKMODE  OSERDUTYMODE  OTSCKPERIOD  OREG_CKSEL_TSIF                            */
        {      3,          1,            8,             0        }, /* High Freq, full rate */
        {      3,          1,            8,             1        }, /* Mid Freq,  full rate */
        {      3,          1,            8,             2        }, /* Low Freq,  full rate */
        {      0,          2,            16,            0        }, /* High Freq, half rate */
        {      0,          2,            16,            1        }, /* Mid Freq,  half rate */
        {      0,          2,            16,            2        }  /* Low Freq,  half rate */
    },
    {  /* Continuous Clock */
       /* OSERCKMODE  OSERDUTYMODE  OTSCKPERIOD  OREG_CKSEL_TSIF                            */
        {      1,          1,            8,             0        }, /* High Freq, full rate */
        {      1,          1,            8,             1        }, /* Mid Freq,  full rate */
        {      1,          1,            8,             2        }, /* Low Freq,  full rate */
        {      2,          2,            16,            0        }, /* High Freq, half rate */
        {      2,          2,            16,            1        }, /* Mid Freq,  half rate */
        {      2,          2,            16,            2        }  /* Low Freq,  half rate */
    }};

    const sony_demod_ts_clk_configuration_t parallelTsClkSetting = 
    {  /* OSERCKMODE  OSERDUTYMODE  OTSCKPERIOD  OREG_CKSEL_TSIF */
               0,          0,            8,             0        
    }; 

    const sony_demod_ts_clk_configuration_t backwardsCompatibleSerialTsClkSetting [2] = 
    {  /* OSERCKMODE  OSERDUTYMODE  OTSCKPERIOD  OREG_CKSEL_TSIF                            */
        {      3,          1,            8,             1        }, /* Gated Clock          */
        {      1,          1,            8,             1        }  /* Continuous Clock     */
    }; 

    const sony_demod_ts_clk_configuration_t backwardsCompatibleParallelTsClkSetting = 
    {  /* OSERCKMODE  OSERDUTYMODE  OTSCKPERIOD  OREG_CKSEL_TSIF */
               0,          0,            8,             1        
    }; 

    SONY_TRACE_ENTER ("sony_demod_SetTsClockModeAndFreq");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_SLEEP_S)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit    default    Name
     * ---------------------------------------------------
     * <SLV-T>  00h     C4h     [7]    1'b0       OSERIALEN
     */
    result = pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xC4, &serialTs, 1);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Slave    Bank    Addr    Bit      Default    Name
     * -----------------------------------------------------------
     * <SLV-T>  00h     D3h     [0]      1'b0       OTSRATECTRLOFF
     */
    result = pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD3, &tsRateCtrlOff, 1);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Slave    Bank    Addr    Bit      Default    Name
     * ------------------------------------------------------
     * <SLV-T>  00h     DEh     [0]      1'b0       OTSIN_OFF
     */
    result = pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xDE, &tsInOff, 1);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if ((tsRateCtrlOff & 0x01) != (tsInOff & 0x01)) {
        /* (OTSRATECTRLOFF = 0 & OTSIN_OFF = 1) or (OTSRATECTRLOFF = 1 & OTSIN_OFF = 0)
         * are invalid combinations */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    if ((tsRateCtrlOff & 0x01) && (tsInOff & 0x01)) {
        /* Backwards compatible mode */
        if (serialTs & 0x80) {
            /* Serial TS */
            tsClkConfiguration = backwardsCompatibleSerialTsClkSetting[pDemod->serialTsClockModeContinuous];
        }
        else {
            /* Parallel TS */
            tsClkConfiguration = backwardsCompatibleParallelTsClkSetting;
        }
    }
    else if (serialTs & 0x80) {
        /* Serial TS */
        switch (system) {
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
            /* Intentional fall through */
            case SONY_DTV_SYSTEM_DVBT:
            case SONY_DTV_SYSTEM_DVBT2:
            case SONY_DTV_SYSTEM_DVBC:
            case SONY_DTV_SYSTEM_DVBC2:
                /* Terrestrial or Cable */
                tsClkConfiguration = serialTsClkSettings[pDemod->serialTsClockModeContinuous][(uint8_t)pDemod->serialTsClkFreqTerrCable];
                break;
#endif
            default:
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
        }
    }
    else {
        /* Parallel TS */
        tsClkConfiguration = parallelTsClkSetting;
    }

    if (serialTs & 0x80) {
        /* Serial TS, so set serial TS specific registers */        
                
        /* slave    Bank    Addr    Bit    default    Name
         * -----------------------------------------------------
         * <SLV-T>  00h     C4h     [1:0]  2'b01      OSERCKMODE
         */
        result = sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xC4, tsClkConfiguration.serialClkMode, 0x03);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* slave    Bank    Addr    Bit    default    Name
         * -------------------------------------------------------
         * <SLV-T>  00h     D1h     [1:0]  2'b01      OSERDUTYMODE
         */
        result = sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD1, tsClkConfiguration.serialDutyMode, 0x03);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
    }

    /* slave    Bank    Addr    Bit    default    Name
     * ------------------------------------------------------
     * <SLV-T>  00h     D9h     [7:0]  8'h08      OTSCKPERIOD
     */
    result = pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD9, tsClkConfiguration.tsClkPeriod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Disable TS IF Clock */
    /* slave    Bank    Addr    Bit    default    Name
     * -------------------------------------------------------
     * <SLV-T>  00h     32h     [0]    1'b1       OREG_CK_TSIF_EN
     */
    result = sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x32, 0x00, 0x01);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* slave    Bank    Addr    Bit    default    Name
     * -------------------------------------------------------
     * <SLV-T>  00h     33h     [1:0]  2'b01      OREG_CKSEL_TSIF
     */
    result = sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x33, tsClkConfiguration.clkSelTsIf, 0x03);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Enable TS IF Clock */
    /* slave    Bank    Addr    Bit    default    Name
     * -------------------------------------------------------
     * <SLV-T>  00h     32h     [0]    1'b1       OREG_CK_TSIF_EN
     */
    result = sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x32, 0x01, 0x01);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    SONY_TRACE_RETURN (result);
}

/*------------------------------------------------------------------------------
 Static Functions
------------------------------------------------------------------------------*/
static sony_result_t SDtoSLT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SDtoSLT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable oscillator */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x15, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (3);

    {
        uint8_t data[2] = {0x00, 0x00};

        /* Select ADC clock mode and X'tal clock mode*/
        if (pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) {
            data[0] = 0x02;
            data[1] = 0x02;
        }   
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) {
            data[0] = 0x00;
            data[1] = 0x00;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz) {
            data[0] = 0x02;
            data[1] = 0x03;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz) {
            data[0] = 0x00;
            data[1] = 0x01;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x13, &data[0], 2) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t SDtoSLS(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SDtoSLS");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable oscillator */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x15, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (3);

    {
        uint8_t data = 0x00;

        /* Select X'tal clock mode*/
        if (pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) {
            data = 0x02;
        }   
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) {
            data = 0x00;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz) {
            data = 0x03;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz) {
            data = 0x01;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x14, data) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQOUT */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x1F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQIN */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9C, 0x40) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
#endif

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x63, 0x16) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x65, 0x27) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x69, 0x06) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t SLTtoSD(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLTtoSD");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    
    /* Disable oscillator */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x15, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}
#if 0
static sony_result_t SLStoSD(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLStoSD");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable DSQOUT */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x3F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable DSQIN */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9C, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    
    /* Disable oscillator */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x15, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t SLStoSLT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLStoSLT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    
    {
        uint8_t data = 0x00;

        /* Select ADC clock mode and X'tal clock mode*/
        if ((pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) || (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz)) {
            data = 0x02;
        }   
        else if ((pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) || (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz)) {
            data = 0x00;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x13, data) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x63, 0x16) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x65, 0x27) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x69, 0x06) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}
#endif
static sony_result_t SLTtoSLS(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLTtoSLS");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    
    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQOUT */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x1F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQIN */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9C, 0x40) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x63, 0x16) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x65, 0x27) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x69, 0x06) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t XtoSLT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("XtoSLT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Clear all demodulator registers */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x02, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (3);

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    {
        uint8_t data[2] = {0x00, 0x00};

        /* Select ADC clock mode and X'tal clock mode*/
        if (pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) {
            data[0] = 0x02;
            data[1] = 0x02;
        }   
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) {
            data[0] = 0x00;
            data[1] = 0x00;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz) {
            data[0] = 0x02;
            data[1] = 0x03;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz) {
            data[0] = 0x00;
            data[1] = 0x01;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x13, &data[0], 2) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t XtoSLS(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("XtoSLS");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Clear all demodulator registers */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x02, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    
    SONY_SLEEP (3);

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    {
        uint8_t data = 0x00;

        /* Select X'tal clock mode*/
        if (pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) {
            data = 0x02;
        }   
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) {
            data = 0x00;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz) {
            data = 0x03;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz) {
            data = 0x01;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x14, data) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Clear demod SW reset */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x10, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_SLEEP (1);

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQOUT */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x1F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* enable DSQIN */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9C, 0x40) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
#endif

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x63, 0x16) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x65, 0x27) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* SADC Bias On */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x69, 0x06) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t clearConfigMemory (sony_demod_t * pDemod)
{
    SONY_TRACE_ENTER ("clearConfigMemory");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    pDemod->configMemoryLastEntry = 0;
    sony_memset(&(pDemod->configMemory), 0x00, sizeof(pDemod->configMemory));

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}

static sony_result_t loadConfigMemory (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    uint8_t i;

    SONY_TRACE_ENTER ("loadConfigMemory");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    for (i = 0; i < pDemod->configMemoryLastEntry; i++) {
        /* Set the bank */
        result = pDemod->pI2c->WriteOneRegister (pDemod->pI2c,
                                                 pDemod->configMemory[i].slaveAddress,
                                                 0x00,
                                                 pDemod->configMemory[i].bank);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        /* Write the register value */
        result = sony_i2c_SetRegisterBits (pDemod->pI2c,
                                           pDemod->configMemory[i].slaveAddress,
                                           pDemod->configMemory[i].registerAddress,
                                           pDemod->configMemory[i].value,
                                           pDemod->configMemory[i].bitMask);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t setConfigMemory (sony_demod_t * pDemod, 
                                                 uint8_t slaveAddress, 
                                                 uint8_t bank, 
                                                 uint8_t registerAddress,
                                                 uint8_t value,
                                                 uint8_t bitMask)
{
    uint8_t i;
    uint8_t valueStored = 0;

    SONY_TRACE_ENTER ("setConfigMemory");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Search for matching address entry already in table */
    for (i = 0; i < pDemod->configMemoryLastEntry; i++){
        if ((valueStored == 0) &&
            (pDemod->configMemory[i].slaveAddress == slaveAddress) &&
            (pDemod->configMemory[i].bank == bank) &&
            (pDemod->configMemory[i].registerAddress == registerAddress)) {
                /* Clear bits to overwrite / set  and then store the new value */
                pDemod->configMemory[i].value &= ~bitMask;
                pDemod->configMemory[i].value |= (value & bitMask);

                /* Add new bits to the bit mask */
                pDemod->configMemory[i].bitMask |= bitMask;

                valueStored = 1;
        }
    }
    
    /* If current register does not exist in the table, add a new entry to the end */
    if (valueStored == 0) {
        if (pDemod->configMemoryLastEntry < SONY_DEMOD_MAX_CONFIG_MEMORY_COUNT) {
            pDemod->configMemory[pDemod->configMemoryLastEntry].slaveAddress = slaveAddress;
            pDemod->configMemory[pDemod->configMemoryLastEntry].bank = bank;
            pDemod->configMemory[pDemod->configMemoryLastEntry].registerAddress = registerAddress;
            pDemod->configMemory[pDemod->configMemoryLastEntry].value = (value & bitMask);
            pDemod->configMemory[pDemod->configMemoryLastEntry].bitMask = bitMask;
            pDemod->configMemoryLastEntry++;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_OVERFLOW);
        }
    }

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}
