#ifndef __HIDVBC_H__
#define __HIDVBC_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 hiDVBC_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel);
HI_S32 hiDVBC_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
HI_S32 hiDVBC_get_ber(HI_U32 u32TunerPort, HI_U32 * pu32BERa);
HI_S32 hiDVBC_get_snr(HI_U32 u32TunerPort, HI_U32 * pu32SignalStrength);
HI_S32 hiDVBC_get_signal_strength(HI_U32 u32TunerPort, HI_U32 * pu32strength);
HI_S32 hiDVBC_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
HI_S32 hiDVBC_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);
HI_S32 hiDVBC_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
