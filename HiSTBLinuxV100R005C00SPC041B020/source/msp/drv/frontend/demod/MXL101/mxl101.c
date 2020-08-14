#include <linux/module.h>
#include <linux/semaphore.h>

#include "hi_type.h"
#include "hi_unf_i2c.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"

#include "MxL101SF_PhyCtrlApi.h"
#include "MxL101SF_OEM_Drv.h"

#include "hi_kernel_adapt.h"

#define MAX_TUNER (3)
HI_U32 u32CurTunerPort = 0;
struct semaphore mxl101_sem;

HI_S32 mxl101_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    HI_S32 s32Ret = HI_FAILURE;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    HI_INIT_MUTEX(&mxl101_sem);

    return s32Ret;
}

HI_S32 mxl101_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    MXL_DEV_INFO_T mxlDevInfo  = {0};
    MXL_XTAL_CFG_T mxlXtalCfg = {0};
    MXL_DEV_MODE_CFG_T mxlDevMode = {0};
    MXL_MPEG_CFG_T mxlMpegOutCfg = {0};
    MXL_TS_CTRL_T mxlTsCfg = {0};
    MXL_TOP_MASTER_CFG_T mxlTopMasterCfg = {0};
    MXL_RF_TUNE_CFG_T mxlChanCfg = {0};
    MXL_DEMOD_SPECTRUM_CFG_T mxlFreqSpectrumCfg = {0};
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;
    // 1. Do SW Reset
    MxLWare_API_ConfigDevice(MXL_DEV_SOFT_RESET_CFG, NULL);

    // 2. Read Back chip id and version
    //    Expecting CHIP ID = 0x61, Version = 0x6
    MxLWare_API_GetDeviceStatus(MXL_DEV_ID_VERSION_REQ, &mxlDevInfo);
    HI_ERR_TUNER("MxL101SF : ChipId = 0x%x, Version = 0x%x\n", mxlDevInfo.DevId,
                                                         mxlDevInfo.DevVer);

    // 3. Init Tuner and Demod
    MxLWare_API_ConfigDevice(MXL_DEV_101SF_OVERWRITE_DEFAULTS_CFG, NULL);

    // Step 4
    // Enable Loop Through if needed
    // Enable Clock Out and configure Clock Out gain if needed
    // Configure MxL101SF XTAL frequency
    // Configure XTAL Bias value if needed

    // Xtal Capacitance value must be configured in accordance
    // with XTAL datasheet’s requirement.
    mxlXtalCfg.XtalFreq = XTAL_24MHz;
    mxlXtalCfg.LoopThruEnable = MXL_DISABLE;
    mxlXtalCfg.XtalBiasCurrent = XTAL_BIAS_NA;
    mxlXtalCfg.XtalCap = 0x19;
    mxlXtalCfg.XtalClkOutEnable = MXL_DISABLE;
    mxlXtalCfg.XtalClkOutGain = CLK_OUT_NA;
    MxLWare_API_ConfigDevice(MXL_DEV_XTAL_SETTINGS_CFG, &mxlXtalCfg);

    // 5. Set Baseband mode, SOC or Tuner only mode
    mxlDevMode.DeviceMode = MXL_SOC_MODE;
    MxLWare_API_ConfigDevice(MXL_DEV_OPERATIONAL_MODE_CFG, &mxlDevMode);

    // 6. Configure MPEG Out
    // CLK, MPEG_CLK_INV, Polarity of MPEG Valid/MPEG Sync
    mxlMpegOutCfg.MpegClkFreq = MPEG_CLOCK_36_571429MHz;
    mxlMpegOutCfg.MpegClkPhase = MPEG_CLK_INVERTED;//MPEG_CLK_IN_PHASE;
    mxlMpegOutCfg.MpegSyncPol = MPEG_CLK_IN_PHASE;
    mxlMpegOutCfg.MpegValidPol = MPEG_CLK_IN_PHASE;
    /*set ts to be parallel or serial mode*/
    switch (g_stTunerOps[u32TunerPort].enTsType)
    {
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
    case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
        mxlMpegOutCfg.SerialOrPar = MPEG_DATA_PARALLEL;
        mxlMpegOutCfg.MpegClkFreq = MPEG_CLOCK_6_857143MHz;
        break;
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50:
        mxlMpegOutCfg.SerialOrPar = MPEG_DATA_SERIAL;
        mxlMpegOutCfg.MpegClkFreq = MPEG_CLOCK_36_571429MHz;
        mxlMpegOutCfg.LsbOrMsbFirst = MPEG_SERIAL_MSB_1ST;
        break;
    default:
        break;
    }
    MxLWare_API_ConfigDevice(MXL_DEV_MPEG_OUT_CFG, &mxlMpegOutCfg);

    // 7. Turn ON TS
    mxlTsCfg.TsCtrl = MXL_ON;
    MxLWare_API_ConfigDevice(MXL_DEV_TS_CTRL_CFG, &mxlTsCfg);

    // 8. Enable Top Master Control
    mxlTopMasterCfg.TopMasterEnable = MXL_ENABLE;
    MxLWare_API_ConfigTuner(MXL_TUNER_TOP_MASTER_CFG, &mxlTopMasterCfg);

    // 9. Disable processing of Inverted Spectrum signals
    if (!(pstChannel -> bSI))
    {
        mxlFreqSpectrumCfg.SpectrumCfg = MXL_OFF;
    }
    else
    {
        mxlFreqSpectrumCfg.SpectrumCfg = MXL_ON;
    }

    MxLWare_API_ConfigDemod(MXL_DEMOD_SPECTRUM_CFG, &mxlFreqSpectrumCfg);

    // 10. Tune RF with channel frequency and bandwidth
    mxlChanCfg.Bandwidth = pstChannel->unSRBW.u32BandWidth/1000;
    mxlChanCfg.Frequency = pstChannel->u32Frequency*1000;
    mxlChanCfg.TpsCellIdRbCtrl = MXL_ENABLE;  // Enable TPS Cell ID feature
    MxLWare_API_ConfigTuner(MXL_TUNER_CHAN_TUNE_CFG, &mxlChanCfg);
    up(&mxl101_sem);
    // Wait 200ms
    Ctrl_Sleep(200);

    return HI_SUCCESS;
}

