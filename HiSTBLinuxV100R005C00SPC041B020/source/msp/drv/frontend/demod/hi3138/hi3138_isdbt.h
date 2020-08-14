#ifndef __HIISDBT_H__
#define __HIISDBT_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */


HI_S32 hiISDBT_monitor_layer(HI_U32 u32TunerPort, HI_UNF_TUNER_ISDBT_MONITOR_LAYER_E enMonLayer);
HI_S32 hiISDBT_get_TMCC_info(HI_U32 u32TunerPort, HI_UNF_TUNER_TMCC_INFO_S *pstTMCCInfo);
HI_S32 hiISDBT_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel);
HI_S32 hi3138_monitor_layer(HI_U32 u32TunerPort, HI_UNF_TUNER_MONITOR_LAYERS_CONFIG_S *pstMonLayersConfig);
HI_S32 hi3138_get_TMCC_info(HI_U32 u32TunerPort, HI_UNF_TUNER_TMCC_INFO_S *pstTMCCInfo);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
