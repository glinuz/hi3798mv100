/*
  Copyright (C) 2011 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          main.c
 *
 *
 *
 * \date          %modify_time%
 *
 * \brief         Sample application code of the NXP TDA182I5a driver.
 *
 * REFERENCE DOCUMENTS :
 *
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#include <linux/delay.h>
#include <linux/semaphore.h>

#include "linux/kernel.h"

#include <hi_debug.h>
#include "hi_type.h"
#include "drv_i2c_ext.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "drv_tuner_ioctl.h"
#include "hi3137.h"

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
#include "tmbslTDA182I5a.h"

/**** Device table.  List of device drivers for newlib.	****/
const struct device_table_entry *device_table[] = {
    0,
    0,
    0,
	0 };	/* end of list */

/* It seems that int isatty(int) is no longer provided by the environment, see the
   example lpc2106_hello_rs232_newlib and the various syscalls.c for a workaround if
   the error message "unknown reference to isatty" appears during the link-process.
   (Reason might be the additional configure-option --disable-newlib-supplied-syscalls for 
   newlib which had to be added since the build of newlib 1.14.0 failed when the "supplied" 
   syscalls were not disabled.) */
int isatty(int);

extern int isatty(int fd)
{
	return 1;
}

//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t 	TDA182I5AI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t 	TDA182I5AI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
tmErrorCode_t 	TDA182I5AWait(tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t 	TDA182I5APrint(UInt32 level, const char* format, ...);
tmErrorCode_t  	TDA182I5AMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
tmErrorCode_t  	TDA182I5AMutexDeinit( ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t  	TDA182I5AMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
tmErrorCode_t  	TDA182I5AMutexRelease(ptmbslFrontEndMutexHandle pMutex);


//*--------------------------------------------------------------------------------------
//* Function Name       : Main
//* Object              : Software entry point
//* Input Parameters    : none.
//* Output Parameters   : none.
//*--------------------------------------------------------------------------------------
int tda182i5a_init_tuner(HI_U32 u32TunerPort)
{//* Begin
	
   /* Variable declarations */
   tmErrorCode_t err = TM_OK;
   tmbslFrontEndDependency_t sSrvTunerFunc;

   tmTDA182I5aStandardMode_t stdMode = tmTDA182I5a_DVBT_8MHz;
   UInt32 uRF = 770166000;
   tmbslFrontEndState_t PLLLock = tmbslFrontEndStateUnknown;
   UInt32 uIF = 0;

	/*if (u32TunerPort >= TUNER_NUM)
	{
		return HI_FAILURE;
	}*/
   
   /* Driver layer struct set-up to link with user written functions */
   sSrvTunerFunc.sIo.Write             = TDA182I5AI2CWrite;
   sSrvTunerFunc.sIo.Read              = TDA182I5AI2CRead;
   sSrvTunerFunc.sTime.Get             = Null;
   sSrvTunerFunc.sTime.Wait            = TDA182I5AWait;
   sSrvTunerFunc.sDebug.Print          = TDA182I5APrint;
   sSrvTunerFunc.sMutex.Init           = TDA182I5AMutexInit;
   sSrvTunerFunc.sMutex.DeInit         = TDA182I5AMutexDeinit;
   sSrvTunerFunc.sMutex.Acquire        = TDA182I5AMutexAcquire;
   sSrvTunerFunc.sMutex.Release        = TDA182I5AMutexRelease;
   sSrvTunerFunc.dwAdditionalDataSize  = 0;
   sSrvTunerFunc.pAdditionalData       = Null;
   
   /* TDA182I5a Driver initialization  */
   err = tmbslTDA182I5a_Open(u32TunerPort, &sSrvTunerFunc);
   
   if(err == TM_OK)
   {   
		/* TDA182I5a Hardware initialization */
		err = tmbslTDA182I5a_HwInit(u32TunerPort);
   }
   if(err == TM_OK)
   {
		/* TDA182I5a standard mode selection */
		err = tmbslTDA182I5a_SetStandardMode(u32TunerPort, stdMode);
   }
   if(err == TM_OK)
   {
		/* TDA182I5a RF frequency selection */
		err = tmbslTDA182I5a_SetRF(u32TunerPort, uRF);
   }
   if(err == TM_OK)
   {
		/* Get TDA182I5a PLL Lock status */
		err = tmbslTDA182I5a_GetLockStatus(u32TunerPort, &PLLLock);
   }

   if(err == TM_OK)
   {
		/* Get TDA182I5a IF value */
		err = tmbslTDA182I5a_GetIF(u32TunerPort, &uIF); 
   }
#if 0
   if(err == TM_OK)
   {
		/* DeInitialize TDA182I5a Driver */
		err = tmbslTDA182I5a_Close(0);
   }
#endif
   return err;
	
}//* End

HI_S32 tda182i5a_set_tuner (HI_U32 u32TunerPort, HI_U32 u32PuRF, HI_U32 u32BandWidth)
{
	tmTDA182I5aStandardMode_t stdMode = tmTDA182I5a_DVBT_8MHz;
	tmErrorCode_t err = TM_OK;
	HI_U32 u32RfFreq;

	/*if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }*/
	
	switch(u32BandWidth)
	{
		case 1700:
			stdMode = tmTDA182I5a_DVBT_1_7MHz;
			break;
		case 6000:
			stdMode = tmTDA182I5a_DVBT_6MHz;
			break;
		case 7000:
			stdMode = tmTDA182I5a_DVBT_7MHz;
			break;
		case 8000:
		default:
			stdMode = tmTDA182I5a_DVBT_8MHz;
			break;
	}

	if(err == TM_OK)
	{
		/* TDA182I5a standard mode selection */
		err = tmbslTDA182I5a_SetStandardMode(u32TunerPort, stdMode);
	}

	u32RfFreq = u32PuRF;

	if(err == TM_OK)
   {
		/* TDA182I5a RF frequency selection */
		err = tmbslTDA182I5a_SetRF(u32TunerPort, u32RfFreq);
   }
	
	return HI_SUCCESS;
}

HI_S32 tda182i5a_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
	if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }
	
	return HI_SUCCESS;
}

