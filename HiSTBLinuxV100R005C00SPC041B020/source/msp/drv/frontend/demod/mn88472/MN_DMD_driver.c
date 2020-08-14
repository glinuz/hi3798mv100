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
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include "MN_DMD_driver.h"
#include "MN_DMD_device.h"
#include "MN_DMD_common.h"
#include "MN_I2C.h"
#include "MN_Tuner.h"
#include "drv_tuner_ioctl.h"
#include "hi_drv_tuner.h"

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
DMD_ERROR_t DMD_API DMD_set_devid( DMD_PARAMETER_t* param , DMD_u32_t id ){
	param->devid = id;

	return DMD_E_OK;
}


/*!	Demodulator API Initialize & Open */
/*!
	*initialize parameter variable (DMD_PARAMETER_t* param)
	*open & connect to USB Device for I2C communication
	 (for WINDOWS API only)
*/
DMD_ERROR_t DMD_API DMD_open( DMD_PARAMETER_t* param)
{
	int i;

	param->devid = 0;
	param->system= DMD_E_NOT_DEFINED;
	/* Initialize Parameter Variable */
	for(i=0;i<DMD_INFORMATION_MAX;i++){
		param->info[i] = 0;		//NOT_DEFINED
	}
	param->bw		= DMD_E_BW_NOT_SPECIFIED;
	param->freq		= 666000;
	param->funit	= DMD_E_KHZ;

	if( DMD_device_open( param ) == DMD_E_ERROR )
	{
		DMD_DBG_TRACE( "ERROR: DMD open" );
		return DMD_E_ERROR;

	}

	if( DMD_I2C_open() == DMD_E_ERROR )
	{
		DMD_DBG_TRACE( "ERROR: I2C open" );
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
DMD_ERROR_t	DMD_API	DMD_init( DMD_PARAMETER_t* param )
{
	/* Tuner_init */
	/*if( DMD_Tuner_init(param) == DMD_E_ERROR )
	{
		HI_ERR_TUNER( "ERROR: Tuner Initialize" );
		return DMD_E_ERROR;
	}*/
	/* Demodulator LSI Initialize */
	if( DMD_device_init( param ) == DMD_E_ERROR )
	{
		HI_ERR_TUNER( "ERROR: DMD Initialize" );
		return DMD_E_ERROR;
	}
	/* Auto Control Sequence Transfer */
	if( DMD_device_load_pseq( param ) == DMD_E_ERROR )
	{
		HI_ERR_TUNER( "ERROR: DMD Load Autocontrol" );
		return DMD_E_ERROR;
	}

	/* Tuner_init */
	if( DMD_Tuner_init( ) != DMD_E_OK )
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
DMD_ERROR_t DMD_API DMD_set_system( DMD_PARAMETER_t* param ){
#if 0
	if( DMD_Tuner_set_system( param ) == DMD_E_ERROR ){
		HI_ERR_TUNER("ERROR:Tuner set system");
		return DMD_E_ERROR;
	}
#endif	
	if( DMD_device_set_system( param ) == DMD_E_ERROR ){
		HI_ERR_TUNER("ERROR:device set system");
		return DMD_E_ERROR;
	}
	/* '11/08/29 : OKAMOTO	Select TS output. */
	if( DMD_set_ts_output( param, param->ts_out ) != DMD_E_OK )
	{
		DMD_DBG_TRACE("ERROR:set ts output");
		return DMD_E_ERROR;
	}

   //DMD_increase_compatibility_with_nonstandard_signal(); //normally, not use
	
	return DMD_E_OK;
}

/*! Tune */
DMD_ERROR_t DMD_API DMD_tune(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t ret;
	/* Call pre tune process */
	ret = DMD_device_pre_tune(  param );
	/* Tuner */
	ret |= DMD_Tuner_tune( param );
	/* Call post tune process */
	ret |= DMD_device_post_tune(  param );
	return ret;
}

/*! scan */
DMD_ERROR_t DMD_API DMD_scan(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	DMD_u8_t pseq_status = 0;
	/* Call pre tune process */
	DMD_device_pre_tune(  param ); //default empty 
	
	/* Tuner: RF and BW input*/
   ret = DMD_Tuner_tune(  param ); 
	if (ret != DMD_E_OK)
	{ 
		HI_INFO_TUNER(" Tuner lock signal NG, not go to demod !!!\n") ;  
		return ret;
	}
		
	/* DMD software reset */
	DMD_device_post_tune(  param );
	
	/* Call Lock/SYNC Status Judgement */
	ret = DMD_device_scan(  param );

    ret = DMD_device_pseq_status(param,&pseq_status);//20130412, troy added, to judge if autoctrl runs or stops.
    if ( pseq_status == 1)
	{
		HI_INFO_TUNER("-------------Autoctrl stops !! pls. check load_pseq function !--------");
		ret = DMD_E_ERROR;
	}
	
	return ret;
}

/*! term */
DMD_ERROR_t DMD_API DMD_term(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_OK;

	ret = DMD_device_term( param);
	ret |= DMD_Tuner_term( );

	return ret;
}

/*! close */
DMD_ERROR_t DMD_API DMD_close( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_OK;

	ret = DMD_device_close(param);

	return ret;
}

/* **************************************************** */
/* Set Information */
/* **************************************************** */
DMD_ERROR_t DMD_API DMD_set_info( DMD_PARAMETER_t* param , DMD_u32_t id  , DMD_u32_t val){

	return DMD_device_set_info( param , id , val );
}

/* **************************************************** */
/* Get Information */
/* **************************************************** */
DMD_ERROR_t DMD_API DMD_get_info( DMD_PARAMETER_t* param , DMD_u32_t id ){

	return DMD_device_get_info( param , id );
}

DMD_text_t DMD_API DMD_info_title( DMD_SYSTEM_t sys ,DMD_u32_t id )
{

	if( id < DMD_E_INFO_COMMON_END_OF_INFORMATION )
	{
		return DMD_INFO_TITLE_COMMON[id];

	}
	else
		switch( sys )
		{
		case DMD_E_DVBT:
			if( id < DMD_E_INFO_DVBT_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return DMD_INFO_TITLE_DVBT[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_DVBT2:
			if( id < DMD_E_INFO_DVBT2_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return DMD_INFO_TITLE_DVBT2[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_DVBC:
			if( id < DMD_E_INFO_DVBC_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return DMD_INFO_TITLE_DVBC[id];
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
				return DMD_INFO_TITLE_ISDBT[id];
			}
			else
			{
				return "Not Defined";
			}
			break;
		case DMD_E_ISDBS:
			if( id < DMD_E_INFO_ISDBS_END_OF_INFORMATION ){
				id -= DMD_E_INFO_COMMON_END_OF_INFORMATION;
				return DMD_INFO_TITLE_ISDBS[id];
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

DMD_text_t DMD_API DMD_info_value( DMD_SYSTEM_t sys ,DMD_u32_t id , DMD_u32_t val )
{


	if( id < DMD_E_INFO_COMMON_END_OF_INFORMATION && id != DMD_E_INFO_STATUS )
	{
		return DMD_info_value_common( id ,val );

	}
	else 
	{
		switch( sys )
		{
		case DMD_E_DVBT2:
			return DMD_info_value_dvbt2( id , val );
			break;
		case DMD_E_DVBT:
			return DMD_info_value_dvbt( id , val );
			break;
#ifndef DMD_DISABLE_ISDB
		case DMD_E_ISDBT:
		case DMD_E_ISDBT_BRAZIL:
			return DMD_info_value_isdbt( id , val );
			break;
		case DMD_E_ISDBS:
			return DMD_info_value_isdbs( id , val );
#endif
		case DMD_E_ATSC:
		case DMD_E_QAMB_256QAM:
		case DMD_E_QAMB_64QAM:
		case DMD_E_QAMC_256QAM:
		case DMD_E_QAMC_64QAM:
			return DMD_info_value_vq( id , val );
		case DMD_E_DVBC:
			return DMD_info_value_dvbc( id , val );
			break;
		default:
			break;

		}
	}

	return "Not Defined";
}
DMD_text_t DMD_API DMD_value_text( DMD_PARAMETER_t* param , DMD_u32_t id )
{
	return DMD_info_value( param->system , id , param->info[id] );
}


/* **************************************************** */
/* Floating Functions */
/* **************************************************** */
// Get BER
DMD_float_t DMD_API DMD_get_ber( DMD_PARAMETER_t* param )
{
	DMD_float_t ret;
#ifdef DMD_FLOATING_FUNCTION
	DMD_get_info( param , DMD_E_INFO_BERRNUM );

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
DMD_float_t DMD_API DMD_get_per( DMD_PARAMETER_t* param )
{
	DMD_float_t ret;
#ifdef DMD_FLOATING_FUNCTION
	DMD_get_info( param , DMD_E_INFO_PACKETNUM );

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
DMD_float_t DMD_API DMD_get_cnr( DMD_PARAMETER_t* param )
{
	DMD_float_t	ret;
	DMD_get_info( param , DMD_E_INFO_CNR_INT );
#ifdef DMD_FLOATING_FUNCTION

	ret = (DMD_float_t) param->info[DMD_E_INFO_CNR_INT] 
	     +  (DMD_float_t) param->info[DMD_E_INFO_CNR_DEC] / ( (DMD_float_t) 100.0 );
#else
	ret = (DMD_float_t) param->info[DMD_E_INFO_CNR_INT];
#endif


	return ret;
}


int Get_RF_T_REF_PWR( DMD_PARAMETER_t * param)
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
		DMD_get_info( param, DMD_E_INFO_DVBT_CONSTELLATION);
		constellation = param->info[DMD_E_INFO_DVBT_CONSTELLATION];

		// Check HP stream exist or not 	 
		DMD_get_info( param, DMD_E_INFO_DVBT_HIERARCHY);			
		hierarchy_exist= param->info[DMD_E_INFO_DVBT_HIERARCHY];

		DMD_get_info( param, DMD_E_INFO_DVBT_HP_CODERATE);	
		DMD_get_info( param, DMD_E_INFO_DVBT_LP_CODERATE);					

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
typedef enum{	DMD_E_DVBT_CONST_QPSK=0,	DMD_E_DVBT_CONST_16QAM,
	DMD_E_DVBT_CONST_64QAM	}	DMD_DVBT_CONST_t;	

typedef enum{	DMD_E_DVBT_CR_1_2=0,	DMD_E_DVBT_CR_2_3,	
	DMD_E_DVBT_CR_3_4,	DMD_E_DVBT_CR_5_6,	DMD_E_DVBT_CR_7_8	}	DMD_DVBT_CR_t;
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


int Get_RF_T2_REF_PWR( DMD_PARAMETER_t * param)
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

		DMD_get_info( param, DMD_E_INFO_DVBT2_DAT_PLP_MOD);
		constellation = param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD];

		DMD_get_info( param,DMD_E_INFO_DVBT2_DAT_PLP_COD);
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

int Calc_T_SSI( DMD_PARAMETER_t * param)
{ 
	int Prec=0,  Prel=0,  Pref=0; 
	int ssi=0; 
	int rxPwrPtr; 

	Tuner_check_RF_input_power(0,&rxPwrPtr);

	Prec=(int)rxPwrPtr/100;

#ifdef COMMON_DEBUG_ON 
	HI_INFO_TUNER("\n Calc_T_SSI_MxLWare603_API_ReqTunerRxPower=%x\n",(int)rxPwrPtr); 
#endif


	  Pref=Get_RF_T_REF_PWR( param);
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

int Calc_T2_SSI( DMD_PARAMETER_t * param)
{ 
			int   Prec=0, Prel=0,  Pref=0; 
			int ssi=0; 
			static int pre_ssi=0;
			DMD_u8_t rd; 
			int rxPwrPtr; 		  
			
		Tuner_check_RF_input_power(0,&rxPwrPtr); 
		
		Prec=(int)rxPwrPtr/100;

		 #ifdef COMMON_DEBUG_ON 
		 	HI_INFO_TUNER("\n Calc_T2_SSI_MxLWare603_API_ReqTunerRxPower=%x\n",(int)rxPwrPtr); 
		 #endif
   
    //troy, 20130131, for T2 signal with FEF together, sometimes SSI return 0 but video play OK. 
    DMD_device_read_P1_flag( param, &rd );
	if (( Prec <= -98 )&&( rd & 0x01))
   	{
   		//printf("FEF detected !\n");
		return pre_ssi;
   	} 
	
	Pref = Get_RF_T2_REF_PWR( param);
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
