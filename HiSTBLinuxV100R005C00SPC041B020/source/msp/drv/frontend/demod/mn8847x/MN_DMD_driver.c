/* **************************************************** */
/*!
   @file	MN_DMD_driver.c
   @brief	Panasonic Demodulator Driver
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */


#include "MN_DMD_common.h"
#include "MN_DMD_device.h"
#include "MN_DMD_driver.h"
#include "MN_I2C.h"
#include "MN_Tuner.h"


/* **************************************************** */
/* Tuner parts */
/* **************************************************** */


/* **************************************************** */
/* Local Functions */
/* **************************************************** */

/* **************************************************** */
/* Definition of Constant Strings */
/* **************************************************** */

/* **************************************************** */
/* Public Functions  */
/* **************************************************** */
/*! Demodulator set device ID */
DMD_ERROR_t DMD_API MN8847X_DMD_set_devid(  DMD_PARAMETER_t* param , DMD_u32_t id )
{
	param->devid = id;

	return DMD_E_OK;
}


/*!	Demodulator API Initialize & Open */
/*!
	*initialize parameter variable (DMD_PARAMETER_t* param)
	*open & connect to USB Device for I2C communication
	 (for WINDOWS API only)
*/
DMD_ERROR_t DMD_API MN8847X_DMD_open( DMD_PARAMETER_t* param)
{
	int i;
    //printk( "\n GLOBAL PARAMETERS INITIALIZATION in %s! \n",__FUNCTION__);	

	/* Initialize Parameter Variable */
	for(i=0;i<DMD_INFORMATION_MAX;i++){
		param->info[i] = 0;		//NOT_DEFINED
	}

	/* User set Channel scan paramters */
	param->devid = 0;
	param->system= DMD_E_DVBT2;
	param->bw		= DMD_E_BW_8MHZ;
	param->freq		= 666000000;
	param->funit	= DMD_E_HZ;
	param->ts_out   = DMD_E_TSOUT_PARALLEL_FIXED_CLOCK;	
	param->dvbt2_plp_no = 0;
	param->dvbt_hp_lp_sel = DMD_E_DVBT_HIER_SEL_LP;
	param->user_scan_action = DMD_E_SCAN_GET_CHANNEL_INFO;// 20140414 : troy, separate user scan action  1.Channel search or 2.Set service
	param->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_BASE; //default setting
#if (DEMOD_TYPE == DMD_TYPE_MN88473)	
	param->t2_supported_profile_type = DMD_E_DVBT2_SOC_SUPPORT_BASE_AND_LITE;//20140414, added for SoC company to set limitation, for MN88473
#endif

	if( MN8847X_DMD_I2C_open( ) != DMD_E_OK  ) //firstly open i2c 
	{
		DMD_DBG_TRACE( "ERROR: I2C open" );
		return DMD_E_ERROR;
	}

	if( MN8847X_DMD_device_open( param ) != DMD_E_OK  )
	{
		DMD_DBG_TRACE( "ERROR: DMD open" );
		return DMD_E_ERROR;

	}	
	return DMD_E_OK;
}

