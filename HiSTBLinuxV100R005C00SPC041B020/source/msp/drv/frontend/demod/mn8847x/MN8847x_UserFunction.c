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
#include "MN_DMD_driver.h"
#include "MN_DMD_device.h"
#include "MN_I2C.h"
#include "MN8847x_UserFunction.h" 

/**
@brief Get the data PLPs that the demodulator has detected.
	If a single PLP service is in use, then pNumPLPs = 1
	and the plpIds[0] shall contain the signalled PLP Id.

@param pPLPIds Pointer to an array of at least 256 bytes in length 
 that can receive the list of data PLPs carried.

@param pNumPLPs The number of data PLPs detected (signalled in L1-post).

@param param	object for target device

@return DMD_E_OK if the pPLPIds and pNumPLPs are valid.
*/
DMD_ERROR_t DMD_API MN8847X_DMD_get_dataPLPs
(  DMD_u8_t * pPLPIds, DMD_u8_t * pNumPLPs , DMD_PARAMETER_t* param )
{
	DMD_u8_t plp_layer_number;

	//DVBT2 and Lock
	MN8847X_DMD_get_info(  param , DMD_E_INFO_LOCK);
	if( param->system != DMD_E_DVBT2 ||
	param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED )
	{
	printk("--- T2 signal unlocked !!--- \n");
	//Error 
	*pNumPLPs = 0;
	return DMD_E_ERROR;
	}

	//count only Data PLP, to record how many Data PLP signal has.
	*pNumPLPs = 0;		

	//Get PLP Number
	MN8847X_DMD_get_info(  param , DMD_E_INFO_DVBT2_NUM_PLP );

#ifdef DETAIL_DIG_DEBUG_ON
	printk("--- GET DATA PLP  in !!--- \n");
	printk("--- Total PLP NUM = %d --- \n", param->info[DMD_E_INFO_DVBT2_NUM_PLP]);
#endif

	for(plp_layer_number=0;plp_layer_number<param->info[DMD_E_INFO_DVBT2_NUM_PLP];plp_layer_number++)
	{
	//select PLP No
	MN8847X_DMD_set_info(   param , DMD_E_INFO_DVBT2_SELECTED_PLP , plp_layer_number );			
	/*
	| When performing a PLP switch reset the demod. Helps prevent any possible latchup
	| or performance issue that may happen when previously tuned to a "bad" PLP
	*/
	//DMD_wait( 300);
	MN8847X_DMD_device_reset(param);//troy.wang, 20130412
	MN8847X_DMD_device_scan(param);
#ifdef DETAIL_DIG_DEBUG_ON
	printk("---PLP NUM[%d] LOCK status = %d ---( 0 means signal locked) \n",plp_layer_number,param->info[DMD_E_INFO_LOCK]); 
#endif

	if(	param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED )
	{
	printk("--- T2 signal unlocked, at PLP[%d] !!--- \n",plp_layer_number);
	return DMD_E_ERROR;
	}

	//get PLP type
	MN8847X_DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_TYPE );
#ifdef DETAIL_DIG_DEBUG_ON  
	printk("--- PLP NUM[%d] TYPE is [%d] (Data plp > 0)--- \n", plp_layer_number, param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE]);
#endif
  
		//Find Data PLP
		if( param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE] != DMD_E_DVBT2_PLP_TYPE_COM )
		{		     
#if 0 //get data PLP's ID(may be different as PLP layer nubmer), and recorded in pointer "pPLPIds"
			DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_ID ); 
			pPLPIds[(*pNumPLPs)] = (DMD_u8_t)param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID];
#else //get data plp's layer number, and recorded in pointer "pPLPIds"			 
			pPLPIds[(*pNumPLPs)] = plp_layer_number; //20131219, for plp layer ID != plp id
#ifdef DETAIL_DIG_DEBUG_ON
			MN8847X_DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_ID ); 
			printk("--- PLP NUM[%d] is DATAPLP, PLPID is [%d] --- \n", plp_layer_number, param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID]);
#endif
#endif                               

		(*pNumPLPs) ++; 
		}
	}

	return DMD_E_OK;
}

/*
Description :  get PLP type of DVB-T2
Input   : 
 plp_layer_number ->  max 255, after set PLP, pls. wait signal locked.
return : value valid only signal locked
Date   : Troy, 20121207 
*/
DMD_ERROR_t DMD_API MN8847X_DMD_get_single_PLP_type(DMD_PARAMETER_t* param, DMD_u8_t plp_layer_number)
{
	//DVBT2 and Lock
	MN8847X_DMD_get_info(  param , DMD_E_INFO_LOCK);
	if( param->system != DMD_E_DVBT2 ||
	param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED )
	{
	//Error 
	return DMD_E_ERROR;
	}

	//select PLP No
	MN8847X_DMD_set_info(   param , DMD_E_INFO_DVBT2_SELECTED_PLP , plp_layer_number );			
	/*
	| When performing a PLP switch reset the demod. Helps prevent any possible latchup
	| or performance issue that may happen when previously tuned to a "bad" PLP
	*/
	//DMD_wait( 300);
	MN8847X_DMD_device_reset(  param);//troy.wang, 20130412
	MN8847X_DMD_device_scan(  param);

#ifdef DETAIL_DIG_DEBUG_ON  
	printk("-%s--PLP NUM[%d] LOCK status = %d ---( 0 means signal locked) \n",__FUNCTION__,plp_layer_number,param->info[DMD_E_INFO_LOCK]); 
#endif

	if(	param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED )
	{
	return DMD_E_ERROR;
	}
	
	//get PLP type
	MN8847X_DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_TYPE );
