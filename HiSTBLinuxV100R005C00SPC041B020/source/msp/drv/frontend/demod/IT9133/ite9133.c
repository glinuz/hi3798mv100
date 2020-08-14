#include "IT9133.h"
#include "Omega.h"
#include "hi_type.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "tuner.h"
#include <linux/semaphore.h>
#include "hi_kernel_adapt.h"

DefaultDemodulator demod[TUNER_NUM] =
{
    {
        NULL,   //Handle userData;
        NULL,   //Byte* firmwareCodes;
        NULL,   //Segment* firmwareSegments;
        NULL,   //Byte* firmwarePartitions;
        NULL,   //Word* scriptSets;
        NULL,   //ValueSet* scripts;
        12000,      //Dword crystalFrequency;
        20250000,       //Dword adcFrequency;
        StreamType_DVBT_PARALLEL,   //StreamType streamType;
        8000,       //Word bandwidth;
        722000,     // Dword frequency;
        0,      //Dword fcw;
        {False,False,0,0,},     //Statistic statistic;
        0,      //Byte hostInterface;
        False,  //Bool booted;
        False,  //Bool initialized;
        0,      //Byte pidCounter;
        0,      //Byte demodAddr;       /** Demodulator I2C Address */
        False   //Bool Clkout_en;       /** Clock output enable */
    },
    {
        NULL,   //Handle userData;
        NULL,   //Byte* firmwareCodes;
        NULL,   //Segment* firmwareSegments;
        NULL,   //Byte* firmwarePartitions;
        NULL,   //Word* scriptSets;
        NULL,   //ValueSet* scripts;
        12000,      //Dword crystalFrequency;
        20250000,       //Dword adcFrequency;
        StreamType_DVBT_PARALLEL,   //StreamType streamType;
        8000,       //Word bandwidth;
        722000,     // Dword frequency;
        0,      //Dword fcw;
        {False,False,0,0,},     //Statistic statistic;
        0,      //Byte hostInterface;
        False,  //Bool booted;
        False,  //Bool initialized;
        0,      //Byte pidCounter;
        0,      //Byte demodAddr;       /** Demodulator I2C Address */
        False   //Bool Clkout_en;       /** Clock output enable */
    },
    {
        NULL,   //Handle userData;
        NULL,   //Byte* firmwareCodes;
        NULL,   //Segment* firmwareSegments;
        NULL,   //Byte* firmwarePartitions;
        NULL,   //Word* scriptSets;
        NULL,   //ValueSet* scripts;
        12000,      //Dword crystalFrequency;
        20250000,       //Dword adcFrequency;
        StreamType_DVBT_PARALLEL,   //StreamType streamType;
        8000,       //Word bandwidth;
        722000,     // Dword frequency;
        0,      //Dword fcw;
        {False,False,0,0,},     //Statistic statistic;
        0,      //Byte hostInterface;
        False,  //Bool booted;
        False,  //Bool initialized;
        0,      //Byte pidCounter;
        0,      //Byte demodAddr;       /** Demodulator I2C Address */
        False   //Bool Clkout_en;       /** Clock output enable */
    },
    {
        NULL,   //Handle userData;
        NULL,   //Byte* firmwareCodes;
        NULL,   //Segment* firmwareSegments;
        NULL,   //Byte* firmwarePartitions;
        NULL,   //Word* scriptSets;
        NULL,   //ValueSet* scripts;
        12000,      //Dword crystalFrequency;
        20250000,       //Dword adcFrequency;
        StreamType_DVBT_PARALLEL,   //StreamType streamType;
        8000,       //Word bandwidth;
        722000,     // Dword frequency;
        0,      //Dword fcw;
        {False,False,0,0,},     //Statistic statistic;
        0,      //Byte hostInterface;
        False,  //Bool booted;
        False,  //Bool initialized;
        0,      //Byte pidCounter;
        0,      //Byte demodAddr;       /** Demodulator I2C Address */
        False   //Bool Clkout_en;       /** Clock output enable */
    },
    {
        NULL,   //Handle userData;
        NULL,   //Byte* firmwareCodes;
        NULL,   //Segment* firmwareSegments;
        NULL,   //Byte* firmwarePartitions;
        NULL,   //Word* scriptSets;
        NULL,   //ValueSet* scripts;
        12000,      //Dword crystalFrequency;
        20250000,       //Dword adcFrequency;
        StreamType_DVBT_PARALLEL,   //StreamType streamType;
        8000,       //Word bandwidth;
        722000,     // Dword frequency;
        0,      //Dword fcw;
        {False,False,0,0,},     //Statistic statistic;
        0,      //Byte hostInterface;
        False,  //Bool booted;
        False,  //Bool initialized;
        0,      //Byte pidCounter;
        0,      //Byte demodAddr;       /** Demodulator I2C Address */
        False   //Bool Clkout_en;       /** Clock output enable */
    }
};
HI_U32 g_ulIt9133InitFlag = HI_FALSE;

