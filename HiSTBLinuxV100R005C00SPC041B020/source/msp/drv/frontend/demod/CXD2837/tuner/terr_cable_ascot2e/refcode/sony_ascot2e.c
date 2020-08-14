/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : 2012/07/24
  File Revision : 1.0.3.0
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 Based on ASCOT2E application note 2.0.0
------------------------------------------------------------------------------*/

#include "sony_ascot2e.h"

/*------------------------------------------------------------------------------
 Definitions of static const table
------------------------------------------------------------------------------*/
#define AUTO         (0xFF) /* For IF_OUT_SEL and AGC_SEL, it means that the value is desided by config flags. */
                            /* For RF_GAIN, it means that RF_GAIN_SEL(SubAddr:0x4E) = 1 */
#define OFFSET(ofs)  ((uint8_t)(ofs) & 0x1F)
#define BW_1_7       (0x03)
#define BW_6         (0x00)
#define BW_7         (0x01)
#define BW_8         (0x02)

/**
  @brief Sony silicon tuner setting for each broadcasting system.
         These values are optimized for Sony demodulators.
         The user have to change these values if other demodulators are used.
         Please check Sony silicon tuner application note for detail.
*/
static const sony_ascot2e_adjust_param_t g_param_table[SONY_ASCOT2E_TV_SYSTEM_NUM] = {
    /*
    IF_OUT_SEL   MIX_OLL    IF_BPF_GC          BW_OFFSET        RF_OLDET
      |   AGC_SEL |   RF_GAIN |    FIF_OFFSET   |          BW     |   IF_BPF_F0
      |     |     |     |     |     |           |           |     |     |       */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(0),  OFFSET(0),  BW_6, 0x09, 0x00}, /**< SONY_ASCOT2E_TV_SYSTEM_UNKNOWN */
    /* Analog */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(0),  OFFSET(0),  BW_6, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_MN_EIAJ  (System-M (Japan)) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(0),  OFFSET(0),  BW_6, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_MN_SAP   (System-M (US)) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(3),  OFFSET(0),  BW_6, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_MN_A2    (System-M (Korea)) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(11), OFFSET(8),  BW_7, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_BG       (System-B/G) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(2),  OFFSET(0),  BW_8, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_I        (System-I) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(2),  OFFSET(0),  BW_8, 0x09, 0x00}, /**< SONY_ASCOT2E_ATV_DK       (System-D/K) */
    {AUTO, AUTO, 0x05, AUTO, 0x01, OFFSET(2),  OFFSET(0),  BW_8, 0x0A, 0x00}, /**< SONY_ASCOT2E_ATV_L        (System-L) */
    {AUTO, AUTO, 0x05, AUTO, 0x01, OFFSET(-1), OFFSET(7),  BW_8, 0x0A, 0x00}, /**< SONY_ASCOT2E_ATV_L_DASH   (System-L DASH) */
    /* Digital */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-6), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_8VSB     (ATSC 8VSB) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-6), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_QAM      (US QAM) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-9), OFFSET(-7), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_ISDBT_6  (ISDB-T 6MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-7), OFFSET(-5), BW_7, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_ISDBT_7  (ISDB-T 7MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-5), OFFSET(-3), BW_8, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_ISDBT_8  (ISDB-T 8MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-8), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT_5   (DVB-T 5MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-8), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT_6   (DVB-T 6MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-6), OFFSET(-4), BW_7, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT_7   (DVB-T 7MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-4), OFFSET(-2), BW_8, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT_8   (DVB-T 8MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-5), OFFSET(0),  BW_1_7,0x0B, 0x00},/**< SONY_ASCOT2E_DTV_DVBT2_1_7(DVB-T2 1.7MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-8), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT2_5  (DVB-T2 5MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-8), OFFSET(-6), BW_6, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT2_6  (DVB-T2 6MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-6), OFFSET(-4), BW_7, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT2_7  (DVB-T2 7MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x06, OFFSET(-4), OFFSET(-2), BW_8, 0x0B, 0x00}, /**< SONY_ASCOT2E_DTV_DVBT2_8  (DVB-T2 8MHzBW) */
    {AUTO, AUTO, 0x02, AUTO, 0x03, OFFSET(-2), OFFSET(-1), BW_8, 0x09, 0x00}, /**< SONY_ASCOT2E_DTV_DVBC     (DVB-C) */
    {AUTO, AUTO, 0x03, AUTO, 0x01, OFFSET(-6), OFFSET(-4), BW_6, 0x09, 0x00}, /**< SONY_ASCOT2E_DTV_DVBC2_6  (DVB-C2 6MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x01, OFFSET(-2), OFFSET(2),  BW_8, 0x09, 0x00}, /**< SONY_ASCOT2E_DTV_DVBC2_8  (DVB-C2 8MHzBW) */
    {AUTO, AUTO, 0x03, AUTO, 0x02, OFFSET(2),  OFFSET(3),  BW_8, 0x0B, 0x00}  /**< SONY_ASCOT2E_DTV_DTMB     (DTMB) */
};


