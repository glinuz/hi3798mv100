/*****************************************************************************/
/* MONTAGE PROPRIETARY AND CONFIDENTIAL                                      */
/* Montage Technology (Shanghai) Co., Ltd                                    */
/* Copyright (c) 2015                                                        */
/* Montage Technology Group Limited and/or its affiliated companies          */
/* All Rights Reserved                                                       */
/*****************************************************************************/
/*
 * Filename:        mt_fe_tn_tc3800.h
 *
 * Description:     Interface for the M88TC3800.
 *
 *
 * Author:          Lei.Zhu
 *
 * Version:         0.01.11
 * Date:            2015-03-04

 * History:
 * Description      Version     Date            Author
 *---------------------------------------------------------------------------
 * File Create      0.00.00     2013.08.20      Lei.Zhu
 * Modify           0.01.00     2013.11.06      Youzhong.Huang
 * Modify           0.01.01     2013.12.23      Roy.Xu
 * Modify           0.01.02     2014.01.09      Roy.Xu
 * Modify           0.01.03     2014.02.28      Lei.Zhu
 * Modify           0.01.04     2014.05.08      Lei.Zhu
 * Modify           0.01.05     2014.07.15      Lei.Zhu
 * Modify           0.01.06     2014.08.07      Lei.Zhu
 * Modify           0.01.07     2014.11.06      Lei.Zhu
 * Modify           0.01.08     2014.12.11      Lei.Zhu
 * Modify           0.01.09     2014.12.17      Lei.Zhu
 * Modify           0.01.10     2015.02.04      Lei.Zhu
 *---------------------------------------------------------------------------
 ****************************************************************************/


#ifndef  _MT_FE_TC3800_H
#define  _MT_FE_TC3800_H


#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define	U8	unsigned char								/* 8bit unsigned	*/
#define	S8	signed char									/* 8bit unsigned	*/
#define	U16	unsigned short								/* 16bit unsigned	*/
#define	S16	signed short								/* 16bit unsigned	*/
#define	U32	unsigned int								/* 32bit unsigned	*/
#define	S32	signed int									/* 16bit unsigned	*/
#define	DB	double
#else
typedef	unsigned char		U8;							/* 8bit unsigned	*/
typedef	unsigned char		S8;							/* 8bit unsigned	*/
typedef	unsigned short		U16;						/* 16bit unsigned	*/
typedef	signed short		S16;						/* 16bit unsigned	*/
typedef	unsigned int		U32;						/* 32bit unsigned	*/
typedef	signed int			S32;						/* 16bit unsigned	*/
typedef	double				DB;
#endif

#define  MT_TC3800_CUSTOM_CONFIG	0

#define  TUNER_I2C_ADDR_TC3800		0xc2

typedef struct _MT_FE_TN_Device_Settings_TC3800
{
	U8			tuner_init_OK;			// Tuner initialize status
	U8			tuner_dev_addr;			// Tuner device address

	U32			tuner_freq;				// RF frequency to be set, unit: KHz

	S16			tuner_mode;				// 0: Normal (LT Off), 1: Normal (LT On), 2: Sleep (LT On), 3: Sleep (LT Off)
	U8			tuner_application;		// 0: Cable, 1: Terrestrial, 2: Analog TV, 3: T2,4:T
	U8			tuner_bandwidth;		// Bandwidth of the channel, unit: MHz, 6/7/8/10
	U32			tuner_crystal;			// Tuner crystal frequency, unit: KHz
	U32			tuner_dac;				// Tuner DAC frequency, unit: KHz
	U8			tuner_clock_out;		// 0: Clock Out Off, 1: Clock Out On

	U8			tuner_int_lt;
	U8			tuner_int_fast_set;

	U8			tuner_moni_type;
	U8			tuner_lb_sen_boost;
	U8			tuner_app_pre;

	U16			tuner_qam;
	U16			tuner_mtt;
	U16			tuner_custom_cfg;
	U32			tuner_version;
	U32			tuner_time;

	U8			tuner_int_im;
	U32			tuner_int_fm;
	U8			tuner_int_nhalf;
	U8			tuner_int_sdm;
	U32			tuner_int_frv_K;
	S32			tuner_ldiv;
} MT_FE_TN_DEVICE_SETTINGS_TC3800, *MT_FE_Tuner_Handle_TC3800;


//external function
//extern S32 _mt_fe_tn_get_reg_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 *reg_data);
//extern S32 _mt_fe_tn_set_reg_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 reg_data);
//extern S32 _mt_fe_tn_set_reg_bit_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 data, U8 high_bit, U8 low_bit);
//extern void _mt_sleep(U32 ticks_ms);

//internal function
void _mt_fe_tn_set_RF_front_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_Mixer_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_PLL_freq_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_BB_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_DAC_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_preset_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_appendix_tc3800(MT_FE_Tuner_Handle_TC3800 handle);

void _mt_fe_tn_div_ratio_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 FVCO_TG, U32 FR_V, U32 FD, U8 N_HALF, U8 SDM, U32 *div_out);
void _mt_fe_tn_freq_fine_tune_core_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 Delta_sign, U8 Num_step, U32 LDIV, U32 FR_V, U8 N_HALF, U8 SDM);
void _mt_fe_tn_freq_fine_tune_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 Delta_sign, U8 Num_step);
void _mt_fe_tn_set_BB_Cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 bandwidth);
void _mt_fe_tn_set_tune_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 WaitTime);
void _mt_fe_tn_set_sysmon_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_poweron_set_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_lpf_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle);

void _mt_fe_tn_set_mch_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void _mt_fe_tn_set_mch_app_tc3800(MT_FE_Tuner_Handle_TC3800 handle);

//APIs
void mt_fe_tn_wakeup_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
void mt_fe_tn_sleep_tc3800(MT_FE_Tuner_Handle_TC3800 handle);

//void mt_fe_tn_init_tc3800(MT_FE_Tuner_Handle_TC3800 handle);
//void mt_fe_tn_set_freq_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 Freq_KHz);

void mt_fe_tn_set_BB_filter_band_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 bandwidth);
void mt_fe_tn_get_diagnose_info_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32* data1, U32* data2);
S32 mt_fe_tn_get_signal_strength_tc3800(MT_FE_Tuner_Handle_TC3800 handle);



#ifdef __cplusplus
}
#endif

#endif

