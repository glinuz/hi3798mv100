#ifndef __HIDVB_T_T2_H__
#define __HIDVB_T_T2_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

/*DVBT专用对外接口*/
HI_S32 hi3138_tp_scan(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanAttr, HI_UNF_TUNER_TER_TPINFO_S *pstTpInfo);

/*DVBT2专用对外接口*/
HI_S32 hi3138_set_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode);
HI_S32 hi3138_set_common_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId);
HI_S32 hi3138_set_common_plp_combination(HI_U32 u32TunerPort, HI_U8 u8ComPlpEna);
HI_S32 hi3138_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum);
HI_S32 hi3138_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *pstPLPType);
HI_S32 hi3138_get_plp_id(HI_U32 u32TunerPort, HI_U8 *u8PlpId);
HI_S32 hi3138_get_plp_group_id(HI_U32 u32TunerPort, HI_U8 *u8GrpPlpId);
HI_S32 hi3138_set_antena_power(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ANTENNA_POWER_E enPower);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
