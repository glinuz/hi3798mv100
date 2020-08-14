#ifndef __HI3138_TS_H__
#define __HI3138_TS_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */


HI_S32 hi3138_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
HI_S32 hi3138_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);
HI_S32 hi3138_ts_output_swap(HI_U32 u32TunerPort,HI_BOOL bSwapFlag);
HI_S32 hi3138_single_stream_config(HI_U32 u32TunerPort);
HI_S32 hi3138_set_ts_port_hiz(HI_U32 u32TunerPort, HI_UNF_TUNER_TS_SERIAL_PIN_E enSeriPin);
HI_S32 hi3138_set_comb_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);
HI_S32 hi3138_mts_serial_config(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType, HI_UNF_TUNER_SIG_TYPE_E enSigType);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