extern struct semaphore it9133_sem;


HI_S32 it9133_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    HI_S32   Ret;
    Demodulator* demodulator = HI_NULL;

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    demodulator = (Demodulator*)(demod + u32TunerPort);

    demod->demodAddr = g_stTunerOps[u32TunerPort].u32DemodAddress;
    demod->userData = (Handle)&g_stTunerOps[u32TunerPort];

    HI_INIT_MUTEX(&it9133_sem);

    Ret = OMEGA_supportLNA((Demodulator*)demodulator, 0x02);
    if(Error_NO_ERROR != Ret)
    {
    HI_ERR_TUNER("OMEGA_supportLNA return %d\t\n", Ret);
    return Ret;
    }

    Ret = Demodulator_initialize_it9133 (demodulator, demod[u32TunerPort].streamType);
    if (Error_NO_ERROR != Ret)
    {
        HI_ERR_TUNER("call Demodulator_initialize_it9133 failed return %x.\n", Ret);
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 it9133_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_S32   Ret;
    /*Bool           locked;
    Byte           quality;*/
    HI_U32 ulFreq = 0;
    HI_U32 ulBw = 0;
    Demodulator* demodulator = (Demodulator*)(demod + u32TunerPort);
    demod->demodAddr = g_stTunerOps[u32TunerPort].u32DemodAddress;
    demod->userData = (Handle)&g_stTunerOps[u32TunerPort];

    ulFreq = pstChannel->u32Frequency;
    ulBw = pstChannel->unSRBW.u32BandWidth;


    Ret = Demodulator_acquireChannel_it9133 (demodulator, ulBw, ulFreq);
    if (Error_NO_ERROR != Ret)
    {
        HI_ERR_TUNER("call Demodulator_acquireChannel_it9133 failed return %x.\n", Ret);
        return Ret;
    }

    HI_INFO_TUNER("call Demodulator_acquireChannel_it9133 OK!\n");
    return HI_SUCCESS;
}

HI_S32 it9133_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    Demodulator* demodulator = (Demodulator*)(demod + u32TunerPort);
    Dword            postErrorCount; /** 24 bits */
    Dword            postBitCount;    /** 16 bits */
    Word             abortCount;

    if(Error_NO_ERROR != Demodulator_getPostVitBer_it9133(demodulator, &postErrorCount, &postBitCount, &abortCount))
    {
        HI_ERR_TUNER("call Demodulator_getPostVitBer_it9133 failed\r\n");
        return HI_FAILURE;
    }
    pu32ber[0] = postErrorCount*1000000000/postBitCount;
    pu32ber[1] = 0;
    pu32ber[2] = 9;

    return HI_SUCCESS;
}

HI_S32 it9133_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    Demodulator* demodulator = (Demodulator*)(demod + u32TunerPort);
    Byte ucTmp = 0;
    if (Error_NO_ERROR != Demodulator_getSNR_it9133(demodulator, &ucTmp))
    {
         HI_ERR_TUNER("call Demodulator_getSNR_it9133 failed\r\n");
         return HI_FAILURE;
    }
    *pu32SNR = ucTmp;
    return HI_SUCCESS;
}


