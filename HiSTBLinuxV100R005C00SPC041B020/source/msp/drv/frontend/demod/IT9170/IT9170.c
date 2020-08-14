#include <linux/module.h>

#include "hi_type.h"
#include "hi_unf_i2c.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"

#include "IT9173.h"
#include "tuner.h"
#include <linux/semaphore.h>
#include "hi_kernel_adapt.h"


#define MAX_TUNER (3)
DefaultDemodulator it9170[MAX_TUNER];
extern struct semaphore it9170_sem;

HI_S32 it9170_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    Dword error = Error_NO_ERROR;
    DefaultDemodulator *demod = HI_NULL;
    StreamType streamType = 0 ;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;
    demod->demodAddr = g_stTunerOps[u32TunerPort].u32DemodAddress;
    demod->userData = (Handle)&g_stTunerOps[u32TunerPort];

    HI_INIT_MUTEX(&it9170_sem);

    error = SAMBA_setConfig((Demodulator *)demod, 0);
    if (error)
    {
        HI_ERR_TUNER("Error SAMBA_setConfig!\n");
        return HI_FAILURE;
    }

    switch (g_stTunerOps[u32TunerPort].enTsType)
    {
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50:
        streamType = StreamType_DVBT_SERIAL;  // Device will output ISDB-T datagram.
        break;
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
    case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
        streamType = StreamType_DVBT_PARALLEL;  // Device will output ISDB-T datagram.
        break;
    default:
        HI_ERR_TUNER("Not supported!\n");
        return HI_FAILURE;
    }

    error = Demodulator_initialize_it9170 ((Demodulator*)demod, streamType);
    if (error)
    {
        HI_ERR_TUNER("Error Demodulator_initialize_it9170! error = 0x%x\n", error);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 it9170_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    DefaultDemodulator *demod = HI_NULL;
    Dword error = Error_NO_ERROR;
    //Dword linkFirmwareVersion;  // Used to store LINK firmware version.
    //Dword ofdmFirmwareVersion;  // Used to store OFDM firmware version.
    Dword frequency = 0;   // Frequency is 473143 KHz.
    Word bandwidth = 0;   // Bandwidth is 6000 KHz.
    //Bool locked;     // The output paramter to check if the channel is locked.

    HI_TUNER_CHECKPOINTER( pstChannel);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;

    frequency = pstChannel->u32Frequency;
    bandwidth = pstChannel->unSRBW.u32BandWidth;
    error = Demodulator_acquireChannel_it9170 ((Demodulator*)demod, bandwidth, frequency);
    if (error)
    {
        HI_ERR_TUNER("Error Code = %X\n", error);
        return HI_FAILURE;
    }
    HI_ERR_TUNER("Demodulator_acquireChannel_it9170 succeed!\n");

    return HI_SUCCESS;
}

HI_S32 it9170_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}

HI_S32 it9170_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    DefaultDemodulator *demod= HI_NULL;
    Bool locked = False;
    HI_S32 s32Ret = 0;

    HI_TUNER_CHECKPOINTER( penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;

    s32Ret = Demodulator_isLocked_it9170((Demodulator*)demod, &locked);

    if (Error_NO_ERROR == s32Ret)
    {
        if (True == locked)
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
        else
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;

        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_TUNER("Get status error!\n");
        return HI_FAILURE;
    }
}

