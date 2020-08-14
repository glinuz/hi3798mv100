//#include <linux/delay.h>
//#include <linux/semaphore.h>

//#include "linux/kernel.h"

//#include <hi_debug.h>
#include "hi_type.h"
//#include "drv_i2c_ext.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#ifndef HI_MINIBOOT_SUPPORT
#include <common.h>
#endif
/*
#include "tmtypes.h"
#include "dvp.h"
#include "tmhalFrontEndtypes.h"
//#include "tmhalFEtypes.h"

#include "tmbslTuner.h"
#include "tmbslTDA18251.h"
#include "tmbslTDA18251local.h"
#include "tmbslTDA18251StaticInstance.h"*/
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          main.c
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Sample application code of the NXP TDA18250 driver.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

//*--------------------------------------------------------------------------------------
//* Include Standard files
//*--------------------------------------------------------------------------------------
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmbslTDA18250A.h"
#include "tmbslTDA18250A_Advanced.h"
#include "tmbslTDA18250A_Local.h"

static HI_U32 s_u32Tda18250ATunerPort = 0;

static Bool tda_read (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
static Bool tda_write (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
static Bool tda_wait (UInt32 ms);
//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t  tda18250AUserWrittenI2CRead (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 ReadLen, UInt8 * pData);
tmErrorCode_t tda18250AWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize,
	UInt8 * pAddr, UInt32 WriteLen, UInt8 * pData);
tmErrorCode_t tda18250AUserWrittenWait (tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t tda18250AUserWrittenPrint (UInt32 level, const char *format, ...);
tmErrorCode_t tda18250AUserWrittenMutexInit (ptmbslFrontEndMutexHandle * ppMutexHandle);
tmErrorCode_t tda18250AUserWrittenMutexDeInit (ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t tda18250AUserWrittenMutexAcquire (ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
tmErrorCode_t tda18250AUserWrittenMutexRelease (ptmbslFrontEndMutexHandle pMutex);

//*--------------------------------------------------------------------------------------
//* Template of function to be provided by customer
//*--------------------------------------------------------------------------------------

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenI2CRead
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenI2CRead (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 ReadLen, UInt8 * pData)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

	err = tda_read (g_stTunerOps[s_u32Tda18250ATunerPort].u32TunerAddress, *pAddr, ReadLen, pData);
/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tdatda18250AWrittenI2CWrite
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 WriteLen, UInt8 * pData)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

	err = tda_write (g_stTunerOps[s_u32Tda18250ATunerPort].u32TunerAddress, *pAddr, WriteLen, pData);

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenWait
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenWait (tmUnitSelect_t tUnit, UInt32 tms)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

	tda_wait (tms);

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenPrint
//* Object              : 
//* Input Parameters    :   UInt32 level, const char* format, ...
//*                         
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------

tmErrorCode_t tda18250AUserWrittenPrint (UInt32 level, const char *format, ...)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;
	return err;
}

//struct semaphore tda18250A_sem;
//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenMutexInit
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenMutexInit (ptmbslFrontEndMutexHandle * ppMutexHandle)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenMutexDeInit
//* Object              : 
//* Input Parameters    :    ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenMutexDeInit (ptmbslFrontEndMutexHandle pMutex)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenMutexAcquire
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenMutexAcquire (ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/
	//err = down_interruptible (&tda18250A_sem);

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : tda18250AUserWrittenMutexRelease
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t tda18250AUserWrittenMutexRelease (ptmbslFrontEndMutexHandle pMutex)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

	//up (&tda18250A_sem);

/* ...*/
/* End of Customer code here */

	return err;
}

//////////////////////////////////////////////////////////////

#define TDA_I2C_ADDR    (0xC0)
#define SITUNER_TRACE_LEVEL 8
#define situner_trace(level, msg...) do { \
		if((level) >= SITUNER_TRACE_LEVEL) { \
			TUNER_DEBUG(KERN_INFO "situner_trace:%s:%d: ", __FILE__, __LINE__); \
			TUNER_DEBUG(msg); \
			TUNER_DEBUG("\n"); \
		} \
	}while(0)


static Bool tda_write (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff)
{
	HI_U8  i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	//HI_U8 au8SendData[65] = { 0 };
	HI_U8 au8SendData[TDA18250A_REG_MAP_NB_BYTES] = { 0 };
	HI_U8 *pu8Tmp = NULL;


	if (NULL == pDataBuff)
	{
		TUNER_DEBUG( "pointer is null\n");
		return HI_FAILURE;
	}

	qam_config_i2c_out (s_u32Tda18250ATunerPort, 1);

	pu8Tmp = pDataBuff;
	au8SendData[0] = uSubAddress;
	for (i = 0; i < uNbData; i++)
	{
		au8SendData[i + 1] = *pu8Tmp;
		pu8Tmp++;
	}

	stI2cDataStr.pu8ReceiveBuf = 0;
	stI2cDataStr.u32ReceiveLength = 0;
	stI2cDataStr.pu8SendBuf = au8SendData;
	stI2cDataStr.u32SendLength = uNbData + 1;	//subaddress + data
	s32Ret = tuner_i2c_send_data (g_stTunerOps[s_u32Tda18250ATunerPort].enI2cChannel, uAddress, &stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		TUNER_DEBUG("[tda18251 error]%s %d IIC write err!status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;

}

static Bool tda_read (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff)
{
	HI_S32 i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	//HI_U8 au8RcvData[65] = { 0 };
	HI_U8 au8RcvData[TDA18250A_REG_MAP_NB_BYTES] = { 0 };


	qam_config_i2c_out (s_u32Tda18250ATunerPort, 1);
	memset ((void *) au8RcvData, 0, sizeof (au8RcvData));

	stI2cDataStr.pu8ReceiveBuf = au8RcvData;
	stI2cDataStr.pu8SendBuf = &uSubAddress;
	stI2cDataStr.u32ReceiveLength = uNbData;
	stI2cDataStr.u32SendLength = 1;

	s32Ret = tuner_i2c_receive_data (g_stTunerOps[s_u32Tda18250ATunerPort].enI2cChannel, uAddress /*|0x01 */ ,
		&stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		TUNER_DEBUG("[tda18251 error]%s %d IIC read err! status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	for (i = 0; i < uNbData; i++)
	{
		pDataBuff[i] = au8RcvData[i];
	}

	return HI_SUCCESS;
}

static Bool tda_wait (UInt32 ms)
{
	msleep_interruptible (ms);

	return HI_SUCCESS;
}

HI_S32 tda18250b_set_tuner (HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);

#define SK_TRACE() TUNER_DEBUG("__file__:%s,__function__:%s, __line__:%d\n", __FILE__,__FUNCTION__,__LINE__)
#define PLL_STEP 62500

HI_S32 tda18250b_init_tuner(HI_U32 u32TunerPort)
{
    s_u32Tda18250ATunerPort = u32TunerPort;
    tmbslFrontEndDependency_t sSrvTunerFunc;
    tmErrorCode_t err = ( CUInt32 )TM_OK;

    TUNER_DEBUG("<0>func = %s, line = %d\n", __FUNCTION__, __LINE__);
    TUNER_DEBUG("<0> TDA18250A_v%d.%d.%d\n", TDA18250A_COMP_NUM, TDA18250A_MAJOR_VER, TDA18250A_MINOR_VER);
    
    {
        /* Low layer struct set-up to link with user written functions */
        sSrvTunerFunc.sIo.Write = tda18250AWrittenI2CWrite;
        sSrvTunerFunc.sIo.Read = tda18250AUserWrittenI2CRead;
        sSrvTunerFunc.sTime.Get = Null;
        sSrvTunerFunc.sTime.Wait = tda18250AUserWrittenWait;
        sSrvTunerFunc.sDebug.Print = tda18250AUserWrittenPrint;
        sSrvTunerFunc.sMutex.Init = tda18250AUserWrittenMutexInit;
        sSrvTunerFunc.sMutex.DeInit = tda18250AUserWrittenMutexDeInit;
        sSrvTunerFunc.sMutex.Acquire = tda18250AUserWrittenMutexAcquire;
        sSrvTunerFunc.sMutex.Release = tda18250AUserWrittenMutexRelease;
        sSrvTunerFunc.dwAdditionalDataSize = 0;
        sSrvTunerFunc.pAdditionalData = Null;

        /* TDA18250A Master Driver low layer setup */
        err = tmbslTDA18250A_Open (u32TunerPort, &sSrvTunerFunc);
        if (err != TM_OK)
        {
            TUNER_DEBUG("tmbslTDA18250A_Open err 0x%x\n",err);
            SK_TRACE ();
            /*goto FAIL;*/
        }

        /* TDA18250A Master Hardware initialization */
        err = tmbslTDA18250A_HwInit (u32TunerPort);
        if (err != TM_OK)
        {   TUNER_DEBUG("tmbslTDA18250A_HwInit err 0x%x\n",err);
            SK_TRACE ();
            /*goto FAIL;*/
        }

        /* TDA18250A Master Hardware power state */
        err = tmbslTDA18250A_SetPowerState (u32TunerPort, tmPowerOn);
        if (err != TM_OK)
        {
           TUNER_DEBUG("tmbslTDA18250A_SetPowerState err 0x%x\n",err);
            SK_TRACE ();
            /*goto FAIL;*/
        }
#if 0

        /* TDA18250A Master standard mode */
        err = tmbslTDA18250A_SetStandardMode (u32TunerPort, TDA18250A_QAM_8MHz);
        if (err != TM_OK)
        {
            TUNER_DEBUG("tmbslTDA18250A_SetStandardMode err 0x%x\n",err);
            SK_TRACE ();
            goto FAIL;
        }
#endif

		tda18250b_set_tuner( u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, 610000);
        msleep(200);

        return HI_SUCCESS;
#if 0

        FAIL:
        tmbslTDA18250A_Close (u32TunerPort);
        //tmbslTDA18250DeInit (1);
        return HI_FAILURE;
#endif
    }
}

HI_S32 tda18250b_set_tuner (HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    HI_S32 s32Timeout = 3;

    tmErrorCode_t err = ( CUInt32 )TM_OK;

    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    TDA18250AIF_Output_Level_t IFLevelMstar = TDA18250A_IF_Output_Level_0_85Vpp_min_7_5_22_5dB;
    TDA18250AStandardMode_t stdModeMaster = TDA18250A_QAM_8MHz;

    UInt32 uRFMaster = u32PuRF * 1000;
    s_u32Tda18250ATunerPort = u32TunerPort;
    if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        //IFLevelMstar = TDA18250A_IF_Output_Level_1Vpp_min_6_24dB;
       
	 switch(g_stTunerOps[u32TunerPort].u32CurrQamMode)
        {
	        case QAM_TYPE_64:                         /*64QAM*/
	            //IFLevelMstar = TDA18250A_IF_Output_Level_0_85Vpp_min_7_5_22_5dB;
		        stdModeMaster = TDA18250A_QAM_6MHz_64QAM;
			
	            break;	       
	        case QAM_TYPE_256:                         /*256QAM*/	            
	            //IFLevelMstar = TDA18250A_IF_Output_Level_1Vpp_min_6_24dB;
	            //IFLevelMstar = TDA18250A_IF_Output_Level_0_5Vpp_min_12_18dB;

		        stdModeMaster = TDA18250A_QAM_6MHz_256QAM;
	            break;
	        default:
	            stdModeMaster = TDA18250A_QAM_6MHz_64QAM;
	            //IFLevelMstar = TDA18250A_IF_Output_Level_0_85Vpp_min_7_5_22_5dB;
	            break;
        }
    }
    else
    {
        switch(g_stTunerOps[u32TunerPort].u32CurrQamMode)
        {
	        case QAM_TYPE_64:                         /*64QAM*/
	            stdModeMaster = TDA18250A_QAM_8MHz_64QAM;
	            uRFMaster += PLL_STEP * 1 / 2;
	            //IFLevelMstar = TDA18250A_IF_Output_Level_1Vpp_min_6_24dB;
	            //IFLevelMstar = TDA18250A_IF_Output_Level_0_85Vpp_min_7_5_22_5dB;
	            break;
	        case QAM_TYPE_128:                         /*128QAM*/
	            stdModeMaster = TDA18250A_QAM_8MHz_64QAM; /*if it is not good then try tmTDA18250_DIG_9MHz_64QAM*/                
	            //IFLevelMstar = TDA18250A_IF_Output_Level_1Vpp_min_6_24dB;
	          
	            break;
	        case QAM_TYPE_256:                         /*256QAM*/
	            stdModeMaster = TDA18250A_QAM_8MHz_256QAM; 
	            //IFLevelMstar = TDA18250A_IF_Output_Level_0_5Vpp_min_12_18dB;
	            break;
	        default:
	            stdModeMaster = TDA18250A_QAM_8MHz_64QAM;
	            //IFLevelMstar = TDA18250A_IF_Output_Level_1Vpp_min_6_24dB;
	            break;
        }
	  
	} 
	IFLevelMstar = TDA18250A_IF_Output_Level_0_5Vpp_min_12_18dB;
#if 0 
    err = tmbslTDA18250A_HwInit(u32TunerPort);
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
    err =tmbslTDA18250A_SetPowerState(u32TunerPort, tmPowerOn);
     if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
#endif
    err = tmbslTDA18250A_SetStandardMode (u32TunerPort, stdModeMaster);
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
		
    err = tmbslTDA18250A_SetIFLevel(u32TunerPort, IFLevelMstar);
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
    err = tmbslTDA18250A_SetRF (u32TunerPort, uRFMaster );
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }

#if 0
    do
    {
        err = tmbslTDA18250A_GetPLLState (u32TunerPort, &PLLLockMaster);
        if (err != TM_OK)
        {
            SK_TRACE ();
            return err;
        }
        
        if (tmbslFrontEndStateLocked == PLLLockMaster)
        {
            //SK_TRACE ();
            return HI_SUCCESS;
        }
        tda_wait(10);
    } while(0 < s32Timeout--);

    SK_TRACE ();
    return HI_FAILURE;
#else
	return HI_SUCCESS;
#endif
}

HI_S32 tda18250b_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    tmErrorCode_t err = ( CUInt32 )TM_OK;
    Int32 s32SignalStrength = 0;
    
    //err = tmbslTDA18250A_GetRSSI(u32TunerPort, &s32SignalStrength);
    err = tmbslTDA18250A_GetPowerLevel(u32TunerPort, &s32SignalStrength);
    if(TM_OK != err)
    {
        SK_TRACE ();
        return err;
    }
    pu32SignalStrength[1] = (HI_U32)s32SignalStrength;
    
    return HI_SUCCESS;
}
HI_S32 tda18250b_resume(HI_U32 u32TunerPort)
{
	s_u32Tda18250ATunerPort = u32TunerPort;
	tmbslFrontEndDependency_t sSrvTunerFunc;
	tmErrorCode_t err = ( CUInt32 )TM_OK;

	TUNER_DEBUG("<0>func = %s, line = %d\n", __FUNCTION__, __LINE__);
	TUNER_DEBUG("<0> TDA18250A_v%d.%d.%d\n", TDA18250A_COMP_NUM, TDA18250A_MAJOR_VER, TDA18250A_MINOR_VER);
	
	{
		/* Low layer struct set-up to link with user written functions */
		sSrvTunerFunc.sIo.Write = tda18250AWrittenI2CWrite;
		sSrvTunerFunc.sIo.Read = tda18250AUserWrittenI2CRead;
		sSrvTunerFunc.sTime.Get = Null;
		sSrvTunerFunc.sTime.Wait = tda18250AUserWrittenWait;
		sSrvTunerFunc.sDebug.Print = tda18250AUserWrittenPrint;
		sSrvTunerFunc.sMutex.Init = tda18250AUserWrittenMutexInit;
		sSrvTunerFunc.sMutex.DeInit = tda18250AUserWrittenMutexDeInit;
		sSrvTunerFunc.sMutex.Acquire = tda18250AUserWrittenMutexAcquire;
		sSrvTunerFunc.sMutex.Release = tda18250AUserWrittenMutexRelease;
		sSrvTunerFunc.dwAdditionalDataSize = 0;
		sSrvTunerFunc.pAdditionalData = Null;

		/* TDA18250A Master Driver low layer setup */
		err = tmbslTDA18250A_Open (u32TunerPort, &sSrvTunerFunc);
		if (err != TM_OK)
		{
			TUNER_DEBUG("tmbslTDA18250A_Open return value 0x%x\n",err);
			/*goto FAIL;*/
		}

		/* TDA18250A Master Hardware initialization */
		err = tmbslTDA18250A_HwInit (u32TunerPort);
		if (err != TM_OK)
		{  
		TUNER_DEBUG("tmbslTDA18250A_HwInit err 0x%x\n",err);
			/*goto FAIL;*/
		}

		/* TDA18250A Master Hardware power state */
		err = tmbslTDA18250A_SetPowerState (u32TunerPort, tmPowerOn);
		if (err != TM_OK)
		{
		   TUNER_DEBUG("tmbslTDA18250A_SetPowerState err 0x%x\n",err);
			/*goto FAIL;*/
		}
#if 0

		/* TDA18250A Master standard mode */
		err = tmbslTDA18250A_SetStandardMode (u32TunerPort, TDA18250A_QAM_8MHz);
		if (err != TM_OK)
		{
			TUNER_DEBUG("tmbslTDA18250A_SetStandardMode err 0x%x\n",err);
		   
			goto FAIL;
		}
#endif

		//tda18250b_set_tuner( u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, 610000);
		//msleep(200);

		return HI_SUCCESS;
#if 0

		FAIL:
		tmbslTDA18250A_Close (u32TunerPort);
		//tmbslTDA18250DeInit (1);
		return HI_FAILURE;
#endif
	}
}


HI_VOID tda18250b_tuner_resume(HI_U32 u32TunerPort)
{
    s_u32Tda18250ATunerPort = u32TunerPort;
    	
    tda18250b_resume(s_u32Tda18250ATunerPort);
    
    return;
}
