/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: alps.c
 * Description: 
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2009-07-07   yangsd    NULL                Create this file.
 ***********************************************************************************/
//#include <hi_debug.h>
#include "hi_type.h"
//#include "hi_i2c.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "tmx7070x.h"
#include "drv_demod.h"
#include <common.h>


HI_S32 tmx7070x_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32Rf)
{
    TUNER_I2C_DATA_S  stI2cDataStr = {0};
    HI_U8  au8Data[6] = {0};
    HI_U32 u32FreqPll;
    HI_U32 u32Step = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    if ((u32Rf < TMX7070X_RF_MIN) ||(u32Rf > TMX7070X_RF_MAX))
    {
        TUNER_DEBUG( "invalid parameter(u32Rf)\n" );
        return HI_FAILURE;
    }  
#if 0
    u32FreqPll = (u32Rf + TMX7070X_IF_DEF)*1000;
    u32step = TMX7070X_STEPS_DEF;
    if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)	
    {       
        u32FreqPll += u32step*2;
    }   
    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode) 
    {       
        u32FreqPll += u32step*1/2;
    }   
    else    
    {      
        u32FreqPll += u32step*1/2;
    }    

    u32FreqPll /= u32step;
    u32RfProg = u32FreqPll*u32step;
#else
    u32FreqPll = (u32Rf + TMX7070X_IF_DEF) * 1000;
    u32Step = TMX7070X_STEPS_DEF;

    u32FreqPll /= u32Step;

#endif
    au8Data[0] = g_stTunerOps[u32TunerPort].u32TunerAddress;     /*Tunner Address*/
    
    au8Data[1] =(HI_U8)((u32FreqPll >> 8) & 0x7f);    
    au8Data[2] =(HI_U8)(u32FreqPll & 0xff);

	
    if(TMX7070X_STEPS_DEF == u32Step)  /*62.5KHz*/
    {
        au8Data[3] = 0x8b;
    }

	
	if(u32Rf <= (185000-TMX7070X_IF_DEF))
	{
		au8Data[4] = 0x01;
	}
	else if(u32Rf <= (465000-TMX7070X_IF_DEF))
	{
		au8Data[4] = 0x06;
	}
	else
	{
		au8Data[4] = 0x0c;
	}

	au8Data[5] = 0xc2;
	
    stI2cDataStr.pu8ReceiveBuf = 0;
    stI2cDataStr.u32ReceiveLength = 0;
    stI2cDataStr.pu8SendBuf = &au8Data[1];
    stI2cDataStr.u32SendLength = 5;  

    qam_config_i2c_out( u32TunerPort, 1 );	
    
    s32Ret = tuner_i2c_send_data(enI2cChannel, au8Data[0], &stI2cDataStr);
    if(HI_SUCCESS != s32Ret)
    {
        TUNER_DEBUG( "tmx7070x_set_tuner error\n" );
        return s32Ret;
    }

    return HI_SUCCESS;
}


