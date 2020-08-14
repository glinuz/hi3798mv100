/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-09-18 14:30:37 #$
  File Revision : $Revision:: 6109 $
------------------------------------------------------------------------------*/

#include "sony_demod_dvbt.h"
#include "sony_demod_dvbt_monitor.h"
#include "sony_math.h"
#include "hi_drv_tuner.h"

/*------------------------------------------------------------------------------
 Static Function Prototypes
------------------------------------------------------------------------------*/
/*
 @brief Confirm demodulator lock
*/
static sony_result_t IsTPSLocked (sony_demod_t * pDemod);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
sony_result_t sony_demod_dvbt_monitor_SyncStat (sony_demod_t * pDemod, 
                                                uint8_t * pSyncStat, 
                                                uint8_t * pTSLockStat, 
                                                uint8_t * pUnlockDetected)
{
    uint8_t rdata = 0x00;
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_SyncStat");

    if ((!pDemod) || (!pSyncStat) || (!pTSLockStat) || (!pUnlockDetected)) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        HI_INFO_TUNER("%s, %d\n", __func__, __LINE__);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Slave    Bank    Addr    Bit     Name                Meaning
     * -----------------------------------------------------------------------
     * <SLV-T>   10h     10h     [2:0]  IREG_SEQ_OSTATE     0-5:UNLOCK, 6:LOCK
     * <SLV-T>   10h     10h     [4]    IEARLY_NOOFDM
     * <SLV-T>   10h     10h     [5]    IREG_TSIF_TS_LOCK     0:UNLOCK, 1:LOCK
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x10, &rdata, 1) != SONY_RESULT_OK) {
        HI_INFO_TUNER("%s, %d, rdata=%x\n", __func__, __LINE__, rdata);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    *pUnlockDetected = (uint8_t)((rdata & 0x10)? 1 : 0);
    *pSyncStat = (uint8_t)(rdata & 0x07);
    *pTSLockStat = (uint8_t)((rdata & 0x20) ? 1 : 0);

    /* Check for valid SyncStat value. */
    if (*pSyncStat == 0x07){
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_IFAGCOut (sony_demod_t * pDemod, 
                                                uint32_t * pIFAGCOut)
{
    uint8_t rdata[2];

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_IFAGCOut");

    if ((!pDemod) || (!pIFAGCOut)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Read pir_agc_gain
     * slave    Bank    Addr    Bit              Name
     * ------------------------------------------------------------
     * <SLV-T>   10h     26h     [3:0]        IIFAGC_OUT_T[11:8]
     * <SLV-T>   10h     27h     [7:0]        IIFAGC_OUT_T[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x26, rdata, 2) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    *pIFAGCOut = ((rdata[0] & 0x0F) << 8) | rdata[1];

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_ModeGuard (sony_demod_t * pDemod,
                                                 sony_dvbt_mode_t * pMode, 
                                                 sony_dvbt_guard_t * pGuard)
{
    uint8_t rdata = 0x00;

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_ModeGuard");

    if ((!pDemod) || (!pMode) || (!pGuard)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TPS Lock check */
    result = IsTPSLocked (pDemod);
    if (result != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (result);
    }
        
    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit              Name                meaning
     * ---------------------------------------------------------------------------------
     * <SLV-T>   10h     40h     [3:2]        IREG_MODE[1:0]     0:2K-mode, 1:8K-mode
     * <SLV-T>   10h     40h     [1:0]        IREG_GI[1:0]       0:1/32, 1:1/16, 2:1/8, 3:1/4
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x40, &rdata, 1) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    *pMode = (sony_dvbt_mode_t) ((rdata >> 2) & 0x03);
    *pGuard = (sony_dvbt_guard_t) (rdata & 0x03);   

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_CarrierOffset (sony_demod_t * pDemod, 
                                                     int32_t * pOffset)
{
    uint8_t rdata[4];
    uint32_t ctlVal = 0;

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_CarrierOffset");

    if ((!pDemod) || (!pOffset)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TPS Lock check */
    result = IsTPSLocked (pDemod);
    if (result != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /*  slave    Bank    Addr    Bit              Name               
     * ---------------------------------------------------------------
     * <SLV-T>   10h     4Ch     [4:0]      IREG_CRCG_CTLVAL[28:24]
     * <SLV-T>   10h     4Dh     [7:0]      IREG_CRCG_CTLVAL[23:16]
     * <SLV-T>   10h     4Eh     [7:0]      IREG_CRCG_CTLVAL[15:8]
     * <SLV-T>   10h     4Fh     [7:0]      IREG_CRCG_CTLVAL[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x4C, rdata, 4) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    /*
     * Carrier Offset [Hz] = -(IREG_CRCG_CTLVAL * (2^-28) * 8 * BW) / (7E-6)
     * 
     * Note: (2^-28 * 8 / 7E-6) = 4.257E-3
     * And: 1 / (2^-28 * 8 / 7E-6) = 234.88 = 235
     */
    ctlVal = ((rdata[0] & 0x1F) << 24) | (rdata[1] << 16) | (rdata[2] << 8) | (rdata[3]);
    *pOffset = sony_Convert2SComplement (ctlVal, 29);
    *pOffset = -1 * ((*pOffset) * (uint8_t)pDemod->bandwidth / 235);

    /* Compensate for inverted spectrum. */
    {
        /* Inverted RF spectrum with non-inverting tuner architecture. */
        if (pDemod->confSense == SONY_DEMOD_TERR_CABLE_SPECTRUM_INV) {
            *pOffset *= -1;
        }
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_PreViterbiBER (sony_demod_t * pDemod, 
                                                     uint32_t * pBER)
{
    uint8_t rdata[2];
    uint32_t bitError = 0;
    uint32_t period = 0;

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_PreViterbiBER");

    if ((!pDemod) || (!pBER)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit              Name           
     * ------------------------------------------------------------
     * <SLV-T>   10h     39h     [0]          IREG_VBER_ACT
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x39, rdata, 1) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if ((rdata[0] & 0x01) == 0) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);    /* Not ready... */
    }

    /* slave    Bank    Addr    Bit              Name           
     * ------------------------------------------------------------
     * <SLV-T>   10h     22h     [7:0]        IREG_VBER_BITECNT[15:8]
     * <SLV-T>   10h     23h     [7:0]        IREG_VBER_BITECNT[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x22, rdata, 2) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    bitError = (rdata[0] << 8) | rdata[1];

    /* Read vber_period
     * slave    Bank    Addr    Bit    default          Name
     * ------------------------------------------------------------
     * <SLV-T>   10h     6Fh     [2:0]    3'd1      OREG_VBER_PERIOD_SEL
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x6F, rdata, 1) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    period = ((rdata[0] & 0x07) == 0) ? 256 : (0x1000 << (rdata[0] & 0x07));

    if ((period == 0) || (bitError > period)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    /*
      BER = bitError * 10000000 / period
          = bitError * 78125 / (period / 128)
          = bitError * 3125 * 25 / (period / 128)
          (Divide in 2 steps to prevent overflow.)
    */
    {
        uint32_t div = 0;
        uint32_t Q = 0;
        uint32_t R = 0;

        div = period / 128;

        Q = (bitError * 3125) / div;
        R = (bitError * 3125) % div;

        R *= 25;
        Q = Q * 25 + R / div;
        R = R % div;

        /* rounding */
        if (R >= div/2) {
            *pBER = Q + 1;
        } 
        else {
            *pBER = Q;
        }
    }
    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_PreRSBER (sony_demod_t * pDemod, 
                                                uint32_t * pBER)
{
    uint8_t rdata[3];
    uint32_t bitError = 0;
    uint32_t periodExp = 0;

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_PreRSBER");

    if ((!pDemod) || (!pBER)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    
    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /*   slave    Bank    Addr    Bit              Name           
     * ------------------------------------------------------------
     * <SLV-T>   10h     62h     [5:0]        IREG_BERN_BITECNT[21:16]
     * <SLV-T>   10h     63h     [7:0]        IREG_BERN_BITECNT[15:8]
     * <SLV-T>   10h     64h     [7:0]        IREG_BERN_BITECNT[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x62, rdata, 3) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Check IREG_BERN_VALID bit (bit 7) */
    /* slave    Bank    Addr    Bit              Name           
     * ------------------------------------------------------------
     * <SLV-T>   10h     62h     [7]          IREG_BERN_VALID
     */
    if ((rdata[0] & 0x80) == 0) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);    /* Not ready... */
    }

    bitError = ((rdata[0] & 0x3F) << 16) | (rdata[1] << 8) | rdata[2];

    /* Read ber_period */
    /* slave    Bank    Addr    Bit    default          Name
     * -------------------------------------------------------------------------
     * <SLV-T>   10h     60h    [4:0]     5'h0B        OREG_BERN_PERIOD[4:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x60, rdata, 1) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    periodExp = (rdata[0] & 0x1F);

    if ((periodExp <= 11) && (bitError > (1U << periodExp) * 204 * 8)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    /*
      BER = bitError * 10000000 / (2^N * 204 * 8)
          = bitError * 312500 / (2^N * 51)
          = bitError * 250 * 1250 / (2^N * 51)
          (Divide in 2 steps to prevent overflow.)
    */
    {
        uint32_t div = 0;
        uint32_t Q = 0;
        uint32_t R = 0;

        if (periodExp <= 8) {
            div = (1U << periodExp) * 51;
        } 
        else {
            div = (1U << 8) * 51;
        }

        Q = (bitError * 250) / div;
        R = (bitError * 250) % div;

        R *= 1250;
        Q = Q * 1250 + R / div;
        R = R % div;

        if (periodExp > 8) {
            /* rounding */
            *pBER = (Q + (1 << (periodExp - 9))) >> (periodExp - 8);
        } 
        else {
            /* rounding */
            if (R >= div/2) {
                *pBER = Q + 1;
            } 
            else {
                *pBER = Q;
            }
        }
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_TPSInfo (sony_demod_t * pDemod, 
                                               sony_dvbt_tpsinfo_t * pInfo)
{
    uint8_t rdata[7];

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_TPSInfo");

    if ((!pDemod) || (!pInfo)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TPS Lock check */
    result = IsTPSLocked (pDemod);
    if (result != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit              Name           
     * ------------------------------------------------------------
     * <SLV-T>   10h     2Fh     [7:6]       ITPS_CNST[1:0]
     * <SLV-T>   10h     2Fh     [5:3]       ITPS_HIER[2:0]
     * <SLV-T>   10h     2Fh     [2:0]       ITPS_HRATE[2:0]
     * <SLV-T>   10h     30h     [7:5]       ITPS_LRATE[2:0]
     * <SLV-T>   10h     30h     [4:3]       ITPS_GI[1:0]
     * <SLV-T>   10h     30h     [2:1]       ITPS_MODE[1:0]
     * <SLV-T>   10h     31h     [7:6]       ITPS_FNUM[1:0]
     * <SLV-T>   10h     31h     [5:0]       ITPS_LENGTH_INDICATOR[5:0]
     * <SLV-T>   10h     32h     [7:0]       ITPS_CELLID[15:8]
     * <SLV-T>   10h     33h     [7:0]       ITPS_CELLID[7:0]
     * <SLV-T>   10h     34h     [5:0]       ITPS_RESERVE_EVEN[5:0]
     * <SLV-T>   10h     35h     [5:0]       ITPS_RESERVE_ODD[5:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x2F, rdata, 7) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    pInfo->constellation = (sony_dvbt_constellation_t) ((rdata[0] >> 6) & 0x03);
    pInfo->hierarchy = (sony_dvbt_hierarchy_t) ((rdata[0] >> 3) & 0x07);
    pInfo->rateHP = (sony_dvbt_coderate_t) (rdata[0] & 0x07);
    pInfo->rateLP = (sony_dvbt_coderate_t) ((rdata[1] >> 5) & 0x07);
    pInfo->guard = (sony_dvbt_guard_t) ((rdata[1] >> 3) & 0x03);
    pInfo->mode = (sony_dvbt_mode_t) ((rdata[1] >> 1) & 0x03);
    pInfo->fnum = (rdata[2] >> 6) & 0x03; 
    pInfo->lengthIndicator = rdata[2] & 0x3F;
    pInfo->cellID = (uint16_t) ((rdata[3] << 8) | rdata[4]);
    pInfo->reservedEven = rdata[5] & 0x3F;
    pInfo->reservedOdd = rdata[6] & 0x3F;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_PacketErrorNumber (sony_demod_t * pDemod, 
                                                         uint32_t * pPEN)
{
    uint8_t rdata[3];

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_PacketErrorNumber");

    if ((!pDemod) || (!pPEN)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }
    
    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* Read ber_cwrjctcnt
     * slave    Bank    Addr    Bit              Name
     * ------------------------------------------------------------
     * <SLV-T>   10h     EAh     [7:0]        IREG_BER_CWRJCTCNT[15:8]
     * <SLV-T>   10h     EBh     [7:0]        IREG_BER_CWRJCTCNT[7:0]
     * <SLV-T>   10h     ECh     [0]          IREG_BER_CWRJCTCNT_VALID
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0xEA, rdata, 3) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if (!(rdata[2] & 0x01)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    *pPEN = (rdata[0] << 8) | rdata[1]; 

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_SpectrumSense (sony_demod_t * pDemod, 
                                                     sony_demod_terr_cable_spectrum_sense_t * pSense)
{
    uint8_t data = 0;

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_SpectrumSense");

    if ((!pDemod) || (!pSense)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TPS Lock check */
    result = IsTPSLocked (pDemod);
    if (result != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit              Name                mean
     * ---------------------------------------------------------------------------------
     * <SLV-T>   10h     3Fh     [0]        IREG_COSNE_SINV      0:not invert,   1:invert
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x3F, &data, sizeof (data)) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    *pSense = (data & 0x01) ? SONY_DEMOD_TERR_CABLE_SPECTRUM_INV : SONY_DEMOD_TERR_CABLE_SPECTRUM_NORMAL;   

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_SNR (sony_demod_t * pDemod, 
                                           int32_t * pSNR)
{
    uint16_t reg = 0;
    uint8_t rdata[2];

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_SNR");

    if ((!pDemod) || (!pSNR)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Freeze registers */
    if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* TPS Lock check */
    result = IsTPSLocked (pDemod);
    if (result != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (result);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /* slave    Bank    Addr    Bit              Name
     * ------------------------------------------------------------
     * <SLV-T>   10h     28h     [7:0]        IREG_SNMON_OD[15:8]
     * <SLV-T>   10h     29h     [7:0]        IREG_SNMON_OD[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x28, rdata, 2) != SONY_RESULT_OK) {
        SLVT_UnFreezeReg (pDemod);
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    SLVT_UnFreezeReg (pDemod);

    reg = (rdata[0] << 8) | rdata[1];

    /* Confirm valid range, clip as necessary */
    if (reg == 0) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    /* IREG_SNMON_OD is clipped at a maximum of 4996.
     *
     * SNR = 10 * log10 (IREG_SNMON_OD / (5350 - IREG_SNMON_OD)) + 28.5
     *     = 10 * (log10(IREG_SNMON_OD) - log10(5350 - IREG_SNMON_OD)) + 28.5
     * sony_log10 returns log10(x) * 100
     * Therefore SNR(dB) * 1000 :
     *     = 10 * 10 * (sony_log10(IREG_SNMON_OD) - sony_log10(5350 - IREG_SNMON_OD) + 28500
     */
    if (reg > 4996) {
        reg = 4996;
    }

    *pSNR = 10 * 10 * ((int32_t) sony_math_log10 (reg) - (int32_t) sony_math_log10 (5350 - reg));
    *pSNR += 28500;

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_SamplingOffset (sony_demod_t * pDemod, 
                                                      int32_t * pPPM)
{
    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_SamplingOffset");

    if ((!pDemod) || (!pPPM)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T*/
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    {
        uint8_t ctlValReg[5];
        uint8_t nominalRateReg[5];
        uint32_t trlCtlVal = 0;
        uint32_t trcgNominalRate = 0;
        int32_t num;
        int32_t den;
        sony_result_t result = SONY_RESULT_OK;

        /* Freeze registers */
        if (SLVT_FreezeReg (pDemod) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        /* TPS Lock check */
        result = IsTPSLocked (pDemod);
        if (result != SONY_RESULT_OK) {
            SLVT_UnFreezeReg (pDemod);
            SONY_TRACE_RETURN (result);
        }

        /* Set SLV-T Bank : 0x10 */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
            SLVT_UnFreezeReg (pDemod);
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }
        
        /* slave    Bank    Addr    Bit              name
         * ---------------------------------------------------------------
         * <SLV-T>   10h     52h     [6:0]      IREG_TRL_CTLVAL_S[38:32]
         * <SLV-T>   10h     53h     [7:0]      IREG_TRL_CTLVAL_S[31:24]
         * <SLV-T>   10h     54h     [7:0]      IREG_TRL_CTLVAL_S[23:16]
         * <SLV-T>   10h     55h     [7:0]      IREG_TRL_CTLVAL_S[15:8]
         * <SLV-T>   10h     56h     [7:0]      IREG_TRL_CTLVAL_S[7:0]
         * <SLV-T>   10h     9Fh     [6:0]      OREG_TRCG_NOMINALRATE[38:32]
         * <SLV-T>   10h     A0h     [7:0]      OREG_TRCG_NOMINALRATE[31:24]
         * <SLV-T>   10h     A1h     [7:0]      OREG_TRCG_NOMINALRATE[23:16]
         * <SLV-T>   10h     A2h     [7:0]      OREG_TRCG_NOMINALRATE[15:8]
         * <SLV-T>   10h     A3h     [7:0]      OREG_TRCG_NOMINALRATE[7:0]
         */
        if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x52, ctlValReg, sizeof (ctlValReg)) != SONY_RESULT_OK) {
            SLVT_UnFreezeReg (pDemod);
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x9F, nominalRateReg, sizeof (nominalRateReg)) != SONY_RESULT_OK) {
            SLVT_UnFreezeReg (pDemod);
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        /* Unfreeze registers. */
        SLVT_UnFreezeReg (pDemod);

        /* Confirm offset range (comparing top 7 bits) */
        if ((ctlValReg[0] & 0x7F) != (nominalRateReg[0] & 0x7F)) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
        }

        /* Clock frequency offset(ppm)  = { ( IREG_TRL_CTLVAL_S - OREG_TRCG_NOMINALRATE ) / OREG_TRCG_NOMINALRATE } * 1000000
         * Numerator = IREG_TRL_CTLVAL_S - OREG_TRCG_NOMINALRATE
         * Denominator = OREG_TRCG_NOMINALRATE / 1*10^8
         */

        /* Top 7 bits can be ignored for subtraction as out of range. */
        trlCtlVal = ctlValReg[1] << 24;
        trlCtlVal |= ctlValReg[2] << 16;
        trlCtlVal |= ctlValReg[3] << 8;
        trlCtlVal |= ctlValReg[4];

        trcgNominalRate = nominalRateReg[1] << 24;
        trcgNominalRate |= nominalRateReg[2] << 16;
        trcgNominalRate |= nominalRateReg[3] << 8;
        trcgNominalRate |= nominalRateReg[4];

        /* Shift down 1 bit to avoid overflow in subtraction */
        trlCtlVal >>= 1;
        trcgNominalRate >>= 1;

        num = trlCtlVal - trcgNominalRate;
        
        /* OREG_TRCG_NOMINALRATE is 39bit therefore:
         * Denominator = (OREG_TRCG_NOMINALRATE [38:8] * 256) / 1*10^8
         *             = (OREG_TRCG_NOMINALRATE [38:8] * 2^8) / (2^8 * 5^8)
         *             = OREG_TRCG_NOMINALRATE [38:8] / 390625
         */
        den = (nominalRateReg[0] & 0x7F) << 24;
        den |= nominalRateReg[1] << 16;
        den |= nominalRateReg[2] << 8;
        den |= nominalRateReg[3];
        den = (den + (390625/2)) / 390625;
        
        /* Shift down to align with numerator */
        den >>= 1;

        /* Perform calculation */
        if (num >= 0) {
            *pPPM = (num + (den/2)) / den;
        } 
        else {
            *pPPM = (num - (den/2)) / den;
        }
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_Quality (sony_demod_t * pDemod, 
                                               uint8_t * pQuality)
{
    sony_dvbt_tpsinfo_t tps;
    sony_dvbt_profile_t profile = SONY_DVBT_PROFILE_HP;
    uint32_t ber = 0;
    int32_t sn = 0;
    int32_t snRel = 0;
    int32_t berSQI = 0;

    /**
     @brief The list of DVB-T Nordig Profile 1 for Non Hierachical signal
            C/N values in dBx1000.
    */
    static const int32_t nordigNonHDVBTdB1000[3][5] = {
    /*   1/2,   2/3,   3/4,   5/6,   7/8                  */    
        {5100,  6900,  7900,  8900,  9700},     /* QPSK   */
        {10800, 13100, 14600, 15600, 16000},    /* 16-QAM */
        {16500, 18700, 20200, 21600, 22500}     /* 64-QAM */
    };

    /**
     @brief The list of DVB-T Nordig Profile 1 for Hierachical signal, HP
            C/N values in dBx1000.
    */
    static const int32_t nordigHierHpDVBTdB1000[3][2][5] = {
          /* alpha = 1                                                */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                      */
            {9100,   12000,  13600,  15000,  16600},   /* LP = 16-QAM */
            {10900,  14100,  15700,  19400,  20600}    /* LP = 64-QAM */
        },/* alpha = 2                                                */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                      */
            {6800,   9100,   10400,  11900,  12700},   /* LP = 16-QAM */
            {8500,   11000,  12800,  15000,  16000}    /* LP = 64-QAM */
        },/* alpha = 4                                                */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                      */
            {5800,   7900,   9100,   10300,  12100},   /* LP = 16-QAM */
            {8000,   9300,   11600,  13000,  12900}    /* LP = 64-QAM */
        }
    };
    /**
     @brief The list of DVB-T Nordig Profile 1 for Hierachical signal, LP
            C/N values in dBx1000.
    */
    static const int32_t nordigHierLpDVBTdB1000[3][2][5] = {
          /* alpha = 1                                           */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                 */
            {12500,  14300,  15300,  16300,  16900},   /* 16-QAM */
            {16700,  19100,  20900,  22500,  23700}    /* 64-QAM */
        },/* alpha = 2                                           */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                 */
            {15000,  17200,  18400,  19100,  20100},   /* 16-QAM */
            {18500,  21200,  23600,  24700,  25900}    /* 64-QAM */
        },/* alpha = 4                                           */ 
        { /* 1/2,    2/3,    3/4,    5/6,    7/8                 */
            {19500,  21400,  22500,  23700,  24700},   /* 16-QAM */
            {21900,  24200,  25600,  26900,  27800}    /* 64-QAM */
        }
    };

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_Quality");

    if ((!pDemod) || (!pQuality)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Monitor TPS for Modulation / Code Rate */
    result = sony_demod_dvbt_monitor_TPSInfo (pDemod, &tps);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Monitor profile for hierarchical signal*/
    if (tps.hierarchy != SONY_DVBT_HIERARCHY_NON) {
        uint8_t data = 0;
        /* Set SLV-T Bank : 0x10 */
        if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        /* slave    Bank    Addr    Bit    default    Value          Name
         * ----------------------------------------------------------------------------------
         * <SLV-T>   10h     67h     [0]      8'h00      8'h01       OREG_LPSELECT
         */
        if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x67, &data, 1) != SONY_RESULT_OK) {
            SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
        }

        profile = ((data & 0x01) == 0x01) ? SONY_DVBT_PROFILE_LP : SONY_DVBT_PROFILE_HP;
    }

    /* Get Pre-RS (Post-Viterbi) BER. */
    result = sony_demod_dvbt_monitor_PreRSBER (pDemod, &ber);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Get SNR value. */
    result = sony_demod_dvbt_monitor_SNR (pDemod, &sn);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    /* Ensure correct TPS values. */
    if ((tps.constellation >= SONY_DVBT_CONSTELLATION_RESERVED_3) || 
        (tps.rateHP >= SONY_DVBT_CODERATE_RESERVED_5) || 
        (tps.rateLP >= SONY_DVBT_CODERATE_RESERVED_5) || 
        (tps.hierarchy > SONY_DVBT_HIERARCHY_4)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_OTHER);
    }

    /* Hierachical transmission with QPSK modulation is an invalid combination */
    if ((tps.hierarchy != SONY_DVBT_HIERARCHY_NON) && (tps.constellation == SONY_DVBT_CONSTELLATION_QPSK)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_OTHER);
    }

    /* Obtain snRel based on code rate and modulation */
    if (tps.hierarchy == SONY_DVBT_HIERARCHY_NON) {
        snRel = sn - nordigNonHDVBTdB1000[tps.constellation][tps.rateHP];        
    }
    else if ( profile == SONY_DVBT_PROFILE_LP ) {
        snRel = sn - nordigHierLpDVBTdB1000[(int32_t)tps.hierarchy-1][(int32_t)tps.constellation-1][tps.rateLP];
    }
    else {
        snRel = sn - nordigHierHpDVBTdB1000[(int32_t)tps.hierarchy-1][(int32_t)tps.constellation-1][tps.rateHP];
    }
      
    /* BER_SQI is calculated from:
     * if (BER > 10^-3)          : 0
     * if (10^-7 < BER <= 10^-3) : BER_SQI = 20*log10(1/BER) - 40
     * if (BER <= 10^-7)         : BER_SQI = 100
      */
    if (ber > 10000) {
        berSQI = 0;
    }
    else if (ber > 1) {
        /* BER_SQI = 20 * log10(1/BER) - 40
         * BER_SQI = 20 * (log10(1) - log10(BER)) - 40
         * 
         * If BER in units of 1e-7
         * BER_SQI = 20 * (log10(1) - (log10(BER) - log10(1e7)) - 40
         * 
         * BER_SQI = 20 * (log10(1e7) - log10(BER)) - 40
         * BER_SQI = 20 * (7 - log10 (BER)) - 40
         */
        berSQI = (int32_t) (10 * sony_math_log10 (ber));
        berSQI = 20 * (7 * 1000 - (berSQI)) - 40 * 1000;
    }
    else {
        berSQI = 100 * 1000;
    }

    /* SQI (Signal Quality Indicator) given by:
     * if (C/Nrel < -7dB)         : SQI = 0
     * if (-7dB <= C/Nrel < +3dB) : SQI = (((C/Nrel - 3) / 10) + 1) * BER_SQI
     * if (C/Nrel >= +3dB)        : SQI = BER_SQI
     */
    if (snRel < -7 * 1000) {
        *pQuality = 0;
    }
    else if (snRel < 3 * 1000) {
        int32_t tmpSQI = (((snRel - (3 * 1000)) / 10) + 1000);
        *pQuality = (uint8_t) (((tmpSQI * berSQI) + (1000000/2)) / (1000000)) & 0xFF;
    }
    else {
        *pQuality = (uint8_t) ((berSQI + 500) / 1000);
    }

    /* Clip to 100% */
    if (*pQuality > 100) {
        *pQuality = 100;
    }

    SONY_TRACE_RETURN (result);
}

sony_result_t sony_demod_dvbt_monitor_PER (sony_demod_t * pDemod, 
                                           uint32_t * pPER)
{
    uint32_t packetError = 0;
    uint32_t period = 0;
    uint8_t rdata[4];

    sony_result_t result = SONY_RESULT_OK;
    SONY_TRACE_ENTER ("sony_demod_dvbt_monitor_PER");

    if ((!pDemod) || (!pPER)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);
    }

    /* Software state check */
    if (pDemod->state != SONY_DEMOD_STATE_ACTIVE_T_C) {
        /* This api is accepted in ACTIVE state only */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    if (pDemod->system != SONY_DTV_SYSTEM_DVBT) {
        /* Not DVB-T */
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_SW_STATE);
    }

    /* Set SLV-T Bank : 0x10 */
    if (pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x00, 0x10) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    /*  slave    Bank    Addr    Bit     Name
     * ---------------------------------------------------------------------------------
     * <SLV-T>   10h     5Ch    [3:0]    OREG_PER_MES[3:0]
     * <SLV-T>   10h     5Dh      [0]    IREG_PER_VALID
     * <SLV-T>   10h     5Eh    [7:0]    IREG_PER_PKTERR[15:8]
     * <SLV-T>   10h     5Fh    [7:0]    IREG_PER_PKTERR[7:0]
     */
    if (pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddressSLVT, 0x5C, rdata, 4) != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_I2C);
    }

    if ((rdata[1] & 0x01) == 0) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);    /* Not ready... */
    }

    /* PER = IREG_PER_PKTERR / ( 2 ^ OREG_PER_MES ) */
    packetError = (rdata[2] << 8) | rdata[3];
    period = 1U << (rdata[0] & 0x0F);

    if ((period == 0) || (packetError > period)) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    /*
      PER = packetError * 1000000 / period
          = packetError * 1000 * 1000 / period
          (Divide in 2 steps to prevent overflow.)
    */
    {
        uint32_t div = 0;
        uint32_t Q = 0;
        uint32_t R = 0;

        div = period;

        Q = (packetError * 1000) / div;
        R = (packetError * 1000) % div;

        R *= 1000;
        Q = Q * 1000 + R / div;
        R = R % div;

        /* rounding */
        if ((div != 1) && (R >= div/2)) {
            *pPER = Q + 1;
        } 
        else {
            *pPER = Q;
        }
    }

    SONY_TRACE_RETURN (result);
}

/*-----------------------------------------------------------------------------
    Static Functions
-----------------------------------------------------------------------------*/
static sony_result_t IsTPSLocked (sony_demod_t * pDemod)
{
    uint8_t sync = 0;
    uint8_t tslock = 0;
    uint8_t earlyUnlock = 0;
    sony_result_t result = SONY_RESULT_OK;

    SONY_TRACE_ENTER ("IsTPSLocked");

    if (!pDemod)
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_ARG);

    result = sony_demod_dvbt_monitor_SyncStat (pDemod, &sync, &tslock, &earlyUnlock);
    if (result != SONY_RESULT_OK) {
        SONY_TRACE_RETURN (result);
    }

    if (sync != 6) {
        SONY_TRACE_RETURN (SONY_RESULT_ERROR_HW_STATE);
    }

    SONY_TRACE_RETURN (SONY_RESULT_OK);
}
