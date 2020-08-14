/***************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 3 API
   L3 sample application
   FILE: Si2147_L3_Test.c
   Supported IC : Si2147
   Compiled for ROM 33 firmware 2_1_build_9
   Revision: 0.1
   Tag:  ROM33_2_1_build_9_V0.1
   Date: March 21 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/
#include "Si2147_L2_API.h"
#include "Si2147_L0_API.h"
#include "drv_i2c_ext.h"
#include "hi_type.h"
#include <linux/file.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/param.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

#include "hi_debug.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

static HI_U32 s_u32Si2147TunerPort = 0 ;
static HI_U32 s_u32Si2147I2cChannel = 5;
//static HI_U32 s_u32Si2147Init = 0;



/* define TUNER_ONLY if using a Si2147 tuner without demod                                                    */
/* (It should be defined at project/makefile level to use 100% identical code for tuner-only and tuner+demod)   */
 //#define TUNER_ONLY 



#define FRONT_END_COUNT 1

int  commandCount  = 0;
int  fe;

static L1_Si2147_Context         FrontEnd_Table[FRONT_END_COUNT];
L1_Si2147_Context         *Si2147;
L1_Si2147_Context         *front_end;

/* Also init a simple i2c context to allow i2c communication checking */
//L0_Context* i2c;
//L0_Context  i2c_context;
CONNECTION_TYPE mode;



/* define SILABS_DEMOD_EVB if used on a tuner_only application on a demod EVB (opens i2c_passthru in main)      */
/* (It should be defined at project/makefile level to use 100% identical code for tuner-only EVB and demod EVB) */
/* #define SILABS_DEMOD_EVB*/

/* define Si2147_COMMAND_LINE_APPLICATION if using this code in console mode                                                     */
/* (Si2147_COMMAND_LINE_APPLICATION should be defined at project/makefile level to use 100% identical code for console and TV )  */
/* #define Si2147_COMMAND_LINE_APPLICATION */

/* define FRONT_END_COUNT 1 for a single tuner application                                                                         */
/* (FRONT_END_COUNT should be defined at project/makefile level to use 100% identical code for single-frontend and multi-frontend) */





//extern HI_U32  si2147_tda_write(HI_U8 uAddress, HI_U32 uNbData, HI_U8 * pDataBuff);
//extern HI_U32  si2147_tda_read(HI_U8 uAddress,HI_U32 uNbData, HI_U8 * pDataBuff);

HI_S32 si2147_init_tuner (HI_U32 u32TunerPort)
{   
     
    /* I2C pointer initialization (to allow i2c validation) */
    //unsigned long RFMaster = u32PuRF * 1000;
	//int  commandCount  = 0;
//	L1_Si2147_Context          FrontEnd_Table[FRONT_END_COUNT] = {{0}};
	//L1_Si2147_Context         *Si2147;
	//L1_Si2147_Context         *front_end = NULL;
    //HI_S32 s32Ret = 0;
	HI_S32 fe = 0;;
	//L0_Context* i2c;
	//L0_Context  i2c_context;
	//CONNECTION_TYPE mode;
    //i2c = &i2c_context;

    //L0_Connect(i2c,CUSTOMER);
   
     s_u32Si2147I2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
	 
    /* Software Init */ 
      front_end = &(FrontEnd_Table[fe]);

	  //g_stTunerOps[u32TunerPort].u32TunerAddress = 0xc7;
       

      Si2147_L1_API_Init(front_end, g_stTunerOps[u32TunerPort].u32TunerAddress);
      front_end->i2c->connectionType = CUSTOMER;
	  
      if (Si2147_Init(front_end) != 0 ) 
     {
        HI_ERR_TUNER("ERROR initializing the Si2147!\n");
		
        return -1;
      }
      else 
      {
	    HI_INFO_TUNER("initializing success\n");
		
	    return 0;
      }
     // Si2147_DTVTune(front_end, RFMaster, Si2147_DTV_MODE_PROP_BW_BW_8MHZ, Si2147_DTV_MODE_PROP_MODULATION_DVBC, Si2147_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL);
	

}

HI_S32 si2147_set_tuner (HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{   
     
       /* I2C pointer initialization (to allow i2c validation) */
    unsigned long RFMaster = u32PuRF * 1000;
	HI_S32 s32Ret = 0;

	//int  commandCount  = 0;
	//L1_Si2147_Context          FrontEnd_Table[FRONT_END_COUNT] = {{0}};
	//L1_Si2147_Context         *Si2147;
	//L1_Si2147_Context         *front_end;
      
	HI_S32 fe = 0;
	//L0_Context* i2c;
	//L0_Context  i2c_context;
	//CONNECTION_TYPE mode;
    //i2c = &i2c_context;
	
    //L0_Connect(i2c,CUSTOMER);

    //s_u32Si2147I2cChannel = enI2cChannel;
    /* Software Init */ 
      front_end = &(FrontEnd_Table[fe]);
      Si2147_L1_API_Init(front_end, g_stTunerOps[u32TunerPort].u32TunerAddress);
      front_end->i2c->connectionType = CUSTOMER;
	 

#if 0
      if (Si2147_Init(front_end) != 0 ) 
     {
        SiTRACE("ERROR initializing the Si2147 at 0x%x!\n",g_stTunerOps[u32TunerPort].u32TunerAddress );
      }
      else 
      {
	  SiTRACE("initializing success\n");
      }
 #endif


     s32Ret = Si2147_DTVTune(front_end, RFMaster, Si2147_DTV_MODE_PROP_BW_BW_8MHZ, Si2147_DTV_MODE_PROP_MODULATION_DVBC, Si2147_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL);
 	if(s32Ret!= 0)
 	{
   		HI_ERR_TUNER("!!!Si2147_DTVTune failed"); 
 	}

     return s32Ret;
}

HI_S32 si2147_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{

  int ret = 0;
   #if 1
    
	//L1_Si2147_Context          FrontEnd_Table[FRONT_END_COUNT] = {{0}};
    //L1_Si2147_Context         *front_end;
	HI_S32 fe = 0;
         	
	front_end = &(FrontEnd_Table[fe]);
	
	Si2147_L1_API_Init(front_end, g_stTunerOps[u32TunerPort].u32TunerAddress);
	front_end->i2c->connectionType = CUSTOMER;
	#endif
	
	ret = Si2147_GetRF(front_end);

  
	pu32SignalStrength[0] = 0;
	pu32SignalStrength[2] = 0;
	pu32SignalStrength[1] = (HI_U32)front_end->rsp->tuner_status.rssi;
	
	if(pu32SignalStrength[1]>= 150)
	{
	    pu32SignalStrength[1] = pu32SignalStrength[1] - 145;
	}
	else
	{
	    pu32SignalStrength[1] = pu32SignalStrength[1] + 109;
	}

	return HI_SUCCESS;
}


HI_S32 si2147_tuner_resume (HI_U32 u32TunerPort)
{   
    s_u32Si2147TunerPort = u32TunerPort;
    si2147_init_tuner(s_u32Si2147TunerPort);    
    return HI_SUCCESS;	
}



	

