
/* **************************************************** */
/*!
   @file	MN_DMD_Tuner_template.c
   @brief	Tuner API wrapper (sample)
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */
//---------- this is sample Tuner API  ----------------
#include "MN_DMD_driver.h"
#include "MN_DMD_common.h"
#include "MN_I2C.h"

/* **************************************************** */
/* **************************************************** */

/*! Tuner Initialize Function*/
DMD_ERROR_t DMD_API MN8847X_DMD_Tuner_init( void )
{
	//TODO:	Please call tuner initialize API here

	return DMD_E_OK ;
}

/*! Tuner Tune Function */
DMD_ERROR_t DMD_API MN8847X_DMD_Tuner_tune( DMD_PARAMETER_t *param)
{
	//TODO:	Please call tune  API here
	 
	//this function is called by DMD_tune
	//DMD_DBG_TRACE("Tuner is not implemeted\n");
	return DMD_E_OK;
}

/*! Tuner Termination Function */
DMD_ERROR_t DMD_API MN8847X_DMD_Tuner_term( void )
{
	//TODO:	Please call tune  API here
	//this function is called by DMD_term
	DMD_DBG_TRACE("Tuner is not implemeted\n");
	return DMD_E_OK;
}

/*! Tuner Tune Function */
DMD_ERROR_t DMD_API MN8847X_DMD_Tuner_set_system(DMD_PARAMETER_t *param)
{
	//TODO:	Please call tune  API here
		
	//this function is called by DMD_tune
	//DMD_DBG_TRACE("Tuner is not implemeted\n");
	return DMD_E_OK ;
}

void MN8847X_Tuner_check_RF_input_power(int* rxPwrPtr)
{
    return;
}

/*
Description :  check if current channel's RF power lower than sensitivity line or not
               DVB-T2/T, nordig standard, QPSK 1/2, sensitivity is -99dBm.
Purpose     :  added for decreasing Auto scan time. 
Input   : param->system
return  : param->info[DMD_E_INFO_RF_POWER_LOWER_THAN_SENSITIVITY_LINE]
Place   : Put after DMD software reset 
Date    : troy, 20140327
*/
void MN8847X_Tuner_check_RF_pwr_skip_scan(DMD_PARAMETER_t *param)
{
	return;
}

/*
issue       : Fulan internal testissue, test DVB-T signal, 20% probability cannot lock signal
              DVBT modulation parameters : GI 1/8 or GI 1/16, others random.
Cause       : Not sure yet. It concerns with LNA adding in front of tuner.
Remedy      : Set Tuner self-AGC for DVB-T     
Side effect : No
Suggest     : 
              1. Compare with other company, which used LNA and MN8847x but no such problem.
              2. Ask Japan side for suggestion 
Creator     : Troy.wang, 20140417
*/
void MN8847X_DMD_DVBT_lock_LNA_signal_use_self_AGC_TEMP(DMD_PARAMETER_t *param)
{
    return;
}