HI_S32 mxl101_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    MXL_DEMOD_LOCK_STATUS_T rsLockStatus = {0};
    UINT8 i = 0;
    MXL_STATUS status = MXL_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPOINTER( penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;
    for (i=0; i<4; i++)
    {
        status = MxLWare_API_GetDemodStatus(MXL_DEMOD_RS_LOCK_REQ, &rsLockStatus);
        if (status)
        {
            up(&mxl101_sem);
            return HI_FAILURE;
        }
        if (rsLockStatus.Status)
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
            break;
        }
        else
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        }
    }

    up(&mxl101_sem);
    return HI_SUCCESS;
}

HI_S32 mxl101_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    MXL_DEMOD_BER_INFO_T ber = {0};
    MXL_STATUS status = MXL_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPOINTER( pu32ber);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;

    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_BER_REQ, &ber);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
        pu32ber[0] = ber.BER;
        pu32ber[1] = 0;
        pu32ber[2] = 10;
    }

    up(&mxl101_sem);
    return HI_SUCCESS;
}

HI_S32 mxl101_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    MXL_DEMOD_SNR_INFO_T snr = {0};
    MXL_STATUS status = MXL_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPOINTER( pu32SNR);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;

    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_SNR_REQ, &snr);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
        *pu32SNR = snr.SNR;
    }

    up(&mxl101_sem);
    return HI_SUCCESS;
}

HI_S32 mxl101_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    MXL_SIGNAL_STATS_T sigStrength = {0};
    MXL_STATUS status = MXL_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;

    status = MxLWare_API_GetTunerStatus(MXL_TUNER_SIGNAL_STRENGTH_REQ, &sigStrength);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
        pu32SignalStrength[1] = sigStrength.SignalStrength + 107;
    }

    up(&mxl101_sem);
    return HI_SUCCESS;
}

HI_S32 mxl101_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}

HI_S32 mxl101_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    MXL_STATUS status = MXL_FALSE;
    MXL_DEMOD_TPS_INFO_T tpsInfo = {0};
    //MXL_DEMOD_TS_PRIORITY_CFG_T tsPriority = {0};
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPOINTER( pstInfo);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T;

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    /*get the constellation*/
    u32CurTunerPort = u32TunerPort;
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_CONSTELLATION_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 0:
        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case 1:
        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 2:
        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

    /*get the code rate*/
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_CODE_RATE_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 0:
        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
            break;
        case 1:
        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
            break;
        case 2:
        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
            break;
        case 3:
        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
            break;
        case 4:
        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

    /*get the fft mode*/
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_FFT_MODE_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 0:
        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
            break;
        case 1:
        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
            break;
        case 2:
        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

    /*get guard interval*/
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_GUARD_INTERVAL_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 0:
        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 1:
        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
            break;
        case 2:
        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
            break;
        case 3:
        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

    /*get hierarchical mod type and alpha*/
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_HIERARCHY_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 0:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_NO;
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_NONE;
            up(&mxl101_sem);
            return HI_SUCCESS;
        case 1:
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

    /*get the value of alpha*/
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TPS_HIERARCHICAL_ALPHA_REQ, &tpsInfo);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
       switch (tpsInfo.TpsInfo)
       {
        case 1:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA1;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA2;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA4;
            break;
        default:
            up(&mxl101_sem);
            return HI_FAILURE;
       }
    }

#if 0
    /*get ts priotity*/
    //tsPriority.I2cSlaveAddr = g_stTunerOps[u32TunerPort].u32DemodAddress;
    //tsPriority.PrivateDataPtr = (HI_VOID *)u32TunerPort;
    status = MxLWare_API_GetDemodStatus(MXL_DEMOD_TS_PRIORITY_REQ, &tsPriority);

    if (status)
    {
        return HI_FAILURE;
    }
    else
    {
       switch (tsPriority.StreamPriority)
       {
        case HP_STREAM:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;
            break;
        case LP_STREAM:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_LP;
            break;
        default:
            return HI_FAILURE;
       }
    }
#endif

    up(&mxl101_sem);
    return HI_SUCCESS;
}

HI_S32 mxl101_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    MXL_TUNER_CHAN_OFFSET_T chanOffset = {0};
    MXL_STATUS status = MXL_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    /*guarantee the pointer is not NULL*/
    HI_TUNER_CHECKPOINTER( pu32Freq);
    HI_TUNER_CHECKPOINTER( pu32Symb);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&mxl101_sem);
    if (s32Ret)
    {
        return HI_FAILURE;
    }
    u32CurTunerPort = u32TunerPort;
    status = MxLWare_API_GetTunerStatus(MXL_TUNER_CHAN_OFFSET_REQ, &chanOffset);

    if (status)
    {
        up(&mxl101_sem);
        return HI_FAILURE;
    }
    else
    {
        *pu32Freq = chanOffset.ChanOffset/1000;
    }

    up(&mxl101_sem);
    return HI_SUCCESS;
}

