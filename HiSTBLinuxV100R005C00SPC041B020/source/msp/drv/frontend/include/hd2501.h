/*************************************************************
** file name:		hd2501_demod.h
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/

#ifndef __HD2501_H__
#define __HD2501_H__

#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/* include -------------------------------------------------*/
/* macro & structure ---------------------------------------*/
/* global & static variables -------------------------------*/
/* extern functions declaration ----------------------------*/

extern HI_S32 hd2501_demod_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 hd2501_demod_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
extern HI_S32 hd2501_demod_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel);
extern HI_S32 hd2501_demod_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 hd2501_demod_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 hd2501_demod_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 hd2501_demod_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 hd2501_demod_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_S32 hd2501_demod_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_S32 hd2501_demod_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 hd2501_demod_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 hd2501_demod_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
extern HI_S32 hd2501_demod_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
extern HI_S32 hd2501_demod_standby(HI_U32 u32TunerPort, HI_U32 u32Standby);
extern HI_S32 hd2501_demod_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
extern HI_S32 hd2501_demod_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);


/* static functions declaration ----------------------------*/
/* functions define ----------------------------------------*/


/* static functions definition -----------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif  //#define __HD2501_DEMOD_H__