/*------------------------------------------------------------------------------
 Definitions of static functions
------------------------------------------------------------------------------*/
/**
 @brief Configure the ASCOT2E tuner from Power On to Sleep state.
*/
static sony_result_t X_pon(sony_ascot2e_t *pTuner);
/**
 @brief Configure the ASCOT2E tuner for specified broadcasting system.
*/
static sony_result_t X_init(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem);
/**
 @brief Configure the ASCOT2E tuner to Active state.
*/
static sony_result_t X_tune(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem, uint8_t vcoCal);
/**
 @brief The last part of tuning sequence.
*/
static sony_result_t X_tune_end(sony_ascot2e_t *pTuner);
/**
 @brief Configure the ASCOT2E tuner to Sleep state.
*/
static sony_result_t X_fin(sony_ascot2e_t *pTuner);
/**
 @brief Reading gain information to calculate IF and RF gain levels.
*/
static sony_result_t X_read_ss(sony_ascot2e_t *pTuner, uint8_t *pAGCReg, uint8_t *pRFAGCReg);

/*------------------------------------------------------------------------------
 Implementation of public functions.
------------------------------------------------------------------------------*/

sony_result_t sony_ascot2e_Create(sony_ascot2e_t *pTuner, uint32_t xtalFreqMHz,
    uint8_t i2cAddress, sony_i2c_t *pI2c, uint32_t flags)
{
    SONY_TRACE_ENTER("sony_ascot2e_Create");

    if((!pTuner) || (!pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Supported Xtal check */
    if((xtalFreqMHz != 16) && (xtalFreqMHz != 24) && (xtalFreqMHz != 41)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_NOSUPPORT);
    }

    /* 41MHz is only for external reference input */
    if((xtalFreqMHz == 41) && !(flags & SONY_ASCOT2E_CONFIG_EXT_REF)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_NOSUPPORT);
    }

    pTuner->state = SONY_ASCOT2E_STATE_UNKNOWN; /* Chip is not accessed for now */
    pTuner->xtalFreqMHz = xtalFreqMHz;
    pTuner->pI2c = pI2c;
    pTuner->i2cAddress = i2cAddress;
    pTuner->flags = flags;
    pTuner->frequencykHz = 0;
    pTuner->tvSystem = SONY_ASCOT2E_TV_SYSTEM_UNKNOWN;
    pTuner->rfagcreg = 0;
    pTuner->rfagcValid = 0;
    pTuner->pParamTable = g_param_table;
    pTuner->user = NULL;

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_Initialize(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_Initialize");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* X_pon sequence */
    result = X_pon(pTuner);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(result); }

    pTuner->state = SONY_ASCOT2E_STATE_SLEEP;
    pTuner->frequencykHz = 0;
    pTuner->tvSystem = SONY_ASCOT2E_TV_SYSTEM_UNKNOWN;

    pTuner->rfagcValid = 0;

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_Tune(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_Tune");

    /* Argument check */
    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if(!SONY_ASCOT2E_IS_ATV(tvSystem) && !SONY_ASCOT2E_IS_DTV(tvSystem)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* State check */
    if((pTuner->state != SONY_ASCOT2E_STATE_SLEEP) && (pTuner->state != SONY_ASCOT2E_STATE_ACTIVE)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    if(pTuner->state == SONY_ASCOT2E_STATE_SLEEP){
        /* Set system to "Unknown". (for safe) */
        pTuner->tvSystem = SONY_ASCOT2E_TV_SYSTEM_UNKNOWN;
    }

    /* In digital tuning, step size is 25kHz. (+ rounding) */
    if(SONY_ASCOT2E_IS_DTV(tvSystem)){
        frequencykHz = ((frequencykHz + 25/2) / 25) * 25;
    }

    /* Broadcasting system dependent setting */
    result = X_init(pTuner, frequencykHz, tvSystem);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_RETURN(result);
    }

    /* Tuning */
    result = X_tune(pTuner, frequencykHz, tvSystem, 1);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_RETURN(result);
    }

    pTuner->state = SONY_ASCOT2E_STATE_ACTIVE;
    pTuner->frequencykHz = frequencykHz;
    pTuner->tvSystem = tvSystem;

    /* Previous rfagcreg value should not be used for sony_ascot2e_ReadGain calculation. */
    pTuner->rfagcValid = 0;

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_TuneEnd(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_TuneEnd");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if(pTuner->state != SONY_ASCOT2E_STATE_ACTIVE){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    result = X_tune_end(pTuner);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(result); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_ShiftFRF(sony_ascot2e_t *pTuner, uint32_t frequencykHz)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_ShiftFRF");

    /* Argument check */
    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* State check */
    if(pTuner->state != SONY_ASCOT2E_STATE_ACTIVE){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    /* In digital tuning, step size is 25kHz. (+ rounding) */
    if(SONY_ASCOT2E_IS_DTV(pTuner->tvSystem)){
        frequencykHz = ((frequencykHz + 25/2) / 25) * 25;
    }

    /* Tune using current system, no VCO calibration */
    result = X_tune(pTuner, frequencykHz, pTuner->tvSystem, 0);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_RETURN(result);
    }

    pTuner->frequencykHz = frequencykHz;

    SONY_SLEEP(10);

    result = X_tune_end(pTuner);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(result); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_Sleep(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_Sleep");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if((pTuner->state != SONY_ASCOT2E_STATE_SLEEP) && (pTuner->state != SONY_ASCOT2E_STATE_ACTIVE)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    result = X_fin(pTuner);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(result); }
    
    pTuner->state = SONY_ASCOT2E_STATE_SLEEP;
    pTuner->frequencykHz = 0;
    pTuner->tvSystem = SONY_ASCOT2E_TV_SYSTEM_UNKNOWN;

    pTuner->rfagcValid = 0;

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_SetGPO(sony_ascot2e_t *pTuner, uint8_t id, uint8_t val)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_SetGPO");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    switch(id){
    case 0:
        result = sony_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, (uint8_t)(val ? 0x01 : 0x00), 0x01);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
        break;
    case 1:
        result = sony_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, (uint8_t)(val ? 0x02 : 0x00), 0x02);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
        break;
    default:
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_SetRfExtCtrl(sony_ascot2e_t *pTuner, uint8_t enable)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_SetRfExtCtrl");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x51, (uint8_t)(enable ? 0x01 : 0x00));
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_ReadGain(sony_ascot2e_t *pTuner, int32_t *pIFGain, int32_t *pRFGain, uint8_t forceRFAGCRead)
{
    sony_result_t result = SONY_RESULT_OK;
    uint8_t agcreg = 0;
    uint8_t rfagcreg = 0;

    SONY_TRACE_ENTER("sony_ascot2e_ReadGain");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if(pTuner->state != SONY_ASCOT2E_STATE_ACTIVE){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    if(forceRFAGCRead || !(pTuner->rfagcValid)){
        result = X_read_ss(pTuner, &agcreg, &rfagcreg);
        if(result != SONY_RESULT_OK){
            SONY_TRACE_RETURN(result);
        }
        /* Update stored RFAGC value */
        pTuner->rfagcreg = rfagcreg;
        pTuner->rfagcValid = 1;
    }else{
        /* Avoid RFAGC read */
        result = X_read_ss(pTuner, &agcreg, NULL);
        if(result != SONY_RESULT_OK){
            SONY_TRACE_RETURN(result);
        }
        /* Use previous RFAGC value */
        rfagcreg = pTuner->rfagcreg;
    }

    /*
        IFGAIN = if(AGC > 0.6){
                     6 + IF_BPF_GC 
                 }else{
                     if(AGC > 0.19){
                         6 + IF_BPF_GC + 69 * (0.6 - AGC)
                     }else{
                         6 + IF_BPF_GC + 69 * 0.41
                     }
                 }
        Note that AGC(V) = AGCReg(by X_read_ss) / 128
        So...
        IFGAIN(100xdB) = if(AGCReg * 100 > 7680){
                             (6 + IF_BPF_GC) * 100 
                         }else if(AGCReg * 100 > 2432){
                             (6 + IF_BPF_GC) * 100 + (69 * (7680 - AGCReg * 100))/128
                         }else{
                             (6 + IF_BPF_GC) * 100 + 69 * 41
                         }
    */
    {
        const int8_t if_bpf_gc_table[] = {-2, 0, 2, 4, 6, 8, 12, 20};
        uint8_t data = 0;
        int8_t if_bpf_gc = 0;
        int32_t agcreg_x100 = agcreg * 100;

        result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x0E, &data, 1);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        if_bpf_gc = if_bpf_gc_table[data & 0x07];

        if(agcreg_x100 > 7680){
            *pIFGain = (6 + if_bpf_gc) * 100;
        }else if(agcreg_x100 > 2432){
            *pIFGain = (6 + if_bpf_gc) * 100 + (69 * (7680 - agcreg_x100) + 64) / 128; /* Round */
        }else{
            *pIFGain = (6 + if_bpf_gc) * 100 + 69 * 41;
        }
    }

    /*
        RFGAIN = if(max(AGC,RFAGC) < 0.42){
                     RFGAIN_MAX
                 }else if(max(AGC,RFAGC) < 0.49){
                     RF_GAIN_MAX - 63 * (max(AGC,RFAGC) - 0.42)
                 }else if(max(AGC,RFAGC) < 0.6){
                     RF_GAIN_MAX - 63 * 0.07
                 }else if(max(AGC,RFAGC) < 1.13){
                     RF_GAIN_MAX - 63 * 0.07 - 63 * (max(AGC,RFAGC) - 0.6)
                 }else if(max(AGC,RFAGC) < 1.38){
                     RF_GAIN_MAX - 63 * 0.6 - 45 * (max(AGC,RFAGC) - 1.13)
                 }else{
                     RF_GAIN_MAX - 63 * 0.6 - 45 * 0.25 + RFGRAD * (max(AGC,RFAGC) - 1.38)
                 }

        Note that AGC(V) = AGCReg(by X_read_ss) / 128
                  RFAGC(V) = RFAGCReg(by X_read_ss) / 128
        So...
        RFGAIN(100xdB) = if(maxagcreg * 100 < 5376){
                             RFGAIN_MAX * 100
                         }else if(maxagcreg * 100 < 6272){
                             RFGAIN_MAX * 100 - (63 * (maxagcreg * 100 - 5376))/128
                         }else if(maxagcreg * 100 < 7680){
                             RFGAIN_MAX * 100 - 63 * 7
                         }else if(maxagcreg * 100 < 14464){
                             RFGAIN_MAX * 100 - 63 * 7 - (63 * (maxagcreg * 100 - 7680))/128
                         }else if(maxagcreg * 100 < 17664){
                             RFGAIN_MAX * 100 - 63 * 60 - (45 * (maxagcreg * 100 - 14464))/128
                         }else{
                             RFGAIN_MAX * 100 - 63 * 60 - 45 * 25 + RFGRAD * (maxagcreg * 100 - 17664))/128
                         }
        (NOTE: maxagcreg = max(AGCReg, RFAGCReg))
    */
    {
        int32_t maxagcreg_x100 = 0;
        int32_t rfgainmax_x100 = 0;

        if(agcreg > rfagcreg){
            maxagcreg_x100 = agcreg * 100;
        }else{
            maxagcreg_x100 = rfagcreg * 100;
        }

        if(pTuner->frequencykHz > 900000){
            rfgainmax_x100 = 4520;
        }else if(pTuner->frequencykHz > 700000){
            rfgainmax_x100 = 4620;
        }else if(pTuner->frequencykHz > 600000){
            rfgainmax_x100 = 4530;
        }else if(pTuner->frequencykHz > 504000){
            rfgainmax_x100 = 4360;
        }else if(pTuner->frequencykHz > 400000){
            rfgainmax_x100 = 4750;
        }else if(pTuner->frequencykHz > 320000){
            rfgainmax_x100 = 4600;
        }else if(pTuner->frequencykHz > 270000){
            rfgainmax_x100 = 4720;
        }else if(pTuner->frequencykHz > 235000){
            rfgainmax_x100 = 4570;
        }else if(pTuner->frequencykHz > 192000){
            rfgainmax_x100 = 4390;
        }else if(pTuner->frequencykHz > 130000){
            rfgainmax_x100 = 4750;
        }else if(pTuner->frequencykHz > 86000){
            rfgainmax_x100 = 4830;
        }else if(pTuner->frequencykHz > 50000){
            rfgainmax_x100 = 4550;
        }else{
            rfgainmax_x100 = 4650;
        }

        if(maxagcreg_x100 < 5376){
            *pRFGain = rfgainmax_x100;
        }else if(maxagcreg_x100 < 6272){
            *pRFGain = rfgainmax_x100 - (63 * (maxagcreg_x100 - 5376) + 64) / 128; /* Round */
        }else if(maxagcreg_x100 < 7680){
            *pRFGain = rfgainmax_x100 - 63 * 7;
        }else if(maxagcreg_x100 < 14464){
            *pRFGain = rfgainmax_x100 - 63 * 7 - (63 * (maxagcreg_x100 - 7680) + 64) / 128; /* Round */
        }else if(maxagcreg_x100 < 17664){
            *pRFGain = rfgainmax_x100 - 63 * 60 - (45 * (maxagcreg_x100 - 14464) + 64) / 128; /* Round */
        }else{
            int32_t rfgrad = -95;

            *pRFGain = rfgainmax_x100 - 63 * 60 - 45 * 25 + (rfgrad * (maxagcreg_x100 - 17664) - 64) / 128; /* Round */
        }
    }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

sony_result_t sony_ascot2e_RFFilterConfig(sony_ascot2e_t *pTuner, uint8_t coeff, uint8_t offset)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("sony_ascot2e_RFFilterConfig");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if((pTuner->state != SONY_ASCOT2E_STATE_SLEEP) && (pTuner->state != SONY_ASCOT2E_STATE_ACTIVE)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    /* Logic wake up, CPU wake up */
    {
        const uint8_t cdata[2] = {0xC4, 0x40};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    {
        uint8_t data[3];

        /* Write VL_TRCKOUT_COEFF */
        data[0] = coeff;
        data[1] = 0x49;
        data[2] = 0x03;
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x16, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        SONY_SLEEP(1);

        /* Write VL_TRCKOUT_OFS */
        data[0] = offset;
        data[1] = 0x4B;
        data[2] = 0x03;
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x16, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        SONY_SLEEP(1);
    }

    /* CPU deep sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Logic sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

/*------------------------------------------------------------------------------
 Implementation of static functions
------------------------------------------------------------------------------*/

static sony_result_t X_pon(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_pon");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Xtal==41MHz special setting */
    if(pTuner->xtalFreqMHz == 41){
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x44, 0x07);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x01 - 0x04 */
    {
        uint8_t data[4];

        /* Xtal frequency setting */
        data[0] = (uint8_t)(pTuner->xtalFreqMHz);

        /* VCO current setting */
        data[1] = 0x06;

        /* Logic wake up, CPU boot */
        data[2] = 0xC4;
        data[3] = 0x40;

        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x01, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x22 - 0x24 */
    {
        /* RFVGA optimization setting (RF_DIST0 - RF_DIST2) */
        const uint8_t cdata[] = {0x10, 0x3F, 0x25};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x22, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* PLL mode setting */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x28, 0x1E);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* RSSI setting */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x59, 0x04);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Wait 80ms */
    SONY_SLEEP(80);

    /* Check CPU_STT/CPU_ERR */
    {
        uint8_t rdata[2];

        result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x1A, rdata, sizeof(rdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        if(rdata[0] != 0x00){
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_HW_STATE); /* CPU_STT != 0x00 */
        }

#ifndef SONY_ASCOT2E_IGNORE_NVM_ERROR /* For no NVM tuner evaluation */
        if((rdata[1] & 0x3F) != 0x00){
            SONY_TRACE_RETURN(SONY_RESULT_ERROR_HW_STATE); /* CPU_ERR[5:0] != 0x00 (NVM Error) */
        }
#endif /* SONY_ASCOT2E_IGNORE_NVM_ERROR */
    }

    /* Xtal oscillator current control setting */
    if(!(pTuner->flags & SONY_ASCOT2E_CONFIG_EXT_REF)){
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x4C, 0x01);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* XOSC_SEL setting */
    {
        uint8_t data = 0;

        if(pTuner->flags & SONY_ASCOT2E_CONFIG_EXT_REF){ /* Use external Xtal */
            /* XOSC_SEL=0(disable) */
            data = 0x00;
        }else{
            /* XOSC_SEL=100uA */
            data = 0x04;
        }
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x07, data);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* CPU deep sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Logic sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Power save setting */
    {
        const uint8_t cdata[2] = {0x00, 0x04};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x50, 0x01);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

static sony_result_t X_init(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_init");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Disable power save */
    {
        const uint8_t cdata[2] = {0xFB, 0x0F};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x50, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* IF_OUT_SEL / AGC_SEL setting */
    {
        uint8_t data = 0x00;

        if(pTuner->pParamTable[tvSystem].AGC_SEL == AUTO){
            /* AGC pin setting by config flags */
            if(SONY_ASCOT2E_IS_ATV(tvSystem)){
                /* Analog */
                if(pTuner->flags & SONY_ASCOT2E_CONFIG_AGC2_ATV){
                    data |= 0x08;
                }
            }else{
                /* Digital */
                if(pTuner->flags & SONY_ASCOT2E_CONFIG_AGC2_DTV){
                    data |= 0x08;
                }
            }
        }else{
            /* AGC pin setting from parameter table */
            data |= (uint8_t)((pTuner->pParamTable[tvSystem].AGC_SEL & 0x03) << 3);
        }

        if(pTuner->pParamTable[tvSystem].IF_OUT_SEL == AUTO){
            /* IFOUT pin setting by config flags */
            if(SONY_ASCOT2E_IS_ATV(tvSystem)){
                /* Analog */
                if(pTuner->flags & SONY_ASCOT2E_CONFIG_IF2_ATV){
                    data |= 0x04;
                }
            }else{
                /* Digital */
                if(pTuner->flags & SONY_ASCOT2E_CONFIG_IF2_DTV){
                    data |= 0x04;
                }
            }
        }else{
            /* IFOUT pin setting from parameter table */
            data |= (uint8_t)((pTuner->pParamTable[tvSystem].IF_OUT_SEL & 0x01) << 2);
        }

        /* Set bit[4:2] only */
        result = sony_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, data, 0x1C);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x06 - 0x0F */
    {
        uint8_t data[10];

        /* REF_R setting (0x06) */
        if(SONY_ASCOT2E_IS_ATV(tvSystem) || (tvSystem == SONY_ASCOT2E_DTV_DVBC)){
            if(frequencykHz > 500000){
                data[0] = (uint8_t)((pTuner->xtalFreqMHz == 41) ? 40 : (pTuner->xtalFreqMHz));
            }else{
                data[0] = (uint8_t)((pTuner->xtalFreqMHz == 41) ? 80 : (pTuner->xtalFreqMHz * 2));
            }
        }else{
            if(frequencykHz > 500000){
                data[0] = (uint8_t)(pTuner->xtalFreqMHz / 8);
            }else{
                data[0] = (uint8_t)(pTuner->xtalFreqMHz / 4);
            }
        }

        /* XOSC_SEL setting (0x07) */
        if(pTuner->flags & SONY_ASCOT2E_CONFIG_EXT_REF){ /* Use external Xtal */
            /* XOSC_SEL=0(disable) */
            data[1] = 0x00;
        }else{
            /* XOSC_SEL=100uA */
            data[1] = 0x04;
        }

        /* KBW setting (0x08), KC0 setting (0x09), KC1 setting (0x0A) */
        if(SONY_ASCOT2E_IS_ATV(tvSystem) || (tvSystem == SONY_ASCOT2E_DTV_DVBC)){
            data[2] = 18;
            data[3] = 120;
            data[4] = 20;
        }else{
            data[2] = 48;
            data[3] = 10;
            data[4] = 30;
        }

        /* ORDER/R2_RANGE/R2_BANK/C2_BANK setting (0x0B) */
        if(SONY_ASCOT2E_IS_ATV(tvSystem) || (tvSystem == SONY_ASCOT2E_DTV_DVBC)){
            if(frequencykHz > 500000){
                /* ORDER=0(1-1 MASH), R2_RANGE=0, R2_BANK=10, C2_BANK=00(800pF) */
                data[5] = 0x08;
            }else{
                /* ORDER=0(1-1 MASH), R2_RANGE=0, R2_BANK=11, C2_BANK=00(800pF) */
                data[5] = 0x0C;
            }
        }else{
            if(frequencykHz > 500000){
                /* ORDER=1(1-1-1 MASH), R2_RANGE=1, R2_BANK=00, C2_BANK=00(800pF) */
                data[5] = 0x30;
            }else{
                /* ORDER=1(1-1-1 MASH), R2_RANGE=1, R2_BANK=10, C2_BANK=00(800pF) */
                data[5] = 0x38;
            }
        }

        /* Set MIX_OLL (0x0C) value from parameter table */
        data[6] = pTuner->pParamTable[tvSystem].MIX_OLL;

        /* Set RF_GAIN (0x0D) setting from parameter table */
        if(pTuner->pParamTable[tvSystem].RF_GAIN == AUTO){
            /* RF_GAIN auto control enable */
            result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x4E, 0x01);
            if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

            data[7] = 0x00; /* RF_GAIN Default value */
        }else{
            /* RF_GAIN auto control disable */
            result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x4E, 0x00);
            if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

            data[7] = pTuner->pParamTable[tvSystem].RF_GAIN;
        }

        /* Set IF_BPF_GC/FIF_OFFSET (0x0E) value from parameter table */
        data[8] = (uint8_t)((pTuner->pParamTable[tvSystem].FIF_OFFSET << 3) | (pTuner->pParamTable[tvSystem].IF_BPF_GC & 0x07));

        /* Set BW_OFFSET (0x0F) value from parameter table */
        data[9] = pTuner->pParamTable[tvSystem].BW_OFFSET;

        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x06, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x45 - 0x47 */
    {
        /* LNA optimization setting */
        /* RF_LNA_DIST1-5, RF_LNA_CM */
        uint8_t data[3];

        if(SONY_ASCOT2E_IS_ATV(tvSystem) || (tvSystem == SONY_ASCOT2E_DTV_DVBC)){
            data[0] = 0x0F;
            data[1] = 0x00;
            data[2] = 0x01;
        }else{
            data[0] = 0x0F;
            data[1] = 0x00;
            data[2] = 0x03;
        }

        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x45, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x49 - 0x4A */
    {
        uint8_t data[2];

        /* Set RF_OLDET_ENX/RF_OLDET_OLL value from parameter table */
        data[0] = pTuner->pParamTable[tvSystem].RF_OLDET;

        /* Set IF_BPF_F0 value from parameter table */
        data[1] = pTuner->pParamTable[tvSystem].IF_BPF_F0;

        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x49, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

static sony_result_t X_tune(sony_ascot2e_t *pTuner, uint32_t frequencykHz, sony_ascot2e_tv_system_t tvSystem, uint8_t vcoCal)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_tune");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* RFAGC fast mode / RFAGC auto control enable (set bit[7], bit[5:4] only) */
    {
        uint8_t data = 0;
        if(vcoCal){
            data = 0x90; /* Set MIX_OL_CPU_EN */
        }else{
            data = 0x10;
        }
        result = sony_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, data, 0xB0);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* Logic wake up, CPU wake up */
    {
        const uint8_t cdata[2] = {0xC4, 0x40};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* 0x10 - 0x14 */
    {
        uint8_t data[5];

        data[0] = (uint8_t)(frequencykHz & 0xFF);         /* 0x10: FRF_L */
        data[1] = (uint8_t)((frequencykHz >> 8) & 0xFF);  /* 0x11: FRF_M */
        data[2] = (uint8_t)((frequencykHz >> 16) & 0x0F); /* 0x12: FRF_H (bit[3:0]) */

        data[2] |= (uint8_t)(pTuner->pParamTable[tvSystem].BW << 4); /* 0x12: BW (bit[5:4]) */

        if(tvSystem == SONY_ASCOT2E_ATV_L_DASH){
            data[2] |= 0x40; /* 0x12: IS_L_DASH (bit[6]) */
        }

        if(SONY_ASCOT2E_IS_ATV(tvSystem)){
            data[2] |= 0x80; /* 0x12: IS_FP (bit[7]) */
        }

        if(vcoCal){
            data[3] = 0xFF; /* 0x13: VCO calibration enable */
        }else{
            data[3] = 0x8F; /* 0x13: VCO calibration disable */
        }

        data[4] = 0xFF; /* 0x14: Analog block enable */
        
        /* Tune (Burst write) */
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x10, data, sizeof(data));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

static sony_result_t X_tune_end(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_tune_end");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* CPU deep sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Logic sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* RFAGC normal mode (set bit[5:4] only) */
    result = sony_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, 0x00, 0x30);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

static sony_result_t X_fin(sony_ascot2e_t *pTuner)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_fin");

    if((!pTuner) || (!pTuner->pI2c)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Power save setting */
    {
        const uint8_t cdata[2] = {0x00, 0x04};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x50, 0x01);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}

static sony_result_t X_read_ss(sony_ascot2e_t *pTuner, uint8_t *pAGCReg, uint8_t *pRFAGCReg)
{
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER("X_read_ss");

    /* pRFAGCReg can be NULL (RFAGC read is skipped) */
    if((!pTuner) || (!pTuner->pI2c) || (!pAGCReg)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
    }

    if((pTuner->state != SONY_ASCOT2E_STATE_SLEEP) && (pTuner->state != SONY_ASCOT2E_STATE_ACTIVE)){
        SONY_TRACE_RETURN(SONY_RESULT_ERROR_SW_STATE);
    }

    /* Logic wake up, CPU wake up */
    {
        const uint8_t cdata[2] = {0xC4, 0x40};
        result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, cdata, sizeof(cdata));
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* ADC enable */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x59, 0x05);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Timer reset */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x18, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_SLEEP(1);

    /* ADC start */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0xF0);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x5A, 0x01);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* ADC read out */
    result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x5B, pAGCReg, 1);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    if(pRFAGCReg){
        /* Connect AGC */
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x59, 0x03);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        /* Timer reset */
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x18, 0x00);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        SONY_SLEEP(1);

        /* ADC start */
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0xF0);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x5A, 0x01);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
        result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

        /* ADC read out */
        result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x5B, pRFAGCReg, 1);
        if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }
    }

    /* ADC disable */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x59, 0x04);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* CPU deep sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    /* Logic sleep */
    result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
    if(result != SONY_RESULT_OK){ SONY_TRACE_RETURN(SONY_RESULT_ERROR_I2C); }

    SONY_TRACE_RETURN(SONY_RESULT_OK);
}