/*! Demodulator LSI Initialize */
/*!
	*release power down
	*Tuner Initialize
	*transfer common register settings via I2C
	*transfer auto countrol sequence via I2C
*/
DMD_ERROR_t	DMD_API	MN8847X_DMD_init(  DMD_PARAMETER_t* param )
{
    /*IF use DMD to repeat I2C cmd, firstly initializing DMD is better. Troy.wangyx, 120801*/
	/* Demodulator LSI Initialize */
	if( MN8847X_DMD_device_init( param ) != DMD_E_OK  )
	{
		DMD_DBG_TRACE( "ERROR: DMD Initialize" );
		return DMD_E_ERROR;
	}
	
	/* Auto Control Sequence Transfer */
	if( MN8847X_DMD_device_load_pseq( param ) != DMD_E_OK  )
	{
		DMD_DBG_TRACE( "ERROR: DMD Load Autocontrol" );
		return DMD_E_ERROR;
	}

	/* '11/08/29 : OKAMOTO	Select TS output. */ //only need set once, based on the hardware condition
	if( MN8847X_DMD_set_ts_output(  param->ts_out ) != DMD_E_OK )
	{
		DMD_DBG_TRACE("ERROR:set ts output");
		return DMD_E_ERROR;
	}
	
#if (DEMOD_TYPE == DMD_TYPE_MN88473)
    //Need firstly set REPEATOR(default)/BYPASS mode, then to control Tuner!
	if(MN8847X_DMD_set_dmd_control_tuner_i2c_mode() != DMD_E_OK )
	{
		DMD_DBG_TRACE("ERROR:set I2C control");
		return DMD_E_ERROR;
	}
#endif

    /* 20140303 : troy.wangyx, ADC input range for cooperation with Tuner and Demod*/
	if( MN8847X_DMD_set_adc_input_range() != DMD_E_OK )
	{
		DMD_DBG_TRACE("ERROR:set adc input range! ");
		return DMD_E_ERROR;
	}

	/* Tuner_init */
	if( MN8847X_DMD_Tuner_init( ) != DMD_E_OK )
	{
		DMD_DBG_TRACE( "ERROR: Tuner Initialize" );
		return DMD_E_ERROR;
	}
	
	return DMD_E_OK;
}

/*! Select Broadcast Sytem */
/*!
	*Transfer register settings for each system
*/
DMD_ERROR_t DMD_API MN8847X_DMD_set_system(  DMD_PARAMETER_t* param ){
     
	if( MN8847X_DMD_Tuner_set_system( param ) != DMD_E_OK  )
	{
		DMD_DBG_TRACE("ERROR:Tuner set system");
		return DMD_E_ERROR;
	}

	if( MN8847X_DMD_device_set_system(  param ) != DMD_E_OK  )
	{
		DMD_DBG_TRACE("ERROR:device set system");
		return DMD_E_ERROR;
	}

#if (DEMOD_TYPE == DMD_TYPE_MN88473)
   if (param->system == DMD_E_DVBT2)
   {
       if( MN8847X_DMD_set_t2_work_profile( param ) != DMD_E_OK )
    	{
    		DMD_DBG_TRACE("ERROR:set t2 work profile output");
    		return DMD_E_ERROR;
    	}
   }
#endif

   MN8847X_DMD_increase_compatibility_with_nonstandard_signal(param); //20131211, for GIEC factory signal
	
	return DMD_E_OK;
}

/*! Tune : Better use DMD_scan() to give non-interrupt time for Demod to lock signal*/
DMD_ERROR_t DMD_API MN8847X_DMD_tune(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t ret;
	/* Call pre tune process */
	ret = MN8847X_DMD_device_pre_tune(  param );
	/* Tuner */
	ret |= MN8847X_DMD_Tuner_tune( param );
	/* Call post tune process */
	ret |= MN8847X_DMD_device_post_tune(  param );
	return ret;
}