#ifdef DETAIL_DIG_DEBUG_ON  
	printk("--- PLP NUM[%d] TYPE is [%d] (Data plp > 0)--- \n", plp_layer_number, param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE]);
#endif

    return DMD_E_OK;
}
/*
Description :  When signal is in Hierarchy mode, customer chooses to decode HP or LP data stream .
Input   : 
 val ->  1 :  HP ;   0 : LP(default)
return : None
Date   : Troy, 20121207 
*/
DMD_ERROR_t DMD_API MN8847X_DMD_SET_DVBT_HP_LP_MODE( DMD_PARAMETER_t* param, DMD_u32_t val) 
{
    if  ((param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)&&(param->system == DMD_E_DVBT)) //if not locked,  unable to analyse  ofdm symbol 
    	{
		 // Check HP stream exist or not .  0 : Non-Hierarchy ; > 0 : Hierarchy 
	     MN8847X_DMD_get_info( param , DMD_E_INFO_DVBT_HIERARCHY);

		 // 0 : DMD_E_DVBT_HIER_SEL_LP(default),	1: DMD_E_DVBT_HIER_SEL_HP 
	      if (param->info[DMD_E_INFO_DVBT_HIERARCHY] != DMD_E_DVBT_HIERARCHY_NO)
	      	{
	      	#ifdef COMMON_DEBUG_ON
			printk("DVB-T Hierarchy exist, select [%d] ? HP : LP\n",val);
			#endif
			MN8847X_DMD_set_info(param, DMD_E_INFO_DVBT_HIERARCHY_SELECT, val);
	      	}
		   else
		   	{
		   	#ifdef COMMON_DEBUG_ON
			printk("DVB-T Hierarchy NOT exist, select default LP\n");
			#endif
			MN8847X_DMD_set_info(param, DMD_E_INFO_DVBT_HIERARCHY_SELECT, 0);
		   	}
    	}
	else
		{
			printk( " INVALID !! signal unlocked or DTV system not DVBT \n");            
		}
      	return DMD_E_OK;
}

void MN8847X_DMD_debug_after_signal_locked( DMD_PARAMETER_t* param)
{
        MN8847X_DMD_get_info(param,DMD_E_INFO_LOCK);
		if(param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)
		{   
				MN8847X_DMD_wait( 2000); //wait for stream be decoded 

				if ( param->system == DMD_E_DVBT2)
			{
				MN8847X_DMD_get_info(   param , DMD_E_INFO_DVBT2_CNR_INT ); 
				MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT2_PERRNUM);
				printk(" -DVBT2-- CNR status --- CNRINT = 0x%x, CNRDEC = 0x%x",param->info[DMD_E_INFO_DVBT2_CNR_INT],param->info[DMD_E_INFO_DVBT2_CNR_DEC]); 
				printk("\n it is locked! pernum = 0x%x, perpack = 0x%x\n",param->info[DMD_E_INFO_DVBT2_PERRNUM],param->info[DMD_E_INFO_DVBT2_PACKETNUM]);

				printk(" CLK bias :  \n");  
				MN8847X_DMD_Carrier_Frequency_bias( param);//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
				MN8847X_DMD_XTAL_Frequency_bias( param);//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.
				}
			else if(   param->system == DMD_E_DVBT )
			{
				MN8847X_DMD_get_info(   param , DMD_E_INFO_DVBT_CNR_INT ); 
				MN8847X_DMD_get_info( param, DMD_E_INFO_DVBT_PERRNUM);
				printk(" --DVBT- CNR status --- CNRINT = 0x%x, CNRDEC = 0x%x",param->info[DMD_E_INFO_DVBT_CNR_INT],param->info[DMD_E_INFO_DVBT_CNR_DEC]); 
				printk("\n it is locked! pernum = 0x%x, perpack = 0x%x\n",param->info[DMD_E_INFO_DVBT_PERRNUM],param->info[DMD_E_INFO_DVBT_PACKETNUM]);

				printk(" CLK bias :  \n");  
				MN8847X_DMD_Carrier_Frequency_bias( param);//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
				MN8847X_DMD_XTAL_Frequency_bias( param);//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.
			}
			else
			{
				printk("------------unknow system in ------------\n");				
			}
		}
		else
		{
			printk("------------ Pls. check signal analysing status after signal locked!! ------------\n");
		}
}

