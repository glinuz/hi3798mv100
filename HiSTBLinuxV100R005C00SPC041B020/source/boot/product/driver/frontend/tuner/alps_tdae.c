/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: alps.c
 * Description: 
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16   w54542    NULL                Create this file.
 ***********************************************************************************/

//#include <hi_debug.h>
#include "hi_type.h"
//#include "hi_i2c.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"

#include <common.h>
//#include "drv_demod.h"
#include "alps_tdae.h"


HI_S32 alps_tdae_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32Rf)
{
    TUNER_I2C_DATA_S  stI2cDataStr = {0};
    HI_U8  au8Data[6] = {0};
    HI_U32 u32FreqPll;
	HI_U32 u32RfProg;  
    HI_U32 u32Step = 0;
    HI_S32 s32Ret = HI_SUCCESS;
  

    if ((u32Rf < ALPS_RF_MIN) ||(u32Rf > ALPS_RF_MAX))
    {
        TUNER_DEBUG( "invalid parameter(u32Rf)\n");
        return HI_FAILURE;
    }  

    u32FreqPll = (u32Rf + ALPS_IF_DEF) * 1000;
    u32Step = ALPS_STEPS_DEF;

    if ( HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType )
    {    
	    if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)	
	    {       
	        u32FreqPll += u32Step * 2;
	    }   
	    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode) 
	    {       
	        u32FreqPll += u32Step * 1 / 2;
	    }   
	    else    
	    {      
	        u32FreqPll += u32Step * 1 / 2;
	    }  
    }
    
    u32FreqPll /= u32Step;
    u32RfProg = u32FreqPll * u32Step;
        
    au8Data[0] = g_stTunerOps[u32TunerPort].u32TunerAddress;  /*Tunner Address*/
    au8Data[1] = (HI_U8)((u32FreqPll >> 8) & 0xff);    
    au8Data[2] = (HI_U8)(u32FreqPll & 0xff);
    
    if(ALPS_STEPS_DEF == u32Step)  /*62.5KHz*/
    {
        au8Data[3] = 0x9b;/*0xbb;*/
    }

    if((47000 <= u32Rf) && (u32Rf < 125000))
    {
        au8Data[4] = 0xa0;/*0x80;*/
    }
    else if((125000 <= u32Rf) && (u32Rf < 366000))
    {
        au8Data[4] = 0xa2;/*0x82;*/ 
    }
    else if((366000 <= u32Rf) && (u32Rf < 622000))
    {
        au8Data[4] = 0x68;/*0xa8;0x48;*/
    }
    else if((622000 <= u32Rf) && (u32Rf < 726000))
    {
        au8Data[4] = 0xa8;/*0x88;*/
    }
    else if((726000 <= u32Rf) && (u32Rf < 862000))
    {
        au8Data[4] = 0xe8;/*0xc8;*/
    }
    
    au8Data[5] = 0xc6;/*0xc0;*/
              
    stI2cDataStr.pu8ReceiveBuf = 0;
    stI2cDataStr.u32ReceiveLength = 0;
    stI2cDataStr.pu8SendBuf = &au8Data[1];
    stI2cDataStr.u32SendLength = 5;  /* modify by chenxu 2008-01-18 AI7D02211 */

    qam_config_i2c_out( u32TunerPort, 1 );	
    
    s32Ret =  tuner_i2c_send_data(enI2cChannel, au8Data[0], &stI2cDataStr);    
    if(HI_SUCCESS != s32Ret)
    {
        TUNER_DEBUG( "alps_set_tuner error\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

