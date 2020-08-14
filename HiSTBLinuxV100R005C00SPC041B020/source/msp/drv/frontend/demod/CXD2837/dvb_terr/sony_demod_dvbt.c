/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-10 14:13:38 #$
  File Revision : $Revision:: 6038 $
------------------------------------------------------------------------------*/

#include "sony_demod_dvbt.h"
#include "sony_demod_dvbt_monitor.h"
#include "hi_drv_tuner.h"

/*------------------------------------------------------------------------------
 Static Function Prototypes
------------------------------------------------------------------------------*/
/**
 @brief Configure the demodulator from Sleep T/C to Active for DVB-T
*/
static sony_result_t SLTtoAT(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator for tuner optimisations and bandwidth specific
        settings.
*/
static sony_result_t SLTtoAT_BandSetting(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from Active DVB-T to Active to DVB-T.  Used
        for changing channel parameters.
*/
static sony_result_t ATtoAT(sony_demod_t * pDemod);

/**
 @brief Configure the demodulator from Active DVB-T to Sleep T/C.
*/
static sony_result_t ATtoSLT(sony_demod_t * pDemod);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
sony_result_t sony_demod_dvbt_Tune (sony_demod_t * pDemod,
                                    sony_dvbt_tune_param_t * pTuneParam)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_Tune");

    if ((!pDemod) || (!pTuneParam)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state == SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->system == SONY_DTV_SYSTEM_DVBT)) {
        /* Demodulator Active and set to DVB-T mode */
        pDemod->bandwidth = pTuneParam->bandwidth;

        result = ATtoAT(pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
    }
    else if((pDemod->state == SONY_DEMOD_STATE_ACTIVE_T_C) && (pDemod->system != SONY_DTV_SYSTEM_DVBT)){
        /* Demodulator Active but not DVB-T mode */
        result = sony_demod_SleepT_C(pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }

        pDemod->system = SONY_DTV_SYSTEM_DVBT;
        pDemod->bandwidth = pTuneParam->bandwidth;

        result = SLTtoAT(pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
    }
    else if (pDemod->state == SONY_DEMOD_STATE_SLEEP_T_C) {
        /* Demodulator in Sleep T / C mode */
        pDemod->system = SONY_DTV_SYSTEM_DVBT;
        pDemod->bandwidth = pTuneParam->bandwidth;

        result = SLTtoAT (pDemod);
        if (result != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (result);
        }
    }
    else {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Update demodulator state */
    pDemod->state = SONY_DEMOD_STATE_ACTIVE_T_C;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_Sleep (sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_Sleep");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    result = ATtoSLT (pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }
    
    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_CheckDemodLock (sony_demod_t * pDemod, 
                                              sony_demod_lock_result_t * pLock)
{
    sony_result_t result = SONY_RESULT_OK;

    uint8_t sync = 0;
    uint8_t tslock = 0;
    uint8_t unlockDetected = 0;

    SONY_TRACE_ENTER ("sony_demod_dvbt_CheckDemodLock");

    if (!pDemod || !pLock) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    *pLock = SONY_DEMOD_LOCK_RESULT_NOTDETECT;

    result = sony_demod_dvbt_monitor_SyncStat (pDemod, &sync, &tslock, &unlockDetected);
    if (result != SONY_RESULT_OK) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        SONY_TRACE_RETURN (result);
    }

    if (unlockDetected) {
        *pLock = SONY_DEMOD_LOCK_RESULT_UNLOCKED;
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        SONY_TRACE_RETURN (SONY_RESULT_OK);
    }

    if (sync >= 6) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        *pLock = SONY_DEMOD_LOCK_RESULT_LOCKED;
    }

    HI_INFO_TUNER("%s, %d, result = %d\n", __func__, __LINE__, result);
    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_CheckTSLock (sony_demod_t * pDemod, 
                                           sony_demod_lock_result_t * pLock)
{
    sony_result_t result = SONY_RESULT_OK;

    uint8_t sync = 0;
    uint8_t tslock = 0;
    uint8_t unlockDetected = 0;

    SONY_TRACE_ENTER ("sony_demod_dvbt_CheckTSLock");

    if (!pDemod || !pLock) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE_T_C state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    *pLock = SONY_DEMOD_LOCK_RESULT_NOTDETECT;

    result = sony_demod_dvbt_monitor_SyncStat (pDemod, &sync, &tslock, &unlockDetected);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if (unlockDetected) {
        *pLock = SONY_DEMOD_LOCK_RESULT_UNLOCKED;
        SONY_TRACE_RETURN (SONY_RESULT_OK);
    }

    if ((sync >= 6) && tslock) {
        *pLock = SONY_DEMOD_LOCK_RESULT_LOCKED;
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_SetProfile (sony_demod_t * pDemod,
                                          sony_dvbt_profile_t profile)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_SetProfile");
    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
    if ((pDemod->state != SONY_DEMOD_STATE_SLEEP_T_C) && (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C)) {
        /* This api is accepted in SLEEP_T_C and ACTIVE_T_C states only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    /* Set DVB-T hierachy setting 
     * slave     Bank    Addr    Bit   default    Value    Name
     * -----------------------------------------------------------------
     * <SLV-T>   10h     67h     [0]   8'h00      8'h01    OREG_LPSELECT
     */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x67, (profile == SONY_DVBT_PROFILE_HP)? 0x00 : 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }
    SONY_TRACE_RETURN (result);
}

/*------------------------------------------------------------------------------
 Static Functions
------------------------------------------------------------------------------*/
static sony_result_t SLTtoAT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLTtoAT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Configure TS clock Mode and Frequency */
    result = sony_demod_SetTsClockModeAndFreq (pDemod, SONY_DTV_SYSTEM_DVBT);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set demod mode */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x17, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable demod clock */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2C, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if (pDemod->enableRfain) {
        /* Enable RF level monitor */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2F, 0x01) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }
    else {
        /* Disable RF level monitor */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2F, 0x00) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    {
        uint8_t data = 0x00;

        /* Enable ADC clock */
        if ((pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) || (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz)) {
            data = 0x01;
        }   
        else if ((pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) || (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz)) {
            data = 0x00;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }
         
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x30, data) != SONY_RESULT_OK)
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable ADC 1 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x1A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    {
        uint8_t data[2] = {0x00, 0x00};

        /* Enable ADC 2 & 3 */
        if ((pDemod->xtalFreq == SONY_DEMOD_XTAL_16000KHz) || (pDemod->xtalFreq == SONY_DEMOD_XTAL_24000KHz)) {
            data[0] = 0x09;
            data[1] = 0x57;
        }   
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_20500KHz) {
            data[0] = 0x09;
            data[1] = 0x54;
        }
        else if (pDemod->xtalFreq == SONY_DEMOD_XTAL_41000KHz) {
            data[0] = 0x0A;
            data[1] = 0xD4;
        }
        else {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_NOSUPPORT);
        }

        if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, &data[0], 2) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Enable ADC 4 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x18, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* IFAGC gain setting
     * slave     Bank    Addr   Bit     default    Value      Name
     * ----------------------------------------------------------------------------------
     * <SLV-T>   10h     D2h    [4:0]   8'h0C      8'h0C      OCTL_IFAGC_COARSEGAIN[4:0]
     */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD2, 0x0C, 0x1F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x11 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x11) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* BBAGC TARGET level setting
     * slave    Bank    Addr    Bit    default     Value          Name
     * ----------------------------------------------------------------------------------
     * <SLV-T>   11h     6Ah    [7:0]     8'h50      8'h50      OREG_ITB_DAGC_TRGT[7:0]
     */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x6A, 0x50) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if ((pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D) || 
        (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E) ||
        (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR)) {
        /* ASCOT setting ON
         * slave    Bank    Addr    Bit    default     Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     A5h     [0]      8'h01      8'h01      OREG_ITB_GDEQ_EN
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA5, 0x01, 0x01) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }
    else {
        /* ASCOT setting OFF
         * slave    Bank    Addr    Bit    default     Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     A5h     [0]      8'h01      8'h00      OREG_ITB_GDEQ_EN
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA5, 0x00, 0x01) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Set SLV-T Bank : 0x18 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x18) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Pre-RS BER moniter setting
     * slave     Bank    Addr   Bit     default    Value      Name
     * --------------------------------------------------------------------------------------------------------
     * <SLV-T>   18h     36h    [2:0]   8'h44      8'h40      OREG_RSD_BER_SEL[2:0],1'b0,OREG_RSD_BERN_SEL[2:0]
     */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x36, 0x40, 0x07) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TSIF setting
     * slave     Bank    Addr   Bit     default    Value      Name
     * ----------------------------------------------------------------------------------
     * <SLV-T>   00h     CEh     [0]      8'h00      8'h01      ONOPARITY
     * <SLV-T>   00h     CFh     [0]      8'h00      8'h01      ONOPARITY_MANUAL_ON
     */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xCE, 0x01, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xCF, 0x01, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TSIF setting
     * slave     Bank    Addr   Bit     default    Value      Name
     * ----------------------------------------------------------------------------------
     * <SLV-T>   10h     66h     [0]      8'h00      8'h01      OREG_TSIF_PCK_LENGTH
     */
    if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x66, 0x01, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set tuner and bandwidth specific settings */
    result = SLTtoAT_BandSetting (pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable HiZ Setting 1 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x28) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable HiZ Setting 2 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x81, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}