HI_S32 it9133_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    Demodulator* demodulator = (Demodulator*)( demod + u32TunerPort);
    Bool bIsLocked = False;
    *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    if (Error_NO_ERROR != Demodulator_isLocked_it9133(demodulator, &bIsLocked))
    {
        HI_ERR_TUNER("call it9133_GetStatus failed\r\n");
        return HI_FAILURE;
    }
    if(True == bIsLocked)
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    return HI_SUCCESS;
}



HI_S32 it9133_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
     g_stTunerOps[u32TunerPort].enTsType = enTsType;
     HI_INFO_TUNER("The stream type of UNF Layer is %d.\n",enTsType);
     switch(enTsType)
     {
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
            demod[u32TunerPort].streamType = StreamType_DVBT_PARALLEL;
            break;
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
            demod[u32TunerPort].streamType = StreamType_DVBT_SERIAL;
            break;
        default:
            return HI_FAILURE;
     }
     return HI_SUCCESS;

}

HI_S32 it9133_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    Demodulator* demodulator = (Demodulator*)( demod + u32TunerPort);
    HI_S32 s32Ret = 0;
    HI_S32 rfpullUpVolt_X10 = 33;
    //HI_S32 ifpullUpVolt_X10 = 33;
    HI_U32 u32SignalStrength;

    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = Demodulator_getSignalStrengthDbm(demodulator, rfpullUpVolt_X10, rfpullUpVolt_X10, (Long*)&u32SignalStrength);

    if (!s32Ret)
    {
        *pu32SignalStrength = u32SignalStrength;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9133_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    Demodulator* demodulator = (Demodulator*)( demod + u32TunerPort);
    HI_S32 s32Ret;
    ChannelModulation CM;

    HI_TUNER_CHECKPOINTER( pstInfo);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = Demodulator_getChannelModulation(demodulator, &CM);

    if (!s32Ret)
    {
        pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T;
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

        if (Hierarchy_NONE == CM.hierarchy)
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_NONE;

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 it9133_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    Demodulator* demodulator = (Demodulator*)( demod + u32TunerPort);
    HI_S32 s32Ret;
    //ChannelModulation CM;
    HI_U8 u8RBFSFCW[3] = {0};
    HI_U8 u8PREGBFSFCW[3] = {0};
    HI_U32 u32CurFreq = 0;
    HI_U32 u32BaseFreq = 0;

    HI_TUNER_CHECKPOINTER( pu32Freq);
    HI_TUNER_CHECKPOINTER( pu32Symb);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = Demodulator_readRegister(demodulator, Processor_OFDM, r_bfs_fcw_q_22_16, u8RBFSFCW);
    s32Ret |= Demodulator_readRegister(demodulator, Processor_OFDM, r_bfs_fcw_q_15_8, u8RBFSFCW + 1);
    s32Ret |= Demodulator_readRegister(demodulator, Processor_OFDM, r_bfs_fcw_q_7_0, u8RBFSFCW + 2);
    s32Ret |= Demodulator_readRegister(demodulator, Processor_OFDM, p_reg_bfs_fcw_22_16, u8PREGBFSFCW);
    s32Ret |= Demodulator_readRegister(demodulator, Processor_OFDM, p_reg_bfs_fcw_15_8, u8PREGBFSFCW + 1);
    s32Ret |= Demodulator_readRegister(demodulator, Processor_OFDM, p_reg_bfs_fcw_7_0, u8PREGBFSFCW + 2);

    if (!s32Ret)
    {
        u32CurFreq = (((HI_U32)u8RBFSFCW[0])<<16) + (((HI_U32)u8RBFSFCW[1])<<8) + ((HI_U32)u8RBFSFCW[2]);
        u32BaseFreq = (((HI_U32)u8PREGBFSFCW[0])<<16) + (((HI_U32)u8PREGBFSFCW[1])<<8) + ((HI_U32)u8PREGBFSFCW[2]);

        *pu32Freq = (u32BaseFreq - u32CurFreq)*(((DefaultDemodulator *)demodulator)->adcFrequency)>>23/1000;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

