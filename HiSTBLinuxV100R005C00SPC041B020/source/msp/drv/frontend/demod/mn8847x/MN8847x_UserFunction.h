/* **************************************************** */
/*!
   @file	MN88472_UserFunction.c
   @brief	Functions for MN88472 customer
   @author	R.Mori
   @date	2011/10/27
   @param
		(c)	Panasonic
   */
/* **************************************************** */


#ifndef MN8847X_USERFUNCTION_H
#define MN8847X_USERFUNCTION_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "MN_DMD_driver.h"
#include <linux/kernel.h>

//--------------------------------------
//	Get PLP information
//--------------------------------------
DMD_ERROR_t DMD_API MN8847X_DMD_get_dataPLPs(  DMD_u8_t * pPLPIds, DMD_u8_t * pNumPLPs , DMD_PARAMETER_t* param  );
DMD_ERROR_t DMD_API MN8847X_DMD_get_single_PLP_type(DMD_PARAMETER_t* param, DMD_u8_t plp_layer_number); //20131211, to optimize scan time
	
/* DVB-T, Select HP or LP mode */
DMD_ERROR_t DMD_API MN8847X_DMD_SET_DVBT_HP_LP_MODE( DMD_PARAMETER_t* param, DMD_u32_t val);



/* debugging after signal locked  */
extern void MN8847X_DMD_debug_after_signal_locked( DMD_PARAMETER_t* param);


#ifdef __cplusplus
}
#endif

#endif
