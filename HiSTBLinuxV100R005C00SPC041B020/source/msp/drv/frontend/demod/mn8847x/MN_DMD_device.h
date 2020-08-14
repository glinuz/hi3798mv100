/* **************************************************** */
/*!
   @file	MN_DMD_Device.h
   @brief	Panasonic Demodulator Driver
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */
/*!
   this file defines common interface for each demodulator device
   */

#include "MN_DMD_driver.h"

#ifndef MN8847X_DMD_DEVICE_H
#define MN8847X_DMD_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

//return DMD_E_OK , if device support the system & bandwidth
extern DMD_ERROR_t	MN8847X_DMD_system_support( DMD_SYSTEM_t sys );

/* **************************************************** */
/*  Demodulator dependence functions (not exported)*/
/* **************************************************** */
//these functions is defined by each device (device_name.c)
extern DMD_ERROR_t	MN8847X_DMD_device_open(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_term(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_close(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_init(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_load_pseq(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_pre_tune(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_post_tune(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_set_system(  DMD_PARAMETER_t *param ); 
extern DMD_ERROR_t	MN8847X_DMD_device_reset(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_scan(  DMD_PARAMETER_t *param );
extern DMD_ERROR_t	MN8847X_DMD_device_get_info(  DMD_PARAMETER_t *param , DMD_u32_t id);
extern DMD_ERROR_t	MN8847X_DMD_device_set_info(  DMD_PARAMETER_t *param , DMD_u32_t id ,DMD_u32_t val);
extern DMD_s32_t MN8847X_DMD_Carrier_Frequency_bias(  DMD_PARAMETER_t* param);//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
extern DMD_s32_t MN8847X_DMD_XTAL_Frequency_bias(  DMD_PARAMETER_t* param);//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.

/* '11/08/29 : OKAMOTO	Select TS output. */
extern DMD_ERROR_t MN8847X_DMD_set_ts_output( DMD_TSOUT ts_out);
extern DMD_ERROR_t MN8847X_DMD_set_t2_work_profile(DMD_PARAMETER_t* param); //20131219, set work mode for MN88473
extern DMD_ERROR_t MN8847X_DMD_set_error_flag_output( DMD_u8_t bErrorFlagOutputEnable );
extern DMD_ERROR_t MN8847X_DMD_set_power_mode(  DMD_PWR_MODE_t running_mode );/* 2012130123 : Troy, power mode selection */
extern DMD_ERROR_t MN8847X_DMD_set_dmd_control_tuner_i2c_mode(void);//20131219, for MN88473
extern DMD_ERROR_t MN8847X_DMD_device_dvbt_64qam_2k_long_delay_echoes_improve(  DMD_PARAMETER_t *param );//20130220, troy, to improve DVB-T long echo delay performance of "Si2158+MN88472"
extern DMD_ERROR_t MN8847X_DMD_device_dvbt2_read_P1_flag(  DMD_u8_t* p1flg_rd ); //troy, 20130131, to judge signal including FEF or not 
extern DMD_ERROR_t MN8847X_DMD_device_dvbc_multipath_tolerance_improve( void  );//20130220, troy, to improve DVB-C multipath tolerance performance of "Si2158+MN88472"
extern DMD_ERROR_t MN8847X_DMD_device_dvbt2_ISSY_validity_check(  DMD_PARAMETER_t *param );//20130306, Isshi san, added for checking ISSY field 
extern DMD_ERROR_t MN8847X_DMD_device_dvbt2_ignore_ISSY(void); //20130305, Simon san, added for ignoring ISSY, to check if ISSY field bring in abnormal action or not .
extern DMD_ERROR_t MN8847X_DMD_DVBC_GET_SYMBOL_RATE(  DMD_PARAMETER_t *param,DMD_u32_t *dvbc_get_symbolrate_Khz ); //20130418, troy added, for DVB-C symbol rate get. 
extern DMD_ERROR_t MN8847X_DMD_device_pseq_status( DMD_u8_t* pseq_status ); //20130412,troy.wang added, to check PSEQ running status. 
extern void MN8847X_DMD_signal_information_printout_all( DMD_PARAMETER_t *param);
extern void MN8847X_DMD_increase_compatibility_with_nonstandard_signal(DMD_PARAMETER_t *param__UNUSED);//20130626, troy.wang added, for non-standard signal with high ratio dummy cell inside.
extern DMD_ERROR_t MN8847X_DMD_device_read_strong_echo(DMD_PARAMETER_t* param,DMD_u8_t* Multipath_inter_level);//20130628, troy.wang added, for 0dB echo test, cnr read out very low which cause SQI =0, but video play well
extern DMD_ERROR_t MN8847X_DMD_set_adc_input_range(void);//20140303, troy.wang added, Set Input IF range from Tuner, for cooperation with Tuner & Demod. 

extern DMD_ERROR_t MN8847X_DMD_T2_FRAME_PROFILE_TYPES_STATS( DMD_PARAMETER_t* param);//20140410,Record T2 Frame profile type in T2 superframe.
#ifdef DMD_88473_AUTO_SET_T2LITE_IF_T2BASE_UNLOCKED
extern DMD_ERROR_t MN8847X_DMD_T2_AUTO_SET_T2LITE_MODE_AFTER_SCAN( DMD_PARAMETER_t* param);//20140410, troy.wangyx added, to set T2-lite work profile automatically
#endif

extern DMD_ERROR_t MN8847X_DMD_Constellation_monitor_pre_setting( DMD_PARAMETER_t* param, DMD_u8_t action, DMD_DVBT2_CON_MON_t dvbt2_mon_type, DMD_DVBT_CON_MON_t dvbt_mon_type);//20131015
extern DMD_ERROR_t MN8847X_DMD_Constellation_monitoring( DMD_PARAMETER_t* param, DMD_u32_t CarrierNo,DMD_s32_t* p_IMON_RD, DMD_s32_t* p_QMON_RD_p);//20131015

extern void MN8847X_DMD_DVBT_lock_high_noise_signal(void);//20131211, for GIEC factory signal(also related with hardware design)
extern DMD_ERROR_t MN8847X_DMD_device_read_chipid( DMD_u8_t*	data );
extern DMD_u32_t	MN8847X_DMD_BER( DMD_SYSTEM_t sys , DMD_u32_t* err , DMD_u32_t* sum );
DMD_u32_t	MN8847X_DMD_CNR( DMD_SYSTEM_t sys , DMD_u32_t* cnr_i , DMD_u32_t* cnr_d );
DMD_u32_t	MN8847X_DMD_AGC(void);

extern DMD_u32_t	MN8847X_DMD_RegSet_Rev;

#ifdef __cplusplus
}
#endif

#endif
