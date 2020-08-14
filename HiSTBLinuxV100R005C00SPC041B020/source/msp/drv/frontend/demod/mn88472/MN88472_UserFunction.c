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
#include "MN88472_UserFunction.h"
#include "hi_drv_tuner.h"

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

DMD_ERROR_t DMD_API DMD_get_dataPLPs
( DMD_u8_t * pPLPIds, DMD_u8_t * pNumPLPs , DMD_PARAMETER_t* param )
{
	DMD_u32_t i;

	//DVBT2 and Lock
	DMD_get_info( param , DMD_E_INFO_LOCK);
	if( param->system != DMD_E_DVBT2 ||
	   param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED )
	{
		//Error 
		*pNumPLPs = 0;
		return DMD_E_ERROR;
	}
	
	*pNumPLPs = 0;		//count only Data PLP
	//Get PLP Number
	DMD_get_info( param , DMD_E_INFO_DVBT2_NUM_PLP );

   #ifdef COMMON_DEBUG_ON
    HI_INFO_TUNER("--- GET DATA PLP  in !!--- \n");
    HI_INFO_TUNER("--- Total PLP NUM = %d --- \n", param->info[DMD_E_INFO_DVBT2_NUM_PLP]);
  #endif
	for(i=0;i<param->info[DMD_E_INFO_DVBT2_NUM_PLP];i++)
	{
		//select PLP No
		 DMD_set_info(   param , DMD_E_INFO_DVBT2_SELECTED_PLP , i );	
		
		/*
		| When performing a PLP switch reset the demod. Helps prevent any possible latchup
		| or performance issue that may happen when previously tuned to a "bad" PLP
		*/
		//DMD_wait( 300);
		DMD_device_reset(  param);//troy.wang, 20130412
		DMD_device_scan(  param);
		HI_INFO_TUNER("--- LOCK status = %d ---( 0 means signal locked) \n",param->info[DMD_E_INFO_LOCK]); 
		 
		 //get PLP type
		 DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_TYPE );
        #ifdef COMMON_DEBUG_ON
  
	 		HI_INFO_TUNER("--- PLP NUM[%d] TYPE is [%d] (Data plp > 0)--- \n", i, param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE]);
	    #endif

		//Find Data PLP
		 if( param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE] != DMD_E_DVBT2_PLP_TYPE_COM )
		 {
		     //get data plp's layer number, and recorded in pointer "pPLPIds"
			 DMD_get_info(  param , DMD_E_INFO_DVBT2_DAT_PLP_ID ); 
                                          
	  #ifdef COMMON_DEBUG_ON
			HI_INFO_TUNER("--- PLP NUM[%d] is DATAPLP, ID is [%d] --- \n", i, param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID]);
	  #endif
			 pPLPIds[(*pNumPLPs)] = (DMD_u8_t)param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID];
			 (*pNumPLPs) ++;
		 }
	}

	return DMD_E_OK;
}

/*
Description :  When signal is in Hierarchy mode, customer chooses to decode HP or LP data stream .
Input   : 
 val ->  1 :  HP ;   0 : LP(default)
return : None
Date   : Troy, 20121207 
*/
DMD_ERROR_t DMD_API DMD_SET_DVBT_HP_LP_MODE( DMD_PARAMETER_t* param, DMD_u32_t val) 
{
    if  ((param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)&&(param->system == DMD_E_DVBT)) //if not locked,  unable to analyse  ofdm symbol 
    	{
		 // Check HP stream exist or not .  0 : Non-Hierarchy ; > 0 : Hierarchy 
	     DMD_get_info( param , DMD_E_INFO_DVBT_HIERARCHY);

		 // 0 : DMD_E_DVBT_HIER_SEL_LP(default),	1: DMD_E_DVBT_HIER_SEL_HP 
	      if  (param->info[DMD_E_INFO_DVBT_HIERARCHY] != DMD_E_DVBT_HIERARCHY_NO)
	      	{
			HI_INFO_TUNER("DVB-T Hierarchy exist, select [%d] ? HP : LP\n",val);
			DMD_set_info(param, DMD_E_INFO_DVBT_HIERARCHY_SELECT, val);
	      	}
		   else
		   	{
			HI_INFO_TUNER("DVB-T Hierarchy NOT exist, select default LP\n");
			DMD_set_info(param, DMD_E_INFO_DVBT_HIERARCHY_SELECT, 0);
		   	}
    	}
	else
		{
			HI_INFO_TUNER( " INVALID !! signal unlocked or DTV system not DVBT \n");            
		}
      	return DMD_E_OK;
}

void DMD_debug_after_signal_locked( DMD_PARAMETER_t* param)
{
		if(param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)
		{   
				DMD_wait( 2000); //wait for stream be decoded 

				if ( param->system == DMD_E_DVBT2)
			{
				DMD_get_info(   param , DMD_E_INFO_DVBT2_CNR_INT ); 
				DMD_get_info( param, DMD_E_INFO_DVBT2_PERRNUM);
				HI_INFO_TUNER(" -DVBT2-- CNR status --- CNRINT = 0x%x, CNRDEC = 0x%x",param->info[DMD_E_INFO_DVBT2_CNR_INT],param->info[DMD_E_INFO_DVBT2_CNR_DEC]); 
				HI_INFO_TUNER("\n it is locked! pernum = 0x%x, perpack = 0x%x\n",param->info[DMD_E_INFO_DVBT2_PERRNUM],param->info[DMD_E_INFO_DVBT2_PACKETNUM]);

				HI_INFO_TUNER(" CLK bias :  \n");  
				DMD_Carrier_Frequency_bias( param);//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
				DMD_XTAL_Frequency_bias( param);//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.
				}
			else if(   param->system == DMD_E_DVBT )
			{
				DMD_get_info(   param , DMD_E_INFO_DVBT_CNR_INT ); 
				DMD_get_info( param, DMD_E_INFO_DVBT_PERRNUM);
				HI_INFO_TUNER(" --DVBT- CNR status --- CNRINT = 0x%x, CNRDEC = 0x%x",param->info[DMD_E_INFO_DVBT_CNR_INT],param->info[DMD_E_INFO_DVBT_CNR_DEC]); 
				HI_INFO_TUNER("\n it is locked! pernum = 0x%x, perpack = 0x%x\n",param->info[DMD_E_INFO_DVBT_PERRNUM],param->info[DMD_E_INFO_DVBT_PACKETNUM]);

				HI_INFO_TUNER(" CLK bias :  \n");  
				DMD_Carrier_Frequency_bias( param);//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
				DMD_XTAL_Frequency_bias( param);//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.
			}
			else
			{
				HI_INFO_TUNER("------------unknow system in ------------\n");				
			}
		}
		else
		{
			HI_INFO_TUNER("------------ Pls. check signal analysing status after signal locked!! ------------\n");
		}
}
