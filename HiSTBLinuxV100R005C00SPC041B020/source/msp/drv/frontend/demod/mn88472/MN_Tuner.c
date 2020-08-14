
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
#include "MN_DMD_device.h"
#include "MN_I2C.h"
#include "MN88472.h"
#include "hi_drv_tuner.h"
#include "mxl603.h"
#include "MxL603_TunerApi.h"

/* **************************************************** */
/* **************************************************** */

/*! Tuner Initialize Function*/
DMD_ERROR_t DMD_API DMD_Tuner_init( void)
{
	//TODO:	Please call tuner initialize API here

	DMD_ERROR_t ret = DMD_E_OK ;
  
	/*ret = MXL603_INIT( );	

     if (ret != DMD_E_OK)
    {
      HI_INFO_TUNER("Error! DMD_Tuner_init\n");    
     }*/
 
	//this function is called by DMD_init
	
	//DMD_DBG_TRACE("Tuner is not implemeted\n");
	return ret ;
}

/*! Tuner Tune Function */
DMD_ERROR_t DMD_API DMD_Tuner_tune( DMD_PARAMETER_t *param)
{
	//TODO:	Please call tune  API here
	DMD_ERROR_t ret = DMD_E_OK;
	DMD_u32_t rf_in_khz = param->freq; 
	DMD_u8_t u8I2cnum = g_stTunerOps[param->devid].enI2cChannel;
 
	ret = mxl603_set_tuner(param->devid,u8I2cnum,rf_in_khz);

	if (ret != DMD_E_OK)
	{
		HI_INFO_TUNER(" Pls. retune - until tuner output stable .\n");
		ret = DMD_E_ERROR;
	}
	 
	//this function is called by DMD_tune
	//DMD_DBG_TRACE("Tuner is not implemeted\n");
	return ret;
}

/*! Tuner Termination Function */
DMD_ERROR_t DMD_API DMD_Tuner_term(void )
{
	//TODO:	Please call tune  API here
	//this function is called by DMD_term
	DMD_DBG_TRACE("Tuner is not implemeted\n");
	return DMD_E_OK;
}
/*! Tuner Tune Function */
DMD_ERROR_t DMD_API DMD_Tuner_set_system(DMD_PARAMETER_t *param)
{
	//TODO:	Please call tune  API here
	DMD_ERROR_t ret = DMD_E_ERROR; 
	/*ret = Mxl603SetSystemMode( );//global parameter elsewhere 


	if (ret != DMD_E_OK)
	{
		HI_INFO_TUNER("Error! DMD_Tuner_set_system \n");
	}	*/
	//this function is called by DMD_tune
	DMD_DBG_TRACE("Tuner is not implemeted\n");
	return ret;
}

void Tuner_check_RF_input_power(DMD_u8_t user_dev_id, int* rxPwrPtr)
{
		MxLWare603_API_ReqTunerRxPower(user_dev_id, (SINT16*)rxPwrPtr); 

}