HI_S32 it9170_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    DefaultDemodulator *demod= HI_NULL;
    HI_S32 s32Ret = 0;
    Dword postErrorCount = 0;
    Dword postBitCount = 0;
    Word aboutCount = 0;

    HI_TUNER_CHECKPOINTER( pu32ber);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;
    s32Ret = Demodulator_getPostVitBer_it9170((Demodulator*)demod, &postErrorCount, &postBitCount,
    &aboutCount);

    if (!s32Ret)
    {
        *pu32ber = 100*postErrorCount / postBitCount;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9170_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    DefaultDemodulator *demod= HI_NULL;
    HI_S32 s32Ret = 0;
    HI_U32 u32SNR = 0;

    HI_TUNER_CHECKPOINTER( pu32SNR);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;
    s32Ret = Demodulator_getSNR_it9170((Demodulator*)demod, (Byte*)&u32SNR);

    if (!s32Ret)
    {
        *pu32SNR = u32SNR;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9170_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    DefaultDemodulator *demod= HI_NULL;
    HI_S32 s32Ret=0;
    HI_U32 u32SignalStrength=0;

    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;
    s32Ret = Demodulator_getSignalStrengthDbm((Demodulator*)demod, (Long*)&u32SignalStrength);

    if (!s32Ret)
    {
        *pu32SignalStrength = u32SignalStrength;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9170_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    DefaultDemodulator *demod= HI_NULL;
    HI_S32 s32Ret=0;
    ChannelModulation CM = {0} ;

    HI_TUNER_CHECKPOINTER( pstInfo);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;
    s32Ret = Demodulator_getChannelModulation((Demodulator*)demod, &CM);

    if (!s32Ret)
    {
        pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_ISDB_T;
        pstInfo->unSignalInfo.stTer.u32Freq = CM.frequency;

        switch (CM.bandwidth)
        {
        case Bandwidth_5M:
            pstInfo->unSignalInfo.stTer.u32BandWidth = 5000;
            break;
        case Bandwidth_6M:
            pstInfo->unSignalInfo.stTer.u32BandWidth = 6000;
            break;
        case Bandwidth_7M:
            pstInfo->unSignalInfo.stTer.u32BandWidth = 7000;
            break;
        case Bandwidth_8M:
            pstInfo->unSignalInfo.stTer.u32BandWidth = 8000;
            break;
        default:
            return HI_FAILURE;
        }

        switch (CM.constellation)
        {
        case Constellation_DQPSK:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_DQPSK;
            break;
        case Constellation_QPSK:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case Constellation_16QAM:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case Constellation_64QAM:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        default:
            return HI_FAILURE;
        }

        switch (CM.priority)
        {
        case Priority_HIGH:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;

            switch (CM.highCodeRate)
            {
            case CodeRate_1_OVER_2:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                break;
            case CodeRate_2_OVER_3:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                break;
            case CodeRate_3_OVER_4:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                break;
            case CodeRate_5_OVER_6:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                break;
            case CodeRate_7_OVER_8:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                break;
            default:
                return HI_FAILURE;
            }
            break;

        case Priority_LOW:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_LP;

            switch (CM.lowCodeRate)
            {
            case CodeRate_1_OVER_2:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                break;
            case CodeRate_2_OVER_3:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                break;
            case CodeRate_3_OVER_4:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                break;
            case CodeRate_5_OVER_6:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                break;
            case CodeRate_7_OVER_8:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                break;
            default:
                return HI_FAILURE;
            }
            break;

        default:
            return HI_FAILURE;
        }

        switch (CM.transmissionMode)
        {
        case TransmissionMode_2K:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
            break;
        case TransmissionMode_4K:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
            break;
        case TransmissionMode_8K:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
            break;
        default:
            return HI_FAILURE;
        }

        pstInfo->unSignalInfo.stTer.enGuardIntv = CM.interval + 2;
        pstInfo->unSignalInfo.stTer.enHierMod = CM.hierarchy + 1;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9170_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    DefaultDemodulator *demod= HI_NULL;
    HI_S32 s32Ret = 0;
    //ChannelModulation CM;
    HI_U8 u8RBFSFCW[3] = {0};
    HI_U8 u8PREGBFSFCW[3] = {0};
    HI_U32 u32CurFreq = 0;
    HI_U32 u32BaseFreq = 0;

    HI_TUNER_CHECKPOINTER( pu32Freq);
    HI_TUNER_CHECKPOINTER( pu32Symb);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demod = it9170+u32TunerPort;

    s32Ret = Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, r_bfs_fcw_q_22_16, u8RBFSFCW);
    s32Ret |= Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, r_bfs_fcw_q_15_8, u8RBFSFCW + 1);
    s32Ret |= Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, r_bfs_fcw_q_7_0, u8RBFSFCW + 2);
    s32Ret |= Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, p_reg_bfs_fcw_22_16, u8PREGBFSFCW);
    s32Ret |= Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, p_reg_bfs_fcw_15_8, u8PREGBFSFCW + 1);
    s32Ret |= Demodulator_readRegister((Demodulator*)demod, Processor_OFDM, p_reg_bfs_fcw_7_0, u8PREGBFSFCW + 2);

    if (!s32Ret)
    {
        u32CurFreq = (((HI_U32)u8RBFSFCW[0])<<16) + (((HI_U32)u8RBFSFCW[1])<<8) + ((HI_U32)u8RBFSFCW[2]);
        u32BaseFreq = (((HI_U32)u8PREGBFSFCW[0])<<16) + (((HI_U32)u8PREGBFSFCW[1])<<8) + ((HI_U32)u8PREGBFSFCW[2]);

        *pu32Freq = (u32BaseFreq - u32CurFreq)*(demod->adcFrequency)>>23/1000;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

