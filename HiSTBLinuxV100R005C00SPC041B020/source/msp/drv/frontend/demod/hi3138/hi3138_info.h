#ifndef __HI3138_INFO_H__
#define __HI3138_INFO_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 hi3138_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
HI_S32 hi3138_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
HI_S32 hi3138_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
HI_S32 hi3138_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
HI_S32 hi3138_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
HI_S32 hi3138_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
HI_S32 hi3138_get_data_sample(HI_U32 u32TunerPort, TUNER_DATA_SRC_E enDataSrc, HI_U32 u32DataLen, HI_UNF_TUNER_SAMPLE_DATA_S *pstData);
HI_VOID hi3138_read_tuner_registers(HI_U32 u32TunerPort, void * p);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
