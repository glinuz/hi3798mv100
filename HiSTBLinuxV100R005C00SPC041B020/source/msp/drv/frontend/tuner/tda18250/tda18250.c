#include <linux/delay.h>
#include <linux/semaphore.h>

#include "linux/kernel.h"

#include <hi_debug.h>
#include "hi_type.h"
#include "drv_i2c_ext.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

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
#include "tmddTDA18250.h"
#include "tmbslTDA18250.h"

static HI_U32 s_u32Tda18250TunerPort = 0;

static Bool tda_read (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
static Bool tda_write (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
static Bool tda_wait (UInt32 ms);
//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CRead (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 ReadLen, UInt8 * pData);
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize,
	UInt8 * pAddr, UInt32 WriteLen, UInt8 * pData);
tmErrorCode_t UserWrittenWait (tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t UserWrittenPrint (UInt32 level, const char *format, ...);
tmErrorCode_t UserWrittenMutexInit (ptmbslFrontEndMutexHandle * ppMutexHandle);
tmErrorCode_t UserWrittenMutexDeInit (ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t UserWrittenMutexAcquire (ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
tmErrorCode_t UserWrittenMutexRelease (ptmbslFrontEndMutexHandle pMutex);

//*--------------------------------------------------------------------------------------
//* Template of function to be provided by customer
//*--------------------------------------------------------------------------------------

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CRead
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CRead (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 ReadLen, UInt8 * pData)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

	err = tda_read (g_stTunerOps[s_u32Tda18250TunerPort].u32TunerAddress, *pAddr, ReadLen, pData);
/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWrite
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8 * pAddr, UInt32 WriteLen, UInt8 * pData)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

	err = tda_write (g_stTunerOps[s_u32Tda18250TunerPort].u32TunerAddress, *pAddr, WriteLen, pData);

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenWait (tmUnitSelect_t tUnit, UInt32 tms)
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
//* Function Name       : UserWrittenPrint
//* Object              : 
//* Input Parameters    :   UInt32 level, const char* format, ...
//*                         
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------

tmErrorCode_t UserWrittenPrint (UInt32 level, const char *format, ...)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

	return err;
}

struct semaphore tda18250_sem;
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexInit
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenMutexInit (ptmbslFrontEndMutexHandle * ppMutexHandle)
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
//* Function Name       : UserWrittenMutexDeInit
//* Object              : 
//* Input Parameters    :    ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenMutexDeInit (ptmbslFrontEndMutexHandle pMutex)
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
//* Function Name       : UserWrittenMutexAcquire
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenMutexAcquire (ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/
	err = down_interruptible (&tda18250_sem);

/* ...*/
/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              : 
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenMutexRelease (ptmbslFrontEndMutexHandle pMutex)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

	up (&tda18250_sem);

/* ...*/
/* End of Customer code here */

	return err;
}

//////////////////////////////////////////////////////////////

#define TDA_I2C_ADDR    (0xC0)
#define SITUNER_TRACE_LEVEL 8

static Bool tda_write (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff)
{
	HI_U8  i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	HI_U8 au8SendData[65] = { 0 };
	HI_U8 *pu8Tmp = NULL;


	if (NULL == pDataBuff)
	{
		HI_ERR_TUNER( "pointer is null\n");
		return HI_FAILURE;
	}

	qam_config_i2c_out (s_u32Tda18250TunerPort, 1);

	pu8Tmp = pDataBuff;
	au8SendData[0] = uSubAddress;
	for (i = 0; i < uNbData; i++)
	{
		au8SendData[i + 1] = *pu8Tmp;;
		pu8Tmp++;
	}

	stI2cDataStr.pu8ReceiveBuf = 0;
	stI2cDataStr.u32ReceiveLength = 0;
	stI2cDataStr.pu8SendBuf = au8SendData;
	stI2cDataStr.u32SendLength = uNbData + 1;	//subaddress + data
	s32Ret = tuner_i2c_send_data (g_stTunerOps[s_u32Tda18250TunerPort].enI2cChannel, uAddress, &stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[tda18251 error]%s %d IIC write err!status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;

}

static Bool tda_read (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff)
{
	HI_S32 i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	HI_U8 au8RcvData[65] = { 0 };


	qam_config_i2c_out (s_u32Tda18250TunerPort, 1);
	memset ((void *) au8RcvData, 0, sizeof (au8RcvData));

	stI2cDataStr.pu8ReceiveBuf = au8RcvData;
	stI2cDataStr.pu8SendBuf = &uSubAddress;
	stI2cDataStr.u32ReceiveLength = uNbData;
	stI2cDataStr.u32SendLength = 1;

	s32Ret = tuner_i2c_receive_data (g_stTunerOps[s_u32Tda18250TunerPort].enI2cChannel, uAddress /*|0x01 */ ,
		&stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[tda18251 error]%s %d IIC read err! status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
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
	msleep(ms);

	return HI_SUCCESS;
}

#define SK_TRACE() HI_INFO_TUNER("__file__:%s,__function__:%s, __line__:%d\n", __FILE__,__FUNCTION__,__LINE__)
#define PLL_STEP 62500

HI_S32 tda18250_init_tuner(HI_U32 u32TunerPort)
{    
    tmbslFrontEndDependency_t sSrvTunerFunc;
    tmErrorCode_t err = ( CUInt32 )TM_OK;

        s_u32Tda18250TunerPort = u32TunerPort;

    {
        /* Low layer struct set-up to link with user written functions */
        sSrvTunerFunc.sIo.Write = UserWrittenI2CWrite;
        sSrvTunerFunc.sIo.Read = UserWrittenI2CRead;
        sSrvTunerFunc.sTime.Get = Null;
        sSrvTunerFunc.sTime.Wait = UserWrittenWait;
        sSrvTunerFunc.sDebug.Print = UserWrittenPrint;
        sSrvTunerFunc.sMutex.Init = UserWrittenMutexInit;
        sSrvTunerFunc.sMutex.DeInit = UserWrittenMutexDeInit;
        sSrvTunerFunc.sMutex.Acquire = UserWrittenMutexAcquire;
        sSrvTunerFunc.sMutex.Release = UserWrittenMutexRelease;
        sSrvTunerFunc.dwAdditionalDataSize = 0;
        sSrvTunerFunc.pAdditionalData = Null;

        /* TDA18250 Master Driver low layer setup */
        err = tmbslTDA18250Init (u32TunerPort, &sSrvTunerFunc);
        if (err != TM_OK)
        {
            SK_TRACE ();
            /*goto FAIL;*/
        }

        /* TDA18250 Master Hardware initialization */
        err = tmbslTDA18250Reset (u32TunerPort);
        if (err != TM_OK)
        {
            SK_TRACE ();
            /*goto FAIL;*/
        }

        /* TDA18250 Master Hardware power state */
        err = tmbslTDA18250SetPowerState (u32TunerPort, tmTDA18250_PowerNormalMode);
        if (err != TM_OK)
        {
            SK_TRACE ();
            /*goto FAIL;*/
        }


        /* TDA18250 Master standard mode */
        err = tmbslTDA18250SetStandardMode (u32TunerPort, tmTDA18250_DIG_9MHz_64QAM);
        if (err != TM_OK)
        {
            SK_TRACE ();
            goto FAIL;
        }

        return HI_SUCCESS;

        FAIL:
        tmbslTDA18250DeInit (u32TunerPort);
        return HI_FAILURE;
    }
}

HI_S32 tda18250_set_tuner (HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    HI_S32 s32Timeout = 3;

    tmErrorCode_t err = ( CUInt32 )TM_OK;

    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    tmddTDA18250IF_Level_t IFLevelMstar = tmddTDA18250_IF_Level_1V;
    tmTDA18250StandardMode_t stdModeMaster = tmTDA18250_DIG_9MHz_64QAM;

    UInt32 uRFMaster = u32PuRF * 1000;
    s_u32Tda18250TunerPort = u32TunerPort;

    if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        IFLevelMstar = tmddTDA18250_IF_Level_1V;
        stdModeMaster = tmTDA18250_DIG_6MHz;
    }
    else
    {
        switch(g_stTunerOps[u32TunerPort].u32CurrQamMode)
        {
            case QAM_TYPE_64:                         /*64QAM*/
                stdModeMaster = tmTDA18250_DIG_9MHz_64QAM;
                uRFMaster += PLL_STEP * 1 / 2;
                IFLevelMstar = tmddTDA18250_IF_Level_1V;
                break;
            case QAM_TYPE_128:                         /*128QAM*/
                stdModeMaster = tmTDA18250_DIG_9MHz_256QAM; /*if it is not good then try tmTDA18250_DIG_9MHz_64QAM*/                
                IFLevelMstar = tmddTDA18250_IF_Level_1V;
                break;
            case QAM_TYPE_256:                         /*256QAM*/
                stdModeMaster = tmTDA18250_DIG_9MHz_256QAM; 
                IFLevelMstar = tmddTDA18250_IF_Level_0_5V;
                break;
            default:
                stdModeMaster = tmTDA18250_DIG_9MHz_64QAM;
                IFLevelMstar = tmddTDA18250_IF_Level_1V;
                break;
        }
    }

    err = tmbslTDA18250SetStandardMode (u32TunerPort, stdModeMaster);
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
		
    err = tmbslTDA18250SetIfLevel(u32TunerPort, IFLevelMstar);
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
		
    err = tmbslTDA18250SetRf (u32TunerPort, uRFMaster );
    if (err != TM_OK)
    {
        SK_TRACE ();
        return err;
    }
		
    do
    {
        err = tmbslTDA18250GetLockStatus (u32TunerPort, &PLLLockMaster);
        if (err != TM_OK)
        {
            SK_TRACE ();
            return err;
        }
        
        if (tmbslFrontEndStateLocked == PLLLockMaster)
        {
            SK_TRACE ();
            return HI_SUCCESS;
        }
        tda_wait(10);
    } while(0 < s32Timeout--);

    SK_TRACE ();
    return HI_FAILURE;
}

HI_S32 tda18250_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    tmErrorCode_t err = ( CUInt32 )TM_OK;
    Int32 s32SignalStrength = 0;
    
    err = tmbslTDA18250GetPowerLevel(u32TunerPort, &s32SignalStrength);
    if(TM_OK != err)
    {
        SK_TRACE ();
        return err;
    }
    if(s32SignalStrength < 0)
    {
        pu32SignalStrength[2] = 1;
        pu32SignalStrength[1] = (HI_U32)((HI_U32)0-s32SignalStrength);
    }
    else
    {
        pu32SignalStrength[2] = 0;
        pu32SignalStrength[1] = (HI_U32)s32SignalStrength;
    }
    
    return HI_SUCCESS;
}

HI_VOID tda18250_tuner_resume(HI_U32 u32TunerPort)
{
    s_u32Tda18250TunerPort = u32TunerPort;
    tda18250_init_tuner(s_u32Tda18250TunerPort);
    
    return;
}
