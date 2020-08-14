/* **************************************************** */
/*!
   @file	MN_Tuner.h
   @brief	Tuner Interface for MN_DMD_Driver
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#ifndef MN8847X_TUNER_H
#define MN8847X_TUNER_H

#ifdef __cplusplus
extern "C" {
#endif

/* **************************************************** */
/*  Tuner dependence functions */
/* **************************************************** */
/* these function is defined by MN_DMD_Tuner_(tuner_name).c */
extern DMD_ERROR_t MN8847X_DMD_Tuner_init(void);
extern DMD_ERROR_t MN8847X_DMD_Tuner_set_system(DMD_PARAMETER_t* param);
extern DMD_ERROR_t MN8847X_DMD_Tuner_tune(DMD_PARAMETER_t* param);
extern DMD_ERROR_t MN8847X_DMD_Tuner_term(void);
extern void MN8847X_Tuner_check_RF_input_power(int* rxPwrPtr);//20130713, troy, uniform Tuner funciton into one file
extern void MN8847X_Tuner_check_RF_pwr_skip_scan(DMD_PARAMETER_t *param);//troy, 20140327, to decrease auto-scan time by check RF power
extern void MN8847X_DMD_DVBT_lock_LNA_signal_use_self_AGC_TEMP(DMD_PARAMETER_t *param);// 20140417, added for Fulan DVB-T

#ifdef __cplusplus
}
#endif

#endif