/*! scan */
DMD_ERROR_t DMD_API MN8847X_DMD_scan(  DMD_PARAMETER_t* param )
{		
	DMD_ERROR_t	ret;
	DMD_u8_t pseq_status = 0;
	int rxPwrPtr = 0;

#ifdef COMMON_DEBUG_ON
HI_INFO_TUNER("DMD_Scan in ! usergetset[%d],dtv[%d],freq[%d],bw[%d],T2_plp[%d],dT2_baselite[%d],T_lphp[%d] \n",
		param->user_scan_action,\
		param->system,\
		param->freq,\
		param->bw,\
		param->dvbt2_plp_no,\
		param->t2_work_profile_mode,\
		param->dvbt_hp_lp_sel);
#endif
	
	/* Call pre tune process */
	MN8847X_DMD_device_pre_tune(  param ); //default empty 
	
	/* Tuner: RF and BW input*/
   ret = MN8847X_DMD_Tuner_tune(  param ); 
	if (ret != DMD_E_OK)
	{ 
		HI_INFO_TUNER(" Tuner lock signal NG, not go to demod !\n") ;
		HI_INFO_TUNER("[Advice] Tuner will always be locked no matter signal exist or not.\n Pls. check I2C communication with Tuner \n") ; 
		HI_INFO_TUNER("you can write a random value to reg 0x10 of Tuner, then read to see if it's the same, if not, pls. check I2C code\n") ; 
		return ret;
	}
	
	/* DMD software reset */
	MN8847X_DMD_device_post_tune(  param );

	/* Tuner check RF power */
	#ifdef DMD_SKIP_SCAN_IF_RF_POWER_LOWER_THAN_SENSITIVITY_LINE
	MN8847X_Tuner_check_RF_pwr_skip_scan(param);//troy, 20140327, to decrease auto-scan time by check RF power
	if (param->info[DMD_E_INFO_RF_POWER_LOWER_THAN_SENSITIVITY_LINE] == 1)
	{
		return DMD_E_OK;
	}	
	#endif
   
	/* Call Lock/SYNC Status Judgement */
	ret = MN8847X_DMD_device_scan(  param );

	/* 20130412, troy added, to judge if autoctrl runs or stops*/
	ret = MN8847X_DMD_device_pseq_status(&pseq_status);
	if ( pseq_status == 1)
	{
		HI_INFO_TUNER("-----------ERROR!!--Autoctrl stops !! pls. check load_pseq function !---- \n");
		HI_INFO_TUNER("[Advice]Pls. try to I2C write PSEQ one byte by one byte \n") ; 
		ret = DMD_E_ERROR;
	}

#ifdef DMD_88473_AUTO_SET_T2LITE_IF_T2BASE_UNLOCKED
if (param->user_scan_action == DMD_E_SCAN_GET_CHANNEL_INFO)//20140414, only affective when search channel.No need when set channel param
	{
		MN8847X_DMD_T2_AUTO_SET_T2LITE_MODE_AFTER_SCAN(param);
	}
#endif

#ifdef COMMON_DEBUG_ON //check why signal not locked; Or, modulation parameter when signal locked
	//step1 : RF power level
	MN8847X_Tuner_check_RF_input_power(&rxPwrPtr);
	HI_INFO_TUNER(" rf level = %d dBm \n",rxPwrPtr);
	HI_INFO_TUNER("[Advice]if rf level lower than sensibility threshold, pls. check RF line connection or ask Tuner company to check hardware\n") ; 

	//step2 : T2-base or T2-lite 
	if (param->system == DMD_E_DVBT2)
	{
		printk("DMD_scan T2 superframe = %d[0: base only; 1:lite only; 2:base+fef; 3:lite+fef; 4:base+lite];\n FEF exist ? [%d] \n",\
		param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE],\
		param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS]);
		printk("[Advice]pls. use function <DMD_set_t2_work_profile> to set suitable work mode \n") ; 
	}	
#endif

#ifdef DETAIL_DIG_DEBUG_ON
	//step3: 
	/*when signal sometimes can be locked, or locked but abnormal*/
	MN8847X_DMD_signal_information_printout_all(param);	
	MN8847X_DMD_read_registers();
#endif


	return ret;
}

/*! term */
DMD_ERROR_t DMD_API MN8847X_DMD_term(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_OK;

	ret = MN8847X_DMD_device_term( param);
	ret |= MN8847X_DMD_Tuner_term( );

	return ret;
}

/*! close */
DMD_ERROR_t DMD_API MN8847X_DMD_close(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_OK;

	ret = MN8847X_DMD_device_close( param);

	return ret;
}

/* **************************************************** */
/* Set Information */
/* **************************************************** */
DMD_ERROR_t DMD_API MN8847X_DMD_set_info(  DMD_PARAMETER_t* param , DMD_u32_t id  , DMD_u32_t val){

	return MN8847X_DMD_device_set_info(   param , id , val );
}

