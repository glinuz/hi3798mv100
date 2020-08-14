#ifndef __HIDVB_S_S2_H__
#define __HIDVB_S_S2_H__

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 hi3138_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
HI_S32 hi3138_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
HI_S32 hi3138_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power);
HI_S32 hi3138_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
HI_S32 hi3138_send_continuous_22K(HI_U32 u32TunerPort,HI_BOOL b22k_on);
HI_S32 hi3138_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
HI_S32 hi3138_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone);
HI_S32 hi3138_tp_verify(HI_U32 u32TunerPort, TUNER_TP_VERIFY_PARAMS_S * pstChannel);
HI_S32 hi3138_set_isi_id(HI_U32 u32TunerPort, HI_U8 u8IsiId);
HI_S32 hi3138_get_total_isi_number(HI_U32 u32TunerPort,HI_U32 *u32TotalStream);
HI_S32 hi3138_get_isi_id(HI_U32 u32TunerPort,HI_U8 u8Stream, HI_U8 *u8IsiId);
HI_S32 hi3138_lnb_link(HI_U32 u32TunerPort);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