HI_VOID tda182i5a_tuner_resume(HI_U32 u32TunerPort)
{
	if (u32TunerPort >= TUNER_NUM)
    {
        return ;
    }
	
	return;
}

//*--------------------------------------------------------------------------------------
//* Template of function to be provided by customer
//*--------------------------------------------------------------------------------------

//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5AI2CRead
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 ReadLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t TDA182I5AI2CRead(tmUnitSelect_t tUnit,	UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_FAIL;
	HI_S32 s32Ret = HI_FAILURE;
    HI_U8 u8RegAddr = 0;
    HI_U8 i;

    /* Customer code here */
    /* ...*/
	u8RegAddr = *pAddr;

    for (i = 0; i < 10; i++)
    {
        //status = HI_DRV_I2C_Read(g_MxlI2cChnNum, (HI_U8)I2C_Addr, u32RegAddr, 2, DataPtr, 1);
        hi3137_config_i2c_out((HI_U32)tUnit);

        if (g_stTunerOps[tUnit].enI2cChannel < HI_STD_I2C_NUM)
        {
            s32Ret = s_tuner_pI2cFunc->pfnI2cRead(g_stTunerOps[tUnit].enI2cChannel, g_stTunerOps[tUnit].u32TunerAddress, u8RegAddr, 1, pData, ReadLen);
        }
        else
        {
            s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(g_stTunerOps[tUnit].enI2cChannel, g_stTunerOps[tUnit].u32TunerAddress, u8RegAddr, 1, pData, ReadLen);
        }
		
        if (HI_SUCCESS == s32Ret)
        {
            return TM_OK;
        }
    }

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5AI2CWrite
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 WriteLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t TDA182I5AI2CWrite(tmUnitSelect_t tUnit, 	UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_FAIL;
	HI_S32	s32Ret = HI_FAILURE;
	TUNER_I2C_DATA_S stI2cData;
    HI_U8 pArray[200],i;

    /* Customer code here */
    /* ...*/
    pArray[0] = *pAddr;
    memcpy(&(pArray[1]), pData,WriteLen);

    stI2cData.pu8ReceiveBuf = NULL;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = pArray;
    stI2cData.u32SendLength = WriteLen+1;
    for(i=0;i<10;i++)
    {
        hi3137_config_i2c_out((HI_U32)tUnit);
        s32Ret = tuner_i2c_send_data(g_stTunerOps[tUnit].enI2cChannel,g_stTunerOps[tUnit].u32TunerAddress,&stI2cData);
        if(!s32Ret)
            return TM_OK;
    }

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5AWait
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t TDA182I5AWait(tmUnitSelect_t tUnit, UInt32 tms)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
	tuner_mdelay(tms);
    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5APrint
//* Object              : 
//* Input Parameters    : 	UInt32 level, const char* format, ...
//* 						
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t 			TDA182I5APrint(UInt32 level, const char* format, ...)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
    //pq_print(format);
    /* ...*/
    /* End of Customer code here */

    return err;
}

struct semaphore tda182I5A_sem;
//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5AMutexInit
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  TDA182I5AMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
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
//* Function Name       : TDA182I5AMutexDeinit
//* Object              : 
//* Input Parameters    : 	 ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  TDA182I5AMutexDeinit( ptmbslFrontEndMutexHandle pMutex)
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
//* Function Name       : TDA182I5AMutexAcquire
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  TDA182I5AMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
	err = down_interruptible (&tda182I5A_sem);
    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : TDA182I5AMutexRelease
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  TDA182I5AMutexRelease(ptmbslFrontEndMutexHandle pMutex)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
	up (&tda182I5A_sem);
    /* ...*/
    /* End of Customer code here */

    return err;
}