/* **************************************************** */
/* Get Information */
/* **************************************************** */
DMD_ERROR_t DMD_API MN8847X_DMD_get_info(  DMD_PARAMETER_t* param , DMD_u32_t id ){

	return MN8847X_DMD_device_get_info(  param , id );
}

DMD_text_t DMD_API MN8847X_DMD_info_title( DMD_SYSTEM_t sys ,DMD_u32_t id )
{

	if( id < DMD_E_INFO_COMMON_END_OF_INFORMATION )
	{
		return MN8847X_DMD_INFO_TITLE_COMMON[id];

	}
	else
		switch( sys )
		{
		case DMD_E_DVBT:
			if( id < DMD_E_INFO_DVBT_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return MN8847X_DMD_INFO_TITLE_DVBT[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_DVBT2:
			if( id < DMD_E_INFO_DVBT2_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return MN8847X_DMD_INFO_TITLE_DVBT2[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_DVBC:
			if( id < DMD_E_INFO_DVBC_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return MN8847X_DMD_INFO_TITLE_DVBC[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
#ifndef DMD_DISABLE_ISDB
		case DMD_E_ISDBT:
		case DMD_E_ISDBT_BRAZIL:
			if( id < DMD_E_INFO_ISDBT_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return MN8847X_DMD_INFO_TITLE_ISDBT[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_ISDBS:
			if( id < DMD_E_INFO_ISDBS_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return MN8847X_DMD_INFO_TITLE_ISDBS[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
#endif
		default:
			break;
		}
	return "Not Defined";
}

DMD_text_t DMD_API MN8847X_DMD_info_value( DMD_SYSTEM_t sys ,DMD_u32_t id , DMD_u32_t val )
{
	if( id < DMD_E_INFO_COMMON_END_OF_INFORMATION && id != DMD_E_INFO_STATUS )
	{
		return MN8847X_DMD_info_value_common( id ,val );

	}
	else 
	{
		switch( sys )
		{
		case DMD_E_DVBT2:
			return MN8847X_DMD_info_value_dvbt2( id , val );
			break;
		case DMD_E_DVBT:
			return MN8847X_DMD_info_value_dvbt( id , val );
			break;
#ifndef DMD_DISABLE_ISDB
		case DMD_E_ISDBT:
		case DMD_E_ISDBT_BRAZIL:
			return MN8847X_DMD_info_value_isdbt( id , val );
			break;
		case DMD_E_ISDBS:
			return MN8847X_DMD_info_value_isdbs( id , val );
#endif
		case DMD_E_ATSC:
		case DMD_E_QAMB_256QAM:
		case DMD_E_QAMB_64QAM:
		case DMD_E_QAMC_256QAM:
		case DMD_E_QAMC_64QAM:
			return MN8847X_DMD_info_value_vq( id , val );
		case DMD_E_DVBC:
			return MN8847X_DMD_info_value_dvbc( id , val );
			break;
		default:
			break;
		}
	}

	return "Not Defined";
}
DMD_text_t DMD_API MN8847X_DMD_value_text( DMD_PARAMETER_t* param , DMD_u32_t id )
{
	return MN8847X_DMD_info_value( param->system , id , param->info[id] );
}


/* **************************************************** */
/* Floating Functions */
/* **************************************************** */
// Get BER
DMD_float_t DMD_API MN8847X_DMD_get_ber(  DMD_PARAMETER_t* param )
{
	DMD_float_t ret;
#ifdef DMD_FLOATING_FUNCTION
	MN8847X_DMD_get_info(  param , DMD_E_INFO_BERRNUM );

	if( param->info[DMD_E_INFO_BITNUM] == 0 )
	{
		ret = 1.0;
	}
	else
	{

		ret = (DMD_float_t) param->info[DMD_E_INFO_BERRNUM]
			/ (DMD_float_t) param->info[DMD_E_INFO_BITNUM];
	}
#else
	ret = 1;
#endif
	return ret;

}
DMD_float_t DMD_API MN8847X_DMD_get_per(  DMD_PARAMETER_t* param )
{
	DMD_float_t ret;
#ifdef DMD_FLOATING_FUNCTION
	MN8847X_DMD_get_info(  param , DMD_E_INFO_PACKETNUM );

	if( param->info[DMD_E_INFO_PACKETNUM] == 0 )
	{
		ret = 1.0;
	}
	else
	{

		ret = (DMD_float_t) param->info[DMD_E_INFO_PERRNUM]
			/ (DMD_float_t) param->info[DMD_E_INFO_PACKETNUM];
	}
#else
	ret = 1;
#endif
	return ret;

}
//Get CNR
DMD_float_t DMD_API MN8847X_DMD_get_cnr(  DMD_PARAMETER_t* param )
{
	DMD_float_t	ret;
	MN8847X_DMD_get_info(  param , DMD_E_INFO_CNR_INT );
#ifdef DMD_FLOATING_FUNCTION

	ret = (DMD_float_t) param->info[DMD_E_INFO_CNR_INT] 
	     +  (DMD_float_t) param->info[DMD_E_INFO_CNR_DEC] / ( (DMD_float_t) 100.0 );
#else
	ret = (DMD_float_t) param->info[DMD_E_INFO_CNR_INT];
#endif


	return ret;
}


int MN8847X_Get_RF_T_REF_PWR( DMD_PARAMETER_t * param)
{
	int   T_REF_PWR;
	DMD_DVBT_CONST_t   constellation;
	DMD_DVBT_HIERARCHY_t   hierarchy_exist;
	DMD_DVBT_HIER_SEL_t hierarchy_selection;
	DMD_DVBT_CR_t   coderate;

       T_REF_PWR = -100;

	      if ( param->system != DMD_E_DVBT)
      	{
			HI_INFO_TUNER(" ERROR : Get_T_REF_PWR, system is not DVB-T @!!\n");
			return  T_REF_PWR;
      	}
		  
		// get LP constelation mode ; HP constelation only one : QPSK 
		MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT_CONSTELLATION);
		constellation = param->info[DMD_E_INFO_DVBT_CONSTELLATION];

		// Check HP stream exist or not 	 
		MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT_HIERARCHY);			
		hierarchy_exist= param->info[DMD_E_INFO_DVBT_HIERARCHY];

		MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT_HP_CODERATE);	
		MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT_LP_CODERATE);					

		// 0 : DMD_E_DVBT_HIER_SEL_LP(default),	1: DMD_E_DVBT_HIER_SEL_HP 
		// [ Attention !]Selection should be executed at topper layer: 
		// DMD_device_set_info(param , DMD_E_INFO_DVBT_HIERARCHY_SELECT , DMD_u32_t val ) // val -> 1 :  HP ; 0 : LP 
		hierarchy_selection = param->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT];
	
	if  (hierarchy_exist != DMD_E_DVBT_HIERARCHY_NO)
	{
	  	if (hierarchy_selection == DMD_E_DVBT_HIER_SEL_HP)//Hierarchy, Alpha 1,2,4 & Customer chooses to decode HP data stream
	  	{  		
			coderate = param->info[DMD_E_INFO_DVBT_HP_CODERATE];
			constellation = DMD_E_DVBT_CONST_QPSK; //HP only supports QPSK
	  	}	
	  else //Non-hierarchy OR Customer choose LP when HP&LP both transmitted. 
	  	{
			coderate = param->info[DMD_E_INFO_DVBT_LP_CODERATE];
	  	}
	}
	else //no hierarchy 
	{   
		//troy, 20130131, tested, found that if not open HIERARCHY, DVB-T code rate gets from HP area. 
		coderate = param->info[DMD_E_INFO_DVBT_HP_CODERATE];
	}

#ifdef COMMON_DEBUG_ON 
		HI_INFO_TUNER(" GET DVBT REF PWR constellation=%d coderate=%d  , hierarchy_selection = %d( 1 : HP; 0 : LP )\n",constellation,coderate,hierarchy_selection);
#endif
/*
typedef enum{	DMD_E_DVBT_CONST_QPSK=0,	DMD_E_DVBT_CONST_16QAM,	DMD_E_DVBT_CONST_64QAM	}	DMD_DVBT_CONST_t;	
typedef enum{	DMD_E_DVBT_CR_1_2=0,	DMD_E_DVBT_CR_2_3,DMD_E_DVBT_CR_3_4,	
DMD_E_DVBT_CR_5_6,	DMD_E_DVBT_CR_7_8	}	DMD_DVBT_CR_t;
*/
/*--- Based on Nordig 2.3, chapter 3.4.4.6 Requirements for the signal strength indicator (SSI) ---*/
if      ( constellation == DMD_E_DVBT_CONST_QPSK ) {                   // QPSK
	      if      ( coderate == 0 ) T_REF_PWR = -93;        //  1/2
	      else if ( coderate == 1 ) T_REF_PWR = -91;        //  2/3
	      else if ( coderate == 2 ) T_REF_PWR = -90;        //  3/4
	      else if ( coderate == 3 ) T_REF_PWR = -89;	    //  5/6
	      else if ( coderate == 4 ) T_REF_PWR = -88;        //  7/8
	      else T_REF_PWR = -100;                           //  unknown
		  }
      else if ( constellation == DMD_E_DVBT_CONST_16QAM) {                   // 16QAM
	      if      ( coderate == 0 ) T_REF_PWR = -87;       //  1/2
	      else if ( coderate == 1 ) T_REF_PWR = -85;       //  2/3
	      else if ( coderate == 2 ) T_REF_PWR = -84;       //  3/4
	      else if ( coderate == 3 ) T_REF_PWR = -83;       //  5/6
	      else if ( coderate == 4 ) T_REF_PWR = -82;       //  7/8
	      else T_REF_PWR = -100;                           //  unknown
		  }
      else if ( constellation == DMD_E_DVBT_CONST_64QAM) {                   // 64QAM
	      if      ( coderate == 0 ) T_REF_PWR = -82;       //  1/2
	      else if ( coderate == 1 ) T_REF_PWR = -80;       //  2/3
	      else if ( coderate == 2 ) T_REF_PWR = -78;       //  3/4
	      else if ( coderate == 3 ) T_REF_PWR = -77;	    //  5/6
	      else if ( coderate == 4 ) T_REF_PWR = -76;       //  7/8
	      else T_REF_PWR = -100;                           //  unknown
		  }
	  else T_REF_PWR = -100;                           //  unknown

  return T_REF_PWR;
}


int MN8847X_Get_RF_T2_REF_PWR( DMD_PARAMETER_t * param)
{
		int   T2_REF_PWR;
		DMD_DVBT2_PLP_MOD_t   constellation;
		DMD_DVBT2_CR_t   coderate;	

      T2_REF_PWR = -100;

		if ( param->system != DMD_E_DVBT2)
      	{
			HI_INFO_TUNER(" ERROR : Get_T2_REF_PWR, system is not DVB-T2 @!!\n");
			return  T2_REF_PWR;
      	}

		MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT2_DAT_PLP_MOD);
		constellation = param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD];

		MN8847X_DMD_get_info( param,DMD_E_INFO_DVBT2_DAT_PLP_COD);
		coderate = param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD];

#ifdef COMMON_DEBUG_ON 
		HI_INFO_TUNER("%s %d  constellation=%d   coderate=%d   \n",__FUNCTION__,__LINE__,constellation,coderate);
#endif
/*
typedef enum{	DMD_E_DVBT2_PLP_MOD_QPSK=0,	DMD_E_DVBT2_PLP_MOD_16QAM,	
DMD_E_DVBT2_PLP_MOD_64QAM,	DMD_E_DVBT2_PLP_MOD_256QAM	}DMD_DVBT2_PLP_MOD_t;

typedef enum{	DMD_E_DVBT2_CR_1_2=0,	DMD_E_DVBT2_CR_3_5,	DMD_E_DVBT2_CR_2_3,	
DMD_E_DVBT2_CR_3_4,	DMD_E_DVBT2_CR_4_5,	DMD_E_DVBT2_CR_5_6	}DMD_DVBT2_CR_t;
*/

/*--- Based on Nordig 2.3, chapter 3.4.4.6 Requirements for the signal strength indicator (SSI) ---*/
  if      ( constellation == DMD_E_DVBT2_PLP_MOD_QPSK ) {                       // QPSK
      if      ( coderate == 0 ) T2_REF_PWR = -96;            //  1/2
      else if ( coderate == 1 ) T2_REF_PWR = -95;            //  3/5
      else if ( coderate == 2 ) T2_REF_PWR = -94;            //  2/3
      else if ( coderate == 3 ) T2_REF_PWR = -93;            //  3/4
      else if ( coderate == 4 ) T2_REF_PWR = -92;            //  4/5
      else if ( coderate == 5 ) T2_REF_PWR = -92;            //  5/6
	  else T2_REF_PWR = -100;                                //  unknown
	  }
  else if ( constellation == 	DMD_E_DVBT2_PLP_MOD_16QAM) {                       // 16QAM
      if      ( coderate == 0 ) T2_REF_PWR = -91;            //  1/2
      else if ( coderate == 1 ) T2_REF_PWR = -89;            //  3/5
      else if ( coderate == 2 ) T2_REF_PWR = -88;           //  2/3
      else if ( coderate == 3 ) T2_REF_PWR = -87;           //  3/4
      else if ( coderate == 4 ) T2_REF_PWR = -86;           //  4/5
      else if ( coderate == 5 ) T2_REF_PWR = -86;           //  5/6
	  else T2_REF_PWR = -100;                                //  unknown
	  }
  else if ( constellation == 	DMD_E_DVBT2_PLP_MOD_64QAM) {                       // 64QAM
      if      ( coderate == 0 ) T2_REF_PWR = -86;           //  1/2
      else if ( coderate == 1 ) T2_REF_PWR = -85;           //  3/5
       else if ( coderate == 2 ) T2_REF_PWR = -83;            //  2/3
      else if ( coderate == 3 ) T2_REF_PWR = -82;           //  3/4
      else if ( coderate == 4 ) T2_REF_PWR = -81;           //  4/5
      else if ( coderate == 5 ) T2_REF_PWR = -80;           //  5/6
	  else T2_REF_PWR = -100;                                //  unknown
	  }
  else if ( constellation == 	DMD_E_DVBT2_PLP_MOD_256QAM) {                       // 256QAM
       if      ( coderate == 0 ) T2_REF_PWR = -83;           //  1/2
      else if ( coderate == 1 ) T2_REF_PWR = -80;           //  3/5
      else if ( coderate == 2 ) T2_REF_PWR = -79;          //  2/3
      else if ( coderate == 3 ) T2_REF_PWR = -77;            //  3/4
      else if ( coderate == 4 ) T2_REF_PWR = -75;           //  4/5
      else if ( coderate == 5 ) T2_REF_PWR = -75;           //  5/6
	 else T2_REF_PWR = -100;                                //  unknown
	  }
  else T2_REF_PWR = -100;                                //  unknown


  return T2_REF_PWR;
}

int MN8847X_Calc_T_SSI( DMD_PARAMETER_t * param)
{ 
	int Prec=0,  Prel=0,  Pref=0; 
	int ssi=0; 
	int rxPwrPtr; 

	MN8847X_Tuner_check_RF_input_power(&rxPwrPtr);    

	Prec=(int)rxPwrPtr/100;

	Pref=MN8847X_Get_RF_T_REF_PWR( param);

#ifdef COMMON_DEBUG_ON 
	HI_INFO_TUNER("\n Calc_T_SSI_MxLWare603_API_ReqTunerRxPower=%d\n",(int)rxPwrPtr); 
	HI_INFO_TUNER("\n PWR_Prec = %d, Pref = %d\n",Prec,Pref); 
#endif

	Prel= Prec-  Pref;

/*
SSI = 0                                    if  Prel < -15dB
SSI = (2/3) * ( Prel + 15)           if -15 dB <=  Prel < 0dB
SSI = 4 *  Prel + 10                   if 0 dB <=   Prel < 20 dB
SSI = (2/3) * ( Prel - 20) + 90    if 20 dB <=   Prel < 35 dB
SSI = 100                                if  Prel >= 35 dB
where
 Prel =  Prec -   Pref
 Prec is referred to signal level expressed in [dBm] at receiver RF signal input.
  Pref is reference signal level value expressed in [dBm] specified in Nordig Unified V2.3 - Table 3.6 for DVB-T and in Table 3.7 for DVB-T2.
*/
	if( Prel<-15)
	{
		ssi=0;
	}else if( Prel<0){
		ssi=(int)(( Prel+15)*2/3);
	}else if( Prel<20){
		ssi=(int)( Prel*4+10);
	}else if( Prel<35){
		ssi=(int)(( Prel-20)*2/3+90);
	}else
		ssi=100;

	if(ssi>100)
		ssi=100;
	else if(ssi<0)
		ssi=0;

	return ssi;
}

int MN8847X_Calc_T2_SSI( DMD_PARAMETER_t * param)
{ 
	int   Prec=0, Prel=0,  Pref=0; 
	int ssi=0; 
	static int pre_ssi=0;
	DMD_u8_t rd; 
	int rxPwrPtr; 		  

	MN8847X_Tuner_check_RF_input_power(&rxPwrPtr); 

	Prec=(int)rxPwrPtr/100;

    //troy, 20130131, for T2 signal with FEF together, sometimes SSI return 0 but video play OK. 
    MN8847X_DMD_device_dvbt2_read_P1_flag(  &rd );
	if (( Prec <= -98 )&&( rd & 0x01))
   	{
   	#ifdef COMMON_DEBUG_ON 
   		HI_INFO_TUNER("FEF detected !\n");
	#endif
		return pre_ssi;
   	} 
	
	Pref = MN8847X_Get_RF_T2_REF_PWR( param);


#ifdef COMMON_DEBUG_ON 
	HI_INFO_TUNER("\n Calc_T2_SSI_MxLWare603_API_ReqTunerRxPower=%d\n",(int)rxPwrPtr); 
	HI_INFO_TUNER("\n PWR_Prec = %d, Pref = %d\n",Prec,Pref); 
#endif

	Prel=  Prec- Pref;
	/*
	SSI = 0											if  Prel < -15dB
	SSI = (2/3) * ( Prel + 15)				if -15 dB <= Prel < 0dB
	SSI = 4 * Prel + 10						if 0 dB <=  Prel < 20 dB
	SSI = (2/3) * (Prel - 20) + 90		if 20 dB <=  Prel < 35 dB
	SSI = 100										if Prel >= 35 dB
	where
	Prel =   Prec -  Pref
	Prec is referred to signal level expressed in [dBm] at receiver RF signal input.
	Pref is reference signal level value expressed in [dBm] specified in Nordig Unified V2.3 - Table 3.6 for DVB-T and in Table 3.7 for DVB-T2.
	*/
	if( Prel<-15)
	{
		ssi=0;
	}else if( Prel<0){
		ssi=(int)(( Prel+15)*2/3);
	}else if( Prel<20){
		ssi=(int)( Prel*4+10);
	}else if( Prel<35){
		ssi=(int)(( Prel-20)*2/3+90);
	}else
		ssi=100;

	if(ssi>100)
		ssi=100;
	else if(ssi<0)
		ssi=0;
	
	pre_ssi=ssi; //recorded previous SSI. 
	
	return ssi;
}