static sony_result_t SLTtoAT_BandSetting(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("SLTtoAT_BandSetting");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }
       
    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    switch (pDemod->bandwidth) {
    case SONY_DEMOD_BW_8_MHZ:
        {
            /*
             * <Timing Recovery setting>
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     9Fh    [5:0]     8'h11      8'h11      OREG_TRCG_NOMINALRATE[37:32]
             * <SLV-T>   10h     A0h    [7:0]     8'hF0      8'hF0      OREG_TRCG_NOMINALRATE[31:24]
             * <SLV-T>   10h     A1h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[23:16]
             * <SLV-T>   10h     A2h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[15:8]
             * <SLV-T>   10h     A3h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[7:0]
             */
            const uint8_t data[] = { 0x11, 0xF0, 0x00, 0x00, 0x00 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9F, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        if ((pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D) || (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E)) {
            /*
             * Group delay equaliser settings for ASCOT2D & ASCOT2E tuners
             * slave    bank    addr     bit      default   value        name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h26      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'hAF      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h06      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hCD      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h13      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hBB      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h28      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hBA      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h23      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hA9      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h1F      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'hA8      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h2C      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'hC8      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x26, 0xAF, 0x06, 0xCD, 0x13, 0xBB, 0x28, 0xBA, 0x23, 0xA9, 0x1F, 0xA8, 0x2C, 0xC8 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        else if (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR) {
            /*
             * Group delay equaliser settings for ASCOT2XR tuner
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h1E      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'h1D      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h29      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hC9      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h2A      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hBA      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h29      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hAD      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h29      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hA4      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h29      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'h9A      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h28      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'h9E      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x1E, 0x1D, 0x29, 0xC9, 0x2A, 0xBA, 0x29, 0xAD, 0x29, 0xA4, 0x29, 0x9A, 0x28, 0x9E };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /*
             * <IF freq setting>
             * slave    bank    addr    bit     default     value           name
             * ---------------------------------------------------------------------------------
             * <SLV-T>   10h     B6h    [7:0]     8'h1F     user defined    OREG_DNCNV_LOFRQ_T[23:16]
             * <SLV-T>   10h     B7h    [7:0]     8'h38     user defined    OREG_DNCNV_LOFRQ_T[15:8]
             * <SLV-T>   10h     B8h    [7:0]     8'h32     user defined    OREG_DNCNV_LOFRQ_T[7:0]
             */
            uint8_t data[3];
            data[0] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_8 >> 16) & 0xFF);
            data[1] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_8 >> 8) & 0xFF);
            data[2] = (uint8_t) (pDemod->iffreqConfig.configDVBT_8 & 0xFF);
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xB6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        /* System bandwidth setting
         * slave    Bank    Addr    Bit      default    Value      Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     D7h    [2:0]    8'h00      8'h00      OREG_CHANNEL_WIDTH[2:0]
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD7, 0x00, 0x07) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        {
            /* Demod core latency setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     D9h    [5:0]     8'h01      8'h01      OREG_CDRB_GTDOFST[13:8]
             * <SLV-T>   10h     DAh    [7:0]     8'hE0      8'hE0      OREG_CDRB_GTDOFST[7:0]
             */
            const uint8_t data[] = { 0x01, 0xE0 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD9, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /* Notch filter setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   17h     38h    [1:0]     8'h00      8'h01      OREG_CAS_CCIFLT2_EN_CW[1:0],OREG_CAS_CCIFLT2_EN_CW2[1:0]
             * <SLV-T>   17h     39h    [1:0]     8'h03      8'h02      OREG_CAS_CWSEQ_ON,OREG_CAS_CWSEQ_ON2
             */
            const uint8_t data[] = { 0x01, 0x02 };

            /* Set SLV-T Bank : 0x17 */
            if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x17) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x38, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        break;

    case SONY_DEMOD_BW_7_MHZ:
        {
            /*
             * <Timing Recovery setting>
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     9Fh    [5:0]     8'h11      8'h14      OREG_TRCG_NOMINALRATE[37:32]
             * <SLV-T>   10h     A0h    [7:0]     8'hF0      8'h80      OREG_TRCG_NOMINALRATE[31:24]
             * <SLV-T>   10h     A1h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[23:16]
             * <SLV-T>   10h     A2h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[15:8]
             * <SLV-T>   10h     A3h    [7:0]     8'h00      8'h00      OREG_TRCG_NOMINALRATE[7:0]
             */
            const uint8_t data[] = { 0x14, 0x80, 0x00, 0x00, 0x00 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9F, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        if ((pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D) || (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E)) {
            /*
             * Group delay equaliser settings for ASCOT2D & ASCOT2E tuners
             * slave    bank    addr     bit      default   value        name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h2C      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'hBD      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h02      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hCF      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h04      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hF8      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h23      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hA6      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h29      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hB0      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h26      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'hA9      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h21      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'hA5      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x2C, 0xBD, 0x02, 0xCF, 0x04, 0xF8, 0x23, 0xA6, 0x29, 0xB0, 0x26, 0xA9, 0x21, 0xA5 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        else if (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR) {
            /*
             * Group delay equaliser settings for ASCOT2XR tuner
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h1B      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'h22      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h2B      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hC1      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h2C      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hB3      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h2B      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hA9      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h2B      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hA0      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h2B      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'h97      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h2B      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'h9B      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x1B, 0x22, 0x2B, 0xC1, 0x2C, 0xB3, 0x2B, 0xA9, 0x2B, 0xA0, 0x2B, 0x97, 0x2B, 0x9B };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /*
             * <IF freq setting>
             * slave    bank    addr    bit     default     value           name
             * ---------------------------------------------------------------------------------
             * <SLV-T>   10h     B6h    [7:0]     8'h1F     user defined    OREG_DNCNV_LOFRQ_T[23:16]
             * <SLV-T>   10h     B7h    [7:0]     8'h38     user defined    OREG_DNCNV_LOFRQ_T[15:8]
             * <SLV-T>   10h     B8h    [7:0]     8'h32     user defined    OREG_DNCNV_LOFRQ_T[7:0]
             */
            uint8_t data[3];
            data[0] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_7 >> 16) & 0xFF);
            data[1] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_7 >> 8) & 0xFF);
            data[2] = (uint8_t) (pDemod->iffreqConfig.configDVBT_7 & 0xFF);
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xB6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        /* System bandwidth setting
         * slave    Bank    Addr    Bit      default    Value      Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     D7h    [2:0]    8'h00      8'h02      OREG_CHANNEL_WIDTH[2:0]
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD7, 0x02, 0x07) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        {
            /* Demod core latency setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     D9h    [5:0]     8'h01      8'h12      OREG_CDRB_GTDOFST[13:8]
             * <SLV-T>   10h     DAh    [7:0]     8'hE0      8'hF8      OREG_CDRB_GTDOFST[7:0]
             */
            const uint8_t data[] = { 0x12, 0xF8 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD9, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /* Notch filter setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   17h     38h    [1:0]     8'h00      8'h00      OREG_CAS_CCIFLT2_EN_CW[1:0],OREG_CAS_CCIFLT2_EN_CW2[1:0]
             * <SLV-T>   17h     39h    [1:0]     8'h03      8'h03      OREG_CAS_CWSEQ_ON,OREG_CAS_CWSEQ_ON2
             */
            const uint8_t data[] = { 0x00, 0x03 };
            
            /* Set SLV-T Bank : 0x17 */
            if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x17) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x38, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        break;

    case SONY_DEMOD_BW_6_MHZ:

        {
            /*
             * <Timing Recovery setting>
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     9Fh    [5:0]     8'h11      8'h17      OREG_TRCG_NOMINALRATE[37:32]
             * <SLV-T>   10h     A0h    [7:0]     8'hF0      8'hEA      OREG_TRCG_NOMINALRATE[31:24]
             * <SLV-T>   10h     A1h    [7:0]     8'h00      8'hAA      OREG_TRCG_NOMINALRATE[23:16]
             * <SLV-T>   10h     A2h    [7:0]     8'h00      8'hAA      OREG_TRCG_NOMINALRATE[15:8]
             * <SLV-T>   10h     A3h    [7:0]     8'h00      8'hAA      OREG_TRCG_NOMINALRATE[7:0]
             */
            const uint8_t data[] = { 0x17, 0xEA, 0xAA, 0xAA, 0xAA };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9F, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        if ((pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D) || (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E)) {
            /*
             * Group delay equaliser settings for ASCOT2D & ASCOT2E tuners
             * slave    bank    addr     bit      default   value        name
             * ----------------------------------------------------------------------------------
             *  <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h27      OREG_ITB_COEF01[7:0]
             *  <SLV-T>   10h     A7h    [7:0]     8'h1D      8'hA7      OREG_ITB_COEF02[7:0]
             *  <SLV-T>   10h     A8h    [7:0]     8'h29      8'h28      OREG_ITB_COEF11[7:0]
             *  <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hB3      OREG_ITB_COEF12[7:0]
             *  <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h02      OREG_ITB_COEF21[7:0]
             *  <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hF0      OREG_ITB_COEF22[7:0]
             *  <SLV-T>   10h     ACh    [7:0]     8'h29      8'h01      OREG_ITB_COEF31[7:0]
             *  <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hE8      OREG_ITB_COEF32[7:0]
             *  <SLV-T>   10h     AEh    [7:0]     8'h29      8'h00      OREG_ITB_COEF41[7:0]
             *  <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hCF      OREG_ITB_COEF42[7:0]
             *  <SLV-T>   10h     B0h    [7:0]     8'h29      8'h00      OREG_ITB_COEF51[7:0]
             *  <SLV-T>   10h     B1h    [7:0]     8'h9A      8'hE6      OREG_ITB_COEF52[7:0]
             *  <SLV-T>   10h     B2h    [7:0]     8'h28      8'h23      OREG_ITB_COEF61[7:0]
             *  <SLV-T>   10h     B3h    [7:0]     8'h9E      8'hA4      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x27, 0xA7, 0x28, 0xB3, 0x02, 0xF0, 0x01, 0xE8, 0x00, 0xCF, 0x00, 0xE6, 0x23, 0xA4 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        else if (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR) {
            /*
             * Group delay equaliser settings for ASCOT2XR tuner
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h19      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'h24      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h2B      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hB7      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h2C      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hAC      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h29      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hA6      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h2A      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'h9F      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h2A      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'h99      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h2A      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'h9B      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x19, 0x24, 0x2B, 0xB7, 0x2C, 0xAC, 0x29, 0xA6, 0x2A, 0x9F, 0x2A, 0x99, 0x2A, 0x9B };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /*
             * <IF freq setting>
             * slave    bank    addr    bit     default     value           name
             * ---------------------------------------------------------------------------------
             * <SLV-T>   10h     B6h    [7:0]     8'h1F     user defined    OREG_DNCNV_LOFRQ_T[23:16]
             * <SLV-T>   10h     B7h    [7:0]     8'h38     user defined    OREG_DNCNV_LOFRQ_T[15:8]
             * <SLV-T>   10h     B8h    [7:0]     8'h32     user defined    OREG_DNCNV_LOFRQ_T[7:0]
             */
            uint8_t data[3];
            data[0] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_6 >> 16) & 0xFF);
            data[1] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_6 >> 8) & 0xFF);
            data[2] = (uint8_t) (pDemod->iffreqConfig.configDVBT_6 & 0xFF);
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xB6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        /* System bandwidth setting
         * slave    Bank    Addr    Bit      default    Value      Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     D7h    [2:0]    8'h00      8'h04      OREG_CHANNEL_WIDTH[2:0]
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD7, 0x04, 0x07) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        {
            /* Demod core latency setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     D9h    [5:0]     8'h01      8'h1F      OREG_CDRB_GTDOFST[13:8]
             * <SLV-T>   10h     DAh    [7:0]     8'hE0      8'hDC      OREG_CDRB_GTDOFST[7:0]
             */
            const uint8_t data[] = { 0x1F, 0xDC };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD9, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /* Notch filter setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   17h     38h    [1:0]     8'h00      8'h00      OREG_CAS_CCIFLT2_EN_CW[1:0],OREG_CAS_CCIFLT2_EN_CW2[1:0]
             * <SLV-T>   17h     39h    [1:0]     8'h03      8'h03      OREG_CAS_CWSEQ_ON,OREG_CAS_CWSEQ_ON2
             */
            const uint8_t data[] = { 0x00, 0x03 };
            
            /* Set SLV-T Bank : 0x17 */
            if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x17) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x38, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        break;

    case SONY_DEMOD_BW_5_MHZ:
        {
            /*
             * <Timing Recovery setting>
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     9Fh    [5:0]     8'h11      8'h1C      OREG_TRCG_NOMINALRATE[37:32]
             * <SLV-T>   10h     A0h    [7:0]     8'hF0      8'hB3      OREG_TRCG_NOMINALRATE[31:24]
             * <SLV-T>   10h     A1h    [7:0]     8'h00      8'h33      OREG_TRCG_NOMINALRATE[23:16]
             * <SLV-T>   10h     A2h    [7:0]     8'h00      8'h33      OREG_TRCG_NOMINALRATE[15:8]
             * <SLV-T>   10h     A3h    [7:0]     8'h00      8'h33      OREG_TRCG_NOMINALRATE[7:0]
             */
            const uint8_t data[] = { 0x1C, 0xB3, 0x33, 0x33, 0x33 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9F, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        if ((pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2D) || (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E)) {
            /*
             * Group delay equaliser settings for ASCOT2D & ASCOT2E tuners
             * slave    bank    addr     bit      default   value        name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h27      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'hA7      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h28      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hB3      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h02      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hF0      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h01      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hE8      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h00      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'hCF      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h00      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'hE6      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h23      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'hA4      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x27, 0xA7, 0x28, 0xB3, 0x02, 0xF0, 0x01, 0xE8, 0x00, 0xCF, 0x00, 0xE6, 0x23, 0xA4 };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        else if (pDemod->tunerOptimize == SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2XR) {
            /*
             * Group delay equaliser settings for ASCOT2XR tuner
             * slave    bank    addr    bit     default     value      name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     A6h    [7:0]     8'h1E      8'h19      OREG_ITB_COEF01[7:0]
             * <SLV-T>   10h     A7h    [7:0]     8'h1D      8'h24      OREG_ITB_COEF02[7:0]
             * <SLV-T>   10h     A8h    [7:0]     8'h29      8'h2B      OREG_ITB_COEF11[7:0]
             * <SLV-T>   10h     A9h    [7:0]     8'hC9      8'hB7      OREG_ITB_COEF12[7:0]
             * <SLV-T>   10h     AAh    [7:0]     8'h2A      8'h2C      OREG_ITB_COEF21[7:0]
             * <SLV-T>   10h     ABh    [7:0]     8'hBA      8'hAC      OREG_ITB_COEF22[7:0]
             * <SLV-T>   10h     ACh    [7:0]     8'h29      8'h29      OREG_ITB_COEF31[7:0]
             * <SLV-T>   10h     ADh    [7:0]     8'hAD      8'hA6      OREG_ITB_COEF32[7:0]
             * <SLV-T>   10h     AEh    [7:0]     8'h29      8'h2A      OREG_ITB_COEF41[7:0]
             * <SLV-T>   10h     AFh    [7:0]     8'hA4      8'h9F      OREG_ITB_COEF42[7:0]
             * <SLV-T>   10h     B0h    [7:0]     8'h29      8'h2A      OREG_ITB_COEF51[7:0]
             * <SLV-T>   10h     B1h    [7:0]     8'h9A      8'h99      OREG_ITB_COEF52[7:0]
             * <SLV-T>   10h     B2h    [7:0]     8'h28      8'h2A      OREG_ITB_COEF61[7:0]
             * <SLV-T>   10h     B3h    [7:0]     8'h9E      8'h9B      OREG_ITB_COEF62[7:0]
             */
            const uint8_t data[] = { 0x19, 0x24, 0x2B, 0xB7, 0x2C, 0xAC, 0x29, 0xA6, 0x2A, 0x9F, 0x2A, 0x99, 0x2A, 0x9B };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xA6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        {
            /*
             * <IF freq setting>
             * slave    bank    addr    bit     default     value           name
             * ---------------------------------------------------------------------------------
             * <SLV-T>   10h     B6h    [7:0]     8'h1F     user defined    OREG_DNCNV_LOFRQ_T[23:16]
             * <SLV-T>   10h     B7h    [7:0]     8'h38     user defined    OREG_DNCNV_LOFRQ_T[15:8]
             * <SLV-T>   10h     B8h    [7:0]     8'h32     user defined    OREG_DNCNV_LOFRQ_T[7:0]
             */
            uint8_t data[3];
            data[0] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_5 >> 16) & 0xFF);
            data[1] = (uint8_t) ((pDemod->iffreqConfig.configDVBT_5 >> 8) & 0xFF);
            data[2] = (uint8_t) (pDemod->iffreqConfig.configDVBT_5 & 0xFF);
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xB6, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }

        /* System bandwidth setting
         * slave    Bank    Addr    Bit      default    Value      Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     D7h    [2:0]    8'h00      8'h06      OREG_CHANNEL_WIDTH[2:0]
         */
        if (sony_i2c_SetRegisterBits (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD7, 0x06, 0x07) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        {
            /* Demod core latency setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   10h     D9h    [5:0]     8'h01      8'h26      OREG_CDRB_GTDOFST[13:8]
             * <SLV-T>   10h     DAh    [7:0]     8'hE0      8'h3C      OREG_CDRB_GTDOFST[7:0]
             */
            const uint8_t data[] = { 0x26, 0x3C };
            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xD9, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }


        {
            /* Notch filter setting
             * slave    Bank    Addr    Bit      default    Value      Name
             * ----------------------------------------------------------------------------------
             * <SLV-T>   17h     38h    [1:0]     8'h00      8'h00      OREG_CAS_CCIFLT2_EN_CW[1:0],OREG_CAS_CCIFLT2_EN_CW2[1:0]
             * <SLV-T>   17h     39h    [1:0]     8'h03      8'h03      OREG_CAS_CWSEQ_ON,OREG_CAS_CWSEQ_ON2
             */
            const uint8_t data[] = { 0x00, 0x03 };

            /* Set SLV-T Bank : 0x17 */
            if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x17) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }

            if (pDemod->pI2c->WriteRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x38, data, sizeof (data)) != SONY_RESULT_OK) {
                SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
            }
        }
        break;

    default:
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    SONY_TRACE_RETURN (result);
}

static sony_result_t ATtoAT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("ATtoAT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable TS output */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xC3, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set tuner and bandwidth specific settings */
    result = SLTtoAT_BandSetting (pDemod);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }    

    SONY_TRACE_RETURN (result); 
}

static sony_result_t ATtoSLT(sony_demod_t * pDemod)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("ATtoSLT");

    if (!pDemod) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Set SLV-T Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable TS output */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xC3, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable Hi-Z setting 1 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x80, 0x3F) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Enable Hi-Z setting 2 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x81, 0xFF) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-X Bank : 0x00 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x00, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable ADC 1 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVX, 0x18, 0x01) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable ADC 2 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x43, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable ADC 3 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x41, 0x0A) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Disable ADC clock */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x30, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if (pDemod->enableRfain) {
        /* Disable RF level monitor */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2F, 0x00) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
    }

    /* Disable demod clock */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2C, 0x00) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SONY_TRACE_RETURN (result);
}
